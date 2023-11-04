/**
 * @file       eDSP_CRCTST.c
 *
 * @brief      CRC test
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eDSP_CRCTST.h"
#include "eDSP_CRC.h"
#include <stdio.h>



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void eDSP_CRCTST_BadPointer(void);
static void eDSP_CRCTST_crc32Value(void);
static void eDSP_CRCTST_crc32Combined(void);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void eDSP_CRCTST_ExeTest(void)
{
	(void)printf("\n\nCRC TEST START \n\n");

    eDSP_CRCTST_BadPointer();
    eDSP_CRCTST_crc32Value();
    eDSP_CRCTST_crc32Combined();

    (void)printf("\n\nCRC TEST END \n\n");
}



/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
static void eDSP_CRCTST_BadPointer(void)
{
    /* Local variable */
    uint32_t l_uCrc32SValTest;
    uint8_t  l_auCrc8SValTest[1u];
    l_auCrc8SValTest[0u] = 0x00u;

    /* Function */
    if( e_eDSP_CRC_RES_BADPOINTER == eDSP_CRC_32Seed(0u, NULL, 1u, &l_uCrc32SValTest) )
    {
        (void)printf("eDSP_CRCTST_BadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("eDSP_CRCTST_BadPointer 1  -- FAIL \n");
    }

    if( e_eDSP_CRC_RES_BADPOINTER == eDSP_CRC_32Seed(0u, l_auCrc8SValTest, 1u, NULL) )
    {
        (void)printf("eDSP_CRCTST_BadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("eDSP_CRCTST_BadPointer 2  -- FAIL \n");
    }

    if( e_eDSP_CRC_RES_BADPOINTER == eDSP_CRC_32(NULL, 1u, &l_uCrc32SValTest) )
    {
        (void)printf("eDSP_CRCTST_BadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("eDSP_CRCTST_BadPointer 3  -- FAIL \n");
    }

    if( e_eDSP_CRC_RES_BADPOINTER == eDSP_CRC_32(l_auCrc8SValTest, 1u, NULL) )
    {
        (void)printf("eDSP_CRCTST_BadPointer 4  -- OK \n");
    }
    else
    {
        (void)printf("eDSP_CRCTST_BadPointer 4  -- FAIL \n");
    }
}

static void eDSP_CRCTST_crc32Value(void)
{
    /* Local variable */
    uint32_t l_uCrcTestValSeed;
    uint8_t l_auCrcTestData1[] = {0x00u, 0x01u, 0x02u};
    uint32_t l_uCrcTestValRet;

    /* Function */
    l_uCrcTestValSeed = 0xFFFFFFFFu;
    if( e_eDSP_CRC_RES_OK == eDSP_CRC_32Seed(l_uCrcTestValSeed, l_auCrcTestData1, sizeof(l_auCrcTestData1), &l_uCrcTestValRet) )
    {
        if( 0x6CFF87B2u == l_uCrcTestValRet)
        {
            (void)printf("eDSP_CRCTST_crc32Value 1  -- OK \n");
        }
        else
        {
            (void)printf("eDSP_CRCTST_crc32Value 1  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eDSP_CRCTST_crc32Value 1  -- FAIL \n");
    }

    l_uCrcTestValSeed = 0x0u;
    if( e_eDSP_CRC_RES_OK == eDSP_CRC_32Seed(l_uCrcTestValSeed, l_auCrcTestData1, sizeof(l_auCrcTestData1), &l_uCrcTestValRet) )
    {
        if( 0xDB9BFAB2u == l_uCrcTestValRet)
        {
            (void)printf("eDSP_CRCTST_crc32Value 2  -- OK \n");
        }
        else
        {
            (void)printf("eDSP_CRCTST_crc32Value 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eDSP_CRCTST_crc32Value 2  -- FAIL \n");
    }

    if( e_eDSP_CRC_RES_OK == eDSP_CRC_32(l_auCrcTestData1, sizeof(l_auCrcTestData1), &l_uCrcTestValRet) )
    {
        if( 0x6CFF87B2u == l_uCrcTestValRet)
        {
            (void)printf("eDSP_CRCTST_crc32Value 3  -- OK \n");
        }
        else
        {
            (void)printf("eDSP_CRCTST_crc32Value 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eDSP_CRCTST_crc32Value 3  -- FAIL \n");
    }

    /* Function */
    uint8_t l_auCrcTestData2[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0xFF, 0xF1, 0xF5, 0x31, 0x32, 0x33,
                              0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0xFF, 0xF1, 0xF5};

    l_uCrcTestValSeed = 0xFFFFFFFFu;
    if( e_eDSP_CRC_RES_OK == eDSP_CRC_32Seed(l_uCrcTestValSeed, l_auCrcTestData2, sizeof(l_auCrcTestData2), &l_uCrcTestValRet) )
    {
        if( 0x4DBE0510u == l_uCrcTestValRet)
        {
            (void)printf("eDSP_CRCTST_crc32Value 4  -- OK \n");
        }
        else
        {
            (void)printf("eDSP_CRCTST_crc32Value 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eDSP_CRCTST_crc32Value 4  -- FAIL \n");
    }

    l_uCrcTestValSeed = 0x0u;
    if( e_eDSP_CRC_RES_OK == eDSP_CRC_32Seed(l_uCrcTestValSeed, l_auCrcTestData2, sizeof(l_auCrcTestData2), &l_uCrcTestValRet) )
    {
        if( 0xB612792Au == l_uCrcTestValRet)
        {
            (void)printf("eDSP_CRCTST_crc32Value 5  -- OK \n");
        }
        else
        {
            (void)printf("eDSP_CRCTST_crc32Value 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eDSP_CRCTST_crc32Value 5  -- FAIL \n");
    }

    if( e_eDSP_CRC_RES_OK == eDSP_CRC_32(l_auCrcTestData2, sizeof(l_auCrcTestData2), &l_uCrcTestValRet) )
    {
        if( 0x4DBE0510u == l_uCrcTestValRet)
        {
            (void)printf("eDSP_CRCTST_crc32Value 6  -- OK \n");
        }
        else
        {
            (void)printf("eDSP_CRCTST_crc32Value 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eDSP_CRCTST_crc32Value 6  -- FAIL \n");
    }
}

static void eDSP_CRCTST_crc32Combined(void)
{
    /* Local variable */
    uint32_t l_uCrcTestValSeedC;
    uint8_t l_auCrcTestDataC[] = {0x00u, 0x01u, 0x02u, 0x03u, 0x04u, 0x05u};
    uint8_t l_auCrcTestDataC2[] = {0x03u, 0x04u, 0x05u};
    uint32_t l_uCrcTestValRetC;

    /* Function */
    l_uCrcTestValSeedC = 0xFFFFFFFFu;
    if( e_eDSP_CRC_RES_OK == eDSP_CRC_32Seed(l_uCrcTestValSeedC, l_auCrcTestDataC, sizeof(l_auCrcTestDataC), &l_uCrcTestValRetC) )
    {
        if( 0x9FE54C6Du == l_uCrcTestValRetC)
        {
            (void)printf("eDSP_CRCTST_crc32Combined 1  -- OK \n");
        }
        else
        {
            (void)printf("eDSP_CRCTST_crc32Combined 1  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eDSP_CRCTST_crc32Combined 1  -- FAIL \n");
    }

    l_uCrcTestValSeedC = 0xFFFFFFFFu;
    if( e_eDSP_CRC_RES_OK == eDSP_CRC_32Seed(l_uCrcTestValSeedC, l_auCrcTestDataC, 0x03u, &l_uCrcTestValRetC) )
    {
        if( 0x6CFF87B2u == l_uCrcTestValRetC)
        {
            (void)printf("eDSP_CRCTST_crc32Combined 2  -- OK \n");
        }
        else
        {
            (void)printf("eDSP_CRCTST_crc32Combined 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eDSP_CRCTST_crc32Combined 2  -- FAIL \n");
    }

    l_uCrcTestValSeedC = l_uCrcTestValRetC;
    if( e_eDSP_CRC_RES_OK == eDSP_CRC_32Seed(l_uCrcTestValSeedC, l_auCrcTestDataC2, 0x03u, &l_uCrcTestValRetC) )
    {
        if( 0x9FE54C6Du == l_uCrcTestValRetC)
        {
            (void)printf("eDSP_CRCTST_crc32Combined 3  -- OK \n");
        }
        else
        {
            (void)printf("eDSP_CRCTST_crc32Combined 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eDSP_CRCTST_crc32Combined 3  -- FAIL \n");
    }
}