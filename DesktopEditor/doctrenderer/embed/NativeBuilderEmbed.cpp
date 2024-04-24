/*
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

#include "NativeBuilderEmbed.h"
#include "NativeBuilderDocumentEmbed.h"
#include "./../docbuilder_p.h"

#include "../../common/Directory.h"

JSSmart<CJSValue> CBuilderEmbed::OpenFile(JSSmart<CJSValue> sPath, JSSmart<CJSValue> sParams)
{
	std::wstring Path = sPath->toStringW();
	std::wstring Params = sParams->toStringW();
	int ret = m_pBuilder->OpenFile(Path.c_str(), Params.c_str());
	return CJSContext::createInt(ret);
}

JSSmart<CJSValue> CBuilderEmbed::CreateFile(JSSmart<CJSValue> type)
{
	int nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
	if (type->isString())
		nFormat = NSDoctRenderer::GetFormatByTexExtention(type->toStringW());
	else
		nFormat = type->toInt32();

	bool ret = m_pBuilder->CreateFile(nFormat);
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
	int ret = m_pBuilder->SaveFile(nFormat, sPath.c_str(), sParams.empty() ? NULL : sParams.c_str());
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
	pBuilderTmpDocNative->_OpenFile(sPath, sParams);
	return oBuilderTmpDoc->toValue();
}
