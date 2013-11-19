<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:number="urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0"
  xmlns:wp="http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:xlink="http://www.w3.org/1999/xlink"
  xmlns:dc="http://purl.org/dc/elements/1.1/"
  xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
  xmlns:pcut="urn:avsmedia:xmlns:post-processings:pcut"
  xmlns:v="urn:schemas-microsoft-com:vml"
  xmlns:o="urn:schemas-microsoft-com:office:office"
  xmlns:cp="http://schemas.openxmlformats.org/package/2006/metadata/core-properties"
  xmlns:oox="urn:oox"
  xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main"
  xmlns:pic="http://schemas.openxmlformats.org/drawingml/2006/picture"
  exclude-result-prefixes="w r xlink number wp oox">

	<xsl:import href="2oox-common.xsl"/>
	<xsl:import href="2oox-styles-common.xsl"/>
	<xsl:import href="2oox-measures.xsl"/>

	<xsl:output method="xml" encoding="UTF-8"/>

	<xsl:strip-space elements="*"/>
	<xsl:preserve-space elements="span"/>

	<!-- Context: p or table element -->
	<xsl:template name="OpenParagraph">
		<xsl:param name="style"/>
		<xsl:text disable-output-escaping="yes">&lt;w:p w:rsidR="004D5F21" w:rsidRPr="00820E0B" w:rsidRDefault="00301D39" pcut:cut="true"&gt;</xsl:text>
		<!--<w:pPr>
            <w:rPr>
               <w:lang w:val="en-US"/>
            </w:rPr>
        </w:pPr>-->
		<w:pPr>
			<xsl:call-template name="InsertParagraphStyle">
				<xsl:with-param name="styleString">
					<xsl:call-template name="getParagraphBackGround">
						<xsl:with-param name="style">
							<xsl:value-of select="$style"/>
						</xsl:with-param>
					</xsl:call-template>
				</xsl:with-param>
				<xsl:with-param name="node" select="."/>
			</xsl:call-template>
      <xsl:variable name="nodeName">
        <xsl:value-of select="name(..)"/>
      </xsl:variable>
       <xsl:if test="$nodeName = 'div'">
        <xsl:variable name="new_style1">
          <xsl:value-of select="../@*"/>
        </xsl:variable>
        <xsl:call-template name="InsertParagraphStyle">
          <xsl:with-param name="styleString" select="$new_style1"/>
          <xsl:with-param name="node" select="."/>
        </xsl:call-template>
       </xsl:if>
			<w:rPr>
				<w:lang w:val="en-US"/>
			</w:rPr>
		</w:pPr>
	</xsl:template>

	<!-- Context: p or table element -->
	<xsl:template name="CloseParagraph">
		<xsl:text disable-output-escaping="yes">&lt;/w:p&gt;</xsl:text>
	</xsl:template>

	<xsl:template name="InsertParagraph">
		<xsl:param name="style"/>
		<xsl:param name="node" select="."/>
		<xsl:param name="ListItemParam" select="''"/>

		<xsl:variable name="styleString">
			<xsl:call-template name="getParagraphBackGround">
				<xsl:with-param name="style">
					<!--<xsl:value-of select="concat( $style, ./p/@style, ';', $ListItemParam)"/>-->
          <xsl:value-of select="concat( $style, ';', $ListItemParam)"/>
        </xsl:with-param>
			</xsl:call-template>
		</xsl:variable>

    <xsl:variable name="new_style1">
      <xsl:choose>
        <xsl:when test="contains($style, 'margin-left:40px') or 'li' != name()">
          <xsl:variable name="StyleWO_Padding">
            <xsl:call-template name="RemovePr">
              <xsl:with-param name="style" select="$styleString"/>
              <xsl:with-param name="namePr" select="'padding'"/>
            </xsl:call-template>
          </xsl:variable>

          <xsl:call-template name="RemovePr">
            <xsl:with-param name="style" select="$StyleWO_Padding" />
            <xsl:with-param name="namePr" select="'margin'"/>
          </xsl:call-template>

        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$styleString"/>
        </xsl:otherwise>
      </xsl:choose>

    </xsl:variable>

		<xsl:variable name="new_style">
			<xsl:choose>
				<xsl:when test="count(preceding-sibling::*) = 0">
					<xsl:value-of select="$style"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name="deleteListPr">
						<xsl:with-param name="style" select="$style"/>
					</xsl:call-template>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<w:p w:rsidR="004D5F21" w:rsidRPr="00820E0B" w:rsidRDefault="00301D39" pcut:cut="true">
			<w:pPr>
				<xsl:call-template name="InsertParagraphStyle">
					<xsl:with-param name="styleString" select="$new_style1"/>
					<xsl:with-param name="node" select="."/>
				</xsl:call-template>

				<w:rPr>
					<xsl:call-template name="InsertRunStyle">
						<xsl:with-param name="styleString" select="$new_style1"/>
					</xsl:call-template>
					<w:lang w:val="en-US"/>
				</w:rPr>

			</w:pPr>

			<xsl:for-each select="$node">
				<xsl:apply-templates mode="paragraph">
					<xsl:with-param name="style" select="$new_style1"/>
				</xsl:apply-templates>
			</xsl:for-each>
		</w:p>
	</xsl:template>

	<xsl:template match="/html">
		<w:document>
			<xsl:call-template name="content"/>
		</w:document>
	</xsl:template>


	<!--main document-->
	<xsl:template name="content">
		<xsl:variable name="genericStyle">
			<xsl:call-template name="GetElementStyle">
				<xsl:with-param name="tag" select="'body'"/>
			</xsl:call-template>
			<xsl:text>;</xsl:text>
		</xsl:variable>

    <xsl:variable name="BodyStyle">
      <xsl:choose>
        <xsl:when test="./body/@style">
          <xsl:value-of select="concat($genericStyle, ./body/@style, ';')"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$genericStyle"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

		<xsl:variable name="backgroundColor">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="concat( $genericStyle, ./body/@style, ';' )"/>
				<xsl:with-param name="propertyName" select="'background-color'"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="backgroundImage">
			<xsl:value-of select="./body/@background-image"/>
		</xsl:variable>

    <xsl:variable name="MarginLeft">
      <xsl:variable name="Temp">
        <xsl:call-template name="GetPropertyFromStyle">
          <xsl:with-param name="styleString" select="$BodyStyle"/>
          <xsl:with-param name="propertyName" select="'margin-left'"/>
        </xsl:call-template>
      </xsl:variable>

      <xsl:choose>
        <xsl:when test="contains($Temp,'%')">
          <xsl:value-of select="format-number(number(substring-before($Temp,'%')) * 11906 div 100,0)"/>
        </xsl:when>
        <xsl:when test="not($Temp = '')">
          <xsl:call-template name="twips-measure">
            <xsl:with-param name="length" select="$Temp"/>
          </xsl:call-template>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="MarginRight">
      <xsl:variable name="Temp">
        <xsl:call-template name="GetPropertyFromStyle">
          <xsl:with-param name="styleString" select="$BodyStyle"/>
          <xsl:with-param name="propertyName" select="'margin-right'"/>
        </xsl:call-template>
      </xsl:variable>

      <xsl:choose>
        <xsl:when test="contains($Temp,'%')">
          <xsl:value-of select="format-number(number(substring-before($Temp,'%')) * 11906 div 100,0)"/>
        </xsl:when>

        <xsl:when test="not($Temp = '')">
          <xsl:call-template name="twips-measure">
            <xsl:with-param name="length" select="$Temp"/>
          </xsl:call-template>
        </xsl:when>
      </xsl:choose>
      
    </xsl:variable>

    <xsl:variable name="MarginTop">
      <xsl:variable name="Temp">
        <xsl:call-template name="GetPropertyFromStyle">
          <xsl:with-param name="styleString" select="$BodyStyle"/>
          <xsl:with-param name="propertyName" select="'margin-top'"/>
        </xsl:call-template>
      </xsl:variable>

      <xsl:choose>
        <xsl:when test="contains($Temp,'%')">
          <xsl:value-of select="format-number(number(substring-before($Temp,'%')) * 16838 div 100,0)"/>
        </xsl:when>

        <xsl:when test="not($Temp = '')">
          <xsl:call-template name="twips-measure">
            <xsl:with-param name="length" select="$Temp"/>
          </xsl:call-template>
        </xsl:when>
      </xsl:choose>

    </xsl:variable>

    <xsl:variable name="MarginBottom">
      <xsl:variable name="Temp">
        <xsl:call-template name="GetPropertyFromStyle">
          <xsl:with-param name="styleString" select="$BodyStyle"/>
          <xsl:with-param name="propertyName" select="'margin-bottom'"/>
        </xsl:call-template>
      </xsl:variable>

      <xsl:choose>
        <xsl:when test="contains($Temp,'%')">
          <xsl:value-of select="format-number(number(substring-before($Temp,'%')) * 16838 div 100,0)"/>
        </xsl:when>
        <xsl:when test="not($Temp = '')">
          <xsl:call-template name="twips-measure">
            <xsl:with-param name="length" select="$Temp"/>
          </xsl:call-template>
        </xsl:when>
      </xsl:choose>

    </xsl:variable>

    <xsl:if test="$backgroundColor != '' or $backgroundImage != ''">
			<xsl:variable name="converted">
				<xsl:call-template name="ConvertColor">
					<xsl:with-param name="color" select="$backgroundColor"/>
				</xsl:call-template>
			</xsl:variable>

			<w:background>
				<xsl:attribute name="w:color">
					<xsl:choose>
						<xsl:when test="$converted != ''">
							<xsl:value-of select="$converted"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>FFFFFF</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>
				<xsl:if test="$backgroundImage != ''">
					<v:background>
						<v:fill>
							<xsl:attribute name="r:id">
								<xsl:text>rId7</xsl:text>
							</xsl:attribute>
							<xsl:attribute name="recolor">
								<xsl:text>t</xsl:text>
							</xsl:attribute>
							<xsl:attribute name="type">
								<xsl:text>frame</xsl:text>
							</xsl:attribute>
						</v:fill>
					</v:background>
				</xsl:if>
			</w:background>
		</xsl:if>
		<w:body>

      <xsl:choose>
				<xsl:when test="./body">
					<xsl:call-template name="InsertDocumentBody"/>
				</xsl:when>
				<xsl:when test="./frameset">
					<xsl:for-each select="./frameset">
						<xsl:call-template name="InsertFrameset"/>
					</xsl:for-each>
				</xsl:when>
			</xsl:choose>

      <xsl:call-template name="SectPr">
        <xsl:with-param name="MarginLeft" select="$MarginLeft"/>
        <xsl:with-param name="MarginRight" select="$MarginRight"/>
        <xsl:with-param name="MarginTop" select="$MarginTop"/>
        <xsl:with-param name="MarginBottom" select="$MarginBottom"/>
      </xsl:call-template>
      
		</w:body>
	</xsl:template>


	<xsl:template name="SectPr">
    <xsl:param name="MarginLeft"/>
    <xsl:param name="MarginRight"/>
    <xsl:param name="MarginTop"/>
    <xsl:param name="MarginBottom"/>

    <w:sectPr w:rsidR="00051DE4" w:rsidSect="00051DE4">
			<w:pgSz w:w="11906" w:h="16838" />
      <xsl:element name="w:pgMar">
        <!-- w:top -->
        <xsl:choose>
          <xsl:when test="$MarginTop = ''">
            <xsl:attribute name="w:top">1134</xsl:attribute>
          </xsl:when>
          <xsl:otherwise>
            <xsl:attribute name="w:top"><xsl:value-of select="$MarginTop"/></xsl:attribute>
          </xsl:otherwise>
        </xsl:choose>

        <!-- w:right -->
        <xsl:choose>
          <xsl:when test="$MarginRight = ''">
            <xsl:attribute name="w:right">850</xsl:attribute>
          </xsl:when>
          <xsl:otherwise>
            <xsl:attribute name="w:right">
              <xsl:value-of select="$MarginRight"/>
            </xsl:attribute>
          </xsl:otherwise>
        </xsl:choose>

        <!-- w:bottom -->
        <xsl:choose>
          <xsl:when test="$MarginBottom = ''">
            <xsl:attribute name="w:bottom">1134</xsl:attribute>
          </xsl:when>
          <xsl:otherwise>
            <xsl:attribute name="w:bottom">
              <xsl:value-of select="$MarginBottom"/>
            </xsl:attribute>
          </xsl:otherwise>
        </xsl:choose>

        <!-- w:left -->
        <xsl:choose>
          <xsl:when test="$MarginLeft = ''">
            <xsl:attribute name="w:left">1701</xsl:attribute>
          </xsl:when>
          <xsl:otherwise>
            <xsl:attribute name="w:left">
              <xsl:value-of select="$MarginLeft"/>
            </xsl:attribute>
          </xsl:otherwise>
        </xsl:choose>

        <xsl:attribute name="w:header">708</xsl:attribute>
        <xsl:attribute name="w:footer">708</xsl:attribute>
        <xsl:attribute name="w:gutter">0</xsl:attribute>
        
      </xsl:element>
			<!--<w:pgMar w:top="1134" w:right="850" w:bottom="1134" w:left="1701" w:header="708" w:footer="708" w:gutter="0" />-->
			<w:cols w:space="708" />
			<w:docGrid w:linePitch="360" />
		</w:sectPr>
	</xsl:template>

	<!--
	  Summary:  Converts the child elements of the body and handels the section conversion
	-->

	<xsl:template name="InsertFrameset">
		<xsl:variable name="cols">
			<xsl:choose>
				<xsl:when test="./@cols">
					<xsl:call-template name="getCount">
						<xsl:with-param name="str" select="./@cols"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>1</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="rows">
			<xsl:choose>
				<xsl:when test="./@rows">
					<xsl:call-template name="getCount">
						<xsl:with-param name="str" select="./@rows"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>1</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<w:tbl>
			<w:tblPr/>
			<w:tblGrid/>
			<xsl:call-template name="InsertFrames">
				<xsl:with-param name="cols" select="$cols"/>
				<xsl:with-param name="rows" select="$rows"/>
			</xsl:call-template>
		</w:tbl>

	</xsl:template>

	<xsl:template name="InsertFrames">
		<xsl:param name="cols"/>
		<xsl:param name="rows"/>

		<xsl:for-each select="./*">
			<xsl:if test="name(.) = 'frame' or name(.) = 'frameset'">
				<xsl:if test="((position() - 1) -  floor((position() - 1) div $cols) * $cols)  = 0">
					<xsl:text disable-output-escaping="yes">&lt;w:tr&gt;</xsl:text>
				</xsl:if>
				<xsl:call-template name="InsertFrame">
					<xsl:with-param name="num" select="position()"/>
					<xsl:with-param name="cols" select="$cols"/>
					<xsl:with-param name="rows" select="$rows"/>
				</xsl:call-template>
				<xsl:if test="(position() - floor(position() div $cols) * $cols) = 0">
					<xsl:text disable-output-escaping="yes">&lt;/w:tr&gt;</xsl:text>
				</xsl:if>
			</xsl:if>
		</xsl:for-each>
	</xsl:template>

	<xsl:template name="InsertFrame">
		<xsl:param name="num"/>
		<xsl:param name="cols"/>
		<xsl:param name="rows"/>
		<w:tc>
			<w:tcPr>
				<w:tcW w:w="0" w:type="auto" />
			</w:tcPr>
			<xsl:choose>
				<xsl:when test="count(child::*) != 0">
					<xsl:choose>
						<xsl:when test="./body">
							<xsl:call-template name="InsertDocumentBody"/>
						</xsl:when>
						<xsl:when test="name(.) = 'frameset'">
							<xsl:for-each select=".">
								<xsl:call-template name="InsertFrameset"/>
							</xsl:for-each>
						</xsl:when>
					</xsl:choose>
				</xsl:when>
				<xsl:otherwise>
					<w:p/>
				</xsl:otherwise>
			</xsl:choose>

		</w:tc>
	</xsl:template>


	<xsl:template name="getCount">
		<xsl:param name="str"/>
		<xsl:param name="count" select="1"/>
		<xsl:choose>
			<xsl:when test="contains($str, ',')">
				<xsl:call-template name="getCount">
					<xsl:with-param name="str" select="substring-after($str, ',')"/>
					<xsl:with-param name="count" select="$count + 1"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$count"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<xsl:template name="InsertDocumentBody">

		<xsl:variable name="genericStyle">
			<xsl:call-template name="GetElementStyle">
				<xsl:with-param name="tag" select="'body'"/>
			</xsl:call-template>
			<xsl:text>;</xsl:text>
		</xsl:variable>

		<xsl:variable name="attr_par_style">
			<xsl:call-template name="delBodyMargin">
				<xsl:with-param name="style">
					<xsl:call-template name="getParagraphBackGround">
						<xsl:with-param name="style">
							<xsl:value-of select="concat( $genericStyle, ./body/@style, ';' )"/>
						</xsl:with-param>
					</xsl:call-template>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:variable>

		<!--<xsl:variable name="st">-->
		<xsl:call-template name="InsertParagraph">
			<xsl:with-param name="style" select="$attr_par_style"/>
			<xsl:with-param name="node" select="./body"/>
		</xsl:call-template>
	</xsl:template>

	<!-- Since HTML/XHTML format has no such a strict hierarchy as
         OOX format has, we have to use complex system of modes and
         virtual elements which transforms free HTML structure into
         an ordered one of OOX. -->

	<xsl:template match="iframe" mode="paragraph">
		<xsl:param name="style"/>

		<xsl:call-template name="CloseParagraph"/>
		<xsl:if test="./body">
			<xsl:call-template name="InsertDocumentBody"/>
		</xsl:if>
		<xsl:call-template name="OpenParagraph">
			<xsl:with-param name="style" select="$style"/>
		</xsl:call-template>

	</xsl:template>


	<xsl:template match="button" mode="paragraph">
		<xsl:param name="style"/>

	</xsl:template>


	<xsl:template name="replaceSpec">
		<xsl:param name="str"/>

		<xsl:call-template name="replaceAmp">
			<xsl:with-param name="str">
				<xsl:call-template name="replaceLt">
					<xsl:with-param name="str">
						<xsl:call-template name="replaceGt">
							<xsl:with-param name="str">
								<xsl:call-template name="replaceQuot">
									<xsl:with-param name="str" select="$str"/>
								</xsl:call-template>
							</xsl:with-param>
						</xsl:call-template>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:with-param>
		</xsl:call-template>

	</xsl:template>


	<xsl:template name="replaceAmp">
		<xsl:param name="str"/>

		<xsl:choose>
			<xsl:when test="contains($str, '&amp;amp;')">
				<xsl:value-of select="substring-before($str, '&amp;amp;')"/>
				<xsl:text>&lt;</xsl:text>

				<xsl:call-template name="replaceAmp">
					<xsl:with-param name="str" select="substring-after($str, '&amp;amp;')"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$str"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<xsl:template name="replaceLt">
		<xsl:param name="str"/>

		<xsl:choose>
			<xsl:when test="contains($str, '&amp;lt;')">
				<xsl:value-of select="substring-before($str, '&amp;lt;')"/>
				<xsl:text>&lt;</xsl:text>

				<xsl:call-template name="replaceLt">
					<xsl:with-param name="str" select="substring-after($str, '&amp;lt;')"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$str"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<xsl:template name="replaceGt">
		<xsl:param name="str"/>

		<xsl:choose>
			<xsl:when test="contains($str, '&amp;gt;')">
				<xsl:value-of select="substring-before($str, '&amp;gt;')"/>
				<xsl:text>&gt;</xsl:text>

				<xsl:call-template name="replaceGt">
					<xsl:with-param name="str" select="substring-after($str, '&amp;gt;')"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$str"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<xsl:template name="replaceQuot">
		<xsl:param name="str"/>

		<xsl:choose>
			<xsl:when test="contains($str, '&amp;quot;')">
				<xsl:value-of select="substring-before($str, '&amp;quot;')"/>
				<xsl:text>&gt;</xsl:text>

				<xsl:call-template name="replaceQuot">
					<xsl:with-param name="str" select="substring-after($str, '&amp;quot;')"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$str"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<xsl:template match="textarea" mode="paragraph">
		<xsl:param name="style"/>

		<xsl:variable name="attr_style">
			<xsl:call-template name="getParagraphBackGround">
				<xsl:with-param name="style">
					<xsl:value-of select="concat($style, '; ', @style)"/>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="val">
			<xsl:call-template name="replaceSpec">
				<xsl:with-param name="str" select="@value"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:call-template name="CloseParagraph"/>
		<xsl:call-template name="OpenParagraph">
			<xsl:with-param name="style" select="$attr_style"/>
		</xsl:call-template>
		<w:r>
			<w:rPr>
				<xsl:call-template name="InsertRunStyle">
					<xsl:with-param name="styleString" select="$attr_style"/>
				</xsl:call-template>
				<w:lang w:val="en-US"/>
			</w:rPr>
			<w:t>
				<xsl:value-of select="$val"/>
				<!--<xsl:value-of select="@value"/>-->
			</w:t>
		</w:r>
		<xsl:call-template name="CloseParagraph"/>
		<xsl:call-template name="OpenParagraph">
			<xsl:with-param name="style" select="$style"/>
		</xsl:call-template>

	</xsl:template>

	<xsl:template name="RemovePr">
		<xsl:param name="style"/>
		<xsl:param name="namePr"/>
        
		<xsl:if test="$namePr != ''">
			<xsl:choose>
				<xsl:when test="contains($style, $namePr)">
          
          <xsl:variable name="StartString" select="substring-before($style, $namePr)"/>
          <xsl:variable name="EndString" select="substring-after( substring-after($style, $StartString), ';' )"/>
          
					<xsl:call-template name="RemovePr">
						<xsl:with-param name="style" select="concat($StartString, $EndString)" />
            <xsl:with-param name="namePr" select="$namePr"/>
					</xsl:call-template>
          
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$style"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:if>
	</xsl:template>

	<!-- Paragraph -->
	<xsl:template match="p | h1 | H1| h2 | h3 | h4 | h5 | h6 | div  " mode="paragraph">
		<xsl:param name="style"/>

    <xsl:variable name="checkParentLink">
      <xsl:value-of select="count(./ancestor::a)"/>
    </xsl:variable>
    <xsl:choose>
      
      <xsl:when test="$checkParentLink != 0">
        <xsl:apply-templates mode="paragraph">
          <xsl:with-param name="style">
            <xsl:call-template name="GetStyleString">
              <xsl:with-param name="style" select="$style"/>
            </xsl:call-template>
          </xsl:with-param>
        </xsl:apply-templates>
      </xsl:when>
      
      <xsl:otherwise>
        <xsl:variable name="new_style1">
			    <xsl:choose>
				    <xsl:when test="count(preceding-sibling::*) = 0">
					    <xsl:value-of select="$style"/>
				    </xsl:when>
				    <xsl:otherwise>
					    <xsl:call-template name="deleteListPr">
						    <xsl:with-param name="style" select="$style"/>
					    </xsl:call-template>
				    </xsl:otherwise>
			    </xsl:choose>
		    </xsl:variable>

		    <xsl:variable name="new_style">
			    <xsl:call-template name="RemovePr">
				    <xsl:with-param name="style">
					    <xsl:call-template name="RemovePr">
						    <xsl:with-param name="style" select="$new_style1"/>
						    <xsl:with-param name="namePr" select="'padding'"/>
					    </xsl:call-template>
				    </xsl:with-param>
				    <xsl:with-param name="namePr" select="'margin'"/>
			    </xsl:call-template>
		    </xsl:variable>

		    <xsl:call-template name="CloseParagraph"/>

		    <xsl:variable name="genericStyle">
			    <xsl:call-template name="GetElementStyle">
				    <xsl:with-param name="tag" select="name()"/>
				    <xsl:with-param name="class" select="@class"/>
            <xsl:with-param name="ID" select="@id" />
			    </xsl:call-template>
			    <xsl:text>;</xsl:text>
		    </xsl:variable>

		    <xsl:variable name="styleString">
			    <xsl:choose>
				    <xsl:when test="self::h1">
					    <xsl:value-of select="concat( $new_style, ' ;font-size: 150%; ', 'font-weight:bold; ', $genericStyle, @style, ';' )"/>
				    </xsl:when>
				    <xsl:when test="self::h2">
					    <xsl:value-of select="concat( $new_style, ' ;font-size: 135%; ', 'font-weight:bold; ', $genericStyle, @style, ';' )"/>
				    </xsl:when>
				    <xsl:when test="self::h3">
					    <xsl:value-of select="concat( $new_style, ' ;font-size: 120%; ', 'font-weight:bold; ', $genericStyle, @style, ';' )"/>
				    </xsl:when>
				    <xsl:when test="self::h4">
					    <xsl:value-of select="concat( $new_style, ' ;font-size: 105%; ', 'font-weight:bold; ', $genericStyle, @style, ';' )"/>
				    </xsl:when>
				    <xsl:when test="self::h5">
					    <xsl:value-of select="concat( $new_style, ' ;font-size: 90%; ', 'font-weight:bold; ', $genericStyle, @style, ';' )"/>
				    </xsl:when>
				    <xsl:when test="self::h6">
					    <xsl:value-of select="concat( $new_style, ' ;font-size: 75%; ', 'font-weight:bold; ', $genericStyle, @style, ';' )"/>
				    </xsl:when>
				    <xsl:when test="self::p">
					    <xsl:value-of select="concat( $new_style, $genericStyle, @style, ';')"/>
				    </xsl:when>
				    <xsl:otherwise>
					    <xsl:value-of select="concat($new_style, $genericStyle, @style, ';')"/>
				    </xsl:otherwise>
			    </xsl:choose>
		    </xsl:variable>

		    <xsl:variable name="align">
			    <xsl:if test="@align">
				    <xsl:value-of select="concat( 'text-align: ', @align, ';')"/>
			    </xsl:if>
		    </xsl:variable>

		    <xsl:variable name="attr_style">
			    <xsl:call-template name="getParagraphBackGround">
				    <xsl:with-param name="style">
					    <xsl:value-of select="concat( $styleString, '; ', @style, ';', $align)"/>
				    </xsl:with-param>
			    </xsl:call-template>
		    </xsl:variable>

        <xsl:variable name="Heading">
          <xsl:choose>
            <xsl:when test="descendant::span[@heading = 'h1']">
              <xsl:text>Heading1</xsl:text>
            </xsl:when>
            <xsl:when test="descendant::span[@heading = 'h2']">
              <xsl:text>Heading2</xsl:text>
            </xsl:when>
            <xsl:when test="descendant::span[@heading = 'h3']">
              <xsl:text>Heading3</xsl:text>
            </xsl:when>
            <xsl:when test="descendant::span[@heading = 'h4']">
              <xsl:text>Heading4</xsl:text>
            </xsl:when>
            <xsl:when test="descendant::span[@heading = 'h5']">
              <xsl:text>Heading5</xsl:text>
            </xsl:when>
            <xsl:when test="descendant::span[@heading = 'h6']">
              <xsl:text>Heading6</xsl:text>
            </xsl:when>
          </xsl:choose>
        </xsl:variable>

		    <w:p w:rsidR="004D5F21" w:rsidRPr="00820E0B" w:rsidRDefault="00301D39" pcut:cut="true">

			    <w:pPr>

            <xsl:choose>
              <xsl:when test="$Heading != ''">
                <w:pStyle w:val="{ $Heading }"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:if test="@class">
                  <w:pStyle w:val="{ concat( 'custom_', @class ) }"/>
                </xsl:if>
              </xsl:otherwise>
            </xsl:choose>

				    <xsl:call-template name="InsertParagraphStyle">
					    <xsl:with-param name="styleString">
						    <xsl:value-of select="$attr_style"/>
					    </xsl:with-param>
					    <xsl:with-param name="node" select="."/>
				    </xsl:call-template>

				    <w:rPr>
					    <xsl:call-template name="InsertRunStyle">
						    <xsl:with-param name="styleString">
							    <xsl:value-of select="$attr_style"/>
						    </xsl:with-param>
					    </xsl:call-template>
					    <w:lang w:val="en-US"/>
				    </w:rPr>
			    </w:pPr>
			    <xsl:apply-templates mode="paragraph">
				    <xsl:with-param name="style">
					    <xsl:value-of select="$attr_style"/>
				    </xsl:with-param>
			    </xsl:apply-templates>
		    </w:p>
		    <xsl:call-template name="OpenParagraph"/>
      </xsl:otherwise>
    </xsl:choose>
	</xsl:template>


	<xsl:template name="getParagraphBackGround">
		<xsl:param name="style"/>
		<xsl:choose>
			<xsl:when test="contains($style, 'background-color')">
				<xsl:call-template name="getParagraphBackGround">
					<xsl:with-param name="style">
						<xsl:value-of select="concat(substring-before($style, 'background-color'), 'paragraphBGcolor', substring-after($style, 'background-color'))"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$style"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<xsl:template name="delBodyMargin">
		<xsl:param name="style"/>
		<xsl:choose>
			<xsl:when test="contains($style, 'margin')">
        
        <xsl:variable name="StartString" select="substring-before($style, 'margin')"/>
        <xsl:variable name="EndString" select="substring-after(substring-after($style, $StartString), ';')"/>
        
				<xsl:value-of select="concat($StartString, $EndString)"/>
        
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$style"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<xsl:template name="getTableBackGround">
		<xsl:param name="style"/>
		<xsl:choose>
			<xsl:when test="contains($style, 'background-color')">
				<xsl:call-template name="getTableBackGround">
					<xsl:with-param name="style">
						<xsl:value-of select="concat(substring-before($style, 'background-color'), 'tableBGcolor', substring-after($style, 'background-color'))"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$style"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<!-- Content element inside paragraph (real o virtual) -->
	<xsl:template match="text() | img | ul | ol"  mode="paragraph">
		<xsl:param name="style"/>

		<xsl:variable name="styleString">
			<xsl:call-template name="trim">
				<xsl:with-param name="s">
					<xsl:value-of select="concat($style, ';')"/>
					<xsl:if test="@style">
						<xsl:value-of select="concat(@style, ';')"/>
					</xsl:if>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:variable>
		<!-- Create virtual run element for it. -->
		<w:r w:rsidRPr="00820E0B">
			<w:rPr>
				<xsl:call-template name="InsertRunStyle">
					<xsl:with-param name="styleString" select="$styleString"/>
				</xsl:call-template>
				<w:lang w:val="en-US"/>
			</w:rPr>
			<xsl:apply-templates select="." mode="run">
				<!-- TODO: in fact, it's not enough to pass only 'style' attribute since
                     element may belong to some CSS class; code should be upgraded. -->
				<xsl:with-param name="style" select="$styleString" />
			</xsl:apply-templates>
		</w:r>
	</xsl:template>

	<!--Переводим html-вские размеры шрифтов в pt-->
	<xsl:template name="htmlSizeToPtSize">
		<xsl:param name="htmlSize"/>
		<!--Значения взяты для Firefox-->
		<xsl:choose>
			<xsl:when test="$htmlSize = 1">
				<xsl:text>7.5pt</xsl:text>
			</xsl:when>
			<xsl:when test="$htmlSize = 2">
				<xsl:text>10pt</xsl:text>
			</xsl:when>
			<xsl:when test="$htmlSize = 3">
				<xsl:text>12pt</xsl:text>
			</xsl:when>
			<xsl:when test="$htmlSize = 4">
				<xsl:text>13.5pt</xsl:text>
			</xsl:when>
			<xsl:when test="$htmlSize = 5">
				<xsl:text>18pt</xsl:text>
			</xsl:when>
			<xsl:when test="$htmlSize = 6">
				<xsl:text>24pt</xsl:text>
			</xsl:when>
			<xsl:when test="$htmlSize = 7">
				<xsl:text>35pt</xsl:text>
			</xsl:when>
		</xsl:choose>
	</xsl:template>


	<!-- Style container -->
	<xsl:template match="span|u|i|em|b|sub|sup|samp|center|strong|font|blockquote|small|big|tt|pre|dfn|cite|code|kbd|s|strike|var" mode="paragraph">
		<xsl:param name="style"/>

		<xsl:apply-templates mode="paragraph">
			<xsl:with-param name="style">
				<xsl:call-template name="GetStyleString">
					<xsl:with-param name="style" select="$style"/>
				</xsl:call-template>
			</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>


	<xsl:template match="option" mode="paragraph">
		<xsl:param name="style"/>
	</xsl:template>


	<!-- Context: any style-container element (span, div, u, i, b, strong, center) -->
	<xsl:template name="GetStyleString">
		<xsl:param name="style"/>

		<xsl:variable name="styleString">
			<xsl:choose>
				<xsl:when test="self::span">
					<xsl:value-of select="$style"/>
				</xsl:when>
				<xsl:when test="self::div">
					<xsl:value-of select="$style"/>
				</xsl:when>
				<xsl:when test="self::center">
					<xsl:value-of select="concat( $style, '; text-align:center')"/>
				</xsl:when>
				<xsl:when test="self::blockquote">
					<xsl:value-of select="concat( $style, '; margin-left:40px; margin-right:40px')"/>
				</xsl:when>
				<xsl:when test="self::pre">
					<xsl:value-of select="concat( $style, '; contextualSpacing:true')"/>
				</xsl:when>
				<xsl:when test="self::big">
					<xsl:variable name="fontSize">
						<xsl:call-template name="getFontSize">
							<xsl:with-param name="style" select="$style"/>
						</xsl:call-template>
					</xsl:variable>
					<xsl:value-of select="concat( $style, '; font-size:')"/>
					<xsl:choose>
						<xsl:when test="$fontSize != '' and $fontSize != 'NaN' and number($fontSize) != 'NaN'">
							<xsl:value-of select="number($fontSize + 6) div 2"/>
						</xsl:when>
					</xsl:choose>
					<xsl:text>pt;</xsl:text>
				</xsl:when>
				<xsl:when test="self::small">
					<xsl:variable name="fontSize">
						<xsl:call-template name="getFontSize">
							<xsl:with-param name="style" select="$style"/>
						</xsl:call-template>
					</xsl:variable>
					<xsl:value-of select="concat( $style, '; font-size:')"/>
					<xsl:choose>
						<xsl:when test="$fontSize != '' and $fontSize != 'NaN' and number($fontSize) != 'NaN'">
							<xsl:choose>
								<xsl:when test="number($fontSize) &gt; 21">
									<xsl:value-of select="number($fontSize - 6) div 2"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:text>7.5</xsl:text>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:when>
					</xsl:choose>
					<xsl:text>pt;</xsl:text>
				</xsl:when>
				<xsl:when test="self::b or self::strong">
					<xsl:value-of select="concat( $style, '; font-weight:bold')"/>
				</xsl:when>
				<xsl:when test="self::code or self::kbd or self::samp or self::tt">
					<xsl:value-of select="$style"/>
					<xsl:text>; font-family:&#39;Courier New;&#39;</xsl:text>
					<!--<xsl:value-of select="concat( $style, '; font-family:&#8242;Courier New;&#8242;' )"/>-->
				</xsl:when>
				<xsl:when test="self::u">
					<xsl:value-of select="concat( $style, '; text-decoration:underline' )"/>
				</xsl:when>
				<xsl:when test="self::s or self::strike">
					<xsl:value-of select="concat( $style, '; text-decoration:line-through' )"/>
				</xsl:when>
				<xsl:when test="self::i or self::dfn or self::cite or self::em or self::var">
					<xsl:value-of select="concat( $style, '; font-style:italic' )"/>
				</xsl:when>
				<xsl:when test="self::sub">
					<xsl:value-of select="concat( $style, '; vertical-align:sub' )"/>
				</xsl:when>
				<xsl:when test="self::sup">
					<xsl:value-of select="concat( $style, '; vertical-align:super' )"/>
				</xsl:when>
				<xsl:when test="self::font">
					<xsl:variable name="fontStyle">
						<xsl:if test="@face != ''">
							<xsl:text>font-family:&#39;</xsl:text>
							<xsl:value-of select="@face"/>
							<xsl:text>&#39;;</xsl:text>
							<!--<xsl:value-of select="concat('font-family:&#8242;', @face, '&#8242;;')"/>-->
						</xsl:if>
						<xsl:if test="@size != ''">
							<xsl:text>font-size:</xsl:text>
							<xsl:call-template name="htmlSizeToPtSize">
								<xsl:with-param name="htmlSize" select="@size"/>
							</xsl:call-template>
							<xsl:text>;</xsl:text>
						</xsl:if>
						<xsl:if test="@color != ''">
							<xsl:value-of select="concat('color:', @color, ';')"/>
						</xsl:if>
					</xsl:variable>
					<xsl:value-of select="concat( $style, ';', $fontStyle, ';', @style )"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="genericStyle">
			<xsl:call-template name="GetElementStyle">
				<xsl:with-param name="tag" select="name(.)"/>
				<xsl:with-param name="class" select="@class"/>
        <xsl:with-param name="ID" select="@id" />
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="trim_style">
			<xsl:call-template name="trim">
				<xsl:with-param name="s" select="concat($styleString, ';', $genericStyle, ';', @style)"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:choose>
			<xsl:when test="count(preceding-sibling::*) = 0">
				<xsl:value-of select="$trim_style"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:call-template name="deleteListPr">
					<xsl:with-param name="style" select="$trim_style"/>
				</xsl:call-template>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

 	<!-- Text -->
	<xsl:template match="text()" mode="run">
		<w:t>
      <xsl:attribute name="xml:space">
        <xsl:text>preserve</xsl:text>
      </xsl:attribute>
			<xsl:variable name="val">
				<xsl:call-template name="replaceSpec">
					<xsl:with-param name="str" select="."/>
				</xsl:call-template>
			</xsl:variable>
			<xsl:value-of select="$val"/>
		</w:t>
	</xsl:template>



	<!--Break-->
	<xsl:template match="br" mode="run">
		<xsl:param name="style"/>
		<w:br/>
	</xsl:template>


	<!-- Table -->
	<xsl:template match="table" mode="paragraph">
		<xsl:param name="style"/>

		<xsl:if test="count(descendant::tr) != 0">
			<xsl:variable name="genericStyle">
				<xsl:call-template name="GetElementStyle">
					<xsl:with-param name="tag" select="'table'"/>
					<xsl:with-param name="class" select="@class"/>
          <xsl:with-param name="ID" select="@id" />
				</xsl:call-template>
				<xsl:text>;</xsl:text>
			</xsl:variable>

			<xsl:variable name="new_style">
				<xsl:call-template name="deletePr">
					<xsl:with-param name="style" select="$style"/>
					<xsl:with-param name="prName" select="'paragraphBGcolor'"/>
				</xsl:call-template>
			</xsl:variable>

			<xsl:variable name="align">
				<xsl:if test="@align != ''">
					<xsl:value-of select="concat( 'text-align: ', @align, ';')"/>
				</xsl:if>
			</xsl:variable>

			<xsl:variable name="width">
				<xsl:if test="@width != ''">
					<xsl:call-template name="someWidth">
						<xsl:with-param name="styleString" select="$new_style"/>
						<xsl:with-param name="attr_width" select="@width"/>
					</xsl:call-template>
				</xsl:if>
			</xsl:variable>

			<xsl:variable name="height">
				<xsl:if test="@height != ''">
					<xsl:value-of select="concat( 'height: ', @height, ';')"/>
				</xsl:if>
			</xsl:variable>

			<xsl:variable name="border">
				<xsl:if test="@border != ''">
					<xsl:value-of select="concat( 'hasBorder: ', @border, ';')"/>
				</xsl:if>
			</xsl:variable>

			<xsl:variable name="cols">
				<xsl:if test="@cols != ''">
					<xsl:value-of select="concat( 'cols: ', @cols, ';')"/>
				</xsl:if>
			</xsl:variable>

			<xsl:variable name="bgcolor">
				<xsl:if test="@bgcolor != ''">
					<xsl:value-of select="concat( 'tableBGcolor: ', @bgcolor, ';')"/>
				</xsl:if>
			</xsl:variable>

			<xsl:variable name="attr_style">
				<xsl:if test="@style">
					<xsl:call-template name="getTableBackGround">
						<xsl:with-param name="style" select="@style"/>
					</xsl:call-template>
					<xsl:text>;</xsl:text>
				</xsl:if>
			</xsl:variable>

			<xsl:variable name="styleString" select="concat( $new_style, '; ', $genericStyle, $attr_style, $align, $width, $height, ';', $border, $cols, $bgcolor)"/>

			<xsl:call-template name="CloseParagraph"/>

			<xsl:if test="count(./caption) != 0">
				<xsl:call-template name="InsertParagraph">
					<xsl:with-param name="style" select="$styleString"/>
					<xsl:with-param name="node" select="./caption"/>
				</xsl:call-template>
			</xsl:if>

			<w:tbl>
				<w:tblPr>
					<xsl:call-template name="InsertTableStyle">
						<xsl:with-param name="styleString" select="$styleString"/>
					</xsl:call-template>
				</w:tblPr>
				<w:tblGrid>
					<!--<xsl:call-template name="setTableGrid">
						<xsl:with-param name="node" select="."/>
						</xsl:call-template>-->
				</w:tblGrid>

				<!--<xsl:apply-templates mode="table">
					<xsl:with-param name="style" select="concat( $style, '; ', $genericStyle, @style, ';', $width, $align)"/>
					</xsl:apply-templates>-->
				<!--NEW-->
				<xsl:variable name="summaryStyles">
					<xsl:call-template name="deletePr">
						<xsl:with-param name="style" select="$styleString"/>
						<xsl:with-param name="prName" select="'align'"/>
					</xsl:call-template>
					<xsl:value-of select="concat('; ', $genericStyle, $attr_style, ';', $width, $align, $bgcolor)"/>
				</xsl:variable>

				<xsl:choose>
					<xsl:when test="count(child::tr) != 0">
						<xsl:call-template name="InsertRowsGroup">
							<xsl:with-param name="style" select="$summaryStyles"/>
							<xsl:with-param name="node" select="."/>
						</xsl:call-template>
					</xsl:when>
					<xsl:otherwise>
						<xsl:if test="count(./thead) != 0">
							<xsl:call-template name="thead">
								<xsl:with-param name="style" select="$summaryStyles"/>
								<xsl:with-param name="node" select="./thead"/>
							</xsl:call-template>
						</xsl:if>
						<xsl:if test="count(./tbody) != 0">
							<xsl:call-template name="tbody">
								<xsl:with-param name="style" select="$summaryStyles"/>
								<xsl:with-param name="node" select="./tbody"/>
							</xsl:call-template>
						</xsl:if>
						<xsl:if test="count(./tfoot) != 0">
							<xsl:call-template name="tfoot">
								<xsl:with-param name="style" select="$summaryStyles"/>
								<xsl:with-param name="node" select="./tfoot"/>
							</xsl:call-template>
						</xsl:if>

					</xsl:otherwise>
				</xsl:choose>

				<!--/NEW-->
			</w:tbl>
			<w:p/>
			<xsl:call-template name="OpenParagraph"/>
		</xsl:if>
	</xsl:template>

	<xsl:template name="deletePr">
		<xsl:param name="style"/>
		<xsl:param name="prName"/>

		<xsl:choose>
			<xsl:when test="$prName != ''">
				<xsl:choose>
					<xsl:when test="contains($style, $prName)">

            <xsl:variable name="StartString" select="substring-before($style, $prName)"/>
            <xsl:variable name="EndString" select="substring-after( substring-after( $style, $StartString ), ';' )"/>

            <xsl:call-template name="deletePr">
							<xsl:with-param name="style" select="concat($StartString, $EndString)"/>
							<xsl:with-param name="prName" select="$prName"/>
						</xsl:call-template>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="$style"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$style"/>
			</xsl:otherwise>
		</xsl:choose>

	</xsl:template>

	<!--<xsl:template match="thead" mode="table">-->
	<xsl:template name="thead">
		<xsl:param name="style"/>
		<xsl:param name="node" select="."/>

		<xsl:for-each select="$node">
			<xsl:variable name="genericStyle">
				<xsl:call-template name="GetElementStyle">
					<xsl:with-param name="tag" select="'thead'"/>
					<xsl:with-param name="class" select="@class"/>
          <xsl:with-param name="ID" select="@id" />
				</xsl:call-template>
				<xsl:text>;</xsl:text>
			</xsl:variable>

			<xsl:variable name="align">
				<xsl:if test="@align">
					<xsl:value-of select="concat( 'text-align: ', @align, ';' )"/>
				</xsl:if>
			</xsl:variable>

			<xsl:variable name="bgcolor">
				<xsl:if test="@bgcolor != ''">
					<xsl:value-of select="concat( 'tableBGcolor: ', @bgcolor, ';')"/>
				</xsl:if>
			</xsl:variable>

			<xsl:variable name="attr_style">
				<xsl:if test="@style">
					<xsl:call-template name="getTableBackGround">
						<xsl:with-param name="style" select="@style"/>
					</xsl:call-template>
					<xsl:text>;</xsl:text>
				</xsl:if>
			</xsl:variable>

			<xsl:if test="count(child::tr) != 0">
				<xsl:call-template name="InsertRowsGroup">
					<xsl:with-param name="style" select="concat( $style, '; ', $genericStyle, $attr_style, $align, $bgcolor, ';' )"/>
					<xsl:with-param name="node" select="."/>
				</xsl:call-template>
			</xsl:if>
		</xsl:for-each>
	</xsl:template>


	<!--<xsl:template match="tbody" mode="table">-->
	<xsl:template name="tbody">
		<xsl:param name="style"/>
		<xsl:param name="node" select="."/>

		<xsl:for-each select="$node">
			<xsl:variable name="genericStyle">
				<xsl:call-template name="GetElementStyle">
					<xsl:with-param name="tag" select="'tbody'"/>
					<xsl:with-param name="class" select="@class"/>
          <xsl:with-param name="ID" select="@id" />
				</xsl:call-template>
				<xsl:text>;</xsl:text>
			</xsl:variable>

			<xsl:variable name="align">
				<xsl:if test="@align">
					<xsl:value-of select="concat( 'text-align: ', @align, ';' )"/>
				</xsl:if>
			</xsl:variable>

			<xsl:variable name="bgcolor">
				<xsl:if test="@bgcolor != ''">
					<xsl:value-of select="concat( 'tableBGcolor: ', @bgcolor, ';')"/>
				</xsl:if>
			</xsl:variable>

			<xsl:variable name="attr_style">
				<xsl:if test="@style">
					<xsl:call-template name="getTableBackGround">
						<xsl:with-param name="style" select="@style"/>
					</xsl:call-template>
					<xsl:text>;</xsl:text>
				</xsl:if>
			</xsl:variable>

			<xsl:if test="count(child::tr) != 0">
				<xsl:call-template name="InsertRowsGroup">
					<xsl:with-param name="style" select="concat( $style, '; ', $genericStyle, $attr_style, $align, $bgcolor, ';' )"/>
					<xsl:with-param name="node" select="."/>
				</xsl:call-template>
			</xsl:if>
		</xsl:for-each>
	</xsl:template>


	<!--<xsl:template match="tfoot" mode="table">-->
	<xsl:template name="tfoot">
		<xsl:param name="style"/>
		<xsl:param name="node" select="."/>

		<xsl:for-each select="$node">
			<xsl:variable name="genericStyle">
				<xsl:call-template name="GetElementStyle">
					<xsl:with-param name="tag" select="'tfoot'"/>
					<xsl:with-param name="class" select="@class"/>
          <xsl:with-param name="ID" select="@id" />
				</xsl:call-template>
				<xsl:text>;</xsl:text>
			</xsl:variable>

			<xsl:variable name="align">
				<xsl:if test="@align">
					<xsl:value-of select="concat( 'text-align: ', @align, ';' )"/>
				</xsl:if>
			</xsl:variable>

			<xsl:variable name="bgcolor">
				<xsl:if test="@bgcolor != ''">
					<xsl:value-of select="concat( 'tableBGcolor: ', @bgcolor, ';')"/>
				</xsl:if>
			</xsl:variable>

			<xsl:variable name="attr_style">
				<xsl:if test="@style">
					<xsl:call-template name="getTableBackGround">
						<xsl:with-param name="style" select="@style"/>
					</xsl:call-template>
					<xsl:text>;</xsl:text>
				</xsl:if>
			</xsl:variable>

			<xsl:if test="count(child::tr) != 0">
				<xsl:call-template name="InsertRowsGroup">
					<xsl:with-param name="style" select="concat( $style, '; ', $genericStyle, $attr_style, $align, $bgcolor, ';' )"/>
					<xsl:with-param name="node" select="."/>
				</xsl:call-template>
			</xsl:if>
		</xsl:for-each>
	</xsl:template>


	<!--NEW-->
	<!--вставляем следующую группу строк-->
	<xsl:template name="InsertRowsGroup">
		<xsl:param name="style"/>
		<xsl:param name="node" select="."/>
		<xsl:param name="start" select="1"/>


		<xsl:for-each select="$node">
			<xsl:call-template name="InsertRow">
				<xsl:with-param name="style" select="$style"/>
				<xsl:with-param name="node" select="child::tr[$start]"/>
			</xsl:call-template>
		</xsl:for-each>
	</xsl:template>
	<!--/NEW-->


	<!--NEW-->
	<!--вставляем следующую строку группы-->
	<xsl:template name="InsertRow">
		<xsl:param name="style"/>
		<xsl:param name="node" select="."/>
		<xsl:param name="mask"/>

		<xsl:for-each select="$node">

			<!--вставка строки-->

			<xsl:variable name="rowStyles1">
				<xsl:call-template name="setRowStyle">
					<xsl:with-param name="style" select="$style"/>
					<xsl:with-param name="node" select="."/>
				</xsl:call-template>
			</xsl:variable>


			<xsl:variable name="rowStyles">
				<xsl:call-template name="RemovePr">
					<xsl:with-param name="style" select="$rowStyles1"/>
					<xsl:with-param name="namePr" select="'height'"/>
				</xsl:call-template>
			</xsl:variable>

			<xsl:if test="child::*[position() = 1]">
				<w:tr>
					<xsl:call-template name="InsertTrStyle">
						<xsl:with-param name="styleString" select="$rowStyles"/>
					</xsl:call-template>
					<xsl:call-template name="InsertRowWithMask">
						<xsl:with-param name="node" select="child::*[position() = 1]"/>
						<xsl:with-param name="mask" select="$mask"/>
						<xsl:with-param name="style" select="$rowStyles"/>
					</xsl:call-template>
				</w:tr>
			</xsl:if>

			<!--генерация маски этой строки-->
			<xsl:variable name="new_mask">
				<xsl:call-template name="createNewMask">
					<xsl:with-param name="node" select="child::*[position() = 1]"/>
					<xsl:with-param name="oldMask" select="$mask"/>
					<xsl:with-param name="rowStyles" select="$rowStyles"/>
				</xsl:call-template>
			</xsl:variable>

			<!--<xsl:value-of select="$new_mask"/>-->

			<!--флаг, указывающий на окончание группы связанных строк-->
			<xsl:variable name="isEndOfGroup">
				<xsl:call-template name="IsEndOfGroup">
					<xsl:with-param name="mask" select="$new_mask"/>
				</xsl:call-template>
			</xsl:variable>

			<xsl:choose>
				<xsl:when test="$isEndOfGroup = 'true'">
					<!--вставляем следующую группу строк-->
					<xsl:call-template name="InsertRowsGroup">
						<xsl:with-param name="style" select="$style"/>
						<xsl:with-param name="node" select="parent::*"/>
						<xsl:with-param name="start" select="count(preceding-sibling::tr) + 2"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:otherwise>
					<!--вставляем следующую строку этой группы-->
					<xsl:call-template name="InsertRow">
						<xsl:with-param name="style" select="$style"/>
						<xsl:with-param name="node" select="following-sibling::tr[position() = 1]"/>
						<xsl:with-param name="mask" select="$new_mask"/>
					</xsl:call-template>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:for-each>

	</xsl:template>
	<!--/NEW-->


	<!--NEW-->
	<xsl:template name="createNewMask">
		<xsl:param name="node" select="."/>
		<xsl:param name="oldMask"/>
		<xsl:param name="numFromMask" select="1"/>
		<xsl:param name="numFromRow" select="1"/>
		<xsl:param name="numPartInHMergedFromMask" select="1"/>
		<xsl:param name="NUM" select="1"/>
		<xsl:param name="rowStyles" />

		<xsl:choose>
			<xsl:when test="$oldMask = ''">
				<xsl:text>POINT_1_</xsl:text>
				<xsl:for-each select="$node">
					<xsl:for-each select="parent::tr">
						<xsl:for-each select="child::*">
							<xsl:value-of select="concat('(', count(preceding-sibling::*) + 1, '):')"/>

							<xsl:choose>
								<xsl:when test="@colspan != ''">
									<xsl:value-of select="@colspan"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:text>1</xsl:text>
								</xsl:otherwise>
							</xsl:choose>
							<xsl:text>-</xsl:text>
							<xsl:choose>
								<xsl:when test="@rowspan != ''">
									<xsl:value-of select="@rowspan"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:text>1</xsl:text>
								</xsl:otherwise>
							</xsl:choose>
							<xsl:text>[</xsl:text>
							<xsl:call-template name="insertCellStyle">
								<xsl:with-param name="style" select="$rowStyles"/>
								<xsl:with-param name="node" select="."/>
								<xsl:with-param name="delRowspan" select="'true'"/>
							</xsl:call-template>
							<xsl:text>]||</xsl:text>
						</xsl:for-each>
					</xsl:for-each>
				</xsl:for-each>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>POINT_2_</xsl:text>
				<xsl:variable name="curPart">
					<xsl:call-template name="GetPartOfMask">
						<xsl:with-param name="mask" select="$oldMask"/>
						<xsl:with-param name="num" select="$numFromMask"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:choose>
					<xsl:when test="$curPart != ''">
						<xsl:variable name="vert">
							<xsl:call-template name="getMerged">
								<xsl:with-param name="partOfMask" select="$curPart"/>
								<xsl:with-param name="name" select="'vert'"/>
							</xsl:call-template>
						</xsl:variable>
						<xsl:variable name="hory">
							<xsl:call-template name="getMerged">
								<xsl:with-param name="partOfMask" select="$curPart"/>
								<xsl:with-param name="name" select="'hory'"/>
							</xsl:call-template>
						</xsl:variable>
						<xsl:variable name="upCellStyle">
							<xsl:call-template name="getMerged">
								<xsl:with-param name="partOfMask" select="$curPart"/>
								<xsl:with-param name="name" select="'style'"/>
							</xsl:call-template>
						</xsl:variable>
						<xsl:choose>
							<xsl:when test="$vert &gt; 1">
								<xsl:text>POINT_3_</xsl:text>
								<xsl:value-of select="concat('(', $NUM, '):', $hory, '-', number($vert) - 1, '[', $upCellStyle, ']||')"/>

								<xsl:call-template name="createNewMask">
									<xsl:with-param name="node" select="$node"/>
									<xsl:with-param name="oldMask" select="$oldMask"/>
									<xsl:with-param name="numFromMask" select="number($numFromMask) + 1"/>
									<xsl:with-param name="numFromRow" select="number($numFromRow)"/>
									<xsl:with-param name="numPartInHMergedFromMask" select="1"/>
									<xsl:with-param name="NUM" select="number($NUM) + 1"/>
									<xsl:with-param name="rowStyles" select="$rowStyles"/>
								</xsl:call-template>

							</xsl:when>
							<xsl:otherwise>
								<xsl:text>POINT_4_</xsl:text>
								<xsl:for-each select="$node">
									<xsl:variable name="countHMergedInCell">
										<xsl:choose>
											<xsl:when test="@colspan != ''">
												<xsl:value-of select="@colspan"/>
											</xsl:when>
											<xsl:otherwise>
												<xsl:text>1</xsl:text>
											</xsl:otherwise>
										</xsl:choose>
									</xsl:variable>

									<xsl:variable name="countVMergedInCell">
										<xsl:choose>
											<xsl:when test="@rowspan != ''">
												<xsl:value-of select="@rowspan"/>
											</xsl:when>
											<xsl:otherwise>
												<xsl:text>1</xsl:text>
											</xsl:otherwise>
										</xsl:choose>
									</xsl:variable>

									<xsl:variable name="thisCellStyle">
										<xsl:call-template name="insertCellStyle">
											<xsl:with-param name="style" select="$rowStyles"/>
											<xsl:with-param name="node" select="."/>
											<xsl:with-param name="delRowspan" select="'true'"/>
										</xsl:call-template>
									</xsl:variable>

									<xsl:value-of select="concat('(', $NUM, '):', $countHMergedInCell, '-', $countVMergedInCell, '[', $thisCellStyle, ']||')"/>

									<xsl:variable name="a">
										<xsl:value-of select="number($hory) - number($numPartInHMergedFromMask) + 1 - number($countHMergedInCell)"/>
									</xsl:variable>

									<xsl:choose>
										<xsl:when test="number($a) &gt; 0">
											<xsl:text>POINT_5_</xsl:text>
											<xsl:call-template name="createNewMask">
												<xsl:with-param name="node" select="following-sibling::*[position() = 1]"/>
												<xsl:with-param name="oldMask" select="$oldMask"/>
												<xsl:with-param name="numFromMask" select="$numFromMask"/>
												<xsl:with-param name="numFromRow" select="number($numFromRow) + 1"/>
												<xsl:with-param name="numPartInHMergedFromMask" select="number($hory) + 1 - number($a)"/>
												<xsl:with-param name="NUM" select="number($NUM) + 1"/>
												<xsl:with-param name="rowStyles" select="$rowStyles"/>
											</xsl:call-template>
										</xsl:when>
										<xsl:when test="number($a) = 0">
											<xsl:text>POINT_6_</xsl:text>
											<xsl:call-template name="createNewMask">
												<xsl:with-param name="node" select="following-sibling::*[position() = 1]"/>
												<xsl:with-param name="oldMask" select="$oldMask"/>
												<xsl:with-param name="numFromMask" select="number($numFromMask) + 1"/>
												<xsl:with-param name="numFromRow" select="number($numFromRow) + 1"/>
												<xsl:with-param name="numPartInHMergedFromMask" select="1"/>
												<xsl:with-param name="NUM" select="number($NUM) + 1"/>
												<xsl:with-param name="rowStyles" select="$rowStyles"/>
											</xsl:call-template>
										</xsl:when>
										<xsl:otherwise>
											<xsl:text>POINT_7_</xsl:text>
											<xsl:variable name="newNumFromMask">
												<xsl:call-template name="getNewNumFromMask">
													<xsl:with-param name="mask" select="$oldMask"/>
													<xsl:with-param name="numInMask" select="number($numFromMask) + 1"/>
													<xsl:with-param name="len" select="$a"/>
												</xsl:call-template>
											</xsl:variable>
											<xsl:variable name="newNumPartInHMergedFromMask">
												<xsl:call-template name="getNewNumPartInHMergedFromMask">
													<xsl:with-param name="mask" select="$oldMask"/>
													<xsl:with-param name="numInMask" select="number($numFromMask) + 1"/>
													<xsl:with-param name="len" select="$a"/>
												</xsl:call-template>
											</xsl:variable>
											<xsl:call-template name="createNewMask">
												<xsl:with-param name="node" select="following-sibling::*[position() = 1]"/>
												<xsl:with-param name="oldMask" select="$oldMask"/>
												<xsl:with-param name="numFromMask" select="$newNumFromMask"/>
												<xsl:with-param name="numFromRow" select="number($numFromRow) + 1"/>
												<xsl:with-param name="numPartInHMergedFromMask" select="$newNumPartInHMergedFromMask"/>
												<xsl:with-param name="NUM" select="number($NUM) + 1"/>
												<xsl:with-param name="rowStyles" select="$rowStyles"/>
											</xsl:call-template>
										</xsl:otherwise>
									</xsl:choose>
								</xsl:for-each>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:when>
					<xsl:otherwise>
						<xsl:for-each select="$node">
							<xsl:value-of select="concat('(', number($NUM), ');')"/>

							<xsl:choose>
								<xsl:when test="@colspan != ''">
									<xsl:value-of select="@colspan"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:text>1</xsl:text>
								</xsl:otherwise>
							</xsl:choose>
							<xsl:text>-</xsl:text>
							<xsl:choose>
								<xsl:when test="@rowspan != ''">
									<xsl:value-of select="@rowspan"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:text>1</xsl:text>
								</xsl:otherwise>
							</xsl:choose>
							<xsl:text>[</xsl:text>
							<xsl:call-template name="insertCellStyle">
								<xsl:with-param name="style" select="$rowStyles"/>
								<xsl:with-param name="node" select="."/>
								<xsl:with-param name="delRowspan" select="'true'"/>
							</xsl:call-template>
							<xsl:text>]||</xsl:text>

							<xsl:for-each select="following-sibling::*">
								<xsl:value-of select="concat('(', number($NUM) + position(), ');')"/>
								<xsl:choose>
									<xsl:when test="@colspan != ''">
										<xsl:value-of select="@colspan"/>
									</xsl:when>
									<xsl:otherwise>
										<xsl:text>1</xsl:text>
									</xsl:otherwise>
								</xsl:choose>
								<xsl:text>-</xsl:text>
								<xsl:choose>
									<xsl:when test="@rowspan != ''">
										<xsl:value-of select="@rowspan"/>
									</xsl:when>
									<xsl:otherwise>
										<xsl:text>1</xsl:text>
									</xsl:otherwise>
								</xsl:choose>
								<xsl:text>[</xsl:text>
								<xsl:call-template name="insertCellStyle">
									<xsl:with-param name="style" select="$rowStyles"/>
									<xsl:with-param name="node" select="."/>
									<xsl:with-param name="delRowspan" select="'true'"/>
								</xsl:call-template>
								<xsl:text>]||</xsl:text>
							</xsl:for-each>
						</xsl:for-each>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:otherwise>
		</xsl:choose>

	</xsl:template>
	<!--/NEW-->


	<!--NEW-->
	<xsl:template name="InsertRowWithMask">
		<xsl:param name="mask"/>
		<xsl:param name="node" select="."/>
		<xsl:param name="style"/>
		<xsl:param name="numFromMask" select="1"/>
		<xsl:param name="numFromRow" select="1"/>
		<xsl:param name="numPartInHMergedFromMask" select="1"/>
		<xsl:param name="NUM" select="1"/>


		<xsl:choose>
			<xsl:when test="$mask = ''">
				<xsl:for-each select="$node">
					<xsl:for-each select="parent::tr">
						<xsl:apply-templates mode="tr">
							<xsl:with-param name="style" select="$style"/>
						</xsl:apply-templates>
					</xsl:for-each>
				</xsl:for-each>
			</xsl:when>
			<xsl:otherwise>
				<xsl:variable name="curPart">
					<xsl:call-template name="GetPartOfMask">
						<xsl:with-param name="mask" select="$mask"/>
						<xsl:with-param name="num" select="$numFromMask"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:choose>
					<xsl:when test="$curPart != ''">
						<xsl:variable name="vert">
							<xsl:call-template name="getMerged">
								<xsl:with-param name="partOfMask" select="$curPart"/>
								<xsl:with-param name="name" select="'vert'"/>
							</xsl:call-template>
						</xsl:variable>
						<xsl:variable name="hory">
							<xsl:call-template name="getMerged">
								<xsl:with-param name="partOfMask" select="$curPart"/>
								<xsl:with-param name="name" select="'hory'"/>
							</xsl:call-template>
						</xsl:variable>
						<xsl:variable name="upCellStyle">
							<xsl:call-template name="getMerged">
								<xsl:with-param name="partOfMask" select="$curPart"/>
								<xsl:with-param name="name" select="'style'"/>
							</xsl:call-template>
						</xsl:variable>
						<xsl:choose>
							<xsl:when test="$vert &gt; 1">
								<xsl:call-template name="InsertVMergedCell">
									<xsl:with-param name="style" select="$upCellStyle"/>
								</xsl:call-template>
								<xsl:call-template name="InsertRowWithMask">
									<xsl:with-param name="style" select="$style"/>
									<xsl:with-param name="node" select="$node"/>
									<xsl:with-param name="mask" select="$mask"/>
									<xsl:with-param name="numFromMask" select="number($numFromMask) + 1"/>
									<xsl:with-param name="numFromRow" select="number($numFromRow)"/>
									<xsl:with-param name="numPartInHMergedFromMask" select="1"/>
								</xsl:call-template>
							</xsl:when>
							<xsl:otherwise>
								<xsl:for-each select="$node">
									<xsl:variable name="countHMergedInCell">
										<xsl:choose>
											<xsl:when test="@colspan != ''">
												<xsl:value-of select="@colspan"/>
											</xsl:when>
											<xsl:otherwise>
												<xsl:text>1</xsl:text>
											</xsl:otherwise>
										</xsl:choose>
									</xsl:variable>

									<xsl:apply-templates select="." mode="tr">
										<xsl:with-param name="style" select="$style"/>
									</xsl:apply-templates>

									<xsl:variable name="a">
										<xsl:value-of select="number($hory) - number($numPartInHMergedFromMask) + 1 - number($countHMergedInCell)"/>
									</xsl:variable>

									<xsl:choose>
										<xsl:when test="number($a) &gt; 0">
											<xsl:call-template name="InsertRowWithMask">
												<xsl:with-param name="style" select="$style"/>
												<xsl:with-param name="node" select="following-sibling::*[position() = 1]"/>
												<xsl:with-param name="mask" select="$mask"/>
												<xsl:with-param name="numFromMask" select="$numFromMask"/>
												<xsl:with-param name="numFromRow" select="number($numFromRow) + 1"/>
												<xsl:with-param name="numPartInHMergedFromMask" select="number($hory) + 1 - number($a)"/>
											</xsl:call-template>
										</xsl:when>
										<xsl:when test="number($a) = 0">
											<xsl:call-template name="InsertRowWithMask">
												<xsl:with-param name="style" select="$style"/>
												<xsl:with-param name="node" select="following-sibling::*[position() = 1]"/>
												<xsl:with-param name="mask" select="$mask"/>
												<xsl:with-param name="numFromMask" select="number($numFromMask) + 1"/>
												<xsl:with-param name="numFromRow" select="number($numFromRow) + 1"/>
												<xsl:with-param name="numPartInHMergedFromMask" select="1"/>
											</xsl:call-template>
										</xsl:when>
										<xsl:otherwise>
											<xsl:variable name="newNumFromMask">
												<xsl:call-template name="getNewNumFromMask">
													<xsl:with-param name="mask" select="$mask"/>
													<xsl:with-param name="numInMask" select="number($numFromMask) + 1"/>
													<xsl:with-param name="len" select="$a"/>
												</xsl:call-template>
											</xsl:variable>
											<xsl:variable name="newNumPartInHMergedFromMask">
												<xsl:call-template name="getNewNumPartInHMergedFromMask">
													<xsl:with-param name="mask" select="$mask"/>
													<xsl:with-param name="numInMask" select="number($numFromMask) + 1"/>
													<xsl:with-param name="len" select="$a"/>
												</xsl:call-template>
											</xsl:variable>
											<xsl:call-template name="InsertRowWithMask">
												<xsl:with-param name="style" select="$style"/>
												<xsl:with-param name="node" select="following-sibling::*[position() = 1]"/>
												<xsl:with-param name="mask" select="$mask"/>
												<xsl:with-param name="numFromMask" select="$newNumFromMask"/>
												<xsl:with-param name="numFromRow" select="number($numFromRow) + 1"/>
												<xsl:with-param name="numPartInHMergedFromMask" select="$newNumPartInHMergedFromMask"/>
											</xsl:call-template>
										</xsl:otherwise>
									</xsl:choose>
								</xsl:for-each>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:when>
					<xsl:otherwise>
						<xsl:for-each select="$node">

							<xsl:apply-templates select="." mode="tr">
								<xsl:with-param name="style" select="$style"/>
							</xsl:apply-templates>

							<xsl:for-each select="following-sibling::*">
								<xsl:apply-templates select="." mode="tr">
									<xsl:with-param name="style" select="$style"/>
								</xsl:apply-templates>
							</xsl:for-each>
						</xsl:for-each>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<!--/NEW-->


	<xsl:template name="InsertVMergedCell">
		<xsl:param name="style"/>

		<w:tc>
			<w:tcPr>

				<xsl:call-template name="InsertTcStyle">
					<xsl:with-param name="styleString" select="$style"/>
				</xsl:call-template>
				<w:vMerge />
			</w:tcPr>
			<w:p w:rsidR="007D01D8" w:rsidRDefault="007D01D8" w:rsidP="007D01D8">
				<w:pPr/>
			</w:p>
		</w:tc>
	</xsl:template>


	<!--NEW-->
	<xsl:template name="getNewNumPartInHMergedFromMask">
		<xsl:param name="mask"/>
		<xsl:param name="numInMask"/>
		<xsl:param name="len"/>

		<xsl:variable name="curPart">
			<xsl:call-template name="GetPartOfMask">
				<xsl:with-param name="mask" select="$mask"/>
				<xsl:with-param name="num" select="$numInMask"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:variable name="hory">
			<xsl:call-template name="getMerged">
				<xsl:with-param name="partOfMask" select="$curPart"/>
				<xsl:with-param name="name" select="'hory'"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:if test="$hory != ''">
			<xsl:variable name="a">
				<xsl:value-of select="number($hory) + number($len)"/>
			</xsl:variable>
			<xsl:choose>
				<xsl:when test="number($a) &gt; 0">
					<xsl:value-of select="number($hory) + number($a)"/>
				</xsl:when>
				<xsl:when test="number($a) = 0">
					<xsl:value-of select="1"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name="getNewNumPartInHMergedFromMask">
						<xsl:with-param name="mask" select="$mask"/>
						<xsl:with-param name="numInMask" select="number($numInMask) + 1"/>
						<xsl:with-param name="len" select="$a"/>
					</xsl:call-template>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:if>
	</xsl:template>
	<!--/NEW-->


	<!--NEW-->
	<xsl:template name="getNewNumFromMask">
		<xsl:param name="mask"/>
		<xsl:param name="numInMask"/>
		<xsl:param name="len"/>

		<xsl:variable name="curPart">
			<xsl:call-template name="GetPartOfMask">
				<xsl:with-param name="mask" select="$mask"/>
				<xsl:with-param name="num" select="$numInMask"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:variable name="hory">
			<xsl:call-template name="getMerged">
				<xsl:with-param name="partOfMask" select="$curPart"/>
				<xsl:with-param name="name" select="'hory'"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:if test="$hory != ''">
			<xsl:variable name="a">
				<xsl:value-of select="number($hory) + number($len)"/>
			</xsl:variable>
			<xsl:choose>
				<xsl:when test="number($a) &gt; 0">
					<xsl:value-of select="$numInMask"/>
				</xsl:when>
				<xsl:when test="number($a) = 0">
					<xsl:value-of select="number($numInMask) + 1"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name="getNewNumFromMask">
						<xsl:with-param name="mask" select="$mask"/>
						<xsl:with-param name="numInMask" select="number($numInMask) + 1"/>
						<xsl:with-param name="len" select="$a"/>
					</xsl:call-template>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:if>
	</xsl:template>
	<!--/NEW-->


	<!--NEW-->
	<xsl:template name="getMerged">
		<xsl:param name="partOfMask"/>
		<xsl:param name="name"/>

		<xsl:choose>
			<xsl:when test="$name = 'hory'">
				<xsl:value-of select="substring-before($partOfMask, '-')"/>
			</xsl:when>
			<xsl:when test="$name = 'vert'">
				<xsl:value-of select="substring-before(substring-after($partOfMask, '-'), '[')"/>
			</xsl:when>
			<xsl:when test="$name = 'style'">
				<xsl:value-of select="substring-before(substring-after($partOfMask, '['), ']')"/>
			</xsl:when>
		</xsl:choose>
	</xsl:template>
	<!--/NEW-->


	<!--NEW-->
	<xsl:template name="GetPartOfMask">
		<xsl:param name="mask"/>
		<xsl:param name="num"/>

		<xsl:variable name="str">
			<xsl:value-of select="concat('(', $num, '):')"/>
		</xsl:variable>
		<xsl:value-of select="substring-before(substring-after($mask, $str), '||')"/>
	</xsl:template>
	<!--/NEW-->


	<!--NEW-->
	<xsl:template name="IsEndOfGroup">
		<xsl:param name="mask"/>
		<xsl:param name="num" select="1"/>

		<xsl:variable name="curPart">
			<xsl:call-template name="GetPartOfMask">
				<xsl:with-param name="mask" select="$mask"/>
				<xsl:with-param name="num" select="$num"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:choose>
			<xsl:when test="$curPart = ''">
				<xsl:text>true</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:variable name="vertMerged">
					<xsl:call-template name="getMerged">
						<xsl:with-param name="partOfMask" select="$curPart"/>
						<xsl:with-param name="name" select="'vert'"/>
					</xsl:call-template>
				</xsl:variable>

				<xsl:choose>
					<xsl:when test="number($vertMerged) &gt; 1">
						<xsl:text>false</xsl:text>
					</xsl:when>
					<xsl:otherwise>
						<xsl:call-template name="IsEndOfGroup">
							<xsl:with-param name="mask" select="$mask"/>
							<xsl:with-param name="num" select="number($num) + 1"/>
						</xsl:call-template>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<!--/NEW-->


	<!-- Table row -->
	<xsl:template match="tr" mode="table">
		<xsl:param name="style"/>

		<xsl:variable name="styles">
			<xsl:call-template name="setRowStyle">
				<xsl:with-param name="style" select="$style"/>
				<xsl:with-param name="node" select="."/>
			</xsl:call-template>

		</xsl:variable>

		<w:tr w:rsidR="004D28DB">
			<xsl:call-template name="InsertTrStyle">
				<xsl:with-param name="styleString" select="$styles"/>
			</xsl:call-template>
			<xsl:apply-templates mode="tr">
				<xsl:with-param name="style" select="$styles"/>
			</xsl:apply-templates>
		</w:tr>
	</xsl:template>


	<!--NEW-->
	<xsl:template name="setRowStyle">
		<xsl:param name="style"/>
		<xsl:param name="node" select="."/>

		<xsl:for-each select="$node">
			<xsl:variable name="genericStyle">
				<xsl:call-template name="GetElementStyle">
					<xsl:with-param name="tag" select="'tr'"/>
					<xsl:with-param name="class" select="@class"/>
          <xsl:with-param name="ID" select="@id" />
				</xsl:call-template>
				<xsl:text>;</xsl:text>
			</xsl:variable>

			<xsl:variable name="align">
				<xsl:if test="@align">
					<xsl:value-of select="concat( 'text-align: ', @align, ';' )"/>
				</xsl:if>
			</xsl:variable>

			<xsl:variable name="valign">
				<xsl:if test="@valign">
					<xsl:value-of select="concat( 'vertical-align: ', @valign, ';' )"/>
				</xsl:if>
			</xsl:variable>

			<xsl:variable name="bgcolor">
				<xsl:if test="@bgcolor != ''">
					<xsl:value-of select="concat( 'tableBGcolor: ', @bgcolor, ';')"/>
				</xsl:if>
			</xsl:variable>

			<xsl:variable name="height">
				<xsl:if test="@height  != ''">
					<xsl:value-of select="concat( 'height: ', @height , ';')"/>
				</xsl:if>
			</xsl:variable>

			<xsl:variable name="attr_style">
				<xsl:if test="@style">
					<xsl:call-template name="getTableBackGround">
						<xsl:with-param name="style" select="@style"/>
					</xsl:call-template>
					<xsl:text>;</xsl:text>
				</xsl:if>
			</xsl:variable>

			<xsl:value-of select="concat( $style, '; ', $genericStyle, @style, '; ', $attr_style, $align, $valign, $bgcolor, $height )"/>
		</xsl:for-each>
	</xsl:template>
	<!--NEW-->


	<!-- Table cell -->
	<xsl:template match="td | th" mode="tr">
		<xsl:param name="style"/>

		<xsl:variable name="styleString">
			<xsl:call-template name="insertCellStyle">
				<xsl:with-param name="style" select="$style"/>
				<xsl:with-param name="node" select="."/>
			</xsl:call-template>
			<xsl:if test="name(.) = 'th'">
				<xsl:text>;font-weight:bold;</xsl:text>
			</xsl:if>
		</xsl:variable>

		<xsl:variable name="bgcolor">
			<xsl:if test="@bgcolor != ''">
				<xsl:value-of select="concat( 'tableBGcolor: ', @bgcolor, ';')"/>
			</xsl:if>
		</xsl:variable>

		<xsl:variable name="attr_style">
			<xsl:if test="@style">
				<xsl:call-template name="getTableBackGround">
					<xsl:with-param name="style" select="@style"/>
				</xsl:call-template>
				<xsl:text>;</xsl:text>
			</xsl:if>
		</xsl:variable>

		<w:tc>
			<w:tcPr>
				<xsl:call-template name="InsertTcStyle">
					<xsl:with-param name="styleString" select="concat($styleString, $bgcolor, $attr_style)"/>
				</xsl:call-template>
			</w:tcPr>

			<xsl:variable name="attr_par_style">
				<xsl:call-template name="getParagraphBackGround">
					<xsl:with-param name="style">
						<xsl:value-of select="concat($styleString, $bgcolor, $attr_style)"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:variable>

			<xsl:call-template name="InsertParagraph">
				<xsl:with-param name="style" select="$attr_par_style"/>
			</xsl:call-template>
		</w:tc>
	</xsl:template>


	<xsl:template name="insertCellStyle">
		<xsl:param name="style"/>
		<xsl:param name="node" select="."/>
		<xsl:param name="delRowspan" select="'false'"/>

		<xsl:for-each select="$node">
			<xsl:if test="name(parent::*) = 'tr'">
				<xsl:variable name="genericStyle">
					<xsl:call-template name="GetElementStyle">
						<xsl:with-param name="tag" select="'td'"/>
						<xsl:with-param name="class" select="@class"/>
            <xsl:with-param name="ID" select="@id" />
					</xsl:call-template>
					<xsl:text>;</xsl:text>
				</xsl:variable>

				<xsl:variable name="align">
					<xsl:if test="@align">
						<xsl:value-of select="concat( 'text-align: ', @align , ';')"/>
					</xsl:if>
				</xsl:variable>

				<xsl:variable name="valign">
					<xsl:if test="@valign">
						<xsl:value-of select="concat( 'valign: ', @valign , ';')"/>
					</xsl:if>
				</xsl:variable>

				<xsl:variable name="colSpan">
					<xsl:if test="@colspan">
						<xsl:value-of select="concat( 'colspan: ', @colspan, ';' )"/>
					</xsl:if>
				</xsl:variable>

				<xsl:variable name="rowSpan">
					<xsl:if test="@rowspan and $delRowspan != 'true'">
						<xsl:value-of select="concat( 'rowspan: ', @rowspan, ';' )"/>
					</xsl:if>
				</xsl:variable>

				<xsl:variable name="width">
					<xsl:if test="@width != ''">
						<xsl:call-template name="someWidth">
							<xsl:with-param name="styleString" select="$style"/>
							<xsl:with-param name="attr_width" select="@width"/>
						</xsl:call-template>
					</xsl:if>
				</xsl:variable>

				<xsl:variable name="bgcolor">
					<xsl:if test="@bgcolor != ''">
						<xsl:value-of select="concat( 'tableBGcolor: ', @bgcolor, ';')"/>
					</xsl:if>
				</xsl:variable>

				<xsl:variable name="attr_style">
					<xsl:if test="@style">
						<xsl:call-template name="getTableBackGround">
							<xsl:with-param name="style" select="@style"/>
						</xsl:call-template>
						<xsl:text>;</xsl:text>
					</xsl:if>
				</xsl:variable>

				<xsl:variable name="font-size">
					<xsl:if test="@font-size != ''">
						<xsl:value-of select="concat( 'size: ', @font-size, ';' )"/>
					</xsl:if>
				</xsl:variable>

				<xsl:variable name="font-family">
					<xsl:if test="@font-size != ''">
						<xsl:value-of select="concat( 'face: ', @font-family, ';' )"/>
					</xsl:if>
				</xsl:variable>

				<xsl:variable name="styleString" select="concat( $style, ';', $attr_style, ';', $genericStyle, $align, $colSpan, $rowSpan, $bgcolor, $font-family, $font-size, $valign )"/>

				<xsl:variable name="newStyleString">
					<xsl:call-template name="styleWithoutWidth">
						<xsl:with-param name="style" select="$styleString"/>
					</xsl:call-template>
				</xsl:variable>

				<xsl:value-of select="concat($newStyleString, ';', $width)"/>
			</xsl:if>
		</xsl:for-each>
	</xsl:template>


	<xsl:template name="someWidth">
		<xsl:param name="styleString"/>
		<xsl:param name="attr_width"/>
		<xsl:choose>
			<xsl:when test="contains($attr_width, '%')">
				<xsl:variable name="w">
					<xsl:call-template name="GetPropertyFromStyle">
						<xsl:with-param name="styleString" select="$styleString"/>
						<xsl:with-param name="propertyName" select="'width'"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:choose>
					<xsl:when test="$w != ''">
						<xsl:variable name="ed">
							<xsl:choose>
								<xsl:when test="contains($w, '%')">
									<xsl:text>%</xsl:text>
								</xsl:when>
								<xsl:when test="number($w) != NaN"/>
								<xsl:otherwise>
									<xsl:text>px</xsl:text>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:variable>
						<xsl:variable name="unit">
							<xsl:choose>
								<xsl:when test="contains($w, '%')">
									<xsl:value-of select="substring-before($w, '%')"/>
								</xsl:when>
								<xsl:when test="number($w) != NaN">
									<xsl:value-of select="$w"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:variable name="full">
										<xsl:call-template name="ConvertToPixels">
											<xsl:with-param name="length" select="$w"/>
										</xsl:call-template>
									</xsl:variable>
									<xsl:value-of select="substring-before($full, 'px')"/>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:variable>
						<xsl:value-of select="concat('width:', string(number($unit) * number(substring-before($attr_width, '%')) div 100), $ed, ';')"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="concat( 'width: ', $attr_width, ';' )"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="concat( 'width: ', $attr_width, ';' )"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<xsl:template name="cut">
		<xsl:param name="w"/>
		<xsl:choose>
			<xsl:when test="contains($w, '%')">
				<xsl:value-of select="substring-before($w, '%')"/>
			</xsl:when>
			<xsl:when test="number($w) != 'NaN'">
				<xsl:value-of select="$w"/>
			</xsl:when>
		</xsl:choose>
	</xsl:template>


	<xsl:template name="styleWithoutWidth">
		<xsl:param name="style"/>
		<xsl:choose>
			<xsl:when test="contains($style, 'width')">
				<xsl:call-template name="styleWithoutWidth">
					<xsl:with-param name="style">
						<xsl:value-of select="concat(substring-before($style, 'width' ), substring-after($style, 'width'))"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$style"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<!-- Hyperlink -->
	<xsl:template match="a" mode="paragraph">
		<xsl:param name="style"/>

		<xsl:variable name="new_style">
			<xsl:choose>
				<xsl:when test="count(preceding-sibling::*) = 0">
					<xsl:value-of select="$style"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name="deleteListPr">
						<xsl:with-param name="style" select="$style"/>
					</xsl:call-template>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="genericStyle">
			<xsl:call-template name="GetElementStyle">
				<xsl:with-param name="tag" select="'a'"/>
				<xsl:with-param name="class" select="@class"/>
        <xsl:with-param name="ID" select="@id" />
			</xsl:call-template>
			<xsl:text>;</xsl:text>
		</xsl:variable>

		<xsl:variable name="align">
			<xsl:if test="@align != ''">
				<xsl:value-of select="concat( 'text-align: ', @align , ';')"/>
			</xsl:if>
		</xsl:variable>

		<xsl:variable name="styleString">
			<xsl:choose>
				<xsl:when test="descendant::img">
					<xsl:value-of select="concat( $new_style, '; ', $genericStyle, @style, ';', $align )"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="concat( $new_style, ';', $genericStyle, @style, ';', $align )"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="display">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="'display'"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="attr_style">
			<xsl:call-template name="getParagraphBackGround">
				<xsl:with-param name="style">
					<xsl:value-of select="$new_style"/>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:variable>

		<xsl:if test="$display = 'block'">
			<xsl:call-template name="CloseParagraph"/>
			<xsl:text disable-output-escaping="yes">&lt;w:p w:rsidR="004D5F21" w:rsidRPr="00820E0B" w:rsidRDefault="00301D39" pcut:cut="true"&gt;</xsl:text>
			<w:pPr>

				<xsl:if test="@class">
					<w:pStyle w:val="{ concat( 'custom_', @class ) }"/>
				</xsl:if>

				<xsl:call-template name="InsertParagraphStyle">
					<xsl:with-param name="styleString">
						<xsl:value-of select="$attr_style"/>
					</xsl:with-param>
					<xsl:with-param name="node" select="."/>
				</xsl:call-template>

				<w:rPr>
					<xsl:call-template name="InsertRunStyle">
						<xsl:with-param name="styleString">
							<xsl:value-of select="$attr_style"/>
						</xsl:with-param>
					</xsl:call-template>
					<w:lang w:val="en-US"/>
				</w:rPr>
			</w:pPr>
		</xsl:if>

		<xsl:variable name="id">
			<xsl:value-of select="generate-id(.)"/>
		</xsl:variable>

		<xsl:if test="@name != ''">
			<w:bookmarkStart>
				<xsl:attribute name="w:id">
					<xsl:value-of select="$id"/>
				</xsl:attribute>
				<xsl:attribute name="w:name">
					<xsl:value-of select="@name"/>
				</xsl:attribute>
			</w:bookmarkStart>
		</xsl:if>

		<xsl:variable name="validHref">
			<xsl:call-template name="hrefIsValid">
				<xsl:with-param name="href" select="@href"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:choose>
			<xsl:when test="$validHref = 'true'">
				<w:hyperlink>
					<xsl:choose>
						<xsl:when test="starts-with( @href, '#' )">
							<xsl:attribute name="w:anchor">
								<xsl:value-of select="substring(@href, 2)"/>
							</xsl:attribute>
						</xsl:when>
						<!--<xsl:when test="contains(translate(@href, 'ABCDEFGHIJKLMNOPQRSTUVWXYZ', 'abcdefghijklmnopqrstuvwxyz'), 'http:')">-->
						<xsl:otherwise>
							<xsl:attribute name="r:id">
								<xsl:call-template name="getRefID"/>
							</xsl:attribute>
						</xsl:otherwise>
						<!--</xsl:when>-->
					</xsl:choose>
					<xsl:apply-templates mode="paragraph">
						<xsl:with-param name="style" select="concat($attr_style, '; text-decoration:underline; color:#0000FF; ', $genericStyle, @style, ';', $align)"/>
					</xsl:apply-templates>
				</w:hyperlink>

			</xsl:when>
			<xsl:otherwise>
				<xsl:apply-templates mode="paragraph">
					<xsl:with-param name="style" select="$style"/>
				</xsl:apply-templates>
			</xsl:otherwise>
		</xsl:choose>

		<xsl:if test="@name != ''">
			<w:bookmarkEnd>
				<xsl:attribute name="w:id">
					<xsl:value-of select="$id"/>
				</xsl:attribute>
			</w:bookmarkEnd>
		</xsl:if>

		<xsl:if test="$display = 'block'">
			<xsl:text disable-output-escaping="yes">&lt;/w:p&gt;</xsl:text>
			<xsl:call-template name="OpenParagraph">
				<xsl:with-param name="style" select="$attr_style"/>
			</xsl:call-template>
		</xsl:if>

	</xsl:template>

	<!-- Image -->
	<xsl:template match="img[ @src ]" mode="run">

    <xsl:variable name="Float">
      <xsl:call-template name="GetPropertyFromStyle">
        <xsl:with-param name="styleString" select="@style"/>
        <xsl:with-param name="propertyName" select="'float'"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="MarginLeft">
      <xsl:call-template name="GetImageMargin">
        <xsl:with-param name="Name" select="'margin-left'"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="MarginTop">
      <xsl:call-template name="GetImageMargin">
        <xsl:with-param name="Name" select="'margin-top'"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="MarginRight">
      <xsl:call-template name="GetImageMargin">
        <xsl:with-param name="Name" select="'margin-right'"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="MarginBottom">
      <xsl:call-template name="GetImageMargin">
        <xsl:with-param name="Name" select="'margin-bottom'"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="RawWidth">
      <xsl:value-of select="number(@width) + $MarginLeft + $MarginRight"/>
    </xsl:variable>

    <xsl:variable name="RawHeight">
      <xsl:value-of select="number(@height) + $MarginTop + $MarginBottom"/>
    </xsl:variable>

    <xsl:variable name="width">
      <xsl:if test="@width">
        <xsl:value-of select="( number(@width) + $MarginLeft + $MarginRight ) * 3 div 4 * 12700"/>
      </xsl:if>
    </xsl:variable>
    
    <xsl:variable name="height">
      <xsl:if test="@height">
        <xsl:value-of select="( number(@height) + $MarginTop + $MarginBottom ) * 3 div 4 * 12700"/>
      </xsl:if>
    </xsl:variable>
    
    <xsl:choose>
      <xsl:when test="contains($Float, 'left') or contains($Float, 'right')">
        
        <w:drawing>
          <wp:anchor distT="0" distB="0" distL="0" distR="0" simplePos="0" relativeHeight="251659264" behindDoc="0" locked="0" layoutInCell="1" allowOverlap="1">
            <wp:simplePos x="0" y="0"/>

            <xsl:if test="@style">

              <xsl:choose>

                <xsl:when test="contains($Float, 'left')">
                  <wp:positionH relativeFrom="column">
                    <wp:align>left</wp:align>
                  </wp:positionH>
                </xsl:when>

                <xsl:when test="contains($Float, 'right')">
                  <wp:positionH relativeFrom="column">
                    <wp:align>right</wp:align>
                  </wp:positionH>
                </xsl:when>

              </xsl:choose>

            </xsl:if>

            <wp:positionV relativeFrom="line">
              <wp:align>
                <xsl:text>top</xsl:text>
              </wp:align>
            </wp:positionV>

            <wp:extent>
              <xsl:attribute name="cx">
                <xsl:value-of select="$width"/>
              </xsl:attribute>
              <xsl:attribute name="cy">
                <xsl:value-of select="$height"/>
              </xsl:attribute>
            </wp:extent>

            <wp:effectExtent l="0" t="0" r="0" b="0"/>
            <wp:wrapSquare wrapText="bothSides"/>
            <wp:docPr>
              <xsl:attribute name="id">
                <xsl:call-template name="getDocPrID"/>
              </xsl:attribute>
              <xsl:attribute name="name">
                <xsl:text>unknown</xsl:text>
              </xsl:attribute>
            </wp:docPr>
            <wp:cNvGraphicFramePr>
              <a:graphicFrameLocks xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main" noChangeAspect="1"/>
            </wp:cNvGraphicFramePr>
            <a:graphic xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">
              <a:graphicData uri="http://schemas.openxmlformats.org/drawingml/2006/picture">
                <pic:pic xmlns:pic="http://schemas.openxmlformats.org/drawingml/2006/picture">
                  <pic:nvPicPr>
                    <pic:cNvPr id="0" name="1"/>
                    <pic:cNvPicPr>
                      <a:picLocks noChangeAspect="1" noChangeArrowheads="1"/>
                    </pic:cNvPicPr>
                  </pic:nvPicPr>
                  <pic:blipFill>
                    <a:blip>
                      <xsl:attribute name="r:embed">
                        <xsl:call-template name="getRefID"/>
                      </xsl:attribute>
                      <xsl:attribute name="cstate">
                        <xsl:text>print</xsl:text>
                      </xsl:attribute>
                    </a:blip>
                    <a:srcRect/>
                    <a:stretch>
                      <a:fillRect>

                        <xsl:if test="$MarginLeft != 0">
                          <xsl:attribute name="l">
                            <xsl:value-of select="format-number($MarginLeft div $RawWidth * 100000, 0)"/>
                          </xsl:attribute>
                        </xsl:if>

                        <xsl:if test="$MarginRight != 0">
                          <xsl:attribute name="r">
                            <xsl:value-of select="format-number($MarginRight div $RawWidth * 100000, 0)"/>
                          </xsl:attribute>
                        </xsl:if>

                        <xsl:if test="$MarginTop != 0">
                          <xsl:attribute name="t">
                            <xsl:value-of select="format-number($MarginTop div $RawHeight * 100000, 0)"/>
                          </xsl:attribute>
                        </xsl:if>

                        <xsl:if test="$MarginBottom != 0">
                          <xsl:attribute name="b">
                            <xsl:value-of select="format-number($MarginBottom div $RawHeight * 100000, 0)"/>
                          </xsl:attribute>
                        </xsl:if>
                      
                      </a:fillRect>
                    </a:stretch>
                  </pic:blipFill>
                  <pic:spPr bwMode="auto">
                    <a:xfrm>
                      <a:off x="0" y="0"/>
                      <a:ext>
                        <xsl:attribute name="cx">
                          <xsl:value-of select="$width"/>
                        </xsl:attribute>
                        <xsl:attribute name="cy">
                          <xsl:value-of select="$height"/>
                        </xsl:attribute>
                      </a:ext>
                    </a:xfrm>
                    <a:prstGeom prst="rect">
                      <a:avLst/>
                    </a:prstGeom>
                    <a:noFill/>
                    <a:ln>
                      <a:noFill/>
                    </a:ln>
                  </pic:spPr>
                </pic:pic>
              </a:graphicData>
            </a:graphic>

          </wp:anchor>
        </w:drawing>

      </xsl:when>
      <xsl:otherwise>


        <w:drawing>
          <wp:inline distT="0" distB="0" distL="0" distR="0">

            <wp:extent>
              <xsl:attribute name="cx">
                <xsl:value-of select="$width"/>
              </xsl:attribute>
              <xsl:attribute name="cy">
                <xsl:value-of select="$height"/>
              </xsl:attribute>
            </wp:extent>
            <wp:effectExtent l="0" t="0" r="0" b="0"/>
            <wp:docPr>
              <xsl:attribute name="id">
                <xsl:call-template name="getDocPrID"/>
              </xsl:attribute>
              <xsl:attribute name="name">
                <xsl:text>unknown</xsl:text>
              </xsl:attribute>
            </wp:docPr>
            <wp:cNvGraphicFramePr>
              <a:graphicFrameLocks xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main" noChangeAspect="1"/>
            </wp:cNvGraphicFramePr>
            <a:graphic xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">
              <a:graphicData uri="http://schemas.openxmlformats.org/drawingml/2006/picture">
                <pic:pic xmlns:pic="http://schemas.openxmlformats.org/drawingml/2006/picture">
                  <pic:nvPicPr>
                    <pic:cNvPr id="0" name="1"/>
                    <pic:cNvPicPr>
                      <a:picLocks noChangeAspect="1" noChangeArrowheads="1"/>
                    </pic:cNvPicPr>
                  </pic:nvPicPr>
                  <pic:blipFill>
                    <a:blip>
                      <xsl:attribute name="r:embed">
                        <xsl:call-template name="getRefID"/>
                      </xsl:attribute>
                      <xsl:attribute name="cstate">
                        <xsl:text>print</xsl:text>
                      </xsl:attribute>
                    </a:blip>
                    <a:srcRect/>
                    <a:stretch>
                      <a:fillRect>

                        <xsl:if test="$MarginLeft != 0">
                          <xsl:attribute name="l">
                            <xsl:value-of select="format-number($MarginLeft div $RawWidth * 100000, 0)"/>
                          </xsl:attribute>
                        </xsl:if>

                        <xsl:if test="$MarginRight != 0">
                          <xsl:attribute name="r">
                            <xsl:value-of select="format-number($MarginRight div $RawWidth * 100000, 0)"/>
                          </xsl:attribute>
                        </xsl:if>

                        <xsl:if test="$MarginTop != 0">
                          <xsl:attribute name="t">
                            <xsl:value-of select="format-number($MarginTop div $RawHeight * 100000, 0)"/>
                          </xsl:attribute>
                        </xsl:if>

                        <xsl:if test="$MarginBottom != 0">
                          <xsl:attribute name="b">
                            <xsl:value-of select="format-number($MarginBottom div $RawHeight * 100000, 0)"/>
                          </xsl:attribute>
                        </xsl:if>

                      </a:fillRect>
                    </a:stretch>
                  </pic:blipFill>
                  <pic:spPr bwMode="auto">
                    <a:xfrm>
                      <a:off x="0" y="0"/>
                      <a:ext>
                        <xsl:attribute name="cx">
                          <xsl:value-of select="$width"/>
                        </xsl:attribute>
                        <xsl:attribute name="cy">
                          <xsl:value-of select="$height"/>
                        </xsl:attribute>
                      </a:ext>
                    </a:xfrm>
                    <a:prstGeom prst="rect">
                      <a:avLst/>
                    </a:prstGeom>
                    <a:noFill/>
                    <a:ln>
                      <a:noFill/>
                    </a:ln>
                  </pic:spPr>
                </pic:pic>
              </a:graphicData>
            </a:graphic>

          </wp:inline>
        </w:drawing>        
        
        <!--<w:pict>

          --><!--Below is some magical spell given us by The Elders.
          We must use it to charm Word's evil spirit.--><!--

          <v:shapetype id="_x0000_t75" coordsize="21600,21600" o:spt="75" o:preferrelative="t" path="m@4@5l@4@11@9@11@9@5xe" filled="f" stroked="f">
            <v:stroke joinstyle="miter"/>
            <v:formulas>
              <v:f eqn="if lineDrawn pixelLineWidth 0"/>
              <v:f eqn="sum @0 1 0"/>
              <v:f eqn="sum 0 0 @1"/>
              <v:f eqn="prod @2 1 2"/>
              <v:f eqn="prod @3 21600 pixelWidth"/>
              <v:f eqn="prod @3 21600 pixelHeight"/>
              <v:f eqn="sum @0 0 1"/>
              <v:f eqn="prod @6 1 2"/>
              <v:f eqn="prod @7 21600 pixelWidth"/>
              <v:f eqn="sum @8 21600 0"/>
              <v:f eqn="prod @7 21600 pixelHeight"/>
              <v:f eqn="sum @10 21600 0"/>
            </v:formulas>
            <v:path o:extrusionok="f" gradientshapeok="t" o:connecttype="rect"/>
            <o:lock v:ext="edit" aspectratio="t"/>
          </v:shapetype>

          --><!--Image data--><!--

          <v:shape id="{ count( preceding::img[ @src ] ) + 1 }" type="#_x0000_t75">

            <xsl:variable name="style">
              <xsl:if test="@style">
                <xsl:value-of select="concat(@style, ';')"/>
              </xsl:if>
            </xsl:variable>
            <xsl:variable name="width">
              <xsl:if test="@width">
                <xsl:text>width:</xsl:text>
                <xsl:value-of select="number(@width) * 3 div 4"/>
                <xsl:text>pt; </xsl:text>
              </xsl:if>
            </xsl:variable>
            <xsl:variable name="height">
              <xsl:if test="@height">
                <xsl:text>height:</xsl:text>
                <xsl:value-of select="number(@height) * 3 div 4"/>
                <xsl:text>pt; </xsl:text>
              </xsl:if>
            </xsl:variable>
            <xsl:attribute name="style">
              <xsl:value-of select="concat($style, $height, $width)"/>
            </xsl:attribute>
            <v:imagedata o:title="">
              <xsl:attribute name="r:id">
                <xsl:call-template name="getRefID"/>
              </xsl:attribute>
            </v:imagedata>
          </v:shape>

        </w:pict>-->
      </xsl:otherwise>
    </xsl:choose>
    
	</xsl:template>

  <xsl:template name="GetImageMargin">
    <xsl:param name="Name"/>
    <xsl:variable name="Temp">
      <xsl:call-template name="GetPropertyFromStyle">
        <xsl:with-param name="styleString" select="@style"/>
        <xsl:with-param name="propertyName" select="$Name"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="Temp1">
      <xsl:choose>
        <xsl:when test="$Temp != '' and not(contains($Temp, 'em'))">
          <xsl:call-template name="ConvertToPixels">
            <xsl:with-param name="length" select="$Temp"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>0px</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="Res">
      <xsl:value-of select="number(substring-before($Temp1, 'px'))"/>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="contains($Res, 'NaN') or $Res = '' ">
        <xsl:text>0</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$Res"/>
      </xsl:otherwise>
    </xsl:choose>
    
  </xsl:template>

	<!-- List  -->
	<!--<xsl:template match="ul | ol" mode="paragraph">
    <xsl:param name="style"/>

    <xsl:call-template name="getList">
      <xsl:with-param name="style" select="$style"/>
    </xsl:call-template>
  </xsl:template>-->

	<xsl:template match="ul | ol" mode="paragraph">
		<xsl:param name="style"/>

		<xsl:variable name="genericStyle">
			<xsl:call-template name="GetElementStyle">
				<xsl:with-param name="tag" select="'ol'"/>
				<xsl:with-param name="class" select="@class"/>
        <xsl:with-param name="ID" select="@id" />
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="number">
			<xsl:call-template name="getListNumId">
				<xsl:with-param name="node" select="."/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="styleWithoutMarginLeft">
			<xsl:call-template name="deleteMarginLeftStyleInList">
				<xsl:with-param name="style" select="$genericStyle"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="attr_style">
			<xsl:call-template name="getParagraphBackGround">
				<xsl:with-param name="style">
					<xsl:value-of select="concat( $style, '; ', $styleWithoutMarginLeft, ';', 'numId:', $number, ';')"/>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:variable>

		<!--<xsl:for-each select="./*">
			<xsl:choose>
				<xsl:when test="name(.) = 'li'">
					<xsl:call-template name="listLevel">
						<xsl:with-param name="style" select="concat( $style, '; ', $styleWithoutMarginLeft)"/>
						<xsl:with-param name="numId" select="concat('numId:', $number, ';')"/>
					</xsl:call-template>
				</xsl:when>
			  <xsl:when test="name(.) = 'ol' or name(.) = 'ul'">
				  <xsl:call-template name="getList">
					  <xsl:with-param name="style" select="concat( $style, '; ', $styleWithoutMarginLeft)"/>
				  </xsl:call-template>
			  </xsl:when>
			  <xsl:otherwise>				  
				  <xsl:for-each select="parent::*">
					  <xsl:value-of select="concat('MAX', '_', name(.))"/>
					  <xsl:apply-templates mode="paragraph">
						  <xsl:with-param name="style" select="concat( $style, '; ', $styleWithoutMarginLeft)"/>
					  </xsl:apply-templates>
				  </xsl:for-each>
			  </xsl:otherwise>
		  </xsl:choose>
	  </xsl:for-each>
		<xsl:apply-templates mode="paragpaph">
      <xsl:with-param name="style" select="concat( $style, '; ', $styleWithoutMarginLeft, ';', 'numId:', $number, ';')"/>
      
    </xsl:apply-templates>-->

		<xsl:apply-templates mode="paragraph">
			<xsl:with-param name="style" select="$attr_style"/>
		</xsl:apply-templates>
	</xsl:template>


	<xsl:template match="li" mode="paragraph">
		<xsl:param name="style"/>

		<xsl:if test="name(parent::*) = 'ol' or name(parent::*) = 'ul'">
			<xsl:variable name="genericStyle">
				<xsl:call-template name="GetElementStyle">
					<xsl:with-param name="tag" select="name()"/>
					<xsl:with-param name="class" select="@class"/>
          <xsl:with-param name="ID" select="@id" />
				</xsl:call-template>
				<xsl:text>;</xsl:text>
			</xsl:variable>

			<xsl:call-template name="listLevel">
				<xsl:with-param name="style" select="concat( $style, ';', $genericStyle, ';', @style)"/>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>


	<!--List Level-->
	<xsl:template name="listLevel">
		<xsl:param name="style"/>

		<xsl:call-template name="CloseParagraph"/>

		<xsl:variable name="genericStyle">
			<xsl:call-template name="GetElementStyle">
				<xsl:with-param name="tag" select="'li'"/>
				<xsl:with-param name="class" select="@class"/>
        <xsl:with-param name="ID" select="@id" />
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="numId">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$style"/>
				<xsl:with-param name="propertyName" select="'numId'"/>
			</xsl:call-template>
		</xsl:variable>
    
    <xsl:variable name="listStyle">
      <xsl:choose>
        <xsl:when test="position()!=last() and contains($style,'margin-bottom:') = false">
          <xsl:value-of select="concat( ';','; contextualSpacing:true')"/>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="styleString" select="concat( $style, ';', $genericStyle, $listStyle)"/>

		<xsl:variable name="styleWithoutMarginLeft">
			<xsl:call-template name="deleteMarginLeftStyleInList">
				<xsl:with-param name="style" select="$styleString"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="level">
			<xsl:call-template name="getListLevel">
				<xsl:with-param name="node" select="."/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="attr_par_style">
			<xsl:call-template name="getParagraphBackGround">
				<xsl:with-param name="style">
					<xsl:value-of select="concat('level:', $level, ';', 'numId:', $numId, ';')"/>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:variable>

		<xsl:call-template name="InsertParagraph">
			<xsl:with-param name="style" select="$styleString"/>
			<xsl:with-param name="ListItemParam" select="$attr_par_style"/>
		</xsl:call-template>

		<xsl:call-template name="OpenParagraph"/>

	</xsl:template>


	<xsl:template name="deleteListPr">
		<xsl:param name="style"/>

		<xsl:choose>
      
			<xsl:when test="contains($style, 'level:')">

        <xsl:variable name="StartString" select="substring-before($style, 'level:')"/>
				<xsl:variable name="EndString" select="substring-after( substring-after($style, $StartString ), ';')"/>
				<xsl:call-template name="deleteListPr">
					<xsl:with-param name="style" select="concat($StartString, $EndString)"/>
				</xsl:call-template>
			</xsl:when>
      
			<xsl:when test="contains($style, 'numId:')">

        <xsl:variable name="StartString" select="substring-before($style, 'numId:')"/>
        <xsl:variable name="EndString" select="substring-after( substring-after($style, $StartString ), ';')"/>
				<xsl:call-template name="deleteListPr">
          <xsl:with-param name="style" select="concat($StartString, $EndString)"/>
        </xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$style"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<xsl:template name="deleteMarginLeftStyleInList">
		<xsl:param name="style"/>

		<xsl:choose>
			<xsl:when test="contains($style, 'margin-left:')">

        <xsl:variable name="StartString" select="substring-before($style, 'margin-left:')"/>
        <xsl:variable name="EndString" select="substring-after( substring-after($style, $StartString ), ';')"/>

        <xsl:value-of select="concat($StartString, $EndString)"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$style"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>



	<!-- Line break  -->
	<xsl:template match="br[ ancestor::a ]" mode="paragraph">
		<w:br/>
	</xsl:template>

  <!-- Line break  -->
  <xsl:template match="br[ not( ancestor::a ) ]" mode="paragraph">
    <xsl:param name="style"/>

    <w:r>

      <!-- Нужно проверить некоторые свойста, возможно данный параграф должeн 
           быть пропущен.     
      -->
      <w:rPr>
        <xsl:call-template name="InsertDisplayProps">
          <xsl:with-param name="styleString" select="$style"/>
        </xsl:call-template>
      </w:rPr>

      <w:br/>

    </w:r>

  </xsl:template>

	<!-- Horizontal line  -->
	<xsl:template match="hr" mode="paragraph">

		<xsl:call-template name="CloseParagraph"/>
		<w:p w:rsidR="004D5F21" w:rsidRPr="00820E0B" w:rsidRDefault="00301D39"  pcut:cut="true">
			<w:pPr>
				<w:ind w:firstLine="0" w:right="0" w:left="0" />
			</w:pPr>
			<w:pict>
				<v:rect id="_x0000_i1025" style="width:0;height:1.5pt" o:hralign="center" o:hrstd="t" o:hr="t" fillcolor="#9d9da1" stroked="f"/>
			</w:pict>
		</w:p>
		<xsl:call-template name="OpenParagraph"/>
	</xsl:template>




</xsl:stylesheet>
