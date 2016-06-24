/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
		case OOX::et_m_t:
		{//??? не нужно ващето ...
			OOX::Logic::CMText *ooxTextMath = dynamic_cast<OOX::Logic::CMText *>(ooxMath);
			if (ooxTextMath)
			{
				RtfCharPtr oChar = RtfCharPtr(new RtfChar);
				
				oChar->setText( ooxTextMath->m_sText );
				rtfMath->m_oVal.AddItem( oChar );
			}
		}break;
		case OOX::et_m_oMathParaPr:
		{
			OOX::Logic::COMathParaPr *ooxSubMath = dynamic_cast<OOX::Logic::COMathParaPr *>(ooxMath);
			if (ooxSubMath)
			{
				RtfMathPtr oSubMath;
				if (ParseElement(oParam, ooxSubMath->m_oMJc.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
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
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oEndChr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oCtrlPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oSepChr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
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
					rtfMath->AddItem(oSubMath);
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
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oMcJc.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
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
					rtfMath->AddItem(oSubMath);

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oCtrlPr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oGrow.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oLimLoc.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oSubHide.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oSupHide.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
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
					rtfMath->AddItem(oSubMath);

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oTransp.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oZeroAsc.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oZeroDesc.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oZeroWid.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
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
					rtfMath->AddItem(oSubMath);
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
					rtfMath->AddItem(oSubMath);

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oBrk.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oLit.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oNor.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oScr.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);

				oSubMath.reset();
				if (ParseElement(oParam, ooxSubMath->m_oSty.GetPointer(), oSubMath))
					rtfMath->AddItem(oSubMath);
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
				OOX::Logic::CRunProperty *ooxRPr = dynamic_cast<OOX::Logic::CRunProperty *>(ooxSubMath->m_oRPr.GetPointer());
				if (ooxRPr)
				{
					RtfCharProperty oCurrentProp;
					oCurrentProp.SetDefaultOOX();
					
					OOXrPrReader orPrReader(ooxRPr);
					orPrReader.Parse( oParam, oCurrentProp );

					RtfCharPtr oChar = RtfCharPtr(new RtfChar);
					
					oChar->m_oProperty = oCurrentProp;
					rtfMath->AddItem( oChar );
				}
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
			
			OOX::Logic::CMathBottomNodes<SimpleTypes::CLimLoc<>>* ooxElemLim =
							dynamic_cast<OOX::Logic::CMathBottomNodes<SimpleTypes::CLimLoc<>>*>(ooxMath);

			OOX::Logic::CMathBottomNodes<SimpleTypes::CUnSignedInteger<>>* ooxElemUnSignInt =
							dynamic_cast<OOX::Logic::CMathBottomNodes<SimpleTypes::CUnSignedInteger<>>*>(ooxMath);

			OOX::Logic::CMathBottomNodes<SimpleTypes::CTopBot<>>* ooxElemTopBot =
							dynamic_cast<OOX::Logic::CMathBottomNodes<SimpleTypes::CTopBot<>>*>(ooxMath);

			OOX::Logic::CMathBottomNodes<SimpleTypes::CFType<>>* ooxElemFType =
							dynamic_cast<OOX::Logic::CMathBottomNodes<SimpleTypes::CFType<>>*>(ooxMath);

			OOX::Logic::CMathBottomNodes<SimpleTypes::CStyle<>>* ooxElemStyle =
							dynamic_cast<OOX::Logic::CMathBottomNodes<SimpleTypes::CStyle<>>*>(ooxMath);

			OOX::Logic::CMathBottomNodes<SimpleTypes::CShp<>>* ooxElemShp =
							dynamic_cast<OOX::Logic::CMathBottomNodes<SimpleTypes::CShp<>>*>(ooxMath);

			OOX::Logic::CMathBottomNodes<SimpleTypes::CScript<>>* ooxElemScript =
							dynamic_cast<OOX::Logic::CMathBottomNodes<SimpleTypes::CScript<>>*>(ooxMath);

			OOX::Logic::CMathBottomNodes<SimpleTypes::CSpacingRule<>>* ooxElemSpacingRule =
							dynamic_cast<OOX::Logic::CMathBottomNodes<SimpleTypes::CSpacingRule<>>*>(ooxMath);

			OOX::Logic::CMathBottomNodes<SimpleTypes::CXAlign<>>* ooxElemXAlign =
							dynamic_cast<OOX::Logic::CMathBottomNodes<SimpleTypes::CXAlign<>>*>(ooxMath);

			OOX::Logic::CMathBottomNodes<SimpleTypes::CMJc<>>* ooxElemMJc =
							dynamic_cast<OOX::Logic::CMathBottomNodes<SimpleTypes::CMJc<>>*>(ooxMath);

			OOX::Logic::CMathBottomNodes<SimpleTypes::CInteger2<>>* ooxElemInteger2 =
							dynamic_cast<OOX::Logic::CMathBottomNodes<SimpleTypes::CInteger2<>>*>(ooxMath);

			OOX::Logic::CMathBottomNodes<SimpleTypes::CYAlign<>>* ooxElemYAlign =
							dynamic_cast<OOX::Logic::CMathBottomNodes<SimpleTypes::CYAlign<>>*>(ooxMath);

			OOX::Logic::CMathBottomNodes<SimpleTypes::CBreakBin<>>* ooxElemBreakBin =
							dynamic_cast<OOX::Logic::CMathBottomNodes<SimpleTypes::CBreakBin<>>*>(ooxMath);

			OOX::Logic::CMathBottomNodes<SimpleTypes::CBreakBinSub<>>* ooxElemBreakBinSub =
							dynamic_cast<OOX::Logic::CMathBottomNodes<SimpleTypes::CBreakBinSub<>>*>(ooxMath);

			OOX::WritingElementWithChilds<OOX::WritingElement>* ooxElemArray = 
							dynamic_cast<OOX::WritingElementWithChilds<OOX::WritingElement>*>(ooxMath);
	//----------------------------------
			nullable<CString> sVal;
			if ((ooxElemBool) && (ooxElemBool->m_val.IsInit()))			sVal = ooxElemBool->m_val->ToString2(SimpleTypes::onofftostringOn);
			else if ((ooxElemChar) && (ooxElemChar->m_val.IsInit()))				sVal = ooxElemChar->m_val->GetValue();
			else if ((ooxElemMeasure) && (ooxElemMeasure->m_val.IsInit()))			sVal = ooxElemMeasure->m_val->ToString();
			else if ((ooxElemInt255) && (ooxElemInt255->m_val.IsInit()))			sVal = ooxElemInt255->m_val->ToString();
			else if ((ooxElemLim) && (ooxElemLim->m_val.IsInit()))					sVal = ooxElemLim->m_val->ToString();
			else if ((ooxElemUnSignInt) && (ooxElemUnSignInt->m_val.IsInit()))		sVal = ooxElemUnSignInt->m_val->ToString();
			else if ((ooxElemTopBot) && (ooxElemTopBot->m_val.IsInit()))			sVal = ooxElemTopBot->m_val->ToString();
			else if ((ooxElemFType) && (ooxElemFType->m_val.IsInit())) 				sVal = ooxElemFType->m_val->ToString();
			else if ((ooxElemStyle) && (ooxElemStyle->m_val.IsInit()))				sVal = ooxElemStyle->m_val->ToString();
			else if ((ooxElemShp) && (ooxElemShp->m_val.IsInit()))					sVal = ooxElemShp->m_val->ToString();
			else if ((ooxElemScript) && (ooxElemScript->m_val.IsInit()))			sVal = ooxElemScript->m_val->ToString();
			else if ((ooxElemSpacingRule) && (ooxElemSpacingRule->m_val.IsInit()))	sVal = ooxElemSpacingRule->m_val->ToString();
			else if ((ooxElemXAlign) && (ooxElemXAlign->m_val.IsInit()))			sVal = ooxElemXAlign->m_val->ToString();
			else if ((ooxElemMJc) && (ooxElemMJc->m_val.IsInit()))					sVal = ooxElemMJc->m_val->ToString();
			else if ((ooxElemInteger2) && (ooxElemInteger2->m_val.IsInit()))		sVal = ooxElemInteger2->m_val->ToString();
			else if ((ooxElemYAlign) && (ooxElemYAlign->m_val.IsInit()))			sVal = ooxElemYAlign->m_val->ToString();
			else if ((ooxElemBreakBin) && (ooxElemBreakBin->m_val.IsInit()))		sVal = ooxElemBreakBin->m_val->ToString();
			else if ((ooxElemBreakBinSub) && (ooxElemBreakBinSub->m_val.IsInit()))	sVal = ooxElemBreakBinSub->m_val->ToString();
	//----------------------------------------------		
			
			if (ooxElemArray)
			{
				OOXMathReader oMathReader(ooxElemArray);
				bool res = oMathReader.Parse( oParam, (*rtfMath) );
			}
			else if (sVal.IsInit())
			{
				rtfMath->m_bIsVal = true;
				RtfCharPtr oChar = RtfCharPtr(new RtfChar);
				oChar->setText( *sVal );
				rtfMath->m_oVal.AddItem( oChar );
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


