#include "stdafx.h"
#include "OOXShapeReader.h"
#include "OOXTextItemReader.h"

bool OOXShapeReader::Parse( ReaderParameter oParam , RtfShapePtr& oOutput)
	{
		//if( oParam.oReader->m_nCurItap != 0 )
		//	oOutput->m_bLayoutInCell = 1;
		XmlUtils::CXmlReader oXmlReader;
		if( TRUE == oXmlReader.OpenFromXmlNode(oParam.oNode) )
		{
				CString sType = oXmlReader.ReadNodeAttribute( _T("type"), _T("") );
				int nType = GetType( sType );
				if( -1 != nType )
					oOutput->m_nShapeType = nType;
				CString sId = oXmlReader.ReadNodeAttribute( _T("id"), _T("") );
				if( _T("") != sId )
					oOutput->m_nID = oParam.oReader->m_oOOXIdGenerator.GetId( sId );

				oOutput->m_nLeft = 0; //стили только с widht height (например в Numbering)
				oOutput->m_nTop = 0;
				CString sStyle = oXmlReader.ReadNodeAttribute( _T("style"), _T("") );
				if( false == ParseStyles( *oOutput, sStyle ) )
					return false;

				CString sAdjustment = oXmlReader.ReadNodeAttribute( _T("adj"), _T("") );
				ParseAdjustment( *oOutput, sAdjustment );

				CString sConnectionType = oXmlReader.ReadNodeAttribute( _T("o:connecttype"), _T("") );
				if( _T("custom") == sConnectionType )
					oOutput->m_nConnectionType = 0;
				else if( _T("none") == sConnectionType )
					oOutput->m_nConnectionType = 1;
				else if( _T("rect") == sConnectionType )
					oOutput->m_nConnectionType = 2;
				else if( _T("segments") == sConnectionType )
					oOutput->m_nConnectionType = 3;

				CString sConnectorStyle = oXmlReader.ReadNodeAttribute( _T("o:connectortype"), _T("") );
				if( _T("straight") == sConnectorStyle )
					oOutput->m_nConnectionType = 0;
				else if( _T("elbow") == sConnectorStyle )
					oOutput->m_nConnectionType = 1;
				else if( _T("curved") == sConnectorStyle )
					oOutput->m_nConnectionType = 2;
				else if( _T("none") == sConnectorStyle )
					oOutput->m_nConnectionType = 3;

				COMMAND_OOX_BOOL_ATTRIBUTE_2( _T("o:allowincell"), oOutput->m_bLayoutInCell, oXmlReader )
				COMMAND_OOX_BOOL_ATTRIBUTE_2( _T("o:allowoverlap"), oOutput->m_bAllowOverlap, oXmlReader )

				CString sWrapcoords = oXmlReader.ReadNodeAttribute( _T("wrapcoords"), _T("") );
				if( _T("") != sWrapcoords )
				{
					int nPosition = 0;
					CString sPoint =  _T("start");
					while( _T("") != sPoint )
					{
						sPoint = sWrapcoords.Tokenize( _T(" "), nPosition);
						oOutput->m_aWrapPoints.Add( Strings::ToInteger( sPoint ) );
					}
				}

				CString sCoordorigin = oXmlReader.ReadNodeAttribute( _T("coordorigin"), _T("") );
				if( _T("") != sCoordorigin )
				{
					int nDelimiter = sCoordorigin.Find(',');
					oOutput->m_nGroupLeft = Strings::ToInteger( sCoordorigin.Left( nDelimiter ) );
					oOutput->m_nGroupTop = Strings::ToInteger( sCoordorigin.Right( nDelimiter ) );
				}

				CString sCoordsize = oXmlReader.ReadNodeAttribute( _T("coordsize"), _T("") );
				if( _T("") != sCoordsize )
				{
					int nDelimiter = sCoordsize.Find(',');
					oOutput->m_nGroupRight = oOutput->m_nGroupLeft + Strings::ToInteger( sCoordsize.Left( nDelimiter ) );
					oOutput->m_nGroupBottom = oOutput->m_nGroupTop + Strings::ToInteger( sCoordsize.Right( nDelimiter ) );
				}

				oXmlReader.ReadNodeList( _T("*") );
				for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
				{
					CString sNodeName = oXmlReader.ReadNodeName( i );
					if( _T("v:imagedata") == sNodeName )
					{
						oOutput->m_oPicture = RtfPicturePtr( new RtfPicture() );
						oOutput->m_eShapeType = RtfShape::st_none;
						oOutput->m_nShapeType = 75;


						CString srId = oXmlReader.ReadNodeAttribute(i, _T("r:id") );
						CString sImagePath = oParam.oReader->m_sDocumentPath + _T("\\") + oParam.oRels->GetByID( srId );
						if( ::GetFileAttributes( sImagePath ) != DWORD(-1) ) //проверяем существует ли файл
						{
							OOXPictureGraphicReader::WriteDataToPicture( sImagePath, *oOutput->m_oPicture, oParam.oReader->m_sDocumentPath );
						}
						int nCropedWidthGoal = oOutput->m_oPicture->m_nWidthGoal;
						if( PROP_DEF != nCropedWidthGoal )
						{
							//делаем crop
							CString sCropLeft = oXmlReader.ReadNodeAttribute(i, _T("cropleft") );
							CString sCropRight = oXmlReader.ReadNodeAttribute(i, _T("cropright") );
							if( _T("") != sCropLeft )
							{
								float nCropLeft = RtfUtility::String2Percent( sCropLeft );
								//oOutput->m_oPicture->m_nCropL = nCropLeft * oOutput->m_oPicture->m_nWidthGoal * oOutput->m_oPicture->m_nScaleX / 100;
								oOutput->m_oPicture->m_nCropL = (int)(nCropLeft * oOutput->m_oPicture->m_nWidthGoal);
								oOutput->m_nCropFromLeft = (int)(nCropLeft * 65536);
								nCropedWidthGoal -= oOutput->m_oPicture->m_nCropL;
							}
							if( _T("") != sCropRight )
							{
								float nCropRight = RtfUtility::String2Percent( sCropRight );
								//oOutput->m_oPicture->m_nCropR = nCropRight * oOutput->m_oPicture->m_nWidthGoal * oOutput->m_oPicture->m_nScaleX / 100;
								oOutput->m_oPicture->m_nCropR = (int)(nCropRight * oOutput->m_oPicture->m_nWidthGoal);
								oOutput->m_nCropFromRight = (int)(nCropRight * 65536);
								nCropedWidthGoal -= oOutput->m_oPicture->m_nCropR;
							}
						}
						int nCropedHeightGoal = oOutput->m_oPicture->m_nHeightGoal;
						if( PROP_DEF != nCropedHeightGoal )
						{
							CString sCropTop = oXmlReader.ReadNodeAttribute(i, _T("croptop") );
							CString sCropBottom = oXmlReader.ReadNodeAttribute(i, _T("cropbottom") );
							if( _T("") != sCropTop )
							{
								float nCropTop = RtfUtility::String2Percent( sCropTop );
								//oOutput->m_oPicture->m_nCropT = nCropTop * oOutput->m_oPicture->m_nHeightGoal * oOutput->m_oPicture->m_dScaleY / 100;
								oOutput->m_oPicture->m_nCropT = (int)(nCropTop * oOutput->m_oPicture->m_nHeightGoal);
								oOutput->m_nCropFromTop = (int)(nCropTop * 65536);
								nCropedHeightGoal -= oOutput->m_oPicture->m_nCropT;
							}
							if( _T("") != sCropBottom )
							{
								float nCropBottom = RtfUtility::String2Percent( sCropBottom );
								//oOutput->m_oPicture->m_nCropT = nCropTop * oOutput->m_oPicture->m_nHeightGoal * oOutput->m_oPicture->m_dScaleY / 100;
								oOutput->m_oPicture->m_nCropB = (int)(nCropBottom * oOutput->m_oPicture->m_nHeightGoal);
								oOutput->m_nCropFromBottom = (int)(nCropBottom * 65536);
								nCropedHeightGoal -= oOutput->m_oPicture->m_nCropB;
							}
						}
						//устанавливаем scale
						if( PROP_DEF != oOutput->m_nLeft && PROP_DEF != oOutput->m_nRight && PROP_DEF != nCropedWidthGoal && 0 != nCropedWidthGoal )
						{
							int nWidth = oOutput->m_nRight - oOutput->m_nLeft;
							double dNewScale = 100 * ( 1.0 * nWidth / nCropedWidthGoal );
							oOutput->m_oPicture->m_dScaleX = dNewScale;
						}
						if( PROP_DEF != oOutput->m_nTop && PROP_DEF != oOutput->m_nBottom && PROP_DEF != nCropedHeightGoal && 0 != nCropedHeightGoal )
						{
							int nHeight = oOutput->m_nBottom - oOutput->m_nTop;
							double dNewScale = 100 * ( 1.0 * nHeight / nCropedHeightGoal );
							oOutput->m_oPicture->m_dScaleY = dNewScale;
						}


						//проверяем на inline
						if( (PROP_DEF == oOutput->m_nLeft || 0 == oOutput->m_nLeft ) && ( PROP_DEF == oOutput->m_nTop || 0 == oOutput->m_nTop ) && PROP_DEF == oOutput->m_nPositionH && PROP_DEF == oOutput->m_nPositionV )
							oOutput->m_eShapeType = RtfShape::st_inline;
					}
					else if( _T("w10:wrap") == sNodeName )
					{
						CString sType = oXmlReader.ReadNodeAttribute( i, _T("type") );
						CString sSide = oXmlReader.ReadNodeAttribute( i, _T("side") );
						if( _T("none") == sType )
							oOutput->m_nWrapType = 3;
						else if( _T("topAndBottom") == sType )
							oOutput->m_nWrapType = 1;
						else if( _T("through") == sType )
						{
							oOutput->m_nWrapType = 5;
							CString sSide = oXmlReader.ReadNodeAttribute( _T("wrapText") );
							if( _T("bothSides") == sSide )
								oOutput->m_nWrapSideType = 0;
							else if( _T("largest") == sSide )
								oOutput->m_nWrapSideType = 3;
							else if( _T("left") == sSide )
								oOutput->m_nWrapSideType = 1;
							else if( _T("right") == sSide )
								oOutput->m_nWrapSideType = 2;
						}
						else if( _T("tight") == sType )
						{
							oOutput->m_nWrapType = 2;
							if( _T("both") == sSide )
								oOutput->m_nWrapSideType = 0;
							else if( _T("largest") == sSide )
								oOutput->m_nWrapSideType = 3;
							else if( _T("left") == sSide )
								oOutput->m_nWrapSideType = 1;
							else if( _T("right") == sSide )
								oOutput->m_nWrapSideType = 2;
						}
						else if( _T("square") == sType )
						{
							oOutput->m_nWrapType = 2;
							if( _T("both") == sSide )
								oOutput->m_nWrapSideType = 0;
							else if( _T("largest") == sSide )
								oOutput->m_nWrapSideType = 3;
							else if( _T("left") == sSide )
								oOutput->m_nWrapSideType = 1;
							else if( _T("right") == sSide )
								oOutput->m_nWrapSideType = 2;
						}
					}
					else if( _T("v:textbox") == sNodeName )
					{
						XML::IXMLDOMNodePtr oSubNode;
						oXmlReader.GetNode( i, oSubNode);
						XmlUtils::CXmlReader oXmlSubReader;
						if( TRUE == oXmlSubReader.OpenFromXmlNode( oSubNode ) )
							if( TRUE == oXmlSubReader.ReadNode( _T("w:txbxContent") ) )
							{
								XML::IXMLDOMNodePtr oSubSubNode;
								oXmlSubReader.GetNode( oSubSubNode);
								ReaderParameter oNewParam = oParam;
								oNewParam.oNode = oSubSubNode;

								OOXTextItemReader oTextItemReader;
								oTextItemReader.Parse( oNewParam );
								if( oTextItemReader.m_oTextItems->GetCount() > 0 )
									oOutput->m_aTextItems = oTextItemReader.m_oTextItems;
							}
					}
				}
			}
		//если ничего не задали делаем inline
		if( (PROP_DEF == oOutput->m_nLeft || 0 == oOutput->m_nLeft ) && ( PROP_DEF == oOutput->m_nTop || 0 == oOutput->m_nTop ) && PROP_DEF == oOutput->m_nPositionH && PROP_DEF == oOutput->m_nPositionV )
		{
			oOutput->m_nLeft = 0;
			oOutput->m_nTop = 0;
			oOutput->m_nPositionHRelative = 3;
			oOutput->m_nPositionVRelative = 3;
		}
		return true;
	}