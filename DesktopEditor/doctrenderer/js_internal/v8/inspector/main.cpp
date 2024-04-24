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

#include "../../js_base.h"

#include <iostream>
#include <fstream>

std::string readFileContent(const std::string& sFilename)
{
	std::ifstream oFile(sFilename);
	if (!oFile.is_open())
	{
		std:: cerr << "Could not open the file: " << sFilename << std::endl;
		return "";
	}
	std::string sContent((std::istreambuf_iterator<char>(oFile)), std::istreambuf_iterator<char>());
	return sContent;
}

using namespace NSJSBase;
int main()
{
	JSSmart<CJSContext> pContext1 = new CJSContext();
	JSSmart<CJSContext> pContext2 = new CJSContext();

	{
		CJSContextScope oScope(pContext1);

		JSSmart<CJSValue> pRet = pContext1->runScript("var special = 42;\n"
													  "special;\n");

		{
			CJSContextScope oScope(pContext2);


			JSSmart<CJSValue> pRet = pContext2->runScript(readFileContent("../example/code.js"));

			std::cout << "RESULT: ";
			if (pRet->isString())
			{
				std::cout << pRet->toStringA() << std::endl;
			}
			else
			{
				std::cout << "ERROR!" << std::endl;
			}
		}

		std::cout << "RESULT: ";
		if (pRet->isNumber())
		{
			std::cout << pRet->toInt32() << std::endl;
		}
		else
		{
			std::cout << "ERROR!" << std::endl;
		}
	}


	{
		CJSContextScope oScope(pContext1);

		JSSmart<CJSValue> pRet = pContext1->runScript("function sayHi(name) {\n"
													  "  var a = special;\n"
													  "  var msg = 'Hi, ' + name;\n"
													  "  return msg;\n"
													  "}\n"
													  "sayHi('Foo');\n");
		pContext1->runScript("sayHi('Foo 2');\n");
		std::cout << "RESULT: ";
		if (pRet->isString())
		{
			std::cout << pRet->toStringA() << std::endl;
		}
		else
		{
			std::cout << "ERROR!" << std::endl;
		}
	}

	{
		CJSContextScope oScope(pContext1);

		JSSmart<CJSObject> pGlobal = pContext1->GetGlobal();
		JSSmart<CJSValue> argv[1];
		argv[0] = pContext1->createString("Bar");
		JSSmart<CJSValue> pRet = pGlobal->call_func("sayHi", 1, argv);

		std::cout << "RESULT: ";
		if (pRet->isString())
		{
			std::cout << pRet->toStringA() << std::endl;
		}
		else
		{
			std::cout << "ERROR!" << std::endl;
		}
	}

	pContext1->Dispose();
	std::cout << "AFTER CONTEXT1 DISPOSE" << std::endl;
	pContext2->Dispose();
	std::cout << "AFTER CONTEXT2 DISPOSE" << std::endl;

	return 0;
}
