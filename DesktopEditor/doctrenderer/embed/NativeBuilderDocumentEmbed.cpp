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

#include "NativeBuilderDocumentEmbed.h"
#include "./../docbuilder_p.h"

#include "../../common/Directory.h"

JSSmart<CJSValue> CBuilderDocumentEmbed::IsValid()
{
	return CJSContext::createBool(m_bIsValid);
}

JSSmart<CJSValue> CBuilderDocumentEmbed::GetBinary()
{
	return CJSContext::createUint8Array(m_sFolder + L"/Editor.bin");
}

JSSmart<CJSValue> CBuilderDocumentEmbed::GetFolder()
{
	return CJSContext::createString(m_sFolder);
}

JSSmart<CJSValue> CBuilderDocumentEmbed::Close()
{
	_CloseFile();
	return NULL;
}

JSSmart<CJSValue> CBuilderDocumentEmbed::GetImageMap()
{
	std::vector<std::wstring> files = NSDirectory::GetFiles(m_sFolder + L"/media");

	JSSmart<CJSObject> obj = CJSContext::createObject();
	for (std::vector<std::wstring>::iterator i = files.begin(); i != files.end(); i++)
	{
		std::wstring sFile = *i;
		NSStringUtils::string_replace(sFile, L"\\", L"/");
		std::wstring sName = L"media/" + NSFile::GetFileName(sFile);

		obj->set(U_TO_UTF8(sName).c_str(), CJSContext::createString(sFile));
	}

	return obj->toValue();
}

void CBuilderDocumentEmbed::_OpenFile(const std::wstring& sFile, const std::wstring& sParams)
{
	NSDoctRenderer::CDocBuilder_Private* pBuilder = GetPrivate(m_pBuilder);

	std::wstring sTmpDir = pBuilder->m_sTmpFolder;

	m_sFolder = NSFile::CFileBinary::CreateTempFileWithUniqueName(sTmpDir, L"DE_");
	if (NSFile::CFileBinary::Exists(m_sFolder))
		NSFile::CFileBinary::Remove(m_sFolder);

	NSStringUtils::string_replace(m_sFolder, L"\\", L"/");

	std::wstring::size_type nPosPoint = m_sFolder.rfind('.');
	if (nPosPoint != std::wstring::npos && nPosPoint > sTmpDir.length())
	{
		m_sFolder = m_sFolder.substr(0, nPosPoint);
	}

	NSDirectory::CreateDirectory(m_sFolder);

	std::wstring sExtCopy = pBuilder->GetFileCopyExt(sFile);
	std::wstring sFileCopy = m_sFolder + L"/origin." + sExtCopy;

	pBuilder->MoveFileOpen(sFile, sFileCopy);
	int nConvertResult = pBuilder->ConvertToInternalFormat(m_sFolder, sFileCopy, sParams);

	if (0 == nConvertResult)
		m_bIsValid = true;
}

void CBuilderDocumentEmbed::_CloseFile()
{
	if (!m_sFolder.empty())
		NSDirectory::DeleteDirectory(m_sFolder);
	m_bIsValid = false;
	m_sFolder = L"";
}
