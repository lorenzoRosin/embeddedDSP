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
#define EDSP_SOF                                                                                   ( ( uint8_t ) 0xA1u )
#define EDSP_EOF                                                                                   ( ( uint8_t ) 0xA2u )
#define EDSP_ESC                                                                                   ( ( uint8_t ) 0xA3u )



/***********************************************************************************************************************
 *      TYPEDEFS STRUCT
 **********************************************************************************************************************/
typedef struct
{
    int32_t    uX;
    int32_t    uY;
}t_eDSP_TYPE_2DPI32;

typedef struct
{
    int64_t    uX;
    int64_t    uY;
}t_eDSP_TYPE_2DPI64;


#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EDSP_TYPE_H */