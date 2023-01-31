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
#include "../WritingElement.h"
#include "../../Common/SimpleTypes_Shared.h"
#include "../../Common/SimpleTypes_OMath.h"

namespace OOX
{	
	namespace Logic
	{		
		//--------------------------------------------------------------------------------
		/*	
			CAln 22.1.2.1, CAlnScr 22.1.2.4,  CArgSz 22.1.2.6, CBaseJc 22.1.2.9, CBegChr 22.1.2.10, CBrkBin 22.1.2.16, CBrkBinSub 22.1.2.17,
			CCGp 22.1.2.18, CCGpRule 22.1.2.19, CChr 22.1.2.20, CCount 22.1.2.21, CCSp 22.1.2.22,
			CDefJc 22.1.2.25, CDegHide 22.1.2.27, CDiff 22.1.2.29, CDispDef 22.1.2.30, CEndChr 22.1.2.33,
			CGrow 22.1.2.43, CHideBot 22.1.2.44, CHideLeft 22.1.2.45, CHideRight 22.1.2.46, CHideTop 22.1.2.47, CInterSp  22.1.2.48,
			CIntLim 22.1.2.49, CIntraSp  22.1.2.48, CMJc 22.1.2.51, CLimLoc 22.1.2.53, CLit 22.1.2.58 , CLMargin 22.1.2.59,
			CMaxDist 22.1.2.63, CMcJc 22.1.2.65, CNaryLim 22.1.2.71, CNoBreak 22.1.2.73,
			CNor 22.1.2.74, CObjDist 22.1.2.76, COpEmu 22.1.2.80 , CPlcHide 22.1.2.83, CPos 22.1.2.84,
			CPostSp 22.1.2.85, CPreSp 22.1.2.86, CRMargin 22.1.2.90, CRSp 22.1.2.92 , CRSpRule 22.1.2.93,
			CScr 22.1.2.94, CSepChr 22.1.2.95, CShow 22.1.2.96, CShp 22.1.2.97, CSmallFrac 22.1.2.98,
			CStrikeBLTR 22.1.2.107 , CStrikeH 22.1.2.108, CStrikeTLBR 22.1.2.109, CStrikeV 22.1.2.110,
			CSty 22.1.2.111 ,CSubHide 22.1.2.113, CSupHide 22.1.2.115, CTransp 22.1.2.117,
			CType 22.1.2.118, CVertJc 22.1.2.119,CWrapIndent 22.1.2.120, CWrapRight 22.1.2.121,
			CZeroAsc 22.1.2.122, CZeroDesc 22.1.2.123, CZeroWid 22.1.2.124
		*/
		//--------------------------------------------------------------------------------
		template <typename TMathBottomType>
		class CMathBottomNodes : public WritingElement
		{		
		public:
			WritingElement_AdditionConstructors(CMathBottomNodes)
			CMathBottomNodes()
			{				
			}
			virtual ~CMathBottomNodes()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				m_sNodeName = oNode.GetName();
				if ( _T("m:aln") == m_sNodeName )
					m_eType = et_m_aln;
				else if ( _T("m:alnScr") == m_sNodeName )
					m_eType = et_m_alnScr;
				else if ( _T("m:argSz") == m_sNodeName )
					m_eType = et_m_argSz;
				else if ( _T("m:baseJc") == m_sNodeName )
					m_eType = et_m_baseJc;
				else if ( _T("m:begChr") == m_sNodeName )
					m_eType = et_m_begChr;
				else if ( _T("m:cGp") == m_sNodeName )
					m_eType = et_m_cGp;
				else if ( _T("m:cGpRule") == m_sNodeName )
					m_eType = et_m_cGpRule;
				else if ( _T("m:chr") == m_sNodeName )
					m_eType = et_m_chr;
				else if ( _T("m:count") == m_sNodeName )
					m_eType = et_m_count;
				else if ( _T("m:cSp") == m_sNodeName )
					m_eType = et_m_cSp;
				else if ( _T("m:degHide") == m_sNodeName )
					m_eType = et_m_degHide;
				else if ( _T("m:diff") == m_sNodeName )
					m_eType = et_m_diff;
				else if ( _T("m:endChr") == m_sNodeName )
					m_eType = et_m_endChr;
				else if ( _T("m:grow") == m_sNodeName )
					m_eType = et_m_grow;
				else if ( _T("m:hideBot") == m_sNodeName )
					m_eType = et_m_hideBot;
				else if ( _T("m:hideLeft") == m_sNodeName )
					m_eType = et_m_hideLeft;
				else if ( _T("m:hideRight") == m_sNodeName )
					m_eType = et_m_hideRight;
				else if ( _T("m:hideTop") == m_sNodeName )
					m_eType = et_m_hideTop;
				else if ( _T("m:jc") == m_sNodeName )
					m_eType = et_m_jc;
				else if ( _T("m:limLoc") == m_sNodeName )
					m_eType = et_m_limLoc;
				else if ( _T("m:lit") == m_sNodeName )
					m_eType = et_m_lit;
				else if ( _T("m:maxDist") == m_sNodeName )
					m_eType = et_m_maxDist;
				else if ( _T("m:mcJc") == m_sNodeName )
					m_eType = et_m_mcJc;
				else if ( _T("m:noBreak") == m_sNodeName )
					m_eType = et_m_noBreak;
				else if ( _T("m:nor") == m_sNodeName )
					m_eType = et_m_nor;
				else if ( _T("m:objDist") == m_sNodeName )
					m_eType = et_m_objDist;
				else if ( _T("m:opEmu") == m_sNodeName )
					m_eType = et_m_opEmu;
				else if ( _T("m:plcHide") == m_sNodeName )
					m_eType = et_m_plcHide;
				else if ( _T("m:pos") == m_sNodeName )
					m_eType = et_m_pos;
				else if ( _T("m:rSp") == m_sNodeName )
					m_eType = et_m_rSp;
				else if ( _T("m:rSpRule") == m_sNodeName )
					m_eType = et_m_rSpRule;
				else if ( _T("m:scr") == m_sNodeName )
					m_eType = et_m_scr;
				else if ( _T("m:sepChr") == m_sNodeName )
					m_eType = et_m_sepChr;
				else if ( _T("m:show") == m_sNodeName )
					m_eType = et_m_show;
				else if ( _T("m:shp") == m_sNodeName )
					m_eType = et_m_shp;
				else if ( _T("m:strikeBLTR") == m_sNodeName )
					m_eType = et_m_strikeBLTR;
				else if ( _T("m:strikeH") == m_sNodeName )
					m_eType = et_m_strikeH;
				else if ( _T("m:strikeTLBR") == m_sNodeName )
					m_eType = et_m_strikeTLBR;
				else if ( _T("m:strikeV") == m_sNodeName )
					m_eType = et_m_strikeV;
				else if ( _T("m:sty") == m_sNodeName )
					m_eType = et_m_sty;
				else if ( _T("m:subHide") == m_sNodeName )
					m_eType = et_m_subHide;
				else if ( _T("m:supHide") == m_sNodeName )
					m_eType = et_m_supHide;
				else if ( _T("m:transp") == m_sNodeName )
					m_eType = et_m_transp;
				else if ( _T("m:type") == m_sNodeName )
					m_eType = et_m_type;
				else if ( _T("m:vertJc") == m_sNodeName )
					m_eType = et_m_vertJc;
				else if ( _T("m:zeroAsc") == m_sNodeName )
					m_eType = et_m_zeroAsc;
				else if ( _T("m:zeroDesc") == m_sNodeName )
					m_eType = et_m_zeroDesc;
				else if ( _T("m:zeroWid") == m_sNodeName )
					m_eType = et_m_zeroWid;

				XmlMacroReadAttributeBase( oNode, _T("m:val"), m_val );
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				m_sNodeName = oReader.GetName();
				if ( _T("m:brkBin") == m_sNodeName )
					m_eType = et_m_brkBin;
				else if ( _T("m:brkBinSub") == m_sNodeName )
					m_eType = et_m_brkBinSub;
				else if ( _T("m:defJc") == m_sNodeName )
					m_eType = et_m_defJc;
				else if ( _T("m:dispDef") == m_sNodeName )
					m_eType = et_m_dispDef;
				else if ( _T("m:interSp") == m_sNodeName )
					m_eType = et_m_interSp;
				else if ( _T("m:intLim") == m_sNodeName )
					m_eType = et_m_intLim;
				else if ( _T("m:intraSp") == m_sNodeName )
					m_eType = et_m_intraSp;				
				else if ( _T("m:lMargin") == m_sNodeName )
					m_eType = et_m_lMargin;				
				else if ( _T("m:naryLim") == m_sNodeName )
					m_eType = et_m_naryLim;
				else if ( _T("m:postSp") == m_sNodeName )
					m_eType = et_m_postSp;
				else if ( _T("m:preSp") == m_sNodeName )
					m_eType = et_m_preSp;
				else if ( _T("m:rMargin") == m_sNodeName )
					m_eType = et_m_rMargin;
				else if ( _T("m:smallFrac") == m_sNodeName )
					m_eType = et_m_smallFrac;
				else if ( _T("m:wrapIndent") == m_sNodeName )
					m_eType = et_m_wrapIndent;
				else if ( _T("m:wrapRight") == m_sNodeName )
					m_eType = et_m_wrapRight;

				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring toXML() const
			{
				std::wstring sNodeName = m_sNodeName;
				if (sNodeName.empty())
					sNodeName = GetMathNodeName(m_eType);
				if (sNodeName.empty()) return L"";

				std::wstring sResult = _T("<") + sNodeName;
				if (m_val.IsInit())
				{
					sResult += _T(" m:val=\"");
					sResult +=  m_val->ToString();
					sResult +=  _T("\"");
				}
				sResult +=  _T("/>");
				return sResult;
			}

			virtual EElementType getType() const
			{
				return m_eType;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("m:val"), m_val )
				WritingElement_ReadAttributes_End( oReader )
			}
			std::wstring GetMathNodeName(const EElementType & enumType)  const
			{//todooo вытащить в одно место - пересекается с MathArgNodes
				switch(enumType)
				{
					case OOX::et_m_deg:				return L"m:deg";
					case OOX::et_m_aln:				return L"m:aln";
					case OOX::et_m_alnScr:			return L"m:alnScr";
					case OOX::et_m_argSz:			return L"m:argSz";
					case OOX::et_m_baseJc:			return L"m:baseJc";
					case OOX::et_m_begChr:			return L"m:begChr";
					case OOX::et_m_brkBin:			return L"m:brkBin";
					case OOX::et_m_brkBinSub:		return L"m:brkBinSub";
					case OOX::et_m_cGp:				return L"m:cGp";
					case OOX::et_m_cGpRule:			return L"m:cGpRule";
					case OOX::et_m_chr:				return L"m:chr";
					case OOX::et_m_count:			return L"m:count";
					case OOX::et_m_cSp:				return L"m:cSp";
					case OOX::et_m_defJc:			return L"m:defJc";
					case OOX::et_m_degHide:			return L"m:degHide";
					case OOX::et_m_diff:			return L"m:diff";
					case OOX::et_m_dispDef:			return L"m:dispDef";
					case OOX::et_m_endChr:			return L"m:endChr";
					case OOX::et_m_grow:			return L"m:grow";
					case OOX::et_m_hideBot:			return L"m:hideBot";
					case OOX::et_m_hideLeft:		return L"m:hideLeft";
					case OOX::et_m_hideRight:		return L"m:hideRight";
					case OOX::et_m_hideTop:			return L"m:hideTop";
					case OOX::et_m_interSp:			return L"m:interSp";
					case OOX::et_m_intLim:			return L"m:intLim";
					case OOX::et_m_intraSp:			return L"m:intraSp";
					case OOX::et_m_jc:				return L"m:jc";
					case OOX::et_m_limLoc:			return L"m:limLoc";
					case OOX::et_m_lit:				return L"m:lit";
					case OOX::et_m_lMargin:			return L"m:lMargin";
					case OOX::et_m_maxDist:			return L"m:maxDist";
					case OOX::et_m_mcJc:			return L"m:mcJc";
					case OOX::et_m_naryLim:			return L"m:naryLim";
					case OOX::et_m_noBreak:			return L"m:noBreak";
					case OOX::et_m_nor:				return L"m:nor";
					case OOX::et_m_objDist:			return L"m:objDist";
					case OOX::et_m_opEmu:			return L"m:opEmu";
					case OOX::et_m_plcHide:			return L"m:plcHide";
					case OOX::et_m_pos:				return L"m:pos";
					case OOX::et_m_postSp:			return L"m:postSp";
					case OOX::et_m_preSp:			return L"m:preSp";
					case OOX::et_m_rMargin:			return L"m:rMargin";
					case OOX::et_m_rSp:				return L"m:rSp";
					case OOX::et_m_rSpRule:			return L"m:rSpRule";
					case OOX::et_m_scr:				return L"m:scr";
					case OOX::et_m_sepChr:			return L"m:sepChr";
					case OOX::et_m_show:			return L"m:show";
					case OOX::et_m_shp:				return L"m:shp";
					case OOX::et_m_smallFrac:		return L"m:smallFrac";
					case OOX::et_m_strikeBLTR:		return L"m:strikeBLTR";
					case OOX::et_m_strikeH:			return L"m:strikeH";
					case OOX::et_m_strikeTLBR:		return L"m:strikeTLBR";
					case OOX::et_m_strikeV:			return L"m:strikeV";
					case OOX::et_m_sty:				return L"m:sty";
					case OOX::et_m_subHide:			return L"m:subHide";
					case OOX::et_m_supHide:			return L"m:supHide";
					case OOX::et_m_transp:			return L"m:transp";
					case OOX::et_m_type:			return L"m:type";
					case OOX::et_m_vertJc:			return L"m:vertJc";
					case OOX::et_m_wrapIndent:		return L"m:wrapIndent";
					case OOX::et_m_wrapRight:		return L"m:wrapRight";
					case OOX::et_m_zeroAsc:			return L"m:zeroAsc";
					case OOX::et_m_zeroDesc:		return L"m:zeroDesc";
					case OOX::et_m_zeroWid:			return L"m:zeroWid";
                    default:
                        break;
                }
				return L"";
			}
		public:
			EElementType m_eType;
			std::wstring m_sNodeName;
			
			nullable<TMathBottomType> m_val;			
		};

		template<class TMathBottomType, class NodeType>
		inline void CMathBottomNodesExFromXML(CMathBottomNodes<TMathBottomType>& v, NodeType& oNode)
		{
			v.fromXML( oNode );
		}
		template<class NodeType>
		inline void CMathBottomNodesExFromXML(CMathBottomNodes<SimpleTypes::COnOff>& v, NodeType& oNode)
		{
			v.m_val.Init();
			v.m_val->FromBool(true);
			v.fromXML( oNode );
		}

		template <typename TMathBottomType, EElementType type = OOX::et_Unknown>
		class CMathBottomNodesEx : public CMathBottomNodes<TMathBottomType>
		{
		public:
			CMathBottomNodesEx()
			{
				CMathBottomNodes<TMathBottomType>::m_eType = type;
			}
			CMathBottomNodesEx(XmlUtils::CXmlNode& oNode)
			{
				CMathBottomNodes<TMathBottomType>::m_eType = type;
				CMathBottomNodesExFromXML(*this, oNode);
			}
			CMathBottomNodesEx(XmlUtils::CXmlLiteReader& oReader)
			{
				CMathBottomNodes<TMathBottomType>::m_eType = type;
				CMathBottomNodesExFromXML(*this, oReader);
			}
			virtual EElementType getType() const
			{
				return CMathBottomNodes<TMathBottomType>::m_eType;
			}
		};

		typedef	CMathBottomNodesEx<SimpleTypes::COnOff,				OOX::et_m_aln>		CAln;
		typedef CMathBottomNodesEx<SimpleTypes::COnOff,				OOX::et_m_alnScr>	CAlnScr;
		typedef CMathBottomNodesEx<SimpleTypes::CInteger2,			OOX::et_m_argSz>	CArgSz;
		typedef CMathBottomNodesEx<SimpleTypes::CYAlign,			OOX::et_m_baseJc>	CBaseJc;
		typedef CMathBottomNodesEx<SimpleTypes::CMChar,				OOX::et_m_begChr>	CBegChr;
		typedef CMathBottomNodesEx<SimpleTypes::CBreakBin,			OOX::et_m_brkBin>	CBrkBin;
		typedef CMathBottomNodesEx<SimpleTypes::CBreakBinSub,		OOX::et_m_brkBinSub>CBrkBinSub;
		typedef CMathBottomNodesEx<SimpleTypes::CUnSignedInteger,	OOX::et_m_cGp>		CCGp;
		typedef CMathBottomNodesEx<SimpleTypes::CSpacingRule,		OOX::et_m_cGpRule>	CCGpRule;
		typedef CMathBottomNodesEx<SimpleTypes::CMChar,				OOX::et_m_chr>		CChr;
		typedef CMathBottomNodesEx<SimpleTypes::CInteger255,		OOX::et_m_count>	CCount;
		typedef CMathBottomNodesEx<SimpleTypes::CUnSignedInteger,	OOX::et_m_cSp>		CCSp;
		typedef CMathBottomNodesEx<SimpleTypes::CMJc,				OOX::et_m_defJc>	CDefJc;
		typedef CMathBottomNodesEx<SimpleTypes::COnOff,				OOX::et_m_degHide>	CDegHide;
		typedef CMathBottomNodesEx<SimpleTypes::COnOff,				OOX::et_m_diff>		CDiff;
		typedef CMathBottomNodesEx<SimpleTypes::COnOff,				OOX::et_m_dispDef>	CDispDef;
		typedef CMathBottomNodesEx<SimpleTypes::CMChar,				OOX::et_m_endChr>	CEndChr;
		typedef CMathBottomNodesEx<SimpleTypes::COnOff,				OOX::et_m_grow>		CGrow;
		typedef CMathBottomNodesEx<SimpleTypes::COnOff,				OOX::et_m_hideBot>	CHideBot;
		typedef CMathBottomNodesEx<SimpleTypes::COnOff,				OOX::et_m_hideLeft>	CHideLeft;
		typedef CMathBottomNodesEx<SimpleTypes::COnOff,				OOX::et_m_hideRight>CHideRight;
		typedef CMathBottomNodesEx<SimpleTypes::COnOff,				OOX::et_m_hideTop>	CHideTop;
		typedef CMathBottomNodesEx<SimpleTypes::CTwipsMeasure,		OOX::et_m_interSp>	CInterSp;
		typedef CMathBottomNodesEx<SimpleTypes::CLimLoc,			OOX::et_m_intLim>	CIntLim;
		typedef CMathBottomNodesEx<SimpleTypes::CTwipsMeasure,		OOX::et_m_intraSp>	CIntraSp;
		typedef CMathBottomNodesEx<SimpleTypes::CMJc,				OOX::et_m_jc>		CMJc;
		typedef CMathBottomNodesEx<SimpleTypes::CLimLoc,			OOX::et_m_limLoc>	CLimLoc;
		typedef CMathBottomNodesEx<SimpleTypes::COnOff,				OOX::et_m_lit>		CLit;
		typedef CMathBottomNodesEx<SimpleTypes::CTwipsMeasure,		OOX::et_m_lMargin>	CLMargin;
		typedef CMathBottomNodesEx<SimpleTypes::COnOff,				OOX::et_m_maxDist>	CMaxDist;
		typedef CMathBottomNodesEx<SimpleTypes::CXAlign,			OOX::et_m_mcJc>		CMcJc;
		typedef CMathBottomNodesEx<SimpleTypes::CLimLoc,			OOX::et_m_naryLim>	CNaryLim;
		typedef CMathBottomNodesEx<SimpleTypes::COnOff,				OOX::et_m_noBreak>	CNoBreak;
		typedef CMathBottomNodesEx<SimpleTypes::COnOff,				OOX::et_m_nor>		CNor;
		typedef CMathBottomNodesEx<SimpleTypes::COnOff,				OOX::et_m_objDist>	CObjDist;
		typedef CMathBottomNodesEx<SimpleTypes::COnOff,				OOX::et_m_opEmu>	COpEmu;
		typedef CMathBottomNodesEx<SimpleTypes::COnOff,				OOX::et_m_plcHide>	CPlcHide;
		typedef CMathBottomNodesEx<SimpleTypes::CTopBot,			OOX::et_m_pos>		CPos;
		typedef CMathBottomNodesEx<SimpleTypes::CTwipsMeasure,		OOX::et_m_postSp>	CPostSp;
		typedef CMathBottomNodesEx<SimpleTypes::CTwipsMeasure,		OOX::et_m_preSp>	CPreSp;
		typedef CMathBottomNodesEx<SimpleTypes::CTwipsMeasure,		OOX::et_m_rMargin>	CRMargin;
		typedef CMathBottomNodesEx<SimpleTypes::CUnSignedInteger,	OOX::et_m_rSp>		CRSp;
		typedef CMathBottomNodesEx<SimpleTypes::CSpacingRule,		OOX::et_m_rSpRule>	CRSpRule;
		typedef CMathBottomNodesEx<SimpleTypes::CScript,			OOX::et_m_scr>		CScr;
		typedef CMathBottomNodesEx<SimpleTypes::CMChar,				OOX::et_m_sepChr>	CSepChr;
		typedef CMathBottomNodesEx<SimpleTypes::COnOff,				OOX::et_m_show>		CShow;
		typedef CMathBottomNodesEx<SimpleTypes::CShp,				OOX::et_m_shp>		CShp;
		typedef CMathBottomNodesEx<SimpleTypes::COnOff,				OOX::et_m_smallFrac>CSmallFrac;
		typedef CMathBottomNodesEx<SimpleTypes::COnOff,				OOX::et_m_strikeBLTR>CStrikeBLTR;
		typedef CMathBottomNodesEx<SimpleTypes::COnOff,				OOX::et_m_strikeH>	CStrikeH;
		typedef CMathBottomNodesEx<SimpleTypes::COnOff,				OOX::et_m_strikeTLBR>CStrikeTLBR;
		typedef CMathBottomNodesEx<SimpleTypes::COnOff,				OOX::et_m_strikeV>	CStrikeV;
		typedef CMathBottomNodesEx<SimpleTypes::CStyle,				OOX::et_m_sty>		CSty;
		typedef CMathBottomNodesEx<SimpleTypes::COnOff,				OOX::et_m_subHide>	CSubHide;
		typedef CMathBottomNodesEx<SimpleTypes::COnOff,				OOX::et_m_supHide>	CSupHide;
		typedef CMathBottomNodesEx<SimpleTypes::COnOff,				OOX::et_m_transp>	CTransp;
		typedef CMathBottomNodesEx<SimpleTypes::CFType,				OOX::et_m_type>		CType;
		typedef CMathBottomNodesEx<SimpleTypes::CTopBot,			OOX::et_m_vertJc>	CVertJc;
		typedef CMathBottomNodesEx<SimpleTypes::CTwipsMeasure,		OOX::et_m_wrapIndent>CWrapIndent;
		typedef CMathBottomNodesEx<SimpleTypes::COnOff,				OOX::et_m_wrapRight>CWrapRight;
		typedef CMathBottomNodesEx<SimpleTypes::COnOff,				OOX::et_m_zeroAsc>	CZeroAsc;
		typedef CMathBottomNodesEx<SimpleTypes::COnOff,				OOX::et_m_zeroDesc>	CZeroDesc;
		typedef CMathBottomNodesEx<SimpleTypes::COnOff,				OOX::et_m_zeroWid>	CZeroWid;

	}// Logic
}//OOX
