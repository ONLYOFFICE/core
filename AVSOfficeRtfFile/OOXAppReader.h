#pragma once
#include "XmlUtils.h"
#include "RtfDocument.h"
#include "OOXReader.h"

	class OOXAppReader
	{
	private: CString m_sFileName;
	public: OOXAppReader(CString sFilename)
			{
				m_sFileName = sFilename;
			}
	public: bool Parse( ReaderParameter oParam )
			{
				XmlUtils::CXmlReader oXmlReader;
				if( TRUE ==  oXmlReader.OpenFromFile(m_sFileName) )
					if( TRUE == oXmlReader.ReadRootNode( _T("Properties") ) )
					{
						CString nEndingTime = oXmlReader.ReadNodeValue( _T("TotalTime") );
						if( _T("") != nEndingTime )
							oParam.oDocument->m_oInformation.m_nEndingTime = Strings::ToInteger( nEndingTime );
						CString nPages = oXmlReader.ReadNodeValue( _T("Pages") );
						if( _T("") != nPages )
							oParam.oDocument->m_oInformation.m_nNumberOfPages = Strings::ToInteger( nPages );
						CString nWords = oXmlReader.ReadNodeValue( _T("Words") );
						if( _T("") != nWords )
							oParam.oDocument->m_oInformation.m_nNumberOfWords = Strings::ToInteger( nWords );
						CString nCharacters = oXmlReader.ReadNodeValue( _T("Characters") );
						if( _T("") != nCharacters )
							oParam.oDocument->m_oInformation.m_nNumberOfCharactersWithoutSpace = Strings::ToInteger( nCharacters );
						CString CharactersWithSpaces = oXmlReader.ReadNodeValue( _T("CharactersWithSpaces") );
						if( _T("") != CharactersWithSpaces )
							oParam.oDocument->m_oInformation.m_nNumberOfCharactersWithSpace = Strings::ToInteger( CharactersWithSpaces );
						CString nVersion = oXmlReader.ReadNodeValue( _T("AppVersion") );
						if( _T("") != nVersion )
							oParam.oDocument->m_oInformation.m_nVersion = Strings::ToInteger( nVersion );

						return true;
					}
				return false;
			}

	};
