#include "stdafx.h"

#include "ViewSettings.h"
#include "ViewManager.h"

STDMETHODIMP CViewSettings::get_ZoomToWindow ( VARIANT_BOOL* Value )
{
	*Value	=	VARIANT_FALSE;

	if ( NULL == m_pViewManager )
		return S_OK;

	if ( m_pViewManager->GetZoomToWindow () )
		*Value	=	VARIANT_TRUE;

	return S_OK;
}

STDMETHODIMP CViewSettings::put_ZoomToWindow ( VARIANT_BOOL Value )
{
	if ( NULL == m_pViewManager )
		return S_OK;

	if ( VARIANT_TRUE == Value )
		m_pViewManager->SetZoomToWindow ( true );
	else
		m_pViewManager->SetZoomToWindow ( false );

	return S_OK;
}

STDMETHODIMP CViewSettings::get_Zoom ( double* Value )
{
	if ( NULL == m_pViewManager )
	{
		*Value	=	0.0;

		return S_OK;
	}

	*Value	=	m_pViewManager->GetZoom ();

	return S_OK;
}

STDMETHODIMP CViewSettings::put_Zoom ( double Value )
{
	if ( NULL == m_pViewManager )
		return S_OK;

	m_pViewManager->SetZoom ( Value );

	return S_OK;
}

STDMETHODIMP CViewSettings::Refresh ()
{
	if ( NULL == m_pViewManager )
		return S_OK;

	HWND hWnd = m_pViewManager->GetHWND();
	if (IsWindow ( hWnd ) )
	{
		if ( NULL != hWnd )
		{
			m_pViewManager->SetRefresh ( ImageRendering::Constants::sc_nRefreshForRebuild );
			
			::InvalidateRect ( hWnd, NULL, TRUE );
		}
	}

	return S_OK;
}

STDMETHODIMP CViewSettings::GetProperty ( BSTR Property, VARIANT* pValue )
{
	if ( NULL != m_pViewManager )
	{	
		ImageRendering::EnumerateViewProperties::GetProperty ( m_pViewManager, Property, pValue );
	}

	return S_OK;
}

STDMETHODIMP CViewSettings::SetProperty ( BSTR Property, VARIANT Value )
{
	if ( NULL != m_pViewManager )
	{	
		ImageRendering::EnumerateViewProperties::SetProperty ( m_pViewManager, Property, Value );
	}

	return S_OK;
}


STDMETHODIMP CViewSettings::GetPropertyN ( long ID, VARIANT* pValue )
{
	if ( NULL == m_pViewManager )
		return S_OK;

	//CComBSTR PropertyName = CComBSTR ( Name );

	//if ( NViewSettingsConstants::g_csViewSettings_Zoom				== Name )
	//{
	//	pValue->vt				=	VT_R8;
	//	pValue->dblVal			=	m_pViewManager->GetZoom ();

	//	return S_OK;
	//}

	//if ( NViewSettingsConstants::g_csViewSettings_CenterX			== Name )
	//{
	//	pValue->vt			=	VT_R8;
	//	pValue->dblVal		=	m_pViewManager->GetCenterX ();

	//	return S_OK;
	//}

	//if ( NViewSettingsConstants::g_csViewSettings_CenterY			== Name )
	//{
	//	pValue->vt			=	VT_R8;
	//	pValue->dblVal		=	m_pViewManager->GetCenterY ();

	//	return S_OK;
	//}

	//if ( NViewSettingsConstants::g_csViewSettings_ImageWidth		== Name	)
	//{
	//	pValue->vt			=	VT_I4;
	//	pValue->lVal		=	(long)m_pViewManager->GetImageWidth();

	//	return S_OK;
	//}

	//if ( NViewSettingsConstants::g_csViewSettings_ImageHeight		== Name	)
	//{
	//	pValue->vt			=	VT_I4;
	//	pValue->lVal		=	(long)m_pViewManager->GetImageHeight();

	//	return S_OK;	
	//}

	//if ( NViewSettingsConstants::g_csViewSettings_PreviewLeft		== Name	)
	//{	
	//	pValue->vt			=	VT_I4;	
	//	pValue->lVal		=	(long)m_pViewManager->GetPreviewLeft();	

	//	return S_OK;	
	//}
	//if ( NViewSettingsConstants::g_csViewSettings_PreviewRight		== Name	)
	//{	
	//	pValue->vt			=	VT_I4;	
	//	pValue->lVal		=	(long)m_pViewManager->GetPreviewRight();	

	//	return S_OK;
	//}

	//if ( NViewSettingsConstants::g_csViewSettings_PreviewTop		== Name	)	
	//{	
	//	pValue->vt			=	VT_I4;
	//	pValue->lVal		=	(long)m_pViewManager->GetPreviewTop();	

	//	return S_OK;	
	//}

	//if ( NViewSettingsConstants::g_csViewSettings_PreviewBottom		== Name	)	
	//{	
	//	pValue->vt			=	VT_I4;
	//	pValue->lVal		=	(long)m_pViewManager->GetPreviewBottom();

	//	return S_OK;
	//}

	//if ( NViewSettingsConstants::g_csViewSettings_BackClearColor	== Name )
	//{
	//	pValue->vt			=	VT_UI4;
	//	pValue->lVal		=	m_pViewManager->GetBackClearColor();

	//	return S_OK;
	//}

	return S_OK;
}

STDMETHODIMP CViewSettings::SetPropertyN ( long ID, VARIANT Value )
{
	if ( NULL == m_pViewManager )
		return S_OK;

	//CComBSTR PropertyName = CComBSTR ( Name );

	//if ( NViewSettingsConstants::g_csViewSettings_Zoom					== Name )
	//{
	//	m_pViewManager->SetZoom ( Value.dblVal );

	//	return S_OK;
	//}

	//if ( NViewSettingsConstants::g_csViewSettings_CenterX				== Name )
	//{
	//	m_pViewManager->SetCenterX ( Value.dblVal );
	//	return S_OK;
	//}

	//if ( NViewSettingsConstants::g_csViewSettings_CenterY				== Name )
	//{
	//	m_pViewManager->SetCenterY ( Value.dblVal );
	//	return S_OK;
	//}

	//if ( NViewSettingsConstants::g_csViewSettings_EnableRullerUp		== Name )
	//{
	//	m_pViewManager->SetRullersUp ( ( Value.boolVal == VARIANT_TRUE ) ? true : false );
	//	return S_OK;
	//}
	//if ( NViewSettingsConstants::g_csViewSettings_EnableRullerDown		== Name )
	//{
	//	m_pViewManager->SetRullersDown ( ( Value.boolVal == VARIANT_TRUE ) ? true : false );
	//	return S_OK;
	//}

	//if ( NViewSettingsConstants::g_csViewSettings_EnableRullerLeft		== Name )
	//{
	//	m_pViewManager->SetRullersLeft ( ( Value.boolVal == VARIANT_TRUE ) ? true : false );
	//	return S_OK;
	//}

	//if ( NViewSettingsConstants::g_csViewSettings_EnableRullerRight		== Name )
	//{
	//	m_pViewManager->SetRullersRight ( ( Value.boolVal == VARIANT_TRUE ) ? true : false );

	//	return S_OK;
	//}


	//if ( NViewSettingsConstants::g_csViewSettings_BackClearColor		== Name )
	//{
	//	m_pViewManager->SetBackClearColor ( Value.ulVal );

	//	return S_OK;
	//}

	return S_OK;
}

