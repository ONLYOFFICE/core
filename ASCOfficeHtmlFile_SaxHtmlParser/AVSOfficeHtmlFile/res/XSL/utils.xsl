<?xml version="1.0" encoding="UTF-8"?>
<?xml-stylesheet href="utils.xsl" type="text/xsl" media="screen"?>

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

	<xsl:template name="substring-after-last">
		<xsl:param name="string" />
		<xsl:param name="occurrence" />

		<xsl:choose>
			<xsl:when test="contains($string, $occurrence)">
				<xsl:call-template name="substring-after-last">
					<xsl:with-param name="string" select="substring-after($string, $occurrence)" />
					<xsl:with-param name="occurrence" select="$occurrence"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$string"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="substring-count">
		<xsl:param name="string" />
		<xsl:param name="occurrence" />
		<xsl:param name="count" select="0" />

		<xsl:choose>
			<xsl:when test="contains($string, $occurrence)">
				<xsl:call-template name="substring-count">
					<xsl:with-param name="string" select="substring-after($string, $occurrence)" />
					<xsl:with-param name="occurrence" select="$occurrence"/>
					<xsl:with-param name="count" select="$count +1" />
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$count"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="GetTagStyleProperty">
		<xsl:param name="tag"/>
		<xsl:param name="propertyName"/>
		<xsl:variable name="propertyValue">
			<xsl:choose>
				<xsl:when test="contains(substring-after($tag/@style,concat($propertyName,':')),';')">
					<xsl:value-of select="substring-before(substring-after($tag/@style,concat($propertyName,':')),';')"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="substring-after($tag/@style,concat($propertyName,':'))"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:value-of select="$propertyValue"/>
	</xsl:template>

	<xsl:template name="DelSpaces">
		<xsl:param name="str"/>

		<xsl:choose>
			<xsl:when test="contains($str, ' ')">
				<xsl:call-template name="DelSpaces">
					<xsl:with-param name="str">
						<xsl:value-of select="concat(substring-before($str, ' '), substring-after($str, ' '))"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$str"/>
			</xsl:otherwise>
		</xsl:choose>

	</xsl:template>

</xsl:stylesheet>