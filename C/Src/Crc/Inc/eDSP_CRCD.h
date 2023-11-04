/**
 * @file       eDSP_CRCD.h
 *
 * @brief      Cal CRC using custom CRC32 using digest approach
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EDSP_CRCD_H
#define EDSP_CRCD_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eDSP_CRC.h"



/***********************************************************************************************************************
 *      TYPEDEFS
 **********************************************************************************************************************/
/* Define a generic crc callback context that must be implemented by the user */
typedef struct t_eDSP_CRCD_CrcCtxUser t_eDSP_CRCD_CrcCtx;

/* Call back of a function that will calculate the CRC for this modules.
 * the p_ptCtx parameter is a custom pointer that can be used by the creator of this CRC callback, and will not be used
 * by the CRCdigest module */
typedef bool_t (*f_eDSP_CRCD_CrcCb) ( t_eDSP_CRCD_CrcCtx* const p_ptCtx, const uint32_t p_uUseed, const uint8_t* p_puData,
                                     const uint32_t p_uDataL, uint32_t* const p_puCrc32Val );

typedef enum
{
    e_eDSP_CRCD_RES_OK = 0,
    e_eDSP_CRCD_RES_BADPARAM,
    e_eDSP_CRCD_RES_BADPOINTER,
	e_eDSP_CRCD_RES_CORRUPTCTX,
    e_eDSP_CRCD_RES_TOOMANYDIGEST,
    e_eDSP_CRCD_RES_NODIGESTDONE,
    e_eDSP_CRCD_RES_NOINITLIB,
    e_eDSP_CRCD_RES_CLBCKREPORTERROR,
}e_eDSP_CRCD_RES;

typedef struct
{
    bool_t bIsInit;
    uint32_t uBaseSeed;
	uint32_t uDigestedTimes;
    uint32_t uLastDigVal;
    f_eDSP_CRCD_CrcCb fCrc;
    t_eDSP_CRCD_CrcCtx* ptCrcCtx;
}t_eDSP_CRCD_Ctx;



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
e_eDSP_CRCD_RES eDSP_CRCD_InitCtx(t_eDSP_CRCD_Ctx* const p_ptCtx, f_eDSP_CRCD_CrcCb p_fCrc,
                                t_eDSP_CRCD_CrcCtx* const p_ptFctx);

/**
 * @brief       Initialize the CRC32 digester context using a selected p_uUseed
 *
 * @param[in]   p_ptCtx     - Crc digester context
 * @param[in]   p_uUseed    - Base Seed
 * @param[in]   p_fCrc      - Pointer to a CRC 32 p_uUseed callback function, that will be used to calculate the CRC32
 * @param[in]   p_ptFctx    - Custom context passed to the callback function p_fCrc
 *
 * @return      e_eDSP_CRCD_RES_BADPOINTER     - In case of bad pointer passed to the function
 *              e_eDSP_CRCD_RES_OK             - Crc digester initialized successfully
 */
e_eDSP_CRCD_RES eDSP_CRCD_SeedInitCtx(t_eDSP_CRCD_Ctx* const p_ptCtx, const uint32_t p_uUseed, f_eDSP_CRCD_CrcCb p_fCrc,
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
e_eDSP_CRCD_RES eDSP_CRCD_IsInit(t_eDSP_CRCD_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Restart the digester and disharge all old value
 *
 * @param[in]   p_ptCtx         - Crc digester context
 *
 * @return      e_eDSP_CRCD_RES_BADPOINTER         - In case of bad pointer passed to the function
 *              e_eDSP_CRCD_RES_OK                 - Data digested successfully
 *              e_eDSP_CRCD_RES_NOINITLIB          - Need to init the lib before taking some action
 *              e_eDSP_CRCD_RES_CORRUPTCTX         - In case of a corrupted context
 */
e_eDSP_CRCD_RES eDSP_CRCD_Restart(t_eDSP_CRCD_Ctx* const p_ptCtx);

/**
 * @brief       Digest a chunk of data that we want to calculate CRC
 *
 * @param[in]   p_ptCtx           - Crc digester context
 * @param[in]   p_puData          - Pointer to a memory area containg a chunk of data to digest
 * @param[in]   p_uDataL          - Lenght of the buffer we will digest
 *
 * @return      e_eDSP_CRCD_RES_BADPOINTER         - In case of bad pointer passed to the function
 *              e_eDSP_CRCD_RES_OK                 - Data digested successfully
 *              e_eDSP_CRCD_RES_NOINITLIB          - Need to init the lib before taking some action
 *              e_eDSP_CRCD_RES_BADPARAM           - In case of bad parameter passed to the function
 *              e_eDSP_CRCD_RES_CORRUPTCTX         - In case of a corrupted context
 *              e_eDSP_CRCD_RES_TOOMANYDIGEST      - Too many digest operation
 *              e_eDSP_CRCD_RES_CLBCKREPORTERROR   - The callback function reported an error
 */
e_eDSP_CRCD_RES eDSP_CRCD_Digest(t_eDSP_CRCD_Ctx* const p_ptCtx, const uint8_t* p_puData, const uint32_t p_uDataL);

/**
 * @brief       Retrive the CRC32 of all the chunk digested using eDSP_CRCD_Digest. After this function is used the
 *              internal state is resetted and old result are discharged.
 *
 * @param[in]   p_ptCtx       - Crc digester context
 * @param[out]  p_puCrcCalc   - Pointer to a memory area that will contain the value of the calculated CRC
 *
 * @return      e_eDSP_CRCD_RES_BADPOINTER         - In case of bad pointer passed to the function
 *              e_eDSP_CRCD_RES_OK                 - Data digested successfully
 *              e_eDSP_CRCD_RES_NOINITLIB          - Need to init the lib before taking some action
 *              e_eDSP_CRCD_RES_CORRUPTCTX         - In case of a corrupted context
 *              e_eDSP_CRCD_RES_NODIGESTDONE       - Need to do al least one digest before getting the CRC32
 */
e_eDSP_CRCD_RES eDSP_CRCD_GetDigestVal(t_eDSP_CRCD_Ctx* const p_ptCtx, uint32_t* const p_puCrcCalc);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EDSP_CRCD_H */