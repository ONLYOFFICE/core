#ifndef FileInformationBlock_h
#define FileInformationBlock_h

#include "VirtualStreamReader.h"

namespace DocFileFormat
{
	typedef enum FibVersion
	{
		//Fib1993 = 0x0065,	// Word 6.0
		//Fib1995 = 0x0068,	// Word 7.0 (95)
		//Fib2002 = 0x0069,	// Word 8.0 (97)
		
		Fib1997 = 0x00C1,		
		Fib2000 = 0x00D9,
		Fib2002 = 0x0101,
		Fib2003 = 0x010C,
		Fib2007 = 0x0112
	} FibVersion;

	typedef struct FibBase
	{
		WORD wIdent;
		FibVersion nFib;
		WORD lid;
		WORD pnNext;
		bool fDot;
		bool fGlsy;
		bool fComplex;
		bool fHasPic;
		WORD cQuickSaves;
		bool fEncrypted;
		bool fWhichTblStm;
		bool fReadOnlyRecommended;
		bool fWriteReservation;
		bool fExtChar;
		bool fLoadOverwrite;
		bool fFarEast;
		bool fCrypto;
		WORD nFibBack;
		int lKey;
		unsigned char envr;
		bool fMac;
		bool fEmptySpecial;
		bool fLoadOverridePage;
		bool fFutureSavedUndo;
		bool fWord97Saved;
		int fcMin;
		int fcMac;
	} FibBase;

	typedef struct RgW97
	{
		WORD lidFE;
	} RgW97;

	typedef struct RgLw97
	{
		int cbMac;
		int ccpText;
		int ccpFtn;
		int ccpHdr;
		int ccpAtn;
		int ccpEdn;
		int ccpTxbx;
		int ccpHdrTxbx;
	} RgLw97;

	typedef struct FibWord97
	{
		unsigned int fcStshfOrig;
		unsigned int lcbStshfOrig;
		unsigned int fcStshf;
		unsigned int lcbStshf;
		unsigned int fcPlcffndRef;
		unsigned int lcbPlcffndRef;
		unsigned int fcPlcffndTxt;
		unsigned int lcbPlcffndTxt;
		unsigned int fcPlcfandRef;
		unsigned int lcbPlcfandRef;
		unsigned int fcPlcfandTxt;
		unsigned int lcbPlcfandTxt;
		unsigned int fcPlcfSed;
		unsigned int lcbPlcfSed;
		unsigned int fcPlcPad;
		unsigned int lcbPlcPad;
		unsigned int fcPlcfPhe;
		unsigned int lcbPlcfPhe;
		unsigned int fcSttbfGlsy;
		unsigned int lcbSttbfGlsy;
		unsigned int fcPlcfGlsy;
		unsigned int lcbPlcfGlsy;
		unsigned int fcPlcfHdd;
		unsigned int lcbPlcfHdd;
		unsigned int fcPlcfBteChpx;
		unsigned int lcbPlcfBteChpx;
		unsigned int fcPlcfBtePapx;
		unsigned int lcbPlcfBtePapx;
		unsigned int fcPlcfSea;
		unsigned int lcbPlcfSea;
		unsigned int fcSttbfFfn;
		unsigned int lcbSttbfFfn;
		unsigned int fcPlcfFldMom;
		unsigned int lcbPlcfFldMom;
		unsigned int fcPlcfFldHdr;
		unsigned int lcbPlcfFldHdr;
		unsigned int fcPlcfFldFtn;
		unsigned int lcbPlcfFldFtn;
		unsigned int fcPlcfFldAtn;
		unsigned int lcbPlcfFldAtn;
		unsigned int fcPlcfFldMcr;
		unsigned int lcbPlcfFldMcr;
		unsigned int fcSttbfBkmk;
		unsigned int lcbSttbfBkmk;
		unsigned int fcPlcfBkf;
		unsigned int lcbPlcfBkf;
		unsigned int fcPlcfBkl;
		unsigned int lcbPlcfBkl;
		unsigned int fcCmds;
		unsigned int lcbCmds;
		unsigned int fcSttbfMcr;
		unsigned int lcbSttbfMcr;
		unsigned int fcPrDrvr;
		unsigned int lcbPrDrvr;
		unsigned int fcPrEnvPort;
		unsigned int lcbPrEnvPort;
		unsigned int fcPrEnvLand;
		unsigned int lcbPrEnvLand;
		unsigned int fcWss;
		unsigned int lcbWss;
		unsigned int fcDop;
		unsigned int lcbDop;
		unsigned int fcSttbfAssoc;
		unsigned int lcbSttbfAssoc;
		unsigned int fcClx;
		unsigned int lcbClx;
		unsigned int fcPlcfPgdFtn;
		unsigned int lcbPlcfPgdFtn;
		unsigned int fcAutosaveSource;
		unsigned int lcbAutosaveSource;
		unsigned int fcGrpXstAtnOwners;
		unsigned int lcbGrpXstAtnOwners;
		unsigned int fcSttbfAtnBkmk;
		unsigned int lcbSttbfAtnBkmk;
		unsigned int fcPlcSpaMom;
		unsigned int lcbPlcSpaMom;
		unsigned int fcPlcSpaHdr;
		unsigned int lcbPlcSpaHdr;
		unsigned int fcPlcfAtnBkf;
		unsigned int lcbPlcfAtnBkf;
		unsigned int fcPlcfAtnBkl;
		unsigned int lcbPlcfAtnBkl;
		unsigned int fcPms;
		unsigned int lcbPms;
		unsigned int fcFormFldSttbs;
		unsigned int lcbFormFldSttbs;
		unsigned int fcPlcfendRef;
		unsigned int lcbPlcfendRef;
		unsigned int fcPlcfendTxt;
		unsigned int lcbPlcfendTxt;
		unsigned int fcPlcfFldEdn;
		unsigned int lcbPlcfFldEdn;
		unsigned int fcDggInfo;
		unsigned int lcbDggInfo;
		unsigned int fcSttbfRMark;
		unsigned int lcbSttbfRMark;
		unsigned int fcSttbfCaption;
		unsigned int lcbSttbfCaption;
		unsigned int fcSttbfAutoCaption;
		unsigned int lcbSttbfAutoCaption;
		unsigned int fcPlcfWkb;
		unsigned int lcbPlcfWkb;
		unsigned int fcPlcfSpl;
		unsigned int lcbPlcfSpl;
		unsigned int fcPlcftxbxTxt;
		unsigned int lcbPlcftxbxTxt;
		unsigned int fcPlcfFldTxbx;
		unsigned int lcbPlcfFldTxbx;
		unsigned int fcPlcfHdrtxbxTxt;
		unsigned int lcbPlcfHdrtxbxTxt;
		unsigned int fcPlcffldHdrTxbx;
		unsigned int lcbPlcffldHdrTxbx;
		unsigned int fcStwUser;
		unsigned int lcbStwUser;
		unsigned int fcSttbTtmbd;
		unsigned int lcbSttbTtmbd;
		unsigned int fcCookieData;
		unsigned int lcbCookieData;
		unsigned int fcPgdMotherOldOld;
		unsigned int lcbPgdMotherOldOld;
		unsigned int fcBkdMotherOldOld;
		unsigned int lcbBkdMotherOldOld;
		unsigned int fcPgdFtnOldOld;
		unsigned int lcbPgdFtnOldOld;
		unsigned int fcBkdFtnOldOld;
		unsigned int lcbBkdFtnOldOld;
		unsigned int fcPgdEdnOldOld;
		unsigned int lcbPgdEdnOldOld;
		unsigned int fcBkdEdnOldOld;
		unsigned int lcbBkdEdnOldOld;
		unsigned int fcSttbfIntlFld;
		unsigned int lcbSttbfIntlFld;
		unsigned int fcRouteSlip;
		unsigned int lcbRouteSlip;
		unsigned int fcSttbSavedBy;
		unsigned int lcbSttbSavedBy;
		unsigned int fcSttbFnm;
		unsigned int lcbSttbFnm;
		unsigned int fcPlfLst;
		unsigned int lcbPlfLst;
		unsigned int fcPlfLfo;
		unsigned int lcbPlfLfo;
		unsigned int fcPlcfTxbxBkd;
		unsigned int lcbPlcfTxbxBkd;
		unsigned int fcPlcfTxbxHdrBkd;
		unsigned int lcbPlcfTxbxHdrBkd;
		unsigned int fcDocUndoWord9;
		unsigned int lcbDocUndoWord9;
		unsigned int fcRgbUse;
		unsigned int lcbRgbUse;
		unsigned int fcUsp;
		unsigned int lcbUsp;
		unsigned int fcUskf;
		unsigned int lcbUskf;
		unsigned int fcPlcupcRgbUse;
		unsigned int lcbPlcupcRgbUse;
		unsigned int fcPlcupcUsp;
		unsigned int lcbPlcupcUsp;
		unsigned int fcSttbGlsyStyle;
		unsigned int lcbSttbGlsyStyle;
		unsigned int fcPlgosl;
		unsigned int lcbPlgosl;
		unsigned int fcPlcocx;
		unsigned int lcbPlcocx;
		unsigned int fcPlcfBteLvc;
		unsigned int lcbPlcfBteLvc;
		unsigned int dwLowDateTime;
		unsigned int dwHighDateTime;
		unsigned int fcPlcfLvcPre10;
		unsigned int lcbPlcfLvcPre10;
		unsigned int fcPlcfAsumy;
		unsigned int lcbPlcfAsumy;
		unsigned int fcPlcfGram;
		unsigned int lcbPlcfGram;
		unsigned int fcSttbListNames;
		unsigned int lcbSttbListNames;
		unsigned int fcSttbfUssr;
		unsigned int lcbSttbfUssr;
	} FibWord97;

	typedef struct FibWord2000
	{
		unsigned int fcPlcfTch;
		unsigned int lcbPlcfTch;
		unsigned int fcRmdThreading;
		unsigned int lcbRmdThreading;
		unsigned int fcMid;
		unsigned int lcbMid;
		unsigned int fcSttbRgtplc;
		unsigned int lcbSttbRgtplc;
		unsigned int fcMsoEnvelope;
		unsigned int lcbMsoEnvelope;
		unsigned int fcPlcfLad;
		unsigned int lcbPlcfLad;
		unsigned int fcRgDofr;
		unsigned int lcbRgDofr;
		unsigned int fcPlcosl;
		unsigned int lcbPlcosl;
		unsigned int fcPlcfCookieOld;
		unsigned int lcbPlcfCookieOld;
		unsigned int fcPgdMotherOld;
		unsigned int lcbPgdMotherOld;
		unsigned int fcBkdMotherOld;
		unsigned int lcbBkdMotherOld;
		unsigned int fcPgdFtnOld;
		unsigned int lcbPgdFtnOld;
		unsigned int fcBkdFtnOld;
		unsigned int lcbBkdFtnOld;
		unsigned int fcPgdEdnOld;
		unsigned int lcbPgdEdnOld;
		unsigned int fcBkdEdnOld;
		unsigned int lcbBkdEdnOld;
	} FibWord2000;

	typedef struct FibWord2002
	{
		unsigned int fcPlcfPgp;
		unsigned int lcbPlcfPgp;
		unsigned int fcPlcfuim;
		unsigned int lcbPlcfuim;
		unsigned int fcPlfguidUim;
		unsigned int lcbPlfguidUim;
		unsigned int fcAtrdExtra;
		unsigned int lcbAtrdExtra;
		unsigned int fcPlrsid;
		unsigned int lcbPlrsid;
		unsigned int fcSttbfBkmkFactoid;
		unsigned int lcbSttbfBkmkFactoid;
		unsigned int fcPlcfBkfFactoid;
		unsigned int lcbPlcfBkfFactoid;
		unsigned int fcPlcfcookie;
		unsigned int lcbPlcfcookie;
		unsigned int fcPlcfBklFactoid;
		unsigned int lcbPlcfBklFactoid;
		unsigned int fcFactoidData;
		unsigned int lcbFactoidData;
		unsigned int fcDocUndo;
		unsigned int lcbDocUndo;
		unsigned int fcSttbfBkmkFcc;
		unsigned int lcbSttbfBkmkFcc;
		unsigned int fcPlcfBkfFcc;
		unsigned int lcbPlcfBkfFcc;
		unsigned int fcPlcfBklFcc;
		unsigned int lcbPlcfBklFcc;
		unsigned int fcSttbfbkmkBPRepairs;
		unsigned int lcbSttbfbkmkBPRepairs;
		unsigned int fcPlcfbkfBPRepairs;
		unsigned int lcbPlcfbkfBPRepairs;
		unsigned int fcPlcfbklBPRepairs;
		unsigned int lcbPlcfbklBPRepairs;
		unsigned int fcPmsNew;
		unsigned int lcbPmsNew;
		unsigned int fcODSO;
		unsigned int lcbODSO;
		unsigned int fcPlcfpmiOldXP;
		unsigned int lcbPlcfpmiOldXP;
		unsigned int fcPlcfpmiNewXP;
		unsigned int lcbPlcfpmiNewXP;
		unsigned int fcPlcfpmiMixedXP;
		unsigned int lcbPlcfpmiMixedXP;
		unsigned int fcPlcffactoid;
		unsigned int lcbPlcffactoid;
		unsigned int fcPlcflvcOldXP;
		unsigned int lcbPlcflvcOldXP;
		unsigned int fcPlcflvcNewXP;
		unsigned int lcbPlcflvcNewXP;
		unsigned int fcPlcflvcMixedXP;
		unsigned int lcbPlcflvcMixedXP;
	} FibWord2002;

	typedef struct FibWord2003
	{
		unsigned int fcHplxsdr;
		unsigned int lcbHplxsdr;
		unsigned int fcSttbfBkmkSdt;
		unsigned int lcbSttbfBkmkSdt;
		unsigned int fcPlcfBkfSdt;
		unsigned int lcbPlcfBkfSdt;
		unsigned int fcPlcfBklSdt;
		unsigned int lcbPlcfBklSdt;
		unsigned int fcCustomXForm;
		unsigned int lcbCustomXForm;
		unsigned int fcSttbfBkmkProt;
		unsigned int lcbSttbfBkmkProt;
		unsigned int fcPlcfBkfProt;
		unsigned int lcbPlcfBkfProt;
		unsigned int fcPlcfBklProt;
		unsigned int lcbPlcfBklProt;
		unsigned int fcSttbProtUser;
		unsigned int lcbSttbProtUser;
		unsigned int fcPlcfpmiOld;
		unsigned int lcbPlcfpmiOld;
		unsigned int fcPlcfpmiOldInline;
		unsigned int lcbPlcfpmiOldInline;
		unsigned int fcPlcfpmiNew;
		unsigned int lcbPlcfpmiNew;
		unsigned int fcPlcfpmiNewInline;
		unsigned int lcbPlcfpmiNewInline;
		unsigned int fcPlcflvcOld;
		unsigned int lcbPlcflvcOld;
		unsigned int fcPlcflvcOldInline;
		unsigned int lcbPlcflvcOldInline;
		unsigned int fcPlcflvcNew;
		unsigned int lcbPlcflvcNew;
		unsigned int fcPlcflvcNewInline;
		unsigned int lcbPlcflvcNewInline;
		unsigned int fcPgdMother;
		unsigned int lcbPgdMother;
		unsigned int fcBkdMother;
		unsigned int lcbBkdMother;
		unsigned int fcAfdMother;
		unsigned int lcbAfdMother;
		unsigned int fcPgdFtn;
		unsigned int lcbPgdFtn;
		unsigned int fcBkdFtn;
		unsigned int lcbBkdFtn;
		unsigned int fcAfdFtn;
		unsigned int lcbAfdFtn;
		unsigned int fcPgdEdn;
		unsigned int lcbPgdEdn;
		unsigned int fcBkdEdn;
		unsigned int lcbBkdEdn;
		unsigned int fcAfdEdn;
		unsigned int lcbAfdEdn;
		unsigned int fcAfd;
		unsigned int lcbAfd;
	} FibWord2003;

	typedef struct FibWord2007
	{
		unsigned int fcPlcfmthd;
		unsigned int lcbPlcfmthd;
		unsigned int fcSttbfBkmkMoveFrom;
		unsigned int lcbSttbfBkmkMoveFrom;
		unsigned int fcPlcfBkfMoveFrom;
		unsigned int lcbPlcfBkfMoveFrom;
		unsigned int fcPlcfBklMoveFrom;
		unsigned int lcbPlcfBklMoveFrom;
		unsigned int fcSttbfBkmkMoveTo;
		unsigned int lcbSttbfBkmkMoveTo;
		unsigned int fcPlcfBkfMoveTo;
		unsigned int lcbPlcfBkfMoveTo;
		unsigned int fcPlcfBklMoveTo;
		unsigned int lcbPlcfBklMoveTo;
		unsigned int fcSttbfBkmkArto;
		unsigned int lcbSttbfBkmkArto;
		unsigned int fcPlcfBkfArto;
		unsigned int lcbPlcfBkfArto;
		unsigned int fcPlcfBklArto;
		unsigned int lcbPlcfBklArto;
		unsigned int fcArtoData;
		unsigned int lcbArtoData;
		unsigned int fcOssTheme;
		unsigned int lcbOssTheme;
		unsigned int fcColorSchemeMapping;
		unsigned int lcbColorSchemeMapping;
	} FibWord2007;

	typedef struct FibNew
	{
		FibVersion nFibNew;
		WORD cQuickSavesNew;
	} FibNew;


	class FileInformationBlock
	{
	public:
		FibBase m_FibBase;
		RgW97 m_RgW97;
		RgLw97 m_RgLw97;
		FibWord97 m_FibWord97;
		FibWord2000 m_FibWord2000;
		FibWord2002 m_FibWord2002;
		FibWord2003 m_FibWord2003;
		FibWord2007 m_FibWord2007;
		FibNew m_FibNew;

		WORD csw;
		WORD cslw;
		WORD cbRgFcLcb;
		WORD cswNew;

		FileInformationBlock( VirtualStreamReader reader )
		{
			unsigned int flag16 = 0;
			unsigned char flag8 = 0;

			//read the FIB base
			this->m_FibBase.wIdent = reader.ReadUInt16(); //0
			this->m_FibBase.nFib = (FibVersion)reader.ReadUInt16(); //2
			reader.ReadBytes( 2, false ); //4
			this->m_FibBase.lid = reader.ReadUInt16(); //6
			this->m_FibBase.pnNext = reader.ReadInt16(); //8
			flag16 = reader.ReadUInt16(); //10
			this->m_FibBase.fDot = FormatUtils::BitmaskToBool((int)flag16, 0x0001);
			this->m_FibBase.fGlsy = FormatUtils::BitmaskToBool((int)flag16, 0x0002);
			this->m_FibBase.fComplex = FormatUtils::BitmaskToBool((int)flag16, 0x0002);
			this->m_FibBase.fHasPic = FormatUtils::BitmaskToBool((int)flag16, 0x0008);
			this->m_FibBase.cQuickSaves = (WORD)(((int)flag16 & 0x00F0) >> 4);
			this->m_FibBase.fEncrypted = FormatUtils::BitmaskToBool((int)flag16, 0x0100);
			this->m_FibBase.fWhichTblStm = FormatUtils::BitmaskToBool((int)flag16, 0x0200);
			this->m_FibBase.fReadOnlyRecommended = FormatUtils::BitmaskToBool((int)flag16, 0x0400);
			this->m_FibBase.fWriteReservation = FormatUtils::BitmaskToBool((int)flag16, 0x0800);
			this->m_FibBase.fExtChar = FormatUtils::BitmaskToBool((int)flag16, 0x1000);
			this->m_FibBase.fLoadOverwrite = FormatUtils::BitmaskToBool((int)flag16, 0x2000);
			this->m_FibBase.fFarEast = FormatUtils::BitmaskToBool((int)flag16, 0x4000);
			this->m_FibBase.fCrypto = FormatUtils::BitmaskToBool((int)flag16, 0x8000);
			this->m_FibBase.nFibBack = reader.ReadUInt16(); //12
			this->m_FibBase.lKey = reader.ReadInt32(); //14
			this->m_FibBase.envr = reader.ReadByte(); //18
			flag8 = reader.ReadByte(); //19
			this->m_FibBase.fMac = FormatUtils::BitmaskToBool((int)flag8, 0x01);
			this->m_FibBase.fEmptySpecial = FormatUtils::BitmaskToBool((int)flag8, 0x02);
			this->m_FibBase.fLoadOverridePage = FormatUtils::BitmaskToBool((int)flag8, 0x04);
			this->m_FibBase.fFutureSavedUndo = FormatUtils::BitmaskToBool((int)flag8, 0x08);
			this->m_FibBase.fWord97Saved = FormatUtils::BitmaskToBool((int)flag8, 0x10);
			reader.ReadBytes( 4, false ); //20
			this->m_FibBase.fcMin = reader.ReadInt32(); //24
			this->m_FibBase.fcMac = reader.ReadInt32(); //28

			this->csw = reader.ReadUInt16(); //32

			//read the RgW97
			reader.ReadBytes( 26, false ); //34 
			this->m_RgW97.lidFE = reader.ReadUInt16(); //60

			this->cslw = reader.ReadUInt16(); //62

			//read the RgLW97
			m_RgLw97.cbMac		=	reader.ReadInt32(); //64
			reader.ReadBytes(8, false); //68
			m_RgLw97.ccpText	=	reader.ReadInt32(); //76
			m_RgLw97.ccpFtn		=	reader.ReadInt32(); //80
			m_RgLw97.ccpHdr		=	reader.ReadInt32(); //84
			
			reader.ReadBytes( 4, false ); //88
			
			m_RgLw97.ccpAtn		=	reader.ReadInt32(); //92
			m_RgLw97.ccpEdn		=	reader.ReadInt32(); //96
			m_RgLw97.ccpTxbx	=	reader.ReadInt32(); //100
			m_RgLw97.ccpHdrTxbx	=	reader.ReadInt32(); //104
			
			reader.ReadBytes(44, false); //108

			cbRgFcLcb			=	reader.ReadUInt16(); //152

			if (m_FibBase.nFib >= Fib1997)
			{
				//Read the FibRgFcLcb97
				m_FibWord97.fcStshfOrig = reader.ReadUInt32(); //154
				m_FibWord97.lcbStshfOrig = reader.ReadUInt32(); //158
				m_FibWord97.fcStshf = reader.ReadUInt32(); //162
				m_FibWord97.lcbStshf = reader.ReadUInt32(); //166
				m_FibWord97.fcPlcffndRef = reader.ReadUInt32(); //170
				m_FibWord97.lcbPlcffndRef = reader.ReadUInt32(); //174
				m_FibWord97.fcPlcffndTxt = reader.ReadUInt32(); //178
				m_FibWord97.lcbPlcffndTxt = reader.ReadUInt32(); //182
				m_FibWord97.fcPlcfandRef = reader.ReadUInt32(); //186
				m_FibWord97.lcbPlcfandRef = reader.ReadUInt32(); //190
				m_FibWord97.fcPlcfandTxt = reader.ReadUInt32(); //194
				m_FibWord97.lcbPlcfandTxt = reader.ReadUInt32(); //198
				m_FibWord97.fcPlcfSed = reader.ReadUInt32(); //202
				m_FibWord97.lcbPlcfSed = reader.ReadUInt32(); //206
				m_FibWord97.fcPlcPad = reader.ReadUInt32(); //210
				m_FibWord97.lcbPlcPad = reader.ReadUInt32(); //214
				m_FibWord97.fcPlcfPhe = reader.ReadUInt32(); //218
				m_FibWord97.lcbPlcfPhe = reader.ReadUInt32(); //222
				m_FibWord97.fcSttbfGlsy = reader.ReadUInt32(); //226
				m_FibWord97.lcbSttbfGlsy = reader.ReadUInt32(); //230
				m_FibWord97.fcPlcfGlsy = reader.ReadUInt32(); //234
				m_FibWord97.lcbPlcfGlsy = reader.ReadUInt32(); //238
				m_FibWord97.fcPlcfHdd = reader.ReadUInt32(); //242
				m_FibWord97.lcbPlcfHdd = reader.ReadUInt32(); //246
				m_FibWord97.fcPlcfBteChpx = reader.ReadUInt32(); //250
				m_FibWord97.lcbPlcfBteChpx = reader.ReadUInt32(); //254
				m_FibWord97.fcPlcfBtePapx = reader.ReadUInt32(); //258
				m_FibWord97.lcbPlcfBtePapx = reader.ReadUInt32(); //262
				m_FibWord97.fcPlcfSea = reader.ReadUInt32(); //266
				m_FibWord97.lcbPlcfSea = reader.ReadUInt32(); //270
				m_FibWord97.fcSttbfFfn = reader.ReadUInt32(); //274
				m_FibWord97.lcbSttbfFfn = reader.ReadUInt32(); //278
				m_FibWord97.fcPlcfFldMom = reader.ReadUInt32(); //282
				m_FibWord97.lcbPlcfFldMom = reader.ReadUInt32(); //286
				m_FibWord97.fcPlcfFldHdr = reader.ReadUInt32(); //290
				m_FibWord97.lcbPlcfFldHdr = reader.ReadUInt32(); //294
				m_FibWord97.fcPlcfFldFtn = reader.ReadUInt32(); //298
				m_FibWord97.lcbPlcfFldFtn = reader.ReadUInt32(); //302
				m_FibWord97.fcPlcfFldAtn = reader.ReadUInt32(); //306
				m_FibWord97.lcbPlcfFldAtn = reader.ReadUInt32(); //310
				m_FibWord97.fcPlcfFldMcr = reader.ReadUInt32(); //314
				m_FibWord97.lcbPlcfFldMcr = reader.ReadUInt32(); //318
				m_FibWord97.fcSttbfBkmk = reader.ReadUInt32(); //322
				m_FibWord97.lcbSttbfBkmk = reader.ReadUInt32(); //326
				m_FibWord97.fcPlcfBkf = reader.ReadUInt32(); //330
				m_FibWord97.lcbPlcfBkf = reader.ReadUInt32(); //334
				m_FibWord97.fcPlcfBkl = reader.ReadUInt32(); //338
				m_FibWord97.lcbPlcfBkl = reader.ReadUInt32(); //342
				m_FibWord97.fcCmds = reader.ReadUInt32(); //346
				m_FibWord97.lcbCmds = reader.ReadUInt32(); //350
				
				reader.ReadUInt32();											//	354
				reader.ReadUInt32();											//	358
				
				m_FibWord97.fcSttbfMcr				=	reader.ReadUInt32();	//	362
				m_FibWord97.lcbSttbfMcr				=	reader.ReadUInt32();	//	366
				m_FibWord97.fcPrDrvr				=	reader.ReadUInt32();	//	370
				m_FibWord97.lcbPrDrvr				=	reader.ReadUInt32();	//	374
				m_FibWord97.fcPrEnvPort				=	reader.ReadUInt32();	//	378
				m_FibWord97.lcbPrEnvPort			=	reader.ReadUInt32();	//	382
				m_FibWord97.fcPrEnvLand				=	reader.ReadUInt32();	//	386
				m_FibWord97.lcbPrEnvLand			=	reader.ReadUInt32();	//	390
				m_FibWord97.fcWss					=	reader.ReadUInt32();	//	394
				m_FibWord97.lcbWss					=	reader.ReadUInt32();	//	398

				m_FibWord97.fcDop					=	reader.ReadUInt32();	//	402
				m_FibWord97.lcbDop					=	reader.ReadUInt32();	//	406

				m_FibWord97.fcSttbfAssoc			=	reader.ReadUInt32();	//	410
				m_FibWord97.lcbSttbfAssoc			=	reader.ReadUInt32();	//	414
				m_FibWord97.fcClx					=	reader.ReadUInt32();	//	418
				m_FibWord97.lcbClx					=	reader.ReadUInt32();	//	422
				m_FibWord97.fcPlcfPgdFtn			=	reader.ReadUInt32();	//	426
				m_FibWord97.lcbPlcfPgdFtn			=	reader.ReadUInt32();	//	430
				m_FibWord97.fcAutosaveSource		=	reader.ReadUInt32();	//	434
				m_FibWord97.lcbAutosaveSource		=	reader.ReadUInt32();	//	438
				m_FibWord97.fcGrpXstAtnOwners		=	reader.ReadUInt32();	//	442
				m_FibWord97.lcbGrpXstAtnOwners		=	reader.ReadUInt32();	//	446
				m_FibWord97.fcSttbfAtnBkmk			=	reader.ReadUInt32();	//	450
				m_FibWord97.lcbSttbfAtnBkmk			=	reader.ReadUInt32();	//	454
				
				reader.ReadUInt32(); //458
				reader.ReadUInt32(); //462
				reader.ReadUInt32(); //466
				reader.ReadUInt32(); //470
				
				m_FibWord97.fcPlcSpaMom = reader.ReadUInt32(); //474
				m_FibWord97.lcbPlcSpaMom = reader.ReadUInt32(); //478
				m_FibWord97.fcPlcSpaHdr = reader.ReadUInt32(); //482
				m_FibWord97.lcbPlcSpaHdr = reader.ReadUInt32(); //486
				m_FibWord97.fcPlcfAtnBkf = reader.ReadUInt32(); //490
				m_FibWord97.lcbPlcfAtnBkf = reader.ReadUInt32(); //494
				m_FibWord97.fcPlcfAtnBkl = reader.ReadUInt32(); //498
				m_FibWord97.lcbPlcfAtnBkl = reader.ReadUInt32(); //502
				m_FibWord97.fcPms = reader.ReadUInt32(); //506
				m_FibWord97.lcbPms = reader.ReadUInt32(); //510
				m_FibWord97.fcFormFldSttbs = reader.ReadUInt32(); //514
				m_FibWord97.lcbFormFldSttbs = reader.ReadUInt32(); //518
				m_FibWord97.fcPlcfendRef = reader.ReadUInt32(); //522
				m_FibWord97.lcbPlcfendRef = reader.ReadUInt32(); //526
				m_FibWord97.fcPlcfendTxt = reader.ReadUInt32(); //530
				m_FibWord97.lcbPlcfendTxt = reader.ReadUInt32(); //534
				m_FibWord97.fcPlcfFldEdn = reader.ReadUInt32(); //538
				m_FibWord97.lcbPlcfFldEdn = reader.ReadUInt32(); //542
				
				reader.ReadUInt32(); //546
				reader.ReadUInt32(); //550
				
				m_FibWord97.fcDggInfo = reader.ReadUInt32(); //554
				m_FibWord97.lcbDggInfo = reader.ReadUInt32(); //558
				m_FibWord97.fcSttbfRMark = reader.ReadUInt32(); //562
				m_FibWord97.lcbSttbfRMark = reader.ReadUInt32(); //566
				m_FibWord97.fcSttbfCaption = reader.ReadUInt32(); //570
				m_FibWord97.lcbSttbfCaption = reader.ReadUInt32(); //574
				m_FibWord97.fcSttbfAutoCaption = reader.ReadUInt32(); //578
				m_FibWord97.lcbSttbfAutoCaption = reader.ReadUInt32(); //582
				m_FibWord97.fcPlcfWkb = reader.ReadUInt32(); //586
				m_FibWord97.lcbPlcfWkb = reader.ReadUInt32(); //590
				m_FibWord97.fcPlcfSpl = reader.ReadUInt32(); //594
				m_FibWord97.lcbPlcfSpl = reader.ReadUInt32(); //598
				m_FibWord97.fcPlcftxbxTxt = reader.ReadUInt32(); //602
				m_FibWord97.lcbPlcftxbxTxt = reader.ReadUInt32(); //606
				m_FibWord97.fcPlcfFldTxbx = reader.ReadUInt32(); //610
				m_FibWord97.lcbPlcfFldTxbx = reader.ReadUInt32(); //614
				m_FibWord97.fcPlcfHdrtxbxTxt = reader.ReadUInt32(); //618
				m_FibWord97.lcbPlcfHdrtxbxTxt = reader.ReadUInt32(); //622
				m_FibWord97.fcPlcffldHdrTxbx = reader.ReadUInt32(); //626
				m_FibWord97.lcbPlcffldHdrTxbx = reader.ReadUInt32(); //630
				m_FibWord97.fcStwUser = reader.ReadUInt32(); //634
				m_FibWord97.lcbStwUser = reader.ReadUInt32(); //638
				m_FibWord97.fcSttbTtmbd = reader.ReadUInt32(); //642
				m_FibWord97.lcbSttbTtmbd = reader.ReadUInt32(); //646
				m_FibWord97.fcCookieData = reader.ReadUInt32(); //650
				m_FibWord97.lcbCookieData = reader.ReadUInt32(); //654
				m_FibWord97.fcPgdMotherOldOld = reader.ReadUInt32(); //658
				m_FibWord97.lcbPgdMotherOldOld = reader.ReadUInt32(); //662
				m_FibWord97.fcBkdMotherOldOld = reader.ReadUInt32(); //666
				m_FibWord97.lcbBkdMotherOldOld = reader.ReadUInt32(); //670
				m_FibWord97.fcPgdFtnOldOld = reader.ReadUInt32(); //674
				m_FibWord97.lcbPgdFtnOldOld = reader.ReadUInt32(); //678
				m_FibWord97.fcBkdFtnOldOld = reader.ReadUInt32(); //682
				m_FibWord97.lcbBkdFtnOldOld = reader.ReadUInt32(); //686
				m_FibWord97.fcPgdEdnOldOld = reader.ReadUInt32(); //690
				m_FibWord97.lcbPgdEdnOldOld = reader.ReadUInt32(); //694
				m_FibWord97.fcBkdEdnOldOld = reader.ReadUInt32(); //698
				m_FibWord97.lcbBkdEdnOldOld = reader.ReadUInt32(); //702
				m_FibWord97.fcSttbfIntlFld = reader.ReadUInt32(); //706
				m_FibWord97.lcbSttbfIntlFld = reader.ReadUInt32(); //710
				m_FibWord97.fcRouteSlip = reader.ReadUInt32(); //714
				m_FibWord97.lcbRouteSlip = reader.ReadUInt32(); //718
				m_FibWord97.fcSttbSavedBy = reader.ReadUInt32(); //722
				m_FibWord97.lcbSttbSavedBy = reader.ReadUInt32(); //726
				m_FibWord97.fcSttbFnm = reader.ReadUInt32(); //730
				m_FibWord97.lcbSttbFnm = reader.ReadUInt32(); //734
				m_FibWord97.fcPlfLst = reader.ReadUInt32(); //738
				m_FibWord97.lcbPlfLst = reader.ReadUInt32(); //742
				m_FibWord97.fcPlfLfo = reader.ReadUInt32(); //746
				m_FibWord97.lcbPlfLfo = reader.ReadUInt32(); //750
				m_FibWord97.fcPlcfTxbxBkd = reader.ReadUInt32(); //754
				m_FibWord97.lcbPlcfTxbxBkd = reader.ReadUInt32(); //758
				m_FibWord97.fcPlcfTxbxHdrBkd = reader.ReadUInt32(); //762
				m_FibWord97.lcbPlcfTxbxHdrBkd = reader.ReadUInt32(); //766
				m_FibWord97.fcDocUndoWord9 = reader.ReadUInt32(); //770
				m_FibWord97.lcbDocUndoWord9 = reader.ReadUInt32(); //774
				m_FibWord97.fcRgbUse = reader.ReadUInt32(); //778
				m_FibWord97.lcbRgbUse = reader.ReadUInt32(); //782
				m_FibWord97.fcUsp = reader.ReadUInt32(); //786
				m_FibWord97.lcbUsp = reader.ReadUInt32(); //790
				m_FibWord97.fcUskf = reader.ReadUInt32(); //794
				m_FibWord97.lcbUskf = reader.ReadUInt32(); //798
				m_FibWord97.fcPlcupcRgbUse = reader.ReadUInt32(); //802
				m_FibWord97.lcbPlcupcRgbUse = reader.ReadUInt32(); //806
				m_FibWord97.fcPlcupcUsp = reader.ReadUInt32(); //810
				m_FibWord97.lcbPlcupcUsp = reader.ReadUInt32(); //814
				m_FibWord97.fcSttbGlsyStyle = reader.ReadUInt32(); //818
				m_FibWord97.lcbSttbGlsyStyle = reader.ReadUInt32(); //822
				m_FibWord97.fcPlgosl = reader.ReadUInt32(); //826
				m_FibWord97.lcbPlgosl = reader.ReadUInt32(); //830
				m_FibWord97.fcPlcocx = reader.ReadUInt32(); //834
				m_FibWord97.lcbPlcocx = reader.ReadUInt32(); //838
				m_FibWord97.fcPlcfBteLvc = reader.ReadUInt32(); //842
				m_FibWord97.lcbPlcfBteLvc = reader.ReadUInt32(); //846
				m_FibWord97.dwLowDateTime = reader.ReadUInt32(); //850
				m_FibWord97.dwHighDateTime = reader.ReadUInt32(); //854
				m_FibWord97.fcPlcfLvcPre10 = reader.ReadUInt32(); //858
				m_FibWord97.lcbPlcfLvcPre10 = reader.ReadUInt32(); //862
				m_FibWord97.fcPlcfAsumy = reader.ReadUInt32(); //866
				m_FibWord97.lcbPlcfAsumy = reader.ReadUInt32(); //870
				m_FibWord97.fcPlcfGram = reader.ReadUInt32(); //874
				m_FibWord97.lcbPlcfGram = reader.ReadUInt32(); //878
				m_FibWord97.fcSttbListNames = reader.ReadUInt32(); //882
				m_FibWord97.lcbSttbListNames = reader.ReadUInt32(); //886
				m_FibWord97.fcSttbfUssr = reader.ReadUInt32(); //890
				m_FibWord97.lcbSttbfUssr = reader.ReadUInt32(); //894
			}

			if (m_FibBase.nFib >= Fib2000)
			{
				//Read also the FibRgFcLcb2000
				this->m_FibWord2000.fcPlcfTch = reader.ReadUInt32(); //898
				this->m_FibWord2000.lcbPlcfTch = reader.ReadUInt32(); //902
				this->m_FibWord2000.fcRmdThreading = reader.ReadUInt32(); //906
				this->m_FibWord2000.lcbRmdThreading = reader.ReadUInt32(); //910
				this->m_FibWord2000.fcMid = reader.ReadUInt32(); //914
				this->m_FibWord2000.lcbMid = reader.ReadUInt32(); //918
				this->m_FibWord2000.fcSttbRgtplc = reader.ReadUInt32(); //922
				this->m_FibWord2000.lcbSttbRgtplc = reader.ReadUInt32(); //926
				this->m_FibWord2000.fcMsoEnvelope = reader.ReadUInt32(); //930
				this->m_FibWord2000.lcbMsoEnvelope = reader.ReadUInt32(); //934
				this->m_FibWord2000.fcPlcfLad = reader.ReadUInt32(); //938
				this->m_FibWord2000.lcbPlcfLad = reader.ReadUInt32(); //942
				this->m_FibWord2000.fcRgDofr = reader.ReadUInt32(); //946
				this->m_FibWord2000.lcbRgDofr = reader.ReadUInt32(); //950
				this->m_FibWord2000.fcPlcosl = reader.ReadUInt32(); //954
				this->m_FibWord2000.lcbPlcosl = reader.ReadUInt32(); //958
				this->m_FibWord2000.fcPlcfCookieOld = reader.ReadUInt32(); //962
				this->m_FibWord2000.lcbPlcfCookieOld = reader.ReadUInt32(); //966
				this->m_FibWord2000.fcPgdMotherOld = reader.ReadUInt32(); //970
				this->m_FibWord2000.lcbPgdMotherOld = reader.ReadUInt32(); //974
				this->m_FibWord2000.fcBkdMotherOld = reader.ReadUInt32(); //978
				this->m_FibWord2000.lcbBkdMotherOld = reader.ReadUInt32(); //982
				this->m_FibWord2000.fcPgdFtnOld = reader.ReadUInt32(); //986
				this->m_FibWord2000.lcbPgdFtnOld = reader.ReadUInt32(); //990
				this->m_FibWord2000.fcBkdFtnOld = reader.ReadUInt32(); //994
				this->m_FibWord2000.lcbBkdFtnOld = reader.ReadUInt32(); //998
				this->m_FibWord2000.fcPgdEdnOld = reader.ReadUInt32(); //1002
				this->m_FibWord2000.lcbPgdEdnOld = reader.ReadUInt32(); //1006
				this->m_FibWord2000.fcBkdEdnOld = reader.ReadUInt32(); //1010
				this->m_FibWord2000.lcbBkdEdnOld = reader.ReadUInt32(); //1014
			}
			if ( this->m_FibBase.nFib >= Fib2002 )
			{
				//Read also the fibRgFcLcb2002
				reader.ReadUInt32(); //1018
				reader.ReadUInt32(); //1022
				this->m_FibWord2002.fcPlcfPgp = reader.ReadUInt32(); //1026
				this->m_FibWord2002.lcbPlcfPgp = reader.ReadUInt32(); //1030
				this->m_FibWord2002.fcPlcfuim = reader.ReadUInt32(); //1034
				this->m_FibWord2002.lcbPlcfuim = reader.ReadUInt32(); //1038
				this->m_FibWord2002.fcPlfguidUim = reader.ReadUInt32(); //1042
				this->m_FibWord2002.lcbPlfguidUim = reader.ReadUInt32(); //1046
				this->m_FibWord2002.fcAtrdExtra = reader.ReadUInt32(); //1050
				this->m_FibWord2002.lcbAtrdExtra = reader.ReadUInt32(); //1054
				this->m_FibWord2002.fcPlrsid = reader.ReadUInt32(); //1058
				this->m_FibWord2002.lcbPlrsid = reader.ReadUInt32(); //1062
				this->m_FibWord2002.fcSttbfBkmkFactoid = reader.ReadUInt32(); //1066
				this->m_FibWord2002.lcbSttbfBkmkFactoid = reader.ReadUInt32(); //1070
				this->m_FibWord2002.fcPlcfBkfFactoid = reader.ReadUInt32(); //1074
				this->m_FibWord2002.lcbPlcfBkfFactoid = reader.ReadUInt32(); //1078
				this->m_FibWord2002.fcPlcfcookie = reader.ReadUInt32(); //1082
				this->m_FibWord2002.lcbPlcfcookie = reader.ReadUInt32(); //1086
				this->m_FibWord2002.fcPlcfBklFactoid = reader.ReadUInt32(); //1090
				this->m_FibWord2002.lcbPlcfBklFactoid = reader.ReadUInt32(); //1094
				this->m_FibWord2002.fcFactoidData = reader.ReadUInt32(); //1098
				this->m_FibWord2002.lcbFactoidData = reader.ReadUInt32(); //1102
				this->m_FibWord2002.fcDocUndo = reader.ReadUInt32(); //1106
				this->m_FibWord2002.lcbDocUndo = reader.ReadUInt32(); //1110
				this->m_FibWord2002.fcSttbfBkmkFcc = reader.ReadUInt32(); //1114
				this->m_FibWord2002.lcbSttbfBkmkFcc = reader.ReadUInt32(); //1118
				this->m_FibWord2002.fcPlcfBkfFcc = reader.ReadUInt32(); //1122
				this->m_FibWord2002.lcbPlcfBkfFcc = reader.ReadUInt32(); //1126
				this->m_FibWord2002.fcPlcfBklFcc = reader.ReadUInt32(); //1130
				this->m_FibWord2002.lcbPlcfBklFcc = reader.ReadUInt32(); //1134
				this->m_FibWord2002.fcSttbfbkmkBPRepairs = reader.ReadUInt32(); //1138
				this->m_FibWord2002.lcbSttbfbkmkBPRepairs = reader.ReadUInt32(); //1142
				this->m_FibWord2002.fcPlcfbkfBPRepairs = reader.ReadUInt32(); //1146
				this->m_FibWord2002.lcbPlcfbkfBPRepairs = reader.ReadUInt32(); //1150
				this->m_FibWord2002.fcPlcfbklBPRepairs = reader.ReadUInt32(); //1154
				this->m_FibWord2002.lcbPlcfbklBPRepairs = reader.ReadUInt32(); //1158
				this->m_FibWord2002.fcPmsNew = reader.ReadUInt32(); //1162
				this->m_FibWord2002.lcbPmsNew = reader.ReadUInt32(); //1166
				this->m_FibWord2002.fcODSO = reader.ReadUInt32(); //1170
				this->m_FibWord2002.lcbODSO = reader.ReadUInt32(); //1174
				this->m_FibWord2002.fcPlcfpmiOldXP = reader.ReadUInt32(); //1178
				this->m_FibWord2002.lcbPlcfpmiOldXP = reader.ReadUInt32(); //1182
				this->m_FibWord2002.fcPlcfpmiNewXP = reader.ReadUInt32(); //1186
				this->m_FibWord2002.lcbPlcfpmiNewXP = reader.ReadUInt32(); //1190
				this->m_FibWord2002.fcPlcfpmiMixedXP = reader.ReadUInt32(); //1194
				this->m_FibWord2002.lcbPlcfpmiMixedXP = reader.ReadUInt32(); //1198
				reader.ReadUInt32(); //1202
				reader.ReadUInt32(); //1206
				this->m_FibWord2002.fcPlcffactoid = reader.ReadUInt32(); //1210
				this->m_FibWord2002.lcbPlcffactoid = reader.ReadUInt32(); //1214
				this->m_FibWord2002.fcPlcflvcOldXP = reader.ReadUInt32(); //1218
				this->m_FibWord2002.lcbPlcflvcOldXP = reader.ReadUInt32(); //1222
				this->m_FibWord2002.fcPlcflvcNewXP = reader.ReadUInt32(); //1226
				this->m_FibWord2002.lcbPlcflvcNewXP = reader.ReadUInt32(); //1230
				this->m_FibWord2002.fcPlcflvcMixedXP = reader.ReadUInt32(); //1234
				this->m_FibWord2002.lcbPlcflvcMixedXP = reader.ReadUInt32(); //1238
			}
			if ( this->m_FibBase.nFib >= Fib2003 )
			{
				//Read also the fibRgFcLcb2003
				this->m_FibWord2003.fcHplxsdr = reader.ReadUInt32();
				this->m_FibWord2003.lcbHplxsdr = reader.ReadUInt32();
				this->m_FibWord2003.fcSttbfBkmkSdt = reader.ReadUInt32();
				this->m_FibWord2003.lcbSttbfBkmkSdt = reader.ReadUInt32();
				this->m_FibWord2003.fcPlcfBkfSdt = reader.ReadUInt32();
				this->m_FibWord2003.lcbPlcfBkfSdt = reader.ReadUInt32();
				this->m_FibWord2003.fcPlcfBklSdt = reader.ReadUInt32();
				this->m_FibWord2003.lcbPlcfBklSdt = reader.ReadUInt32();
				this->m_FibWord2003.fcCustomXForm = reader.ReadUInt32();
				this->m_FibWord2003.lcbCustomXForm = reader.ReadUInt32();
				this->m_FibWord2003.fcSttbfBkmkProt = reader.ReadUInt32();
				this->m_FibWord2003.lcbSttbfBkmkProt = reader.ReadUInt32();
				this->m_FibWord2003.fcPlcfBkfProt = reader.ReadUInt32();
				this->m_FibWord2003.lcbPlcfBkfProt = reader.ReadUInt32();
				this->m_FibWord2003.fcPlcfBklProt = reader.ReadUInt32();
				this->m_FibWord2003.lcbPlcfBklProt = reader.ReadUInt32();
				this->m_FibWord2003.fcSttbProtUser = reader.ReadUInt32();
				this->m_FibWord2003.lcbSttbProtUser = reader.ReadUInt32();
				reader.ReadUInt32();
				reader.ReadUInt32();
				this->m_FibWord2003.fcPlcfpmiOld = reader.ReadUInt32();
				this->m_FibWord2003.lcbPlcfpmiOld = reader.ReadUInt32();
				this->m_FibWord2003.fcPlcfpmiOldInline = reader.ReadUInt32();
				this->m_FibWord2003.lcbPlcfpmiOldInline = reader.ReadUInt32();
				this->m_FibWord2003.fcPlcfpmiNew = reader.ReadUInt32();
				this->m_FibWord2003.lcbPlcfpmiNew = reader.ReadUInt32();
				this->m_FibWord2003.fcPlcfpmiNewInline = reader.ReadUInt32();
				this->m_FibWord2003.lcbPlcfpmiNewInline = reader.ReadUInt32();
				this->m_FibWord2003.fcPlcflvcOld = reader.ReadUInt32();
				this->m_FibWord2003.lcbPlcflvcOld = reader.ReadUInt32();
				this->m_FibWord2003.fcPlcflvcOldInline = reader.ReadUInt32();
				this->m_FibWord2003.lcbPlcflvcOldInline = reader.ReadUInt32();
				this->m_FibWord2003.fcPlcflvcNew = reader.ReadUInt32();
				this->m_FibWord2003.lcbPlcflvcNew = reader.ReadUInt32();
				this->m_FibWord2003.fcPlcflvcNewInline = reader.ReadUInt32();
				this->m_FibWord2003.lcbPlcflvcNewInline = reader.ReadUInt32();
				this->m_FibWord2003.fcPgdMother = reader.ReadUInt32();
				this->m_FibWord2003.lcbPgdMother = reader.ReadUInt32();
				this->m_FibWord2003.fcBkdMother = reader.ReadUInt32();
				this->m_FibWord2003.lcbBkdMother = reader.ReadUInt32();
				this->m_FibWord2003.fcAfdMother = reader.ReadUInt32();
				this->m_FibWord2003.lcbAfdMother = reader.ReadUInt32();
				this->m_FibWord2003.fcPgdFtn = reader.ReadUInt32();
				this->m_FibWord2003.lcbPgdFtn = reader.ReadUInt32();
				this->m_FibWord2003.fcBkdFtn = reader.ReadUInt32();
				this->m_FibWord2003.lcbBkdFtn = reader.ReadUInt32();
				this->m_FibWord2003.fcAfdFtn = reader.ReadUInt32();
				this->m_FibWord2003.lcbAfdFtn = reader.ReadUInt32();
				this->m_FibWord2003.fcPgdEdn = reader.ReadUInt32();
				this->m_FibWord2003.lcbPgdEdn = reader.ReadUInt32();
				this->m_FibWord2003.fcBkdEdn = reader.ReadUInt32();
				this->m_FibWord2003.lcbBkdEdn = reader.ReadUInt32();
				this->m_FibWord2003.fcAfdEdn = reader.ReadUInt32();
				this->m_FibWord2003.lcbAfdEdn = reader.ReadUInt32();
				this->m_FibWord2003.fcAfd = reader.ReadUInt32();
				this->m_FibWord2003.lcbAfd = reader.ReadUInt32();
			}
			if ( this->m_FibBase.nFib >= Fib2007 )
			{
				//Read also the fibRgFcLcb2007
				this->m_FibWord2007.fcPlcfmthd = reader.ReadUInt32();
				this->m_FibWord2007.lcbPlcfmthd = reader.ReadUInt32();
				this->m_FibWord2007.fcSttbfBkmkMoveFrom = reader.ReadUInt32();
				this->m_FibWord2007.lcbSttbfBkmkMoveFrom = reader.ReadUInt32();
				this->m_FibWord2007.fcPlcfBkfMoveFrom = reader.ReadUInt32();
				this->m_FibWord2007.lcbPlcfBkfMoveFrom = reader.ReadUInt32();
				this->m_FibWord2007.fcPlcfBklMoveFrom = reader.ReadUInt32();
				this->m_FibWord2007.lcbPlcfBklMoveFrom = reader.ReadUInt32();
				this->m_FibWord2007.fcSttbfBkmkMoveTo = reader.ReadUInt32();
				this->m_FibWord2007.lcbSttbfBkmkMoveTo = reader.ReadUInt32();
				this->m_FibWord2007.fcPlcfBkfMoveTo = reader.ReadUInt32();
				this->m_FibWord2007.lcbPlcfBkfMoveTo = reader.ReadUInt32();
				this->m_FibWord2007.fcPlcfBklMoveTo = reader.ReadUInt32();
				this->m_FibWord2007.lcbPlcfBklMoveTo = reader.ReadUInt32();
				reader.ReadUInt32();
				reader.ReadUInt32();
				reader.ReadUInt32();
				reader.ReadUInt32();
				reader.ReadUInt32();
				reader.ReadUInt32();
				this->m_FibWord2007.fcSttbfBkmkArto = reader.ReadUInt32();
				this->m_FibWord2007.lcbSttbfBkmkArto = reader.ReadUInt32();
				this->m_FibWord2007.fcPlcfBkfArto = reader.ReadUInt32();
				this->m_FibWord2007.lcbPlcfBkfArto = reader.ReadUInt32();
				this->m_FibWord2007.fcPlcfBklArto = reader.ReadUInt32();
				this->m_FibWord2007.lcbPlcfBklArto = reader.ReadUInt32();
				this->m_FibWord2007.fcArtoData = reader.ReadUInt32();
				this->m_FibWord2007.lcbArtoData = reader.ReadUInt32();
				reader.ReadUInt32();
				reader.ReadUInt32();
				reader.ReadUInt32();
				reader.ReadUInt32();
				reader.ReadUInt32();
				reader.ReadUInt32();
				this->m_FibWord2007.fcOssTheme = reader.ReadUInt32();
				this->m_FibWord2007.lcbOssTheme = reader.ReadUInt32();
				this->m_FibWord2007.fcColorSchemeMapping = reader.ReadUInt32();
				this->m_FibWord2007.lcbColorSchemeMapping = reader.ReadUInt32();
			}

			this->cswNew = reader.ReadUInt16();

			if (this->cswNew != 0)
			{
				//Read the FibRgCswNew
				this->m_FibNew.nFibNew = (FibVersion)reader.ReadUInt16();
				this->m_FibNew.cQuickSavesNew = reader.ReadUInt16();
			}
		}
	};
}
#endif FileInformationBlock_h
