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
#pragma once

#include "../../Base/Nullable.h"
#include "../../Common/SimpleTypes_Drawing.h"
#include "../../Common/SimpleTypes_Shared.h"

#include "../WritingElement.h"

namespace OOX
{
	namespace Drawing
	{
		enum EGeomType
		{
			geomtypeUnknown = 0,
			geomtypeCustom  = 1,
			geomtypePreset  = 2,
		};
		//--------------------------------------------------------------------------------
		// CAdjPoint2D 20.1.9.17;20.1.9.20 (Part 1)
		//--------------------------------------------------------------------------------	
		class CAdjPoint2D : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CAdjPoint2D)
			CAdjPoint2D()
			{
				m_eType = et_Unknown;
			}
			virtual ~CAdjPoint2D()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				m_eType = et_Unknown;
				// TO DO: Реализовать CAdjPoint2D::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_eType = et_Unknown;
                CString sName = oReader.GetName();

				if ( _T("a:pos") == sName )
					m_eType = et_a_pos;
				else if ( _T("a:pt") == sName )
					m_eType = et_a_pt;
				else
					return;

				ReadAttributes( oReader ); 

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString      toXML() const
			{
				CString sResult;
				if ( et_a_pos == m_eType )
					sResult = _T("<a:pos x=\"") + m_oX.ToString() + _T("\" y=\"") + m_oY.ToString() + _T("\"/>");
				if ( et_a_pt == m_eType )
					sResult = _T("<a:pt x=\"") + m_oX.ToString() + _T("\" y=\"") + m_oY.ToString() + _T("\"/>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return m_eType;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("x"), m_oX )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("y"), m_oY )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			EElementType m_eType;

			// Attributes
			SimpleTypes::CAdjCoordinate<> m_oX;
			SimpleTypes::CAdjCoordinate<> m_oY;

		};
		//--------------------------------------------------------------------------------
		// CPolarAdjustHandle 20.1.9.2 (Part 1)
		//--------------------------------------------------------------------------------	
		class CPolarAdjustHandle : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPolarAdjustHandle)
			CPolarAdjustHandle()
			{
			}
			virtual ~CPolarAdjustHandle()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CPolarAdjustHandle::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader ); 

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();
					if ( _T("a:pos") == sName )
						m_oPos = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:ahPolar ");

                if ( m_oGdRefR.IsInit() )
				{
					sResult += _T("gdRefR=\"");
					sResult += m_oGdRefR->ToString();
					sResult += _T("\" ");
				}

				if ( m_oMinR.IsInit() )
				{
					sResult += _T("minR=\"");
					sResult += m_oMinR->ToString();
					sResult += _T("\" ");
				}

				if ( m_oMaxR.IsInit() )
				{
					sResult += _T("maxR=\"");
					sResult += m_oMaxR->ToString();
					sResult += _T("\" ");
				}

				if ( m_oGdRefAng.IsInit() )
				{
					sResult += _T("gdRefAng=\"");
					sResult += m_oGdRefAng->ToString();
					sResult += _T("\" ");
				}

				if ( m_oMinAng.IsInit() )
				{
					sResult += _T("minAng=\"");
					sResult += m_oMinAng->ToString();
					sResult += _T("\" ");
				}

				if ( m_oMaxAng.IsInit() )
				{
					sResult += _T("maxAng=\"");
					sResult += m_oMaxAng->ToString();
					sResult += _T("\" ");
				}

				sResult += _T(">");

				sResult += m_oPos.toXML();

				sResult += _T("</a:ahPolar>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_ahPolar;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("gdRefAng"), m_oGdRefAng )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("gdRefR"),   m_oGdRefR )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("maxAng"),   m_oMaxAng )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("maxR"),     m_oMaxR )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("minAng"),   m_oMinAng )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("minR"),     m_oMinR )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes
			nullable<SimpleTypes::CGeomGuideName>   m_oGdRefAng;
			nullable<SimpleTypes::CGeomGuideName>   m_oGdRefR;
			nullable<SimpleTypes::CAdjAngle<>>      m_oMaxAng;
			nullable<SimpleTypes::CAdjCoordinate<>> m_oMaxR;
			nullable<SimpleTypes::CAdjAngle<>>      m_oMinAng;
			nullable<SimpleTypes::CAdjCoordinate<>> m_oMinR;

			// Childs
			OOX::Drawing::CAdjPoint2D               m_oPos;

		};
		//--------------------------------------------------------------------------------
		// CXYAdjustHandle 20.1.9.3 (Part 1)
		//--------------------------------------------------------------------------------	
		class CXYAdjustHandle : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CXYAdjustHandle)
			CXYAdjustHandle()
			{
			}
			virtual ~CXYAdjustHandle()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CXYAdjustHandle::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader ); 

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();
					if ( _T("a:pos") == sName )
						m_oPos = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:ahXY ");

				if ( m_oGdRefX.IsInit() )
				{
					sResult += _T("gdRefX=\"");
					sResult += m_oGdRefX->ToString();
					sResult += _T("\" ");
				}

				if ( m_oMinX.IsInit() )
				{
					sResult += _T("minX=\"");
					sResult += m_oMinX->ToString();
					sResult += _T("\" ");
				}

				if ( m_oMaxX.IsInit() )
				{
					sResult += _T("maxX=\"");
					sResult += m_oMaxX->ToString();
					sResult += _T("\" ");
				}

				if ( m_oGdRefY.IsInit() )
				{
					sResult += _T("gdRefY=\"");
					sResult += m_oGdRefY->ToString();
					sResult += _T("\" ");
				}

				if ( m_oMinY.IsInit() )
				{
					sResult += _T("minY=\"");
					sResult += m_oMinY->ToString();
					sResult += _T("\" ");
				}

				if ( m_oMaxY.IsInit() )
				{
					sResult += _T("maxY=\"");
					sResult += m_oMaxY->ToString();
					sResult += _T("\" ");
				}

				sResult += _T(">");

				sResult += m_oPos.toXML();

				sResult += _T("</a:ahXY>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_ahXY;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("gdRefX"), m_oGdRefX )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("gdRefY"), m_oGdRefY )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("maxX"),   m_oMaxX )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("maxY"),   m_oMaxY )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("minX"),   m_oMinX )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("minY"),   m_oMinY )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes
			nullable<SimpleTypes::CGeomGuideName>   m_oGdRefX;
			nullable<SimpleTypes::CGeomGuideName>   m_oGdRefY;
			nullable<SimpleTypes::CAdjCoordinate<>> m_oMaxX;
			nullable<SimpleTypes::CAdjCoordinate<>> m_oMaxY;
			nullable<SimpleTypes::CAdjCoordinate<>> m_oMinX;
			nullable<SimpleTypes::CAdjCoordinate<>> m_oMinY;

			// Childs
			OOX::Drawing::CAdjPoint2D               m_oPos;

		};
		//--------------------------------------------------------------------------------
		// CPath2DArcTo 20.1.9.4 (Part 1)
		//--------------------------------------------------------------------------------	
		class CPath2DArcTo : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPath2DArcTo)
			CPath2DArcTo()
			{
			}
			virtual ~CPath2DArcTo()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CPath2DArcTo::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader ); 

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:arcTo wR=\"")    + m_oWr.ToString()
					                  + _T("\" hR=\"")    + m_oHr.ToString()
					                  + _T("\" stAng=\"") + m_oStAng.ToString() 
						              + _T("\" swAng=\"") + m_oSwAng.ToString()
						              + _T("\"/>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_arcTo;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("hR"),    m_oHr )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("stAng"), m_oStAng )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("swAng"), m_oSwAng )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("wR"),    m_oWr )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes
			SimpleTypes::CAdjCoordinate<> m_oHr;
			SimpleTypes::CAdjAngle<>      m_oStAng;
			SimpleTypes::CAdjAngle<>      m_oSwAng;
			SimpleTypes::CAdjCoordinate<> m_oWr;
		};
		//--------------------------------------------------------------------------------
		// CPath2DClose 20.1.9.6 (Part 1)
		//--------------------------------------------------------------------------------	
		class CPath2DClose : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPath2DClose)
			CPath2DClose()
			{
			}
			virtual ~CPath2DClose()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CPath2DClose::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString      toXML() const
			{
				return _T("<a:close/>");;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_close;
			}
		};
		//--------------------------------------------------------------------------------
		// CPath2DCubicBezierTo 20.1.9.7 (Part 1)
		//--------------------------------------------------------------------------------	
		class CPath2DCubicBezierTo : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPath2DCubicBezierTo)
			CPath2DCubicBezierTo()
			{
			}
			virtual ~CPath2DCubicBezierTo()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CPath2DCubicBezierTo::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nPtCount = 0;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();

					if ( 0 == nPtCount && _T("a:pt") == sName )
					{
						m_oCtrl1 = oReader;
						nPtCount++;
					}
					else if ( 1 == nPtCount && _T("a:pt") == sName )
					{
						m_oCtrl2 = oReader;
						nPtCount++;
					}
					else if ( 2 == nPtCount && _T("a:pt") == sName )
					{
						m_oEnd = oReader;
						nPtCount++;
					}
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:cubicBezTo>");

				sResult += m_oCtrl1.toXML();
				sResult += m_oCtrl2.toXML();
				sResult += m_oEnd.toXML();

				sResult += _T("</a:cubicBezTo>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_cubicBezTo;
			}

		public:

			OOX::Drawing::CAdjPoint2D m_oCtrl1;
			OOX::Drawing::CAdjPoint2D m_oCtrl2;
			OOX::Drawing::CAdjPoint2D m_oEnd;

		};
		//--------------------------------------------------------------------------------
		// CConnectionSite 20.1.9.9 (Part 1)
		//--------------------------------------------------------------------------------	
		class CConnectionSite : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CConnectionSite)
			CConnectionSite()
			{
			}
			virtual ~CConnectionSite()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CConnectionSite::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();

					if ( _T("a:pos") == sName )
						m_oPos = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:cxn ang=\"") + m_oAng.ToString() + _T("\">");
				sResult += m_oPos.toXML();
				sResult += _T("</a:cxn>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_cxn;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("ang"), m_oAng )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:

			// Attributes
			SimpleTypes::CAdjAngle<>  m_oAng;

			// Childs
			OOX::Drawing::CAdjPoint2D m_oPos;
		};
		//--------------------------------------------------------------------------------
		// CConnectionSiteList 20.1.9.10 (Part 1)
		//--------------------------------------------------------------------------------	
		class CConnectionSiteList : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CConnectionSiteList)
			CConnectionSiteList()
			{
			}
			virtual ~CConnectionSiteList()
			{
				for ( unsigned int nIndex = 0; nIndex < m_arrCxn.size(); nIndex++ )
				{
					if ( m_arrCxn[nIndex] ) delete m_arrCxn[nIndex];
					m_arrCxn[nIndex] = NULL;
				}
				m_arrCxn.clear();
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CConnectionSiteList::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();

					if ( _T("a:cxn") == sName )
					{
						OOX::Drawing::CConnectionSite *oCxn = new OOX::Drawing::CConnectionSite(oReader);
                        if (oCxn) m_arrCxn.push_back( oCxn );
					}
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:cxnLst>");

                for ( unsigned int nIndex = 0; nIndex < m_arrCxn.size(); nIndex++ )
				{
					if (m_arrCxn[nIndex])
						sResult += m_arrCxn[nIndex]->toXML();
				}

				sResult += _T("</a:cxnLst>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_cxnLst;
			}

		public:

			// Childs
            std::vector<OOX::Drawing::CConnectionSite*> m_arrCxn;
		};
		//--------------------------------------------------------------------------------
		// CGeomGuide 20.1.9.11 (Part 1)
		//--------------------------------------------------------------------------------	
		class CGeomGuide : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CGeomGuide)
			CGeomGuide()
			{
			}
			virtual ~CGeomGuide()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CGeomGuide::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader ); 

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:gd name=\"") + m_oName.ToString() + _T("\" fmla=\"") + m_oFmla.ToString() + _T("\"/>");
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_gd;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("fmla"), m_oFmla )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("name"), m_oName )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes
			SimpleTypes::CGeomGuideFormula m_oFmla;
			SimpleTypes::CGeomGuideName    m_oName;

		};
		//--------------------------------------------------------------------------------
		// CGeomGuideList 20.1.9.5;20.1.9.12 (Part 1)
		//--------------------------------------------------------------------------------	
		class CGeomGuideList : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CGeomGuideList)
			CGeomGuideList()
			{
				m_eType = et_Unknown;
			}
			virtual ~CGeomGuideList()
			{
				for ( unsigned int nIndex = 0; nIndex < m_arrGd.size(); nIndex++ )
				{
					if ( m_arrGd[nIndex] ) delete m_arrGd[nIndex];
					m_arrGd[nIndex] = NULL;
				}
				m_arrGd.clear();
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				m_eType = et_Unknown;
				// TO DO: Реализовать CGeomGuideList::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_eType = et_Unknown;
                CString sName = oReader.GetName();

				if ( _T("a:avLst") == sName )
					m_eType = et_a_avLst;
				else if ( _T("a:gdLst") == sName )
					m_eType = et_a_gdLst;
				else
					return;

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					sName = oReader.GetName();

					if ( _T("a:gd") == sName )
					{
						OOX::Drawing::CGeomGuide *oGd = new OOX::Drawing::CGeomGuide(oReader);
                        if (oGd) m_arrGd.push_back( oGd );
					}
				}
			}
			virtual CString      toXML() const
			{
				CString sResult;
				
				if ( et_a_avLst == m_eType )
					sResult = _T("<a:avLst>");
				else if ( et_a_gdLst == m_eType )
					sResult = _T("<a:gdLst>");
				else 
					return _T("");
									

                for ( unsigned int nIndex = 0; nIndex < m_arrGd.size(); nIndex++ )
				{
					if (m_arrGd[nIndex])
						sResult += m_arrGd[nIndex]->toXML();
				}

				if ( et_a_avLst == m_eType )
					sResult += _T("</a:avLst>");
				else if ( et_a_gdLst == m_eType )
					sResult += _T("</a:gdLst>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return m_eType;
			}

		public:

			EElementType                           m_eType;

			// Childs
            std::vector<OOX::Drawing::CGeomGuide*> m_arrGd;
		};
		//--------------------------------------------------------------------------------
		// CPath2DLineTo 20.1.9.13 (Part 1)
		//--------------------------------------------------------------------------------	
		class CPath2DLineTo : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPath2DLineTo)
			CPath2DLineTo()
			{
			}
			virtual ~CPath2DLineTo()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CPath2DLineTo::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();

					if ( _T("a:pt") == sName )
						m_oPt = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:lnTo>");
				sResult += m_oPt.toXML();
				sResult += _T("</a:lnTo>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_lnTo;
			}

		public:

			OOX::Drawing::CAdjPoint2D m_oPt;

		};
		//--------------------------------------------------------------------------------
		// CPath2DMoveTo 20.1.9.14 (Part 1)
		//--------------------------------------------------------------------------------	
		class CPath2DMoveTo : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPath2DMoveTo)
			CPath2DMoveTo()
			{
			}
			virtual ~CPath2DMoveTo()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CPath2DMoveTo::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();

					if ( _T("a:pt") == sName )
						m_oPt = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:moveTo>");
				sResult += m_oPt.toXML();
				sResult += _T("</a:moveTo>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_moveTo;
			}

		public:

			OOX::Drawing::CAdjPoint2D m_oPt;

		};
		//--------------------------------------------------------------------------------
		// CPath2DQuadBezierTo 20.1.9.21 (Part 1)
		//--------------------------------------------------------------------------------	
		class CPath2DQuadBezierTo : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPath2DQuadBezierTo)
			CPath2DQuadBezierTo()
			{
			}
			virtual ~CPath2DQuadBezierTo()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CPath2DQuadBezierTo::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nPtCount = 0;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();

					if ( 0 == nPtCount && _T("a:pt") == sName )
					{
						m_oCtrl = oReader;
						nPtCount++;
					}
					else if ( 1 == nPtCount && _T("a:pt") == sName )
					{
						m_oEnd = oReader;
						nPtCount++;
					}
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:quadBezTo>");

				sResult += m_oCtrl.toXML();
				sResult += m_oEnd.toXML();

				sResult += _T("</a:quadBezTo>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_quadBezTo;
			}

		public:

			OOX::Drawing::CAdjPoint2D m_oCtrl;
			OOX::Drawing::CAdjPoint2D m_oEnd;

		};
		//--------------------------------------------------------------------------------
		// CPath2D 20.1.9.15 (Part 1)
		//--------------------------------------------------------------------------------	
		class CPath2D : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPath2D)
			CPath2D()
			{
			}
			CPath2D(const CPath2D& oOther)
			{
				m_oExtrusionOk = oOther.m_oExtrusionOk;
				m_oFill        = oOther.m_oFill;
				m_oH           = oOther.m_oH;
				m_oStroke      = oOther.m_oStroke;
				m_oW           = oOther.m_oW;

                for ( unsigned int nIndex = 0; nIndex < oOther.m_arrItems.size(); nIndex++ )
				{
					OOX::EElementType eType = oOther.m_arrItems[nIndex]->getType();

					WritingElement *pItem = NULL;
					switch ( eType )
					{
					case et_a_arcTo:      pItem = new CPath2DArcTo        ( (const CPath2DArcTo&) *oOther.m_arrItems[nIndex] ); break;
					case et_a_close:      pItem = new CPath2DClose        ( (const CPath2DClose&) *oOther.m_arrItems[nIndex] ); break;
					case et_a_cubicBezTo: pItem = new CPath2DCubicBezierTo( (const CPath2DCubicBezierTo&) *oOther.m_arrItems[nIndex] ); break;
					case et_a_lnTo:       pItem = new CPath2DLineTo       ( (const CPath2DLineTo&) *oOther.m_arrItems[nIndex] ); break;
					case et_a_moveTo:     pItem = new CPath2DMoveTo       ( (const CPath2DMoveTo&) *oOther.m_arrItems[nIndex] ); break;
					case et_a_quadBezTo:  pItem = new CPath2DQuadBezierTo ( (const CPath2DQuadBezierTo&) *oOther.m_arrItems[nIndex] ); break;
					}

					if ( NULL != pItem )
                        m_arrItems.push_back ( pItem );
				}
			}
			virtual ~CPath2D()
			{
				Clear();
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CPath2D::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader ); 

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();
					WritingElement* pItem = NULL;

					if ( _T("a:arcTo") == sName )
						pItem = new CPath2DArcTo( oReader );
					else if ( _T("a:close") == sName )
						pItem = new CPath2DClose( oReader );
					else if ( _T("a:cubicBezTo") == sName )
						pItem = new CPath2DCubicBezierTo( oReader );
					else if ( _T("a:lnTo") == sName )
						pItem = new CPath2DLineTo( oReader );
					else if ( _T("a:moveTo") == sName )
						pItem = new CPath2DMoveTo( oReader );
					else if ( _T("a:quadBezTo") == sName )
						pItem = new CPath2DQuadBezierTo( oReader );

					if ( pItem )
                        m_arrItems.push_back( pItem );
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:path w=\"") + m_oW.ToString() 
					                 + _T("\" h=\"") + m_oH.ToString()
									 + _T("\" fill=\"") + m_oFill.ToString() 
									 + _T("\" stroke=\"") + m_oStroke.ToString() 
									 + _T("\" extrusionOk=\"") + m_oExtrusionOk.ToString() 
									 + _T("\">");

                for ( unsigned int nIndex = 0; nIndex < m_arrItems.size(); nIndex++ )
				{
					if ( m_arrItems[nIndex] )
						sResult += m_arrItems[nIndex]->toXML();
				}

				sResult += _T("</a:path>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_path;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("extrusionOk"), m_oExtrusionOk )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("fill"),        m_oFill )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("h"),           m_oH )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("stroke"),      m_oStroke )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w"),           m_oW )
				WritingElement_ReadAttributes_End( oReader )
			}


		public:

			void Clear()
			{
                for ( unsigned int nIndex = 0; nIndex < m_arrItems.size(); nIndex++ )
				{
					if ( m_arrItems[nIndex] )
						delete m_arrItems[nIndex];

					m_arrItems[nIndex] = NULL;
				}

                m_arrItems.clear();
			}

		public:

			// Attributes
			SimpleTypes::COnOff<SimpleTypes::onoffTrue>               m_oExtrusionOk;
			SimpleTypes::CPathFillMode<SimpleTypes::pathfillmodeNorm> m_oFill;
			SimpleTypes::CPositiveCoordinate<0>                       m_oH;
			SimpleTypes::COnOff<SimpleTypes::onoffTrue>               m_oStroke;
			SimpleTypes::CPositiveCoordinate<0>                       m_oW;

			// Childs
            std::vector<WritingElement*>                             m_arrItems;

		};
		//--------------------------------------------------------------------------------
		// CPath2DList 20.1.9.16 (Part 1)
		//--------------------------------------------------------------------------------	
		class CPath2DList : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPath2DList)
			CPath2DList()
			{
			}
			virtual ~CPath2DList()
			{
				for ( unsigned int nIndex = 0; nIndex < m_arrPath.size(); nIndex++ )
				{
					if ( m_arrPath[nIndex] ) delete m_arrPath[nIndex];
					m_arrPath[nIndex] = NULL;
				}
				m_arrPath.clear();
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CPath2DList::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();

					if ( _T("a:path") == sName )
					{
						OOX::Drawing::CPath2D *oPath = new OOX::Drawing::CPath2D(oReader);
						if (oPath) m_arrPath.push_back( oPath );
					}
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:pathLst>");

				for ( unsigned int nIndex = 0; nIndex < m_arrPath.size(); nIndex++ )
				{
					if (m_arrPath[nIndex])
						sResult += m_arrPath[nIndex]->toXML();
				}

				sResult += _T("</a:pathLst>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_pathLst;
			}

		public:

			// Childs
            std::vector<OOX::Drawing::CPath2D*> m_arrPath;
		};
		//--------------------------------------------------------------------------------
		// CAdjustHandleList 20.1.9.1 (Part 1)
		//--------------------------------------------------------------------------------	
		class CAdjustHandleList : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CAdjustHandleList)
			CAdjustHandleList()
			{
			}
			CAdjustHandleList(const CAdjustHandleList& oOther)
			{
                for ( unsigned int nIndex = 0; nIndex < oOther.m_arrItems.size(); nIndex++ )
				{
					OOX::EElementType eType = oOther.m_arrItems[nIndex]->getType();

					WritingElement *pItem = NULL;
					switch ( eType )
					{
					case et_a_ahPolar: pItem = new CPolarAdjustHandle( (const CPolarAdjustHandle&) *oOther.m_arrItems[nIndex] ); break;
					case et_a_ahXY:    pItem = new CXYAdjustHandle   ( (const CXYAdjustHandle&) *oOther.m_arrItems[nIndex] ); break;
					}

					if ( NULL != pItem )
                        m_arrItems.push_back( pItem );
				}

			}
			virtual ~CAdjustHandleList()
			{
				Clear();
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CAdjustHandleList::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();

					WritingElement* pElement = NULL;

					if ( _T("a:ahPolar") == sName )
						pElement = new CPolarAdjustHandle( oReader );
					else if ( _T("a:ahXY") == sName )  
						pElement = new CXYAdjustHandle( oReader );

					if ( pElement )
                        m_arrItems.push_back( pElement );
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:ahLst>");

                for ( unsigned int nIndex = 0; nIndex < m_arrItems.size (); nIndex++ )
				{
					if ( m_arrItems[nIndex] )
						sResult += m_arrItems[nIndex]->toXML();
				}

				sResult += _T("</a:ahLst>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_ahLst;
			}


		public:

			void Clear()
			{
                for ( unsigned int nIndex = 0; nIndex < m_arrItems.size(); nIndex++ )
				{
					if ( m_arrItems[nIndex] )
						delete m_arrItems[nIndex];

					m_arrItems[nIndex] = NULL;
				}

				m_arrItems.clear();
			}

		public:

			// Childs
            std::vector<WritingElement*> m_arrItems;
		};
		//--------------------------------------------------------------------------------
		// CPresetGeometry2D 20.1.9.18 (Part 1)
		//--------------------------------------------------------------------------------	
		class CPresetGeometry2D : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPresetGeometry2D)
			CPresetGeometry2D()
			{
			}
			virtual ~CPresetGeometry2D()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CPresetGeometry2D::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader ); 

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();
					WritingElement* pItem = NULL;

					if ( _T("a:avLst") == sName )
						m_oAvLst = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:prstGeom prst=\"") + m_oPrst.ToString() + _T("\">");

				if ( m_oAvLst.IsInit() )
					sResult += m_oAvLst->toXML();

				sResult += _T("</a:prstGeom>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_prstGeom;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("prst"), m_oPrst )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes
			SimpleTypes::CShapeType<>              m_oPrst;

			// Childs
			nullable<OOX::Drawing::CGeomGuideList> m_oAvLst;

		};
		//--------------------------------------------------------------------------------
		// CPresetTextShape 20.1.9.19 (Part 1)
		//--------------------------------------------------------------------------------	
		class CPresetTextShape : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPresetTextShape)
			CPresetTextShape()
			{
			}
			virtual ~CPresetTextShape()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CPresetTextShape::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader ); 

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();
					WritingElement* pItem = NULL;

					if ( _T("a:avLst") == sName )
						m_oAvLst = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:prstTxWarp prst=\"") + m_oPrst.ToString() + _T("\">");

				if ( m_oAvLst.IsInit() )
					sResult += m_oAvLst->toXML();

				sResult += _T("</a:prstTxWarp>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_prstTxWarp;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("prst"), m_oPrst )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes
			SimpleTypes::CTextShapeType<>          m_oPrst;

			// Childs
			nullable<OOX::Drawing::CGeomGuideList> m_oAvLst;

		};
		//--------------------------------------------------------------------------------
		// CGeomRect 20.1.9.22 (Part 1)
		//--------------------------------------------------------------------------------	
		class CGeomRect : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CGeomRect)
			CGeomRect()
			{
			}
			virtual ~CGeomRect()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CGeomRect::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader ); 

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:rect l=\"") + m_oL.ToString() 
					                 + _T("\" t=\"") + m_oT.ToString() 
					                + _T("\" r=\"") + m_oR.ToString() 
					                + _T("\" b=\"") + m_oB.ToString() 
					                + _T("\"/>");
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_rect;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("b"), m_oB )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("l"), m_oL )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("r"), m_oR )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("t"), m_oT )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes
			SimpleTypes::CAdjCoordinate<> m_oB;
			SimpleTypes::CAdjCoordinate<> m_oL;
			SimpleTypes::CAdjCoordinate<> m_oR;
			SimpleTypes::CAdjCoordinate<> m_oT;

		};
		//--------------------------------------------------------------------------------
		// CCustomGeometry2D 20.1.9.8 (Part 1)
		//--------------------------------------------------------------------------------	
		class CCustomGeometry2D : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCustomGeometry2D)
			CCustomGeometry2D()
			{
			}
			virtual ~CCustomGeometry2D()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CCustomGeometry2D::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();

					if ( _T("a:ahLst") == sName )
						m_oAhLst = oReader;
					else if ( _T("a:avLst") == sName )  
						m_oAvLst = oReader;
					else if ( _T("a:cxnLst") == sName )  
						m_oCnxLst = oReader;
					else if ( _T("a:gdLst") == sName )  
						m_oGdLst = oReader;
					else if ( _T("a:pathLst") == sName )  
						m_oPthLst = oReader;
					else if ( _T("a:rect") == sName )  
						m_oRect = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:custGeom>");

				if ( m_oAvLst.IsInit() )
					sResult += m_oAvLst->toXML();

				if ( m_oGdLst.IsInit() )
					sResult += m_oGdLst->toXML();

				if ( m_oAhLst.IsInit() )
					sResult += m_oAhLst->toXML();

				if ( m_oCnxLst.IsInit() )
					sResult += m_oCnxLst->toXML();

				if ( m_oRect.IsInit() )
					sResult += m_oRect->toXML();

				sResult += m_oPthLst.toXML();

				sResult += _T("</a:custGeom>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_custGeom;
			}

		public:

			// Childs
			nullable<OOX::Drawing::CAdjustHandleList  > m_oAhLst;
			nullable<OOX::Drawing::CGeomGuideList     > m_oAvLst;
			nullable<OOX::Drawing::CConnectionSiteList> m_oCnxLst;
			nullable<OOX::Drawing::CGeomGuideList     > m_oGdLst;
			nullable<OOX::Drawing::CGeomRect          > m_oRect;
			OOX::Drawing::CPath2DList                   m_oPthLst;
		};

	} // namespace Drawing
} // namespace OOX

