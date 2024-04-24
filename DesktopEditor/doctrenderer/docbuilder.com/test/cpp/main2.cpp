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

