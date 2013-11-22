#pragma once

#include <atlcoll.h>

#include "IAnimationLayer.h" 
#include "AnimationFramework.h"

using namespace Animation;

[
	coclass,
	default(IAnimationLayer),
	threading(apartment),
	vi_progid("AVSGraphics.COMAnimationLayer"),
	progid("AVSGraphics.COMAnimationLayer.1"),
	version(1.0),
	uuid("4BEDD28A-C9BB-4518-A700-4F2F0A2C2FA1"),
	helpstring("CCOMAnimationLayer Class"),
	registration_script("control.rgs")
]

class ATL_NO_VTABLE CCOMAnimationLayer : public IAnimationLayer
{
public:
	
	CCOMAnimationLayer()
	{
		m_pShowElement		=	NULL;
		m_pLayer			=	NULL;

		m_dBeginTime		=	0.0;
		m_dEndTime			=	0.0;
		m_dDuration			=	0.0;

		m_pFontManagerREF	=	NULL;
	}

	~CCOMAnimationLayer()
	{
		m_pFontManagerREF	=	NULL;

		RELEASEOBJECT (m_pShowElement);
		RELEASEOBJECT (m_pLayer);

		for ( int i = 0; i < (int)m_arrLayers.GetCount (); ++i )
			RELEASEINTERFACE (m_arrLayers[i]);
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:	

	// IAnimationLayer

	STDMETHOD(LoadFromXml)(BSTR Xml)
	{
		CBuilderAnimateDrawers	oBuilder (m_pFontManagerREF);

		XmlUtils::CXmlNode oXmlNode;
		if ( oXmlNode.FromXmlString ( (CString) Xml ) )
		{
			IShowElement* pObj	=	oBuilder.FromXml ( oXmlNode );
			if ( pObj )
			{
				CAnimationLayer* pLayer	=	new CAnimationLayer ();
				if ( pLayer )
				{
					XmlUtils::CXmlNode oXmlLayer;
					if ( oXmlNode.GetNode(_T("animations"), oXmlLayer ) )
					{
						if (pLayer->GetBuilder().From(oXmlLayer.GetXml()))
						{
							pObj->SetBaseLayer ( pLayer );

							RELEASEOBJECT ( m_pShowElement );
							RELEASEOBJECT ( m_pLayer );

							m_pShowElement	=	pObj;
							m_pLayer		=	pLayer;

							return S_OK;
						}
					}
					else
					{
						pObj->SetBaseLayer ( NULL );	//	STATIC OBJECT

						RELEASEOBJECT ( m_pShowElement );
						RELEASEOBJECT ( m_pLayer );

						m_pShowElement		=	pObj;

						return S_OK;
					}

					RELEASEOBJECT ( pLayer );
				}

				RELEASEOBJECT ( pObj );
			}
		}

		return S_OK;
	}

	STDMETHOD(AddSubLayer)(IUnknown* pUnk)	
	{
		if ( pUnk )
		{
			IAnimationLayer* pLayer = NULL;
			if ( SUCCEEDED ( pUnk->QueryInterface ( &pLayer ) ) )
			{
				pUnk->Release ();

				if ( pLayer )
				{
					pLayer->AddRef ();
					m_arrLayers.Add ( pLayer );

					return S_OK;
				}			
			}

			ISStream* pStream = NULL;
			if ( SUCCEEDED ( pUnk->QueryInterface ( &pStream ) ) )
			{
				pUnk->Release ();

				if (pStream)
				{
					if(m_pShowElement)
					{
						CStream* pUserStream = static_cast<CStream*> (m_pShowElement);
						if (pUserStream)
						{
							pUserStream->SetStream (pStream);

							if (NULL == m_pLayer)
							{
								m_pLayer = new CAnimationLayer ();

								if (m_pLayer)
									m_pShowElement->SetBaseLayer (m_pLayer);
							}

							return S_OK;
						}
					}
				}
			}
		}

		return S_OK;
	}

	
	STDMETHOD(get_BeginTime)(double* dTime)
	{
		*dTime			=	m_dBeginTime;

		return S_OK;
	}

	STDMETHOD(put_BeginTime)(double dTime)
	{
		m_dBeginTime	=	dTime;
		
		m_dDuration		=	m_dEndTime - m_dBeginTime;

		return S_OK;
	}
	STDMETHOD(get_EndTime)(double* dTime)
	{
		*dTime			=	m_dEndTime;

		return S_OK;
	}

	STDMETHOD(put_EndTime)(double dTime)
	{
		m_dEndTime		=	dTime;

		m_dDuration		=	m_dEndTime - m_dBeginTime;

		return S_OK;
	}

	STDMETHOD(get_Duration)(double* dTime)
	{
		*dTime			=	m_dDuration;
		return S_OK;
	}


	STDMETHOD(get_IsStatic)(VARIANT_BOOL* bStatic)
	{
		*bStatic = VARIANT_TRUE;

		VARIANT_BOOL isStatic = VARIANT_TRUE;
		for ( int i = 0; i < (int)m_arrLayers.GetCount (); ++i )
		{
			m_arrLayers[i]->get_IsStatic ( &isStatic );
			if ( VARIANT_FALSE == isStatic )
			{
				*bStatic = VARIANT_FALSE;
				return S_OK;
			}
		}

		if (m_pShowElement)
		{
			if (FALSE == m_pShowElement->IsStatic ())
			{
				*bStatic = VARIANT_FALSE;
				return S_OK;
			}
		}

		return S_OK;
	}

	STDMETHOD(ClearCache)()
	{
		if (m_pShowElement)
			m_pShowElement->ClearCache ();

		for ( long i = 0; i < (long)m_arrLayers.GetCount (); ++i )
			m_arrLayers[i]->ClearCache ();

		return S_OK;
	}
	
	STDMETHOD(Draw)(IAVSRenderer* pCommander, double dTime)
	{
		if ( m_dBeginTime <= dTime && dTime < m_dEndTime )
		{	
			if ( m_pLayer && m_pShowElement )
			{
				m_pLayer->SetTime (dTime - m_dBeginTime);
				m_pShowElement->Render (pCommander);
			}

			if ( NULL == m_pLayer && m_pShowElement )
			{
				m_pShowElement->Render (pCommander);
			}

			for ( long i = 0; i < (long)m_arrLayers.GetCount (); ++i )
			{
				m_arrLayers[i]->Draw (pCommander, dTime);
			}
		}

		return S_OK;
	}

	
	// NOT IMPLEMENTED
	STDMETHOD(DrawOnMediaData)(IUnknown* punkFrame, double dTime)	
	{
		return S_OK;
	}

	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		if ( CString ( L"InternalCache" ) == CComBSTR ( ParamName ) )
		{
			if ( m_pShowElement )
			{
				m_pShowElement->SetInternalCache ( (ParamValue.boolVal==VARIANT_TRUE) ? TRUE : FALSE );
			}

			for ( long i = 0; i < (long)m_arrLayers.GetCount (); ++i )
			{
				m_arrLayers[i]->SetAdditionalParam ( ParamName, ParamValue );
			}
		}

		if ( CString ( L"FontManager" ) == CComBSTR (ParamName) )
		{
			m_pFontManagerREF = NULL;

			if (ParamValue.punkVal)
			{
				ParamValue.punkVal->QueryInterface (&m_pFontManagerREF);
				ParamValue.punkVal->Release();
			}

			if (m_pFontManagerREF)
			{
				for ( long i = 0; i < (long)m_arrLayers.GetCount (); ++i )
					m_arrLayers[i]->SetAdditionalParam (ParamName, ParamValue);
			}
		}

		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}

public:

	inline void SetCacheManager (CCacheStorage* pCache)
	{
		if (m_pShowElement)
			m_pShowElement->SetCacheReferences(pCache);

		for ( long i = 0; i < (long)m_arrLayers.GetCount (); ++i )
		{
			CCOMAnimationLayer* pComLayer	=	static_cast<CCOMAnimationLayer*> (m_arrLayers[i]);
			if (pComLayer)
			{
				pComLayer->SetCacheManager (pCache);
			}
		}
	}

protected:

	double							m_dBeginTime;
	double							m_dEndTime;

	double							m_dDuration;			//	recalculate auto

	IShowElement*					m_pShowElement;
	CAnimationLayer*				m_pLayer;

	CAtlArray <IAnimationLayer*>	m_arrLayers;
	IAVSFontManager*				m_pFontManagerREF;
};
