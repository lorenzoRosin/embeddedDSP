/**
 * @file       eDSP_DERIVATIVE.h
 *
 * @brief      Derivation operation modules
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EDSP_DERIVATIVE_H
#define EDSP_DERIVATIVE_H



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
    e_eDSP_DERIVATIVE_RES_OK = 0,
    e_eDSP_DERIVATIVE_RES_BADPOINTER,
    e_eDSP_DERIVATIVE_RES_BADPARAM,
    e_eDSP_DERIVATIVE_RES_OVERFLOW,
    e_eDSP_DERIVATIVE_RES_CORRUPTCTX,
    e_eDSP_DERIVATIVE_RES_NOINITLIB
}e_eDSP_DERIVATIVE_RES;

typedef struct
{
    bool_t bIsInit;
    bool_t bHasPrev;
    bool_t bHasCurrent;
    int64_t uPreviousVal;
    int64_t uCurrentVal;
    uint32_t uTimeElapsedFromCurToPre;
}t_eDSP_DERIVATIVE_Ctx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the multiple point linearizer using a series of point
 *
 * @param[in]   p_ptCtx       - Multiple point linearizer context
 * @param[in]   p_tSeries     - Struct containing the series of point
 *
 * @return      e_eDSP_DERIVATIVE_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        e_eDSP_DERIVATIVE_RES_BADPARAM       - In case of an invalid parameter passed to the function
 *              e_eDSP_DERIVATIVE_RES_OK             - Multiple point linearizer initialized successfully
 */
e_eDSP_DERIVATIVE_RES eDSP_DERIVATIVE_InitCtx(t_eDSP_DERIVATIVE_Ctx* const p_ptCtx);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx         - Multiple point linearizer context
 * @param[out]  p_pbIsInit      - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eDSP_DERIVATIVE_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eDSP_DERIVATIVE_RES_OK            - Operation ended correctly
 */
e_eDSP_DERIVATIVE_RES eDSP_DERIVATIVE_IsInit(t_eDSP_DERIVATIVE_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Given the series of point used during the init phase, calculate the Y value of the line passing in the
 *              provided two point, given an X value
 *
 * @param[in]   p_ptCtx         - Multiple point linearizer context
 * @param[in]   p_uX            - X value to use during the calculation of the Y value
 * @param[in]   p_puY           - Pointer to an int64_t where the value of the calculated Y will be placed.
 *
 * @return      e_eDSP_DERIVATIVE_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eDSP_DERIVATIVE_RES_BADPARAM     - In case of an invalid parameter passed to the function.
 *                                                    - so when the two point are the same or when the line as and angle
 *                                                    - of +-90 degree
 *		        e_eDSP_DERIVATIVE_RES_NOINITLIB    - Need to init the data stuffer context before taking some action
 *		        e_eDSP_DERIVATIVE_RES_CORRUPTCTX   - In case of an corrupted context
 *              e_eDSP_DERIVATIVE_RES_OK           - Operation ended correctly
 */
e_eDSP_DERIVATIVE_RES eDSP_DERIVATIVE_InsertValue(t_eDSP_DERIVATIVE_Ctx* const p_ptCtx, const int64_t p_value,
                                                  const uint32_t p_timeFromLast);

/**
 * @brief       Given the series of point used during the init phase, calculate the Y value of the line passing in the
 *              provided two point, given an X value
 *
 * @param[in]   p_ptCtx         - Multiple point linearizer context
 * @param[in]   p_uX            - X value to use during the calculation of the Y value
 * @param[in]   p_puY           - Pointer to an int64_t where the value of the calculated Y will be placed.
 *
 * @return      e_eDSP_DERIVATIVE_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eDSP_DERIVATIVE_RES_BADPARAM     - In case of an invalid parameter passed to the function.
 *                                                    - so when the two point are the same or when the line as and angle
 *                                                    - of +-90 degree
 *		        e_eDSP_DERIVATIVE_RES_NOINITLIB    - Need to init the data stuffer context before taking some action
 *		        e_eDSP_DERIVATIVE_RES_CORRUPTCTX   - In case of an corrupted context
 *              e_eDSP_DERIVATIVE_RES_OK           - Operation ended correctly
 */
e_eDSP_DERIVATIVE_RES eDSP_DERIVATIVE_CalcDerivate(t_eDSP_DERIVATIVE_Ctx* const p_ptCtx, int64_t* const p_piDerivate);

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EDSP_DERIVATIVE_H */