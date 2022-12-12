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

#include "../../Common/ComplexTypes.h"

#include "oMathBottomNodes.h"
#include "OMath.h"

#include "../Logic/RunContent.h"
#include "../Logic/FldChar.h"
#include "../Logic/RunProperty.h"
#include "../Logic/Annotations.h"
#include "../Logic/Pict.h"
#include "../Drawing/Drawing.h"
#include "../../PPTXFormat/Logic/RunProperties.h"


namespace OOX
{	
	// Elements 22.1.2
	namespace Logic
	{
		#define CMathArgNodesEx(name, type)\
			class name : public CMathArgNodes\
			{\
			public:\
				name(OOX::Document *pMain = NULL) : CMathArgNodes(pMain) {m_eType = type;}\
				name(XmlUtils::CXmlNode& oNode)\
				{\
					fromXML( oNode );\
				}\
				name(XmlUtils::CXmlLiteReader& oReader)\
				{\
					fromXML( oReader );\
				}\
				virtual EElementType getType() const\
				{\
					return type;\
				}\
			};
		CMathArgNodesEx(CDeg, OOX::et_m_deg)
		CMathArgNodesEx(CDen, OOX::et_m_den)
		CMathArgNodesEx(CElement, OOX::et_m_e)
		CMathArgNodesEx(CFName, OOX::et_m_fName)
		CMathArgNodesEx(CLim, OOX::et_m_lim)
		CMathArgNodesEx(CNum, OOX::et_m_num)
		CMathArgNodesEx(COMath, OOX::et_m_oMath)
		CMathArgNodesEx(CSub, OOX::et_m_sub)
		CMathArgNodesEx(CSup, OOX::et_m_sup)
//-------------------------------------------------------------------------------------------------------------------------------

		//--------------------------------------------------------------------------------
		// CCtrlPr 22.1.2.23 (Control Properties) 
		//--------------------------------------------------------------------------------
		class CCtrlPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCtrlPr)
			CCtrlPr();
			virtual ~CCtrlPr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CRunProperty> m_oRPr;
			nullable<PPTX::Logic::RunProperties> m_oARPr;
			nullable<OOX::Logic::CRPrChange> m_oIns;
			nullable<OOX::Logic::CRPrChange> m_oDel;
		};

		//--------------------------------------------------------------------------------
		// CAccPr 22.1.2.2 (Accent Properties)  
		//--------------------------------------------------------------------------------
		class CAccPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CAccPr)
			CAccPr();
			virtual ~CAccPr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CChr>			m_oChr;
			nullable<OOX::Logic::CCtrlPr>		m_oCtrlPr;
		};

		//--------------------------------------------------------------------------------
		// CAcc 22.1.2.1  (Accent) 
		//--------------------------------------------------------------------------------
		class CAcc : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CAcc)
			CAcc();
			virtual ~CAcc();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			std::wstring      toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CAccPr>		m_oAccPr;
			nullable<OOX::Logic::CElement>		m_oElement;
		};

		//--------------------------------------------------------------------------------
		// CArgPr 22.1.2.5  (Argument Properties) 
		//--------------------------------------------------------------------------------
		class CArgPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CArgPr)
			CArgPr();
			virtual ~CArgPr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CArgSz> m_oArgSz;
		};		

		//--------------------------------------------------------------------------------
		// CBarPr 22.1.2.8 (Bar Properties) 
		//--------------------------------------------------------------------------------
		class CBarPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBarPr)
			CBarPr();
			virtual ~CBarPr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CCtrlPr> m_oCtrlPr;
			nullable<OOX::Logic::CPos> m_oPos;
		};		

		//--------------------------------------------------------------------------------
		// CBar 22.1.2.7 (Bar) 
		//--------------------------------------------------------------------------------
		class CBar : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBar)
			CBar();
			virtual ~CBar();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CBarPr>		m_oBarPr;
			nullable<OOX::Logic::CElement>		m_oElement;

		};

		//--------------------------------------------------------------------------------
		// CBorderBoxPr 22.1.2.12 (Border-Box Properties) 
		//--------------------------------------------------------------------------------
		class CBorderBoxPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBorderBoxPr)
			CBorderBoxPr();
			virtual ~CBorderBoxPr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CCtrlPr>			 m_oCtrlPr;
			nullable<OOX::Logic::CHideBot>			 m_oHideBot;
			nullable<OOX::Logic::CHideLeft>			 m_oHideLeft;
			nullable<OOX::Logic::CHideRight>		 m_oHideRight;
			nullable<OOX::Logic::CHideTop>			 m_oHideTop;
			nullable<OOX::Logic::CStrikeBLTR>		 m_oStrikeBLTR;
			nullable<OOX::Logic::CStrikeH>			 m_oStrikeH;
			nullable<OOX::Logic::CStrikeTLBR>		 m_oStrikeTLBR;
			nullable<OOX::Logic::CStrikeV>			 m_oStrikeV;
		};

		//--------------------------------------------------------------------------------
		// CBorderBox 22.1.2.11 (Border-Box Object) 
		//--------------------------------------------------------------------------------
		class CBorderBox : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBorderBox)
			CBorderBox();;
			virtual ~CBorderBox();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CBorderBoxPr>	 m_oBorderBoxPr;
			nullable<OOX::Logic::CElement>		 m_oElement;
		};		 

		//--------------------------------------------------------------------------------
		// CBrk 22.1.2.15 (Break) 
		//--------------------------------------------------------------------------------
		class CBrk : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBrk)
			CBrk();
			virtual ~CBrk();

			virtual void  fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CInteger255> m_alnAt;
		};

		//--------------------------------------------------------------------------------
		// CBoxPr 22.1.2.14 (Box Properties)
		//--------------------------------------------------------------------------------
		class CBoxPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBoxPr)
			CBoxPr();
			virtual ~CBoxPr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CAln>			 m_oAln;
			nullable<OOX::Logic::CBrk>			 m_oBrk;
			nullable<OOX::Logic::CCtrlPr>		 m_oCtrlPr;
			nullable<OOX::Logic::CDiff>			 m_oDiff;
			nullable<OOX::Logic::CNoBreak>		 m_oNoBreak;
			nullable<OOX::Logic::COpEmu>		 m_oOpEmu;
		};		

		//--------------------------------------------------------------------------------
		// CBox 22.1.2.13 (Box Object) 
		//--------------------------------------------------------------------------------
		class CBox : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBox)
			CBox();
			virtual ~CBox();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CBoxPr> m_oBoxPr;
			nullable<OOX::Logic::CElement> m_oElement;
		};		 

		//--------------------------------------------------------------------------------
		// CDelimiterPr 22.1.2.31 (Delimiter Properties) 
		//--------------------------------------------------------------------------------
		class CDelimiterPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDelimiterPr)
			CDelimiterPr();
			virtual ~CDelimiterPr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CBegChr>		m_oBegChr;
			nullable<OOX::Logic::CCtrlPr>		m_oCtrlPr;
			nullable<OOX::Logic::CEndChr>		m_oEndChr;
			nullable<OOX::Logic::CGrow>			m_oGrow;
			nullable<OOX::Logic::CSepChr>		m_oSepChr;
			nullable<OOX::Logic::CShp>			m_oShp;
		};		

		//--------------------------------------------------------------------------------
		// CDelimiter 22.1.2.24 (Delimiter Object) 
		//--------------------------------------------------------------------------------
		class CDelimiter : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CDelimiter)
			CDelimiter();
			virtual ~CDelimiter();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			LONG m_lColumn;
		};

		//--------------------------------------------------------------------------------
		// CEqArrPr 22.1.2.35 (Array Object)  
		//--------------------------------------------------------------------------------
		class CEqArrPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CEqArrPr)
			CEqArrPr();
			virtual ~CEqArrPr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CBaseJc>		m_oBaseJc;
			nullable<OOX::Logic::CCtrlPr>		m_oCtrlPr;
			nullable<OOX::Logic::CMaxDist>		m_oMaxDist;
			nullable<OOX::Logic::CObjDist>		m_oObjDist;
			nullable<OOX::Logic::CRSp>			m_oRSp;
			nullable<OOX::Logic::CRSpRule>		m_oRSpRule;
		};

		//--------------------------------------------------------------------------------
		// CEqArr 22.1.2.34 (Array Object)  
		//--------------------------------------------------------------------------------
		class CEqArr : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CEqArr)
			CEqArr();
			virtual ~CEqArr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			LONG    m_lRow;
		};				

		//--------------------------------------------------------------------------------
		// CFPr 22.1.2.38 (Fraction Properties) 
		//--------------------------------------------------------------------------------
		class CFPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFPr)
			CFPr();
			virtual ~CFPr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CCtrlPr>		m_oCtrlPr;
			nullable<OOX::Logic::CType>			m_oType;
		};		

		//--------------------------------------------------------------------------------
		// CFraction 22.1.2.36 (Fraction Object)  
		//--------------------------------------------------------------------------------
		class CFraction : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFraction)
			CFraction();
			virtual ~CFraction();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CDen>			m_oDen;
			nullable<OOX::Logic::CFPr>			m_oFPr;
			nullable<OOX::Logic::CNum>			m_oNum;
		};		

		//--------------------------------------------------------------------------------
		// CFuncPr 22.1.2.40 (Function Properties) 
		//--------------------------------------------------------------------------------
		class CFuncPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFuncPr)
			CFuncPr();
			virtual ~CFuncPr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CCtrlPr>		m_oCtrlPr;
		};		

		//--------------------------------------------------------------------------------
		// CFunc 22.1.2.39 (Function Apply Object) 
		//--------------------------------------------------------------------------------
		class CFunc : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFunc)
			CFunc();
			virtual ~CFunc();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CElement>		m_oElement;
			nullable<OOX::Logic::CFName>		m_oFName;
			nullable<OOX::Logic::CFuncPr>		m_oFuncPr;
		};		

		//--------------------------------------------------------------------------------
		// CGroupChrPr 22.1.2.42 (Group-Character Properties ) 
		//--------------------------------------------------------------------------------
		class CGroupChrPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CGroupChrPr)
			CGroupChrPr();
			virtual ~CGroupChrPr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CChr>			m_oChr;
			nullable<OOX::Logic::CCtrlPr>		m_oCtrlPr;
			nullable<OOX::Logic::CPos>			m_oPos;
			nullable<OOX::Logic::CVertJc>		m_oVertJc;
		};

		//--------------------------------------------------------------------------------
		// CGroupChr 22.1.2.41 (Group-Character Object) 
		//--------------------------------------------------------------------------------
		class CGroupChr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CGroupChr)
			CGroupChr();
			virtual ~CGroupChr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CElement>		m_oElement;
			nullable<OOX::Logic::CGroupChrPr>	m_oGroupChrPr;
		};

		//--------------------------------------------------------------------------------
		// CLimLowPr 22.1.2.55 (Lower-Limit Properties)
		//--------------------------------------------------------------------------------
		class CLimLowPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CLimLowPr)
			CLimLowPr();
			virtual ~CLimLowPr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CCtrlPr> m_oCtrlPr;
		};		 

		//--------------------------------------------------------------------------------
		// CLimLow 22.1.2.54 (Lower-Limit Object) 
		//--------------------------------------------------------------------------------
		class CLimLow : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CLimLow)
			CLimLow();
			virtual ~CLimLow();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CElement>		 m_oElement;
			nullable<OOX::Logic::CLim>			 m_oLim;
			nullable<OOX::Logic::CLimLowPr>		 m_oLimLowPr;
		};		

		//--------------------------------------------------------------------------------
		// CLimUppPr 22.1.2.57 (Upper-Limit Properties) 
		//--------------------------------------------------------------------------------
		class CLimUppPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CLimUppPr)
			CLimUppPr();
			virtual ~CLimUppPr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CCtrlPr> m_oCtrlPr;
		};		

		//--------------------------------------------------------------------------------
		// CLimUpp 22.1.2.56 (Upper-Limit Object)
		//--------------------------------------------------------------------------------
		class CLimUpp : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CLimUpp)
			CLimUpp();
			virtual ~CLimUpp();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CElement>		 m_oElement;
			nullable<OOX::Logic::CLim>			 m_oLim;
			nullable<OOX::Logic::CLimUppPr>		 m_oLimUppPr;
		};			

		//--------------------------------------------------------------------------------
		// CMathFont 22.1.2.61 (Math Font) 
		//--------------------------------------------------------------------------------
		class CMathFont : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CMathFont)
			CMathFont();
			virtual ~CMathFont();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_string m_val;
		};			 

		//--------------------------------------------------------------------------------
		// CMathPr 22.1.2.62 (Math Properties) 
		//--------------------------------------------------------------------------------
		class CMathPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CMathPr)
			CMathPr();
			virtual ~CMathPr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CBrkBin> m_oBrkBin;
			nullable<OOX::Logic::CBrkBinSub> m_oBrkBinSub;
			nullable<OOX::Logic::CDefJc> m_oDefJc;
			nullable<OOX::Logic::CDispDef> m_oDispDef;
			nullable<OOX::Logic::CInterSp> m_oInterSp;
			nullable<OOX::Logic::CIntLim> m_oIntLim;
			nullable<OOX::Logic::CIntraSp> m_oIntraSp;
			nullable<OOX::Logic::CLMargin> m_oLMargin;
			nullable<OOX::Logic::CMathFont> m_oMathFont;
			nullable<OOX::Logic::CNaryLim> m_oNaryLim;
			nullable<OOX::Logic::CPostSp> m_oPostSp;
			nullable<OOX::Logic::CPreSp> m_oPreSp;
			nullable<OOX::Logic::CRMargin> m_oRMargin;
			nullable<OOX::Logic::CSmallFrac> m_oSmallFrac;
			nullable<OOX::Logic::CWrapIndent> m_oWrapIndent;
			nullable<OOX::Logic::CWrapRight> m_oWrapRight;
		};		

		//--------------------------------------------------------------------------------
		// CMcPr 22.1.2.66 (Matrix Column Properties) 
		//--------------------------------------------------------------------------------
		class CMcPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CMcPr)
			CMcPr();
			virtual ~CMcPr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CCount> m_oCount;
			nullable<OOX::Logic::CMcJc> m_oMcJc;
		};		

		//--------------------------------------------------------------------------------
		// CMc 22.1.2.64  (Matrix Column)
		//--------------------------------------------------------------------------------
		class CMc : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CMc)
			CMc();
			virtual ~CMc();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CMcPr> m_oMcPr;
		};		

		//--------------------------------------------------------------------------------
		// CMcs 22.1.2.67 (Matrix Columns) 
		//--------------------------------------------------------------------------------
		class CMcs : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CMcs)
			CMcs();
			virtual ~CMcs();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;
		};

		//--------------------------------------------------------------------------------
		// CMPr 22.1.2.68   (Matrix Properties) 
		//--------------------------------------------------------------------------------
		class CMPr : public WritingElement
		{
		public:
			CMPr();
			CMPr(XmlUtils::CXmlNode &oNode);
			CMPr(XmlUtils::CXmlLiteReader& oReader);

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CBaseJc>		m_oBaseJc;
			nullable<OOX::Logic::CCGp>			m_oCGp;
			nullable<OOX::Logic::CCGpRule>		m_oCGpRule;
			nullable<OOX::Logic::CCSp>			m_oCSp;
			nullable<OOX::Logic::CCtrlPr>		m_oCtrlPr;
			nullable<OOX::Logic::CMcs>			m_oMcs;
			nullable<OOX::Logic::CPlcHide>		m_oPlcHide;
			nullable<OOX::Logic::CRSp>			m_oRSp;
			nullable<OOX::Logic::CRSpRule>		m_oRSpRule;
		};

		//--------------------------------------------------------------------------------
		// CMr 22.1.2.69 (Matrix Row)
		//--------------------------------------------------------------------------------
		class CMr : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CMr)
			CMr();
			virtual ~CMr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			LONG GetCol();

			LONG	 m_lCol;
		};

		//--------------------------------------------------------------------------------
		// CMatrix 22.1.2.60 (Matrix Object) 
		//--------------------------------------------------------------------------------
		class CMatrix : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CMatrix)
			CMatrix();
			virtual ~CMatrix();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			LONG m_lRow;
		};

		//--------------------------------------------------------------------------------
		// CNaryPr 22.1.2.72   (n-ary Properties) 
		//--------------------------------------------------------------------------------
		class CNaryPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CNaryPr)
			CNaryPr();
			virtual ~CNaryPr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CChr>				m_oChr;
			nullable<OOX::Logic::CCtrlPr>			m_oCtrlPr;
			nullable<OOX::Logic::CGrow>				m_oGrow;
			nullable<OOX::Logic::CLimLoc>			m_oLimLoc;
			nullable<OOX::Logic::CSubHide>			m_oSubHide;
			nullable<OOX::Logic::CSupHide>			m_oSupHide;
		};		

		//--------------------------------------------------------------------------------
		// CNary 22.1.2.70 (n-ary Operator Object)
		//--------------------------------------------------------------------------------
		class CNary : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CNary)
			CNary();
			virtual ~CNary();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CElement>		m_oElement;
			nullable<OOX::Logic::CNaryPr>		m_oNaryPr;
			nullable<OOX::Logic::CSub>			m_oSub;
			nullable<OOX::Logic::CSup>			m_oSup;
		};		

		//--------------------------------------------------------------------------------
		// COMathParaPr 22.1.2.79 
		//--------------------------------------------------------------------------------
		class COMathParaPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(COMathParaPr)
			COMathParaPr();
			virtual ~COMathParaPr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CMJc>  m_oMJc;
		};			

		//--------------------------------------------------------------------------------
		// CPhantPr 22.1.2.82   (Phantom Properties) 
		//--------------------------------------------------------------------------------
		class CPhantPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPhantPr)
			CPhantPr();
			virtual ~CPhantPr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CCtrlPr>			m_oCtrlPr;
			nullable<OOX::Logic::CShow>				m_oShow;
			nullable<OOX::Logic::CTransp>			m_oTransp;
			nullable<OOX::Logic::CZeroAsc>			m_oZeroAsc;
			nullable<OOX::Logic::CZeroDesc>			m_oZeroDesc;
			nullable<OOX::Logic::CZeroWid>			m_oZeroWid;
		};		 

		//--------------------------------------------------------------------------------
		// CPhant 22.1.2.81   (Phantom Object) 
		//--------------------------------------------------------------------------------
		class CPhant : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPhant)
			CPhant();
			virtual ~CPhant();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CElement>  m_oElement;
			nullable<OOX::Logic::CPhantPr>  m_oPhantPr;
		};

		//--------------------------------------------------------------------------------
		// CMText 22.1.2.116  (Math Text) 
		//--------------------------------------------------------------------------------	
		class CMText : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CMText)
			CMText();
			virtual ~CMText();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CXmlSpace> m_oSpace;
			std::wstring m_sText;
		};		

		//--------------------------------------------------------------------------------
		// CMRPr 22.1.2.91   (Run Properties) 
		//--------------------------------------------------------------------------------
		class CMRPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CMRPr)
			CMRPr();
			virtual ~CMRPr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			nullable<OOX::Logic::CAln> m_oAln;
			nullable<OOX::Logic::CBrk> m_oBrk;
			nullable<OOX::Logic::CLit> m_oLit;
			nullable<OOX::Logic::CNor> m_oNor;
			nullable<OOX::Logic::CScr> m_oScr;
			nullable<OOX::Logic::CSty> m_oSty;
		};		
		class CMDel;
		class CMIns;

		//--------------------------------------------------------------------------------
		// CMRun 22.1.2.87  (Math Run)
		//--------------------------------------------------------------------------------
		class CMRun : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CMRun)
			CMRun();
			virtual ~CMRun();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;			
			virtual std::wstring toXMLInner() const;

			virtual EElementType getType() const;

	// Childs
			nullable<OOX::Logic::CAnnotationRef>			m_oAnnotationRef;
			nullable<OOX::Logic::CBr>						m_oBr;
			nullable<OOX::Logic::CCommentReference>			m_oCommentReference;
			nullable<OOX::Logic::CContentPart>				m_oContentPart;
			nullable<OOX::Logic::CContinuationSeparator>	m_oContinuationSeparator;
			nullable<OOX::Logic::CCr>						m_oCr;
			nullable<OOX::Logic::CDayLong>					m_oDayLong;
			nullable<OOX::Logic::CDayShort>					m_oDayShort;
			nullable<OOX::Logic::CMDel>						m_oDel;
			nullable<OOX::Logic::CDelInstrText>				m_oDelInstrText;
			nullable<OOX::Logic::CDelText>					m_oDelText;
			nullable<OOX::Logic::CDrawing>					m_oDrawing;
			nullable<OOX::Logic::CEndnoteRef>				m_oEndnoteRef;
			nullable<OOX::Logic::CEndnoteReference>			m_oEndnoteReference;
			nullable<OOX::Logic::CFldChar>					m_oFldChar;
			nullable<OOX::Logic::CFootnoteRef>				m_oFootnoteRef;
			nullable<OOX::Logic::CFootnoteReference>		m_oFootnoteReference;
			nullable<OOX::Logic::CMIns>						m_oIns;
			nullable<OOX::Logic::CInstrText>				m_oInstrText;
			nullable<OOX::Logic::CLastRenderedPageBreak>	m_oLastRenderedPageBreak;
			nullable<OOX::Logic::CMonthLong>				m_oMonthLong;
			nullable<OOX::Logic::CMonthShort>				m_oMonthShort;
			nullable<OOX::Logic::CNoBreakHyphen>			m_oNoBreakHyphen;
			nullable<OOX::Logic::CObject>					m_oObject;
			nullable<OOX::Logic::CPgNum>					m_oPgNum;
			nullable<OOX::Logic::CPTab>						m_oPtab;
			nullable<OOX::Logic::CRunProperty>				m_oRPr;
			nullable<OOX::Logic::CMRPr>						m_oMRPr;
			nullable<PPTX::Logic::RunProperties>			m_oARPr;
			nullable<OOX::Logic::CRuby>						m_oRuby;
			nullable<OOX::Logic::CSeparator>				m_oSeparator;
			nullable<OOX::Logic::CSoftHyphen>				m_oSoftHyphen;
			nullable<OOX::Logic::CSym>						m_oSym;
			nullable<OOX::Logic::CMText>					m_oMText;
			nullable<OOX::Logic::CText>						m_oText;
			nullable<OOX::Logic::CTab>						m_oTab;
			nullable<OOX::Logic::CYearLong>					m_oYearLong;
			nullable<OOX::Logic::CYearShort>				m_oYearShort;
		};

		class CMDel : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CMDel)
			CMDel();
			virtual ~CMDel();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<std::wstring>					m_sAuthor;
			nullable<SimpleTypes::CDateTime>		m_oDate;
			nullable<SimpleTypes::CDecimalNumber>	m_oId;
			nullable<std::wstring>					m_sUserId;

			// Childs
			nullable<CMRun>							m_oRun;
		};

		class CMIns : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CMIns)
			CMIns();
			virtual ~CMIns();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<std::wstring                  >	m_sAuthor;
			nullable<SimpleTypes::CDateTime        >	m_oDate;
			nullable<SimpleTypes::CDecimalNumber   >	m_oId;
			nullable<std::wstring                  >	m_sUserId;

			// Childs
			nullable<CMRun>								m_oRun;
		};

		//--------------------------------------------------------------------------------
		// CRadPr 22.1.2.89   (Radical Properties)
		//--------------------------------------------------------------------------------
		class CRadPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CRadPr)
			CRadPr();
			virtual ~CRadPr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

	//Childs
			nullable<OOX::Logic::CCtrlPr>				m_oCtrlPr;
			nullable<OOX::Logic::CDegHide>				m_oDegHide;
		};		

		//--------------------------------------------------------------------------------
		// CRad 22.1.2.88   (Radical Object)
		//--------------------------------------------------------------------------------
		class CRad : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CRad)
			CRad();
			virtual ~CRad();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

	//Childs
			nullable<OOX::Logic::CDeg>				m_oDeg;
			nullable<OOX::Logic::CElement>			m_oElement;
			nullable<OOX::Logic::CRadPr>			m_oRadPr;
		};		 

		//--------------------------------------------------------------------------------
		// CSPrePr 22.1.2.100   (Pre-Sub-Superscript Properties)) 
		//--------------------------------------------------------------------------------
		class CSPrePr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSPrePr)
			CSPrePr();
			virtual ~CSPrePr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

	//Childs
			nullable<OOX::Logic::CCtrlPr>	m_oCtrlPr;
		};		

		//--------------------------------------------------------------------------------
		// CSPre 22.1.2.99   (Pre-Sub-Superscript Object) 
		//--------------------------------------------------------------------------------
		class CSPre : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSPre)
			CSPre();
			virtual ~CSPre();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

	//Childs
			nullable<OOX::Logic::CElement>		m_oElement;
			nullable<OOX::Logic::CSPrePr>		m_oSPrePr;
			nullable<OOX::Logic::CSub>			m_oSub;
			nullable<OOX::Logic::CSup>			m_oSup;
		};

		//--------------------------------------------------------------------------------
		// CSSubPr 22.1.2.102   (Subscript Properties) 
		//--------------------------------------------------------------------------------
		class CSSubPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSSubPr)
			CSSubPr();
			virtual ~CSSubPr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

	//Childs
			nullable<OOX::Logic::CCtrlPr>		m_oCtrlPr;
		};		

		//--------------------------------------------------------------------------------
		// CSSub 22.1.2.101   (Subscript Object) 
		//--------------------------------------------------------------------------------
		class CSSub : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSSub)
			CSSub();
			virtual ~CSSub();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

	//Childs
			nullable<OOX::Logic::CElement>		m_oElement;
			nullable<OOX::Logic::CSSubPr>		m_oSSubPr;
			nullable<OOX::Logic::CSub>			m_oSub;
		};

		//--------------------------------------------------------------------------------
		// CSSubSupPr 22.1.2.104   (Sub-Superscript Properties) 
		//--------------------------------------------------------------------------------
		class CSSubSupPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSSubSupPr)
			CSSubSupPr();
			virtual ~CSSubSupPr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

	//Childs
			nullable<OOX::Logic::CAlnScr>		m_oAlnScr;
			nullable<OOX::Logic::CCtrlPr>		m_oCtrlPr;
		};		

		//--------------------------------------------------------------------------------
		// CSSubSup 22.1.2.103   (Sub-Superscript Object)  
		//--------------------------------------------------------------------------------
		class CSSubSup : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSSubSup)
			CSSubSup();
			virtual ~CSSubSup();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

	//Childs
			nullable<OOX::Logic::CElement>		m_oElement;
			nullable<OOX::Logic::CSSubSupPr>	m_oSSubSupPr;
			nullable<OOX::Logic::CSub>			m_oSub;
			nullable<OOX::Logic::CSup>			m_oSup;
		};		

		//--------------------------------------------------------------------------------
		// CSSupPr 22.1.2.106   (Superscript Properties)
		//--------------------------------------------------------------------------------
		class CSSupPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSSupPr)
			CSSupPr();
			virtual ~CSSupPr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

	//Childs
			nullable<OOX::Logic::CCtrlPr>		m_oCtrlPr;
		};		

		//--------------------------------------------------------------------------------
		// CSSup 22.1.2.105   (Superscript Object)
		//--------------------------------------------------------------------------------
		class CSSup : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSSup)
			CSSup();
			virtual ~CSSup();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

	//Childs
			nullable<OOX::Logic::CElement>		m_oElement;
			nullable<OOX::Logic::CSSupPr>		m_oSSupPr;
			nullable<OOX::Logic::CSup>			m_oSup;
		};

	} // namespace OMath	
} // namespace OOX

