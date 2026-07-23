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
