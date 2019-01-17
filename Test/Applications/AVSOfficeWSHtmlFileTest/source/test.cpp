/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>

static int test_num = 1;

bool CompareFiles(const _bstr_t& verified_file, const _bstr_t& verification_pattern_file)
{
	std::ifstream vf(static_cast<char*>(verified_file));
	std::ifstream vpf(static_cast<char*>(verification_pattern_file));
	if(!vf || !vpf)
	{
		return false;
	}

	size_t tag_counter = 0;
	size_t line_number = 0;
	while(!vf.eof() && ! vpf.eof())
	{
		std::string line1;
		std::string line2;
		vf >> line1;
		vpf >> line2;
		if(4 == ++tag_counter && 0 == line1.find("id="))
		{
			continue;
		}
		if(!line1.empty() && line1.at(0) == '<')
		{
			++line_number;
		}
		if(line1 != line2)
		{
			std::cout << "Error: file \"" <<  static_cast<char*>(verified_file) <<
				"\" has erroneous lines. The first difference is at line " << 
				line_number << ":" << std::endl;
			std::cout << line1 << std::endl;
			std::cout << "The pattern is:" << std::endl;
			std::cout << line2 << std::endl;
			return false;
		}
	}
	if(!vf.eof())
	{
		std::cout << "Error: file \"" <<  static_cast<char*>(verified_file) << "\" has additional lines" << std::endl;
		return false;
	}
	if(!vpf.eof())
	{
		std::cout << "Error: file \"" <<  static_cast<char*>(verified_file) << "\" has not enough data" << std::endl;
		return false;
	}
	return true;
}
bool convertXLSX2HTMLAndCompare(const _bstr_t& name_without_extension, const std::string& file_description)
{
	AVSOfficeWSHtmlFile::IWSHtmlFilePtr html_file(L"AVSOfficeWSHtmlFile.WSHtmlFile.1");
	AVSOfficeUtils::IOfficeUtilsPtr ooxArchiver(L"AVSOfficeUtils.OfficeUtils.1");
	HRESULT res;
	HRESULT res2;
	HRESULT resA;

	resA = ooxArchiver->ExtractToDirectory(L"../InFiles/" + name_without_extension + ".xlsx",
		L"../InFiles/" + name_without_extension, _bstr_t(), 0);
	res = html_file->SaveToFile(L"../OutFiles/" + name_without_extension + ".htm",
		L"../InFiles/" + name_without_extension, L"");
	res2 = html_file->SaveToFile(L"../OutFiles/" + name_without_extension + "_single.htm",
		L"../InFiles/" + name_without_extension, 
		L"<?xml version=\"1.0\" standalone=\"yes\" ?>"
		L"<Options>"
		L"  <HTML>"
		L"    <SingleFile val = \"true\"/>"
		L"  </HTML>"
		L"</Options>");
	bool result = res == S_OK && res2 == S_OK && resA == S_OK;
	if(result)
	{
		result = CompareFiles("../InFiles/" + name_without_extension + "_in.xml", "../InFiles/" + name_without_extension + "_in_verify.xml") &&
			CompareFiles("../OutFiles/" + name_without_extension + ".htm_out.xml", "../OutFiles/" + name_without_extension + ".htm_out_verify.xml");
	}

	std::cout << "Test " << test_num++ << ". XLSX -> HTML (" << file_description << ")      " << (result ? "OK" : "FAILED") << std::endl << std::endl;
	return result;
}

class ComInitializer
{
public:
	ComInitializer()
	{
		CoInitialize(NULL);
	}
	~ComInitializer()
	{
		CoUninitialize();
	}
};


int _tmain(int argc, _TCHAR* argv[])
{
	std::cout  << "EWSConverter test program. Copyright NKT, 2010" << std::endl << std::endl;

	ComInitializer com_initializer;
	try
	{
		if(argc > 1)
		{
			if(argv[1] == std::wstring(L"x2h"))
			{
				convertXLSX2HTMLAndCompare(argv[2], "Command line file \"" + 
					std::string(static_cast<char*>(_bstr_t(argv[2]))) + ".xlsx\" to HTML");
			}
		}
		else
		{
			convertXLSX2HTMLAndCompare(L"Alpha", "First try file saved in Excel2007");
		}
	}
	catch(std::exception& e)
	{
		std::cout << "Exception caught!!! Message: " << e.what() << std::endl;
	}
	return 0;
}

