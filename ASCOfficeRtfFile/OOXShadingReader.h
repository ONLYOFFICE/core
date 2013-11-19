#pragma once

	class OOXShadingReader
	{
	public: OOXShadingReader()
			{
			}
	public: bool Parse( ReaderParameter oParam, RtfShading& oOutputShading )
			{	
				XmlUtils::CXmlReader oXmlReader;
				if( TRUE == oXmlReader.OpenFromXmlNode( oParam.oNode ) )
				{
					CString sXml = oXmlReader.ReadNodeXml();
					RtfColor oShadingColor;
					RtfColor oShadingFillColor;
					CString sColor = oXmlReader.ReadNodeAttribute( _T("w:color") );
					CharLower(sColor.GetBuffer() );
					sColor.ReleaseBuffer();
					if( _T("") != sColor && _T("auto") != sColor && sColor.GetLength() == 6 )
					{
						RtfColor oColor;
						oColor.SetHEXString( sColor );
						oOutputShading.m_nForeColor = oParam.oDocument->m_oColorTable.AddItem( oColor );
					}
					CString sFill = oXmlReader.ReadNodeAttribute( _T("w:fill") );
					sFill.MakeLower();
					if( _T("") != sFill && _T("auto") != sFill && sFill.GetLength() == 6 )
					{
						RtfColor oColor;
						oColor.SetHEXString( sFill );
						oOutputShading.m_nBackColor = oParam.oDocument->m_oColorTable.AddItem( oColor );
					}

					CString sType = oXmlReader.ReadNodeAttribute( _T("w:val"), _T("") );
					if( -1 != sType.Find(_T("pct") ) )
					{
						sType.Replace( _T("pct"), _T("") );
						oOutputShading.m_nValue = Strings::ToInteger( sType );
					}
					else if( _T("clear") == sType )
						oOutputShading.m_nValue = 0;
					else if( _T("solid") == sType )
						oOutputShading.m_nValue = 100;
					else if( _T("thinHorzStripehorzStripe") == sType )
						oOutputShading.m_eType = RtfShading::st_chbghoriz;
					else if( _T("thinVertStripe") == sType )
						oOutputShading.m_eType = RtfShading::st_chbgvert;
					else if( _T("thinReverseDiagStripe") == sType )
						oOutputShading.m_eType = RtfShading::st_chbgfdiag;
					else if( _T("thinDiagStripe") == sType )
						oOutputShading.m_eType = RtfShading::st_chbgbdiag;
					else if( _T("thinHorzCross") == sType )
						oOutputShading.m_eType = RtfShading::st_chbgcross;
					else if( _T("thinDiagCross") == sType )
						oOutputShading.m_eType = RtfShading::st_chbgdcross;
					else if( _T("horzStripe") == sType )
						oOutputShading.m_eType = RtfShading::st_chbgdkhoriz;
					else if( _T("vertStripe") == sType )
						oOutputShading.m_eType = RtfShading::st_chbgdkvert;
					else if( _T("reverseDiagStripe") == sType )
						oOutputShading.m_eType = RtfShading::st_chbgdkfdiag;
					else if( _T("diagStripe") == sType )
						oOutputShading.m_eType = RtfShading::st_chbgdkbdiag;
					else if( _T("horzCross") == sType )
						oOutputShading.m_eType = RtfShading::st_chbgdkcross;
					else if( _T("diagCross") == sType )
						oOutputShading.m_eType = RtfShading::st_chbgdkdcross;
				}
				return true;
			}
	};
