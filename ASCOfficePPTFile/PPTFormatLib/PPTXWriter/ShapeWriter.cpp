/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "ShapeWriter.h"
#include "StylesWriter.h"
#include "../../../ASCOfficePPTXFile/Editor/Drawing/Theme.h"

#include "../../../ASCOfficeXlsFile2/source/XlsXlsxConverter/ShapeType.h"
#include "../../../Common/MS-LCID.h"

#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/SpTreeElem.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Shape.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/SpTree.h"

#include <iostream>

CStylesWriter::CStylesWriter() : m_pTheme(NULL) {}
CStylesWriter::CStylesWriter(PPT_FORMAT::CTheme* pTheme) : m_pTheme(pTheme) {}

void CStylesWriter::ConvertStyleLevel(PPT_FORMAT::CTextStyleLevel& oLevel, PPT_FORMAT::CStringWriter& oWriter, const int& nLevel)
{//дублирование CTextPFRun и  CTextCFRun с ShapeWriter - todooo  - вынести отдельно
    std::wstring str1;
	if (nLevel == 9)
		str1 = _T("<a:defPPr");
	else
	{
		str1 = L"<a:lvl" + std::to_wstring(nLevel + 1) + L"pPr";
	}

	oWriter.WriteString(str1);

	PPT_FORMAT::CTextPFRun* pPF = &oLevel.m_oPFRun;

	if (pPF->textDirection.is_init())
	{
		if (pPF->textDirection.get() == 1)	oWriter.WriteString(L" rtl=\"1\"");
		else								oWriter.WriteString(L" rtl=\"0\"");
	}
	if (pPF->fontAlign.is_init())
	{
		std::wstring strProp = GetFontAlign(pPF->fontAlign.get());
		oWriter.WriteString(L" fontAlgn=\"" + strProp + L"\"");
	}

	int leftMargin = 0;
	if (pPF->leftMargin.is_init())
	{
		leftMargin = pPF->leftMargin.get();
		std::wstring strProp = std::to_wstring(leftMargin);
		oWriter.WriteString(L" marL=\"" + strProp + L"\"");
	}
	if (pPF->indent.is_init())
	{
		std::wstring strProp = std::to_wstring(pPF->indent.get() - leftMargin);
		oWriter.WriteString(L" indent=\"" + strProp + L"\"");
	}
	if (pPF->textAlignment.is_init())
	{
		std::wstring strProp = GetTextAlign(pPF->textAlignment.get());
		oWriter.WriteString(L" algn=\"" + strProp + L"\"");
	}
	if (pPF->defaultTabSize.is_init())
	{
        std::wstring strProp = std::to_wstring(pPF->defaultTabSize.get());
		oWriter.WriteString(L" defTabSz=\"" + strProp + L"\"");
	}
	oWriter.WriteString(L">");

	if (pPF->tabStops.size() > 0)
	{
		oWriter.WriteString(L"<a:tabLst>");
		for (size_t t = 0 ; t < pPF->tabStops.size(); t++)
		{
			std::wstring strTabPos = std::to_wstring(pPF->tabStops[t].first);
			oWriter.WriteString(L"<a:tab pos=\"" + strTabPos + L"\"");

			if		(pPF->tabStops[t].second == 1)	oWriter.WriteString(L" algn=\"ctr\"/>");
			else if (pPF->tabStops[t].second == 2)	oWriter.WriteString(L" algn=\"r\"/>");
			else if (pPF->tabStops[t].second == 3)	oWriter.WriteString(L" algn=\"dec\"/>");
			else									oWriter.WriteString(L" algn=\"l\"/>");
		}
		oWriter.WriteString(L"</a:tabLst>");
	}

	if (pPF->hasBullet.is_init())
	{
		if (pPF->hasBullet.get())
		{
			if (pPF->bulletColor.is_init())
			{
				oWriter.WriteString(L"<a:buClr>");
				oWriter.WriteString(PPT_FORMAT::CShapeWriter::ConvertColor(pPF->bulletColor.get(), 255));
				oWriter.WriteString(L"</a:buClr>");
			}	
			if (pPF->bulletSize.is_init())
			{
				if (pPF->bulletSize.get() > 24 && pPF->bulletSize.get() < 401)
				{
					std::wstring strProp = std::to_wstring(pPF->bulletSize.get() * 1000 );
					oWriter.WriteString(L"<a:buSzPct val=\"" + strProp + L"\"/>");
				}
				if (pPF->bulletSize.get() < 0 && pPF->bulletSize.get() > -4001)
				{
					std::wstring strProp = std::to_wstring(- pPF->bulletSize.get() );
					oWriter.WriteString(L"<a:buSzPts val=\"" + strProp + L"\"/>");
				}
			}
			if (pPF->bulletFontProperties.is_init())
			{
				oWriter.WriteString(L"<a:buFont typeface=\"" + pPF->bulletFontProperties->Name + L"\"");

				if ( pPF->bulletFontProperties->PitchFamily > 0)
				{
					oWriter.WriteString(std::wstring(L" pitchFamily=\"") + std::to_wstring(pPF->bulletFontProperties->PitchFamily) + L"\"");
				}
				if ( pPF->bulletFontProperties->Charset > 0)
				{
					oWriter.WriteString(std::wstring(L" charset=\"") + std::to_wstring((char)pPF->bulletFontProperties->Charset) + L"\"");
				}
				oWriter.WriteString(std::wstring(L"/>"));

			}
			if (pPF->bulletChar.is_init())
			{
				wchar_t bu = pPF->bulletChar.get();
				
				oWriter.WriteString(L"<a:buChar char=\"");
				oWriter.WriteStringXML(std::wstring(&bu, 1));
				oWriter.WriteString(L"\"/>");	
			}
		}
		else
		{
			oWriter.WriteString(L"<a:buNone/>");
		}
	}

	double dKoef1 = 3.52777778;
	if (pPF->lineSpacing.is_init())
	{
        LONG val = pPF->lineSpacing.get();
		if (val > 0)
		{
			std::wstring str = std::to_wstring( (int)(val * 0.125 * 100/*/ dKoef1*/));
			oWriter.WriteString(L"<a:lnSpc><a:spcPts val=\"" + str + L"\"/></a:lnSpc>");
		}
		else if (val < 0 && val > -13200)
		{
			std::wstring str = std::to_wstring(-val * 1000);
			oWriter.WriteString(L"<a:lnSpc><a:spcPct val=\"" + str + L"\"/></a:lnSpc>");
		}
	}
	if (pPF->spaceAfter.is_init())
	{
        LONG val = pPF->spaceAfter.get();
		if (val > 0)
		{
			std::wstring str = std::to_wstring((int)(val * 0.125 * 100/*/ dKoef1*/));
			oWriter.WriteString(L"<a:spcAft><a:spcPts val=\"" + str + L"\"/></a:spcAft>");
		}
		else if (val < 0 && val > -13200)
		{
			std::wstring str = std::to_wstring(-val * 1000);
			oWriter.WriteString(L"<a:spcAft><a:spcPct val=\"" + str + L"\"/></a:spcAft>");
		}
	}
	if (pPF->spaceBefore.is_init())
	{
        LONG val = pPF->spaceBefore.get();
		if (val > 0)
		{
			std::wstring str = std::to_wstring((int)(val * 0.125 * 100/*/ dKoef1*/));
			oWriter.WriteString(L"<a:spcBef><a:spcPts val=\"" + str + L"\"/></a:spcBef>");
		}
		else if (val < 0 && val > -13200)
		{
			std::wstring str = std::to_wstring(-val * 1000);
			oWriter.WriteString(L"<a:spcBef><a:spcPct val=\"" + str + L"\"/></a:spcBef>");
		}
	}

	oWriter.WriteString(L"<a:defRPr");

	PPT_FORMAT::CTextCFRun* pCF = &oLevel.m_oCFRun;
	
	if (pCF->Size.is_init())
	{
		std::wstring str = std::to_wstring((int)(100 * pCF->Size.get()));
		oWriter.WriteString(L" sz=\"" + str + L"\"");
	}
	if (pCF->FontBold.is_init())
	{
		if (pCF->FontBold.get())
			oWriter.WriteString(L" b=\"1\"");
		else
			oWriter.WriteString(L" b=\"0\"");
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
			oWriter.WriteString(L"<a:solidFill><a:schemeClr val=\"" + GetColorInScheme(pCF->Color->m_lSchemeIndex) + L"\"/></a:solidFill>");
		}
		else
		{
            std::wstring strColor = XmlUtils::IntToString(pCF->Color->GetLONG_RGB(), L"%06x");

			oWriter.WriteString(L"<a:solidFill><a:srgbClr val=\"" + strColor + L"\"/></a:solidFill>");
		}
	}
	if ((pCF->font.ansi.is_init()) && (!pCF->font.ansi->Name.empty()))
	{
		oWriter.WriteString(L"<a:latin typeface=\"" + pCF->font.ansi->Name + L"\"/>");
	}
	else if (pCF->fontRef.is_init())
	{
		if ((m_pTheme) && (pCF->fontRef.get() < m_pTheme->m_arFonts.size()))
		{
			oWriter.WriteString(L"<a:latin typeface=\"" + m_pTheme->m_arFonts[pCF->fontRef.get()].Name + L"\"/>");
		}
		else
		{
			if (0 == pCF->fontRef.get())
			{
				oWriter.WriteString(L"<a:latin typeface=\"+mj-lt\"/>");
			}
			else
			{
				oWriter.WriteString(L"<a:latin typeface=\"+mn-lt\"/>");
			}
		}
	}
	if (pCF->font.ea.is_init())
	{
		oWriter.WriteString(L"<a:ea typeface=\"" + pCF->font.ea->Name + L"\"/>");
	}
	if (pCF->font.sym.is_init())
	{
		oWriter.WriteString(L"<a:sym typeface=\"" + pCF->font.sym->Name + L"\"/>");
	}
	oWriter.WriteString(L"</a:defRPr>");		

    std::wstring str3;
	if (nLevel == 9)
		str3 = L"</a:defPPr>";
	else
		str3 = L"</a:lvl" + std::to_wstring(nLevel + 1) + L"pPr>";

	oWriter.WriteString(str3);
}
PPT_FORMAT::CShapeWriter::CShapeWriter()
{
	m_pTheme		= NULL;
	m_pRels			= NULL;
	m_lNextShapeID	= 1000;

	m_bWordArt		= false;
	m_bTextBox		= false;
	
//////////////////////////////////////////////////////////////////////
	m_dDpiX = 96.0;
	m_dDpiY	= 96.0;

	m_lClipMode = c_nClipRegionTypeWinding;

	m_pSimpleGraphicsConverter = new Aggplus::CGraphicsPathSimpleConverter();
	//m_pSimpleGraphicsConverter->SetRenderer(this);

	m_dTransformAngle = 0.0;

	m_pFontManager = NULL;
}
bool PPT_FORMAT::CShapeWriter::SetElement(CElementPtr pElem)
{
	m_pElement = pElem;
	
	m_pSimpleGraphicsConverter->PathCommandEnd();

	m_bWordArt		= false;
	m_bTextBox		= false;

	m_xmlGeomAlternative.clear();
	m_xmlTxBodyAlternative.clear();
	m_xmlAlternative.clear();

	m_oWriter.ClearNoAttack();
	m_oWriterPath.ClearNoAttack();
	m_oWriterVML.ClearNoAttack();	

	return (m_pElement != NULL);
}
std::wstring PPT_FORMAT::CShapeWriter::ConvertLine(CPen & pen)
{
	PPT_FORMAT::CStringWriter line_writer;

	std::wstring strL;
	switch(pen.LineStyle)
	{
		case 1: strL = L"  cmpd=\"dbl\"";		break;
		case 2: strL = L"  cmpd=\"thickThin\""; break;
		case 3: strL = L"  cmpd=\"thinThick\"";	break;
		case 4: strL = L"  cmpd=\"tri\"";		break;
	}
	line_writer.WriteString(L"<a:ln w=\"" + std::to_wstring(pen.Size) + L"\"" + strL + L">");

	line_writer.WriteString(L"<a:solidFill>");
		line_writer.WriteString(ConvertColor(pen.Color, pen.Alpha));
	line_writer.WriteString(L"</a:solidFill>");

	switch(pen.DashStyle)
	{
		case 1:	line_writer.WriteString(L"<a:prstDash val=\"sysDash\"/>");			break;
		case 2:	line_writer.WriteString(L"<a:prstDash val=\"sysDot\"/>");			break;
		case 3:	line_writer.WriteString(L"<a:prstDash val=\"sysDashDot\"/>");		break;
		case 4:	line_writer.WriteString(L"<a:prstDash val=\"sysDashDotDot\"/>");	break;
		case 5:	line_writer.WriteString(L"<a:prstDash val=\"dot\"/>");				break;
		case 6:	line_writer.WriteString(L"<a:prstDash val=\"dash\"/>");				break;
		case 7:	line_writer.WriteString(L"<a:prstDash val=\"lgDash\"/>");			break;
		case 8:	line_writer.WriteString(L"<a:prstDash val=\"dashDot\"/>");			break;
		case 9:	line_writer.WriteString(L"<a:prstDash val=\"lgDashDot\"/>");		break;
		case 10:line_writer.WriteString(L"<a:prstDash val=\"lgDashDotDot\"/>");		break;
	}
	switch(pen.LineJoin)
	{
		case 0:	line_writer.WriteString(L"<a:bevel/>");	break;
		case 1:	line_writer.WriteString(L"<a:miter/>");	break;
		case 2:	line_writer.WriteString(L"<a:round/>");	break;
	}

	line_writer.WriteString(L"<a:headEnd" + ConvertLineEnd(pen.LineStartCap, pen.LineStartLength, pen.LineStartWidth)	+ L"/>");
	line_writer.WriteString(L"<a:tailEnd" + ConvertLineEnd(pen.LineEndCap, pen.LineEndLength, pen.LineEndWidth)			+ L"/>");
		
	line_writer.WriteString(L"</a:ln>");

	return line_writer.GetData();
}
std::wstring PPT_FORMAT::CShapeWriter::ConvertLineEnd(unsigned char cap, unsigned char length, unsigned char width)
{
	if (cap < 1) return L"";

	std::wstring sResult;

	switch(cap)
	{
		case 1: sResult += L" type=\"triangle\"";	break;
		case 2: sResult += L" type=\"stealth\"";	break;
		case 3: sResult += L" type=\"diamond\"";	break;
		case 4: sResult += L" type=\"oval\"";		break;
		case 5: sResult += L" type=\"arrow\"";		break;
	}
	switch(length)
	{
		case 0: sResult += L" len=\"sm\"";	break;
		case 1: sResult += L" len=\"med\"";	break;
		case 2: sResult += L" len=\"lg\"";	break;
	}
	switch(width)
	{
		case 0: sResult += L" w=\"sm\"";	break;
		case 1: sResult += L" w=\"med\"";	break;
		case 2: sResult += L" w=\"lg\"";	break;
	}
	return sResult;
}
std::wstring PPT_FORMAT::CShapeWriter::ConvertBrush(CBrush & brush)
{
	PPT_FORMAT::CStringWriter brush_writer;
	
	if (brush.Type == c_BrushTypeTexture)
	{
		if (m_pElement && ( m_pElement->m_etType == etPicture ||
							m_pElement->m_etType == etAudio ||
							m_pElement->m_etType == etVideo))		//фон для картинки с празрачностью
		{
			brush_writer.WriteString(L"<a:noFill/>");
		}
		else
		{
			std::wstring strRid = m_pRels->WriteImage(brush.TexturePath);

			brush_writer.WriteString(L"<a:blipFill dpi=\"0\" rotWithShape=\"1\"><a:blip r:embed=\"" + strRid + L"\"/><a:srcRect/>");
				
			if (	brush.TextureMode == c_BrushTextureModeTile)
				brush_writer.WriteString(L"<a:tile/>");
			else
				brush_writer.WriteString(L"<a:stretch><a:fillRect/></a:stretch>");

			brush_writer.WriteString(L"</a:blipFill>");
		}
	}
	else if ( brush.Type == c_BrushTypeNoFill ) 
	{
		brush_writer.WriteString(L"<a:noFill/>");
	}
	else if (	brush.Type == c_BrushTypePathGradient1	||
				brush.Type == c_BrushTypePathGradient2	||
				brush.Type == c_BrushTypeCenter			||
				brush.Type == c_BrushTypeHorizontal		||
				brush.Type == c_BrushTypeVertical		) 
	{
		brush_writer.WriteString(L"<a:gradFill><a:gsLst>");
		if (brush.ColorsPosition.empty() == false)
		{
			for (size_t i = 0; i < brush.ColorsPosition.size(); i++)
			{
				std::wstring str = std::to_wstring( (int)(brush.ColorsPosition[i].second * 1000));
				
				brush_writer.WriteString(L"<a:gs pos=\"" + str + L"\">");
					brush_writer.WriteString(ConvertColor(brush.ColorsPosition[i].first, 255));
				brush_writer.WriteString(L"</a:gs>");	
			}
		}
		else
		{
			brush_writer.WriteString(L"<a:gs pos=\"0\">");
				brush_writer.WriteString(ConvertColor(brush.Color1, brush.Alpha1));
			brush_writer.WriteString(L"</a:gs>");
			
			brush_writer.WriteString(L"<a:gs pos=\"100000\">");
				brush_writer.WriteString(ConvertColor(brush.Color2, brush.Alpha2));
			brush_writer.WriteString(L"</a:gs>");
		}
		brush_writer.WriteString(L"</a:gsLst>");
		brush_writer.WriteString(L"<a:lin ang=\"");
		{
			if (brush.LinearAngle < -180)	brush.LinearAngle += 180;
			if (brush.LinearAngle > 180)	brush.LinearAngle -= 180;

			double val = (90 - brush.LinearAngle) ;
			if (val < 0)	val = 0;
			if (val > 360)	val -= 360;
			
			std::wstring str = std::to_wstring((int)(val * 60000));
			brush_writer.WriteString(str);
		}
		brush_writer.WriteString(L"\" scaled=\"1\"/>");
		brush_writer.WriteString(L"</a:gradFill>");
	}
	else if(brush.Type == c_BrushTypePattern)
	{//типов нету в ппт - вместо них шаблон-картинка
		brush_writer.WriteString(L"<a:pattFill prst=\"pct80\">");
			brush_writer.WriteString(L"<a:fgClr>");
				brush_writer.WriteString(ConvertColor(brush.Color1, brush.Alpha1));
			brush_writer.WriteString(L"</a:fgClr>");
			brush_writer.WriteString(L"<a:bgClr>");
				brush_writer.WriteString(ConvertColor(brush.Color2, brush.Alpha2));
			brush_writer.WriteString(L"</a:bgClr>");
		brush_writer.WriteString(L"</a:pattFill>");
	}
	else if (brush.Type != c_BrushTypeNotSet)
	{
		brush_writer.WriteString(std::wstring(L"<a:solidFill>"));
			brush_writer.WriteString(ConvertColor(brush.Color1, brush.Alpha1));
		brush_writer.WriteString(std::wstring(L"</a:solidFill>"));
	}
	return brush_writer.GetData();
}

std::wstring PPT_FORMAT::CShapeWriter::ConvertShadow(CShadow	& shadow)
{
	std::wstring	Preset;
	bool			Inner = false;

	if (shadow.Visible == false)		return _T("");

	double dist = sqrt(shadow.DistanceY * shadow.DistanceY  + shadow.DistanceX * shadow.DistanceX);
	double dir	= 0;
	
	if (fabs(shadow.DistanceY) > 0)
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

	std::wstring strDir;
	std::wstring strDist;

	if (shadow.DistanceY != 0 && shadow.DistanceX != 0)
	{
		if (shadow.DistanceY < 0 && shadow.DistanceX < 0) dir /=2;

		strDir	= L" dir=\"" + std::to_wstring((int)(dir * 60000)) + L"\"";
		strDist	= L" dist=\"" + std::to_wstring((int)dist) + L"\"";	
	}

	std::wstring strSY;
	if (shadow.ScaleYToY < 1 || shadow.ScaleYToY > 1)
	{
		if (shadow.ScaleYToX < 1)shadow.ScaleYToY = -shadow.ScaleYToY;

		strSY = L" sy=\"" + std::to_wstring((int)(shadow.ScaleYToY * 100000)) + L"\"";	
	}
	std::wstring strSX;
	if (shadow.ScaleYToX < 1 || shadow.ScaleYToX > 1)
	{
		strSX = L" kx=\"" + std::to_wstring((int)((shadow.ScaleYToX + 0.5) * 360000)) + L"\"";
	}
	PPT_FORMAT::CStringWriter shadow_writer;
	
	shadow_writer.WriteString(L"<a:effectLst>");

	if (!Preset.empty())
	{
		shadow_writer.WriteString(L"<a:prstShdw");
			shadow_writer.WriteString(L" prst=\"" + Preset + L"\"");
			//shadow_writer.WriteString(std::wstring(L" rotWithShape=\"0\" algn=\"ctr\""));
			shadow_writer.WriteString(strDir);
			shadow_writer.WriteString(strDist);
		shadow_writer.WriteString(L">");

		shadow_writer.WriteString(ConvertColor(shadow.Color,shadow.Alpha));
		shadow_writer.WriteString(L"</a:prstShdw>");
	}
	else if (Inner)
	{
		shadow_writer.WriteString(L"<a:innerShdw");
			shadow_writer.WriteString(L" rotWithShape=\"0\"");
			if (strSX.empty() && strSY.empty())
			{
				shadow_writer.WriteString(L" algn=\"ctr\"");
			}
			shadow_writer.WriteString(strSX);
			shadow_writer.WriteString(strSY);
			shadow_writer.WriteString(strDir);
			shadow_writer.WriteString(strDist);
		shadow_writer.WriteString(L">");

		shadow_writer.WriteString(ConvertColor(shadow.Color,shadow.Alpha));
		shadow_writer.WriteString(L"</a:innerShdw>");
	}
	else
	{
		shadow_writer.WriteString(L"<a:outerShdw");
			shadow_writer.WriteString(L" rotWithShape=\"0\"");
			if (strSX.empty() && strSY.empty())
			{
				shadow_writer.WriteString(L" algn=\"ctr\"");
			}
			shadow_writer.WriteString(strSX);
			shadow_writer.WriteString(strSY);
			shadow_writer.WriteString(strDir);
			shadow_writer.WriteString(strDist);
		shadow_writer.WriteString(L">");

		shadow_writer.WriteString(ConvertColor(shadow.Color,shadow.Alpha));
		shadow_writer.WriteString(L"</a:outerShdw>");
	}
	shadow_writer.WriteString(L"</a:effectLst>");
	return shadow_writer.GetData();
}

std::wstring PPT_FORMAT::CShapeWriter::ConvertColor(CColor & color, long alpha)
{
	PPT_FORMAT::CStringWriter color_writer;
	if (color.m_lSchemeIndex == -1)
    {
		if (255 == alpha)
		{
            color_writer.WriteString(L"<a:srgbClr val=\"" + XmlUtils::IntToString(color.GetLONG_RGB(), L"%06X") + L"\"/>");
		}
		else
		{
            color_writer.WriteString(L"<a:srgbClr val=\"" + XmlUtils::IntToString(color.GetLONG_RGB(), L"%06X") + L"\">" +
                                     L"<a:alpha val=\"" + std::to_wstring((int)(alpha * 100000 / 255)) + L"\"/></a:srgbClr>");
		}
	}
	else
	{
		if (255 == alpha)
		{
			color_writer.WriteString(L"<a:schemeClr val=\"" + CStylesWriter::GetColorInScheme(color.m_lSchemeIndex) + L"\"/>");
		}
		else
		{
			std::wstring strAlpha = std::to_wstring((int)(alpha * 100000 / 255));
			color_writer.WriteString(L"<a:schemeClr val=\"" + CStylesWriter::GetColorInScheme(color.m_lSchemeIndex) + L"\"><a:alpha val=\"" + strAlpha + L"\"/></a:schemeClr>");
		}
	}
	return color_writer.GetData();
}
void PPT_FORMAT::CShapeWriter::WriteImageInfo()
{
    CImageElement*	pImageElement = dynamic_cast<CImageElement*>(m_pElement.get());
	if (!pImageElement) return;
	
	CAudioElement*	pAudioElement = dynamic_cast<CAudioElement*>(pImageElement);
    CVideoElement*	pVideoElement = dynamic_cast<CVideoElement*>(pImageElement);
	
	m_oWriter.WriteString(std::wstring(L"<p:nvPicPr>"));

	if (pImageElement->m_lID < 0) 
		pImageElement->m_lID = m_lNextShapeID;

	std::wstring strShapeID = std::to_wstring(pImageElement->m_lID);	
	
	m_oWriter.WriteString(std::wstring(L"<p:cNvPr id=\"") + strShapeID + L"\"" );
	
	if (pImageElement->m_sName.empty()) 
	{
		if (pAudioElement)		pImageElement->m_sName = std::wstring(L"Audio ") + strShapeID;
		else if (pVideoElement)	pImageElement->m_sName = std::wstring(L"Video ") + strShapeID;
		else					pImageElement->m_sName = std::wstring(L"Image ") + strShapeID;
	}
	
	if (pImageElement->m_bHidden)	m_oWriter.WriteString(std::wstring(L" hidden=\"1\""));

	m_oWriter.WriteString(std::wstring(L" name=\""));
		m_oWriter.WriteStringXML(pImageElement->m_sName);
	m_oWriter.WriteString(std::wstring(L"\""));
	
	if (!pImageElement->m_sDescription.empty())
	{//бывает всякая разная бяка сохранена 
		m_oWriter.WriteString(std::wstring(L" descr=\""));
		m_oWriter.WriteString(XmlUtils::EncodeXmlStringExtend(pImageElement->m_sDescription, true));
		m_oWriter.WriteString(std::wstring(L"\""));
	}
	m_oWriter.WriteString(std::wstring(L">"));

	if (pVideoElement || pAudioElement)
	{
		m_oWriter.WriteString(std::wstring(L"<a:hlinkClick r:id=\"\" action=\"ppaction://media\"/>"));
	}

	m_oWriter.WriteString(std::wstring(L"</p:cNvPr><p:cNvPicPr><a:picLocks"));
	
	if (false == m_pElement->m_bChildAnchorEnabled)
	{
		m_oWriter.WriteString(std::wstring(L" noGrp=\"1\""));
	}
	m_oWriter.WriteString(std::wstring(L" noChangeAspect=\"1\"/></p:cNvPicPr>"));

	++m_lNextShapeID;

	m_oWriter.WriteString(std::wstring(L"<p:nvPr>"));
	
	if (-1 != pImageElement->m_lPlaceholderType)
	{
		m_oWriter.WriteString(std::wstring(L"<p:ph"));
		
		if (pImageElement->m_lPlaceholderType > 0 && pImageElement->m_lPlaceholderType != PT_Body_Empty)
			m_oWriter.WriteString(std::wstring(L" type=\"") + GetPhType(pImageElement->m_lPlaceholderType) + L"\"");
		
		if (-1 != pImageElement->m_lPlaceholderID)
		{
			std::wstring strIdx = std::to_wstring(pImageElement->m_lPlaceholderID );
			m_oWriter.WriteString(std::wstring(L" idx=\"") + strIdx + L"\"");

		}
		m_oWriter.WriteString(std::wstring(L"/>"));
	}

	std::wstring sMediaFile;
	
	if ((pVideoElement) && (!pVideoElement->m_strVideoFileName.empty()))
	{
		bool bExternal = false;
		std::wstring strRid = m_pRels->WriteVideo(pVideoElement->m_strVideoFileName, bExternal);
	
		m_oWriter.WriteString(L"<a:videoFile r:link=\"" + strRid + L"\"/>");

		sMediaFile = bExternal ? L"" : pVideoElement->m_strVideoFileName;
	}

	if ((pAudioElement) && (!pAudioElement->m_strAudioFileName.empty()))
	{
		bool bExternal = false;
		std::wstring strRid = m_pRels->WriteAudio(pAudioElement->m_strAudioFileName, bExternal);

		m_oWriter.WriteString(L"<a:audioFile r:link=\"" + strRid + L"\"/>");
		
		sMediaFile = bExternal ? L"" : pAudioElement->m_strAudioFileName;
	}
	if (sMediaFile.empty() == false)
	{
		std::wstring strRid = m_pRels->WriteMedia(sMediaFile);
		if (!strRid.empty())
		{
			m_oWriter.WriteString(L"<p:extLst><p:ext uri=\"{DAA4B4D4-6D71-4841-9C94-3DE7FCFB9230}\">\
<p14:media xmlns:p14=\"http://schemas.microsoft.com/office/powerpoint/2010/main\" r:embed=\"" + strRid + L"\"/></p:ext></p:extLst>");
		}
	}

	m_oWriter.WriteString(std::wstring(L"</p:nvPr>"));
	
    std::wstring str2 = _T("</p:nvPicPr>");
	m_oWriter.WriteString(str2);
}
void PPT_FORMAT::CShapeWriter::WriteGroupInfo()
{
	CGroupElement* pGroupElement = dynamic_cast<CGroupElement*>(m_pElement.get());
	if (!pGroupElement) return;

	m_oWriter.WriteString(std::wstring(L"<p:nvGrpSpPr>"));

	if (pGroupElement->m_lID < 0) 
		pGroupElement->m_lID = m_lNextShapeID;

	std::wstring strShapeID = std::to_wstring(pGroupElement->m_lID);

	m_oWriter.WriteString(std::wstring(L"<p:cNvPr id=\"") + strShapeID + L"\"");

	if (pGroupElement->m_sName.empty()) pGroupElement->m_sName = std::wstring(L"Group ") +  strShapeID;

	if (pGroupElement->m_bHidden)	m_oWriter.WriteString(std::wstring(L" hidden=\"1\""));
	
	m_oWriter.WriteString(std::wstring(L" name=\""));
		m_oWriter.WriteStringXML(pGroupElement->m_sName);
	m_oWriter.WriteString(std::wstring(L"\""));

	if (!pGroupElement->m_sDescription.empty())
	{
		m_oWriter.WriteString(std::wstring(L" descr=\""));
			m_oWriter.WriteStringXML(pGroupElement->m_sDescription);
		m_oWriter.WriteString(std::wstring(L"\""));
	}
	m_oWriter.WriteString(std::wstring(L">"));
	if (!pGroupElement->m_sHyperlink.empty())
	{
        std::wstring rId = m_pRels->WriteHyperlink(pGroupElement->m_sHyperlink);

		m_oWriter.WriteString(std::wstring(L"<a:hlinkClick"));
			m_oWriter.WriteString(std::wstring(L" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\""));					
			m_oWriter.WriteString(std::wstring(L" r:id=\"" + rId ));
		m_oWriter.WriteString(std::wstring(L"\"></a:hlinkClick>"));
	}		
	m_oWriter.WriteString(std::wstring(L"</p:cNvPr>"));
	
	m_oWriter.WriteString(std::wstring(L"<p:cNvGrpSpPr><a:grpSpLocks/></p:cNvGrpSpPr>"));

	++m_lNextShapeID;

	m_oWriter.WriteString(std::wstring(L"<p:nvPr/>"));
	
    std::wstring str2 = _T("</p:nvGrpSpPr>");
	m_oWriter.WriteString(str2);
}

void PPT_FORMAT::CShapeWriter::WriteShapeInfo()
{
	CShapeElement* pShapeElement = dynamic_cast<CShapeElement*>(m_pElement.get());
	if (!pShapeElement) return;

	m_oWriter.WriteString(std::wstring(L"<p:nvSpPr>"));

	if (pShapeElement->m_lID < 0) 
		pShapeElement->m_lID = m_lNextShapeID;

	std::wstring strShapeID = std::to_wstring(pShapeElement->m_lID);

	m_oWriter.WriteString(std::wstring(L"<p:cNvPr id=\"") + strShapeID + L"\"");

	if (pShapeElement->m_sName.empty()) pShapeElement->m_sName = std::wstring(L"Shape ") +  strShapeID;

	if (pShapeElement->m_bHidden)	m_oWriter.WriteString(std::wstring(L" hidden=\"1\""));
	
	m_oWriter.WriteString(std::wstring(L" name=\""));
		m_oWriter.WriteStringXML(pShapeElement->m_sName);
	m_oWriter.WriteString(std::wstring(L"\""));

	if (!pShapeElement->m_sDescription.empty())
	{
		m_oWriter.WriteString(std::wstring(L" descr=\""));
			m_oWriter.WriteStringXML(pShapeElement->m_sDescription);
		m_oWriter.WriteString(std::wstring(L"\""));
	}
	m_oWriter.WriteString(std::wstring(L">"));
	if (!pShapeElement->m_sHyperlink.empty())
	{
        std::wstring rId = m_pRels->WriteHyperlink(pShapeElement->m_sHyperlink);

		m_oWriter.WriteString(std::wstring(L"<a:hlinkClick"));
			m_oWriter.WriteString(std::wstring(L" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\""));					
			m_oWriter.WriteString(std::wstring(L" r:id=\"" + rId ));
		m_oWriter.WriteString(std::wstring(L"\"></a:hlinkClick>"));
	}		
	m_oWriter.WriteString(std::wstring(L"</p:cNvPr>"));
	
	m_oWriter.WriteString(std::wstring(L"<p:cNvSpPr"));
		if (m_bTextBox)
			m_oWriter.WriteString(std::wstring(L" txBox=\"1\""));
	m_oWriter.WriteString(std::wstring(L"><a:spLocks"));
	
	if (false == m_pElement->m_bChildAnchorEnabled)
	{
		m_oWriter.WriteString(std::wstring(L" noGrp=\"1\""));
	}
	m_oWriter.WriteString(std::wstring(L" noChangeShapeType=\"1\"/>"));

	m_oWriter.WriteString(std::wstring(L"</p:cNvSpPr>"));

	++m_lNextShapeID;

	if (-1 != pShapeElement->m_lPlaceholderType)
	{
		m_oWriter.WriteString(std::wstring(L"<p:nvPr><p:ph"));
		
		if (pShapeElement->m_lPlaceholderType > 0 && pShapeElement->m_lPlaceholderType != PT_Body_Empty)
			m_oWriter.WriteString(std::wstring(L" type=\"") + GetPhType(pShapeElement->m_lPlaceholderType) + _T("\""));
		
		if ( pShapeElement->m_lPlaceholderID != -1)
		{
			std::wstring strIdx = std::to_wstring( pShapeElement->m_lPlaceholderID );			 
			m_oWriter.WriteString(std::wstring(L" idx=\"") + strIdx + L"\"");
		}
		
		if (pShapeElement->m_lPlaceholderSizePreset > 1 && !isTitlePlaceholder(pShapeElement->m_lPlaceholderType))
		{
			if (pShapeElement->m_lPlaceholderSizePreset == 1)
				m_oWriter.WriteString(std::wstring(L" size=\"half\""));
			else if (pShapeElement->m_lPlaceholderSizePreset == 2)
				m_oWriter.WriteString(std::wstring(L" size=\"quarter\""));
			else if (pShapeElement->m_lPlaceholderSizePreset == 3)
			{
				if (isBodyPlaceholder(pShapeElement->m_lPlaceholderType))
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
	
    std::wstring str2 = _T("</p:nvSpPr>");
	m_oWriter.WriteString(str2);
}
void PPT_FORMAT::CShapeWriter::Write3dShape()
{
	CShapeElement* pShapeElement = dynamic_cast<CShapeElement*>(m_pElement.get());
	if (!pShapeElement) return;

	CPPTShape *pPPTShape = dynamic_cast<CPPTShape *>(pShapeElement->m_pShape->getBaseShape().get());
	if (!pPPTShape) return;

	if (!pPPTShape->m_o3dOptions.bEnabled) return;

	//{
	//	std::cout << "........................................................................\n";
	//	std::wcout << pShapeElement->m_sName << L"\n";
	//	if (pPPTShape->m_o3dOptions.dSpecularAmt) 
	//	{
	//		std::cout << "SpecularAmt\t" << (*pPPTShape->m_o3dOptions.dSpecularAmt) << "\n";
	//	}
	//	if (pPPTShape->m_o3dOptions.dDiffuseAmt) 
	//	{
	//		std::cout << "iffuseAmt\t" << (*pPPTShape->m_o3dOptions.dDiffuseAmt) << "\n";
	//	}
	//	if (pPPTShape->m_o3dOptions.dShininess) 
	//	{
	//		std::cout << "Shininess\t" << (*pPPTShape->m_o3dOptions.dShininess) << "\n";
	//	}
	//	if (pPPTShape->m_o3dOptions.dXRotationAngle) 
	//	{
	//		std::cout << "XRotationAngle\t" << (*pPPTShape->m_o3dOptions.dXRotationAngle) << "\n";
	//	}
	//	if (pPPTShape->m_o3dOptions.dYRotationAngle) 
	//	{
	//		std::cout << "YRotationAngle\t" << (*pPPTShape->m_o3dOptions.dYRotationAngle) << "\n";
	//	}
	//	if (pPPTShape->m_o3dOptions.dRotationAxisX) 
	//	{
	//		std::cout << "RotationAxisX\t" << (*pPPTShape->m_o3dOptions.dRotationAxisX) << "\n";
	//	}
	//	if (pPPTShape->m_o3dOptions.dRotationAxisY) 
	//	{
	//		std::cout << "RotationAxisY\t" << (*pPPTShape->m_o3dOptions.dRotationAxisY) << "\n";
	//	}
	//	if (pPPTShape->m_o3dOptions.dRotationAxisZ) 
	//	{
	//		std::cout << "RotationAxisZ\t" << (*pPPTShape->m_o3dOptions.dRotationAxisZ) << "\n";
	//	}
	//	if (pPPTShape->m_o3dOptions.dRotationAngle) 
	//	{
	//		std::cout << "RotationAngle\t" << (*pPPTShape->m_o3dOptions.dRotationAngle) << "\n";
	//	}
	//	if (pPPTShape->m_o3dOptions.dRotationCenterX) 
	//	{
	//		std::cout << "RotationCenterX\t" << (*pPPTShape->m_o3dOptions.dRotationCenterX) << "\n";
	//	}
	//	if (pPPTShape->m_o3dOptions.dRotationCenterY) 
	//	{
	//		std::cout << "RotationCenterY\t" << (*pPPTShape->m_o3dOptions.dRotationCenterY) << "\n";
	//	}
	//	if (pPPTShape->m_o3dOptions.dRotationCenterZ) 
	//	{
	//		std::cout << "RotationCenterZ\t" << (*pPPTShape->m_o3dOptions.dRotationCenterZ) << "\n";
	//	}
	//	if (pPPTShape->m_o3dOptions.dTolerance) 
	//	{
	//		std::cout << "Tolerance\t" << (*pPPTShape->m_o3dOptions.dTolerance) << "\n";
	//	}
	//	std::cout << "XViewpoint\t" << (pPPTShape->m_o3dOptions.dXViewpoint) << "\n";
	//	std::cout << "YViewpoint\t" << (pPPTShape->m_o3dOptions.dYViewpoint) << "\n";
	//	if (pPPTShape->m_o3dOptions.dZViewpoint)
	//	{
	//		std::cout << "ZViewpoint\t" << (*pPPTShape->m_o3dOptions.dZViewpoint) << "\n";
	//	}
	//	std::cout << "OriginX\t" << (pPPTShape->m_o3dOptions.dOriginX) << "\n";
	//	std::cout << "OriginY\t" << (pPPTShape->m_o3dOptions.dOriginY) << "\n";
	//	std::cout << "SkewAngle\t" << (pPPTShape->m_o3dOptions.dSkewAngle) << "\n";
	//	std::cout << "SkewAmount\t" << (pPPTShape->m_o3dOptions.nSkewAmount) << "\n";

	//	if (pPPTShape->m_o3dOptions.dAmbientIntensity)
	//	{
	//		std::cout << "AmbientIntensity\t" << (*pPPTShape->m_o3dOptions.dAmbientIntensity) << "\n";
	//	}		
	//	if (pPPTShape->m_o3dOptions.dKeyIntensity)
	//	{
	//		std::cout << "KeyIntensity\t" << (*pPPTShape->m_o3dOptions.dKeyIntensity) << "\n";
	//	}
	//	std::cout << "KeyX\t" << (pPPTShape->m_o3dOptions.dKeyX) << "\n";
	//	std::cout << "KeyY\t" << (pPPTShape->m_o3dOptions.dKeyY) << "\n";
	//	if (pPPTShape->m_o3dOptions.dKeyZ)
	//	{
	//		std::cout << "KeyZ\t" << (*pPPTShape->m_o3dOptions.dKeyZ) << "\n";
	//	}
	//	std::cout << "FillIntensity\t" << (pPPTShape->m_o3dOptions.dFillIntensity) << "\n";
	//	std::cout << "FillX\t" << (pPPTShape->m_o3dOptions.dFillX) << "\n";
	//	std::cout << "FillY\t" << (pPPTShape->m_o3dOptions.dFillY) << "\n";
	//	if (pPPTShape->m_o3dOptions.dFillZ)
	//	{
	//		std::cout << "FillZ\t" << (*pPPTShape->m_o3dOptions.dFillZ) << "\n";
	//	}
	//	std::cout << "........................................................................\n";
	//}

	m_oWriter.WriteString(std::wstring(L"<a:scene3d>"));
		m_oWriter.WriteString(std::wstring(L"<a:camera"));
		
		std::wstring sAngle;
		if (pPPTShape->m_o3dOptions.dSkewAngle < 10) sAngle = L"Left";
		else if (pPPTShape->m_o3dOptions.dSkewAngle >= 10 && pPPTShape->m_o3dOptions.dSkewAngle < 80) sAngle = L"BottomLeft";
		else if (pPPTShape->m_o3dOptions.dSkewAngle >= 80 && pPPTShape->m_o3dOptions.dSkewAngle < 100) sAngle = L"Bottom";
		else if (pPPTShape->m_o3dOptions.dSkewAngle >= 100 && pPPTShape->m_o3dOptions.dSkewAngle < 145) sAngle = L"BottomRight";
		else if (pPPTShape->m_o3dOptions.dSkewAngle >= 145 && pPPTShape->m_o3dOptions.dSkewAngle < 190) sAngle = L"Right";
		else if (pPPTShape->m_o3dOptions.dSkewAngle >= 190 && pPPTShape->m_o3dOptions.dSkewAngle < 235) sAngle = L"TopRight";
		else if (pPPTShape->m_o3dOptions.dSkewAngle >= 235 && pPPTShape->m_o3dOptions.dSkewAngle < 280) sAngle = L"Top";
		else if (pPPTShape->m_o3dOptions.dSkewAngle >= 280 && pPPTShape->m_o3dOptions.dSkewAngle < 325) sAngle = L"TopLeft";
		else sAngle = L"Front";

		if (pPPTShape->m_o3dOptions.bParallel)
			m_oWriter.WriteString(std::wstring(L" prst=\"legacyOblique" + sAngle + L"\""));
		else
			m_oWriter.WriteString(std::wstring(L" prst=\"legacyPerspective" + sAngle + L"\""));
		m_oWriter.WriteString(std::wstring(L">"));
		if (pPPTShape->m_o3dOptions.bConstrainRotation)
		{
			m_oWriter.WriteString(std::wstring(L"<a:rot"));
				m_oWriter.WriteString(std::wstring(L" lat=\"" + std::to_wstring((int)(60000 * pPPTShape->m_o3dOptions.dXRotationAngle.get_value_or(0))) + L"\""));
				m_oWriter.WriteString(std::wstring(L" lon=\"" + std::to_wstring((int)(60000 * pPPTShape->m_o3dOptions.dYRotationAngle.get_value_or(0))) + L"\""));
				m_oWriter.WriteString(std::wstring(L" rev=\"" + std::to_wstring((int)(60000 * pPPTShape->m_o3dOptions.dRotationAngle.get_value_or(0))) + L"\""));
			m_oWriter.WriteString(std::wstring(L"/>"));
		}
		m_oWriter.WriteString(std::wstring(L"</a:camera>"));

  		m_oWriter.WriteString(std::wstring(L"<a:lightRig"));
			if (pPPTShape->m_o3dOptions.dFillIntensity < 0.3)		
				m_oWriter.WriteString(std::wstring(L" rig=\"legacyHarsh3\""));
			else if (pPPTShape->m_o3dOptions.dFillIntensity < 0.4)	
				m_oWriter.WriteString(std::wstring(L" rig=\"legacyFlat4\""));
			else if (pPPTShape->m_o3dOptions.dFillIntensity > 0.6 || 
				(pPPTShape->m_o3dOptions.dKeyX < 0.1 && pPPTShape->m_o3dOptions.dFillX < 0.1 && 
				 pPPTShape->m_o3dOptions.dKeyY < 0.1 && pPPTShape->m_o3dOptions.dFillY < 0.1))	
				m_oWriter.WriteString(std::wstring(L" rig=\"legacyFlat1\""));
			else
				m_oWriter.WriteString(std::wstring(L" rig=\"legacyFlat3\""));

			if (pPPTShape->m_o3dOptions.dKeyX < 0.1 && pPPTShape->m_o3dOptions.dFillX < 0.1)
			{
				if (pPPTShape->m_o3dOptions.dKeyY < 0.1 && pPPTShape->m_o3dOptions.dFillY < 0.1)
															m_oWriter.WriteString(std::wstring(L" dir=\"t\""));
				else if (pPPTShape->m_o3dOptions.dKeyY < 0)	m_oWriter.WriteString(std::wstring(L" dir=\"r\""));
				else										m_oWriter.WriteString(std::wstring(L" dir=\"l\""));
			}
			else
			{
				if (pPPTShape->m_o3dOptions.dKeyX < 0)	m_oWriter.WriteString(std::wstring(L" dir=\"t\""));
				else									m_oWriter.WriteString(std::wstring(L" dir=\"b\""));
			}
		m_oWriter.WriteString(std::wstring(L"/>"));
	m_oWriter.WriteString(std::wstring(L"</a:scene3d>"));
	
	m_oWriter.WriteString(std::wstring(L"<a:sp3d"));
		m_oWriter.WriteString(std::wstring(L" extrusionH=\"" + std::to_wstring(pPPTShape->m_o3dOptions.nExtrudeBackward) + L"\""));

	switch(pPPTShape->m_o3dOptions.nRenderMode)
	{
	case 0x00000000://solid
	{
		if (pPPTShape->m_o3dOptions.bMetallic)
			m_oWriter.WriteString(std::wstring(L" prstMaterial=\"legacyMetal\""));
		else 
			m_oWriter.WriteString(std::wstring(L" prstMaterial=\"legacyMatte\"")); 
	}break; 
	case 0x00000001: m_oWriter.WriteString(std::wstring(L" prstMaterial=\"legacyWireframe\"")); break; //wireframe
	case 0x00000002: m_oWriter.WriteString(std::wstring(L" prstMaterial=\"legacyWireframe\"")); break; //bounding cube ????
	}

	m_oWriter.WriteString(std::wstring(L">"));

	m_oWriter.WriteString(std::wstring(L"<a:bevelT w=\"13500\" h=\"13500\" prst=\"angle\"/>"));
	m_oWriter.WriteString(std::wstring(L"<a:bevelB w=\"13500\" h=\"13500\" prst=\"angle\"/>"));

	std::wstring strExtrusionClr;
		if (pPPTShape->m_o3dOptions.oExtrusionColor)
		{
			strExtrusionClr = ConvertColor(*pPPTShape->m_o3dOptions.oExtrusionColor);
		}
		else
		{
			if (pShapeElement->m_oBrush.Type != c_BrushTypeNoFill)
			{
				strExtrusionClr = ConvertColor(pShapeElement->m_oBrush.Color1, pShapeElement->m_oBrush.Alpha1);
			}
			else
			{
				strExtrusionClr = ConvertColor(pShapeElement->m_oPen.Color, pShapeElement->m_oPen.Alpha);
			}
		}
	m_oWriter.WriteString(std::wstring(L"<a:extrusionClr>"));
		m_oWriter.WriteString(strExtrusionClr);
	m_oWriter.WriteString(std::wstring(L"</a:extrusionClr>"));
	m_oWriter.WriteString(std::wstring(L"<a:contourClr>"));
		m_oWriter.WriteString(strExtrusionClr);
	m_oWriter.WriteString(std::wstring(L"</a:contourClr>"));

	m_oWriter.WriteString(std::wstring(L"</a:sp3d>"));
}
void PPT_FORMAT::CShapeWriter::WriteTextInfo()
{
	//if (false == m_xmlTxBodyAlternative.empty())
	//{
	//	m_oWriter.WriteString(m_xmlTxBodyAlternative);

	//	return;
	//}
	CShapeElement* pShapeElement = dynamic_cast<CShapeElement*>(m_pElement.get());
	if (!pShapeElement) return;

	CDoubleRect oTextRect;

	size_t nCount = pShapeElement->m_pShape->m_oText.m_arParagraphs.size();

	m_oWriter.WriteString(std::wstring(L"<p:txBody>"));

	m_oWriter.WriteString(std::wstring(L"<a:bodyPr" ));

	pShapeElement->m_pShape->GetTextRect(oTextRect);

    std::wstring str =  L" lIns=\"" + std::to_wstring((int)oTextRect.left) + L"\" tIns=\"" + std::to_wstring((int)oTextRect.top) +
                        L"\" rIns=\"" + std::to_wstring((int)oTextRect.right) + L"\" bIns=\"" + std::to_wstring((int)oTextRect.bottom) + L"\"";
	
	m_oWriter.WriteString(str);
	
//	m_oWriter.WriteString(std::wstring(L" lIns=\"0\" tIns=\"0\" rIns=\"0\" bIns=\"0\""));

	if (pShapeElement->m_pShape->m_oText.m_oAttributes.m_nTextAlignVertical == 0 )			
		m_oWriter.WriteString(L" anchor=\"t\"");
	else if (pShapeElement->m_pShape->m_oText.m_oAttributes.m_nTextAlignVertical == 2 )	
		m_oWriter.WriteString(L" anchor=\"b\"");
	else if (pShapeElement->m_pShape->m_oText.m_oAttributes.m_nTextAlignVertical == 1 )	
	{
		m_oWriter.WriteString(L" anchor=\"ctr\"");
		m_oWriter.WriteString(L" anchorCtr=\"0\"");
	}
	if (pShapeElement->m_pShape->m_oText.m_oAttributes.m_dTextRotate > 0)
	{
		std::wstring strProp = std::to_wstring((int)(pShapeElement->m_pShape->m_oText.m_oAttributes.m_dTextRotate * 60000));
		m_oWriter.WriteString(L" rot=\"" + strProp + L"\"");
	}
	if (pShapeElement->m_pShape->m_oText.m_nTextFlow >= 0)
	{
		switch(pShapeElement->m_pShape->m_oText.m_nTextFlow)
		{
			case 1: 
			case 3: m_oWriter.WriteString(L" vert=\"vert\"");			break;
			case 2: m_oWriter.WriteString(L" vert=\"vert270\"");		break;
			case 5: m_oWriter.WriteString(L" vert=\"wordArtVert\"");	break;
		}
	}
	else if (pShapeElement->m_pShape->m_oText.m_bVertical)
	{
		m_oWriter.WriteString(L" vert=\"eaVert\"");
	}
	m_oWriter.WriteString(L">");

	if (m_bWordArt)
	{
		std::wstring prstTxWarp = oox::Spt2WordArtShapeType((oox::MSOSPT)pShapeElement->m_lShapeType);				
		m_oWriter.WriteString(std::wstring(L"<a:prstTxWarp"));
			m_oWriter.WriteString(std::wstring(L" prst=\"") + prstTxWarp + _T("\">"));
			m_oWriter.WriteString(std::wstring(L"<a:avLst>"));//модификаторы

			CPPTShape *pPPTShape = dynamic_cast<CPPTShape *>(pShapeElement->m_pShape->getBaseShape().get());
            std::wstring strVal;

			for (size_t i = 0 ; (pPPTShape) && (i < pPPTShape->m_arAdjustments.size()); i++)
			{
				switch(pShapeElement->m_lShapeType)
				{
					case oox::msosptTextFadeUp:
					{
						double kf = 4.63; //"волшебный"
						std::wstring strVal = std::to_wstring((int)(kf * pPPTShape->m_arAdjustments[i]));
						
						m_oWriter.WriteString(L"<a:gd name=\"adj\" fmla=\"val ");
						m_oWriter.WriteString(strVal + L"\"/>");
					}break;
				}
				
			}

			m_oWriter.WriteString(L"</a:avLst>");
		m_oWriter.WriteString(L"</a:prstTxWarp>");
	}
	if (pShapeElement->m_pShape->m_oText.m_bAutoFit)
	{
		m_oWriter.WriteString(L"<a:spAutoFit/>");
	}
	m_oWriter.WriteString(L"</a:bodyPr>");
	
	if (0 == nCount)
	{
		m_oWriter.WriteString(L"<a:lstStyle/><a:p><a:endParaRPr dirty=\"0\"/></a:p></p:txBody>");
		return;
	}
	m_oWriter.WriteString(L"<a:lstStyle>");

	if (!m_bWordArt)
	{
		CStylesWriter styleWriter(m_pTheme);
		styleWriter.ConvertStyles(pShapeElement->m_pShape->m_oText.m_oStyles, m_oWriter);
	}

	m_oWriter.WriteString(std::wstring(L"</a:lstStyle>"));

	for (size_t nIndexPar = 0; nIndexPar < nCount; ++nIndexPar)
	{
		PPT_FORMAT::CParagraph* pParagraph = &pShapeElement->m_pShape->m_oText.m_arParagraphs[nIndexPar];

		//if (m_bWordArt && nIndexPar == nCount-1)
		//{
		//	if (pParagraph->m_arSpans.size() < 1) break;
		//	if (pParagraph->m_arSpans.size() == 1 && pParagraph->m_arSpans[0].m_strText.empty()) break;
		//}

		std::wstring _str1 = std::to_wstring(pParagraph->m_lTextLevel);
		m_oWriter.WriteString(L"<a:p><a:pPr lvl=\"" + _str1 + L"\"");

		PPT_FORMAT::CTextPFRun* pPF = &pParagraph->m_oPFRun;

		if (pPF->textDirection.is_init())
		{
			if (pPF->textDirection.get() == 1)	m_oWriter.WriteString(std::wstring(L" rtl=\"1\""));
			else								m_oWriter.WriteString(std::wstring(L" rtl=\"0\""));
		}
		if (pPF->fontAlign.is_init())
		{
			std::wstring strProp = CStylesWriter::GetFontAlign(pPF->fontAlign.get());
			m_oWriter.WriteString(std::wstring(L" fontAlgn=\"") + strProp + L"\"");
		}
		int leftMargin = 0;
		if (pPF->leftMargin.is_init())
		{
			leftMargin = pPF->leftMargin.get();
            std::wstring strProp = std::to_wstring( leftMargin );
			m_oWriter.WriteString(L" marL=\"" + strProp + L"\"");			
		}
		if (pPF->indent.is_init())
		{
			std::wstring strProp = std::to_wstring(pPF->indent.get() - leftMargin);
			m_oWriter.WriteString(L" indent=\"" + strProp + L"\"");
		}
		if (pPF->textAlignment.is_init())
		{
            std::wstring strProp = CStylesWriter::GetTextAlign(pPF->textAlignment.get());
			m_oWriter.WriteString(L" algn=\"" + strProp + L"\"");
		}
		if (pPF->defaultTabSize.is_init())
		{
			std::wstring strProp= std::to_wstring( pPF->defaultTabSize.get());
			m_oWriter.WriteString(L" defTabSz=\"" + strProp + L"\"");
		}
		m_oWriter.WriteString(L">");

		double dKoef1 = 3.52777778; // :-) чё это не понятно ...
		if (pPF->lineSpacing.is_init())
		{
			LONG val = pPF->lineSpacing.get();
			//1/1024 master unit or 1/589824 inch.
			//1 inch = 576 master unit -> 1 master unit = 0.125 pt
			if (val > 0)//The absolute value specifies spacing in master units.
			{
				std::wstring strProp = std::to_wstring( (int)(val* 0.125 * 100/*/ dKoef1*/));
				m_oWriter.WriteString(L"<a:lnSpc><a:spcPts val=\"" + strProp + L"\"/></a:lnSpc>");
			}
			else if (val < 0 && val > -13200)
			{//0 to 13200, inclusive - The value specifies spacing as a percentage of the text line height.
				std::wstring strProp = std::to_wstring( -val * 1000);
				m_oWriter.WriteString(L"<a:lnSpc><a:spcPct val=\"" + strProp + L"\"/></a:lnSpc>");
			}
		}
		if (pPF->spaceAfter.is_init())
		{
			LONG val = pPF->spaceAfter.get();
			if (val > 0)
			{
				std::wstring strProp = std::to_wstring((int)(val * 0.125 * 100/*/ dKoef1*/));
				m_oWriter.WriteString(L"<a:spcAft><a:spcPts val=\"" + strProp + L"\"/></a:spcAft>");
			}
			else if (val < 0 && val > -13200)
			{
				std::wstring strProp = std::to_wstring(-val * 1000);
				m_oWriter.WriteString(L"<a:spcAft><a:spcPct val=\"" + strProp + L"\"/></a:spcAft>");
			}
		}
		if (pPF->spaceBefore.is_init())
		{
			LONG val = pPF->spaceBefore.get();
			if (val > 0)
			{
				std::wstring strProp = std::to_wstring((int)(val * 0.125 * 100/*/ dKoef1*/));
				m_oWriter.WriteString(L"<a:spcBef><a:spcPts val=\"" + strProp + L"\"/></a:spcBef>");
			}
			else if (val < 0 && val > -13200)
			{
				std::wstring strProp = std::to_wstring(-val * 1000);
				m_oWriter.WriteString(L"<a:spcBef><a:spcPct val=\"" + strProp + L"\"/></a:spcBef>");
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
						std::wstring strProp = std::to_wstring(pPF->bulletSize.get() * 1000 );
						m_oWriter.WriteString(L"<a:buSzPct val=\"" + strProp + L"\"/>");
					}
					if (pPF->bulletSize.get() < 0 && pPF->bulletSize.get() > -4001)
					{
						std::wstring strProp = std::to_wstring(- pPF->bulletSize.get() );
						m_oWriter.WriteString(L"<a:buSzPts val=\"" + strProp + L"\"/>");
					}
				}
				if (pPF->bulletFontProperties.is_init())
				{
					m_oWriter.WriteString(std::wstring(L"<a:buFont typeface=\"") + pPF->bulletFontProperties->Name + L"\"");

					if ( pPF->bulletFontProperties->PitchFamily > 0)
					{
						m_oWriter.WriteString(std::wstring(L" pitchFamily=\"") + std::to_wstring(pPF->bulletFontProperties->PitchFamily) + L"\"");
					}
					if ( pPF->bulletFontProperties->Charset > 0)
					{
						m_oWriter.WriteString(std::wstring(L" charset=\"") + std::to_wstring((char)pPF->bulletFontProperties->Charset) + L"\"");
					}
					m_oWriter.WriteString(std::wstring(L"/>"));
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
				
				if (!set)
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
					PPT_FORMAT::CTextCFRun* pCF = &pParagraph->m_arSpans[nSpan].m_oRun;
					if ((pCF->Size.is_init()) && (pCF->Size.get() > 0) && (pCF->Size.get() < 4001))
					{
                        m_oWriter.WriteString(L"<a:endParaRPr lang=\"en-US\" sz=\"" + std::to_wstring((int)(100 * pCF->Size.get())) + L"\"/>");
					}
					else
					{
						m_oWriter.WriteString(std::wstring(L"<a:endParaRPr lang=\"en-US\"/>"));
					}
					continue;
				}
			}

			PPT_FORMAT::CTextCFRun* pCF = &pParagraph->m_arSpans[nSpan].m_oRun;
			int span_sz = pParagraph->m_arSpans[nSpan].m_strText.length() ;

            if	((span_sz==1 && ( pParagraph->m_arSpans[nSpan].m_strText[0] == (wchar_t)13 )) ||
                ((span_sz==2 && ( pParagraph->m_arSpans[nSpan].m_strText[0] == (wchar_t)13 ) && ( pParagraph->m_arSpans[nSpan].m_strText[1] == (wchar_t)13 ))))
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
				if (pShapeElement->m_lPlaceholderType == PT_MasterSlideNumber/* && pParagraph->m_arSpans[nSpan].m_bField*/)
				{
					m_oWriter.WriteString(std::wstring(L"<a:fld id=\"{D038279B-FC19-497E-A7D1-5ADD9CAF016F}\" type=\"slidenum\"><a:rPr"));
				}
				else if (pShapeElement->m_lPlaceholderType == PT_MasterDate && pShapeElement->m_nFormatDate == 1/* && pParagraph->m_arSpans[nSpan].m_bField*/)
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
                m_oWriter.WriteString(L" sz=\"" + std::to_wstring((int)(100 * pCF->Size.get())) + L"\"");
			}		
			if (pCF->BaseLineOffset.is_init())
			{
                m_oWriter.WriteString(L" baseline=\"" + std::to_wstring((int)(1000 * pCF->BaseLineOffset.get())) + L"\"");
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
				if (pShapeElement->m_bLine)
				{
					m_oWriter.WriteString(ConvertLine(pShapeElement->m_oPen));
				}
				m_oWriter.WriteString(ConvertBrush(pShapeElement->m_oBrush));
				m_oWriter.WriteString(ConvertShadow(pShapeElement->m_oShadow));
			}
		else
			{
				if (pCF->Color.is_init())
				{
					if (pCF->Color->m_lSchemeIndex != -1)
					{
                        std::wstring strProp = _T("<a:solidFill><a:schemeClr val=\"") + CStylesWriter::GetColorInScheme(pCF->Color->m_lSchemeIndex) + _T("\"/></a:solidFill>");
						m_oWriter.WriteString(strProp);
					}
					else
					{
                        std::wstring strColor = XmlUtils::IntToString(pCF->Color->GetLONG_RGB(), L"%06x");

                        m_oWriter.WriteString(L"<a:solidFill><a:srgbClr val=\"" + strColor + L"\"/></a:solidFill>");
					}
				}
			}

			if (pCF->font.font.is_init())
			{
				m_oWriter.WriteString(std::wstring(L"<a:latin typeface=\"") + pCF->font.font->Name + _T("\"/>"));
			}
			else if (pCF->fontRef.is_init())
			{
				if ((m_pTheme) && (pCF->fontRef.get() < m_pTheme->m_arFonts.size()))
				{
					m_oWriter.WriteString(L"<a:latin typeface=\"" + m_pTheme->m_arFonts[pCF->fontRef.get()].Name + L"\"/>");
				}
				else
				{
					if (0 == pCF->fontRef.get())
					{
						m_oWriter.WriteString(L"<a:latin typeface=\"+mj-lt\"/>");
					}
					else
					{
						m_oWriter.WriteString(L"<a:latin typeface=\"+mn-lt\"/>");
					}
				}
			}

			if (pCF->font.ea.is_init())
			{
				m_oWriter.WriteString(std::wstring(L"<a:ea typeface=\"") + pCF->font.ea->Name + L"\"/>");
			}
			if (pCF->font.sym.is_init())
			{
				m_oWriter.WriteString(std::wstring(L"<a:sym typeface=\"") + pCF->font.sym->Name + _T("\"/>"));
			}
			m_oWriter.WriteString(std::wstring(L"</a:rPr>"));

			if (pParagraph->m_arSpans[nSpan].m_bBreak)
			{
				m_oWriter.WriteString(std::wstring(L"</a:br>"));
			}
			else
			{
                std::wstring strT1 = _T("<a:t>");
				m_oWriter.WriteString(strT1);

				std::wstring strT = pParagraph->m_arSpans[nSpan].m_strText;
				CorrectXmlString(strT);
				m_oWriter.WriteString(strT);

                std::wstring strT2 = _T("</a:t>");
				m_oWriter.WriteString(strT2);
				
				if ((pShapeElement->m_lPlaceholderType == PT_MasterSlideNumber || 
					(pShapeElement->m_lPlaceholderType == PT_MasterDate && pShapeElement->m_nFormatDate == 1))/* &&  pParagraph->m_arSpans[nSpan].m_bField*/)
				{
					m_oWriter.WriteString(std::wstring(L"</a:fld>"));
				}
				else
					m_oWriter.WriteString(std::wstring(L"</a:r>"));
			}
		}

        std::wstring strEndPar = _T("</a:p>");
		m_oWriter.WriteString(strEndPar);
	}

    std::wstring str5 = _T("</p:txBody>");
	m_oWriter.WriteString(str5);
}
std::wstring PPT_FORMAT::CShapeWriter::ConvertGroup()
{
	CGroupElement* pGroupElement = dynamic_cast<CGroupElement*>(m_pElement.get());
	if (!pGroupElement) return L"";

	m_oWriter.WriteString(std::wstring(L"<p:grpSp>"));

	WriteGroupInfo();
	
	m_pElement->NormalizeCoordsByMetric();

	m_oWriter.WriteString(std::wstring(L"<p:grpSpPr>"));

	if (pGroupElement->m_bChildAnchorEnabled || pGroupElement->m_bAnchorEnabled)
	{
        std::wstring str;
		
		m_oWriter.WriteString(std::wstring(L"<a:xfrm"));	
			if (0 != pGroupElement->m_dRotate)
			{
				if (pGroupElement->m_bFlipH && pGroupElement->m_dRotate < 180)
					pGroupElement->m_dRotate += 180; //Тікбұрышты үшбұрыштарды.ppt slide-7

                m_oWriter.WriteString(L" rot=\"" + std::to_wstring((int)(pGroupElement->m_dRotate * 60000)) + L"\"");
			}
			if (pGroupElement->m_bFlipH)
			{
				m_oWriter.WriteString(std::wstring(L" flipH=\"1\""));
			}
			if (pGroupElement->m_bFlipV)
			{
				m_oWriter.WriteString(std::wstring(L" flipV=\"1\""));
			}
		m_oWriter.WriteString(std::wstring(L">"));

        m_oWriter.WriteString(L"<a:off x=\"" + 
			std::to_wstring(pGroupElement->m_bChildAnchorEnabled ? (int)pGroupElement->m_rcChildAnchor.left : (int)pGroupElement->m_rcAnchor.left) 
			+ L"\" y=\"" + 
			std::to_wstring(pGroupElement->m_bChildAnchorEnabled ? (int)pGroupElement->m_rcChildAnchor.top : (int)pGroupElement->m_rcAnchor.top) +
			L"\"/>");

		double width	= pGroupElement->m_bChildAnchorEnabled ? pGroupElement->m_rcChildAnchor.GetWidth() : pGroupElement->m_rcAnchor.GetWidth();
		double height	= pGroupElement->m_bChildAnchorEnabled ? pGroupElement->m_rcChildAnchor.GetHeight() : pGroupElement->m_rcAnchor.GetHeight();

		if ( width > 0 || height > 0 )
		{
            m_oWriter.WriteString(L"<a:ext cx=\"" + std::to_wstring((int)width) + L"\" cy=\"" + std::to_wstring((int)height) + L"\"/>");
		}

		m_oWriter.WriteString(L"<a:chOff x=\"" + std::to_wstring((int)pGroupElement->m_rcGroupAnchor.left) + 
									L"\" y=\"" + std::to_wstring((int)pGroupElement->m_rcGroupAnchor.top) + L"\"/>");
		m_oWriter.WriteString(L"<a:chExt cx=\"" + std::to_wstring((int)pGroupElement->m_rcGroupAnchor.GetWidth()) + 
									L"\" cy=\"" + std::to_wstring((int)pGroupElement->m_rcGroupAnchor.GetHeight()) + L"\"/>");

		m_oWriter.WriteString(std::wstring(L"</a:xfrm>"));
	} 

	m_oWriter.WriteString(std::wstring(L"</p:grpSpPr>"));			

	pGroupElement = NULL;

	return m_oWriter.GetData();
}


std::wstring PPT_FORMAT::CShapeWriter::ConvertShape()
{
	CImageElement* pImageElement = dynamic_cast<CImageElement*>(m_pElement.get());
	CGroupElement* pGroupElement = dynamic_cast<CGroupElement*>(m_pElement.get());
	CShapeElement* pShapeElement = dynamic_cast<CShapeElement*>(m_pElement.get());
	
	if (pImageElement) return ConvertImage();
	if (pGroupElement) return ConvertGroup();

	if (pShapeElement == NULL) return _T("");

	std::wstring prstTxWarp;
	std::wstring prstGeom	= oox::Spt2ShapeType_mini((oox::MSOSPT)pShapeElement->m_lShapeType);
//rect, ellipse, line

	if (prstGeom.empty())
	{
		prstTxWarp = oox::Spt2WordArtShapeType((oox::MSOSPT)pShapeElement->m_lShapeType);
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
		if (oox::msosptTextBox == (oox::MSOSPT)pShapeElement->m_lShapeType)	
			m_bTextBox = true;
		if (oox::msosptLine == (oox::MSOSPT)pShapeElement->m_lShapeType)	
			pShapeElement->m_bLine = true;
	}

	if (pShapeElement->m_pShape && !pShapeElement->m_pShape->m_strXmlString.empty())
	{
		ParseXmlAlternative(pShapeElement->m_pShape->m_strXmlString);
	}

	m_oWriter.WriteString(std::wstring(L"<p:sp>"));

	WriteShapeInfo();

	CGeomShapeInfo oInfo;

 	oInfo.m_lOriginalWidth	= m_pElement->m_bChildAnchorEnabled ? (LONG)m_pElement->m_rcChildAnchor.GetWidth() : (LONG)m_pElement->m_rcAnchor.GetWidth();
    oInfo.m_lOriginalHeight	= m_pElement->m_bChildAnchorEnabled ? (LONG)m_pElement->m_rcChildAnchor.GetHeight() : (LONG)m_pElement->m_rcAnchor.GetHeight();

	m_pElement->NormalizeCoordsByMetric();
	oInfo.SetBounds(m_pElement->m_bChildAnchorEnabled ? m_pElement->m_rcChildAnchor : m_pElement->m_rcAnchor);
	
	oInfo.m_dRotate = pShapeElement->m_dRotate;
	oInfo.m_bFlipH	= pShapeElement->m_bFlipH;
	oInfo.m_bFlipV	= pShapeElement->m_bFlipV;

	m_oWriter.WriteString(std::wstring(L"<p:spPr>"));

	if (pShapeElement->m_bChildAnchorEnabled || pShapeElement->m_bAnchorEnabled)
	{
        std::wstring str;
		
		m_oWriter.WriteString(std::wstring(L"<a:xfrm"));	
			if (0 != pShapeElement->m_dRotate)
			{
				if (pShapeElement->m_bFlipH && pShapeElement->m_dRotate < 180)
					pShapeElement->m_dRotate += 180; //Тікбұрышты үшбұрыштарды.ppt slide-7

                m_oWriter.WriteString(L" rot=\"" + std::to_wstring((int)(pShapeElement->m_dRotate * 60000)) + L"\"");
			}
			if (pShapeElement->m_bFlipH)
			{
				m_oWriter.WriteString(std::wstring(L" flipH=\"1\""));
			}
			if (pShapeElement->m_bFlipV)
			{
				m_oWriter.WriteString(std::wstring(L" flipV=\"1\""));
			}
		m_oWriter.WriteString(std::wstring(L">"));

        m_oWriter.WriteString(L"<a:off x=\"" + 
			std::to_wstring(pShapeElement->m_bChildAnchorEnabled ? (int)pShapeElement->m_rcChildAnchor.left : (int)pShapeElement->m_rcAnchor.left) 
			+ L"\" y=\"" + 
			std::to_wstring(pShapeElement->m_bChildAnchorEnabled ? (int)pShapeElement->m_rcChildAnchor.top : (int)pShapeElement->m_rcAnchor.top) +
			L"\"/>");

		double width	= pShapeElement->m_bChildAnchorEnabled ? pShapeElement->m_rcChildAnchor.GetWidth() : pShapeElement->m_rcAnchor.GetWidth();
		double height	= pShapeElement->m_bChildAnchorEnabled ? pShapeElement->m_rcChildAnchor.GetHeight() : pShapeElement->m_rcAnchor.GetHeight();

		if ( width > 0 || height > 0 )
		{
            m_oWriter.WriteString(L"<a:ext cx=\"" + std::to_wstring((int)width) + L"\" cy=\"" + std::to_wstring((int)height) + L"\"/>");
		}
		m_oWriter.WriteString(std::wstring(L"</a:xfrm>"));
	}

    CBaseShapePtr shape = pShapeElement->m_pShape->getBaseShape();

	if (pShapeElement->m_pShape->m_lDrawType & c_ShapeDrawType_Graphic || shape->m_bCustomShape)
	{
		pShapeElement->m_pShape->ToRenderer(dynamic_cast<IRenderer*>(this), oInfo, 0.0, 1.0);
	}

	if (!m_xmlGeomAlternative.empty())
	{
		m_oWriter.WriteString(m_xmlGeomAlternative);
	}
	else
	{
		if ((prstGeom.empty() == false || pShapeElement->m_bShapePreset) && prstTxWarp.empty() && !shape->m_bCustomShape)
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
			m_oWriter.WriteString(pShapeElement->ConvertPPTShapeToPPTX());
		}
		else
		{
			//word art
			m_oWriter.WriteString(std::wstring(L"<a:prstGeom prst=\"rect\"/>"));
		}
	}

	if (!m_bWordArt)
	{
		m_oWriter.WriteString(ConvertBrush(pShapeElement->m_oBrush));
		if (pShapeElement->m_bLine)
		{
			m_oWriter.WriteString(ConvertLine(pShapeElement->m_oPen));
		}
		m_oWriter.WriteString(ConvertShadow(pShapeElement->m_oShadow));
	}
	Write3dShape();

	m_oWriter.WriteString(std::wstring(L"</p:spPr>"));			

	WriteTextInfo();
	m_oWriter.WriteString(std::wstring(L"</p:sp>"));

	pShapeElement = NULL;

	return m_oWriter.GetData();
}
void PPT_FORMAT::CShapeWriter::ParseXmlAlternative(const std::wstring & xml)
{
	XmlUtils::CXmlLiteReader oReader;

	if ( !oReader.FromString( xml ) )
		return;
	if ( !oReader.ReadNextNode() )
		return;

	std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

	nullable<PPTX::Logic::SpTreeElem> oElement;

	if ( L"graphicFrame" == sName || L"pic" == sName || L"sp" == sName || L"grpSp" == sName || L"cxnSp" == sName || L"AlternateContent" == sName
		|| L"spTree" )
	{
		oElement = oReader;
	}

	if (oElement.IsInit())
	{
		smart_ptr<PPTX::Logic::Shape> shape = oElement->GetElem().smart_dynamic_cast<PPTX::Logic::Shape>();
		if (shape.IsInit())
		{
			NSBinPptxRW::CXmlWriter writer(XMLWRITER_DOC_TYPE_PPTX);
			shape->spPr.Geometry.toXmlWriter(&writer);

			if (shape->spPr.scene3d.IsInit())
				shape->spPr.scene3d->toXmlWriter(&writer);
			
			if (shape->spPr.sp3d.IsInit())
				shape->spPr.sp3d->toXmlWriter(&writer);

			m_xmlGeomAlternative = writer.GetXmlString();

			writer.ClearNoAttack();
			if ((shape->txBody.IsInit()) && (shape->txBody->bodyPr.IsInit()) && (shape->txBody->bodyPr->prstTxWarp.IsInit()))
			{//только WordArt
				shape->txBody->toXmlWriter(&writer);
				m_xmlTxBodyAlternative = writer.GetXmlString();
			}
		}
		smart_ptr<PPTX::Logic::SpTree> groupShape = oElement->GetElem().smart_dynamic_cast<PPTX::Logic::SpTree>();
		if (groupShape.IsInit())
		{//smartArt			
			NSBinPptxRW::CXmlWriter writer(XMLWRITER_DOC_TYPE_PPTX);
			groupShape->toXmlWriter(&writer);

			m_xmlAlternative = writer.GetXmlString();
		}
	}
}


std::wstring PPT_FORMAT::CShapeWriter::ConvertImage()
{
	CImageElement* pImageElement = dynamic_cast<CImageElement*>(m_pElement.get());
	if (!pImageElement) return L"";

	if (pImageElement->m_bImagePresent == false)
	{
		if (pImageElement->m_sName.empty()) return _T("");
		//ppt_presentation.ppt - ссылка на файл на диске 

		pImageElement->m_strImageFileName.clear();
		pImageElement->m_bImagePresent = true;
	}
    std::wstring strRid;
	if (!pImageElement->m_strImageFileName.empty())
	{
		strRid = m_pRels->WriteImage(pImageElement->m_strImageFileName);
	}	
	else if (!pImageElement->m_sImageName.empty())
	{
		strRid = m_pRels->WriteHyperlinkImage(CorrectXmlString3(pImageElement->m_sImageName));
	}	
	
	if (strRid.empty()) return _T("");

	m_oWriter.WriteString(std::wstring(L"<p:pic>"));

	WriteImageInfo();

	CGeomShapeInfo oInfo;

	oInfo.m_lOriginalWidth	= m_pElement->m_bChildAnchorEnabled ? (LONG)m_pElement->m_rcChildAnchor.GetWidth() : (LONG)m_pElement->m_rcAnchor.GetWidth();
    oInfo.m_lOriginalHeight	= m_pElement->m_bChildAnchorEnabled ? (LONG)m_pElement->m_rcChildAnchor.GetHeight() : (LONG)m_pElement->m_rcAnchor.GetHeight();

	m_pElement->NormalizeCoordsByMetric();
	oInfo.SetBounds(m_pElement->m_bChildAnchorEnabled ? m_pElement->m_rcChildAnchor : m_pElement->m_rcAnchor);
	
	oInfo.m_dRotate = pImageElement->m_dRotate;
	oInfo.m_bFlipH	= pImageElement->m_bFlipH;
	oInfo.m_bFlipV	= pImageElement->m_bFlipV;

	m_oWriter.WriteString(std::wstring(L"<p:blipFill>"));
		
        std::wstring strWrite = _T("<a:blip r:embed=\"") + strRid + _T("\"/>");
		m_oWriter.WriteString(strWrite);

		m_oWriter.WriteString(std::wstring(L"<a:srcRect"));
		if (pImageElement->m_bCropEnabled)
		{//124-125 Q3 V2 Win File 3.ppt
            m_oWriter.WriteString(L" l=\"" + std::to_wstring((_INT64)(pImageElement->m_lcropFromLeft  *1.5)) + L"\"");
            m_oWriter.WriteString(L" t=\"" + std::to_wstring((_INT64)(pImageElement->m_lcropFromTop   *1.5)) + L"\"");
            m_oWriter.WriteString(L" r=\"" + std::to_wstring((_INT64)(pImageElement->m_lcropFromRight *1.5)) + L"\"");
            m_oWriter.WriteString(L" b=\"" + std::to_wstring((_INT64)(pImageElement->m_lcropFromBottom*1.5)) + L"\"");
		}
		m_oWriter.WriteString(std::wstring(L"/>"));
		
		if (pImageElement->m_bTile)
		{
			m_oWriter.WriteString(std::wstring(L"<a:tile/>"));
		}
		else if (pImageElement->m_bStretch)
		{
			m_oWriter.WriteString(std::wstring(L"<a:stretch><a:fillRect/></a:stretch>"));
		}

	m_oWriter.WriteString(std::wstring(L"</p:blipFill>"));

	m_oWriter.WriteString(std::wstring(L"<p:spPr>"));

	if (pImageElement->m_bChildAnchorEnabled || pImageElement->m_bAnchorEnabled)
	{
        std::wstring str;
		
		m_oWriter.WriteString(std::wstring(L"<a:xfrm"));	
			if (0 != pImageElement->m_dRotate)
			{
                m_oWriter.WriteString(L" rot=\"" + std::to_wstring((int)(pImageElement->m_dRotate * 60000)) + L"\"");
			}
			if (pImageElement->m_bFlipH)
			{
				m_oWriter.WriteString(std::wstring(L" flipH=\"1\""));
			}
			if (pImageElement->m_bFlipV)
			{
				m_oWriter.WriteString(std::wstring(L" flipV=\"1\""));
			}
		m_oWriter.WriteString(std::wstring(L">"));

        m_oWriter.WriteString(L"<a:off x=\"" + 
			std::to_wstring(pImageElement->m_bChildAnchorEnabled ? (int)pImageElement->m_rcChildAnchor.left : (int)pImageElement->m_rcAnchor.left) 
			+ L"\" y=\"" + 
			std::to_wstring(pImageElement->m_bChildAnchorEnabled ? (int)pImageElement->m_rcChildAnchor.top : (int)pImageElement->m_rcAnchor.top) +
			L"\"/>");

		double width	= pImageElement->m_bChildAnchorEnabled ? pImageElement->m_rcChildAnchor.GetWidth() : pImageElement->m_rcAnchor.GetWidth();
		double height	= pImageElement->m_bChildAnchorEnabled ? pImageElement->m_rcChildAnchor.GetHeight() : pImageElement->m_rcAnchor.GetHeight();
		
		if ( width > 0 || height > 0 )
		{
            m_oWriter.WriteString(L"<a:ext cx=\"" + std::to_wstring((int)width) + L"\" cy=\"" + std::to_wstring((int)height) + L"\"/>");
		}
		m_oWriter.WriteString(std::wstring(L"</a:xfrm>"));
	}
	m_oWriter.WriteString(std::wstring(L"<a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom>"));

	m_oWriter.WriteString(ConvertBrush(pImageElement->m_oBrush));

	if (pImageElement->m_bLine)
	{
		m_oWriter.WriteString(ConvertLine(pImageElement->m_oPen));
	}

	m_oWriter.WriteString(std::wstring(L"</p:spPr>"));			

	m_oWriter.WriteString(std::wstring(L"</p:pic>"));
	
	pImageElement = NULL;
	return m_oWriter.GetData();
}
HRESULT PPT_FORMAT::CShapeWriter::get_Type(LONG* lType)
{
	if (NULL == lType)
        return S_FALSE;

	*lType = c_nSVGConverter;
	return S_OK;
}
//-------- Функции для работы со страницей --------------------------------------------------
HRESULT PPT_FORMAT::CShapeWriter::NewPage()
{
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_Height(double* dHeight)
{
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_Height(const double& dHeight)
{


	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_Width(double* dWidth)
{
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_Width(const double& dWidth)
{
	return S_OK;
}

HRESULT PPT_FORMAT::CShapeWriter::get_DpiX(double* dDpiX)
{
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_DpiY(double* dDpiY)
{
	return S_OK;
}
// pen --------------------------------------------------------------------------------------
HRESULT PPT_FORMAT::CShapeWriter::SetPen(std::wstring bsXML)
{
    //m_oPen.FromXmlString((std::wstring)bsXML);
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_PenColor(LONG* lColor)
{
	*lColor = m_oPen.Color.GetLONG();
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_PenColor(const LONG&  lColor)
{
	BYTE lScheme = ((_UINT32)(lColor)) >> 24;

	if (0xFF != lScheme)
		m_oPen.Color.SetBGR(lColor);
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_PenAlpha(LONG* lAlpha)
{
	*lAlpha = m_oPen.Alpha;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_PenAlpha(const LONG& lAlpha)
{
	m_oPen.Alpha = lAlpha;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_PenSize(double* dSize)
{
	*dSize = m_oPen.Size;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_PenSize(const double& dSize)
{
	m_oPen.Size = (long)(dSize * 25.4 / 96.0);
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_PenDashStyle(BYTE* val)
{
	*val = m_oPen.DashStyle;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_PenDashStyle(const BYTE& val)
{
	m_oPen.DashStyle = val;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_PenLineStartCap(BYTE* val)
{
	*val = m_oPen.LineStartCap;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_PenLineStartCap(const BYTE& val)
{
	m_oPen.LineStartCap = val;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_PenLineEndCap(BYTE* val)
{
	*val = m_oPen.LineEndCap;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_PenLineEndCap(const BYTE& val)
{
	m_oPen.LineEndCap = val;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_PenLineJoin(BYTE* val)
{
	*val = m_oPen.LineJoin;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_PenLineJoin(const BYTE& val)
{
	m_oPen.LineJoin = val;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_PenDashOffset(double* val)
{
	*val = m_oPen.DashOffset;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_PenDashOffset(const double&  val)
{
	m_oPen.DashOffset = val;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_PenAlign(LONG* val)
{
	*val = m_oPen.Align;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_PenAlign(const LONG& val)
{
	m_oPen.Align = val;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_PenMiterLimit(double* val)
{
	*val = m_oPen.MiterLimit;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_PenMiterLimit(const double& val)
{
	m_oPen.MiterLimit = val;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::PenDashPattern(double* pPattern, LONG lCount)
{
	if (NULL != pPattern)
	{
		m_oPen.SetDashPattern(pPattern, lCount);
	}

	return S_OK;
}
// brush ------------------------------------------------------------------------------------
HRESULT PPT_FORMAT::CShapeWriter::SetBrush(std::wstring bsXML)
{
    //m_oBrush.FromXmlString((std::wstring)bsXML);
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_BrushType(LONG* lType)
{
	*lType = m_oBrush.Type;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_BrushType(const LONG& lType)
{
	m_oBrush.Type = lType;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_BrushColor1(LONG* lColor)
{
	*lColor = m_oBrush.Color1.GetLONG();
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_BrushColor1(const LONG& lColor)
{
	BYTE lScheme = ((_UINT32)(lColor)) >> 24;

	if (0xFF != lScheme)
		m_oBrush.Color1.SetBGR(lColor);
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_BrushAlpha1(LONG* lAlpha)
{
	*lAlpha = m_oBrush.Alpha1;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_BrushAlpha1(const LONG& lAlpha)
{
	m_oBrush.Alpha1 = lAlpha;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_BrushColor2(LONG* lColor)
{
	*lColor = m_oBrush.Color2.GetLONG();
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_BrushColor2(const LONG& lColor)
{
	m_oBrush.Color2.SetBGR(lColor);
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_BrushAlpha2(LONG* lAlpha)
{
	*lAlpha = m_oBrush.Alpha2;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_BrushAlpha2(const LONG& lAlpha)
{
	m_oBrush.Alpha2 = lAlpha;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_BrushTexturePath(std::wstring* bsPath)
{
	if (bsPath == NULL) return S_OK;
	*bsPath = m_oBrush.TexturePath;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_BrushTexturePath(const std::wstring& bsPath)
{
	m_oBrush.TexturePath = bsPath;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_BrushTextureMode(LONG* lMode)
{
	*lMode = m_oBrush.TextureMode;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_BrushTextureMode(const LONG& lMode)
{
	m_oBrush.TextureMode = lMode;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_BrushTextureAlpha(LONG* lTxAlpha)
{
	*lTxAlpha = m_oBrush.TextureAlpha;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_BrushTextureAlpha(const LONG& lTxAlpha)
{
	m_oBrush.TextureAlpha = lTxAlpha;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_BrushLinearAngle(double* dAngle)
{
	*dAngle = m_oBrush.LinearAngle;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_BrushLinearAngle(const double& dAngle)
{
	m_oBrush.LinearAngle = dAngle;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::BrushRect(const INT& val, const double& left, const double& top, const double& width, const double& height)
{
	m_oBrush.Rectable = val;
	m_oBrush.Rect.X = (float)left;
	m_oBrush.Rect.Y = (float)top;
	m_oBrush.Rect.Width  = (float)width;
	m_oBrush.Rect.Height = (float)height;	

	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::BrushBounds(const double& left, const double& top, const double& width, const double& height)
{
	return S_OK;
}

HRESULT PPT_FORMAT::CShapeWriter::put_BrushGradientColors(LONG* lColors, double* pPositions, LONG nCount)
{

	return S_OK;
}
// font -------------------------------------------------------------------------------------
HRESULT PPT_FORMAT::CShapeWriter::SetFont(std::wstring bsXML)
{
    //m_oFont.FromXmlString((std::wstring)bsXML);
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_FontName(std::wstring* bsName)
{
	if (bsName == NULL) return S_OK;
	*bsName = m_oFont.Name;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_FontName(const std::wstring& bsName)
{
	m_oFont.Name = bsName;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_FontPath(std::wstring* bsName)
{
	if (bsName == NULL) return S_OK;
	*bsName = m_oFont.Path;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_FontPath(const std::wstring& bsName)
{
	m_oFont.Path = bsName;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_FontSize(double* dSize)
{
	*dSize = m_oFont.Size;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_FontSize(const double& dSize)
{
	m_oFont.Size = dSize;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_FontStyle(LONG* lStyle)
{
	*lStyle = m_oFont.GetStyle();
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_FontStyle(const LONG& lStyle)
{
	m_oFont.SetStyle(lStyle);
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_FontStringGID(INT* bGID)
{
	*bGID = m_oFont.StringGID;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_FontStringGID(const INT& bGID)
{
	m_oFont.StringGID = bGID;
	m_pFontManager->SetStringGID(bGID);
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_FontCharSpace(double* dSpace)
{
	*dSpace = m_oFont.CharSpace;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_FontCharSpace(const double& dSpace)
{
	m_oFont.CharSpace = dSpace;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_FontFaceIndex(int* lFaceIndex)
{
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_FontFaceIndex(const int& lFaceIndex)
{
	return S_OK;
}
// shadow -----------------------------------------------------------------------------------
HRESULT PPT_FORMAT::CShapeWriter::SetShadow(std::wstring bsXML)
{
    //m_oShadow.FromXmlString((std::wstring)bsXML);
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_ShadowDistanceX(double* val)
{
	*val = m_oShadow.DistanceX;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_ShadowDistanceX(double val)
{
	m_oShadow.DistanceX = val;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_ShadowDistanceY(double* val)
{
	*val = m_oShadow.DistanceY;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_ShadowDistanceY(double val)
{
	m_oShadow.DistanceY = val;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_ShadowBlurSize(double* val)
{
	*val = m_oShadow.BlurSize;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_ShadowBlurSize(double val)
{
	m_oShadow.BlurSize = val;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_ShadowColor(LONG* val)
{
	*val = m_oShadow.Color.GetLONG();
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_ShadowColor(LONG val)
{
	m_oShadow.Color.SetBGR(val);
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_ShadowAlpha(LONG* val)
{
	*val = m_oShadow.Alpha;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_ShadowAlpha(LONG val)
{
	m_oShadow.Alpha = val;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_ShadowVisible(bool* val)
{
	*val = m_oShadow.Visible;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_ShadowVisible(bool val)
{
	m_oShadow.Visible = val;
	return S_OK;
}
// edge -------------------------------------------------------------------------------------
HRESULT PPT_FORMAT::CShapeWriter::SetEdgeText(std::wstring bsXML)
{
    //m_oEdge.FromXmlString((std::wstring)bsXML);
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_EdgeVisible(LONG* val)
{
	*val = m_oEdge.Visible;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_EdgeVisible(LONG val)
{
	m_oEdge.Visible = val;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_EdgeColor(LONG* val)
{
	*val = m_oEdge.Color.GetLONG();
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_EdgeColor(LONG val)
{
	m_oEdge.Color.SetBGR(val);
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_EdgeAlpha(LONG* val)
{
	*val = m_oEdge.Alpha;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_EdgeAlpha(LONG val)
{
	m_oEdge.Alpha = val;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::get_EdgeDist(double* val)
{
	*val = m_oEdge.Dist;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_EdgeDist(double val)
{
	m_oEdge.Dist = val;
	return S_OK;
}
//-------- Функции для вывода текста --------------------------------------------------------
HRESULT PPT_FORMAT::CShapeWriter::CommandDrawText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h)
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
HRESULT PPT_FORMAT::CShapeWriter::CommandDrawTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h)
{

	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::CommandDrawTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h)
{

	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::PathCommandTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h)
{

	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::PathCommandTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h)
{

	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::CommandLong(const LONG& lType, const LONG& lCommand)
{

	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::CommandDouble(const LONG& lType, const double& dCommand)
{

	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::CommandString(const LONG& lType, const std::wstring& sCommand)
{

	return S_OK;
}

HRESULT PPT_FORMAT::CShapeWriter::CommandDrawTextEx(const std::wstring& bsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h)
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
HRESULT PPT_FORMAT::CShapeWriter::BeginCommand(const _UINT32& lType)
{
	if (c_nPathType == lType)
	{
		m_pSimpleGraphicsConverter->PathCommandEnd();
	}

	m_lCurrentCommandType = lType;

	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::EndCommand(const _UINT32& lType)
{
	m_lCurrentCommandType = -1;
	return S_OK;
}
//-------- Функции для работы с Graphics Path -----------------------------------------------
HRESULT PPT_FORMAT::CShapeWriter::PathCommandMoveTo(const double& x, const double& y)
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
HRESULT PPT_FORMAT::CShapeWriter::PathCommandLineTo(const double& x, const double& y)
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
HRESULT PPT_FORMAT::CShapeWriter::PathCommandLinesTo(double* points, const int& count)
{
	m_pSimpleGraphicsConverter->PathCommandLinesTo(points, count);
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::PathCommandCurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3)
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
HRESULT PPT_FORMAT::CShapeWriter::PathCommandCurvesTo(double* points, const int& count)
{
	m_pSimpleGraphicsConverter->PathCommandCurvesTo(points, count);
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::PathCommandArcTo(const double& x, const double& y, const double& w, const double& h, const double& startAngle, const double& sweepAngle)
{
	m_pSimpleGraphicsConverter->PathCommandArcTo(x, y, w, h, startAngle, sweepAngle);
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::PathCommandClose()
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
HRESULT PPT_FORMAT::CShapeWriter::PathCommandEnd()
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
HRESULT PPT_FORMAT::CShapeWriter::DrawPath(const LONG& nType)
{
	bool bIsStroke	= ((0x01 == (nType & 0x01)) && (0 != m_oPen.Alpha));
	bool bIsFill	= ((0xFF < nType) && (0 != m_oBrush.Alpha1));

	if (bIsStroke && bIsFill)
	{
        m_oWriterVML.WriteString(L"<a:path w=\"43200\" h=\"43200\">");
		m_oWriterVML.Write(m_oWriterPath);
		m_oWriterVML.WriteString(std::wstring(L"</a:path>"));
	}
	else if (bIsStroke)
	{
        m_oWriterVML.WriteString(L"<a:path w=\"43200\" h=\"43200\" fill=\"none\">");
		m_oWriterVML.Write(m_oWriterPath);
		m_oWriterVML.WriteString(std::wstring(L"</a:path>"));
	}
	else if (bIsFill)
	{
        m_oWriterVML.WriteString(L"<a:path w=\"43200\" h=\"43200\" stroke=\"0\">");
		m_oWriterVML.Write(m_oWriterPath);
		m_oWriterVML.WriteString(std::wstring(L"</a:path>"));
	}

	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::PathCommandStart()
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
HRESULT PPT_FORMAT::CShapeWriter::PathCommandGetCurrentPoint(double* fX, double* fY)
{
	m_pSimpleGraphicsConverter->PathCommandGetCurrentPoint(fX, fY);	
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::PathCommandText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h)
{
	_SetFont();
	m_pSimpleGraphicsConverter->PathCommandText(bsText, m_pFontManager, x, y, w, h, 0);
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::PathCommandTextEx(const std::wstring& bsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h)
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
HRESULT PPT_FORMAT::CShapeWriter::DrawImage(IGrObject* pImage, const double& x, const double& y, const double& w, const double& h)
{
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::DrawImageFromFile(const std::wstring&, const double& x, const double& y, const double& w, const double& h, const BYTE& lAlpha)
{
	return S_OK;
}
// transform --------------------------------------------------------------------------------
HRESULT PPT_FORMAT::CShapeWriter::GetCommandParams(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, _UINT32* lFlags)
{
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, _UINT32 lFlags)
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
HRESULT PPT_FORMAT::CShapeWriter::SetTransform(const double& m1, const double& m2, const double& m3, const double& m4, const double& m5, const double& m6)
{
	Aggplus::CMatrix oTrans(m1, m2, m3, m4, m5, m6);
	m_oTransform = oTrans;

	CalculateFullTransform();

	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
{
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::ResetTransform(void)
{
	m_oTransform.Reset();
	CalculateFullTransform();
	return S_OK;
}
// -----------------------------------------------------------------------------------------
HRESULT PPT_FORMAT::CShapeWriter::get_ClipMode(LONG* plMode)
{
	*plMode = m_lClipMode;
	return S_OK;
}
HRESULT PPT_FORMAT::CShapeWriter::put_ClipMode(const LONG& lMode)
{
	m_lClipMode = lMode;
	return S_OK;
}