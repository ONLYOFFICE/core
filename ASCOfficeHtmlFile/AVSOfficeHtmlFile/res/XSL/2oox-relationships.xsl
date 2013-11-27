<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:import href="2oox-common.xsl"/>

	<xsl:output method="xml" encoding="UTF-8"/>

	<xsl:strip-space elements="*"/>

	<xsl:template match="/html">
		<xsl:message>Progress: html</xsl:message>
		<Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships">
			<Relationship Id="rId1" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles" Target="styles.xml"/>
			<Relationship Id="rId2" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings" Target="settings.xml"/>
			<Relationship Id="rId3" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/webSettings" Target="webSettings.xml"/>
			<Relationship Id="rId4" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/fontTable" Target="fontTable.xml"/>
			<Relationship Id="rId5" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme" Target="theme/theme1.xml"/>
			<Relationship Id="rId6" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/numbering" Target="numbering.xml"/>
      <xsl:apply-templates select="./body[@background-image]"/>
      <xsl:apply-templates select="./*/*"/>

		</Relationships>
	</xsl:template>

	<!-- hyperlinks -->
	<xsl:template match="a[ @href ]">
		<xsl:message>Progress: a (reference)</xsl:message>

		<xsl:variable name="validHref">
			<xsl:call-template name="hrefIsValid">
				<xsl:with-param name="href" select="@href"/>
			</xsl:call-template>
		</xsl:variable>


		<xsl:if test="$validHref = 'true'">
			<Relationship>
				<xsl:attribute name="Id">
					<xsl:call-template name="getRefID"/>
				</xsl:attribute>
				<xsl:attribute name="Type">
					<xsl:text>http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink</xsl:text>
				</xsl:attribute>
				<xsl:attribute name="Target">
					<xsl:variable name="path1">
						<xsl:choose>
							<xsl:when test="contains(@href, '?')">
								<xsl:call-template name="trim">
									<xsl:with-param name="s" select="substring-before(@href, '?')"/>
								</xsl:call-template>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="@href"/>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:variable>
					<xsl:variable name="path2">
						<xsl:choose>
							<xsl:when test="contains($path1, '&amp;')">
								<xsl:call-template name="trim">
									<xsl:with-param name="s" select="substring-before($path1, '&amp;')"/>
								</xsl:call-template>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="$path1"/>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:variable>
					<xsl:variable name="path3">
						<xsl:choose>
							<xsl:when test="contains($path2, '&quot;')">
								<xsl:call-template name="trim">
									<xsl:with-param name="s" select="substring-before($path1, '&quot;')"/>
								</xsl:call-template>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="$path2"/>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:variable>
					<xsl:call-template name="trim">
						<xsl:with-param name="s" select="$path3"/>
					</xsl:call-template>
				</xsl:attribute>
				<xsl:attribute name="TargetMode">
					<xsl:text>External</xsl:text>
				</xsl:attribute>
			</Relationship>
		</xsl:if>
		<xsl:apply-templates/>
	</xsl:template>

	<!-- images -->
	<xsl:template match="img[ @src ]">
		<xsl:message>Progress: img (reference)</xsl:message>
		<xsl:variable name="rId">
			<xsl:call-template name="getRefID"/>
		</xsl:variable>
		<Relationship>
			<xsl:attribute name="Id">
				<xsl:value-of select="$rId"/>
			</xsl:attribute>
			<xsl:attribute name="Type">
				<xsl:text>http://schemas.openxmlformats.org/officeDocument/2006/relationships/image</xsl:text>
			</xsl:attribute>
			<xsl:attribute name="Source">
				<xsl:value-of select="@src"/>
			</xsl:attribute>
			<xsl:attribute name="Target">
				<xsl:call-template name="getImagePath">
					<xsl:with-param name="src" select="@src"/>
				</xsl:call-template>
			</xsl:attribute>
		</Relationship>
	</xsl:template>


	<!-- background-image -->
	<xsl:template match="body[@background-image]">
		<xsl:message>Progress: img (reference)</xsl:message>
		<xsl:variable name="rId">
			<xsl:text>rId7</xsl:text>
		</xsl:variable>
		<Relationship>
			<xsl:attribute name="Id">
				<xsl:value-of select="$rId"/>
			</xsl:attribute>
			<xsl:attribute name="Type">
				<xsl:text>http://schemas.openxmlformats.org/officeDocument/2006/relationships/image</xsl:text>
			</xsl:attribute>
			<xsl:attribute name="Source">
				<xsl:value-of select="."/>
			</xsl:attribute>
			<xsl:attribute name="Target">
				<xsl:call-template name="getImagePath">
					<xsl:with-param name="src" select="."/>
				</xsl:call-template>
			</xsl:attribute>
		</Relationship>
	</xsl:template>

	<xsl:template match="text()"/>



</xsl:stylesheet>
