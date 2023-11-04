/**
 * @file       eDSP_BSTFBUNSTFTST.c
 *
 * @brief      Data stuffer and unstuffer test
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eDSP_BSTFBUNSTFTST.h"
#include "eDSP_BSTF.h"
#include "eDSP_BUNSTF.h"
#include <stdio.h>

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "CERT-STR32-C"
    /* Suppressed for code clarity in test execution*/
#endif



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void eDSP_BSTFBUNSTFTST_Common(void);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void eDSP_BSTFBUNSTFTST_ExeTest(void)
{
	(void)printf("\n\nDATA STUFF UNSTUFF TEST START \n\n");

    eDSP_BSTFBUNSTFTST_Common();

    (void)printf("\n\nDATA STUFF UNSTUFF END \n\n");
}



/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
typedef struct
{
    uint32_t uDataTestL;
    uint8_t  *puDataTest;
}t_eDSP_BSTFBUNSTFTST_PrvMatrix;

static void eDSP_BSTFBUNSTFTST_Common(void)
{
    /* Local variable */
    t_eDSP_BUNSTF_Ctx l_tCtxUnStuff;
    t_eDSP_BSTF_Ctx l_tCtxStuff;
    static uint8_t  l_auDataUnStuffPool[300];
    static uint8_t  l_auDataStuffPool[300];
    static uint8_t  l_auTempPool[300u];
    uint32_t  l_uTemp32;
    uint32_t  l_uTemp32sec;
    uint32_t l_uIndex;
    uint8_t* l_puTempPo;
    uint32_t l_uTempPSize;

    /* Test data */
    static uint8_t l_auTest1[5u]   = {0x01u, EDSP_SOF, EDSP_EOF, EDSP_ESC, 0x21u};
    static uint8_t l_auTest2[5u]   = {0x01u, EDSP_ESC, EDSP_ESC, EDSP_ESC, 0x21u};
    static uint8_t l_auTest3[7u]   = {0x01u, EDSP_ESC, EDSP_ESC, EDSP_ESC, 0x21u, (uint8_t)(~EDSP_ESC), (uint8_t)(~EDSP_ESC)};
    static uint8_t l_auTest4[1u]   = {0x01u};
    static uint8_t l_auTest5[200u] = {0xFFu};


    /* Test Matrix */
    t_eDSP_BSTFBUNSTFTST_PrvMatrix testMatrix[5] = {
        { .uDataTestL = sizeof(l_auTest1), .puDataTest = l_auTest1 },
        { .uDataTestL = sizeof(l_auTest2), .puDataTest = l_auTest2 },
        { .uDataTestL = sizeof(l_auTest3), .puDataTest = l_auTest3 },
        { .uDataTestL = sizeof(l_auTest4), .puDataTest = l_auTest4 },
        { .uDataTestL = sizeof(l_auTest5), .puDataTest = l_auTest5 }
    };

    for(l_uIndex = 0u; l_uIndex < (uint32_t)( ( sizeof(testMatrix) ) / ( sizeof(t_eDSP_BSTFBUNSTFTST_PrvMatrix) ) ); l_uIndex++)
    {
        /* Function Init */
        if( e_eDSP_BSTF_RES_OK == eDSP_BSTF_InitCtx(&l_tCtxStuff, l_auDataStuffPool, sizeof(l_auDataStuffPool)) )
        {
            (void)printf("eDSP_BSTFBUNSTFTST_Common 1[%u]  -- OK \n", l_uIndex);
        }
        else
        {
            (void)printf("eDSP_BSTFBUNSTFTST_Common 1[%u]  -- FAIL \n", l_uIndex);
        }

        if( e_eDSP_BUNSTF_RES_OK == eDSP_BUNSTF_InitCtx(&l_tCtxUnStuff, l_auDataUnStuffPool, sizeof(l_auDataUnStuffPool)) )
        {
            (void)printf("eDSP_BSTFBUNSTFTST_Common 2[%u]  -- OK \n", l_uIndex);
        }
        else
        {
            (void)printf("eDSP_BSTFBUNSTFTST_Common 2[%u]  -- FAIL \n", l_uIndex);
        }

        /* Copy data in byte stuffer */
        if( e_eDSP_BSTF_RES_OK == eDSP_BSTF_GetWherePutData(&l_tCtxStuff, &l_puTempPo, &l_uTempPSize) )
        {
            if( l_uTempPSize == sizeof(l_auDataStuffPool) )
            {
                (void)memcpy(l_puTempPo, testMatrix[l_uIndex].puDataTest, testMatrix[l_uIndex].uDataTestL);
                (void)printf("eDSP_BSTFBUNSTFTST_Common 3[%u]  -- OK \n", l_uIndex);
            }
            else
            {
                (void)printf("eDSP_BSTFBUNSTFTST_Common 3[%u]  -- FAIL \n", l_uIndex);
            }
        }
        else
        {
            (void)printf("eDSP_BSTFBUNSTFTST_Common 3[%u]  -- FAIL \n", l_uIndex);
        }


        /* Function Init part two */
        if( e_eDSP_BSTF_RES_OK == eDSP_BSTF_NewFrame(&l_tCtxStuff, testMatrix[l_uIndex].uDataTestL) )
        {
            (void)printf("eDSP_BSTFBUNSTFTST_Common 4[%u]  -- OK \n", l_uIndex);
        }
        else
        {
            (void)printf("eDSP_BSTFBUNSTFTST_Common 4[%u]  -- FAIL \n", l_uIndex);
        }

        if( e_eDSP_BUNSTF_RES_OK == eDSP_BUNSTF_NewFrame(&l_tCtxUnStuff) )
        {
            (void)printf("eDSP_BSTFBUNSTFTST_Common 5[%u]  -- OK \n", l_uIndex);
        }
        else
        {
            (void)printf("eDSP_BSTFBUNSTFTST_Common 5[%u]  -- FAIL \n", l_uIndex);
        }

        /* Stuff */
        if( e_eDSP_BSTF_RES_FRAMEENDED == eDSP_BSTF_GetStufChunk(&l_tCtxStuff, l_auTempPool, sizeof(l_auTempPool), &l_uTemp32) )
        {
            (void)printf("eDSP_BSTFBUNSTFTST_Common 6[%u]  -- OK \n", l_uIndex);
        }
        else
        {
            (void)printf("eDSP_BSTFBUNSTFTST_Common 6[%u]  -- FAIL \n", l_uIndex);
        }

        /* unstuff */
        if( e_eDSP_BUNSTF_RES_FRAMEENDED == eDSP_BUNSTF_InsStufChunk( &l_tCtxUnStuff, l_auTempPool, l_uTemp32, &l_uTemp32sec ) )
        {
            if( e_eDSP_BUNSTF_RES_OK == eDSP_BUNSTF_GetUnstufLen(&l_tCtxUnStuff, &l_uTemp32sec) )
            {
                if( testMatrix[l_uIndex].uDataTestL == l_uTemp32sec )
                {
                    if( 0 == memcmp(l_auDataUnStuffPool, testMatrix[l_uIndex].puDataTest, testMatrix[l_uIndex].uDataTestL) )
                    {
                        (void)printf("eDSP_BSTFBUNSTFTST_Common 7[%u]  -- OK \n", l_uIndex);
                    }
                    else
                    {
                        (void)printf("eDSP_BSTFBUNSTFTST_Common 7[%u]  -- FAIL \n", l_uIndex);
                    }
                }
                else
                {
                    (void)printf("eDSP_BSTFBUNSTFTST_Common 7[%u]  -- FAIL \n", l_uIndex);
                }
            }
            else
            {
                (void)printf("eDSP_BSTFBUNSTFTST_Common 7[%u]  -- FAIL \n", l_uIndex);
            }
        }
        else
        {
            (void)printf("eDSP_BSTFBUNSTFTST_Common 7[%u]  -- FAIL \n", l_uIndex);
        }
    }
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "CERT-STR32-C"
#endif