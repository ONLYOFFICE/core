#pragma once

#include "IViewSettings.h"
#include "ViewManager.h"

class ImageRendering::CViewManager;

[
	coclass,
	threading(single),
	vi_progid("RR.ViewSettings"),
	progid("RR.ViewSettings.1"),
	version(1.0),
	uuid("B23D0A34-C9B2-4D9D-A1B2-CC66B067463C"),
	helpstring("ViewSettings Class")
]

class CViewSettings : public IViewSettings
{
public:
	CViewSettings ()
	{
		m_pViewManager = NULL;

	}
	~CViewSettings ()
	{

	}

	void SetViewPtr ( ImageRendering::CViewManager* pViewManager )
	{
		m_pViewManager	=	pViewManager;
	}

	
	STDMETHOD ( IViewSettings::get_ZoomToWindow ) ( VARIANT_BOOL* Value );
	STDMETHOD ( IViewSettings::put_ZoomToWindow ) ( VARIANT_BOOL Value );
	STDMETHOD ( IViewSettings::get_Zoom ) ( double* Value );
	STDMETHOD ( IViewSettings::put_Zoom ) ( double Value );
	
	STDMETHOD ( IViewSettings::Refresh ) ( );
	STDMETHOD ( IViewSettings::SetProperty ) ( BSTR Property, VARIANT Value );
	STDMETHOD ( IViewSettings::GetProperty ) ( BSTR Property, VARIANT* pValue );
	STDMETHOD ( IViewSettings::SetPropertyN ) ( long ID, VARIANT Value );
	STDMETHOD ( IViewSettings::GetPropertyN ) ( long ID, VARIANT* pValue );

private:

	ImageRendering::CViewManager*   m_pViewManager;
};
