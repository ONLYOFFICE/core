#include <iostream>

#include "../src/OfficeUtils.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/xml/include/xmlutils.h"
#include "../../DesktopEditor/common/StringBuilder.h"

int main(int argc, char *argv[])
{
	std::wstring sep;
#ifdef WIN32
	sep = L"\\";
#else
	sep = L"/";
#endif // WIN32

	COfficeUtils utils;
	std::wstring curr_dir = NSFile::GetProcessDirectory();
	std::wstring output_dir = curr_dir + sep + L"output";
	std::wstring input_dir = curr_dir + sep + L"input";
	std::wstring temp_dir = curr_dir + sep + L"temp";

	if(NSDirectory::Exists(input_dir))
		NSDirectory::DeleteDirectory(input_dir);
	NSDirectory::CreateDirectory(input_dir);

	if(NSDirectory::Exists(output_dir))
		NSDirectory::DeleteDirectory(output_dir);
	NSDirectory::CreateDirectory(output_dir);

	if(NSDirectory::Exists(temp_dir))
		NSDirectory::DeleteDirectory(temp_dir);
	NSDirectory::CreateDirectory(temp_dir);

	std::vector<std::wstring> tests;
	tests.push_back(L"qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890");
	tests.push_back(L"-_=+[{]};\',.`~!@#$%^&()!№;%()");
	tests.push_back(L"こんにちは世界 مرحبا بالعالم");
	tests.push_back(L"⌦ⵄ⬺▨⿐⸌⍒⿆₟⹖Ⅼ⌡┓ⓡ⣾⩬⍠⊆≆⳩≾⃉⌮⢸⠐⩏„⨑☰⟬∬⤢⹛⮚∅✢⓴✎ℙ⥅ⅱ‟⥩⩲⺐⫒Ⱌⴳ⒎⹰ⲧ⩷ₗ∸⬐⭹⓾✓⤓≪♽™⠒⌙➴⵶⡣⼷⬖⓭⒱Ȿ⹦⸪⼺⒣≕◼⾙‖");

#ifdef LINUX
	tests.push_back(L"\\\\hello\\world!\\\\");
#endif // LINUX

	std::wstring long_test;
	for(int i = 0; i < 100; i++)
		long_test += L"a";
	tests.push_back(long_test);

	for(int i = 0; i < tests.size(); i++)
	{
		while(!NSDirectory::Exists(input_dir + sep + tests[i]))
			NSDirectory::CreateDirectories(input_dir + sep + tests[i]);
		std::wstring dir = input_dir + sep + tests[i];

		std::wstring file_path = dir + sep + tests[i];
		std::wstring simple_file_path = dir + sep + L"file";

		NSFile::CFileBinary file;
		file.CreateFile(file_path);
		file.WriteStringUTF8(file_path, true);
		file.CloseFile();

		file.CreateFile(simple_file_path);
		file.WriteStringUTF8(simple_file_path, true);
		file.CloseFile();

		utils.CompressFileOrDirectory(input_dir + sep + tests[i], temp_dir + sep + tests[i] + L".zip");
	}

	std::vector<std::wstring> files = NSDirectory::GetFiles(temp_dir);

	for(auto &file : files)
	{
		std::wstring filename = NSFile::GetFileName(file);
		NSDirectory::CreateDirectory(output_dir + sep + filename);
		utils.ExtractToDirectory(file, output_dir + sep + filename, NULL, false);
	}

	return 0;
}
