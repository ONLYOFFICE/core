#pragma once
#ifndef OOX_LOGIC_DRAWING_CORE_INFO_INCLUDE_H_
#define OOX_LOGIC_DRAWING_CORE_INFO_INCLUDE_H_

#include "../../Base/Nullable.h"
#include "../../Common/SimpleTypes_Drawing.h"
#include "../../Common/SimpleTypes_Shared.h"

#include "../WritingElement.h"
#include "../RId.h"

#include "DrawingExt.h"
#include "DrawingShapeElements.h"
#include "DrawingTransform.h"
#include "DrawingEffects.h"
#include "DrawingStyles2.h"

namespace OOX
{
	namespace Drawing
	{
		//--------------------------------------------------------------------------------
		// CNonVisualPictureProperties 20.1.2.2.7;20.2.2.2 (Part 1)
		//--------------------------------------------------------------------------------	
		class CPictureLocking;
		class CNonVisualPictureProperties : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CNonVisualPictureProperties)
			CNonVisualPictureProperties()
			{
				m_eType = et_Unknown;
			}
			virtual ~CNonVisualPictureProperties()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode);
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual CString      toXML() const;
			virtual EElementType getType() const
			{
				return m_eType;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("preferRelativeResize"), m_oPreferRelativeResize )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			EElementType      m_eType;

			// Attributes
			SimpleTypes::COnOff<SimpleTypes::onoffTrue> m_oPreferRelativeResize;

			// Childs
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
			nullable<OOX::Drawing::CPictureLocking>         m_oPicLocks;
		};
		//--------------------------------------------------------------------------------
		// CNonVisualDrawingProps 20.1.2.2.8;20.2.2.3;20.4.2.5 (Part 1)
		//--------------------------------------------------------------------------------	
		class CHyperlink;
		class CNonVisualDrawingProps : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CNonVisualDrawingProps)
			CNonVisualDrawingProps()
			{
				m_eType = et_Unknown;
			}
			virtual ~CNonVisualDrawingProps()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode);
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual CString      toXML() const;
			virtual EElementType getType() const
			{
				return m_eType;
			}


		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("descr"),  m_sDescr )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("hidden"), m_oHidden )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("id"),     m_oId )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("name"),   m_sName )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("title"),  m_sTitle )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			EElementType m_eType;

			// Attributes
			nullable<CString>                               m_sDescr;
			nullable<SimpleTypes::COnOff<>>                 m_oHidden;
			nullable<SimpleTypes::CDrawingElementId<>>      m_oId;
			nullable<CString>                               m_sName;
			nullable<CString>                               m_sTitle;

			// Childs
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
			nullable<OOX::Drawing::CHyperlink             > m_oHlinkClick;
			nullable<OOX::Drawing::CHyperlink             > m_oHlinkHover;
		};
		//--------------------------------------------------------------------------------
		// CGraphicalObjectFrameLocking 20.1.2.2.19 (Part 1)
		//--------------------------------------------------------------------------------	
		class CGraphicalObjectFrameLocking : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CGraphicalObjectFrameLocking)
			CGraphicalObjectFrameLocking()
			{
			}
			virtual ~CGraphicalObjectFrameLocking()
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
					if ( _T("a:extLst") == sName )
						m_oExtLst = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:graphicFrameLocks ");
				
				sResult += _T("noChangeAspect=\"") + m_oNoChangeAspect.ToString() + _T("\" ");
				sResult += _T("noDrilldown=\"")    + m_oNoDrilldown.ToString()    + _T("\" ");
				sResult += _T("noGrp=\"")          + m_oNoGrp.ToString()          + _T("\" ");
				sResult += _T("noMove=\"")         + m_oNoMove.ToString()         + _T("\" ");
				sResult += _T("noResize=\"")       + m_oNoResize.ToString()       + _T("\" ");
				sResult += _T("noSelect=\"")       + m_oNoSelect.ToString()       + _T("\" ");

				sResult += _T(">");
				
				if ( m_oExtLst.IsInit() )
					sResult += m_oExtLst->toXML();
				
				sResult += _T("</a:graphicFrameLocks>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_graphicFrameLocks;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("noChangeAspect"), m_oNoChangeAspect )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("noDrilldown"),    m_oNoDrilldown )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("noGrp"),          m_oNoGrp )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("noMove"),         m_oNoMove )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("noResize"),       m_oNoResize )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("noSelect"),       m_oNoSelect )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes
			SimpleTypes::COnOff<SimpleTypes::onoffFalse   > m_oNoChangeAspect;
			SimpleTypes::COnOff<SimpleTypes::onoffFalse   > m_oNoDrilldown;
			SimpleTypes::COnOff<SimpleTypes::onoffFalse   > m_oNoGrp;
			SimpleTypes::COnOff<SimpleTypes::onoffFalse   > m_oNoMove;
			SimpleTypes::COnOff<SimpleTypes::onoffFalse   > m_oNoResize;
			SimpleTypes::COnOff<SimpleTypes::onoffFalse   > m_oNoSelect;

			// Childs
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
		};
		//--------------------------------------------------------------------------------
		// CHyperlink 20.1.2.2.23;21.1.2.3.5 (Part 1)
		//--------------------------------------------------------------------------------	
		class CEmbeddedWAVAudioFile;
		class CHyperlink : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CHyperlink)
			CHyperlink()
			{
				m_eType = et_Unknown;
			}
			virtual ~CHyperlink()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode);
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual CString      toXML() const;
			virtual EElementType getType() const
			{
				return m_eType;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("action"),         m_sAction )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("endSnd"),         m_oEndSnd )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("highlightClick"), m_oHighlightClick )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("history"),        m_oHistory )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("r:id"),           m_oId )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("invalidUrl"),     m_sInvalidUrl )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("tgtFrame"),       m_sTgtFrame )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("tooltip"),        m_sTooltip )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			EElementType                                           m_eType;

			// Attributes
			nullable<CString                                     > m_sAction;
			nullable<SimpleTypes::COnOff<>                       > m_oEndSnd;
			nullable<SimpleTypes::COnOff<>                       > m_oHighlightClick;
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffTrue> > m_oHistory;
			nullable<SimpleTypes::CRelationshipId                > m_oId;
			nullable<CString                                     > m_sInvalidUrl;
			nullable<CString                                     > m_sTgtFrame;
			nullable<CString                                     > m_sTooltip;

			// Childs
			nullable<OOX::Drawing::COfficeArtExtensionList       > m_oExtLst;
			nullable<OOX::Drawing::CEmbeddedWAVAudioFile         > m_oSnd;
		};	
        //-----------------------------------------------------------------------
        // CLineProperties 20.1.2.2.24
        //-----------------------------------------------------------------------
        class CLineProperties : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CLineProperties)
			CLineProperties()
			{
				m_eType       = et_Unknown;
				m_eFillType   = filltypeUnknown;
				m_eDashType   = linedashtypeUnknown;
				m_eJoinType   = linejointypeUnknown;
			}
			virtual ~CLineProperties()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				m_eType       = et_Unknown;
				m_eFillType   = filltypeUnknown;
				m_eDashType   = linedashtypeUnknown;
				m_eJoinType   = linejointypeUnknown;

				// TO DO: Реализовать CLineProperties::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_eType       = et_Unknown;
				m_eFillType   = filltypeUnknown;
				m_eDashType   = linedashtypeUnknown;
				m_eJoinType   = linejointypeUnknown;

                CString sName = oReader.GetName();
				if ( _T("a:ln") == sName )
					m_eType = et_a_ln;
				else
					return;

				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					sName = oReader.GetName();
					if ( _T("a:bevel") == sName )
					{
						m_oBevel    = oReader;
						m_eJoinType = linejointypeBevel;
					}
					else if ( _T("a:custDash") == sName )
					{
						m_oCustDash = oReader;
						m_eDashType = linedashtypeCustom;
					}
					else if ( _T("a:extLst") == sName )
						m_oExtLst = oReader;
					else if ( _T("a:gradFill") == sName )
					{
						m_oGradFill = oReader;
						m_eFillType = filltypeGradient;
					}
					else if ( _T("a:headEnd") == sName )
						m_oHeadEnd = oReader;
					else if ( _T("a:miter") == sName )
					{
						m_oMiter    = oReader;
						m_eJoinType = linejointypeMiter;
					}
					else if ( _T("a:noFill") == sName )
					{
						m_oNoFill   = oReader;
						m_eFillType = filltypeNo;
					}
					else if ( _T("a:pattFill") == sName )
					{
						m_oPattFill = oReader;
						m_eFillType = filltypePattern;
					}
					else if ( _T("a:prstDash") == sName )
					{
						m_oPrstDash = oReader;
						m_eDashType = linedashtypePreset;
					}
					else if ( _T("a:round") == sName )
					{
						m_oRound    = oReader;
						m_eJoinType = linejointypeRound;
					}
					else if ( _T("a:solidFill") == sName )
					{
						m_oSolidFill = oReader;
						m_eFillType  = filltypeSolid;
					}
					else if ( _T("a:tailEnd") == sName )
						m_oTailEnd = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult;

				if ( et_a_ln == m_eType )
					sResult = _T("<a:ln ");
				else
					return _T("");

				if ( m_oW.IsInit() )
					sResult += _T("w=\"") + m_oW->ToString() + _T("\" ");
				if ( m_oCap.IsInit() )
					sResult += _T("cap=\"") + m_oCap->ToString() + _T("\" ");
				if ( m_oCmpd.IsInit() )
					sResult += _T("cmpd=\"") + m_oCmpd->ToString() + _T("\" ");
				if ( m_oAlgn.IsInit() )
					sResult += _T("algn=\"") + m_oAlgn->ToString() + _T("\" ");

				sResult += _T(">");


				switch (m_eFillType)
				{
				case filltypeNo:

					if ( m_oNoFill.IsInit() )
						sResult += m_oNoFill->toXML();

					break;

				case filltypeSolid:

					if ( m_oSolidFill.IsInit() )
						sResult += m_oSolidFill->toXML();

					break;

				case filltypeGradient:

					if ( m_oGradFill.IsInit() )
						sResult += m_oGradFill->toXML();

					break;

				case filltypePattern:

					if ( m_oPattFill.IsInit() )
						sResult += m_oPattFill->toXML();

					break;
				}

				switch ( m_eDashType )
				{
				case linedashtypeCustom:

					if ( m_oCustDash.IsInit() )
						sResult += m_oCustDash->toXML();

					break;

				case linedashtypePreset:

					if ( m_oPrstDash.IsInit() )
						sResult += m_oPrstDash->toXML();

					break;
				}

				switch ( m_eJoinType )
				{
				case linejointypeRound:

					if ( m_oRound.IsInit() )
						sResult += m_oRound->toXML();

					break;

				case linejointypeBevel:

					if ( m_oBevel.IsInit() )
						sResult += m_oBevel->toXML();

					break;

				case linejointypeMiter:

					if ( m_oMiter.IsInit() )
						sResult += m_oMiter->toXML();

					break;
				}

				if ( m_oHeadEnd.IsInit() )
					sResult += m_oHeadEnd->toXML();

				if ( m_oTailEnd.IsInit() )
					sResult += m_oTailEnd->toXML();

				if ( m_oExtLst.IsInit() )
					sResult += m_oExtLst->toXML();

				if ( et_a_ln == m_eType )
					sResult += _T("</a:ln>");
				else
					return _T("");

				return sResult;

			}
			virtual EElementType getType() const
			{
				return m_eType;
			}


		public:

			EFillType     GetFillType() const
			{
				return m_eFillType;
			}

			ELineDashType GetDashType() const
			{
				return m_eDashType;
			}

			ELineJoinType GetJoinType() const
			{
				return m_eJoinType;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("algn"), m_oAlgn )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("cap"),  m_oCap )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("cmpd"), m_oCmpd )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w"),    m_oW )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			EElementType                                      m_eType;       // Тип ноды

			// Attributes
			nullable<SimpleTypes::CPenAlignment<>>            m_oAlgn;
			nullable<SimpleTypes::CLineCap<>>                 m_oCap;
			nullable<SimpleTypes::CCompoundLine<>>            m_oCmpd;
			nullable<SimpleTypes::CLineWidth<>>               m_oW;

			// Childs
			EFillType                                         m_eFillType;   // Тип заливки
			nullable<OOX::Drawing::CGradientFillProperties>   m_oGradFill;
			nullable<OOX::Drawing::CNoFillProperties>         m_oNoFill;
			nullable<OOX::Drawing::CPatternFillProperties>    m_oPattFill;
			nullable<OOX::Drawing::CSolidColorFillProperties> m_oSolidFill;

			ELineDashType                                     m_eDashType;   // Тип штриха
			nullable<OOX::Drawing::CDashStopList>             m_oCustDash;
			nullable<OOX::Drawing::CPresetLineDashProperties> m_oPrstDash;

			ELineJoinType                                     m_eJoinType;   // Тип соединения линий
			nullable<OOX::Drawing::CLineJoinBevel>            m_oBevel;
			nullable<OOX::Drawing::CLineJoinMiterProperties>  m_oMiter;
			nullable<OOX::Drawing::CLineJoinRound>            m_oRound;

			nullable<OOX::Drawing::CLineEndProperties>        m_oHeadEnd;
			nullable<OOX::Drawing::CLineEndProperties>        m_oTailEnd;
			nullable<OOX::Drawing::COfficeArtExtensionList>   m_oExtLst;
		};
		//--------------------------------------------------------------------------------
		// CPictureLocking 20.1.2.2.31 (Part 1)
		//--------------------------------------------------------------------------------	
		class CPictureLocking : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPictureLocking)
			CPictureLocking()
			{
			}
			virtual ~CPictureLocking()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO : Реализовать
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

					if ( _T("a:extLst") == sName )
						m_oExtLst = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:picLocks ");

				if ( m_oNoAdjustHandles.GetValue() != SimpleTypes::onoffFalse )
					sResult += _T("noAdjustHandles=\"true\" ");

				if ( m_oNoChangeArrowheads.GetValue() != SimpleTypes::onoffFalse )
					sResult += _T("noChangeArrowheads=\"true\" ");

				if ( m_oNoChangeAspect.GetValue() != SimpleTypes::onoffFalse )
					sResult += _T("noChangeAspect=\"true\" ");

				if ( m_oNoChangeShapeType.GetValue() != SimpleTypes::onoffFalse )
					sResult += _T("noChangeShapeType=\"true\" ");

				if ( m_oNoCrop.GetValue() != SimpleTypes::onoffFalse )
					sResult += _T("noCrop=\"true\" ");

				if ( m_oNoEditPoints.GetValue() != SimpleTypes::onoffFalse )
					sResult += _T("noEditPoints=\"true\" ");

				if ( m_oNoGrp.GetValue() != SimpleTypes::onoffFalse )
					sResult += _T("noGrp=\"true\" ");

				if ( m_oNoMove.GetValue() != SimpleTypes::onoffFalse )
					sResult += _T("noMove=\"true\" ");

				if ( m_oNoResize.GetValue() != SimpleTypes::onoffFalse )
					sResult += _T("noResize=\"true\" ");

				if ( m_oNoRot.GetValue() != SimpleTypes::onoffFalse )
					sResult += _T("noRot=\"true\" ");

				if ( m_oNoSelect.GetValue() != SimpleTypes::onoffFalse )
					sResult += _T("noSelect=\"true\" ");

				if ( m_oExtLst.IsInit() )
				{
					sResult += _T(">");
					sResult += m_oExtLst->toXML();
					sResult += _T("</a:picLocks>");
				}
				else
					sResult += _T("/>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return et_a_picLocks;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("noAdjustHandles"), m_oNoAdjustHandles )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("noChangeArrowheads"), m_oNoChangeArrowheads )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("noChangeAspect"), m_oNoChangeAspect )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("noChangeShapeType"), m_oNoChangeShapeType )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("noCrop"), m_oNoCrop )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("noEditPoints"), m_oNoEditPoints )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("noGrp"), m_oNoGrp )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("noMove"), m_oNoMove )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("noResize"), m_oNoResize )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("noRot"), m_oNoRot )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("noSelect"), m_oNoSelect )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes
			SimpleTypes::COnOff<SimpleTypes::onoffFalse> m_oNoAdjustHandles;
			SimpleTypes::COnOff<SimpleTypes::onoffFalse> m_oNoChangeArrowheads;
			SimpleTypes::COnOff<SimpleTypes::onoffFalse> m_oNoChangeAspect;
			SimpleTypes::COnOff<SimpleTypes::onoffFalse> m_oNoChangeShapeType;
			SimpleTypes::COnOff<SimpleTypes::onoffFalse> m_oNoCrop;
			SimpleTypes::COnOff<SimpleTypes::onoffFalse> m_oNoEditPoints;
			SimpleTypes::COnOff<SimpleTypes::onoffFalse> m_oNoGrp;
			SimpleTypes::COnOff<SimpleTypes::onoffFalse> m_oNoMove;
			SimpleTypes::COnOff<SimpleTypes::onoffFalse> m_oNoResize;
			SimpleTypes::COnOff<SimpleTypes::onoffFalse> m_oNoRot;
			SimpleTypes::COnOff<SimpleTypes::onoffFalse> m_oNoSelect;

			// Childs
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
		};


        //-----------------------------------------------------------------------
        // EmbeddedWAVAudioFile 20.1.2.2.32
        //-----------------------------------------------------------------------
        class CEmbeddedWAVAudioFile : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CEmbeddedWAVAudioFile)
			CEmbeddedWAVAudioFile()
			{
			}
			virtual ~CEmbeddedWAVAudioFile()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				oNode.ReadAttributeBase( _T("r:embed"), m_oEmbed );
				oNode.ReadAttributeBase( _T("name"), m_sName );
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:snd ");

				if ( m_oEmbed.IsInit() ) sResult += _T("r:embed=\"") + m_oEmbed->ToString() + _T("\" ");
				if ( m_sName.IsInit()  ) 
				{
					sResult += _T("name=\"");
					sResult += m_sName->GetString();
					sResult += _T("\" ");
				}

				sResult += _T("/>");

				return sResult;

			}
			virtual EElementType getType() const
			{
				return et_a_snd;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("r:embed"), m_oEmbed )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("name"), m_sName )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CRelationshipId> m_oEmbed;
			nullable<CString                     > m_sName;

		};
        //-----------------------------------------------------------------------
        // CShapeProperties 20.1.2.2.35
        //-----------------------------------------------------------------------
        class CShapeProperties : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CShapeProperties)
			CShapeProperties()
			{
				m_eType       = et_Unknown;
				m_eFillType   = filltypeUnknown;
				m_eGeomType   = geomtypeUnknown;
				m_eEffectType = effecttypeUnknown;
			}
			virtual ~CShapeProperties()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				m_eType       = et_Unknown;
				m_eFillType   = filltypeUnknown;
				m_eGeomType   = geomtypeUnknown;
				m_eEffectType = effecttypeUnknown;

				// TO DO: Реализовать CShapeProperties::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_eType       = et_Unknown;
				m_eFillType   = filltypeUnknown;
				m_eGeomType   = geomtypeUnknown;
				m_eEffectType = effecttypeUnknown;

                CString sName = oReader.GetName();
					if ( _T("a:spPr") == sName )
					m_eType = et_a_spPr;
				else if ( _T("pic:spPr") == sName )
					m_eType = et_pic_spPr;
				else if ( _T("xdr:spPr") == sName )
					m_eType = et_xdr_spPr;
				else if ( _T("c:spPr") == sName )
					m_eType = et_c_spPr;
				else if ( _T("wps:spPr") == sName )
					m_eType = et_w_spPr;
				else if ( _T("dsp:spPr") == sName )
					m_eType = et_dsp_spPr;
				else
					return;

				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					sName = oReader.GetName();
					if ( _T("a:blipFill") == sName )
					{
						m_oBlipFill = oReader;
						m_eFillType = filltypeBlip;
					}
					else if ( _T("a:custGeom") == sName )
					{
						m_oCustGeom = oReader;
						m_eGeomType = geomtypeCustom;
					}
					else if ( _T("a:effectDag") == sName )
					{
						m_oEffectDag  = oReader;
						m_eEffectType = effecttypeDag;
					}
					else if ( _T("a:effectLst") == sName )
					{
						m_oEffectList = oReader;
						m_eEffectType = effecttypeLst;
					}
					else if ( _T("a:extLst") == sName )
						m_oExtLst = oReader;
					else if ( _T("a:gradFill") == sName )
					{
						m_oGradFill = oReader;
						m_eFillType = filltypeGradient;
					}
					else if ( _T("a:grpFill") == sName )
					{
						m_oGrpFill  = oReader;
						m_eFillType = filltypeGroup;
					}
					else if ( _T("a:ln") == sName )
						m_oLn = oReader;
					else if ( _T("a:noFill") == sName )
					{
						m_oNoFill   = oReader;
						m_eFillType = filltypeNo;
					}
					else if ( _T("a:pattFill") == sName )
					{
						m_oPattFill = oReader;
						m_eFillType = filltypePattern;
					}
					else if ( _T("a:prstGeom") == sName )
					{
						m_oPrstGeom = oReader;
						m_eGeomType = geomtypePreset;
					}
					else if ( _T("a:scene3d") == sName )
						m_oScene3D = oReader;
					else if ( _T("a:solidFill") == sName )
					{
						m_oSolidFill = oReader;
						m_eFillType  = filltypeSolid;
					}
					else if ( _T("a:sp3d") == sName )
						m_oSp3D = oReader;
					else if ( _T("a:xfrm") == sName )
						m_oXfrm = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult;

				if ( m_eType == et_a_spPr )
					sResult = _T("<a:spPr ");
				else if ( et_pic_spPr == m_eType )
					sResult = _T("<pic:spPr ");
				else if ( et_xdr_spPr == m_eType )
					sResult = _T("<xdr:spPr ");
				else if ( et_c_spPr == m_eType )
					sResult = _T("<c:spPr ");
				else if ( et_w_spPr == m_eType )
					sResult = _T("<wps:spPr ");
				else if ( et_dsp_spPr == m_eType )
					sResult = _T("<dsp:spPr ");
				else
					return _T("");

				// Attributes
				if ( m_oBwMode.IsInit() )
					sResult += _T("bwMode=\"") + m_oBwMode->ToString() + _T("\">");
				else
					sResult += _T(">");


				// Childs
				if ( m_oXfrm.IsInit() )
					sResult += m_oXfrm->toXML();

				switch ( m_eGeomType )
				{
				case geomtypeCustom:

					if ( m_oCustGeom.IsInit() )
						sResult += m_oCustGeom->toXML(); 
					break;

				case geomtypePreset: 

					if ( m_oPrstGeom.IsInit() )
						sResult += m_oPrstGeom->toXML(); 
					break;
				}

				switch ( m_eFillType )
				{
				case filltypeNo:

					if ( m_oNoFill.IsInit() )
						sResult += m_oNoFill->toXML();
					break;

				case filltypeSolid: 

					if ( m_oSolidFill.IsInit() )
						sResult += m_oSolidFill->toXML(); 					
					break;

				case filltypeGradient:

					if ( m_oGradFill.IsInit() )
						sResult += m_oGradFill->toXML();
					break;

				case filltypeBlip:

					if ( m_oBlipFill.IsInit() )
						sResult += m_oBlipFill->toXML();
					break;

				case filltypePattern:

					if ( m_oPattFill.IsInit() )
						sResult += m_oPattFill->toXML();
					break;

				case filltypeGroup:

					if ( m_oGrpFill.IsInit() )
						sResult += m_oGrpFill->toXML();
					break;
				}

				if ( m_oLn.IsInit() )
					sResult += m_oLn->toXML();

				switch ( m_eEffectType )
				{
				case effecttypeDag:

					if ( m_oEffectDag.IsInit() )
						sResult += m_oEffectDag->toXML();
					break;

				case effecttypeLst:

					if ( m_oEffectList.IsInit() )
						sResult += m_oEffectList->toXML();
					break;
				}

				if ( m_oScene3D.IsInit() )
					sResult += m_oScene3D->toXML();

				if ( m_oSp3D.IsInit() )
					sResult += m_oSp3D->toXML();

				if ( m_oExtLst.IsInit() )
					sResult += m_oExtLst->toXML();

				if ( m_eType == et_a_spPr )
					sResult += _T("</a:spPr>");
				else if ( et_pic_spPr == m_eType )
					sResult = _T("</pic:spPr>");
				else if ( et_xdr_spPr == m_eType )
					sResult = _T("</xdr:spPr>");
				else if ( et_c_spPr == m_eType )
					sResult = _T("</c:spPr>");
				else if ( et_w_spPr == m_eType )
					sResult = _T("</wps:spPr>");
				else if ( et_dsp_spPr == m_eType )
					sResult = _T("</dsp:spPr>");
				return sResult;
			}
			virtual EElementType getType() const
			{
				return m_eType;
			}
		public:

			EFillType   GetFillType() const
			{
				return m_eFillType;
			}

			EGeomType   GetGeomType() const
			{
				return m_eGeomType;
			}

			EEffectType GetEffectType() const
			{
				return m_eEffectType;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("bwMode"), m_oBwMode )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			EElementType                                      m_eType;       // Тип ноды

			// Attributes
			nullable<SimpleTypes::CBlackWhiteMode<>>          m_oBwMode;

			// Childs
			EFillType                                         m_eFillType;   // Тип заливки
			nullable<OOX::Drawing::CBlipFillProperties>       m_oBlipFill;
			nullable<OOX::Drawing::CGradientFillProperties>   m_oGradFill;
			nullable<OOX::Drawing::CGroupFillProperties>      m_oGrpFill;
			nullable<OOX::Drawing::CNoFillProperties>         m_oNoFill;
			nullable<OOX::Drawing::CPatternFillProperties>    m_oPattFill;
			nullable<OOX::Drawing::CSolidColorFillProperties> m_oSolidFill;

			EGeomType                                         m_eGeomType;   // Тип графики
			nullable<OOX::Drawing::CCustomGeometry2D>         m_oCustGeom;
			nullable<OOX::Drawing::CPresetGeometry2D>         m_oPrstGeom;

			EEffectType                                       m_eEffectType; // Тип контейнера эффектов
			nullable<OOX::Drawing::CEffectContainer>          m_oEffectDag;
			nullable<OOX::Drawing::CEffectList>               m_oEffectList;

			nullable<OOX::Drawing::CLineProperties>           m_oLn;
			nullable<OOX::Drawing::COfficeArtExtensionList>   m_oExtLst;
			nullable<OOX::Drawing::CScene3D>                  m_oScene3D;
			nullable<OOX::Drawing::CShape3D>                  m_oSp3D;
			nullable<OOX::Drawing::CTransform2D>              m_oXfrm;


		};

		//--------------------------------------------------------------------------------
		//		20.5.2.18 grpSpPr (Group Shape Properties)
		//--------------------------------------------------------------------------------	
		class CGroupShapeProperties : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CGroupShapeProperties)
			CGroupShapeProperties()
			{
				m_eType       = et_Unknown;
				m_eFillType   = filltypeUnknown;
				m_eEffectType = effecttypeUnknown;
			}
			virtual ~CGroupShapeProperties()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
			}
			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				m_eType       = et_Unknown;
				m_eFillType   = filltypeUnknown;
				m_eEffectType = effecttypeUnknown;

				// TO DO: Реализовать CShapeProperties::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_eType       = et_Unknown;
				m_eFillType   = filltypeUnknown;
				m_eEffectType = effecttypeUnknown;

                CString sName = oReader.GetName();
				if ( _T("a:grpSpPr") == sName )
					m_eType = 		et_a_groupSpPr;
				else if ( _T("xdr:grpSpPr") == sName )
					m_eType = et_xdr_groupSpPr;
				else if ( _T("wpg:grpSpPr") == sName )
					m_eType = et_w_groupSpPr;
				else if ( _T("dsp:grpSpPr") == sName )
					m_eType = et_dsp_groupSpPr;
				else
					return;

				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					sName = oReader.GetName();
					if ( _T("a:blipFill") == sName )
					{
						m_oBlipFill = oReader;
						m_eFillType = filltypeBlip;
					}
					else if ( _T("a:effectDag") == sName )
					{
						m_oEffectDag  = oReader;
						m_eEffectType = effecttypeDag;
					}
					else if ( _T("a:effectLst") == sName )
					{
						m_oEffectList = oReader;
						m_eEffectType = effecttypeLst;
					}
					else if ( _T("a:extLst") == sName )
						m_oExtLst = oReader;
					else if ( _T("a:gradFill") == sName )
					{
						m_oGradFill = oReader;
						m_eFillType = filltypeGradient;
					}
					else if ( _T("a:grpFill") == sName )
					{
						m_oGrpFill  = oReader;
						m_eFillType = filltypeGroup;
					}
					else if ( _T("a:noFill") == sName )
					{
						m_oNoFill   = oReader;
						m_eFillType = filltypeNo;
					}
					else if ( _T("a:pattFill") == sName )
					{
						m_oPattFill = oReader;
						m_eFillType = filltypePattern;
					}
					else if ( _T("a:scene3d") == sName )
						m_oScene3D = oReader;
					else if ( _T("a:solidFill") == sName )
					{
						m_oSolidFill = oReader;
						m_eFillType  = filltypeSolid;
					}
					else if ( _T("a:xfrm") == sName )
						m_oXfrm = oReader;
				}			
			}

			virtual EElementType getType () const
			{
				return m_eType;
			}
			
			EFillType   GetFillType() const
			{
				return m_eFillType;
			}

			EEffectType GetEffectType() const
			{
				return m_eEffectType;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			EElementType                                      m_eType;       // Тип ноды

			// Childs
			EFillType                                         m_eFillType;   // Тип заливки
			nullable<OOX::Drawing::CBlipFillProperties>       m_oBlipFill;
			nullable<OOX::Drawing::CGradientFillProperties>   m_oGradFill;
			nullable<OOX::Drawing::CGroupFillProperties>      m_oGrpFill;
			nullable<OOX::Drawing::CNoFillProperties>         m_oNoFill;
			nullable<OOX::Drawing::CPatternFillProperties>    m_oPattFill;
			nullable<OOX::Drawing::CSolidColorFillProperties> m_oSolidFill;

			EEffectType                                       m_eEffectType; // Тип контейнера эффектов
			nullable<OOX::Drawing::CEffectContainer>          m_oEffectDag;
			nullable<OOX::Drawing::CEffectList>               m_oEffectList;

			nullable<OOX::Drawing::COfficeArtExtensionList>   m_oExtLst;
			nullable<OOX::Drawing::CScene3D>                  m_oScene3D;
			nullable<OOX::Drawing::CGroupTransform2D>		  m_oXfrm;
		};
        //-----------------------------------------------------------------------
        // CShapeStyle 20.1.2.2.37
        //-----------------------------------------------------------------------
        class CShapeStyle : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CShapeStyle)
			CShapeStyle()
			{
			}
			virtual ~CShapeStyle()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CShapeStyle::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{			
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();
					if ( _T("a:effectRef") == sName )
						m_oEffectRef = oReader;
					else if ( _T("a:fillRef") == sName )
						m_oFillRef = oReader;
					else if ( _T("a:fontRef") == sName )
						m_oFontRef = oReader;
					else if ( _T("a:lnRef") == sName )
						m_oLnRef = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:style>");

				sResult += m_oLnRef.toXML();
				sResult += m_oFillRef.toXML();
				sResult += m_oEffectRef.toXML();
				sResult += m_oFontRef.toXML();

				sResult += _T("</a:style>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return et_a_style;
			}
			//todooo сделать расширение до разных namespaсe

		public:

			// Childs
			OOX::Drawing::CStyleMatrixReference m_oEffectRef;
			OOX::Drawing::CStyleMatrixReference m_oFillRef;
			OOX::Drawing::CFontReference        m_oFontRef;
			OOX::Drawing::CStyleMatrixReference m_oLnRef;
		};
	} // Drawing

} // OOX



#endif // OOX_LOGIC_DRAWING_CORE_INFO_INCLUDE_H_
