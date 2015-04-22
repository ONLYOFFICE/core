#pragma once
#include "../RtfDocument.h"
#include "../../../../Common/DocxFormat/Source/DocxFormat/Math/oMathContent.h"

class OOXMathPropReader
{
private: 
	OOX::Logic::CMathPr *m_ooxMathPr;
public: 
	OOXMathPropReader(OOX::Logic::CMathPr *ooxMathPr)
	{
		m_ooxMathPr = ooxMathPr;
	}
	bool Parse( ReaderParameter oParam )
	{
		if (m_ooxMathPr == NULL) return false;

		for (long i = 0; i < m_ooxMathPr->m_arrItems.size(); i++)
		{
			if (m_ooxMathPr->m_arrItems[i] == NULL) continue;

			switch(m_ooxMathPr->m_arrItems[i]->getType())
			{
				case OOX::et_m_mathFont:
				{
					OOX::Logic::CMathFont* pFont = dynamic_cast<OOX::Logic::CMathFont*>(m_ooxMathPr->m_arrItems[i]);
					RtfFont oFont;
					if (( pFont && pFont->m_val.IsInit()) && (true == oParam.oRtf->m_oFontTable.GetFont(pFont->m_val.get2(), oFont)))
						oParam.oRtf->m_oMathProp.mmathFont = oFont.m_nID;
				}break;
				case OOX::et_m_brkBin:
				{
					OOX::Logic::CBrkBin* pBrkBin = dynamic_cast<OOX::Logic::CBrkBin*>(m_ooxMathPr->m_arrItems[i]);
					if ((pBrkBin) && (pBrkBin->m_val.IsInit()))
					{
						switch(pBrkBin->m_val->GetValue())
						{
						case SimpleTypes::breakBinAfter		: oParam.oRtf->m_oMathProp.mbrkBin = 1; break;
						case SimpleTypes::breakBinBefore	: oParam.oRtf->m_oMathProp.mbrkBin = 0; break;
						case SimpleTypes::breakBinRepeat	: 
						default								: oParam.oRtf->m_oMathProp.mbrkBin = 2; break;
						}

					}
				}break;
				case OOX::et_m_brkBinSub:
				{
					OOX::Logic::CBrkBinSub* pBrkBinSub = dynamic_cast<OOX::Logic::CBrkBinSub*>(m_ooxMathPr->m_arrItems[i]);
					if ((pBrkBinSub) && (pBrkBinSub->m_val.IsInit()))
					{
						switch(pBrkBinSub->m_val->GetValue())
						{
						case SimpleTypes::breakBinPlusMinus  : oParam.oRtf->m_oMathProp.mbrkBinSub = 1; break;
						case SimpleTypes::breakBinMinusPlus  : oParam.oRtf->m_oMathProp.mbrkBinSub = 2; break;
						case SimpleTypes::breakBinMinusMinus : oParam.oRtf->m_oMathProp.mbrkBinSub = 0; break;
						}
					}
				}break;
				case OOX::et_m_defJc:
				{
					OOX::Logic::CDefJc* pDefJc = dynamic_cast<OOX::Logic::CDefJc*>(m_ooxMathPr->m_arrItems[i]);
					if ((pDefJc) && (pDefJc->m_val.IsInit()))
					{
						switch(pDefJc->m_val->GetValue())
						{
						case SimpleTypes::mjcCenter		: oParam.oRtf->m_oMathProp.mbrkBinSub = 2; break;
						case SimpleTypes::mjcLeft		: oParam.oRtf->m_oMathProp.mbrkBinSub = 3; break;
						case SimpleTypes::mjcRight		: oParam.oRtf->m_oMathProp.mbrkBinSub = 4; break;
						case SimpleTypes::mjcCenterGroup: 
						default							: oParam.oRtf->m_oMathProp.mbrkBinSub = 1; break;
						}
					}
				}break;
				case OOX::et_m_dispDef:
				{
					OOX::Logic::CDispDef* pDispDef = dynamic_cast<OOX::Logic::CDispDef*>(m_ooxMathPr->m_arrItems[i]);
					if ((pDispDef) && (pDispDef->m_val.IsInit()))
						oParam.oRtf->m_oMathProp.mdispDef = pDispDef->m_val->ToBool();
				}break;
				case OOX::et_m_interSp:
				{
					OOX::Logic::CInterSp* pInterSp = dynamic_cast<OOX::Logic::CInterSp*>(m_ooxMathPr->m_arrItems[i]);
					if ((pInterSp) && (pInterSp->m_val.IsInit()))
						oParam.oRtf->m_oMathProp.minterSp = pInterSp->m_val->ToTwips(); //todooo
				}break;
				case OOX::et_m_intraSp:
				{
					OOX::Logic::CIntraSp* pIntraSp = dynamic_cast<OOX::Logic::CIntraSp*>(m_ooxMathPr->m_arrItems[i]);
					if ((pIntraSp) && (pIntraSp->m_val.IsInit()))
						oParam.oRtf->m_oMathProp.mintraSp = pIntraSp->m_val->ToTwips(); //todooo
				}break;
				case OOX::et_m_lMargin:
				{
					OOX::Logic::CLMargin* pMargin = dynamic_cast<OOX::Logic::CLMargin*>(m_ooxMathPr->m_arrItems[i]);
					if ((pMargin) && (pMargin->m_val.IsInit()))
						oParam.oRtf->m_oMathProp.mlMargin = pMargin->m_val->ToTwips(); //todooo
				}break;
				case OOX::et_m_rMargin:
				{
					OOX::Logic::CRMargin* pMargin = dynamic_cast<OOX::Logic::CRMargin*>(m_ooxMathPr->m_arrItems[i]);
					if ((pMargin) && (pMargin->m_val.IsInit()))
						oParam.oRtf->m_oMathProp.mrMargin = pMargin->m_val->ToTwips(); //todooo
				}break;
				case OOX::et_m_naryLim:
				{
					OOX::Logic::CNaryLim* pNaryLim = dynamic_cast<OOX::Logic::CNaryLim*>(m_ooxMathPr->m_arrItems[i]);
					if ( (pNaryLim) && (pNaryLim->m_val.IsInit()) )
					{
						if( pNaryLim->m_val->GetValue()		 ==  SimpleTypes::limLocSubSup)	oParam.oRtf->m_oMathProp.mnaryLim = 0;
						else if( pNaryLim->m_val->GetValue() ==  SimpleTypes::limLocUndOvr)	oParam.oRtf->m_oMathProp.mnaryLim = 1;
					}
				}break;
				case OOX::et_m_postSp:
				{
					OOX::Logic::CPostSp* pPostSp = dynamic_cast<OOX::Logic::CPostSp*>(m_ooxMathPr->m_arrItems[i]);
					if ((pPostSp) && (pPostSp->m_val.IsInit()))
						oParam.oRtf->m_oMathProp.mpostSp = pPostSp->m_val->ToTwips(); //todooo
				}break;
				case OOX::et_m_preSp:
				{
					OOX::Logic::CPreSp* pPreSp = dynamic_cast<OOX::Logic::CPreSp*>(m_ooxMathPr->m_arrItems[i]);
					if ((pPreSp) && (pPreSp->m_val.IsInit()))
						oParam.oRtf->m_oMathProp.mpreSp = pPreSp->m_val->ToTwips(); //todooo
				}break;
				case OOX::et_m_smallFrac:
				{
					OOX::Logic::CSmallFrac* pSmallFrac = dynamic_cast<OOX::Logic::CSmallFrac*>(m_ooxMathPr->m_arrItems[i]);
					if ((pSmallFrac) && (pSmallFrac->m_val.IsInit()))
						oParam.oRtf->m_oMathProp.msmallFrac = pSmallFrac->m_val->ToBool();
				}break;
				case OOX::et_m_wrapIndent:
				{
					OOX::Logic::CWrapIndent* pWrapIndent = dynamic_cast<OOX::Logic::CWrapIndent*>(m_ooxMathPr->m_arrItems[i]);
					if ((pWrapIndent) && (pWrapIndent->m_val.IsInit()))
						oParam.oRtf->m_oMathProp.mwrapIndent = pWrapIndent->m_val->ToTwips(); //todooo
				}break;
				case OOX::et_m_wrapRight:
				{
					OOX::Logic::CWrapRight* pWrapRight = dynamic_cast<OOX::Logic::CWrapRight*>(m_ooxMathPr->m_arrItems[i]);
					if ((pWrapRight) && (pWrapRight->m_val.IsInit()))
						oParam.oRtf->m_oMathProp.mwrapRight = pWrapRight->m_val->ToBool(); //todooo
				}break;
			}
		}
		return true;
	}
};
