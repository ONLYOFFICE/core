#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdlib>

#include "gtest/gtest.h"

#include "../src/OfficeUtils.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"

// max buffer size for reading a zip file into memory
#define MAX_SIZE 100000

testing::AssertionResult IsFilesPathsEqual(std::vector<std::wstring> files1, std::vector<std::wstring> files2)
{
	// sort
	std::sort(files1.begin(), files1.end());
	std::sort(files2.begin(), files2.end());

	// compare count
	if(files1.size() != files2.size())
		return testing::AssertionFailure() << "count of files is not equal "
										   << files1.size()
										   << " != "
										   << files2.size();

	// compare filenames
	for(size_t i = 0; i < files1.size(); ++i)
	{
		std::wstring filename1 = NSFile::GetFileName(files1[i]);
		std::wstring filename2 = NSFile::GetFileName(files2[i]);

		if(filename1 != filename2)
			return testing::AssertionFailure() << U_TO_UTF8(filename2)
											   << " is not found";
	}

	for(size_t i = 0; i < files1.size(); ++i)
	{
		if(files1[i] != files2[i])
			return testing::AssertionFailure() << U_TO_UTF8(files1[i])
											   << " is not equal "
											   << U_TO_UTF8(files2[i]);
	}
	return testing::AssertionSuccess();
}

class COfficeUtilsTest : public testing::Test
{
public:
	COfficeUtils utils;

	std::wstring workDirectory;
	std::wstring zipDirectory;
	std::wstring unzipDirectory;
	std::wstring wsep;
	std::wstring tempDirectory;

	std::vector<std::wstring> expected_general;
	std::vector<std::wstring> expected_general_no_folder;
	std::vector<std::wstring> expected_deep;
	std::vector<std::wstring> expected_docx_like;

	virtual void SetUp() override
	{

#ifdef WIN32
		wsep = L"\\";
#else
		wsep = L"/";
#endif // WIN32
		workDirectory = NSFile::GetProcessDirectory();
		unzipDirectory = workDirectory + wsep + L".." + wsep + L"unzip";
		zipDirectory = workDirectory + wsep + L".." + wsep + L"zip";
		tempDirectory = workDirectory + wsep + L".." + wsep + L"temp";

		if(!NSDirectory::Exists(unzipDirectory))
			NSDirectory::CreateDirectories(unzipDirectory);

		if(!NSDirectory::Exists(tempDirectory))
			NSDirectory::CreateDirectories(tempDirectory);

		// general
		expected_general.push_back(L"1.txt");
		expected_general.push_back(L"2.txt");
		expected_general.push_back(L"3.txt");

		expected_general.push_back(L"こんにちは世界 مرحبا بالعالم" + wsep + L"こんにちは世界 مرحبا بالعالم");
		expected_general.push_back(L"こんにちは世界 مرحبا بالعالم" + wsep + L"こんにちは世界 مرحبا بالعالم123" + wsep + L"222.txt");

		expected_general.push_back(L"НП" + wsep + L"НП" + wsep + L"NF" + wsep + L"new.txt");
		expected_general.push_back(L"MF" + wsep + L"file.txt");

		expected_general.push_back(L"⌦ⵄ⬺▨⿐⸌⍒⿆₟⹖Ⅼ⌡┓ⓡ⣾⩬⍠⊆≆⳩≾⃉⌮⢸⠐⩏„⨑☰⟬∬⤢"
						   L"⹛⮚∅✢⓴✎ℙ⥅ⅱ‟⥩⩲⺐⫒Ⱌⴳ⒎⹰ⲧ⩷ₗ∸⬐⭹⓾✓⤓≪♽™⠒⌙➴"
						   L"⵶⡣⼷⬖⓭⒱Ȿ⹦⸪⼺⒣≕◼⾙‖" + wsep + L"⌦ⵄ⬺▨⿐⸌⍒⿆₟⹖Ⅼ⌡┓");

		expected_general.push_back(L"-_=+[{]};',.`~!@#$%^&()!№;%()" + wsep + L"-_=+[{]};',.`~!@#$%^&()!№;%()");
		expected_general.push_back(L"-_=+[{]};',.`~!@#$%^&()!№;%()" + wsep + L"こんにちは世界 مرحبا بالعالم");

		// general_no_folder
		expected_general_no_folder.push_back(L"1.txt");
		expected_general_no_folder.push_back(L"2.txt");
		expected_general_no_folder.push_back(L"3.txt");

		expected_general_no_folder.push_back(L"こんにちは世界 مرحبا بالعالم");
		expected_general_no_folder.push_back(L"222.txt");

		expected_general_no_folder.push_back(L"new.txt");
		expected_general_no_folder.push_back(L"file.txt");

		expected_general_no_folder.push_back(L"⌦ⵄ⬺▨⿐⸌⍒⿆₟⹖Ⅼ⌡┓");
		expected_general_no_folder.push_back(L"-_=+[{]};',.`~!@#$%^&()!№;%()");

		// deep
		expected_deep.push_back(L"1" + wsep + L"2" + wsep + L"3" + wsep + L"4" + wsep + L"hi.txt");
		expected_deep.push_back(L"1" + wsep + L"2" + wsep + L"3" + wsep + L"4" + wsep + L"5" + wsep +
						   L"6" + wsep + L"7" + wsep + L"8" + wsep + L"9" + wsep + L"10" + wsep + L"hi.txt");

		// docx_like
		expected_docx_like.push_back(L"[Content_Types].xml");
		expected_docx_like.push_back(L"_rels" + wsep + L".rels");

		expected_docx_like.push_back(L"docProps" + wsep + L"app.xml");
		expected_docx_like.push_back(L"docProps" + wsep + L"core.xml");

		expected_docx_like.push_back(L"word" + wsep + L"document.xml");
		expected_docx_like.push_back(L"word" + wsep + L"endnotes.xml");
		expected_docx_like.push_back(L"word" + wsep + L"fontTable.xml");
		expected_docx_like.push_back(L"word" + wsep + L"footnotes.xml");
		expected_docx_like.push_back(L"word" + wsep + L"settings.xml");
		expected_docx_like.push_back(L"word" + wsep + L"webSettings.xml");
		expected_docx_like.push_back(L"word" + wsep + L"theme" + wsep + L"theme1.xml");

	}

	virtual void TearDown() override
	{
	}
};

TEST_F(COfficeUtilsTest, general_linux)
{
	std::wstring zip_filename = L"general_linux";
	std::wstring zip_path = zipDirectory + wsep + zip_filename;
	std::wstring unzip_folder = unzipDirectory + wsep + zip_filename;

	if(NSDirectory::Exists(unzip_folder))
		NSDirectory::DeleteDirectory(unzip_folder);

	NSDirectory::CreateDirectories(unzip_folder);

	HRESULT error_code = utils.ExtractToDirectory(zip_path, unzip_folder, NULL, false);
	ASSERT_EQ(error_code, S_OK);

	std::vector<std::wstring> unzip_files = NSDirectory::GetFiles(unzip_folder, true);
	std::vector<std::wstring> actual;

	for(size_t i = 0; i < unzip_files.size(); ++i)
	{
		size_t subpath_n = unzip_files[i].size() - unzip_folder.size() - 1;
		std::wstring subpath = unzip_files[i].substr(unzip_folder.size() + 1, subpath_n);
		actual.push_back(subpath);
	}

	EXPECT_TRUE(IsFilesPathsEqual(actual, expected_general));

	// data check
	std::wstring data;
	ASSERT_TRUE(NSFile::CFileBinary::ReadAllTextUtf8(unzip_folder + wsep + L"MF" + wsep + L"file.txt", data));
	EXPECT_EQ(data, L"123 321");
}

TEST_F(COfficeUtilsTest, general_linux_mem)
{
	std::wstring zip_filename = L"general_linux";
	std::wstring zip_path = zipDirectory + wsep + zip_filename;
	std::wstring unzip_folder = unzipDirectory + wsep + L"general_linux_mem";

	if(NSDirectory::Exists(unzip_folder))
		NSDirectory::DeleteDirectory(unzip_folder);

	NSDirectory::CreateDirectories(unzip_folder);

	DWORD len = 0;
	BYTE* bytes = new BYTE[MAX_SIZE];
	NSFile::CFileBinary::ReadAllBytes(zip_path, &bytes, len);

	HRESULT error_code = utils.ExtractToDirectory(bytes, len, unzip_folder, NULL, false);
	delete[] bytes;
	ASSERT_EQ(error_code, S_OK);

	std::vector<std::wstring> unzip_files = NSDirectory::GetFiles(unzip_folder, true);
	std::vector<std::wstring> actual;

	for(size_t i = 0; i < unzip_files.size(); ++i)
	{
		size_t subpath_n = unzip_files[i].size() - unzip_folder.size() - 1;
		std::wstring subpath = unzip_files[i].substr(unzip_folder.size() + 1, subpath_n);
		actual.push_back(subpath);
	}

	EXPECT_TRUE(IsFilesPathsEqual(actual, expected_general));

	// data check
	std::wstring data;
	ASSERT_TRUE(NSFile::CFileBinary::ReadAllTextUtf8(unzip_folder + wsep + L"MF" + wsep + L"file.txt", data));
	EXPECT_EQ(data, L"123 321");
}

TEST_F(COfficeUtilsTest, general_linux_no_folder)
{
	std::wstring zip_filename = L"general_linux";
	std::wstring zip_path = zipDirectory + wsep + zip_filename;
	std::wstring unzip_folder = unzipDirectory + wsep + L"general_linux_no_folder";

	if(NSDirectory::Exists(unzip_folder))
		NSDirectory::DeleteDirectory(unzip_folder);

	NSDirectory::CreateDirectories(unzip_folder);

	HRESULT error_code = utils.ExtractToDirectory(zip_path, unzip_folder, NULL, true);
	ASSERT_EQ(error_code, S_OK);

	std::vector<std::wstring> unzip_files = NSDirectory::GetFiles(unzip_folder, true);
	std::vector<std::wstring> actual;

	for(size_t i = 0; i < unzip_files.size(); ++i)
	{
		size_t subpath_n = unzip_files[i].size() - unzip_folder.size() - 1;
		std::wstring subpath = unzip_files[i].substr(unzip_folder.size() + 1, subpath_n);
		actual.push_back(subpath);
	}
	EXPECT_TRUE(IsFilesPathsEqual(actual, expected_general_no_folder));
}

TEST_F(COfficeUtilsTest, deep_win)
{
	std::wstring zip_filename = L"deep_win";
	std::wstring zip_path = zipDirectory + wsep + zip_filename;
	std::wstring unzip_folder = unzipDirectory + wsep + zip_filename;

	if(NSDirectory::Exists(unzip_folder))
		NSDirectory::DeleteDirectory(unzip_folder);

	NSDirectory::CreateDirectories(unzip_folder);

	HRESULT error_code = utils.ExtractToDirectory(zip_path, unzip_folder, NULL, false);
	ASSERT_EQ(error_code, S_OK);

	std::vector<std::wstring> unzip_files = NSDirectory::GetFiles(unzip_folder, true);
	std::vector<std::wstring> actual;

	for(size_t i = 0; i < unzip_files.size(); ++i)
	{
		size_t subpath_n = unzip_files[i].size() - unzip_folder.size() - 1;
		std::wstring subpath = unzip_files[i].substr(unzip_folder.size() + 1, subpath_n);
		actual.push_back(subpath);
	}
	EXPECT_TRUE(IsFilesPathsEqual(actual, expected_deep));
}

TEST_F(COfficeUtilsTest, deep_linux)
{
	std::wstring zip_filename = L"deep_linux";
	std::wstring zip_path = zipDirectory + wsep + zip_filename;
	std::wstring unzip_folder = unzipDirectory + wsep + zip_filename;

	if(NSDirectory::Exists(unzip_folder))
		NSDirectory::DeleteDirectory(unzip_folder);

	NSDirectory::CreateDirectories(unzip_folder);

	HRESULT error_code = utils.ExtractToDirectory(zip_path, unzip_folder, NULL, false);
	ASSERT_EQ(error_code, S_OK);

	std::vector<std::wstring> unzip_files = NSDirectory::GetFiles(unzip_folder, true);
	std::vector<std::wstring> actual;

	for(size_t i = 0; i < unzip_files.size(); ++i)
	{
		size_t subpath_n = unzip_files[i].size() - unzip_folder.size() - 1;
		std::wstring subpath = unzip_files[i].substr(unzip_folder.size() + 1, subpath_n);
		actual.push_back(subpath);
	}
	EXPECT_TRUE(IsFilesPathsEqual(actual, expected_deep));
}

TEST_F(COfficeUtilsTest, docx_like_win)
{
	std::wstring zip_filename = L"docx_like_win";
	std::wstring zip_path = zipDirectory + wsep + zip_filename;
	std::wstring unzip_folder = unzipDirectory + wsep + zip_filename;

	if(NSDirectory::Exists(unzip_folder))
		NSDirectory::DeleteDirectory(unzip_folder);

	NSDirectory::CreateDirectories(unzip_folder);

	HRESULT error_code = utils.ExtractToDirectory(zip_path, unzip_folder, NULL, false);
	ASSERT_EQ(error_code, S_OK);

	std::vector<std::wstring> unzip_files = NSDirectory::GetFiles(unzip_folder, true);
	std::vector<std::wstring> actual;

	for(size_t i = 0; i < unzip_files.size(); ++i)
	{
		size_t subpath_n = unzip_files[i].size() - unzip_folder.size() - 1;
		std::wstring subpath = unzip_files[i].substr(unzip_folder.size() + 1, subpath_n);
		actual.push_back(subpath);
	}
	EXPECT_TRUE(IsFilesPathsEqual(actual, expected_docx_like));
}

TEST_F(COfficeUtilsTest, docx_like_linux)
{
	std::wstring zip_filename = L"docx_like_linux";
	std::wstring zip_path = zipDirectory + wsep + zip_filename;
	std::wstring unzip_folder = unzipDirectory + wsep + zip_filename;

	if(NSDirectory::Exists(unzip_folder))
		NSDirectory::DeleteDirectory(unzip_folder);

	NSDirectory::CreateDirectories(unzip_folder);

	HRESULT error_code = utils.ExtractToDirectory(zip_path, unzip_folder, NULL, false);
	ASSERT_EQ(error_code, S_OK);

	std::vector<std::wstring> unzip_files = NSDirectory::GetFiles(unzip_folder, true);
	std::vector<std::wstring> actual;

	for(size_t i = 0; i < unzip_files.size(); ++i)
	{
		size_t subpath_n = unzip_files[i].size() - unzip_folder.size() - 1;
		std::wstring subpath = unzip_files[i].substr(unzip_folder.size() + 1, subpath_n);
		actual.push_back(subpath);
	}
	EXPECT_TRUE(IsFilesPathsEqual(actual, expected_docx_like));
}

TEST_F(COfficeUtilsTest, other_win)
{
	std::wstring zip_filename = L"general_linux";
	std::wstring zip_path = zipDirectory + wsep + zip_filename;

	// IsArchive(...)

	HRESULT error_code = utils.IsArchive(zip_path);
	EXPECT_EQ(error_code, S_OK);

	size_t len = 100;
	BYTE* data = new BYTE[len];
	error_code = utils.IsArchive(data, len);
	delete [] data;
	EXPECT_EQ(error_code, S_FALSE);

	// IsFileExistInArchive(...)

	error_code = utils.IsFileExistInArchive(zip_path, L"2.txt");
	EXPECT_EQ(error_code, S_OK);

	error_code = utils.IsFileExistInArchive(zip_path, L"4.txt");
	EXPECT_EQ(error_code, S_FALSE);

	// LoadFileFromArchive(...)

	data = new BYTE[100];
	ULONG nFileSize;
	error_code = utils.LoadFileFromArchive(zip_path, L"MF/file.txt", &data, nFileSize);
	std::wstring wstr = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8(data, nFileSize);
	delete[] data;

	ASSERT_EQ(error_code, S_OK);
	EXPECT_EQ(wstr, L"123 321");

	DWORD bytes_count = 0;
	BYTE* bytes = new BYTE[MAX_SIZE];
	NSFile::CFileBinary::ReadAllBytes(zip_path, &bytes, bytes_count);

	data = new BYTE[100];
	error_code = utils.LoadFileFromArchive(zip_path, L"MF/file.txt", &data, nFileSize);
	wstr = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8(data, nFileSize);
	delete[] data;
	delete[] bytes;

	ASSERT_EQ(error_code, S_OK);
	EXPECT_EQ(wstr, L"123 321");
}

TEST_F(COfficeUtilsTest, time_file)
{
	// creating file
	std::wstring filename = L"time_file_test.txt";
	std::wstring file_path = tempDirectory + wsep + filename;

	std::wstring zip_filename =  L"time_file_test.zip";
	std::wstring zip_path = tempDirectory + wsep + zip_filename;

	std::wstring unzip_folder = tempDirectory + wsep + L"time_file_test";
	std::wstring unzip_path = unzip_folder + wsep + filename;

	// folder for unzip files
	if (NSDirectory::Exists(unzip_folder))
		NSDirectory::DeleteDirectory(unzip_folder);

	NSDirectory::CreateDirectories(unzip_folder);

	// create file to zip, then unzip it
	if (NSFile::CFileBinary::Exists(file_path))
		NSFile::CFileBinary::Remove(file_path);

	NSFile::CFileBinary file;
	file.CreateFileW(file_path);
	file.WriteStringUTF8(L"some text");
	file.CloseFile();

	struct tm edit_time_before{};
	bool result_get_time_before = NSFile::CFileBinary::GetTime(file_path, &edit_time_before);
	ASSERT_EQ(result_get_time_before, true);

	HRESULT error_code = utils.CompressFileOrDirectory(file_path, zip_path);
	ASSERT_EQ(error_code, S_OK);

	error_code = utils.ExtractToDirectory(zip_path, unzip_folder, NULL, false);
	ASSERT_EQ(error_code, S_OK);

	struct tm edit_time_after{};
	bool result_get_time_after = NSFile::CFileBinary::GetTime(unzip_path, &edit_time_after);
	ASSERT_EQ(result_get_time_after, true);

	EXPECT_EQ(edit_time_before.tm_sec, edit_time_after.tm_sec);
	EXPECT_EQ(edit_time_before.tm_min, edit_time_after.tm_min);
	EXPECT_EQ(edit_time_before.tm_hour, edit_time_after.tm_hour);
	EXPECT_EQ(edit_time_before.tm_mday, edit_time_after.tm_mday);
	EXPECT_EQ(edit_time_before.tm_mon, edit_time_after.tm_mon);
	EXPECT_EQ(edit_time_before.tm_year, edit_time_after.tm_year);
}

TEST_F(COfficeUtilsTest, time_folder)
{
	std::wstring file_folder = tempDirectory + wsep + L"time_file_test_folder";

	// creating file
	std::wstring filename = L"time_file_test.txt";
	std::wstring file_path = file_folder + wsep + filename;

	std::wstring zip_filename =  L"time_file_test.zip";
	std::wstring zip_path = tempDirectory + wsep + zip_filename;

	std::wstring unzip_folder = tempDirectory + wsep + L"time_file_test";
	std::wstring unzip_path = unzip_folder + wsep + filename;

	// folder for unzip files
	if (NSDirectory::Exists(unzip_folder))
		NSDirectory::DeleteDirectory(unzip_folder);

	NSDirectory::CreateDirectories(unzip_folder);

	// folder for zip files
	if (NSDirectory::Exists(file_folder))
		NSDirectory::DeleteDirectory(file_folder);

	NSDirectory::CreateDirectories(file_folder);

	// create file to zip, then unzip it
	if (NSFile::CFileBinary::Exists(file_path))
		NSFile::CFileBinary::Remove(file_path);

	NSFile::CFileBinary file;
	file.CreateFileW(file_path);
	file.WriteStringUTF8(L"some text");
	file.CloseFile();

	struct tm edit_time_before{};
	bool result_get_time_before = NSFile::CFileBinary::GetTime(file_path, &edit_time_before);
	ASSERT_EQ(result_get_time_before, true);

	HRESULT error_code = utils.CompressFileOrDirectory(file_folder, zip_path);
	ASSERT_EQ(error_code, S_OK);

	error_code = utils.ExtractToDirectory(zip_path, unzip_folder, NULL, false);
	ASSERT_EQ(error_code, S_OK);

	struct tm edit_time_after{};
	bool result_get_time_after = NSFile::CFileBinary::GetTime(unzip_path, &edit_time_after);
	ASSERT_EQ(result_get_time_after, true);

	// the 2-second precision
	EXPECT_LE(std::abs(edit_time_before.tm_sec - edit_time_after.tm_sec), 1);

	EXPECT_EQ(edit_time_before.tm_min, edit_time_after.tm_min);
	EXPECT_EQ(edit_time_before.tm_hour, edit_time_after.tm_hour);
	EXPECT_EQ(edit_time_before.tm_mday, edit_time_after.tm_mday);
	EXPECT_EQ(edit_time_before.tm_mon, edit_time_after.tm_mon);
	EXPECT_EQ(edit_time_before.tm_year, edit_time_after.tm_year);
}

