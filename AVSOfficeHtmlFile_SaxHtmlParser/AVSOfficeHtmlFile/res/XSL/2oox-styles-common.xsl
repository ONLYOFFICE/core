<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
                xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"
                xmlns:w14="http://schemas.microsoft.com/office/word/2010/wordml">

	<xsl:import href="2oox-common.xsl"/>
	<xsl:import href="2oox-measures.xsl"/>

	<xsl:output method="xml"/>
	<!-- encoding="UTF-8"/>-->


	<xsl:template name="setMarginLeft">
		<xsl:param name="node" select="."/>
		<xsl:param name="style"/>


		<xsl:variable name="marginLeft">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$style"/>
				<xsl:with-param name="propertyName" select="'margin-left'"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="marginLeftInPx">
			<xsl:choose>
				<xsl:when test="$marginLeft != '' and not(contains($marginLeft, 'em'))">
					<xsl:call-template name="ConvertToPixels">
						<xsl:with-param name="length" select="$marginLeft"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>0px</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:for-each select="$node">
			<xsl:if test="(count(ancestor::ul) != 0 or count(ancestor::ol) != 0)  and $marginLeftInPx != ''">
				<xsl:variable name="listLevel" select="count(ancestor::ul)+count(ancestor::ol)"/>

				<xsl:value-of select="concat('margin-left:', floor(number(substring-before($marginLeftInPx, 'px')) + (number($listLevel)) * 1440 div 1134 div 0.0264), 'px;')"/>
				<!--<xsl:value-of select="concat('hanging:', '360;')"/>-->
			</xsl:if>
		</xsl:for-each>

	</xsl:template>

	<xsl:template name="InsertParagraphStyle">
		<xsl:param name="styleString"/>
		<xsl:param name="node" select="."/>

    <xsl:call-template name="InsertDisplayProps">
      <xsl:with-param name="styleString" select="$styleString"/>
    </xsl:call-template>

    <xsl:call-template name="InsertTextAlign">
			<xsl:with-param name="styleString" select="$styleString"/>
		</xsl:call-template>

		<xsl:variable name="marginLeft">
			<xsl:call-template name="setMarginLeft">
				<xsl:with-param name="node" select="."/>
				<xsl:with-param name="style" select="$styleString"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:call-template name="InsertTextMargins">
			<xsl:with-param name="styleString" select="concat($styleString, ';', $marginLeft)"/>
		</xsl:call-template>

		<xsl:call-template name="InsertNumberingProperty">
			<xsl:with-param name="styleString" select="$styleString"/>
		</xsl:call-template>

		<xsl:call-template name="InsertParagraphBackGroundColor">
			<xsl:with-param name="styleString" select="$styleString"/>
		</xsl:call-template>

    <xsl:call-template name="InsertParaEffects">
      <xsl:with-param name="styleString" select="$styleString"/>
    </xsl:call-template>

    <xsl:call-template name="InsertPageBreakBefore">
      <xsl:with-param name="styleString" select="$styleString"/>
    </xsl:call-template>
    
  </xsl:template>


	<xsl:template name="InsertRunStyle">
		<xsl:param name="styleString"/>

    <xsl:call-template name="InsertDisplayProps">
      <xsl:with-param name="styleString" select="$styleString"/>
    </xsl:call-template>

    <xsl:call-template name="InsertFont">
			<xsl:with-param name="styleString" select="$styleString"/>
		</xsl:call-template>

		<xsl:call-template name="InsertTextAlign">
			<xsl:with-param name="styleString" select="$styleString"/>
		</xsl:call-template>

		<xsl:call-template name="InsertColor">
			<xsl:with-param name="styleString" select="$styleString"/>
		</xsl:call-template>

		<xsl:call-template name="InsertBackgoundColor">
			<xsl:with-param name="styleString" select="$styleString"/>
		</xsl:call-template>

		<xsl:call-template name="InsertTextDecoration">
			<xsl:with-param name="styleString" select="$styleString"/>
		</xsl:call-template>

		<xsl:call-template name="InsertLettersSpacing">
			<xsl:with-param name="styleString" select="$styleString"/>
		</xsl:call-template>

		<xsl:call-template name="InsertVerticalAlign">
			<xsl:with-param name="styleString" select="$styleString"/>
		</xsl:call-template>

    <xsl:call-template name="InsertParaEffects">
      <xsl:with-param name="styleString" select="$styleString"/>
    </xsl:call-template>
    
	</xsl:template>


	<xsl:template name="InsertTableStyle">
		<xsl:param name="styleString"/>

		<w:tblLook w:val="04A0"/>

		<xsl:call-template name="InsertTableWidth">
			<xsl:with-param name="styleString" select="$styleString"/>
		</xsl:call-template>

		<xsl:call-template name="InsertBorderCollapse">
			<xsl:with-param name="styleString" select="$styleString"/>
		</xsl:call-template>

		<xsl:call-template name="InsertTextAlign">
			<xsl:with-param name="styleString" select="$styleString"/>
		</xsl:call-template>

		<w:tblBorders>
			<xsl:call-template name="InsertBorders">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="flag">true</xsl:with-param>
			</xsl:call-template>
		</w:tblBorders>

	</xsl:template>


	<xsl:template name="InsertTcStyle">
		<xsl:param name="styleString"/>

		<xsl:call-template name="InsertCellWidth">
			<xsl:with-param name="styleString" select="$styleString"/>
		</xsl:call-template>

		<xsl:call-template name="InsertTextAlign">
			<xsl:with-param name="styleString" select="$styleString"/>
		</xsl:call-template>

		<xsl:call-template name="InsertVerticalTextAlignInTc">
			<xsl:with-param name="styleString" select="$styleString"/>
		</xsl:call-template>

		<xsl:call-template name="InsertColSpan">
			<xsl:with-param name="styleString" select="$styleString"/>
		</xsl:call-template>

		<xsl:call-template name="InsertRowSpan">
			<xsl:with-param name="styleString" select="$styleString"/>
		</xsl:call-template>

		<xsl:call-template name="InsertTableBackGroundColor">
			<xsl:with-param name="styleString" select="$styleString"/>
		</xsl:call-template>

		<w:tcBorders>
			<xsl:call-template name="InsertBorders">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="flag">false</xsl:with-param>
			</xsl:call-template>
		</w:tcBorders>
	</xsl:template>


	<xsl:template name="InsertTrStyle">
		<xsl:param name="styleString"/>
		<w:trPr>
			<xsl:call-template name="InsertRowHeight">
				<xsl:with-param name="styleString" select="$styleString"/>
			</xsl:call-template>
		</w:trPr>
	</xsl:template>


	<xsl:template name="InsertRowHeight">
		<xsl:param name="styleString"/>

		<xsl:variable name="RowHeight">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="'height'"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:if test="$RowHeight">
			<w:trHeight>
				<xsl:attribute name="w:val">
					<xsl:call-template name="ConvertMeasure">
						<xsl:with-param name="length" select="$RowHeight"/>
						<xsl:with-param name="unit" select="'twips'"/>
					</xsl:call-template>
				</xsl:attribute>
			</w:trHeight>
		</xsl:if>

	</xsl:template>


	<xsl:template name="InsertTableWidth">
		<xsl:param name="styleString"/>

		<w:tblW>
			<xsl:call-template name="InsertWidthAttr">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="isTable" select="'true'"/>
			</xsl:call-template>
		</w:tblW>

	</xsl:template>


	<xsl:template name="InsertParagraphBackGroundColor">
		<xsl:param name="styleString"/>

		<xsl:variable name="paragraphColor">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="'paragraphBGcolor'"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:if test="$paragraphColor != '' and $paragraphColor != 'transparent' and $paragraphColor != 'inherit'">
			<xsl:variable name="converted">
				<xsl:call-template name="ConvertColor">
					<xsl:with-param name="color" select="$paragraphColor"/>
				</xsl:call-template>
			</xsl:variable>

			<xsl:if test="$converted != ''">
				<w:shd w:val="clear" w:color="auto" w:fill="{ $converted }"/>
			</xsl:if>
			<!--<w:highlight w:val="{ $converted }"/>-->
		</xsl:if>
	</xsl:template>


	<xsl:template name="InsertTableBackGroundColor">
		<xsl:param name="styleString"/>

		<xsl:variable name="tableColor">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="'tableBGcolor'"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:if test="$tableColor != '' and $tableColor != 'transparent' and $tableColor != 'inherit'">
			<xsl:variable name="converted">
				<xsl:call-template name="ConvertColor">
					<xsl:with-param name="color" select="$tableColor"/>
				</xsl:call-template>
			</xsl:variable>

			<xsl:if test="$converted != ''">
				<w:shd w:val="clear" w:color="auto" w:fill="{ $converted }"/>
			</xsl:if>
		</xsl:if>
	</xsl:template>


	<xsl:template name="InsertColSpan">
		<xsl:param name="styleString"/>

		<xsl:variable name="colSpan">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="'colspan'"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:if test="$colSpan != ''">
			<w:gridSpan w:val="{ $colSpan }"/>
		</xsl:if>
	</xsl:template>


	<xsl:template name="InsertRowSpan">
		<xsl:param name="styleString"/>

		<xsl:variable name="rowSpan">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="'rowspan'"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:if test="$rowSpan != ''">
			<w:vMerge w:val="restart" w:count_rows_in_span ="{ $rowSpan }"/>
		</xsl:if>
	</xsl:template>


	<xsl:template name="InsertCellWidth">
		<xsl:param name="styleString"/>
		<w:tcW>
			<xsl:call-template name="InsertWidthAttr">
				<xsl:with-param name="styleString" select="$styleString"/>
			</xsl:call-template>
		</w:tcW>
	</xsl:template>


	<xsl:template name="InsertWidthAttr">
		<xsl:param name="styleString"/>
		<xsl:param name="isTable" select="'false'"/>

		<xsl:variable name="width">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="'width'"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="twip_val">
			<xsl:call-template name="ConvertMeasure">
				<xsl:with-param name="length" select="$width"/>
				<xsl:with-param name="unit" select="'twips'"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="perc_val">
			<xsl:value-of select="substring-before( $width, '%' )"/>
		</xsl:variable>

		<xsl:choose>
			<xsl:when test="( $perc_val != '0' ) and ( $perc_val != '' ) and ( $perc_val != 'NaN' ) and $perc_val != NaN">

				<xsl:attribute name="w:w">
					<xsl:value-of select="$perc_val * 50"/>
				</xsl:attribute>
				<xsl:attribute name="w:type">
					<xsl:value-of select="'pct'"/>
				</xsl:attribute>
			</xsl:when>
			<xsl:when test="( number( $twip_val ) != '0' ) and ( $twip_val != '' ) and $twip_val != 'NaN' and $twip_val != NaN ">
				<xsl:attribute name="w:w">
					<xsl:value-of select="number( $twip_val )"/>
				</xsl:attribute>
				<xsl:attribute name="w:type">
					<xsl:value-of select="'dxa'"/>
				</xsl:attribute>
			</xsl:when>
			<xsl:when test="$width != '' and number($width) != 'NaN' and $width != 'auto' and $width != NaN">
				<xsl:attribute name="w:w">
					<xsl:value-of select="number($width) * 5"/>
				</xsl:attribute>
				<xsl:attribute name="w:type">
					<xsl:value-of select="'pct'"/>
				</xsl:attribute>
			</xsl:when>
			<xsl:otherwise>
				<xsl:attribute name="w:w">
					<xsl:value-of select="'0'"/>
				</xsl:attribute>
				<xsl:attribute name="w:type">
					<xsl:value-of select="'auto'"/>
				</xsl:attribute>
			</xsl:otherwise>
		</xsl:choose>

	</xsl:template>


	<xsl:template name="InsertLettersSpacing">
		<xsl:param name="styleString"/>

		<xsl:variable name="LettersSpacing">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="'letter-spacing'"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:if test="$LettersSpacing != ''">
			<w:spacing>
				<xsl:attribute name="w:val">
					<xsl:choose>
						<xsl:when test="contains($LettersSpacing, 'pt')">
							<xsl:call-template name="ConvertMeasure">
								<xsl:with-param name="length" select="$LettersSpacing"/>
								<xsl:with-param name="unit" select="'point'"/>
							</xsl:call-template>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>0</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>
			</w:spacing>
		</xsl:if>
	</xsl:template>


	<xsl:template name="InsertVerticalAlign">
		<xsl:param name="styleString"/>

		<xsl:variable name="VerticalAlign">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="'vertical-align'"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:if test="$VerticalAlign != ''">
			<w:vertAlign>
				<xsl:attribute name="w:val">
					<xsl:choose>
						<xsl:when test="$VerticalAlign = 'super'">
							<xsl:text>superscript</xsl:text>
						</xsl:when>
						<xsl:when test="$VerticalAlign = 'sub'">
							<xsl:text>subscript</xsl:text>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>normal</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>
			</w:vertAlign>

		</xsl:if>
	</xsl:template>


	<xsl:template name="getFontSize">
		<xsl:param name="style"/>
    <xsl:param name="isDefaultSize"/>

		<xsl:variable name="size">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$style"/>
				<xsl:with-param name="propertyName" select="'font-size'"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="contains($size, 'em')">
				<xsl:variable name="s">
					<xsl:call-template name="getFontSize">
						<xsl:with-param name="style" select="concat(substring-before($style, concat('font-size: ', $size, ';')), substring-after($style, concat('font-size: ', $size, ';')))"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:value-of select="round(number( substring-before($size, 'em')) * number($s))"/>
			</xsl:when>
			<xsl:when test="$size != ''">
				<xsl:choose>
					<xsl:when test="starts-with( $size, '#' )">
						<!-- HTMLayout specific -->
						<xsl:call-template name="ConvertFontSize">
							<xsl:with-param name="size" select="substring-after( $size, '#' )"/>
						</xsl:call-template>
					</xsl:when>
					<xsl:when test="contains($size, '%')">
						<xsl:value-of  select="substring-before($size, '%') * 24 div 100"/>

						<!--<xsl:choose>
							<xsl:when test="number(substring-before($size, '%')) != NaN and number(substring-before($size, '%')) != 'NaN'">
								<xsl:value-of  select="substring-before($size, '%') * 24"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:text>24</xsl:text>
							</xsl:otherwise>
						</xsl:choose>		-->
					</xsl:when>
					<xsl:when test="$size != ''">
						<xsl:call-template name="ConvertFontSize">
							<xsl:with-param name="size" select="$size"/>
              <xsl:with-param name="isDefaultSize" select="$isDefaultSize"/>
            </xsl:call-template>
					</xsl:when>
				</xsl:choose>
			</xsl:when>
      <xsl:when test="$isDefaultSize != ''">
        <xsl:text>12</xsl:text>
      </xsl:when>
      <xsl:otherwise>
				<xsl:text>24</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<xsl:template name="InsertFont">
		<xsl:param name="styleString"/>
		<xsl:variable name="font">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="'font'"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:variable name="fontStyle">
			<xsl:choose>
				<xsl:when test="$font = ''">
					<xsl:call-template name="GetPropertyFromStyle">
						<xsl:with-param name="styleString" select="$styleString"/>
						<xsl:with-param name="propertyName" select="'font-style'"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name="GetCompositePropertyPart">
						<xsl:with-param name="property" select="$font"/>
						<xsl:with-param name="pos" select="0"/>
					</xsl:call-template>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="fontVariant">
			<xsl:choose>
				<xsl:when test="$font = ''">
					<xsl:call-template name="GetPropertyFromStyle">
						<xsl:with-param name="styleString" select="$styleString"/>
						<xsl:with-param name="propertyName" select="'font-variant'"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name="GetCompositePropertyPart">
						<xsl:with-param name="property" select="$font"/>
						<xsl:with-param name="pos" select="1"/>
					</xsl:call-template>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="fontWeight">
			<xsl:choose>
				<xsl:when test="$font = ''">
					<xsl:call-template name="GetPropertyFromStyle">
						<xsl:with-param name="styleString" select="$styleString"/>
						<xsl:with-param name="propertyName" select="'font-weight'"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name="GetCompositePropertyPart">
						<xsl:with-param name="property" select="$font"/>
						<xsl:with-param name="pos" select="2"/>
					</xsl:call-template>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="fontSize">
			<xsl:call-template name="getFontSize">
				<xsl:with-param name="style" select="$styleString"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:variable name="fontFamily">
			<xsl:choose>
				<xsl:when test="contains($styleString, 'font-family')">
					<xsl:call-template name="GetPropertyFromStyle">
						<xsl:with-param name="styleString" select="$styleString"/>
						<xsl:with-param name="propertyName" select="'font-family'"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="$font != ''">
					<xsl:call-template name="GetCompositePropertyPart">
						<xsl:with-param name="property" select="$font"/>
						<xsl:with-param name="pos" select="4"/>
					</xsl:call-template>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<xsl:choose>
			<xsl:when test="$fontStyle = 'italic'">
				<w:i/>
			</xsl:when>
		</xsl:choose>
		<xsl:choose>
			<xsl:when test="$fontWeight = 'bold'">
				<w:b/>
			</xsl:when>
			<!-- Stub: actualy, bolder is a relative weight value -->
			<xsl:when test="$fontWeight = 'bolder'">
				<w:b/>
			</xsl:when>
			<xsl:when test="number( $fontWeight > 700 )">
				<w:b/>
			</xsl:when>
		</xsl:choose>
		<xsl:if test="$fontSize != '' and $fontSize != 'NaN' and number($fontSize) != 'NaN'">
			<w:sz w:val="{ $fontSize }"/>
			<w:szCs w:val="{ $fontSize }"/>
		</xsl:if>
		<xsl:if test="$fontFamily != ''">
			<xsl:variable name="one-font">
				<xsl:choose>
					<xsl:when test="contains($fontFamily, ',')">
						<xsl:value-of select="substring-before($fontFamily, ',')"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="$fontFamily"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<xsl:variable name="ap">
				<xsl:text>&#39;</xsl:text>
			</xsl:variable>
			<xsl:variable name="FontName">
				<xsl:choose>
					<xsl:when test="contains($one-font, $ap)">
						<xsl:value-of select="substring-before(substring-after($one-font, $ap), $ap)"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="$one-font"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<w:rFonts>
				<xsl:attribute name="w:ascii">
					<xsl:value-of select="$FontName"/>
				</xsl:attribute>
				<xsl:attribute name="w:eastAsia">
					<xsl:value-of select="$FontName"/>
				</xsl:attribute>
				<xsl:attribute name="w:hAnsi">
					<xsl:value-of select="$FontName"/>
				</xsl:attribute>
				<xsl:attribute name="w:cs">
					<xsl:value-of select="$FontName"/>
				</xsl:attribute>
			</w:rFonts>
		</xsl:if>

	</xsl:template>

	<xsl:template name="ConvertFontSize">
		<xsl:param name="size"/>
    <xsl:param name="isDefaultSize"/>
    
    <xsl:variable name="numeric">
			<xsl:choose>
				<xsl:when test="contains($size, 'pt')">
					<xsl:call-template name="ConvertMeasure">
						<xsl:with-param name="length" select="$size"/>
						<xsl:with-param name="unit" select="'point'"/>
					</xsl:call-template>
				</xsl:when>
        <xsl:when test="contains($size, 'px')">
          <xsl:call-template name="ConvertMeasure">
            <xsl:with-param name="length" select="$size"/>
            <xsl:with-param name="unit" select="'point'"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:when test="contains($size, 'em')">
					<xsl:value-of select="concat(number(substring-before($size, 'em')) * 100, '%')"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<xsl:choose>
			<xsl:when test="$size = 'xx-small'">
				<xsl:text>12</xsl:text>
			</xsl:when>
			<xsl:when test="$size = 'x-small'">
				<xsl:text>16</xsl:text>
			</xsl:when>
			<xsl:when test="$size = 'small'">
				<xsl:text>20</xsl:text>
			</xsl:when>
			<xsl:when test="$size = 'smaller'">
				<!-- Stub: actualy, smaller is a relative size value -->
				<xsl:text>20</xsl:text>
			</xsl:when>
			<xsl:when test="$size = 'medium'">
				<xsl:text>24</xsl:text>
			</xsl:when>
			<xsl:when test="$size = 'large'">
				<xsl:text>32</xsl:text>
			</xsl:when>
			<xsl:when test="$size = 'larger'">
				<!-- Stub: actualy, larger is a relative size value -->
				<xsl:text>32</xsl:text>
			</xsl:when>
			<xsl:when test="$size = 'x-large'">
				<xsl:text>40</xsl:text>
			</xsl:when>
			<xsl:when test="$size = 'xx-large'">
				<xsl:text>48</xsl:text>
			</xsl:when>
			<xsl:when test="contains( $numeric, '%' )">
				<xsl:value-of select="round( ( number( substring-before( $numeric, '%' ) ) * 24 ) div 100 )"/>
			</xsl:when>
			<xsl:when test="contains( $size, '%' )">
				<!-- Stub: use 12pt font as normal -->
				<xsl:value-of select="round( ( number( substring-before( $size, '%' ) ) * 24 ) div 100 )"/>
			</xsl:when>
      <xsl:when test="$isDefaultSize != ''">
        <xsl:value-of select="$numeric"/>
      </xsl:when>
      <xsl:otherwise>
				<xsl:value-of select="$numeric * 2"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="InsertColor">
		<xsl:param name="styleString"/>

		<xsl:variable name="color">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="'color'"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="converted">
			<xsl:call-template name="ConvertColor">
				<xsl:with-param name="color" select="$color"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:if test="( $color != '' ) and ( $converted != '' )">
			<w:color w:val="{ $converted }"/>
		</xsl:if>
	</xsl:template>


	<xsl:template name="InsertBackgoundColor">
		<xsl:param name="styleString"/>

		<xsl:variable name="backgroundColor">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="'background-color'"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:if test="$backgroundColor != '' and $backgroundColor != 'transparent' and $backgroundColor != 'inherit'">
			<xsl:variable name="converted">
				<xsl:call-template name="ConvertColor">
					<xsl:with-param name="color" select="$backgroundColor"/>
				</xsl:call-template>
			</xsl:variable>

			<xsl:if test="$converted != ''">
				<w:shd w:val="clear" w:color="auto" w:fill="{ $converted }"/>
			</xsl:if>
		</xsl:if>
	</xsl:template>


	<xsl:template name="InsertTextDecoration">
		<xsl:param name="styleString"/>

		<xsl:variable name="textDecoration">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="'text-decoration'"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:if test="contains( $textDecoration, 'underline' )">
			<w:u w:val="single"/>
		</xsl:if>
		<xsl:if test="contains( $textDecoration, 'overline' )">
			<!-- TODO -->
		</xsl:if>
		<xsl:if test="contains( $textDecoration, 'line-through' )">
			<w:strike/>
		</xsl:if>
		<xsl:if test="contains( $textDecoration, 'blink' )">
			<!-- TODO -->
		</xsl:if>
	</xsl:template>


	<xsl:template name="InsertVerticalTextAlignInTc">
		<xsl:param name="styleString"/>

		<xsl:variable name="valign">
			<xsl:choose>
				<xsl:when test="contains($styleString, 'valign')">
					<xsl:call-template name="GetPropertyFromStyle">
						<xsl:with-param name="styleString" select="$styleString"/>
						<xsl:with-param name="propertyName" select="'valign'"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="contains($styleString, 'vertical-align')">
					<xsl:call-template name="GetPropertyFromStyle">
						<xsl:with-param name="styleString" select="$styleString"/>
						<xsl:with-param name="propertyName" select="'vertical-align'"/>
					</xsl:call-template>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<xsl:choose>
			<xsl:when test="$valign = 'top'">
				<w:vAlign w:val="top"/>
			</xsl:when>
			<xsl:when test="$valign = 'bottom' or $valign = ''">
				<w:vAlign w:val="bottom"/>
			</xsl:when>
			<xsl:when test="$valign = 'center' or $valign = 'middle'">
				<w:vAlign w:val="center"/>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="InsertTextAlign">
		<xsl:param name="styleString"/>


		<xsl:variable name="textAlign">
			<xsl:variable name="a1">
				<xsl:call-template name="GetPropertyFromStyle">
					<xsl:with-param name="styleString" select="$styleString"/>
					<xsl:with-param name="propertyName" select="'text-align'"/>
				</xsl:call-template>
			</xsl:variable>
			<xsl:variable name="a2">
				<xsl:call-template name="GetPropertyFromStyle">
					<xsl:with-param name="styleString" select="$styleString"/>
					<xsl:with-param name="propertyName" select="'align'"/>
				</xsl:call-template>
			</xsl:variable>
			<xsl:variable name="a3">
				<xsl:call-template name="GetPropertyFromStyle">
					<xsl:with-param name="styleString" select="$styleString"/>
					<xsl:with-param name="propertyName" select="'float'"/>
				</xsl:call-template>
			</xsl:variable>
			<xsl:choose>
				<xsl:when test="$a1 != ''">
					<xsl:value-of select="$a1"/>
				</xsl:when>
				<xsl:when test="$a2 != ''">
					<xsl:value-of select="$a2"/>
				</xsl:when>
				<xsl:when test="$a3 != ''">
					<xsl:value-of select="$a3"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<xsl:choose>
			<xsl:when test="$textAlign = 'justify'">
				<w:jc w:val="both"/>
			</xsl:when>
			<xsl:when test="$textAlign = 'left'">
				<w:jc w:val="left"/>
			</xsl:when>
			<xsl:when test="$textAlign = 'right'">
				<w:jc w:val="right"/>
			</xsl:when>
      <xsl:when test="$textAlign = 'center' and name(parent::*) != 'td' or $textAlign = 'middle' and name(parent::*) != 'table'">
        <w:jc w:val="center"/>
      </xsl:when>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="InsertPadding">
		<xsl:param name="styleString"/>
		<w:pBdr>
			<w:top>
				<xsl:call-template name="InsertPaddingSide">
					<xsl:with-param name="styleString" select="$styleString"/>
					<xsl:with-param name="side" select="'top'"/>
				</xsl:call-template>
			</w:top>
			<w:bottom>
				<xsl:call-template name="InsertPaddingSide">
					<xsl:with-param name="styleString" select="$styleString"/>
					<xsl:with-param name="side" select="'top'"/>
				</xsl:call-template>
			</w:bottom>
			<w:left>
				<xsl:call-template name="InsertPaddingSide">
					<xsl:with-param name="styleString" select="$styleString"/>
					<xsl:with-param name="side" select="'top'"/>
				</xsl:call-template>
			</w:left>
			<w:right>
				<xsl:call-template name="InsertPaddingSide">
					<xsl:with-param name="styleString" select="$styleString"/>
					<xsl:with-param name="side" select="'top'"/>
				</xsl:call-template>
			</w:right>
		</w:pBdr>
	</xsl:template>

	<xsl:template name="InsertPaddingSide">
		<xsl:param name="styleString"/>
		<xsl:param name="side"/>

		<xsl:variable name="sideName" select="concat( 'padding-', $side )"/>

		<xsl:variable name="padding">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="$sideName"/>
			</xsl:call-template>
		</xsl:variable>

		<!--
        <xsl:attribute name="w:space">
            <xsl:call-template name="ConvertMeasure">
                <xsl:with-param name="length" select="$padding"/>
                <xsl:with-param name="unit" select="'point'"/>
            </xsl:call-template>
        </xsl:attribute>
        -->

	</xsl:template>

	<xsl:template name="InsertNumberingProperty">
		<xsl:param name="styleString"/>

		<xsl:variable name="level">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="'level'"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="numId">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="'numId'"/>
			</xsl:call-template>
		</xsl:variable>

    <xsl:variable name="checkTag">
      <xsl:value-of select="name(current())"/>
    </xsl:variable>

		<xsl:if test="$level != '' and $numId != '' and $checkTag = 'li'">
			<w:numPr>
				<w:ilvl>
					<xsl:attribute name="w:val">
						<xsl:value-of select="$level"/>
					</xsl:attribute>
				</w:ilvl>

				<w:numId>
					<xsl:attribute name="w:val">
						<xsl:value-of select="$numId"/>
					</xsl:attribute>
				</w:numId>
			</w:numPr>
		</xsl:if>


	</xsl:template>

	<xsl:template name="InsertTextMargins">
		<xsl:param name="styleString"/>


		<xsl:variable name="contextSpacing">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="'contextualSpacing'"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:if test="$contextSpacing = 'true'">
			<w:contextualSpacing/>
		</xsl:if>

		<xsl:variable name="margin">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="'margin'"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="marginParts">
			<xsl:call-template name="CountCompositePropertyParts">
				<xsl:with-param name="property" select="$margin"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="padding">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="padding"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="marginTop">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetPropertyFromStyle">
					<xsl:with-param name="styleString" select="$styleString"/>
					<xsl:with-param name="propertyName" select="'margin-top'"/>
				</xsl:call-template>
			</xsl:variable>
			<xsl:variable name="factMargin">
				<xsl:choose>
					<xsl:when test="$tmp != ''">
						<xsl:value-of select="$tmp"/>
					</xsl:when>
					<xsl:when test="$marginParts = 2">
						<xsl:call-template name="GetCompositePropertyPart">
							<xsl:with-param name="property" select="$margin"/>
							<xsl:with-param name="pos" select="0"/>
						</xsl:call-template>
					</xsl:when>
					<xsl:when test="$margin != ''">
						<xsl:call-template name="GetCompositePropertyPart">
							<xsl:with-param name="property" select="$margin"/>
							<xsl:with-param name="pos" select="0"/>
						</xsl:call-template>
					</xsl:when>
				</xsl:choose>
			</xsl:variable>
			<xsl:choose>
				<xsl:when test="contains($factMargin, '-')">
					<xsl:text>0</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$factMargin"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="marginRight">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetPropertyFromStyle">
					<xsl:with-param name="styleString" select="$styleString"/>
					<xsl:with-param name="propertyName" select="'margin-right'"/>
				</xsl:call-template>
			</xsl:variable>
			<xsl:choose>
				<xsl:when test="$tmp != ''">
					<xsl:value-of select="$tmp"/>
				</xsl:when>
				<xsl:when test="$marginParts = 2">
					<xsl:call-template name="GetCompositePropertyPart">
						<xsl:with-param name="property" select="$margin"/>
						<xsl:with-param name="pos" select="1"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="$margin != ''">
					<xsl:call-template name="GetCompositePropertyPart">
						<xsl:with-param name="property" select="$margin"/>
						<xsl:with-param name="pos" select="1"/>
					</xsl:call-template>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="marginBottom">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetPropertyFromStyle">
					<xsl:with-param name="styleString" select="$styleString"/>
					<xsl:with-param name="propertyName" select="'margin-bottom'"/>
				</xsl:call-template>
			</xsl:variable>
			<xsl:variable name="factMargin">
				<xsl:choose>
					<xsl:when test="$tmp != ''">
						<xsl:value-of select="$tmp"/>
					</xsl:when>
					<xsl:when test="$marginParts = 2">
						<xsl:call-template name="GetCompositePropertyPart">
							<xsl:with-param name="property" select="$margin"/>
							<xsl:with-param name="pos" select="0"/>
						</xsl:call-template>
					</xsl:when>
					<xsl:when test="$margin != ''">
						<xsl:call-template name="GetCompositePropertyPart">
							<xsl:with-param name="property" select="$margin"/>
							<xsl:with-param name="pos" select="2"/>
						</xsl:call-template>
					</xsl:when>
				</xsl:choose>
			</xsl:variable>
			<xsl:choose>
				<xsl:when test="contains($factMargin, '-')">
					<xsl:text>0</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$factMargin"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="marginLeft">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetPropertyFromStyle">
					<xsl:with-param name="styleString" select="$styleString"/>
					<xsl:with-param name="propertyName" select="'margin-left'"/>
				</xsl:call-template>
			</xsl:variable>
			<xsl:choose>
				<xsl:when test="$tmp != ''">
					<xsl:value-of select="$tmp"/>
				</xsl:when>
				<xsl:when test="$marginParts = 2">
					<xsl:call-template name="GetCompositePropertyPart">
						<xsl:with-param name="property" select="$margin"/>
						<xsl:with-param name="pos" select="1"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="$margin != ''">
					<xsl:call-template name="GetCompositePropertyPart">
						<xsl:with-param name="property" select="$margin"/>
						<xsl:with-param name="pos" select="3"/>
					</xsl:call-template>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="paddingTop">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetPropertyFromStyle">
					<xsl:with-param name="styleString" select="$styleString"/>
					<xsl:with-param name="propertyName" select="'padding-top'"/>
				</xsl:call-template>
			</xsl:variable>
			<xsl:choose>
				<xsl:when test="$tmp != ''">
					<xsl:value-of select="$tmp"/>
				</xsl:when>
				<xsl:when test="$padding != ''">
					<xsl:call-template name="GetCompositePropertyPart">
						<xsl:with-param name="property" select="$padding"/>
						<xsl:with-param name="pos" select="0"/>
					</xsl:call-template>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="paddingRight">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetPropertyFromStyle">
					<xsl:with-param name="styleString" select="$styleString"/>
					<xsl:with-param name="propertyName" select="'padding-right'"/>
				</xsl:call-template>
			</xsl:variable>
			<xsl:choose>
				<xsl:when test="$tmp != ''">
					<xsl:value-of select="$tmp"/>
				</xsl:when>
				<xsl:when test="$padding != ''">
					<xsl:call-template name="GetCompositePropertyPart">
						<xsl:with-param name="property" select="$padding"/>
						<xsl:with-param name="pos" select="1"/>
					</xsl:call-template>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="paddingBottom">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetPropertyFromStyle">
					<xsl:with-param name="styleString" select="$styleString"/>
					<xsl:with-param name="propertyName" select="'padding-bottom'"/>
				</xsl:call-template>
			</xsl:variable>
			<xsl:choose>
				<xsl:when test="$tmp != ''">
					<xsl:value-of select="$tmp"/>
				</xsl:when>
				<xsl:when test="$padding != ''">
					<xsl:call-template name="GetCompositePropertyPart">
						<xsl:with-param name="property" select="$padding"/>
						<xsl:with-param name="pos" select="2"/>
					</xsl:call-template>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="paddingLeft">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetPropertyFromStyle">
					<xsl:with-param name="styleString" select="$styleString"/>
					<xsl:with-param name="propertyName" select="'padding-left'"/>
				</xsl:call-template>
			</xsl:variable>
			<xsl:choose>
				<xsl:when test="$tmp != ''">
					<xsl:value-of select="$tmp"/>
				</xsl:when>
				<xsl:when test="$padding != ''">
					<xsl:call-template name="GetCompositePropertyPart">
						<xsl:with-param name="property" select="$padding"/>
						<xsl:with-param name="pos" select="3"/>
					</xsl:call-template>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="textIndent">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="'text-indent'"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="lineHeight">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="'line-height'"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="letterSpacing">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="'letter-spacing'"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="hanging">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="'hanging'"/>
			</xsl:call-template>
		</xsl:variable>

    <xsl:variable name="nameTag">
      <xsl:value-of select="name(current())"/>
    </xsl:variable>

		<xsl:if test="$textIndent != '' or $paddingLeft != '' or $marginRight != '' or $paddingRight != '' or $marginLeft != ''">
			<w:ind>
				  <xsl:if test="$textIndent != '' or $paddingLeft != ''">
					  <xsl:variable name="tmp1">
						  <xsl:choose>
							  <xsl:when test="$textIndent != ''">
								  <xsl:call-template name="ConvertMeasure">
									  <xsl:with-param name="length" select="$textIndent"/>
									  <xsl:with-param name="unit" select="'twips'"/>
								  </xsl:call-template>
							  </xsl:when>
							  <xsl:otherwise>
								  <xsl:text>0</xsl:text>
							  </xsl:otherwise>
						  </xsl:choose>
					  </xsl:variable>
					  <xsl:variable name="tmp2">
						  <xsl:choose>
							  <xsl:when test="$paddingLeft != ''">
								  <xsl:call-template name="ConvertMeasure">
									  <xsl:with-param name="length" select="$paddingLeft"/>
									  <xsl:with-param name="unit" select="'twips'"/>
								  </xsl:call-template>
							  </xsl:when>
							  <xsl:otherwise>
								  <xsl:text>0</xsl:text>
							  </xsl:otherwise>
						  </xsl:choose>
					  </xsl:variable>
					  <!--<xsl:if test="number(number($tmp1) + number($tmp2)) != NaN or number(number($tmp1) + number($tmp2)) != 'NaN'">
						  <xsl:attribute name="w:firstLine">
							  <xsl:value-of select="number($tmp1) + number($tmp2)"/>
						  </xsl:attribute>
					  </xsl:if>
				  </xsl:if>-->
            <xsl:variable name="tmp3">
              <xsl:choose>
                <xsl:when test="$marginLeft != ''">
                  <xsl:call-template name="ConvertMeasure">
                    <xsl:with-param name="length" select="$marginLeft"/>
                    <xsl:with-param name="unit" select="'twips'"/>
                  </xsl:call-template>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:text>0</xsl:text>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>
            <xsl:if test="$nameTag != 'li'">
              <xsl:if test="number(number($tmp1) + number($tmp2)) != NaN or number(number($tmp1) + number($tmp2)) != 'NaN'">
                <xsl:attribute name="w:firstLine">
                  <xsl:value-of select="number($tmp1) + number($tmp2)"/>
                </xsl:attribute>
              </xsl:if>
              <xsl:if test="name(parent::*) = 'ol' or name(parent::*) = 'ul'">
                <xsl:attribute name="w:firstLine">
                  <xsl:value-of select="number($tmp1) + number($tmp2)-number($tmp3)+1000"/>
                </xsl:attribute>
              </xsl:if>
            </xsl:if>
          </xsl:if>

        <xsl:variable name="parentMarginRight">
          <xsl:call-template name="GetPropertyFromStyle">
            <xsl:with-param name="styleString" select="../@style"/>
            <xsl:with-param name="propertyName" select="'margin-right'"/>
          </xsl:call-template>
        </xsl:variable>
          <xsl:variable name="parentPaddingRight">
          <xsl:call-template name="GetPropertyFromStyle">
            <xsl:with-param name="styleString" select="../@style"/>
            <xsl:with-param name="propertyName" select="'padding-right'"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:variable name="tmpAllMarginRight">
          <xsl:choose>
            <xsl:when test="$parentMarginRight != ''">
              <xsl:call-template name="ConvertMeasure">
                <xsl:with-param name="length" select="$parentMarginRight"/>
                <xsl:with-param name="unit" select="'twips'"/>
              </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>0</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        <xsl:variable name="tmpAllPaddingRight">
          <xsl:choose>
            <xsl:when test="$parentMarginRight != ''">
              <xsl:call-template name="ConvertMeasure">
                <xsl:with-param name="length" select="$parentPaddingRight"/>
                <xsl:with-param name="unit" select="'twips'"/>
              </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>0</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        
				  <xsl:if test="$marginRight != '' or $paddingRight != '' or ($tmpAllPaddingRight != '' and $nameTag = 'li') or ($tmpAllMarginRight != '' and $nameTag = 'li')">
					  <xsl:variable name="tmp1">
						  <xsl:choose>
							  <xsl:when test="$marginRight != ''">
								  <xsl:call-template name="ConvertMeasure">
									  <xsl:with-param name="length" select="$marginRight"/>
									  <xsl:with-param name="unit" select="'twips'"/>
								  </xsl:call-template>
							  </xsl:when>
							  <xsl:otherwise>
								  <xsl:text>0</xsl:text>
							  </xsl:otherwise>
						  </xsl:choose>
					  </xsl:variable>
					  <xsl:variable name="tmp2">
						  <xsl:choose>
							  <xsl:when test="$paddingRight != ''">
								  <xsl:call-template name="ConvertMeasure">
									  <xsl:with-param name="length" select="$paddingRight"/>
									  <xsl:with-param name="unit" select="'twips'"/>
								  </xsl:call-template>
							  </xsl:when>
							  <xsl:otherwise>
								  <xsl:text>0</xsl:text>
							  </xsl:otherwise>
						  </xsl:choose>
					  </xsl:variable>
				    <xsl:if test="(number(number($tmp1) + number($tmp2)) != NaN or number(number($tmp1) + number($tmp2)) != 'NaN') and $nameTag != 'li'">
					    <xsl:attribute name="w:right">
						    <xsl:value-of select="number($tmp1) + number($tmp2)"/>
					    </xsl:attribute>
				    </xsl:if>
            <xsl:if test="$nameTag = 'li'">
              <xsl:attribute name="w:right">
                <xsl:value-of select="$tmp1 + $tmp2 + $tmpAllMarginRight + $tmpAllPaddingRight"/>
              </xsl:attribute>
            </xsl:if>
				  </xsl:if>


				  <xsl:if test="$marginLeft != '' or $paddingLeft != ''">
					  <xsl:variable name="tmp1">
						  <xsl:choose>
							  <xsl:when test="$marginLeft != ''">
								  <xsl:call-template name="ConvertMeasure">
									  <xsl:with-param name="length" select="$marginLeft"/>
									  <xsl:with-param name="unit" select="'twips'"/>
								  </xsl:call-template>
							  </xsl:when>
							  <xsl:otherwise>
								  <xsl:text>0</xsl:text>
							  </xsl:otherwise>
						  </xsl:choose>
					  </xsl:variable>
					  <xsl:variable name="tmp2">
						  <xsl:choose>
							  <xsl:when test="$paddingLeft != ''">
								  <xsl:call-template name="ConvertMeasure">
									  <xsl:with-param name="length" select="$paddingLeft"/>
									  <xsl:with-param name="unit" select="'twips'"/>
								  </xsl:call-template>
							  </xsl:when>
							  <xsl:otherwise>
								  <xsl:text>0</xsl:text>
							  </xsl:otherwise>
						  </xsl:choose>
					  </xsl:variable>
            <xsl:variable name="marginLeft2">
              <xsl:call-template name="GetPropertyFromStyle">
                <xsl:with-param name="styleString" select="@style"/>
                <xsl:with-param name="propertyName" select="'margin-left'"/>
              </xsl:call-template>
            </xsl:variable>
            <xsl:variable name="marginLeft3">
              <xsl:call-template name="ConvertMeasure">
                <xsl:with-param name="length" select="$marginLeft2"/>
                <xsl:with-param name="unit" select="'twips'"/>
              </xsl:call-template>
            </xsl:variable>
            <xsl:variable name="tmpMarinLeft">
              <xsl:choose>
                <xsl:when test="$marginLeft = 'NaN' or $marginLeft = 'NaNpx'">
                  <xsl:value-of select="$marginLeft3 + 720"/>
                </xsl:when>
                <xsl:when test="$marginLeft != '' and $marginLeft != '1em'">
                  <xsl:call-template name="ConvertMeasure">
                    <xsl:with-param name="length" select="$marginLeft"/>
                    <xsl:with-param name="unit" select="'twips'"/>
                  </xsl:call-template>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:text>0</xsl:text>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>

            <xsl:variable name="paddingLeftLi">
              <xsl:call-template name="GetPropertyFromStyle">
                <xsl:with-param name="styleString" select="@style"/>
                <xsl:with-param name="propertyName" select="'padding-left'"/>
              </xsl:call-template>
            </xsl:variable>

            <xsl:variable name="tmpPaddingLeft">
              <xsl:choose>
                <xsl:when test="$paddingLeftLi != ''">
                  <xsl:call-template name="ConvertMeasure">
                    <xsl:with-param name="length" select="$paddingLeftLi"/>
                    <xsl:with-param name="unit" select="'twips'"/>
                  </xsl:call-template>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:text>0</xsl:text>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>

            <xsl:variable name="parentMarginLeft">
              <xsl:call-template name="GetPropertyFromStyle">
                <xsl:with-param name="styleString" select="../@style"/>
                <xsl:with-param name="propertyName" select="'margin-left'"/>
              </xsl:call-template>
            </xsl:variable>

            <xsl:variable name="paddingList">
              <xsl:call-template name="GetPropertyFromStyle">
                <xsl:with-param name="styleString" select="../@style"/>
                <xsl:with-param name="propertyName" select="'padding-left'"/>
              </xsl:call-template>
            </xsl:variable>

            <xsl:variable name="tmp21">
              <xsl:choose>
                <xsl:when test="$parentMarginLeft != ''">
                  <xsl:call-template name="ConvertMeasure">
                    <xsl:with-param name="length" select="$parentMarginLeft"/>
                    <xsl:with-param name="unit" select="'twips'"/>
                  </xsl:call-template>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:text>0</xsl:text>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>
            <xsl:variable name="tmp22">
              <xsl:choose>
                <xsl:when test="$paddingList != ''">
                  <xsl:call-template name="ConvertMeasure">
                    <xsl:with-param name="length" select="$paddingList"/>
                    <xsl:with-param name="unit" select="'twips'"/>
                  </xsl:call-template>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:text>0</xsl:text>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>
					  <xsl:if test="(number(number($tmp1) + number($tmp2)) != NaN or number(number($tmp1) + number($tmp2)) != 'NaN') and $nameTag != 'li'">
						  <xsl:attribute name="w:left">
							  <xsl:value-of select="number($tmp1) + number($tmp2)"/>
						  </xsl:attribute>
					  </xsl:if>
            <xsl:if test="$nameTag = 'li' and $tmpPaddingLeft != 'NaN' and $tmpMarinLeft != 'NaN' and $tmp21 != 'NaN' and $tmp22 != 'NaN'">
              <xsl:choose>
                <xsl:when test="($tmpPaddingLeft = 0 and $tmpMarinLeft = '720') or (($marginLeft2 = '0' and $marginLeft = 'NaNpx' and $tmpPaddingLeft = '0') and ($paddingList = '0' or $paddingList = '0px'))">
                  <xsl:variable name="tmpTest">
                    <xsl:value-of select="$tmpPaddingLeft + $tmpMarinLeft + $tmp21"/>
                  </xsl:variable>
                </xsl:when>
                <xsl:when test="($paddingList != 'NaN' and $paddingList != 'NaNpx' and $paddingList != '') ">
                  <xsl:attribute name="w:left">
                    <xsl:value-of select="$tmpMarinLeft + $tmp21 + $tmp22 - 720"/>
                  </xsl:attribute>
                  <xsl:attribute name="w:hanging">
                    <xsl:value-of select="$tmpPaddingLeft + 360"/>
                  </xsl:attribute>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:attribute name="w:left">
                    <xsl:value-of select="$tmpMarinLeft + $tmp21 + $tmp22"/>
                  </xsl:attribute>
                  <xsl:attribute name="w:hanging">
                    <xsl:value-of select="$tmpPaddingLeft + 360"/>
                  </xsl:attribute>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:if>
				  </xsl:if>

				  <xsl:if test="$hanging != '' and $nameTag != 'li'">
					  <xsl:attribute name="w:hanging">
						  <xsl:value-of select="$hanging"/>
					  </xsl:attribute>
				  </xsl:if>
			</w:ind>
		</xsl:if>

		<xsl:choose>
			<xsl:when test="$marginTop != '' or $marginBottom != '' or $lineHeight != '' or $letterSpacing != '' or $paddingBottom != '' or $paddingTop != ''">
				<w:spacing>
          <xsl:variable name="tmp1">
            <xsl:choose>
              <xsl:when test="$marginTop != ''">
                <xsl:call-template name="ConvertMeasure">
                  <xsl:with-param name="length" select="$marginTop"/>
                  <xsl:with-param name="unit" select="'twips'"/>
                </xsl:call-template>
              </xsl:when>
              <xsl:otherwise>
                <xsl:text>0</xsl:text>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:variable>
          <xsl:variable name="tmp2">
            <xsl:choose>
              <xsl:when test="$paddingTop != ''">
                <xsl:call-template name="ConvertMeasure">
                  <xsl:with-param name="length" select="$paddingTop"/>
                  <xsl:with-param name="unit" select="'twips'"/>
                </xsl:call-template>
              </xsl:when>
              <xsl:otherwise>
                <xsl:text>0</xsl:text>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:variable>
          <xsl:variable name="tmp3">
            <xsl:choose>
              <xsl:when test="$marginBottom != ''">
                <xsl:call-template name="ConvertMeasure">
                  <xsl:with-param name="length" select="$marginBottom"/>
                  <xsl:with-param name="unit" select="'twips'"/>
                </xsl:call-template>
              </xsl:when>
              <xsl:otherwise>
                <xsl:text>0</xsl:text>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:variable>
          <xsl:variable name="tmp4">
            <xsl:choose>
              <xsl:when test="$paddingBottom != ''">
                <xsl:call-template name="ConvertMeasure">
                  <xsl:with-param name="length" select="$paddingBottom"/>
                  <xsl:with-param name="unit" select="'twips'"/>
                </xsl:call-template>
              </xsl:when>
              <xsl:otherwise>
                <xsl:text>0</xsl:text>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:variable>
					<xsl:attribute name="w:before">
            <xsl:value-of select="$tmp1 + $tmp2"/>
					</xsl:attribute>
					<xsl:attribute name="w:after">
            <xsl:value-of select="$tmp3 + $tmp4"/>
					</xsl:attribute>
					<xsl:if test="$lineHeight != ''">
						<xsl:attribute name="w:line">
							<xsl:choose>
								<xsl:when test="contains($lineHeight, '%')">
									<xsl:value-of select="number(substring-before($lineHeight, '%')) * 2.4"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:text>240</xsl:text>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:attribute>
						<xsl:attribute name="w:lineRule">
							<xsl:text>auto</xsl:text>
						</xsl:attribute>
					</xsl:if>
          <!--<xsl:if test="$lineHeight = ''">
            <xsl:attribute name="w:line">
              <xsl:text>240</xsl:text>
            </xsl:attribute>
            <xsl:attribute name="w:lineRule">
              <xsl:text>auto</xsl:text>
            </xsl:attribute>
          </xsl:if>-->
					<xsl:if test="$letterSpacing != ''">
						<xsl:attribute name="w:val">
							<xsl:call-template name="ConvertMeasure">
								<xsl:with-param name="length" select="$letterSpacing"/>
								<xsl:with-param name="unit" select="'twips'"/>
							</xsl:call-template>
						</xsl:attribute>
					</xsl:if>
				</w:spacing>
			</xsl:when>
			<xsl:otherwise>
				<!--<w:spacing w:before="100" w:beforeAutospacing="1" w:after="100" w:afterAutospacing="1" w:line="240"/>-->
			</xsl:otherwise>
		</xsl:choose>

	</xsl:template>

	<xsl:template name="InsertBorderCollapse">
		<xsl:param name="styleString"/>

		<xsl:variable name="borderCollapse">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="'border-collapse'"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:if test="$borderCollapse != 'collapse' and $borderCollapse != ''">
			<w:tblCellSpacing w:w="15" w:type="dxa"/>
		</xsl:if>
	</xsl:template>


	<xsl:template name="InsertBorders">
		<xsl:param name="styleString"/>
		<xsl:param name="flag">false</xsl:param>

		<xsl:variable name="border">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="'border'"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="hasBorder">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="'hasBorder'"/>
			</xsl:call-template>
		</xsl:variable>


		<xsl:variable name="borderStyle">
			<xsl:choose>
				<xsl:when test="contains($styleString, 'border-style')">
					<xsl:call-template name="GetPropertyFromStyle">
						<xsl:with-param name="styleString" select="$styleString"/>
						<xsl:with-param name="propertyName" select="'border-style'"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="$border != ''">
					<xsl:value-of select="$border"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="borderWidth">
			<xsl:choose>
				<xsl:when test="$hasBorder != ''">
					<xsl:value-of select="$hasBorder"/>
				</xsl:when>
				<xsl:when test="contains($styleString, 'border-width')">
					<xsl:call-template name="GetPropertyFromStyle">
						<xsl:with-param name="styleString" select="$styleString"/>
						<xsl:with-param name="propertyName" select="'border-width'"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="$border != ''">
					<xsl:value-of select="$border"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="borderColor">
			<xsl:choose>
				<xsl:when test="contains($styleString, 'border-color')">
					<xsl:call-template name="GetPropertyFromStyle">
						<xsl:with-param name="styleString" select="$styleString"/>
						<xsl:with-param name="propertyName" select="'border-color'"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="$border != ''">
					<xsl:value-of select="$border"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="borderLeft">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="'border-left'"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="borderLeftStyle">
			<xsl:choose>
				<xsl:when test="contains($styleString, 'border-left-style')">
					<xsl:call-template name="GetPropertyFromStyle">
						<xsl:with-param name="styleString" select="$styleString"/>
						<xsl:with-param name="propertyName" select="'border-left-style'"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="$borderLeft != ''">
					<xsl:value-of select="$borderLeft"/>
				</xsl:when>
				<xsl:when test="$borderStyle != ''">
					<xsl:value-of select="$borderStyle"/>
				</xsl:when>
				<xsl:when test="$border != ''">
					<xsl:value-of select="$border"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="borderLeftWidth">
			<xsl:choose>
				<xsl:when test="contains($styleString, 'border-left-width')">
					<xsl:call-template name="GetPropertyFromStyle">
						<xsl:with-param name="styleString" select="$styleString"/>
						<xsl:with-param name="propertyName" select="'border-left-width'"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="$borderLeft != ''">
					<xsl:value-of select="$borderLeft"/>
				</xsl:when>
				<xsl:when test="$borderWidth != ''">
					<xsl:value-of select="$borderWidth"/>
				</xsl:when>
				<xsl:when test="$border != ''">
					<xsl:value-of select="$border"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="borderLeftColor">
			<xsl:choose>
				<xsl:when test="contains($styleString, 'border-left-color')">
					<xsl:call-template name="GetPropertyFromStyle">
						<xsl:with-param name="styleString" select="$styleString"/>
						<xsl:with-param name="propertyName" select="'border-left-color'"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="$borderLeft != ''">
					<xsl:value-of select="$borderLeft"/>
				</xsl:when>
				<xsl:when test="$borderColor != ''">
					<xsl:value-of select="$borderColor"/>
				</xsl:when>
				<xsl:when test="$border != ''">
					<xsl:value-of select="$border"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="borderRight">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="'border-right'"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="borderRightStyle">
			<xsl:choose>
				<xsl:when test="contains($styleString, 'border-right-style')">
					<xsl:call-template name="GetPropertyFromStyle">
						<xsl:with-param name="styleString" select="$styleString"/>
						<xsl:with-param name="propertyName" select="'border-right-style'"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="$borderRight != ''">
					<xsl:value-of select="$borderRight"/>
				</xsl:when>
				<xsl:when test="$borderStyle != ''">
					<xsl:value-of select="$borderStyle"/>
				</xsl:when>
				<xsl:when test="$border != ''">
					<xsl:value-of select="$border"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="borderRightWidth">
			<xsl:choose>
				<xsl:when test="contains($styleString, 'border-right-width')">
					<xsl:call-template name="GetPropertyFromStyle">
						<xsl:with-param name="styleString" select="$styleString"/>
						<xsl:with-param name="propertyName" select="'border-right-width'"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="$borderRight != ''">
					<xsl:value-of select="$borderRight"/>
				</xsl:when>
				<xsl:when test="$borderWidth != ''">
					<xsl:value-of select="$borderWidth"/>
				</xsl:when>
				<xsl:when test="$border != ''">
					<xsl:value-of select="$border"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="borderRightColor">
			<xsl:choose>
				<xsl:when test="contains($styleString, 'border-right-color')">
					<xsl:call-template name="GetPropertyFromStyle">
						<xsl:with-param name="styleString" select="$styleString"/>
						<xsl:with-param name="propertyName" select="'border-right-color'"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="$borderRight != ''">
					<xsl:value-of select="$borderRight"/>
				</xsl:when>
				<xsl:when test="$borderColor != ''">
					<xsl:value-of select="$borderColor"/>
				</xsl:when>
				<xsl:when test="$border != ''">
					<xsl:value-of select="$border"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>


		<xsl:variable name="borderTop">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="'border-top'"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="borderTopStyle">
			<xsl:choose>
				<xsl:when test="contains($styleString, 'border-top-style')">
					<xsl:call-template name="GetPropertyFromStyle">
						<xsl:with-param name="styleString" select="$styleString"/>
						<xsl:with-param name="propertyName" select="'border-top-style'"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="$borderTop != ''">
					<xsl:value-of select="$borderTop"/>
				</xsl:when>
				<xsl:when test="$borderStyle != ''">
					<xsl:value-of select="$borderStyle"/>
				</xsl:when>
				<xsl:when test="$border != ''">
					<xsl:value-of select="$border"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="borderTopWidth">
			<xsl:choose>
				<xsl:when test="contains($styleString, 'border-top-width')">
					<xsl:call-template name="GetPropertyFromStyle">
						<xsl:with-param name="styleString" select="$styleString"/>
						<xsl:with-param name="propertyName" select="'border-top-width'"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="$borderTop != ''">
					<xsl:value-of select="$borderTop"/>
				</xsl:when>
				<xsl:when test="$borderWidth != ''">
					<xsl:value-of select="$borderWidth"/>
				</xsl:when>
				<xsl:when test="$border != ''">
					<xsl:value-of select="$border"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="borderTopColor">
			<xsl:choose>
				<xsl:when test="contains($styleString, 'border-top-color')">
					<xsl:call-template name="GetPropertyFromStyle">
						<xsl:with-param name="styleString" select="$styleString"/>
						<xsl:with-param name="propertyName" select="'border-top-color'"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="$borderTop != ''">
					<xsl:value-of select="$borderTop"/>
				</xsl:when>
				<xsl:when test="$borderColor != ''">
					<xsl:value-of select="$borderColor"/>
				</xsl:when>
				<xsl:when test="$border != ''">
					<xsl:value-of select="$border"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>


		<xsl:variable name="borderBottom">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="'border-bottom'"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="borderBottomStyle">
			<xsl:choose>
				<xsl:when test="contains($styleString, 'border-bottom-style')">
					<xsl:call-template name="GetPropertyFromStyle">
						<xsl:with-param name="styleString" select="$styleString"/>
						<xsl:with-param name="propertyName" select="'border-bottom-style'"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="$borderBottom != ''">
					<xsl:value-of select="$borderBottom"/>
				</xsl:when>
				<xsl:when test="$borderStyle != ''">
					<xsl:value-of select="$borderStyle"/>
				</xsl:when>
				<xsl:when test="$border != ''">
					<xsl:value-of select="$border"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="borderBottomWidth">
			<xsl:choose>
				<xsl:when test="contains($styleString, 'border-bottom-width')">
					<xsl:call-template name="GetPropertyFromStyle">
						<xsl:with-param name="styleString" select="$styleString"/>
						<xsl:with-param name="propertyName" select="'border-bottom-width'"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="$borderBottom != ''">
					<xsl:value-of select="$borderBottom"/>
				</xsl:when>
				<xsl:when test="$borderWidth != ''">
					<xsl:value-of select="$borderWidth"/>
				</xsl:when>
				<xsl:when test="$border != ''">
					<xsl:value-of select="$border"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="borderBottomColor">
			<xsl:choose>
				<xsl:when test="contains($styleString, 'border-bottom-color')">
					<xsl:call-template name="GetPropertyFromStyle">
						<xsl:with-param name="styleString" select="$styleString"/>
						<xsl:with-param name="propertyName" select="'border-bottom-color'"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="$borderBottom != ''">
					<xsl:value-of select="$borderBottom"/>
				</xsl:when>
				<xsl:when test="$borderColor != ''">
					<xsl:value-of select="$borderColor"/>
				</xsl:when>
				<xsl:when test="$border != ''">
					<xsl:value-of select="$border"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="margin">
			<xsl:call-template name="GetPropertyFromStyle">
				<xsl:with-param name="styleString" select="$styleString"/>
				<xsl:with-param name="propertyName" select="'margin'"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:variable name="marginParts">
			<xsl:call-template name="CountCompositePropertyParts">
				<xsl:with-param name="property" select="$margin"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="marginTop">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetPropertyFromStyle">
					<xsl:with-param name="styleString" select="$styleString"/>
					<xsl:with-param name="propertyName" select="'margin-top'"/>
				</xsl:call-template>
			</xsl:variable>
			<xsl:choose>
				<xsl:when test="$tmp != ''">
					<xsl:value-of select="$tmp"/>
				</xsl:when>
				<xsl:when test="$marginParts = 2">
					<xsl:call-template name="GetCompositePropertyPart">
						<xsl:with-param name="property" select="$margin"/>
						<xsl:with-param name="pos" select="0"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="$margin != ''">
					<xsl:call-template name="GetCompositePropertyPart">
						<xsl:with-param name="property" select="$margin"/>
						<xsl:with-param name="pos" select="0"/>
					</xsl:call-template>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="marginRight">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetPropertyFromStyle">
					<xsl:with-param name="styleString" select="$styleString"/>
					<xsl:with-param name="propertyName" select="'margin-right'"/>
				</xsl:call-template>
			</xsl:variable>
			<xsl:choose>
				<xsl:when test="$tmp != ''">
					<xsl:value-of select="$tmp"/>
				</xsl:when>
				<xsl:when test="$marginParts = 2">
					<xsl:call-template name="GetCompositePropertyPart">
						<xsl:with-param name="property" select="$margin"/>
						<xsl:with-param name="pos" select="1"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="$margin != ''">
					<xsl:call-template name="GetCompositePropertyPart">
						<xsl:with-param name="property" select="$margin"/>
						<xsl:with-param name="pos" select="1"/>
					</xsl:call-template>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="marginBottom">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetPropertyFromStyle">
					<xsl:with-param name="styleString" select="$styleString"/>
					<xsl:with-param name="propertyName" select="'margin-bottom'"/>
				</xsl:call-template>
			</xsl:variable>
			<xsl:choose>
				<xsl:when test="$tmp != ''">
					<xsl:value-of select="$tmp"/>
				</xsl:when>
				<xsl:when test="$marginParts = 2">
					<xsl:call-template name="GetCompositePropertyPart">
						<xsl:with-param name="property" select="$margin"/>
						<xsl:with-param name="pos" select="0"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="$margin != ''">
					<xsl:call-template name="GetCompositePropertyPart">
						<xsl:with-param name="property" select="$margin"/>
						<xsl:with-param name="pos" select="2"/>
					</xsl:call-template>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="marginLeft">
			<xsl:variable name="tmp">
				<xsl:call-template name="GetPropertyFromStyle">
					<xsl:with-param name="styleString" select="$styleString"/>
					<xsl:with-param name="propertyName" select="'margin-left'"/>
				</xsl:call-template>
			</xsl:variable>
			<xsl:choose>
				<xsl:when test="$tmp != ''">
					<xsl:value-of select="$tmp"/>
				</xsl:when>
				<xsl:when test="$marginParts = 2">
					<xsl:call-template name="GetCompositePropertyPart">
						<xsl:with-param name="property" select="$margin"/>
						<xsl:with-param name="pos" select="1"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="$margin != ''">
					<xsl:call-template name="GetCompositePropertyPart">
						<xsl:with-param name="property" select="$margin"/>
						<xsl:with-param name="pos" select="3"/>
					</xsl:call-template>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="borderCollapse"/>

		<xsl:if test="$borderTopWidth != '' or $flag = 'true'">
			<w:top>
				<xsl:attribute name="w:val">
					<xsl:choose>
						<xsl:when test="$borderTopStyle != ''">
							<xsl:call-template name="ConvertBorderStyle">
								<xsl:with-param name="style" select="$borderTopStyle"/>
							</xsl:call-template>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>single</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>
				<xsl:attribute name="w:sz">
					<xsl:variable name="bdr">
						<xsl:choose>
							<xsl:when test="$borderTopWidth != '' or $borderTopWidth != '0'">
								<xsl:call-template name="ConvertBorderWidth">
									<xsl:with-param name="width" select="$borderTopWidth"/>
								</xsl:call-template>
							</xsl:when>
						</xsl:choose>
					</xsl:variable>
					<xsl:choose>
						<xsl:when test="$bdr != '' and $bdr != 'NaN' and number($bdr) != 0">
							<xsl:value-of select="$bdr"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>auto</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>

				<xsl:attribute name="w:color">
					<xsl:choose>
						<xsl:when test="$borderTopColor != ''">
							<xsl:call-template name="ConvertBorderColor">
								<xsl:with-param name="style" select="$borderTopColor"/>
							</xsl:call-template>
							<!--<xsl:call-template name="ConvertColor">
										<xsl:with-param name="color" select="$borderTopColor"/>
									</xsl:call-template>-->
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>auto</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>
				<xsl:attribute name="w:space">
					<xsl:call-template name="ConvertMeasure">
						<xsl:with-param name="length" select="number( $marginTop )"/>
						<xsl:with-param name="unit" select="'point'"/>
					</xsl:call-template>
				</xsl:attribute>
			</w:top>
		</xsl:if>

		<xsl:if test="$borderLeftWidth != '' or $flag = 'true'">
			<w:left>
				<xsl:attribute name="w:val">
					<xsl:choose>
						<xsl:when test="$borderLeftStyle != ''">
							<xsl:call-template name="ConvertBorderStyle">
								<xsl:with-param name="style" select="$borderLeftStyle"/>
							</xsl:call-template>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>single</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>

				<xsl:attribute name="w:sz">
					<xsl:variable name="bdr">
						<xsl:choose>
							<xsl:when test="$borderLeftWidth != '' or $borderLeftWidth != '0'">
								<xsl:call-template name="ConvertBorderWidth">
									<xsl:with-param name="width" select="$borderLeftWidth"/>
								</xsl:call-template>
							</xsl:when>
						</xsl:choose>
					</xsl:variable>
					<xsl:choose>
						<xsl:when test="$bdr != '' and $bdr != 'NaN' and number($bdr) != 0">
							<xsl:value-of select="$bdr"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>auto</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>

				<xsl:attribute name="w:color">
					<xsl:choose>
						<xsl:when test="$borderLeftColor != ''">
							<xsl:call-template name="ConvertBorderColor">
								<xsl:with-param name="style" select="$borderTopColor"/>
							</xsl:call-template>
							<!--<xsl:call-template name="ConvertColor">
                  <xsl:with-param name="color" select="$borderLeftColor"/>
                </xsl:call-template>-->
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>auto</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>
				<xsl:attribute name="w:space">
					<xsl:call-template name="ConvertMeasure">
						<xsl:with-param name="length" select="number( $marginLeft )"/>
						<xsl:with-param name="unit" select="'point'"/>
					</xsl:call-template>
				</xsl:attribute>
			</w:left>
		</xsl:if>

		<xsl:if test="$borderBottomWidth != '' or $flag = 'true'">
			<w:bottom>
				<xsl:attribute name="w:val">
					<xsl:choose>
						<xsl:when test="$borderBottomStyle != ''">
							<xsl:call-template name="ConvertBorderStyle">
								<xsl:with-param name="style" select="$borderBottomStyle"/>
							</xsl:call-template>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>single</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>

				<xsl:attribute name="w:sz">
					<xsl:variable name="bdr">
						<xsl:choose>
							<xsl:when test="$borderBottomWidth != '' or $borderBottomWidth != '0'">
								<xsl:call-template name="ConvertBorderWidth">
									<xsl:with-param name="width" select="$borderBottomWidth"/>
								</xsl:call-template>
							</xsl:when>
						</xsl:choose>
					</xsl:variable>
					<xsl:choose>
						<xsl:when test="$bdr != '' and $bdr != 'NaN' and number($bdr) != 0">
							<xsl:value-of select="$bdr"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>auto</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>

				<xsl:attribute name="w:color">
					<xsl:choose>
						<xsl:when test="$borderBottomColor != ''">
							<xsl:call-template name="ConvertBorderColor">
								<xsl:with-param name="style" select="$borderBottomColor"/>
							</xsl:call-template>
							<!--<xsl:call-template name="ConvertColor">
                  <xsl:with-param name="color" select="$borderBottomColor"/>
                </xsl:call-template>-->
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>auto</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>
				<xsl:attribute name="w:space">
					<xsl:call-template name="ConvertMeasure">
						<xsl:with-param name="length" select="number( $marginBottom )"/>
						<xsl:with-param name="unit" select="'point'"/>
					</xsl:call-template>
				</xsl:attribute>
			</w:bottom>
		</xsl:if>

		<xsl:if test="$borderRightWidth != '' or $flag = 'true'">
			<w:right>
				<xsl:attribute name="w:val">
					<xsl:choose>
						<xsl:when test="$borderRightStyle != ''">
							<xsl:call-template name="ConvertBorderStyle">
								<xsl:with-param name="style" select="$borderRightStyle"/>
							</xsl:call-template>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>single</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>

				<xsl:attribute name="w:sz">
					<xsl:variable name="bdr">
						<xsl:choose>
							<xsl:when test="$borderRightWidth != '' or $borderRightWidth != '0'">
								<xsl:call-template name="ConvertBorderWidth">
									<xsl:with-param name="width" select="$borderRightWidth"/>
								</xsl:call-template>
							</xsl:when>
						</xsl:choose>
					</xsl:variable>
					<xsl:choose>
						<xsl:when test="$bdr != '' and $bdr != 'NaN' and number($bdr) != 0">
							<xsl:value-of select="$bdr"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>auto</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>

				<xsl:attribute name="w:color">
					<xsl:choose>
						<xsl:when test="$borderRightColor != ''">
							<xsl:call-template name="ConvertBorderColor">
								<xsl:with-param name="style" select="$borderRightColor"/>
							</xsl:call-template>
							<!--<xsl:call-template name="ConvertColor">
                  <xsl:with-param name="color" select="$borderRightColor"/>
                </xsl:call-template>-->
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>auto</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>
				<xsl:attribute name="w:space">
					<xsl:call-template name="ConvertMeasure">
						<xsl:with-param name="length" select="number( $marginRight )"/>
						<xsl:with-param name="unit" select="'point'"/>
					</xsl:call-template>
				</xsl:attribute>
			</w:right>
		</xsl:if>

		<xsl:if test="$flag = 'true'">
			<w:insideV>
				<xsl:attribute name="w:val">
					<xsl:choose>
						<xsl:when test="$borderStyle != ''">
							<xsl:call-template name="ConvertBorderStyle">
								<xsl:with-param name="style" select="$borderStyle"/>
							</xsl:call-template>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>single</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>

				<xsl:attribute name="w:sz">
					<xsl:variable name="bdr">
						<xsl:choose>
							<xsl:when test="$borderWidth != '' or $borderWidth != '0'">
								<xsl:call-template name="ConvertBorderWidth">
									<xsl:with-param name="width" select="$borderWidth"/>
								</xsl:call-template>
							</xsl:when>
						</xsl:choose>
					</xsl:variable>
					<xsl:choose>
						<xsl:when test="$bdr != '' and $bdr != 'NaN' and number($bdr) != 0">
							<xsl:value-of select="$bdr"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>auto</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>

				<xsl:attribute name="w:color">
					<xsl:choose>
						<xsl:when test="$borderColor != ''">
							<xsl:call-template name="ConvertBorderColor">
								<xsl:with-param name="style" select="$borderColor"/>
							</xsl:call-template>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>auto</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>
				<xsl:attribute name="w:space">
					<xsl:call-template name="ConvertMeasure">
						<xsl:with-param name="length" select="number( $margin )"/>
						<xsl:with-param name="unit" select="'point'"/>
					</xsl:call-template>
				</xsl:attribute>
			</w:insideV>

			<w:insideH>
				<xsl:attribute name="w:val">
					<xsl:choose>
						<xsl:when test="$borderStyle != ''">
							<xsl:call-template name="ConvertBorderStyle">
								<xsl:with-param name="style" select="$borderStyle"/>
							</xsl:call-template>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>single</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>

				<xsl:attribute name="w:sz">
					<xsl:variable name="bdr">
						<xsl:choose>
							<xsl:when test="$borderWidth != '' or $borderWidth != '0'">
								<xsl:call-template name="ConvertBorderWidth">
									<xsl:with-param name="width" select="$borderWidth"/>
								</xsl:call-template>
							</xsl:when>
						</xsl:choose>
					</xsl:variable>
					<xsl:choose>
						<xsl:when test="$bdr != '' and $bdr != 'NaN' and number($bdr) != 0">
							<xsl:value-of select="$bdr"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>auto</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>

				<xsl:attribute name="w:color">
					<xsl:choose>
						<xsl:when test="$borderColor != ''">
							<xsl:call-template name="ConvertBorderColor">
								<xsl:with-param name="style" select="$borderColor"/>
							</xsl:call-template>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>auto</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>
				<xsl:attribute name="w:space">
					<xsl:call-template name="ConvertMeasure">
						<xsl:with-param name="length" select="number( $margin )"/>
						<xsl:with-param name="unit" select="'point'"/>
					</xsl:call-template>
				</xsl:attribute>
			</w:insideH>
		</xsl:if>
	</xsl:template>


	<xsl:template name="ConvertBorderColor">
		<xsl:param name="style"/>

		<xsl:variable name="Color">
			<xsl:call-template name="IncludeBorderColor">
				<xsl:with-param name="style" select="$style"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:variable name="col">
			<xsl:call-template name="ConvertColor">
				<xsl:with-param name="color" select="$Color"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="$col != ''">
				<xsl:value-of select="$col"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>auto</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<xsl:template name="IncludeBorderColor">
		<xsl:param name="style"/>
		<xsl:param name="count" select="0"/>

		<xsl:variable name="newStyle">
			<xsl:call-template name="GetCompositePropertyPart">
				<xsl:with-param name="property" select="$style"/>
				<xsl:with-param name="pos" select="$count"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:choose>
			<xsl:when test="$newStyle != ''">
				<xsl:variable name="contDig">
					<xsl:call-template name="containsDigit">
						<xsl:with-param name="str" select="$newStyle"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:choose>
					<!-- if this part is border-style-->
					<xsl:when test="contains($newStyle, 'outset') or contains($newStyle, 'solid') or 
													contains($newStyle, 'none') or contains($newStyle, 'double') or 
													contains($newStyle, 'hidden') or contains($newStyle, 'dashed') or 
													contains($newStyle, 'ridge') or contains($newStyle, 'groove') or contains($newStyle, 'inset')">
						<xsl:call-template name="IncludeBorderColor">
							<xsl:with-param name="style" select="$style"/>
							<xsl:with-param name="count" select="$count + 1"/>
						</xsl:call-template>
					</xsl:when>
					<xsl:when test="$contDig = 'true' and (starts-with($newStyle, '#') or contains($newStyle, 'rgb'))">
						<xsl:choose>
							<xsl:when test="starts-with($newStyle, '#') or contains($newStyle, 'rgb')">
								<xsl:value-of select="$newStyle"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="IncludeBorderColor">
									<xsl:with-param name="style" select="$style"/>
									<xsl:with-param name="count" select="$count + 1"/>
								</xsl:call-template>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:when>
					<xsl:otherwise>
						<xsl:call-template name="IncludeBorderColor">
							<xsl:with-param name="style" select="$style"/>
							<xsl:with-param name="count" select="$count + 1"/>
						</xsl:call-template>						
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>0</xsl:text>
			</xsl:otherwise>
		</xsl:choose>

	</xsl:template>


	<xsl:template name="ConvertBorderWidth">
		<xsl:param name="width"/>

		<xsl:variable name="Width">
			<xsl:call-template name="IncludeBorderWidth">
				<xsl:with-param name="style" select="$width"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="pointMeasure">
			<xsl:call-template name="ConvertMeasure">
				<xsl:with-param name="length" select="$Width"/>
				<xsl:with-param name="unit" select="'point'"/>
				<xsl:with-param name="round" select="false"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:value-of select="round($pointMeasure * 8)"/>
	</xsl:template>


	<xsl:template name="IncludeBorderWidth">
		<xsl:param name="style"/>
		<xsl:param name="count" select="0"/>

		<xsl:variable name="newStyle">
			<xsl:call-template name="GetCompositePropertyPart">
				<xsl:with-param name="property" select="$style"/>
				<xsl:with-param name="pos" select="$count"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:choose>
			<xsl:when test="$newStyle != ''">
				<xsl:variable name="contDig">
					<xsl:call-template name="containsDigit">
						<xsl:with-param name="str" select="$newStyle"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:choose>
					<xsl:when test="$contDig = 'true' and not(starts-with($newStyle, '#')) and not(contains($newStyle, 'rgb'))">
						<xsl:value-of select="$newStyle"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:call-template name="IncludeBorderWidth">
							<xsl:with-param name="style" select="$style"/>
							<xsl:with-param name="count" select="$count + 1"/>
						</xsl:call-template>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>0</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<xsl:template name="containsDigit">
		<xsl:param name="str"/>

		<xsl:choose>
			<xsl:when test="contains($str, '0') or contains($str, '1') or 
											contains($str, '2') or contains($str, '3') or 
											contains($str, '4') or contains($str, '5') or 
											contains($str, '6') or contains($str, '7') or 
											contains($str, '8') or contains($str, '9')">
				<xsl:text>true</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>false</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<xsl:template name="ConvertBorderStyle">
		<xsl:param name="style"/>

		<xsl:variable name="s">
			<xsl:call-template name="trim">
				<xsl:with-param name="s" select="$style"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:choose>
			<xsl:when test="$s = 'outset' or contains($s, 'outset')">
				<xsl:text>outset</xsl:text>
			</xsl:when>
			<xsl:when test="$s = 'solid' or contains($s, 'solid')">
				<xsl:text>single</xsl:text>
			</xsl:when>
			<xsl:when test="$s = 'none' or contains($s, 'none')">
				<xsl:text>none</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>auto</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<xsl:template name="GetPropertyFromStyle">
		<xsl:param name="styleString"/>
		<xsl:param name="propertyName"/>

		<xsl:variable name="propertyString">
			<xsl:choose>
				<!-- 'Semi-colon' is a separator -->
				<xsl:when test="contains( $styleString, concat( ';', $propertyName, ':' ) )">
					<xsl:value-of select="substring-after( $styleString, concat( ';', $propertyName, ':' ) )"/>
				</xsl:when>
				<!-- 'Tab' is a separator -->
				<xsl:when test="contains( $styleString, concat( '&#09;', $propertyName, ':' ) )">
					<xsl:value-of select="substring-after( $styleString, concat( '&#09;', $propertyName, ':' ) )"/>
				</xsl:when>
				<!-- 'Line feed' is a separator -->
				<xsl:when test="contains( $styleString, concat( '&#10;', $propertyName, ':' ) )">
					<xsl:value-of select="substring-after( $styleString, concat( '&#10;', $propertyName, ':' ) )"/>
				</xsl:when>
				<!-- 'Carriage return' is a separator -->
				<xsl:when test="contains( $styleString, concat( '&#13;', $propertyName, ':' ) )">
					<xsl:value-of select="substring-after( $styleString, concat( '&#13;', $propertyName, ':' ) )"/>
				</xsl:when>
				<!-- 'Space' is a separator -->
				<xsl:when test="contains( $styleString, concat( '&#32;', $propertyName, ':' ) )">
					<xsl:value-of select="substring-after( $styleString, concat( '&#32;', $propertyName, ':' ) )"/>
				</xsl:when>
				<!-- No separator - first entry -->
				<xsl:when test="starts-with( $styleString, concat( $propertyName, ':' ) )">
					<xsl:value-of select="substring-after( $styleString, concat( $propertyName, ':' ) )"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<!-- Get the last entry of a property! -->
		<xsl:variable name="nextValue">
			<xsl:if test="$propertyString != ''">
				<xsl:call-template name="GetPropertyFromStyle">
					<xsl:with-param name="styleString" select="$propertyString"/>
					<xsl:with-param name="propertyName" select="$propertyName"/>
				</xsl:call-template>
			</xsl:if>
		</xsl:variable>

		<xsl:variable name="propertyValue">
			<xsl:call-template name="trim">
				<xsl:with-param name="s">
					<xsl:choose>
						<xsl:when test="$nextValue != ''">
							<xsl:value-of select="$nextValue"/>
						</xsl:when>
						<xsl:when test="contains( $propertyString , ';' )">
							<xsl:value-of select="substring-before( $propertyString , ';' )"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="$propertyString"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="propertyRes">
			<xsl:choose>
				<xsl:when test="starts-with( $propertyValue, '&quot;' ) ">
					<xsl:value-of select="substring-before( substring-after( $propertyValue, '&quot;' ), '&quot;' )"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$propertyValue"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:call-template name="trim">
			<xsl:with-param name="s" select="$propertyRes"/>
		</xsl:call-template>
	</xsl:template>


	<xsl:template name="GetTagProperty">
		<xsl:param name="tag"/>
		<xsl:param name="propertyName"/>

		<xsl:call-template name="GetPropertyFromStyle">
			<xsl:with-param name="styleString" select="$tag/@style"/>
			<xsl:with-param name="propertyName" select="$propertyName"/>
		</xsl:call-template>
	</xsl:template>


	<xsl:template name="delSpaceInSkob">
		<xsl:param name="s"/>

		<xsl:choose>
			<xsl:when test="contains($s, '(') and contains($s, ')') and contains(substring-after(substring-before($s, ')'), '('), ' ')">
				<xsl:call-template name="delSpaceInSkob">
					<xsl:with-param name="s">
						<xsl:value-of select="substring-before($s, '(')"/>
						<xsl:text>(</xsl:text>
						<xsl:call-template name="delSpace">
							<xsl:with-param name="s" select="substring-after(substring-before($s, ')'), '(')"/>
						</xsl:call-template>
						<xsl:text>)</xsl:text>
						<xsl:value-of select="substring-after($s, ')')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$s"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<xsl:template name="delSpace">
		<xsl:param name="s"/>

		<xsl:choose>
			<xsl:when test="contains($s, ' ')">
				<xsl:call-template name="delSpace">
					<xsl:with-param name="s" select="concat(substring-before($s, ' '), substring-after($s, ' '))"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$s"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<xsl:template name="GetCompositePropertyPart">
		<xsl:param name="property"/>
		<xsl:param name="pos"/>

		<xsl:variable name="value">
			<xsl:call-template name="trim">
				<xsl:with-param name="s" select="$property"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:variable name="s">
			<xsl:call-template name="delSpaceInSkob">
				<xsl:with-param name="s" select="$value"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="$pos &lt;= '0'">
				<xsl:choose>
					<xsl:when test="contains( $s, ' ' )">
						<xsl:value-of select="substring-before( $s, ' ' )"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="$s"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:otherwise>
				<xsl:call-template name="GetCompositePropertyPart">
					<xsl:with-param name="property" select="substring-after( $s, ' ' )"/>
					<xsl:with-param name="pos" select="$pos - 1"/>
				</xsl:call-template>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<xsl:template name="CountCompositePropertyParts">
		<xsl:param name="property"/>

		<xsl:variable name="value">
			<xsl:call-template name="trim">
				<xsl:with-param name="s" select="$property"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:choose>
			<xsl:when test="contains( $value, ' ' )">
				<xsl:variable name="tmp">
					<xsl:call-template name="CountCompositePropertyParts">
						<xsl:with-param name="property" select="substring-after( $value, ' ' )"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:value-of select="$tmp + 1"/>
			</xsl:when>
			<xsl:when test="$value != ''">
				<xsl:text>1</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>0</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

  <!-- <xsl:template name="LastWord">
       Ищем последнее слово в последовательности слов, разделенных 
       единичными пробелами
  -->
  <xsl:template name="LastWord">
    <xsl:param name="String"/>

    <xsl:choose>
      <xsl:when test="contains($String, ' ')">
        <xsl:call-template name="LastWord">
          <xsl:with-param name="String" select="substring-after($String, ' ')" />
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$String"/>
      </xsl:otherwise>
    </xsl:choose>
    
  </xsl:template>


  <!-- <xsl:template name="CheckCurElementStyle">
       Проверяем совпадуют ли определенные Тэг и Класс с одним из списка.
  -->
  <xsl:template name="CheckCurElementStyle">
    <xsl:param name="TagClassString" />
    <xsl:param name="Tag" />
    <xsl:param name="Class" />
    <xsl:param name="ID"/>

    <xsl:variable name="CurString">
      
      <xsl:variable name="Temp1">

        <xsl:choose>
          <xsl:when test="contains($TagClassString, ',')">
            <xsl:value-of select="normalize-space(substring-before($TagClassString, ','))"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="normalize-space($TagClassString)"/>
          </xsl:otherwise>
        </xsl:choose>
        
      </xsl:variable>

      <xsl:call-template name="LastWord">
        <xsl:with-param name="String" select="$Temp1" />
      </xsl:call-template>
      
    </xsl:variable>

    <xsl:choose>

      <xsl:when test="$ID != ''">

        <xsl:choose>
          <xsl:when test="contains($CurString, '#') and ( $ID = substring-after($CurString, '#') )">
            <xsl:text>true</xsl:text>
          </xsl:when>
          <xsl:when test="contains($TagClassString, ',')">
            <xsl:call-template name="CheckCurElementStyle">
              <xsl:with-param name="TagClassString" select="substring-after( $TagClassString, ',' )"/>
              <xsl:with-param name="Tag" select="$Tag"/>
              <xsl:with-param name="Class" select="$Class"/>
              <xsl:with-param name="ID" select="$ID" />
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>false</xsl:text>
          </xsl:otherwise>
        </xsl:choose>

      </xsl:when>

      <xsl:otherwise>

        <xsl:variable name="CurTag">
          <xsl:choose>
            <xsl:when test="contains($CurString, '.')">
              <xsl:value-of select="substring-before($CurString, '.')" />
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$CurString"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        <xsl:variable name="CurClass">
          <xsl:choose>
            <xsl:when test="contains($CurString, '.')">
              <xsl:value-of select="substring-after($CurString, '.')" />
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="''"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>

        <xsl:choose>
          <xsl:when test="( $Class = $CurClass ) and ( $Tag = $CurTag )">
            <xsl:text>true</xsl:text>
          </xsl:when>
          <xsl:when test="contains($TagClassString, ',')">
            <xsl:call-template name="CheckCurElementStyle">
              <xsl:with-param name="TagClassString" select="substring-after( $TagClassString, ',' )"/>
              <xsl:with-param name="Tag" select="$Tag"/>
              <xsl:with-param name="Class" select="$Class"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>false</xsl:text>
          </xsl:otherwise>
        </xsl:choose>

      </xsl:otherwise>

    </xsl:choose>
      
  </xsl:template>

  
	<xsl:template name="GetCurElementStyle">
		<xsl:param name="cssString"/>
		<xsl:param name="tag"/>
		<xsl:param name="class"/>
    <xsl:param name="ID"/>

    <xsl:if test="contains($cssString, '{')">

      <xsl:choose>

        <xsl:when test="$ID != ''">

          <xsl:variable name="CheckCurStyle">
            <xsl:call-template name="CheckCurElementStyle">
              <xsl:with-param name="TagClassString" select="substring-before( $cssString, '{' )"/>
              <xsl:with-param name="Tag" select="$tag" />
              <xsl:with-param name="ID" select="$ID" />
            </xsl:call-template>
          </xsl:variable>

          <xsl:choose>
            <xsl:when test="$CheckCurStyle = 'true'">
              <xsl:value-of select="substring-before( substring-after( $cssString, '{' ), '}' )"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:call-template name="GetCurElementStyle">
                <xsl:with-param name="cssString" select="substring-after( $cssString, '}' )"/>
                <xsl:with-param name="tag" select="$tag"/>
                <xsl:with-param name="class" select="$class"/>
                <xsl:with-param name="ID" select="$ID"/>
              </xsl:call-template>
            </xsl:otherwise>
          </xsl:choose>

        </xsl:when>

        <xsl:otherwise>

          <xsl:variable name="CheckCurStyle">
            <xsl:call-template name="CheckCurElementStyle">
              <xsl:with-param name="TagClassString" select="substring-before( $cssString, '{' )"/>
              <xsl:with-param name="Tag" select="$tag" />
              <xsl:with-param name="Class" select="$class" />
            </xsl:call-template>
          </xsl:variable>

          <xsl:choose>
            <xsl:when test="$CheckCurStyle = 'true'">
              <xsl:value-of select="substring-before( substring-after( $cssString, '{' ), '}' )"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:call-template name="GetCurElementStyle">
                <xsl:with-param name="cssString" select="substring-after( $cssString, '}' )"/>
                <xsl:with-param name="tag" select="$tag"/>
                <xsl:with-param name="class" select="$class"/>
              </xsl:call-template>
            </xsl:otherwise>
          </xsl:choose>

        </xsl:otherwise>

      </xsl:choose>

    </xsl:if>
    
    
		<!--<xsl:variable name="elementString" select="substring-before( $cssString, '}' )"/>

		<xsl:variable name="curClass">
			<xsl:call-template name="trim">
				<xsl:with-param name="s" select="substring-after(substring-before($elementString,'{'),'.')"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:variable name="curTag">
			<xsl:choose>
				<xsl:when test="$curClass != ''">
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

		<xsl:choose>
			<xsl:when test="( $class = $curClass ) and ( $tag = $curTag )">
				<xsl:value-of select="substring-after( $elementString, '{' )"/>
			</xsl:when>
			<xsl:when test="$cssString != ''">
				<xsl:call-template name="GetCurElementStyle">
					<xsl:with-param name="cssString" select="substring-after( $cssString, '}' )"/>
					<xsl:with-param name="tag" select="$tag"/>
					<xsl:with-param name="class" select="$class"/>
				</xsl:call-template>
			</xsl:when>
		</xsl:choose>-->
	</xsl:template>


	<xsl:template name="GetElementStyle">
		<xsl:param name="tag"/>
		<xsl:param name="class"/>
    <xsl:param name="ID"/>

		<xsl:variable name="cssString">
			<xsl:value-of select="translate(/html/head/style/text(), 'ABCDEFGHIJKLMNOPQRSTUVWXYZ', 'abcdefghijklmnopqrstuvwxyz')"/>
		</xsl:variable>

    <xsl:variable name="Tag">
      <xsl:value-of select="translate($tag, 'ABCDEFGHIJKLMNOPQRSTUVWXYZ', 'abcdefghijklmnopqrstuvwxyz')"/>
    </xsl:variable>

    <xsl:variable name="Class">
      <xsl:value-of select="translate($class, 'ABCDEFGHIJKLMNOPQRSTUVWXYZ', 'abcdefghijklmnopqrstuvwxyz')"/>
    </xsl:variable>

    <xsl:variable name="Id">
      <xsl:value-of select="translate($ID, 'ABCDEFGHIJKLMNOPQRSTUVWXYZ', 'abcdefghijklmnopqrstuvwxyz')"/>
    </xsl:variable>

    <xsl:variable name="tagStyle">
			<xsl:call-template name="GetCurElementStyle">
				<xsl:with-param name="cssString" select="$cssString"/>
				<xsl:with-param name="tag" select="$Tag"/>
			</xsl:call-template>
			<xsl:text>;</xsl:text>
		</xsl:variable>

		<xsl:variable name="classStyle">
			<xsl:if test="$class != ''">
				<xsl:call-template name="GetCurElementStyle">
					<xsl:with-param name="cssString" select="$cssString"/>
					<xsl:with-param name="class" select="$Class"/>
				</xsl:call-template>
				<xsl:text>;</xsl:text>
			</xsl:if>
		</xsl:variable>

		<xsl:variable name="specStyle">
			<xsl:if test="$class != ''">
				<xsl:call-template name="GetCurElementStyle">
					<xsl:with-param name="cssString" select="$cssString"/>
					<xsl:with-param name="tag" select="$Tag"/>
					<xsl:with-param name="class" select="$Class"/>
				</xsl:call-template>
				<xsl:text>;</xsl:text>
			</xsl:if>
		</xsl:variable>

    <xsl:variable name="IDStyle">
      <xsl:if test="$ID != ''">
        <xsl:call-template name="GetCurElementStyle">
          <xsl:with-param name="cssString" select="$cssString"/>
          <xsl:with-param name="ID" select="$Id"/>
        </xsl:call-template>
        <xsl:text>;</xsl:text>
      </xsl:if>
    </xsl:variable>


    <xsl:value-of select="concat( $tagStyle, $classStyle, $specStyle, $IDStyle )"/>
	</xsl:template>

  <!--<xsl:template name="setTableGrid">
    <xsl:param name="node"/>
    <xsl:for-each select="$node//col">
      <xsl:call-template name="setCols">
        <xsl:with-param name="node" select="."/>
      </xsl:call-template>
    </xsl:for-each>
  </xsl:template>-->

  <xsl:template name="setTableGrid">
		<xsl:param name="node"/>

    <xsl:variable name="widthTable">
      <xsl:call-template name="getWidthTable">
        <xsl:with-param name="node" select="$node"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="marginLeft">
      <xsl:call-template name="GetPropertyFromStyle">
        <xsl:with-param name="styleString" select="@style"/>
        <xsl:with-param name="propertyName" select="'margin-left'"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="marginRight">
      <xsl:call-template name="GetPropertyFromStyle">
        <xsl:with-param name="styleString" select="@style"/>
        <xsl:with-param name="propertyName" select="'margin-rigth'"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="marginLeftTwips">
      <xsl:choose>
        <xsl:when test="$marginLeft != ''">
          <xsl:call-template name="ConvertMeasure">
            <xsl:with-param name="length" select="$marginLeft"/>
            <xsl:with-param name="unit" select="'twips'"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="1701"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="marginRightTwips">
      <xsl:choose>
        <xsl:when test="$marginRight != ''">
          <xsl:call-template name="ConvertMeasure">
            <xsl:with-param name="length" select="$marginRight"/>
            <xsl:with-param name="unit" select="'twips'"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="850"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    
    <xsl:variable name="widthPage">
      <xsl:value-of select="11906 - ($marginRightTwips + $marginLeftTwips)"/>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="$node//tr[1]//th != '' and $node//tr[1]//td != ''">
        <xsl:for-each select="$node//tr[1]/*">
          <xsl:variable name="curPosition">
            <xsl:value-of select="position()"/>
          </xsl:variable>
          <xsl:variable name="maxWidth">
            <xsl:call-template name="getMaxWidthColumns">
              <xsl:with-param name="list" select="$node//tr//child::*[position()= $curPosition]"/>
            </xsl:call-template>
          </xsl:variable>
          <xsl:variable name="trueWidth">
            <xsl:value-of select="(($maxWidth) div ($widthTable))*($widthPage)"/>
          </xsl:variable>
          <w:gridCol>
            <xsl:attribute name="w:w">
              <xsl:value-of select="round($trueWidth)"/>
            </xsl:attribute>
          </w:gridCol>
        </xsl:for-each>
      </xsl:when>
      <xsl:when test="$node//tr[1]//th != ''">
        <xsl:for-each select="$node//tr[1]//th">
          <xsl:variable name="curPosition">
            <xsl:value-of select="position()"/>
          </xsl:variable>
          <xsl:variable name="maxWidth">
            <xsl:call-template name="getMaxWidthColumns">
              <xsl:with-param name="list" select="$node//tr//child::*[position()= $curPosition]"/>
            </xsl:call-template>
          </xsl:variable>
          <xsl:variable name="trueWidth">
            <xsl:value-of select="(($maxWidth) div ($widthTable))*($widthPage)"/>
          </xsl:variable>
          <w:gridCol>
            <xsl:attribute name="w:w">
              <xsl:value-of select="round($trueWidth)"/>
            </xsl:attribute>
          </w:gridCol>
        </xsl:for-each>
      </xsl:when>
      <xsl:otherwise>
        <xsl:for-each select="$node//tr[1]//td">
          <xsl:variable name="curPosition">
            <xsl:value-of select="position()"/>
          </xsl:variable>
          <xsl:variable name="maxWidth">
            <xsl:call-template name="getMaxWidthColumns">
              <xsl:with-param name="list" select="$node//tr//child::*[position()= $curPosition]"/>
            </xsl:call-template>
          </xsl:variable>
          <xsl:variable name="trueWidth">
            <xsl:value-of select="(($maxWidth) div ($widthTable))*($widthPage)"/>
          </xsl:variable>
          <w:gridCol>
            <xsl:attribute name="w:w">
              <xsl:value-of select="round($trueWidth)"/>
            </xsl:attribute>
          </w:gridCol>
        </xsl:for-each>
      </xsl:otherwise>
    </xsl:choose> 
	</xsl:template>

  <xsl:template name="getWidthTable">
    <xsl:param name="node"/>
    <xsl:variable name="widthTable">
      <xsl:choose>
        <xsl:when test="$node//tr[1]//th != ''">
          <xsl:call-template name="for">
            <xsl:with-param name="count" select="count($node//tr[1]//th)"/>
            <xsl:with-param name="node" select="$node"/>
            <xsl:with-param name="allWidth" select="0"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:call-template name="for">
            <xsl:with-param name="count" select="count($node//tr[1]//td)"/>
            <xsl:with-param name="node" select="$node"/>
            <xsl:with-param name="allWidth" select="0"/>
          </xsl:call-template>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:value-of select="$widthTable"/>
  </xsl:template>

  <xsl:template name="for">
    <xsl:param name="i" select="1" />
    <xsl:param name="count" />
    <xsl:param name="node" />
    <xsl:param name="allWidth"/>
    <xsl:variable name="maxWidth">
      <xsl:call-template name="getMaxWidthColumns">
        <xsl:with-param name="list" select="current()//tr//child::*[position()= $i]"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="$i &lt;= ($count)">
        <xsl:call-template name="for">
          <xsl:with-param name="i" select="$i + 1"/>
          <xsl:with-param name="count" select="$count"/>
          <xsl:with-param name="allWidth" select="$allWidth + $maxWidth"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$allWidth"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  
  <!--get width table cell(real width in style attribute or width of word)-->
  <xsl:template name ="getMaxWidthColumns">
    <xsl:param name ="list" />
    <xsl:param name ="prevVal" />
    <xsl:param name ="prevStyleWidth" />

    <xsl:variable name="style">
      <xsl:value-of select="$list/@style"/>
    </xsl:variable>
    
    <xsl:variable name="fontSize">
      <xsl:call-template name="getFontSize">
        <xsl:with-param name="style" select="$style"/>
        <xsl:with-param name="isDefaultSize" select="'true'"/>
      </xsl:call-template>
    </xsl:variable>
    
    <xsl:variable name="text">
      <xsl:value-of select="normalize-space($list/child::text())"/>
    </xsl:variable>

    <!--<xsl:variable name="text">
      <xsl:value-of select="normalize-space($list)"/>
    </xsl:variable>-->

    <xsl:variable name="fontSize2">
      <xsl:call-template name="ConvertMeasure">
        <xsl:with-param name="length" select="concat($fontSize*0.6, 'px')"/>
        <xsl:with-param name="unit" select="'twips'"/>
      </xsl:call-template>
    </xsl:variable>
    
    <xsl:variable name="lengthText">
      <xsl:value-of select="(string-length($text))*($fontSize2)"/>
    </xsl:variable>

    <xsl:variable name="widthTd">
      <xsl:call-template name="GetPropertyFromStyle">
        <xsl:with-param name="styleString" select="$style"/>
        <xsl:with-param name="propertyName" select="'width'"/>
      </xsl:call-template>
    </xsl:variable>
    
    <xsl:variable name="widthCell">
      <xsl:choose>
        <xsl:when test="$widthTd != ''">
          <xsl:call-template name="ConvertMeasure">
            <xsl:with-param name="length" select="$widthTd"/>
            <xsl:with-param name="unit" select="'twips'"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="0"/>
        </xsl:otherwise>
     </xsl:choose>
    </xsl:variable>
    
    <xsl:variable name="lengthTextNew">
      <xsl:choose>
          <xsl:when test="($prevStyleWidth != '') and ($prevStyleWidth != 0) and ($widthCell != 0) and ($widthCell > $prevStyleWidth)">
            <xsl:value-of select="$widthCell"/>
          </xsl:when>
          <xsl:when test="($widthCell != '') and ($widthCell != 0)">
            <xsl:value-of select="$widthCell"/>
          </xsl:when>
          <xsl:when test="($prevVal = '') or (($lengthText > $prevVal) and ($prevVal != ''))">
              <xsl:value-of select="$lengthText"/>
          </xsl:when>
          <xsl:otherwise>
              <xsl:value-of select="$prevVal"/>
          </xsl:otherwise>
        </xsl:choose>
    </xsl:variable>

    <xsl:variable name ="rest">   
      <xsl:choose>
          <xsl:when test="count($list) != 0">
              <xsl:call-template name ="getMaxWidthColumns">
                <xsl:with-param name ="list" select ="$list[position() != 1]" />
                <xsl:with-param name ="prevVal" select ="$lengthTextNew" />
                <xsl:with-param name ="prevStyleWidth" select ="$widthCell" />
              </xsl:call-template>
          </xsl:when>
          <xsl:when test="count($list) = 0">
              <xsl:value-of select="$lengthTextNew"/>
          </xsl:when>
        </xsl:choose>
    </xsl:variable>

    <xsl:value-of select="$rest"/>
  </xsl:template>
  
  
 	<xsl:template name="setCols">
		<xsl:param name="node"/>

		<xsl:for-each select="$node">
			<xsl:if test="@width != ''">
				<xsl:variable name="width">
					<xsl:call-template name="ConvertMeasure">
						<xsl:with-param name="length" select="@width"/>
						<xsl:with-param name="unit" select="'twips'"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:variable name="span">
					<xsl:choose>
						<xsl:when test="@span">
							<xsl:value-of select="@span"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>1</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>
				<xsl:call-template name="setCol">
					<xsl:with-param name="width" select="@width"/>
					<xsl:with-param name="count" select="$span"/>
				</xsl:call-template>
			</xsl:if>
		</xsl:for-each>
	</xsl:template>

	<xsl:template name="setCol">
		<xsl:param name="width"/>
		<xsl:param name="count"/>

		<xsl:if test="$count > 0">
			<w:gridCol>
				<xsl:attribute name="w:w">
					<xsl:value-of select="round($width * 1440 div 96)"/>
				</xsl:attribute>
			</w:gridCol>

			<xsl:call-template name="setCol">
				<xsl:with-param name="width" select="$width"/>
				<xsl:with-param name="count" select="$count - 1"/>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

  <!-- <xsl:template name="InsertParaEffects">
       Данный тимплэт вставляет настройки для показа данного параграфа,
       например, visibility:hidden, значит содержимое параграфа должно быть
       невидимым.
  -->  
  <xsl:template name="InsertParaEffects">
    <xsl:param name="styleString"/>

    <xsl:if test="contains($styleString, 'visibility:hidden')">
      <xsl:element name="w14:textFill">
        <xsl:element name="w14:noFill" />
      </xsl:element>
    </xsl:if>
      
  </xsl:template>

  <!-- <xsl:template name="InsertDisplayProps">
       Данный тимплэт вставляет настройки для показа данного параграфа,
       например, display:none, значит параграф должен быть скрыт.
  -->
  <xsl:template name="InsertDisplayProps">
    <xsl:param name="styleString"/>

    <xsl:if test="contains($styleString, 'display:none')">
      <xsl:element name="w:vanish"/>
    </xsl:if>
    
  </xsl:template>

  <!-- <xsl:template name="InsertPageBreakBefore">
       Данный тимплэт вставляет разрыв страницы после параграфа.
  -->
  <xsl:template name="InsertPageBreakBefore">
    <xsl:param name="styleString"/>

    <xsl:variable name="PageBreak">
      <xsl:call-template name="GetPropertyFromStyle">
        <xsl:with-param name="styleString" select="$styleString"/>
        <xsl:with-param name="propertyName" select="'page-break-after'"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:if test="contains($PageBreak, 'always')">
      <xsl:element name="w:pageBreakBefore"/>
    </xsl:if>

  </xsl:template>

</xsl:stylesheet>
