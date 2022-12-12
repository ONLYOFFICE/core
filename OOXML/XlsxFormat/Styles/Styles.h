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

#include "../Xlsx.h"

#include "../CommonInclude.h"

#include "Borders.h"
#include "CellStyles.h"
#include "Xfs.h"
#include "Colors.h"
#include "dxf.h"
#include "Fills.h"
#include "Fonts.h"
#include "NumFmts.h"
#include "TableStyles.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CStyle2003 : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CStyle2003)
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
		};
	} //Spreadsheet
} // namespace OOX

