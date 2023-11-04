/**
 * @file       eDSP_TYPE.h
 *
 * @brief      Edsp lib type header
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EDSP_TYPE_H
#define EDSP_TYPE_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eDSP_STANDARDTYPE.h"



/***********************************************************************************************************************
 *      DEFINES
 **********************************************************************************************************************/
#define EDSP_SOF                                                                                    ( ( uint8_t ) 0xA1u )
#define EDSP_EOF                                                                                    ( ( uint8_t ) 0xA2u )
#define EDSP_ESC                                                                                    ( ( uint8_t ) 0xA3u )



/***********************************************************************************************************************
 *      TYPEDEFS STRUCT
 **********************************************************************************************************************/

typedef struct
{
    uint32_t    uX;
    uint32_t    uY;
}t_eDSP_TYPE_2DP32U;


#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EDSP_TYPE_H */