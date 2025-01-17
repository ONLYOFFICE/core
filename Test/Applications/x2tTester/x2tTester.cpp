#include "x2tTester.h"
#include "../../../X2tConverter/src/run.h"

class CFormatsList;
class Cx2tTester;
class CConverter;

std::wstring GetFileExtLower(const std::wstring& file)
{
	std::wstring input_ext = NSFile::GetFileExtention(file);
	for (auto& c : input_ext)
		c = std::tolower(c);
	return input_ext;
}

CFormatsList::CFormatsList()
{
}
CFormatsList::CFormatsList(const CFormatsList& list)
{
	*this = list;
}

CFormatsList& CFormatsList::operator=(const CFormatsList& list)
{
	m_documents.clear();
	m_presentations.clear();
	m_spreadsheets.clear();
	m_images.clear();
	m_crossplatform.clear();

	for(auto& val : list.m_documents)
		m_documents.push_back(val);

	for(auto& val : list.m_presentations)
		m_presentations.push_back(val);

	for(auto& val : list.m_spreadsheets)
		m_spreadsheets.push_back(val);

	for(auto& val : list.m_images)
		m_images.push_back(val);

	for(auto& val : list.m_draw)
		m_draw.push_back(val);

	for(auto& val : list.m_crossplatform)
		m_crossplatform.push_back(val);

	m_pdf = list.m_pdf;
	return *this;
}

std::vector<std::wstring> CFormatsList::GetDocuments() const
{
	return m_documents;
}
std::vector<std::wstring> CFormatsList::GetPresentations() const
{
	return m_presentations;
}
std::vector<std::wstring> CFormatsList::GetSpreadsheets() const
{
	return m_spreadsheets;
}
std::vector<std::wstring> CFormatsList::GetCrossplatform() const
{
	return m_crossplatform;
}
std::vector<std::wstring> CFormatsList::GetDraw() const
{
	return m_draw;
}
std::vector<std::wstring> CFormatsList::GetImages() const
{
	return m_images;
}
std::wstring CFormatsList::GetPdf() const
{
	return m_pdf;
}

bool CFormatsList::IsDocument(const std::wstring& ext) const
{
	return std::find(m_documents.begin(), m_documents.end(), ext) != m_documents.end();
}
bool CFormatsList::IsPresentation(const std::wstring& ext) const
{
	return std::find(m_presentations.begin(), m_presentations.end(), ext) != m_presentations.end();
}
bool CFormatsList::IsSpreadsheet(const std::wstring& ext) const
{
	return std::find(m_spreadsheets.begin(), m_spreadsheets.end(), ext) != m_spreadsheets.end();
}
bool CFormatsList::IsCrossplatform(const std::wstring& ext) const
{
	return std::find(m_crossplatform.begin(), m_crossplatform.end(), ext) != m_crossplatform.end();
}
bool CFormatsList::IsDraw(const std::wstring& ext) const
{
	return std::find(m_draw.begin(), m_draw.end(), ext) != m_draw.end();
}
bool CFormatsList::IsImage(const std::wstring& ext) const
{
	return std::find(m_images.begin(), m_images.end(), ext) != m_images.end();
}
bool CFormatsList::IsPdf(const std::wstring& ext) const
{
	return ext == m_pdf;
}
bool CFormatsList::IsAny(const std::wstring& ext) const
{
	return IsDocument(ext) || IsPresentation(ext) || IsSpreadsheet(ext) || IsCrossplatform(ext) || IsImage(ext) || IsPdf(ext) || IsDraw(ext);
}

void CFormatsList::AddDocument(const std::wstring& ext)
{
	m_documents.push_back(ext);
}
void CFormatsList::AddPresentation(const std::wstring& ext)
{
	m_presentations.push_back(ext);
}
void CFormatsList::AddSpreadsheet(const std::wstring& ext)
{
	m_spreadsheets.push_back(ext);
}
void CFormatsList::AddCrossplatform(const std::wstring& ext)
{
	m_crossplatform.push_back(ext);
}
void CFormatsList::AddDraw(const std::wstring& ext)
{
	m_draw.push_back(ext);
}
void CFormatsList::AddImage(const std::wstring& ext)
{
	m_images.push_back(ext);
}

std::vector<std::wstring> CFormatsList::GetAllExts() const
{
	std::vector<std::wstring> all_formats;

	for (const auto& val : m_documents)
		all_formats.push_back(val);

	for (const auto& val : m_presentations)
		all_formats.push_back(val);

	for (const auto& val : m_spreadsheets)
		all_formats.push_back(val);

	for (const auto& val : m_images)
		all_formats.push_back(val);

	for (const auto& val : m_crossplatform)
		all_formats.push_back(val);

	for (const auto& val : m_draw)
		all_formats.push_back(val);

	if (!m_pdf.empty())
		all_formats.push_back(m_pdf);

	return all_formats;
}

CFormatsList CFormatsList::GetDefaultExts()
{
	CFormatsList list;

	list.m_documents.push_back(L"doct");
	list.m_documents.push_back(L"doc");
	list.m_documents.push_back(L"docm");
	list.m_documents.push_back(L"docx");
	list.m_documents.push_back(L"docxf");
	list.m_documents.push_back(L"dot");
	list.m_documents.push_back(L"dotm");
	list.m_documents.push_back(L"dotx");
	list.m_documents.push_back(L"epub");
	list.m_documents.push_back(L"fb2");
	list.m_documents.push_back(L"fodt");
	list.m_documents.push_back(L"htm");
	list.m_documents.push_back(L"html");
	list.m_documents.push_back(L"mht");
	list.m_documents.push_back(L"odt");
	list.m_documents.push_back(L"ott");
	list.m_documents.push_back(L"oxps");
	list.m_documents.push_back(L"rtf");
	list.m_documents.push_back(L"stw");
	list.m_documents.push_back(L"sxw");
	list.m_documents.push_back(L"txt");
	list.m_documents.push_back(L"wps");
	list.m_documents.push_back(L"wpt");

	list.m_presentations.push_back(L"pptt");
	list.m_presentations.push_back(L"dps");
	list.m_presentations.push_back(L"dpt");
	list.m_presentations.push_back(L"fodp");
	list.m_presentations.push_back(L"odp");
	list.m_presentations.push_back(L"otp");
	list.m_presentations.push_back(L"pot");
	list.m_presentations.push_back(L"potm");
	list.m_presentations.push_back(L"potx");
	list.m_presentations.push_back(L"pps");
	list.m_presentations.push_back(L"ppsm");
	list.m_presentations.push_back(L"ppsx");
	list.m_presentations.push_back(L"ppt");
	list.m_presentations.push_back(L"pptm");
	list.m_presentations.push_back(L"pptx");
	list.m_presentations.push_back(L"sxi");

	list.m_spreadsheets.push_back(L"xlst");
	list.m_spreadsheets.push_back(L"csv");
	list.m_spreadsheets.push_back(L"et");
	list.m_spreadsheets.push_back(L"ett");
	list.m_spreadsheets.push_back(L"fods");
	list.m_spreadsheets.push_back(L"ods");
	list.m_spreadsheets.push_back(L"ots");
	list.m_spreadsheets.push_back(L"sxc");
	list.m_spreadsheets.push_back(L"xls");
	list.m_spreadsheets.push_back(L"xlsb");
	list.m_spreadsheets.push_back(L"xlsm");
	list.m_spreadsheets.push_back(L"xlsx");
	list.m_spreadsheets.push_back(L"xlt");
	list.m_spreadsheets.push_back(L"xltm");
	list.m_spreadsheets.push_back(L"xltx");

	list.m_crossplatform.push_back(L"djvu");
	list.m_crossplatform.push_back(L"xps");

	list.m_draw.push_back(L"vsdx");

	list.m_pdf = L"pdf";

	return list;
}

CFormatsList CFormatsList::GetOutputExts()
{
	CFormatsList list;

	list.m_documents.push_back(L"doct");
	list.m_documents.push_back(L"docm");
	list.m_documents.push_back(L"docx");
	list.m_documents.push_back(L"docxf");
	list.m_documents.push_back(L"dotm");
	list.m_documents.push_back(L"dotx");
	list.m_documents.push_back(L"epub");
	list.m_documents.push_back(L"fb2");
	list.m_documents.push_back(L"html");
	list.m_documents.push_back(L"odt");
	list.m_documents.push_back(L"ott");
	list.m_documents.push_back(L"rtf");
	list.m_documents.push_back(L"txt");

	list.m_presentations.push_back(L"pptt");
	list.m_presentations.push_back(L"odp");
	list.m_presentations.push_back(L"otp");
	list.m_presentations.push_back(L"potm");
	list.m_presentations.push_back(L"potx");
	list.m_presentations.push_back(L"ppsm");
	list.m_presentations.push_back(L"ppsx");
	list.m_presentations.push_back(L"pptm");
	list.m_presentations.push_back(L"pptx");

	list.m_spreadsheets.push_back(L"xlst");
	list.m_spreadsheets.push_back(L"csv");
	list.m_spreadsheets.push_back(L"ods");
	list.m_spreadsheets.push_back(L"ots");
	list.m_spreadsheets.push_back(L"xlsb");
	list.m_spreadsheets.push_back(L"xlsm");
	list.m_spreadsheets.push_back(L"xlsx");
	list.m_spreadsheets.push_back(L"xltm");
	list.m_spreadsheets.push_back(L"xltx");

	list.m_crossplatform.push_back(L"djvu");
	list.m_crossplatform.push_back(L"xps");

	list.m_images.push_back(L"jpg");
	list.m_images.push_back(L"png");

	list.m_pdf = L"pdf";

	return list;
}

CFormatsList CFormatsList::GetExtractExts()
{
	CFormatsList list;

	list.m_images.push_back(L"emf");
	list.m_images.push_back(L"wmf");

	return list;
}

Cx2tTester::Cx2tTester(const std::wstring& configPath)
{
	m_bIsUseSystemFonts = true;
	m_bIsErrorsOnly = false;
	m_bIsTimestamp = true;
	m_bIsDeleteOk = false;
	m_bIsFilenameCsvTxtParams = true;
	m_bIsFilenamePassword = true;
	m_bTroughConversion = false;
	m_bSaveEnvironment = false;

	m_bExtract = false;
	m_bConvertBeforeExtract = false;

	m_defaultCsvDelimiter = L";";
	m_defaultCsvTxtEndcoding = L"UTF-8";
	m_inputFormatsList = CFormatsList::GetDefaultExts();
	m_outputFormatsList = CFormatsList::GetOutputExts();
	m_extractFormatsList = CFormatsList::GetExtractExts();
	m_timeout = 5 * 60; // 5 min

	SetConfig(configPath);

	m_errorsXmlDirectory = m_outputDirectory + FILE_SEPARATOR_STR + L"_errors";
	m_troughConversionDirectory = m_outputDirectory + FILE_SEPARATOR_STR + L"_t";
	m_tempDirectory = m_outputDirectory + FILE_SEPARATOR_STR + L"_temp";
	m_fontsDirectory = NSFile::GetProcessDirectory() + FILE_SEPARATOR_STR + L"fonts";


	// CorrectPathW works strange with directories starts with "./"
	if(m_outputDirectory.find(L"./") == 0)
		m_outputDirectory.erase(0, 2);

	// no slash at the end
	if(m_outputDirectory.size() > 2 && m_outputDirectory[m_outputDirectory.size() - 1] == L'/')
		m_outputDirectory.erase(m_outputDirectory.size() - 1, 1);

	// on linux the backslash can be part of the filename
#ifdef WIN32

	if(m_outputDirectory.size() > 2 && m_outputDirectory[m_outputDirectory.size() - 1] == L'\\')
		m_outputDirectory.erase(m_outputDirectory.size() - 1, 1);

#endif

	if(m_bIsTimestamp)
	{
		time_t now = time(0);
		std::tm* time = std::localtime(&now);
		std::wstring timestamp =
		        std::to_wstring(time->tm_mday) + L"_" +
		        std::to_wstring(time->tm_mon + 1) + L"_" +
		        std::to_wstring(time->tm_year + 1900) + L"_" +
		        std::to_wstring(time->tm_hour) + L"_" +
		        std::to_wstring(time->tm_min) + L"_" +
		        std::to_wstring(time->tm_sec);

		std::wstring report_ext = NSFile::GetFileExtention(m_reportFile);
		m_reportFile = m_reportFile.substr(0, m_reportFile.size() - report_ext.size() - 1);
		m_reportFile += L"_" + timestamp + L"." + report_ext;
	}

	if(NSFile::CFileBinary::Exists(m_reportFile))
		NSFile::CFileBinary::Remove(m_reportFile);

	if(!m_reportStream.CreateFileW(m_reportFile))
	{
		std::cerr << "Report file is not open!" << std::endl;
		exit(-1);
	}

	m_coresCS.InitializeCriticalSection();
	m_reportCS.InitializeCriticalSection();
	m_outputCS.InitializeCriticalSection();

	m_currentProc = 0;
	WriteReportHeader();
}
Cx2tTester::~Cx2tTester()
{
	m_coresCS.DeleteCriticalSection();
	m_reportCS.DeleteCriticalSection();
	m_outputCS.DeleteCriticalSection();
	m_reportStream.CloseFile();

	for(auto&& val : m_deleteLaterFiles)
		NSFile::CFileBinary::Remove(val);

	for(auto&& val : m_deleteLaterDirectories)
		NSDirectory::DeleteDirectory(val);
}

void Cx2tTester::SetConfig(const std::wstring& configPath)
{
	bool default_input_formats = true;
	bool default_output_formats = true;

	XmlUtils::CXmlNode root;
	std::vector<XmlUtils::CXmlNode> nodes;
	if(root.FromXmlFile(configPath) && root.GetChilds(nodes))
	{
		for(size_t i = 0; i < nodes.size(); i++)
		{
			XmlUtils::CXmlNode node = nodes[i];
			std::wstring name = node.GetName();

			// key-value
			if(name == L"reportPath") m_reportFile = node.GetText();
			else if(name == L"inputDirectory") m_inputDirectory = node.GetText();
			else if(name == L"outputDirectory") m_outputDirectory = node.GetText();
			else if(name == L"x2tPath") m_x2tPath = node.GetText();
			else if(name == L"cores") m_maxProc = std::stoi(node.GetText());
			else if(name == L"errorsOnly" && !node.GetText().empty()) m_bIsErrorsOnly = std::stoi(node.GetText());
			else if(name == L"timestamp" && !node.GetText().empty()) m_bIsTimestamp = std::stoi(node.GetText());
			else if(name == L"deleteOk" && !node.GetText().empty()) m_bIsDeleteOk = std::stoi(node.GetText());
			else if(name == L"timeout" && !node.GetText().empty()) m_timeout = std::stoi(node.GetText());
			else if(name == L"filenameCsvTxtParams" && !node.GetText().empty()) m_bIsFilenameCsvTxtParams = std::stoi(node.GetText());
			else if(name == L"filenamePassword" && !node.GetText().empty()) m_bIsFilenamePassword = std::stoi(node.GetText());
			else if(name == L"troughConversion" && !node.GetText().empty()) m_bTroughConversion = std::stoi(node.GetText());
			else if(name == L"saveEnvironment" && !node.GetText().empty()) m_bSaveEnvironment = std::stoi(node.GetText());
			else if(name == L"defaultCsvTxtEncoding" && !node.GetText().empty()) m_defaultCsvTxtEndcoding = node.GetText();
			else if(name == L"extract" && !node.GetText().empty()) m_bExtract = std::stoi(node.GetText());
			else if(name == L"convertBeforeExtract" && !node.GetText().empty()) m_bConvertBeforeExtract = std::stoi(node.GetText());
			else if(name == L"defaultCsvDelimiter" && !node.GetText().empty()) m_defaultCsvDelimiter = (wchar_t)std::stoi(node.GetText(), nullptr, 16);
			else if(name == L"inputFilesList" && !node.GetText().empty())
			{
				XmlUtils::CXmlNode files_list_root;
				std::vector<XmlUtils::CXmlNode> files_list_nodes;

				std::wstring files_list = node.GetText();
				if(files_list_root.FromXmlFile(files_list) && files_list_root.GetChilds(files_list_nodes))
				{
					for(size_t j = 0; j < files_list_nodes.size(); j++)
					{
						XmlUtils::CXmlNode &n = files_list_nodes[j];
						m_inputFiles.push_back(n.GetText());
					}
				}
				else
				{
					std::cerr << "Input files list is not open!" << std::endl;
				}
			}
			else if(name == L"input" && !node.GetText().empty())
			{
				default_input_formats = false;
				std::wstring extensions = node.GetText();
				extensions += L' ';
				m_inputExts = ParseExtensionsString(extensions, m_inputFormatsList);
			}
			else if(name == L"output" && !node.GetText().empty())
			{
				default_output_formats = false;
				std::wstring extensions = node.GetText();
				extensions += L' ';
				m_outputExts = ParseExtensionsString(extensions, m_outputFormatsList);
			}
			else if (name == L"fonts")
			{
				m_bIsUseSystemFonts = (1 == node.ReadAttributeInt(L"system", 1)) ? true : false;
				std::vector<XmlUtils::CXmlNode> oNodeFontDirs = node.ReadNodesNoNS(L"directory");
				for (size_t nIndex = 0, nCount = oNodeFontDirs.size(); nIndex < nCount; ++nIndex)
				{
					XmlUtils::CXmlNode &oNodeDir = oNodeFontDirs[nIndex];
					m_arAdditionalFontsDirs.push_back(oNodeDir.GetText());
				}
			}
		}
	}
	else
	{
		std::cerr << "Config file is not open!" << std::endl;
		exit(-1);
	}

	if (default_input_formats)
		m_inputExts = m_inputFormatsList.GetAllExts();

	if (default_output_formats)
	{
		if (m_bExtract)
			m_outputExts = m_extractFormatsList.GetAllExts();
		else
			m_outputExts = m_outputFormatsList.GetAllExts();
	}

}
void Cx2tTester::Start()
{
	// setup timer
	m_timeStart = NSTimers::GetTickCount();

	m_outputDirectory = CorrectPathW(m_outputDirectory);
	m_errorsXmlDirectory = CorrectPathW(m_errorsXmlDirectory);
	m_troughConversionDirectory = CorrectPathW(m_troughConversionDirectory);
	m_tempDirectory = CorrectPathW(m_tempDirectory);

	// setup & clear output folder
	if(NSDirectory::Exists(m_outputDirectory))
		NSDirectory::DeleteDirectory(m_outputDirectory);

	NSDirectory::CreateDirectory(m_outputDirectory);

	// setup & clear errors folder
	if(NSDirectory::Exists(m_errorsXmlDirectory))
		NSDirectory::DeleteDirectory(m_errorsXmlDirectory);

	NSDirectory::CreateDirectory(m_errorsXmlDirectory);

	// check fonts
	CApplicationFontsWorker fonts_worker;
	fonts_worker.m_sDirectory = m_fontsDirectory;
	if (!NSDirectory::Exists(fonts_worker.m_sDirectory))
		NSDirectory::CreateDirectory(fonts_worker.m_sDirectory);

	fonts_worker.m_bIsUseSystemFonts = m_bIsUseSystemFonts;

	for (std::vector<std::wstring>::iterator i = m_arAdditionalFontsDirs.begin(); i != m_arAdditionalFontsDirs.end(); i++)
	{
		std::wstring sFolder = *i;
		if (0 == sFolder.find(L"."))
			sFolder = NSFile::GetProcessDirectory() + FILE_SEPARATOR_STR + sFolder;
		fonts_worker.m_arAdditionalFolders.push_back(sFolder);
	}

	fonts_worker.m_bIsNeedThumbnails = false;
	NSFonts::IApplicationFonts* pFonts = fonts_worker.Check();
	RELEASEINTERFACE(pFonts);

	std::vector<std::wstring> files = NSDirectory::GetFiles(m_inputDirectory, true);
	for(int i = 0; i < files.size(); i++)
	{
		std::wstring& input_file = files[i];
		std::wstring input_filename = NSFile::GetFileName(input_file);
		std::wstring input_ext = GetFileExtLower(input_file);

		// if no format in input formats - skip
		if(std::find(m_inputExts.begin(), m_inputExts.end(), input_ext) == m_inputExts.end()
		        || (std::find(m_inputFiles.begin(), m_inputFiles.end(), input_filename) == m_inputFiles.end()
		            && !m_inputFiles.empty()))
		{
			files.erase(files.begin() + i);
			i--;
		}
	}

	if (m_bExtract)
	{
		COfficeFileFormatChecker checker;
		COfficeUtils utils;
		std::vector<std::wstring> files_to_convert;

		for (size_t i = 0; i < files.size(); i++)
			if (utils.IsArchive(files[i]) == S_FALSE && checker.isOfficeFile(files[i]))
			{
				if (m_bConvertBeforeExtract)
					files_to_convert.push_back(files[i]);
				files.erase(files.begin() + i);
			}

		if (!files_to_convert.empty())
		{
			if(NSDirectory::Exists(m_tempDirectory))
				NSDirectory::DeleteDirectory(m_tempDirectory);

			NSDirectory::CreateDirectories(m_tempDirectory);

			auto copy_inputDirectory = m_inputDirectory;
			auto copy_outputDirectory = m_outputDirectory;
			auto copy_outputExts = m_outputExts;

			m_outputDirectory = m_tempDirectory;
			m_outputExts = {L"docx"};

			Convert(files_to_convert, true, true);

			m_outputDirectory = copy_outputDirectory;
			m_outputExts = copy_outputExts;

			m_inputDirectory = m_tempDirectory;
			std::vector<std::wstring> temp_files = NSDirectory::GetFiles(m_tempDirectory, true);
			Extract(temp_files);

			m_inputDirectory = copy_inputDirectory;
		}

		Extract(files);

		if(NSDirectory::Exists(m_tempDirectory))
			NSDirectory::DeleteDirectory(m_tempDirectory);

		return;
	}

	// conversion in _t directory -> _t directory to output
	if(m_bTroughConversion)
	{
		if(NSDirectory::Exists(m_troughConversionDirectory))
			NSDirectory::DeleteDirectory(m_troughConversionDirectory);

		NSDirectory::CreateDirectory(m_troughConversionDirectory);

		auto copy_outputDirectory = m_outputDirectory;
		auto copy_outputExts = m_outputExts;

		m_outputDirectory = m_troughConversionDirectory;
		m_outputExts = {L"doct", L"xlst", L"pptt"};

		Convert(files, true, true);

		m_outputDirectory = copy_outputDirectory;
		m_inputExts = m_outputExts;
		m_outputExts = copy_outputExts;

		m_inputDirectory = m_troughConversionDirectory;
		files = NSDirectory::GetFiles(m_troughConversionDirectory, true);
	}

	Convert(files);
	WriteTime();
}

void Cx2tTester::Convert(const std::vector<std::wstring>& files, bool bNoDirectory, bool bTrough)
{
	if(files.size() < m_maxProc)
		m_maxProc = files.size();

	for(int i = 0; i < files.size(); i++)
	{
		const std::wstring& input_file = files[i];
		std::wstring input_filename = NSFile::GetFileName(input_file);
		std::wstring input_ext = GetFileExtLower(input_file);
		std::wstring input_file_directory = NSFile::GetDirectoryName(input_file);

		// takes full directory after input folder
		std::wstring input_subfolders = input_file_directory.substr(m_inputDirectory.size(),
		                                                            input_file_directory.size() - m_inputDirectory.size());

		std::wstring output_files_directory = m_outputDirectory + input_subfolders;
		if(!bNoDirectory)
			output_files_directory += FILE_SEPARATOR_STR + input_filename;

		// setup output_formats for file
		std::vector<std::wstring> output_file_exts;

		for(auto& ext : m_outputExts)
		{
			// documents -> documents
			if(((m_outputFormatsList.IsDocument(ext) && m_inputFormatsList.IsDocument(input_ext))
			    // spreadsheets -> spreadsheets
			    || (m_outputFormatsList.IsSpreadsheet(ext) && m_inputFormatsList.IsSpreadsheet(input_ext))
			    //presentations -> presentations
			    || (m_outputFormatsList.IsPresentation(ext) && m_inputFormatsList.IsPresentation(input_ext))
			    // xps -> docx
			    || (ext == L"docx" && input_ext == L"xps")
			    // pdf -> docx
			    || (ext == L"docx" && m_inputFormatsList.IsPdf(input_ext))
			    // all formats -> images
			    || m_outputFormatsList.IsImage(ext)
			    // all formats -> pdf
			    || m_outputFormatsList.IsPdf(ext))
			        // input format != output format
			        && ext != input_ext
			        // any good input ext
			        && m_inputFormatsList.IsAny(input_ext))
			{
				output_file_exts.push_back(ext);
			}
		}

		if(output_file_exts.empty())
			continue;

		// setup & clear output subfolder
		if (!NSDirectory::Exists(output_files_directory))
			NSDirectory::CreateDirectories(output_files_directory);

		std::wstring csvTxtEncodingS = m_defaultCsvTxtEndcoding;
		std::wstring csvDelimiter = m_defaultCsvDelimiter;

		// setup csv & txt additional params
		if(m_bIsFilenameCsvTxtParams
		        || input_ext == L"txt"
		        || input_ext == L"csv")
		{
			std::wstring find_str = L"[cp";
			size_t pos1 = input_filename.find(find_str);
			size_t pos2 = input_filename.find(L"]", pos1 + 1);
			if(pos1 != std::wstring::npos && pos2 != std::wstring::npos)
				csvTxtEncodingS = input_filename.substr(pos1 + find_str.size(), pos2 - pos1 - find_str.size());
		}

		int csvTxtEncoding;
		for(auto &val : NSUnicodeConverter::Encodings)
			if(val.Name == U_TO_UTF8(csvTxtEncodingS))
			{
				csvTxtEncoding = val.Index;
				break;
			}

		if(m_bIsFilenameCsvTxtParams || input_ext == L"csv")
		{
			std::wstring find_str = L"[del%";
			size_t pos1 = input_filename.find(find_str);
			size_t pos2 = input_filename.find(L"]", pos1 + 1);
			if(pos1 != std::wstring::npos && pos2 != std::wstring::npos)
				csvDelimiter = (wchar_t)std::stoi(input_filename.substr(pos1 + find_str.size(), pos2 - pos1 - find_str.size()), nullptr, 16);
		}

		std::wstring password;
		if(m_bIsFilenamePassword)
		{
			std::wstring find_str = L"[pass";
			size_t pos1 = input_filename.find(find_str);
			size_t pos2 = input_filename.find(L"]", pos1 + 1);
			if(pos1 != std::wstring::npos && pos2 != std::wstring::npos)
				password = input_filename.substr(pos1 + find_str.size(), pos2 - pos1 - find_str.size());
		}

		// waiting...
		do
		{
			NSThreads::Sleep(50);
		} while(IsAllBusy());



		m_coresCS.Enter();

		// setup & start new coverter
		CConverter *converter = new CConverter(this);
		converter->SetInputFile(input_file);
		converter->SetInputExt(input_ext);
		converter->SetOutputFilesDirectory(output_files_directory);
		converter->SetOutputExts(output_file_exts);
		converter->SetFontsDirectory(m_fontsDirectory);
		converter->SetX2tPath(m_x2tPath);
		converter->SetErrorsOnly(m_bIsErrorsOnly);
		converter->SetDeleteOk(m_bIsDeleteOk);
		converter->SetTrough(bTrough);
		converter->SetXmlErrorsDirectory(m_errorsXmlDirectory);
		converter->SetCsvTxtEncoding(csvTxtEncoding);
		converter->SetCsvDelimiter(csvDelimiter);
		converter->SetPassword(password);
		converter->SetTimeout(m_timeout);
		converter->SetFilesCount(files.size(), i + 1);
		converter->SetSaveEnvironment(m_bSaveEnvironment);
		converter->DestroyOnFinish();
		m_currentProc++;

		m_coresCS.Leave();

		converter->Start(0);
	}

	// waiting all procs end
	while(!IsAllFree())
		NSThreads::Sleep(150);
}
void Cx2tTester::Extract(const std::vector<std::wstring>& files)
{
	if(files.size() < m_maxProc)
		m_maxProc = files.size();

	for (int i = 0; i < files.size(); i++)
	{
		const std::wstring& input_file = files[i];
		std::wstring input_filename = NSFile::GetFileName(input_file);
		std::wstring input_file_directory = NSFile::GetDirectoryName(input_file);
		std::wstring input_subfolders = input_file_directory.substr(m_inputDirectory.size(),
		                                                            input_file_directory.size() - m_inputDirectory.size());
		std::wstring output_files_directory = m_outputDirectory + input_subfolders + FILE_SEPARATOR_STR + input_filename;

		if(!NSDirectory::Exists(output_files_directory))
			NSDirectory::CreateDirectories(output_files_directory);

		// waiting...
		do
		{
			NSThreads::Sleep(50);
		} while(IsAllBusy());

		m_coresCS.Enter();

		// setup & start new extractor
		CExtractor *extractor = new CExtractor(this);
		extractor->SetInputFile(input_file);
		extractor->SetOutputFilesDirectory(output_files_directory);
		extractor->SetExtractExts(m_outputExts);
		extractor->SetFilesCount(files.size(), i + 1);
		extractor->DestroyOnFinish();
		m_currentProc++;

		m_coresCS.Leave();

		extractor->Start(0);
	}

	// waiting all procs end
	while(!IsAllFree())
		NSThreads::Sleep(150);
}

void Cx2tTester::WriteReportHeader()
{
	CTemporaryCS CS(&m_reportCS);
	m_reportStream.WriteStringUTF8(L"Input file\t", true);
	m_reportStream.WriteStringUTF8(L"Output file\t", false);
	m_reportStream.WriteStringUTF8(L"Direction\t", false);
	m_reportStream.WriteStringUTF8(L"Time\t", false);
	m_reportStream.WriteStringUTF8(L"Input size\t", false);
	m_reportStream.WriteStringUTF8(L"Output size\t", false);
	m_reportStream.WriteStringUTF8(L"Exit code\t", false);
	m_reportStream.WriteStringUTF8(L"Log\n", false);
}
void Cx2tTester::WriteReport(const Report& report)
{
	CTemporaryCS CS(&m_reportCS);

	m_reportStream.WriteStringUTF8(report.inputFile + L"\t", false);
	m_reportStream.WriteStringUTF8(report.outputFile + L"\t", false);
	m_reportStream.WriteStringUTF8(report.direction + L"\t", false);
	m_reportStream.WriteStringUTF8(std::to_wstring(report.time) + L"\t", false);
	m_reportStream.WriteStringUTF8(std::to_wstring(report.inputSize) + L"\t", false);
	m_reportStream.WriteStringUTF8(std::to_wstring(report.outputSize) + L"\t", false);
	m_reportStream.WriteStringUTF8(report.exitCode + L"\t", false);
	m_reportStream.WriteStringUTF8(report.log + L"\n", false);
}
void Cx2tTester::WriteReports(const std::vector<Report>& reports)
{
	CTemporaryCS CS(&m_reportCS);
	for(auto& report : reports)
	{
		m_reportStream.WriteStringUTF8(report.inputFile + L"\t", false);
		m_reportStream.WriteStringUTF8(report.outputFile + L"\t", false);
		m_reportStream.WriteStringUTF8(report.direction + L"\t", false);
		m_reportStream.WriteStringUTF8(std::to_wstring(report.time) + L"\t", false);
		m_reportStream.WriteStringUTF8(std::to_wstring(report.inputSize) + L"\t", false);
		m_reportStream.WriteStringUTF8(std::to_wstring(report.outputSize) + L"\t", false);
		m_reportStream.WriteStringUTF8(report.exitCode + L"\t", false);
		m_reportStream.WriteStringUTF8(report.log + L"\n", false);
	}
}
void Cx2tTester::WriteTime()
{
	CTemporaryCS CS(&m_reportCS);
	unsigned long time = NSTimers::GetTickCount() - m_timeStart;
	m_reportStream.WriteStringUTF8(L"Time: " + std::to_wstring(time));
}

void Cx2tTester::AddDeleteLaterFile(const std::wstring& file)
{
	m_deleteLaterFiles.push_back(file);
}
void Cx2tTester::AddDeleteLaterDirectory(const std::wstring& directory)
{
	m_deleteLaterDirectories.push_back(directory);
}

bool Cx2tTester::IsAllBusy()
{
	CTemporaryCS CS(&m_coresCS);
	return m_currentProc == m_maxProc;
}
bool Cx2tTester::IsAllFree()
{
	CTemporaryCS CS(&m_coresCS);
	return m_currentProc == 0;
}

std::vector<std::wstring> Cx2tTester::ParseExtensionsString(std::wstring extensions, const CFormatsList& fl)
{
	std::vector<std::wstring> exts;
	int pos = 0;
	while ((pos = extensions.find(' ')) != std::wstring::npos)
	{
		std::wstring ext = extensions.substr(0, pos);
		for (auto& c : ext)
			c = std::tolower(c);

		if(ext == L"documents")
			exts = fl.GetDocuments();

		else if(ext == L"presentations")
			exts = fl.GetPresentations();

		else if(ext == L"spreadsheets")
			exts = fl.GetSpreadsheets();

		else if (pos != 0)
			exts.push_back(ext);

		extensions.erase(0, pos + 1);
	}
	return exts;
}

CConverter::CConverter(Cx2tTester* internal) : m_internal(internal)
{
}
CConverter::~CConverter()
{
	Stop();
}

void CConverter::SetInputFile(const std::wstring& inputFile)
{
	m_inputFile = inputFile;
}
void CConverter::SetInputExt(const std::wstring& inputExt)
{
	m_inputExt = inputExt;
}
void CConverter::SetOutputFilesDirectory(const std::wstring& outputFilesDirectory)
{
	m_outputFilesDirectory = outputFilesDirectory;
}
void CConverter::SetOutputExts(const std::vector<std::wstring>& outputExts)
{
	m_outputExts = outputExts;
}
void CConverter::SetFontsDirectory(const std::wstring& fontsDirectory)
{
	m_fontsDirectory = fontsDirectory;
}
void CConverter::SetX2tPath(const std::wstring& x2tPath)
{
	m_x2tPath = x2tPath;
}
void CConverter::SetErrorsOnly(bool bIsErrorsOnly)
{
	m_bIsErrorsOnly = bIsErrorsOnly;
}
void CConverter::SetDeleteOk(bool bIsDeleteOk)
{
	m_bIsDeleteOk = bIsDeleteOk;
}
void CConverter::SetTrough(bool bIsTrough)
{
	m_bIsTrough = bIsTrough;
}
void CConverter::SetXmlErrorsDirectory(const std::wstring& errorsXmlDirectory)
{
	m_errorsXmlDirectory = errorsXmlDirectory;
}
void CConverter::SetCsvTxtEncoding(int csvTxtEncoding)
{
	m_csvTxtEncoding = csvTxtEncoding;
}
void CConverter::SetCsvDelimiter(const std::wstring& csvDelimiter)
{
	m_csvDelimiter = csvDelimiter;
}
void CConverter::SetPassword(const std::wstring& password)
{
	m_password = password;
}
void CConverter::SetTimeout(unsigned long timeout)
{
	m_timeout = timeout;
}
void CConverter::SetFilesCount(int totalFiles, int currFile)
{
	m_totalFiles = totalFiles;
	m_currFile = currFile;
}

void CConverter::SetSaveEnvironment(bool bSaveEnvironment)
{
	m_bSaveEnvironment = bSaveEnvironment;
}


DWORD CConverter::ThreadProc()
{
	std::vector<Cx2tTester::Report> reports;

	std::wstring input_filename = NSFile::GetFileName(m_inputFile);
	std::wstring input_ext = L'.' + NSFile::GetFileExtention(input_filename);
	std::wstring input_filename_no_ext = input_filename.substr(0, input_filename.size() - input_ext.size());

	unsigned long time_file_start = NSTimers::GetTickCount();

	bool is_all_ok = true;

#ifdef WIN32

	// x2t cannot work with normalized paths in xml
	if(m_outputFilesDirectory.find(L"\\\\?\\") == 0)
		m_outputFilesDirectory.erase(0, 4);

#endif // WIN32

	// input_format in many output exts
	for(int i = 0; i < m_outputExts.size(); i++)
	{
		std::wstring output_ext = L"."+ m_outputExts[i];
		int output_format =  m_checker.GetFormatByExtension(output_ext);

		std::wstring xml_params_filename = input_filename + L"_" + output_ext + L".xml";
		std::wstring xml_params_file = m_outputFilesDirectory + FILE_SEPARATOR_STR + xml_params_filename;

		std::wstring output_file = m_outputFilesDirectory
		        + FILE_SEPARATOR_STR + input_filename_no_ext + output_ext;

		std::wstring output_filename = NSFile::GetFileName(output_file);

		xml_params_file = CorrectPathW(xml_params_file);


		// creating temporary xml file with params
		NSStringUtils::CStringBuilder builder;
		builder.WriteString(L"<?xml version=\"1.0\" encoding=\"utf-8\"?>");

		builder.WriteString(L"<Root>");

		builder.WriteString(L"<m_sFileFrom>");
		builder.WriteEncodeXmlString(m_inputFile);
		builder.WriteString(L"</m_sFileFrom>");

		builder.WriteString(L"<m_sFileTo>");
		builder.WriteEncodeXmlString(output_file);
		builder.WriteString(L"</m_sFileTo>");

		builder.WriteString(L"<m_nFormatTo>");
		if (output_format & AVS_OFFICESTUDIO_FILE_IMAGE)
			builder.WriteString(std::to_wstring(AVS_OFFICESTUDIO_FILE_IMAGE));
		else
			builder.WriteString(std::to_wstring(output_format));
		builder.WriteString(L"</m_nFormatTo>");

		builder.WriteString(L"<m_sThemeDir>./</m_sThemeDir>");
		builder.WriteString(L"<m_bDontSaveAdditional>true</m_bDontSaveAdditional>");

		builder.WriteString(L"<m_sAllFontsPath>");
		builder.WriteEncodeXmlString(m_fontsDirectory + FILE_SEPARATOR_STR + L"AllFonts.js");
		builder.WriteString(L"</m_sAllFontsPath>");

		builder.WriteString(L"<m_sFontDir>");
		builder.WriteEncodeXmlString(m_fontsDirectory);
		builder.WriteString(L"</m_sFontDir>");

		if (output_format & AVS_OFFICESTUDIO_FILE_IMAGE)
		{
			builder.WriteString(L"<m_oThumbnail><format>");
			if (AVS_OFFICESTUDIO_FILE_IMAGE_JPG == output_format)
				builder.WriteString(L"3");
			else
				builder.WriteString(L"4");
			builder.WriteString(L"</format><aspect>2</aspect><first>false</first><zip>false</zip><width>1000</width><height>1000</height></m_oThumbnail>");
		}

		// csv & txt needs encoding param
		if(m_inputExt == L"txt" || m_inputExt == L"csv")
		{
			builder.WriteString(L"<m_nCsvTxtEncoding>");
			builder.WriteEncodeXmlString(std::to_wstring(m_csvTxtEncoding));
			builder.WriteString(L"</m_nCsvTxtEncoding>");
		}

		// csv needs delimiter param
		if(m_inputExt == L"csv")
		{
			builder.WriteString(L"<m_nCsvDelimiterChar>");
			builder.WriteEncodeXmlString(m_csvDelimiter);
			builder.WriteString(L"</m_nCsvDelimiterChar>");
		}

		// password
		if(!m_password.empty())
		{
			builder.WriteString(L"<m_sPassword>");
			builder.WriteEncodeXmlString(m_password);
			builder.WriteString(L"</m_sPassword>");
		}

		builder.WriteString(L"<m_sJsonParams>{&quot;spreadsheetLayout&quot;:{&quot;gridLines&quot;:true,&quot;headings&quot;:true,&quot;fitToHeight&quot;:1,&quot;fitToWidth&quot;:1,&quot;orientation&quot;:&quot;landscape&quot;}}</m_sJsonParams>");
		builder.WriteString(L"</Root>");

		std::wstring xml_params = builder.GetData();

		NSFile::CFileBinary o_xml_file;
		bool is_created = o_xml_file.CreateFileW(xml_params_file);
		if(!is_created)
		{
			m_internal->m_outputCS.Enter();
			std::cout << "Error creating XML conversion file!" << std::endl;
			m_internal->m_outputCS.Leave();
			continue;
		}
		o_xml_file.WriteStringUTF8(xml_params, true);
		o_xml_file.CloseFile();

#ifdef WIN32

		// x2t cannot work with normalized paths in xml
		if(xml_params_file.find(L"\\\\?\\") == 0)
			xml_params_file.erase(0, 4);

#endif // WIN32

		bool is_timeout = false;
		int exit_code = NSX2T::Convert(NSFile::GetDirectoryName(m_x2tPath), xml_params_file, m_timeout, &is_timeout, m_bSaveEnvironment);

		bool exist;
		if(output_format & AVS_OFFICESTUDIO_FILE_IMAGE)
			exist = NSDirectory::Exists(output_file);
		else
			exist = NSFile::CFileBinary::Exists(output_file);

		// is everything ok
		bool ok = !exit_code && exist && !is_timeout;

		int input_size = 0;
		int output_size = 0;

		NSFile::CFileBinary b_file;

		b_file.OpenFile(m_inputFile);
		input_size = b_file.GetFileSize();
		b_file.CloseFile();

		// get sizes
		if (ok)
		{
			if(output_format & AVS_OFFICESTUDIO_FILE_IMAGE)
			{
				int total_size = 0;
				std::vector<std::wstring> pages = NSDirectory::GetFiles(output_file);
				for(int j = 0; j < pages.size(); j++)
				{
					b_file.OpenFile(pages[j]);
					total_size += b_file.GetFileSize();
					b_file.CloseFile();
				}
				output_size = total_size;
			}
			else
			{
				b_file.OpenFile(output_file);
				output_size = b_file.GetFileSize();
				b_file.CloseFile();
			}
		}

		// save param xml of error conversion
		if(!ok)
		{
			std::wstring err_xml_file = m_errorsXmlDirectory + FILE_SEPARATOR_STR + xml_params_filename;
			err_xml_file = CorrectPathW(err_xml_file);
			NSFile::CFileBinary::SaveToFile(err_xml_file, xml_params, true);
		}

		// writing report
		if(!m_bIsErrorsOnly || !ok)
		{
			Cx2tTester::Report report;
			report.inputFile = input_filename;
			report.outputFile = output_filename;
			report.direction = m_inputExt + L"-" + output_ext.substr(1, output_ext.size() - 1);
			report.time = NSTimers::GetTickCount() - time_file_start;
			report.inputSize = input_size;
			report.outputSize = output_size;
			if(is_timeout)
				report.exitCode = L"TIMEOUT";
			else
				report.exitCode = std::to_wstring(exit_code);
			report.log = xml_params;
			reports.push_back(report);
		}

		if(ok)
			NSFile::CFileBinary::Remove(xml_params_file);

		std::string input_file_UTF8 = U_TO_UTF8(input_filename);
		std::string output_file_UTF8 = U_TO_UTF8(output_filename);

		// output_CS start
		m_internal->m_outputCS.Enter();

		std::cout << "[" << m_currFile << "/" << m_totalFiles << "](" << i + 1 << "/" << m_outputExts.size() << ") ";
		std::cout << "(" << m_internal->m_currentProc << " processes now) ";
		std::cout << input_file_UTF8 << " to " << output_file_UTF8 << " ";

		if(ok)
			std::cout << "OK";
		else
		{
			is_all_ok = false;
			std::cout << "BAD ";
			if(is_timeout)
				std::cout << "TIMEOUT";
			else if(exit_code)
				std::cout << exit_code;
			else
				std::cout << "NOT EXIST";
		}

		std::cout << std::endl;
		m_internal->m_outputCS.Leave();

		if(m_bIsDeleteOk && ok)
		{
			if(m_bIsTrough)
			{
				if(output_format & AVS_OFFICESTUDIO_FILE_IMAGE)
					m_internal->AddDeleteLaterDirectory(output_file);
				else
					m_internal->AddDeleteLaterFile(output_file);
			}
			else
			{
				if(output_format & AVS_OFFICESTUDIO_FILE_IMAGE)
					NSDirectory::DeleteDirectory(output_file);
				else
					NSFile::CFileBinary::Remove(output_file);
			}
		}
	}
	m_internal->WriteReports(reports);

	if(m_bIsDeleteOk && is_all_ok && !m_bIsTrough)
		NSDirectory::DeleteDirectory(m_outputFilesDirectory);

	CTemporaryCS CS(&m_internal->m_coresCS);
	m_internal->m_currentProc--;

	return 0;
}

CExtractor::CExtractor(Cx2tTester* internal) : m_internal(internal)
{
}
CExtractor::~CExtractor()
{
	Stop();
}

void CExtractor::SetInputFile(const std::wstring& inputFile)
{
	m_inputFile = inputFile;
}
void CExtractor::SetOutputFilesDirectory(const std::wstring& outputFilesDirectory)
{
	m_outputFilesDirectory = outputFilesDirectory;
}
void CExtractor::SetExtractExts(const std::vector<std::wstring>& extractExts)
{
	m_extractExts = extractExts;
}
void CExtractor::SetFilesCount(int totalFiles, int currFile)
{
	m_totalFiles = totalFiles;
	m_currFile = currFile;
}

DWORD CExtractor::ThreadProc()
{
	std::wstring input_filename = NSFile::GetFileName(m_inputFile);
	std::wstring input_ext = L'.' + NSFile::GetFileExtention(input_filename);
	std::wstring input_filename_no_ext = input_filename.substr(0, input_filename.size() - input_ext.size());

	for (size_t i = 0; i < m_extractExts.size(); i++)
	{
		const std::wstring& extract_ext = m_extractExts[i];
		std::wstring output_folder = m_outputFilesDirectory + FILE_SEPARATOR_STR + extract_ext;

		// output_CS start
		m_internal->m_outputCS.Enter();

		std::cout << "[" << m_currFile << "/" << m_totalFiles << "](" << i + 1 << "/" << m_extractExts.size() << ") ";
		std::cout << "(" << m_internal->m_currentProc << " processes now) ";
		std::cout << U_TO_UTF8(input_filename) << " extract " << U_TO_UTF8(extract_ext) << " ";

		std::cout << std::endl;
		m_internal->m_outputCS.Leave();

		if (NSDirectory::Exists(output_folder))
			NSDirectory::DeleteDirectory(output_folder);

		NSDirectory::CreateDirectories(output_folder);

		std::wstring temp_folder = NSDirectory::CreateDirectoryWithUniqueName(output_folder);
		m_utils.ExtractToDirectory(m_inputFile, temp_folder, nullptr, false);

		auto unzip_files = NSDirectory::GetFiles(temp_folder, true);
		bool delete_empty = true;
		for (const auto& file : unzip_files)
		{
			if (NSFile::GetFileExtention(file) == m_extractExts[i])
			{
				delete_empty = false;
				NSFile::CFileBinary::Move(file, output_folder + FILE_SEPARATOR_STR +NSFile::GetFileName(file));
			}
		}
		if (delete_empty)
			NSDirectory::DeleteDirectory(output_folder);
		NSDirectory::DeleteDirectory(temp_folder);
	}
	if (NSDirectory::GetFilesCount(m_outputFilesDirectory, true) == 0)
		NSDirectory::DeleteDirectory(m_outputFilesDirectory);

	m_internal->m_currentProc--;
	return 0;
}
