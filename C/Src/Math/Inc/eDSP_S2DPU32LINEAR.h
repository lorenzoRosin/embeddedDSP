/**
 * @file       eDSP_S2DPU32LINEAR.h
 *
 * @brief      Single 2D point Linearization
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EDSP_S2DPU32LINEAR_H
#define EDSP_S2DPU32LINEAR_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eDSP_TYPE.h"



/***********************************************************************************************************************
 *      TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    e_eDSP_S2DPU32LINEAR_RES_OK = 0,
    e_eDSP_S2DPU32LINEAR_RES_BADPARAM,
    e_eDSP_S2DPU32LINEAR_RES_BADPOINTER,
}e_eDSP_S2DPU32LINEAR_RES;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Given two point get the value of y using a given X using a rect passing in the two point as function
 *
 * @param[in]   p_tP1           - First point
 * @param[in]   p_tP2           - Second point
 * @param[in]   p_uX            - X value to use during the calculation of the Y value
 * @param[in]   p_puY           - Pointer to an uint32_t where the value of the calculated Y will be placed.
 *
 * @return      e_eDSP_S2DPU32LINEAR_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eDSP_S2DPU32LINEAR_RES_BADPARAM     - In case of an invalid parameter passed to the function
 *              e_eDSP_S2DPU32LINEAR_RES_OK           - Operation ended correctly
 */
e_eDSP_S2DPU32LINEAR_RES eDSP_S2DPU32LINEAR_Linearize( const t_eDSP_TYPE_2DP32U p_tP1,  const t_eDSP_TYPE_2DP32U p_tP2,
                                                       const uint32_t p_uX, uint32_t* const p_puY);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EDSP_S2DPU32LINEAR_H */