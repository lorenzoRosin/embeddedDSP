/**
 * @file       eDSP_MaxCheck.h
 *
 * @brief      Check if some operation exceed storage limits
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eDSP_MaxCheck.h"



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eDSP_MAXCHECK_RES eDSP_MAXCHECK_SUMI64Check( const int64_t p_iFirst, const int64_t p_iSecond )
{
	/* Local variable for return */
	e_eDSP_MAXCHECK_RES l_eRes;

	/* Need to check every condition */
	if( ( p_iFirst > 0 ) && ( p_iSecond > 0 ) )
	{
		if( p_iFirst > ( MAX_INT64VAL - p_iSecond ) )
		{
			l_eRes = e_eDSP_MAXCHECK_RES_OK;
		}
		else
		{
			l_eRes = e_eDSP_MAXCHECK_OUTLIMIT;
		}
	}
	else if( ( p_iFirst < 0 ) && ( p_iSecond < 0 ) )
	{
		if( p_iFirst < ( MIN_INT64VAL - p_iSecond ) )
		{
			l_eRes = e_eDSP_MAXCHECK_RES_OK;
		}
		else
		{
			l_eRes = e_eDSP_MAXCHECK_OUTLIMIT;
		}
	}
	else
	{
		/* No needs for other check */
		l_eRes = e_eDSP_MAXCHECK_RES_OK;
	}

	return l_eRes;
}

e_eDSP_MAXCHECK_RES eDSP_MAXCHECK_SUBTI64Check( const int64_t p_iFirst, const int64_t p_iSecond )
{
	/* Local variable for return */
	e_eDSP_MAXCHECK_RES l_eRes;

	l_eRes = eDSP_MAXCHECK_SUMI64Check( p_iFirst, (int64_t) -p_iSecond );

	return l_eRes;
}

e_eDSP_MAXCHECK_RES eDSP_MAXCHECK_MOLTIPI64Check( const int64_t p_iFirst, const int64_t p_iSecond )
{
	/* Local variable for return */
	e_eDSP_MAXCHECK_RES l_eRes;

	/* Need to check every condition */
	if( ( p_iFirst == 0 ) || ( p_iSecond == 0 ) )
	{
		l_eRes = e_eDSP_MAXCHECK_RES_OK;
	}
	else if( ( p_iFirst > 0 ) && ( p_iSecond > 0 ) )
	{
		if( p_iFirst > ( MAX_INT64VAL / p_iSecond ) )
		{
			l_eRes = e_eDSP_MAXCHECK_OUTLIMIT;
		}
		else
		{
			l_eRes = e_eDSP_MAXCHECK_RES_OK;
		}
	}
	else if( ( p_iFirst < 0 ) && ( p_iSecond < 0 ) )
	{
		if( p_iFirst < ( MAX_INT64VAL / p_iSecond ) )
		{
			l_eRes = e_eDSP_MAXCHECK_OUTLIMIT;
		}
		else
		{
			l_eRes = e_eDSP_MAXCHECK_RES_OK;
		}
	}
	else if( ( p_iFirst > 0 ) && ( p_iSecond < 0 ) )
	{
		if( p_iFirst > ( MAX_INT64VAL / p_iSecond ) )
		{
			l_eRes = e_eDSP_MAXCHECK_OUTLIMIT;
		}
		else
		{
			l_eRes = e_eDSP_MAXCHECK_RES_OK;
		}
	}
	else
	{
		if( p_iFirst < ( MAX_INT64VAL / p_iSecond ) )
		{
			l_eRes = e_eDSP_MAXCHECK_OUTLIMIT;
		}
		else
		{
			l_eRes = e_eDSP_MAXCHECK_RES_OK;
		}
	}





	return l_eRes;
}