#include "ShapeWriter.h"
#include "StylesWriter.h"

#include "../../../ASCOfficeXlsFile2/source/XlsXlsxConverter/ShapeType.h"
#include "../../../Common/MS-LCID.h"

const double EMU_MM = 36000;

void CStylesWriter::ConvertStyleLevel(NSPresentationEditor::CTextStyleLevel& oLevel,	NSPresentationEditor::CMetricInfo& oMetricInfo,
																		NSPresentationEditor::CStringWriter& oWriter, const int& nLevel)
{//дублирование CTextPFRun и  CTextCFRun с ShapeWriter - todooo  - вынести отдельно
	CString str1;
	if (nLevel == 9)
		str1 = _T("<a:defPPr");
	else
		str1.Format(_T("<a:lvl%dpPr"), nLevel + 1);

	oWriter.WriteString(str1);

	NSPresentationEditor::CTextPFRun* pPF = &oLevel.m_oPFRun;

	if (pPF->textDirection.is_init())
	{
		if (pPF->textDirection.get() == 1)	oWriter.WriteString(std::wstring(L" rtl=\"1\""));
		else								oWriter.WriteString(std::wstring(L" rtl=\"0\""));
	}
	if (pPF->fontAlign.is_init())
	{
		CString strProp = GetFontAlign(pPF->fontAlign.get());
		oWriter.WriteString(std::wstring(L" fontAlgn=\"") + string2std_string(strProp) + _T("\""));
	}
	if (pPF->leftMargin.is_init())
	{
		CString strProp;
		strProp.Format(_T(" marL=\"%d\""), pPF->leftMargin.get());
		oWriter.WriteString(strProp);

		if (pPF->indent.is_init() == false)
			pPF->indent = (LONG)0;
	}
	if (pPF->indent.is_init())
	{
		CString strProp;
		strProp.Format(_T(" indent=\"%d\""), pPF->indent.get());
		oWriter.WriteString(strProp);
	}
	if (pPF->textAlignment.is_init())
	{
		CString strProp = GetTextAlign(pPF->textAlignment.get());
		oWriter.WriteString(std::wstring(L" algn=\"") + string2std_string(strProp) + _T("\""));
	}
	if (pPF->defaultTabSize.is_init())
	{
		CString strProp;
		strProp.Format(_T(" defTabSz=\"%d\""), pPF->defaultTabSize.get());
		oWriter.WriteString(strProp);
	}
	CString str2 = _T(">");
	oWriter.WriteString(str2);

	if (pPF->tabStops.size() > 0)
	{
		oWriter.WriteString(std::wstring(L"<a:tabLst>"));
		for (int t = 0 ; t < pPF->tabStops.size(); t++)
		{
			CString strTabPos; 	strTabPos.Format(L"%d", pPF->tabStops[t].first) ;
			oWriter.WriteString(std::wstring(L"<a:tab pos=\"") + string2std_string(strTabPos) + _T("\""));

			if		(pPF->tabStops[t].second == 1)	oWriter.WriteString(std::wstring(L" algn=\"ctr\"/>"));
			else if (pPF->tabStops[t].second == 2)	oWriter.WriteString(std::wstring(L" algn=\"r\"/>"));
			else if (pPF->tabStops[t].second == 3)	oWriter.WriteString(std::wstring(L" algn=\"dec\"/>"));
			else									oWriter.WriteString(std::wstring(L" algn=\"l\"/>"));
		}
		oWriter.WriteString(std::wstring(L"</a:tabLst>"));
	}

	if (pPF->hasBullet.is_init())
	{
		if (pPF->hasBullet.get())
		{
			if (pPF->bulletColor.is_init())
			{
				oWriter.WriteString(std::wstring(L"<a:buClr>"));
				oWriter.WriteString(NSPresentationEditor::CShapeWriter::ConvertColor(pPF->bulletColor.get(), 255));
				oWriter.WriteString(std::wstring(L"</a:buClr>"));
			}	
			if (pPF->bulletSize.is_init())
			{
				if (pPF->bulletSize.get() > 24 && pPF->bulletSize.get() < 401)
				{
					CString str;
					str.Format(_T("<a:buSzPct val=\"%d\"/>"), pPF->bulletSize.get() * 1000 );
					oWriter.WriteString(str);
				}
				if (pPF->bulletSize.get() < 0 && pPF->bulletSize.get() > -4001)
				{
					CString str;
					str.Format(_T("<a:buSzPts val=\"%d\"/>"), - pPF->bulletSize.get() );
					oWriter.WriteString(str);
				}
			}
			if (pPF->bulletFontProperties.is_init())
			{
				oWriter.WriteString(std::wstring(L"<a:buFont typeface=\"") + pPF->bulletFontProperties->strFontName + _T("\"/>"));
			}
			if (pPF->bulletChar.is_init())
			{
				wchar_t bu = pPF->bulletChar.get();
				
				oWriter.WriteString(std::wstring(L"<a:buChar char=\""));
				oWriter.WriteStringXML(std::wstring(&bu, 1));
				oWriter.WriteString(std::wstring(L"\"/>"));	
			}
		}
		else
		{
			CString strB = _T("<a:buNone/>");
			oWriter.WriteString(strB);
		}
	}

	double dKoef1 = 3.52777778;
	if (pPF->lineSpacing.is_init())
	{
		LONG val = pPF->lineSpacing.get();
		if (val > 0)
		{
			CString str = _T("");
			str.Format(_T("<a:lnSpc><a:spcPts val=\"%d\"/></a:lnSpc>"), (int)(val * 0.125 * 100/*/ dKoef1*/));
			oWriter.WriteString(str);
		}
		else if (val < 0 && val > -13200)
		{
			CString str = _T("");
			str.Format(_T("<a:lnSpc><a:spcPct val=\"%d\"/></a:lnSpc>"), -val * 1000);
			oWriter.WriteString(str);
		}
	}
	if (pPF->spaceAfter.is_init())
	{
		LONG val = pPF->spaceAfter.get();
		if (val > 0)
		{
			CString str = _T("");
			str.Format(_T("<a:spcAft><a:spcPts val=\"%d\"/></a:spcAft>"), (int)(val * 0.125 * 100/*/ dKoef1*/));
			oWriter.WriteString(str);
		}
		else if (val < 0 && val > -13200)
		{
			CString str = _T("");
			str.Format(_T("<a:spcAft><a:spcPct val=\"%d\"/></a:spcAft>"), -val * 1000);
			oWriter.WriteString(str);
		}
	}
	if (pPF->spaceBefore.is_init())
	{
		LONG val = pPF->spaceBefore.get();
		if (val > 0)
		{
			CString str = _T("");
			str.Format(_T("<a:spcBef><a:spcPts val=\"%d\"/></a:spcBef>"), (int)(val * 0.125 * 100/*/ dKoef1*/));
			oWriter.WriteString(str);
		}
		else if (val < 0 && val > -13200)
		{
			CString str = _T("");
			str.Format(_T("<a:spcBef><a:spcPct val=\"%d\"/></a:spcBef>"), -val * 1000);
			oWriter.WriteString(str);
		}
	}

	CString strCF1 = _T("<a:defRPr");
	oWriter.WriteString(strCF1);

	NSPresentationEditor::CTextCFRun* pCF = &oLevel.m_oCFRun;
	
	if (pCF->Size.is_init())
	{
		CString strProp = _T("");
		strProp.Format(_T(" sz=\"%d\""), (int)(100 * pCF->Size.get()));
		oWriter.WriteString(strProp);
	}
	if (pCF->FontBold.is_init())
	{
		if (pCF->FontBold.get())
			oWriter.WriteString(std::wstring(L" b=\"1\""));
		else
			oWriter.WriteString(std::wstring(L" b=\"0\""));
	}
	if (pCF->FontItalic.is_init())
	{
		if (pCF->FontItalic.get())
			oWriter.WriteString(std::wstring(L" i=\"1\""));
		else
			oWriter.WriteString(std::wstring(L" i=\"0\""));
	}
	if (pCF->Language.is_init())
	{
		std::wstring str_lang = msLCID2wstring(pCF->Language.get());

		if (str_lang.length() > 0)
			oWriter.WriteString(std::wstring(L" lang=\"") + str_lang + _T("\""));
	}
	oWriter.WriteString(std::wstring(L">"));

	if (pCF->Color.is_init())
	{
		if (pCF->Color->m_lSchemeIndex != -1)
		{
			CString strProp = _T("<a:solidFill><a:schemeClr val=\"") + GetColorInScheme(pCF->Color->m_lSchemeIndex) + _T("\"/></a:solidFill>");
			oWriter.WriteString(strProp);
		}
		else
		{
			CString strColor = _T("");
			strColor.Format(_T("%06x"), pCF->Color->GetLONG_RGB());

			CString strProp = _T("<a:solidFill><a:srgbClr val=\"") + strColor + _T("\"/></a:solidFill>");
			oWriter.WriteString(strProp);
		}
	}
	if (pCF->Typeface.is_init())
	{
		if (0 == pCF->Typeface.get())
		{
			CString strProp = _T("<a:latin typeface=\"+mj-lt\"/>");
			oWriter.WriteString(strProp);
		}
		else
		{
			CString strProp = _T("<a:latin typeface=\"+mn-lt\"/>");
			oWriter.WriteString(strProp);
		}
	}
	else if ((pCF->FontProperties.is_init()) && (!pCF->FontProperties->strFontName.empty()))
	{
		oWriter.WriteString(std::wstring(L"<a:latin typeface=\"") + pCF->FontProperties->strFontName + _T("\"/>"));
	}
	if (pCF->FontPropertiesEA.is_init())
	{
		oWriter.WriteString(std::wstring(L"<a:ea typeface=\"") + pCF->FontPropertiesEA->strFontName + _T("\"/>"));
	}
	if (pCF->FontPropertiesSym.is_init())
	{
		oWriter.WriteString(std::wstring(L"<a:sym typeface=\"") + pCF->FontPropertiesSym->strFontName + _T("\"/>"));
	}
	CString strCF2 = _T("</a:defRPr>");
	oWriter.WriteString(strCF2);		

	CString str3 = _T("");
	if (nLevel == 9)
		str3 = _T("</a:defPPr>");
	else
		str3.Format(_T("</a:lvl%dpPr>"), nLevel + 1);

	oWriter.WriteString(str3);
}
NSPresentationEditor::CShapeWriter::CShapeWriter()
{
	m_pRels			= NULL;
	m_lNextShapeID	= 1000;

	m_bWordArt		= false;
	m_bTextBox		= false;
	
//////////////////////////////////////////////////////////////////////
	m_dDpiX = 96.0;
	m_dDpiY	= 96.0;

	m_lClipMode = c_nClipRegionTypeWinding;

	m_pSimpleGraphicsConverter = new Aggplus::CGraphicsPathSimpleConverter();
	m_pSimpleGraphicsConverter->SetRenderer(this);

	m_dTransformAngle = 0.0;

	m_dWidth	= 100;
	m_dHeight	= 100;

	m_pFontManager = NULL;

	m_pImageElement = NULL;
	m_pShapeElement = NULL;
}
CString	NSPresentationEditor::CShapeWriter::ConvertLine(CPen & pen)
{
	NSPresentationEditor::CStringWriter line_writer;

	CString strLine = _T("");
	strLine.Format(_T("<a:ln w=\"%d\">"), (int)(pen.Size * 36000));
	line_writer.WriteString(strLine);

	line_writer.WriteString(std::wstring(L"<a:solidFill>"));
		line_writer.WriteString(ConvertColor(pen.Color, pen.Alpha));
	line_writer.WriteString(std::wstring(L"</a:solidFill>"));

	line_writer.WriteString(std::wstring(L"<a:round/><a:headEnd/><a:tailEnd/></a:ln>"));

	return line_writer.GetData();
}
CString	NSPresentationEditor::CShapeWriter::ConvertBrush(CBrush & brush)
{
	NSPresentationEditor::CStringWriter brush_writer;
	
	if (brush.Type == c_BrushTypeTexture)
	{
		CString strRid = m_pRels->WriteImage(brush.TexturePath);

		brush_writer.WriteString(std::wstring(L"<a:blipFill dpi=\"0\" rotWithShape=\"1\"><a:blip r:embed=\"") + string2std_string(strRid) + _T("\"/><a:srcRect/>"));
			
		if (	brush.TextureMode == c_BrushTextureModeTile)
			brush_writer.WriteString(std::wstring(L"<a:tile/>"));
		else
			brush_writer.WriteString(std::wstring(L"<a:stretch><a:fillRect/></a:stretch>"));

		brush_writer.WriteString(std::wstring(L"</a:blipFill>"));
	}
	else if (	brush.Type == c_BrushTypeNoFill) 
	{
		brush_writer.WriteString(std::wstring(L"<a:noFill/>"));
	}
	else if (	brush.Type == c_BrushTypePathGradient1	||
				brush.Type == c_BrushTypePathGradient2	||
				brush.Type == c_BrushTypeCenter			||
				brush.Type == c_BrushTypeHorizontal		||
				brush.Type == c_BrushTypeVertical		) 
	{
		brush_writer.WriteString(std::wstring(L"<a:gradFill><a:gsLst>"));
		if (brush.ColorsPosition.empty() == false)
		{
			for (int i = 0; i < brush.ColorsPosition.size(); i++)
			{
				CString strPos; strPos.Format(L"%d", (int)(brush.ColorsPosition[i].second * 1000));
				
				brush_writer.WriteString(std::wstring(L"<a:gs pos=\"") + string2std_string(strPos)+ L"\">");
					brush_writer.WriteString(ConvertColor(brush.ColorsPosition[i].first, 255));
				brush_writer.WriteString(std::wstring(L"</a:gs>"));	
			}
		}
		else
		{
			brush_writer.WriteString(std::wstring(L"<a:gs pos=\"0\">"));
				brush_writer.WriteString(ConvertColor(brush.Color1, brush.Alpha1));
			brush_writer.WriteString(std::wstring(L"</a:gs>"));
			
			brush_writer.WriteString(std::wstring(L"<a:gs pos=\"100000\">"));
				brush_writer.WriteString(ConvertColor(brush.Color2, brush.Alpha2));
			brush_writer.WriteString(std::wstring(L"</a:gs>"));
		}
		brush_writer.WriteString(std::wstring(L"</a:gsLst>"));
		brush_writer.WriteString(std::wstring(L"<a:lin ang=\"5400000\" scaled=\"1\"/>"));
		brush_writer.WriteString(std::wstring(L"</a:gradFill>"));
	}
	else if(brush.Type == c_BrushTypePattern)
	{//типов нету в ппт - вместо них шаблон-картинка
		brush_writer.WriteString(std::wstring(L"<a:pattFill prst=\"pct80\">"));
			brush_writer.WriteString(std::wstring(L"<a:fgClr>"));
				brush_writer.WriteString(ConvertColor(brush.Color1, brush.Alpha1));
			brush_writer.WriteString(std::wstring(L"</a:fgClr>"));
			brush_writer.WriteString(std::wstring(L"<a:bgClr>"));
				brush_writer.WriteString(ConvertColor(brush.Color2, brush.Alpha2));
			brush_writer.WriteString(std::wstring(L"</a:bgClr>"));
		brush_writer.WriteString(std::wstring(L"</a:pattFill>"));
	}
	else
	{
		brush_writer.WriteString(std::wstring(L"<a:solidFill>"));
			brush_writer.WriteString(ConvertColor(brush.Color1, brush.Alpha1));
		brush_writer.WriteString(std::wstring(L"</a:solidFill>"));
	}
	return brush_writer.GetData();
}

CString	NSPresentationEditor::CShapeWriter::ConvertShadow(CShadow	& shadow)
{
	std::wstring	Preset;
	bool			Inner = false;

	if (shadow.Visible == false)		return _T("");

	double dist = sqrt(shadow.DistanceY * shadow.DistanceY  + shadow.DistanceX * shadow.DistanceX);
	double dir	= 0;
	
	if (abs(shadow.DistanceY) > 0)
	{
		dir = 180 * atan(shadow.DistanceX / shadow.DistanceY) / 3.1415926;

		if (dir < 0) dir += 180;

		if (shadow.DistanceX < 0 && shadow.DistanceY < 0) dir *= 10;
	}

	if (shadow.Type == 1) Preset = L"shdw13";

	if (shadow.Type == 5)
	{
		if (shadow.DistanceX < 0 && shadow.DistanceY < 0)	Preset = L"shdw18";
		else												Preset = L"shdw17";
	}
	if (shadow.Type == 2 && shadow.OriginX < 0) 
	{
		if (shadow.OriginX < -1.4)
		{
			if (shadow.ScaleYToY < 0)	Preset = L"shdw15";
			else						Preset = L"shdw11";
		}
		else
		{
			if (shadow.ScaleYToY < 0)	Preset = L"shdw16";
			else						Preset = L"shdw12";
		}
	}

	CString strDir = _T("");
	CString strDist = _T("");

	if (shadow.DistanceY != 0 && shadow.DistanceX != 0)
	{
		if (shadow.DistanceY < 0 && shadow.DistanceX < 0) dir /=2;

		strDir.Format(_T(" dir=\"%d\""), (int)(dir * 60000));
		strDist.Format(_T(" dist=\"%d\""), (int)(dist * 36000));	
	}

	CString strSY = _T("");
	if (shadow.ScaleYToY < 1 || shadow.ScaleYToY > 1)
	{
		if (shadow.ScaleYToX < 1)shadow.ScaleYToY = -shadow.ScaleYToY;

		strSY.Format(_T(" sy=\"%d\""), (int)(shadow.ScaleYToY * 100000));	
	}
	CString strSX = _T("");
	if (shadow.ScaleYToX < 1 || shadow.ScaleYToX > 1)
	{
		//strSX.Format(_T(" sx=\"%d\""), (int)(shadow.ScaleYToX * 100000));	
		strSX.Format(_T(" kx=\"%d\""), (int)((shadow.ScaleYToX + 0.5) * 360000));	
	}
	NSPresentationEditor::CStringWriter shadow_writer;
	
	shadow_writer.WriteString(std::wstring(L"<a:effectLst>"));

	if (!Preset.empty())
	{
		shadow_writer.WriteString(std::wstring(L"<a:prstShdw"));
			shadow_writer.WriteString(std::wstring(L" prst=\"") + Preset + std::wstring(L"\""));
			//shadow_writer.WriteString(std::wstring(L" rotWithShape=\"0\" algn=\"ctr\""));
			shadow_writer.WriteString(strDir);
			shadow_writer.WriteString(strDist);
		shadow_writer.WriteString(std::wstring(L">"));

		shadow_writer.WriteString(ConvertColor(shadow.Color,shadow.Alpha));
		shadow_writer.WriteString(std::wstring(L"</a:prstShdw>"));
	}
	else if (Inner)
	{
		shadow_writer.WriteString(std::wstring(L"<a:innerShdw"));
			shadow_writer.WriteString(std::wstring(L" rotWithShape=\"0\""));
			if (strSX.IsEmpty() && strSY.IsEmpty())
			{
				shadow_writer.WriteString(std::wstring(L" algn=\"ctr\""));
			}
			shadow_writer.WriteString(strSX);
			shadow_writer.WriteString(strSY);
			shadow_writer.WriteString(strDir);
			shadow_writer.WriteString(strDist);
		shadow_writer.WriteString(std::wstring(L">"));

		shadow_writer.WriteString(ConvertColor(shadow.Color,shadow.Alpha));
		shadow_writer.WriteString(std::wstring(L"</a:innerShdw>"));
	}
	else
	{
		shadow_writer.WriteString(std::wstring(L"<a:outerShdw"));
			shadow_writer.WriteString(std::wstring(L" rotWithShape=\"0\""));
			if (strSX.IsEmpty() && strSY.IsEmpty())
			{
				shadow_writer.WriteString(std::wstring(L" algn=\"ctr\""));
			}
			shadow_writer.WriteString(strSX);
			shadow_writer.WriteString(strSY);
			shadow_writer.WriteString(strDir);
			shadow_writer.WriteString(strDist);
		shadow_writer.WriteString(std::wstring(L">"));

		shadow_writer.WriteString(ConvertColor(shadow.Color,shadow.Alpha));
		shadow_writer.WriteString(std::wstring(L"</a:outerShdw>"));
	}
	shadow_writer.WriteString(std::wstring(L"</a:effectLst>"));
	return shadow_writer.GetData();
}

CString  NSPresentationEditor::CShapeWriter::ConvertColor(CColor & color, long alpha)
{
	NSPresentationEditor::CStringWriter color_writer;
	if (color.m_lSchemeIndex == -1)
	{
		if (255 == alpha)
		{
			CString str = _T("");
			str.Format(_T("<a:srgbClr val=\"%06X\"/>"), color.GetLONG_RGB());
			color_writer.WriteString(str);
		}
		else
		{
			CString str = _T("");
			str.Format(_T("<a:srgbClr val=\"%06X\"><a:alpha val=\"%d\"/></a:srgbClr>"), color.GetLONG_RGB(), (int)(alpha * 100000 / 255));
			color_writer.WriteString(str);
		}
	}
	else
	{
		if (255 == alpha)
		{
            CString str = _T("<a:schemeClr val=\"") + CStylesWriter::GetColorInScheme(color.m_lSchemeIndex) + _T("\"/>");
			color_writer.WriteString(str);
		}
		else
		{
            CString strAlpha; strAlpha.Format(_T("%d"), (int)(alpha * 100000 / 255));

            CString str = _T("<a:schemeClr val=\"") + CStylesWriter::GetColorInScheme(color.m_lSchemeIndex) + _T("\"><a:alpha val=\"") + strAlpha + _T("\"/></a:schemeClr>");

			color_writer.WriteString(str);
		}
	}
	return color_writer.GetData();
}
void NSPresentationEditor::CShapeWriter::WriteImageInfo()
{
	m_oWriter.WriteString(std::wstring(L"<p:nvPicPr>"));

	CString strShapeID;
	strShapeID.Format(_T("%d"),	m_lNextShapeID);			
	
	m_oWriter.WriteString(std::wstring(L"<p:cNvPr id=\"") +string2std_string(strShapeID) + L"\"" );
	
	if (m_pImageElement->m_sName.empty()) m_pImageElement->m_sName = std::wstring(L"Image ") +  string2std_string(strShapeID);
	
	m_oWriter.WriteString(std::wstring(L" name=\""));
		m_oWriter.WriteStringXML(m_pImageElement->m_sName);
	m_oWriter.WriteString(std::wstring(L"\""));
	
	//if (!m_pImageElement->m_sDescription.empty())
	//{//бывает всякая разная бяка сохранена 
	//	m_oWriter.WriteString(std::wstring(L" descr=\""));
	//		m_oWriter.WriteStringXML(m_pImageElement->m_sDescription);
	//	m_oWriter.WriteString(std::wstring(L"\""));
	//}

	m_oWriter.WriteString(std::wstring(L"></p:cNvPr><p:cNvPicPr><a:spLocks noGrp=\"1\" noChangeAspect=\"1\"/></p:cNvPicPr>"));

	++m_lNextShapeID;

	if (-1 != m_pImageElement->m_lPlaceholderType)
	{
		m_oWriter.WriteString(std::wstring(L"<p:nvPr><p:ph"));
		if (m_pImageElement->m_lPlaceholderType > 0)
			m_oWriter.WriteString(std::wstring(L" type=\"") + GetPhType(m_pImageElement->m_lPlaceholderType) +_T("\""));
		
		if (-1 != m_pImageElement->m_lPlaceholderID)
		{
            CString strIdx; strIdx.Format(_T("%d"), m_pImageElement->m_lPlaceholderID );
			m_oWriter.WriteString(std::wstring(L" idx=\"") + string2std_string(strIdx) + _T("\""));

		}
		m_oWriter.WriteString(std::wstring(L"/></p:nvPr>"));
	}
	else
	{
		m_oWriter.WriteString(std::wstring(L"<p:nvPr/>"));
	}
	
	CString str2 = _T("</p:nvPicPr>");
	m_oWriter.WriteString(str2);
}

void NSPresentationEditor::CShapeWriter::WriteShapeInfo()
{
	m_oWriter.WriteString(std::wstring(L"<p:nvSpPr>"));

	CString strShapeID = _T("");

	if (m_pShapeElement->m_lID < 0) 
		m_pShapeElement->m_lID = m_lNextShapeID;

	strShapeID.Format(L"%d", m_pShapeElement->m_lID);

	m_oWriter.WriteString(std::wstring(L"<p:cNvPr id=\"") + string2std_string(strShapeID) + L"\"");

	if (m_pShapeElement->m_sName.empty()) m_pShapeElement->m_sName = std::wstring(L"Shape ") +  string2std_string(strShapeID);

	m_oWriter.WriteString(std::wstring(L" name=\""));
		m_oWriter.WriteStringXML(m_pShapeElement->m_sName);
	m_oWriter.WriteString(std::wstring(L"\""));

	if (!m_pShapeElement->m_sDescription.empty())
	{
		m_oWriter.WriteString(std::wstring(L" descr=\""));
			m_oWriter.WriteStringXML(m_pShapeElement->m_sDescription);
		m_oWriter.WriteString(std::wstring(L"\""));
	}
	m_oWriter.WriteString(std::wstring(L">"));
	if (!m_pShapeElement->m_sHyperlink.empty())
	{
		std::wstring rId = (m_pRels->WriteHyperlink(m_pShapeElement->m_sHyperlink)).GetBuffer();

		m_oWriter.WriteString(std::wstring(L"<a:hlinkClick"));
			m_oWriter.WriteString(std::wstring(L" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\""));					
			m_oWriter.WriteString(std::wstring(L" r:id=\"" + rId ));
		m_oWriter.WriteString(std::wstring(L"\"></a:hlinkClick>"));
	}		
	m_oWriter.WriteString(std::wstring(L"</p:cNvPr>"));
	
	m_oWriter.WriteString(std::wstring(L"<p:cNvSpPr"));
		if (m_bTextBox)
			m_oWriter.WriteString(std::wstring(L" txBox=\"1\""));
	m_oWriter.WriteString(std::wstring(L"><a:spLocks noGrp=\"1\" noChangeArrowheads=\"1\"/>"));
	m_oWriter.WriteString(std::wstring(L"</p:cNvSpPr>"));

	++m_lNextShapeID;

	if (-1 != m_pShapeElement->m_lPlaceholderType)
	{
		m_oWriter.WriteString(std::wstring(L"<p:nvPr><p:ph"));
		
		if (m_pShapeElement->m_lPlaceholderType > 0 /*&& !isBodyPlaceholder(m_pShapeElement->m_lPlaceholderType)*/)
			m_oWriter.WriteString(std::wstring(L" type=\"") + GetPhType(m_pShapeElement->m_lPlaceholderType) + _T("\""));
		
		if ( m_pShapeElement->m_lPlaceholderID != -1)
		{
            CString strIdx; strIdx.Format(_T("%d"), m_pShapeElement->m_lPlaceholderID );			 
			m_oWriter.WriteString(std::wstring(L" idx=\"") + string2std_string(strIdx) + _T("\""));
		}
		
		if (m_pShapeElement->m_lPlaceholderSizePreset > 1 && !isTitlePlaceholder(m_pShapeElement->m_lPlaceholderType))
		{
			if (m_pShapeElement->m_lPlaceholderSizePreset == 1)
				m_oWriter.WriteString(std::wstring(L" size=\"half\""));
			if (m_pShapeElement->m_lPlaceholderSizePreset == 2)
				m_oWriter.WriteString(std::wstring(L" size=\"quarter\""));
			if (m_pShapeElement->m_lPlaceholderSizePreset == 3)
			{
				if (isBodyPlaceholder(m_pShapeElement->m_lPlaceholderType))
					m_oWriter.WriteString(std::wstring(L" size=\"half\""));
				else 
					m_oWriter.WriteString(std::wstring(L" size=\"quarter\""));
			}
		}		
		m_oWriter.WriteString(std::wstring(L"/></p:nvPr>"));
	}
	else
	{
		m_oWriter.WriteString(std::wstring(L"<p:nvPr/>"));
	}
	
	CString str2 = _T("</p:nvSpPr>");
	m_oWriter.WriteString(str2);
}
void NSPresentationEditor::CShapeWriter::WriteTextInfo()
{
	size_t nCount = m_pShapeElement->m_oShape.m_oText.m_arParagraphs.size();

	m_oWriter.WriteString(std::wstring(L"<p:txBody>"));

	m_oWriter.WriteString(std::wstring(L"<a:bodyPr" ));

  //		int __l = (int)((m_oBounds.left		+ m_oTextRect.left)	* EMU_MM);
		//int __t = (int)((m_oBounds.top		+ m_oTextRect.top)	* EMU_MM);
		//int __r = (int)((m_oBounds.right	- m_oTextRect.right)* EMU_MM);
		//int __b = (int)((m_oBounds.bottom	- m_oTextRect.bottom)* EMU_MM);

		//if (true)
		//{
		//	CString str;
		//	str.Format(L" lIns=\"%d\" tIns=\"%d\" rIns=\"%d\" bIns=\"%d\"",__l, __t, __r, __b);
		//	m_oWriter.WriteString(str);
		//}
		//else
			m_oWriter.WriteString(std::wstring(L" lIns=\"0\" tIns=\"0\" rIns=\"0\" bIns=\"0\""));

		if (m_pShapeElement->m_oShape.m_oText.m_oAttributes.m_nTextAlignVertical == 0 )			
			m_oWriter.WriteString(" anchor=\"t\"");
		else if (m_pShapeElement->m_oShape.m_oText.m_oAttributes.m_nTextAlignVertical == 2 )	
			m_oWriter.WriteString(" anchor=\"b\"");
		else if (m_pShapeElement->m_oShape.m_oText.m_oAttributes.m_nTextAlignVertical == 1 )	
		{
			m_oWriter.WriteString(" anchor=\"ctr\"");
			m_oWriter.WriteString(" anchorCtr=\"0\"");
		}
		if (m_pShapeElement->m_oShape.m_oText.m_oAttributes.m_dTextRotate > 0)
		{
			CString strProp;
			strProp.Format(_T(" rot=\"%d\""), (int)(m_pShapeElement->m_oShape.m_oText.m_oAttributes.m_dTextRotate * 60000));
			m_oWriter.WriteString(strProp);
		}
		if (m_pShapeElement->m_oShape.m_oText.m_bVertical)
		{
			m_oWriter.WriteString(" vert=\"eaVert\"");
		}
		m_oWriter.WriteString(std::wstring(L">"));

		if (m_bWordArt)
		{
			std::wstring prstTxWarp = oox::Spt2WordArtShapeType((oox::MSOSPT)m_pShapeElement->m_lShapeType);				
			m_oWriter.WriteString(std::wstring(L"<a:prstTxWarp"));
				m_oWriter.WriteString(std::wstring(L" prst=\"") + prstTxWarp + _T("\">"));
				m_oWriter.WriteString(std::wstring(L"<a:avLst>"));//модификаторы

				CPPTShape *pPPTShape = dynamic_cast<CPPTShape *>(m_pShapeElement->m_oShape.m_pShape);
				CString strVal;

				for (int i = 0 ; (pPPTShape) && (i < pPPTShape->m_arAdjustments.size()); i++)
				{
					switch(m_pShapeElement->m_lShapeType)
					{
						case oox::msosptTextFadeUp:
						{
							double kf = 4.63; //"волшебный"
							strVal.Format(L"%d", (int)(kf * pPPTShape->m_arAdjustments[i]));
							m_oWriter.WriteString(std::wstring(L"<a:gd name=\"adj\" fmla=\"val "));
							m_oWriter.WriteString(strVal + _T("\"/>"));
						}break;
					}
					
				}

				m_oWriter.WriteString(std::wstring(L"</a:avLst>"));
			m_oWriter.WriteString(std::wstring(L"</a:prstTxWarp>"));
		}
		if (m_pShapeElement->m_oShape.m_oText.m_bAutoFit)
		{
			m_oWriter.WriteString(std::wstring(L"<a:spAutoFit/>"));
		}
	m_oWriter.WriteString(std::wstring(L"</a:bodyPr>"));
	
	if (0 == nCount)
	{
		m_oWriter.WriteString(std::wstring(L"<a:lstStyle/><a:p><a:endParaRPr dirty=\"0\"/></a:p></p:txBody>"));
		return;
	}
	m_oWriter.WriteString(std::wstring(L"<a:lstStyle>"));

	if (!m_bWordArt)
	{
		CStylesWriter::ConvertStyles(m_pShapeElement->m_oShape.m_oText.m_oStyles, m_oMetricInfo, m_oWriter);
	}

	m_oWriter.WriteString(std::wstring(L"</a:lstStyle>"));

	for (size_t nIndexPar = 0; nIndexPar < nCount; ++nIndexPar)
	{
		NSPresentationEditor::CParagraph* pParagraph = &m_pShapeElement->m_oShape.m_oText.m_arParagraphs[nIndexPar];

		//if (m_bWordArt && nIndexPar == nCount-1)
		//{
		//	if (pParagraph->m_arSpans.size() < 1) break;
		//	if (pParagraph->m_arSpans.size() == 1 && pParagraph->m_arSpans[0].m_strText.empty()) break;
		//}

		CString _str1 = _T("");
		_str1.Format(_T("<a:p><a:pPr lvl=\"%d\""), pParagraph->m_lTextLevel);
		m_oWriter.WriteString(_str1);

		NSPresentationEditor::CTextPFRun* pPF = &pParagraph->m_oPFRun;

		if (pPF->textDirection.is_init())
		{
			if (pPF->textDirection.get() == 1)	m_oWriter.WriteString(std::wstring(L" rtl=\"1\""));
			else								m_oWriter.WriteString(std::wstring(L" rtl=\"0\""));
		}
		if (pPF->fontAlign.is_init())
		{
			CString strProp = CStylesWriter::GetFontAlign(pPF->fontAlign.get());
			m_oWriter.WriteString(std::wstring(L" fontAlgn=\"") + string2std_string(strProp) + _T("\""));
		}
		if (pPF->leftMargin.is_init())
		{
			CString strProp;
			strProp.Format(_T(" marL=\"%d\""), pPF->leftMargin.get());
			m_oWriter.WriteString(strProp);
			if (pPF->indent.is_init() == false)
				pPF->indent = (LONG)0;

		}
		if (pPF->indent.is_init())
		{
			int ind = pPF->indent.get();
			CString strProp = _T("");
			strProp.Format(_T(" indent=\"%d\""), pPF->indent.get());
			m_oWriter.WriteString(strProp);
		}
		if (pPF->textAlignment.is_init())
		{
			CString strProp = CStylesWriter::GetTextAlign(pPF->textAlignment.get());
			m_oWriter.WriteString(std::wstring(L" algn=\"") + string2std_string(strProp) + _T("\""));
		}
		if (pPF->defaultTabSize.is_init())
		{
			CString strProp = _T("");
			strProp.Format(_T(" defTabSz=\"%d\""), pPF->defaultTabSize.get());
			m_oWriter.WriteString(strProp);
		}
		CString _str2 = _T(">");
		m_oWriter.WriteString(_str2);

		double dKoef1 = 3.52777778; // :-) чё это не понятно ...
		if (pPF->lineSpacing.is_init())
		{
			LONG val = pPF->lineSpacing.get();
			//1/1024 master unit or 1/589824 inch.
			//1 inch = 576 master unit -> 1 master unit = 0.125 pt
			if (val > 0)//The absolute value specifies spacing in master units.
			{
				CString str = _T("");
				str.Format(_T("<a:lnSpc><a:spcPts val=\"%d\"/></a:lnSpc>"), (int)(val* 0.125 * 100/*/ dKoef1*/));
				m_oWriter.WriteString(str);
			}
			else if (val < 0 && val > -13200)
			{//0 to 13200, inclusive - The value specifies spacing as a percentage of the text line height.

				CString str = _T("");
				str.Format(_T("<a:lnSpc><a:spcPct val=\"%d\"/></a:lnSpc>"), -val * 1000);
				m_oWriter.WriteString(str);
			}
		}
		if (pPF->spaceAfter.is_init())
		{
			LONG val = pPF->spaceAfter.get();
			if (val > 0)
			{
				CString str = _T("");
				str.Format(_T("<a:spcAft><a:spcPts val=\"%d\"/></a:spcAft>"), (int)(val * 0.125 * 100/*/ dKoef1*/));
				m_oWriter.WriteString(str);
			}
			else if (val < 0 && val > -13200)
			{
				CString str;
				str.Format(_T("<a:spcAft><a:spcPct val=\"%d\"/></a:spcAft>"), -val * 1000);
				m_oWriter.WriteString(str);
			}
		}
		if (pPF->spaceBefore.is_init())
		{
			LONG val = pPF->spaceBefore.get();
			if (val > 0)
			{
				CString str = _T("");
				str.Format(_T("<a:spcBef><a:spcPts val=\"%d\"/></a:spcBef>"), (int)(val * 0.125 * 100/*/ dKoef1*/));
				m_oWriter.WriteString(str);
			}
			else if (val < 0 && val > -13200)
			{
				CString str;
				str.Format(_T("<a:spcBef><a:spcPct val=\"%d\"/></a:spcBef>"), -val * 1000);
				m_oWriter.WriteString(str);
			}
		}

		if (pPF->hasBullet.is_init())
		{
			if (pPF->hasBullet.get())
			{
				if (pPF->bulletColor.is_init())
				{
					m_oWriter.WriteString(std::wstring(L"<a:buClr>"));
						m_oWriter.WriteString(ConvertColor(pPF->bulletColor.get(), 255));
					m_oWriter.WriteString(std::wstring(L"</a:buClr>"));
				}	
				if (pPF->bulletSize.is_init())
				{
					if (pPF->bulletSize.get() > 24 && pPF->bulletSize.get() < 401)
					{
						CString str;
						str.Format(_T("<a:buSzPct val=\"%d\"/>"), pPF->bulletSize.get() * 1000 );
						m_oWriter.WriteString(str);
					}
					if (pPF->bulletSize.get() < 0 && pPF->bulletSize.get() > -4001)
					{
						CString str;
						str.Format(_T("<a:buSzPts val=\"%d\"/>"), - pPF->bulletSize.get() );
						m_oWriter.WriteString(str);
					}
				}
				if (pPF->bulletFontProperties.is_init())
				{
					m_oWriter.WriteString(std::wstring(L"<a:buFont typeface=\"") + pPF->bulletFontProperties->strFontName + _T("\"/>"));
				}

				bool set = true;
				if (pPF->bulletFontProperties.is_init() == false && pPF->bulletSize.is_init() == false)
				{
					m_oWriter.WriteString(std::wstring(L"<a:buFontTx/>"));
					if (pPF->bulletColor.is_init() == false)
						set = false;
				}

				if (pPF->bulletChar.is_init())
				{
					wchar_t bu = pPF->bulletChar.get();
					m_oWriter.WriteString(std::wstring(L"<a:buChar char=\""));
					m_oWriter.WriteStringXML(std::wstring(&bu, 1));
					m_oWriter.WriteString(std::wstring(L"\"/>"));
					set = true;
				}
				
				if (!set && pParagraph->m_lTextLevel == 0)
				{
					wchar_t bu = 0x2022;
					m_oWriter.WriteString(std::wstring(L"<a:buChar char=\""));
					m_oWriter.WriteStringXML(std::wstring(&bu, 1));
					m_oWriter.WriteString(std::wstring(L"\"/>"));
				}
			}
			else
			{
				m_oWriter.WriteString(std::wstring(L"<a:buNone/>"));
			}
		}

		m_oWriter.WriteString(std::wstring(L"</a:pPr>"));

		std::wstring typeRun = L"a:r";

		size_t nCountSpans = pParagraph->m_arSpans.size();
		for (size_t nSpan = 0; nSpan < nCountSpans; ++nSpan)
		{
			if (true)
			{
				if ((nSpan == (nCountSpans - 1)) && (_T("\n") == pParagraph->m_arSpans[nSpan].m_strText || pParagraph->m_arSpans[nSpan].m_strText.empty()) )
				{
					NSPresentationEditor::CTextCFRun* pCF = &pParagraph->m_arSpans[nSpan].m_oRun;
					if ((pCF->Size.is_init()) && (pCF->Size.get() > 0) && (pCF->Size.get() < 4001))
					{
						CString strProp;
						strProp.Format(_T("<a:endParaRPr lang=\"en-US\" sz=\"%d\"/>"), (int)(100 * pCF->Size.get()));
						m_oWriter.WriteString(strProp);
					}
					else
					{
						m_oWriter.WriteString(std::wstring(L"<a:endParaRPr lang=\"en-US\"/>"));
					}
					continue;
				}
			}

			NSPresentationEditor::CTextCFRun* pCF = &pParagraph->m_arSpans[nSpan].m_oRun;
			int span_sz = pParagraph->m_arSpans[nSpan].m_strText.length() ;

			if	((span_sz==1 && ( pParagraph->m_arSpans[nSpan].m_strText[0] == (TCHAR)13 )) ||
				((span_sz==2 && ( pParagraph->m_arSpans[nSpan].m_strText[0] == (TCHAR)13 ) && ( pParagraph->m_arSpans[nSpan].m_strText[1] == (TCHAR)13 ))))
			{
				continue;
			}

			if (pParagraph->m_arSpans[nSpan].m_strText.empty() && !pParagraph->m_arSpans[nSpan].m_bBreak) continue;
			
			if (pParagraph->m_arSpans[nSpan].m_bBreak)
			{
				m_oWriter.WriteString(std::wstring(L"<a:br><a:rPr"));
			}
			else
			{
				if (m_pShapeElement->m_lPlaceholderType == PT_MasterSlideNumber && pParagraph->m_arSpans[nSpan].m_bField)
				{
					m_oWriter.WriteString(std::wstring(L"<a:fld id=\"{D038279B-FC19-497E-A7D1-5ADD9CAF016F}\" type=\"slidenum\"><a:rPr"));
				}
				else if (m_pShapeElement->m_lPlaceholderType == PT_MasterDate && m_pShapeElement->m_nFormatDate == 1 && pParagraph->m_arSpans[nSpan].m_bField)
				{
					m_oWriter.WriteString(std::wstring(L"<a:fld id=\"{D7E01130-044F-4930-9A27-C729C70D8524}\" type=\"datetime1\"><a:rPr"));
				}
				else
				{						
					m_oWriter.WriteString(std::wstring(L"<a:r><a:rPr"));
				}
			}					
			
			if ((pCF->Size.is_init()) && (pCF->Size.get() > 0) && (pCF->Size.get() < 4001))
			{
				CString strProp = _T("");
				strProp.Format(_T(" sz=\"%d\""), (int)(100 * pCF->Size.get()));
				m_oWriter.WriteString(strProp);
			}				
			if (pCF->FontBold.is_init())
			{
				if (pCF->FontBold.get())
					m_oWriter.WriteString(std::wstring(L" b=\"1\""));
				else
					m_oWriter.WriteString(std::wstring(L" b=\"0\""));
			}
			if (pCF->FontItalic.is_init())
			{
				if (pCF->FontItalic.get())
					m_oWriter.WriteString(std::wstring(L" i=\"1\""));
				else
					m_oWriter.WriteString(std::wstring(L" i=\"0\""));
			}

			if (pCF->FontUnderline.is_init())
			{
				if (pCF->FontUnderline.get())
					m_oWriter.WriteString(std::wstring(L" u=\"sng\""));
				else
					m_oWriter.WriteString(std::wstring(L" u=\"none\""));
			}
			if (pCF->Language.is_init())
			{
				std::wstring str_lang = msLCID2wstring(pCF->Language.get());

				if (str_lang.length() > 0)
					m_oWriter.WriteString(std::wstring(L" lang=\"") + str_lang + _T("\""));
			}
			m_oWriter.WriteString(std::wstring(L">"));

			if (m_bWordArt)
			{//порядок важен - линия, заливка, тень !!!			
				if (m_pShapeElement->m_bLine)
				{
					m_oWriter.WriteString(ConvertLine(m_pShapeElement->m_oPen));
				}
				m_oWriter.WriteString(ConvertBrush(m_pShapeElement->m_oBrush));
				m_oWriter.WriteString(ConvertShadow(m_pShapeElement->m_oShadow));
			}
		else
			{
				if (pCF->Color.is_init())
				{
					if (pCF->Color->m_lSchemeIndex != -1)
					{
						CString strProp = _T("<a:solidFill><a:schemeClr val=\"") + CStylesWriter::GetColorInScheme(pCF->Color->m_lSchemeIndex) + _T("\"/></a:solidFill>");
						m_oWriter.WriteString(strProp);
					}
					else
					{
						CString strColor = _T("");
						strColor.Format(_T("%06x"), pCF->Color->GetLONG_RGB());

						CString strProp = _T("<a:solidFill><a:srgbClr val=\"") + strColor + _T("\"/></a:solidFill>");
						m_oWriter.WriteString(strProp);
					}
				}
			}

			if (pCF->Typeface.is_init())
			{
				if (0 == pCF->Typeface.get())
				{
					CString strProp = _T("<a:latin typeface=\"+mj-lt\"/>");
					m_oWriter.WriteString(strProp);
				}
				else
				{
					CString strProp = _T("<a:latin typeface=\"+mn-lt\"/>");
					m_oWriter.WriteString(strProp);
				}
			}
			else if (pCF->FontProperties.is_init())
			{
				m_oWriter.WriteString(std::wstring(L"<a:latin typeface=\"") + pCF->FontProperties->strFontName + _T("\"/>"));
			}
			if (pCF->FontPropertiesEA.is_init())
			{
				m_oWriter.WriteString(std::wstring(L"<a:ea typeface=\"") + pCF->FontPropertiesEA->strFontName + _T("\"/>"));
			}
			if (pCF->FontPropertiesSym.is_init())
			{
				m_oWriter.WriteString(std::wstring(L"<a:sym typeface=\"") + pCF->FontPropertiesSym->strFontName + _T("\"/>"));
			}
			m_oWriter.WriteString(std::wstring(L"</a:rPr>"));

			if (pParagraph->m_arSpans[nSpan].m_bBreak)
			{
				m_oWriter.WriteString(std::wstring(L"</a:br>"));
			}
			else
			{
				CString strT1 = _T("<a:t>");
				m_oWriter.WriteString(strT1);

				std::wstring strT = pParagraph->m_arSpans[nSpan].m_strText;
				NSPresentationEditor::CorrectXmlString(strT);
				m_oWriter.WriteString(strT);

				CString strT2 = _T("</a:t>");
				m_oWriter.WriteString(strT2);
				
				if ((m_pShapeElement->m_lPlaceholderType == PT_MasterSlideNumber || 
					(m_pShapeElement->m_lPlaceholderType == PT_MasterDate && m_pShapeElement->m_nFormatDate == 1)) &&  pParagraph->m_arSpans[nSpan].m_bField)
				{
					m_oWriter.WriteString(std::wstring(L"</a:fld>"));
				}
				else
					m_oWriter.WriteString(std::wstring(L"</a:r>"));
			}
		}

		CString strEndPar = _T("</a:p>");
		m_oWriter.WriteString(strEndPar);
	}

	CString str5 = _T("</p:txBody>");
	m_oWriter.WriteString(str5);
}

CString NSPresentationEditor::CShapeWriter::ConvertShape()
{
	if (m_pImageElement) return ConvertImage();

	if (m_pShapeElement == NULL) return _T("");

	std::wstring prstTxWarp;
	std::wstring prstGeom	= oox::Spt2ShapeType_mini((oox::MSOSPT)m_pShapeElement->m_lShapeType);
//rect, ellipse, line

	if (prstGeom.empty())
	{
		prstTxWarp = oox::Spt2WordArtShapeType((oox::MSOSPT)m_pShapeElement->m_lShapeType);
		if (prstTxWarp.empty() == false)
		{
			m_bWordArt = true;
			m_bTextBox = true;
			prstGeom = L"rect";
			m_oBrush.Type = c_BrushTypeNoFill;
		}
	}
	else
	{
		if (oox::msosptTextBox == (oox::MSOSPT)m_pShapeElement->m_lShapeType)	
			m_bTextBox = true;
		if (oox::msosptLine == (oox::MSOSPT)m_pShapeElement->m_lShapeType)	
			m_pShapeElement->m_bLine = true;
	}

	m_oWriter.WriteString(std::wstring(L"<p:sp>"));

	WriteShapeInfo();

	CGeomShapeInfo oInfo;
	oInfo.SetBounds(m_oBounds);

	oInfo.m_dRotate = m_pShapeElement->m_dRotate;
	oInfo.m_bFlipH	= m_pShapeElement->m_bFlipH;
	oInfo.m_bFlipV	= m_pShapeElement->m_bFlipV;

	oInfo.m_lOriginalWidth	= (LONG)m_pShapeElement->m_rcBoundsOriginal.GetWidth();
	oInfo.m_lOriginalHeight	= (LONG)m_pShapeElement->m_rcBoundsOriginal.GetHeight();

	m_oWriter.WriteString(std::wstring(L"<p:spPr>"));

	if (m_pShapeElement->m_bBoundsEnabled)
	{
		CString str;
		
		m_oWriter.WriteString(std::wstring(L"<a:xfrm"));	
			if (0 != m_pShapeElement->m_dRotate)
			{
				str.Format(L" rot=\"%d\"", (int)(m_pShapeElement->m_dRotate * 60000));
				m_oWriter.WriteString(str);
			}
			if (m_pShapeElement->m_bFlipH)
			{
				m_oWriter.WriteString(std::wstring(L" flipH=\"1\""));
			}
			if (m_pShapeElement->m_bFlipV)
			{
				m_oWriter.WriteString(std::wstring(L" flipV=\"1\""));
			}
		m_oWriter.WriteString(std::wstring(L">"));

		str.Format(_T("<a:off x=\"%d\" y=\"%d\"/>"), (int)m_pShapeElement->m_rcBoundsOriginal.left,	(int)m_pShapeElement->m_rcBoundsOriginal.top);				
		m_oWriter.WriteString(str);

		int width	= m_pShapeElement->m_rcBoundsOriginal.right - m_pShapeElement->m_rcBoundsOriginal.left;
		int height	= m_pShapeElement->m_rcBoundsOriginal.bottom - m_pShapeElement->m_rcBoundsOriginal.top;
		if ( width > 0 || height > 0 )
		{
			str.Format(_T("<a:ext cx=\"%d\" cy=\"%d\"/>"), width, height);
			m_oWriter.WriteString(str);
		}
		m_oWriter.WriteString(std::wstring(L"</a:xfrm>"));
	}

	if (m_pShapeElement->m_oShape.m_lDrawType & c_ShapeDrawType_Graphic)
	{
		m_pShapeElement->m_oShape.ToRenderer(dynamic_cast<IRenderer*>(this), oInfo, m_oMetricInfo, 0.0, 1.0);
	}

	if ((prstGeom.empty() == false || m_pShapeElement->m_bShapePreset) && prstTxWarp.empty())
	{
		if (prstGeom.empty()) prstGeom = L"rect";
		m_oWriter.WriteString(std::wstring(L"<a:prstGeom"));
		{
			m_oWriter.WriteString(std::wstring(L" prst=\"") + prstGeom + std::wstring(L"\">"));
			if (!m_bWordArt)	
			{
				m_oWriter.WriteString(std::wstring(L"<a:avLst/>"));
			}
		}
		m_oWriter.WriteString(std::wstring(L"</a:prstGeom>"));	
	}
	else if (prstTxWarp.empty())
	{
		m_oWriter.WriteString(m_pShapeElement->ConvertPPTShapeToPPTX());
	}
	else
	{
		//word art
		m_oWriter.WriteString(std::wstring(L"<a:prstGeom prst=\"rect\"/>"));
	}

	if (!m_bWordArt)
	{
		m_oWriter.WriteString(ConvertBrush(m_pShapeElement->m_oBrush));
		if (m_pShapeElement->m_bLine)
		{
			m_oWriter.WriteString(ConvertLine(m_pShapeElement->m_oPen));
		}
		m_oWriter.WriteString(ConvertShadow(m_pShapeElement->m_oShadow));
	}

	m_oWriter.WriteString(std::wstring(L"</p:spPr>"));			

	WriteTextInfo();
	m_oWriter.WriteString(std::wstring(L"</p:sp>"));

	m_pShapeElement = NULL;

	return m_oWriter.GetData();
}

CString NSPresentationEditor::CShapeWriter::ConvertImage()
{
	if (m_pImageElement->m_bImagePresent == false)
	{
		if (m_pImageElement->m_sName.empty()) return _T("");
		//ppt_presentation.ppt - ссылка на файл на диске 

		m_pImageElement->m_strImageFileName.clear();
		m_pImageElement->m_bImagePresent = true;
	}
	
	m_oWriter.WriteString(std::wstring(L"<p:pic>"));

	WriteImageInfo();

	CGeomShapeInfo oInfo;
	oInfo.SetBounds(m_oBounds);

	oInfo.m_dRotate = m_pImageElement->m_dRotate;
	oInfo.m_bFlipH	= m_pImageElement->m_bFlipH;
	oInfo.m_bFlipV	= m_pImageElement->m_bFlipV;

	oInfo.m_lOriginalWidth	= (LONG)m_pImageElement->m_rcBoundsOriginal.GetWidth();
	oInfo.m_lOriginalHeight	= (LONG)m_pImageElement->m_rcBoundsOriginal.GetHeight();

	m_oWriter.WriteString(std::wstring(L"<p:blipFill>"));
		CString strRid;
		if (m_pImageElement->m_strImageFileName.empty())
		{
			strRid = m_pRels->WriteHyperlinkImage(CorrectXmlString3(m_pImageElement->m_sImageName));
		}
		else
		{
			strRid = m_pRels->WriteImage(m_pImageElement->m_strImageFileName);
		}

		CString strWrite = _T("<a:blip r:embed=\"") + strRid + _T("\"/>");
		
		m_oWriter.WriteString(strWrite);

		m_oWriter.WriteString(std::wstring(L"<a:srcRect"));
		if (m_pImageElement->m_bCropEnabled)
		{
			CString sCrop;
			sCrop.Format(L" l=\"%d\"", (size_t)(m_pImageElement->m_lcropFromLeft*1.5)); 
			m_oWriter.WriteString(sCrop);
			sCrop.Format(L" t=\"%d\"", (size_t)(m_pImageElement->m_lcropFromTop*1.5)); 
			m_oWriter.WriteString(sCrop);
			sCrop.Format(L" r=\"%d\"", (size_t)(m_pImageElement->m_lcropFromRight*1.5)); 
			m_oWriter.WriteString(sCrop);
			sCrop.Format(L" b=\"%d\"", (size_t)(m_pImageElement->m_lcropFromBottom*1.5)); 
			m_oWriter.WriteString(sCrop);
		}
		m_oWriter.WriteString(std::wstring(L"/>"));
		
		if (m_pImageElement->m_bTile)
		{
			m_oWriter.WriteString(std::wstring(L"<a:tile/>"));
		}
		else if (m_pImageElement->m_bStretch)
		{
			m_oWriter.WriteString(std::wstring(L"<a:stretch><a:fillRect/></a:stretch>"));
		}

	m_oWriter.WriteString(std::wstring(L"</p:blipFill>"));

	m_oWriter.WriteString(std::wstring(L"<p:spPr>"));

	if (m_pImageElement->m_bBoundsEnabled)
	{
		CString str;
		
		m_oWriter.WriteString(std::wstring(L"<a:xfrm"));	
			if (0 != m_pImageElement->m_dRotate)
			{
				str.Format(L" rot=\"%d\"", m_pImageElement->m_dRotate * 60000);
				m_oWriter.WriteString(str);
			}
			if (m_pImageElement->m_bFlipH)
			{
				m_oWriter.WriteString(std::wstring(L" flipH=\"1\""));
			}
			if (m_pImageElement->m_bFlipV)
			{
				m_oWriter.WriteString(std::wstring(L" flipV=\"1\""));
			}
		m_oWriter.WriteString(std::wstring(L">"));

		str.Format(_T("<a:off x=\"%d\" y=\"%d\"/>"), (int)m_pImageElement->m_rcBoundsOriginal.left,	(int)m_pImageElement->m_rcBoundsOriginal.top);				
		m_oWriter.WriteString(str);

		int width	= m_pImageElement->m_rcBoundsOriginal.right - m_pImageElement->m_rcBoundsOriginal.left;
		int height	= m_pImageElement->m_rcBoundsOriginal.bottom - m_pImageElement->m_rcBoundsOriginal.top;
		if ( width > 0 || height > 0 )
		{
			str.Format(_T("<a:ext cx=\"%d\" cy=\"%d\"/>"), width, height);
			m_oWriter.WriteString(str);
		}
		m_oWriter.WriteString(std::wstring(L"</a:xfrm>"));
	}
	m_oWriter.WriteString(std::wstring(L"<a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom>"));

	m_oWriter.WriteString(ConvertBrush(m_pImageElement->m_oBrush));

	if (m_pImageElement->m_bLine)
	{
		m_oWriter.WriteString(ConvertLine(m_pImageElement->m_oPen));
	}

	m_oWriter.WriteString(std::wstring(L"</p:spPr>"));			

	m_oWriter.WriteString(std::wstring(L"</p:pic>"));
	
	m_pImageElement = NULL;
	return m_oWriter.GetData();
}

HRESULT NSPresentationEditor::CShapeWriter::get_Type(LONG* lType)
{
	if (NULL == lType)
        return S_FALSE;

	*lType = c_nSVGConverter;
	return S_OK;
}
//-------- Функции для работы со страницей --------------------------------------------------
HRESULT NSPresentationEditor::CShapeWriter::NewPage()
{
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_Height(double* dHeight)
{
	if (NULL != dHeight)
		*dHeight = m_dHeight;	
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_Height(const double& dHeight)
{
	m_dHeight = dHeight;

	m_oBaseTransform.Reset();
    double dScaleX = 43200 / (std::max)(m_oBounds.GetWidth(), 0.01);
    double dScaleY = 43200 / (std::max)(m_oBounds.GetHeight(), 0.01);
	m_oBaseTransform.Translate(-m_oBounds.left, -m_oBounds.top, Aggplus::MatrixOrderAppend);
	m_oBaseTransform.Scale(dScaleX, dScaleY, Aggplus::MatrixOrderAppend);
	
	CalculateFullTransform();

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_Width(double* dWidth)
{
	if (NULL != dWidth)
		*dWidth = m_dWidth;	
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_Width(const double& dWidth)
{
	m_dWidth = dWidth;

	m_oBaseTransform.Reset();
    double dScaleX = 43200 / (std::max)(m_oBounds.GetWidth(), 0.01);
    double dScaleY = 43200 / (std::max)(m_oBounds.GetHeight(), 0.01);
	m_oBaseTransform.Translate(-m_oBounds.left, -m_oBounds.top, Aggplus::MatrixOrderAppend);
	m_oBaseTransform.Scale(dScaleX, dScaleY, Aggplus::MatrixOrderAppend);

	CalculateFullTransform();

	return S_OK;
}

HRESULT NSPresentationEditor::CShapeWriter::get_DpiX(double* dDpiX)
{
	if (NULL != dDpiX)
		*dDpiX = m_dDpiX;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_DpiY(double* dDpiY)
{
	if (NULL != dDpiY)
		*dDpiY = m_dDpiY;
	return S_OK;
}
// pen --------------------------------------------------------------------------------------
HRESULT NSPresentationEditor::CShapeWriter::SetPen(BSTR bsXML)
{
	//m_oPen.FromXmlString((CString)bsXML);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_PenColor(LONG* lColor)
{
	*lColor = m_oPen.Color.GetLONG();
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_PenColor(const LONG&  lColor)
{
	BYTE lScheme = ((DWORD)(lColor)) >> 24;

	if (0xFF != lScheme)
		m_oPen.Color.SetBGR(lColor);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_PenAlpha(LONG* lAlpha)
{
	*lAlpha = m_oPen.Alpha;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_PenAlpha(const LONG& lAlpha)
{
	m_oPen.Alpha = lAlpha;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_PenSize(double* dSize)
{
	*dSize = m_oPen.Size;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_PenSize(const double& dSize)
{
	m_oPen.Size = dSize * 25.4 / 96.0;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_PenDashStyle(BYTE* val)
{
	*val = m_oPen.DashStyle;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_PenDashStyle(const BYTE& val)
{
	m_oPen.DashStyle = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_PenLineStartCap(BYTE* val)
{
	*val = m_oPen.LineStartCap;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_PenLineStartCap(const BYTE& val)
{
	m_oPen.LineStartCap = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_PenLineEndCap(BYTE* val)
{
	*val = m_oPen.LineEndCap;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_PenLineEndCap(const BYTE& val)
{
	m_oPen.LineEndCap = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_PenLineJoin(BYTE* val)
{
	*val = m_oPen.LineJoin;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_PenLineJoin(const BYTE& val)
{
	m_oPen.LineJoin = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_PenDashOffset(double* val)
{
	*val = m_oPen.DashOffset;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_PenDashOffset(const double&  val)
{
	m_oPen.DashOffset = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_PenAlign(LONG* val)
{
	*val = m_oPen.Align;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_PenAlign(const LONG& val)
{
	m_oPen.Align = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_PenMiterLimit(double* val)
{
	*val = m_oPen.MiterLimit;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_PenMiterLimit(const double& val)
{
	m_oPen.MiterLimit = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::PenDashPattern(double* pPattern, LONG lCount)
{
	if (NULL != pPattern)
	{
		m_oPen.SetDashPattern(pPattern, lCount);
	}

	return S_OK;
}
// brush ------------------------------------------------------------------------------------
HRESULT NSPresentationEditor::CShapeWriter::SetBrush(BSTR bsXML)
{
	//m_oBrush.FromXmlString((CString)bsXML);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_BrushType(LONG* lType)
{
	*lType = m_oBrush.Type;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_BrushType(const LONG& lType)
{
	m_oBrush.Type = lType;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_BrushColor1(LONG* lColor)
{
	*lColor = m_oBrush.Color1.GetLONG();
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_BrushColor1(const LONG& lColor)
{
	BYTE lScheme = ((DWORD)(lColor)) >> 24;

	if (0xFF != lScheme)
		m_oBrush.Color1.SetBGR(lColor);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_BrushAlpha1(LONG* lAlpha)
{
	*lAlpha = m_oBrush.Alpha1;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_BrushAlpha1(const LONG& lAlpha)
{
	m_oBrush.Alpha1 = lAlpha;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_BrushColor2(LONG* lColor)
{
	*lColor = m_oBrush.Color2.GetLONG();
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_BrushColor2(const LONG& lColor)
{
	m_oBrush.Color2.SetBGR(lColor);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_BrushAlpha2(LONG* lAlpha)
{
	*lAlpha = m_oBrush.Alpha2;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_BrushAlpha2(const LONG& lAlpha)
{
	m_oBrush.Alpha2 = lAlpha;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_BrushTexturePath(std::wstring* bsPath)
{
	if (bsPath == NULL) return S_OK;
	*bsPath = m_oBrush.TexturePath;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_BrushTexturePath(const std::wstring& bsPath)
{
	m_oBrush.TexturePath = bsPath;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_BrushTextureMode(LONG* lMode)
{
	*lMode = m_oBrush.TextureMode;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_BrushTextureMode(const LONG& lMode)
{
	m_oBrush.TextureMode = lMode;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_BrushTextureAlpha(LONG* lTxAlpha)
{
	*lTxAlpha = m_oBrush.TextureAlpha;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_BrushTextureAlpha(const LONG& lTxAlpha)
{
	m_oBrush.TextureAlpha = lTxAlpha;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_BrushLinearAngle(double* dAngle)
{
	*dAngle = m_oBrush.LinearAngle;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_BrushLinearAngle(const double& dAngle)
{
	m_oBrush.LinearAngle = dAngle;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::BrushRect(const INT& val, const double& left, const double& top, const double& width, const double& height)
{
	m_oBrush.Rectable = val;
	m_oBrush.Rect.X = (float)left;
	m_oBrush.Rect.Y = (float)top;
	m_oBrush.Rect.Width  = (float)width;
	m_oBrush.Rect.Height = (float)height;	

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::BrushBounds(const double& left, const double& top, const double& width, const double& height)
{
	return S_OK;
}

HRESULT NSPresentationEditor::CShapeWriter::put_BrushGradientColors(LONG* lColors, double* pPositions, LONG nCount)
{

	return S_OK;
}
// font -------------------------------------------------------------------------------------
HRESULT NSPresentationEditor::CShapeWriter::SetFont(BSTR bsXML)
{
	//m_oFont.FromXmlString((CString)bsXML);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_FontName(std::wstring* bsName)
{
	if (bsName == NULL) return S_OK;
	*bsName = m_oFont.Name;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_FontName(const std::wstring& bsName)
{
	m_oFont.Name = bsName;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_FontPath(std::wstring* bsName)
{
	if (bsName == NULL) return S_OK;
	*bsName = m_oFont.Path;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_FontPath(const std::wstring& bsName)
{
	m_oFont.Path = std_string2string(bsName);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_FontSize(double* dSize)
{
	*dSize = m_oFont.Size;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_FontSize(const double& dSize)
{
	m_oFont.Size = dSize;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_FontStyle(LONG* lStyle)
{
	*lStyle = m_oFont.GetStyle();
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_FontStyle(const LONG& lStyle)
{
	m_oFont.SetStyle(lStyle);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_FontStringGID(INT* bGID)
{
	*bGID = m_oFont.StringGID;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_FontStringGID(const INT& bGID)
{
	m_oFont.StringGID = bGID;
	m_pFontManager->SetStringGID(bGID);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_FontCharSpace(double* dSpace)
{
	*dSpace = m_oFont.CharSpace;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_FontCharSpace(const double& dSpace)
{
	m_oFont.CharSpace = dSpace;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_FontFaceIndex(int* lFaceIndex)
{
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_FontFaceIndex(const int& lFaceIndex)
{
	return S_OK;
}
// shadow -----------------------------------------------------------------------------------
HRESULT NSPresentationEditor::CShapeWriter::SetShadow(BSTR bsXML)
{
	//m_oShadow.FromXmlString((CString)bsXML);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_ShadowDistanceX(double* val)
{
	*val = m_oShadow.DistanceX;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_ShadowDistanceX(double val)
{
	m_oShadow.DistanceX = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_ShadowDistanceY(double* val)
{
	*val = m_oShadow.DistanceY;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_ShadowDistanceY(double val)
{
	m_oShadow.DistanceY = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_ShadowBlurSize(double* val)
{
	*val = m_oShadow.BlurSize;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_ShadowBlurSize(double val)
{
	m_oShadow.BlurSize = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_ShadowColor(LONG* val)
{
	*val = m_oShadow.Color.GetLONG();
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_ShadowColor(LONG val)
{
	m_oShadow.Color.SetBGR(val);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_ShadowAlpha(LONG* val)
{
	*val = m_oShadow.Alpha;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_ShadowAlpha(LONG val)
{
	m_oShadow.Alpha = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_ShadowVisible(bool* val)
{
	*val = m_oShadow.Visible;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_ShadowVisible(bool val)
{
	m_oShadow.Visible = val;
	return S_OK;
}
// edge -------------------------------------------------------------------------------------
HRESULT NSPresentationEditor::CShapeWriter::SetEdgeText(BSTR bsXML)
{
	//m_oEdge.FromXmlString((CString)bsXML);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_EdgeVisible(LONG* val)
{
	*val = m_oEdge.Visible;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_EdgeVisible(LONG val)
{
	m_oEdge.Visible = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_EdgeColor(LONG* val)
{
	*val = m_oEdge.Color.GetLONG();
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_EdgeColor(LONG val)
{
	m_oEdge.Color.SetBGR(val);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_EdgeAlpha(LONG* val)
{
	*val = m_oEdge.Alpha;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_EdgeAlpha(LONG val)
{
	m_oEdge.Alpha = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_EdgeDist(double* val)
{
	*val = m_oEdge.Dist;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_EdgeDist(double val)
{
	m_oEdge.Dist = val;
	return S_OK;
}
//-------- Функции для вывода текста --------------------------------------------------------
HRESULT NSPresentationEditor::CShapeWriter::CommandDrawText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h)
{
	if (c_nHyperlinkType == m_lCurrentCommandType)
		return S_OK;

    if (true)
	{
		PathCommandEnd();
		BeginCommand(c_nPathType);
		PathCommandText(bsText, x, y, w, h);
		DrawPath(c_nWindingFillMode);
		EndCommand(c_nPathType);
		PathCommandEnd();
		return S_OK;
	}

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::CommandDrawTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h)
{

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::CommandDrawTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h)
{

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::PathCommandTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h)
{

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::PathCommandTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h)
{

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::CommandLong(const LONG& lType, const LONG& lCommand)
{

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::CommandDouble(const LONG& lType, const double& dCommand)
{

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::CommandString(const LONG& lType, const std::wstring& sCommand)
{

	return S_OK;
}

HRESULT NSPresentationEditor::CShapeWriter::CommandDrawTextEx(const std::wstring& bsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h)
{
    if (true)
	{
		PathCommandEnd();
		BeginCommand(c_nPathType);
		PathCommandTextEx(bsUnicodeText, pGids, nGidsCount ,x, y, w, h);
		DrawPath(c_nWindingFillMode);
		EndCommand(c_nPathType);
		PathCommandEnd();
		return S_OK;
	}
	return S_OK;
}
//-------- Маркеры для команд ---------------------------------------------------------------
HRESULT NSPresentationEditor::CShapeWriter::BeginCommand(const DWORD& lType)
{
	if (c_nPathType == lType)
	{
		m_pSimpleGraphicsConverter->PathCommandEnd();
	}

	m_lCurrentCommandType = lType;

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::EndCommand(const DWORD& lType)
{
	m_lCurrentCommandType = -1;
	return S_OK;
}
//-------- Функции для работы с Graphics Path -----------------------------------------------
HRESULT NSPresentationEditor::CShapeWriter::PathCommandMoveTo(const double& x, const double& y)
{
	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		MoveTo(x, y);
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandMoveTo(x, y);
	}

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::PathCommandLineTo(const double& x, const double& y)
{
	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		LineTo(x, y);
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandLineTo(x, y);
	}

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::PathCommandLinesTo(double* points, const int& count)
{
	m_pSimpleGraphicsConverter->PathCommandLinesTo(points, count);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::PathCommandCurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3)
{
	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		CurveTo(x1, y1, x2, y2, x3, y3);
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandCurveTo(x1, y1, x2, y2, x3, y3);
	}

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::PathCommandCurvesTo(double* points, const int& count)
{
	m_pSimpleGraphicsConverter->PathCommandCurvesTo(points, count);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::PathCommandArcTo(const double& x, const double& y, const double& w, const double& h, const double& startAngle, const double& sweepAngle)
{
	m_pSimpleGraphicsConverter->PathCommandArcTo(x, y, w, h, startAngle, sweepAngle);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::PathCommandClose()
{
	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		Close();
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandClose();
	}

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::PathCommandEnd()
{
	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		End();
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandEnd();
	}

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::DrawPath(const LONG& nType)
{
	bool bIsStroke	= ((0x01 == (nType & 0x01)) && (0 != m_oPen.Alpha));
	bool bIsFill	= ((0xFF < nType) && (0 != m_oBrush.Alpha1));

	if (bIsStroke && bIsFill)
	{
		CString str = _T("");
		str.Format(_T("<a:path w=\"43200\" h=\"43200\">"));
		m_oWriterVML.WriteString(str);
		m_oWriterVML.Write(m_oWriterPath);
		m_oWriterVML.WriteString(std::wstring(L"</a:path>"));
	}
	else if (bIsStroke)
	{
		CString str = _T("");
		str.Format(_T("<a:path w=\"43200\" h=\"43200\" fill=\"none\">"));
		m_oWriterVML.WriteString(str);
		m_oWriterVML.Write(m_oWriterPath);
		m_oWriterVML.WriteString(std::wstring(L"</a:path>"));
	}
	else if (bIsFill)
	{
		CString str = _T("");
		str.Format(_T("<a:path w=\"43200\" h=\"43200\" stroke=\"0\">"));
		m_oWriterVML.WriteString(str);
		m_oWriterVML.Write(m_oWriterPath);
		m_oWriterVML.WriteString(std::wstring(L"</a:path>"));
	}

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::PathCommandStart()
{
	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		Start();
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandEnd();
		m_pSimpleGraphicsConverter->PathCommandStart();
	}

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::PathCommandGetCurrentPoint(double* fX, double* fY)
{
	m_pSimpleGraphicsConverter->PathCommandGetCurrentPoint(fX, fY);	
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::PathCommandText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h)
{
	_SetFont();
	m_pSimpleGraphicsConverter->PathCommandText(bsText, m_pFontManager, x, y, w, h, 0);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::PathCommandTextEx(const std::wstring& bsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h)
{
    bool bGid = m_oFont.StringGID;
	//TODOO
	//if (NULL != bsGidText)
	//{
 //       m_oFont.StringGID = true;
	//	PathCommandText(bsUnicodeText, x, y, w, h, baselineOffset);
	//}
	
    m_oFont.StringGID = false;
	PathCommandText(bsUnicodeText, x, y, w, h);

	m_oFont.StringGID = bGid;
	return S_OK;
}
//-------- Функции для вывода изображений ---------------------------------------------------
HRESULT NSPresentationEditor::CShapeWriter::DrawImage(IGrObject* pImage, const double& x, const double& y, const double& w, const double& h)
{
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::DrawImageFromFile(const std::wstring&, const double& x, const double& y, const double& w, const double& h, const BYTE& lAlpha)
{
	return S_OK;
}
// transform --------------------------------------------------------------------------------
HRESULT NSPresentationEditor::CShapeWriter::GetCommandParams(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
{
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
{
	if ((dWidth <= 1) || (dHeight <= 1))
		lFlags = 0;

    bool bFlipX = (0 != (c_nParamFlipX & lFlags));
    bool bFlipY = (0 != (c_nParamFlipY & lFlags));
	
	double m11 = bFlipX ? -1.0 : 1.0;
	double m22 = bFlipY ? -1.0 : 1.0;
	
	Aggplus::CMatrix oMatrix(1, 0, 0, 1, 0, 0);

	if ((0 != dAngle) || (0 != lFlags))
	{
		double dCentreX = (dLeft + dWidth / 2.0);
		double dCentreY = (dTop + dHeight / 2.0);
		
		oMatrix.Translate(-dCentreX, -dCentreY	, Aggplus::MatrixOrderAppend);
		
		oMatrix.Rotate(dAngle					, Aggplus::MatrixOrderAppend);
		oMatrix.Scale(m11, m22					, Aggplus::MatrixOrderAppend);
		
		oMatrix.Translate(dCentreX, dCentreY	, Aggplus::MatrixOrderAppend);
	}

	m_oTransform = oMatrix;
	CalculateFullTransform();

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::SetTransform(const double& m1, const double& m2, const double& m3, const double& m4, const double& m5, const double& m6)
{
	Aggplus::CMatrix oTrans(m1, m2, m3, m4, m5, m6);
	m_oTransform = oTrans;

	CalculateFullTransform();

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
{
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::ResetTransform(void)
{
	m_oTransform.Reset();
	CalculateFullTransform();
	return S_OK;
}
// -----------------------------------------------------------------------------------------
HRESULT NSPresentationEditor::CShapeWriter::get_ClipMode(LONG* plMode)
{
	*plMode = m_lClipMode;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_ClipMode(const LONG& lMode)
{
	m_lClipMode = lMode;
	return S_OK;
}
// additiaonal params ----------------------------------------------------------------------
HRESULT NSPresentationEditor::CShapeWriter::SetAdditionalParam(BSTR ParamName, VARIANT ParamValue)
{
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::GetAdditionalParam(BSTR ParamName, VARIANT* ParamValue)
{
	return S_OK;
}
// --------------------------------------------------------------------------------------------
