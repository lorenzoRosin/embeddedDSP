/**
 * @file       eDSP_STANDARDTYPE.h
 *
 * @brief      Edsp standard type header
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EDSP_STANDARDTYPE_H
#define EDSP_STANDARDTYPE_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>



/***********************************************************************************************************************
 *      DEFINES
 **********************************************************************************************************************/
#ifndef NULL
  #define NULL          ( _NULL )
#endif

#ifndef bool_t
    typedef bool bool_t;
#endif



/***********************************************************************************************************************
 *      UINT VALUES
 **********************************************************************************************************************/
#ifndef MAX_UINT64VAL
  #define MAX_UINT64VAL                                                             ( ( uint64_t ) 0xFFFFFFFFFFFFFFFFu )
#endif

#ifndef MIN_UINT64VAL
  #define MIN_UINT64VAL                                                             ( ( uint64_t ) 0x0000000000000000u )
#endif

#ifndef MAX_UINT32VAL
  #define MAX_UINT32VAL                                                                     ( ( uint32_t ) 0xFFFFFFFFu )
#endif

#ifndef MIN_UINT32VAL
  #define MIN_UINT32VAL                                                                     ( ( uint32_t ) 0x00000000u )
#endif

#ifndef MAX_UINT16VAL
  #define MAX_UINT16VAL                                                                         ( ( uint16_t ) 0xFFFFu )
#endif

#ifndef MIN_UINT16VAL
  #define MIN_UINT16VAL                                                                         ( ( uint16_t ) 0x0000u )
#endif

#ifndef MAX_UINT8VAL
  #define MAX_UINT8VAL                                                                             ( ( uint8_t ) 0xFFu )
#endif

#ifndef MIN_UINT8VAL
  #define MIN_UINT8VAL                                                                             ( ( uint8_t ) 0x00u )
#endif



/***********************************************************************************************************************
 *      INT VALUES
 **********************************************************************************************************************/
#ifndef MAX_INT64VAL
  #define MAX_INT64VAL                                                               ( ( int64_t ) 9223372036854775807 )
#endif

#ifndef MIN_INT64VAL
  #define MIN_INT64VAL                                                              ( ( int64_t ) -9223372036854775808 )
#endif

#ifndef MAX_INT32VAL
  #define MAX_INT32VAL                                                                        ( ( int32_t ) 2147483647 )
#endif

#ifndef MIN_INT32VAL
  #define MIN_INT32VAL                                                                       ( ( int32_t ) -2147483648 )
#endif



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EDSP_STANDARDTYPE_H */