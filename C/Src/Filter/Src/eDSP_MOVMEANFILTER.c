/**
 * @file       eDSP_MOVMEANFILTER.c
 *
 * @brief      Mean filter implementation
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eDSP_MOVMEANFILTER.h"
#include "eDSP_MAXCHECK.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eDSP_MOVMEANFILTER_IsStatusStillCoherent(t_eDSP_MOVMEANFILTER_Ctx* const p_ptCtx);
static e_eDSP_MOVMEANFILTER_RES eDSP_MOVMEANFILTER_MaxCheckResToMED(const e_eDSP_MAXCHECK_RES p_tMaxRet);


/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eDSP_MOVMEANFILTER_RES eDSP_MOVMEANFILTER_InitCtx(t_eDSP_MOVMEANFILTER_Ctx* const p_ptCtx, int64_t* p_piWindowsBuffer,
                                              uint32_t p_uWindowsBuffLen)
{
	/* Local variable */
	e_eDSP_MOVMEANFILTER_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_piWindowsBuffer ) )
	{
		l_eRes = e_eDSP_MOVMEANFILTER_RES_BADPOINTER;
	}
	else
	{
		/* Check data validity */
		if( p_uWindowsBuffLen <= 2u )
		{
			l_eRes = e_eDSP_MOVMEANFILTER_RES_BADPARAM;
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
			l_eRes = e_eDSP_MOVMEANFILTER_RES_OK;
		}
	}

	return l_eRes;
}

e_eDSP_MOVMEANFILTER_RES eDSP_MOVMEANFILTER_IsInit(t_eDSP_MOVMEANFILTER_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eDSP_MOVMEANFILTER_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eDSP_MOVMEANFILTER_RES_BADPOINTER;
	}
	else
	{
        *p_pbIsInit = p_ptCtx->bIsInit;
        l_eRes = e_eDSP_MOVMEANFILTER_RES_OK;
	}

	return l_eRes;
}

e_eDSP_MOVMEANFILTER_RES eDSP_MOVMEANFILTER_InsertValueAndCalculate(t_eDSP_MOVMEANFILTER_Ctx* const p_ptCtx,
                                                                  const int64_t p_iValue, int64_t* const p_pFilteredVal)
{
	/* Local variable for return */
	e_eDSP_MOVMEANFILTER_RES l_eRes;
	e_eDSP_MAXCHECK_RES l_eMaxRes;

	/* Local variable for calculation */
	uint32_t l_uCnt;
	int64_t  l_iSum;
	int64_t  l_iNearest;
	int64_t  l_iNearestDiff;
	int64_t  l_iCurrDiff;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pFilteredVal ) )
	{
		l_eRes = e_eDSP_MOVMEANFILTER_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eDSP_MOVMEANFILTER_RES_NOINITLIB;
		}
		else
		{
            /* Check data coherence */
            if( false == eDSP_MOVMEANFILTER_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eDSP_MOVMEANFILTER_RES_CORRUPTCTX;
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
					l_eRes = e_eDSP_MOVMEANFILTER_RES_NEEDSMOREVALUE;
				}
				else
				{
					/* The window is full */
					l_eRes = e_eDSP_MOVMEANFILTER_RES_OK;
					l_uCnt = 0u;
					l_iSum = 0u;

					/* Calculate the factibility of the sum for the means */
					while( ( e_eDSP_MOVMEANFILTER_RES_OK == l_eRes ) && ( l_uCnt < p_ptCtx->uWindowsLen ) )
					{
						l_eMaxRes = eDSP_MAXCHECK_SUMI64Check(l_iSum, p_ptCtx->piWindowsBuffer[l_uCnt]);
						l_eRes = eDSP_MOVMEANFILTER_MaxCheckResToMED(l_eMaxRes);

						if( e_eDSP_MOVMEANFILTER_RES_OK == l_eRes )
						{
							l_uCnt++;
							l_iSum += p_ptCtx->piWindowsBuffer[l_uCnt];
						}
					}

					if( e_eDSP_MOVMEANFILTER_RES_OK == l_eRes )
					{
						/* Calculate and return the mean */
						*p_pFilteredVal = l_iSum / p_ptCtx->uWindowsLen;
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
static bool_t eDSP_MOVMEANFILTER_IsStatusStillCoherent(t_eDSP_MOVMEANFILTER_Ctx* const p_ptCtx)
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

static e_eDSP_MOVMEANFILTER_RES eDSP_MOVMEANFILTER_MaxCheckResToMED(const e_eDSP_MAXCHECK_RES p_tMaxRet)
{
	e_eDSP_MOVMEANFILTER_RES l_eRet;

	if( e_eDSP_MAXCHECK_RES_OK == p_tMaxRet )
	{
		l_eRet = e_eDSP_MOVMEANFILTER_RES_OK;
	}
	else
	{
		l_eRet = e_eDSP_MOVMEANFILTER_RES_OVERFLOW;
	}

	return l_eRet;
}
