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
// Tokenize_Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Tokenize_Test.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <iostream>
#include <vector>
#include <assert.h>
// The one and only application object

CWinApp theApp;

using namespace std;

CStringA Tokenize (CStringA base, char* pTokens, int& iStart)
    {
        if (iStart < 0 || NULL == pTokens)
            return CStringA();



		int nSize = base.GetLength();
        int nCurrentSymbolPos = iStart;

        // iterate all the symbols from iStart pos
        while (nCurrentSymbolPos < nSize)
        {
            char nCurrentSymbol = base[nCurrentSymbolPos];
            // iterate all the tokens
            char *pCurrentTokenPtr = pTokens;
            while (*pCurrentTokenPtr != (char) 0)
            {
                if (nCurrentSymbol == *pCurrentTokenPtr)
                {
                    // found
                    int nOldStartValue = iStart;
                    iStart = nCurrentSymbolPos + 1;

					int nCharsCount = nCurrentSymbolPos - nOldStartValue;

					if (0 == nCharsCount)
					{
						// skip this char
						break;
					}

                    return base.Mid (nOldStartValue, nCurrentSymbolPos - nOldStartValue);
                }

                pCurrentTokenPtr++;
            }
            nCurrentSymbolPos++;
        }
        // return last string after last token
		int nOldStartValue = iStart;
        iStart = -1;
        return base.Mid (nOldStartValue);
    }

bool is_equal (const std::vector<std::string> &sTokenized1, const std::vector<std::string> &sTokenized2)
{
	if (sTokenized1.size() != sTokenized2.size())
		return false;

	for (int i = 0; i < sTokenized1.size(); ++i)
	{
		if (0 != sTokenized1[i].compare (sTokenized2[i]))
			return false;
	}
	return true;
}

bool tokenizeTest (char *text, char* tokens)
{
	std::vector<std::string> sTokenized1;
	std::vector<std::string> sTokenized2;

	std::cout << "text:" << text << std::endl;

	// CString
	CStringA str = text;
	int curPos = 0;

	CStringA resToken= str.Tokenize(tokens,curPos);
	while (resToken != "")
	{
	   printf ("Resulting token: %s\n", resToken.GetString());
		sTokenized1.push_back (resToken.GetString());
	   resToken = str.Tokenize(tokens, curPos);
	};	

	// our implementation
	curPos = 0;
		CStringA resToken2 = Tokenize(str, tokens,curPos);
		while (resToken2 != "")
		{
		   printf ("Resulting token: %s\n", resToken2.GetString());
			sTokenized2.push_back (resToken2.GetString());
		   resToken2 = Tokenize(str, tokens, curPos);
		};	

		// test
		return is_equal (sTokenized1, sTokenized2);
}

bool DoTokenizeTests ()
{
	bool bRes = true;
		
		bRes = tokenizeTest("qwerty!wertyui#qwertyu%qwertyuio% 123456789 &123456789%23456789#", "%#");
		assert (bRes);
		bRes = tokenizeTest("fhdgf wghfkwjfqlw;idwugyefh fweh fiuw;euwekhfiwefhkwefhilwefjp", " ");
		assert (bRes);
		bRes = tokenizeTest("1234567890--0762uijacbsdjkfhdfklghbdjhvieuybvgiueytierug", "-");
		assert (bRes);
		bRes = tokenizeTest("vjdhsfuibyc7qwybr78tr67wtcg78w6rt78tyoi8yrh7wqetrgq876rt87wq", "78");
		assert (bRes);
		bRes = tokenizeTest("bvcxnылвормвлоругпрукгшпругкрпкгшупирптлпгоькшп", "гш");
		assert (bRes);	
		bRes = tokenizeTest("bvcxn1234567890-1234567890-123456789012345678901234567890", " ");
		assert (bRes);	
		bRes = tokenizeTest("------############----------#########-------%%%%qwertyu&&&&", "-#%");
		assert (bRes);
		bRes = tokenizeTest("------############----------#########-------%%%%qwertyu&&&&", "-");
		assert (bRes);
		bRes = tokenizeTest("------############----------#########-------%%%%qwertyu&&&&", "&");
		assert (bRes);
		return bRes;
}

bool DoSysColorsTest ()
{
	bool bRes = true;

	printf ("//***************** GetSysColor values begin (Win7 x64) *****************\n");
	DWORD nValue = 0;

	printf ("switch (nIndex) {\n");

	nValue = GetSysColor(COLOR_3DDKSHADOW);
	printf ("case COLOR_3DDKSHADOW: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_3DFACE);
	printf ("case COLOR_3DFACE: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_3DHIGHLIGHT);
	printf ("case COLOR_3DHIGHLIGHT: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_3DHILIGHT);
	printf ("case COLOR_3DHILIGHT: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_3DLIGHT);
	printf ("case COLOR_3DLIGHT: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_3DSHADOW);
	printf ("case COLOR_3DSHADOW: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_ACTIVEBORDER);
	printf ("case COLOR_ACTIVEBORDER: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_ACTIVECAPTION);
	printf ("case COLOR_ACTIVECAPTION: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_APPWORKSPACE);
	printf ("case COLOR_APPWORKSPACE: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_BACKGROUND);
	printf ("case COLOR_BACKGROUND: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_BTNFACE);
	printf ("case COLOR_BTNFACE: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_BTNHIGHLIGHT);
	printf ("case COLOR_BTNHIGHLIGHT: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_BTNHILIGHT);
	printf ("case COLOR_BTNHILIGHT: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_BTNSHADOW);
	printf ("case COLOR_BTNSHADOW: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_BTNTEXT);
	printf ("case COLOR_BTNTEXT: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_CAPTIONTEXT);
	printf ("case COLOR_CAPTIONTEXT: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_DESKTOP);
	printf ("case COLOR_DESKTOP: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_GRADIENTACTIVECAPTION);
	printf ("case COLOR_GRADIENTACTIVECAPTION: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_GRADIENTINACTIVECAPTION);
	printf ("case COLOR_GRADIENTINACTIVECAPTION: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_GRAYTEXT);
	printf ("case COLOR_GRAYTEXT: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_HIGHLIGHT);
	printf ("case COLOR_HIGHLIGHT: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_HIGHLIGHTTEXT);
	printf ("case COLOR_HIGHLIGHTTEXT: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_HOTLIGHT);
	printf ("case COLOR_HOTLIGHT: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_INACTIVEBORDER);
	printf ("case COLOR_INACTIVEBORDER: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_INACTIVECAPTION);
	printf ("case COLOR_INACTIVECAPTION: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_INACTIVECAPTIONTEXT);
	printf ("case COLOR_INACTIVECAPTIONTEXT: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_INFOBK);
	printf ("case COLOR_INFOBK: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_INFOTEXT);
	printf ("case COLOR_INFOTEXT: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_MENU);
	printf ("case COLOR_MENU: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_MENUHILIGHT);
	printf ("case COLOR_MENUHILIGHT: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_MENUBAR);
	printf ("case COLOR_MENUBAR: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_MENUTEXT);
	printf ("case COLOR_MENUTEXT: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_SCROLLBAR);
	printf ("case COLOR_SCROLLBAR: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_WINDOW);
	printf ("case COLOR_WINDOW: nValue = 0x%x; break;\n", nValue);

	nValue = GetSysColor(COLOR_WINDOWFRAME);
	printf ("case COLOR_WINDOWFRAME: nValue = 0x%6x; break;\n", nValue);

	nValue = GetSysColor(COLOR_WINDOWTEXT);
	printf ("case COLOR_WINDOWTEXT: nValue = 0x%x; break;\n", nValue);


	printf ("} // switch (nIndex)\n");
	printf ("//***************** GetSysColor values end *****************\n");


	return bRes;
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		//DoTokenizeTests ();
		DoSysColorsTest ();		
	}

	return nRetCode;
}



