/**
 * @file       eDSP_FOHPASSFILTER.c
 *
 * @brief      Median filter implementation
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eDSP_FOHPASSFILTER.h"
#include "eDSP_MAXCHECK.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eDSP_FOHPASSFILTER_IsStatusStillCoherent(t_eDSP_FOHPASSFILTER_Ctx* const p_ptCtx);
static e_eDSP_FOHPASSFILTER_RES eDSP_FOHPASSFILTER_MaxCheckResToMED(const e_eDSP_MAXCHECK_RES p_tMaxRet);


/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eDSP_FOHPASSFILTER_RES eDSP_FOHPASSFILTER_InitCtx(t_eDSP_FOHPASSFILTER_Ctx* const p_ptCtx, int64_t* p_piWindowsBuffer,
                                                  uint32_t p_uWindowsBuffLen)
{
	/* Local variable */
	e_eDSP_FOHPASSFILTER_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_piWindowsBuffer ) )
	{
		l_eRes = e_eDSP_FOHPASSFILTER_RES_BADPOINTER;
	}
	else
	{
		/* Check data validity */
		if( p_uWindowsBuffLen <= 2u )
		{
			l_eRes = e_eDSP_FOHPASSFILTER_RES_BADPARAM;
		}
		else
		{
			/* Initialize internal status */
			p_ptCtx->bIsInit = true;
			p_ptCtx->uWindowsLen = p_uWindowsBuffLen;
			p_ptCtx->uFilledData = 0u;
			p_ptCtx->uCurDataLocation = 0u;
			memset(p_piWindowsBuffer, 0, sizeof(int64_t));

			/* All OK */
			l_eRes = e_eDSP_FOHPASSFILTER_RES_OK;
		}
	}

	return l_eRes;
}

e_eDSP_FOHPASSFILTER_RES eDSP_FOHPASSFILTER_IsInit(t_eDSP_FOHPASSFILTER_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eDSP_FOHPASSFILTER_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eDSP_FOHPASSFILTER_RES_BADPOINTER;
	}
	else
	{
        *p_pbIsInit = p_ptCtx->bIsInit;
        l_eRes = e_eDSP_FOHPASSFILTER_RES_OK;
	}

	return l_eRes;
}

e_eDSP_FOHPASSFILTER_RES eDSP_FOHPASSFILTER_InsertValueAndCalculate(t_eDSP_FOHPASSFILTER_Ctx* const p_ptCtx,
                                                                  const int64_t p_iValue, int64_t* const p_pFilteredVal)
{
	/* Local variable for return */
	e_eDSP_FOHPASSFILTER_RES l_eRes;
	e_eDSP_MAXCHECK_RES l_eMaxRes;

	/* Local variable for calculation */
	uint32_t l_uCnt;
	int64_t l_iSum;
	int64_t l_iMean;
	int64_t l_iNearest;
	int64_t l_iNearestDiff;
	int64_t l_iCurrDiff;


	/* To calculate the Low pass filter we will use an RC circuit.

	   -----| |-------------
	   				 /
		Vin			 \    Vout
					 /
					 \
	   ---------------------
	   The equantion of the circuit is:
	   Vout(ti) = R * Ir(ti) -> Vout(ti) = R * C * ( dVin(t)/ dt - dVout(i)/dt )
	   and using discrete time we have:
	   Vout(ti) = R * C * ( Vin(i)-Vin(i-1)/( t(i)-t(i-1) ) - Vout(i)-Vout(i-1)/( t(i)-t(i-1) ) )
	   Vout(ti) =  ( RC / ( t(i)-t(i-1) ) ) * ( Vout(i-1) + Vin(i)-Vin(i-1) )
	   Doing other math in the frequency domains we found out that the cutoff frequency is Fc = 1 / ( 2 pi RC )
	   and so the value of RC = 1 / ( 2 pi Fc )
	*/



	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pFilteredVal ) )
	{
		l_eRes = e_eDSP_FOHPASSFILTER_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eDSP_FOHPASSFILTER_RES_NOINITLIB;
		}
		else
		{
            /* Check data coherence */
            if( false == eDSP_FOHPASSFILTER_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eDSP_FOHPASSFILTER_RES_CORRUPTCTX;
            }
			else
			{
				/* Insert data */
				p_ptCtx->piWindowsBuffer[p_ptCtx->uCurDataLocation] = p_iValue;
				p_ptCtx->uCurDataLocation++;

				/* Manage rolback */
				if( p_ptCtx->uCurDataLocation >= p_ptCtx->uWindowsLen )
				{
					p_ptCtx->uCurDataLocation = 0u;
				}

				if( p_ptCtx->uFilledData < ( p_ptCtx->uWindowsLen - 1u ) )
				{
					/* Increase filler counter */
					p_ptCtx->uFilledData++;

					/* Need more data */
					l_eRes = e_eDSP_FOHPASSFILTER_RES_NEEDSMOREVALUE;
				}
				else
				{
					/* The window is full */
					l_eRes = e_eDSP_FOHPASSFILTER_RES_OK;
					l_uCnt = 0u;
					l_iSum = 0u;

					/* Calculate the factibility of the sum for the means */
					while( ( e_eDSP_FOHPASSFILTER_RES_OK == l_eRes ) && ( l_uCnt < p_ptCtx->uWindowsLen ) )
					{
						l_eMaxRes = eDSP_MAXCHECK_SUMI64Check(l_iSum, p_ptCtx->piWindowsBuffer[l_uCnt]);
						l_eRes = eDSP_FOHPASSFILTER_MaxCheckResToMED(l_eMaxRes);

						if( e_eDSP_FOHPASSFILTER_RES_OK == l_eRes )
						{
							l_uCnt++;
							l_iSum += p_ptCtx->piWindowsBuffer[l_uCnt];
						}
					}

					if( e_eDSP_FOHPASSFILTER_RES_OK == l_eRes )
					{
						/* Calculate the mean */
						l_iMean = l_iSum / p_ptCtx->uWindowsLen;

						/* re-init counter */
						l_uCnt = 0u;

						/* search for the nearest one */
						while( ( e_eDSP_FOHPASSFILTER_RES_OK == l_eRes ) && ( l_uCnt < p_ptCtx->uWindowsLen ) )
						{
							l_eMaxRes = eDSP_MAXCHECK_SUBTI64Check(l_iMean, p_ptCtx->piWindowsBuffer[l_uCnt]);
							l_eRes = eDSP_FOHPASSFILTER_MaxCheckResToMED(l_eMaxRes);

							if( e_eDSP_FOHPASSFILTER_RES_OK == l_eRes )
							{
								/* Calc diff */
								l_iCurrDiff = l_iMean - p_ptCtx->piWindowsBuffer[l_uCnt];

								/* abs of the difference */
								if( l_iCurrDiff < 0u )
								{
									l_iCurrDiff = -l_iCurrDiff;
								}

								/* On first round init default variable */
								if( 0u == l_uCnt )
								{
									/* On first round init default variable */
									l_iNearest = p_ptCtx->piWindowsBuffer[l_uCnt];
									l_iNearestDiff = l_iCurrDiff;
								}
								else
								{
									/* Compare with the alredy founded */
									if( l_iCurrDiff < l_iNearestDiff )
									{
										l_iNearestDiff = p_ptCtx->piWindowsBuffer[l_uCnt];
									}
								}
							}
						}

						/* if all ok return value */
						if( e_eDSP_FOHPASSFILTER_RES_OK == l_eRes )
						{
							*p_pFilteredVal = l_iNearest;
						}
					}
				}
			}
		}
    }

	return l_eRes;
}



/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
static bool_t eDSP_FOHPASSFILTER_IsStatusStillCoherent(t_eDSP_FOHPASSFILTER_Ctx* const p_ptCtx)
{
    /* Return local var */
    bool_t l_eRes;

	/* Check pointer validity */
	if( NULL == p_ptCtx->piWindowsBuffer )
	{
		l_eRes = false;
	}
    else
    {
		/* Check data validity */
		if( ( p_ptCtx->uWindowsLen <= 2u ) || ( p_ptCtx->uFilledData > p_ptCtx->uWindowsLen ) ||
			( p_ptCtx->uCurDataLocation >= p_ptCtx->uWindowsLen )  )
		{
			l_eRes = false;
		}
		else
		{
			/* Check data validity */
			if( ( p_ptCtx->uFilledData < p_ptCtx->uWindowsLen ) && ( p_ptCtx->uCurDataLocation >= p_ptCtx->uFilledData ) )
			{
				l_eRes = false;
			}
			else
			{
				l_eRes = true;
			}
		}
    }

    return l_eRes;
}

static e_eDSP_FOHPASSFILTER_RES eDSP_FOHPASSFILTER_MaxCheckResToMED(const e_eDSP_MAXCHECK_RES p_tMaxRet)
{
	e_eDSP_FOHPASSFILTER_RES l_eRet;

	if( e_eDSP_MAXCHECK_RES_OK == p_tMaxRet )
	{
		l_eRet = e_eDSP_FOHPASSFILTER_RES_OK;
	}
	else
	{
		l_eRet = e_eDSP_FOHPASSFILTER_RES_OVERFLOW;
	}

	return l_eRet;
}
