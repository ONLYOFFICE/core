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

#include "FileType.h"
#include "FileTypes.h"
#include "RId.h"
#include "WritingElement.h"

#include "External/External.h"

#include "../Base/SmartPtr.h"
#include "../SystemUtility/SystemUtility.h"

#include "../../DesktopEditor/common/File.h"

#include <boost/unordered_map.hpp>

namespace OOX
{
	namespace Rels
	{
		class CRelationShip : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CRelationShip)

			CRelationShip();
			CRelationShip(const OOX::RId& rId, const std::wstring& sType, const OOX::CPath& oFilePath, bool bExternal = false);
			CRelationShip(const OOX::RId& rId, const smart_ptr<External> pExternal);
			virtual ~CRelationShip();
			
			virtual void  fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void fromXML(XmlUtils::CXmlNode& oNode);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			const std::wstring Type() const;
			const CPath   Filename() const;
			const CPath   Target() const;
			const bool    IsExternal() const;
			const RId rId() const;
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			RId						m_rId;
			CPath					m_oTarget;
            std::wstring            m_sType;
            nullable<std::wstring>  m_sMode;
		};

	} // namespace Rels

	class CRels
	{
	public:
		CRels();
		CRels(const CPath& oFilePath);
		~CRels();

		void Read (const CPath& oFilePath);
		void Write(const CPath& oFilePath) const;

		void Registration(const RId& rId, const std::wstring& oRelationShipType, const CPath& oPath, bool bExternal = false);
		void Registration(const RId& rId, const FileType& oType, const CPath& oPath, bool bExternal = false);
		void Registration(const RId& rId, const smart_ptr<External> pExternal);

		void GetRel(const RId& rId, Rels::CRelationShip** ppRelationShip);

		std::vector<Rels::CRelationShip*> m_arRelations;
		boost::unordered_map<std::wstring, Rels::CRelationShip*> m_mapRelations;
	private:
		const CPath CreateFileName(const CPath& oFilePath) const;
	};

} // namespace OOX
