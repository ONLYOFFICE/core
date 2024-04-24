/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

#include "../../../../Common/3dParty/misc/proclimits.h"
#include "../../../../DesktopEditor/common/StringExt.h"
using namespace std;

void test()
{
	bool res = false;
	long long nBytes;
	std::wstring sBytes;
	sBytes = L"1KB";
	res = NSStringExt::FromHumanReadableByteCount(sBytes, nBytes);
	wcout<<L"res="<<res<<L";equal="<<(1000 == nBytes)<<L";sBytes="<<sBytes<<L";nBytes="<<nBytes<< endl;

	sBytes = L"1MB";
	res = NSStringExt::FromHumanReadableByteCount(sBytes, nBytes);
	wcout<<L"res="<<res<<L";equal="<<(1000000 == nBytes)<<L";sBytes="<<sBytes<<L";nBytes="<<nBytes<< endl;

	sBytes = L"1KiB";
	res = NSStringExt::FromHumanReadableByteCount(sBytes, nBytes);
	wcout<<L"res="<<res<<L";equal="<<(1024 == nBytes)<<L";sBytes="<<sBytes<<L";nBytes="<<nBytes<< endl;

	sBytes = L"11kb";
	res = NSStringExt::FromHumanReadableByteCount(sBytes, nBytes);
	wcout<<L"res="<<res<<L";equal="<<(11000 == nBytes)<<L";sBytes="<<sBytes<<L";nBytes="<<nBytes<< endl;

	sBytes = L"4GiB";
	res = NSStringExt::FromHumanReadableByteCount(sBytes, nBytes);
	wcout<<L"res="<<res<<L";equal="<<(4294967296 == nBytes)<<L";sBytes="<<sBytes<<L";nBytes="<<nBytes<< endl;

	sBytes = L"-1 kB ";
	res = NSStringExt::FromHumanReadableByteCount(sBytes, nBytes);
	wcout<<L"res="<<res<<L";equal="<<(-1000 == nBytes)<<L";sBytes="<<sBytes<<L";nBytes="<<nBytes<< endl;
}


int main(int argc, char* argv[])
{
	test();

	cout << "Start" << endl;
	long lMemoryLimit = 5 * 1024 * 1024;
	long lAllocSize = 1 * 1024 * 1024;
	if(argc > 1)
	{
		lMemoryLimit = atol(argv[1]) * 1024 * 1024;
	}
	if(argc > 2)
	{
		lAllocSize = atol(argv[2]) * 1024 * 1024;
	}

	if(lMemoryLimit > 0)
	{
		limit_memory(lMemoryLimit);
	}
	cout << "limit_memory:" <<lMemoryLimit<< endl;
	vector<unsigned char*> consumer;
	int index = 0;
	while (true) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		cout << "sleep_for:" << ++index<< endl;
		consumer.push_back(new unsigned char[lAllocSize]);
	}
	cout << "End" << endl;
	return 0;
}
