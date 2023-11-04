/**
 * @file       eDSP_BUNSTFPRV.h
 *
 * @brief      Byte Unstuffer utils private definition
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EDSP_BUNSTFPRV_H
#define EDSP_BUNSTFPRV_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      PRIVATE TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    e_eDSP_BUNSTFPRV_SM_NEEDSOF = 0,
    e_eDSP_BUNSTFPRV_SM_NEEDRAWDATA,
    e_eDSP_BUNSTFPRV_SM_NEEDNEGATEDATA,
    e_eDSP_BUNSTFPRV_SM_UNSTUFFEND,
    e_eDSP_BUNSTFPRV_SM_UNSTUFFFAIL
}e_eDSP_BUNSTFPRV_SM;



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EDSP_BUNSTFPRV_H */
