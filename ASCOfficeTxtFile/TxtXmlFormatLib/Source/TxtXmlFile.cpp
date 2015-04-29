#include <string>

#include "TxtXmlFile.h"

#include "Docx2Txt/Converter.h"
#include "Txt2Docx/Converter.h"

#include "Common/StlUtils.h"

#include "../../../Common/DocxFormat/Source/DocxFormat/Docx.h"

namespace NSBinPptxRW
{
	class  CDrawingConverter;
}
namespace BinDocxRW
{
	int g_nCurFormatVersion = 0;
}


#include "../../../ASCOfficeUtils/ASCOfficeUtilsLib/OfficeUtils.h"
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

    Writers::FileWriter *pDocxWriter =  new Writers::FileWriter(sDstPath, _T(""), 1, false, NULL, _T(""));
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

