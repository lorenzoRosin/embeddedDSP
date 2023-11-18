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
static bool_t eDSP_M2DPI64LINEAR_IsStatusStillCoherent(t_eDSP_M2DPI64LINEAR_Ctx* const p_ptCtx);
static e_eDSP_M2DPI64LINEAR_RES eDSP_M2DPI64LINEAR_S2DPTRestoM2DP(const e_eDSP_S2DPI64LINEAR_RES p_eRet);
static bool_t eDSP_M2DPI64LINEAR_IsListValid(const t_eDSP_M2DPI64LINEAR_PointSeries p_tListCheck);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eDSP_M2DPI64LINEAR_RES eDSP_M2DPI64LINEAR_InitCtx(t_eDSP_M2DPI64LINEAR_Ctx* const p_ptCtx,
                                                    t_eDSP_M2DPI64LINEAR_PointSeries p_tSeries)
{
	/* Local variable */
	e_eDSP_M2DPI64LINEAR_RES l_eRes;
	bool_t l_bRet;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_tSeries.ptPointArray ) )
	{
		l_eRes = e_eDSP_M2DPI64LINEAR_RES_BADPOINTER;
	}
	else
	{
        /* Check data validity */
		l_bRet = eDSP_M2DPI64LINEAR_IsListValid(p_tSeries);

        if( false == l_bRet )
        {
            l_eRes = e_eDSP_M2DPI64LINEAR_RES_BADPARAM;
        }
        else
        {
            /* Initialize internal status */
            p_ptCtx->bIsInit = true;
            p_ptCtx->tPoinSeries = p_tSeries;

			/* All OK */
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

e_eDSP_M2DPI64LINEAR_RES eDSP_M2DPI64LINEAR_Linearize(t_eDSP_M2DPI64LINEAR_Ctx* const p_ptCtx, const int64_t p_uX,
                                                      int64_t* const p_puY)
{
	/* Local variable for return */
	e_eDSP_M2DPI64LINEAR_RES l_eRes;
	e_eDSP_S2DPI64LINEAR_RES l_eSingleRes;

	/* Local variable for calculation */
	t_eDSP_TYPE_2DPI64 l_tPFirst;
	t_eDSP_TYPE_2DPI64 l_tPSecond;
	uint32_t l_uIndx;


	/* Check pointer validity */
	if( NULL == p_puY )
	{
		l_eRes = e_eDSP_M2DPI64LINEAR_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == p_ptCtx->bIsInit )
		{
			l_eRes = e_eDSP_M2DPI64LINEAR_RES_NOINITLIB;
		}
		else
		{
            /* Check data coherence */
            if( false == eDSP_M2DPI64LINEAR_IsStatusStillCoherent(p_ptCtx) )
            {
                l_eRes = e_eDSP_M2DPI64LINEAR_RES_CORRUPTCTX;
            }
			else
			{
				/* Init vaiable */
				l_uIndx = 0u;

				/* Check corner case before starting */
				if( p_uX <= p_ptCtx->tPoinSeries.ptPointArray[0u].uX )
				{
					/* point request is pre data  */
					l_tPFirst = p_ptCtx->tPoinSeries.ptPointArray[0u];
					l_tPSecond = p_ptCtx->tPoinSeries.ptPointArray[1u];
				}
				else if( p_uX >= p_ptCtx->tPoinSeries.ptPointArray[ ( p_ptCtx->tPoinSeries.uNumPoint - 1u ) ].uX )
				{
					/* point request is post data  */
					l_tPFirst = p_ptCtx->tPoinSeries.ptPointArray[( p_ptCtx->tPoinSeries.uNumPoint - 2u )];
					l_tPSecond = p_ptCtx->tPoinSeries.ptPointArray[( p_ptCtx->tPoinSeries.uNumPoint - 1u )];
				}
				else
				{
					/* point request is inside data  */
					while( l_uIndx < ( p_ptCtx->tPoinSeries.uNumPoint - 1u ) )
					{


						if( ( p_uX >= p_ptCtx->tPoinSeries.ptPointArray[l_uIndx].uX ) &&
						    ( p_uX <= p_ptCtx->tPoinSeries.ptPointArray[l_uIndx+ 1u].uX ))
						{
							l_tPFirst = p_ptCtx->tPoinSeries.ptPointArray[l_uIndx];
							l_tPSecond = p_ptCtx->tPoinSeries.ptPointArray[l_uIndx + 1u];
						}

						/* Increase counter */
						l_uIndx++;
					}
				}

				/* Linearize */
				l_eSingleRes = eDSP_S2DPI64LINEAR_Linearize(l_tPFirst, l_tPSecond, p_uX, p_puY);
				l_eRes = eDSP_M2DPI64LINEAR_S2DPTRestoM2DP(l_eSingleRes);
			}
		}
    }

	return l_eRes;
}



/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
static bool_t eDSP_M2DPI64LINEAR_IsStatusStillCoherent(t_eDSP_M2DPI64LINEAR_Ctx* const p_ptCtx)
{
    /* Return local var */
    bool_t l_eRes;

	/* We must check only the coherence of the point array */
	l_eRes = eDSP_M2DPI64LINEAR_IsListValid(p_ptCtx->tPoinSeries);

    return l_eRes;
}

static e_eDSP_M2DPI64LINEAR_RES eDSP_M2DPI64LINEAR_S2DPTRestoM2DP(const e_eDSP_S2DPI64LINEAR_RES p_eRet)
{
    /* Return local var */
	e_eDSP_M2DPI64LINEAR_RES l_eRet;

	switch( p_eRet )
	{
		case e_eDSP_S2DPI64LINEAR_RES_OK:
		{
			l_eRet = e_eDSP_M2DPI64LINEAR_RES_OK;
			break;
		}

		case e_eDSP_S2DPI64LINEAR_RES_BADPOINTER:
		{
			l_eRet = e_eDSP_M2DPI64LINEAR_RES_BADPOINTER;
			break;
		}

		case e_eDSP_S2DPI64LINEAR_RES_BADPARAM:
		{
			l_eRet = e_eDSP_M2DPI64LINEAR_RES_BADPARAM;
			break;
		}

		case e_eDSP_S2DPI64LINEAR_RES_OUTLIMIT:
		{
			l_eRet = e_eDSP_M2DPI64LINEAR_RES_OUTLIMIT;
			break;
		}

		default:
		{
			l_eRet = e_eDSP_M2DPI64LINEAR_RES_CORRUPTCTX;
		}
	}


	return l_eRet;
}

static bool_t eDSP_M2DPI64LINEAR_IsListValid(const t_eDSP_M2DPI64LINEAR_PointSeries p_tListCheck)
{
    /* Return local var */
	bool_t l_bRet;

	/* Local variable for calculation */
	uint32_t l_uIndx;

	/* Check NULL pointer */
	if( NULL == p_tListCheck.ptPointArray )
	{
		l_bRet = false;
	}
	else
	{
		/* We need al least two point to linearize */
		if( p_tListCheck.uNumPoint <= 1u )
		{
			l_bRet = false;
		}
		else
		{
			/* Init vaiable */
			l_uIndx = 0u;
			l_bRet = true;

			/* Needs to check that the list is in order and that two point dosen't have any commonx x values */
			while( ( l_uIndx < ( p_tListCheck.uNumPoint - 1u ) ) && ( true == l_bRet )  )
			{
				if( p_tListCheck.ptPointArray[l_uIndx].uX >= p_tListCheck.ptPointArray[l_uIndx+ 1u].uX )
				{
					l_bRet = false;
				}

				/* Increase counter */
				l_uIndx++;
			}
		}
	}

	return l_bRet;
}