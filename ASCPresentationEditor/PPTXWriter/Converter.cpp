#pragma once

#include "../OfficeDrawing/Document.h"
#include "../../Common/DocxFormat/Source/SystemUtility/File.h"
#include "../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"

#include "../../ASCOfficePPTFile/PPTFormatLib/Reader/PPTDocumentInfo.h"
#include "ShapeWriter.h"
#include "StylesWriter.h"

#include "../../Common/DocxFormat/Source/SystemUtility/SystemUtility.h"
#include "../../ASCOfficePPTXFile/Editor/DefaultNotesMaster.h"
#include "../../ASCOfficePPTXFile/Editor/DefaultNotesTheme.h"

#include "Converter.h"	

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
<dc:creator>OnlyOffice</dc:creator>\
<cp:lastModifiedBy>OnlyOffice</cp:lastModifiedBy>\
<cp:revision>1</cp:revision>\
</cp:coreProperties>");
	}
};


NSPresentationEditor::CPPTXWriter::CPPTXWriter()
{
    m_strTempDirectory	= FileSystem::Directory::GetTempPath() + FILE_SEPARATOR_STR + _T("TempPPTX");
    m_strDstFileName	= FileSystem::Directory::GetTempPath() + FILE_SEPARATOR_STR + _T("Test.pptx");

	m_pDocument			= NULL;
	m_pUserInfo			= NULL;

	m_pShapeWriter = new CShapeWriter();
}
	
NSPresentationEditor::CPPTXWriter::~CPPTXWriter()
{
	RELEASEOBJECT(m_pShapeWriter);
}

void NSPresentationEditor::CPPTXWriter::CreateFile(CPPTUserInfo* pUserInfo	)
{
	m_pUserInfo = pUserInfo;

	m_pDocument = dynamic_cast<CDocument*>(pUserInfo);

	//m_pDocument->Calculate();

	m_pDocument->m_oInfo.m_lUnitsHor			= 36000 * m_pDocument->m_oInfo.m_lMillimetresHor;
	m_pDocument->m_oInfo.m_lUnitsVer			= 36000 * m_pDocument->m_oInfo.m_lMillimetresVer;

	m_oManager.Clear();
	m_oManager.SetDstMedia(m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("media") + FILE_SEPARATOR_STR);

	m_pShapeWriter->InitNextId();

    FileSystem::Directory::CreateDirectory(m_strTempDirectory);
	CFile oFile;
	CString strMemory = _T("");

	// _rels 
    FileSystem::Directory::CreateDirectory(m_strTempDirectory + FILE_SEPARATOR_STR + _T("_rels"));
	
	oFile.CreateFile(m_strTempDirectory + FILE_SEPARATOR_STR + _T("_rels")  + FILE_SEPARATOR_STR + _T(".rels"));
	strMemory = NSPPTXWriterConst::g_string_rels_presentation;
	oFile.WriteStringUTF8(strMemory);
	oFile.CloseFile();

	// docProps 
    FileSystem::Directory::CreateDirectory(m_strTempDirectory + FILE_SEPARATOR_STR + _T("docProps"));
	
	// core
	oFile.CreateFile(m_strTempDirectory + FILE_SEPARATOR_STR + _T("docProps") + FILE_SEPARATOR_STR + _T("core.xml"));
	strMemory = NSPPTXWriterConst::g_string_core;
	oFile.WriteStringUTF8(strMemory);
	oFile.CloseFile();

	// app
	oFile.CreateFile(m_strTempDirectory + FILE_SEPARATOR_STR + _T("docProps") + FILE_SEPARATOR_STR + _T("app.xml"));
	WriteApp(oFile);
	oFile.CloseFile();

	// content types
	WriteContentTypes();

	// ppt 
    FileSystem::Directory::CreateDirectory(m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt"));
	WritePresInfo();

	WriteAll();
}

void NSPresentationEditor::CPPTXWriter::CreateFile(CDocument* pDocument)
{
	m_pDocument = pDocument;
	m_oManager.Clear();
	m_oManager.SetDstMedia(m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("media") + FILE_SEPARATOR_STR);

	m_pShapeWriter->InitNextId();

    FileSystem::Directory::CreateDirectory(m_strTempDirectory);
	CFile oFile;
	CString strMemory = _T("");

	// _rels 
    FileSystem::Directory::CreateDirectory(m_strTempDirectory + FILE_SEPARATOR_STR + _T("_rels"));
	
	oFile.CreateFile(m_strTempDirectory + FILE_SEPARATOR_STR + _T("_rels")  + FILE_SEPARATOR_STR + _T(".rels"));
	strMemory = NSPPTXWriterConst::g_string_rels_presentation;
	oFile.WriteStringUTF8(strMemory);
	oFile.CloseFile();

	// docProps 
    FileSystem::Directory::CreateDirectory(m_strTempDirectory + FILE_SEPARATOR_STR + _T("docProps"));
	
	// core
	oFile.CreateFile(m_strTempDirectory + FILE_SEPARATOR_STR + _T("docProps") + FILE_SEPARATOR_STR + _T("core.xml"));
	strMemory = NSPPTXWriterConst::g_string_core;
	oFile.WriteStringUTF8(strMemory);
	oFile.CloseFile();

	// app
	oFile.CreateFile(m_strTempDirectory + FILE_SEPARATOR_STR + _T("docProps") + FILE_SEPARATOR_STR + _T("app.xml"));
	WriteApp(oFile);
	oFile.CloseFile();

	// content types
	WriteContentTypes();

	// ppt 
    FileSystem::Directory::CreateDirectory(m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt"));
	WritePresInfo();

	WriteAll();
}

void NSPresentationEditor::CPPTXWriter::CloseFile()
{
	m_oManager.Clear();
}


void NSPresentationEditor::CPPTXWriter::WriteContentTypes()
{
	CString strContentTypes = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
<Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\">\
<Default Extension=\"bmp\" ContentType=\"image/bmp\" />\
<Default Extension=\"png\" ContentType=\"image/png\" />\
<Default Extension=\"jpeg\" ContentType=\"image/jpeg\" />\
<Default Extension=\"wmf\" ContentType=\"image/x-wmf\" />\
<Default Extension=\"rels\" ContentType=\"application/vnd.openxmlformats-package.relationships+xml\" />\
<Default Extension=\"xml\" ContentType=\"application/xml\" />\
<Default Extension=\"gif\" ContentType=\"image/gif\"/>\
<Default Extension=\"emf\" ContentType=\"image/x-emf\"/>\
<Default Extension=\"bin\" ContentType=\"application/vnd.openxmlformats-officedocument.oleObject\" />\
<Default Extension=\"jpg\" ContentType=\"application/octet-stream\"/>");

	strContentTypes += _T("<Override PartName=\"/ppt/presentation.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml\" />\
<Override PartName=\"/ppt/presProps.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.presProps+xml\" />\
<Override PartName=\"/ppt/viewProps.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.viewProps+xml\" />\
<Override PartName=\"/ppt/tableStyles.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.tableStyles+xml\"/>\
<Override PartName=\"/docProps/core.xml\" ContentType=\"application/vnd.openxmlformats-package.core-properties+xml\" />\
<Override PartName=\"/docProps/app.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.extended-properties+xml\" />");

	int nThemes = (int)m_pDocument->m_arThemes.size();
	int nIndexLayout = 0;
	for (int nT = 0; nT < nThemes; ++nT)
	{
		CString strTheme = _T("");
		strTheme.Format(_T("<Override PartName=\"/ppt/theme/theme%d.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.theme+xml\" />\
<Override PartName=\"/ppt/slideMasters/slideMaster%d.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.slideMaster+xml\" />"), 
				nT + 1, nT + 1);

		strContentTypes += strTheme;

		int nCountL = (int)m_pDocument->m_arThemes[nT].m_arLayouts.size();
		for (int nL = 0; nL < nCountL; ++nL, ++nIndexLayout)
		{
			CString strL = _T("");
			strL.Format(_T("<Override PartName=\"/ppt/slideLayouts/slideLayout%d.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.slideLayout+xml\" />"), nIndexLayout + 1);
			strContentTypes += strL;
		}
	}
	CString strNotesTheme = _T("");
	strNotesTheme.Format(_T("<Override PartName=\"/ppt/theme/theme%d.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.theme+xml\" />"),
			nThemes + 1);

	strContentTypes += strNotesTheme;
	strContentTypes += _T("<Override PartName=\"/ppt/notesMasters/notesMaster1.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.notesMaster+xml\"/>");

	int nCountS = (int)m_pDocument->m_arSlides.size();
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
	oFile.CreateFile(m_strTempDirectory + FILE_SEPARATOR_STR + _T("[Content_Types].xml"));
	oFile.WriteStringUTF8(strContentTypes);
	oFile.CloseFile();
}


void NSPresentationEditor::CPPTXWriter::WriteApp(CFile& oFile)
{
	CString str1 = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
<Properties xmlns=\"http://schemas.openxmlformats.org/officeDocument/2006/extended-properties\" xmlns:vt=\"http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes\">\
<TotalTime>0</TotalTime>\
<Words>0</Words>\
<Application>OnlyOffice</Application>\
<PresentationFormat>On-screen Show (4:3)</PresentationFormat>\
<Paragraphs>0</Paragraphs>");

	oFile.WriteStringUTF8(str1);

	CString str2 = _T("");
	str2.Format(_T("<Slides>%d</Slides>"), (int)m_pDocument->m_arSlides.size());
	oFile.WriteStringUTF8(str2);

	CString str3 = _T("<Notes>0</Notes>\
<HiddenSlides>0</HiddenSlides>\
<MMClips>2</MMClips>\
<ScaleCrop>false</ScaleCrop>\
<HeadingPairs>\
<vt:vector size=\"4\" baseType=\"variant\">");
	oFile.WriteStringUTF8(str3);

	int nCountThemes = (int)m_pDocument->m_arThemes.size();
	int nCountSlides = (int)m_pDocument->m_arSlides.size();

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

void NSPresentationEditor::CPPTXWriter::WritePresInfo()
{
	CFile oFile;
	
	// tableStyles.xml
	oFile.CreateFile(m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt")  + FILE_SEPARATOR_STR + _T("tableStyles.xml"));
	
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
	
	oFile.CreateFile(m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("presProps.xml"));
	oFile.WriteStringUTF8(str);
	oFile.CloseFile();

	// viewProps.xml
	str = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
<p:viewPr xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\">\
<p:normalViewPr><p:restoredLeft sz=\"15620\"/><p:restoredTop sz=\"94660\"/></p:normalViewPr><p:slideViewPr><p:cSldViewPr><p:cViewPr varScale=\"1\">\
<p:scale><a:sx n=\"104\" d=\"100\"/><a:sy n=\"104\" d=\"100\"/></p:scale><p:origin x=\"-1236\" y=\"-90\"/></p:cViewPr><p:guideLst>\
<p:guide orient=\"horz\" pos=\"2160\"/><p:guide pos=\"2880\"/></p:guideLst></p:cSldViewPr></p:slideViewPr><p:notesTextViewPr><p:cViewPr><p:scale>\
<a:sx n=\"1\" d=\"1\"/><a:sy n=\"1\" d=\"1\"/></p:scale><p:origin x=\"0\" y=\"0\"/></p:cViewPr></p:notesTextViewPr><p:gridSpacing cx=\"72008\" cy=\"72008\"/></p:viewPr>");
	
	oFile.CreateFile(m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("viewProps.xml"));
	oFile.WriteStringUTF8(str);
	oFile.CloseFile();

	// presentation.xml + _rels/presentation.xml.rels
	CString strPresRels;

	CString strPresMasters	= _T("");
	CString strPresSlides	= _T("");

	size_t nCountLayouts = 0;
	size_t nCountThemes = m_pDocument->m_arThemes.size();
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
		nCountLayouts += m_pDocument->m_arThemes[nIndexTheme].m_arLayouts.size();
		nCountLayouts += 1;

		strPresMasters += strRels;
	}

	int nCurrentRels = (int)(2 * nCountThemes + 1);
	size_t nCountSlides = m_pDocument->m_arSlides.size();
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

	CString strPptRels = m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("_rels");
    FileSystem::Directory::CreateDirectory(strPptRels);
	oFile.CreateFile(strPptRels + FILE_SEPARATOR_STR + _T("presentation.xml.rels"));
	oFile.WriteStringUTF8(strPresRels);
	oFile.CloseFile();

	CString strSizePres = _T("");
	strSizePres.Format(_T("<p:sldSz cx=\"%d\" cy=\"%d\" type=\"screen4x3\" /><p:notesSz cx=\"%d\" cy=\"%d\" />"), 
		m_pDocument->m_oInfo.m_lUnitsHor, m_pDocument->m_oInfo.m_lUnitsVer, m_pDocument->m_oInfo.m_lUnitsVer, m_pDocument->m_oInfo.m_lUnitsHor);

	CString strDefaultTextStyle = _T("<p:defaultTextStyle>");
	if (m_pDocument->m_arThemes.size()>0)
	{
		strDefaultTextStyle += CStylesWriter::ConvertStyles(m_pDocument->m_arThemes[0].m_pStyles[0], m_pDocument->m_oInfo, 9);
	}
	strDefaultTextStyle += _T("</p:defaultTextStyle>");

	CString strPres = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
<p:presentation xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\" saveSubsetFonts=\"1\">\
<p:sldMasterIdLst>") + strPresMasters + _T("</p:sldMasterIdLst>") + strNotesIDs + _T("<p:sldIdLst>") + strPresSlides + _T("</p:sldIdLst>") + strSizePres + strDefaultTextStyle + _T("</p:presentation>");

    oFile.CreateFile(m_strTempDirectory+ FILE_SEPARATOR_STR  + _T("ppt") + FILE_SEPARATOR_STR + _T("presentation.xml"));
	oFile.WriteStringUTF8(strPres);
	oFile.CloseFile();		
}

void NSPresentationEditor::CPPTXWriter::WriteAll()
{
	CString strPptDirectory = m_strTempDirectory + FILE_SEPARATOR_STR  + _T("ppt") + FILE_SEPARATOR_STR ;
	
    FileSystem::Directory::CreateDirectory(strPptDirectory + _T("media"));
    FileSystem::Directory::CreateDirectory(strPptDirectory + _T("theme"));
    FileSystem::Directory::CreateDirectory(strPptDirectory + _T("slideMasters"));
    FileSystem::Directory::CreateDirectory(strPptDirectory + _T("slideMasters")		+ FILE_SEPARATOR_STR + _T("_rels"));
    FileSystem::Directory::CreateDirectory(strPptDirectory + _T("slideLayouts"));
    FileSystem::Directory::CreateDirectory(strPptDirectory + _T("slideLayouts")		+ FILE_SEPARATOR_STR + _T("_rels"));
    FileSystem::Directory::CreateDirectory(strPptDirectory + _T("slides"));
    FileSystem::Directory::CreateDirectory(strPptDirectory + _T("slides")			+ FILE_SEPARATOR_STR + _T("_rels"));
    FileSystem::Directory::CreateDirectory(strPptDirectory + _T("notesMasters"));
    FileSystem::Directory::CreateDirectory(strPptDirectory + _T("notesMasters")		+ FILE_SEPARATOR_STR + _T("_rels"));
    FileSystem::Directory::CreateDirectory(strPptDirectory + _T("notesSlides"));
    FileSystem::Directory::CreateDirectory(strPptDirectory + _T("notesSlides")		+ FILE_SEPARATOR_STR + _T("_rels"));
	
	CString strNotesTheme		= _T("");

	strNotesTheme.Format(_T("theme%d.xml"), (int)m_pDocument->m_arThemes.size() + 1);
	strNotesTheme = strPptDirectory + _T("theme") + FILE_SEPARATOR_STR + strNotesTheme;

	Writers::DefaultNotesThemeWriter writerNotesTheme;
	writerNotesTheme.Write( strNotesTheme);

	CString strNotesMaster	= strPptDirectory + _T("notesMasters") + FILE_SEPARATOR_STR + _T("notesMaster1.xml");
	Writers::DefaultNotesMasterWriter writerNotesMaster;
	writerNotesMaster.Write(strNotesMaster);

	CString strNotesMasterRels = strPptDirectory + _T("notesMasters") + FILE_SEPARATOR_STR +_T("_rels");
    FileSystem::Directory::CreateDirectory(strNotesMasterRels);

	CString strThemeNotesNum = _T("");
	strThemeNotesNum.Format(_T("%d"), (int)m_pDocument->m_arThemes.size() + 1);
	
	CString strVal = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\
<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"../theme/theme") + strThemeNotesNum + _T(".xml\"/></Relationships>");
	
	CString strNotesMasterRelsFile = strNotesMasterRels+ FILE_SEPARATOR_STR + _T("notesMaster1.xml.rels");
	CFile oFileRels;
	oFileRels.CreateFile(strNotesMasterRelsFile);
	oFileRels.WriteStringUTF8(strVal);
	oFileRels.CloseFile();
	// -----------------------------------------------------

	WriteThemes();
	
	WriteSlides();
}

void NSPresentationEditor::CPPTXWriter::WriteThemes()
{
	CString strPptDirectory = m_strTempDirectory + FILE_SEPARATOR_STR  + _T("ppt") + FILE_SEPARATOR_STR ;

	int nCount = (int)m_pDocument->m_arThemes.size();
	int nStartLayout = 0;
	for (int nIndexTheme = 0; nIndexTheme < nCount; ++nIndexTheme)
	{
		CTheme* pTheme = &m_pDocument->m_arThemes[nIndexTheme];
		
		CString strThemeFile;
		strThemeFile.Format(_T("theme%d.xml"), nIndexTheme + 1);
		strThemeFile = strPptDirectory + _T("theme") + FILE_SEPARATOR_STR + strThemeFile;
		
		CFile oFile;
		oFile.CreateFile(strThemeFile);

		NSPresentationEditor::CStringWriter oStringWriter;

		oStringWriter.WriteString(std::wstring(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?><a:theme xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" name=\""));
		oStringWriter.WriteStringXML(pTheme->m_sThemeName);
		oStringWriter.WriteString(std::wstring(L"\"><a:themeElements>"));
	
		WriteColorScheme(oStringWriter, L"Default", pTheme->m_arColorScheme);

		oStringWriter.WriteString(std::wstring(L"<a:fontScheme name=\"default\"><a:majorFont><a:latin typeface=\""));
		oStringWriter.WriteStringXML(pTheme->m_arFonts[0].Name);
		oStringWriter.WriteString(std::wstring(L"\"/><a:ea typeface=\"\"/><a:cs typeface=\"\"/></a:majorFont>"));

		oStringWriter.WriteString(std::wstring(L"<a:minorFont><a:latin typeface=\""));
		
		if (pTheme->m_arFonts.size() >1 )	oStringWriter.WriteString(pTheme->m_arFonts[1].Name);
		else oStringWriter.WriteStringXML(pTheme->m_arFonts[0].Name);

		oStringWriter.WriteString(std::wstring(L"\"/><a:ea typeface=\"\"/><a:cs typeface=\"\"/></a:minorFont>"));
		oStringWriter.WriteString(std::wstring(L"</a:fontScheme>"));
 
		oStringWriter.WriteString(std::wstring(L"<a:fmtScheme name=\"Default\">\
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
<a:fillToRect l=\"50000\" t=\"50000\" r=\"50000\" b=\"50000\"/></a:path></a:gradFill></a:bgFillStyleLst></a:fmtScheme>"));

		oStringWriter.WriteString(std::wstring(L"</a:themeElements><a:objectDefaults/>"));
		oStringWriter.WriteString(std::wstring(L"<a:extraClrSchemeLst>"));
		
		for (int i = 0 ; i < pTheme->m_arExtraColorScheme.size(); i++)
		{
			CString str;
			str.Format(_T(" %d"), i + 1);
			WriteColorScheme(oStringWriter, pTheme->m_sThemeName + string2std_string(str), pTheme->m_arExtraColorScheme[i], true); //extra
		}
		
		oStringWriter.WriteString(std::wstring(L"</a:extraClrSchemeLst>"));
		oStringWriter.WriteString(std::wstring(L"</a:theme>"));

		oFile.WriteStringUTF8(oStringWriter.GetData());
		oFile.CloseFile();

		// теперь masterslide
		CRelsGenerator oRels(&m_oManager);
		int nCountLayouts = (int)pTheme->m_arLayouts.size();
		
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

		size_t nElements = pTheme->m_arElements.size();
		for (size_t nEl = 0; nEl < nElements; ++nEl)
		{
			if (pTheme->m_arElements[nEl]->m_lPlaceholderType == 0)
				pTheme->m_arElements[nEl]->m_lPlaceholderType =100; //body тип прописывать !!
			
			WriteElement(oWriter, oRels, pTheme->m_arElements[nEl]);
		}

		oWriter.WriteString(std::wstring(L"</p:spTree></p:cSld>"));

		CString strOverrideColorScheme = _T("<p:clrMap bg1=\"lt1\" tx1=\"dk1\" bg2=\"lt2\" tx2=\"dk2\" accent1=\"accent1\" accent2=\"accent2\" accent3=\"accent3\" accent4=\"accent4\" accent5=\"accent5\" accent6=\"accent6\" hlink=\"hlink\" folHlink=\"folHlink\"/>");
		oWriter.WriteString(strOverrideColorScheme);

		oWriter.WriteString(std::wstring(L"<p:sldLayoutIdLst>"));

		size_t __nCountLayouts = 0;
		for (int nIndexLayout = 0; nIndexLayout < nCountLayouts; ++nIndexLayout)
		{
			CString strMasterLayout = _T("");
			strMasterLayout.Format(_T("<p:sldLayoutId id=\"%u\" r:id=\"rId%d\"/>"), 0x80000000 + nIndexTheme + 1 + nStartLayout + nIndexLayout, nIndexLayout + 1);
			oWriter.WriteString(strMasterLayout);

			WriteLayout(pTheme->m_arLayouts[nIndexLayout], nIndexLayout, nStartLayout, nIndexTheme);
		}

		oWriter.WriteString(std::wstring(L"</p:sldLayoutIdLst>"));

		oWriter.WriteString(std::wstring(L"<p:txStyles>"));

		oWriter.WriteString(std::wstring(L"<p:titleStyle>"));
		CStylesWriter::ConvertStyles(pTheme->m_pStyles[1], pTheme->m_oInfo, oWriter, 9);
		oWriter.WriteString(std::wstring(L"</p:titleStyle>"));

		oWriter.WriteString(std::wstring(L"<p:bodyStyle>"));
		CStylesWriter::ConvertStyles(pTheme->m_pStyles[2], pTheme->m_oInfo, oWriter, 9);
		oWriter.WriteString(std::wstring(L"</p:bodyStyle>"));

		oWriter.WriteString(std::wstring(L"<p:otherStyle>"));
		CStylesWriter::ConvertStyles(pTheme->m_pStyles[3], pTheme->m_oInfo, oWriter, 9);
		oWriter.WriteString(std::wstring(L"</p:otherStyle>"));
		
		oWriter.WriteString(std::wstring(L"</p:txStyles></p:sldMaster>"));

		CString strSlideMasterFile;
		strSlideMasterFile.Format(_T("slideMaster%d.xml"), nIndexTheme + 1);
		strSlideMasterFile = strPptDirectory + _T("slideMasters") + FILE_SEPARATOR_STR + strSlideMasterFile;

		oFile.CreateFile(strSlideMasterFile);
		CString strMaster = oWriter.GetData();
		oFile.WriteStringUTF8(strMaster);
		oFile.CloseFile();

		oRels.CloseRels();

		CString strSlideMasterRelsFile;
		strSlideMasterRelsFile.Format(_T("slideMaster%d.xml.rels"), nIndexTheme + 1);
		strSlideMasterRelsFile = strPptDirectory + _T("slideMasters") + FILE_SEPARATOR_STR + _T("_rels") + FILE_SEPARATOR_STR + strSlideMasterRelsFile;

		oRels.SaveRels(strSlideMasterRelsFile);				

		nStartLayout += nCountLayouts;
	}
}

void NSPresentationEditor::CPPTXWriter::WriteColorScheme(CStringWriter& oStringWriter, const std::wstring & name, const std::vector<CColor> & colors, bool extra)
{
	if (colors.size() < 1)
	{
		oStringWriter.WriteString(std::wstring(L"<a:clrScheme name=\"Default\"><a:dk1><a:sysClr val=\"windowText\" lastClr=\"000000\"/>\
		</a:dk1><a:lt1><a:sysClr val=\"window\" lastClr=\"FFFFFF\"/></a:lt1><a:dk2><a:srgbClr val=\"1F497D\"/></a:dk2><a:lt2>\
		<a:srgbClr val=\"EEECE1\"/></a:lt2><a:accent1><a:srgbClr val=\"4F81BD\"/></a:accent1><a:accent2><a:srgbClr val=\"C0504D\"/>\
		</a:accent2><a:accent3><a:srgbClr val=\"9BBB59\"/></a:accent3><a:accent4><a:srgbClr val=\"8064A2\"/></a:accent4><a:accent5>\
		<a:srgbClr val=\"4BACC6\"/></a:accent5><a:accent6><a:srgbClr val=\"F79646\"/></a:accent6><a:hlink><a:srgbClr val=\"0000FF\"/>\
		</a:hlink><a:folHlink><a:srgbClr val=\"800080\"/></a:folHlink></a:clrScheme>"));
		return;
	}

	if (extra)
		oStringWriter.WriteString(std::wstring(L"<a:extraClrScheme>"));

	oStringWriter.WriteString(std::wstring(L"<a:clrScheme name=\""));
	
	oStringWriter.WriteStringXML(name);
	oStringWriter.WriteString(std::wstring(L"\">"));
	CString strVal;
	
	strVal.Format(_T("<a:dk1><a:srgbClr val=\"%06X\"/></a:dk1>"), colors[14].GetLONG_RGB());
	oStringWriter.WriteString(strVal);
	
	strVal.Format(_T("<a:lt1><a:srgbClr val=\"%06X\"/></a:lt1>"), colors[13].GetLONG_RGB());
	oStringWriter.WriteString(strVal);
	
	strVal.Format(_T("<a:dk2><a:srgbClr val=\"%06X\"/></a:dk2>"), colors[16].GetLONG_RGB());
	oStringWriter.WriteString(strVal);
	
	strVal.Format(_T("<a:lt2><a:srgbClr val=\"%06X\"/></a:lt2>"), colors[15].GetLONG_RGB());
	oStringWriter.WriteString(strVal);
	
	strVal.Format(_T("<a:accent1><a:srgbClr val=\"%06X\"/></a:accent1>"), colors[5].GetLONG_RGB());
	oStringWriter.WriteString(strVal);
	
	strVal.Format(_T("<a:accent2><a:srgbClr val=\"%06X\"/></a:accent2>"), colors[6].GetLONG_RGB());
	oStringWriter.WriteString(strVal);
	
	strVal.Format(_T("<a:accent3><a:srgbClr val=\"%06X\"/></a:accent3>"), colors[7].GetLONG_RGB());
	oStringWriter.WriteString(strVal);
	
	strVal.Format(_T("<a:accent4><a:srgbClr val=\"%06X\"/></a:accent4>"), colors[8].GetLONG_RGB());
	oStringWriter.WriteString(strVal);
	
	strVal.Format(_T("<a:accent5><a:srgbClr val=\"%06X\"/></a:accent5>"), colors[9].GetLONG_RGB());
	oStringWriter.WriteString(strVal);
	
	strVal.Format(_T("<a:accent6><a:srgbClr val=\"%06X\"/></a:accent6>"), colors[10].GetLONG_RGB());
	oStringWriter.WriteString(strVal);
	
	strVal.Format(_T("<a:hlink><a:srgbClr val=\"%06X\"/></a:hlink>"), colors[11].GetLONG_RGB());
	oStringWriter.WriteString(strVal);
	
	strVal.Format(_T("<a:folHlink><a:srgbClr val=\"%06X\"/></a:folHlink>"), colors[12].GetLONG_RGB());
	oStringWriter.WriteString(strVal);

	oStringWriter.WriteString(std::wstring(L"</a:clrScheme>"));
	if (extra)
	{
		oStringWriter.WriteString(std::wstring(L"<a:clrMap bg1=\"lt1\" tx1=\"dk1\" bg2=\"lt2\" tx2=\"dk2\" accent1=\"accent1\" \
accent2=\"accent2\" accent3=\"accent3\" accent4=\"accent4\" accent5=\"accent5\" accent6=\"accent6\" hlink=\"hlink\" folHlink=\"folHlink\"/>"));
		oStringWriter.WriteString(std::wstring(L"</a:extraClrScheme>"));
	}
}

void NSPresentationEditor::CPPTXWriter::WriteBackground(CStringWriter& oWriter, CRelsGenerator& oRels, CBrush& oBackground)
{
	oWriter.WriteString(std::wstring(L"<p:bg><p:bgPr>"));

	m_pShapeWriter->SetRelsGenerator(&oRels);
	{
		oWriter.WriteString(m_pShapeWriter->ConvertBrush(oBackground));
	}
	oWriter.WriteString(std::wstring(L"</p:bgPr></p:bg>"));
}

void NSPresentationEditor::CPPTXWriter::WriteElement(CStringWriter& oWriter, CRelsGenerator& oRels, IElement* pElement, CLayout* pLayout)
{
	CImageElement* pImageElem = dynamic_cast<CImageElement*>(pElement);
	if (pImageElem)
	{
		pImageElem->m_oMetric = m_pDocument->m_oInfo;
		pImageElem->NormalizeCoordsByMetric();

		m_pShapeWriter->SetShape(pImageElem);
	}

	CShapeElement* pShapeElem = dynamic_cast<CShapeElement*>(pElement);
	if (pShapeElem)
	{
		pShapeElem->m_oMetric = m_pDocument->m_oInfo;
		pShapeElem->NormalizeCoordsByMetric();

		m_pShapeWriter->SetShape(pShapeElem);
	}

	if (pImageElem || pShapeElem)
	{
		m_pShapeWriter->SetRelsGenerator(&oRels);

		if (NULL != pLayout)
		{
			if (-1 != pElement->m_lPlaceholderType)
			{
				size_t nCountElements = pLayout->m_arElements.size();
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
								pElement->m_bBoundsEnabled = false;
						}

						break;
					}
				}
			}
		}
		oWriter.WriteString(m_pShapeWriter->ConvertShape());
	}
}

void NSPresentationEditor::CPPTXWriter::WriteLayout(CLayout& oLayout, int nIndexLayout, int nStartLayout, int nIndexTheme)
{
	CStringWriter oWriter;
	CRelsGenerator oRels(&m_oManager);
	oRels.StartLayout(nIndexTheme);

	oWriter.WriteString(std::wstring(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));

    oWriter.WriteString(std::wstring(L"<p:sldLayout xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\""));
    oWriter.WriteString(std::wstring(L" type=\"") + oLayout.m_strLayoutType + _T("\""));
    oWriter.WriteString(std::wstring(L" showMasterSp=\"") + (oLayout.m_bShowMasterShapes ? _T("1") : _T("0")));
    oWriter.WriteString(std::wstring(L"\" preserve=\"1\"><p:cSld>"));

	if (oLayout.m_bIsBackground)
	{
		WriteBackground(oWriter, oRels, oLayout.m_oBackground);
	}

	CString strElems = _T("<p:spTree><p:nvGrpSpPr><p:cNvPr id=\"1\" name=\"\"/><p:cNvGrpSpPr/><p:nvPr/></p:nvGrpSpPr><p:grpSpPr>\
<a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"0\" cy=\"0\"/><a:chOff x=\"0\" y=\"0\"/><a:chExt cx=\"0\" cy=\"0\"/></a:xfrm></p:grpSpPr>");
	oWriter.WriteString(strElems);

	size_t nElements = oLayout.m_arElements.size();
	for (size_t nEl = 0; nEl < nElements; ++nEl)
		WriteElement(oWriter, oRels,  oLayout.m_arElements[nEl]);

	oWriter.WriteString(std::wstring(L"</p:spTree></p:cSld>"));

	oWriter.WriteString(std::wstring(L"<p:clrMapOvr><a:masterClrMapping/></p:clrMapOvr>"));
	oWriter.WriteString(std::wstring(L"</p:sldLayout>"));

	oRels.CloseRels();

	CString strXml = oWriter.GetData();
	CString strFile = _T("");
	strFile.Format(_T("slideLayout%d.xml"), nIndexLayout + nStartLayout + 1);
	
	CFile oFile;
	CString strFileLayoutPath= m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("slideLayouts") + FILE_SEPARATOR_STR;
	oFile.CreateFile(strFileLayoutPath  + strFile);
	oFile.WriteStringUTF8(strXml);
	oFile.CloseFile();

	strFile = _T("");
	strFile.Format(_T("slideLayout%d.xml.rels"), nIndexLayout + nStartLayout + 1);
	oRels.SaveRels(strFileLayoutPath + _T("_rels") + FILE_SEPARATOR_STR + strFile);			
}
void NSPresentationEditor::CPPTXWriter::WriteSlide(int nIndexSlide)
{
	CStringWriter oWriter;
	CRelsGenerator oRels(&m_oManager);
	
	CSlide* pSlide = m_pDocument->m_arSlides[nIndexSlide];
	
	if (0 == pSlide->m_lThemeID)
		oRels.StartSlide(pSlide->m_lLayoutID, nIndexSlide);
	else
	{
		int nLayout = pSlide->m_lLayoutID;
		for (int i = 0; i < pSlide->m_lThemeID; ++i)
			nLayout += (int)m_pDocument->m_arThemes[i].m_arLayouts.size();

		oRels.StartSlide(nLayout, nIndexSlide);
	}

	CString str1 = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><p:sld xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\"><p:cSld>");
	oWriter.WriteString(str1);

	if (pSlide->m_bIsBackground)
	{
		WriteBackground(oWriter, oRels, pSlide->m_oBackground);
	}

	CString strElems = _T("<p:spTree><p:nvGrpSpPr><p:cNvPr id=\"1\" name=\"\"/><p:cNvGrpSpPr/><p:nvPr/></p:nvGrpSpPr><p:grpSpPr>\
<a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"0\" cy=\"0\"/><a:chOff x=\"0\" y=\"0\"/><a:chExt cx=\"0\" cy=\"0\"/></a:xfrm></p:grpSpPr>");
	oWriter.WriteString(strElems);

	size_t nElements = pSlide->m_arElements.size();
	for (size_t nEl = 0; nEl < nElements; ++nEl)
	{
		WriteElement(oWriter, oRels,  pSlide->m_arElements[nEl], &m_pDocument->m_arThemes[pSlide->m_lThemeID].m_arLayouts[pSlide->m_lLayoutID]);
	}
	oWriter.WriteString(std::wstring(L"</p:spTree></p:cSld>"));

	oWriter.WriteString(std::wstring(L"<p:clrMapOvr><a:masterClrMapping/></p:clrMapOvr>"));
	oWriter.WriteString(std::wstring(L"<p:timing><p:tnLst><p:par><p:cTn id=\"1\" dur=\"indefinite\" restart=\"never\" nodeType=\"tmRoot\" /></p:par></p:tnLst></p:timing>"));
	oWriter.WriteString(std::wstring(L"</p:sld>"));

	oRels.CloseRels();

	CString strXml = oWriter.GetData();
	CString strFile = _T("");
	strFile.Format(_T("slide%d.xml"), nIndexSlide + 1);
	CString strFileSlidePath= m_strTempDirectory + FILE_SEPARATOR_STR + _T("ppt") + FILE_SEPARATOR_STR + _T("slides")  + FILE_SEPARATOR_STR;

	CFile oFile;
	oFile.CreateFile(strFileSlidePath + strFile);
	oFile.WriteStringUTF8(strXml);
	oFile.CloseFile();

	strFile = _T("");
	strFile.Format(_T("slide%d.xml.rels"), nIndexSlide + 1);
	oRels.SaveRels(strFileSlidePath + _T("_rels") + FILE_SEPARATOR_STR + strFile);
}


void NSPresentationEditor::CPPTXWriter::WriteSlides()
{
	size_t nCountSlides = m_pDocument->m_arSlides.size();
	for (size_t nIndexS = 0; nIndexS < nCountSlides; ++nIndexS)
	{
		CRelsGenerator::StartNotes((int)nIndexS, m_strTempDirectory, m_pDocument->m_arSlides[nIndexS]->m_strComment);
		WriteSlide((int)nIndexS);
	}
}