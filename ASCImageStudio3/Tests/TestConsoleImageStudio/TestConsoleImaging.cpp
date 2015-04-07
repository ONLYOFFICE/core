#include "stdafx.h"

class CApplication
{
	ImageStudio::IImageTransforms* m_pImageStudio;

	IUnknown* m_pImageSource;
	IUnknown* m_pImageResult;
	
public:
	
	CApplication()
	{
		CoInitialize(NULL);

		m_pImageStudio = NULL;

		m_pImageSource = NULL;
		m_pImageResult = NULL;
	}
	~CApplication()
	{
		Destroy();

		CoUninitialize();
	}
	
	void Destroy()
	{
		if (m_pImageStudio != NULL)
		{
			m_pImageStudio->Release();
			m_pImageStudio = NULL;
		}

		if (m_pImageSource != NULL)
		{
			m_pImageSource->Release();
			m_pImageSource = NULL;
		}
		if (m_pImageResult != NULL)
		{
			m_pImageResult->Release();
			m_pImageResult = NULL;
		}
	}

	BOOL Create()
	{
		Destroy();

		if (S_OK != CoCreateInstance(ImageStudio::CLSID_ImageTransforms, NULL, CLSCTX_INPROC, ImageStudio::IID_IImageTransforms, (void**)&m_pImageStudio))
			m_pImageStudio = NULL;

		if (m_pImageStudio == NULL)
			return FALSE;

		return TRUE;
	}
	
	BOOL ProcessTransforms()
	{
		if (m_pImageStudio == NULL)
			return FALSE;

		// return ProcessTransform_None();
		// return ProcessTransform_Load();
		return ProcessTransform_LoadSave();
	}

	BOOL ProcessTransform_None()
	{
		_bstr_t strTransform = "";

		strTransform += "<transforms>";
		strTransform += " <ImageFile-LoadImage sourcepath=\"c:\\in.bmp\" />";
		strTransform += " <ImageTransform-AdjustBrightness level=\"100\" />";
		strTransform += " <ImageFile-SaveAsBmp destinationpath=\"c:\\out.bmp\" format=\"888\" />";
		strTransform += "</transforms>";

		/*
		strTransform += "<transforms>";
		strTransform += " <ImageFile-LoadImage sourcepath=\"c:\\in.bmp\" />";
		strTransform += " <ImageTransform-TransformResize width=\"32\" height=\"32\"  type=\"257\" backcolor=\"255\" />";
		strTransform += " <ImageFile-SaveAsBmp destinationpath=\"c:\\out.bmp\" format=\"888\" />";
		strTransform += "</transforms>";
		*/

		BSTR bstrTransform = strTransform.copy();

		VARIANT_BOOL bSuccess = VARIANT_FALSE;

		m_pImageStudio->SetXml(bstrTransform, &bSuccess);

		if (bSuccess == VARIANT_TRUE)
			m_pImageStudio->Transform(&bSuccess);
		
		SysFreeString(bstrTransform);

		return (bSuccess == VARIANT_TRUE);
	}
	BOOL ProcessTransform_Load()
	{
		_bstr_t strTransform = "";

		strTransform += "<transforms>";
		strTransform += " <ImageFile-LoadImage sourcepath=\"c:\\in.bmp\" />";
		strTransform += " <ImageTransform-TransformRotate angle=\"45\" size=\"1\" backcolor=\"255\" />";
		strTransform += " <ImageTransform-AdjustBrightness level=\"30\" />";
		strTransform += "</transforms>";

		BSTR bstrTransform = strTransform.copy();

		VARIANT_BOOL bSuccess = VARIANT_FALSE;

		m_pImageStudio->SetXml(bstrTransform, &bSuccess);

		if (bSuccess == VARIANT_TRUE)
		{
			m_pImageStudio->Transform(&bSuccess);

			if (bSuccess == VARIANT_TRUE)
			{
				VARIANT vResult;
				m_pImageStudio->GetResult(0, &vResult);
				// vResult.punkVal->Release();
			}
		}
		
		SysFreeString(bstrTransform);

		return (bSuccess == VARIANT_TRUE);
	}
	BOOL ProcessTransform_LoadSave()
	{
		_bstr_t strTransform1 = "";
		_bstr_t strTransform2 = "";

		strTransform1 += "<transforms>";
		strTransform1 += " <ImageFile-LoadImage sourcepath=\"c:\\in.bmp\" />";
		strTransform1 += " <ImageTransform-TransformRotate angle=\"45\" size=\"1\" backcolor=\"255\" />";
		strTransform1 += " <ImageTransform-AdjustBrightness level=\"30\" />";
		strTransform1 += "</transforms>";

		strTransform2 += "<transforms>";
		strTransform2 += " <ImageTransform-AdjustBrightness level=\"30\" />";
		strTransform2 += "</transforms>";

		BSTR bstrTransform1 = strTransform1.copy();
		BSTR bstrTransform2 = strTransform2.copy();

		VARIANT_BOOL bSuccess = VARIANT_FALSE;

		m_pImageStudio->SetXml(bstrTransform1, &bSuccess);
		if (bSuccess == VARIANT_TRUE)
		{
			// VARIANT vtEmpty; vtEmpty.punkVal = NULL; m_pImageStudio->SetSource(0, vtEmpty);

			m_pImageStudio->Transform(&bSuccess);
			if (bSuccess == VARIANT_TRUE)
			{
				VARIANT vResult1;
				m_pImageStudio->GetResult(0, &vResult1);
				IUnknown* pSource = vResult1.punkVal;

				m_pImageStudio->SetXml(bstrTransform2, &bSuccess);
				m_pImageStudio->SetSource(0, vResult1);

				m_pImageStudio->Transform(&bSuccess);
				if (bSuccess == VARIANT_TRUE)
				{
					VARIANT vResult2;
					m_pImageStudio->GetResult(0, &vResult2);
					IUnknown* pResult = vResult2.punkVal;

					pResult->Release();

					// TODO
					pResult->AddRef(); _RPT1(_CRT_WARN, "-- 1=%d\n", pResult->Release());

					int b = 0;
				}

				pSource->Release();
			}
		}
		
		SysFreeString(bstrTransform1);
		SysFreeString(bstrTransform2);

		return (bSuccess == VARIANT_TRUE);
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	CApplication oApplication;
	if (!oApplication.Create())
	{
		printf("Failed to create necessary ImageStudio interface\n");
		return 1;
	}

	for (int index = 0; index < 100; ++index)
	{
		if (!oApplication.ProcessTransforms())
		{
			printf("Failed to perform transforms\n");
			return 1;
		}
	}

	return 0;
}

