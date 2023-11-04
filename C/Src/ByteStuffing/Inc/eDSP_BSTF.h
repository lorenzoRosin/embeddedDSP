/**
 * @file       eDSP_BSTF.h
 *
 * @brief      Byte stuffer utils
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EDSP_BSTF_H
#define EDSP_BSTF_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eDSP_TYPE.h"
#include "eDSP_BSTFPRV.h"



/***********************************************************************************************************************
 *      TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    e_eDSP_BSTF_RES_OK = 0,
    e_eDSP_BSTF_RES_BADPARAM,
    e_eDSP_BSTF_RES_BADPOINTER,
	e_eDSP_BSTF_RES_CORRUPTCTX,
    e_eDSP_BSTF_RES_FRAMEENDED,
    e_eDSP_BSTF_RES_NOINITLIB,
    e_eDSP_BSTF_RES_NOINITFRAME,
}e_eDSP_BSTF_RES;

typedef struct
{
    bool_t   bIsInit;
	uint8_t* puBuff;
	uint32_t uBuffL;
    uint32_t uFrameL;
	uint32_t uFrameCtr;
    e_eDSP_BSTFPRV_SM eSM;
}t_eDSP_BSTF_Ctx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the byte stuffer context
 *
 * @param[in]   p_ptCtx    - Byte stuffer context
 * @param[in]   p_puBuff   - Pointer to a memory area that we will use to store data that needs to be stuffed
 * @param[in]   p_uBuffL   - Dimension in byte of the memory area
 *
 * @return      e_eDSP_BSTF_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eDSP_BSTF_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *              e_eDSP_BSTF_RES_OK            - Operation ended correctly
 */
e_eDSP_BSTF_RES eDSP_BSTF_InitCtx(t_eDSP_BSTF_Ctx* const p_ptCtx, uint8_t* p_puBuff, const uint32_t p_uBuffL);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx       - Byte stuffer context
 * @param[out]  p_pbIsInit    - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eDSP_BSTF_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eDSP_BSTF_RES_OK            - Operation ended correctly
 */
e_eDSP_BSTF_RES eDSP_BSTF_IsInit(t_eDSP_BSTF_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Retrive the pointer of the buffer that the user can use to insert data payload that need to be stuffed
 *
 * @param[in]   p_ptCtx    - Byte stuffer context
 * @param[out]  p_ppuData  - Pointer to a Pointer where the raw data needs to be copied before starting a frame
 * @param[out]  p_puMaxL   - Pointer to a uint32_t variable where the max number of data that can be copied in
 *                           p_ppuData will be placed
 *
 * @return      e_eDSP_BSTF_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eDSP_BSTF_RES_NOINITLIB     - Need to init the data stuffer context before taking some action
 *		        e_eDSP_BSTF_RES_CORRUPTCTX    - In case of an corrupted context
 *              e_eDSP_BSTF_RES_OK            - Operation ended correctly
 */
e_eDSP_BSTF_RES eDSP_BSTF_GetWherePutData(t_eDSP_BSTF_Ctx* const p_ptCtx, uint8_t** p_ppuData, uint32_t* const p_puMaxL);

/**
 * @brief       Start to stuff a new frame given the dimension of raw payload it self. This function suppouse that
 *              data payload that need to be stuffed were already copied in memory.( see eDSP_BSTF_GetWherePutData
 *              in order to know how get the data pointer )
 *
 * @param[in]   p_ptCtx      - Byte stuffer context
 * @param[in]   p_uFrameL    - lenght of the raw data present in the frame that we need to stuff
 *
 * @return      e_eDSP_BSTF_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eDSP_BSTF_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *		        e_eDSP_BSTF_RES_NOINITLIB     - Need to init the data stuffer context before taking some action
 *		        e_eDSP_BSTF_RES_CORRUPTCTX    - In case of an corrupted context
 *              e_eDSP_BSTF_RES_OK            - Operation ended correctly
 */
e_eDSP_BSTF_RES eDSP_BSTF_NewFrame(t_eDSP_BSTF_Ctx* const p_ptCtx, const uint32_t p_uFrameL);

/**
 * @brief       Restart to stuff the already passed data/the current frame
 *
 * @param[in]   p_ptCtx         - Byte stuffer context
 *
 * @return      e_eDSP_BSTF_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eDSP_BSTF_RES_NOINITLIB     - Need to init the data stuffer context before taking some action
 *		        e_eDSP_BSTF_RES_NOINITFRAME   - Need to start a frame before restarting the current frame
 *		        e_eDSP_BSTF_RES_CORRUPTCTX    - In case of an corrupted context
 *              e_eDSP_BSTF_RES_OK            - Operation ended correctly
 */
e_eDSP_BSTF_RES eDSP_BSTF_RestartFrame(t_eDSP_BSTF_Ctx* const p_ptCtx);

/**
 * @brief       Retrive the numbers of stuffed bytes that can be retrived using eDSP_BSTF_GetStufChunk (e.g. if the
 *              value of the returned value is zero it's means that the frame is ended ).
 *
 * @param[in]   p_ptCtx       - Byte stuffer context
 * @param[out]  p_puGettedL   - Pointer to a uint32_t variable where the numbers of retrivable stuffed data will be
 *                              placed
 *
 * @return      e_eDSP_BSTF_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eDSP_BSTF_RES_NOINITLIB     - Need to init the data stuffer context before taking some action
 *		        e_eDSP_BSTF_RES_NOINITFRAME   - Need to start a frame before getting how many stuffed data we can retrive
 *		        e_eDSP_BSTF_RES_CORRUPTCTX    - In case of an corrupted context
 *              e_eDSP_BSTF_RES_OK            - Operation ended correctly
 */
e_eDSP_BSTF_RES eDSP_BSTF_GetRemByteToGet(t_eDSP_BSTF_Ctx* const p_ptCtx, uint32_t* const p_puGettedL);

/**
 * @brief       Retrive stuffed data chunk. The raw data copied in the buffer retrived using the function
 *              eDSP_BSTF_GetWherePutData will be stuffed and retrived by this function.
 *
 * @param[in]   p_ptCtx        - Byte stuffer context
 * @param[in]   p_puStuffedBuf - Pointer to the destination area where stuffed data will be placed by this function
 * @param[in]   p_uMaxBufL     - Max fillable size of the destination area
 * @param[out]  p_puGettedL    - Pointer to an uint32_t were we will store the number stuffed data inserted in
 *                               p_puStuffedBuf. Note that if the function return e_eDSP_BSTF_RES_OK the value of
 *                               filledLen will be equals to p_uMaxBufL. The value filledLen infact could be lower
 *                               than max dest size only if some error is returned or if the frame is
 *                               ended ( e_eDSP_BSTF_RES_FRAMEENDED ).
 * @return      e_eDSP_BSTF_RES_BADPOINTER    - In case of bad pointer passed to the function
 *		        e_eDSP_BSTF_RES_NOINITLIB     - Need to init the data stuffer context before taking some action
 *		        e_eDSP_BSTF_RES_BADPARAM      - In case of an invalid parameter passed to the function
 *		        e_eDSP_BSTF_RES_NOINITFRAME   - Need to start a frame before retriving data
 *		        e_eDSP_BSTF_RES_CORRUPTCTX    - In case of an corrupted context
 *              e_eDSP_BSTF_RES_FRAMEENDED    - No more data that we can elaborate, restart or start a new frame to
 *                                             proceed. This means that we have finished stuffing the current frame.
 *                                             Keep in mind in this case that the value of filledLen could be lower
 *                                             than p_uMaxBufL.
 *              e_eDSP_BSTF_RES_OK            - Operation ended correctly. This dosent mean that the stuffing process is
 *                                             completed, but we can be sure that filledLen will have the same value of
 *                                             p_uMaxBufL
 */
e_eDSP_BSTF_RES eDSP_BSTF_GetStufChunk(t_eDSP_BSTF_Ctx* const p_ptCtx, uint8_t* p_puStuffedBuf, const uint32_t p_uMaxBufL,
                                     uint32_t* const p_puGettedL);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EDSP_BSTF_H */