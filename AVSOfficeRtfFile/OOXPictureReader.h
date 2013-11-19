#pragma once
#include "RtfPicture.h"
#include "OOXPictureAnchorReader.h"
#include "OOXPictureInlineReader.h"

	class OOXPictureReader
	{
	public: OOXPictureReader()
			{
			}
	public: bool Parse( ReaderParameter oParam , RtfShape& oOutput)
			{
				//if( oParam.oReader->m_nCurItap != 0 )
				//	oOutput.m_bLayoutInCell = 1;
				XmlUtils::CXmlReader oXmlReader;
				BOOL oRes = oXmlReader.OpenFromXmlNode(oParam.oNode);
				CString sRelsId ;
				if( TRUE == oXmlReader.ReadNodeList( _T("*") ) )
				{
					for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
					{
						CString sNode = oXmlReader.ReadNodeName( i );
						XML::IXMLDOMNodePtr oNode;
						oXmlReader.GetNode( i, oNode);
						ReaderParameter oNewParam = oParam;
						oNewParam.oNode = oNode;
						if( _T("wp:inline") == sNode )
						{
							OOXPictureInlineReader oPictureInlineReader;
							return oPictureInlineReader.Parse( oNewParam, oOutput );
						}
						else if( _T("wp:anchor") == sNode  )
						{
							OOXPictureAnchorReader oPictureAnchorReader;
							return oPictureAnchorReader.Parse( oNewParam, oOutput );
						}
						else 
							return false;
					}
				}
				return true;
			}
	};
