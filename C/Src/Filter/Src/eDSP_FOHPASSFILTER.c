/**
 * @file       eDSP_FOHPASSFILTER.c
 *
 * @brief      First order High pass filter implementation
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
static e_eDSP_FOHPASSFILTER_RES eDSP_FOHPASSFILTER_MaxCheckResToFOHPF(const e_eDSP_MAXCHECK_RES p_tMaxRet);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eDSP_FOHPASSFILTER_RES eDSP_FOHPASSFILTER_InitCtx(t_eDSP_FOHPASSFILTER_Ctx* const p_ptCtx, uint64_t p_uCutFreqMilHz)
{
	/* Local variable */
	e_eDSP_FOHPASSFILTER_RES l_eRes;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eDSP_FOHPASSFILTER_RES_BADPOINTER;
	}
	else
	{
		/* Check data validity */
		if( 0u == p_uCutFreqMilHz )
		{
			l_eRes = e_eDSP_FOHPASSFILTER_RES_BADPARAM;
		}
		else
		{
			/* Initialize internal status */
			p_ptCtx->bIsInit = true;
			p_ptCtx->uCutFreqMilHz = p_uCutFreqMilHz;
			p_ptCtx->iPrevOutValue = 0u;

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
                                                                    const int64_t p_iValue,
                                                                    int64_t* const p_pFilteredVal)
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
				/* To calculate the High pass filter we will use an RC circuit.

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

				l_eRes = e_eDSP_FOHPASSFILTER_RES_OK;
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

static e_eDSP_FOHPASSFILTER_RES eDSP_FOHPASSFILTER_MaxCheckResToFOHPF(const e_eDSP_MAXCHECK_RES p_tMaxRet)
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
