#pragma once
#include "../Common/XmlUtils.h"
#include "RtfDocument.h"
#include "OOXReader.h"

	class OOXCoreReader
	{
	private: CString m_sFileName;
	public: OOXCoreReader(CString sFilename)
			{
				m_sFileName = sFilename;
			}
	public: bool Parse( ReaderParameter oParam )
			{
				XmlUtils::CXmlReader oXmlReader;
				if( TRUE ==  oXmlReader.OpenFromFile(m_sFileName) )
					if( TRUE == oXmlReader.ReadRootNode( _T("cp:coreProperties") ) )
					{
						CString sValue = oXmlReader.ReadNodeValue( _T("dc:title"), _T("") ) ;
						if( _T("") != sValue )
							oParam.oDocument->m_oInformation.m_sTitle = sValue;
						sValue = oXmlReader.ReadNodeValue( _T("dc:subject"), _T("") ) ;
						if( _T("") != sValue )
							oParam.oDocument->m_oInformation.m_sSubject = sValue;
						sValue = oXmlReader.ReadNodeValue( _T("dc:creator"), _T("") ) ;
						if( _T("") != sValue )
							oParam.oDocument->m_oInformation.m_sAuthor = sValue;
						sValue = oXmlReader.ReadNodeValue( _T("dc:description"), _T("") ) ;
						if( _T("") != sValue )
							oParam.oDocument->m_oInformation.m_sComment = sValue;
						sValue = oXmlReader.ReadNodeValue( _T("cp:keywords"), _T("") ) ;
						if( _T("") != sValue )
							oParam.oDocument->m_oInformation.m_sKeywords = sValue;
						return true;
					}
				return false;
			}

	};
