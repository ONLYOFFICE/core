#include "XlsxSerializer.h"

#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/common/File.h"
#include "../XlsxSerializerCom/Reader/BinaryWriter.h"
#include "../XlsxSerializerCom/Reader/FontProcessor.h"
#include "../XlsxSerializerCom/Writer/BinaryReader.h"

namespace BinXlsxRW{
	int g_nCurFormatVersion = 0;

	CXlsxSerializer::CXlsxSerializer()
	{
		m_pExternalDrawingConverter = NULL;
	}
	CXlsxSerializer::~CXlsxSerializer()
	{
	}
	bool CXlsxSerializer::loadFromFile(CString& sSrcFileName, CString& sDstPath, CString& sXMLOptions, CString& sMediaDir)
	{
		NSBinPptxRW::CDrawingConverter oOfficeDrawingConverter;
		BSTR bstrMediaDir = sMediaDir.AllocSysString();
		oOfficeDrawingConverter.SetMediaDstPath(bstrMediaDir);
		SysFreeString(bstrMediaDir);

		//папка с бинарников
		TCHAR tFolder[256];
		TCHAR tDrive[256];
		_tsplitpath( sSrcFileName, tDrive, tFolder, NULL, NULL );
		CString sFolder = CString(tFolder);
		CString sDrive = CString(tDrive);
		CString sFileInDir = sDrive + sFolder;

		VARIANT var;
		var.vt = VT_BSTR;
		var.bstrVal = sFileInDir.AllocSysString();
		oOfficeDrawingConverter.SetAdditionalParam(L"SourceFileDir2", var);
		RELEASESYSSTRING(var.bstrVal);

		BinXlsxRW::BinaryFileReader oBinaryFileReader;
		oBinaryFileReader.ReadFile(sSrcFileName, sDstPath, &oOfficeDrawingConverter, sXMLOptions);
		return true;
	}
	bool CXlsxSerializer::saveToFile(CString& sDstFileName, CString& sSrcPath, CString& sXMLOptions)
	{
		BinXlsxRW::FontProcessor fp;
		fp.setFontDir(m_sFontDir);
		PPTXFile::IOfficeFontPicker* pFontPicker = NULL;
		CoCreateInstance(__uuidof(PPTXFile::COfficeFontPicker), NULL, CLSCTX_ALL, __uuidof(PPTXFile::IOfficeFontPicker), (void**)(&pFontPicker));
		BSTR bstrFontDir1 = m_sFontDir.AllocSysString();
		pFontPicker->Init(bstrFontDir1);
		SysFreeString(bstrFontDir1);
		NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager = NULL;
		if(false == m_sEmbeddedFontsDir.IsEmpty())
		{
			NSDirectory::CreateDirectory(string2std_string(m_sEmbeddedFontsDir));

			BSTR bstrEmbeddedFontsDirectory = m_sEmbeddedFontsDir.AllocSysString();
			pFontPicker->SetEmbeddedFontsDirectory(bstrEmbeddedFontsDirectory);
			SysFreeString(bstrEmbeddedFontsDirectory);

			VARIANT vt;
			pFontPicker->GetAdditionalParam(_T("NativeCutter"), &vt);
			pEmbeddedFontsManager = (NSFontCutter::CEmbeddedFontsManager*)vt.pvRecord;

			ASCGraphics::IASCFontManager* pFontManager = fp.getFontManager();
			//добавим мега шрифт
			pEmbeddedFontsManager->CheckFont(_T("Wingdings 3"), pFontManager);
			pEmbeddedFontsManager->CheckFont(_T("Arial"), pFontManager);
			//pEmbeddedFontsManager добавл€ютс€ все цифры
			//дл€ заголовков
			pEmbeddedFontsManager->CheckFont(_T("Calibri"), pFontManager);
			pEmbeddedFontsManager->CheckString(CString(_T("ABCDEFGHIJKLMNOPQRSTUVWXYZ")));

			//дополнение дл€ ошибок "#NULL!", "#DIV/0!"...
			pEmbeddedFontsManager->CheckString(CString(_T("#!/?")));
			//дополнение дл€ num форматов по умолчанию с id от 0 до 49
			pEmbeddedFontsManager->CheckString(CString(_T(".%E+-():")));
		}

		NSBinPptxRW::CDrawingConverter oOfficeDrawingConverter;

		BSTR bstrFontDir = m_sFontDir.AllocSysString();
		oOfficeDrawingConverter.SetFontDir(bstrFontDir);
		SysFreeString(bstrFontDir);
		VARIANT vt;
		vt.vt = VT_UNKNOWN;
		vt.punkVal = pFontPicker;
		oOfficeDrawingConverter.SetAdditionalParam(_T("FontPicker"), vt);

		BinXlsxRW::BinaryFileWriter oBinaryFileWriter(fp);
		oBinaryFileWriter.Open(sSrcPath, sDstFileName, pEmbeddedFontsManager, &oOfficeDrawingConverter, sXMLOptions);

		RELEASEINTERFACE(pFontPicker);
		return true;
	}
	bool CXlsxSerializer::loadChart(CString& sChartPath, unsigned char** ppBinary, long& lDataSize)
	{
		bool bRes = false;
		*ppBinary = NULL;
		OOX::Spreadsheet::CChartSpace oChart(sChartPath);
		if(NULL != m_pExternalDrawingConverter)
		{
			NSBinPptxRW::CBinaryFileWriter oBufferedStream;
			BinXlsxRW::BinaryCommonWriter oBcw(oBufferedStream);

			BSTR bstrChartPath = sChartPath.AllocSysString();
			m_pExternalDrawingConverter->SetRelsPath(bstrChartPath);
			SysFreeString(bstrChartPath);

			BinXlsxRW::BinaryChartWriter oBinaryChartWriter(oBufferedStream, m_pExternalDrawingConverter);	
			oBinaryChartWriter.WriteCT_ChartSpace(oChart);

			if (NULL != ppBinary)
			{
				lDataSize = oBufferedStream.GetPosition();
				*ppBinary = new unsigned char[lDataSize];
				BYTE* pDataS = oBufferedStream.GetBuffer();
				memcpy(*ppBinary, pDataS, lDataSize);
				bRes = true;
			}
		}
		return bRes;
	}
	bool CXlsxSerializer::saveChart(SAFEARRAY* pBinaryObj, long lStart, long lLength, CString& sFilepath, CString& sContentTypePath, CString** sContentTypeElement)
	{
		bool bRes = false;
		*sContentTypeElement = NULL;
		if(NULL != m_pExternalDrawingConverter)
		{
			NSBinPptxRW::CBinaryFileReader oBufferedStream;
			oBufferedStream.Init((BYTE*)pBinaryObj->pvData, lStart, lLength);

			m_pExternalDrawingConverter->SetDstContentRels();

			//получаем sThemePath из bsFilename предполага€ что папка theme находитс€ на уровень выше bsFilename
			CString sThemePath;
			CString sFilenameReverse = sFilepath;sFilenameReverse.MakeReverse();
			int nIndex = sFilenameReverse.Find('\\');
			nIndex = sFilenameReverse.Find('\\', nIndex + 1);
			if(-1 != nIndex)
				sThemePath = sFilepath.Left(sFilepath.GetLength() - nIndex) + _T("theme");

			//todo theme path
			BinXlsxRW::SaveParams oSaveParams(sThemePath);
			OOX::Spreadsheet::CChartSpace oChartSpace;
			BinXlsxRW::BinaryChartReader oBinaryChartReader(oBufferedStream, oSaveParams, pBinaryObj, m_pExternalDrawingConverter);
			oBinaryChartReader.ReadCT_ChartSpace(lLength, &oChartSpace.m_oChartSpace);

			if(oChartSpace.isValid())
			{
				TCHAR tDrive[256];
				TCHAR tFolder[256];
				TCHAR tFilename[256];
				TCHAR tExt[256];
				_tsplitpath( sFilepath, tDrive, tFolder, tFilename, tExt );
				CString sDrive(tDrive);
				CString sFolder(tFolder);
				CString sFilename(tFilename);
				CString sExt(tExt);

				CString sRelsDir = sDrive + sFolder;
				sRelsDir.Append(_T("_rels"));
				DWORD dwFileAttr = ::GetFileAttributes( sRelsDir );
				if( dwFileAttr == INVALID_FILE_ATTRIBUTES )
					OOX::CSystemUtility::CreateDirectories(sRelsDir);

				oChartSpace.write2(sFilepath);

				CString sRelsPath;
				sRelsPath.Format(_T("%s\\%s.rels"), sRelsDir, sFilename + sExt);

				BSTR bstrRelsPath = sRelsPath.AllocSysString();
				m_pExternalDrawingConverter->SaveDstContentRels(bstrRelsPath);
				SysFreeString(bstrRelsPath);

				CString sContentType(sContentTypePath);
				sContentType.Append(sFilename);
				sContentType.Append(sExt);

				(*sContentTypeElement) = new CString();
				(*sContentTypeElement)->Format(_T("<Override PartName=\"%s\" ContentType=\"application/vnd.openxmlformats-officedocument.drawingml.chart+xml\"/>"), sContentType);
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