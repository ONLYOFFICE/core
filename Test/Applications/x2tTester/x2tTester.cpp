#include "x2tTester.h"

Cx2tTester::Cx2tTester(std::wstring configPath)
{
	setConfig(configPath);

	m_xmlParams = m_outputFolder + L'\\' + L"params.xml";

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
	// setup & clear output folder
	if(NSDirectory::Exists(m_outputFolder))
		NSDirectory::DeleteDirectory(m_outputFolder);

	NSDirectory::CreateDirectory(m_outputFolder);
	std::wstring process = NSFile::GetProcessDirectory();

	std::vector<std::wstring> files = NSDirectory::GetFiles(m_inputFolder, false);
	COfficeFileFormatChecker checker;

	for(std::wstring& file : files)
	{

		// setup folder for output files
		std::wstring output_files_folder = m_outputFolder +
				L'\\' + NSFile::GetFileName(file);

		if(NSDirectory::Exists(output_files_folder))
			NSDirectory::DeleteDirectory(output_files_folder);

		NSDirectory::CreateDirectory(output_files_folder);

		std::wstring input_ext = NSFile::GetFileExtention(file);
		int input_format = COfficeFileFormatChecker::GetFormatByExtension(L'.' + input_ext);

		// input_ext in many output exts
		for(int& output_format : m_formats[input_format])
		{
			std::wstring output_file_path = output_files_folder
					+ L'\\' + NSFile::GetFileName(file) + checker.GetExtensionByType(output_format);

			// creating temporary xml file with params
			NSStringUtils::CStringBuilder builder;
			builder.WriteString(L"<?xml version=\"1.0\" encoding=\"utf-8\"?>");

			builder.WriteString(L"<Root>");

			builder.WriteString(L"<m_sFileFrom>");
			builder.WriteString(file);
			builder.WriteString(L"</m_sFileFrom>");

			builder.WriteString(L"<m_sFileTo>");
			builder.WriteString(output_file_path);
			builder.WriteString(L"</m_sFileTo>");


			builder.WriteString(L"<m_nFormatFrom>");
			builder.WriteString(std::to_wstring(input_format));
			builder.WriteString(L"</m_nFormatFrom>");


			builder.WriteString(L"<m_nFormatTo>");
			builder.WriteString(std::to_wstring(output_format));
			builder.WriteString(L"</m_nFormatTo>");

			builder.WriteString(L"<m_bDontSaveAdditional>true</m_bDontSaveAdditional>");

//			builder.WriteString(L"<m_sAllFontsPath>");
//			builder.WriteString(process + L"/fonts/AllFonts.js");
//			builder.WriteString(L"</m_sAllFontsPath>");

//			builder.WriteString(L"<m_sFontDir>");
//			builder.WriteString(process + L"/fonts");
//			builder.WriteString(L"</m_sFontDir>");

			std::wcout << process << std::endl;


			builder.WriteString(L"</Root>");

			std::wstring xml_params = builder.GetData();

			NSFile::CFileBinary::SaveToFile(m_xmlParams, xml_params, true);

			std::wcout << file << L" to " << output_file_path << L"... ";
			int exit_code = convert(file, output_file_path);

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
		for(int i = 0; i < nodes.GetCount(); i++)
		{
			XmlUtils::CXmlNode node;
			nodes.GetAt(i, node);
			std::wstring name = node.GetName();\

			// key-value
			if(name == L"reportPath") m_reportPath = node.GetText();
			else if(name == L"inputFolder") m_inputFolder = node.GetText();
			else if(name == L"outputFolder") m_outputFolder = node.GetText();
			else if(name == L"x2tPath") m_x2tPath = node.GetText();

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
int Cx2tTester::convert(const std::wstring& inputPath, const std::wstring& outputPath)
{
	std::wstring command = m_x2tPath + L' ' + m_xmlParams;
	wchar_t *ptr_command = new wchar_t[command.size() + 1];
	memcpy(ptr_command, command.c_str(), command.length() * sizeof(wchar_t));
	ptr_command[command.size()] = L'\0';

	PROCESS_INFORMATION processinfo;
	STARTUPINFO sturtupinfo;
	ZeroMemory(&processinfo,sizeof(PROCESS_INFORMATION));
	ZeroMemory(&sturtupinfo,sizeof(STARTUPINFO));
	sturtupinfo.cb = sizeof(sturtupinfo);

	BOOL bool_result = CreateProcessW(NULL, ptr_command,
								  NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &sturtupinfo, &processinfo);

	WaitForSingleObject(processinfo.hProcess, INFINITE);
	RELEASEARRAYOBJECTS(ptr_command);

	DWORD exit_code = 0;
	GetExitCodeProcess(processinfo.hProcess, &exit_code);
	return exit_code;
}
