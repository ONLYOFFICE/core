#include "Test.h"
#include "stdafx.h"
#include <ObjBase.h>
#include <vector>
#include <string>

namespace Test
{
	namespace
	{
		const std::vector<std::wstring> FoldersList();
		const std::vector<std::wstring> folders = FoldersList();

		const std::wstring TestPath = L".//..//..//Test//Documents";
		const std::wstring OriginDocx	= L"OriginDocx";
		const std::wstring OriginHtml	= L"OriginHtml.html"; 

		const std::wstring FromHtml	= L"DocxFromHtml";
		const std::wstring FromDocx	= L"HtmlFromDocx";
	}


	void Docx2Html()
	{
		CoInitialize(NULL);
		for(size_t i = 0; i < folders.size(); i++)
		{
			const std::wstring& folder = folders[i];

			const std::wstring inputPath  = TestPath + L"//" + folder + L"//" + OriginDocx;
			const std::wstring outputPath = TestPath + L"//" + folder + L"//" + FromDocx;

			AVSOfficeHtmlFile::IHtmlFilePtr ptr_html(__uuidof( AVSOfficeHtmlFile::CHtmlFile));
			ptr_html->SaveToFile(outputPath.c_str(), inputPath.c_str(), L"");
			
		}
		CoUninitialize();
	}


	void Html2Docx()
	{
		CoInitialize(NULL);
		for(size_t i = 0; i < folders.size(); i++)
		{
			const std::wstring& folder = folders[i];

			const std::wstring inputPath  = TestPath + L"//" + folder + L"//" + OriginHtml;
			const std::wstring outputPath = TestPath + L"//" + folder + L"//" + FromHtml;
			
			AVSOfficeHtmlFile::IHtmlFilePtr ptr_html(__uuidof( AVSOfficeHtmlFile::CHtmlFile));
			ptr_html->LoadFromFile(inputPath.c_str(), outputPath.c_str(), L"");
			
		}
		CoUninitialize();
	}
	

	namespace
	{
		const std::vector<std::wstring> FoldersList()
		{
			std::vector<std::wstring> folders;

//			folders.push_back(L"Empty");					//есть OriginHtml
//			folders.push_back(L"Empty2");					//есть OriginHtml
//			folders.push_back(L"Hello");					//есть OriginHtml
//			folders.push_back(L"Moskow");					//есть OriginHtml
//			folders.push_back(L"Chkalow");					//есть OriginHtml

//			folders.push_back(L"Inch");						//есть OriginHtml
//			folders.push_back(L"Space");					
//			folders.push_back(L"Breaks");					
//			folders.push_back(L"Pasport");					
//			folders.push_back(L"Garniture");				//есть OriginHtml

//			folders.push_back(L"Font");						//есть OriginHtml
//			folders.push_back(L"Align");					//есть OriginHtml
//			folders.push_back(L"Index");					//есть OriginHtml
//			folders.push_back(L"ColorText");				//есть OriginHtml
//			folders.push_back(L"Styles");

//			folders.push_back(L"Table");					//есть OriginHtml
//			folders.push_back(L"TableInTable");				//есть OriginHtml
//			folders.push_back(L"TableInTableInTable");
//			folders.push_back(L"BorderInTable");
//			folders.push_back(L"HMergedCells");				//есть OriginHtml

//			folders.push_back(L"VMergedCells");
//			folders.push_back(L"MergedCells");
//			folders.push_back(L"DiffMergedCells");
//			folders.push_back(L"Hyper");					//есть OriginHtml
//			folders.push_back(L"Image");					//есть OriginHtml

//			folders.push_back(L"Images");
//			folders.push_back(L"Endnote");
//			folders.push_back(L"Footnote");					//есть OriginHtml
//			folders.push_back(L"NumberList");
//			folders.push_back(L"Numbering");

//			folders.push_back(L"MarkerList");
//			folders.push_back(L"BulletNumbering");
//			folders.push_back(L"SomeLists");
//			folders.push_back(L"ListWithBulletsAndNumbers");
//			folders.push_back(L"SomeParagraphsInListItem");

//			folders.push_back(L"ColsAndBreaks");
//			folders.push_back(L"FootnotesOnOnePage");
//			folders.push_back(L"EndnotesOnOnePage");
//			folders.push_back(L"FootnotesOnSomePages");
//			folders.push_back(L"EndnotesOnSomePages");

//			folders.push_back(L"FootnoteAndEndnote");
//			folders.push_back(L"FootnotesAndEndnotes");
//			folders.push_back(L"ColorOfNotes");
//			folders.push_back(L"SizeOfNotes");
//			folders.push_back(L"NumFormatsOfNotes");

//			folders.push_back(L"StyleFromStyleLibrary");
//			folders.push_back(L"StylesFromStyleLibrary");
//			folders.push_back(L"DifStylesFromStyleLibrary");
//			folders.push_back(L"Ole-pict");
//			folders.push_back(L"Ole-Chart");	

//			folders.push_back(L"ManyOle");
//			folders.push_back(L"OleImageText");
//			folders.push_back(L"VertSpace");					//есть OriginHtml
//			folders.push_back(L"SymbolsSpace");					//есть OriginHtml
//			folders.push_back(L"Title");						//есть OriginHtml

//			folders.push_back(L"BigTitle");
//			folders.push_back(L"InnerHyper/");					//есть OriginHtml
//			folders.push_back(L"ManyInnerHyper/");
//			folders.push_back(L"FooterAndHeader/");
//			folders.push_back(L"NoName/");

//			folders.push_back(L"DifferentColumns/");
//			folders.push_back(L"Pict/");
//			folders.push_back(L"AutoShapes/");
//			folders.push_back(L"TextBoxes/");
//			folders.push_back(L"WordArt/");

//			folders.push_back(L"AVSMedia.NET/");
//			folders.push_back(L"HtmlBook/");

//			folders.push_back(L"ImageEMF/");

			return folders;
		}
	}
}
