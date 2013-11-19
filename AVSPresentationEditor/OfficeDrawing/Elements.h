#pragma once
#include "Theme.h"
#include "Shapes\Shape.h"

#ifdef _PRESENTATION_WRITER_
#include "SVGConverter.h"
#endif

#include "../PPTXWriter/FileDownloader.h"

#ifdef ENABLE_ODP_TO_PPTX_CONVERT

// enable ppt to pptx shape convertation
#ifndef ENABLE_PPT_TO_PPTX_CONVERT
#define ENABLE_PPT_TO_PPTX_CONVERT
#endif

#include "Shapes/BaseShape/PPTShape/PPT2PPTXShapeConverter.h"
#include "Shapes/BaseShape/ODPShape/ODP2PPTXShapeConverter.h"
#endif

#ifdef ENABLE_PPT_TO_PPTX_CONVERT
#include "Shapes/BaseShape/PPTShape/PPT2PPTXShapeConverter.h"
#endif

namespace NSPresentationEditor
{
	/**************************************************************
				  здесь все типы элементов
	**************************************************************/

	class CVideoElement : public IElement
	{
	public:
		CString		m_strFileName;
		double		m_dVideoDuration;
		BYTE		m_nAlpha;
		LONG		m_lAngle;

		double		m_dClipStartTime;
		double		m_dClipEndTime;

		BOOL		m_bLoop;

	public:
		CVideoElement() : IElement()
		{
			m_etType			=	etVideo;
			
			m_strFileName		=	_T("");
			m_dVideoDuration	=	0.0;
			m_nAlpha			=	0xFF;
					
			m_dClipStartTime	=	-1.0;
			m_dClipEndTime		=	-1.0;
			m_bLoop				=	FALSE;
		}

		virtual ~CVideoElement()
		{
		}

		virtual void WriteToXml(XmlUtils::CXmlWriter& oWriter)
		{
			CString strXml = SaveToXML();
			oWriter.WriteString(strXml);
		}
		virtual void ReadFromXml(XmlUtils::CXmlNode& oNode)
		{
		}
		
		virtual CString SaveToXML()
		{
			return GetVideoStream () + GetAudioStream ();
		}
		
		CString ToAnimationXml()
		{
			return _T("");
		}

		virtual IElement* CreateDublicate()
		{
			CVideoElement* pVideoElement = new CVideoElement();
			
			SetProperiesToDublicate((IElement*)pVideoElement);

			pVideoElement->m_strFileName		=	m_strFileName;
			pVideoElement->m_nAlpha				=	m_nAlpha;

			pVideoElement->m_dVideoDuration		=	m_dVideoDuration;

			pVideoElement->m_dClipStartTime		=	m_dClipStartTime;
			pVideoElement->m_dClipEndTime		=	m_dClipEndTime;
			pVideoElement->m_bLoop				=	m_bLoop;

			return (IElement*)pVideoElement;
		}

		virtual void SetupProperty(CSlide* pSlide, CTheme* pTheme, CLayout* pLayout, CElementProperty* pProperty)
		{
		}

		virtual CString ToXmlVideoSource()
		{
			return SaveToXML();
		}
		virtual CString ToXmlEditor()
		{
			return SaveToXML();
		}

		inline CString GetVideoStream ()
		{
			int lIndex = m_strFileName.Find(L"file:///");
			if (0 == lIndex)
			{
				m_strFileName = m_strFileName.Mid(8);
				m_strFileName.Replace('/', '\\');
			}
			
			CString strFileName = m_strFileName;
			CorrectXmlString(strFileName);

			CString element;
			element.Format ( _T("<VideoStream left='%d' top='%d' right='%d' bottom='%d' angle='%f' loop='%d' ")
				_T(" widthmetric='%d' heightmetric='%d' ")
				_T(" file='%s' begin='%f' end='%f' >"),
				(LONG)m_rcBounds.left, (LONG)m_rcBounds.top, (LONG)m_rcBounds.right, (LONG)m_rcBounds.bottom, m_dRotate, m_bLoop,
				m_oMetric.m_lMillimetresHor, m_oMetric.m_lMillimetresVer,
				strFileName, m_dClipStartTime, m_dClipEndTime );

			CString animations;
			if((long)m_oAnimations.m_arAnimations.GetCount())
			{
				m_oAnimations.m_dSlideWidth		=	m_oMetric.m_lMillimetresHor;
				m_oAnimations.m_dSlideHeight	=	m_oMetric.m_lMillimetresVer;
				animations						=	m_oAnimations.ToXml(m_dStartTime, m_dEndTime);

				element		+=	animations;
			}

			CString timeLine;
			timeLine.Format ( _T("<timeline type = \"1\"  begin=\"%f\" end=\"%f\" fadein=\"0\" fadeout=\"0\" completeness=\"1.0\"/> "),	m_dStartTime, m_dEndTime );

			element			+=	timeLine;		
			element			+=	_T("</VideoStream>");

			return element;
		}

		inline CString GetAudioStream ()
		{
			CString element = _T("");
			element.Format(_T("<AudioSource StartTime='%lf' Duration='%lf' Amplify='%lf' loop='%d' >"), m_dStartTime, m_dEndTime - m_dStartTime, 100.0, m_bLoop);

			int lIndex = m_strFileName.Find(L"file:///");
			if (0 == lIndex)
			{
				m_strFileName = m_strFileName.Mid(8);
				m_strFileName.Replace('/', '\\');
				m_strFileName.Replace(L"%20", L" ");
			}

			CString strFileName = m_strFileName;
			CorrectXmlString(strFileName);

			CString source;
			source.Format(_T("<Source StartTime='%lf' EndTime='%lf' FilePath='%s'/>"), m_dClipStartTime, m_dClipEndTime, strFileName);
			element	+=	source;		

			CString animations;
			if((long)m_oAnimations.m_arAnimations.GetCount())								//	для audio только "media call's" - play - pause - stop
			{
				m_oAnimations.m_dSlideWidth		=	m_oMetric.m_lMillimetresHor;
				m_oAnimations.m_dSlideHeight	=	m_oMetric.m_lMillimetresVer;
				animations						=	m_oAnimations.ToXml(m_dStartTime, m_dEndTime);

				element		+=	animations;
			}
			
			CString timeLine;
			timeLine.Format ( _T("<timeline type = \"1\"  begin=\"%f\" end=\"%f\" fadein=\"0\" fadeout=\"0\" completeness=\"1.0\"/> "),	m_dStartTime, m_dEndTime );
			
			element			+=	timeLine;		
			element			+=	_T("</AudioSource>");

			return element;
		}
	};

	class CImageElement : public IElement
	{
	public:
		CString		m_strFileName;

		BYTE		m_nAlpha;
		bool		m_bApplyBounds;
		CDoubleRect m_rcImageBounds;

	public:
		CImageElement() : IElement()
		{
			m_etType = etPicture;
			
			m_strFileName = _T("");

			m_nAlpha = 0xFF;

			m_bApplyBounds = false;
			m_rcImageBounds.top = 0.0;
			m_rcImageBounds.left = 0.0;
			m_rcImageBounds.right = 0.0;
			m_rcImageBounds.bottom = 0.0;
		}

		virtual ~CImageElement()
		{
		}

		virtual void WriteToXml(XmlUtils::CXmlWriter& oWriter)
		{
			CString strXml = SaveToXML();
			oWriter.WriteString(strXml);
		}
		virtual void ReadFromXml(XmlUtils::CXmlNode& oNode)
		{
		}

		virtual CString SaveToXML()
		{
			if (0 != m_oAnimations.m_arAnimations.GetCount())
				return ToAnimationXml();
			
			LONG lFlags = 0;
			if (m_bFlipH)
				lFlags |= 0x0001;
			if (m_bFlipV)
				lFlags |= 0x0002;

			CString strEffect = _T("");
			strEffect.Format(_T("<ImagePaint-DrawImageFromFile left='%d' top='%d' right='%d' bottom='%d' angle='%lf' flags='%d' filepath='%s' metric='0' backcolor='-1' scaletype='-1' scalecolor='255' widthmetric='%d' heightmetric='%d'>\
					<timeline type='1' begin='%lf' end='%lf' fadein='0' fadeout='0' completeness='1.0' /></ImagePaint-DrawImageFromFile>"), 
					(LONG)m_rcBounds.left, (LONG)m_rcBounds.top, (LONG)m_rcBounds.right, (LONG)m_rcBounds.bottom,
					m_dRotate, lFlags, m_strFileName, 
					m_oMetric.m_lMillimetresHor, m_oMetric.m_lMillimetresVer, 
					m_dStartTime, m_dEndTime);

			return strEffect;
		}

		CString ToAnimationXml()
		{
			LONG lFlags = 0;
			if (m_bFlipH)
				lFlags |= 0x0001;
			if (m_bFlipV)
				lFlags |= 0x0002;

			CString strEffect = _T("");
			strEffect.Format(_T("<ImagePaint-DrawImageFromFile left='%d' top='%d' right='%d' bottom='%d' angle='%lf' flags='%d' filepath='%s' metric='0' backcolor='-1' scaletype='-1' scalecolor='255' widthmetric='%d' heightmetric='%d'>\
					<timeline type='1' begin='%lf' end='%lf' fadein='0' fadeout='0' completeness='1.0' /></ImagePaint-DrawImageFromFile>"), 
					(LONG)m_rcBounds.left, (LONG)m_rcBounds.top, (LONG)m_rcBounds.right, (LONG)m_rcBounds.bottom,
					m_dRotate, lFlags, m_strFileName, 
					m_oMetric.m_lMillimetresHor, m_oMetric.m_lMillimetresVer, 
					m_dStartTime, m_dEndTime);

			CString TimeLine;
			TimeLine.Format ( _T("<timeline type = \"1\"  begin=\"%f\" end=\"%f\" fadein=\"0\" fadeout=\"0\" completeness=\"1.0\"/> "),
				m_dStartTime, m_dEndTime );

			CString Source	= m_oAnimations.ToXml(m_dStartTime, m_dEndTime);
			Source	+=	TimeLine;
			Source	+=	_T("</ImagePaint-DrawImageFromFileAnimate>");

			strEffect.Replace ( _T("</ImagePaint-DrawImageFromFile>"), Source );
			strEffect.Replace ( _T("<ImagePaint-DrawImageFromFile"), _T("<ImagePaint-DrawImageFromFileAnimate") );

			return strEffect;
		}

		virtual IElement* CreateDublicate()
		{
			CImageElement* pImageElement = new CImageElement();
			
			SetProperiesToDublicate((IElement*)pImageElement);

			pImageElement->m_strFileName = m_strFileName;
			pImageElement->m_nAlpha = m_nAlpha;

			pImageElement->m_bApplyBounds = m_bApplyBounds;
			pImageElement->m_rcImageBounds.top = m_rcImageBounds.top;
			pImageElement->m_rcImageBounds.left = m_rcImageBounds.left;
			pImageElement->m_rcImageBounds.right = m_rcImageBounds.right;
			pImageElement->m_rcImageBounds.bottom = m_rcImageBounds.bottom;

			return (IElement*)pImageElement;
		}

		virtual void SetupProperty(CSlide* pSlide, CTheme* pTheme, CLayout* pLayout, CElementProperty* pProperty)
		{
		}

		virtual CString ToXmlVideoSource()
		{
			return SaveToXML();
		}
		virtual CString ToXmlEditor()
		{
			AVSGraphics::IAVSSVGRenderer* pSVG = NULL;
			CoCreateInstance(AVSGraphics::CLSID_CAVSSVGRenderer, NULL, CLSCTX_ALL, AVSGraphics::IID_IAVSSVGRenderer, (void**)&pSVG);

			pSVG->put_Width(m_oMetric.m_lMillimetresHor);
			pSVG->put_Height(m_oMetric.m_lMillimetresVer);
			
			pSVG->CreateOfficeFile(L"", 0);

			BSTR bsFileName = m_strFileName.AllocSysString();
			pSVG->DrawImageFromFile(bsFileName, m_rcBounds.left, m_rcBounds.top, m_rcBounds.right - m_rcBounds.left, m_rcBounds.bottom - m_rcBounds.top);
			SysFreeString(bsFileName);

			pSVG->CloseFile(0);

			BSTR bsResult = NULL;
			pSVG->get_Data(&bsResult);

			CString strXml = (CString)bsResult;
			SysFreeString(bsResult);
			RELEASEINTERFACE(pSVG);

			int nIndexStart = strXml.Find((TCHAR)'>');
			int nIndexEnd	= strXml.ReverseFind((TCHAR)'<');

			if ((-1 != nIndexStart) && (nIndexEnd > nIndexStart))
			{
				strXml = strXml.Mid(nIndexStart + 1, nIndexEnd - nIndexStart - 1);
			}

			double _dLeft	= m_rcBounds.left * 96 / 25.4;
			double _dTop	= m_rcBounds.top  * 96 / 25.4;
			double _dWidth	= (m_rcBounds.right - m_rcBounds.left) * 96 / 25.4;
			double _dHeight	= (m_rcBounds.bottom - m_rcBounds.top) * 96 / 25.4;

			CString strTxPath = m_strFileName;
			CorrectXmlString(strTxPath);

			CString strElement = _T("");
			strElement.Format(_T("<shape background='%d' changeable='%d' left='%d' top='%d' width='%d' height='%d' txpath='%s'>"), 
				(int)(m_bIsBackground ? 1 : 0), (int)(m_bIsChangeable ? 1 : 0), (LONG)_dLeft, (LONG)_dTop, (LONG)_dWidth, (LONG)_dHeight, strTxPath);
			strXml = strElement + strXml + _T("</shape>");

			return strXml;
		}

		AVSINLINE CString DownloadImage(const CString& strFile)
		{
			CFileDownloader oDownloader(strFile, TRUE);
			oDownloader.Start( 1 );
			while ( oDownloader.IsRunned() )
			{
				::Sleep( 10 );
			}

			if ( oDownloader.IsFileDownloaded() )
			{
				m_strFileName = oDownloader.GetFilePath();
			}
		}
	};

	class CShapeElement : public IElement
	{
	private:
		NSBaseShape::ClassType m_ClassType;

	public:
		int m_lShapeType;
		CShape m_oShape;
		
	public:
		CShapeElement(NSBaseShape::ClassType ClassType, int eType) : IElement(), m_lShapeType(eType), m_oShape(ClassType, eType)
		{
			m_ClassType = ClassType;
			
			m_etType = etShape;

			m_oShape.m_rcBounds = m_rcBounds;

			m_oShape.m_dStartTime = m_dStartTime;
			m_oShape.m_dStartTime = m_dEndTime;
		}

		CShapeElement() : m_oShape(NSBaseShape::unknown, 0x1000)
		{
			m_lShapeType = 0x1000;
			m_etType = etShape;
		}

		CShapeElement(const CString& str) : IElement(), m_oShape(NSBaseShape::unknown, 0x1000)
		{
			m_lShapeType = 0x1000;
			m_oShape.LoadFromXML(str);
			m_ClassType = m_oShape.m_pShape->GetClassType();
		}

		virtual ~CShapeElement()
		{
		}

		virtual void WriteToXml(XmlUtils::CXmlWriter& oWriter)
		{
			CString strXml = SaveToXML();
			oWriter.WriteString(strXml);
		}
		virtual void ReadFromXml(XmlUtils::CXmlNode& oNode)
		{
		}

		virtual CString SaveToXML()
		{
			if (0 != m_oAnimations.m_arAnimations.GetCount())
				return ToAnimationXml();
			
			CGeomShapeInfo oInfo;
			oInfo.SetBounds(m_rcBounds);

			oInfo.m_dRotate = m_dRotate;
			oInfo.m_bFlipH	= m_bFlipH;
			oInfo.m_bFlipV	= m_bFlipV;

			return m_oShape.ToXml(oInfo, m_oMetric, m_dStartTime, m_dEndTime, m_pTheme, m_pLayout);
		}

		CString ToAnimationXml()
		{
			CGeomShapeInfo oInfo;
			oInfo.SetBounds(m_rcBounds);

			oInfo.m_dRotate = m_dRotate;
			oInfo.m_bFlipH	= m_bFlipH;
			oInfo.m_bFlipV	= m_bFlipV;

			m_oAnimations.m_dSlideWidth		=	m_oMetric.m_lMillimetresHor;
			m_oAnimations.m_dSlideHeight	=	m_oMetric.m_lMillimetresVer;

			CString timeXML;
			timeXML.Format(_T("<timeline type = \"1\"  begin=\"%f\" end=\"%f\" fadein=\"0\" fadeout=\"0\" completeness=\"1.0\"/> "), m_dStartTime, m_dEndTime );
			
			CString baseXML = m_oShape.ToXml(oInfo, m_oMetric, m_dStartTime, m_dEndTime, m_pTheme, m_pLayout);

			if ( -1 != baseXML.Find ( _T("<ImagePaint-DrawTextEx") ) )
			{
				CString sBounds;	// FIX
				sBounds.Format(_T("<bounds left='%d' top='%d' right='%d' bottom='%d'/>"), 
					(LONG)m_rcBounds.left, (LONG)m_rcBounds.top, (LONG)m_rcBounds.right, (LONG)m_rcBounds.bottom);

				CString sReplace = m_oAnimations.ToXml(m_dStartTime, m_dEndTime) + timeXML + sBounds + _T("</ImagePaint-DrawTextAnimateEx>");
				
				baseXML.Replace ( _T("</ImagePaint-DrawTextEx>"),	sReplace );
				baseXML.Replace ( _T("<ImagePaint-DrawTextEx"),		_T("<ImagePaint-DrawTextAnimateEx") );
			}

			if ( -1 != baseXML.Find ( _T("<ImagePaint-DrawGraphicPath") ) )
			{
				CString sAnim = m_oAnimations.ToXml(m_dStartTime, m_dEndTime, TRUE);
				if (sAnim.GetLength())
				{
					CString sReplace = sAnim + timeXML + _T("</ImagePaint-DrawGraphicPathAnimate>");
					
					baseXML.Replace ( _T("</ImagePaint-DrawGraphicPath>"),	sReplace );
					baseXML.Replace ( _T("<ImagePaint-DrawGraphicPath"),	_T("<ImagePaint-DrawGraphicPathAnimate") );
				}
			}
			
			return baseXML;
		}

		virtual IElement* CreateDublicate()
		{
			CShapeElement* pShapeElement = new CShapeElement(m_ClassType, m_lShapeType);
			
			SetProperiesToDublicate((IElement*)pShapeElement);

			m_oShape.SetToDublicate(&pShapeElement->m_oShape);
			return (IElement*)pShapeElement;
		}

		
		virtual void SetupProperties(CSlide* pSlide, CTheme* pTheme, CLayout* pLayout)
		{
			CAtlMap<CElementProperty::Type, CElementProperty>* pMap = &m_oProperties.m_arProperties;
			
			POSITION pos = pMap->GetStartPosition();
			while (NULL != pos)
			{
				CElementProperty oProperty = pMap->GetNextValue(pos);
				SetupProperty(pSlide, pTheme, pLayout, &oProperty);
			}

			m_oShape.m_oText.m_lPlaceholderType = m_lPlaceholderType;
			m_oShape.m_oText.m_lPlaceholderID	= m_lPlaceholderID;

			m_oShape.m_pShape->ReCalculate();

			SetupTextProperties(pSlide, pTheme, pLayout);

			CalculateColor(m_oShape.m_oPen.Color, pSlide, pTheme, pLayout);
			CalculateColor(m_oShape.m_oBrush.Color1, pSlide, pTheme, pLayout);
			CalculateColor(m_oShape.m_oBrush.Color2, pSlide, pTheme, pLayout);
		}

		virtual void SetupTextProperties(CSlide* pSlide, CTheme* pTheme, CLayout* pLayout);

		void CalculateColor(CColor& oColor, CSlide* pSlide, CTheme* pTheme, CLayout* pLayout);

		virtual void SetupProperty(CSlide* pSlide, CTheme* pTheme, CLayout* pLayout, CElementProperty* pProperty)
		{
			const LONG EMU_MM = 36000;

			switch (pProperty->m_ID)
			{
			case CElementProperty::epTextMarginLeft:
				{
					m_oShape.m_dTextMarginX		= (double)pProperty->m_dwValue / EMU_MM;
					break;
				}
			case CElementProperty::epTextMarginTop:
				{
					m_oShape.m_dTextMarginY			= (double)pProperty->m_dwValue / EMU_MM;
					break;
				}
			case CElementProperty::epTextMarginRight:
				{
					m_oShape.m_dTextMarginRight		= (double)pProperty->m_dwValue / EMU_MM;
					break;
				}
			case CElementProperty::epTextMarginBottom:
				{
					m_oShape.m_dTextMarginBottom	= (double)pProperty->m_dwValue / EMU_MM;
					break;
				}
			case CElementProperty::epText:
				{
					//m_oShape.m_oText.m_sText		= pProperty->m_strAdvanced;
				}
			case CElementProperty::epTextWrap:
				{
					m_oShape.m_oText.m_lWrapMode	= (LONG)pProperty->m_dwValue;
					break;
				}
			case CElementProperty::epBrushType:
				{
					m_oShape.m_oBrush.Type			= (LONG)pProperty->m_dwValue;
					break;
				}
			case CElementProperty::epBrushColor1:
				{
					m_oShape.m_oBrush.Color1.SetSBGR(pProperty->m_dwValue);
					CalculateColor(m_oShape.m_oBrush.Color1, pSlide, pTheme, pLayout);
					break;
				}
			case CElementProperty::epBrushColor2:
				{
					m_oShape.m_oBrush.Color2.SetSBGR(pProperty->m_dwValue);
					CalculateColor(m_oShape.m_oBrush.Color2, pSlide, pTheme, pLayout);
					break;
				}
			case CElementProperty::epBrushAlpha1:
				{
					m_oShape.m_oBrush.Alpha1 = (BYTE)pProperty->m_dwValue;
					break;
				}
			case CElementProperty::epBrushAlpha2:
				{
					m_oShape.m_oBrush.Alpha2 = (BYTE)pProperty->m_dwValue;
					break;
				}
			case CElementProperty::epBrushTxPath:
				{
					m_oShape.m_oBrush.TexturePath = pProperty->m_strAdvanced;
					break;
				}
			case CElementProperty::epBrushTxMode:
				{
					m_oShape.m_oBrush.TextureMode	= (LONG)pProperty->m_dwValue;
					break;
				}
			case CElementProperty::epFilled:
				{
					if (0 == pProperty->m_dwValue)
					{
						m_oShape.m_oBrush.Alpha1		= 0;
					}
					break;
				}
			case CElementProperty::epPenColor:
				{
					m_oShape.m_oPen.Color.SetSBGR(pProperty->m_dwValue);
					CalculateColor(m_oShape.m_oPen.Color, pSlide, pTheme, pLayout);
					break;
				}
			case CElementProperty::epPenAlpha:
				{
					m_oShape.m_oPen.Alpha		= (BYTE)pProperty->m_dwValue;
					break;
				}
			case CElementProperty::epPenWidth:
				{
					m_oShape.m_oPen.Size		= (double)pProperty->m_dwValue / 0xFFFF;
					break;
				}
			case CElementProperty::epPenJoin:
				{
					m_oShape.m_oPen.LineJoin	= (BYTE)pProperty->m_dwValue;
					break;
				}
			case CElementProperty::epLineDash:
				{
					m_oShape.m_oPen.DashStyle	= (BYTE)pProperty->m_dwValue;
					break;
				}
			case CElementProperty::epLineStartCap:
				{
					m_oShape.m_oPen.LineStartCap	= (BYTE)pProperty->m_dwValue;
					break;
				}
			case CElementProperty::epLineEndCap:
				{
					m_oShape.m_oPen.LineEndCap		= (BYTE)pProperty->m_dwValue;
					break;
				}
			case CElementProperty::epStroked:
				{
					if (0 == pProperty->m_dwValue)
						m_oShape.m_oPen.Alpha = 0;

					break;
				}
			case CElementProperty::epFontName:
				{
					m_oShape.m_oText.m_oAttributes.m_oFont.Name = pProperty->m_strAdvanced;
					break;
				}
			case CElementProperty::epFontHorAlign:
				{
					m_oShape.m_oText.m_oAttributes.m_nTextAlignHorizontal = (int)pProperty->m_dwValue;
					break;
				}
			case CElementProperty::epFontVertAlign:
				{
					m_oShape.m_oText.m_oAttributes.m_nTextAlignVertical	  = (int)pProperty->m_dwValue;	
					break;
				}
			case CElementProperty::epFontSize:
				{
					m_oShape.m_oText.m_oAttributes.m_oFont.Size			= (double)pProperty->m_dwValue / 0xFFFF;
					break;
				}
			case CElementProperty::epFontBold:
				{
					m_oShape.m_oText.m_oAttributes.m_oFont.Bold			= (BOOL)pProperty->m_dwValue;
					break;
				}
			case CElementProperty::epFontItalic:
				{
					m_oShape.m_oText.m_oAttributes.m_oFont.Italic		= (BOOL)pProperty->m_dwValue;
					break;
				}
			case CElementProperty::epFontStrikeout:
				{
					m_oShape.m_oText.m_oAttributes.m_oFont.Strikeout	= (BYTE)pProperty->m_dwValue;
					break;
				}
			default:
				break;
			}
		}

		virtual CString ToXmlVideoSource()
		{
			return SaveToXML();
		}

		virtual CString ToXmlEditor()
		{
			CString strPPTXShape = _T("");
			
			NSBaseShape::ClassType eShapeType = NSBaseShape::unknown;
			if (m_oShape.m_pShape != NULL)
				eShapeType = m_oShape.m_pShape->GetClassType();

#ifdef ENABLE_PPT_TO_PPTX_CONVERT
			if (eShapeType == NSBaseShape::ppt)
				strPPTXShape = ConvertPPTShapeToPPTX();
#endif

#ifdef ENABLE_ODP_TO_PPTX_CONVERT
			if (eShapeType == NSBaseShape::odp)
				strPPTXShape = ConvertODPShapeToPPTX();
#endif
		
			CGeomShapeInfo oInfo;
			oInfo.SetBounds(m_rcBounds);

			oInfo.m_dRotate = m_dRotate;
			oInfo.m_bFlipH	= m_bFlipH;
			oInfo.m_bFlipV	= m_bFlipV;

			oInfo.m_lOriginalWidth	= (LONG)m_rcBoundsOriginal.GetWidth();
			oInfo.m_lOriginalHeight	= (LONG)m_rcBoundsOriginal.GetHeight();

			if ((0 == m_oShape.m_oPen.Alpha) && (0 == m_oShape.m_oBrush.Alpha1) && (1000 == m_oShape.m_oBrush.Type))
				m_oShape.m_lDrawType &= c_ShapeDrawType_Text;

			CString strXml	= _T("");
			if (m_oShape.m_lDrawType & c_ShapeDrawType_Graphic)
			{
				AVSGraphics::IAVSSVGRenderer* pSVG = NULL;
				CoCreateInstance(AVSGraphics::CLSID_CAVSSVGRenderer, NULL, CLSCTX_ALL, AVSGraphics::IID_IAVSSVGRenderer, (void**)&pSVG);

				pSVG->put_Width(m_oMetric.m_lMillimetresHor);
				pSVG->put_Height(m_oMetric.m_lMillimetresVer);

				pSVG->CreateOfficeFile(L"", 0);

				//if (m_oShape.m_oPen.LineEndCap == 0x11 ||
				//	m_oShape.m_oPen.LineEndCap == 0x14 ||
				//	m_oShape.m_oPen.LineEndCap == 0x13 ||
				//	m_oShape.m_oPen.LineEndCap == 0x12)
				//{
				//	AVSGraphics::IAVSMetafile* pMetafile = NULL;
				//	CoCreateInstance(AVSGraphics::CLSID_CAVSMetafile, NULL, CLSCTX_ALL, AVSGraphics::IID_IAVSMetafile, (void**)&pMetafile);
				//	m_oShape.ToRenderer((IAVSRenderer*)pMetafile, oInfo, m_oMetric, m_dStartTime, m_dEndTime);
				//	pMetafile->Draw2((IUnknown*)pSVG, 96.0, 96.0, NULL);
				//	RELEASEINTERFACE(pMetafile);
				//}
				//else
				{
					m_oShape.ToRenderer((IAVSRenderer*)pSVG, oInfo, m_oMetric, m_dStartTime, m_dEndTime);
				}

				pSVG->CloseFile(0);
				
				BSTR bsResult = NULL;
				pSVG->get_Data(&bsResult);

				strXml = (CString)bsResult;
				SysFreeString(bsResult);
				RELEASEINTERFACE(pSVG);

				int nIndexStart = strXml.Find((TCHAR)'>');
				int nIndexEnd	= strXml.ReverseFind((TCHAR)'<');

				if ((-1 != nIndexStart) && (nIndexEnd > nIndexStart))
				{
					strXml = strXml.Mid(nIndexStart + 1, nIndexEnd - nIndexStart - 1);
				}

				double _dLeft	= m_rcBounds.left * 96 / 25.4;
				double _dTop	= m_rcBounds.top  * 96 / 25.4;
				double _dWidth	= (m_rcBounds.right - m_rcBounds.left) * 96 / 25.4;
				double _dHeight	= (m_rcBounds.bottom - m_rcBounds.top) * 96 / 25.4;

				CString strTxPath = _T("");
				if (c_BrushTypeTexture == m_oShape.m_oBrush.Type)
					strTxPath = m_oShape.m_oBrush.TexturePath;

				CorrectXmlString(strTxPath);
				if (_T("") != strPPTXShape)
					CorrectXmlString(strPPTXShape);

				CString strElement = _T("");
				strElement.Format(_T("<shape background='%d' changeable='%d' left='%d' top='%d' width='%d' height='%d' txpath='%s' pptx_shape='%s'>"), 
					(int)(m_bIsBackground ? 1 : 0), (int)(m_bIsChangeable ? 1 : 0), 
					(LONG)_dLeft, (LONG)_dTop, (LONG)_dWidth, (LONG)_dHeight, strTxPath, strPPTXShape);
				strXml = strElement + strXml + _T("</shape>");
			}
			else
			{
				double _dLeft	= m_rcBounds.left * 96 / 25.4;
				double _dTop	= m_rcBounds.top  * 96 / 25.4;
				double _dWidth	= (m_rcBounds.right - m_rcBounds.left) * 96 / 25.4;
				double _dHeight	= (m_rcBounds.bottom - m_rcBounds.top) * 96 / 25.4;

				int nL = (int)_dLeft;
				int nT = (int)_dTop;
				int nR = nL + (int)_dWidth;
				int nB = nT + (int)_dHeight;

				strXml.Format(_T("<g><path style=\"fill:none;stroke:none\" d=\"M %d,%d L %d,%d L %d,%d L %d,%d Z\"/></g>"), 
					nL, nT, nR, nT, nR, nB, nL, nB);

				CString strTxPath = _T("");
				if (c_BrushTypeTexture == m_oShape.m_oBrush.Type)
					strTxPath = m_oShape.m_oBrush.TexturePath;

				CorrectXmlString(strTxPath);

				CString strElement = _T("");
				strElement.Format(_T("<shape background='%d' changeable='%d' left='%d' top='%d' width='%d' height='%d' txpath='%s'>"), 
					(int)(m_bIsBackground ? 1 : 0), (int)(m_bIsChangeable ? 1 : 0), 
					(LONG)_dLeft, (LONG)_dTop, (LONG)_dWidth, (LONG)_dHeight, strTxPath);
				strXml = strElement + strXml + _T("</shape>");
			}

			if ((m_oShape.m_oText.IsEmptyText()) && (m_oShape.m_oBrush.Type == c_BrushTypeTexture))
			{
				return strXml;
			}

			CElemInfo oElemInfo;
			oElemInfo.m_bIsBackground = m_bIsBackground;
			oElemInfo.m_bIsChangeable = m_bIsChangeable;
			oElemInfo.m_lID			  = m_lID;
			return strXml + m_oShape.GetTextXHTML(oInfo, m_oMetric, m_dStartTime, m_dEndTime, oElemInfo, m_pTheme, m_pLayout);
		}

#ifdef _PRESENTATION_WRITER_
		void LoadFromXmlNode2(XmlUtils::CXmlNode& oNode)
		{
			m_rcBounds.left		= oNode.ReadAttributeDouble(_T("left"));
			m_rcBounds.top		= oNode.ReadAttributeDouble(_T("top"));
			m_rcBounds.right	= m_rcBounds.left + max(1.0, oNode.ReadAttributeDouble(_T("width")));
			m_rcBounds.bottom	= m_rcBounds.top + max(1.0, oNode.ReadAttributeDouble(_T("height")));

			m_rcBounds.Scale(25.4 / 96);

			if (_T("shape") == oNode.GetName())
			{
				LoadShapeGeom(oNode);
			}
			else if (_T("text") == oNode.GetName())
			{
				NormalizeCoordsByMetric();

				m_oShape.m_pShape = new CPPTShape();
				m_oShape.m_oText.m_arParagraphs.RemoveAll();

				XmlUtils::CXmlNode oNodeBody;
				if (oNode.GetNode(_T("body"), oNodeBody))
				{
					CTextLoader::ConvertText2(oNodeBody, m_oShape.m_oText, m_oMetric);
				}								
			}

			if (NULL == m_oShape.m_pShape)
				m_oShape.m_pShape = new CPPTShape();
		}

		void LoadFromXmlNode(XmlUtils::CXmlNode& oNode, CDoubleRect* pRectPlaceholder = NULL)
		{
			// здесь имя ноды может быть "placeholder"/"element"
			XmlUtils::CXmlNode oNodeShape;
			XmlUtils::CXmlNode oNodeText;

			double dKoef = 25.4 / 96;

			bool bIsShapePresent = false;
			if (oNode.GetNode(_T("shape"), oNodeShape))
			{
				bIsShapePresent = true;
				
				m_rcBounds.left		= oNodeShape.ReadAttributeDouble(_T("left"));
				m_rcBounds.top		= oNodeShape.ReadAttributeDouble(_T("top"));
				m_rcBounds.right	= m_rcBounds.left + max(1.0, oNodeShape.ReadAttributeDouble(_T("width")));
				m_rcBounds.bottom	= m_rcBounds.top + max(1.0, oNodeShape.ReadAttributeDouble(_T("height")));

				m_rcBounds.Scale(dKoef);

				LoadShapeGeom(oNodeShape);
			}
			if (oNode.GetNode(_T("text"), oNodeText))
			{
				CDoubleRect oTextRect;

				oTextRect.left		= oNodeText.ReadAttributeDouble(_T("left"));
				oTextRect.top		= oNodeText.ReadAttributeDouble(_T("top"));
				oTextRect.right		= oTextRect.left + max(1.0, oNodeText.ReadAttributeDouble(_T("width")));
				oTextRect.bottom	= oTextRect.top + max(1.0, oNodeText.ReadAttributeDouble(_T("height")));

				oTextRect.Scale(dKoef);

				if (!bIsShapePresent)
				{
					if (NULL == pRectPlaceholder)
						m_rcBounds = oTextRect;
					else
						m_rcBounds = *pRectPlaceholder;
					m_oShape.m_pShape = new CPPTShape();
				}
				else
				{
					//m_rcBounds = oTextRect;

					double dWidthShape	= m_rcBounds.GetWidth();
					double dHeightShape = m_rcBounds.GetHeight();

					int x = round((oTextRect.left	- m_rcBounds.left)	* ShapeSize / dWidthShape);
					int y = round((oTextRect.top	- m_rcBounds.top)	* ShapeSize / dHeightShape);
					int r = round((oTextRect.right	- m_rcBounds.left)	* ShapeSize / dWidthShape);
					int b = round((oTextRect.bottom	- m_rcBounds.top)	* ShapeSize / dHeightShape);

					CString strTextRect = _T("");
					strTextRect.Format(_T("%d,%d,%d,%d;"), x, y, r, b);

					m_oShape.m_pShape->LoadTextRect(strTextRect);
				}

				NormalizeCoordsByMetric();

				XmlUtils::CXmlNode oNodeBody;
				if (oNodeText.GetNode(_T("body"), oNodeBody))
				{
					CTextLoader::ConvertText2(oNodeBody, m_oShape.m_oText, m_oMetric);
				}								
			}

			if (NULL == m_oShape.m_pShape)
				m_oShape.m_pShape = new CPPTShape();
		}

		void LoadShapeGeom(XmlUtils::CXmlNode& oNode)
		{
			NormalizeCoordsByMetric();
			
			int nW = (int)(m_oMetric.m_lMillimetresHor * 96 / 25.4);
			int nH = (int)(m_oMetric.m_lMillimetresVer * 96 / 25.4);

			m_oShape.m_strPPTXShape = oNode.GetAttribute(_T("pptx_shape"));

			CString strTxPath = oNode.GetAttribute(_T("txpath"));
			if (_T("") != strTxPath)
			{
				bool bIsBase64 = false;
				if (4 < strTxPath.GetLength())
				{
					CString strData64 = strTxPath.Mid(0, 4);
					if (_T("data") == strData64)
						bIsBase64 = true;
				}

				if (false == bIsBase64)
				{
					if (NULL == m_oShape.m_pShape)
						m_oShape.m_pShape = new CPPTShape();

					m_oShape.m_pShape->LoadPathList(_T("m,l,43200r43200,l43200,xe"));
					m_oShape.m_oPen.Alpha			= 0;
					m_oShape.m_oBrush.Type			= c_BrushTypeTexture;
					m_oShape.m_oBrush.TexturePath	= strTxPath;
					return;
				}
			}


			CString strSvg = _T("");
			strSvg.Format(_T("<svg width=\"%dpx\" height=\"%dpx\" viewBox=\"0 0 %d %d\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">"),
				nW, nH, nW, nH);

			XmlUtils::CXmlNodes oAllNodes;
			oNode.GetNodes(_T("g"), oAllNodes);

			int nCount = oAllNodes.GetCount();
			for (int i = 0; i < nCount; ++i)
			{
				XmlUtils::CXmlNode oNodeMem;
				oAllNodes.GetAt(i, oNodeMem);
				strSvg += oNodeMem.GetXml();
			}
			
			strSvg += _T("</svg>");			
			
			AVSGraphics::ISVGTransformer* pSVGTransformer = NULL;
			CoCreateInstance(__uuidof(AVSGraphics::SVGTransformer), NULL, CLSCTX_ALL, __uuidof(AVSGraphics::ISVGTransformer), (void**)&pSVGTransformer);

			ISVGConverter* pSVGConverter = NULL;
			CoCreateInstance(__uuidof(CSVGConverter), NULL, CLSCTX_ALL, __uuidof(ISVGConverter), (void**)&pSVGConverter);

			CSVGConverter* pClassSVGConverter = (CSVGConverter*)pSVGConverter;
			pClassSVGConverter->SetOffsetXY(m_rcBounds.left, m_rcBounds.top, m_rcBounds.right - m_rcBounds.left, m_rcBounds.bottom - m_rcBounds.top);

			IUnknown* punkRenderer = NULL;
			pSVGConverter->QueryInterface(IID_IUnknown, (void**)&punkRenderer);

			BSTR bsSVG = strSvg.AllocSysString();
			pSVGTransformer->Load(bsSVG);
			SysFreeString(bsSVG);

			pSVGConverter->put_Width(m_rcBounds.GetWidth());
			pSVGConverter->put_Height(m_rcBounds.GetHeight());
			pSVGTransformer->Draw(punkRenderer, 0, 0, nW, nH);
			RELEASEINTERFACE(punkRenderer);

			RELEASEINTERFACE(pSVGTransformer);

			if (NULL == m_oShape.m_pShape)
				m_oShape.m_pShape = new CPPTShape();

			pClassSVGConverter->SetSettings(m_oShape.m_oPen, m_oShape.m_oBrush, m_oShape.m_pShape->m_oPath);
			RELEASEINTERFACE(pSVGConverter);
		}
#endif

#ifdef ENABLE_ODP_TO_PPTX_CONVERT
		CString ConvertODPShapeToPPTX()				
		{
			CString strPPTXXml = _T("");

			COdpShape* pODPShape = dynamic_cast<COdpShape*>(m_oShape.m_pShape);
			NSGuidesVML::CFormulaConverterODP2PPT pODPConverter;
			if(!pODPShape->m_eType || pODPShape->m_eType == 1)
				return _T("");

			pODPConverter.ConvertOdp ( pODPShape );
			pODPConverter.ConvertType ( pODPShape->m_eType);

			//CSupportShape pSupShapeODP (pODPShape, pODPConverter);

			/*CBaseShape->m_strPath = pODPConverter.strPath;*/
			CPPTShape* pPPTShape = new CPPTShape();
			pPPTShape->m_strPath = pODPConverter.strPath;
			pPPTShape->m_oManager.m_arFormulas = pODPConverter.m_arFormulas;
			pPPTShape->m_arAdjustments = pODPShape->m_arAdjustments;
			pPPTShape->m_oPath = pODPShape->m_oPath;
			pPPTShape->m_arStringTextRects[0] = pODPConverter.strRect;
			//pPPTShape->m_eType = pODPShape->m_eType;
			pPPTShape->m_arHandles = pODPConverter.m_arHandles;

			NSGuidesVML::CFormParam pParamCoef;
			pParamCoef.m_eType = ptFormula;
			pParamCoef.m_lParam = pODPConverter.m_lPiFormulaNum;
			pParamCoef.m_lCoef = 1;

			strPPTXXml = ConvertPPTtoPPTX ( pPPTShape, pParamCoef );		

			return strPPTXXml;
		}

#endif

#ifdef ENABLE_PPT_TO_PPTX_CONVERT

		AVSINLINE CString ConvertPPTShapeToPPTX(bool bIsNamespace = false)
		{
			CPPTShape* pPPTShape = dynamic_cast<CPPTShape*>(m_oShape.m_pShape);
			if (NULL == pPPTShape)
			{
				// такого быть не может
				return _T("");
			}			

			NSGuidesVML::CFormParam pParamCoef;
			pParamCoef.m_eType = ptValue;
			pParamCoef.m_lParam = 65536;
			pParamCoef.m_lCoef = 65536;

			return ConvertPPTtoPPTX(pPPTShape, pParamCoef, bIsNamespace);			
		}

#ifndef OPTIMIZE_COMPILE_CONVERT_PPT_TO_PPTX

		CString ConvertPPTtoPPTX(CPPTShape* pPPTShape, const NSGuidesVML::CFormParam& pCoef, bool bIsNamespace = false)
		{
			if (pPPTShape->m_eType == PPTShapes::sptCRect)
			{
				if (bIsNamespace)
				{
					return _T("<a:prstGeom xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" prst=\"rect\"><a:avLst/></a:prstGeom>");
				}
				return _T("<a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom>");
			}
						
			CString strXmlPPTX = bIsNamespace ? _T("<a:custGeom xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">") : _T("<a:custGeom>");

			CFormulaConverter pFormulaConverter;

			//coeff
				pFormulaConverter.ConvertCoef ( pCoef );

			//guids----------------------------------------
			int nGuidCount = pPPTShape->m_oManager.m_arFormulas.GetSize();
			if (0 != nGuidCount)
			{
					pFormulaConverter.ConvertFormula ( pPPTShape->m_oManager.m_arFormulas);					
			}				

			//path------------------------------------------
			int nPathCount = pPPTShape->m_strPath.GetLength();
			if (0 != nPathCount && pPPTShape->m_eType != 1)
			{
				pFormulaConverter.ConvertPath (pPPTShape->m_strPath, pPPTShape->m_oPath);

				//string rect
				int nRectCount = (int)pPPTShape->m_arStringTextRects.GetCount();
				if (0 != nRectCount)
				{
					pFormulaConverter.ConvertTextRect ( pPPTShape->m_arStringTextRects[0] );					
				}

				int nHandlesCount = pPPTShape->m_arHandles.GetSize();
				int nAdjCount = pPPTShape->m_arAdjustments.GetSize();

				//handles
				if (0 != nHandlesCount || 0 != nAdjCount)
				{
					pFormulaConverter.ConvertHandle (pPPTShape->m_arHandles, pPPTShape->m_arAdjustments, pPPTShape->m_eType);
				}
				pFormulaConverter.strGuidsRes = pFormulaConverter.strCoef + pFormulaConverter.strGuidsRes;

				//adj----------------------------
				if (pFormulaConverter.strAdjRes.GetLength() == 0)
					strXmlPPTX += _T("<a:avLst/>");
				else
					strXmlPPTX += _T("<a:avLst>") + pFormulaConverter.strAdjRes + _T("</a:avLst>");

				//guids--------------------------
				if (pFormulaConverter.strGuidsRes.GetLength() == 0)
					strXmlPPTX += _T("<a:gdLst/>");
				else
					strXmlPPTX += _T("<a:gdLst>") + pFormulaConverter.strGuidsRes + _T("</a:gdLst>");

				//handles---------------------------
				if (pFormulaConverter.strHandleRes.GetLength() == 0)
					strXmlPPTX += _T("<a:ahLst/>");
				else
					strXmlPPTX += _T("<a:ahLst>") + pFormulaConverter.strHandleRes + _T("</a:ahLst>");
					
				//connectors-------------------------
				strXmlPPTX += _T("<a:cxnLst/>");
				
				//textRect---------------------------
				if (pFormulaConverter.strTextRect.GetLength() != 0)
					strXmlPPTX += pFormulaConverter.strTextRect;

				//path------------------------------
				strXmlPPTX += _T("<a:pathLst>");
				strXmlPPTX += pFormulaConverter.strPathRes;
				strXmlPPTX += _T("</a:pathLst>");					
			}

			strXmlPPTX += _T("</a:custGeom>");
			return strXmlPPTX;
		}
#else
		CString ConvertPPTtoPPTX(CPPTShape* pPPTShape, const NSGuidesVML::CFormParam& pCoef, bool bIsNamespace = false)
		{
			if (pPPTShape->m_eType == PPTShapes::sptCRect)
			{
				if (bIsNamespace)
				{
					return _T("<a:prstGeom xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" prst=\"rect\"><a:avLst/></a:prstGeom>");
				}
				return _T("<a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom>");
			}
			else if (pPPTShape->m_eType == PPTShapes::sptCLine)
			{
				if (bIsNamespace)
				{
					return _T("<a:prstGeom xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" prst=\"line\"><a:avLst/></a:prstGeom>");
				}
				return _T("<a:prstGeom prst=\"line\"><a:avLst/></a:prstGeom>");
			}
						
			CString strXmlPPTX = bIsNamespace ? _T("<a:custGeom xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">") : _T("<a:custGeom>");

			CFormulaConverter pFormulaConverter;

			//coeff
			pFormulaConverter.ConvertCoef(pCoef);

			//guids----------------------------------------
			int nGuidCount = pPPTShape->m_oManager.m_arFormulas.GetSize();
			if (0 != nGuidCount)
			{
				pFormulaConverter.ConvertFormula(pPPTShape->m_oManager.m_arFormulas);
			}				

			//path------------------------------------------
			int nPathCount = pPPTShape->m_strPath.GetLength();
			if (0 != nPathCount && pPPTShape->m_eType != 1)
			{
				pFormulaConverter.ConvertPath(pPPTShape->m_strPath, pPPTShape->m_oPath);

				//string rect
				int nRectCount = (int)pPPTShape->m_arStringTextRects.GetCount();
				if (0 != nRectCount)
				{
					pFormulaConverter.ConvertTextRect(pPPTShape->m_arStringTextRects[0]);
				}

				int nHandlesCount = pPPTShape->m_arHandles.GetSize();
				int nAdjCount = pPPTShape->m_arAdjustments.GetSize();

				//handles
				if (0 != nHandlesCount || 0 != nAdjCount)
				{
					pFormulaConverter.ConvertHandle(pPPTShape->m_arHandles, pPPTShape->m_arAdjustments, pPPTShape->m_eType);
				}

				//adj----------------------------
				if (pFormulaConverter.m_oAdjRes.GetSize() == 0)
					strXmlPPTX += _T("<a:avLst/>");
				else
					strXmlPPTX += _T("<a:avLst>") + pFormulaConverter.m_oAdjRes.GetXmlString() + _T("</a:avLst>");

				//guids--------------------------
				if (pFormulaConverter.m_oGuidsRes.GetSize() == 0)
					strXmlPPTX += _T("<a:gdLst>") + pFormulaConverter.m_oCoef.GetXmlString() + _T("</a:gdLst>");
				else
					strXmlPPTX += _T("<a:gdLst>") + pFormulaConverter.m_oCoef.GetXmlString() + pFormulaConverter.m_oGuidsRes.GetXmlString() + _T("</a:gdLst>");

				//handles---------------------------
				if (pFormulaConverter.m_oHandleRes.GetSize() == 0)
					strXmlPPTX += _T("<a:ahLst/>");
				else
					strXmlPPTX += _T("<a:ahLst>") + pFormulaConverter.m_oHandleRes.GetXmlString() + _T("</a:ahLst>");
					
				//connectors-------------------------
				strXmlPPTX += _T("<a:cxnLst/>");
				
				//textRect---------------------------
				if (pFormulaConverter.m_oTextRect.GetSize() != 0)
					strXmlPPTX += pFormulaConverter.m_oTextRect.GetXmlString();

				//path------------------------------
				strXmlPPTX += _T("<a:pathLst>");
				strXmlPPTX += pFormulaConverter.m_oPathRes.GetXmlString();
				strXmlPPTX += _T("</a:pathLst>");					
			}

			strXmlPPTX += _T("</a:custGeom>");
			return strXmlPPTX;
		}
#endif

#endif
		
	};

	class CAudioElement : public IElement
	{
	public:
		CString		m_strFileName;
		
		BYTE		m_nAmplify;
		bool		m_bWithVideo;
		double		m_dAudioDuration;

		double		m_dClipStartTime;
		double		m_dClipEndTime;
		BOOL		m_bLoop;

	public:
		CAudioElement() : IElement()
		{
			m_etType = etAudio;
			
			m_strFileName = _T("");

			m_nAmplify = 100;
			m_bWithVideo = false;

			m_dAudioDuration = 0.0;
			m_bLoop = false;

			m_dClipStartTime	= 0.0;
			m_dClipEndTime		= -1.0;
		}

		virtual ~CAudioElement()
		{
		}

		virtual CString SaveToXML()	
		{
			CString element = _T("");
			element.Format(_T("<AudioSource StartTime='%lf' Duration='%lf' Amplify='%lf'>"), m_dStartTime, m_dEndTime-m_dStartTime, (double)m_nAmplify);

			int lIndex = m_strFileName.Find(L"file:///");
			if (0 == lIndex)
			{
				m_strFileName = m_strFileName.Mid(8);
				m_strFileName.Replace('/', '\\');
				m_strFileName.Replace(L"%20", L" ");
			}

			CString strFileName = m_strFileName;
			CorrectXmlString(strFileName);

			CString source;
			source.Format(_T("<Source StartTime='%lf' EndTime='%lf' FilePath='%s' loop='%d' />"), m_dClipStartTime, m_dClipEndTime, strFileName, m_bLoop);
			element	+=	source;		

			CString animations;
			if((long)m_oAnimations.m_arAnimations.GetCount())								//	для audio только "media call's" - play - pause - stop
			{
				m_oAnimations.m_dSlideWidth		=	m_oMetric.m_lMillimetresHor;
				m_oAnimations.m_dSlideHeight	=	m_oMetric.m_lMillimetresVer;
				animations						=	m_oAnimations.ToXml(m_dStartTime, m_dEndTime);

				element		+=	animations;
			}
			
			CString timeLine;
			timeLine.Format ( _T("<timeline type = \"1\"  begin=\"%f\" end=\"%f\" fadein=\"0\" fadeout=\"0\" completeness=\"1.0\"/> "),	m_dStartTime, m_dEndTime );
			
			element			+=	timeLine;		
			element			+=	_T("</AudioSource>");

			return element;
		}

		virtual void WriteToXml(XmlUtils::CXmlWriter& oWriter)
		{
			CString strXml = SaveToXML();
			oWriter.WriteString(strXml);
		}

		virtual void ReadFromXml(XmlUtils::CXmlNode& oNode)
		{
		}

		virtual IElement* CreateDublicate()
		{
			CAudioElement* pAudioElement = new CAudioElement();
			
			SetProperiesToDublicate((IElement*)pAudioElement);

			pAudioElement->m_strFileName = m_strFileName;
			pAudioElement->m_nAmplify = m_nAmplify;
			pAudioElement->m_bWithVideo = m_bWithVideo;

			pAudioElement->m_dAudioDuration = m_dAudioDuration;
			pAudioElement->m_bLoop			= m_bLoop;

			pAudioElement->m_dClipStartTime	= m_dClipStartTime;
			pAudioElement->m_dClipEndTime	= m_dClipEndTime;

			return (IElement*)pAudioElement;
		}

		virtual void SetupProperty(CSlide* pSlide, CTheme* pTheme, CLayout* pLayout, CElementProperty* pProperty)
		{
		}

		virtual CString ToXmlVideoSource()
		{
			return SaveToXML();
		}
		virtual CString ToXmlEditor()
		{
			return _T("");
		}
	};
}
namespace NSStrings
{
	class CTextItem
	{
	protected:
		wchar_t*	m_pData;
		size_t		m_lSize;

		wchar_t*	m_pDataCur;
		size_t		m_lSizeCur;

	public:
		CTextItem()
		{
			m_pData = NULL;
			m_lSize = 0;

			m_pDataCur	= m_pData;
			m_lSizeCur	= m_lSize;
		}
		CTextItem(const CTextItem& oSrc)
		{
			m_pData = NULL;
			*this = oSrc;
		}
		CTextItem& operator=(const CTextItem& oSrc)
		{
			RELEASEMEM(m_pData);

			m_lSize		= oSrc.m_lSize;
			m_lSizeCur	= oSrc.m_lSizeCur;
			m_pData		= (wchar_t*)malloc(m_lSize * sizeof(wchar_t));

			memcpy(m_pData, oSrc.m_pData, m_lSizeCur * sizeof(wchar_t));
							
			m_pDataCur = m_pData + m_lSizeCur;

			return *this;
		}

		CTextItem(const size_t& nLen)
		{
			m_lSize = nLen;
			m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));
				
			m_lSizeCur = 0;
			m_pDataCur = m_pData;
		}
		CTextItem(wchar_t* pData, const size_t& nLen)
		{
			m_lSize = nLen;
			m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));

			memcpy(m_pData, pData, m_lSize * sizeof(wchar_t));
				
			m_lSizeCur = m_lSize;
			m_pDataCur = m_pData + m_lSize;
		}
		CTextItem(wchar_t* pData, BYTE* pUnicodeChecker = NULL)
		{
			size_t nLen = GetStringLen(pData);

			m_lSize = nLen;
			m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));

			memcpy(m_pData, pData, m_lSize * sizeof(wchar_t));
				
			m_lSizeCur = m_lSize;
			m_pDataCur = m_pData + m_lSize;

			if (NULL != pUnicodeChecker)
			{
				wchar_t* pMemory = m_pData;
				while (pMemory < m_pDataCur)
				{
					if (!pUnicodeChecker[*pMemory])
						*pMemory = wchar_t(' ');
					++pMemory;
				}
			}
		}
		virtual ~CTextItem()
		{
			RELEASEMEM(m_pData);
		}

		AVSINLINE void AddSize(const size_t& nSize)
		{
			if (NULL == m_pData)
			{
				m_lSize = max(nSize, 1000);				
				m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));
				
				m_lSizeCur = 0;
				m_pDataCur = m_pData;
				return;
			}

			if ((m_lSizeCur + nSize) > m_lSize)
			{
				while ((m_lSizeCur + nSize) > m_lSize)
				{
					m_lSize *= 2;
				}

				wchar_t* pRealloc = (wchar_t*)realloc(m_pData, m_lSize * sizeof(wchar_t));
				if (NULL != pRealloc)
				{
					// реаллок сработал
					m_pData		= pRealloc;
					m_pDataCur	= m_pData + m_lSizeCur;
				}
				else
				{
					wchar_t* pMalloc = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));
					memcpy(pMalloc, m_pData, m_lSizeCur * sizeof(wchar_t));

					free(m_pData);
					m_pData		= pMalloc;
					m_pDataCur	= m_pData + m_lSizeCur;
				}
			}
		}

	public:
		
		AVSINLINE void operator+=(const CTextItem& oTemp)
		{
			WriteString(oTemp.m_pData, oTemp.m_lSizeCur);
		}
		AVSINLINE void operator+=(_bstr_t& oTemp)
		{
			size_t nLen = oTemp.length();
			WriteString(oTemp.GetBSTR(), nLen);
		}
		AVSINLINE void operator+=(CString& oTemp)
		{
			size_t nLen = (size_t)oTemp.GetLength();

			#ifdef _UNICODE
			WriteString(oTemp.GetBuffer(), nLen);
			#else
			CStringW str = (CStringW)oTemp;
			WriteString(str.GetBuffer(), nLen);
			#endif
		}
		AVSINLINE wchar_t operator[](const size_t& nIndex)
		{
			if (nIndex < m_lSizeCur)
				return m_pData[nIndex];

			return 0;
		}

		AVSINLINE void SetText(BSTR& bsText)
		{
			ClearNoAttack();
			size_t nLen = GetStringLen(bsText);

			WriteString(bsText, nLen);

			for (size_t i = 0; i < nLen; ++i)
			{
				if (WCHAR(8233) == m_pData[i])
					m_pData[i] = WCHAR(' ');
			}
		}
		AVSINLINE void AddSpace()
		{
			AddSize(1);
			*m_pDataCur = wchar_t(' ');

			++m_lSizeCur;
			++m_pDataCur;
		}
		AVSINLINE void CorrectUnicode(const BYTE* pUnicodeChecker)
		{
			if (NULL != pUnicodeChecker)
			{
				wchar_t* pMemory = m_pData;
				while (pMemory < m_pDataCur)
				{
					if (!pUnicodeChecker[*pMemory])
						*pMemory = wchar_t(' ');
					++pMemory;
				}
			}
		}
		AVSINLINE void RemoveLastSpaces()
		{
			wchar_t* pMemory = m_pDataCur - 1;
			while ((pMemory > m_pData) && (wchar_t(' ') == *pMemory))
			{
				--pMemory;
				--m_lSizeCur;
				--m_pDataCur;
			}

		}
		AVSINLINE bool IsSpace()
		{
			if (1 != m_lSizeCur)
				return false;
			return (wchar_t(' ') == *m_pData);
		}
		
	public:
		AVSINLINE void WriteString(wchar_t* pString, const size_t& nLen)
		{
			AddSize(nLen);
			//memcpy(m_pDataCur, pString, nLen * sizeof(wchar_t));
			memcpy(m_pDataCur, pString, nLen << 1);
			m_pDataCur += nLen;
			m_lSizeCur += nLen;
		}
		AVSINLINE size_t GetCurSize()
		{
			return m_lSizeCur;
		}
		AVSINLINE size_t GetSize()
		{
			return m_lSize;
		}
		AVSINLINE void Clear()
		{
			RELEASEMEM(m_pData);
			
			m_pData = NULL;
			m_lSize = 0;

			m_pDataCur	= m_pData;
			m_lSizeCur	= 0;
		}
		AVSINLINE void ClearNoAttack()
		{
			m_pDataCur	= m_pData;
			m_lSizeCur	= 0;
		}

		AVSINLINE size_t GetStringLen(wchar_t* pData)
		{
			wchar_t* s = pData;
			for (; *s != 0; ++s);
			return (size_t)(s - pData);
		}

		AVSINLINE CString GetCString()
		{
			CString str(m_pData, (int)m_lSizeCur);
			return str;
		}
		AVSINLINE wchar_t* GetBuffer()
		{
			return m_pData;
		}
	};

	class CStringWriter : public CTextItem
	{
	public:
		CStringWriter() : CTextItem()
		{
		}
		virtual ~CStringWriter()
		{
		}

	public:
		
		AVSINLINE void WriteString(_bstr_t& bsString)
		{
			size_t nLen = bsString.length();
			CTextItem::WriteString(bsString.GetBSTR(), nLen);
		}
		AVSINLINE void WriteString(CString& sString)
		{
			size_t nLen = (size_t)sString.GetLength();

			#ifdef _UNICODE
			CTextItem::WriteString(sString.GetBuffer(), nLen);
			#else
			CStringW str = (CStringW)sString;
			WriteString(str.GetBuffer(), nLen);
			#endif
		}
		AVSINLINE void WriteString(wchar_t* pString, const size_t& nLen)
		{
			CTextItem::WriteString(pString, nLen);
		}
		AVSINLINE void Write(CStringWriter& oWriter)
		{
			CTextItem::WriteString(oWriter.m_pData, oWriter.m_lSizeCur);
		}
	};
}