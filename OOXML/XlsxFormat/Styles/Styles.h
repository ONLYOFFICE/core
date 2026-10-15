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
#pragma once

#include "../Xlsx.h"

#include "../WritingElement.h"
#include "../../Base/Nullable.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CBorder;
		class CBorders;
		class CFill;
		class CFont;
		class CNumFmt;
		class CAligment;
		class CCellStyles;
		class CCellStyleXfs;
		class CCellXfs;
		class CColors;
		class CDxfs;
		class CFills;
		class CFonts;
		class CNumFmts;
		class CTableStyles;
	}

	namespace Drawing
	{
		class COfficeArtExtensionList;
	}

	namespace Spreadsheet
	{
		class CStyle2003 : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CStyle2003)
			CStyle2003(OOX::Document* pMain);
			virtual ~CStyle2003();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_string		m_sName;
			nullable_string		m_sId;
			nullable_string		m_sParentId;

            nullable<OOX::Spreadsheet::CBorder>		m_oBorder;
			nullable<OOX::Spreadsheet::CFill>		m_oFill;
			nullable<OOX::Spreadsheet::CFont>		m_oFont;
			nullable<OOX::Spreadsheet::CNumFmt>		m_oNumFmt;
			nullable<OOX::Spreadsheet::CAligment>	m_oAligment;

			bool bStyleContinuous = false;
			bool bUsed = false;
		};

		class CStyles : public OOX::File, public OOX::IFileContainer, public WritingElement
		{
		public:
			CStyles(OOX::Document* pMain);
			CStyles(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CStyles();

			void readBin(const CPath& oPath);
			XLS::BaseObjectPtr WriteBin() const;
			void toXLS(XLS::BaseObjectPtr globalsStreamPtr);

			virtual void read(const CPath& oPath);
			virtual void read(const CPath& oRootPath, const CPath& oPath);
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
			void AfterRead();
			void ConvertStyle2003(CStyle2003 *style2003, bool bDefault = false);
			void MergeStyles2003(CStyle2003 *style2003, CStyle2003 *parent);
			void PrepareToWrite();
			virtual const OOX::FileType type() const;
			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;
			const CPath& GetReadPath();

		public:
			CPath										m_oReadPath;

			nullable<OOX::Spreadsheet::CBorders>		m_oBorders;
			nullable<OOX::Spreadsheet::CCellStyles>		m_oCellStyles;
			nullable<OOX::Spreadsheet::CCellStyleXfs>	m_oCellStyleXfs;
			nullable<OOX::Spreadsheet::CCellXfs>		m_oCellXfs;
			nullable<OOX::Spreadsheet::CColors>			m_oColors;
			nullable<OOX::Spreadsheet::CDxfs>			m_oDxfs;
			nullable<OOX::Spreadsheet::CFills>			m_oFills;
			nullable<OOX::Spreadsheet::CFonts>			m_oFonts;
			nullable<OOX::Spreadsheet::CNumFmts>		m_oNumFmts;
			nullable<OOX::Spreadsheet::CTableStyles>	m_oTableStyles;

			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;

			std::vector<CStyle2003*>		m_arrStyles2003;
			std::map<std::wstring, size_t>	m_mapStyles2003;
			std::map<unsigned int, bool>	m_mapStylesContinues2003;
			size_t							m_nStyleNormal2003 = 0xffffffff;
		private:
			void SetFillXLS(XLS::BaseObjectPtr XFSPtr, XLS::BaseObjectPtr workbookPtr);
			void SetBordersXLS(XLS::BaseObjectPtr XFSPtr,XLS::BaseObjectPtr workbookPtr);
			void SetExtPropsXLS(XLS::BaseObjectPtr XFSPt);
		};
	} //Spreadsheet
} // namespace OOX

