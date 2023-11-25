/**
 * @file       eDSP_INTEGRAL.h
 *
 * @brief      Integral calculation
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EDSP_INTEGRAL_H
#define EDSP_INTEGRAL_H



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
    e_eDSP_INTEGRAL_RES_OK = 0,
    e_eDSP_INTEGRAL_RES_BADPOINTER,
    e_eDSP_INTEGRAL_RES_BADPARAM,
    e_eDSP_INTEGRAL_RES_OVERFLOW,
    e_eDSP_INTEGRAL_RES_NEEDSMOREVALUE,
    e_eDSP_INTEGRAL_RES_CORRUPTCTX,
    e_eDSP_INTEGRAL_RES_NOINITLIB
}e_eDSP_INTEGRAL_RES;

typedef struct
{
    bool_t   bIsInit;
    bool_t   bHasPrev;
    bool_t   bHasCurrent;
    int64_t  uPreviousVal;
    int64_t  uCurrentVal;
    int64_t  uIntegral;
    uint32_t uTimeElapsFromCurToPre;
}t_eDSP_INTEGRAL_Ctx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the integral function
 *
 * @param[in]   p_ptCtx       - Integral context
 *
 * @return      e_eDSP_INTEGRAL_RES_BADPOINTER     - In case of bad pointer passed to the function
 *              e_eDSP_INTEGRAL_RES_OK             - Integral initialized successfully
 */
e_eDSP_INTEGRAL_RES eDSP_INTEGRAL_InitCtx(t_eDSP_INTEGRAL_Ctx* const p_ptCtx);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx         - Integral context
 * @param[out]  p_pbIsInit      - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eDSP_INTEGRAL_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eDSP_INTEGRAL_RES_OK            - Operation ended correctly
 */
e_eDSP_INTEGRAL_RES eDSP_INTEGRAL_IsInit(t_eDSP_INTEGRAL_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Insert the a value of data that we will use to calculate the integral using all the previously inserted
 *              value. p_timeFromLast cannot be zero. Is enough values are inserted return the values of the integral
 *
 * @param[in]   p_ptCtx         - Integral context
 * @param[in]   p_iValue         - Current value that we want to integrate
 * @param[in]   p_timeFromLast  - Time elapsed from the previously inserted value 
 * @param[out]  p_piIntegral    - Pointer to an int64_t where the value of the calculated integral will be placed.
 *
 * @return      e_eDSP_INTEGRAL_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eDSP_INTEGRAL_RES_BADPARAM     - In case of an invalid parameter passed to the function.
 *		        e_eDSP_INTEGRAL_RES_NOINITLIB    - Need to init the data stuffer context before taking some action
 *		        e_eDSP_INTEGRAL_RES_CORRUPTCTX   - In case of an corrupted context
 *              e_eDSP_INTEGRAL_RES_NEEDSMOREVALUE   - Need to add more value to be able to calculate the integral
 *              e_eDSP_INTEGRAL_RES_OK           - Operation ended correctly
 */
e_eDSP_INTEGRAL_RES eDSP_INTEGRAL_InsertValueAndCalcIntegral(t_eDSP_INTEGRAL_Ctx* const p_ptCtx, const int64_t p_iValue,
                                                             const uint32_t p_timeFromLast, int64_t* const p_piIntegral);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EDSP_INTEGRAL_H */