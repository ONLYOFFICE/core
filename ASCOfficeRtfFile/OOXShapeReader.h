#pragma once
#include "RtfDocument.h"
#include "XmlUtils.h"
#include "RtfShape.h"
#include "OOXPictureGraphicReader.h"
#include "OOXReaderBasic.h"

	class OOXShapeReader
	{
	public: OOXShapeReader()
			{
			}
	public: bool Parse( ReaderParameter oParam , RtfShapePtr& oOutput);
	private: void ParseAdjustment(RtfShape& oShape, CString sAdjustment)
			 {
				 int nPosition = 0;
				 CString sAdj = sAdjustment.Tokenize(_T(","), nPosition);
				 if( _T("") == sAdj )
					 return;
				 oShape.m_nAdjustValue = Strings::ToInteger( sAdj );
				 sAdj = sAdjustment.Tokenize(_T(","), nPosition);
				 if( _T("") == sAdj )
					 return;
				 oShape.m_nAdjustValue2 = Strings::ToInteger( sAdj );
				 sAdj = sAdjustment.Tokenize(_T(","), nPosition);
				 if( _T("") == sAdj )
					 return;
				 oShape.m_nAdjustValue3 = Strings::ToInteger( sAdj );
				 sAdj = sAdjustment.Tokenize(_T(","), nPosition);
				 if( _T("") == sAdj )
					 return;
				 oShape.m_nAdjustValue4 = Strings::ToInteger( sAdj );
				 sAdj = sAdjustment.Tokenize(_T(","), nPosition);
				 if( _T("") == sAdj )
					 return;
				 oShape.m_nAdjustValue5 = Strings::ToInteger( sAdj );
				 sAdj = sAdjustment.Tokenize(_T(","), nPosition);
				 if( _T("") == sAdj )
					 return;
				 oShape.m_nAdjustValue6 = Strings::ToInteger( sAdj );
				 sAdj = sAdjustment.Tokenize(_T(","), nPosition);
				 if( _T("") == sAdj )
					 return;
				 oShape.m_nAdjustValue7 = Strings::ToInteger( sAdj );
				 sAdj = sAdjustment.Tokenize(_T(","), nPosition);
				 if( _T("") == sAdj )
					 return;
				 oShape.m_nAdjustValue8 = Strings::ToInteger( sAdj );
				 sAdj = sAdjustment.Tokenize(_T(","), nPosition);
				 if( _T("") == sAdj )
					 return;
				 oShape.m_nAdjustValue9 = Strings::ToInteger( sAdj );
				 sAdj = sAdjustment.Tokenize(_T(","), nPosition);
				 if( _T("") == sAdj )
					 return;
				 oShape.m_nAdjustValue10 = Strings::ToInteger( sAdj );
			 }

	private: bool ParseStyles(RtfShape& oShape, CString sStyles)
		{
			int nPosition = 0;
			CString sStyle =  _T("start");
			while( _T("") != sStyle )
			{
				sStyle = sStyles.Tokenize( _T(";"), nPosition);
				ParseStyle( oShape, sStyle );
			}
			return true;
		}
	private: bool ParseStyle(RtfShape& oShape, CString sStyle)
		{
			int nDelimiter = sStyle.Find(':');
			CString sProperty = sStyle.Left( nDelimiter );
			CString sValue = sStyle.Right( sStyle.GetLength() - nDelimiter - 1 );
			if(  _T("margin-left") == sProperty )
				oShape.m_nLeft = RtfUtility::String2Twips( sValue );
			else if(  _T("margin-top") == sProperty )
				oShape.m_nTop = RtfUtility::String2Twips( sValue );
			else if(  _T("margin-bottom") == sProperty )
				oShape.m_nRight = RtfUtility::String2Twips( sValue );
			else if(  _T("margin-right") == sProperty )
				oShape.m_nBottom = RtfUtility::String2Twips( sValue );
			else if(  _T("left") == sProperty )
				oShape.m_nRelLeft = Strings::ToInteger( sValue );
			else if(  _T("right") == sProperty )
				oShape.m_nRelRight = Strings::ToInteger( sValue );
			else if(  _T("bottom") == sProperty )
				oShape.m_nRelBottom = Strings::ToInteger( sValue );
			else if(  _T("right") == sProperty )
				oShape.m_nRelRight = Strings::ToInteger( sValue );
			else if(  _T("width") == sProperty )
			{
				int nWidth = RtfUtility::String2Twips( sValue );
				if( PROP_DEF != oShape.m_nLeft )
					oShape.m_nRight = oShape.m_nLeft + nWidth;
				if( PROP_DEF != oShape.m_nRelLeft)
					oShape.m_nRelRight = oShape.m_nRelLeft + nWidth;
			}
			else if(  _T("height") == sProperty )
			{
				int nHeight = RtfUtility::String2Twips( sValue );
				if( PROP_DEF != oShape.m_nTop )
					oShape.m_nBottom = oShape.m_nTop + nHeight;
				if( PROP_DEF != oShape.m_nRelTop)
					oShape.m_nRelBottom = oShape.m_nRelTop + nHeight;
			}
			else if(  _T("mso-position-horizontal") == sProperty )
			{
				if( _T("absolute") == sValue )
					oShape.m_nPositionH = 0;
				else if( _T("left") == sValue )
					oShape.m_nPositionH = 1;
				else if( _T("center") == sValue )
					oShape.m_nPositionH = 2;
				else if( _T("right") == sValue )
					oShape.m_nPositionH = 3;
				else if( _T("inside") == sValue )
					oShape.m_nPositionH = 4;
				else if( _T("outside") == sValue )
					oShape.m_nPositionH = 5;
			}
			else if(  _T("mso-left-percent") == sProperty )
				oShape.m_nPositionHPct = Strings::ToInteger( sValue );
			else if(  _T("mso-position-horizontal-relative") == sProperty )
			{
				if( _T("margin") == sValue )
					oShape.m_nPositionHRelative = 0;
				else if( _T("page") == sValue )
					oShape.m_nPositionHRelative = 1;
				else if( _T("text") == sValue )
					oShape.m_nPositionHRelative = 2;
				else if( _T("char") == sValue )
					oShape.m_nPositionHRelative = 3;
				else if( _T("left-margin-area") == sValue )
					oShape.m_nPositionHRelative = 4;
				else if( _T("right-margin-area") == sValue )
					oShape.m_nPositionHRelative = 5;
				else if( _T("inner-margin-area") == sValue )
					oShape.m_nPositionHRelative = 6;
				else if( _T("outer-margin-area") == sValue )
					oShape.m_nPositionHRelative = 7;
			}
			else if(  _T("mso-position-horizontal-relative") == sProperty )
			{
				if( _T("page") == sValue )
					oShape.m_eXAnchor = RtfShape::ax_page;
				else if( _T("margin") == sValue )
					oShape.m_eXAnchor = RtfShape::ax_margin;
				else if( _T("text") == sValue )
					oShape.m_eXAnchor = RtfShape::ax_column;
			}
			else if(  _T("mso-position-vertical") == sProperty )
			{
				if( _T("absolute") == sValue )
					oShape.m_nPositionV = 0;
				else if( _T("top") == sValue )
					oShape.m_nPositionV = 1;
				else if( _T("center") == sValue )
					oShape.m_nPositionV = 2;
				else if( _T("bottom") == sValue )
					oShape.m_nPositionV = 3;
				else if( _T("inside") == sValue )
					oShape.m_nPositionV = 4;
				else if( _T("outside") == sValue )
					oShape.m_nPositionV = 5;
			}
			else if(  _T("mso-top-percent") == sProperty )
				oShape.m_nPositionVPct = Strings::ToInteger( sValue );
			else if(  _T("mso-position-vertical-relative") == sProperty )
			{
				if( _T("margin") == sValue )
					oShape.m_nPositionVRelative = 0;
				else if( _T("page") == sValue )
					oShape.m_nPositionVRelative = 1;
				else if( _T("text") == sValue )
					oShape.m_nPositionVRelative = 2;
				else if( _T("line") == sValue )
					oShape.m_nPositionVRelative = 3;
				else if( _T("top-margin-area") == sValue )
					oShape.m_nPositionVRelative = 4;
				else if( _T("bottom-margin-area") == sValue )
					oShape.m_nPositionVRelative = 5;
				else if( _T("inner-margin-area") == sValue )
					oShape.m_nPositionVRelative = 6;
				else if( _T("outer-margin-area") == sValue )
					oShape.m_nPositionVRelative = 7;
			}
			else if(  _T("mso-position-vertical-relative") == sProperty )
			{
				if( _T("page") == sValue )
					oShape.m_eYAnchor = RtfShape::ay_page;
				else if( _T("margin") == sValue )
					oShape.m_eYAnchor = RtfShape::ay_margin;
				else if( _T("text") == sValue )
					oShape.m_eYAnchor = RtfShape::ay_margin;
			}
			else if(  _T("mso-width-relative") == sProperty )
			{
				if( _T("margin") == sValue )
					oShape.m_nPctWidthRelative = 0;
				else if( _T("page") == sValue )
					oShape.m_nPctWidthRelative = 1;
				else if( _T("left-margin-area") == sValue )
					oShape.m_nPctWidthRelative = 2;
				else if( _T("right-margin-area") == sValue )
					oShape.m_nPctWidthRelative = 3;
				else if( _T("inner-margin-area") == sValue )
					oShape.m_nPctWidthRelative = 4;
				else if( _T("outer-margin-area") == sValue )
					oShape.m_nPctWidthRelative = 5;

			}
			else if(  _T("mso-height-relative") == sProperty )
			{
				if( _T("margin") == sValue )
					oShape.m_nPctHeightRelative = 0;
				else if( _T("page") == sValue )
					oShape.m_nPctHeightRelative = 1;
				else if( _T("top-margin-area") == sValue )
					oShape.m_nPctHeightRelative = 2;
				else if( _T("bottom-margin-area") == sValue )
					oShape.m_nPctHeightRelative = 3;
				else if( _T("inner-margin-area") == sValue )
					oShape.m_nPctHeightRelative = 4;
				else if( _T("outer-margin-area") == sValue )
					oShape.m_nPctHeightRelative = 5;
			}
			else if( _T("rotation") == sProperty )
			{
				int nRotation = Strings::ToInteger( sValue );
				oShape.m_nRotation = 65536 * nRotation;
				if( PROP_DEF != oShape.m_nRelRight || PROP_DEF != oShape.m_nRelLeft || PROP_DEF != oShape.m_nRelTop || PROP_DEF != oShape.m_nRelBottom  )
					oShape.m_nRelRotation = oShape.m_nRotation;
			}
			else if( _T("z-index") == sProperty )
			{
				int nValue = Strings::ToInteger( sValue );
				oShape.m_nZOrder = nValue;
				if( nValue > 0 )
					oShape.m_nZOrderRelative = 0;
				else if( nValue < 0 )
					oShape.m_nZOrderRelative = 1;
			}
			else if(  _T("flip") == sProperty )
			{
				if( PROP_DEF != sValue.Find('x') )
					oShape.m_bFlipH = true;
				if( PROP_DEF != sValue.Find('y') )
					oShape.m_bFlipV = true;
			}
			else if(  _T("mso-wrap-distance-left") == sProperty )
				oShape.m_nWrapDistLeft = RtfUtility::String2Twips( sValue );
			else if(  _T("mso-wrap-distance-top") == sProperty )
				oShape.m_nWrapDistTop = RtfUtility::String2Twips( sValue );
			else if(  _T("mso-wrap-distance-right") == sProperty )
				oShape.m_nWrapDistRight = RtfUtility::String2Twips( sValue );
			else if(  _T("mso-wrap-distance-bottom") == sProperty )
				oShape.m_nWrapDistBottom = RtfUtility::String2Twips( sValue );
			return true;
		}
	private: int GetType( CString sType )
			 {
				 int nResult = -1;
				 int nStartIndex = 0;
				 CString sToken = sType.Tokenize( _T("_"), nStartIndex);
				 while( _T("") != sToken )
				 {
					 if( sToken.GetLength() > 0 && 't' == sToken[0] )
					 {
						 sToken = sToken.Right( sToken.GetLength() - 1 );
						 nResult = Strings::ToInteger( sToken );
						 break;
					 }
					 sToken = sType.Tokenize( _T("_"), nStartIndex);
				 }
				 return nResult;
			 }
	};


class OOXShapeGroupReader
{
public: OOXShapeGroupReader()
		{
		}
public: bool Parse( ReaderParameter oParam , RtfShapeGroupPtr& oOutput)
		{
			XmlUtils::CXmlReader oXmlReader;
			if( TRUE == oXmlReader.OpenFromXmlNode(oParam.oNode) )
			{
				OOXShapeReader oShapeReader;
				RtfShapePtr oBaseShape = boost::shared_static_cast<RtfShape, RtfShapeGroup>( oOutput );
				oShapeReader.Parse( oParam, oBaseShape );
				oXmlReader.ReadNodeList( _T("*") );
				for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
				{
					CString sNodeName = oXmlReader.ReadNodeName( i );
					XML::IXMLDOMNodePtr oNode;
					oXmlReader.GetNode( i, oNode);
					ReaderParameter oNewParam = oParam;
					oNewParam.oNode = oNode;
					if( _T("v:shape") == sNodeName )
					{
						RtfShapePtr oNewShape( new RtfShape() );
						OOXShapeReader oShapeReader;
						if( true == oShapeReader.Parse( oNewParam, oNewShape ) )
							 oOutput->AddItem( oNewShape );
					}
					else if( _T("v:group") == sNodeName )
					{
						RtfShapeGroupPtr oNewShape( new RtfShapeGroup() );
						OOXShapeGroupReader oShapeReader;
						if( true == oShapeReader.Parse( oNewParam, oNewShape ) )
							 oOutput->AddItem( oNewShape );
					}
				}
			}
			return true;
		}
};