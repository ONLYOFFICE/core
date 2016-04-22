
#include "OOXMathReader.h"
#include "OOXParagraphReader.h"

#include "../../../Common/DocxFormat/Source/DocxFormat/Math/OMath.h"
#include "../../../Common/DocxFormat/Source/DocxFormat/Math/oMathContent.h"
#include "../../../Common/DocxFormat/Source/DocxFormat/Math/oMathBottomNodes.h"

bool OOXMathReader::ParseElement(ReaderParameter oParam , OOX::WritingElement * ooxMath, RtfMathPtr & rtfMath)
{
	if (!ooxMath) return false;

	rtfMath = RtfMathPtr( new RtfMath() );
	if (!rtfMath) return false;

	OOX::EElementType ooxTypeElement = ooxMath->getType();

	rtfMath->SetOOXType(ooxTypeElement);

	switch(ooxTypeElement)
	{
		case OOX::et_w_rPr:
		{
			OOX::Logic::CRunProperty *ooxRPr = dynamic_cast<OOX::Logic::CRunProperty *>(ooxMath);
			if (ooxRPr)
			{
				RtfCharProperty oCurrentProp;
				oCurrentProp.SetDefaultOOX();
				
				OOXrPrReader orPrReader(ooxRPr);
				orPrReader.Parse( oParam, oCurrentProp );

				RtfCharPtr oChar = RtfCharPtr(new RtfChar);
				
				oChar->m_oProperty = oCurrentProp;
				rtfMath->m_oVal.AddItem( oChar );
			}
		}break;
		case OOX::et_m_r:
		{
			RtfCharProperty oCurrentProp;
			oCurrentProp.SetDefaultOOX();
			
			OOX::Logic::CMRun *ooxRunMath = dynamic_cast<OOX::Logic::CMRun *>(ooxMath);

			OOX::Logic::CRunProperty *ooxRPr = dynamic_cast<OOX::Logic::CRunProperty *>(ooxRunMath->m_oRPr.GetPointer());
			OOXrPrReader orPrReader(ooxRPr);
			orPrReader.Parse( oParam, oCurrentProp );

			oCurrentProp = RtfCharProperty();
			oCurrentProp.SetDefaultOOX();
			orPrReader.Parse( oParam, oCurrentProp );
		//-----------------------------------------------------------------------------------

			OOX::Logic::CMText *ooxTextMath = dynamic_cast<OOX::Logic::CMText *>(ooxRunMath->m_oMText.GetPointer());
			
			if (ooxTextMath)
			{
				RtfCharPtr oChar = RtfCharPtr(new RtfChar);
				
				oChar->m_oProperty = oCurrentProp;
				oChar->setText( ooxTextMath->m_sText );
				rtfMath->m_oVal.AddItem( oChar );
			}
		}break;
		//case OOX::et_m_t:
		//{
		//	OOX::Logic::CMText *ooxTextMath = dynamic_cast<OOX::Logic::CMText *>(m_ooxElem->m_arrItems[i]);
		//	if (ooxTextMath)
		//	{
		//		RtfCharPtr oChar = RtfCharPtr(new RtfChar);
		//		
		//		oChar->m_oProperty = oCurrentProp;
		//		oChar->setText( ooxTextMath->m_sText );
		//		rtfMath->m_oVal.AddItem( oChar );
		//	}
		//}break;
		case OOX::et_m_oMathParaPr:
		{
			OOX::Logic::COMathParaPr *ooxSubMath = dynamic_cast<OOX::Logic::COMathParaPr *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oMJc.GetPointer(), oSubMath))
				{
					oSubMath->SetOOXType(OOX::et_m_jc);
					rtfMath->AddItem(oSubMath);
				}
			}
		}break;
		case OOX::et_m_acc:
		{
			OOX::Logic::CAcc *ooxSubMath = dynamic_cast<OOX::Logic::CAcc *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oAccPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oElement.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);			
			}
		}break;
		case OOX::et_m_accPr:
		{
			OOX::Logic::CAccPr *ooxSubMath = dynamic_cast<OOX::Logic::CAccPr *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oChr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oCtrlPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);			
			}
		}break;
		case OOX::et_m_argPr:
		{
			OOX::Logic::CArgPr *ooxSubMath = dynamic_cast<OOX::Logic::CArgPr *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oArgSz.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
			}
		}break;
		case OOX::et_m_barPr:
		{
			OOX::Logic::CBarPr *ooxSubMath = dynamic_cast<OOX::Logic::CBarPr *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oCtrlPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oPos.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
			}
		}break;
		case OOX::et_m_bar:
		{
			OOX::Logic::CBar *ooxSubMath = dynamic_cast<OOX::Logic::CBar *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oBarPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oElement.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
			}
		}break;		
		case OOX::et_m_dPr:
		{
			OOX::Logic::CDelimiterPr *ooxSubMath = dynamic_cast<OOX::Logic::CDelimiterPr *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oBegChr.GetPointer(), oSubMath))
				{
					oSubMath->SetOOXType(OOX::et_m_begChr);
					rtfMath->AddItem(oSubMath);
				}
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oEndChr.GetPointer(), oSubMath))
				{
					oSubMath->SetOOXType(OOX::et_m_endChr);
					rtfMath->AddItem(oSubMath);
				}
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oCtrlPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oSepChr.GetPointer(), oSubMath))
				{
					oSubMath->SetOOXType(OOX::et_m_sepChr);
					rtfMath->AddItem(oSubMath);
				}
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oShp.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oGrow.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
			}
		}break;
		case OOX::et_m_eqArrPr:
		{
			OOX::Logic::CEqArrPr *ooxSubMath = dynamic_cast<OOX::Logic::CEqArrPr *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oCtrlPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oMaxDist.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oObjDist.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				///.....///
			}
		}break;
		case OOX::et_m_fPr:
		{
			OOX::Logic::CFPr *ooxSubMath = dynamic_cast<OOX::Logic::CFPr *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oCtrlPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oType.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
			}
		}break;
		case OOX::et_m_f:
		{
			OOX::Logic::CFraction *ooxSubMath = dynamic_cast<OOX::Logic::CFraction *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oFPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);		

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oNum.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);		

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oDen.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
			}
		}break;
		case OOX::et_m_funcPr:
		{
			OOX::Logic::CFuncPr *ooxSubMath = dynamic_cast<OOX::Logic::CFuncPr *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oCtrlPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);				
			}
		}break;
		case OOX::et_m_func:
		{
			OOX::Logic::CFunc *ooxSubMath = dynamic_cast<OOX::Logic::CFunc *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oFuncPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);		

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oFName.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oElement.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
			}
		}break;
		case OOX::et_m_groupChrPr:
		{
			OOX::Logic::CGroupChrPr *ooxSubMath = dynamic_cast<OOX::Logic::CGroupChrPr *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oCtrlPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);		

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oPos.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oVertJc.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oChr.GetPointer(), oSubMath))
				{
					oSubMath->SetOOXType(OOX::et_m_chr);
					rtfMath->AddItem(oSubMath);
				}
			}
		}break;
		case OOX::et_m_groupChr:
		{
			OOX::Logic::CGroupChr *ooxSubMath = dynamic_cast<OOX::Logic::CGroupChr *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oGroupChrPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);		

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oElement.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
			}
		}break;		
		case OOX::et_m_limLowPr:
		{
			OOX::Logic::CLimLowPr *ooxSubMath = dynamic_cast<OOX::Logic::CLimLowPr *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oCtrlPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);		
			}
		}break;
		case OOX::et_m_limLow:
		{
			OOX::Logic::CLimLow *ooxSubMath = dynamic_cast<OOX::Logic::CLimLow *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oLimLowPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);		

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oElement.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oLim.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
			}
		}break;
		case OOX::et_m_limUppPr:
		{
			OOX::Logic::CLimUppPr *ooxSubMath = dynamic_cast<OOX::Logic::CLimUppPr *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oCtrlPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);		
			}
		}break;
		case OOX::et_m_limUpp:
		{
			OOX::Logic::CLimUpp *ooxSubMath = dynamic_cast<OOX::Logic::CLimUpp *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oLimUppPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);		

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oElement.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oLim.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
			}
		}break;		
		case OOX::et_m_mathFont:
		{
			OOX::Logic::CMathFont *ooxSubMath = dynamic_cast<OOX::Logic::CMathFont *>(ooxMath);
			if (ooxSubMath)
			{
				RtfFont oFont;
				if (( ooxSubMath && ooxSubMath->m_val.IsInit()) && (true == oParam.oRtf->m_oFontTable.GetFont(ooxSubMath->m_val.get2(), oFont)))
				{
					rtfMath->m_bIsVal = true;
					RtfCharPtr oChar = RtfCharPtr(new RtfChar);
					CString s; s.AppendFormat( _T("%d"), oFont.m_nID );
					oChar->setText( s );
					rtfMath->m_oVal.AddItem( oChar );
				}
			}
		}break;
		case OOX::et_m_mc:
		{
			OOX::Logic::CMc *ooxSubMath = dynamic_cast<OOX::Logic::CMc *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oMcPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
			}
		}break;
		case OOX::et_m_mcPr:
		{
			OOX::Logic::CMcPr *ooxSubMath = dynamic_cast<OOX::Logic::CMcPr *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oCount.GetPointer(), oSubMath))
				{
					oSubMath->SetOOXType(OOX::et_m_count);
					rtfMath->AddItem(oSubMath);
				}
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oMcJc.GetPointer(), oSubMath))
				{
					oSubMath->SetOOXType(OOX::et_m_mcJc);
					rtfMath->AddItem(oSubMath);
				}
			}
		}break;
		case OOX::et_m_mPr:
		{
			OOX::Logic::CMPr *ooxSubMath = dynamic_cast<OOX::Logic::CMPr *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oBaseJc.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oCGp.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oCGpRule.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oCSp.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oCtrlPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oMcs.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oRSp.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oRSpRule.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oPlcHide.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
			}
		}break;
		case OOX::et_m_nary:
		{
			OOX::Logic::CNary *ooxSubMath = dynamic_cast<OOX::Logic::CNary *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oNaryPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);				
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oElement.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oSub.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oSup.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
			}
		}break;
		case OOX::et_m_naryPr:
		{
			OOX::Logic::CNaryPr *ooxSubMath = dynamic_cast<OOX::Logic::CNaryPr *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oChr.GetPointer(), oSubMath))
				{
					oSubMath->SetOOXType(OOX::et_m_chr);
					rtfMath->AddItem(oSubMath);
				}

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oCtrlPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oGrow.GetPointer(), oSubMath))
				{
					oSubMath->SetOOXType(OOX::et_m_grow);
					rtfMath->AddItem(oSubMath);
				}
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oLimLoc.GetPointer(), oSubMath))
				{
					oSubMath->SetOOXType(OOX::et_m_limLoc);
					rtfMath->AddItem(oSubMath);
				}
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oSubHide.GetPointer(), oSubMath))
				{
					oSubMath->SetOOXType(OOX::et_m_subHide);
					rtfMath->AddItem(oSubMath);
				}				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oSupHide.GetPointer(), oSubMath))
				{
					oSubMath->SetOOXType(OOX::et_m_supHide);
					rtfMath->AddItem(oSubMath);
				}
			}
		}break;
		case OOX::et_m_phantPr:
		{
			OOX::Logic::CPhantPr *ooxSubMath = dynamic_cast<OOX::Logic::CPhantPr *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oCtrlPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oShow.GetPointer(), oSubMath))
				{
					oSubMath->SetOOXType(OOX::et_m_show);
					rtfMath->AddItem(oSubMath);
				}

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oTransp.GetPointer(), oSubMath))
				{
					oSubMath->SetOOXType(OOX::et_m_transp);
					rtfMath->AddItem(oSubMath);
				}

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oZeroAsc.GetPointer(), oSubMath))
				{
					oSubMath->SetOOXType(OOX::et_m_zeroAsc);
					rtfMath->AddItem(oSubMath);
				}

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oZeroDesc.GetPointer(), oSubMath))
				{
					oSubMath->SetOOXType(OOX::et_m_zeroDesc);
					rtfMath->AddItem(oSubMath);
				}

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oZeroWid.GetPointer(), oSubMath))
				{
					oSubMath->SetOOXType(OOX::et_m_zeroWid);
					rtfMath->AddItem(oSubMath);
				}
			}
		}break;
		case OOX::et_m_phant:
		{
			OOX::Logic::CPhant *ooxSubMath = dynamic_cast<OOX::Logic::CPhant *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oPhantPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oElement.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
			}
		}break;
		case OOX::et_m_radPr:
		{
			OOX::Logic::CRadPr *ooxSubMath = dynamic_cast<OOX::Logic::CRadPr *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oCtrlPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oDegHide.GetPointer(), oSubMath))
				{
					oSubMath->SetOOXType(OOX::et_m_degHide);
					rtfMath->AddItem(oSubMath);
				}
			}
		}break;
		case OOX::et_m_rad:
		{
			OOX::Logic::CRad *ooxSubMath = dynamic_cast<OOX::Logic::CRad *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oRadPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oDeg.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oElement.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
			}
		}break;
		case OOX::et_m_rPr:
		{
			OOX::Logic::CMRPr *ooxSubMath = dynamic_cast<OOX::Logic::CMRPr *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oAln.GetPointer(), oSubMath))
				{
					oSubMath->SetOOXType(OOX::et_m_aln);
					rtfMath->AddItem(oSubMath);
				}

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oBrk.GetPointer(), oSubMath))
				{
					oSubMath->SetOOXType(OOX::et_m_brk);
					rtfMath->AddItem(oSubMath);
				}

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oLit.GetPointer(), oSubMath))
				{
					oSubMath->SetOOXType(OOX::et_m_lit);
					rtfMath->AddItem(oSubMath);
				}

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oNor.GetPointer(), oSubMath))
				{
					oSubMath->SetOOXType(OOX::et_m_nor);
					rtfMath->AddItem(oSubMath);
				}

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oScr.GetPointer(), oSubMath))
				{
					oSubMath->SetOOXType(OOX::et_m_scr);
					rtfMath->AddItem(oSubMath);
				}

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oSty.GetPointer(), oSubMath))
				{
					oSubMath->SetOOXType(OOX::et_m_sty);
					rtfMath->AddItem(oSubMath);
				}
			}
		}break;
		case OOX::et_m_sPrePr:
		{
			OOX::Logic::CSPrePr *ooxSubMath = dynamic_cast<OOX::Logic::CSPrePr *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oCtrlPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
			}
		}break;
		case OOX::et_m_sPre:
		{
			OOX::Logic::CSPre *ooxSubMath = dynamic_cast<OOX::Logic::CSPre *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oSPrePr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oElement.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oSub.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oSup.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
			}
		}break;
		case OOX::et_m_sSubPr:
		{
			OOX::Logic::CSSubPr *ooxSubMath = dynamic_cast<OOX::Logic::CSSubPr *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oCtrlPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
			}
		}break;
		case OOX::et_m_sSub:
		{
			OOX::Logic::CSSub *ooxSubMath = dynamic_cast<OOX::Logic::CSSub *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oSSubPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oElement.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oSub.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
			}
		}break;
		case OOX::et_m_sSubSupPr:
		{
			OOX::Logic::CSSubSupPr *ooxSubMath = dynamic_cast<OOX::Logic::CSSubSupPr *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oCtrlPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oAlnScr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
			}
		}break;
		case OOX::et_m_sSubSup:
		{
			OOX::Logic::CSSubSup *ooxSubMath = dynamic_cast<OOX::Logic::CSSubSup *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oSSubSupPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oElement.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oSub.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oSup.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
			}
		}break;
		case OOX::et_m_sSupPr:
		{
			OOX::Logic::CSSupPr *ooxSubMath = dynamic_cast<OOX::Logic::CSSupPr *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oCtrlPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
			}
		}break;
		case OOX::et_m_sSup:
		{
			OOX::Logic::CSSup *ooxSubMath = dynamic_cast<OOX::Logic::CSSup *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oSSupPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oElement.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oSup.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
			}
		}break;
		case OOX::et_m_ctrlPr:
		{
			OOX::Logic::CCtrlPr *ooxSubMath = dynamic_cast<OOX::Logic::CCtrlPr *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oRPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
			}
		}break;
		default:
		{
			OOX::Logic::CMathBottomNodes<SimpleTypes::COnOff<>>* ooxElemBool =
							dynamic_cast<OOX::Logic::CMathBottomNodes<SimpleTypes::COnOff<>>*>(ooxMath);

			OOX::Logic::CMathBottomNodes<SimpleTypes::CMChar>* ooxElemChar =
							dynamic_cast<OOX::Logic::CMathBottomNodes<SimpleTypes::CMChar>*>(ooxMath);

			OOX::Logic::CMathBottomNodes<SimpleTypes::CTwipsMeasure>* ooxElemMeasure =
							dynamic_cast<OOX::Logic::CMathBottomNodes<SimpleTypes::CTwipsMeasure>*>(ooxMath);

			OOX::Logic::CMathBottomNodes<SimpleTypes::CInteger255<>>* ooxElemInt255 =
							dynamic_cast<OOX::Logic::CMathBottomNodes<SimpleTypes::CInteger255<>>*>(ooxMath);
			

			OOX::WritingElementWithChilds<OOX::WritingElement>* ooxElemArray = 
							dynamic_cast<OOX::WritingElementWithChilds<OOX::WritingElement>*>(ooxMath);
			
			if ((ooxElemBool) && (ooxElemBool->m_val.IsInit()))
			{
				rtfMath->m_bIsVal = true;
				RtfCharPtr oChar = RtfCharPtr(new RtfChar);
				oChar->setText( ooxElemBool->m_val->ToString2(SimpleTypes::onofftostringOn) );
				rtfMath->m_oVal.AddItem( oChar );
			}
			else if ((ooxElemChar) && (ooxElemChar->m_val.IsInit()))
			{
				rtfMath->m_bIsVal = true;
				RtfCharPtr oChar = RtfCharPtr(new RtfChar);
				oChar->setText( ooxElemChar->m_val->GetValue() );
				rtfMath->m_oVal.AddItem( oChar );
			}
			else if ((ooxElemMeasure) && (ooxElemMeasure->m_val.IsInit()))
			{
				rtfMath->m_bIsVal = true;
				RtfCharPtr oChar = RtfCharPtr(new RtfChar);
				oChar->setText( ooxElemMeasure->m_val->ToString() );
				rtfMath->m_oVal.AddItem( oChar );
			}
			else if (ooxElemInt255)
			{
				rtfMath->m_bIsVal = true;
				RtfCharPtr oChar = RtfCharPtr(new RtfChar);
				oChar->setText( ooxElemInt255->m_val->ToString() );
				rtfMath->m_oVal.AddItem( oChar );
			}
			else if (ooxElemArray)
			{
				OOXMathReader oMathReader(ooxElemArray);
				bool res = oMathReader.Parse( oParam, (*rtfMath) );
			}
			else
			{
				//todoooo
				return false;
			}
		}break;
	}
	return true;
}


