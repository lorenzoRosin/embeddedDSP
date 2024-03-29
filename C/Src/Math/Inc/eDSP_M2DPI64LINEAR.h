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
    e_eDSP_M2DPI64LINEAR_RES_OVERFLOW,
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
 * @brief       Initialize the multiple point linearizer using a series of point
 *
 * @param[in]   p_ptCtx       - Multiple point linearizer context
 * @param[in]   p_tSeries     - Struct containing the series of point
 *
 * @return      e_eDSP_M2DPI64LINEAR_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        e_eDSP_M2DPI64LINEAR_RES_BADPARAM       - In case of an invalid parameter passed to the function.
 *                                                      - so when the two point are the same or when the line as and
 *                                                      - angle of +-90 degree, or when there are less than 3 point
 *              e_eDSP_M2DPI64LINEAR_RES_OK             - Multiple point linearizer initialized successfully
 */
e_eDSP_M2DPI64LINEAR_RES eDSP_M2DPI64LINEAR_InitCtx(t_eDSP_M2DPI64LINEAR_Ctx* const p_ptCtx,
                                                    t_eDSP_M2DPI64LINEAR_PointSeries p_tSeries);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx         - Multiple point linearizer context
 * @param[out]  p_pbIsInit      - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eDSP_M2DPI64LINEAR_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eDSP_M2DPI64LINEAR_RES_OK            - Operation ended correctly
 */
e_eDSP_M2DPI64LINEAR_RES eDSP_M2DPI64LINEAR_IsInit(t_eDSP_M2DPI64LINEAR_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Given the series of point used during the init phase, calculate the Y value of the line passing in
 *              nearest two point of the series, given an X value
 *
 * @param[in]   p_ptCtx         - Multiple point linearizer context
 * @param[in]   p_uX            - X value to use during the calculation of the Y value
 * @param[out]  p_puY           - Pointer to an int64_t where the value of the calculated Y will be placed.
 *
 * @return      e_eDSP_M2DPI64LINEAR_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eDSP_M2DPI64LINEAR_RES_NOINITLIB    - Need to init the data stuffer context before taking some action
 *		        e_eDSP_M2DPI64LINEAR_RES_CORRUPTCTX   - In case of an corrupted context
 *              e_eDSP_M2DPI64LINEAR_RES_OK           - Operation ended correctly
 */
e_eDSP_M2DPI64LINEAR_RES eDSP_M2DPI64LINEAR_Linearize(t_eDSP_M2DPI64LINEAR_Ctx* const p_ptCtx, const int64_t p_uX,
                                                      int64_t* const p_puY);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EDSP_M2DPI64LINEAR_H */