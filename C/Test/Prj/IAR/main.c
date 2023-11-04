#include "eDSP_CRCTST.h"
#include "eDSP_CRCDTST.h"
#include "eDSP_CIRQTST.h"
#include "eDSP_DPKTST.h"
#include "eDSP_DUNPKTST.h"
#include "eDSP_DPKDUNPKTST.h"
#include "eDSP_BSTFTST.h"
#include "eDSP_BUSTFTST.h"
#include "eDSP_BSTFBUNSTFTST.h"

int main(void);


int main(void)
{
    /* Start testing */
    eDSP_CRCTST_ExeTest();
    eDSP_CRCDTST_ExeTest();
    eDSP_CIRQTST_ExeTest();
    eDSP_DPKTST_ExeTest();
    eDSP_DUNPKTST_ExeTest();
	eDSP_DPKDUNPKTST_ExeTest();
    eDSP_BSTFTST_ExeTest();
    eDSP_BUSTFTST_ExeTest();
    eDSP_BSTFBUNSTFTST_ExeTest();

    return 0;
}
