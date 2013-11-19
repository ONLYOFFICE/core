#pragma once
#ifndef OOX_LOGIC_DRAWING_SHARED_INCLUDE_H_
#define OOX_LOGIC_DRAWING_SHARED_INCLUDE_H_

#include "../../Base/Nullable.h"
#include "../../Common/SimpleTypes_Drawing.h"
#include "../../Common/SimpleTypes_Shared.h"

#include "../WritingElement.h"

#include "DrawingExt.h"

namespace OOX
{
	namespace Drawing
	{
		//--------------------------------------------------------------------------------
		// CColorMapping 20.1.6.1;20.1.6.8 (Part 1)
		//--------------------------------------------------------------------------------	
		class CColorMapping : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CColorMapping)
			CColorMapping()
			{
				m_eType = et_Unknown;
			}
			virtual ~CColorMapping()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				m_eType = et_Unknown;
				// TO DO: Реализовать CColorMapping::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_eType = et_Unknown;
				CWCharWrapper sName = oReader.GetName();
				if ( _T("a:clrMap") == sName )
					m_eType = et_a_clrMap;
				else if ( _T("a:overrideClrMapping") == sName )
					m_eType = et_a_overrideClrMapping;
				else
					return;

				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();
					if ( _T("a:extLst") == sName )
						m_oExtLst = oReader;
				}
			}
			virtual CString      toXML() const
			{
				if ( et_a_clrMap == m_eType )
				{
					CString sResult = _T("<a:clrMap bg1=\"")     + m_oBg1.ToString()     +
						_T("\" tx1=\"")     + m_oTx1.ToString()     + 
						_T("\" bg2=\"")     + m_oBg2.ToString()     +
						_T("\" tx2=\"")     + m_oTx2.ToString()     +
						_T("\" accent1=\"") + m_oAccent1.ToString() + 
						_T("\" accent2=\"") + m_oAccent2.ToString() + 
						_T("\" accent3=\"") + m_oAccent3.ToString() + 
						_T("\" accent4=\"") + m_oAccent4.ToString() + 
						_T("\" accent5=\"") + m_oAccent5.ToString() + 
						_T("\" accent6=\"") + m_oAccent6.ToString() + _T("\"");

					if ( m_oExtLst.IsInit() )
					{
						sResult += _T(">");
						sResult += m_oExtLst->toXML();
						sResult += _T("</a:clrMap>");
					}
					else
						sResult += _T("/>");

					return sResult;
				}
				else if ( et_a_overrideClrMapping == m_eType )
				{
					CString sResult = _T("<a:overrideClrMapping bg1=\"")     + m_oBg1.ToString()     +
						_T("\" tx1=\"")     + m_oTx1.ToString()     + 
						_T("\" bg2=\"")     + m_oBg2.ToString()     +
						_T("\" tx2=\"")     + m_oTx2.ToString()     +
						_T("\" accent1=\"") + m_oAccent1.ToString() + 
						_T("\" accent2=\"") + m_oAccent2.ToString() + 
						_T("\" accent3=\"") + m_oAccent3.ToString() + 
						_T("\" accent4=\"") + m_oAccent4.ToString() + 
						_T("\" accent5=\"") + m_oAccent5.ToString() + 
						_T("\" accent6=\"") + m_oAccent6.ToString() + _T("\"");

					if ( m_oExtLst.IsInit() )
					{
						sResult += _T(">");
						sResult += m_oExtLst->toXML();
						sResult += _T("</a:overrideClrMapping>");
					}
					else
						sResult += _T("/>");

					return sResult;
				}

				return _T("");
			}
			virtual EElementType getType() const
			{
				return m_eType;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				if ( oReader.GetAttributesCount() <= 0 )
					return;
				
				if ( !oReader.MoveToFirstAttribute() )
					return;
				
				CWCharWrapper wsName = oReader.GetName();
				while( !wsName.IsNull() )
				{
					wchar_t wsChar0 = wsName[0]; 

					switch ( wsChar0 )
					{
					case 'a':
						if      ( _T("accent1") == wsName ) m_oAccent1 = oReader.GetText();
						else if ( _T("accent2") == wsName ) m_oAccent2 = oReader.GetText();
						else if ( _T("accent3") == wsName ) m_oAccent3 = oReader.GetText();
						else if ( _T("accent4") == wsName ) m_oAccent4 = oReader.GetText();
						else if ( _T("accent5") == wsName ) m_oAccent5 = oReader.GetText();
						else if ( _T("accent6") == wsName ) m_oAccent6 = oReader.GetText();
						break;

					case 'b':
						if      ( _T("bg1") == wsName ) m_oBg1 = oReader.GetText();
						else if ( _T("bg2") == wsName ) m_oBg2 = oReader.GetText();
						break;

					case 'f':
						if      ( _T("folHlink") == wsName ) m_oFloHlink = oReader.GetText();
						break;

					case 'h':
						if      ( _T("hlink") == wsName ) m_oHlink = oReader.GetText();
						break;

					case 't':
						if      ( _T("tx1") == wsName ) m_oTx1 = oReader.GetText();
						else if ( _T("tx2") == wsName ) m_oTx2 = oReader.GetText();
						break;
					}

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();
			}

		public:

			EElementType m_eType;

			// Attributes
			SimpleTypes::CColorSchemeIndex<> m_oAccent1;
			SimpleTypes::CColorSchemeIndex<> m_oAccent2;
			SimpleTypes::CColorSchemeIndex<> m_oAccent3;
			SimpleTypes::CColorSchemeIndex<> m_oAccent4;
			SimpleTypes::CColorSchemeIndex<> m_oAccent5;
			SimpleTypes::CColorSchemeIndex<> m_oAccent6;
			SimpleTypes::CColorSchemeIndex<> m_oBg1;
			SimpleTypes::CColorSchemeIndex<> m_oBg2;
			SimpleTypes::CColorSchemeIndex<> m_oFloHlink;
			SimpleTypes::CColorSchemeIndex<> m_oHlink;
			SimpleTypes::CColorSchemeIndex<> m_oTx1;
			SimpleTypes::CColorSchemeIndex<> m_oTx2;

			// Childs
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;

		};

		//--------------------------------------------------------------------------------
		// CColorScheme 20.1.6.2 (Part 1)
		//--------------------------------------------------------------------------------	
		class CColorScheme : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CColorScheme)
			CColorScheme()
			{
			}
			virtual ~CColorScheme()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CColorScheme::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();
					if ( _T("a:accent1") == sName )
						m_oAccent1 = oReader;
					else if ( _T("a:accent2") == sName )
						m_oAccent2 = oReader;
					else if ( _T("a:accent3") == sName )
						m_oAccent3 = oReader;
					else if ( _T("a:accent4") == sName )
						m_oAccent4 = oReader;
					else if ( _T("a:accent5") == sName )
						m_oAccent5 = oReader;
					else if ( _T("a:accent6") == sName )
						m_oAccent6 = oReader;
					else if ( _T("a:dk1") == sName )
						m_oDk1 = oReader;
					else if ( _T("a:dk2") == sName )
						m_oDk2 = oReader;
					else if ( _T("a:extLst") == sName )
						m_oExtLst = oReader;
					else if ( _T("a:folHlink") == sName )
						m_oFolHlink = oReader;
					else if ( _T("a:hlink") == sName )
						m_oHlink = oReader;
					else if ( _T("a:lt1") == sName )
						m_oLt1 = oReader;
					else if ( _T("a:lt2") == sName )
						m_oLt2 = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:clrScheme name=\"") + m_sName + _T("\">");

				sResult += m_oDk1.toXML();
				sResult += m_oLt1.toXML();
				sResult += m_oDk2.toXML();
				sResult += m_oLt2.toXML();
				sResult += m_oAccent1.toXML();
				sResult += m_oAccent2.toXML();
				sResult += m_oAccent3.toXML();
				sResult += m_oAccent4.toXML();
				sResult += m_oAccent5.toXML();
				sResult += m_oAccent6.toXML();
				sResult += m_oHlink.toXML();
				sResult += m_oFolHlink.toXML();

				if ( m_oExtLst.IsInit() )
					sResult += m_oExtLst->toXML();

				sResult += _T("</a:clrScheme>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_clrScheme;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("name"), m_sName )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes
			CString                                         m_sName;

			// Childs
			OOX::Drawing::CStyleColor                       m_oAccent1;
			OOX::Drawing::CStyleColor                       m_oAccent2;
			OOX::Drawing::CStyleColor                       m_oAccent3;
			OOX::Drawing::CStyleColor                       m_oAccent4;
			OOX::Drawing::CStyleColor                       m_oAccent5;
			OOX::Drawing::CStyleColor                       m_oAccent6;
			OOX::Drawing::CStyleColor                       m_oDk1;
			OOX::Drawing::CStyleColor                       m_oDk2;
			OOX::Drawing::CStyleColor                       m_oFolHlink;
			OOX::Drawing::CStyleColor                       m_oHlink;
			OOX::Drawing::CStyleColor                       m_oLt1;
			OOX::Drawing::CStyleColor                       m_oLt2;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
		};

		//--------------------------------------------------------------------------------
		// CCustomColorList 20.1.6.3 (Part 1)
		//--------------------------------------------------------------------------------	
		class CCustomColorList : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCustomColorList)
			CCustomColorList()
			{
			}
			virtual ~CCustomColorList()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CCustomColorList::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();
					if ( _T("a:custClr") == sName )
					{
						OOX::Drawing::CCustomColor oCustClr = oReader;
						m_arrCustClr.Add( oCustClr );
					}
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:custClrLst>");

				for ( int nIndex = 0; nIndex < m_arrCustClr.GetSize(); nIndex++ )
					sResult += m_arrCustClr[nIndex].toXML();

				sResult += _T("</a:custClrLst>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_custClrLst;
			}

		public:

			// Childs
			CSimpleArray<OOX::Drawing::CCustomColor> m_arrCustClr;
		};

		//--------------------------------------------------------------------------------
		// CColorSchemeAndMapping 20.1.6.4 (Part 1)
		//--------------------------------------------------------------------------------	
		class CColorSchemeAndMapping : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CColorSchemeAndMapping)
			CColorSchemeAndMapping()
			{
			}
			virtual ~CColorSchemeAndMapping()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CColorSchemeAndMapping::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();
					if ( _T("a:clrScheme") == sName )
						m_oClrScheme = oReader;
					else if ( _T("a:clrMap") == sName )
						m_oClrMap = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:extraClrScheme>");

				sResult += m_oClrScheme.toXML();

				if ( m_oClrMap.IsInit() )
					sResult += m_oClrMap->toXML();

				sResult += _T("</a:extraClrScheme>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_extraClrScheme;
			}

		public:

			// Childs
			OOX::Drawing::CColorScheme            m_oClrScheme;
			nullable<OOX::Drawing::CColorMapping> m_oClrMap;
		};

		//--------------------------------------------------------------------------------
		// CColorSchemeList 20.1.6.5 (Part 1)
		//--------------------------------------------------------------------------------	
		class CColorSchemeList : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CColorSchemeList)
			CColorSchemeList()
			{
			}
			virtual ~CColorSchemeList()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CColorSchemeList::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();
					if ( _T("a:extraClrScheme") == sName )
					{
						OOX::Drawing::CColorSchemeAndMapping oExtra = oReader;
						m_arrExtraClrScheme.Add( oExtra );
					}
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:extraClrSchemeLst>");

				for ( int nIndex = 0; nIndex < m_arrExtraClrScheme.GetSize(); nIndex++ )
					sResult += m_arrExtraClrScheme[nIndex].toXML();

				sResult += _T("</a:extraClrSchemeLst>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_extraClrSchemeLst;
			}

		public:

			// Childs
			CSimpleArray<OOX::Drawing::CColorSchemeAndMapping> m_arrExtraClrScheme;
		};

		//--------------------------------------------------------------------------------
		// CMasterColorMapping 20.1.6.6 (Part 1)
		//--------------------------------------------------------------------------------	
		class CMasterColorMapping : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CMasterColorMapping)
			CMasterColorMapping()
			{
			}
			virtual ~CMasterColorMapping()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString      toXML() const
			{
				return _T("<a:masterClrMapping/>");
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_masterClrMapping;
			}
		};

		//--------------------------------------------------------------------------------
		// CObjectStyleDefaults 20.1.6.7 (Part 1)
		//--------------------------------------------------------------------------------	
		class CObjectStyleDefaults : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CObjectStyleDefaults)
			CObjectStyleDefaults()
			{
			}
			virtual ~CObjectStyleDefaults()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();
					if ( _T("a:extLst") == sName )
						m_oExtLst = oReader;
					else if ( _T("a:lnDef") == sName )
						m_oLnDef = oReader;
					else if ( _T("a:spDef") == sName )
						m_oSpDef = oReader;
					else if ( _T("a:txDef") == sName )
						m_oTxDef = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:objectDefaults>");

				if ( m_oSpDef.IsInit() )
					sResult += m_oSpDef->toXML();

				if ( m_oLnDef.IsInit() )
					sResult += m_oLnDef->toXML();

				if ( m_oTxDef.IsInit() )
					sResult += m_oTxDef->toXML();

				if ( m_oExtLst.IsInit() )
					sResult += m_oExtLst->toXML();

				sResult += _T("</a:objectDefaults>");
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_objectDefaults;
			}

		public:

			// Childs
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
			nullable<OOX::Drawing::CDefaultShapeDefinition> m_oLnDef;
			nullable<OOX::Drawing::CDefaultShapeDefinition> m_oSpDef;
			nullable<OOX::Drawing::CDefaultShapeDefinition> m_oTxDef;
		};
		//--------------------------------------------------------------------------------
		// CBaseStyles 20.1.6.10 (Part 1)
		//--------------------------------------------------------------------------------	
		class CBaseStyles : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBaseStyles)
			CBaseStyles()
			{
			}
			virtual ~CBaseStyles()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();
					if ( _T("a:clrScheme") == sName )
						m_oClrScheme = oReader;
					else if ( _T("a:extLst") == sName )
						m_oExtLst = oReader;
					else if ( _T("a:fmtScheme") == sName )
						m_oFmtScheme = oReader;
					else if ( _T("a:fontScheme") == sName )
						m_oFontScheme = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:themeElements>");

				sResult += m_oClrScheme.toXML();
				sResult += m_oFontScheme.toXML();
				sResult += m_oFmtScheme.toXML();

				if ( m_oExtLst.IsInit() )
					sResult += m_oExtLst->toXML();

				sResult += _T("</a:themeElements>");
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_themeElements;
			}

		public:

			// Childs
			OOX::Drawing::CColorScheme                      m_oClrScheme;
			OOX::Drawing::CStyleMatrix                      m_oFmtScheme;
			OOX::Drawing::CFontScheme                       m_oFontScheme;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
		};

	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_DRAWING_SHARED_INCLUDE_H_