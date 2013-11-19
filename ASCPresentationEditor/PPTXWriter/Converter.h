#pragma once
#include "AvsUtils.h"
#include "../OfficeDrawing/Document.h"
#include "../../Common/DocxFormat/Source/SystemUtility/File.h"

#include "ShapeWriter.h"
#include "StylesWriter.h"
#include "ImageManager.h"

#import "../../../Redist/AVSOfficeStudio/AVSOfficeUtils.dll"		named_guids rename_namespace("AVSOfficeUtils"), raw_interfaces_only

namespace NSPresentationEditor
{
	namespace NSPPTXWriterConst
	{
		static CString g_string_rels_presentation = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\
<Relationship Id=\"rId3\" Type=\"http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties\" Target=\"docProps/core.xml\"/>\
<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument\" Target=\"ppt/presentation.xml\"/>\
<Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties\" Target=\"docProps/app.xml\"/>\
</Relationships>");

		static CString g_string_core = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
<cp:coreProperties xmlns:cp=\"http://schemas.openxmlformats.org/package/2006/metadata/core-properties\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:dcterms=\"http://purl.org/dc/terms/\" xmlns:dcmitype=\"http://purl.org/dc/dcmitype/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\
<dc:title>Slide 1</dc:title>\
<dc:creator>teamlab</dc:creator>\
<cp:lastModifiedBy>teamlab</cp:lastModifiedBy>\
<cp:revision>1</cp:revision>\
</cp:coreProperties>");
	}

	class CPPTXWriter
	{
	public:
		CString	m_strTempDirectory;
		CString m_strDstFileName;

		CDocument* m_pDocument;
		CImageManager m_oManager;

		CShapeWriter* m_pShapeWriter;

	public:
		CPPTXWriter()
		{
			m_strTempDirectory	= _T("C:\\TempPPTX");
			m_strDstFileName	= _T("C:\\Test.pptx");

			m_pDocument			= NULL;

			IAVSRenderer* pRenderer = NULL;
			CoCreateInstance(__uuidof(CShapeWriter), NULL, CLSCTX_ALL, __uuidof(IAVSRenderer), (void**)&pRenderer);

			m_pShapeWriter = (CShapeWriter*)pRenderer;
		}
		~CPPTXWriter()
		{
			m_pShapeWriter->Release();
		}

		void CreateFile(CDocument* pDocument)
		{
			m_pDocument = pDocument;
			m_oManager.Clear();
			m_oManager.SetDstMedia(m_strTempDirectory + _T("\\ppt\\media\\"));

			m_pShapeWriter->InitNextId();

			CDirectory::CreateDirectory(m_strTempDirectory);
			CFile oFile;
			CString strMemory = _T("");

			// _rels -------------------------------------------------------------
			CDirectory::CreateDirectory(m_strTempDirectory + _T("\\_rels"));
			oFile.CreateFile(m_strTempDirectory + _T("\\_rels\\.rels"));
			strMemory = NSPPTXWriterConst::g_string_rels_presentation;
			oFile.WriteStringUTF8(strMemory);
			oFile.CloseFile();
			// -------------------------------------------------------------------

			// docProps ----------------------------------------------------------
			CDirectory::CreateDirectory(m_strTempDirectory + _T("\\docProps"));
			
			// core
			oFile.CreateFile(m_strTempDirectory + _T("\\docProps\\core.xml"));
			strMemory = NSPPTXWriterConst::g_string_core;
			oFile.WriteStringUTF8(strMemory);
			oFile.CloseFile();

			// app
			oFile.CreateFile(m_strTempDirectory + _T("\\docProps\\app.xml"));
			WriteApp(oFile);
			oFile.CloseFile();
			// -------------------------------------------------------------------

			// content types
			WriteContentTypes();
			// -------------------------------------------------------------------

			// ppt ---------------------------------------------------------------
			CDirectory::CreateDirectory(m_strTempDirectory + _T("\\ppt"));
			WritePresInfo();
			// -------------------------------------------------------------------		
			WriteAll();
		}
		void CloseFile()
		{
			// здесь только архивация и удаление temp
			AVSOfficeUtils::IOfficeUtils* pOfficeUtils = NULL;
			if (S_OK != CoCreateInstance(__uuidof(AVSOfficeUtils::COfficeUtils), NULL, CLSCTX_INPROC_SERVER, __uuidof(AVSOfficeUtils::IOfficeUtils),(void**)&pOfficeUtils))
				return;

			// архивируем
			BSTR bstrSrcPath = m_strTempDirectory.AllocSysString();
			BSTR bstrDstPath = m_strDstFileName.AllocSysString();
			pOfficeUtils->CompressFileOrDirectory( bstrSrcPath, bstrDstPath, -1 );
			SysFreeString(bstrSrcPath);
			SysFreeString(bstrDstPath);
			RELEASEINTERFACE(pOfficeUtils);

			// удаляем
			RemoveDirOrFile(m_strTempDirectory);
			m_oManager.Clear();
		}

		void CloseFile2()
		{
			RemoveDirOrFile(m_strTempDirectory);
			m_oManager.Clear();
		}

		void Write()
		{
		}

		void WriteContentTypes()
		{
			CString strContentTypes = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
<Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\">\
<Default Extension=\"png\" ContentType=\"image/png\" />\
<Default Extension=\"jpeg\" ContentType=\"image/jpeg\" />\
<Default Extension=\"wmf\" ContentType=\"image/x-wmf\" />\
<Default Extension=\"rels\" ContentType=\"application/vnd.openxmlformats-package.relationships+xml\" />\
<Default Extension=\"xml\" ContentType=\"application/xml\" />\
<Default Extension=\"gif\" ContentType=\"image/gif\"/>\
<Default Extension=\"emf\" ContentType=\"image/x-emf\"/>\
<Default Extension=\"jpg\" ContentType=\"application/octet-stream\"/>");

			strContentTypes += _T("<Override PartName=\"/ppt/presentation.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml\" />\
<Override PartName=\"/ppt/presProps.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.presProps+xml\" />\
<Override PartName=\"/ppt/viewProps.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.viewProps+xml\" />\
<Override PartName=\"/ppt/tableStyles.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.tableStyles+xml\"/>\
<Override PartName=\"/docProps/core.xml\" ContentType=\"application/vnd.openxmlformats-package.core-properties+xml\" />\
<Override PartName=\"/docProps/app.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.extended-properties+xml\" />");

			int nThemes = (int)m_pDocument->m_arThemes.GetCount();
			int nIndexLayout = 0;
			for (int nT = 0; nT < nThemes; ++nT)
			{
				CString strTheme = _T("");
				strTheme.Format(_T("<Override PartName=\"/ppt/theme/theme%d.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.theme+xml\" />\
<Override PartName=\"/ppt/slideMasters/slideMaster%d.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.slideMaster+xml\" />"), 
						nT + 1, nT + 1);

				strContentTypes += strTheme;

				int nCountL = (int)m_pDocument->m_arThemes[nT].m_arLayouts.GetCount();
				for (int nL = 0; nL < nCountL; ++nL, ++nIndexLayout)
				{
					CString strL = _T("");
					strL.Format(_T("<Override PartName=\"/ppt/slideLayouts/slideLayout%d.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.slideLayout+xml\" />"), nIndexLayout + 1);
					strContentTypes += strL;
				}
			}
			CString strT = _T("");
			strT.Format(_T("<Override PartName=\"/ppt/theme/theme%d.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.theme+xml\"/>"), nThemes + 1);
			strContentTypes += strT;

			strContentTypes += _T("<Override PartName=\"/ppt/notesMasters/notesMaster1.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.notesMaster+xml\"/>");

			int nCountS = (int)m_pDocument->m_arSlides.GetCount();
			for (int nS = 0; nS < nCountS; ++nS)
			{
				CString strS = _T("");
				strS.Format(_T("<Override PartName=\"/ppt/slides/slide%d.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.slide+xml\" />"), nS + 1);
				strContentTypes += strS;

				CString strN = _T("");
				strN.Format(_T("<Override PartName=\"/ppt/notesSlides/notesSlide%d.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.notesSlide+xml\"/>"), nS + 1);
				strContentTypes += strN;
			}

			strContentTypes += _T("</Types>");

			CFile oFile;
			oFile.CreateFile(m_strTempDirectory + _T("\\[Content_Types].xml"));
			oFile.WriteStringUTF8(strContentTypes);
			oFile.CloseFile();
		}

	protected:
		void WriteApp(CFile& oFile)
		{
			CString str1 = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
<Properties xmlns=\"http://schemas.openxmlformats.org/officeDocument/2006/extended-properties\" xmlns:vt=\"http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes\">\
<TotalTime>0</TotalTime>\
<Words>0</Words>\
<Application>Microsoft Office PowerPoint</Application>\
<PresentationFormat>On-screen Show (4:3)</PresentationFormat>\
<Paragraphs>0</Paragraphs>");

			oFile.WriteStringUTF8(str1);

			CString str2 = _T("");
			str2.Format(_T("<Slides>%d</Slides>"), (int)m_pDocument->m_arSlides.GetCount());
			oFile.WriteStringUTF8(str2);

			CString str3 = _T("<Notes>0</Notes>\
<HiddenSlides>0</HiddenSlides>\
<MMClips>2</MMClips>\
<ScaleCrop>false</ScaleCrop>\
<HeadingPairs>\
<vt:vector size=\"4\" baseType=\"variant\">");
			oFile.WriteStringUTF8(str3);

			int nCountThemes = (int)m_pDocument->m_arThemes.GetCount();
			int nCountSlides = (int)m_pDocument->m_arSlides.GetCount();

			CString strThemes = _T("");
			strThemes.Format(_T("<vt:variant><vt:lpstr>Theme</vt:lpstr></vt:variant><vt:variant><vt:i4>%d</vt:i4></vt:variant>"), nCountThemes);

			CString strSlides = _T("");
			strSlides.Format(_T("<vt:variant><vt:lpstr>Slide Titles</vt:lpstr></vt:variant><vt:variant><vt:i4>%d</vt:i4></vt:variant></vt:vector></HeadingPairs>"), nCountSlides);

			CString str4 = _T("");
			str4.Format(_T("<TitlesOfParts><vt:vector size=\"%d\" baseType=\"lpstr\">"), nCountSlides + nCountThemes);

			str4 = strThemes + strSlides + str4;
			oFile.WriteStringUTF8(str4);

			CString strMemory = _T("");
			for (int i = 1; i <= nCountThemes; ++i)
			{
				CString strMem = _T("");
				strMem.Format(_T("<vt:lpstr>Theme %d</vt:lpstr>"), i);
				strMemory += strMem;
			}
			for (int i = 1; i <= nCountSlides; ++i)
			{
				CString strMem = _T("");
				strMem.Format(_T("<vt:lpstr>Slide %d</vt:lpstr>"), i);
				strMemory += strMem;
			}

			CString str5 = _T("</vt:vector>\
</TitlesOfParts>\
<Company></Company>\
<LinksUpToDate>false</LinksUpToDate>\
<SharedDoc>false</SharedDoc>\
<HyperlinksChanged>false</HyperlinksChanged>\
<AppVersion>1.0000</AppVersion>\
</Properties>");

			strMemory += str5;

			oFile.WriteStringUTF8(strMemory);
		}

		void WritePresInfo()
		{
			CFile oFile;
			
			// tableStyles.xml
			oFile.CreateFile(m_strTempDirectory + _T("\\ppt\\tableStyles.xml"));
			CString str = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
<a:tblStyleLst xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" def=\"{5C22544A-7EE6-4342-B048-85BDC9FD1C3A}\" />");
			oFile.WriteStringUTF8(str);
			oFile.CloseFile();

			// presProps.xml
			str = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
<p:presentationPr xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\">\
<p:extLst><p:ext uri=\"{E76CE94A-603C-4142-B9EB-6D1370010A27}\">\
<p14:discardImageEditData xmlns:p14=\"http://schemas.microsoft.com/office/powerpoint/2010/main\" val=\"0\" /></p:ext>\
<p:ext uri=\"{D31A062A-798A-4329-ABDD-BBA856620510}\">\
<p14:defaultImageDpi xmlns:p14=\"http://schemas.microsoft.com/office/powerpoint/2010/main\" val=\"220\" />\
</p:ext></p:extLst></p:presentationPr>");
			oFile.CreateFile(m_strTempDirectory + _T("\\ppt\\presProps.xml"));
			oFile.WriteStringUTF8(str);
			oFile.CloseFile();

			// viewProps.xml
			str = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
<p:viewPr xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\">\
<p:normalViewPr><p:restoredLeft sz=\"15620\"/><p:restoredTop sz=\"94660\"/></p:normalViewPr><p:slideViewPr><p:cSldViewPr><p:cViewPr varScale=\"1\">\
<p:scale><a:sx n=\"104\" d=\"100\"/><a:sy n=\"104\" d=\"100\"/></p:scale><p:origin x=\"-1236\" y=\"-90\"/></p:cViewPr><p:guideLst>\
<p:guide orient=\"horz\" pos=\"2160\"/><p:guide pos=\"2880\"/></p:guideLst></p:cSldViewPr></p:slideViewPr><p:notesTextViewPr><p:cViewPr><p:scale>\
<a:sx n=\"1\" d=\"1\"/><a:sy n=\"1\" d=\"1\"/></p:scale><p:origin x=\"0\" y=\"0\"/></p:cViewPr></p:notesTextViewPr><p:gridSpacing cx=\"72008\" cy=\"72008\"/></p:viewPr>");
			oFile.CreateFile(m_strTempDirectory + _T("\\ppt\\viewProps.xml"));
			oFile.WriteStringUTF8(str);
			oFile.CloseFile();

			// presentation.xml + _rels/presentation.xml.rels
			CString strPresRels;

			CString strPresMasters	= _T("");
			CString strPresSlides	= _T("");

			size_t nCountLayouts = 0;
			size_t nCountThemes = m_pDocument->m_arThemes.GetCount();
			for (size_t nIndexTheme = 0; nIndexTheme < nCountThemes; ++nIndexTheme)
			{
				CString strRels = _T("");
				strRels.Format(_T("<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideMaster\" Target=\"slideMasters/slideMaster%d.xml\" />"), 2 * nIndexTheme + 1, nIndexTheme + 1);
				strPresRels += strRels;

				strRels = _T("");
				strRels.Format(_T("<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"theme/theme%d.xml\" />"), 2 * nIndexTheme + 2, nIndexTheme + 1);
				strPresRels += strRels;

				strRels = _T("");
				strRels.Format(_T("<p:sldMasterId id=\"%u\" r:id=\"rId%d\" />"), 0x80000000 + nCountLayouts, 2 * nIndexTheme + 1);
				nCountLayouts += m_pDocument->m_arThemes[nIndexTheme].m_arLayouts.GetCount();
				nCountLayouts += 1;

				strPresMasters += strRels;
			}

			int nCurrentRels = (int)(2 * nCountThemes + 1);
			size_t nCountSlides = m_pDocument->m_arSlides.GetCount();
			for (size_t nIndexSlide = 0; nIndexSlide < nCountSlides; ++nIndexSlide, ++nCurrentRels)
			{
				CString strRels = _T("");
				strRels.Format(_T("<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide\" Target=\"slides/slide%d.xml\" />"), nCurrentRels, nIndexSlide + 1);
				strPresRels += strRels;

				strRels = _T("");
				strRels.Format(_T("<p:sldId id=\"%d\" r:id=\"rId%d\" />"), 256 + nIndexSlide, nCurrentRels);
				strPresSlides += strRels;
			}

			CString strNotesIDs = _T("");
			strNotesIDs.Format(_T("<p:notesMasterIdLst><p:notesMasterId r:id=\"rId%d\"/></p:notesMasterIdLst>"), nCurrentRels);

			CString strRels0 = _T("");
			strRels0.Format(_T("<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/notesMaster\" Target=\"notesMasters/notesMaster1.xml\"/>"), nCurrentRels);

			++nCurrentRels;

			CString strRels1 = _T("");
			strRels1.Format(_T("<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/presProps\" Target=\"presProps.xml\" />"), nCurrentRels++);
			CString strRels2 = _T("");
			strRels2.Format(_T("<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/tableStyles\" Target=\"tableStyles.xml\" />"), nCurrentRels++);
			CString strRels3 = _T("");
			strRels3.Format(_T("<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/viewProps\" Target=\"viewProps.xml\" />"), nCurrentRels++);

			strPresRels = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">") + strPresRels + strRels0 + strRels1 + strRels2 + strRels3 + _T("</Relationships>");

			CDirectory::CreateDirectory(m_strTempDirectory + _T("\\ppt\\_rels"));
			oFile.CreateFile(m_strTempDirectory + _T("\\ppt\\_rels\\presentation.xml.rels"));
			oFile.WriteStringUTF8(strPresRels);
			oFile.CloseFile();

			CString strSizePres = _T("");
			strSizePres.Format(_T("<p:sldSz cx=\"%d\" cy=\"%d\" type=\"screen4x3\" /><p:notesSz cx=\"%d\" cy=\"%d\" />"), 
				m_pDocument->m_oInfo.m_lUnitsHor, m_pDocument->m_oInfo.m_lUnitsVer, m_pDocument->m_oInfo.m_lUnitsVer, m_pDocument->m_oInfo.m_lUnitsHor);

			CString strDefaultTextStyle = _T("<p:defaultTextStyle>");
			strDefaultTextStyle += CStylesWriter::ConvertStyles(m_pDocument->m_arThemes[0].m_pStyles[0], m_pDocument->m_oInfo, 9);
			strDefaultTextStyle += _T("</p:defaultTextStyle>");

			CString strPres = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
<p:presentation xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\" saveSubsetFonts=\"1\">\
<p:sldMasterIdLst>") + strPresMasters + _T("</p:sldMasterIdLst>") + strNotesIDs + _T("<p:sldIdLst>") + strPresSlides + _T("</p:sldIdLst>") + strSizePres + strDefaultTextStyle + _T("</p:presentation>");
			oFile.CreateFile(m_strTempDirectory + _T("\\ppt\\presentation.xml"));
			oFile.WriteStringUTF8(strPres);
			oFile.CloseFile();		
		}

		void WriteAll()
		{
			CDirectory::CreateDirectory(m_strTempDirectory + _T("\\ppt\\media"));
			CDirectory::CreateDirectory(m_strTempDirectory + _T("\\ppt\\theme"));
			CDirectory::CreateDirectory(m_strTempDirectory + _T("\\ppt\\slideMasters"));
			CDirectory::CreateDirectory(m_strTempDirectory + _T("\\ppt\\slideMasters\\_rels"));
			CDirectory::CreateDirectory(m_strTempDirectory + _T("\\ppt\\slideLayouts"));
			CDirectory::CreateDirectory(m_strTempDirectory + _T("\\ppt\\slideLayouts\\_rels"));
			CDirectory::CreateDirectory(m_strTempDirectory + _T("\\ppt\\slides"));
			CDirectory::CreateDirectory(m_strTempDirectory + _T("\\ppt\\slides\\_rels"));
			CDirectory::CreateDirectory(m_strTempDirectory + _T("\\ppt\\notesMasters"));
			CDirectory::CreateDirectory(m_strTempDirectory + _T("\\ppt\\notesMasters\\_rels"));
			CDirectory::CreateDirectory(m_strTempDirectory + _T("\\ppt\\notesSlides"));
			CDirectory::CreateDirectory(m_strTempDirectory + _T("\\ppt\\notesSlides\\_rels"));
			
			HINSTANCE hInst = _AtlBaseModule.GetModuleInstance();
			CString strThemeNotes = _T("");
			strThemeNotes.Format(_T("\\ppt\\theme\\theme%d.xml"), (int)m_pDocument->m_arThemes.GetCount() + 1);
			LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_XML_NOTESTHEME), _T("PPTXW"), m_strTempDirectory + strThemeNotes);
			LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_XML_NOTESMASTER), _T("PPTXW"), m_strTempDirectory + _T("\\ppt\\notesMasters\\notesMaster1.xml"));
		
			// - notesMasterRels -----------------------------------
			CDirectory::CreateDirectory(m_strTempDirectory + _T("\\ppt\\notesMasters\\_rels"));
			CString strThemeNotesNum = _T("");
			strThemeNotesNum.Format(_T("%d"), (int)m_pDocument->m_arThemes.GetCount() + 1);
			CString strVal = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\
<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"../theme/theme") + strThemeNotesNum + _T(".xml\"/></Relationships>");
			CFile oFileRels;
			oFileRels.CreateFile(m_strTempDirectory + _T("\\ppt\\notesMasters\\_rels\\notesMaster1.xml.rels"));
			oFileRels.WriteStringUTF8(strVal);
			oFileRels.CloseFile();
			// -----------------------------------------------------

			int nCount = (int)m_pDocument->m_arThemes.GetCount();
			int nStartLayout = 0;
			for (int nIndexTheme = 0; nIndexTheme < nCount; ++nIndexTheme)
			{
				CString strThemeXml = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
<a:theme xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" name=\"default\">\
<a:themeElements><a:clrScheme name=\"default 1\">");

				CTheme* pTheme = &m_pDocument->m_arThemes[nIndexTheme];

				CString strVal;
				
				strVal = _T(""); strVal.Format(_T("<a:dk1><a:srgbClr val=\"%06x\"/></a:dk1>"), pTheme->m_arColorScheme[14].GetLONG_RGB());
				strThemeXml += strVal;
				strVal = _T(""); strVal.Format(_T("<a:lt1><a:srgbClr val=\"%06x\"/></a:lt1>"), pTheme->m_arColorScheme[13].GetLONG_RGB());
				strThemeXml += strVal;
				strVal = _T(""); strVal.Format(_T("<a:dk2><a:srgbClr val=\"%06x\"/></a:dk2>"), pTheme->m_arColorScheme[16].GetLONG_RGB());
				strThemeXml += strVal;
				strVal = _T(""); strVal.Format(_T("<a:lt2><a:srgbClr val=\"%06x\"/></a:lt2>"), pTheme->m_arColorScheme[15].GetLONG_RGB());
				strThemeXml += strVal;
				strVal = _T(""); strVal.Format(_T("<a:accent1><a:srgbClr val=\"%06x\"/></a:accent1>"), pTheme->m_arColorScheme[5].GetLONG_RGB());
				strThemeXml += strVal;
				strVal = _T(""); strVal.Format(_T("<a:accent2><a:srgbClr val=\"%06x\"/></a:accent2>"), pTheme->m_arColorScheme[6].GetLONG_RGB());
				strThemeXml += strVal;
				strVal = _T(""); strVal.Format(_T("<a:accent3><a:srgbClr val=\"%06x\"/></a:accent3>"), pTheme->m_arColorScheme[7].GetLONG_RGB());
				strThemeXml += strVal;
				strVal = _T(""); strVal.Format(_T("<a:accent4><a:srgbClr val=\"%06x\"/></a:accent4>"), pTheme->m_arColorScheme[8].GetLONG_RGB());
				strThemeXml += strVal;
				strVal = _T(""); strVal.Format(_T("<a:accent5><a:srgbClr val=\"%06x\"/></a:accent5>"), pTheme->m_arColorScheme[9].GetLONG_RGB());
				strThemeXml += strVal;
				strVal = _T(""); strVal.Format(_T("<a:accent6><a:srgbClr val=\"%06x\"/></a:accent6>"), pTheme->m_arColorScheme[10].GetLONG_RGB());
				strThemeXml += strVal;
				strVal = _T(""); strVal.Format(_T("<a:hlink><a:srgbClr val=\"%06x\"/></a:hlink>"), pTheme->m_arColorScheme[11].GetLONG_RGB());
				strThemeXml += strVal;
				strVal = _T(""); strVal.Format(_T("<a:folHlink><a:srgbClr val=\"%06x\"/></a:folHlink>"), pTheme->m_arColorScheme[12].GetLONG_RGB());
				strThemeXml += strVal;

				strThemeXml += _T("</a:clrScheme>");

				CString strFonts = _T("");

				CString sFont1 = _T("");
				if (0 < pTheme->m_arFonts.GetCount())
					sFont1 = pTheme->m_arFonts[0].Name;
				CString sFont2 = _T("");
				if (1 < pTheme->m_arFonts.GetCount())
					sFont2 = pTheme->m_arFonts[1].Name;

				strFonts.Format(_T("<a:fontScheme name=\"default\"><a:majorFont><a:latin typeface=\"%d\"/><a:ea typeface=\"\"/><a:cs typeface=\"\"/>\
</a:majorFont><a:minorFont><a:latin typeface=\"%s\"/><a:ea typeface=\"\"/><a:cs typeface=\"\"/></a:minorFont></a:fontScheme>"), 
						sFont1, sFont2);

				strThemeXml += strFonts;

				strThemeXml += _T("<a:fmtScheme name=\"Default\">\
<a:fillStyleLst><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill><a:gradFill rotWithShape=\"1\"><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\">\
<a:tint val=\"50000\"/><a:satMod val=\"300000\"/></a:schemeClr></a:gs><a:gs pos=\"35000\"><a:schemeClr val=\"phClr\"><a:tint val=\"37000\"/>\
<a:satMod val=\"300000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:tint val=\"15000\"/><a:satMod val=\"350000\"/>\
</a:schemeClr></a:gs></a:gsLst><a:lin ang=\"16200000\" scaled=\"1\"/></a:gradFill><a:gradFill rotWithShape=\"1\"><a:gsLst><a:gs pos=\"0\">\
<a:schemeClr val=\"phClr\"><a:shade val=\"51000\"/><a:satMod val=\"130000\"/></a:schemeClr></a:gs><a:gs pos=\"80000\"><a:schemeClr val=\"phClr\">\
<a:shade val=\"93000\"/><a:satMod val=\"130000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:shade val=\"94000\"/>\
<a:satMod val=\"135000\"/></a:schemeClr></a:gs></a:gsLst><a:lin ang=\"16200000\" scaled=\"0\"/></a:gradFill></a:fillStyleLst><a:lnStyleLst>\
<a:ln w=\"9525\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"phClr\"><a:shade val=\"95000\"/><a:satMod val=\"105000\"/>\
</a:schemeClr></a:solidFill><a:prstDash val=\"solid\"/></a:ln><a:ln w=\"25400\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill>\
<a:schemeClr val=\"phClr\"/></a:solidFill><a:prstDash val=\"solid\"/></a:ln><a:ln w=\"38100\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\">\
<a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill><a:prstDash val=\"solid\"/></a:ln></a:lnStyleLst><a:effectStyleLst><a:effectStyle><a:effectLst>\
<a:outerShdw blurRad=\"40000\" dist=\"20000\" dir=\"5400000\" rotWithShape=\"0\"><a:srgbClr val=\"000000\"><a:alpha val=\"38000\"/></a:srgbClr>\
</a:outerShdw></a:effectLst></a:effectStyle><a:effectStyle><a:effectLst><a:outerShdw blurRad=\"40000\" dist=\"23000\" dir=\"5400000\" rotWithShape=\"0\">\
<a:srgbClr val=\"000000\"><a:alpha val=\"35000\"/></a:srgbClr></a:outerShdw></a:effectLst></a:effectStyle><a:effectStyle><a:effectLst>\
<a:outerShdw blurRad=\"40000\" dist=\"23000\" dir=\"5400000\" rotWithShape=\"0\"><a:srgbClr val=\"000000\"><a:alpha val=\"35000\"/></a:srgbClr>\
</a:outerShdw></a:effectLst><a:scene3d><a:camera prst=\"orthographicFront\"><a:rot lat=\"0\" lon=\"0\" rev=\"0\"/></a:camera>\
<a:lightRig rig=\"threePt\" dir=\"t\"><a:rot lat=\"0\" lon=\"0\" rev=\"1200000\"/></a:lightRig></a:scene3d><a:sp3d><a:bevelT w=\"63500\" h=\"25400\"/>\
</a:sp3d></a:effectStyle></a:effectStyleLst><a:bgFillStyleLst><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill><a:gradFill rotWithShape=\"1\">\
<a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:tint val=\"40000\"/><a:satMod val=\"350000\"/></a:schemeClr></a:gs><a:gs pos=\"40000\">\
<a:schemeClr val=\"phClr\"><a:tint val=\"45000\"/><a:shade val=\"99000\"/><a:satMod val=\"350000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\">\
<a:schemeClr val=\"phClr\"><a:shade val=\"20000\"/><a:satMod val=\"255000\"/></a:schemeClr></a:gs></a:gsLst><a:path path=\"circle\">\
<a:fillToRect l=\"50000\" t=\"-80000\" r=\"50000\" b=\"180000\"/></a:path></a:gradFill><a:gradFill rotWithShape=\"1\"><a:gsLst><a:gs pos=\"0\">\
<a:schemeClr val=\"phClr\"><a:tint val=\"80000\"/><a:satMod val=\"300000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\">\
<a:shade val=\"30000\"/><a:satMod val=\"200000\"/></a:schemeClr></a:gs></a:gsLst><a:path path=\"circle\">\
<a:fillToRect l=\"50000\" t=\"50000\" r=\"50000\" b=\"50000\"/></a:path></a:gradFill></a:bgFillStyleLst></a:fmtScheme>");

				strThemeXml += _T("</a:themeElements><a:objectDefaults/><a:extraClrSchemeLst/></a:theme>");

				CString strFile = _T("");
				strFile.Format(_T("\\ppt\\theme\\theme%d.xml"), nIndexTheme + 1);
				CFile oFile;
				oFile.CreateFile(m_strTempDirectory + strFile);
				oFile.WriteStringUTF8(strThemeXml);
				oFile.CloseFile();

				// теперь masterslide
				CRelsGenerator oRels(&m_oManager);
				int nCountLayouts = (int)pTheme->m_arLayouts.GetCount();
				oRels.StartMaster(nIndexTheme, nStartLayout, nCountLayouts);

				CStringWriter oWriter;
				CString str1 = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
<p:sldMaster xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\">\
<p:cSld>");
				oWriter.WriteString(str1);

				if (pTheme->m_bIsBackground)
				{
					WriteBackground(oWriter, oRels, pTheme->m_oBackground);
				}
				CString strElems = _T("<p:spTree><p:nvGrpSpPr><p:cNvPr id=\"1\" name=\"\"/><p:cNvGrpSpPr/><p:nvPr/></p:nvGrpSpPr><p:grpSpPr>\
<a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"0\" cy=\"0\"/><a:chOff x=\"0\" y=\"0\"/><a:chExt cx=\"0\" cy=\"0\"/></a:xfrm></p:grpSpPr>");
				oWriter.WriteString(strElems);

				size_t nElements = pTheme->m_arElements.GetCount();
				for (size_t nEl = 0; nEl < nElements; ++nEl)
					WriteElement(oWriter, oRels, pTheme->m_arElements[nEl]);

				oWriter.WriteString(_T("</p:spTree></p:cSld>"));

				CString strOverrideColorScheme = _T("<p:clrMap bg1=\"lt1\" tx1=\"dk1\" bg2=\"lt2\" tx2=\"dk2\" accent1=\"accent1\" accent2=\"accent2\" accent3=\"accent3\" accent4=\"accent4\" accent5=\"accent5\" accent6=\"accent6\" hlink=\"hlink\" folHlink=\"folHlink\"/>");
				oWriter.WriteString(strOverrideColorScheme);

				oWriter.WriteString(_T("<p:sldLayoutIdLst>"));

				size_t __nCountLayouts = 0;
				for (int nIndexLayout = 0; nIndexLayout < nCountLayouts; ++nIndexLayout)
				{
					CString strMasterLayout = _T("");
					strMasterLayout.Format(_T("<p:sldLayoutId id=\"%u\" r:id=\"rId%d\"/>"), 0x80000000 + nIndexTheme + 1 + nStartLayout + nIndexLayout, nIndexLayout + 1);
					oWriter.WriteString(strMasterLayout);

					WriteLayout(pTheme->m_arLayouts[nIndexLayout], nIndexLayout, nStartLayout, nIndexTheme);
				}

				oWriter.WriteString(_T("</p:sldLayoutIdLst>"));

				oWriter.WriteString(_T("<p:txStyles>"));

				oWriter.WriteString(_T("<p:titleStyle>"));
				CStylesWriter::ConvertStyles(pTheme->m_pStyles[1], pTheme->m_oInfo, oWriter, 9);
				oWriter.WriteString(_T("</p:titleStyle>"));

				oWriter.WriteString(_T("<p:bodyStyle>"));
				CStylesWriter::ConvertStyles(pTheme->m_pStyles[2], pTheme->m_oInfo, oWriter, 9);
				oWriter.WriteString(_T("</p:bodyStyle>"));

				oWriter.WriteString(_T("<p:otherStyle>"));
				CStylesWriter::ConvertStyles(pTheme->m_pStyles[3], pTheme->m_oInfo, oWriter, 9);
				oWriter.WriteString(_T("</p:otherStyle>"));
				
				oWriter.WriteString(_T("</p:txStyles></p:sldMaster>"));

				strFile = _T("");
				strFile.Format(_T("\\ppt\\slideMasters\\slideMaster%d.xml"), nIndexTheme + 1);
				oFile.CreateFile(m_strTempDirectory + strFile);
				CString strMaster = oWriter.GetData();
				oFile.WriteStringUTF8(strMaster);
				oFile.CloseFile();

				oRels.CloseRels();
				strFile = _T("");
				strFile.Format(_T("\\ppt\\slideMasters\\_rels\\slideMaster%d.xml.rels"), nIndexTheme + 1);
				oRels.SaveRels(m_strTempDirectory + strFile);				

				nStartLayout += nCountLayouts;
			}

			size_t nCountSlides = m_pDocument->m_arSlides.GetCount();
			for (size_t nIndexS = 0; nIndexS < nCountSlides; ++nIndexS)
			{
				CRelsGenerator::StartNotes((int)nIndexS, m_strTempDirectory, m_pDocument->m_arSlides[nIndexS].m_strComment);
				WriteSlide((int)nIndexS);
			}
		}

		void WriteBackground(CStringWriter& oWriter, CRelsGenerator& oRels, CBrush& oBackground)
		{
			if (oBackground.Type == c_BrushTypeTexture)
			{
				CString strRid = oRels.WriteImage(oBackground.TexturePath);

				CString strWrite = _T("<p:bg><p:bgPr><a:blipFill dpi=\"0\" rotWithShape=\"0\"><a:blip r:embed=\"") + strRid + 
					_T("\"/><a:srcRect/><a:stretch><a:fillRect/></a:stretch></a:blipFill><a:effectLst/></p:bgPr></p:bg>");

				oWriter.WriteString(strWrite);
				return;
			}
			if (oBackground.Color1.m_lSchemeIndex == -1)
			{
				CString str = _T("");
				str.Format(_T("<a:solidFill><a:srgbClr val=\"%06x\"/></a:solidFill>"), oBackground.Color1.GetLONG_RGB());
				str = _T("<p:bg><p:bgPr>") + str + _T("</p:bgPr></p:bg>");
				oWriter.WriteString(str);
			}
			else
			{
				CString str = _T("");
				str.Format(_T("<a:solidFill><a:schemeClr val=\"%s\"/></a:solidFill>"), CStylesWriter::GetColorInScheme(oBackground.Color1.m_lSchemeIndex));
				str = _T("<p:bg><p:bgPr>") + str + _T("</p:bgPr></p:bg>");
				oWriter.WriteString(str);
			}
		}

		void WriteElement(CStringWriter& oWriter, CRelsGenerator& oRels, IElement* pElement, CLayout* pLayout = NULL)
		{
			if (etShape != pElement->m_etType)
				return;

			CShapeElement* pShapeElem = dynamic_cast<CShapeElement*>(pElement);
			m_pShapeWriter->SetShape(pShapeElem);
			m_pShapeWriter->SetRelsGenerator(&oRels);

			if (NULL != pLayout)
			{
				if (-1 != pElement->m_lPlaceholderType)
				{
					size_t nCountElements = pLayout->m_arElements.GetCount();
					for (size_t nIndex = 0; nIndex < nCountElements; ++nIndex)
					{
						if ((pElement->m_lPlaceholderType == pLayout->m_arElements[nIndex]->m_lPlaceholderType) &&
							(pElement->m_lPlaceholderID == pLayout->m_arElements[nIndex]->m_lPlaceholderID))
						{
							IElement* pElLayout = pLayout->m_arElements[nIndex];
							bool bIsEqualTransform = ((pElement->m_dRotate == pElLayout->m_dRotate) 
								&& (pElement->m_bFlipH == pElLayout->m_bFlipH) && (pElement->m_bFlipV == pElLayout->m_bFlipV));

							if (bIsEqualTransform)
							{
								if (pElement->m_rcBounds.IsEqual(pElLayout->m_rcBounds, 0.5))
									m_pShapeWriter->SetIsWriteGeom(false);
							}

							break;
						}
					}
				}
			}

			oWriter.WriteString(m_pShapeWriter->ConvertShape());
		}

		void WriteLayout(CLayout& oLayout, int nIndexLayout, int nStartLayout, int nIndexTheme)
		{
			CStringWriter oWriter;
			CRelsGenerator oRels(&m_oManager);
			oRels.StartLayout(nIndexTheme);

			CString str1 = _T("");
			str1.Format(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><p:sldLayout xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\" \
						   type=\"%s\" showMasterSp=\"%s\" preserve=\"1\"><p:cSld>"), oLayout.m_strLayoutType, oLayout.m_bShowMasterShapes ? _T("1") : _T("0"));
			oWriter.WriteString(str1);

			if (oLayout.m_bIsBackground)
			{
				WriteBackground(oWriter, oRels, oLayout.m_oBackground);
			}

			CString strElems = _T("<p:spTree><p:nvGrpSpPr><p:cNvPr id=\"1\" name=\"\"/><p:cNvGrpSpPr/><p:nvPr/></p:nvGrpSpPr><p:grpSpPr>\
<a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"0\" cy=\"0\"/><a:chOff x=\"0\" y=\"0\"/><a:chExt cx=\"0\" cy=\"0\"/></a:xfrm></p:grpSpPr>");
			oWriter.WriteString(strElems);

			size_t nElements = oLayout.m_arElements.GetCount();
			for (size_t nEl = 0; nEl < nElements; ++nEl)
				WriteElement(oWriter, oRels,  oLayout.m_arElements[nEl]);

			oWriter.WriteString(_T("</p:spTree></p:cSld>"));

			oWriter.WriteString(_T("<p:clrMapOvr><a:masterClrMapping/></p:clrMapOvr>"));
			oWriter.WriteString(_T("</p:sldLayout>"));

			oRels.CloseRels();

			CString strXml = oWriter.GetData();
			CString strFile = _T("");
			strFile.Format(_T("\\ppt\\slideLayouts\\slideLayout%d.xml"), nIndexLayout + nStartLayout + 1);
			CFile oFile;
			oFile.CreateFile(m_strTempDirectory + strFile);
			oFile.WriteStringUTF8(strXml);
			oFile.CloseFile();

			strFile = _T("");
			strFile.Format(_T("\\ppt\\slideLayouts\\_rels\\slideLayout%d.xml.rels"), nIndexLayout + nStartLayout + 1);
			oRels.SaveRels(m_strTempDirectory + strFile);			
		}
		void WriteSlide(int nIndexSlide)
		{
			CStringWriter oWriter;
			CRelsGenerator oRels(&m_oManager);
			
			CSlide& oSlide = m_pDocument->m_arSlides[nIndexSlide];
			
			if (0 == oSlide.m_lThemeID)
				oRels.StartSlide(oSlide.m_lLayoutID, nIndexSlide);
			else
			{
				int nLayout = oSlide.m_lLayoutID;
				for (int i = 0; i < oSlide.m_lThemeID; ++i)
					nLayout += (int)m_pDocument->m_arThemes[i].m_arLayouts.GetCount();

				oRels.StartSlide(nLayout, nIndexSlide);
			}

			CString str1 = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><p:sld xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\"><p:cSld>");
			oWriter.WriteString(str1);

			if (oSlide.m_bIsBackground)
			{
				WriteBackground(oWriter, oRels, oSlide.m_oBackground);
			}

			CString strElems = _T("<p:spTree><p:nvGrpSpPr><p:cNvPr id=\"1\" name=\"\"/><p:cNvGrpSpPr/><p:nvPr/></p:nvGrpSpPr><p:grpSpPr>\
<a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"0\" cy=\"0\"/><a:chOff x=\"0\" y=\"0\"/><a:chExt cx=\"0\" cy=\"0\"/></a:xfrm></p:grpSpPr>");
			oWriter.WriteString(strElems);

			size_t nElements = oSlide.m_arElements.GetCount();
			for (size_t nEl = 0; nEl < nElements; ++nEl)
				WriteElement(oWriter, oRels,  oSlide.m_arElements[nEl], &m_pDocument->m_arThemes[oSlide.m_lThemeID].m_arLayouts[oSlide.m_lLayoutID]);

			oWriter.WriteString(_T("</p:spTree></p:cSld>"));

			oWriter.WriteString(_T("<p:clrMapOvr><a:masterClrMapping/></p:clrMapOvr>"));
			oWriter.WriteString(_T("<p:timing><p:tnLst><p:par><p:cTn id=\"1\" dur=\"indefinite\" restart=\"never\" nodeType=\"tmRoot\" /></p:par></p:tnLst></p:timing>"));
			oWriter.WriteString(_T("</p:sld>"));

			oRels.CloseRels();

			CString strXml = oWriter.GetData();
			CString strFile = _T("");
			strFile.Format(_T("\\ppt\\slides\\slide%d.xml"), nIndexSlide + 1);
			CFile oFile;
			oFile.CreateFile(m_strTempDirectory + strFile);
			oFile.WriteStringUTF8(strXml);
			oFile.CloseFile();

			strFile = _T("");
			strFile.Format(_T("\\ppt\\slides\\_rels\\slide%d.xml.rels"), nIndexSlide + 1);
			oRels.SaveRels(m_strTempDirectory + strFile);
		}

	protected:
		static int RemoveDirOrFile(CString sPath)
		{
			DWORD dwFileAttrib = ::GetFileAttributes( sPath );
			if(  dwFileAttrib != INVALID_FILE_ATTRIBUTES )
			{
				DWORD dwResult = 0;
				if( 0 != (FILE_ATTRIBUTE_DIRECTORY & dwFileAttrib) )
				{
					HANDLE Handle;
					WIN32_FIND_DATA FindData;
					DWORDLONG Result = 0;

					Handle = FindFirstFile( ( sPath + _T("\\*.*") ), &FindData );
					if ( Handle == INVALID_HANDLE_VALUE )
						return 0;
					do
					{
						BOOL bRes = TRUE;
						if( ( CString( FindData.cFileName ) != _T(".") ) && ( CString( FindData.cFileName ) != _T("..") ) )
							if( FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
								Result += RemoveDirOrFile( sPath + _T("\\") + FindData.cFileName ); //
							else
								bRes = DeleteFile( sPath + _T("\\") + FindData.cFileName );
						if( FALSE == bRes )
							dwResult += 1;
					}
					while( FindNextFile( Handle, &FindData ) != 0 );
					FindClose( Handle );
					BOOL bRes = RemoveDirectory( sPath );
					if( FALSE == bRes )
						dwResult += 1;
				}
				else
				{
					if( FALSE == DeleteFile( sPath ) )
						dwResult = 1;
				}

				return dwResult;
			}// 0 - все закончилось хорошо
			return 0;
		}

		void LoadResourceFile(HINSTANCE hInst, LPCTSTR sResName, LPCTSTR sResType, const CString& strDstFile)
		{
			HRSRC hrRes = FindResource(hInst, sResName, sResType);
			if (!hrRes)
				return;

			HGLOBAL hGlobal = LoadResource(hInst, hrRes);
			DWORD sz = SizeofResource(hInst, hrRes);
			void* ptrRes = LockResource(hGlobal);
			
			CFile oFile;
			oFile.CreateFile(strDstFile);
			oFile.WriteFile(ptrRes, sz);

			UnlockResource(hGlobal);
			FreeResource(hGlobal);
		}
	};
}