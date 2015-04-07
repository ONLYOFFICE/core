#include "stdafx.h"
#include "Document.h"

void CDrawingDocument::ToXmlSource3()
{
	m_strXmlVideoSource	=	_T("");
	m_strXmlTransforms	=	_T("");
	m_strXmlOverlays	=	_T("");
	CString strVideoOverlay = _T("");
	CString strAudioOverlay = _T("");

	if ( m_arPages.GetCount () > 0 )
	{
		//CString strXmlVideoSource = _T("");
		
		double dDuration = 0.0;
		for (size_t nIndex = 0; nIndex < m_arPages.GetCount(); ++nIndex)
		{
			dDuration += m_arPages[nIndex]->m_dDuration;
		}

		// strXmlVideoSource.Format(_T("<VideoSources><ColorSource Color='16777215' Duration='%lf'/></VideoSources>"),	dDuration);

		m_nWriteSlideTimeOffset	=	0.0;
		m_nWriteSlideTime		=	0.0;

		CString XmlSlideSource	=	_T("");
/*	
		double TimeTransition	=	2000.0;
		for ( size_t nIndex = 0; nIndex < m_arPages.GetCount() - 1; ++nIndex )
		{
			int KeyIndex = m_mapTransitions.FindKey ( (DWORD)nIndex );
			if ( -1 != KeyIndex )
			{
				if ( m_arPages [ nIndex ]->m_dDuration < TimeTransition )
					m_arPages [ nIndex ]->m_dDuration += TimeTransition;
			
				if ( m_arPages [ nIndex + 1 ]->m_dDuration < TimeTransition )
					m_arPages [ nIndex + 1 ]->m_dDuration += TimeTransition;
			}
		}
*/
		for ( size_t nIndex = 0; nIndex < m_arPages.GetCount(); ++nIndex )
		{
			CString XmlCompose	=	_T("");

			bool bInsertCutBlack	=	false;
			bool bInsertCutNone		=	false;

			int KeyIndex = m_mapTransitions.FindKey ( (DWORD)nIndex );
			if ( -1 != KeyIndex )
			{
				ODCSlideShowSlideInfoAtom InfoAtom = m_mapTransitions.GetValueAt ( KeyIndex );

				/*
				if ( 0 == InfoAtom.m_nEffectType )
				{
					bInsertCutNone		=	true;
					if ( 1 == InfoAtom.m_nEffectDirection )
					{
						bInsertCutNone	=	false;
						bInsertCutBlack	=	true;
						//m_strXmlVideoSource	+=	_T("<ColorSource Color=\"0\" Duration=\"500\" />");	
					}

					//continue;
				}
				*/

				XmlCompose		=	GetXmlSlideTransition ( InfoAtom );
			}

			XmlSlideSource.Format ( _T("<ColorSource Color=\"16777215\" Duration=\"%lf\" widthmetric=\"%lf\" heightmetric=\"%lf\">"), 
				m_arPages [ nIndex ]->m_dDuration, (double)m_arPages[nIndex]->m_lWidth, (double)m_arPages[nIndex]->m_lHeight );

			m_strXmlTransforms	=	_T("");
			//strVideoOverlay	=	_T("");
			//strAudioOverlay	=	_T("");

			m_nWriteSlideTime	=	m_arPages [ nIndex ]->m_dDuration;
		
			double dScaleHor	=	(double)(m_arPages[nIndex]->m_lWidth) / m_arPages[nIndex]->m_lOriginalWidth;
			double dScaleVer	=	(double)(m_arPages[nIndex]->m_lHeight) / m_arPages[nIndex]->m_lOriginalHeight;

			// если на слайде есть анимации
			Animations::ODCSlideTimeLine* pTimeLine	=	m_mapAnimations.Lookup ( (DWORD)nIndex );
			
			if ( NULL != pTimeLine )
			{
				WriteAnimationXml ( pTimeLine, m_arPages[nIndex], dScaleHor, dScaleVer );
			}
			else
			{
				for ( int nElem = 0; nElem < m_arPages[nIndex]->m_arElements.GetSize(); ++nElem )
				{
					CString strParam = m_arPages[nIndex]->m_arElements[nElem]->ToXml();

					if (etVideo == m_arPages[nIndex]->m_arElements[nElem]->m_etType)
					{
						strVideoOverlay += strParam;
					}
					else if (etAudio == m_arPages[nIndex]->m_arElements[nElem]->m_etType)
					{
						strAudioOverlay += strParam;
					}
					else
					{
						m_strXmlTransforms += strParam;
					}
				}
			}

			// m_nWriteSlideTimeOffset	+=	m_arPages[nIndex].m_dDuration;

		
			if ( -1 != KeyIndex && 0 == nIndex && XmlCompose.GetLength () > 1 )
			{
				CString strMem = _T("");
				strMem.Format(_T("<ColorSource Color=\"0\" Duration=\"1000\" widthmetric=\"%lf\" heightmetric=\"%lf\" />"),
					(double)m_arPages[nIndex]->m_lWidth, (double)m_arPages[nIndex]->m_lHeight );
				
				m_strXmlVideoSource	+=	strMem;	// 
			}
			
			m_strXmlVideoSource		+=	XmlCompose;
			
			XmlSlideSource			+=	_T("<VideoTransforms>")	+ m_strXmlTransforms	+	_T("</VideoTransforms>");
			//XmlSlideSource			+=	_T("<VideoOverlays>")	+ m_strXmlOverlays		+	_T("</VideoOverlays>");
			//m_strXmlOverlays        +=   _T("<VideoOverlays>")	+ m_strXmlOverlays		+	_T("</VideoOverlays>");
//			m_strXmlOverlays        +=   strSlideOverlay;
			XmlSlideSource			+=	_T("</ColorSource>");
		
			m_strXmlVideoSource		+=	XmlSlideSource;
		}

		m_strXmlVideoSource			=	_T("<VideoSources>") + m_strXmlVideoSource + _T("</VideoSources>");
	}
	
	m_strXmlTransforms				=	_T("");
	//m_strXmlOverlays				=	_T("");

	m_strXmlOverlays        =   _T("<VideoOverlays>")	+ strVideoOverlay +	_T("</VideoOverlays>") + _T("<AudioOverlays>") + 
		_T("<AudioSources>") + strAudioOverlay +	_T("</AudioSources>") +

//_T("<DestinationFormat><AudioFormat><CLSID>{573912EE-8BC2-4099-B598-971635E2F602}</CLSID><Setting><StandardSetting><MediaType type=\"3\">100</MediaType>") +
//_T("<FormatTag type=\"3\">1</FormatTag><SampleRate type=\"3\">44100</SampleRate><Bitrate type=\"3\">1411200</Bitrate><Channels type=\"3\">2</Channels>") +
//_T("<SampleSize type=\"3\">16</SampleSize><BlockAlign type=\"3\">4</BlockAlign><CompressRate type=\"5\">1.000000</CompressRate><ChannelsMask type=\"3\">3</ChannelsMask>") +
//_T("<CodecName type=\"8\">PCM</CodecName><ExtraData type=\"8209\" /></StandardSetting><ExtendedSetting /></Setting></AudioFormat></DestinationFormat>") +

	_T("</AudioOverlays>");

	m_strXmlVideoSource = _T("<SingleSource>") + m_strXmlVideoSource + _T("</SingleSource>");
	m_strXmlVideoSource = _T("<MultiSource>") + m_strXmlVideoSource + m_strXmlOverlays + _T("</MultiSource>");
}

void CDrawingDocument::WriteAnimationXml ( Animations::ODCSlideTimeLine* pTimeLine, CElementsContainer* pSlide, double nScaleW, double nScaleH )
{
	if (NULL == pSlide)
		return;	
	
	for ( int nElemInd = 0; nElemInd < pSlide->m_arElements.GetSize(); ++nElemInd )
	{
		IElement* pSlideElement = pSlide->m_arElements [ nElemInd ];
		if ( NULL != pSlideElement )
		{
			// анимации для видео
			if ( 0 == pSlideElement->m_etType )
			{
				// TODO : к видео так же должны быть применимы анимации ???
				m_strXmlOverlays	+=	pSlideElement->ToXml ();
				continue;
			}

			// анимации для картинок
			if ( 2 == pSlideElement->m_etType )
			{
				CImageElement* pImage	=	static_cast<CImageElement*> ( pSlideElement );
				if ( NULL != pImage )
				{
					AnimationsArray* pSources = pTimeLine->GetAnimation().Lookup ( pSlideElement->m_lID );
					if ( pSources )
					{
						m_strXmlTransforms	+=	GetAnimationImageXml ( static_cast<double> ( pSlide->m_lWidth ), static_cast<double> ( pSlide->m_lHeight ), pImage, pSources );
						continue;
					}
				}
			}

			// анимации для фигур
			if ( 3 == pSlideElement->m_etType ) 
			{
				CShapeElement* pShape	=	static_cast<CShapeElement*> ( pSlideElement );
				if ( NULL != pShape )
				{
					AnimationsArray* pSources = pTimeLine->GetAnimation().Lookup ( pSlideElement->m_lID );
					if ( pSources )
					{
						m_strXmlTransforms	+=	GetAnimationShapeXml ( static_cast<double> ( pSlide->m_lWidth ), static_cast<double> ( pSlide->m_lHeight ), pShape, pSources );
						continue;
					}
				}
			}

			m_strXmlTransforms	+=	pSlideElement->ToXml ();
		}
	}
}

CString CDrawingDocument::GetAnimationImageXml ( double fX, double fY, CImageElement* pImage, AnimationsArray* pSource )
{
	CString Source	=	CString ( _T("") );
		Source.Format (	
				_T("<ImagePaint-DrawImageFromFileAnimate ")
				_T("%s")
				_T(" >")
				_T("%s")
				_T("<timeline type = \"1\"  begin=\"%f\" end=\"%f\" fadein=\"0\" fadeout=\"0\" completeness=\"1.0\"/> ")
				_T("</ImagePaint-DrawImageFromFileAnimate>"),
				GetXmlImageSource ( pImage ),
				GetXmlAnimationSource ( fX, fY, pSource ),
				m_nWriteSlideTimeOffset, m_nWriteSlideTimeOffset + m_nWriteSlideTime );

	return Source;
}

CString CDrawingDocument::GetAnimationShapeXml ( double fX, double fY, CShapeElement* pShape, AnimationsArray* pSource )
{
	CGeomShapeInfo infGeomShape;
	
	infGeomShape.m_dLeft	=	pShape->m_rcBounds.left;
	infGeomShape.m_dTop		=	pShape->m_rcBounds.top;
	infGeomShape.m_dWidth	=	pShape->m_rcBounds.right	-	pShape->m_rcBounds.left;
	infGeomShape.m_dHeight	=	pShape->m_rcBounds.bottom	-	pShape->m_rcBounds.top;

	infGeomShape.m_dLimoX	=	pShape->m_oShape.m_lLimoX;
	infGeomShape.m_dLimoY	=	pShape->m_oShape.m_lLimoY;

	CString Source	=	CString ( _T("") );

	for ( int i = 0; i < pShape->m_oShape.m_pShape->m_oPath.m_arParts.GetSize(); ++i )
	{
		Source	+=	CString ( _T("<ImagePaint-DrawGraphicPathAnimate>") );

		CString FillFormat;
		FillFormat.Format ( _T("<stroke>%s</stroke><fill>%s</fill><widthmm>%d</widthmm><heightmm>%d</heightmm>"), 
							NSAttributes::BoolToString ( pShape->m_oShape.m_pShape->m_oPath.m_arParts[i].m_bStroke ), 
							NSAttributes::BoolToString ( pShape->m_oShape.m_pShape->m_oPath.m_arParts[i].m_bFill ),
							pShape->m_oMetric.m_lMillimetresHor,
							pShape->m_oMetric.m_lMillimetresVer );

		Source	+=	FillFormat;

		if ( pShape->m_oShape.m_pShape->m_oPath.m_arParts[i].m_bFill )
		{
			Source += pShape->m_oShape.m_oBrush.ToString2();
		}

		Source += pShape->m_oShape.m_oPen.ToString2 ();

		for ( int nIndex = 0; nIndex < pShape->m_oShape.m_pShape->m_oPath.m_arParts[i].m_arSlices.GetSize(); ++nIndex )
		{
#if defined(PPTX_DEF)
			Source += pShape->m_oShape.m_pShape->m_oPath.m_arParts[i].m_arSlices [ nIndex ].ToXml ( infGeomShape, pShape->m_oShape.m_pShape->m_oPath.m_arParts[i].width,
				pShape->m_oShape.m_pShape->m_oPath.m_arParts[i].height, NSBaseShape::pptx);
#endif
#if defined(PPT_DEF)
			Source += pShape->m_oShape.m_pShape->m_oPath.m_arParts[i].m_arSlices [ nIndex ].ToXml ( infGeomShape, pShape->m_oShape.m_pShape->m_oPath.m_arParts[i].width,
				pShape->m_oShape.m_pShape->m_oPath.m_arParts[i].height, NSBaseShape::ppt);
#endif
#if defined(ODP_DEF)
			Source += pShape->m_oShape.m_pShape->m_oPath.m_arParts[i].m_arSlices [ nIndex ].ToXml ( infGeomShape, pShape->m_oShape.m_pShape->m_oPath.m_arParts[i].width,
				pShape->m_oShape.m_pShape->m_oPath.m_arParts[i].height, NSBaseShape::odp);
#endif

		}

		CString TimeLine;
		TimeLine.Format ( _T("<timeline type = \"1\"  begin=\"%f\" end=\"%f\" fadein=\"0\" fadeout=\"0\" completeness=\"1.0\"/> "),
						m_nWriteSlideTimeOffset, m_nWriteSlideTimeOffset + m_nWriteSlideTime );

		Source	+=	GetXmlAnimationSource ( fX, fY, pSource );
		Source	+=	TimeLine;
		Source	+=	CString ( _T("</ImagePaint-DrawGraphicPathAnimate>") );
	}

	return Source;
}

CString CDrawingDocument::GetXmlImageSource ( CImageElement* pImage )
{
	CString Source;	
	Source.Format (
		_T("left=\"%d\" top=\"%d\" right=\"%d\" bottom=\"%d\" filepath=\"%s\" ")
		_T("metric=\"0\" backcolor=\"-1\" scaletype=\"-1\" scalecolor=\"255\" "),
		pImage->m_rcBounds.left, pImage->m_rcBounds.top, pImage->m_rcBounds.right, pImage->m_rcBounds.bottom, CString ( CW2A ( pImage->m_strFileName ) ) );

	return Source; 
}

CString CDrawingDocument::GetXmlAnimationSource ( double fX, double fY, AnimationsArray* pSource )
{
	CString XmlAnimationSource;

	// TODO : три эффекта максиум должно быть выполнятся на одном участке
	for ( UINT i = 0; i < pSource->GetCount(); ++i )
	{
		CString XmlEffect; 

		Animations::ODTimeLineElement& pESource = pSource->GetAt(i);

		// MOTION PATH EFFECT
		if ( 0x00000004 == pESource.m_nEffectType )
		{
			Animations::ODMotionPath path;
			if ( path.Create ( pESource.m_MotionPath ) )
			{
				XmlEffect.Format (	_T("<animation_effect>")
										_T("<effect_id>%d</effect_id>")
										_T("<effect_type>%d</effect_type>")
												_T("%s")							//	MOTION PATH
										_T("<time_delay>%f</time_delay>")
										_T("<time_duration>%f</time_duration>")
											_T("<plale_width>%f</plale_width>")
											_T("<plane_height>%f</plane_height>")
									_T("</animation_effect>"), 
									
									pESource.m_nEffectID,
									pESource.m_nEffectType,
									Animations::Serialize::CreateMotionPath ( path, fX, fY ),
									m_nWriteSlideTimeOffset + pESource.m_nTimeDelay,
									pESource.m_nDuration,
									fX, 
									fY );

									XmlAnimationSource	+=	XmlEffect;
			}
		}

		// ENTRANCE										EXIT
		if ( 0x00000001 == pESource.m_nEffectType || 0x00000002 == pESource.m_nEffectType )
		{
			XmlEffect.Format (	_T("<animation_effect>")
									_T("<effect_id>%d</effect_id>")
									_T("<effect_type>%d</effect_type>")
									_T("<effect_dir>%d</effect_dir>")
									_T("<time_delay>%f</time_delay>")
									_T("<time_duration>%f</time_duration>")
									_T("<plale_width>%f</plale_width>")
									_T("<plane_height>%f</plane_height>")
								_T("</animation_effect>"), 
									
								pESource.m_nEffectID,
								pESource.m_nEffectType,
								pESource.m_nEffectDir,
								m_nWriteSlideTimeOffset + pESource.m_nTimeDelay,
								pESource.m_nDuration,
								fX, 
								fY );

								XmlAnimationSource	+=	XmlEffect;
		}
	}	 

	return XmlAnimationSource;
}

CString CDrawingDocument::GetXmlSlideTransition ( ODCSlideShowSlideInfoAtom InfoAtom )
{
	CString Source	=	CString ( _T("") );

	int EffectID	=	1;

	switch ( InfoAtom.m_nEffectType )
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
			if ( 0x00 == InfoAtom.m_nEffectDirection )	//	Vertical	//	Slide Side Top To Bottom
				EffectID	=	1003;
			if ( 0x01 == InfoAtom.m_nEffectDirection )	//	Horizontal	//	Slide Side Left To Righ
				EffectID	=	1001;
		}
		break;
	case  3 : // Checker 
		{
			if ( 0x00 == InfoAtom.m_nEffectDirection )	//	Horizontal	//	WipeCheckerRightToLef
				EffectID	=	2901;
			if ( 0x01 == InfoAtom.m_nEffectDirection )	//	Vertical	//	WipeCheckerTopToBottom
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
			if ( 0x00 == InfoAtom.m_nEffectDirection )	//	Horizontal
				EffectID	=	5331;
			if ( 0x01 == InfoAtom.m_nEffectDirection )	//	Vertical
				EffectID	=	5332;
		}
		break;
	case  9 : // Strips 
		{
			if ( 0x04 == InfoAtom.m_nEffectDirection )	//	Left Up		//	Mosaic Right Bottom
				EffectID	=	110;
			if ( 0x05 == InfoAtom.m_nEffectDirection )	//	Right Up	//	Mosaic Left Bottom
				EffectID	=	111;
			if ( 0x06 == InfoAtom.m_nEffectDirection )	//	Left Down	//	Mosaic Right Top
				EffectID	=	109;
			if ( 0x07 == InfoAtom.m_nEffectDirection )	//	Right Down	//	Mosaic Left Top
				EffectID	=	108;
		}
		break;
	case  10: // Wipe 
		{
			if ( 0x00 == InfoAtom.m_nEffectDirection )	//	Left			//	Reveal, Left
				EffectID	=	5322;
			if ( 0x01 == InfoAtom.m_nEffectDirection )	//	Up				//	Reveal, Up
				EffectID	=	5323;
			if ( 0x02 == InfoAtom.m_nEffectDirection )	//	Right			//	Reveal, Right
				EffectID	=	5321;
			if ( 0x03 == InfoAtom.m_nEffectDirection )	//	Down			//	Reveal, Down
				EffectID	=	5320;
		}
		break;
	case  11: // Box In/Out 
		{
			if ( 0x00 == InfoAtom.m_nEffectDirection )	//	Wipe Center Box Out 
				EffectID	=	702;
			if ( 0x01 == InfoAtom.m_nEffectDirection )	//	Wipe Center Box In
				EffectID	=	701;
		}
		break;
	case  13 :// Split 
		{
			if ( 0x00 == InfoAtom.m_nEffectDirection )	//	Horizontally out	//	Split, Horizontal
				EffectID	=	5324;
#pragma message ("TODO : Split Horizontal In - добавить такой эффект в ImageStudio")	
			if ( 0x01 == InfoAtom.m_nEffectDirection )	//	Horizontally in		//	
				EffectID	=	5324;					
			if ( 0x02 == InfoAtom.m_nEffectDirection )	//	Vertically out		//	Split, Vertical
				EffectID	=	5325;
#pragma message ("TODO : Split Vertical In - добавить такой эффект в ImageStudio")
			if ( 0x03 == InfoAtom.m_nEffectDirection )	//	Vertically in		//	
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
			if ( 0x00 == InfoAtom.m_nEffectDirection )	//	Left			//	Push Side Right To Left
				EffectID	=	1402;
			if ( 0x01 == InfoAtom.m_nEffectDirection )	//	Up				//	Push Side Bottom To Top
				EffectID	=	1404;
			if ( 0x02 == InfoAtom.m_nEffectDirection )	//	Right			//	Push Side Left To Right
				EffectID	=	1401;
			if ( 0x03 == InfoAtom.m_nEffectDirection )	//	Down			//	Push Side Top To Bottom
				EffectID	=	1403;
		}
		break;
	case  21 : // Comb 
		{
			if ( 0x00 == InfoAtom.m_nEffectDirection )	//	Horizontal			//	MosaicStrips
				EffectID	=	103;
#pragma message ("TODO : MosaicStrips Vertical - добавить такой эффект в ImageStudio")
			if ( 0x01 == InfoAtom.m_nEffectDirection )	//	Vertical			//	MosaicStrips
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
			if ( 0x01 == InfoAtom.m_nEffectDirection )	//	ClockBottomReverse
				EffectID	=	3004;
			if ( 0x02 == InfoAtom.m_nEffectDirection )	//	Clock2AnglesClockwise
				EffectID	=	3013;
			if ( 0x03 == InfoAtom.m_nEffectDirection )	//	Clock4AnglesClockwise
				EffectID	=	3017;
			if ( 0x04 == InfoAtom.m_nEffectDirection )	//	Clock6AnglesClockwise
				EffectID	=	3019;
			if ( 0x08 == InfoAtom.m_nEffectDirection )	//	Clock8AnglesClockwise
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

	Source.Format (	_T("<VideoCompose Time = \"%d\" effectid=\"%d\" />"), InfoAtom.m_nTime, EffectID );	 

	return Source;
}

void CDrawingDocument::NormalizeTransitionTime()
{
	int Transition1 = 0;
	int Duration = 0;
	int Transition2 = 0;

	for(int i = 0; i < m_arPages.GetCount(); i++)
	{
		int index = m_mapTransitions.FindKey(i);
		if(index >= 0)
			Transition1 = m_mapTransitions.GetValueAt(index).m_nTime;// .Lookup(i).m_nTime;
		else
			Transition1 = 0;

		Duration = m_arPages[i]->m_dDuration;

		index = m_mapTransitions.FindKey(i+1);
		if(index >= 0)
			Transition2 = m_mapTransitions.GetValueAt(index).m_nTime;//.Lookup(i+1).m_nTime;
		else
			Transition2 = 0;

		if(i == 0)
		{
			if(Transition1 == 0)
				m_arPages[i]->m_dStartTime = 0;
			else
				m_arPages[i]->m_dStartTime = 1000;
		}
		else
			m_arPages[i]->m_dStartTime = m_arPages[i-1]->m_dStartTime + m_arPages[i-1]->m_dDuration;
		m_arPages[i]->m_dDuration = Transition1 + Duration + Transition2;
		m_arPages[i]->m_dEndTime = m_arPages[i]->m_dStartTime + m_arPages[i]->m_dDuration;

		for(int j = 0; j < m_arPages[i]->m_arElements.GetSize(); j++)
		{
			if(m_arPages[i]->m_arElements[j]->m_etType == etAudio)
			{
				if(((CAudioElement*)(m_arPages[i]->m_arElements[j]))->m_bWithVideo == false)
					continue;
			}
			if((m_arPages[i]->m_arElements[j]->m_etType == etVideo) || (m_arPages[i]->m_arElements[j]->m_etType == etAudio))
			{
				double VideoDuration = m_arPages[i]->m_arElements[j]->m_dEndTime - m_arPages[i]->m_arElements[j]->m_dStartTime;
				m_arPages[i]->m_arElements[j]->m_dStartTime = m_arPages[i]->m_dStartTime + Transition1;
				m_arPages[i]->m_arElements[j]->m_dEndTime = min(m_arPages[i]->m_dEndTime - Transition2, m_arPages[i]->m_dStartTime + Transition1 + VideoDuration);
			}
			else
			{
				//m_arPages[i]->m_arElements[j]->m_dStartTime = Transition1;
				m_arPages[i]->m_arElements[j]->m_dEndTime = m_arPages[i]->m_dDuration;
			}
		}
	}
}