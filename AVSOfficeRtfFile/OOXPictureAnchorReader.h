#pragma once
#include "OOXPictureGraphicReader.h"
#include "OOXReaderBasic.h"

	class OOXPictureAnchorReader
	{
	public: OOXPictureAnchorReader()
			{
			}
	public: bool Parse( ReaderParameter oParam , RtfShape& oOutput)
			{
				XmlUtils::CXmlReader oXmlReader;
				if( TRUE == oXmlReader.OpenFromXmlNode(oParam.oNode) )
				{
					oOutput.m_oPicture = RtfPicturePtr( new RtfPicture() );
					oOutput.m_eShapeType = RtfShape::st_anchor;
					oOutput.m_nShapeType = 75;
					COMMAND_OOX_BOOL_ATTRIBUTE_2( _T("allowOverlap"), oOutput.m_bAllowOverlap, oXmlReader )
					COMMAND_OOX_BOOL_ATTRIBUTE_2( _T("behindDoc"), oOutput.m_nZOrderRelative, oXmlReader )
					COMMAND_OOX_BOOL_ATTRIBUTE_2( _T("hidden"), oOutput.m_bHidden, oXmlReader )
					COMMAND_OOX_BOOL_ATTRIBUTE_2( _T("layoutInCell"), oOutput.m_bLayoutInCell, oXmlReader )
					COMMAND_OOX_BOOL_ATTRIBUTE_2( _T("locked"), oOutput.m_bLockAnchor, oXmlReader )
					COMMAND_OOX_INT_ATTRIBUTE_2( _T("relativeHeight"), oOutput.m_nZOrder, oXmlReader )
					int nDistLeft = PROP_DEF;
					int nDistTop = PROP_DEF;
					int nDistRight = PROP_DEF;
					int nDistBottom = PROP_DEF;
					COMMAND_OOX_INT_ATTRIBUTE_2( _T("distL"), nDistLeft, oXmlReader )
					COMMAND_OOX_INT_ATTRIBUTE_2( _T("distT"), nDistTop, oXmlReader )
					COMMAND_OOX_INT_ATTRIBUTE_2( _T("distR"), nDistRight, oXmlReader )
					COMMAND_OOX_INT_ATTRIBUTE_2( _T("distB"), nDistBottom, oXmlReader )
					if( PROP_DEF != nDistLeft )
						oOutput.m_nWrapDistLeft = RtfUtility::Emu2Twips( nDistLeft );
					if( PROP_DEF != nDistTop )
						oOutput.m_nWrapDistTop = RtfUtility::Emu2Twips( nDistTop );
					if( PROP_DEF != nDistRight )
						oOutput.m_nWrapDistRight = RtfUtility::Emu2Twips( nDistRight );
					if( PROP_DEF != nDistBottom )
						oOutput.m_nWrapDistBottom = RtfUtility::Emu2Twips( nDistBottom );

					BOOL bSimplePos;
					COMMAND_OOX_BOOL_ATTRIBUTE_2( _T("simplePos"), bSimplePos, oXmlReader )

					int nWidth = PROP_DEF;
					int nHeight = PROP_DEF;
					oXmlReader.ReadNodeList( _T("*") );
					for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
					{
						CString sNodeName = oXmlReader.ReadNodeName( i );
						if( _T("wp:simplePos") == sNodeName )
						{
							int nLeft = PROP_DEF;
							int nTop = PROP_DEF;
							COMMAND_OOX_INT_ATTRIBUTE( _T("x"), oOutput.m_nLeft, oXmlReader, i )
							COMMAND_OOX_INT_ATTRIBUTE( _T("y"), oOutput.m_nTop, oXmlReader, i  )
							if( PROP_DEF != nLeft )
								oOutput.m_nLeft = RtfUtility::Emu2Twips( nLeft );
							if( PROP_DEF != nTop )
								oOutput.m_nTop = RtfUtility::Emu2Twips( nTop );
						}
						else if( _T("wp:extent") == sNodeName )
						{

							COMMAND_OOX_INT_ATTRIBUTE( _T("cx"), nWidth, oXmlReader, i  )
							COMMAND_OOX_INT_ATTRIBUTE( _T("cy"), nHeight, oXmlReader, i  )

							if( PROP_DEF != nWidth && PROP_DEF != nHeight )
							{
								nWidth = RtfUtility::Emu2Twips( nWidth );
								nHeight = RtfUtility::Emu2Twips( nHeight );
								if( PROP_DEF != oOutput.m_nLeft && PROP_DEF != oOutput.m_nTop )
								{
									oOutput.m_nRight = oOutput.m_nLeft + nWidth;
									oOutput.m_nBottom = oOutput.m_nTop + nHeight;
								}
							}
						}
						else if( _T("wp:positionH") == sNodeName )
						{
							CString sRelative = oXmlReader.ReadNodeAttribute( i, _T("relativeFrom") );
							if( _T("margin") == sRelative )
							{
								oOutput.m_nPositionHRelative = 0;
								oOutput.m_eXAnchor = RtfShape::ax_margin;
							}
							else if( _T("page") == sRelative )
							{
								oOutput.m_nPositionHRelative = 1;
								oOutput.m_eXAnchor = RtfShape::ax_page;
							}
							else if( _T("column") == sRelative )
							{
								oOutput.m_nPositionHRelative = 2;
								oOutput.m_eXAnchor = RtfShape::ax_column;
							}
							else if( _T("character") == sRelative )
							{
								oOutput.m_nPositionHRelative = 3;
								oOutput.m_eXAnchor = RtfShape::ax_margin;
							}
							else if( _T("leftMargin") == sRelative )
							{
								oOutput.m_nPositionHRelative = 4;
								oOutput.m_eXAnchor = RtfShape::ax_margin;
							}
							else if( _T("rightMargin") == sRelative )
							{
								oOutput.m_nPositionHRelative = 5;
								oOutput.m_eXAnchor = RtfShape::ax_margin;
							}
							else if( _T("insideMargin") == sRelative )
							{
								oOutput.m_nPositionHRelative = 6;
								oOutput.m_eXAnchor = RtfShape::ax_margin;
							}
							else if( _T("outsideMargin") == sRelative )
							{
								oOutput.m_nPositionHRelative = 7;
								oOutput.m_eXAnchor = RtfShape::ax_margin;
							}
							CString sAlign = oXmlReader.ReadNodeValue(i, _T("wp:align"), _T("") );
							if( _T("") != sAlign )
							{
								if( _T("left") == sAlign )
									oOutput.m_nPositionH = 1;
								else if( _T("center") == sAlign )
									oOutput.m_nPositionH = 2;
								else if( _T("right") == sAlign )
									oOutput.m_nPositionH = 3;
								else if( _T("inside") == sAlign )
									oOutput.m_nPositionH = 4;
								else if( _T("outside") == sAlign )
									oOutput.m_nPositionH = 5;
							}

							CString sOffset = oXmlReader.ReadNodeValue(i, _T("wp:posOffset") );
							if( _T("") != sOffset )
								oOutput.m_nLeft = RtfUtility::Emu2Twips( Strings::ToInteger( sOffset ) );
						}
						else if( _T("wp:positionV") == sNodeName )
						{
							CString sRelative = oXmlReader.ReadNodeAttribute(i, _T("relativeFrom") );
							if( _T("margin") == sRelative )
							{
								oOutput.m_nPositionVRelative = 0;
								oOutput.m_eYAnchor = RtfShape::ay_margin;
							}
							else if( _T("page") == sRelative )
							{
								oOutput.m_nPositionVRelative = 1;
								oOutput.m_eYAnchor = RtfShape::ay_page;
							}
							else if( _T("paragraph") == sRelative )
							{
								oOutput.m_nPositionVRelative = 2;
								oOutput.m_eYAnchor = RtfShape::ay_Para;
							}
							else if( _T("line") == sRelative )
							{
								oOutput.m_nPositionVRelative = 3;
								oOutput.m_eYAnchor = RtfShape::ay_Para;
							}
							else if( _T("topMargin") == sRelative )
							{
								oOutput.m_nPositionVRelative = 4;
								oOutput.m_eYAnchor = RtfShape::ay_margin;
							}
							else if( _T("bottomMargin") == sRelative )
							{
								oOutput.m_nPositionVRelative = 5;
								oOutput.m_eYAnchor = RtfShape::ay_margin;
							}
							else if( _T("insideMargin") == sRelative )
							{
								oOutput.m_nPositionVRelative = 6;
								oOutput.m_eYAnchor = RtfShape::ay_margin;
							}
							else if( _T("outsideMargin") == sRelative )
							{
								oOutput.m_nPositionVRelative = 7;
								oOutput.m_eYAnchor = RtfShape::ay_margin;
							}
							CString sAlign = oXmlReader.ReadNodeValue(i, _T("wp:align"), _T("") );
							if( _T("") != sAlign )
							{
								if( _T("top") == sAlign )
									oOutput.m_nPositionV = 1;
								else if( _T("center") == sAlign )
									oOutput.m_nPositionV = 2;
								else if( _T("bottom") == sAlign )
									oOutput.m_nPositionV = 3;
								else if( _T("inside") == sAlign )
									oOutput.m_nPositionV = 4;
								else if( _T("outside") == sAlign )
									oOutput.m_nPositionV = 5;
							}
							CString sOffset = oXmlReader.ReadNodeValue(i, _T("wp:posOffset") );
							if( _T("") != sOffset )
								oOutput.m_nTop = RtfUtility::Emu2Twips( Strings::ToInteger( sOffset ) );
						}
						else if( _T("wp:wrapNone") == sNodeName )
							oOutput.m_nWrapType = 3;
						else if( _T("wp:wrapSquare") == sNodeName )
						{
							oOutput.m_nWrapType = 2;
							CString sSide = oXmlReader.ReadNodeAttribute( _T("wrapText") );
							if( _T("bothSides") == sSide )
								oOutput.m_nWrapSideType = 0;
							else if( _T("largest") == sSide )
								oOutput.m_nWrapSideType = 3;
							else if( _T("left") == sSide )
								oOutput.m_nWrapSideType = 1;
							else if( _T("right") == sSide )
								oOutput.m_nWrapSideType = 2;
						}
						else if( _T("wp:wrapThrough") == sNodeName )
						{
							oOutput.m_nWrapType = 5;
							CString sSide = oXmlReader.ReadNodeAttribute( _T("wrapText") );
							if( _T("bothSides") == sSide )
								oOutput.m_nWrapSideType = 0;
							else if( _T("largest") == sSide )
								oOutput.m_nWrapSideType = 3;
							else if( _T("left") == sSide )
								oOutput.m_nWrapSideType = 1;
							else if( _T("right") == sSide )
								oOutput.m_nWrapSideType = 2;

							XML::IXMLDOMNodePtr oNode;
							oXmlReader.GetNode( i, oNode);
							XmlUtils::CXmlReader oXmlSubReader;
							oXmlSubReader.OpenFromXmlNode( oNode );
							if( TRUE == oXmlSubReader.ReadNode( _T("wp:wrapPolygon") ) )
							{
								oXmlSubReader.ReadNodeList( _T("*") );
								for( int i = 0; i < oXmlSubReader.GetLengthList(); i++ )
								{
									int nValue;
									COMMAND_OOX_INT_ATTRIBUTE_2( _T("x"), nValue, oXmlSubReader );
									oOutput.m_aWrapPoints.Add( nValue );
									COMMAND_OOX_INT_ATTRIBUTE_2( _T("y"), nValue, oXmlSubReader );
									oOutput.m_aWrapPoints.Add( nValue );
								}
							}
						}
						else if( _T("wp:wrapTight") == sNodeName )
						{
							oOutput.m_nWrapType = 4;
							CString sSide = oXmlReader.ReadNodeAttribute( _T("wrapText") );
							if( _T("bothSides") == sSide )
								oOutput.m_nWrapSideType = 0;
							else if( _T("largest") == sSide )
								oOutput.m_nWrapSideType = 3;
							else if( _T("left") == sSide )
								oOutput.m_nWrapSideType = 1;
							else if( _T("right") == sSide )
								oOutput.m_nWrapSideType = 2;

							XML::IXMLDOMNodePtr oNode;
							oXmlReader.GetNode( i, oNode);
							XmlUtils::CXmlReader oXmlSubReader;
							oXmlSubReader.OpenFromXmlNode( oNode );
							if( TRUE == oXmlSubReader.ReadNode( _T("wrapPolygon") ) )
							{
								oXmlSubReader.ReadNodeList( _T("*") );
								for( int i = 0; i < oXmlSubReader.GetLengthList(); i++ )
								{
									int nValue;
									COMMAND_OOX_INT_ATTRIBUTE_2( _T("x"), nValue, oXmlSubReader );
									oOutput.m_aWrapPoints.Add( nValue );
									COMMAND_OOX_INT_ATTRIBUTE_2( _T("y"), nValue, oXmlSubReader );
									oOutput.m_aWrapPoints.Add( nValue );
								}
							}
						}
						else if( _T("wp:wrapTopAndBottom") == sNodeName )
							oOutput.m_nWrapType = 1;
						else if( _T("a:graphic") == sNodeName )
						{
							XML::IXMLDOMNodePtr oNode;
							oXmlReader.GetNode(i, oNode);
							ReaderParameter oNewParam = oParam;
							oNewParam.oNode = oNode;
							OOXPictureGraphicReader oGraphicReader;
							oGraphicReader.Parse( oNewParam, *oOutput.m_oPicture );
						}
					}
					//изменяем scale в соответсявии с выходным размером
					if( PROP_DEF != nWidth && PROP_DEF != oOutput.m_oPicture->m_nWidthGoal )
					{
						int nNewScale = (int)(100 * ( 1.0 * nWidth / oOutput.m_oPicture->m_nWidthGoal ));
						oOutput.m_oPicture->m_dScaleX = nNewScale;
					}
					if( PROP_DEF != nHeight && PROP_DEF != oOutput.m_oPicture->m_nHeightGoal )
					{
						int nNewScale = (int)(100 * ( 1.0 * nHeight / oOutput.m_oPicture->m_nHeightGoal ));
						oOutput.m_oPicture->m_dScaleY = nNewScale;
					}
				}

				return true;
			}
	};
