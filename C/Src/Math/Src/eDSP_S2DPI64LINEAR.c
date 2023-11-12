/**
 * @file       eDSP_S2DPI64LINEAR.c
 *
 * @brief      Single 2D point Linearization on a int64_t point
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eDSP_S2DPI64LINEAR.h"
#include "eDSP_MaxCheck.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static e_eDSP_S2DPI64LINEAR_RES eDSP_S2DPI64LINEAR_MaxCheckRestToS2DP(const e_eDSP_MAXCHECK_RES p_tMaxRet);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eDSP_S2DPI64LINEAR_RES eDSP_S2DPI64LINEAR_Linearize( const t_eDSP_TYPE_2DPI64 p_tP1, const t_eDSP_TYPE_2DPI64 p_tP2,
                                                       const int64_t p_uX, int64_t* const p_puY )
{
	/* Local variable for return */
	e_eDSP_S2DPI64LINEAR_RES l_eRes;
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
		l_eRes = e_eDSP_S2DPI64LINEAR_RES_BADPOINTER;
	}
	else
	{
		/* Check param, we must esclude from the calculation point with the same X, escluding do equals point and
		   point that can generate a rect angle rect */
		if( p_tP1.uX == p_tP2.uX )
		{
			l_eRes = e_eDSP_S2DPI64LINEAR_RES_BADPARAM;
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
			   rect definition -> y = m * x + q
			   we can find m doing -> m = dy/dx = ( Ysecond - Yfirst ) / ( Xsecond - Xfirst )
			   q is equals to -> q = Yfirst - m*Xfirst
			   and so y is -> y = m*x + q = m * x + Yfirst - m * Xfirst = m * ( x - Xfirst ) + Yfirst
			                    = ( ( Ysecond - Yfirst )  * ( x - Xfirst ) ) / ( Xsecond - Xfirst ) + Yfirst
								= ( ( A )  * ( B ) ) / ( C ) + Yfirst
			   we need to be carefull because we are not using floating point, and we must retain as much precision as
			   possible */

			/* Calculate single addend */
			l_eMaxRes = eDSP_MAXCHECK_SUBTI64Check(l_tPSecond.uY, l_tPFirst.uY);
			l_eRes = eDSP_S2DPI64LINEAR_MaxCheckRestToS2DP(l_eMaxRes);

			if( e_eDSP_S2DPI64LINEAR_RES_OK == l_eRes )
			{
				l_iA = l_tPSecond.uY - l_tPFirst.uY;

				l_eMaxRes = eDSP_MAXCHECK_SUBTI64Check(p_uX, l_tPFirst.uX);
				l_eRes = eDSP_S2DPI64LINEAR_MaxCheckRestToS2DP(l_eMaxRes);

				if( e_eDSP_S2DPI64LINEAR_RES_OK == l_eRes )
				{
					l_iB = p_uX - l_tPFirst.uX;

					l_eMaxRes = eDSP_MAXCHECK_SUBTI64Check(l_tPSecond.uX , l_tPFirst.uX);
					l_eRes = eDSP_S2DPI64LINEAR_MaxCheckRestToS2DP(l_eMaxRes);

					if( e_eDSP_S2DPI64LINEAR_RES_OK == l_eRes )
					{
						l_iC = l_tPSecond.uX - l_tPFirst.uX;

						l_eMaxRes = eDSP_MAXCHECK_MOLTIPI64Check(l_iA, l_iB);
						l_eRes = eDSP_S2DPI64LINEAR_MaxCheckRestToS2DP(l_eMaxRes);

						if( e_eDSP_S2DPI64LINEAR_RES_OK == l_eRes )
						{
							l_iAB = l_iA * l_iB;
							l_iABC = l_iAB / l_iC;

							l_eMaxRes = eDSP_MAXCHECK_SUMI64Check(l_iABC, l_tPFirst.uY);
							l_eRes = eDSP_S2DPI64LINEAR_MaxCheckRestToS2DP(l_eMaxRes);

							if( e_eDSP_S2DPI64LINEAR_RES_OK == l_eRes )
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
static e_eDSP_S2DPI64LINEAR_RES eDSP_S2DPI64LINEAR_MaxCheckRestToS2DP(const e_eDSP_MAXCHECK_RES p_tMaxRet)
{
	e_eDSP_S2DPI64LINEAR_RES l_eRet;

	if( e_eDSP_MAXCHECK_RES_OK == p_tMaxRet )
	{
		l_eRet = e_eDSP_S2DPI64LINEAR_RES_OK;
	}
	else
	{
		l_eRet = e_eDSP_S2DPI64LINEAR_RES_OUTLIMIT;
	}

	return l_eRet;
}