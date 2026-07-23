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

int main(int argc, char *argv[])
{
	CoInitialize(NULL);

	IONLYOFFICEDocBuilder* oBuilder = NULL;
	IONLYOFFICEDocBuilderContext* oContext = NULL;
	IONLYOFFICEDocBuilderContextScope* oScope = NULL;

	IONLYOFFICEDocBuilderValue* oGlobal = NULL;
	IONLYOFFICEDocBuilderValue* oApi = NULL;
	IONLYOFFICEDocBuilderValue* oDocument = NULL;
	IONLYOFFICEDocBuilderValue* oParagraph = NULL;
	IONLYOFFICEDocBuilderValue* oContent = NULL;

	HRESULT hr = CoCreateInstance(__uuidof(CONLYOFFICEDocBuilder), NULL, CLSCTX_ALL, __uuidof(IONLYOFFICEDocBuilder), (void**)&oBuilder);

	if (FAILED(hr))
	{
		CoUninitialize();                                             \
		return 1;
	}

	VARIANT_BOOL b;

	oBuilder->Initialize();
	oBuilder->CreateFile(_bstr_t("docx"), &b);
	oBuilder->GetContext(&oContext);

	oContext->CreateScope(&oScope);
	oContext->GetGlobal(&oGlobal);

	oGlobal->GetProperty(_bstr_t("Api"), &oApi);
	oContext->CreateArray(1, &oContent);

	oApi->Call(_bstr_t("GetDocument"), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), &oDocument);
	oApi->Call(_bstr_t("CreateParagraph"), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), &oParagraph);
	oContext->CreateArray(1, &oContent);
	
	oParagraph->Call(_bstr_t("SetSpacingAfter"), ATL::CComVariant(1000), ATL::CComVariant(VARIANT_FALSE), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), NULL);
	oParagraph->Call(_bstr_t("AddText"), ATL::CComVariant("Hello from COM!"), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), NULL);
	oContent->Set(0, oParagraph);

	oDocument->Call(_bstr_t("InsertContent"), ATL::CComVariant(oContent), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), NULL);

	oBuilder->SaveFile(_bstr_t("docx"), _bstr_t("result.docx"), &b);

	RELEASEINTERFACE(oContent);

	IONLYOFFICEDocBuilderValue* oArr = NULL;
	CComSafeArray<BYTE> arr;
	arr.Add(1);
	oContext->CreateTypedArray(ATL::CComVariant(arr), 1, &oArr);

	oBuilder->CloseFile();
	oBuilder->Dispose();

	RELEASEINTERFACE(oBuilder);
	RELEASEINTERFACE(oContext);
	RELEASEINTERFACE(oScope);

	RELEASEINTERFACE(oGlobal);
	RELEASEINTERFACE(oApi);
	RELEASEINTERFACE(oDocument);
	RELEASEINTERFACE(oParagraph);
	RELEASEINTERFACE(oContent);

	CoUninitialize();
	return 0;
}

