/**
 * @file       eDSP_M2DPI64LINEAR.h
 *
 * @brief      Multiple 2D point Linearization on a int64_t
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EDSP_M2DPI64LINEAR_H
#define EDSP_M2DPI64LINEAR_H



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
    e_eDSP_M2DPI64LINEAR_RES_OK = 0,
    e_eDSP_M2DPI64LINEAR_RES_BADPOINTER,
    e_eDSP_M2DPI64LINEAR_RES_BADPARAM,
    e_eDSP_M2DPI64LINEAR_RES_OUTLIMIT,
    e_eDSP_M2DPI64LINEAR_RES_CORRUPTCTX,
    e_eDSP_M2DPI64LINEAR_RES_NOINITLIB
}e_eDSP_M2DPI64LINEAR_RES;

typedef struct
{
    uint32_t uNumPoint;
    t_eDSP_TYPE_2DPI64* ptPointArray;
}t_eDSP_M2DPI64LINEAR_PointSeries;

typedef struct
{
    bool_t bIsInit;
    t_eDSP_M2DPI64LINEAR_PointSeries tPoinSeries;
}t_eDSP_M2DPI64LINEAR_Ctx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the CRC32 digester context ( use as base p_uUseed 0xFFFFFFFFu )
 *
 * @param[in]   p_ptCtx       - Crc digester context
 * @param[in]   p_fCrc        - Pointer to a CRC 32 p_uUseed callback function, that will be used to calculate the CRC32
 * @param[in]   p_ptFctx      - Custom context passed to the callback function p_fCrc
 *
 * @return      e_eDSP_CRCD_RES_BADPOINTER     - In case of bad pointer passed to the function
 *              e_eDSP_CRCD_RES_OK             - Crc digester initialized successfully
 */
e_eDSP_CRCD_RES eDSP_CRCD_InitCtx(t_eDSP_M2DPI64LINEAR_Ctx* const p_ptCtx, f_eDSP_CRCD_CrcCb p_fCrc,
                                t_eDSP_CRCD_CrcCtx* const p_ptFctx);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx         - Crc digester context
 * @param[out]  p_pbIsInit      - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eDSP_CRCD_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eDSP_CRCD_RES_OK            - Operation ended correctly
 */
e_eDSP_CRCD_RES eDSP_CRCD_IsInit(t_eDSP_M2DPI64LINEAR_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Given two point, calculate the Y value of the rect passing in the provided two point, given an X value
 *
 * @param[in]   p_tP1           - First point
 * @param[in]   p_tP2           - Second point
 * @param[in]   p_uX            - X value to use during the calculation of the Y value
 * @param[in]   p_puY           - Pointer to an int64_t where the value of the calculated Y will be placed.
 *
 * @return      e_eDSP_M2DPI64LINEAR_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eDSP_M2DPI64LINEAR_RES_BADPARAM     - In case of an invalid parameter passed to the function.
 *                                                    - so when the two point are the same or when the rect as and angle
 *                                                    - of +-90 degree
 *              e_eDSP_M2DPI64LINEAR_RES_OK           - Operation ended correctly
 */
e_eDSP_M2DPI64LINEAR_RES eDSP_M2DPI64LINEAR_Linearize( t_eDSP_M2DPI64LINEAR_Ctx* const p_ptCtx, const t_eDSP_TYPE_2DPI64 p_tP1,
                                                       const t_eDSP_TYPE_2DPI64 p_tP2, const int64_t p_uX, int64_t* const p_puY );



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EDSP_M2DPI64LINEAR_H */