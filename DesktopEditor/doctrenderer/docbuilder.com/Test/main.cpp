#include <iostream>
#include <comutil.h>
#include <atlcomcli.h>

#include "../_docbuilder.h"
#include "../_docbuilder_i.c"

#ifdef _NATIVE_WCHAR_T_DEFINED
# ifdef _DEBUG
# pragma comment(lib, "comsuppwd.lib")
# else
# pragma comment(lib, "comsuppw.lib")
# endif
#else
# ifdef _DEBUG
# pragma comment(lib, "comsuppd.lib")
# else
# pragma comment(lib, "comsupp.lib")
# endif
#endif

int main(int argc, char *argv[])
{
	wchar_t work_dir[] = L"C:/Program Files/ONLYOFFICE/DocumentBuilder";
	wchar_t result_path[] = L"result.docx";
	CoInitialize(NULL);

	IONLYOFFICEDocBuilder* oBuilder = NULL;
	IONLYOFFICEDocBuilderContext* oContext = NULL;
	IONLYOFFICEDocBuilderContextScope* oScope = NULL;

	IONLYOFFICEDocBuilderValue* oGlobal = NULL;
	IONLYOFFICEDocBuilderValue* oApi = NULL;
	IONLYOFFICEDocBuilderValue* oDocument = NULL;
	IONLYOFFICEDocBuilderValue* oParagraph = NULL;
	IONLYOFFICEDocBuilderValue* oContent = NULL;

	HRESULT hr;

	hr = CoCreateInstance(__uuidof(CONLYOFFICEDocBuilder), NULL, CLSCTX_ALL,
		__uuidof(IONLYOFFICEDocBuilder), (void**)&oBuilder);
	
	if (FAILED(hr))
	{
		std::cout << "Failed!" << std::endl;
		exit(1);
	}

	VARIANT_BOOL b;

	oBuilder->Initialize(work_dir);
	oBuilder->SetProperty(_bstr_t("--work-directory"), work_dir);
	oBuilder->CreateFileW(result_path, &b);
	oBuilder->GetContext(&oContext);

	oContext->CreateScope(&oScope);
	oContext->GetGlobal(&oGlobal);

	oGlobal->GetProperty(_bstr_t("Api"), &oApi);
	oContext->CreateArray(1, &oContent);

	oApi->Call(_bstr_t(L"GetDocument"), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), &oDocument);
	oApi->Call(_bstr_t(L"CreateParagraph"), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), &oParagraph);
	oContext->CreateArray(1, &oContent);
	

	oParagraph->Call(_bstr_t(L"SetSpacingAfter"), ATL::CComVariant(1000), ATL::CComVariant(VARIANT_FALSE), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), NULL);
	oParagraph->Call(_bstr_t(L"AddText"), ATL::CComVariant(L"Hello from .com!"), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), NULL);
	oContent->Set(0, oParagraph);

	oDocument->Call(_bstr_t(L"InsertContent"), ATL::CComVariant(oContent), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), ATL::CComVariant(), NULL);

	oBuilder->SaveFile(_bstr_t(".docx"), result_path, &b);
	oBuilder->CloseFile();
	oBuilder->Dispose();

	CoUninitialize();
	return 0;
}

