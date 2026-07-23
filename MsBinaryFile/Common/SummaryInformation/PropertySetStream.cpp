/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#include "../../Common/Utils/simple_xml_writer.h"

#include "PropertySetStream.h"
#include "CodePageOle.h"

#include "../../XlsFile/Format/Binary/CFStream.h"
#include "../../XlsFile/Format/Auxiliary/HelpFunc.h"
#include "../../../DesktopEditor/common/SystemUtils.h"
#include "../../../OOXML/Base/Base.h"
#include "../../../OOXML/Base/Unit.h"

namespace OLEPS
{
	void PropertySetStream::read(XLS::CFStreamPtr stream, bool ext)
	{
		if (!stream) return;

		unsigned short	ByteOrder = 0;
		unsigned short	Version = 0;
		_UINT32			SystemIdentifier = 0;
		_UINT32			NumPropertySets = 0;
		_GUID_			Clsid = {};

		*stream >> ByteOrder >> Version >> SystemIdentifier;

		if (ByteOrder != 0xFFFE) //file (53).xls
			return;

		if (Version != 0x01 && Version != 0x00)
			return;

		*stream >> SystemIdentifier >> Clsid;

		*stream >> NumPropertySets;

		if (SystemIdentifier == NumPropertySets)
		{//oops 
			_GUID_ Clsid2 = {};
			_UINT32 reserved1 = 0, reserved2 = 0;
			
			*stream >> Clsid2 >> reserved1 >> reserved2; // ??? ReportBuilder
			*stream >> NumPropertySets;
		}

		if (NumPropertySets != 0x01 && NumPropertySets != 0x02)
		{
			NumPropertySets = 0x01;
			stream->seekFromBegin(28);
		}

		std::vector<_UINT32> property_sets_offsets;
		for (_UINT32 i = 0; i < NumPropertySets; ++i)
		{
			_GUID_ FMTID;

			_UINT32 Offset;
			*stream >> FMTID >> Offset;

			if (Offset < stream->getStreamSize())
				property_sets_offsets.push_back(Offset);
		}

		for (_UINT32 i = 0; i < property_sets_offsets.size(); ++i)
		{
			PropertySetPtr set = PropertySetPtr(new PropertySet(stream, property_sets_offsets[i], ext));

			property_sets.push_back(set);
		}
	}
	PropertyPtr PropertySetStream::GetProperty(unsigned int id)
	{
		PropertyPtr prop;

		for (size_t i = 0; i < property_sets.size(); ++i)
		{
			prop = property_sets[i]->GetProperty(id);
			if (prop)
				break;
		}
        return prop;
	}
	const unsigned short PropertySetStream::GetCodePage()
	{
		return 0 != property_sets.size() ? property_sets[0]->GetCodePage() : 0;
	}
	std::wstring PropertySetStream::GetApp()
	{
		if (property_sets.empty()) return L"";

		PropertyPtr prop;

		std::wstringstream output;
		CP_XML_WRITER(output)
		{
			CP_XML_NODE(L"Properties")
			{
				CP_XML_ATTR(L"xmlns", L"http://schemas.openxmlformats.org/officeDocument/2006/extended-properties");
				CP_XML_ATTR(L"xmlns:vt", L"http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes");

				prop = GetProperty(TEMPLATE);
				if ((prop) && (false == prop->IsEmpty()))
				{
					CP_XML_NODE(L"Template")
					{
						CP_XML_CONTENT(prop->toString());
					}
				}
				prop = GetProperty(MANAGER);
				if ((prop) && (false == prop->IsEmpty()))
				{
					CP_XML_NODE(L"Manager")
					{
						CP_XML_CONTENT(prop->toString());
					}
				}
				prop = GetProperty(COMPANY);
				if ((prop) && (false == prop->IsEmpty()))
				{
					CP_XML_NODE(L"Company")
					{
						CP_XML_CONTENT(prop->toString());
					}
				}
				prop = GetProperty(PAGECOUNT);
				if (prop)
				{
					CP_XML_NODE(L"Pages")
					{
                        CP_XML_STREAM() << prop->toString();
					}
				}
				prop = GetProperty(WORDCOUNT);
				if (prop)
				{
					CP_XML_NODE(L"Words")
					{
                        CP_XML_STREAM() << prop->toString();
					}
				}
				//prop = GetProperty(CHARCOUNT);
				//if (prop)
				//{
				//	CP_XML_NODE(L"CharactersWithSpaces")
				//	{
                //		CP_XML_STREAM() << prop->toString();
				//	}
				//}
				prop = GetProperty(CHARCOUNT);
				if (prop)
				{
					CP_XML_NODE(L"Characters")
					{
                        CP_XML_STREAM() << prop->toString();
					}
				}
				prop = GetProperty(PRESFORMAT);
				if ((prop) && (false == prop->IsEmpty()))
				{
					CP_XML_NODE(L"PresentationFormat")
					{
                        CP_XML_STREAM() << prop->toString();
					}
				}
				prop = GetProperty(LINECOUNT);
				if (prop)
				{
					CP_XML_NODE(L"Lines")
					{
                        CP_XML_STREAM() << prop->toString();
					}
				}
				prop = GetProperty(PARACOUNT);
				if (prop)
				{
					CP_XML_NODE(L"Paragraphs")
					{
                        CP_XML_STREAM() << prop->toString();
					}
				}
				prop = GetProperty(SLIDECOUNT);
				if (prop)
				{
					CP_XML_NODE(L"Slides")
					{
                        CP_XML_STREAM() << prop->toString();
					}
				}
				prop = GetProperty(NOTECOUNT);
				if (prop)
				{
					CP_XML_NODE(L"Notes")
					{
                        CP_XML_STREAM() << prop->toString();
					}
				}
				prop = GetProperty(HIDDENCOUNT);
				if (prop)
				{
					CP_XML_NODE(L"HiddenSlides")
					{
                        CP_XML_STREAM() << prop->toString();
					}
				}
				prop = GetProperty(MMCLIPCOUNT);
				if (prop)
				{
					CP_XML_NODE(L"MMClips")
					{
                        CP_XML_STREAM() << prop->toString();
					}
				}
				prop = GetProperty(SCALE);
				if (prop)
				{
					CP_XML_NODE(L"ScaleCrop")
					{
                        CP_XML_STREAM() << prop->toString();
					}
				}
				prop = GetProperty(HEADINGPAIR);
				if (prop)
				{
					CP_XML_NODE(L"HeadingPairs")
					{
                        CP_XML_STREAM() << prop->toString();
					}
				}
				prop = GetProperty(DOCPARTS);
				if (prop)
				{
					CP_XML_NODE(L"TitlesOfParts")
					{
                        CP_XML_STREAM() << prop->toString();
					}
				}
				prop = GetProperty(LINKBASE);
				if (prop)
				{
					CP_XML_NODE(L"LinksUpToDate")
					{
                        CP_XML_STREAM() << prop->toString();
					}
				}
				prop = GetProperty(CCHWITHSPACES);
				if (prop)
				{
					CP_XML_NODE(L"CharactersWithSpaces")
					{
                        CP_XML_STREAM() << prop->toString();
					}
				}
				prop = GetProperty(SHAREDDOC);
				if (prop)
				{
					CP_XML_NODE(L"SharedDoc")
					{
                        CP_XML_STREAM() << prop->toString();
					}
				}
				//	LINKBASE = 0x1014,
				prop = GetProperty(HLINKS);
				if (prop)
				{
					CP_XML_NODE(L"HLinks")
					{
                        CP_XML_STREAM() << prop->toString();
					}
				}
				prop = GetProperty(HYPERLINKSCHANGED);
				if (prop)
				{
					CP_XML_NODE(L"HyperlinksChanged")
					{
                        CP_XML_STREAM() << prop->toString();
					}
				}
				//	DIGSIG = 0x1018,
				CP_XML_NODE(L"Application")
				{
					std::wstring sApplication = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvApplicationName);
					if (sApplication.empty())
						sApplication = NSSystemUtils::gc_EnvApplicationNameDefault;

					prop = GetProperty(APPNAME);
					if ((prop) && (false == prop->IsEmpty()))
					{
                        CP_XML_STREAM() << (prop->toString() + L"/" + sApplication);
					}
					else
					{
						CP_XML_STREAM() << sApplication;
#if defined(INTVER)
						std::string s = VALUE2STR(INTVER);
						CP_XML_STREAM() << L"/" << std::wstring(s.begin(), s.end());
#endif				
					}
				}
				prop = GetProperty(DOCVERSION);
				if ((prop) && (false == prop->IsEmpty()))
				{
					CP_XML_NODE(L"AppVersion")
					{
                        CP_XML_STREAM() << prop->toString();
					}
				}
				prop = GetProperty(DOC_SECURITY);
				if (prop)
				{
					CP_XML_NODE(L"DocSecurity")
					{
                        CP_XML_STREAM() << prop->toString();
					}
				}
			}
		}
		return output.str();
	}
	std::wstring PropertySetStream::GetCore()
	{
		if (property_sets.empty()) return L"";

		std::wstringstream output;
		PropertyPtr prop;
		std::wstring value;

		CP_XML_WRITER(output)
		{
			CP_XML_NODE(L"cp:coreProperties")
			{
				CP_XML_ATTR(L"xmlns:cp", L"http://schemas.openxmlformats.org/package/2006/metadata/core-properties");
				CP_XML_ATTR(L"xmlns:dc", L"http://purl.org/dc/elements/1.1/");
				CP_XML_ATTR(L"xmlns:dcterms", L"http://purl.org/dc/terms/");
				CP_XML_ATTR(L"xmlns:dcmitype", L"http://purl.org/dc/dcmitype/");
				CP_XML_ATTR(L"xmlns:xsi", L"http://www.w3.org/2001/XMLSchema-instance");

				prop = GetProperty(TITLE);
				value = prop ? prop->toString() : L"";
				if (!value.empty())
				{
					CP_XML_NODE(L"dc:title")
                    {
						CP_XML_CONTENT(value);
					}
				}
				prop = GetProperty(SUBJECT);
				value = prop ? prop->toString() : L"";
				if (!value.empty())
				{
					CP_XML_NODE(L"dc:subject")
					{
						CP_XML_CONTENT(value);
					}
				}
				prop = GetProperty(AUTHOR);
				value = prop ? prop->toString() : L"";
				if (!value.empty())
				{
					CP_XML_NODE(L"dc:creator")
					{
						CP_XML_CONTENT(value);
					}
				}
				prop = GetProperty(KEYWORDS);
				value = prop ? prop->toString() : L"";
				if (!value.empty())
				{
					CP_XML_NODE(L"cp:keywords")
					{
						CP_XML_CONTENT(value);
					}
				}
				prop = GetProperty(COMMENTS);
				value = prop ? prop->toString() : L"";
				if (!value.empty())
				{
					CP_XML_NODE(L"dc:description")
					{
						CP_XML_CONTENT(value);
					}
				}
				prop = GetProperty(LANGUAGE);
				value = prop ? prop->toString() : L"";
				if (!value.empty())
				{
					CP_XML_NODE(L"dc:language")
					{
                        CP_XML_STREAM() << value;
					}
				}				
				prop = GetProperty(VERSION);
				value = prop ? prop->toString() : L"";
				if (!value.empty())
				{
					CP_XML_NODE(L"cp:version")
					{
                        CP_XML_STREAM() << value;
					}
				}
				prop = GetProperty(LASTAUTHOR);
				value = prop ? prop->toString() : L"";
				if (!value.empty())
				{
					CP_XML_NODE(L"cp:lastModifiedBy")
					{
						CP_XML_CONTENT(value);
					}
				}
				prop = GetProperty(REVNUMBER);
				value = prop ? prop->toString() : L"";
				if (!value.empty())
				{
					unsigned int val = XmlUtils::GetUInteger(value);
					CP_XML_NODE(L"cp:revision")
					{
						CP_XML_STREAM() << val;
					}
				}
				prop = GetProperty(CREATE_DTM);
				value = prop ? prop->toString() : L"";
				if (!value.empty())
				{
					CP_XML_NODE(L"dcterms:created")
					{
						CP_XML_ATTR(L"xsi:type", L"dcterms:W3CDTF");
                        CP_XML_STREAM() << value;
					}
				}
				prop = GetProperty(LASTSAVE_DTM);
				value = prop ? prop->toString() : L"";
				if (!value.empty())
				{
					CP_XML_NODE(L"dcterms:modified")
					{
						CP_XML_ATTR(L"xsi:type", L"dcterms:W3CDTF");
                        CP_XML_STREAM() << value;
					}
				}
				prop = GetProperty(CATEGORY);
				value = prop ? prop->toString() : L"";
				if (!value.empty())
				{
					CP_XML_NODE(L"cp:category")
					{
						CP_XML_CONTENT(value);
					}
				}
				prop = GetProperty(CONTENTSTATUS);
				value = prop ? prop->toString() : L"";
				if (!value.empty())
				{
					CP_XML_NODE(L"cp:contentStatus")
					{
                        CP_XML_STREAM() << value;
					}
				}
				//	BYTECOUNT = 0x1004,
				//	LINKSDIRTY = 0x1010,
				//	CONTENTTYPE = 0x101A,
			}
		}
		return output.str();
	}
} // namespace OLEPS
