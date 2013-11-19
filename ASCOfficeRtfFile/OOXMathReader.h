#pragma once
#include "RtfDocument.h"
#include "XmlUtils.h"
#include "RtfMath.h"

	class OOXMathReader
	{
	public: OOXMathReader()
			{
			}
	public: bool Parse( ReaderParameter oParam , RtfMath& oOutput)
			{
				RtfCharProperty oCurrentProp;
				oCurrentProp.SetDefaultOOX();

				XmlUtils::CXmlReader oXmlReader;
				if( TRUE == oXmlReader.OpenFromXmlNode(oParam.oNode) )
					if( TRUE == oXmlReader.ReadNodeList( _T("*") ) )
					{
						for(int i = 0; i < (int)oXmlReader.GetLengthList(); i++)
						{
							CString sNodeName =  oXmlReader.ReadNodeName( i );
							CString sValue = oXmlReader.ReadNodeAttribute( i, _T("m:val") );
							RtfMathPtr oNewMath( new RtfMath() );
							if( true == RtfMath::IsOOXControlWord( sNodeName ) )
							{
								XML::IXMLDOMNodePtr oNode;
								oXmlReader.GetNode( i , oNode );
								ReaderParameter oNewParam = oParam;
								oNewParam.oNode = oNode;
								oNewMath->SetOOXName( sNodeName );
								OOXMathReader oMathReader;
								if( true == oMathReader.Parse( oNewParam, (*oNewMath) ) )
									oOutput.AddItem( oNewMath );
							}
							//здесь смотрим rPr
							if( _T("w:rPr") == sNodeName )
							{
								XML::IXMLDOMNodePtr oNode;
								oXmlReader.GetNode( i , oNode );
								ReaderParameter oNewParam = oParam;
								oNewParam.oNode = oNode;

								OOXrPrReader orPrReader;
								orPrReader.Parse( oNewParam, oOutput.oProperty.m_oCharProp );
							}
							//здесь смотрим дополнительные атрибуты
							if( _T("m:alnScr") == sNodeName )
							{
								if( _T("1") == sValue )
									oNewMath->oProperty.malnScr = true;
							}
							else if( _T("m:oMathParaPr") == sNodeName )
							{
								XML::IXMLDOMNodePtr oNode;
								oXmlReader.GetNode( i , oNode );
								XmlUtils::CXmlReader oXmlSubReader;
								oXmlSubReader.OpenFromXmlNode( oNode );
								if( TRUE == oXmlSubReader.ReadNode( _T("m:jc") ) )
								{
									sValue = oXmlSubReader.ReadNodeAttribute( _T("m:val") ); 
									if( _T("") != sValue )
									{
										if( _T("center") == sValue )
											oNewMath->oProperty.moMathParaPr = 1;
										else if( _T("centerGroup") == sValue )
											oNewMath->oProperty.moMathParaPr = 2;
										else if( _T("left") == sValue )
											oNewMath->oProperty.moMathParaPr = 3;
										else if( _T("right") == sValue )
											oNewMath->oProperty.moMathParaPr = 4;
										else
											oNewMath->oProperty.moMathParaPr = 1;
									}
								}
							}
							else if( _T("m:brk") == sNodeName )
							{
								if( _T("false") != sValue )
									oNewMath->oProperty.Break = true;
							}
							else if( _T("m:chr") == sNodeName )
							{
								if( _T("") != sValue )
								{
									int nValue = PROP_DEF;
									if( -1 != sValue.Find( _T("&#") ) )
									{
										sValue.Replace( _T("&#"), _T("") );
										sValue.Replace( _T(";"), _T("") );
										nValue = Strings::ToInteger( sValue );
									}
									else
									{
										if( sValue.GetLength() > 0 )
											nValue = sValue[0];
									}
									if( PROP_DEF != nValue )
										oNewMath->oProperty.mchr = nValue;
								}
							}
							else if( _T("m:lit") == sNodeName )
							{
								if( "false" != sValue )
									oNewMath->oProperty.mlit = true;
							}
							else if( _T("m:nor") == sNodeName )
							{
								if( _T("1") == sValue )
									oNewMath->oProperty.NormalText = true;
							}
							else if( _T("m:scr") == sNodeName )
							{
								if( _T("roman") == sValue )
									oNewMath->oProperty.mscr = 0;
								else if( _T("script") == sValue )
									oNewMath->oProperty.mscr = 1;
								else if( _T("fraktur") == sValue )
									oNewMath->oProperty.mscr = 2;
								else if( _T("double-struck") == sValue )
									oNewMath->oProperty.mscr = 3;
								else if( _T("sans-serif") == sValue )
									oNewMath->oProperty.mscr = 4;
								else if( _T("monospace") == sValue )
									oNewMath->oProperty.mscr = 5;
							}
							else if( _T("m:sty") == sNodeName )
							{
								if( _T("p") == sValue )
									oNewMath->oProperty.msty = 0;
								else if( _T("b") == sValue )
									oNewMath->oProperty.msty = 1;
								else if( _T("i") == sValue )
									oNewMath->oProperty.msty = 2;
								else if( _T("bi") == sValue )
									oNewMath->oProperty.msty = 3;
							}
							else if( _T("w:rPr") == sNodeName)
							{
								XML::IXMLDOMNodePtr oNode;
								oXmlReader.GetNode( i , oNode );
								ReaderParameter oNewParam = oParam;
								oNewParam.oNode = oNode;

								oCurrentProp = RtfCharProperty();
								oCurrentProp.SetDefaultOOX();
								OOXrPrReader orPrReader;
								orPrReader.Parse( oNewParam, oCurrentProp );
							}
							else if( _T("m:t") == sNodeName )
							{
								CString sNodeName = oXmlReader.ReadNodeAttribute( i, _T("xml:space"), _T("") );
								CString sNodeValue= oXmlReader.ReadNodeText(i);
								if( _T("preserve") != sNodeName )
									sNodeValue = RtfUtility::Preserve( sNodeValue );
								RtfCharPtr oChar = RtfCharPtr(new RtfChar);
								oChar->m_oProperty = oCurrentProp;//todo
								oChar->setText( sNodeValue );
								oOutput.m_oVal.AddItem( oChar );
							}
							else if( _T("m:jc") == sNodeName )
							{
								if( _T("center") == sValue )
									oNewMath->oProperty.moMathParaPr = 1;
								else if( _T("centerGroup") == sValue )
									oNewMath->oProperty.moMathParaPr = 2;
								else if( _T("left") == sValue )
									oNewMath->oProperty.moMathParaPr = 3;
								else if( _T("right") == sValue )
									oNewMath->oProperty.moMathParaPr = 4;
							}
							else if( _T("m:rSp") == sNodeName )
							{
								if( _T("") != sValue )
									oNewMath->oProperty.RowSpacing = Strings::ToInteger( sValue );
							}
							else if( _T("m:rSpRule") == sNodeName )
							{
								if( _T("") != sValue )
									oNewMath->oProperty.RowSpacingRule = Strings::ToInteger( sValue );
							}
							else if( _T("m:cGp") == sNodeName )
							{
								if( _T("") != sValue )
									oNewMath->oProperty.CellGap = Strings::ToInteger( sValue );
							}
							else if( _T("m:cGpRule") == sNodeName )
							{
								if( _T("") != sValue )
									oNewMath->oProperty.CellGapRule = Strings::ToInteger( sValue );
							}
							else if( _T("m:cSp") == sNodeName )
							{
								if( _T("") != sValue )
									oNewMath->oProperty.CellSpacing = Strings::ToInteger( sValue );
							}
							else if( _T("m:cSp") == sNodeName )
							{
								if( _T("") != sValue )
									oNewMath->oProperty.CellSpacing = Strings::ToInteger( sValue );
							}
							else if( _T("m:hideLeft") == sNodeName )
							{
								if( _T("false") != sValue )
									oNewMath->oProperty.HideLeft = true;
							}
							else if( _T("m:hideTop") == sNodeName )
							{
								if( _T("false") != sValue )
									oNewMath->oProperty.HideTop = true;
							}
							else if( _T("m:hideRight") == sNodeName )
							{
								if( _T("false") != sValue )
									oNewMath->oProperty.HideRight = true;
							}
							else if( _T("m:hideBottom") == sNodeName )
							{
								if( _T("false") != sValue )
									oNewMath->oProperty.HideBottom = true;
							}
							else if( _T("m:strikeH") == sNodeName )
							{
								if( _T("false") != sValue )
									oNewMath->oProperty.StrikeHor = true;
							}
							else if( _T("m:strikeV") == sNodeName )
							{
								if( _T("false") != sValue )
									oNewMath->oProperty.StrikeVer = true;
							}
							else if( _T("m:strikeTLBR") == sNodeName )
							{
								if( _T("false") != sValue )
									oNewMath->oProperty.StrikeLR = true;
							}
							else if( _T("m:strikeBLTR") == sNodeName )
							{
								if( _T("false") != sValue )
									oNewMath->oProperty.StrikeRL = true;
							}
							else if( _T("m:aln") == sNodeName )
							{
								if( _T("false") != sValue )
									oNewMath->oProperty.Alignment = true;
							}
							else if( _T("m:diff") == sNodeName )
							{
								if( _T("1") == sValue )
									oNewMath->oProperty.Differential = true;
							}
							else if( _T("m:noBreak") == sNodeName )
							{
								if( _T("1") == sValue )
									oNewMath->oProperty.NoBreak = true;
							}
							else if( _T("m:opEmu") == sNodeName )
							{
								if( _T("1") == sValue )
									oNewMath->oProperty.Emulator = true;
							}
							else if( _T("m:show") == sNodeName )
							{
								if( _T("false") != sValue )
									oNewMath->oProperty.mshow = true;
							}
							else if( _T("m:transp") == sNodeName )
							{
								if( _T("false") != sValue )
									oNewMath->oProperty.mtransp = true;
							}
							else if( _T("m:zeroAsc") == sNodeName )
							{
								if( _T("false") != sValue )
									oNewMath->oProperty.mzeroAsc = true;
							}
							else if( _T("m:zeroDesc") == sNodeName )
							{
								if( _T("false") != sValue )
									oNewMath->oProperty.mzeroDesc = true;
							}
							else if( _T("m:zeroWid") == sNodeName )
							{
								if( _T("false") != sValue )
									oNewMath->oProperty.mzeroWid = true;
							}
						}
						return true;
					}
				return false;
			}
	};
