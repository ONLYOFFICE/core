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
#ifndef OOX_VML_OFFICE_INCLUDE_H_
#define OOX_VML_OFFICE_INCLUDE_H_

#include "../../Base/Nullable.h"

#include "../../Common/ComplexTypes.h"

#include "../../Common/SimpleTypes_Shared.h"
#include "../../Common/SimpleTypes_Word.h"
#include "../../Common/SimpleTypes_Vml.h"

#include "../WritingElement.h"
#include "../RId.h"

#include "../../PPTXFormat/WrapperWritingElement.h"

namespace OOX
{
	namespace VmlOffice
	{
		//--------------------------------------------------------------------------------
		// CStrokeChild 14.2.2.1;14.2.2.6;14.2.2.16;14.2.2.26;14.2.2.32 (Part 4)
		//--------------------------------------------------------------------------------	
		class CStrokeChild : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CStrokeChild)
			CStrokeChild(OOX::Document *pMain = NULL);
			virtual ~CStrokeChild();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;


		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			EElementType                                m_eType;

			nullable<std::wstring>						m_sAlthref;
			nullable<SimpleTypes::CColorType>			m_oColor;
			nullable<SimpleTypes::CColorType>		    m_oColor2;
			nullable<SimpleTypes::Vml::CVmlDashStyle>	m_oDashStyle;
			nullable<SimpleTypes::CStrokeArrowType>		m_oEndArrow;
			nullable<SimpleTypes::CStrokeArrowLength>	m_oEndArrowLength;
			nullable<SimpleTypes::CStrokeArrowWidth>	m_oEndArrowWidth;
			nullable<SimpleTypes::CStrokeEndCap>		m_oEndCap;
			nullable<SimpleTypes::CExt>					m_oExt;
			nullable<SimpleTypes::CFillType>		    m_oFillType;
			nullable<SimpleTypes::CTrueFalse>		    m_oForceDash;
			nullable<std::wstring>						m_sHref;
			nullable<SimpleTypes::CTrueFalse>           m_oImageAlignShape;
			nullable<SimpleTypes::CImageAspect>			m_oImageAspect;
			nullable<std::wstring>						m_sImageSize;
			nullable<SimpleTypes::CTrueFalse>           m_oInsetPen;
			nullable<SimpleTypes::CStrokeJoinStyle>		m_oJoinStyle;
			nullable<SimpleTypes::CStrokeLineStyle>		m_oLineStyle;
			nullable<SimpleTypes::CDecimalNumber>	    m_oMiterLimit;
			nullable<SimpleTypes::CTrueFalse>           m_oOn;
			nullable<std::wstring>						m_sOpacity;
			nullable<std::wstring>						m_sSrc;
			nullable<SimpleTypes::CStrokeArrowType>		m_oStartArrow;
			nullable<SimpleTypes::CStrokeArrowLength>	m_oStartArrowLength;
			nullable<SimpleTypes::CStrokeArrowWidth>	m_oStartArrowWidth;
			nullable<std::wstring>						m_sTitle;
			nullable<std::wstring>						m_sWeight;

		};

		//--------------------------------------------------------------------------------
		// CCallout 14.2.2.2 (Part 4)
		//--------------------------------------------------------------------------------	
		class CCallout : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCallout)
			CCallout(OOX::Document *pMain = NULL);
			virtual ~CCallout();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CTrueFalse>             m_oAccentbar;
			nullable<SimpleTypes::CVmlAngle>	          m_oAngle;
			nullable<SimpleTypes::CCoordinate>            m_oDistance;
			nullable<SimpleTypes::CCalloutDrop>           m_oDrop;
			nullable<SimpleTypes::CTrueFalse>             m_oDropAuto;
			nullable<SimpleTypes::CExt>                   m_oExt;
			nullable<SimpleTypes::CCoordinate>            m_oGap;
			nullable<SimpleTypes::CCoordinate>            m_oLength;
			nullable<SimpleTypes::CTrueFalse>             m_oLengthSpecified;
			nullable<SimpleTypes::CTrueFalse>             m_oMinusX;
			nullable<SimpleTypes::CTrueFalse>             m_oMinusY;
			nullable<SimpleTypes::CTrueFalse>             m_oOn;
			nullable<SimpleTypes::CTrueFalse>             m_oTextBorder;
			nullable<SimpleTypes::Vml::CVmlCalloutType>   m_oType;
		};

		//--------------------------------------------------------------------------------
		// CClipPath 14.2.2.3 (Part 4)
		//--------------------------------------------------------------------------------	
		class CClipPath : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CClipPath)
			CClipPath(OOX::Document *pMain = NULL);
			virtual ~CClipPath();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			SimpleTypes::Vml::CVmlPath m_oV;
		};

		//--------------------------------------------------------------------------------
		// CColorMenu 14.2.2.4 (Part 4)
		//--------------------------------------------------------------------------------	
		class CColorMenu : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CColorMenu)
			CColorMenu();
			virtual ~CColorMenu();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CExt>			m_oExt;
			SimpleTypes::CColorType				m_oExtrusionColor = SimpleTypes::colortypeRGB;
			SimpleTypes::CColorType				m_oFillColor = SimpleTypes::colortypeRGB;
			SimpleTypes::CColorType				m_oShadowColor = SimpleTypes::colortypeRGB;
			SimpleTypes::CColorType				m_oStrokeColor = SimpleTypes::colortypeRGB;
		};

		//--------------------------------------------------------------------------------
		// CColorMru 14.2.2.5 (Part 4)
		//--------------------------------------------------------------------------------	
		class CColorMru : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CColorMru)
			CColorMru(OOX::Document *pMain = NULL);
			virtual ~CColorMru();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			std::vector<SimpleTypes::CColorType*>	m_arrColors;
			nullable<SimpleTypes::CExt>				m_oExt;
		};

		//--------------------------------------------------------------------------------
		// CComplex 14.2.2.7 (Part 4)
		//--------------------------------------------------------------------------------	
		class CComplex : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CComplex)
			CComplex(OOX::Document *pMain = NULL);
			virtual ~CComplex();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CExt> m_oExt;
		};

		//--------------------------------------------------------------------------------
		// CRelation 14.2.2.24 (Part 4)
		//--------------------------------------------------------------------------------	
		class CRelation : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CRelation)
			CRelation();
			virtual ~CRelation();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CExt>			m_oExt;
			nullable<std::wstring>				m_sIdCntr;
			nullable<std::wstring>				m_sIdDest;
			nullable<std::wstring>				m_sIdSrc;
		};

		//--------------------------------------------------------------------------------
		// CRelationTable 14.2.2.25 (Part 4)
		//--------------------------------------------------------------------------------	
		class CRelationTable : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CRelationTable)
			CRelationTable();
			virtual ~CRelationTable();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CExt>					m_oExt;
			std::vector<OOX::VmlOffice::CRelation*>		m_arrRel;
		};

		//--------------------------------------------------------------------------------
		// CDiagram 14.2.2.8 (Part 4)
		//--------------------------------------------------------------------------------	
		class CDiagram : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDiagram)
			CDiagram(OOX::Document *pMain = NULL);
			virtual ~CDiagram();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CTrueFalse>		m_oAutoFormat;
			nullable<SimpleTypes::CTrueFalse>		m_oAutoLayout;
			nullable<std::wstring>					m_sConstrainbounds;
			nullable<SimpleTypes::CDecimalNumber>	m_oDmgBaseTextScale;
			nullable<SimpleTypes::CDecimalNumber>	m_oDmgFontSize;
			nullable<SimpleTypes::CDecimalNumber>	m_oDmgScaleX;
			nullable<SimpleTypes::CDecimalNumber>	m_oDmgScaleY;
			nullable<SimpleTypes::CDecimalNumber>	m_oDmgStyle;
			nullable<SimpleTypes::CExt>				m_oExt;
			nullable<SimpleTypes::CTrueFalse>		m_oReverse;

			nullable<OOX::VmlOffice::CRelationTable>      m_oRelationTable;
		};

		//--------------------------------------------------------------------------------
		// CEntry 14.2.2.9 (Part 4)
		//--------------------------------------------------------------------------------	
		class CEntry : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CEntry)
			CEntry();
			virtual ~CEntry();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CDecimalNumber> m_oNew;
			nullable<SimpleTypes::CDecimalNumber> m_oOld;
		};

		//--------------------------------------------------------------------------------
		// CEquationXml 14.2.2.10 (Part 4)
		//--------------------------------------------------------------------------------	
		class CEquationXml : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CEquationXml)
			CEquationXml(OOX::Document *pMain = NULL);
			virtual ~CEquationXml();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CAlternateMathContentType> m_oContentType;
		};

		//--------------------------------------------------------------------------------
		// CExtrusion 14.2.2.11 (Part 4)
		//--------------------------------------------------------------------------------	
		class CExtrusion : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CExtrusion)
			CExtrusion(OOX::Document *pMain = NULL);
			virtual ~CExtrusion();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			SimpleTypes::CTrueFalse                                          m_oAutoRotationCenter;
			SimpleTypes::CPoint                                              m_oBackDepth;
			SimpleTypes::Vml::CVml_1_65536                                   m_oBrightness;
			nullable<SimpleTypes::CColorType>								 m_oColor;
			SimpleTypes::CColorMode                                          m_oColorMode;
			SimpleTypes::Vml::CVml_1_65536                                   m_oDiffusity;
			SimpleTypes::CPoint                                              m_oEdge;
			nullable<SimpleTypes::CExt>                                      m_oExt;
			SimpleTypes::CDecimalNumber                                      m_oFacet = 30000;
			SimpleTypes::CPoint                                              m_oForeDepth;
			SimpleTypes::CTrueFalse                                          m_oLightFace = SimpleTypes::booleanTrue;
			SimpleTypes::CTrueFalse                                          m_oLightHarsh = SimpleTypes::booleanTrue;
			SimpleTypes::CTrueFalse                                          m_oLightHarsh2;
			SimpleTypes::Vml::CVml_1_65536                                   m_oLightLevel;
			SimpleTypes::Vml::CVml_1_65536                                   m_oLightLevel2;
			SimpleTypes::Vml::CVml_Vector3D_65536                            m_oLightPosition;
			SimpleTypes::Vml::CVml_Vector3D_65536                            m_oLightPosition2;
			SimpleTypes::CTrueFalse                                          m_oLockRotationCenter = SimpleTypes::booleanTrue;
			SimpleTypes::CTrueFalse                                          m_oMetal;
			SimpleTypes::CTrueFalse                                          m_oOn;
			SimpleTypes::Vml::CVml_Vector3D                                  m_oOrientation;
			SimpleTypes::CDecimalNumber                                      m_oOrientationAngle;
			SimpleTypes::CExtrusionPlane                                     m_oPlane;
			SimpleTypes::CExtrusionRender                                    m_oRender;
			SimpleTypes::Vml::CVml_Vector2D                                  m_oRotationAngle;
			SimpleTypes::Vml::CVml_Vector3D                                  m_oRotationCenter;
			SimpleTypes::CDecimalNumber                                      m_oShininess = 5;
			SimpleTypes::CPositiveFixedPercentage                            m_oSkewAmt;
			SimpleTypes::CDecimalNumber                                      m_oSkewAngle = 225;
			SimpleTypes::Vml::CVml_1_65536                                   m_oSpecularity;
			SimpleTypes::CExtrusionType                                      m_oType;
			SimpleTypes::Vml::CVml_Vector3D                                  m_oViewPoint;
			SimpleTypes::Vml::CVml_Vector2D_F                                m_oViewPointOrigin;
		};

		//--------------------------------------------------------------------------------
		// CFieldCodes 14.2.2.12 (Part 4)
		//--------------------------------------------------------------------------------	
		class CFieldCodes : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFieldCodes)
			CFieldCodes();
			virtual ~CFieldCodes();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			// Text
			std::wstring m_sText;
		};

		//--------------------------------------------------------------------------------
		// CFill 14.2.2.13 (Part 4)
		//--------------------------------------------------------------------------------	
		class CFill : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFill)
			CFill(OOX::Document *pMain = NULL);
			virtual ~CFill();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CExt>				m_oExt;
			nullable<SimpleTypes::CFillType1>		m_oType;
		};

		//--------------------------------------------------------------------------------
		// CIdMap 14.2.2.14 (Part 4)
		//--------------------------------------------------------------------------------	
		class CIdMap : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CIdMap)
			CIdMap();
			virtual ~CIdMap();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_string m_sData;
			nullable<SimpleTypes::CExt> m_oExt;
		};

		//--------------------------------------------------------------------------------
		// CInk 14.2.2.15 (Part 4)
		//--------------------------------------------------------------------------------	
		class CInk : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CInk)
			CInk(OOX::Document *pMain = NULL);
			virtual ~CInk();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CTrueFalse>	m_oAnnotation;
			nullable<SimpleTypes::CContentType>	m_oContentType;
			nullable<std::wstring>				m_sI;
		};

		//--------------------------------------------------------------------------------
		// CLinkType 14.2.2.17 (Part 4)
		//--------------------------------------------------------------------------------	
		class CLinkType : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CLinkType)
			CLinkType();
			virtual ~CLinkType();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			// Text
			SimpleTypes::COLELinkType m_oValue;
		};

		//--------------------------------------------------------------------------------
		// CLock 14.2.2.18 (Part 4)
		//--------------------------------------------------------------------------------	
		class CLock : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CLock)
			CLock(OOX::Document *pMain = NULL);
			virtual ~CLock();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			// Attributes
			SimpleTypes::CTrueFalse               m_oAdjustHandles;
			SimpleTypes::CTrueFalse               m_oAspectRatio;
			SimpleTypes::CTrueFalse               m_oCropping;
			nullable<SimpleTypes::CExt>           m_oExt;
			SimpleTypes::CTrueFalse               m_oGrouping;
			SimpleTypes::CTrueFalse               m_oPosition;
			SimpleTypes::CTrueFalse               m_oRotation;
			SimpleTypes::CTrueFalse               m_oSelection;
			SimpleTypes::CTrueFalse               m_oShapeType;
			SimpleTypes::CTrueFalse               m_oText;
			SimpleTypes::CTrueFalse               m_oUnGrouping;
			SimpleTypes::CTrueFalse               m_oVerticies;
		};

		//--------------------------------------------------------------------------------
		// CLockedField 14.2.2.19 (Part 4)
		//--------------------------------------------------------------------------------	
		class CLockedField : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CLockedField)
			CLockedField();
			virtual ~CLockedField();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			SimpleTypes::CTrueFalse m_oValue;
		};

		//--------------------------------------------------------------------------------
		// COLEObject 14.2.2.20 (Part 4)
		//--------------------------------------------------------------------------------	
		class COLEObject : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(COLEObject)
			COLEObject(OOX::Document *pMain = NULL);
			virtual ~COLEObject();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::COLEDrawAspect>	m_oDrawAspect;
			nullable<SimpleTypes::CRelationshipId>	m_oId;
			nullable_string							m_sObjectId;
			nullable_string							m_sProgId;
			nullable_string							m_sShapeId;
			nullable<SimpleTypes::COLEType>			m_oType;
			nullable<SimpleTypes::COLEUpdateMode>	m_oUpdateMode;

			nullable_string							m_oFieldCodes2;
			nullable<OOX::VmlOffice::CFieldCodes>	m_oFieldCodes;

			nullable<OOX::VmlOffice::CLinkType>		m_oLinkType;
			nullable<OOX::VmlOffice::CLockedField>	m_oLockedField;
		};

		//--------------------------------------------------------------------------------
		// CProxy 14.2.2.21 (Part 4)
		//--------------------------------------------------------------------------------	
		class CProxy : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CProxy)
			CProxy();
			virtual ~CProxy();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			SimpleTypes::CDecimalNumber							m_oConnectLoc;
			SimpleTypes::CTrueFalse								m_oEnd;
			std::wstring										m_sIdRef;
			SimpleTypes::CTrueFalse								m_oStart;
		};

		//--------------------------------------------------------------------------------
		// CR 14.2.2.22 (Part 4)
		//--------------------------------------------------------------------------------	
		class CR : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CR)
			CR();
			virtual ~CR();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			// Attributes
			nullable<SimpleTypes::CHow>		m_oHow;
			std::wstring					m_sId;
			nullable<std::wstring>			m_sIdRef;
			nullable<SimpleTypes::CRType>	m_oType;
			
			// Childs
			std::vector<OOX::VmlOffice::CProxy*>	m_arrProxy;
		};

		//--------------------------------------------------------------------------------
		// CRegroupTable 14.2.2.23 (Part 4)
		//--------------------------------------------------------------------------------	
		class CRegroupTable : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CRegroupTable)
			CRegroupTable();
			virtual ~CRegroupTable();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			// Attributes
			nullable<SimpleTypes::CExt>				m_oExt;
			
			// Childs
			std::vector<OOX::VmlOffice::CEntry*>	m_arrEntry;
		};

		//--------------------------------------------------------------------------------
		// CRules 14.2.2.27 (Part 4)
		//--------------------------------------------------------------------------------	
		class CRules : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CRules)
			CRules();
			virtual ~CRules();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			// Attributes
			nullable<SimpleTypes::CExt>			m_oExt;
			
			// Childs
			std::vector<OOX::VmlOffice::CR*>	m_arrR;
		};

		//--------------------------------------------------------------------------------
		// CShapeLayout 14.2.2.29 (Part 4)
		//--------------------------------------------------------------------------------	
		class CShapeLayout : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CShapeLayout)
			CShapeLayout(OOX::Document *pMain = NULL);
			virtual ~CShapeLayout();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			// Attributes
			nullable<SimpleTypes::CExt>				m_oExt;
			
			// Childs
			nullable<OOX::VmlOffice::CIdMap>		m_oIdMap;
			nullable<OOX::VmlOffice::CRegroupTable>	m_oRegroupTable;
			nullable<OOX::VmlOffice::CRules>		m_oRules;
		};

		//--------------------------------------------------------------------------------
		// CSignatureLine 14.2.2.30 (Part 4)
		//--------------------------------------------------------------------------------	
        class CSignatureLine : public PPTX::WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSignatureLine)
			CSignatureLine(OOX::Document *pMain = NULL);
			virtual ~CSignatureLine();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			// Attributes
			nullable<std::wstring>				m_sAddXml;
			nullable<SimpleTypes::COnOff>		m_oAllowComments;
			nullable<SimpleTypes::CExt>			m_oExt;
			nullable<SimpleTypes::CGuid>		m_oId;
			nullable<SimpleTypes::COnOff>		m_oIsSignatureLine;
			nullable<SimpleTypes::CGuid>		m_oProvId;
			nullable<SimpleTypes::COnOff>		m_oShowSignDate;
			nullable<std::wstring>				m_sSigningInstructions;
			nullable<SimpleTypes::COnOff>		m_oSigningInstructionsSet;
			nullable<std::wstring>				m_sSigProvUrl;
			nullable<std::wstring>				m_sSuggestedSigner;
			nullable<std::wstring>				m_sSuggestedSigner2;
			nullable<std::wstring>				m_sSuggestedSignerEmail;
		};

		//--------------------------------------------------------------------------------
		// CSkew 14.2.2.31 (Part 4)
		//--------------------------------------------------------------------------------	
		class CSkew : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSkew)
			CSkew(OOX::Document *pMain = NULL);
			virtual ~CSkew();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			// Attributes
			nullable<SimpleTypes::CExt>							m_oExt;
			nullable<std::wstring>								m_sId;
			nullable<std::wstring>								m_sMatrix;
			nullable<std::wstring>								m_sOffset;
			SimpleTypes::CTrueFalse								m_oOn;
			nullable<std::wstring>								m_sOrigin;
		};

	} // namespace Vml
} // namespace OOX

#endif // OOX_VML_OFFICE_INCLUDE_H_
