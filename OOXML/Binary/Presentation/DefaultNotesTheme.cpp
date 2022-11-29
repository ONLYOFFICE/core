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

#include "DefaultNotesTheme.h"

namespace Writers
{
		DefaultNotesThemeWriter::DefaultNotesThemeWriter()
		{
		}
		void DefaultNotesThemeWriter::Write(std::wstring fileName)
		{
			std::wstring s_Common = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");

	s_Common += _T("<a:theme xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" name=\"Office Theme\">\
<a:themeElements>\
<a:clrScheme name=\"\">\
<a:dk1>\
<a:srgbClr val=\"000000\"/>\
</a:dk1>\
<a:lt1>\
<a:srgbClr val=\"FFFFFF\"/>\
</a:lt1>\
<a:dk2>\
<a:srgbClr val=\"000000\"/>\
</a:dk2>\
<a:lt2>\
<a:srgbClr val=\"808080\"/>\
</a:lt2>\
<a:accent1>\
<a:srgbClr val=\"BBE0E3\"/>\
</a:accent1>\
<a:accent2>\
<a:srgbClr val=\"333399\"/>\
</a:accent2>\
<a:accent3>\
<a:srgbClr val=\"FFFFFF\"/>\
</a:accent3>\
<a:accent4>\
<a:srgbClr val=\"000000\"/>\
</a:accent4>\
<a:accent5>\
<a:srgbClr val=\"DAEDEF\"/>\
</a:accent5>\
<a:accent6>\
<a:srgbClr val=\"2D2D8A\"/>\
</a:accent6>\
<a:hlink>\
<a:srgbClr val=\"009999\"/>\
</a:hlink>\
<a:folHlink>\
<a:srgbClr val=\"99CC00\"/>\
</a:folHlink>\
</a:clrScheme>\
<a:fontScheme name=\"Office\">\
<a:majorFont>\
<a:latin typeface=\"Calibri\"/>\
<a:ea typeface=\"\"/>\
<a:cs typeface=\"\"/>\
<a:font script=\"Jpan\" typeface=\"ＭＳ Ｐゴシック\"/>\
<a:font script=\"Hang\" typeface=\"맑은 고딕\"/>\
<a:font script=\"Hans\" typeface=\"宋体\"/>\
<a:font script=\"Hant\" typeface=\"新細明體\"/>\
<a:font script=\"Arab\" typeface=\"Times New Roman\"/>\
<a:font script=\"Hebr\" typeface=\"Times New Roman\"/>\
<a:font script=\"Thai\" typeface=\"Angsana New\"/>\
<a:font script=\"Ethi\" typeface=\"Nyala\"/>\
<a:font script=\"Beng\" typeface=\"Vrinda\"/>\
<a:font script=\"Gujr\" typeface=\"Shruti\"/>\
<a:font script=\"Khmr\" typeface=\"MoolBoran\"/>\
<a:font script=\"Knda\" typeface=\"Tunga\"/>\
<a:font script=\"Guru\" typeface=\"Raavi\"/>\
<a:font script=\"Cans\" typeface=\"Euphemia\"/>\
<a:font script=\"Cher\" typeface=\"Plantagenet Cherokee\"/>\
<a:font script=\"Yiii\" typeface=\"Microsoft Yi Baiti\"/>\
<a:font script=\"Tibt\" typeface=\"Microsoft Himalaya\"/>\
<a:font script=\"Thaa\" typeface=\"MV Boli\"/>\
<a:font script=\"Deva\" typeface=\"Mangal\"/>\
<a:font script=\"Telu\" typeface=\"Gautami\"/>\
<a:font script=\"Taml\" typeface=\"Latha\"/>\
<a:font script=\"Syrc\" typeface=\"Estrangelo Edessa\"/>\
<a:font script=\"Orya\" typeface=\"Kalinga\"/>\
<a:font script=\"Mlym\" typeface=\"Kartika\"/>\
<a:font script=\"Laoo\" typeface=\"DokChampa\"/>\
<a:font script=\"Sinh\" typeface=\"Iskoola Pota\"/>\
<a:font script=\"Mong\" typeface=\"Mongolian Baiti\"/>\
<a:font script=\"Viet\" typeface=\"Times New Roman\"/>\
<a:font script=\"Uigh\" typeface=\"Microsoft Uighur\"/>\
</a:majorFont>");
	s_Common += _T("<a:minorFont>\
<a:latin typeface=\"Calibri\"/>\
<a:ea typeface=\"\"/>\
<a:cs typeface=\"\"/>\
<a:font script=\"Jpan\" typeface=\"ＭＳ Ｐゴシック\"/>\
<a:font script=\"Hang\" typeface=\"맑은 고딕\"/>\
<a:font script=\"Hans\" typeface=\"宋体\"/>\
<a:font script=\"Hant\" typeface=\"新細明體\"/>\
<a:font script=\"Arab\" typeface=\"Arial\"/>\
<a:font script=\"Hebr\" typeface=\"Arial\"/>\
<a:font script=\"Thai\" typeface=\"Cordia New\"/>\
<a:font script=\"Ethi\" typeface=\"Nyala\"/>\
<a:font script=\"Beng\" typeface=\"Vrinda\"/>\
<a:font script=\"Gujr\" typeface=\"Shruti\"/>\
<a:font script=\"Khmr\" typeface=\"DaunPenh\"/>\
<a:font script=\"Knda\" typeface=\"Tunga\"/>\
<a:font script=\"Guru\" typeface=\"Raavi\"/>\
<a:font script=\"Cans\" typeface=\"Euphemia\"/>\
<a:font script=\"Cher\" typeface=\"Plantagenet Cherokee\"/>\
<a:font script=\"Yiii\" typeface=\"Microsoft Yi Baiti\"/>\
<a:font script=\"Tibt\" typeface=\"Microsoft Himalaya\"/>\
<a:font script=\"Thaa\" typeface=\"MV Boli\"/>\
<a:font script=\"Deva\" typeface=\"Mangal\"/>\
<a:font script=\"Telu\" typeface=\"Gautami\"/>\
<a:font script=\"Taml\" typeface=\"Latha\"/>\
<a:font script=\"Syrc\" typeface=\"Estrangelo Edessa\"/>\
<a:font script=\"Orya\" typeface=\"Kalinga\"/>\
<a:font script=\"Mlym\" typeface=\"Kartika\"/>\
<a:font script=\"Laoo\" typeface=\"DokChampa\"/>\
<a:font script=\"Sinh\" typeface=\"Iskoola Pota\"/>\
<a:font script=\"Mong\" typeface=\"Mongolian Baiti\"/>\
<a:font script=\"Viet\" typeface=\"Arial\"/>\
<a:font script=\"Uigh\" typeface=\"Microsoft Uighur\"/>\
</a:minorFont>\
</a:fontScheme>\
<a:fmtScheme name=\"Office\">\
<a:fillStyleLst>\
<a:solidFill>\
<a:schemeClr val=\"phClr\"/>\
</a:solidFill>");
	 s_Common += _T("<a:gradFill rotWithShape=\"1\">\
<a:gsLst>\
<a:gs pos=\"0\">\
<a:schemeClr val=\"phClr\">\
<a:tint val=\"50000\"/>\
<a:satMod val=\"300000\"/>\
</a:schemeClr>\
</a:gs>\
<a:gs pos=\"35000\">\
<a:schemeClr val=\"phClr\">\
<a:tint val=\"37000\"/>\
<a:satMod val=\"300000\"/>\
</a:schemeClr>\
</a:gs>\
<a:gs pos=\"100000\">\
<a:schemeClr val=\"phClr\">\
<a:tint val=\"15000\"/>\
<a:satMod val=\"350000\"/>\
</a:schemeClr>\
</a:gs>\
</a:gsLst>\
<a:lin ang=\"16200000\" scaled=\"1\"/>\
</a:gradFill>\
<a:gradFill rotWithShape=\"1\">\
<a:gsLst>\
<a:gs pos=\"0\">\
<a:schemeClr val=\"phClr\">\
<a:shade val=\"51000\"/>\
<a:satMod val=\"130000\"/>\
</a:schemeClr>\
</a:gs>\
<a:gs pos=\"80000\">\
<a:schemeClr val=\"phClr\">\
<a:shade val=\"93000\"/>\
<a:satMod val=\"130000\"/>\
</a:schemeClr>\
</a:gs>\
<a:gs pos=\"100000\">\
<a:schemeClr val=\"phClr\">\
<a:shade val=\"94000\"/>\
<a:satMod val=\"135000\"/>\
</a:schemeClr>\
</a:gs>\
</a:gsLst>\
<a:lin ang=\"16200000\" scaled=\"0\"/>\
</a:gradFill>\
</a:fillStyleLst>\
<a:lnStyleLst>\
<a:ln w=\"9525\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\">\
<a:solidFill>");
	 s_Common += _T("<a:schemeClr val=\"phClr\">\
<a:shade val=\"95000\"/>\
<a:satMod val=\"105000\"/>\
</a:schemeClr>\
</a:solidFill>\
<a:prstDash val=\"solid\"/>\
</a:ln>\
<a:ln w=\"25400\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\">\
<a:solidFill>\
<a:schemeClr val=\"phClr\"/>\
</a:solidFill>\
<a:prstDash val=\"solid\"/>\
</a:ln>\
<a:ln w=\"38100\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\">\
<a:solidFill>\
<a:schemeClr val=\"phClr\"/>\
</a:solidFill>\
<a:prstDash val=\"solid\"/>\
</a:ln>\
</a:lnStyleLst>\
<a:effectStyleLst>\
<a:effectStyle>\
<a:effectLst>\
<a:outerShdw blurRad=\"40000\" dist=\"20000\" dir=\"5400000\" rotWithShape=\"0\">\
<a:srgbClr val=\"000000\">\
<a:alpha val=\"38000\"/>\
</a:srgbClr>\
</a:outerShdw>\
</a:effectLst>\
</a:effectStyle>\
<a:effectStyle>\
<a:effectLst>\
<a:outerShdw blurRad=\"40000\" dist=\"23000\" dir=\"5400000\" rotWithShape=\"0\">\
<a:srgbClr val=\"000000\">\
<a:alpha val=\"35000\"/>\
</a:srgbClr>\
</a:outerShdw>\
</a:effectLst>\
</a:effectStyle>\
<a:effectStyle>");
	 s_Common += _T("<a:effectLst>\
<a:outerShdw blurRad=\"40000\" dist=\"23000\" dir=\"5400000\" rotWithShape=\"0\">\
<a:srgbClr val=\"000000\">\
<a:alpha val=\"35000\"/>\
</a:srgbClr>\
</a:outerShdw>\
</a:effectLst>\
<a:scene3d>\
<a:camera prst=\"orthographicFront\">\
<a:rot lat=\"0\" lon=\"0\" rev=\"0\"/>\
</a:camera>\
<a:lightRig rig=\"threePt\" dir=\"t\">\
<a:rot lat=\"0\" lon=\"0\" rev=\"1200000\"/>\
</a:lightRig>\
</a:scene3d>\
<a:sp3d>\
<a:bevelT w=\"63500\" h=\"25400\"/>\
</a:sp3d>\
</a:effectStyle>\
</a:effectStyleLst>\
<a:bgFillStyleLst>\
<a:solidFill>\
<a:schemeClr val=\"phClr\"/>\
</a:solidFill>\
<a:gradFill rotWithShape=\"1\">\
<a:gsLst>\
<a:gs pos=\"0\">\
<a:schemeClr val=\"phClr\">\
<a:tint val=\"40000\"/>\
<a:satMod val=\"350000\"/>\
</a:schemeClr>\
</a:gs>\
<a:gs pos=\"40000\">\
<a:schemeClr val=\"phClr\">\
<a:tint val=\"45000\"/>\
<a:shade val=\"99000\"/>\
<a:satMod val=\"350000\"/>\
</a:schemeClr>\
</a:gs>\
<a:gs pos=\"100000\">\
<a:schemeClr val=\"phClr\">\
<a:shade val=\"20000\"/>\
<a:satMod val=\"255000\"/>\
</a:schemeClr>\
</a:gs>\
</a:gsLst>\
<a:path path=\"circle\">\
<a:fillToRect l=\"50000\" t=\"-80000\" r=\"50000\" b=\"180000\"/>\
</a:path>\
</a:gradFill>\
<a:gradFill rotWithShape=\"1\">\
<a:gsLst>\
<a:gs pos=\"0\">\
<a:schemeClr val=\"phClr\">\
<a:tint val=\"80000\"/>\
<a:satMod val=\"300000\"/>\
</a:schemeClr>\
</a:gs>\
<a:gs pos=\"100000\">\
<a:schemeClr val=\"phClr\">\
<a:shade val=\"30000\"/>\
<a:satMod val=\"200000\"/>\
</a:schemeClr>\
</a:gs>\
</a:gsLst>\
<a:path path=\"circle\">\
<a:fillToRect l=\"50000\" t=\"50000\" r=\"50000\" b=\"50000\"/>\
</a:path>\
</a:gradFill>\
</a:bgFillStyleLst>\
</a:fmtScheme>\
</a:themeElements>\
<a:objectDefaults/>\
<a:extraClrSchemeLst/>\
</a:theme>");

			OOX::CPath pathFile= fileName;

			NSFile::CFileBinary oFile;
			oFile.CreateFile(pathFile.GetPath());
			oFile.WriteStringUTF8(s_Common);
			oFile.CloseFile();
		}
}

