/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "../Writer/BinaryReaderS.h"

#include "../../Presentation/BinReaderWriterDefines.h"
#include "../../Document/BinReader/DefaultThemeWriter.h"

#include "../../../XlsxFormat/Chart/Chart.h"
#include "../../../XlsxFormat/Chart/ChartDrawing.h"

#include "../../../PPTXFormat/Theme.h"

#include "../../Document/DocWrapper/XlsxSerializer.h"
#include "../../../../OfficeUtils/src/OfficeUtils.h"

using namespace OOX::Spreadsheet;

namespace BinXlsxRW
{
	SaveParams::SaveParams(const std::wstring& _sDrawingsPath, const std::wstring& _sEmbeddingsPath, const std::wstring& _sThemePath, OOX::CContentTypes* _pContentTypes, CSVWriter* _pCSVWriter, bool bMacro) :
		bMacroEnabled(bMacro), pContentTypes(_pContentTypes), sThemePath(_sThemePath), 
		sDrawingsPath(_sDrawingsPath), sEmbeddingsPath(_sEmbeddingsPath), pCSVWriter(_pCSVWriter)
	{
	}

	const BYTE c_oserct_extlstEXT = 0;

	const BYTE c_oserct_chartspaceDATE1904 = 0;
	const BYTE c_oserct_chartspaceLANG = 1;
	const BYTE c_oserct_chartspaceROUNDEDCORNERS = 2;
	const BYTE c_oserct_chartspaceALTERNATECONTENT = 3;
	const BYTE c_oserct_chartspaceSTYLE = 4;
	const BYTE c_oserct_chartspaceCLRMAPOVR = 5;
	const BYTE c_oserct_chartspacePIVOTSOURCE = 6;
	const BYTE c_oserct_chartspacePROTECTION = 7;
	const BYTE c_oserct_chartspaceCHART = 8;
	const BYTE c_oserct_chartspaceSPPR = 9;
	const BYTE c_oserct_chartspaceTXPR = 10;
	const BYTE c_oserct_chartspaceEXTERNALDATA = 11;
	const BYTE c_oserct_chartspacePRINTSETTINGS = 12;
	const BYTE c_oserct_chartspaceUSERSHAPES = 13;
	const BYTE c_oserct_chartspaceTHEMEOVERRIDE = 15;
	const BYTE c_oserct_chartspaceXLSX = 16;
	const BYTE c_oserct_chartspaceSTYLES = 17;
	const BYTE c_oserct_chartspaceCOLORS = 18;
	const BYTE c_oserct_chartspaceXLSXEXTERNAL = 19;
	const BYTE c_oserct_chartspaceXLSXZIP = 20;

	const BYTE c_oserct_usershapes_COUNT = 0;
	const BYTE c_oserct_usershapes_SHAPE_REL = 1;
	const BYTE c_oserct_usershapes_SHAPE_ABS = 2;

	const BYTE c_oserct_unsignedintVAL = 0;
	const BYTE c_oserct_booleanVAL = 0;
	const BYTE c_oserct_doubleVAL = 0;
	const BYTE c_oserct_stringVAL = 0;
	const BYTE c_oserct_byteVAL = 0;

	const BYTE c_oserct_pagesetupPAPERSIZE = 0;
	const BYTE c_oserct_pagesetupPAPERHEIGHT = 1;
	const BYTE c_oserct_pagesetupPAPERWIDTH = 2;
	const BYTE c_oserct_pagesetupFIRSTPAGENUMBER = 3;
	const BYTE c_oserct_pagesetupORIENTATION = 4;
	const BYTE c_oserct_pagesetupBLACKANDWHITE = 5;
	const BYTE c_oserct_pagesetupDRAFT = 6;
	const BYTE c_oserct_pagesetupUSEFIRSTPAGENUMBER = 7;
	const BYTE c_oserct_pagesetupHORIZONTALDPI = 8;
	const BYTE c_oserct_pagesetupVERTICALDPI = 9;
	const BYTE c_oserct_pagesetupCOPIES = 10;

	const BYTE c_oserct_pagemarginsL = 0;
	const BYTE c_oserct_pagemarginsR = 1;
	const BYTE c_oserct_pagemarginsT = 2;
	const BYTE c_oserct_pagemarginsB = 3;
	const BYTE c_oserct_pagemarginsHEADER = 4;
	const BYTE c_oserct_pagemarginsFOOTER = 5;

	const BYTE c_oserct_headerfooterODDHEADER = 0;
	const BYTE c_oserct_headerfooterODDFOOTER = 1;
	const BYTE c_oserct_headerfooterEVENHEADER = 2;
	const BYTE c_oserct_headerfooterEVENFOOTER = 3;
	const BYTE c_oserct_headerfooterFIRSTHEADER = 4;
	const BYTE c_oserct_headerfooterFIRSTFOOTER = 5;
	const BYTE c_oserct_headerfooterALIGNWITHMARGINS = 6;
	const BYTE c_oserct_headerfooterDIFFERENTODDEVEN = 7;
	const BYTE c_oserct_headerfooterDIFFERENTFIRST = 8;

	const BYTE c_oserct_printsettingsHEADERFOOTER = 0;
	const BYTE c_oserct_printsettingsPAGEMARGINS = 1;
	const BYTE c_oserct_printsettingsPAGESETUP = 2;

	const BYTE c_oserct_externaldataAUTOUPDATE = 0;

	const BYTE c_oserct_dispblanksasVAL = 0;

	const BYTE c_oserct_legendentryIDX = 0;
	const BYTE c_oserct_legendentryDELETE = 1;
	const BYTE c_oserct_legendentryTXPR = 2;

	const BYTE c_oserct_extensionANY = 0;
	const BYTE c_oserct_extensionURI = 1;

	const BYTE c_oserct_legendposVAL = 0;

	const BYTE c_oserct_legendLEGENDPOS = 0;
	const BYTE c_oserct_legendLEGENDENTRY = 1;
	const BYTE c_oserct_legendLAYOUT = 2;
	const BYTE c_oserct_legendOVERLAY = 3;
	const BYTE c_oserct_legendSPPR = 4;
	const BYTE c_oserct_legendTXPR = 5;

	const BYTE c_oserct_layoutMANUALLAYOUT = 0;

	const BYTE c_oserct_manuallayoutLAYOUTTARGET = 0;
	const BYTE c_oserct_manuallayoutXMODE = 1;
	const BYTE c_oserct_manuallayoutYMODE = 2;
	const BYTE c_oserct_manuallayoutWMODE = 3;
	const BYTE c_oserct_manuallayoutHMODE = 4;
	const BYTE c_oserct_manuallayoutX = 5;
	const BYTE c_oserct_manuallayoutY = 6;
	const BYTE c_oserct_manuallayoutW = 7;
	const BYTE c_oserct_manuallayoutH = 8;

	const BYTE c_oserct_layouttargetVAL = 0;

	const BYTE c_oserct_layoutmodeVAL = 0;



	const BYTE c_oserct_dtableSHOWHORZBORDER = 0;
	const BYTE c_oserct_dtableSHOWVERTBORDER = 1;
	const BYTE c_oserct_dtableSHOWOUTLINE = 2;
	const BYTE c_oserct_dtableSHOWKEYS = 3;
	const BYTE c_oserct_dtableSPPR = 4;
	const BYTE c_oserct_dtableTXPR = 5;

	const BYTE c_oserct_seraxAXID = 0;
	const BYTE c_oserct_seraxSCALING = 1;
	const BYTE c_oserct_seraxDELETE = 2;
	const BYTE c_oserct_seraxAXPOS = 3;
	const BYTE c_oserct_seraxMAJORGRIDLINES = 4;
	const BYTE c_oserct_seraxMINORGRIDLINES = 5;
	const BYTE c_oserct_seraxTITLE = 6;
	const BYTE c_oserct_seraxNUMFMT = 7;
	const BYTE c_oserct_seraxMAJORTICKMARK = 8;
	const BYTE c_oserct_seraxMINORTICKMARK = 9;
	const BYTE c_oserct_seraxTICKLBLPOS = 10;
	const BYTE c_oserct_seraxSPPR = 11;
	const BYTE c_oserct_seraxTXPR = 12;
	const BYTE c_oserct_seraxCROSSAX = 13;
	const BYTE c_oserct_seraxCROSSES = 14;
	const BYTE c_oserct_seraxCROSSESAT = 15;
	const BYTE c_oserct_seraxTICKLBLSKIP = 16;
	const BYTE c_oserct_seraxTICKMARKSKIP = 17;

	const BYTE c_oserct_scalingLOGBASE = 0;
	const BYTE c_oserct_scalingORIENTATION = 1;
	const BYTE c_oserct_scalingMAX = 2;
	const BYTE c_oserct_scalingMIN = 3;

	const BYTE c_oserct_logbaseVAL = 0;

	const BYTE c_oserct_orientationVAL = 0;

	const BYTE c_oserct_axposVAL = 0;

	const BYTE c_oserct_chartlinesSPPR = 0;

	const BYTE c_oserct_titleTX = 0;
	const BYTE c_oserct_titleLAYOUT = 1;
	const BYTE c_oserct_titleOVERLAY = 2;
	const BYTE c_oserct_titleSPPR = 3;
	const BYTE c_oserct_titleTXPR = 4;

	const BYTE c_oserct_txRICH = 0;
	const BYTE c_oserct_txSTRREF = 1;

	const BYTE c_oserct_strrefF = 0;
	const BYTE c_oserct_strrefSTRCACHE = 1;

	const BYTE c_oserct_strdataPTCOUNT = 0;
	const BYTE c_oserct_strdataPT = 1;

	const BYTE c_oserct_strvalV = 0;
	const BYTE c_oserct_strvalIDX = 1;

	const BYTE c_oserct_numfmtFORMATCODE = 0;
	const BYTE c_oserct_numfmtSOURCELINKED = 1;

	const BYTE c_oserct_tickmarkVAL = 0;

	const BYTE c_oserct_ticklblposVAL = 0;

	const BYTE c_oserct_crossesVAL = 0;

	const BYTE c_oserct_skipVAL = 0;

	const BYTE c_oserct_timeunitVAL = 0;

	const BYTE c_oserct_dateaxAXID = 0;
	const BYTE c_oserct_dateaxSCALING = 1;
	const BYTE c_oserct_dateaxDELETE = 2;
	const BYTE c_oserct_dateaxAXPOS = 3;
	const BYTE c_oserct_dateaxMAJORGRIDLINES = 4;
	const BYTE c_oserct_dateaxMINORGRIDLINES = 5;
	const BYTE c_oserct_dateaxTITLE = 6;
	const BYTE c_oserct_dateaxNUMFMT = 7;
	const BYTE c_oserct_dateaxMAJORTICKMARK = 8;
	const BYTE c_oserct_dateaxMINORTICKMARK = 9;
	const BYTE c_oserct_dateaxTICKLBLPOS = 10;
	const BYTE c_oserct_dateaxSPPR = 11;
	const BYTE c_oserct_dateaxTXPR = 12;
	const BYTE c_oserct_dateaxCROSSAX = 13;
	const BYTE c_oserct_dateaxCROSSES = 14;
	const BYTE c_oserct_dateaxCROSSESAT = 15;
	const BYTE c_oserct_dateaxAUTO = 16;
	const BYTE c_oserct_dateaxLBLOFFSET = 17;
	const BYTE c_oserct_dateaxBASETIMEUNIT = 18;
	const BYTE c_oserct_dateaxMAJORUNIT = 19;
	const BYTE c_oserct_dateaxMAJORTIMEUNIT = 20;
	const BYTE c_oserct_dateaxMINORUNIT = 21;
	const BYTE c_oserct_dateaxMINORTIMEUNIT = 22;

	const BYTE c_oserct_lbloffsetVAL = 0;

	const BYTE c_oserct_axisunitVAL = 0;

	const BYTE c_oserct_lblalgnVAL = 0;

	const BYTE c_oserct_cataxAXID = 0;
	const BYTE c_oserct_cataxSCALING = 1;
	const BYTE c_oserct_cataxDELETE = 2;
	const BYTE c_oserct_cataxAXPOS = 3;
	const BYTE c_oserct_cataxMAJORGRIDLINES = 4;
	const BYTE c_oserct_cataxMINORGRIDLINES = 5;
	const BYTE c_oserct_cataxTITLE = 6;
	const BYTE c_oserct_cataxNUMFMT = 7;
	const BYTE c_oserct_cataxMAJORTICKMARK = 8;
	const BYTE c_oserct_cataxMINORTICKMARK = 9;
	const BYTE c_oserct_cataxTICKLBLPOS = 10;
	const BYTE c_oserct_cataxSPPR = 11;
	const BYTE c_oserct_cataxTXPR = 12;
	const BYTE c_oserct_cataxCROSSAX = 13;
	const BYTE c_oserct_cataxCROSSES = 14;
	const BYTE c_oserct_cataxCROSSESAT = 15;
	const BYTE c_oserct_cataxAUTO = 16;
	const BYTE c_oserct_cataxLBLALGN = 17;
	const BYTE c_oserct_cataxLBLOFFSET = 18;
	const BYTE c_oserct_cataxTICKLBLSKIP = 19;
	const BYTE c_oserct_cataxTICKMARKSKIP = 20;
	const BYTE c_oserct_cataxNOMULTILVLLBL = 21;

	const BYTE c_oserct_dispunitslblLAYOUT = 0;
	const BYTE c_oserct_dispunitslblTX = 1;
	const BYTE c_oserct_dispunitslblSPPR = 2;
	const BYTE c_oserct_dispunitslblTXPR = 3;

	const BYTE c_oserct_builtinunitVAL = 0;

	const BYTE c_oserct_dispunitsBUILTINUNIT = 0;
	const BYTE c_oserct_dispunitsCUSTUNIT = 1;
	const BYTE c_oserct_dispunitsDISPUNITSLBL = 2;

	const BYTE c_oserct_crossbetweenVAL = 0;

	const BYTE c_oserct_valaxAXID = 0;
	const BYTE c_oserct_valaxSCALING = 1;
	const BYTE c_oserct_valaxDELETE = 2;
	const BYTE c_oserct_valaxAXPOS = 3;
	const BYTE c_oserct_valaxMAJORGRIDLINES = 4;
	const BYTE c_oserct_valaxMINORGRIDLINES = 5;
	const BYTE c_oserct_valaxTITLE = 6;
	const BYTE c_oserct_valaxNUMFMT = 7;
	const BYTE c_oserct_valaxMAJORTICKMARK = 8;
	const BYTE c_oserct_valaxMINORTICKMARK = 9;
	const BYTE c_oserct_valaxTICKLBLPOS = 10;
	const BYTE c_oserct_valaxSPPR = 11;
	const BYTE c_oserct_valaxTXPR = 12;
	const BYTE c_oserct_valaxCROSSAX = 13;
	const BYTE c_oserct_valaxCROSSES = 14;
	const BYTE c_oserct_valaxCROSSESAT = 15;
	const BYTE c_oserct_valaxCROSSBETWEEN = 16;
	const BYTE c_oserct_valaxMAJORUNIT = 17;
	const BYTE c_oserct_valaxMINORUNIT = 18;
	const BYTE c_oserct_valaxDISPUNITS = 19;

	const BYTE c_oserct_sizerepresentsVAL = 0;

	const BYTE c_oserct_bubblescaleVAL = 0;

	const BYTE c_oserct_bubbleserIDX = 0;
	const BYTE c_oserct_bubbleserORDER = 1;
	const BYTE c_oserct_bubbleserTX = 2;
	const BYTE c_oserct_bubbleserSPPR = 3;
	const BYTE c_oserct_bubbleserINVERTIFNEGATIVE = 4;
	const BYTE c_oserct_bubbleserDPT = 5;
	const BYTE c_oserct_bubbleserDLBLS = 6;
	const BYTE c_oserct_bubbleserTRENDLINE = 7;
	const BYTE c_oserct_bubbleserERRBARS = 8;
	const BYTE c_oserct_bubbleserXVAL = 9;
	const BYTE c_oserct_bubbleserYVAL = 10;
	const BYTE c_oserct_bubbleserBUBBLESIZE = 11;
	const BYTE c_oserct_bubbleserBUBBLE3D = 12;

	const BYTE c_oserct_sertxSTRREF = 0;
	const BYTE c_oserct_sertxV = 1;

	const BYTE c_oserct_dptIDX = 0;
	const BYTE c_oserct_dptINVERTIFNEGATIVE = 1;
	const BYTE c_oserct_dptMARKER = 2;
	const BYTE c_oserct_dptBUBBLE3D = 3;
	const BYTE c_oserct_dptEXPLOSION = 4;
	const BYTE c_oserct_dptSPPR = 5;
	const BYTE c_oserct_dptPICTUREOPTIONS = 6;

	const BYTE c_oserct_markerSYMBOL = 0;
	const BYTE c_oserct_markerSIZE = 1;
	const BYTE c_oserct_markerSPPR = 2;

	const BYTE c_oserct_markerstyleVAL = 0;

	const BYTE c_oserct_markersizeVAL = 0;

	const BYTE c_oserct_pictureoptionsAPPLYTOFRONT = 0;
	const BYTE c_oserct_pictureoptionsAPPLYTOSIDES = 1;
	const BYTE c_oserct_pictureoptionsAPPLYTOEND = 2;
	const BYTE c_oserct_pictureoptionsPICTUREFORMAT = 3;
	const BYTE c_oserct_pictureoptionsPICTURESTACKUNIT = 4;

	const BYTE c_oserct_pictureformatVAL = 0;

	const BYTE c_oserct_picturestackunitVAL = 0;

	const BYTE c_oserct_dlblsDLBL = 0;
	const BYTE c_oserct_dlblsITEMS = 1;
	const BYTE c_oserct_dlblsDLBLPOS = 2;
	const BYTE c_oserct_dlblsDELETE = 3;
	const BYTE c_oserct_dlblsLEADERLINES = 4;
	const BYTE c_oserct_dlblsNUMFMT = 5;
	const BYTE c_oserct_dlblsSEPARATOR = 6;
	const BYTE c_oserct_dlblsSHOWBUBBLESIZE = 7;
	const BYTE c_oserct_dlblsSHOWCATNAME = 8;
	const BYTE c_oserct_dlblsSHOWLEADERLINES = 9;
	const BYTE c_oserct_dlblsSHOWLEGENDKEY = 10;
	const BYTE c_oserct_dlblsSHOWPERCENT = 11;
	const BYTE c_oserct_dlblsSHOWSERNAME = 12;
	const BYTE c_oserct_dlblsSHOWVAL = 13;
	const BYTE c_oserct_dlblsSPPR = 14;
	const BYTE c_oserct_dlblsTXPR = 15;

	const BYTE c_oserct_dlblIDX = 0;
	const BYTE c_oserct_dlblITEMS = 1;
	const BYTE c_oserct_dlblDLBLPOS = 2;
	const BYTE c_oserct_dlblDELETE = 3;
	const BYTE c_oserct_dlblLAYOUT = 4;
	const BYTE c_oserct_dlblNUMFMT = 5;
	const BYTE c_oserct_dlblSEPARATOR = 6;
	const BYTE c_oserct_dlblSHOWBUBBLESIZE = 7;
	const BYTE c_oserct_dlblSHOWCATNAME = 8;
	const BYTE c_oserct_dlblSHOWLEGENDKEY = 9;
	const BYTE c_oserct_dlblSHOWPERCENT = 10;
	const BYTE c_oserct_dlblSHOWSERNAME = 11;
	const BYTE c_oserct_dlblSHOWVAL = 12;
	const BYTE c_oserct_dlblSPPR = 13;
	const BYTE c_oserct_dlblTX = 14;
	const BYTE c_oserct_dlblTXPR = 15;
	
	const BYTE c_oserct_xForSave = 25;
	const BYTE c_oserct_showDataLabelsRange = 26;
	const BYTE c_oserct_showLeaderLines = 27;
	const BYTE c_oserct_leaderLines = 28;
	const BYTE c_oserct_dlblFieldTable = 29;

	const BYTE c_oserct_dlblposVAL = 0;

	const BYTE c_oserct_trendlineNAME = 0;
	const BYTE c_oserct_trendlineSPPR = 1;
	const BYTE c_oserct_trendlineTRENDLINETYPE = 2;
	const BYTE c_oserct_trendlineORDER = 3;
	const BYTE c_oserct_trendlinePERIOD = 4;
	const BYTE c_oserct_trendlineFORWARD = 5;
	const BYTE c_oserct_trendlineBACKWARD = 6;
	const BYTE c_oserct_trendlineINTERCEPT = 7;
	const BYTE c_oserct_trendlineDISPRSQR = 8;
	const BYTE c_oserct_trendlineDISPEQ = 9;
	const BYTE c_oserct_trendlineTRENDLINELBL = 10;

	const BYTE c_oserct_trendlinetypeVAL = 0;

	const BYTE c_oserct_orderVAL = 0;

	const BYTE c_oserct_periodVAL = 0;

	const BYTE c_oserct_trendlinelblLAYOUT = 0;
	const BYTE c_oserct_trendlinelblTX = 1;
	const BYTE c_oserct_trendlinelblNUMFMT = 2;
	const BYTE c_oserct_trendlinelblSPPR = 3;
	const BYTE c_oserct_trendlinelblTXPR = 4;

	const BYTE c_oserct_errbarsERRDIR = 0;
	const BYTE c_oserct_errbarsERRBARTYPE = 1;
	const BYTE c_oserct_errbarsERRVALTYPE = 2;
	const BYTE c_oserct_errbarsNOENDCAP = 3;
	const BYTE c_oserct_errbarsPLUS = 4;
	const BYTE c_oserct_errbarsMINUS = 5;
	const BYTE c_oserct_errbarsVAL = 6;
	const BYTE c_oserct_errbarsSPPR = 7;

	const BYTE c_oserct_errdirVAL = 0;

	const BYTE c_oserct_errbartypeVAL = 0;

	const BYTE c_oserct_errvaltypeVAL = 0;

	const BYTE c_oserct_numdatasourceNUMLIT = 0;
	const BYTE c_oserct_numdatasourceNUMREF = 1;

	const BYTE c_oserct_numdataFORMATCODE = 0;
	const BYTE c_oserct_numdataPTCOUNT = 1;
	const BYTE c_oserct_numdataPT = 2;

	const BYTE c_oserct_numvalV = 0;
	const BYTE c_oserct_numvalIDX = 1;
	const BYTE c_oserct_numvalFORMATCODE = 2;

	const BYTE c_oserct_numrefF = 0;
	const BYTE c_oserct_numrefNUMCACHE = 1;

	const BYTE c_oserct_axdatasourceMULTILVLSTRREF = 0;
	const BYTE c_oserct_axdatasourceNUMLIT = 1;
	const BYTE c_oserct_axdatasourceNUMREF = 2;
	const BYTE c_oserct_axdatasourceSTRLIT = 3;
	const BYTE c_oserct_axdatasourceSTRREF = 4;

	const BYTE c_oserct_multilvlstrrefF = 0;
	const BYTE c_oserct_multilvlstrrefMULTILVLSTRCACHE = 1;

	const BYTE c_oserct_lvlPT = 0;

	const BYTE c_oserct_multilvlstrdataPTCOUNT = 0;
	const BYTE c_oserct_multilvlstrdataLVL = 1;

	const BYTE c_oserct_bubblechartVARYCOLORS = 0;
	const BYTE c_oserct_bubblechartSER = 1;
	const BYTE c_oserct_bubblechartDLBLS = 2;
	const BYTE c_oserct_bubblechartBUBBLE3D = 3;
	const BYTE c_oserct_bubblechartBUBBLESCALE = 4;
	const BYTE c_oserct_bubblechartSHOWNEGBUBBLES = 5;
	const BYTE c_oserct_bubblechartSIZEREPRESENTS = 6;
	const BYTE c_oserct_bubblechartAXID = 7;

	const BYTE c_oserct_bandfmtsBANDFMT = 0;

	const BYTE c_oserct_surface3dchartWIREFRAME = 0;
	const BYTE c_oserct_surface3dchartSER = 1;
	const BYTE c_oserct_surface3dchartBANDFMTS = 2;
	const BYTE c_oserct_surface3dchartAXID = 3;

	const BYTE c_oserct_surfaceserIDX = 0;
	const BYTE c_oserct_surfaceserORDER = 1;
	const BYTE c_oserct_surfaceserTX = 2;
	const BYTE c_oserct_surfaceserSPPR = 3;
	const BYTE c_oserct_surfaceserCAT = 4;
	const BYTE c_oserct_surfaceserVAL = 5;

	const BYTE c_oserct_bandfmtIDX = 0;
	const BYTE c_oserct_bandfmtSPPR = 1;

	const BYTE c_oserct_surfacechartWIREFRAME = 0;
	const BYTE c_oserct_surfacechartSER = 1;
	const BYTE c_oserct_surfacechartBANDFMTS = 2;
	const BYTE c_oserct_surfacechartAXID = 3;

	const BYTE c_oserct_secondpiesizeVAL = 0;

	const BYTE c_oserct_splittypeVAL = 0;

	const BYTE c_oserct_ofpietypeVAL = 0;

	const BYTE c_oserct_custsplitSECONDPIEPT = 0;

	const BYTE c_oserct_ofpiechartOFPIETYPE = 0;
	const BYTE c_oserct_ofpiechartVARYCOLORS = 1;
	const BYTE c_oserct_ofpiechartSER = 2;
	const BYTE c_oserct_ofpiechartDLBLS = 3;
	const BYTE c_oserct_ofpiechartGAPWIDTH = 4;
	const BYTE c_oserct_ofpiechartSPLITTYPE = 5;
	const BYTE c_oserct_ofpiechartSPLITPOS = 6;
	const BYTE c_oserct_ofpiechartCUSTSPLIT = 7;
	const BYTE c_oserct_ofpiechartSECONDPIESIZE = 8;
	const BYTE c_oserct_ofpiechartSERLINES = 9;

	const BYTE c_oserct_pieserIDX = 0;
	const BYTE c_oserct_pieserORDER = 1;
	const BYTE c_oserct_pieserTX = 2;
	const BYTE c_oserct_pieserSPPR = 3;
	const BYTE c_oserct_pieserEXPLOSION = 4;
	const BYTE c_oserct_pieserDPT = 5;
	const BYTE c_oserct_pieserDLBLS = 6;
	const BYTE c_oserct_pieserCAT = 7;
	const BYTE c_oserct_pieserVAL = 8;

	const BYTE c_oserct_gapamountVAL = 0;

	const BYTE c_oserct_bar3dchartBARDIR = 0;
	const BYTE c_oserct_bar3dchartGROUPING = 1;
	const BYTE c_oserct_bar3dchartVARYCOLORS = 2;
	const BYTE c_oserct_bar3dchartSER = 3;
	const BYTE c_oserct_bar3dchartDLBLS = 4;
	const BYTE c_oserct_bar3dchartGAPWIDTH = 5;
	const BYTE c_oserct_bar3dchartGAPDEPTH = 6;
	const BYTE c_oserct_bar3dchartSHAPE = 7;
	const BYTE c_oserct_bar3dchartAXID = 8;

	const BYTE c_oserct_bardirVAL = 0;

	const BYTE c_oserct_bargroupingVAL = 0;

	const BYTE c_oserct_barserIDX = 0;
	const BYTE c_oserct_barserORDER = 1;
	const BYTE c_oserct_barserTX = 2;
	const BYTE c_oserct_barserSPPR = 3;
	const BYTE c_oserct_barserINVERTIFNEGATIVE = 4;
	const BYTE c_oserct_barserPICTUREOPTIONS = 5;
	const BYTE c_oserct_barserDPT = 6;
	const BYTE c_oserct_barserDLBLS = 7;
	const BYTE c_oserct_barserTRENDLINE = 8;
	const BYTE c_oserct_barserERRBARS = 9;
	const BYTE c_oserct_barserCAT = 10;
	const BYTE c_oserct_barserVAL = 11;
	const BYTE c_oserct_barserSHAPE = 12;

	const BYTE c_oserct_shapeVAL = 0;

	const BYTE c_oserct_overlapVAL = 0;

	const BYTE c_oserct_barchartBARDIR = 0;
	const BYTE c_oserct_barchartGROUPING = 1;
	const BYTE c_oserct_barchartVARYCOLORS = 2;
	const BYTE c_oserct_barchartSER = 3;
	const BYTE c_oserct_barchartDLBLS = 4;
	const BYTE c_oserct_barchartGAPWIDTH = 5;
	const BYTE c_oserct_barchartOVERLAP = 6;
	const BYTE c_oserct_barchartSERLINES = 7;
	const BYTE c_oserct_barchartAXID = 8;

	const BYTE c_oserct_holesizeVAL = 0;

	const BYTE c_oserct_doughnutchartVARYCOLORS = 0;
	const BYTE c_oserct_doughnutchartSER = 1;
	const BYTE c_oserct_doughnutchartDLBLS = 2;
	const BYTE c_oserct_doughnutchartFIRSTSLICEANG = 3;
	const BYTE c_oserct_doughnutchartHOLESIZE = 4;

	const BYTE c_oserct_firstsliceangVAL = 0;

	const BYTE c_oserct_pie3dchartVARYCOLORS = 0;
	const BYTE c_oserct_pie3dchartSER = 1;
	const BYTE c_oserct_pie3dchartDLBLS = 2;

	const BYTE c_oserct_piechartVARYCOLORS = 0;
	const BYTE c_oserct_piechartSER = 1;
	const BYTE c_oserct_piechartDLBLS = 2;
	const BYTE c_oserct_piechartFIRSTSLICEANG = 3;

	const BYTE c_oserct_scatterserIDX = 0;
	const BYTE c_oserct_scatterserORDER = 1;
	const BYTE c_oserct_scatterserTX = 2;
	const BYTE c_oserct_scatterserSPPR = 3;
	const BYTE c_oserct_scatterserMARKER = 4;
	const BYTE c_oserct_scatterserDPT = 5;
	const BYTE c_oserct_scatterserDLBLS = 6;
	const BYTE c_oserct_scatterserTRENDLINE = 7;
	const BYTE c_oserct_scatterserERRBARS = 8;
	const BYTE c_oserct_scatterserXVAL = 9;
	const BYTE c_oserct_scatterserYVAL = 10;
	const BYTE c_oserct_scatterserSMOOTH = 11;

	const BYTE c_oserct_scatterstyleVAL = 0;

	const BYTE c_oserct_scatterchartSCATTERSTYLE = 0;
	const BYTE c_oserct_scatterchartVARYCOLORS = 1;
	const BYTE c_oserct_scatterchartSER = 2;
	const BYTE c_oserct_scatterchartDLBLS = 3;
	const BYTE c_oserct_scatterchartAXID = 4;

	const BYTE c_oserct_radarserIDX = 0;
	const BYTE c_oserct_radarserORDER = 1;
	const BYTE c_oserct_radarserTX = 2;
	const BYTE c_oserct_radarserSPPR = 3;
	const BYTE c_oserct_radarserMARKER = 4;
	const BYTE c_oserct_radarserDPT = 5;
	const BYTE c_oserct_radarserDLBLS = 6;
	const BYTE c_oserct_radarserCAT = 7;
	const BYTE c_oserct_radarserVAL = 8;

	const BYTE c_oserct_radarstyleVAL = 0;

	const BYTE c_oserct_radarchartRADARSTYLE = 0;
	const BYTE c_oserct_radarchartVARYCOLORS = 1;
	const BYTE c_oserct_radarchartSER = 2;
	const BYTE c_oserct_radarchartDLBLS = 3;
	const BYTE c_oserct_radarchartAXID = 4;

	const BYTE c_oserct_stockchartSER = 0;
	const BYTE c_oserct_stockchartDLBLS = 1;
	const BYTE c_oserct_stockchartDROPLINES = 2;
	const BYTE c_oserct_stockchartHILOWLINES = 3;
	const BYTE c_oserct_stockchartUPDOWNBARS = 4;
	const BYTE c_oserct_stockchartAXID = 5;

	const BYTE c_oserct_lineserIDX = 0;
	const BYTE c_oserct_lineserORDER = 1;
	const BYTE c_oserct_lineserTX = 2;
	const BYTE c_oserct_lineserSPPR = 3;
	const BYTE c_oserct_lineserMARKER = 4;
	const BYTE c_oserct_lineserDPT = 5;
	const BYTE c_oserct_lineserDLBLS = 6;
	const BYTE c_oserct_lineserTRENDLINE = 7;
	const BYTE c_oserct_lineserERRBARS = 8;
	const BYTE c_oserct_lineserCAT = 9;
	const BYTE c_oserct_lineserVAL = 10;
	const BYTE c_oserct_lineserSMOOTH = 11;

	const BYTE c_oserct_updownbarsGAPWIDTH = 0;
	const BYTE c_oserct_updownbarsUPBARS = 1;
	const BYTE c_oserct_updownbarsDOWNBARS = 2;

	const BYTE c_oserct_updownbarSPPR = 0;

	const BYTE c_oserct_line3dchartGROUPING = 0;
	const BYTE c_oserct_line3dchartVARYCOLORS = 1;
	const BYTE c_oserct_line3dchartSER = 2;
	const BYTE c_oserct_line3dchartDLBLS = 3;
	const BYTE c_oserct_line3dchartDROPLINES = 4;
	const BYTE c_oserct_line3dchartGAPDEPTH = 5;
	const BYTE c_oserct_line3dchartAXID = 6;

	const BYTE c_oserct_groupingVAL = 0;

	const BYTE c_oserct_linechartGROUPING = 0;
	const BYTE c_oserct_linechartVARYCOLORS = 1;
	const BYTE c_oserct_linechartSER = 2;
	const BYTE c_oserct_linechartDLBLS = 3;
	const BYTE c_oserct_linechartDROPLINES = 4;
	const BYTE c_oserct_linechartHILOWLINES = 5;
	const BYTE c_oserct_linechartUPDOWNBARS = 6;
	const BYTE c_oserct_linechartMARKER = 7;
	const BYTE c_oserct_linechartSMOOTH = 8;
	const BYTE c_oserct_linechartAXID = 9;

	const BYTE c_oserct_area3dchartGROUPING = 0;
	const BYTE c_oserct_area3dchartVARYCOLORS = 1;
	const BYTE c_oserct_area3dchartSER = 2;
	const BYTE c_oserct_area3dchartDLBLS = 3;
	const BYTE c_oserct_area3dchartDROPLINES = 4;
	const BYTE c_oserct_area3dchartGAPDEPTH = 5;
	const BYTE c_oserct_area3dchartAXID = 6;

	const BYTE c_oserct_areaserIDX = 0;
	const BYTE c_oserct_areaserORDER = 1;
	const BYTE c_oserct_areaserTX = 2;
	const BYTE c_oserct_areaserSPPR = 3;
	const BYTE c_oserct_areaserPICTUREOPTIONS = 4;
	const BYTE c_oserct_areaserDPT = 5;
	const BYTE c_oserct_areaserDLBLS = 6;
	const BYTE c_oserct_areaserTRENDLINE = 7;
	const BYTE c_oserct_areaserERRBARS = 8;
	const BYTE c_oserct_areaserCAT = 9;
	const BYTE c_oserct_areaserVAL = 10;

	const BYTE c_oserct_areachartGROUPING = 0;
	const BYTE c_oserct_areachartVARYCOLORS = 1;
	const BYTE c_oserct_areachartSER = 2;
	const BYTE c_oserct_areachartDLBLS = 3;
	const BYTE c_oserct_areachartDROPLINES = 4;
	const BYTE c_oserct_areachartAXID = 5;

	const BYTE c_oserct_plotareaLAYOUT = 0;
	const BYTE c_oserct_plotareaITEMS = 1;
	const BYTE c_oserct_plotareaAREA3DCHART = 2;
	const BYTE c_oserct_plotareaAREACHART = 3;
	const BYTE c_oserct_plotareaBAR3DCHART = 4;
	const BYTE c_oserct_plotareaBARCHART = 5;
	const BYTE c_oserct_plotareaBUBBLECHART = 6;
	const BYTE c_oserct_plotareaDOUGHNUTCHART = 7;
	const BYTE c_oserct_plotareaLINE3DCHART = 8;
	const BYTE c_oserct_plotareaLINECHART = 9;
	const BYTE c_oserct_plotareaOFPIECHART = 10;
	const BYTE c_oserct_plotareaPIE3DCHART = 11;
	const BYTE c_oserct_plotareaPIECHART = 12;
	const BYTE c_oserct_plotareaRADARCHART = 13;
	const BYTE c_oserct_plotareaSCATTERCHART = 14;
	const BYTE c_oserct_plotareaSTOCKCHART = 15;
	const BYTE c_oserct_plotareaSURFACE3DCHART = 16;
	const BYTE c_oserct_plotareaSURFACECHART = 17;
	const BYTE c_oserct_plotareaITEMS1 = 18;
	const BYTE c_oserct_plotareaCATAX = 19;
	const BYTE c_oserct_plotareaDATEAX = 20;
	const BYTE c_oserct_plotareaSERAX = 21;
	const BYTE c_oserct_plotareaVALAX = 22;
	const BYTE c_oserct_plotareaDTABLE = 23;
	const BYTE c_oserct_plotareaSPPR = 24;

	const BYTE c_oserct_thicknessVAL = 0;

	const BYTE c_oserct_surfaceTHICKNESS = 0;
	const BYTE c_oserct_surfaceSPPR = 1;
	const BYTE c_oserct_surfacePICTUREOPTIONS = 2;

	const BYTE c_oserct_perspectiveVAL = 0;

	const BYTE c_oserct_depthpercentVAL = 0;

	const BYTE c_oserct_rotyVAL = 0;

	const BYTE c_oserct_hpercentVAL = 0;

	const BYTE c_oserct_rotxVAL = 0;

	const BYTE c_oserct_view3dROTX = 0;
	const BYTE c_oserct_view3dHPERCENT = 1;
	const BYTE c_oserct_view3dROTY = 2;
	const BYTE c_oserct_view3dDEPTHPERCENT = 3;
	const BYTE c_oserct_view3dRANGAX = 4;
	const BYTE c_oserct_view3dPERSPECTIVE = 5;

	const BYTE c_oserct_pivotfmtIDX = 0;
	const BYTE c_oserct_pivotfmtSPPR = 1;
	const BYTE c_oserct_pivotfmtTXPR = 2;
	const BYTE c_oserct_pivotfmtMARKER = 3;
	const BYTE c_oserct_pivotfmtDLBL = 4;

	const BYTE c_oserct_pivotfmtsPIVOTFMT = 0;

	const BYTE c_oserct_chartTITLE = 0;
	const BYTE c_oserct_chartAUTOTITLEDELETED = 1;
	const BYTE c_oserct_chartPIVOTFMTS = 2;
	const BYTE c_oserct_chartVIEW3D = 3;
	const BYTE c_oserct_chartFLOOR = 4;
	const BYTE c_oserct_chartSIDEWALL = 5;
	const BYTE c_oserct_chartBACKWALL = 6;
	const BYTE c_oserct_chartPLOTAREA = 7;
	const BYTE c_oserct_chartLEGEND = 8;
	const BYTE c_oserct_chartPLOTVISONLY = 9;
	const BYTE c_oserct_chartDISPBLANKSAS = 10;
	const BYTE c_oserct_chartSHOWDLBLSOVERMAX = 11;

	const BYTE c_oserct_protectionCHARTOBJECT = 0;
	const BYTE c_oserct_protectionDATA = 1;
	const BYTE c_oserct_protectionFORMATTING = 2;
	const BYTE c_oserct_protectionSELECTION = 3;
	const BYTE c_oserct_protectionUSERINTERFACE = 4;

	const BYTE c_oserct_pivotsourceNAME = 0;
	const BYTE c_oserct_pivotsourceFMTID = 1;

	const BYTE c_oserct_style1VAL = 0;

	const BYTE c_oserct_styleVAL = 0;

	const BYTE c_oserct_textlanguageidVAL = 0;

	const BYTE c_oseralternatecontentCHOICE = 0;
	const BYTE c_oseralternatecontentFALLBACK = 1;

	const BYTE c_oseralternatecontentchoiceSTYLE = 0;
	const BYTE c_oseralternatecontentchoiceREQUIRES = 1;

	const BYTE c_oseralternatecontentfallbackSTYLE = 0;

	const BYTE c_oserct_chartstyleID = 0;
	const BYTE c_oserct_chartstyleENTRY = 1;
	const BYTE c_oserct_chartstyleMARKERLAYOUT = 2;

	const BYTE c_oserct_chartstyleENTRYTYPE = 0;
	const BYTE c_oserct_chartstyleLNREF = 1;
	const BYTE c_oserct_chartstyleFILLREF = 2;
	const BYTE c_oserct_chartstyleEFFECTREF = 3;
	const BYTE c_oserct_chartstyleFONTREF = 4;
	const BYTE c_oserct_chartstyleDEFPR = 5;
	const BYTE c_oserct_chartstyleBODYPR = 6;
	const BYTE c_oserct_chartstyleSPPR = 7;
	const BYTE c_oserct_chartstyleLINEWIDTH = 8;

	const BYTE c_oserct_chartstyleMARKERSYMBOL = 0;
	const BYTE c_oserct_chartstyleMARKERSIZE = 1;

	const BYTE c_oserct_chartcolorsID = 0;
	const BYTE c_oserct_chartcolorsMETH = 1;
	const BYTE c_oserct_chartcolorsVARIATION = 2;
	const BYTE c_oserct_chartcolorsCOLOR = 3;
	const BYTE c_oserct_chartcolorsEFFECT = 4;

	const BYTE c_oserct_chartExSpaceCHARTDATA = 0;
	const BYTE c_oserct_chartExSpaceCHART = 1;
	const BYTE c_oserct_chartExSpaceSPPR = 2;
	const BYTE c_oserct_chartExSpaceTXPR = 3;
	const BYTE c_oserct_chartExSpaceCLRMAPOVR = 4;
	const BYTE c_oserct_chartExSpaceXLSX = c_oserct_chartspaceXLSX;/* = 16*/
	const BYTE c_oserct_chartExSpaceSTYLES = c_oserct_chartspaceSTYLES;/* = 17*/
	const BYTE c_oserct_chartExSpaceCOLORS = c_oserct_chartspaceCOLORS;/* = 18*/
	const BYTE c_oserct_chartExSpaceXLSXEXTERNAL = c_oserct_chartspaceXLSXEXTERNAL;/* = 19*/
	const BYTE c_oserct_chartExSpaceXLSXZIP = c_oserct_chartspaceXLSXZIP;/* = 19*/

	const BYTE c_oserct_chartExDATA = 0;
	const BYTE c_oserct_chartExEXTERNALDATA = 1;

	const BYTE c_oserct_chartExExternalAUTOUPDATE = 0;

	const BYTE c_oserct_chartExChartPLOTAREA = 0;
	const BYTE c_oserct_chartExChartTITLE = 1;
	const BYTE c_oserct_chartExChartLEGEND = 2;

	const BYTE c_oserct_chartExChartAREAREGION = 0;
	const BYTE c_oserct_chartExChartAXIS = 1;
	const BYTE c_oserct_chartExChartSPPR = 2;

	const BYTE c_oserct_chartExAreaPLOTSURFACE = 0;
	const BYTE c_oserct_chartExAreaSERIES = 1;
	
	const BYTE c_oserct_chartExAxisID = 0;

	const BYTE c_oserct_chartExPlotSurfaceSPPR = 0;
	const BYTE c_oserct_chartExAxisHIDDEN = 1;
	const BYTE c_oserct_chartExAxisCATSCALING = 2;
	const BYTE c_oserct_chartExAxisVALSCALING = 3;
	const BYTE c_oserct_chartExAxisTITLE = 4;
	const BYTE c_oserct_chartExAxisUNIT = 5;
	const BYTE c_oserct_chartExAxisNUMFMT = 6;
	const BYTE c_oserct_chartExAxisMAJORTICK = 7;
	const BYTE c_oserct_chartExAxisMINORTICK = 8;
	const BYTE c_oserct_chartExAxisMAJORGRID = 9;
	const BYTE c_oserct_chartExAxisMINORGRID = 10;
	const BYTE c_oserct_chartExAxisTICKLABELS = 11;
	const BYTE c_oserct_chartExAxisTXPR = 12;
	const BYTE c_oserct_chartExAxisSPPR = 13;

	const BYTE c_oserct_chartExSeriesDATAPT = 0;
	const BYTE c_oserct_chartExSeriesDATALABELS = 1;
	const BYTE c_oserct_chartExSeriesLAYOUTPROPS = 2;
	const BYTE c_oserct_chartExSeriesTEXT = 3;
	const BYTE c_oserct_chartExSeriesAXIS = 4;
	const BYTE c_oserct_chartExSeriesDATAID = 5;
	const BYTE c_oserct_chartExSeriesSPPR = 6;
	const BYTE c_oserct_chartExSeriesLAYOUTID = 7;
	const BYTE c_oserct_chartExSeriesHIDDEN = 8;
	const BYTE c_oserct_chartExSeriesOWNERIDX = 9;
	const BYTE c_oserct_chartExSeriesFORMATIDX = 10;
	const BYTE c_oserct_chartExSeriesUNIQUEID = 11;

	const BYTE c_oserct_chartExDataPointIDX = 0;
	const BYTE c_oserct_chartExDataPointSPPR = 1;

	const BYTE c_oserct_chartExDataLabelsPOS = 0;
	const BYTE c_oserct_chartExDataLabelsNUMFMT = 1;
	const BYTE c_oserct_chartExDataLabelsTXPR = 2;
	const BYTE c_oserct_chartExDataLabelsSPPR = 3;
	const BYTE c_oserct_chartExDataLabelsVISABILITIES = 4;
	const BYTE c_oserct_chartExDataLabelsSEPARATOR = 5;
	const BYTE c_oserct_chartExDataLabelsDATALABEL = 6;
	const BYTE c_oserct_chartExDataLabelsDATALABELHIDDEN = 7;

	const BYTE c_oserct_chartExNumberFormatFORMATCODE = 0;
	const BYTE c_oserct_chartExNumberFormatSOURCELINKED = 1;

	const BYTE c_oserct_chartExDataLabelIDX = 0;
	const BYTE c_oserct_chartExDataLabelPOS = 1;
	const BYTE c_oserct_chartExDataLabelNUMFMT = 2;
	const BYTE c_oserct_chartExDataLabelTXPR = 3;
	const BYTE c_oserct_chartExDataLabelSPPR = 4;
	const BYTE c_oserct_chartExDataLabelVISABILITIES = 5;
	const BYTE c_oserct_chartExDataLabelSEPARATOR = 6;

	const BYTE c_oserct_chartExDataLabelHiddenIDX = 0;

	const BYTE c_oserct_chartExSeriesLayoutPARENT = 0;
	const BYTE c_oserct_chartExSeriesLayoutREGION = 1;
	const BYTE c_oserct_chartExSeriesLayoutVISABILITIES = 2;
	const BYTE c_oserct_chartExSeriesLayoutAGGREGATION = 3;
	const BYTE c_oserct_chartExSeriesLayoutBINNING = 4;
	const BYTE c_oserct_chartExSeriesLayoutSTATISTIC = 5;
	const BYTE c_oserct_chartExSeriesLayoutSUBTOTALS = 6;

	const BYTE c_oserct_chartExDataLabelVisibilitiesSERIES = 0;
	const BYTE c_oserct_chartExDataLabelVisibilitiesCATEGORY = 1;
	const BYTE c_oserct_chartExDataLabelVisibilitiesVALUE = 2;

	const BYTE c_oserct_chartExBinningBINSIZE = 0;
	const BYTE c_oserct_chartExBinningBINCOUNT = 1;
	const BYTE c_oserct_chartExBinningINTERVAL = 2;
	const BYTE c_oserct_chartExBinningUNDERVAL = 3;
	const BYTE c_oserct_chartExBinningUNDERAUTO = 4;
	const BYTE c_oserct_chartExBinningOVERVAL = 5;
	const BYTE c_oserct_chartExBinningOVERAUTO = 6;

	const BYTE c_oserct_chartExTitleTX = 0;
	const BYTE c_oserct_chartExTitleTXPR = 1;
	const BYTE c_oserct_chartExTitleSPPR = 2;
	const BYTE c_oserct_chartExTitlePOS = 3;
	const BYTE c_oserct_chartExTitleALIGN = 4;
	const BYTE c_oserct_chartExTitleOVERLAY = 5;

	const BYTE c_oserct_chartExLegendTXPR = 0;
	const BYTE c_oserct_chartExLegendSPPR = 1;
	const BYTE c_oserct_chartExLegendPOS = 2;
	const BYTE c_oserct_chartExLegendALIGN = 3;
	const BYTE c_oserct_chartExLegendOVERLAY = 4;

	const BYTE c_oserct_chartExTextRICH = 0;
	const BYTE c_oserct_chartExTextDATA = 1;

	const BYTE c_oserct_chartExTextDataFORMULA = 0;
	const BYTE c_oserct_chartExTextDataVALUE = 1;

	const BYTE c_oserct_chartExDataID = 0;
	const BYTE c_oserct_chartExDataSTRDIMENSION = 1;
	const BYTE c_oserct_chartExDataNUMDIMENSION = 2;

	const BYTE c_oserct_chartExSubtotalsIDX = 0;

	const BYTE c_oserct_chartExSeriesVisibilitiesCONNECTOR = 0;
	const BYTE c_oserct_chartExSeriesVisibilitiesMEANLINE = 1;
	const BYTE c_oserct_chartExSeriesVisibilitiesMEANMARKER = 2;
	const BYTE c_oserct_chartExSeriesVisibilitiesNONOUTLIERS = 3;
	const BYTE c_oserct_chartExSeriesVisibilitiesOUTLIERS = 4;

	const BYTE c_oserct_chartExCatScalingGAPAUTO = 0;
	const BYTE c_oserct_chartExCatScalingGAPVAL = 1;

	const BYTE c_oserct_chartExValScalingMAXAUTO = 0;
	const BYTE c_oserct_chartExValScalingMAXVAL = 1;
	const BYTE c_oserct_chartExValScalingMINAUTO = 2;
	const BYTE c_oserct_chartExValScalingMINVAL = 3;
	const BYTE c_oserct_chartExValScalingMAJUNITAUTO = 4;
	const BYTE c_oserct_chartExValScalingMAJUNITVAL = 5;
	const BYTE c_oserct_chartExValScalingMINUNITAUTO = 6;
	const BYTE c_oserct_chartExValScalingMINUNITVAL = 7;

	const BYTE c_oserct_chartExAxisUnitTYPE = 0;
	const BYTE c_oserct_chartExAxisUnitLABEL = 1;

	const BYTE c_oserct_chartExAxisUnitsLabelTEXT = 0;
	const BYTE c_oserct_chartExAxisUnitsLabelSPPR = 1;
	const BYTE c_oserct_chartExAxisUnitsLabelTXPR = 2;

	const BYTE c_oserct_chartExTickMarksTYPE = 0;

	const BYTE c_oserct_chartExGridlinesSPPR = 0;

	const BYTE c_oserct_chartExStatisticsMETHOD = 0;

	const BYTE c_oserct_chartExDataDimensionTYPE = 0;
	const BYTE c_oserct_chartExDataDimensionFORMULA = 1;
	const BYTE c_oserct_chartExDataDimensionNF = 2;
	const BYTE c_oserct_chartExDataDimensionSTRINGLEVEL = 3;
	const BYTE c_oserct_chartExDataDimensionNUMERICLEVEL = 4;

	const BYTE c_oserct_chartExFormulaCONTENT = 0;
	const BYTE c_oserct_chartExFormulaDIRECTION = 1;

	const BYTE c_oserct_chartExDataLevelNAME = 0;
	const BYTE c_oserct_chartExDataLevelCOUNT = 1;
	const BYTE c_oserct_chartExDataLevelPT = 2;
	const BYTE c_oserct_chartExDataLevelFORMATCODE = 3;

	const BYTE c_oserct_chartExDataValueIDX = 0;
	const BYTE c_oserct_chartExDataValueCONTENT = 1;

// extens ... 0x80
	const BYTE c_oserct_dataLabel = 0x81;
	const BYTE c_oserct_chartFiltering = 0x82;
	const BYTE c_oserct_chartDataDisplayNaAsBlank = 0x83;
	const BYTE c_oserct_chartExternalReference = 0x84;
	const BYTE c_oserct_chartDataExternalFileKey = 0x85;
	const BYTE c_oserct_chartDataExternalInstanceId = 0x86;

	const BYTE c_oserct_dataLabelsRange = 0x90;
	const BYTE c_oserct_filteredLineSeries = 0x91;
	const BYTE c_oserct_filteredScatterSeries = 0x92;
	const BYTE c_oserct_filteredRadarSeries = 0x93;
	const BYTE c_oserct_filteredBarSeries = 0x94;
	const BYTE c_oserct_filteredAreaSeries = 0x95;
	const BYTE c_oserct_filteredBubbleSeries = 0x96;
	const BYTE c_oserct_filteredSurfaceSeries = 0x97;
	const BYTE c_oserct_filteredPieSeries = 0x98;
	const BYTE c_oserct_fullRef = 0x99;
	const BYTE c_oserct_levelRef = 0x9A;
	const BYTE c_oserct_formulaRef = 0x9B;
	const BYTE c_oserct_categoryFilterExceptions = 0x9C;
	const BYTE c_oserct_categoryFilterException = 0x9D;
	const BYTE c_oserct_filteredSeriesTitle = 0x9E;
	const BYTE c_oserct_filteredCategoryTitle = 0x9F;

	const BYTE c_oserct_dataLabelsRangeFormula = 0xA0;
	const BYTE c_oserct_dataLabelsRangeCache = 0xA1;

	const BYTE c_oserct_filterSqref = 0xB1;
	const BYTE c_oserct_filterSpPr = 0xB2;
	const BYTE c_oserct_filterExplosion = 0xB3;
	const BYTE c_oserct_filterInvertIfNegative = 0xB4;
	const BYTE c_oserct_filterBubble3D = 0xB5;
	const BYTE c_oserct_filterMarker = 0xB6;
	const BYTE c_oserct_filterLbl = 0xB7;

	BinaryChartReader::BinaryChartReader(NSBinPptxRW::CBinaryFileReader& oBufferedStream, SaveParams& oSaveParams, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter)
		: Binary_CommonReader(oBufferedStream), m_oSaveParams(oSaveParams), m_pOfficeDrawingConverter(pOfficeDrawingConverter)
	{}

	int BinaryChartReader::ReadCT_ChartFile(long length, OOX::Spreadsheet::CChartFile* pChartFile)
	{
		int res = c_oSerConstants::ReadOk;
		READ1_DEF(length, res, this->ReadCT_ChartFileContent, pChartFile);
		return res;
	}
	int BinaryChartReader::ReadCT_ChartFileContent(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		
		OOX::Spreadsheet::CChartFile *pChart = static_cast<OOX::Spreadsheet::CChartFile*>(poResult);
		
		if (type > 0x80)
		{
			if (!pChart->m_oChartSpace.m_extLst.IsInit()) pChart->m_oChartSpace.m_extLst.Init();
			return ReadExtensions(type, length, pChart->m_oChartSpace.m_extLst.GetPointer());
		}
		if (c_oserct_chartspaceDATE1904 == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			pChart->m_oChartSpace.m_date1904 = pNewElem;
		}
		else if (c_oserct_chartspaceLANG == type)
		{
			std::wstring val;
			READ1_DEF(length, res, this->ReadCT_String, &val);
			pChart->m_oChartSpace.m_lang = val;
		}
		else if (c_oserct_chartspaceROUNDEDCORNERS == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			pChart->m_oChartSpace.m_roundedCorners = pNewElem;
		}
		else if (c_oserct_chartspaceALTERNATECONTENT == type)
		{
			AlternateContent* pNewElem = new AlternateContent;
			READ1_DEF(length, res, this->ReadAlternateContent, pNewElem);
			pChart->m_oChartSpace.m_AlternateContent = pNewElem;
		}
		else if (c_oserct_chartspaceSTYLE == type)
		{
			pChart->m_oChartSpace.m_style = new CT_Style;
			READ1_DEF(length, res, this->ReadCT_Style, pChart->m_oChartSpace.m_style);
		}
		else if (c_oserct_chartspaceCLRMAPOVR == type)
		{
			BYTE typeRec1 = m_oBufferedStream.GetUChar();
			
			pChart->m_oChartSpace.m_oClrMapOvr = new PPTX::Logic::ClrMap();

			pChart->m_oChartSpace.m_oClrMapOvr->m_name = L"c:clrMapOvr";
			pChart->m_oChartSpace.m_oClrMapOvr->fromPPTY(&m_oBufferedStream);
		}
		else if (c_oserct_chartspacePIVOTSOURCE == type)
		{
			CT_PivotSource* pNewElem = new CT_PivotSource;
			READ1_DEF(length, res, this->ReadCT_PivotSource, pNewElem);
			pChart->m_oChartSpace.m_pivotSource = pNewElem;
		}
		else if (c_oserct_chartspacePROTECTION == type)
		{
			CT_Protection* pNewElem = new CT_Protection;
			READ1_DEF(length, res, this->ReadCT_Protection, pNewElem);
			pChart->m_oChartSpace.m_protection = pNewElem;
		}
		else if (c_oserct_chartspaceCHART == type)
		{
			pChart->m_oChartSpace.m_chart = new CT_Chart;
			READ1_DEF(length, res, this->ReadCT_Chart, pChart->m_oChartSpace.m_chart);
		}
		else if (c_oserct_chartspaceSPPR == type)
		{
			pChart->m_oChartSpace.m_spPr = new PPTX::Logic::SpPr;
			pChart->m_oChartSpace.m_spPr->m_namespace = L"c";
			res = ReadCT_PptxElement(0, length, pChart->m_oChartSpace.m_spPr.GetPointer());
		}
		else if (c_oserct_chartspaceTXPR == type)
		{
			pChart->m_oChartSpace.m_txPr = new PPTX::Logic::TxBody;
			pChart->m_oChartSpace.m_txPr->m_name = L"c:txPr";
			res = ReadCT_PptxElement(0, length, pChart->m_oChartSpace.m_txPr.GetPointer());
		}
		else if (c_oserct_chartspacePRINTSETTINGS == type)
		{
			pChart->m_oChartSpace.m_printSettings = new CT_PrintSettings;
			READ1_DEF(length, res, this->ReadCT_PrintSettings, pChart->m_oChartSpace.m_printSettings);
		}
		else if (c_oserct_chartspaceTHEMEOVERRIDE == type)
		{
            std::wstring sThemeOverrideName      = L"themeOverride" + std::to_wstring(m_oBufferedStream.m_nThemeOverrideCount++) + L".xml";
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
			if (-1 != nIndex)
			{
                std::wstring sContentTypesPath = m_oSaveParams.sThemePath.substr(nIndex + 1);
                XmlUtils::replace_all(sContentTypesPath, L"\\", L"/");

				m_oSaveParams.pContentTypes->Registration(L"application/vnd.openxmlformats-officedocument.themeOverride+xml", sContentTypesPath, sThemeOverrideName);
			}
		}
		else if (c_oserct_chartspaceUSERSHAPES == type)
		{			
			OOX::CSystemUtility::CreateDirectories(m_oSaveParams.sDrawingsPath);

			OOX::CPath pathDrawingsRelsDir = m_oSaveParams.sDrawingsPath  + FILE_SEPARATOR_STR + _T("_rels");
			OOX::CSystemUtility::CreateDirectories(pathDrawingsRelsDir.GetPath());

			m_pOfficeDrawingConverter->SetDstContentRels();
			
			OOX::CChartDrawing* pChartDrawing = new OOX::CChartDrawing(NULL);
			READ1_DEF(length, res, this->ReadCT_userShapes, pChartDrawing);

			if (res == c_oSerConstants::ReadOk)
			{
				NSCommon::smart_ptr<OOX::File> pDrawingFile(pChartDrawing);
				pChart->Add(pDrawingFile);

				OOX::CPath pathDrawingsRels = pathDrawingsRelsDir.GetPath() + FILE_SEPARATOR_STR + pChartDrawing->m_sOutputFilename + _T(".rels");
				m_pOfficeDrawingConverter->SaveDstContentRels(pathDrawingsRels.GetPath());

				unsigned int rId = 0;
				m_pOfficeDrawingConverter->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/chartUserShapes", L"../drawings/" + pChartDrawing->m_sOutputFilename, std::wstring(), &rId);

				pChart->m_oChartSpace.m_userShapes = new CT_RelId;
				pChart->m_oChartSpace.m_userShapes->m_id = new std::wstring;
				*pChart->m_oChartSpace.m_userShapes->m_id = OOX::RId(rId).ToString();
			}
		}
		else if (c_oserct_chartspaceEXTERNALDATA == type)
		{
			if (!pChart->m_oChartSpace.m_externalData)
				pChart->m_oChartSpace.m_externalData = new CT_ExternalData;
			
			READ1_DEF(length, res, this->ReadCT_ExternalData, pChart->m_oChartSpace.m_externalData);
		}
		else if (c_oserct_chartspaceXLSXEXTERNAL == type)
		{
			OOX::CPath path = m_oBufferedStream.GetString4(length);

			unsigned int rId = 0;
			m_pOfficeDrawingConverter->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/oleObject", path.GetPath(), L"External", &rId);
			m_pOfficeDrawingConverter->m_pImageManager->m_pContentTypes->AddDefault(path.GetExtention());

			if (!pChart->m_oChartSpace.m_externalData)
				pChart->m_oChartSpace.m_externalData = new CT_ExternalData;

			pChart->m_oChartSpace.m_externalData->m_id = new std::wstring;
			*pChart->m_oChartSpace.m_externalData->m_id = OOX::RId(rId).ToString();
		}
		else if (c_oserct_chartspaceXLSX	== type || 
				 c_oserct_chartspaceXLSXZIP == type)
		{			
			OOX::CSystemUtility::CreateDirectories(m_oSaveParams.sEmbeddingsPath);

			OOX::CPath pathEmbeddingRelsDir = m_oSaveParams.sEmbeddingsPath + FILE_SEPARATOR_STR + _T("_rels");
			OOX::CSystemUtility::CreateDirectories(pathEmbeddingRelsDir.GetPath());

			m_pOfficeDrawingConverter->SetDstContentRels();

			NSCommon::smart_ptr<OOX::Media> pXlsxFile;			
			
			if (c_oserct_chartspaceXLSXZIP == type)
			{
				ReadCT_XlsxZip(m_oBufferedStream.GetPointer(0), length, pXlsxFile);
			}
			else
			{
				ReadCT_XlsxBin(m_oBufferedStream.GetPointer(0), length, pXlsxFile);
			}
			m_oBufferedStream.Skip(length);

			if (pXlsxFile.IsInit())
			{
				OOX::CPath pathDrawingsRels = pathEmbeddingRelsDir.GetPath() + FILE_SEPARATOR_STR + pXlsxFile->m_sOutputFilename + _T(".rels");
				m_pOfficeDrawingConverter->SaveDstContentRels(pathDrawingsRels.GetPath());
								
				NSCommon::smart_ptr<OOX::File> pFile = pXlsxFile.smart_dynamic_cast<OOX::File>();
				pChart->AddNoWrite(pFile, L"../embeddings");

				unsigned int rId = 0;
				m_pOfficeDrawingConverter->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/package", L"../embeddings/" + pXlsxFile->m_sOutputFilename, std::wstring(), &rId);
				m_pOfficeDrawingConverter->m_pImageManager->m_pContentTypes->AddDefault(pXlsxFile->filename().GetExtention());

				if (!pChart->m_oChartSpace.m_externalData)
					pChart->m_oChartSpace.m_externalData = new CT_ExternalData;

				pChart->m_oChartSpace.m_externalData->m_id = new std::wstring;
				*pChart->m_oChartSpace.m_externalData->m_id = OOX::RId(rId).ToString();
			}
		}
		else if (c_oserct_chartspaceSTYLES == type)
		{
			NSCommon::smart_ptr<OOX::Spreadsheet::CChartStyleFile> chartstyle = new OOX::Spreadsheet::CChartStyleFile(NULL);
			READ1_DEF(length, res, ReadCT_ChartStyle, chartstyle.GetPointer());

			if (res == c_oSerConstants::ReadOk)
			{
				NSCommon::smart_ptr<OOX::File> pFile = chartstyle.smart_dynamic_cast<OOX::File>();
				pChart->Add(pFile);

				unsigned int rId = 0;
				m_pOfficeDrawingConverter->WriteRels(L"http://schemas.microsoft.com/office/2011/relationships/chartStyle", chartstyle->m_sOutputFilename, std::wstring(), &rId);
			}
		}
		else if (c_oserct_chartspaceCOLORS == type)
		{
			NSCommon::smart_ptr<OOX::Spreadsheet::CChartColorsFile> chartcolors = new OOX::Spreadsheet::CChartColorsFile(NULL);
			READ1_DEF(length, res, ReadCT_ChartColors, chartcolors.GetPointer());

			if (res == c_oSerConstants::ReadOk)
			{
				NSCommon::smart_ptr<OOX::File> pFile = chartcolors.smart_dynamic_cast<OOX::File>();
				pChart->Add(pFile);

				unsigned int rId = 0;
				m_pOfficeDrawingConverter->WriteRels(L"http://schemas.microsoft.com/office/2011/relationships/chartColorStyle", chartcolors->m_sOutputFilename, std::wstring(), &rId);
			}
		}

		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_XlsxZip(BYTE* pData, long length, NSCommon::smart_ptr<OOX::Media>& fileOut)
	{
		if (length < 1 || !pData)
			return c_oSerConstants::ReadUnknown;
		
		fileOut = new OOX::OleObject(NULL, true, m_pOfficeDrawingConverter->m_pBinaryReader->m_nDocumentType == XMLWRITER_DOC_TYPE_DOCX);
		
		int id = m_pOfficeDrawingConverter->m_pBinaryReader->m_nCountEmbedded++;

		bool bMacroEnabled = false;
		std::wstring sXlsxFilename = L"Microsoft_Excel_Worksheet" + std::to_wstring(id) + (bMacroEnabled ? L".xlsm" : L".xlsx");

		NSFile::CFileBinary file;
		if (file.CreateFileW(m_oSaveParams.sEmbeddingsPath + FILE_SEPARATOR_STR + sXlsxFilename))
		{
			file.WriteFile(pData, length);
			file.CloseFile();
		}
		fileOut->set_filename(m_oSaveParams.sEmbeddingsPath + FILE_SEPARATOR_STR + sXlsxFilename, false);

		m_pOfficeDrawingConverter->m_pBinaryReader->m_pRels->m_pManager->m_pContentTypes->AddDefault(bMacroEnabled ? L"xlsm" : L"xlsx");
	}
	int BinaryChartReader::ReadCT_XlsxBin(BYTE *pData, long length, NSCommon::smart_ptr<OOX::Media> & file)
	{
		if (length < 1 || !pData)
			return c_oSerConstants::ReadUnknown;
		//------------------------------------------------------------------
		std::wstring sDstEmbedded = m_oSaveParams.sEmbeddingsPath;

		std::wstring sDstEmbeddedTemp = NSDirectory::CreateDirectoryWithUniqueName(sDstEmbedded);
		
		if (false == sDstEmbeddedTemp.empty())
		{
			file = new OOX::OleObject(NULL, true, m_pOfficeDrawingConverter->m_pBinaryReader->m_nDocumentType == XMLWRITER_DOC_TYPE_DOCX);

			int id = m_pOfficeDrawingConverter->m_pBinaryReader->m_nCountEmbedded++;

			OOX::Spreadsheet::CXlsx			oXlsx;
			BinXlsxRW::BinaryFileReader		oEmbeddedReader;
			NSBinPptxRW::CDrawingConverter	oDrawingConverter;

			std::wstring sDrawingsPath = sDstEmbeddedTemp + FILE_SEPARATOR_STR + L"xl" + FILE_SEPARATOR_STR + L"drawings";
			std::wstring sThemePath = sDstEmbeddedTemp + FILE_SEPARATOR_STR + L"xl" + FILE_SEPARATOR_STR + L"theme";
			std::wstring sEmbeddingsPath = sDstEmbeddedTemp + FILE_SEPARATOR_STR + L"xl" + FILE_SEPARATOR_STR + L"embeddings";

			BinXlsxRW::SaveParams oSaveParams(sDrawingsPath, sEmbeddingsPath, sThemePath, oDrawingConverter.GetContentTypes(), NULL, false);

			std::wstring sXmlOptions, sMediaPath, sEmbedPath;
			BinXlsxRW::CXlsxSerializer::CreateXlsxFolders(sXmlOptions, sDstEmbeddedTemp, sMediaPath, sEmbedPath);

			boost::unordered_map<std::wstring, size_t>	old_enum_map = oXlsx.m_mapEnumeratedGlobal;

			oXlsx.m_mapEnumeratedGlobal.clear();

			oDrawingConverter.m_pBinaryReader->Init(pData, 0, length);

			oDrawingConverter.SetDstPath(sDstEmbeddedTemp + FILE_SEPARATOR_STR + L"xl");
			oDrawingConverter.SetSrcPath(m_pOfficeDrawingConverter->m_pBinaryReader->m_strFolder, XMLWRITER_DOC_TYPE_XLSX);

			oDrawingConverter.SetMediaDstPath(sMediaPath);
			oDrawingConverter.SetEmbedDstPath(sEmbedPath);

			oEmbeddedReader.ReadMainTable(oXlsx, *oDrawingConverter.m_pBinaryReader, m_pOfficeDrawingConverter->m_pBinaryReader->m_strFolder, sDstEmbeddedTemp, oSaveParams, &oDrawingConverter);

			oXlsx.PrepareToWrite();

			oXlsx.Write(sDstEmbeddedTemp, *oSaveParams.pContentTypes);

			std::wstring sXlsxFilename = L"Microsoft_Excel_Worksheet" + std::to_wstring(id) + (oSaveParams.bMacroEnabled ? L".xlsm" : L".xlsx");
			COfficeUtils oOfficeUtils(NULL);
			oOfficeUtils.CompressFileOrDirectory(sDstEmbeddedTemp, sDstEmbedded + FILE_SEPARATOR_STR + sXlsxFilename, true);

			oXlsx.m_mapEnumeratedGlobal = old_enum_map;

			file->set_filename(sDstEmbedded + FILE_SEPARATOR_STR + sXlsxFilename, false);

			m_pOfficeDrawingConverter->m_pBinaryReader->m_pRels->m_pManager->m_pContentTypes->AddDefault(oSaveParams.bMacroEnabled ? L"xlsm" : L"xlsx");
		
			NSDirectory::DeleteDirectory(sDstEmbeddedTemp);
		}

		return c_oSerConstants::ReadOk;
	}
	int BinaryChartReader::ReadCT_SignedByte(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		int* poVal = static_cast<int*>(poResult);

		*poVal = m_oBufferedStream.GetChar();
		return res;
	}
	int BinaryChartReader::ReadCT_UnsignedByte(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		unsigned int* poVal = static_cast<unsigned int*>(poResult);

		*poVal = m_oBufferedStream.GetUChar();
		return res;
	}
	int BinaryChartReader::ReadCT_UnsignedInt(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		unsigned int* poVal = static_cast<unsigned int*>(poResult);
		
		*poVal = m_oBufferedStream.GetLong();
		return res;
	}
	int BinaryChartReader::ReadCT_String(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		std::wstring* poVal = static_cast<std::wstring*>(poResult);

		*poVal = m_oBufferedStream.GetString4(length);
		return res;
	}
	int BinaryChartReader::ReadCT_Double(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		double* poVal = static_cast<double*>(poResult);

		*poVal = m_oBufferedStream.GetDoubleReal();
		return res;
	}
	int BinaryChartReader::ReadCT_Boolean(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		bool* poVal = static_cast<bool*>(poResult);
		
		*poVal = m_oBufferedStream.GetBool();
		return res;
	}
	int BinaryChartReader::ReadCT_PptxElement(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		PPTX::WrapperWritingElement* poVal = static_cast<PPTX::WrapperWritingElement*>(poResult);
		if (length > 0)
		{
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
		if (c_oserct_usershapes_COUNT == type)
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

		if (!pAnchor && (!pRelAnchor || pAbsAnchor)) return c_oSerConstants::ReadUnknown;

		int res = c_oSerConstants::ReadOk;
		if (c_oSer_DrawingType::From == type)
		{
			pAnchor->m_oFrom.Init();
			READ2_DEF_SPREADSHEET(length, res, this->ReadCT_FromTo, pAnchor->m_oFrom.GetPointer());
		}
		else if (c_oSer_DrawingType::To == type)
		{
			pRelAnchor->m_oTo.Init();
			READ2_DEF_SPREADSHEET(length, res, this->ReadCT_FromTo, pRelAnchor->m_oTo.GetPointer());
		}
		else if (c_oSer_DrawingType::Ext == type)
		{
			pAbsAnchor->m_oExt.Init();
			READ2_DEF_SPREADSHEET(length, res, this->ReadCT_Ext, pAbsAnchor->m_oExt.GetPointer());
		}
		else if (c_oSer_DrawingType::pptxDrawing == type)
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
		if (c_oSer_DrawingPosType::X == type)
		{
			pFromTo->m_oX.reset(new double);
			*pFromTo->m_oX = m_oBufferedStream.GetDoubleReal();
		}
		if (c_oSer_DrawingPosType::Y == type)
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
		if (c_oSer_DrawingPosType::X == type)
		{
			pExt->m_oCx.reset(new double);
			*pExt->m_oCx = m_oBufferedStream.GetDoubleReal();
		}
		if (c_oSer_DrawingPosType::Y == type)
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
		if (c_oserct_pagesetupPAPERSIZE == type)
		{
			poVal->m_paperSize = m_oBufferedStream.GetLong();
		}
		else if (c_oserct_pagesetupPAPERHEIGHT == type)
		{
			poVal->m_paperHeight = m_oBufferedStream.GetString4(length);
		}
		else if (c_oserct_pagesetupPAPERWIDTH == type)
		{
			poVal->m_paperWidth = m_oBufferedStream.GetString4(length);
		}
		else if (c_oserct_pagesetupFIRSTPAGENUMBER == type)
		{
			poVal->m_firstPageNumber = (unsigned int)m_oBufferedStream.GetLong();
		}
		else if (c_oserct_pagesetupORIENTATION == type)
		{
			poVal->m_orientation.Init();
			poVal->m_orientation->SetValueFromByte(m_oBufferedStream.GetUChar()); 
		}
		else if (c_oserct_pagesetupBLACKANDWHITE == type)
		{
			poVal->m_blackAndWhite = m_oBufferedStream.GetBool();
		}
		else if (c_oserct_pagesetupDRAFT == type)
		{
			poVal->m_draft = m_oBufferedStream.GetBool();
		}
		else if (c_oserct_pagesetupUSEFIRSTPAGENUMBER == type)
		{
			poVal->m_useFirstPageNumber = m_oBufferedStream.GetBool();
		}
		else if (c_oserct_pagesetupHORIZONTALDPI == type)
		{
			poVal->m_horizontalDpi = m_oBufferedStream.GetLong();
		}
		else if (c_oserct_pagesetupVERTICALDPI == type)
		{
			poVal->m_verticalDpi = m_oBufferedStream.GetLong();
		}
		else if (c_oserct_pagesetupCOPIES == type)
		{
			poVal->m_copies = m_oBufferedStream.GetLong();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_PageMargins(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_PageMargins* poVal = static_cast<CT_PageMargins*>(poResult);
		if (c_oserct_pagemarginsL == type)
		{
			poVal->m_l = m_oBufferedStream.GetDoubleReal();
		}
		else if (c_oserct_pagemarginsR == type)
		{
			poVal->m_r = m_oBufferedStream.GetDoubleReal();
		}
		else if (c_oserct_pagemarginsT == type)
		{
			poVal->m_t = m_oBufferedStream.GetDoubleReal();
		}
		else if (c_oserct_pagemarginsB == type)
		{
			poVal->m_b = m_oBufferedStream.GetDoubleReal();
		}
		else if (c_oserct_pagemarginsHEADER == type)
		{
			poVal->m_header = m_oBufferedStream.GetDoubleReal();
		}
		else if (c_oserct_pagemarginsFOOTER == type)
		{

			poVal->m_footer = m_oBufferedStream.GetDoubleReal();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_HeaderFooter(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_HeaderFooter* poVal = static_cast<CT_HeaderFooter*>(poResult);
		if (c_oserct_headerfooterODDHEADER == type)
		{
			poVal->m_oddHeader = m_oBufferedStream.GetString4(length);;
		}
		else if (c_oserct_headerfooterODDFOOTER == type)
		{
			poVal->m_oddFooter = m_oBufferedStream.GetString4(length);;
		}
		else if (c_oserct_headerfooterEVENHEADER == type)
		{
			poVal->m_evenHeader = m_oBufferedStream.GetString4(length);;
		}
		else if (c_oserct_headerfooterEVENFOOTER == type)
		{
			poVal->m_evenFooter = m_oBufferedStream.GetString4(length);
		}
		else if (c_oserct_headerfooterFIRSTHEADER == type)
		{
			poVal->m_firstHeader = m_oBufferedStream.GetString4(length);
		}
		else if (c_oserct_headerfooterFIRSTFOOTER == type)
		{
			poVal->m_firstFooter = m_oBufferedStream.GetString4(length);
		}
		else if (c_oserct_headerfooterALIGNWITHMARGINS == type)
		{
			poVal->m_alignWithMargins = m_oBufferedStream.GetBool();;
		}
		else if (c_oserct_headerfooterDIFFERENTODDEVEN == type)
		{
			poVal->m_differentOddEven = m_oBufferedStream.GetBool();;
		}
		else if (c_oserct_headerfooterDIFFERENTFIRST == type)
		{
			poVal->m_differentFirst = m_oBufferedStream.GetBool();;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_PrintSettings(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_PrintSettings* poVal = static_cast<CT_PrintSettings*>(poResult);
		if (c_oserct_printsettingsHEADERFOOTER == type)
		{
			CT_HeaderFooter* pNewElem = new CT_HeaderFooter;
			READ1_DEF(length, res, this->ReadCT_HeaderFooter, pNewElem);
			poVal->m_headerFooter = pNewElem;
		}
		else if (c_oserct_printsettingsPAGEMARGINS == type)
		{
			CT_PageMargins* pNewElem = new CT_PageMargins;
			READ1_DEF(length, res, this->ReadCT_PageMargins, pNewElem);
			poVal->m_pageMargins = pNewElem;
		}
		else if (c_oserct_printsettingsPAGESETUP == type)
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
		if (c_oserct_externaldataAUTOUPDATE == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_autoUpdate = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_DispBlanksAs(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CDispBlanksAs* poVal = static_cast<CDispBlanksAs*>(poResult);
		if (c_oserct_dispblanksasVAL == type)
		{
			poVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_LegendEntry(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_LegendEntry* poVal = static_cast<CT_LegendEntry*>(poResult);
		if (c_oserct_legendentryIDX == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_idx = pNewElem;
		}
		else if (c_oserct_legendentryDELETE == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_delete = pNewElem;
		}
		else if (c_oserct_legendentryTXPR == type)
		{
			poVal->m_txPr = new PPTX::Logic::TxBody;
			poVal->m_txPr->m_name = L"c:txPr";
			res = ReadCT_PptxElement(0, length, poVal->m_txPr.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}

	int BinaryChartReader::ReadCT_LegendPos(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CLegendPos* poVal = static_cast<CLegendPos*>(poResult);
		if (c_oserct_legendposVAL == type)
		{
			poVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Legend(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Legend* poVal = static_cast<CT_Legend*>(poResult);
		if (c_oserct_legendLEGENDPOS == type)
		{
			poVal->m_legendPos.Init(); 
			READ1_DEF(length, res, this->ReadCT_LegendPos, poVal->m_legendPos.GetPointer());
		}
		else if (c_oserct_legendLEGENDENTRY == type)
		{
			CT_LegendEntry* pNewElem = new CT_LegendEntry;
			READ1_DEF(length, res, this->ReadCT_LegendEntry, pNewElem);
			poVal->m_legendEntry.push_back(pNewElem);
		}
		else if (c_oserct_legendLAYOUT == type)
		{
			CT_Layout* pNewElem = new CT_Layout;
			READ1_DEF(length, res, this->ReadCT_Layout, pNewElem);
			poVal->m_layout = pNewElem;
		}
		else if (c_oserct_legendOVERLAY == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_overlay = pNewElem;
		}
		else if (c_oserct_legendSPPR == type)
		{
			poVal->m_spPr = new PPTX::Logic::SpPr;
			poVal->m_spPr->m_namespace = L"c";
			ReadCT_PptxElement(0, length, poVal->m_spPr.GetPointer());
		}
		else if (c_oserct_legendTXPR == type)
		{
			poVal->m_txPr = new PPTX::Logic::TxBody;
			poVal->m_txPr->m_name = L"c:txPr";
			res = ReadCT_PptxElement(0, length, poVal->m_txPr.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Layout(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Layout* poVal = static_cast<CT_Layout*>(poResult);
		if (c_oserct_layoutMANUALLAYOUT == type)
		{
			CT_ManualLayout* pNewElem = new CT_ManualLayout;
			READ1_DEF(length, res, this->ReadCT_ManualLayout, pNewElem);
			poVal->m_manualLayout = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ManualLayout(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_ManualLayout* poVal = static_cast<CT_ManualLayout*>(poResult);
		if (c_oserct_manuallayoutLAYOUTTARGET == type)
		{
			poVal->m_layoutTarget.Init();
			READ1_DEF(length, res, this->ReadCT_LayoutTarget, poVal->m_layoutTarget.GetPointer());
		}
		else if (c_oserct_manuallayoutXMODE == type)
		{
			poVal->m_xMode.Init();
			READ1_DEF(length, res, this->ReadCT_LayoutMode, poVal->m_xMode.GetPointer());
		}
		else if (c_oserct_manuallayoutYMODE == type)
		{
			poVal->m_yMode.Init();
			READ1_DEF(length, res, this->ReadCT_LayoutMode, poVal->m_yMode.GetPointer());
		}
		else if (c_oserct_manuallayoutWMODE == type)
		{
			poVal->m_wMode.Init();
			READ1_DEF(length, res, this->ReadCT_LayoutMode, poVal->m_wMode.GetPointer());
		}
		else if (c_oserct_manuallayoutHMODE == type)
		{
			poVal->m_hMode.Init();
			READ1_DEF(length, res, this->ReadCT_LayoutMode, poVal->m_hMode.GetPointer());
		}
		else if (c_oserct_manuallayoutX == type)
		{
			double pNewElem;
			READ1_DEF(length, res, this->ReadCT_Double, &pNewElem);
			poVal->m_x = pNewElem;
		}
		else if (c_oserct_manuallayoutY == type)
		{
			double pNewElem;
			READ1_DEF(length, res, this->ReadCT_Double, &pNewElem);
			poVal->m_y = pNewElem;
		}
		else if (c_oserct_manuallayoutW == type)
		{
			double pNewElem;
			READ1_DEF(length, res, this->ReadCT_Double, &pNewElem);
			poVal->m_w = pNewElem;
		}
		else if (c_oserct_manuallayoutH == type)
		{
			double pNewElem;
			READ1_DEF(length, res, this->ReadCT_Double, &pNewElem);
			poVal->m_h = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_LayoutTarget(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CLayoutTarget* poVal = static_cast<CLayoutTarget*>(poResult);
		if (c_oserct_layouttargetVAL == type)
		{
			poVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_LayoutMode(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CLayoutMode* poVal = static_cast<CLayoutMode*>(poResult);
		if (c_oserct_layoutmodeVAL == type)
		{
			poVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}

	int BinaryChartReader::ReadCT_DTable(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_DTable* poVal = static_cast<CT_DTable*>(poResult);
		if (c_oserct_dtableSHOWHORZBORDER == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_showHorzBorder = pNewElem;
		}
		else if (c_oserct_dtableSHOWVERTBORDER == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_showVertBorder = pNewElem;
		}
		else if (c_oserct_dtableSHOWOUTLINE == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_showOutline = pNewElem;
		}
		else if (c_oserct_dtableSHOWKEYS == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_showKeys = pNewElem;
		}
		else if (c_oserct_dtableSPPR == type)
		{
			poVal->m_spPr = new PPTX::Logic::SpPr;
			poVal->m_spPr->m_namespace = L"c";
			ReadCT_PptxElement(0, length, poVal->m_spPr.GetPointer());
		}
		else if (c_oserct_dtableTXPR == type)
		{
			poVal->m_txPr = new PPTX::Logic::TxBody;
			poVal->m_txPr->m_name = L"c:txPr";
			res = ReadCT_PptxElement(0, length, poVal->m_txPr.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_SerAx(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_SerAx* poVal = static_cast<CT_SerAx*>(poResult);
		if (c_oserct_seraxAXID == type)
		{
			unsigned int val;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &val);
			poVal->m_axId = val;
		}
		else if (c_oserct_seraxSCALING == type)
		{
			CT_Scaling* pNewElem = new CT_Scaling;
			READ1_DEF(length, res, this->ReadCT_Scaling, pNewElem);
			poVal->m_scaling = pNewElem;
		}
		else if (c_oserct_seraxDELETE == type)
		{
			bool val;
			READ1_DEF(length, res, this->ReadCT_Boolean, &val);
			poVal->m_delete = val;
		}
		else if (c_oserct_seraxAXPOS == type)
		{
			poVal->m_axPos.Init();
			READ1_DEF(length, res, this->ReadCT_AxPos, poVal->m_axPos.GetPointer());
		}
		else if (c_oserct_seraxMAJORGRIDLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_majorGridlines = pNewElem;
		}
		else if (c_oserct_seraxMINORGRIDLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_minorGridlines = pNewElem;
		}
		else if (c_oserct_seraxTITLE == type)
		{
			CT_Title* pNewElem = new CT_Title;
			READ1_DEF(length, res, this->ReadCT_Title, pNewElem);
			poVal->m_title = pNewElem;
		}
		else if (c_oserct_seraxNUMFMT == type)
		{
			CT_NumFmt* pNewElem = new CT_NumFmt;
			READ1_DEF(length, res, this->ReadCT_NumFmt, pNewElem);
			poVal->m_numFmt = pNewElem;
		}
		else if (c_oserct_seraxMAJORTICKMARK == type)
		{
			poVal->m_majorTickMark.Init();
			READ1_DEF(length, res, this->ReadCT_TickMark, poVal->m_majorTickMark.GetPointer());
		}
		else if (c_oserct_seraxMINORTICKMARK == type)
		{
			poVal->m_minorTickMark.Init();
			READ1_DEF(length, res, this->ReadCT_TickMark, poVal->m_minorTickMark.GetPointer());
		}
		else if (c_oserct_seraxTICKLBLPOS == type)
		{
			poVal->m_tickLblPos.Init();
			READ1_DEF(length, res, this->ReadCT_TickLblPos, poVal->m_tickLblPos.GetPointer());
		}
		else if (c_oserct_seraxSPPR == type)
		{
			poVal->m_spPr = new PPTX::Logic::SpPr;
			poVal->m_spPr->m_namespace = L"c";
			ReadCT_PptxElement(0, length, poVal->m_spPr.GetPointer());
		}
		else if (c_oserct_seraxTXPR == type)
		{
			poVal->m_txPr = new PPTX::Logic::TxBody;
			poVal->m_txPr->m_name = L"c:txPr";
			res = ReadCT_PptxElement(0, length, poVal->m_txPr.GetPointer());
		}
		else if (c_oserct_seraxCROSSAX == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_crossAx = pNewElem;
		}
		else if (c_oserct_seraxCROSSES == type)
		{
			poVal->m_crosses.Init();
			READ1_DEF(length, res, this->ReadCT_Crosses, poVal->m_crosses.GetPointer());
		}
		else if (c_oserct_seraxCROSSESAT == type)
		{
			double pNewElem;
			READ1_DEF(length, res, this->ReadCT_Double, &pNewElem);
			poVal->m_crossesAt = pNewElem;
		}
		else if (c_oserct_seraxTICKLBLSKIP == type)
		{
			unsigned int val;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &val);
			poVal->m_tickLblSkip = val;
		}
		else if (c_oserct_seraxTICKMARKSKIP == type)
		{
			unsigned int val;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &val);
			poVal->m_tickMarkSkip = val;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Scaling(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Scaling* poVal = static_cast<CT_Scaling*>(poResult);
		if (c_oserct_scalingLOGBASE == type)
		{
			double val;
			READ1_DEF(length, res, this->ReadCT_Double, &val);
			poVal->m_logBase = val;
		}
		else if (c_oserct_scalingORIENTATION == type)
		{
			poVal->m_orientation.Init();
			READ1_DEF(length, res, this->ReadCT_Orientation, poVal->m_orientation.GetPointer());
		}
		else if (c_oserct_scalingMAX == type)
		{
			double val;
			READ1_DEF(length, res, this->ReadCT_Double, &val);
			poVal->m_max = val;
		}
		else if (c_oserct_scalingMIN == type)
		{
			double val;
			READ1_DEF(length, res, this->ReadCT_Double, &val);
			poVal->m_min = val;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Orientation(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		COrientation* poVal = static_cast<COrientation*>(poResult);
		if (c_oserct_orientationVAL == type)
		{
			poVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_AxPos(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CAxPos* poVal = static_cast<CAxPos*>(poResult);
		if (c_oserct_axposVAL == type)
		{
			poVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartLines(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_ChartLines* poVal = static_cast<CT_ChartLines*>(poResult);
		if (c_oserct_chartlinesSPPR == type)
		{
			poVal->m_spPr = new PPTX::Logic::SpPr;
			poVal->m_spPr->m_namespace = L"c";
			ReadCT_PptxElement(0, length, poVal->m_spPr.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Title(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Title* poVal = static_cast<CT_Title*>(poResult);
		if (c_oserct_titleTX == type)
		{
			CT_Tx* pNewElem = new CT_Tx;
			READ1_DEF(length, res, this->ReadCT_Tx, pNewElem);
			poVal->m_tx = pNewElem;
		}
		else if (c_oserct_titleLAYOUT == type)
		{
			CT_Layout* pNewElem = new CT_Layout;
			READ1_DEF(length, res, this->ReadCT_Layout, pNewElem);
			poVal->m_layout = pNewElem;
		}
		else if (c_oserct_titleOVERLAY == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_overlay = pNewElem;
		}
		else if (c_oserct_titleSPPR == type)
		{
			poVal->m_spPr = new PPTX::Logic::SpPr;
			poVal->m_spPr->m_namespace = L"c";
			ReadCT_PptxElement(0, length, poVal->m_spPr.GetPointer());
		}
		else if (c_oserct_titleTXPR == type)
		{
			poVal->m_txPr = new PPTX::Logic::TxBody;
			poVal->m_txPr->m_name = L"c:txPr";
			res = ReadCT_PptxElement(0, length, poVal->m_txPr.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Tx(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Tx* poVal = static_cast<CT_Tx*>(poResult);
		if (c_oserct_txRICH == type)
		{
			BYTE typeRec1 = m_oBufferedStream.GetUChar();

			poVal->m_oRich = new PPTX::Logic::TxBody;
			poVal->m_oRich->fromPPTY(&m_oBufferedStream);
			
			poVal->m_oRich->m_name = L"c:rich";
		}
		else if (c_oserct_txSTRREF == type)
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

		if (type > 0x80)
		{
			if (!poVal->m_extLst.IsInit()) poVal->m_extLst.Init();
			return ReadExtensions(type, length, poVal->m_extLst.GetPointer());
		}
//-----------------------------------------------------------------------------------------
		if (c_oserct_strrefF == type)
		{
			poVal->m_f = m_oBufferedStream.GetString4(length);
		}
		else if (c_oserct_strrefSTRCACHE == type)
		{
			CT_StrData* pNewElem = new CT_StrData;
			READ1_DEF(length, res, this->ReadCT_StrData, pNewElem);
			poVal->m_strCache = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_StrData(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_StrData* poVal = static_cast<CT_StrData*>(poResult);
		if (c_oserct_strdataPTCOUNT == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_ptCount = pNewElem;
		}
		else if (c_oserct_strdataPT == type)
		{
			CT_StrVal* pNewElem = new CT_StrVal;
			READ1_DEF(length, res, this->ReadCT_StrVal, pNewElem);
			poVal->m_pt.push_back(pNewElem);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_StrVal(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_StrVal* poVal = static_cast<CT_StrVal*>(poResult);
		if (c_oserct_strvalV == type)
		{
			poVal->m_v = m_oBufferedStream.GetString4(length);
		}
		else if (c_oserct_strvalIDX == type)
		{
			poVal->m_idx = (unsigned int)m_oBufferedStream.GetLong();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_NumFmt(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_NumFmt* poVal = static_cast<CT_NumFmt*>(poResult);
		if (c_oserct_numfmtFORMATCODE == type)
		{
			poVal->m_formatCode = m_oBufferedStream.GetString4(length);
		}
		else if (c_oserct_numfmtSOURCELINKED == type)
		{
			poVal->m_sourceLinked = m_oBufferedStream.GetBool();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_TickMark(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CTickMark* poVal = static_cast<CTickMark*>(poResult);
		if (c_oserct_tickmarkVAL == type)
		{
			poVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_TickLblPos(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CTickLblPos* poVal = static_cast<CTickLblPos*>(poResult);
		if (c_oserct_ticklblposVAL == type)
		{
			poVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Crosses(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CCrosses* poVal = static_cast<CCrosses*>(poResult);
		if (c_oserct_crossesVAL == type)
		{
			poVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_TimeUnit(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CTimeUnit* poVal = static_cast<CTimeUnit*>(poResult);
		if (c_oserct_timeunitVAL == type)
		{
			poVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_DateAx(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_DateAx* poVal = static_cast<CT_DateAx*>(poResult);
		if (c_oserct_dateaxAXID == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_axId = pNewElem;
		}
		else if (c_oserct_dateaxSCALING == type)
		{
			CT_Scaling* pNewElem = new CT_Scaling;
			READ1_DEF(length, res, this->ReadCT_Scaling, pNewElem);
			poVal->m_scaling = pNewElem;
		}
		else if (c_oserct_dateaxDELETE == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_delete = pNewElem;
		}
		else if (c_oserct_dateaxAXPOS == type)
		{
			poVal->m_axPos.Init();
			READ1_DEF(length, res, this->ReadCT_AxPos, poVal->m_axPos.GetPointer());
		}
		else if (c_oserct_dateaxMAJORGRIDLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_majorGridlines = pNewElem;
		}
		else if (c_oserct_dateaxMINORGRIDLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_minorGridlines = pNewElem;
		}
		else if (c_oserct_dateaxTITLE == type)
		{
			CT_Title* pNewElem = new CT_Title;
			READ1_DEF(length, res, this->ReadCT_Title, pNewElem);
			poVal->m_title = pNewElem;
		}
		else if (c_oserct_dateaxNUMFMT == type)
		{
			CT_NumFmt* pNewElem = new CT_NumFmt;
			READ1_DEF(length, res, this->ReadCT_NumFmt, pNewElem);
			poVal->m_numFmt = pNewElem;
		}
		else if (c_oserct_dateaxMAJORTICKMARK == type)
		{
			poVal->m_majorTickMark.Init();
			READ1_DEF(length, res, this->ReadCT_TickMark, poVal->m_majorTickMark.GetPointer());
		}
		else if (c_oserct_dateaxMINORTICKMARK == type)
		{
			poVal->m_minorTickMark.Init();
			READ1_DEF(length, res, this->ReadCT_TickMark, poVal->m_minorTickMark.GetPointer());
		}
		else if (c_oserct_dateaxTICKLBLPOS == type)
		{
			poVal->m_tickLblPos.Init();
			READ1_DEF(length, res, this->ReadCT_TickLblPos, poVal->m_tickLblPos.GetPointer());
		}
		else if (c_oserct_dateaxSPPR == type)
		{
			poVal->m_spPr = new PPTX::Logic::SpPr;
			poVal->m_spPr->m_namespace = L"c";
			ReadCT_PptxElement(0, length, poVal->m_spPr.GetPointer());
		}
		else if (c_oserct_dateaxTXPR == type)
		{
			poVal->m_txPr = new PPTX::Logic::TxBody;
			poVal->m_txPr->m_name = L"c:txPr";
			res = ReadCT_PptxElement(0, length, poVal->m_txPr.GetPointer());
		}
		else if (c_oserct_dateaxCROSSAX == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_crossAx = pNewElem;
		}
		else if (c_oserct_dateaxCROSSES == type)
		{
			poVal->m_crosses.Init();
			READ1_DEF(length, res, this->ReadCT_Crosses, poVal->m_crosses.GetPointer());
		}
		else if (c_oserct_dateaxCROSSESAT == type)
		{
			double pNewElem;
			READ1_DEF(length, res, this->ReadCT_Double, &pNewElem);
			poVal->m_crossesAt = pNewElem;
		}
		else if (c_oserct_dateaxAUTO == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_auto = pNewElem;
		}
		else if (c_oserct_dateaxLBLOFFSET == type)
		{
			std::wstring val;
			READ1_DEF(length, res, this->ReadCT_String, &val);
			poVal->m_lblOffset = val;
		}
		else if (c_oserct_dateaxBASETIMEUNIT == type)
		{
			poVal->m_baseTimeUnit.Init();
			READ1_DEF(length, res, this->ReadCT_TimeUnit, poVal->m_baseTimeUnit.GetPointer());
		}
		else if (c_oserct_dateaxMAJORUNIT == type)
		{
			double val;
			READ1_DEF(length, res, this->ReadCT_Double, &val);
			poVal->m_majorUnit = val;
		}
		else if (c_oserct_dateaxMAJORTIMEUNIT == type)
		{
			poVal->m_majorTimeUnit.Init();
			READ1_DEF(length, res, this->ReadCT_TimeUnit, poVal->m_majorTimeUnit.GetPointer());
		}
		else if (c_oserct_dateaxMINORUNIT == type)
		{
			double val;
			READ1_DEF(length, res, this->ReadCT_Double, &val);
			poVal->m_minorUnit = val;
		}
		else if (c_oserct_dateaxMINORTIMEUNIT == type)
		{
			poVal->m_minorTimeUnit.Init();
			READ1_DEF(length, res, this->ReadCT_TimeUnit, poVal->m_minorTimeUnit.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_LblAlgn(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CLblAlgn* poVal = static_cast<CLblAlgn*>(poResult);
		if (c_oserct_lblalgnVAL == type)
		{
			poVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_CatAx(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_CatAx* poVal = static_cast<CT_CatAx*>(poResult);
		if (c_oserct_cataxAXID == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_axId = pNewElem;
		}
		else if (c_oserct_cataxSCALING == type)
		{
			CT_Scaling* pNewElem = new CT_Scaling;
			READ1_DEF(length, res, this->ReadCT_Scaling, pNewElem);
			poVal->m_scaling = pNewElem;
		}
		else if (c_oserct_cataxDELETE == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_delete = pNewElem;
		}
		else if (c_oserct_cataxAXPOS == type)
		{
			poVal->m_axPos.Init();
			READ1_DEF(length, res, this->ReadCT_AxPos, poVal->m_axPos.GetPointer());
		}
		else if (c_oserct_cataxMAJORGRIDLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_majorGridlines = pNewElem;
		}
		else if (c_oserct_cataxMINORGRIDLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_minorGridlines = pNewElem;
		}
		else if (c_oserct_cataxTITLE == type)
		{
			CT_Title* pNewElem = new CT_Title;
			READ1_DEF(length, res, this->ReadCT_Title, pNewElem);
			poVal->m_title = pNewElem;
		}
		else if (c_oserct_cataxNUMFMT == type)
		{
			CT_NumFmt* pNewElem = new CT_NumFmt;
			READ1_DEF(length, res, this->ReadCT_NumFmt, pNewElem);
			poVal->m_numFmt = pNewElem;
		}
		else if (c_oserct_cataxMAJORTICKMARK == type)
		{
			poVal->m_majorTickMark.Init();
			READ1_DEF(length, res, this->ReadCT_TickMark, poVal->m_majorTickMark.GetPointer());
		}
		else if (c_oserct_cataxMINORTICKMARK == type)
		{
			poVal->m_minorTickMark.Init();
			READ1_DEF(length, res, this->ReadCT_TickMark, poVal->m_minorTickMark.GetPointer());
		}
		else if (c_oserct_cataxTICKLBLPOS == type)
		{
			poVal->m_tickLblPos.Init();
			READ1_DEF(length, res, this->ReadCT_TickLblPos, poVal->m_tickLblPos.GetPointer());
		}
		else if (c_oserct_cataxSPPR == type)
		{
			poVal->m_spPr = new PPTX::Logic::SpPr;
			poVal->m_spPr->m_namespace = L"c";
			ReadCT_PptxElement(0, length, poVal->m_spPr.GetPointer());
		}
		else if (c_oserct_cataxTXPR == type)
		{
			poVal->m_txPr = new PPTX::Logic::TxBody;
			poVal->m_txPr->m_name = L"c:txPr";
			res = ReadCT_PptxElement(0, length, poVal->m_txPr.GetPointer());
		}
		else if (c_oserct_cataxCROSSAX == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_crossAx = pNewElem;
		}
		else if (c_oserct_cataxCROSSES == type)
		{
			poVal->m_crosses.Init();
			READ1_DEF(length, res, this->ReadCT_Crosses, poVal->m_crosses.GetPointer());
		}
		else if (c_oserct_cataxCROSSESAT == type)
		{
			double pNewElem;
			READ1_DEF(length, res, this->ReadCT_Double, &pNewElem);
			poVal->m_crossesAt = pNewElem;
		}
		else if (c_oserct_cataxAUTO == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_auto = pNewElem;
		}
		else if (c_oserct_cataxLBLALGN == type)
		{
			poVal->m_lblAlgn.Init();
			READ1_DEF(length, res, this->ReadCT_LblAlgn, poVal->m_lblAlgn.GetPointer());
		}
		else if (c_oserct_cataxLBLOFFSET == type)
		{
			std::wstring val;
			READ1_DEF(length, res, this->ReadCT_String, &val);
			poVal->m_lblOffset = val;
		}
		else if (c_oserct_cataxTICKLBLSKIP == type)
		{
			unsigned int val;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &val);
			poVal->m_tickLblSkip = val;
		}
		else if (c_oserct_cataxTICKMARKSKIP == type)
		{
			unsigned int val;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &val);
			poVal->m_tickMarkSkip = val;
		}
		else if (c_oserct_cataxNOMULTILVLLBL == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_noMultiLvlLbl = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_DispUnitsLbl(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_DispUnitsLbl* poVal = static_cast<CT_DispUnitsLbl*>(poResult);
		if (c_oserct_dispunitslblLAYOUT == type)
		{
			CT_Layout* pNewElem = new CT_Layout;
			READ1_DEF(length, res, this->ReadCT_Layout, pNewElem);
			poVal->m_layout = pNewElem;
		}
		else if (c_oserct_dispunitslblTX == type)
		{
			CT_Tx* pNewElem = new CT_Tx;
			READ1_DEF(length, res, this->ReadCT_Tx, pNewElem);
			poVal->m_tx = pNewElem;
		}
		else if (c_oserct_dispunitslblSPPR == type)
		{
			poVal->m_spPr = new PPTX::Logic::SpPr;
			poVal->m_spPr->m_namespace = L"c";
			ReadCT_PptxElement(0, length, poVal->m_spPr.GetPointer());
		}
		else if (c_oserct_dispunitslblTXPR == type)
		{
			poVal->m_txPr = new PPTX::Logic::TxBody;
			poVal->m_txPr->m_name = L"c:txPr";
			res = ReadCT_PptxElement(0, length, poVal->m_txPr.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_BuiltInUnit(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CBuiltInUnit* poVal = static_cast<CBuiltInUnit*>(poResult);
		if (c_oserct_builtinunitVAL == type)
		{
			poVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_DispUnits(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_DispUnits* poVal = static_cast<CT_DispUnits*>(poResult);
		if (c_oserct_dispunitsBUILTINUNIT == type)
		{
			poVal->m_builtInUnit.Init();
			READ1_DEF(length, res, this->ReadCT_BuiltInUnit, poVal->m_builtInUnit.GetPointer());
		}
		else if (c_oserct_dispunitsCUSTUNIT == type)
		{
			double pNewElem;
			READ1_DEF(length, res, this->ReadCT_Double, &pNewElem);
			poVal->m_custUnit = pNewElem;
		}
		else if (c_oserct_dispunitsDISPUNITSLBL == type)
		{
			CT_DispUnitsLbl* pNewElem = new CT_DispUnitsLbl;
			READ1_DEF(length, res, this->ReadCT_DispUnitsLbl, pNewElem);
			poVal->m_dispUnitsLbl = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_CrossBetween(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CCrossBetween* poVal = static_cast<CCrossBetween*>(poResult);
		if (c_oserct_crossbetweenVAL == type)
		{
			poVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ValAx(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_ValAx* poVal = static_cast<CT_ValAx*>(poResult);
		if (c_oserct_valaxAXID == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_axId = pNewElem;
		}
		else if (c_oserct_valaxSCALING == type)
		{
			CT_Scaling* pNewElem = new CT_Scaling;
			READ1_DEF(length, res, this->ReadCT_Scaling, pNewElem);
			poVal->m_scaling = pNewElem;
		}
		else if (c_oserct_valaxDELETE == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_delete = pNewElem;
		}
		else if (c_oserct_valaxAXPOS == type)
		{
			poVal->m_axPos.Init();
			READ1_DEF(length, res, this->ReadCT_AxPos, poVal->m_axPos.GetPointer());
		}
		else if (c_oserct_valaxMAJORGRIDLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_majorGridlines = pNewElem;
		}
		else if (c_oserct_valaxMINORGRIDLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_minorGridlines = pNewElem;
		}
		else if (c_oserct_valaxTITLE == type)
		{
			CT_Title* pNewElem = new CT_Title;
			READ1_DEF(length, res, this->ReadCT_Title, pNewElem);
			poVal->m_title = pNewElem;
		}
		else if (c_oserct_valaxNUMFMT == type)
		{
			CT_NumFmt* pNewElem = new CT_NumFmt;
			READ1_DEF(length, res, this->ReadCT_NumFmt, pNewElem);
			poVal->m_numFmt = pNewElem;
		}
		else if (c_oserct_valaxMAJORTICKMARK == type)
		{
			poVal->m_majorTickMark.Init();
			READ1_DEF(length, res, this->ReadCT_TickMark, poVal->m_majorTickMark.GetPointer());
		}
		else if (c_oserct_valaxMINORTICKMARK == type)
		{
			poVal->m_minorTickMark.Init();
			READ1_DEF(length, res, this->ReadCT_TickMark, poVal->m_minorTickMark.GetPointer());
		}
		else if (c_oserct_valaxTICKLBLPOS == type)
		{
			poVal->m_tickLblPos.Init();
			READ1_DEF(length, res, this->ReadCT_TickLblPos, poVal->m_tickLblPos.GetPointer());
		}
		else if (c_oserct_valaxSPPR == type)
		{
			poVal->m_spPr = new PPTX::Logic::SpPr;
			poVal->m_spPr->m_namespace = L"c";
			ReadCT_PptxElement(0, length, poVal->m_spPr.GetPointer());
		}
		else if (c_oserct_valaxTXPR == type)
		{
			poVal->m_txPr = new PPTX::Logic::TxBody;
			poVal->m_txPr->m_name = L"c:txPr";
			res = ReadCT_PptxElement(0, length, poVal->m_txPr.GetPointer());
		}
		else if (c_oserct_valaxCROSSAX == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_crossAx = pNewElem;
		}
		else if (c_oserct_valaxCROSSES == type)
		{
			poVal->m_crosses.Init();
			READ1_DEF(length, res, this->ReadCT_Crosses, poVal->m_crosses.GetPointer());
		}
		else if (c_oserct_valaxCROSSESAT == type)
		{
			double val;
			READ1_DEF(length, res, this->ReadCT_Double, &val);
			poVal->m_crossesAt = val;
		}
		else if (c_oserct_valaxCROSSBETWEEN == type)
		{
			poVal->m_crossBetween.Init();
			READ1_DEF(length, res, this->ReadCT_CrossBetween, poVal->m_crossBetween.GetPointer());
		}
		else if (c_oserct_valaxMAJORUNIT == type)
		{
			double val;
			READ1_DEF(length, res, this->ReadCT_Double, &val);
			poVal->m_majorUnit = val;
		}
		else if (c_oserct_valaxMINORUNIT == type)
		{
			double val;
			READ1_DEF(length, res, this->ReadCT_Double, &val);
			poVal->m_minorUnit = val;
		}
		else if (c_oserct_valaxDISPUNITS == type)
		{
			CT_DispUnits* pNewElem = new CT_DispUnits;
			READ1_DEF(length, res, this->ReadCT_DispUnits, pNewElem);
			poVal->m_dispUnits = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_SizeRepresents(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CSizeRepresents* poVal = static_cast<CSizeRepresents*>(poResult);
		if (c_oserct_sizerepresentsVAL == type)
		{
			poVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_BubbleSer(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_BubbleSer* poVal = static_cast<CT_BubbleSer*>(poResult);

		if (type > 0x80)
		{
			if (!poVal->m_extLst.IsInit()) poVal->m_extLst.Init();
			return ReadExtensions(type, length, poVal->m_extLst.GetPointer());
		}
//-----------------------------------------------------------------------------------------
		if (c_oserct_bubbleserIDX == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_idx = pNewElem;
		}
		else if (c_oserct_bubbleserORDER == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_order = pNewElem;
		}
		else if (c_oserct_bubbleserTX == type)
		{
			CT_SerTx* pNewElem = new CT_SerTx;
			READ1_DEF(length, res, this->ReadCT_SerTx, pNewElem);
			poVal->m_tx = pNewElem;
		}
		else if (c_oserct_bubbleserSPPR == type)
		{
			poVal->m_spPr = new PPTX::Logic::SpPr;
			poVal->m_spPr->m_namespace = L"c";
			ReadCT_PptxElement(0, length, poVal->m_spPr.GetPointer());
		}
		else if (c_oserct_bubbleserINVERTIFNEGATIVE == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_invertIfNegative = pNewElem;
		}
		else if (c_oserct_bubbleserDPT == type)
		{
			CT_DPt* pNewElem = new CT_DPt;
			READ1_DEF(length, res, this->ReadCT_DPt, pNewElem);
			poVal->m_dPt.push_back(pNewElem);
		}
		else if (c_oserct_bubbleserDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if (c_oserct_bubbleserTRENDLINE == type)
		{
			CT_Trendline* pNewElem = new CT_Trendline;
			READ1_DEF(length, res, this->ReadCT_Trendline, pNewElem);
			poVal->m_trendline.push_back(pNewElem);
		}
		else if (c_oserct_bubbleserERRBARS == type)
		{
			CT_ErrBars* pNewElem = new CT_ErrBars;
			READ1_DEF(length, res, this->ReadCT_ErrBars, pNewElem);
			poVal->m_errBars.push_back(pNewElem);
		}
		else if (c_oserct_bubbleserXVAL == type)
		{
			CT_AxDataSource* pNewElem = new CT_AxDataSource;
			READ1_DEF(length, res, this->ReadCT_AxDataSource, pNewElem);
			poVal->m_xVal = pNewElem;
		}
		else if (c_oserct_bubbleserYVAL == type)
		{
			CT_NumDataSource* pNewElem = new CT_NumDataSource;
			READ1_DEF(length, res, this->ReadCT_NumDataSource, pNewElem);
			poVal->m_yVal = pNewElem;
		}
		else if (c_oserct_bubbleserBUBBLESIZE == type)
		{
			CT_NumDataSource* pNewElem = new CT_NumDataSource;
			READ1_DEF(length, res, this->ReadCT_NumDataSource, pNewElem);
			poVal->m_bubbleSize = pNewElem;
		}
		else if (c_oserct_bubbleserBUBBLE3D == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_bubble3D = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_SerTx(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_SerTx* poVal = static_cast<CT_SerTx*>(poResult);
		if (c_oserct_sertxSTRREF == type)
		{
			CT_StrRef* pNewElem = new CT_StrRef;
			READ1_DEF(length, res, this->ReadCT_StrRef, pNewElem);
			poVal->m_strRef = pNewElem;
		}
		else if (c_oserct_sertxV == type)
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
		if (c_oserct_dptIDX == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_idx = pNewElem;
		}
		else if (c_oserct_dptINVERTIFNEGATIVE == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_invertIfNegative = pNewElem;
		}
		else if (c_oserct_dptMARKER == type)
		{
			CT_Marker* pNewElem = new CT_Marker;
			READ1_DEF(length, res, this->ReadCT_Marker, pNewElem);
			poVal->m_marker = pNewElem;
		}
		else if (c_oserct_dptBUBBLE3D == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_bubble3D = pNewElem;
		}
		else if (c_oserct_dptEXPLOSION == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_explosion = pNewElem;
		}
		else if (c_oserct_dptSPPR == type)
		{
			poVal->m_spPr = new PPTX::Logic::SpPr;
			poVal->m_spPr->m_namespace = L"c";
			ReadCT_PptxElement(0, length, poVal->m_spPr.GetPointer());
		}
		else if (c_oserct_dptPICTUREOPTIONS == type)
		{
			CT_PictureOptions* pNewElem = new CT_PictureOptions;
			READ1_DEF(length, res, this->ReadCT_PictureOptions, pNewElem);
			poVal->m_pictureOptions = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Marker(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Marker* poVal = static_cast<CT_Marker*>(poResult);
		if (c_oserct_markerSYMBOL == type)
		{
			poVal->m_symbol.Init();
			READ1_DEF(length, res, this->ReadCT_MarkerStyle, poVal->m_symbol.GetPointer());
		}
		else if (c_oserct_markerSIZE == type)
		{
			unsigned int val;
			READ1_DEF(length, res, this->ReadCT_UnsignedByte, &val);
			poVal->m_size = val;
		}
		else if (c_oserct_markerSPPR == type)
		{
			poVal->m_spPr = new PPTX::Logic::SpPr;
			poVal->m_spPr->m_namespace = L"c";
			ReadCT_PptxElement(0, length, poVal->m_spPr.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_MarkerStyle(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CMarkerStyle* poVal = static_cast<CMarkerStyle*>(poResult);
		if (c_oserct_markerstyleVAL == type)
		{
			poVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_PictureOptions(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_PictureOptions* poVal = static_cast<CT_PictureOptions*>(poResult);
		if (c_oserct_pictureoptionsAPPLYTOFRONT == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_applyToFront = pNewElem;
		}
		else if (c_oserct_pictureoptionsAPPLYTOSIDES == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_applyToSides = pNewElem;
		}
		else if (c_oserct_pictureoptionsAPPLYTOEND == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_applyToEnd = pNewElem;
		}
		else if (c_oserct_pictureoptionsPICTUREFORMAT == type)
		{
			poVal->m_pictureFormat.Init();
			READ1_DEF(length, res, this->ReadCT_PictureFormat, poVal->m_pictureFormat.GetPointer());
		}
		else if (c_oserct_pictureoptionsPICTURESTACKUNIT == type)
		{
			double val;
			READ1_DEF(length, res, this->ReadCT_Double, &val);
			poVal->m_pictureStackUnit = val;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_PictureFormat(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CPictureFormat* poVal = static_cast<CPictureFormat*>(poResult);
		if (c_oserct_pictureformatVAL == type)
		{
			poVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_dataLabelsRange(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::CSeriesDataLabelsRange* pVal = static_cast<OOX::Spreadsheet::CSeriesDataLabelsRange*>(poResult);

		if (type == c_oserct_dataLabelsRangeFormula)
		{
			pVal->m_f = m_oBufferedStream.GetString4(length);
		}
		else if (type == c_oserct_dataLabelsRangeCache)
		{
			pVal->m_dlblRangeCache.Init();
			READ1_DEF(length, res, this->ReadCT_StrData, pVal->m_dlblRangeCache.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_filterException(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::CCategoryFilterException* pVal = static_cast<OOX::Spreadsheet::CCategoryFilterException*>(poResult);

		if (type == c_oserct_filterSqref)
		{
			pVal->m_sqref = m_oBufferedStream.GetString4(length);
		}
		else if (type == c_oserct_filterSpPr)
		{
			pVal->m_spPr = new PPTX::Logic::SpPr;
			pVal->m_spPr->m_namespace = L"c15";
			res = ReadCT_PptxElement(0, length, pVal->m_spPr.GetPointer());
		}
		else if (type == c_oserct_filterExplosion)
		{
			unsigned int val;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &val);
			pVal->m_explosion = val;
		}
		else if (type == c_oserct_filterInvertIfNegative)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			pVal->m_invertIfNegative = pNewElem;
		}
		else if (type == c_oserct_filterBubble3D)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			pVal->m_bubble3D = pNewElem;
		}
		else if (type == c_oserct_filterMarker)
		{
			pVal->m_marker.Init();
			READ1_DEF(length, res, this->ReadCT_Marker, pVal->m_marker.GetPointer());
		}
		else if (type == c_oserct_filterLbl)
		{
			pVal->m_dLbl.Init();
			READ1_DEF(length, res, this->ReadCT_DLbl, pVal->m_dLbl.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartFiltering(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::CSeriesFiltering* pVal = static_cast<OOX::Spreadsheet::CSeriesFiltering*>(poResult);

		if (type == c_oserct_dataLabelsRange)
		{
			pVal->m_dataLabelsRange.Init();
			READ1_DEF(length, res, this->ReadCT_dataLabelsRange, pVal->m_dataLabelsRange.GetPointer());
		}
		else if (type == c_oserct_filteredSeriesTitle)
		{
			pVal->m_filteredSeriesTitle.Init();
			READ1_DEF(length, res, this->ReadCT_Tx, pVal->m_filteredSeriesTitle.GetPointer());
		}
		else if (type == c_oserct_filteredCategoryTitle)
		{
			pVal->m_filteredCategoryTitle.Init();
			READ1_DEF(length, res, this->ReadCT_AxDataSource, pVal->m_filteredCategoryTitle.GetPointer());
		}
		else if (type == c_oserct_filteredLineSeries)
		{
			pVal->m_filteredLineSeries.Init();
			READ1_DEF(length, res, this->ReadCT_LineSer, pVal->m_filteredLineSeries.GetPointer());
		}
		else if (type == c_oserct_filteredScatterSeries)
		{
			pVal->m_filteredScatterSeries.Init();
			READ1_DEF(length, res, this->ReadCT_ScatterSer, pVal->m_filteredScatterSeries.GetPointer());
		}
		else if (type == c_oserct_filteredRadarSeries)
		{
			pVal->m_filteredRadarSeries.Init();
			READ1_DEF(length, res, this->ReadCT_RadarSer, pVal->m_filteredRadarSeries.GetPointer());
		}
		else if (type == c_oserct_filteredBarSeries)
		{
			pVal->m_filteredBarSeries.Init();
			READ1_DEF(length, res, this->ReadCT_BarSer, pVal->m_filteredBarSeries.GetPointer());
		}
		else if (type == c_oserct_filteredAreaSeries)
		{
			pVal->m_filteredAreaSeries.Init();
			READ1_DEF(length, res, this->ReadCT_AreaSer, pVal->m_filteredAreaSeries.GetPointer());
		}
		else if (type == c_oserct_filteredBubbleSeries)
		{
			pVal->m_filteredBubbleSeries.Init();
			READ1_DEF(length, res, this->ReadCT_BubbleSer, pVal->m_filteredBubbleSeries.GetPointer());
		}
		else if (type == c_oserct_filteredSurfaceSeries)
		{
			pVal->m_filteredSurfaceSeries.Init();
			READ1_DEF(length, res, this->ReadCT_SurfaceSer, pVal->m_filteredSurfaceSeries.GetPointer());
		}
		else if (type == c_oserct_filteredPieSeries)
		{
			pVal->m_filteredPieSeries.Init();
			READ1_DEF(length, res, this->ReadCT_PieSer, pVal->m_filteredPieSeries.GetPointer());
		}
		else if (type == c_oserct_fullRef)
		{
			pVal->m_fullRef = m_oBufferedStream.GetString4(length);
		}
		else if (type == c_oserct_levelRef)
		{
			pVal->m_levelRef = m_oBufferedStream.GetString4(length);
		}
		else if (type == c_oserct_formulaRef)
		{
			pVal->m_formulaRef = m_oBufferedStream.GetString4(length);
		}
		else if (type == c_oserct_categoryFilterExceptions)
		{
			pVal->m_categoryFilterExceptions.Init();
			READ1_DEF(length, res, this->ReadCT_filterExceptions, pVal->m_categoryFilterExceptions.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_filterExceptions(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::CCategoryFilterExceptions* pVal = static_cast<OOX::Spreadsheet::CCategoryFilterExceptions*>(poResult);

		if (type == c_oserct_categoryFilterException)
		{
			pVal->m_arrItems.push_back(new OOX::Spreadsheet::CCategoryFilterException());
			READ1_DEF(length, res, this->ReadCT_filterException, pVal->m_arrItems.back());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadExtensions(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Drawing::COfficeArtExtensionList* pVal = static_cast<OOX::Drawing::COfficeArtExtensionList*>(poResult);

		if (type == c_oserct_dataLabel)
		{
			pVal->m_arrExt.push_back(new OOX::Drawing::COfficeArtExtension());
			pVal->m_arrExt.back()->m_sUri = L"{CE6537A1-D6FC-4f65-9D91-7224C49458BB}";
			pVal->m_arrExt.back()->m_oChartDataLabel.Init();			
			
			READ1_DEF(length, res, this->ReadCT_DLbl, pVal->m_arrExt.back()->m_oChartDataLabel.GetPointer());
		}
		else if (type == c_oserct_chartFiltering)
		{
			pVal->m_arrExt.push_back(new OOX::Drawing::COfficeArtExtension());
			pVal->m_arrExt.back()->m_sUri = L"{02D57815-91ED-43cb-92C2-25804820EDAC}";
			pVal->m_arrExt.back()->m_oChartFiltering.Init();

			READ1_DEF(length, res, this->ReadCT_ChartFiltering, pVal->m_arrExt.back()->m_oChartFiltering.GetPointer());
		}
		else if (type == c_oserct_chartDataDisplayNaAsBlank)
		{
			pVal->m_arrExt.push_back(new OOX::Drawing::COfficeArtExtension());
			pVal->m_arrExt.back()->m_sUri = L"{56B9EC1D-385E-4148-901F-78D8002777C0}";
			pVal->m_arrExt.back()->m_sAdditionalNamespace = L"xmlns:c16r3=\"http://schemas.microsoft.com/office/drawing/2017/03/chart\"";

			pVal->m_arrExt.back()->m_oDataDisplayNaAsBlank = m_oBufferedStream.GetBool();
		}
		else if (type == c_oserct_chartExternalReference)
		{
			pVal->m_arrExt.push_back(new OOX::Drawing::COfficeArtExtension());
			pVal->m_arrExt.back()->m_sUri = L"{C3750BE0-5CA9-4D1C-82C7-79D762991C26}";

			READ1_DEF(length, res, this->ReadCT_ChartExternalReference, pVal->m_arrExt.back());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExternalReference(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Drawing::COfficeArtExtension* poVal = static_cast<OOX::Drawing::COfficeArtExtension*>(poResult);

		if (type == c_oserct_chartDataExternalFileKey)
		{
			poVal->m_oFileKey = m_oBufferedStream.GetString4(length);
		}
		else if (type == c_oserct_chartDataExternalInstanceId)
		{
			poVal->m_oInstanceId = m_oBufferedStream.GetString4(length);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_DLbls(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_DLbls* poVal = static_cast<CT_DLbls*>(poResult);

		if (type > 0x80)
		{
			if (!poVal->m_extLst.IsInit()) poVal->m_extLst.Init();
			return ReadExtensions(type, length, poVal->m_extLst.GetPointer());
		}
//-----------------------------------------------------------------------------------------
		switch (type)
		{
			case c_oserct_dlblsDLBL:
			{
				CT_DLbl* pNewElem = new CT_DLbl;
				READ1_DEF(length, res, this->ReadCT_DLbl, pNewElem);
				poVal->m_dLbl.push_back(pNewElem);
			}break;
			case c_oserct_dlblsDLBLPOS:
			{
				CDLblPos* pNewElem = new CDLblPos;
				READ1_DEF(length, res, this->ReadCT_DLblPos, pNewElem);
				ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
				*eElemtype = itemschoicetype3DLBLPOS;
				poVal->m_ItemsElementName0.push_back(eElemtype);
				poVal->m_Items.push_back(pNewElem);
			}break;
			case c_oserct_dlblsDELETE:
			{
				bool *pNewElem = new bool;
				READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
				ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
				*eElemtype = itemschoicetype3DELETE;
				poVal->m_ItemsElementName0.push_back(eElemtype);
				poVal->m_Items.push_back(pNewElem);
			}break;
			case c_oserct_dlblsLEADERLINES:
			{
				CT_ChartLines* pNewElem = new CT_ChartLines;
				READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
				ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
				*eElemtype = itemschoicetype3LEADERLINES;
				poVal->m_ItemsElementName0.push_back(eElemtype);
				poVal->m_Items.push_back(pNewElem);
			}break;
			case c_oserct_dlblsNUMFMT:
			{
				CT_NumFmt* pNewElem = new CT_NumFmt;
				READ1_DEF(length, res, this->ReadCT_NumFmt, pNewElem);
				ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
				*eElemtype = itemschoicetype3NUMFMT;
				poVal->m_ItemsElementName0.push_back(eElemtype);
				poVal->m_Items.push_back(pNewElem);
			}break;
			case c_oserct_dlblsSEPARATOR:
			{
				std::wstring* pNewElem = new std::wstring;
				*pNewElem = m_oBufferedStream.GetString4(length);
				ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
				*eElemtype = itemschoicetype3SEPARATOR;
				poVal->m_ItemsElementName0.push_back(eElemtype);
				poVal->m_Items.push_back(pNewElem);
			}break;
			case c_oserct_dlblsSHOWBUBBLESIZE:
			{
				bool *pNewElem = new bool;
				READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
				ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
				*eElemtype = itemschoicetype3SHOWBUBBLESIZE;
				poVal->m_ItemsElementName0.push_back(eElemtype);
				poVal->m_Items.push_back(pNewElem);
			}break;
			case c_oserct_dlblsSHOWCATNAME:
			{
				bool *pNewElem = new bool;
				READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
				ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
				*eElemtype = itemschoicetype3SHOWCATNAME;
				poVal->m_ItemsElementName0.push_back(eElemtype);
				poVal->m_Items.push_back(pNewElem);
			}break;
			case c_oserct_dlblsSHOWLEADERLINES:
			{
				bool *pNewElem = new bool;
				READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
				ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
				*eElemtype = itemschoicetype3SHOWLEADERLINES;
				poVal->m_ItemsElementName0.push_back(eElemtype);
				poVal->m_Items.push_back(pNewElem);
			}break;
			case c_oserct_dlblsSHOWLEGENDKEY:
			{
				bool *pNewElem = new bool;
				READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
				ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
				*eElemtype = itemschoicetype3SHOWLEGENDKEY;
				poVal->m_ItemsElementName0.push_back(eElemtype);
				poVal->m_Items.push_back(pNewElem);
			}break;
			case c_oserct_dlblsSHOWPERCENT:
			{
				bool *pNewElem = new bool;
				READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
				ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
				*eElemtype = itemschoicetype3SHOWPERCENT;
				poVal->m_ItemsElementName0.push_back(eElemtype);
				poVal->m_Items.push_back(pNewElem);
			}break;
			case c_oserct_dlblsSHOWSERNAME:
			{
				bool *pNewElem = new bool;
				READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
				ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
				*eElemtype = itemschoicetype3SHOWSERNAME;
				poVal->m_ItemsElementName0.push_back(eElemtype);
				poVal->m_Items.push_back(pNewElem);
			}break;
			case c_oserct_dlblsSHOWVAL:
			{
				bool *pNewElem = new bool;;
				READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
				poVal->m_Items.push_back(pNewElem);

				ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
				*eElemtype = itemschoicetype3SHOWVAL;
				poVal->m_ItemsElementName0.push_back(eElemtype);
			}break;
			case c_oserct_dlblsSPPR:
			{
				ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
				*eElemtype = itemschoicetype3SPPR;
				poVal->m_ItemsElementName0.push_back(eElemtype);

				PPTX::Logic::SpPr * pNewElem = new PPTX::Logic::SpPr();
				pNewElem->m_namespace = L"c";
				res = ReadCT_PptxElement(0, length, pNewElem);
				poVal->m_Items.push_back(pNewElem);
			}break;
			case c_oserct_dlblsTXPR:
			{
				ItemsChoiceType3* eElemtype = new ItemsChoiceType3;
				*eElemtype = itemschoicetype3TXPR;
				poVal->m_ItemsElementName0.push_back(eElemtype);

				PPTX::Logic::TxBody * pNewElem = new PPTX::Logic::TxBody();
				pNewElem->m_name = L"c:txPr";
				res = ReadCT_PptxElement(0, length, pNewElem);
				poVal->m_Items.push_back(pNewElem);
			}break;
			default:
				res = c_oSerConstants::ReadUnknown;
		}
		return res;
	}
	int BinaryChartReader::ReadCT_DLbl(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_DLbl* poVal = static_cast<CT_DLbl*>(poResult);
		
		if (type > 0x80)
		{
			if (!poVal->m_extLst.IsInit()) poVal->m_extLst.Init();
			return ReadExtensions(type, length, poVal->m_extLst.GetPointer());
		}
//----------------------------------------------------------------------------------------
		switch (type)
		{
			case c_oserct_dlblIDX:
			{
				unsigned int pNewElem;
				READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
				poVal->m_idx = pNewElem;
			}break;
			case c_oserct_dlblDLBLPOS:
			{
				CDLblPos* pNewElem = new CDLblPos;
				READ1_DEF(length, res, this->ReadCT_DLblPos, pNewElem);
				ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
				*eElemtype = itemschoicetype4DLBLPOS;
				poVal->m_ItemsElementName0.push_back(eElemtype);
				poVal->m_Items.push_back(pNewElem);
			}break;
			case c_oserct_dlblDELETE:
			{
				bool *pNewElem = new bool;
				READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
				ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
				*eElemtype = itemschoicetype4DELETE;
				poVal->m_ItemsElementName0.push_back(eElemtype);
				poVal->m_Items.push_back(pNewElem);
			}break;
			case c_oserct_dlblLAYOUT:
			{
				CT_Layout* pNewElem = new CT_Layout;
				READ1_DEF(length, res, this->ReadCT_Layout, pNewElem);
				ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
				*eElemtype = itemschoicetype4LAYOUT;
				poVal->m_ItemsElementName0.push_back(eElemtype);
				poVal->m_Items.push_back(pNewElem);
			}break;
			case c_oserct_dlblNUMFMT:
			{
				CT_NumFmt* pNewElem = new CT_NumFmt;
				READ1_DEF(length, res, this->ReadCT_NumFmt, pNewElem);
				ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
				*eElemtype = itemschoicetype4NUMFMT;
				poVal->m_ItemsElementName0.push_back(eElemtype);
				poVal->m_Items.push_back(pNewElem);
			}break;
			case c_oserct_dlblSEPARATOR:
			{
				std::wstring* pNewElem = new std::wstring;
				*pNewElem = m_oBufferedStream.GetString4(length);
				ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
				*eElemtype = itemschoicetype4SEPARATOR;
				poVal->m_ItemsElementName0.push_back(eElemtype);
				poVal->m_Items.push_back(pNewElem);
			}break;
			case c_oserct_dlblSHOWBUBBLESIZE:
			{
				bool *pNewElem = new bool;
				READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
				ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
				*eElemtype = itemschoicetype4SHOWBUBBLESIZE;
				poVal->m_ItemsElementName0.push_back(eElemtype);
				poVal->m_Items.push_back(pNewElem);
			}break;
			case c_oserct_dlblSHOWCATNAME:
			{
				bool *pNewElem = new bool;
				READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
				ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
				*eElemtype = itemschoicetype4SHOWCATNAME;
				poVal->m_ItemsElementName0.push_back(eElemtype);
				poVal->m_Items.push_back(pNewElem);
			}break;
			case c_oserct_dlblSHOWLEGENDKEY:
			{
				bool *pNewElem = new bool;
				READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
				ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
				*eElemtype = itemschoicetype4SHOWLEGENDKEY;
				poVal->m_ItemsElementName0.push_back(eElemtype);
				poVal->m_Items.push_back(pNewElem);
			}break;
			case c_oserct_dlblSHOWPERCENT:
			{
				bool *pNewElem = new bool;
				READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
				ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
				*eElemtype = itemschoicetype4SHOWPERCENT;
				poVal->m_ItemsElementName0.push_back(eElemtype);
				poVal->m_Items.push_back(pNewElem);
			}break;
			case c_oserct_dlblSHOWSERNAME:
			{
				bool *pNewElem = new bool;
				READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
				ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
				*eElemtype = itemschoicetype4SHOWSERNAME;
				poVal->m_ItemsElementName0.push_back(eElemtype);
				poVal->m_Items.push_back(pNewElem);
			}break;
			case c_oserct_dlblSHOWVAL:
			{
				bool *pNewElem = new bool;
				READ1_DEF(length, res, this->ReadCT_Boolean, pNewElem);
				ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
				*eElemtype = itemschoicetype4SHOWVAL;
				poVal->m_ItemsElementName0.push_back(eElemtype);
				poVal->m_Items.push_back(pNewElem);
			}break;
			case c_oserct_dlblSPPR:
			{
				ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
				*eElemtype = itemschoicetype4SPPR;
				poVal->m_ItemsElementName0.push_back(eElemtype);

				PPTX::Logic::SpPr *pNewElem = new PPTX::Logic::SpPr;
				pNewElem->m_namespace = L"c";
				res = ReadCT_PptxElement(0, length, pNewElem);
				poVal->m_Items.push_back(pNewElem);
			}break;
			case c_oserct_dlblTX:
			{
				CT_Tx* pNewElem = new CT_Tx;
				READ1_DEF(length, res, this->ReadCT_Tx, pNewElem);
				ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
				*eElemtype = itemschoicetype4TX;
				poVal->m_ItemsElementName0.push_back(eElemtype);
				poVal->m_Items.push_back(pNewElem);
			}break;
			case c_oserct_dlblTXPR:
			{
				ItemsChoiceType4* eElemtype = new ItemsChoiceType4;
				*eElemtype = itemschoicetype4TXPR;
				poVal->m_ItemsElementName0.push_back(eElemtype);

				PPTX::Logic::TxBody * pNewElem = new PPTX::Logic::TxBody();
				pNewElem->m_name = L"c:txPr";
				res = ReadCT_PptxElement(0, length, pNewElem);
				poVal->m_Items.push_back(pNewElem);
			}break;
			case c_oserct_xForSave:
			{
				bool val;
				READ1_DEF(length, res, this->ReadCT_Boolean, &val);
				poVal->m_xForSave = val;
			}break;
			case c_oserct_showDataLabelsRange:
			{
				bool val;
				READ1_DEF(length, res, this->ReadCT_Boolean, &val);
				poVal->m_showDataLabelsRange = val;
			}break;
			case c_oserct_showLeaderLines:
			{
				bool val;
				READ1_DEF(length, res, this->ReadCT_Boolean, &val);
				poVal->m_showLeaderLines = val;
			}break;
			case c_oserct_leaderLines:
			{
				poVal->m_leaderLines = new CT_ChartLines;
				READ1_DEF(length, res, this->ReadCT_ChartLines, poVal->m_leaderLines);
			}break;
			default:
				res = c_oSerConstants::ReadUnknown;
		}
		return res;
	}
	int BinaryChartReader::ReadCT_DLblPos(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CDLblPos* poVal = static_cast<CDLblPos*>(poResult);
		
		poVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		return res;
	}
	int BinaryChartReader::ReadCT_Trendline(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Trendline* poVal = static_cast<CT_Trendline*>(poResult);
		if (c_oserct_trendlineNAME == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_name = pNewElem;
		}
		else if (c_oserct_trendlineSPPR == type)
		{
			poVal->m_spPr = new PPTX::Logic::SpPr;
			poVal->m_spPr->m_namespace = L"c";
			res = ReadCT_PptxElement(0, length, poVal->m_spPr.GetPointer());
		}
		else if (c_oserct_trendlineTRENDLINETYPE == type)
		{
			poVal->m_trendlineType.Init();
			READ1_DEF(length, res, this->ReadCT_TrendlineType, poVal->m_trendlineType.GetPointer());
		}
		else if (c_oserct_trendlineORDER == type)
		{
			unsigned int val;
			READ1_DEF(length, res, this->ReadCT_UnsignedByte, &val);
			poVal->m_order = val;
		}
		else if (c_oserct_trendlinePERIOD == type)
		{
			unsigned int val;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &val);
			poVal->m_period = val;
		}
		else if (c_oserct_trendlineFORWARD == type)
		{
			double pNewElem;
			READ1_DEF(length, res, this->ReadCT_Double, &pNewElem);
			poVal->m_forward = pNewElem;
		}
		else if (c_oserct_trendlineBACKWARD == type)
		{
			double pNewElem;
			READ1_DEF(length, res, this->ReadCT_Double, &pNewElem);
			poVal->m_backward = pNewElem;
		}
		else if (c_oserct_trendlineINTERCEPT == type)
		{
			double pNewElem;
			READ1_DEF(length, res, this->ReadCT_Double, &pNewElem);
			poVal->m_intercept = pNewElem;
		}
		else if (c_oserct_trendlineDISPRSQR == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_dispRSqr = pNewElem;
		}
		else if (c_oserct_trendlineDISPEQ == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_dispEq = pNewElem;
		}
		else if (c_oserct_trendlineTRENDLINELBL == type)
		{
			CT_TrendlineLbl* pNewElem = new CT_TrendlineLbl;
			READ1_DEF(length, res, this->ReadCT_TrendlineLbl, pNewElem);
			poVal->m_trendlineLbl = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_TrendlineType(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CTrendlineType* poVal = static_cast<CTrendlineType*>(poResult);
		if (c_oserct_trendlinetypeVAL == type)
		{
			poVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_TrendlineLbl(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_TrendlineLbl* poVal = static_cast<CT_TrendlineLbl*>(poResult);
		if (c_oserct_trendlinelblLAYOUT == type)
		{
			CT_Layout* pNewElem = new CT_Layout;
			READ1_DEF(length, res, this->ReadCT_Layout, pNewElem);
			poVal->m_layout = pNewElem;
		}
		else if (c_oserct_trendlinelblTX == type)
		{
			CT_Tx* pNewElem = new CT_Tx;
			READ1_DEF(length, res, this->ReadCT_Tx, pNewElem);
			poVal->m_tx = pNewElem;
		}
		else if (c_oserct_trendlinelblNUMFMT == type)
		{
			CT_NumFmt* pNewElem = new CT_NumFmt;
			READ1_DEF(length, res, this->ReadCT_NumFmt, pNewElem);
			poVal->m_numFmt = pNewElem;
		}
		else if (c_oserct_trendlinelblSPPR == type)
		{
			poVal->m_spPr = new PPTX::Logic::SpPr;
			poVal->m_spPr->m_namespace = L"c";
			res = ReadCT_PptxElement(0, length, poVal->m_spPr.GetPointer());
		}
		else if (c_oserct_trendlinelblTXPR == type)
		{
			poVal->m_txPr = new PPTX::Logic::TxBody;
			poVal->m_txPr->m_name = L"c:txPr";
			res = ReadCT_PptxElement(0, length, poVal->m_txPr.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ErrBars(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_ErrBars* poVal = static_cast<CT_ErrBars*>(poResult);
		if (c_oserct_errbarsERRDIR == type)
		{
			poVal->m_errDir.Init();
			READ1_DEF(length, res, this->ReadCT_ErrDir, poVal->m_errDir.GetPointer());
		}
		else if (c_oserct_errbarsERRBARTYPE == type)
		{
			poVal->m_errBarType.Init();
			READ1_DEF(length, res, this->ReadCT_ErrBarType, poVal->m_errBarType.GetPointer());
		}
		else if (c_oserct_errbarsERRVALTYPE == type)
		{
			poVal->m_errValType.Init();
			READ1_DEF(length, res, this->ReadCT_ErrValType, poVal->m_errValType.GetPointer());
		}
		else if (c_oserct_errbarsNOENDCAP == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_noEndCap = pNewElem;
		}
		else if (c_oserct_errbarsPLUS == type)
		{
			CT_NumDataSource* pNewElem = new CT_NumDataSource;
			READ1_DEF(length, res, this->ReadCT_NumDataSource, pNewElem);
			poVal->m_plus = pNewElem;
		}
		else if (c_oserct_errbarsMINUS == type)
		{
			CT_NumDataSource* pNewElem = new CT_NumDataSource;
			READ1_DEF(length, res, this->ReadCT_NumDataSource, pNewElem);
			poVal->m_minus = pNewElem;
		}
		else if (c_oserct_errbarsVAL == type)
		{
			double pNewElem;
			READ1_DEF(length, res, this->ReadCT_Double, &pNewElem);
			poVal->m_val = pNewElem;
		}
		else if (c_oserct_errbarsSPPR == type)
		{
			poVal->m_spPr = new PPTX::Logic::SpPr;
			poVal->m_spPr->m_namespace = L"c";
			res = ReadCT_PptxElement(0, length, poVal->m_spPr.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ErrDir(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CErrDir* poVal = static_cast<CErrDir*>(poResult);
		if (c_oserct_errdirVAL == type)
		{
			poVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ErrBarType(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CErrBarType* poVal = static_cast<CErrBarType*>(poResult);
		if (c_oserct_errbartypeVAL == type)
		{
			poVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ErrValType(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CErrValType* poVal = static_cast<CErrValType*>(poResult);
		if (c_oserct_errvaltypeVAL == type)
		{
			poVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_NumDataSource(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_NumDataSource* poVal = static_cast<CT_NumDataSource*>(poResult);
		if (c_oserct_numdatasourceNUMLIT == type)
		{
			CT_NumData* pNewElem = new CT_NumData;
			READ1_DEF(length, res, this->ReadCT_NumData, pNewElem);
			poVal->m_numLit = pNewElem;
		}
		else if (c_oserct_numdatasourceNUMREF == type)
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
		if (c_oserct_numdataFORMATCODE == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_formatCode = pNewElem;
		}
		else if (c_oserct_numdataPTCOUNT == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_ptCount = pNewElem;
		}
		else if (c_oserct_numdataPT == type)
		{
			CT_NumVal* pNewElem = new CT_NumVal;
			READ1_DEF(length, res, this->ReadCT_NumVal, pNewElem);
			poVal->m_pt.push_back(pNewElem);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_NumVal(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_NumVal* poVal = static_cast<CT_NumVal*>(poResult);
		if (c_oserct_numvalV == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_v = pNewElem;
		}
		else if (c_oserct_numvalIDX == type)
		{
			poVal->m_idx = (unsigned int)m_oBufferedStream.GetLong();
		}
		else if (c_oserct_numvalFORMATCODE == type)
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

		if (type > 0x80)
		{
			if (!poVal->m_extLst.IsInit()) poVal->m_extLst.Init();
			return ReadExtensions(type, length, poVal->m_extLst.GetPointer());
		}
//-----------------------------------------------------------------------------------------
		if (c_oserct_numrefF == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_f = pNewElem;
		}
		else if (c_oserct_numrefNUMCACHE == type)
		{
			CT_NumData* pNewElem = new CT_NumData;
			READ1_DEF(length, res, this->ReadCT_NumData, pNewElem);
			poVal->m_numCache = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_AxDataSource(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_AxDataSource* poVal = static_cast<CT_AxDataSource*>(poResult);
		if (c_oserct_axdatasourceMULTILVLSTRREF == type)
		{
			CT_MultiLvlStrRef* pNewElem = new CT_MultiLvlStrRef;
			READ1_DEF(length, res, this->ReadCT_MultiLvlStrRef, pNewElem);
			poVal->m_multiLvlStrRef = pNewElem;
		}
		else if (c_oserct_axdatasourceNUMLIT == type)
		{
			CT_NumData* pNewElem = new CT_NumData;
			READ1_DEF(length, res, this->ReadCT_NumData, pNewElem);
			poVal->m_numLit = pNewElem;
		}
		else if (c_oserct_axdatasourceNUMREF == type)
		{
			CT_NumRef* pNewElem = new CT_NumRef;
			READ1_DEF(length, res, this->ReadCT_NumRef, pNewElem);
			poVal->m_numRef = pNewElem;
		}
		else if (c_oserct_axdatasourceSTRLIT == type)
		{
			CT_StrData* pNewElem = new CT_StrData;
			READ1_DEF(length, res, this->ReadCT_StrData, pNewElem);
			poVal->m_strLit = pNewElem;
		}
		else if (c_oserct_axdatasourceSTRREF == type)
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
		if (c_oserct_multilvlstrrefF == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_f = pNewElem;
		}
		else if (c_oserct_multilvlstrrefMULTILVLSTRCACHE == type)
		{
			CT_MultiLvlStrData* pNewElem = new CT_MultiLvlStrData;
			READ1_DEF(length, res, this->ReadCT_MultiLvlStrData, pNewElem);
			poVal->m_multiLvlStrCache = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_lvl(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_lvl* poVal = static_cast<CT_lvl*>(poResult);
		if (c_oserct_lvlPT == type)
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
		if (c_oserct_multilvlstrdataPTCOUNT == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_ptCount = pNewElem;
		}
		else if (c_oserct_multilvlstrdataLVL == type)
		{
			CT_lvl* pNewElem = new CT_lvl;
			READ1_DEF(length, res, this->ReadCT_lvl, pNewElem);
			poVal->m_lvl.push_back(pNewElem);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_BubbleChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_BubbleChart* poVal = static_cast<CT_BubbleChart*>(poResult);

		if (type > 0x80)
		{
			if (!poVal->m_extLst.IsInit()) poVal->m_extLst.Init();
			return ReadExtensions(type, length, poVal->m_extLst.GetPointer());
		}
//-----------------------------------------------------------------------------------------
		if (c_oserct_bubblechartVARYCOLORS == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_varyColors = pNewElem;
		}
		else if (c_oserct_bubblechartSER == type)
		{
			CT_BubbleSer* pNewElem = new CT_BubbleSer;
			READ1_DEF(length, res, this->ReadCT_BubbleSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if (c_oserct_bubblechartDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if (c_oserct_bubblechartBUBBLE3D == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_bubble3D = pNewElem;
		}
		else if (c_oserct_bubblechartBUBBLESCALE == type)
		{
			std::wstring val;
			READ1_DEF(length, res, this->ReadCT_String, &val);
			poVal->m_bubbleScale = val;
		}
		else if (c_oserct_bubblechartSHOWNEGBUBBLES == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_showNegBubbles = pNewElem;
		}
		else if (c_oserct_bubblechartSIZEREPRESENTS == type)
		{
			poVal->m_sizeRepresents.Init();
			READ1_DEF(length, res, this->ReadCT_SizeRepresents, poVal->m_sizeRepresents.GetPointer());
		}
		else if (c_oserct_bubblechartAXID == type)
		{
			unsigned int val;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &val);
			poVal->m_axId.push_back(val);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_bandFmts(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_bandFmts* poVal = static_cast<CT_bandFmts*>(poResult);
		if (c_oserct_bandfmtsBANDFMT == type)
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

		if (type > 0x80)
		{
			if (!poVal->m_extLst.IsInit()) poVal->m_extLst.Init();
			return ReadExtensions(type, length, poVal->m_extLst.GetPointer());
		}
//-----------------------------------------------------------------------------------------
		if (c_oserct_surface3dchartWIREFRAME == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_wireframe = pNewElem;
		}
		else if (c_oserct_surface3dchartSER == type)
		{
			CT_SurfaceSer* pNewElem = new CT_SurfaceSer;
			READ1_DEF(length, res, this->ReadCT_SurfaceSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if (c_oserct_surface3dchartBANDFMTS == type)
		{
			CT_bandFmts* pNewElem = new CT_bandFmts;
			READ1_DEF(length, res, this->ReadCT_bandFmts, pNewElem);
			poVal->m_bandFmts = pNewElem;
		}
		else if (c_oserct_surface3dchartAXID == type)
		{
			unsigned int val;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &val);
			poVal->m_axId.push_back(val);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_SurfaceSer(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_SurfaceSer* poVal = static_cast<CT_SurfaceSer*>(poResult);

		if (type > 0x80)
		{
			if (!poVal->m_extLst.IsInit()) poVal->m_extLst.Init();
			return ReadExtensions(type, length, poVal->m_extLst.GetPointer());
		}
//-----------------------------------------------------------------------------------------
		if (c_oserct_surfaceserIDX == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_idx = pNewElem;
		}
		else if (c_oserct_surfaceserORDER == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_order = pNewElem;
		}
		else if (c_oserct_surfaceserTX == type)
		{
			CT_SerTx* pNewElem = new CT_SerTx;
			READ1_DEF(length, res, this->ReadCT_SerTx, pNewElem);
			poVal->m_tx = pNewElem;
		}
		else if (c_oserct_surfaceserSPPR == type)
		{
			poVal->m_spPr = new PPTX::Logic::SpPr;
			poVal->m_spPr->m_namespace = L"c";
			res = ReadCT_PptxElement(0, length, poVal->m_spPr.GetPointer());
		}
		else if (c_oserct_surfaceserCAT == type)
		{
			CT_AxDataSource* pNewElem = new CT_AxDataSource;
			READ1_DEF(length, res, this->ReadCT_AxDataSource, pNewElem);
			poVal->m_cat = pNewElem;
		}
		else if (c_oserct_surfaceserVAL == type)
		{
			CT_NumDataSource* pNewElem = new CT_NumDataSource;
			READ1_DEF(length, res, this->ReadCT_NumDataSource, pNewElem);
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_BandFmt(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_BandFmt* poVal = static_cast<CT_BandFmt*>(poResult);
		if (c_oserct_bandfmtIDX == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_idx = pNewElem;
		}
		else if (c_oserct_bandfmtSPPR == type)
		{
			poVal->m_spPr = new PPTX::Logic::SpPr;
			poVal->m_spPr->m_namespace = L"c";
			res = ReadCT_PptxElement(0, length, poVal->m_spPr.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_SurfaceChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_SurfaceChart* poVal = static_cast<CT_SurfaceChart*>(poResult);

		if (type > 0x80)
		{
			if (!poVal->m_extLst.IsInit()) poVal->m_extLst.Init();
			return ReadExtensions(type, length, poVal->m_extLst.GetPointer());
		}
//-----------------------------------------------------------------------------------------
		if (c_oserct_surfacechartWIREFRAME == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_wireframe = pNewElem;
		}
		else if (c_oserct_surfacechartSER == type)
		{
			CT_SurfaceSer* pNewElem = new CT_SurfaceSer;
			READ1_DEF(length, res, this->ReadCT_SurfaceSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if (c_oserct_surfacechartBANDFMTS == type)
		{
			CT_bandFmts* pNewElem = new CT_bandFmts;
			READ1_DEF(length, res, this->ReadCT_bandFmts, pNewElem);
			poVal->m_bandFmts = pNewElem;
		}
		else if (c_oserct_surfacechartAXID == type)
		{
			unsigned int val;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &val);
			poVal->m_axId.push_back(val);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_SplitType(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CSplitType* poVal = static_cast<CSplitType*>(poResult);
		if (c_oserct_splittypeVAL == type)
		{
			poVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_OfPieType(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		COfPieType* poVal = static_cast<COfPieType*>(poResult);
		if (c_oserct_ofpietypeVAL == type)
		{
			poVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_custSplit(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_custSplit* poVal = static_cast<CT_custSplit*>(poResult);
		if (c_oserct_custsplitSECONDPIEPT == type)
		{
			unsigned int val;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &val);

			poVal->m_secondPiePt.push_back(val);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_OfPieChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_OfPieChart* poVal = static_cast<CT_OfPieChart*>(poResult);

		if (type > 0x80)
		{
			if (!poVal->m_extLst.IsInit()) poVal->m_extLst.Init();
			return ReadExtensions(type, length, poVal->m_extLst.GetPointer());
		}
//-----------------------------------------------------------------------------------------
		if (c_oserct_ofpiechartOFPIETYPE == type)
		{
			poVal->m_ofPieType.Init();
			READ1_DEF(length, res, this->ReadCT_OfPieType, poVal->m_ofPieType.GetPointer());
		}
		else if (c_oserct_ofpiechartVARYCOLORS == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_varyColors = pNewElem;
		}
		else if (c_oserct_ofpiechartSER == type)
		{
			CT_PieSer* pNewElem = new CT_PieSer;
			READ1_DEF(length, res, this->ReadCT_PieSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if (c_oserct_ofpiechartDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if (c_oserct_ofpiechartGAPWIDTH == type)
		{
			std::wstring val;
			READ1_DEF(length, res, this->ReadCT_String, &val);
			poVal->m_gapWidth = val;
		}
		else if (c_oserct_ofpiechartSPLITTYPE == type)
		{
			poVal->m_splitType.Init();
			READ1_DEF(length, res, this->ReadCT_SplitType, poVal->m_splitType.GetPointer());
		}
		else if (c_oserct_ofpiechartSPLITPOS == type)
		{
			double pNewElem;
			READ1_DEF(length, res, this->ReadCT_Double, &pNewElem);
			poVal->m_splitPos = pNewElem;
		}
		else if (c_oserct_ofpiechartCUSTSPLIT == type)
		{
			CT_custSplit* pNewElem = new CT_custSplit;
			READ1_DEF(length, res, this->ReadCT_custSplit, pNewElem);
			poVal->m_custSplit = pNewElem;
		}
		else if (c_oserct_ofpiechartSECONDPIESIZE == type)
		{
			std::wstring val;
			READ1_DEF(length, res, this->ReadCT_String, &val);
			poVal->m_secondPieSize = val;
		}
		else if (c_oserct_ofpiechartSERLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_serLines.push_back(pNewElem);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_PieSer(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_PieSer* poVal = static_cast<CT_PieSer*>(poResult);

		if (type > 0x80)
		{
			if (!poVal->m_extLst.IsInit()) poVal->m_extLst.Init();
			return ReadExtensions(type, length, poVal->m_extLst.GetPointer());
		}
//-----------------------------------------------------------------------------------------
		if (c_oserct_pieserIDX == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_idx = pNewElem;
		}
		else if (c_oserct_pieserORDER == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_order = pNewElem;
		}
		else if (c_oserct_pieserTX == type)
		{
			CT_SerTx* pNewElem = new CT_SerTx;
			READ1_DEF(length, res, this->ReadCT_SerTx, pNewElem);
			poVal->m_tx = pNewElem;
		}
		else if (c_oserct_pieserSPPR == type)
		{
			poVal->m_spPr = new PPTX::Logic::SpPr;
			poVal->m_spPr->m_namespace = L"c";
			res = ReadCT_PptxElement(0, length, poVal->m_spPr.GetPointer());
		}
		else if (c_oserct_pieserEXPLOSION == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_explosion = pNewElem;
		}
		else if (c_oserct_pieserDPT == type)
		{
			CT_DPt* pNewElem = new CT_DPt;
			READ1_DEF(length, res, this->ReadCT_DPt, pNewElem);
			poVal->m_dPt.push_back(pNewElem);
		}
		else if (c_oserct_pieserDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if (c_oserct_pieserCAT == type)
		{
			CT_AxDataSource* pNewElem = new CT_AxDataSource;
			READ1_DEF(length, res, this->ReadCT_AxDataSource, pNewElem);
			poVal->m_cat = pNewElem;
		}
		else if (c_oserct_pieserVAL == type)
		{
			CT_NumDataSource* pNewElem = new CT_NumDataSource;
			READ1_DEF(length, res, this->ReadCT_NumDataSource, pNewElem);
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

		if (type > 0x80)
		{
			if (!poVal->m_extLst.IsInit()) poVal->m_extLst.Init();
			return ReadExtensions(type, length, poVal->m_extLst.GetPointer());
		}
//-----------------------------------------------------------------------------------------
		if (c_oserct_bar3dchartBARDIR == type)
		{
			poVal->m_barDir.Init();
			READ1_DEF(length, res, this->ReadCT_BarDir, poVal->m_barDir.GetPointer());
		}
		else if (c_oserct_bar3dchartGROUPING == type)
		{
			poVal->m_grouping.Init();
			READ1_DEF(length, res, this->ReadCT_BarGrouping, poVal->m_grouping.GetPointer());
		}
		else if (c_oserct_bar3dchartVARYCOLORS == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_varyColors = pNewElem;
		}
		else if (c_oserct_bar3dchartSER == type)
		{
			CT_BarSer* pNewElem = new CT_BarSer;
			READ1_DEF(length, res, this->ReadCT_BarSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if (c_oserct_bar3dchartDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if (c_oserct_bar3dchartGAPWIDTH == type)
		{
			std::wstring val;
			READ1_DEF(length, res, this->ReadCT_String, &val);
			poVal->m_gapWidth = val;
		}
		else if (c_oserct_bar3dchartGAPDEPTH == type)
		{
			std::wstring val;
			READ1_DEF(length, res, this->ReadCT_String, &val);
			poVal->m_gapDepth = val;
		}
		else if (c_oserct_bar3dchartSHAPE == type)
		{
			poVal->m_shape.Init();
			READ1_DEF(length, res, this->ReadCT_Shape, poVal->m_shape.GetPointer());
		}
		else if (c_oserct_bar3dchartAXID == type)
		{
			unsigned int val;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &val);
			poVal->m_axId.push_back(val);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_BarDir(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CBarDir* poVal = static_cast<CBarDir*>(poResult);
		
		poVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		return res;
	}
	int BinaryChartReader::ReadCT_BarGrouping(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CBarGrouping* poVal = static_cast<CBarGrouping*>(poResult);
		
		poVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		return res;
	}
	int BinaryChartReader::ReadCT_BarSer(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_BarSer* poVal = static_cast<CT_BarSer*>(poResult);

		if (type > 0x80)
		{
			if (!poVal->m_extLst.IsInit()) poVal->m_extLst.Init();
			return ReadExtensions(type, length, poVal->m_extLst.GetPointer());
		}
//-----------------------------------------------------------------------------------------
		if (c_oserct_barserIDX == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_idx = pNewElem;
		}
		else if (c_oserct_barserORDER == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_order = pNewElem;
		}
		else if (c_oserct_barserTX == type)
		{
			CT_SerTx* pNewElem = new CT_SerTx;
			READ1_DEF(length, res, this->ReadCT_SerTx, pNewElem);
			poVal->m_tx = pNewElem;
		}
		else if (c_oserct_barserSPPR == type)
		{
			poVal->m_spPr = new PPTX::Logic::SpPr;
			poVal->m_spPr->m_namespace = L"c";
			res = ReadCT_PptxElement(0, length, poVal->m_spPr.GetPointer());
		}
		else if (c_oserct_barserINVERTIFNEGATIVE == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_invertIfNegative = pNewElem;
		}
		else if (c_oserct_barserPICTUREOPTIONS == type)
		{
			CT_PictureOptions* pNewElem = new CT_PictureOptions;
			READ1_DEF(length, res, this->ReadCT_PictureOptions, pNewElem);
			poVal->m_pictureOptions = pNewElem;
		}
		else if (c_oserct_barserDPT == type)
		{
			CT_DPt* pNewElem = new CT_DPt;
			READ1_DEF(length, res, this->ReadCT_DPt, pNewElem);
			poVal->m_dPt.push_back(pNewElem);
		}
		else if (c_oserct_barserDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if (c_oserct_barserTRENDLINE == type)
		{
			CT_Trendline* pNewElem = new CT_Trendline;
			READ1_DEF(length, res, this->ReadCT_Trendline, pNewElem);
			poVal->m_trendline.push_back(pNewElem);
		}
		else if (c_oserct_barserERRBARS == type)
		{
			CT_ErrBars* pNewElem = new CT_ErrBars;
			READ1_DEF(length, res, this->ReadCT_ErrBars, pNewElem);
			poVal->m_errBars = pNewElem;
		}
		else if (c_oserct_barserCAT == type)
		{
			CT_AxDataSource* pNewElem = new CT_AxDataSource;
			READ1_DEF(length, res, this->ReadCT_AxDataSource, pNewElem);
			poVal->m_cat = pNewElem;
		}
		else if (c_oserct_barserVAL == type)
		{
			CT_NumDataSource* pNewElem = new CT_NumDataSource;
			READ1_DEF(length, res, this->ReadCT_NumDataSource, pNewElem);
			poVal->m_val = pNewElem;
		}
		else if (c_oserct_barserSHAPE == type)
		{
			poVal->m_shape.Init();
			READ1_DEF(length, res, this->ReadCT_Shape, poVal->m_shape.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Shape(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CShapeType* poVal = static_cast<CShapeType*>(poResult);
		
		poVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		return res;
	}
	int BinaryChartReader::ReadCT_BarChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_BarChart* poVal = static_cast<CT_BarChart*>(poResult);
		
		if (type > 0x80)
		{
			if (!poVal->m_extLst.IsInit()) poVal->m_extLst.Init();
			return ReadExtensions(type, length, poVal->m_extLst.GetPointer());
		}
//-----------------------------------------------------------------------------------------
		if (c_oserct_barchartBARDIR == type)
		{
			poVal->m_barDir.Init();
			READ1_DEF(length, res, this->ReadCT_BarDir, poVal->m_barDir.GetPointer());
		}
		else if (c_oserct_barchartGROUPING == type)
		{
			poVal->m_grouping.Init();
			READ1_DEF(length, res, this->ReadCT_BarGrouping, poVal->m_grouping.GetPointer());
		}
		else if (c_oserct_barchartVARYCOLORS == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_varyColors = pNewElem;
		}
		else if (c_oserct_barchartSER == type)
		{
			CT_BarSer* pNewElem = new CT_BarSer;
			READ1_DEF(length, res, this->ReadCT_BarSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if (c_oserct_barchartDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if (c_oserct_barchartGAPWIDTH == type)
		{
			std::wstring val;
			READ1_DEF(length, res, this->ReadCT_String, &val);
			poVal->m_gapWidth = val;
		}
		else if (c_oserct_barchartOVERLAP == type)
		{
			std::wstring val;
			READ1_DEF(length, res, this->ReadCT_String, &val);
			poVal->m_overlap = val;
		}
		else if (c_oserct_barchartSERLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_serLines.push_back(pNewElem);
		}
		else if (c_oserct_barchartAXID == type)
		{
			unsigned int val;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &val);
			poVal->m_axId.push_back(val);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_DoughnutChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_DoughnutChart* poVal = static_cast<CT_DoughnutChart*>(poResult);
		if (c_oserct_doughnutchartVARYCOLORS == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_varyColors = pNewElem;
		}
		else if (c_oserct_doughnutchartSER == type)
		{
			CT_PieSer* pNewElem = new CT_PieSer;
			READ1_DEF(length, res, this->ReadCT_PieSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if (c_oserct_doughnutchartDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if (c_oserct_doughnutchartFIRSTSLICEANG == type)
		{
			unsigned int val;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &val);
			poVal->m_firstSliceAng = val;
		}
		else if (c_oserct_doughnutchartHOLESIZE == type)
		{
			std::wstring val;
			READ1_DEF(length, res, this->ReadCT_String, &val);
			poVal->m_holeSize = val;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Pie3DChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Pie3DChart* poVal = static_cast<CT_Pie3DChart*>(poResult);

		if (type > 0x80)
		{
			if (!poVal->m_extLst.IsInit()) poVal->m_extLst.Init();
			return ReadExtensions(type, length, poVal->m_extLst.GetPointer());
		}
//-----------------------------------------------------------------------------------------
		if (c_oserct_pie3dchartVARYCOLORS == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_varyColors = pNewElem;
		}
		else if (c_oserct_pie3dchartSER == type)
		{
			CT_PieSer* pNewElem = new CT_PieSer;
			READ1_DEF(length, res, this->ReadCT_PieSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if (c_oserct_pie3dchartDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_PieChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_PieChart* poVal = static_cast<CT_PieChart*>(poResult);

		if (type > 0x80)
		{
			if (!poVal->m_extLst.IsInit()) poVal->m_extLst.Init();
			return ReadExtensions(type, length, poVal->m_extLst.GetPointer());
		}
//-----------------------------------------------------------------------------------------
		if (c_oserct_piechartVARYCOLORS == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_varyColors = pNewElem;
		}
		else if (c_oserct_piechartSER == type)
		{
			CT_PieSer* pNewElem = new CT_PieSer;
			READ1_DEF(length, res, this->ReadCT_PieSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if (c_oserct_piechartDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if (c_oserct_piechartFIRSTSLICEANG == type)
		{
			unsigned int val;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &val);
			poVal->m_firstSliceAng = val;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ScatterSer(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_ScatterSer* poVal = static_cast<CT_ScatterSer*>(poResult);

		if (type > 0x80)
		{
			if (!poVal->m_extLst.IsInit()) poVal->m_extLst.Init();
			return ReadExtensions(type, length, poVal->m_extLst.GetPointer());
		}
//-----------------------------------------------------------------------------------------
		if (c_oserct_scatterserIDX == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_idx = pNewElem;
		}
		else if (c_oserct_scatterserORDER == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_order = pNewElem;
		}
		else if (c_oserct_scatterserTX == type)
		{
			CT_SerTx* pNewElem = new CT_SerTx;
			READ1_DEF(length, res, this->ReadCT_SerTx, pNewElem);
			poVal->m_tx = pNewElem;
		}
		else if (c_oserct_scatterserSPPR == type)
		{
			poVal->m_spPr = new PPTX::Logic::SpPr;
			poVal->m_spPr->m_namespace = L"c";
			res = ReadCT_PptxElement(0, length, poVal->m_spPr.GetPointer());
		}
		else if (c_oserct_scatterserMARKER == type)
		{
			CT_Marker* pNewElem = new CT_Marker;
			READ1_DEF(length, res, this->ReadCT_Marker, pNewElem);
			poVal->m_marker = pNewElem;
		}
		else if (c_oserct_scatterserDPT == type)
		{
			CT_DPt* pNewElem = new CT_DPt;
			READ1_DEF(length, res, this->ReadCT_DPt, pNewElem);
			poVal->m_dPt.push_back(pNewElem);
		}
		else if (c_oserct_scatterserDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if (c_oserct_scatterserTRENDLINE == type)
		{
			CT_Trendline* pNewElem = new CT_Trendline;
			READ1_DEF(length, res, this->ReadCT_Trendline, pNewElem);
			poVal->m_trendline.push_back(pNewElem);
		}
		else if (c_oserct_scatterserERRBARS == type)
		{
			CT_ErrBars* pNewElem = new CT_ErrBars;
			READ1_DEF(length, res, this->ReadCT_ErrBars, pNewElem);
			poVal->m_errBars.push_back(pNewElem);
		}
		else if (c_oserct_scatterserXVAL == type)
		{
			CT_AxDataSource* pNewElem = new CT_AxDataSource;
			READ1_DEF(length, res, this->ReadCT_AxDataSource, pNewElem);
			poVal->m_xVal = pNewElem;
		}
		else if (c_oserct_scatterserYVAL == type)
		{
			CT_NumDataSource* pNewElem = new CT_NumDataSource;
			READ1_DEF(length, res, this->ReadCT_NumDataSource, pNewElem);
			poVal->m_yVal = pNewElem;
		}
		else if (c_oserct_scatterserSMOOTH == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_smooth = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ScatterStyle(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CScatterStyle* poVal = static_cast<CScatterStyle*>(poResult);
		if (c_oserct_scatterstyleVAL == type)
		{
			poVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ScatterChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_ScatterChart* poVal = static_cast<CT_ScatterChart*>(poResult);

		if (type > 0x80)
		{
			if (!poVal->m_extLst.IsInit()) poVal->m_extLst.Init();
			return ReadExtensions(type, length, poVal->m_extLst.GetPointer());
		}
//-----------------------------------------------------------------------------------------
		if (c_oserct_scatterchartSCATTERSTYLE == type)
		{
			poVal->m_scatterStyle.Init();
			READ1_DEF(length, res, this->ReadCT_ScatterStyle, poVal->m_scatterStyle.GetPointer());
		}
		else if (c_oserct_scatterchartVARYCOLORS == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_varyColors = pNewElem;
		}
		else if (c_oserct_scatterchartSER == type)
		{
			CT_ScatterSer* pNewElem = new CT_ScatterSer;
			READ1_DEF(length, res, this->ReadCT_ScatterSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if (c_oserct_scatterchartDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if (c_oserct_scatterchartAXID == type)
		{
			unsigned int val;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &val);
			poVal->m_axId.push_back(val);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_RadarSer(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_RadarSer* poVal = static_cast<CT_RadarSer*>(poResult);

		if (type > 0x80)
		{
			if (!poVal->m_extLst.IsInit()) poVal->m_extLst.Init();
			return ReadExtensions(type, length, poVal->m_extLst.GetPointer());
		}		
		if (c_oserct_radarserIDX == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_idx = pNewElem;
		}
		else if (c_oserct_radarserORDER == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_order = pNewElem;
		}
		else if (c_oserct_radarserTX == type)
		{
			CT_SerTx* pNewElem = new CT_SerTx;
			READ1_DEF(length, res, this->ReadCT_SerTx, pNewElem);
			poVal->m_tx = pNewElem;
		}
		else if (c_oserct_radarserSPPR == type)
		{
			poVal->m_spPr = new PPTX::Logic::SpPr;
			poVal->m_spPr->m_namespace = L"c";
			res = ReadCT_PptxElement(0, length, poVal->m_spPr.GetPointer());
		}
		else if (c_oserct_radarserMARKER == type)
		{
			CT_Marker* pNewElem = new CT_Marker;
			READ1_DEF(length, res, this->ReadCT_Marker, pNewElem);
			poVal->m_marker = pNewElem;
		}
		else if (c_oserct_radarserDPT == type)
		{
			CT_DPt* pNewElem = new CT_DPt;
			READ1_DEF(length, res, this->ReadCT_DPt, pNewElem);
			poVal->m_dPt.push_back(pNewElem);
		}
		else if (c_oserct_radarserDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if (c_oserct_radarserCAT == type)
		{
			CT_AxDataSource* pNewElem = new CT_AxDataSource;
			READ1_DEF(length, res, this->ReadCT_AxDataSource, pNewElem);
			poVal->m_cat = pNewElem;
		}
		else if (c_oserct_radarserVAL == type)
		{
			CT_NumDataSource* pNewElem = new CT_NumDataSource;
			READ1_DEF(length, res, this->ReadCT_NumDataSource, pNewElem);
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_RadarStyle(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CRadarStyle* poVal = static_cast<CRadarStyle*>(poResult);
		if (c_oserct_radarstyleVAL == type)
		{
			poVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_RadarChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_RadarChart* poVal = static_cast<CT_RadarChart*>(poResult);

		if (type > 0x80)
		{
			if (!poVal->m_extLst.IsInit()) poVal->m_extLst.Init();
			return ReadExtensions(type, length, poVal->m_extLst.GetPointer());
		}
//-----------------------------------------------------------------------------------------
		if (c_oserct_radarchartRADARSTYLE == type)
		{
			poVal->m_radarStyle.Init();
			READ1_DEF(length, res, this->ReadCT_RadarStyle, poVal->m_radarStyle.GetPointer());
		}
		else if (c_oserct_radarchartVARYCOLORS == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_varyColors = pNewElem;
		}
		else if (c_oserct_radarchartSER == type)
		{
			CT_RadarSer* pNewElem = new CT_RadarSer;
			READ1_DEF(length, res, this->ReadCT_RadarSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if (c_oserct_radarchartDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if (c_oserct_radarchartAXID == type)
		{
			unsigned int val;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &val);
			poVal->m_axId.push_back(val);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_StockChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_StockChart* poVal = static_cast<CT_StockChart*>(poResult);
		if (c_oserct_stockchartSER == type)
		{
			CT_LineSer* pNewElem = new CT_LineSer;
			READ1_DEF(length, res, this->ReadCT_LineSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if (c_oserct_stockchartDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if (c_oserct_stockchartDROPLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_dropLines = pNewElem;
		}
		else if (c_oserct_stockchartHILOWLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_hiLowLines = pNewElem;
		}
		else if (c_oserct_stockchartUPDOWNBARS == type)
		{
			CT_UpDownBars* pNewElem = new CT_UpDownBars;
			READ1_DEF(length, res, this->ReadCT_UpDownBars, pNewElem);
			poVal->m_upDownBars = pNewElem;
		}
		else if (c_oserct_stockchartAXID == type)
		{
			unsigned int val;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &val);
			poVal->m_axId.push_back(val);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_LineSer(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_LineSer* poVal = static_cast<CT_LineSer*>(poResult);

		if (type > 0x80)
		{
			if (!poVal->m_extLst.IsInit()) poVal->m_extLst.Init();
			return ReadExtensions(type, length, poVal->m_extLst.GetPointer());
		}
//-----------------------------------------------------------------------------------------
		if (c_oserct_lineserIDX == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_idx = pNewElem;
		}
		else if (c_oserct_lineserORDER == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_order = pNewElem;
		}
		else if (c_oserct_lineserTX == type)
		{
			CT_SerTx* pNewElem = new CT_SerTx;
			READ1_DEF(length, res, this->ReadCT_SerTx, pNewElem);
			poVal->m_tx = pNewElem;
		}
		else if (c_oserct_lineserSPPR == type)
		{
			poVal->m_spPr = new PPTX::Logic::SpPr;
			poVal->m_spPr->m_namespace = L"c";
			res = ReadCT_PptxElement(0, length, poVal->m_spPr.GetPointer());
		}
		else if (c_oserct_lineserMARKER == type)
		{
			CT_Marker* pNewElem = new CT_Marker;
			READ1_DEF(length, res, this->ReadCT_Marker, pNewElem);
			poVal->m_marker = pNewElem;
		}
		else if (c_oserct_lineserDPT == type)
		{
			CT_DPt* pNewElem = new CT_DPt;
			READ1_DEF(length, res, this->ReadCT_DPt, pNewElem);
			poVal->m_dPt.push_back(pNewElem);
		}
		else if (c_oserct_lineserDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if (c_oserct_lineserTRENDLINE == type)
		{
			CT_Trendline* pNewElem = new CT_Trendline;
			READ1_DEF(length, res, this->ReadCT_Trendline, pNewElem);
			poVal->m_trendline.push_back(pNewElem);
		}
		else if (c_oserct_lineserERRBARS == type)
		{
			CT_ErrBars* pNewElem = new CT_ErrBars;
			READ1_DEF(length, res, this->ReadCT_ErrBars, pNewElem);
			poVal->m_errBars = pNewElem;
		}
		else if (c_oserct_lineserCAT == type)
		{
			CT_AxDataSource* pNewElem = new CT_AxDataSource;
			READ1_DEF(length, res, this->ReadCT_AxDataSource, pNewElem);
			poVal->m_cat = pNewElem;
		}
		else if (c_oserct_lineserVAL == type)
		{
			CT_NumDataSource* pNewElem = new CT_NumDataSource;
			READ1_DEF(length, res, this->ReadCT_NumDataSource, pNewElem);
			poVal->m_val = pNewElem;
		}
		else if (c_oserct_lineserSMOOTH == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_smooth = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_UpDownBars(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_UpDownBars* poVal = static_cast<CT_UpDownBars*>(poResult);
		if (c_oserct_updownbarsGAPWIDTH == type)
		{
			std::wstring val;
			READ1_DEF(length, res, this->ReadCT_String, &val);
			poVal->m_gapWidth = val;
		}
		else if (c_oserct_updownbarsUPBARS == type)
		{
			CT_UpDownBar* pNewElem = new CT_UpDownBar;
			READ1_DEF(length, res, this->ReadCT_UpDownBar, pNewElem);
			poVal->m_upBars = pNewElem;
		}
		else if (c_oserct_updownbarsDOWNBARS == type)
		{
			CT_UpDownBar* pNewElem = new CT_UpDownBar;
			READ1_DEF(length, res, this->ReadCT_UpDownBar, pNewElem);
			poVal->m_downBars = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_UpDownBar(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_UpDownBar* poVal = static_cast<CT_UpDownBar*>(poResult);
		if (c_oserct_updownbarSPPR == type)
		{
			poVal->m_spPr = new PPTX::Logic::SpPr;
			poVal->m_spPr->m_namespace = L"c";
			res = ReadCT_PptxElement(0, length, poVal->m_spPr.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Line3DChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Line3DChart* poVal = static_cast<CT_Line3DChart*>(poResult);

		if (type > 0x80)
		{
			if (!poVal->m_extLst.IsInit()) poVal->m_extLst.Init();
			return ReadExtensions(type, length, poVal->m_extLst.GetPointer());
		}
//-----------------------------------------------------------------------------------------
		if (c_oserct_line3dchartGROUPING == type)
		{
			poVal->m_grouping.Init();
			READ1_DEF(length, res, this->ReadCT_Grouping, poVal->m_grouping.GetPointer());
		}
		else if (c_oserct_line3dchartVARYCOLORS == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_varyColors = pNewElem;
		}
		else if (c_oserct_line3dchartSER == type)
		{
			CT_LineSer* pNewElem = new CT_LineSer;
			READ1_DEF(length, res, this->ReadCT_LineSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if (c_oserct_line3dchartDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if (c_oserct_line3dchartDROPLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_dropLines = pNewElem;
		}
		else if (c_oserct_line3dchartGAPDEPTH == type)
		{
			std::wstring val;
			READ1_DEF(length, res, this->ReadCT_String, &val);
			poVal->m_gapDepth = val;
		}
		else if (c_oserct_line3dchartAXID == type)
		{
			unsigned int val;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &val);
			poVal->m_axId.push_back(val);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Grouping(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CGrouping* poVal = static_cast<CGrouping*>(poResult);
		if (c_oserct_groupingVAL == type)
		{
			poVal->SetValueFromByte(m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_LineChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_LineChart* poVal = static_cast<CT_LineChart*>(poResult);

		if (type > 0x80)
		{
			if (!poVal->m_extLst.IsInit()) poVal->m_extLst.Init();
			return ReadExtensions(type, length, poVal->m_extLst.GetPointer());
		}
//-----------------------------------------------------------------------------------------
		if (c_oserct_linechartGROUPING == type)
		{
			poVal->m_grouping.Init();
			READ1_DEF(length, res, this->ReadCT_Grouping, poVal->m_grouping.GetPointer());
		}
		else if (c_oserct_linechartVARYCOLORS == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_varyColors = pNewElem;
		}
		else if (c_oserct_linechartSER == type)
		{
			CT_LineSer* pNewElem = new CT_LineSer;
			READ1_DEF(length, res, this->ReadCT_LineSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if (c_oserct_linechartDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if (c_oserct_linechartDROPLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_dropLines = pNewElem;
		}
		else if (c_oserct_linechartHILOWLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_hiLowLines = pNewElem;
		}
		else if (c_oserct_linechartUPDOWNBARS == type)
		{
			CT_UpDownBars* pNewElem = new CT_UpDownBars;
			READ1_DEF(length, res, this->ReadCT_UpDownBars, pNewElem);
			poVal->m_upDownBars = pNewElem;
		}
		else if (c_oserct_linechartMARKER == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_marker = pNewElem;
		}
		else if (c_oserct_linechartSMOOTH == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_smooth = pNewElem;
		}
		else if (c_oserct_linechartAXID == type)
		{
			unsigned int val;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &val);
			
			poVal->m_axId.push_back(val);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Area3DChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Area3DChart* poVal = static_cast<CT_Area3DChart*>(poResult);

		if (type > 0x80)
		{
			if (!poVal->m_extLst.IsInit()) poVal->m_extLst.Init();
			return ReadExtensions(type, length, poVal->m_extLst.GetPointer());
		}
//-----------------------------------------------------------------------------------------
		if (c_oserct_area3dchartGROUPING == type)
		{
			poVal->m_grouping.Init();
			READ1_DEF(length, res, this->ReadCT_Grouping, poVal->m_grouping.GetPointer());
		}
		else if (c_oserct_area3dchartVARYCOLORS == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_varyColors = pNewElem;
		}
		else if (c_oserct_area3dchartSER == type)
		{
			CT_AreaSer* pNewElem = new CT_AreaSer;
			READ1_DEF(length, res, this->ReadCT_AreaSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if (c_oserct_area3dchartDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if (c_oserct_area3dchartDROPLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_dropLines = pNewElem;
		}
		else if (c_oserct_area3dchartGAPDEPTH == type)
		{
			std::wstring val;
			READ1_DEF(length, res, this->ReadCT_String, &val);
			poVal->m_gapDepth = val;
		}
		else if (c_oserct_area3dchartAXID == type)
		{
			unsigned int val;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &val);
			poVal->m_axId.push_back(val);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_AreaSer(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_AreaSer* poVal = static_cast<CT_AreaSer*>(poResult);

		if (type > 0x80)
		{
			if (!poVal->m_extLst.IsInit()) poVal->m_extLst.Init();
			return ReadExtensions(type, length, poVal->m_extLst.GetPointer());
		}
//-----------------------------------------------------------------------------------------
		if (c_oserct_areaserIDX == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_idx = pNewElem;
		}
		else if (c_oserct_areaserORDER == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_order = pNewElem;
		}
		else if (c_oserct_areaserTX == type)
		{
			CT_SerTx* pNewElem = new CT_SerTx;
			READ1_DEF(length, res, this->ReadCT_SerTx, pNewElem);
			poVal->m_tx = pNewElem;
		}
		else if (c_oserct_areaserSPPR == type)
		{
			poVal->m_spPr = new PPTX::Logic::SpPr;
			poVal->m_spPr->m_namespace = L"c";
			res = ReadCT_PptxElement(0, length, poVal->m_spPr.GetPointer());
		}
		else if (c_oserct_areaserPICTUREOPTIONS == type)
		{
			CT_PictureOptions* pNewElem = new CT_PictureOptions;
			READ1_DEF(length, res, this->ReadCT_PictureOptions, pNewElem);
			poVal->m_pictureOptions = pNewElem;
		}
		else if (c_oserct_areaserDPT == type)
		{
			CT_DPt* pNewElem = new CT_DPt;
			READ1_DEF(length, res, this->ReadCT_DPt, pNewElem);
			poVal->m_dPt.push_back(pNewElem);
		}
		else if (c_oserct_areaserDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if (c_oserct_areaserTRENDLINE == type)
		{
			CT_Trendline* pNewElem = new CT_Trendline;
			READ1_DEF(length, res, this->ReadCT_Trendline, pNewElem);
			poVal->m_trendline.push_back(pNewElem);
		}
		else if (c_oserct_areaserERRBARS == type)
		{
			CT_ErrBars* pNewElem = new CT_ErrBars;
			READ1_DEF(length, res, this->ReadCT_ErrBars, pNewElem);
			poVal->m_errBars.push_back(pNewElem);
		}
		else if (c_oserct_areaserCAT == type)
		{
			CT_AxDataSource* pNewElem = new CT_AxDataSource;
			READ1_DEF(length, res, this->ReadCT_AxDataSource, pNewElem);
			poVal->m_cat = pNewElem;
		}
		else if (c_oserct_areaserVAL == type)
		{
			CT_NumDataSource* pNewElem = new CT_NumDataSource;
			READ1_DEF(length, res, this->ReadCT_NumDataSource, pNewElem);
			poVal->m_val = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_AreaChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_AreaChart* poVal = static_cast<CT_AreaChart*>(poResult);

		if (type > 0x80)
		{
			if (!poVal->m_extLst.IsInit()) poVal->m_extLst.Init();
			return ReadExtensions(type, length, poVal->m_extLst.GetPointer());
		}
//-----------------------------------------------------------------------------------------
		if (c_oserct_areachartGROUPING == type)
		{
			poVal->m_grouping.Init();
			READ1_DEF(length, res, this->ReadCT_Grouping, poVal->m_grouping.GetPointer());
		}
		else if (c_oserct_areachartVARYCOLORS == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_varyColors = pNewElem;
		}
		else if (c_oserct_areachartSER == type)
		{
			CT_AreaSer* pNewElem = new CT_AreaSer;
			READ1_DEF(length, res, this->ReadCT_AreaSer, pNewElem);
			poVal->m_ser.push_back(pNewElem);
		}
		else if (c_oserct_areachartDLBLS == type)
		{
			CT_DLbls* pNewElem = new CT_DLbls;
			READ1_DEF(length, res, this->ReadCT_DLbls, pNewElem);
			poVal->m_dLbls = pNewElem;
		}
		else if (c_oserct_areachartDROPLINES == type)
		{
			CT_ChartLines* pNewElem = new CT_ChartLines;
			READ1_DEF(length, res, this->ReadCT_ChartLines, pNewElem);
			poVal->m_dropLines = pNewElem;
		}
		else if (c_oserct_areachartAXID == type)
		{
			unsigned int val;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &val);
			poVal->m_axId.push_back(val);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_PlotArea(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_PlotArea* poVal = static_cast<CT_PlotArea*>(poResult);
		if (c_oserct_plotareaLAYOUT == type)
		{
			CT_Layout* pNewElem = new CT_Layout;
			READ1_DEF(length, res, this->ReadCT_Layout, pNewElem);
			poVal->m_layout = pNewElem;
		}
		else if (c_oserct_plotareaAREA3DCHART == type)
		{
			CT_Area3DChart* pNewElem = new CT_Area3DChart;
			READ1_DEF(length, res, this->ReadCT_Area3DChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5AREA3DCHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if (c_oserct_plotareaAREACHART == type)
		{
			CT_AreaChart* pNewElem = new CT_AreaChart;
			READ1_DEF(length, res, this->ReadCT_AreaChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5AREACHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if (c_oserct_plotareaBAR3DCHART == type)
		{
			CT_Bar3DChart* pNewElem = new CT_Bar3DChart;
			READ1_DEF(length, res, this->ReadCT_Bar3DChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5BAR3DCHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if (c_oserct_plotareaBARCHART == type)
		{
			CT_BarChart* pNewElem = new CT_BarChart;
			READ1_DEF(length, res, this->ReadCT_BarChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5BARCHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if (c_oserct_plotareaBUBBLECHART == type)
		{
			CT_BubbleChart* pNewElem = new CT_BubbleChart;
			READ1_DEF(length, res, this->ReadCT_BubbleChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5BUBBLECHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if (c_oserct_plotareaDOUGHNUTCHART == type)
		{
			CT_DoughnutChart* pNewElem = new CT_DoughnutChart;
			READ1_DEF(length, res, this->ReadCT_DoughnutChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5DOUGHNUTCHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if (c_oserct_plotareaLINE3DCHART == type)
		{
			CT_Line3DChart* pNewElem = new CT_Line3DChart;
			READ1_DEF(length, res, this->ReadCT_Line3DChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5LINE3DCHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if (c_oserct_plotareaLINECHART == type)
		{
			CT_LineChart* pNewElem = new CT_LineChart;
			READ1_DEF(length, res, this->ReadCT_LineChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5LINECHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if (c_oserct_plotareaOFPIECHART == type)
		{
			CT_OfPieChart* pNewElem = new CT_OfPieChart;
			READ1_DEF(length, res, this->ReadCT_OfPieChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5OFPIECHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if (c_oserct_plotareaPIE3DCHART == type)
		{
			CT_Pie3DChart* pNewElem = new CT_Pie3DChart;
			READ1_DEF(length, res, this->ReadCT_Pie3DChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5PIE3DCHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if (c_oserct_plotareaPIECHART == type)
		{
			CT_PieChart* pNewElem = new CT_PieChart;
			READ1_DEF(length, res, this->ReadCT_PieChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5PIECHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if (c_oserct_plotareaRADARCHART == type)
		{
			CT_RadarChart* pNewElem = new CT_RadarChart;
			READ1_DEF(length, res, this->ReadCT_RadarChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5RADARCHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if (c_oserct_plotareaSCATTERCHART == type)
		{
			CT_ScatterChart* pNewElem = new CT_ScatterChart;
			READ1_DEF(length, res, this->ReadCT_ScatterChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5SCATTERCHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if (c_oserct_plotareaSTOCKCHART == type)
		{
			CT_StockChart* pNewElem = new CT_StockChart;
			READ1_DEF(length, res, this->ReadCT_StockChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5STOCKCHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if (c_oserct_plotareaSURFACE3DCHART == type)
		{
			CT_Surface3DChart* pNewElem = new CT_Surface3DChart;
			READ1_DEF(length, res, this->ReadCT_Surface3DChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5SURFACE3DCHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if (c_oserct_plotareaSURFACECHART == type)
		{
			CT_SurfaceChart* pNewElem = new CT_SurfaceChart;
			READ1_DEF(length, res, this->ReadCT_SurfaceChart, pNewElem);
			ItemsChoiceType5* eElemtype = new ItemsChoiceType5;
			*eElemtype = itemschoicetype5SURFACECHART;
			poVal->m_ItemsElementName0.push_back(eElemtype);
			poVal->m_Items.push_back(pNewElem);
		}
		else if (c_oserct_plotareaCATAX == type)
		{
			CT_CatAx* pNewElem = new CT_CatAx;
			READ1_DEF(length, res, this->ReadCT_CatAx, pNewElem);
			ItemsChoiceType6* eElemtype = new ItemsChoiceType6;
			*eElemtype = itemschoicetype6CATAX;
			poVal->m_ItemsElementName1.push_back(eElemtype);
			poVal->m_Items1.push_back(pNewElem);
		}
		else if (c_oserct_plotareaDATEAX == type)
		{
			CT_DateAx* pNewElem = new CT_DateAx;
			READ1_DEF(length, res, this->ReadCT_DateAx, pNewElem);
			ItemsChoiceType6* eElemtype = new ItemsChoiceType6;
			*eElemtype = itemschoicetype6DATEAX;
			poVal->m_ItemsElementName1.push_back(eElemtype);
			poVal->m_Items1.push_back(pNewElem);
		}
		else if (c_oserct_plotareaSERAX == type)
		{
			CT_SerAx* pNewElem = new CT_SerAx;
			READ1_DEF(length, res, this->ReadCT_SerAx, pNewElem);
			ItemsChoiceType6* eElemtype = new ItemsChoiceType6;
			*eElemtype = itemschoicetype6SERAX;
			poVal->m_ItemsElementName1.push_back(eElemtype);
			poVal->m_Items1.push_back(pNewElem);
		}
		else if (c_oserct_plotareaVALAX == type)
		{
			CT_ValAx* pNewElem = new CT_ValAx;
			READ1_DEF(length, res, this->ReadCT_ValAx, pNewElem);
			ItemsChoiceType6* eElemtype = new ItemsChoiceType6;
			*eElemtype = itemschoicetype6VALAX;
			poVal->m_ItemsElementName1.push_back(eElemtype);
			poVal->m_Items1.push_back(pNewElem);
		}
		else if (c_oserct_plotareaDTABLE == type)
		{
			CT_DTable* pNewElem = new CT_DTable;
			READ1_DEF(length, res, this->ReadCT_DTable, pNewElem);
			poVal->m_dTable = pNewElem;
		}
		else if (c_oserct_plotareaSPPR == type)
		{
			poVal->m_spPr = new PPTX::Logic::SpPr;
			poVal->m_spPr->m_namespace = L"c";
			res = ReadCT_PptxElement(0, length, poVal->m_spPr.GetPointer());
		}

		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Surface(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Surface* poVal = static_cast<CT_Surface*>(poResult);
		if (c_oserct_surfaceTHICKNESS == type)
		{
			std::wstring val;
			READ1_DEF(length, res, this->ReadCT_String, &val);
			poVal->m_thickness = val;
		}
		else if (c_oserct_surfaceSPPR == type)
		{
			poVal->m_spPr = new PPTX::Logic::SpPr;
			poVal->m_spPr->m_namespace = L"c";
			res = ReadCT_PptxElement(0, length, poVal->m_spPr.GetPointer());
		}
		else if (c_oserct_surfacePICTUREOPTIONS == type)
		{
			CT_PictureOptions* pNewElem = new CT_PictureOptions;
			READ1_DEF(length, res, this->ReadCT_PictureOptions, pNewElem);
			poVal->m_pictureOptions = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_View3D(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_View3D* poVal = static_cast<CT_View3D*>(poResult);
		if (c_oserct_view3dROTX == type)
		{
			int val;
			READ1_DEF(length, res, this->ReadCT_SignedByte, &val);
			poVal->m_rotX = val;
		}
		else if (c_oserct_view3dHPERCENT == type)
		{
			std::wstring val;
			READ1_DEF(length, res, this->ReadCT_String, &val);
			poVal->m_hPercent = val;
		}
		else if (c_oserct_view3dROTY == type)
		{
			unsigned int val;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &val);
			poVal->m_rotY = val;
		}
		else if (c_oserct_view3dDEPTHPERCENT == type)
		{
			std::wstring val;
			READ1_DEF(length, res, this->ReadCT_String, &val);
			poVal->m_depthPercent = val;
		}
		else if (c_oserct_view3dRANGAX == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_rAngAx = pNewElem;
		}
		else if (c_oserct_view3dPERSPECTIVE == type)
		{
			unsigned int val;
			READ1_DEF(length, res, this->ReadCT_UnsignedByte, &val);
			poVal->m_perspective = val;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_PivotFmt(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_PivotFmt* poVal = static_cast<CT_PivotFmt*>(poResult);
		if (c_oserct_pivotfmtIDX == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_idx = pNewElem;
		}
		else if (c_oserct_pivotfmtSPPR == type)
		{
			poVal->m_spPr = new PPTX::Logic::SpPr;
			poVal->m_spPr->m_namespace = L"c";
			res = ReadCT_PptxElement(0, length, poVal->m_spPr.GetPointer());
		}
		else if (c_oserct_pivotfmtTXPR == type)
		{
			poVal->m_txPr = new PPTX::Logic::TxBody;
			poVal->m_txPr->m_name = L"c:txPr";
			res = ReadCT_PptxElement(0, length, poVal->m_txPr.GetPointer());
		}
		else if (c_oserct_pivotfmtMARKER == type)
		{
			CT_Marker* pNewElem = new CT_Marker;
			READ1_DEF(length, res, this->ReadCT_Marker, pNewElem);
			poVal->m_marker = pNewElem;
		}
		else if (c_oserct_pivotfmtDLBL == type)
		{
			CT_DLbl* pNewElem = new CT_DLbl;
			READ1_DEF(length, res, this->ReadCT_DLbl, pNewElem);
			poVal->m_dLbl = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_pivotFmts(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_pivotFmts* poVal = static_cast<CT_pivotFmts*>(poResult);
		if (c_oserct_pivotfmtsPIVOTFMT == type)
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
		
		if (type > 0x80)
		{
			if (!poVal->m_extLst.IsInit()) poVal->m_extLst.Init();
			return ReadExtensions(type, length, poVal->m_extLst.GetPointer());
		}
//---------------------------------------------------------------------------------------------------------
		if (c_oserct_chartTITLE == type)
		{
			CT_Title* pNewElem = new CT_Title;
			READ1_DEF(length, res, this->ReadCT_Title, pNewElem);
			poVal->m_title = pNewElem;
		}
		else if (c_oserct_chartAUTOTITLEDELETED == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_autoTitleDeleted = pNewElem;
		}
		else if (c_oserct_chartPIVOTFMTS == type)
		{
			CT_pivotFmts* pNewElem = new CT_pivotFmts;
			READ1_DEF(length, res, this->ReadCT_pivotFmts, pNewElem);
			poVal->m_pivotFmts = pNewElem;
		}
		else if (c_oserct_chartVIEW3D == type)
		{
			CT_View3D* pNewElem = new CT_View3D;
			READ1_DEF(length, res, this->ReadCT_View3D, pNewElem);
			poVal->m_view3D = pNewElem;
		}
		else if (c_oserct_chartFLOOR == type)
		{
			CT_Surface* pNewElem = new CT_Surface;
			READ1_DEF(length, res, this->ReadCT_Surface, pNewElem);
			poVal->m_floor = pNewElem;
		}
		else if (c_oserct_chartSIDEWALL == type)
		{
			CT_Surface* pNewElem = new CT_Surface;
			READ1_DEF(length, res, this->ReadCT_Surface, pNewElem);
			poVal->m_sideWall = pNewElem;
		}
		else if (c_oserct_chartBACKWALL == type)
		{
			CT_Surface* pNewElem = new CT_Surface;
			READ1_DEF(length, res, this->ReadCT_Surface, pNewElem);
			poVal->m_backWall = pNewElem;
		}
		else if (c_oserct_chartPLOTAREA == type)
		{
			CT_PlotArea* pNewElem = new CT_PlotArea;
			READ1_DEF(length, res, this->ReadCT_PlotArea, pNewElem);
			poVal->m_plotArea = pNewElem;
		}
		else if (c_oserct_chartLEGEND == type)
		{
			CT_Legend* pNewElem = new CT_Legend;
			READ1_DEF(length, res, this->ReadCT_Legend, pNewElem);
			poVal->m_legend = pNewElem;
		}
		else if (c_oserct_chartPLOTVISONLY == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_plotVisOnly = pNewElem;
		}
		else if (c_oserct_chartDISPBLANKSAS == type)
		{
			poVal->m_dispBlanksAs.Init();
			READ1_DEF(length, res, this->ReadCT_DispBlanksAs, poVal->m_dispBlanksAs.GetPointer());
		}
		else if (c_oserct_chartSHOWDLBLSOVERMAX == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_showDLblsOverMax = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Protection(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Protection* poVal = static_cast<CT_Protection*>(poResult);
		if (c_oserct_protectionCHARTOBJECT == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_chartObject = pNewElem;
		}
		else if (c_oserct_protectionDATA == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_data = pNewElem;
		}
		else if (c_oserct_protectionFORMATTING == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_formatting = pNewElem;
		}
		else if (c_oserct_protectionSELECTION == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
			poVal->m_selection = pNewElem;
		}
		else if (c_oserct_protectionUSERINTERFACE == type)
		{
			bool pNewElem;
			READ1_DEF(length, res, this->ReadCT_Boolean, &pNewElem);
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
		if (c_oserct_pivotsourceNAME == type)
		{
			std::wstring* pNewElem = new std::wstring;
			*pNewElem = m_oBufferedStream.GetString4(length);
			poVal->m_name = pNewElem;
		}
		else if (c_oserct_pivotsourceFMTID == type)
		{
			unsigned int pNewElem;
			READ1_DEF(length, res, this->ReadCT_UnsignedInt, &pNewElem);
			poVal->m_fmtId = pNewElem;
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_Style(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		CT_Style* poVal = static_cast<CT_Style*>(poResult);
		if (c_oserct_styleVAL == type)
		{
			poVal->m_val = m_oBufferedStream.GetUChar();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadAlternateContent(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		AlternateContent* poVal = static_cast<AlternateContent*>(poResult);
		if (c_oseralternatecontentCHOICE == type)
		{
			AlternateContentChoice* pNewElem = new AlternateContentChoice;
			READ1_DEF(length, res, this->ReadAlternateContentChoice, pNewElem);
			poVal->m_Choice.push_back(pNewElem);
		}
		else if (c_oseralternatecontentFALLBACK == type)
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
		if (c_oseralternatecontentchoiceSTYLE == type)
		{
			poVal->m_style = new CT_Style;
			READ1_DEF(length, res, this->ReadCT_Style, poVal->m_style);
		}
		else if (c_oseralternatecontentchoiceREQUIRES == type)
		{
			poVal->m_Requires = new std::wstring;
			*poVal->m_Requires = m_oBufferedStream.GetString4(length);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadAlternateContentFallback(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		AlternateContentFallback* poVal = static_cast<AlternateContentFallback*>(poResult);
		if (c_oseralternatecontentfallbackSTYLE == type)
		{
			poVal->m_style = new CT_Style;
			READ1_DEF(length, res, this->ReadCT_Style, poVal->m_style);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}	
	int BinaryChartReader::ReadCT_ChartColors(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::CChartColorsFile* poVal = static_cast<OOX::Spreadsheet::CChartColorsFile*>(poResult);

		if (c_oserct_chartcolorsID == type)
		{
			poVal->m_oColorStyle.m_id = m_oBufferedStream.GetULong();
		}
		else if (c_oserct_chartcolorsMETH == type)
		{
			poVal->m_oColorStyle.m_meth = m_oBufferedStream.GetString4(length);
		}
		else if (c_oserct_chartcolorsVARIATION == type)
		{
			OOX::Spreadsheet::ChartEx::CVariation *pVariation = new OOX::Spreadsheet::ChartEx::CVariation();
			
			READ1_DEF(length, res, this->ReadCT_ColorsVariation, pVariation);
			poVal->m_oColorStyle.m_arrItems.push_back(pVariation);
		}
		else if (c_oserct_chartcolorsCOLOR == type)
		{
			PPTX::Logic::UniColor *pColor = new PPTX::Logic::UniColor();

			ReadCT_PptxElement(0, length, pColor);
			poVal->m_oColorStyle.m_arrItems.push_back(pColor);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ColorsVariation(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CVariation * poVal = static_cast<OOX::Spreadsheet::ChartEx::CVariation*>(poResult);

		if (c_oserct_chartcolorsEFFECT == type)
		{
			PPTX::Logic::ColorModifier *pEffect = new PPTX::Logic::ColorModifier();

			ReadCT_PptxElement(0, length, pEffect);
			poVal->m_arrItems.push_back(pEffect);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartStyle(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::CChartStyleFile* poVal = static_cast<OOX::Spreadsheet::CChartStyleFile*>(poResult);

		if (c_oserct_chartstyleID == type)
		{
			poVal->m_oChartStyle.m_id = m_oBufferedStream.GetULong();
		}
		else if (c_oserct_chartstyleENTRY == type)
		{
			poVal->m_oChartStyle.m_arStyleEntries.push_back(new OOX::Spreadsheet::ChartEx::CStyleEntry());
			READ1_DEF(length, res, this->ReadCT_StyleEntry, poVal->m_oChartStyle.m_arStyleEntries.back());
		}
		else if (c_oserct_chartstyleMARKERLAYOUT == type)
		{
			poVal->m_oChartStyle.m_dataPointMarkerLayout = new OOX::Spreadsheet::ChartEx::CMarkerLayout();
			READ1_DEF(length, res, this->ReadCT_StyleMarkerLayout, poVal->m_oChartStyle.m_dataPointMarkerLayout.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_StyleEntry(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CStyleEntry* poVal = static_cast<OOX::Spreadsheet::ChartEx::CStyleEntry*>(poResult);

		if (c_oserct_chartstyleENTRYTYPE == type)
		{
			poVal->setTypeStyleEntry(m_oBufferedStream.GetUChar());
		}
		else if (c_oserct_chartstyleLNREF == type)
		{
			poVal->m_lnRef.m_name = L"cs:lnRef";
			res = ReadCT_PptxElement(0, length, &poVal->m_lnRef);
		}
		else if (c_oserct_chartstyleFILLREF == type)
		{
			poVal->m_fillRef.m_name = L"cs:fillRef";
			res = ReadCT_PptxElement(0, length, &poVal->m_fillRef);
		}
		else if (c_oserct_chartstyleEFFECTREF == type)
		{
			poVal->m_effectRef.m_name = L"cs:effectRef";
			res = ReadCT_PptxElement(0, length, &poVal->m_effectRef);
		}
		else if (c_oserct_chartstyleFONTREF == type)
		{
			poVal->m_fontRef.m_name = L"cs:fontRef";
			res = ReadCT_PptxElement(0, length, &poVal->m_fontRef);
		}
		else if (c_oserct_chartstyleDEFPR == type)
		{
			poVal->m_defRPr = new PPTX::Logic::RunProperties();
			poVal->m_defRPr->m_name = L"cs:defPr";
			res = ReadCT_PptxElement(0, length, poVal->m_defRPr.GetPointer());
		}
		else if (c_oserct_chartstyleBODYPR == type)
		{
			poVal->m_bodyPr = new PPTX::Logic::BodyPr();
			poVal->m_bodyPr->m_namespace = L"cs";
			res = ReadCT_PptxElement(0, length, poVal->m_bodyPr.GetPointer());
		}
		else if (c_oserct_chartstyleSPPR == type)
		{
			poVal->m_spPr = new PPTX::Logic::SpPr();
			poVal->m_spPr->m_namespace = L"cs";
			res = ReadCT_PptxElement(0, length, poVal->m_spPr.GetPointer());
		}
		else if (c_oserct_chartstyleLINEWIDTH == type)
		{
			poVal->m_lineWidthScale = m_oBufferedStream.GetULong();
		}
		else
			res = c_oSerConstants::ReadUnknown;
			return res;
	}
	int BinaryChartReader::ReadCT_StyleMarkerLayout(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CMarkerLayout* poVal = static_cast<OOX::Spreadsheet::ChartEx::CMarkerLayout*>(poResult);

		if (c_oserct_chartstyleMARKERSYMBOL == type)
		{
			poVal->m_symbol = (SimpleTypes::Spreadsheet::EChartSymbol)m_oBufferedStream.GetUChar();
		}
		else if (c_oserct_chartstyleMARKERSIZE == type)
		{
			poVal->m_size = m_oBufferedStream.GetULong();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExFile(long length, OOX::Spreadsheet::CChartExFile* pChartExFile)
	{
		int res = c_oSerConstants::ReadOk;
		READ1_DEF(length, res, this->ReadCT_ChartExFileContent, pChartExFile);
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExFileContent(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::CChartExFile *pChart = static_cast<OOX::Spreadsheet::CChartExFile*>(poResult);

		if (c_oserct_chartExSpaceCHART == type)
		{
			READ1_DEF(length, res, this->ReadCT_ChartExChart, &pChart->m_oChartSpace.m_chart);
		}
		else if (c_oserct_chartExSpaceCHARTDATA == type)
		{
			READ1_DEF(length, res, this->ReadCT_ChartExChartData, &pChart->m_oChartSpace.m_chartData);
		}
		else if (c_oserct_chartExSpaceCLRMAPOVR == type)
		{
			BYTE typeRec1 = m_oBufferedStream.GetUChar();

			pChart->m_oChartSpace.m_oClrMapOvr = new PPTX::Logic::ClrMap();

			pChart->m_oChartSpace.m_oClrMapOvr->m_name = L"cx:clrMapOvr";
			pChart->m_oChartSpace.m_oClrMapOvr->fromPPTY(&m_oBufferedStream);
		}
		else if (c_oserct_chartExSpaceSPPR == type)
		{
			pChart->m_oChartSpace.m_spPr = new PPTX::Logic::SpPr;
			pChart->m_oChartSpace.m_spPr->m_namespace = L"cx";
			res = ReadCT_PptxElement(0, length, pChart->m_oChartSpace.m_spPr.GetPointer());
		}
		else if (c_oserct_chartExSpaceTXPR == type)
		{
			pChart->m_oChartSpace.m_txPr = new PPTX::Logic::TxBody;
			pChart->m_oChartSpace.m_txPr->m_name = L"cx:txPr";
			res = ReadCT_PptxElement(0, length, pChart->m_oChartSpace.m_txPr.GetPointer());
		}
		else if (c_oserct_chartspaceXLSXEXTERNAL == type)
		{
			OOX::CPath path = m_oBufferedStream.GetString4(length);
			
			unsigned int rId = 0;
			m_pOfficeDrawingConverter->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/oleObject", path.GetPath(), L"External", &rId);
			m_pOfficeDrawingConverter->m_pImageManager->m_pContentTypes->AddDefault(path.GetExtention());

			if (false == pChart->m_oChartSpace.m_chartData.m_externalData.IsInit())
				pChart->m_oChartSpace.m_chartData.m_externalData.Init();

			pChart->m_oChartSpace.m_chartData.m_externalData->m_id = OOX::RId(rId).ToString();
		}
		else if (c_oserct_chartspaceXLSX == type || 
				c_oserct_chartspaceXLSXZIP == type)
		{
			OOX::CSystemUtility::CreateDirectories(m_oSaveParams.sEmbeddingsPath);

			OOX::CPath pathEmbeddingRelsDir = m_oSaveParams.sEmbeddingsPath + FILE_SEPARATOR_STR + _T("_rels");
			OOX::CSystemUtility::CreateDirectories(pathEmbeddingRelsDir.GetPath());

			m_pOfficeDrawingConverter->SetDstContentRels();

			NSCommon::smart_ptr<OOX::Media> pXlsxFile;

			if (c_oserct_chartspaceXLSXZIP == type)
			{
				ReadCT_XlsxZip(m_oBufferedStream.GetPointer(0), length, pXlsxFile);
			}
			else
			{
				ReadCT_XlsxBin(m_oBufferedStream.GetPointer(0), length, pXlsxFile);
			}

			m_oBufferedStream.Skip(length);

			if (pXlsxFile.IsInit())
			{
				OOX::CPath pathDrawingsRels = pathEmbeddingRelsDir.GetPath() + FILE_SEPARATOR_STR + pXlsxFile->m_sOutputFilename + _T(".rels");
				m_pOfficeDrawingConverter->SaveDstContentRels(pathDrawingsRels.GetPath());

				NSCommon::smart_ptr<OOX::File> pFile = pXlsxFile.smart_dynamic_cast<OOX::File>();
				pChart->AddNoWrite(pFile, L"../embeddings");

				unsigned int rId = 0;
				m_pOfficeDrawingConverter->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/package", L"../embeddings/" + pXlsxFile->m_sOutputFilename, std::wstring(), &rId);
				m_pOfficeDrawingConverter->m_pImageManager->m_pContentTypes->AddDefault(pXlsxFile->filename().GetExtention());

				if (false == pChart->m_oChartSpace.m_chartData.m_externalData.IsInit())
					pChart->m_oChartSpace.m_chartData.m_externalData.Init();

				pChart->m_oChartSpace.m_chartData.m_externalData->m_id = OOX::RId(rId).ToString();
			}
		}
		else if (c_oserct_chartspaceSTYLES == type)
		{
			NSCommon::smart_ptr<OOX::Spreadsheet::CChartStyleFile> chartstyle = new OOX::Spreadsheet::CChartStyleFile(NULL);
			READ1_DEF(length, res, ReadCT_ChartStyle, chartstyle.GetPointer());

			if (res == c_oSerConstants::ReadOk)
			{
				NSCommon::smart_ptr<OOX::File> pFile = chartstyle.smart_dynamic_cast<OOX::File>();
				pChart->Add(pFile);

				unsigned int rId = 0;
				m_pOfficeDrawingConverter->WriteRels(L"http://schemas.microsoft.com/office/2011/relationships/chartStyle", chartstyle->m_sOutputFilename, std::wstring(), &rId);
			}
		}
		else if (c_oserct_chartspaceCOLORS == type)
		{
			NSCommon::smart_ptr<OOX::Spreadsheet::CChartColorsFile> chartcolors = new OOX::Spreadsheet::CChartColorsFile(NULL);
			READ1_DEF(length, res, ReadCT_ChartColors, chartcolors.GetPointer());

			if (res == c_oSerConstants::ReadOk)
			{
				NSCommon::smart_ptr<OOX::File> pFile = chartcolors.smart_dynamic_cast<OOX::File>();
				pChart->Add(pFile);

				unsigned int rId = 0;
				m_pOfficeDrawingConverter->WriteRels(L"http://schemas.microsoft.com/office/2011/relationships/chartColorStyle", chartcolors->m_sOutputFilename, std::wstring(), &rId);
			}
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExChart(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CChart *pChart = static_cast<OOX::Spreadsheet::ChartEx::CChart*>(poResult);

		if (c_oserct_chartExChartPLOTAREA == type)
		{
			READ1_DEF(length, res, this->ReadCT_ChartExPlotArea, &pChart->m_plotArea);
		}
		else if (c_oserct_chartExChartTITLE == type)
		{
			pChart->m_title.Init();
			READ1_DEF(length, res, this->ReadCT_ChartExTitle, pChart->m_title.GetPointer());
		}
		else if (c_oserct_chartExChartLEGEND == type)
		{
			pChart->m_legend.Init();
			READ1_DEF(length, res, this->ReadCT_ChartExLegend, pChart->m_legend.GetPointer());
		}

		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExChartData(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CChartData *pChartData = static_cast<OOX::Spreadsheet::ChartEx::CChartData*>(poResult);

		if (c_oserct_chartExDATA == type)
		{
			pChartData->m_arData.push_back(new OOX::Spreadsheet::ChartEx::CData());
			READ1_DEF(length, res, this->ReadCT_ChartExData, pChartData->m_arData.back());
		}
		else if (c_oserct_chartExEXTERNALDATA == type)
		{
			if (false == pChartData->m_externalData.IsInit())
				pChartData->m_externalData.Init();

			READ1_DEF(length, res, this->ReadCT_ChartExExternalData, pChartData->m_externalData.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExData(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CData *pData = static_cast<OOX::Spreadsheet::ChartEx::CData*>(poResult);

		if (c_oserct_chartExDataID == type)
		{
			pData->m_id = m_oBufferedStream.GetULong();
		}
		else if (c_oserct_chartExDataSTRDIMENSION == type)
		{
			OOX::Spreadsheet::ChartEx::CStrDimension *pDimension = new OOX::Spreadsheet::ChartEx::CStrDimension();
			READ1_DEF(length, res, this->ReadCT_ChartExDataStrDimension, pDimension);

			pData->m_arDimension.push_back(dynamic_cast<OOX::Spreadsheet::ChartEx::CDimension*>(pDimension));
		}
		else if (c_oserct_chartExDataNUMDIMENSION == type)
		{
			OOX::Spreadsheet::ChartEx::CNumDimension *pDimension = new OOX::Spreadsheet::ChartEx::CNumDimension();
			READ1_DEF(length, res, this->ReadCT_ChartExDataNumDimension, pDimension);
			
			pData->m_arDimension.push_back(dynamic_cast<OOX::Spreadsheet::ChartEx::CDimension*>(pDimension));
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExDataStrDimension(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CStrDimension *pDimension = static_cast<OOX::Spreadsheet::ChartEx::CStrDimension*>(poResult);

		if (c_oserct_chartExDataDimensionTYPE == type)
		{
			pDimension->m_type.Init();
			pDimension->m_type->SetValue((SimpleTypes::Spreadsheet::EDimensionType)m_oBufferedStream.GetUChar());
		}
		else if (c_oserct_chartExDataDimensionFORMULA == type)
		{
			pDimension->m_f.Init();
			READ1_DEF(length, res, this->ReadCT_ChartExFormula, pDimension->m_f.GetPointer());
		}
		else if (c_oserct_chartExDataDimensionNF == type)
		{
			pDimension->m_nf = m_oBufferedStream.GetString4(length);
		}
		else if (c_oserct_chartExDataDimensionSTRINGLEVEL == type)
		{
			pDimension->m_levelData.push_back(new OOX::Spreadsheet::ChartEx::CStringLevel());
			READ1_DEF(length, res, this->ReadCT_ChartExStringLevel, pDimension->m_levelData.back());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExDataNumDimension(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CNumDimension *pDimension = static_cast<OOX::Spreadsheet::ChartEx::CNumDimension*>(poResult);

		if (c_oserct_chartExDataDimensionTYPE == type)
		{
			pDimension->m_type.Init();
			pDimension->m_type->SetValue((SimpleTypes::Spreadsheet::EDimensionType)m_oBufferedStream.GetUChar());
		}
		else if (c_oserct_chartExDataDimensionFORMULA == type)
		{
			pDimension->m_f.Init();
			READ1_DEF(length, res, this->ReadCT_ChartExFormula, pDimension->m_f.GetPointer());
		}
		else if (c_oserct_chartExDataDimensionNF == type)
		{
			pDimension->m_nf = m_oBufferedStream.GetString4(length);
		}
		else if (c_oserct_chartExDataDimensionNUMERICLEVEL == type)
		{
			pDimension->m_levelData.push_back(new OOX::Spreadsheet::ChartEx::CNumericLevel());
			READ1_DEF(length, res, this->ReadCT_ChartExNumericLevel, pDimension->m_levelData.back());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExExternalData(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CExternalData *pChartData = static_cast<OOX::Spreadsheet::ChartEx::CExternalData*>(poResult);

		if (c_oserct_chartExExternalAUTOUPDATE == type)
		{
			pChartData->m_autoUpdate = m_oBufferedStream.GetBool();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExFormula(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CFormula*pFormula = static_cast<OOX::Spreadsheet::ChartEx::CFormula*>(poResult);

		if (c_oserct_chartExFormulaCONTENT == type)
		{
			pFormula->m_content = m_oBufferedStream.GetString4(length);
		}
		else if (c_oserct_chartExFormulaDIRECTION == type)
		{
			pFormula->m_dir.Init();
			pFormula->m_dir->SetValue((SimpleTypes::Spreadsheet::EFormulaDirection)m_oBufferedStream.GetUChar());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExStringLevel(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CStringLevel *pLevel = static_cast<OOX::Spreadsheet::ChartEx::CStringLevel*>(poResult);

		if (c_oserct_chartExDataLevelNAME == type)
		{
			pLevel->m_name = m_oBufferedStream.GetString4(length);
		}
		else if (c_oserct_chartExDataLevelCOUNT == type)
		{
			pLevel->m_ptCount = m_oBufferedStream.GetLong();
		}
		else if (c_oserct_chartExDataLevelPT == type)
		{
			pLevel->m_arPt.push_back(new OOX::Spreadsheet::ChartEx::CStringValue());
			READ1_DEF(length, res, this->ReadCT_ChartExStringValue, pLevel->m_arPt.back());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExNumericLevel(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CNumericLevel *pLevel = static_cast<OOX::Spreadsheet::ChartEx::CNumericLevel*>(poResult);

		if (c_oserct_chartExDataLevelNAME == type)
		{
			pLevel->m_name = m_oBufferedStream.GetString4(length);
		}
		else if (c_oserct_chartExDataLevelCOUNT == type)
		{
			pLevel->m_ptCount = m_oBufferedStream.GetLong();
		}
		else if (c_oserct_chartExDataLevelPT == type)
		{
			pLevel->m_arPt.push_back(new OOX::Spreadsheet::ChartEx::CNumericValue());
			READ1_DEF(length, res, this->ReadCT_ChartExNumericValue, pLevel->m_arPt.back());
		}
		else if (c_oserct_chartExDataLevelFORMATCODE == type)
		{
			pLevel->m_formatCode = m_oBufferedStream.GetString4(length);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExStringValue(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CStringValue *pValue = static_cast<OOX::Spreadsheet::ChartEx::CStringValue*>(poResult);

		if (c_oserct_chartExDataValueIDX == type)
		{
			pValue->m_idx = m_oBufferedStream.GetLong();
		}
		else if (c_oserct_chartExDataValueCONTENT == type)
		{
			pValue->m_content = m_oBufferedStream.GetString4(length);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExNumericValue(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CNumericValue *pValue = static_cast<OOX::Spreadsheet::ChartEx::CNumericValue*>(poResult);

		if (c_oserct_chartExDataValueIDX == type)
		{
			pValue->m_idx = m_oBufferedStream.GetLong();
		}
		else if (c_oserct_chartExDataValueCONTENT == type)
		{
			pValue->m_content = m_oBufferedStream.GetDoubleReal();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExPlotArea(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CPlotArea *pPlotArea = static_cast<OOX::Spreadsheet::ChartEx::CPlotArea*>(poResult);

		if (c_oserct_chartExChartAREAREGION == type)
		{
			READ1_DEF(length, res, this->ReadCT_ChartExPlotAreaRegion, &pPlotArea->m_plotAreaRegion);
		}
		else if (c_oserct_chartExChartAXIS == type)
		{
			pPlotArea->m_arAxis.push_back(new OOX::Spreadsheet::ChartEx::CAxis());
			READ1_DEF(length, res, this->ReadCT_ChartExAxis, pPlotArea->m_arAxis.back());
		}
		else if (c_oserct_chartExChartSPPR == type)
		{
			pPlotArea->m_spPr = new PPTX::Logic::SpPr;
			pPlotArea->m_spPr->m_namespace = L"cx";
			res = ReadCT_PptxElement(0, length, pPlotArea->m_spPr.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExPlotAreaRegion(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CPlotAreaRegion *pPlotAreaRegion = static_cast<OOX::Spreadsheet::ChartEx::CPlotAreaRegion*>(poResult);

		if (c_oserct_chartExAreaPLOTSURFACE == type)
		{
			pPlotAreaRegion->m_plotSurface.Init();
			READ1_DEF(length, res, this->ReadCT_ChartExPlotSurface, pPlotAreaRegion->m_plotSurface.GetPointer());
		}
		else if (c_oserct_chartExAreaSERIES == type)
		{
			pPlotAreaRegion->m_arSeries.push_back(new OOX::Spreadsheet::ChartEx::CSeries());
			READ1_DEF(length, res, this->ReadCT_ChartExSeries, pPlotAreaRegion->m_arSeries.back());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExTitle(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CTitle *pTitle = static_cast<OOX::Spreadsheet::ChartEx::CTitle*>(poResult);

		if (c_oserct_chartExTitleTX == type)
		{
			pTitle->m_tx.Init();
			READ1_DEF(length, res, this->ReadCT_ChartExText, pTitle->m_tx.GetPointer());
		}
		else if (c_oserct_chartExTitleTXPR == type)
		{
			pTitle->m_txPr = new PPTX::Logic::TxBody;
			pTitle->m_txPr->m_name = L"cx:txPr";
			res = ReadCT_PptxElement(0, length, pTitle->m_txPr.GetPointer());
		}
		else if (c_oserct_chartExTitleSPPR == type)
		{
			pTitle->m_spPr = new PPTX::Logic::SpPr;
			pTitle->m_spPr->m_namespace = L"cx";
			res = ReadCT_PptxElement(0, length, pTitle->m_spPr.GetPointer());
		}
		else if (c_oserct_chartExTitlePOS == type)
		{
			pTitle->m_pos = (SimpleTypes::Spreadsheet::ESidePos)m_oBufferedStream.GetUChar();
		}
		else if (c_oserct_chartExTitleALIGN == type)
		{
			pTitle->m_align = (SimpleTypes::Spreadsheet::EPosAlign)m_oBufferedStream.GetUChar();
		}
		else if (c_oserct_chartExTitleOVERLAY == type)
		{
			pTitle->m_overlay = m_oBufferedStream.GetBool();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExLegend(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CLegend *pLegend = static_cast<OOX::Spreadsheet::ChartEx::CLegend*>(poResult);

		if (c_oserct_chartExLegendTXPR == type)
		{
			pLegend->m_txPr = new PPTX::Logic::TxBody;
			pLegend->m_txPr->m_name = L"cx:txPr";
			res = ReadCT_PptxElement(0, length, pLegend->m_txPr.GetPointer());
		}
		else if (c_oserct_chartExLegendSPPR == type)
		{
			pLegend->m_spPr = new PPTX::Logic::SpPr;
			pLegend->m_spPr->m_namespace = L"cx";
			res = ReadCT_PptxElement(0, length, pLegend->m_spPr.GetPointer());
		}
		else if (c_oserct_chartExLegendPOS == type)
		{
			pLegend->m_pos = (SimpleTypes::Spreadsheet::ESidePos)m_oBufferedStream.GetUChar();
		}
		else if (c_oserct_chartExLegendALIGN == type)
		{
			pLegend->m_align = (SimpleTypes::Spreadsheet::EPosAlign)m_oBufferedStream.GetUChar();
		}
		else if (c_oserct_chartExLegendOVERLAY == type)
		{
			pLegend->m_overlay = m_oBufferedStream.GetBool();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExText(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CText *pText = static_cast<OOX::Spreadsheet::ChartEx::CText*>(poResult);

		if (c_oserct_chartExTextRICH == type)
		{
			pText->m_oRich.Init();
			pText->m_oRich->m_name = L"cx:rich";
			res = ReadCT_PptxElement(0, length, pText->m_oRich.GetPointer());
		}
		else if (c_oserct_chartExTextDATA == type)
		{
			pText->m_txData.Init();
			READ1_DEF(length, res, this->ReadCT_ChartExTextData, pText->m_txData.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExTextData(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CTextData *pTextData = static_cast<OOX::Spreadsheet::ChartEx::CTextData*>(poResult);

		if (c_oserct_chartExTextDataFORMULA == type)
		{
			pTextData->m_oF.Init();
			READ1_DEF(length, res, this->ReadCT_ChartExFormula, pTextData->m_oF.GetPointer());
		}
		else if (c_oserct_chartExTextDataVALUE == type)
		{
			pTextData->m_oV = m_oBufferedStream.GetString4(length);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExPlotSurface(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CPlotSurface *pPlotSurface = static_cast<OOX::Spreadsheet::ChartEx::CPlotSurface*>(poResult);

		if (c_oserct_chartExPlotSurfaceSPPR == type)
		{
			pPlotSurface->m_spPr = new PPTX::Logic::SpPr;
			pPlotSurface->m_spPr->m_namespace = L"cx";
			res = ReadCT_PptxElement(0, length, pPlotSurface->m_spPr.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExSeries(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CSeries *pSeries = static_cast<OOX::Spreadsheet::ChartEx::CSeries*>(poResult);

		if (c_oserct_chartExSeriesDATAPT == type)
		{
			pSeries->m_arDataPt.push_back(new OOX::Spreadsheet::ChartEx::CDataPoint());
			READ1_DEF(length, res, this->ReadCT_ChartExDataPoint, pSeries->m_arDataPt.back());
		}
		else if (c_oserct_chartExSeriesDATALABELS == type)
		{
			pSeries->m_dataLabels.Init();
			READ1_DEF(length, res, this->ReadCT_ChartExDataLabels, pSeries->m_dataLabels.GetPointer());
		}
		else if (c_oserct_chartExSeriesLAYOUTPROPS == type)
		{
			pSeries->m_layoutPr.Init();
			READ1_DEF(length, res, this->ReadCT_ChartExSeriesLayoutProperties, pSeries->m_layoutPr.GetPointer());
		}
		else if (c_oserct_chartExSeriesTEXT == type)
		{
			pSeries->m_tx.Init();
			READ1_DEF(length, res, this->ReadCT_ChartExText, pSeries->m_tx.GetPointer());
		}
		else if (c_oserct_chartExSeriesAXIS == type)
		{
			pSeries->m_arAxisId.push_back(m_oBufferedStream.GetULong());
		}
		else if (c_oserct_chartExSeriesDATAID == type)
		{
			pSeries->m_dataId.Init();
			pSeries->m_dataId->m_oVal = m_oBufferedStream.GetLong();
		}
		else if (c_oserct_chartExSeriesSPPR == type)
		{
			pSeries->m_spPr = new PPTX::Logic::SpPr;
			pSeries->m_spPr->m_namespace = L"cx";
			res = ReadCT_PptxElement(0, length, pSeries->m_spPr.GetPointer());
		}
		else if (c_oserct_chartExSeriesLAYOUTID == type)
		{
			pSeries->m_oLayoutId = (SimpleTypes::Spreadsheet::ESeriesLayout)m_oBufferedStream.GetUChar();
		}
		else if (c_oserct_chartExSeriesHIDDEN == type)
		{
			pSeries->m_bHidden = m_oBufferedStream.GetBool();
		}
		else if (c_oserct_chartExSeriesOWNERIDX == type)
		{
			pSeries->m_nOwnerIdx = m_oBufferedStream.GetLong();
		}
		else if (c_oserct_chartExSeriesFORMATIDX == type)
		{
			pSeries->m_nFormatIdx = m_oBufferedStream.GetLong();
		}
		else if (c_oserct_chartExSeriesUNIQUEID == type)
		{
			pSeries->m_sUniqueId = m_oBufferedStream.GetString4(length);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExDataPoint(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CDataPoint *pDataPoint = static_cast<OOX::Spreadsheet::ChartEx::CDataPoint*>(poResult);

		if (c_oserct_chartExDataPointIDX == type)
		{
			pDataPoint->m_idx = m_oBufferedStream.GetLong();
		}
		else if (c_oserct_chartExDataPointSPPR == type)
		{
			pDataPoint->m_spPr = new PPTX::Logic::SpPr;
			pDataPoint->m_spPr->m_namespace = L"cx";
			res = ReadCT_PptxElement(0, length, pDataPoint->m_spPr.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExDataLabels(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CDataLabels *pDataLabels = static_cast<OOX::Spreadsheet::ChartEx::CDataLabels*>(poResult);
		
		if (c_oserct_chartExDataLabelsPOS == type)
		{
			pDataLabels->m_pos = (SimpleTypes::Spreadsheet::EDataLabelPos)m_oBufferedStream.GetUChar();
		}
		else if (c_oserct_chartExDataLabelsNUMFMT == type)
		{
			pDataLabels->m_numFmt.Init();
			READ1_DEF(length, res, this->ReadCT_ChartExNumberFormat, pDataLabels->m_numFmt.GetPointer());
		}
		else  if (c_oserct_chartExDataLabelsTXPR == type)
		{
			pDataLabels->m_txPr = new PPTX::Logic::TxBody;
			pDataLabels->m_txPr->m_name = L"cx:txPr";
			res = ReadCT_PptxElement(0, length, pDataLabels->m_txPr.GetPointer());
		}
		else if (c_oserct_chartExDataLabelsSPPR == type)
		{
			pDataLabels->m_spPr = new PPTX::Logic::SpPr;
			pDataLabels->m_spPr->m_namespace = L"cx";
			res = ReadCT_PptxElement(0, length, pDataLabels->m_spPr.GetPointer());
		}
		else if (c_oserct_chartExDataLabelsVISABILITIES == type)
		{
			pDataLabels->m_visibility.Init();
			READ1_DEF(length, res, this->ReadCT_ChartExDataLabelVisibilities, pDataLabels->m_visibility.GetPointer());
		}
		else if (c_oserct_chartExDataLabelsSEPARATOR == type)
		{
			pDataLabels->m_separator = m_oBufferedStream.GetString4(length);
		}
		else if (c_oserct_chartExDataLabelsDATALABEL == type)
		{
			pDataLabels->m_arDataLabel.push_back(new OOX::Spreadsheet::ChartEx::CDataLabel());
			READ1_DEF(length, res, this->ReadCT_ChartExDataLabel, pDataLabels->m_arDataLabel.back());
		}
		else if (c_oserct_chartExDataLabelsDATALABELHIDDEN == type)
		{
			pDataLabels->m_arDataLabelHidden.push_back(new OOX::Spreadsheet::ChartEx::CDataLabelHidden());
			READ1_DEF(length, res, this->ReadCT_ChartExDataLabelHidden, pDataLabels->m_arDataLabelHidden.back());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExDataLabel(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CDataLabel *pDataLabel = static_cast<OOX::Spreadsheet::ChartEx::CDataLabel*>(poResult);

		if (c_oserct_chartExDataLabelIDX == type)
		{
			pDataLabel->m_idx = m_oBufferedStream.GetLong();
		}
		else if (c_oserct_chartExDataLabelPOS == type)
		{
			pDataLabel->m_dataLabelPos = (SimpleTypes::Spreadsheet::EDataLabelPos)m_oBufferedStream.GetUChar();
		}
		else if (c_oserct_chartExDataLabelNUMFMT == type)
		{
			pDataLabel->m_numFmt.Init();
			READ1_DEF(length, res, this->ReadCT_ChartExNumberFormat, pDataLabel->m_numFmt.GetPointer());
		}
		else  if (c_oserct_chartExDataLabelTXPR == type)
		{
			pDataLabel->m_txPr = new PPTX::Logic::TxBody;
			pDataLabel->m_txPr->m_name = L"cx:txPr";
			res = ReadCT_PptxElement(0, length, pDataLabel->m_txPr.GetPointer());
		}
		else if (c_oserct_chartExDataLabelSPPR == type)
		{
			pDataLabel->m_spPr = new PPTX::Logic::SpPr;
			pDataLabel->m_spPr->m_namespace = L"cx";
			res = ReadCT_PptxElement(0, length, pDataLabel->m_spPr.GetPointer());
		}
		else if (c_oserct_chartExDataLabelVISABILITIES == type)
		{
			pDataLabel->m_visibility.Init();
			READ1_DEF(length, res, this->ReadCT_ChartExDataLabelVisibilities, pDataLabel->m_visibility.GetPointer());
		}
		else if (c_oserct_chartExDataLabelSEPARATOR == type)
		{
			pDataLabel->m_separator = m_oBufferedStream.GetString4(length);
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExDataLabelHidden(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CDataLabelHidden *pDataLabelHidden = static_cast<OOX::Spreadsheet::ChartEx::CDataLabelHidden*>(poResult);

		if (c_oserct_chartExDataLabelHiddenIDX == type)
		{
			pDataLabelHidden->m_idx = m_oBufferedStream.GetLong();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExSeriesLayoutProperties(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CSeriesLayoutProperties *pLayout = static_cast<OOX::Spreadsheet::ChartEx::CSeriesLayoutProperties*>(poResult);

		if (c_oserct_chartExSeriesLayoutPARENT == type)
		{
			pLayout->m_parentLabelLayout.Init();
			pLayout->m_parentLabelLayout->m_oVal = (SimpleTypes::Spreadsheet::EParentLabelLayout)m_oBufferedStream.GetUChar();
		}
		else if (c_oserct_chartExSeriesLayoutREGION == type)
		{
			pLayout->m_regionLabelLayout.Init();
			pLayout->m_regionLabelLayout->m_oVal = (SimpleTypes::Spreadsheet::ERegionLabelLayout)m_oBufferedStream.GetUChar();
		}
		else if (c_oserct_chartExSeriesLayoutVISABILITIES == type)
		{
			pLayout->m_visibility.Init();
			READ1_DEF(length, res, this->ReadCT_ChartExSeriesElementVisibilities, pLayout->m_visibility.GetPointer());
		}
		else if (c_oserct_chartExSeriesLayoutAGGREGATION == type)
		{
			pLayout->m_aggregation = m_oBufferedStream.GetBool();
		}
		else if (c_oserct_chartExSeriesLayoutBINNING == type)
		{
			pLayout->m_binning.Init();
			READ1_DEF(length, res, this->ReadCT_ChartExBinning, pLayout->m_binning.GetPointer());
		}
		else if (c_oserct_chartExSeriesLayoutSTATISTIC == type)
		{
			pLayout->m_statistics.Init();
			READ1_DEF(length, res, this->ReadCT_ChartExStatistics, pLayout->m_statistics.GetPointer());
		}
		else if (c_oserct_chartExSeriesLayoutSUBTOTALS == type)
		{
			pLayout->m_subtotals.Init();
			READ1_DEF(length, res, this->ReadCT_ChartExSubtotals, pLayout->m_subtotals.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExNumberFormat(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CNumberFormat *pNumberFormat = static_cast<OOX::Spreadsheet::ChartEx::CNumberFormat*>(poResult);

		if (c_oserct_chartExNumberFormatFORMATCODE == type)
		{
			pNumberFormat->m_formatCode = m_oBufferedStream.GetString4(length);
		}
		else if (c_oserct_chartExNumberFormatSOURCELINKED == type)
		{
			pNumberFormat->m_sourceLinked = m_oBufferedStream.GetBool();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExDataLabelVisibilities(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CDataLabelVisibilities *pVisibilities = static_cast<OOX::Spreadsheet::ChartEx::CDataLabelVisibilities*>(poResult);

		if (c_oserct_chartExDataLabelVisibilitiesSERIES == type)
		{
			pVisibilities->m_seriesName = m_oBufferedStream.GetBool();
		}
		else if (c_oserct_chartExDataLabelVisibilitiesCATEGORY == type)
		{
			pVisibilities->m_categoryName = m_oBufferedStream.GetBool();
		}
		else if (c_oserct_chartExDataLabelVisibilitiesVALUE == type)
		{
			pVisibilities->m_value = m_oBufferedStream.GetBool();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExBinning(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CBinning *pBinning = static_cast<OOX::Spreadsheet::ChartEx::CBinning*>(poResult);

		if (c_oserct_chartExBinningBINSIZE == type)
		{
			pBinning->m_binSize.Init();
			pBinning->m_binSize->m_oVal = m_oBufferedStream.GetDoubleReal();
		}
		else if (c_oserct_chartExBinningBINCOUNT == type)
		{
			pBinning->m_binCount.Init();
			pBinning->m_binCount->m_oVal = m_oBufferedStream.GetLong();
		}
		else if (c_oserct_chartExBinningINTERVAL == type)
		{
			pBinning->m_intervalClosed = (SimpleTypes::Spreadsheet::ESidePos)m_oBufferedStream.GetUChar();
		}
		else if (c_oserct_chartExBinningUNDERVAL == type)
		{
			pBinning->m_underflow = m_oBufferedStream.GetDoubleReal();
		}
		else if (c_oserct_chartExBinningOVERVAL == type)
		{
			pBinning->m_overflow = m_oBufferedStream.GetDoubleReal();
		}
		else if (c_oserct_chartExBinningUNDERAUTO == type)
		{
			pBinning->m_underflow = (SimpleTypes::Spreadsheet::EDoubleOrAutomatic)m_oBufferedStream.GetUChar();
		}
		else if (c_oserct_chartExBinningOVERAUTO == type)
		{
			pBinning->m_overflow = (SimpleTypes::Spreadsheet::EDoubleOrAutomatic)m_oBufferedStream.GetUChar();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExStatistics(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CStatistics *pStatistics = static_cast<OOX::Spreadsheet::ChartEx::CStatistics*>(poResult);

		if (c_oserct_chartExStatisticsMETHOD == type)
		{
			pStatistics->m_quartileMethod = (SimpleTypes::Spreadsheet::EQuartileMethod)m_oBufferedStream.GetUChar();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExSubtotals(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CSubtotals *pSubtotals = static_cast<OOX::Spreadsheet::ChartEx::CSubtotals*>(poResult);

		if (c_oserct_chartExStatisticsMETHOD == type)
		{
			pSubtotals->m_arIdx.push_back(m_oBufferedStream.GetULong());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExSeriesElementVisibilities(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CSeriesElementVisibilities *pVisibilities = static_cast<OOX::Spreadsheet::ChartEx::CSeriesElementVisibilities*>(poResult);

		if (c_oserct_chartExSeriesVisibilitiesCONNECTOR == type)
		{
			pVisibilities->m_connectorLines = m_oBufferedStream.GetBool();
		}
		else if (c_oserct_chartExSeriesVisibilitiesMEANLINE == type)
		{
			pVisibilities->m_meanLine = m_oBufferedStream.GetBool();
		}
		else if (c_oserct_chartExSeriesVisibilitiesMEANMARKER == type)
		{
			pVisibilities->m_meanMarker = m_oBufferedStream.GetBool();
		}
		else if (c_oserct_chartExSeriesVisibilitiesNONOUTLIERS == type)
		{
			pVisibilities->m_nonoutliers = m_oBufferedStream.GetBool();
		}
		else if (c_oserct_chartExSeriesVisibilitiesOUTLIERS == type)
		{
			pVisibilities->m_outliers = m_oBufferedStream.GetBool();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExAxis(BYTE type, long length, void* poResult)
	{
		OOX::Spreadsheet::ChartEx::CAxis *pAxis = static_cast<OOX::Spreadsheet::ChartEx::CAxis*>(poResult);
		int res = c_oSerConstants::ReadOk;

		if (c_oserct_chartExAxisID == type)
		{
			pAxis->m_id = m_oBufferedStream.GetLong();
		}
		else if (c_oserct_chartExAxisHIDDEN == type)
		{
			pAxis->m_hidden = m_oBufferedStream.GetBool();
		}
		else if (c_oserct_chartExAxisCATSCALING == type)
		{
			pAxis->m_catScaling.Init();
			READ1_DEF(length, res, this->ReadCT_ChartExCatScaling, pAxis->m_catScaling.GetPointer());
		}
		else if (c_oserct_chartExAxisVALSCALING == type)
		{
			pAxis->m_valScaling.Init();
			READ1_DEF(length, res, this->ReadCT_ChartExValScaling, pAxis->m_valScaling.GetPointer());
		}
		else if (c_oserct_chartExAxisTITLE == type)
		{
			pAxis->m_title.Init();
			READ1_DEF(length, res, this->ReadCT_ChartExTitle, pAxis->m_title.GetPointer());
		}
		else if (c_oserct_chartExAxisUNIT == type)
		{
			pAxis->m_units.Init();
			READ1_DEF(length, res, this->ReadCT_ChartExAxisUnit, pAxis->m_units.GetPointer());
		}
		else if (c_oserct_chartExAxisNUMFMT == type)
		{
			pAxis->m_numFmt.Init();
			READ1_DEF(length, res, this->ReadCT_ChartExNumberFormat, pAxis->m_numFmt.GetPointer());
		}
		else if (c_oserct_chartExAxisMAJORTICK == type)
		{
			pAxis->m_majorTickMarks.Init();
			pAxis->m_majorTickMarks->m_name = L"cx:majorTickMarks";
			READ1_DEF(length, res, this->ReadCT_ChartExTickMarks, pAxis->m_majorTickMarks.GetPointer());
		}
		else if (c_oserct_chartExAxisMINORTICK == type)
		{
			pAxis->m_minorTickMarks.Init();
			pAxis->m_minorTickMarks->m_name = L"cx:minorTickMarks";
			READ1_DEF(length, res, this->ReadCT_ChartExTickMarks, pAxis->m_minorTickMarks.GetPointer());
		}
		else if (c_oserct_chartExAxisMAJORGRID == type)
		{
			pAxis->m_majorGridlines.Init();
			pAxis->m_majorGridlines->m_name = L"cx:majorGridlines";
			READ1_DEF(length, res, this->ReadCT_ChartExGridlines, pAxis->m_majorGridlines.GetPointer());
		}
		else if (c_oserct_chartExAxisMINORGRID == type)
		{
			pAxis->m_minorGridlines.Init();
			pAxis->m_minorGridlines->m_name = L"cx:minorGridlines";
			READ1_DEF(length, res, this->ReadCT_ChartExGridlines, pAxis->m_minorGridlines.GetPointer());
		}
		else if (c_oserct_chartExAxisTICKLABELS == type)
		{
			pAxis->m_tickLabels = m_oBufferedStream.GetBool();
		}
		else  if (c_oserct_chartExAxisTXPR == type)
		{
			pAxis->m_txPr = new PPTX::Logic::TxBody;
			pAxis->m_txPr->m_name = L"cx:txPr";
			res = ReadCT_PptxElement(0, length, pAxis->m_txPr.GetPointer());
		}
		else if (c_oserct_chartExAxisSPPR == type)
		{
			pAxis->m_spPr = new PPTX::Logic::SpPr;
			pAxis->m_spPr->m_namespace = L"cx";
			res = ReadCT_PptxElement(0, length, pAxis->m_spPr.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExCatScaling(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CCatScaling *pScaling = static_cast<OOX::Spreadsheet::ChartEx::CCatScaling*>(poResult);

		if (c_oserct_chartExCatScalingGAPAUTO == type)
		{
			pScaling->m_gapWidth = (SimpleTypes::Spreadsheet::EDoubleOrAutomatic)m_oBufferedStream.GetUChar();
		}
		else if (c_oserct_chartExCatScalingGAPVAL == type)
		{
			pScaling->m_gapWidth = m_oBufferedStream.GetDoubleReal();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExValScaling(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CValScaling *pScaling = static_cast<OOX::Spreadsheet::ChartEx::CValScaling*>(poResult);

		if (c_oserct_chartExValScalingMINAUTO == type)
		{
			pScaling->m_min = (SimpleTypes::Spreadsheet::EDoubleOrAutomatic)m_oBufferedStream.GetUChar();
		}
		else if (c_oserct_chartExValScalingMINVAL == type)
		{
			pScaling->m_min = m_oBufferedStream.GetDoubleReal();
		}
		else if (c_oserct_chartExValScalingMAXAUTO == type)
		{
			pScaling->m_max = (SimpleTypes::Spreadsheet::EDoubleOrAutomatic)m_oBufferedStream.GetUChar();
		}
		else if (c_oserct_chartExValScalingMAXVAL == type)
		{
			pScaling->m_max = m_oBufferedStream.GetDoubleReal();
		}
		else if (c_oserct_chartExValScalingMAJUNITAUTO == type)
		{
			pScaling->m_majorUnit = (SimpleTypes::Spreadsheet::EDoubleOrAutomatic)m_oBufferedStream.GetUChar();
		}
		else if (c_oserct_chartExValScalingMAJUNITVAL == type)
		{
			pScaling->m_majorUnit = m_oBufferedStream.GetDoubleReal();
		}
		else if (c_oserct_chartExValScalingMINUNITAUTO == type)
		{
			pScaling->m_minorUnit = (SimpleTypes::Spreadsheet::EDoubleOrAutomatic)m_oBufferedStream.GetUChar();
		}
		else if (c_oserct_chartExValScalingMINUNITVAL == type)
		{
			pScaling->m_minorUnit = m_oBufferedStream.GetDoubleReal();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExAxisUnit(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CAxisUnit *pAxisUnit = static_cast<OOX::Spreadsheet::ChartEx::CAxisUnit*>(poResult);

		if (c_oserct_chartExAxisUnitTYPE == type)
		{
			pAxisUnit->m_unit = (SimpleTypes::Spreadsheet::EAxisUnit)m_oBufferedStream.GetUChar();
		}
		else if (c_oserct_chartExAxisUnitLABEL == type)
		{
			pAxisUnit->m_unitsLabel.Init();
			READ1_DEF(length, res, this->ReadCT_ChartExAxisUnitsLabel, pAxisUnit->m_unitsLabel.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExTickMarks(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CTickMarks *pTickMarks = static_cast<OOX::Spreadsheet::ChartEx::CTickMarks*>(poResult);

		if (c_oserct_chartExTickMarksTYPE == type)
		{
			pTickMarks->m_type = (SimpleTypes::Spreadsheet::ETickMarksType)m_oBufferedStream.GetUChar();
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExGridlines(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CGridlines *pGridlines = static_cast<OOX::Spreadsheet::ChartEx::CGridlines*>(poResult);
		
		if (c_oserct_chartExGridlinesSPPR == type)
		{
			pGridlines->m_spPr = new PPTX::Logic::SpPr;
			pGridlines->m_spPr->m_namespace = L"cx";
			res = ReadCT_PptxElement(0, length, pGridlines->m_spPr.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
	int BinaryChartReader::ReadCT_ChartExAxisUnitsLabel(BYTE type, long length, void* poResult)
	{
		int res = c_oSerConstants::ReadOk;
		OOX::Spreadsheet::ChartEx::CAxisUnitsLabel *pAxisUnitsLabel = static_cast<OOX::Spreadsheet::ChartEx::CAxisUnitsLabel*>(poResult);

		if (c_oserct_chartExAxisUnitsLabelSPPR == type)
		{
			pAxisUnitsLabel->m_spPr = new PPTX::Logic::SpPr;
			pAxisUnitsLabel->m_spPr->m_namespace = L"cx";
			res = ReadCT_PptxElement(0, length, pAxisUnitsLabel->m_spPr.GetPointer());
		}
		else if (c_oserct_chartExAxisUnitsLabelTXPR == type)
		{
			pAxisUnitsLabel->m_txPr = new PPTX::Logic::TxBody;
			pAxisUnitsLabel->m_txPr->m_name = L"cx:txPr";
			res = ReadCT_PptxElement(0, length, pAxisUnitsLabel->m_txPr.GetPointer());
		}
		else if (c_oserct_chartExAxisUnitsLabelTEXT == type)
		{
			pAxisUnitsLabel->m_tx.Init();
			READ1_DEF(length, res, this->ReadCT_ChartExText, pAxisUnitsLabel->m_tx.GetPointer());
		}
		else
			res = c_oSerConstants::ReadUnknown;
		return res;
	}
//---------------------------------------------------------------
	BinaryChartWriter::BinaryChartWriter(NSBinPptxRW::CBinaryFileWriter &oBufferedStream, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter):m_oBcw(oBufferedStream),m_pOfficeDrawingConverter(pOfficeDrawingConverter)
	{}

 	void BinaryChartWriter::WriteCT_ChartFile(OOX::Spreadsheet::CChartFile& oChartFile)
	{
		CT_ChartSpace& oVal = oChartFile.m_oChartSpace;
		
		if (oVal.m_date1904.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartspaceDATE1904);
			WriteCT_Boolean(*oVal.m_date1904);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_lang.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartspaceLANG);
			WriteCT_String(*oVal.m_lang);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_roundedCorners.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartspaceROUNDEDCORNERS);
			WriteCT_Boolean(*oVal.m_roundedCorners);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_AlternateContent)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartspaceALTERNATECONTENT);
			WriteAlternateContent(*oVal.m_AlternateContent);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_style)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartspaceSTYLE);
			WriteCT_Style(*oVal.m_style);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oClrMapOvr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartspaceCLRMAPOVR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oClrMapOvr);
			m_oBcw.WriteItemEnd(nCurPos);			
		}
		if (NULL != oVal.m_pivotSource)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartspacePIVOTSOURCE);
			WriteCT_PivotSource(*oVal.m_pivotSource);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_protection)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartspacePROTECTION);
			WriteCT_Protection(*oVal.m_protection);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_chart)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartspaceCHART);
			WriteCT_Chart(*oVal.m_chart);
			m_oBcw.WriteItemEnd(nCurPos);

		}
		if (oVal.m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartspaceSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_txPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartspaceTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_txPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_externalData)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartspaceEXTERNALDATA);
			WriteCT_ExternalData(*oVal.m_externalData);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_printSettings)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartspacePRINTSETTINGS);
			WriteCT_PrintSettings(*oVal.m_printSettings);
			m_oBcw.WriteItemEnd(nCurPos);
		}

		WriteExtensions(oVal.m_extLst.GetPointer());

		if ((NULL != oVal.m_userShapes) && (oVal.m_userShapes->m_id.IsInit()))
		{
			smart_ptr<OOX::File> oFile = oChartFile.Find(*oVal.m_userShapes->m_id);
			
			if (oFile.IsInit() && OOX::FileTypes::ChartDrawing == oFile->type())
			{
				OOX::CChartDrawing* pDrawing = (OOX::CChartDrawing*)oFile.GetPointer();
				
				OOX::IFileContainer* oldRels = m_pOfficeDrawingConverter->GetRelsPtr();
				m_pOfficeDrawingConverter->SetRelsPtr(pDrawing);
				
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartspaceUSERSHAPES);

				int nCurPos1 = m_oBcw.WriteItemStart(c_oserct_usershapes_COUNT);
					m_oBcw.m_oStream.WriteLONG(pDrawing->m_arrItems.size());
				m_oBcw.WriteItemEnd(nCurPos1);
				
				OOX::IFileContainer* oldRelsStream = m_oBcw.m_oStream.GetRelsPtr();
				m_oBcw.m_oStream.SetRelsPtr(pDrawing);

				for (size_t i = 0; i < pDrawing->m_arrItems.size(); i++)
				{
					OOX::CAbsSizeAnchor* absSize = dynamic_cast<OOX::CAbsSizeAnchor*>(pDrawing->m_arrItems[i]);
					OOX::CRelSizeAnchor* relSize = dynamic_cast<OOX::CRelSizeAnchor*>(pDrawing->m_arrItems[i]);
					
					int nCurPos2 = m_oBcw.WriteItemStart(relSize ? c_oserct_usershapes_SHAPE_REL : c_oserct_usershapes_SHAPE_ABS);
						WriteCT_Shape(pDrawing->m_arrItems[i]);
					m_oBcw.WriteItemEnd(nCurPos2);
				}
				
				m_oBcw.WriteItemEnd(nCurPos);
				
				m_oBcw.m_oStream.SetRelsPtr(oldRelsStream);
				m_pOfficeDrawingConverter->SetRelsPtr(oldRels);
			}
		}	
	}
	void BinaryChartWriter::WriteCT_Shape(OOX::CSizeAnchor *pVal)
	{
		OOX::CAbsSizeAnchor* absSize = dynamic_cast<OOX::CAbsSizeAnchor*>(pVal);
		OOX::CRelSizeAnchor* relSize = dynamic_cast<OOX::CRelSizeAnchor*>(pVal);

	//From
		int nCurPos = 0;
		if (pVal->m_oFrom.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSer_DrawingType::From);
				WriteCT_FromTo(*pVal->m_oFrom);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	//To
		if ((relSize) && (relSize->m_oTo.IsInit()))
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSer_DrawingType::To);
				WriteCT_FromTo(*relSize->m_oTo);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	//Ext
		if ((absSize) && (absSize->m_oExt.IsInit()))
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
		if (oFromTo.m_oX.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_DrawingPosType::X);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
			m_oBcw.m_oStream.WriteDoubleReal(oFromTo.m_oX.get());
		}
		if (oFromTo.m_oY.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_DrawingPosType::Y);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
			m_oBcw.m_oStream.WriteDoubleReal(oFromTo.m_oY.get());
		}
	}
	void BinaryChartWriter::WriteCT_Ext(OOX::CExt& oExt)
	{
		if (oExt.m_oCx.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_DrawingExtType::Cx);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
			m_oBcw.m_oStream.WriteDoubleReal(oExt.m_oCx.get());
		}
		if (oExt.m_oCy.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_DrawingExtType::Cy);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
			m_oBcw.m_oStream.WriteDoubleReal(oExt.m_oCy.get());
		}
	}
	void BinaryChartWriter::WriteCT_PageSetup(CT_PageSetup& oVal)
	{
		if (oVal.m_paperSize.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagesetupPAPERSIZE);
			m_oBcw.m_oStream.WriteLONG(*oVal.m_paperSize);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_paperHeight.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagesetupPAPERHEIGHT);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_paperHeight);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_paperWidth.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagesetupPAPERWIDTH);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_paperWidth);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_firstPageNumber.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagesetupFIRSTPAGENUMBER);
			m_oBcw.m_oStream.WriteLONG(*oVal.m_firstPageNumber);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_orientation.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagesetupORIENTATION);
			m_oBcw.m_oStream.WriteBYTE((BYTE)oVal.m_orientation->GetValue());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_blackAndWhite.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagesetupBLACKANDWHITE);
			m_oBcw.m_oStream.WriteBOOL(*oVal.m_blackAndWhite);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_draft.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagesetupDRAFT);
			m_oBcw.m_oStream.WriteBOOL(*oVal.m_draft);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_useFirstPageNumber.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagesetupUSEFIRSTPAGENUMBER);
			m_oBcw.m_oStream.WriteBOOL(*oVal.m_useFirstPageNumber);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_horizontalDpi.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagesetupHORIZONTALDPI);
			m_oBcw.m_oStream.WriteLONG(*oVal.m_horizontalDpi);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_verticalDpi.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagesetupVERTICALDPI);
			m_oBcw.m_oStream.WriteLONG(*oVal.m_verticalDpi);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_copies.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagesetupCOPIES);
			m_oBcw.m_oStream.WriteLONG(*oVal.m_copies);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_PageMargins(CT_PageMargins& oVal)
	{
		if (oVal.m_l.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagemarginsL);
			m_oBcw.m_oStream.WriteDoubleReal(*oVal.m_l);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_r.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagemarginsR);
			m_oBcw.m_oStream.WriteDoubleReal(*oVal.m_r);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_t.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagemarginsT);
			m_oBcw.m_oStream.WriteDoubleReal(*oVal.m_t);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_b.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagemarginsB);
			m_oBcw.m_oStream.WriteDoubleReal(*oVal.m_b);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_header.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagemarginsHEADER);
			m_oBcw.m_oStream.WriteDoubleReal(*oVal.m_header);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_footer.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pagemarginsFOOTER);
			m_oBcw.m_oStream.WriteDoubleReal(*oVal.m_footer);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_HeaderFooter(CT_HeaderFooter& oVal)
	{
		if (oVal.m_oddHeader.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_headerfooterODDHEADER);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_oddHeader);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oddFooter.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_headerfooterODDFOOTER);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_oddFooter);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_evenHeader.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_headerfooterEVENHEADER);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_evenHeader);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_evenFooter.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_headerfooterEVENFOOTER);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_evenFooter);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_firstHeader.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_headerfooterFIRSTHEADER);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_firstHeader);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_firstFooter.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_headerfooterFIRSTFOOTER);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_firstFooter);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_alignWithMargins.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_headerfooterALIGNWITHMARGINS);
			m_oBcw.m_oStream.WriteBOOL(*oVal.m_alignWithMargins);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_differentOddEven.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_headerfooterDIFFERENTODDEVEN);
			m_oBcw.m_oStream.WriteBOOL(*oVal.m_differentOddEven);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_differentFirst.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_headerfooterDIFFERENTFIRST);
			m_oBcw.m_oStream.WriteBOOL(*oVal.m_differentFirst);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_PrintSettings(CT_PrintSettings& oVal)
	{
		if (NULL != oVal.m_headerFooter)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_printsettingsHEADERFOOTER);
			WriteCT_HeaderFooter(*oVal.m_headerFooter);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_pageMargins)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_printsettingsPAGEMARGINS);
			WriteCT_PageMargins(*oVal.m_pageMargins);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_pageSetup)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_printsettingsPAGESETUP);
			WriteCT_PageSetup(*oVal.m_pageSetup);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ExternalData(CT_ExternalData& oVal)
	{
		if (oVal.m_autoUpdate.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_externaldataAUTOUPDATE);
			WriteCT_Boolean(*oVal.m_autoUpdate);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		//if (NULL != oVal.m_id) - выше
	}
	void BinaryChartWriter::WriteCT_DispBlanksAs(CDispBlanksAs& oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_dispblanksasVAL);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_LegendEntry(CT_LegendEntry& oVal)
	{
		if (oVal.m_idx.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_legendentryIDX);
			WriteCT_UnsignedInt(*oVal.m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_delete.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_legendentryDELETE);
			WriteCT_Boolean(*oVal.m_delete);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_txPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_legendentryTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_txPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}

	void BinaryChartWriter::WriteCT_LegendPos(CLegendPos& oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_legendposVAL);
		BYTE nVal = (BYTE)(oVal.GetValue());
		m_oBcw.m_oStream.WriteBYTE(nVal);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_Legend(CT_Legend& oVal)
	{
		if (oVal.m_legendPos.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_legendLEGENDPOS);
			WriteCT_LegendPos(*oVal.m_legendPos);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_legendEntry.size(); i < length; ++i)
		{
			CT_LegendEntry* pVal = oVal.m_legendEntry[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_legendLEGENDENTRY);
				WriteCT_LegendEntry(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (NULL != oVal.m_layout)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_legendLAYOUT);
			WriteCT_Layout(*oVal.m_layout);
			m_oBcw.WriteItemEnd(nCurPos);
		}

		if (oVal.m_overlay.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_legendOVERLAY);
			WriteCT_Boolean(*oVal.m_overlay);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_legendSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_txPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_legendTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_txPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_Layout(CT_Layout& oVal)
	{
		if (NULL != oVal.m_manualLayout)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_layoutMANUALLAYOUT);
			WriteCT_ManualLayout(*oVal.m_manualLayout);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_ManualLayout(CT_ManualLayout& oVal)
	{
		if (oVal.m_layoutTarget.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_manuallayoutLAYOUTTARGET);
			WriteCT_LayoutTarget(*oVal.m_layoutTarget);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_xMode.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_manuallayoutXMODE);
			WriteCT_LayoutMode(*oVal.m_xMode);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_yMode.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_manuallayoutYMODE);
			WriteCT_LayoutMode(*oVal.m_yMode);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_wMode.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_manuallayoutWMODE);
			WriteCT_LayoutMode(*oVal.m_wMode);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_hMode.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_manuallayoutHMODE);
			WriteCT_LayoutMode(*oVal.m_hMode);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_x.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_manuallayoutX);
			WriteCT_Double(*oVal.m_x);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_y.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_manuallayoutY);
			WriteCT_Double(*oVal.m_y);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_w.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_manuallayoutW);
			WriteCT_Double(*oVal.m_w);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_h.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_manuallayoutH);
			WriteCT_Double(*oVal.m_h);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_LayoutTarget(CLayoutTarget& oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_layouttargetVAL);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_LayoutMode(CLayoutMode& oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_layoutmodeVAL);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_Boolean(bool oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_booleanVAL);
		m_oBcw.m_oStream.WriteBOOL(oVal);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_UnsignedInt(unsigned int oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_unsignedintVAL);
		m_oBcw.m_oStream.WriteLONG(oVal);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_UnsignedByte(unsigned char oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_byteVAL);
		m_oBcw.m_oStream.WriteBYTE(oVal);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_SignedByte(char oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_byteVAL);
		m_oBcw.m_oStream.WriteSBYTE(oVal);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_Double(double oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_doubleVAL);
		m_oBcw.m_oStream.WriteDoubleReal(oVal);
		m_oBcw.WriteItemEnd(nCurPos);		
	}
	void BinaryChartWriter::WriteCT_String(std::wstring oVal)
	{	
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_stringVAL);
		m_oBcw.m_oStream.WriteStringW4(oVal);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_DTable(CT_DTable& oVal)
	{
		if (oVal.m_showHorzBorder.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dtableSHOWHORZBORDER);
			WriteCT_Boolean(*oVal.m_showHorzBorder);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_showVertBorder.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dtableSHOWVERTBORDER);
			WriteCT_Boolean(*oVal.m_showVertBorder);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_showOutline.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dtableSHOWOUTLINE);
			WriteCT_Boolean(*oVal.m_showOutline);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_showKeys.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dtableSHOWKEYS);
			WriteCT_Boolean(*oVal.m_showKeys);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dtableSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_txPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dtableTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_txPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_SerAx(CT_SerAx& oVal)
	{
		if (oVal.m_axId.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxAXID);
			WriteCT_UnsignedInt(*oVal.m_axId);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_scaling)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxSCALING);
			WriteCT_Scaling(*oVal.m_scaling);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_delete.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxDELETE);
			WriteCT_Boolean(*oVal.m_delete);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_axPos.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxAXPOS);
			WriteCT_AxPos(*oVal.m_axPos);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_majorGridlines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxMAJORGRIDLINES);
			WriteCT_ChartLines(*oVal.m_majorGridlines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_minorGridlines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxMINORGRIDLINES);
			WriteCT_ChartLines(*oVal.m_minorGridlines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_title)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxTITLE);
			WriteCT_Title(*oVal.m_title);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_numFmt)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxNUMFMT);
			WriteCT_NumFmt(*oVal.m_numFmt);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_majorTickMark.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxMAJORTICKMARK);
			WriteCT_TickMark(*oVal.m_majorTickMark);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_minorTickMark.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxMINORTICKMARK);
			WriteCT_TickMark(*oVal.m_minorTickMark);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_tickLblPos.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxTICKLBLPOS);
			WriteCT_TickLblPos(*oVal.m_tickLblPos);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_txPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_txPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_crossAx.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxCROSSAX);
			WriteCT_UnsignedInt(*oVal.m_crossAx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_crosses.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxCROSSES);
			WriteCT_Crosses(*oVal.m_crosses);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_crossesAt.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxCROSSESAT);
			WriteCT_Double(*oVal.m_crossesAt);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_tickLblSkip.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxTICKLBLSKIP);
			WriteCT_UnsignedInt(*oVal.m_tickLblSkip);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_tickMarkSkip.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_seraxTICKMARKSKIP);
			WriteCT_UnsignedInt(*oVal.m_tickMarkSkip);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_Scaling(CT_Scaling& oVal)
	{
		if (oVal.m_logBase.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scalingLOGBASE);
			WriteCT_Double(*oVal.m_logBase);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_orientation.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scalingORIENTATION);
			WriteCT_Orientation(*oVal.m_orientation);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_max.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scalingMAX);
			WriteCT_Double(*oVal.m_max);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_min.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scalingMIN);
			WriteCT_Double(*oVal.m_min);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_Orientation(COrientation& oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_orientationVAL);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_AxPos(CAxPos& oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_axposVAL);
		m_oBcw.m_oStream.WriteBYTE(oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_ChartLines(CT_ChartLines& oVal, bool bExt)
	{
		if (oVal.m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartlinesSPPR + (bExt ? 0x80 : 0));
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_Title(CT_Title& oVal)
	{
		if (NULL != oVal.m_tx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_titleTX);
			WriteCT_Tx(*oVal.m_tx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_layout)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_titleLAYOUT);
			WriteCT_Layout(*oVal.m_layout);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_overlay.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_titleOVERLAY);
			WriteCT_Boolean(*oVal.m_overlay);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_titleSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_txPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_titleTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_txPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_Tx(CT_Tx& oVal)
	{
		if (oVal.m_oRich.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_txRICH);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oRich);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_strRef)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_txSTRREF);
			WriteCT_StrRef(*oVal.m_strRef);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_StrRef(CT_StrRef& oVal)
	{
		if (oVal.m_f.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_strrefF);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_f);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_strCache)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_strrefSTRCACHE);
			WriteCT_StrData(*oVal.m_strCache);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_StrData(CT_StrData& oVal)
	{
		if (oVal.m_ptCount.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_strdataPTCOUNT);
			WriteCT_UnsignedInt(*oVal.m_ptCount);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_pt.size(); i < length; ++i)
		{
			CT_StrVal* pVal = oVal.m_pt[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_strdataPT);
				WriteCT_StrVal(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_StrVal(CT_StrVal& oVal)
	{
		if (oVal.m_v.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_strvalV);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_v);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_idx.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_strvalIDX);
			m_oBcw.m_oStream.WriteLONG(*oVal.m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_NumFmt(CT_NumFmt& oVal)
	{
		if (oVal.m_formatCode.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_numfmtFORMATCODE);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_formatCode);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_sourceLinked.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_numfmtSOURCELINKED);
			m_oBcw.m_oStream.WriteBOOL(*oVal.m_sourceLinked);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_TickMark(CTickMark& oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_tickmarkVAL);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_TickLblPos(CTickLblPos& oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_ticklblposVAL);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_Crosses(CCrosses& oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_crossesVAL);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_TimeUnit(CTimeUnit& oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_timeunitVAL);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_DateAx(CT_DateAx& oVal)
	{
		if (oVal.m_axId.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxAXID);
			WriteCT_UnsignedInt(*oVal.m_axId);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_scaling)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxSCALING);
			WriteCT_Scaling(*oVal.m_scaling);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_delete.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxDELETE);
			WriteCT_Boolean(*oVal.m_delete);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_axPos.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxAXPOS);
			WriteCT_AxPos(*oVal.m_axPos);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_majorGridlines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxMAJORGRIDLINES);
			WriteCT_ChartLines(*oVal.m_majorGridlines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_minorGridlines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxMINORGRIDLINES);
			WriteCT_ChartLines(*oVal.m_minorGridlines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_title)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxTITLE);
			WriteCT_Title(*oVal.m_title);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_numFmt)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxNUMFMT);
			WriteCT_NumFmt(*oVal.m_numFmt);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_majorTickMark.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxMAJORTICKMARK);
			WriteCT_TickMark(*oVal.m_majorTickMark);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_minorTickMark.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxMINORTICKMARK);
			WriteCT_TickMark(*oVal.m_minorTickMark);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_tickLblPos.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxTICKLBLPOS);
			WriteCT_TickLblPos(*oVal.m_tickLblPos);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_txPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_txPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_crossAx.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxCROSSAX);
			WriteCT_UnsignedInt(*oVal.m_crossAx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_crosses.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxCROSSES);
			WriteCT_Crosses(*oVal.m_crosses);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_crossesAt.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxCROSSESAT);
			WriteCT_Double(*oVal.m_crossesAt);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_auto.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxAUTO);
			WriteCT_Boolean(*oVal.m_auto);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_lblOffset.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxLBLOFFSET);
			WriteCT_String(*oVal.m_lblOffset);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_baseTimeUnit.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxBASETIMEUNIT);
			WriteCT_TimeUnit(*oVal.m_baseTimeUnit);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_majorUnit.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxMAJORUNIT);
			WriteCT_Double(*oVal.m_majorUnit);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_majorTimeUnit.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxMAJORTIMEUNIT);
			WriteCT_TimeUnit(*oVal.m_majorTimeUnit);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_minorUnit.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxMINORUNIT);
			WriteCT_Double(*oVal.m_minorUnit);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_minorTimeUnit.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dateaxMINORTIMEUNIT);
			WriteCT_TimeUnit(*oVal.m_minorTimeUnit);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_LblAlgn(CLblAlgn& oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_lblalgnVAL);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_CatAx(CT_CatAx& oVal)
	{
		if (oVal.m_axId.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxAXID);
			WriteCT_UnsignedInt(*oVal.m_axId);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_scaling)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxSCALING);
			WriteCT_Scaling(*oVal.m_scaling);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_delete.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxDELETE);
			WriteCT_Boolean(*oVal.m_delete);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_axPos.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxAXPOS);
			WriteCT_AxPos(*oVal.m_axPos);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_majorGridlines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxMAJORGRIDLINES);
			WriteCT_ChartLines(*oVal.m_majorGridlines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_minorGridlines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxMINORGRIDLINES);
			WriteCT_ChartLines(*oVal.m_minorGridlines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_title)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxTITLE);
			WriteCT_Title(*oVal.m_title);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_numFmt)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxNUMFMT);
			WriteCT_NumFmt(*oVal.m_numFmt);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_majorTickMark.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxMAJORTICKMARK);
			WriteCT_TickMark(*oVal.m_majorTickMark);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_minorTickMark.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxMINORTICKMARK);
			WriteCT_TickMark(*oVal.m_minorTickMark);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_tickLblPos.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxTICKLBLPOS);
			WriteCT_TickLblPos(*oVal.m_tickLblPos);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_txPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_txPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_crossAx.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxCROSSAX);
			WriteCT_UnsignedInt(*oVal.m_crossAx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_crosses.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxCROSSES);
			WriteCT_Crosses(*oVal.m_crosses);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_crossesAt.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxCROSSESAT);
			WriteCT_Double(*oVal.m_crossesAt);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_auto.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxAUTO);
			WriteCT_Boolean(*oVal.m_auto);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_lblAlgn.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxLBLALGN);
			WriteCT_LblAlgn(*oVal.m_lblAlgn);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_lblOffset.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxLBLOFFSET);
			WriteCT_String(*oVal.m_lblOffset);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_tickLblSkip.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxTICKLBLSKIP);
			WriteCT_UnsignedInt(*oVal.m_tickLblSkip);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_tickMarkSkip.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxTICKMARKSKIP);
			WriteCT_UnsignedInt(*oVal.m_tickMarkSkip);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_noMultiLvlLbl.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_cataxNOMULTILVLLBL);
			WriteCT_Boolean(*oVal.m_noMultiLvlLbl);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_DispUnitsLbl(CT_DispUnitsLbl& oVal)
	{
		if (NULL != oVal.m_layout)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dispunitslblLAYOUT);
			WriteCT_Layout(*oVal.m_layout);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_tx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dispunitslblTX);
			WriteCT_Tx(*oVal.m_tx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dispunitslblSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_txPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dispunitslblTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_txPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_BuiltInUnit(CBuiltInUnit& oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_builtinunitVAL);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_DispUnits(CT_DispUnits& oVal)
	{
		if (oVal.m_builtInUnit.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dispunitsBUILTINUNIT);
			WriteCT_BuiltInUnit(*oVal.m_builtInUnit);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_custUnit.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dispunitsCUSTUNIT);
			WriteCT_Double(*oVal.m_custUnit);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_dispUnitsLbl)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dispunitsDISPUNITSLBL);
			WriteCT_DispUnitsLbl(*oVal.m_dispUnitsLbl);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_CrossBetween(CCrossBetween& oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_crossbetweenVAL);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_ValAx(CT_ValAx& oVal)
	{
		if (oVal.m_axId.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxAXID);
			WriteCT_UnsignedInt(*oVal.m_axId);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_scaling)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxSCALING);
			WriteCT_Scaling(*oVal.m_scaling);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_delete.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxDELETE);
			WriteCT_Boolean(*oVal.m_delete);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_axPos.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxAXPOS);
			WriteCT_AxPos(*oVal.m_axPos);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_majorGridlines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxMAJORGRIDLINES);
			WriteCT_ChartLines(*oVal.m_majorGridlines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_minorGridlines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxMINORGRIDLINES);
			WriteCT_ChartLines(*oVal.m_minorGridlines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_title)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxTITLE);
			WriteCT_Title(*oVal.m_title);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_numFmt)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxNUMFMT);
			WriteCT_NumFmt(*oVal.m_numFmt);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_majorTickMark.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxMAJORTICKMARK);
			WriteCT_TickMark(*oVal.m_majorTickMark);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_minorTickMark.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxMINORTICKMARK);
			WriteCT_TickMark(*oVal.m_minorTickMark);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_tickLblPos.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxTICKLBLPOS);
			WriteCT_TickLblPos(*oVal.m_tickLblPos);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_txPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_txPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_crossAx.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxCROSSAX);
			WriteCT_UnsignedInt(*oVal.m_crossAx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_crosses.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxCROSSES);
			WriteCT_Crosses(*oVal.m_crosses);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_crossesAt.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxCROSSESAT);
			WriteCT_Double(*oVal.m_crossesAt);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_crossBetween.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxCROSSBETWEEN);
			WriteCT_CrossBetween(*oVal.m_crossBetween);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_majorUnit.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxMAJORUNIT);
			WriteCT_Double(*oVal.m_majorUnit);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_minorUnit.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxMINORUNIT);
			WriteCT_Double(*oVal.m_minorUnit);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_dispUnits)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_valaxDISPUNITS);
			WriteCT_DispUnits(*oVal.m_dispUnits);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_SizeRepresents(CSizeRepresents& oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_sizerepresentsVAL);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_BubbleSer(CT_BubbleSer& oVal)
	{
		if (oVal.m_idx.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubbleserIDX);
			WriteCT_UnsignedInt(*oVal.m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_order.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubbleserORDER);
			WriteCT_UnsignedInt(*oVal.m_order);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_tx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubbleserTX);
			WriteCT_SerTx(*oVal.m_tx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubbleserSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_invertIfNegative.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubbleserINVERTIFNEGATIVE);
			WriteCT_Boolean(*oVal.m_invertIfNegative);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_dPt.size(); i < length; ++i)
		{
			CT_DPt* pVal = oVal.m_dPt[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubbleserDPT);
				WriteCT_DPt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubbleserDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_trendline.size(); i < length; ++i)
		{
			CT_Trendline* pVal = oVal.m_trendline[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubbleserTRENDLINE);
				WriteCT_Trendline(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		for(size_t i = 0, length = oVal.m_errBars.size(); i < length; ++i)
		{
			CT_ErrBars* pVal = oVal.m_errBars[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubbleserERRBARS);
				WriteCT_ErrBars(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (NULL != oVal.m_xVal)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubbleserXVAL);
			WriteCT_AxDataSource(*oVal.m_xVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_yVal)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubbleserYVAL);
			WriteCT_NumDataSource(*oVal.m_yVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_bubbleSize)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubbleserBUBBLESIZE);
			WriteCT_NumDataSource(*oVal.m_bubbleSize);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_bubble3D.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubbleserBUBBLE3D);
			WriteCT_Boolean(*oVal.m_bubble3D);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_SerTx(CT_SerTx& oVal)
	{
		if (NULL != oVal.m_strRef)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_sertxSTRREF);
			WriteCT_StrRef(*oVal.m_strRef);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_v.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_sertxV);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_v);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_DPt(CT_DPt& oVal)
	{
		if (oVal.m_idx.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dptIDX);
			WriteCT_UnsignedInt(*oVal.m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_invertIfNegative.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dptINVERTIFNEGATIVE);
			WriteCT_Boolean(*oVal.m_invertIfNegative);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_marker)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dptMARKER);
			WriteCT_Marker(*oVal.m_marker);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_bubble3D.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dptBUBBLE3D);
			WriteCT_Boolean(*oVal.m_bubble3D);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_explosion.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dptEXPLOSION);
			WriteCT_UnsignedInt(*oVal.m_explosion);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dptSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_pictureOptions)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dptPICTUREOPTIONS);
			WriteCT_PictureOptions(*oVal.m_pictureOptions);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_Marker(CT_Marker& oVal)
	{
		if (oVal.m_symbol.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_markerSYMBOL);
			WriteCT_MarkerStyle(*oVal.m_symbol);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_size.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_markerSIZE);
			WriteCT_UnsignedByte(*oVal.m_size);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_markerSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_MarkerStyle(CMarkerStyle& oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_markerstyleVAL);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}

	void BinaryChartWriter::WriteCT_PictureOptions(CT_PictureOptions& oVal)
	{
		if (oVal.m_applyToFront.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pictureoptionsAPPLYTOFRONT);
			WriteCT_Boolean(*oVal.m_applyToFront);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_applyToSides.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pictureoptionsAPPLYTOSIDES);
			WriteCT_Boolean(*oVal.m_applyToSides);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_applyToEnd.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pictureoptionsAPPLYTOEND);
			WriteCT_Boolean(*oVal.m_applyToEnd);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_pictureFormat.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pictureoptionsPICTUREFORMAT);
			WriteCT_PictureFormat(*oVal.m_pictureFormat);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_pictureStackUnit.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pictureoptionsPICTURESTACKUNIT);
			WriteCT_Double(*oVal.m_pictureStackUnit);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_PictureFormat(CPictureFormat& oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_pictureformatVAL);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}

	void BinaryChartWriter::WriteCT_DLbls(CT_DLbls& oVal)
	{
		for (size_t i = 0, length = oVal.m_dLbl.size(); i < length; ++i)
		{
			CT_DLbl* pVal = oVal.m_dLbl[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblsDLBL);
				WriteCT_DLbl(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		for (size_t i = 0; i < oVal.m_Items.size(); ++i)
		{
			ItemsChoiceType3 eType = *oVal.m_ItemsElementName0[i];
			toBin(eType, oVal.m_Items[i]);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteExtensions(OOX::Drawing::COfficeArtExtensionList *pVal)
	{
		if (!pVal) return;
		
		for (size_t i = 0; i < pVal->m_arrExt.size(); ++i)
		{
			if (!pVal->m_arrExt[i]) continue;
			
			if (pVal->m_arrExt[i]->m_oChartDataLabel.IsInit())
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_dataLabel);
				WriteCT_DLbl(*pVal->m_arrExt[i]->m_oChartDataLabel);
				m_oBcw.WriteItemEnd(nCurPos);
			}
			if (pVal->m_arrExt[i]->m_oChartFiltering.IsInit())
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartFiltering);
				WriteChartFiltering(pVal->m_arrExt[i]->m_oChartFiltering.GetPointer());
				m_oBcw.WriteItemEnd(nCurPos);
			}
			if (pVal->m_arrExt[i]->m_oDataDisplayNaAsBlank.IsInit())
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartDataDisplayNaAsBlank);
					m_oBcw.m_oStream.WriteBOOL(*pVal->m_arrExt[i]->m_oDataDisplayNaAsBlank);
				m_oBcw.WriteItemEnd(nCurPos);
			}
			if (pVal->m_arrExt[i]->m_oFileKey.IsInit() || pVal->m_arrExt[i]->m_oInstanceId.IsInit())
			{
				int nCurPos1 = m_oBcw.WriteItemStart(c_oserct_chartExternalReference);
				
				if (pVal->m_arrExt[i]->m_oFileKey.IsInit())
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartDataExternalFileKey);
					m_oBcw.m_oStream.WriteStringW4(*pVal->m_arrExt[i]->m_oFileKey);
					m_oBcw.WriteItemWithLengthEnd(nCurPos);
				}
				if (pVal->m_arrExt[i]->m_oInstanceId.IsInit())
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartDataExternalInstanceId);
					m_oBcw.m_oStream.WriteStringW4(*pVal->m_arrExt[i]->m_oInstanceId);
					m_oBcw.WriteItemWithLengthEnd(nCurPos);
				}
				m_oBcw.WriteItemEnd(nCurPos1);
			}
		}
	}

	void BinaryChartWriter::toBin(ItemsChoiceType3 eType, void* pVal)
	{
		switch(eType)
		{
		case itemschoicetype3DLBLPOS:
			{
				CDLblPos* pTypeVal = static_cast<CDLblPos*>(pVal);
				if (NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblsDLBLPOS);
					WriteCT_DLblPos(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype3DELETE:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				if (NULL != pTypeVal)
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
				if (NULL != pTypeVal)
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
				if (NULL != pTypeVal)
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
				if (NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblsSEPARATOR);
					m_oBcw.m_oStream.WriteStringW4(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype3SHOWBUBBLESIZE:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				if (NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblsSHOWBUBBLESIZE);
					WriteCT_Boolean(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype3SHOWCATNAME:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				if (NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblsSHOWCATNAME);
					WriteCT_Boolean(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype3SHOWLEADERLINES:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				if (NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblsSHOWLEADERLINES);
					WriteCT_Boolean(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype3SHOWLEGENDKEY:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				if (NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblsSHOWLEGENDKEY);
					WriteCT_Boolean(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype3SHOWPERCENT:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				if (NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblsSHOWPERCENT);
					WriteCT_Boolean(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype3SHOWSERNAME:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				if (NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblsSHOWSERNAME);
					WriteCT_Boolean(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype3SHOWVAL:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				if (NULL != pTypeVal)
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
				if (NULL != pTypeVal)
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
				if (NULL != pTypeVal)
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
		if (oVal.m_idx.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblIDX);
			WriteCT_UnsignedInt(*oVal.m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for (size_t i = 0; i < oVal.m_Items.size(); ++i)
		{
			ItemsChoiceType4 eType = *oVal.m_ItemsElementName0[i];
			toBin(eType, oVal.m_Items[i]);
		}

		if (oVal.m_xForSave.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_xForSave);
			WriteCT_Boolean(*oVal.m_xForSave);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_showDataLabelsRange.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_showDataLabelsRange);
			WriteCT_Boolean(*oVal.m_showDataLabelsRange);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		//if (NULL != m_dlblFieldTable

		if (oVal.m_showLeaderLines.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_showLeaderLines);
			WriteCT_Boolean(*oVal.m_showLeaderLines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_leaderLines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_leaderLines);
			WriteCT_ChartLines(*oVal.m_leaderLines);
			m_oBcw.WriteItemEnd(nCurPos);
		}

		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::toBin(ItemsChoiceType4 eType, void* pVal)
	{
		switch(eType)
		{
		case itemschoicetype4DLBLPOS:
			{
				CDLblPos* pTypeVal = static_cast<CDLblPos*>(pVal);
				if (NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblDLBLPOS);
					WriteCT_DLblPos(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype4DELETE:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				if (NULL != pTypeVal)
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
				if (NULL != pTypeVal)
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
				if (NULL != pTypeVal)
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
				if (NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblSEPARATOR);
					m_oBcw.m_oStream.WriteStringW4(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype4SHOWBUBBLESIZE:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				if (NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblSHOWBUBBLESIZE);
					WriteCT_Boolean(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype4SHOWCATNAME:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				if (NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblSHOWCATNAME);
					WriteCT_Boolean(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype4SHOWLEGENDKEY:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				if (NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblSHOWLEGENDKEY);
					WriteCT_Boolean(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype4SHOWPERCENT:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				if (NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblSHOWPERCENT);
					WriteCT_Boolean(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype4SHOWSERNAME:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				if (NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblSHOWSERNAME);
					WriteCT_Boolean(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		case itemschoicetype4SHOWVAL:
			{
				bool* pTypeVal = static_cast<bool*>(pVal);
				if (NULL != pTypeVal)
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
				if (NULL != pTypeVal)
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
				if (NULL != pTypeVal)
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
				if (NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblTXPR);
					m_oBcw.m_oStream.WriteRecord1(0, (*pTypeVal));
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		}
	}
	void BinaryChartWriter::WriteCT_DLblPos(CDLblPos& oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_dlblposVAL);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_Trendline(CT_Trendline& oVal)
	{
		if (oVal.m_name.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlineNAME);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_name);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlineSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_trendlineType.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlineTRENDLINETYPE);
			WriteCT_TrendlineType(*oVal.m_trendlineType);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_order.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlineORDER);
			WriteCT_UnsignedByte(*oVal.m_order);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_period.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlinePERIOD);
			WriteCT_UnsignedInt(*oVal.m_period);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_forward.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlineFORWARD);
			WriteCT_Double(*oVal.m_forward);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_backward.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlineBACKWARD);
			WriteCT_Double(*oVal.m_backward);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_intercept.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlineINTERCEPT);
			WriteCT_Double(*oVal.m_intercept);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_dispRSqr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlineDISPRSQR);
			WriteCT_Boolean(*oVal.m_dispRSqr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_dispEq.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlineDISPEQ);
			WriteCT_Boolean(*oVal.m_dispEq);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_trendlineLbl)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlineTRENDLINELBL);
			WriteCT_TrendlineLbl(*oVal.m_trendlineLbl);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_TrendlineType(CTrendlineType& oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlinetypeVAL);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_TrendlineLbl(CT_TrendlineLbl& oVal)
	{
		if (NULL != oVal.m_layout)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlinelblLAYOUT);
			WriteCT_Layout(*oVal.m_layout);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_tx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlinelblTX);
			WriteCT_Tx(*oVal.m_tx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_numFmt)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlinelblNUMFMT);
			WriteCT_NumFmt(*oVal.m_numFmt);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlinelblSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_txPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_trendlinelblTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_txPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_ErrBars(CT_ErrBars& oVal)
	{
		if (oVal.m_errDir.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_errbarsERRDIR);
			WriteCT_ErrDir(*oVal.m_errDir);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_errBarType.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_errbarsERRBARTYPE);
			WriteCT_ErrBarType(*oVal.m_errBarType);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_errValType.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_errbarsERRVALTYPE);
			WriteCT_ErrValType(*oVal.m_errValType);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_noEndCap.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_errbarsNOENDCAP);
			WriteCT_Boolean(*oVal.m_noEndCap);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_plus)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_errbarsPLUS);
			WriteCT_NumDataSource(*oVal.m_plus);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_minus)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_errbarsMINUS);
			WriteCT_NumDataSource(*oVal.m_minus);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_val.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_errbarsVAL);
			WriteCT_Double(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_errbarsSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_ErrDir(CErrDir& oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_errdirVAL);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_ErrBarType(CErrBarType& oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_errbartypeVAL);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_ErrValType(CErrValType& oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_errvaltypeVAL);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_NumDataSource(CT_NumDataSource& oVal)
	{
		if (NULL != oVal.m_numLit)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_numdatasourceNUMLIT);
			WriteCT_NumData(*oVal.m_numLit);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_numRef)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_numdatasourceNUMREF);
			WriteCT_NumRef(*oVal.m_numRef);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_NumData(CT_NumData& oVal)
	{
		if (oVal.m_formatCode.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_numdataFORMATCODE);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_formatCode);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_ptCount.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_numdataPTCOUNT);
			WriteCT_UnsignedInt(*oVal.m_ptCount);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for (size_t i = 0, length = oVal.m_pt.size(); i < length; ++i)
		{
			CT_NumVal* pVal = oVal.m_pt[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_numdataPT);
				WriteCT_NumVal(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_NumVal(CT_NumVal& oVal)
	{
		if (oVal.m_v.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_numvalV);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_v);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_idx.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_numvalIDX);
			m_oBcw.m_oStream.WriteLONG(*oVal.m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_formatCode.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_numvalFORMATCODE);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_formatCode);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_NumRef(CT_NumRef& oVal)
	{
		if (oVal.m_f.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_numrefF);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_f);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_numCache)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_numrefNUMCACHE);
			WriteCT_NumData(*oVal.m_numCache);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_AxDataSource(CT_AxDataSource& oVal)
	{
		if (NULL != oVal.m_multiLvlStrRef)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_axdatasourceMULTILVLSTRREF);
			WriteCT_MultiLvlStrRef(*oVal.m_multiLvlStrRef);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_numLit)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_axdatasourceNUMLIT);
			WriteCT_NumData(*oVal.m_numLit);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_numRef)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_axdatasourceNUMREF);
			WriteCT_NumRef(*oVal.m_numRef);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_strLit)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_axdatasourceSTRLIT);
			WriteCT_StrData(*oVal.m_strLit);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_strRef)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_axdatasourceSTRREF);
			WriteCT_StrRef(*oVal.m_strRef);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_MultiLvlStrRef(CT_MultiLvlStrRef& oVal)
	{
		if (oVal.m_f.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_multilvlstrrefF);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_f);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_multiLvlStrCache)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_multilvlstrrefMULTILVLSTRCACHE);
			WriteCT_MultiLvlStrData(*oVal.m_multiLvlStrCache);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_lvl(CT_lvl& oVal)
	{
		for(size_t i = 0, length = oVal.m_pt.size(); i < length; ++i)
		{
			CT_StrVal* pVal = oVal.m_pt[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_lvlPT);
				WriteCT_StrVal(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
	}
	void BinaryChartWriter::WriteCT_MultiLvlStrData(CT_MultiLvlStrData& oVal)
	{
		if (oVal.m_ptCount.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_multilvlstrdataPTCOUNT);
			WriteCT_UnsignedInt(*oVal.m_ptCount);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_lvl.size(); i < length; ++i)
		{
			CT_lvl* pVal = oVal.m_lvl[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_multilvlstrdataLVL);
				WriteCT_lvl(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_BubbleChart(CT_BubbleChart& oVal)
	{
		if (oVal.m_varyColors.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubblechartVARYCOLORS);
			WriteCT_Boolean(*oVal.m_varyColors);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_BubbleSer* pVal = oVal.m_ser[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubblechartSER);
				WriteCT_BubbleSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubblechartDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_bubble3D.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubblechartBUBBLE3D);
			WriteCT_Boolean(*oVal.m_bubble3D);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_bubbleScale.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubblechartBUBBLESCALE);
			WriteCT_String(*oVal.m_bubbleScale);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_showNegBubbles.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubblechartSHOWNEGBUBBLES);
			WriteCT_Boolean(*oVal.m_showNegBubbles);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_sizeRepresents.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubblechartSIZEREPRESENTS);
			WriteCT_SizeRepresents(*oVal.m_sizeRepresents);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for (size_t i = 0, length = oVal.m_axId.size(); i < length; ++i)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bubblechartAXID);
			WriteCT_UnsignedInt(oVal.m_axId[i]);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_bandFmts(CT_bandFmts& oVal)
	{
		for(size_t i = 0, length = oVal.m_bandFmt.size(); i < length; ++i)
		{
			CT_BandFmt* pVal = oVal.m_bandFmt[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_bandfmtsBANDFMT);
				WriteCT_BandFmt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
	}
	void BinaryChartWriter::WriteCT_Surface3DChart(CT_Surface3DChart& oVal)
	{
		if (oVal.m_wireframe.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surface3dchartWIREFRAME);
			WriteCT_Boolean(*oVal.m_wireframe);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_SurfaceSer* pVal = oVal.m_ser[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_surface3dchartSER);
				WriteCT_SurfaceSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (NULL != oVal.m_bandFmts)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surface3dchartBANDFMTS);
			WriteCT_bandFmts(*oVal.m_bandFmts);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for (size_t i = 0, length = oVal.m_axId.size(); i < length; ++i)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surface3dchartAXID);
			WriteCT_UnsignedInt(oVal.m_axId[i]);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_SurfaceSer(CT_SurfaceSer& oVal)
	{
		if (oVal.m_idx.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surfaceserIDX);
			WriteCT_UnsignedInt(*oVal.m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_order.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surfaceserORDER);
			WriteCT_UnsignedInt(*oVal.m_order);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_tx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surfaceserTX);
			WriteCT_SerTx(*oVal.m_tx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surfaceserSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_cat)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surfaceserCAT);
			WriteCT_AxDataSource(*oVal.m_cat);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surfaceserVAL);
			WriteCT_NumDataSource(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_BandFmt(CT_BandFmt& oVal)
	{
		if (oVal.m_idx.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bandfmtIDX);
			WriteCT_UnsignedInt(*oVal.m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bandfmtSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_SurfaceChart(CT_SurfaceChart& oVal)
	{
		if (oVal.m_wireframe.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surfacechartWIREFRAME);
			WriteCT_Boolean(*oVal.m_wireframe);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for (size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_SurfaceSer* pVal = oVal.m_ser[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_surfacechartSER);
				WriteCT_SurfaceSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (NULL != oVal.m_bandFmts)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surfacechartBANDFMTS);
			WriteCT_bandFmts(*oVal.m_bandFmts);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_axId.size(); i < length; ++i)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surfacechartAXID);
			WriteCT_UnsignedInt(oVal.m_axId[i]);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_SplitType(CSplitType& oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_splittypeVAL);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_OfPieType(COfPieType& oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_ofpietypeVAL);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_custSplit(CT_custSplit& oVal)
	{
		for(size_t i = 0, length = oVal.m_secondPiePt.size(); i < length; ++i)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_custsplitSECONDPIEPT);
			WriteCT_UnsignedInt(oVal.m_secondPiePt[i]);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_OfPieChart(CT_OfPieChart& oVal)
	{
		if (oVal.m_ofPieType.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_ofpiechartOFPIETYPE);
			WriteCT_OfPieType(*oVal.m_ofPieType);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_varyColors.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_ofpiechartVARYCOLORS);
			WriteCT_Boolean(*oVal.m_varyColors);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for (size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_PieSer* pVal = oVal.m_ser[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_ofpiechartSER);
				WriteCT_PieSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_ofpiechartDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_gapWidth.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_ofpiechartGAPWIDTH);
			WriteCT_String(*oVal.m_gapWidth);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_splitType.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_ofpiechartSPLITTYPE);
			WriteCT_SplitType(*oVal.m_splitType);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_splitPos.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_ofpiechartSPLITPOS);
			WriteCT_Double(*oVal.m_splitPos);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_custSplit)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_ofpiechartCUSTSPLIT);
			WriteCT_custSplit(*oVal.m_custSplit);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_secondPieSize.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_ofpiechartSECONDPIESIZE);
			WriteCT_String(*oVal.m_secondPieSize);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for (size_t i = 0, length = oVal.m_serLines.size(); i < length; ++i)
		{
			CT_ChartLines* pVal = oVal.m_serLines[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_ofpiechartSERLINES);
				WriteCT_ChartLines(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_PieSer(CT_PieSer& oVal)
	{
		if (oVal.m_idx.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pieserIDX);
			WriteCT_UnsignedInt(*oVal.m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_order.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pieserORDER);
			WriteCT_UnsignedInt(*oVal.m_order);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_tx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pieserTX);
			WriteCT_SerTx(*oVal.m_tx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pieserSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_explosion.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pieserEXPLOSION);
			WriteCT_UnsignedInt(*oVal.m_explosion);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_dPt.size(); i < length; ++i)
		{
			CT_DPt* pVal = oVal.m_dPt[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_pieserDPT);
				WriteCT_DPt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pieserDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_cat)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pieserCAT);
			WriteCT_AxDataSource(*oVal.m_cat);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pieserVAL);
			WriteCT_NumDataSource(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_Bar3DChart(CT_Bar3DChart& oVal)
	{
		if (oVal.m_barDir.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bar3dchartBARDIR);
			WriteCT_BarDir(*oVal.m_barDir);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_grouping.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bar3dchartGROUPING);
			WriteCT_BarGrouping(*oVal.m_grouping);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_varyColors.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bar3dchartVARYCOLORS);
			WriteCT_Boolean(*oVal.m_varyColors);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for (size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_BarSer* pVal = oVal.m_ser[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_bar3dchartSER);
				WriteCT_BarSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bar3dchartDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_gapWidth.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bar3dchartGAPWIDTH);
			WriteCT_String(*oVal.m_gapWidth);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_gapDepth.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bar3dchartGAPDEPTH);
			WriteCT_String(*oVal.m_gapDepth);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_shape.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bar3dchartSHAPE);
			WriteCT_Shape(*oVal.m_shape);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_axId.size(); i < length; ++i)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_bar3dchartAXID);
			WriteCT_UnsignedInt(oVal.m_axId[i]);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_BarDir(CBarDir& oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_bardirVAL);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_BarGrouping(CBarGrouping& oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_bargroupingVAL);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_BarSer(CT_BarSer& oVal)
	{
		if (oVal.m_idx.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barserIDX);
			WriteCT_UnsignedInt(*oVal.m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_order.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barserORDER);
			WriteCT_UnsignedInt(*oVal.m_order);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_tx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barserTX);
			WriteCT_SerTx(*oVal.m_tx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barserSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_invertIfNegative.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barserINVERTIFNEGATIVE);
			WriteCT_Boolean(*oVal.m_invertIfNegative);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_pictureOptions)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barserPICTUREOPTIONS);
			WriteCT_PictureOptions(*oVal.m_pictureOptions);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for (size_t i = 0, length = oVal.m_dPt.size(); i < length; ++i)
		{
			CT_DPt* pVal = oVal.m_dPt[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_barserDPT);
				WriteCT_DPt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barserDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for (size_t i = 0, length = oVal.m_trendline.size(); i < length; ++i)
		{
			CT_Trendline* pVal = oVal.m_trendline[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_barserTRENDLINE);
				WriteCT_Trendline(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (NULL != oVal.m_errBars)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barserERRBARS);
			WriteCT_ErrBars(*oVal.m_errBars);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_cat)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barserCAT);
			WriteCT_AxDataSource(*oVal.m_cat);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barserVAL);
			WriteCT_NumDataSource(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_shape.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barserSHAPE);
			WriteCT_Shape(*oVal.m_shape);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_Shape(CShapeType& oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_shapeVAL);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_BarChart(CT_BarChart& oVal)
	{
		if (oVal.m_barDir.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barchartBARDIR);
			WriteCT_BarDir(*oVal.m_barDir);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_grouping.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barchartGROUPING);
			WriteCT_BarGrouping(*oVal.m_grouping);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_varyColors.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barchartVARYCOLORS);
			WriteCT_Boolean(*oVal.m_varyColors);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for (size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_BarSer* pVal = oVal.m_ser[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_barchartSER);
				WriteCT_BarSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barchartDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_gapWidth.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barchartGAPWIDTH);
			WriteCT_String(*oVal.m_gapWidth);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_overlap.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barchartOVERLAP);
			WriteCT_String(*oVal.m_overlap);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_serLines.size(); i < length; ++i)
		{
			CT_ChartLines* pVal = oVal.m_serLines[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_barchartSERLINES);
				WriteCT_ChartLines(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		for (size_t i = 0, length = oVal.m_axId.size(); i < length; ++i)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_barchartAXID);
			WriteCT_UnsignedInt(oVal.m_axId[i]);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_DoughnutChart(CT_DoughnutChart& oVal)
	{
		if (oVal.m_varyColors.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_doughnutchartVARYCOLORS);
			WriteCT_Boolean(*oVal.m_varyColors);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for (size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_PieSer* pVal = oVal.m_ser[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_doughnutchartSER);
				WriteCT_PieSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_doughnutchartDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_firstSliceAng.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_doughnutchartFIRSTSLICEANG);
			WriteCT_UnsignedInt(*oVal.m_firstSliceAng);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_holeSize.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_doughnutchartHOLESIZE);
			WriteCT_String(*oVal.m_holeSize);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_Pie3DChart(CT_Pie3DChart& oVal)
	{
		if (oVal.m_varyColors.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pie3dchartVARYCOLORS);
			WriteCT_Boolean(*oVal.m_varyColors);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_PieSer* pVal = oVal.m_ser[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_pie3dchartSER);
				WriteCT_PieSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pie3dchartDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_PieChart(CT_PieChart& oVal)
	{
		if (oVal.m_varyColors.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_piechartVARYCOLORS);
			WriteCT_Boolean(*oVal.m_varyColors);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_PieSer* pVal = oVal.m_ser[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_piechartSER);
				WriteCT_PieSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_piechartDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_firstSliceAng.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_piechartFIRSTSLICEANG);
			WriteCT_UnsignedInt(*oVal.m_firstSliceAng);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_ScatterSer(CT_ScatterSer& oVal)
	{
		if (oVal.m_idx.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterserIDX);
			WriteCT_UnsignedInt(*oVal.m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_order.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterserORDER);
			WriteCT_UnsignedInt(*oVal.m_order);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_tx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterserTX);
			WriteCT_SerTx(*oVal.m_tx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterserSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_marker)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterserMARKER);
			WriteCT_Marker(*oVal.m_marker);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_dPt.size(); i < length; ++i)
		{
			CT_DPt* pVal = oVal.m_dPt[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterserDPT);
				WriteCT_DPt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterserDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_trendline.size(); i < length; ++i)
		{
			CT_Trendline* pVal = oVal.m_trendline[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterserTRENDLINE);
				WriteCT_Trendline(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		for(size_t i = 0, length = oVal.m_errBars.size(); i < length; ++i)
		{
			CT_ErrBars* pVal = oVal.m_errBars[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterserERRBARS);
				WriteCT_ErrBars(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (NULL != oVal.m_xVal)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterserXVAL);
			WriteCT_AxDataSource(*oVal.m_xVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_yVal)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterserYVAL);
			WriteCT_NumDataSource(*oVal.m_yVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_smooth.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterserSMOOTH);
			WriteCT_Boolean(*oVal.m_smooth);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_ScatterStyle(CScatterStyle& oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterstyleVAL);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_ScatterChart(CT_ScatterChart& oVal)
	{
		if (oVal.m_scatterStyle.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterchartSCATTERSTYLE);
			WriteCT_ScatterStyle(*oVal.m_scatterStyle);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_varyColors.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterchartVARYCOLORS);
			WriteCT_Boolean(*oVal.m_varyColors);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for (size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_ScatterSer* pVal = oVal.m_ser[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterchartSER);
				WriteCT_ScatterSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterchartDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for (size_t i = 0, length = oVal.m_axId.size(); i < length; ++i)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_scatterchartAXID);
			WriteCT_UnsignedInt(oVal.m_axId[i]);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_RadarSer(CT_RadarSer& oVal)
	{
		if (oVal.m_idx.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarserIDX);
			WriteCT_UnsignedInt(*oVal.m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_order.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarserORDER);
			WriteCT_UnsignedInt(*oVal.m_order);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_tx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarserTX);
			WriteCT_SerTx(*oVal.m_tx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarserSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_marker)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarserMARKER);
			WriteCT_Marker(*oVal.m_marker);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for (size_t i = 0, length = oVal.m_dPt.size(); i < length; ++i)
		{
			CT_DPt* pVal = oVal.m_dPt[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarserDPT);
				WriteCT_DPt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarserDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_cat)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarserCAT);
			WriteCT_AxDataSource(*oVal.m_cat);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarserVAL);
			WriteCT_NumDataSource(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_RadarStyle(CRadarStyle& oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarstyleVAL);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_RadarChart(CT_RadarChart& oVal)
	{
		if (oVal.m_radarStyle.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarchartRADARSTYLE);
			WriteCT_RadarStyle(*oVal.m_radarStyle);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_varyColors.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarchartVARYCOLORS);
			WriteCT_Boolean(*oVal.m_varyColors);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for (size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_RadarSer* pVal = oVal.m_ser[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarchartSER);
				WriteCT_RadarSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarchartDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_axId.size(); i < length; ++i)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_radarchartAXID);
			WriteCT_UnsignedInt(oVal.m_axId[i]);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_StockChart(CT_StockChart& oVal)
	{
		for(size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_LineSer* pVal = oVal.m_ser[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_stockchartSER);
				WriteCT_LineSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_stockchartDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_dropLines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_stockchartDROPLINES);
			WriteCT_ChartLines(*oVal.m_dropLines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_hiLowLines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_stockchartHILOWLINES);
			WriteCT_ChartLines(*oVal.m_hiLowLines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_upDownBars)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_stockchartUPDOWNBARS);
			WriteCT_UpDownBars(*oVal.m_upDownBars);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for (size_t i = 0, length = oVal.m_axId.size(); i < length; ++i)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_stockchartAXID);
			WriteCT_UnsignedInt(oVal.m_axId[i]);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_LineSer(CT_LineSer& oVal)
	{
		if (oVal.m_idx.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_lineserIDX);
			WriteCT_UnsignedInt(*oVal.m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_order.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_lineserORDER);
			WriteCT_UnsignedInt(*oVal.m_order);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_tx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_lineserTX);
			WriteCT_SerTx(*oVal.m_tx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_lineserSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_marker)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_lineserMARKER);
			WriteCT_Marker(*oVal.m_marker);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_dPt.size(); i < length; ++i)
		{
			CT_DPt* pVal = oVal.m_dPt[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_lineserDPT);
				WriteCT_DPt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_lineserDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_trendline.size(); i < length; ++i)
		{
			CT_Trendline* pVal = oVal.m_trendline[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_lineserTRENDLINE);
				WriteCT_Trendline(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (NULL != oVal.m_errBars)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_lineserERRBARS);
			WriteCT_ErrBars(*oVal.m_errBars);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_cat)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_lineserCAT);
			WriteCT_AxDataSource(*oVal.m_cat);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_lineserVAL);
			WriteCT_NumDataSource(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_smooth.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_lineserSMOOTH);
			WriteCT_Boolean(*oVal.m_smooth);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_UpDownBars(CT_UpDownBars& oVal)
	{
		if (oVal.m_gapWidth.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_updownbarsGAPWIDTH);
			WriteCT_String(*oVal.m_gapWidth);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_upBars)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_updownbarsUPBARS);
			WriteCT_UpDownBar(*oVal.m_upBars);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_downBars)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_updownbarsDOWNBARS);
			WriteCT_UpDownBar(*oVal.m_downBars);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_UpDownBar(CT_UpDownBar& oVal)
	{
		if (oVal.m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_updownbarSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_Line3DChart(CT_Line3DChart& oVal)
	{
		if (oVal.m_grouping.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_line3dchartGROUPING);
			WriteCT_Grouping(*oVal.m_grouping);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_varyColors.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_line3dchartVARYCOLORS);
			WriteCT_Boolean(*oVal.m_varyColors);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_LineSer* pVal = oVal.m_ser[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_line3dchartSER);
				WriteCT_LineSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_line3dchartDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_dropLines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_line3dchartDROPLINES);
			WriteCT_ChartLines(*oVal.m_dropLines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_gapDepth.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_line3dchartGAPDEPTH);
			WriteCT_String(*oVal.m_gapDepth);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_axId.size(); i < length; ++i)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_line3dchartAXID);
			WriteCT_UnsignedInt(oVal.m_axId[i]);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_Grouping(CGrouping& oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_groupingVAL);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oVal.GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_LineChart(CT_LineChart& oVal)
	{
		if (oVal.m_grouping.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_linechartGROUPING);
			WriteCT_Grouping(*oVal.m_grouping);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_varyColors.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_linechartVARYCOLORS);
			WriteCT_Boolean(*oVal.m_varyColors);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_LineSer* pVal = oVal.m_ser[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_linechartSER);
				WriteCT_LineSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_linechartDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_dropLines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_linechartDROPLINES);
			WriteCT_ChartLines(*oVal.m_dropLines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_hiLowLines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_linechartHILOWLINES);
			WriteCT_ChartLines(*oVal.m_hiLowLines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_upDownBars)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_linechartUPDOWNBARS);
			WriteCT_UpDownBars(*oVal.m_upDownBars);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_marker.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_linechartMARKER);
			WriteCT_Boolean(*oVal.m_marker);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_smooth.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_linechartSMOOTH);
			WriteCT_Boolean(*oVal.m_smooth);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_axId.size(); i < length; ++i)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_linechartAXID);
			WriteCT_UnsignedInt(oVal.m_axId[i]);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_Area3DChart(CT_Area3DChart& oVal)
	{
		if (oVal.m_grouping.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_area3dchartGROUPING);
			WriteCT_Grouping(*oVal.m_grouping);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_varyColors.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_area3dchartVARYCOLORS);
			WriteCT_Boolean(*oVal.m_varyColors);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_AreaSer* pVal = oVal.m_ser[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_area3dchartSER);
				WriteCT_AreaSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_area3dchartDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_dropLines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_area3dchartDROPLINES);
			WriteCT_ChartLines(*oVal.m_dropLines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_gapDepth.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_area3dchartGAPDEPTH);
			WriteCT_String(*oVal.m_gapDepth);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_axId.size(); i < length; ++i)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_area3dchartAXID);
			WriteCT_UnsignedInt(oVal.m_axId[i]);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_AreaSer(CT_AreaSer& oVal)
	{
		if (oVal.m_idx.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_areaserIDX);
			WriteCT_UnsignedInt(*oVal.m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_order.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_areaserORDER);
			WriteCT_UnsignedInt(*oVal.m_order);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_tx)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_areaserTX);
			WriteCT_SerTx(*oVal.m_tx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_areaserSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_pictureOptions)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_areaserPICTUREOPTIONS);
			WriteCT_PictureOptions(*oVal.m_pictureOptions);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for (size_t i = 0, length = oVal.m_dPt.size(); i < length; ++i)
		{
			CT_DPt* pVal = oVal.m_dPt[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_areaserDPT);
				WriteCT_DPt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_areaserDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for (size_t i = 0, length = oVal.m_trendline.size(); i < length; ++i)
		{
			CT_Trendline* pVal = oVal.m_trendline[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_areaserTRENDLINE);
				WriteCT_Trendline(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		for (size_t i = 0, length = oVal.m_errBars.size(); i < length; ++i)
		{
			CT_ErrBars* pVal = oVal.m_errBars[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_areaserERRBARS);
				WriteCT_ErrBars(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (NULL != oVal.m_cat)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_areaserCAT);
			WriteCT_AxDataSource(*oVal.m_cat);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_val)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_areaserVAL);
			WriteCT_NumDataSource(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_AreaChart(CT_AreaChart& oVal)
	{
		if (oVal.m_grouping.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_areachartGROUPING);
			WriteCT_Grouping(*oVal.m_grouping);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_varyColors.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_areachartVARYCOLORS);
			WriteCT_Boolean(*oVal.m_varyColors);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_ser.size(); i < length; ++i)
		{
			CT_AreaSer* pVal = oVal.m_ser[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_areachartSER);
				WriteCT_AreaSer(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (NULL != oVal.m_dLbls)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_areachartDLBLS);
			WriteCT_DLbls(*oVal.m_dLbls);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_dropLines)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_areachartDROPLINES);
			WriteCT_ChartLines(*oVal.m_dropLines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for(size_t i = 0, length = oVal.m_axId.size(); i < length; ++i)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_areachartAXID);
			WriteCT_UnsignedInt(oVal.m_axId[i]);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_PlotArea(CT_PlotArea& oVal)
	{
		if (NULL != oVal.m_layout)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_plotareaLAYOUT);
			WriteCT_Layout(*oVal.m_layout);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for (size_t i = 0; i < oVal.m_Items.size(); ++i)
		{
			ItemsChoiceType5 eType = *oVal.m_ItemsElementName0[i];
			toBin(eType, oVal.m_Items[i]);
		}
		for(size_t i = 0, length = oVal.m_Items1.size(); i < length; ++i)
		{
			ItemsChoiceType6 eType = *oVal.m_ItemsElementName1[i];
			toBin(eType, oVal.m_Items1[i]);
		}
		if (NULL != oVal.m_dTable)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_plotareaDTABLE);
			WriteCT_DTable(*oVal.m_dTable);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_plotareaSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::toBin(ItemsChoiceType5 eType, void* pVal){
		switch(eType)
		{
		case itemschoicetype5AREA3DCHART:
			{
				CT_Area3DChart* pTypeVal = static_cast<CT_Area3DChart*>(pVal);
				if (NULL != pTypeVal)
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
				if (NULL != pTypeVal)
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
				if (NULL != pTypeVal)
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
				if (NULL != pTypeVal)
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
				if (NULL != pTypeVal)
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
				if (NULL != pTypeVal)
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
				if (NULL != pTypeVal)
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
				if (NULL != pTypeVal)
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
				if (NULL != pTypeVal)
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
				if (NULL != pTypeVal)
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
				if (NULL != pTypeVal)
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
				if (NULL != pTypeVal)
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
				if (NULL != pTypeVal)
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
				if (NULL != pTypeVal)
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
				if (NULL != pTypeVal)
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
				if (NULL != pTypeVal)
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
				if (NULL != pTypeVal)
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
				if (NULL != pTypeVal)
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
				if (NULL != pTypeVal)
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
				if (NULL != pTypeVal)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_plotareaVALAX);
					WriteCT_ValAx(*pTypeVal);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			break;
		}
	}
	void BinaryChartWriter::WriteCT_Surface(CT_Surface& oVal)
	{
		if (oVal.m_thickness.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surfaceTHICKNESS);
			WriteCT_String(*oVal.m_thickness);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surfaceSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_pictureOptions)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_surfacePICTUREOPTIONS);
			WriteCT_PictureOptions(*oVal.m_pictureOptions);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}

	void BinaryChartWriter::WriteCT_View3D(CT_View3D& oVal)
	{
		if (oVal.m_rotX.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_view3dROTX); 
			WriteCT_SignedByte(*oVal.m_rotX);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_hPercent.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_view3dHPERCENT);
			WriteCT_String(*oVal.m_hPercent);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_rotY.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_view3dROTY);
			WriteCT_UnsignedInt(*oVal.m_rotY);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_depthPercent.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_view3dDEPTHPERCENT);
			WriteCT_String(*oVal.m_depthPercent);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_rAngAx.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_view3dRANGAX);
			WriteCT_Boolean(*oVal.m_rAngAx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_perspective.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_view3dPERSPECTIVE);
			WriteCT_UnsignedByte(*oVal.m_perspective);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_PivotFmt(CT_PivotFmt& oVal)
	{
		if (oVal.m_idx.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pivotfmtIDX);
			WriteCT_UnsignedInt(*oVal.m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pivotfmtSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_txPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pivotfmtTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_txPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_marker)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pivotfmtMARKER);
			WriteCT_Marker(*oVal.m_marker);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_dLbl)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pivotfmtDLBL);
			WriteCT_DLbl(*oVal.m_dLbl);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_pivotFmts(CT_pivotFmts& oVal)
	{
		for(size_t i = 0, length = oVal.m_pivotFmt.size(); i < length; ++i)
		{
			CT_PivotFmt* pVal = oVal.m_pivotFmt[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_pivotfmtsPIVOTFMT);
				WriteCT_PivotFmt(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
	}
	void BinaryChartWriter::WriteCT_Chart(CT_Chart& oVal)
	{
		if (NULL != oVal.m_title)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartTITLE);
			WriteCT_Title(*oVal.m_title);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_autoTitleDeleted.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartAUTOTITLEDELETED);
			WriteCT_Boolean(*oVal.m_autoTitleDeleted);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_pivotFmts)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartPIVOTFMTS);
			WriteCT_pivotFmts(*oVal.m_pivotFmts);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_view3D)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartVIEW3D);
			WriteCT_View3D(*oVal.m_view3D);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_floor)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartFLOOR);
			WriteCT_Surface(*oVal.m_floor);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_sideWall)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartSIDEWALL);
			WriteCT_Surface(*oVal.m_sideWall);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_backWall)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartBACKWALL);
			WriteCT_Surface(*oVal.m_backWall);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_plotArea)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartPLOTAREA);
			WriteCT_PlotArea(*oVal.m_plotArea);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (NULL != oVal.m_legend)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartLEGEND);
			WriteCT_Legend(*oVal.m_legend);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_plotVisOnly.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartPLOTVISONLY);
			WriteCT_Boolean(*oVal.m_plotVisOnly);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_dispBlanksAs.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartDISPBLANKSAS);
			WriteCT_DispBlanksAs(*oVal.m_dispBlanksAs);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_showDLblsOverMax.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartSHOWDLBLSOVERMAX);
			WriteCT_Boolean(*oVal.m_showDLblsOverMax);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		WriteExtensions(oVal.m_extLst.GetPointer());
	}
	void BinaryChartWriter::WriteCT_Protection(CT_Protection& oVal)
	{
		if (oVal.m_chartObject.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_protectionCHARTOBJECT);
			WriteCT_Boolean(*oVal.m_chartObject);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_data.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_protectionDATA);
			WriteCT_Boolean(*oVal.m_data);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_formatting.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_protectionFORMATTING);
			WriteCT_Boolean(*oVal.m_formatting);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_selection.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_protectionSELECTION);
			WriteCT_Boolean(*oVal.m_selection);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_userInterface.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_protectionUSERINTERFACE);
			WriteCT_Boolean(*oVal.m_userInterface);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_PivotSource(CT_PivotSource& oVal)
	{
		if (oVal.m_name.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pivotsourceNAME);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_name);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_fmtId.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_pivotsourceFMTID);
			WriteCT_UnsignedInt(*oVal.m_fmtId);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_Style(CT_Style& oVal)
	{
		if (oVal.m_val.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(oVal.m_namespace == L"c14" ? c_oserct_style1VAL : c_oserct_styleVAL);
			m_oBcw.m_oStream.WriteBYTE(*oVal.m_val);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteAlternateContent(AlternateContent& oVal)
	{
		for (size_t i = 0, length = oVal.m_Choice.size(); i < length; ++i)
		{
			AlternateContentChoice* pVal = oVal.m_Choice[i];
			if (NULL != pVal)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oseralternatecontentCHOICE);
				WriteAlternateContentChoice(*pVal);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (NULL != oVal.m_Fallback)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oseralternatecontentFALLBACK);
			WriteAlternateContentFallback(*oVal.m_Fallback);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteAlternateContentChoice(AlternateContentChoice& oVal)
	{
		if (NULL != oVal.m_style)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oseralternatecontentchoiceSTYLE);
			WriteCT_Style(*oVal.m_style);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_Requires.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oseralternatecontentchoiceREQUIRES);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_Requires);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteAlternateContentFallback(AlternateContentFallback& oVal)
	{
		if (NULL != oVal.m_style)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oseralternatecontentfallbackSTYLE);
			WriteCT_Style(*oVal.m_style);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ChartStyle(OOX::Spreadsheet::CChartStyleFile & oVal)
	{
		if (oVal.m_oChartStyle.m_id.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartstyleID);
			m_oBcw.m_oStream.WriteULONG(*oVal.m_oChartStyle.m_id);
			m_oBcw.WriteItemEnd(nCurPos);
		}

		for (size_t i = 0; i < oVal.m_oChartStyle.m_arStyleEntries.size(); ++i)
		{
			if (!oVal.m_oChartStyle.m_arStyleEntries[i]) continue;

			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartstyleENTRY);
			WriteCT_StyleEntry(*oVal.m_oChartStyle.m_arStyleEntries[i]);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oChartStyle.m_dataPointMarkerLayout.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartstyleMARKERLAYOUT);
			WriteCT_MarkerLayout(*oVal.m_oChartStyle.m_dataPointMarkerLayout);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_StyleEntry(OOX::Spreadsheet::ChartEx::CStyleEntry & oVal)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartstyleENTRYTYPE);
		m_oBcw.m_oStream.WriteBYTE(oVal.getTypeStyleEntry());
		m_oBcw.WriteItemEnd(nCurPos);

		nCurPos = m_oBcw.WriteItemStart(c_oserct_chartstyleLNREF);
		m_oBcw.m_oStream.WriteRecord1(0, oVal.m_lnRef);
		m_oBcw.WriteItemEnd(nCurPos);
		
		nCurPos = m_oBcw.WriteItemStart(c_oserct_chartstyleFILLREF);
		m_oBcw.m_oStream.WriteRecord1(0, oVal.m_fillRef);
		m_oBcw.WriteItemEnd(nCurPos);

		nCurPos = m_oBcw.WriteItemStart(c_oserct_chartstyleEFFECTREF);
		m_oBcw.m_oStream.WriteRecord1(0, oVal.m_effectRef);
		m_oBcw.WriteItemEnd(nCurPos);

		nCurPos = m_oBcw.WriteItemStart(c_oserct_chartstyleFONTREF);
		m_oBcw.m_oStream.WriteRecord1(0, oVal.m_fontRef);
		m_oBcw.WriteItemEnd(nCurPos);

		if (oVal.m_defRPr.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oserct_chartstyleDEFPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_defRPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_bodyPr.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oserct_chartstyleBODYPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_bodyPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_spPr.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oserct_chartstyleSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_lineWidthScale.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oserct_chartstyleLINEWIDTH);
			m_oBcw.m_oStream.WriteDoubleReal(*oVal.m_lineWidthScale);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_MarkerLayout(OOX::Spreadsheet::ChartEx::CMarkerLayout & oVal)
	{
		if (oVal.m_symbol.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartstyleMARKERSYMBOL);
			m_oBcw.m_oStream.WriteBYTE(oVal.m_symbol->GetValue());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_size.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartstyleMARKERSIZE);
			m_oBcw.m_oStream.WriteULONG(*oVal.m_size);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ChartColor(OOX::Spreadsheet::CChartColorsFile & oVal)
	{		
		if (oVal.m_oColorStyle.m_id.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartcolorsID);
			m_oBcw.m_oStream.WriteULONG(*oVal.m_oColorStyle.m_id);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oColorStyle.m_meth.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartcolorsMETH);
			m_oBcw.m_oStream.WriteStringW4(*oVal.m_oColorStyle.m_meth);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for (size_t i = 0; i < oVal.m_oColorStyle.m_arrItems.size(); ++i)
		{
			OOX::Spreadsheet::ChartEx::CVariation *pVariation = dynamic_cast<OOX::Spreadsheet::ChartEx::CVariation*>(oVal.m_oColorStyle.m_arrItems[i]);

			if (pVariation)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartcolorsVARIATION);
				WriteCT_Variation(*pVariation);
				m_oBcw.WriteItemEnd(nCurPos);
			}
			else
			{
				PPTX::Logic::UniColor *pColor = dynamic_cast<PPTX::Logic::UniColor*>(oVal.m_oColorStyle.m_arrItems[i]);
				if (pColor)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartcolorsCOLOR);
					m_oBcw.m_oStream.WriteRecord1(0, *pColor);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
		}

	}
	void BinaryChartWriter::WriteCT_Variation(OOX::Spreadsheet::ChartEx::CVariation & oVal)
	{
		for (size_t i = 0; i < oVal.m_arrItems.size(); ++i)
		{
			if (!oVal.m_arrItems[i]) continue;
			
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartcolorsEFFECT);
			m_oBcw.m_oStream.WriteRecord1(0, *oVal.m_arrItems[i]);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
//-------------------------------------------------------------------------------------------------------------
	void BinaryChartWriter::WriteCT_ChartExFile(OOX::Spreadsheet::CChartExFile & oChartFile)
	{
		OOX::Spreadsheet::ChartEx::CChartSpace & oVal = oChartFile.m_oChartSpace;

		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExSpaceCHARTDATA);
			WriteCT_ChartExChartData(&oVal.m_chartData);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExSpaceCHART);
			WriteCT_ChartExChart(&oVal.m_chart);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_oClrMapOvr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExSpaceCLRMAPOVR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_oClrMapOvr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExSpaceSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (oVal.m_txPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExSpaceTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, oVal.m_txPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ChartExChartData(OOX::Spreadsheet::ChartEx::CChartData *pVal)
	{
		if (!pVal) return;

		for (size_t i = 0; i < pVal->m_arData.size(); ++i)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDATA);
				WriteCT_ChartExData(pVal->m_arData[i]);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_externalData.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExEXTERNALDATA);
				WriteCT_ChartExExternalData(pVal->m_externalData.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ChartExChart(OOX::Spreadsheet::ChartEx::CChart *pVal)
	{
		if (!pVal) return;

		int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExChartPLOTAREA);
		WriteCT_ChartExPlotArea(&pVal->m_plotArea);
		m_oBcw.WriteItemEnd(nCurPos);

		if (pVal->m_title.IsInit())
		{			
			nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExChartTITLE);
			WriteCT_ChartExTitle(pVal->m_title.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_legend.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExChartLEGEND);
			WriteCT_ChartExLegend(pVal->m_legend.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ChartExData(OOX::Spreadsheet::ChartEx::CData *pVal)
	{
		if (!pVal) return;

		if (pVal->m_id.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataID);
			m_oBcw.m_oStream.WriteULONG(*pVal->m_id);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for (size_t i = 0; i < pVal->m_arDimension.size(); ++i)
		{
			OOX::Spreadsheet::ChartEx::CStrDimension *pStr = dynamic_cast<OOX::Spreadsheet::ChartEx::CStrDimension*>(pVal->m_arDimension[i]);
			if (pStr)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataSTRDIMENSION);
				WriteCT_ChartExDataDimension(pStr);
				m_oBcw.WriteItemEnd(nCurPos);
			}
			else
			{
				OOX::Spreadsheet::ChartEx::CNumDimension *pNum = dynamic_cast<OOX::Spreadsheet::ChartEx::CNumDimension*>(pVal->m_arDimension[i]);
				if (pNum)
				{
					int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataNUMDIMENSION);
					WriteCT_ChartExDataDimension(pNum);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
		}
	}
	void BinaryChartWriter::WriteCT_ChartExDataDimension(OOX::Spreadsheet::ChartEx::CStrDimension *pVal)
	{
		if (!pVal) return;

		int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataDimensionTYPE);
		m_oBcw.m_oStream.WriteBYTE(pVal->m_type->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);		

		if (pVal->m_f.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataDimensionFORMULA);
			WriteCT_ChartExFormula(pVal->m_f.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_nf.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataDimensionNF);
			m_oBcw.m_oStream.WriteStringW4(*pVal->m_nf);
			m_oBcw.WriteItemEnd(nCurPos);
		}

		for (size_t i = 0; i < pVal->m_levelData.size(); ++i)
		{
			if (pVal->m_levelData[i])
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataDimensionSTRINGLEVEL);
				WriteCT_ChartExStringLevel(pVal->m_levelData[i]);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}		
	}
	void BinaryChartWriter::WriteCT_ChartExDataDimension(OOX::Spreadsheet::ChartEx::CNumDimension *pVal)
	{
		if (!pVal) return;

		int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataDimensionTYPE);
		m_oBcw.m_oStream.WriteBYTE(pVal->m_type->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);

		if (pVal->m_f.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataDimensionFORMULA);
			WriteCT_ChartExFormula(pVal->m_f.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_nf.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataDimensionNF);
			m_oBcw.m_oStream.WriteStringW4(*pVal->m_nf);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for (size_t i = 0; i < pVal->m_levelData.size(); ++i)
		{
			if (pVal->m_levelData[i])
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataDimensionNUMERICLEVEL);
				WriteCT_ChartExNumericLevel(pVal->m_levelData[i]);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
	}
	void BinaryChartWriter::WriteCT_ChartExExternalData(OOX::Spreadsheet::ChartEx::CExternalData *pVal)
	{
		if (!pVal) return;
		
		if (pVal->m_autoUpdate.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExExternalAUTOUPDATE);
			m_oBcw.m_oStream.WriteBOOL(*pVal->m_autoUpdate);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		//if (NULL != pVal.m_id) - выше
	}
	void BinaryChartWriter::WriteCT_ChartExFormula(OOX::Spreadsheet::ChartEx::CFormula *pVal)
	{
		if (!pVal) return;

		int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExFormulaCONTENT);
		m_oBcw.m_oStream.WriteStringW4(pVal->m_content);
		m_oBcw.WriteItemEnd(nCurPos);

		if (pVal->m_dir.IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExFormulaDIRECTION);
			m_oBcw.m_oStream.WriteBYTE(pVal->m_dir->GetValue());
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ChartExStringLevel(OOX::Spreadsheet::ChartEx::CStringLevel *pVal)
	{
		if (!pVal) return;

		if (pVal->m_name.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataLevelNAME);
			m_oBcw.m_oStream.WriteStringW4(*pVal->m_name);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_ptCount.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataLevelCOUNT);
			m_oBcw.m_oStream.WriteINT(*pVal->m_ptCount);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for (size_t i = 0; i < pVal->m_arPt.size(); ++i)
		{
			if (pVal->m_arPt[i])
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataLevelPT);
				WriteCT_ChartExStringValue(pVal->m_arPt[i]);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}	
	}
	void BinaryChartWriter::WriteCT_ChartExNumericLevel(OOX::Spreadsheet::ChartEx::CNumericLevel *pVal)
	{
		if (!pVal) return;

		if (pVal->m_name.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataLevelNAME);
			m_oBcw.m_oStream.WriteStringW4(*pVal->m_name);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_ptCount.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataLevelCOUNT);
			m_oBcw.m_oStream.WriteINT(*pVal->m_ptCount);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_formatCode.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataLevelFORMATCODE);
			m_oBcw.m_oStream.WriteStringW4(*pVal->m_formatCode);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for (size_t i = 0; i < pVal->m_arPt.size(); ++i)
		{
			if (pVal->m_arPt[i])
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataLevelPT);
				WriteCT_ChartExNumericValue(pVal->m_arPt[i]);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
	}
	void BinaryChartWriter::WriteCT_ChartExStringValue(OOX::Spreadsheet::ChartEx::CStringValue *pVal)
	{
		if (!pVal) return;

		if (pVal->m_idx.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataValueIDX);
			m_oBcw.m_oStream.WriteLONG(*pVal->m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataValueCONTENT);
		m_oBcw.m_oStream.WriteStringW4(pVal->m_content);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_ChartExNumericValue(OOX::Spreadsheet::ChartEx::CNumericValue *pVal)
	{
		if (!pVal) return;

		if (pVal->m_idx.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataValueIDX);
			m_oBcw.m_oStream.WriteLONG(*pVal->m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataValueCONTENT);
		m_oBcw.m_oStream.WriteDoubleReal(*pVal->m_content);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCT_ChartExPlotArea(OOX::Spreadsheet::ChartEx::CPlotArea*pVal)
	{
		if (!pVal) return;

		int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExChartAREAREGION);
		WriteCT_ChartExPlotAreaRegion(&pVal->m_plotAreaRegion);
		m_oBcw.WriteItemEnd(nCurPos);

		for (size_t i = 0; i < pVal->m_arAxis.size(); ++i)
		{
			if (pVal->m_arAxis[i])
			{
				nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExChartAXIS);
				WriteCT_ChartExAxis(pVal->m_arAxis[i]);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (pVal->m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExChartSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, pVal->m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ChartExTitle(OOX::Spreadsheet::ChartEx::CTitle *pVal)
	{
		if (!pVal) return;

		if (pVal->m_tx.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExTitleTX);
			WriteCT_ChartExText(pVal->m_tx.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_txPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExTitleTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, pVal->m_txPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExTitleSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, pVal->m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_pos.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExTitlePOS);
			m_oBcw.m_oStream.WriteBYTE(pVal->m_pos->GetValue());
			m_oBcw.WriteItemEnd(nCurPos);
		}		
		if (pVal->m_align.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExTitleALIGN);
			m_oBcw.m_oStream.WriteBYTE(pVal->m_align->GetValue());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_overlay.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExTitleOVERLAY);
			m_oBcw.m_oStream.WriteBOOL(*pVal->m_overlay);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ChartExLegend(OOX::Spreadsheet::ChartEx::CLegend *pVal)
	{
		if (!pVal) return;

		if (pVal->m_txPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExLegendTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, pVal->m_txPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExLegendSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, pVal->m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_pos.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExLegendPOS);
			m_oBcw.m_oStream.WriteBYTE(pVal->m_pos->GetValue());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_align.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExLegendALIGN);
			m_oBcw.m_oStream.WriteBYTE(pVal->m_align->GetValue());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_overlay.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExLegendOVERLAY);
			m_oBcw.m_oStream.WriteBOOL(*pVal->m_overlay);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ChartExPlotAreaRegion(OOX::Spreadsheet::ChartEx::CPlotAreaRegion *pVal)
	{
		if (!pVal) return;

		if (pVal->m_plotSurface.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExAreaPLOTSURFACE);
			WriteCT_ChartExPlotSurface(pVal->m_plotSurface.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}		
		for (size_t i = 0; i < pVal->m_arSeries.size(); ++i)
		{
			if (pVal->m_arSeries[i])
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExAreaSERIES);
				WriteCT_ChartExSeries(pVal->m_arSeries[i]);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
	}
	void BinaryChartWriter::WriteCT_ChartExAxis(OOX::Spreadsheet::ChartEx::CAxis *pVal)
	{
		if (pVal->m_id.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExAxisID);
			m_oBcw.m_oStream.WriteLONG(*pVal->m_id);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_hidden.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExAxisHIDDEN);
			m_oBcw.m_oStream.WriteBOOL(*pVal->m_hidden);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_catScaling.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExAxisCATSCALING);
			WriteCT_ChartExCatScaling(pVal->m_catScaling.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_valScaling.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExAxisVALSCALING);
			WriteCT_ChartExValScaling(pVal->m_valScaling.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_title.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExAxisTITLE);
			WriteCT_ChartExTitle(pVal->m_title.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_units.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExAxisUNIT);
			WriteCT_ChartExAxisUnit(pVal->m_units.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_numFmt.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExAxisNUMFMT);
			WriteCT_ChartExNumberFormat(pVal->m_numFmt.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_majorTickMarks.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExAxisMAJORTICK);
			WriteCT_ChartExTickMarks(pVal->m_majorTickMarks.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_minorTickMarks.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExAxisMINORTICK);
			WriteCT_ChartExTickMarks(pVal->m_minorTickMarks.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_majorGridlines.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExAxisMAJORGRID);
			WriteCT_ChartExGridlines(pVal->m_majorGridlines.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_minorGridlines.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExAxisMINORGRID);
			WriteCT_ChartExGridlines(pVal->m_minorGridlines.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_tickLabels.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExAxisTICKLABELS);
			m_oBcw.m_oStream.WriteBOOL(*pVal->m_tickLabels);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_txPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExAxisTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, pVal->m_txPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExAxisSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, pVal->m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ChartExText(OOX::Spreadsheet::ChartEx::CText *pVal)
	{
		if (!pVal) return;

		if (pVal->m_oRich.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExTextRICH);
			m_oBcw.m_oStream.WriteRecord2(0, pVal->m_oRich);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_txData.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExTextDATA);
			WriteCT_ChartExTextData(pVal->m_txData.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ChartExTextData(OOX::Spreadsheet::ChartEx::CTextData *pVal)
	{
		if (!pVal) return;

		if (pVal->m_oF.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExTextDataFORMULA);
			WriteCT_ChartExFormula(pVal->m_oF.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_oV.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExTextDataVALUE);
			m_oBcw.m_oStream.WriteStringW4(*pVal->m_oV);
			m_oBcw.WriteItemEnd(nCurPos);
		}

	}
	void BinaryChartWriter::WriteCT_ChartExPlotSurface(OOX::Spreadsheet::ChartEx::CPlotSurface *pVal)
	{
		if (!pVal) return;

		if (pVal->m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExPlotSurfaceSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, pVal->m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ChartExSeries(OOX::Spreadsheet::ChartEx::CSeries *pVal)
	{
		for (size_t i = 0; i < pVal->m_arDataPt.size(); ++i)
		{
			if (pVal->m_arDataPt[i])
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExSeriesDATAPT);
				WriteCT_ChartExDataPoint(pVal->m_arDataPt[i]);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (pVal->m_dataLabels.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExSeriesDATALABELS);
			WriteCT_ChartExDataLabels(pVal->m_dataLabels.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_layoutPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExSeriesLAYOUTPROPS);
			WriteCT_ChartExSeriesLayoutProperties(pVal->m_layoutPr.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_tx.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExSeriesTEXT);
			WriteCT_ChartExText(pVal->m_tx.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		for (size_t i = 0; i < pVal->m_arAxisId.size(); ++i)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExSeriesAXIS);
			m_oBcw.m_oStream.WriteULONG(pVal->m_arAxisId[i]);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if ((pVal->m_dataId.IsInit()) && (pVal->m_dataId->m_oVal.IsInit()))
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExSeriesDATAID);
			m_oBcw.m_oStream.WriteLONG(*(pVal->m_dataId->m_oVal));
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExSeriesSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, pVal->m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_oLayoutId.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExSeriesLAYOUTID);
			m_oBcw.m_oStream.WriteBYTE(pVal->m_oLayoutId->GetValue());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_bHidden.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExSeriesHIDDEN);
			m_oBcw.m_oStream.WriteBOOL(*pVal->m_bHidden);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_nOwnerIdx.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExSeriesOWNERIDX);
			m_oBcw.m_oStream.WriteLONG(*pVal->m_nOwnerIdx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_nFormatIdx.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExSeriesFORMATIDX);
			m_oBcw.m_oStream.WriteLONG(*pVal->m_nFormatIdx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_sUniqueId.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExSeriesUNIQUEID);
			m_oBcw.m_oStream.WriteStringW4(*pVal->m_sUniqueId);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ChartExDataPoint(OOX::Spreadsheet::ChartEx::CDataPoint *pVal)
	{
		if (pVal->m_idx.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataPointIDX);
			m_oBcw.m_oStream.WriteLONG(*pVal->m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataPointSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, pVal->m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ChartExDataLabels(OOX::Spreadsheet::ChartEx::CDataLabels *pVal)
	{
		if (pVal->m_pos.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataLabelsPOS);
			m_oBcw.m_oStream.WriteBYTE(pVal->m_pos->GetValue());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_numFmt.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataLabelsNUMFMT);
			WriteCT_ChartExNumberFormat(pVal->m_numFmt.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_txPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataLabelsTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, pVal->m_txPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataLabelsSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, pVal->m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_visibility.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataLabelsVISABILITIES);
			WriteCT_ChartExDataLabelVisibilities(pVal->m_visibility.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_separator.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataLabelsSEPARATOR);
			m_oBcw.m_oStream.WriteStringW4(*pVal->m_separator);
			m_oBcw.WriteItemEnd(nCurPos);
		}	
		for (size_t i = 0; i < pVal->m_arDataLabel.size(); ++i)
		{
			if (pVal->m_arDataLabel[i])
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataLabelsDATALABEL);
				WriteCT_ChartExDataLabel(pVal->m_arDataLabel[i]);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		for (size_t i = 0; i < pVal->m_arDataLabelHidden.size(); ++i)
		{
			if (pVal->m_arDataLabelHidden[i])
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataLabelsDATALABELHIDDEN);
				WriteCT_ChartExDataLabelHidden(pVal->m_arDataLabelHidden[i]);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
	}
	void BinaryChartWriter::WriteCT_ChartExDataLabel(OOX::Spreadsheet::ChartEx::CDataLabel *pVal)
	{
		if (pVal->m_idx.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataLabelIDX);
			m_oBcw.m_oStream.WriteLONG(*pVal->m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_dataLabelPos.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataLabelPOS);
			m_oBcw.m_oStream.WriteBYTE(pVal->m_dataLabelPos->GetValue());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_numFmt.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataLabelNUMFMT);
			WriteCT_ChartExNumberFormat(pVal->m_numFmt.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_txPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataLabelTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, pVal->m_txPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataLabelSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, pVal->m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_separator.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataLabelSEPARATOR);
			m_oBcw.m_oStream.WriteStringW4(*pVal->m_separator);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_visibility.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataLabelVISABILITIES);
			WriteCT_ChartExDataLabelVisibilities(pVal->m_visibility.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ChartExDataLabelHidden(OOX::Spreadsheet::ChartEx::CDataLabelHidden *pVal)
	{
		if (pVal->m_idx.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataLabelHiddenIDX);
			m_oBcw.m_oStream.WriteLONG(*pVal->m_idx);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ChartExSeriesLayoutProperties(OOX::Spreadsheet::ChartEx::CSeriesLayoutProperties *pVal)
	{
		if ((pVal->m_parentLabelLayout.IsInit()) && (pVal->m_parentLabelLayout->m_oVal.IsInit()))
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExSeriesLayoutPARENT);
			m_oBcw.m_oStream.WriteBYTE(pVal->m_parentLabelLayout->m_oVal->GetValue());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if ((pVal->m_regionLabelLayout.IsInit()) && (pVal->m_regionLabelLayout->m_oVal.IsInit()))
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExSeriesLayoutREGION);
			m_oBcw.m_oStream.WriteBYTE(pVal->m_regionLabelLayout->m_oVal->GetValue());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_visibility.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExSeriesLayoutVISABILITIES);
			WriteCT_ChartExSeriesElementVisibilities(pVal->m_visibility.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_aggregation.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExSeriesLayoutAGGREGATION);
			m_oBcw.m_oStream.WriteBOOL(*pVal->m_aggregation);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_binning.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExSeriesLayoutBINNING);
			WriteCT_ChartExBinning(pVal->m_binning.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_statistics.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExSeriesLayoutSTATISTIC);
			WriteCT_ChartExStatistics(pVal->m_statistics.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_subtotals.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExSeriesLayoutSUBTOTALS);
			WriteCT_ChartExSubtotals(pVal->m_subtotals.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ChartExNumberFormat(OOX::Spreadsheet::ChartEx::CNumberFormat *pVal)
	{
		if (pVal->m_formatCode.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExNumberFormatFORMATCODE);
			m_oBcw.m_oStream.WriteStringW4(*pVal->m_formatCode);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_sourceLinked.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExNumberFormatSOURCELINKED);
			m_oBcw.m_oStream.WriteBOOL(*pVal->m_sourceLinked);
			m_oBcw.WriteItemEnd(nCurPos);
		}	
	}
	void BinaryChartWriter::WriteCT_ChartExDataLabelVisibilities(OOX::Spreadsheet::ChartEx::CDataLabelVisibilities *pVal)
	{
		if (pVal->m_seriesName.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataLabelVisibilitiesSERIES);
			m_oBcw.m_oStream.WriteBOOL(*pVal->m_seriesName);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_categoryName.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataLabelVisibilitiesCATEGORY);
			m_oBcw.m_oStream.WriteBOOL(*pVal->m_categoryName);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_value.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExDataLabelVisibilitiesVALUE);
			m_oBcw.m_oStream.WriteBOOL(*pVal->m_value);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ChartExBinning(OOX::Spreadsheet::ChartEx::CBinning *pVal)
	{
		if ((pVal->m_binSize.IsInit()) && (pVal->m_binSize->m_oVal.IsInit()))
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExBinningBINSIZE);
			m_oBcw.m_oStream.WriteDoubleReal(pVal->m_binSize->m_oVal->GetValue());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if ((pVal->m_binCount.IsInit()) && (pVal->m_binCount->m_oVal.IsInit()))
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExBinningBINCOUNT);
			m_oBcw.m_oStream.WriteLONG(*pVal->m_binCount->m_oVal);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_intervalClosed.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExBinningINTERVAL);
			m_oBcw.m_oStream.WriteBYTE(pVal->m_intervalClosed->GetValue());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_underflow.IsInit())
		{
			if (pVal->m_underflow->GetValue() == SimpleTypes::Spreadsheet::typeAuto)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExBinningUNDERAUTO);
				m_oBcw.m_oStream.WriteBYTE(pVal->m_underflow->GetValue());
				m_oBcw.WriteItemEnd(nCurPos);
			}
			else
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExBinningUNDERVAL);
				m_oBcw.m_oStream.WriteDoubleReal(pVal->m_underflow->GetDoubleValue());
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (pVal->m_overflow.IsInit())
		{
			if (pVal->m_overflow->GetValue() == SimpleTypes::Spreadsheet::typeAuto)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExBinningOVERAUTO);
				m_oBcw.m_oStream.WriteBYTE(pVal->m_overflow->GetValue());
				m_oBcw.WriteItemEnd(nCurPos);
			}
			else
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExBinningOVERVAL);
				m_oBcw.m_oStream.WriteDoubleReal(pVal->m_overflow->GetDoubleValue());
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
	}
	void BinaryChartWriter::WriteCT_ChartExStatistics(OOX::Spreadsheet::ChartEx::CStatistics *pVal)
	{
		if (pVal->m_quartileMethod.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExStatisticsMETHOD);
			m_oBcw.m_oStream.WriteBYTE(pVal->m_quartileMethod->GetValue());
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ChartExSubtotals(OOX::Spreadsheet::ChartEx::CSubtotals *pVal)
	{
		for (size_t i = 0; i < pVal->m_arIdx.size(); ++i)
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExSubtotalsIDX);
			m_oBcw.m_oStream.WriteULONG(pVal->m_arIdx[i]);
			m_oBcw.WriteItemEnd(nCurPos);
		}

	}
	void BinaryChartWriter::WriteCT_ChartExSeriesElementVisibilities(OOX::Spreadsheet::ChartEx::CSeriesElementVisibilities *pVal)
	{
		if (pVal->m_connectorLines.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExSeriesVisibilitiesCONNECTOR);
			m_oBcw.m_oStream.WriteBOOL(*pVal->m_connectorLines);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_meanLine.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExSeriesVisibilitiesMEANLINE);
			m_oBcw.m_oStream.WriteBOOL(*pVal->m_meanLine);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_meanMarker.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExSeriesVisibilitiesMEANMARKER);
			m_oBcw.m_oStream.WriteBOOL(*pVal->m_meanMarker);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_nonoutliers.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExSeriesVisibilitiesNONOUTLIERS);
			m_oBcw.m_oStream.WriteBOOL(*pVal->m_nonoutliers);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_outliers.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExSeriesVisibilitiesOUTLIERS);
			m_oBcw.m_oStream.WriteBOOL(*pVal->m_outliers);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ChartExCatScaling(OOX::Spreadsheet::ChartEx::CCatScaling *pVal)
	{
		if (!pVal) return;

		if (pVal->m_gapWidth.IsInit())
		{
			if (pVal->m_gapWidth->GetValue() == SimpleTypes::Spreadsheet::typeAuto)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExCatScalingGAPAUTO);
				m_oBcw.m_oStream.WriteBYTE(pVal->m_gapWidth->GetValue());
				m_oBcw.WriteItemEnd(nCurPos);
			}
			else
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExCatScalingGAPVAL);
				m_oBcw.m_oStream.WriteDoubleReal(pVal->m_gapWidth->GetDoubleValue());
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
	}
	void BinaryChartWriter::WriteCT_ChartExValScaling(OOX::Spreadsheet::ChartEx::CValScaling *pVal)
	{
		if (!pVal) return;

		if (pVal->m_max.IsInit())
		{
			if (pVal->m_max->GetValue() == SimpleTypes::Spreadsheet::typeAuto)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExValScalingMAXAUTO);
				m_oBcw.m_oStream.WriteBYTE(pVal->m_max->GetValue());
				m_oBcw.WriteItemEnd(nCurPos);
			}
			else
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExValScalingMAXVAL);
				m_oBcw.m_oStream.WriteDoubleReal(pVal->m_max->GetDoubleValue());
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (pVal->m_min.IsInit())
		{
			if (pVal->m_min->GetValue() == SimpleTypes::Spreadsheet::typeAuto)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExValScalingMINAUTO);
				m_oBcw.m_oStream.WriteBYTE(pVal->m_min->GetValue());
				m_oBcw.WriteItemEnd(nCurPos);
			}
			else
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExValScalingMINVAL);
				m_oBcw.m_oStream.WriteDoubleReal(pVal->m_min->GetDoubleValue());
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (pVal->m_majorUnit.IsInit())
		{
			if (pVal->m_majorUnit->GetValue() == SimpleTypes::Spreadsheet::typeAuto)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExValScalingMAJUNITAUTO);
				m_oBcw.m_oStream.WriteBYTE(pVal->m_majorUnit->GetValue());
				m_oBcw.WriteItemEnd(nCurPos);
			}
			else
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExValScalingMAJUNITVAL);
				m_oBcw.m_oStream.WriteDoubleReal(pVal->m_majorUnit->GetDoubleValue());
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		if (pVal->m_minorUnit.IsInit())
		{
			if (pVal->m_minorUnit->GetValue() == SimpleTypes::Spreadsheet::typeAuto)
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExValScalingMINUNITAUTO);
				m_oBcw.m_oStream.WriteBYTE(pVal->m_minorUnit->GetValue());
				m_oBcw.WriteItemEnd(nCurPos);
			}
			else
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExValScalingMINUNITVAL);
				m_oBcw.m_oStream.WriteDoubleReal(pVal->m_minorUnit->GetDoubleValue());
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
	}
	void BinaryChartWriter::WriteCT_ChartExAxisUnit(OOX::Spreadsheet::ChartEx::CAxisUnit *pVal)
	{
		if (!pVal) return;

		if (pVal->m_unitsLabel.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExAxisUnitLABEL);
			WriteCT_ChartExAxisUnitsLabel(pVal->m_unitsLabel.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_unit.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExAxisUnitTYPE);
			m_oBcw.m_oStream.WriteBYTE(pVal->m_unit->GetValue());
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ChartExAxisUnitsLabel(OOX::Spreadsheet::ChartEx::CAxisUnitsLabel *pVal)
	{
		if (!pVal) return;
		if (pVal->m_tx.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExAxisUnitsLabelTEXT);
			WriteCT_ChartExText(pVal->m_tx.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExAxisUnitsLabelSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, pVal->m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_txPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExAxisUnitsLabelTXPR);
			m_oBcw.m_oStream.WriteRecord2(0, pVal->m_txPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ChartExTickMarks(OOX::Spreadsheet::ChartEx::CTickMarks *pVal)
	{
		if (!pVal) return;

		if (pVal->m_type.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExTickMarksTYPE);
			m_oBcw.m_oStream.WriteBYTE(pVal->m_type->GetValue());
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteCT_ChartExGridlines(OOX::Spreadsheet::ChartEx::CGridlines *pVal)
	{
		if (!pVal) return;

		if (pVal->m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_chartExGridlinesSPPR);
			m_oBcw.m_oStream.WriteRecord2(0, pVal->m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	void BinaryChartWriter::WriteChartFiltering(OOX::Spreadsheet::CSeriesFiltering *pVal)
	{
		if (!pVal) return;

		WriteDataLabelsRange(pVal->m_dataLabelsRange.GetPointer());

		WriteFilteredSeriesTitle(pVal->m_filteredSeriesTitle.GetPointer());
		WriteFilteredCategoryTitle(pVal->m_filteredCategoryTitle.GetPointer());

		WriteFilteredLineSeries(pVal->m_filteredLineSeries.GetPointer());
		WriteFilteredScatterSeries(pVal->m_filteredScatterSeries.GetPointer());
		WriteFilteredRadarSeries(pVal->m_filteredRadarSeries.GetPointer());
		WriteFilteredBarSeries(pVal->m_filteredBarSeries.GetPointer());
		WriteFilteredAreaSeries(pVal->m_filteredAreaSeries.GetPointer());
		WriteFilteredBubbleSeries(pVal->m_filteredBubbleSeries.GetPointer());
		WriteFilteredSurfaceSeries(pVal->m_filteredSurfaceSeries.GetPointer());
		WriteFilteredPieSeries(pVal->m_filteredPieSeries.GetPointer());

		WriteFullRef(pVal->m_fullRef.get_value_or(L""));
		WriteLevelRef(pVal->m_levelRef.get_value_or(L""));
		WriteFormulaRef(pVal->m_formulaRef.get_value_or(L""));

		WriteCategoryFilterExceptions(pVal->m_categoryFilterExceptions.GetPointer());
	}
	void BinaryChartWriter::WriteDataLabelsRange(OOX::Spreadsheet::CSeriesDataLabelsRange *pVal)
	{
		if (!pVal) return;
	
		int nCurPos2 = m_oBcw.WriteItemStart(c_oserct_dataLabelsRange);
		if (pVal->m_f.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dataLabelsRangeFormula);
			m_oBcw.m_oStream.WriteStringW4(*pVal->m_f);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_dlblRangeCache.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_dataLabelsRangeCache);
			WriteCT_StrData(*pVal->m_dlblRangeCache);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		m_oBcw.WriteItemEnd(nCurPos2);
	}
	void BinaryChartWriter::WriteFilteredSeriesTitle(OOX::Spreadsheet::CT_Tx *pVal)
	{
		if (!pVal) return;
		
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_filteredSeriesTitle);
		WriteCT_Tx(*pVal);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteFilteredCategoryTitle(OOX::Spreadsheet::CT_AxDataSource *pVal)
	{
		if (!pVal) return;
		
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_filteredCategoryTitle);
		WriteCT_AxDataSource(*pVal);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteFilteredLineSeries(OOX::Spreadsheet::CT_LineSer *pVal)
	{
		if (!pVal) return;

		int nCurPos = m_oBcw.WriteItemStart(c_oserct_filteredLineSeries);
		WriteCT_LineSer(*pVal);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteFilteredScatterSeries(OOX::Spreadsheet::CT_ScatterSer *pVal)
	{
		if (!pVal) return;

		int nCurPos = m_oBcw.WriteItemStart(c_oserct_filteredScatterSeries);
		WriteCT_ScatterSer(*pVal);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteFilteredRadarSeries(OOX::Spreadsheet::CT_RadarSer *pVal)
	{
		if (!pVal) return;

		int nCurPos = m_oBcw.WriteItemStart(c_oserct_filteredRadarSeries);
		WriteCT_RadarSer(*pVal);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteFilteredBarSeries(OOX::Spreadsheet::CT_BarSer *pVal)
	{
		if (!pVal) return;

		int nCurPos = m_oBcw.WriteItemStart(c_oserct_filteredBarSeries);
		WriteCT_BarSer(*pVal);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteFilteredAreaSeries(OOX::Spreadsheet::CT_AreaSer *pVal)
	{
		if (!pVal) return;

		int nCurPos = m_oBcw.WriteItemStart(c_oserct_filteredAreaSeries);
		WriteCT_AreaSer(*pVal);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteFilteredBubbleSeries(OOX::Spreadsheet::CT_BubbleSer *pVal)
	{
		if (!pVal) return;

		int nCurPos = m_oBcw.WriteItemStart(c_oserct_filteredBubbleSeries);
		WriteCT_BubbleSer(*pVal);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteFilteredSurfaceSeries(OOX::Spreadsheet::CT_SurfaceSer *pVal)
	{
		if (!pVal) return;

		int nCurPos = m_oBcw.WriteItemStart(c_oserct_filteredSurfaceSeries);
		WriteCT_SurfaceSer(*pVal);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteFilteredPieSeries(OOX::Spreadsheet::CT_PieSer *pVal)
	{
		if (!pVal) return;
		
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_filteredPieSeries);
		WriteCT_PieSer(*pVal);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteFullRef(const std::wstring & ref)
	{
		if (ref.empty()) return;
		
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_fullRef);
		m_oBcw.m_oStream.WriteStringW4(ref);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteLevelRef(const std::wstring & ref)
	{
		if (ref.empty()) return;
		
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_levelRef);
		m_oBcw.m_oStream.WriteStringW4(ref);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteFormulaRef(const std::wstring & ref)
	{
		if (ref.empty()) return;
		
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_formulaRef);
		m_oBcw.m_oStream.WriteStringW4(ref);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCategoryFilterExceptions(OOX::Spreadsheet::CCategoryFilterExceptions *pVal)
	{
		if (!pVal) return;
		
		int nCurPos = m_oBcw.WriteItemStart(c_oserct_categoryFilterExceptions);
		for (size_t i = 0; i < pVal->m_arrItems.size(); ++i)
		{
			int nCurPos1 = m_oBcw.WriteItemStart(c_oserct_categoryFilterException);
				WriteCategoryFilterException(pVal->m_arrItems[i]);
			m_oBcw.WriteItemEnd(nCurPos1);
		}
		m_oBcw.WriteItemEnd(nCurPos);
	}
	void BinaryChartWriter::WriteCategoryFilterException(OOX::Spreadsheet::CCategoryFilterException *pVal)
	{
		if (!pVal) return;
		
		if (pVal->m_sqref.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_filterSqref);
			m_oBcw.m_oStream.WriteStringW4(*pVal->m_sqref);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_spPr.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_filterSpPr);
			m_oBcw.m_oStream.WriteRecord2(0, pVal->m_spPr);
			m_oBcw.WriteItemEnd(nCurPos);

		}
		if (pVal->m_explosion.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_filterExplosion);
			WriteCT_UnsignedInt(*pVal->m_explosion);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_invertIfNegative.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_filterInvertIfNegative);
			WriteCT_Boolean(*pVal->m_invertIfNegative);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_bubble3D.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_filterBubble3D);
			WriteCT_Boolean(*pVal->m_bubble3D);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_marker.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_filterMarker);
			WriteCT_Marker(*pVal->m_marker);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if (pVal->m_dLbl.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oserct_filterLbl);
			WriteCT_DLbl(*pVal->m_dLbl);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
}
