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
#include "../../DesktopEditor/common/StringExt.h"

namespace XPS
{
    void RemoveLastSlashes(std::wstring& sPath)
    {
        size_t nLen = sPath.length();
        if (nLen > 0)
        {
            if (sPath[nLen - 1] == '/' || sPath[nLen - 1] == '\\')
                sPath.erase(nLen - 1);
        }

        if (std::wstring::npos == sPath.find('%'))
            return;

        std::wstring result;
        result.reserve(nLen);
        for (size_t i = 0; i < nLen; ++i)
        {
            if (i < nLen - 2 && sPath[i] == '%')
            {
                wchar_t a = sPath[i + 1];
                int aInt = -1;
                if (a >= '0' && a <= '9')
                    aInt = a - '0';
                else if (a >= 'a' && a <= 'f')
                    aInt = 10 + a - 'a';
                else if (a >= 'A' && a <= 'F')
                    aInt = 10 + a - 'A';

                if (aInt < 0)
                {
                    result.push_back(sPath[i]);
                    continue;
                }

                wchar_t b = sPath[i + 2];
                int bInt = -1;
                if (b >= '0' && b <= '9')
                    bInt = b - '0';
                else if (b >= 'a' && b <= 'f')
                    bInt = 10 + b - 'a';
                else if (b >= 'A' && b <= 'F')
                    bInt = 10 + b - 'A';

                if (bInt < 0)
                {
                    result.push_back(sPath[i]);
                    continue;
                }

                result.push_back((wchar_t)((aInt << 4) | bInt));
                i += 2;
            }
            else
            {
                result.push_back(sPath[i]);
            }
        }
        sPath = result;
    }

    CDocument::CDocument(NSFonts::IFontManager* pFontManager)
	{		
		m_pFontManager = pFontManager;
		m_mPages.clear();
		m_wsPath = NULL;
	}
	CDocument::~CDocument()
	{		
		Close();
	}
	bool CDocument::Read(IFolder* pFolder)
	{
		Close();

		m_wsPath = pFolder;

		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromStringA(m_wsPath->readXml(L"_rels/.rels")))
			return false;

		if (!oReader.ReadNextNode())
			return false;

		std::wstring wsName = oReader.GetNameNoNS();
		if (L"Relationships" != wsName)
			return false;

		std::wstring wsTargetFile;
		while (oReader.ReadNextNode())
		{
			wsName = oReader.GetNameNoNS();
			if (L"Relationship" == wsName)
			{
				std::wstring wsAttr;
				ReadAttribute(oReader, L"Type", wsAttr);

                // http://schemas.microsoft.com/xps/2005/06/fixedrepresentation
                // http://schemas.openxps.org/oxps/v1.0/fixedrepresentation
                if (wsAttr.find(L"fixedrepresentation") != std::wstring::npos &&
                    ((wsAttr.find(L"/xps/") != std::wstring::npos) || (wsAttr.find(L"/oxps/") != std::wstring::npos)))
				{
					ReadAttribute(oReader, L"Target", wsTargetFile);
					break;
				}
			}
		}

        RemoveLastSlashes(wsTargetFile);
		if (wsTargetFile.empty())
			return false;

		oReader.Clear();

        if (!m_wsPath->existsXml(wsTargetFile))
		{
			wsTargetFile = GetPath(L"_rels/.rels") + wsTargetFile;
            if (!m_wsPath->existsXml(wsTargetFile))
				return false;
		}
		
		if (!oReader.FromStringA(m_wsPath->readXml(wsTargetFile)))
			return false;

		if (!oReader.ReadNextNode())
			return false;

		wsName = oReader.GetNameNoNS();
		if (L"FixedDocumentSequence" != wsName)
			return false;

		std::wstring wsSourceFile;
		while (oReader.ReadNextNode())
		{
			wsName = oReader.GetNameNoNS();
			if (L"DocumentReference" == wsName)
			{
				ReadAttribute(oReader, L"Source", wsSourceFile);
				break;
			}
		}

        RemoveLastSlashes(wsSourceFile);
		if (wsSourceFile.empty())
			return false;

        if (!m_wsPath->existsXml(wsSourceFile))
		{
			wsSourceFile = GetPath(wsTargetFile) + wsSourceFile;
            if (!m_wsPath->existsXml(wsSourceFile))
				return false;
		}

		std::wstring wsFilePath = GetPath(wsSourceFile);

		// Оглавление, содержание, structure
		oReader.Clear();

		std::wstring wsStructureTargetFile = wsFilePath + L"_rels/" + NSFile::GetFileName(wsSourceFile) + L".rels";
		std::wstring wsStructureFile;
		if (m_wsPath->exists(wsStructureTargetFile) && oReader.FromStringA(m_wsPath->readXml(wsStructureTargetFile))
				&& oReader.ReadNextNode() && oReader.GetNameNoNS() == L"Relationships")
		{
			while (oReader.ReadNextNode())
			{
				if (L"Relationship" == oReader.GetNameNoNS())
				{
					std::wstring wsAttr;
					ReadAttribute(oReader, L"Type", wsAttr);
					if (L"http://schemas.microsoft.com/xps/2005/06/documentstructure" == wsAttr)
					{
						ReadAttribute(oReader, L"Target", wsStructureFile);
						break;
					}
				}
			}
		}

		std::wstring wsFullStructureFile;
		if (!wsStructureFile.empty())
		{
			if (m_wsPath->exists(wsStructureFile))
				wsFullStructureFile = wsStructureFile;
			else if (m_wsPath->exists(wsFilePath + wsStructureFile))
				wsFullStructureFile = wsFilePath + wsStructureFile;
			else
			{
				wsStructureFile = GetPath(wsStructureTargetFile) + wsStructureFile;
				if (m_wsPath->exists(wsStructureFile))
					wsFullStructureFile = wsStructureFile;
			}
		}

		if (!wsFullStructureFile.empty())
		{
			oReader.Clear();
			if (oReader.FromStringA(m_wsPath->readXml(wsFullStructureFile)) && oReader.ReadNextNode() && oReader.GetNameNoNS() == L"DocumentStructure")
			{
				while (oReader.ReadNextNode())
				{
					if (L"DocumentStructure.Outline" == oReader.GetNameNoNS() && oReader.ReadNextNode() && oReader.GetNameNoNS() == L"DocumentOutline")
					{
						while (oReader.ReadNextNode())
						{
							if (oReader.GetNameNoNS() == L"OutlineEntry")
							{
								CDocumentStructure oStructure;
								oStructure.nLevel = 1; // OutlineLevel по умолчанию имеет значение 1
								oStructure.nPage  = 0;
								oStructure.dY     = 0; // по умолчанию верхняя часть страницы (216)
								while (oReader.MoveToNextAttribute())
								{
									std::wstring wsAttrName = oReader.GetName();
									std::wstring wsAttrText = oReader.GetText();
									if (wsAttrName == L"OutlineLevel")
										oStructure.nLevel = GetInteger(wsAttrText);
									else if (wsAttrName == L"Description")
										oStructure.sDescription = U_TO_UTF8(wsAttrText);
									else if (wsAttrName == L"OutlineTarget")
									{
										size_t nSharp = wsAttrText.find(L'#');
										if (nSharp != std::wstring::npos)
											oStructure.wsTarget = wsAttrText.substr(nSharp + 1);
									}
								}
								oReader.MoveToElement();
								m_vStructure.push_back(oStructure);
							}
						}
					}
				}
			}
		}

		oReader.Clear();

		if (!oReader.FromStringA(m_wsPath->readXml(wsSourceFile)))
			return false;

		if (!oReader.ReadNextNode())
			return false;

		wsName = oReader.GetNameNoNS();
		if (L"FixedDocument" != wsName)
			return false;

		std::wstring wsSource;

		int nIndex = 0;
		while (oReader.ReadNextNode())
		{
			wsName = oReader.GetNameNoNS();

			if (L"PageContent" == wsName)
			{
				ReadAttribute(oReader, L"Source", wsSource);

				std::wstring wsPagePath = wsSource;
                RemoveLastSlashes(wsPagePath);
                if (!m_wsPath->existsXml(wsPagePath))
				{
					wsPagePath = wsFilePath + wsSource;
                    if (!m_wsPath->existsXml(wsPagePath))
						continue;
				}

                if (!oReader.IsEmptyElement())
                {
                    int nDepth = oReader.GetDepth();
                    while (oReader.ReadNextSiblingNode(nDepth))
                    {
                        if (oReader.GetNameNoNS() == L"PageContent.LinkTargets")
                        {
                            int nLinkDepth = oReader.GetDepth();
                            while (oReader.ReadNextSiblingNode(nLinkDepth))
                            {
                                if (oReader.GetNameNoNS() == L"LinkTarget")
                                {
                                    std::wstring wsNameTarget;
                                    ReadAttribute(oReader, L"Name", wsNameTarget);
                                    if (!wsNameTarget.empty())
                                    {
                                        std::vector<CDocumentStructure>::iterator find = std::find_if(m_vStructure.begin(), m_vStructure.end(), [wsNameTarget](const CDocumentStructure& str){ return str.wsTarget == wsNameTarget; });
                                        if (find != m_vStructure.end())
                                            find->nPage = nIndex;
                                        else
                                            m_mInternalLinks.insert(std::pair<std::wstring, int>(wsNameTarget, nIndex));
                                    }
                                }
                            }
                        }
                    }
                }

				m_mPages.insert(std::pair<int, XPS::Page*>(nIndex++, new XPS::Page(wsPagePath, m_wsPath, &m_oFontList, m_pFontManager, this)));
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
	BYTE* CDocument::GetStructure()
	{
        NSWasm::CData oRes;
		oRes.SkipLen();
		for (const CDocumentStructure& str : m_vStructure)
		{
			oRes.AddInt(str.nPage);
			oRes.AddInt(str.nLevel);
            oRes.AddDouble(str.dY);
			oRes.WriteString((BYTE*)str.sDescription.c_str(), str.sDescription.length());
		}
		oRes.WriteLen();
		BYTE* bRes = oRes.GetBuffer();
		oRes.ClearWithoutAttack();
		return bRes;
	}
	BYTE* CDocument::GetPageLinks (int nPageIndex)
	{
		std::map<int, XPS::Page*>::const_iterator oIter = m_mPages.find(nPageIndex);
		if (oIter != m_mPages.end())
            return oIter->second->m_oLinks.Serialize();
		return NULL;
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
		RELEASEOBJECT(m_wsPath);
	}
	CStaticResource* CDocument::GetStaticResource(const wchar_t* wsPath)
	{
		for (auto oIt : m_mStaticResources)
		{
			if (oIt.first == wsPath)
				return oIt.second;
		}

		CStaticResource* pStaticResource = new CStaticResource(m_wsPath->readXml(wsPath));
		m_mStaticResources.insert(std::pair<std::wstring, CStaticResource*>(wsPath, pStaticResource));
		return pStaticResource;
	}

	std::wstring CDocument::GetInfo()
	{
		XmlUtils::CXmlLiteReader oReader;
		std::wstring sRes = L"{";

		if (oReader.FromStringA(m_wsPath->readXml(L"_rels/.rels")) && oReader.ReadNextNode() && L"Relationships" == oReader.GetNameNoNS())
		{
			std::wstring wsCoreFile;
			while (oReader.ReadNextNode())
			{
				if (L"Relationship" == oReader.GetNameNoNS())
				{
					std::wstring wsAttr;
					ReadAttribute(oReader, L"Type", wsAttr);

					if (wsAttr.find(L"core-properties") != std::wstring::npos)
					{
						ReadAttribute(oReader, L"Target", wsCoreFile);
						break;
					}
				}
			}

			if (!wsCoreFile.empty() && m_wsPath->exists(wsCoreFile))
			{
				oReader.Clear();
				if (oReader.FromStringA(m_wsPath->readXml(wsCoreFile)) && oReader.ReadNextNode() && oReader.GetNameNoNS() == L"coreProperties")
				{
					while (oReader.ReadNextNode())
					{
						std::wstring sName = oReader.GetName();
						if (sName == L"dc:title")
							sName = L"Title";
						else if (sName == L"dc:subject")
							sName = L"Subject";
						else if (sName == L"dc:creator")
                            sName = L"Author";
						else if (sName == L"dcterms:created")
							sName = L"CreationDate";
						else if (sName == L"dcterms:modified")
							sName = L"ModDate";

						sRes += L"\"";
						sRes += sName;
						sRes += L"\":\"";
						sName = oReader.GetText2();
						NSStringExt::Replace(sName, L"\"", L"\\\"");
                        sRes += sName;
						sRes += L"\",";
					}
				}
			}
		}
		int nW = 0;
		int nH = 0;
		GetPageSize(0, nW, nH);
        sRes += L"\"PageWidth\":";
        sRes += std::to_wstring((int)(nW * 100));
        sRes += L",\"PageHeight\":";
        sRes += std::to_wstring((int)(nH * 100));
        sRes += L",\"NumberOfPages\":";
        sRes += std::to_wstring(GetPageCount());
        sRes += L"}";

		return sRes;
	}
}
