#include "XlsxSerializer.h"

#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Path.h"
#include "../../XlsxSerializerCom/Reader/BinaryWriter.h"
#include "../../XlsxSerializerCom/Writer/BinaryReader.h"
#include "../../ASCOfficePPTXFile/Editor/FontPicker.h"

#ifdef _WIN32
	#include "../BinReader/DefaultThemeWriterWin.h"
#else
	#include "../BinReader/DefaultThemeWriterLinux.h"
#endif

namespace BinXlsxRW{
	int g_nCurFormatVersion = 0;

	CXlsxSerializer::CXlsxSerializer()
	{
		m_pExternalDrawingConverter = NULL;
	}
	CXlsxSerializer::~CXlsxSerializer()
	{
	}

	void CXlsxSerializer::CreateXlsxFolders(CString& sXmlOptions, CString sDstPath,  CString& sMediaPath)
	{
		OOX::CPath pathMediaDir = sDstPath + _T("\\xl\\media");
		
		// File Type (Можно парсить не два раза, а один, если передавать в ReadFile не опции, а параметры)
		BYTE fileType;
		UINT nCodePage;
		WCHAR wcDelimiter;
		SerializeCommon::ReadFileType(sXmlOptions, fileType, nCodePage, wcDelimiter);

		if (c_oFileTypes::CSV != fileType)
		{
			OOX::CPath pathXlDir = sDstPath + _T("\\xl");

			OOX::CPath pathThemeDir = pathXlDir + FILE_SEPARATOR_STR + OOX::FileTypes::Theme.DefaultDirectory().GetPath();
			
			OOX::CPath pathThemeFile = pathThemeDir + FILE_SEPARATOR_STR + OOX::FileTypes::Theme.DefaultFileName().GetPath();
			
			OOX::CPath pathThemeThemeRelsDir = pathThemeDir + _T("\\_rels");

			NSDirectory::CreateDirectory(string2std_string(pathXlDir.GetPath()));
			NSDirectory::CreateDirectory(string2std_string(pathThemeDir.GetPath()));
			NSDirectory::CreateDirectory(string2std_string(pathThemeThemeRelsDir.GetPath()));
			NSDirectory::CreateDirectory(string2std_string(pathMediaDir.GetPath()));

			//Create Default Theme
			{
				Writers::DefaultThemeWriter oDefaultThemeWriter;
				oDefaultThemeWriter.Write(pathThemeFile.GetPath());
			}
		}

		sMediaPath = pathMediaDir.GetPath();
	}
	bool CXlsxSerializer::loadFromFile(CString& sSrcFileName, CString& sDstPath, CString& sXMLOptions, CString& sMediaDir)
	{
		NSBinPptxRW::CDrawingConverter oOfficeDrawingConverter;
		oOfficeDrawingConverter.SetMediaDstPath(sMediaDir);

		//папка с бинарников
		std::wstring strFileInDir = NSSystemPath::GetDirectoryName(string2std_string(sSrcFileName));
		CString sFileInDir = strFileInDir.c_str();

		VARIANT var;
		var.vt = VT_BSTR;
#ifdef _WIN32
		var.bstrVal = sFileInDir.AllocSysString();
		oOfficeDrawingConverter.SetAdditionalParam(CString(L"SourceFileDir2"), var);
		RELEASESYSSTRING(var.bstrVal);
#else
		var.bstrVal = sFileInDir.GetString();
		oOfficeDrawingConverter.SetAdditionalParam(CString(L"SourceFileDir2"), var);
#endif

		BinXlsxRW::BinaryFileReader oBinaryFileReader;
		oBinaryFileReader.ReadFile(sSrcFileName, sDstPath, &oOfficeDrawingConverter, sXMLOptions);
		return true;
	}
	bool CXlsxSerializer::saveToFile(CString& sDstFileName, CString& sSrcPath, CString& sXMLOptions)
	{
		COfficeFontPicker* pFontPicker = new COfficeFontPicker();
		pFontPicker->Init(m_sFontDir);
		CFontManager* pFontManager = pFontPicker->get_FontManager();
		DocWrapper::FontProcessor fp;
		fp.setFontManager(pFontManager);
		
		NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager = NULL;
		if(false == m_sEmbeddedFontsDir.IsEmpty())
		{
			NSDirectory::CreateDirectory(string2std_string(m_sEmbeddedFontsDir));

			pFontPicker->SetEmbeddedFontsDirectory(m_sEmbeddedFontsDir);
			pEmbeddedFontsManager = pFontPicker->GetNativeCutter();

			//добавим мега шрифт
			pEmbeddedFontsManager->CheckFont(_T("Wingdings 3"), pFontManager);
			pEmbeddedFontsManager->CheckFont(_T("Arial"), pFontManager);
			//pEmbeddedFontsManager добавляются все цифры
			//для заголовков
			pEmbeddedFontsManager->CheckFont(_T("Calibri"), pFontManager);
			pEmbeddedFontsManager->CheckString(CString(_T("ABCDEFGHIJKLMNOPQRSTUVWXYZ")));

			//дополнение для ошибок "#NULL!", "#DIV/0!"...
			pEmbeddedFontsManager->CheckString(CString(_T("#!/?")));
			//дополнение для num форматов по умолчанию с id от 0 до 49
			pEmbeddedFontsManager->CheckString(CString(_T(".%E+-():")));
		}

		NSBinPptxRW::CDrawingConverter oOfficeDrawingConverter;
		oOfficeDrawingConverter.SetFontManager(pFontManager);
		oOfficeDrawingConverter.SetFontDir(m_sFontDir);
		oOfficeDrawingConverter.SetFontPicker(pFontPicker);

		BinXlsxRW::BinaryFileWriter oBinaryFileWriter(fp);
		oBinaryFileWriter.Open(sSrcPath, sDstFileName, pEmbeddedFontsManager, &oOfficeDrawingConverter, sXMLOptions);

		RELEASEOBJECT(pFontPicker);
		return true;
	}
	bool CXlsxSerializer::loadChart(CString& sChartPath, NSBinPptxRW::CBinaryFileWriter& oBufferedStream, long& lDataSize)
	{
		bool bRes = false;
		OOX::Spreadsheet::CChartSpace oChart(sChartPath);
		if(NULL != m_pExternalDrawingConverter)
		{
			long nStartPos = oBufferedStream.GetPosition();
			BinXlsxRW::BinaryCommonWriter oBcw(oBufferedStream);

			CString sOldRelsPath = m_pExternalDrawingConverter->GetRelsPath();
			m_pExternalDrawingConverter->SetRelsPath(sChartPath);

			BinXlsxRW::BinaryChartWriter oBinaryChartWriter(oBufferedStream, m_pExternalDrawingConverter);	
			oBinaryChartWriter.WriteCT_ChartSpace(oChart);

			m_pExternalDrawingConverter->SetRelsPath(sOldRelsPath);

			long nEndPos = oBufferedStream.GetPosition();
			lDataSize = nEndPos - nStartPos;
			bRes = true;
		}
		return bRes;
	}
	bool CXlsxSerializer::saveChart(NSBinPptxRW::CBinaryFileReader& oBufferedStream, long lLength, CString& sFilepath, CString& sContentTypePath, CString** sContentTypeElement)
	{
		bool bRes = false;
		*sContentTypeElement = NULL;
		if(NULL != m_pExternalDrawingConverter)
		{
			m_pExternalDrawingConverter->SetDstContentRels();

			//получаем sThemePath из bsFilename предполагая что папка theme находится на уровень выше bsFilename
			CString sThemePath;
			CString sFilenameReverse = sFilepath;sFilenameReverse.MakeReverse();
			
			int nIndex	= sFilenameReverse.Find(FILE_SEPARATOR_CHAR);
			nIndex		= sFilenameReverse.Find(FILE_SEPARATOR_CHAR, nIndex + 1);
			if(-1 != nIndex)
				sThemePath = sFilepath.Left(sFilepath.GetLength() - nIndex) + _T("theme");

			//todo theme path
			BinXlsxRW::SaveParams oSaveParams(sThemePath);
			OOX::Spreadsheet::CChartSpace oChartSpace;
			BinXlsxRW::BinaryChartReader oBinaryChartReader(oBufferedStream, oSaveParams, m_pExternalDrawingConverter);
			oBinaryChartReader.ReadCT_ChartSpace(lLength, &oChartSpace.m_oChartSpace);

			if(oChartSpace.isValid())
			{
				std::wstring strFilepath	= string2std_string(sFilepath);
				std::wstring strDir			= NSSystemPath::GetDirectoryName(strFilepath);
				std::wstring strFilename	= NSSystemPath::GetFileName(strFilepath);

				CString sRelsDir = strDir.c_str();
				CString sFilename = strFilename.c_str();
				sRelsDir.Append(_T("_rels"));
				
				OOX::CSystemUtility::CreateDirectories(sRelsDir);

				oChartSpace.write2(sFilepath);

				OOX::CPath pathRelsPath = sRelsDir + FILE_SEPARATOR_STR + sFilename + _T(".rels");
				m_pExternalDrawingConverter->SaveDstContentRels(pathRelsPath.GetPath());

				CString sContentType(sContentTypePath);
				sContentType.Append(sFilename);

				(*sContentTypeElement) = new CString();
				(*sContentTypeElement)->Format(_T("<Override PartName=\"%ls\" ContentType=\"application/vnd.openxmlformats-officedocument.drawingml.chart+xml\"/>"), sContentType);
				(*sContentTypeElement)->Append(oSaveParams.sAdditionalContentTypes);
				
				bRes = true;
			}
		}
		return bRes;
	}
	void CXlsxSerializer::setFontDir(CString& sFontDir)
	{
		m_sFontDir = sFontDir;
	}
	void CXlsxSerializer::setEmbeddedFontsDir(CString& sEmbeddedFontsDir)
	{
		m_sEmbeddedFontsDir = sEmbeddedFontsDir;
	}
	void CXlsxSerializer::setDrawingConverter(NSBinPptxRW::CDrawingConverter* pDrawingConverter)
	{
		m_pExternalDrawingConverter = pDrawingConverter;
	}
};