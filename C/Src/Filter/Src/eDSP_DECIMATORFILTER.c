/**
 * @file       eDSP_DERIVATIVE.h
 *
 * @brief      Multiple 2D point Linearization on a int64_t
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eDSP_DERIVATIVE.h"
#include "eDSP_MAXCHECK.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eDSP_DERIVATIVE_IsStatusStillCoherent(t_eDSP_DERIVATIVE_Ctx* const p_ptCtx);
static e_eDSP_DERIVATIVE_RES eDSP_DERIVATIVE_MaxCheckResToS2DP(const e_eDSP_MAXCHECK_RES p_tMaxRet);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eDSP_DERIVATIVE_RES eDSP_DERIVATIVE_InitCtx(t_eDSP_DERIVATIVE_Ctx* const p_ptCtx)
{
	/* Local variable */
	e_eDSP_DERIVATIVE_RES l_eRes;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eDSP_DERIVATIVE_RES_BADPOINTER;
	}
	else
	{
        /* Initialize internal status */
        p_ptCtx->bIsInit = true;
        p_ptCtx->bHasPrev = false;
		p_ptCtx->bHasCurrent = false;
		p_ptCtx->uPreviousVal = 0;
		p_ptCtx->uCurrentVal = 0;
		p_ptCtx->uTimeElapsedFromCurToPre = 0u;

		/* All OK */
        l_eRes = e_eDSP_DERIVATIVE_RES_OK;
	}

	return l_eRes;
}

e_eDSP_DERIVATIVE_RES eDSP_DERIVATIVE_IsInit(t_eDSP_DERIVATIVE_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eDSP_DERIVATIVE_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eDSP_DERIVATIVE_RES_BADPOINTER;
	}
	else
	{
        *p_pbIsInit = p_ptCtx->bIsInit;
        l_eRes = e_eDSP_DERIVATIVE_RES_OK;
	}

	return l_eRes;
}

e_eDSP_DERIVATIVE_RES eDSP_DERIVATIVE_InsertValue(t_eDSP_DERIVATIVE_Ctx* const p_ptCtx, const int64_t p_iValue,
                                                  const uint32_t p_timeFromLast)
{
	/* Local variable for return */
	e_eDSP_DERIVATIVE_RES l_eRes;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eDSP_DERIVATIVE_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eDSP_DERIVATIVE_RES_NOINITLIB;
		}
		else
		{
            /* Check data coherence */
            if( false == eDSP_DERIVATIVE_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eDSP_DERIVATIVE_RES_CORRUPTCTX;
            }
			else
			{
                /* Check data validity */
				if( 0u == p_timeFromLast )
				{
					l_eRes = e_eDSP_DERIVATIVE_RES_BADPARAM;
				}
				else
				{
					/* Check data validity */
					p_ptCtx->uPreviousVal = p_ptCtx->uCurrentVal;
					p_ptCtx->uCurrentVal = p_iValue;
					p_ptCtx->uTimeElapsedFromCurToPre = p_timeFromLast;

					if( false == p_ptCtx->bHasCurrent )
					{
						p_ptCtx->bHasCurrent = true;
					}
					else
					{
						p_ptCtx->bHasPrev = true;
					}
				}
			}
		}
    }

	return l_eRes;
}

e_eDSP_DERIVATIVE_RES eDSP_DERIVATIVE_CalcDerivate(t_eDSP_DERIVATIVE_Ctx* const p_ptCtx, int64_t* const p_piDerivate)
{
	/* Local variable for return */
	e_eDSP_DERIVATIVE_RES l_eRes;
	e_eDSP_MAXCHECK_RES l_eMaxRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_piDerivate ))
	{
		l_eRes = e_eDSP_DERIVATIVE_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eDSP_DERIVATIVE_RES_NOINITLIB;
		}
		else
		{
            /* Check data coherence */
            if( false == eDSP_DERIVATIVE_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eDSP_DERIVATIVE_RES_CORRUPTCTX;
            }
			else
			{
                /* Check if we can proceed */
				if( false == p_ptCtx->bHasPrev )
				{
					l_eRes = e_eDSP_DERIVATIVE_RES_BADPARAM;
				}
				else
				{
					l_eMaxRes = eDSP_MAXCHECK_SUBTI64Check(p_ptCtx->uCurrentVal, p_ptCtx->uPreviousVal);
					l_eRes = eDSP_DERIVATIVE_MaxCheckResToS2DP(l_eMaxRes);

					if( e_eDSP_DERIVATIVE_RES_OK == l_eRes )
					{
						/* calculate */
						*p_piDerivate = p_ptCtx->uCurrentVal - p_ptCtx->uPreviousVal / p_ptCtx->uTimeElapsedFromCurToPre;
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
static bool_t eDSP_DERIVATIVE_IsStatusStillCoherent(t_eDSP_DERIVATIVE_Ctx* const p_ptCtx)
{
    /* Return local var */
    bool_t l_eRes;

	l_eRes = true;

    return l_eRes;
}

static e_eDSP_DERIVATIVE_RES eDSP_DERIVATIVE_MaxCheckResToS2DP(const e_eDSP_MAXCHECK_RES p_tMaxRet)
{
	e_eDSP_DERIVATIVE_RES l_eRet;

	if( e_eDSP_MAXCHECK_RES_OK == p_tMaxRet )
	{
		l_eRet = e_eDSP_DERIVATIVE_RES_OK;
	}
	else
	{
		l_eRet = e_eDSP_DERIVATIVE_RES_OVERFLOW;
	}

	return l_eRet;
}
