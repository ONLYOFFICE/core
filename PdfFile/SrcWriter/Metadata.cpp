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
#include "Metadata.h"
#include "Streams.h"
#include "Info.h"

#include "../../DesktopEditor/common/SystemUtils.h"
#include "../../OOXML/Base/Base.h"

void EscapeCharacters(std::string& s)
{
	NSStringUtils::string_replaceA(s, "&", "&amp;");
	NSStringUtils::string_replaceA(s, "<", "&lt;");
	NSStringUtils::string_replaceA(s, ">", "&gt;");
	NSStringUtils::string_replaceA(s, "\"", "&quot;");
	NSStringUtils::string_replaceA(s, "\'", "&#39;");
}

namespace PdfWriter
{
	//----------------------------------------------------------------------------------------
	// Metadata
	//----------------------------------------------------------------------------------------
	CMetadata::CMetadata(CXref* pXref, CInfoDict* pInfo)
	{
		pXref->Add(this);

		Add("Type", "Metadata");
		Add("Subtype", "XML");

		m_pStream = new CMemoryStream();
		SetStream(pXref, m_pStream);

		std::string sXML;

        unsigned char strBOM[4] = { 0xEF, 0xBB, 0xBF, 0 };
        std::string sBOM((char*)strBOM);

		// Begin
        sXML += ("<?xpacket begin=\"" + sBOM + "\" id=\"W5M0MpCehiHzreSzNTczkc9d\"?>\n<x:xmpmeta xmlns:x=\"adobe:ns:meta/\" x:xmptk=\"3.1-701\">\n<rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\">\n");

		sXML += "<rdf:Description rdf:about=\"\" xmlns:pdf=\"http://ns.adobe.com/pdf/1.3/\">\n";
		// Producer
		sXML += "<pdf:Producer>";		
		sXML += pInfo->GetInfo(InfoProducer);
		sXML += "</pdf:Producer>\n";
		// Keywords
		const char* sKeywords = pInfo->GetInfo(InfoKeyWords);
		if (sKeywords)
		{
			std::string s = sKeywords;
			EscapeCharacters(s);

			sXML += "<pdf:Keywords>";
			sXML += s;
			sXML += "</pdf:Keywords>\n";
		}
		sXML += "</rdf:Description>\n";

		// Creator Tool
		std::wstring sApplication = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvApplicationName);
		if (sApplication.empty())
			sApplication = NSSystemUtils::gc_EnvApplicationNameDefault;

		sXML += "<rdf:Description rdf:about=\"\" xmlns:xmp=\"http://ns.adobe.com/xap/1.0/\">\n";
		sXML += "<xmp:CreatorTool>";
		sXML += NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sApplication);

#if defined(INTVER)
		sXML += "/";
		sXML += VALUE2STR(INTVER);
#endif

		sXML += "</xmp:CreatorTool>\n";

		TDate oDate = pInfo->GetDate();
		sXML += "<xmp:CreateDate>";
		oDate.AppendToString(sXML);
		sXML += "</xmp:CreateDate>";

		sXML += "<xmp:ModifyDate>";
		oDate.AppendToString(sXML);
		sXML += "</xmp:ModifyDate>";

		sXML += "</rdf:Description>\n";

		// DC
		const char* sTitle   = pInfo->GetInfo(InfoTitle);
		const char* sAuthor  = pInfo->GetInfo(InfoAuthor);
		const char* sSubject = pInfo->GetInfo(InfoSubject);
		if (sTitle || sAuthor || sSubject)
		{
			sXML += "<rdf:Description rdf:about=\"\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\">\n";
			sXML += "<dc:format>application/pdf</dc:format>";
			if (sSubject)
			{
				std::string s = sSubject;
				EscapeCharacters(s);

				sXML += "<dc:description><rdf:Alt><rdf:li xml:lang=\"x-default\">";
				sXML += s;
				sXML += "</rdf:li></rdf:Alt></dc:description>\n";
			}
			if (sAuthor)
			{
				std::string s = sAuthor;
				EscapeCharacters(s);

				sXML += "<dc:creator><rdf:Seq><rdf:li>";
				sXML += s;
				sXML += "</rdf:li></rdf:Seq></dc:creator>\n";
			}
			if (sTitle)
			{
				std::string s = sTitle;
				EscapeCharacters(s);

				sXML += "<dc:title><rdf:Alt><rdf:li xml:lang=\"x-default\">";
				sXML += s;
				sXML += "</rdf:li></rdf:Alt></dc:title>\n";
			}
			sXML += "</rdf:Description>\n";
		}

		if (pXref->IsPDFA())
		{
			sXML += "<rdf:Description rdf:about=\"\" xmlns:pdfaid=\"http://www.aiim.org/pdfa/ns/id/\">\n";
			sXML += "<pdfaid:part>1</pdfaid:part><pdfaid:conformance>A</pdfaid:conformance>\n";
			sXML += "</rdf:Description>";
		}

		// End
		sXML += "</rdf:RDF>\n</x:xmpmeta><?xpacket end=\"w\"?>";

		m_pStream->WriteStr(sXML.c_str());
	}

}
