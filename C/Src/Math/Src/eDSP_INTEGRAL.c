/**
 * @file       eDSP_INTEGRAL.h
 *
 * @brief      Integral calculation
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eDSP_INTEGRAL.h"
#include "eDSP_MAXCHECK.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eDSP_INTEGRAL_IsStatusStillCoherent(t_eDSP_INTEGRAL_Ctx* const p_ptCtx);
static e_eDSP_INTEGRAL_RES eDSP_INTEGRAL_MaxCheckResToINTEGRAL(const e_eDSP_MAXCHECK_RES p_tMaxRet);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eDSP_INTEGRAL_RES eDSP_INTEGRAL_InitCtx(t_eDSP_INTEGRAL_Ctx* const p_ptCtx)
{
	/* Local variable */
	e_eDSP_INTEGRAL_RES l_eRes;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eDSP_INTEGRAL_RES_BADPOINTER;
	}
	else
	{
        /* Initialize internal status */
        p_ptCtx->bIsInit = true;
        p_ptCtx->bHasPrev = false;
		p_ptCtx->bHasCurrent = false;
		p_ptCtx->uPreviousVal = 0;
		p_ptCtx->uCurrentVal = 0;
		p_ptCtx->uIntegral   = 0;
		p_ptCtx->uTimeElapsFromCurToPre = 0u;

		/* All OK */
        l_eRes = e_eDSP_INTEGRAL_RES_OK;
	}

	return l_eRes;
}

e_eDSP_INTEGRAL_RES eDSP_INTEGRAL_IsInit(t_eDSP_INTEGRAL_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eDSP_INTEGRAL_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eDSP_INTEGRAL_RES_BADPOINTER;
	}
	else
	{
        *p_pbIsInit = p_ptCtx->bIsInit;
        l_eRes = e_eDSP_INTEGRAL_RES_OK;
	}

	return l_eRes;
}

e_eDSP_INTEGRAL_RES eDSP_INTEGRAL_InsertValueAndCalcIntegral(t_eDSP_INTEGRAL_Ctx* const p_ptCtx, const int64_t p_iValue,
                                                             const uint32_t p_timeFromLast, int64_t* const p_piIntegral)
{
	/* Local variable for return */
	e_eDSP_INTEGRAL_RES l_eRes;
	e_eDSP_MAXCHECK_RES l_eMaxRes;

	/* Local Variable for calculation */
	uint64_t l_uDeltaIncr;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_piIntegral ) )
	{
		l_eRes = e_eDSP_INTEGRAL_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eDSP_INTEGRAL_RES_NOINITLIB;
		}
		else
		{
            /* Check data coherence */
            if( false == eDSP_INTEGRAL_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eDSP_INTEGRAL_RES_CORRUPTCTX;
            }
			else
			{
                /* Check data validity */
				if( 0u == p_timeFromLast )
				{
					l_eRes = e_eDSP_INTEGRAL_RES_BADPARAM;
				}
				else
				{
					/* All ok */
					l_eRes = e_eDSP_INTEGRAL_RES_OK;

					/* Insert data */
					p_ptCtx->uPreviousVal = p_ptCtx->uCurrentVal;
					p_ptCtx->uCurrentVal = p_iValue;
					p_ptCtx->uTimeElapsFromCurToPre = p_timeFromLast;

					if( false == p_ptCtx->bHasCurrent )
					{
						/* first entry */
						p_ptCtx->bHasCurrent = true;
					}
					else
					{
						if( false == p_ptCtx->bHasPrev )
						{
							/* First time adding data */
							p_ptCtx->bHasPrev = true;
						}
					}

					/* Check if we can proceed with calculation */
					if( false == p_ptCtx->bHasPrev )
					{
						/* Request more data */
						l_eRes = e_eDSP_INTEGRAL_RES_NEEDSMOREVALUE;
					}
					else
					{
						/* the integral operation is defined as follow:
						* Sx   -> lim of h -> 0 of the function: SUM[ f(x)  * ( xh - x) ]
						* Dx   -> lim of h -> 0 of the function: SUM[ f(xh) * ( xh - x) ]
						* Mean -> lim of h -> 0 of the function: SUM[ ( (f(xh) - f(x))/2 ) * ( xh - x) ]
						* when h is pretty small we can have a good aproximation
						* --> our fun [vSx]:   Integral = Integral + previousvalue * timeelapsed
						* --> our fun [vDx]:   Integral = Integral + currentvalue  * timeelapsed
						* --> our fun [vMean]: Integral = Integral + ((currentvalue + previousvalue)/2) * timeelapsed
						* */

						l_eMaxRes = eDSP_MAXCHECK_MOLTIPI64Check(p_ptCtx->uCurrentVal, p_ptCtx->uTimeElapsFromCurToPre);
						l_eRes = eDSP_INTEGRAL_MaxCheckResToINTEGRAL(l_eMaxRes);

						if( e_eDSP_INTEGRAL_RES_OK == l_eRes )
						{
							l_uDeltaIncr = p_ptCtx->uCurrentVal * p_ptCtx->uTimeElapsFromCurToPre;

							l_eMaxRes = eDSP_MAXCHECK_SUMI64Check(p_ptCtx->uIntegral, l_uDeltaIncr);
							l_eRes = eDSP_INTEGRAL_MaxCheckResToINTEGRAL(l_eMaxRes);

							if( e_eDSP_INTEGRAL_RES_OK == l_eRes )
							{
								/* calculate */
								p_ptCtx->uIntegral = p_ptCtx->uIntegral + l_uDeltaIncr;
								*p_piIntegral = p_ptCtx->uIntegral;
							}
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
static bool_t eDSP_INTEGRAL_IsStatusStillCoherent(t_eDSP_INTEGRAL_Ctx* const p_ptCtx)
{
    /* Return local var */
    bool_t l_eRes;

	if( false == p_ptCtx->bHasCurrent )
	{
		/* No current value, no data present in the system */
		if( ( true == p_ptCtx->bHasPrev ) || ( 0 != p_ptCtx->uPreviousVal ) || ( 0 != p_ptCtx->uCurrentVal ) ||
			( 0 != p_ptCtx->uTimeElapsFromCurToPre ) || ( 0 != p_ptCtx->uIntegral ) )
		{
			l_eRes = false;
		}
		else
		{
			l_eRes = true;
		}
	}
	else
	{
		/* has current value */
		if( false == p_ptCtx->bHasPrev )
		{
			/* No previous value */
			if( ( 0 != p_ptCtx->uPreviousVal ) || ( 0 != p_ptCtx->uTimeElapsFromCurToPre ) )
			{
				l_eRes = false;
			}
			else
			{
				if( p_ptCtx->uCurrentVal != p_ptCtx->uIntegral )
				{
					l_eRes = false;
				}
				else
				{
					l_eRes = true;
				}
			}
		}
		else
		{
			/* Has even a previous value */
			if( 0 != p_ptCtx->uTimeElapsFromCurToPre )
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

static e_eDSP_INTEGRAL_RES eDSP_INTEGRAL_MaxCheckResToINTEGRAL(const e_eDSP_MAXCHECK_RES p_tMaxRet)
{
	e_eDSP_INTEGRAL_RES l_eRet;

	if( e_eDSP_MAXCHECK_RES_OK == p_tMaxRet )
	{
		l_eRet = e_eDSP_INTEGRAL_RES_OK;
	}
	else
	{
		l_eRet = e_eDSP_INTEGRAL_RES_OVERFLOW;
	}

	return l_eRet;
}
