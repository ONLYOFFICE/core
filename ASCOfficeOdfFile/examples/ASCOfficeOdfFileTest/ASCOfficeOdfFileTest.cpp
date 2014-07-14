// ASCOfficeOdfFileTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <boost/timer.hpp>
#include <iostream>
#include <string>

#import "../../../Redist/ASCOfficeOdfFile.dll" rename_namespace("ASCOfficeOdfFile"), raw_interfaces_only
#import "../../../Redist/ASCPresentationEditor.dll" rename_namespace("Editor"), raw_interfaces_only

#define HR_RET(HR) if FAILED(hr = (HR)) { _ASSERTE(false); return -1; }

class CCallback : public ASCOfficeOdfFile::_IAVSOfficeFileTemplateEvents  
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
		if ((iid == __uuidof(ASCOfficeOdfFile::_IAVSOfficeFileTemplateEvents)) ||
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
	ATL::CComPtr<ASCOfficeOdfFile::IAVSOfficeFileTemplate> officeOdfFile;
    HRESULT hr;
    HR_RET(officeOdfFile.CoCreateInstance(__uuidof(ASCOfficeOdfFile::COfficeOdfFile)));
//_______________________________________________________________________________________________________
		IUnknown *pUnk;
		IConnectionPointContainer* pContainer;
		IConnectionPoint* pCP;
		CCallback *pEvents = NULL;
		DWORD dwAdvise=0;

		pEvents = new CCallback;
		pEvents->AddRef();

		HR_RET(officeOdfFile->QueryInterface(IID_IConnectionPointContainer, (void**)&pContainer));
		HR_RET(pContainer->FindConnectionPoint(__uuidof(ASCOfficeOdfFile::_IAVSOfficeFileTemplateEvents),&pCP));	

		HR_RET(pEvents->QueryInterface(IID_IUnknown,(VOID **)&pUnk));
		HR_RET(pCP->Advise(pUnk,&dwAdvise));

		pContainer->Release();pContainer=NULL;
		pUnk->Release(); pUnk=NULL;
//____________
    boost::timer t1;
    officeOdfFile->LoadFromFile(ATL::CComBSTR(argv[1]), ATL::CComBSTR(argv[2]), NULL);
    std::cout << "\n\nTime : " << t1.elapsed() << "\n";    
    return 0;
}

int PresentationEditor(int argc, _TCHAR* argv[])
{
	ATL::CComPtr<Editor::IAVSPresentationEditor> officeEditor;
    HRESULT hr;
	HR_RET(officeEditor.CoCreateInstance(__uuidof(Editor::CAVSPresentationEditor)));

    boost::timer t1;
	//hr = officeEditor->ConvertToPPTY(ATL::CComBSTR(argv[1]),ATL::CComBSTR(argv[2]));
    hr = officeEditor->OpenFile(ATL::CComBSTR(argv[1]));
	officeEditor->Save(1,ATL::CComBSTR(argv[2]));
    std::cout << "\n\nTime : " << t1.elapsed() << "\n";    
    return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
    CoInitialize(NULL);
   
	//return PresentationEditor(argc, argv);
	return ConvertSingle(argc, argv);


	return 0;
}
