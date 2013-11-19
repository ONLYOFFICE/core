#pragma once
#include "OOXFontReader.h"
#include "RtfProperty.h"


	class OOXFontSchemeReader
	{
	public: OOXFontSchemeReader()
			{
			}
	public: bool Parse( ReaderParameter oParam )
			{
				XmlUtils::CXmlReader oXmlReader;
				if( FALSE ==  oXmlReader.OpenFromXmlNode(oParam.oNode) )
					return false;

				if(TRUE == oXmlReader.ReadNode( _T("a:majorFont") ) ) 
				{
					oXmlReader.ReadNodeList( _T("*") );
					for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
					{
						CString sNodeName = oXmlReader.ReadNodeName(i);
						CString sFontName = oXmlReader.ReadNodeAttribute(i, _T("typeface"), _T("") );
						if( _T("") != sFontName)
						{
							if( sNodeName == _T("a:latin") )
							{
								oParam.oReader->m_smajorAscii = sFontName;
								oParam.oReader->m_smajorHAnsi = sFontName;
							}
							else if( sNodeName == _T("a:ea") )
								oParam.oReader->m_smajorEastAsia = sFontName;
							else if( sNodeName == _T("a:cs") )
								oParam.oReader->m_smajorBidi = sFontName;
						}
					}
				}
				oXmlReader.OpenFromXmlNode(oParam.oNode);

				if( TRUE == oXmlReader.ReadNode( _T("a:minorFont") ) )
				{
					oXmlReader.ReadNodeList( _T("*") );
					for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
					{
						CString sNodeName = oXmlReader.ReadNodeName(i);
						CString sFontName = oXmlReader.ReadNodeAttribute(i, _T("typeface"), _T("") );
						if( _T("") != sFontName)
						{
							if( sNodeName == _T("a:latin") )
							{
								oParam.oReader->m_sminorAscii = sFontName;
								oParam.oReader->m_sminorHAnsi = sFontName;
							}
							else if( sNodeName == _T("a:ea") )
								oParam.oReader->m_sminorEastAsia = sFontName;
							else if( sNodeName == _T("a:cs") )
								oParam.oReader->m_sminorBidi = sFontName;
						}
					}
				}

				return true;
			}
	};
