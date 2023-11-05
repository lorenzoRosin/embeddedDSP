/**
 * @file       eDSP_S2DPU32LINEAR.c
 *
 * @brief      Single 2D point Linearization
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eDSP_S2DPU32LINEAR.h"



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eDSP_S2DPU32LINEAR_RES eDSP_S2DPU32LINEAR_Linearize( const t_eDSP_TYPE_2DP32U p_tP1,  const t_eDSP_TYPE_2DP32U p_tP2,
                                                       const uint32_t p_uX, uint32_t* const p_puY)
{
	/* Local variable */
	e_eDSP_S2DPU32LINEAR_RES l_eRes;

	/* Check pointer validity */
	if( NULL == p_puY )
	{
		l_eRes = e_eDSP_S2DPU32LINEAR_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eDSP_S2DPU32LINEAR_RES_NOINITLIB;
		}
		else
		{
            /* Check internal status validity */
            if( false == eDSP_S2DPU32LINEAR_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eDSP_S2DPU32LINEAR_RES_CORRUPTCTX;
            }
            else
            {
                /* Update index */
                p_ptCtx->uMemPKCtr = 0u;
                l_eRes = e_eDSP_S2DPU32LINEAR_RES_OK;
            }
		}
    }

	return l_eRes;
}

