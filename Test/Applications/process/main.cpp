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

//#include "../../../../desktop-sdk/ChromiumBasedEditors/lib/src/cefwrapper/external_process.h"
#include "../../../../desktop-sdk/ChromiumBasedEditors/lib/src/cefwrapper/external_process_with_childs.h"

#include <iostream>

class CProcessRunnerCallbackWork : public NSProcesses::CProcessRunnerCallback
{
public:
	CProcessRunnerCallbackWork(){}
	virtual ~CProcessRunnerCallbackWork(){}

	virtual void process_callback(const int& id, const NSProcesses::StreamType& type, const std::string& message)
	{
		std::string type_out = "stop";
		switch (type)
		{
		case NSProcesses::StreamType::StdOut:
			type_out = "stdout";
			break;
		case NSProcesses::StreamType::StdErr:
			type_out = "stderr";
			break;
		case NSProcesses::StreamType::Stop:
			break;
		default:
			break;
		}

		std::cout << "[" << id << ", " << type_out << "] " << message << std::endl;
	}
};

int main()
{
	CProcessRunnerCallbackWork callback;
	NSProcesses::CProcessManager manager(&callback);

	//manager.Start("ping -c 4 google.com", {});
	//manager.Start("calc", {});

	manager.Start("docker run -i --rm -e GITHUB_PERSONAL_ACCESS_TOKEN ghcr.io/github/github-mcp-server", {
		{"GITHUB_PERSONAL_ACCESS_TOKEN", "token"}
	});

	std::this_thread::sleep_for(std::chrono::seconds(15));

	manager.StopAll();

	std::cout << "Hello World!" << std::endl;
	return 0;
}
