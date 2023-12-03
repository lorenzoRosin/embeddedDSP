/**
 * @file       eDSP_FOHPASSFILTER.h
 *
 * @brief      First Order High pass filter implementation
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EDSP_FOHPASSFILTER_H
#define EDSP_FOHPASSFILTER_H



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
    e_eDSP_FOHPASSFILTER_RES_OK = 0,
    e_eDSP_FOHPASSFILTER_RES_BADPOINTER,
    e_eDSP_FOHPASSFILTER_RES_BADPARAM,
    e_eDSP_FOHPASSFILTER_RES_OVERFLOW,
    e_eDSP_FOHPASSFILTER_RES_NEEDSMOREVALUE,
    e_eDSP_FOHPASSFILTER_RES_CORRUPTCTX,
    e_eDSP_FOHPASSFILTER_RES_NOINITLIB
}e_eDSP_FOHPASSFILTER_RES;

typedef struct
{
    bool_t   bIsInit;
    int64_t  iPrevOutValue;
    uint64_t uCutFreqMilHz;
}t_eDSP_FOHPASSFILTER_Ctx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the first order High pass filter
 *
 * @param[in]   p_ptCtx            - First order High pass filter context
 * @param[in]   p_uCutFreqMilHz    - Cuttoff frequency of the first order High pass filter, in milli Hertz

 *
 * @return      e_eDSP_FOHPASSFILTER_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        e_eDSP_FOHPASSFILTER_RES_BADPARAM       - In case of an invalid parameter passed to the function
 *              e_eDSP_FOHPASSFILTER_RES_OK             - First order High pass filter initialized successfully
 */
e_eDSP_FOHPASSFILTER_RES eDSP_FOHPASSFILTER_InitCtx(t_eDSP_FOHPASSFILTER_Ctx* const p_ptCtx, uint64_t p_uCutFreqMilHz);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx         - First order High pass filter context
 * @param[out]  p_pbIsInit      - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eDSP_FOHPASSFILTER_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eDSP_FOHPASSFILTER_RES_OK            - Operation ended correctly
 */
e_eDSP_FOHPASSFILTER_RES eDSP_FOHPASSFILTER_IsInit(t_eDSP_FOHPASSFILTER_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Insert a new value for the first order High pass filter and calculate the filtered values. Keep in mind
 *              that during the first insertion we are not able o have some filtered data.
 *
 * @param[in]   p_ptCtx         - First order High pass filter context
 * @param[in]   p_iValue        - Current value that we want to filter
 * @param[out]  p_pFilteredVal  - Pointer to an int64_t where the value of the calculated derivate will be placed.
 *
 * @return      e_eDSP_FOHPASSFILTER_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        e_eDSP_FOHPASSFILTER_RES_BADPARAM       - In case of an invalid parameter passed to the function.
 *		        e_eDSP_FOHPASSFILTER_RES_NOINITLIB      - Need to init the data stuffer context before taking some action
 *		        e_eDSP_FOHPASSFILTER_RES_CORRUPTCTX     - In case of an corrupted context
 *              e_eDSP_FOHPASSFILTER_RES_NEEDSMOREVALUE - Need to add more value to be able to calculate the derivate
 *              e_eDSP_FOHPASSFILTER_RES_OK             - Operation ended correctly
 */
e_eDSP_FOHPASSFILTER_RES eDSP_FOHPASSFILTER_InsertValueAndCalculate(t_eDSP_FOHPASSFILTER_Ctx* const p_ptCtx,
                                                                    const int64_t p_iValue,
                                                                    int64_t* const p_pFilteredVal);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EDSP_FOHPASSFILTER_H */