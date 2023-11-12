/**
 * @file       eDSP_M2DPI64LINEAR.h
 *
 * @brief      Multiple 2D point Linearization on a int64_t
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eDSP_M2DPI64LINEAR.h"
#include "eDSP_S2DPI64LINEAR.h"
#include "eDSP_MaxCheck.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSS_DB_IsStatusStillCoherent(t_eDSP_M2DPI64LINEAR_Ctx* const p_ptCtx);
static e_eDSP_M2DPI64LINEAR_RES eDSP_M2DPI64LINEAR_MaxCheckRestToS2DP(const e_eDSP_MAXCHECK_RES p_tMaxRet);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eDSP_M2DPI64LINEAR_RES eDSP_M2DPI64LINEAR_InitCtx(t_eDSP_M2DPI64LINEAR_Ctx* const p_ptCtx,
                                                    t_eDSP_M2DPI64LINEAR_PointSeries p_tSeries)
{
	/* Local variable */
	e_eDSP_M2DPI64LINEAR_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuff ) )
	{
		l_eRes = e_eDSP_M2DPI64LINEAR_RES_BADPOINTER;
	}
	else
	{
        /* Check data validity */
        if( p_uBuffL <= 0u )
        {
            l_eRes = e_eDSP_M2DPI64LINEAR_RES_BADPARAM;
        }
        else
        {
            /* Initialize internal status */
            p_ptCtx->bIsInit = true;
            p_ptCtx->puBuff = p_puBuff;
            p_ptCtx->uBuffL = p_uBuffL;
            p_ptCtx->uFrameL = 0u;
            p_ptCtx->uFrameCtr = 0u;
            p_ptCtx->eSM = e_eDSP_M2DPI64LINEARPRV_SM_NEEDSOF;

            l_eRes = e_eDSP_M2DPI64LINEAR_RES_OK;
        }
	}

	return l_eRes;
}

e_eDSP_M2DPI64LINEAR_RES eDSP_M2DPI64LINEAR_IsInit(t_eDSP_M2DPI64LINEAR_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eDSP_M2DPI64LINEAR_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eDSP_M2DPI64LINEAR_RES_BADPOINTER;
	}
	else
	{
        *p_pbIsInit = p_ptCtx->bIsInit;
        l_eRes = e_eDSP_M2DPI64LINEAR_RES_OK;
	}

	return l_eRes;
}

e_eDSP_M2DPI64LINEAR_RES eDSP_M2DPI64LINEAR_InitCtx(t_eDSP_M2DPI64LINEAR_Ctx* const p_ptCtx,
                                                    t_eDSP_M2DPI64LINEAR_PointSeries p_tSeries)
{
	/* Local variable for return */
	e_eDSP_M2DPI64LINEAR_RES l_eRes;
	e_eDSP_MAXCHECK_RES l_eMaxRes;

	/* Local variable for calculation */
	t_eDSP_TYPE_2DPI64 l_tPFirst;
	t_eDSP_TYPE_2DPI64 l_tPSecond;
	uint64_t l_iA;
	uint64_t l_iB;
	uint64_t l_iC;
	uint64_t l_iAB;
	uint64_t l_iABC;
	/* Check pointer validity */
	if( NULL == p_puY )
	{
		l_eRes = e_eDSP_M2DPI64LINEAR_RES_BADPOINTER;
	}
	else
	{
		/* Check param, we must esclude from the calculation point with the same X, escluding do equals point and
		   point that can generate a line angle line */
		if( p_tP1.uX == p_tP2.uX )
		{
			l_eRes = e_eDSP_M2DPI64LINEAR_RES_BADPARAM;
		}
		else
		{
			/* Find the first point and then the second*/
			if( p_tP1.uX <= p_tP2.uX )
			{
				l_tPFirst = p_tP1;
				l_tPSecond = p_tP2;
			}
			else
			{
				l_tPFirst = p_tP2;
				l_tPSecond = p_tP1;
			}

			/* do calculation:
			   line definition -> y = m * x + q
			   we can find m doing -> m = dy/dx = ( Ysecond - Yfirst ) / ( Xsecond - Xfirst )
			   q is equals to -> q = Yfirst - m*Xfirst
			   and so y is -> y = m*x + q = m * x + Yfirst - m * Xfirst = m * ( x - Xfirst ) + Yfirst
			                    = ( ( Ysecond - Yfirst )  * ( x - Xfirst ) ) / ( Xsecond - Xfirst ) + Yfirst
								= ( ( A )  * ( B ) ) / ( C ) + Yfirst
			   we need to be carefull because we are not using floating point, and we must retain as much precision as
			   possible */

			/* Calculate single addend */
			l_eMaxRes = eDSP_MAXCHECK_SUBTI64Check(l_tPSecond.uY, l_tPFirst.uY);
			l_eRes = eDSP_M2DPI64LINEAR_MaxCheckRestToS2DP(l_eMaxRes);

			if( e_eDSP_M2DPI64LINEAR_RES_OK == l_eRes )
			{
				l_iA = l_tPSecond.uY - l_tPFirst.uY;

				l_eMaxRes = eDSP_MAXCHECK_SUBTI64Check(p_uX, l_tPFirst.uX);
				l_eRes = eDSP_M2DPI64LINEAR_MaxCheckRestToS2DP(l_eMaxRes);

				if( e_eDSP_M2DPI64LINEAR_RES_OK == l_eRes )
				{
					l_iB = p_uX - l_tPFirst.uX;

					l_eMaxRes = eDSP_MAXCHECK_SUBTI64Check(l_tPSecond.uX , l_tPFirst.uX);
					l_eRes = eDSP_M2DPI64LINEAR_MaxCheckRestToS2DP(l_eMaxRes);

					if( e_eDSP_M2DPI64LINEAR_RES_OK == l_eRes )
					{
						l_iC = l_tPSecond.uX - l_tPFirst.uX;

						l_eMaxRes = eDSP_MAXCHECK_MOLTIPI64Check(l_iA, l_iB);
						l_eRes = eDSP_M2DPI64LINEAR_MaxCheckRestToS2DP(l_eMaxRes);

						if( e_eDSP_M2DPI64LINEAR_RES_OK == l_eRes )
						{
							l_iAB = l_iA * l_iB;
							l_iABC = l_iAB / l_iC;

							l_eMaxRes = eDSP_MAXCHECK_SUMI64Check(l_iABC, l_tPFirst.uY);
							l_eRes = eDSP_M2DPI64LINEAR_MaxCheckRestToS2DP(l_eMaxRes);

							if( e_eDSP_M2DPI64LINEAR_RES_OK == l_eRes )
							{
								*p_puY = l_iABC + l_tPFirst.uY;
							}
						}
					}
				}
			}
		}
    }

	return l_eRes;
}



/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
static bool_t eFSS_DB_IsStatusStillCoherent(t_eDSP_M2DPI64LINEAR_Ctx* const p_ptCtx)
{
    /* Return local var */
    bool_t l_eRes;
    e_eFSS_DBC_RES l_eDBCRes;

    /* Local variable for storage */
    t_eFSS_DBC_StorBuf l_tBuff;
    uint32_t l_uTotPage;

    /* Get usable pages and buffer length so we can check database default value validity */
    l_uTotPage = 0u;
    l_eDBCRes = eFSS_DBC_GetBuffNUsable(&p_ptCtx->tDbcCtx, &l_tBuff, &l_uTotPage);

    if( e_eFSS_DBC_RES_OK != l_eDBCRes )
    {
        l_eRes = false;
    }
    else
    {
        /* Check data validity */
        if( ( l_uTotPage <= 0u ) || ( l_tBuff.uBufL < EFSS_DB_MINPAGESIZE ) )
        {
            l_eRes = false;
        }
        else
        {
            /* Check validity of the passed db struct */
            l_eRes = eFSS_DB_IsDbDefStructValid(p_ptCtx->tDB, l_uTotPage, l_tBuff.uBufL);
        }
    }

    return l_eRes;
}

static e_eDSP_M2DPI64LINEAR_RES eDSP_M2DPI64LINEAR_MaxCheckRestToS2DP(const e_eDSP_MAXCHECK_RES p_tMaxRet)
{
	e_eDSP_M2DPI64LINEAR_RES l_eRet;

	if( e_eDSP_MAXCHECK_RES_OK == p_tMaxRet )
	{
		l_eRet = e_eDSP_M2DPI64LINEAR_RES_OK;
	}
	else
	{
		l_eRet = e_eDSP_M2DPI64LINEAR_RES_OUTLIMIT;
	}

	return l_eRet;
}