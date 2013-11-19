<?xml version="1.0" encoding="UTF-8"?>
<!--
  * Copyright (c) 2006, Clever Age
  * All rights reserved.
  * 
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions are met:
  *
  *     * Redistributions of source code must retain the above copyright
  *       notice, this list of conditions and the following disclaimer.
  *     * Redistributions in binary form must reproduce the above copyright
  *       notice, this list of conditions and the following disclaimer in the
  *       documentation and/or other materials provided with the distribution.
  *     * Neither the name of Clever Age nor the names of its contributors 
  *       may be used to endorse or promote products derived from this software
  *       without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
  * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
  * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"
  xmlns:wp="http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing"
  xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main"
  xmlns:pic="http://schemas.openxmlformats.org/drawingml/2006/picture"
  xmlns:uri="http://schemas.openxmlformats.org/drawingml/2006/picture"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
  xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
  xmlns:xlink="http://www.w3.org/1999/xlink"
  xmlns:o="urn:schemas-microsoft-com:office:office"
  xmlns:pzip="urn:cleverage:xmlns:post-processings:zip"
  xmlns="http://schemas.openxmlformats.org/package/2006/relationships"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
  xmlns:v="urn:schemas-microsoft-com:vml"
  xmlns:oox="urn:oox"
  xmlns:rels="http://schemas.openxmlformats.org/package/2006/relationships"
  exclude-result-prefixes="w uri draw a pic r rels wp w xlink oox rels">

	<!--
  *************************************************************************
  SUMMARY
  *************************************************************************
  This stylesheet handles the conversion of DrawingML pictures.
  *************************************************************************
  -->

	<!-- 
  *************************************************************************
  MATCHING TEMPLATES
  *************************************************************************
  -->

	<xsl:template match="w:drawing">
		<!--<span>-->
		<!--<xsl:variable name="pictureProperties">
                <xsl:call-template name="InsertPictureProperties">
                    <xsl:with-param name="drawing" select="." />
                </xsl:call-template>
            </xsl:variable>
            <xsl:if test="$pictureProperties != ''">
                <xsl:attribute name="style">
                    <xsl:value-of select="$pictureProperties"/>
                </xsl:attribute>
            </xsl:if>-->
		<xsl:apply-templates select="wp:inline | wp:anchor"/>
		<!--</span>-->
	</xsl:template>

	<xsl:template match="w:drawing" mode="automaticstyles">
		<!--<style type="text/css">
            <xsl:text> img.</xsl:text>
            <xsl:value-of select="generate-id(.)"/>
            <xsl:text> {</xsl:text>

            <xsl:call-template name="InsertPictureProperties">
                <xsl:with-param name="drawing" select="." />
            </xsl:call-template>
            <xsl:text>} </xsl:text>
        </style>-->
	</xsl:template>

	<xsl:template match="w:drawing[descendant::a:hlinkClick]">
		<a>
			<xsl:attribute name="href">
				<xsl:variable name="relationshipId" select="descendant::a:hlinkClick/@r:id"/>
				<xsl:variable name="document">
					<xsl:call-template name="GetDocumentName"/>
				</xsl:variable>
				<xsl:variable name="relDestination">
					<xsl:call-template name="GetTarget">
						<xsl:with-param name="document" select="$document"/>
						<xsl:with-param name="id" select="$relationshipId"/>
					</xsl:call-template>
				</xsl:variable>

				<xsl:call-template name="GetLinkPath">
					<xsl:with-param name="linkHref" select="$relDestination"/>
				</xsl:call-template>
			</xsl:attribute>
			<xsl:apply-templates select="wp:inline | wp:anchor"/>
		</a>

	</xsl:template>

	<xsl:template match="wp:inline | wp:anchor">
		<xsl:variable name="document">
			<xsl:call-template name="GetDocumentName"/>
		</xsl:variable>

		<!--<xsl:call-template name="CopyPictures">
      <xsl:with-param name="document">
        <xsl:value-of select="$document"/>
      </xsl:with-param>
    </xsl:call-template>-->

		<xsl:if test="wp:cNvGraphicFramePr/a:graphicFrameLocks/@noChangeAspect">
			<xsl:message terminate="no">translation.oox2htm.picture.size.lockAspectRation</xsl:message>
		</xsl:if>

		<xsl:if test="a:graphic/a:graphicData/pic:pic/pic:nvPicPr/pic:cNvPicPr/@preferRelativeResize">
			<xsl:message terminate="no">translation.oox2htm.frame.relativeSize</xsl:message>
		</xsl:if>

		<xsl:if test="@locked = '1'">
			<xsl:message terminate="no">translation.oox2htm.picture.anchor.lock</xsl:message>
		</xsl:if>

		<xsl:if test="@allowOverlap = '1'">
			<xsl:message terminate="no">translation.oox2htm.picture.overlap</xsl:message>
		</xsl:if>

    <!-- Специальный случай, когда у нас вертикальное положение задано относительно строки
         или параграфа, а горизонтальное либо задано align, либо смещение относительно колонки -->    
    <xsl:variable name="SpecialAnchor">
      <xsl:if test="self::wp:anchor">

        <xsl:if test="(wp:positionH/@relativeFrom = 'column' or wp:positionH/@relativeFrom = 'character') and ( wp:positionV/@relativeFrom = 'line' ) and wp:positionV/wp:align and wp:positionH/wp:align">
          <xsl:text>true</xsl:text>
        </xsl:if>
        
      </xsl:if>
    </xsl:variable>

    <xsl:choose>

      <xsl:when test="$SpecialAnchor = 'true'">               
        
        <img>
          
          <xsl:variable name="HorizontalPos">
            <xsl:choose>
              <xsl:when test="wp:positionH/wp:align = 'left'">
                <xsl:text>float:left;</xsl:text>
              </xsl:when>
              <xsl:when test="wp:positionH/wp:align = 'right'">
                <xsl:text>float:right;</xsl:text>
              </xsl:when>
            </xsl:choose>
          </xsl:variable>

          <xsl:variable name="Margins">
            <xsl:call-template name="InsertImageMargins"/>
          </xsl:variable>
          
          <xsl:attribute name="style">
            <xsl:value-of select="concat( $Margins, $HorizontalPos )"/>
          </xsl:attribute>

          <!--drawing name-->
          <xsl:attribute name="alt">
            <xsl:value-of select="wp:docPr/@name"/>
          </xsl:attribute>

          <!--size-->
          <xsl:call-template name="SetSize"/>

          <!--align-->
          <xsl:call-template name="InsertImageWrap">
            <xsl:with-param name="wpAnchor" select="../wp:anchor"/>
          </xsl:call-template>

          <!-- image href from relationships-->
          <xsl:if test="key('Part', concat('word/_rels/',$document,'.rels'))">
            <xsl:call-template name="InsertImageHref">
              <xsl:with-param name="document" select="$document"/>
            </xsl:call-template>
          </xsl:if>

        </img>
      </xsl:when>
      <xsl:otherwise>
        <!-- Если картинка wp:inline, тогда не надо ее в отдельный div выделять -->
        <xsl:if test="self::wp:anchor">
          <xsl:text disable-output-escaping="yes">&lt;div&gt;</xsl:text>
          <!--position-->
          <xsl:call-template name="SetPosition"/>
        </xsl:if>

        <img>
          <!-- anchor type-->
          <!--TODO: Useful code. Should be uncommented and reworked later.-->
          <!--<xsl:call-template name="InsertImageAnchorType"/>-->

          <!--style name-->
          <!--<xsl:attribute name="class">
                <xsl:value-of select="generate-id(ancestor::w:drawing)"/>
            </xsl:attribute>-->

          <xsl:variable name="pictureStyle">
            <xsl:call-template name="InsertPictureProperties">
              <xsl:with-param name="drawing" select="." />
            </xsl:call-template>
          </xsl:variable>

          <!--<xsl:if test="$pictureStyle != ''">
                <xsl:attribute name="style">
                    <xsl:value-of select="$pictureStyle"/>
                </xsl:attribute>
            </xsl:if>-->

          <!--<xsl:call-template name="DrawingMLToZindex" >
                <xsl:with-param name="wpAnchor" select="." />
            </xsl:call-template>-->

          <xsl:variable name="Margins">
            <xsl:call-template name="InsertImageMargins"/>
          </xsl:variable>
          <!-- Маргины -->
          <xsl:attribute name="style">
            <xsl:value-of select="$Margins"/>
          </xsl:attribute>

          <!--drawing name-->
          <xsl:attribute name="alt">
            <xsl:value-of select="wp:docPr/@name"/>
          </xsl:attribute>

          <!--size-->
          <xsl:call-template name="SetSize"/>

          <!--align-->
          <xsl:call-template name="InsertImageWrap">
            <xsl:with-param name="wpAnchor" select="../wp:anchor"/>
          </xsl:call-template>

          <!-- image href from relationships-->
          <xsl:if test="key('Part', concat('word/_rels/',$document,'.rels'))">
            <xsl:call-template name="InsertImageHref">
              <xsl:with-param name="document" select="$document"/>
            </xsl:call-template>
          </xsl:if>

        </img>
        <xsl:if test="self::wp:anchor">
          <xsl:text disable-output-escaping="yes">&lt;/div&gt;</xsl:text>
          <!--position-->
          <xsl:call-template name="SetPosition"/>
        </xsl:if>

      </xsl:otherwise>
    </xsl:choose>
	</xsl:template>

	<!-- 
  *************************************************************************
  CALLED TEMPLATES
  *************************************************************************
  -->

	<!--
  Summary:  Inserts the z-index for a draw:frame.
  Author:   makz (DIaLOGIKa)
  Params:   wpAnchor: the wp:anchor element of the DrawingML picture.
  -->
	<xsl:template name="DrawingMLToZindex" >
		<xsl:param name="wpAnchor" />

		<xsl:variable name="index" select="number($wpAnchor/@relativeHeight)" />

		<!--
    makz:
    Z-Index normalization:
    We put the values in new reserved ranges:
    hdr/ftr background: 0 - 500.000.000
    hdr/ftr foreground: 500.000.001 - 1.000.000.000
    maindoc background: 1.000.000.001 - 1.500.000.000
    maindoc foreground: 1.500.000.001 - 2.147.483.647
    -->
		<xsl:variable name="normalizedIndex">
			<xsl:choose>
				<xsl:when test ="ancestor::*[w:hdr] or ancestor::*[w:ftr]">
					<xsl:choose>
						<xsl:when test="$wpAnchor/@behindDoc='1'">
							<!-- DML in hdr ftr background -->
							<xsl:value-of select="0 + $index" />
						</xsl:when>
						<xsl:otherwise>
							<!-- DML in hdr ftr foreground -->
							<xsl:value-of select="500000001 + $index" />
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
				<xsl:otherwise>
					<xsl:choose>
						<xsl:when test="$wpAnchor/@behindDoc='1'">
							<!-- DML in main doc background -->
							<xsl:value-of select="1500000000 - $index" />
						</xsl:when>
						<xsl:otherwise>
							<!-- DML in main doc foreground -->
							<xsl:value-of select="1500000001 + $index" />
						</xsl:otherwise>
					</xsl:choose>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:text>z-index:</xsl:text>
		<xsl:choose>
			<xsl:when test="$normalizedIndex != 'NaN'">
				<xsl:value-of select="$normalizedIndex"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>auto</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:text>; </xsl:text>
	</xsl:template>


	<!--
  Summary: Writes the anchor-type attribute
  Author: Clever Age
  -->
	<xsl:template name="InsertImageAnchorType">
		<xsl:attribute name="text:anchor-type">
			<xsl:variable name="verticalRelativeFrom" select="wp:positionV/@relativeFrom"/>
			<xsl:variable name="horizontalRelativeFrom" select="wp:positionH/@relativeFrom"/>
			<xsl:variable name="layoutInCell" select="@layoutInCell"/>

			<xsl:choose>
				<!-- images in header must be anchored to paragraph or as-char -->
				<xsl:when test="ancestor::*[w:hdr]">
					<xsl:choose>
						<xsl:when test="name(.) = 'wp:inline'">
							<xsl:text>as-char</xsl:text>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>paragraph</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
				<!-- images in pages -->
				<xsl:otherwise>
					<xsl:choose>
						<xsl:when test="name(.) = 'wp:inline' ">
							<xsl:text>as-char</xsl:text>
						</xsl:when>
						<xsl:when test="$verticalRelativeFrom = 'line' or $horizontalRelativeFrom = 'line'">
							<xsl:text>char</xsl:text>
						</xsl:when>
						<xsl:when test="$verticalRelativeFrom = 'character' or $horizontalRelativeFrom = 'character'">
							<xsl:text>char</xsl:text>
						</xsl:when>
						<xsl:when test="$verticalRelativeFrom = 'page'">
							<xsl:text>page</xsl:text>
						</xsl:when>
						<xsl:when test="$verticalRelativeFrom = 'paragraph'">
							<xsl:text>char</xsl:text>
						</xsl:when>
						<xsl:when test="$layoutInCell = 1">
							<xsl:text>paragraph</xsl:text>
						</xsl:when>
						<xsl:when test="$layoutInCell = 0">
							<xsl:text>page</xsl:text>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>page</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:attribute>
	</xsl:template>

	<xsl:template name="SetSize">

    <xsl:variable name="RawHeight">
      <xsl:variable name="Temp">
        <xsl:call-template name="PictureHeight"/>
      </xsl:variable>
      <xsl:variable name="Temp1">
        <xsl:value-of select="number(substring-before($Temp, 'px'))"/>
      </xsl:variable>
      <xsl:choose>
        <xsl:when test="contains($Temp1, 'Nan') or $Temp1 = ''">
          <xsl:text>0</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$Temp1"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="RawWidth">
      <xsl:variable name="Temp">
        <xsl:call-template name="PictureWidth"/>
      </xsl:variable>
      <xsl:variable name="Temp1">
        <xsl:value-of select="number(substring-before($Temp, 'px'))"/>
      </xsl:variable>
      <xsl:choose>
        <xsl:when test="contains($Temp1, 'Nan') or $Temp1 = ''">
          <xsl:text>0</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$Temp1"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="MarginTop">
      <xsl:call-template name="CalculateImageMargin">
        <xsl:with-param name="AttributeDist" select="0"/>
        <xsl:with-param name="AttributeFillRect" select="a:graphic/a:graphicData/pic:pic/pic:blipFill/a:stretch/a:fillRect/@t"/>
        <xsl:with-param name="Dim" select="$RawHeight"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="MarginBottom">
      <xsl:call-template name="CalculateImageMargin">
        <xsl:with-param name="AttributeDist" select="0"/>
        <xsl:with-param name="AttributeFillRect" select="a:graphic/a:graphicData/pic:pic/pic:blipFill/a:stretch/a:fillRect/@b"/>
        <xsl:with-param name="Dim" select="$RawHeight"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="MarginLeft">
      <xsl:call-template name="CalculateImageMargin">
        <xsl:with-param name="AttributeDist" select="0"/>
        <xsl:with-param name="AttributeFillRect" select="a:graphic/a:graphicData/pic:pic/pic:blipFill/a:stretch/a:fillRect/@l"/>
        <xsl:with-param name="Dim" select="$RawWidth"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="MarginRight">
      <xsl:call-template name="CalculateImageMargin">
        <xsl:with-param name="AttributeDist" select="0"/>
        <xsl:with-param name="AttributeFillRect" select="a:graphic/a:graphicData/pic:pic/pic:blipFill/a:stretch/a:fillRect/@r"/>
        <xsl:with-param name="Dim" select="$RawWidth"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>
			<xsl:when test="a:graphic/a:graphicData/pic:pic/pic:spPr/a:ln and not(a:graphic/a:graphicData/pic:pic/pic:spPr/a:ln/a:noFill)">

        <xsl:variable name="Border">
          <xsl:variable name="Temp">
            <xsl:call-template name="ConvertEmu">
              <xsl:with-param name="length">
                <xsl:value-of select="a:graphic/a:graphicData/pic:pic/pic:spPr/a:ln/@w"/>
              </xsl:with-param>
              <xsl:with-param name="unit">px</xsl:with-param>
            </xsl:call-template>
          </xsl:variable>
          <xsl:variable name="Temp1">
            <xsl:value-of select="number(substring-before($Temp, 'px'))"/>
          </xsl:variable>
          <xsl:choose>
            <xsl:when test="contains($Temp1, 'Nan') or $Temp1 = ''">
              <xsl:text>0</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$Temp1"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>

        <xsl:variable name="Width">
          <xsl:value-of select="$RawWidth - $MarginLeft - $MarginRight + $Border + $Border"/>
        </xsl:variable>

        <xsl:variable name="Height">
          <xsl:value-of select="$RawHeight - $MarginTop - $MarginBottom + $Border + $Border"/>
        </xsl:variable>

        <xsl:attribute name="height">
          <xsl:value-of select="format-number($Height, 0)"/>
        </xsl:attribute>
        <xsl:attribute name="width">
          <xsl:value-of select="format-number($Width,0)"/>
        </xsl:attribute>
			</xsl:when>
			<xsl:otherwise>
        <xsl:variable name="Width">
          <xsl:value-of select="$RawWidth - $MarginLeft - $MarginRight"/>
        </xsl:variable>

        <xsl:variable name="Height">
          <xsl:value-of select="$RawHeight - $MarginTop - $MarginBottom"/>
        </xsl:variable>
        
				<xsl:attribute name="height">
					<xsl:value-of select="format-number($Height,0)"/>
				</xsl:attribute>
				<xsl:attribute name="width">
					<xsl:value-of select="format-number($Width,0)"/>
				</xsl:attribute>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

  <xsl:template name="PictureHeight">
    <xsl:choose>
      <xsl:when test="a:graphic/a:graphicData/pic:pic/pic:spPr/a:ln and not(a:graphic/a:graphicData/pic:pic/pic:spPr/a:ln/a:noFill)">
        <xsl:call-template name="ConvertEmu">
          <xsl:with-param name="length" select="wp:extent/@cy"/>
          <xsl:with-param name="unit">px</xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="ConvertEmu">
          <xsl:with-param name="length" select="wp:extent/@cy"/>
          <xsl:with-param name="unit">px</xsl:with-param>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="PictureWidth">
    <xsl:choose>
      <xsl:when test="a:graphic/a:graphicData/pic:pic/pic:spPr/a:ln and not(a:graphic/a:graphicData/pic:pic/pic:spPr/a:ln/a:noFill)">
        <xsl:call-template name="ConvertEmu">
          <xsl:with-param name="length" select="wp:extent/@cx"/>
          <xsl:with-param name="unit">px</xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="ConvertEmu">
          <xsl:with-param name="length" select="wp:extent/@cx"/>
          <xsl:with-param name="unit">px</xsl:with-param>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>


  <xsl:template name="SetPosition">
		<xsl:variable name="x">
			<xsl:call-template name="ConvertEmu">
				<xsl:with-param name="length" select="wp:positionH/wp:posOffset"/>
				<xsl:with-param name="unit">cm</xsl:with-param>
			</xsl:call-template>
		</xsl:variable>
		<xsl:variable name="y">
			<xsl:choose>
				<xsl:when test="wp:positionV/@relativeFrom = 'line'">
					<xsl:call-template name="ConvertEmu">
						<xsl:with-param name="length">
							<!--
              makz, 19.5.2008:
              The previous code here was:
              <xsl:value-of select="-wp:positionV/wp:posOffset"/>
              I don't know why the value should get inverted. I removed for a bugfix.
              -->
							<xsl:value-of select="wp:positionV/wp:posOffset"/>
						</xsl:with-param>
						<xsl:with-param name="unit">cm</xsl:with-param>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="wp:positionV/@relativeFrom = 'bottomMargin'">
					<xsl:variable name="pgH">
						<xsl:call-template name="ConvertTwips">
							<xsl:with-param name="length">
								<xsl:value-of select="/w:document/w:body/w:sectPr/w:pgSz/@w:h"/>
							</xsl:with-param>
							<xsl:with-param name="unit">cm</xsl:with-param>
						</xsl:call-template>
					</xsl:variable>
					<xsl:variable name="botMar">
						<xsl:call-template name="ConvertTwips">
							<xsl:with-param name="length">
								<xsl:value-of select="/w:document/w:body/w:sectPr/w:pgMar/@w:bottom"/>
							</xsl:with-param>
							<xsl:with-param name="unit">cm</xsl:with-param>
						</xsl:call-template>
					</xsl:variable>
					<xsl:variable name="Pos">
						<xsl:call-template name="ConvertEmu">
							<xsl:with-param name="length">
								<xsl:value-of select="wp:positionV/wp:posOffset"/>
							</xsl:with-param>
							<xsl:with-param name="unit">cm</xsl:with-param>
						</xsl:call-template>
					</xsl:variable>
					<xsl:value-of
						select="substring-before($pgH,'cm')-substring-before($botMar,'cm') + substring-before($Pos,'cm')"/>
					<xsl:text>cm</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:variable name="vertOnPage">
						<xsl:call-template name="ConvertEmu">
							<xsl:with-param name="length" select="wp:positionV/wp:posOffset"/>
							<xsl:with-param name="unit">cm</xsl:with-param>
						</xsl:call-template>
					</xsl:variable>
					<xsl:variable name="pagesHeights">
						<xsl:call-template name="ConvertTwips">
							<xsl:with-param name="length">
								<xsl:call-template name="calculatePageHeights">
									<xsl:with-param name="node" select="."/>
								</xsl:call-template>
							</xsl:with-param>
							<xsl:with-param name="unit" select="'cm'"/>
						</xsl:call-template>
					</xsl:variable>
					<xsl:value-of select="concat(substring-before($vertOnPage, 'cm') + substring-before($pagesHeights, 'cm'), 'cm')"/>
					<xsl:value-of select="concat(';', $pagesHeights)"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:attribute name="style">
			<xsl:text>position:absolute;</xsl:text>
			<xsl:value-of select="concat('left:', $x, ';')"/>
			<xsl:value-of select="concat('top:', $y, ';')"/>
		</xsl:attribute>
	</xsl:template>

	<xsl:template name="GetFileName">
		<xsl:param name="path"/>
		<xsl:choose>
			<xsl:when test="contains($path, '/')">
				<xsl:call-template name="GetFileName">
					<xsl:with-param name="path" select="substring-after($path,'/')"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="contains($path, '\')">
				<xsl:call-template name="GetFileName">
					<xsl:with-param name="path" select="substring-after($path,'\')"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$path"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!-- inserts image href from relationships -->
	<xsl:template name="InsertImageHref">
		<xsl:param name="document"/>
		<xsl:param name="rId"/>
		<xsl:param name="targetName"/>
		<xsl:param name="srcFolder" select="'$MediaFolder$'"/>

		<xsl:variable name="id">
			<xsl:choose>
				<xsl:when test="$rId != ''">
					<xsl:value-of select="$rId"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="a:graphic/a:graphicData/pic:pic/pic:blipFill/a:blip/@r:embed"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:for-each
          select="key('Part', concat('word/_rels/',$document,'.rels'))//node()[name() = 'Relationship']">
			<xsl:if test="./@Id=$id">
				<xsl:variable name="targetmode">
					<xsl:value-of select="./@TargetMode"/>
				</xsl:variable>
				<xsl:variable name="pzipsource">
					<xsl:value-of select="./@Target"/>
				</xsl:variable>
				<xsl:variable name="pziptarget">
					<xsl:choose>
						<xsl:when test="$targetName != ''">
							<xsl:value-of select="$targetName"/>
						</xsl:when>
						<xsl:when test="$targetmode='External'">
							<xsl:value-of select="$pzipsource"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="GetFileName">
								<xsl:with-param name="path" select="$pzipsource"/>
							</xsl:call-template>
							<!--<xsl:value-of select="substring-after($pzipsource,'/')"/>-->
						</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>
				<xsl:attribute name="src">
					<!--Postprocessor directive-->
					<!--<xsl:text>$MediaFolder$</xsl:text>-->
					<xsl:choose>
						<xsl:when test="$targetmode='External'">
							<xsl:value-of select="concat('EXTERNAL_' ,$pziptarget)"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="$pziptarget"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>
			</xsl:if>
		</xsl:for-each>
	</xsl:template>

	<!--
  -->
	<xsl:template name="InsertPictureProperties">
		<xsl:param name="drawing" />

		<xsl:call-template name="InsertImagePosH"/>
		<xsl:call-template name="InsertImagePosV"/>
		<xsl:call-template name="InsertImageFlip"/>
		<xsl:call-template name="InsertImageCrop"/>
		<!--
        <xsl:call-template name="InsertImageWrap">
            <xsl:with-param name="wpAnchor" select="$drawing/wp:anchor" />
        </xsl:call-template>
        -->
		<xsl:call-template name="InsertImageMargins"/>
		<!--No corresponding property in HTML/CSS-->
		<!--<xsl:call-template name="InsertImageFlowWithText"/>-->
		<xsl:call-template name="InsertImageBorder"/>
		<xsl:call-template name="DrawingMLToZindex">
			<xsl:with-param name="wpAnchor" select="." />
		</xsl:call-template>
	</xsl:template>

	<xsl:template name="InsertImageFlowWithText">
		<!-- @layoutInCell: Specifies how this DrawingML object shall behave when its anchor is located in a table
			cell; and its specified position would cause it to intersect with a table cell displayed in the document.-->
		<xsl:variable name="layoutInCell" select="(wp:inline/@layoutInCell | wp:anchor/@layoutInCell) and ./ancestor::w:tc"/>
		<xsl:attribute name="draw:flow-with-text">
			<xsl:choose>
				<!--If the pic is inside the table-->
				<xsl:when test="(ancestor::w:hdr or ancestor::w:ftr) and (./ancestor::w:tc)">
					<xsl:text>true</xsl:text>
				</xsl:when>
				<xsl:when test="$layoutInCell = 1">
					<!--only set to true of actually inside a table. @draw:flow-with-text has an effect 
				in OOo even when the shape is not inside a table -->
					<xsl:text>true</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>false</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:attribute>
	</xsl:template>

	<xsl:template name="InsertImageBorder">
		<xsl:if
          test="*[self::wp:inline or self::wp:anchor]/a:graphic/a:graphicData/pic:pic/pic:spPr/a:ln[not(a:noFill)]">
			<xsl:variable name="width">
				<xsl:call-template name="ConvertEmu3">
					<xsl:with-param name="length">
						<xsl:value-of
                          select="*[self::wp:inline or self::wp:anchor]/a:graphic/a:graphicData/pic:pic/pic:spPr/a:ln/@w"
            />
					</xsl:with-param>
					<xsl:with-param name="unit">cm</xsl:with-param>
				</xsl:call-template>
			</xsl:variable>
			<xsl:variable name="type">
				<xsl:choose>
					<xsl:when
                      test="*[self::wp:inline or self::wp:anchor]/a:graphic/a:graphicData/pic:pic/pic:spPr/a:ln/a:prstDash/@val = 'solid'">
						<xsl:text>solid</xsl:text>
					</xsl:when>
					<xsl:when
                     test="*[self::wp:inline or self::wp:anchor]/a:graphic/a:graphicData/pic:pic/pic:spPr/a:ln/@cmpd = 'double'">
						<xsl:text>double</xsl:text>
					</xsl:when>
					<xsl:when
                     test="*[self::wp:inline or self::wp:anchor]/a:graphic/a:graphicData/pic:pic/pic:spPr/a:ln/@cmpd = 'thickThin'">
						<xsl:text>double</xsl:text>
					</xsl:when>
					<xsl:when
                    test="*[self::wp:inline or self::wp:anchor]/a:graphic/a:graphicData/pic:pic/pic:spPr/a:ln/@cmpd = 'thinThick'">
						<xsl:text>double</xsl:text>
					</xsl:when>
					<xsl:otherwise>
						<xsl:text>solid</xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<xsl:variable name="color">
				<xsl:choose>
					<xsl:when
                      test="*[self::wp:inline or self::wp:anchor]/a:graphic/a:graphicData/pic:pic/pic:spPr/a:ln/a:solidFill/a:srgbClr">
						<xsl:value-of
                          select="*[self::wp:inline or self::wp:anchor]/a:graphic/a:graphicData/pic:pic/pic:spPr/a:ln/a:solidFill/a:srgbClr/@val"
            />
					</xsl:when>
					<xsl:otherwise>
						<xsl:text>000000</xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<xsl:attribute name="fo:border">
				<xsl:value-of select="concat($width,' ',$type,' #',$color)"/>
			</xsl:attribute>
		</xsl:if>
	</xsl:template>

	<!--
  Summary:  Inserts the style:wrap attribute for a draw:frame
  Author:   Clever Age
  Modified: makz (DIaLOGIKa)
  Params:   wpAnchor: The wp:anchor element of the DML picture
  -->
	<xsl:template name="InsertImageWrap">
		<xsl:param name="wpAnchor" />

		<xsl:if test="$wpAnchor/wp:wrapSquare or 
            $wpAnchor/wp:wrapTight or 
            $wpAnchor/wp:wrapTopAndBottom or 
            $wpAnchor/wp:wrapThrough or 
            $wpAnchor/wp:wrapNone">

			<xsl:attribute name="align">
				<xsl:choose>
					<!--
                    <xsl:when test="$wpAnchor/wp:wrapSquare">
                        <xsl:call-template name="InsertSquareWrap">
                            <xsl:with-param name="wrap" select="$wpAnchor/wp:wrapSquare/@wrapText"/>
                        </xsl:call-template>
                    </xsl:when>
                    <xsl:when test="$wpAnchor/wp:wrapTight">
                        <xsl:call-template name="InsertSquareWrap">
                            <xsl:with-param name="wrap" select="$wpAnchor/wp:wrapTight/@wrapText"/>
                        </xsl:call-template>
                    </xsl:when>
                    <xsl:when test="$wpAnchor/wp:wrapTopAndBottom">
                        <xsl:text>none</xsl:text>
                    </xsl:when>
                    <xsl:when test="$wpAnchor/wp:wrapThrough">
                        <xsl:call-template name="InsertSquareWrap">
                            <xsl:with-param name="wrap" select="$wpAnchor/wp:wrapThrough/@wrapText"/>
                        </xsl:call-template>
                    </xsl:when>
                    -->
					<xsl:when test="$wpAnchor/wp:wrapNone">
						<xsl:text>bottom</xsl:text>
					</xsl:when>
					<xsl:otherwise>
						<xsl:text>left</xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>
		</xsl:if>

		<!--
    makz:
    If no wrap is specified, Word wraps the picture in front of the text.
    If the picture is in a header or footer it will be rendered behind the text of the main document.
    If the behindDoc attribute is set, it will also be rendered behind the text.
    -->
		<!--
        <xsl:if test="$wpAnchor/wp:wrapNone">
            <xsl:attribute name="style:run-through">
                <xsl:choose>
                    <xsl:when test="$wpAnchor/@behindDoc='1'">
                        <xsl:text>background</xsl:text>
                    </xsl:when>
                    <xsl:when test="ancestor::*[w:hdr] or ancestor::*[w:ftr]" >
                        <xsl:text>background</xsl:text>
                    </xsl:when>
                    <xsl:otherwise>
                        <xsl:text>foreground</xsl:text>
                    </xsl:otherwise>
                </xsl:choose>
            </xsl:attribute>
        </xsl:if>
        -->
	</xsl:template>

	<xsl:template name="InsertImageMargins">

    <xsl:variable name="Height">
      <xsl:variable name="Temp">
        <xsl:call-template name="PictureHeight"/>
      </xsl:variable>
      <xsl:variable name="Temp1">
        <xsl:value-of select="number(substring-before($Temp, 'px'))"/>
      </xsl:variable>
      <xsl:choose>
        <xsl:when test="contains($Temp1, 'Nan') or $Temp1 = ''">
          <xsl:text>0</xsl:text>
         </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$Temp1"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="Width">
      <xsl:variable name="Temp">
        <xsl:call-template name="PictureWidth"/>
      </xsl:variable>
      <xsl:variable name="Temp1">
        <xsl:value-of select="number(substring-before($Temp, 'px'))"/>
      </xsl:variable>
      <xsl:choose>
        <xsl:when test="contains($Temp1, 'Nan') or $Temp1 = ''">
          <xsl:text>0</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$Temp1"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="MarginTop">
      <xsl:call-template name="CalculateImageMargin">
        <xsl:with-param name="AttributeDist" select="wp:anchor/@distT"/>
        <xsl:with-param name="AttributeFillRect" select="a:graphic/a:graphicData/pic:pic/pic:blipFill/a:stretch/a:fillRect/@t"/>
        <xsl:with-param name="Dim" select="$Height"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="MarginBottom">
      <xsl:call-template name="CalculateImageMargin">
        <xsl:with-param name="AttributeDist" select="wp:anchor/@distB"/>
        <xsl:with-param name="AttributeFillRect" select="a:graphic/a:graphicData/pic:pic/pic:blipFill/a:stretch/a:fillRect/@b"/>
        <xsl:with-param name="Dim" select="$Height"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="MarginLeft">
      <xsl:call-template name="CalculateImageMargin">
        <xsl:with-param name="AttributeDist" select="wp:anchor/@distL"/>
        <xsl:with-param name="AttributeFillRect" select="a:graphic/a:graphicData/pic:pic/pic:blipFill/a:stretch/a:fillRect/@l"/>
        <xsl:with-param name="Dim" select="$Width"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="MarginRight">
      <xsl:call-template name="CalculateImageMargin">
        <xsl:with-param name="AttributeDist" select="wp:anchor/@distR"/>
        <xsl:with-param name="AttributeFillRect" select="a:graphic/a:graphicData/pic:pic/pic:blipFill/a:stretch/a:fillRect/@r"/>
        <xsl:with-param name="Dim" select="$Width"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:if test="$MarginTop != 0">
      <xsl:text>margin-top:</xsl:text>
      <xsl:value-of select="format-number($MarginTop,0)"/>
      <xsl:text>px; </xsl:text>
    </xsl:if>

    <xsl:if test="$MarginBottom != 0">
      <xsl:text>margin-bottom:</xsl:text>
      <xsl:value-of select="format-number($MarginBottom,0)"/>
      <xsl:text>px; </xsl:text>
    </xsl:if>

    <xsl:if test="$MarginLeft != 0">
      <xsl:text>margin-left:</xsl:text>
      <xsl:value-of select="format-number($MarginLeft,0)"/>
      <xsl:text>px; </xsl:text>
    </xsl:if>

    <xsl:if test="$MarginRight != 0">
      <xsl:text>margin-right:</xsl:text>
      <xsl:value-of select="format-number($MarginRight,0)"/>
      <xsl:text>px; </xsl:text>
    </xsl:if>   

  </xsl:template>

  <xsl:template name="CalculateImageMargin">
    <xsl:param name="AttributeDist"/>
    <xsl:param name="AttributeFillRect"/>
    <xsl:param name="Dim"/>
      
      <xsl:variable name="Temp">
        <xsl:choose>
          <xsl:when test="$AttributeFillRect">
            <xsl:variable name="Value">
              <xsl:value-of select="$AttributeFillRect"/>
            </xsl:variable>
            <xsl:variable name="Value2">
              <xsl:choose>
                <xsl:when test="contains($Value, '%')">
                  <xsl:value-of select="number(substring-before( $Value, '%' )) div 100"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="number($Value) div 100000"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>
            <xsl:choose>
              <xsl:when test="contains($Value2, 'NaN') or $Value2 = ''">
                <xsl:text>0</xsl:text>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$Value2 * $Dim"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>0</xsl:text>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <xsl:variable name="Temp2">
        <xsl:variable name="Value">
          <xsl:call-template name="ConvertEmu">
            <xsl:with-param name="length" select="$AttributeDist"/>
            <xsl:with-param name="unit">px</xsl:with-param>
          </xsl:call-template>
        </xsl:variable>
        <xsl:variable name="Value2">
          <xsl:variable name="Value3">
            <xsl:value-of select="number(substring-before( $Value, 'px' ))"/>
          </xsl:variable>
          <xsl:choose>
            <xsl:when test="contains($Value3, 'NaN') or $Value3 = ''">
              <xsl:text>0</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$Value3"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        <xsl:choose>
          <xsl:when test="wp:anchor">
            <xsl:value-of select="$Value2"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>0</xsl:text>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <xsl:value-of select="$Temp + $Temp2"/>
    
  </xsl:template>

	<xsl:template name="InsertSquareWrap">
		<xsl:param name="wrap"/>

		<xsl:choose>
			<xsl:when test="$wrap = 'bothSides' ">
				<xsl:text>parallel</xsl:text>
			</xsl:when>
			<xsl:when test="$wrap = 'largest' ">
				<xsl:text>dynamic</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$wrap"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="GetCropSize">
		<xsl:param name="cropValue"/>
		<xsl:param name="cropOppositeValue"/>
		<xsl:param name="resultSize"/>

		<xsl:choose>
			<xsl:when test="not($cropValue)">
				<xsl:text>0</xsl:text>
			</xsl:when>

			<xsl:otherwise>
				<xsl:variable name="cropPercent" select="$cropValue div (100000)"/>
				<xsl:variable name="cropOppositePercent">
					<xsl:choose>
						<xsl:when test="$cropOppositeValue">
							<xsl:value-of select="$cropOppositeValue div (100000)"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>0</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>

				<xsl:value-of
                  select="format-number(($resultSize div(1 - $cropPercent - $cropOppositePercent)) *  $cropPercent , '0.000' )"
        />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="InsertImagePosH">

		<xsl:if test="descendant::wp:positionH">


			<!--<xsl:call-template name="InsertGraphicPosH">
                <xsl:with-param name="align" select="descendant::wp:positionH/wp:align"/>
            </xsl:call-template>

            <xsl:call-template name="InsertGraphicPosRelativeH">
                <xsl:with-param name="relativeFrom" select="descendant::wp:positionH/@relativeFrom"/>
            </xsl:call-template>-->
		</xsl:if>
	</xsl:template>

	<xsl:template name="InsertGraphicPosH">
		<xsl:param name="align"/>
		<xsl:param name="relativeFrom" />

		<!--
        <xsl:attribute name="style:horizontal-pos">
            <xsl:choose>
                <!-@-added by chhavi to fix problam related to alignment and absolute position  in case of inner-margin and outer-margin area  -@->
                <!-@- Sona: #2149141 changes because mirror margins was lost-@->
                <xsl:when test ="$relativeFrom='inner-margin-area' and (not($align) or $align ='' or $align = 'absolute')   or  $relativeFrom = 'outer-margin-area' and (not($align) or $align ='' or $align = 'absolute') ">
                    <xsl:text>from-inside</xsl:text>
                </xsl:when>
                <xsl:when test ="$relativeFrom='inner-margin-area' or $relativeFrom='outer-margin-area'">
                    <xsl:if test ="$align='left'">
                        <xsl:text>inside</xsl:text>
                    </xsl:if>
                    <xsl:if test ="$align='right'">
                        <xsl:text>outside</xsl:text>
                    </xsl:if>
                    <xsl:if test ="$align='center'">
                        <xsl:text>middle</xsl:text>
                    </xsl:if>
                </xsl:when>
                <!-@-end here-@->
                <!-@- Sona: #2149141 changes because mirror margins was lost-@->
                <xsl:when test="$align = 'absolute' ">
                    <xsl:text>from-left</xsl:text>
                </xsl:when>
                <xsl:when test="$align and $align != '' ">
                    <xsl:value-of select="$align"/>
                </xsl:when>

                <xsl:otherwise>
                    <xsl:text>from-left</xsl:text>
                </xsl:otherwise>
            </xsl:choose>
        </xsl:attribute>
        -->
	</xsl:template>

	<xsl:template name="InsertGraphicPosRelativeH">
		<xsl:param name="relativeFrom"/>
		<xsl:param name="hPos" />

		<!--
        <xsl:attribute name="style:horizontal-rel">
            <xsl:choose>
                <xsl:when test="$relativeFrom = 'margin' or $relativeFrom = 'column'">
                    <xsl:text>page-content</xsl:text>
                </xsl:when>
                <xsl:when test="$relativeFrom ='page'">
                    <xsl:text>page</xsl:text>
                </xsl:when>
                <xsl:when test="$relativeFrom ='text'">
                    <xsl:text>paragraph</xsl:text>
                </xsl:when>
                <xsl:when test=" $relativeFrom = 'outside-margin-area'">
                    <!-@-<xsl:text>page-start-margin</xsl:text>-@->
                    <xsl:text>page</xsl:text>
                </xsl:when>
                <xsl:when test=" $relativeFrom = 'inside-margin-area'">
                    <!-@-<xsl:text>page-end-margin</xsl:text>-@->
                    <xsl:text>page</xsl:text>
                </xsl:when>
                <xsl:when test="$relativeFrom = 'character' or $relativeFrom = 'char'">
                    <xsl:text>char</xsl:text>
                </xsl:when>
                <!-@- COMMENT : following values not defined in OOX spec, but used by Word 2007 -@->
                <!-@-added by chhavi to improve absolute and alignment position-@->
                <xsl:when test="$relativeFrom = 'left-margin-area' or $relativeFrom = 'outer-margin-area'">
                    <!-@- Sona: #2149141 changes because mirror margins was lost-@->
                    <xsl:text>page-start-margin</xsl:text>
                </xsl:when>
                <xsl:when test="$relativeFrom = 'right-margin-area' or $relativeFrom = 'inner-margin-area'">
                    <xsl:text>page-end-margin</xsl:text>
                </xsl:when>

                <!-@-end here-@->
                <xsl:when test="$relativeFrom=''">
                    <!-@- 
          if no relation is set, Word uses default values, 
          depeding on the position
          -@->
                    <xsl:choose>
                        <!-@- 
            If no position is set, it is absolute positioning.
            In this case the default relation is the paragraph-content 
            -@->
                        <xsl:when test="$hPos=''">
                            <xsl:text>paragraph-content</xsl:text>
                        </xsl:when>
                        <!-@- 
            If a position is set, it is relative positioning.
            In this case the default relation is the page-content 
            -@->
                        <xsl:otherwise>
                            <xsl:text>page-content</xsl:text>
                        </xsl:otherwise>
                    </xsl:choose>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:text>paragraph</xsl:text>
                </xsl:otherwise>
            </xsl:choose>
        </xsl:attribute>
        -->
	</xsl:template>

	<xsl:template name="InsertGraphicPosV">
		<xsl:param name="align"/>
		<xsl:param name="relativeFrom"/>

		<xsl:text>vertical-align:</xsl:text>
		<xsl:choose>
			<xsl:when test="$align and $align != ''">
				<xsl:choose>
					<!--special rules-->
					<xsl:when  test="$relativeFrom = 'top-margin-area' or $relativeFrom = 'bottom-margin-area' or $relativeFrom = 'inner-margin-area' or $relativeFrom = 'outer-margin-area'">
						<xsl:text>middle</xsl:text>
					</xsl:when>
					<xsl:when test=" $relativeFrom = 'line'  and $align= 'bottom' ">
						<xsl:text>top</xsl:text>
					</xsl:when>
					<xsl:when test=" $relativeFrom = 'line'  and $align= 'top' ">
						<xsl:text>bottom</xsl:text>
					</xsl:when>
					<!--default rules-->
					<!--changed by chhavi top to from-top-->
					<xsl:when test="$align = 'top' ">
						<xsl:text>top</xsl:text>
					</xsl:when>
					<xsl:when test="$align = 'center' ">
						<xsl:text>middle</xsl:text>
					</xsl:when>
					<xsl:when test="$align = 'bottom' ">
						<xsl:text>bottom</xsl:text>
					</xsl:when>
					<xsl:when test="$align = 'inside' ">
						<xsl:text>top</xsl:text>
					</xsl:when>
					<xsl:when test="$align = 'outside' ">
						<xsl:text>bottom</xsl:text>
					</xsl:when>
					<xsl:when test="$align = 'from-top' ">
						<xsl:text>top</xsl:text>
					</xsl:when>
					<xsl:otherwise>
						<!-- Sona: change if the "mso-position-vertical" value is absolute-->
						<xsl:text>middle</xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:otherwise>
				<xsl:choose>
					<!-- if there is vertical position offset -->
					<xsl:when test="contains(@style,'margin-top') or wp:anchor/wp:positionV/wp:posOffset/text() != '' or (w:pPr/w:framePr/@w:y and not(w:pPr/w:framePr/@w:yAlign))">
						<xsl:text>middle</xsl:text>
					</xsl:when>
					<xsl:otherwise>
						<xsl:text>middle</xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:text>; </xsl:text>
	</xsl:template>

	<xsl:template name="InsertGraphicPosRelativeV">
		<xsl:param name="relativeFrom"/>
		<xsl:param name="align"/>

		<!--
        <xsl:attribute name="style:vertical-rel">
            <xsl:choose>
                <xsl:when test="$relativeFrom = 'margin' ">
                    <xsl:text>page-content</xsl:text>
                </xsl:when>
                <xsl:when test="$relativeFrom ='page'">
                    <xsl:text>page</xsl:text>
                </xsl:when>
                <xsl:when test="$relativeFrom = 'top-margin-area'">
                    <xsl:text>page</xsl:text>
                </xsl:when>
                <xsl:when test="$relativeFrom = 'bottom-margin-area'">
                    <xsl:text>page</xsl:text>
                </xsl:when>
                <xsl:when test="$relativeFrom = 'inner-margin-area'">
                    <xsl:text>page</xsl:text>
                </xsl:when>
                <xsl:when test="$relativeFrom = 'outer-margin-area'">
                    <xsl:text>page</xsl:text>
                </xsl:when>
                <xsl:when test="$relativeFrom = 'line'">
                    <!-@-<xsl:text>line</xsl:text>-@->
                    <!-@-changed by chhavi-@->
                    <xsl:text>paragraph-content</xsl:text>
                    <!-@-end here-@->
                </xsl:when>
                <xsl:when test="$relativeFrom = 'paragraph'">
                    <xsl:text>paragraph-content</xsl:text>
                </xsl:when>
                <xsl:when test="$relativeFrom = 'text' or $relativeFrom = '' ">
                    <xsl:text>paragraph-content</xsl:text>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:text>page</xsl:text>
                </xsl:otherwise>
            </xsl:choose>
        </xsl:attribute>

        <xsl:if
          test="$relativeFrom = 'line' and ($align = 'center'  or $align =  'outside' or $align = 'bottom' ) ">
            <xsl:attribute name="draw:flow-with-text">
                <xsl:text>false</xsl:text>
            </xsl:attribute>
        </xsl:if>
        -->
	</xsl:template>

	<xsl:template name="InsertImagePosV">
		<xsl:if test="descendant::wp:positionV">
			<xsl:variable name="align" select="descendant::wp:positionV/wp:align"/>
			<xsl:variable name="relativeFrom" select="descendant::wp:positionV/@relativeFrom"/>

			<!--<xsl:call-template name="InsertGraphicPosV">
                <xsl:with-param name="align" select="$align"/>
                <xsl:with-param name="relativeFrom" select="$relativeFrom"/>
            </xsl:call-template>

            <xsl:call-template name="InsertGraphicPosRelativeV">
                <xsl:with-param name="relativeFrom" select="$relativeFrom"/>
                <xsl:with-param name="align" select="$align"/>
            </xsl:call-template>-->
		</xsl:if>
	</xsl:template>

	<xsl:template name="InsertImageFlip">
		<!--  picture flip (vertical, horizontal)-->
		<xsl:if test="descendant::pic:spPr/a:xfrm/attribute::node()">
			<xsl:for-each select="descendant::pic:pic">
				<xsl:attribute name="style:mirror">
					<xsl:choose>
						<xsl:when test="pic:spPr/a:xfrm/@flipV = '1' and pic:spPr/a:xfrm/@flipH = '1'">
							<xsl:text>horizontal vertical</xsl:text>
						</xsl:when>
						<xsl:when test="pic:spPr/a:xfrm/@flipV = '1' ">
							<xsl:text>vertical</xsl:text>
						</xsl:when>
						<xsl:when test="pic:spPr/a:xfrm/@flipH = '1' ">
							<xsl:text>horizontal</xsl:text>
						</xsl:when>
					</xsl:choose>
				</xsl:attribute>
			</xsl:for-each>
		</xsl:if>
	</xsl:template>

	<xsl:template name="InsertImageCrop">
		<!-- crop -->
		<xsl:if test="descendant::pic:blipFill/a:srcRect/attribute::node()">
			<xsl:for-each select="descendant::pic:pic">
				<xsl:variable name="width">
					<xsl:variable name="widthText">
						<xsl:call-template name="ConvertEmu">
							<xsl:with-param name="length" select="ancestor::w:drawing/descendant::wp:extent/@cx"/>
							<xsl:with-param name="unit">cm</xsl:with-param>
						</xsl:call-template>
					</xsl:variable>
					<xsl:value-of select="substring-before($widthText,'cm')"/>
				</xsl:variable>

				<xsl:variable name="height">
					<xsl:variable name="heightText">
						<xsl:call-template name="ConvertEmu">
							<xsl:with-param name="length" select="ancestor::w:drawing/descendant::wp:extent/@cy"/>
							<xsl:with-param name="unit">cm</xsl:with-param>
						</xsl:call-template>
					</xsl:variable>
					<xsl:value-of select="substring-before($heightText,'cm')"/>
				</xsl:variable>

				<xsl:variable name="leftCrop">
					<xsl:call-template name="GetCropSize">
						<xsl:with-param name="cropValue" select="pic:blipFill/a:srcRect/@l"/>
						<xsl:with-param name="cropOppositeValue" select="pic:blipFill/a:srcRect/@r"/>
						<xsl:with-param name="resultSize" select="$width"/>
					</xsl:call-template>
				</xsl:variable>

				<xsl:variable name="rightCrop">
					<xsl:call-template name="GetCropSize">
						<xsl:with-param name="cropValue" select="pic:blipFill/a:srcRect/@r"/>
						<xsl:with-param name="cropOppositeValue" select="pic:blipFill/a:srcRect/@l"/>
						<xsl:with-param name="resultSize" select="$width"/>
					</xsl:call-template>
				</xsl:variable>

				<xsl:variable name="topCrop">
					<xsl:call-template name="GetCropSize">
						<xsl:with-param name="cropValue" select="pic:blipFill/a:srcRect/@t"/>
						<xsl:with-param name="cropOppositeValue" select="pic:blipFill/a:srcRect/@b"/>
						<xsl:with-param name="resultSize" select="$height"/>
					</xsl:call-template>
				</xsl:variable>

				<xsl:variable name="bottomCrop">
					<xsl:call-template name="GetCropSize">
						<xsl:with-param name="cropValue" select="pic:blipFill/a:srcRect/@b"/>
						<xsl:with-param name="cropOppositeValue" select="pic:blipFill/a:srcRect/@t"/>
						<xsl:with-param name="resultSize" select="$height"/>
					</xsl:call-template>
				</xsl:variable>

				<xsl:text>clip:</xsl:text>

				<xsl:variable name="oldValue">
					<xsl:value-of select="concat('rect(',$topCrop,'cm ',$rightCrop,'cm ',$bottomCrop,'cm ',$leftCrop,'cm',')')"/>
				</xsl:variable>

				<xsl:variable name="filecode" select="pic:blipFill/a:blip/@r:embed"></xsl:variable>
				<xsl:variable name="filename" select="key('Part', 'word/_rels/document.xml.rels')/rels:Relationships/rels:Relationship[@Id = $filecode]/@Target"></xsl:variable>
				<xsl:value-of select="$oldValue"/>
				<!--<xsl:value-of select="concat('COMPUTEODFCROPPING[', pic:blipFill/a:srcRect/@r, ',' ,pic:blipFill/a:srcRect/@l, ',' , pic:blipFill/a:srcRect/@t, ',' ,pic:blipFill/a:srcRect/@b, ',word/' , $filename,  ',' , $oldValue, ']')"/>-->
				<xsl:text>; </xsl:text>
			</xsl:for-each>
		</xsl:if>
	</xsl:template>

</xsl:stylesheet>
