//Generated code
#pragma once
#include "CommonWriter.h"
#include "..\..\Common\DocxFormat\Source\XlsxFormat\Chart\ChartSerialize.h"
#include "..\Writer\BinaryCommonReader.h"
using namespace OOX::Spreadsheet;
namespace BinXlsxRW {

	extern BYTE c_oserct_chartspaceDATE1904;
	extern BYTE c_oserct_chartspaceLANG;
	extern BYTE c_oserct_chartspaceROUNDEDCORNERS;
	extern BYTE c_oserct_chartspaceALTERNATECONTENT;
	extern BYTE c_oserct_chartspaceSTYLE;
	extern BYTE c_oserct_chartspaceCLRMAPOVR;
	extern BYTE c_oserct_chartspacePIVOTSOURCE;
	extern BYTE c_oserct_chartspacePROTECTION;
	extern BYTE c_oserct_chartspaceCHART;
	extern BYTE c_oserct_chartspaceSPPR;
	extern BYTE c_oserct_chartspaceTXPR;
	extern BYTE c_oserct_chartspaceEXTERNALDATA;
	extern BYTE c_oserct_chartspacePRINTSETTINGS;
	extern BYTE c_oserct_chartspaceUSERSHAPES;
	extern BYTE c_oserct_chartspaceEXTLST;

	extern BYTE c_oserct_booleanVAL;

	extern BYTE c_oserct_relidID;

	extern BYTE c_oserct_pagesetupPAPERSIZE;
	extern BYTE c_oserct_pagesetupPAPERHEIGHT;
	extern BYTE c_oserct_pagesetupPAPERWIDTH;
	extern BYTE c_oserct_pagesetupFIRSTPAGENUMBER;
	extern BYTE c_oserct_pagesetupORIENTATION;
	extern BYTE c_oserct_pagesetupBLACKANDWHITE;
	extern BYTE c_oserct_pagesetupDRAFT;
	extern BYTE c_oserct_pagesetupUSEFIRSTPAGENUMBER;
	extern BYTE c_oserct_pagesetupHORIZONTALDPI;
	extern BYTE c_oserct_pagesetupVERTICALDPI;
	extern BYTE c_oserct_pagesetupCOPIES;

	extern BYTE c_oserct_pagemarginsL;
	extern BYTE c_oserct_pagemarginsR;
	extern BYTE c_oserct_pagemarginsT;
	extern BYTE c_oserct_pagemarginsB;
	extern BYTE c_oserct_pagemarginsHEADER;
	extern BYTE c_oserct_pagemarginsFOOTER;

	extern BYTE c_oserct_headerfooterODDHEADER;
	extern BYTE c_oserct_headerfooterODDFOOTER;
	extern BYTE c_oserct_headerfooterEVENHEADER;
	extern BYTE c_oserct_headerfooterEVENFOOTER;
	extern BYTE c_oserct_headerfooterFIRSTHEADER;
	extern BYTE c_oserct_headerfooterFIRSTFOOTER;
	extern BYTE c_oserct_headerfooterALIGNWITHMARGINS;
	extern BYTE c_oserct_headerfooterDIFFERENTODDEVEN;
	extern BYTE c_oserct_headerfooterDIFFERENTFIRST;

	extern BYTE c_oserct_printsettingsHEADERFOOTER;
	extern BYTE c_oserct_printsettingsPAGEMARGINS;
	extern BYTE c_oserct_printsettingsPAGESETUP;

	extern BYTE c_oserct_externaldataAUTOUPDATE;
	extern BYTE c_oserct_externaldataID;

	extern BYTE c_oserct_dispblanksasVAL;

	extern BYTE c_oserct_legendentryIDX;
	extern BYTE c_oserct_legendentryDELETE;
	extern BYTE c_oserct_legendentryTXPR;
	extern BYTE c_oserct_legendentryEXTLST;

	extern BYTE c_oserct_unsignedintVAL;

	extern BYTE c_oserct_extensionANY;
	extern BYTE c_oserct_extensionURI;

	extern BYTE c_oserct_legendposVAL;

	extern BYTE c_oserct_legendLEGENDPOS;
	extern BYTE c_oserct_legendLEGENDENTRY;
	extern BYTE c_oserct_legendLAYOUT;
	extern BYTE c_oserct_legendOVERLAY;
	extern BYTE c_oserct_legendSPPR;
	extern BYTE c_oserct_legendTXPR;
	extern BYTE c_oserct_legendEXTLST;

	extern BYTE c_oserct_layoutMANUALLAYOUT;
	extern BYTE c_oserct_layoutEXTLST;

	extern BYTE c_oserct_manuallayoutLAYOUTTARGET;
	extern BYTE c_oserct_manuallayoutXMODE;
	extern BYTE c_oserct_manuallayoutYMODE;
	extern BYTE c_oserct_manuallayoutWMODE;
	extern BYTE c_oserct_manuallayoutHMODE;
	extern BYTE c_oserct_manuallayoutX;
	extern BYTE c_oserct_manuallayoutY;
	extern BYTE c_oserct_manuallayoutW;
	extern BYTE c_oserct_manuallayoutH;
	extern BYTE c_oserct_manuallayoutEXTLST;

	extern BYTE c_oserct_layouttargetVAL;

	extern BYTE c_oserct_layoutmodeVAL;

	extern BYTE c_oserct_doubleVAL;

	extern BYTE c_oserct_dtableSHOWHORZBORDER;
	extern BYTE c_oserct_dtableSHOWVERTBORDER;
	extern BYTE c_oserct_dtableSHOWOUTLINE;
	extern BYTE c_oserct_dtableSHOWKEYS;
	extern BYTE c_oserct_dtableSPPR;
	extern BYTE c_oserct_dtableTXPR;
	extern BYTE c_oserct_dtableEXTLST;

	extern BYTE c_oserct_seraxAXID;
	extern BYTE c_oserct_seraxSCALING;
	extern BYTE c_oserct_seraxDELETE;
	extern BYTE c_oserct_seraxAXPOS;
	extern BYTE c_oserct_seraxMAJORGRIDLINES;
	extern BYTE c_oserct_seraxMINORGRIDLINES;
	extern BYTE c_oserct_seraxTITLE;
	extern BYTE c_oserct_seraxNUMFMT;
	extern BYTE c_oserct_seraxMAJORTICKMARK;
	extern BYTE c_oserct_seraxMINORTICKMARK;
	extern BYTE c_oserct_seraxTICKLBLPOS;
	extern BYTE c_oserct_seraxSPPR;
	extern BYTE c_oserct_seraxTXPR;
	extern BYTE c_oserct_seraxCROSSAX;
	extern BYTE c_oserct_seraxCROSSES;
	extern BYTE c_oserct_seraxCROSSESAT;
	extern BYTE c_oserct_seraxTICKLBLSKIP;
	extern BYTE c_oserct_seraxTICKMARKSKIP;
	extern BYTE c_oserct_seraxEXTLST;

	extern BYTE c_oserct_scalingLOGBASE;
	extern BYTE c_oserct_scalingORIENTATION;
	extern BYTE c_oserct_scalingMAX;
	extern BYTE c_oserct_scalingMIN;
	extern BYTE c_oserct_scalingEXTLST;

	extern BYTE c_oserct_logbaseVAL;

	extern BYTE c_oserct_orientationVAL;

	extern BYTE c_oserct_axposVAL;

	extern BYTE c_oserct_chartlinesSPPR;

	extern BYTE c_oserct_titleTX;
	extern BYTE c_oserct_titleLAYOUT;
	extern BYTE c_oserct_titleOVERLAY;
	extern BYTE c_oserct_titleSPPR;
	extern BYTE c_oserct_titleTXPR;
	extern BYTE c_oserct_titleEXTLST;

	extern BYTE c_oserct_txRICH;
	extern BYTE c_oserct_txSTRREF;

	extern BYTE c_oserct_strrefF;
	extern BYTE c_oserct_strrefSTRCACHE;
	extern BYTE c_oserct_strrefEXTLST;

	extern BYTE c_oserct_strdataPTCOUNT;
	extern BYTE c_oserct_strdataPT;
	extern BYTE c_oserct_strdataEXTLST;

	extern BYTE c_oserct_strvalV;
	extern BYTE c_oserct_strvalIDX;

	extern BYTE c_oserct_numfmtFORMATCODE;
	extern BYTE c_oserct_numfmtSOURCELINKED;

	extern BYTE c_oserct_tickmarkVAL;

	extern BYTE c_oserct_ticklblposVAL;

	extern BYTE c_oserct_crossesVAL;

	extern BYTE c_oserct_skipVAL;

	extern BYTE c_oserct_timeunitVAL;

	extern BYTE c_oserct_dateaxAXID;
	extern BYTE c_oserct_dateaxSCALING;
	extern BYTE c_oserct_dateaxDELETE;
	extern BYTE c_oserct_dateaxAXPOS;
	extern BYTE c_oserct_dateaxMAJORGRIDLINES;
	extern BYTE c_oserct_dateaxMINORGRIDLINES;
	extern BYTE c_oserct_dateaxTITLE;
	extern BYTE c_oserct_dateaxNUMFMT;
	extern BYTE c_oserct_dateaxMAJORTICKMARK;
	extern BYTE c_oserct_dateaxMINORTICKMARK;
	extern BYTE c_oserct_dateaxTICKLBLPOS;
	extern BYTE c_oserct_dateaxSPPR;
	extern BYTE c_oserct_dateaxTXPR;
	extern BYTE c_oserct_dateaxCROSSAX;
	extern BYTE c_oserct_dateaxCROSSES;
	extern BYTE c_oserct_dateaxCROSSESAT;
	extern BYTE c_oserct_dateaxAUTO;
	extern BYTE c_oserct_dateaxLBLOFFSET;
	extern BYTE c_oserct_dateaxBASETIMEUNIT;
	extern BYTE c_oserct_dateaxMAJORUNIT;
	extern BYTE c_oserct_dateaxMAJORTIMEUNIT;
	extern BYTE c_oserct_dateaxMINORUNIT;
	extern BYTE c_oserct_dateaxMINORTIMEUNIT;
	extern BYTE c_oserct_dateaxEXTLST;

	extern BYTE c_oserct_lbloffsetVAL;

	extern BYTE c_oserct_axisunitVAL;

	extern BYTE c_oserct_lblalgnVAL;

	extern BYTE c_oserct_cataxAXID;
	extern BYTE c_oserct_cataxSCALING;
	extern BYTE c_oserct_cataxDELETE;
	extern BYTE c_oserct_cataxAXPOS;
	extern BYTE c_oserct_cataxMAJORGRIDLINES;
	extern BYTE c_oserct_cataxMINORGRIDLINES;
	extern BYTE c_oserct_cataxTITLE;
	extern BYTE c_oserct_cataxNUMFMT;
	extern BYTE c_oserct_cataxMAJORTICKMARK;
	extern BYTE c_oserct_cataxMINORTICKMARK;
	extern BYTE c_oserct_cataxTICKLBLPOS;
	extern BYTE c_oserct_cataxSPPR;
	extern BYTE c_oserct_cataxTXPR;
	extern BYTE c_oserct_cataxCROSSAX;
	extern BYTE c_oserct_cataxCROSSES;
	extern BYTE c_oserct_cataxCROSSESAT;
	extern BYTE c_oserct_cataxAUTO;
	extern BYTE c_oserct_cataxLBLALGN;
	extern BYTE c_oserct_cataxLBLOFFSET;
	extern BYTE c_oserct_cataxTICKLBLSKIP;
	extern BYTE c_oserct_cataxTICKMARKSKIP;
	extern BYTE c_oserct_cataxNOMULTILVLLBL;
	extern BYTE c_oserct_cataxEXTLST;

	extern BYTE c_oserct_dispunitslblLAYOUT;
	extern BYTE c_oserct_dispunitslblTX;
	extern BYTE c_oserct_dispunitslblSPPR;
	extern BYTE c_oserct_dispunitslblTXPR;

	extern BYTE c_oserct_builtinunitVAL;

	extern BYTE c_oserct_dispunitsBUILTINUNIT;
	extern BYTE c_oserct_dispunitsCUSTUNIT;
	extern BYTE c_oserct_dispunitsDISPUNITSLBL;
	extern BYTE c_oserct_dispunitsEXTLST;

	extern BYTE c_oserct_crossbetweenVAL;

	extern BYTE c_oserct_valaxAXID;
	extern BYTE c_oserct_valaxSCALING;
	extern BYTE c_oserct_valaxDELETE;
	extern BYTE c_oserct_valaxAXPOS;
	extern BYTE c_oserct_valaxMAJORGRIDLINES;
	extern BYTE c_oserct_valaxMINORGRIDLINES;
	extern BYTE c_oserct_valaxTITLE;
	extern BYTE c_oserct_valaxNUMFMT;
	extern BYTE c_oserct_valaxMAJORTICKMARK;
	extern BYTE c_oserct_valaxMINORTICKMARK;
	extern BYTE c_oserct_valaxTICKLBLPOS;
	extern BYTE c_oserct_valaxSPPR;
	extern BYTE c_oserct_valaxTXPR;
	extern BYTE c_oserct_valaxCROSSAX;
	extern BYTE c_oserct_valaxCROSSES;
	extern BYTE c_oserct_valaxCROSSESAT;
	extern BYTE c_oserct_valaxCROSSBETWEEN;
	extern BYTE c_oserct_valaxMAJORUNIT;
	extern BYTE c_oserct_valaxMINORUNIT;
	extern BYTE c_oserct_valaxDISPUNITS;
	extern BYTE c_oserct_valaxEXTLST;

	extern BYTE c_oserct_sizerepresentsVAL;

	extern BYTE c_oserct_bubblescaleVAL;

	extern BYTE c_oserct_bubbleserIDX;
	extern BYTE c_oserct_bubbleserORDER;
	extern BYTE c_oserct_bubbleserTX;
	extern BYTE c_oserct_bubbleserSPPR;
	extern BYTE c_oserct_bubbleserINVERTIFNEGATIVE;
	extern BYTE c_oserct_bubbleserDPT;
	extern BYTE c_oserct_bubbleserDLBLS;
	extern BYTE c_oserct_bubbleserTRENDLINE;
	extern BYTE c_oserct_bubbleserERRBARS;
	extern BYTE c_oserct_bubbleserXVAL;
	extern BYTE c_oserct_bubbleserYVAL;
	extern BYTE c_oserct_bubbleserBUBBLESIZE;
	extern BYTE c_oserct_bubbleserBUBBLE3D;
	extern BYTE c_oserct_bubbleserEXTLST;

	extern BYTE c_oserct_sertxSTRREF;
	extern BYTE c_oserct_sertxV;

	extern BYTE c_oserct_dptIDX;
	extern BYTE c_oserct_dptINVERTIFNEGATIVE;
	extern BYTE c_oserct_dptMARKER;
	extern BYTE c_oserct_dptBUBBLE3D;
	extern BYTE c_oserct_dptEXPLOSION;
	extern BYTE c_oserct_dptSPPR;
	extern BYTE c_oserct_dptPICTUREOPTIONS;
	extern BYTE c_oserct_dptEXTLST;

	extern BYTE c_oserct_markerSYMBOL;
	extern BYTE c_oserct_markerSIZE;
	extern BYTE c_oserct_markerSPPR;
	extern BYTE c_oserct_markerEXTLST;

	extern BYTE c_oserct_markerstyleVAL;

	extern BYTE c_oserct_markersizeVAL;

	extern BYTE c_oserct_pictureoptionsAPPLYTOFRONT;
	extern BYTE c_oserct_pictureoptionsAPPLYTOSIDES;
	extern BYTE c_oserct_pictureoptionsAPPLYTOEND;
	extern BYTE c_oserct_pictureoptionsPICTUREFORMAT;
	extern BYTE c_oserct_pictureoptionsPICTURESTACKUNIT;

	extern BYTE c_oserct_pictureformatVAL;

	extern BYTE c_oserct_picturestackunitVAL;

	extern BYTE c_oserct_dlblsDLBL;
	extern BYTE c_oserct_dlblsITEMS;
	extern BYTE c_oserct_dlblsDLBLPOS;
	extern BYTE c_oserct_dlblsDELETE;
	extern BYTE c_oserct_dlblsLEADERLINES;
	extern BYTE c_oserct_dlblsNUMFMT;
	extern BYTE c_oserct_dlblsSEPARATOR;
	extern BYTE c_oserct_dlblsSHOWBUBBLESIZE;
	extern BYTE c_oserct_dlblsSHOWCATNAME;
	extern BYTE c_oserct_dlblsSHOWLEADERLINES;
	extern BYTE c_oserct_dlblsSHOWLEGENDKEY;
	extern BYTE c_oserct_dlblsSHOWPERCENT;
	extern BYTE c_oserct_dlblsSHOWSERNAME;
	extern BYTE c_oserct_dlblsSHOWVAL;
	extern BYTE c_oserct_dlblsSPPR;
	extern BYTE c_oserct_dlblsTXPR;
	extern BYTE c_oserct_dlblsEXTLST;

	extern BYTE c_oserct_dlblIDX;
	extern BYTE c_oserct_dlblITEMS;
	extern BYTE c_oserct_dlblDLBLPOS;
	extern BYTE c_oserct_dlblDELETE;
	extern BYTE c_oserct_dlblLAYOUT;
	extern BYTE c_oserct_dlblNUMFMT;
	extern BYTE c_oserct_dlblSEPARATOR;
	extern BYTE c_oserct_dlblSHOWBUBBLESIZE;
	extern BYTE c_oserct_dlblSHOWCATNAME;
	extern BYTE c_oserct_dlblSHOWLEGENDKEY;
	extern BYTE c_oserct_dlblSHOWPERCENT;
	extern BYTE c_oserct_dlblSHOWSERNAME;
	extern BYTE c_oserct_dlblSHOWVAL;
	extern BYTE c_oserct_dlblSPPR;
	extern BYTE c_oserct_dlblTX;
	extern BYTE c_oserct_dlblTXPR;
	extern BYTE c_oserct_dlblEXTLST;

	extern BYTE c_oserct_dlblposVAL;

	extern BYTE c_oserct_trendlineNAME;
	extern BYTE c_oserct_trendlineSPPR;
	extern BYTE c_oserct_trendlineTRENDLINETYPE;
	extern BYTE c_oserct_trendlineORDER;
	extern BYTE c_oserct_trendlinePERIOD;
	extern BYTE c_oserct_trendlineFORWARD;
	extern BYTE c_oserct_trendlineBACKWARD;
	extern BYTE c_oserct_trendlineINTERCEPT;
	extern BYTE c_oserct_trendlineDISPRSQR;
	extern BYTE c_oserct_trendlineDISPEQ;
	extern BYTE c_oserct_trendlineTRENDLINELBL;
	extern BYTE c_oserct_trendlineEXTLST;

	extern BYTE c_oserct_trendlinetypeVAL;

	extern BYTE c_oserct_orderVAL;

	extern BYTE c_oserct_periodVAL;

	extern BYTE c_oserct_trendlinelblLAYOUT;
	extern BYTE c_oserct_trendlinelblTX;
	extern BYTE c_oserct_trendlinelblNUMFMT;
	extern BYTE c_oserct_trendlinelblSPPR;
	extern BYTE c_oserct_trendlinelblTXPR;
	extern BYTE c_oserct_trendlinelblEXTLST;

	extern BYTE c_oserct_errbarsERRDIR;
	extern BYTE c_oserct_errbarsERRBARTYPE;
	extern BYTE c_oserct_errbarsERRVALTYPE;
	extern BYTE c_oserct_errbarsNOENDCAP;
	extern BYTE c_oserct_errbarsPLUS;
	extern BYTE c_oserct_errbarsMINUS;
	extern BYTE c_oserct_errbarsVAL;
	extern BYTE c_oserct_errbarsSPPR;
	extern BYTE c_oserct_errbarsEXTLST;

	extern BYTE c_oserct_errdirVAL;

	extern BYTE c_oserct_errbartypeVAL;

	extern BYTE c_oserct_errvaltypeVAL;

	extern BYTE c_oserct_numdatasourceNUMLIT;
	extern BYTE c_oserct_numdatasourceNUMREF;

	extern BYTE c_oserct_numdataFORMATCODE;
	extern BYTE c_oserct_numdataPTCOUNT;
	extern BYTE c_oserct_numdataPT;
	extern BYTE c_oserct_numdataEXTLST;

	extern BYTE c_oserct_numvalV;
	extern BYTE c_oserct_numvalIDX;
	extern BYTE c_oserct_numvalFORMATCODE;

	extern BYTE c_oserct_numrefF;
	extern BYTE c_oserct_numrefNUMCACHE;
	extern BYTE c_oserct_numrefEXTLST;

	extern BYTE c_oserct_axdatasourceMULTILVLSTRREF;
	extern BYTE c_oserct_axdatasourceNUMLIT;
	extern BYTE c_oserct_axdatasourceNUMREF;
	extern BYTE c_oserct_axdatasourceSTRLIT;
	extern BYTE c_oserct_axdatasourceSTRREF;

	extern BYTE c_oserct_multilvlstrrefF;
	extern BYTE c_oserct_multilvlstrrefMULTILVLSTRCACHE;
	extern BYTE c_oserct_multilvlstrrefEXTLST;

	extern BYTE c_oserct_multilvlstrdataPTCOUNT;
	extern BYTE c_oserct_multilvlstrdataLVL;
	extern BYTE c_oserct_multilvlstrdataEXTLST;

	extern BYTE c_oserct_bubblechartVARYCOLORS;
	extern BYTE c_oserct_bubblechartSER;
	extern BYTE c_oserct_bubblechartDLBLS;
	extern BYTE c_oserct_bubblechartBUBBLE3D;
	extern BYTE c_oserct_bubblechartBUBBLESCALE;
	extern BYTE c_oserct_bubblechartSHOWNEGBUBBLES;
	extern BYTE c_oserct_bubblechartSIZEREPRESENTS;
	extern BYTE c_oserct_bubblechartAXID;
	extern BYTE c_oserct_bubblechartEXTLST;

	extern BYTE c_oserct_surface3dchartWIREFRAME;
	extern BYTE c_oserct_surface3dchartSER;
	extern BYTE c_oserct_surface3dchartBANDFMTS;
	extern BYTE c_oserct_surface3dchartAXID;
	extern BYTE c_oserct_surface3dchartEXTLST;

	extern BYTE c_oserct_surfaceserIDX;
	extern BYTE c_oserct_surfaceserORDER;
	extern BYTE c_oserct_surfaceserTX;
	extern BYTE c_oserct_surfaceserSPPR;
	extern BYTE c_oserct_surfaceserCAT;
	extern BYTE c_oserct_surfaceserVAL;
	extern BYTE c_oserct_surfaceserEXTLST;

	extern BYTE c_oserct_bandfmtIDX;
	extern BYTE c_oserct_bandfmtSPPR;

	extern BYTE c_oserct_surfacechartWIREFRAME;
	extern BYTE c_oserct_surfacechartSER;
	extern BYTE c_oserct_surfacechartBANDFMTS;
	extern BYTE c_oserct_surfacechartAXID;
	extern BYTE c_oserct_surfacechartEXTLST;

	extern BYTE c_oserct_secondpiesizeVAL;

	extern BYTE c_oserct_splittypeVAL;

	extern BYTE c_oserct_ofpietypeVAL;

	extern BYTE c_oserct_ofpiechartOFPIETYPE;
	extern BYTE c_oserct_ofpiechartVARYCOLORS;
	extern BYTE c_oserct_ofpiechartSER;
	extern BYTE c_oserct_ofpiechartDLBLS;
	extern BYTE c_oserct_ofpiechartGAPWIDTH;
	extern BYTE c_oserct_ofpiechartSPLITTYPE;
	extern BYTE c_oserct_ofpiechartSPLITPOS;
	extern BYTE c_oserct_ofpiechartCUSTSPLIT;
	extern BYTE c_oserct_ofpiechartSECONDPIESIZE;
	extern BYTE c_oserct_ofpiechartSERLINES;
	extern BYTE c_oserct_ofpiechartEXTLST;

	extern BYTE c_oserct_pieserIDX;
	extern BYTE c_oserct_pieserORDER;
	extern BYTE c_oserct_pieserTX;
	extern BYTE c_oserct_pieserSPPR;
	extern BYTE c_oserct_pieserEXPLOSION;
	extern BYTE c_oserct_pieserDPT;
	extern BYTE c_oserct_pieserDLBLS;
	extern BYTE c_oserct_pieserCAT;
	extern BYTE c_oserct_pieserVAL;
	extern BYTE c_oserct_pieserEXTLST;

	extern BYTE c_oserct_gapamountVAL;

	extern BYTE c_oserct_bar3dchartBARDIR;
	extern BYTE c_oserct_bar3dchartGROUPING;
	extern BYTE c_oserct_bar3dchartVARYCOLORS;
	extern BYTE c_oserct_bar3dchartSER;
	extern BYTE c_oserct_bar3dchartDLBLS;
	extern BYTE c_oserct_bar3dchartGAPWIDTH;
	extern BYTE c_oserct_bar3dchartGAPDEPTH;
	extern BYTE c_oserct_bar3dchartSHAPE;
	extern BYTE c_oserct_bar3dchartAXID;
	extern BYTE c_oserct_bar3dchartEXTLST;

	extern BYTE c_oserct_bardirVAL;

	extern BYTE c_oserct_bargroupingVAL;

	extern BYTE c_oserct_barserIDX;
	extern BYTE c_oserct_barserORDER;
	extern BYTE c_oserct_barserTX;
	extern BYTE c_oserct_barserSPPR;
	extern BYTE c_oserct_barserINVERTIFNEGATIVE;
	extern BYTE c_oserct_barserPICTUREOPTIONS;
	extern BYTE c_oserct_barserDPT;
	extern BYTE c_oserct_barserDLBLS;
	extern BYTE c_oserct_barserTRENDLINE;
	extern BYTE c_oserct_barserERRBARS;
	extern BYTE c_oserct_barserCAT;
	extern BYTE c_oserct_barserVAL;
	extern BYTE c_oserct_barserSHAPE;
	extern BYTE c_oserct_barserEXTLST;

	extern BYTE c_oserct_shapeVAL;

	extern BYTE c_oserct_overlapVAL;

	extern BYTE c_oserct_barchartBARDIR;
	extern BYTE c_oserct_barchartGROUPING;
	extern BYTE c_oserct_barchartVARYCOLORS;
	extern BYTE c_oserct_barchartSER;
	extern BYTE c_oserct_barchartDLBLS;
	extern BYTE c_oserct_barchartGAPWIDTH;
	extern BYTE c_oserct_barchartOVERLAP;
	extern BYTE c_oserct_barchartSERLINES;
	extern BYTE c_oserct_barchartAXID;
	extern BYTE c_oserct_barchartEXTLST;

	extern BYTE c_oserct_holesizeVAL;

	extern BYTE c_oserct_doughnutchartVARYCOLORS;
	extern BYTE c_oserct_doughnutchartSER;
	extern BYTE c_oserct_doughnutchartDLBLS;
	extern BYTE c_oserct_doughnutchartFIRSTSLICEANG;
	extern BYTE c_oserct_doughnutchartHOLESIZE;
	extern BYTE c_oserct_doughnutchartEXTLST;

	extern BYTE c_oserct_firstsliceangVAL;

	extern BYTE c_oserct_pie3dchartVARYCOLORS;
	extern BYTE c_oserct_pie3dchartSER;
	extern BYTE c_oserct_pie3dchartDLBLS;
	extern BYTE c_oserct_pie3dchartEXTLST;

	extern BYTE c_oserct_piechartVARYCOLORS;
	extern BYTE c_oserct_piechartSER;
	extern BYTE c_oserct_piechartDLBLS;
	extern BYTE c_oserct_piechartFIRSTSLICEANG;
	extern BYTE c_oserct_piechartEXTLST;

	extern BYTE c_oserct_scatterserIDX;
	extern BYTE c_oserct_scatterserORDER;
	extern BYTE c_oserct_scatterserTX;
	extern BYTE c_oserct_scatterserSPPR;
	extern BYTE c_oserct_scatterserMARKER;
	extern BYTE c_oserct_scatterserDPT;
	extern BYTE c_oserct_scatterserDLBLS;
	extern BYTE c_oserct_scatterserTRENDLINE;
	extern BYTE c_oserct_scatterserERRBARS;
	extern BYTE c_oserct_scatterserXVAL;
	extern BYTE c_oserct_scatterserYVAL;
	extern BYTE c_oserct_scatterserSMOOTH;
	extern BYTE c_oserct_scatterserEXTLST;

	extern BYTE c_oserct_scatterstyleVAL;

	extern BYTE c_oserct_scatterchartSCATTERSTYLE;
	extern BYTE c_oserct_scatterchartVARYCOLORS;
	extern BYTE c_oserct_scatterchartSER;
	extern BYTE c_oserct_scatterchartDLBLS;
	extern BYTE c_oserct_scatterchartAXID;
	extern BYTE c_oserct_scatterchartEXTLST;

	extern BYTE c_oserct_radarserIDX;
	extern BYTE c_oserct_radarserORDER;
	extern BYTE c_oserct_radarserTX;
	extern BYTE c_oserct_radarserSPPR;
	extern BYTE c_oserct_radarserMARKER;
	extern BYTE c_oserct_radarserDPT;
	extern BYTE c_oserct_radarserDLBLS;
	extern BYTE c_oserct_radarserCAT;
	extern BYTE c_oserct_radarserVAL;
	extern BYTE c_oserct_radarserEXTLST;

	extern BYTE c_oserct_radarstyleVAL;

	extern BYTE c_oserct_radarchartRADARSTYLE;
	extern BYTE c_oserct_radarchartVARYCOLORS;
	extern BYTE c_oserct_radarchartSER;
	extern BYTE c_oserct_radarchartDLBLS;
	extern BYTE c_oserct_radarchartAXID;
	extern BYTE c_oserct_radarchartEXTLST;

	extern BYTE c_oserct_stockchartSER;
	extern BYTE c_oserct_stockchartDLBLS;
	extern BYTE c_oserct_stockchartDROPLINES;
	extern BYTE c_oserct_stockchartHILOWLINES;
	extern BYTE c_oserct_stockchartUPDOWNBARS;
	extern BYTE c_oserct_stockchartAXID;
	extern BYTE c_oserct_stockchartEXTLST;

	extern BYTE c_oserct_lineserIDX;
	extern BYTE c_oserct_lineserORDER;
	extern BYTE c_oserct_lineserTX;
	extern BYTE c_oserct_lineserSPPR;
	extern BYTE c_oserct_lineserMARKER;
	extern BYTE c_oserct_lineserDPT;
	extern BYTE c_oserct_lineserDLBLS;
	extern BYTE c_oserct_lineserTRENDLINE;
	extern BYTE c_oserct_lineserERRBARS;
	extern BYTE c_oserct_lineserCAT;
	extern BYTE c_oserct_lineserVAL;
	extern BYTE c_oserct_lineserSMOOTH;
	extern BYTE c_oserct_lineserEXTLST;

	extern BYTE c_oserct_updownbarsGAPWIDTH;
	extern BYTE c_oserct_updownbarsUPBARS;
	extern BYTE c_oserct_updownbarsDOWNBARS;
	extern BYTE c_oserct_updownbarsEXTLST;

	extern BYTE c_oserct_updownbarSPPR;

	extern BYTE c_oserct_line3dchartGROUPING;
	extern BYTE c_oserct_line3dchartVARYCOLORS;
	extern BYTE c_oserct_line3dchartSER;
	extern BYTE c_oserct_line3dchartDLBLS;
	extern BYTE c_oserct_line3dchartDROPLINES;
	extern BYTE c_oserct_line3dchartGAPDEPTH;
	extern BYTE c_oserct_line3dchartAXID;
	extern BYTE c_oserct_line3dchartEXTLST;

	extern BYTE c_oserct_groupingVAL;

	extern BYTE c_oserct_linechartGROUPING;
	extern BYTE c_oserct_linechartVARYCOLORS;
	extern BYTE c_oserct_linechartSER;
	extern BYTE c_oserct_linechartDLBLS;
	extern BYTE c_oserct_linechartDROPLINES;
	extern BYTE c_oserct_linechartHILOWLINES;
	extern BYTE c_oserct_linechartUPDOWNBARS;
	extern BYTE c_oserct_linechartMARKER;
	extern BYTE c_oserct_linechartSMOOTH;
	extern BYTE c_oserct_linechartAXID;
	extern BYTE c_oserct_linechartEXTLST;

	extern BYTE c_oserct_area3dchartGROUPING;
	extern BYTE c_oserct_area3dchartVARYCOLORS;
	extern BYTE c_oserct_area3dchartSER;
	extern BYTE c_oserct_area3dchartDLBLS;
	extern BYTE c_oserct_area3dchartDROPLINES;
	extern BYTE c_oserct_area3dchartGAPDEPTH;
	extern BYTE c_oserct_area3dchartAXID;
	extern BYTE c_oserct_area3dchartEXTLST;

	extern BYTE c_oserct_areaserIDX;
	extern BYTE c_oserct_areaserORDER;
	extern BYTE c_oserct_areaserTX;
	extern BYTE c_oserct_areaserSPPR;
	extern BYTE c_oserct_areaserPICTUREOPTIONS;
	extern BYTE c_oserct_areaserDPT;
	extern BYTE c_oserct_areaserDLBLS;
	extern BYTE c_oserct_areaserTRENDLINE;
	extern BYTE c_oserct_areaserERRBARS;
	extern BYTE c_oserct_areaserCAT;
	extern BYTE c_oserct_areaserVAL;
	extern BYTE c_oserct_areaserEXTLST;

	extern BYTE c_oserct_areachartGROUPING;
	extern BYTE c_oserct_areachartVARYCOLORS;
	extern BYTE c_oserct_areachartSER;
	extern BYTE c_oserct_areachartDLBLS;
	extern BYTE c_oserct_areachartDROPLINES;
	extern BYTE c_oserct_areachartAXID;
	extern BYTE c_oserct_areachartEXTLST;

	extern BYTE c_oserct_plotareaLAYOUT;
	extern BYTE c_oserct_plotareaITEMS;
	extern BYTE c_oserct_plotareaAREA3DCHART;
	extern BYTE c_oserct_plotareaAREACHART;
	extern BYTE c_oserct_plotareaBAR3DCHART;
	extern BYTE c_oserct_plotareaBARCHART;
	extern BYTE c_oserct_plotareaBUBBLECHART;
	extern BYTE c_oserct_plotareaDOUGHNUTCHART;
	extern BYTE c_oserct_plotareaLINE3DCHART;
	extern BYTE c_oserct_plotareaLINECHART;
	extern BYTE c_oserct_plotareaOFPIECHART;
	extern BYTE c_oserct_plotareaPIE3DCHART;
	extern BYTE c_oserct_plotareaPIECHART;
	extern BYTE c_oserct_plotareaRADARCHART;
	extern BYTE c_oserct_plotareaSCATTERCHART;
	extern BYTE c_oserct_plotareaSTOCKCHART;
	extern BYTE c_oserct_plotareaSURFACE3DCHART;
	extern BYTE c_oserct_plotareaSURFACECHART;
	extern BYTE c_oserct_plotareaITEMS1;
	extern BYTE c_oserct_plotareaCATAX;
	extern BYTE c_oserct_plotareaDATEAX;
	extern BYTE c_oserct_plotareaSERAX;
	extern BYTE c_oserct_plotareaVALAX;
	extern BYTE c_oserct_plotareaDTABLE;
	extern BYTE c_oserct_plotareaSPPR;
	extern BYTE c_oserct_plotareaEXTLST;

	extern BYTE c_oserct_thicknessVAL;

	extern BYTE c_oserct_surfaceTHICKNESS;
	extern BYTE c_oserct_surfaceSPPR;
	extern BYTE c_oserct_surfacePICTUREOPTIONS;
	extern BYTE c_oserct_surfaceEXTLST;

	extern BYTE c_oserct_perspectiveVAL;

	extern BYTE c_oserct_depthpercentVAL;

	extern BYTE c_oserct_rotyVAL;

	extern BYTE c_oserct_hpercentVAL;

	extern BYTE c_oserct_rotxVAL;

	extern BYTE c_oserct_view3dROTX;
	extern BYTE c_oserct_view3dHPERCENT;
	extern BYTE c_oserct_view3dROTY;
	extern BYTE c_oserct_view3dDEPTHPERCENT;
	extern BYTE c_oserct_view3dRANGAX;
	extern BYTE c_oserct_view3dPERSPECTIVE;
	extern BYTE c_oserct_view3dEXTLST;

	extern BYTE c_oserct_pivotfmtIDX;
	extern BYTE c_oserct_pivotfmtSPPR;
	extern BYTE c_oserct_pivotfmtTXPR;
	extern BYTE c_oserct_pivotfmtMARKER;
	extern BYTE c_oserct_pivotfmtDLBL;
	extern BYTE c_oserct_pivotfmtEXTLST;

	extern BYTE c_oserct_chartTITLE;
	extern BYTE c_oserct_chartAUTOTITLEDELETED;
	extern BYTE c_oserct_chartPIVOTFMTS;
	extern BYTE c_oserct_chartVIEW3D;
	extern BYTE c_oserct_chartFLOOR;
	extern BYTE c_oserct_chartSIDEWALL;
	extern BYTE c_oserct_chartBACKWALL;
	extern BYTE c_oserct_chartPLOTAREA;
	extern BYTE c_oserct_chartLEGEND;
	extern BYTE c_oserct_chartPLOTVISONLY;
	extern BYTE c_oserct_chartDISPBLANKSAS;
	extern BYTE c_oserct_chartSHOWDLBLSOVERMAX;
	extern BYTE c_oserct_chartEXTLST;

	extern BYTE c_oserct_protectionCHARTOBJECT;
	extern BYTE c_oserct_protectionDATA;
	extern BYTE c_oserct_protectionFORMATTING;
	extern BYTE c_oserct_protectionSELECTION;
	extern BYTE c_oserct_protectionUSERINTERFACE;

	extern BYTE c_oserct_pivotsourceNAME;
	extern BYTE c_oserct_pivotsourceFMTID;
	extern BYTE c_oserct_pivotsourceEXTLST;

	extern BYTE c_oserct_style1VAL;

	extern BYTE c_oserct_styleVAL;

	extern BYTE c_oserct_textlanguageidVAL;

	extern BYTE c_oseralternatecontentCHOICE;
	extern BYTE c_oseralternatecontentFALLBACK;

	extern BYTE c_oseralternatecontentchoiceSTYLE;
	extern BYTE c_oseralternatecontentchoiceREQUIRES;

	extern BYTE c_oseralternatecontentfallbackSTYLE;

	class BinaryChartReader : public Binary_CommonReader<BinaryChartReader>
	{
		LPSAFEARRAY m_pArray;
		PPTXFile::IAVSOfficeDrawingConverter* m_pOfficeDrawingConverter;
	public:
		BinaryChartReader(Streams::CBufferedStream& oBufferedStream, LPSAFEARRAY pArray, PPTXFile::IAVSOfficeDrawingConverter* pOfficeDrawingConverter);
	public: int ReadCT_ChartSpace(long length, CT_ChartSpace* poResult);
	private: int ReadCT_ChartSpace(BYTE type, long length, void* poResult);
	private: int ReadCT_Boolean(BYTE type, long length, void* poResult);
	public: int ReadCT_RelId(long length, CT_RelId* poResult);
	private: int ReadCT_RelId(BYTE type, long length, void* poResult);
	private: int ReadCT_PageSetup(BYTE type, long length, void* poResult);
	private: int ReadCT_PageMargins(BYTE type, long length, void* poResult);
	private: int ReadCT_HeaderFooter(BYTE type, long length, void* poResult);
	private: int ReadCT_PrintSettings(BYTE type, long length, void* poResult);
	private: int ReadCT_ExternalData(BYTE type, long length, void* poResult);
	private: int ReadCT_DispBlanksAs(BYTE type, long length, void* poResult);
	private: int ReadCT_LegendEntry(BYTE type, long length, void* poResult);
	private: int ReadCT_UnsignedInt(BYTE type, long length, void* poResult);
	private: int ReadCT_Extension(BYTE type, long length, void* poResult);
	private: int ReadCT_LegendPos(BYTE type, long length, void* poResult);
	private: int ReadCT_Legend(BYTE type, long length, void* poResult);
	private: int ReadCT_Layout(BYTE type, long length, void* poResult);
	private: int ReadCT_ManualLayout(BYTE type, long length, void* poResult);
	private: int ReadCT_LayoutTarget(BYTE type, long length, void* poResult);
	private: int ReadCT_LayoutMode(BYTE type, long length, void* poResult);
	private: int ReadCT_Double(BYTE type, long length, void* poResult);
	private: int ReadCT_DTable(BYTE type, long length, void* poResult);
	private: int ReadCT_SerAx(BYTE type, long length, void* poResult);
	private: int ReadCT_Scaling(BYTE type, long length, void* poResult);
	private: int ReadCT_LogBase(BYTE type, long length, void* poResult);
	private: int ReadCT_Orientation(BYTE type, long length, void* poResult);
	private: int ReadCT_AxPos(BYTE type, long length, void* poResult);
	private: int ReadCT_ChartLines(BYTE type, long length, void* poResult);
	private: int ReadCT_Title(BYTE type, long length, void* poResult);
	private: int ReadCT_Tx(BYTE type, long length, void* poResult);
	private: int ReadCT_StrRef(BYTE type, long length, void* poResult);
	private: int ReadCT_StrData(BYTE type, long length, void* poResult);
	private: int ReadCT_StrVal(BYTE type, long length, void* poResult);
	private: int ReadCT_NumFmt(BYTE type, long length, void* poResult);
	private: int ReadCT_TickMark(BYTE type, long length, void* poResult);
	private: int ReadCT_TickLblPos(BYTE type, long length, void* poResult);
	private: int ReadCT_Crosses(BYTE type, long length, void* poResult);
	private: int ReadCT_Skip(BYTE type, long length, void* poResult);
	private: int ReadCT_TimeUnit(BYTE type, long length, void* poResult);
	private: int ReadCT_DateAx(BYTE type, long length, void* poResult);
	private: int ReadCT_LblOffset(BYTE type, long length, void* poResult);
	private: int ReadCT_AxisUnit(BYTE type, long length, void* poResult);
	private: int ReadCT_LblAlgn(BYTE type, long length, void* poResult);
	private: int ReadCT_CatAx(BYTE type, long length, void* poResult);
	private: int ReadCT_DispUnitsLbl(BYTE type, long length, void* poResult);
	private: int ReadCT_BuiltInUnit(BYTE type, long length, void* poResult);
	private: int ReadCT_DispUnits(BYTE type, long length, void* poResult);
	private: int ReadCT_CrossBetween(BYTE type, long length, void* poResult);
	private: int ReadCT_ValAx(BYTE type, long length, void* poResult);
	private: int ReadCT_SizeRepresents(BYTE type, long length, void* poResult);
	private: int ReadCT_BubbleScale(BYTE type, long length, void* poResult);
	private: int ReadCT_BubbleSer(BYTE type, long length, void* poResult);
	private: int ReadCT_SerTx(BYTE type, long length, void* poResult);
	private: int ReadCT_DPt(BYTE type, long length, void* poResult);
	private: int ReadCT_Marker(BYTE type, long length, void* poResult);
	private: int ReadCT_MarkerStyle(BYTE type, long length, void* poResult);
	private: int ReadCT_MarkerSize(BYTE type, long length, void* poResult);
	private: int ReadCT_PictureOptions(BYTE type, long length, void* poResult);
	private: int ReadCT_PictureFormat(BYTE type, long length, void* poResult);
	private: int ReadCT_PictureStackUnit(BYTE type, long length, void* poResult);
	private: int ReadCT_DLbls(BYTE type, long length, void* poResult);
	private: int ReadCT_DLbl(BYTE type, long length, void* poResult);
	private: int ReadCT_DLblPos(BYTE type, long length, void* poResult);
	private: int ReadCT_Trendline(BYTE type, long length, void* poResult);
	private: int ReadCT_TrendlineType(BYTE type, long length, void* poResult);
	private: int ReadCT_Order(BYTE type, long length, void* poResult);
	private: int ReadCT_Period(BYTE type, long length, void* poResult);
	private: int ReadCT_TrendlineLbl(BYTE type, long length, void* poResult);
	private: int ReadCT_ErrBars(BYTE type, long length, void* poResult);
	private: int ReadCT_ErrDir(BYTE type, long length, void* poResult);
	private: int ReadCT_ErrBarType(BYTE type, long length, void* poResult);
	private: int ReadCT_ErrValType(BYTE type, long length, void* poResult);
	private: int ReadCT_NumDataSource(BYTE type, long length, void* poResult);
	private: int ReadCT_NumData(BYTE type, long length, void* poResult);
	private: int ReadCT_NumVal(BYTE type, long length, void* poResult);
	private: int ReadCT_NumRef(BYTE type, long length, void* poResult);
	private: int ReadCT_AxDataSource(BYTE type, long length, void* poResult);
	private: int ReadCT_MultiLvlStrRef(BYTE type, long length, void* poResult);
	private: int ReadCT_MultiLvlStrData(BYTE type, long length, void* poResult);
	private: int ReadCT_BubbleChart(BYTE type, long length, void* poResult);
	private: int ReadCT_Surface3DChart(BYTE type, long length, void* poResult);
	private: int ReadCT_SurfaceSer(BYTE type, long length, void* poResult);
	private: int ReadCT_BandFmt(BYTE type, long length, void* poResult);
	private: int ReadCT_SurfaceChart(BYTE type, long length, void* poResult);
	private: int ReadCT_SecondPieSize(BYTE type, long length, void* poResult);
	private: int ReadCT_SplitType(BYTE type, long length, void* poResult);
	private: int ReadCT_OfPieType(BYTE type, long length, void* poResult);
	private: int ReadCT_OfPieChart(BYTE type, long length, void* poResult);
	private: int ReadCT_PieSer(BYTE type, long length, void* poResult);
	private: int ReadCT_GapAmount(BYTE type, long length, void* poResult);
	private: int ReadCT_Bar3DChart(BYTE type, long length, void* poResult);
	private: int ReadCT_BarDir(BYTE type, long length, void* poResult);
	private: int ReadCT_BarGrouping(BYTE type, long length, void* poResult);
	private: int ReadCT_BarSer(BYTE type, long length, void* poResult);
	private: int ReadCT_Shape(BYTE type, long length, void* poResult);
	private: int ReadCT_Overlap(BYTE type, long length, void* poResult);
	private: int ReadCT_BarChart(BYTE type, long length, void* poResult);
	private: int ReadCT_HoleSize(BYTE type, long length, void* poResult);
	private: int ReadCT_DoughnutChart(BYTE type, long length, void* poResult);
	private: int ReadCT_FirstSliceAng(BYTE type, long length, void* poResult);
	private: int ReadCT_Pie3DChart(BYTE type, long length, void* poResult);
	private: int ReadCT_PieChart(BYTE type, long length, void* poResult);
	private: int ReadCT_ScatterSer(BYTE type, long length, void* poResult);
	private: int ReadCT_ScatterStyle(BYTE type, long length, void* poResult);
	private: int ReadCT_ScatterChart(BYTE type, long length, void* poResult);
	private: int ReadCT_RadarSer(BYTE type, long length, void* poResult);
	private: int ReadCT_RadarStyle(BYTE type, long length, void* poResult);
	private: int ReadCT_RadarChart(BYTE type, long length, void* poResult);
	private: int ReadCT_StockChart(BYTE type, long length, void* poResult);
	private: int ReadCT_LineSer(BYTE type, long length, void* poResult);
	private: int ReadCT_UpDownBars(BYTE type, long length, void* poResult);
	private: int ReadCT_UpDownBar(BYTE type, long length, void* poResult);
	private: int ReadCT_Line3DChart(BYTE type, long length, void* poResult);
	private: int ReadCT_Grouping(BYTE type, long length, void* poResult);
	private: int ReadCT_LineChart(BYTE type, long length, void* poResult);
	private: int ReadCT_Area3DChart(BYTE type, long length, void* poResult);
	private: int ReadCT_AreaSer(BYTE type, long length, void* poResult);
	private: int ReadCT_AreaChart(BYTE type, long length, void* poResult);
	private: int ReadCT_PlotArea(BYTE type, long length, void* poResult);
	private: int ReadCT_Thickness(BYTE type, long length, void* poResult);
	private: int ReadCT_Surface(BYTE type, long length, void* poResult);
	private: int ReadCT_Perspective(BYTE type, long length, void* poResult);
	private: int ReadCT_DepthPercent(BYTE type, long length, void* poResult);
	private: int ReadCT_RotY(BYTE type, long length, void* poResult);
	private: int ReadCT_HPercent(BYTE type, long length, void* poResult);
	private: int ReadCT_RotX(BYTE type, long length, void* poResult);
	private: int ReadCT_View3D(BYTE type, long length, void* poResult);
	private: int ReadCT_PivotFmt(BYTE type, long length, void* poResult);
	private: int ReadCT_Chart(BYTE type, long length, void* poResult);
	private: int ReadCT_Protection(BYTE type, long length, void* poResult);
	private: int ReadCT_PivotSource(BYTE type, long length, void* poResult);
	private: int ReadCT_Style1(BYTE type, long length, void* poResult);
	private: int ReadCT_Style(BYTE type, long length, void* poResult);
	private: int ReadCT_TextLanguageID(BYTE type, long length, void* poResult);
	private: int ReadAlternateContent(BYTE type, long length, void* poResult);
	private: int ReadAlternateContentChoice(BYTE type, long length, void* poResult);
	private: int ReadAlternateContentFallback(BYTE type, long length, void* poResult);
	};
	class BinaryChartWriter
	{
		BinaryCommonWriter m_oBcw;
		PPTXFile::IAVSOfficeDrawingConverter* m_pOfficeDrawingConverter;
	public:
		BinaryChartWriter(Streams::CBufferedStream &oCBufferedStream, PPTXFile::IAVSOfficeDrawingConverter* pOfficeDrawingConverter);
	public: void WriteCT_ChartSpace(CT_ChartSpace& oVal);
	public: void WriteCT_Boolean(CT_Boolean& oVal);
	public: void WriteCT_RelId(CT_RelId& oVal);
	public: void WriteCT_PageSetup(CT_PageSetup& oVal);
	public: void WriteCT_PageMargins(CT_PageMargins& oVal);
	public: void WriteCT_HeaderFooter(CT_HeaderFooter& oVal);
	public: void WriteCT_PrintSettings(CT_PrintSettings& oVal);
	public: void WriteCT_ExternalData(CT_ExternalData& oVal);
	public: void WriteCT_DispBlanksAs(CT_DispBlanksAs& oVal);
	public: void WriteCT_LegendEntry(CT_LegendEntry& oVal);
	public: void WriteCT_UnsignedInt(CT_UnsignedInt& oVal);
	public: void WriteCT_Extension(CT_Extension& oVal);
	public: void WriteCT_LegendPos(CT_LegendPos& oVal);
	public: void WriteCT_Legend(CT_Legend& oVal);
	public: void WriteCT_Layout(CT_Layout& oVal);
	public: void WriteCT_ManualLayout(CT_ManualLayout& oVal);
	public: void WriteCT_LayoutTarget(CT_LayoutTarget& oVal);
	public: void WriteCT_LayoutMode(CT_LayoutMode& oVal);
	public: void WriteCT_Double(CT_Double& oVal);
	public: void WriteCT_DTable(CT_DTable& oVal);
	public: void WriteCT_SerAx(CT_SerAx& oVal);
	public: void WriteCT_Scaling(CT_Scaling& oVal);
	public: void WriteCT_LogBase(CT_LogBase& oVal);
	public: void WriteCT_Orientation(CT_Orientation& oVal);
	public: void WriteCT_AxPos(CT_AxPos& oVal);
	public: void WriteCT_ChartLines(CT_ChartLines& oVal);
	public: void WriteCT_Title(CT_Title& oVal);
	public: void WriteCT_Tx(CT_Tx& oVal);
	public: void WriteCT_StrRef(CT_StrRef& oVal);
	public: void WriteCT_StrData(CT_StrData& oVal);
	public: void WriteCT_StrVal(CT_StrVal& oVal);
	public: void WriteCT_NumFmt(CT_NumFmt& oVal);
	public: void WriteCT_TickMark(CT_TickMark& oVal);
	public: void WriteCT_TickLblPos(CT_TickLblPos& oVal);
	public: void WriteCT_Crosses(CT_Crosses& oVal);
	public: void WriteCT_Skip(CT_Skip& oVal);
	public: void WriteCT_TimeUnit(CT_TimeUnit& oVal);
	public: void WriteCT_DateAx(CT_DateAx& oVal);
	public: void WriteCT_LblOffset(CT_LblOffset& oVal);
	public: void WriteCT_AxisUnit(CT_AxisUnit& oVal);
	public: void WriteCT_LblAlgn(CT_LblAlgn& oVal);
	public: void WriteCT_CatAx(CT_CatAx& oVal);
	public: void WriteCT_DispUnitsLbl(CT_DispUnitsLbl& oVal);
	public: void WriteCT_BuiltInUnit(CT_BuiltInUnit& oVal);
	public: void WriteCT_DispUnits(CT_DispUnits& oVal);
	public: void WriteCT_CrossBetween(CT_CrossBetween& oVal);
	public: void WriteCT_ValAx(CT_ValAx& oVal);
	public: void WriteCT_SizeRepresents(CT_SizeRepresents& oVal);
	public: void WriteCT_BubbleScale(CT_BubbleScale& oVal);
	public: void WriteCT_BubbleSer(CT_BubbleSer& oVal);
	public: void WriteCT_SerTx(CT_SerTx& oVal);
	public: void WriteCT_DPt(CT_DPt& oVal);
	public: void WriteCT_Marker(CT_Marker& oVal);
	public: void WriteCT_MarkerStyle(CT_MarkerStyle& oVal);
	public: void WriteCT_MarkerSize(CT_MarkerSize& oVal);
	public: void WriteCT_PictureOptions(CT_PictureOptions& oVal);
	public: void WriteCT_PictureFormat(CT_PictureFormat& oVal);
	public: void WriteCT_PictureStackUnit(CT_PictureStackUnit& oVal);
	public: void WriteCT_DLbls(CT_DLbls& oVal);
	private: void toBin(ItemsChoiceType3 eType, void* pVal);
	public: void WriteCT_DLbl(CT_DLbl& oVal);
	private: void toBin(ItemsChoiceType4 eType, void* pVal);
	public: void WriteCT_DLblPos(CT_DLblPos& oVal);
	public: void WriteCT_Trendline(CT_Trendline& oVal);
	public: void WriteCT_TrendlineType(CT_TrendlineType& oVal);
	public: void WriteCT_Order(CT_Order& oVal);
	public: void WriteCT_Period(CT_Period& oVal);
	public: void WriteCT_TrendlineLbl(CT_TrendlineLbl& oVal);
	public: void WriteCT_ErrBars(CT_ErrBars& oVal);
	public: void WriteCT_ErrDir(CT_ErrDir& oVal);
	public: void WriteCT_ErrBarType(CT_ErrBarType& oVal);
	public: void WriteCT_ErrValType(CT_ErrValType& oVal);
	public: void WriteCT_NumDataSource(CT_NumDataSource& oVal);
	public: void WriteCT_NumData(CT_NumData& oVal);
	public: void WriteCT_NumVal(CT_NumVal& oVal);
	public: void WriteCT_NumRef(CT_NumRef& oVal);
	public: void WriteCT_AxDataSource(CT_AxDataSource& oVal);
	public: void WriteCT_MultiLvlStrRef(CT_MultiLvlStrRef& oVal);
	public: void WriteCT_MultiLvlStrData(CT_MultiLvlStrData& oVal);
	public: void WriteCT_BubbleChart(CT_BubbleChart& oVal);
	public: void WriteCT_Surface3DChart(CT_Surface3DChart& oVal);
	public: void WriteCT_SurfaceSer(CT_SurfaceSer& oVal);
	public: void WriteCT_BandFmt(CT_BandFmt& oVal);
	public: void WriteCT_SurfaceChart(CT_SurfaceChart& oVal);
	public: void WriteCT_SecondPieSize(CT_SecondPieSize& oVal);
	public: void WriteCT_SplitType(CT_SplitType& oVal);
	public: void WriteCT_OfPieType(CT_OfPieType& oVal);
	public: void WriteCT_OfPieChart(CT_OfPieChart& oVal);
	public: void WriteCT_PieSer(CT_PieSer& oVal);
	public: void WriteCT_GapAmount(CT_GapAmount& oVal);
	public: void WriteCT_Bar3DChart(CT_Bar3DChart& oVal);
	public: void WriteCT_BarDir(CT_BarDir& oVal);
	public: void WriteCT_BarGrouping(CT_BarGrouping& oVal);
	public: void WriteCT_BarSer(CT_BarSer& oVal);
	public: void WriteCT_Shape(CT_Shape& oVal);
	public: void WriteCT_Overlap(CT_Overlap& oVal);
	public: void WriteCT_BarChart(CT_BarChart& oVal);
	public: void WriteCT_HoleSize(CT_HoleSize& oVal);
	public: void WriteCT_DoughnutChart(CT_DoughnutChart& oVal);
	public: void WriteCT_FirstSliceAng(CT_FirstSliceAng& oVal);
	public: void WriteCT_Pie3DChart(CT_Pie3DChart& oVal);
	public: void WriteCT_PieChart(CT_PieChart& oVal);
	public: void WriteCT_ScatterSer(CT_ScatterSer& oVal);
	public: void WriteCT_ScatterStyle(CT_ScatterStyle& oVal);
	public: void WriteCT_ScatterChart(CT_ScatterChart& oVal);
	public: void WriteCT_RadarSer(CT_RadarSer& oVal);
	public: void WriteCT_RadarStyle(CT_RadarStyle& oVal);
	public: void WriteCT_RadarChart(CT_RadarChart& oVal);
	public: void WriteCT_StockChart(CT_StockChart& oVal);
	public: void WriteCT_LineSer(CT_LineSer& oVal);
	public: void WriteCT_UpDownBars(CT_UpDownBars& oVal);
	public: void WriteCT_UpDownBar(CT_UpDownBar& oVal);
	public: void WriteCT_Line3DChart(CT_Line3DChart& oVal);
	public: void WriteCT_Grouping(CT_Grouping& oVal);
	public: void WriteCT_LineChart(CT_LineChart& oVal);
	public: void WriteCT_Area3DChart(CT_Area3DChart& oVal);
	public: void WriteCT_AreaSer(CT_AreaSer& oVal);
	public: void WriteCT_AreaChart(CT_AreaChart& oVal);
	public: void WriteCT_PlotArea(CT_PlotArea& oVal);
	private: void toBin(ItemsChoiceType5 eType, void* pVal);
	private: void toBin(ItemsChoiceType6 eType, void* pVal);
	public: void WriteCT_Thickness(CT_Thickness& oVal);
	public: void WriteCT_Surface(CT_Surface& oVal);
	public: void WriteCT_Perspective(CT_Perspective& oVal);
	public: void WriteCT_DepthPercent(CT_DepthPercent& oVal);
	public: void WriteCT_RotY(CT_RotY& oVal);
	public: void WriteCT_HPercent(CT_HPercent& oVal);
	public: void WriteCT_RotX(CT_RotX& oVal);
	public: void WriteCT_View3D(CT_View3D& oVal);
	public: void WriteCT_PivotFmt(CT_PivotFmt& oVal);
	public: void WriteCT_Chart(CT_Chart& oVal);
	public: void WriteCT_Protection(CT_Protection& oVal);
	public: void WriteCT_PivotSource(CT_PivotSource& oVal);
	public: void WriteCT_Style1(CT_Style1& oVal);
	public: void WriteCT_Style(CT_Style& oVal);
	public: void WriteCT_TextLanguageID(CT_TextLanguageID& oVal);
	public: void WriteAlternateContent(AlternateContent& oVal);
	public: void WriteAlternateContentChoice(AlternateContentChoice& oVal);
	public: void WriteAlternateContentFallback(AlternateContentFallback& oVal);
	};
}