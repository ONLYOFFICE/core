#pragma once

#include "./../include/CertificateCommon.h"
#include "../../../../OfficeUtils/src/ZipFolder.h"
#include "./XmlTransform.h"
#include <cstdio>
#include <ctime>
#include <time.h>

class CSignFolderFiles
{
public:
	std::map<std::wstring, std::wstring>    m_content_types;
	std::vector<std::wstring>               m_rels;
	std::vector<std::wstring>               m_files;

public:
	CSignFolderFiles() {}
	~CSignFolderFiles() {}

public:
	static bool CheckNeedSign(const std::wstring& sCheckFile)
	{
		if (0 == sCheckFile.find(L"_xmlsignatures") ||
				0 == sCheckFile.find(L"docProps") ||
				0 == sCheckFile.find(L"[Content_Types].xml") ||
				0 == sCheckFile.find(L"[trash]"))
			return false;
		return true;
	}

	void Folder_Parse(IFolder* pFolder, bool bIsAddSlash = false)
	{
		// 1) Parse files in directory
		std::vector<std::wstring> files = pFolder->getFiles(L"", true);

		// 2) Check each file
		std::wstring sFolder = L"";
		for (std::vector<std::wstring>::iterator i = files.begin(); i != files.end(); i++)
		{
			std::wstring sCheckFile = *i;

			// make cool filename
			sCheckFile = pFolder->getLocalFilePath(sCheckFile);

			// check needed file
			if (!CheckNeedSign(sCheckFile))
				continue;

			// check rels and add to needed array
			std::wstring::size_type posExt = sCheckFile.rfind(L".");
			if (std::wstring::npos == posExt)
				continue;

			std::wstring sExt = sCheckFile.substr(posExt + 1);

			if (bIsAddSlash)
				sCheckFile = L"/" + sCheckFile;

			if (sExt == L"rels")
				m_rels.push_back(sCheckFile);
			else
				m_files.push_back(sCheckFile);
		}

		std::sort(m_rels.begin(), m_rels.end());
		std::sort(m_files.begin(), m_files.end());
	}

	void Folder_ParseContentTypes(IFolder* pFolder)
	{
		XmlUtils::CXmlNode oNode = pFolder->getNodeFromFile(L"/[Content_Types].xml");

		std::vector<XmlUtils::CXmlNode> nodesDefaults = oNode.GetNodes(L"Default");
		std::vector<XmlUtils::CXmlNode> nodesOverrides = oNode.GetNodes(L"Override");

		size_t nCount = nodesDefaults.size();
		for (size_t i = 0; i < nCount; ++i)
		{
			XmlUtils::CXmlNode node = nodesDefaults[i];
			m_content_types.insert(std::pair<std::wstring, std::wstring>(node.GetAttribute("Extension"), node.GetAttribute("ContentType")));
		}

		nCount = nodesOverrides.size();
		for (size_t i = 0; i < nCount; ++i)
		{
			XmlUtils::CXmlNode node = nodesOverrides[i];
			m_content_types.insert(std::pair<std::wstring, std::wstring>(node.GetAttribute("PartName"), node.GetAttribute("ContentType")));
		}
	}
};

