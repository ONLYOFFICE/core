#pragma once
#include "OOXTextItemReader.h"

class OOXDocumentReader
{
private: CString m_sFilePath;
private: OOXTextItemReader m_oTextItemReader;
private: OOXReader* m_poReader;
private: RtfDocument* m_poDocument;
public: OOXDocumentReader(CString sFilePath):m_oTextItemReader(true)
		{
			m_sFilePath = sFilePath;
		}
public: ~OOXDocumentReader()
		{
		}
public: bool Parse( ReaderParameter oParam )
		{
			m_poReader = oParam.oReader;
			m_poDocument = oParam.oDocument;

			//парсим документ
			XmlUtils::CXmlReader oXmlReader;
			if( TRUE == oXmlReader.OpenFromFile(m_sFilePath) )
				if( TRUE == oXmlReader.ReadRootNode( _T("w:document") ) )
					if( TRUE == oXmlReader.ReadNode( _T("w:body") ) || ( TRUE == oXmlReader.ReadRootNode( _T("w:wordDocument") ) && TRUE == oXmlReader.ReadNode( _T("w:body") ) ) )
					{
						RtfSectionPtr oCurSection;
						if( true == oParam.oDocument->GetItem( oCurSection ) )
						{
							oCurSection->m_oProperty.SetDefaultOOX();
							//сначала считаем количесво секций и заполняем их свойства
							XML::IXMLDOMNodePtr oNode;
							oXmlReader.GetNode(oNode);
							XmlUtils::CXmlReader oXmlSubReader;
							if( TRUE == oXmlSubReader.OpenFromXmlNode( oNode ) )
							{
								if( TRUE == oXmlSubReader.ReadNodeList( _T("w:p") ) )
								{
									for( int i = 0; i < oXmlSubReader.GetLengthList(); i++ )
									{
										XML::IXMLDOMNodePtr oSubNode;
										oXmlSubReader.GetNode( i, oSubNode );
										XmlUtils::CXmlReader oXmlSubSubReader;
										if( TRUE == oXmlSubSubReader.OpenFromXmlNode( oSubNode ) )
											if( TRUE == oXmlSubSubReader.ReadNode( _T("w:pPr") ) )
												if( TRUE == oXmlSubSubReader.ReadNode( _T("w:sectPr") ) )
												{
													XML::IXMLDOMNodePtr oSubSubNode;
													oXmlSubSubReader.GetNode(oSubSubNode);
													ReaderParameter oNewParam = oParam;
													oNewParam.oNode = oSubSubNode;
													OOXSectionPropertyReader oSectReader;
													if( true == oSectReader.Parse( oNewParam, oCurSection->m_oProperty ) )
													{
														//создаем новую секцию
														oCurSection = RtfSectionPtr( new RtfSection() );
														oCurSection->m_oProperty.SetDefaultOOX();
														oParam.oDocument->AddItem( oCurSection );
													}
												}
									}
								}
								oXmlSubReader.OpenFromXmlNode( oNode );
								//читаем свойства последней секции
								if( TRUE == oXmlSubReader.ReadNode( _T("w:sectPr") ) )
								{
									XML::IXMLDOMNodePtr oNode;
									oXmlSubReader.GetNode(oNode);
									ReaderParameter oNewParam = oParam;
									oNewParam.oNode = oNode;
									OOXSectionPropertyReader oSectReader;
									oSectReader.Parse( oNewParam, oCurSection->m_oProperty );
								}
							}
							RtfSectionPtr oFirstSection;
							if( true == m_poDocument->GetItem( oFirstSection, 0 ) )
							{
								//читаем в первую секцию пока не придет событие OnSectionEnd
								m_oTextItemReader.m_oTextItems = oFirstSection;

								XML::IXMLDOMNodePtr oParseNode;
								oXmlReader.GetNode(oParseNode);
								ReaderParameter oNewParam = oParam;
								oNewParam.oNode = oParseNode;
								m_oTextItemReader.Parse( oNewParam );
							}
						}
					}
			return true;
		}
};