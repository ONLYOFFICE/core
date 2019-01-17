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
#include <string>

#include "TxtXmlFile.h"

#include "ConvertDocx2Txt.h"
#include "ConvertTxt2Docx.h"

#include "Common/StlUtils.h"

#include "../../../Common/DocxFormat/Source/DocxFormat/Docx.h"

#include "../../../Common/DocxFormat/Source/DocxFormat/App.h"
#include "../../../Common/DocxFormat/Source/DocxFormat/Core.h"
#include "../../../DesktopEditor/common/SystemUtils.h"
#include "../../../Common/OfficeFileErrorDescription.h"
namespace NSBinPptxRW
{
	class  CDrawingConverter;
}

#include "../../../OfficeUtils/src/OfficeUtils.h"
#include "../../../Common/OfficeDefines.h"

#include "../../../Common/DocxFormat/Source/SystemUtility/File.h"

#include "../../../ASCOfficeDocxFile2/DocWrapper/FontProcessor.h"

#include "../../../ASCOfficeDocxFile2/BinReader/FileWriter.h"


CTxtXmlFile::CTxtXmlFile()
{
}


bool CTxtXmlFile::Progress(long ID, long Percent)
{
    SHORT res = 0;
    m_lPercent = Percent;
    //OnProgressEx(ID, Percent, &res); todooo out event
    return (res != 0);
}

static int ParseTxtOptions(const std::wstring & sXmlOptions)
{
	int encoding = -1;
	
	XmlUtils::CXmlLiteReader xmlReader;
	
	if (xmlReader.FromString(sXmlOptions))
	{
		xmlReader.ReadNextNode();//root - <Options>

		int nCurDepth = xmlReader.GetDepth();
		while ( xmlReader.ReadNextSiblingNode( nCurDepth ) )
		{
            std::wstring sName = xmlReader.GetName();

			if (sName == _T("TXTOptions"))
			{
				int nCurDepth1 = xmlReader.GetDepth();
				while ( xmlReader.ReadNextSiblingNode( nCurDepth1 ) )
				{
                    std::wstring sName1 = xmlReader.GetName();
					if (sName1 == _T("Encoding"))
					{
                        std::wstring strValue = xmlReader.GetText2();
						encoding = StlUtils::ToInteger(strValue);
					}
				}
			}
		}
	}
	return encoding;
}


_UINT32 CTxtXmlFile::txt_LoadFromFile(const std::wstring & sSrcFileName, const std::wstring & sDstPath, const std::wstring & sXMLOptions)
{
    Writers::FileWriter *pDocxWriter =  new Writers::FileWriter(sDstPath, L"", true, 1, false, NULL, L"");
	if (pDocxWriter == NULL) return AVS_FILEUTILS_ERROR_CONVERT;

    CreateDocxEmpty(sDstPath, pDocxWriter);

	try
	{
		int encoding  = ParseTxtOptions(sXMLOptions);

		Progress(0, 0);
		Txt2Docx::Converter converter( encoding);
        converter.read(sSrcFileName);
		Progress(0, 100000);
		converter.convert(*this);
		converter.write(pDocxWriter->m_oDocumentWriter.m_oContent);
		Progress(0, 1000000);
	}
	catch(...)
	{
		return AVS_FILEUTILS_ERROR_CONVERT;
	}

	pDocxWriter->m_oDocumentWriter.Write(); //overwrite document.xml

	delete pDocxWriter;
	pDocxWriter = NULL;

	return 0;
}


_UINT32 CTxtXmlFile::txt_SaveToFile(const std::wstring & sDstFileName, const std::wstring & sSrcPath, const std::wstring & sXMLOptions)
{
	try
	{
		Progress(0, 0);
		Docx2Txt::Converter converter;
        converter.read(sSrcPath);
		Progress(0, 100000);
		converter.convert(*this);

		int encoding  = ParseTxtOptions(sXMLOptions);
		
		if (encoding == EncodingType::Utf8)
            converter.writeUtf8(sDstFileName);
		else if (encoding == EncodingType::Unicode)
            converter.writeUnicode(sDstFileName);
		else if (encoding == EncodingType::Ansi)
            converter.writeAnsi(sDstFileName);
		else if (encoding == EncodingType::BigEndian)
            converter.writeBigEndian(sDstFileName);
		else if (encoding > 0) //code page
		{
            converter.write(sDstFileName);
		}
		else //auto define
            converter.write(sDstFileName);

		Progress(0, 1000000);
	}
	catch(...)
	{
		return AVS_FILEUTILS_ERROR_CONVERT;
	}

  return 0;
}


void CTxtXmlFile::CreateDocxEmpty(const std::wstring & _strDirectory, Writers::FileWriter * pDocxWriter)
{
    std::wstring strDirectory = _strDirectory;
	// rels
    OOX::CPath pathRels = strDirectory + FILE_SEPARATOR_STR +L"_rels";
    NSDirectory::CreateDirectory(pathRels.GetPath());

	// word
    OOX::CPath pathWord = strDirectory + FILE_SEPARATOR_STR + L"word";
    NSDirectory::CreateDirectory(pathWord.GetPath());

	// documentRels
    OOX::CPath pathWordRels = pathWord + FILE_SEPARATOR_STR + _T("_rels");
    NSDirectory::CreateDirectory(pathWordRels.GetPath());

	//media
    OOX::CPath pathMedia = pathWord + FILE_SEPARATOR_STR + _T("media");
    std::wstring sMediaPath = pathMedia.GetPath();

	// theme
    OOX::CPath pathTheme = pathWord + FILE_SEPARATOR_STR + _T("theme");
    NSDirectory::CreateDirectory(pathTheme.GetPath());

    OOX::CPath pathThemeRels = pathTheme + FILE_SEPARATOR_STR + _T("_rels");
    NSDirectory::CreateDirectory(pathThemeRels.GetPath());
	
    pathTheme = pathTheme + FILE_SEPARATOR_STR + _T("theme1.xml");

//default files

	pDocxWriter->m_oTheme.Write(pathTheme.GetPath());

	OOX::CContentTypes oContentTypes;
	//docProps
    OOX::CPath pathDocProps = strDirectory + FILE_SEPARATOR_STR + _T("docProps");
    NSDirectory::CreateDirectory(pathDocProps.GetPath());
	
    OOX::CPath DocProps = std::wstring(_T("docProps"));

	OOX::CApp* pApp = new OOX::CApp(NULL);
	if (pApp)
	{
		std::wstring sApplication = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvApplicationName);
		if (sApplication.empty())
			sApplication = NSSystemUtils::gc_EnvApplicationNameDefault;
		pApp->SetApplication(sApplication);
#if defined(INTVER)
        pApp->SetAppVersion(VALUE2STR(INTVER));
#endif
		pApp->SetDocSecurity(0);
		pApp->SetScaleCrop(false);
		pApp->SetLinksUpToDate(false);
		pApp->SetSharedDoc(false);
		pApp->SetHyperlinksChanged(false);
		
		pApp->write(pathDocProps + FILE_SEPARATOR_STR + _T("app.xml"), DocProps, oContentTypes);
		delete pApp;
	}				
	OOX::CCore* pCore = new OOX::CCore(NULL);
	if (pCore)
	{
		pCore->SetCreator(_T(""));
		pCore->SetLastModifiedBy(_T(""));
		pCore->write(pathDocProps + FILE_SEPARATOR_STR + _T("core.xml"), DocProps, oContentTypes);
		delete pCore;
	} 
/////////////////////////////////////////////////////////////////////////////////////
	pDocxWriter->m_oTheme.Write(strDirectory);
	pDocxWriter->m_oStylesWriter.Write();
	pDocxWriter->m_oFontTableWriter.Write();
	
	pDocxWriter->m_oSettingWriter.Write();
	pDocxWriter->m_oWebSettingsWriter.Write();
	
	pDocxWriter->m_oDocumentWriter.Write();
	
	pDocxWriter->m_oDocumentRelsWriter.Write();

	oContentTypes.Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml",	OOX::CPath(L"/word"),		OOX::CPath(L"document.xml"));
	oContentTypes.Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml",		OOX::CPath(L"/word"),		OOX::CPath(L"styles.xml"));
	oContentTypes.Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.settings+xml",		OOX::CPath(L"/word"),		OOX::CPath(L"settings.xml"));
	oContentTypes.Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.webSettings+xml",	OOX::CPath(L"/word"),		OOX::CPath(L"webSettings.xml"));
	oContentTypes.Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.fontTable+xml",		OOX::CPath(L"/word"),		OOX::CPath(L"fontTable.xml"));
	oContentTypes.Registration(L"application/vnd.openxmlformats-officedocument.theme+xml",							OOX::CPath(L"/word/theme"), OOX::CPath(L"theme1.xml"));

	oContentTypes.Write(strDirectory);
}

