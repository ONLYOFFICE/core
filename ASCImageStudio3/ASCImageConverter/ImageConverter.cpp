// AVSXMLImageConverter.cpp : Implementation of CAVSXMLImageConverter

#include "stdafx.h"
#include "atldefine.h"
#include "ImageConverter.h"
#include "ImageConverterDefines.h"


STDMETHODIMP CAVSXMLImageConverter::Start(BSTR bstrXMLString)
{
	CTemporaryCS oCS(&m_csThread);

#ifdef _DEBUG  // сохраняем входную XML в файл C:\AVSImageConverterXML.xml
	XmlUtils::CXmlWriter oWriter;
	oWriter.SetXmlString(CString(bstrXMLString));
	oWriter.SaveToFile(_T("C:\\AVSImageConverterXML.xml"), TRUE);
#endif

	//при повторном вызове полностью останавливаем предыдущий вызов
	if( IMAGECONVERTER_STATE_IDLE != m_oConvertManager.GetStatus() )
		Stop();

	//если не задано количество потоков выставляем по количеству процессоров
	if( m_oConvertManager.GetTreadCount() <= 0 )
		SetThreadType(-1);

	m_oCurError.Clear();
	//парсим xml
	XmlUtils::CXmlReader oReader;
	if( TRUE == oReader.OpenFromXmlString( CString(bstrXMLString) ))
		if( TRUE == oReader.ReadRootNode( CString(L"ImageConverter") ))
			if( TRUE == oReader.ReadNodeList( CString(L"Image") ) )
			{
				//обнуляем конвертеры
				m_oConvertManager.Clear();
				m_oConvertManager.SetLogo(m_piLogoImage);
				m_oConvertManager.SetLogoParam(m_nLogoParam);
				m_oConvertManager.SetWM( m_bAddWM );
				m_oConvertManager.SetCache( m_piFilesCache );
				//добавляем конвертерам задачи
				if( 0 != m_oConvertManager.AddTasks( oReader ) )
				{
					OnError( IMAGECONVERTER_ERROR_XML_FORMAT );
					return S_FALSE;
				}
				if( 1 == m_oConvertManager.Start() )
					OnComplete();
				return S_OK;
			}
	return S_FALSE;	
}
STDMETHODIMP CAVSXMLImageConverter::_OnTimer(void)
{
	if( true == m_oConvertManager.IsSomeChange() )
	{
		//получаем изменения
		m_oConvertManager.Lock();
		LONG nNewProcess = m_oConvertManager.GetProcess();
		CAtlArray<LONG> aErrors;
		m_oConvertManager.GetErrors( aErrors );
		CAtlArray<CErrorItem> aErrors2;
		m_oConvertManager.GetErrors2( aErrors2 );
		bool bIsComplete = m_oConvertManager.GetIsComplete();
		CAtlArray<CString> aCompleteFiles;
		m_oConvertManager.GetCompleteFiles( aCompleteFiles );
		m_oConvertManager.Free();

		//Считаем прогресс отличен от запомненного то генерим Event
		if( m_lLastProcess != nNewProcess )
		{
			m_lLastProcess = nNewProcess;
			OnProcess( nNewProcess );
		}

		for( INT32 i=0; i < (INT32)aCompleteFiles.GetCount(); i++)
		{
			CComBSTR bstrCompleteFile( aCompleteFiles[i] );
			OnCompleteFile( bstrCompleteFile.m_str );
		}

		//проверяем не произошло ли ошибок
		for( INT32 i=0; i < (INT32)aErrors.GetCount(); i++)
		{
			OnError(aErrors[i]);
			Resume();
		}
		//ошибки по которым можно получить дополнительную информацию через GetAdditionalParam
		for( INT32 i=0; i < (INT32)aErrors2.GetCount(); i++)
		{
			m_oCurError = aErrors2[i];
			OnError( aErrors2[i].nID );
			Resume();
		}

		//генерируем OnComplete
		if( true == bIsComplete )
		{
			m_oConvertManager.Clear(  );
			OnComplete();
		}
	}
	return S_OK;
}
STDMETHODIMP CAVSXMLImageConverter::Stop(void)
{
	CTemporaryCS oCS(&m_csThread);
	m_oConvertManager.Stop();
	return S_OK;
}

STDMETHODIMP CAVSXMLImageConverter::Suspend(void)
{
	CTemporaryCS oCS(&m_csThread);
	m_oConvertManager.Suspend();
	return S_OK;
}

STDMETHODIMP CAVSXMLImageConverter::Resume(void)
{
	CTemporaryCS oCS(&m_csThread);
	m_oConvertManager.Resume();
	return S_OK;
}
STDMETHODIMP CAVSXMLImageConverter::get_Status(LONG* pVal)
{
	CTemporaryCS oCS(&m_csThread);

	if( NULL == pVal )
		return S_FALSE;

	(*pVal) = m_oConvertManager.GetStatus();
	return S_OK;
}
STDMETHODIMP CAVSXMLImageConverter::SetAdditionalParam(BSTR ParamName, VARIANT ParamValue)
{
	CTemporaryCS oCS(&m_csThread);

	CString sParamNameStr ( ParamName);
	sParamNameStr.MakeLower();

	if( sParamNameStr == _T("logo") )
	{
		IUnknown* piUnkInput = ParamValue.punkVal;
		RELEASEINTERFACE( m_piLogoImage );
		if(NULL != piUnkInput)
		{
			HRESULT hRes = piUnkInput->QueryInterface( MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&m_piLogoImage );
			if( false == SUCCEEDED(hRes ) || NULL == m_piLogoImage)
			{
				OnError( IMAGECONVERTER_ERROR_INPUT_DATA );
				return S_FALSE;
			}
		}
	}
	else if(sParamNameStr == _T("logoparams"))
	{
		m_nLogoParam = ParamValue.intVal;
		m_oConvertManager.SetLogoParam( m_nLogoParam );
	}
	else if( sParamNameStr == _T("unregistered") )
	{
		if( VARIANT_TRUE == ParamValue.boolVal )
			m_bAddWM = true;
		else 
			m_bAddWM = false;
		m_oConvertManager.SetWM( m_bAddWM );
	}
	else if( sParamNameStr == CString("oldphotomasksfolder") )
	{
		m_piImageSource->SetAdditionalParam(ParamName, ParamValue);
		m_oConvertManager.SetAdditionalParam( ParamName, ParamValue.bstrVal );
	}

	return S_OK;
}
STDMETHODIMP CAVSXMLImageConverter::GetAdditionalParam(BSTR ParamName, VARIANT* ParamValue)
{
	CTemporaryCS oCS(&m_csThread);
	CString sParamNameStr ( ParamName);
	sParamNameStr.MakeLower();

	if(sParamNameStr == _T("logo"))
	{
		IUnknown* piUnkOutput = NULL;
		if( NULL != m_piLogoImage)
		{
			m_piLogoImage->QueryInterface( IID_IUnknown ,  (void**)&piUnkOutput);
			ParamValue->vt = VT_UNKNOWN;
			ParamValue->punkVal = piUnkOutput;
		}
	}
	else if(sParamNameStr == _T("logoparams"))
	{
		ParamValue->vt = VT_INT;
		ParamValue->intVal = m_nLogoParam;
	}
	else if( sParamNameStr == _T("unregistered") )
	{
		ParamValue->vt = VT_BOOL;
		if( true == m_bAddWM)
			ParamValue->intVal = VARIANT_TRUE;
		else
			ParamValue->intVal = VARIANT_FALSE;
	}
	else if( sParamNameStr == _T("errorfilename") )
	{
		ParamValue->vt = VT_BSTR;
		ParamValue->bstrVal = m_oCurError.sFilename.AllocSysString();
	}
	else if( sParamNameStr == _T("sourcescomplete") )
	{
		ParamValue->vt = VT_I4;
		ParamValue->lVal = m_oCurError.nSourcesComplete;
	}
	else if( sParamNameStr == _T("sourcesall") )
	{
		ParamValue->vt = VT_I4;
		ParamValue->lVal = m_oCurError.nSourcesAll;
	}
	return S_OK;
}
STDMETHODIMP CAVSXMLImageConverter::GetCurrentPicture( LONG nWidth, LONG nHeight, IUnknown** ppVal )
{
	CTemporaryCS oCS(&m_csThread);
	if( NULL == ppVal )
		return S_FALSE;
	(*ppVal) = NULL;

	CString sCurImage = m_oConvertManager.GetCurImageName();
	if( _T("") != sCurImage )
	{
		BSTR bstrFile =  sCurImage.AllocSysString();
		HRESULT hRes = m_piImageSource->LoadImageW( bstrFile, nWidth, nHeight, ppVal );
		SysFreeString( bstrFile );
		return hRes;
	}
	return S_FALSE;
}
STDMETHODIMP CAVSXMLImageConverter::GetEstimatedTime(FLOAT* pVal)
{
	CTemporaryCS oCS(&m_csThread);

	if(NULL != pVal)
	{
		(*pVal) = (FLOAT)m_oConvertManager.GetEstimatedTime();
		return S_OK;
	}
	else
		return S_FALSE;
}
STDMETHODIMP CAVSXMLImageConverter::GetElapsedTime(FLOAT* pVal)
{
	CTemporaryCS oCS(&m_csThread);
	if(NULL != pVal)
	{
		(*pVal) = (FLOAT)m_oConvertManager.GetElapsedTime();
		return S_OK;
	}
	else
		return S_FALSE;
}

STDMETHODIMP CAVSXMLImageConverter::SetThreadType(LONG NewVal)
{
	CTemporaryCS oCS(&m_csThread);
	SYSTEM_INFO siSysInfo;
	DWORD dwProcNum;
	INT32 nThreadCount;
	switch( NewVal )
	{
		case -1:
				GetSystemInfo( &siSysInfo ); 
				nThreadCount = 3 * siSysInfo.dwNumberOfProcessors / 2;
				break;
		case 0:
				nThreadCount = 1;
				break;
		case 1:
				nThreadCount = 2;
				GetSystemInfo( &siSysInfo ); 
				dwProcNum = 3 * siSysInfo.dwNumberOfProcessors / 2;
				if( (INT32)dwProcNum > nThreadCount)
					nThreadCount = dwProcNum;
				break;
		default:
				GetSystemInfo( &siSysInfo ); 
				nThreadCount = 3 * siSysInfo.dwNumberOfProcessors / 2;
				break;
	}
	m_oConvertManager.SetTreadCount( nThreadCount );
	return S_OK;
}
STDMETHODIMP CAVSXMLImageConverter::SetThreadPriority(LONG NewVal)
{
	CTemporaryCS oCS(&m_csThread);
	m_oConvertManager.SetThreadPriority( NewVal );
	return S_OK;
}
STDMETHODIMP CAVSXMLImageConverter::get_FilesCache(IUnknown** pVal)
{
	if(  NULL == pVal )
		return S_FALSE;
	(*pVal) = NULL;
	if( NULL == m_piFilesCache )
		return S_OK;

	return m_piFilesCache->QueryInterface( IID_IUnknown , (void**)pVal );
}
STDMETHODIMP CAVSXMLImageConverter::put_FilesCache(IUnknown* newVal)
{
	CTemporaryCS oCS(&m_csThread);

	RELEASEINTERFACE(m_piFilesCache);
	if(NULL != newVal)
	{
		ImageStudio::IAVSImageCache* piFileCache = NULL;
		HRESULT hRes = newVal->QueryInterface( ImageStudio::IID_IAVSImageCache, (void**)&piFileCache );
		if( SUCCEEDED(hRes) &&  NULL != piFileCache )
			m_piFilesCache = piFileCache;
		else
			return S_FALSE;
	}
	return S_OK;
}