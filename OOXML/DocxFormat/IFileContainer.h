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

#include "RId.h"
#include "UnknowTypeFile.h"
#include "IFileBuilder.h"
#include <boost/unordered_map.hpp>

namespace OOX 
{
	class File;
	class FileType;
	class CRels;
	class CContentTypes;
	class HyperLink;
}

namespace OOX
{
	class IFileContainer
	{
	public:
		IFileContainer(OOX::Document* pMain);
		virtual ~IFileContainer();

		bool m_bVisioPages = false;
        bool m_bSpreadsheets = false;

		static boost::unordered_map<std::wstring, size_t>	m_mapEnumeratedGlobal;
		OOX::Document*										m_pMainDocument;
        smart_ptr<OOX::CRels>								m_pCurRels;

		void Read (const OOX::CPath& oRootPath, const OOX::CPath& oPath);
		
		template<class TypeOut> 
		smart_ptr<TypeOut> Get (const RId& rId) const
		{
			std::map<std::wstring, smart_ptr<OOX::File>>::const_iterator pFind = m_mapContainer.find(rId.get());
			if (pFind == m_mapContainer.end ())
				return smart_ptr<TypeOut>();
			return pFind->second.smart_dynamic_cast<TypeOut>();
		}

		std::vector<smart_ptr<OOX::File>>& GetContainer() {return m_arContainer;}
		
		template<typename T>
		const bool IsExist() const;
		const bool IsExist(const OOX::RId& rId) const;
		const bool IsExternal(const OOX::RId& rId) const;
		
		std::wstring IsExistHyperlink(const std::wstring & href);
		
		const RId Add(smart_ptr<OOX::File>& pFile);
		void      Add(const OOX::RId& rId, smart_ptr<OOX::File>& pFile);
		
		const RId AddNoWrite(const smart_ptr<OOX::File>& pFile, const std::wstring& oDefDir);
		void      AddNoWrite(const OOX::RId& rId, const smart_ptr<OOX::File>& pFile, const std::wstring& oDefDir);

		template<typename T> 
        T&                   Find();
		smart_ptr<OOX::File> Find(const OOX::RId& type) const;
		smart_ptr<OOX::File> Find(const FileType& oType) const;

		smart_ptr<OOX::File> operator [](const OOX::RId rId);
		smart_ptr<OOX::File> operator [](const FileType& oType);
		
		void SetGlobalNumberByType(const std::wstring& sOverrideType, int val);
		int GetGlobalNumberByType(const std::wstring& sOverrideType);
		void AssignOutputFilename(smart_ptr<OOX::File>& pFile);

		const RId GetMaxRId();
	protected:
		static UnknowTypeFile m_oUnknown;
		std::vector<smart_ptr<OOX::File>> m_arContainer;
		std::map<std::wstring, smart_ptr<OOX::File>> m_mapContainer;

        boost::unordered_map<std::wstring, std::wstring> m_mNoWriteContainer;
        unsigned int m_lMaxRid;

		void Read (const OOX::CRels& oRels, const OOX::CPath& oRootPath, const CPath& oPath);
		void Write (const OOX::CPath& oFileName, const CPath& oDir, OOX::CContentTypes& oContent) const;
		void Write (OOX::CRels& oRels, const CPath& oCurrent, const CPath& oDir, OOX::CContentTypes& oContent) const;

		void Commit (const CPath& oPath);
		void Finalize(const CPath& oFilefilename, const CPath& oDir, OOX::CContentTypes& oContent);
		void Finalize(OOX::CRels& oRels, const CPath& oCurrent, const CPath& oDir, OOX::CContentTypes& oContent);

	private:
        boost::unordered_map<std::wstring, size_t>			m_mapAddNamePair;
		boost::unordered_map<std::wstring, std::wstring>	m_mapHyperlinks;      
	};

} // namespace OOX
