#include <iostream>

#include "../_docbuilder.h"
#include "../_docbuilder_i.c"

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

	wchar_t param[] = L"--work-directory";
	wchar_t Api[] = L"Api";
	wchar_t GetDocument[] = L"GetDocument";
	wchar_t CreateParagraph[] = L"CreateParagraph";
	VARIANT_BOOL b;

	oBuilder->Initialize(work_dir);
	oBuilder->SetProperty(param, work_dir);
	oBuilder->CreateFileW(result_path, &b);

	//std::cout << oContext << std::endl;
	hr = oBuilder->GetContext(&oContext); // NULL

	oContext->CreateScope(&oScope);

	oContext->GetGlobal(&oGlobal);
	oGlobal->GetProperty(Api, &oApi);


	wchar_t docx[] = L".docx";
	oBuilder->SaveFile(docx, result_path, &b);
	oBuilder->CloseFile();

	oBuilder->Dispose();

	CoUninitialize();
	return 0;
}

