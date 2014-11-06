#pragma once
#ifndef OOX_LOGIC_DRAWING_INCLUDE_H_
#define OOX_LOGIC_DRAWING_INCLUDE_H_

#include "../../Base/Nullable.h"
#include "../../Common/SimpleTypes_Drawing.h"
#include "../../Common/SimpleTypes_Shared.h"
#include "../../Common/Encoding.h"

#include "../WritingElement.h"
#include "../RId.h"

#include "DrawingExt.h"
#include "DrawingColors.h"
#include "DrawingEffects.h"
#include "DrawingCoreInfo.h"
#include "DrawingGraphic.h"

#include "../../SystemUtility/SystemUtility.h"

namespace OOX
{
	namespace Drawing
	{
		//--------------------------------------------------------------------------------
		// CNonVisualGraphicFrameProperties 20.4.2.4 (Part 1)
		//--------------------------------------------------------------------------------	
		class CNonVisualGraphicFrameProperties : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CNonVisualGraphicFrameProperties)
			CNonVisualGraphicFrameProperties()
			{
			}
			virtual ~CNonVisualGraphicFrameProperties()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();
					if ( _T("a:extLst") == sName )
						m_oExtLst = oReader;
					else if ( _T("a:graphicFrameLocks") == sName )
						m_oGraphicFrameLocks = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<wp:cNvGraphicFramePr>");

				if ( m_oExtLst.IsInit() )
					sResult += m_oExtLst->toXML();
				if ( m_oGraphicFrameLocks.IsInit() )
					sResult += m_oGraphicFrameLocks->toXML();

				sResult += _T("</wp:cNvGraphicFramePr>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_wp_cNvGraphicFramePr;
			}

		public:

			// Childs
			nullable<OOX::Drawing::COfficeArtExtensionList     > m_oExtLst;
			nullable<OOX::Drawing::CGraphicalObjectFrameLocking> m_oGraphicFrameLocks;
		};
		//-----------------------------------------------------------------------
		// EffectExtent 20.4.2.6
		//-----------------------------------------------------------------------
		class CEffectExtent : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CEffectExtent)
			CEffectExtent()
			{
			}
			virtual ~CEffectExtent()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				oNode.ReadAttributeBase( _T("l"), m_oL );
				oNode.ReadAttributeBase( _T("t"), m_oT );
				oNode.ReadAttributeBase( _T("r"), m_oR );
				oNode.ReadAttributeBase( _T("b"), m_oB );
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<wp:effectExtent ");

				sResult += _T("l=\"") + m_oL.ToString() + _T("\" ");
				sResult += _T("t=\"") + m_oT.ToString() + _T("\" ");
				sResult += _T("r=\"") + m_oR.ToString() + _T("\" ");
				sResult += _T("b=\"") + m_oB.ToString() + _T("\" ");

				sResult += _T("</wp:effectExtent>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_wp_effectExtent;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("l"), m_oL )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("t"), m_oT )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("r"), m_oR )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("b"), m_oB )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

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
			CPosH()
			{
			}
			virtual ~CPosH()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();
					if ( _T("wp:align") == sName )
					{
						m_oAlign = oReader.GetText2().GetString();
						m_bAlign = true;
					}
					else if ( _T("wp:posOffset") == sName )
					{
						m_oPosOffset = oReader.GetText2().GetString();
						m_bAlign = false;
					}
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<wp:positionH ");

				if ( m_oRelativeFrom.IsInit() )
					sResult += _T("relativeFrom=\"") + m_oRelativeFrom->ToString() + _T("\">");
				else
					sResult += _T(">");

				if ( m_bAlign && m_oAlign.IsInit() )
					sResult += _T("<wp:align>") + m_oAlign->ToString() + _T("</wp:align>");
				else if ( !m_bAlign && m_oPosOffset.IsInit() )
					sResult += _T("<wp:posOffset>") + m_oPosOffset->ToString() + _T("</wp:posOffset>");

				sResult += _T("</wp:positionH>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_wp_positionH;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("relativeFrom"), m_oRelativeFrom )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			bool IsAlign() const
			{
				return m_bAlign;
			}
			bool IsPosOffset() const
			{
				return !m_bAlign;
			}

		public:

			bool                                     m_bAlign; // используем Align или PosOffset

			// Attributes
			nullable<SimpleTypes::CRelFromH<>      > m_oRelativeFrom;

			// Childs
			nullable<SimpleTypes::CAlignH<>        > m_oAlign;
			nullable<SimpleTypes::CPositionOffset<>> m_oPosOffset;
		};
		//--------------------------------------------------------------------------------
		// CPosV 20.4.2.11 (Part 1)
		//--------------------------------------------------------------------------------	
		class CPosV : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPosV)
			CPosV()
			{
			}
			virtual ~CPosV()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();
					if ( _T("wp:align") == sName )
					{
						m_oAlign = oReader.GetText2().GetString();
						m_bAlign = true;
					}
					else if ( _T("wp:posOffset") == sName )
					{
						m_oPosOffset = oReader.GetText2().GetString();
						m_bAlign = false;
					}
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<wp:positionV ");

				if ( m_oRelativeFrom.IsInit() )
					sResult += _T("relativeFrom=\"") + m_oRelativeFrom->ToString() + _T("\">");
				else
					sResult += _T(">");

				if ( m_bAlign && m_oAlign.IsInit() )
					sResult += _T("<wp:align>") + m_oAlign->ToString() + _T("</wp:align>");
				else if ( !m_bAlign && m_oPosOffset.IsInit() )
					sResult += _T("<wp:posOffset>") + m_oPosOffset->ToString() + _T("</wp:posOffset>");

				sResult += _T("</wp:positionV>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_wp_positionV;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("relativeFrom"), m_oRelativeFrom )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			bool IsAlign() const
			{
				return m_bAlign;
			}
			bool IsPosOffset() const
			{
				return !m_bAlign;
			}

		public:

			bool                                     m_bAlign; // используем Align или PosOffset

			// Attributes
			nullable<SimpleTypes::CRelFromV<>      > m_oRelativeFrom;

			// Childs
			nullable<SimpleTypes::CAlignV<>        > m_oAlign;
			nullable<SimpleTypes::CPositionOffset<>> m_oPosOffset;
		};
		//--------------------------------------------------------------------------------
		// CWrapNone 20.4.2.15 (Part 1)
		//--------------------------------------------------------------------------------	
		class CWrapNone : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CWrapNone)
			CWrapNone()
			{
			}
			virtual ~CWrapNone()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
			}
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual EElementType getType() const
			{
				return OOX::et_wp_wrapNone;
			}
		};
		//--------------------------------------------------------------------------------
		// CWrapSquare 20.4.2.17 (Part 1)
		//--------------------------------------------------------------------------------	
		class CWrapSquare : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CWrapSquare)
			CWrapSquare()
			{
			}
			virtual ~CWrapSquare()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();
					if ( _T("wp:effectExtents") == sName )
						m_oEffectExtent = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<wp:wrapSquare ");
				if(m_oWrapText.IsInit())
					sResult += _T("wrapText=\"") + m_oWrapText->ToString() + _T("\" ");

				if ( m_oDistB.IsInit() ) sResult += _T("distB=\"") + m_oDistB->ToString() + _T("\" ");
				if ( m_oDistL.IsInit() ) sResult += _T("distL=\"") + m_oDistL->ToString() + _T("\" ");
				if ( m_oDistR.IsInit() ) sResult += _T("distR=\"") + m_oDistR->ToString() + _T("\" ");
				if ( m_oDistT.IsInit() ) sResult += _T("distT=\"") + m_oDistT->ToString() + _T("\" ");

				if ( m_oEffectExtent.IsInit() )
					sResult += m_oEffectExtent->toXML();

				sResult += _T("</wp:wrapSquare>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_wp_wrapSquare;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("distB"),    m_oDistB )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("distL"),    m_oDistL )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("distR"),    m_oDistR )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("distT"),    m_oDistT )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("wrapText"), m_oWrapText )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes
			nullable<SimpleTypes::CWrapDistance<>> m_oDistB;
			nullable<SimpleTypes::CWrapDistance<>> m_oDistL;
			nullable<SimpleTypes::CWrapDistance<>> m_oDistR;
			nullable<SimpleTypes::CWrapDistance<>> m_oDistT;
			nullable<SimpleTypes::CWrapText<>>     m_oWrapText;

			// Childs
			nullable<OOX::Drawing::CEffectExtent>  m_oEffectExtent;
		};
		//--------------------------------------------------------------------------------
		// CWrapPath 20.4.2.16 (Part 1)
		//--------------------------------------------------------------------------------	
		class CWrapPath : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CWrapPath)
			CWrapPath()
			{
			}
			virtual ~CWrapPath()
			{
				for ( unsigned int nIndex = 0; nIndex < m_arrLineTo.size(); nIndex++ )
				{
					if ( m_arrLineTo[nIndex] )delete m_arrLineTo[nIndex];
					m_arrLineTo[nIndex] = NULL;
				}
				m_arrLineTo.clear();
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();

				bool bStart = false;
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();
					if ( _T("wp:start") == sName )
					{
						m_oStart = oReader;
						bStart = true;
					}
					else if ( bStart && _T("wp:lineTo") == sName )
					{
						ComplexTypes::Drawing::CPoint2D *oPoint = new ComplexTypes::Drawing::CPoint2D(oReader);
						if (oPoint) m_arrLineTo.push_back( oPoint );
					}
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("");
				//CString sResult = _T("<wp:wrapPolygon ");
				//
				//if ( m_oEdited.IsInit() ) sResult += _T("edited=\"") + m_oEdited->ToString() + _T("\">");
				//else                      sResult += _T(">");

				//sResult += _T("<wp:start ") +  m_oStart.ToString() + _T("/>");

				//for ( int nIndex = 0; nIndex < m_arrLineTo.GetSize(); nIndex++ )
				//	sResult += _T("<wp:lineTo ") + m_arrLineTo[nIndex].ToString() + _T("/>");

				//sResult += _T("</wp:wrapPolygon>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_wp_wrapPolygon;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("edited"), m_oEdited )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes
			nullable<SimpleTypes::COnOff<> >				m_oEdited;

			// Childs
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
			CWrapThrough()
			{
			}
			virtual ~CWrapThrough()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();
					if ( _T("wp:wrapPolygon") == sName )
						m_oWrapPolygon = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<wp:wrapThrough ");
				
				if ( m_oDistL.IsInit()    ) sResult += _T("distL=\"")    + m_oDistL->ToString()    + _T("\" ");
				if ( m_oDistR.IsInit()    ) sResult += _T("distR=\"")    + m_oDistR->ToString()    + _T("\" ");
				if ( m_oWrapText.IsInit() ) sResult += _T("wrapText=\"") + m_oWrapText->ToString() + _T("\" ");
				
				sResult += _T(">");
				if(m_oWrapPolygon.IsInit())
					sResult += m_oWrapPolygon->toXML();
				sResult += _T("</wp:wrapThrough>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_wp_wrapThrough;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("distL"),    m_oDistL )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("distR"),    m_oDistR )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("wrapText"), m_oWrapText )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes
			nullable<SimpleTypes::CWrapDistance<> >  m_oDistL;
			nullable<SimpleTypes::CWrapDistance<> >  m_oDistR;
			nullable<SimpleTypes::CWrapText<>     >  m_oWrapText;

			// Childs
			nullable<OOX::Drawing::CWrapPath>     m_oWrapPolygon;
		};
		//--------------------------------------------------------------------------------
		// CWrapTight 20.4.2.19 (Part 1)
		//--------------------------------------------------------------------------------	
		class CWrapTight : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CWrapTight)
			CWrapTight()
			{
			}
			virtual ~CWrapTight()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();
					if ( _T("wp:wrapPolygon") == sName )
						m_oWrapPolygon = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<wp:wrapTight ");
				
				if ( m_oDistL.IsInit()    ) sResult += _T("distL=\"")    + m_oDistL->ToString()    + _T("\" ");
				if ( m_oDistR.IsInit()    ) sResult += _T("distR=\"")    + m_oDistR->ToString()    + _T("\" ");
				if ( m_oWrapText.IsInit() ) sResult += _T("wrapText=\"") + m_oWrapText->ToString() + _T("\" ");

				sResult += _T(">");
				if(m_oWrapPolygon.IsInit())
					sResult += m_oWrapPolygon->toXML();
				sResult += _T("</wp:wrapTight>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_wp_wrapTight;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("distL"),    m_oDistL )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("distR"),    m_oDistR )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("wrapText"), m_oWrapText )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes
			nullable<SimpleTypes::CWrapDistance<> >  m_oDistL;
			nullable<SimpleTypes::CWrapDistance<> >  m_oDistR;
			nullable<SimpleTypes::CWrapText<>     >  m_oWrapText;

			// Childs
			nullable<OOX::Drawing::CWrapPath>                  m_oWrapPolygon;
		};
		//--------------------------------------------------------------------------------
		// CWrapTopBottom 20.4.2.20 (Part 1)
		//--------------------------------------------------------------------------------	
		class CWrapTopBottom : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CWrapTopBottom)
			CWrapTopBottom()
			{
			}
			virtual ~CWrapTopBottom()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();
					if ( _T("wp:effectExtent") == sName )
						m_oEffectExtent = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<wp:wrapTopAndBottom ");
				
				if ( m_oDistB.IsInit() ) sResult += _T("distB=\"") + m_oDistB->ToString() + _T("\" ");
				if ( m_oDistT.IsInit() ) sResult += _T("distT=\"") + m_oDistT->ToString() + _T("\" ");

				sResult += _T(">");
				
				if ( m_oEffectExtent.IsInit() )
					sResult += m_oEffectExtent->toXML();
				
				sResult += _T("</wp:wrapTopAndBottom>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_wp_wrapTopAndBottom;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("distB"), m_oDistB )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("distT"), m_oDistT )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes
			nullable<SimpleTypes::CWrapDistance<>> m_oDistB;
			nullable<SimpleTypes::CWrapDistance<>> m_oDistT;

			// Childs
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
			CAnchor()
			{
			}
			virtual ~CAnchor()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				m_eWrapType = anchorwrapUnknown;

				// TO DO: Реализовать
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();
					if ( _T("wp:cNvGraphicFramePr") == sName )
						m_oCNvGraphicFramePr = oReader;
					else if ( _T("wp:docPr") == sName )
						m_oDocPr = oReader;
					else if ( _T("wp:effectExtent") == sName )
						m_oEffectExtent = oReader;
					else if ( _T("wp:extent") == sName )
						m_oExtent = oReader;
					else if ( _T("a:graphic") == sName )
						m_oGraphic = oReader;
					else if ( _T("wp:positionH") == sName )
						m_oPositionH = oReader;
					else if ( _T("wp:positionV") == sName )
						m_oPositionV = oReader;
					else if ( _T("wp:simplePos") == sName )
						m_oSimplePos = oReader;
					else if ( false == m_eWrapType.IsInit() )
					{
						if ( _T("wp:wrapNone") == sName )
						{
							m_oWrapNone = oReader;
							m_eWrapType = anchorwrapNone;
						}
						else if ( _T("wp:wrapSquare") == sName )
						{
							m_oWrapSquare = oReader;
							m_eWrapType = anchorwrapSquare;
						}
						else if ( _T("wp:wrapThrough") == sName )
						{
							m_oWrapThrough = oReader;
							m_eWrapType = anchorwrapThrough;
						}
						else if ( _T("wp:wrapTight") == sName )
						{
							m_oWrapTight = oReader;
							m_eWrapType = anchorwrapTight;
						}
						else if ( _T("wp:wrapTopAndBottom") == sName )
						{
							m_oWrapTopAndBottom = oReader;
							m_eWrapType = anchorwrapTopAndBottom;
						}
					}
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("");
				//CString sResult = _T("<wp:anchor ");
				//
				//sResult += _T("allowOverlap=\"") + m_oAllowOverlap.ToString()   + _T("\" ");
				//sResult += _T("behindDoc=\"")    + m_oBehindDoc.ToString()      + _T("\" ");
				//if ( m_oDistB.IsInit() ) sResult += _T("distB=\"") + m_oDistB->ToString() + _T("\" ");
				//if ( m_oDistL.IsInit() ) sResult += _T("distL=\"") + m_oDistL->ToString() + _T("\" ");
				//if ( m_oDistR.IsInit() ) sResult += _T("distR=\"") + m_oDistR->ToString() + _T("\" ");
				//if ( m_oDistT.IsInit() ) sResult += _T("distT=\"") + m_oDistT->ToString() + _T("\" ");
				//sResult += _T("hidden=\"")         + m_oHidden.ToString()         + _T("\" ");
				//sResult += _T("layoutInCell=\"")   + m_oLayoutInCell.ToString()   + _T("\" ");
				//sResult += _T("locked=\"")         + m_oLocked.ToString()         + _T("\" ");
				//sResult += _T("relativeHeight=\"") + m_oRelativeHeight.ToString() + _T("\" ");
				//sResult += _T("simplePos=\"")      + m_bSimplePos.ToString()      + _T("\" ");

				//sResult += _T(">");
				//
				//if ( m_oSimplePos.IsInit() )
				//	sResult += _T("<wp:simplePos ") + m_oSimplePos->ToString() + _T("/>");

				//if ( m_oPositionH.IsInit() )
				//	sResult += m_oPositionH->toXML();

				//if ( m_oPositionV.IsInit() )
				//	sResult += m_oPositionV->toXML();

				//if ( m_oExtent.IsInit() )  
				//	sResult += _T("<wp:extent ") + m_oExtent->ToString() + _T("/>");

				//if ( m_oEffectExtent.IsInit() )
				//	sResult += m_oEffectExtent->toXML();

				//switch( m_eWrapType )
				//{
				//case anchorwrapNone:
				//	{
				//		if ( m_oWrapNone.IsInit() )
				//			sResult += m_oWrapNone->toXML();

				//		break;
				//	}
				//case anchorwrapSquare:
				//	{
				//		if ( m_oWrapSquare.IsInit() )
				//			sResult += m_oWrapSquare->toXML();

				//		break;
				//	}

				//case anchorwrapThrough:
				//	{
				//		if ( m_oWrapThrough.IsInit() )
				//			sResult += m_oWrapThrough->toXML();

				//		break;
				//	}
				//case anchorwrapTight:
				//	{
				//		if ( m_oWrapTight.IsInit() )
				//			sResult += m_oWrapTight->toXML();

				//		break;
				//	}
				//case anchorwrapTopAndBottom:
				//	{
				//		if ( m_oWrapTopAndBottom.IsInit() )
				//			sResult += m_oWrapTopAndBottom->toXML();

				//		break;
				//	}
				//}

				//if ( m_oDocPr.IsInit() )
				//	sResult += m_oDocPr->toXML();

				//if ( m_oCNvGraphicFramePr.IsInit() )
				//	sResult += m_oCNvGraphicFramePr->toXML();

				//sResult += m_oGraphic.toXML();
				//
				//sResult += _T("</wp:anchor>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_wp_anchor;
			}

		public:

			//void SetWrapType(const EAnchorWrapType& eType)
			//{
			//	m_eWrapType = eType;

			//	m_oWrapNone.reset();
			//	m_oWrapSquare.reset();
			//	m_oWrapThrough.reset();
			//	m_oWrapTight.reset();
			//	m_oWrapTopAndBottom.reset();

			//	switch ( m_eWrapType )
			//	{
			//	case anchorwrapNone: m_oWrapNone.Init(); break;
			//	case anchorwrapSquare: m_oWrapSquare.Init(); break;
			//	case anchorwrapThrough: m_oWrapThrough.Init(); break;
			//	case anchorwrapTight: m_oWrapTight.Init(); break;
			//	case anchorwrapTopAndBottom: m_oWrapTopAndBottom.Init(); break;
			//	}
			//}

			//EAnchorWrapType GetWrapType() const
			//{
			//	return m_eWrapType;
			//}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("allowOverlap"),   m_oAllowOverlap )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("behindDoc"),      m_oBehindDoc )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("distB"),          m_oDistB )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("distL"),          m_oDistL )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("distR"),          m_oDistR )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("distT"),          m_oDistT )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("hidden"),         m_oHidden )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("layoutInCell"),   m_oLayoutInCell )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("locked"),         m_oLocked )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("relativeHeight"), m_oRelativeHeight )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("simplePos"),      m_bSimplePos )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<EAnchorWrapType> m_eWrapType;

			// Attributes
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse>> m_oAllowOverlap;
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse>> m_oBehindDoc;
			nullable<SimpleTypes::CWrapDistance<>      > m_oDistB;
			nullable<SimpleTypes::CWrapDistance<>      > m_oDistL;
			nullable<SimpleTypes::CWrapDistance<>      > m_oDistR;
			nullable<SimpleTypes::CWrapDistance<>      > m_oDistT;
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse>> m_oHidden;
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse>> m_oLayoutInCell;
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse>> m_oLocked;
			nullable<SimpleTypes::CUnsignedDecimalNumber<0> >      m_oRelativeHeight;
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse>> m_bSimplePos;

			// Childs
			nullable<OOX::Drawing::CNonVisualGraphicFrameProperties> m_oCNvGraphicFramePr;
			nullable<OOX::Drawing::CNonVisualDrawingProps          > m_oDocPr;
			nullable<OOX::Drawing::CEffectExtent                   > m_oEffectExtent;
			nullable<ComplexTypes::Drawing::CPositiveSize2D        > m_oExtent;
			nullable<OOX::Drawing::CGraphic                        > m_oGraphic;
			nullable<OOX::Drawing::CPosH                           > m_oPositionH;
			nullable<OOX::Drawing::CPosV                           > m_oPositionV;
			nullable<ComplexTypes::Drawing::CPoint2D               > m_oSimplePos;
			nullable<OOX::Drawing::CWrapNone                       > m_oWrapNone;
			nullable<OOX::Drawing::CWrapSquare                     > m_oWrapSquare;
			nullable<OOX::Drawing::CWrapThrough                    > m_oWrapThrough;
			nullable<OOX::Drawing::CWrapTight                      > m_oWrapTight;
			nullable<OOX::Drawing::CWrapTopBottom                  > m_oWrapTopAndBottom;

		};
		//--------------------------------------------------------------------------------
		// CInline 20.4.2.8 (Part 1)
		//--------------------------------------------------------------------------------	
		class CInline : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CInline)
			CInline()
			{
			}
			virtual ~CInline()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();
					if ( _T("wp:cNvGraphicFramePr") == sName )
						m_oCNvGraphicFramePr = oReader;
					else if ( _T("wp:docPr") == sName )
						m_oDocPr = oReader;
					else if ( _T("wp:effectExtent") == sName )
						m_oEffectExtent = oReader;
					else if ( _T("wp:extent") == sName )
						m_oExtent = oReader;
					else if ( _T("a:graphic") == sName )
						m_oGraphic = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<wp:inline ");
				
				if ( m_oDistB.IsInit() ) sResult += _T("distB=\"") + m_oDistB->ToString() + _T("\" ");
				if ( m_oDistL.IsInit() ) sResult += _T("distL=\"") + m_oDistL->ToString() + _T("\" ");
				if ( m_oDistR.IsInit() ) sResult += _T("distR=\"") + m_oDistR->ToString() + _T("\" ");
				if ( m_oDistT.IsInit() ) sResult += _T("distT=\"") + m_oDistT->ToString() + _T("\" ");

				sResult += _T(">");
				
				if ( m_oExtent.IsInit() )  
					sResult += _T("<wp:extent ") + m_oExtent->ToString() + _T("/>");

				if ( m_oEffectExtent.IsInit() )
					sResult += m_oEffectExtent->toXML();

				if ( m_oDocPr.IsInit() )
					sResult += m_oDocPr->toXML();

				if ( m_oCNvGraphicFramePr.IsInit() )
					sResult += m_oCNvGraphicFramePr->toXML();

				if ( m_oGraphic.IsInit() )
					sResult += m_oGraphic->toXML();
				
				sResult += _T("</wp:inline>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_wp_inline;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("distB"),          m_oDistB )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("distL"),          m_oDistL )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("distR"),          m_oDistR )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("distT"),          m_oDistT )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes
			nullable<SimpleTypes::CWrapDistance<>      > m_oDistB;
			nullable<SimpleTypes::CWrapDistance<>      > m_oDistL;
			nullable<SimpleTypes::CWrapDistance<>      > m_oDistR;
			nullable<SimpleTypes::CWrapDistance<>      > m_oDistT;

			// Childs
			nullable<OOX::Drawing::CNonVisualGraphicFrameProperties> m_oCNvGraphicFramePr;
			nullable<OOX::Drawing::CNonVisualDrawingProps          > m_oDocPr;
			nullable<OOX::Drawing::CEffectExtent                   > m_oEffectExtent;
			nullable<ComplexTypes::Drawing::CPositiveSize2D        > m_oExtent;
			nullable<OOX::Drawing::CGraphic>                         m_oGraphic;
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
			CDrawing()
			{
			}
			CDrawing(XmlUtils::CXmlNode& oNode)
			{
				fromXML( oNode );
			}
			CDrawing(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			//CDrawing(const RId& rId, const OOX::CPath& oFileName, const long lWidth, const long lHeight)
			//{
			//	m_bAnchor = false;

			//	if ( !m_oInline.Init() ) return;

			//	if ( !m_oInline->m_oDistB.Init() ) return;
			//	if ( !m_oInline->m_oDistT.Init() ) return;
			//	if ( !m_oInline->m_oDistR.Init() ) return;
			//	if ( !m_oInline->m_oDistL.Init() ) return;

			//	if ( !m_oInline->m_oExtent.Init() ) return;
			//	if ( !m_oInline->m_oEffectExtent.Init() ) return;

			//	int nNewH, nNewW, nDpi = 96;
			//	double dHeightPage = 25.7; // в сантиметрах
			//	double dWidthPage  = 16.5; // в сантиметрах
			//	double dMaximum = max( lWidth / ( dWidthPage / 2.54 * nDpi ), lHeight / ( dHeightPage / 2.54 * nDpi ) );

			//	if ( dMaximum < 1 )
			//	{				
			//		nNewH = (int)(lWidth  * 2.54 * 72 * 100 * 1000 / 20 / 96); // from px to sx 
			//		nNewW = (int)(lHeight * 2.54 * 72 * 100 * 1000 / 20 / 96); // from px to sx 
			//	}
			//	else
			//	{				
			//		nNewH = (int)(lWidth  / dMaximum * 2.54 * 72 * 100 * 1000 / 20 / 96); //from px to sx
			//		nNewW = (int)(lHeight / dMaximum * 2.54 * 72 * 100 * 1000 / 20 / 96); //from px to sx
			//	}

			//	m_oInline->m_oExtent->m_oCx.SetValue( nNewW );
			//	m_oInline->m_oExtent->m_oCy.SetValue( nNewH );

			//	m_oInline->m_oEffectExtent->m_oL.FromEmu( 19050 );
			//	m_oInline->m_oEffectExtent->m_oT.FromEmu( 0 );
			//	m_oInline->m_oEffectExtent->m_oR.FromEmu( 3175 );
			//	m_oInline->m_oEffectExtent->m_oB.FromEmu( 0 );

			//	if ( !m_oInline->m_oDocPr.Init() ) return;
			//	if ( !m_oInline->m_oDocPr->m_oId.Init() ) return;
			//	m_oInline->m_oDocPr->m_oId->SetValue( 1 );
			//	m_oInline->m_oDocPr->m_sName  = _T("Image 0");
			//	m_oInline->m_oDocPr->m_sDescr = CStringW( Encoding::unicode2utf8( oFileName.m_strFilename ) );

			//	if ( !m_oInline->m_oCNvGraphicFramePr.Init() ) return;
			//	if ( !m_oInline->m_oCNvGraphicFramePr->m_oGraphicFrameLocks.Init() ) return;
			//	m_oInline->m_oCNvGraphicFramePr->m_oGraphicFrameLocks->m_oNoChangeAspect.SetValue( SimpleTypes::onoffTrue );
			//	
			//	m_oInline->m_oGraphic.m_sUri = _T("http://schemas.openxmlformats.org/drawingml/2006/picture");
			//	//m_oInline->m_oGraphic->Pic->Id = 0;
			//	//m_oInline->m_oGraphic->Pic->Name = Encoding::unicode2utf8(filename.filename());
			//	//m_oInline->m_oGraphic->Pic->rId = rId;

			//	//m_oInline->m_oGraphic->Pic->Off.init();
			//	//m_oInline->m_oGraphic->Pic->Off->X = 0;
			//	//m_oInline->m_oGraphic->Pic->Off->Y = 0;

			//	//m_oInline->m_oGraphic->Pic->Ext.init();
			//	//m_oInline->m_oGraphic->Pic->Ext->Width  = nNewW;
			//	//m_oInline->m_oGraphic->Pic->Ext->Height = nNewH;
			//	//m_oInline->m_oGraphic->Pic->Prst = "rect" ;
			//}
			//CDrawing(const RId& rId, const OOX::CPath& oFileName, const long lEmuX, const SimpleTypes::ERelFromH& eRelFromH, const long lEmuY, const SimpleTypes::ERelFromV& eRelFromV, const long lEmuWidth, const long lEmuHeight)
			//{
			//	m_bAnchor = true;

			//	if ( !m_oAnchor.Init() ) return;

			//	if ( !m_oAnchor->m_oDistB.Init() ) return;
			//	if ( !m_oAnchor->m_oDistT.Init() ) return;
			//	if ( !m_oAnchor->m_oDistR.Init() ) return;
			//	if ( !m_oAnchor->m_oDistL.Init() ) return;

			//	if ( !m_oAnchor->m_oPositionH.Init() ) return;
			//	if ( !m_oAnchor->m_oPositionV.Init() ) return;

			//	if ( !m_oAnchor->m_oPositionH->m_oRelativeFrom.Init() ) return;
			//	if ( !m_oAnchor->m_oPositionH->m_oPosOffset.Init() ) return;
			//	if ( !m_oAnchor->m_oPositionV->m_oRelativeFrom.Init() ) return;
			//	if ( !m_oAnchor->m_oPositionV->m_oPosOffset.Init() ) return;
			//	
			//	m_oAnchor->m_oPositionH->m_oPosOffset->SetValue( lEmuX );
			//	m_oAnchor->m_oPositionH->m_oRelativeFrom->SetValue( eRelFromH );
			//	m_oAnchor->m_oPositionV->m_oPosOffset->SetValue( lEmuY );
			//	m_oAnchor->m_oPositionV->m_oRelativeFrom->SetValue( eRelFromV );

			//	m_oAnchor->SetWrapType( OOX::Drawing::anchorwrapTopAndBottom );

			//	m_oAnchor->m_bSimplePos.SetValue( SimpleTypes::onoffFalse );
			//	m_oAnchor->m_oRelativeHeight.SetValue( 0 );
			//	m_oAnchor->m_oBehindDoc.SetValue( SimpleTypes::onoffFalse );
			//	m_oAnchor->m_oLocked.SetValue( SimpleTypes::onoffFalse );
			//	m_oAnchor->m_oLayoutInCell.SetValue( SimpleTypes::onoffFalse );
			//	m_oAnchor->m_oAllowOverlap.SetValue( SimpleTypes::onoffFalse );

			//	if ( !m_oAnchor->m_oSimplePos.Init() ) return;
			//	m_oAnchor->m_oSimplePos->m_oX.FromEmu( 0 );
			//	m_oAnchor->m_oSimplePos->m_oY.FromEmu( 0 );

			//	if ( !m_oAnchor->m_oExtent.Init() ) return;
			//	m_oAnchor->m_oExtent->m_oCx.FromEmu( lEmuWidth );
			//	m_oAnchor->m_oExtent->m_oCy.FromEmu( lEmuHeight );

			//	if ( !m_oAnchor->m_oEffectExtent.Init() ) return;				
			//	m_oAnchor->m_oEffectExtent->m_oL.FromEmu( 19050 );
			//	m_oAnchor->m_oEffectExtent->m_oT.FromEmu( 0 );
			//	m_oAnchor->m_oEffectExtent->m_oR.FromEmu( 3175 );
			//	m_oAnchor->m_oEffectExtent->m_oB.FromEmu( 0 );

			//	if ( !m_oAnchor->m_oDocPr.Init() ) return;
			//	if ( !m_oAnchor->m_oDocPr->m_oId.Init() ) return;

			//	m_oAnchor->m_oDocPr->m_oId->SetValue( 1 );
			//	m_oAnchor->m_oDocPr->m_sName  = _T("Image 0");
			//	m_oAnchor->m_oDocPr->m_sDescr = CStringW( Encoding::unicode2utf8( oFileName.m_strFilename ) );

			//	if ( !m_oAnchor->m_oCNvGraphicFramePr.Init() ) return;
			//	if ( !m_oAnchor->m_oCNvGraphicFramePr->m_oGraphicFrameLocks.Init() ) return;
			//	m_oAnchor->m_oCNvGraphicFramePr->m_oGraphicFrameLocks->m_oNoChangeAspect.SetValue( SimpleTypes::onoffTrue );
			//	
			//	m_oAnchor->m_oGraphic.m_sUri = _T("http://schemas.openxmlformats.org/drawingml/2006/picture");

			//	//m_oAnchor->m_oGraphic->Pic->Id = 0;
			//	//m_oAnchor->m_oGraphic->Pic->Name = Encoding::unicode2utf8(filename.filename());
			//	//m_oAnchor->m_oGraphic->Pic->rId = rId;

			//	//m_oAnchor->m_oGraphic->Pic->Off.init();
			//	//m_oAnchor->m_oGraphic->Pic->Off->X = 0;
			//	//m_oAnchor->m_oGraphic->Pic->Off->Y = 0;

			//	//m_oAnchor->m_oGraphic->Pic->Ext.init();
			//	//m_oAnchor->m_oGraphic->Pic->Ext->Width  = lEmuWidth;
			//	//m_oAnchor->m_oGraphic->Pic->Ext->Height = lEmuHeight;
			//	//m_oAnchor->m_oGraphic->Pic->Prst = "rect";			
			//}
			virtual ~CDrawing()
			{
			}
			const CDrawing& operator =(const XmlUtils::CXmlNode& oNode)
			{
				fromXML( (XmlUtils::CXmlNode&)oNode );
				return *this;
			}
			const CDrawing& operator =(const XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return *this;
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML2(oReader, false);
			}
			void fromXML2(XmlUtils::CXmlLiteReader& oReader, bool bDoNotReadXml)
			{
				if ( oReader.IsEmptyNode() )
					return;
				XmlUtils::CXmlLiteReader* pReader = NULL;
				if(bDoNotReadXml)
				{
					pReader = &oReader;
				}
				else
				{
					m_sXml.Init();                    
					m_sXml->Append(oReader.GetOuterXml());
					CString sXml;
					sXml.Format(_T("<root xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\">%ls</root>"), m_sXml.get());
					pReader = new XmlUtils::CXmlLiteReader();
					pReader->FromString(sXml);
					pReader->ReadNextNode();//root
					pReader->ReadNextNode();//drawing
				}

				int nCurDepth = pReader->GetDepth();
				while( pReader->ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = pReader->GetName();
					if ( _T("wp:inline") == sName )
					{
						m_oInline = *pReader;
						m_bAnchor = false;
					}
					else if ( _T("wp:anchor") == sName )
					{
						m_oAnchor = *pReader;
						m_bAnchor = true;
					}
				}
				if(false == bDoNotReadXml)
				{
					delete pReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<w:drawing>");

				if ( m_bAnchor && m_oAnchor.IsInit() )
					sResult += m_oAnchor->toXML();
				else if ( !m_bAnchor && m_oInline.IsInit() )
					sResult += m_oInline->toXML();

				sResult += _T("</w:drawing>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_drawing;
			}

		public:

			bool IsAnchor() const
			{
				return m_bAnchor;
			}
			bool IsInline() const
			{
				return !m_bAnchor;
			}

		public:

			bool                            m_bAnchor; // используем Anchor или Inline

			nullable<CString> m_sXml;
			// Childs
			nullable<OOX::Drawing::CAnchor> m_oAnchor;
			nullable<OOX::Drawing::CInline> m_oInline;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_DRAWING_INCLUDE_H_
