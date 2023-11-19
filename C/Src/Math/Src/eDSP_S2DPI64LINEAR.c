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
#include "eDSP_MAXCHECK.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static e_eDSP_S2DPI64LINEAR_RES eDSP_S2DPI64LINEAR_MaxCheckResToS2DP(const e_eDSP_MAXCHECK_RES p_tMaxRet);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eDSP_S2DPI64LINEAR_RES eDSP_S2DPI64LINEAR_Linearize(const t_eDSP_TYPE_2DPI64 p_tP1, const t_eDSP_TYPE_2DPI64 p_tP2,
                                                      const int64_t p_iX, int64_t* const p_piY)
{
	/* Local variable for return */
	e_eDSP_S2DPI64LINEAR_RES l_eRes;
	e_eDSP_MAXCHECK_RES l_eMaxRes;

	/* Local variable for calculation */
	uint64_t l_iA;
	uint64_t l_iB;
	uint64_t l_iC;
	uint64_t l_iAB;
	uint64_t l_iABC;

	/* Check pointer validity */
	if( NULL == p_piY )
	{
		l_eRes = e_eDSP_S2DPI64LINEAR_RES_BADPOINTER;
	}
	else
	{
		/* Check param, we must esclude from the calculation point with the same X, escluding so equals point and
		   point that can generate a line with rect angle */
		if( p_tP1.uX == p_tP2.uX )
		{
			l_eRes = e_eDSP_S2DPI64LINEAR_RES_BADPARAM;
		}
		else
		{
			/* Esclude from calculation precise point */
			if( p_tP1.uX == p_iX )
			{
				/* No need for calculation here */
				*p_piY = p_tP1.uY;

				/* All ok */
				l_eRes = e_eDSP_S2DPI64LINEAR_RES_OK;
			}
			else if( p_tP2.uX == p_iX )
			{
				/* No need for calculation here */
				*p_piY = p_tP2.uY;

				/* All ok */
				l_eRes = e_eDSP_S2DPI64LINEAR_RES_OK;
			}
			else
			{
				/* do calculation:
				   1) line definition ->  y = m * x + q
				   2) m definition    ->  m = dy / dx = ( Yp1 - Yp2 ) / ( Xp1 - Xp2 ) = ( Yp2 - Yp1 ) / ( Xp2 - Xp1 )
				   3) q definition    ->  q = y - m * x = Yp1 - m * Xp1 = Yp2 - m * Xp2
				   4) y calculation   ->  y = m * x + q = m * x + Yp1 - m * Xp1 = m * ( x - Xp1 ) + Yp1
									        = ( ( Yp2 - Yp1 )  * ( x - Xp1 ) ) / ( Xp2 - Xp1 ) + Yp1
									        = ( ( A )  * ( B ) ) / ( C ) + Yp1
				   Note: we need to be carefull because we are not using floating point, and we must retain as much
				         precision as possible */

				/* Calculate single addend */
				l_eMaxRes = eDSP_MAXCHECK_SUBTI64Check(p_tP2.uY, p_tP1.uY);
				l_eRes = eDSP_S2DPI64LINEAR_MaxCheckResToS2DP(l_eMaxRes);

				if( e_eDSP_S2DPI64LINEAR_RES_OK == l_eRes )
				{
					l_iA = p_tP2.uY - p_tP1.uY;

					l_eMaxRes = eDSP_MAXCHECK_SUBTI64Check(p_iX, p_tP1.uX);
					l_eRes = eDSP_S2DPI64LINEAR_MaxCheckResToS2DP(l_eMaxRes);

					if( e_eDSP_S2DPI64LINEAR_RES_OK == l_eRes )
					{
						l_iB = p_iX - p_tP1.uX;

						l_eMaxRes = eDSP_MAXCHECK_SUBTI64Check(p_tP2.uX , p_tP1.uX);
						l_eRes = eDSP_S2DPI64LINEAR_MaxCheckResToS2DP(l_eMaxRes);

						if( e_eDSP_S2DPI64LINEAR_RES_OK == l_eRes )
						{
							l_iC = p_tP2.uX - p_tP1.uX;

							l_eMaxRes = eDSP_MAXCHECK_MOLTIPI64Check(l_iA, l_iB);
							l_eRes = eDSP_S2DPI64LINEAR_MaxCheckResToS2DP(l_eMaxRes);

							if( e_eDSP_S2DPI64LINEAR_RES_OK == l_eRes )
							{
								l_iAB = l_iA * l_iB;
								l_iABC = l_iAB / l_iC;

								l_eMaxRes = eDSP_MAXCHECK_SUMI64Check(l_iABC, p_tP1.uY);
								l_eRes = eDSP_S2DPI64LINEAR_MaxCheckResToS2DP(l_eMaxRes);

								if( e_eDSP_S2DPI64LINEAR_RES_OK == l_eRes )
								{
									*p_piY = l_iABC + p_tP1.uY;
								}
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
static e_eDSP_S2DPI64LINEAR_RES eDSP_S2DPI64LINEAR_MaxCheckResToS2DP(const e_eDSP_MAXCHECK_RES p_tMaxRet)
{
	/* Local variable for return */
	e_eDSP_S2DPI64LINEAR_RES l_eRet;

	if( e_eDSP_MAXCHECK_RES_OK == p_tMaxRet )
	{
		l_eRet = e_eDSP_S2DPI64LINEAR_RES_OK;
	}
	else
	{
		l_eRet = e_eDSP_S2DPI64LINEAR_RES_OVERFLOW;
	}

	return l_eRet;
}