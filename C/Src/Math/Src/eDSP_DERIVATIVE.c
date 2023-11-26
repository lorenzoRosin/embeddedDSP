/**
 * @file       eDSP_DERIVATIVE.h
 *
 * @brief      Derivation operation modules
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
static e_eDSP_DERIVATIVE_RES eDSP_DERIVATIVE_MaxCheckResToDERIVATE(const e_eDSP_MAXCHECK_RES p_tMaxRet);



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

e_eDSP_DERIVATIVE_RES eDSP_DERIVATIVE_InsertValueAndGetDerivate(t_eDSP_DERIVATIVE_Ctx* const p_ptCtx,
                                                                const int64_t p_iValue, const uint32_t p_timeFromLast,
                                                                int64_t* const p_piDerivate)
{
	/* Local variable for return */
	e_eDSP_DERIVATIVE_RES l_eRes;
	e_eDSP_MAXCHECK_RES l_eMaxRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_piDerivate ) )
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
					/* All ok */
					l_eRes = e_eDSP_DERIVATIVE_RES_OK;

					/* Insert data */
					p_ptCtx->uPreviousVal = p_ptCtx->uCurrentVal;
					p_ptCtx->uCurrentVal = p_iValue;
					p_ptCtx->uTimeElapsedFromCurToPre = p_timeFromLast;

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
						l_eRes = e_eDSP_DERIVATIVE_RES_NEEDSMOREVALUE;
					}
					else
					{
						/* the derivate operation is defined as follow:
						* lim of h -> 0 of the function: ( ( f(x0+h) - f(x0) ) / h ) wich is in ours function:
						currentvalue - previousvalue / timeelapsed from values */
						l_eMaxRes = eDSP_MAXCHECK_SUBTI64Check(p_ptCtx->uCurrentVal, p_ptCtx->uPreviousVal);
						l_eRes = eDSP_DERIVATIVE_MaxCheckResToDERIVATE(l_eMaxRes);

						if( e_eDSP_DERIVATIVE_RES_OK == l_eRes )
						{
							/* calculate */
							*p_piDerivate = ( p_ptCtx->uCurrentVal - p_ptCtx->uPreviousVal ) /
											p_ptCtx->uTimeElapsedFromCurToPre;
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
static bool_t eDSP_DERIVATIVE_IsStatusStillCoherent(t_eDSP_DERIVATIVE_Ctx* const p_ptCtx)
{
    /* Return local var */
    bool_t l_eRes;

	if( false == p_ptCtx->bHasCurrent )
	{
		/* No current value, no data present in the system */
		if( ( true == p_ptCtx->bHasPrev ) || ( 0 != p_ptCtx->uPreviousVal ) || ( 0 != p_ptCtx->uCurrentVal ) ||
			( 0 != p_ptCtx->uTimeElapsedFromCurToPre ) )
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
			if( ( 0 != p_ptCtx->uPreviousVal ) || ( 0 != p_ptCtx->uTimeElapsedFromCurToPre ) )
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
			/* Has even a previous value */
			if( 0 != p_ptCtx->uTimeElapsedFromCurToPre )
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

static e_eDSP_DERIVATIVE_RES eDSP_DERIVATIVE_MaxCheckResToDERIVATE(const e_eDSP_MAXCHECK_RES p_tMaxRet)
{
	/* Return local var */
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