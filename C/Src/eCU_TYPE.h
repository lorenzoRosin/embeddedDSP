/**
 * @file       eCU_TYPE.h
 *
 * @brief      Ecu lib type header
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef ECU_TYPE_H
#define ECU_TYPE_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eCU_STANDARDTYPE.h"



/***********************************************************************************************************************
 *      DEFINES
 **********************************************************************************************************************/
#define ECU_SOF                                                                                    ( ( uint8_t ) 0xA1u )
#define ECU_EOF                                                                                    ( ( uint8_t ) 0xA2u )
#define ECU_ESC                                                                                    ( ( uint8_t ) 0xA3u )



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* ECU_TYPE_H */