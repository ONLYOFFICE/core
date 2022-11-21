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

#include "../../Base/Nullable.h"
#include "../../Common/SimpleTypes_Drawing.h"
#include "../../Common/SimpleTypes_Shared.h"

#include "../WritingElement.h"
#include "../RId.h"

#include "../../PPTXFormat/Logic/GraphicFrame.h"

#include "../../SystemUtility/SystemUtility.h"

namespace OOX
{
	namespace Drawing
	{
		//-----------------------------------------------------------------------
		// EffectExtent 20.4.2.6
		//-----------------------------------------------------------------------

		class CEffectExtent : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CEffectExtent)

			CEffectExtent();
			virtual ~CEffectExtent();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			SimpleTypes::CCoordinate m_oB;
			SimpleTypes::CCoordinate m_oL;
			SimpleTypes::CCoordinate m_oR;
			SimpleTypes::CCoordinate m_oT;
		};

		//--------------------------------------------------------------------------------
		// CPosH 20.4.2.10 (Part 1)
		//--------------------------------------------------------------------------------	

		class CPosH : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPosH)

			CPosH();
			virtual ~CPosH();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			bool IsAlign() const;
			bool IsPosOffset() const;

			bool										m_bAlign; // используем Align или (PosOffset или PctOffset)

			nullable<SimpleTypes::CRelFromH>			m_oRelativeFrom;

			nullable<SimpleTypes::CAlignH>				m_oAlign;
			nullable<SimpleTypes::CPositionOffset>		m_oPosOffset;
			nullable<SimpleTypes::CPercentage>			m_oPctOffset;
		};

		//--------------------------------------------------------------------------------
		// CPosV 20.4.2.11 (Part 1)
		//--------------------------------------------------------------------------------	

		class CPosV : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPosV)

			CPosV();
			virtual ~CPosV();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			bool IsAlign() const;
			bool IsPosOffset() const;

			bool										m_bAlign; // используем Align или PosOffset

			nullable<SimpleTypes::CRelFromV>			m_oRelativeFrom;

			nullable<SimpleTypes::CAlignV>				m_oAlign;
			nullable<SimpleTypes::CPositionOffset>		m_oPosOffset;
			nullable<SimpleTypes::CPercentage>			m_oPctOffset;
		};

		class CSizeRelH : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSizeRelH)

			CSizeRelH();
			virtual ~CSizeRelH();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
	// Attributes
			nullable<SimpleTypes::CSizeRelFromH>	m_oRelativeFrom;
	// Childs
			nullable<SimpleTypes::CPercentage>		m_oPctWidth;
		};

		class CSizeRelV : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSizeRelV)

			CSizeRelV();
			virtual ~CSizeRelV();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
	// Attributes
			nullable<SimpleTypes::CSizeRelFromV>	m_oRelativeFrom;
	// Childs
			nullable<SimpleTypes::CPercentage>		m_oPctHeight;
		};

		//--------------------------------------------------------------------------------
		// CWrapNone 20.4.2.15 (Part 1)
		//--------------------------------------------------------------------------------	

		class CWrapNone : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CWrapNone)

			CWrapNone();
			virtual ~CWrapNone();

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const;
		};

		//--------------------------------------------------------------------------------
		// CWrapSquare 20.4.2.17 (Part 1)
		//--------------------------------------------------------------------------------	

		class CWrapSquare : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CWrapSquare)

			CWrapSquare();
			virtual ~CWrapSquare();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CWrapDistance>	m_oDistB;
			nullable<SimpleTypes::CWrapDistance>	m_oDistL;
			nullable<SimpleTypes::CWrapDistance>	m_oDistR;
			nullable<SimpleTypes::CWrapDistance>	m_oDistT;
			nullable<SimpleTypes::CWrapText>		m_oWrapText;

			nullable<OOX::Drawing::CEffectExtent>	m_oEffectExtent;
		};

		//--------------------------------------------------------------------------------
		// CWrapPath 20.4.2.16 (Part 1)
		//--------------------------------------------------------------------------------	

		class CWrapPath : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CWrapPath)

			CWrapPath();
			virtual ~CWrapPath();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::COnOff >					m_oEdited;
			nullable<ComplexTypes::Drawing::CPoint2D>       m_oStart;
			std::vector<ComplexTypes::Drawing::CPoint2D*>	m_arrLineTo;
		};

		//--------------------------------------------------------------------------------
		// CWrapThrough 20.4.2.18 (Part 1)
		//--------------------------------------------------------------------------------	

		class CWrapThrough : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CWrapThrough)

			CWrapThrough();
			virtual ~CWrapThrough();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CWrapDistance>	m_oDistL;
			nullable<SimpleTypes::CWrapDistance>	m_oDistR;
			nullable<SimpleTypes::CWrapText>		m_oWrapText;
			nullable<OOX::Drawing::CWrapPath>		m_oWrapPolygon;
		};

		//--------------------------------------------------------------------------------
		// CWrapTight 20.4.2.19 (Part 1)
		//--------------------------------------------------------------------------------	

		class CWrapTight : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CWrapTight)

			CWrapTight();
			virtual ~CWrapTight();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CWrapDistance>	m_oDistL;
			nullable<SimpleTypes::CWrapDistance>	m_oDistR;
			nullable<SimpleTypes::CWrapText>		m_oWrapText;
            nullable<OOX::Drawing::CWrapPath>		m_oWrapPolygon;
		};

		//--------------------------------------------------------------------------------
		// CWrapTopBottom 20.4.2.20 (Part 1)
		//--------------------------------------------------------------------------------	

		class CWrapTopBottom : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CWrapTopBottom)

			CWrapTopBottom();
			virtual ~CWrapTopBottom();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CWrapDistance> m_oDistB;
			nullable<SimpleTypes::CWrapDistance> m_oDistT;
			nullable<OOX::Drawing::CEffectExtent>  m_oEffectExtent;
		};

		//--------------------------------------------------------------------------------
		// CAnchor 20.4.2.3 (Part 1)
		//--------------------------------------------------------------------------------	

		enum EAnchorWrapType
		{
			anchorwrapUnknown      = 0,
			anchorwrapNone         = 1,
			anchorwrapSquare       = 2,
			anchorwrapThrough      = 3,
			anchorwrapTight        = 4,
			anchorwrapTopAndBottom = 5
		};

		class CAnchor : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CAnchor)

			CAnchor(OOX::Document *pMain = NULL);
			virtual ~CAnchor();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XmlUtils::CXmlNode& oNode);

		public:
			nullable<EAnchorWrapType>								m_eWrapType;

			nullable<SimpleTypes::COnOff>							m_oAllowOverlap;
			nullable<SimpleTypes::COnOff>							m_oBehindDoc;
			nullable<SimpleTypes::CWrapDistance>					m_oDistB;
			nullable<SimpleTypes::CWrapDistance>					m_oDistL;
			nullable<SimpleTypes::CWrapDistance>					m_oDistR;
			nullable<SimpleTypes::CWrapDistance>					m_oDistT;
			nullable<SimpleTypes::COnOff>							m_oHidden;
			nullable<SimpleTypes::COnOff>							m_oLayoutInCell;
			nullable<SimpleTypes::COnOff>							m_oLocked;
			nullable<SimpleTypes::CUnsignedDecimalNumber>			m_oRelativeHeight;
			nullable<SimpleTypes::COnOff>							m_bSimplePos;

			nullable<PPTX::Logic::CNvPr>						m_oDocPr;			
			nullable<OOX::Drawing::CEffectExtent>				m_oEffectExtent;
			nullable<ComplexTypes::Drawing::CPositiveSize2D>	m_oExtent;
			nullable<OOX::Drawing::CPosH>						m_oPositionH;
			nullable<OOX::Drawing::CPosV>						m_oPositionV;
			nullable<OOX::Drawing::CSizeRelH>					m_oSizeRelH;
			nullable<OOX::Drawing::CSizeRelV>					m_oSizeRelV;
			nullable<ComplexTypes::Drawing::CPoint2D>			m_oSimplePos;
			nullable<OOX::Drawing::CWrapNone>					m_oWrapNone;
			nullable<OOX::Drawing::CWrapSquare>					m_oWrapSquare;
			nullable<OOX::Drawing::CWrapThrough>				m_oWrapThrough;
			nullable<OOX::Drawing::CWrapTight>					m_oWrapTight;
			nullable<OOX::Drawing::CWrapTopBottom>				m_oWrapTopAndBottom;

			PPTX::Logic::GraphicFrame							m_oGraphic;
		};

		//--------------------------------------------------------------------------------
		// CInline 20.4.2.8 (Part 1)
		//--------------------------------------------------------------------------------	

		class CInline : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CInline)

			CInline(OOX::Document *pMain = NULL);
			virtual ~CInline();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlNode& oNode);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
	// Attributes
			nullable<SimpleTypes::CWrapDistance>				m_oDistB;
			nullable<SimpleTypes::CWrapDistance>				m_oDistL;
			nullable<SimpleTypes::CWrapDistance>				m_oDistR;
			nullable<SimpleTypes::CWrapDistance>				m_oDistT;
	// Childs
			nullable<PPTX::Logic::CNvPr>						m_oDocPr;
			nullable<OOX::Drawing::CEffectExtent>				m_oEffectExtent;
			nullable<ComplexTypes::Drawing::CPositiveSize2D>	m_oExtent;
			PPTX::Logic::GraphicFrame							m_oGraphic;
		};

	} // Drawing
} // OOX

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// CDrawing 17.3.3.9 (Part 1)
		//--------------------------------------------------------------------------------	

		class CDrawing : public WritingElement
		{
		public:
			CDrawing(OOX::Document *pMain = NULL);
			CDrawing(XmlUtils::CXmlNode& oNode);
			CDrawing(XmlUtils::CXmlLiteReader& oReader);

			virtual ~CDrawing();

			const CDrawing& operator =(const XmlUtils::CXmlNode& oNode);
			const CDrawing& operator =(const XmlUtils::CXmlLiteReader& oReader);

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;

			virtual EElementType getType() const;

			bool IsAnchor() const;
			bool IsInline() const;

			bool                            m_bAnchor; // используем Anchor или Inline
	// Childs
			nullable<OOX::Drawing::CAnchor> m_oAnchor;
			nullable<OOX::Drawing::CInline> m_oInline;
		};

	} // namespace Logic
} // namespace OOX

