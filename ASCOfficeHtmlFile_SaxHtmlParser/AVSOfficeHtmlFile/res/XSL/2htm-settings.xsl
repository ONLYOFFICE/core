<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
				xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
				xmlns:xlink="http://www.w3.org/1999/xlink"
				xmlns:rels="http://schemas.openxmlformats.org/package/2006/relationships"
				xmlns:config="urn:oasis:names:tc:opendocument:xmlns:config:1.0"
				xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"
				xmlns:ooo="http://openoffice.org/2004/office"
				xmlns:odfcompat="http://odf-converter.sourceforge.net/compatibilitySettings/1.0"
				xmlns:oox="urn:oox"
                xmlns="http://www.w3.org/1999/xhtml"
				exclude-result-prefixes="oox odfcompat w rels">

	<xsl:template name="settings">
		<office:document-settings office:version="1.1">
			<office:settings>
				<config:config-item-set config:name="ooo:view-settings">
					<xsl:choose>
						<xsl:when
						  test="key('Part', 'word/document.xml')/w:document/w:body/descendant::w:ins or 
								key('Part', 'word/document.xml')/w:document/w:body/descendant::w:del or
								key('Part', 'word/document.xml')/w:document/w:body/descendant::w:pPrChange or
								key('Part', 'word/document.xml')/w:document/w:body/descendant::w:rPrChange">
							<config:config-item config:name="ShowRedlineChanges" config:type="boolean">true</config:config-item>
						</xsl:when>
						<xsl:otherwise>
							<config:config-item config:name="ShowRedlineChanges" config:type="boolean">false</config:config-item>
						</xsl:otherwise>
					</xsl:choose>
				</config:config-item-set>



				<config:config-item-set config:name="ooo:configuration-settings">

					<xsl:variable name="CompatibilitySettingsFile">
						<xsl:for-each select="key('Part', 'word/_rels/document.xml.rels')/rels:Relationships/rels:Relationship[@Type = 'http://schemas.openxmlformats.org/officeDocument/2006/relationships/customXml']">
							<xsl:if test="key('Part',substring-after(@Target,'/'))/odfcompat:CompatibilitySettings">
								<xsl:value-of select="substring-after(@Target,'/')"/>
							</xsl:if>
						</xsl:for-each>
					</xsl:variable>

					<!-- -->
					<xsl:for-each select="key('Part', $CompatibilitySettingsFile)/odfcompat:CompatibilitySettings/odfcompat:CompatibilitySetting">
						<xsl:if test="@name != 'AddParaTableSpacing' 
										and @name != 'AddParaTableSpacingAtStart' 
										and @name != 'PrinterIndependentLayout'">
							<config:config-item config:name="{@name}" config:type="{@type}">
								<xsl:choose>
									<xsl:when test="@type = 'boolean' and @value = '1'">
										<xsl:value-of select="'true'"/>
									</xsl:when>
									<xsl:when test="@type = 'boolean' and @value = '0'">
										<xsl:value-of select="'false'"/>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select="@value"/>
									</xsl:otherwise>
								</xsl:choose>
							</config:config-item>
						</xsl:if>
					</xsl:for-each>

					<!--clam, dialogika: re-inserted "true" part (bug #1787090)-->
					<config:config-item config:name="AddParaTableSpacing" config:type="boolean">
						<xsl:choose>
							<xsl:when test="key('Part', 'word/settings.xml')/w:settings/w:compat/w:doNotUseHTMLParagraphAutoSpacing">true</xsl:when>
							<xsl:otherwise>false</xsl:otherwise>
						</xsl:choose>
					</config:config-item>

					<!-- Uncommenting AddParaTableSpacing will introduce a side effect on line length in OpenOffice 2.x.!!  -->
					<!--
					<config:config-item-set config:name="ooo:configuration-settings">                   
					  <config:config-item config:name="AddParaTableSpacing" config:type="boolean">
						<xsl:choose>
						  <xsl:when test="key('Part', 'word/settings.xml')/w:settings/w:compat/w:doNotUseHTMLParagraphAutoSpacing">true</xsl:when>
						  <xsl:otherwise>false</xsl:otherwise>
						</xsl:choose>                        
					  </config:config-item>
					  <config:config-item config:name="UseOldNumbering" config:type="boolean">false</config:config-item>
					</config:config-item-set>
					  -->

					<!--clam, dialogika: bugfix 1948059-->
					<config:config-item config:name="AddParaSpacingToTableCells" config:type="boolean">true</config:config-item>
					<config:config-item config:name="AddParaTableSpacingAtStart" config:type="boolean">true</config:config-item>

					<!--clam, dialogika-->
					<xsl:if test="not(key('Part', $CompatibilitySettingsFile)/odfcompat:CompatibilitySettings/odfcompat:CompatibilitySetting[@name='AddExternalLeading'])">
						<config:config-item config:name="AddExternalLeading" config:type="boolean">true</config:config-item>
					</xsl:if>

					<!--math, dialogika: Added for correct indentation calculation BEGIN -->
					<xsl:if test="not(key('Part', $CompatibilitySettingsFile)/odfcompat:CompatibilitySettings/odfcompat:CompatibilitySetting[@name='IgnoreFirstLineIndentInNumbering'])">
						<config:config-item config:name="IgnoreFirstLineIndentInNumbering" config:type="boolean">false</config:config-item>
					</xsl:if>

					<!-- 2008-10-20/divo: Fix for # 2030271-->
					<xsl:if test="not(key('Part', $CompatibilitySettingsFile)/odfcompat:CompatibilitySettings/odfcompat:CompatibilitySetting[@name='UseFormerLineSpacing'])">
						<config:config-item config:name="UseFormerLineSpacing" config:type="boolean">false</config:config-item>
					</xsl:if>

					<!-- makz: Use new text wrapping to emulate Word text wrapping -->
					<xsl:if test="not(key('Part', $CompatibilitySettingsFile)/odfcompat:CompatibilitySettings/odfcompat:CompatibilitySetting[@name='UseFormerTextWrapping'])">
						<config:config-item config:name="UseFormerTextWrapping" config:type="boolean">false</config:config-item>
					</xsl:if>


					<!--math, dialogika: Added for correct indentation calculation END -->

					<!--divo, dialogika: retain Use Printer Metrics compatibility setting BEGIN -->
					<config:config-item config:name="PrinterIndependentLayout" config:type="string">
						<xsl:choose>
							<xsl:when test="key('Part', 'word/settings.xml')/w:settings/w:compat/w:usePrinterMetrics">
								<xsl:value-of select="'disabled'"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="'high-resolution'"/>
							</xsl:otherwise>
						</xsl:choose>
					</config:config-item>
					<!--divo, dialogika: retain Use Printer Metrics compatibility setting END -->

				</config:config-item-set >

			</office:settings>
		</office:document-settings>
	</xsl:template>

	<xsl:template match="w:compat"> </xsl:template>

</xsl:stylesheet>
