// ASCOfficeXlsFileTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <boost/timer.hpp>
#include <iostream>
#include <string>

#import "../win32/x64/Debug/ASCOfficeXlsFile2.dll" rename_namespace("ASCOfficeXlsFile"), raw_interfaces_only

#define HR_RET(HR) if FAILED(hr = (HR)) { _ASSERTE(false); return -1; }

class CCallback : public ASCOfficeXlsFile::_IAVSOfficeFileTemplateEvents  
{
public:

	CCallback(){m_cnt=0;}
	virtual ~CCallback(){}
    STDMETHOD(GetTypeInfoCount)(UINT*) { return E_NOTIMPL; }
    STDMETHOD(GetTypeInfo)(UINT, LCID, ITypeInfo**) { return E_NOTIMPL; }
    STDMETHOD(GetIDsOfNames)(REFIID, LPOLESTR*, UINT, LCID, DISPID*) { return E_NOTIMPL; }

    STDMETHOD(Invoke)(
		DISPID		dispIdMember, 
		REFIID		riid, 
		LCID		lcid, 
		WORD		wFlags, 
		DISPPARAMS*	pDispParams, 
		VARIANT*	pVarResult, 
		EXCEPINFO*	pExcepInfo, 
		UINT*		puArgErr) 
	{
		switch(dispIdMember)
		{
		case 1:
			std::cout << "\nPercent : " << pDispParams->rgvarg[0].lVal / 10000. << "%\n";    
			return(S_OK);
			break;
		default:
			return(E_NOTIMPL); 
		}
	}

    STDMETHOD(QueryInterface)(REFIID iid, LPVOID* ppv) 
	{ 
		if ((iid == __uuidof(ASCOfficeXlsFile::_IAVSOfficeFileTemplateEvents)) ||
        (iid == __uuidof(IDispatch)) ||
        (iid == __uuidof(IUnknown)))
            *ppv = this;
        else {
            *ppv = 0;
            return E_NOINTERFACE;
        }
        AddRef();
        return S_OK;
    }

    STDMETHOD_(ULONG,AddRef)() { 
		return InterlockedIncrement(&m_cnt); 
	}

    STDMETHOD_(ULONG,Release)() { 
	    InterlockedDecrement(&m_cnt);
		if (m_cnt!=0) return m_cnt;
		delete this;
		return 0;
    }
protected:
	LONG   m_cnt;
private:
};



int ConvertSingle(int argc, _TCHAR* argv[])
{
	ATL::CComPtr<ASCOfficeXlsFile::IAVSOfficeFileTemplate> officeXlsFile;
    HRESULT hr;
	HR_RET(officeXlsFile.CoCreateInstance(__uuidof(ASCOfficeXlsFile::COfficeXlsFile)));
//_______________________________________________________________________________________________________
		IUnknown *pUnk;
		IConnectionPointContainer* pContainer;
		IConnectionPoint* pCP;
		CCallback *pEvents = NULL;
		DWORD dwAdvise=0;

		pEvents = new CCallback;
		pEvents->AddRef();

		HR_RET(officeXlsFile->QueryInterface(IID_IConnectionPointContainer, (void**)&pContainer));
		HR_RET(pContainer->FindConnectionPoint(__uuidof(ASCOfficeXlsFile::_IAVSOfficeFileTemplateEvents),&pCP));	

		HR_RET(pEvents->QueryInterface(IID_IUnknown,(VOID **)&pUnk));
		HR_RET(pCP->Advise(pUnk,&dwAdvise));

		pContainer->Release();pContainer=NULL;
		pUnk->Release(); pUnk=NULL;
//____________
    boost::timer t1;
    officeXlsFile->LoadFromFile(ATL::CComBSTR(argv[1]), ATL::CComBSTR(argv[2]), NULL);
    std::cout << "\n\nTime : " << t1.elapsed() << "\n";    
    return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
    CoInitialize(NULL);
   
	return ConvertSingle(argc, argv);


	return 0;
}
