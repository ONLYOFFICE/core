// CAVSOfficeViewer.h : Declaration of the CAVSOfficeViewer

#pragma once
#include "stdafx.h"
#include "resource.h"       // main symbols
#include "Registration.h"

#include "XmlUtils.h"
#include "AvsUtils.h"
#include "ImageSerializeObjects2.h"
#include "OfficeCommandsRenderer.h"
#include "GdiPlusEx.h"
#include "Common.h"
#include "TimeMeasurer.h"

// шифрование XML
#include "RSA\XMLDecoder.h"

// чтоб не копировать - вставим из ремейкера
#include "BaseThread.h"

using namespace ImageStudio::Serialize::Paint::Common;

// IAVSOfficeViewer
[object, uuid("0CE42558-9A94-4815-A2F9-8B0E673AF81F"), dual, pointer_default(unique)]
__interface IAVSOfficeViewer : IDispatch
{
	[id(0)] HRESULT SetXml([in] BSTR bstrXml);
	
	[id(1)] HRESULT AddRenderer([in] IUnknown* punkRenderer);
	[id(2)] HRESULT RemoveRenderer([in] IUnknown* punkRenderer);

	[id(10)] HRESULT Start();
	[id(11)] HRESULT Stop();
	[id(12)] HRESULT Suspend();
	[id(13)] HRESULT Resume();

	[id(50), propget] HRESULT Status([out, retval] LONG* pStatus);
	
	[id(60), propget] HRESULT Priority([out, retval] LONG* pPriority);
	[id(60), propput] HRESULT Priority([in] LONG Priotity);

	[id(2000), helpstring("method SetAdditionalParam")] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(2001), helpstring("method GetAdditionalParam")] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};

[dispinterface, uuid("53143587-2E56-4d64-BBDA-F0983D89CEB8")]
__interface _IAVSOfficeViewerEvents
{
	[id(0)] HRESULT OnError(LONG lError);
	[id(1)] HRESULT OnStop();

	[id(5)] HRESULT OnProgress(LONG lProgressPage);
	[id(6)] HRESULT OnProgressParce(LONG lType, LONG lProgress);

	// это, например, чтобы подсунуть нового gdiplus-ного рендерера
	[id(10)] HRESULT OnNewPage(double dWidthMm, double dHeightMm);
	[id(11)] HRESULT OnCompletePage();
};

// CAVSOfficeViewer
[coclass, uuid("B1C2CDC2-F922-4620-8119-BFA06A351BDF"), event_source(com), threading(apartment), vi_progid("AVSOfficeViewer.OfficeViewer"), progid("AVSOfficeViewer.OfficeViewer.1"), version(1.0)]
class ATL_NO_VTABLE CAVSOfficeViewer :	
	public IAVSOfficeViewer, 
	public CBaseThread,
	public CEventReceiver,
	public CTextMeasurer
{
protected:

	class CPageInfo
	{
	public:
		CString m_strPageInfo;

		double m_dWidthMetric;
		double m_dHeightMetric;
		
		CBaseThread* m_pThread;
		CEventReceiver* m_pReceiver;

	public:
		CPageInfo() : 
			m_pThread(NULL),
			m_pReceiver(NULL),
			m_strPageInfo(_T("")),
			m_dWidthMetric(0.0),
			m_dHeightMetric(0.0)
		{
		}
		CPageInfo(const CPageInfo& oSrc)
		{
			*this = oSrc;
		}
		CPageInfo& operator=(const CPageInfo& oSrc)
		{
			m_strPageInfo = oSrc.m_strPageInfo;
			m_pThread = oSrc.m_pThread;
			m_pReceiver = oSrc.m_pReceiver;

			m_dWidthMetric = oSrc.m_dWidthMetric;
			m_dHeightMetric = oSrc.m_dHeightMetric;

			return *this;
		}

	public:

		void Render(IAVSOfficeRendererTemplate* pRenderer, CTextMeasurer* pMeasurer, double dDPIX, double dDPIY)
		{
			if ((NULL == pRenderer) || (NULL == pMeasurer) || (NULL == m_pThread))
				return;

			XmlUtils::CXmlNode oNodeSource;
			oNodeSource.FromXmlString(m_strPageInfo);

			if (g_cpszXMLImageSource == oNodeSource.GetName())
			{
				CString strFileName = oNodeSource.GetAttributeOrValue(_T("FilePath"));

				float fX = 0;
				float fY = 0;
				float fWidth	= m_dWidthMetric;
				float fHeight	= m_dHeightMetric;

				IAVSCommandsRenderer* pRend2 = NULL;
				pRenderer->QueryInterface(__uuidof(IAVSCommandsRenderer), (void**)&pRend2);

				if (NULL != pRend2)
				{
					pRenderer->SetWidth(fWidth);
					pRenderer->SetHeight(fHeight);
					
					BSTR bstrFile = strFileName.AllocSysString();
					pRend2->DrawImageFromFile(bstrFile, fX, fY, fWidth, fHeight);
					SysFreeString(bstrFile);
					RELEASEINTERFACE(pRend2);
				}
				else
				{
					ImageStudio::Serialize::Paint::Common::CDrawImageFromFile oImagePainter;
					oImagePainter.FilePath = strFileName;
					oImagePainter.Left		= fX;
					oImagePainter.Top		= fY;
					oImagePainter.Right		= fX + fWidth;
					oImagePainter.Bottom	= fY + fHeight;
					oImagePainter.m_dWidthMM	= m_dWidthMetric;
					oImagePainter.m_dHeightMM	= m_dHeightMetric;

					oImagePainter.Draw(pRenderer, dDPIX, dDPIY);
				}

				return;
			}

			XmlUtils::CXmlNode oNodeVideoTransforms;
			if ( oNodeSource.GetNode(g_cpszXMLVideoTransforms, oNodeVideoTransforms) )
			{
				XmlUtils::CXmlReader oReader;
				if (oReader.SetXmlString(oNodeVideoTransforms.GetXml()) && oReader.ReadNodeList(_T("*")))
				{
					int nEffectCount = oReader.GetLengthList();

					if (0 < nEffectCount)
					{
						for (int i = 0; (i < nEffectCount) && (m_pThread->IsRunned()); ++i)
						{
							if (NULL != m_pReceiver)
							{
								m_pReceiver->OnSendEvent(OFFICEEDITOR_PROGRESS_TYPE_ELEMENT, 100 * (i + 1) / nEffectCount, 0);
							}
							
							XmlUtils::CXmlNode oNodeEffect;
							oNodeEffect.FromXmlString(oReader.ReadNodeXml(i));
							
							CString sNodeName = oNodeEffect.GetName();
							if (g_cpszXMLEffectText == sNodeName)
							{
								if (NULL != pMeasurer)
								{
									pMeasurer->InternalFromXmlNode(oNodeEffect);

									if (!pMeasurer->IsFormated())
									{
										pMeasurer->InitDrawText(pRenderer, 720, 576);
									}

									pMeasurer->DrawText(pRenderer, 1.0);							
								}
							}
							else if (g_cpszXMLPath == sNodeName)
							{
								ImageStudio::Serialize::Paint::Common::CDrawGraphicPath oPath;
								oPath.InternalFromXmlNode(oNodeEffect);

								oPath.Draw(pRenderer);
								pRenderer->PathCommandEnd();
							}
							else if (g_cpszXMLImage == sNodeName)
							{
								ImageStudio::Serialize::Paint::Common::CDrawImageFromFile oImagePainter;
								oImagePainter.InternalFromXmlNode(oNodeEffect);

								BOOL bNeedDraw = TRUE;
								if (-1 == dDPIX && -1 == dDPIY)
								{
									IAVSCommandsRenderer* pCommandRenderer = NULL;
									pRenderer->QueryInterface(__uuidof(IAVSCommandsRenderer), (void**)&pCommandRenderer);

									if (NULL != pCommandRenderer)
									{
										oImagePainter.DrawFromFile(pRenderer);
										RELEASEINTERFACE(pCommandRenderer);
										bNeedDraw = FALSE;
									}
								}

								if (bNeedDraw)
								{
									oImagePainter.Draw(pRenderer, dDPIX, dDPIY);
								}
							}

							m_pThread->CheckSuspend();
						}
					}
				}
			}
		}
	};

protected:

	CGdiPlusInit m_oInit;
	CXMLDecoder	m_oDecoder;

	// массив xml-ок, для видео сорса.
	// может формироваться двумя способами.
	// 1) просто распарсить xml videosource
	// 2) с помощью использования XmlRenderer'а, который будет формировать 
	// страницы, по xml-fo
	CAtlArray<CPageInfo> m_arPages;

	// xsl-fo
	CString m_strFoXml;

	// ну и самое главное...
	CAtlArray<IAVSOfficeRendererTemplate*> m_arRenderers;

public:
	__event __interface _IAVSOfficeViewerEvents;

	CAVSOfficeViewer() : m_arRenderers(), m_arPages(), CBaseThread(0)
	{
		m_oInit.Init();

		m_strFoXml = _T("");
	}

	~CAVSOfficeViewer()
	{
		Stop();
		
		size_t nCount = m_arRenderers.GetCount();
		for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
		{
			RELEASEINTERFACE((m_arRenderers[nIndex]));
		}

		m_arRenderers.RemoveAll();
	}

public:
	STDMETHOD(SetXml)(BSTR bstrXml)
	{
		m_arPages.RemoveAll();
		
		//CString strXml = CString(bstrXml);
		//strXml = m_oDecoder.DecryptXML(strXml);

		CString strXml = m_oDecoder.DecryptXML(bstrXml);

		m_strFoXml = _T("");

		// теперь нужно расфасовать все по страницам с эффетами
		XmlUtils::CXmlNode oRootNode;
		oRootNode.FromXmlString(strXml);

		XmlUtils::CXmlNode XmlSingleSourceNode;
		
		BOOL bIsVideoSource = FALSE;
		if (_T("Tracks") == oRootNode.GetName())
		{
			XmlUtils::CXmlNodes oUsers;
			if (oRootNode.GetNodes(g_cpszXMLMultiSource, oUsers))
			{
				if (0 < oUsers.GetCount())
				{
					XmlUtils::CXmlNode oMulti;
					if (oUsers.GetAt(0, oMulti))
					{
						if (oMulti.GetNode(g_cpszXMLSingleSource, XmlSingleSourceNode))
						{
							bIsVideoSource = TRUE;
						}
					}
				}
			}
		}
		else if (oRootNode.GetNode(g_cpszXMLSingleSource, XmlSingleSourceNode))
		{
			bIsVideoSource = TRUE;
		}
		
		if ( bIsVideoSource )
		{
			CString str = XmlSingleSourceNode.GetXml();
			
			XmlUtils::CXmlNode XmlNodeVideo;
			if ( XmlSingleSourceNode.GetNode ( g_cpszXMLVideoSources, XmlNodeVideo ) )
			{
				XmlUtils::CXmlReader oReader;
				if (oReader.SetXmlString(XmlNodeVideo.GetXml()) && oReader.ReadNodeList(_T("*")))
				{
					int nSourceCount = oReader.GetLengthList();
					if (0 < nSourceCount)
					{
						BOOL bIsRemoveFirst = FALSE;
						
						for (int i = 0; i < nSourceCount; ++i)
						{
							CString sNodeName = oReader.ReadNodeName(i);
							if (g_cpszXMLColorSource == sNodeName)
							{
								XmlUtils::CXmlNode oPageNode;
								if ( oPageNode.FromXmlString(oReader.ReadNodeXml(i)) )
								{
									m_arPages.Add();
									m_arPages[m_arPages.GetCount() - 1].m_strPageInfo = oPageNode.GetXml();
									m_arPages[m_arPages.GetCount() - 1].m_pThread = this;
									m_arPages[m_arPages.GetCount() - 1].m_pReceiver = this;

									m_arPages[m_arPages.GetCount() - 1].m_dWidthMetric  = Strings::ToDouble(oPageNode.GetAttributeOrValue(_T("widthmetric"), _T("210")));
									m_arPages[m_arPages.GetCount() - 1].m_dHeightMetric = Strings::ToDouble(oPageNode.GetAttributeOrValue(_T("heightmetric"), _T("190")));
								}
							}
							else if (g_cpszXMLImageSource == sNodeName)
							{
								XmlUtils::CXmlNode oPageNode;
								if ( oPageNode.FromXmlString(oReader.ReadNodeXml(i)) )
								{
									m_arPages.Add();
									m_arPages[m_arPages.GetCount() - 1].m_strPageInfo = oPageNode.GetXml();
									m_arPages[m_arPages.GetCount() - 1].m_pThread = this;
									m_arPages[m_arPages.GetCount() - 1].m_pReceiver = this;

									m_arPages[m_arPages.GetCount() - 1].m_dWidthMetric  = Strings::ToDouble(oPageNode.GetAttributeOrValue(_T("widthmetric"), _T("210")));
									m_arPages[m_arPages.GetCount() - 1].m_dHeightMetric = Strings::ToDouble(oPageNode.GetAttributeOrValue(_T("heightmetric"), _T("190")));
								}
							}
							else if (g_cpszXMLVideoCompose == sNodeName && !bIsRemoveFirst)
							{
								if (1 == m_arPages.GetCount())
								{
									m_arPages.RemoveAll();
									bIsRemoveFirst = TRUE;
								}
							}
						}
					}
				}
			}
		}
		else
		{
			// проверить на fo - формат.
			// и тогда нужно использовать XmlRenderer.
			// он тоже в этой длл должен быть
			m_strFoXml = strXml;

			m_strFoXml = GetCStringFromUTF8((CStringA)m_strFoXml);
		}		
		
		return S_OK;
	}
	STDMETHOD(AddRenderer)(IUnknown* punkRenderer)
	{
		// ищем такой же...
		size_t nCount = m_arRenderers.GetCount();
		for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
		{
			if (punkRenderer == m_arRenderers[nIndex])
			{
				// на одном и том же два раза не рисуем
				return S_OK;
			}
		}
		
		IAVSOfficeRendererTemplate* pRenderer = NULL;
		if (NULL != punkRenderer)
			punkRenderer->QueryInterface(__uuidof(IAVSOfficeRendererTemplate), (void**)&pRenderer);

		if (NULL != pRenderer)
		{
			m_arRenderers.Add(pRenderer);
		}

		return S_OK;
	}

	STDMETHOD(RemoveRenderer)(IUnknown* punkRenderer)
	{
		// ищем такой же...
		size_t nCount = m_arRenderers.GetCount();
		for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
		{
			if (punkRenderer == m_arRenderers[nIndex])
			{
				RELEASEINTERFACE((m_arRenderers[nIndex]));
				m_arRenderers.RemoveAt(nIndex);
				break;
			}
		}
		
		return S_OK;
	}

	STDMETHOD(Start)()
	{
		if (IsRunned())
			return S_FALSE;
		
		StartWork(m_lThreadPriority);
		return S_OK;
	}
	STDMETHOD(Stop)()
	{
		StopWork();
		return S_OK;
	}
	STDMETHOD(Suspend)()
	{
		SuspendWork();
		return S_OK;
	}
	STDMETHOD(Resume)()
	{
		ResumeWork();
		return S_OK;
	}

	STDMETHOD(get_Status)(LONG* pStatus)
	{
		*pStatus = AVS_OFFICEEDITOR_STATUS_NONE;
		if (m_bRunThread)
			*pStatus = AVS_OFFICEEDITOR_STATUS_STARTED;
		return S_OK;
	}
	STDMETHOD(get_Priority)(LONG* pPriority)
	{
		*pPriority = GetPriority();
		return S_OK;
	}
	STDMETHOD(put_Priority)(LONG Priority)
	{
		m_lThreadPriority = Priority;
		if (IsRunned())
		{
			::SetThreadPriority(m_hThread, m_lThreadPriority);
		}
		return S_OK;
	}

	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		CString sParamName; sParamName = ParamName;
		if (g_csBlowfishKeyParamName == sParamName)
		{		
               			if (!m_oDecoder.SetBlowfishKey(ParamValue.punkVal))
				return S_FALSE;
		}
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}

protected:

	virtual DWORD ThreadProc()
	{
		if (!CRegistratorClient::IsRegistered())
			return S_OK;
		
		CoInitialize(NULL);	

		if (_T("") == m_strFoXml)
		{
			// просто отрендерим все страницы. они уже сформированы
			size_t nCountPages = m_arPages.GetCount();
			
			for (size_t nIndex = 0; nIndex < nCountPages; ++nIndex)
			{
				size_t nRendCount = m_arRenderers.GetCount();
				
				//
				//Suspend();

				OnNewPage(m_arPages[nIndex].m_dWidthMetric, m_arPages[nIndex].m_dHeightMetric);
				
				//
				CheckSuspend();
				
				for (size_t nRendIndex = 0; nRendIndex < nRendCount; ++nRendIndex)
				{
					if (NULL == m_arRenderers[nRendIndex])
						continue;
					
					m_arRenderers[nRendIndex]->NewPage();
					
					m_arRenderers[nRendIndex]->SetWidth((float)m_arPages[nIndex].m_dWidthMetric);
					m_arRenderers[nRendIndex]->SetHeight((float)m_arPages[nIndex].m_dHeightMetric);
				}
				
				for (size_t nRendIndex = 0; nRendIndex < nRendCount; ++nRendIndex)
				{
					if (NULL == m_arRenderers[nRendIndex])
						continue;
					
					double dDPIX = -1;
					double dDPIY = -1;

					if (NULL != m_arRenderers[nRendIndex])
					{
						VARIANT var;
						var.vt = VT_R8;
						var.dblVal = -1;

						m_arRenderers[nRendIndex]->GetAdditionalParam(L"DPIX", &var);
						dDPIX = var.dblVal;

						var.dblVal = -1;
						m_arRenderers[nRendIndex]->GetAdditionalParam(L"DPIY", &var);
						dDPIY = var.dblVal;
					}

					m_arPages[nIndex].Render(m_arRenderers[nRendIndex], this, dDPIX, dDPIY);
				}
				
				//
				//Suspend();

				IWorker_OnCompletePage();

				OnProgress(100 * (nIndex + 1) / nCountPages);
				CheckSuspend();
			}
		}
		else
		{
			XmlUtils::CXmlNode oNodeFo;
			if (oNodeFo.FromXmlString(m_strFoXml))
			{
				InternalFromXmlNode(oNodeFo);

				for (size_t nIndex = 0; nIndex < m_arRenderers.GetCount(); ++nIndex)
				{
					BOOL bIsLastPage	= FALSE;

					do
					{
						if (!m_bRunThread)
							break;

						CheckSuspend();
						bIsLastPage = DrawText(m_arRenderers[nIndex], 1.0);		
					} 
					while (!bIsLastPage);
				}
			}
		}
		
		OnStop();
		CoUninitialize();
		m_bRunThread = FALSE;
		
		return 0;
	}

	virtual void OnSendEvent(LONG lType, LONG lParam1, LONG lParam2)
	{
		switch (lType)
		{
		case OFFICEEDITOR_PROGRESS_TYPE_ERROR:
			{
				OnError(lParam1);
				break;
			}
		case OFFICEEDITOR_PROGRESS_TYPE_STOP:
			{
				OnStop();
				break;
			}
		case OFFICEEDITOR_PROGRESS_TYPE_PAGE:
			{
				OnProgress(lParam1);
				break;
			}
		case OFFICEEDITOR_PROGRESS_TYPE_ELEMENT:
			{
				OnProgressParce(OFFICEEDITOR_PROGRESS_TYPE_ELEMENT, lParam1);
				break;
			}
		default:
			break;
		};
	}

	// IWorker events
	virtual void IWorker_OnNewPage(double dWidthMm, double dHeightMm)
	{
		//
		//Suspend();
		
		OnNewPage(dWidthMm, dHeightMm);

		//
		CheckSuspend();
		
		if (0 < m_arRenderers[0])
		{
			SetRenderer(m_arRenderers[0]);
			m_arRenderers[0]->NewPage();

			m_arRenderers[0]->SetWidth((float)dWidthMm);
			m_arRenderers[0]->SetHeight((float)dHeightMm);
		}
		else
		{
			SetRenderer(NULL);
		}
	}

	virtual void IWorker_OnCompletePage()
	{
		//
		//Suspend();

		// счас поставим что-то вроде заглушки
		if (0 < m_arRenderers.GetCount())
		{
			if (NULL != m_arRenderers[0])
			{
				m_arRenderers[0]->EndCommand(c_nPageType);
			}
		}
		
		OnCompletePage();

		//
		CheckSuspend();
	}

	virtual void IWorker_OnProgress(double dCompleteness)
	{
		LONG lPercents = (LONG)(100 * dCompleteness);
		OnProgress(lPercents);
	}

private:
	
	CString GetCStringFromUTF8( CStringA str )
	{
		BYTE* pBuffer = (BYTE*)str.GetBuffer();
		LONG lLen = str.GetLength();

		return GetCStringFromUTF8(pBuffer, lLen);
	}

	CString GetCStringFromUTF8( BYTE* pBuffer, LONG lCount )
	{
		LONG lLenght = 0;
		CStringW strRes = L"";
		for (LONG lIndex = 0; lIndex < lCount; ++lIndex)
		{
			if (0x00 == (0x80 & pBuffer[lIndex]))
			{
				strRes += (WCHAR)pBuffer[lIndex];
				continue;
			}
			else if (0x00 == (0x20 & pBuffer[lIndex]))
			{
				WCHAR mem = (WCHAR)(((pBuffer[lIndex] & 0x1F) << 6) + (pBuffer[lIndex + 1] & 0x3F));
				strRes += mem;

				lIndex += 1;
			}
			else if (0x00 == (0x10 & pBuffer[lIndex]))
			{
				WCHAR mem = (WCHAR)(((pBuffer[lIndex] & 0x0F) << 12) + ((pBuffer[lIndex + 1] & 0x3F) << 6) + (pBuffer[lIndex + 2] & 0x3F));
				strRes += mem;

				lIndex += 2;
			}
			else
			{
				BYTE mem = pBuffer[lIndex];

				int n = 0;
			}
		}
		
		return (CString)strRes;
	}
};
