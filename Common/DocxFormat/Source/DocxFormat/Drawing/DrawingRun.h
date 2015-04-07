#pragma once
#ifndef OOX_LOGIC_DRAWING_RUN_INCLUDE_H_
#define OOX_LOGIC_DRAWING_RUN_INCLUDE_H_

#include "../../Base/Nullable.h"
#include "../../Common/SimpleTypes_Drawing.h"
#include "../../Common/SimpleTypes_Shared.h"

#include "../WritingElement.h"

namespace OOX
{
	namespace Drawing
	{
		//--------------------------------------------------------------------------------
		// CTextFont 21.1.2.3.1;21.1.2.3.3;21.1.2.3.7;21.1.2.3.10 (Part 1)
		//--------------------------------------------------------------------------------	
		class CTextFont : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTextFont)
			CTextFont()
			{
				m_eType = et_Unknown;
			}
			virtual ~CTextFont()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				m_eType = et_Unknown;
				// TO DO: Реализовать CTextFont::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_eType = et_Unknown;
				CWCharWrapper sName = oReader.GetName();
				if ( _T("a:cs") == sName )
					m_eType = et_a_cs;
				else if ( _T("a:ea") == sName )
					m_eType = et_a_ea;
				else if ( _T("a:latin") == sName )
					m_eType = et_a_latin;
				else if ( _T("a:sym") == sName )
					m_eType = et_a_sym;
				else
					return;

				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString      toXML() const
			{
				CString sResult;

				switch ( m_eType )
				{
				case et_a_cs:    sResult = _T("<a:cs"); break;
				case et_a_ea:    sResult = _T("<a:ea"); break;
				case et_a_latin: sResult = _T("<a:latin"); break;
				case et_a_sym:   sResult = _T("<a:sym"); break;
				default: return _T("");
				}

				if ( m_oTypeFace.IsInit() )
					sResult += _T(" typeface=\"") + m_oTypeFace->ToString() + _T("\"");

				if ( m_oPanose.IsInit() )
					sResult += _T(" panose=\"") + m_oPanose->ToString() + _T("\"");

				if ( SimpleTypes::pitchfamilyDefUnk != m_oPitchFamily.GetValue() )
					sResult += _T(" pitchFamily=\"") + m_oPitchFamily.ToString() + _T("\"");

				if ( SimpleTypes::fontcharsetDefault != m_oCharset.GetValue() )
					sResult += _T(" charset=\"") + m_oCharset.ToString() + _T("\"");

				sResult += _T("/>");

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
					case 'c':
						if      ( _T("charset")     == wsName ) m_oCharset = oReader.GetText();
						break;

					case 'p':
						if      ( _T("panose")      == wsName ) m_oPanose = oReader.GetText();
						else if ( _T("pitchFamily") == wsName ) m_oPitchFamily = oReader.GetText();
						break;

					case 't':
						if      ( _T("typeface")    == wsName ) m_oTypeFace = oReader.GetText();
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
			SimpleTypes::CFontCharset<SimpleTypes::fontcharsetDefault> m_oCharset;
			nullable<SimpleTypes::CPanose>                             m_oPanose;
			SimpleTypes::CPitchFamily<SimpleTypes::pitchfamilyDefUnk>  m_oPitchFamily;
			nullable<SimpleTypes::CTextTypeface>                       m_oTypeFace;
		};

	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_DRAWING_RUN_INCLUDE_H_