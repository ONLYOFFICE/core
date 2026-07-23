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
#ifndef OOX_CALCCHAIN_FILE_INCLUDE_H_
#define OOX_CALCCHAIN_FILE_INCLUDE_H_

#include "../WritingElement.h"
#include "../../Base/Nullable.h"
#include "../Xlsx.h"

namespace SimpleTypes
{
	class COnOff;
	class CUnsignedDecimalNumber;
}

namespace OOX
{
	namespace Spreadsheet
	{
		class CCalcCell : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionMethods(CCalcCell)
			CCalcCell();
			virtual ~CCalcCell();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::COnOff>					m_oArray;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oSheetId;
			nullable<SimpleTypes::COnOff>					m_oDependencyLevel;
			nullable<std::wstring>							m_oRef;
			nullable<SimpleTypes::COnOff>					m_oChildChain;
			nullable<SimpleTypes::COnOff>					m_oNewThread;
		};

		//not implemented children:
		//<extLst>
		class CCalcChain : public OOX::File, public OOX::IFileContainer
		{
		public:
			CCalcChain(OOX::Document* pMain);
			CCalcChain(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CCalcChain();

			virtual void read(const CPath& oPath);
			virtual void read(const CPath& oRootPath, const CPath& oPath);
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;

			virtual const OOX::FileType type() const;

			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;

			const CPath& GetReadPath();
			void ClearItems();

		private:
			CPath m_oReadPath;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			std::vector<CCalcCell*>  m_arrItems;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_CALCCHAIN_FILE_INCLUDE_H_
