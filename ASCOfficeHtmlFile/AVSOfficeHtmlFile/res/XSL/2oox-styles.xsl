<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
                xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main">

	<xsl:import href="2oox-common.xsl"/>
	<xsl:import href="2oox-styles-common.xsl"/>

	<xsl:output method="xml" encoding="UTF-8"/>

	<xsl:strip-space elements="*"/>

	<xsl:template name="GetOoxStyleName">
		<xsl:param name="htmlStyleName"/>

		<xsl:choose>
			<xsl:when test="$ExtendedLog = 'true'">
				<xsl:message>
					<xsl:value-of select="concat( 'Progress: [GetOoxStyleName], htmlStyleName = ', $htmlStyleName )"/>
				</xsl:message>
			</xsl:when>
			<xsl:otherwise>
				<xsl:message>Progress: [GetOoxStyleName]</xsl:message>
			</xsl:otherwise>
		</xsl:choose>

		<xsl:variable name="style">
			<xsl:call-template name="trim">
				<xsl:with-param name="s" select="$htmlStyleName"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:choose>
			<xsl:when test="$style = 'p'">
				<xsl:text>paragraph</xsl:text>
			</xsl:when>
			<xsl:when test="$style = 'table'">
				<xsl:text>table</xsl:text>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="ParseCssElement">
		<xsl:param name="elementString"/>

		<xsl:choose>
			<xsl:when test="$ExtendedLog = 'true'">
				<xsl:message>
					<xsl:value-of select="concat( 'Progress: [ParseCssElement], elementString = ', $elementString )"/>
				</xsl:message>
			</xsl:when>
			<xsl:otherwise>
				<xsl:message>Progress: [ParseCssElement]</xsl:message>
			</xsl:otherwise>
		</xsl:choose>

		<xsl:variable name="class">
			<xsl:call-template name="trim">
				<xsl:with-param name="s" select="substring-after(substring-before($elementString,'{'),'.')"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:variable name="tag">
			<xsl:choose>
				<xsl:when test="$class != ''">
					<xsl:call-template name="trim">
						<xsl:with-param name="s" select="substring-before($elementString,'.')"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name="trim">
						<xsl:with-param name="s" select="substring-before($elementString,'{')"/>
					</xsl:call-template>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="ooxStyleName">
			<xsl:call-template name="GetOoxStyleName">
				<xsl:with-param name="htmlStyleName" select="$tag"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:if test="$ooxStyleName != ''">
			<w:style>
				<xsl:attribute name="w:type">
					<xsl:value-of select="$ooxStyleName"/>
				</xsl:attribute>
				<xsl:choose>
					<xsl:when test="$class = ''">
						<xsl:attribute name="w:default">
							<xsl:text>1</xsl:text>
						</xsl:attribute>
						<xsl:attribute name="w:styleId">
							<xsl:value-of select="concat( 'default_', $tag )"/>
						</xsl:attribute>
						<w:name w:val="{ concat( 'default_', $tag ) }"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:attribute name="w:customStyle">
							<xsl:text>1</xsl:text>
						</xsl:attribute>
						<xsl:attribute name="w:styleId">
							<xsl:value-of select="concat( 'custom_', $class )"/>
						</xsl:attribute>
						<w:name w:val="{ concat( 'custom_', $class ) }"/>
					</xsl:otherwise>
				</xsl:choose>

				<xsl:choose>
					<xsl:when test="$tag = 'p'">
						<w:pPr>
							<xsl:call-template name="InsertParagraphStyle">
								<xsl:with-param name="styleString" select="substring-after($elementString,'{')"/>
							</xsl:call-template>
						</w:pPr>
						<w:rPr>
							<xsl:call-template name="InsertRunStyle">
								<xsl:with-param name="styleString" select="substring-after($elementString,'{')"/>
							</xsl:call-template>
						</w:rPr>
					</xsl:when>
					<xsl:when test="$tag = 'table'">
						<w:tblPr>
							<xsl:call-template name="InsertTableStyle">
								<xsl:with-param name="styleString" select="substring-after($elementString,'{')"/>
							</xsl:call-template>
						</w:tblPr>
					</xsl:when>
					<xsl:when test="$tag = ''">
						<w:pPr>
							<xsl:call-template name="InsertParagraphStyle">
								<xsl:with-param name="styleString" select="substring-after($elementString,'{')"/>
							</xsl:call-template>
						</w:pPr>
						<w:rPr>
							<xsl:call-template name="InsertRunStyle">
								<xsl:with-param name="styleString" select="substring-after($elementString,'{')"/>
							</xsl:call-template>
						</w:rPr>
						<w:tblPr>
							<xsl:call-template name="InsertTableStyle">
								<xsl:with-param name="styleString" select="substring-after($elementString,'{')"/>
							</xsl:call-template>
						</w:tblPr>
					</xsl:when>
				</xsl:choose>
			</w:style>
		</xsl:if>

	</xsl:template>

	<xsl:template name="ParseCss">
		<xsl:param name="cssString"/>

		<xsl:choose>
			<xsl:when test="$ExtendedLog = 'true'">
				<xsl:message>
					<xsl:value-of select="concat( 'Progress: [ParseCss], cssString = ', $cssString )"/>
				</xsl:message>
			</xsl:when>
			<xsl:otherwise>
				<xsl:message>Progress: [ParseCss]</xsl:message>
			</xsl:otherwise>
		</xsl:choose>

		<xsl:if test="$cssString != ''">
			<xsl:call-template name="ParseCssElement">
				<xsl:with-param name="elementString" select="substring-before( $cssString, '}' )"/>
			</xsl:call-template>
			<xsl:call-template name="ParseCss">
				<xsl:with-param name="cssString" select="substring-after( $cssString, '}' )"/>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="/html">
		<!--AVSMedia - TODO: Stub. Styles should be translated correctly.-->
		<xsl:message>Progress: html</xsl:message>
		<w:styles>
			<w:docDefaults>
				<w:rPrDefault>
					<w:rPr>
						<w:rFonts w:asciiTheme="minorHAnsi" w:eastAsiaTheme="minorEastAsia" w:hAnsiTheme="minorHAnsi" w:cstheme="minorBidi"/>
						<w:sz w:val="22"/>
						<w:szCs w:val="22"/>
						<w:lang w:val="ru-RU" w:eastAsia="zh-TW" w:bidi="ar-SA"/>
					</w:rPr>
				</w:rPrDefault>
				<w:pPrDefault>
					<w:pPr>
						<w:spacing w:after="200" />
						<!--расстояние между параграфами по умолчанию должно быть-->
						<!--<w:spacing w:after="200" w:line="276" w:lineRule="auto"/>-->
					</w:pPr>
				</w:pPrDefault>
			</w:docDefaults>
			<w:latentStyles w:defLockedState="0" w:defUIPriority="99" w:defSemiHidden="1" w:defUnhideWhenUsed="1" w:defQFormat="0" w:count="267">
				<w:lsdException w:name="Normal" w:semiHidden="0" w:uiPriority="0" w:unhideWhenUsed="0" w:qFormat="1"/>
				<w:lsdException w:name="heading 1" w:semiHidden="0" w:uiPriority="9" w:unhideWhenUsed="0" w:qFormat="1"/>
				<w:lsdException w:name="heading 2" w:uiPriority="9" w:qFormat="1"/>
				<w:lsdException w:name="heading 3" w:uiPriority="9" w:qFormat="1"/>
				<w:lsdException w:name="heading 4" w:uiPriority="9" w:qFormat="1"/>
				<w:lsdException w:name="heading 5" w:uiPriority="9" w:qFormat="1"/>
				<w:lsdException w:name="heading 6" w:uiPriority="9" w:qFormat="1"/>
				<w:lsdException w:name="heading 7" w:uiPriority="9" w:qFormat="1"/>
				<w:lsdException w:name="heading 8" w:uiPriority="9" w:qFormat="1"/>
				<w:lsdException w:name="heading 9" w:uiPriority="9" w:qFormat="1"/>
				<w:lsdException w:name="toc 1" w:uiPriority="39"/>
				<w:lsdException w:name="toc 2" w:uiPriority="39"/>
				<w:lsdException w:name="toc 3" w:uiPriority="39"/>
				<w:lsdException w:name="toc 4" w:uiPriority="39"/>
				<w:lsdException w:name="toc 5" w:uiPriority="39"/>
				<w:lsdException w:name="toc 6" w:uiPriority="39"/>
				<w:lsdException w:name="toc 7" w:uiPriority="39"/>
				<w:lsdException w:name="toc 8" w:uiPriority="39"/>
				<w:lsdException w:name="toc 9" w:uiPriority="39"/>
				<w:lsdException w:name="caption" w:uiPriority="35" w:qFormat="1"/>
				<w:lsdException w:name="Title" w:semiHidden="0" w:uiPriority="10" w:unhideWhenUsed="0" w:qFormat="1"/>
				<w:lsdException w:name="Default Paragraph Font" w:uiPriority="1"/>
				<w:lsdException w:name="Subtitle" w:semiHidden="0" w:uiPriority="11" w:unhideWhenUsed="0" w:qFormat="1"/>
				<w:lsdException w:name="Strong" w:semiHidden="0" w:uiPriority="22" w:unhideWhenUsed="0" w:qFormat="1"/>
				<w:lsdException w:name="Emphasis" w:semiHidden="0" w:uiPriority="20" w:unhideWhenUsed="0" w:qFormat="1"/>
				<w:lsdException w:name="Table Grid" w:semiHidden="0" w:uiPriority="59" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Placeholder Text" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="No Spacing" w:semiHidden="0" w:uiPriority="1" w:unhideWhenUsed="0" w:qFormat="1"/>
				<w:lsdException w:name="Light Shading" w:semiHidden="0" w:uiPriority="60" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Light List" w:semiHidden="0" w:uiPriority="61" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Light Grid" w:semiHidden="0" w:uiPriority="62" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Shading 1" w:semiHidden="0" w:uiPriority="63" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Shading 2" w:semiHidden="0" w:uiPriority="64" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium List 1" w:semiHidden="0" w:uiPriority="65" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium List 2" w:semiHidden="0" w:uiPriority="66" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Grid 1" w:semiHidden="0" w:uiPriority="67" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Grid 2" w:semiHidden="0" w:uiPriority="68" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Grid 3" w:semiHidden="0" w:uiPriority="69" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Dark List" w:semiHidden="0" w:uiPriority="70" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Colorful Shading" w:semiHidden="0" w:uiPriority="71" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Colorful List" w:semiHidden="0" w:uiPriority="72" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Colorful Grid" w:semiHidden="0" w:uiPriority="73" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Light Shading Accent 1" w:semiHidden="0" w:uiPriority="60" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Light List Accent 1" w:semiHidden="0" w:uiPriority="61" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Light Grid Accent 1" w:semiHidden="0" w:uiPriority="62" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Shading 1 Accent 1" w:semiHidden="0" w:uiPriority="63" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Shading 2 Accent 1" w:semiHidden="0" w:uiPriority="64" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium List 1 Accent 1" w:semiHidden="0" w:uiPriority="65" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Revision" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="List Paragraph" w:semiHidden="0" w:uiPriority="34" w:unhideWhenUsed="0" w:qFormat="1"/>
				<w:lsdException w:name="Quote" w:semiHidden="0" w:uiPriority="29" w:unhideWhenUsed="0" w:qFormat="1"/>
				<w:lsdException w:name="Intense Quote" w:semiHidden="0" w:uiPriority="30" w:unhideWhenUsed="0" w:qFormat="1"/>
				<w:lsdException w:name="Medium List 2 Accent 1" w:semiHidden="0" w:uiPriority="66" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Grid 1 Accent 1" w:semiHidden="0" w:uiPriority="67" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Grid 2 Accent 1" w:semiHidden="0" w:uiPriority="68" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Grid 3 Accent 1" w:semiHidden="0" w:uiPriority="69" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Dark List Accent 1" w:semiHidden="0" w:uiPriority="70" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Colorful Shading Accent 1" w:semiHidden="0" w:uiPriority="71" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Colorful List Accent 1" w:semiHidden="0" w:uiPriority="72" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Colorful Grid Accent 1" w:semiHidden="0" w:uiPriority="73" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Light Shading Accent 2" w:semiHidden="0" w:uiPriority="60" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Light List Accent 2" w:semiHidden="0" w:uiPriority="61" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Light Grid Accent 2" w:semiHidden="0" w:uiPriority="62" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Shading 1 Accent 2" w:semiHidden="0" w:uiPriority="63" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Shading 2 Accent 2" w:semiHidden="0" w:uiPriority="64" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium List 1 Accent 2" w:semiHidden="0" w:uiPriority="65" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium List 2 Accent 2" w:semiHidden="0" w:uiPriority="66" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Grid 1 Accent 2" w:semiHidden="0" w:uiPriority="67" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Grid 2 Accent 2" w:semiHidden="0" w:uiPriority="68" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Grid 3 Accent 2" w:semiHidden="0" w:uiPriority="69" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Dark List Accent 2" w:semiHidden="0" w:uiPriority="70" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Colorful Shading Accent 2" w:semiHidden="0" w:uiPriority="71" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Colorful List Accent 2" w:semiHidden="0" w:uiPriority="72" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Colorful Grid Accent 2" w:semiHidden="0" w:uiPriority="73" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Light Shading Accent 3" w:semiHidden="0" w:uiPriority="60" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Light List Accent 3" w:semiHidden="0" w:uiPriority="61" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Light Grid Accent 3" w:semiHidden="0" w:uiPriority="62" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Shading 1 Accent 3" w:semiHidden="0" w:uiPriority="63" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Shading 2 Accent 3" w:semiHidden="0" w:uiPriority="64" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium List 1 Accent 3" w:semiHidden="0" w:uiPriority="65" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium List 2 Accent 3" w:semiHidden="0" w:uiPriority="66" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Grid 1 Accent 3" w:semiHidden="0" w:uiPriority="67" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Grid 2 Accent 3" w:semiHidden="0" w:uiPriority="68" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Grid 3 Accent 3" w:semiHidden="0" w:uiPriority="69" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Dark List Accent 3" w:semiHidden="0" w:uiPriority="70" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Colorful Shading Accent 3" w:semiHidden="0" w:uiPriority="71" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Colorful List Accent 3" w:semiHidden="0" w:uiPriority="72" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Colorful Grid Accent 3" w:semiHidden="0" w:uiPriority="73" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Light Shading Accent 4" w:semiHidden="0" w:uiPriority="60" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Light List Accent 4" w:semiHidden="0" w:uiPriority="61" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Light Grid Accent 4" w:semiHidden="0" w:uiPriority="62" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Shading 1 Accent 4" w:semiHidden="0" w:uiPriority="63" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Shading 2 Accent 4" w:semiHidden="0" w:uiPriority="64" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium List 1 Accent 4" w:semiHidden="0" w:uiPriority="65" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium List 2 Accent 4" w:semiHidden="0" w:uiPriority="66" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Grid 1 Accent 4" w:semiHidden="0" w:uiPriority="67" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Grid 2 Accent 4" w:semiHidden="0" w:uiPriority="68" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Grid 3 Accent 4" w:semiHidden="0" w:uiPriority="69" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Dark List Accent 4" w:semiHidden="0" w:uiPriority="70" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Colorful Shading Accent 4" w:semiHidden="0" w:uiPriority="71" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Colorful List Accent 4" w:semiHidden="0" w:uiPriority="72" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Colorful Grid Accent 4" w:semiHidden="0" w:uiPriority="73" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Light Shading Accent 5" w:semiHidden="0" w:uiPriority="60" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Light List Accent 5" w:semiHidden="0" w:uiPriority="61" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Light Grid Accent 5" w:semiHidden="0" w:uiPriority="62" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Shading 1 Accent 5" w:semiHidden="0" w:uiPriority="63" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Shading 2 Accent 5" w:semiHidden="0" w:uiPriority="64" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium List 1 Accent 5" w:semiHidden="0" w:uiPriority="65" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium List 2 Accent 5" w:semiHidden="0" w:uiPriority="66" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Grid 1 Accent 5" w:semiHidden="0" w:uiPriority="67" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Grid 2 Accent 5" w:semiHidden="0" w:uiPriority="68" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Grid 3 Accent 5" w:semiHidden="0" w:uiPriority="69" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Dark List Accent 5" w:semiHidden="0" w:uiPriority="70" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Colorful Shading Accent 5" w:semiHidden="0" w:uiPriority="71" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Colorful List Accent 5" w:semiHidden="0" w:uiPriority="72" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Colorful Grid Accent 5" w:semiHidden="0" w:uiPriority="73" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Light Shading Accent 6" w:semiHidden="0" w:uiPriority="60" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Light List Accent 6" w:semiHidden="0" w:uiPriority="61" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Light Grid Accent 6" w:semiHidden="0" w:uiPriority="62" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Shading 1 Accent 6" w:semiHidden="0" w:uiPriority="63" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Shading 2 Accent 6" w:semiHidden="0" w:uiPriority="64" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium List 1 Accent 6" w:semiHidden="0" w:uiPriority="65" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium List 2 Accent 6" w:semiHidden="0" w:uiPriority="66" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Grid 1 Accent 6" w:semiHidden="0" w:uiPriority="67" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Grid 2 Accent 6" w:semiHidden="0" w:uiPriority="68" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Medium Grid 3 Accent 6" w:semiHidden="0" w:uiPriority="69" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Dark List Accent 6" w:semiHidden="0" w:uiPriority="70" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Colorful Shading Accent 6" w:semiHidden="0" w:uiPriority="71" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Colorful List Accent 6" w:semiHidden="0" w:uiPriority="72" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Colorful Grid Accent 6" w:semiHidden="0" w:uiPriority="73" w:unhideWhenUsed="0"/>
				<w:lsdException w:name="Subtle Emphasis" w:semiHidden="0" w:uiPriority="19" w:unhideWhenUsed="0" w:qFormat="1"/>
				<w:lsdException w:name="Intense Emphasis" w:semiHidden="0" w:uiPriority="21" w:unhideWhenUsed="0" w:qFormat="1"/>
				<w:lsdException w:name="Subtle Reference" w:semiHidden="0" w:uiPriority="31" w:unhideWhenUsed="0" w:qFormat="1"/>
				<w:lsdException w:name="Intense Reference" w:semiHidden="0" w:uiPriority="32" w:unhideWhenUsed="0" w:qFormat="1"/>
				<w:lsdException w:name="Book Title" w:semiHidden="0" w:uiPriority="33" w:unhideWhenUsed="0" w:qFormat="1"/>
				<w:lsdException w:name="Bibliography" w:uiPriority="37"/>
				<w:lsdException w:name="TOC Heading" w:uiPriority="39" w:qFormat="1"/>
			</w:latentStyles>
			<w:style w:type="paragraph" w:default="1" w:styleId="a">
				<w:name w:val="Normal"/>
				<w:qFormat/>
				<w:rsid w:val="00753386"/>
			</w:style>
			<w:style w:type="character" w:default="1" w:styleId="a0">
				<w:name w:val="Default Paragraph Font"/>
				<w:uiPriority w:val="1"/>
				<w:semiHidden/>
				<w:unhideWhenUsed/>
			</w:style>
			<w:style w:type="table" w:default="1" w:styleId="a1">
				<w:name w:val="Normal Table"/>
				<w:uiPriority w:val="99"/>
				<w:semiHidden/>
				<w:unhideWhenUsed/>
				<w:qFormat/>
				<w:tblPr>
					<w:tblInd w:w="0" w:type="dxa"/>
					<w:tblCellMar>
						<w:top w:w="0" w:type="dxa"/>
						<w:left w:w="108" w:type="dxa"/>
						<w:bottom w:w="0" w:type="dxa"/>
						<w:right w:w="108" w:type="dxa"/>
					</w:tblCellMar>
				</w:tblPr>
			</w:style>
			<w:style w:type="numbering" w:default="1" w:styleId="a2">
				<w:name w:val="No List"/>
				<w:uiPriority w:val="99"/>
				<w:semiHidden/>
				<w:unhideWhenUsed/>
			</w:style>
      <w:style w:type="paragraph" w:styleId="Heading1">
        <w:name w:val="heading 1"/>
        <w:basedOn w:val="a"/>
        <w:next w:val="a"/>
        <w:qFormat/>
        <w:rsid w:val="00EF7B96"/>
        <w:pPr>
          <w:spacing w:before="240" w:after="60"/>
          <w:outlineLvl w:val="0"/>
        </w:pPr>
        <w:rPr>
          <w:rFonts w:ascii="Arial" w:eastAsia="Arial" w:hAnsi="Arial" w:cs="Arial"/>
          <w:b/>
          <w:bCs/>
          <w:sz w:val="32"/>
          <w:szCs w:val="32"/>
        </w:rPr>
      </w:style>
      <w:style w:type="paragraph" w:styleId="Heading2">
        <w:name w:val="heading 2"/>
        <w:basedOn w:val="a"/>
        <w:next w:val="a"/>
        <w:qFormat/>
        <w:rsid w:val="00EF7B96"/>
        <w:pPr>
          <w:spacing w:before="240" w:after="60"/>
          <w:outlineLvl w:val="1"/>
        </w:pPr>
        <w:rPr>
          <w:rFonts w:ascii="Arial" w:eastAsia="Arial" w:hAnsi="Arial" w:cs="Arial"/>
          <w:b/>
          <w:bCs/>
          <w:i/>
          <w:iCs/>
          <w:sz w:val="28"/>
          <w:szCs w:val="28"/>
        </w:rPr>
      </w:style>
      <w:style w:type="paragraph" w:styleId="Heading3">
        <w:name w:val="heading 3"/>
        <w:basedOn w:val="a"/>
        <w:next w:val="a"/>
        <w:qFormat/>
        <w:rsid w:val="00EF7B96"/>
        <w:pPr>
          <w:spacing w:before="240" w:after="60"/>
          <w:outlineLvl w:val="2"/>
        </w:pPr>
        <w:rPr>
          <w:rFonts w:ascii="Arial" w:eastAsia="Arial" w:hAnsi="Arial" w:cs="Arial"/>
          <w:b/>
          <w:bCs/>
          <w:sz w:val="26"/>
          <w:szCs w:val="26"/>
        </w:rPr>
      </w:style>
      <w:style w:type="paragraph" w:styleId="Heading4">
        <w:name w:val="heading 4"/>
        <w:basedOn w:val="a"/>
        <w:next w:val="a"/>
        <w:qFormat/>
        <w:rsid w:val="00EF7B96"/>
        <w:pPr>
          <w:spacing w:before="240" w:after="60"/>
          <w:outlineLvl w:val="3"/>
        </w:pPr>
        <w:rPr>
          <w:b/>
          <w:bCs/>
          <w:sz w:val="28"/>
          <w:szCs w:val="28"/>
        </w:rPr>
      </w:style>
      <w:style w:type="paragraph" w:styleId="Heading5">
        <w:name w:val="heading 5"/>
        <w:basedOn w:val="a"/>
        <w:next w:val="a"/>
        <w:qFormat/>
        <w:rsid w:val="00EF7B96"/>
        <w:pPr>
          <w:spacing w:before="240" w:after="60"/>
          <w:outlineLvl w:val="4"/>
        </w:pPr>
        <w:rPr>
          <w:b/>
          <w:bCs/>
          <w:i/>
          <w:iCs/>
          <w:sz w:val="26"/>
          <w:szCs w:val="26"/>
        </w:rPr>
      </w:style>
      <w:style w:type="paragraph" w:styleId="Heading6">
        <w:name w:val="heading 6"/>
        <w:basedOn w:val="a"/>
        <w:next w:val="a"/>
        <w:qFormat/>
        <w:rsid w:val="00EF7B96"/>
        <w:pPr>
          <w:spacing w:before="240" w:after="60"/>
          <w:outlineLvl w:val="5"/>
        </w:pPr>
        <w:rPr>
          <w:b/>
          <w:bCs/>
          <w:sz w:val="22"/>
          <w:szCs w:val="22"/>
        </w:rPr>
      </w:style>
			<xsl:apply-templates/>
		</w:styles>
	</xsl:template>

	<xsl:template match="head">
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="style">
		<xsl:message>Progress: style</xsl:message>
		<xsl:call-template name="ParseCss">
			<xsl:with-param name="cssString" select="text()"/>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match="body"/>

	<xsl:template match="text()"/>

</xsl:stylesheet>
