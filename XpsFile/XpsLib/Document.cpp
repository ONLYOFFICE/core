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
#include "Document.h"
#include "StaticResources.h"

#include "../../DesktopEditor/xml/include/xmlutils.h"
#include "../../DesktopEditor/common/File.h"

namespace XPS
{
    CDocument::CDocument(NSFonts::IFontManager* pFontManager)
	{		
		m_pFontManager = pFontManager;
		m_mPages.clear();
	}
	CDocument::~CDocument()
	{		
		Close();
	}
	bool CDocument::ReadFromPath(const std::wstring& wsPath)
	{
		Close();

		m_wsPath = wsPath;

		XmlUtils::CXmlLiteReader oReader;

		std::wstring wsRelsPath = NormalizePath(wsPath + L"_rels/.rels");
		if (!oReader.FromFile(wsRelsPath))
			return false;

		if (!oReader.ReadNextNode())
			return false;

		std::wstring wsName = oReader.GetName();
		if (L"Relationships" != wsName)
			return false;

		std::wstring wsTargetFile;
		while (oReader.ReadNextNode())
		{
			wsName = oReader.GetName();
			if (L"Relationship" == wsName)
			{
				std::wstring wsAttr;
				ReadAttribute(oReader, L"Type", wsAttr);

				if (L"http://schemas.microsoft.com/xps/2005/06/fixedrepresentation" == wsAttr)
				{
					ReadAttribute(oReader, L"Target", wsTargetFile);
					break;
				}
			}
		}

		if (wsTargetFile.empty())
			return false;

		oReader.Clear();

		std::wstring wsTargerFullPath = m_wsPath + wsTargetFile;
		if (!NSFile::CFileBinary::Exists(wsTargerFullPath))
		{
			wsTargerFullPath = GetPath(wsRelsPath) + wsTargetFile;
			if (!NSFile::CFileBinary::Exists(wsTargerFullPath))
				return false;
		}
		
		if (!oReader.FromFile(wsTargerFullPath))
			return false;

		if (!oReader.ReadNextNode())
			return false;

		wsName = oReader.GetName();
		if (L"FixedDocumentSequence" != wsName)
			return false;

		std::wstring wsSourceFile;
		while (oReader.ReadNextNode())
		{
			wsName = oReader.GetName();
			if (L"DocumentReference" == wsName)
			{
				ReadAttribute(oReader, L"Source", wsSourceFile);
				break;
			}
		}

		if (wsSourceFile.empty())
			return false;

		oReader.Clear();


		std::wstring wsSourceFullPath = m_wsPath + wsSourceFile;
		if (!NSFile::CFileBinary::Exists(wsSourceFullPath))
		{
			wsSourceFullPath = GetPath(wsTargerFullPath) + wsSourceFile;
			if (!NSFile::CFileBinary::Exists(wsSourceFullPath))
				return false;
		}

		if (!oReader.FromFile(wsSourceFullPath))
			return false;

		if (!oReader.ReadNextNode())
			return false;

		wsName = oReader.GetName();
		if (L"FixedDocument" != wsName)
			return false;

		std::wstring wsFilePath = GetPath(wsSourceFullPath);
		std::wstring wsPagePath;
		std::wstring wsSource;

		int nIndex = 0;
		while (oReader.ReadNextNode())
		{
			wsName = oReader.GetName();

			if (L"PageContent" == wsName)
			{
				ReadAttribute(oReader, L"Source", wsSource);

				std::wstring wsPagePath = m_wsPath + wsSource;
				if (!NSFile::CFileBinary::Exists(wsPagePath))
				{
					wsPagePath = wsFilePath + wsSource;
					if (!NSFile::CFileBinary::Exists(wsPagePath))
						continue;
				}

				m_mPages.insert(std::pair<int, XPS::Page*>(nIndex++, new XPS::Page(wsPagePath, wsPath, &m_oFontList, m_pFontManager, this)));
			}

		}

		return true;		
	}
	int  CDocument::GetPageCount()const
	{
		return (int)m_mPages.size();
	}
	void CDocument::GetPageSize(int nPageIndex, int& nW, int& nH)
	{
		std::map<int, XPS::Page*>::const_iterator oIter = m_mPages.find(nPageIndex);
		if (oIter != m_mPages.end())
			oIter->second->GetSize(nW, nH);
	}
	void CDocument::DrawPage(int nPageIndex, IRenderer* pRenderer, bool* pbBreak)
	{
		std::map<int, XPS::Page*>::const_iterator oIter = m_mPages.find(nPageIndex);
		if (oIter != m_mPages.end())
			oIter->second->Draw(pRenderer, pbBreak);
	}
	void CDocument::Close()
	{
		for (std::map<int, XPS::Page*>::iterator oIter = m_mPages.begin(); oIter != m_mPages.end(); oIter++)
		{
			if (oIter->second)
				delete oIter->second;
		}
		m_mPages.clear();
		m_oFontList.Clear();

		for (std::map<std::wstring, CStaticResource*>::iterator oIter = m_mStaticResources.begin(); oIter != m_mStaticResources.end(); oIter++)
		{
			if (oIter->second)
				delete oIter->second;
		}
		m_mStaticResources.clear();
	}
	CStaticResource* CDocument::GetStaticResource(const wchar_t* wsPath)
	{
		for (auto oIt : m_mStaticResources)
		{
			if (oIt.first == wsPath)
				return oIt.second;
		}

		CStaticResource* pStaticResource = new CStaticResource(wsPath);
		m_mStaticResources.insert(std::pair<std::wstring, CStaticResource*>(wsPath, pStaticResource));
		return pStaticResource;
	}
}
