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

#include "Res.h"

#include "Utils/CFontChecker.h"
#include "Utils/Utils.h"

#include "../../DesktopEditor/common/Directory.h"
#include "../../OfficeUtils/src/ZipFolder.h"

namespace OFD
{
CRes::CRes()
{}

CRes::~CRes()
{
	#define CLEAR_VECTOR(type, arvaribles)\
	for (const std::pair<unsigned int, type*>& oValue : arvaribles)\
		delete oValue.second

	CLEAR_VECTOR(CColorSpace,           m_mColorSpaces);
	CLEAR_VECTOR(CDrawParam,            m_mDrawParams);
	CLEAR_VECTOR(CFont,                 m_mFonts);
	CLEAR_VECTOR(CMultiMedia,           m_mMultiMedias);
	CLEAR_VECTOR(CCompositeGraphicUnit, m_mCCompositeGraphicUnits);
}

template<class T>
inline void AddElementToMap(T* pElement, unsigned int unIndex, std::map<unsigned int, T*>& mElements)
{
	if (nullptr == pElement)
		return;

	typename std::map<unsigned int, T*>::const_iterator itFound = mElements.find(unIndex);

	if (mElements.cend() != itFound)
		delete itFound->second;

	mElements.insert(std::make_pair(unIndex, pElement));
}

bool CRes::Read(const std::wstring& wsFilePath, const std::wstring& wsRootPath, IFolder* pFolder)
{
	if (wsFilePath.empty() || !CanUseThisPath(wsFilePath, wsRootPath))
		return false;

	const std::wstring wsFullPath{CombinePaths(wsRootPath, wsFilePath)};

	CXmlReader oLiteReader;

	if (!pFolder->getReaderFromFile(wsFullPath, oLiteReader) || !oLiteReader.ReadNextNode() || L"ofd:Res" != oLiteReader.GetName() || oLiteReader.IsEmptyNode())
		return false;

	std::wstring wsResRootPath{wsRootPath};

	if (0 != oLiteReader.GetAttributesCount() && oLiteReader.MoveToFirstAttribute())
	{
		std::string sNodeName;
		do
		{
			sNodeName = oLiteReader.GetNameA();

			if ("BaseLoc" == sNodeName)
			{
				wsResRootPath = CombinePaths(NSDirectory::GetFolderPath(wsFullPath), oLiteReader.GetText());
				break;
			}
		} while (oLiteReader.MoveToNextAttribute());
	}

	oLiteReader.MoveToElement();

	std::string sNodeName;

	#define PARSE_CONTAINER(container_name, element_name, element_type, melements, creator)\
	if (container_name == sNodeName)\
	{\
		const int nChildrenDepth = oLiteReader.GetDepth();\
		element_type* pElement = nullptr;\
		while (oLiteReader.ReadNextSiblingNode(nChildrenDepth))\
		{\
			if (element_name == oLiteReader.GetNameA())\
			{\
				pElement = creator;\
				if (nullptr == pElement)\
					continue;\
				AddElementToMap(pElement, pElement->GetID(), melements);\
			}\
		}\
		continue;\
	}

	#define PARSE_CONTAINER_WITHOUT_PATH(container_name, element_name, element_type, melements)\
	PARSE_CONTAINER(container_name, element_name, element_type, melements, new element_type(oLiteReader))

	#define PARSE_CONTAINER_WITH_PATH(container_name, element_name, element_type, melements)\
	PARSE_CONTAINER(container_name, element_name, element_type, melements, new element_type(oLiteReader, wsResRootPath))

	const int nDepth = oLiteReader.GetDepth();

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		sNodeName = oLiteReader.GetNameA();

		PARSE_CONTAINER_WITHOUT_PATH("ofd:ColorSpaces",           "ofd:ColorSpace",            CColorSpace,           m_mColorSpaces)
		PARSE_CONTAINER_WITHOUT_PATH("ofd:DrawParams",            "ofd:DrawParam",             CDrawParam,            m_mDrawParams)
		PARSE_CONTAINER_WITHOUT_PATH("ofd:CompositeGraphicUnits", "ofd:CCompositeGraphicUnit", CCompositeGraphicUnit, m_mCCompositeGraphicUnits)

		PARSE_CONTAINER_WITH_PATH("ofd:MultiMedias",              "ofd:MultiMedia",            CMultiMedia,           m_mMultiMedias)

		PARSE_CONTAINER("ofd:Fonts", "ofd:Font", CFont, m_mFonts, new CFont(oLiteReader, wsResRootPath, pFolder))
	}

	return true;
}

#define RETURN_ELEMENT_FROM_MAP(element_type, mElements)\
	const std::map<unsigned int, element_type*>::const_iterator itFound = mElements.find(unId);\
	return (mElements.cend() != itFound) ? itFound->second : nullptr\

const CColorSpace* CRes::GetColorSpace(unsigned int unId) const
{
	RETURN_ELEMENT_FROM_MAP(CColorSpace, m_mColorSpaces);
}

const CDrawParam* CRes::GetDrawParam(unsigned int unId) const
{
	RETURN_ELEMENT_FROM_MAP(CDrawParam, m_mDrawParams);
}

const CFont* CRes::GetFont(unsigned int unId) const
{
	RETURN_ELEMENT_FROM_MAP(CFont, m_mFonts);
}

const CMultiMedia* CRes::GetMultiMedia(unsigned int unId) const
{
	RETURN_ELEMENT_FROM_MAP(CMultiMedia, m_mMultiMedias);
}

const CCompositeGraphicUnit* CRes::GetCompositeGraphicUnit(unsigned int unId) const
{
	RETURN_ELEMENT_FROM_MAP(CCompositeGraphicUnit, m_mCCompositeGraphicUnits);
}

std::vector<const CDrawParam*> CRes::GetDrawParams() const
{
	std::vector<const CDrawParam*> arValues;

	for (std::map<unsigned int, CDrawParam*>::const_iterator itBegin = m_mDrawParams.cbegin(); itBegin != m_mDrawParams.cend(); ++itBegin)
		arValues.push_back(itBegin->second);

	return arValues;
}

void CRes::UpdateFonts(CFontChecker* pFontChecker)
{
	if (nullptr == pFontChecker)
		return;

	for (std::pair<unsigned int, CFont*> oElement : m_mFonts)
		pFontChecker->UpdateFont(oElement.second);
}
}
