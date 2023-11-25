/**
 * @file       eDSP_MEDIANFILTER.h
 *
 * @brief      Median filter implementation
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EDSP_MEDIANFILTER_H
#define EDSP_MEDIANFILTER_H



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
    e_eDSP_MEDIANFILTER_RES_OK = 0,
    e_eDSP_MEDIANFILTER_RES_BADPOINTER,
    e_eDSP_MEDIANFILTER_RES_BADPARAM,
    e_eDSP_MEDIANFILTER_RES_OVERFLOW,
    e_eDSP_MEDIANFILTER_RES_NEEDSMOREVALUE,
    e_eDSP_MEDIANFILTER_RES_CORRUPTCTX,
    e_eDSP_MEDIANFILTER_RES_NOINITLIB
}e_eDSP_MEDIANFILTER_RES;

typedef struct
{
    bool_t   bIsInit;
    uint32_t uWindowsLen;
    uint32_t uFilledData;
    uint32_t uCurDataLocation;
    int64_t* piWindowsBuffer;    
}t_eDSP_MEDIANFILTER_Ctx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the median filter with used buffer
 *
 * @param[in]   p_ptCtx             - Median filter context
 * @param[in]   p_piWindowsBuffer   - Buffer that must be provided in order to calculate the median value
 * @param[in]   p_uWindowsBuffLen   - Numbers of element of the p_piWindowsBuffer, and len of the median windows.
 *
 * @return      e_eDSP_MEDIANFILTER_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        e_eDSP_MEDIANFILTER_RES_BADPARAM       - In case of an invalid parameter passed to the function
 *              e_eDSP_MEDIANFILTER_RES_OK             - Median filter initialized successfully
 */
e_eDSP_MEDIANFILTER_RES eDSP_MEDIANFILTER_InitCtx(t_eDSP_MEDIANFILTER_Ctx* const p_ptCtx, int64_t* p_piWindowsBuffer, 
                                                  uint32_t p_uWindowsBuffLen);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx         - Median filter context
 * @param[out]  p_pbIsInit      - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eDSP_MEDIANFILTER_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eDSP_MEDIANFILTER_RES_OK            - Operation ended correctly
 */
e_eDSP_MEDIANFILTER_RES eDSP_MEDIANFILTER_IsInit(t_eDSP_MEDIANFILTER_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Insert a new value for the median filter and calculate the filtered values. Keep in mind that during
 *              the first insertion we are not able o have some filtered data.
 *
 * @param[in]   p_ptCtx         - Median filter context
 * @param[in]   p_iValue        - Current value that we want to filter
 * @param[out]  p_pFilteredVal  - Pointer to an int64_t where the value of the calculated derivate will be placed.
 *
 * @return      e_eDSP_MEDIANFILTER_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        e_eDSP_MEDIANFILTER_RES_BADPARAM       - In case of an invalid parameter passed to the function.
 *		        e_eDSP_MEDIANFILTER_RES_NOINITLIB      - Need to init the data stuffer context before taking some action
 *		        e_eDSP_MEDIANFILTER_RES_CORRUPTCTX     - In case of an corrupted context
 *              e_eDSP_MEDIANFILTER_RES_NEEDSMOREVALUE - Need to add more value to be able to calculate the derivate
 *              e_eDSP_MEDIANFILTER_RES_OK             - Operation ended correctly
 */
e_eDSP_MEDIANFILTER_RES eDSP_MEDIANFILTER_InsertValueAndCalculate(t_eDSP_MEDIANFILTER_Ctx* const p_ptCtx, 
                                                                  const int64_t p_iValue, int64_t* const p_pFilteredVal);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EDSP_MEDIANFILTER_H */