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

#include "DefaultNotesMaster.h"

namespace Writers
{
		DefaultNotesMasterWriter::DefaultNotesMasterWriter()
		{
		}
		void DefaultNotesMasterWriter::Write(std::wstring fileName)
		{
			std::wstring s_Common = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");

	s_Common += _T("<p:notesMaster xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\">\
<p:cSld>\
<p:bg>\
<p:bgPr>\
<a:solidFill>\
<a:schemeClr val=\"bg1\"/>\
</a:solidFill>\
<a:effectLst/>\
</p:bgPr>\
</p:bg>\
<p:spTree>\
<p:nvGrpSpPr>\
<p:cNvPr id=\"1\" name=\"\"/>\
<p:cNvGrpSpPr/>\
<p:nvPr/>\
</p:nvGrpSpPr>\
<p:grpSpPr>\
<a:xfrm>\
<a:off x=\"0\" y=\"0\"/>\
<a:ext cx=\"0\" cy=\"0\"/>\
<a:chOff x=\"0\" y=\"0\"/>\
<a:chExt cx=\"0\" cy=\"0\"/>\
</a:xfrm>\
</p:grpSpPr>\
<p:sp>\
<p:nvSpPr>\
<p:cNvPr id=\"11271\" name=\"Rectangle 7\"/>\
<p:cNvSpPr>\
<a:spLocks noGrp=\"1\" noChangeArrowheads=\"1\"/>\
</p:cNvSpPr>\
<p:nvPr>\
<p:ph type=\"body\" idx=\"1\"/>\
</p:nvPr>\
</p:nvSpPr>\
<p:spPr bwMode=\"auto\">\
<a:xfrm>\
<a:off x=\"3884613\" y=\"8685213\"/>\
<a:ext cx=\"2971800\" cy=\"457200\"/>\
</a:xfrm>\
<a:prstGeom prst=\"rect\">\
<a:avLst/>\
</a:prstGeom>\
<a:extLst/>\
</p:spPr>\
<p:txBody>\
<a:bodyPr/>\
<a:lstStyle>\
<a:lvl1pPr algn=\"l\">\
<a:defRPr sz=\"1200\">\
<a:latin typeface=\"Arial\" charset=\"0\"/>\
</a:defRPr>\
</a:lvl1pPr>\
</a:lstStyle>\
<a:p>\
<a:endParaRPr/>\
</a:p>\
</p:txBody>\
</p:sp>\
</p:spTree>\
</p:cSld>\
<p:clrMap bg1=\"lt1\" tx1=\"dk1\" bg2=\"lt2\" tx2=\"dk2\" accent1=\"accent1\" accent2=\"accent2\" accent3=\"accent3\" accent4=\"accent4\" accent5=\"accent5\" accent6=\"accent6\" hlink=\"hlink\" folHlink=\"folHlink\"/>\
<p:notesStyle>\
<a:lvl1pPr algn=\"l\" rtl=\"0\" fontAlgn=\"base\">\
<a:spcBef>\
<a:spcPct val=\"30000\"/>\
</a:spcBef>\
<a:spcAft>\
<a:spcPct val=\"0\"/>\
</a:spcAft>\
<a:defRPr sz=\"1200\" kern=\"1200\">\
<a:solidFill>\
<a:schemeClr val=\"tx1\"/>\
</a:solidFill>\
<a:latin typeface=\"Arial\" charset=\"0\"/>\
<a:ea typeface=\"+mn-ea\"/>\
<a:cs typeface=\"+mn-cs\"/>\
</a:defRPr>\
</a:lvl1pPr>\
<a:lvl2pPr marL=\"457200\" algn=\"l\" rtl=\"0\" fontAlgn=\"base\">\
<a:spcBef>\
<a:spcPct val=\"30000\"/>\
</a:spcBef>\
<a:spcAft>\
<a:spcPct val=\"0\"/>\
</a:spcAft>\
<a:defRPr sz=\"1200\" kern=\"1200\">\
<a:solidFill>\
<a:schemeClr val=\"tx1\"/>\
</a:solidFill>\
<a:latin typeface=\"Arial\" charset=\"0\"/>\
<a:ea typeface=\"+mn-ea\"/>\
<a:cs typeface=\"+mn-cs\"/>\
</a:defRPr>\
</a:lvl2pPr>\
<a:lvl3pPr marL=\"914400\" algn=\"l\" rtl=\"0\" fontAlgn=\"base\">\
<a:spcBef>\
<a:spcPct val=\"30000\"/>\
</a:spcBef>\
<a:spcAft>\
<a:spcPct val=\"0\"/>\
</a:spcAft>\
<a:defRPr sz=\"1200\" kern=\"1200\">\
<a:solidFill>\
<a:schemeClr val=\"tx1\"/>\
</a:solidFill>\
<a:latin typeface=\"Arial\" charset=\"0\"/>\
<a:ea typeface=\"+mn-ea\"/>\
<a:cs typeface=\"+mn-cs\"/>\
</a:defRPr>\
</a:lvl3pPr>\
<a:lvl4pPr marL=\"1371600\" algn=\"l\" rtl=\"0\" fontAlgn=\"base\">\
<a:spcBef>\
<a:spcPct val=\"30000\"/>\
</a:spcBef>\
<a:spcAft>\
<a:spcPct val=\"0\"/>\
</a:spcAft>\
<a:defRPr sz=\"1200\" kern=\"1200\">\
<a:solidFill>\
<a:schemeClr val=\"tx1\"/>\
</a:solidFill>\
<a:latin typeface=\"Arial\" charset=\"0\"/>\
<a:ea typeface=\"+mn-ea\"/>\
<a:cs typeface=\"+mn-cs\"/>\
</a:defRPr>\
</a:lvl4pPr>\
<a:lvl5pPr marL=\"1828800\" algn=\"l\" rtl=\"0\" fontAlgn=\"base\">\
<a:spcBef>\
<a:spcPct val=\"30000\"/>\
</a:spcBef>\
<a:spcAft>\
<a:spcPct val=\"0\"/>\
</a:spcAft>\
<a:defRPr sz=\"1200\" kern=\"1200\">\
<a:solidFill>\
<a:schemeClr val=\"tx1\"/>\
</a:solidFill>\
<a:latin typeface=\"Arial\" charset=\"0\"/>\
<a:ea typeface=\"+mn-ea\"/>\
<a:cs typeface=\"+mn-cs\"/>\
</a:defRPr>\
</a:lvl5pPr>\
<a:lvl6pPr marL=\"2286000\" algn=\"l\" defTabSz=\"914400\" rtl=\"0\" eaLnBrk=\"1\" latinLnBrk=\"0\" hangingPunct=\"1\">\
<a:defRPr sz=\"1200\" kern=\"1200\">\
<a:solidFill>\
<a:schemeClr val=\"tx1\"/>\
</a:solidFill>\
<a:latin typeface=\"+mn-lt\"/>\
<a:ea typeface=\"+mn-ea\"/>\
<a:cs typeface=\"+mn-cs\"/>\
</a:defRPr>\
</a:lvl6pPr>\
<a:lvl7pPr marL=\"2743200\" algn=\"l\" defTabSz=\"914400\" rtl=\"0\" eaLnBrk=\"1\" latinLnBrk=\"0\" hangingPunct=\"1\">\
<a:defRPr sz=\"1200\" kern=\"1200\">\
<a:solidFill>\
<a:schemeClr val=\"tx1\"/>\
</a:solidFill>\
<a:latin typeface=\"+mn-lt\"/>\
<a:ea typeface=\"+mn-ea\"/>\
<a:cs typeface=\"+mn-cs\"/>\
</a:defRPr>\
</a:lvl7pPr>\
<a:lvl8pPr marL=\"3200400\" algn=\"l\" defTabSz=\"914400\" rtl=\"0\" eaLnBrk=\"1\" latinLnBrk=\"0\" hangingPunct=\"1\">\
<a:defRPr sz=\"1200\" kern=\"1200\">\
<a:solidFill>\
<a:schemeClr val=\"tx1\"/>\
</a:solidFill>\
<a:latin typeface=\"+mn-lt\"/>\
<a:ea typeface=\"+mn-ea\"/>\
<a:cs typeface=\"+mn-cs\"/>\
</a:defRPr>\
</a:lvl8pPr>\
<a:lvl9pPr marL=\"3657600\" algn=\"l\" defTabSz=\"914400\" rtl=\"0\" eaLnBrk=\"1\" latinLnBrk=\"0\" hangingPunct=\"1\">\
<a:defRPr sz=\"1200\" kern=\"1200\">\
<a:solidFill>\
<a:schemeClr val=\"tx1\"/>\
</a:solidFill>\
<a:latin typeface=\"+mn-lt\"/>\
<a:ea typeface=\"+mn-ea\"/>\
<a:cs typeface=\"+mn-cs\"/>\
</a:defRPr>\
</a:lvl9pPr>\
</p:notesStyle>\
</p:notesMaster>");

			OOX::CPath pathFile = fileName;

			NSFile::CFileBinary oFile;
			oFile.CreateFile(pathFile.GetPath());
			oFile.WriteStringUTF8(s_Common);
			oFile.CloseFile();
		}
}

