#pragma once
#include "OOXAbstractNumReader.h"
#include "OOXNumberingMapReader.h"
#include "OOXShapeReader.h"

	class OOXNumberingReader
	{
	public: bool Parse( ReaderParameter oParam, CString sFileName )
			{
				//выделяем только имя файла
				int nSlashIndex = sFileName.ReverseFind( '\\' );
				CString sName = sFileName.Right( sFileName.GetLength() - nSlashIndex  - 1) ;

				//проверяем наличие соответствующих rels
				CString sRelsPath = oParam.oReader->m_sDocumentPath + _T("\\_rels\\") + sName + _T(".rels");
				OOXRelsReaderPtr oRelsReader;
				if( ::GetFileAttributes( sRelsPath ) != DWORD(-1) )
				{
					oRelsReader = OOXRelsReaderPtr( new OOXRelsReader() );
					oRelsReader->Init( sRelsPath );
				}
				oParam.oRels = oRelsReader;

				XmlUtils::CXmlReader oXmlReader;
				oXmlReader.OpenFromFile(sFileName);
				oXmlReader.ReadRootNode( _T("w:numbering") );
				oXmlReader.ReadNodeList( _T("*") );
				for(int i=0;i<oXmlReader.GetLengthList();i++)
				{
					CString sNodeName = oXmlReader.ReadNodeName(i);
					XML::IXMLDOMNodePtr oNode;
					oXmlReader.GetNode(i,oNode);
					ReaderParameter oNewParam = oParam;
					oNewParam.oNode = oNode;
					if( _T("w:abstractNum") == sNodeName )
					{
						OOXAbstractNumReader oParagraph;
						oParagraph.Parse( oNewParam );
					}
					else if( _T("w:num") == sNodeName)
					{
						OOXNumberingMapReader oParagraph;
						oParagraph.Parse( oNewParam );
					}
					else if( _T("w:numPicBullet") == sNodeName )
					{
						CString sID = oXmlReader.ReadNodeAttribute( i, _T("w:numPicBulletId") );
						if( _T("") != sID )
						{
							int nID = Strings::ToInteger( sID );
							XmlUtils::CXmlReader oXmlSubReader;
							oXmlSubReader.OpenFromXmlNode( oNode );
							oXmlSubReader.ReadNode( _T("w:pict") );
							oXmlSubReader.ReadNode( _T("v:shape") );
							XML::IXMLDOMNodePtr oSubNode;
							oXmlSubReader.GetNode(oSubNode);
							ReaderParameter oNewParam = oParam;
							oNewParam.oNode = oSubNode;
							
							OOXShapeReader oShapeReader;
							RtfShapePtr oNewShape( new RtfShape() );	
							if( true == oShapeReader.Parse( oNewParam, oNewShape ) )
							{
								oNewShape->m_eShapeType = RtfShape::st_inline;
								oNewShape->m_nPositionHRelative = 3;
								oNewShape->m_nPositionVRelative = 3;
								int nIndex = oParam.oDocument->m_oListTabel.m_aPictureList.GetCount();
								oParam.oReader->m_mapPictureBullet.SetAt( nID, nIndex );
								oParam.oDocument->m_oListTabel.m_aPictureList.AddItem( oNewShape );
							}
						}
					}
				}
				
				return true;
			}
	};
