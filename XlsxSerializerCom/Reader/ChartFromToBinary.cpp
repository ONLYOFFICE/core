/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "ChartFromToBinary.h"
#include "../Common/BinReaderWriterDefines.h"
#include "../../ASCOfficePPTXFile/Editor/BinReaderWriterDefines.h"
#include "../../ASCOfficeDocxFile2/BinReader/DefaultThemeWriter.h"

#include "../../Common/DocxFormat/Source/XlsxFormat/Chart/Chart.h"
#include "../../Common/DocxFormat/Source/DocxFormat/ChartDrawing.h"
#include "../../ASCOfficePPTXFile/PPTXFormat/Theme.h"

using namespace OOX::Spreadsheet;

namespace BinXlsxRW
{
	SaveParams::SaveParams(const std::wstring& _sDrawingsPath, const std::wstring& _sThemePath, OOX::CContentTypes* _pContentTypes, CSVWriter::CCSVWriter* _pCSVWriter)
	{
		pContentTypes	= _pContentTypes;
        sThemePath		= _sThemePath;
		sDrawingsPath	= _sDrawingsPath;

		nThemeOverrideCount = 1;
		pCSVWriter = _pCSVWriter;
	}

	BYTE c_oserct_extlstEXT = 0;

	BYTE c_oserct_chartspaceDATE1904 = 0;
	BYTE c_oserct_chartspaceLANG = 1;
	BYTE c_oserct_chartspaceROUNDEDCORNERS = 2;
	BYTE c_oserct_chartspaceALTERNATECONTENT = 3;
	BYTE c_oserct_chartspaceSTYLE = 4;
	BYTE c_oserct_chartspaceCLRMAPOVR = 5;
	BYTE c_oserct_chartspacePIVOTSOURCE = 6;
	BYTE c_oserct_chartspacePROTECTION = 7;
	BYTE c_oserct_chartspaceCHART = 8;
	BYTE c_oserct_chartspaceSPPR = 9;
	BYTE c_oserct_chartspaceTXPR = 10;
	BYTE c_oserct_chartspaceEXTERNALDATA = 11;
	BYTE c_oserct_chartspacePRINTSETTINGS = 12;
	BYTE c_oserct_chartspaceUSERSHAPES = 13;
	BYTE c_oserct_chartspaceEXTLST = 14;
	BYTE c_oserct_chartspaceTHEMEOVERRIDE = 15;
	
	BYTE c_oserct_usershapes_COUNT = 0;
	BYTE c_oserct_usershapes_SHAPE_REL = 1;
	BYTE c_oserct_usershapes_SHAPE_ABS = 2;

	BYTE c_oserct_booleanVAL = 0;

	BYTE c_oserct_relidID = 0;

	BYTE c_oserct_pagesetupPAPERSIZE = 0;
	BYTE c_oserct_pagesetupPAPERHEIGHT = 1;
	BYTE c_oserct_pagesetupPAPERWIDTH = 2;
	BYTE c_oserct_pagesetupFIRSTPAGENUMBER = 3;
	BYTE c_oserct_pagesetupORIENTATION = 4;
	BYTE c_oserct_pagesetupBLACKANDWHITE = 5;
	BYTE c_oserct_pagesetupDRAFT = 6;
	BYTE c_oserct_pagesetupUSEFIRSTPAGENUMBER = 7;
	BYTE c_oserct_pagesetupHORIZONTALDPI = 8;
	BYTE c_oserct_pagesetupVERTICALDPI = 9;
	BYTE c_oserct_pagesetupCOPIES = 10;

	BYTE c_oserct_pagemarginsL = 0;
	BYTE c_oserct_pagemarginsR = 1;
	BYTE c_oserct_pagemarginsT = 2;
	BYTE c_oserct_pagemarginsB = 3;
	BYTE c_oserct_pagemarginsHEADER = 4;
	BYTE c_oserct_pagemarginsFOOTER = 5;

	BYTE c_oserct_headerfooterODDHEADER = 0;
	BYTE c_oserct_headerfooterODDFOOTER = 1;
	BYTE c_oserct_headerfooterEVENHEADER = 2;
	BYTE c_oserct_headerfooterEVENFOOTER = 3;
	BYTE c_oserct_headerfooterFIRSTHEADER = 4;
	BYTE c_oserct_headerfooterFIRSTFOOTER = 5;
	BYTE c_oserct_headerfooterALIGNWITHMARGINS = 6;
	BYTE c_oserct_headerfooterDIFFERENTODDEVEN = 7;
	BYTE c_oserct_headerfooterDIFFERENTFIRST = 8;

	BYTE c_oserct_printsettingsHEADERFOOTER = 0;
	BYTE c_oserct_printsettingsPAGEMARGINS = 1;
	BYTE c_oserct_printsettingsPAGESETUP = 2;

	BYTE c_oserct_externaldataAUTOUPDATE = 0;
	BYTE c_oserct_externaldataID = 1;

	BYTE c_oserct_dispblanksasVAL = 0;

	BYTE c_oserct_legendentryIDX = 0;
	BYTE c_oserct_legendentryDELETE = 1;
	BYTE c_oserct_legendentryTXPR = 2;
	BYTE c_oserct_legendentryEXTLST = 3;

	BYTE c_oserct_unsignedintVAL = 0;

	BYTE c_oserct_extensionANY = 0;
	BYTE c_oserct_extensionURI = 1;

	BYTE c_oserct_legendposVAL = 0;

	BYTE c_oserct_legendLEGENDPOS = 0;
	BYTE c_oserct_legendLEGENDENTRY = 1;
	BYTE c_oserct_legendLAYOUT = 2;
	BYTE c_oserct_legendOVERLAY = 3;
	BYTE c_oserct_legendSPPR = 4;
	BYTE c_oserct_legendTXPR = 5;
	BYTE c_oserct_legendEXTLST = 6;

	BYTE c_oserct_layoutMANUALLAYOUT = 0;
	BYTE c_oserct_layoutEXTLST = 1;

	BYTE c_oserct_manuallayoutLAYOUTTARGET = 0;
	BYTE c_oserct_manuallayoutXMODE = 1;
	BYTE c_oserct_manuallayoutYMODE = 2;
	BYTE c_oserct_manuallayoutWMODE = 3;
	BYTE c_oserct_manuallayoutHMODE = 4;
	BYTE c_oserct_manuallayoutX = 5;
	BYTE c_oserct_manuallayoutY = 6;
	BYTE c_oserct_manuallayoutW = 7;
	BYTE c_oserct_manuallayoutH = 8;
	BYTE c_oserct_manuallayoutEXTLST = 9;

	BYTE c_oserct_layouttargetVAL = 0;

	BYTE c_oserct_layoutmodeVAL = 0;

	BYTE c_oserct_doubleVAL = 0;

	BYTE c_oserct_dtableSHOWHORZBORDER = 0;
	BYTE c_oserct_dtableSHOWVERTBORDER = 1;
	BYTE c_oserct_dtableSHOWOUTLINE = 2;
	BYTE c_oserct_dtableSHOWKEYS = 3;
	BYTE c_oserct_dtableSPPR = 4;
	BYTE c_oserct_dtableTXPR = 5;
	BYTE c_oserct_dtableEXTLST = 6;

	BYTE c_oserct_seraxAXID = 0;
	BYTE c_oserct_seraxSCALING = 1;
	BYTE c_oserct_seraxDELETE = 2;
	BYTE c_oserct_seraxAXPOS = 3;
	BYTE c_oserct_seraxMAJORGRIDLINES = 4;
	BYTE c_oserct_seraxMINORGRIDLINES = 5;
	BYTE c_oserct_seraxTITLE = 6;
	BYTE c_oserct_seraxNUMFMT = 7;
	BYTE c_oserct_seraxMAJORTICKMARK = 8;
	BYTE c_oserct_seraxMINORTICKMARK = 9;
	BYTE c_oserct_seraxTICKLBLPOS = 10;
	BYTE c_oserct_seraxSPPR = 11;
	BYTE c_oserct_seraxTXPR = 12;
	BYTE c_oserct_seraxCROSSAX = 13;
	BYTE c_oserct_seraxCROSSES = 14;
	BYTE c_oserct_seraxCROSSESAT = 15;
	BYTE c_oserct_seraxTICKLBLSKIP = 16;
	BYTE c_oserct_seraxTICKMARKSKIP = 17;
	BYTE c_oserct_seraxEXTLST = 18;

	BYTE c_oserct_scalingLOGBASE = 0;
	BYTE c_oserct_scalingORIENTATION = 1;
	BYTE c_oserct_scalingMAX = 2;
	BYTE c_oserct_scalingMIN = 3;
	BYTE c_oserct_scalingEXTLST = 4;

	BYTE c_oserct_logbaseVAL = 0;

	BYTE c_oserct_orientationVAL = 0;

	BYTE c_oserct_axposVAL = 0;

	BYTE c_oserct_chartlinesSPPR = 0;

	BYTE c_oserct_titleTX = 0;
	BYTE c_oserct_titleLAYOUT = 1;
	BYTE c_oserct_titleOVERLAY = 2;
	BYTE c_oserct_titleSPPR = 3;
	BYTE c_oserct_titleTXPR = 4;
	BYTE c_oserct_titleEXTLST = 5;

	BYTE c_oserct_txRICH = 0;
	BYTE c_oserct_txSTRREF = 1;

	BYTE c_oserct_strrefF = 0;
	BYTE c_oserct_strrefSTRCACHE = 1;
	BYTE c_oserct_strrefEXTLST = 2;

	BYTE c_oserct_strdataPTCOUNT = 0;
	BYTE c_oserct_strdataPT = 1;
	BYTE c_oserct_strdataEXTLST = 2;

	BYTE c_oserct_strvalV = 0;
	BYTE c_oserct_strvalIDX = 1;

	BYTE c_oserct_numfmtFORMATCODE = 0;
	BYTE c_oserct_numfmtSOURCELINKED = 1;

	BYTE c_oserct_tickmarkVAL = 0;

	BYTE c_oserct_ticklblposVAL = 0;

	BYTE c_oserct_crossesVAL = 0;

	BYTE c_oserct_skipVAL = 0;

	BYTE c_oserct_timeunitVAL = 0;

	BYTE c_oserct_dateaxAXID = 0;
	BYTE c_oserct_dateaxSCALING = 1;
	BYTE c_oserct_dateaxDELETE = 2;
	BYTE c_oserct_dateaxAXPOS = 3;
	BYTE c_oserct_dateaxMAJORGRIDLINES = 4;
	BYTE c_oserct_dateaxMINORGRIDLINES = 5;
	BYTE c_oserct_dateaxTITLE = 6;
	BYTE c_oserct_dateaxNUMFMT = 7;
	BYTE c_oserct_dateaxMAJORTICKMARK = 8;
	BYTE c_oserct_dateaxMINORTICKMARK = 9;
	BYTE c_oserct_dateaxTICKLBLPOS = 10;
	BYTE c_oserct_dateaxSPPR = 11;
	BYTE c_oserct_dateaxTXPR = 12;
	BYTE c_oserct_dateaxCROSSAX = 13;
	BYTE c_oserct_dateaxCROSSES = 14;
	BYTE c_oserct_dateaxCROSSESAT = 15;
	BYTE c_oserct_dateaxAUTO = 16;
	BYTE c_oserct_dateaxLBLOFFSET = 17;
	BYTE c_oserct_dateaxBASETIMEUNIT = 18;
	BYTE c_oserct_dateaxMAJORUNIT = 19;
	BYTE c_oserct_dateaxMAJORTIMEUNIT = 20;
	BYTE c_oserct_dateaxMINORUNIT = 21;
	BYTE c_oserct_dateaxMINORTIMEUNIT = 22;
	BYTE c_oserct_dateaxEXTLST = 23;

	BYTE c_oserct_lbloffsetVAL = 0;

	BYTE c_oserct_axisunitVAL = 0;

	BYTE c_oserct_lblalgnVAL = 0;

	BYTE c_oserct_cataxAXID = 0;
	BYTE c_oserct_cataxSCALING = 1;
	BYTE c_oserct_cataxDELETE = 2;
	BYTE c_oserct_cataxAXPOS = 3;
	BYTE c_oserct_cataxMAJORGRIDLINES = 4;
	BYTE c_oserct_cataxMINORGRIDLINES = 5;
	BYTE c_oserct_cataxTITLE = 6;
	BYTE c_oserct_cataxNUMFMT = 7;
	BYTE c_oserct_cataxMAJORTICKMARK = 8;
	BYTE c_oserct_cataxMINORTICKMARK = 9;
	BYTE c_oserct_cataxTICKLBLPOS = 10;
	BYTE c_oserct_cataxSPPR = 11;
	BYTE c_oserct_cataxTXPR = 12;
	BYTE c_oserct_cataxCROSSAX = 13;
	BYTE c_oserct_cataxCROSSES = 14;
	BYTE c_oserct_cataxCROSSESAT = 15;
	BYTE c_oserct_cataxAUTO = 16;
	BYTE c_oserct_cataxLBLALGN = 17;
	BYTE c_oserct_cataxLBLOFFSET = 18;
	BYTE c_oserct_cataxTICKLBLSKIP = 19;
	BYTE c_oserct_cataxTICKMARKSKIP = 20;
	BYTE c_oserct_cataxNOMULTILVLLBL = 21;
	BYTE c_oserct_cataxEXTLST = 22;

	BYTE c_oserct_dispunitslblLAYOUT = 0;
	BYTE c_oserct_dispunitslblTX = 1;
	BYTE c_oserct_dispunitslblSPPR = 2;
	BYTE c_oserct_dispunitslblTXPR = 3;

	BYTE c_oserct_builtinunitVAL = 0;

	BYTE c_oserct_dispunitsBUILTINUNIT = 0;
	BYTE c_oserct_dispunitsCUSTUNIT = 1;
	BYTE c_oserct_dispunitsDISPUNITSLBL = 2;
	BYTE c_oserct_dispunitsEXTLST = 3;

	BYTE c_oserct_crossbetweenVAL = 0;

	BYTE c_oserct_valaxAXID = 0;
	BYTE c_oserct_valaxSCALING = 1;
	BYTE c_oserct_valaxDELETE = 2;
	BYTE c_oserct_valaxAXPOS = 3;
	BYTE c_oserct_valaxMAJORGRIDLINES = 4;
	BYTE c_oserct_valaxMINORGRIDLINES = 5;
	BYTE c_oserct_valaxTITLE = 6;
	BYTE c_oserct_valaxNUMFMT = 7;
	BYTE c_oserct_valaxMAJORTICKMARK = 8;
	BYTE c_oserct_valaxMINORTICKMARK = 9;
	BYTE c_oserct_valaxTICKLBLPOS = 10;
	BYTE c_oserct_valaxSPPR = 11;
	BYTE c_oserct_valaxTXPR = 12;
	BYTE c_oserct_valaxCROSSAX = 13;
	BYTE c_oserct_valaxCROSSES = 14;
	BYTE c_oserct_valaxCROSSESAT = 15;
	BYTE c_oserct_valaxCROSSBETWEEN = 16;
	BYTE c_oserct_valaxMAJORUNIT = 17;
	BYTE c_oserct_valaxMINORUNIT = 18;
	BYTE c_oserct_valaxDISPUNITS = 19;
	BYTE c_oserct_valaxEXTLST = 20;

	BYTE c_oserct_sizerepresentsVAL = 0;

	BYTE c_oserct_bubblescaleVAL = 0;

	BYTE c_oserct_bubbleserIDX = 0;
	BYTE c_oserct_bubbleserORDER = 1;
	BYTE c_oserct_bubbleserTX = 2;
	BYTE c_oserct_bubbleserSPPR = 3;
	BYTE c_oserct_bubbleserINVERTIFNEGATIVE = 4;
	BYTE c_oserct_bubbleserDPT = 5;
	BYTE c_oserct_bubbleserDLBLS = 6;
	BYTE c_oserct_bubbleserTRENDLINE = 7;
	BYTE c_oserct_bubbleserERRBARS = 8;
	BYTE c_oserct_bubbleserXVAL = 9;
	BYTE c_oserct_bubbleserYVAL = 10;
	BYTE c_oserct_bubbleserBUBBLESIZE = 11;
	BYTE c_oserct_bubbleserBUBBLE3D = 12;
	BYTE c_oserct_bubbleserEXTLST = 13;

	BYTE c_oserct_sertxSTRREF = 0;
	BYTE c_oserct_sertxV = 1;

	BYTE c_oserct_dptIDX = 0;
	BYTE c_oserct_dptINVERTIFNEGATIVE = 1;
	BYTE c_oserct_dptMARKER = 2;
	BYTE c_oserct_dptBUBBLE3D = 3;
	BYTE c_oserct_dptEXPLOSION = 4;
	BYTE c_oserct_dptSPPR = 5;
	BYTE c_oserct_dptPICTUREOPTIONS = 6;
	BYTE c_oserct_dptEXTLST = 7;

	BYTE c_oserct_markerSYMBOL = 0;
	BYTE c_oserct_markerSIZE = 1;
	BYTE c_oserct_markerSPPR = 2;
	BYTE c_oserct_markerEXTLST = 3;

	BYTE c_oserct_markerstyleVAL = 0;

	BYTE c_oserct_markersizeVAL = 0;

	BYTE c_oserct_pictureoptionsAPPLYTOFRONT = 0;
	BYTE c_oserct_pictureoptionsAPPLYTOSIDES = 1;
	BYTE c_oserct_pictureoptionsAPPLYTOEND = 2;
	BYTE c_oserct_pictureoptionsPICTUREFORMAT = 3;
	BYTE c_oserct_pictureoptionsPICTURESTACKUNIT = 4;

	BYTE c_oserct_pictureformatVAL = 0;

	BYTE c_oserct_picturestackunitVAL = 0;

	BYTE c_oserct_dlblsDLBL = 0;
	BYTE c_oserct_dlblsITEMS = 1;
	BYTE c_oserct_dlblsDLBLPOS = 2;
	BYTE c_oserct_dlblsDELETE = 3;
	BYTE c_oserct_dlblsLEADERLINES = 4;
	BYTE c_oserct_dlblsNUMFMT = 5;
	BYTE c_oserct_dlblsSEPARATOR = 6;
	BYTE c_oserct_dlblsSHOWBUBBLESIZE = 7;
	BYTE c_oserct_dlblsSHOWCATNAME = 8;
	BYTE c_oserct_dlblsSHOWLEADERLINES = 9;
	BYTE c_oserct_dlblsSHOWLEGENDKEY = 10;
	BYTE c_oserct_dlblsSHOWPERCENT = 11;
	BYTE c_oserct_dlblsSHOWSERNAME = 12;
	BYTE c_oserct_dlblsSHOWVAL = 13;
	BYTE c_oserct_dlblsSPPR = 14;
	BYTE c_oserct_dlblsTXPR = 15;
	BYTE c_oserct_dlblsEXTLST = 16;

	BYTE c_oserct_dlblIDX = 0;
	BYTE c_oserct_dlblITEMS = 1;
	BYTE c_oserct_dlblDLBLPOS = 2;
	BYTE c_oserct_dlblDELETE = 3;
	BYTE c_oserct_dlblLAYOUT = 4;
	BYTE c_oserct_dlblNUMFMT = 5;
	BYTE c_oserct_dlblSEPARATOR = 6;
	BYTE c_oserct_dlblSHOWBUBBLESIZE = 7;
	BYTE c_oserct_dlblSHOWCATNAME = 8;
	BYTE c_oserct_dlblSHOWLEGENDKEY = 9;
	BYTE c_oserct_dlblSHOWPERCENT = 10;
	BYTE c_oserct_dlblSHOWSERNAME = 11;
	BYTE c_oserct_dlblSHOWVAL = 12;
	BYTE c_oserct_dlblSPPR = 13;
	BYTE c_oserct_dlblTX = 14;
	BYTE c_oserct_dlblTXPR = 15;
	BYTE c_oserct_dlblEXTLST = 16;

	BYTE c_oserct_dlblposVAL = 0;

	BYTE c_oserct_trendlineNAME = 0;
	BYTE c_oserct_trendlineSPPR = 1;
	BYTE c_oserct_trendlineTRENDLINETYPE = 2;
	BYTE c_oserct_trendlineORDER = 3;
	BYTE c_oserct_trendlinePERIOD = 4;
	BYTE c_oserct_trendlineFORWARD = 5;
	BYTE c_oserct_trendlineBACKWARD = 6;
	BYTE c_oserct_trendlineINTERCEPT = 7;
	BYTE c_oserct_trendlineDISPRSQR = 8;
	BYTE c_oserct_trendlineDISPEQ = 9;
	BYTE c_oserct_trendlineTRENDLINELBL = 10;
	BYTE c_oserct_trendlineEXTLST = 11;

	BYTE c_oserct_trendlinetypeVAL = 0;

	BYTE c_oserct_orderVAL = 0;

	BYTE c_oserct_periodVAL = 0;

	BYTE c_oserct_trendlinelblLAYOUT = 0;
	BYTE c_oserct_trendlinelblTX = 1;
	BYTE c_oserct_trendlinelblNUMFMT = 2;
	BYTE c_oserct_trendlinelblSPPR = 3;
	BYTE c_oserct_trendlinelblTXPR = 4;
	BYTE c_oserct_trendlinelblEXTLST = 5;

	BYTE c_oserct_errbarsERRDIR = 0;
	BYTE c_oserct_errbarsERRBARTYPE = 1;
	BYTE c_oserct_errbarsERRVALTYPE = 2;
	BYTE c_oserct_errbarsNOENDCAP = 3;
	BYTE c_oserct_errbarsPLUS = 4;
	BYTE c_oserct_errbarsMINUS = 5;
	BYTE c_oserct_errbarsVAL = 6;
	BYTE c_oserct_errbarsSPPR = 7;
	BYTE c_oserct_errbarsEXTLST = 8;

	BYTE c_oserct_errdirVAL = 0;

	BYTE c_oserct_errbartypeVAL = 0;

	BYTE c_oserct_errvaltypeVAL = 0;

	BYTE c_oserct_numdatasourceNUMLIT = 0;
	BYTE c_oserct_numdatasourceNUMREF = 1;

	BYTE c_oserct_numdataFORMATCODE = 0;
	BYTE c_oserct_numdataPTCOUNT = 1;
	BYTE c_oserct_numdataPT = 2;
	BYTE c_oserct_numdataEXTLST = 3;

	BYTE c_oserct_numvalV = 0;
	BYTE c_oserct_numvalIDX = 1;
	BYTE c_oserct_numvalFORMATCODE = 2;

	BYTE c_oserct_numrefF = 0;
	BYTE c_oserct_numrefNUMCACHE = 1;
	BYTE c_oserct_numrefEXTLST = 2;

	BYTE c_oserct_axdatasourceMULTILVLSTRREF = 0;
	BYTE c_oserct_axdatasourceNUMLIT = 1;
	BYTE c_oserct_axdatasourceNUMREF = 2;
	BYTE c_oserct_axdatasourceSTRLIT = 3;
	BYTE c_oserct_axdatasourceSTRREF = 4;

	BYTE c_oserct_multilvlstrrefF = 0;
	BYTE c_oserct_multilvlstrrefMULTILVLSTRCACHE = 1;
	BYTE c_oserct_multilvlstrrefEXTLST = 2;

	BYTE c_oserct_lvlPT = 0;

	BYTE c_oserct_multilvlstrdataPTCOUNT = 0;
	BYTE c_oserct_multilvlstrdataLVL = 1;
	BYTE c_oserct_multilvlstrdataEXTLST = 2;

	BYTE c_oserct_bubblechartVARYCOLORS = 0;
	BYTE c_oserct_bubblechartSER = 1;
	BYTE c_oserct_bubblechartDLBLS = 2;
	BYTE c_oserct_bubblechartBUBBLE3D = 3;
	BYTE c_oserct_bubblechartBUBBLESCALE = 4;
	BYTE c_oserct_bubblechartSHOWNEGBUBBLES = 5;
	BYTE c_oserct_bubblechartSIZEREPRESENTS = 6;
	BYTE c_oserct_bubblechartAXID = 7;
	BYTE c_oserct_bubblechartEXTLST = 8;

	BYTE c_oserct_bandfmtsBANDFMT = 0;

	BYTE c_oserct_surface3dchartWIREFRAME = 0;
	BYTE c_oserct_surface3dchartSER = 1;
	BYTE c_oserct_surface3dchartBANDFMTS = 2;
	BYTE c_oserct_surface3dchartAXID = 3;
	BYTE c_oserct_surface3dchartEXTLST = 4;

	BYTE c_oserct_surfaceserIDX = 0;
	BYTE c_oserct_surfaceserORDER = 1;
	BYTE c_oserct_surfaceserTX = 2;
	BYTE c_oserct_surfaceserSPPR = 3;
	BYTE c_oserct_surfaceserCAT = 4;
	BYTE c_oserct_surfaceserVAL = 5;
	BYTE c_oserct_surfaceserEXTLST = 6;

	BYTE c_oserct_bandfmtIDX = 0;
	BYTE c_oserct_bandfmtSPPR = 1;

	BYTE c_oserct_surfacechartWIREFRAME = 0;
	BYTE c_oserct_surfacechartSER = 1;
	BYTE c_oserct_surfacechartBANDFMTS = 2;
	BYTE c_oserct_surfacechartAXID = 3;
	BYTE c_oserct_surfacechartEXTLST = 4;

	BYTE c_oserct_secondpiesizeVAL = 0;

	BYTE c_oserct_splittypeVAL = 0;

	BYTE c_oserct_ofpietypeVAL = 0;

	BYTE c_oserct_custsplitSECONDPIEPT = 0;

	BYTE c_oserct_ofpiechartOFPIETYPE = 0;
	BYTE c_oserct_ofpiechartVARYCOLORS = 1;
	BYTE c_oserct_ofpiechartSER = 2;
	BYTE c_oserct_ofpiechartDLBLS = 3;
	BYTE c_oserct_ofpiechartGAPWIDTH = 4;
	BYTE c_oserct_ofpiechartSPLITTYPE = 5;
	BYTE c_oserct_ofpiechartSPLITPOS = 6;
	BYTE c_oserct_ofpiechartCUSTSPLIT = 7;
	BYTE c_oserct_ofpiechartSECONDPIESIZE = 8;
	BYTE c_oserct_ofpiechartSERLINES = 9;
	BYTE c_oserct_ofpiechartEXTLST = 10;

	BYTE c_oserct_pieserIDX = 0;
	BYTE c_oserct_pieserORDER = 1;
	BYTE c_oserct_pieserTX = 2;
	BYTE c_oserct_pieserSPPR = 3;
	BYTE c_oserct_pieserEXPLOSION = 4;
	BYTE c_oserct_pieserDPT = 5;
	BYTE c_oserct_pieserDLBLS = 6;
	BYTE c_oserct_pieserCAT = 7;
	BYTE c_oserct_pieserVAL = 8;
	BYTE c_oserct_pieserEXTLST = 9;

	BYTE c_oserct_gapamountVAL = 0;

	BYTE c_oserct_bar3dchartBARDIR = 0;
	BYTE c_oserct_bar3dchartGROUPING = 1;
	BYTE c_oserct_bar3dchartVARYCOLORS = 2;
	BYTE c_oserct_bar3dchartSER = 3;
	BYTE c_oserct_bar3dchartDLBLS = 4;
	BYTE c_oserct_bar3dchartGAPWIDTH = 5;
	BYTE c_oserct_bar3dchartGAPDEPTH = 6;
	BYTE c_oserct_bar3dchartSHAPE = 7;
	BYTE c_oserct_bar3dchartAXID = 8;
	BYTE c_oserct_bar3dchartEXTLST = 9;

	BYTE c_oserct_bardirVAL = 0;

	BYTE c_oserct_bargroupingVAL = 0;

	BYTE c_oserct_barserIDX = 0;
	BYTE c_oserct_barserORDER = 1;
	BYTE c_oserct_barserTX = 2;
	BYTE c_oserct_barserSPPR = 3;
	BYTE c_oserct_barserINVERTIFNEGATIVE = 4;
	BYTE c_oserct_barserPICTUREOPTIONS = 5;
	BYTE c_oserct_barserDPT = 6;
	BYTE c_oserct_barserDLBLS = 7;
	BYTE c_oserct_barserTRENDLINE = 8;
	BYTE c_oserct_barserERRBARS = 9;
	BYTE c_oserct_barserCAT = 10;
	BYTE c_oserct_barserVAL = 11;
	BYTE c_oserct_barserSHAPE = 12;
	BYTE c_oserct_barserEXTLST = 13;

	BYTE c_oserct_shapeVAL = 0;

	BYTE c_oserct_overlapVAL = 0;

	BYTE c_oserct_barchartBARDIR = 0;
	BYTE c_oserct_barchartGROUPING = 1;
	BYTE c_oserct_barchartVARYCOLORS = 2;
	BYTE c_oserct_barchartSER = 3;
	BYTE c_oserct_barchartDLBLS = 4;
	BYTE c_oserct_barchartGAPWIDTH = 5;
	BYTE c_oserct_barchartOVERLAP = 6;
	BYTE c_oserct_barchartSERLINES = 7;
	BYTE c_oserct_barchartAXID = 8;
	BYTE c_oserct_barchartEXTLST = 9;

	BYTE c_oserct_holesizeVAL = 0;

	BYTE c_oserct_doughnutchartVARYCOLORS = 0;
	BYTE c_oserct_doughnutchartSER = 1;
	BYTE c_oserct_doughnutchartDLBLS = 2;
	BYTE c_oserct_doughnutchartFIRSTSLICEANG = 3;
	BYTE c_oserct_doughnutchartHOLESIZE = 4;
	BYTE c_oserct_doughnutchartEXTLST = 5;

	BYTE c_oserct_firstsliceangVAL = 0;

	BYTE c_oserct_pie3dchartVARYCOLORS = 0;
	BYTE c_oserct_pie3dchartSER = 1;
	BYTE c_oserct_pie3dchartDLBLS = 2;
	BYTE c_oserct_pie3dchartEXTLST = 3;

	BYTE c_oserct_piechartVARYCOLORS = 0;
	BYTE c_oserct_piechartSER = 1;
	BYTE c_oserct_piechartDLBLS = 2;
	BYTE c_oserct_piechartFIRSTSLICEANG = 3;
	BYTE c_oserct_piechartEXTLST = 4;

	BYTE c_oserct_scatterserIDX = 0;
	BYTE c_oserct_scatterserORDER = 1;
	BYTE c_oserct_scatterserTX = 2;
	BYTE c_oserct_scatterserSPPR = 3;
	BYTE c_oserct_scatterserMARKER = 4;
	BYTE c_oserct_scatterserDPT = 5;
	BYTE c_oserct_scatterserDLBLS = 6;
	BYTE c_oserct_scatterserTRENDLINE = 7;
	BYTE c_oserct_scatterserERRBARS = 8;
	BYTE c_oserct_scatterserXVAL = 9;
	BYTE c_oserct_scatterserYVAL = 10;
	BYTE c_oserct_scatterserSMOOTH = 11;
	BYTE c_oserct_scatterserEXTLST = 12;

	BYTE c_oserct_scatterstyleVAL = 0;

	BYTE c_oserct_scatterchartSCATTERSTYLE = 0;
	BYTE c_oserct_scatterchartVARYCOLORS = 1;
	BYTE c_oserct_scatterchartSER = 2;
	BYTE c_oserct_scatterchartDLBLS = 3;
	BYTE c_oserct_scatterchartAXID = 4;
	BYTE c_oserct_scatterchartEXTLST = 5;

	BYTE c_oserct_radarserIDX = 0;
	BYTE c_oserct_radarserORDER = 1;
	BYTE c_oserct_radarserTX = 2;
	BYTE c_oserct_radarserSPPR = 3;
	BYTE c_oserct_radarserMARKER = 4;
	BYTE c_oserct_radarserDPT = 5;
	BYTE c_oserct_radarserDLBLS = 6;
	BYTE c_oserct_radarserCAT = 7;
	BYTE c_oserct_radarserVAL = 8;
	BYTE c_oserct_radarserEXTLST = 9;

	BYTE c_oserct_radarstyleVAL = 0;

	BYTE c_oserct_radarchartRADARSTYLE = 0;
	BYTE c_oserct_radarchartVARYCOLORS = 1;
	BYTE c_oserct_radarchartSER = 2;
	BYTE c_oserct_radarchartDLBLS = 3;
	BYTE c_oserct_radarchartAXID = 4;
	BYTE c_oserct_radarchartEXTLST = 5;

	BYTE c_oserct_stockchartSER = 0;
	BYTE c_oserct_stockchartDLBLS = 1;
	BYTE c_oserct_stockchartDROPLINES = 2;
	BYTE c_oserct_stockchartHILOWLINES = 3;
	BYTE c_oserct_stockchartUPDOWNBARS = 4;
	BYTE c_oserct_stockchartAXID = 5;
	BYTE c_oserct_stockchartEXTLST = 6;

	BYTE c_oserct_lineserIDX = 0;
	BYTE c_oserct_lineserORDER = 1;
	BYTE c_oserct_lineserTX = 2;
	BYTE c_oserct_lineserSPPR = 3;
	BYTE c_oserct_lineserMARKER = 4;
	BYTE c_oserct_lineserDPT = 5;
	BYTE c_oserct_lineserDLBLS = 6;
	BYTE c_oserct_lineserTRENDLINE = 7;
	BYTE c_oserct_lineserERRBARS = 8;
	BYTE c_oserct_lineserCAT = 9;
	BYTE c_oserct_lineserVAL = 10;
	BYTE c_oserct_lineserSMOOTH = 11;
	BYTE c_oserct_lineserEXTLST = 12;

	BYTE c_oserct_updownbarsGAPWIDTH = 0;
	BYTE c_oserct_updownbarsUPBARS = 1;
	BYTE c_oserct_updownbarsDOWNBARS = 2;
	BYTE c_oserct_updownbarsEXTLST = 3;

	BYTE c_oserct_updownbarSPPR = 0;

	BYTE c_oserct_line3dchartGROUPING = 0;
	BYTE c_oserct_line3dchartVARYCOLORS = 1;
	BYTE c_oserct_line3dchartSER = 2;
	BYTE c_oserct_line3dchartDLBLS = 3;
	BYTE c_oserct_line3dchartDROPLINES = 4;
	BYTE c_oserct_line3dchartGAPDEPTH = 5;
	BYTE c_oserct_line3dchartAXID = 6;
	BYTE c_oserct_line3dchartEXTLST = 7;

	BYTE c_oserct_groupingVAL = 0;

	BYTE c_oserct_linechartGROUPING = 0;
	BYTE c_oserct_linechartVARYCOLORS = 1;
	BYTE c_oserct_linechartSER = 2;
	BYTE c_oserct_linechartDLBLS = 3;
	BYTE c_oserct_linechartDROPLINES = 4;
	BYTE c_oserct_linechartHILOWLINES = 5;
	BYTE c_oserct_linechartUPDOWNBARS = 6;
	BYTE c_oserct_linechartMARKER = 7;
	BYTE c_oserct_linechartSMOOTH = 8;
	BYTE c_oserct_linechartAXID = 9;
	BYTE c_oserct_linechartEXTLST = 10;

	BYTE c_oserct_area3dchartGROUPING = 0;
	BYTE c_oserct_area3dchartVARYCOLORS = 1;
	BYTE c_oserct_area3dchartSER = 2;
	BYTE c_oserct_area3dchartDLBLS = 3;
	BYTE c_oserct_area3dchartDROPLINES = 4;
	BYTE c_oserct_area3dchartGAPDEPTH = 5;
	BYTE c_oserct_area3dchartAXID = 6;
	BYTE c_oserct_area3dchartEXTLST = 7;

	BYTE c_oserct_areaserIDX = 0;
	BYTE c_oserct_areaserORDER = 1;
	BYTE c_oserct_areaserTX = 2;
	BYTE c_oserct_areaserSPPR = 3;
	BYTE c_oserct_areaserPICTUREOPTIONS = 4;
	BYTE c_oserct_areaserDPT = 5;
	BYTE c_oserct_areaserDLBLS = 6;
	BYTE c_oserct_areaserTRENDLINE = 7;
	BYTE c_oserct_areaserERRBARS = 8;
	BYTE c_oserct_areaserCAT = 9;
	BYTE c_oserct_areaserVAL = 10;
	BYTE c_oserct_areaserEXTLST = 11;

	BYTE c_oserct_areachartGROUPING = 0;
	BYTE c_oserct_areachartVARYCOLORS = 1;
	BYTE c_oserct_areachartSER = 2;
	BYTE c_oserct_areachartDLBLS = 3;
	BYTE c_oserct_areachartDROPLINES = 4;
	BYTE c_oserct_areachartAXID = 5;
	BYTE c_oserct_areachartEXTLST = 6;

	BYTE c_oserct_plotareaLAYOUT = 0;
	BYTE c_oserct_plotareaITEMS = 1;
	BYTE c_oserct_plotareaAREA3DCHART = 2;
	BYTE c_oserct_plotareaAREACHART = 3;
	BYTE c_oserct_plotareaBAR3DCHART = 4;
	BYTE c_oserct_plotareaBARCHART = 5;
	BYTE c_oserct_plotareaBUBBLECHART = 6;
	BYTE c_oserct_plotareaDOUGHNUTCHART = 7;
	BYTE c_oserct_plotareaLINE3DCHART = 8;
	BYTE c_oserct_plotareaLINECHART = 9;
	BYTE c_oserct_plotareaOFPIECHART = 10;
	BYTE c_oserct_plotareaPIE3DCHART = 11;
	BYTE c_oserct_plotareaPIECHART = 12;
	BYTE c_oserct_plotareaRADARCHART = 13;
	BYTE c_oserct_plotareaSCATTERCHART = 14;
	BYTE c_oserct_plotareaSTOCKCHART = 15;
	BYTE c_oserct_plotareaSURFACE3DCHART = 16;
	BYTE c_oserct_plotareaSURFACECHART = 17;
	BYTE c_oserct_plotareaITEMS1 = 18;
	BYTE c_oserct_plotareaCATAX = 19;
	BYTE c_oserct_plotareaDATEAX = 20;
	BYTE c_oserct_plotareaSERAX = 21;
	BYTE c_oserct_plotareaVALAX = 22;
	BYTE c_oserct_plotareaDTABLE = 23;
	BYTE c_oserct_plotareaSPPR = 24;
	BYTE c_oserct_plotareaEXTLST = 25;

	BYTE c_oserct_thicknessVAL = 0;

	BYTE c_oserct_surfaceTHICKNESS = 0;
	BYTE c_oserct_surfaceSPPR = 1;
	BYTE c_oserct_surfacePICTUREOPTIONS = 2;
	BYTE c_oserct_surfaceEXTLST = 3;

	BYTE c_oserct_perspectiveVAL = 0;

	BYTE c_oserct_depthpercentVAL = 0;

	BYTE c_oserct_rotyVAL = 0;

	BYTE c_oserct_hpercentVAL = 0;

	BYTE c_oserct_rotxVAL = 0;

	BYTE c_oserct_view3dROTX = 0;
	BYTE c_oserct_view3dHPERCENT = 1;
	BYTE c_oserct_view3dROTY = 2;
	BYTE c_oserct_view3dDEPTHPERCENT = 3;
	BYTE c_oserct_view3dRANGAX = 4;
	BYTE c_oserct_view3dPERSPECTIVE = 5;
	BYTE c_oserct_view3dEXTLST = 6;

	BYTE c_oserct_pivotfmtIDX = 0;
	BYTE c_oserct_pivotfmtSPPR = 1;
	BYTE c_oserct_pivotfmtTXPR = 2;
	BYTE c_oserct_pivotfmtMARKER = 3;
	BYTE c_oserct_pivotfmtDLBL = 4;
	BYTE c_oserct_pivotfmtEXTLST = 5;

	BYTE c_oserct_pivotfmtsPIVOTFMT = 0;

	BYTE c_oserct_chartTITLE = 0;
	BYTE c_oserct_chartAUTOTITLEDELETED = 1;
	BYTE c_oserct_chartPIVOTFMTS = 2;
	BYTE c_oserct_chartVIEW3D = 3;
	BYTE c_oserct_chartFLOOR = 4;
	BYTE c_oserct_chartSIDEWALL = 5;
	BYTE c_oserct_chartBACKWALL = 6;
	BYTE c_oserct_chartPLOTAREA = 7;
	BYTE c_oserct_chartLEGEND = 8;
	BYTE c_oserct_chartPLOTVISONLY = 9;
	BYTE c_oserct_chartDISPBLANKSAS = 10;
	BYTE c_oserct_chartSHOWDLBLSOVERMAX = 11;
	BYTE c_oserct_chartEXTLST = 12;

	BYTE c_oserct_protectionCHARTOBJECT = 0;
	BYTE c_oserct_protectionDATA = 1;
	BYTE c_oserct_protectionFORMATTING = 2;
	BYTE c_oserct_protectionSELECTION = 3;
	BYTE c_oserct_protectionUSERINTERFACE = 4;

	BYTE c_oserct_pivotsourceNAME = 0;
	BYTE c_oserct_pivotsourceFMTID = 1;
	BYTE c_oserct_pivotsourceEXTLST = 2;

	BYTE c_oserct_style1VAL = 0;

	BYTE c_oserct_styleVAL = 0;

	BYTE c_oserct_textlanguageidVAL = 0;

	BYTE c_oseralternatecontentCHOICE = 0;
	BYTE c_oseralternatecontentFALLBACK = 1;

	BYTE c_oseralternatecontentchoiceSTYLE = 0;
	BYTE c_oseralternatecontentchoiceREQUIRES = 1;

	BYTE c_oseralternatecontentfallbackSTYLE = 0;

	BinaryChartReader::BinaryChartReader(NSBinPptxRW::CBinaryFileReader& oBufferedStream, SaveParams& oSaveParams, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter)
		: Binary_CommonReader(oBufferedStream), m_oSaveParams(oSaveParams), m_pOfficeDrawingConverter(pOfficeDrawingConverter)
	{}

	int BinaryChartReader::ReadCT_extLst(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_extLst* poVal = static_cast<CT_extLst*>(poResult);
		if(c_oserct_extlstEXT == type)
		{
			CT_Extension* pNewElem = new CT_Extension;
			READ1_DEF(length, res, this->ReadCT_Extension, pNewElem);
			poVal->m_ext.push_back(pNewElem);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartSpace(long length, OOX::Spreadsheet::CChartSpace* pChartSpace)
	{
		int res = c_oSerConstants::ReadOk;
		READ1_DEF(length, res, this->ReadCT_ChartSpace, pChartSpace);
		return res;
	}
	int BinaryChartReader::ReadCT_ChartSpace(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		
		OOX::Spreadsheet::CChartSpace *pChart = static_cast<OOX::Spreadsheet::CChartSpace*>(poResult);
		
		if(c_oserct_chartspaceDATE1904 == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			pChart->m_oChartSpace.m_date1904 = pNewElem;
		}
		else if(c_oserct_chartspaceLANG == type)
		{
			CT_TextLanguageID* pNewElem = new CT_TextLanguageID;
			READ1_DEF(length, res, this->ReadCT_TextLanguageID, pNewElem);
			pChart->m_oChartSpace.m_lang = pNewElem;
		}
		else if(c_oserct_chartspaceROUNDEDCORNERS == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			pChart->m_oChartSpace.m_roundedCorners = pNewElem;
		}
		else if(c_oserct_chartspaceALTERNATECONTENT == type)
		{
			AlternateContent* pNewElem = new AlternateContent;
			READ1_DEF(length, res, this->ReadAlternateContent, pNewElem);
			pChart->m_oChartSpace.m_AlternateContent = pNewElem;
		}
		else if(c_oserct_chartspaceSTYLE == type)
		{
			CT_Style1* pNewElem = new CT_Style1;
			READ1_DEF(length, res, this->ReadCT_Style1, pNewElem);
			pChart->m_oChartSpace.m_style = pNewElem;
		}
		else if(c_oserct_chartspaceCLRMAPOVR == type)
		{
			BYTE typeRec1 = m_oBufferedStream.GetUChar();
			
			pChart->m_oChartSpace.m_oClrMapOvr = new PPTX::Logic::ClrMap();

			pChart->m_oChartSpace.m_oClrMapOvr->m_name = L"c:clrMapOvr";
			pChart->m_oChartSpace.m_oClrMapOvr->fromPPTY(&m_oBufferedStream);
		}
		else if(c_oserct_chartspacePIVOTSOURCE == type)
		{
			CT_PivotSource* pNewElem = new CT_PivotSource;
			READ1_DEF(length, res, this->ReadCT_PivotSource, pNewElem);
			pChart->m_oChartSpace.m_pivotSource = pNewElem;
		}
		else if(c_oserct_chartspacePROTECTION == type)
		{
			CT_Protection* pNewElem = new CT_Protection;
			READ1_DEF(length, res, this->ReadCT_Protection, pNewElem);
			pChart->m_oChartSpace.m_protection = pNewElem;
		}
		else if(c_oserct_chartspaceCHART == type)
		{
			CT_Chart* pNewElem = new CT_Chart;
			READ1_DEF(length, res, this->ReadCT_Chart, pNewElem);
			pChart->m_oChartSpace.m_chart = pNewElem;
		}
		else if(c_oserct_chartspaceSPPR == type)
		{
			pChart->m_oChartSpace.m_oSpPr = new PPTX::Logic::SpPr;
			res = ReadCT_SpPr(0, length, pChart->m_oChartSpace.m_oSpPr.GetPointer());
		}
		else if(c_oserct_chartspaceTXPR == type)
		{
			pChart->m_oChartSpace.m_oTxPr = new PPTX::Logic::TxBody;
			res = ReadCT_TxPr(0, length, pChart->m_oChartSpace.m_oTxPr.GetPointer());
		}
		//else if(c_oserct_chartspaceEXTERNALDATA == type)
		//{
		//	CT_ExternalData* pNewElem = new CT_ExternalData;
		//	READ1_DEF(length, res, this->ReadCT_ExternalData, pNewElem);
		//	pChart->m_oChartSpace.m_externalData = pNewElem;
		//}
		else if(c_oserct_chartspacePRINTSETTINGS == type)
		{
			CT_PrintSettings* pNewElem = new CT_PrintSettings;
			READ1_DEF(length, res, this->ReadCT_PrintSettings, pNewElem);
			pChart->m_oChartSpace.m_printSettings = pNewElem;
		}
		else if(c_oserct_chartspaceEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			pChart->m_oChartSpace.m_extLst = pNewElem;
		}
		else if(c_oserct_chartspaceTHEMEOVERRIDE == type)
		{
            std::wstring sThemeOverrideName      = L"themeOverride" + std::to_wstring(m_oSaveParams.nThemeOverrideCount++) + L".xml";
            std::wstring sThemeOverrideRelsPath  = L"../theme/" + sThemeOverrideName;

            OOX::CPath pathThemeOverrideFile = m_oSaveParams.sThemePath + FILE_SEPARATOR_STR + sThemeOverrideName;

			smart_ptr<PPTX::Theme> pTheme = new PPTX::Theme(NULL);

			pTheme->fromPPTY(&m_oBufferedStream);
			pTheme->isThemeOverride = true;
			
			NSBinPptxRW::CXmlWriter xmlWriter;
			pTheme->toXmlWriter(&xmlWriter);
			
			Writers::DefaultThemeWriter oThemeFile;
			oThemeFile.m_sContent = xmlWriter.GetXmlString();
			oThemeFile.Write(pathThemeOverrideFile.GetPath());

			unsigned int rId;
            m_pOfficeDrawingConverter->WriteRels(std::wstring(_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/themeOverride")), sThemeOverrideRelsPath, std::wstring(), &rId);

            int nIndex = m_oSaveParams.sThemePath.rfind(FILE_SEPARATOR_CHAR); 
                nIndex = m_oSaveParams.sThemePath.rfind(FILE_SEPARATOR_CHAR, nIndex - 1);
			if(-1 != nIndex)
			{
                std::wstring sContentTypesPath = m_oSaveParams.sThemePath.substr(nIndex + 1);
                XmlUtils::replace_all(sContentTypesPath, L"\\", L"/");

				m_oSaveParams.pContentTypes->Registration(L"application/vnd.openxmlformats-officedocument.themeOverride+xml", sContentTypesPath, sThemeOverrideName);
			}
		}
		else if(c_oserct_chartspaceUSERSHAPES == type)
		{			
			OOX::CSystemUtility::CreateDirectories(m_oSaveParams.sDrawingsPath);

			OOX::CPath pathDrawingsRelsDir = m_oSaveParams.sDrawingsPath  + FILE_SEPARATOR_STR + _T("_rels");
			OOX::CSystemUtility::CreateDirectories(pathDrawingsRelsDir.GetPath());

			m_pOfficeDrawingConverter->SetDstContentRels();
			
			OOX::CChartDrawing* pChartDrawing = new OOX::CChartDrawing(NULL);
			READ1_DEF(length, res, this->ReadCT_userShapes, pChartDrawing);

			NSCommon::smart_ptr<OOX::File> pDrawingFile(pChartDrawing);
			pChart->Add(pDrawingFile);
			
			OOX::CPath pathDrawingsRels = pathDrawingsRelsDir.GetPath()  + FILE_SEPARATOR_STR + pChartDrawing->m_sOutputFilename + _T(".rels");
			m_pOfficeDrawingConverter->SaveDstContentRels(pathDrawingsRels.GetPath());
			
			unsigned int rId = 0;
            m_pOfficeDrawingConverter->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/chartUserShapes", L"../drawings/" + pChartDrawing->m_sOutputFilename, std::wstring(), &rId);
			
			pChart->m_oChartSpace.m_userShapes = new CT_RelId;
			pChart->m_oChartSpace.m_userShapes->m_id = new std::wstring;
			*pChart->m_oChartSpace.m_userShapes->m_id = OOX::RId(rId).ToString();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Boolean(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Boolean* poVal = static_cast<CT_Boolean*>(poResult);
		if(c_oserct_booleanVAL == type)
		{
			bool* pNewElem = new bool;
			*pNewElem = m_oBufferedStream.GetBool();
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_SpPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		PPTX::Logic::SpPr* poVal = static_cast<PPTX::Logic::SpPr*>(poResult);
		if(length > 0)
		{
			poVal->m_namespace = L"c";
			long nCurPos = m_oBufferedStream.GetPos();

			BYTE typeRec1 = m_oBufferedStream.GetUChar();
			poVal->fromPPTY(&m_oBufferedStream);

			m_oBufferedStream.Seek(nCurPos + length);
		}
		return res;
	}
	int BinaryChartReader::ReadCT_TxPr(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		PPTX::Logic::TxBody* poVal = static_cast<PPTX::Logic::TxBody*>(poResult);
		if(length > 0)
		{
			poVal->m_name = L"c:txPr";
			long nCurPos = m_oBufferedStream.GetPos();

			BYTE typeRec1 = m_oBufferedStream.GetUChar();
			poVal->fromPPTY(&m_oBufferedStream);

			m_oBufferedStream.Seek(nCurPos + length);
		}
		return res;
	}
	
	int BinaryChartReader::ReadCT_userShapes(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		if(c_oserct_usershapes_COUNT == type)
		{
			long count_shapes = m_oBufferedStream.GetLong();
		}
		else if (c_oserct_usershapes_SHAPE_REL)
		{
			OOX::CChartDrawing* chartDrawing = (OOX::CChartDrawing*)poResult;
			
			OOX::CSizeAnchor *pNewElem = new OOX::CRelSizeAnchor();
			chartDrawing->m_arrItems.push_back(pNewElem);

			READ1_DEF(length, res, this->ReadCT_userShape, pNewElem);
		}
		else if (c_oserct_usershapes_SHAPE_ABS)
		{
			OOX::CChartDrawing* chartDrawing = (OOX::CChartDrawing*)poResult;
			
			OOX::CSizeAnchor *pNewElem = new OOX::CAbsSizeAnchor();
			chartDrawing->m_arrItems.push_back(pNewElem);

			READ1_DEF(length, res, this->ReadCT_userShape, pNewElem);
		}
		return res;
	}	
	int BinaryChartReader::ReadCT_userShape(BYTE type, long length, void* poResult)
	{
		OOX::CSizeAnchor*		pAnchor		= static_cast<OOX::CSizeAnchor*>	(poResult);
		OOX::CAbsSizeAnchor*	pAbsAnchor	= dynamic_cast<OOX::CAbsSizeAnchor*>(pAnchor);
		OOX::CRelSizeAnchor*	pRelAnchor	= dynamic_cast<OOX::CRelSizeAnchor*>(pAnchor);

		if(!pAnchor && (!pRelAnchor || pAbsAnchor)) return c_oSerConstants::ReadUnknown;

		int res = c_oSerConstants::ReadOk;
		if(c_oSer_DrawingType::From == type)
		{
			pAnchor->m_oFrom.Init();
			READ2_DEF_SPREADSHEET(length, res, this->ReadCT_FromTo, pAnchor->m_oFrom.GetPointer());
		}
		else if(c_oSer_DrawingType::To == type)
		{
			pRelAnchor->m_oTo.Init();
			READ2_DEF_SPREADSHEET(length, res, this->ReadCT_FromTo, pRelAnchor->m_oTo.GetPointer());
		}
		else if(c_oSer_DrawingType::Ext == type)
		{
			pAbsAnchor->m_oExt.Init();
			READ2_DEF_SPREADSHEET(length, res, this->ReadCT_Ext, pAbsAnchor->m_oExt.GetPointer());
		}
		else if(c_oSer_DrawingType::pptxDrawing == type)
		{
			pAnchor->m_oElement.Init();

			BYTE typeRec1   = m_oBufferedStream.GetUChar();    // must be 0;
			LONG _e         = m_oBufferedStream.GetPos()   + m_oBufferedStream.GetLong() + 4;

			m_oBufferedStream.Skip(5); // type record (must be 1) + 4 byte - len record

			pAnchor->m_oElement->fromPPTY(&m_oBufferedStream);

			if (!pAnchor->m_oElement->is_init())
				res = c_oSerConstants::ReadUnknown;
		}
		return res;
	}
	int BinaryChartReader::ReadCT_FromTo(BYTE type, long length, void* poResult)
	{
		OOX::CFromTo* pFromTo = static_cast<OOX::CFromTo*>(poResult);

		int res = c_oSerConstants::ReadOk;
		if(c_oSer_DrawingPosType::X == type)
		{
			pFromTo->m_oX.reset(new double);
			*pFromTo->m_oX = m_oBufferedStream.GetDoubleReal();
		}
		if(c_oSer_DrawingPosType::Y == type)
		{
			pFromTo->m_oY.reset(new double);
			*pFromTo->m_oY = m_oBufferedStream.GetDoubleReal();
		}
		return res;
	}
	int BinaryChartReader::ReadCT_Ext(BYTE type, long length, void* poResult)
	{
		OOX::CExt* pExt = static_cast<OOX::CExt*>(poResult);

		int res = c_oSerConstants::ReadOk;
		if(c_oSer_DrawingPosType::X == type)
		{
			pExt->m_oCx.reset(new double);
			*pExt->m_oCx = m_oBufferedStream.GetDoubleReal();
		}
		if(c_oSer_DrawingPosType::Y == type)
		{
			pExt->m_oCy.reset(new double);
			*pExt->m_oCy = m_oBufferedStream.GetDoubleReal();
		}
		return res;
	}

	int BinaryChartReader::ReadCT_PageSetup(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_PageSetup* poVal = static_cast<CT_PageSetup*>(poResult);
		if(c_oserct_pagesetupPAPERSIZE == type)
		{
			unsigned long* pNewElem = new unsigned long;
			*pNewElem = m_oBufferedStream.GetLong();
			poVal->m_paperSize = pNewElem;
		}
		else if(c_oserct_pagesetupPAPERHEIGHT == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_paperHeight = pNewElem;
		}
		else if(c_oserct_pagesetupPAPERWIDTH == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_paperWidth = pNewElem;
		}
		else if(c_oserct_pagesetupFIRSTPAGENUMBER == type)
		{
			unsigned long* pNewElem = new unsigned long;
			*pNewElem = m_oBufferedStream.GetLong();
			poVal->m_firstPageNumber = pNewElem;
		}
		else if(c_oserct_pagesetupORIENTATION == type)
		{
			ST_PageSetupOrientation* pNewElem = new ST_PageSetupOrientation;
			*pNewElem = (ST_PageSetupOrientation)m_oBufferedStream.GetUChar();
			;
			poVal->m_orientation = pNewElem;
		}
		else if(c_oserct_pagesetupBLACKANDWHITE == type)
		{
			bool* pNewElem = new bool;
			*pNewElem = m_oBufferedStream.GetBool();
			poVal->m_blackAndWhite = pNewElem;
		}
		else if(c_oserct_pagesetupDRAFT == type)
		{
			bool* pNewElem = new bool;
			*pNewElem = m_oBufferedStream.GetBool();
			poVal->m_draft = pNewElem;
		}
		else if(c_oserct_pagesetupUSEFIRSTPAGENUMBER == type)
		{
			bool* pNewElem = new bool;
			*pNewElem = m_oBufferedStream.GetBool();
			poVal->m_useFirstPageNumber = pNewElem;
		}
		else if(c_oserct_pagesetupHORIZONTALDPI == type)
		{
			long* pNewElem = new long;
			*pNewElem = m_oBufferedStream.GetLong();
			poVal->m_horizontalDpi = pNewElem;
		}
		else if(c_oserct_pagesetupVERTICALDPI == type)
		{
			long* pNewElem = new long;
			*pNewElem = m_oBufferedStream.GetLong();
			poVal->m_verticalDpi = pNewElem;
		}
		else if(c_oserct_pagesetupCOPIES == type)
		{
			unsigned long* pNewElem = new unsigned long;
			*pNewElem = m_oBufferedStream.GetLong();
			poVal->m_copies = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_PageMargins(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_PageMargins* poVal = static_cast<CT_PageMargins*>(poResult);
		if(c_oserct_pagemarginsL == type)
		{
			double* pNewElem = new double;
			*pNewElem = m_oBufferedStream.GetDoubleReal();
			poVal->m_l = pNewElem;
		}
		else if(c_oserct_pagemarginsR == type)
		{
			double* pNewElem = new double;
			*pNewElem = m_oBufferedStream.GetDoubleReal();
			poVal->m_r = pNewElem;
		}
		else if(c_oserct_pagemarginsT == type)
		{
			double* pNewElem = new double;
			*pNewElem = m_oBufferedStream.GetDoubleReal();
			poVal->m_t = pNewElem;
		}
		else if(c_oserct_pagemarginsB == type)
		{
			double* pNewElem = new double;
			*pNewElem = m_oBufferedStream.GetDoubleReal();
			poVal->m_b = pNewElem;
		}
		else if(c_oserct_pagemarginsHEADER == type)
		{
			double* pNewElem = new double;
			*pNewElem = m_oBufferedStream.GetDoubleReal();
			poVal->m_header = pNewElem;
		}
		else if(c_oserct_pagemarginsFOOTER == type)
		{
			double* pNewElem = new double;
			*pNewElem = m_oBufferedStream.GetDoubleReal();
			poVal->m_footer = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_HeaderFooter(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_HeaderFooter* poVal = static_cast<CT_HeaderFooter*>(poResult);
		if(c_oserct_headerfooterODDHEADER == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_oddHeader = pNewElem;
		}
		else if(c_oserct_headerfooterODDFOOTER == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_oddFooter = pNewElem;
		}
		else if(c_oserct_headerfooterEVENHEADER == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_evenHeader = pNewElem;
		}
		else if(c_oserct_headerfooterEVENFOOTER == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_evenFooter = pNewElem;
		}
		else if(c_oserct_headerfooterFIRSTHEADER == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_firstHeader = pNewElem;
		}
		else if(c_oserct_headerfooterFIRSTFOOTER == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_firstFooter = pNewElem;
		}
		else if(c_oserct_headerfooterALIGNWITHMARGINS == type)
		{
			bool* pNewElem = new bool;
			*pNewElem = m_oBufferedStream.GetBool();
			poVal->m_alignWithMargins = pNewElem;
		}
		else if(c_oserct_headerfooterDIFFERENTODDEVEN == type)
		{
			bool* pNewElem = new bool;
			*pNewElem = m_oBufferedStream.GetBool();
			poVal->m_differentOddEven = pNewElem;
		}
		else if(c_oserct_headerfooterDIFFERENTFIRST == type)
		{
			bool* pNewElem = new bool;
			*pNewElem = m_oBufferedStream.GetBool();
			poVal->m_differentFirst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_PrintSettings(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_PrintSettings* poVal = static_cast<CT_PrintSettings*>(poResult);
		if(c_oserct_printsettingsHEADERFOOTER == type)
		{
			CT_HeaderFooter* pNewElem = new CT_HeaderFooter;
			READ1_DEF(length, res, this->ReadCT_HeaderFooter, pNewElem);
			poVal->m_headerFooter = pNewElem;
		}
		else if(c_oserct_printsettingsPAGEMARGINS == type)
		{
			CT_PageMargins* pNewElem = new CT_PageMargins;
			READ1_DEF(length, res, this->ReadCT_PageMargins, pNewElem);
			poVal->m_pageMargins = pNewElem;
		}
		else if(c_oserct_printsettingsPAGESETUP == type)
		{
			CT_PageSetup* pNewElem = new CT_PageSetup;
			READ1_DEF(length, res, this->ReadCT_PageSetup, pNewElem);
			poVal->m_pageSetup = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ExternalData(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_ExternalData* poVal = static_cast<CT_ExternalData*>(poResult);
		if(c_oserct_externaldataAUTOUPDATE == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_autoUpdate = pNewElem;
		}
		else if(c_oserct_externaldataID == type)
		{
			std::wstring* pNewElem = new std::wstring;
			//todo
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_id = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_DispBlanksAs(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_DispBlanksAs* poVal = static_cast<CT_DispBlanksAs*>(poResult);
		if(c_oserct_dispblanksasVAL == type)
		{
			ST_DispBlanksAs* pNewElem = new ST_DispBlanksAs;
			*pNewElem = (ST_DispBlanksAs)m_oBufferedStream.GetUChar();
			;
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_LegendEntry(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_LegendEntry* poVal = static_cast<CT_LegendEntry*>(poResult);
		if(c_oserct_legendentryIDX == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_idx = pNewElem;
		}
		else if(c_oserct_legendentryDELETE == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_delete = pNewElem;
		}
		else if(c_oserct_legendentryTXPR == type)
		{
			poVal->m_oTxPr = new PPTX::Logic::TxBody;
			res = ReadCT_TxPr(0, length, poVal->m_oTxPr.GetPointer());
		}
		else if(c_oserct_legendentryEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_UnsignedInt(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_UnsignedInt* poVal = static_cast<CT_UnsignedInt*>(poResult);
		if(c_oserct_unsignedintVAL == type)
		{
			unsigned long* pNewElem = new unsigned long;
			*pNewElem = m_oBufferedStream.GetLong();
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Extension(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Extension* poVal = static_cast<CT_Extension*>(poResult);
		if(c_oserct_extensionANY == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_Any = pNewElem;
		}
		else if(c_oserct_extensionURI == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_uri = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_LegendPos(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_LegendPos* poVal = static_cast<CT_LegendPos*>(poResult);
		if(c_oserct_legendposVAL == type)
		{
			ST_LegendPos* pNewElem = new ST_LegendPos;
			*pNewElem = (ST_LegendPos)m_oBufferedStream.GetUChar();
			;
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Legend(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Legend* poVal = static_cast<CT_Legend*>(poResult);
		if(c_oserct_legendLEGENDPOS == type)
		{
			CT_LegendPos* pNewElem = new CT_LegendPos;
			READ1_DEF(length, res, this->ReadCT_LegendPos, pNewElem);
			poVal->m_legendPos = pNewElem;
		}
		else if(c_oserct_legendLEGENDENTRY == type)
		{
			CT_LegendEntry* pNewElem = new CT_LegendEntry;
			READ1_DEF(length, res, this->ReadCT_LegendEntry, pNewElem);
			poVal->m_legendEntry.push_back(pNewElem);
		}
		else if(c_oserct_legendLAYOUT == type)
		{
			CT_Layout* pNewElem = new CT_Layout;
			READ1_DEF(length, res, this->ReadCT_Layout, pNewElem);
			poVal->m_layout = pNewElem;
		}
		else if(c_oserct_legendOVERLAY == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_overlay = pNewElem;
		}
		else if(c_oserct_legendSPPR == type)
		{
			poVal->m_oSpPr = new PPTX::Logic::SpPr;
			ReadCT_SpPr(0, length, poVal->m_oSpPr.GetPointer());
		}
		else if(c_oserct_legendTXPR == type)
		{
			poVal->m_oTxPr = new PPTX::Logic::TxBody;
			res = ReadCT_TxPr(0, length, poVal->m_oTxPr.GetPointer());
		}
		else if(c_oserct_legendEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Layout(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Layout* poVal = static_cast<CT_Layout*>(poResult);
		if(c_oserct_layoutMANUALLAYOUT == type)
		{
			CT_ManualLayout* pNewElem = new CT_ManualLayout;
			READ1_DEF(length, res, this->ReadCT_ManualLayout, pNewElem);
			poVal->m_manualLayout = pNewElem;
		}
		else if(c_oserct_layoutEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ManualLayout(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_ManualLayout* poVal = static_cast<CT_ManualLayout*>(poResult);
		if(c_oserct_manuallayoutLAYOUTTARGET == type)
		{
			CT_LayoutTarget* pNewElem = new CT_LayoutTarget;
			READ1_DEF(length, res, this->ReadCT_LayoutTarget, pNewElem);
			poVal->m_layoutTarget = pNewElem;
		}
		else if(c_oserct_manuallayoutXMODE == type)
		{
			CT_LayoutMode* pNewElem = new CT_LayoutMode;
			READ1_DEF(length, res, this->ReadCT_LayoutMode, pNewElem);
			poVal->m_xMode = pNewElem;
		}
		else if(c_oserct_manuallayoutYMODE == type)
		{
			CT_LayoutMode* pNewElem = new CT_LayoutMode;
			READ1_DEF(length, res, this->ReadCT_LayoutMode, pNewElem);
			poVal->m_yMode = pNewElem;
		}
		else if(c_oserct_manuallayoutWMODE == type)
		{
			CT_LayoutMode* pNewElem = new CT_LayoutMode;
			READ1_DEF(length, res, this->ReadCT_LayoutMode, pNewElem);
			poVal->m_wMode = pNewElem;
		}
		else if(c_oserct_manuallayoutHMODE == type)
		{
			CT_LayoutMode* pNewElem = new CT_LayoutMode;
			READ1_DEF(length, res, this->ReadCT_LayoutMode, pNewElem);
			poVal->m_hMode = pNewElem;
		}
		else if(c_oserct_manuallayoutX == type)
		{
			CT_Double* pNewElem = new CT_Double;
			READ1_DEF(length, res, this->ReadCT_Double, pNewElem);
			poVal->m_x = pNewElem;
		}
		else if(c_oserct_manuallayoutY == type)
		{
			CT_Double* pNewElem = new CT_Double;
			READ1_DEF(length, res, this->ReadCT_Double, pNewElem);
			poVal->m_y = pNewElem;
		}
		else if(c_oserct_manuallayoutW == type)
		{
			CT_Double* pNewElem = new CT_Double;
			READ1_DEF(length, res, this->ReadCT_Double, pNewElem);
			poVal->m_w = pNewElem;
		}
		else if(c_oserct_manuallayoutH == type)
		{
			CT_Double* pNewElem = new CT_Double;
			READ1_DEF(length, res, this->ReadCT_Double, pNewElem);
			poVal->m_h = pNewElem;
		}
		else if(c_oserct_manuallayoutEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_LayoutTarget(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_LayoutTarget* poVal = static_cast<CT_LayoutTarget*>(poResult);
		if(c_oserct_layouttargetVAL == type)
		{
			ST_LayoutTarget* pNewElem = new ST_LayoutTarget;
			*pNewElem = (ST_LayoutTarget)m_oBufferedStream.GetUChar();
			;
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_LayoutMode(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_LayoutMode* poVal = static_cast<CT_LayoutMode*>(poResult);
		if(c_oserct_layoutmodeVAL == type)
		{
			ST_LayoutMode* pNewElem = new ST_LayoutMode;
			*pNewElem = (ST_LayoutMode)m_oBufferedStream.GetUChar();
			;
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Double(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Double* poVal = static_cast<CT_Double*>(poResult);
		if(c_oserct_doubleVAL == type)
		{
			double* pNewElem = new double;
			*pNewElem = m_oBufferedStream.GetDoubleReal();
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_DTable(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_DTable* poVal = static_cast<CT_DTable*>(poResult);
		if(c_oserct_dtableSHOWHORZBORDER == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_showHorzBorder = pNewElem;
		}
		else if(c_oserct_dtableSHOWVERTBORDER == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_showVertBorder = pNewElem;
		}
		else if(c_oserct_dtableSHOWOUTLINE == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_showOutline = pNewElem;
		}
		else if(c_oserct_dtableSHOWKEYS == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_showKeys = pNewElem;
		}
		else if(c_oserct_dtableSPPR == type)
		{
			poVal->m_oSpPr = new PPTX::Logic::SpPr;
			ReadCT_SpPr(0, length, poVal->m_oSpPr.GetPointer());
		}
		else if(c_oserct_dtableTXPR == type)
		{
			poVal->m_oTxPr = new PPTX::Logic::TxBody;
			res = ReadCT_TxPr(0, length, poVal->m_oTxPr.GetPointer());
		}
		else if(c_oserct_dtableEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_SerAx(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_SerAx* poVal = static_cast<CT_SerAx*>(poResult);
		if(c_oserct_seraxAXID == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_axId = pNewElem;
		}
		else if(c_oserct_seraxSCALING == type)
		{
			CT_Scaling* pNewElem = new CT_Scaling;
			READ1_DEF(length, res, this->ReadCT_Scaling, pNewElem);
			poVal->m_scaling = pNewElem;
		}
		else if(c_oserct_seraxDELETE == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_delete = pNewElem;
		}
		else if(c_oserct_seraxAXPOS == type)
		{
			CT_AxPos* pNewElem = new CT_AxPos;
			READ1_DEF(length, res, this->ReadCT_AxPos, pNewElem);
			poVal->m_axPos = pNewElem;
		}
		else if(c_oserct_seraxMAJORGRIDLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_majorGridlines = pNewElem;
		}
		else if(c_oserct_seraxMINORGRIDLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_minorGridlines = pNewElem;
		}
		else if(c_oserct_seraxTITLE == type)
		{
			CT_Title* pNewElem = new CT_Title;
			READ1_DEF(length, res, this->ReadCT_Title, pNewElem);
			poVal->m_title = pNewElem;
		}
		else if(c_oserct_seraxNUMFMT == type)
		{
			CT_NumFmt* pNewElem = new CT_NumFmt;
			READ1_DEF(length, res, this->ReadCT_NumFmt, pNewElem);
			poVal->m_numFmt = pNewElem;
		}
		else if(c_oserct_seraxMAJORTICKMARK == type)
		{
			CT_TickMark* pNewElem = new CT_TickMark;
			READ1_DEF(length, res, this->ReadCT_TickMark, pNewElem);
			poVal->m_majorTickMark = pNewElem;
		}
		else if(c_oserct_seraxMINORTICKMARK == type)
		{
			CT_TickMark* pNewElem = new CT_TickMark;
			READ1_DEF(length, res, this->ReadCT_TickMark, pNewElem);
			poVal->m_minorTickMark = pNewElem;
		}
		else if(c_oserct_seraxTICKLBLPOS == type)
		{
			CT_TickLblPos* pNewElem = new CT_TickLblPos;
			READ1_DEF(length, res, this->ReadCT_TickLblPos, pNewElem);
			poVal->m_tickLblPos = pNewElem;
		}
		else if(c_oserct_seraxSPPR == type)
		{
			poVal->m_oSpPr = new PPTX::Logic::SpPr;
			ReadCT_SpPr(0, length, poVal->m_oSpPr.GetPointer());
		}
		else if(c_oserct_seraxTXPR == type)
		{
			poVal->m_oTxPr = new PPTX::Logic::TxBody;
			res = ReadCT_TxPr(0, length, poVal->m_oTxPr.GetPointer());
		}
		else if(c_oserct_seraxCROSSAX == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_crossAx = pNewElem;
		}
		else if(c_oserct_seraxCROSSES == type)
		{
			CT_Crosses* pNewElem = new CT_Crosses;
			READ1_DEF(length, res, this->ReadCT_Crosses, pNewElem);
			poVal->m_crosses = pNewElem;
		}
		else if(c_oserct_seraxCROSSESAT == type)
		{
			CT_Double* pNewElem = new CT_Double;
			READ1_DEF(length, res, this->ReadCT_Double, pNewElem);
			poVal->m_crossesAt = pNewElem;
		}
		else if(c_oserct_seraxTICKLBLSKIP == type)
		{
			CT_Skip* pNewElem = new CT_Skip;
			READ1_DEF(length, res, this->ReadCT_Skip, pNewElem);
			poVal->m_tickLblSkip = pNewElem;
		}
		else if(c_oserct_seraxTICKMARKSKIP == type)
		{
			CT_Skip* pNewElem = new CT_Skip;
			READ1_DEF(length, res, this->ReadCT_Skip, pNewElem);
			poVal->m_tickMarkSkip = pNewElem;
		}
		else if(c_oserct_seraxEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Scaling(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Scaling* poVal = static_cast<CT_Scaling*>(poResult);
		if(c_oserct_scalingLOGBASE == type)
		{
			CT_LogBase* pNewElem = new CT_LogBase;
			READ1_DEF(length, res, this->ReadCT_LogBase, pNewElem);
			poVal->m_logBase = pNewElem;
		}
		else if(c_oserct_scalingORIENTATION == type)
		{
			CT_Orientation* pNewElem = new CT_Orientation;
			READ1_DEF(length, res, this->ReadCT_Orientation, pNewElem);
			poVal->m_orientation = pNewElem;
		}
		else if(c_oserct_scalingMAX == type)
		{
			CT_Double* pNewElem = new CT_Double;
			READ1_DEF(length, res, this->ReadCT_Double, pNewElem);
			poVal->m_max = pNewElem;
		}
		else if(c_oserct_scalingMIN == type)
		{
			CT_Double* pNewElem = new CT_Double;
			READ1_DEF(length, res, this->ReadCT_Double, pNewElem);
			poVal->m_min = pNewElem;
		}
		else if(c_oserct_scalingEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_LogBase(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_LogBase* poVal = static_cast<CT_LogBase*>(poResult);
		if(c_oserct_logbaseVAL == type)
		{
			double* pNewElem = new double;
			*pNewElem = m_oBufferedStream.GetDoubleReal();
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Orientation(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Orientation* poVal = static_cast<CT_Orientation*>(poResult);
		if(c_oserct_orientationVAL == type)
		{
			ST_Orientation* pNewElem = new ST_Orientation;
			*pNewElem = (ST_Orientation)m_oBufferedStream.GetUChar();
			;
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_AxPos(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_AxPos* poVal = static_cast<CT_AxPos*>(poResult);
		if(c_oserct_axposVAL == type)
		{
			ST_AxPos* pNewElem = new ST_AxPos;
			*pNewElem = (ST_AxPos)m_oBufferedStream.GetUChar();
			;
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartLines(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_ChartLines* poVal = static_cast<CT_ChartLines*>(poResult);
		if(c_oserct_chartlinesSPPR == type)
		{
			poVal->m_oSpPr = new PPTX::Logic::SpPr;
			ReadCT_SpPr(0, length, poVal->m_oSpPr.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Title(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Title* poVal = static_cast<CT_Title*>(poResult);
		if(c_oserct_titleTX == type)
		{
			CT_Tx* pNewElem = new CT_Tx;
			READ1_DEF(length, res, this->ReadCT_Tx, pNewElem);
			poVal->m_tx = pNewElem;
		}
		else if(c_oserct_titleLAYOUT == type)
		{
			CT_Layout* pNewElem = new CT_Layout;
			READ1_DEF(length, res, this->ReadCT_Layout, pNewElem);
			poVal->m_layout = pNewElem;
		}
		else if(c_oserct_titleOVERLAY == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_overlay = pNewElem;
		}
		else if(c_oserct_titleSPPR == type)
		{
			poVal->m_oSpPr = new PPTX::Logic::SpPr;
			ReadCT_SpPr(0, length, poVal->m_oSpPr.GetPointer());
		}
		else if(c_oserct_titleTXPR == type)
		{
			poVal->m_oTxPr = new PPTX::Logic::TxBody;
			res = ReadCT_TxPr(0, length, poVal->m_oTxPr.GetPointer());
		}
		else if(c_oserct_titleEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Tx(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Tx* poVal = static_cast<CT_Tx*>(poResult);
		if(c_oserct_txRICH == type)
		{
			BYTE typeRec1 = m_oBufferedStream.GetUChar();

			poVal->m_oRich = new PPTX::Logic::TxBody;
			poVal->m_oRich->fromPPTY(&m_oBufferedStream);
			
			poVal->m_oRich->m_name = L"c:rich";
		}
		else if(c_oserct_txSTRREF == type)
		{
			CT_StrRef* pNewElem = new CT_StrRef;
			READ1_DEF(length, res, this->ReadCT_StrRef, pNewElem);
			poVal->m_strRef = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_StrRef(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_StrRef* poVal = static_cast<CT_StrRef*>(poResult);
		if(c_oserct_strrefF == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_f = pNewElem;
		}
		else if(c_oserct_strrefSTRCACHE == type)
		{
			CT_StrData* pNewElem = new CT_StrData;
			READ1_DEF(length, res, this->ReadCT_StrData, pNewElem);
			poVal->m_strCache = pNewElem;
		}
		else if(c_oserct_strrefEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_StrData(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_StrData* poVal = static_cast<CT_StrData*>(poResult);
		if(c_oserct_strdataPTCOUNT == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_ptCount = pNewElem;
		}
		else if(c_oserct_strdataPT == type)
		{
			CT_StrVal* pNewElem = new CT_StrVal;
			READ1_DEF(length, res, this->ReadCT_StrVal, pNewElem);
			poVal->m_pt.push_back(pNewElem);
		}
		else if(c_oserct_strdataEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_StrVal(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_StrVal* poVal = static_cast<CT_StrVal*>(poResult);
		if(c_oserct_strvalV == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_v = pNewElem;
		}
		else if(c_oserct_strvalIDX == type)
		{
			unsigned long* pNewElem = new unsigned long;
			*pNewElem = m_oBufferedStream.GetLong();
			poVal->m_idx = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_NumFmt(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_NumFmt* poVal = static_cast<CT_NumFmt*>(poResult);
		if(c_oserct_numfmtFORMATCODE == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_formatCode = pNewElem;
		}
		else if(c_oserct_numfmtSOURCELINKED == type)
		{
			bool* pNewElem = new bool;
			*pNewElem = m_oBufferedStream.GetBool();
			poVal->m_sourceLinked = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_TickMark(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_TickMark* poVal = static_cast<CT_TickMark*>(poResult);
		if(c_oserct_tickmarkVAL == type)
		{
			ST_TickMark* pNewElem = new ST_TickMark;
			*pNewElem = (ST_TickMark)m_oBufferedStream.GetUChar();
			;
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_TickLblPos(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_TickLblPos* poVal = static_cast<CT_TickLblPos*>(poResult);
		if(c_oserct_ticklblposVAL == type)
		{
			ST_TickLblPos* pNewElem = new ST_TickLblPos;
			*pNewElem = (ST_TickLblPos)m_oBufferedStream.GetUChar();
			;
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Crosses(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Crosses* poVal = static_cast<CT_Crosses*>(poResult);
		if(c_oserct_crossesVAL == type)
		{
			ST_Crosses* pNewElem = new ST_Crosses;
			*pNewElem = (ST_Crosses)m_oBufferedStream.GetUChar();
			;
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Skip(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Skip* poVal = static_cast<CT_Skip*>(poResult);
		if(c_oserct_skipVAL == type)
		{
			unsigned long* pNewElem = new unsigned long;
			*pNewElem = m_oBufferedStream.GetLong();
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_TimeUnit(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_TimeUnit* poVal = static_cast<CT_TimeUnit*>(poResult);
		if(c_oserct_timeunitVAL == type)
		{
			ST_TimeUnit* pNewElem = new ST_TimeUnit;
			*pNewElem = (ST_TimeUnit)m_oBufferedStream.GetUChar();
			;
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_DateAx(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_DateAx* poVal = static_cast<CT_DateAx*>(poResult);
		if(c_oserct_dateaxAXID == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_axId = pNewElem;
		}
		else if(c_oserct_dateaxSCALING == type)
		{
			CT_Scaling* pNewElem = new CT_Scaling;
			READ1_DEF(length, res, this->ReadCT_Scaling, pNewElem);
			poVal->m_scaling = pNewElem;
		}
		else if(c_oserct_dateaxDELETE == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_delete = pNewElem;
		}
		else if(c_oserct_dateaxAXPOS == type)
		{
			CT_AxPos* pNewElem = new CT_AxPos;
			READ1_DEF(length, res, this->ReadCT_AxPos, pNewElem);
			poVal->m_axPos = pNewElem;
		}
		else if(c_oserct_dateaxMAJORGRIDLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_majorGridlines = pNewElem;
		}
		else if(c_oserct_dateaxMINORGRIDLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_minorGridlines = pNewElem;
		}
		else if(c_oserct_dateaxTITLE == type)
		{
			CT_Title* pNewElem = new CT_Title;
			READ1_DEF(length, res, this->ReadCT_Title, pNewElem);
			poVal->m_title = pNewElem;
		}
		else if(c_oserct_dateaxNUMFMT == type)
		{
			CT_NumFmt* pNewElem = new CT_NumFmt;
			READ1_DEF(length, res, this->ReadCT_NumFmt, pNewElem);
			poVal->m_numFmt = pNewElem;
		}
		else if(c_oserct_dateaxMAJORTICKMARK == type)
		{
			CT_TickMark* pNewElem = new CT_TickMark;
			READ1_DEF(length, res, this->ReadCT_TickMark, pNewElem);
			poVal->m_majorTickMark = pNewElem;
		}
		else if(c_oserct_dateaxMINORTICKMARK == type)
		{
			CT_TickMark* pNewElem = new CT_TickMark;
			READ1_DEF(length, res, this->ReadCT_TickMark, pNewElem);
			poVal->m_minorTickMark = pNewElem;
		}
		else if(c_oserct_dateaxTICKLBLPOS == type)
		{
			CT_TickLblPos* pNewElem = new CT_TickLblPos;
			READ1_DEF(length, res, this->ReadCT_TickLblPos, pNewElem);
			poVal->m_tickLblPos = pNewElem;
		}
		else if(c_oserct_dateaxSPPR == type)
		{
			poVal->m_oSpPr = new PPTX::Logic::SpPr;
			ReadCT_SpPr(0, length, poVal->m_oSpPr.GetPointer());
		}
		else if(c_oserct_dateaxTXPR == type)
		{
			poVal->m_oTxPr = new PPTX::Logic::TxBody;
			res = ReadCT_TxPr(0, length, poVal->m_oTxPr.GetPointer());
		}
		else if(c_oserct_dateaxCROSSAX == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_crossAx = pNewElem;
		}
		else if(c_oserct_dateaxCROSSES == type)
		{
			CT_Crosses* pNewElem = new CT_Crosses;
			READ1_DEF(length, res, this->ReadCT_Crosses, pNewElem);
			poVal->m_crosses = pNewElem;
		}
		else if(c_oserct_dateaxCROSSESAT == type)
		{
			CT_Double* pNewElem = new CT_Double;
			READ1_DEF(length, res, this->ReadCT_Double, pNewElem);
			poVal->m_crossesAt = pNewElem;
		}
		else if(c_oserct_dateaxAUTO == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_auto = pNewElem;
		}
		else if(c_oserct_dateaxLBLOFFSET == type)
		{
			CT_LblOffset* pNewElem = new CT_LblOffset;
			READ1_DEF(length, res, this->ReadCT_LblOffset, pNewElem);
			poVal->m_lblOffset = pNewElem;
		}
		else if(c_oserct_dateaxBASETIMEUNIT == type)
		{
			CT_TimeUnit* pNewElem = new CT_TimeUnit;
			READ1_DEF(length, res, this->ReadCT_TimeUnit, pNewElem);
			poVal->m_baseTimeUnit = pNewElem;
		}
		else if(c_oserct_dateaxMAJORUNIT == type)
		{
			CT_AxisUnit* pNewElem = new CT_AxisUnit;
			READ1_DEF(length, res, this->ReadCT_AxisUnit, pNewElem);
			poVal->m_majorUnit = pNewElem;
		}
		else if(c_oserct_dateaxMAJORTIMEUNIT == type)
		{
			CT_TimeUnit* pNewElem = new CT_TimeUnit;
			READ1_DEF(length, res, this->ReadCT_TimeUnit, pNewElem);
			poVal->m_majorTimeUnit = pNewElem;
		}
		else if(c_oserct_dateaxMINORUNIT == type)
		{
			CT_AxisUnit* pNewElem = new CT_AxisUnit;
			READ1_DEF(length, res, this->ReadCT_AxisUnit, pNewElem);
			poVal->m_minorUnit = pNewElem;
		}
		else if(c_oserct_dateaxMINORTIMEUNIT == type)
		{
			CT_TimeUnit* pNewElem = new CT_TimeUnit;
			READ1_DEF(length, res, this->ReadCT_TimeUnit, pNewElem);
			poVal->m_minorTimeUnit = pNewElem;
		}
		else if(c_oserct_dateaxEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_LblOffset(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_LblOffset* poVal = static_cast<CT_LblOffset*>(poResult);
		if(c_oserct_lbloffsetVAL == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_AxisUnit(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_AxisUnit* poVal = static_cast<CT_AxisUnit*>(poResult);
		if(c_oserct_axisunitVAL == type)
		{
			double* pNewElem = new double;
			*pNewElem = m_oBufferedStream.GetDoubleReal();
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_LblAlgn(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_LblAlgn* poVal = static_cast<CT_LblAlgn*>(poResult);
		if(c_oserct_lblalgnVAL == type)
		{
			ST_LblAlgn* pNewElem = new ST_LblAlgn;
			*pNewElem = (ST_LblAlgn)m_oBufferedStream.GetUChar();
			;
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_CatAx(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_CatAx* poVal = static_cast<CT_CatAx*>(poResult);
		if(c_oserct_cataxAXID == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_axId = pNewElem;
		}
		else if(c_oserct_cataxSCALING == type)
		{
			CT_Scaling* pNewElem = new CT_Scaling;
			READ1_DEF(length, res, this->ReadCT_Scaling, pNewElem);
			poVal->m_scaling = pNewElem;
		}
		else if(c_oserct_cataxDELETE == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_delete = pNewElem;
		}
		else if(c_oserct_cataxAXPOS == type)
		{
			CT_AxPos* pNewElem = new CT_AxPos;
			READ1_DEF(length, res, this->ReadCT_AxPos, pNewElem);
			poVal->m_axPos = pNewElem;
		}
		else if(c_oserct_cataxMAJORGRIDLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_majorGridlines = pNewElem;
		}
		else if(c_oserct_cataxMINORGRIDLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_minorGridlines = pNewElem;
		}
		else if(c_oserct_cataxTITLE == type)
		{
			CT_Title* pNewElem = new CT_Title;
			READ1_DEF(length, res, this->ReadCT_Title, pNewElem);
			poVal->m_title = pNewElem;
		}
		else if(c_oserct_cataxNUMFMT == type)
		{
			CT_NumFmt* pNewElem = new CT_NumFmt;
			READ1_DEF(length, res, this->ReadCT_NumFmt, pNewElem);
			poVal->m_numFmt = pNewElem;
		}
		else if(c_oserct_cataxMAJORTICKMARK == type)
		{
			CT_TickMark* pNewElem = new CT_TickMark;
			READ1_DEF(length, res, this->ReadCT_TickMark, pNewElem);
			poVal->m_majorTickMark = pNewElem;
		}
		else if(c_oserct_cataxMINORTICKMARK == type)
		{
			CT_TickMark* pNewElem = new CT_TickMark;
			READ1_DEF(length, res, this->ReadCT_TickMark, pNewElem);
			poVal->m_minorTickMark = pNewElem;
		}
		else if(c_oserct_cataxTICKLBLPOS == type)
		{
			CT_TickLblPos* pNewElem = new CT_TickLblPos;
			READ1_DEF(length, res, this->ReadCT_TickLblPos, pNewElem);
			poVal->m_tickLblPos = pNewElem;
		}
		else if(c_oserct_cataxSPPR == type)
		{
			poVal->m_oSpPr = new PPTX::Logic::SpPr;
			ReadCT_SpPr(0, length, poVal->m_oSpPr.GetPointer());
		}
		else if(c_oserct_cataxTXPR == type)
		{
			poVal->m_oTxPr = new PPTX::Logic::TxBody;
			res = ReadCT_TxPr(0, length, poVal->m_oTxPr.GetPointer());
		}
		else if(c_oserct_cataxCROSSAX == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_crossAx = pNewElem;
		}
		else if(c_oserct_cataxCROSSES == type)
		{
			CT_Crosses* pNewElem = new CT_Crosses;
			READ1_DEF(length, res, this->ReadCT_Crosses, pNewElem);
			poVal->m_crosses = pNewElem;
		}
		else if(c_oserct_cataxCROSSESAT == type)
		{
			CT_Double* pNewElem = new CT_Double;
			READ1_DEF(length, res, this->ReadCT_Double, pNewElem);
			poVal->m_crossesAt = pNewElem;
		}
		else if(c_oserct_cataxAUTO == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_auto = pNewElem;
		}
		else if(c_oserct_cataxLBLALGN == type)
		{
			CT_LblAlgn* pNewElem = new CT_LblAlgn;
			READ1_DEF(length, res, this->ReadCT_LblAlgn, pNewElem);
			poVal->m_lblAlgn = pNewElem;
		}
		else if(c_oserct_cataxLBLOFFSET == type)
		{
			CT_LblOffset* pNewElem = new CT_LblOffset;
			READ1_DEF(length, res, this->ReadCT_LblOffset, pNewElem);
			poVal->m_lblOffset = pNewElem;
		}
		else if(c_oserct_cataxTICKLBLSKIP == type)
		{
			CT_Skip* pNewElem = new CT_Skip;
			READ1_DEF(length, res, this->ReadCT_Skip, pNewElem);
			poVal->m_tickLblSkip = pNewElem;
		}
		else if(c_oserct_cataxTICKMARKSKIP == type)
		{
			CT_Skip* pNewElem = new CT_Skip;
			READ1_DEF(length, res, this->ReadCT_Skip, pNewElem);
			poVal->m_tickMarkSkip = pNewElem;
		}
		else if(c_oserct_cataxNOMULTILVLLBL == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_noMultiLvlLbl = pNewElem;
		}
		else if(c_oserct_cataxEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_DispUnitsLbl(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_DispUnitsLbl* poVal = static_cast<CT_DispUnitsLbl*>(poResult);
		if(c_oserct_dispunitslblLAYOUT == type)
		{
			CT_Layout* pNewElem = new CT_Layout;
			READ1_DEF(length, res, this->ReadCT_Layout, pNewElem);
			poVal->m_layout = pNewElem;
		}
		else if(c_oserct_dispunitslblTX == type)
		{
			CT_Tx* pNewElem = new CT_Tx;
			READ1_DEF(length, res, this->ReadCT_Tx, pNewElem);
			poVal->m_tx = pNewElem;
		}
		else if(c_oserct_dispunitslblSPPR == type)
		{
			poVal->m_oSpPr = new PPTX::Logic::SpPr;
			ReadCT_SpPr(0, length, poVal->m_oSpPr.GetPointer());
		}
		else if(c_oserct_dispunitslblTXPR == type)
		{
			poVal->m_oTxPr = new PPTX::Logic::TxBody;
			res = ReadCT_TxPr(0, length, poVal->m_oTxPr.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_BuiltInUnit(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_BuiltInUnit* poVal = static_cast<CT_BuiltInUnit*>(poResult);
		if(c_oserct_builtinunitVAL == type)
		{
			ST_BuiltInUnit* pNewElem = new ST_BuiltInUnit;
			*pNewElem = (ST_BuiltInUnit)m_oBufferedStream.GetUChar();
			;
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_DispUnits(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_DispUnits* poVal = static_cast<CT_DispUnits*>(poResult);
		if(c_oserct_dispunitsBUILTINUNIT == type)
		{
			CT_BuiltInUnit* pNewElem = new CT_BuiltInUnit;
			READ1_DEF(length, res, this->ReadCT_BuiltInUnit, pNewElem);
			poVal->m_builtInUnit = pNewElem;
		}
		else if(c_oserct_dispunitsCUSTUNIT == type)
		{
			CT_Double* pNewElem = new CT_Double;
			READ1_DEF(length, res, this->ReadCT_Double, pNewElem);
			poVal->m_custUnit = pNewElem;
		}
		else if(c_oserct_dispunitsDISPUNITSLBL == type)
		{
			CT_DispUnitsLbl* pNewElem = new CT_DispUnitsLbl;
			READ1_DEF(length, res, this->ReadCT_DispUnitsLbl, pNewElem);
			poVal->m_dispUnitsLbl = pNewElem;
		}
		else if(c_oserct_dispunitsEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_CrossBetween(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_CrossBetween* poVal = static_cast<CT_CrossBetween*>(poResult);
		if(c_oserct_crossbetweenVAL == type)
		{
			ST_CrossBetween* pNewElem = new ST_CrossBetween;
			*pNewElem = (ST_CrossBetween)m_oBufferedStream.GetUChar();
			;
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ValAx(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_ValAx* poVal = static_cast<CT_ValAx*>(poResult);
		if(c_oserct_valaxAXID == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_axId = pNewElem;
		}
		else if(c_oserct_valaxSCALING == type)
		{
			CT_Scaling* pNewElem = new CT_Scaling;
			READ1_DEF(length, res, this->ReadCT_Scaling, pNewElem);
			poVal->m_scaling = pNewElem;
		}
		else if(c_oserct_valaxDELETE == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_delete = pNewElem;
		}
		else if(c_oserct_valaxAXPOS == type)
		{
			CT_AxPos* pNewElem = new CT_AxPos;
			READ1_DEF(length, res, this->ReadCT_AxPos, pNewElem);
			poVal->m_axPos = pNewElem;
		}
		else if(c_oserct_valaxMAJORGRIDLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_majorGridlines = pNewElem;
		}
		else if(c_oserct_valaxMINORGRIDLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_minorGridlines = pNewElem;
		}
		else if(c_oserct_valaxTITLE == type)
		{
			CT_Title* pNewElem = new CT_Title;
			READ1_DEF(length, res, this->ReadCT_Title, pNewElem);
			poVal->m_title = pNewElem;
		}
		else if(c_oserct_valaxNUMFMT == type)
		{
			CT_NumFmt* pNewElem = new CT_NumFmt;
			READ1_DEF(length, res, this->ReadCT_NumFmt, pNewElem);
			poVal->m_numFmt = pNewElem;
		}
		else if(c_oserct_valaxMAJORTICKMARK == type)
		{
			CT_TickMark* pNewElem = new CT_TickMark;
			READ1_DEF(length, res, this->ReadCT_TickMark, pNewElem);
			poVal->m_majorTickMark = pNewElem;
		}
		else if(c_oserct_valaxMINORTICKMARK == type)
		{
			CT_TickMark* pNewElem = new CT_TickMark;
			READ1_DEF(length, res, this->ReadCT_TickMark, pNewElem);
			poVal->m_minorTickMark = pNewElem;
		}
		else if(c_oserct_valaxTICKLBLPOS == type)
		{
			CT_TickLblPos* pNewElem = new CT_TickLblPos;
			READ1_DEF(length, res, this->ReadCT_TickLblPos, pNewElem);
			poVal->m_tickLblPos = pNewElem;
		}
		else if(c_oserct_valaxSPPR == type)
		{
			poVal->m_oSpPr = new PPTX::Logic::SpPr;
			ReadCT_SpPr(0, length, poVal->m_oSpPr.GetPointer());
		}
		else if(c_oserct_valaxTXPR == type)
		{
			poVal->m_oTxPr = new PPTX::Logic::TxBody;
			res = ReadCT_TxPr(0, length, poVal->m_oTxPr.GetPointer());
		}
		else if(c_oserct_valaxCROSSAX == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_crossAx = pNewElem;
		}
		else if(c_oserct_valaxCROSSES == type)
		{
			CT_Crosses* pNewElem = new CT_Crosses;
			READ1_DEF(length, res, this->ReadCT_Crosses, pNewElem);
			poVal->m_crosses = pNewElem;
		}
		else if(c_oserct_valaxCROSSESAT == type)
		{
			CT_Double* pNewElem = new CT_Double;
			READ1_DEF(length, res, this->ReadCT_Double, pNewElem);
			poVal->m_crossesAt = pNewElem;
		}
		else if(c_oserct_valaxCROSSBETWEEN == type)
		{
			CT_CrossBetween* pNewElem = new CT_CrossBetween;
			READ1_DEF(length, res, this->ReadCT_CrossBetween, pNewElem);
			poVal->m_crossBetween = pNewElem;
		}
		else if(c_oserct_valaxMAJORUNIT == type)
		{
			CT_AxisUnit* pNewElem = new CT_AxisUnit;
			READ1_DEF(length, res, this->ReadCT_AxisUnit, pNewElem);
			poVal->m_majorUnit = pNewElem;
		}
		else if(c_oserct_valaxMINORUNIT == type)
		{
			CT_AxisUnit* pNewElem = new CT_AxisUnit;
			READ1_DEF(length, res, this->ReadCT_AxisUnit, pNewElem);
			poVal->m_minorUnit = pNewElem;
		}
		else if(c_oserct_valaxDISPUNITS == type)
		{
			CT_DispUnits* pNewElem = new CT_DispUnits;
			READ1_DEF(length, res, this->ReadCT_DispUnits, pNewElem);
			poVal->m_dispUnits = pNewElem;
		}
		else if(c_oserct_valaxEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_SizeRepresents(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_SizeRepresents* poVal = static_cast<CT_SizeRepresents*>(poResult);
		if(c_oserct_sizerepresentsVAL == type)
		{
			ST_SizeRepresents* pNewElem = new ST_SizeRepresents;
			*pNewElem = (ST_SizeRepresents)m_oBufferedStream.GetUChar();
			;
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_BubbleScale(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_BubbleScale* poVal = static_cast<CT_BubbleScale*>(poResult);
		if(c_oserct_bubblescaleVAL == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_BubbleSer(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_BubbleSer* poVal = static_cast<CT_BubbleSer*>(poResult);
		if(c_oserct_bubbleserIDX == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_idx = pNewElem;
		}
		else if(c_oserct_bubbleserORDER == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_order = pNewElem;
		}
		else if(c_oserct_bubbleserTX == type)
		{
			CT_SerTx* pNewElem = new CT_SerTx;
			READ1_DEF(length, res, this->ReadCT_SerTx, pNewElem);
			poVal->m_tx = pNewElem;
		}
		else if(c_oserct_bubbleserSPPR == type)
		{
			poVal->m_oSpPr = new PPTX::Logic::SpPr;
			ReadCT_SpPr(0, length, poVal->m_oSpPr.GetPointer());
		}
		else if(c_oserct_bubbleserINVERTIFNEGATIVE == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_invertIfNegative = pNewElem;
		}
		else if(c_oserct_bubbleserDPT == type)
		{
			CT_DPt* pNewElem = new CT_DPt;
			READ1_DEF(length, res, this->ReadCT_DPt, pNewElem);
			poVal->m_dPt.push_back(pNewElem);
		}
		else if(c_oserct_bubbleserDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if(c_oserct_bubbleserTRENDLINE == type)
		{
			CT_Trendline* pNewElem = new CT_Trendline;
			READ1_DEF(length, res, this->ReadCT_Trendline, pNewElem);
			poVal->m_trendline.push_back(pNewElem);
		}
		else if(c_oserct_bubbleserERRBARS == type)
		{
			CT_ErrBars* pNewElem = new CT_ErrBars;
			READ1_DEF(length, res, this->ReadCT_ErrBars, pNewElem);
			poVal->m_errBars.push_back(pNewElem);
		}
		else if(c_oserct_bubbleserXVAL == type)
		{
			CT_AxDataSource* pNewElem = new CT_AxDataSource;
			READ1_DEF(length, res, this->ReadCT_AxDataSource, pNewElem);
			poVal->m_xVal = pNewElem;
		}
		else if(c_oserct_bubbleserYVAL == type)
		{
			CT_NumDataSource* pNewElem = new CT_NumDataSource;
			READ1_DEF(length, res, this->ReadCT_NumDataSource, pNewElem);
			poVal->m_yVal = pNewElem;
		}
		else if(c_oserct_bubbleserBUBBLESIZE == type)
		{
			CT_NumDataSource* pNewElem = new CT_NumDataSource;
			READ1_DEF(length, res, this->ReadCT_NumDataSource, pNewElem);
			poVal->m_bubbleSize = pNewElem;
		}
		else if(c_oserct_bubbleserBUBBLE3D == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_bubble3D = pNewElem;
		}
		else if(c_oserct_bubbleserEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_SerTx(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_SerTx* poVal = static_cast<CT_SerTx*>(poResult);
		if(c_oserct_sertxSTRREF == type)
		{
			CT_StrRef* pNewElem = new CT_StrRef;
			READ1_DEF(length, res, this->ReadCT_StrRef, pNewElem);
			poVal->m_strRef = pNewElem;
		}
		else if(c_oserct_sertxV == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_v = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_DPt(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_DPt* poVal = static_cast<CT_DPt*>(poResult);
		if(c_oserct_dptIDX == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_idx = pNewElem;
		}
		else if(c_oserct_dptINVERTIFNEGATIVE == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_invertIfNegative = pNewElem;
		}
		else if(c_oserct_dptMARKER == type)
		{
			CT_Marker* pNewElem = new CT_Marker;
			READ1_DEF(length, res, this->ReadCT_Marker, pNewElem);
			poVal->m_marker = pNewElem;
		}
		else if(c_oserct_dptBUBBLE3D == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_bubble3D = pNewElem;
		}
		else if(c_oserct_dptEXPLOSION == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_explosion = pNewElem;
		}
		else if(c_oserct_dptSPPR == type)
		{
			poVal->m_oSpPr = new PPTX::Logic::SpPr;
			ReadCT_SpPr(0, length, poVal->m_oSpPr.GetPointer());
		}
		else if(c_oserct_dptPICTUREOPTIONS == type)
		{
			CT_PictureOptions* pNewElem = new CT_PictureOptions;
			READ1_DEF(length, res, this->ReadCT_PictureOptions, pNewElem);
			poVal->m_pictureOptions = pNewElem;
		}
		else if(c_oserct_dptEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Marker(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Marker* poVal = static_cast<CT_Marker*>(poResult);
		if(c_oserct_markerSYMBOL == type)
		{
			CT_MarkerStyle* pNewElem = new CT_MarkerStyle;
			READ1_DEF(length, res, this->ReadCT_MarkerStyle, pNewElem);
			poVal->m_symbol = pNewElem;
		}
		else if(c_oserct_markerSIZE == type)
		{
			CT_MarkerSize* pNewElem = new CT_MarkerSize;
			READ1_DEF(length, res, this->ReadCT_MarkerSize, pNewElem);
			poVal->m_size = pNewElem;
		}
		else if(c_oserct_markerSPPR == type)
		{
			poVal->m_oSpPr = new PPTX::Logic::SpPr;
			ReadCT_SpPr(0, length, poVal->m_oSpPr.GetPointer());
		}
		else if(c_oserct_markerEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_MarkerStyle(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_MarkerStyle* poVal = static_cast<CT_MarkerStyle*>(poResult);
		if(c_oserct_markerstyleVAL == type)
		{
			ST_MarkerStyle* pNewElem = new ST_MarkerStyle;
			*pNewElem = (ST_MarkerStyle)m_oBufferedStream.GetUChar();
			;
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_MarkerSize(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_MarkerSize* poVal = static_cast<CT_MarkerSize*>(poResult);
		if(c_oserct_markersizeVAL == type)
		{
			unsigned char* pNewElem = new unsigned char;
			*pNewElem = m_oBufferedStream.GetUChar();
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_PictureOptions(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_PictureOptions* poVal = static_cast<CT_PictureOptions*>(poResult);
		if(c_oserct_pictureoptionsAPPLYTOFRONT == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_applyToFront = pNewElem;
		}
		else if(c_oserct_pictureoptionsAPPLYTOSIDES == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_applyToSides = pNewElem;
		}
		else if(c_oserct_pictureoptionsAPPLYTOEND == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_applyToEnd = pNewElem;
		}
		else if(c_oserct_pictureoptionsPICTUREFORMAT == type)
		{
			CT_PictureFormat* pNewElem = new CT_PictureFormat;
			READ1_DEF(length, res, this->ReadCT_PictureFormat, pNewElem);
			poVal->m_pictureFormat = pNewElem;
		}
		else if(c_oserct_pictureoptionsPICTURESTACKUNIT == type)
		{
			CT_PictureStackUnit* pNewElem = new CT_PictureStackUnit;
			READ1_DEF(length, res, this->ReadCT_PictureStackUnit, pNewElem);
			poVal->m_pictureStackUnit = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_PictureFormat(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_PictureFormat* poVal = static_cast<CT_PictureFormat*>(poResult);
		if(c_oserct_pictureformatVAL == type)
		{
			ST_PictureFormat* pNewElem = new ST_PictureFormat;
			*pNewElem = (ST_PictureFormat)m_oBufferedStream.GetUChar();
			;
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_PictureStackUnit(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_PictureStackUnit* poVal = static_cast<CT_PictureStackUnit*>(poResult);
		if(c_oserct_picturestackunitVAL == type)
		{
			double* pNewElem = new double;
			*pNewElem = m_oBufferedStream.GetDoubleReal();
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_DLbls(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_DLbls* poVal = static_cast<CT_DLbls*>(poResult);
		if(c_oserct_dlblsDLBL == type)
		{
			CT_DLbl* pNewElem = new CT_DLbl;
			READ1_DEF(length, res, this->ReadCT_DLbl, pNewElem);
			poVal->m_dLbl.push_back(pNewElem);
		}
		else if(c_oserct_dlblsDLBLPOS == type)
		{
			CT_DLblPos* pNewElem = new CT_DLblPos;
			READ1_DEF(length, res, this->ReadCT_DLblPos, pNewElem);
			ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
			*eElemtype = itemschoicetype3DLBLPOS;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_dlblsDELETE == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
			*eElemtype = itemschoicetype3DELETE;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_dlblsLEADERLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
			*eElemtype = itemschoicetype3LEADERLINES;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_dlblsNUMFMT == type)
		{
			CT_NumFmt* pNewElem = new CT_NumFmt;
			READ1_DEF(length, res, this->ReadCT_NumFmt, pNewElem);
			ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
			*eElemtype = itemschoicetype3NUMFMT;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_dlblsSEPARATOR == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
			*eElemtype = itemschoicetype3SEPARATOR;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_dlblsSHOWBUBBLESIZE == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
			*eElemtype = itemschoicetype3SHOWBUBBLESIZE;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_dlblsSHOWCATNAME == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
			*eElemtype = itemschoicetype3SHOWCATNAME;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_dlblsSHOWLEADERLINES == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
			*eElemtype = itemschoicetype3SHOWLEADERLINES;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_dlblsSHOWLEGENDKEY == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
			*eElemtype = itemschoicetype3SHOWLEGENDKEY;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_dlblsSHOWPERCENT == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
			*eElemtype = itemschoicetype3SHOWPERCENT;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_dlblsSHOWSERNAME == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
			*eElemtype = itemschoicetype3SHOWSERNAME;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_dlblsSHOWVAL == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_Items.push_back(pNewElem);
			
			ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
			*eElemtype = itemschoicetype3SHOWVAL;
			poVal->m_ItemsElementName0.push_back(eElemtype);
		}
		else if(c_oserct_dlblsSPPR == type)
		{
			ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
			*eElemtype = itemschoicetype3SPPR;
			poVal->m_ItemsElementName0.push_back(eElemtype);

			PPTX::Logic::SpPr * pNewElem = new PPTX::Logic::SpPr();
			res = ReadCT_SpPr(0, length, pNewElem);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_dlblsTXPR == type)
		{
			ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
			*eElemtype = itemschoicetype3TXPR;
			poVal->m_ItemsElementName0.push_back(eElemtype);

			PPTX::Logic::TxBody * pNewElem = new PPTX::Logic::TxBody();
			res = ReadCT_TxPr(0, length, pNewElem);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_dlblsEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_DLbl(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_DLbl* poVal = static_cast<CT_DLbl*>(poResult);
		if(c_oserct_dlblIDX == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_idx = pNewElem;
		}
		else if(c_oserct_dlblDLBLPOS == type)
		{
			CT_DLblPos* pNewElem = new CT_DLblPos;
			READ1_DEF(length, res, this->ReadCT_DLblPos, pNewElem);
			ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
			*eElemtype = itemschoicetype4DLBLPOS;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_dlblDELETE == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
			*eElemtype = itemschoicetype4DELETE;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_dlblLAYOUT == type)
		{
			CT_Layout* pNewElem = new CT_Layout;
			READ1_DEF(length, res, this->ReadCT_Layout, pNewElem);
			ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
			*eElemtype = itemschoicetype4LAYOUT;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_dlblNUMFMT == type)
		{
			CT_NumFmt* pNewElem = new CT_NumFmt;
			READ1_DEF(length, res, this->ReadCT_NumFmt, pNewElem);
			ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
			*eElemtype = itemschoicetype4NUMFMT;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_dlblSEPARATOR == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
			*eElemtype = itemschoicetype4SEPARATOR;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_dlblSHOWBUBBLESIZE == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
			*eElemtype = itemschoicetype4SHOWBUBBLESIZE;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_dlblSHOWCATNAME == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
			*eElemtype = itemschoicetype4SHOWCATNAME;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_dlblSHOWLEGENDKEY == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
			*eElemtype = itemschoicetype4SHOWLEGENDKEY;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_dlblSHOWPERCENT == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
			*eElemtype = itemschoicetype4SHOWPERCENT;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_dlblSHOWSERNAME == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
			*eElemtype = itemschoicetype4SHOWSERNAME;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_dlblSHOWVAL == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
			*eElemtype = itemschoicetype4SHOWVAL;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_dlblSPPR == type)
		{
			ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
			*eElemtype = itemschoicetype4SPPR;
			poVal->m_ItemsElementName0.push_back(eElemtype);

			PPTX::Logic::SpPr *pNewElem = new PPTX::Logic::SpPr;
			res = ReadCT_SpPr(0, length, pNewElem);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_dlblTX == type)
		{
			CT_Tx* pNewElem = new CT_Tx;
			READ1_DEF(length, res, this->ReadCT_Tx, pNewElem);
			ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
			*eElemtype = itemschoicetype4TX;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_dlblTXPR == type)
		{
			ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
			*eElemtype = itemschoicetype4TXPR;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			
			PPTX::Logic::TxBody * pNewElem = new PPTX::Logic::TxBody();
			res = ReadCT_TxPr(0, length, pNewElem);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_dlblEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_DLblPos(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_DLblPos* poVal = static_cast<CT_DLblPos*>(poResult);
		if(c_oserct_dlblposVAL == type)
		{
			ST_DLblPos* pNewElem = new ST_DLblPos;
			*pNewElem = (ST_DLblPos)m_oBufferedStream.GetUChar();
			;
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Trendline(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Trendline* poVal = static_cast<CT_Trendline*>(poResult);
		if(c_oserct_trendlineNAME == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_name = pNewElem;
		}
		else if(c_oserct_trendlineSPPR == type)
		{
			poVal->m_oSpPr = new PPTX::Logic::SpPr;
			ReadCT_SpPr(0, length, poVal->m_oSpPr.GetPointer());
		}
		else if(c_oserct_trendlineTRENDLINETYPE == type)
		{
			CT_TrendlineType* pNewElem = new CT_TrendlineType;
			READ1_DEF(length, res, this->ReadCT_TrendlineType, pNewElem);
			poVal->m_trendlineType = pNewElem;
		}
		else if(c_oserct_trendlineORDER == type)
		{
			CT_Order* pNewElem = new CT_Order;
			READ1_DEF(length, res, this->ReadCT_Order, pNewElem);
			poVal->m_order = pNewElem;
		}
		else if(c_oserct_trendlinePERIOD == type)
		{
			CT_Period* pNewElem = new CT_Period;
			READ1_DEF(length, res, this->ReadCT_Period, pNewElem);
			poVal->m_period = pNewElem;
		}
		else if(c_oserct_trendlineFORWARD == type)
		{
			CT_Double* pNewElem = new CT_Double;
			READ1_DEF(length, res, this->ReadCT_Double, pNewElem);
			poVal->m_forward = pNewElem;
		}
		else if(c_oserct_trendlineBACKWARD == type)
		{
			CT_Double* pNewElem = new CT_Double;
			READ1_DEF(length, res, this->ReadCT_Double, pNewElem);
			poVal->m_backward = pNewElem;
		}
		else if(c_oserct_trendlineINTERCEPT == type)
		{
			CT_Double* pNewElem = new CT_Double;
			READ1_DEF(length, res, this->ReadCT_Double, pNewElem);
			poVal->m_intercept = pNewElem;
		}
		else if(c_oserct_trendlineDISPRSQR == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_dispRSqr = pNewElem;
		}
		else if(c_oserct_trendlineDISPEQ == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_dispEq = pNewElem;
		}
		else if(c_oserct_trendlineTRENDLINELBL == type)
		{
			CT_TrendlineLbl* pNewElem = new CT_TrendlineLbl;
			READ1_DEF(length, res, this->ReadCT_TrendlineLbl, pNewElem);
			poVal->m_trendlineLbl = pNewElem;
		}
		else if(c_oserct_trendlineEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_TrendlineType(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_TrendlineType* poVal = static_cast<CT_TrendlineType*>(poResult);
		if(c_oserct_trendlinetypeVAL == type)
		{
			ST_TrendlineType* pNewElem = new ST_TrendlineType;
			*pNewElem = (ST_TrendlineType)m_oBufferedStream.GetUChar();
			;
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Order(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Order* poVal = static_cast<CT_Order*>(poResult);
		if(c_oserct_orderVAL == type)
		{
			unsigned char* pNewElem = new unsigned char;
			*pNewElem = m_oBufferedStream.GetUChar();
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Period(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Period* poVal = static_cast<CT_Period*>(poResult);
		if(c_oserct_periodVAL == type)
		{
			unsigned long* pNewElem = new unsigned long;
			*pNewElem = m_oBufferedStream.GetLong();
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_TrendlineLbl(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_TrendlineLbl* poVal = static_cast<CT_TrendlineLbl*>(poResult);
		if(c_oserct_trendlinelblLAYOUT == type)
		{
			CT_Layout* pNewElem = new CT_Layout;
			READ1_DEF(length, res, this->ReadCT_Layout, pNewElem);
			poVal->m_layout = pNewElem;
		}
		else if(c_oserct_trendlinelblTX == type)
		{
			CT_Tx* pNewElem = new CT_Tx;
			READ1_DEF(length, res, this->ReadCT_Tx, pNewElem);
			poVal->m_tx = pNewElem;
		}
		else if(c_oserct_trendlinelblNUMFMT == type)
		{
			CT_NumFmt* pNewElem = new CT_NumFmt;
			READ1_DEF(length, res, this->ReadCT_NumFmt, pNewElem);
			poVal->m_numFmt = pNewElem;
		}
		else if(c_oserct_trendlinelblSPPR == type)
		{
			poVal->m_oSpPr = new PPTX::Logic::SpPr;
			ReadCT_SpPr(0, length, poVal->m_oSpPr.GetPointer());
		}
		else if(c_oserct_trendlinelblTXPR == type)
		{
			poVal->m_oTxPr = new PPTX::Logic::TxBody;
			res = ReadCT_TxPr(0, length, poVal->m_oTxPr.GetPointer());
		}
		else if(c_oserct_trendlinelblEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ErrBars(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_ErrBars* poVal = static_cast<CT_ErrBars*>(poResult);
		if(c_oserct_errbarsERRDIR == type)
		{
			CT_ErrDir* pNewElem = new CT_ErrDir;
			READ1_DEF(length, res, this->ReadCT_ErrDir, pNewElem);
			poVal->m_errDir = pNewElem;
		}
		else if(c_oserct_errbarsERRBARTYPE == type)
		{
			CT_ErrBarType* pNewElem = new CT_ErrBarType;
			READ1_DEF(length, res, this->ReadCT_ErrBarType, pNewElem);
			poVal->m_errBarType = pNewElem;
		}
		else if(c_oserct_errbarsERRVALTYPE == type)
		{
			CT_ErrValType* pNewElem = new CT_ErrValType;
			READ1_DEF(length, res, this->ReadCT_ErrValType, pNewElem);
			poVal->m_errValType = pNewElem;
		}
		else if(c_oserct_errbarsNOENDCAP == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_noEndCap = pNewElem;
		}
		else if(c_oserct_errbarsPLUS == type)
		{
			CT_NumDataSource* pNewElem = new CT_NumDataSource;
			READ1_DEF(length, res, this->ReadCT_NumDataSource, pNewElem);
			poVal->m_plus = pNewElem;
		}
		else if(c_oserct_errbarsMINUS == type)
		{
			CT_NumDataSource* pNewElem = new CT_NumDataSource;
			READ1_DEF(length, res, this->ReadCT_NumDataSource, pNewElem);
			poVal->m_minus = pNewElem;
		}
		else if(c_oserct_errbarsVAL == type)
		{
			CT_Double* pNewElem = new CT_Double;
			READ1_DEF(length, res, this->ReadCT_Double, pNewElem);
			poVal->m_val = pNewElem;
		}
		else if(c_oserct_errbarsSPPR == type)
		{
			poVal->m_oSpPr = new PPTX::Logic::SpPr;
			ReadCT_SpPr(0, length, poVal->m_oSpPr.GetPointer());
		}
		else if(c_oserct_errbarsEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ErrDir(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_ErrDir* poVal = static_cast<CT_ErrDir*>(poResult);
		if(c_oserct_errdirVAL == type)
		{
			ST_ErrDir* pNewElem = new ST_ErrDir;
			*pNewElem = (ST_ErrDir)m_oBufferedStream.GetUChar();
			;
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ErrBarType(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_ErrBarType* poVal = static_cast<CT_ErrBarType*>(poResult);
		if(c_oserct_errbartypeVAL == type)
		{
			ST_ErrBarType* pNewElem = new ST_ErrBarType;
			*pNewElem = (ST_ErrBarType)m_oBufferedStream.GetUChar();
			;
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ErrValType(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_ErrValType* poVal = static_cast<CT_ErrValType*>(poResult);
		if(c_oserct_errvaltypeVAL == type)
		{
			ST_ErrValType* pNewElem = new ST_ErrValType;
			*pNewElem = (ST_ErrValType)m_oBufferedStream.GetUChar();
			;
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_NumDataSource(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_NumDataSource* poVal = static_cast<CT_NumDataSource*>(poResult);
		if(c_oserct_numdatasourceNUMLIT == type)
		{
			CT_NumData* pNewElem = new CT_NumData;
			READ1_DEF(length, res, this->ReadCT_NumData, pNewElem);
			poVal->m_numLit = pNewElem;
		}
		else if(c_oserct_numdatasourceNUMREF == type)
		{
			CT_NumRef* pNewElem = new CT_NumRef;
			READ1_DEF(length, res, this->ReadCT_NumRef, pNewElem);
			poVal->m_numRef = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_NumData(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_NumData* poVal = static_cast<CT_NumData*>(poResult);
		if(c_oserct_numdataFORMATCODE == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_formatCode = pNewElem;
		}
		else if(c_oserct_numdataPTCOUNT == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_ptCount = pNewElem;
		}
		else if(c_oserct_numdataPT == type)
		{
			CT_NumVal* pNewElem = new CT_NumVal;
			READ1_DEF(length, res, this->ReadCT_NumVal, pNewElem);
			poVal->m_pt.push_back(pNewElem);
		}
		else if(c_oserct_numdataEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_NumVal(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_NumVal* poVal = static_cast<CT_NumVal*>(poResult);
		if(c_oserct_numvalV == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_v = pNewElem;
		}
		else if(c_oserct_numvalIDX == type)
		{
			unsigned long* pNewElem = new unsigned long;
			*pNewElem = m_oBufferedStream.GetLong();
			poVal->m_idx = pNewElem;
		}
		else if(c_oserct_numvalFORMATCODE == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_formatCode = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_NumRef(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_NumRef* poVal = static_cast<CT_NumRef*>(poResult);
		if(c_oserct_numrefF == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_f = pNewElem;
		}
		else if(c_oserct_numrefNUMCACHE == type)
		{
			CT_NumData* pNewElem = new CT_NumData;
			READ1_DEF(length, res, this->ReadCT_NumData, pNewElem);
			poVal->m_numCache = pNewElem;
		}
		else if(c_oserct_numrefEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_AxDataSource(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_AxDataSource* poVal = static_cast<CT_AxDataSource*>(poResult);
		if(c_oserct_axdatasourceMULTILVLSTRREF == type)
		{
			CT_MultiLvlStrRef* pNewElem = new CT_MultiLvlStrRef;
			READ1_DEF(length, res, this->ReadCT_MultiLvlStrRef, pNewElem);
			poVal->m_multiLvlStrRef = pNewElem;
		}
		else if(c_oserct_axdatasourceNUMLIT == type)
		{
			CT_NumData* pNewElem = new CT_NumData;
			READ1_DEF(length, res, this->ReadCT_NumData, pNewElem);
			poVal->m_numLit = pNewElem;
		}
		else if(c_oserct_axdatasourceNUMREF == type)
		{
			CT_NumRef* pNewElem = new CT_NumRef;
			READ1_DEF(length, res, this->ReadCT_NumRef, pNewElem);
			poVal->m_numRef = pNewElem;
		}
		else if(c_oserct_axdatasourceSTRLIT == type)
		{
			CT_StrData* pNewElem = new CT_StrData;
			READ1_DEF(length, res, this->ReadCT_StrData, pNewElem);
			poVal->m_strLit = pNewElem;
		}
		else if(c_oserct_axdatasourceSTRREF == type)
		{
			CT_StrRef* pNewElem = new CT_StrRef;
			READ1_DEF(length, res, this->ReadCT_StrRef, pNewElem);
			poVal->m_strRef = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_MultiLvlStrRef(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_MultiLvlStrRef* poVal = static_cast<CT_MultiLvlStrRef*>(poResult);
		if(c_oserct_multilvlstrrefF == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_f = pNewElem;
		}
		else if(c_oserct_multilvlstrrefMULTILVLSTRCACHE == type)
		{
			CT_MultiLvlStrData* pNewElem = new CT_MultiLvlStrData;
			READ1_DEF(length, res, this->ReadCT_MultiLvlStrData, pNewElem);
			poVal->m_multiLvlStrCache = pNewElem;
		}
		else if(c_oserct_multilvlstrrefEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_lvl(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_lvl* poVal = static_cast<CT_lvl*>(poResult);
		if(c_oserct_lvlPT == type)
		{
			CT_StrVal* pNewElem = new CT_StrVal;
			READ1_DEF(length, res, this->ReadCT_StrVal, pNewElem);
			poVal->m_pt.push_back(pNewElem);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_MultiLvlStrData(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_MultiLvlStrData* poVal = static_cast<CT_MultiLvlStrData*>(poResult);
		if(c_oserct_multilvlstrdataPTCOUNT == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_ptCount = pNewElem;
		}
		else if(c_oserct_multilvlstrdataLVL == type)
		{
			CT_lvl* pNewElem = new CT_lvl;
			READ1_DEF(length, res, this->ReadCT_lvl, pNewElem);
			poVal->m_lvl.push_back(pNewElem);
		}
		else if(c_oserct_multilvlstrdataEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_BubbleChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_BubbleChart* poVal = static_cast<CT_BubbleChart*>(poResult);
		if(c_oserct_bubblechartVARYCOLORS == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_varyColors = pNewElem;
		}
		else if(c_oserct_bubblechartSER == type)
		{
			CT_BubbleSer* pNewElem = new CT_BubbleSer;
			READ1_DEF(length, res, this->ReadCT_BubbleSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if(c_oserct_bubblechartDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if(c_oserct_bubblechartBUBBLE3D == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_bubble3D = pNewElem;
		}
		else if(c_oserct_bubblechartBUBBLESCALE == type)
		{
			CT_BubbleScale* pNewElem = new CT_BubbleScale;
			READ1_DEF(length, res, this->ReadCT_BubbleScale, pNewElem);
			poVal->m_bubbleScale = pNewElem;
		}
		else if(c_oserct_bubblechartSHOWNEGBUBBLES == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_showNegBubbles = pNewElem;
		}
		else if(c_oserct_bubblechartSIZEREPRESENTS == type)
		{
			CT_SizeRepresents* pNewElem = new CT_SizeRepresents;
			READ1_DEF(length, res, this->ReadCT_SizeRepresents, pNewElem);
			poVal->m_sizeRepresents = pNewElem;
		}
		else if(c_oserct_bubblechartAXID == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_axId.push_back(pNewElem);
		}
		else if(c_oserct_bubblechartEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_bandFmts(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_bandFmts* poVal = static_cast<CT_bandFmts*>(poResult);
		if(c_oserct_bandfmtsBANDFMT == type)
		{
			CT_BandFmt* pNewElem = new CT_BandFmt;
			READ1_DEF(length, res, this->ReadCT_BandFmt, pNewElem);
			poVal->m_bandFmt.push_back(pNewElem);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Surface3DChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Surface3DChart* poVal = static_cast<CT_Surface3DChart*>(poResult);
		if(c_oserct_surface3dchartWIREFRAME == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_wireframe = pNewElem;
		}
		else if(c_oserct_surface3dchartSER == type)
		{
			CT_SurfaceSer* pNewElem = new CT_SurfaceSer;
			READ1_DEF(length, res, this->ReadCT_SurfaceSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if(c_oserct_surface3dchartBANDFMTS == type)
		{
			CT_bandFmts* pNewElem = new CT_bandFmts;
			READ1_DEF(length, res, this->ReadCT_bandFmts, pNewElem);
			poVal->m_bandFmts = pNewElem;
		}
		else if(c_oserct_surface3dchartAXID == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_axId.push_back(pNewElem);
		}
		else if(c_oserct_surface3dchartEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_SurfaceSer(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_SurfaceSer* poVal = static_cast<CT_SurfaceSer*>(poResult);
		if(c_oserct_surfaceserIDX == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_idx = pNewElem;
		}
		else if(c_oserct_surfaceserORDER == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_order = pNewElem;
		}
		else if(c_oserct_surfaceserTX == type)
		{
			CT_SerTx* pNewElem = new CT_SerTx;
			READ1_DEF(length, res, this->ReadCT_SerTx, pNewElem);
			poVal->m_tx = pNewElem;
		}
		else if(c_oserct_surfaceserSPPR == type)
		{
			poVal->m_oSpPr = new PPTX::Logic::SpPr;
			ReadCT_SpPr(0, length, poVal->m_oSpPr.GetPointer());
		}
		else if(c_oserct_surfaceserCAT == type)
		{
			CT_AxDataSource* pNewElem = new CT_AxDataSource;
			READ1_DEF(length, res, this->ReadCT_AxDataSource, pNewElem);
			poVal->m_cat = pNewElem;
		}
		else if(c_oserct_surfaceserVAL == type)
		{
			CT_NumDataSource* pNewElem = new CT_NumDataSource;
			READ1_DEF(length, res, this->ReadCT_NumDataSource, pNewElem);
			poVal->m_val = pNewElem;
		}
		else if(c_oserct_surfaceserEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_BandFmt(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_BandFmt* poVal = static_cast<CT_BandFmt*>(poResult);
		if(c_oserct_bandfmtIDX == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_idx = pNewElem;
		}
		else if(c_oserct_bandfmtSPPR == type)
		{
			poVal->m_oSpPr = new PPTX::Logic::SpPr;
			ReadCT_SpPr(0, length, poVal->m_oSpPr.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_SurfaceChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_SurfaceChart* poVal = static_cast<CT_SurfaceChart*>(poResult);
		if(c_oserct_surfacechartWIREFRAME == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_wireframe = pNewElem;
		}
		else if(c_oserct_surfacechartSER == type)
		{
			CT_SurfaceSer* pNewElem = new CT_SurfaceSer;
			READ1_DEF(length, res, this->ReadCT_SurfaceSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if(c_oserct_surfacechartBANDFMTS == type)
		{
			CT_bandFmts* pNewElem = new CT_bandFmts;
			READ1_DEF(length, res, this->ReadCT_bandFmts, pNewElem);
			poVal->m_bandFmts = pNewElem;
		}
		else if(c_oserct_surfacechartAXID == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_axId.push_back(pNewElem);
		}
		else if(c_oserct_surfacechartEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_SecondPieSize(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_SecondPieSize* poVal = static_cast<CT_SecondPieSize*>(poResult);
		if(c_oserct_secondpiesizeVAL == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_SplitType(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_SplitType* poVal = static_cast<CT_SplitType*>(poResult);
		if(c_oserct_splittypeVAL == type)
		{
			ST_SplitType* pNewElem = new ST_SplitType;
			*pNewElem = (ST_SplitType)m_oBufferedStream.GetUChar();
			;
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_OfPieType(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_OfPieType* poVal = static_cast<CT_OfPieType*>(poResult);
		if(c_oserct_ofpietypeVAL == type)
		{
			ST_OfPieType* pNewElem = new ST_OfPieType;
			*pNewElem = (ST_OfPieType)m_oBufferedStream.GetUChar();
			;
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_custSplit(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_custSplit* poVal = static_cast<CT_custSplit*>(poResult);
		if(c_oserct_custsplitSECONDPIEPT == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_secondPiePt.push_back(pNewElem);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_OfPieChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_OfPieChart* poVal = static_cast<CT_OfPieChart*>(poResult);
		if(c_oserct_ofpiechartOFPIETYPE == type)
		{
			CT_OfPieType* pNewElem = new CT_OfPieType;
			READ1_DEF(length, res, this->ReadCT_OfPieType, pNewElem);
			poVal->m_ofPieType = pNewElem;
		}
		else if(c_oserct_ofpiechartVARYCOLORS == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_varyColors = pNewElem;
		}
		else if(c_oserct_ofpiechartSER == type)
		{
			CT_PieSer* pNewElem = new CT_PieSer;
			READ1_DEF(length, res, this->ReadCT_PieSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if(c_oserct_ofpiechartDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if(c_oserct_ofpiechartGAPWIDTH == type)
		{
			CT_GapAmount* pNewElem = new CT_GapAmount;
			READ1_DEF(length, res, this->ReadCT_GapAmount, pNewElem);
			poVal->m_gapWidth = pNewElem;
		}
		else if(c_oserct_ofpiechartSPLITTYPE == type)
		{
			CT_SplitType* pNewElem = new CT_SplitType;
			READ1_DEF(length, res, this->ReadCT_SplitType, pNewElem);
			poVal->m_splitType = pNewElem;
		}
		else if(c_oserct_ofpiechartSPLITPOS == type)
		{
			CT_Double* pNewElem = new CT_Double;
			READ1_DEF(length, res, this->ReadCT_Double, pNewElem);
			poVal->m_splitPos = pNewElem;
		}
		else if(c_oserct_ofpiechartCUSTSPLIT == type)
		{
			CT_custSplit* pNewElem = new CT_custSplit;
			READ1_DEF(length, res, this->ReadCT_custSplit, pNewElem);
			poVal->m_custSplit = pNewElem;
		}
		else if(c_oserct_ofpiechartSECONDPIESIZE == type)
		{
			CT_SecondPieSize* pNewElem = new CT_SecondPieSize;
			READ1_DEF(length, res, this->ReadCT_SecondPieSize, pNewElem);
			poVal->m_secondPieSize = pNewElem;
		}
		else if(c_oserct_ofpiechartSERLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_serLines.push_back(pNewElem);
		}
		else if(c_oserct_ofpiechartEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_PieSer(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_PieSer* poVal = static_cast<CT_PieSer*>(poResult);
		if(c_oserct_pieserIDX == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_idx = pNewElem;
		}
		else if(c_oserct_pieserORDER == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_order = pNewElem;
		}
		else if(c_oserct_pieserTX == type)
		{
			CT_SerTx* pNewElem = new CT_SerTx;
			READ1_DEF(length, res, this->ReadCT_SerTx, pNewElem);
			poVal->m_tx = pNewElem;
		}
		else if(c_oserct_pieserSPPR == type)
		{
			poVal->m_oSpPr = new PPTX::Logic::SpPr;
			ReadCT_SpPr(0, length, poVal->m_oSpPr.GetPointer());
		}
		else if(c_oserct_pieserEXPLOSION == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_explosion = pNewElem;
		}
		else if(c_oserct_pieserDPT == type)
		{
			CT_DPt* pNewElem = new CT_DPt;
			READ1_DEF(length, res, this->ReadCT_DPt, pNewElem);
			poVal->m_dPt.push_back(pNewElem);
		}
		else if(c_oserct_pieserDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if(c_oserct_pieserCAT == type)
		{
			CT_AxDataSource* pNewElem = new CT_AxDataSource;
			READ1_DEF(length, res, this->ReadCT_AxDataSource, pNewElem);
			poVal->m_cat = pNewElem;
		}
		else if(c_oserct_pieserVAL == type)
		{
			CT_NumDataSource* pNewElem = new CT_NumDataSource;
			READ1_DEF(length, res, this->ReadCT_NumDataSource, pNewElem);
			poVal->m_val = pNewElem;
		}
		else if(c_oserct_pieserEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_GapAmount(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_GapAmount* poVal = static_cast<CT_GapAmount*>(poResult);
		if(c_oserct_gapamountVAL == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Bar3DChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Bar3DChart* poVal = static_cast<CT_Bar3DChart*>(poResult);
		if(c_oserct_bar3dchartBARDIR == type)
		{
			CT_BarDir* pNewElem = new CT_BarDir;
			READ1_DEF(length, res, this->ReadCT_BarDir, pNewElem);
			poVal->m_barDir = pNewElem;
		}
		else if(c_oserct_bar3dchartGROUPING == type)
		{
			CT_BarGrouping* pNewElem = new CT_BarGrouping;
			READ1_DEF(length, res, this->ReadCT_BarGrouping, pNewElem);
			poVal->m_grouping = pNewElem;
		}
		else if(c_oserct_bar3dchartVARYCOLORS == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_varyColors = pNewElem;
		}
		else if(c_oserct_bar3dchartSER == type)
		{
			CT_BarSer* pNewElem = new CT_BarSer;
			READ1_DEF(length, res, this->ReadCT_BarSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if(c_oserct_bar3dchartDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if(c_oserct_bar3dchartGAPWIDTH == type)
		{
			CT_GapAmount* pNewElem = new CT_GapAmount;
			READ1_DEF(length, res, this->ReadCT_GapAmount, pNewElem);
			poVal->m_gapWidth = pNewElem;
		}
		else if(c_oserct_bar3dchartGAPDEPTH == type)
		{
			CT_GapAmount* pNewElem = new CT_GapAmount;
			READ1_DEF(length, res, this->ReadCT_GapAmount, pNewElem);
			poVal->m_gapDepth = pNewElem;
		}
		else if(c_oserct_bar3dchartSHAPE == type)
		{
			CT_Shape* pNewElem = new CT_Shape;
			READ1_DEF(length, res, this->ReadCT_Shape, pNewElem);
			poVal->m_shape = pNewElem;
		}
		else if(c_oserct_bar3dchartAXID == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_axId.push_back(pNewElem);
		}
		else if(c_oserct_bar3dchartEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_BarDir(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_BarDir* poVal = static_cast<CT_BarDir*>(poResult);
		if(c_oserct_bardirVAL == type)
		{
			ST_BarDir* pNewElem = new ST_BarDir;
			*pNewElem = (ST_BarDir)m_oBufferedStream.GetUChar();
			;
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_BarGrouping(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_BarGrouping* poVal = static_cast<CT_BarGrouping*>(poResult);
		if(c_oserct_bargroupingVAL == type)
		{
			ST_BarGrouping* pNewElem = new ST_BarGrouping;
			*pNewElem = (ST_BarGrouping)m_oBufferedStream.GetUChar();
			;
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_BarSer(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_BarSer* poVal = static_cast<CT_BarSer*>(poResult);
		if(c_oserct_barserIDX == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_idx = pNewElem;
		}
		else if(c_oserct_barserORDER == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_order = pNewElem;
		}
		else if(c_oserct_barserTX == type)
		{
			CT_SerTx* pNewElem = new CT_SerTx;
			READ1_DEF(length, res, this->ReadCT_SerTx, pNewElem);
			poVal->m_tx = pNewElem;
		}
		else if(c_oserct_barserSPPR == type)
		{
			poVal->m_oSpPr = new PPTX::Logic::SpPr;
			ReadCT_SpPr(0, length, poVal->m_oSpPr.GetPointer());
		}
		else if(c_oserct_barserINVERTIFNEGATIVE == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_invertIfNegative = pNewElem;
		}
		else if(c_oserct_barserPICTUREOPTIONS == type)
		{
			CT_PictureOptions* pNewElem = new CT_PictureOptions;
			READ1_DEF(length, res, this->ReadCT_PictureOptions, pNewElem);
			poVal->m_pictureOptions = pNewElem;
		}
		else if(c_oserct_barserDPT == type)
		{
			CT_DPt* pNewElem = new CT_DPt;
			READ1_DEF(length, res, this->ReadCT_DPt, pNewElem);
			poVal->m_dPt.push_back(pNewElem);
		}
		else if(c_oserct_barserDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if(c_oserct_barserTRENDLINE == type)
		{
			CT_Trendline* pNewElem = new CT_Trendline;
			READ1_DEF(length, res, this->ReadCT_Trendline, pNewElem);
			poVal->m_trendline.push_back(pNewElem);
		}
		else if(c_oserct_barserERRBARS == type)
		{
			CT_ErrBars* pNewElem = new CT_ErrBars;
			READ1_DEF(length, res, this->ReadCT_ErrBars, pNewElem);
			poVal->m_errBars = pNewElem;
		}
		else if(c_oserct_barserCAT == type)
		{
			CT_AxDataSource* pNewElem = new CT_AxDataSource;
			READ1_DEF(length, res, this->ReadCT_AxDataSource, pNewElem);
			poVal->m_cat = pNewElem;
		}
		else if(c_oserct_barserVAL == type)
		{
			CT_NumDataSource* pNewElem = new CT_NumDataSource;
			READ1_DEF(length, res, this->ReadCT_NumDataSource, pNewElem);
			poVal->m_val = pNewElem;
		}
		else if(c_oserct_barserSHAPE == type)
		{
			CT_Shape* pNewElem = new CT_Shape;
			READ1_DEF(length, res, this->ReadCT_Shape, pNewElem);
			poVal->m_shape = pNewElem;
		}
		else if(c_oserct_barserEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Shape(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Shape* poVal = static_cast<CT_Shape*>(poResult);
		if(c_oserct_shapeVAL == type)
		{
			ST_Shape* pNewElem = new ST_Shape;
			*pNewElem = (ST_Shape)m_oBufferedStream.GetUChar();
			;
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Overlap(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Overlap* poVal = static_cast<CT_Overlap*>(poResult);
		if(c_oserct_overlapVAL == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_BarChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_BarChart* poVal = static_cast<CT_BarChart*>(poResult);
		if(c_oserct_barchartBARDIR == type)
		{
			CT_BarDir* pNewElem = new CT_BarDir;
			READ1_DEF(length, res, this->ReadCT_BarDir, pNewElem);
			poVal->m_barDir = pNewElem;
		}
		else if(c_oserct_barchartGROUPING == type)
		{
			CT_BarGrouping* pNewElem = new CT_BarGrouping;
			READ1_DEF(length, res, this->ReadCT_BarGrouping, pNewElem);
			poVal->m_grouping = pNewElem;
		}
		else if(c_oserct_barchartVARYCOLORS == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_varyColors = pNewElem;
		}
		else if(c_oserct_barchartSER == type)
		{
			CT_BarSer* pNewElem = new CT_BarSer;
			READ1_DEF(length, res, this->ReadCT_BarSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if(c_oserct_barchartDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if(c_oserct_barchartGAPWIDTH == type)
		{
			CT_GapAmount* pNewElem = new CT_GapAmount;
			READ1_DEF(length, res, this->ReadCT_GapAmount, pNewElem);
			poVal->m_gapWidth = pNewElem;
		}
		else if(c_oserct_barchartOVERLAP == type)
		{
			CT_Overlap* pNewElem = new CT_Overlap;
			READ1_DEF(length, res, this->ReadCT_Overlap, pNewElem);
			poVal->m_overlap = pNewElem;
		}
		else if(c_oserct_barchartSERLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_serLines.push_back(pNewElem);
		}
		else if(c_oserct_barchartAXID == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_axId.push_back(pNewElem);
		}
		else if(c_oserct_barchartEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_HoleSize(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_HoleSize* poVal = static_cast<CT_HoleSize*>(poResult);
		if(c_oserct_holesizeVAL == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_DoughnutChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_DoughnutChart* poVal = static_cast<CT_DoughnutChart*>(poResult);
		if(c_oserct_doughnutchartVARYCOLORS == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_varyColors = pNewElem;
		}
		else if(c_oserct_doughnutchartSER == type)
		{
			CT_PieSer* pNewElem = new CT_PieSer;
			READ1_DEF(length, res, this->ReadCT_PieSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if(c_oserct_doughnutchartDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if(c_oserct_doughnutchartFIRSTSLICEANG == type)
		{
			CT_FirstSliceAng* pNewElem = new CT_FirstSliceAng;
			READ1_DEF(length, res, this->ReadCT_FirstSliceAng, pNewElem);
			poVal->m_firstSliceAng = pNewElem;
		}
		else if(c_oserct_doughnutchartHOLESIZE == type)
		{
			CT_HoleSize* pNewElem = new CT_HoleSize;
			READ1_DEF(length, res, this->ReadCT_HoleSize, pNewElem);
			poVal->m_holeSize = pNewElem;
		}
		else if(c_oserct_doughnutchartEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_FirstSliceAng(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_FirstSliceAng* poVal = static_cast<CT_FirstSliceAng*>(poResult);
		if(c_oserct_firstsliceangVAL == type)
		{
			unsigned long* pNewElem = new unsigned long;
			*pNewElem = m_oBufferedStream.GetLong();
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Pie3DChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Pie3DChart* poVal = static_cast<CT_Pie3DChart*>(poResult);
		if(c_oserct_pie3dchartVARYCOLORS == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_varyColors = pNewElem;
		}
		else if(c_oserct_pie3dchartSER == type)
		{
			CT_PieSer* pNewElem = new CT_PieSer;
			READ1_DEF(length, res, this->ReadCT_PieSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if(c_oserct_pie3dchartDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if(c_oserct_pie3dchartEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_PieChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_PieChart* poVal = static_cast<CT_PieChart*>(poResult);
		if(c_oserct_piechartVARYCOLORS == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_varyColors = pNewElem;
		}
		else if(c_oserct_piechartSER == type)
		{
			CT_PieSer* pNewElem = new CT_PieSer;
			READ1_DEF(length, res, this->ReadCT_PieSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if(c_oserct_piechartDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if(c_oserct_piechartFIRSTSLICEANG == type)
		{
			CT_FirstSliceAng* pNewElem = new CT_FirstSliceAng;
			READ1_DEF(length, res, this->ReadCT_FirstSliceAng, pNewElem);
			poVal->m_firstSliceAng = pNewElem;
		}
		else if(c_oserct_piechartEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ScatterSer(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_ScatterSer* poVal = static_cast<CT_ScatterSer*>(poResult);
		if(c_oserct_scatterserIDX == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_idx = pNewElem;
		}
		else if(c_oserct_scatterserORDER == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_order = pNewElem;
		}
		else if(c_oserct_scatterserTX == type)
		{
			CT_SerTx* pNewElem = new CT_SerTx;
			READ1_DEF(length, res, this->ReadCT_SerTx, pNewElem);
			poVal->m_tx = pNewElem;
		}
		else if(c_oserct_scatterserSPPR == type)
		{
			poVal->m_oSpPr = new PPTX::Logic::SpPr;
			ReadCT_SpPr(0, length, poVal->m_oSpPr.GetPointer());
		}
		else if(c_oserct_scatterserMARKER == type)
		{
			CT_Marker* pNewElem = new CT_Marker;
			READ1_DEF(length, res, this->ReadCT_Marker, pNewElem);
			poVal->m_marker = pNewElem;
		}
		else if(c_oserct_scatterserDPT == type)
		{
			CT_DPt* pNewElem = new CT_DPt;
			READ1_DEF(length, res, this->ReadCT_DPt, pNewElem);
			poVal->m_dPt.push_back(pNewElem);
		}
		else if(c_oserct_scatterserDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if(c_oserct_scatterserTRENDLINE == type)
		{
			CT_Trendline* pNewElem = new CT_Trendline;
			READ1_DEF(length, res, this->ReadCT_Trendline, pNewElem);
			poVal->m_trendline.push_back(pNewElem);
		}
		else if(c_oserct_scatterserERRBARS == type)
		{
			CT_ErrBars* pNewElem = new CT_ErrBars;
			READ1_DEF(length, res, this->ReadCT_ErrBars, pNewElem);
			poVal->m_errBars.push_back(pNewElem);
		}
		else if(c_oserct_scatterserXVAL == type)
		{
			CT_AxDataSource* pNewElem = new CT_AxDataSource;
			READ1_DEF(length, res, this->ReadCT_AxDataSource, pNewElem);
			poVal->m_xVal = pNewElem;
		}
		else if(c_oserct_scatterserYVAL == type)
		{
			CT_NumDataSource* pNewElem = new CT_NumDataSource;
			READ1_DEF(length, res, this->ReadCT_NumDataSource, pNewElem);
			poVal->m_yVal = pNewElem;
		}
		else if(c_oserct_scatterserSMOOTH == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_smooth = pNewElem;
		}
		else if(c_oserct_scatterserEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ScatterStyle(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_ScatterStyle* poVal = static_cast<CT_ScatterStyle*>(poResult);
		if(c_oserct_scatterstyleVAL == type)
		{
			ST_ScatterStyle* pNewElem = new ST_ScatterStyle;
			*pNewElem = (ST_ScatterStyle)m_oBufferedStream.GetUChar();
			;
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ScatterChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_ScatterChart* poVal = static_cast<CT_ScatterChart*>(poResult);
		if(c_oserct_scatterchartSCATTERSTYLE == type)
		{
			CT_ScatterStyle* pNewElem = new CT_ScatterStyle;
			READ1_DEF(length, res, this->ReadCT_ScatterStyle, pNewElem);
			poVal->m_scatterStyle = pNewElem;
		}
		else if(c_oserct_scatterchartVARYCOLORS == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_varyColors = pNewElem;
		}
		else if(c_oserct_scatterchartSER == type)
		{
			CT_ScatterSer* pNewElem = new CT_ScatterSer;
			READ1_DEF(length, res, this->ReadCT_ScatterSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if(c_oserct_scatterchartDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if(c_oserct_scatterchartAXID == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_axId.push_back(pNewElem);
		}
		else if(c_oserct_scatterchartEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_RadarSer(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_RadarSer* poVal = static_cast<CT_RadarSer*>(poResult);
		if(c_oserct_radarserIDX == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_idx = pNewElem;
		}
		else if(c_oserct_radarserORDER == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_order = pNewElem;
		}
		else if(c_oserct_radarserTX == type)
		{
			CT_SerTx* pNewElem = new CT_SerTx;
			READ1_DEF(length, res, this->ReadCT_SerTx, pNewElem);
			poVal->m_tx = pNewElem;
		}
		else if(c_oserct_radarserSPPR == type)
		{
			poVal->m_oSpPr = new PPTX::Logic::SpPr;
			ReadCT_SpPr(0, length, poVal->m_oSpPr.GetPointer());
		}
		else if(c_oserct_radarserMARKER == type)
		{
			CT_Marker* pNewElem = new CT_Marker;
			READ1_DEF(length, res, this->ReadCT_Marker, pNewElem);
			poVal->m_marker = pNewElem;
		}
		else if(c_oserct_radarserDPT == type)
		{
			CT_DPt* pNewElem = new CT_DPt;
			READ1_DEF(length, res, this->ReadCT_DPt, pNewElem);
			poVal->m_dPt.push_back(pNewElem);
		}
		else if(c_oserct_radarserDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if(c_oserct_radarserCAT == type)
		{
			CT_AxDataSource* pNewElem = new CT_AxDataSource;
			READ1_DEF(length, res, this->ReadCT_AxDataSource, pNewElem);
			poVal->m_cat = pNewElem;
		}
		else if(c_oserct_radarserVAL == type)
		{
			CT_NumDataSource* pNewElem = new CT_NumDataSource;
			READ1_DEF(length, res, this->ReadCT_NumDataSource, pNewElem);
			poVal->m_val = pNewElem;
		}
		else if(c_oserct_radarserEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_RadarStyle(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_RadarStyle* poVal = static_cast<CT_RadarStyle*>(poResult);
		if(c_oserct_radarstyleVAL == type)
		{
			ST_RadarStyle* pNewElem = new ST_RadarStyle;
			*pNewElem = (ST_RadarStyle)m_oBufferedStream.GetUChar();
			;
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_RadarChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_RadarChart* poVal = static_cast<CT_RadarChart*>(poResult);
		if(c_oserct_radarchartRADARSTYLE == type)
		{
			CT_RadarStyle* pNewElem = new CT_RadarStyle;
			READ1_DEF(length, res, this->ReadCT_RadarStyle, pNewElem);
			poVal->m_radarStyle = pNewElem;
		}
		else if(c_oserct_radarchartVARYCOLORS == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_varyColors = pNewElem;
		}
		else if(c_oserct_radarchartSER == type)
		{
			CT_RadarSer* pNewElem = new CT_RadarSer;
			READ1_DEF(length, res, this->ReadCT_RadarSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if(c_oserct_radarchartDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if(c_oserct_radarchartAXID == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_axId.push_back(pNewElem);
		}
		else if(c_oserct_radarchartEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_StockChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_StockChart* poVal = static_cast<CT_StockChart*>(poResult);
		if(c_oserct_stockchartSER == type)
		{
			CT_LineSer* pNewElem = new CT_LineSer;
			READ1_DEF(length, res, this->ReadCT_LineSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if(c_oserct_stockchartDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if(c_oserct_stockchartDROPLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_dropLines = pNewElem;
		}
		else if(c_oserct_stockchartHILOWLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_hiLowLines = pNewElem;
		}
		else if(c_oserct_stockchartUPDOWNBARS == type)
		{
			CT_UpDownBars* pNewElem = new CT_UpDownBars;
			READ1_DEF(length, res, this->ReadCT_UpDownBars, pNewElem);
			poVal->m_upDownBars = pNewElem;
		}
		else if(c_oserct_stockchartAXID == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_axId.push_back(pNewElem);
		}
		else if(c_oserct_stockchartEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_LineSer(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_LineSer* poVal = static_cast<CT_LineSer*>(poResult);
		if(c_oserct_lineserIDX == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_idx = pNewElem;
		}
		else if(c_oserct_lineserORDER == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_order = pNewElem;
		}
		else if(c_oserct_lineserTX == type)
		{
			CT_SerTx* pNewElem = new CT_SerTx;
			READ1_DEF(length, res, this->ReadCT_SerTx, pNewElem);
			poVal->m_tx = pNewElem;
		}
		else if(c_oserct_lineserSPPR == type)
		{
			poVal->m_oSpPr = new PPTX::Logic::SpPr;
			ReadCT_SpPr(0, length, poVal->m_oSpPr.GetPointer());
		}
		else if(c_oserct_lineserMARKER == type)
		{
			CT_Marker* pNewElem = new CT_Marker;
			READ1_DEF(length, res, this->ReadCT_Marker, pNewElem);
			poVal->m_marker = pNewElem;
		}
		else if(c_oserct_lineserDPT == type)
		{
			CT_DPt* pNewElem = new CT_DPt;
			READ1_DEF(length, res, this->ReadCT_DPt, pNewElem);
			poVal->m_dPt.push_back(pNewElem);
		}
		else if(c_oserct_lineserDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if(c_oserct_lineserTRENDLINE == type)
		{
			CT_Trendline* pNewElem = new CT_Trendline;
			READ1_DEF(length, res, this->ReadCT_Trendline, pNewElem);
			poVal->m_trendline.push_back(pNewElem);
		}
		else if(c_oserct_lineserERRBARS == type)
		{
			CT_ErrBars* pNewElem = new CT_ErrBars;
			READ1_DEF(length, res, this->ReadCT_ErrBars, pNewElem);
			poVal->m_errBars = pNewElem;
		}
		else if(c_oserct_lineserCAT == type)
		{
			CT_AxDataSource* pNewElem = new CT_AxDataSource;
			READ1_DEF(length, res, this->ReadCT_AxDataSource, pNewElem);
			poVal->m_cat = pNewElem;
		}
		else if(c_oserct_lineserVAL == type)
		{
			CT_NumDataSource* pNewElem = new CT_NumDataSource;
			READ1_DEF(length, res, this->ReadCT_NumDataSource, pNewElem);
			poVal->m_val = pNewElem;
		}
		else if(c_oserct_lineserSMOOTH == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_smooth = pNewElem;
		}
		else if(c_oserct_lineserEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_UpDownBars(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_UpDownBars* poVal = static_cast<CT_UpDownBars*>(poResult);
		if(c_oserct_updownbarsGAPWIDTH == type)
		{
			CT_GapAmount* pNewElem = new CT_GapAmount;
			READ1_DEF(length, res, this->ReadCT_GapAmount, pNewElem);
			poVal->m_gapWidth = pNewElem;
		}
		else if(c_oserct_updownbarsUPBARS == type)
		{
			CT_UpDownBar* pNewElem = new CT_UpDownBar;
			READ1_DEF(length, res, this->ReadCT_UpDownBar, pNewElem);
			poVal->m_upBars = pNewElem;
		}
		else if(c_oserct_updownbarsDOWNBARS == type)
		{
			CT_UpDownBar* pNewElem = new CT_UpDownBar;
			READ1_DEF(length, res, this->ReadCT_UpDownBar, pNewElem);
			poVal->m_downBars = pNewElem;
		}
		else if(c_oserct_updownbarsEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_UpDownBar(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_UpDownBar* poVal = static_cast<CT_UpDownBar*>(poResult);
		if(c_oserct_updownbarSPPR == type)
		{
			poVal->m_oSpPr = new PPTX::Logic::SpPr;
			ReadCT_SpPr(0, length, poVal->m_oSpPr.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Line3DChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Line3DChart* poVal = static_cast<CT_Line3DChart*>(poResult);
		if(c_oserct_line3dchartGROUPING == type)
		{
			CT_Grouping* pNewElem = new CT_Grouping;
			READ1_DEF(length, res, this->ReadCT_Grouping, pNewElem);
			poVal->m_grouping = pNewElem;
		}
		else if(c_oserct_line3dchartVARYCOLORS == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_varyColors = pNewElem;
		}
		else if(c_oserct_line3dchartSER == type)
		{
			CT_LineSer* pNewElem = new CT_LineSer;
			READ1_DEF(length, res, this->ReadCT_LineSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if(c_oserct_line3dchartDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if(c_oserct_line3dchartDROPLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_dropLines = pNewElem;
		}
		else if(c_oserct_line3dchartGAPDEPTH == type)
		{
			CT_GapAmount* pNewElem = new CT_GapAmount;
			READ1_DEF(length, res, this->ReadCT_GapAmount, pNewElem);
			poVal->m_gapDepth = pNewElem;
		}
		else if(c_oserct_line3dchartAXID == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_axId.push_back(pNewElem);
		}
		else if(c_oserct_line3dchartEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Grouping(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Grouping* poVal = static_cast<CT_Grouping*>(poResult);
		if(c_oserct_groupingVAL == type)
		{
			ST_Grouping* pNewElem = new ST_Grouping;
			*pNewElem = (ST_Grouping)m_oBufferedStream.GetUChar();
			;
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_LineChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_LineChart* poVal = static_cast<CT_LineChart*>(poResult);
		if(c_oserct_linechartGROUPING == type)
		{
			CT_Grouping* pNewElem = new CT_Grouping;
			READ1_DEF(length, res, this->ReadCT_Grouping, pNewElem);
			poVal->m_grouping = pNewElem;
		}
		else if(c_oserct_linechartVARYCOLORS == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_varyColors = pNewElem;
		}
		else if(c_oserct_linechartSER == type)
		{
			CT_LineSer* pNewElem = new CT_LineSer;
			READ1_DEF(length, res, this->ReadCT_LineSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if(c_oserct_linechartDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if(c_oserct_linechartDROPLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_dropLines = pNewElem;
		}
		else if(c_oserct_linechartHILOWLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_hiLowLines = pNewElem;
		}
		else if(c_oserct_linechartUPDOWNBARS == type)
		{
			CT_UpDownBars* pNewElem = new CT_UpDownBars;
			READ1_DEF(length, res, this->ReadCT_UpDownBars, pNewElem);
			poVal->m_upDownBars = pNewElem;
		}
		else if(c_oserct_linechartMARKER == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_marker = pNewElem;
		}
		else if(c_oserct_linechartSMOOTH == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_smooth = pNewElem;
		}
		else if(c_oserct_linechartAXID == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_axId.push_back(pNewElem);
		}
		else if(c_oserct_linechartEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Area3DChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Area3DChart* poVal = static_cast<CT_Area3DChart*>(poResult);
		if(c_oserct_area3dchartGROUPING == type)
		{
			CT_Grouping* pNewElem = new CT_Grouping;
			READ1_DEF(length, res, this->ReadCT_Grouping, pNewElem);
			poVal->m_grouping = pNewElem;
		}
		else if(c_oserct_area3dchartVARYCOLORS == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_varyColors = pNewElem;
		}
		else if(c_oserct_area3dchartSER == type)
		{
			CT_AreaSer* pNewElem = new CT_AreaSer;
			READ1_DEF(length, res, this->ReadCT_AreaSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if(c_oserct_area3dchartDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if(c_oserct_area3dchartDROPLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_dropLines = pNewElem;
		}
		else if(c_oserct_area3dchartGAPDEPTH == type)
		{
			CT_GapAmount* pNewElem = new CT_GapAmount;
			READ1_DEF(length, res, this->ReadCT_GapAmount, pNewElem);
			poVal->m_gapDepth = pNewElem;
		}
		else if(c_oserct_area3dchartAXID == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_axId.push_back(pNewElem);
		}
		else if(c_oserct_area3dchartEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_AreaSer(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_AreaSer* poVal = static_cast<CT_AreaSer*>(poResult);
		if(c_oserct_areaserIDX == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_idx = pNewElem;
		}
		else if(c_oserct_areaserORDER == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_order = pNewElem;
		}
		else if(c_oserct_areaserTX == type)
		{
			CT_SerTx* pNewElem = new CT_SerTx;
			READ1_DEF(length, res, this->ReadCT_SerTx, pNewElem);
			poVal->m_tx = pNewElem;
		}
		else if(c_oserct_areaserSPPR == type)
		{
			poVal->m_oSpPr = new PPTX::Logic::SpPr;
			ReadCT_SpPr(0, length, poVal->m_oSpPr.GetPointer());
		}
		else if(c_oserct_areaserPICTUREOPTIONS == type)
		{
			CT_PictureOptions* pNewElem = new CT_PictureOptions;
			READ1_DEF(length, res, this->ReadCT_PictureOptions, pNewElem);
			poVal->m_pictureOptions = pNewElem;
		}
		else if(c_oserct_areaserDPT == type)
		{
			CT_DPt* pNewElem = new CT_DPt;
			READ1_DEF(length, res, this->ReadCT_DPt, pNewElem);
			poVal->m_dPt.push_back(pNewElem);
		}
		else if(c_oserct_areaserDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if(c_oserct_areaserTRENDLINE == type)
		{
			CT_Trendline* pNewElem = new CT_Trendline;
			READ1_DEF(length, res, this->ReadCT_Trendline, pNewElem);
			poVal->m_trendline.push_back(pNewElem);
		}
		else if(c_oserct_areaserERRBARS == type)
		{
			CT_ErrBars* pNewElem = new CT_ErrBars;
			READ1_DEF(length, res, this->ReadCT_ErrBars, pNewElem);
			poVal->m_errBars.push_back(pNewElem);
		}
		else if(c_oserct_areaserCAT == type)
		{
			CT_AxDataSource* pNewElem = new CT_AxDataSource;
			READ1_DEF(length, res, this->ReadCT_AxDataSource, pNewElem);
			poVal->m_cat = pNewElem;
		}
		else if(c_oserct_areaserVAL == type)
		{
			CT_NumDataSource* pNewElem = new CT_NumDataSource;
			READ1_DEF(length, res, this->ReadCT_NumDataSource, pNewElem);
			poVal->m_val = pNewElem;
		}
		else if(c_oserct_areaserEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_AreaChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_AreaChart* poVal = static_cast<CT_AreaChart*>(poResult);
		if(c_oserct_areachartGROUPING == type)
		{
			CT_Grouping* pNewElem = new CT_Grouping;
			READ1_DEF(length, res, this->ReadCT_Grouping, pNewElem);
			poVal->m_grouping = pNewElem;
		}
		else if(c_oserct_areachartVARYCOLORS == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_varyColors = pNewElem;
		}
		else if(c_oserct_areachartSER == type)
		{
			CT_AreaSer* pNewElem = new CT_AreaSer;
			READ1_DEF(length, res, this->ReadCT_AreaSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if(c_oserct_areachartDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if(c_oserct_areachartDROPLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_dropLines = pNewElem;
		}
		else if(c_oserct_areachartAXID == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_axId.push_back(pNewElem);
		}
		else if(c_oserct_areachartEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_PlotArea(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_PlotArea* poVal = static_cast<CT_PlotArea*>(poResult);
		if(c_oserct_plotareaLAYOUT == type)
		{
			CT_Layout* pNewElem = new CT_Layout;
			READ1_DEF(length, res, this->ReadCT_Layout, pNewElem);
			poVal->m_layout = pNewElem;
		}
		else if(c_oserct_plotareaAREA3DCHART == type)
		{
			CT_Area3DChart* pNewElem = new CT_Area3DChart;
			READ1_DEF(length, res, this->ReadCT_Area3DChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5AREA3DCHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_plotareaAREACHART == type)
		{
			CT_AreaChart* pNewElem = new CT_AreaChart;
			READ1_DEF(length, res, this->ReadCT_AreaChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5AREACHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_plotareaBAR3DCHART == type)
		{
			CT_Bar3DChart* pNewElem = new CT_Bar3DChart;
			READ1_DEF(length, res, this->ReadCT_Bar3DChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5BAR3DCHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_plotareaBARCHART == type)
		{
			CT_BarChart* pNewElem = new CT_BarChart;
			READ1_DEF(length, res, this->ReadCT_BarChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5BARCHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_plotareaBUBBLECHART == type)
		{
			CT_BubbleChart* pNewElem = new CT_BubbleChart;
			READ1_DEF(length, res, this->ReadCT_BubbleChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5BUBBLECHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_plotareaDOUGHNUTCHART == type)
		{
			CT_DoughnutChart* pNewElem = new CT_DoughnutChart;
			READ1_DEF(length, res, this->ReadCT_DoughnutChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5DOUGHNUTCHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_plotareaLINE3DCHART == type)
		{
			CT_Line3DChart* pNewElem = new CT_Line3DChart;
			READ1_DEF(length, res, this->ReadCT_Line3DChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5LINE3DCHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_plotareaLINECHART == type)
		{
			CT_LineChart* pNewElem = new CT_LineChart;
			READ1_DEF(length, res, this->ReadCT_LineChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5LINECHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_plotareaOFPIECHART == type)
		{
			CT_OfPieChart* pNewElem = new CT_OfPieChart;
			READ1_DEF(length, res, this->ReadCT_OfPieChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5OFPIECHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_plotareaPIE3DCHART == type)
		{
			CT_Pie3DChart* pNewElem = new CT_Pie3DChart;
			READ1_DEF(length, res, this->ReadCT_Pie3DChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5PIE3DCHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_plotareaPIECHART == type)
		{
			CT_PieChart* pNewElem = new CT_PieChart;
			READ1_DEF(length, res, this->ReadCT_PieChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5PIECHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_plotareaRADARCHART == type)
		{
			CT_RadarChart* pNewElem = new CT_RadarChart;
			READ1_DEF(length, res, this->ReadCT_RadarChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5RADARCHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_plotareaSCATTERCHART == type)
		{
			CT_ScatterChart* pNewElem = new CT_ScatterChart;
			READ1_DEF(length, res, this->ReadCT_ScatterChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5SCATTERCHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_plotareaSTOCKCHART == type)
		{
			CT_StockChart* pNewElem = new CT_StockChart;
			READ1_DEF(length, res, this->ReadCT_StockChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5STOCKCHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_plotareaSURFACE3DCHART == type)
		{
			CT_Surface3DChart* pNewElem = new CT_Surface3DChart;
			READ1_DEF(length, res, this->ReadCT_Surface3DChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5SURFACE3DCHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_plotareaSURFACECHART == type)
		{
			CT_SurfaceChart* pNewElem = new CT_SurfaceChart;
			READ1_DEF(length, res, this->ReadCT_SurfaceChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5SURFACECHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if(c_oserct_plotareaCATAX == type)
		{
			CT_CatAx* pNewElem = new CT_CatAx;
			READ1_DEF(length, res, this->ReadCT_CatAx, pNewElem);
			ItemsChoiceType6* eElemtype = new ItemsChoiceType6;
			*eElemtype = itemschoicetype6CATAX;
			poVal->m_ItemsElementName1.push_back(eElemtype);
			poVal->m_Items1.push_back(pNewElem);
		}
		else if(c_oserct_plotareaDATEAX == type)
		{
			CT_DateAx* pNewElem = new CT_DateAx;
			READ1_DEF(length, res, this->ReadCT_DateAx, pNewElem);
			ItemsChoiceType6* eElemtype = new ItemsChoiceType6;
			*eElemtype = itemschoicetype6DATEAX;
			poVal->m_ItemsElementName1.push_back(eElemtype);
			poVal->m_Items1.push_back(pNewElem);
		}
		else if(c_oserct_plotareaSERAX == type)
		{
			CT_SerAx* pNewElem = new CT_SerAx;
			READ1_DEF(length, res, this->ReadCT_SerAx, pNewElem);
			ItemsChoiceType6* eElemtype = new ItemsChoiceType6;
			*eElemtype = itemschoicetype6SERAX;
			poVal->m_ItemsElementName1.push_back(eElemtype);
			poVal->m_Items1.push_back(pNewElem);
		}
		else if(c_oserct_plotareaVALAX == type)
		{
			CT_ValAx* pNewElem = new CT_ValAx;
			READ1_DEF(length, res, this->ReadCT_ValAx, pNewElem);
			ItemsChoiceType6* eElemtype = new ItemsChoiceType6;
			*eElemtype = itemschoicetype6VALAX;
			poVal->m_ItemsElementName1.push_back(eElemtype);
			poVal->m_Items1.push_back(pNewElem);
		}
		else if(c_oserct_plotareaDTABLE == type)
		{
			CT_DTable* pNewElem = new CT_DTable;
			READ1_DEF(length, res, this->ReadCT_DTable, pNewElem);
			poVal->m_dTable = pNewElem;
		}
		else if(c_oserct_plotareaSPPR == type)
		{
			poVal->m_oSpPr = new PPTX::Logic::SpPr;
			ReadCT_SpPr(0, length, poVal->m_oSpPr.GetPointer());
		}
		else if(c_oserct_plotareaEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Thickness(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Thickness* poVal = static_cast<CT_Thickness*>(poResult);
		if(c_oserct_thicknessVAL == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Surface(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Surface* poVal = static_cast<CT_Surface*>(poResult);
		if(c_oserct_surfaceTHICKNESS == type)
		{
			CT_Thickness* pNewElem = new CT_Thickness;
			READ1_DEF(length, res, this->ReadCT_Thickness, pNewElem);
			poVal->m_thickness = pNewElem;
		}
		else if(c_oserct_surfaceSPPR == type)
		{
			poVal->m_oSpPr = new PPTX::Logic::SpPr;
			ReadCT_SpPr(0, length, poVal->m_oSpPr.GetPointer());
		}
		else if(c_oserct_surfacePICTUREOPTIONS == type)
		{
			CT_PictureOptions* pNewElem = new CT_PictureOptions;
			READ1_DEF(length, res, this->ReadCT_PictureOptions, pNewElem);
			poVal->m_pictureOptions = pNewElem;
		}
		else if(c_oserct_surfaceEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Perspective(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Perspective* poVal = static_cast<CT_Perspective*>(poResult);
		if(c_oserct_perspectiveVAL == type)
		{
			unsigned char* pNewElem = new unsigned char;
			*pNewElem = m_oBufferedStream.GetUChar();
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_DepthPercent(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_DepthPercent* poVal = static_cast<CT_DepthPercent*>(poResult);
		if(c_oserct_depthpercentVAL == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_RotY(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_RotY* poVal = static_cast<CT_RotY*>(poResult);
		if(c_oserct_rotyVAL == type)
		{
			unsigned long* pNewElem = new unsigned long;
			*pNewElem = m_oBufferedStream.GetLong();
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_HPercent(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_HPercent* poVal = static_cast<CT_HPercent*>(poResult);
		if(c_oserct_hpercentVAL == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_RotX(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_RotX* poVal = static_cast<CT_RotX*>(poResult);
		if(c_oserct_rotxVAL == type)
		{
			signed char* pNewElem = new signed char;
			*pNewElem = m_oBufferedStream.GetChar();
			poVal->m_val = (signed char*)pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_View3D(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_View3D* poVal = static_cast<CT_View3D*>(poResult);
		if(c_oserct_view3dROTX == type)
		{
			CT_RotX* pNewElem = new CT_RotX;
			READ1_DEF(length, res, this->ReadCT_RotX, pNewElem);
			poVal->m_rotX = pNewElem;
		}
		else if(c_oserct_view3dHPERCENT == type)
		{
			CT_HPercent* pNewElem = new CT_HPercent;
			READ1_DEF(length, res, this->ReadCT_HPercent, pNewElem);
			poVal->m_hPercent = pNewElem;
		}
		else if(c_oserct_view3dROTY == type)
		{
			CT_RotY* pNewElem = new CT_RotY;
			READ1_DEF(length, res, this->ReadCT_RotY, pNewElem);
			poVal->m_rotY = pNewElem;
		}
		else if(c_oserct_view3dDEPTHPERCENT == type)
		{
			CT_DepthPercent* pNewElem = new CT_DepthPercent;
			READ1_DEF(length, res, this->ReadCT_DepthPercent, pNewElem);
			poVal->m_depthPercent = pNewElem;
		}
		else if(c_oserct_view3dRANGAX == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_rAngAx = pNewElem;
		}
		else if(c_oserct_view3dPERSPECTIVE == type)
		{
			CT_Perspective* pNewElem = new CT_Perspective;
			READ1_DEF(length, res, this->ReadCT_Perspective, pNewElem);
			poVal->m_perspective = pNewElem;
		}
		else if(c_oserct_view3dEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_PivotFmt(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_PivotFmt* poVal = static_cast<CT_PivotFmt*>(poResult);
		if(c_oserct_pivotfmtIDX == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_idx = pNewElem;
		}
		else if(c_oserct_pivotfmtSPPR == type)
		{
			poVal->m_oSpPr = new PPTX::Logic::SpPr;
			ReadCT_SpPr(0, length, poVal->m_oSpPr.GetPointer());
		}
		else if(c_oserct_pivotfmtTXPR == type)
		{
			poVal->m_oTxPr = new PPTX::Logic::TxBody;
			res = ReadCT_TxPr(0, length, poVal->m_oTxPr.GetPointer());
		}
		else if(c_oserct_pivotfmtMARKER == type)
		{
			CT_Marker* pNewElem = new CT_Marker;
			READ1_DEF(length, res, this->ReadCT_Marker, pNewElem);
			poVal->m_marker = pNewElem;
		}
		else if(c_oserct_pivotfmtDLBL == type)
		{
			CT_DLbl* pNewElem = new CT_DLbl;
			READ1_DEF(length, res, this->ReadCT_DLbl, pNewElem);
			poVal->m_dLbl = pNewElem;
		}
		else if(c_oserct_pivotfmtEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_pivotFmts(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_pivotFmts* poVal = static_cast<CT_pivotFmts*>(poResult);
		if(c_oserct_pivotfmtsPIVOTFMT == type)
		{
			CT_PivotFmt* pNewElem = new CT_PivotFmt;
			READ1_DEF(length, res, this->ReadCT_PivotFmt, pNewElem);
			poVal->m_pivotFmt.push_back(pNewElem);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Chart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Chart* poVal = static_cast<CT_Chart*>(poResult);
		if(c_oserct_chartTITLE == type)
		{
			CT_Title* pNewElem = new CT_Title;
			READ1_DEF(length, res, this->ReadCT_Title, pNewElem);
			poVal->m_title = pNewElem;
		}
		else if(c_oserct_chartAUTOTITLEDELETED == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_autoTitleDeleted = pNewElem;
		}
		else if(c_oserct_chartPIVOTFMTS == type)
		{
			CT_pivotFmts* pNewElem = new CT_pivotFmts;
			READ1_DEF(length, res, this->ReadCT_pivotFmts, pNewElem);
			poVal->m_pivotFmts = pNewElem;
		}
		else if(c_oserct_chartVIEW3D == type)
		{
			CT_View3D* pNewElem = new CT_View3D;
			READ1_DEF(length, res, this->ReadCT_View3D, pNewElem);
			poVal->m_view3D = pNewElem;
		}
		else if(c_oserct_chartFLOOR == type)
		{
			CT_Surface* pNewElem = new CT_Surface;
			READ1_DEF(length, res, this->ReadCT_Surface, pNewElem);
			poVal->m_floor = pNewElem;
		}
		else if(c_oserct_chartSIDEWALL == type)
		{
			CT_Surface* pNewElem = new CT_Surface;
			READ1_DEF(length, res, this->ReadCT_Surface, pNewElem);
			poVal->m_sideWall = pNewElem;
		}
		else if(c_oserct_chartBACKWALL == type)
		{
			CT_Surface* pNewElem = new CT_Surface;
			READ1_DEF(length, res, this->ReadCT_Surface, pNewElem);
			poVal->m_backWall = pNewElem;
		}
		else if(c_oserct_chartPLOTAREA == type)
		{
			CT_PlotArea* pNewElem = new CT_PlotArea;
			READ1_DEF(length, res, this->ReadCT_PlotArea, pNewElem);
			poVal->m_plotArea = pNewElem;
		}
		else if(c_oserct_chartLEGEND == type)
		{
			CT_Legend* pNewElem = new CT_Legend;
			READ1_DEF(length, res, this->ReadCT_Legend, pNewElem);
			poVal->m_legend = pNewElem;
		}
		else if(c_oserct_chartPLOTVISONLY == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_plotVisOnly = pNewElem;
		}
		else if(c_oserct_chartDISPBLANKSAS == type)
		{
			CT_DispBlanksAs* pNewElem = new CT_DispBlanksAs;
			READ1_DEF(length, res, this->ReadCT_DispBlanksAs, pNewElem);
			poVal->m_dispBlanksAs = pNewElem;
		}
		else if(c_oserct_chartSHOWDLBLSOVERMAX == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_showDLblsOverMax = pNewElem;
		}
		else if(c_oserct_chartEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Protection(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Protection* poVal = static_cast<CT_Protection*>(poResult);
		if(c_oserct_protectionCHARTOBJECT == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_chartObject = pNewElem;
		}
		else if(c_oserct_protectionDATA == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_data = pNewElem;
		}
		else if(c_oserct_protectionFORMATTING == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_formatting = pNewElem;
		}
		else if(c_oserct_protectionSELECTION == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_selection = pNewElem;
		}
		else if(c_oserct_protectionUSERINTERFACE == type)
		{
			CT_Boolean* pNewElem = new CT_Boolean;
			READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
			poVal->m_userInterface = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_PivotSource(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_PivotSource* poVal = static_cast<CT_PivotSource*>(poResult);
		if(c_oserct_pivotsourceNAME == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_name = pNewElem;
		}
		else if(c_oserct_pivotsourceFMTID == type)
		{
			CT_UnsignedInt* pNewElem = new CT_UnsignedInt;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, pNewElem);
			poVal->m_fmtId = pNewElem;
		}
		else if(c_oserct_pivotsourceEXTLST == type)
		{
			CT_extLst* pNewElem = new CT_extLst;
			READ1_DEF(length, res, this->ReadCT_extLst, pNewElem);
			poVal->m_extLst.push_back(pNewElem);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Style1(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Style1* poVal = static_cast<CT_Style1*>(poResult);
		if(c_oserct_style1VAL == type)
		{
			unsigned char* pNewElem = new unsigned char;
			*pNewElem = m_oBufferedStream.GetUChar();
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Style(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Style* poVal = static_cast<CT_Style*>(poResult);
		if(c_oserct_styleVAL == type)
		{
			unsigned char* pNewElem = new unsigned char;
			*pNewElem = m_oBufferedStream.GetUChar();
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_TextLanguageID(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_TextLanguageID* poVal = static_cast<CT_TextLanguageID*>(poResult);
		if(c_oserct_textlanguageidVAL == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadAlternateContent(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		AlternateContent* poVal = static_cast<AlternateContent*>(poResult);
		if(c_oseralternatecontentCHOICE == type)
		{
			AlternateContentChoice* pNewElem = new AlternateContentChoice;
			READ1_DEF(length, res, this->ReadAlternateContentChoice, pNewElem);
			poVal->m_Choice.push_back(pNewElem);
		}
		else if(c_oseralternatecontentFALLBACK == type)
		{
			AlternateContentFallback* pNewElem = new AlternateContentFallback;
			READ1_DEF(length, res, this->ReadAlternateContentFallback, pNewElem);
			poVal->m_Fallback = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadAlternateContentChoice(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		AlternateContentChoice* poVal = static_cast<AlternateContentChoice*>(poResult);
		if(c_oseralternatecontentchoiceSTYLE == type)
		{
			CT_Style* pNewElem = new CT_Style;
			READ1_DEF(length, res, this->ReadCT_Style, pNewElem);
			poVal->m_style = pNewElem;
		}
		else if(c_oseralternatecontentchoiceREQUIRES == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_Requires = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadAlternateContentFallback(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		AlternateContentFallback* poVal = static_cast<AlternateContentFallback*>(poResult);
		if(c_oseralternatecontentfallbackSTYLE == type)
		{
			CT_Style1* pNewElem = new CT_Style1;
			READ1_DEF(length, res, this->ReadCT_Style1, pNewElem);
			poVal->m_style = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	BinaryChartWriter::BinaryChartWriter(NSBinPptxRW::CBinaryFileWriter &oBufferedStream, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter):m_oBcw(oBufferedStream),m_pOfficeDrawingConverter(pOfficeDrawingConverter)
	{}

 	void BinaryChartWriter::WriteCT_extLst(CT_extLst& oVal)
	{
		for(size_t i = 0, length = oVal.m_ext.size(); i < length; ++i)
		{
			CT_Extension* pVal = oVal.m_ext[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_extlstEXT);
				WriteCT_Extension(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
	}
	void BinaryChartWriter::WriteCT_ChartSpace(OOX::Spreadsheet::CChartSpace& oChartSpace)
	{
		CT_ChartSpace& oVal = oChartSpace.m_oChartSpace;
		if(NULL != oVal.m_date1904)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartspaceDATE1904);
			WriteCT_Boolean(*oVal.m_date1904);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_lang)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartspaceLANG);
			WriteCT_TextLanguageID(*oVal.m_lang);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_roundedCorners)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartspaceROUNDEDCORNERS);
			WriteCT_Boolean(*oVal.m_roundedCorners);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_AlternateContent)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartspaceALTERNATECONTENT);
			WriteAlternateContent(*oVal.m_AlternateContent);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_style)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartspaceSTYLE);
			WriteCT_Style1(*oVal.m_style);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(oVal.m_oClrMapOvr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartspaceCLRMAPOVR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oClrMapOvr);
			m_oBcw.WriteItemEnd(nCurPos);			
		}
		if(NULL != oVal.m_pivotSource)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartspacePIVOTSOURCE);
			WriteCT_PivotSource(*oVal.m_pivotSource);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_protection)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartspacePROTECTION);
			WriteCT_Protection(*oVal.m_protection);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_chart)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartspaceCHART);
			WriteCT_Chart(*oVal.m_chart);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oSpPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartspaceSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oSpPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oTxPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartspaceTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oTxPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		//if(NULL != oVal.m_externalData)
		//{
		//	int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartspaceEXTERNALDATA);
		//	WriteCT_ExternalData(*oVal.m_externalData);
		//	m_oBcw.WriteItemEnd(nCurPos);
		//}
		if(NULL != oVal.m_printSettings)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartspacePRINTSETTINGS);
			WriteCT_PrintSettings(*oVal.m_printSettings);
			m_oBcw.WriteItemEnd(nCurPos);
		}

		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartspaceEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}

		std::vector<smart_ptr<OOX::File>>& container = oChartSpace.GetContainer();

		for (size_t i = 0; i < container.size(); ++i)
		{
			if (OOX::FileTypes::ThemeOverride == container[i]->type())
			{
				PPTX::Theme* pThemeOverride = dynamic_cast<PPTX::Theme*>(container[i].GetPointer());
		
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartspaceTHEMEOVERRIDE);
				pThemeOverride->toPPTY(&m_oBcw.m_oStream);
				m_oBcw.WriteItemEnd(nCurPos);
				break;
			}
		}
		if ((NULL != oVal.m_userShapes) && (NULL != oVal.m_userShapes->m_id))
		{
			smart_ptr<OOX::File> oFile = oChartSpace.Find(*oVal.m_userShapes->m_id);
			
			if (oFile.IsInit() && OOX::FileTypes::ChartDrawing == oFile->type())
			{
				OOX::CChartDrawing* pDrawing = (OOX::CChartDrawing*)oFile.GetPointer();
				
				smart_ptr<OOX::IFileContainer> oldRels = m_pOfficeDrawingConverter->GetRels();
				m_pOfficeDrawingConverter->SetRels(pDrawing);
				
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartspaceUSERSHAPES);

				int nCurPos1 = m_oBcw.WriteItemStart(c_oserct_usershapes_COUNT);
					m_oBcw.m_oStream.WriteLONG(pDrawing->m_arrItems.size());
				m_oBcw.WriteItemEnd(nCurPos1);
				
				smart_ptr<OOX::IFileContainer> oldRelsStream = *m_oBcw.m_oStream.m_pCurrentContainer;
				*m_oBcw.m_oStream.m_pCurrentContainer = pDrawing;
				m_oBcw.m_oStream.m_pCurrentContainer->AddRef();

				for (size_t i = 0; i < pDrawing->m_arrItems.size(); i++)
				{
					OOX::CAbsSizeAnchor* absSize = dynamic_cast<OOX::CAbsSizeAnchor*>(pDrawing->m_arrItems[i]);
					OOX::CRelSizeAnchor* relSize = dynamic_cast<OOX::CRelSizeAnchor*>(pDrawing->m_arrItems[i]);
					
					int nCurPos2 = m_oBcw.WriteItemStart(relSize ? c_oserct_usershapes_SHAPE_REL : c_oserct_usershapes_SHAPE_ABS);
						WriteCT_Shape(pDrawing->m_arrItems[i]);
					m_oBcw.WriteItemEnd(nCurPos2);
				}
				
				m_oBcw.WriteItemEnd(nCurPos);
				
				*m_oBcw.m_oStream.m_pCurrentContainer = oldRelsStream;				
				m_pOfficeDrawingConverter->SetRels(oldRels);
			}
		}	
	}
	void BinaryChartWriter::WriteCT_Shape(OOX::CSizeAnchor *pVal)
	{
		OOX::CAbsSizeAnchor* absSize = dynamic_cast<OOX::CAbsSizeAnchor*>(pVal);
		OOX::CRelSizeAnchor* relSize = dynamic_cast<OOX::CRelSizeAnchor*>(pVal);

	//From
		int nCurPos = 0;
		if(pVal->m_oFrom.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSer_DrawingType::From);
				WriteCT_FromTo(*pVal->m_oFrom);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	//To
		if((relSize) && (relSize->m_oTo.IsInit()))
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSer_DrawingType::To);
				WriteCT_FromTo(*relSize->m_oTo);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	//Ext
		if((absSize) && (absSize->m_oExt.IsInit()))
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSer_DrawingType::Ext);
				WriteCT_Ext(*absSize->m_oExt);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_oElement.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_DrawingType::pptxDrawing);
			nCurPos = m_oBcw.WriteItemWithLengthStart();

			m_oBcw.m_oStream.StartRecord(0);
			m_oBcw.m_oStream.WriteRecord2(1, pVal->m_oElement->GetElem());
			m_oBcw.m_oStream.EndRecord();


			m_oBcw.WriteItemWithLengthEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_FromTo(OOX::CFromTo& oFromTo)
	{
		if(oFromTo.m_oX.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_DrawingPosType::X);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
			m_oBcw.m_oStream.WriteDoubleReal(oFromTo.m_oX.get());
		}
		if(oFromTo.m_oY.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_DrawingPosType::Y);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
			m_oBcw.m_oStream.WriteDoubleReal(oFromTo.m_oY.get());
		}
	}
	void BinaryChartWriter::WriteCT_Ext(OOX::CExt& oExt)
	{
		if(oExt.m_oCx.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_DrawingExtType::Cx);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
			m_oBcw.m_oStream.WriteDoubleReal(oExt.m_oCx.get());
		}
		if(oExt.m_oCy.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_DrawingExtType::Cy);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
			m_oBcw.m_oStream.WriteDoubleReal(oExt.m_oCy.get());
		}
	}
	void BinaryChartWriter::WriteCT_Boolean(CT_Boolean& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_booleanVAL);
			m_oBcw.m_oStream.WriteBOOL(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_RelId(CT_RelId& oVal)
	{
		if(NULL != oVal.m_id)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_relidID);
			//todo
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_id);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_PageSetup(CT_PageSetup& oVal)
	{
		if(NULL != oVal.m_paperSize)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagesetupPAPERSIZE);
			m_oBcw.m_oStream.WriteLONG(*oVal.m_paperSize);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_paperHeight)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagesetupPAPERHEIGHT);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_paperHeight);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_paperWidth)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagesetupPAPERWIDTH);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_paperWidth);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_firstPageNumber)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagesetupFIRSTPAGENUMBER);
			m_oBcw.m_oStream.WriteLONG(*oVal.m_firstPageNumber);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_orientation)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagesetupORIENTATION);
			int nVal = (int)(*oVal.m_orientation);
			m_oBcw.m_oStream.WriteBYTE(*&nVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_blackAndWhite)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagesetupBLACKANDWHITE);
			m_oBcw.m_oStream.WriteBOOL(*oVal.m_blackAndWhite);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_draft)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagesetupDRAFT);
			m_oBcw.m_oStream.WriteBOOL(*oVal.m_draft);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_useFirstPageNumber)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagesetupUSEFIRSTPAGENUMBER);
			m_oBcw.m_oStream.WriteBOOL(*oVal.m_useFirstPageNumber);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_horizontalDpi)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagesetupHORIZONTALDPI);
			m_oBcw.m_oStream.WriteLONG(*oVal.m_horizontalDpi);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_verticalDpi)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagesetupVERTICALDPI);
			m_oBcw.m_oStream.WriteLONG(*oVal.m_verticalDpi);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_copies)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagesetupCOPIES);
			m_oBcw.m_oStream.WriteLONG(*oVal.m_copies);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_PageMargins(CT_PageMargins& oVal)
	{
		if(NULL != oVal.m_l)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagemarginsL);
			m_oBcw.m_oStream.WriteDoubleReal(*oVal.m_l);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_r)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagemarginsR);
			m_oBcw.m_oStream.WriteDoubleReal(*oVal.m_r);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_t)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagemarginsT);
			m_oBcw.m_oStream.WriteDoubleReal(*oVal.m_t);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_b)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagemarginsB);
			m_oBcw.m_oStream.WriteDoubleReal(*oVal.m_b);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_header)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagemarginsHEADER);
			m_oBcw.m_oStream.WriteDoubleReal(*oVal.m_header);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_footer)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagemarginsFOOTER);
			m_oBcw.m_oStream.WriteDoubleReal(*oVal.m_footer);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_HeaderFooter(CT_HeaderFooter& oVal)
	{
		if(NULL != oVal.m_oddHeader)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_headerfooterODDHEADER);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_oddHeader);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_oddFooter)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_headerfooterODDFOOTER);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_oddFooter);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_evenHeader)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_headerfooterEVENHEADER);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_evenHeader);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_evenFooter)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_headerfooterEVENFOOTER);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_evenFooter);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_firstHeader)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_headerfooterFIRSTHEADER);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_firstHeader);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_firstFooter)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_headerfooterFIRSTFOOTER);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_firstFooter);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_alignWithMargins)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_headerfooterALIGNWITHMARGINS);
			m_oBcw.m_oStream.WriteBOOL(*oVal.m_alignWithMargins);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_differentOddEven)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_headerfooterDIFFERENTODDEVEN);
			m_oBcw.m_oStream.WriteBOOL(*oVal.m_differentOddEven);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_differentFirst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_headerfooterDIFFERENTFIRST);
			m_oBcw.m_oStream.WriteBOOL(*oVal.m_differentFirst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_PrintSettings(CT_PrintSettings& oVal)
	{
		if(NULL != oVal.m_headerFooter)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_printsettingsHEADERFOOTER);
			WriteCT_HeaderFooter(*oVal.m_headerFooter);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_pageMargins)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_printsettingsPAGEMARGINS);
			WriteCT_PageMargins(*oVal.m_pageMargins);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_pageSetup)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_printsettingsPAGESETUP);
			WriteCT_PageSetup(*oVal.m_pageSetup);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ExternalData(CT_ExternalData& oVal)
	{
		if(NULL != oVal.m_autoUpdate)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_externaldataAUTOUPDATE);
			WriteCT_Boolean(*oVal.m_autoUpdate);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_id)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_externaldataID);
			//todo
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_id);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_DispBlanksAs(CT_DispBlanksAs& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dispblanksasVAL);
			int nVal = (int)(*oVal.m_val);
			m_oBcw.m_oStream.WriteBYTE(*&nVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_LegendEntry(CT_LegendEntry& oVal)
	{
		if(NULL != oVal.m_idx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_legendentryIDX);
			WriteCT_UnsignedInt(*oVal.m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_delete)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_legendentryDELETE);
			WriteCT_Boolean(*oVal.m_delete);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oTxPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_legendentryTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oTxPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_legendentryEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_UnsignedInt(CT_UnsignedInt& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_unsignedintVAL);
			m_oBcw.m_oStream.WriteLONG(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_Extension(CT_Extension& oVal)
	{
		if(NULL != oVal.m_Any)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_extensionANY);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_Any);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_uri)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_extensionURI);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_uri);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_LegendPos(CT_LegendPos& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_legendposVAL);
			int nVal = (int)(*oVal.m_val);
			m_oBcw.m_oStream.WriteBYTE(*&nVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_Legend(CT_Legend& oVal)
	{
		if(NULL != oVal.m_legendPos)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_legendLEGENDPOS);
			WriteCT_LegendPos(*oVal.m_legendPos);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_legendEntry.size(); i < length; ++i)
		{
			CT_LegendEntry* pVal = oVal.m_legendEntry[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_legendLEGENDENTRY);
				WriteCT_LegendEntry(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_layout)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_legendLAYOUT);
			WriteCT_Layout(*oVal.m_layout);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_overlay)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_legendOVERLAY);
			WriteCT_Boolean(*oVal.m_overlay);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oSpPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_legendSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oSpPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oTxPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_legendTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oTxPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_legendEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_Layout(CT_Layout& oVal)
	{
		if(NULL != oVal.m_manualLayout)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_layoutMANUALLAYOUT);
			WriteCT_ManualLayout(*oVal.m_manualLayout);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_layoutEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ManualLayout(CT_ManualLayout& oVal)
	{
		if(NULL != oVal.m_layoutTarget)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_manuallayoutLAYOUTTARGET);
			WriteCT_LayoutTarget(*oVal.m_layoutTarget);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_xMode)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_manuallayoutXMODE);
			WriteCT_LayoutMode(*oVal.m_xMode);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_yMode)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_manuallayoutYMODE);
			WriteCT_LayoutMode(*oVal.m_yMode);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_wMode)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_manuallayoutWMODE);
			WriteCT_LayoutMode(*oVal.m_wMode);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_hMode)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_manuallayoutHMODE);
			WriteCT_LayoutMode(*oVal.m_hMode);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_x)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_manuallayoutX);
			WriteCT_Double(*oVal.m_x);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_y)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_manuallayoutY);
			WriteCT_Double(*oVal.m_y);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_w)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_manuallayoutW);
			WriteCT_Double(*oVal.m_w);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_h)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_manuallayoutH);
			WriteCT_Double(*oVal.m_h);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_manuallayoutEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_LayoutTarget(CT_LayoutTarget& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_layouttargetVAL);
			int nVal = (int)(*oVal.m_val);
			m_oBcw.m_oStream.WriteBYTE(*&nVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_LayoutMode(CT_LayoutMode& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_layoutmodeVAL);
			int nVal = (int)(*oVal.m_val);
			m_oBcw.m_oStream.WriteBYTE(*&nVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_Double(CT_Double& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_doubleVAL);
			m_oBcw.m_oStream.WriteDoubleReal(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_DTable(CT_DTable& oVal)
	{
		if(NULL != oVal.m_showHorzBorder)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dtableSHOWHORZBORDER);
			WriteCT_Boolean(*oVal.m_showHorzBorder);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_showVertBorder)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dtableSHOWVERTBORDER);
			WriteCT_Boolean(*oVal.m_showVertBorder);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_showOutline)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dtableSHOWOUTLINE);
			WriteCT_Boolean(*oVal.m_showOutline);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_showKeys)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dtableSHOWKEYS);
			WriteCT_Boolean(*oVal.m_showKeys);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oSpPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dtableSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oSpPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oTxPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dtableTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oTxPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dtableEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_SerAx(CT_SerAx& oVal)
	{
		if(NULL != oVal.m_axId)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxAXID);
			WriteCT_UnsignedInt(*oVal.m_axId);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_scaling)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxSCALING);
			WriteCT_Scaling(*oVal.m_scaling);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_delete)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxDELETE);
			WriteCT_Boolean(*oVal.m_delete);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_axPos)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxAXPOS);
			WriteCT_AxPos(*oVal.m_axPos);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_majorGridlines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxMAJORGRIDLINES);
			WriteCT_ChartLines(*oVal.m_majorGridlines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_minorGridlines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxMINORGRIDLINES);
			WriteCT_ChartLines(*oVal.m_minorGridlines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_title)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxTITLE);
			WriteCT_Title(*oVal.m_title);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_numFmt)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxNUMFMT);
			WriteCT_NumFmt(*oVal.m_numFmt);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_majorTickMark)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxMAJORTICKMARK);
			WriteCT_TickMark(*oVal.m_majorTickMark);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_minorTickMark)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxMINORTICKMARK);
			WriteCT_TickMark(*oVal.m_minorTickMark);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_tickLblPos)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxTICKLBLPOS);
			WriteCT_TickLblPos(*oVal.m_tickLblPos);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oSpPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oSpPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oTxPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oTxPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_crossAx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxCROSSAX);
			WriteCT_UnsignedInt(*oVal.m_crossAx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_crosses)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxCROSSES);
			WriteCT_Crosses(*oVal.m_crosses);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_crossesAt)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxCROSSESAT);
			WriteCT_Double(*oVal.m_crossesAt);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_tickLblSkip)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxTICKLBLSKIP);
			WriteCT_Skip(*oVal.m_tickLblSkip);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_tickMarkSkip)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxTICKMARKSKIP);
			WriteCT_Skip(*oVal.m_tickMarkSkip);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_Scaling(CT_Scaling& oVal)
	{
		if(NULL != oVal.m_logBase)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scalingLOGBASE);
			WriteCT_LogBase(*oVal.m_logBase);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_orientation)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scalingORIENTATION);
			WriteCT_Orientation(*oVal.m_orientation);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_max)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scalingMAX);
			WriteCT_Double(*oVal.m_max);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_min)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scalingMIN);
			WriteCT_Double(*oVal.m_min);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scalingEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_LogBase(CT_LogBase& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_logbaseVAL);
			m_oBcw.m_oStream.WriteDoubleReal(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_Orientation(CT_Orientation& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_orientationVAL);
			int nVal = (int)(*oVal.m_val);
			m_oBcw.m_oStream.WriteBYTE(*&nVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_AxPos(CT_AxPos& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_axposVAL);
			int nVal = (int)(*oVal.m_val);
			m_oBcw.m_oStream.WriteBYTE(*&nVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ChartLines(CT_ChartLines& oVal)
	{
		if (oVal.m_oSpPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartlinesSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oSpPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_Title(CT_Title& oVal)
	{
		if(NULL != oVal.m_tx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_titleTX);
			WriteCT_Tx(*oVal.m_tx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_layout)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_titleLAYOUT);
			WriteCT_Layout(*oVal.m_layout);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_overlay)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_titleOVERLAY);
			WriteCT_Boolean(*oVal.m_overlay);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oSpPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_titleSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oSpPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oTxPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_titleTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oTxPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_titleEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_Tx(CT_Tx& oVal)
	{
		if(oVal.m_oRich.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_txRICH);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oRich);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_strRef)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_txSTRREF);
			WriteCT_StrRef(*oVal.m_strRef);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_StrRef(CT_StrRef& oVal)
	{
		if(NULL != oVal.m_f)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_strrefF);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_f);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_strCache)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_strrefSTRCACHE);
			WriteCT_StrData(*oVal.m_strCache);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_strrefEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_StrData(CT_StrData& oVal)
	{
		if(NULL != oVal.m_ptCount)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_strdataPTCOUNT);
			WriteCT_UnsignedInt(*oVal.m_ptCount);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_pt.size(); i < length; ++i)
		{
			CT_StrVal* pVal = oVal.m_pt[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_strdataPT);
				WriteCT_StrVal(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_strdataEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_StrVal(CT_StrVal& oVal)
	{
		if(NULL != oVal.m_v)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_strvalV);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_v);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_idx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_strvalIDX);
			m_oBcw.m_oStream.WriteLONG(*oVal.m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_NumFmt(CT_NumFmt& oVal)
	{
		if(NULL != oVal.m_formatCode)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_numfmtFORMATCODE);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_formatCode);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_sourceLinked)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_numfmtSOURCELINKED);
			m_oBcw.m_oStream.WriteBOOL(*oVal.m_sourceLinked);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_TickMark(CT_TickMark& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_tickmarkVAL);
			int nVal = (int)(*oVal.m_val);
			m_oBcw.m_oStream.WriteBYTE(*&nVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_TickLblPos(CT_TickLblPos& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_ticklblposVAL);
			int nVal = (int)(*oVal.m_val);
			m_oBcw.m_oStream.WriteBYTE(*&nVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_Crosses(CT_Crosses& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_crossesVAL);
			int nVal = (int)(*oVal.m_val);
			m_oBcw.m_oStream.WriteBYTE(*&nVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_Skip(CT_Skip& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_skipVAL);
			m_oBcw.m_oStream.WriteLONG(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_TimeUnit(CT_TimeUnit& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_timeunitVAL);
			int nVal = (int)(*oVal.m_val);
			m_oBcw.m_oStream.WriteBYTE(*&nVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_DateAx(CT_DateAx& oVal)
	{
		if(NULL != oVal.m_axId)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxAXID);
			WriteCT_UnsignedInt(*oVal.m_axId);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_scaling)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxSCALING);
			WriteCT_Scaling(*oVal.m_scaling);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_delete)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxDELETE);
			WriteCT_Boolean(*oVal.m_delete);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_axPos)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxAXPOS);
			WriteCT_AxPos(*oVal.m_axPos);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_majorGridlines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxMAJORGRIDLINES);
			WriteCT_ChartLines(*oVal.m_majorGridlines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_minorGridlines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxMINORGRIDLINES);
			WriteCT_ChartLines(*oVal.m_minorGridlines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_title)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxTITLE);
			WriteCT_Title(*oVal.m_title);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_numFmt)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxNUMFMT);
			WriteCT_NumFmt(*oVal.m_numFmt);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_majorTickMark)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxMAJORTICKMARK);
			WriteCT_TickMark(*oVal.m_majorTickMark);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_minorTickMark)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxMINORTICKMARK);
			WriteCT_TickMark(*oVal.m_minorTickMark);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_tickLblPos)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxTICKLBLPOS);
			WriteCT_TickLblPos(*oVal.m_tickLblPos);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oSpPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oSpPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oTxPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oTxPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_crossAx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxCROSSAX);
			WriteCT_UnsignedInt(*oVal.m_crossAx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_crosses)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxCROSSES);
			WriteCT_Crosses(*oVal.m_crosses);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_crossesAt)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxCROSSESAT);
			WriteCT_Double(*oVal.m_crossesAt);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_auto)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxAUTO);
			WriteCT_Boolean(*oVal.m_auto);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_lblOffset)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxLBLOFFSET);
			WriteCT_LblOffset(*oVal.m_lblOffset);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_baseTimeUnit)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxBASETIMEUNIT);
			WriteCT_TimeUnit(*oVal.m_baseTimeUnit);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_majorUnit)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxMAJORUNIT);
			WriteCT_AxisUnit(*oVal.m_majorUnit);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_majorTimeUnit)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxMAJORTIMEUNIT);
			WriteCT_TimeUnit(*oVal.m_majorTimeUnit);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_minorUnit)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxMINORUNIT);
			WriteCT_AxisUnit(*oVal.m_minorUnit);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_minorTimeUnit)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxMINORTIMEUNIT);
			WriteCT_TimeUnit(*oVal.m_minorTimeUnit);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_LblOffset(CT_LblOffset& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_lbloffsetVAL);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_AxisUnit(CT_AxisUnit& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_axisunitVAL);
			m_oBcw.m_oStream.WriteDoubleReal(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_LblAlgn(CT_LblAlgn& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_lblalgnVAL);
			int nVal = (int)(*oVal.m_val);
			m_oBcw.m_oStream.WriteBYTE(*&nVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_CatAx(CT_CatAx& oVal)
	{
		if(NULL != oVal.m_axId)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxAXID);
			WriteCT_UnsignedInt(*oVal.m_axId);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_scaling)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxSCALING);
			WriteCT_Scaling(*oVal.m_scaling);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_delete)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxDELETE);
			WriteCT_Boolean(*oVal.m_delete);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_axPos)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxAXPOS);
			WriteCT_AxPos(*oVal.m_axPos);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_majorGridlines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxMAJORGRIDLINES);
			WriteCT_ChartLines(*oVal.m_majorGridlines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_minorGridlines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxMINORGRIDLINES);
			WriteCT_ChartLines(*oVal.m_minorGridlines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_title)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxTITLE);
			WriteCT_Title(*oVal.m_title);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_numFmt)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxNUMFMT);
			WriteCT_NumFmt(*oVal.m_numFmt);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_majorTickMark)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxMAJORTICKMARK);
			WriteCT_TickMark(*oVal.m_majorTickMark);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_minorTickMark)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxMINORTICKMARK);
			WriteCT_TickMark(*oVal.m_minorTickMark);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_tickLblPos)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxTICKLBLPOS);
			WriteCT_TickLblPos(*oVal.m_tickLblPos);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oSpPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oSpPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oTxPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oTxPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_crossAx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxCROSSAX);
			WriteCT_UnsignedInt(*oVal.m_crossAx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_crosses)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxCROSSES);
			WriteCT_Crosses(*oVal.m_crosses);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_crossesAt)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxCROSSESAT);
			WriteCT_Double(*oVal.m_crossesAt);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_auto)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxAUTO);
			WriteCT_Boolean(*oVal.m_auto);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_lblAlgn)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxLBLALGN);
			WriteCT_LblAlgn(*oVal.m_lblAlgn);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_lblOffset)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxLBLOFFSET);
			WriteCT_LblOffset(*oVal.m_lblOffset);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_tickLblSkip)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxTICKLBLSKIP);
			WriteCT_Skip(*oVal.m_tickLblSkip);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_tickMarkSkip)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxTICKMARKSKIP);
			WriteCT_Skip(*oVal.m_tickMarkSkip);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_noMultiLvlLbl)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxNOMULTILVLLBL);
			WriteCT_Boolean(*oVal.m_noMultiLvlLbl);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_DispUnitsLbl(CT_DispUnitsLbl& oVal)
	{
		if(NULL != oVal.m_layout)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dispunitslblLAYOUT);
			WriteCT_Layout(*oVal.m_layout);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_tx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dispunitslblTX);
			WriteCT_Tx(*oVal.m_tx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oSpPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dispunitslblSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oSpPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oTxPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dispunitslblTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oTxPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_BuiltInUnit(CT_BuiltInUnit& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_builtinunitVAL);
			int nVal = (int)(*oVal.m_val);
			m_oBcw.m_oStream.WriteBYTE(*&nVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_DispUnits(CT_DispUnits& oVal)
	{
		if(NULL != oVal.m_builtInUnit)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dispunitsBUILTINUNIT);
			WriteCT_BuiltInUnit(*oVal.m_builtInUnit);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_custUnit)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dispunitsCUSTUNIT);
			WriteCT_Double(*oVal.m_custUnit);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_dispUnitsLbl)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dispunitsDISPUNITSLBL);
			WriteCT_DispUnitsLbl(*oVal.m_dispUnitsLbl);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dispunitsEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_CrossBetween(CT_CrossBetween& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_crossbetweenVAL);
			int nVal = (int)(*oVal.m_val);
			m_oBcw.m_oStream.WriteBYTE(*&nVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ValAx(CT_ValAx& oVal)
	{
		if(NULL != oVal.m_axId)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxAXID);
			WriteCT_UnsignedInt(*oVal.m_axId);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_scaling)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxSCALING);
			WriteCT_Scaling(*oVal.m_scaling);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_delete)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxDELETE);
			WriteCT_Boolean(*oVal.m_delete);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_axPos)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxAXPOS);
			WriteCT_AxPos(*oVal.m_axPos);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_majorGridlines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxMAJORGRIDLINES);
			WriteCT_ChartLines(*oVal.m_majorGridlines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_minorGridlines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxMINORGRIDLINES);
			WriteCT_ChartLines(*oVal.m_minorGridlines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_title)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxTITLE);
			WriteCT_Title(*oVal.m_title);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_numFmt)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxNUMFMT);
			WriteCT_NumFmt(*oVal.m_numFmt);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_majorTickMark)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxMAJORTICKMARK);
			WriteCT_TickMark(*oVal.m_majorTickMark);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_minorTickMark)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxMINORTICKMARK);
			WriteCT_TickMark(*oVal.m_minorTickMark);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_tickLblPos)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxTICKLBLPOS);
			WriteCT_TickLblPos(*oVal.m_tickLblPos);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oSpPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oSpPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oTxPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oTxPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_crossAx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxCROSSAX);
			WriteCT_UnsignedInt(*oVal.m_crossAx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_crosses)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxCROSSES);
			WriteCT_Crosses(*oVal.m_crosses);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_crossesAt)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxCROSSESAT);
			WriteCT_Double(*oVal.m_crossesAt);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_crossBetween)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxCROSSBETWEEN);
			WriteCT_CrossBetween(*oVal.m_crossBetween);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_majorUnit)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxMAJORUNIT);
			WriteCT_AxisUnit(*oVal.m_majorUnit);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_minorUnit)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxMINORUNIT);
			WriteCT_AxisUnit(*oVal.m_minorUnit);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_dispUnits)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxDISPUNITS);
			WriteCT_DispUnits(*oVal.m_dispUnits);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_SizeRepresents(CT_SizeRepresents& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_sizerepresentsVAL);
			int nVal = (int)(*oVal.m_val);
			m_oBcw.m_oStream.WriteBYTE(*&nVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_BubbleScale(CT_BubbleScale& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubblescaleVAL);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_BubbleSer(CT_BubbleSer& oVal)
	{
		if(NULL != oVal.m_idx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubbleserIDX);
			WriteCT_UnsignedInt(*oVal.m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_order)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubbleserORDER);
			WriteCT_UnsignedInt(*oVal.m_order);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_tx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubbleserTX);
			WriteCT_SerTx(*oVal.m_tx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oSpPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubbleserSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oSpPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_invertIfNegative)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubbleserINVERTIFNEGATIVE);
			WriteCT_Boolean(*oVal.m_invertIfNegative);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_dPt.size(); i < length; ++i)
		{
			CT_DPt* pVal = oVal.m_dPt[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubbleserDPT);
				WriteCT_DPt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubbleserDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_trendline.size(); i < length; ++i)
		{
			CT_Trendline* pVal = oVal.m_trendline[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubbleserTRENDLINE);
				WriteCT_Trendline(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		for(size_t i = 0, length = oVal.m_errBars.size(); i < length; ++i)
		{
			CT_ErrBars* pVal = oVal.m_errBars[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubbleserERRBARS);
				WriteCT_ErrBars(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_xVal)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubbleserXVAL);
			WriteCT_AxDataSource(*oVal.m_xVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_yVal)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubbleserYVAL);
			WriteCT_NumDataSource(*oVal.m_yVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_bubbleSize)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubbleserBUBBLESIZE);
			WriteCT_NumDataSource(*oVal.m_bubbleSize);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_bubble3D)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubbleserBUBBLE3D);
			WriteCT_Boolean(*oVal.m_bubble3D);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubbleserEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_SerTx(CT_SerTx& oVal)
	{
		if(NULL != oVal.m_strRef)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_sertxSTRREF);
			WriteCT_StrRef(*oVal.m_strRef);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_v)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_sertxV);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_v);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_DPt(CT_DPt& oVal)
	{
		if(NULL != oVal.m_idx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dptIDX);
			WriteCT_UnsignedInt(*oVal.m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_invertIfNegative)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dptINVERTIFNEGATIVE);
			WriteCT_Boolean(*oVal.m_invertIfNegative);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_marker)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dptMARKER);
			WriteCT_Marker(*oVal.m_marker);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_bubble3D)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dptBUBBLE3D);
			WriteCT_Boolean(*oVal.m_bubble3D);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_explosion)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dptEXPLOSION);
			WriteCT_UnsignedInt(*oVal.m_explosion);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oSpPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dptSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oSpPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_pictureOptions)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dptPICTUREOPTIONS);
			WriteCT_PictureOptions(*oVal.m_pictureOptions);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dptEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_Marker(CT_Marker& oVal)
	{
		if(NULL != oVal.m_symbol)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_markerSYMBOL);
			WriteCT_MarkerStyle(*oVal.m_symbol);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_size)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_markerSIZE);
			WriteCT_MarkerSize(*oVal.m_size);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oSpPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_markerSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oSpPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_markerEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_MarkerStyle(CT_MarkerStyle& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_markerstyleVAL);
			int nVal = (int)(*oVal.m_val);
			m_oBcw.m_oStream.WriteBYTE(*&nVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_MarkerSize(CT_MarkerSize& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_markersizeVAL);
			m_oBcw.m_oStream.WriteBYTE(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_PictureOptions(CT_PictureOptions& oVal)
	{
		if(NULL != oVal.m_applyToFront)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pictureoptionsAPPLYTOFRONT);
			WriteCT_Boolean(*oVal.m_applyToFront);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_applyToSides)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pictureoptionsAPPLYTOSIDES);
			WriteCT_Boolean(*oVal.m_applyToSides);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_applyToEnd)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pictureoptionsAPPLYTOEND);
			WriteCT_Boolean(*oVal.m_applyToEnd);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_pictureFormat)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pictureoptionsPICTUREFORMAT);
			WriteCT_PictureFormat(*oVal.m_pictureFormat);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_pictureStackUnit)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pictureoptionsPICTURESTACKUNIT);
			WriteCT_PictureStackUnit(*oVal.m_pictureStackUnit);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_PictureFormat(CT_PictureFormat& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pictureformatVAL);
			int nVal = (int)(*oVal.m_val);
			m_oBcw.m_oStream.WriteBYTE(*&nVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_PictureStackUnit(CT_PictureStackUnit& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_picturestackunitVAL);
			m_oBcw.m_oStream.WriteDoubleReal(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_DLbls(CT_DLbls& oVal)
	{
		for(size_t i = 0, length = oVal.m_dLbl.size(); i < length; ++i)
		{
			CT_DLbl* pVal = oVal.m_dLbl[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblsDLBL);
				WriteCT_DLbl(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		for(size_t i = 0, length = oVal.m_Items.size(); i < length; ++i)
		{
			ItemsChoiceType3 eType = *oVal.m_ItemsElementName0[i];
			toBin(eType, oVal.m_Items[i]);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblsEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::toBin(ItemsChoiceType3 eType, void* pVal){
		switch(eType)
		{
		case itemschoicetype3DLBLPOS:
			{
				CT_DLblPos* pTypeVal = static_cast<CT_DLblPos*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblsDLBLPOS);
					WriteCT_DLblPos(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype3DELETE:
			{
				CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblsDELETE);
					WriteCT_Boolean(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype3LEADERLINES:
			{
				CT_ChartLines* pTypeVal = static_cast<CT_ChartLines*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblsLEADERLINES);
					WriteCT_ChartLines(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype3NUMFMT:
			{
				CT_NumFmt* pTypeVal = static_cast<CT_NumFmt*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblsNUMFMT);
					WriteCT_NumFmt(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype3SEPARATOR:
			{
				std::wstring* pTypeVal = static_cast<std::wstring*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblsSEPARATOR);
					m_oBcw.m_oStream.WriteStringW4(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype3SHOWBUBBLESIZE:
			{
				CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblsSHOWBUBBLESIZE);
					WriteCT_Boolean(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype3SHOWCATNAME:
			{
				CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblsSHOWCATNAME);
					WriteCT_Boolean(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype3SHOWLEADERLINES:
			{
				CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblsSHOWLEADERLINES);
					WriteCT_Boolean(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype3SHOWLEGENDKEY:
			{
				CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblsSHOWLEGENDKEY);
					WriteCT_Boolean(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype3SHOWPERCENT:
			{
				CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblsSHOWPERCENT);
					WriteCT_Boolean(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype3SHOWSERNAME:
			{
				CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblsSHOWSERNAME);
					WriteCT_Boolean(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype3SHOWVAL:
			{
				CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblsSHOWVAL);
					WriteCT_Boolean(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype3SPPR:
			{
				PPTX::Logic::SpPr* pTypeVal = static_cast<PPTX::Logic::SpPr*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblsSPPR);
					m_oBcw.m_oStream.WriteRecord1(0, (*pTypeVal));
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype3TXPR:
			{
				PPTX::Logic::TxBody* pTypeVal = static_cast<PPTX::Logic::TxBody*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblsTXPR);
					m_oBcw.m_oStream.WriteRecord1(0, (*pTypeVal));
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		}
	}
	void BinaryChartWriter::WriteCT_DLbl(CT_DLbl& oVal)
	{
		if(NULL != oVal.m_idx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblIDX);
			WriteCT_UnsignedInt(*oVal.m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_Items.size(); i < length; ++i)
		{
			ItemsChoiceType4 eType = *oVal.m_ItemsElementName0[i];
			toBin(eType, oVal.m_Items[i]);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::toBin(ItemsChoiceType4 eType, void* pVal){
		switch(eType)
		{
		case itemschoicetype4DLBLPOS:
			{
				CT_DLblPos* pTypeVal = static_cast<CT_DLblPos*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblDLBLPOS);
					WriteCT_DLblPos(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype4DELETE:
			{
				CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblDELETE);
					WriteCT_Boolean(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype4LAYOUT:
			{
				CT_Layout* pTypeVal = static_cast<CT_Layout*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblLAYOUT);
					WriteCT_Layout(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype4NUMFMT:
			{
				CT_NumFmt* pTypeVal = static_cast<CT_NumFmt*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblNUMFMT);
					WriteCT_NumFmt(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype4SEPARATOR:
			{
				std::wstring* pTypeVal = static_cast<std::wstring*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblSEPARATOR);
					m_oBcw.m_oStream.WriteStringW4(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype4SHOWBUBBLESIZE:
			{
				CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblSHOWBUBBLESIZE);
					WriteCT_Boolean(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype4SHOWCATNAME:
			{
				CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblSHOWCATNAME);
					WriteCT_Boolean(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype4SHOWLEGENDKEY:
			{
				CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblSHOWLEGENDKEY);
					WriteCT_Boolean(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype4SHOWPERCENT:
			{
				CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblSHOWPERCENT);
					WriteCT_Boolean(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype4SHOWSERNAME:
			{
				CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblSHOWSERNAME);
					WriteCT_Boolean(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype4SHOWVAL:
			{
				CT_Boolean* pTypeVal = static_cast<CT_Boolean*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblSHOWVAL);
					WriteCT_Boolean(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype4SPPR:
			{
				PPTX::Logic::SpPr* pTypeVal = static_cast<PPTX::Logic::SpPr*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblSPPR);
					m_oBcw.m_oStream.WriteRecord1(0, (*pTypeVal));
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype4TX:
			{
				CT_Tx* pTypeVal = static_cast<CT_Tx*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblTX);
					WriteCT_Tx(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype4TXPR:
			{
				PPTX::Logic::TxBody* pTypeVal = static_cast<PPTX::Logic::TxBody*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblTXPR);
					m_oBcw.m_oStream.WriteRecord1(0, (*pTypeVal));
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		}
	}
	void BinaryChartWriter::WriteCT_DLblPos(CT_DLblPos& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblposVAL);
			int nVal = (int)(*oVal.m_val);
			m_oBcw.m_oStream.WriteBYTE(*&nVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_Trendline(CT_Trendline& oVal)
	{
		if(NULL != oVal.m_name)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlineNAME);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_name);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oSpPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlineSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oSpPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_trendlineType)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlineTRENDLINETYPE);
			WriteCT_TrendlineType(*oVal.m_trendlineType);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_order)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlineORDER);
			WriteCT_Order(*oVal.m_order);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_period)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlinePERIOD);
			WriteCT_Period(*oVal.m_period);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_forward)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlineFORWARD);
			WriteCT_Double(*oVal.m_forward);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_backward)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlineBACKWARD);
			WriteCT_Double(*oVal.m_backward);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_intercept)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlineINTERCEPT);
			WriteCT_Double(*oVal.m_intercept);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_dispRSqr)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlineDISPRSQR);
			WriteCT_Boolean(*oVal.m_dispRSqr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_dispEq)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlineDISPEQ);
			WriteCT_Boolean(*oVal.m_dispEq);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_trendlineLbl)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlineTRENDLINELBL);
			WriteCT_TrendlineLbl(*oVal.m_trendlineLbl);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlineEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_TrendlineType(CT_TrendlineType& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlinetypeVAL);
			int nVal = (int)(*oVal.m_val);
			m_oBcw.m_oStream.WriteBYTE(*&nVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_Order(CT_Order& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_orderVAL);
			m_oBcw.m_oStream.WriteBYTE(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_Period(CT_Period& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_periodVAL);
			m_oBcw.m_oStream.WriteLONG(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_TrendlineLbl(CT_TrendlineLbl& oVal)
	{
		if(NULL != oVal.m_layout)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlinelblLAYOUT);
			WriteCT_Layout(*oVal.m_layout);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_tx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlinelblTX);
			WriteCT_Tx(*oVal.m_tx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_numFmt)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlinelblNUMFMT);
			WriteCT_NumFmt(*oVal.m_numFmt);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oSpPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlinelblSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oSpPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oTxPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlinelblTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oTxPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlinelblEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ErrBars(CT_ErrBars& oVal)
	{
		if(NULL != oVal.m_errDir)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_errbarsERRDIR);
			WriteCT_ErrDir(*oVal.m_errDir);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_errBarType)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_errbarsERRBARTYPE);
			WriteCT_ErrBarType(*oVal.m_errBarType);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_errValType)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_errbarsERRVALTYPE);
			WriteCT_ErrValType(*oVal.m_errValType);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_noEndCap)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_errbarsNOENDCAP);
			WriteCT_Boolean(*oVal.m_noEndCap);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_plus)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_errbarsPLUS);
			WriteCT_NumDataSource(*oVal.m_plus);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_minus)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_errbarsMINUS);
			WriteCT_NumDataSource(*oVal.m_minus);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_errbarsVAL);
			WriteCT_Double(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oSpPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_errbarsSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oSpPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_errbarsEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ErrDir(CT_ErrDir& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_errdirVAL);
			int nVal = (int)(*oVal.m_val);
			m_oBcw.m_oStream.WriteBYTE(*&nVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ErrBarType(CT_ErrBarType& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_errbartypeVAL);
			int nVal = (int)(*oVal.m_val);
			m_oBcw.m_oStream.WriteBYTE(*&nVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ErrValType(CT_ErrValType& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_errvaltypeVAL);
			int nVal = (int)(*oVal.m_val);
			m_oBcw.m_oStream.WriteBYTE(*&nVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_NumDataSource(CT_NumDataSource& oVal)
	{
		if(NULL != oVal.m_numLit)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_numdatasourceNUMLIT);
			WriteCT_NumData(*oVal.m_numLit);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_numRef)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_numdatasourceNUMREF);
			WriteCT_NumRef(*oVal.m_numRef);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_NumData(CT_NumData& oVal)
	{
		if(NULL != oVal.m_formatCode)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_numdataFORMATCODE);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_formatCode);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_ptCount)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_numdataPTCOUNT);
			WriteCT_UnsignedInt(*oVal.m_ptCount);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_pt.size(); i < length; ++i)
		{
			CT_NumVal* pVal = oVal.m_pt[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_numdataPT);
				WriteCT_NumVal(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_numdataEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_NumVal(CT_NumVal& oVal)
	{
		if(NULL != oVal.m_v)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_numvalV);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_v);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_idx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_numvalIDX);
			m_oBcw.m_oStream.WriteLONG(*oVal.m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_formatCode)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_numvalFORMATCODE);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_formatCode);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_NumRef(CT_NumRef& oVal)
	{
		if(NULL != oVal.m_f)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_numrefF);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_f);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_numCache)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_numrefNUMCACHE);
			WriteCT_NumData(*oVal.m_numCache);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_numrefEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_AxDataSource(CT_AxDataSource& oVal)
	{
		if(NULL != oVal.m_multiLvlStrRef)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_axdatasourceMULTILVLSTRREF);
			WriteCT_MultiLvlStrRef(*oVal.m_multiLvlStrRef);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_numLit)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_axdatasourceNUMLIT);
			WriteCT_NumData(*oVal.m_numLit);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_numRef)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_axdatasourceNUMREF);
			WriteCT_NumRef(*oVal.m_numRef);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_strLit)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_axdatasourceSTRLIT);
			WriteCT_StrData(*oVal.m_strLit);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_strRef)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_axdatasourceSTRREF);
			WriteCT_StrRef(*oVal.m_strRef);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_MultiLvlStrRef(CT_MultiLvlStrRef& oVal)
	{
		if(NULL != oVal.m_f)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_multilvlstrrefF);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_f);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_multiLvlStrCache)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_multilvlstrrefMULTILVLSTRCACHE);
			WriteCT_MultiLvlStrData(*oVal.m_multiLvlStrCache);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_multilvlstrrefEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_lvl(CT_lvl& oVal)
	{
		for(size_t i = 0, length = oVal.m_pt.size(); i < length; ++i)
		{
			CT_StrVal* pVal = oVal.m_pt[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_lvlPT);
				WriteCT_StrVal(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
	}
	void BinaryChartWriter::WriteCT_MultiLvlStrData(CT_MultiLvlStrData& oVal)
	{
		if(NULL != oVal.m_ptCount)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_multilvlstrdataPTCOUNT);
			WriteCT_UnsignedInt(*oVal.m_ptCount);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_lvl.size(); i < length; ++i)
		{
			CT_lvl* pVal = oVal.m_lvl[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_multilvlstrdataLVL);
				WriteCT_lvl(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_multilvlstrdataEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_BubbleChart(CT_BubbleChart& oVal)
	{
		if(NULL != oVal.m_varyColors)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubblechartVARYCOLORS);
			WriteCT_Boolean(*oVal.m_varyColors);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_BubbleSer* pVal = oVal.m_ser[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubblechartSER);
				WriteCT_BubbleSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubblechartDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_bubble3D)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubblechartBUBBLE3D);
			WriteCT_Boolean(*oVal.m_bubble3D);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_bubbleScale)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubblechartBUBBLESCALE);
			WriteCT_BubbleScale(*oVal.m_bubbleScale);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_showNegBubbles)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubblechartSHOWNEGBUBBLES);
			WriteCT_Boolean(*oVal.m_showNegBubbles);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_sizeRepresents)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubblechartSIZEREPRESENTS);
			WriteCT_SizeRepresents(*oVal.m_sizeRepresents);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_axId.size(); i < length; ++i)
		{
			CT_UnsignedInt* pVal = oVal.m_axId[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubblechartAXID);
				WriteCT_UnsignedInt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubblechartEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_bandFmts(CT_bandFmts& oVal)
	{
		for(size_t i = 0, length = oVal.m_bandFmt.size(); i < length; ++i)
		{
			CT_BandFmt* pVal = oVal.m_bandFmt[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_bandfmtsBANDFMT);
				WriteCT_BandFmt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
	}
	void BinaryChartWriter::WriteCT_Surface3DChart(CT_Surface3DChart& oVal)
	{
		if(NULL != oVal.m_wireframe)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surface3dchartWIREFRAME);
			WriteCT_Boolean(*oVal.m_wireframe);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_SurfaceSer* pVal = oVal.m_ser[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_surface3dchartSER);
				WriteCT_SurfaceSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_bandFmts)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surface3dchartBANDFMTS);
			WriteCT_bandFmts(*oVal.m_bandFmts);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_axId.size(); i < length; ++i)
		{
			CT_UnsignedInt* pVal = oVal.m_axId[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_surface3dchartAXID);
				WriteCT_UnsignedInt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surface3dchartEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_SurfaceSer(CT_SurfaceSer& oVal)
	{
		if(NULL != oVal.m_idx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surfaceserIDX);
			WriteCT_UnsignedInt(*oVal.m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_order)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surfaceserORDER);
			WriteCT_UnsignedInt(*oVal.m_order);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_tx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surfaceserTX);
			WriteCT_SerTx(*oVal.m_tx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oSpPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surfaceserSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oSpPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_cat)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surfaceserCAT);
			WriteCT_AxDataSource(*oVal.m_cat);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surfaceserVAL);
			WriteCT_NumDataSource(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surfaceserEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_BandFmt(CT_BandFmt& oVal)
	{
		if(NULL != oVal.m_idx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bandfmtIDX);
			WriteCT_UnsignedInt(*oVal.m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oSpPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bandfmtSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oSpPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_SurfaceChart(CT_SurfaceChart& oVal)
	{
		if(NULL != oVal.m_wireframe)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surfacechartWIREFRAME);
			WriteCT_Boolean(*oVal.m_wireframe);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_SurfaceSer* pVal = oVal.m_ser[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_surfacechartSER);
				WriteCT_SurfaceSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_bandFmts)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surfacechartBANDFMTS);
			WriteCT_bandFmts(*oVal.m_bandFmts);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_axId.size(); i < length; ++i)
		{
			CT_UnsignedInt* pVal = oVal.m_axId[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_surfacechartAXID);
				WriteCT_UnsignedInt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surfacechartEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_SecondPieSize(CT_SecondPieSize& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_secondpiesizeVAL);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_SplitType(CT_SplitType& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_splittypeVAL);
			int nVal = (int)(*oVal.m_val);
			m_oBcw.m_oStream.WriteBYTE(*&nVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_OfPieType(CT_OfPieType& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_ofpietypeVAL);
			int nVal = (int)(*oVal.m_val);
			m_oBcw.m_oStream.WriteBYTE(*&nVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_custSplit(CT_custSplit& oVal)
	{
		for(size_t i = 0, length = oVal.m_secondPiePt.size(); i < length; ++i)
		{
			CT_UnsignedInt* pVal = oVal.m_secondPiePt[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_custsplitSECONDPIEPT);
				WriteCT_UnsignedInt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
	}
	void BinaryChartWriter::WriteCT_OfPieChart(CT_OfPieChart& oVal)
	{
		if(NULL != oVal.m_ofPieType)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_ofpiechartOFPIETYPE);
			WriteCT_OfPieType(*oVal.m_ofPieType);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_varyColors)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_ofpiechartVARYCOLORS);
			WriteCT_Boolean(*oVal.m_varyColors);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_PieSer* pVal = oVal.m_ser[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_ofpiechartSER);
				WriteCT_PieSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_ofpiechartDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_gapWidth)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_ofpiechartGAPWIDTH);
			WriteCT_GapAmount(*oVal.m_gapWidth);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_splitType)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_ofpiechartSPLITTYPE);
			WriteCT_SplitType(*oVal.m_splitType);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_splitPos)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_ofpiechartSPLITPOS);
			WriteCT_Double(*oVal.m_splitPos);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_custSplit)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_ofpiechartCUSTSPLIT);
			WriteCT_custSplit(*oVal.m_custSplit);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_secondPieSize)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_ofpiechartSECONDPIESIZE);
			WriteCT_SecondPieSize(*oVal.m_secondPieSize);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_serLines.size(); i < length; ++i)
		{
			CT_ChartLines* pVal = oVal.m_serLines[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_ofpiechartSERLINES);
				WriteCT_ChartLines(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_ofpiechartEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_PieSer(CT_PieSer& oVal)
	{
		if(NULL != oVal.m_idx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pieserIDX);
			WriteCT_UnsignedInt(*oVal.m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_order)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pieserORDER);
			WriteCT_UnsignedInt(*oVal.m_order);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_tx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pieserTX);
			WriteCT_SerTx(*oVal.m_tx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oSpPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pieserSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oSpPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_explosion)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pieserEXPLOSION);
			WriteCT_UnsignedInt(*oVal.m_explosion);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_dPt.size(); i < length; ++i)
		{
			CT_DPt* pVal = oVal.m_dPt[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_pieserDPT);
				WriteCT_DPt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pieserDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_cat)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pieserCAT);
			WriteCT_AxDataSource(*oVal.m_cat);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pieserVAL);
			WriteCT_NumDataSource(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pieserEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_GapAmount(CT_GapAmount& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_gapamountVAL);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_Bar3DChart(CT_Bar3DChart& oVal)
	{
		if(NULL != oVal.m_barDir)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bar3dchartBARDIR);
			WriteCT_BarDir(*oVal.m_barDir);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_grouping)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bar3dchartGROUPING);
			WriteCT_BarGrouping(*oVal.m_grouping);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_varyColors)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bar3dchartVARYCOLORS);
			WriteCT_Boolean(*oVal.m_varyColors);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_BarSer* pVal = oVal.m_ser[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_bar3dchartSER);
				WriteCT_BarSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bar3dchartDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_gapWidth)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bar3dchartGAPWIDTH);
			WriteCT_GapAmount(*oVal.m_gapWidth);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_gapDepth)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bar3dchartGAPDEPTH);
			WriteCT_GapAmount(*oVal.m_gapDepth);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_shape)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bar3dchartSHAPE);
			WriteCT_Shape(*oVal.m_shape);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_axId.size(); i < length; ++i)
		{
			CT_UnsignedInt* pVal = oVal.m_axId[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_bar3dchartAXID);
				WriteCT_UnsignedInt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bar3dchartEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_BarDir(CT_BarDir& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bardirVAL);
			int nVal = (int)(*oVal.m_val);
			m_oBcw.m_oStream.WriteBYTE(*&nVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_BarGrouping(CT_BarGrouping& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bargroupingVAL);
			int nVal = (int)(*oVal.m_val);
			m_oBcw.m_oStream.WriteBYTE(*&nVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_BarSer(CT_BarSer& oVal)
	{
		if(NULL != oVal.m_idx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barserIDX);
			WriteCT_UnsignedInt(*oVal.m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_order)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barserORDER);
			WriteCT_UnsignedInt(*oVal.m_order);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_tx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barserTX);
			WriteCT_SerTx(*oVal.m_tx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oSpPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barserSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oSpPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_invertIfNegative)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barserINVERTIFNEGATIVE);
			WriteCT_Boolean(*oVal.m_invertIfNegative);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_pictureOptions)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barserPICTUREOPTIONS);
			WriteCT_PictureOptions(*oVal.m_pictureOptions);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_dPt.size(); i < length; ++i)
		{
			CT_DPt* pVal = oVal.m_dPt[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_barserDPT);
				WriteCT_DPt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barserDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_trendline.size(); i < length; ++i)
		{
			CT_Trendline* pVal = oVal.m_trendline[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_barserTRENDLINE);
				WriteCT_Trendline(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_errBars)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barserERRBARS);
			WriteCT_ErrBars(*oVal.m_errBars);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_cat)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barserCAT);
			WriteCT_AxDataSource(*oVal.m_cat);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barserVAL);
			WriteCT_NumDataSource(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_shape)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barserSHAPE);
			WriteCT_Shape(*oVal.m_shape);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barserEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_Shape(CT_Shape& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_shapeVAL);
			int nVal = (int)(*oVal.m_val);
			m_oBcw.m_oStream.WriteBYTE(*&nVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_Overlap(CT_Overlap& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_overlapVAL);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_BarChart(CT_BarChart& oVal)
	{
		if(NULL != oVal.m_barDir)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barchartBARDIR);
			WriteCT_BarDir(*oVal.m_barDir);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_grouping)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barchartGROUPING);
			WriteCT_BarGrouping(*oVal.m_grouping);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_varyColors)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barchartVARYCOLORS);
			WriteCT_Boolean(*oVal.m_varyColors);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_BarSer* pVal = oVal.m_ser[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_barchartSER);
				WriteCT_BarSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barchartDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_gapWidth)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barchartGAPWIDTH);
			WriteCT_GapAmount(*oVal.m_gapWidth);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_overlap)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barchartOVERLAP);
			WriteCT_Overlap(*oVal.m_overlap);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_serLines.size(); i < length; ++i)
		{
			CT_ChartLines* pVal = oVal.m_serLines[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_barchartSERLINES);
				WriteCT_ChartLines(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		for(size_t i = 0, length = oVal.m_axId.size(); i < length; ++i)
		{
			CT_UnsignedInt* pVal = oVal.m_axId[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_barchartAXID);
				WriteCT_UnsignedInt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barchartEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_HoleSize(CT_HoleSize& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_holesizeVAL);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_DoughnutChart(CT_DoughnutChart& oVal)
	{
		if(NULL != oVal.m_varyColors)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_doughnutchartVARYCOLORS);
			WriteCT_Boolean(*oVal.m_varyColors);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_PieSer* pVal = oVal.m_ser[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_doughnutchartSER);
				WriteCT_PieSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_doughnutchartDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_firstSliceAng)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_doughnutchartFIRSTSLICEANG);
			WriteCT_FirstSliceAng(*oVal.m_firstSliceAng);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_holeSize)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_doughnutchartHOLESIZE);
			WriteCT_HoleSize(*oVal.m_holeSize);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_doughnutchartEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_FirstSliceAng(CT_FirstSliceAng& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_firstsliceangVAL);
			m_oBcw.m_oStream.WriteLONG(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_Pie3DChart(CT_Pie3DChart& oVal)
	{
		if(NULL != oVal.m_varyColors)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pie3dchartVARYCOLORS);
			WriteCT_Boolean(*oVal.m_varyColors);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_PieSer* pVal = oVal.m_ser[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_pie3dchartSER);
				WriteCT_PieSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pie3dchartDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pie3dchartEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_PieChart(CT_PieChart& oVal)
	{
		if(NULL != oVal.m_varyColors)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_piechartVARYCOLORS);
			WriteCT_Boolean(*oVal.m_varyColors);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_PieSer* pVal = oVal.m_ser[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_piechartSER);
				WriteCT_PieSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_piechartDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_firstSliceAng)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_piechartFIRSTSLICEANG);
			WriteCT_FirstSliceAng(*oVal.m_firstSliceAng);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_piechartEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ScatterSer(CT_ScatterSer& oVal)
	{
		if(NULL != oVal.m_idx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterserIDX);
			WriteCT_UnsignedInt(*oVal.m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_order)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterserORDER);
			WriteCT_UnsignedInt(*oVal.m_order);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_tx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterserTX);
			WriteCT_SerTx(*oVal.m_tx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oSpPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterserSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oSpPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_marker)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterserMARKER);
			WriteCT_Marker(*oVal.m_marker);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_dPt.size(); i < length; ++i)
		{
			CT_DPt* pVal = oVal.m_dPt[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterserDPT);
				WriteCT_DPt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterserDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_trendline.size(); i < length; ++i)
		{
			CT_Trendline* pVal = oVal.m_trendline[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterserTRENDLINE);
				WriteCT_Trendline(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		for(size_t i = 0, length = oVal.m_errBars.size(); i < length; ++i)
		{
			CT_ErrBars* pVal = oVal.m_errBars[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterserERRBARS);
				WriteCT_ErrBars(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_xVal)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterserXVAL);
			WriteCT_AxDataSource(*oVal.m_xVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_yVal)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterserYVAL);
			WriteCT_NumDataSource(*oVal.m_yVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_smooth)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterserSMOOTH);
			WriteCT_Boolean(*oVal.m_smooth);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterserEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ScatterStyle(CT_ScatterStyle& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterstyleVAL);
			int nVal = (int)(*oVal.m_val);
			m_oBcw.m_oStream.WriteBYTE(*&nVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ScatterChart(CT_ScatterChart& oVal)
	{
		if(NULL != oVal.m_scatterStyle)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterchartSCATTERSTYLE);
			WriteCT_ScatterStyle(*oVal.m_scatterStyle);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_varyColors)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterchartVARYCOLORS);
			WriteCT_Boolean(*oVal.m_varyColors);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_ScatterSer* pVal = oVal.m_ser[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterchartSER);
				WriteCT_ScatterSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterchartDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_axId.size(); i < length; ++i)
		{
			CT_UnsignedInt* pVal = oVal.m_axId[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterchartAXID);
				WriteCT_UnsignedInt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterchartEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_RadarSer(CT_RadarSer& oVal)
	{
		if(NULL != oVal.m_idx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarserIDX);
			WriteCT_UnsignedInt(*oVal.m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_order)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarserORDER);
			WriteCT_UnsignedInt(*oVal.m_order);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_tx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarserTX);
			WriteCT_SerTx(*oVal.m_tx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oSpPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarserSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oSpPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_marker)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarserMARKER);
			WriteCT_Marker(*oVal.m_marker);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_dPt.size(); i < length; ++i)
		{
			CT_DPt* pVal = oVal.m_dPt[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarserDPT);
				WriteCT_DPt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarserDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_cat)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarserCAT);
			WriteCT_AxDataSource(*oVal.m_cat);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarserVAL);
			WriteCT_NumDataSource(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarserEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_RadarStyle(CT_RadarStyle& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarstyleVAL);
			int nVal = (int)(*oVal.m_val);
			m_oBcw.m_oStream.WriteBYTE(*&nVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_RadarChart(CT_RadarChart& oVal)
	{
		if(NULL != oVal.m_radarStyle)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarchartRADARSTYLE);
			WriteCT_RadarStyle(*oVal.m_radarStyle);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_varyColors)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarchartVARYCOLORS);
			WriteCT_Boolean(*oVal.m_varyColors);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_RadarSer* pVal = oVal.m_ser[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarchartSER);
				WriteCT_RadarSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarchartDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_axId.size(); i < length; ++i)
		{
			CT_UnsignedInt* pVal = oVal.m_axId[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarchartAXID);
				WriteCT_UnsignedInt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarchartEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_StockChart(CT_StockChart& oVal)
	{
		for(size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_LineSer* pVal = oVal.m_ser[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_stockchartSER);
				WriteCT_LineSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_stockchartDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_dropLines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_stockchartDROPLINES);
			WriteCT_ChartLines(*oVal.m_dropLines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_hiLowLines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_stockchartHILOWLINES);
			WriteCT_ChartLines(*oVal.m_hiLowLines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_upDownBars)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_stockchartUPDOWNBARS);
			WriteCT_UpDownBars(*oVal.m_upDownBars);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_axId.size(); i < length; ++i)
		{
			CT_UnsignedInt* pVal = oVal.m_axId[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_stockchartAXID);
				WriteCT_UnsignedInt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_stockchartEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_LineSer(CT_LineSer& oVal)
	{
		if(NULL != oVal.m_idx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_lineserIDX);
			WriteCT_UnsignedInt(*oVal.m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_order)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_lineserORDER);
			WriteCT_UnsignedInt(*oVal.m_order);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_tx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_lineserTX);
			WriteCT_SerTx(*oVal.m_tx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oSpPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_lineserSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oSpPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_marker)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_lineserMARKER);
			WriteCT_Marker(*oVal.m_marker);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_dPt.size(); i < length; ++i)
		{
			CT_DPt* pVal = oVal.m_dPt[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_lineserDPT);
				WriteCT_DPt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_lineserDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_trendline.size(); i < length; ++i)
		{
			CT_Trendline* pVal = oVal.m_trendline[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_lineserTRENDLINE);
				WriteCT_Trendline(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_errBars)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_lineserERRBARS);
			WriteCT_ErrBars(*oVal.m_errBars);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_cat)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_lineserCAT);
			WriteCT_AxDataSource(*oVal.m_cat);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_lineserVAL);
			WriteCT_NumDataSource(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_smooth)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_lineserSMOOTH);
			WriteCT_Boolean(*oVal.m_smooth);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_lineserEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_UpDownBars(CT_UpDownBars& oVal)
	{
		if(NULL != oVal.m_gapWidth)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_updownbarsGAPWIDTH);
			WriteCT_GapAmount(*oVal.m_gapWidth);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_upBars)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_updownbarsUPBARS);
			WriteCT_UpDownBar(*oVal.m_upBars);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_downBars)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_updownbarsDOWNBARS);
			WriteCT_UpDownBar(*oVal.m_downBars);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_updownbarsEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_UpDownBar(CT_UpDownBar& oVal)
	{
		if (oVal.m_oSpPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_updownbarSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oSpPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_Line3DChart(CT_Line3DChart& oVal)
	{
		if(NULL != oVal.m_grouping)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_line3dchartGROUPING);
			WriteCT_Grouping(*oVal.m_grouping);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_varyColors)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_line3dchartVARYCOLORS);
			WriteCT_Boolean(*oVal.m_varyColors);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_LineSer* pVal = oVal.m_ser[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_line3dchartSER);
				WriteCT_LineSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_line3dchartDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_dropLines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_line3dchartDROPLINES);
			WriteCT_ChartLines(*oVal.m_dropLines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_gapDepth)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_line3dchartGAPDEPTH);
			WriteCT_GapAmount(*oVal.m_gapDepth);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_axId.size(); i < length; ++i)
		{
			CT_UnsignedInt* pVal = oVal.m_axId[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_line3dchartAXID);
				WriteCT_UnsignedInt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_line3dchartEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_Grouping(CT_Grouping& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_groupingVAL);
			int nVal = (int)(*oVal.m_val);
			m_oBcw.m_oStream.WriteBYTE(*&nVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_LineChart(CT_LineChart& oVal)
	{
		if(NULL != oVal.m_grouping)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_linechartGROUPING);
			WriteCT_Grouping(*oVal.m_grouping);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_varyColors)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_linechartVARYCOLORS);
			WriteCT_Boolean(*oVal.m_varyColors);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_LineSer* pVal = oVal.m_ser[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_linechartSER);
				WriteCT_LineSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_linechartDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_dropLines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_linechartDROPLINES);
			WriteCT_ChartLines(*oVal.m_dropLines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_hiLowLines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_linechartHILOWLINES);
			WriteCT_ChartLines(*oVal.m_hiLowLines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_upDownBars)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_linechartUPDOWNBARS);
			WriteCT_UpDownBars(*oVal.m_upDownBars);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_marker)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_linechartMARKER);
			WriteCT_Boolean(*oVal.m_marker);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_smooth)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_linechartSMOOTH);
			WriteCT_Boolean(*oVal.m_smooth);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_axId.size(); i < length; ++i)
		{
			CT_UnsignedInt* pVal = oVal.m_axId[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_linechartAXID);
				WriteCT_UnsignedInt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_linechartEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_Area3DChart(CT_Area3DChart& oVal)
	{
		if(NULL != oVal.m_grouping)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_area3dchartGROUPING);
			WriteCT_Grouping(*oVal.m_grouping);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_varyColors)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_area3dchartVARYCOLORS);
			WriteCT_Boolean(*oVal.m_varyColors);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_AreaSer* pVal = oVal.m_ser[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_area3dchartSER);
				WriteCT_AreaSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_area3dchartDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_dropLines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_area3dchartDROPLINES);
			WriteCT_ChartLines(*oVal.m_dropLines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_gapDepth)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_area3dchartGAPDEPTH);
			WriteCT_GapAmount(*oVal.m_gapDepth);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_axId.size(); i < length; ++i)
		{
			CT_UnsignedInt* pVal = oVal.m_axId[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_area3dchartAXID);
				WriteCT_UnsignedInt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_area3dchartEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_AreaSer(CT_AreaSer& oVal)
	{
		if(NULL != oVal.m_idx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_areaserIDX);
			WriteCT_UnsignedInt(*oVal.m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_order)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_areaserORDER);
			WriteCT_UnsignedInt(*oVal.m_order);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_tx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_areaserTX);
			WriteCT_SerTx(*oVal.m_tx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oSpPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_areaserSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oSpPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_pictureOptions)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_areaserPICTUREOPTIONS);
			WriteCT_PictureOptions(*oVal.m_pictureOptions);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_dPt.size(); i < length; ++i)
		{
			CT_DPt* pVal = oVal.m_dPt[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_areaserDPT);
				WriteCT_DPt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_areaserDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_trendline.size(); i < length; ++i)
		{
			CT_Trendline* pVal = oVal.m_trendline[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_areaserTRENDLINE);
				WriteCT_Trendline(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		for(size_t i = 0, length = oVal.m_errBars.size(); i < length; ++i)
		{
			CT_ErrBars* pVal = oVal.m_errBars[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_areaserERRBARS);
				WriteCT_ErrBars(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_cat)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_areaserCAT);
			WriteCT_AxDataSource(*oVal.m_cat);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_areaserVAL);
			WriteCT_NumDataSource(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_areaserEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_AreaChart(CT_AreaChart& oVal)
	{
		if(NULL != oVal.m_grouping)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_areachartGROUPING);
			WriteCT_Grouping(*oVal.m_grouping);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_varyColors)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_areachartVARYCOLORS);
			WriteCT_Boolean(*oVal.m_varyColors);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_AreaSer* pVal = oVal.m_ser[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_areachartSER);
				WriteCT_AreaSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_areachartDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_dropLines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_areachartDROPLINES);
			WriteCT_ChartLines(*oVal.m_dropLines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_axId.size(); i < length; ++i)
		{
			CT_UnsignedInt* pVal = oVal.m_axId[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_areachartAXID);
				WriteCT_UnsignedInt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_areachartEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_PlotArea(CT_PlotArea& oVal)
	{
		if(NULL != oVal.m_layout)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_plotareaLAYOUT);
			WriteCT_Layout(*oVal.m_layout);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_Items.size(); i < length; ++i)
		{
			ItemsChoiceType5 eType = *oVal.m_ItemsElementName0[i];
			toBin(eType, oVal.m_Items[i]);
		}
		for(size_t i = 0, length = oVal.m_Items1.size(); i < length; ++i)
		{
			ItemsChoiceType6 eType = *oVal.m_ItemsElementName1[i];
			toBin(eType, oVal.m_Items1[i]);
		}
		if(NULL != oVal.m_dTable)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_plotareaDTABLE);
			WriteCT_DTable(*oVal.m_dTable);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oSpPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_plotareaSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oSpPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_plotareaEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::toBin(ItemsChoiceType5 eType, void* pVal){
		switch(eType)
		{
		case itemschoicetype5AREA3DCHART:
			{
				CT_Area3DChart* pTypeVal = static_cast<CT_Area3DChart*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_plotareaAREA3DCHART);
					WriteCT_Area3DChart(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype5AREACHART:
			{
				CT_AreaChart* pTypeVal = static_cast<CT_AreaChart*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_plotareaAREACHART);
					WriteCT_AreaChart(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype5BAR3DCHART:
			{
				CT_Bar3DChart* pTypeVal = static_cast<CT_Bar3DChart*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_plotareaBAR3DCHART);
					WriteCT_Bar3DChart(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype5BARCHART:
			{
				CT_BarChart* pTypeVal = static_cast<CT_BarChart*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_plotareaBARCHART);
					WriteCT_BarChart(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype5BUBBLECHART:
			{
				CT_BubbleChart* pTypeVal = static_cast<CT_BubbleChart*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_plotareaBUBBLECHART);
					WriteCT_BubbleChart(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype5DOUGHNUTCHART:
			{
				CT_DoughnutChart* pTypeVal = static_cast<CT_DoughnutChart*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_plotareaDOUGHNUTCHART);
					WriteCT_DoughnutChart(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype5LINE3DCHART:
			{
				CT_Line3DChart* pTypeVal = static_cast<CT_Line3DChart*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_plotareaLINE3DCHART);
					WriteCT_Line3DChart(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype5LINECHART:
			{
				CT_LineChart* pTypeVal = static_cast<CT_LineChart*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_plotareaLINECHART);
					WriteCT_LineChart(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype5OFPIECHART:
			{
				CT_OfPieChart* pTypeVal = static_cast<CT_OfPieChart*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_plotareaOFPIECHART);
					WriteCT_OfPieChart(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype5PIE3DCHART:
			{
				CT_Pie3DChart* pTypeVal = static_cast<CT_Pie3DChart*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_plotareaPIE3DCHART);
					WriteCT_Pie3DChart(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype5PIECHART:
			{
				CT_PieChart* pTypeVal = static_cast<CT_PieChart*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_plotareaPIECHART);
					WriteCT_PieChart(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype5RADARCHART:
			{
				CT_RadarChart* pTypeVal = static_cast<CT_RadarChart*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_plotareaRADARCHART);
					WriteCT_RadarChart(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype5SCATTERCHART:
			{
				CT_ScatterChart* pTypeVal = static_cast<CT_ScatterChart*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_plotareaSCATTERCHART);
					WriteCT_ScatterChart(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype5STOCKCHART:
			{
				CT_StockChart* pTypeVal = static_cast<CT_StockChart*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_plotareaSTOCKCHART);
					WriteCT_StockChart(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype5SURFACE3DCHART:
			{
				CT_Surface3DChart* pTypeVal = static_cast<CT_Surface3DChart*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_plotareaSURFACE3DCHART);
					WriteCT_Surface3DChart(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype5SURFACECHART:
			{
				CT_SurfaceChart* pTypeVal = static_cast<CT_SurfaceChart*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_plotareaSURFACECHART);
					WriteCT_SurfaceChart(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		}
	}
	void BinaryChartWriter::toBin(ItemsChoiceType6 eType, void* pVal){
		switch(eType)
		{
		case itemschoicetype6CATAX:
			{
				CT_CatAx* pTypeVal = static_cast<CT_CatAx*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_plotareaCATAX);
					WriteCT_CatAx(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype6DATEAX:
			{
				CT_DateAx* pTypeVal = static_cast<CT_DateAx*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_plotareaDATEAX);
					WriteCT_DateAx(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype6SERAX:
			{
				CT_SerAx* pTypeVal = static_cast<CT_SerAx*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_plotareaSERAX);
					WriteCT_SerAx(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype6VALAX:
			{
				CT_ValAx* pTypeVal = static_cast<CT_ValAx*>(pVal);
				if(NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_plotareaVALAX);
					WriteCT_ValAx(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		}
	}
	void BinaryChartWriter::WriteCT_Thickness(CT_Thickness& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_thicknessVAL);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_Surface(CT_Surface& oVal)
	{
		if(NULL != oVal.m_thickness)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surfaceTHICKNESS);
			WriteCT_Thickness(*oVal.m_thickness);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oSpPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surfaceSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oSpPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_pictureOptions)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surfacePICTUREOPTIONS);
			WriteCT_PictureOptions(*oVal.m_pictureOptions);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surfaceEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_Perspective(CT_Perspective& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_perspectiveVAL);
			m_oBcw.m_oStream.WriteBYTE(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_DepthPercent(CT_DepthPercent& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_depthpercentVAL);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_RotY(CT_RotY& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_rotyVAL);
			m_oBcw.m_oStream.WriteLONG(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_HPercent(CT_HPercent& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_hpercentVAL);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_RotX(CT_RotX& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_rotxVAL);
			m_oBcw.m_oStream.WriteSBYTE(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_View3D(CT_View3D& oVal)
	{
		if(NULL != oVal.m_rotX)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_view3dROTX);
			WriteCT_RotX(*oVal.m_rotX);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_hPercent)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_view3dHPERCENT);
			WriteCT_HPercent(*oVal.m_hPercent);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_rotY)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_view3dROTY);
			WriteCT_RotY(*oVal.m_rotY);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_depthPercent)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_view3dDEPTHPERCENT);
			WriteCT_DepthPercent(*oVal.m_depthPercent);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_rAngAx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_view3dRANGAX);
			WriteCT_Boolean(*oVal.m_rAngAx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_perspective)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_view3dPERSPECTIVE);
			WriteCT_Perspective(*oVal.m_perspective);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_view3dEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_PivotFmt(CT_PivotFmt& oVal)
	{
		if(NULL != oVal.m_idx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pivotfmtIDX);
			WriteCT_UnsignedInt(*oVal.m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oSpPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pivotfmtSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oSpPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oTxPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pivotfmtTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oTxPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_marker)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pivotfmtMARKER);
			WriteCT_Marker(*oVal.m_marker);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_dLbl)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pivotfmtDLBL);
			WriteCT_DLbl(*oVal.m_dLbl);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pivotfmtEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_pivotFmts(CT_pivotFmts& oVal)
	{
		for(size_t i = 0, length = oVal.m_pivotFmt.size(); i < length; ++i)
		{
			CT_PivotFmt* pVal = oVal.m_pivotFmt[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_pivotfmtsPIVOTFMT);
				WriteCT_PivotFmt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
	}
	void BinaryChartWriter::WriteCT_Chart(CT_Chart& oVal)
	{
		if(NULL != oVal.m_title)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartTITLE);
			WriteCT_Title(*oVal.m_title);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_autoTitleDeleted)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartAUTOTITLEDELETED);
			WriteCT_Boolean(*oVal.m_autoTitleDeleted);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_pivotFmts)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartPIVOTFMTS);
			WriteCT_pivotFmts(*oVal.m_pivotFmts);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_view3D)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartVIEW3D);
			WriteCT_View3D(*oVal.m_view3D);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_floor)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartFLOOR);
			WriteCT_Surface(*oVal.m_floor);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_sideWall)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartSIDEWALL);
			WriteCT_Surface(*oVal.m_sideWall);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_backWall)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartBACKWALL);
			WriteCT_Surface(*oVal.m_backWall);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_plotArea)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartPLOTAREA);
			WriteCT_PlotArea(*oVal.m_plotArea);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_legend)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartLEGEND);
			WriteCT_Legend(*oVal.m_legend);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_plotVisOnly)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartPLOTVISONLY);
			WriteCT_Boolean(*oVal.m_plotVisOnly);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_dispBlanksAs)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartDISPBLANKSAS);
			WriteCT_DispBlanksAs(*oVal.m_dispBlanksAs);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_showDLblsOverMax)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartSHOWDLBLSOVERMAX);
			WriteCT_Boolean(*oVal.m_showDLblsOverMax);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_extLst)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartEXTLST);
			WriteCT_extLst(*oVal.m_extLst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_Protection(CT_Protection& oVal)
	{
		if(NULL != oVal.m_chartObject)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_protectionCHARTOBJECT);
			WriteCT_Boolean(*oVal.m_chartObject);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_data)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_protectionDATA);
			WriteCT_Boolean(*oVal.m_data);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_formatting)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_protectionFORMATTING);
			WriteCT_Boolean(*oVal.m_formatting);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_selection)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_protectionSELECTION);
			WriteCT_Boolean(*oVal.m_selection);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_userInterface)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_protectionUSERINTERFACE);
			WriteCT_Boolean(*oVal.m_userInterface);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_PivotSource(CT_PivotSource& oVal)
	{
		if(NULL != oVal.m_name)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pivotsourceNAME);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_name);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_fmtId)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pivotsourceFMTID);
			WriteCT_UnsignedInt(*oVal.m_fmtId);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_extLst.size(); i < length; ++i)
		{
			CT_extLst* pVal = oVal.m_extLst[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_pivotsourceEXTLST);
				WriteCT_extLst(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
	}
	void BinaryChartWriter::WriteCT_Style1(CT_Style1& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_style1VAL);
			m_oBcw.m_oStream.WriteBYTE(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_Style(CT_Style& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_styleVAL);
			m_oBcw.m_oStream.WriteBYTE(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_TextLanguageID(CT_TextLanguageID& oVal)
	{
		if(NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_textlanguageidVAL);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteAlternateContent(AlternateContent& oVal)
	{
		for(size_t i = 0, length = oVal.m_Choice.size(); i < length; ++i)
		{
			AlternateContentChoice* pVal = oVal.m_Choice[i];
			if(NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oseralternatecontentCHOICE);
				WriteAlternateContentChoice(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if(NULL != oVal.m_Fallback)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oseralternatecontentFALLBACK);
			WriteAlternateContentFallback(*oVal.m_Fallback);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteAlternateContentChoice(AlternateContentChoice& oVal)
	{
		if(NULL != oVal.m_style)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oseralternatecontentchoiceSTYLE);
			WriteCT_Style(*oVal.m_style);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(NULL != oVal.m_Requires)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oseralternatecontentchoiceREQUIRES);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_Requires);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteAlternateContentFallback(AlternateContentFallback& oVal)
	{
		if(NULL != oVal.m_style)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oseralternatecontentfallbackSTYLE);
			WriteCT_Style1(*oVal.m_style);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
}
