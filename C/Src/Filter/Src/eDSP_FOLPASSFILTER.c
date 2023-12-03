/**
 * @file       eDSP_FOLPASSFILTER.c
 *
 * @brief      First order Low pass filter implementation
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eDSP_FOLPASSFILTER.h"
#include "eDSP_MAXCHECK.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eDSP_FOLPASSFILTER_IsStatusStillCoherent(t_eDSP_FOLPASSFILTER_Ctx* const p_ptCtx);
static e_eDSP_FOLPASSFILTER_RES eDSP_FOLPASSFILTER_MaxCheckResToFOLPF(const e_eDSP_MAXCHECK_RES p_tMaxRet);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eDSP_FOLPASSFILTER_RES eDSP_FOLPASSFILTER_InitCtx(t_eDSP_FOLPASSFILTER_Ctx* const p_ptCtx, uint64_t p_uCutFreqMilHz)
{
	/* Local variable */
	e_eDSP_FOLPASSFILTER_RES l_eRes;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eDSP_FOLPASSFILTER_RES_BADPOINTER;
	}
	else
	{
		/* Check data validity */
		if( 0u == p_uCutFreqMilHz )
		{
			l_eRes = e_eDSP_FOLPASSFILTER_RES_BADPARAM;
		}
		else
		{
			/* Initialize internal status */
			p_ptCtx->bIsInit = true;
			p_ptCtx->uCutFreqMilHz = p_uCutFreqMilHz;
			p_ptCtx->iPrevOutValue = 0u;

			/* All OK */
			l_eRes = e_eDSP_FOLPASSFILTER_RES_OK;
		}
	}

	return l_eRes;
}

e_eDSP_FOLPASSFILTER_RES eDSP_FOLPASSFILTER_IsInit(t_eDSP_FOLPASSFILTER_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eDSP_FOLPASSFILTER_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eDSP_FOLPASSFILTER_RES_BADPOINTER;
	}
	else
	{
        *p_pbIsInit = p_ptCtx->bIsInit;
        l_eRes = e_eDSP_FOLPASSFILTER_RES_OK;
	}

	return l_eRes;
}

e_eDSP_FOLPASSFILTER_RES eDSP_FOLPASSFILTER_InsertValueAndCalculate(t_eDSP_FOLPASSFILTER_Ctx* const p_ptCtx,
                                                                    const int64_t p_iValue,
                                                                    int64_t* const p_pFilteredVal)
{
	/* Local variable for return */
	e_eDSP_FOLPASSFILTER_RES l_eRes;
	e_eDSP_MAXCHECK_RES l_eMaxRes;

	/* Local variable for calculation */
	uint32_t l_uCnt;
	int64_t l_iSum;
	int64_t l_iMean;
	int64_t l_iNearest;
	int64_t l_iNearestDiff;
	int64_t l_iCurrDiff;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pFilteredVal ) )
	{
		l_eRes = e_eDSP_FOLPASSFILTER_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eDSP_FOLPASSFILTER_RES_NOINITLIB;
		}
		else
		{
            /* Check data coherence */
            if( false == eDSP_FOLPASSFILTER_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eDSP_FOLPASSFILTER_RES_CORRUPTCTX;
            }
			else
			{
				/* To calculate the First order Low pass filter we will use an RC circuit.

					-----/\/\/\----------
									|
						Vin		   ___    Vout
								   ___
									|
					---------------------
					The equantion of the circuit is:
					Vin(ti) - Vout(ti) = Vr(ti)
					Vin(ti) - Vout(ti) = R * Ir(ti)
					Vin(ti) - Vout(ti) = R *  ( C * ( dVout(ti) / dT ) )
					Vin(ti) - Vout(ti) = RC * ( dVout(ti) / dT )
					and using discrete time we have:
					Vin(ti) - Vout(ti) = RC * (    ( Vout(i) - Vout(i-1) )    /    ( t(i) - t(i-1) )    )
					Vout(ti) = Vin(ti) - RC * (    ( Vout(i) - Vout(i-1) )    /    ( t(i) - t(i-1) )    )
					... More calc ...
					Vout(ti) = ( ( t(i)-t(i-1) ) / ( RC + ( t(i)-t(i-1) ) ) * Vin(ti) +
							   ( RC / ( RC + ( t(i)-t(i-1) ) )              * Vout(ti-1)
					Doing other math in the frequency domains we found out that the cutoff frequency is
					Fc = 1 / ( 2 pi RC ) and so the value of RC = 1 / ( 2 pi Fc )
				*/

				l_eRes = e_eDSP_FOLPASSFILTER_RES_OK;
			}
		}
    }

	return l_eRes;
}



/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
static bool_t eDSP_FOLPASSFILTER_IsStatusStillCoherent(t_eDSP_FOLPASSFILTER_Ctx* const p_ptCtx)
{
    /* Return local var */
    bool_t l_eRes;

	/* Check pointer validity */
	if( 0u == p_ptCtx->uCutFreqMilHz )
	{
		l_eRes = false;
	}
    else
    {
		l_eRes = true;
    }

    return l_eRes;
}

static e_eDSP_FOLPASSFILTER_RES eDSP_FOLPASSFILTER_MaxCheckResToFOLPF(const e_eDSP_MAXCHECK_RES p_tMaxRet)
{
	e_eDSP_FOLPASSFILTER_RES l_eRet;

	if( e_eDSP_MAXCHECK_RES_OK == p_tMaxRet )
	{
		l_eRet = e_eDSP_FOLPASSFILTER_RES_OK;
	}
	else
	{
		l_eRet = e_eDSP_FOLPASSFILTER_RES_OVERFLOW;
	}

	return l_eRet;
}
