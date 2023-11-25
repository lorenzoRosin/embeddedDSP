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
    e_eDSP_DERIVATIVE_RES_NEEDSMOREVALUE,
    e_eDSP_DERIVATIVE_RES_CORRUPTCTX,
    e_eDSP_DERIVATIVE_RES_NOINITLIB
}e_eDSP_DERIVATIVE_RES;

typedef struct
{
    bool_t   bIsInit;
    bool_t   bHasPrev;
    bool_t   bHasCurrent;
    int64_t  uPreviousVal;
    int64_t  uCurrentVal;
    uint32_t uTimeElapsedFromCurToPre;
}t_eDSP_DERIVATIVE_Ctx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the derivate function
 *
 * @param[in]   p_ptCtx       - Derivate context
 *
 * @return      e_eDSP_DERIVATIVE_RES_BADPOINTER     - In case of bad pointer passed to the function
 *              e_eDSP_DERIVATIVE_RES_OK             - Derivate initialized successfully
 */
e_eDSP_DERIVATIVE_RES eDSP_DERIVATIVE_InitCtx(t_eDSP_DERIVATIVE_Ctx* const p_ptCtx);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx         - Derivate context
 * @param[out]  p_pbIsInit      - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eDSP_DERIVATIVE_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eDSP_DERIVATIVE_RES_OK            - Operation ended correctly
 */
e_eDSP_DERIVATIVE_RES eDSP_DERIVATIVE_IsInit(t_eDSP_DERIVATIVE_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Insert the a value of data that we will use to calculate the derivate from the previously inserted
 *              value. p_timeFromLast cannot be zero. After adding the data, if enough sample are present,
 *              the derivate value is returned.
 *
 * @param[in]   p_ptCtx         - Derivate context
 * @param[in]   p_iValue         - Current value that we want to derivate
 * @param[in]   p_timeFromLast  - Time elapsed from the previously inserted value 
 * @param[out]  p_piDerivate    - Pointer to an int64_t where the value of the calculated derivate will be placed.
 *
 * @return      e_eDSP_DERIVATIVE_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        e_eDSP_DERIVATIVE_RES_BADPARAM       - In case of an invalid parameter passed to the function.
 *		        e_eDSP_DERIVATIVE_RES_NOINITLIB      - Need to init the data stuffer context before taking some action
 *		        e_eDSP_DERIVATIVE_RES_CORRUPTCTX     - In case of an corrupted context
 *              e_eDSP_DERIVATIVE_RES_NEEDSMOREVALUE - Need to add more value to be able to calculate the derivate
 *              e_eDSP_DERIVATIVE_RES_OK             - Operation ended correctly
 */
e_eDSP_DERIVATIVE_RES eDSP_DERIVATIVE_InsertValueAndGetDerivate(t_eDSP_DERIVATIVE_Ctx* const p_ptCtx, 
                                                                const int64_t p_iValue, const uint32_t p_timeFromLast, 
                                                                int64_t* const p_piDerivate);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EDSP_DERIVATIVE_H */