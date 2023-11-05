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



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eDSP_S2DPI32LINEAR_RES eDSP_S2DPI32LINEAR_Linearize( const t_eDSP_TYPE_2DPI32 p_tP1,  const t_eDSP_TYPE_2DPI32 p_tP2,
                                                       const int32_t p_uX, int32_t* const p_puY)
{
	/* Local variable */
	e_eDSP_S2DPI32LINEAR_RES l_eRes;
	t_eDSP_TYPE_2DPI32 l_tPFirst;
	t_eDSP_TYPE_2DPI32 l_tPSecond;

	/* Check pointer validity */
	if( NULL == p_puY )
	{
		l_eRes = e_eDSP_S2DPI32LINEAR_RES_BADPOINTER;
	}
	else
	{
		/* Check param, we must esclude from the calculation point with the same X */
		if( p_tP1.uX == p_tP2.uX )
		{
			l_eRes = e_eDSP_S2DPI32LINEAR_RES_BADPARAM;
		}
		else
		{
			/* No more problem */
			l_eRes = e_eDSP_S2DPI32LINEAR_RES_OK;

			/* Find the first point and then the second in order to avoid working with negative numbers */
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

			/* do calculation: y = m*x + q and m = yp/xp, we need to be carefull because we are not using floating point,
			   and we must reating as much precision as possible.
			   To not work with negative numbers scompose 3 different cases */

			*p_puY = ( p_uX * ( ( l_tPSecond.uY - l_tPFirst.uY ) / ( l_tPSecond.uX - l_tPFirst.uX ) ) ) + ;
		}
    }

	return l_eRes;
}

