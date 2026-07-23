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

#include "NativeBuilderEmbed.h"
#include "NativeBuilderDocumentEmbed.h"
#include "./../docbuilder_p.h"

#include "../../common/Directory.h"

JSSmart<CJSValue> CBuilderEmbed::OpenFile(JSSmart<CJSValue> sPath, JSSmart<CJSValue> sParams)
{
	std::wstring Path = sPath->toStringW();
	std::wstring Params = sParams->toStringW();

	// Since we now use snapshots, and we can't always determine which editor is needed
	// (since the code may be BEFORE opening the file). And if the opening came from
	// builderJS.OpenFile - in this case we skip reopening.
	NSDoctRenderer::CDocBuilder_Private* pPrivate = GetPrivate();
	if (pPrivate->m_pWorker->IsSimpleJSInstance())
		pPrivate->m_bIsOpenedFromSimpleJS = true;

	int ret = m_pBuilder->OpenFile(Path.c_str(), Params.c_str());

	if (pPrivate->m_pWorker->IsSimpleJSInstance())
	{
		JSSmart<CJSContext> current = CJSContext::GetCurrent();
		current->runScript("throw 0;");
	}

	return CJSContext::createInt(ret);
}

JSSmart<CJSValue> CBuilderEmbed::CreateFile(JSSmart<CJSValue> type)
{
	int nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
	if (type->isString())
		nFormat = NSDoctRenderer::GetFormatByTexExtention(type->toStringW());
	else
		nFormat = type->toInt32();

	// Since we now use snapshots, and we can't always determine which editor is needed
	// (since the code may be BEFORE opening the file). And if the opening came from
	// builderJS.OpenFile - in this case we skip reopening.
	NSDoctRenderer::CDocBuilder_Private* pPrivate = GetPrivate();
	if (pPrivate->m_pWorker->IsSimpleJSInstance())
		pPrivate->m_bIsOpenedFromSimpleJS = true;

	bool ret = m_pBuilder->CreateFile(nFormat);

	if (pPrivate->m_pWorker->IsSimpleJSInstance())
	{
		JSSmart<CJSContext> current = CJSContext::GetCurrent();
		current->runScript("throw 0;");
	}

	return CJSContext::createBool(ret);
}

JSSmart<CJSValue> CBuilderEmbed::SetTmpFolder(JSSmart<CJSValue> path)
{
	std::wstring sPath = path->toStringW();
	m_pBuilder->SetTmpFolder(sPath.c_str());
	return NULL;
}

JSSmart<CJSValue> CBuilderEmbed::SaveFile(JSSmart<CJSValue> type, JSSmart<CJSValue> path, JSSmart<CJSValue> params)
{
	int nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
	if (type->isString())
		nFormat = NSDoctRenderer::GetFormatByTexExtention(type->toStringW());
	else
		nFormat = type->toInt32();

	std::wstring sPath = path->toStringW();
	std::wstring sParams = params->toStringW();
	int ret = GetPrivate()->SaveFile(nFormat, sPath.c_str(), sParams.empty() ? NULL : sParams.c_str(), false);
	return CJSContext::createInt(ret);
}

JSSmart<CJSValue> CBuilderEmbed::CloseFile()
{
	std::cout << "Warning: use builderJS.CloseFile(); separately!";
	m_pBuilder->GetPrivate()->CloseFile(false);
	return NULL;
}

JSSmart<CJSValue> CBuilderEmbed::OpenTmpFile(JSSmart<CJSValue> path, JSSmart<CJSValue> params)
{
	std::wstring sPath = path->toStringW();
	std::wstring sParams = params->toStringW();
	JSSmart<CJSObject> oBuilderTmpDoc = CJSContext::createEmbedObject("CBuilderDocumentEmbed");
	CBuilderDocumentEmbed* pBuilderTmpDocNative = static_cast<CBuilderDocumentEmbed*>(oBuilderTmpDoc->getNative());
	pBuilderTmpDocNative->m_pBuilder = m_pBuilder;
	pBuilderTmpDocNative->SetExternalize(true);
	pBuilderTmpDocNative->_OpenFile(sPath, sParams);
	return oBuilderTmpDoc->toValue();
}
