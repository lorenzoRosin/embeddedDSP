/**
 * @file       eDSP_DPK.h
 *
 * @brief      Data pack utils
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EDSP_DPK_H
#define EDSP_DPK_H



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
    e_eDSP_DPK_RES_OK = 0,
    e_eDSP_DPK_RES_BADPARAM,
    e_eDSP_DPK_RES_BADPOINTER,
	e_eDSP_DPK_RES_CORRUPTCTX,
    e_eDSP_DPK_RES_OUTOFMEM,
    e_eDSP_DPK_RES_NOINITLIB,
}e_eDSP_DPK_RES;

typedef struct
{
    bool_t bIsInit;
	bool_t bIsLE;
	uint8_t* puMemPK;
	uint32_t uMemPKL;
	uint32_t uMemPKCtr;
}t_eDSP_DPK_Ctx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the data packer context
 *
 * @param[in]   p_ptCtx    - Data packer context
 * @param[in]   p_puMemPK  - Pointer to a memory area that we will fill with packet data
 * @param[in]   p_uMemPKL  - Dimension in byte of the data packer area
 * @param[in]   p_bIsLEnd  - Select if data packer must work in Little Endian or Big Endian
 *
 * @return      e_eDSP_DPK_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eDSP_DPK_RES_BADPARAM     - In case of an invalid parameter passed to the function
 *              e_eDSP_DPK_RES_OK           - Operation ended correctly
 */
e_eDSP_DPK_RES eDSP_DPK_InitCtx(t_eDSP_DPK_Ctx* const p_ptCtx, uint8_t* p_puMemPK, const uint32_t p_uMemPKL,
                              const bool_t p_bIsLEnd);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx      - Data packer context
 * @param[out]  p_pbIsInit   - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eDSP_DPK_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eDSP_DPK_RES_OK            - Operation ended correctly
 */
e_eDSP_DPK_RES eDSP_DPK_IsInit(t_eDSP_DPK_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Reset data packer and restart packing data from start, discharging old data present, if any
 *
 * @param[in]   p_ptCtx         - Data packer context
 *
 * @return      e_eDSP_DPK_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eDSP_DPK_RES_NOINITLIB    - Need to init the data packer before taking some action
 *		        e_eDSP_DPK_RES_CORRUPTCTX   - In case of a corrupted context
 *              e_eDSP_DPK_RES_OK           - Operation ended correctly
 */
e_eDSP_DPK_RES eDSP_DPK_StartNewPack(t_eDSP_DPK_Ctx* const p_ptCtx);

/**
 * @brief       Retrive the pointer to the stored packed data, and the data size of the packed data.
 *
 * @param[in]   p_ptCtx     - Data packer context
 * @param[out]  p_ppuData   - Pointer to a Pointer pointing to the packed data buffer
 * @param[out]  p_puDataL   - Pointer to a uint32_t variable where the size of the packed data buffer will be placed
 *
 * @return      e_eDSP_DPK_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eDSP_DPK_RES_NOINITLIB    - Need to init the data packer before taking some action
 *		        e_eDSP_DPK_RES_CORRUPTCTX   - In case of a corrupted context
 *              e_eDSP_DPK_RES_OK           - Operation ended correctly
 */
e_eDSP_DPK_RES eDSP_DPK_GetDataReference(t_eDSP_DPK_Ctx* const p_ptCtx, uint8_t** p_ppuData, uint32_t* const p_puDataL);

/**
 * @brief       Retrive how many byte we have packed
 *
 * @param[in]   p_ptCtx   - Data packer context
 * @param[out]  p_puDataL - Pointer to a uint32_t variable where the size of the packed data buffer will be placed
 *
 * @return      e_eDSP_DPK_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eDSP_DPK_RES_NOINITLIB    - Need to init the data packer before taking some action
 *		        e_eDSP_DPK_RES_CORRUPTCTX   - In case of a corrupted context
 *              e_eDSP_DPK_RES_OK           - Operation ended correctly
 */
e_eDSP_DPK_RES eDSP_DPK_GetNPushed(t_eDSP_DPK_Ctx* const p_ptCtx, uint32_t* const p_puDataL);

/**
 * @brief       Push an array in data packer
 *
 * @param[in]   p_ptCtx         - Data packer context
 * @param[in]   p_puData        - Pointer to a memory area containing the data that we want to push
 * @param[in]   p_uDataL        - Data size of the data that we want to push
 *
 * @return      e_eDSP_DPK_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eDSP_DPK_RES_NOINITLIB    - Need to init the data packer before taking some action
 *		        e_eDSP_DPK_RES_BADPARAM     - In case of an invalid parameter passed to the function
 *		        e_eDSP_DPK_RES_CORRUPTCTX   - In case of a corrupted context
 *              e_eDSP_DPK_RES_OUTOFMEM     - Not enought memory to push other data
 *              e_eDSP_DPK_RES_OK           - Operation ended correctly
 */
e_eDSP_DPK_RES eDSP_DPK_PushArray(t_eDSP_DPK_Ctx* const p_ptCtx, uint8_t* p_puData, const uint32_t p_uDataL);

/**
 * @brief       Push one byte in data packer
 *
 * @param[in]   p_ptCtx         - Data packer context
 * @param[in]   p_uData         - Data to push in data packer
 *
 * @return      e_eDSP_DPK_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eDSP_DPK_RES_NOINITLIB    - Need to init the data packer before taking some action
 *		        e_eDSP_DPK_RES_CORRUPTCTX   - In case of a corrupted context
 *              e_eDSP_DPK_RES_OUTOFMEM     - Not enought memory to push other data
 *              e_eDSP_DPK_RES_OK           - Operation ended correctly
 */
e_eDSP_DPK_RES eDSP_DPK_PushU8(t_eDSP_DPK_Ctx* const p_ptCtx, const uint8_t p_uData);

/**
 * @brief       Push 2 byte in data packer
 *
 * @param[in]   p_ptCtx           - Data packer context
 * @param[in]   p_uData           - Data to push in data packer
 *
 * @return      e_eDSP_DPK_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eDSP_DPK_RES_NOINITLIB    - Need to init the data packer before taking some action
 *		        e_eDSP_DPK_RES_CORRUPTCTX   - In case of a corrupted context
 *              e_eDSP_DPK_RES_OUTOFMEM     - Not enought memory to push other data
 *              e_eDSP_DPK_RES_OK           - Operation ended correctly
 */
e_eDSP_DPK_RES eDSP_DPK_PushU16(t_eDSP_DPK_Ctx* const p_ptCtx, const uint16_t p_uData);

/**
 * @brief       Push 4 byte in data packer
 *
 * @param[in]   p_ptCtx           - Data packer context
 * @param[in]   p_uData           - Data to push in data packer
 *
 * @return      e_eDSP_DPK_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eDSP_DPK_RES_NOINITLIB    - Need to init the data packer before taking some action
 *		        e_eDSP_DPK_RES_CORRUPTCTX   - In case of a corrupted context
 *              e_eDSP_DPK_RES_OUTOFMEM     - Not enought memory to push other data
 *              e_eDSP_DPK_RES_OK           - Operation ended correctly
 */
e_eDSP_DPK_RES eDSP_DPK_PushU32(t_eDSP_DPK_Ctx* const p_ptCtx, const uint32_t p_uData);

/**
 * @brief       Push 8 byte in data packer
 *
 * @param[in]   p_ptCtx           - Data packer context
 * @param[in]   p_uData           - Data to push in data packer
 *
 * @return      e_eDSP_DPK_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eDSP_DPK_RES_NOINITLIB    - Need to init the data packer before taking some action
 *		        e_eDSP_DPK_RES_CORRUPTCTX   - In case of a corrupted context
 *              e_eDSP_DPK_RES_OUTOFMEM     - Not enought memory to push other data
 *              e_eDSP_DPK_RES_OK           - Operation ended correctly
 */
 e_eDSP_DPK_RES eDSP_DPK_PushU64(t_eDSP_DPK_Ctx* const p_ptCtx, const uint64_t p_uData);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EDSP_DPK_H */