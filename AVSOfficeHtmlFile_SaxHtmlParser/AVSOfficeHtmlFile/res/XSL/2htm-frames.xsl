<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
  xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
  xmlns:v="urn:schemas-microsoft-com:vml"
  xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
  xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:w10="urn:schemas-microsoft-com:office:word"
  xmlns:o="urn:schemas-microsoft-com:office:office"
  xmlns:xlink="http://www.w3.org/1999/xlink"
  xmlns:dc="http://purl.org/dc/elements/1.1/" xmlns:pzip="urn:cleverage:xmlns:post-processings:zip"
  xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main"
  xmlns:pic="http://schemas.openxmlformats.org/drawingml/2006/picture"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns:wp="http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing"
  xmlns:number="urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0"
  xmlns:oox="urn:oox"
  xmlns:rels="http://schemas.openxmlformats.org/package/2006/relationships"
                xmlns="http://www.w3.org/1999/xhtml"
  exclude-result-prefixes="w r draw number wp xlink v w10 o rels oox">

	<!-- 
  *************************************************************************
  MATCHING TEMPLATES
  *************************************************************************
  -->
	<xsl:import href="measures.xsl"/>

	<!-- 
  Summary: Converts frames
  Author: Clever Age
  -->
	<xsl:template match="w:p[w:pPr/w:framePr]">
		<xsl:choose>
			<xsl:when test="w:pPr/w:framePr[@w:dropCap='drop']">
				<!--Do nothing-->
			</xsl:when>
			<xsl:when test="w:pPr/w:framePr[@w:dropCap='margin']">
				<xsl:message terminate="no">translation.oox2htm.dropcap.inMargin</xsl:message>
			</xsl:when>
			<xsl:otherwise>
				<!-- 
        check if the preceding framePr is identically 
        If it is, then it's w:p belongs to the same frame and 
        this frame was already merged in to the previous.
        -->
				<xsl:variable name="precedingP" select="./preceding-sibling::*[1]" />
				<xsl:variable name="identically">
					<xsl:call-template name="CompareFrames">
						<xsl:with-param name="frame1" select="./w:pPr/w:framePr"/>
						<xsl:with-param name="frame2" select="$precedingP/w:pPr/w:framePr"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:if test="$identically='false'">
					<!--
          makz: If the frame is anchored to the page insert only the frame
          else insert the paragraph and the frame.
          
          A frame which iss not aligned to the page (in horiz. or vert. direction) 
          needs a paragraph due to it's anchor.

          Frames in header/footer need a surrounding text:p, otherwise, OpenOffice will not show them
          -->
					<xsl:choose>
						<xsl:when test="ancestor::*[name()='w:hdr'] or ancestor::*[name()='w:ftr']">
							<p>
								<xsl:call-template name="InsertFrame">
									<xsl:with-param name="framePr" select="w:pPr/w:framePr" />
								</xsl:call-template>
								<xsl:text>&#160;</xsl:text>
							</p>
						</xsl:when>
						<xsl:when test="w:pPr/w:framePr/@w:hAnchor='page' and w:pPr/w:framePr/@w:vAnchor='page'">
							<xsl:call-template name="InsertFrame">
								<xsl:with-param name="framePr" select="w:pPr/w:framePr" />
							</xsl:call-template>
						</xsl:when>
						<xsl:when test=".//w:commentReference or .//w:footnoteReference or .//w:endnoteReference or .//w:fldChar or .//w:fldSimple">
							<xsl:call-template name="InsertParagraphToFrame"/>
						</xsl:when>
						<xsl:otherwise>
							<p>
								<xsl:attribute name="class">
									<xsl:choose>
										<xsl:when test="w:pPr/w:pStyle/@w:val">
											<xsl:value-of select="w:pPr/w:pStyle/@w:val"/>
										</xsl:when>
										<xsl:otherwise>
											<xsl:value-of select="generate-id()"/>
										</xsl:otherwise>
									</xsl:choose>
								</xsl:attribute>
								<xsl:call-template name="InsertFrame">
									<xsl:with-param name="framePr" select="w:pPr/w:framePr" />
								</xsl:call-template>
								<xsl:text>&#160;</xsl:text>
							</p>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:if>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<!--
  Summary:  Sometimes Word puts "invisible" frames immediatly after table that is inserted in another table.
            I can only suppose that this is a workaround for preventing some display issues.
            These frames shall not be converted.
            Inserted for bugfix 1948030.
  Author:   makz (DIaLOGIKa)
  -->
	<xsl:template match="w:p[w:pPr/w:framePr and w:pPr/w:suppressOverlap and ancestor::w:tc]">
		<!-- do nothing -->
	</xsl:template>


	<!--
  Summary: Convert the frame properties into automatic styles
  Author: Clever Age
  -->
	<xsl:template match="w:p[w:pPr/w:framePr]" mode="automaticstyles">
		<xsl:choose>
			<xsl:when test="w:pPr/w:framePr[@w:dropCap='drop']">
				<!--Do nothing-->
			</xsl:when>
			<xsl:when test="w:pPr/w:framePr[@w:dropCap='margin']">
				<xsl:message terminate="no">translation.oox2htm.dropcap.inMargin</xsl:message>
			</xsl:when>
		</xsl:choose>
		<xsl:apply-templates mode="automaticstyles"/>
	</xsl:template>


	<!-- 
  *************************************************************************
  CALLED TEMPLATES
  *************************************************************************
  -->

	<!--
  Summary:  Inserts a draw:frame
  Author:   makz (DIaLOGIKa)
  Params:   framePr: The properties of the paragraph frame
  -->
	<xsl:template name="InsertFrame">
		<xsl:param name="framePr" />

		<div>
			<xsl:variable name="x">
				<xsl:if test="$framePr/@w:x">
					<xsl:call-template name="ConvertTwips">
						<xsl:with-param name="length" select="$framePr/@w:x"/>
						<xsl:with-param name="unit" select="'cm'"/>
					</xsl:call-template>
				</xsl:if>
			</xsl:variable>
			<xsl:variable name="y">
				<xsl:if test="$framePr/@w:y">
					<xsl:variable name="pageHeights">
						<xsl:call-template name="calculatePageHeights">
							<xsl:with-param name="node" select="."/>
						</xsl:call-template>
					</xsl:variable>
					<xsl:call-template name="ConvertTwips">
						<xsl:with-param name="length">
							<xsl:value-of select="$pageHeights + $framePr/@w:y"/>
						</xsl:with-param>
						<xsl:with-param name="unit" select="'cm'"/>
					</xsl:call-template>
				</xsl:if>
			</xsl:variable>

			<xsl:if test="$x and $y">
				<xsl:variable name="style">
					<xsl:text>position:absolute;</xsl:text>
					<xsl:value-of select="concat('left:', $x, ';')"/>
					<xsl:value-of select="concat('top:', $y, ';')"/>
				</xsl:variable>
				<xsl:attribute name="style">
					<xsl:value-of select="$style"/>
				</xsl:attribute>
			</xsl:if>
			<xsl:call-template name="InsertParagraphToFrame"/>
		</div>
		<!--<div>
			<xsl:variable name="x">
				<xsl:if test="$framePr/@w:x">
					<xsl:call-template name="ConvertTwips">
						<xsl:with-param name="length" select="$framePr/@w:x"/>
						<xsl:with-param name="unit" select="'cm'"/>
					</xsl:call-template>
				</xsl:if>
			</xsl:variable>
			<xsl:if test="$x">
				<xsl:variable name="style">
					<xsl:value-of select="concat('margin-left:', $x, ';')"/>
				</xsl:variable>
				<xsl:attribute name="style">
					<xsl:value-of select="$style"/>
				</xsl:attribute>
			</xsl:if>

			<xsl:call-template name="InsertParagraphToFrame"/>
		</div>  -->
	</xsl:template>


	<xsl:template name="calculatePageHeights">
		<xsl:param name="sum" select="0"/>
		<xsl:param name="node" select="."/>

		<xsl:value-of select="sum(preceding::w:sectPr/w:pgSz/@w:h)"/>
	</xsl:template>

	<!--
  Summary:  Inserts the width and height values for a draw:frame
  Author:   makz (DIaLOGIKa)
  Params:   framePr: The properties of the paragraph frame
  -->
	<xsl:template name="InsertFrameSize">
		<xsl:param name="framePr" />

		<xsl:choose>
			<xsl:when test="$framePr/@w:w">
				<!-- If a size is specifed, use the width attribute -->
				<xsl:attribute name="width">
					<xsl:call-template name="ConvertMeasure">
						<xsl:with-param name="length">
							<xsl:value-of select="$framePr/@w:w"/>
						</xsl:with-param>
						<xsl:with-param name="destUnit" select="'cm'"/>
					</xsl:call-template>
				</xsl:attribute>
			</xsl:when>
			<xsl:otherwise>
				<!-- If no width is specified, the frame is autosized, so use the min-width attribute -->
				<xsl:attribute name="width">
					<xsl:text>0cm</xsl:text>
				</xsl:attribute>
			</xsl:otherwise>
		</xsl:choose>

		<xsl:attribute name="height">
			<xsl:call-template name="ConvertMeasure">
				<xsl:with-param name="length">
					<xsl:value-of select="$framePr/@w:h"/>
				</xsl:with-param>
				<xsl:with-param name="destUnit" select="'cm'"/>
			</xsl:call-template>
		</xsl:attribute>
	</xsl:template>


	<!--
  Summary:  Inserts the absolute position for a draw:frame
  Author:   makz (DIaLOGIKa)
  Params:   framePr: The properties of the paragraph frame
  -->
	<xsl:template name="InsertFramePosition">
		<xsl:param name="framePr" />

		<xsl:attribute name="left">
			<xsl:call-template name="ConvertMeasure">
				<xsl:with-param name="length">
					<xsl:value-of select="$framePr/@w:x"/>
				</xsl:with-param>
				<xsl:with-param name="destUnit" select="'cm'"/>
			</xsl:call-template>
		</xsl:attribute>

		<xsl:attribute name="top">
			<xsl:call-template name="ConvertMeasure">
				<xsl:with-param name="length">
					<xsl:value-of select="$framePr/@w:y"/>
				</xsl:with-param>
				<xsl:with-param name="destUnit" select="'cm'"/>
			</xsl:call-template>
		</xsl:attribute>
	</xsl:template>


	<!--
  Summary:  Inserts the anchor type for a draw:frame
  Author:   makz (DIaLOGIKa)
  Params:   framePr: The properties of the paragraph frame
  -->
	<xsl:template name="InsertFrameAnchor">
		<xsl:param name="framePr" />

		<!--
    OOX knows a vertical anchor and a horizontal anchor.
    ODF only knows a anchor type.
    -->
		<xsl:attribute name="text:anchor-type">
			<xsl:choose>

				<xsl:when test="ancestor::*[name()='w:hdr'] or ancestor::*[name()='w:ftr']">
					<xsl:text>paragraph</xsl:text>
				</xsl:when>
				<xsl:when test="$framePr/@w:vAnchor='text' or $framePr/@w:hAnchor='text'">
					<xsl:text>paragraph</xsl:text>
				</xsl:when>
				<xsl:when test="$framePr/@w:vAnchor='margin' or $framePr/@w:hAnchor='margin'">
					<xsl:text>paragraph</xsl:text>
				</xsl:when>
				<xsl:when test="$framePr/@w:vAnchor='page' and $framePr/@w:hAnchor='page'">
					<xsl:text>page</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>paragraph</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:attribute>
	</xsl:template>


	<!--
  Summary:  Inserts the vertical and horizontal relation and position values into the style of a draw:frame
  Author:   Clever Age
  Modified: makz (DIaLOGIKa)
  Params:   framePr: The properties of the paragraph frame
  -->
	<xsl:template name="InsertFrameAnchorForStyle">
		<xsl:param name="framePr"/>

		<!-- Translation Rules                   -->
		<!-- w:wrap    =>  style:wrap            -->
		<!-- w:yAlign  =>  style:vertical-pos    -->
		<!-- w:vAnchor =>  style:vertical-rel    -->
		<!-- w:xAlign  =>  style:horizontal-pos  -->
		<!-- w:hAnchor =>  style:horizontal-rel  -->
		<xsl:variable name="Wrap"     select = "$framePr/@w:wrap"/>
		<xsl:variable name="yAlign"   select = "$framePr/@w:yAlign"/>
		<xsl:variable name="vAnchor"  select = "$framePr/@w:vAnchor"/>
		<xsl:variable name="xAlign"   select = "$framePr/@w:xAlign"/>
		<xsl:variable name="hAnchor"  select = "$framePr/@w:hAnchor"/>

		<xsl:call-template name="InsertWrap">
			<xsl:with-param name="wrap" select="$Wrap" />
		</xsl:call-template>

		<xsl:if test ="count($yAlign)>0 or count($vAnchor)>0">
			<xsl:call-template name="InsertVerticalPos">
				<xsl:with-param name="yAlign" select="$yAlign" />
				<xsl:with-param name="y" select="$framePr/@w:y" />
			</xsl:call-template>
			<xsl:call-template name="InsertVerticalRel">
				<xsl:with-param name="vRel" select="$vAnchor" />
				<xsl:with-param name="vPos" select="$yAlign" />
			</xsl:call-template>
		</xsl:if>

		<xsl:if test ="count($xAlign)>0 or count($hAnchor)>0">
			<xsl:call-template name="InsertHorizontalPos">
				<xsl:with-param name="xAlign" select="$xAlign" />
			</xsl:call-template>
			<xsl:call-template name="InsertHorizontalRel">
				<xsl:with-param name="hRel" select="$hAnchor" />
				<xsl:with-param name="hPos" select="$xAlign" />
			</xsl:call-template>
		</xsl:if>

	</xsl:template>


	<!--
  Summary:  Inserts the borders into the style of a draw:frame
  Author:   makz (DIaLOGIKa)
  Params:   pPr: The properties of the paragraph
  -->
	<xsl:template name="InsertFrameBordersForStyle">
		<xsl:param name="pPr" />

		<!--
      The border properties of a border can be defined in the automatic styles of the content.xml
      or in the styles.xml
      That happens if a predefined paragraph style is made to a frame.
      -->
		<xsl:variable name="styleId" select="$pPr/w:pStyle/@w:val"/>
		<xsl:variable name="externalBorderStyle" select="key('StyleId', $styleId)"/>

		<xsl:choose>
			<!-- The border is defined in the document -->
			<xsl:when test="$pPr/w:pBdr">
				<xsl:call-template name="InsertBorders">
					<xsl:with-param name="border" select="$pPr/w:pBdr"/>
				</xsl:call-template>
				<xsl:call-template name="InsertParagraphShadow"/>
			</xsl:when>

			<!-- The border is defined in styles.xml -->
			<xsl:when test="$externalBorderStyle/w:pPr/w:pBdr">
				<xsl:call-template name="InsertBorders">
					<xsl:with-param name="border" select="$externalBorderStyle/w:pPr/w:pBdr"/>
				</xsl:call-template>
				<xsl:call-template name="InsertParagraphShadow"/>
			</xsl:when>

			<!-- No border is defined -->
			<xsl:otherwise>
				<xsl:attribute name="fo:border">
					<xsl:text>none</xsl:text>
				</xsl:attribute>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<!--
  Summary:  Inserts the shading/background into the style of a draw:frame
  Author:   makz (DIaLOGIKa)
  Params:   pPr: The properties of the paragraph
  -->
	<xsl:template name="InsertFrameBackgroundForStyle">
		<xsl:param name="pPr" />

		<xsl:choose>
			<!-- The shading is defined in the document -->
			<xsl:when test="$pPr/w:shd">

				<xsl:attribute name="fo:background-color">
					<xsl:variable name="fill">
						<xsl:choose>
							<xsl:when test="$pPr/w:shd/@w:fill = 'auto'">
								<xsl:text>000000</xsl:text>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="$pPr/w:shd/@w:fill"/>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:variable>

					<xsl:value-of select="concat('#', $fill)"/>
				</xsl:attribute>

				<xsl:attribute name="style:background-transparency">
					<xsl:variable name="percent">
						<xsl:choose>
							<xsl:when test="$pPr/w:shd/@w:val='clear' or $pPr/w:shd/@w:val='nil' or $pPr/w:shd/@w:val='solid'">
								<xsl:text>0</xsl:text>
							</xsl:when>
							<xsl:when test="substring($pPr/w:shd/@w:val, 0, 3) = 'pct'">
								<xsl:value-of select="substring($pPr/w:shd/@w:val, 4, 2)"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:text>0</xsl:text>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:variable>

					<xsl:value-of select ="concat($percent, '%')"/>
				</xsl:attribute>

			</xsl:when>
		</xsl:choose>

	</xsl:template>


	<!--
  Summary:  Inserts the margin into the style of a draw:frame
  Author:   makz (DIaLOGIKa)
  Params:   framePr: The properties of the paragraph frame
  -->
	<xsl:template name="InsertFrameMarginForStyle">
		<xsl:param name="framePr" />

		<!-- vertical margin -->
		<xsl:variable name="vertMargin">
			<xsl:choose>
				<xsl:when test="$framePr/@w:vSpace">
					<xsl:call-template name="ConvertTwips">
						<xsl:with-param name="length" select="$framePr/@w:vSpace" />
						<xsl:with-param name="unit" select="'cm'" />
					</xsl:call-template>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>0cm</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<!-- horizontal margin -->
		<xsl:variable name="horzMargin">
			<xsl:choose>
				<xsl:when test="$framePr/@w:hSpace">
					<xsl:call-template name="ConvertTwips">
						<xsl:with-param name="length" select="$framePr/@w:hSpace" />
						<xsl:with-param name="unit" select="'cm'" />
					</xsl:call-template>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>0cm</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:attribute name="fo:margin-bottom">
			<xsl:value-of select="$vertMargin"/>
		</xsl:attribute>

		<xsl:attribute name="fo:margin-top">
			<xsl:value-of select="$vertMargin"/>
		</xsl:attribute>

		<xsl:attribute name="fo:margin-left">
			<xsl:value-of select="$horzMargin"/>
		</xsl:attribute>

		<xsl:attribute name="fo:margin-right">
			<xsl:value-of select="$horzMargin"/>
		</xsl:attribute>

	</xsl:template>


	<!--
  Summary: Inserts the paragraphs in a frame
  Author: Clever Age
  -->
	<xsl:template name="InsertParagraphToFrame">
		<xsl:param name="paragraph" select="."/>

		<p>
			<xsl:attribute name="class">
				<xsl:text>style_</xsl:text>
				<xsl:choose>
					<xsl:when test="$paragraph/w:pPr/w:pStyle/@w:val">
						<xsl:value-of select="$paragraph/w:pPr/w:pStyle/@w:val"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="generate-id($paragraph)"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>
			<xsl:apply-templates select="$paragraph/node()"/>
			<xsl:text>&#160;</xsl:text>
		</p>

		<!-- if there is another paragraph with framePr -->
		<xsl:variable name="followingP" select="$paragraph/following-sibling::*[1]" />
		<xsl:if test="$followingP/w:pPr/w:framePr">
			<!-- check if the folowing framePr is identically -->
			<xsl:variable name="identically">
				<xsl:call-template name="CompareFrames">
					<xsl:with-param name="frame1" select="$paragraph/w:pPr/w:framePr"/>
					<xsl:with-param name="frame2" select="$followingP/w:pPr/w:framePr"/>
				</xsl:call-template>
			</xsl:variable>
			<!-- then merge the two paragraphs -->
			<xsl:if test="$identically='true'">
				<xsl:call-template name="InsertParagraphToFrame">
					<xsl:with-param name="paragraph" select="$followingP"/>
				</xsl:call-template>
			</xsl:if>
		</xsl:if>
	</xsl:template>


	<!--
  Summary: Inserts the attribute for vertical position
  Author: makz (DIaLOGIKa)
  Date: 15.11.2007
  -->
	<xsl:template name="InsertVerticalPos">
		<xsl:param name="yAlign" />
		<xsl:param name="y" />

		<xsl:attribute name="style:vertical-pos">
			<xsl:choose>
				<xsl:when test="$yAlign='bottom'">
					<xsl:text>bottom</xsl:text>
				</xsl:when>
				<xsl:when test="$yAlign='top'">
					<xsl:choose>
						<!--
              makz: If there is a y coordinate then OOo needs "from-top" 
              because "top" cannot have coordinates.
              -->
						<xsl:when test="$y">
							<xsl:text>from-top</xsl:text>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>top</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
				<xsl:when test="$yAlign='center'">
					<xsl:text>middle</xsl:text>
				</xsl:when>
				<!--
          makz: If no yAlign is specified in OOX document use "from-top"
          -->
				<xsl:otherwise>
					<xsl:text>from-top</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:attribute>
	</xsl:template>


	<!--
  Summary: Inserts the attribute for horizontal position
  Author: makz (DIaLOGIKa)
  Date: 15.11.2007
  -->
	<xsl:template name="InsertHorizontalPos">
		<xsl:param name="xAlign" />

		<xsl:attribute name="style:horizontal-pos">
			<xsl:choose>
				<xsl:when test="$xAlign='center'">
					<xsl:text>middle</xsl:text>
				</xsl:when>
				<xsl:when test="$xAlign='inside'">
					<xsl:text>left</xsl:text>
				</xsl:when>
				<xsl:when test="$xAlign='outside'">
					<xsl:text>right</xsl:text>
				</xsl:when>
				<xsl:when test="$xAlign='left'">
					<xsl:text>left</xsl:text>
				</xsl:when>
				<xsl:when test="$xAlign='right'">
					<xsl:text>right</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>from-left</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:attribute >
	</xsl:template>


	<!--
  Summary: Inserts the attribute for horizontal relation
  Author: makz (DIaLOGIKa)
  Date: 15.11.2007
  -->
	<xsl:template name="InsertHorizontalRel">
		<xsl:param name="hRel" />
		<xsl:param name="hPos" />

		<xsl:attribute name="style:horizontal-rel">
			<xsl:choose>
				<xsl:when test="$hRel='margin'">
					<xsl:text>paragraph-content</xsl:text>
				</xsl:when>
				<xsl:when test="$hRel='page'">
					<xsl:text>page</xsl:text>
				</xsl:when>
				<xsl:when test="$hRel='text'">
					<xsl:text>page-content</xsl:text>
				</xsl:when>
				<xsl:when test="$hRel=''">
					<!-- 
          if no relation is set, Word uses default values, 
          depeding on the position
          -->
					<xsl:choose>
						<!-- 
            If no position is set, it is absolute positioning.
            In this case the default relation is the paragraph-content 
            -->
						<xsl:when test="$hPos=''">
							<xsl:text>paragraph-content</xsl:text>
						</xsl:when>
						<!-- 
            If a position is set, it is relative positioning.
            In this case the default relation is the page-content 
            -->
						<xsl:otherwise>
							<xsl:text>page-content</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
			</xsl:choose>
		</xsl:attribute>
	</xsl:template>


	<!--
  Summary: Inserts the attribute for vertical relation
  Author: makz (DIaLOGIKa)
  Date: 15.11.2007
  -->
	<xsl:template name="InsertVerticalRel">
		<xsl:param name="vRel" />
		<xsl:param name="vPos" />

		<xsl:attribute name="style:vertical-rel">
			<xsl:choose>
				<xsl:when test="$vRel='page'">
					<xsl:text>page</xsl:text>
				</xsl:when>
				<xsl:when test="$vRel='text'">
					<xsl:text>baseline</xsl:text>
				</xsl:when>
				<xsl:when test="$vRel=''">
					<!-- 
          if no relation is set, Word uses default values, 
          depeding on the position
          -->
					<xsl:choose>
						<!-- 
            If no position is set, it is absolute positioning.
            In this case the default relation is the paragraph-content 
            -->
						<xsl:when test="$vPos=''">
							<xsl:text>paragraph-content</xsl:text>
						</xsl:when>
						<!-- 
            If a position is set, it is relative positioning.
            In this case the default relation is the page-content 
            -->
						<xsl:otherwise>
							<xsl:text>page-content</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>page-content</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:attribute>
	</xsl:template>


	<!--
  Summary: Inserts the attribute for wrap
  Author: makz (DIaLOGIKa)
  Date: 15.11.2007
  -->
	<xsl:template name="InsertWrap">
		<xsl:param name="wrap" />

		<xsl:attribute name="style:wrap">
			<xsl:choose>
				<xsl:when test="$wrap ='around'">
					<xsl:text>parallel</xsl:text>
				</xsl:when>
				<xsl:when test="$wrap ='auto'">
					<xsl:text>parallel</xsl:text>
				</xsl:when>
				<xsl:when test="$wrap ='none'">
					<xsl:text>run-through</xsl:text>
				</xsl:when>
				<xsl:when test="$wrap ='notBeside'">
					<xsl:text>none</xsl:text>
				</xsl:when>
				<xsl:when test="$wrap ='through'">
					<xsl:text>parallel</xsl:text>
				</xsl:when>
				<xsl:when test="$wrap ='tight'">
					<xsl:text>parallel</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<!-- not specified -->
					<xsl:text>parallel</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:attribute>
	</xsl:template>


	<!--
  Summary:  Compares 2 OOX frames based on their attributes
  Author:   makz (DIaLOGIKa)
  Date:     29.10.2007
  -->
	<xsl:template name="CompareFrames">
		<xsl:param name="frame1" />
		<xsl:param name="frame2" />

		<xsl:choose>
			<xsl:when test="string($frame1/@w:anchorLock) = string($frame2/@w:anchorLock) and 
                      string($frame1/@w:dropCap) = string($frame2/@w:dropCap) and 
                      string($frame1/@w:h) = string($frame2/@w:h) and 
                      string($frame1/@w:hAnchor) = string($frame2/@w:hAnchor) and 
                      string($frame1/@w:hRule) = string($frame2/@w:hRule) and 
                      string($frame1/@w:hSpace) = string($frame2/@w:hSpace) and 
                      string($frame1/@w:lines) = string($frame2/@w:lines) and            
                      string($frame1/@w:vAnchor) = string($frame2/@w:vAnchor) and 
                      string($frame1/@w:vSpace) = string($frame2/@w:vSpace) and 
                      string($frame1/@w:w) = string($frame2/@w:w) and 
                      string($frame1/@w:wrap) = string($frame2/@w:wrap) and 
                      string($frame1/@w:x) = string($frame2/@w:x) and 
                      string($frame1/@w:xAlign) = string($frame2/@w:xAlign) and 
                      string($frame1/@w:y) = string($frame2/@w:y) and 
                      string($frame1/@w:yAlign) = string($frame2/@w:yAlign)">
				<xsl:text>true</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>false</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<!--
  Summary:  Inserts a flow attribute into the style of a draw:frame
  Author:   makz (DIaLOGIKa)
  -->
	<xsl:template name="InsertFrameFlowForStyle">
		<!--
    ODF Spec says:
    The style:flow-with-text attribute specifies the behavior of 
    drawing shapes that are positioned at a certain distance below an anchor 
    and do not fit on the page where the anchor is. 
    If the value of the property is true, such drawing objects follow the text flow, 
    that is, they a displayed on the next page. If the attribute value is false, 
    such drawing objects are displayed outside the page's text area.
    -->
		<!--
    makz: 
    Setting this to false will emulate the behavior of Word.
    Frames in headers/footers MUST have flow-with-text: false, 
    but I think it doesn't affect frames in main document, that's why it will always be written.
    -->
		<xsl:attribute name="style:flow-with-text">
			<xsl:text>false</xsl:text>
		</xsl:attribute>
	</xsl:template>
</xsl:stylesheet>
