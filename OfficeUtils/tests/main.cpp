#include <iostream>
#include <algorithm>

#include "gtest/gtest.h"

#include "../src/OfficeUtils.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"

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

		if(!NSDirectory::Exists(unzipDirectory))
			NSDirectory::CreateDirectories(unzipDirectory);

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

TEST_F(COfficeUtilsTest, general_win)
{
	std::wstring zip_filename = L"general_win";
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

TEST_F(COfficeUtilsTest, general_win_no_folder)
{
	std::wstring zip_filename = L"general_win";
	std::wstring zip_path = zipDirectory + wsep + zip_filename;
	std::wstring unzip_folder = unzipDirectory + wsep + L"general_win_no_folder";

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




