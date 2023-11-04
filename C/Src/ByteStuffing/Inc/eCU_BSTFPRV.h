/**
 * @file       eDSP_BSTFPRV.h
 *
 * @brief      Byte stuffer utils private definition
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EDSP_BSTFPRV_H
#define EDSP_BSTFPRV_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      PRIVATE TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    e_eDSP_BSTFPRV_SM_NEEDSOF = 0,
    e_eDSP_BSTFPRV_SM_NEEDRAWDATA,
    e_eDSP_BSTFPRV_SM_NEEDNEGATEPRECDATA,
	e_eDSP_BSTFPRV_SM_NEEDEOF,
    e_eDSP_BSTFPRV_SM_STUFFEND
}e_eDSP_BSTFPRV_SM;



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EDSP_BSTFPRV_H */