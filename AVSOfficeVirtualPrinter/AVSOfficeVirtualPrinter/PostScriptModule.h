// GSModule.h: interface for the CGSModule class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <tchar.h>
#include <string>
#include "../Common/Registry.h"

#ifndef _TSTLSTRING
#define _TSTLSTRING

#ifndef _UNICODE
	namespace std
	{
		typedef ios				_tios;
		typedef string			_tstring;
	}
#else
	namespace std
	{
		typedef wios			_tios;
		typedef wstring			_tstring;
	};
#endif
#endif

namespace PS
{

	/*
	Класс для загрузки AVSPostScript.dll
	*/
	class CModule  
	{
	public:

	private:

		HMODULE m_hModule;

	public:

		CModule()
		{
			m_hModule  = NULL;
		}

		virtual ~CModule()
		{
			if (m_hModule)
			{
				FreeLibrary(m_hModule);
				m_hModule = NULL;
			}
		}


		HMODULE GetModule()
		{
			if ( !m_hModule )
				LoadModule();

			return m_hModule;
		}

		bool IsDllLoaded() const
		{
			return m_hModule != NULL;
		}


	private:
		void LoadModule()
		{
			USES_CONVERSION;
			
			if (m_hModule != NULL)
				return;

			//char buf[256] = "E:\\Work\\Tests\\AVSPrinter\\AVSPrinter\\bin\\gsdll32.dll";

			CRegistry oReg;
			oReg.SetHKEY(HKEY_CLASSES_ROOT);
			oReg.SetKeyRoot("CLSID\\{09F62A45-7EF9-4861-B02C-0BC5250CFC4E}\\InprocServer32");
			TValueInfo oItem = oReg.GetRegistryValue("");
			CString sPath = "";
			if ( oItem.pData )
				sPath = CString(oItem.pData);
			LPCSTR sFullPath = (LPCSTR)sPath.GetBuffer();
			LPSTR sDrive = new char[MAX_PATH];
			LPSTR sDir = new char[MAX_PATH];
			_splitpath( sFullPath, sDrive, sDir, NULL, NULL );

			sPath = CString(sDrive) + CString(sDir) + "AVSOfficePostScript.dll";

			m_hModule = LoadLibrary((LPCSTR)sPath.GetBuffer() );

			delete []sDrive;
			delete []sDir;
		}
	};

	typedef struct MainInstance_s MainInstance;

};
