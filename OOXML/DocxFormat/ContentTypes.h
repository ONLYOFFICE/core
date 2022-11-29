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
#ifndef OOX_CONTENT_TYPES_INCLUDE_H_
#define OOX_CONTENT_TYPES_INCLUDE_H_

#include "../SystemUtility/SystemUtility.h"

#include "FileType.h"
#include "WritingElement.h"
#include <boost/unordered_map.hpp>


namespace OOX
{
	namespace ContentTypes
	{
		class CExtensionTable
		{
		public:
			CExtensionTable();

			const std::wstring operator[] (const std::wstring& sExtension) const;

		private:
            std::map <std::wstring, std::wstring> m_mTable;
		};

		class CDefault : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDefault)
			CDefault();
			CDefault(const std::wstring& sExtension);
			virtual ~CDefault();

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

            std::wstring m_sExtension;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};

		class COverride : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(COverride)
			COverride();
			COverride(const std::wstring& sType, const CPath& oPath);
			virtual ~COverride();

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			const std::wstring type() const;
			const OOX::CPath filename() const;

		private:
            std::wstring	m_sType;
			OOX::CPath		m_oPart;

		};
	} // namespace ContentTypes

    static const CPath c_oContentTypeFileName (_T("[Content_Types].xml"));

	class CContentTypes
	{
	public:
		CContentTypes();
		CContentTypes(const CPath& oPath);
		~CContentTypes();

		bool Read (const CPath& oDirPath);
		bool ReadFromString (std::wstring& sXml);

		void Merge(CContentTypes * pSrcContentTypes);
		bool Write(const CPath& oDirPath) const;

		void Registration(const std::wstring& sType, const CPath& oDirectory, const CPath& oFilename);
		void Registration(const std::wstring& sType, const std::wstring& sDirectory, const std::wstring& sFilename);

		void AddDefault(const std::wstring& sExtension);

	private:
		bool ReadFromReader (XmlUtils::CXmlLiteReader& oReader);
		void AddOverride(const std::wstring& sType, std::wstring sPath);

	public:
        boost::unordered_map<std::wstring, ContentTypes::CDefault>	m_mapDefaults;
        boost::unordered_map<std::wstring, ContentTypes::COverride>	m_mapOverrides;
	};

} // namespace OOX

#endif // OOX_CONTENT_TYPES_INCLUDE_H_
