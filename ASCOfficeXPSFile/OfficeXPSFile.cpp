#include "stdafx.h"
#include "OfficeXPSFile.h"
#include "Resource.h"

CXPSFile::CXPSFile()
{
}


HRESULT CXPSFile::FinalConstruct()
{
	CString sTempPath;
	GetTempPath(4096, sTempPath.GetBuffer(4096));
	sTempPath.ReleaseBuffer();
	
	m_strTempDirectory = sTempPath;
		
	m_pDocumentRenderer = NULL;

	InitializeCriticalSection(&m_oCS);

	m_pFolder = NULL;
	m_pOfficeUtils = NULL;

	if(S_OK != CoCreateInstance( __uuidof(AVSOfficeUtils::COfficeUtils), NULL, CLSCTX_INPROC_SERVER, __uuidof(AVSOfficeUtils::IOfficeUtils),
		(void **)&(this->m_pOfficeUtils) ))
		return S_FALSE;

	return S_OK;
}


void CXPSFile::FinalRelease()
{
	RELEASEINTERFACE(m_pDocumentRenderer);

	if ( this->m_pOfficeUtils != NULL )
	{
		this->m_pOfficeUtils->Release();
		this->m_pOfficeUtils = NULL;
	}
	
	if ( this->m_pFolder != NULL )
	{
		m_pFolder->Close();
		delete m_pFolder;
		m_pFolder = NULL;
	}

	DeleteCriticalSection(&m_oCS);

	// удалить директорию!
	CStringW localTempDir = m_strTempDirectory;

	SHFILEOPSTRUCTW shfos;
	ZeroMemory(&shfos, sizeof(shfos));
	shfos.wFunc = FO_DELETE;
	CStringW _local = localTempDir + CStringW(L"*.*");
	_local.AppendChar(0);
	_local.AppendChar(0);
	shfos.pFrom = _local.GetString();
	shfos.fFlags = FOF_SILENT + FOF_NOCONFIRMATION;
	if(SHFileOperationW(&shfos) != 0)
		int i = 0;//return S_FALSE;
}


STDMETHODIMP CXPSFile::get_PagesCount(LONG* plCount)
{
	*plCount = 0;
	if(m_pFolder != NULL)
		*plCount = m_pFolder->GetPageCount();
	return S_OK;
}

STDMETHODIMP CXPSFile::GetPageInfo(int nPage, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY)
{
	int w = 0, h = 0;
	if(m_pFolder != NULL)
		m_pFolder->GetPageSize(nPage, w, h);
	*pdWidth = w * 25.4 / 96;
	*pdHeight = h * 25.4 / 96;
	*pdDpiX = 96;
	*pdDpiY = 96;
	return S_OK;
}


STDMETHODIMP CXPSFile::DrawPage(int nPage, IUnknown* punkRenderer, BOOL* pBreak)
{
	if(m_pFolder != NULL)
	{
		AVSGraphics::IAVSRenderer* pRenderer = NULL;
		punkRenderer->QueryInterface(AVSGraphics::IID_IAVSRenderer, (void**)&pRenderer);
		if(pRenderer != NULL)
		{
			m_pFolder->DrawPage(nPage, pRenderer, pBreak);
			pRenderer->Release();
		}
	}
	return S_OK;
}

STDMETHODIMP CXPSFile::LoadFromFile(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
{
	put_TempDirectory(sDstPath);

	if(m_pOfficeUtils == NULL)
		return S_FALSE;

	_bstr_t bs = sDstPath;//m_strTempDirectory;

	OfficeUtilsEvents< AVSOfficeUtils::_IAVSOfficeUtilsEvents, CXPSFile >* oEventUtils = new OfficeUtilsEvents< AVSOfficeUtils::_IAVSOfficeUtilsEvents, CXPSFile >(this);
	oEventUtils->AddRef();
	oEventUtils->Advise( m_pOfficeUtils );
	
	const HRESULT extractResult =  m_pOfficeUtils->ExtractToDirectory(sSrcFileName, bs.GetBSTR(), NULL, 0);
	
	CFile::CorrectDirectorySrc(CString(bs.GetBSTR()));

	oEventUtils->UnAdvise( m_pOfficeUtils );
	oEventUtils->Release();

	if ( extractResult != S_OK )
	{
	  return extractResult;
	}

	//_bstr_t bs = m_strTempDirectory;
	//HRESULT hr = m_pOfficeUtils->ExtractToDirectory( sSrcFileName, bs.GetBSTR(), NULL, 0);
	//if(hr != S_OK)
	//	return hr;

	if(m_pFolder != NULL)
		delete m_pFolder;
	m_pFolder = new XPS::Folder();

	CString tempStr(bs.GetBSTR());
	tempStr += _T('/');
	m_pFolder->ReadFromPath(tempStr);

	int w = 0, h = 0;
	long nPages;
	get_PagesCount(&nPages);

	SAFEARRAYBOUND rgsab;
	rgsab.lLbound	= 0;
	rgsab.cElements	= 2 * nPages;

	SAFEARRAY *psaPagesSizes = SafeArrayCreate( VT_R8, 1, &rgsab );
	double *pSizes = (double*)psaPagesSizes->pvData;

	int nWidth	= 1000, nHeight	= 1000;
	double dW_MM, dH_MM;

	for (int nPageIndex = 0; nPageIndex < nPages; ++nPageIndex, pSizes += 2)
	{
		m_pFolder->GetPageSize(nPageIndex, nWidth, nHeight);

		dW_MM = 25.4 * nWidth  / 96.0;
		dH_MM = 25.4 * nHeight / 96.0;

		pSizes[0] = dW_MM;
		pSizes[1] = dH_MM;		
	}

	IUnknown* punkInst = NULL;
	this->QueryInterface(IID_IUnknown, (void**)&punkInst);
	m_pDocumentRenderer->SetDocumentInstance( punkInst, psaPagesSizes );
	RELEASEINTERFACE(punkInst);

	SafeArrayDestroy( psaPagesSizes );
/*
	for (int page = 0; page < nPages; ++page)
	{
		m_pDocumentRenderer->NewPage();
		m_pFolder->GetPageSize(page, w, h);
		m_pDocumentRenderer->put_Width(w * 25.4 / 96);
		m_pDocumentRenderer->put_Height(h * 25.4 / 96);

		IUnknown* punkInst = NULL;
		this->QueryInterface(IID_IUnknown, (void**)&punkInst);

		m_pDocumentRenderer->SetPageInstance(punkInst);

		RELEASEINTERFACE(punkInst);
		m_pDocumentRenderer->EndCommand(0x0001);
	}
*/
	RELEASEINTERFACE(m_pDocumentRenderer);

	return S_OK;
}


STDMETHODIMP CXPSFile::SaveToFile(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
{
	return S_OK;
}