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

#include "File.h"
#include "Font.h"
#include "FileTypes.h"

#include "../Common/SimpleTypes_Word.h"
#include "../Common/SimpleTypes_Shared.h"
#include "../Common/ComplexTypes.h"

#include "../SystemUtility/File.h"
	
namespace OOX
{
	class CFontTable : public OOX::File, public WritingElement
	{
	public:
		CFontTable(OOX::Document *pMain);
		CFontTable(OOX::Document *pMain, const CPath &oFilePath);
		CFontTable(XmlUtils::CXmlNode& oNode);
		CFontTable(XmlUtils::CXmlLiteReader& oReader);
		virtual ~CFontTable();

		const CFontTable& operator =(const XmlUtils::CXmlNode& oNode);
		const CFontTable& operator =(const XmlUtils::CXmlLiteReader& oReader);

		bool Find(std::wstring &sFontName, CFont *oFont);

		virtual void read(const CPath &oFilePath);
		virtual void write(const CPath &oFilePath, const CPath &oDirectory, CContentTypes& oContent) const;

		virtual const FileType type() const;


		virtual const CPath DefaultDirectory() const;
		virtual const CPath DefaultFileName() const;

		virtual void fromXML(XmlUtils::CXmlNode& oNode);
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

		virtual std::wstring toXML() const;
		virtual EElementType getType() const;

//------------------------------------------------------------------------
		nullable<ComplexTypes::Word::CFonts>	m_oDefaultFonts;
		std::vector<CFont*>						m_arrFonts;

	};

} // namespace OOX
