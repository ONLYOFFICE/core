#pragma once

#include <windows.h> 

#define __WINDOWS__
#define _Windows

#include "PostScriptModule.h"
#include <string>
#include <strstream>
#include <list>

//! AVSPostScript Wrapper

namespace PS
{

	class CSettings
	{
	private:
		CSettings(const CSettings&);
		CSettings& operator=(const CSettings&);
	public:
		CSettings()
		{
			m_arParams.RemoveAll();
		};

		virtual ~CSettings()
		{
			m_arParams.RemoveAll();
		};

		static enum EDevice
		{
			 PDFWriter = 0,
			 PNGWriter = 1,
			 JPEGWriter = 2,
			 //BMPWriter = 3,
			 //PCXWriter = 4,
			 //TIFFWriter = 5,
			 //PSWriter = 6,
			 //EPSWriter = 7,
			 //TXTWriter = 8,
			 //PDFAWriter = 9,
			 //PDFXWriter = 10,
			 //PSDWriter = 11,
			 //PCLWriter = 12,
			 //RAWWriter = 13,
			 //SVGWriter = 14,
		};

		void SetDevice(EDevice eDevice)	
		{	
			m_eDevice = eDevice;
			UpdateParams();
		};
		void SetOutputFile(CString sOutputFile)
		{	
			m_sOutputFile = sOutputFile;
			m_sOutputFile.Replace("%", "%%");
			UpdateParams();
		};


	public:

		CSimpleArray<CString> GetParams()
		{
			return m_arParams;
		}

		int GetParamsCount()
		{
			return m_arParams.GetSize();
		}

		void UpdateParams()
		{
			switch( m_eDevice )
			{
			case PDFWriter:
				PDFParams();
				break;
			case PNGWriter:
				PNGParams();
				break;
			case JPEGWriter:
				JPEGParams();
				break;
			//case BMPWriter:
			//	break;
			//case PCXWriter:
			//	break;
			//case TIFFWriter:
			//	break;
			//case PSWriter:
			//	break;
			//case EPSWriter:
			//	break;
			//case PDFAWriter:
			//	break;
			//case PDFXWriter:
			//	break;
			//case PSDWriter:
			//	break;
			//case PCLWriter:
			//	break;
			//case RAWWriter:
			//	break;
			//case SVGWriter:
			//	break;
			}
		}

		void PDFParams()
		{
			m_arParams.RemoveAll();
			m_arParams.Add("");
			m_arParams.Add("-q");
			m_arParams.Add("-dNOPAUSE");
			m_arParams.Add("-dBATCH");
			CString sFont = "-sFONTPATH=";
				sFont += GetFontsPath();
			m_arParams.Add(sFont);
			m_arParams.Add("-sDEVICE=pdfwrite");
			m_arParams.Add("-dPDFSETTINGS=/default");
			m_arParams.Add("-dCompatibilityLevel=1.4");
			m_arParams.Add("-r600x600");
			m_arParams.Add("-dProcessColorModel=/DeviceCMYK");
			m_arParams.Add("-dAutoRotatePages=/PageByPage");
			m_arParams.Add("-dCompressPages=true");
			m_arParams.Add("-dEmbedAllFonts=true");
			m_arParams.Add("-dSubsetFonts=true");
			m_arParams.Add("-dMaxSubsetPct=100");
			m_arParams.Add("-dConvertCMYKImagesToRGB=false");
			CString sOutput = "-sOutputFile=";
			sOutput += m_sOutputFile;
			m_arParams.Add(sOutput.GetBuffer());
			m_arParams.Add("-dEncodeColorImages=true");
			m_arParams.Add("-dAutoFilterColorImages=true");
			m_arParams.Add("-dEncodeGrayImages=true");
			m_arParams.Add("-dAutoFilterGrayImages=true");
			m_arParams.Add("-dEncodeMonoImages=true");
			m_arParams.Add("-dMonoImageFilter=/CCITTFaxEncode");
			m_arParams.Add("-dDownsampleMonoImages=false");
			m_arParams.Add("-dPreserveOverprintSettings=true");
			m_arParams.Add("-dUCRandBGInfo=/Preserve");
			m_arParams.Add("-dUseFlateCompression=true");
			m_arParams.Add("-dParseDSCCommentsForDocInfo=true");
			m_arParams.Add("-dParseDSCComments=true");
			m_arParams.Add("-dOPM=0");
			m_arParams.Add("-dOffOptimizations=0");
			m_arParams.Add("-dLockDistillerParams=false");
			m_arParams.Add("-dGrayImageDepth=-1");
			m_arParams.Add("-dASCII85EncodePages=false");
			m_arParams.Add("-dDefaultRenderingIntent=/Default");
			m_arParams.Add("-dTransferFunctionInfo=/Preserve");
			m_arParams.Add("-dPreserveHalftoneInfo=false");
			m_arParams.Add("-dDetectBlends=true");

		}


		void PNGParams()
		{
			m_arParams.RemoveAll();
			m_arParams.Add("");
			m_arParams.Add("-q");
			m_arParams.Add("-dNOPAUSE");
			m_arParams.Add("-dBATCH");
			CString sFont = "-sFONTPATH=";
				sFont += GetFontsPath();
			m_arParams.Add(sFont);
			m_arParams.Add("-sDEVICE=png16m");
			m_arParams.Add("-r150x150");
			CString sOutput = "-sOutputFile=";
			sOutput += m_sOutputFile;
			sOutput.Replace(".", "%d.");
			m_arParams.Add(sOutput.GetBuffer());
		}
		void JPEGParams()
		{
			m_arParams.RemoveAll();
			m_arParams.Add("");
			m_arParams.Add("-q");
			m_arParams.Add("-dNOPAUSE");
			m_arParams.Add("-dBATCH");
			CString sFont = "-sFONTPATH=";
				sFont += GetFontsPath();
			m_arParams.Add(sFont);
			m_arParams.Add("-sDEVICE=jpeg");
			m_arParams.Add("-dJPEGQ=90");
			m_arParams.Add("-r150x150");
			CString sOutput = "-sOutputFile=";
			sOutput += m_sOutputFile;
			sOutput.Replace(".", "%d.");
			m_arParams.Add(sOutput.GetBuffer());
		}

	protected:

		CString GetFontsPath()
		{

			CRegistry oReg;
			oReg.SetHKEY(HKEY_CURRENT_USER);
			oReg.SetKeyRoot("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer");
			oReg.SetSubkey("Shell Folders");
			TValueInfo oItem = oReg.GetRegistryValue("Fonts");
			CString sFontsPath((LPSTR)oItem.pData);

			return sFontsPath;
		}

	protected:

		CSimpleArray<CString> m_arParams;

		EDevice               m_eDevice;
		CString               m_sOutputFile;

	};



	/*! \brief Dynamic wrapper for the AVSPostScript.dll

	\par Initialization

	\par Sending scripts

	\par WARNING
	Only 1 instance of AVSPostScript is authorized.

	*/
	class CAPI : public CModule
	{
	public:

		CAPI(CSettings &oSettings)
		:
		m_Instance(NULL),
		m_NewInstance(NULL),
		m_DeleteInstance(NULL),
		m_Exit(NULL),
		m_InitWithArgs(NULL),
		m_bInitSuccess(false),
		m_RunFile(NULL)
		{
			if ( m_bAlreadyInstance )
				return;

			// загружаем библиотеку AVSPostSrcipt.dll
			HMODULE hModule = GetModule();
			if ( !hModule )
				return;

			// Вытаскиваем указатели на функции

			m_NewInstance    = ( int  (__stdcall*)(MainInstance**, void*))                  GetProcAddress( hModule, "NewInstance");
			m_DeleteInstance = ( void (__stdcall*)(MainInstance*))                          GetProcAddress( hModule, "DeleteInstance");
			m_InitWithArgs   = ( int  (__stdcall*)(MainInstance*, int, char **))            GetProcAddress( hModule, "InitWithArgs");
			m_RunFile        = ( int  (__stdcall*)(MainInstance*, const char*, int , int *))GetProcAddress( hModule, "RunFile");
			m_Exit           = ( int  (__stdcall*)(MainInstance *))                         GetProcAddress( hModule, "Exit");

			if ( !m_NewInstance || !m_DeleteInstance || !m_InitWithArgs || !m_RunFile || !m_Exit)
				return;

			m_NewInstance( &m_Instance, NULL);

			if ( !m_Instance )
				return;

			m_bAlreadyInstance=true;

			// лучше запускать файлы .ps через m_RunFile, а не через параметры
			// в m_InitWithArgs

			int nArgc = oSettings.GetParamsCount();
			char **ppParams;
			ppParams = new char*[nArgc];
			CSimpleArray<CString> arParams = oSettings.GetParams();
			for ( int nIndex = 0; nIndex < nArgc; nIndex++ )
				ppParams[nIndex] = arParams[nIndex].GetBuffer();

			m_iLastStatus = m_InitWithArgs( m_Instance, nArgc, (char**)ppParams);
			m_bInitSuccess = ( m_iLastStatus == 0 );

			delete ppParams;
		}


		virtual ~CAPI()
		{			
			if ( m_bInitSuccess && m_Exit)
				m_Exit( m_Instance);

			if ( m_DeleteInstance)
				m_DeleteInstance( m_Instance);

			m_bAlreadyInstance = false;
			//! returns true if dll was not loaded with success
		}
		bool IsInvalid() const
		{
			return !IsDllLoaded() || !m_Instance || m_iLastStatus <= -100  || !m_bInitSuccess;
		}


		int RunFile(CString sFileName)
		{
			USES_CONVERSION;

			if (IsInvalid())
				return -1;

			char *buf = sFileName.GetBuffer();
			return m_iLastStatus = m_RunFile( m_Instance, T2CA(buf), 0, &m_iExitCode);
		};

	protected:

		HMODULE       m_hModule;
		MainInstance* m_Instance;
		int           m_iLastStatus;
		int           m_iExitCode;

	private:
		//! true if already loaded
		static bool m_bAlreadyInstance;
		//! True if construction successfull
		bool        m_bInitSuccess;

		
		/*! \brief 
		*  Создание основного объекта для работы с PS файлами.
		*  Этот объект передается остальным API - функциям 
		
		*  WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING

		*  AVSPostScript.dll поддерживает только один объект.
		*  Контролирует присутствие объекта переменная m_bAlreadyInstance

		*  WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
		*/
		int  (__stdcall* m_NewInstance)(MainInstance**, void*);

		/*! \brief  
		*  Удаление основного объекта. До вызова этой функции
		*  нужно закончить работу, с AVSPostSrcipt вызвав функции Exit()
		*/
		void (__stdcall* m_DeleteInstance)(MainInstance*);
				
		/*! \brief Инициализация.
		* Если все хорошо, то возвращаемое значение 0. Дальше можно  
		* вызывать функцию RunFile() и\или заканчивать работу функцией 
		* Exit().
		*/
		int  (__stdcall* m_InitWithArgs)(MainInstance *, int, char **);
		
		/*! \brief 
		* Если функция вернула значение <= -100, значит либо произошел выход, либо 
		* возникла ошибка. Следует вызвать функцию Exit() в данном случае.
		*/
		
		int  (__stdcall* m_RunFile)(MainInstance*, const char*, int , int *);
				
		/*! \brief  Выход.
		* Эту функцию нужно вызвать при окончании работы, если была вызвана функция 
		* InitWithArgs(), и ее нужно вызывать до вызова функции DeleteInstance().
		*/
		int (__stdcall* m_Exit)(MainInstance *);
	};

	bool CAPI::m_bAlreadyInstance = false;

};
