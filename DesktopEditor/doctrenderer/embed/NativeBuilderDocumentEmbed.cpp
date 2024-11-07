#include "NativeBuilderDocumentEmbed.h"
#include "./../docbuilder_p.h"

#include "../../common/Directory.h"
#include "../server.h"

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
	{
		if (CServerInstance::getInstance().IsEnable())
			CServerInstance::getInstance().AddTmpFile(m_sFolder);
		m_bIsValid = true;
	}
}

void CBuilderDocumentEmbed::_CloseFile()
{
	if (!m_sFolder.empty())
	{
		NSDirectory::DeleteDirectory(m_sFolder);

		if (m_bIsValid && CServerInstance::getInstance().IsEnable())
			CServerInstance::getInstance().RemoveTmpFile(m_sFolder);
	}

	m_bIsValid = false;
	m_sFolder = L"";
}
