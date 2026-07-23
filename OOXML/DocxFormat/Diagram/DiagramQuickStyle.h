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

#include "DiagramData.h"

namespace OOX
{
	namespace Diagram
	{
		class CStyleLbl : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CStyleLbl)

			CStyleLbl();
			virtual ~CStyleLbl();

			virtual std::wstring toXML() const;

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_string m_sName;

			nullable<PPTX::Logic::Scene3d> m_oScene3d;
			nullable<PPTX::Logic::Sp3d> m_oSp3d;
			nullable<PPTX::Logic::ShapeStyle> m_oStyle;
			nullable<PPTX::Logic::TxBody> m_oTxPr;

			nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
		};
	}

	class CDiagramQuickStyle : public OOX::FileGlobalEnumerated, public OOX::IFileContainer
	{
	public:
		CDiagramQuickStyle(OOX::Document* pMain, bool bDocument = true);
		CDiagramQuickStyle(OOX::Document* pMain, const CPath& uri);
		CDiagramQuickStyle(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);

		virtual ~CDiagramQuickStyle();

		virtual void read(const CPath& oFilePath);
		virtual void read(const CPath& oRootPath, const CPath& oFilePath);
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const;

		virtual const OOX::FileType type() const;
		virtual const CPath DefaultDirectory() const;
		virtual const CPath DefaultFileName() const;

		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

//--------------------------------------------

		nullable_string m_sUniqueId;
		nullable_string m_sMinVer;

		nullable<Diagram::CCatLst> m_oCatLst;
		nullable<Diagram::CText> m_oDesc;
		nullable<Diagram::CText> m_oTitle;
		std::vector<Diagram::CStyleLbl*> m_arStyleLbl;
		nullable<PPTX::Logic::Scene3d>	m_oScene3d;

		nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;

	private:
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		bool m_bDocument = true; //for upper/lower level rels (defaultDirectory)
	};

} // namespace OOX
