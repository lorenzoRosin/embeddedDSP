/**
 * @file       eDSP_S2DPI64LINEAR.h
 *
 * @brief      Single 2D point Linearization on a int64_t point
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EDSP_S2DPI64LINEAR_H
#define EDSP_S2DPI64LINEAR_H



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
    e_eDSP_S2DPI64LINEAR_RES_OK = 0,
    e_eDSP_S2DPI64LINEAR_RES_BADPOINTER,
    e_eDSP_S2DPI64LINEAR_RES_BADPARAM,
    e_eDSP_S2DPI64LINEAR_RES_OVERFLOW
}e_eDSP_S2DPI64LINEAR_RES;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Given two point, calculate the Y value of the line passing in the provided two point, given an X value
 *
 * @param[in]   p_tP1           - First point
 * @param[in]   p_tP2           - Second point
 * @param[in]   p_iX            - X value to use during the calculation of the Y value
 * @param[in]   p_piY           - Pointer to an int64_t where the value of the calculated Y will be placed.
 *
 * @return      e_eDSP_S2DPI64LINEAR_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eDSP_S2DPI64LINEAR_RES_BADPARAM     - In case of an invalid parameter passed to the function.
 *                                                    - so when the two point are the same or when the line as and angle
 *                                                    - of +-90 degree
 *		        e_eDSP_S2DPI64LINEAR_RES_OVERFLOW     - In case some internal operation generate an overflow, usualy
 *                                                    - due to the fact that we are using too large values
 *              e_eDSP_S2DPI64LINEAR_RES_OK           - Operation ended correctly
 */
e_eDSP_S2DPI64LINEAR_RES eDSP_S2DPI64LINEAR_Linearize(const t_eDSP_TYPE_2DPI64 p_tP1, const t_eDSP_TYPE_2DPI64 p_tP2,
                                                      const int64_t p_iX, int64_t* const p_piY);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EDSP_S2DPI64LINEAR_H */