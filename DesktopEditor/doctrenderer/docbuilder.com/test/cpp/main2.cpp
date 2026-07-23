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
#include <comutil.h>
#include <atlcomcli.h>
#include <atlsafe.h>
#include <string>

#include "../../src/docbuilder_midl.h"

#ifdef _UNICODE
# pragma comment(lib, "comsuppw.lib")
#else
# pragma comment(lib, "comsupp.lib")
#endif

#define RELEASEINTERFACE(pinterface)  \
{                                     \
    if (NULL != pinterface)           \
    {                                 \
        pinterface->Release();        \
        pinterface = NULL;            \
    }                                 \
}

#define EMPTY_PARAM ATL::CComVariant()
#define _B(x) _bstr_t(L##x)

int main(int argc, char *argv[])
{
	// uncomment for debug js
	//SetEnvironmentVariableA("V8_USE_INSPECTOR", "1");

	CoInitialize(NULL);
	
	IONLYOFFICEDocBuilder* pBuilder = NULL;
	if (FAILED(CoCreateInstance(__uuidof(CONLYOFFICEDocBuilder), NULL, CLSCTX_ALL, __uuidof(IONLYOFFICEDocBuilder), (void**)&pBuilder)))
	{
		CoUninitialize();
		return 1;
	}

	VARIANT_BOOL bRes;
	pBuilder->Initialize();
	pBuilder->OpenFile(_B("file.docx"), _B(""), &bRes);
	//pBuilder->SaveFile(_B("html"), _B("D:/FILES/images.html"), &bRes);

	IONLYOFFICEDocBuilderContext* pContext = NULL;
	pBuilder->GetContext(&pContext);

	IONLYOFFICEDocBuilderContextScope* pScope = NULL;
	pContext->CreateScope(&pScope);

	IONLYOFFICEDocBuilderValue* pGlobal = NULL; 
	pContext->GetGlobal(&pGlobal);

	IONLYOFFICEDocBuilderValue* pApi = NULL;
	pGlobal->GetProperty(_B("Api"), &pApi);
	IONLYOFFICEDocBuilderValue* pDocument = NULL;
	pApi->Call(_B("GetDocument"), EMPTY_PARAM, EMPTY_PARAM, EMPTY_PARAM, EMPTY_PARAM, EMPTY_PARAM, EMPTY_PARAM, &pDocument);

	IONLYOFFICEDocBuilderValue* pRanges = NULL;
	pDocument->Call(_B("Search"), ATL::CComVariant("year"), EMPTY_PARAM, EMPTY_PARAM, EMPTY_PARAM, EMPTY_PARAM, EMPTY_PARAM, &pRanges);

	if (pRanges)
	{
		VARIANT_BOOL vbIsArray = VARIANT_FALSE;
		pRanges->IsArray(&vbIsArray);

		if (VARIANT_TRUE == vbIsArray)
		{
			long nCount = 0;
			pRanges->GetLength(&nCount);

			if (0 < nCount)
			{
				IONLYOFFICEDocBuilderValue* pSearchRange = NULL;
				pRanges->Get(0, &pSearchRange);

				IONLYOFFICEDocBuilderValue* pComment = NULL;
				pSearchRange->Call(_B("AddComment"), ATL::CComVariant("Comment Text"), ATL::CComVariant("Author"), 
					EMPTY_PARAM, EMPTY_PARAM, EMPTY_PARAM, EMPTY_PARAM, &pComment);

				IONLYOFFICEDocBuilderValue* pCommentID = NULL;
				pComment->Call(_B("GetId"), EMPTY_PARAM, EMPTY_PARAM, EMPTY_PARAM, EMPTY_PARAM, EMPTY_PARAM, EMPTY_PARAM, &pCommentID);

				// get comment id. 
				// work with comment: 
				// https://api.onlyoffice.com/docbuilder/textdocumentapi/apidocument/getcommentbyid
				// https://api.onlyoffice.com/docbuilder/textdocumentapi/apicomment

				BSTR bsCommentId = NULL;
				pCommentID->ToString(&bsCommentId);
				SysFreeString(bsCommentId);

				RELEASEINTERFACE(pCommentID);
				RELEASEINTERFACE(pComment);
				RELEASEINTERFACE(pSearchRange);
			}
		}
	}

	RELEASEINTERFACE(pRanges);
	RELEASEINTERFACE(pDocument);
	RELEASEINTERFACE(pApi);	

	RELEASEINTERFACE(pGlobal);
	RELEASEINTERFACE(pScope);
	RELEASEINTERFACE(pContext);

	pBuilder->SaveFile(_B("docx"), _B("file.docx"), &bRes);

	pBuilder->CloseFile();
	pBuilder->Dispose();

	RELEASEINTERFACE(pBuilder);

	CoUninitialize();
	return 0;
}

