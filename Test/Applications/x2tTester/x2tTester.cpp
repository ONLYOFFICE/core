#include "x2tTester.h"
#include "../../../X2tConverter/src/run.h"

struct FormatsList
{
public:
	std::vector<int> documents;
	std::vector<int> presentations;
	std::vector<int> spreadsheets;
	std::vector<int> crossplatform;
	std::vector<int> images;

	FormatsList();

	bool isDocoment(int format);
	bool isPresentation(int format);
	bool isSpreadsheet(int format);
	bool isCrossplatform(int format);
	bool isImage(int format);
};

bool FormatsList::isDocoment(int format)
{
	return std::find(documents.begin(), documents.end(), format) != documents.end();
}
bool FormatsList::isPresentation(int format)
{
	return std::find(presentations.begin(), presentations.end(), format) != presentations.end();
}
bool FormatsList::isSpreadsheet(int format)
{
	return std::find(spreadsheets.begin(), spreadsheets.end(), format) != spreadsheets.end();
}
bool FormatsList::isCrossplatform(int format)
{
	return std::find(crossplatform.begin(), crossplatform.end(), format) != crossplatform.end();
}
bool FormatsList::isImage(int format)
{
	return std::find(images.begin(), images.end(), format) != images.end();
}

FormatsList::FormatsList()
{
	documents.push_back(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX);
	documents.push_back(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC);
	documents.push_back(AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT);
	documents.push_back(AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF);
	documents.push_back(AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT);
	documents.push_back(AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML);
	documents.push_back(AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT);
	documents.push_back(AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB);
	documents.push_back(AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2);
	documents.push_back(AVS_OFFICESTUDIO_FILE_DOCUMENT_MOBI);
	documents.push_back(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM);
	documents.push_back(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX);
	documents.push_back(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM);
	documents.push_back(AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT_FLAT);
	documents.push_back(AVS_OFFICESTUDIO_FILE_DOCUMENT_OTT);
	documents.push_back(AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM);
	documents.push_back(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF);

	presentations.push_back(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX);
	presentations.push_back(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT);
	presentations.push_back(AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP);
	presentations.push_back(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX);
	presentations.push_back(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM);
	presentations.push_back(AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX);
	presentations.push_back(AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM);
	presentations.push_back(AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP_FLAT);
	presentations.push_back(AVS_OFFICESTUDIO_FILE_PRESENTATION_OTP);

	spreadsheets.push_back(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX);
	spreadsheets.push_back(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS);
	spreadsheets.push_back(AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS);
	spreadsheets.push_back(AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV);
	spreadsheets.push_back(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM);
	spreadsheets.push_back(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM);
	spreadsheets.push_back(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSB);
	spreadsheets.push_back(AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS_FLAT);
	spreadsheets.push_back(AVS_OFFICESTUDIO_FILE_SPREADSHEET_OTS);

	crossplatform.push_back(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF);

	images.push_back(AVS_OFFICESTUDIO_FILE_IMAGE_JPG);
	images.push_back(AVS_OFFICESTUDIO_FILE_IMAGE_PNG);
}

Cx2tTester::Cx2tTester(std::wstring configPath)
{
	m_bIsUseSystemFonts = true;
	setConfig(configPath);

	m_xmlParams = m_outputFolder + L"/params.xml";

	// todo check
	m_reportStream.open(m_reportPath);
	if(!m_reportStream.is_open())
	{
		std::wcerr << "Report file is not open!" << std::endl;
		exit(-1);
	}

	setReportHeader();
}
Cx2tTester::~Cx2tTester()
{
	m_reportStream.close();
}

void Cx2tTester::Start()
{
	// check fonts
	std::wstring sFontsDirectory = NSFile::GetProcessDirectory() + L"/fonts";
	CApplicationFontsWorker oWorker;
	oWorker.m_sDirectory = sFontsDirectory;
	if (!NSDirectory::Exists(oWorker.m_sDirectory))
		NSDirectory::CreateDirectory(oWorker.m_sDirectory);

	oWorker.m_bIsUseSystemFonts = m_bIsUseSystemFonts;

	for (std::vector<std::wstring>::iterator i = m_arAdditionalFontsDirs.begin(); i != m_arAdditionalFontsDirs.end(); i++)
		oWorker.m_arAdditionalFolders.push_back(*i);

	oWorker.m_bIsNeedThumbnails = false;
	NSFonts::IApplicationFonts* pFonts = oWorker.Check();
	RELEASEINTERFACE(pFonts);

	// setup & clear output folder
	if(NSDirectory::Exists(m_outputFolder))
		NSDirectory::DeleteDirectory(m_outputFolder);

	NSDirectory::CreateDirectory(m_outputFolder);

	std::vector<std::wstring> files = NSDirectory::GetFiles(m_inputFolder, false);
	COfficeFileFormatChecker checker;

	FormatsList formatsList;

	for(std::wstring& file : files)
	{
		// setup folder for output files
		std::wstring output_files_folder = m_outputFolder + L'/' + NSFile::GetFileName(file);

		if(NSDirectory::Exists(output_files_folder))
			NSDirectory::DeleteDirectory(output_files_folder);

		NSDirectory::CreateDirectory(output_files_folder);

		std::wstring input_ext = NSFile::GetFileExtention(file);
		int input_format = COfficeFileFormatChecker::GetFormatByExtension(L'.' + input_ext);

		// setup output_formats
		std::vector<int> output_formats;

		if(formatsList.isDocoment(input_format))
			output_formats = formatsList.documents;

		if(formatsList.isPresentation(input_format))
			output_formats = formatsList.presentations;

		if(formatsList.isSpreadsheet(input_format))
			output_formats = formatsList.spreadsheets;

		for(auto it : formatsList.crossplatform)
			output_formats.push_back(it);

		for(auto it : formatsList.images)
			output_formats.push_back(it);


		// input_format in many output exts
		for(int& output_format : output_formats)
		{
			if(input_format == output_format)
				continue;

			std::wstring output_file_path = output_files_folder
					+ L"/" + NSFile::GetFileName(file) + checker.GetExtensionByType(output_format);

			// creating temporary xml file with params
			NSStringUtils::CStringBuilder builder;
			builder.WriteString(L"<?xml version=\"1.0\" encoding=\"utf-8\"?>");

			builder.WriteString(L"<Root>");

			builder.WriteString(L"<m_sFileFrom>");
			builder.WriteEncodeXmlString(file);
			builder.WriteString(L"</m_sFileFrom>");

			builder.WriteString(L"<m_sFileTo>");
			builder.WriteEncodeXmlString(output_file_path);
			builder.WriteString(L"</m_sFileTo>");


			builder.WriteString(L"<m_nFormatFrom>");
			builder.WriteString(std::to_wstring(input_format));
			builder.WriteString(L"</m_nFormatFrom>");


			builder.WriteString(L"<m_nFormatTo>");
			builder.WriteString(std::to_wstring(output_format));
			builder.WriteString(L"</m_nFormatTo>");

			builder.WriteString(L"<m_sThemeDir>./</m_sThemeDir>");
			builder.WriteString(L"<m_bDontSaveAdditional>true</m_bDontSaveAdditional>");

			builder.WriteString(L"<m_sAllFontsPath>");
			builder.WriteEncodeXmlString(sFontsDirectory + L"/AllFonts.js");
			builder.WriteString(L"</m_sAllFontsPath>");

			builder.WriteString(L"<m_sFontDir>");
			builder.WriteEncodeXmlString(sFontsDirectory);
			builder.WriteString(L"</m_sFontDir>");

			if(formatsList.isImage(output_format))
			{
				builder.WriteString(L"<m_oThumbnail><format>4</format><aspect>2</aspect><first>false</first><width>1000</width><height>1000</height></m_oThumbnail>");
				builder.WriteString(L"<m_sJsonParams>{&quot;spreadsheetLayout&quot;:{&quot;gridLines&quot;:true,&quot;headings&quot;:true,&quot;fitToHeight&quot;:1,&quot;fitToWidth&quot;:1,&quot;orientation&quot;:&quot;landscape&quot;}}</m_sJsonParams>");
			}

			builder.WriteString(L"</Root>");

			std::wstring xml_params = builder.GetData();

			NSFile::CFileBinary::SaveToFile(m_xmlParams, xml_params, true);

			std::wcout << file << L" to " << output_file_path << L"... ";
			int exit_code = convert();

			if(!exit_code)
				std::wcout << "OK" << std::endl;
			else
				std::wcout << "BAD" << std::endl;

			// writing report
			Report report;
			report.inputFile = file;
			report.outputFile = output_file_path;
			report.inputExt = checker.GetExtensionByType(input_format);
			report.outputExt = checker.GetExtensionByType(output_format);
			report.exitCode = exit_code;

			writeReport(report);
		}
	}
	m_reportStream.close();
	NSFile::CFileBinary::Remove(m_xmlParams);
}

void Cx2tTester::setConfig(const std::wstring& configPath)
{
	XmlUtils::CXmlNode root;
	XmlUtils::CXmlNodes nodes;
	if(root.FromXmlFile(configPath) && root.GetChilds(nodes))
	{
		for(int i = 0; i < nodes.GetCount(); i++)
		{
			XmlUtils::CXmlNode node;
			nodes.GetAt(i, node);
			std::wstring name = node.GetName();

			// key-value
			if(name == L"reportPath") m_reportPath = node.GetText();
			else if(name == L"inputFolder") m_inputFolder = node.GetText();
			else if(name == L"outputFolder") m_outputFolder = node.GetText();
			else if(name == L"x2tPath") m_x2tPath = node.GetText();
			else if (name == L"fonts")
			{
				m_bIsUseSystemFonts = (1 == node.ReadValueInt(L"system", 1)) ? true : false;
				XmlUtils::CXmlNodes oNodeFontDirs = node.ReadNodesNoNS(L"directories");
				for (int nIndex = 0, nCount = oNodeFontDirs.GetCount(); nIndex < nCount; ++nIndex)
				{
					XmlUtils::CXmlNode oNodeDir;
					oNodeFontDirs.GetAt(nIndex, oNodeDir);
					m_arAdditionalFontsDirs.push_back(oNodeDir.GetText());
				}
			}
			// arrays
			else
			{
				XmlUtils::CXmlNodes array;
				node.GetNodes(L"item", array);

				std::wstring input_ext = L'.' + name;
				int input_format = COfficeFileFormatChecker::GetFormatByExtension(input_ext);

				if(!input_format)
					continue;

				for (int i = 0; i < array.GetCount(); i++)
				{
					XmlUtils::CXmlNode item;
					array.GetAt(i, item);

					std::wstring output_ext = L'.' + item.GetText();

					int output_format = COfficeFileFormatChecker::GetFormatByExtension(output_ext);

					if(!output_format)
						continue;

					m_formats[input_format].push_back(output_format);
				}
			}
		}
	}
	// else err
}
void Cx2tTester::setReportHeader()
{
	m_reportStream << L"Input file" << L"\t";
	m_reportStream << L"Output file" << L"\t";
	m_reportStream << L"Input extension" << L"\t";
	m_reportStream << L"Output extension" << L"\t";
	m_reportStream << L"Exit code" << L"\n";
}
void Cx2tTester::writeReport(const Report& report)
{
	m_reportStream << report.inputFile << L"\t";
	m_reportStream << report.outputFile << L"\t";
	m_reportStream << report.inputExt << L"\t";
	m_reportStream << report.outputExt << L"\t";
	m_reportStream << report.exitCode << L"\n";
}
int Cx2tTester::convert()
{
	return NSX2T::Convert(NSFile::GetDirectoryName(m_x2tPath), m_xmlParams);
}
