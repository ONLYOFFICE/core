/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

#include "../../../../Common/3dParty/misc/proclimits.h"
#include "../../../../DesktopEditor/common/StringExt.h"
using namespace std;

int g_nFailedChecks = 0;

void check(const std::wstring& sBytes, bool bExpectedRes, long long nExpected)
{
	long long nBytes = 0;
	bool res = NSStringExt::FromHumanReadableByteCount(sBytes, nBytes);
	bool bEqual = (bExpectedRes == res && nExpected == nBytes);
	if (!bEqual)
		++g_nFailedChecks;
	wcout<<L"res="<<res<<L";equal="<<bEqual<<L";sBytes="<<sBytes<<L";nBytes="<<nBytes<< endl;
}

void test()
{
	check(L"1KB", true, 1000);
	check(L"1MB", true, 1000000);
	check(L"1KiB", true, 1024);
	check(L"11kb", true, 11000);
	check(L"4GiB", true, 4294967296LL);
	check(L"-1 kB ", true, -1000);
	check(L"4294967296", true, 4294967296LL);
	check(L"4608MiB", true, 4831838208LL);
	check(L"4.5GiB", true, 4831838208LL);
	check(L"4.5000000000000GiB", true, 4831838208LL);
	check(L"0.5KiB", true, 512);
	check(L".5GiB", true, 536870912LL);
	check(L"4.GiB", true, 4294967296LL);
	check(L"1.5 kB", true, 1500);
	check(L"-0.5KiB", true, -512);
	check(L"\n-0.5GiB", true, -536870912LL);
	check(L"0.000000001TiB", true, 1099);
	check(L"0.9999999999GiB", true, 1073741823LL);
	check(L"0.0000000009TiB", true, 989);
	check(L"1PiB", true, 1125899906842624LL);
	check(L"1EiB", true, 1152921504606846976LL);
	check(L"9007199254740993", true, 9007199254740992LL);
	check(L"4,5GiB", false, 0);
	check(L"4.5XiB", false, 0);
	check(L"1.2.3GiB", false, 0);
	check(L"9223372036854775807", false, 0);
	check(L"-9223372036854775808", false, 0);
	check(L"8EiB", false, 0);
	check(L"8YiB", false, 0);
	check(L"", false, 0);
	wcout<<L"failed="<<g_nFailedChecks<<endl;
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
