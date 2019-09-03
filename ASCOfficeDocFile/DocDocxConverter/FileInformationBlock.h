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
#pragma once

#include "VirtualStreamReader.h"
#include "../../ASCOfficeXlsFile2/source/XlsFormat/Crypt/Decryptor.h"

namespace DocFileFormat
{
	typedef enum FibVersion
	{
		Fib1983 = 0x002d,	// WinWord 2
		Fib1985 = 0x003F,	// 
//---------------------------------------------------------
		Fib1989 = 0x0045,	// 
		Fib1993 = 0x0065,	// Word 6.0
		Fib1995 = 0x0068,	// Word 7.0 (95)
//---------------------------------------------------------
		Fib1996 = 0x0069,	// Word 8.0 (97)		
		Fib1997 = 0x00C1,		
		Fib2000 = 0x00D9,
		Fib2002 = 0x0101,
		Fib2003 = 0x010C,
		Fib2007 = 0x0112
	} FibVersion;

	typedef struct FibBase
	{
		FibBase() :
	wIdent(0), 		nFib(Fib1997), 	lid(0), 	pnNext(0), 	fDot(false), 	fGlsy(false), 	fComplex(false),	fHasPic(false), 
	cQuickSaves(0), fEncrypted(false), fWhichTblStm(false), fReadOnlyRecommended(false), fWriteReservation(false), 
	fExtChar(false), fLoadOverwrite(false), fFarEast(false), fObfuscation(false), nFibBack(0), 	lKey(0), 	envr(0), fMac(false),
	fEmptySpecial(false), fLoadOverridePage(false), fFutureSavedUndo(false), fWord97Saved(false), fcMin(0), fcMac(0) {}
//------------------------------------------------------------
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
		bool fObfuscation;
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
		int cbMac = 0;
		int ccpText = 0;
		int ccpFtn = 0;
		int ccpHdr = 0;
		int ccpAtn = 0;
		int ccpEdn = 0;
		int ccpTxbx = 0;
		int ccpHdrTxbx = 0;
	} RgLw97;
	
	typedef struct FibWord2
	{
		unsigned int Spare  = 0;
		unsigned short rgwSpare0[3];

		unsigned int fcPlcMcr = 0;
		unsigned int lcbPlcMcr = 0;

		unsigned int fcPrEnv = 0;
		unsigned int lcbPrEnv = 0;

		unsigned int fcSpare5 = 0;
		unsigned int lcbSpare5 = 0;
		unsigned int fcSpare6 = 0;
		unsigned int lcbSpare6 = 0;
		unsigned int wSpare4 = 0;				
					
		unsigned int pnChpFirst = 0;
		unsigned int pnPapFirst = 0;
		unsigned int cpnBteChp = 0;
		unsigned int cpnBtePap = 0;	
	} FibWord2;

	typedef struct FibWord97
	{
		unsigned int fcStshfOrig = 0;
		unsigned int lcbStshfOrig = 0;
		unsigned int fcStshf = 0;
		unsigned int lcbStshf = 0;
		unsigned int fcPlcffndRef = 0;
		unsigned int lcbPlcffndRef = 0;
		unsigned int fcPlcffndTxt = 0;
		unsigned int lcbPlcffndTxt = 0;
		unsigned int fcPlcfandRef = 0;
		unsigned int lcbPlcfandRef = 0;
		unsigned int fcPlcfandTxt = 0;
		unsigned int lcbPlcfandTxt = 0;
		unsigned int fcPlcfSed = 0;
		unsigned int lcbPlcfSed = 0;
		unsigned int fcPlcPad = 0;
		unsigned int lcbPlcPad = 0;
		unsigned int fcPlcfPhe = 0;
		unsigned int lcbPlcfPhe = 0;
		unsigned int fcSttbfGlsy = 0;
		unsigned int lcbSttbfGlsy = 0;
		unsigned int fcPlcfGlsy = 0;
		unsigned int lcbPlcfGlsy = 0;
		unsigned int fcPlcfHdd = 0;
		unsigned int lcbPlcfHdd = 0;
		unsigned int fcPlcfBteChpx = 0;
		unsigned int lcbPlcfBteChpx = 0;
		unsigned int fcPlcfBtePapx = 0;
		unsigned int lcbPlcfBtePapx = 0;
		unsigned int fcPlcfSea = 0;
		unsigned int lcbPlcfSea = 0;
		unsigned int fcSttbfFfn = 0;
		unsigned int lcbSttbfFfn = 0;
		unsigned int fcPlcfFldMom = 0;
		unsigned int lcbPlcfFldMom = 0;
		unsigned int fcPlcfFldHdr = 0;
		unsigned int lcbPlcfFldHdr = 0;
		unsigned int fcPlcfFldFtn = 0;
		unsigned int lcbPlcfFldFtn = 0;
		unsigned int fcPlcfFldAtn = 0;
		unsigned int lcbPlcfFldAtn = 0;
		unsigned int fcPlcfFldMcr = 0;
		unsigned int lcbPlcfFldMcr = 0;
		unsigned int fcSttbfBkmk = 0;
		unsigned int lcbSttbfBkmk = 0;
		unsigned int fcPlcfBkf = 0;
		unsigned int lcbPlcfBkf = 0;
		unsigned int fcPlcfBkl = 0;
		unsigned int lcbPlcfBkl = 0;
		unsigned int fcCmds = 0;
		unsigned int lcbCmds = 0;
		unsigned int fcSttbfMcr = 0;
		unsigned int lcbSttbfMcr = 0;
		unsigned int fcPrDrvr = 0;
		unsigned int lcbPrDrvr = 0;
		unsigned int fcPrEnvPort = 0;
		unsigned int lcbPrEnvPort = 0;
		unsigned int fcPrEnvLand = 0;
		unsigned int lcbPrEnvLand = 0;
		unsigned int fcWss = 0;
		unsigned int lcbWss = 0;
		unsigned int fcDop = 0;
		unsigned int lcbDop = 0;
		unsigned int fcSttbfAssoc = 0;
		unsigned int lcbSttbfAssoc = 0;
		unsigned int fcClx = 0;
		unsigned int lcbClx = 0;
		unsigned int fcPlcfPgdFtn = 0;
		unsigned int lcbPlcfPgdFtn = 0;
		unsigned int fcAutosaveSource = 0;
		unsigned int lcbAutosaveSource = 0;
		unsigned int fcGrpXstAtnOwners = 0;
		unsigned int lcbGrpXstAtnOwners = 0;
		unsigned int fcSttbfAtnBkmk = 0;
		unsigned int lcbSttbfAtnBkmk = 0;
		unsigned int fcPlcSpaMom = 0;
		unsigned int lcbPlcSpaMom = 0;
		unsigned int fcPlcSpaHdr = 0;
		unsigned int lcbPlcSpaHdr = 0;
		unsigned int fcPlcfAtnBkf = 0;
		unsigned int lcbPlcfAtnBkf = 0;
		unsigned int fcPlcfAtnBkl = 0;
		unsigned int lcbPlcfAtnBkl = 0;
		unsigned int fcPms = 0;
		unsigned int lcbPms = 0;
		unsigned int fcFormFldSttbs = 0;
		unsigned int lcbFormFldSttbs = 0;
		unsigned int fcPlcfendRef = 0;
		unsigned int lcbPlcfendRef = 0;
		unsigned int fcPlcfendTxt = 0;
		unsigned int lcbPlcfendTxt = 0;
		unsigned int fcPlcfFldEdn = 0;
		unsigned int lcbPlcfFldEdn = 0;
		unsigned int fcDggInfo = 0;
		unsigned int lcbDggInfo = 0;
		unsigned int fcSttbfRMark = 0;
		unsigned int lcbSttbfRMark = 0;
		unsigned int fcSttbfCaption = 0;
		unsigned int lcbSttbfCaption = 0;
		unsigned int fcSttbfAutoCaption = 0;
		unsigned int lcbSttbfAutoCaption = 0;
		unsigned int fcPlcfWkb = 0;
		unsigned int lcbPlcfWkb = 0;
		unsigned int fcPlcfSpl = 0;
		unsigned int lcbPlcfSpl = 0;
		unsigned int fcPlcftxbxTxt = 0;
		unsigned int lcbPlcftxbxTxt = 0;
		unsigned int fcPlcfFldTxbx = 0;
		unsigned int lcbPlcfFldTxbx = 0;
		unsigned int fcPlcfHdrtxbxTxt = 0;
		unsigned int lcbPlcfHdrtxbxTxt = 0;
		unsigned int fcPlcffldHdrTxbx = 0;
		unsigned int lcbPlcffldHdrTxbx = 0;
		unsigned int fcStwUser = 0;
		unsigned int lcbStwUser = 0;
		unsigned int fcSttbTtmbd = 0;
		unsigned int lcbSttbTtmbd = 0;
		unsigned int fcCookieData = 0;
		unsigned int lcbCookieData = 0;
		unsigned int fcPgdMotherOldOld = 0;
		unsigned int lcbPgdMotherOldOld = 0;
		unsigned int fcBkdMotherOldOld = 0;
		unsigned int lcbBkdMotherOldOld = 0;
		unsigned int fcPgdFtnOldOld = 0;
		unsigned int lcbPgdFtnOldOld = 0;
		unsigned int fcBkdFtnOldOld = 0;
		unsigned int lcbBkdFtnOldOld = 0;
		unsigned int fcPgdEdnOldOld = 0;
		unsigned int lcbPgdEdnOldOld = 0;
		unsigned int fcBkdEdnOldOld = 0;
		unsigned int lcbBkdEdnOldOld = 0;
		unsigned int fcSttbfIntlFld = 0;
		unsigned int lcbSttbfIntlFld = 0;
		unsigned int fcRouteSlip = 0;
		unsigned int lcbRouteSlip = 0;
		unsigned int fcSttbSavedBy = 0;
		unsigned int lcbSttbSavedBy = 0;
		unsigned int fcSttbFnm = 0;
		unsigned int lcbSttbFnm = 0;
		unsigned int fcPlfLst = 0;
		unsigned int lcbPlfLst = 0;
		unsigned int fcPlfLfo = 0;
		unsigned int lcbPlfLfo = 0;
		unsigned int fcPlcfTxbxBkd = 0;
		unsigned int lcbPlcfTxbxBkd = 0;
		unsigned int fcPlcfTxbxHdrBkd = 0;
		unsigned int lcbPlcfTxbxHdrBkd = 0;
		unsigned int fcDocUndoWord9 = 0;
		unsigned int lcbDocUndoWord9 = 0;
		unsigned int fcRgbUse = 0;
		unsigned int lcbRgbUse = 0;
		unsigned int fcUsp = 0;
		unsigned int lcbUsp = 0;
		unsigned int fcUskf = 0;
		unsigned int lcbUskf = 0;
		unsigned int fcPlcupcRgbUse = 0;
		unsigned int lcbPlcupcRgbUse = 0;
		unsigned int fcPlcupcUsp = 0;
		unsigned int lcbPlcupcUsp = 0;
		unsigned int fcSttbGlsyStyle = 0;
		unsigned int lcbSttbGlsyStyle = 0;
		unsigned int fcPlgosl = 0;
		unsigned int lcbPlgosl = 0;
		unsigned int fcPlcocx = 0;
		unsigned int lcbPlcocx = 0;
		unsigned int fcPlcfBteLvc = 0;
		unsigned int lcbPlcfBteLvc = 0;
		unsigned int dwLowDateTime = 0;
		unsigned int dwHighDateTime = 0;
		unsigned int fcPlcfLvcPre10 = 0;
		unsigned int lcbPlcfLvcPre10 = 0;
		unsigned int fcPlcfAsumy = 0;
		unsigned int lcbPlcfAsumy = 0;
		unsigned int fcPlcfGram = 0;
		unsigned int lcbPlcfGram = 0;
		unsigned int fcSttbListNames = 0;
		unsigned int lcbSttbListNames = 0;
		unsigned int fcSttbfUssr = 0;
		unsigned int lcbSttbfUssr = 0;
	} FibWord97;

	typedef struct FibWord2000
	{
		unsigned int fcPlcfTch = 0;
		unsigned int lcbPlcfTch = 0;
		unsigned int fcRmdThreading = 0;
		unsigned int lcbRmdThreading = 0;
		unsigned int fcMid = 0;
		unsigned int lcbMid = 0;
		unsigned int fcSttbRgtplc = 0;
		unsigned int lcbSttbRgtplc = 0;
		unsigned int fcMsoEnvelope = 0;
		unsigned int lcbMsoEnvelope = 0;
		unsigned int fcPlcfLad = 0;
		unsigned int lcbPlcfLad = 0;
		unsigned int fcRgDofr = 0;
		unsigned int lcbRgDofr = 0;
		unsigned int fcPlcosl = 0;
		unsigned int lcbPlcosl = 0;
		unsigned int fcPlcfCookieOld = 0;
		unsigned int lcbPlcfCookieOld = 0;
		unsigned int fcPgdMotherOld = 0;
		unsigned int lcbPgdMotherOld = 0;
		unsigned int fcBkdMotherOld = 0;
		unsigned int lcbBkdMotherOld = 0;
		unsigned int fcPgdFtnOld = 0;
		unsigned int lcbPgdFtnOld = 0;
		unsigned int fcBkdFtnOld = 0;
		unsigned int lcbBkdFtnOld = 0;
		unsigned int fcPgdEdnOld = 0;
		unsigned int lcbPgdEdnOld = 0;
		unsigned int fcBkdEdnOld = 0;
		unsigned int lcbBkdEdnOld = 0;
	} FibWord2000;

	typedef struct FibWord2002
	{
		unsigned int fcPlcfPgp = 0;
		unsigned int lcbPlcfPgp = 0;
		unsigned int fcPlcfuim = 0;
		unsigned int lcbPlcfuim = 0;
		unsigned int fcPlfguidUim = 0;
		unsigned int lcbPlfguidUim = 0;
		unsigned int fcAtrdExtra = 0;
		unsigned int lcbAtrdExtra = 0;
		unsigned int fcPlrsid = 0;
		unsigned int lcbPlrsid = 0;
		unsigned int fcSttbfBkmkFactoid = 0;
		unsigned int lcbSttbfBkmkFactoid = 0;
		unsigned int fcPlcfBkfFactoid = 0;
		unsigned int lcbPlcfBkfFactoid = 0;
		unsigned int fcPlcfcookie = 0;
		unsigned int lcbPlcfcookie = 0;
		unsigned int fcPlcfBklFactoid = 0;
		unsigned int lcbPlcfBklFactoid = 0;
		unsigned int fcFactoidData = 0;
		unsigned int lcbFactoidData = 0;
		unsigned int fcDocUndo = 0;
		unsigned int lcbDocUndo = 0;
		unsigned int fcSttbfBkmkFcc = 0;
		unsigned int lcbSttbfBkmkFcc = 0;
		unsigned int fcPlcfBkfFcc = 0;
		unsigned int lcbPlcfBkfFcc = 0;
		unsigned int fcPlcfBklFcc = 0;
		unsigned int lcbPlcfBklFcc = 0;
		unsigned int fcSttbfbkmkBPRepairs = 0;
		unsigned int lcbSttbfbkmkBPRepairs = 0;
		unsigned int fcPlcfbkfBPRepairs = 0;
		unsigned int lcbPlcfbkfBPRepairs = 0;
		unsigned int fcPlcfbklBPRepairs = 0;
		unsigned int lcbPlcfbklBPRepairs = 0;
		unsigned int fcPmsNew = 0;
		unsigned int lcbPmsNew = 0;
		unsigned int fcODSO = 0;
		unsigned int lcbODSO = 0;
		unsigned int fcPlcfpmiOldXP = 0;
		unsigned int lcbPlcfpmiOldXP = 0;
		unsigned int fcPlcfpmiNewXP = 0;
		unsigned int lcbPlcfpmiNewXP = 0;
		unsigned int fcPlcfpmiMixedXP = 0;
		unsigned int lcbPlcfpmiMixedXP = 0;
		unsigned int fcPlcffactoid = 0;
		unsigned int lcbPlcffactoid = 0;
		unsigned int fcPlcflvcOldXP = 0;
		unsigned int lcbPlcflvcOldXP = 0;
		unsigned int fcPlcflvcNewXP = 0;
		unsigned int lcbPlcflvcNewXP = 0;
		unsigned int fcPlcflvcMixedXP = 0;
		unsigned int lcbPlcflvcMixedXP = 0;
	} FibWord2002;

	typedef struct FibWord2003
	{
		unsigned int fcHplxsdr = 0;
		unsigned int lcbHplxsdr = 0;
		unsigned int fcSttbfBkmkSdt = 0;
		unsigned int lcbSttbfBkmkSdt = 0;
		unsigned int fcPlcfBkfSdt = 0;
		unsigned int lcbPlcfBkfSdt = 0;
		unsigned int fcPlcfBklSdt = 0;
		unsigned int lcbPlcfBklSdt = 0;
		unsigned int fcCustomXForm = 0;
		unsigned int lcbCustomXForm = 0;
		unsigned int fcSttbfBkmkProt = 0;
		unsigned int lcbSttbfBkmkProt = 0;
		unsigned int fcPlcfBkfProt = 0;
		unsigned int lcbPlcfBkfProt = 0;
		unsigned int fcPlcfBklProt = 0;
		unsigned int lcbPlcfBklProt = 0;
		unsigned int fcSttbProtUser = 0;
		unsigned int lcbSttbProtUser = 0;
		unsigned int fcPlcfpmiOld = 0;
		unsigned int lcbPlcfpmiOld = 0;
		unsigned int fcPlcfpmiOldInline = 0;
		unsigned int lcbPlcfpmiOldInline = 0;
		unsigned int fcPlcfpmiNew = 0;
		unsigned int lcbPlcfpmiNew = 0;
		unsigned int fcPlcfpmiNewInline = 0;
		unsigned int lcbPlcfpmiNewInline = 0;
		unsigned int fcPlcflvcOld = 0;
		unsigned int lcbPlcflvcOld = 0;
		unsigned int fcPlcflvcOldInline = 0;
		unsigned int lcbPlcflvcOldInline = 0;
		unsigned int fcPlcflvcNew = 0;
		unsigned int lcbPlcflvcNew = 0;
		unsigned int fcPlcflvcNewInline = 0;
		unsigned int lcbPlcflvcNewInline = 0;
		unsigned int fcPgdMother = 0;
		unsigned int lcbPgdMother = 0;
		unsigned int fcBkdMother = 0;
		unsigned int lcbBkdMother = 0;
		unsigned int fcAfdMother = 0;
		unsigned int lcbAfdMother = 0;
		unsigned int fcPgdFtn = 0;
		unsigned int lcbPgdFtn = 0;
		unsigned int fcBkdFtn = 0;
		unsigned int lcbBkdFtn = 0;
		unsigned int fcAfdFtn = 0;
		unsigned int lcbAfdFtn = 0;
		unsigned int fcPgdEdn = 0;
		unsigned int lcbPgdEdn = 0;
		unsigned int fcBkdEdn = 0;
		unsigned int lcbBkdEdn = 0;
		unsigned int fcAfdEdn = 0;
		unsigned int lcbAfdEdn = 0;
		unsigned int fcAfd = 0;
		unsigned int lcbAfd = 0;
	} FibWord2003;

	typedef struct FibWord2007
	{
		unsigned int fcPlcfmthd = 0;
		unsigned int lcbPlcfmthd = 0;
		unsigned int fcSttbfBkmkMoveFrom = 0;
		unsigned int lcbSttbfBkmkMoveFrom = 0;
		unsigned int fcPlcfBkfMoveFrom = 0;
		unsigned int lcbPlcfBkfMoveFrom = 0;
		unsigned int fcPlcfBklMoveFrom = 0;
		unsigned int lcbPlcfBklMoveFrom = 0;
		unsigned int fcSttbfBkmkMoveTo = 0;
		unsigned int lcbSttbfBkmkMoveTo = 0;
		unsigned int fcPlcfBkfMoveTo = 0;
		unsigned int lcbPlcfBkfMoveTo = 0;
		unsigned int fcPlcfBklMoveTo = 0;
		unsigned int lcbPlcfBklMoveTo = 0;
		unsigned int fcSttbfBkmkArto = 0;
		unsigned int lcbSttbfBkmkArto = 0;
		unsigned int fcPlcfBkfArto = 0;
		unsigned int lcbPlcfBkfArto = 0;
		unsigned int fcPlcfBklArto = 0;
		unsigned int lcbPlcfBklArto = 0;
		unsigned int fcArtoData = 0;
		unsigned int lcbArtoData = 0;
		unsigned int fcOssTheme = 0;
		unsigned int lcbOssTheme = 0;
		unsigned int fcColorSchemeMapping = 0;
		unsigned int lcbColorSchemeMapping = 0;
	} FibWord2007;

	struct FibNew
	{
		FibNew() : nFibNew(Fib1997), cQuickSavesNew(0), lidThemeOther(0), lidThemeFE(0), lidThemeCS(0)  {}
		FibVersion nFibNew;
		WORD cQuickSavesNew;
		WORD lidThemeOther;
		WORD lidThemeFE;
		WORD lidThemeCS;
	};


	class FileInformationBlock
	{
	public:
		int				m_nWordVersion;
		int				m_CodePage;
		FibBase			m_FibBase;
		
		RgW97			m_RgW97;
		RgLw97			m_RgLw97;
		
		FibWord2		m_FibWord2;
		FibWord97		m_FibWord97;
		FibWord2000		m_FibWord2000;
		FibWord2002		m_FibWord2002;
		FibWord2003		m_FibWord2003;
		FibWord2007		m_FibWord2007;
		
		FibNew			m_FibNew;

		WORD			csw;
		WORD			cslw;
		WORD			cbRgFcLcb;
		WORD			cswNew;
		
		void reset( VirtualStreamReader reader )
		{
            if (m_FibBase.nFib > 0 && m_FibBase.nFib <= Fib1995)
			{
				/*fcSpare0	= */reader.ReadInt32();
				/*fcSpare1	= */reader.ReadInt32();
				/*fcSpare2	= */reader.ReadInt32();
				/*fcSpare3	= */reader.ReadInt32(); //48
				m_RgLw97.ccpText			= reader.ReadInt32(); 
				m_RgLw97.ccpFtn				= reader.ReadInt32();
				m_RgLw97.ccpHdr				= reader.ReadInt32();

                /*m_FibWord97.ccpMcr		= */reader.ReadInt32();
				m_RgLw97.ccpAtn				= reader.ReadInt32();

				if (m_FibBase.nFib > Fib1985)
				{
					m_RgLw97.ccpEdn				= reader.ReadInt32();
					m_RgLw97.ccpTxbx			= reader.ReadInt32();				
					m_RgLw97.ccpHdrTxbx			= reader.ReadInt32();
				}
				else
				{
					int	ccpSpare0				= reader.ReadInt32();	
					int	ccpSpare1				= reader.ReadInt32();
					int	ccpSpare2				= reader.ReadInt32();	
				}
				int	ccpSpare3					= reader.ReadInt32();	

				if (m_FibBase.nFib > Fib1985)
				{
					m_FibWord97.fcStshfOrig		= reader.ReadInt32();
					m_FibWord97.lcbStshfOrig	= reader.ReadInt32();
					m_FibWord97.fcStshf			= reader.ReadInt32();	//88
					m_FibWord97.lcbStshf		= reader.ReadInt32();
					m_FibWord97.fcPlcffndRef	= reader.ReadInt32();
					m_FibWord97.lcbPlcffndRef	= reader.ReadInt32();	

					m_FibWord97.fcPlcffndTxt	= reader.ReadInt32();//112
					m_FibWord97.lcbPlcffndTxt	= reader.ReadInt32();

					m_FibWord97.fcPlcfandRef	= reader.ReadInt32();
					m_FibWord97.lcbPlcfandRef	= reader.ReadInt32();

					m_FibWord97.fcPlcfandTxt	= reader.ReadInt32();
					m_FibWord97.lcbPlcfandTxt	= reader.ReadInt32();

					m_FibWord97.fcPlcfSed		= reader.ReadInt32(); //136
					m_FibWord97.lcbPlcfSed		= reader.ReadInt32();

					m_FibWord97.fcPlcPad		= reader.ReadInt32();
					m_FibWord97.lcbPlcPad		= reader.ReadInt32();

					m_FibWord97.fcPlcfPhe		= reader.ReadInt32();
					m_FibWord97.lcbPlcfPhe		= reader.ReadInt32();

					m_FibWord97.fcSttbfGlsy		= reader.ReadInt32();
					m_FibWord97.lcbSttbfGlsy	= reader.ReadInt32();
					
					m_FibWord97.fcPlcfGlsy		= reader.ReadInt32();
					m_FibWord97.lcbPlcfGlsy		= reader.ReadInt32();
					m_FibWord97.fcPlcfHdd		= reader.ReadInt32();
					m_FibWord97.lcbPlcfHdd		= reader.ReadInt32();

					m_FibWord97.fcPlcfBteChpx	= reader.ReadInt32();
					m_FibWord97.lcbPlcfBteChpx	= reader.ReadInt32();

					m_FibWord97.fcPlcfBtePapx 	= reader.ReadInt32();
					m_FibWord97.lcbPlcfBtePapx	= reader.ReadInt32();

					m_FibWord97.fcPlcfSea		= reader.ReadInt32();
					m_FibWord97.lcbPlcfSea		= reader.ReadInt32();

					m_FibWord97.fcSttbfFfn		= reader.ReadInt32();
					m_FibWord97.lcbSttbfFfn		= reader.ReadInt32();

					m_FibWord97.fcPlcfFldMom	= reader.ReadInt32(); //216
					m_FibWord97.lcbPlcfFldMom	= reader.ReadInt32();

					m_FibWord97.fcPlcfFldHdr	= reader.ReadInt32();
					m_FibWord97.lcbPlcfFldHdr	= reader.ReadInt32();

					m_FibWord97.fcPlcfFldFtn	= reader.ReadInt32();
					m_FibWord97.lcbPlcfFldFtn	= reader.ReadInt32();

					m_FibWord97.fcPlcfFldAtn	= reader.ReadInt32();
					m_FibWord97.lcbPlcfFldAtn	= reader.ReadInt32();

					m_FibWord97.fcPlcfFldMcr	= reader.ReadInt32();
					m_FibWord97.lcbPlcfFldMcr	= reader.ReadInt32();

					m_FibWord97.fcSttbfBkmk		= reader.ReadInt32();
					m_FibWord97.lcbSttbfBkmk	= reader.ReadInt32();

					m_FibWord97.fcPlcfBkf		= reader.ReadInt32();
					m_FibWord97.lcbPlcfBkf		= reader.ReadInt32();

					m_FibWord97.fcPlcfBkl		= reader.ReadInt32();
					m_FibWord97.lcbPlcfBkl		= reader.ReadInt32();

					m_FibWord97.fcCmds			= reader.ReadInt32();
					m_FibWord97.lcbCmds			= reader.ReadInt32();

					/*m_FibWord97.fcPlcMcr		= */reader.ReadInt32();
					/*m_FibWord97.lcbPlcMcr		= */reader.ReadInt32();

					m_FibWord97.fcSttbfMcr		= reader.ReadInt32();
					m_FibWord97.lcbSttbfMcr		= reader.ReadInt32();

					m_FibWord97.fcPrDrvr		= reader.ReadInt32();
					m_FibWord97.lcbPrDrvr		= reader.ReadInt32();

					m_FibWord97.fcPrEnvPort		= reader.ReadInt32();
					m_FibWord97.lcbPrEnvPort	= reader.ReadInt32();	//316

					m_FibWord97.fcPrEnvLand		= reader.ReadInt32();
					m_FibWord97.lcbPrEnvLand	= reader.ReadInt32();

					m_FibWord97.fcWss			= reader.ReadInt32();
					m_FibWord97.lcbWss			= reader.ReadInt32();

					m_FibWord97.fcDop			= reader.ReadInt32();
					m_FibWord97.lcbDop			= reader.ReadInt32();

					m_FibWord97.fcSttbfAssoc	= reader.ReadInt32();
					m_FibWord97.lcbSttbfAssoc	= reader.ReadInt32();

					m_FibWord97.fcClx			= reader.ReadInt32();
					m_FibWord97.lcbClx			= reader.ReadInt32(); 
					
					m_FibWord97.fcPlcfPgdFtn	= reader.ReadInt32();
					m_FibWord97.lcbPlcfPgdFtn	= reader.ReadInt32();

					m_FibWord97.fcAutosaveSource	= reader.ReadInt32();
					m_FibWord97.lcbAutosaveSource	= reader.ReadInt32();
					m_FibWord97.fcGrpXstAtnOwners	= reader.ReadInt32();
					m_FibWord97.lcbGrpXstAtnOwners	= reader.ReadInt32();
					
					m_FibWord97.fcSttbfAtnBkmk		= reader.ReadInt32();
					m_FibWord97.lcbSttbfAtnBkmk		= reader.ReadInt32();
					
					m_FibWord2.wSpare4				= reader.ReadInt16(); //392
					m_FibWord2.pnChpFirst			= reader.ReadInt16();
					m_FibWord2.pnPapFirst			= reader.ReadInt16();

					m_FibWord2.cpnBteChp			= reader.ReadInt16();
					m_FibWord2.cpnBtePap			= reader.ReadInt16();
					
					m_FibWord97.fcPlcSpaMom			= reader.ReadInt32();
					m_FibWord97.lcbPlcSpaMom		= reader.ReadInt32();

					m_FibWord97.fcPlcSpaHdr			= reader.ReadInt32();		/*fcPlcfdoaHdr*/
					m_FibWord97.lcbPlcSpaHdr		= reader.ReadInt32();		/*lcbPlcfdoaHdr*/

					/*m_FibWord97.fcUnused1			=*/ reader.ReadInt32();
					/*m_FibWord97.lcbUnused1		=*/ reader.ReadInt32();
					/*m_FibWord97.fcUnused2			=*/ reader.ReadInt32();
					/*m_FibWord97.lcbUnused2		=*/ reader.ReadInt32();

					m_FibWord97.fcPlcfAtnBkf		= reader.ReadInt32();
					m_FibWord97.lcbPlcfAtnBkf		= reader.ReadInt32();
					m_FibWord97.fcPlcfAtnBkl		= reader.ReadInt32();
					m_FibWord97.lcbPlcfAtnBkl		= reader.ReadInt32();
					m_FibWord97.fcPms				= reader.ReadInt32();
					m_FibWord97.lcbPms				= reader.ReadInt32();
					m_FibWord97.fcFormFldSttbs/*f*/	= reader.ReadInt32();
					m_FibWord97.lcbFormFldSttbs/*f*/= reader.ReadInt32();
					
					m_FibWord97.fcPlcfendRef		= reader.ReadInt32(); //466
					m_FibWord97.lcbPlcfendRef		= reader.ReadInt32();
					m_FibWord97.fcPlcfendTxt		= reader.ReadInt32();
					m_FibWord97.lcbPlcfendTxt		= reader.ReadInt32();
					m_FibWord97.fcPlcfFldEdn		= reader.ReadInt32();
					m_FibWord97.lcbPlcfFldEdn		= reader.ReadInt32();
					/*m_FibWord97.fcPlcfpgdEdn		=*/ reader.ReadInt32();
					/*m_FibWord97.lcbPlcfpgdEdn		=*/ reader.ReadInt32();
					/*m_FibWord97.fcUnused3			=*/ reader.ReadInt32();
					/*m_FibWord97.lcbUnused3		=*/ reader.ReadInt32();
					m_FibWord97.fcSttbfRMark		= reader.ReadInt32();
					m_FibWord97.lcbSttbfRMark		= reader.ReadInt32();
					m_FibWord97.fcSttbfCaption		= reader.ReadInt32();
					m_FibWord97.lcbSttbfCaption		= reader.ReadInt32();
					m_FibWord97.fcSttbfAutoCaption	= reader.ReadInt32();
					m_FibWord97.lcbSttbfAutoCaption	= reader.ReadInt32();
					m_FibWord97.fcPlcfWkb			= reader.ReadInt32();	//530
					m_FibWord97.lcbPlcfWkb			= reader.ReadInt32();
					/*m_FibWord97.fcUnused4			=*/ reader.ReadInt32();
					/*m_FibWord97.lcbUnused4		=*/ reader.ReadInt32();
					m_FibWord97.fcPlcftxbxTxt		= reader.ReadInt32();
					m_FibWord97.lcbPlcftxbxTxt		= reader.ReadInt32();
					m_FibWord97.fcPlcfFldTxbx		= reader.ReadInt32();
					m_FibWord97.lcbPlcfFldTxbx		= reader.ReadInt32();
					m_FibWord97.fcPlcfHdrtxbxTxt	= reader.ReadInt32();
					m_FibWord97.lcbPlcfHdrtxbxTxt	= reader.ReadInt32();
					m_FibWord97.fcPlcffldHdrTxbx	= reader.ReadInt32();
					m_FibWord97.lcbPlcffldHdrTxbx	= reader.ReadInt32();
					m_FibWord97.fcStwUser			= reader.ReadInt32();
					m_FibWord97.lcbStwUser			= reader.ReadInt32();
					m_FibWord97.fcSttbTtmbd			= reader.ReadInt32();
					m_FibWord97.lcbSttbTtmbd		= reader.ReadInt32();
					/*m_FibWord97.fcPlcunused		= */reader.ReadInt32();
					/*m_FibWord97.lcbUnused			= */reader.ReadInt32();
					
					m_FibWord97.fcPgdMotherOldOld	= reader.ReadInt32();	/*fcpgdMother.fcPgd*/	//602
					m_FibWord97.lcbPgdMotherOldOld	= reader.ReadInt32();	/*fcPgdMother.lcbPgd*/	
					m_FibWord97.fcBkdMotherOldOld	= reader.ReadInt32();	/*fcPgdMother.fcBkd*/
					m_FibWord97.lcbBkdMotherOldOld	= reader.ReadInt32();	/*fcPgdMother.lcbBkd*/ // ???? size 2 ????
					
					m_FibWord97.fcPgdFtnOldOld		= reader.ReadInt32();	/*fcPgdFtn.fcPgd*/
					m_FibWord97.lcbPgdFtnOldOld		= reader.ReadInt32();	/*rgfcpgdFtn.lcbPgd*/
					m_FibWord97.fcBkdFtnOldOld		= reader.ReadInt32();	/*rgfcpgdFtn.fcBkd*/
					m_FibWord97.lcbBkdFtnOldOld		= reader.ReadInt32();	/*rgfcpgdFtn.lcbBkd*/

					m_FibWord97.fcPgdEdnOldOld		= reader.ReadInt32();	/*fcpgdFtn.fcPgd*/
					m_FibWord97.lcbPgdEdnOldOld		= reader.ReadInt32();	/*rgfcpgdFtn.lcbPgd*/
					m_FibWord97.fcBkdEdnOldOld		= reader.ReadInt32();	/*rgfcpgdFtn.fcBkd*/
					m_FibWord97.lcbBkdEdnOldOld		= reader.ReadInt32();	/*rgfcpgdFtn.lcbBkd*/
					
					m_FibWord97.fcSttbfIntlFld		= reader.ReadInt32();
					m_FibWord97.lcbSttbfIntlFld		= reader.ReadInt32();
					m_FibWord97.fcRouteSlip			= reader.ReadInt32();	//656
					m_FibWord97.lcbRouteSlip		= reader.ReadInt32();
					m_FibWord97.fcSttbSavedBy		= reader.ReadInt32();
					m_FibWord97.lcbSttbSavedBy		= reader.ReadInt32();
					m_FibWord97.fcSttbFnm			= reader.ReadInt32();
					m_FibWord97.lcbSttbFnm			= reader.ReadInt32();	//676	
				}
				else
				{
					m_FibWord97.fcStshfOrig		= reader.ReadInt32();// 0x58
					m_FibWord97.lcbStshfOrig	= reader.ReadInt16();
					m_FibWord97.fcStshf			= reader.ReadInt32();
					m_FibWord97.lcbStshf		= reader.ReadInt16();//0x62
					
					m_FibWord97.fcPlcffndRef	= reader.ReadInt32();
					m_FibWord97.lcbPlcffndRef	= reader.ReadInt16();//0x68

					m_FibWord97.fcPlcffndTxt	= reader.ReadInt32();//0x6a
					m_FibWord97.lcbPlcffndTxt	= reader.ReadInt16();

					m_FibWord97.fcPlcfandRef	= reader.ReadInt32();//0x70
					m_FibWord97.lcbPlcfandRef	= reader.ReadInt16();
					
					m_FibWord97.fcPlcfandTxt	= reader.ReadInt32();//0x76
					m_FibWord97.lcbPlcfandTxt	= reader.ReadInt16();

					m_FibWord97.fcPlcfSed		= reader.ReadInt32(); //0x7c
					m_FibWord97.lcbPlcfSed		= reader.ReadInt16();

					m_FibWord97.fcPlcPad		= reader.ReadInt32();
					m_FibWord97.lcbPlcPad		= reader.ReadInt16();

					m_FibWord97.fcPlcfPhe		= reader.ReadInt32();// 0x88
					m_FibWord97.lcbPlcfPhe		= reader.ReadInt16();

					m_FibWord97.fcSttbfGlsy		= reader.ReadInt32();
					m_FibWord97.lcbSttbfGlsy	= reader.ReadInt16();
					
					m_FibWord97.fcPlcfGlsy		= reader.ReadInt32();
					m_FibWord97.lcbPlcfGlsy		= reader.ReadInt16();

					m_FibWord97.fcPlcfHdd		= reader.ReadInt32();// 0x9a
					m_FibWord97.lcbPlcfHdd		= reader.ReadInt16();

					m_FibWord97.fcPlcfBteChpx	= reader.ReadInt32();
					m_FibWord97.lcbPlcfBteChpx	= reader.ReadInt16();

					m_FibWord97.fcPlcfBtePapx 	= reader.ReadInt32();// 0xa0
					m_FibWord97.lcbPlcfBtePapx	= reader.ReadInt16();

					m_FibWord97.fcPlcfSea		= reader.ReadInt32();// 0xac
					m_FibWord97.lcbPlcfSea		= reader.ReadInt16();

					m_FibWord97.fcSttbfFfn		= reader.ReadInt32();// 0xb2
					m_FibWord97.lcbSttbfFfn		= reader.ReadInt16();

					m_FibWord97.fcPlcfFldMom	= reader.ReadInt32(); // 0xb8=184
					m_FibWord97.lcbPlcfFldMom	= reader.ReadInt16();

					m_FibWord97.fcPlcfFldHdr	= reader.ReadInt32();
					m_FibWord97.lcbPlcfFldHdr	= reader.ReadInt16();

					m_FibWord97.fcPlcfFldFtn	= reader.ReadInt32();
					m_FibWord97.lcbPlcfFldFtn	= reader.ReadInt16();

					m_FibWord97.fcPlcfFldAtn	= reader.ReadInt32();
					m_FibWord97.lcbPlcfFldAtn	= reader.ReadInt16();

					m_FibWord97.fcPlcfFldMcr	= reader.ReadInt32();
					m_FibWord97.lcbPlcfFldMcr	= reader.ReadInt16();

					m_FibWord97.fcSttbfBkmk		= reader.ReadInt32();// 0xd6
					m_FibWord97.lcbSttbfBkmk	= reader.ReadInt16();

					m_FibWord97.fcPlcfBkf		= reader.ReadInt32();
					m_FibWord97.lcbPlcfBkf		= reader.ReadInt16();

					m_FibWord97.fcPlcfBkl		= reader.ReadInt32();
					m_FibWord97.lcbPlcfBkl		= reader.ReadInt16();

					m_FibWord97.fcCmds			= reader.ReadInt32();// 0xe8
					m_FibWord97.lcbCmds			= reader.ReadInt16();

					m_FibWord2.fcPlcMcr			= reader.ReadInt32();// 0xee
					m_FibWord2.lcbPlcMcr		= reader.ReadInt16();

					m_FibWord97.fcSttbfMcr		= reader.ReadInt32();// 0xf4
					m_FibWord97.lcbSttbfMcr		= reader.ReadInt16();

					m_FibWord97.fcPrDrvr		= reader.ReadInt32();// 
					m_FibWord97.lcbPrDrvr		= reader.ReadInt16();

					m_FibWord97.fcPrEnvPort		= reader.ReadInt32();// 
					m_FibWord97.lcbPrEnvPort		= reader.ReadInt16();

					m_FibWord97.fcPrEnvLand		= reader.ReadInt32();// 
					m_FibWord97.lcbPrEnvLand	= reader.ReadInt16();

					m_FibWord97.fcWss			= reader.ReadInt32();// 268
					m_FibWord97.lcbWss			= reader.ReadInt16();

					m_FibWord97.fcDop			= reader.ReadInt32();
					m_FibWord97.lcbDop			= reader.ReadInt16();

					m_FibWord97.fcSttbfAssoc	= reader.ReadInt32();
					m_FibWord97.lcbSttbfAssoc	= reader.ReadInt16();;

					m_FibWord97.fcClx			= reader.ReadInt32(); 
					m_FibWord97.lcbClx			= reader.ReadInt16(); 
					
					m_FibWord97.fcPlcfPgdFtn	= reader.ReadInt32(); //292
					m_FibWord97.lcbPlcfPgdFtn	= reader.ReadInt16();

					m_FibWord97.fcAutosaveSource	= reader.ReadInt32();
					m_FibWord97.lcbAutosaveSource	= reader.ReadInt16();
					
					m_FibWord2.fcSpare5				= reader.ReadInt32();
					m_FibWord2.lcbSpare5			= reader.ReadInt16();
					m_FibWord2.fcSpare6				= reader.ReadInt32();
					m_FibWord2.lcbSpare6			= reader.ReadInt16();
					m_FibWord2.wSpare4				= reader.ReadInt16();					
					
					m_FibWord2.pnChpFirst			= reader.ReadInt16();
					m_FibWord2.pnPapFirst			= reader.ReadInt16();
					m_FibWord2.cpnBteChp			= reader.ReadInt16();
					m_FibWord2.cpnBtePap			= reader.ReadInt16();	
				}
			}
            if (m_FibBase.nFib > Fib1995 || m_FibBase.nFib == 0)
			{				
				int reserv1			=	reader.ReadInt32();
				int reserv2			=	reader.ReadInt32();
				
				m_RgLw97.ccpText	=	reader.ReadInt32(); //76
				m_RgLw97.ccpFtn		=	reader.ReadInt32(); //80
				m_RgLw97.ccpHdr		=	reader.ReadInt32(); //84
				
				int reserv3			=	reader.ReadInt32();
				
				m_RgLw97.ccpAtn		=	reader.ReadInt32(); //92
				m_RgLw97.ccpEdn		=	reader.ReadInt32(); //96
				m_RgLw97.ccpTxbx	=	reader.ReadInt32(); //100
				m_RgLw97.ccpHdrTxbx	=	reader.ReadInt32(); //104
				
				int reserv4			=	reader.ReadInt32();
				int reserv5			=	reader.ReadInt32();
				int reserv6			=	reader.ReadInt32();
				int reserv7			=	reader.ReadInt32();
				int reserv8			=	reader.ReadInt32();
				int reserv9			=	reader.ReadInt32();
				int reserv10		=	reader.ReadInt32();
				int reserv11		=	reader.ReadInt32();
				int reserv12		=	reader.ReadInt32();
				int reserv13		=	reader.ReadInt32();
				int reserv14		=	reader.ReadInt32();

				cbRgFcLcb			=	reader.ReadUInt16(); //152

				switch(cbRgFcLcb)
				{
					case 0x005D:	m_FibBase.nFib = Fib1997; break;
					case 0x006C:	m_FibBase.nFib = Fib2000; break;
					case 0x0088:	m_FibBase.nFib = Fib2002; break;
					case 0x00A4:	m_FibBase.nFib = Fib2003; break;
					case 0x00B7:	m_FibBase.nFib = Fib2007; break;
				}

				m_FibWord97.fcStshfOrig		= reader.ReadUInt32(); //154
				m_FibWord97.lcbStshfOrig	= reader.ReadUInt32(); //158
				m_FibWord97.fcStshf			= reader.ReadUInt32(); //162
				m_FibWord97.lcbStshf		= reader.ReadUInt32(); //166
				m_FibWord97.fcPlcffndRef	= reader.ReadUInt32(); //170
				m_FibWord97.lcbPlcffndRef	= reader.ReadUInt32(); //174
				m_FibWord97.fcPlcffndTxt	= reader.ReadUInt32(); //178
				m_FibWord97.lcbPlcffndTxt	= reader.ReadUInt32(); //182
				m_FibWord97.fcPlcfandRef	= reader.ReadUInt32(); //186
				m_FibWord97.lcbPlcfandRef	= reader.ReadUInt32(); //190
				m_FibWord97.fcPlcfandTxt	= reader.ReadUInt32(); //194
				m_FibWord97.lcbPlcfandTxt	= reader.ReadUInt32(); //198
				m_FibWord97.fcPlcfSed		= reader.ReadUInt32(); //202
				m_FibWord97.lcbPlcfSed		= reader.ReadUInt32(); //206
				m_FibWord97.fcPlcPad		= reader.ReadUInt32(); //210
				m_FibWord97.lcbPlcPad		= reader.ReadUInt32(); //214
				m_FibWord97.fcPlcfPhe		= reader.ReadUInt32(); //218
				m_FibWord97.lcbPlcfPhe		= reader.ReadUInt32(); //222
				m_FibWord97.fcSttbfGlsy		= reader.ReadUInt32(); //226
				m_FibWord97.lcbSttbfGlsy	= reader.ReadUInt32(); //230
				m_FibWord97.fcPlcfGlsy		= reader.ReadUInt32(); //234
				m_FibWord97.lcbPlcfGlsy		= reader.ReadUInt32(); //238
				m_FibWord97.fcPlcfHdd		= reader.ReadUInt32(); //242
				m_FibWord97.lcbPlcfHdd		= reader.ReadUInt32(); //246
				m_FibWord97.fcPlcfBteChpx	= reader.ReadUInt32(); //250
				m_FibWord97.lcbPlcfBteChpx	= reader.ReadUInt32(); //254
				m_FibWord97.fcPlcfBtePapx	= reader.ReadUInt32(); //258
				m_FibWord97.lcbPlcfBtePapx	= reader.ReadUInt32(); //262
				m_FibWord97.fcPlcfSea		= reader.ReadUInt32(); //266
				m_FibWord97.lcbPlcfSea		= reader.ReadUInt32(); //270
				m_FibWord97.fcSttbfFfn		= reader.ReadUInt32(); //274
				m_FibWord97.lcbSttbfFfn		= reader.ReadUInt32(); //278
				m_FibWord97.fcPlcfFldMom	= reader.ReadUInt32(); //282
				m_FibWord97.lcbPlcfFldMom	= reader.ReadUInt32(); //286
				m_FibWord97.fcPlcfFldHdr	= reader.ReadUInt32(); //290
				m_FibWord97.lcbPlcfFldHdr	= reader.ReadUInt32(); //294
				m_FibWord97.fcPlcfFldFtn	= reader.ReadUInt32(); //298
				m_FibWord97.lcbPlcfFldFtn	= reader.ReadUInt32(); //302
				m_FibWord97.fcPlcfFldAtn	= reader.ReadUInt32(); //306
				m_FibWord97.lcbPlcfFldAtn	= reader.ReadUInt32(); //310
				m_FibWord97.fcPlcfFldMcr	= reader.ReadUInt32(); //314
				m_FibWord97.lcbPlcfFldMcr	= reader.ReadUInt32(); //318
				m_FibWord97.fcSttbfBkmk		= reader.ReadUInt32(); //322
				m_FibWord97.lcbSttbfBkmk	= reader.ReadUInt32(); //326
				m_FibWord97.fcPlcfBkf		= reader.ReadUInt32(); //330
				m_FibWord97.lcbPlcfBkf		= reader.ReadUInt32(); //334
				m_FibWord97.fcPlcfBkl		= reader.ReadUInt32(); //338
				m_FibWord97.lcbPlcfBkl		= reader.ReadUInt32(); //342
				m_FibWord97.fcCmds			= reader.ReadUInt32(); //346
				m_FibWord97.lcbCmds			 = reader.ReadUInt32(); //350
				
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
				
				m_FibWord97.fcPlcSpaMom		= reader.ReadUInt32(); //474
				m_FibWord97.lcbPlcSpaMom	= reader.ReadUInt32(); //478
				m_FibWord97.fcPlcSpaHdr		= reader.ReadUInt32(); //482
				m_FibWord97.lcbPlcSpaHdr	= reader.ReadUInt32(); //486
				m_FibWord97.fcPlcfAtnBkf	= reader.ReadUInt32(); //490
				m_FibWord97.lcbPlcfAtnBkf	= reader.ReadUInt32(); //494
				m_FibWord97.fcPlcfAtnBkl	= reader.ReadUInt32(); //498
				m_FibWord97.lcbPlcfAtnBkl	= reader.ReadUInt32(); //502
				m_FibWord97.fcPms			= reader.ReadUInt32(); //506
				m_FibWord97.lcbPms			= reader.ReadUInt32(); //510
				m_FibWord97.fcFormFldSttbs	= reader.ReadUInt32(); //514
				m_FibWord97.lcbFormFldSttbs = reader.ReadUInt32(); //518
				m_FibWord97.fcPlcfendRef	= reader.ReadUInt32(); //522
				m_FibWord97.lcbPlcfendRef	= reader.ReadUInt32(); //526
				m_FibWord97.fcPlcfendTxt	= reader.ReadUInt32(); //530
				m_FibWord97.lcbPlcfendTxt	= reader.ReadUInt32(); //534
				m_FibWord97.fcPlcfFldEdn	= reader.ReadUInt32(); //538
				m_FibWord97.lcbPlcfFldEdn	= reader.ReadUInt32(); //542
				
				reader.ReadUInt32(); //546
				reader.ReadUInt32(); //550
				
				m_FibWord97.fcDggInfo			= reader.ReadUInt32(); //554
				m_FibWord97.lcbDggInfo			= reader.ReadUInt32(); //558
				m_FibWord97.fcSttbfRMark		= reader.ReadUInt32(); //562
				m_FibWord97.lcbSttbfRMark		= reader.ReadUInt32(); //566
				m_FibWord97.fcSttbfCaption		= reader.ReadUInt32(); //570
				m_FibWord97.lcbSttbfCaption		= reader.ReadUInt32(); //574
				m_FibWord97.fcSttbfAutoCaption	= reader.ReadUInt32(); //578
				m_FibWord97.lcbSttbfAutoCaption = reader.ReadUInt32(); //582
				m_FibWord97.fcPlcfWkb			= reader.ReadUInt32(); //586
				m_FibWord97.lcbPlcfWkb			= reader.ReadUInt32(); //590
				m_FibWord97.fcPlcfSpl			= reader.ReadUInt32(); //594
				m_FibWord97.lcbPlcfSpl			= reader.ReadUInt32(); //598
				m_FibWord97.fcPlcftxbxTxt		= reader.ReadUInt32(); //602
				m_FibWord97.lcbPlcftxbxTxt		= reader.ReadUInt32(); //606
				m_FibWord97.fcPlcfFldTxbx		= reader.ReadUInt32(); //610
				m_FibWord97.lcbPlcfFldTxbx		= reader.ReadUInt32(); //614
				m_FibWord97.fcPlcfHdrtxbxTxt	= reader.ReadUInt32(); //618
				m_FibWord97.lcbPlcfHdrtxbxTxt	= reader.ReadUInt32(); //622
				m_FibWord97.fcPlcffldHdrTxbx	= reader.ReadUInt32(); //626
				m_FibWord97.lcbPlcffldHdrTxbx	= reader.ReadUInt32(); //630
				m_FibWord97.fcStwUser			= reader.ReadUInt32(); //634
				m_FibWord97.lcbStwUser			= reader.ReadUInt32(); //638
				m_FibWord97.fcSttbTtmbd			= reader.ReadUInt32(); //642
				m_FibWord97.lcbSttbTtmbd		= reader.ReadUInt32(); //646
				m_FibWord97.fcCookieData		= reader.ReadUInt32(); //650
				m_FibWord97.lcbCookieData		= reader.ReadUInt32(); //654
				m_FibWord97.fcPgdMotherOldOld	= reader.ReadUInt32(); //658
				m_FibWord97.lcbPgdMotherOldOld	= reader.ReadUInt32(); //662
				m_FibWord97.fcBkdMotherOldOld	= reader.ReadUInt32(); //666
				m_FibWord97.lcbBkdMotherOldOld	= reader.ReadUInt32(); //670
				m_FibWord97.fcPgdFtnOldOld		= reader.ReadUInt32(); //674
				m_FibWord97.lcbPgdFtnOldOld		= reader.ReadUInt32(); //678
				m_FibWord97.fcBkdFtnOldOld		= reader.ReadUInt32(); //682
				m_FibWord97.lcbBkdFtnOldOld		= reader.ReadUInt32(); //686
				m_FibWord97.fcPgdEdnOldOld		= reader.ReadUInt32(); //690
				m_FibWord97.lcbPgdEdnOldOld		= reader.ReadUInt32(); //694
				m_FibWord97.fcBkdEdnOldOld		= reader.ReadUInt32(); //698
				m_FibWord97.lcbBkdEdnOldOld		= reader.ReadUInt32(); //702
				m_FibWord97.fcSttbfIntlFld		= reader.ReadUInt32(); //706
				m_FibWord97.lcbSttbfIntlFld		= reader.ReadUInt32(); //710
				m_FibWord97.fcRouteSlip			= reader.ReadUInt32(); //714
				m_FibWord97.lcbRouteSlip		= reader.ReadUInt32(); //718
				m_FibWord97.fcSttbSavedBy		= reader.ReadUInt32(); //722
				m_FibWord97.lcbSttbSavedBy		= reader.ReadUInt32(); //726
				m_FibWord97.fcSttbFnm			= reader.ReadUInt32(); //730
				m_FibWord97.lcbSttbFnm			= reader.ReadUInt32(); //734
				m_FibWord97.fcPlfLst			= reader.ReadUInt32(); //738
				m_FibWord97.lcbPlfLst			= reader.ReadUInt32(); //742
				m_FibWord97.fcPlfLfo			= reader.ReadUInt32(); //746
				m_FibWord97.lcbPlfLfo			= reader.ReadUInt32(); //750
				m_FibWord97.fcPlcfTxbxBkd		= reader.ReadUInt32(); //754
				m_FibWord97.lcbPlcfTxbxBkd		= reader.ReadUInt32(); //758
				m_FibWord97.fcPlcfTxbxHdrBkd	= reader.ReadUInt32(); //762
				m_FibWord97.lcbPlcfTxbxHdrBkd	= reader.ReadUInt32(); //766
				m_FibWord97.fcDocUndoWord9		= reader.ReadUInt32(); //770
				m_FibWord97.lcbDocUndoWord9		= reader.ReadUInt32(); //774
				m_FibWord97.fcRgbUse			= reader.ReadUInt32(); //778
				m_FibWord97.lcbRgbUse			= reader.ReadUInt32(); //782
				m_FibWord97.fcUsp				= reader.ReadUInt32(); //786
				m_FibWord97.lcbUsp				= reader.ReadUInt32(); //790
				m_FibWord97.fcUskf				= reader.ReadUInt32(); //794
				m_FibWord97.lcbUskf				= reader.ReadUInt32(); //798
				m_FibWord97.fcPlcupcRgbUse		= reader.ReadUInt32(); //802
				m_FibWord97.lcbPlcupcRgbUse		= reader.ReadUInt32(); //806
				m_FibWord97.fcPlcupcUsp			= reader.ReadUInt32(); //810
				m_FibWord97.lcbPlcupcUsp		= reader.ReadUInt32(); //814
				m_FibWord97.fcSttbGlsyStyle		= reader.ReadUInt32(); //818
				m_FibWord97.lcbSttbGlsyStyle	= reader.ReadUInt32(); //822
				m_FibWord97.fcPlgosl			= reader.ReadUInt32(); //826
				m_FibWord97.lcbPlgosl			= reader.ReadUInt32(); //830
				m_FibWord97.fcPlcocx			= reader.ReadUInt32(); //834
				m_FibWord97.lcbPlcocx			= reader.ReadUInt32(); //838
				m_FibWord97.fcPlcfBteLvc		= reader.ReadUInt32(); //842
				m_FibWord97.lcbPlcfBteLvc		= reader.ReadUInt32(); //846
				m_FibWord97.dwLowDateTime		= reader.ReadUInt32(); //850
				m_FibWord97.dwHighDateTime		= reader.ReadUInt32(); //854
				m_FibWord97.fcPlcfLvcPre10		= reader.ReadUInt32(); //858
				m_FibWord97.lcbPlcfLvcPre10		= reader.ReadUInt32(); //862
				m_FibWord97.fcPlcfAsumy			= reader.ReadUInt32(); //866
				m_FibWord97.lcbPlcfAsumy		= reader.ReadUInt32(); //870
				m_FibWord97.fcPlcfGram			= reader.ReadUInt32(); //874
				m_FibWord97.lcbPlcfGram			= reader.ReadUInt32(); //878
				m_FibWord97.fcSttbListNames		= reader.ReadUInt32(); //882
				m_FibWord97.lcbSttbListNames	= reader.ReadUInt32(); //886
				m_FibWord97.fcSttbfUssr			= reader.ReadUInt32(); //890
				m_FibWord97.lcbSttbfUssr		= reader.ReadUInt32(); //894
			}

			if (m_FibBase.nFib >= Fib2000)
			{
				//Read also the FibRgFcLcb2000
				m_FibWord2000.fcPlcfTch			= reader.ReadUInt32(); //898
				m_FibWord2000.lcbPlcfTch		= reader.ReadUInt32(); //902
				m_FibWord2000.fcRmdThreading	= reader.ReadUInt32(); //906
				m_FibWord2000.lcbRmdThreading	= reader.ReadUInt32(); //910
				m_FibWord2000.fcMid				= reader.ReadUInt32(); //914
				m_FibWord2000.lcbMid			= reader.ReadUInt32(); //918
				m_FibWord2000.fcSttbRgtplc		= reader.ReadUInt32(); //922
				m_FibWord2000.lcbSttbRgtplc		= reader.ReadUInt32(); //926
				m_FibWord2000.fcMsoEnvelope		= reader.ReadUInt32(); //930
				m_FibWord2000.lcbMsoEnvelope	= reader.ReadUInt32(); //934
				m_FibWord2000.fcPlcfLad			= reader.ReadUInt32(); //938
				m_FibWord2000.lcbPlcfLad		= reader.ReadUInt32(); //942
				m_FibWord2000.fcRgDofr			= reader.ReadUInt32(); //946
				m_FibWord2000.lcbRgDofr			= reader.ReadUInt32(); //950
				m_FibWord2000.fcPlcosl			= reader.ReadUInt32(); //954
				m_FibWord2000.lcbPlcosl			= reader.ReadUInt32(); //958
				m_FibWord2000.fcPlcfCookieOld	= reader.ReadUInt32(); //962
				m_FibWord2000.lcbPlcfCookieOld	= reader.ReadUInt32(); //966
				m_FibWord2000.fcPgdMotherOld	= reader.ReadUInt32(); //970
				m_FibWord2000.lcbPgdMotherOld	= reader.ReadUInt32(); //974
				m_FibWord2000.fcBkdMotherOld	= reader.ReadUInt32(); //978
				m_FibWord2000.lcbBkdMotherOld	= reader.ReadUInt32(); //982
				m_FibWord2000.fcPgdFtnOld		= reader.ReadUInt32(); //986
				m_FibWord2000.lcbPgdFtnOld		= reader.ReadUInt32(); //990
				m_FibWord2000.fcBkdFtnOld		= reader.ReadUInt32(); //994
				m_FibWord2000.lcbBkdFtnOld		= reader.ReadUInt32(); //998
				m_FibWord2000.fcPgdEdnOld		= reader.ReadUInt32(); //1002
				m_FibWord2000.lcbPgdEdnOld		= reader.ReadUInt32(); //1006
				m_FibWord2000.fcBkdEdnOld		= reader.ReadUInt32(); //1010
				m_FibWord2000.lcbBkdEdnOld		= reader.ReadUInt32(); //1014
			}
			if ( m_FibBase.nFib >= Fib2002 )
			{
				//Read also the fibRgFcLcb2002
				reader.ReadUInt32(); //1018
				reader.ReadUInt32(); //1022
				m_FibWord2002.fcPlcfPgp			= reader.ReadUInt32(); //1026
				m_FibWord2002.lcbPlcfPgp		= reader.ReadUInt32(); //1030
				m_FibWord2002.fcPlcfuim			= reader.ReadUInt32(); //1034
				m_FibWord2002.lcbPlcfuim		= reader.ReadUInt32(); //1038
				m_FibWord2002.fcPlfguidUim		= reader.ReadUInt32(); //1042
				m_FibWord2002.lcbPlfguidUim		= reader.ReadUInt32(); //1046
				m_FibWord2002.fcAtrdExtra		= reader.ReadUInt32(); //1050
				m_FibWord2002.lcbAtrdExtra		= reader.ReadUInt32(); //1054
				m_FibWord2002.fcPlrsid			= reader.ReadUInt32(); //1058
				m_FibWord2002.lcbPlrsid			= reader.ReadUInt32(); //1062
				m_FibWord2002.fcSttbfBkmkFactoid = reader.ReadUInt32(); //1066
				m_FibWord2002.lcbSttbfBkmkFactoid = reader.ReadUInt32(); //1070
				m_FibWord2002.fcPlcfBkfFactoid	= reader.ReadUInt32(); //1074
				m_FibWord2002.lcbPlcfBkfFactoid = reader.ReadUInt32(); //1078
				m_FibWord2002.fcPlcfcookie		= reader.ReadUInt32(); //1082
				m_FibWord2002.lcbPlcfcookie		= reader.ReadUInt32(); //1086
				m_FibWord2002.fcPlcfBklFactoid	= reader.ReadUInt32(); //1090
				m_FibWord2002.lcbPlcfBklFactoid	= reader.ReadUInt32(); //1094
				m_FibWord2002.fcFactoidData		= reader.ReadUInt32(); //1098
				m_FibWord2002.lcbFactoidData	= reader.ReadUInt32(); //1102
				m_FibWord2002.fcDocUndo			= reader.ReadUInt32(); //1106
				m_FibWord2002.lcbDocUndo		= reader.ReadUInt32(); //1110
				m_FibWord2002.fcSttbfBkmkFcc	= reader.ReadUInt32(); //1114
				m_FibWord2002.lcbSttbfBkmkFcc	= reader.ReadUInt32(); //1118
				m_FibWord2002.fcPlcfBkfFcc		= reader.ReadUInt32(); //1122
				m_FibWord2002.lcbPlcfBkfFcc		= reader.ReadUInt32(); //1126
				m_FibWord2002.fcPlcfBklFcc		= reader.ReadUInt32(); //1130
				m_FibWord2002.lcbPlcfBklFcc		= reader.ReadUInt32(); //1134
				m_FibWord2002.fcSttbfbkmkBPRepairs = reader.ReadUInt32(); //1138
				m_FibWord2002.lcbSttbfbkmkBPRepairs = reader.ReadUInt32(); //1142
				m_FibWord2002.fcPlcfbkfBPRepairs = reader.ReadUInt32(); //1146
				m_FibWord2002.lcbPlcfbkfBPRepairs = reader.ReadUInt32(); //1150
				m_FibWord2002.fcPlcfbklBPRepairs = reader.ReadUInt32(); //1154
				m_FibWord2002.lcbPlcfbklBPRepairs = reader.ReadUInt32(); //1158
				m_FibWord2002.fcPmsNew			= reader.ReadUInt32(); //1162
				m_FibWord2002.lcbPmsNew			= reader.ReadUInt32(); //1166
				m_FibWord2002.fcODSO			= reader.ReadUInt32(); //1170
				m_FibWord2002.lcbODSO			= reader.ReadUInt32(); //1174
				m_FibWord2002.fcPlcfpmiOldXP	= reader.ReadUInt32(); //1178
				m_FibWord2002.lcbPlcfpmiOldXP = reader.ReadUInt32(); //1182
				m_FibWord2002.fcPlcfpmiNewXP	= reader.ReadUInt32(); //1186
				m_FibWord2002.lcbPlcfpmiNewXP	= reader.ReadUInt32(); //1190
				m_FibWord2002.fcPlcfpmiMixedXP	= reader.ReadUInt32(); //1194
				m_FibWord2002.lcbPlcfpmiMixedXP = reader.ReadUInt32(); //1198
				reader.ReadUInt32(); //1202
				reader.ReadUInt32(); //1206
				m_FibWord2002.fcPlcffactoid		= reader.ReadUInt32(); //1210
				m_FibWord2002.lcbPlcffactoid	= reader.ReadUInt32(); //1214
				m_FibWord2002.fcPlcflvcOldXP	= reader.ReadUInt32(); //1218
				m_FibWord2002.lcbPlcflvcOldXP	= reader.ReadUInt32(); //1222
				m_FibWord2002.fcPlcflvcNewXP	= reader.ReadUInt32(); //1226
				m_FibWord2002.lcbPlcflvcNewXP	= reader.ReadUInt32(); //1230
				m_FibWord2002.fcPlcflvcMixedXP	= reader.ReadUInt32(); //1234
				m_FibWord2002.lcbPlcflvcMixedXP	= reader.ReadUInt32(); //1238
			}
			if ( m_FibBase.nFib >= Fib2003 )
			{
				//Read also the fibRgFcLcb2003
				m_FibWord2003.fcHplxsdr			= reader.ReadUInt32();
				m_FibWord2003.lcbHplxsdr		= reader.ReadUInt32();
				m_FibWord2003.fcSttbfBkmkSdt	= reader.ReadUInt32();
				m_FibWord2003.lcbSttbfBkmkSdt	= reader.ReadUInt32();
				m_FibWord2003.fcPlcfBkfSdt		= reader.ReadUInt32();
				m_FibWord2003.lcbPlcfBkfSdt		= reader.ReadUInt32();
				m_FibWord2003.fcPlcfBklSdt		= reader.ReadUInt32();
				m_FibWord2003.lcbPlcfBklSdt		= reader.ReadUInt32();
				m_FibWord2003.fcCustomXForm		= reader.ReadUInt32();
				m_FibWord2003.lcbCustomXForm	= reader.ReadUInt32();
				m_FibWord2003.fcSttbfBkmkProt	= reader.ReadUInt32();
				m_FibWord2003.lcbSttbfBkmkProt	= reader.ReadUInt32();
				m_FibWord2003.fcPlcfBkfProt		= reader.ReadUInt32();
				m_FibWord2003.lcbPlcfBkfProt	= reader.ReadUInt32();
				m_FibWord2003.fcPlcfBklProt		= reader.ReadUInt32();
				m_FibWord2003.lcbPlcfBklProt	= reader.ReadUInt32();
				m_FibWord2003.fcSttbProtUser	= reader.ReadUInt32();
				m_FibWord2003.lcbSttbProtUser	= reader.ReadUInt32();
				reader.ReadUInt32();
				reader.ReadUInt32();
				m_FibWord2003.fcPlcfpmiOld		= reader.ReadUInt32();
				m_FibWord2003.lcbPlcfpmiOld		= reader.ReadUInt32();
				m_FibWord2003.fcPlcfpmiOldInline= reader.ReadUInt32();
				m_FibWord2003.lcbPlcfpmiOldInline = reader.ReadUInt32();
				m_FibWord2003.fcPlcfpmiNew		= reader.ReadUInt32();
				m_FibWord2003.lcbPlcfpmiNew		= reader.ReadUInt32();
				m_FibWord2003.fcPlcfpmiNewInline= reader.ReadUInt32();
				m_FibWord2003.lcbPlcfpmiNewInline = reader.ReadUInt32();
				m_FibWord2003.fcPlcflvcOld		= reader.ReadUInt32();
				m_FibWord2003.lcbPlcflvcOld		= reader.ReadUInt32();
				m_FibWord2003.fcPlcflvcOldInline= reader.ReadUInt32();
				m_FibWord2003.lcbPlcflvcOldInline = reader.ReadUInt32();
				m_FibWord2003.fcPlcflvcNew		= reader.ReadUInt32();
				m_FibWord2003.lcbPlcflvcNew		= reader.ReadUInt32();
				m_FibWord2003.fcPlcflvcNewInline= reader.ReadUInt32();
				m_FibWord2003.lcbPlcflvcNewInline = reader.ReadUInt32();
				m_FibWord2003.fcPgdMother		= reader.ReadUInt32();
				m_FibWord2003.lcbPgdMother		= reader.ReadUInt32();
				m_FibWord2003.fcBkdMother		= reader.ReadUInt32();
				m_FibWord2003.lcbBkdMother		= reader.ReadUInt32();
				m_FibWord2003.fcAfdMother		= reader.ReadUInt32();
				m_FibWord2003.lcbAfdMother		= reader.ReadUInt32();
				m_FibWord2003.fcPgdFtn			= reader.ReadUInt32();
				m_FibWord2003.lcbPgdFtn			= reader.ReadUInt32();
				m_FibWord2003.fcBkdFtn			= reader.ReadUInt32();
				m_FibWord2003.lcbBkdFtn			= reader.ReadUInt32();
				m_FibWord2003.fcAfdFtn			= reader.ReadUInt32();
				m_FibWord2003.lcbAfdFtn			= reader.ReadUInt32();
				m_FibWord2003.fcPgdEdn			= reader.ReadUInt32();
				m_FibWord2003.lcbPgdEdn			= reader.ReadUInt32();
				m_FibWord2003.fcBkdEdn			= reader.ReadUInt32();
				m_FibWord2003.lcbBkdEdn			= reader.ReadUInt32();
				m_FibWord2003.fcAfdEdn			= reader.ReadUInt32();
				m_FibWord2003.lcbAfdEdn			= reader.ReadUInt32();
				m_FibWord2003.fcAfd				= reader.ReadUInt32();
				m_FibWord2003.lcbAfd			= reader.ReadUInt32();
			}
			if ( m_FibBase.nFib >= Fib2007 )
			{
				//Read also the fibRgFcLcb2007
				m_FibWord2007.fcPlcfmthd		= reader.ReadUInt32();
				m_FibWord2007.lcbPlcfmthd		= reader.ReadUInt32();
				m_FibWord2007.fcSttbfBkmkMoveFrom = reader.ReadUInt32();
				m_FibWord2007.lcbSttbfBkmkMoveFrom = reader.ReadUInt32();
				m_FibWord2007.fcPlcfBkfMoveFrom	= reader.ReadUInt32();
				m_FibWord2007.lcbPlcfBkfMoveFrom= reader.ReadUInt32();
				m_FibWord2007.fcPlcfBklMoveFrom	= reader.ReadUInt32();
				m_FibWord2007.lcbPlcfBklMoveFrom= reader.ReadUInt32();
				m_FibWord2007.fcSttbfBkmkMoveTo	= reader.ReadUInt32();
				m_FibWord2007.lcbSttbfBkmkMoveTo= reader.ReadUInt32();
				m_FibWord2007.fcPlcfBkfMoveTo	= reader.ReadUInt32();
				m_FibWord2007.lcbPlcfBkfMoveTo	= reader.ReadUInt32();
				m_FibWord2007.fcPlcfBklMoveTo	= reader.ReadUInt32();
				m_FibWord2007.lcbPlcfBklMoveTo	= reader.ReadUInt32();
				reader.ReadUInt32();
				reader.ReadUInt32();
				reader.ReadUInt32();
				reader.ReadUInt32();
				reader.ReadUInt32();
				reader.ReadUInt32();
				m_FibWord2007.fcSttbfBkmkArto	= reader.ReadUInt32();
				m_FibWord2007.lcbSttbfBkmkArto	= reader.ReadUInt32();
				m_FibWord2007.fcPlcfBkfArto		= reader.ReadUInt32();
				m_FibWord2007.lcbPlcfBkfArto	= reader.ReadUInt32();
				m_FibWord2007.fcPlcfBklArto		= reader.ReadUInt32();
				m_FibWord2007.lcbPlcfBklArto	= reader.ReadUInt32();
				m_FibWord2007.fcArtoData		= reader.ReadUInt32();
				m_FibWord2007.lcbArtoData		= reader.ReadUInt32();
				reader.ReadUInt32();
				reader.ReadUInt32();
				reader.ReadUInt32();
				reader.ReadUInt32();
				reader.ReadUInt32();
				reader.ReadUInt32();
				m_FibWord2007.fcOssTheme			= reader.ReadUInt32();
				m_FibWord2007.lcbOssTheme			= reader.ReadUInt32();
				m_FibWord2007.fcColorSchemeMapping	= reader.ReadUInt32();
				m_FibWord2007.lcbColorSchemeMapping	= reader.ReadUInt32();
			}

			cswNew = reader.ReadUInt16();

			if (cswNew != 0)
			{
	//Read the FibRgCswNew
				m_FibNew.nFibNew = (FibVersion)reader.ReadUInt16();
				
				if (m_FibNew.nFibNew == 0) m_FibNew.nFibNew = Fib1997;

				m_FibNew.cQuickSavesNew = reader.ReadUInt16();
			
				if (m_FibNew.nFibNew == 0x00D9 ||
					m_FibNew.nFibNew == 0x0101 ||
					m_FibNew.nFibNew == 0x010C )
				{
				}
				else if (m_FibNew.nFibNew == 0x0112)
				{
					m_FibNew.lidThemeOther	= reader.ReadUInt16();
					m_FibNew.lidThemeFE		= reader.ReadUInt16();
					m_FibNew.lidThemeCS		= reader.ReadUInt16();
				}
			}
		}
		FileInformationBlock( VirtualStreamReader reader )
		{
			m_nWordVersion	= 0;
			m_CodePage		= 1250;

			unsigned int flag16 = 0;
			unsigned char flag8 = 0;

			//read the FIB base
			m_FibBase.wIdent	= reader.ReadUInt16(); //0
			m_FibBase.nFib		= (FibVersion)reader.ReadUInt16(); //2
			
			reader.ReadBytes( 2, false ); //4 //nProduct

			m_FibBase.lid				= reader.ReadUInt16(); //6
			m_FibBase.pnNext			= reader.ReadInt16(); //8
			
			flag16 = reader.ReadUInt16(); //10
			
			m_FibBase.fDot				= ((flag16 & 0x0001) >> 2) != 0;
			m_FibBase.fGlsy				= ((flag16 & 0x0002) >> 1) != 0;
			m_FibBase.fComplex			= ((flag16 & 0x0004) >> 2) != 0;
            m_FibBase.fHasPic			= ((flag16 & 0x0008) >> 3) != 0;
            m_FibBase.cQuickSaves		= (WORD)(((int)flag16 & 0x00F0) >> 4);
            m_FibBase.fEncrypted		= FormatUtils::BitmaskToBool((int)flag16, 0x0100);
			m_FibBase.fWhichTblStm		= FormatUtils::BitmaskToBool((int)flag16, 0x0200);
			m_FibBase.fReadOnlyRecommended	= FormatUtils::BitmaskToBool((int)flag16, 0x0400);
			m_FibBase.fWriteReservation	= FormatUtils::BitmaskToBool((int)flag16, 0x0800);
			m_FibBase.fExtChar			= FormatUtils::BitmaskToBool((int)flag16, 0x1000);
			m_FibBase.fLoadOverwrite	= FormatUtils::BitmaskToBool((int)flag16, 0x2000);
			m_FibBase.fFarEast			= FormatUtils::BitmaskToBool((int)flag16, 0x4000);
			m_FibBase.fObfuscation		= FormatUtils::BitmaskToBool((int)flag16, 0x8000);
			
			m_FibBase.nFibBack			= reader.ReadUInt16(); //12

			if (m_FibBase.nFib < Fib1989)
			{			
				m_FibWord2.Spare = reader.ReadInt32();
				m_FibWord2.rgwSpare0[0] = reader.ReadUInt16();
				m_FibWord2.rgwSpare0[1] = reader.ReadUInt16();
				m_FibWord2.rgwSpare0[2] = reader.ReadUInt16();
			}
			else
			{
				m_FibBase.lKey				= reader.ReadInt32(); //14
				m_FibBase.envr				= reader.ReadByte(); //18
				
				flag8 = reader.ReadByte(); //19
				
				m_FibBase.fMac				= FormatUtils::BitmaskToBool((int)flag8, 0x01);
				m_FibBase.fEmptySpecial		= FormatUtils::BitmaskToBool((int)flag8, 0x02);
				m_FibBase.fLoadOverridePage	= FormatUtils::BitmaskToBool((int)flag8, 0x04);
				m_FibBase.fFutureSavedUndo	= FormatUtils::BitmaskToBool((int)flag8, 0x08);
				m_FibBase.fWord97Saved		= FormatUtils::BitmaskToBool((int)flag8, 0x10);
				
				reader.ReadBytes( 4, false ); //20
			}
			
			m_FibBase.fcMin = reader.ReadInt32(); //24
			m_FibBase.fcMac = reader.ReadInt32(); //28

			if (m_FibBase.nFib  > Fib1995)
				csw = reader.ReadUInt16(); //32

			if (m_FibBase.nFib > 0 && m_FibBase.nFib <= Fib1995)
			{
				m_RgLw97.cbMac = reader.ReadInt32();//32
			}
			else if (m_FibBase.nFib > Fib1995 || m_FibBase.nFib == 0)
			{					
			//read the RgW97
				int reserv1		= reader.ReadUInt16();
				int reserv2		= reader.ReadUInt16();
				int reserv3		= reader.ReadUInt16();
				int reserv4		= reader.ReadUInt16();
				int reserv5		= reader.ReadUInt16();
				int reserv6		= reader.ReadUInt16();
				int reserv7		= reader.ReadUInt16();
				int reserv8		= reader.ReadUInt16();
				int reserv9		= reader.ReadUInt16();
				int reserv10	= reader.ReadUInt16();
				int reserv11	= reader.ReadUInt16();
				int reserv12	= reader.ReadUInt16();
				int reserv13	= reader.ReadUInt16();
				
				m_RgW97.lidFE = reader.ReadUInt16(); //60

				cslw = reader.ReadUInt16(); //62

			//read the RgLW97

				m_RgLw97.cbMac =	reader.ReadInt32(); //64
			}
			reset(reader);
		}
	};
}
