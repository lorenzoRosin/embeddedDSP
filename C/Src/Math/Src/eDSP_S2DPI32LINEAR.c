/**
 * @file       eDSP_S2DPI32LINEAR.c
 *
 * @brief      Single 2D point Linearization on a int32_t point
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eDSP_S2DPI32LINEAR.h"
#include "eDSP_MaxCheck.h"



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eDSP_S2DPI32LINEAR_RES eDSP_S2DPI32LINEAR_Linearize( const t_eDSP_TYPE_2DPI32 p_tP1, const t_eDSP_TYPE_2DPI32 p_tP2,
                                                       const int32_t p_uX, int32_t* const p_puY )
{
	/* Local variable for return */
	e_eDSP_S2DPI32LINEAR_RES l_eRes;

	/* Local variable for calculation */
	t_eDSP_TYPE_2DPI32 l_tPFirst;
	t_eDSP_TYPE_2DPI32 l_tPSecond;
	uint64_t l_iA;
	uint64_t l_iB;
	uint64_t l_iC;

	/* Check pointer validity */
	if( NULL == p_puY )
	{
		l_eRes = e_eDSP_S2DPI32LINEAR_RES_BADPOINTER;
	}
	else
	{
		/* Check param, we must esclude from the calculation point with the same X, escluding do equals point and
		   point that can generate a rect angle rect */
		if( p_tP1.uX == p_tP2.uX )
		{
			l_eRes = e_eDSP_S2DPI32LINEAR_RES_BADPARAM;
		}
		else
		{
			/* No more problem */
			l_eRes = e_eDSP_S2DPI32LINEAR_RES_OK;

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
			   we need to be carefull because we are not using floating point, and we must reteing as much precision as
			   possible */

			*p_puY =  ( ( l_tPSecond.uY - l_tPFirst.uY ) * ( l_tPSecond.uY - l_tPFirst.uY ) ) / ( l_tPSecond.uX - l_tPFirst.uX ) + ;
		}
    }

	return l_eRes;
}

