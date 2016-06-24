/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

namespace NSBinPptxRW
{
	class  CDrawingConverter;
}

#include "../../../OfficeUtils/src/OfficeUtils.h"
#include "../../../Common/OfficeDefines.h"

#include "../../../Common/DocxFormat/Source/SystemUtility/File.h"

#include "../../../DesktopEditor/common/Path.h"
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
			CString sName = xmlReader.GetName();

			if (sName == _T("TXTOptions"))
			{
				int nCurDepth1 = xmlReader.GetDepth();
				while ( xmlReader.ReadNextSiblingNode( nCurDepth1 ) )
				{
					CString sName1 = xmlReader.GetName();
					if (sName1 == _T("Encoding"))
					{
						CString strValue = xmlReader.GetText2();
						encoding = StlUtils::ToInteger(string2std_string(strValue));
					}
				}
			}
		}
	}
	return encoding;
}


HRESULT CTxtXmlFile::txt_LoadFromFile(const std::wstring & sSrcFileName, const std::wstring & sDstPath, const std::wstring & sXMLOptions)
{
	//проверка на структуру xml - если что не так выкинет быстро
	//HRESULT hr = xml_LoadFromFile(sSrcFileName, sDstPath, sXMLOptions);
	//if(hr == S_OK)
	//	return S_OK;

	//As Text

    Writers::FileWriter *pDocxWriter =  new Writers::FileWriter(std_string2string(sDstPath), _T(""), 1, false, NULL, _T(""));
	if (pDocxWriter == NULL) return S_FALSE;

    CreateDocxEmpty(std_string2string(sDstPath), pDocxWriter);

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
		return S_FALSE;
	}

	pDocxWriter->m_oDocumentWriter.Write(); //overwrite document.xml

	delete pDocxWriter;
	pDocxWriter = NULL;

	return S_OK;
}


HRESULT CTxtXmlFile::txt_SaveToFile(const std::wstring & sDstFileName, const std::wstring & sSrcPath, const std::wstring & sXMLOptions)
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
		return S_FALSE;
	}

  return S_OK;
}


void CTxtXmlFile::CreateDocxEmpty(CString strDirectory, Writers::FileWriter * pDocxWriter) 
{
	// rels
    OOX::CPath pathRels = strDirectory + FILE_SEPARATOR_STR + _T("_rels");
    FileSystem::Directory::CreateDirectory(pathRels.GetPath());

	// word
    OOX::CPath pathWord = strDirectory + FILE_SEPARATOR_STR + _T("word");
    FileSystem::Directory::CreateDirectory(pathWord.GetPath());

	// documentRels
    OOX::CPath pathWordRels = pathWord + FILE_SEPARATOR_STR + _T("_rels");
    FileSystem::Directory::CreateDirectory(pathWordRels.GetPath());

	//media
    OOX::CPath pathMedia = pathWord + FILE_SEPARATOR_STR + _T("media");
	CString sMediaPath = pathMedia.GetPath();

	// theme
    OOX::CPath pathTheme = pathWord + FILE_SEPARATOR_STR + _T("theme");
    FileSystem::Directory::CreateDirectory(pathTheme.GetPath());

    OOX::CPath pathThemeRels = pathTheme + FILE_SEPARATOR_STR + _T("_rels");
    FileSystem::Directory::CreateDirectory(pathThemeRels.GetPath());
	
    pathTheme = pathTheme + FILE_SEPARATOR_STR + _T("theme1.xml");

//default files

	pDocxWriter->m_oDefaultTheme.Write(pathTheme.GetPath());

	OOX::CContentTypes oContentTypes;
	//docProps
    OOX::CPath pathDocProps = strDirectory + FILE_SEPARATOR_STR + _T("docProps");
    FileSystem::Directory::CreateDirectory(pathDocProps.GetPath());
	
    OOX::CPath DocProps = CString(_T("docProps"));

	OOX::CApp* pApp = new OOX::CApp();
	if (pApp)
	{
		pApp->SetApplication(_T("OnlyOffice"));
		pApp->SetAppVersion(_T("3.0000"));
		pApp->SetDocSecurity(0);
		pApp->SetScaleCrop(false);
		pApp->SetLinksUpToDate(false);
		pApp->SetSharedDoc(false);
		pApp->SetHyperlinksChanged(false);
		
		pApp->write(pathDocProps + FILE_SEPARATOR_STR + _T("app.xml"), DocProps, oContentTypes);
		delete pApp;
	}				
	OOX::CCore* pCore = new OOX::CCore();
	if (pCore)
	{
		pCore->SetCreator(_T(""));
		pCore->SetLastModifiedBy(_T(""));
		pCore->write(pathDocProps + FILE_SEPARATOR_STR + _T("core.xml"), DocProps, oContentTypes);
		delete pCore;
	} 
/////////////////////////////////////////////////////////////////////////////////////

	pDocxWriter->m_oCommentsWriter.Write();
	pDocxWriter->m_oChartWriter.Write();
	pDocxWriter->m_oStylesWriter.Write();
	pDocxWriter->m_oNumberingWriter.Write();
	pDocxWriter->m_oFontTableWriter.Write();
	pDocxWriter->m_oHeaderFooterWriter.Write();
	//Setting пишем после HeaderFooter, чтобы заполнить evenAndOddHeaders
	pDocxWriter->m_oSettingWriter.Write();
	pDocxWriter->m_oWebSettingsWriter.Write();
	//Document пишем после HeaderFooter, чтобы заполнить sectPr
	pDocxWriter->m_oDocumentWriter.Write();
	//Rels и ContentTypes пишем в конце
	pDocxWriter->m_oDocumentRelsWriter.Write();
	pDocxWriter->m_oContentTypesWriter.Write();
}

