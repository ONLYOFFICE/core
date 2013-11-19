#pragma once
#include "OOXrPrReader.h"
#include "OOXpPrReader.h"


	class OOXDocDefaultsReader
	{
	public: OOXDocDefaultsReader()
			{
			}
	public: bool Parse( ReaderParameter oParam )
			{
				XmlUtils::CXmlReader oXmlReader;
				if( FALSE ==  oXmlReader.OpenFromXmlNode(oParam.oNode) )
					return false;

					if( TRUE == oXmlReader.ReadNode( _T("w:rPrDefault") ) )
						if( TRUE == oXmlReader.ReadNode( _T("w:rPr") ) )
						{
							XML::IXMLDOMNodePtr oNode;
							oXmlReader.GetNode(oNode);
							ReaderParameter oNewParam = oParam;
							oNewParam.oNode = oNode;
							RtfCharProperty oNewProperty;
							oNewProperty.SetDefaultOOX();
							OOXrPrReader orPrReader;
							orPrReader.m_bDefStyle = false;
							if( true == orPrReader.Parse( oNewParam, oNewProperty ) )
								oParam.oDocument->m_oDefaultCharProp = oNewProperty;
						}
					oXmlReader.OpenFromXmlNode(oParam.oNode);
					if( TRUE == oXmlReader.ReadNode( _T("w:pPrDefault") ) )
						if( TRUE == oXmlReader.ReadNode( _T("w:pPr") ) )
						{
							XML::IXMLDOMNodePtr oNode;
							oXmlReader.GetNode(oNode);
							ReaderParameter oNewParam = oParam;
							oNewParam.oNode = oNode;
							RtfParagraphProperty oNewProperty;
							OOXpPrReader opPrReader;
							opPrReader.m_bDefStyle = false;
							bool bStartNewSectoion;
							if( true == opPrReader.Parse( oNewParam, oNewProperty, CcnfStyle(), bStartNewSectoion ) )
								oParam.oDocument->m_oDefaultParagraphProp = oNewProperty;
						}
				return false;
			}

	};
