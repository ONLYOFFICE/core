#pragma once
#include "OOXPictureGraphicReader.h"

	class OOXPictureInlineReader
	{
	public: OOXPictureInlineReader()
			{
			}
	public: bool Parse( ReaderParameter oParam, RtfShape& oOutput)
			{
				XmlUtils::CXmlReader oXmlReader;
				if( TRUE == oXmlReader.OpenFromXmlNode(oParam.oNode) )
					if( TRUE == oXmlReader.ReadNodeList( _T("*") ) )
					{
						oOutput.m_oPicture = RtfPicturePtr( new RtfPicture() );
						oOutput.m_eShapeType = RtfShape::st_inline;
						oOutput.m_nShapeType = 75;
						oOutput.m_nLeft = 0;
						oOutput.m_nTop = 0;
						oOutput.m_nPositionHRelative = 3;
						oOutput.m_nPositionVRelative = 3;
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

						int nWidth = PROP_DEF;
						int nHeight = PROP_DEF;
						for(int i=0;i< oXmlReader.GetLengthList();i++)
						{
							CString sNodeName = oXmlReader.ReadNodeName(i);
							if( _T("wp:extent") == sNodeName )
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
									oOutput.m_oPicture->m_nWidthGoal = nWidth;
									oOutput.m_oPicture->m_nHeightGoal = nHeight;
								}
							}
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
							double dNewScale = 100 * ( 1.0 * nWidth / oOutput.m_oPicture->m_nWidthGoal );
							oOutput.m_oPicture->m_dScaleX = dNewScale;
						}
						if( PROP_DEF != nHeight && PROP_DEF != oOutput.m_oPicture->m_nHeightGoal )
						{
							double dNewScale = 100 * ( 1.0 * nHeight / oOutput.m_oPicture->m_nHeightGoal );
							oOutput.m_oPicture->m_dScaleY = dNewScale;
						}
						return true;
					}
				return false;
			}
	};
