#pragma once
#include "Registry.h"

class CInstallPrinter
{

private:

	CSimpleArray<PORT_INFO_1>    m_arPorts;
	BYTE*                        m_pPortBuffer;

	CSimpleArray<MONITOR_INFO_1> m_arMonitors;
	BYTE*                        m_pMonitorBuffer;

	CSimpleArray<DRIVER_INFO_1>  m_arDrivers;
	BYTE*                        m_pDriverBuffer;

	CSimpleArray<PRINTER_INFO_2> m_arPrinters;
	BYTE*                        m_pPrinterBuffer;

	CString Win9xDLLName;
	CString WinNTDLLName;
	CString Win9xEnvironment;
	CString WinNTEnvironment;

	CString DLLName;
	CString Environment;


public:

	CInstallPrinter()
	{
		Win9xDLLName = "pdfcmn95.dll";
		WinNTDLLName = "pdfcmnnt.dll";
		Win9xEnvironment = "Windows 4.0";
		WinNTEnvironment = "Windows NT x86";

		m_pPortBuffer = NULL;
		GetPorts();

		m_pMonitorBuffer = NULL;
		GetMonitors();

		m_pDriverBuffer = NULL;
		GetDrivers();

		m_pPrinterBuffer = NULL;
		GetPrinters();
	};

	~CInstallPrinter()
	{
		if ( m_pPortBuffer )
			delete []m_pPortBuffer;

		if ( m_pMonitorBuffer )
			delete []m_pMonitorBuffer;

		if ( m_pDriverBuffer )
			delete []m_pDriverBuffer;

		if ( m_pPrinterBuffer )
			delete []m_pPrinterBuffer;
	};


	
	BOOL    InstallWindowsPrinter(CString sMonitorname, CString sPortname, CString sDrivername, CString sPrintername, CString sAppDir = "E:\\101")
	{
		CRegistry oReg;
		oReg.SetHKEY(HKEY_CLASSES_ROOT);
		oReg.SetKeyRoot("TypeLib\\{53AC170F-ACF5-4FA0-83FE-8FEA0D4D03DB}\\1.0\\0\\win32");
		TValueInfo oItem = oReg.GetRegistryValue("");
		CString sRedmonCommand(oItem.pData);
		
		Environment = WinNTEnvironment;
		DLLName = WinNTDLLName;
		if ( !PortIsInstalled(sPortname) )
		{
			InstallPort(sPortname, sMonitorname, sRedmonCommand, sPrintername);
			GetPorts();
		}

		if ( !MonitorIsInstalled(sMonitorname) )
		{
			InstallMonitor(sMonitorname, Environment, DLLName);
			GetMonitors();
		}
		//GetDrivers("Windows 4.0")
		GetDrivers("Windows NT x86");
		if ( !DriverIsInstalled(sDrivername) )
		{
			InstallDriver(sDrivername);
			GetDrivers("Windows NT x86");
		}
		if ( !PrinterIsInstalled(sPrintername) )
		{
			InstallPrinter(sPrintername, sDrivername, sPortname);
			GetPrinters();
		}
		
	//50310: If IsWinVista Then
	//50320: reg = New clsRegistry
	//50330: With reg
	//50340: .hkey = modApiTypesEnums.hkey.HKEY_LOCAL_MACHINE
	//50350: .KeyRoot = "SYSTEM\CurrentControlSet\Services"
	//50360: .Subkey = "Spooler"
	//50370: 'UPGRADE_WARNING: Couldn't resolve default property of object reg.GetRegistryValue(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
	//				orgValue = .GetRegistryValue("RequiredPrivileges")
	//50380: value = orgValue
	//50390: If InStr(1, Replace(value, vbNullChar, " "), "SeBackupPrivilege", CompareMethod.Text) = 0 Then
	//50400: If Asc(Mid(value, Len(value), 1)) <> 0 Then
	//50410: value = value & vbNullChar
	//50420: End If
	//50430: value = value & "SeBackupPrivilege" & vbNullChar
	//50440: End If
	//50450: If InStr(1, Replace(value, vbNullChar, " "), "SeRestorePrivilege", CompareMethod.Text) = 0 Then
	//50460: If Asc(Mid(value, Len(value), 1)) <> 0 Then
	//50470: value = value & vbNullChar
	//50480: End If
	//50490: value = value & "SeRestorePrivilege" & vbNullChar
	//50500: End If
	//50510: If orgValue <> value Then
	//50520: Call .SetRegistryValue("RequiredPrivileges", value, modApiTypesEnums.DataType.REG_MULTI_SZ)
	//50530: Debug.Print(StopService("Spooler"))
	//50540: Debug.Print(StartService("Spooler"))
	//50550: End If
	//50560: End With
	//50570: End If
		return TRUE;
	};	

	BOOL    UnInstallWindowsPrinter(CString sMonitorname, CString sPortname, CString sDrivername, CString sPrintername)
	{
		UnInstallPrinter(sPrintername);
		GetPrinters();
		UnInstallDriver(sDrivername, "Windows NT x86");
		GetDrivers();
		UnInstallPort(sPortname);
		GetPorts();
		UnInstallMonitor(sMonitorname);
		GetMonitors();
		return TRUE;
	};
	BOOL    WindowsPrinterIsInstalled(CString sMonitorname, CString sPortname, CString sDrivername, CString sPrintername)
	{
		if ( !PortIsInstalled(sPortname)     || !MonitorIsInstalled(sMonitorname) ||
			 !DriverIsInstalled(sDrivername) || !PrinterIsInstalled(sPrintername) )
			 return FALSE;
		return TRUE;
	};
	DWORD   GetPrinterJobsCount(CString sPrintername)
	{
		GetPrinters();
		int nIndex = 0;
		for ( ; nIndex < m_arPrinters.GetSize(); nIndex++ )
		{
			CString sCurPrinter(m_arPrinters[nIndex].pPrinterName);
			if ( -1 != sPrintername.Find(sCurPrinter) && -1 != sCurPrinter.Find(sPrintername) )
				break;
		}

		if ( nIndex >= m_arPrinters.GetSize() )
			return -1;

		DWORD nRes =  m_arPrinters[nIndex].cJobs;
		return nRes;
	};
	BOOL    SetPrinterOutPutFile(CString sPrintername, CString sDrivername, CString sPortname, CString sFilePath = "", DWORD dwFormat = 0)
	{
		// храним информацию в ключе pParameters
		CString sParameters = "";
		if ( sFilePath.GetLength() > 0 )
		{
			sParameters = "OutPutFormat=";

			char str[33];
			_itoa(dwFormat, str, 10);
			sParameters += str;

			sParameters += ";OutPutFile=";
			sParameters += sFilePath;
		}

		HANDLE hPrinter;

		PRINTER_DEFAULTS oPrinterDefaults;
		oPrinterDefaults.pDatatype     = NULL;
		oPrinterDefaults.pDevMode      = NULL;
		oPrinterDefaults.DesiredAccess = PRINTER_ALL_ACCESS;

		OpenPrinter((LPSTR)sPrintername.GetBuffer(), &hPrinter, &oPrinterDefaults);
		if ( !hPrinter )
			return FALSE;

		PRINTER_INFO_2A oPrinterInfo;
		oPrinterInfo.pPrinterName    = (LPSTR)sPrintername.GetBuffer();
		oPrinterInfo.pDriverName     = (LPSTR)sDrivername.GetBuffer();
		oPrinterInfo.pPrintProcessor = (LPSTR)"WinPrint";
		oPrinterInfo.pPortName       = (LPSTR)sPortname.GetBuffer();
		oPrinterInfo.pComment        = (LPSTR)"AVS Document Printer";
		oPrinterInfo.pShareName      = (LPSTR)sPrintername.GetBuffer();
		oPrinterInfo.pDatatype       = (LPSTR)"RAW";
		oPrinterInfo.Priority        = 1;
		oPrinterInfo.DefaultPriority = 1;

		if ( 1 == m_arPrinters.GetSize() )
			oPrinterInfo.Attributes = 64; // утсанавливаем как defaultprinter
		else
			oPrinterInfo.Attributes = 0;

		oPrinterInfo.pServerName         = NULL;
		oPrinterInfo.pLocation           = NULL;
		oPrinterInfo.pSepFile            = NULL;
		oPrinterInfo.pParameters         = (LPSTR)sParameters.GetBuffer();
		oPrinterInfo.pSecurityDescriptor = NULL;

		oPrinterInfo.pDevMode = NULL;
		oPrinterInfo.StartTime = 0;
		oPrinterInfo.UntilTime = 0;

		SetPrinter(hPrinter, 2, (LPBYTE)&oPrinterInfo, 0);

		ClosePrinter(hPrinter);

		return TRUE;

	};
	BOOL    CancelPrinterJob(CString sPrintername, CString sDrivername, CString sPortname)
	{
		// храним информацию в ключе pParameters
		CString sParameters = "CancelPrinterJob";

		HANDLE hPrinter;

		PRINTER_DEFAULTS oPrinterDefaults;
		oPrinterDefaults.pDatatype     = NULL;
		oPrinterDefaults.pDevMode      = NULL;
		oPrinterDefaults.DesiredAccess = PRINTER_ALL_ACCESS;

		OpenPrinter((LPSTR)sPrintername.GetBuffer(), &hPrinter, &oPrinterDefaults);
		if ( !hPrinter )
			return FALSE;

		PRINTER_INFO_2A oPrinterInfo;
		oPrinterInfo.pPrinterName    = (LPSTR)sPrintername.GetBuffer();
		oPrinterInfo.pDriverName     = (LPSTR)sDrivername.GetBuffer();
		oPrinterInfo.pPrintProcessor = (LPSTR)"WinPrint";
		oPrinterInfo.pPortName       = (LPSTR)sPortname.GetBuffer();
		oPrinterInfo.pComment        = (LPSTR)"AVS Document Printer";
		oPrinterInfo.pShareName      = (LPSTR)sPrintername.GetBuffer();
		oPrinterInfo.pDatatype       = (LPSTR)"RAW";
		oPrinterInfo.Priority        = 1;
		oPrinterInfo.DefaultPriority = 1;

		if ( 1 == m_arPrinters.GetSize() )
			oPrinterInfo.Attributes = 64; // утсанавливаем как defaultprinter
		else
			oPrinterInfo.Attributes = 0;

		oPrinterInfo.pServerName         = NULL;
		oPrinterInfo.pLocation           = NULL;
		oPrinterInfo.pSepFile            = NULL;
		oPrinterInfo.pParameters         = (LPSTR)sParameters.GetBuffer();
		oPrinterInfo.pSecurityDescriptor = NULL;

		oPrinterInfo.pDevMode = NULL;
		oPrinterInfo.StartTime = 0;
		oPrinterInfo.UntilTime = 0;

		SetPrinter(hPrinter, 2, (LPBYTE)&oPrinterInfo, 0);

		ClosePrinter(hPrinter);

		return TRUE;

	};
	CString GetPrinterOutPutFile(CString sPrintername, DWORD *nOutputFormat)
	{
		CString sResult = "";
		GetPrinters();
		int nIndex = 0;
		for ( ; nIndex < m_arPrinters.GetSize(); nIndex++ )
		{
			CString sCurPrinter(m_arPrinters[nIndex].pPrinterName);
			if ( -1 != sPrintername.Find(sCurPrinter) && -1 != sCurPrinter.Find(sPrintername) )
				break;
		}

		if ( nIndex >= m_arPrinters.GetSize() )
			return sResult;

		LPSTR sParameters = m_arPrinters[nIndex].pParameters;
		CString sTemp(sParameters);

		if ( -1 != sTemp.Find("CancelPrinterJob") )
			return "CancelPrinterJob";

		if ( -1 == sTemp.Find("OutPutFile=") )
			return sResult;

		if ( -1 == sTemp.Find("OutPutFormat=") )
		{
			sResult = sTemp.Right(sTemp.GetLength() - 11);
			*nOutputFormat = 0;
			return sResult;
		}

		int nFormat = sTemp.Find("OutPutFormat=");
		int nFile   = sTemp.Find("OutPutFile=");
		
		CString sFormat = sTemp;
		sFormat.GetBufferSetLength(nFile - 1);
		CString sFile = sTemp.Right(sTemp.GetLength() - sFormat.GetLength() - 1 - 11);
		sFormat = sFormat.Right(sFormat.GetLength() - 13);
		*nOutputFormat = atoi( sFormat.GetBuffer() );

		return sFile;

	};
private:

	BOOL InstallMonitor(CString sMonitorname, CString sEnvironment, CString sDLLName)
	{
		MONITOR_INFO_2 oMonitorInf;

		oMonitorInf.pName        = (LPSTR)sMonitorname.GetBuffer();
		oMonitorInf.pDLLName     = (LPSTR)sDLLName.GetBuffer();
		oMonitorInf.pEnvironment = (LPSTR)sEnvironment.GetBuffer();
		
		if ( 0 == AddMonitor(NULL, 2, (LPBYTE)&oMonitorInf) )
			return FALSE;
		else 
			return TRUE;
	};
	BOOL UnInstallMonitor(CString sMonitorname)
	{
		if ( 0 == DeleteMonitor(NULL, NULL, (LPSTR)sMonitorname.GetBuffer() ) )
		{
			DWORD dwError = GetLastError();
			return FALSE;
		}
		else 
			return TRUE;
	};
	void InstallPort(CString sPortname, CString sMonitorname, CString sRedmonCommand, CString sPrintername)
	{
		CRegistry oReg;
		oReg.SetHKEY(HKEY_LOCAL_MACHINE);
		oReg.SetKeyRoot("System\\CurrentControlSet\\Control\\Print\\Monitors");
		oReg.SetSubkey(sMonitorname + "\\Ports\\" + sPortname);
		if ( !oReg.KeyExist() )
			oReg.CreateKey(sMonitorname + "\\Ports\\" + sPortname);

		BYTE pDWORD300[4] = {44, 1, 0, 0};
		BYTE pDWORD0[4]   = { 0, 0, 0, 0};
		BYTE pDWORD1[4]   = { 1, 0, 0, 0};

		oReg.SetRegistryValue("Command",      REG_SZ,    sRedmonCommand.GetLength(), (BYTE*)sRedmonCommand.GetBuffer() );
		oReg.SetRegistryValue("Delay",        REG_DWORD, 4,                          (BYTE*)pDWORD300);
		oReg.SetRegistryValue("Description",  REG_SZ,    15,                         (BYTE*)"Redirected Port");
		oReg.SetRegistryValue("LogFileDebug", REG_DWORD, 4,                          (BYTE*)pDWORD0);
		oReg.SetRegistryValue("LogFileUse",   REG_DWORD, 4,                          (BYTE*)pDWORD0);
		oReg.SetRegistryValue("Output",       REG_DWORD, 4,                          (BYTE*)pDWORD0);
		oReg.SetRegistryValue("Printer",      REG_SZ,    sPrintername.GetLength(),   (BYTE*)sPrintername.GetBuffer() );
		oReg.SetRegistryValue("PrintError",   REG_DWORD, 4,                          (BYTE*)pDWORD0);
		oReg.SetRegistryValue("RunUser",      REG_DWORD, 4,                          (BYTE*)pDWORD1);
		oReg.SetRegistryValue("ShowWindow",   REG_DWORD, 4,                          (BYTE*)pDWORD0);

	};	
	BOOL UnInstallPort(CString sPortname)
	{
		if ( 0 == DeletePort(NULL, NULL, (LPSTR)sPortname.GetBuffer() ) )
		{
			DWORD dwError = GetLastError();
			return FALSE;
		}
		else
			return TRUE;
	};
	BOOL InstallDriver(CString sDrivername)
	{

		DRIVER_INFO_3 oDriverInf;
		oDriverInf.pName            = (LPSTR)sDrivername.GetBuffer();
		oDriverInf.pDefaultDataType = (LPSTR)"RAW";
		oDriverInf.pMonitorName     = NULL;
	//50060: If IsWin9xMe = True Then
	//50070: .cVersion = 0
	//50080: .pDependentFiles = "ADOBEPS4.HLP" & vbNullString & "ICONLIB.DLL" & vbNullString & "PSMON.DLL" & vbNullString & "ADFONTS.MFM" & vbNullString & "ADOBEPS4.HLP" & vbNullString & "ADOBEPS4.DRV" & vbNullString & "ADIST5.PPD" & vbNullString & vbNullString
	//50150: .pConfigFile = "ADOBEPS4.DRV"
	//50160: .pDriverPath = "ADOBEPS4.DRV"
	//50170: .pEnvironment = Win9xEnvironment
	//50180: .pHelpFile = "ADOBEPS4.HLP"
	//50190: .pDataFile = "ADIST5.PPD"
	//50200: ' ???
	//50210: '   .cVersion = 3474436
	//50220: End If
	//50230: If IsWinNT4 = True Then
	//50240: .cVersion = 2
	//50250: .pDependentFiles = "PDFCREAT.PPD" & vbNullString & "ADOBEPS5.DLL" & vbNullString & "ADOBEPSU.DLL" & vbNullString & "ADOBEPS5.NTF" & vbNullString & "ADOBEPSU.HLP" & vbNullString & vbNullString
	//50300: .pConfigFile = "ADOBEPSU.DLL"
	//50310: .pDriverPath = "ADOBEPS5.DLL"
	//50320: .pEnvironment = WinNTEnvironment
	//50330: .pHelpFile = "ADOBEPSU.HLP"
	//50340: .pDataFile = "PDFCREAT.PPD"
	//50350: End If
	//50360: If IsWin2000 = True Then
	//50370: .cVersion = 3
	//50380: .pDependentFiles = "PSCRIPT.NTF" & vbNullString & vbNullString
	//50390: .pConfigFile = "PS5UI.DLL"
	//50400: .pDriverPath = "PSCRIPT5.DLL"
	//50410: .pEnvironment = WinNTEnvironment
	//50420: .pHelpFile = "PSCRIPT.HLP"
	//50430: .pDataFile = "PDFCREAT.PPD"
	//50440: End If

	//50450: If IsWinXPPlus = True Or IsWinVista = True Then 'WinXP and above
		oDriverInf.cVersion        = 3;
		oDriverInf.pDependentFiles = (LPSTR)"PSCRIPT.NTF\0PSCRIPT5.DLL\0PS5UI.DLL\0";
		oDriverInf.pConfigFile     = (LPSTR)"PS5UI.DLL";
		oDriverInf.pDriverPath     = (LPSTR)"PSCRIPT5.DLL";
		oDriverInf.pEnvironment    = (LPSTR)WinNTEnvironment.GetBuffer();
		oDriverInf.pHelpFile       = (LPSTR)"PSCRIPT.HLP";
		oDriverInf.pDataFile       = (LPSTR)"PDF.PPD";
	//50530: End If
		if ( 0 == AddPrinterDriver(NULL, oDriverInf.cVersion, (LPBYTE)&oDriverInf) )
		{
			DWORD dwError = GetLastError();
			return FALSE;
		}
		else
			return TRUE;
	};	
	BOOL UnInstallDriver(CString sDrivername, CString sEnvironment)
	{
		if ( 0 == DeletePrinterDriver(NULL, (LPSTR)sEnvironment.GetBuffer(), (LPSTR)sDrivername.GetBuffer() ) )
		{
			DWORD dwError = GetLastError();
			return FALSE;
		}
		else
			return TRUE;
	};
	BOOL InstallPrinter(CString sPrintername, CString sDrivername, CString sPortname, CString sOutPutFile = NULL)
	{
		PRINTER_INFO_2A oPrinterInfo;
		oPrinterInfo.pPrinterName    = (LPSTR)sPrintername.GetBuffer();
		oPrinterInfo.pDriverName     = (LPSTR)sDrivername.GetBuffer();
		oPrinterInfo.pPrintProcessor = (LPSTR)"WinPrint";
		oPrinterInfo.pPortName       = (LPSTR)sPortname.GetBuffer();
		oPrinterInfo.pComment        = (LPSTR)"AVS Document Printer";
		oPrinterInfo.pShareName      = (LPSTR)sPrintername.GetBuffer();
		oPrinterInfo.pDatatype       = (LPSTR)"RAW";
		oPrinterInfo.Priority        = 1;
		oPrinterInfo.DefaultPriority = 1;

		if ( 0 == m_arPrinters.GetSize() )
			oPrinterInfo.Attributes = 64; // утсанавливаем как defaultprinter
		else
			oPrinterInfo.Attributes = 0;

		oPrinterInfo.pServerName         = NULL;
		oPrinterInfo.pLocation           = NULL;
		oPrinterInfo.pSepFile            = NULL;
		oPrinterInfo.pParameters         = (LPSTR)sOutPutFile.GetBuffer();
		oPrinterInfo.pSecurityDescriptor = NULL;

		oPrinterInfo.pDevMode = NULL;
		oPrinterInfo.StartTime = 0;
		oPrinterInfo.UntilTime = 0;

		HANDLE oHandle = AddPrinter(NULL, 2, (LPBYTE)&oPrinterInfo);

		if ( !oHandle )
			return FALSE;
		else
		{
			ClosePrinter(oHandle);
			return TRUE;
		}
	};
	BOOL UnInstallPrinter(CString sPrintername)
	{
		PRINTER_DEFAULTS oPrinterDefaults;
		HANDLE oHandle;
		oPrinterDefaults.pDatatype     = NULL;
		oPrinterDefaults.pDevMode      = NULL;
		oPrinterDefaults.DesiredAccess = PRINTER_ALL_ACCESS;
		DWORD dwError = GetLastError();
		dwError = GetLastError();
		if ( OpenPrinter((LPSTR)sPrintername.GetBuffer(), &oHandle, &oPrinterDefaults) )
		{
			dwError = GetLastError();
			if ( DeletePrinter(oHandle) )
			{
				dwError = GetLastError();
				if ( ClosePrinter(oHandle) )
				{
					dwError = GetLastError();
					return TRUE;
				}
			}
		}
		return FALSE;
	};
	BOOL GetMonitors(void)
	{
		BOOL   bResult = FALSE;
		DWORD  nBufferLength = 0;
		LPBYTE pBuffer = new BYTE[1024];
		DWORD  nBufferNeed = 0;
		DWORD  nMonitorsCount = 0;

		bResult = EnumMonitors(NULL, 1, pBuffer, nBufferLength, &nBufferNeed, &nMonitorsCount);
		delete []pBuffer;

		if ( nBufferNeed > 0 )
		{
			if ( m_pMonitorBuffer )
				delete []m_pMonitorBuffer;
			m_arMonitors.RemoveAll();

			nBufferLength = nBufferNeed;
			m_pMonitorBuffer = new BYTE[nBufferNeed];

			bResult = EnumMonitors(NULL, 1, m_pMonitorBuffer, nBufferLength, &nBufferNeed, &nMonitorsCount);

			BYTE *pCur = m_pMonitorBuffer;
			for ( int nIndex = 0; nIndex < nMonitorsCount; nIndex++)
			{
				const MONITOR_INFO_1& oMonitor = *(MONITOR_INFO_1*)pCur;
				pCur += sizeof(oMonitor);
				m_arMonitors.Add(oMonitor);
			}
			// здесь буфер не освобождаем, чтобы информация о портах сохранялась
			// удаление происходит в деструкторе

		}

		return bResult;
	}

	BOOL GetPorts(void)
	{
		BOOL   bResult = FALSE;
		DWORD  nBufferLenght = 0;
		LPBYTE pBuffer = new BYTE[1024];
		DWORD  nBufferNeed = 0;
		DWORD  nPortsCount = 0;

		bResult = EnumPorts(NULL, 1, pBuffer, nBufferLenght, &nBufferNeed, &nPortsCount);
		delete []pBuffer;

		if ( nBufferNeed > 0 )
		{

			if ( m_pPortBuffer )
				delete []m_pPortBuffer;
			m_arPorts.RemoveAll();

			nBufferLenght = nBufferNeed;
			m_pPortBuffer = new BYTE[nBufferNeed];
			bResult = EnumPorts(NULL, 1, m_pPortBuffer, nBufferLenght, &nBufferNeed, &nPortsCount);
			BYTE *pCur = m_pPortBuffer;
			for ( int nIndex = 0; nIndex < nPortsCount; nIndex++)
			{
				const PORT_INFO_1& oPort = *(PORT_INFO_1*)pCur;
				pCur += sizeof(oPort);
				m_arPorts.Add(oPort);
			}
			// здесь буфер не освобождаем, чтобы информация о портах сохранялась
			// удаление происходит в деструкторе
		}
		return bResult;
	}	

	BOOL GetDrivers(CString sEnvironment = "")
	{
		BOOL   bResult = FALSE;
		DWORD  nBufferLenght = 0;
		LPBYTE pBuffer = new BYTE[1024];
		DWORD  nBufferNeed = 0;
		DWORD  nDriversCount = 0;

		bResult = EnumPrinterDrivers(NULL, (LPSTR)sEnvironment.GetBuffer(), 1, pBuffer, nBufferLenght, &nBufferNeed, &nDriversCount);
		delete []pBuffer;

		if ( nBufferNeed > 0 )
		{

			if ( m_pDriverBuffer )
				delete []m_pDriverBuffer;
			m_arDrivers.RemoveAll();

			nBufferLenght = nBufferNeed;
			m_pDriverBuffer = new BYTE[nBufferNeed];
			bResult = EnumPrinterDrivers(NULL, (LPSTR)sEnvironment.GetBuffer(), 1, m_pDriverBuffer, nBufferLenght, &nBufferNeed, &nDriversCount);
			BYTE *pCur = m_pDriverBuffer;
			for ( int nIndex = 0; nIndex < nDriversCount; nIndex++)
			{
				const DRIVER_INFO_1& oDriver = *(DRIVER_INFO_1*)pCur;
				pCur += sizeof(oDriver);
				m_arDrivers.Add(oDriver);
			}
			// здесь буфер не освобождаем, чтобы информация о портах сохранялась
			// удаление происходит в деструкторе
		}
		return bResult;

	}
	BOOL GetPrinters(void)
	{
		BOOL   bResult = FALSE;
		DWORD  nBufferLenght = 0;
		LPBYTE pBuffer = new BYTE[1024];
		DWORD  nBufferNeed = 0;
		DWORD  nPrintersCount = 0;

		bResult = EnumPrinters(PRINTER_ENUM_LOCAL, NULL, 2, pBuffer, nBufferLenght, &nBufferNeed, &nPrintersCount);
		delete []pBuffer;

		if ( nBufferNeed > 0 )
		{

			if ( m_pPrinterBuffer )
				delete []m_pPrinterBuffer;
			m_arPrinters.RemoveAll();

			nBufferLenght = nBufferNeed;
			m_pPrinterBuffer = new BYTE[nBufferNeed];
			bResult = EnumPrinters(PRINTER_ENUM_LOCAL, NULL, 2, m_pPrinterBuffer, nBufferLenght, &nBufferNeed, &nPrintersCount);
			BYTE *pCur = m_pPrinterBuffer;
			for ( int nIndex = 0; nIndex < nPrintersCount; nIndex++)
			{
				const PRINTER_INFO_2& oPrinter = *(PRINTER_INFO_2*)pCur;
				pCur += sizeof(oPrinter);
				m_arPrinters.Add(oPrinter);
			}
			// здесь буфер не освобождаем, чтобы информация о портах сохранялась
			// удаление происходит в деструкторе
		}
		return bResult;
	}	
	BOOL PortIsInstalled(CString sPortname )
	{
		for ( int nIndex = 0; nIndex < m_arPorts.GetSize(); nIndex++ )
		{
			CString sCurPort(m_arPorts[nIndex].pName);
			if ( -1 != sPortname.Find(sCurPort) && -1 != sCurPort.Find(sPortname) )
				return TRUE;
		}
		return FALSE;
	};

	BOOL MonitorIsInstalled(CString sMonitorname)
	{
		for ( int nIndex = 0; nIndex < m_arMonitors.GetSize(); nIndex++ )
		{
			CString sCurMonitor(m_arMonitors[nIndex].pName);
			if ( -1 != sMonitorname.Find(sCurMonitor) && -1 != sCurMonitor.Find(sMonitorname) )
				return TRUE;
		}
		return FALSE;
	};
	BOOL DriverIsInstalled(CString sDrivername)
	{
		for ( int nIndex = 0; nIndex < m_arDrivers.GetSize(); nIndex++ )
		{
			CString sCurDriver(m_arDrivers[nIndex].pName);
			if ( -1 != sDrivername.Find(sCurDriver) && -1 != sCurDriver.Find(sDrivername) )
				return TRUE;
		}
		return FALSE;
	};	
	BOOL PrinterIsInstalled(CString sPrintername)
	{
		for ( int nIndex = 0; nIndex < m_arPrinters.GetSize(); nIndex++ )
		{
			CString sCurPrinter(m_arPrinters[nIndex].pPrinterName);
			if ( -1 != sPrintername.Find(sCurPrinter) && -1 != sCurPrinter.Find(sPrintername) )
				return TRUE;
		}
		return FALSE;
	};	
};