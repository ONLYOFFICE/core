#pragma once

#include "../../Common/DocxFormat/Source/SystemUtility/File.h"
#include "../../Common/DocxFormat/Source/SystemUtility/SystemUtility.h"

#include "App.h"
#include "Core.h"
#include "Theme.h"
#include "Slide.h"
#include "Folder.h"
#include "PresProps.h"
#include "SlideMaster.h"
#include "ViewProps.h"
#include "VmlDrawing.h"
#include "TableStyles.h"
#include "Presentation.h"
//#include "NormalViewPr.h"
//#include "SlideViewPr.h"
//#include "NotesTextViewPr.h"

class CPPTXFile
{
public:

	CPPTXFile()
	{
		m_nCountThemes	=	0;
		m_nCountMasters	=	0;
		m_nCountLayouts	=	0;
		m_nCountSlides	=	0;
	}

	virtual ~CPPTXFile ()
	{

	}

	inline void Create (CString strFolder)
	{
		m_strDstFolder = strFolder;

		CDirectory::CreateDirectory(m_strDstFolder);
		CDirectory::CreateDirectory(m_strDstFolder, _T("docProps"));
		CString strPPT = m_strDstFolder + _T("\\ppt");
		CDirectory::CreateDirectory(strPPT);
		CDirectory::CreateDirectory(strPPT, _T("media"));

		InitApp();
		InitCore();
		InitViewProps();
		InitTableStyles();
		InitPresProps();

		CXmlWriter oXmlWriter;

		//LONG nCountThemes	=	0;
		//LONG nCountMasters	=	0;
		//LONG nCountLayouts	=	0;
		//LONG nCountSlides	=	0;

		//if (0 == nCountThemes || 0 == nCountMasters || 0 == nCountLayouts || 0 == nCountSlides)
		//	return;

		// app 
		oXmlWriter.ClearNoAttack();
		m_oApp.toXmlWriter(&oXmlWriter);
		oXmlWriter.SaveToFile(m_strDstFolder + _T("\\docProps\\app.xml"));

		// core
		oXmlWriter.ClearNoAttack();
		m_oCore.toXmlWriter(&oXmlWriter);
		oXmlWriter.SaveToFile(m_strDstFolder + _T("\\docProps\\core.xml"));

		// presProps
		oXmlWriter.ClearNoAttack();
		m_oPresProps.toXmlWriter(&oXmlWriter);
		oXmlWriter.SaveToFile(m_strDstFolder + _T("\\ppt\\presProps.xml"));

		// viewProps
		oXmlWriter.ClearNoAttack();
		m_oViewProps.toXmlWriter(&oXmlWriter);
		oXmlWriter.SaveToFile(m_strDstFolder + _T("\\ppt\\viewProps.xml"));

		// tablestyles
		oXmlWriter.ClearNoAttack();
		m_oTableStyles.toXmlWriter(&oXmlWriter);
		oXmlWriter.SaveToFile(m_strDstFolder + _T("\\ppt\\tableStyles.xml"));

		// Presentattion
		//....

		WriteContenTypes ();
		WriteRels ();
	}

protected:

	inline void InitApp()
	{
		m_oApp.TotalTime			=	0;
		m_oApp.Words				=	0;
		m_oApp.Application			=	_T("Teamlab Office");
		m_oApp.PresentationFormat	=	_T("On-screen Show (4:3)");
		m_oApp.Paragraphs			=	0;
		m_oApp.Slides				=	(int)m_arSlides.GetCount();
		m_oApp.Notes				=	(int)m_arSlides.GetCount();
		m_oApp.HiddenSlides			=	0;
		m_oApp.MMClips				=	2;
		m_oApp.ScaleCrop			=	false;

		int nCountThemes			=	static_cast<int>(m_arSlideMasters.GetCount());
		int nCountSlides			=	static_cast<int>(m_arSlides.GetCount());

		m_oApp.HeadingPairs.Add();
		m_oApp.HeadingPairs[0].m_type			=	_T("lpstr");
		m_oApp.HeadingPairs[0].m_strContent		=	_T("Theme");
		m_oApp.HeadingPairs.Add();
		m_oApp.HeadingPairs[1].m_type			=	_T("i4");
		m_oApp.HeadingPairs[1].m_iContent		=	nCountThemes;
		m_oApp.HeadingPairs.Add();
		m_oApp.HeadingPairs[2].m_type			=	_T("lpstr");
		m_oApp.HeadingPairs[2].m_strContent		=	_T("Slide Titles");
		m_oApp.HeadingPairs.Add();
		m_oApp.HeadingPairs[3].m_type			=	_T("i4");
		m_oApp.HeadingPairs[3].m_iContent		=	nCountSlides;

		for (int i = 0; i < nCountThemes; ++i)
		{
			CString s = _T("");
			s.Format(_T("Theme %d"), i + 1);
			m_oApp.TitlesOfParts.Add();
			m_oApp.TitlesOfParts[i].m_title = s;
		}

		for (int i = 0; i < nCountSlides; ++i)
		{
			CString s = _T("");
			s.Format(_T("Slide %d"), i + 1);
			m_oApp.TitlesOfParts.Add();
			m_oApp.TitlesOfParts[nCountThemes + i].m_title = s;
		}

		m_oApp.Company				=	_T("Teamlab Office");
		m_oApp.LinksUpToDate		=	false;
		m_oApp.SharedDoc			=	false;
		m_oApp.HyperlinksChanged	=	false;
		m_oApp.AppVersion			=	_T("1.0000");			
	}

	inline void InitCore()
	{
		m_oCore.title				=	_T("Slide 1");
		m_oCore.creator				=	_T("Teamlab Office");
		m_oCore.lastModifiedBy		=	_T("Teamlab Office");
		m_oCore.revision			=	_T("1");
	}
	inline void InitViewProps()
	{
		m_oViewProps.NormalViewPr = new PPTX::nsViewProps::NormalViewPr();
		m_oViewProps.NormalViewPr->restoredLeft.sz = 15620;
		m_oViewProps.NormalViewPr->restoredTop.sz = 94660;

		m_oViewProps.SlideViewPr = new PPTX::nsViewProps::SlideViewPr();
		m_oViewProps.SlideViewPr->CSldViewPr.CViewPr.attrVarScale = true;
		m_oViewProps.SlideViewPr->CSldViewPr.CViewPr.Scale.sx.n = 104;
		m_oViewProps.SlideViewPr->CSldViewPr.CViewPr.Scale.sx.d = 100;
		m_oViewProps.SlideViewPr->CSldViewPr.CViewPr.Scale.sy.n = 104;
		m_oViewProps.SlideViewPr->CSldViewPr.CViewPr.Scale.sy.d = 100;
		m_oViewProps.SlideViewPr->CSldViewPr.CViewPr.Origin.x = -1236;
		m_oViewProps.SlideViewPr->CSldViewPr.CViewPr.Origin.y = -90;

		m_oViewProps.SlideViewPr->CSldViewPr.GuideLst.Add();
		m_oViewProps.SlideViewPr->CSldViewPr.GuideLst[0].orient = _T("horz");
		m_oViewProps.SlideViewPr->CSldViewPr.GuideLst[0].pos = 2160;
		m_oViewProps.SlideViewPr->CSldViewPr.GuideLst.Add();
		m_oViewProps.SlideViewPr->CSldViewPr.GuideLst[1].pos = 2880;

		m_oViewProps.NotesTextViewPr = new PPTX::nsViewProps::NotesTextViewPr();
		m_oViewProps.NotesTextViewPr->CViewPr.Origin.x = 0;
		m_oViewProps.NotesTextViewPr->CViewPr.Origin.y = 0;
		m_oViewProps.NotesTextViewPr->CViewPr.Scale.sx.n = 100;
		m_oViewProps.NotesTextViewPr->CViewPr.Scale.sx.d = 100;
		m_oViewProps.NotesTextViewPr->CViewPr.Scale.sy.n = 100;
		m_oViewProps.NotesTextViewPr->CViewPr.Scale.sy.d = 100;

		m_oViewProps.GridSpacing = new PPTX::nsViewProps::GridSpacing();
		m_oViewProps.GridSpacing->cx = 72008;
		m_oViewProps.GridSpacing->cy = 72008;
	}

	inline void InitTableStyles()
	{
		m_oTableStyles.def = _T("{5C22544A-7EE6-4342-B048-85BDC9FD1C3A}");
	}
	inline void InitPresProps()
	{
		// nothing
	}

	inline void InitNotesMasters(int nIndexTheme)
	{
		/*
		HINSTANCE hInst = _AtlBaseModule.GetModuleInstance();

		CString strThemeNotes = _T("");
		strThemeNotes.Format(_T("\\ppt\\theme\\theme%d.xml"), (int)m_arThemes.GetCount() + 1);
		LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_XML_NOTESTHEME), _T("PPTXW"), m_strDstFolder + strThemeNotes);

		CDirectory::CreateDirectory(m_strDstFolder + _T("\\ppt\\notesMasters"));
		LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_XML_NOTESMASTER), _T("PPTXW"), m_strDstFolder + _T("\\ppt\\notesMasters\\notesMaster1.xml"));

		CDirectory::CreateDirectory(m_strDstFolder + _T("\\ppt\\notesMasters\\_rels"));
		CString strThemeNotesNum = _T("");
		strThemeNotesNum.Format(_T("%d"), (int)m_arThemes.GetCount() + 1);
		CString strVal = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
		<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\
		<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"../theme/theme") + strThemeNotesNum + _T(".xml\"/></Relationships>");
		CFile oFileRels;
		oFileRels.CreateFile(m_strDstFolder + _T("\\ppt\\notesMasters\\_rels\\notesMaster1.xml.rels"));
		oFileRels.WriteStringUTF8(strVal);
		oFileRels.CloseFile();
		*/
	}

	inline void InitNote()
	{
		/*
		PPTX::Logic::NvGrpSpPr& nvGrpSpPr = m_oDefaultNote.cSld.spTree.nvGrpSpPr;
		nvGrpSpPr.cNvPr.id = 1;
		nvGrpSpPr.cNvPr.name = _T("");

		PPTX::Logic::Xfrm* xfrm = new PPTX::Logic::Xfrm();
		xfrm->offX = 0;
		xfrm->offY = 0;
		xfrm->extX = 0;
		xfrm->extY = 0;
		xfrm->chOffX = 0;
		xfrm->chOffY = 0;
		xfrm->chExtX = 0;
		xfrm->chExtY = 0;
		m_oDefaultNote.cSld.spTree.m_name = _T("p:spTree");
		m_oDefaultNote.cSld.spTree.grpSpPr.xfrm = xfrm;

		// shape comment !!! (TODO:)
		PPTX::Logic::Shape* pShape = new PPTX::Logic::Shape();
		pShape->nvSpPr.cNvPr.id = 100000;
		pShape->nvSpPr.cNvPr.name = _T("");

		pShape->nvSpPr.cNvSpPr.noGrp = true;
		pShape->nvSpPr.cNvSpPr.noChangeArrowheads = true;

		pShape->nvSpPr.nvPr.ph = new PPTX::Logic::Ph();
		pShape->nvSpPr.nvPr.ph->type = _T("body");
		pShape->nvSpPr.nvPr.ph->idx = _T("1");

		PPTX::Logic::TxBody* pTxBody = new PPTX::Logic::TxBody();
		pTxBody->Paragrs.Add();

		PPTX::Logic::Run* pTxRun = new PPTX::Logic::Run();
		pTxRun->rPr = new PPTX::Logic::RunProperties();
		pTxRun->rPr->smtClean = false;
		pTxRun->SetText(_T("")); // enter simple comment here

		pShape->txBody = pTxBody;

		pTxBody->Paragrs[0].RunElems.Add();
		pTxBody->Paragrs[0].RunElems[0].InitRun(pTxRun);

		m_oDefaultNote.cSld.spTree.SpTreeElems.Add();
		m_oDefaultNote.cSld.spTree.SpTreeElems[0].InitElem(pShape);

		m_oDefaultNote.clrMapOvr = new PPTX::Logic::ClrMapOvr();
		*/
	}



	//
	inline void WriteContenTypes ()
	{
		// content types
		CStringWriter oContentTypes;
		oContentTypes.WriteString(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
									 <Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\">\
									 <Default Extension=\"png\" ContentType=\"image/png\" />\
									 <Default Extension=\"jpeg\" ContentType=\"image/jpeg\" />\
									 <Default Extension=\"wmf\" ContentType=\"image/x-wmf\" />\
									 <Default Extension=\"rels\" ContentType=\"application/vnd.openxmlformats-package.relationships+xml\" />\
									 <Default Extension=\"xml\" ContentType=\"application/xml\" />\
									 <Default Extension=\"gif\" ContentType=\"image/gif\"/>\
									 <Default Extension=\"emf\" ContentType=\"image/x-emf\"/>\
									 <Default Extension=\"jpg\" ContentType=\"application/octet-stream\"/>\
									 \
									 <Override PartName=\"/ppt/presentation.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml\" />\
									 <Override PartName=\"/ppt/presProps.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.presProps+xml\" />\
									 <Override PartName=\"/ppt/viewProps.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.viewProps+xml\" />\
									 <Override PartName=\"/ppt/tableStyles.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.tableStyles+xml\"/>\
									 <Override PartName=\"/docProps/core.xml\" ContentType=\"application/vnd.openxmlformats-package.core-properties+xml\" />\
									 <Override PartName=\"/docProps/app.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.extended-properties+xml\" />"));
		// themes
		for (LONG i = 0; i < (LONG)m_arThemes.GetCount(); ++i)
		{
			CString strTheme = _T("");
			strTheme.Format(_T("<Override PartName=\"/ppt/theme/theme%d.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.theme+xml\"/>"), i + 1);
			oContentTypes.WriteString(strTheme);
		}

		if (TRUE)
		{
			// notes theme
			CString strTheme = _T("");
			strTheme.Format(_T("<Override PartName=\"/ppt/theme/theme%d.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.theme+xml\"/>"), m_arThemes.GetCount() + 1);
			oContentTypes.WriteString(strTheme);

			oContentTypes.WriteString(_T("<Override PartName=\"/ppt/notesMasters/notesMaster1.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.notesMaster+xml\"/>"));
		}

		// masters
		for (LONG i = 0; i < m_nCountMasters; ++i)
		{
			CString strMaster = _T("");
			strMaster.Format(_T("<Override PartName=\"/ppt/slideMasters/slideMaster%d.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.slideMaster+xml\" />"), i + 1);
			oContentTypes.WriteString(strMaster);
		}

		// layouts
		for (LONG i = 0; i < m_nCountLayouts; ++i)
		{
			CString strL = _T("");
			strL.Format(_T("<Override PartName=\"/ppt/slideLayouts/slideLayout%d.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.slideLayout+xml\" />"), i + 1);
			oContentTypes.WriteString(strL);
		}

		// slides
		for (LONG i = 0; i < m_nCountSlides; ++i)
		{
			CString strS = _T("");
			strS.Format(_T("<Override PartName=\"/ppt/slides/slide%d.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.slide+xml\" />"), i + 1);
			oContentTypes.WriteString(strS);
		}

		// notes
		for (LONG i = 0; i < m_nCountSlides; ++i)
		{
			CString strN = _T("");
			strN.Format(_T("<Override PartName=\"/ppt/notesSlides/notesSlide%d.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.notesSlide+xml\"/>"), i + 1);
			oContentTypes.WriteString(strN);
		}

		oContentTypes.WriteString(_T("</Types>"));

		CFile oFile;
		oFile.CreateFile(m_strDstFolder + _T("\\[Content_Types].xml"));
		CString strContentTypes = oContentTypes.GetData();
		oFile.WriteStringUTF8(strContentTypes);
		oFile.CloseFile();
	}

	inline void WriteRels ()
	{
		CString strRELS = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
							 <Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\
							 <Relationship Id=\"rId3\" Type=\"http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties\" Target=\"docProps/core.xml\"/>\
							 <Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument\" Target=\"ppt/presentation.xml\"/>\
							 <Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties\" Target=\"docProps/app.xml\"/>\
							 </Relationships>");

		CDirectory::CreateDirectory(m_strDstFolder + _T("\\_rels"));
	
		CFile oFile;
		oFile.CreateFile(m_strDstFolder + _T("\\_rels\\.rels"));
		oFile.WriteStringUTF8(strRELS);
		oFile.CloseFile();
	}

protected:

	CString					m_strDstFolder;

	PPTX::App				m_oApp;
	PPTX::Core				m_oCore;
	PPTX::Presentation		m_oPresentation;
	PPTX::TableStyles		m_oTableStyles;
	PPTX::VmlDrawing		m_oVmlDrawing;
	PPTX::ViewProps			m_oViewProps;
	PPTX::PresProps			m_oPresProps;

	CAtlArray<PPTX::Slide>			m_arSlides;
	CAtlArray<PPTX::Theme>			m_arThemes;
	CAtlArray<PPTX::SlideMaster>	m_arSlideMasters;

	CAtlMap<BYTE, LONG>				m_mainTables;
	CAtlArray<LONG>					m_arSlides_Layout;

	LONG m_nCountThemes;
	LONG m_nCountMasters;
	LONG m_nCountLayouts;
	LONG m_nCountSlides;



};

class CFileTransform
{
public:
	CFileTransform()
	{
		m_pFolder		= NULL;
		m_pOfficeUtils	= NULL;

		WCHAR buffer[4096];
		GetTempPathW(4096, buffer);
		m_strTempDir = CStringW(buffer);
		GetLongPathNameW(m_strTempDir.GetString(), buffer, 4096);
		m_strTempDir = CStringW(buffer) + CStringW("_PPTXCC\\");

		m_oInit.Init();

		CoCreateInstance(__uuidof(AVSOfficeUtils::COfficeUtils), NULL, CLSCTX_INPROC_SERVER, __uuidof(AVSOfficeUtils::IOfficeUtils),(void**)&m_pOfficeUtils);
	}

	virtual ~CFileTransform()
	{
		RELEASEINTERFACE(m_pOfficeUtils);
		RELEASEOBJECT(m_pFolder);

	}

	HRESULT LoadFromFile(BSTR sSrcFileName, BSTR sXMLOptions)
	{
		OOX::CPath oSrcPath (sSrcFileName);
		m_strDirectory	=	oSrcPath.GetDirectory() + CString(L"_pptxTemp");

		RELEASEOBJECT(m_pFolder);
		m_pFolder = new PPTX::Folder();

		return S_OK;
	}

	HRESULT SaveToFile(BSTR sDstFileName, BSTR sXMLOptions)
	{
		if (NULL == m_pFolder)
			return S_FALSE;

		OOX::CPath oPath(m_strDirectory);
		m_pFolder->write(oPath);

		m_oFile.Create(oPath.GetPath());

		HRESULT hr = m_pOfficeUtils->CompressFileOrDirectory(CComBSTR(m_strDirectory), sDstFileName, -1);

		DeleteFolder (oPath.GetPath());

		return hr;
	}

private:

	inline static BOOL DeleteFolder (CString strFolder)
	{
		strFolder.Replace(L"\\", L"\\\\");

		TCHAR* pFolder = new TCHAR [strFolder.GetLength() + 2];
		if (NULL == pFolder)
			return FALSE;

		_tcscpy (pFolder, strFolder);  
		pFolder [strFolder.GetLength() + 1] = 0;   

		SHFILEOPSTRUCT oFileOp	=	
		{
			NULL,
			FO_DELETE,
			pFolder,
			L"",
			FOF_NOCONFIRMATION |
			FOF_NOERRORUI |
			FOF_SILENT,
			false,
			0,
			L"" 
		};

		if (0 == SHFileOperation(&oFileOp))
		{
			RELEASEARRAYOBJECTS(pFolder);
			return TRUE;
		}

		RELEASEARRAYOBJECTS(pFolder);
		return FALSE;
	}

private:

	AVSOfficeUtils::IOfficeUtils*	m_pOfficeUtils;
	PPTX::Folder*					m_pFolder;
	CStringW						m_strTempDir;
	CString							m_strDirectory;

	CPPTXFile						m_oFile;

	//NSPresentationEditor::CDocument	m_oDocument;

	CGdiPlusInit					m_oInit;
};
