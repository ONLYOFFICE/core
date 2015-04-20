#include "RtfShape.h"
#include "Writer/OOXWriter.h"
#include "RtfOle.h"

CString RtfShape::RenderToRtf(RenderParameter oRenderParameter)
		{
			CString sResult;
			if( PROP_DEF != m_nShapeType && 0 != m_nShapeType )
			{
				//запоминаем координаты и если нужно поворачиваем
				int nLeft = m_nLeft;
				int nTop = m_nTop;
				int nRight = m_nRight;
				int nBottom = m_nBottom;
				int nRotate = m_nRotation;

				int nRelLeft = m_nRelLeft;
				int nRelTop = m_nRelTop;
				int nRelRight = m_nRelRight;
				int nRelBottom = m_nRelBottom;
				int nRelRotate = m_nRelRotation;

				if( PROP_DEF != m_nRotation && PROP_DEF != m_nLeft && PROP_DEF != m_nTop &&
					PROP_DEF != m_nRight && PROP_DEF != m_nBottom )
					ToRtfRotation( m_nRotation, m_nLeft, m_nTop, m_nRight, m_nBottom );

				if( PROP_DEF != m_nRelRotation && PROP_DEF != m_nRelLeft && PROP_DEF != m_nRelTop &&
					PROP_DEF != m_nRelRight && PROP_DEF != m_nRelBottom )
					ToRtfRotation( m_nRelRotation, m_nRelLeft, m_nRelTop, m_nRelRight, m_nRelBottom );

				if( st_inline == m_eShapeType )
				{
					if( NULL != m_oPicture )
					{
						if( RtfPicture::dt_wmf == m_oPicture->eDataType )
							sResult.Append( m_oPicture->RenderToRtf( oRenderParameter ) );
						else
						{
							sResult.Append( _T("{\\*\\shppict") );
							//CString sPictProp = RenderToRtfShapeProperty( oRenderParameter );
							//if( false == sPictProp.IsEmpty() )
							//	sResult.AppendFormat( _T("{\\*\\picprop %ls}"), sPictProp );
							
							sResult.Append( m_oPicture->RenderToRtf( oRenderParameter ) );
							sResult.Append( _T("}") );
							sResult.Append( _T("{\\nonshppict") );
							sResult.Append( m_oPicture->GenerateWMF( oRenderParameter ) );

							sResult.Append( _T("}") );
						}
					}
				}
				else
				{
					sResult.Append(_T("{\\shp{\\*\\shpinst"));
					RENDER_RTF_INT( m_nLeft, sResult, _T("shpleft") );
					RENDER_RTF_INT( m_nTop, sResult, _T("shptop") );
					RENDER_RTF_INT( m_nBottom, sResult, _T("shpbottom") );
					RENDER_RTF_INT( m_nRight, sResult, _T("shpright") );
					RENDER_RTF_INT( m_nID, sResult, _T("shplid") );
					RENDER_RTF_INT( m_nZOrder, sResult, _T("shpz") );
					RENDER_RTF_INT( m_nHeader, sResult, _T("shpfhdr") );
					RENDER_RTF_INT( m_nWrapType, sResult, _T("shpwr") );
					RENDER_RTF_INT( m_nWrapSideType, sResult, _T("shpwrk") );
					RENDER_RTF_BOOL( m_bLockAnchor, sResult, _T("shplockanchor") );

					switch(m_eXAnchor)
					{
						case ax_page: sResult.Append( _T("\\shpbxpage") );break;
						case ax_margin: sResult.Append( _T("\\shpbxmargin") );break;
						case ax_column: sResult.Append( _T("\\shpbxcolumn") );break;
					}
					sResult.Append( _T("\\shpbxignore") );
					switch(m_eYAnchor)
					{
						case ay_page: sResult.Append( _T("\\shpbypage") );break;
						case ay_margin: sResult.Append( _T("\\shpbymargin") );break;
						case ay_Para: sResult.Append( _T("\\shpbypara") );break;
					}
					sResult.Append( _T("\\shpbyignore") );
					RENDER_RTF_INT( m_nZOrderRelative, sResult, _T("shpfblwtxt") );

					sResult.Append( RenderToRtfShapeProperty( oRenderParameter ) );
					//picture
					if( 0 != m_oPicture )
					{
						sResult.Append( _T("{\\sp{\\sn pib}{\\sv ") );
						sResult.Append( m_oPicture->RenderToRtf( oRenderParameter ) );
						sResult.Append( _T("}}") );
					}
					//textbox
					if( 0 != m_aTextItems )
					{
						sResult.Append( _T("{\\shptxt ") );
						sResult.Append( m_aTextItems->RenderToRtf( oRenderParameter ) );
						sResult.Append( _T("}") );
					}
					sResult.Append(_T("}"));
					if( 0 != m_oPicture )
					{
						sResult.Append(_T("{\\shprslt\\par\\plain"));
						sResult.Append( m_oPicture->GenerateWMF( oRenderParameter ) );
						sResult.Append(_T("\\par}"));
					}
					sResult.Append(_T("}"));
				}
				//восстанавливаем координаты и если нужно поворачиваем
				m_nLeft = nLeft;
				m_nTop = nTop;
				m_nRight = nRight;
				m_nBottom = nBottom;
				m_nRotation = nRotate;

				m_nRelLeft = nRelLeft;
				m_nRelTop = nRelTop;
				m_nRelRight = nRelRight;
				m_nRelBottom = nRelBottom;
				m_nRelRotation = nRelRotate;
			}
			return sResult;
		}
CString RtfShape::RenderToRtfShapeProperty(RenderParameter oRenderParameter)
		{
			CString sResult;

			//Position absolute
			if( PROP_DEF != m_nPositionH )
				sResult.AppendFormat( _T("{\\sp{\\sn posh}{\\sv %d}}"),m_nPositionH);
			if( PROP_DEF != m_nPositionHRelative )
				sResult.AppendFormat( _T("{\\sp{\\sn posrelh}{\\sv %d}}"),m_nPositionHRelative);
			if( PROP_DEF != m_nPositionV )
				sResult.AppendFormat( _T("{\\sp{\\sn posv}{\\sv %d}}"),m_nPositionV);
			if( PROP_DEF != m_nPositionVRelative )
				sResult.AppendFormat( _T("{\\sp{\\sn posrelv}{\\sv %d}}"),m_nPositionVRelative);
			if(  PROP_DEF != m_bLayoutInCell )
				sResult.AppendFormat( _T("{\\sp{\\sn fLayoutInCell}{\\sv %d}}"),m_bLayoutInCell);
			if(  PROP_DEF != m_bAllowOverlap )
				sResult.AppendFormat( _T("{\\sp{\\sn fAllowOverlap}{\\sv %d}}"),m_bAllowOverlap);

			//Position relative
			if(  PROP_DEF != m_nPositionHPct )
				sResult.AppendFormat( _T("{\\sp{\\sn pctHorizPos}{\\sv %d}}"),m_nPositionHPct);
			if(  PROP_DEF != m_nPositionVPct )
				sResult.AppendFormat( _T("{\\sp{\\sn pctVertPos}{\\sv %d}}"),m_nPositionVPct);
			if(  PROP_DEF != m_nPctWidth )
				sResult.AppendFormat( _T("{\\sp{\\sn pctHoriz}{\\sv %d}}"),m_nPctWidth);
			if(  PROP_DEF != m_nPctHeight )
				sResult.AppendFormat( _T("{\\sp{\\sn pctVert}{\\sv %d}}"),m_nPctHeight);
			if(  PROP_DEF != m_nPctWidthRelative )
				sResult.AppendFormat( _T("{\\sp{\\sn sizerelh}{\\sv %d}}"),m_nPctWidthRelative);
			if(  PROP_DEF != m_nPctHeightRelative )
				sResult.AppendFormat( _T("{\\sp{\\sn sizerelv}{\\sv %d}}"),m_nPctHeightRelative);
			if(  PROP_DEF != m_nColStart )
				sResult.AppendFormat( _T("{\\sp{\\sn colStart}{\\sv %d}}"),m_nColStart);
			if(  PROP_DEF != m_nColSpan )
				sResult.AppendFormat( _T("{\\sp{\\sn colSpan}{\\sv %d}}"),m_nColSpan);
			//Rehydration
			if(  _T("") != m_sMetroBlob )
                sResult.AppendFormat( _T("{\\sp{\\sn metroBlob}{\\sv %d}}"),m_sMetroBlob.GetBuffer());

			//Object Type
			if(  PROP_DEF != m_bIsBullet )
				sResult.AppendFormat( _T("{\\sp{\\sn fIsBullet}{\\sv %d}}"),m_bIsBullet);
			if(  PROP_DEF != m_nRotation  )
				sResult.AppendFormat( _T("{\\sp{\\sn rotation}{\\sv %d}}"),m_nRotation);
			if(  PROP_DEF != m_bFlipV )
				sResult.AppendFormat( _T("{\\sp{\\sn fFlipV}{\\sv %d}}"),m_bFlipV);
			if(  PROP_DEF != m_bFlipH )
				sResult.AppendFormat( _T("{\\sp{\\sn fFlipH}{\\sv %d}}"),m_bFlipH);
			if(  PROP_DEF != m_nShapeType )
				sResult.AppendFormat( _T("{\\sp{\\sn shapeType}{\\sv %d}}"),m_nShapeType);
			if(  PROP_DEF != m_nWrapDistLeft )
				sResult.AppendFormat( _T("{\\sp{\\sn dxWrapDistLeft}{\\sv %d}}"), RtfUtility::Twips2Emu( m_nWrapDistLeft ));
			if(  PROP_DEF != m_nWrapDistTop )
				sResult.AppendFormat( _T("{\\sp{\\sn dyWrapDistTop}{\\sv %d}}"), RtfUtility::Twips2Emu( m_nWrapDistTop ));
			if(  PROP_DEF != m_nWrapDistRight ) 
				sResult.AppendFormat( _T("{\\sp{\\sn dxWrapDistRight}{\\sv %d}}"), RtfUtility::Twips2Emu( m_nWrapDistRight ));
			if(  PROP_DEF != m_nWrapDistBottom )
				sResult.AppendFormat( _T("{\\sp{\\sn dyWrapDistBottom}{\\sv %d}}"),RtfUtility::Twips2Emu(  m_nWrapDistBottom ));
			if(  PROP_DEF != m_nZOrderRelative )
				sResult.AppendFormat( _T("{\\sp{\\sn fBehindDocument}{\\sv %d}}"),m_nZOrderRelative);
			if(  PROP_DEF != m_bHidden )
				sResult.AppendFormat( _T("{\\sp{\\sn fHidden}{\\sv %d}}"),m_bHidden);
			//Geometry
			if( PROP_DEF != m_nAdjustValue )
				sResult.AppendFormat( _T("{\\sp{\\sn adjustValue}{\\sv %d}}"),m_nAdjustValue );
			if( PROP_DEF != m_nAdjustValue )
				sResult.AppendFormat( _T("{\\sp{\\sn adjust2Value}{\\sv %d}}"),m_nAdjustValue2 );
			if( PROP_DEF != m_nAdjustValue )
				sResult.AppendFormat( _T("{\\sp{\\sn adjust3Value}{\\sv %d}}"),m_nAdjustValue3 );
			if( PROP_DEF != m_nAdjustValue )
				sResult.AppendFormat( _T("{\\sp{\\sn adjust4Value}{\\sv %d}}"),m_nAdjustValue4 );
			if( PROP_DEF != m_nAdjustValue )
				sResult.AppendFormat( _T("{\\sp{\\sn adjust5Value}{\\sv %d}}"),m_nAdjustValue5 );
			if( PROP_DEF != m_nAdjustValue )
				sResult.AppendFormat( _T("{\\sp{\\sn adjust6Value}{\\sv %d}}"),m_nAdjustValue6 );
			if( PROP_DEF != m_nAdjustValue )
				sResult.AppendFormat( _T("{\\sp{\\sn adjust7Value}{\\sv %d}}"),m_nAdjustValue7 );
			if( PROP_DEF != m_nAdjustValue )
				sResult.AppendFormat( _T("{\\sp{\\sn adjust8Value}{\\sv %d}}"),m_nAdjustValue8 );
			if( PROP_DEF != m_nAdjustValue )
				sResult.AppendFormat( _T("{\\sp{\\sn adjust9Value}{\\sv %d}}"),m_nAdjustValue9 );
			if( PROP_DEF != m_nAdjustValue )
				sResult.AppendFormat( _T("{\\sp{\\sn adjust10Value}{\\sv %d}}"),m_nAdjustValue10 );
			//Connectors
			if( PROP_DEF != m_nConnectionType )
				sResult.AppendFormat( _T("{\\sp{\\sn cxk}{\\sv %d}}"),m_nConnectionType );
			if( PROP_DEF != m_nConnectorStyle )
				sResult.AppendFormat( _T("{\\sp{\\sn cxstyle}{\\sv %d}}"),m_nConnectorStyle );
			//WordArt Effects
			if( PROP_DEF != m_nCropFromTop )
				sResult.AppendFormat( _T("{\\sp{\\sn cropFromTop}{\\sv %d}}"),m_nCropFromTop );
			if( PROP_DEF != m_nCropFromBottom )
				sResult.AppendFormat( _T("{\\sp{\\sn cropFromBottom}{\\sv %d}}"),m_nCropFromBottom );
			if( PROP_DEF != m_nCropFromLeft )
				sResult.AppendFormat( _T("{\\sp{\\sn cropFromLeft}{\\sv %d}}"),m_nCropFromLeft );
			if( PROP_DEF != m_nCropFromRight )
				sResult.AppendFormat( _T("{\\sp{\\sn cropFromRight}{\\sv %d}}"),m_nCropFromRight );

			//Grouped Shapes
			if( PROP_DEF != m_nGroupBottom )
				sResult.AppendFormat( _T("{\\sp{\\sn groupBottom}{\\sv %d}}"),m_nCropFromRight );
			if( PROP_DEF != m_nGroupLeft )
				sResult.AppendFormat( _T("{\\sp{\\sn groupLeft}{\\sv %d}}"),m_nGroupLeft );
			if( PROP_DEF != m_nGroupRight )
				sResult.AppendFormat( _T("{\\sp{\\sn groupRight}{\\sv %d}}"),m_nGroupRight );
			if( PROP_DEF != m_nGroupTop )
				sResult.AppendFormat( _T("{\\sp{\\sn groupTop}{\\sv %d}}"),m_nGroupTop );
			if( PROP_DEF != m_nRelBottom )
				sResult.AppendFormat( _T("{\\sp{\\sn relBottom}{\\sv %d}}"),m_nRelBottom );
			if( PROP_DEF != m_nRelLeft )
				sResult.AppendFormat( _T("{\\sp{\\sn relLeft}{\\sv %d}}"),m_nRelLeft );
			if( PROP_DEF != m_nRelRight )
				sResult.AppendFormat( _T("{\\sp{\\sn relRight}{\\sv %d}}"),m_nRelRight );
			if( PROP_DEF != m_nRelTop )
				sResult.AppendFormat( _T("{\\sp{\\sn relTop}{\\sv %d}}"),m_nRelTop );
			if( PROP_DEF != m_nRelRotation )
				sResult.AppendFormat( _T("{\\sp{\\sn relRotation}{\\sv %d}}"),m_nRelRotation );
			if( PROP_DEF != m_nRelZOrder )
				sResult.AppendFormat( _T("{\\sp{\\sn dhgt}{\\sv %d}}"),m_nRelZOrder );

			//Line
			if( PROP_DEF != m_nLineStartArrow )
				sResult.AppendFormat( _T("{\\sp{\\sn lineStartArrowhead}{\\sv %d}}"),m_nLineStartArrow );
			if( PROP_DEF != m_nLineEndArrow )
				sResult.AppendFormat( _T("{\\sp{\\sn lineEndArrowhead}{\\sv %d}}"),m_nLineEndArrow );
			if( PROP_DEF != m_nLineStartArrowWidth )
				sResult.AppendFormat( _T("{\\sp{\\sn lineStartArrowWidth}{\\sv %d}}"),m_nLineStartArrowWidth );
			if( PROP_DEF != m_nLineStartArrowLength )
				sResult.AppendFormat( _T("{\\sp{\\sn lineStartArrowLength}{\\sv %d}}"),m_nLineStartArrowLength );
			if( PROP_DEF != m_nLineEndArrowWidth )
				sResult.AppendFormat( _T("{\\sp{\\sn lineEndArrowWidth}{\\sv %d}}"),m_nLineEndArrowWidth );
			if( PROP_DEF != m_nLineEndArrowLength )
				sResult.AppendFormat( _T("{\\sp{\\sn lineEndArrowLength}{\\sv %d}}"),m_nLineEndArrowLength );
			if( PROP_DEF != m_nLineWidth )
				sResult.AppendFormat( _T("{\\sp{\\sn lineWidth}{\\sv %d}}"),m_nLineWidth );

			//pWrapPolygonVertices	Points of the text wrap polygon.
			int nWrapPointCount = (int)m_aWrapPoints.size();
			if( nWrapPointCount > 0 )
			{
				sResult.AppendFormat( _T("{\\sp{\\sn pWrapPolygonVertices}{\\sv 8;%d"), nWrapPointCount / 2 );
				for( int i = 0; i < nWrapPointCount - 1; i += 2 )
					sResult.AppendFormat( _T(";(%d,%d)"), m_aWrapPoints[i], m_aWrapPoints[ i + 1 ] );
				sResult.Append( _T("}}") );
			}

			return sResult;
		}
CString RtfShape::RenderToOOX(RenderParameter oRenderParameter)
		{
			CString sResult;
			RtfDocument* poDocument = static_cast<RtfDocument*>(oRenderParameter.poDocument);
			if( PROP_DEF != m_nShapeType && 0 != m_nShapeType )
			{
				if( false && RENDER_TO_OOX_PARAM_SHAPE_WSHAPE != oRenderParameter.nType && NULL != m_oPicture ) //картинки пишем как drawing
				{
					if( st_inline == m_eShapeType )
					{
						if( PROP_DEF != m_oPicture->m_nWidthGoal && PROP_DEF != m_oPicture->m_nHeightGoal )
						{
							sResult.Append( _T("<w:r><w:drawing><wp:inline") );
							int nDistLeft = 0;
							int nDistTop = 0;
							int nDistRight = 0;
							int nDistBottom = 0;
							if( PROP_DEF != m_nWrapDistLeft ) 
								nDistLeft = RtfUtility::Twips2Emu( m_nWrapDistLeft );
							if( PROP_DEF != m_nWrapDistTop ) 
								nDistTop = RtfUtility::Twips2Emu( m_nWrapDistTop );
							if( PROP_DEF != m_nWrapDistRight ) 
								nDistRight = RtfUtility::Twips2Emu( m_nWrapDistRight );
							if( PROP_DEF != m_nWrapDistBottom ) 
								nDistBottom = RtfUtility::Twips2Emu( m_nWrapDistBottom );
							RENDER_OOX_INT_ATTRIBUTE( nDistLeft, sResult, _T("distL") )
							RENDER_OOX_INT_ATTRIBUTE( nDistTop, sResult, _T("distT") )
							RENDER_OOX_INT_ATTRIBUTE( nDistRight, sResult, _T("distR") )
							RENDER_OOX_INT_ATTRIBUTE( nDistBottom, sResult, _T("distB") )
							sResult.Append( _T(">") );

							if( PROP_DEF != m_oPicture->m_nWidthGoal && PROP_DEF != m_oPicture->m_nHeightGoal )
								sResult.AppendFormat( _T("<wp:extent cx=\"%d\" cy=\"%d\"/>"), RtfUtility::Twips2Emu( m_oPicture->m_nWidthGoal ), RtfUtility::Twips2Emu(  m_oPicture->m_nHeightGoal ) );
							int nImageID = poDocument->m_oIdGenerator.Generate_ImagePropId();
                            sResult.AppendFormat( _T("<wp:docPr id=\"%d\" name=\"%ls\" descr=\"\"/>"), m_sName.GetBuffer(), nImageID );

							int nCropLeft = PROP_DEF;
							int nCropTop = PROP_DEF;
							int nCropRight = PROP_DEF;
							int nCropBottom = PROP_DEF;

							if( PROP_DEF != m_nCropFromLeft )
								nCropLeft = (int)( 100000.0 * m_nCropFromLeft / 65536 );
							else if( PROP_DEF != m_oPicture->m_nWidthGoal && PROP_DEF != m_oPicture->m_nCropL )
								nCropLeft = (int)( 100000.0 * m_oPicture->m_nCropL / m_oPicture->m_nWidthGoal );
							if( PROP_DEF != m_nCropFromTop )
								nCropTop = (int)( 100000.0 * m_nCropFromTop / 65536 );
							else if( PROP_DEF != m_oPicture->m_nHeightGoal && PROP_DEF != m_oPicture->m_nCropT )
								nCropTop = (int)( 100000.0 * m_oPicture->m_nCropT / m_oPicture->m_nHeightGoal );
							if( PROP_DEF != m_nCropFromRight )
								nCropRight = (int)( 100000.0 * m_nCropFromRight / 65536 );
							else if( PROP_DEF != m_oPicture->m_nWidthGoal && PROP_DEF != m_oPicture->m_nCropR )
								nCropRight = (int)( 100000.0 * m_oPicture->m_nCropR / m_oPicture->m_nWidthGoal );
							if( PROP_DEF != m_nCropFromBottom )
								nCropBottom = (int)( 100000.0 * m_nCropFromBottom / 65536 );
							else if( PROP_DEF != m_oPicture->m_nHeightGoal && PROP_DEF != m_oPicture->m_nCropB )
								nCropBottom = (int)( 100000.0 * m_oPicture->m_nCropB / m_oPicture->m_nHeightGoal );

							sResult.Append( _T("<a:graphic><a:graphicData  uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:pic xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">"));

							sResult.Append( _T("<pic:nvPicPr>") );
                            sResult.AppendFormat( _T("<pic:cNvPr id=\"%d\" name=\"%ls\" descr=\"\"/>"), nImageID, m_sName.GetBuffer() );
                            if( 0 == m_bLockAnchor )
								sResult.Append( _T("<pic:cNvPicPr><a:picLocks noChangeAspect=\"0\" noChangeArrowheads=\"1\"/></pic:cNvPicPr>") );
							else
								sResult.Append( _T("<pic:cNvPicPr><a:picLocks noChangeAspect=\"1\" noChangeArrowheads=\"1\"/></pic:cNvPicPr>") );
							sResult.Append( _T("</pic:nvPicPr>") );

                            sResult.AppendFormat( _T("<pic:blipFill><a:blip r:embed=\"%ls\"/>"), m_oPicture->RenderToOOX(oRenderParameter).GetBuffer() );
							CString sCrop;
							if( PROP_DEF != nCropLeft )
								sCrop.AppendFormat( _T(" l=\"%d\""), nCropLeft ); //тысячный доли процента
							if( PROP_DEF != nCropTop )
								sCrop.AppendFormat( _T(" t=\"%d\""), nCropTop );
							if( PROP_DEF != nCropRight )
								sCrop.AppendFormat( _T(" r=\"%d\""), nCropRight );
							if( PROP_DEF != nCropBottom )
								sCrop.AppendFormat( _T(" b=\"%d\""), nCropBottom );
							if( false == sCrop.IsEmpty() )
                                sResult.AppendFormat( _T("<a:srcRect %ls/>"), sCrop.GetBuffer() );
							sResult.Append( _T("<a:stretch><a:fillRect/></a:stretch>") );
							sResult.Append( _T("</pic:blipFill>") );
							if( PROP_DEF != m_oPicture->m_nWidthGoal && PROP_DEF != m_oPicture->m_nHeightGoal )
							{
								sResult.Append( _T("<pic:spPr bwMode=\"auto\">") );
								sResult.Append( _T("<a:xfrm") );
								int nRotation = PROP_DEF;
								if( PROP_DEF != m_nRotation ) 
									nRotation = m_nRotation;
								else
									nRotation = m_nRelRotation;
								if( PROP_DEF != nRotation )
								{
									int nOOXRotation = (int)( 60000.0 * nRotation / 0x10000 );//Опытным путем 1 градус == 60000
									sResult.AppendFormat( _T(" rot=\"%d\""), nOOXRotation );
								}
								sResult.Append( _T(">") );
								sResult.AppendFormat( _T("<a:off x=\"0\" y=\"0\"/><a:ext cx=\"%d\" cy=\"%d\"/>"), m_oPicture->m_nWidthGoal, m_oPicture->m_nHeightGoal );
								sResult.Append( _T("</a:xfrm>") );
								sResult.Append( _T("<a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom>") );
								sResult.Append( _T("</pic:spPr>") );
							}
							sResult.Append( _T("</pic:pic></a:graphicData></a:graphic>") );

							sResult.Append( _T("</wp:inline></w:drawing></w:r>") );
						}
					}
					else
					{
						if( PROP_DEF != m_oPicture->m_nWidthGoal && PROP_DEF != m_oPicture->m_nHeightGoal )
						{
							sResult.Append( _T("<w:r><w:drawing><wp:anchor") );
							int nDistLeft = 0;//аттрибуты обязательные
							int nDistTop = 0;
							int nDistRight = 0;
							int nDistBottom = 0;
							int nOverlap = 1;
							int nBehind = 0;
							int nLayout = 1;
							int nLocked = 0;
							int nZIndex = 100;


							if( PROP_DEF != m_nWrapDistLeft ) 
								nDistLeft = RtfUtility::Twips2Emu( m_nWrapDistLeft );
							if( PROP_DEF != m_nWrapDistTop ) 
								nDistTop = RtfUtility::Twips2Emu( m_nWrapDistTop );
							if( PROP_DEF != m_nWrapDistRight ) 
								nDistRight = RtfUtility::Twips2Emu( m_nWrapDistRight );
							if( PROP_DEF != m_nWrapDistBottom ) 
								nDistBottom = RtfUtility::Twips2Emu( m_nWrapDistBottom );
							if( PROP_DEF != m_bAllowOverlap )
								nOverlap = m_bAllowOverlap;
							if( PROP_DEF != m_nZOrderRelative )
								nBehind = m_nZOrderRelative;
							if( PROP_DEF != m_bLayoutInCell )
								nLayout = m_bLayoutInCell;
							if( PROP_DEF != m_bLockAnchor )
								nLocked = m_bLockAnchor;
							if( PROP_DEF != m_nZOrder )
								nZIndex = m_nZOrder;

							RENDER_OOX_INT_ATTRIBUTE( nDistLeft, sResult, _T("distL") )
							RENDER_OOX_INT_ATTRIBUTE( nDistTop, sResult, _T("distT") )
							RENDER_OOX_INT_ATTRIBUTE( nDistRight, sResult, _T("distR") )
							RENDER_OOX_INT_ATTRIBUTE( nDistBottom, sResult, _T("distB") )
							RENDER_OOX_BOOL_ATTRIBUTE( nOverlap, sResult, _T("allowOverlap") )
							RENDER_OOX_BOOL_ATTRIBUTE( nBehind, sResult, _T("behindDoc") )
							RENDER_OOX_BOOL_ATTRIBUTE( m_bHidden, sResult, _T("hidden") )
							RENDER_OOX_BOOL_ATTRIBUTE( nLayout, sResult, _T("layoutInCell") )
							RENDER_OOX_BOOL_ATTRIBUTE( nLocked, sResult, _T("locked") )
							RENDER_OOX_INT_ATTRIBUTE( nZIndex, sResult, _T("relativeHeight") )
							RENDER_OOX_BOOL_ATTRIBUTE( 0, sResult, _T("simplePos") )
							sResult.Append( _T(">") );

							if( PROP_DEF != m_nLeft && PROP_DEF != m_nTop )
								sResult.AppendFormat( _T("<wp:simplePos x=\"%d\" y=\"%d\"/>"),RtfUtility::Twips2Emu(m_nLeft), RtfUtility::Twips2Emu(m_nTop) );

							if( ( PROP_DEF != m_nPositionHRelative || ax_none != m_eXAnchor ) && (PROP_DEF != m_nPositionH || PROP_DEF != m_nLeft ) )
							{
								sResult.Append( _T("<wp:positionH") );
								if( PROP_DEF != m_nPositionHRelative )
								{
									switch( m_nPositionHRelative )
									{
										case 0: sResult.Append( _T(" relativeFrom=\"margin\"") );break;
										case 1: sResult.Append( _T(" relativeFrom=\"page\"") );break;
										case 2: sResult.Append( _T(" relativeFrom=\"column\"") );break;
										case 3: sResult.Append( _T(" relativeFrom=\"character\"") );break;
										case 4: sResult.Append( _T(" relativeFrom=\"leftMargin\"") );break;
										case 5: sResult.Append( _T(" relativeFrom=\"rightMargin\"") );break;
										case 6: sResult.Append( _T(" relativeFrom=\"insideMargin\"") );break;
										case 7: sResult.Append( _T(" relativeFrom=\"outsideMargin\"") );break;
									}
								}
								else
								{
									switch( m_eXAnchor )
									{
										case ax_margin: sResult.Append( _T(" relativeFrom=\"margin\"") );break;
										case ax_page: sResult.Append( _T(" relativeFrom=\"page\"") );break;
										case ax_column: sResult.Append( _T(" relativeFrom=\"column\"") );break;
									}
								}
								sResult.Append( _T(">") );
								if( PROP_DEF != m_nPositionH )
								{
									switch( m_nPositionH )
									{
										case 1: sResult.Append( _T("<wp:align>left</wp:align>") );break;
										case 2: sResult.Append( _T("<wp:align>center</wp:align>") );break;
										case 3: sResult.Append( _T("<wp:align>right</wp:align>") );break;
										case 4: sResult.Append( _T("<wp:align>inside</wp:align>") );break;
										case 5: sResult.Append( _T("<wp:align>outside</wp:align>") );break;
									}
								}
								else
								{
									sResult.AppendFormat( _T("<wp:posOffset>%d</wp:posOffset>"), RtfUtility::Twips2Emu( m_nLeft ) );
								}
								sResult.Append( _T("</wp:positionH>") );
							}
							if( ( PROP_DEF != m_nPositionVRelative || ax_none != m_eYAnchor ) && (PROP_DEF != m_nPositionV || PROP_DEF != m_nTop ) )
							{
								sResult.Append( _T("<wp:positionV") );
								if( PROP_DEF != m_nPositionVRelative )
								{
									switch( m_nPositionVRelative )
									{
										case 0: sResult.Append( _T(" relativeFrom=\"margin\"") );break;
										case 1: sResult.Append( _T(" relativeFrom=\"page\"") );break;
										case 2: sResult.Append( _T(" relativeFrom=\"paragraph\"") );break;
										case 3: sResult.Append( _T(" relativeFrom=\"line\"") );break;
										case 4: sResult.Append( _T(" relativeFrom=\"topMargin\"") );break;
										case 5: sResult.Append( _T(" relativeFrom=\"bottomMargin\"") );break;
										case 6: sResult.Append( _T(" relativeFrom=\"insideMargin\"") );break;
										case 7: sResult.Append( _T(" relativeFrom=\"outsideMargin\"") );break;
									}
								}
								else
								{
									switch( m_eYAnchor )
									{
										case ay_margin: sResult.Append( _T(" relativeFrom=\"margin\"") );break;
										case ay_page: sResult.Append( _T(" relativeFrom=\"page\"") );break;
										case ay_Para: sResult.Append( _T(" relativeFrom=\"paragraph\"") );break;
									}
								}
								sResult.Append( _T(">") );
								if( PROP_DEF != m_nPositionV )
								{
									switch( m_nPositionV )
									{
										case 1: sResult.Append( _T("<wp:align>top</wp:align>") );break;
										case 2: sResult.Append( _T("<wp:align>center</wp:align>") );break;
										case 3: sResult.Append( _T("<wp:align>bottom</wp:align>") );break;
										case 4: sResult.Append( _T("<wp:align>inside</wp:align>") );break;
										case 5: sResult.Append( _T("<wp:align>outside</wp:align>") );break;
									}
								}
								else
								{
									sResult.AppendFormat( _T("<wp:posOffset>%d</wp:posOffset>"), RtfUtility::Twips2Emu( m_nTop ) );
								}
								sResult.Append( _T("</wp:positionV>") );
							}

							int nWidth;
							int nHeight;
							if( PROP_DEF != m_nLeft && PROP_DEF != m_nRight && PROP_DEF != m_nTop &&  PROP_DEF != m_nBottom )
							{
								nWidth = m_nRight - m_nLeft;
								nHeight = m_nBottom - m_nTop;
							}
							else if( PROP_DEF != m_oPicture->m_nWidthGoal && PROP_DEF != m_oPicture->m_nHeightGoal )
							{
								nWidth = m_oPicture->m_nWidthGoal;
								nHeight = m_oPicture->m_nHeightGoal;
							}
							if( PROP_DEF != nWidth && PROP_DEF != nHeight )
								sResult.AppendFormat( _T("<wp:extent cx=\"%d\" cy=\"%d\"/>"), RtfUtility::Twips2Emu( nWidth ), RtfUtility::Twips2Emu(  nHeight ) );

							if( PROP_DEF != m_nWrapType && 3 != m_nWrapType)
							{
								sResult.Append( _T("<") );
								CString sTag;
								switch( m_nWrapType )
								{
									case 1:sTag.Append( _T("wp:wrapTopAndBottom") );break;
									case 2:sTag.Append( _T("wp:wrapSquare") );break;
									case 3:sTag.Append( _T("wp:wrapNone") );break;
									case 4:sTag.Append( _T("wp:wrapTight") );break;
									case 5:sTag.Append( _T("wp:wrapThrough") );break;
								}
								sResult.Append( sTag );
								switch( m_nWrapSideType )
								{
									case 0:sResult.Append( _T(" wrapText=\"bothSides\"") );break;
									case 1:sResult.Append( _T(" wrapText=\"left\"") );break;
									case 2:sResult.Append( _T(" wrapText=\"right\"") );break;
									case 3:sResult.Append( _T(" wrapText=\"largest\"") );break;
								}

								if( m_aWrapPoints.size() == 0 )
									sResult.Append( _T("/>") );
								else
								{
									sResult.Append( _T(">") );
									sResult.Append( _T("<wp:wrapPolygon>") );
									if(  m_aWrapPoints.size() > 1 )
										sResult.AppendFormat( _T("<wp:start x=\"%d\" y=\"%d\" />"), m_aWrapPoints[0], m_aWrapPoints[1] );
									for( int i = 2; i < (int)m_aWrapPoints.size() - 1; i+=2 )
										sResult.AppendFormat( _T("<wp:lineTo x=\"%d\" y=\"%d\" />"), m_aWrapPoints[i], m_aWrapPoints[i+1] );
									sResult.Append( _T("</wp:wrapPolygon>") );
                                    sResult.AppendFormat( _T("</%ls>"), sTag.GetBuffer() );
								}
							}
							int nImageID = poDocument->m_oIdGenerator.Generate_ImagePropId();
                            sResult.AppendFormat( _T("<wp:docPr id=\"%d\" name=\"%ls\" descr=\"\"/>"), nImageID, m_sName.GetBuffer() );

							int nCropLeft = PROP_DEF;
							int nCropTop = PROP_DEF;
							int nCropRight = PROP_DEF;
							int nCropBottom = PROP_DEF;

							if( PROP_DEF != m_nCropFromLeft )
								nCropLeft = (int)( 100000.0 * m_nCropFromLeft / 65536 );
							else if( PROP_DEF != m_oPicture->m_nWidthGoal && PROP_DEF != m_oPicture->m_nCropL )
								nCropLeft = (int)( 100000.0 * m_oPicture->m_nCropL / m_oPicture->m_nWidthGoal );
							if( PROP_DEF != m_nCropFromTop )
								nCropTop = (int)( 100000.0 * m_nCropFromTop / 65536 );
							else if( PROP_DEF != m_oPicture->m_nHeightGoal && PROP_DEF != m_oPicture->m_nCropT )
								nCropTop = (int)( 100000.0 * m_oPicture->m_nCropT / m_oPicture->m_nHeightGoal );
							if( PROP_DEF != m_nCropFromRight )
								nCropRight = (int)( 100000.0 * m_nCropFromRight / 65536 );
							else if( PROP_DEF != m_oPicture->m_nWidthGoal && PROP_DEF != m_oPicture->m_nCropR )
								nCropRight = (int)( 100000.0 * m_oPicture->m_nCropR / m_oPicture->m_nWidthGoal );
							if( PROP_DEF != m_nCropFromBottom )
								nCropBottom = (int)( 100000.0 * m_nCropFromBottom / 65536 );
							else if( PROP_DEF != m_oPicture->m_nHeightGoal && PROP_DEF != m_oPicture->m_nCropB )
								nCropBottom = (int)( 100000.0 * m_oPicture->m_nCropB / m_oPicture->m_nHeightGoal );

								sResult.Append( _T("<a:graphic><a:graphicData  uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:pic xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">"));

								sResult.Append( _T("<pic:nvPicPr>") );
                                sResult.AppendFormat( _T("<pic:cNvPr id=\"%d\" name=\"%ls\" descr=\"\"/>"), nImageID, m_sName.GetBuffer() );
								if( 0 == m_bLockAnchor )
									sResult.Append( _T("<pic:cNvPicPr><a:picLocks noChangeAspect=\"0\" noChangeArrowheads=\"1\"/></pic:cNvPicPr>") );
								else
									sResult.Append( _T("<pic:cNvPicPr><a:picLocks noChangeAspect=\"1\" noChangeArrowheads=\"1\"/></pic:cNvPicPr>") );
								sResult.Append( _T("</pic:nvPicPr>") );

                                sResult.AppendFormat( _T("<pic:blipFill><a:blip r:embed=\"%ls\"/>"), m_oPicture->RenderToOOX(oRenderParameter).GetBuffer() );
								CString sCrop;
								if( PROP_DEF != nCropLeft )
									sCrop.AppendFormat( _T(" l=\"%d\""), nCropLeft ); //тысячный доли процента
								if( PROP_DEF != nCropTop )
									sCrop.AppendFormat( _T(" t=\"%d\""), nCropTop );
								if( PROP_DEF != nCropRight )
									sCrop.AppendFormat( _T(" r=\"%d\""), nCropRight );
								if( PROP_DEF != nCropBottom )
									sCrop.AppendFormat( _T(" b=\"%d\""), nCropBottom );
								if( false == sCrop.IsEmpty() )
                                    sResult.AppendFormat( _T("<a:srcRect %ls/>"), sCrop.GetBuffer() );
								sResult.Append( _T("<a:stretch><a:fillRect/></a:stretch>") );

								sResult.Append( _T("</pic:blipFill>") );
								if( PROP_DEF != m_oPicture->m_nWidthGoal && PROP_DEF != m_oPicture->m_nHeightGoal )
								{
									sResult.Append( _T("<pic:spPr bwMode=\"auto\">") );
									sResult.Append( _T("<a:xfrm") );
									int nRotation = PROP_DEF;
									if( PROP_DEF != m_nRotation ) 
										nRotation = m_nRotation;
									else
										nRotation = m_nRelRotation;
									if( PROP_DEF != nRotation )
									{
										int nOOXRotation = (int)( 60000.0 * nRotation / 65536 );//Опытным путем 1 градус == 60000
										sResult.AppendFormat( _T(" rot=\"%d\""), nOOXRotation );
									}
									sResult.Append( _T(">") );
									sResult.AppendFormat( _T("<a:off x=\"0\" y=\"0\"/><a:ext cx=\"%d\" cy=\"%d\"/>"), m_oPicture->m_nWidthGoal, m_oPicture->m_nHeightGoal );
									sResult.Append( _T("</a:xfrm>") );
									sResult.Append( _T("<a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom>") );
									sResult.Append( _T("</pic:spPr>") );
								}
								sResult.Append( _T("</pic:pic></a:graphicData></a:graphic>") );

							sResult.Append( _T("</wp:anchor></w:drawing></w:r>") );
						}
					}
				}
				else
				{
					TextItemContainerPtr aTempTextItems;
					if( 75 == m_nShapeType && 0 != m_aTextItems )
					{//Значит это Ole обьект с anchor, для него все также только TextBox надо делать по-другому
						aTempTextItems = m_aTextItems;
						m_aTextItems = TextItemContainerPtr();
						m_bIsOle = true;
					}

					sResult = RenderToOOXBegin(oRenderParameter);
					if( _T("") != sResult )
						sResult.Append( RenderToOOXEnd(oRenderParameter) );

					CString sOle;
					if( 0 != aTempTextItems )
					{//пишем только Ole обьект
						//ищем первый ole обьект
						RtfOlePtr poFirstOle;
						int nTempTextItemsCount = aTempTextItems->GetCount();
						for( int i = 0; i < nTempTextItemsCount; i++ )
						{
							ITextItemPtr piCurTextItem;
							aTempTextItems->GetItem( piCurTextItem, i );
							if( NULL != piCurTextItem && TYPE_RTF_PARAGRAPH == piCurTextItem->GetType() )
							{
								RtfParagraphPtr poCurParagraph = boost::shared_static_cast< RtfParagraph, ITextItem >( piCurTextItem );
								if( NULL != poCurParagraph )
								{
									bool bBreak = false;
									for( int j = 0; j < poCurParagraph->GetCount(); j++ )
									{
										IDocumentElementPtr piCurIDocumentElement;
										poCurParagraph->GetItem( piCurIDocumentElement, j );
										if( NULL != piCurIDocumentElement && TYPE_RTF_OLE == piCurIDocumentElement->GetType() )
										{
											//рендерим только Ole часть
											RenderParameter oNewParam = oRenderParameter;
											oNewParam.nType = RENDER_TO_OOX_PARAM_OLE_ONLY;
											oNewParam.nValue = m_nID;

											RtfOlePtr poCurOle = boost::shared_static_cast< RtfOle, IDocumentElement >( piCurIDocumentElement );
											if( NULL != poCurOle )
											{
												sOle.Append( poCurOle->RenderToOOX( oNewParam ) );
												bBreak = true;
												break;
											}
										}
									}
									if( true == bBreak )
										break;
								}
							}
						}
						//возвращаем text box на место
						m_aTextItems = aTempTextItems;
					}
					if( false == sOle.IsEmpty() )
						sResult.Replace( _T("</w:pict>"), sOle + _T("</w:pict>") );
				}
			}
			return sResult;
		}
CString RtfShape::RenderToOOXBegin(RenderParameter oRenderParameter)
{
	if( false == IsValid() )
		return _T("");

			RtfDocument* poDocument = static_cast<RtfDocument*>( oRenderParameter.poDocument );
			CString sResult;
			if( RENDER_TO_OOX_PARAM_SHAPE_WSHAPE2 == oRenderParameter.nType )
				;
			else if( RENDER_TO_OOX_PARAM_SHAPE_WSHAPE == oRenderParameter.nType )
				sResult.Append(_T("<w:pict>"));
			else
				sResult.Append(_T("<w:r><w:pict>"));//работает по умолчанию
			if( _T("") == oRenderParameter.sValue )
				sResult.Append( _T("<v:shape") );
			else
                sResult.AppendFormat( _T("<%ls"),oRenderParameter.sValue.GetBuffer() );

			sResult.AppendFormat( _T(" id=\"_x0000_s%d\""), poDocument->GetShapeId( m_nID ) );
			if( PROP_DEF != m_nShapeType )
				sResult.AppendFormat( _T(" type=\"#_x0000_t%d\""),m_nShapeType );

			//Connectors
			switch( m_nConnectionType )
			{
				case 0: sResult.Append(_T(" o:connecttype=\"custom\""));break;
				case 1: sResult.Append(_T(" o:connecttype=\"none\""));break;
				case 2: sResult.Append(_T(" o:connecttype=\"rect\""));break;
				case 3: sResult.Append(_T(" o:connecttype=\"segments\""));break;
			}
			switch( m_nConnectorStyle )
			{
				case 0: sResult.Append(_T(" o:connectortype=\"straight\""));break;
				case 1: sResult.Append(_T(" o:connectortype=\"elbow\""));break;
				case 2: sResult.Append(_T(" o:connectortype=\"curved\""));break;
				case 3: sResult.Append(_T(" o:connectortype=\"none\""));break;
			}
			//Connectors

			//Position
			CString sStyle ;
			if( PROP_DEF != m_nLeft &&  PROP_DEF != m_nRight && PROP_DEF != m_nTop && PROP_DEF != m_nBottom   )
			{
				//не пишем если inline
				if( 3 != m_nPositionHRelative || 3 != m_nPositionVRelative )
				{
					sStyle .Append( _T("position:absolute;") );
					sStyle.AppendFormat(_T("margin-left:%fpt;"), RtfUtility::Twip2pt(m_nLeft) );
					sStyle.AppendFormat(_T("margin-top:%fpt;"), RtfUtility::Twip2pt(m_nTop));
					sStyle.AppendFormat(_T("margin-bottom:%fpt;"), RtfUtility::Twip2pt(m_nBottom) );
					sStyle.AppendFormat(_T("margin-right:%fpt;"), RtfUtility::Twip2pt(m_nRight));
				}
				int nWidth = m_nRight - m_nLeft;
				int nHeight = m_nBottom - m_nTop;
				sStyle.AppendFormat(_T("width:%fpt;height:%fpt;"), RtfUtility::Twip2pt(nWidth), RtfUtility::Twip2pt(nHeight));
			}
			else if( PROP_DEF != m_nRelLeft &&  PROP_DEF != m_nRelRight && PROP_DEF != m_nRelTop && PROP_DEF != m_nRelBottom  )
			{
				int nWidth = m_nRelRight - m_nRelLeft;
				int nHeight = m_nRelBottom - m_nRelTop;
				sStyle .Append( _T("position:absolute;") );
				sStyle.AppendFormat(_T("left:%d;"), m_nRelLeft );
				sStyle.AppendFormat(_T("top:%d;"), m_nRelTop);
				sStyle.AppendFormat(_T("bottom:%d;"), m_nRelBottom );
				sStyle.AppendFormat(_T("right:%d;"), m_nRelRight);
				sStyle.AppendFormat(_T("width:%d;height:%d;"), nWidth, nHeight);
			}
			else if( 0 != m_oPicture && PROP_DEF != m_oPicture->m_nWidthGoal && PROP_DEF != m_oPicture->m_nHeightGoal && PROP_DEF != (int)m_oPicture->m_dScaleX && PROP_DEF != (int)m_oPicture->m_dScaleY )
			{
				int nWidth = (int)(m_oPicture->m_nWidthGoal * m_oPicture->m_dScaleX / 100);
				if( PROP_DEF != m_oPicture->m_nCropL )
					nWidth -= m_oPicture->m_nCropL;
				if( PROP_DEF != m_oPicture->m_nCropR )
					nWidth -= m_oPicture->m_nCropR;

				int nHeight = (int)(m_oPicture->m_nHeightGoal * m_oPicture->m_dScaleY / 100);
				if( PROP_DEF != m_oPicture->m_nCropT )
					nHeight -= m_oPicture->m_nCropT;
				if( PROP_DEF != m_oPicture->m_nCropB )
					nHeight -= m_oPicture->m_nCropB;

				sStyle.AppendFormat(_T("width:%dpt;height:%dpt;"), (int)RtfUtility::Twip2pt(nWidth), (int)RtfUtility::Twip2pt(nHeight));
			}

			switch( m_nPositionH )
			{
				case 0: sStyle.Append(_T("mso-position-horizontal:absolute;"));break;
				case 1: sStyle.Append(_T("mso-position-horizontal:left;"));break;
				case 2: sStyle.Append(_T("mso-position-horizontal:center;"));break;
				case 3: sStyle.Append(_T("mso-position-horizontal:right;"));break;
				case 4: sStyle.Append(_T("mso-position-horizontal:inside;"));break;
				case 5: sStyle.Append(_T("mso-position-horizontal:outside;"));break;
			}
			if( PROP_DEF != m_nPositionHPct )//todo
			{
				sStyle.AppendFormat(_T("mso-left-percent:%d;"), m_nPositionHPct);
			}
			if( PROP_DEF != m_nPositionH && PROP_DEF == m_nPositionHRelative )
				m_nPositionHRelative = 2;
			if( PROP_DEF != m_nPositionHRelative )
			{
				switch( m_nPositionHRelative )
				{
					case 0: sStyle.Append(_T("mso-position-horizontal-relative:margin;"));break;
					case 1: sStyle.Append(_T("mso-position-horizontal-relative:page;"));break;
					case 2: sStyle.Append(_T("mso-position-horizontal-relative:text;"));break;
					case 3: sStyle.Append(_T("mso-position-horizontal-relative:char;"));break;
					case 4: sStyle.Append(_T("mso-position-horizontal-relative:left-margin-area;"));break;
					case 5: sStyle.Append(_T("mso-position-horizontal-relative:right-margin-area;"));break;
					case 6: sStyle.Append(_T("mso-position-horizontal-relative:inner-margin-area;"));break;
					case 7: sStyle.Append(_T("mso-position-horizontal-relative:outer-margin-area;"));break;
				}
			}
			else
			{
				switch( m_eXAnchor )
				{
					case ax_page: sStyle.Append(_T("mso-position-horizontal-relative:page;"));break;
					case ax_margin: sStyle.Append(_T("mso-position-horizontal-relative:margin;"));break;
					//case ax_column: sStyle.Append(_T("mso-position-horizontal-relative:text;"));break;
				}
			}

			switch( m_nPositionV )
			{
				case 0: sStyle.Append(_T("mso-position-vertical:absolute;"));break;
				case 1: sStyle.Append(_T("mso-position-vertical:top;"));break;
				case 2: sStyle.Append(_T("mso-position-vertical:center;"));break;
				case 3: sStyle.Append(_T("mso-position-vertical:bottom;"));break;
				case 4: sStyle.Append(_T("mso-position-vertical:inside;"));break;
				case 5: sStyle.Append(_T("mso-position-vertical:outside;"));break;
			}
			if( PROP_DEF != m_nPositionVPct )
				sStyle.AppendFormat(_T("mso-top-percent:%d;"),m_nPositionVPct);

			if( PROP_DEF != m_nPositionV && PROP_DEF == m_nPositionVRelative )
				m_nPositionVRelative =2;
			if( PROP_DEF != m_nPositionVRelative )
			{
				switch( m_nPositionVRelative )
				{
					case 0: sStyle.Append(_T("mso-position-vertical-relative:margin;"));break;
					case 1: sStyle.Append(_T("mso-position-vertical-relative:page;"));break;
					case 2: sStyle.Append(_T("mso-position-vertical-relative:text;"));break;
					case 3: sStyle.Append(_T("mso-position-vertical-relative:line;"));break;
					case 4: sStyle.Append(_T("mso-position-vertical-relative:top-margin-area;"));break;
					case 5: sStyle.Append(_T("mso-position-vertical-relative:bottom-margin-area;"));break;
					case 6: sStyle.Append(_T("mso-position-vertical-relative:inner-margin-area;"));break;
					case 7: sStyle.Append(_T("mso-position-vertical-relative:outer-margin-area;"));break;
				}
			}
			else
			{
				switch( m_eYAnchor )
				{
					case ay_page: sStyle.Append(_T("mso-position-vertical-relative:page;"));break;
					case ay_margin: sStyle.Append(_T("mso-position-vertical-relative:margin;"));break;
					//case ay_Para: sStyle.Append(_T("mso-position-vertical-relative:text;"));break;
				}
			}
			if( PROP_DEF != m_nPctWidth )
				sStyle.AppendFormat(_T("mso-width-percent:%d;"),m_nPctWidth);
			switch( m_nPctWidthRelative )
			{
				case 0:sStyle.Append(_T("mso-width-relative:margin;"));break;
				case 1:sStyle.Append(_T("mso-width-relative:page;"));break;
				case 2:sStyle.Append(_T("mso-width-relative:left-margin-area;"));break;
				case 3:sStyle.Append(_T("mso-width-relative:right-margin-area;"));break;
				case 4:sStyle.Append(_T("mso-width-relative:inner-margin-area;"));break;
				case 5:sStyle.Append(_T("mso-width-relative:outer-margin-area;"));break;
			}
			if( PROP_DEF != m_nPctHeight )
				sStyle.AppendFormat(_T("mso-height-percent:%d;"),m_nPctHeight);
			switch( m_nPctHeightRelative )
			{
				case 0:sStyle.Append(_T("mso-height-relative:margin;"));break;
				case 1:sStyle.Append(_T("mso-height-relative:page;"));break;
				case 2:sStyle.Append(_T("mso-height-relative:top-margin-area;"));break;
				case 3:sStyle.Append(_T("mso-height-relative:bottom-margin-area;"));break;
				case 4:sStyle.Append(_T("mso-height-relative:inner-margin-area;"));break;
				case 5:sStyle.Append(_T("mso-height-relative:outer-margin-area;"));break;
			}

			if( PROP_DEF != m_nRotation )
				sStyle.AppendFormat(_T("rotation:%d;"),m_nRotation / 65536 );
			else if( PROP_DEF != m_nRelRotation )
				sStyle.AppendFormat(_T("rotation:%d;"),m_nRelRotation / 65536 );

			int nZIndex = PROP_DEF;
			if( PROP_DEF != m_nRelZOrder )
				nZIndex = m_nRelZOrder;
			else if( PROP_DEF != m_nZOrder )
				nZIndex = m_nZOrder;
			else
				nZIndex = 100; //на свое усмотрение ставлю 100
			if( PROP_DEF != m_nZOrderRelative )
			{
				//берем большое значение чтобы сделать строго выше или ниже текста
				if( 0 == m_nZOrderRelative )
					sStyle.AppendFormat( _T("z-index:%d;"), 10000 + nZIndex );//Text is below shape
				else
					sStyle.AppendFormat( _T("z-index:%d;"), -10000 + nZIndex );//Shape is below text
			}
			else
				sStyle.AppendFormat( _T("z-index:%d;"), nZIndex );

			if(  PROP_DEF != m_nWrapDistLeft )
				sStyle.AppendFormat( _T("mso-wrap-distance-left:%fpt;"), RtfUtility::Twip2pt( m_nWrapDistLeft ));
			if(  PROP_DEF != m_nWrapDistTop )
				sStyle.AppendFormat( _T("mso-wrap-distance-top:%fpt;"), RtfUtility::Twip2pt( m_nWrapDistTop ));
			if(  PROP_DEF != m_nWrapDistRight ) 
				sStyle.AppendFormat( _T("mso-wrap-distance-right:%fpt;"), RtfUtility::Twip2pt( m_nWrapDistRight ));
			if(  PROP_DEF != m_nWrapDistBottom )
				sStyle.AppendFormat( _T("mso-wrap-distance-bottom:%fpt;"), RtfUtility::Twip2pt( m_nWrapDistBottom ));

			if( false == sStyle.IsEmpty() )
			{
				sStyle.Delete( sStyle.GetLength() - 1 );
                sResult.AppendFormat( _T(" style=\"%ls\""),sStyle.GetBuffer() );
			}

			if( PROP_DEF != m_bLayoutInCell )
			{
				if( 0 == m_bLayoutInCell )
					sResult.Append( _T(" o:allowincell=\"false\""));
				else
					sResult.Append( _T(" o:allowincell=\"true\""));
			}
			if( PROP_DEF != m_bAllowOverlap )
			{
				if( 0 == m_bAllowOverlap )
					sResult.Append( _T(" o:allowoverlap=\"false\""));
				else
					sResult.Append( _T(" o:allowoverlap=\"true\""));
			}
			//Position

			//Geometry
			CString sAdjust;
			if( PROP_DEF != m_nAdjustValue )
			{
				sAdjust.AppendFormat( _T("%d"),m_nAdjustValue);
				if( PROP_DEF != m_nAdjustValue2 )
				{
					sAdjust.AppendFormat( _T(",%d"),m_nAdjustValue2);
					if( PROP_DEF != m_nAdjustValue3 )
					{
						sAdjust.AppendFormat( _T(",%d"),m_nAdjustValue3);
						if( PROP_DEF != m_nAdjustValue4 )
						{
							sAdjust.AppendFormat( _T(",%d"),m_nAdjustValue4);
							if( PROP_DEF != m_nAdjustValue5 )
							{
								sAdjust.AppendFormat( _T(",%d"),m_nAdjustValue5);
								if( PROP_DEF != m_nAdjustValue6 )
								{
									sAdjust.AppendFormat( _T(",%d"),m_nAdjustValue6);
									if( PROP_DEF != m_nAdjustValue7 )
									{
										sAdjust.AppendFormat( _T(",%d"),m_nAdjustValue7);
										if( PROP_DEF != m_nAdjustValue8 )
										{
											sAdjust.AppendFormat( _T(",%d"),m_nAdjustValue8);
											if( PROP_DEF != m_nAdjustValue9 )
											{
												sAdjust.AppendFormat( _T(",%d"),m_nAdjustValue9);
												if( PROP_DEF != m_nAdjustValue10 )
												{
													sAdjust.AppendFormat( _T(",%d"),m_nAdjustValue10);
												}
											}
										}
									}
								}
							}
						}
					}
				}
            sResult.AppendFormat( _T(" adj=\"%ls\""),sAdjust.GetBuffer() );
			}
			//Geometry
			if( m_aWrapPoints.size() > 0 )
			{
				sResult.Append( _T(" wrapcoords=\""));
				sResult.AppendFormat( _T("%d"), m_aWrapPoints[0]);
				for( int i = 1; i < (int)m_aWrapPoints.size(); i++ )
					sResult.AppendFormat( _T(" %d"), m_aWrapPoints[i]);
				sResult.Append( _T("\""));
			}
			if( PROP_DEF != m_nGroupLeft && PROP_DEF != m_nGroupTop )
				sResult.AppendFormat( _T(" coordorigin=\"%d,%d\""), m_nGroupLeft, m_nGroupTop);
			if( PROP_DEF != m_nGroupLeft && PROP_DEF != m_nGroupTop && PROP_DEF != m_nGroupRight && PROP_DEF != m_nGroupBottom)
				sResult.AppendFormat( _T(" coordsize=\"%d,%d\""), m_nGroupRight - m_nGroupLeft, m_nGroupBottom - m_nGroupTop );
			if( true == m_bIsOle )
				sResult.Append( _T(" o:ole=\"\"") );
			if( true == m_bFilled )
				sResult.Append( _T(" filled=\"t\"") );
			else
				sResult.Append( _T(" filled=\"f\"") );
			if( true == m_bLine )
				sResult.Append( _T(" stroked=\"t\"") );
			else
				sResult.Append( _T(" stroked=\"f\"") );
			if(PROP_DEF != m_nLineWidth)
				sResult.AppendFormat( _T(" strokeweight=\"%fpt\""), RtfUtility::Emu2Pt(m_nLineWidth) );
			sResult.Append( _T(">") );
			if( PROP_DEF != m_nWrapType && 3 != m_nWrapType)
			{
				sResult.Append( _T("<w10:wrap"));
				switch( m_nWrapType )
				{
					case 1:sResult.Append( _T(" type=\"topAndBottom\""));break;
					case 2:sResult.Append( _T(" type=\"square\""));break;
					case 3:sResult.Append( _T(" type=\"none\""));break;
					case 4:sResult.Append( _T(" type=\"tight\""));break;
					case 5:sResult.Append( _T(" type=\"through\""));break;
				}
				switch( m_nWrapSideType )
				{
					case 0:sResult.Append( _T(" side=\"both\""));break;
					case 1:sResult.Append( _T(" side=\"left\""));break;
					case 2:sResult.Append( _T(" side=\"right\""));break;
					case 3:sResult.Append( _T(" side=\"largest\""));break;
				}
				sResult.Append( _T("/>"));
			}
			
			//Line
			CString sStroke;
			if( PROP_DEF != m_nLineStartArrow )
			{
				switch( m_nLineStartArrow )
				{
					case 0: sStroke.Append( _T(" startarrow=\"none\"") ); break;
					case 1: sStroke.Append( _T(" startarrow=\"block\"") ); break;
					case 2: sStroke.Append( _T(" startarrow=\"classic\"") ); break;
					case 3: sStroke.Append( _T(" startarrow=\"diamond\"") ); break;
					case 4: sStroke.Append( _T(" startarrow=\"oval\"") ); break;
					case 5: sStroke.Append( _T(" startarrow=\"open\"") ); break;
					case 6: sStroke.Append( _T(" startarrow=\"block\"") ); break;
					case 7: sStroke.Append( _T(" startarrow=\"block\"") ); break;
				}
			}
			if( PROP_DEF != m_nLineStartArrowLength )
			{
				switch( m_nLineStartArrowLength )
				{
					case 0: sStroke.Append( _T(" startarrowlength=\"short\"") ); break;
					case 1: sStroke.Append( _T(" startarrowlength=\"medium\"") ); break;
					case 2: sStroke.Append( _T(" startarrowlength=\"long\"") ); break;
				}
			}
			if( PROP_DEF != m_nLineStartArrowWidth )
			{
				switch( m_nLineStartArrowWidth )
				{
					case 0: sStroke.Append( _T(" startarrowwidth=\"narrow\"") ); break;
					case 1: sStroke.Append( _T(" startarrowwidth=\"medium\"") ); break;
					case 2: sStroke.Append( _T(" startarrowwidth=\"wide\"") ); break;
				}
			}
			if( PROP_DEF != m_nLineEndArrow )
			{
				switch( m_nLineEndArrow )
				{
					case 0: sStroke.Append( _T(" endarrow=\"none\"") ); break;
					case 1: sStroke.Append( _T(" endarrow=\"block\"") ); break;
					case 2: sStroke.Append( _T(" endarrow=\"classic\"") ); break;
					case 3: sStroke.Append( _T(" endarrow=\"diamond\"") ); break;
					case 4: sStroke.Append( _T(" endarrow=\"oval\"") ); break;
					case 5: sStroke.Append( _T(" endarrow=\"open\"") ); break;
					case 6: sStroke.Append( _T(" endarrow=\"block\"") ); break;
					case 7: sStroke.Append( _T(" endarrow=\"block\"") ); break;
				}
			}
			if( PROP_DEF != m_nLineEndArrowLength )
			{
				switch( m_nLineEndArrowLength )
				{
					case 0: sStroke.Append( _T(" endarrowlength=\"short\"") ); break;
					case 1: sStroke.Append( _T(" endarrowlength=\"medium\"") ); break;
					case 2: sStroke.Append( _T(" endarrowlength=\"long\"") ); break;
				}
			}
			if( PROP_DEF != m_nLineEndArrowWidth )
			{
				switch( m_nLineEndArrowWidth )
				{
					case 0: sStroke.Append( _T(" endarrowwidth=\"narrow\"") ); break;
					case 1: sStroke.Append( _T(" endarrowwidth=\"medium\"") ); break;
					case 2: sStroke.Append( _T(" endarrowwidth=\"wide\"") ); break;
				}
			}
			if( false == sStroke.IsEmpty() )
                sResult.AppendFormat( _T("<v:stroke %ls/>"), sStroke.GetBuffer() );

			if( 0 != m_aTextItems )
			{
				RenderParameter oNewParam = oRenderParameter;
				oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;

				sResult.AppendFormat( _T("<v:textbox inset=\"%.2fpt,%.2fpt,%.2fpt,%.2fpt\">"), RtfUtility::Emu2Pt(m_nTexpLeft), RtfUtility::Emu2Pt(m_nTexpTop), RtfUtility::Emu2Pt(m_nTexpRight), RtfUtility::Emu2Pt(m_nTexpBottom) );
				sResult.Append( _T("<w:txbxContent>") );
				sResult.Append( m_aTextItems->RenderToOOX(oNewParam) );
				sResult.Append( _T("</w:txbxContent>") );
				sResult.Append( _T("</v:textbox>") );
			}
			if( m_oPicture != 0 )
			{
				int nCropLeft = PROP_DEF;
				int nCropTop = PROP_DEF;
				int nCropRight = PROP_DEF;
				int nCropBottom = PROP_DEF;

				if( PROP_DEF != m_nCropFromLeft )
					nCropLeft = m_nCropFromLeft;
				else if( PROP_DEF != m_oPicture->m_nWidthGoal && PROP_DEF != m_oPicture->m_nCropL )
					nCropLeft = (int)( 65536 * ( 1.0 * m_oPicture->m_nCropL / m_oPicture->m_nWidthGoal) ); //This numeric value can also be specified in 1/65536-ths if a trailing "f" is supplied
				if( PROP_DEF != m_nCropFromTop )
					nCropTop = m_nCropFromTop;
				else if( PROP_DEF != m_oPicture->m_nHeightGoal && PROP_DEF != m_oPicture->m_nCropT )
					nCropTop = (int)( 65536 * ( 1.0 * m_oPicture->m_nCropT / m_oPicture->m_nHeightGoal) );
				if( PROP_DEF != m_nCropFromRight )
					nCropRight = m_nCropFromRight;
				else if( PROP_DEF != m_oPicture->m_nWidthGoal && PROP_DEF != m_oPicture->m_nCropR )
					nCropRight = (int)( 65536 * ( 1.0 * m_oPicture->m_nCropR / m_oPicture->m_nWidthGoal) );
				if( PROP_DEF != m_nCropFromBottom )
					nCropBottom = m_nCropFromBottom;
				else if( PROP_DEF != m_oPicture->m_nHeightGoal && PROP_DEF != m_oPicture->m_nCropB )
					nCropBottom = (int)( 65536 * ( 1.0 * m_oPicture->m_nCropB / m_oPicture->m_nHeightGoal) );

				CString sPicture = m_oPicture->RenderToOOX(oRenderParameter);
				if( _T("") == sPicture )//если не сохранилась картинка, то весь shape будет бесполезным
					return _T("");
                sResult.AppendFormat( _T("<v:imagedata r:id=\"%ls\""), sPicture.GetBuffer() );
				if( PROP_DEF != nCropLeft )
					sResult.AppendFormat( _T(" cropleft=\"%df\""), nCropLeft );
				if( PROP_DEF != nCropTop )
					sResult.AppendFormat( _T(" croptop=\"%df\""), nCropTop );
				if( PROP_DEF != nCropRight )
					sResult.AppendFormat( _T(" cropright=\"%df\""), nCropRight );
				if( PROP_DEF != nCropBottom )
					sResult.AppendFormat( _T(" cropbottom=\"%df\""), nCropBottom );
				sResult.Append( _T(" o:title=\"\"/>") );
			}
	return sResult;
}
CString RtfShape::RenderToOOXEnd(RenderParameter oRenderParameter)
{
	CString sResult;
	if( _T("") == oRenderParameter.sValue )
		sResult.Append( _T("</v:shape>") );
	else
        sResult.AppendFormat( _T("</%ls>"), oRenderParameter.sValue.GetBuffer() );
	if( RENDER_TO_OOX_PARAM_SHAPE_WSHAPE2 == oRenderParameter.nType )
		;
	else if( RENDER_TO_OOX_PARAM_SHAPE_WSHAPE == oRenderParameter.nType )
		sResult.Append(_T("</w:pict>"));
	else
		sResult.Append(_T("</w:pict></w:r>"));//работает по умолчанию
	return sResult;
}
CString RtfShapeGroup::RenderToRtf(RenderParameter oRenderParameter)
{
	return RtfShape::RenderToRtf(  oRenderParameter  );
}
CString RtfShapeGroup::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult;
	RenderParameter oNewParamGroup = oRenderParameter;
	oNewParamGroup.sValue = _T("v:group");
	RenderParameter oNewParamShape = oRenderParameter;
	oNewParamShape.sValue = _T("v:shape");
	sResult = RenderToOOXBegin( oNewParamGroup );
	for( int i = 0; i < (int)m_aArray.size(); i++ )
		sResult.Append( m_aArray[i]->RenderToOOX( oNewParamShape ) );
	sResult.Append( RenderToOOXEnd( oNewParamGroup ) );
	return sResult;
}
