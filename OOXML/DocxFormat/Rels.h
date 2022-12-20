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
			WritingElement_AdditionConstructors(CRelationShip)

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
