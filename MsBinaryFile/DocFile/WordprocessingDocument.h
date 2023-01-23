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
#pragma once

#include "OpenXmlPackage.h"

namespace DocFileFormat
{
	class WordprocessingDocument: public OpenXmlPackage
	{
		friend class Converter;
		friend class MainDocumentMapping;
		friend class DocumentMapping;
		friend class CommandTableMapping;
		friend class StyleSheetMapping;
		friend class NumberingMapping;
		friend class FontTableMapping;
		friend class FootnotesMapping;
		friend class EndnotesMapping;
		friend class CommentsMapping;
		friend class SettingsMapping;
		friend class VMLPictureMapping;
		friend class VMLShapeMapping;
		friend class SectionPropertiesMapping;
		friend class HeaderMapping;
		friend class FooterMapping;
		friend class OleObjectMapping;

	private:
		std::wstring FileName;

		std::wstring FontTableXML;
		std::wstring DocumentXML;
		std::wstring StyleSheetXML;
		std::wstring FootnotesXML;
		std::wstring EndnotesXML;
		std::wstring NumberingXML;
		std::wstring CommentsXML;
		std::wstring CommentsExtendedXML;
		std::wstring SettingsXML;
		std::wstring CommandTableXML;

		std::list<ImageFileStructure>		ImagesList;
		std::list<OleObjectFileStructure>	OleObjectsList;

		std::list<std::wstring> HeaderXMLList;
		std::list<std::wstring> FooterXMLList;

	public:
		int _isSectionPageBreak; //0 - not set, 1 -page break, 2 - continues, -1 - already

		WordprocessingDocument(const std::wstring & _docxDirectory, const WordDocument* _docFile);
		virtual ~WordprocessingDocument();
		
		_UINT32 SaveDocument(bool &bMacros);
	};
}