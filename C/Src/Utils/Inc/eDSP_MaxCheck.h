/**
 * @file       eDSP_MAXCHECK.h
 *
 * @brief      Check if some operation exceed storage limits
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EDSP_MAXCHECK
#define EDSP_MAXCHECK



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
    e_eDSP_MAXCHECK_RES_OK = 0,
    e_eDSP_MAXCHECK_OVERFLOW
}e_eDSP_MAXCHECK_RES;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Given two value check if the sum exceed the int64_t storage area ( p_iFirst + p_iSecond )
 * @param[in]   p_iFirst           - First operator
 * @param[in]   p_iSecond          - Second operator
 *
 * @return      e_eDSP_MAXCHECK_OVERFLOW         - Operation exceed limits
 *              e_eDSP_MAXCHECK_RES_OK           - Operation dosen't exceed limits
 */
e_eDSP_MAXCHECK_RES eDSP_MAXCHECK_SUMI64Check(const int64_t p_iFirst, const int64_t p_iSecond);

/**
 * @brief       Given two value check if the subtration exceed the int64_t storage area ( p_iFirst - p_iSecond )
 * @param[in]   p_iFirst           - First operator
 * @param[in]   p_iSecond          - Second operator
 *
 * @return      e_eDSP_MAXCHECK_OVERFLOW         - Operation exceed limits
 *              e_eDSP_MAXCHECK_RES_OK           - Operation dosen't exceed limits
 */
e_eDSP_MAXCHECK_RES eDSP_MAXCHECK_SUBTI64Check(const int64_t p_iFirst, const int64_t p_iSecond);

/**
 * @brief       Given two value check if the moltiplication exceed the int64_t storage area ( p_iFirst * p_iSecond )
 * @param[in]   p_iFirst           - First operator
 * @param[in]   p_iSecond          - Second operator
 *
 * @return      e_eDSP_MAXCHECK_OVERFLOW         - Operation exceed limits
 *              e_eDSP_MAXCHECK_RES_OK           - Operation dosen't exceed limits
 */
e_eDSP_MAXCHECK_RES eDSP_MAXCHECK_MOLTIPI64Check(const int64_t p_iFirst, const int64_t p_iSecond);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EDSP_MAXCHECK */