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
#include "../XlsFile/Format/Crypt/Decryptor.h"

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
		
		void reset( VirtualStreamReader reader );

		FileInformationBlock( VirtualStreamReader reader );
	};
}
