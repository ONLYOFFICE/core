#pragma once
#include "RtfProperty.h"
#include "OOXReaderBasic.h"

class CtblLook
{
public: bool bFirstRow;
public: bool bLastRow;
public: bool bFirstCol;
public: bool bLastCol;
public: bool bNoVBand;
public: bool bNoHBand;
public: CtblLook()
		{
			bFirstRow = false;
			bLastRow = false;
			bFirstCol = false;
			bLastCol = false;
			bNoVBand = false;
			bNoHBand = false;
		}
};

class OOXtblLookReader
{
public: bool Parse( ReaderParameter oParam, CtblLook& oOutput)
		{
			XmlUtils::CXmlReader oXmlReader;
			oXmlReader.OpenFromXmlNode( oParam.oNode );
			CString sVal = oXmlReader.ReadNodeAttribute( _T("w:val"), _T("") );

			if( _T("") != sVal )
			{
				int nVal = 0;
				_stscanf( sVal, _T("%x"), &nVal);
				if( 256 & nVal ) //100000000
					oOutput.bFirstRow = false;
				else
					oOutput.bFirstRow = true;
				if( 128 & nVal ) //010000000
					oOutput.bLastRow = false;
				else
					oOutput.bLastRow = true;
				if( 64 & nVal ) //001000000
					oOutput.bFirstCol = false;
				else
					oOutput.bFirstCol = true;
				if( 32 & nVal ) //000100000
					oOutput.bLastCol = false;
				else
					oOutput.bLastCol = true;
				if( 16 & nVal ) //000010000
					oOutput.bNoHBand = false;
				else
					oOutput.bNoHBand = true;
				if( 8 & nVal ) //000001000
					oOutput.bNoVBand = false;
				else
					oOutput.bNoVBand = true;
			}
			else
			{
				COMMAND_OOX_BOOL_ATTRIBUTE_2( _T("firstRow"), oOutput.bFirstRow , oXmlReader )
				COMMAND_OOX_BOOL_ATTRIBUTE_2( _T("lastRow"), oOutput.bLastRow , oXmlReader )
				COMMAND_OOX_BOOL_ATTRIBUTE_2( _T("firstColumn"), oOutput.bFirstCol , oXmlReader )
				COMMAND_OOX_BOOL_ATTRIBUTE_2( _T("lastColumn"), oOutput.bLastCol , oXmlReader )
				COMMAND_OOX_BOOL_ATTRIBUTE_2( _T("noHBand"), oOutput.bNoHBand , oXmlReader )
				COMMAND_OOX_BOOL_ATTRIBUTE_2( _T("noVBand"), oOutput.bNoVBand , oXmlReader )
			}
			return true;

		}


};
