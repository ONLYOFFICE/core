﻿/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

		//необработанные child:
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
