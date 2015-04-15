#pragma once
#include "../RtfDocument.h"
#include "../RtfMath.h"

class OOXMathReader
{
private:
	OOX::WritingElementWithChilds<OOX::WritingElement>	*m_ooxElem;

public: 
	OOXMathReader(OOX::WritingElementWithChilds<OOX::WritingElement>* ooxElem)
	{
		m_ooxElem = ooxElem;
	}
	bool Parse( ReaderParameter oParam , RtfMath& oOutput)
	{
		RtfCharProperty oCurrentProp;
		oCurrentProp.SetDefaultOOX();
		
		if (m_ooxElem == NULL) return false;

		for(int i = 0; i < m_ooxElem->m_arrItems.size(); i++)
		{
			RtfMathPtr oNewMath( new RtfMath() );
			switch(m_ooxElem->m_arrItems[i]->getType())
			{
			case OOX::et_w_rPr:
				{
					OOX::Logic::CRunProperty *ooxRPr = dynamic_cast<OOX::Logic::CRunProperty *>(m_ooxElem->m_arrItems[i]);
					OOXrPrReader orPrReader(ooxRPr);
					orPrReader.Parse( oParam, oOutput.oProperty.m_oCharProp );
///
					oCurrentProp = RtfCharProperty();
					oCurrentProp.SetDefaultOOX();
					orPrReader.Parse( oParam, oCurrentProp );
				}break;
			case OOX::et_m_alnScr:
				{
					OOX::Logic::CAlnScr *ooxAlnScr = dynamic_cast<OOX::Logic::CAlnScr *>(m_ooxElem->m_arrItems[i]);
					if( ooxAlnScr->m_val.IsInit())
						oNewMath->oProperty.malnScr = ooxAlnScr->m_val->ToBool();
				}break;
			case OOX::et_m_oMathParaPr:
				{
					OOX::Logic::COMathParaPr *ooxMathParaPr = dynamic_cast<OOX::Logic::COMathParaPr *>(m_ooxElem->m_arrItems[i]);
					if( ooxMathParaPr->m_oMJc.IsInit() && ooxMathParaPr->m_oMJc->m_val.IsInit())
					{
						oNewMath->oProperty.moMathParaPr = 1;
						switch(ooxMathParaPr->m_oMJc->m_val->GetValue())
						{
							case SimpleTypes::mjcCenter			: oNewMath->oProperty.moMathParaPr = 1; break;
							case SimpleTypes::mjcCenterGroup	: oNewMath->oProperty.moMathParaPr = 2; break;
							case SimpleTypes::mjcLeft			: oNewMath->oProperty.moMathParaPr = 3; break;
							case SimpleTypes::mjcRight			: oNewMath->oProperty.moMathParaPr = 4; break;
						}
					}
				}break;
			case OOX::et_m_brk:
				{
					OOX::Logic::CBrk *ooxMath = dynamic_cast<OOX::Logic::CBrk *>(m_ooxElem->m_arrItems[i]);
					if( ooxMath->m_alnAt.IsInit() )
						oNewMath->oProperty.Break = ooxMath->m_alnAt->GetValue();
				}break;
			case OOX::et_m_chr:
				{
					OOX::Logic::CChr *ooxMath = dynamic_cast<OOX::Logic::CChr *>(m_ooxElem->m_arrItems[i]);
					if( ooxMath->m_val.IsInit())
					{
						CString sValue = ooxMath->m_val->GetValue();
						int nValue = PROP_DEF;
						if( -1 != sValue.Find( _T("&#") ) )
						{
							sValue.Replace( _T("&#"), _T("") );
							sValue.Replace( _T(";"), _T("") );
							nValue = Strings::ToInteger( sValue );
						}
						else
						{
							if( sValue.GetLength() > 0 )
								nValue = sValue[0];
						}
						if( PROP_DEF != nValue )
							oNewMath->oProperty.mchr = nValue;
					}
				}break;
			case OOX::et_m_lit:
				{
					OOX::Logic::CLit *ooxMath = dynamic_cast<OOX::Logic::CLit *>(m_ooxElem->m_arrItems[i]);
					if( ooxMath->m_val.IsInit() )
						oNewMath->oProperty.mlit = ooxMath->m_val->ToBool();
				}break;
			case OOX::et_m_nor:
				{
					OOX::Logic::CNor *ooxMath = dynamic_cast<OOX::Logic::CNor *>(m_ooxElem->m_arrItems[i]);
					if( ooxMath->m_val.IsInit() )
						oNewMath->oProperty.NormalText = ooxMath->m_val->ToBool();
				}break;
			case OOX::et_m_scr:
				{
					OOX::Logic::CScr *ooxMath = dynamic_cast<OOX::Logic::CScr *>(m_ooxElem->m_arrItems[i]);
					if (ooxMath->m_val.IsInit())
					{
						switch(ooxMath->m_val->GetValue())
						{
							case SimpleTypes::scriptDoubleStruck: oNewMath->oProperty.mscr = 3; break;
							case SimpleTypes::scriptFraktur		: oNewMath->oProperty.mscr = 2; break;
							case SimpleTypes::scriptMonospace	: oNewMath->oProperty.mscr = 5; break;
							case SimpleTypes::scriptRoman		: oNewMath->oProperty.mscr = 0;	break;
							case SimpleTypes::scriptSansSerif	: oNewMath->oProperty.mscr = 4; break;
							case SimpleTypes::scriptScript		: oNewMath->oProperty.mscr = 1; break;
						}
					}
				}break;
			case OOX::et_m_sty:
				{
					OOX::Logic::CSty *ooxMath = dynamic_cast<OOX::Logic::CSty *>(m_ooxElem->m_arrItems[i]);
					if (ooxMath->m_val.IsInit())
					{
						switch(ooxMath->m_val->GetValue())
						{
							case SimpleTypes::styleBold			: oNewMath->oProperty.msty = 1; break;
							case SimpleTypes::styleBoldItalic	: oNewMath->oProperty.msty = 3; break;
							case SimpleTypes::styleItalic		: oNewMath->oProperty.msty = 2; break;
							case SimpleTypes::stylePlain		: oNewMath->oProperty.msty = 0; break;
						}
					}
				}break;
			case OOX::et_m_jc:
				{
					OOX::Logic::CMJc *ooxMath = dynamic_cast<OOX::Logic::CMJc *>(m_ooxElem->m_arrItems[i]);
					if (ooxMath->m_val.IsInit())
					{
						switch(ooxMath->m_val->GetValue())
						{
							case SimpleTypes::mjcCenter			: oNewMath->oProperty.moMathParaPr = 1; break;
							case SimpleTypes::mjcCenterGroup	: oNewMath->oProperty.moMathParaPr = 2; break;
							case SimpleTypes::mjcLeft			: oNewMath->oProperty.moMathParaPr = 3; break;
							case SimpleTypes::mjcRight			: oNewMath->oProperty.moMathParaPr = 4; break;
						}
					}
				}break;
			case OOX::et_m_rSp:
				{
					OOX::Logic::CRSp *ooxMath = dynamic_cast<OOX::Logic::CRSp *>(m_ooxElem->m_arrItems[i]);
					if( ooxMath->m_val.IsInit() )
						oNewMath->oProperty.RowSpacing = ooxMath->m_val->GetValue();
				}break;
			case OOX::et_m_t:
				{
					OOX::Logic::CText *ooxMath = dynamic_cast<OOX::Logic::CText *>(m_ooxElem->m_arrItems[i]);

					RtfCharPtr oChar = RtfCharPtr(new RtfChar);
					
					oChar->m_oProperty = oCurrentProp;//todo
					oChar->setText( ooxMath->m_sText );
					oOutput.m_oVal.AddItem( oChar );
				}break;
			case OOX::et_m_rSpRule:
				{
					OOX::Logic::CRSpRule *ooxMath = dynamic_cast<OOX::Logic::CRSpRule *>(m_ooxElem->m_arrItems[i]);
					if( ooxMath->m_val.IsInit() )
						oNewMath->oProperty.RowSpacingRule = ooxMath->m_val->GetValue();
				}break;
			case OOX::et_m_cGp:
				{
					OOX::Logic::CCGp *ooxMath = dynamic_cast<OOX::Logic::CCGp *>(m_ooxElem->m_arrItems[i]);
					if( ooxMath->m_val.IsInit())
						oNewMath->oProperty.CellGap = ooxMath->m_val->GetValue();
				}break;
			case OOX::et_m_cGpRule:
				{
					OOX::Logic::CCGpRule *ooxMath = dynamic_cast<OOX::Logic::CCGpRule *>(m_ooxElem->m_arrItems[i]);
					if( ooxMath->m_val.IsInit() )
						oNewMath->oProperty.CellGapRule = ooxMath->m_val->GetValue();
				}break;
			case OOX::et_m_cSp:
				{
					OOX::Logic::CCSp *ooxMath = dynamic_cast<OOX::Logic::CCSp *>(m_ooxElem->m_arrItems[i]);
					if( ooxMath->m_val.IsInit() )
						oNewMath->oProperty.CellSpacing = ooxMath->m_val->GetValue();
				}break;
			case OOX::et_m_hideLeft:
				{
					OOX::Logic::CHideLeft *ooxMath = dynamic_cast<OOX::Logic::CHideLeft *>(m_ooxElem->m_arrItems[i]);
					if( ooxMath->m_val.IsInit() )
						oNewMath->oProperty.HideLeft = ooxMath->m_val->ToBool();
				}break;
			case OOX::et_m_hideTop:
				{
					OOX::Logic::CHideTop *ooxMath = dynamic_cast<OOX::Logic::CHideTop *>(m_ooxElem->m_arrItems[i]);
					if( ooxMath->m_val.IsInit() )
						oNewMath->oProperty.HideTop = ooxMath->m_val->ToBool();
				}break;
			case OOX::et_m_hideRight:
				{
					OOX::Logic::CHideRight *ooxMath = dynamic_cast<OOX::Logic::CHideRight *>(m_ooxElem->m_arrItems[i]);
					if( ooxMath->m_val.IsInit() )
						oNewMath->oProperty.HideRight = ooxMath->m_val->ToBool();
				}break;
			case OOX::et_m_hideBot:
				{
					OOX::Logic::CHideBot *ooxMath = dynamic_cast<OOX::Logic::CHideBot *>(m_ooxElem->m_arrItems[i]);
					if( ooxMath->m_val.IsInit() )
						oNewMath->oProperty.HideBottom = ooxMath->m_val->ToBool();
				}break;
			case OOX::et_m_strikeH:
				{
					OOX::Logic::CStrikeH *ooxMath = dynamic_cast<OOX::Logic::CStrikeH *>(m_ooxElem->m_arrItems[i]);
					if( ooxMath->m_val.IsInit() )
						oNewMath->oProperty.StrikeHor = ooxMath->m_val->ToBool();
				}break;
			case OOX::et_m_strikeV:
				{
					OOX::Logic::CStrikeV *ooxMath = dynamic_cast<OOX::Logic::CStrikeV *>(m_ooxElem->m_arrItems[i]);
					if( ooxMath->m_val.IsInit() )
						oNewMath->oProperty.StrikeVer = ooxMath->m_val->ToBool();
				}break;
			case OOX::et_m_strikeTLBR:
				{
					OOX::Logic::CStrikeTLBR *ooxMath = dynamic_cast<OOX::Logic::CStrikeTLBR *>(m_ooxElem->m_arrItems[i]);
					if( ooxMath->m_val.IsInit() )
						oNewMath->oProperty.StrikeLR = ooxMath->m_val->ToBool();
				}break;
			case OOX::et_m_strikeBLTR:
				{
					OOX::Logic::CStrikeBLTR *ooxMath = dynamic_cast<OOX::Logic::CStrikeBLTR  *>(m_ooxElem->m_arrItems[i]);
					if( ooxMath->m_val.IsInit() )
						oNewMath->oProperty.StrikeRL = ooxMath->m_val->ToBool();
				}break;
			case OOX::et_m_aln:
			{
				OOX::Logic::CAln *ooxMath = dynamic_cast<OOX::Logic::CAln *>(m_ooxElem->m_arrItems[i]);
				if( ooxMath->m_val.IsInit() )
					oNewMath->oProperty.Alignment = ooxMath->m_val->ToBool();
			}break;
			case OOX::et_m_diff:
			{
				OOX::Logic::CDiff *ooxMath = dynamic_cast<OOX::Logic::CDiff *>(m_ooxElem->m_arrItems[i]);
				if( ooxMath->m_val.IsInit() )
					oNewMath->oProperty.Differential = ooxMath->m_val->ToBool();
			}break;
			case OOX::et_m_noBreak:
			{
				OOX::Logic::CNoBreak *ooxMath = dynamic_cast<OOX::Logic::CNoBreak *>(m_ooxElem->m_arrItems[i]);
				if( ooxMath->m_val.IsInit() )
					oNewMath->oProperty.NoBreak = ooxMath->m_val->ToBool();
			}break;
			case OOX::et_m_opEmu:
			{
				OOX::Logic::COpEmu *ooxMath = dynamic_cast<OOX::Logic::COpEmu *>(m_ooxElem->m_arrItems[i]);
				if( ooxMath->m_val.IsInit() )
					oNewMath->oProperty.Emulator = ooxMath->m_val->ToBool();
			}break;
			case OOX::et_m_show:
			{
				OOX::Logic::CShow *ooxMath = dynamic_cast<OOX::Logic::CShow *>(m_ooxElem->m_arrItems[i]);
				if( ooxMath->m_val.IsInit() )
					oNewMath->oProperty.mshow = ooxMath->m_val->ToBool();
			}break;
			case OOX::et_m_transp:
			{
				OOX::Logic::CTransp *ooxMath = dynamic_cast<OOX::Logic::CTransp *>(m_ooxElem->m_arrItems[i]);
				if( ooxMath->m_val.IsInit() )
					oNewMath->oProperty.mtransp = ooxMath->m_val->ToBool();
			}
			case OOX::et_m_zeroAsc:
			{
				OOX::Logic::CZeroAsc *ooxMath = dynamic_cast<OOX::Logic::CZeroAsc *>(m_ooxElem->m_arrItems[i]);
				if( ooxMath->m_val.IsInit() )
					oNewMath->oProperty.mzeroAsc = ooxMath->m_val->ToBool();
			}break;
			case OOX::et_m_zeroDesc:
			{
				OOX::Logic::CZeroDesc *ooxMath = dynamic_cast<OOX::Logic::CZeroDesc *>(m_ooxElem->m_arrItems[i]);
				if( ooxMath->m_val.IsInit() )
					oNewMath->oProperty.mzeroDesc = ooxMath->m_val->ToBool();
			}
			case OOX::et_m_zeroWid:
			{
				OOX::Logic::CZeroWid *ooxMath = dynamic_cast<OOX::Logic::CZeroWid *>(m_ooxElem->m_arrItems[i]);
				if( ooxMath->m_val.IsInit() )
					oNewMath->oProperty.mzeroWid = ooxMath->m_val->ToBool();
			}break;
			default:
				{
					OOX::WritingElementWithChilds<OOX::WritingElement>* ooxElem =
									dynamic_cast<OOX::WritingElementWithChilds<OOX::WritingElement>*>(m_ooxElem->m_arrItems[i]);
					
					if (ooxElem)
					{
						//oNewMath->SetOOXName( ooxElem-> );

						OOXMathReader oMathReader(ooxElem);
						if( true == oMathReader.Parse( oParam, (*oNewMath) ) )
							oOutput.AddItem( oNewMath );
					}
				}break;
			}
		}
		return true;
	}
};
