#pragma once
#include "AudioOverlay.h"
#include "Slide.h"

namespace NSPresentationEditor
{
	class CDocument : public IBase
	{
	public:
		CAtlArray<CTheme>	m_arThemes;
		CAtlArray<CSlide>	m_arSlides;

		CMetricInfo			m_oInfo;

	public:

		CDocument() : m_arThemes(), m_arSlides()
		{
		}

		~CDocument()
		{
		}

		CDocument& operator=(const CDocument& oSrc)
		{
			m_arThemes.Copy(oSrc.m_arThemes);
			m_arSlides.Copy(oSrc.m_arSlides);	

			m_oInfo = oSrc.m_oInfo;

			return *this;
		}

		CDocument(const CDocument& oSrc)
		{
			*this = oSrc;
		}

	public:
		inline void Clear()
		{
			ClearThemes();
			ClearSlides();
		}

		// работа с темами
		inline void ClearThemes()
		{
			m_arThemes.RemoveAll();
		}
		inline void AddTheme(const CTheme& oTheme)
		{
			m_arThemes.Add(oTheme);
		}
		inline void UpdateTheme(size_t nIndex, const CTheme& oTheme)
		{
			if (nIndex >= m_arThemes.GetCount())
				return;

			m_arThemes[nIndex] = oTheme;
		}
		// работа со слайдом
		inline void ClearSlides()
		{
			m_arSlides.RemoveAll();
		}
		inline void AddSlide(const CSlide& oSlide)
		{
			m_arSlides.Add(oSlide);
		}
		inline void UpdateSlide(size_t nIndex, const CSlide& oSlide)
		{
			if (nIndex >= m_arSlides.GetCount())
				return;

			m_arSlides[nIndex] = oSlide;
		}

	public:

		// расчет layouts
		void CalculateLayouts()
		{
			size_t nCountThemes = m_arThemes.GetCount();
			for (size_t i = 0; i < nCountThemes; ++i)
			{
				CTheme* pTheme = &m_arThemes[i];
				size_t nCountLayouts = pTheme->m_arLayouts.GetCount();

				for (size_t j = 0; j < nCountLayouts; ++j)
				{
					CLayout* pLayout = &pTheme->m_arLayouts[j];

					size_t nCountElements = pLayout->m_arElements.GetCount();
					for (size_t nElem = 0; nElem < nCountElements; ++nElem)
					{
						if (pLayout->m_bUseThemeColorScheme)
						{
							pLayout->m_arElements[nElem]->SetupProperties(NULL, &m_arThemes[i], NULL);
						}
						else
						{
							pLayout->m_arElements[nElem]->SetupProperties(NULL, &m_arThemes[i], NULL);
						}
					}
				}
			}
		}
		
		// функция производит расчет по теме и слайдам
		void Calculate()
		{
			CalculateLayouts();

			size_t nCount = m_arSlides.GetCount();
			for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
			{
				LONG lThemeID	= m_arSlides[nIndex].m_lThemeID;
				
				if ((0 > lThemeID) || (lThemeID >= (LONG)m_arThemes.GetCount()))
				{
					m_arSlides[nIndex].Calculate(NULL);
					continue;
				}

				m_arSlides[nIndex].Calculate(&m_arThemes[lThemeID]);				
			}

			// проставим стили темы всем элементам
			size_t nCountThemes = m_arThemes.GetCount();
			for (size_t i = 0; i < nCountThemes; ++i)
			{
				CTheme* pTheme = &m_arThemes[i];
				
				size_t nCountEl = pTheme->m_arElements.GetCount();
				for (size_t j = 0; j < nCountEl; ++j)
				{
					pTheme->m_arElements[j]->m_pTheme = pTheme;
				}

				size_t nCountLayouts = pTheme->m_arLayouts.GetCount();
				for (size_t j = 0; j < nCountLayouts; ++j)
				{
					CLayout* pLayout = &pTheme->m_arLayouts[j];

					nCountEl = pLayout->m_arElements.GetCount();
					for (size_t k = 0; k < nCountEl; ++k)
					{
						pLayout->m_arElements[k]->m_pTheme = pTheme;
					}
				}
			}
		}

		// функция нужна для отмены старой темы/шаблона
		void ClearPreset()
		{
			size_t nCount = m_arSlides.GetCount();
			for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
			{
				m_arSlides[nIndex].ClearPreset();
			}
		}

		// videosource
		CString SaveToXML()
		{
			CString strXmlSource		= _T("");
			CString strXmlVideoOverlay	= _T("");
			
			CAudioOverlay oAudioOverlay;
			CalculateTimes(oAudioOverlay);

			size_t nCountSlides = m_arSlides.GetCount();
			if ( nCountSlides > 0 )
			{
				double dDuration = 0.0;
				for (size_t nIndex = 0; nIndex < m_arSlides.GetCount(); ++nIndex)
				{
					dDuration += m_arSlides[nIndex].m_dDuration;
				}

				CString XmlSlideSource	=	_T("");
			
				for ( size_t nIndex = 0; nIndex < m_arSlides.GetCount(); ++nIndex )
				{
					CSlide* pSlide = &m_arSlides[nIndex];
					
					CString XmlCompose					= _T("");
					CString XmlTransforms				= _T("");

					bool bIsTransition = (0 != pSlide->m_oSlideShow.m_oTransition.m_dSpeed);
					if ( bIsTransition )
					{
						XmlCompose		=	GetXmlSlideTransition (m_arSlides[nIndex]/*, oAudioOverlay*/);
					}

					XmlSlideSource.Format ( _T("<ColorSource Color=\"16777215\" Duration=\"%lf\" widthmetric=\"%lf\" heightmetric=\"%lf\">"), 
						pSlide->m_dDuration, (double)pSlide->m_lWidth, (double)pSlide->m_lHeight );

					double dScaleHor	=	(double)(pSlide->m_lWidth)	/ pSlide->m_lOriginalWidth;
					double dScaleVer	=	(double)(pSlide->m_lHeight) / pSlide->m_lOriginalHeight;

					if ( FALSE )
					{
						// анимации теперь должны быть реализованы в элементе
					}
					else
					{
						int nElements = (int)pSlide->m_arElements.GetCount();
						for ( int nElem = 0; nElem < nElements; ++nElem )
						{
							if (etVideo == pSlide->m_arElements[nElem]->m_etType)
							{
								CString strParam = pSlide->m_arElements[nElem]->SaveToXML();
								strXmlVideoOverlay += strParam;
							}
							else if (etAudio == pSlide->m_arElements[nElem]->m_etType)
							{
								// все сделается в аудиооверлее
								continue;
							}
							else
							{
								CString strParam = pSlide->m_arElements[nElem]->SaveToXML();
								XmlTransforms += strParam;
							}
						}
					}
				
					if ( bIsTransition && (0 == nIndex) && (XmlCompose.GetLength () > 1) )
					{
						CString strMem = _T("");
						strMem.Format(_T("<ColorSource Color=\"0\" Duration=\"%d\" widthmetric=\"%lf\" heightmetric=\"%lf\" />"),
							pSlide->m_oSlideShow.m_oTransition.m_dSpeed, (double)pSlide->m_lWidth, (double)pSlide->m_lHeight );
						
						strXmlSource +=	strMem;
					}
					
					strXmlSource		+=	XmlCompose;
					
					XmlSlideSource		+=	_T("<VideoTransforms>")	+ XmlTransforms	+	_T("</VideoTransforms>");
					XmlSlideSource		+=	_T("</ColorSource>");
				
					strXmlSource		+=	XmlSlideSource;
				}

				strXmlSource			=	_T("<VideoSources>") + strXmlSource + _T("</VideoSources>");
			}

			oAudioOverlay.Calculate();
			CString strAudioOverlay = oAudioOverlay.GetAudioOverlay();
			
			CString strXmlOverlays        =   _T("<VideoOverlays>")	+ strXmlVideoOverlay +	_T("</VideoOverlays>") + _T("<AudioOverlays>") + 
				_T("<AudioSources>") + strAudioOverlay +	_T("</AudioSources>") +	_T("</AudioOverlays>");

			strXmlSource = _T("<SingleSource>") + strXmlSource + _T("</SingleSource>");
			strXmlSource = _T("<MultiSource>") + strXmlSource + strXmlOverlays + _T("</MultiSource>");

			return strXmlSource;
		}

		CString GetXmlSlideTransition ( CSlide& oSlide/*, CAudioOverlay& oAudioOverlay*/ )
		{
			CString Source	=	CString ( _T("") );

			int EffectID	=	1;
			LONG lEffectDirection = oSlide.m_oSlideShow.m_oTransition.m_nEffectDirection;

			switch ( oSlide.m_oSlideShow.m_oTransition.m_nEffectType )
			{
			case 0 : // Cut 
				{
					// Отрабатывается выше ( выкидывать слайд, или вставлять на несколько ms )
				}
				break;
			case 1 : // Random	//
				{
					// рандом вставить
					EffectID	=	1;							//	Fade Solid Smal
				}
				break;
			case  2 : // Blinds 
				{
					if ( 0x00 == lEffectDirection )	//	Vertical	//	Slide Side Top To Bottom
						EffectID	=	1003;
					if ( 0x01 == lEffectDirection )	//	Horizontal	//	Slide Side Left To Righ
						EffectID	=	1001;
				}
				break;
			case  3 : // Checker 
				{
					if ( 0x00 == lEffectDirection )	//	Horizontal	//	WipeCheckerRightToLef
						EffectID	=	2901;
					if ( 0x01 == lEffectDirection )	//	Vertical	//	WipeCheckerTopToBottom
						EffectID	=	2903;
				}
				break;
			case  4 : // Cover 
				{
					//0x00  Left 
		#pragma message ("TODO : Cover - добавить такой эффект в ImageStudio")	
					//0x01  Up 
					//0x02  Right 
					//0x03  Down 
					//0x04  Left Up 
					//0x05  Right Up 
					//0x06  Left Down 
					//0x07  Right Down 
				}
				break;
			case  5 : // Dissolve 
				{
		#pragma message ("TODO : Dissolve - добавить такой эффект в ImageStudio")
				
					EffectID	=	1;							//	Fade Solid Smal
				}
				break;
			case  6 : // Fade 
				{
					EffectID	=	4904;						//	Flash Dark
				}
				break;
			case  7 : // Uncover 
				{
		#pragma message ("TODO : Uncover - добавить такой эффект в ImageStudio")	
					//0x00  Left 
					//0x01  Up 
					//0x02  Right 
					//0x03  Down 
					//0x04  Left Up 
					//0x05  Right Up 
					//0x06  Left Down 
					//0x07  Right Down 
				}
				break;
			case  8 : // Random Bars 
				{
					if ( 0x00 == lEffectDirection )	//	Horizontal
						EffectID	=	5331;
					if ( 0x01 == lEffectDirection )	//	Vertical
						EffectID	=	5332;
				}
				break;
			case  9 : // Strips 
				{
					if ( 0x04 == lEffectDirection )	//	Left Up		//	Mosaic Right Bottom
						EffectID	=	110;
					if ( 0x05 == lEffectDirection )	//	Right Up	//	Mosaic Left Bottom
						EffectID	=	111;
					if ( 0x06 == lEffectDirection )	//	Left Down	//	Mosaic Right Top
						EffectID	=	109;
					if ( 0x07 == lEffectDirection )	//	Right Down	//	Mosaic Left Top
						EffectID	=	108;
				}
				break;
			case  10: // Wipe 
				{
					if ( 0x00 == lEffectDirection )	//	Left			//	Reveal, Left
						EffectID	=	5322;
					if ( 0x01 == lEffectDirection )	//	Up				//	Reveal, Up
						EffectID	=	5323;
					if ( 0x02 == lEffectDirection )	//	Right			//	Reveal, Right
						EffectID	=	5321;
					if ( 0x03 == lEffectDirection )	//	Down			//	Reveal, Down
						EffectID	=	5320;
				}
				break;
			case  11: // Box In/Out 
				{
					if ( 0x00 == lEffectDirection )	//	Wipe Center Box Out 
						EffectID	=	702;
					if ( 0x01 == lEffectDirection )	//	Wipe Center Box In
						EffectID	=	701;
				}
				break;
			case  13 :// Split 
				{
					if ( 0x00 == lEffectDirection )	//	Horizontally out	//	Split, Horizontal
						EffectID	=	5324;
		#pragma message ("TODO : Split Horizontal In - добавить такой эффект в ImageStudio")	
					if ( 0x01 == lEffectDirection )	//	Horizontally in		//	
						EffectID	=	5324;					
					if ( 0x02 == lEffectDirection )	//	Vertically out		//	Split, Vertical
						EffectID	=	5325;
		#pragma message ("TODO : Split Vertical In - добавить такой эффект в ImageStudio")
					if ( 0x03 == lEffectDirection )	//	Vertically in		//	
						EffectID	=	5325;						
				}
				break;
			case  17 : // Diamond 
				{
					EffectID	=	704;						//	Wipe Center Diamond Out
				}
				break;
			case  18 : // Plus
				{
					EffectID	=	708;						//	Wipe Center Cross Out
				}
				break;
			case  19 : // Wedge 
				{
					EffectID	=	3006;						//	Clock Top Opposite
				}
				break;
			case 20 : // Push 
				{
					if ( 0x00 == lEffectDirection )	//	Left			//	Push Side Right To Left
						EffectID	=	1402;
					if ( 0x01 == lEffectDirection )	//	Up				//	Push Side Bottom To Top
						EffectID	=	1404;
					if ( 0x02 == lEffectDirection )	//	Right			//	Push Side Left To Right
						EffectID	=	1401;
					if ( 0x03 == lEffectDirection )	//	Down			//	Push Side Top To Bottom
						EffectID	=	1403;
				}
				break;
			case  21 : // Comb 
				{
					if ( 0x00 == lEffectDirection )	//	Horizontal			//	MosaicStrips
						EffectID	=	103;
		#pragma message ("TODO : MosaicStrips Vertical - добавить такой эффект в ImageStudio")
					if ( 0x01 == lEffectDirection )	//	Vertical			//	MosaicStrips
						EffectID	=	103;
				}
				break;
			case  22 : // Newsflash 
				{
					EffectID	=	313;						//	Rotate Twice Out Clockwise
				}
				break;
			case  23 :// AlphaFade 
				{
					EffectID	=	1;							//	Fade Solid Smal
				}
				break;
			case 26 : // Wheel  
				{
					if ( 0x01 == lEffectDirection )	//	ClockBottomReverse
						EffectID	=	3004;
					if ( 0x02 == lEffectDirection )	//	Clock2AnglesClockwise
						EffectID	=	3013;
					if ( 0x03 == lEffectDirection )	//	Clock4AnglesClockwise
						EffectID	=	3017;
					if ( 0x04 == lEffectDirection )	//	Clock6AnglesClockwise
						EffectID	=	3019;
					if ( 0x08 == lEffectDirection )	//	Clock8AnglesClockwise
						EffectID	=	3021;
				}
				break;
			case  27 : // Circle	
				{
					EffectID	=	706;						//	Wipe Center Circle Out
				}
				break;
			default :
				{
					return Source;
				}
				break;
			}

			Source.Format (	_T("<VideoCompose Time = \"%lf\" effectid=\"%d\" />"), oSlide.m_oSlideShow.m_oTransition.m_dSpeed, EffectID );	 

			/*

			if (oSlide.m_oSlideShow.m_oTransition.m_bAudioPresent)
			{
				CAudioElement oAudioElem;
				
				oAudioElem.m_dStartTime		= oSlide.m_dStartTime - oSlide.m_oSlideShow.m_oTransition.m_dSpeed;
				oAudioElem.m_dEndTime		= oAudioOverlay.m_dAllDuration;
				oAudioElem.m_strFileName	= oSlide.m_oSlideShow.m_oTransition.m_oAudio.m_strFileName;

				CAudioPart oPart(&oAudioElem);
				oPart.m_bStop = oSlide.m_oSlideShow.m_oTransition.m_bStopSound;
				oPart.m_bLoop = oSlide.m_oSlideShow.m_oTransition.m_bLoopSound;
				oPart.m_bIsTransition = true;

				oAudioOverlay.m_arParts.Add(oPart);
			}

			*/
			
			return Source;
		}

		double CalculateTimes(CAudioOverlay& oAudioOverlay)
		{
			CaclulateSlideTimes();

			LONG lCount = (LONG)m_arSlides.GetCount();

			double dAllDuration = 0;
			if (lCount > 0)
			{
				dAllDuration = m_arSlides[lCount - 1].m_dEndTime;
			}

			oAudioOverlay.m_dAllDuration = dAllDuration;

			for (LONG i = 0; i < lCount; ++i)
			{
				CalculateSlideElements(i, oAudioOverlay);
			}

			return dAllDuration;
		}

		void CaclulateSlideTimes()
		{
			double Transition1	= 0;
			double Duration		= 0;
			double Transition2	= 0;

			int nCount = (int)m_arSlides.GetCount();
			for (int i = 0; i < nCount; ++i)
			{
				CSlide* pSlide = &m_arSlides[i];
				
				double dSlideDuration = 0;
				if (pSlide->m_oSlideShow.m_dSlideDuration > 0)
				{
					dSlideDuration = m_arSlides[i].m_oSlideShow.m_dSlideDuration;
				}
				if (0 == dSlideDuration)
				{
					// решено сделать так: если время слайда не выставлено - то берем максимальную
					// длительность видео, лежащего на нем. Если таких нет - то дефолтовая длительность,
					// впрочем она уже выставлена

					size_t nSize = pSlide->m_arElements.GetCount();
					for (size_t j = 0; j < nSize; ++j)
					{
						if (etVideo == pSlide->m_arElements[j]->m_etType)
						{
							dSlideDuration = max(dSlideDuration, ((CVideoElement*)pSlide->m_arElements[j])->m_dVideoDuration);
						}
					}

					if (0 == dSlideDuration)
					{
						dSlideDuration = pSlide->m_dDuration;
					}
				}

				pSlide->m_dDuration = dSlideDuration;
				
				Transition1 = pSlide->m_oSlideShow.m_oTransition.m_dSpeed;

				if (i < (nCount - 1))
					Transition2 = m_arSlides[i + 1].m_oSlideShow.m_oTransition.m_dSpeed;
				
				Duration = pSlide->m_dDuration;

				if (i == 0)
				{
					pSlide->m_dStartTime = Transition1;
				}
				else
				{
					pSlide->m_dStartTime = m_arSlides[i-1].m_dStartTime + m_arSlides[i-1].m_dDuration;
				}
				
				pSlide->m_dDuration	= Transition1 + Duration + Transition2;
				pSlide->m_dEndTime	= pSlide->m_dStartTime + pSlide->m_dDuration;
			}
		}

		void CalculateSlideElements(int nIndex, CAudioOverlay& oAudioOverlay)
		{
			double dStartTime	= m_arSlides[nIndex].m_dStartTime;
			double dEndTime		= m_arSlides[nIndex].m_dEndTime;
			double dDuration	= m_arSlides[nIndex].m_dDuration;
			
			CSlide* pSlide = &m_arSlides[nIndex];
			
			size_t nCountElems = pSlide->m_arElements.GetCount();
			for (size_t i = 0; i < nCountElems; ++i)
			{
				IElement* pElement = pSlide->m_arElements[i];

				switch (pElement->m_etType)
				{
					//case etPicture:
					//case etShape:
					//case etVideo:
					//case etText:
					//	{
					//					//pElement->m_dStartTime	= 0;
					//					//pElement->m_dEndTime	= dDuration;
					//
					//		break;
					//	}
				case etAudio:
					{
						CAudioElement* pAudioElem = dynamic_cast<CAudioElement*>(pElement);

						if (NULL != pAudioElem)
						{
							// а другого и быть не может
							if (pAudioElem->m_bWithVideo)
							{
								pElement->m_dStartTime	= dStartTime;
								pElement->m_dEndTime	= dEndTime;
							}
							else if (pAudioElem->m_bLoop)
							{
								pElement->m_dStartTime	= dStartTime;
								pElement->m_dEndTime	= oAudioOverlay.m_dAllDuration/* - dStartTime*/;
							}
							else
							{
								pElement->m_dStartTime	= dStartTime;
								pElement->m_dEndTime	= min(dStartTime + pAudioElem->m_dAudioDuration, oAudioOverlay.m_dAllDuration/* - dStartTime*/);
							}

							CAudioPart oPart(pAudioElem);
							oAudioOverlay.m_arParts.Add(oPart);					
						}

						break;
					}
				default:
					//{
					//pElement->m_dStartTime	= 0;
					//pElement->m_dEndTime	= dDuration;

					break;
					//}
				};
			}
		}

	public:

		virtual void ReadFromXml(XmlUtils::CXmlNode& oNode)
		{
#ifdef _PRESENTATION_WRITER_
			Clear();

			m_oInfo.m_lMillimetresHor	= oNode.ReadAttributeInt(_T("hor_mm"));
			m_oInfo.m_lMillimetresVer	= oNode.ReadAttributeInt(_T("ver_mm"));
			//m_oInfo.m_lUnitsHor			= oNode.ReadAttributeInt(_T("hor_uni"));
			//m_oInfo.m_lUnitsVer			= oNode.ReadAttributeInt(_T("ver_uni"));
			m_oInfo.m_lUnitsHor			= 36000 * m_oInfo.m_lMillimetresHor;
			m_oInfo.m_lUnitsVer			= 36000 * m_oInfo.m_lMillimetresVer;

			XmlUtils::CXmlNode oNodeThemes;			
			if (oNode.GetNode(_T("Themes"), oNodeThemes))
			{
				XmlUtils::CXmlNodes oThemes;
				if (oNodeThemes.GetNodes(_T("Theme"), oThemes))
				{
					int nCountThemes = oThemes.GetCount();
					for (int i = 0; i < nCountThemes; ++i)
					{
						XmlUtils::CXmlNode oNodeT;
						oThemes.GetAt(i, oNodeT);

						m_arThemes.Add();

						CTheme* pTheme = &m_arThemes[m_arThemes.GetCount() - 1];
						pTheme->m_oInfo = m_oInfo;
						pTheme->ReadFromXml(oNodeT);
					}
				}
			}

			XmlUtils::CXmlNode oNodeSlides;
			if (oNode.GetNode(_T("Slides"), oNodeSlides))
			{
				XmlUtils::CXmlNodes oSlides;
				if (oNodeSlides.GetNodes(_T("Slide"), oSlides))
				{
					int nCountSlides = oSlides.GetCount();
					for (int i = 0; i < nCountSlides; ++i)
					{
						XmlUtils::CXmlNode oNodeS;
						oSlides.GetAt(i, oNodeS);

						m_arSlides.Add();
						
						CSlide* pSlide = &m_arSlides[m_arSlides.GetCount() - 1];

						pSlide->m_lThemeID	= oNodeS.ReadAttributeInt(_T("themeid"));
						pSlide->m_lLayoutID	= oNodeS.ReadAttributeInt(_T("layoutid"));
					
						pSlide->m_bShowMasterShapes	= (1 == oNodeS.ReadAttributeInt(_T("usethemeobjects")));
						pSlide->SetMetricInfo(m_oInfo);
						pSlide->ReadFromXml(oNodeS);
					}
				}
			}
#endif
		}

		virtual void WriteToXml(XmlUtils::CXmlWriter& oWriter)
		{
		}

	public:
		
		CString ToXmlVideoSource()
		{
			return SaveToXML();
		}
		CString ToXmlVideoSource2()
		{
			CString strXmlSource		= _T("");
			CString strXmlVideoOverlay	= _T("");
			
			//CAudioOverlay oAudioOverlay;
			//CalculateTimes(oAudioOverlay);

			size_t nCountSlides = m_arSlides.GetCount();
			if ( nCountSlides > 0 )
			{
				double dDuration = 0.0;
				for (size_t nIndex = 0; nIndex < m_arSlides.GetCount(); ++nIndex)
				{
					dDuration += m_arSlides[nIndex].m_dDuration;
				}

				CString XmlSlideSource	=	_T("");
			
				for ( size_t nIndex = 0; nIndex < m_arSlides.GetCount(); ++nIndex )
				{
					CSlide* pSlide = &m_arSlides[nIndex];
					
					CString XmlCompose					= _T("");
					CString XmlTransforms				= _T("");

					bool bIsTransition = (0 != pSlide->m_oSlideShow.m_oTransition.m_dSpeed);
					if ( bIsTransition )
					{
						XmlCompose		=	GetXmlSlideTransition (m_arSlides[nIndex]/*, oAudioOverlay*/);
					}

					XmlSlideSource.Format ( _T("<ColorSource Color=\"16777215\" Duration=\"%lf\" widthmetric=\"%lf\" heightmetric=\"%lf\">"), 
						pSlide->m_dDuration, (double)pSlide->m_lWidth, (double)pSlide->m_lHeight );

					double dScaleHor	=	(double)(pSlide->m_lWidth)	/ pSlide->m_lOriginalWidth;
					double dScaleVer	=	(double)(pSlide->m_lHeight) / pSlide->m_lOriginalHeight;

					if ( FALSE )
					{
						// анимации теперь должны быть реализованы в элементе
					}
					else
					{
						CTheme* pTheme = NULL;
						CLayout* pLayout = NULL;
						
						if ((0 <= pSlide->m_lThemeID) && (pSlide->m_lThemeID < (LONG)m_arThemes.GetCount()))
						{
							pTheme = &m_arThemes[pSlide->m_lThemeID];
						}
						if ((NULL != pTheme) && ((0 <= pSlide->m_lLayoutID) && (pSlide->m_lLayoutID < (LONG)pTheme->m_arLayouts.GetCount())))
						{
							pLayout = &pTheme->m_arLayouts[pSlide->m_lLayoutID];
						}

						// background
						#ifdef PPT_DEF
						CShapeElement oElem(NSPresentationEditor::NSBaseShape::ppt, PPTShapes::sptCRect);
						#else
						#ifdef ODP_DEF
						CShapeElement oElem(NSPresentationEditor::NSBaseShape::odp, OdpShapes::sptCRect);
						oElem.m_oShape.m_dWidthLogic	= ((COdpShape*)oElem.m_oShape.m_pShape)->FManager.GetValue(_T("width"));
						oElem.m_oShape.m_dHeightLogic	= ((COdpShape*)oElem.m_oShape.m_pShape)->FManager.GetValue(_T("height"));
						#else
						CShapeElement oElem(NSPresentationEditor::NSBaseShape::pptx, OOXMLShapes::sptCRect);
						#endif
						#endif

						oElem.m_oMetric = m_oInfo;
						oElem.m_rcBoundsOriginal.left	= 0.0;
						oElem.m_rcBoundsOriginal.right	= (double)m_oInfo.m_lUnitsHor;
						oElem.m_rcBoundsOriginal.top	= 0.0;
						oElem.m_rcBoundsOriginal.bottom	= (double)m_oInfo.m_lUnitsVer;

						double dScaleX = (double)m_oInfo.m_lMillimetresHor / m_oInfo.m_lUnitsHor;
						double dScaleY = (double)m_oInfo.m_lMillimetresVer / m_oInfo.m_lUnitsVer;

						oElem.NormalizeCoords(dScaleX, dScaleY);
						
						if (pSlide->m_bIsBackground)
						{
							oElem.m_oShape.m_oBrush = pSlide->m_oBackground;
							oElem.SetupProperties(pSlide, pTheme, pLayout);
						}
						else if ((NULL != pLayout) && pLayout->m_bIsBackground)
						{
							oElem.m_oShape.m_oBrush = pLayout->m_oBackground;
							oElem.SetupProperties(NULL, pTheme, pLayout);
						}
						else if (NULL != pTheme)
						{
							oElem.m_oShape.m_oBrush = pTheme->m_oBackground;
							oElem.SetupProperties(NULL, pTheme, NULL);
						}

						XmlTransforms += oElem.SaveToXML();

						// theme elements
						if (pSlide->m_bShowMasterShapes && (NULL != pLayout) && pLayout->m_bShowMasterShapes && (NULL != pTheme))
						{							
							size_t nCount = pTheme->m_arElements.GetCount();
							for (size_t ii = 0; ii < nCount; ++ii)
								XmlTransforms += pTheme->m_arElements[ii]->SaveToXML();
						}

						// layout elements
						if (NULL != pLayout)
						{
							size_t nCount = pLayout->m_arElements.GetCount();
							for (size_t ii = 0; ii < nCount; ++ii)
							{
								if (-1 == pLayout->m_arElements[ii]->m_lPlaceholderType)
									XmlTransforms += pLayout->m_arElements[ii]->SaveToXML();
							}
						}
						
						int nElements = (int)pSlide->m_arElements.GetCount();
						for ( int nElem = 0; nElem < nElements; ++nElem )
						{
							if (etVideo == pSlide->m_arElements[nElem]->m_etType)
							{
								CString strParam = pSlide->m_arElements[nElem]->SaveToXML();
								XmlTransforms += strParam;
							}
							else if (etAudio == pSlide->m_arElements[nElem]->m_etType)
							{
								CString strParam = pSlide->m_arElements[nElem]->SaveToXML();
								XmlTransforms += strParam;
							}
							else
							{
								CString strParam = pSlide->m_arElements[nElem]->SaveToXML();
								XmlTransforms += strParam;
							}
						}
					}
				
					if ( bIsTransition && (0 == nIndex) && (XmlCompose.GetLength () > 1) )
					{
						CString strMem = _T("");
						strMem.Format(_T("<ColorSource Color=\"0\" Duration=\"%lf\" widthmetric=\"%lf\" heightmetric=\"%lf\" />"),
							pSlide->m_oSlideShow.m_oTransition.m_dSpeed, (double)pSlide->m_lWidth, (double)pSlide->m_lHeight );
						
						strXmlSource +=	strMem;
					}
					
					strXmlSource		+=	XmlCompose;
					
					XmlSlideSource		+=	_T("<VideoTransforms>")	+ XmlTransforms	+	_T("</VideoTransforms>");
					XmlSlideSource		+=	_T("</ColorSource>");
				
					strXmlSource		+=	XmlSlideSource;
				}

				strXmlSource			=	_T("<VideoSources>") + strXmlSource + _T("</VideoSources>");
			}

			//oAudioOverlay.Calculate();
			//CString strAudioOverlay = oAudioOverlay.GetAudioOverlay();
			//
			//CString strXmlOverlays        =    _T("<VideoOverlays>")	+ strXmlVideoOverlay +	_T("</VideoOverlays>") + 
			//	_T("<AudioOverlays>") + _T("<AudioSources>") + strAudioOverlay +	_T("</AudioSources>") +	_T("</AudioOverlays>");

			strXmlSource = _T("<SingleSource>") + strXmlSource + _T("</SingleSource>");
			strXmlSource = _T("<MultiSource>") + strXmlSource /*+ strXmlOverlays*/ + _T("</MultiSource>");

			return strXmlSource;
		}
		CString ToXmlEditor()
		{
			CString strXmlSource		= _T("");
			
			CAudioOverlay oAudioOverlay;
			CalculateTimes(oAudioOverlay);

			size_t nCountSlides = m_arSlides.GetCount();
			if ( nCountSlides > 0 )
			{
				CString XmlSlideSource	=	_T("");
			
				for ( size_t nIndex = 0; nIndex < nCountSlides; ++nIndex )
				{
					CSlide* pSlide = &m_arSlides[nIndex];

					CString XmlTransforms				= _T("");

					LONG lWidthPix		= (LONG)(96.0 * pSlide->m_lWidth  / 25.4);
					LONG lHeightPix		= (LONG)(96.0 * pSlide->m_lHeight / 25.4);

					XmlSlideSource.Format ( _T("<Slide color=\"16777215\" duration=\"%lf\" width=\"%d\" height=\"%d\" transition=\"%d\" transition-speed=\"%.2lf\">"), 
						pSlide->m_dDuration, lWidthPix, lHeightPix, 
						pSlide->m_oSlideShow.m_oTransition.m_nEffectType, pSlide->m_oSlideShow.m_oTransition.m_dSpeed );

					int nElements = (int)pSlide->m_arElements.GetCount();
					for ( int nElem = 0; nElem < nElements; ++nElem )
					{
						if (etAudio == pSlide->m_arElements[nElem]->m_etType)
						{
							// все сделается в аудиооверлее
							continue;
						}
						
						XmlTransforms += pSlide->m_arElements[nElem]->ToXmlEditor();
					}
				
					XmlSlideSource		+=	XmlTransforms;
					
					XmlSlideSource		+=	_T("</Slide>");
				
					strXmlSource		+=	XmlSlideSource;
				}

				strXmlSource			=	_T("<Graphics>") + strXmlSource + _T("</Graphics>");
			}

			oAudioOverlay.Calculate();
			
			strXmlSource = _T("<Presentation xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">") + strXmlSource + _T("<Audio>") + oAudioOverlay.GetAudioOverlay() + _T("</Audio>") + _T("</Presentation>");
			return strXmlSource;
		}

		CString ToXmlEditor2()
		{
			CStringWriter oWriter;

			CString strMetric = _T("");
			strMetric.Format(_T(" hor_mm=\"%d\" ver_mm=\"%d\" hor_uni=\"%d\" ver_uni=\"%d\">"),
				m_oInfo.m_lMillimetresHor, m_oInfo.m_lMillimetresVer, m_oInfo.m_lUnitsHor, m_oInfo.m_lUnitsVer);

			oWriter.WriteString(_T("<Presentation xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\"") + strMetric);

			oWriter.WriteString(_T("<Themes>"));
			size_t nCount = m_arThemes.GetCount();
			for (size_t i = 0; i < nCount; ++i)
			{
				oWriter.WriteString(m_arThemes[i].ToXmlEditor(m_oInfo));
			}
			oWriter.WriteString(_T("</Themes>"));

			oWriter.WriteString(_T("<Slides>"));
			nCount = m_arSlides.GetCount();
			for (size_t i = 0; i < nCount; ++i)
			{
				NSPresentationEditor::CTheme*		pThemeSlide = NULL;
				NSPresentationEditor::CLayout*		pLayoutSlide = NULL;

				NSPresentationEditor::CSlide*		pSlide = &m_arSlides[i];

				if ((0 <= pSlide->m_lThemeID) && (pSlide->m_lThemeID < (LONG)m_arThemes.GetCount()))
				{
					pThemeSlide = &m_arThemes[pSlide->m_lThemeID];
				}
				if ((NULL != pThemeSlide) && (0 <= pSlide->m_lLayoutID) && (pSlide->m_lLayoutID < (LONG)pThemeSlide->m_arLayouts.GetCount()))
				{
					pLayoutSlide = &pThemeSlide->m_arLayouts[pSlide->m_lLayoutID];
				}

				oWriter.WriteString(m_arSlides[i].ToXmlEditor(pThemeSlide, pLayoutSlide, m_oInfo));
			}
			oWriter.WriteString(_T("</Slides>"));

			oWriter.WriteString(_T("</Presentation>"));

			oWriter.CorrectUnicodeString();
			return oWriter.GetData();
		}

		void CalculateEditor(const NSPresentationEditor::CMetricInfo& oInfo, bool bIsPlaceholderSetUp = false)
		{
			m_oInfo = oInfo;

			double dScaleX = (double)m_oInfo.m_lMillimetresHor / m_oInfo.m_lUnitsHor;
			double dScaleY = (double)m_oInfo.m_lMillimetresVer / m_oInfo.m_lUnitsVer;

			size_t nCountThemes = m_arThemes.GetCount();
			for (size_t i = 0; i < nCountThemes; ++i)
			{
				CTheme* pTheme = &m_arThemes[i];
				pTheme->CalculateStyles();

				size_t nCountElems = pTheme->m_arElements.GetCount();
				for (size_t nIndexEl = 0; nIndexEl < nCountElems; ++nIndexEl)
				{
					IElement* pElement = pTheme->m_arElements[nIndexEl];

					pElement->m_pTheme = pTheme;
					pElement->m_pLayout = NULL;
					
					pElement->m_oMetric = m_oInfo;
					pElement->NormalizeCoords(dScaleX, dScaleY);

					pElement->SetupProperties(NULL, pTheme, NULL);
				}

				size_t nCountLayouts = pTheme->m_arLayouts.GetCount();
				for (size_t nIndexL = 0; nIndexL < nCountLayouts; ++nIndexL)
				{
					CLayout* pLayout = &pTheme->m_arLayouts[nIndexL];

					pLayout->m_lOriginalWidth	= m_oInfo.m_lUnitsHor;
					pLayout->m_lOriginalHeight	= m_oInfo.m_lUnitsVer;
					pLayout->m_lWidth			= m_oInfo.m_lMillimetresHor;
					pLayout->m_lHeight			= m_oInfo.m_lMillimetresVer;

					size_t nCountLayoutElements = pLayout->m_arElements.GetCount();
					for (size_t nIndexLayoutEl = 0; nIndexLayoutEl < nCountLayoutElements; ++nIndexLayoutEl)
					{
						IElement* pElement = pLayout->m_arElements[nIndexLayoutEl];

						pElement->m_oMetric = m_oInfo;
						pElement->NormalizeCoords(dScaleX, dScaleY);

						pElement->m_pTheme = pTheme;
						pElement->m_pLayout = NULL;

						CShapeElement* pShape = dynamic_cast<CShapeElement*>(pElement);
						if (!pLayout->m_bUseThemeColorScheme && NULL != pShape)
						{
							LONG lPhType = pElement->m_lPlaceholderType;
							LONG lIndex = 0;
							if (15 == lPhType)
								lIndex = 1;
							else if (0 == lPhType)
								lIndex = 2;
							else if (-1 != lPhType)
								lIndex = 3;

							CTextStyles* pThemeStyles = &pTheme->m_pStyles[lIndex];
							for (int nIndexLevel = 0; nIndexLevel < 10; ++nIndexLevel)
							{
								if (!pThemeStyles->m_pLevels[nIndexLevel].is_init())
									continue;
								if (!pThemeStyles->m_pLevels[nIndexLevel]->m_oCFRun.Color.is_init())
									continue;
								if (pThemeStyles->m_pLevels[nIndexLevel]->m_oCFRun.Color->m_lSchemeIndex == -1)
									continue;

								LONG lIndexSchemeT = pThemeStyles->m_pLevels[nIndexLevel]->m_oCFRun.Color->m_lSchemeIndex;

								if (!pShape->m_oShape.m_oText.m_oStyles.m_pLevels[0].is_init())
									pShape->m_oShape.m_oText.m_oStyles.m_pLevels[0] = new CTextStyleLevel();

								if (!pShape->m_oShape.m_oText.m_oStyles.m_pLevels[0]->m_oCFRun.Color.is_init())
									pShape->m_oShape.m_oText.m_oStyles.m_pLevels[0]->m_oCFRun.Color = new CColor();

								if (pShape->m_oShape.m_oText.m_oStyles.m_pLevels[0]->m_oCFRun.Color->m_lSchemeIndex != -1)
									continue;

								pShape->m_oShape.m_oText.m_oStyles.m_pLevels[0]->m_oCFRun.Color->m_lSchemeIndex = lIndexSchemeT;
								pShape->m_oShape.m_oText.m_oStyles.m_pLevels[0]->m_oCFRun.Color->R = pLayout->m_arColorScheme[lIndexSchemeT].R;
								pShape->m_oShape.m_oText.m_oStyles.m_pLevels[0]->m_oCFRun.Color->G = pLayout->m_arColorScheme[lIndexSchemeT].G;
								pShape->m_oShape.m_oText.m_oStyles.m_pLevels[0]->m_oCFRun.Color->B = pLayout->m_arColorScheme[lIndexSchemeT].B;

								bIsPlaceholderSetUp = true;
							}
						}
					
						pElement->SetupProperties(NULL, pTheme, pLayout);
					}
				}
			}

			size_t nCountSlides = m_arSlides.GetCount();
			for (size_t i = 0; i < nCountSlides; ++i)
			{
				CSlide* pSlide = &m_arSlides[i];

				pSlide->m_lOriginalWidth	= m_oInfo.m_lUnitsHor;
				pSlide->m_lOriginalHeight	= m_oInfo.m_lUnitsVer;
				pSlide->m_lWidth			= m_oInfo.m_lMillimetresHor;
				pSlide->m_lHeight			= m_oInfo.m_lMillimetresVer;

				CTheme* pTheme = NULL;
				if ((0 <= pSlide->m_lThemeID) && (pSlide->m_lThemeID < (LONG)nCountThemes))
					pTheme = &m_arThemes[pSlide->m_lThemeID];

				CLayout* pLayout = NULL;
				if (NULL != pTheme)
				{
					if ((0 <= pSlide->m_lLayoutID) && (pSlide->m_lLayoutID < (LONG)pTheme->m_arLayouts.GetCount()))
						pLayout = &pTheme->m_arLayouts[pSlide->m_lLayoutID];
				}

				size_t nCountElems = pSlide->m_arElements.GetCount();
				for (size_t nIndexEl = 0; nIndexEl < nCountElems; ++nIndexEl)
				{
					IElement* pElement = pSlide->m_arElements[nIndexEl];

					pElement->m_pTheme = pTheme;
					pElement->m_pLayout = pLayout;

					pElement->m_oMetric = m_oInfo;
					//pElement->NormalizeCoords(dScaleX, dScaleY);

					pElement->SetupProperties(pSlide, pTheme, pLayout);
				}

				if (NULL != pLayout && bIsPlaceholderSetUp)
					pSlide->SetUpPlaceholderStyles(pLayout);
			}
		}

		virtual CString SerializeToXml()
		{
			CString strDoc = _T("<Document>");
			for (size_t i = 0; i < m_arSlides.GetCount(); ++i)
			{
				strDoc += m_arSlides[i].SerializeToXml();
			}
			strDoc += _T("</Document>");
			return strDoc;
		}
	};
}