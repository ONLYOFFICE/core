<?xml version="1.0" encoding="UTF-8" ?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:ve="http://schemas.openxmlformats.org/markup-compatibility/2006"
                xmlns:o="urn:schemas-microsoft-com:office:office"
                xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
                xmlns:m="http://schemas.openxmlformats.org/officeDocument/2006/math"
                xmlns:v="urn:schemas-microsoft-com:vml"
                xmlns:wp="http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing"
                xmlns:w10="urn:schemas-microsoft-com:office:word"
                xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"
                xmlns:wne="http://schemas.microsoft.com/office/word/2006/wordml">

	<xsl:import href="2oox-common.xsl"/>
	<xsl:import href="2oox-styles-common.xsl"/>

	<xsl:output method="xml" encoding="UTF-8"/>
	<xsl:strip-space elements="*" />

	<xsl:template match="/html">
		<xsl:message>Progress: html</xsl:message>
		<w:numbering>
			<xsl:apply-templates select="./body//ol |./body//ul"/>

			<xsl:variable name="count">
				<xsl:value-of select="count((./body//ol |./body//ul)[count(ancestor::ol | ancestor::ul) = 0])"/>
			</xsl:variable>

			<!--Пробегаем по всем спискам-->
			<xsl:for-each select="(./body//ol |./body//ul)[count(ancestor::ol | ancestor::ul) = 0]">
				<xsl:variable name="id">
					<xsl:call-template name="getListNumId">
						<xsl:with-param name="node" select="."/>
					</xsl:call-template>
				</xsl:variable>

				<xsl:call-template name="num">
					<xsl:with-param name="id" select="$id"/>
				</xsl:call-template>
			</xsl:for-each>
		</w:numbering>
	</xsl:template>


	<xsl:template match="ul | ol">
		<xsl:if test="count(ancestor::ol | ancestor::ul) = 0">
			<xsl:variable name="id">
				<xsl:call-template name="getListNumId">
					<xsl:with-param name="node" select="."/>
				</xsl:call-template>
			</xsl:variable>
			<xsl:call-template name="abstractNum">
				<xsl:with-param name="id" select="$id"/>
				<xsl:with-param name="node" select="."/>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<!--Создание настроек списка-->
	<xsl:template name="abstractNum">
		<xsl:param name="id"/>
		<xsl:param name="node" select="."/>
		<w:abstractNum>
			<xsl:attribute name="w:abstractNumId">
				<xsl:value-of select="number($id) - 1"/>
			</xsl:attribute>
			<w:nsid>
				<xsl:attribute name="w:val">
					<xsl:text>039E76A0</xsl:text>
					<xsl:value-of select="$id"/>
				</xsl:attribute>
			</w:nsid>
			<w:multiLevelType w:val="hybridMultilevel" />
			<w:tmpl>
				<xsl:attribute name="w:val">
					<xsl:text>562E76E2</xsl:text>
					<xsl:value-of select="$id"/>
				</xsl:attribute>
			</w:tmpl>

			<xsl:for-each select="descendant-or-self::ol | descendant-or-self::ul">

				<xsl:variable name="genericStyle">
					<xsl:call-template name="GetElementStyle">
						<xsl:with-param name="tag" select="name()"/>
						<xsl:with-param name="class" select="@class"/>
            <xsl:with-param name="ID" select="@id" />
					</xsl:call-template>
				</xsl:variable>

				<xsl:variable name="format">
					<xsl:if test="@type != ''">
						<xsl:text>list-style-type:</xsl:text>
						<xsl:choose>
							<xsl:when test="name() = 'ol'">
								<xsl:choose>
									<xsl:when test="@type = 'A' or 'UPPER-ALPHA' = translate(@type, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ')">
										<xsl:text>upperLetter</xsl:text>
									</xsl:when>
									<xsl:when test="@type = 'a' or 'LOWER-ALPHA' = translate(@type, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ')">
										<xsl:text>lowerLetter</xsl:text>
									</xsl:when>
									<xsl:when test="@type = 'I' or 'UPPER-ROMAN' = translate(@type, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ')">
										<xsl:text>upperRoman</xsl:text>
									</xsl:when>
									<xsl:when test="@type = 'i' or 'LOWER-ROMAN' = translate(@type, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ')">
										<xsl:text>lowerRoman</xsl:text>
									</xsl:when>
									<xsl:when test="@type = '1' or 'DECIMAL' = translate(@type, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ')">
										<xsl:text>decimal</xsl:text>
									</xsl:when>
								</xsl:choose>
							</xsl:when>
							<xsl:when test="name() = 'ul'">
								<xsl:choose>
									<xsl:when test="'DISC'   = translate(@type, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ')">
										<xsl:text></xsl:text>
									</xsl:when>
									<xsl:when test="'CIRCLE' = translate(@type, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ')">
										<xsl:text>o</xsl:text>
									</xsl:when>
									<xsl:when test="'SUQARE' = translate(@type, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ')">
										<xsl:text></xsl:text>
									</xsl:when>
								</xsl:choose>
							</xsl:when>
						</xsl:choose>
						<xsl:text>;</xsl:text>
					</xsl:if>
				</xsl:variable>

				<xsl:variable name="start">
					<xsl:if test="@start != ''">
						<xsl:value-of select="concat('start:', @start, ';')"/>
					</xsl:if>
				</xsl:variable>

				<xsl:call-template name="setListLevel">
					<xsl:with-param name="styleString" select="concat($genericStyle, ';', $format, $start)"/>
					<xsl:with-param name="level">
						<xsl:call-template name="getListLevel">
							<xsl:with-param name="node" select="."/>
						</xsl:call-template>
					</xsl:with-param>
					<xsl:with-param name="type" select="name()"/>
				</xsl:call-template>
			</xsl:for-each>

		</w:abstractNum>
	</xsl:template>

	<!--Создание связующего звена-->
	<xsl:template name="num">
		<xsl:param name="id"/>
		<w:num>
			<xsl:attribute name="w:numId">
				<xsl:value-of select="$id"/>
			</xsl:attribute>
			<w:abstractNumId>
				<xsl:attribute name="w:val">
					<xsl:value-of select="number($id) - 1"/>
				</xsl:attribute>
			</w:abstractNumId>
		</w:num>
	</xsl:template>

	<!--Создание настроек уровня списка-->
	<xsl:template name="setListLevel">
		<xsl:param name="styleString"/>
		<xsl:param name="level"/>
		<xsl:param name="type"/>

		<w:lvl>
			<xsl:attribute name="w:ilvl">
				<xsl:value-of select="number($level) + 1"/>
			</xsl:attribute>

			<xsl:if test="number($level) + 1 != 0">
				<xsl:attribute name="w:tentative">
					<xsl:text>1</xsl:text>
				</xsl:attribute>
			</xsl:if>

			<xsl:attribute name="w:tplc">
				<xsl:text>039E76A0</xsl:text>
				<xsl:value-of select="number($level) + 1"/>
			</xsl:attribute>

			<xsl:variable name="start">
				<xsl:call-template name="GetPropertyFromStyle">
					<xsl:with-param name="styleString" select="concat($styleString, @style, ./li/@style)"/>
					<xsl:with-param name="propertyName" select="'start'"/>
				</xsl:call-template>
			</xsl:variable>

			<w:start>
				<xsl:attribute name="w:val">
					<xsl:choose>
						<xsl:when test="$start != ''">
							<xsl:value-of select="$start"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>1</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>
			</w:start>

			<xsl:variable name="format">
				<xsl:variable name="fmt">
					<xsl:call-template name="GetPropertyFromStyle">
						<xsl:with-param name="styleString" select="concat($styleString, @style, ./li/@style)"/>
						<xsl:with-param name="propertyName" select="'list-style-type'"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:choose>
					<xsl:when test="$fmt = ''">
						<xsl:call-template name="GetPropertyFromStyle">
							<xsl:with-param name="styleString" select="concat($styleString, @style, ./li/@style)"/>
							<xsl:with-param name="propertyName" select="'list-style'"/>
						</xsl:call-template>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="$fmt"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>

			<w:numFmt>
				<xsl:attribute name="w:val">
					<xsl:choose>
						<xsl:when test="$type = 'ol'">
							<xsl:choose>
                <xsl:when test="contains(translate($format, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'), 'UPPER-ALPHA')">
                  <xsl:text>upperLetter</xsl:text>
                </xsl:when>
                <xsl:when test="contains(translate($format, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'), 'LOWER-ALPHA')">
                  <xsl:text>lowerLetter</xsl:text>
                </xsl:when>
                <xsl:when test="contains(translate($format, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'), 'UPPER-ROMAN')">
                  <xsl:text>upperRoman</xsl:text>
                </xsl:when>
                <xsl:when test="contains(translate($format, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'), 'LOWER-ROMAN')">
                  <xsl:text>lowerRoman</xsl:text>
                </xsl:when>
                <xsl:when test="contains(translate($format, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'), 'DECIMAL')">
                  <xsl:text>decimal</xsl:text>
                </xsl:when>
                <xsl:when test="contains(translate($format, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'), 'NONE')">
                  <xsl:text>none</xsl:text>
                </xsl:when>
								<xsl:otherwise>
									<xsl:text>decimal</xsl:text>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:when>
						<xsl:when test="$type = 'ul'">
							<xsl:variable name="isBullet">
								<xsl:call-template name="isBulletFormat">
									<xsl:with-param name="format" select="$format"/>
								</xsl:call-template>
							</xsl:variable>
							<xsl:choose>
								<xsl:when test="$isBullet = 'true'">
									<xsl:text>bullet</xsl:text>
								</xsl:when>
								<xsl:otherwise>
									<xsl:choose>
										<xsl:when test="contains(translate($format, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'), 'UPPER-ALPHA')">
											<xsl:text>upperLetter</xsl:text>
										</xsl:when>
										<xsl:when test="contains(translate($format, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'), 'LOWER-ALPHA')">
											<xsl:text>lowerLetter</xsl:text>
										</xsl:when>
										<xsl:when test="contains(translate($format, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'), 'UPPER-ROMAN')">
											<xsl:text>upperRoman</xsl:text>
										</xsl:when>
										<xsl:when test="contains(translate($format, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'), 'LOWER-ROMAN')">
											<xsl:text>lowerRoman</xsl:text>
										</xsl:when>
										<xsl:when test="contains(translate($format, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'), 'DECIMAL')">
											<xsl:text>decimal</xsl:text>
										</xsl:when>
										<xsl:when test="contains(translate($format, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'), 'NONE')">
											<xsl:text>none</xsl:text>
										</xsl:when>
									</xsl:choose>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:when>
					</xsl:choose>
				</xsl:attribute>
			</w:numFmt>


			<xsl:variable name="val">
				<xsl:choose>
					<xsl:when test="$type = 'ol'">
						<xsl:choose>
							<xsl:when test="contains(translate($format, 'abcdefghijklmnopqrstuvwxyz','ABCDEFGHIJKLMNOPQRSTUVWXYZ'), 'NONE')">
								<xsl:text></xsl:text>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="concat('%', number($level) + 2, '.')"/>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:when>
					<xsl:when test="$type = 'ul'">
						<xsl:variable name="isBullet">
							<xsl:call-template name="isBulletFormat">
								<xsl:with-param name="format" select="$format"/>
							</xsl:call-template>
						</xsl:variable>
						<xsl:choose>
							<xsl:when test="$isBullet = 'true'">
								<xsl:choose>
									<xsl:when test="contains(translate($format, 'abcdefghijklmnopqrstuvwxyz', 
'ABCDEFGHIJKLMNOPQRSTUVWXYZ'), 'DISC')">
										<xsl:text></xsl:text>
									</xsl:when>
									<xsl:when test="contains(translate($format, 'abcdefghijklmnopqrstuvwxyz', 
'ABCDEFGHIJKLMNOPQRSTUVWXYZ'), 'CIRCLE')">
										<xsl:text>o</xsl:text>
									</xsl:when>
									<xsl:when test="contains(translate($format, 'abcdefghijklmnopqrstuvwxyz', 
'ABCDEFGHIJKLMNOPQRSTUVWXYZ'), 'SUQARE')">
										<xsl:text></xsl:text>
									</xsl:when>
									<xsl:otherwise>
										<xsl:text></xsl:text>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:when>
							<xsl:otherwise>
								<xsl:choose>
									<xsl:when test="contains(translate($format, 'abcdefghijklmnopqrstuvwxyz', 
'ABCDEFGHIJKLMNOPQRSTUVWXYZ'), 'NONE')">
										<xsl:text></xsl:text>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select="concat('%', number($level) + 2, '.')"/>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:when>
				</xsl:choose>
			</xsl:variable>
			<w:lvlText>
				<xsl:attribute name="w:val">
					<xsl:value-of select="$val"/>
				</xsl:attribute>
			</w:lvlText>

			<w:lvlJc>
				<xsl:attribute name="w:val">
					<xsl:text>right</xsl:text>
				</xsl:attribute>
			</w:lvlJc>

			<w:pPr>
				<w:ind>
					<xsl:attribute name="w:left">
						<xsl:value-of select="(number($level) + 2) * 720 + 720"/>
					</xsl:attribute>
					<xsl:attribute name="w:hanging">
						<xsl:text>360</xsl:text>
					</xsl:attribute>
				</w:ind>
			</w:pPr>
			<xsl:variable name="isBullet">
				<xsl:call-template name="isBulletFormat">
					<xsl:with-param name="format" select="$format"/>
				</xsl:call-template>
			</xsl:variable>
			<xsl:choose>
				<xsl:when test="$val = ''">
					<w:rPr>
						<w:rFonts w:ascii="Symbol" w:hAnsi="Symbol" w:hint="default"/>
					</w:rPr>
				</xsl:when>
				<xsl:when test="$val = 'o'">
					<w:rPr>
						<w:rFonts w:ascii="Courier New" w:hAnsi="Courier New" w:cs="Courier New" w:hint="default"/>
					</w:rPr>
				</xsl:when>
				<xsl:when test="$val = ''">
					<w:rPr>
						<w:rFonts w:ascii="Wingdings" w:hAnsi="Wingdings" w:hint="default"/>
					</w:rPr>
				</xsl:when>
			</xsl:choose>
		</w:lvl>
	</xsl:template>


	<xsl:template name="isBulletFormat">
		<xsl:param name="format"/>

		<xsl:choose>
			<xsl:when test="$format!= ''">
				<xsl:choose>
					<xsl:when test="contains(translate($format, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'), 'UPPER-ALPHA')
                       or contains(translate($format, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'), 'LOWER-ALPHA')
                       or contains(translate($format, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'), 'UPPER-ROMAN')
                       or contains(translate($format, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'), 'LOWER-ROMAN')
                       or contains(translate($format, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'), 'DECIMAL')
                       or contains(translate($format, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'), 'NONE')">
					</xsl:when>
					<xsl:otherwise>
						<xsl:text>true</xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>true</xsl:text>
			</xsl:otherwise>
		</xsl:choose>

	</xsl:template>


</xsl:stylesheet>