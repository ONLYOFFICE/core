#pragma once
#include "RtfDocument.h"
#include "../Common/XmlUtils.h"

	class OOXMathPropReader
	{
	public: OOXMathPropReader()
			{
			}
	public: bool Parse( ReaderParameter oParam )
			{
				XmlUtils::CXmlReader oXmlReader;
				if( TRUE == oXmlReader.OpenFromXmlNode(oParam.oNode) )
					if( TRUE == oXmlReader.ReadNodeList( _T("*") ) )
					{
						for(int i = 0; i < (int)oXmlReader.GetLengthList(); i++)
						{
							CString sProperty =  oXmlReader.ReadNodeName( i );
							CString sValue =  oXmlReader.ReadNodeAttribute( i, _T("m:val") );
							if( _T("m:mathFont") == sProperty )
							{
								RtfFont oFont;
								if( true == oParam.oDocument->m_oFontTable.GetFont(sValue, oFont) )
									oParam.oDocument->m_oMathProp.mmathFont = oFont.m_nID;
							}
							else if( _T("m:brkBin") == sProperty )
							{
								if( _T("before") == sValue )
									oParam.oDocument->m_oMathProp.mbrkBin = 0;
								else if( _T("after") == sValue )
									oParam.oDocument->m_oMathProp.mbrkBin = 1;
								else if( _T("repeat") == sValue )
									oParam.oDocument->m_oMathProp.mbrkBin = 2;
							}
							else if( _T("m:brkBinSub") == sProperty )
							{
								if( _T("--") == sValue )
									oParam.oDocument->m_oMathProp.mbrkBinSub = 0;
								else if( _T("+-") == sValue )
									oParam.oDocument->m_oMathProp.mbrkBinSub = 1;
								else if( _T("-+") == sValue )
									oParam.oDocument->m_oMathProp.mbrkBinSub = 2;
							}
							else if( _T("m:defJc") == sProperty )
							{
								if( _T("centerGroup") == sValue )
									oParam.oDocument->m_oMathProp.mbrkBinSub = 1;
								else if( _T("center") == sValue )
									oParam.oDocument->m_oMathProp.mbrkBinSub = 2;
								else if( _T("left") == sValue )
									oParam.oDocument->m_oMathProp.mbrkBinSub = 3;
								else if( _T("right") == sValue )
									oParam.oDocument->m_oMathProp.mbrkBinSub = 4;
							}
							else if( _T("m:dispDef") == sProperty )
							{
								if( _T("") != sValue )
									oParam.oDocument->m_oMathProp.mdispDef = Strings::ToInteger( sValue );
							}
							else if( _T("m:interSp") == sProperty )
							{
								if( _T("") != sValue )
									oParam.oDocument->m_oMathProp.minterSp = Strings::ToInteger( sValue );
							}
							else if( _T("m:intraSp") == sProperty )
							{
								if( _T("") != sValue )
									oParam.oDocument->m_oMathProp.mintraSp = Strings::ToInteger( sValue );
							}
							else if( _T("m:intraSp") == sProperty )
							{
								if( _T("") != sValue )
									oParam.oDocument->m_oMathProp.mintraSp = Strings::ToInteger( sValue );
							}
							else if( _T("m:lMargin") == sProperty )
							{
								if( _T("") != sValue )
									oParam.oDocument->m_oMathProp.mlMargin = Strings::ToInteger( sValue );
							}
							else if( _T("m:rMargin") == sProperty )
							{
								if( _T("") != sValue )
									oParam.oDocument->m_oMathProp.mrMargin = Strings::ToInteger( sValue );
							}
							else if( _T("m:naryLim") == sProperty )
							{
								if( _T("subSup") == sValue )
									oParam.oDocument->m_oMathProp.mnaryLim = 0;
								else if( _T("undOvr") == sValue )
									oParam.oDocument->m_oMathProp.mnaryLim = 1;
							}
							else if( _T("m:postSp") == sProperty )
							{
								if( _T("") != sValue )
									oParam.oDocument->m_oMathProp.mpostSp = Strings::ToInteger( sValue );
							}
							else if( _T("m:preSp") == sProperty )
							{
								if( _T("") != sValue )
									oParam.oDocument->m_oMathProp.mpreSp = Strings::ToInteger( sValue );
							}
							else if( _T("m:smallFrac") == sProperty )
							{
								if( _T("") != sValue )
									oParam.oDocument->m_oMathProp.msmallFrac = Strings::ToInteger( sValue );
							}
							else if( _T("m:wrapIndent") == sProperty )
							{
								if( _T("") != sValue )
									oParam.oDocument->m_oMathProp.mwrapIndent = Strings::ToInteger( sValue );
							}
							else if( _T("m:wrapRight") == sProperty )
							{
								if( _T("") != sValue )
									oParam.oDocument->m_oMathProp.mwrapRight = Strings::ToInteger( sValue );
							}
						}
						return true;
					}
				return false;
			}
	};
