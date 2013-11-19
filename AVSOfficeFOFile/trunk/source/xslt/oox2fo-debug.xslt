<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet
  version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:msxsl="urn:schemas-microsoft-com:xslt"
  xmlns:fo="http://www.w3.org/1999/XSL/Format"
  xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"
  xmlns:wp="http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing"
  xmlns:mc="http://schemas.openxmlformats.org/markup-compatibility/2006"
  xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main"
  xmlns:w10="urn:schemas-microsoft-com:office:word"
  xmlns:my="dummy.namespace"
  xmlns:v="urn:schemas-microsoft-com:vml"
  xmlns:o="urn:schemas-microsoft-com:office:office"
  xmlns:oox="urn:oox"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns:rels="http://schemas.openxmlformats.org/package/2006/relationships"
  xmlns:pic="http://schemas.openxmlformats.org/drawingml/2006/picture"
  xmlns:cp="http://schemas.openxmlformats.org/package/2006/metadata/core-properties"
  xmlns:dc="http://purl.org/dc/elements/1.1/"
  xmlns:dcterms="http://purl.org/dc/terms/"
  xmlns:dcmitype="http://purl.org/dc/dcmitype/"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xmlns:vt="http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes"
  xmlns:prop="http://schemas.openxmlformats.org/officeDocument/2006/extended-properties"

  exclude-result-prefixes="w oox msxsl my rels r v cp dc dcterms dcmitype xsi wp w10 mc a rels pic o vt prop">

	<!-- Settings -->
	<xsl:output method="xml" version="1.0" indent="no" encoding="utf-8"/>
	<xsl:variable name="oox2fo-debug">
		<xsl:value-of select="0"/>
	</xsl:variable>
	<xsl:variable name="verbose">
		<xsl:value-of select="'yes'"/>
	</xsl:variable>

	<!-- Defaults -->
	<xsl:variable name="default-width.list-label" select="9"/>
	<xsl:variable name="default-font-size" select="10"/>
	<xsl:variable name="default-font-size.list-label" select="12"/>
	<xsl:variable name="default-font-size.symbol" select="12"/>
	<xsl:variable name="default-widows" select="2"/>
	<xsl:variable name="default-orphans" select="2"/>
	<xsl:variable name="white-space-collapse" select="'false'"/>
	<xsl:variable name="default-header-extent" select="'792pt'"/>
	<xsl:variable name="default-footer-extent" select="'792pt'"/>
	<xsl:variable name="default-line-height" select="1.147"/>
	<xsl:variable name="default-margin-size" select="28.35"/>

	<!-- Globals -->
	<xsl:variable name="not-supported-inlines" select="''"/>
	<xsl:variable name="image-relationship" select="'http://schemas.openxmlformats.org/officeDocument/2006/relationships/image'"/>
	<xsl:key name="Part" match="/oox:package/oox:part" use="@oox:name"/>
	<xsl:key name="styles" match="/oox:package/oox:part/w:styles/w:style" use="@w:styleId"/>
	<xsl:key name="default-styles" match="/oox:package/oox:part/w:styles/w:style[@w:default = 1 or @w:default = 'true' or @w:default = 'on']" use="@w:type"/>
	<xsl:key name="numbering-num" match="/oox:package/oox:part/w:numbering/w:num" use="@w:numId"/>
	<xsl:key name="numbering-abstract" match="/oox:package/oox:part/w:numbering/w:abstractNum" use="@w:abstractNumId"/>
	<xsl:key name="relationships-relationship" match="/oox:package/oox:part[@oox:name='word/_rels/document.xml.rels']/rels:Relationships/rels:Relationship" use="@Id"/>
	<xsl:key name="relationships-relationship-image" match="/oox:package/oox:part[@oox:name='word/_rels/document.xml.rels']/rels:Relationships/rels:Relationship[@Type='http://schemas.openxmlformats.org/officeDocument/2006/relationships/image']" use="@Id"/>
	<xsl:variable name="default-paragraph-style" select="key('Part', 'word/styles.xml')/w:styles/w:style[(@w:default='1' or @w:default='on' or @w:default='true') and @w:type='paragraph' and position()=1]"/>
	<xsl:variable name="default-character-style" select="key('Part', 'word/styles.xml')/w:styles/w:style[(@w:default='1' or @w:default='on' or @w:default='true') and @w:type='character' and position()=1]"/>
	<xsl:variable name="avs-properties-fofilename" select="key('Part', 'AvsProperties')/oox:AvsProperties/oox:OutputFOFileName/@value"/>
	<xsl:variable name="avs-properties-temp-directory" select="key('Part', 'AvsProperties')/oox:AvsProperties/oox:TemploraryDirectory/@value"/>
	<xsl:variable name="default-paragraph-style-docdef" select="key('Part', 'word/styles.xml')/w:styles/w:docDefaults/w:pPrDefault"/>
	<xsl:variable name="default-character-style-docdef" select="key('Part', 'word/styles.xml')/w:styles/w:docDefaults/w:rPrDefault/w:rPr/*"/>
	<xsl:variable name="default-table-style" select="key('Part', 'word/styles.xml')/w:styles/w:style[(@w:default='1' or @w:default='on' or @w:default='true') and @w:type='table']"/>
	<xsl:variable name="default-rPr-style" select="key('Part', 'word/styles.xml')/w:styles/w:docDefaults/w:rPrDefault/w:rPr"/>
	<xsl:variable name="default-pPr-style" select="key('Part', 'word/styles.xml')/w:styles/w:docDefaults/w:pPrDefault/w:pPr"/>
	<xsl:variable name="relationships" select="key('Part', 'word/_rels/document.xml.rels')/rels:Relationships"/>
	<xsl:variable name="font_theme" select="$relationships/rels:Relationship[@Type='http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme'][1]/@Target"/>
	<xsl:variable name="font-scheme" select="key('Part', concat('word/', $font_theme))/a:theme/a:themeElements/a:fontScheme"/>
	<xsl:variable name="default-tab-stop" select="key('Part', 'word/settings.xml')/w:settings/w:defaultTabStop"/>
	<xsl:variable name="allToPt">
		<xsl:value-of select="'yes'"/>
	</xsl:variable>

	<xsl:variable name="numberingPart" select="/oox:package/oox:part/w:numbering"/>

	<!-- DEBUG -->
	<xsl:variable name="debugSection" select="2"/>
	<xsl:variable name="debugParagraph" select="2"/>
	<xsl:variable name="debugTest" select="1"/>
	
	<xsl:template name="debugMessage">
		<xsl:param name="type"/>
		<xsl:param name="text"/>
		<xsl:variable name="allow">
			<xsl:call-template name="allowDebug">
				<xsl:with-param name="type" select="substring-before($type, ',')"/>
				<xsl:with-param name="level" select="substring-after($type, ',')"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:if test="$allow = 'true'">
			<xsl:message>
				<xsl:value-of select="concat($type, ': ', $text)"/>
			</xsl:message>
		</xsl:if>
	</xsl:template>	
	<xsl:template name="allowDebug">
		<xsl:param name="type"/>
		<xsl:param name="level" select="3"/>
		<xsl:variable name="targetLevel">
			<xsl:choose>
				<xsl:when test="'section' = $type">
					<xsl:value-of select="$debugSection"/>
				</xsl:when>
				<xsl:when test="'paragraph' = $type">
					<xsl:value-of select="$debugParagraph"/>
				</xsl:when>
				<xsl:when test="'test' = $type">
					<xsl:value-of select="$debugTest"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="0"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="$targetLevel >= $level">
				<xsl:value-of select="'true'"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="'false'"/>
			</xsl:otherwise>			
		</xsl:choose>
	</xsl:template>

	<!-- DOCUMENT -->
	<xsl:template match="/oox:package">
		<xsl:element name="fo:root">
			<xsl:call-template name="GatherDocInfo"/>
			<xsl:call-template name="GenerateLayoutMasterSet"/>
			<xsl:call-template name="GeneratePageSequences"/>
		</xsl:element>
	</xsl:template>
	<!-- SECTIONS -->
	<xsl:variable name="sections" select="key('Part', 'word/document.xml')/w:document/w:body/descendant::*[self::w:sectPr]"/>	
	<xsl:template name="GenerateLayoutMasterSet">
		<xsl:call-template name="debugMessage">
			<xsl:with-param name="type" select="'section,2'"/>
			<xsl:with-param name="text" select="concat('sections count=', count($sections))"/>
		</xsl:call-template>
		<xsl:call-template name="debugMessage">
			<xsl:with-param name="type" select="'paragraph,2'"/>
			<xsl:with-param name="text" select="concat('start paragraphs count=', count($start-paragraphs))"/>
		</xsl:call-template>
		<fo:layout-master-set>
			<xsl:choose>
				<xsl:when test="count($sections) > 0">
					<xsl:call-template name="ProcessSections"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name="CreateDefaultSection"/>
				</xsl:otherwise>
			</xsl:choose>
		</fo:layout-master-set>
	</xsl:template>	
	<xsl:template name="ProcessSections">
		<xsl:param name="iteration" select="1"/>
		<xsl:call-template name="debugMessage">
			<xsl:with-param name="type" select="'section,3'"/>
			<xsl:with-param name="text" select="concat('num = ', $iteration)"/>
		</xsl:call-template>
		<xsl:apply-templates select="$sections[$iteration]">
			<xsl:with-param name="index" select="$iteration"/>
		</xsl:apply-templates>
		<xsl:if test="count($sections) != $iteration">
			<xsl:call-template name="ProcessSections">
				<xsl:with-param name="iteration" select="$iteration+1"/>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>
	<xsl:template name="CreateDefaultSection">
		<fo:simple-page-master master-name="section1-simple-page-master" continuous="false">
			<fo:region-body region-name="xsl-region-body" hasCols="true" margin-top="21.3pt" margin-bottom="21.3pt">
				<w:cols w:space="708"/>
			</fo:region-body>
			<fo:region-before region-name="xsl-region-before" extent="792pt"></fo:region-before>
			<fo:region-after region-name="xsl-region-after" extent="792pt" display-align="after"></fo:region-after>
			<pageNumbering format="" start="-1"/>
		</fo:simple-page-master>
	</xsl:template>
	<xsl:template match="w:sectPr">
		<xsl:param name="index" select="0"/>
		<fo:simple-page-master master-name="{concat('section', $index, '-simple-page-master')}">
			<xsl:attribute name="continuous">
				<xsl:choose>
					<xsl:when test="w:type/@w:val = 'continuous'">
						<xsl:text>true</xsl:text>
					</xsl:when>
					<xsl:otherwise>
						<xsl:text>false</xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>
			<xsl:apply-templates select="w:pgSz"/>
			<xsl:apply-templates select="w:pgMar">
				<xsl:with-param name="area" select="'page-master'"/>
			</xsl:apply-templates>

			<fo:region-body region-name="xsl-region-body">
				<xsl:if test="w:cols">
					<xsl:attribute name="hasCols">
						<xsl:text>true</xsl:text>
					</xsl:attribute>
				</xsl:if>
				<xsl:apply-templates select="w:pgMar">
					<xsl:with-param name="area" select="'region-body'"/>
				</xsl:apply-templates>
				<xsl:apply-templates select="w:pgBorders"/>
				<xsl:apply-templates select="w:vAlign"/>
				<xsl:apply-templates select="w:cols"/>
			</fo:region-body>
			<fo:region-before region-name="xsl-region-before" extent="{$default-footer-extent}">
				<xsl:apply-templates select="w:pgMar">
					<xsl:with-param name="area" select="'static-content'"/>
				</xsl:apply-templates>
			</fo:region-before>
			<fo:region-after region-name="xsl-region-after" extent="{$default-footer-extent}" display-align="after">
				<!--<fo:region-after region-name="xsl-region-after" extent="{$default-footer-extent}">-->
				<xsl:apply-templates select="w:pgMar">
					<xsl:with-param name="area" select="'static-content'"/>
				</xsl:apply-templates>
			</fo:region-after>

			<xsl:call-template name="PageNumbering"/>
		</fo:simple-page-master>
	</xsl:template>
	<xsl:template match="w:pgSz">
		<xsl:variable name="page-width">
			<xsl:value-of select="@w:w div 1440"/>
		</xsl:variable>
		<xsl:variable name="page-height">
			<xsl:value-of select="@w:h div 1440"/>
		</xsl:variable>
		<xsl:attribute name="page-width">
			<xsl:choose>
				<xsl:when test="$allToPt='yes'">
					<xsl:call-template name="Convert-in2pt">
						<xsl:with-param name="inputValue" select="$page-width"/>
					</xsl:call-template>
					<xsl:text>pt</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name="Convert-in2cm">
						<xsl:with-param name="inputValue" select="$page-width"/>
					</xsl:call-template>
					<xsl:text>cm</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:attribute>
		<xsl:attribute name="page-height">
			<xsl:choose>
				<xsl:when test="$allToPt='yes'">
					<xsl:call-template name="Convert-in2pt">
						<xsl:with-param name="inputValue" select="$page-height"/>
					</xsl:call-template>
					<xsl:text>pt</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name="Convert-in2cm">
						<xsl:with-param name="inputValue" select="$page-height"/>
					</xsl:call-template>
					<xsl:text>cm</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:attribute>
	</xsl:template>
	<xsl:template match="w:pgMar">
		<xsl:param name="area" select="'region-body'"/>

		<xsl:variable name="border-right" select="../w:pgBorders/w:right"/>
		<xsl:variable name="border-left" select="../w:pgBorders/w:left"/>
		<xsl:variable name="border-top" select="../w:pgBorders/w:top"/>
		<xsl:variable name="border-bottom" select="../w:pgBorders/w:bottom"/>
		<xsl:choose>
			<!-- set margins on simple-page-master -->
			<xsl:when test="$area='page-master'">
				<!-- @w:right, @w:left, @w:header, @w:bottom define page master margins -->
				<xsl:variable name="left-value">
					<xsl:choose>
						<xsl:when test="@w:left">
							<xsl:value-of select="@w:left div 20"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="$default-margin-size"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>
				<xsl:variable name="right-value">
					<xsl:choose>
						<xsl:when test="@w:right">
							<xsl:value-of select="@w:right div 20"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="$default-margin-size"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>
				<xsl:variable name="header-value">
					<xsl:choose>
						<xsl:when test="@w:header">
							<xsl:value-of select="@w:header div 20"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="$default-margin-size"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>
				<xsl:variable name="footer-value">
					<xsl:choose>
						<xsl:when test="@w:footer">
							<xsl:value-of select="@w:footer div 20"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="$default-margin-size"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>

				<xsl:attribute name="margin-top">
					<xsl:value-of select="$header-value"/>
					<xsl:text>pt</xsl:text>
				</xsl:attribute>
				<xsl:attribute name="margin-bottom">
					<xsl:value-of select="$footer-value"/>
					<xsl:text>pt</xsl:text>
				</xsl:attribute>
				<xsl:attribute name="margin-right">
					<xsl:choose>
						<!-- if page has borders - the right margin is the distance -->
						<!-- between page edge and the right border.                -->
						<xsl:when test="$border-right">
							<xsl:choose>
								<xsl:when test="../w:pgBorders/@w:offset-from='page'">
									<xsl:value-of select="$border-right/@w:space"/>
									<xsl:text>pt</xsl:text>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="$right-value - ($border-right/@w:sz div 8) - ($border-right/@w:space)"/>
									<xsl:text>pt</xsl:text>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="$right-value"/>
							<xsl:text>pt</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>
				<xsl:attribute name="margin-left">
					<xsl:choose>
						<!-- if page has borders - the left margin is the distance -->
						<!-- between page edge and the left border.                -->
						<xsl:when test="$border-left">
							<xsl:choose>
								<xsl:when test="../w:pgBorders/@w:offset-from='page'">
									<xsl:value-of select="$border-left/@w:space"/>
									<xsl:text>pt</xsl:text>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="$left-value - ($border-left/@w:sz div 8) - ($border-left/@w:space)"/>
									<xsl:text>pt</xsl:text>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="$left-value"/>
							<xsl:text>pt</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>
			</xsl:when>
			<!-- set margins and paddings on region-body -->
			<xsl:when test="$area='region-body'">
				<xsl:choose>
					<!-- if page has borders - the body would have paddings -->
					<xsl:when test="../w:pgBorders">
						<xsl:if test="$border-top">
							<xsl:choose>
								<xsl:when test="../w:pgBorders/@w:offset-from='page'">
									<xsl:attribute name="margin-top">
										<xsl:value-of select="$border-top/@w:space - (@w:header div 20)"/>
										<xsl:text>pt</xsl:text>
									</xsl:attribute>
									<xsl:attribute name="padding-top">
										<xsl:value-of select="(@w:top div 20) - $border-top/@w:space - ($border-top/@w:sz div 8)"/>
										<xsl:text>pt</xsl:text>
									</xsl:attribute>
								</xsl:when>
								<xsl:otherwise>
									<xsl:attribute name="margin-top">
										<xsl:value-of select="(@w:top - @w:header) div 20 - $border-top/@w:space - ($border-top/@w:sz div 8)"/>
										<xsl:text>pt</xsl:text>
									</xsl:attribute>
									<xsl:attribute name="padding-top">
										<xsl:value-of select="$border-top/@w:space"/>
										<xsl:text>pt</xsl:text>
									</xsl:attribute>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:if>
						<xsl:if test="$border-bottom">
							<xsl:choose>
								<xsl:when test="../w:pgBorders/@w:offset-from='page'">
									<xsl:attribute name="margin-bottom">
										<xsl:value-of select="$border-bottom/@w:space - (@w:footer div 20)"/>
										<xsl:text>pt</xsl:text>
									</xsl:attribute>
									<xsl:attribute name="padding-bottom">
										<xsl:value-of select="(@w:bottom div 20) - $border-bottom/@w:space - ($border-bottom/@w:sz div 8)"/>
										<xsl:text>pt</xsl:text>
									</xsl:attribute>
								</xsl:when>
								<xsl:otherwise>
									<xsl:attribute name="margin-bottom">
										<xsl:value-of select="(@w:bottom - @w:footer) div 20 - $border-bottom/@w:space - ($border-bottom/@w:sz div 8)"/>
										<xsl:text>pt</xsl:text>
									</xsl:attribute>
									<xsl:attribute name="padding-bottom">
										<xsl:value-of select="$border-bottom/@w:space"/>
										<xsl:text>pt</xsl:text>
									</xsl:attribute>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:if>
						<xsl:if test="$border-left">
							<xsl:attribute name="padding-left">
								<xsl:choose>
									<xsl:when test="../w:pgBorders/@w:offset-from='page'">
										<xsl:value-of select="(@w:left div 20) - ($border-left/@w:sz div 8) - ($border-left/@w:space)"/>
										<xsl:text>pt</xsl:text>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select="$border-left/@w:space"/>
										<xsl:text>pt</xsl:text>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:attribute>
						</xsl:if>
						<xsl:if test="$border-right">
							<xsl:attribute name="padding-right">
								<xsl:choose>
									<xsl:when test="../w:pgBorders/@w:offset-from='page'">
										<xsl:value-of select="(@w:right div 20) - ($border-right/@w:sz div 8) - ($border-right/@w:space)"/>
										<xsl:text>pt</xsl:text>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select="$border-right/@w:space"/>
										<xsl:text>pt</xsl:text>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:attribute>
						</xsl:if>
					</xsl:when>
					<!-- @w:top and @w:bottom defines body margins -->
					<xsl:otherwise>
						<xsl:if test="@w:top">
							<xsl:attribute name="margin-top">
								<xsl:choose>
									<xsl:when test="@w:header">
										<xsl:value-of select="(@w:top - @w:header) div 20"/>
										<xsl:text>pt</xsl:text>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select="@w:top div 20 - $default-margin-size"/>
										<xsl:text>pt</xsl:text>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:attribute>
						</xsl:if>
						<xsl:if test="@w:bottom">
							<xsl:attribute name="margin-bottom">
								<xsl:choose>
									<xsl:when test="@w:footer">
										<xsl:value-of select="(@w:bottom - @w:footer) div 20"/>
										<xsl:text>pt</xsl:text>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select="@w:bottom div 20 - $default-margin-size"/>
										<xsl:text>pt</xsl:text>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:attribute>
						</xsl:if>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<!-- set paddings on region-before/region-after -->
			<xsl:when test="$area='static-content'">
				<xsl:if test="../w:pgBorders">
					<xsl:if test="$border-left">
						<xsl:attribute name="padding-left">
							<xsl:choose>
								<xsl:when test="../w:pgBorders/@w:offset-from='page'">
									<xsl:value-of select="(@w:left div 20) - ($border-left/@w:sz div 8) - ($border-left/@w:space)"/>
									<xsl:text>pt</xsl:text>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="$border-left/@w:space"/>
									<xsl:text>pt</xsl:text>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:attribute>
					</xsl:if>
					<xsl:if test="$border-right">
						<xsl:attribute name="padding-right">
							<xsl:choose>
								<xsl:when test="../w:pgBorders/@w:offset-from='page'">
									<xsl:value-of select="(@w:right div 20) - ($border-right/@w:sz div 8) - ($border-right/@w:space)"/>
									<xsl:text>pt</xsl:text>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="$border-right/@w:space"/>
									<xsl:text>pt</xsl:text>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:attribute>
					</xsl:if>
				</xsl:if>
			</xsl:when>
			<xsl:otherwise/>
		</xsl:choose>
	</xsl:template>
	<xsl:template match="w:cols">
		<xsl:copy-of select="."/>
	</xsl:template>
	<xsl:template name="PageNumbering">
		<xsl:variable name="format">
			<xsl:value-of select="w:pgNumType/@w:fmt"/>
		</xsl:variable>
		<xsl:variable name="start">
			<xsl:choose>
				<xsl:when test="w:pgNumType/@w:start">
					<xsl:value-of select="w:pgNumType/@w:start"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>-1</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<pageNumbering format="{$format}" start="{$start}"/>
	</xsl:template>
	<!-- FLOWS -->
	<xsl:variable name="paragraphs" select="key('Part', 'word/document.xml')/w:document/w:body/*[not(self::w:sectPr)]"/>
	<xsl:variable name="start-paragraphs" select="key('Part', 'word/document.xml')/w:document/w:body/*[w:pPr/w:sectPr]/following-sibling::*[1] | $paragraphs[1]"/>
	<xsl:template name="GeneratePageSequences">
		<xsl:param name="iteration" select="1"/>
		<fo:page-sequence master-reference="{concat('section', $iteration, '-simple-page-master')}">
			<xsl:call-template name="ProcessPageSequence">
				<xsl:with-param name="index" select="$iteration"/>
			</xsl:call-template>
		</fo:page-sequence>
		<xsl:if test="count($sections) != $iteration and count($sections) != 0">
			<xsl:call-template name="GeneratePageSequences">
				<xsl:with-param name="iteration" select="$iteration+1"/>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>
	<xsl:template name="ProcessPageSequence">
		<xsl:param name="index" select="1"/>
		<xsl:call-template name="ProcessFlows">
			<xsl:with-param name="index" select="$index"/>
		</xsl:call-template>
	</xsl:template>
	<xsl:template name="ProcessFlows">
		<xsl:param name="index" select="1"/>
		<fo:flow flow-name="xsl-region-body">
			<xsl:variable name="count-sibling-current" select="count($start-paragraphs[position()=$index]/following-sibling::*)"/>
			<xsl:variable name="count-sibling-next" select="count($start-paragraphs[position()=($index+1)]/following-sibling::*)"/>
			<xsl:apply-templates select="$start-paragraphs[position()=$index]"/>
			<xsl:for-each select="$start-paragraphs[position()=$index]/following-sibling::*[(position() &lt; $count-sibling-current - $count-sibling-next) and (not(self::sectPr))]">
				<xsl:apply-templates select="current()"/>
			</xsl:for-each>
		</fo:flow>
		<xsl:call-template name="ProcessHeader">
			<xsl:with-param name="index" select="$index"/>
		</xsl:call-template>
		<xsl:call-template name="ProcessFooter">
			<xsl:with-param name="index" select="$index"/>
		</xsl:call-template>
	</xsl:template>
	<xsl:template name="ProcessHeaderOld">
		<xsl:param name="index" select="1"/>
		<xsl:variable name="header-rid" select="$sections[$index]/w:headerReference[@w:type = 'default']/@r:id"/>
		<xsl:variable name="header-name">
			<xsl:choose>
				<xsl:when test="$header-rid">
					<xsl:value-of select="key('Part', 'word/_rels/document.xml.rels')/rels:Relationships/rels:Relationship[@Id=$header-rid]/@Target"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="key('Part', 'word/_rels/document.xml.rels')/rels:Relationships/rels:Relationship[@Type='http://schemas.openxmlformats.org/officeDocument/2006/relationships/header']/@Target"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="header-file-path" select="concat('word/', $header-name)"/>
		<fo:static-content flow-name="xsl-region-before">
			<xsl:apply-templates select="key('Part', $header-file-path)/w:hdr/*"/>
		</fo:static-content>
	</xsl:template>
	<xsl:template name="ProcessHeader">
		<xsl:param name="index" select="1"/>
		<xsl:variable name="first_page">
			<xsl:call-template name="TestFirstPage">
				<xsl:with-param name="index" select="$index"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:if test="$first_page = 'true'">
			<xsl:variable name="header_rid">
				<xsl:choose>
					<xsl:when test="$sections[$index]/w:headerReference[@w:type = 'first']/@r:id">
						<xsl:value-of select="$sections[$index]/w:headerReference[@w:type = 'first']/@r:id"/>
					</xsl:when>
					<xsl:when test="($index > 1) and $sections[$index - 1]/w:headerReference[@w:type = 'first']/@r:id">
						<xsl:value-of select="$sections[$index - 1]/w:headerReference[@w:type = 'first']/@r:id"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:text></xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<xsl:choose>
				<xsl:when test="$header_rid != ''">
					<xsl:call-template name="CreateStaticContent">
						<xsl:with-param name="flow_name" select="'xsl-region-before'"/>
						<xsl:with-param name="type" select="'first'"/>
						<xsl:with-param name="rid" select="$header_rid"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name="CreateStaticContent">
						<xsl:with-param name="flow_name" select="'xsl-region-before'"/>
						<xsl:with-param name="type" select="'first'"/>
					</xsl:call-template>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:if>
		<xsl:variable name="even_and_odd_pages">
			<xsl:call-template name="TestEvenAndOddPages"/>
		</xsl:variable>
		<xsl:if test="$even_and_odd_pages = 'true'">
			<xsl:variable name="header_rid">
				<xsl:choose>
					<xsl:when test="$sections[$index]/w:headerReference[@w:type = 'even']/@r:id">
						<xsl:value-of select="$sections[$index]/w:headerReference[@w:type = 'even']/@r:id"/>
					</xsl:when>
					<xsl:when test="($index > 1) and $sections[$index - 1]/w:headerReference[@w:type = 'even']/@r:id">
						<xsl:value-of select="$sections[$index - 1]/w:headerReference[@w:type = 'even']/@r:id"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:text></xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<xsl:if test="$header_rid != ''">
				<xsl:call-template name="CreateStaticContent">
					<xsl:with-param name="flow_name" select="'xsl-region-before'"/>
					<xsl:with-param name="type" select="'even'"/>
					<xsl:with-param name="rid" select="$header_rid"/>
				</xsl:call-template>
			</xsl:if>
		</xsl:if>
		<xsl:variable name="header_rid">
			<xsl:choose>
				<xsl:when test="$sections[$index]/w:headerReference[@w:type = 'default']/@r:id">
					<xsl:value-of select="$sections[$index]/w:headerReference[@w:type = 'default']/@r:id"/>
				</xsl:when>
				<xsl:when test="($index > 1) and $sections[$index - 1]/w:headerReference[@w:type = 'default']/@r:id">
					<xsl:value-of select="$sections[$index - 1]/w:headerReference[@w:type = 'default']/@r:id"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text></xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:if test="$header_rid != ''">
			<xsl:call-template name="CreateStaticContent">
				<xsl:with-param name="flow_name" select="'xsl-region-before'"/>
				<xsl:with-param name="type" select="'odd'"/>
				<xsl:with-param name="rid" select="$header_rid"/>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>
	<xsl:template name="ProcessFooter">
		<xsl:param name="index" select="1"/>
		<xsl:variable name="first_page">
			<xsl:call-template name="TestFirstPage">
				<xsl:with-param name="index" select="$index"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:if test="$first_page = 'true'">
			<xsl:variable name="footer_rid">
				<xsl:choose>
					<xsl:when test="$sections[$index]/w:footerReference[@w:type = 'first']/@r:id">
						<xsl:value-of select="$sections[$index]/w:footerReference[@w:type = 'first']/@r:id"/>
					</xsl:when>
					<xsl:when test="($index > 1) and $sections[$index - 1]/w:footerReference[@w:type = 'first']/@r:id">
						<xsl:value-of select="$sections[$index - 1]/w:footerReference[@w:type = 'first']/@r:id"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:text></xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<xsl:choose>
				<xsl:when test="$footer_rid != ''">
					<xsl:call-template name="CreateStaticContent">
						<xsl:with-param name="flow_name" select="'xsl-region-after'"/>
						<xsl:with-param name="type" select="'first'"/>
						<xsl:with-param name="rid" select="$footer_rid"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name="CreateStaticContent">
						<xsl:with-param name="flow_name" select="'xsl-region-after'"/>
						<xsl:with-param name="type" select="'first'"/>
					</xsl:call-template>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:if>
		<xsl:variable name="even_and_odd_pages">
			<xsl:call-template name="TestEvenAndOddPages"/>
		</xsl:variable>
		<xsl:if test="$even_and_odd_pages = 'true'">
			<xsl:variable name="footer_rid">
				<xsl:choose>
					<xsl:when test="$sections[$index]/w:footerReference[@w:type = 'even']/@r:id">
						<xsl:value-of select="$sections[$index]/w:footerReference[@w:type = 'even']/@r:id"/>
					</xsl:when>
					<xsl:when test="($index > 1) and $sections[$index - 1]/w:footerReference[@w:type = 'even']/@r:id">
						<xsl:value-of select="$sections[$index - 1]/w:footerReference[@w:type = 'even']/@r:id"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:text></xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<xsl:if test="$footer_rid != ''">
				<xsl:call-template name="CreateStaticContent">
					<xsl:with-param name="flow_name" select="'xsl-region-after'"/>
					<xsl:with-param name="type" select="'even'"/>
					<xsl:with-param name="rid" select="$footer_rid"/>
				</xsl:call-template>
			</xsl:if>
		</xsl:if>
		<xsl:variable name="footer_rid">
			<xsl:choose>
				<xsl:when test="$sections[$index]/w:footerReference[@w:type = 'default']/@r:id">
					<xsl:value-of select="$sections[$index]/w:footerReference[@w:type = 'default']/@r:id"/>
				</xsl:when>
				<xsl:when test="($index > 1) and $sections[$index - 1]/w:footerReference[@w:type = 'default']/@r:id">
					<xsl:value-of select="$sections[$index - 1]/w:footerReference[@w:type = 'default']/@r:id"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text></xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:if test="$footer_rid != ''">
			<xsl:call-template name="CreateStaticContent">
				<xsl:with-param name="flow_name" select="'xsl-region-after'"/>
				<xsl:with-param name="type" select="'odd'"/>
				<xsl:with-param name="rid" select="$footer_rid"/>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>
	<xsl:template name="TestFirstPage">
		<xsl:param name="index" select="1"/>
		<xsl:choose>
			<xsl:when test="$sections[$index]/w:titlePg">
				<xsl:text>true</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>false</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="TestEvenAndOddPages">
		<xsl:choose>
			<xsl:when test="key('Part', 'word/settings.xml')/w:settings/w:evenAndOddHeaders">
				<xsl:text>true</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>false</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="CreateStaticContent">
		<xsl:param name="flow_name" select="''"/>
		<xsl:param name="type" select="''"/>
		<xsl:param name="rid" select="''"/>
		<xsl:choose>
			<xsl:when test="$rid != ''">
				<xsl:variable name="file_name">
					<xsl:choose>
						<xsl:when test="$rid">
							<xsl:value-of select="key('Part', 'word/_rels/document.xml.rels')/rels:Relationships/rels:Relationship[@Id = $rid]/@Target"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text></xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>
				<xsl:if test="$file_name!=''">
					<xsl:variable name="file_path" select="concat('word/', $file_name)"/>
					<fo:static-content flow-name="{$flow_name}" type="{$type}">
						<xsl:apply-templates select="key('Part', $file_path)/w:ftr/*"/>
						<xsl:apply-templates select="key('Part', $file_path)/w:hdr/*"/>
					</fo:static-content>
				</xsl:if>
			</xsl:when>
			<xsl:otherwise>
				<fo:static-content flow-name="{$flow_name}" type="{$type}"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="GenerateStaticContentSettings">
		<xsl:param name="index" select="1"/>
		<xsl:variable name="section" select="$sections[$index]"/>
	</xsl:template>
	<!-- DOC INFO -->
	<xsl:template name="GatherDocInfo">
		<fo:doc-info>
			<xsl:call-template name="GeneratePageCount"/>
			<xsl:call-template name="GenerateSpacingAfterBefore"/>
			<xsl:call-template name="GenerateDocBackground"/>

			<xsl:call-template name="GenerateFontTable"/>
			<xsl:call-template name="GenerateListNumSettings"/>
			<xsl:call-template name="GeneratePageNumSettings"/>
		</fo:doc-info>
	</xsl:template>
	<xsl:template name="GeneratePageCount">
		<xsl:if test="key('Part', 'docProps/app.xml')/prop:Properties">
			<xsl:attribute name="pages">
				<xsl:value-of select="key('Part', 'docProps/app.xml')/prop:Properties/prop:Pages/text()"/>
			</xsl:attribute>
		</xsl:if>
	</xsl:template>
	<xsl:template name="GenerateDocBackground">
		<xsl:copy-of select="key('Part', 'word/document.xml')/w:document/w:background"/>
	</xsl:template>
	<xsl:template name="GenerateSpacingAfterBefore">
		<xsl:attribute name="useHtmlSpacing">
			<xsl:choose>
				<xsl:when test="key('Part', 'word/settings.xml')/w:settings/w:compat/w:doNotUseHTMLParagraphAutoSpacing">
					<xsl:text>false</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>true</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:attribute>
	</xsl:template>
	<!-- font table -->
	<xsl:template name="GenerateFontTable">
		<font-table>
			<xsl:apply-templates select="key('Part', 'word/fontTable.xml')/w:fonts/w:font"/>
		</font-table>
	</xsl:template>
	<xsl:template match="w:font">
		<font>
			<xsl:attribute name="name">
				<xsl:value-of select="@w:name"/>
			</xsl:attribute>
			<xsl:if test="w:altName">
				<altName>
					<xsl:value-of select="w:altName/@w:val"/>
				</altName>
			</xsl:if>
			<charset>
				<xsl:value-of select="w:charset/@w:val"/>
			</charset>
			<xsl:if test="w:family">
				<family>
					<xsl:value-of select="w:family/@w:val"/>
				</family>
			</xsl:if>
			<xsl:if test="w:panose1">
				<panose>
					<xsl:value-of select="w:panose1/@w:val"/>
				</panose>
			</xsl:if>
			<xsl:if test="w:pitch">
				<fixed>
					<xsl:choose>
						<xsl:when test="w:pitch/@w:val">
							<xsl:choose>
								<xsl:when test="w:pitch/@w:val='variable'">
									<xsl:text>0</xsl:text>
								</xsl:when>
								<xsl:when test="w:pitch/@w:val='default'">
									<xsl:text>0</xsl:text>
								</xsl:when>
								<xsl:otherwise>
									<xsl:text>1</xsl:text>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>1</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</fixed>
			</xsl:if>
			<xsl:if test="w:sig">
				<sig usb0="{w:sig/@w:usb0}" usb1="{w:sig/@w:usb1}" usb2="{w:sig/@w:usb2}" usb3="{w:sig/@w:usb3}" csb0="{w:sig/@w:csb0}" csb1="{w:sig/@w:csb1}"/>
			</xsl:if>
		</font>
	</xsl:template>
	<xsl:template match="w:vAlign">
		<xsl:attribute name="display-align">
			<xsl:choose>
				<xsl:when test="@w:val='top'">
					<xsl:text>before</xsl:text>
				</xsl:when>
				<xsl:when test="@w:val='center'">
					<xsl:text>center</xsl:text>
				</xsl:when>
				<xsl:when test="@w:val='bottom'">
					<xsl:text>after</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>auto</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:attribute>
	</xsl:template>
	<!-- page numeration settings -->
	<xsl:template name="GeneratePageNumSettings">
		<xsl:variable name="pageCount">
			<xsl:value-of select="key('Part', 'docProps/app.xml')/prop:Properties/prop:Pages/text()"/>
		</xsl:variable>
		<pageNumSettings pageCount="{$pageCount}"/>
	</xsl:template>
	<!-- list numeration settings -->
	<xsl:template name="GenerateListNumSettings">
		<listSettings>
			<xsl:apply-templates select="$numberingPart/w:abstractNum" mode="numSettings"/>
			<xsl:apply-templates select="$numberingPart/w:num[count(./w:lvlOverride) != 0]" mode="numSettings"/>
		</listSettings>
	</xsl:template>
	<xsl:template match="w:abstractNum" mode="numSettings">
		<num id="{@w:abstractNumId}">
			<xsl:apply-templates select="w:lvl" mode="numSettings"/>
		</num>
	</xsl:template>
	<xsl:template match="w:num" mode="numSettings">
		<numOverride id="{@w:numId}">
			<xsl:apply-templates select="w:lvlOverride/w:lvl" mode="numSettings"/>
		</numOverride>
	</xsl:template>
	<xsl:template match="w:lvl" mode="numSettings">
		<lvl index="{@w:ilvl}" text="{w:lvlText/@w:val}" format="{w:numFmt/@w:val}" start="{w:start/@w:val}"/>
	</xsl:template>
	<!-- PARAGRAPH -->
	<xsl:template match="w:p">
		<xsl:param name="footnote-current-symbol"/>
		<xsl:param name="endnote-current-symbol"/>
		<xsl:variable name="is_footnote" select="parent::w:footnote"/>
		<xsl:variable name="is_endnote" select="parent::w:endnote"/>
		<xsl:variable name="is_num">
			<xsl:call-template name="TestIsNum"/>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="$is_num='true'">
				<xsl:call-template name="CreateNum"/>
			</xsl:when>
			<xsl:when test="$is_footnote">
				<fo:block start-indent="0" text-indent="0" font-size="10pt" padding-before="3pt">
					<!-- Set properties -->
					<xsl:call-template name="ApplyParaProperties"/>
					<xsl:if test="position() = 1 and count(./w:r/w:footnoteRef) != 0">
						<fo:inline>
							<xsl:for-each select="./w:r[count(./w:footnoteRef) != 0]">
								<xsl:call-template name="ApplyRunProperties"/>
							</xsl:for-each>
							<text>
								<xsl:value-of select="$footnote-current-symbol"/>
							</text>
						</fo:inline>
					</xsl:if>
					<xsl:apply-templates select="*[not(w:footnoteRef) and not(self::w:pPr)]"/>
				</fo:block>
			</xsl:when>
			<xsl:when test="$is_endnote">
				<fo:block start-indent="0" text-indent="0" font-size="10pt" padding-before="3pt">
					<!-- Set properties -->
					<!--<xsl:call-template name="GetPStyle"/>-->
					<xsl:call-template name="ApplyParaProperties"/>
					<xsl:if test="position() = 1 and count(./w:r/w:endnoteRef) != 0">
						<fo:inline>
							<xsl:for-each select="./w:r[count(./w:endnoteRef) != 0]">
								<xsl:call-template name="ApplyRunProperties"/>
							</xsl:for-each>
							<text>
								<xsl:value-of select="$endnote-current-symbol"/>
							</text>
						</fo:inline>
					</xsl:if>
					<xsl:apply-templates select="*[not(w:endnoteRef) and not(self::w:pPr)]"/>
				</fo:block>
			</xsl:when>
			<xsl:otherwise>
				<fo:block>
					<xsl:call-template name="GetPStyle"/>
					<xsl:variable name="isFrame">
						<xsl:call-template name="IsFrame"/>
					</xsl:variable>
					<xsl:choose>
						<xsl:when test="$isFrame = 'true'">
							<xsl:call-template name="CreateFrameContent"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="SetContentOfParagraph"/>
						</xsl:otherwise>
					</xsl:choose>
				</fo:block>
				<xsl:for-each select=".//w:endnoteReference">
					<xsl:variable name="endnote-current-id">
						<xsl:value-of select="@w:id"/>
					</xsl:variable>

					<xsl:variable name="endnote-text-mark">
						<xsl:choose>
							<xsl:when test="@w:customMarkFollows">
								<xsl:value-of select="@w:customMarkFollows"/>
							</xsl:when>
						</xsl:choose>
					</xsl:variable>

					<xsl:variable name="endnote-cur-symbol">
						<xsl:choose>
							<xsl:when test="$endnote-text-mark = 1">
								<xsl:for-each select="w:endnoteReference">
									<xsl:for-each select="following-sibling::w:t">
										<xsl:value-of select="text()"/>
									</xsl:for-each>
								</xsl:for-each>
							</xsl:when>
							<xsl:otherwise>
								<xsl:number level="any" count="w:r[child::w:endnoteReference]" format="i"/>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:variable>

					<fo:endnote>
						<xsl:apply-templates select="/oox:package/oox:part/w:endnotes/w:endnote[@w:id = $endnote-current-id]/w:p">
							<xsl:with-param name="endnote-current-symbol" select="$endnote-cur-symbol"/>
						</xsl:apply-templates>
					</fo:endnote>

				</xsl:for-each>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="SetContentOfParagraph">
		<!--<xsl:call-template name="ApplyParaProperties"/>-->
		<xsl:call-template name="ProcessBookmarks"/>
		<xsl:choose>
			<xsl:when test="count(*[not(self::w:pPr | self::w:bookmarkStart)]) = 0">
				<xsl:call-template name="ApplyParaProperties">
					<xsl:with-param name="emptyPar" select="'true'"/>
				</xsl:call-template>
				<xsl:call-template name="InsertContentOfEmptyParagraph"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:call-template name="ApplyParaProperties"/>
				<xsl:apply-templates select="*[self::w:r | self::w:sdt | self::w:hyperlink | self::w:fldSimple | w:smartTag]"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="ApplyParaProperties">
		<xsl:param name="target" select="'all'"/>
		<xsl:param name="emptyPar" select="'false'"/>
		<xsl:if test="$target = 'base' or $target = 'all'">
			<xsl:call-template name="GetPStyle"/>
			<xsl:call-template name="ApplyFontTheme"/>
			<xsl:call-template name="setHeadingLevel"/>
			<xsl:apply-templates select="$default-character-style-docdef"/>
			<xsl:apply-templates select="$default-paragraph-style-docdef"/>
			<xsl:apply-templates select="$default-pPr-style"/>
			<xsl:apply-templates select="$default-paragraph-style"/>
			<xsl:apply-templates select="key('styles', w:pPr/w:pStyle/@w:val)[@w:type='paragraph'][position() = last()]"/>
			<xsl:for-each select="key('styles', w:pPr/w:pStyle/@w:val)[@w:type='paragraph']">
				<xsl:call-template name="GetPStyle"/>
			</xsl:for-each>
		</xsl:if>
		<xsl:choose>
			<xsl:when test="$target = 'all'">
				<xsl:for-each select="ancestor-or-self::*">
					<xsl:if test="name()='w:tbl'">
						<xsl:variable name="table-style" select="w:tblPr/w:tblStyle/@w:val"/>
						<xsl:if test="$table-style">
							<xsl:apply-templates select="key('styles', $table-style)/w:pPr"/>
						</xsl:if>
					</xsl:if>
				</xsl:for-each>
				<xsl:if test="parent::w:tc">
					<xsl:for-each select="parent::w:tc/w:tcPr/*">
						<xsl:if test="$default-paragraph-style/w:pPr/*[name()=name(current())]">
							<xsl:apply-templates select="."/>
						</xsl:if>
					</xsl:for-each>
				</xsl:if>
				<xsl:apply-templates select="$default-tab-stop"/>
				<xsl:choose>
					<xsl:when test="$emptyPar = 'true'">
						<xsl:apply-templates select="w:pPr/*[not(self::w:pStyle) and not(self::w:listPr) and not(self::w:sectPr) and not(self::w:numPr) and not(self::w:tabs)]"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:apply-templates select="w:pPr/*[not(self::w:rPr) and not(self::w:pStyle) and not(self::w:listPr) and not(self::w:sectPr) and not(self::w:numPr) and not(self::w:tabs)]"/>
					</xsl:otherwise>
				</xsl:choose>
				<xsl:apply-templates select="./w:pPr/w:tabs"/>
				<xsl:apply-templates select="key('styles', w:pPr/w:pStyle/@w:val)[@w:type='paragraph']/w:pPr/w:tabs"/>
			</xsl:when>
			<xsl:when test="$target = 'tabs'">
				<xsl:apply-templates select="./w:pPr/w:tabs"/>
				<xsl:apply-templates select="key('styles', w:pPr/w:pStyle/@w:val)[@w:type='paragraph']/w:pPr/w:tabs"/>
			</xsl:when>
			<xsl:when test="$target = 'other'">
				<xsl:for-each select="ancestor-or-self::*">
					<xsl:if test="name()='w:tbl'">
						<xsl:variable name="table-style" select="w:tblPr/w:tblStyle/@w:val"/>
						<xsl:if test="$table-style">
							<xsl:apply-templates select="key('styles', $table-style)/w:pPr"/>
						</xsl:if>
					</xsl:if>
				</xsl:for-each>
				<xsl:if test="parent::w:tc">
					<xsl:for-each select="parent::w:tc/w:tcPr/*">
						<xsl:if test="$default-paragraph-style/w:pPr/*[name()=name(current())]">
							<xsl:apply-templates select="."/>
						</xsl:if>
					</xsl:for-each>
				</xsl:if>
				<xsl:apply-templates select="$default-tab-stop"/>
				<xsl:choose>
					<xsl:when test="$emptyPar = 'true'">
						<xsl:apply-templates select="w:pPr/*[not(self::w:pStyle) and not(self::w:listPr) and not(self::w:sectPr) and not(self::w:numPr) and not(self::w:tabs)]"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:apply-templates select="w:pPr/*[not(self::w:rPr) and not(self::w:pStyle) and not(self::w:listPr) and not(self::w:sectPr) and not(self::w:numPr) and not(self::w:tabs)]"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="ProcessBookmarks">
		<xsl:if test="count(w:bookmarkStart) != 0">
			<xsl:attribute name="id">
				<xsl:apply-templates select="w:bookmarkStart"/>
			</xsl:attribute>
		</xsl:if>
	</xsl:template>
	<xsl:template name="InsertContentOfEmptyParagraph">
		<xsl:if test="not(./w:pPr/w:sectPr) or ./preceding-sibling::*[1]/w:pPr/w:sectPr">
			<fo:leader/>
		</xsl:if>
	</xsl:template>
	<!-- heading level -->
	<xsl:template name="setHeadingLevel">
		<xsl:param name="style" select="''"/>
		<xsl:param name="includes" select="'0'"/>
		<xsl:variable name="headings">
			<level name="heading 1">1</level>
			<level name="heading 2">2</level>
			<level name="heading 3">3</level>
		</xsl:variable>
		<xsl:variable name="maxIncludes">
			5
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="$style!=''">
				<xsl:variable name="styleName">
					<xsl:value-of select="key('styles', $style)/w:name/@w:val"/>
				</xsl:variable>
				<xsl:variable name="level">
					<xsl:value-of select="msxsl:node-set($headings)/level[@name=$styleName]"/>
				</xsl:variable>
				<xsl:choose>
					<xsl:when test="$level!=''">
						<xsl:attribute name="headingLvl">
							<xsl:value-of select="$level"/>
						</xsl:attribute>
					</xsl:when>
					<xsl:otherwise>
						<xsl:variable name="parentStyle">
							<xsl:value-of select="key('styles', $style)/w:basedOn/@w:val"/>
						</xsl:variable>
						<xsl:if test="$parentStyle!='' and $includes&lt;$maxIncludes">
							<xsl:call-template name="setHeadingLevel">
								<xsl:with-param name="style" select="$parentStyle"/>
								<xsl:with-param name="includes" select="$includes+1"/>
							</xsl:call-template>
						</xsl:if>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:otherwise>
				<xsl:variable name="paragraphStyle">
					<xsl:value-of select="w:pPr/w:pStyle/@w:val"/>
				</xsl:variable>
				<xsl:if test="$paragraphStyle!=''">
					<xsl:call-template name="setHeadingLevel">
						<xsl:with-param name="style" select="$paragraphStyle"/>
					</xsl:call-template>
				</xsl:if>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<!-- FRAME -->
	<xsl:template name="IsFrame">
		<xsl:choose>
			<xsl:when test="./w:pPr/w:framePr and not(ancestor::w:tbl) and (./w:pPr/w:framePr/@w:x or ./w:pPr/w:framePr/@w:y or ./w:pPr/w:framePr/@w:hAnchor or ./w:pPr/w:framePr/@w:h or ./w:pPr/w:framePr/@w:w)">
				<xsl:text>true</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>false</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="CreateFrameContent">
		<fo:alternate-content>
			<fo:property type="runtime">
				<fo:wrap>
					<xsl:variable name="wrapType" select="./w:pPr/w:framePr/@w:wrap"/>
					<xsl:attribute name="type">
						<xsl:choose>
							<xsl:when test="$wrapType = 'notBeside'">
								<xsl:text>topAndBottom</xsl:text>
							</xsl:when>
							<xsl:when test="$wrapType != ''">
								<xsl:text>square</xsl:text>
							</xsl:when>
							<xsl:otherwise>
								<xsl:text>none</xsl:text>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:attribute>
				</fo:wrap>
				<fo:inline-properties/>
				<fo:graphic type="LargeLetter"/>
			</fo:property>
			<fo:content>
				<fo:block>
					<xsl:call-template name="SetContentOfParagraph"/>
				</fo:block>
			</fo:content>
		</fo:alternate-content>
	</xsl:template>
	<!-- NUMBERING -->
	<xsl:template name="TestIsNum">
		<xsl:variable name="p_num_pr" select="w:pPr/w:numPr"/>
		<xsl:variable name="style_num_pr" select="key('styles', w:pPr/w:pStyle/@w:val)[@w:type='paragraph']"/>
		<xsl:variable name="num_id">
			<xsl:choose>
				<xsl:when test="$p_num_pr/w:numId/@w:val">
					<xsl:value-of select="$p_num_pr/w:numId/@w:val"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name="GetNumIdFromStyle">
						<xsl:with-param name="style" select="$style_num_pr"/>
					</xsl:call-template>
				</xsl:otherwise>

			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="num_lvl">
			<xsl:choose>
				<xsl:when test="$p_num_pr/w:ilvl/@w:val">
					<xsl:value-of select="$p_num_pr/w:ilvl/@w:val"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name="GetNumLvlFromStyle">
						<xsl:with-param name="style" select="$style_num_pr"/>
					</xsl:call-template>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="num" select="key('numbering-num', $num_id)"/>
		<xsl:variable name="abstract_num" select="key('numbering-abstract', $num/w:abstractNumId/@w:val)"/>
		<xsl:variable name="lvl_text">
			<xsl:value-of select="$abstract_num/w:lvl[position()=1]/w:lvlText/@w:val"/>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="($num_id!='') and ($lvl_text!='')">
				<xsl:value-of select="'true'"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="'false'"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="CreateNum">
		<xsl:variable name="p_num_pr" select="w:pPr/w:numPr"/>
		<xsl:variable name="style_num_pr" select="key('styles', w:pPr/w:pStyle/@w:val)[@w:type='paragraph']"/>
		<xsl:message>
			<xsl:text>---</xsl:text>
			<xsl:value-of select="$p_num_pr/w:numId/@w:val"/>
		</xsl:message>
		<xsl:variable name="num_id">
			<xsl:choose>
				<xsl:when test="$p_num_pr/w:numId/@w:val">
					<xsl:value-of select="$p_num_pr/w:numId/@w:val"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name="GetNumIdFromStyle">
						<xsl:with-param name="style" select="$style_num_pr"/>
					</xsl:call-template>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:message>
			<xsl:value-of select="$num_id"/>
		</xsl:message>
		<xsl:variable name="num_lvl">
			<xsl:choose>
				<xsl:when test="$p_num_pr/w:ilvl/@w:val">
					<xsl:value-of select="$p_num_pr/w:ilvl/@w:val"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name="GetNumLvlFromStyle">
						<xsl:with-param name="style" select="$style_num_pr"/>
					</xsl:call-template>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<fo:block>
			<xsl:call-template name="GetPStyle"/>
			<xsl:call-template name="ProcessBookmarks"/>
			<xsl:call-template name="CreateNumLabel">
				<xsl:with-param name="num_id" select="$num_id"/>
				<xsl:with-param name="num_lvl" select="$num_lvl"/>
			</xsl:call-template>
			<fo:inline>
				<fo:tab/>
			</fo:inline>
			<xsl:apply-templates select="*[not(self::w:pPr or self::w:bookmarkStart)]"/>
			<xsl:if test="not(w:r)">
				<fo:leader/>
			</xsl:if>
		</fo:block>
	</xsl:template>
	<xsl:template name="GetNumIdFromStyle">
		<xsl:param name="style"/>
		<xsl:choose>
			<xsl:when test="$style/w:pPr/w:numPr/w:numId/@w:val">
				<xsl:value-of select="$style/w:pPr/w:numPr/w:numId/@w:val"/>
			</xsl:when>
			<xsl:when test="$style/w:basedOn/@w:val">
				<xsl:if test="$style/@w:styleId != $style/w:basedOn/@w:val">
					<xsl:call-template name="GetNumIdFromStyle">
						<xsl:with-param name="style" select="key('styles', $style/w:basedOn/@w:val)[@w:type='paragraph']"/>
					</xsl:call-template>
				</xsl:if>
			</xsl:when>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="GetNumLvlFromStyle">
		<xsl:param name="style"/>
		<xsl:choose>
			<xsl:when test="$style/w:pPr/w:numPr/w:ilvl/@w:val">
				<xsl:value-of select="$style/w:pPr/w:numPr/w:ilvl/@w:val"/>
			</xsl:when>
			<xsl:when test="$style/w:basedOn/@w:val">
				<xsl:if test="$style/@w:styleId != $style/w:basedOn/@w:val">
					<xsl:call-template name="GetNumLvlFromStyle">
						<xsl:with-param name="style" select="key('styles', $style/w:basedOn/@w:val)[@w:type='paragraph']"/>
					</xsl:call-template>
				</xsl:if>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>0</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<!-- style -->
	<xsl:template name="GetPStyle">
		<xsl:if test="w:pPr/w:pStyle">
			<xsl:attribute name="fo:pStyle">
				<xsl:value-of select="w:pPr/w:pStyle/@w:val"/>
			</xsl:attribute>
		</xsl:if>
		<xsl:if test="w:pPr/w:contextualSpacing">
			<xsl:attribute name="fo:contextualSpacing">
				<xsl:choose>
					<xsl:when test="not(@w:val) or not(@w:val = '0') ">
						<xsl:text>true</xsl:text>
					</xsl:when>
					<xsl:otherwise>
						<xsl:text>false</xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>
		</xsl:if>
	</xsl:template>
	<xsl:template match="w:contextualSpacing">
		<xsl:attribute name="fo:contextualSpacing">
			<xsl:choose>
				<xsl:when test="not(@w:val) or not(@w:val = '0') ">
					<xsl:text>true</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>false</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:attribute>
	</xsl:template>
	<!-- label -->
	<xsl:template name="CreateNumLabel">
		<xsl:param name="num_id"/>
		<xsl:param name="num_lvl"/>
		<xsl:apply-templates select="$default-character-style-docdef"/>
		<xsl:apply-templates select="$default-character-style"/>
		<xsl:call-template name="ApplyParaProperties">
			<xsl:with-param name="target" select="'base'"/>
		</xsl:call-template>
		<xsl:apply-templates select="key('numbering-num', $num_id)">
			<xsl:with-param name="list-level" select="$num_lvl"/>
			<xsl:with-param name="mode" select="'block'"/>
			<xsl:with-param name="target" select="'other'"/>
		</xsl:apply-templates>
		<xsl:call-template name="ApplyParaProperties">
			<xsl:with-param name="target" select="'other'"/>
		</xsl:call-template>
		<xsl:call-template name="ApplyParaProperties">
			<xsl:with-param name="target" select="'tabs'"/>
		</xsl:call-template>
		<xsl:apply-templates select="key('numbering-num', $num_id)">
			<xsl:with-param name="list-level" select="$num_lvl"/>
			<xsl:with-param name="mode" select="'block'"/>
			<xsl:with-param name="target" select="'tabs'"/>
		</xsl:apply-templates>
		<fo:inline>
			<xsl:call-template name="CreateStyles">
				<xsl:with-param name="numId" select="$num_id"/>
				<xsl:with-param name="numLvl" select="$num_lvl"/>
			</xsl:call-template>
			<xsl:variable name="current-abstract-num-id">
				<!--<xsl:value-of select="/oox:package/oox:part/w:numbering/w:num[@w:numId = current()/w:numId/@w:val]/w:abstractNumId/@w:val"/>-->
				<xsl:value-of select="key('numbering-num', $num_id)/w:abstractNumId/@w:val"/>
			</xsl:variable>
			<xsl:variable name="current-num-format">
				<xsl:choose>
					<xsl:when test="key('numbering-num', @num_id)/w:lvlOverride[@w:ilvl = $num_lvl]/w:lvl/w:numFmt/@w:val">
						<xsl:value-of select="key('numbering-num', $num_id)/w:lvlOverride[@w:ilvl = $num_lvl]/w:lvl/w:numFmt/@w:val"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="/oox:package/oox:part/w:numbering/w:abstractNum[@w:abstractNumId=$current-abstract-num-id]/w:lvl[@w:ilvl=$num_lvl]/w:numFmt/@w:val"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<xsl:variable name="absnum_id">
				<xsl:value-of select="key('numbering-num', $num_id)/w:abstractNumId/@w:val"/>
			</xsl:variable>
			<xsl:if test="count(/oox:package/oox:part/w:numbering/w:num[@w:numId = $num_id])!=0">
				<xsl:choose>
					<xsl:when test="$current-num-format='none'">
						<fo:numbering id="{$absnum_id}" level="{$num_lvl}" text="" format="none"/>
						<!--<text/>-->
					</xsl:when>
					<xsl:when test="$current-num-format='bullet'">
						<!--<text>-->
						<fo:numbering id="{$absnum_id}" numId="{$num_id}" level="{$num_lvl}">
							<xsl:attribute name="text">
								<xsl:choose>
									<xsl:when test="key('numbering-num', $num_id)/w:lvlOverride[@w:ilvl = $num_lvl]/w:lvl/w:lvlText/@w:val">
										<xsl:value-of select="key('numbering-num', $num_id)/w:lvlOverride[@w:ilvl = $num_lvl]/w:lvl/w:lvlText/@w:val"/>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select="/oox:package/oox:part/w:numbering/w:abstractNum[@w:abstractNumId=$current-abstract-num-id]/w:lvl[@w:ilvl=$num_lvl]/w:lvlText/@w:val"/>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:attribute>
							<xsl:attribute name="format">
								<xsl:text>bullet</xsl:text>
							</xsl:attribute>
						</fo:numbering>
						<!--</text>-->
					</xsl:when>
					<xsl:otherwise>
						<xsl:variable name="num_text">
							<xsl:call-template name="GetNumText">
								<xsl:with-param name="num_id" select="$num_id"/>
								<xsl:with-param name="num_lvl" select="$num_lvl"/>
							</xsl:call-template>
						</xsl:variable>
						<xsl:variable name="num_format">
							<xsl:call-template name="GetNumFormat">
								<xsl:with-param name="num_id" select="$num_id"/>
								<xsl:with-param name="num_lvl" select="$num_lvl"/>
							</xsl:call-template>
						</xsl:variable>
						<xsl:variable name="num_start">
							<xsl:call-template name="GetNumStart">
								<xsl:with-param name="num_id" select="$num_id"/>
								<xsl:with-param name="num_lvl" select="$num_lvl"/>
							</xsl:call-template>
						</xsl:variable>
						<fo:numbering id="{$absnum_id}" numId="{$num_id}" absId="{$absnum_id}" 
							level="{$num_lvl}" text="{$num_text}" format="{$num_format}"
							start="{$num_start}"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:if>
		</fo:inline>
	</xsl:template>
	<xsl:template name="GetNumText">
		<xsl:param name="num_id"/>
		<xsl:param name="num_lvl"/>
		<xsl:choose>
			<xsl:when test="key('numbering-num', @num_id)/w:lvlOverride[@w:ilvl = $num_lvl]/w:lvl/w:lvlText/@w:val">
				<xsl:value-of select="key('numbering-num', $num_id)/w:lvlOverride[@w:ilvl = $num_lvl]/w:lvl/w:lvlText/@w:val"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:variable name="absnum_id">
					<xsl:value-of select="key('numbering-num', $num_id)/w:abstractNumId/@w:val"/>
				</xsl:variable>
				<xsl:value-of select="key('numbering-abstract', $absnum_id)/w:lvl[@w:ilvl=$num_lvl]/w:lvlText/@w:val"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="GetNumFormat">
		<xsl:param name="num_id"/>
		<xsl:param name="num_lvl"/>
		<xsl:variable name="absnum_id">
			<xsl:value-of select="key('numbering-num', $num_id)/w:abstractNumId/@w:val"/>
		</xsl:variable>
		<xsl:value-of select="key('numbering-abstract', $absnum_id)/w:lvl[@w:ilvl=$num_lvl]/w:numFmt/@w:val"/>
	</xsl:template>
	<xsl:template name="GetNumStart">
		<xsl:param name="num_id"/>
		<xsl:param name="num_lvl"/>
		<xsl:variable name="absnum_id">
			<xsl:value-of select="key('numbering-num', $num_id)/w:abstractNumId/@w:val"/>
		</xsl:variable>
		<xsl:value-of select="key('numbering-abstract', $absnum_id)/w:lvl[@w:ilvl=$num_lvl]/w:start/@w:val"/>
	</xsl:template>
	<xsl:template name="CreateStyles">
		<xsl:param name="numId"/>
		<xsl:param name="numLvl"/>
		<style-complete>
			<xsl:apply-templates select="w:pPr" mode="getRunPr">
				<xsl:with-param name="style" select="'complete'"/>
			</xsl:apply-templates>
			<xsl:apply-templates select="key('numbering-num', $numId)">
				<xsl:with-param name="list-level" select="$numLvl"/>
				<xsl:with-param name="mode" select="'run'"/>
				<xsl:with-param name="target" select="'complete'"/>
			</xsl:apply-templates>
		</style-complete>
		<style-reduce>
			<xsl:apply-templates select="w:pPr" mode="getRunPr">
				<xsl:with-param name="style" select="'complete'"/>
			</xsl:apply-templates>
			<xsl:apply-templates select="key('numbering-num', $numId)">
				<xsl:with-param name="list-level" select="$numLvl"/>
				<xsl:with-param name="mode" select="'run'"/>
				<xsl:with-param name="target" select="'reduce'"/>
			</xsl:apply-templates>
		</style-reduce>
	</xsl:template>
	<xsl:template match="w:num">
		<xsl:param name="mode"/>
		<xsl:param name="target"/>
		<xsl:param name="list-level"/>
		<xsl:apply-templates select="w:lvlOverride[@w:ilvl = $list-level]/w:lvl">
			<xsl:with-param name="mode" select="$mode"/>
			<xsl:with-param name="target" select="$target"/>
		</xsl:apply-templates>
		<xsl:apply-templates select="key('numbering-abstract',w:abstractNumId/@w:val)">
			<xsl:with-param name="list-level" select="$list-level"/>
			<xsl:with-param name="mode" select="$mode"/>
			<xsl:with-param name="target" select="$target"/>
		</xsl:apply-templates>
	</xsl:template>
	<xsl:template match="w:abstractNum">
		<xsl:param name="mode"/>
		<xsl:param name="target"/>
		<xsl:param name="list-level"/>
		<xsl:apply-templates select="w:lvl[@w:ilvl = $list-level]">
			<xsl:with-param name="mode" select="$mode"/>
			<xsl:with-param name="target" select="$target"/>
		</xsl:apply-templates>
	</xsl:template>
	<xsl:template match="w:lvl">
		<xsl:param name="mode"/>
		<xsl:param name="target"/>
		<xsl:choose>
			<xsl:when test="$mode = 'block'">
				<xsl:call-template name="ApplyParaProperties">
					<xsl:with-param name="target" select="$target"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$mode = 'run' and $target = 'complete'">
				<xsl:call-template name="GetRunProperties">
					<xsl:with-param name="style" select="'complete'"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$mode = 'run' and $target = 'reduce'">
				<xsl:call-template name="GetRunProperties">
					<xsl:with-param name="style" select="'reduce'"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:call-template name="ApplyRunProperties"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<!-- TAB -->
	<xsl:template match="w:tabs">
		<fo:tabs>
			<xsl:apply-templates select="w:tab">
				<xsl:with-param name="mode" select="'settings'"/>
				<xsl:sort order="ascending" select="@w:pos" data-type="number"/>
			</xsl:apply-templates>
		</fo:tabs>
	</xsl:template>
	<xsl:template match="w:tab">
		<xsl:param name="mode"/>
		<xsl:choose>
			<xsl:when test="$mode='settings'">
				<fo:tab position="{concat(@w:pos div 20, 'pt')}">
					<xsl:if test="@w:leader">
						<xsl:attribute name="leader">
							<xsl:value-of select="@w:leader"/>
						</xsl:attribute>
					</xsl:if>
					<xsl:if test="@w:val">
						<xsl:attribute name="val">
							<xsl:value-of select="@w:val"/>
						</xsl:attribute>
					</xsl:if>
				</fo:tab>
			</xsl:when>
			<xsl:otherwise>
				<fo:tab/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template match="w:defaultTabStop">
		<xsl:attribute name="default-tab-size">
			<xsl:value-of select="@w:val div 20"/>
			<xsl:text>pt</xsl:text>
		</xsl:attribute>
	</xsl:template>
	<!-- RUN -->
	<xsl:template match="w:r">
		<xsl:apply-templates select="./*" mode="run"/>
		<xsl:if test="count(./*) - count(./w:rPr) = 0">
			<fo:leader>
				<xsl:call-template name="ApplyRunProperties"/>
			</fo:leader>
		</xsl:if>
	</xsl:template>
	<xsl:template match="mc:AlternateContent" mode="run">
		<xsl:element name="fo:alternate-content">
			<xsl:apply-templates select="."/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="w:drawing" mode="run">
		<xsl:element name="fo:alternate-content">
			<xsl:apply-templates select=".">
				<xsl:with-param name="type" select="'property'"/>
			</xsl:apply-templates>
			<xsl:apply-templates select=".">
				<xsl:with-param name="type" select="'content'"/>
			</xsl:apply-templates>
		</xsl:element>
	</xsl:template>
	<xsl:template match="w:pict | w:object" mode="run">
		<xsl:for-each select="./*">
			<xsl:choose>
				<xsl:when test="name(.) = 'v:oval' or name(.) = 'v:rect' or name(.) = 'v:roundrect' or name(.) = 'v:shape' or name(.) = 'v:line'">
					<xsl:call-template name="SetShapeToAltContent"/>
				</xsl:when>
				<xsl:when test="name(.) = 'v:group'">
					<xsl:call-template name="SetGroupShapes"/>
				</xsl:when>
			</xsl:choose>
		</xsl:for-each>
	</xsl:template>
	<xsl:template match="w:footnoteReference" mode="run">
		<xsl:variable name="footnote-current-id">
			<xsl:value-of select="./@w:id"/>
		</xsl:variable>

		<xsl:variable name="footnote-text-mark">
			<xsl:choose>
				<xsl:when test="./@w:customMarkFollows">
					<xsl:value-of select="./@w:customMarkFollows"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>0</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="footnote-current-symbol">
			<xsl:choose>
				<xsl:when test="$footnote-text-mark = 1">
					<xsl:for-each select="following-sibling::w:t">
						<xsl:value-of select="text()"/>
					</xsl:for-each>
				</xsl:when>
				<xsl:otherwise>
					<!--TODO доделать-->
					<xsl:number level="any" count="w:r[child::w:footnoteReference]" format="1"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<fo:footnote>
			<fo:inline>
				<xsl:for-each select="parent::w:r">
					<xsl:call-template name="ApplyRunProperties"/>
				</xsl:for-each>
				<text>
					<xsl:value-of select="$footnote-current-symbol"/>
				</text>
			</fo:inline>
			<fo:footnote-body>
				<xsl:apply-templates select="/oox:package/oox:part/w:footnotes/w:footnote[@w:id = $footnote-current-id]/w:p">
					<xsl:with-param name="footnote-current-symbol" select="number($footnote-current-symbol)"/>
				</xsl:apply-templates>
			</fo:footnote-body>
		</fo:footnote>
	</xsl:template>
	<xsl:template match="w:endnoteReference" mode="run">
		<xsl:variable name="endnote-current-id">
			<xsl:value-of select="./@w:id"/>
		</xsl:variable>

		<xsl:variable name="endnote-text-mark">
			<xsl:choose>
				<xsl:when test="./@w:customMarkFollows">
					<xsl:value-of select="./@w:customMarkFollows"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="endnote-current-symbol">
			<xsl:choose>
				<xsl:when test="$endnote-text-mark = 1">
					<xsl:for-each select="following-sibling::w:t">
						<xsl:value-of select="text()"/>
					</xsl:for-each>
				</xsl:when>
				<xsl:otherwise>
					<!--TODO доделать-->
					<xsl:number level="any" count="w:r[child::w:endnoteReference]" format="i"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<fo:inline>
			<xsl:for-each select="parent::w:r">
				<xsl:call-template name="ApplyRunProperties"/>
			</xsl:for-each>
			<text>
				<xsl:value-of select="$endnote-current-symbol"/>
			</text>
		</fo:inline>
	</xsl:template>
	<xsl:template match="w:fldChar" mode="run">
		<xsl:apply-templates select="."/>
	</xsl:template>
	<xsl:template match="w:instrText" mode="run">
		<xsl:choose>
			<xsl:when test="preceding-sibling::w:rPr">
				<fo:inline>
					<xsl:for-each select="parent::w:r">
						<xsl:call-template name="ApplyRunProperties"/>
					</xsl:for-each>
					<xsl:apply-templates select="."/>
				</fo:inline>
			</xsl:when>
			<xsl:otherwise>
				<xsl:apply-templates select="."/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template match="w:t | w:tab | w:br" mode="run">
		<fo:inline>
			<xsl:for-each select="parent::w:r">
				<xsl:call-template name="ApplyRunProperties"/>
			</xsl:for-each>
			<xsl:apply-templates select="."/>
		</fo:inline>
	</xsl:template>
	<xsl:template match="w:sym" mode="run">
		<fo:inline>
			<xsl:variable name="font" select="./@w:font"/>
			<xsl:for-each select="parent::w:r">
				<xsl:call-template name="ApplyRunProperties">
					<xsl:with-param name="font" select="$font"/>
				</xsl:call-template>
			</xsl:for-each>
			<xsl:apply-templates select="."/>
		</fo:inline>
	</xsl:template>
	<!-- run -->
	<!--<xsl:template match="w:r">
		<xsl:choose>
			<xsl:when test="mc:AlternateContent">
				<xsl:element name="fo:alternate-content">
					<xsl:apply-templates select="mc:AlternateContent"/>
				</xsl:element>
			</xsl:when>
			<xsl:when test="w:drawing">
				<xsl:element name="fo:alternate-content">
					<xsl:apply-templates select="w:drawing">
						<xsl:with-param name="type" select="'property'"/>
					</xsl:apply-templates>
					<xsl:apply-templates select="w:drawing">
						<xsl:with-param name="type" select="'content'"/>
					</xsl:apply-templates>
				</xsl:element>
			</xsl:when>
			<xsl:when test="w:pict | w:object">
				<xsl:for-each select="./*[name(.) = 'w:pict' or name(.) = 'w:object']/*">
					<xsl:choose>
						<xsl:when test="name(.) = 'v:oval' or name(.) = 'v:rect' or name(.) = 'v:roundrect' or name(.) = 'v:shape' or name(.) = 'v:line'">
							<xsl:call-template name="SetShapeToAltContent"/>
						</xsl:when>
						<xsl:when test="name(.) = 'v:group'">
							<xsl:call-template name="SetGroupShapes"/>
						</xsl:when>
					</xsl:choose>
				</xsl:for-each>
			</xsl:when>      
			<xsl:when test="w:footnoteReference">
				<xsl:variable name="footnote-current-id">
					<xsl:value-of select="w:footnoteReference/@w:id"/>
				</xsl:variable>

        <xsl:variable name="footnote-text-mark">
          <xsl:choose>
            <xsl:when test="w:footnoteReference/@w:customMarkFollows">
              <xsl:value-of select="w:footnoteReference/@w:customMarkFollows"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>0</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>       

				<xsl:variable name="footnote-current-symbol">
					<xsl:number level="any" count="/oox:package/oox:part/w:document//w:footnoteReference" format="1"/>
					<xsl:number level="any" from="/oox:package/oox:part/w:document" count="//w:footnoteReference" format="1"/>
          <xsl:choose>
            <xsl:when test="$footnote-text-mark = 1">
              <xsl:for-each select="w:footnoteReference">
                <xsl:for-each select="following-sibling::w:t">
                  <xsl:value-of select="text()"/>
                </xsl:for-each>
              </xsl:for-each>              
            </xsl:when>
            <xsl:otherwise>
              <xsl:number level="any" count="w:r[child::w:footnoteReference]" format="1"/>              
            </xsl:otherwise>
          </xsl:choose>					
				</xsl:variable>

				<fo:footnote>
					<fo:inline>
						<xsl:call-template name="ApplyRunProperties"/>
            <text>
              <xsl:value-of select="$footnote-current-symbol"/>
            </text>
						<xsl:number level="any" count="/oox:package/oox:part/w:document//w:footnoteReference" format="1"/>
						<xsl:number level="any" from="/oox:package/oox:part/w:document" count="//w:footnoteReference" format="1"/>
					</fo:inline>
					<fo:footnote-body>
						<xsl:apply-templates select="/oox:package/oox:part/w:footnotes/w:footnote[@w:id = $footnote-current-id]/w:p">
							<xsl:with-param name="footnote-current-symbol" select="number($footnote-current-symbol)"/>
						</xsl:apply-templates>
					</fo:footnote-body>
				</fo:footnote>
			</xsl:when>
      <xsl:when test="w:endnoteReference">
        <xsl:variable name="endnote-current-id">
          <xsl:value-of select="w:endnoteReference/@w:id"/>
        </xsl:variable>

        <xsl:variable name="endnote-text-mark">
          <xsl:choose>
            <xsl:when test="w:endnoteReference/@w:customMarkFollows">
              <xsl:value-of select="w:endnoteReference/@w:customMarkFollows"/>
            </xsl:when>
          </xsl:choose>
        </xsl:variable>

        <xsl:variable name="endnote-current-symbol">         
          <xsl:choose>
            <xsl:when test="$endnote-text-mark = 1">
              <xsl:for-each select="w:endnoteReference">
                <xsl:for-each select="following-sibling::w:t">
                  <xsl:value-of select="text()"/>
                </xsl:for-each>
              </xsl:for-each>
            </xsl:when>
            <xsl:otherwise>
              <xsl:number level="any" count="w:r[child::w:endnoteReference]" format="i"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>        
          <fo:inline>
            <xsl:call-template name="ApplyRunProperties"/>
            <text>
              <xsl:value-of select="$endnote-current-symbol"/>
            </text>
          </fo:inline>
      </xsl:when>
			<xsl:when test="w:fldChar">
				<xsl:apply-templates select="w:fldChar"/>
			</xsl:when>
			<xsl:when test="w:instrText">
				<xsl:choose>
					<xsl:when test="w:rPr">
						<fo:inline>
							<xsl:call-template name="ApplyRunProperties"/>
							<xsl:apply-templates select="w:instrText"/>
						</fo:inline>
					</xsl:when>
					<xsl:otherwise>
						<xsl:apply-templates select="w:instrText"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:when test="w:t | w:tab | w:br">
				<fo:inline>
					<xsl:call-template name="ApplyRunProperties"/>
					<xsl:apply-templates select="w:t | w:tab | w:br"/>
				</fo:inline>
			</xsl:when>
			<xsl:otherwise>
				<fo:leader/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>-->
	<xsl:template match="w:pPr" mode="getRunPr">
		<xsl:param name="style"/>
		<xsl:call-template name="GetRunProperties">
			<xsl:with-param name="style" select="$style"/>
		</xsl:call-template>
	</xsl:template>
	<xsl:template name="GetRunProperties">
		<xsl:param name="style"/>
		
		<xsl:variable name="runStyle" select="key('styles', w:rPr/w:rStyle/@w:val)[@w:type='character']/w:rPr/*"/>
		<xsl:variable name="paraStyleR" select="key('styles', ancestor::w:p/w:pPr/w:pStyle/@w:val)[@w:type='paragraph']/w:rPr/*"/>
		<xsl:variable name="paraStyleR1" select="key('styles', ancestor::w:p/w:pPr/w:pStyle/@w:val)[@w:type='paragraph']"/>
		<xsl:variable name="docDefStyle" select="key('Part', 'word/styles.xml')/w:styles/w:docDefaults/w:rPrDefault/w:rPr/*"/>
		
		<xsl:choose>
			<xsl:when test="$style = 'complete'">
				<xsl:apply-templates select="$default-character-style"/>
				<xsl:apply-templates select="$paraStyleR1">
					<xsl:with-param name="onlyRun" select="1"/>
				</xsl:apply-templates>
				<xsl:for-each select="$runStyle">
					<xsl:if test="not($paraStyleR[name()=name(current()) and current()/@w:val='off'])">
						<xsl:apply-templates select="."/>
					</xsl:if>
				</xsl:for-each>
				<xsl:apply-templates select="w:rPr/*[not(self::w:rStyle)]"/>
				<!--<xsl:if test="$font != ''">
					<xsl:attribute name="font-family">
						<xsl:value-of select="$font"/>
					</xsl:attribute>
				</xsl:if>-->
			</xsl:when>
			<xsl:when test="$style = 'reduce'">
				<xsl:apply-templates select="$default-character-style"/>
				<xsl:apply-templates select="$paraStyleR1">
					<xsl:with-param name="onlyRun" select="1"/>
				</xsl:apply-templates>
				<xsl:apply-templates select="w:rPr/*[not(self::w:rStyle)]"/>
				<!--<xsl:if test="$font != ''">
					<xsl:attribute name="font-family">
						<xsl:value-of select="$font"/>
					</xsl:attribute>
				</xsl:if>-->
			</xsl:when>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="ApplyRunPropertiesInParagraph">
		<xsl:variable name="runStyle" select="key('styles', w:rPr/w:rStyle/@w:val)[@w:type='character']/w:rPr/*"/>
		<xsl:variable name="paraStyleR" select="key('styles', ancestor::w:p/w:pPr/w:pStyle/@w:val)[@w:type='paragraph']/w:rPr/*"/>
		<xsl:variable name="paraStyleR1" select="key('styles', ancestor::w:p/w:pPr/w:pStyle/@w:val)[@w:type='paragraph']"/>
		<xsl:variable name="docDefStyle" select="key('Part', 'word/styles.xml')/w:styles/w:docDefaults/w:rPrDefault/w:rPr/*"/>
		<xsl:apply-templates select="$default-character-style"/>
		<xsl:apply-templates select="$paraStyleR1">
			<xsl:with-param name="onlyRun" select="1"/>
		</xsl:apply-templates>
		<xsl:for-each select="$runStyle">
			<xsl:if test="not($paraStyleR[name()=name(current()) and current()/@w:val='off'])">
				<xsl:apply-templates select="."/>
			</xsl:if>
		</xsl:for-each>
		<xsl:apply-templates select="w:rPr/*[not(self::w:rStyle)]"/>
	</xsl:template>
	<xsl:template name="ApplyRunProperties">
		<xsl:param name="font"/>
		<xsl:variable name="runStyle" select="key('styles', w:rPr/w:rStyle/@w:val)[@w:type='character']/w:rPr/*"/>
		<xsl:variable name="paraStyleR" select="key('styles', ancestor::w:p/w:pPr/w:pStyle/@w:val)[@w:type='paragraph']/w:rPr/*"/>
		<xsl:variable name="paraStyleR1" select="key('styles', ancestor::w:p/w:pPr/w:pStyle/@w:val)[@w:type='paragraph']"/>
		<xsl:variable name="docDefStyle" select="key('Part', 'word/styles.xml')/w:styles/w:docDefaults/w:rPrDefault/w:rPr/*"/>
		<style-complete>
			<xsl:apply-templates select="$default-character-style"/>
			<xsl:apply-templates select="$paraStyleR1">
				<xsl:with-param name="onlyRun" select="1"/>
			</xsl:apply-templates>
			<xsl:for-each select="$runStyle">
				<xsl:if test="not($paraStyleR[name()=name(current()) and current()/@w:val='off'])">
					<xsl:apply-templates select="."/>
				</xsl:if>
			</xsl:for-each>
			<xsl:apply-templates select="w:rPr/*[not(self::w:rStyle)]"/>
			<xsl:if test="$font != ''">
				<xsl:attribute name="font-family">
					<xsl:value-of select="$font"/>
				</xsl:attribute>
			</xsl:if>
		</style-complete>
		<style-reduce>
			<xsl:apply-templates select="$default-character-style"/>
			<xsl:apply-templates select="$paraStyleR1">
				<xsl:with-param name="onlyRun" select="1"/>
			</xsl:apply-templates>
			<xsl:apply-templates select="w:rPr/*[not(self::w:rStyle)]"/>
			<xsl:if test="$font != ''">
				<xsl:attribute name="font-family">
					<xsl:value-of select="$font"/>
				</xsl:attribute>
			</xsl:if>
		</style-reduce>
	</xsl:template>
	<!-- text -->
	<xsl:template match="w:t">
		<text>
			<xsl:if test="@xml:space">
				<xsl:attribute name="xml:space">
					<xsl:text>preserve</xsl:text>
				</xsl:attribute>
			</xsl:if>
			<xsl:value-of select="."/>
		</text>
	</xsl:template>
	<xsl:template match="w:sym">
		<text>
			<xsl:variable name="length" select="string-length(@w:char)"/>
			<xsl:variable name="char">
				<xsl:value-of select="substring(@w:char, $length - 4 + 1)"/>
			</xsl:variable>
			<xsl:variable name="new_char">
				<xsl:choose>
					<xsl:when test="not(starts-with($char, 'f') or starts-with($char, 'F'))">
						<xsl:value-of select="concat('F', substring($char, 2))"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="$char"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<xsl:variable name="dec">
				<xsl:call-template name="HexToDec2">
					<xsl:with-param name="hex">
						<xsl:value-of select="$new_char"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:variable>
			<xsl:text disable-output-escaping="yes">&amp;#</xsl:text>
			<xsl:value-of select="number($dec)"/>
			<xsl:text>;</xsl:text>
		</text>
	</xsl:template>
	<!-- field char -->
	<xsl:template match="w:fldChar">
		<field-char>
			<xsl:attribute name="type">
				<xsl:value-of select="@w:fldCharType"/>
			</xsl:attribute>
		</field-char>
	</xsl:template>
	<xsl:template match="w:instrText">
		<text>
			<xsl:value-of select="text()"/>
		</text>
	</xsl:template>
	<!-- field simple -->
	<xsl:template match="w:fldSimple">
		<xsl:call-template name="CreateFieldSimple">
			<xsl:with-param name="instruction" select="normalize-space(@w:instr)"/>
			<xsl:with-param name="is-simple" select="1"/>
		</xsl:call-template>
	</xsl:template>
	<xsl:template name="CreateFieldSimple">
		<xsl:param name="instruction"/>
		<xsl:param name="default-text"/>
		<xsl:param name="is-simple" select="0"/>
		<xsl:choose>
			<xsl:when test="starts-with($instruction, 'PAGE')">
				<fo:page-number/>
			</xsl:when>
			<xsl:when test="starts-with($instruction, 'NUMPAGES')">
				<fo:page-number-citation ref-id="{generate-id(/)}"/>
			</xsl:when>
			<xsl:when test="starts-with($instruction, 'SUBJECT')">
				<xsl:choose>
					<xsl:when test="/oox:package/oox:part/cp:coreProperties/dc:subject">
						<xsl:value-of select="/oox:package/oox:part/cp:coreProperties/dc:subject/."/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:apply-templates select="child::*"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:when test="starts-with($instruction, 'TITLE')">
				<xsl:choose>
					<xsl:when test="/oox:package/oox:part/cp:coreProperties/dc:title">
						<xsl:value-of select="/oox:package/oox:part/cp:coreProperties/dc:title/."/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:apply-templates select="child::*"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:when test="starts-with($instruction, 'HYPERLINK') and $is-simple = '1'">
				<fo:basic-link>
					<xsl:attribute name="internal-destination">
						<xsl:choose>
							<xsl:when test="contains($instruction, '\')">
								<xsl:value-of select="normalize-space(substring-after(substring-before($instruction, '\'), 'HYPERLINK'))"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="normalize-space(substring-after($instruction, 'HYPERLINK'))"/>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:attribute>
					<xsl:choose>
						<xsl:when test="$default-text">
							<xsl:value-of select="$default-text"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:apply-templates select="w:r"/>
						</xsl:otherwise>
					</xsl:choose>
				</fo:basic-link>
			</xsl:when>
			<xsl:when test="starts-with($instruction, 'REF')">
				<fo:basic-link>
					<xsl:attribute name="internal-destination">
						<xsl:choose>
							<xsl:when test="contains($instruction, '\')">
								<xsl:value-of select="normalize-space(substring-after(substring-before($instruction, '\'), 'REF'))"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="normalize-space(substring-after($instruction, 'REF'))"/>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:attribute>
					<xsl:value-of select="$default-text"/>
				</fo:basic-link>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$default-text"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<!-- bookmark -->
	<xsl:template match="w:bookmarkStart">
		<xsl:value-of select="@w:name"/>
		<xsl:text>;</xsl:text>
	</xsl:template>

	<!-- ========================= -->
	<!-- Images And Shapes Content -->
	<!-- ========================= -->
	<xsl:template match="mc:AlternateContent">
		<xsl:choose>
			<xsl:when test="not (mc:Fallback/w:pict/v:shape/v:textbox)">
				<xsl:apply-templates select="mc:Choice"/>
				<xsl:apply-templates select="mc:Fallback"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:apply-templates select="mc:Choice"/>
				<xsl:apply-templates select="mc:Fallback"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template match="mc:Choice">
		<xsl:apply-templates select="w:drawing">
			<xsl:with-param name="type" select="'property'"/>
		</xsl:apply-templates>
	</xsl:template>
	<xsl:template match="mc:Fallback">
		<xsl:element name="fo:content">
			<xsl:apply-templates select="w:pict"/>
		</xsl:element>
	</xsl:template>
	<xsl:template name="IsImageInShape">
		<xsl:value-of select="./v:imagedata/@r:id"/>
	</xsl:template>
	<xsl:template name="ImageInShape">
		<xsl:param name="type" select="'w:pict'"/>
		<xsl:element name="fo:property">
			<xsl:attribute name="type">
				<xsl:value-of select="'runtime'"/>
			</xsl:attribute>
			<xsl:element name="fo:wrap">
				<xsl:attribute name="type">
					<xsl:choose>
						<xsl:when test="w:pict/v:shape/w10:wrap/@type">
							<xsl:value-of select="w:pict/v:shape/w10:wrap/@type"/>
						</xsl:when>
						<xsl:when test="w:object/v:shape/w10:wrap/@type">
							<xsl:value-of select="w:pict/v:shape/w10:wrap/@type"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="'none'"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>
			</xsl:element>
			<xsl:element name="inline">
				<xsl:for-each select="ancestor::w:r">
					<xsl:if test="./w:rPr/w:position/@w:val">
						<xsl:attribute name="vert-pos">
							<xsl:value-of select="./w:rPr/w:position/@w:val div 2"/>
						</xsl:attribute>
					</xsl:if>
				</xsl:for-each>
			</xsl:element>
			<xsl:element name="fo:graphic">
				<xsl:attribute name="type">
					<xsl:choose>
						<xsl:when test="$type = 'w:pict' or $type = 'w:object'">
							<xsl:value-of select="'picture'"/>
						</xsl:when>
						<xsl:when test="$type = 'w:shape'">
							<xsl:value-of select="'shape'"/>
						</xsl:when>
						<xsl:when test="$type = 'v:group'">
							<xsl:value-of select="'group'"/>
						</xsl:when>
					</xsl:choose>
				</xsl:attribute>
				<xsl:if test="$type = 'w:pict' or $type = 'w:object'">
					<xsl:element name="fo:source">
						<xsl:attribute name="dir">
							<xsl:value-of select="$avs-properties-temp-directory"/>
						</xsl:attribute>
						<xsl:choose>
							<xsl:when test="$type = 'w:pict'">
								<xsl:call-template name="SolveRelation">
									<xsl:with-param name="id" select="./v:imagedata/@r:id"/>
									<xsl:with-param name="arg" select="'Target'"/>
								</xsl:call-template>
							</xsl:when>
							<xsl:when test="$type = 'w:object'">
								<xsl:call-template name="SolveRelation">
									<xsl:with-param name="id" select="./v:imagedata/@r:id"/>
									<xsl:with-param name="arg" select="'Target'"/>
								</xsl:call-template>
							</xsl:when>
						</xsl:choose>
					</xsl:element>
				</xsl:if>
			</xsl:element>
		</xsl:element>
		<xsl:element name="fo:content">
			<xsl:apply-templates select="."/>
		</xsl:element>
	</xsl:template>
	<xsl:template name="SetShapeToAltContent">
		<xsl:element name="fo:alternate-content">
			<xsl:variable name="is-image">
				<xsl:call-template name="IsImageInShape"/>
			</xsl:variable>
			<xsl:choose>
				<xsl:when test="$is-image !=''">
					<xsl:call-template name="ImageInShape">
						<xsl:with-param name="type" select="'w:pict'"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="count(ancestor/w:object) != 0 and $is-image != ''">
					<xsl:call-template name="ImageInShape">
						<xsl:with-param name="type" select="'w:object'"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name="ImageInShape">
						<xsl:with-param name="type" select="'w:shape'"/>
					</xsl:call-template>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:element>
	</xsl:template>
	<xsl:template match="w:pict | w:object">
		<xsl:for-each select="./*">
			<xsl:choose>
				<xsl:when test="name(.) = 'v:oval' or name(.) = 'v:rect' or name(.) = 'v:roundrect' or name(.) = 'v:shape' or name(.) = 'v:line'">
					<xsl:call-template name="SetShapeToAltContent"/>
				</xsl:when>
				<xsl:when test="name(.) = 'v:group'">
					<xsl:call-template name="SetGroupShapes"/>
				</xsl:when>
			</xsl:choose>
		</xsl:for-each>
	</xsl:template>
	<xsl:template name="SetGroupShapes">
		<fo:alternate-content>
			<fo:property type="runtime">
				<fo:graphic type="group"/>
			</fo:property>
			<fo:content>
				<group>
					<xsl:call-template name="insertBounds"/>
					<xsl:if test="@coordorigin">
						<xsl:element name="originX">
							<xsl:attribute name="val">
								<xsl:value-of select="substring-before(@coordorigin, ',')"/>
							</xsl:attribute>
						</xsl:element>
						<xsl:element name="originY">
							<xsl:attribute name="val">
								<xsl:value-of select="substring-after(@coordorigin, ',')"/>
							</xsl:attribute>
						</xsl:element>
					</xsl:if>
					<xsl:if test="@coordsize">
						<xsl:element name="originWidth">
							<xsl:attribute name="val">
								<xsl:value-of select="substring-before(@coordsize, ',')"/>
							</xsl:attribute>
						</xsl:element>
						<xsl:element name="originHeight">
							<xsl:attribute name="val">
								<xsl:value-of select="substring-after(@coordsize, ',')"/>
							</xsl:attribute>
						</xsl:element>
					</xsl:if>
					<xsl:for-each select="./*">
						<xsl:choose>
							<xsl:when test="name(.) = 'v:oval' or name(.) = 'v:rect' or name(.) = 'v:roundrect' or name(.) = 'v:shape' or name(.) = 'v:line'">
								<xsl:call-template name="SetShapeToAltContent"/>
							</xsl:when>
							<xsl:when test="name(.) = 'v:group'">
								<xsl:call-template name="SetGroupShapes"/>
							</xsl:when>
						</xsl:choose>
					</xsl:for-each>
					<xsl:copy-of select="./w10:wrap"/>
				</group>
			</fo:content>
		</fo:alternate-content>
	</xsl:template>
	<!--<xsl:template match="w:pict | w:object">
		<xsl:copy>
			<xsl:apply-templates select="@*|node()">
				<xsl:with-param name="isNeed" select="'yes'"/>
			</xsl:apply-templates>
		</xsl:copy>
	</xsl:template>-->
	<!--<xsl:template match="v:shape | v:oval | v:rect">
    <xsl:copy>
      <xsl:apply-templates select="@*|node()">
        <xsl:with-param name="isNeed" select="'yes'"/>
      </xsl:apply-templates>
    </xsl:copy>
  </xsl:template>-->
	<xsl:template match="@* | node()">
		<xsl:param name="isNeed" select="'no'"/>
		<xsl:if test="$isNeed='yes'">
			<xsl:copy>
				<xsl:apply-templates select="* | @*">
					<xsl:with-param name="isNeed" select="'yes'"/>
				</xsl:apply-templates>
			</xsl:copy>
		</xsl:if>
	</xsl:template>
	<xsl:template match="@o:gfxdata" priority="2">
		<xsl:param name="isNeed" select="'no'"/>
	</xsl:template>

	<xsl:template match="w:drawing">
		<xsl:param name="type"/>
		<xsl:choose>
			<xsl:when test="$type='property'">
				<xsl:element name="fo:property">
					<xsl:choose>
						<xsl:when test="wp:anchor">
							<xsl:apply-templates select="wp:anchor"/>
						</xsl:when>
						<xsl:when test="wp:inline">
							<xsl:apply-templates select="wp:inline"/>
						</xsl:when>
					</xsl:choose>
				</xsl:element>
			</xsl:when>
			<xsl:when test="$type='content'">
				<xsl:element name="fo:content">
				</xsl:element>
			</xsl:when>
			<xsl:otherwise>
				<fo:otherwise/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template match="wp:anchor">
		<xsl:attribute name="type">
			<xsl:value-of select="'anchor'"/>
		</xsl:attribute>
		<xsl:element name="fo:anchor">
			<xsl:attribute name="distance-top">
				<xsl:call-template name="Convert-em2pt">
					<xsl:with-param name="inputValue" select="@distT"/>
				</xsl:call-template>
			</xsl:attribute>
			<xsl:attribute name="distance-bottom">
				<xsl:call-template name="Convert-em2pt">
					<xsl:with-param name="inputValue" select="@distB"/>
				</xsl:call-template>
			</xsl:attribute>
			<xsl:attribute name="distance-left">
				<xsl:call-template name="Convert-em2pt">
					<xsl:with-param name="inputValue" select="@distL"/>
				</xsl:call-template>
			</xsl:attribute>
			<xsl:attribute name="distance-right">
				<xsl:call-template name="Convert-em2pt">
					<xsl:with-param name="inputValue" select="@distR"/>
				</xsl:call-template>
			</xsl:attribute>
		</xsl:element>
		<xsl:call-template name="WrapType"/>
		<xsl:call-template name="InsertTransformParams"/>
		<xsl:call-template name="InsertStretchParams"/>
		<xsl:call-template name="FillPosition"/>
		<xsl:call-template name="FillExtent"/>
		<xsl:element name="inline">
			<xsl:for-each select="ancestor::w:r">
				<xsl:if test="./w:rPr/w:position/@w:val">
					<xsl:attribute name="vert-pos">
						<xsl:value-of select="./w:rPr/w:position/@w:val div 2"/>
					</xsl:attribute>
				</xsl:if>
			</xsl:for-each>
		</xsl:element>
		<xsl:apply-templates select="wp:wrapTight/wp:wrapPolygon"/>
		<xsl:apply-templates select="a:graphic"/>
	</xsl:template>
	<xsl:template match="wp:inline">
		<xsl:attribute name="type">
			<xsl:value-of select="'inline'"/>
		</xsl:attribute>
		<xsl:call-template name="FillExtent"/>
		<xsl:call-template name="InsertStretchParams"/>
		<xsl:call-template name="InsertTransformParams"/>
		<xsl:element name="inline">
			<xsl:for-each select="ancestor::w:r">
				<xsl:if test="./w:rPr/w:position/@w:val">
					<xsl:attribute name="vert-pos">
						<xsl:value-of select="./w:rPr/w:position/@w:val div 2"/>
					</xsl:attribute>
				</xsl:if>
			</xsl:for-each>
		</xsl:element>
		<xsl:apply-templates select="a:graphic"/>
	</xsl:template>
	<xsl:template name="InsertStretchParams">
		<xsl:variable name="fillRect" select="a:graphic/a:graphicData/pic:pic/pic:blipFill/a:stretch/a:fillRect"/>
		<xsl:if test="$fillRect">
			<xsl:variable name="top">
				<xsl:value-of select="$fillRect/@t div 1000"/>
			</xsl:variable>
			<xsl:variable name="bottom">
				<xsl:value-of select="$fillRect/@b div 1000"/>
			</xsl:variable>
			<xsl:variable name="left">
				<xsl:value-of select="$fillRect/@l div 1000"/>
			</xsl:variable>
			<xsl:variable name="right">
				<xsl:value-of select="$fillRect/@r div 1000"/>
			</xsl:variable>
			<stretch top="{$top}" bottom="{$bottom}" left="{$left}" right="{$right}"/>
		</xsl:if>
	</xsl:template>
	<xsl:template name="InsertTransformParams">
		<xsl:variable name="frame" select="a:graphic/a:graphicData/pic:pic/pic:spPr/a:xfrm"/>
		<xsl:if test="$frame">
			<xsl:variable name="rotate">
				<xsl:value-of select="$frame/@rot div 60000"/>
			</xsl:variable>
			<xsl:variable name="hFlip">
				<xsl:choose>
					<xsl:when test="$frame/@flipH = 'true' or $frame/@flipH = '1'">
						<xsl:text>true</xsl:text>
					</xsl:when>
					<xsl:otherwise>
						<xsl:text>false</xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<xsl:variable name="vFlip">
				<xsl:choose>
					<xsl:when test="$frame/@flipV = 'true' or $frame/@flipV = '1'">
						<xsl:text>true</xsl:text>
					</xsl:when>
					<xsl:otherwise>
						<xsl:text>false</xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<transform rotate="{$rotate}" hFlip="{$hFlip}" vFlip="{$vFlip}"/>
		</xsl:if>
	</xsl:template>
	<xsl:template name="WrapType">
		<xsl:element name="fo:wrap">
			<xsl:attribute name="type">
				<xsl:choose>
					<xsl:when test="wp:wrapTopAndBottom">
						<xsl:value-of select="'topAndBottom'"/>
					</xsl:when>
					<xsl:when test="wp:wrapTight">
						<xsl:value-of select="'tight'"/>
					</xsl:when>
					<xsl:when test="wp:wrapSquare">
						<xsl:value-of select="'square'"/>
					</xsl:when>
					<xsl:when test="wp:wrapThrough">
						<xsl:value-of select="'through'"/>
					</xsl:when>
					<xsl:when test="wp:wrapNone">
						<xsl:value-of select="'none'"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="'none'"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>
			<xsl:attribute name="behind-doc">
				<xsl:value-of select="@behindDoc"/>
			</xsl:attribute>
		</xsl:element>
	</xsl:template>
	<xsl:template name="FillPosition">
		<xsl:element name="fo:position-x">
			<xsl:attribute name="relative-from">
				<xsl:value-of select="wp:positionH/@relativeFrom"/>
			</xsl:attribute>
			<xsl:if test="wp:positionH/wp:align/text()">
				<xsl:attribute name="align">
					<xsl:value-of select="wp:positionH/wp:align/text()"/>
				</xsl:attribute>
			</xsl:if>
			<xsl:call-template name="Convert-em2pt">
				<xsl:with-param name="inputValue" select="wp:positionH/wp:posOffset"/>
			</xsl:call-template>
		</xsl:element>
		<xsl:element name="fo:position-y">
			<xsl:attribute name="relative-from">
				<xsl:value-of select="wp:positionV/@relativeFrom"/>
			</xsl:attribute>
			<xsl:if test="wp:positionV/wp:align/text()">
				<xsl:attribute name="align">
					<xsl:value-of select="wp:positionV/wp:align/text()"/>
				</xsl:attribute>
			</xsl:if>
			<xsl:call-template name="Convert-em2pt">
				<xsl:with-param name="inputValue" select="wp:positionV/wp:posOffset"/>
			</xsl:call-template>
		</xsl:element>
	</xsl:template>
	<xsl:template name="FillExtent">
		<xsl:element name="fo:extent-x">
			<xsl:call-template name="Convert-em2pt">
				<xsl:with-param name="inputValue" select="wp:extent/@cx"/>
			</xsl:call-template>
		</xsl:element>
		<xsl:element name="fo:extent-y">
			<xsl:call-template name="Convert-em2pt">
				<xsl:with-param name="inputValue" select="wp:extent/@cy"/>
			</xsl:call-template>
		</xsl:element>
	</xsl:template>
	<xsl:template match="wp:wrapPolygon">
		<xsl:element name="fo:wrap-polygon">
			<xsl:apply-templates select="*"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="wp:start">
		<xsl:element name="fo:start">
			<xsl:call-template name="FillPoint"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="wp:lineTo">
		<xsl:element name="fo:line-to">
			<xsl:call-template name="FillPoint"/>
		</xsl:element>
	</xsl:template>
	<xsl:template name="FillPoint">
		<xsl:attribute name="x">
			<xsl:value-of select="@x"/>
		</xsl:attribute>
		<xsl:attribute name="y">
			<xsl:value-of select="@y"/>
		</xsl:attribute>
	</xsl:template>
	<xsl:template match="a:graphic">
		<xsl:element name="fo:graphic">
			<xsl:choose>
				<xsl:when test="a:graphicData/@uri='http://schemas.openxmlformats.org/drawingml/2006/picture'">
					<xsl:attribute name="type">
						<xsl:value-of select="'picture'"/>
					</xsl:attribute>
					<xsl:apply-templates select="a:graphicData/pic:pic"/>
				</xsl:when>
				<xsl:when test="a:graphicData/@uri='http://schemas.microsoft.com/office/word/2010/wordprocessingShape'">
					<xsl:attribute name="type">
						<xsl:value-of select="'shape'"/>
					</xsl:attribute>
				</xsl:when>
				<xsl:when test="a:graphicData/@uri='http://schemas.microsoft.com/office/word/2008/6/28/wordprocessingCanvas'">
					<xsl:attribute name="type">
						<xsl:value-of select="'shape'"/>
					</xsl:attribute>
				</xsl:when>
				<xsl:when test="a:graphicData/@uri='http://schemas.microsoft.com/office/word/2010/wordprocessingGroup'">
					<xsl:attribute name="type">
						<xsl:value-of select="'group'"/>
					</xsl:attribute>
				</xsl:when>
			</xsl:choose>
		</xsl:element>
	</xsl:template>
	<xsl:template match="pic:pic">
		<xsl:element name="fo:source">
			<xsl:attribute name="dir">
				<xsl:value-of select="$avs-properties-temp-directory"/>
			</xsl:attribute>
			<xsl:call-template name="SolveRelation">
				<xsl:with-param name="id" select="pic:blipFill/a:blip/@r:embed"/>
				<xsl:with-param name="arg" select="'Target'"/>
			</xsl:call-template>
		</xsl:element>
	</xsl:template>

	<xsl:template name="SolveRelation">
		<xsl:param name="id"/>
		<xsl:param name="arg"/>
		<xsl:for-each select="ancestor-or-self::*">
			<xsl:if test="@oox:name">
				<xsl:variable name="xml-file" select="@oox:name"/>
				<xsl:variable name="relation-file" select="concat('word/_rels/',substring-after($xml-file, 'word/'), '.rels')"/>
				<xsl:value-of select="key('Part', $relation-file)/rels:Relationships/rels:Relationship[@Id=$id]/@Target"/>
			</xsl:if>
		</xsl:for-each>
		<!--
    <xsl:choose>
      <xsl:when test="$arg = 'Target'">
        <xsl:value-of select="$relationships/rels:Relationship[@Id=$id]/@Target"/>
      </xsl:when>
      <xsl:when test="$arg = 'TargetMode'">
        <xsl:value-of select="$relationships/rels:Relationship[@Id=$id]/@TargetMode"/>
      </xsl:when>
    </xsl:choose>
    -->
	</xsl:template>
	<!-- tags -->
	<xsl:template match="w:sdt">
		<xsl:apply-templates select="w:sdtContent/w:p"/>
		<xsl:apply-templates select="w:sdtContent/w:r"/>
		<xsl:apply-templates select="w:sdtContent/w:tc"/>
	</xsl:template>
	<xsl:template match="w:smartTag">
		<xsl:choose>
			<xsl:when test="w:smartTag">
				<xsl:apply-templates select="w:smartTag"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:if test="w:r">
					<xsl:apply-templates select="w:r"/>
				</xsl:if>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!-- ======================================== -->
	<!-- Represents Keep Lines Together option:   -->
	<!-- prevents a page break in this paragraph. -->
	<!-- parent elements: w:pPr                   -->
	<!-- ======================================== -->
	<xsl:template match="w:keepLines">
		<xsl:attribute name="keep-together.within-page">
			<xsl:choose>
				<xsl:when test="@w:val='off'">
					<xsl:text>auto</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>always</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:attribute>
	</xsl:template>

	<!-- ============================================ -->
	<!-- Represents Keep with Next Paragraph option:  -->
	<!-- prevents a page break between this paragraph -->
	<!-- and the next.                                -->
	<!-- parent elements: w:pPr                       -->
	<!-- ============================================ -->
	<xsl:template match="w:keepNext">
		<xsl:attribute name="keep-with-next">
			<xsl:choose>
				<xsl:when test="@w:val='off'">
					<xsl:text>auto</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>always</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:attribute>
	</xsl:template>


	<!-- ============================================ -->
	<!-- Represents Widow/Orphan Control option:      -->
	<!-- prevents Word from printing the last line of -->
	<!-- a paragraph by itself at the top of the page -->
	<!-- (widow) or the first line of a paragraph at  -->
	<!-- the bottom of a page (orphan).               -->
	<!-- parent elements: w:pPr                       -->
	<!-- ============================================ -->
	<xsl:template match="w:widowControl">
		<xsl:choose>
			<xsl:when test="@w:val='off'">
				<xsl:attribute name="widows">
					<xsl:text>1</xsl:text>
				</xsl:attribute>
				<xsl:attribute name="orphans">
					<xsl:text>1</xsl:text>
				</xsl:attribute>
			</xsl:when>
			<xsl:otherwise>
				<xsl:attribute name="widows">
					<xsl:text>2</xsl:text>
				</xsl:attribute>
				<xsl:attribute name="orphans">
					<xsl:text>2</xsl:text>
				</xsl:attribute>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>



	<!--
  Отступ абзаца. 
  parent elements: w:pPr
  -->
	<xsl:template match="w:ind">
		<!-- start-indent/end-indent -->
		<xsl:if test="@w:start">
			<xsl:attribute name="start-indent">
				<xsl:value-of select="@w:start div 20"/>
				<xsl:text>pt</xsl:text>
			</xsl:attribute>
		</xsl:if>
		<xsl:if test="@w:startChars">
			<xsl:attribute name="start-indent">
				<xsl:value-of select="@w:startChars *.12"/>
				<xsl:text>pt</xsl:text>
			</xsl:attribute>
		</xsl:if>
		<xsl:if test="@w:end">
			<xsl:attribute name="end-indent">
				<xsl:value-of select="@w:end div 20"/>
				<xsl:text>pt</xsl:text>
			</xsl:attribute>
		</xsl:if>
		<xsl:if test="@w:endChars">
			<xsl:attribute name="end-indent">
				<xsl:value-of select="@w:endChars *.12"/>
				<xsl:text>pt</xsl:text>
			</xsl:attribute>
		</xsl:if>
		<!-- text-indent -->
		<xsl:if test="@w:firstLine">
			<xsl:attribute name="text-indent">
				<xsl:value-of select="@w:firstLine div 20"/>
				<xsl:text>pt</xsl:text>
			</xsl:attribute>
		</xsl:if>
		<xsl:if test="@w:firstLineChars">
			<xsl:attribute name="text-indent">
				<xsl:value-of select="@w:firstLineChars *.12"/>
				<xsl:text>pt</xsl:text>
			</xsl:attribute>
		</xsl:if>
		<xsl:if test="@w:hanging">
			<xsl:attribute name="text-indent">
				<xsl:value-of select="- @w:hanging div 20"/>
				<xsl:text>pt</xsl:text>
			</xsl:attribute>
		</xsl:if>
		<xsl:if test="@w:hangingChars">
			<xsl:attribute name="text-indent">
				<xsl:value-of select="- @w:hangingChars *.12"/>
				<xsl:text>pt</xsl:text>
			</xsl:attribute>
		</xsl:if>

		<!-- Следующие имена являются устаревшими (использовались в Word XML 2003) -->
		<xsl:if test="@w:left">
			<xsl:attribute name="start-indent">
				<xsl:value-of select="@w:left div 20"/>
				<xsl:text>pt</xsl:text>
			</xsl:attribute>
		</xsl:if>
		<xsl:if test="@w:left-chars">
			<xsl:attribute name="start-indent">
				<xsl:value-of select="@w:left-chars *.12"/>
				<xsl:text>pt</xsl:text>
			</xsl:attribute>
		</xsl:if>
		<xsl:if test="@w:right">
			<xsl:attribute name="end-indent">
				<xsl:value-of select="@w:right div 20"/>
				<xsl:text>pt</xsl:text>
			</xsl:attribute>
		</xsl:if>
		<xsl:if test="@w:right-chars">
			<xsl:attribute name="end-indent">
				<xsl:value-of select="@w:right-chars *.12"/>
				<xsl:text>pt</xsl:text>
			</xsl:attribute>
		</xsl:if>
		<!-- text-indent -->
		<xsl:if test="@w:first-line">
			<xsl:attribute name="text-indent">
				<xsl:value-of select="@w:first-line div 20"/>
				<xsl:text>pt</xsl:text>
			</xsl:attribute>
		</xsl:if>
		<xsl:if test="@w:first-line-chars">
			<xsl:attribute name="text-indent">
				<xsl:value-of select="@w:first-line-chars *.12"/>
				<xsl:text>pt</xsl:text>
			</xsl:attribute>
		</xsl:if>
		<xsl:if test="@w:hanging-chars">
			<xsl:attribute name="text-indent">
				<xsl:value-of select="- @w:hanging-chars *.12"/>
				<xsl:text>pt</xsl:text>
			</xsl:attribute>
		</xsl:if>

	</xsl:template>

	<xsl:template match="w:pBdr">
		<xsl:if test="./w:left">
			<xsl:call-template name="InsertBorderSide">
				<xsl:with-param name="isTableBorder" select="'true'"/>
				<xsl:with-param name="side" select="./w:left"/>
				<xsl:with-param name="sideName" select="'left'"/>
				<xsl:with-param name="emulateOpenOfficeTableBorders" select="'true'" />
			</xsl:call-template>
		</xsl:if>
		<xsl:if test="./w:right">
			<xsl:call-template name="InsertBorderSide">
				<xsl:with-param name="isTableBorder" select="'true'"/>
				<xsl:with-param name="side" select="./w:right"/>
				<xsl:with-param name="sideName" select="'right'"/>
				<xsl:with-param name="emulateOpenOfficeTableBorders" select="'true'" />
			</xsl:call-template>
		</xsl:if>
		<xsl:if test="./w:bottom">
			<xsl:call-template name="InsertBorderSide">
				<xsl:with-param name="isTableBorder" select="'true'"/>
				<xsl:with-param name="side" select="./w:bottom"/>
				<xsl:with-param name="sideName" select="'bottom'"/>
				<xsl:with-param name="emulateOpenOfficeTableBorders" select="'true'" />
			</xsl:call-template>
		</xsl:if>
		<xsl:if test="./w:top">
			<xsl:call-template name="InsertBorderSide">
				<xsl:with-param name="isTableBorder" select="'true'"/>
				<xsl:with-param name="side" select="./w:top"/>
				<xsl:with-param name="sideName" select="'top'"/>
				<xsl:with-param name="emulateOpenOfficeTableBorders" select="'true'" />
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="w:framePr">
		<xsl:copy-of select="@*"/>
		<xsl:attribute name="frame">
			<xsl:text>true</xsl:text>
		</xsl:attribute>
	</xsl:template>


	<!-- Разрыв страницы перед параграфом -->
	<!-- w:pageBreakBefore -> break-before -->
	<xsl:template match="w:pageBreakBefore">
		<xsl:variable name="BreakIs">
			<xsl:choose>
				<xsl:when test="not(@w:val)">
					<xsl:text>true</xsl:text>
				</xsl:when>
				<xsl:when test="@w:val != 'off' and @w:val != '0' and @w:val != 'false'">
					<xsl:text>true</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>false</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="$BreakIs = 'true'">
				<xsl:attribute name="break-before">
					<xsl:text>page</xsl:text>
				</xsl:attribute>
			</xsl:when>
			<xsl:otherwise>
				<xsl:attribute name="break-before">
					<xsl:text>off</xsl:text>
				</xsl:attribute>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!-- выравнивание строк по ширине -->
	<!-- w:jc -> text-align -->
	<!--
  /TODO:
  @w:val='mediumKashida'
  @w:val='numTab'
  @w:val='highKashida'
  @w:val='lowKashida'
  @w:val='thaiDistribute'
  -->
	<xsl:template match="w:jc">
		<xsl:attribute name="text-align">
			<xsl:choose>
				<xsl:when test="@w:val='left' or @w:val='start'">
					<xsl:text>left</xsl:text>
				</xsl:when>
				<xsl:when test="@w:val='right' or @w:val='end'">
					<xsl:text>right</xsl:text>
				</xsl:when>
				<xsl:when test="@w:val='center'">
					<xsl:text>center</xsl:text>
				</xsl:when>
				<xsl:when test="@w:val='both' or @w:val='distribute'">
					<xsl:text>justify</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>inherit</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:attribute>
		<xsl:if test="@w:val='distribute'">
			<xsl:attribute name="text-align-last">
				<xsl:text>justify</xsl:text>
			</xsl:attribute>
		</xsl:if>
	</xsl:template>

	<!-- w:sz -->
	<xsl:template match="w:sz">
		<xsl:attribute name="font-size">
			<xsl:value-of select="@w:val div 2"/>
			<xsl:text>pt</xsl:text>
		</xsl:attribute>
	</xsl:template>

	<xsl:template match="w:szCs">
		<!--<xsl:if test="count(parent::*/w:sz) = 0">
			<xsl:attribute name="font-size">
				<xsl:value-of select="@w:val div 2"/>
				<xsl:text>pt</xsl:text>
			</xsl:attribute>
		</xsl:if>-->
	</xsl:template>

	<xsl:template match="w:kern">
		<!-- /TODO -->
	</xsl:template>

	<!-- w:b -> font-weight bold -->
	<xsl:template match="w:b">
		<xsl:attribute name="font-weight">
			<xsl:choose>
				<xsl:when test="(@w:val = 'off') or (@w:val = '0') or (@w:val = 'false')">
					<xsl:text>normal</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>bold</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:attribute>
	</xsl:template>

	<!-- w:i -> font-style italic -->
	<xsl:template match="w:i">
		<xsl:attribute name="font-style">
			<xsl:choose>
				<xsl:when test="(@w:val = 'off') or (@w:val = '0') or (@w:val = 'false')">
					<xsl:text>normal</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>italic</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:attribute>
	</xsl:template>

	<!-- TODO: ! -->
	<xsl:template match="w:shadow">
		<xsl:attribute name="text-shadow">
			<xsl:choose>
				<xsl:when test="(@w:val = 'off') or (@w:val = '0') or (@w:val = 'false')">
					<xsl:text>none</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>black</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:attribute>
	</xsl:template>

	<xsl:template match="w:caps">
		<xsl:choose>
			<xsl:when test="@w:val">
				<xsl:choose>
					<xsl:when test="(@w:val = 'off') or (@w:val = '0') or (@w:val = 'false')">
					</xsl:when>
					<xsl:otherwise>
						<xsl:attribute name="text-transform">
							<xsl:text>caps</xsl:text>
						</xsl:attribute>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:otherwise>
				<xsl:attribute name="text-transform">
					<xsl:text>caps</xsl:text>
				</xsl:attribute>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template match="w:smallCaps">
		<xsl:choose>
			<xsl:when test="@w:val">
				<xsl:choose>
					<xsl:when test="(@w:val = 'off') or (@w:val = '0') or (@w:val = 'false')">
					</xsl:when>
					<xsl:otherwise>
						<xsl:attribute name="text-transform">
							<xsl:text>smallCaps</xsl:text>
						</xsl:attribute>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:otherwise>
				<xsl:attribute name="text-transform">
					<xsl:text>smallCaps</xsl:text>
				</xsl:attribute>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!-- Зачеркивание -->
	<xsl:template match="w:strike | w:dstrike">
		<xsl:variable name="val">
			<xsl:choose>
				<xsl:when test="@w:val">
					<xsl:value-of select="@w:val"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>true</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:if test="$val != 'false'">
			<xsl:attribute name="strike">
				<xsl:choose>
					<xsl:when test="parent::*/w:strike">
						<xsl:text>single</xsl:text>
					</xsl:when>
					<xsl:otherwise>
						<xsl:text>double</xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>
		</xsl:if>
	</xsl:template>

	<!--Подчеркивание-->
	<xsl:template match="w:u">
		<xsl:attribute name="underline">
			<xsl:choose>
				<xsl:when test="@w:val = ''">
					<xsl:text>single</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="@w:val"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:attribute>
	</xsl:template>


	<!-- w:strike -> text-decoration line-through -->
	<!--<xsl:template match="w:strike">
    <xsl:attribute name="text-decoration">
      <xsl:choose>
        <xsl:when test="(@w:val = 'off') or (@w:val = '0') or (@w:val = 'false')">
          <xsl:text>none</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>line-through</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:attribute>
  </xsl:template> -->

	<!-- w:dstrike -> text-decoration line-through -->
	<!--<xsl:template match="w:dstrike">
    <xsl:attribute name="text-decoration">
      <xsl:choose>
        <xsl:when test="(@w:val = 'off') or (@w:val = '0') or (@w:val = 'false')">
          <xsl:text>none</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>line-through</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:attribute>
  </xsl:template>-->

	<!-- w:u -> text-decoration underline -->
	<!-- <xsl:template match="w:u">
    <xsl:attribute name="text-decoration">
      <xsl:choose>
        <xsl:when test="@w:val = 'none'">
          <xsl:text>none</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>underline</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:attribute>
  </xsl:template> -->

	<xsl:template match="w:rFonts">
		<xsl:choose>
			<xsl:when test="@w:ascii">
				<xsl:attribute name="font-family">
					<xsl:value-of select="@w:ascii"/>
				</xsl:attribute>
			</xsl:when>
			<xsl:when test="@w:hAnsi">
				<xsl:attribute name="font-family">
					<xsl:value-of select="@w:hAnsi"/>
				</xsl:attribute>
			</xsl:when>
		</xsl:choose>
		<xsl:if test="@w:eastAsia">
			<xsl:attribute name="font-family-ea">
				<xsl:value-of select="@w:eastAsia"/>
			</xsl:attribute>
		</xsl:if>
		<xsl:if test="@w:cs">
			<xsl:attribute name="font-family-cs">
				<xsl:value-of select="@w:cs"/>
			</xsl:attribute>
		</xsl:if>
	</xsl:template>

	<xsl:template name="ApplyFontTheme">
		<xsl:variable name="default-theme-font">
			<xsl:value-of select="$font-scheme/a:minorFont/a:latin/@typeface"/>
		</xsl:variable>
		<xsl:if test="$default-theme-font != ''">
			<xsl:attribute name="font-family">
				<xsl:value-of select="$default-theme-font"/>
			</xsl:attribute>
		</xsl:if>
	</xsl:template>

	<!-- w:vertAlign -> baseline-shift and font-size -->
	<xsl:template match="w:vertAlign">

		<xsl:if test="name(../../.)!='w:pPr'">
			<!-- непонятно почему это потребовалось, у свойств параграфа стояло w:wertAlign='superscript', хотя вроде бы не должно быть -->
			<!-- /todo: разобраться почему так -->

			<xsl:choose>
				<xsl:when test="@w:val='superscript'">
					<xsl:attribute name="baseline-shift">
						<xsl:text>super</xsl:text>
					</xsl:attribute>
				</xsl:when>
				<xsl:when test="@w:val='subscript'">
					<xsl:attribute name="baseline-shift">
						<xsl:text>sub</xsl:text>
					</xsl:attribute>
				</xsl:when>
				<xsl:otherwise/>
			</xsl:choose>

		</xsl:if>

	</xsl:template>

	<!-- w:position -> baseline-shift -->
	<xsl:template match="w:position">
		<xsl:attribute name="baseline-shift">
			<xsl:value-of select="@w:val div 2"/>
			<xsl:text>pt</xsl:text>
		</xsl:attribute>
	</xsl:template>

	<!-- w:lang -> language -->
	<!-- Language Reference -->
	<!-- Проверяем, не пустой ли аттрибут, MS Word почему-то записывает пустые <w:lang/> -->
	<xsl:template match="w:lang">
		<xsl:if test="@w:val">
			<xsl:attribute name="language">
				<xsl:call-template name="NormalizeLanguage">
					<xsl:with-param name="inputString" select="@w:val"/>
				</xsl:call-template>
			</xsl:attribute>
		</xsl:if>
	</xsl:template>


	<!-- COLOR -->
	<!-- w:color -> color -->
	<xsl:template match="w:color">
		<xsl:attribute name="color">
			<xsl:call-template name="ConvertHexColor">
				<xsl:with-param name="hexColor" select="@w:val"/>
			</xsl:call-template>
		</xsl:attribute>
	</xsl:template>


	<!-- w:highlight -> background-color -->
	<xsl:template match="w:highlight">
		<xsl:attribute name="background-color">
			<!-- удаляем символы '-' из имени цвета -->
			<xsl:variable name="colorNameText">
				<xsl:call-template name="RemoveSimbols">
					<xsl:with-param name="inputString" select="@w:val"/>
					<xsl:with-param name="simbolToRemove" select="'-'"/>
				</xsl:call-template>
			</xsl:variable>

			<!-- переводим в нижний регистр -->
			<xsl:variable name="colorNameLowerCase">
				<xsl:call-template name="LowerCaseLatin">
					<xsl:with-param name="inputString" select="$colorNameText"/>
				</xsl:call-template>
			</xsl:variable>

			<!-- переводим имя цвета в соответствие со стандартом XSL-FO -->
			<xsl:call-template name="NormalizeColorName">
				<xsl:with-param name="colorName" select="$colorNameLowerCase"/>
			</xsl:call-template>
		</xsl:attribute>
		<!-- match="w:highlight" -->
	</xsl:template>
	<xsl:template match="w:spacing">
		<xsl:choose>
			<xsl:when test="parent::w:pPr">
				<xsl:apply-templates select="." mode="paragraph"/>
			</xsl:when>
			<xsl:when test="parent::w:rPr">
				<xsl:apply-templates select="." mode="run"/>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<xsl:template match="w:spacing" mode="run">
		<xsl:if test="@w:val">
			<xsl:attribute name="letter-spacing">
				<xsl:value-of select="@w:val div 20"/>
				<xsl:text>pt</xsl:text>
			</xsl:attribute>
		</xsl:if>
	</xsl:template>


	<xsl:template match="w:spacing" mode="paragraph">
		<xsl:param name="mode"/>
		<!-- block-level properties:                 -->
		<!-- space-before/space-after or line-height -->
		<!-- Parent element: w:pPr.                  -->
		<xsl:if test="@w:before and not($mode='after')">
			<xsl:attribute name="space-before">
				<xsl:choose>
					<xsl:when test="@w:beforeAutospacing='1' or w:beforeAutospacing='on'">
						<xsl:value-of select="12"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="@w:before div 20"/>
					</xsl:otherwise>
				</xsl:choose>
				<xsl:text>pt</xsl:text>
			</xsl:attribute>
			<xsl:if test="not(@w:before-autospacing) or @w:before-autospacing='off'">
				<xsl:attribute name="space-before.conditionality">
					<xsl:text>retain</xsl:text>
				</xsl:attribute>
			</xsl:if>
		</xsl:if>
		<xsl:if test="@w:beforeLines and not($mode='after')">
			<xsl:attribute name="space-before">
				<xsl:value-of select="@w:beforeLines *.12"/>
				<xsl:text>pt</xsl:text>
			</xsl:attribute>
			<xsl:if test="not(@w:before-autospacing) or @w:before-autospacing='off'">
				<xsl:attribute name="space-before.conditionality">
					<xsl:text>retain</xsl:text>
				</xsl:attribute>
			</xsl:if>
		</xsl:if>
		<xsl:if test="@w:after and not($mode='before')">
			<xsl:attribute name="space-after">
				<xsl:choose>
					<xsl:when test="@w:afterAutospacing='1' or w:afterAutospacing='on'">
						<xsl:value-of select="12"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="@w:after div 20"/>
					</xsl:otherwise>
				</xsl:choose>
				<xsl:text>pt</xsl:text>
			</xsl:attribute>
			<xsl:if test="not(@w:after-autospacing) or @w:before-autospacing='off'">
				<xsl:attribute name="space-after.conditionality">
					<xsl:text>retain</xsl:text>
				</xsl:attribute>
			</xsl:if>
		</xsl:if>
		<xsl:if test="@w:afterLines and not($mode='before')">
			<xsl:attribute name="space-after">
				<xsl:value-of select="@w:afterLines *.12"/>
				<xsl:text>pt</xsl:text>
			</xsl:attribute>
			<xsl:if test="not(@w:after-autospacing) or @w:before-autospacing='off'">
				<xsl:attribute name="space-after.conditionality">
					<xsl:text>retain</xsl:text>
				</xsl:attribute>
			</xsl:if>
		</xsl:if>
		<!-- line-height -->
		<xsl:if test="@w:line and not($mode='before') and not($mode='after')">
			<xsl:choose>
				<xsl:when test="@w:lineRule='atLeast'">
					<xsl:attribute name="line-height.minimum">
						<xsl:value-of select="@w:line div 2.4"/>
						<xsl:text>%</xsl:text>
					</xsl:attribute>
					<xsl:attribute name="line-height">
						<xsl:text>0pt</xsl:text>
					</xsl:attribute>
				</xsl:when>
				<xsl:when test="@w:lineRule='exact'">
					<xsl:attribute name="line-height">
						<xsl:value-of select="@w:line div 20"/>
						<xsl:text>pt</xsl:text>
					</xsl:attribute>
				</xsl:when>
				<xsl:otherwise>
					<!-- MS Word converts multiplier to twips when saving .doc as .xml. -->
					<!-- The conversion however is based on default font size - 12pt -  -->
					<!-- and not depends on real font size.                             -->
					<xsl:attribute name="line-height">
						<!--<xsl:value-of select="@w:line div 2.40"/>
            <xsl:text>%</xsl:text>-->
						<xsl:value-of select="@w:line div 2.4"/>
						<xsl:text>%</xsl:text>
					</xsl:attribute>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:if>
		<!-- inline-level property: letter-spacing -->
		<!-- Parent element: w:rPr.                -->
		<xsl:if test="@w:val">
			<xsl:attribute name="letter-spacing">
				<xsl:value-of select="@w:val div 20"/>
				<xsl:text>pt</xsl:text>
			</xsl:attribute>
		</xsl:if>

		<!-- <xsl:template match="w:spacing"> -->
	</xsl:template>


	<!-- ====================== -->
	<!-- Style-level properties -->
	<!-- ====================== -->
	<xsl:template match="w:style">
		<xsl:param name="specific-attribute"/>
		<xsl:param name="mode"/>
		<xsl:param name="onlyRun"/>

		<!-- Apply "parent" style properties -->
		<xsl:if test="@w:styleId !=  w:basedOn/@w:val">
			<xsl:apply-templates select="key('styles', w:basedOn/@w:val)">
				<xsl:with-param name="specific-attribute" select="$specific-attribute"/>
				<xsl:with-param name="mode" select="$mode"/>
				<xsl:with-param name="onlyRun" select="$onlyRun"/>
			</xsl:apply-templates>
		</xsl:if>

		<xsl:choose>
			<xsl:when test="$onlyRun">
				<xsl:apply-templates select="w:rPr">
					<xsl:with-param name="specific-attribute" select="$specific-attribute"/>
					<xsl:with-param name="mode" select="$mode"/>
				</xsl:apply-templates>
			</xsl:when>
			<xsl:otherwise>
				<!-- Apply paragraph, character, and table properties. -->
				<xsl:apply-templates select="w:pPr|w:rPr|w:tblPr">
					<xsl:with-param name="specific-attribute" select="$specific-attribute"/>
					<xsl:with-param name="mode" select="$mode"/>
				</xsl:apply-templates>
			</xsl:otherwise>
		</xsl:choose>

		<!-- table-style conditional-override properties -->
		<xsl:apply-templates select="w:tblPr/*[not(self::w:trPr) and not(self::w:tcPr)]"/>

	</xsl:template>


	<!-- ===================================== -->
	<!-- Style-level properties fot table-rows -->
	<!-- ===================================== -->
	<xsl:template match="w:style" mode="table-rows">

		<xsl:if test="@w:styleId !=  w:basedOn/@w:val">
			<xsl:apply-templates select="key('styles', w:basedOn/@w:val)" mode="table-rows"/>
		</xsl:if>

		<xsl:apply-templates select="w:trPr"/>

		<xsl:apply-templates select="w:tblStylePr/w:trPr"/>
	</xsl:template>
	<!-- ====================================== -->
	<!-- Style-level properties fot table-cells -->
	<!-- ====================================== -->
	<xsl:template match="w:style" mode="table-cells">

		<xsl:if test="@w:styleId !=  w:basedOn/@w:val">
			<xsl:apply-templates select="key('styles', w:basedOn/@w:val)" mode="table-cells"/>
		</xsl:if>

		<xsl:apply-templates select="w:tblPr/w:tblCellMar | w:tblPr/w:tblBorders/w:insideH | w:tblPr/w:tblBorders/w:insideV"/>
		<xsl:apply-templates select="w:tcPr"/>

		<xsl:apply-templates select="w:tblStylePr/w:tcPr"/>
	</xsl:template>

	<!-- ================ -->
	<!-- Block properties -->
	<!-- ================ -->
	<xsl:template match="w:pPr">
		<xsl:param name="specific-attribute"/>
		<xsl:param name="mode"/>
		<xsl:choose>
			<xsl:when test="$specific-attribute">
				<xsl:apply-templates select="*[name()=$specific-attribute]">
					<xsl:with-param name="mode" select="$mode"/>
				</xsl:apply-templates>
			</xsl:when>
			<xsl:otherwise>
				<xsl:apply-templates select="*[not(self::w:pStyle) and not(self::w:listPr) and not(self::w:numPr) and not(self::w:tabs)]"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<!-- ================= -->
	<!-- Inline properties -->
	<!-- ================= -->
	<xsl:template match="w:rPr">
		<xsl:param name="specific-attribute"/>
		<xsl:param name="mode"/>

		<xsl:choose>
			<xsl:when test="$specific-attribute">
				<xsl:apply-templates select="*[name()=$specific-attribute]">
					<xsl:with-param name="mode" select="$mode"/>
				</xsl:apply-templates>
			</xsl:when>
			<xsl:otherwise>
				<xsl:apply-templates select="*[not(self::w:rStyle)]"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<!-- ======================================== -->
	<!-- Represents shading and background-color. -->
	<!-- parent elements: w:pPr, w:rPr, w:tblPr,  -->
	<!-- w:tblPrEx, w:tcpr.                       -->
	<!-- ======================================== -->
	<!--  <xsl:template match="w:shd">
    <xsl:if test="@w:fill">
      <xsl:attribute name="background-color">
        <xsl:call-template name="ConvertHexColor">
          <xsl:with-param name="hexColor" select="@w:fill"/>
        </xsl:call-template>
      </xsl:attribute>
    </xsl:if>
  </xsl:template> -->

	<!--
  <xsl:template match="w:shd">
    <xsl:if test="@w:color">
      <xsl:attribute name="background-color">
        <xsl:call-template name="ConvertHexColor">
          <xsl:with-param name="hexColor" select="@w:color"/>
        </xsl:call-template>
      </xsl:attribute>
    </xsl:if>
  </xsl:template>
  -->

	<xsl:template match="w:shd">
		<xsl:if test="not(preceding-sibling::w:highlight or following-sibling::w:highlight)">
			<!--<xsl:choose>
					<xsl:when test="@w:color != 'auto'">
						<xsl:attribute name="background-color">
							<xsl:call-template name="ConvertHexColor">
								<xsl:with-param name="hexColor" select="@w:color"/>
							</xsl:call-template>
						</xsl:attribute>
					</xsl:when>
					<xsl:when test="@w:fill != 'auto'">
						<xsl:attribute name="background-color">
							<xsl:call-template name="ConvertHexColor">
								<xsl:with-param name="hexColor" select="@w:fill"/>
							</xsl:call-template>
						</xsl:attribute>
					</xsl:when>
				</xsl:choose>-->
			<xsl:variable name="color">
				<xsl:choose>
					<xsl:when test="@w:color != 'auto'">
						<xsl:call-template name="ConvertHexColor">
							<xsl:with-param name="hexColor" select="@w:color"/>
						</xsl:call-template>
					</xsl:when>
					<xsl:otherwise>
						<xsl:text>#000000</xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<xsl:variable name="fill">
				<xsl:choose>
					<xsl:when test="@w:fill != 'auto'">
						<xsl:call-template name="ConvertHexColor">
							<xsl:with-param name="hexColor" select="@w:fill"/>
						</xsl:call-template>
					</xsl:when>
					<xsl:otherwise>
						<xsl:text>#FFFFFF</xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<xsl:choose>
				<xsl:when test="@w:val = 'solid'">
					<xsl:attribute name="background-color">
						<xsl:value-of select="$color"/>
					</xsl:attribute>
				</xsl:when>
				<xsl:when test="@w:val = 'clear'">
					<xsl:if test="@w:fill != 'auto'">
						<xsl:attribute name="background-color">
							<xsl:value-of select="$fill"/>
						</xsl:attribute>
					</xsl:if>
				</xsl:when>
				<xsl:when test="contains(@w:val, 'pct')">
					<xsl:attribute name="background-color">
						<xsl:call-template name="GetColor">
							<xsl:with-param name="color1" select="substring-after($fill, '#')"/>
							<xsl:with-param name="color2" select="substring-after($color, '#')"/>
							<xsl:with-param name="koef" select="substring-after(@w:val, 'pct')"/>
						</xsl:call-template>
					</xsl:attribute>
				</xsl:when>
			</xsl:choose>
			<!--<xsl:choose>
					<xsl:when test="@w:val = 'solid'">
						<xsl:attribute name="background-color">
							<xsl:choose>
								<xsl:when test="@w:color != 'auto'">
									<xsl:call-template name="ConvertHexColor">
										<xsl:with-param name="hexColor" select="@w:color"/>
									</xsl:call-template>
								</xsl:when>
								<xsl:otherwise>
									<xsl:text>#000000</xsl:text>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:attribute>
					</xsl:when>
					<xsl:when test="@w:val = 'clear'">
						<xsl:choose>
							<xsl:when test="@w:fill != 'auto'">								
								<xsl:attribute name="background-color">
									<xsl:call-template name="ConvertHexColor">
										<xsl:with-param name="hexColor" select="@w:fill"/>
									</xsl:call-template>									
								</xsl:attribute>
							</xsl:when>
						</xsl:choose>
					</xsl:when>
				</xsl:choose>-->
		</xsl:if>
	</xsl:template>

	<xsl:template name="GetColor">
		<xsl:param name="color1"/>
		<xsl:param name="color2"/>
		<xsl:param name="koef"/>

		<xsl:variable name="new_color">
			<xsl:text>#</xsl:text>
			<xsl:call-template name="ConvertColor">
				<xsl:with-param name="hex1" select="substring($color1, 1, 2)"/>
				<xsl:with-param name="hex2" select="substring($color2, 1, 2)"/>
				<xsl:with-param name="koef" select="$koef div 100"/>
			</xsl:call-template>
			<xsl:call-template name="ConvertColor">
				<xsl:with-param name="hex1" select="substring($color1, 3, 2)"/>
				<xsl:with-param name="hex2" select="substring($color2, 3, 2)"/>
				<xsl:with-param name="koef" select="$koef div 100"/>
			</xsl:call-template>
			<xsl:call-template name="ConvertColor">
				<xsl:with-param name="hex1" select="substring($color1, 5, 2)"/>
				<xsl:with-param name="hex2" select="substring($color2, 5, 2)"/>
				<xsl:with-param name="koef" select="$koef div 100"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:value-of select="$new_color"/>
	</xsl:template>

	<xsl:template name="ConvertColor">
		<xsl:param name="hex1"/>
		<xsl:param name="hex2"/>
		<xsl:param name="koef"/>

		<xsl:variable name="dec1">
			<xsl:call-template name="HexToDec">
				<xsl:with-param name="hex" select="$hex1"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:variable name="dec2">
			<xsl:call-template name="HexToDec">
				<xsl:with-param name="hex" select="$hex2"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:variable name="new_dec">
			<xsl:value-of select="ceiling($dec1 + ($dec2 - $dec1) * $koef)"/>
		</xsl:variable>

		<!--<xsl:value-of select="concat('(', $dec1, '_', $dec2, '_', $new_dec, '_',$koef,')')"/>-->

		<xsl:call-template name="DecToHex">
			<xsl:with-param name="dec">
				<xsl:choose>
					<xsl:when test="$new_dec &lt; 0">
						<xsl:value-of select="255 - $new_dec"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="$new_dec"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:with-param>
		</xsl:call-template>
	</xsl:template>

	<xsl:template name="HexToDec">
		<xsl:param name="hex"/>
		<xsl:variable name="cifr1" select="substring($hex, 1, 1)"/>
		<xsl:variable name="dec1">
			<xsl:call-template name="GetDec">
				<xsl:with-param name="hex" select="$cifr1"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="cifr2" select="substring($hex, 2, 1)"/>
		<xsl:variable name="dec2">
			<xsl:call-template name="GetDec">
				<xsl:with-param name="hex" select="$cifr2"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:value-of select="$dec1 * 16 + $dec2"/>
	</xsl:template>

	<xsl:template name="HexToDec2">
		<xsl:param name="hex"/>
		<xsl:param name="resDec" select="0"/>
		<xsl:choose>
			<xsl:when test="string-length($hex) &gt; 0 ">
				<xsl:variable name="cifr" select="substring($hex, 1, 1)"/>
				<xsl:variable name="dec">
					<xsl:call-template name="GetDec">
						<xsl:with-param name="hex" select="$cifr"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:call-template name="HexToDec2">
					<xsl:with-param name="hex" select="substring($hex, 2)"/>
					<xsl:with-param name="resDec" select="$resDec * 16 + $dec"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$resDec"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="GetDec">
		<xsl:param name="hex"/>
		<xsl:variable name="hex1" select="translate($hex,'ABCDEF','abcdef')"/>

		<xsl:choose>
			<xsl:when test="$hex1 = 'a'">
				<xsl:text>10</xsl:text>
			</xsl:when>
			<xsl:when test="$hex1 = 'b'">
				<xsl:text>11</xsl:text>
			</xsl:when>
			<xsl:when test="$hex1 = 'c'">
				<xsl:text>12</xsl:text>
			</xsl:when>
			<xsl:when test="$hex1 = 'd'">
				<xsl:text>13</xsl:text>
			</xsl:when>
			<xsl:when test="$hex1 = 'e'">
				<xsl:text>14</xsl:text>
			</xsl:when>
			<xsl:when test="$hex1 = 'f'">
				<xsl:text>15</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$hex1"/>
			</xsl:otherwise>

		</xsl:choose>
	</xsl:template>

	<xsl:template name="DecToHex">
		<xsl:param name="dec"/>

		<xsl:variable name="first">
			<xsl:value-of select="(number($dec) - number($dec) mod 16) div 16"/>
		</xsl:variable>
		<xsl:variable name="second">
			<xsl:value-of select="(number($dec) mod 16)"/>
		</xsl:variable>
		<xsl:call-template name="hexTable">
			<xsl:with-param name="dec" select="$first"/>
		</xsl:call-template>
		<xsl:call-template name="hexTable">
			<xsl:with-param name="dec" select="$second"/>
		</xsl:call-template>
	</xsl:template>

	<xsl:template name="hexTable">
		<xsl:param name="dec"/>

		<xsl:variable name="decimal">
			<xsl:text>10</xsl:text>
		</xsl:variable>

		<xsl:choose>
			<xsl:when test="number($dec)&lt; 10">
				<xsl:value-of select="$dec"/>
			</xsl:when>
			<xsl:when test="$dec = '10'">
				<xsl:value-of select="'a'"/>
			</xsl:when>
			<xsl:when test="$dec = '11'">
				<xsl:value-of select="'b'"/>
			</xsl:when>
			<xsl:when test="$dec = '12'">
				<xsl:value-of select="'c'"/>
			</xsl:when>
			<xsl:when test="$dec = '13'">
				<xsl:value-of select="'d'"/>
			</xsl:when>
			<xsl:when test="$dec = '14'">
				<xsl:value-of select="'e'"/>
			</xsl:when>
			<xsl:when test="$dec = '15'">
				<xsl:value-of select="'f'"/>
			</xsl:when>
		</xsl:choose>
	</xsl:template>



	<!-- ТАБЛИЦЫ -->
	<!-- ТАБЛИЦА w:tbl  -->
	<xsl:template match="w:tbl">
		<fo:table>
			<!-- Set default-style table properties -->
			<xsl:apply-templates select="$default-table-style"/>
			<!-- Set style-level table properties -->
			<xsl:apply-templates select="key('styles', w:tblPr/w:tblStyle/@w:val)[@w:type='table']"/>
			<!-- Set direct table properties -->
			<!--<xsl:apply-templates select="w:tblPr/*[not(self::w:tblStyle)and not(self::w:tblCellMar)]"/>-->
			<xsl:apply-templates select="w:tblPr/*[not(self::w:tblStyle)]"/>
			<!-- Set table columns -->
			<xsl:apply-templates select="w:tblGrid"/>
			<!-- Internal links -->
			<!-- <xsl:apply-templates select="aml:annotation"/> -->
			<!-- table-header -->
			<xsl:variable name="header-rows" select="w:tr[w:trPr/tblHeader[not(@w:val='off')]]"/>
			<xsl:if test="$header-rows">
				<fo:table-header start-indent="0pt" end-indent="0pt">
					<xsl:apply-templates select="$header-rows"/>
				</fo:table-header>
			</xsl:if>
			<fo:table-body start-indent="0pt" end-indent="0pt">
				<xsl:apply-templates select="w:tr[not(w:trPr/tblHeader) or w:trPr/tblHeader/@w:val='off']"/>
			</fo:table-body>
		</fo:table>
	</xsl:template>

	<xsl:template match="w:tblBorders">
		<!--<xsl:apply-templates select="w:top | w:bottom | w:left | w:right" mode="border"/>-->
	</xsl:template>

	<!-- ====================== -->
	<!-- Parent element: w:tbl. -->
	<!-- ====================== -->
	<xsl:template match="w:tblGrid">
		<xsl:apply-templates select="*"/>
	</xsl:template>


	<!-- СТОЛБЕЦ -->
	<!-- Parent element: w:tblGrid. -->
	<xsl:template match="w:gridCol">
		<fo:table-column>
			<xsl:attribute name="column-number">
				<xsl:number count="w:gridCol"/>
			</xsl:attribute>
			<xsl:attribute name="column-width">
				<xsl:value-of select="@w:w div 20"/>
				<xsl:text>pt</xsl:text>
			</xsl:attribute>
		</fo:table-column>
	</xsl:template>


	<!-- СТРОКА -->
	<!-- Parent element: w:tbl. -->

	<xsl:template match="w:tr">
		<xsl:element name="fo:table-row">
			<xsl:variable name="table-properties" select="ancestor::w:tbl/w:tblPr"/>
			<!-- Set default-style table-row properties -->
			<xsl:apply-templates select="$default-table-style" mode="table-rows"/>
			<!-- Set style-level table-row properties -->
			<xsl:apply-templates select="key('styles', $table-properties/w:tblStyle/@w:val)[@w:type='table']" mode="table-rows"/>
			<!-- Overriden table properties for the row -->
			<xsl:apply-templates select="w:tblPrEx/*"/>
			<!-- Set direct table-row properties -->
			<xsl:apply-templates select="w:trPr/*"/>
			<!-- Generate table cells -->
			<xsl:apply-templates select="w:tc | w:sdt"/>
		</xsl:element>
	</xsl:template>

	<xsl:template match="w:trHeight">
		<xsl:attribute name="height">
			<xsl:value-of select="@w:val div 20"/>
			<xsl:text>pt</xsl:text>
		</xsl:attribute>
	</xsl:template>


	<!-- ЯЧЕЙКА -->
	<!-- Parent element: w:tr.    -->

	<xsl:template match="w:tc">
		<!-- should not generate fo:table-cell when the w:tc is spanned cell -->
		<xsl:choose>
			<xsl:when test="(not(w:tcPr/w:hMerge) or w:tcPr/w:hMerge/@w:val='restart') and (not(w:tcPr/w:vMerge) or w:tcPr/w:vMerge/@w:val='restart')">
				<fo:table-cell>
					<xsl:variable name="last-index" select="count(ancestor::w:tbl/w:tblPr)"/>
					<xsl:variable name="table-properties" select="ancestor::w:tbl/w:tblPr[$last-index]"/>
					<xsl:variable name="cellStyleName">
						<xsl:for-each select="w:tcPr[1]">
							<xsl:call-template name="getTableCellStyleName"/>
						</xsl:for-each>
					</xsl:variable>
					<xsl:variable name="color">
						<xsl:call-template name="InsertBGColors">
							<xsl:with-param name="cellStyleName" select="$cellStyleName"/>
						</xsl:call-template>
					</xsl:variable>
					<xsl:if test="$color != ''">
						<xsl:attribute name="background-color">
							<xsl:value-of select="$color"/>
						</xsl:attribute>
					</xsl:if>
					<xsl:attribute name="name">
						<xsl:value-of select="$cellStyleName"/>
					</xsl:attribute>
					<xsl:call-template name="InsertLeftBbr">
						<xsl:with-param name="cellStyleName" select="$cellStyleName"/>
					</xsl:call-template>
					<xsl:call-template name="InsertRightBbr">
						<xsl:with-param name="cellStyleName" select="$cellStyleName"/>
					</xsl:call-template>
					<xsl:call-template name="InsertTopBbr">
						<xsl:with-param name="cellStyleName" select="$cellStyleName"/>
					</xsl:call-template>
					<xsl:call-template name="InsertBottomBbr">
						<xsl:with-param name="cellStyleName" select="$cellStyleName"/>
					</xsl:call-template>
					<!-- Set default-style table-cell properties -->
					<xsl:apply-templates select="$default-table-style" mode="table-cells"/>
					<!-- Set style-level table-cell properties -->
					<xsl:apply-templates select="key('styles', $table-properties/w:tblStyle/@w:val)[@w:type='table']" mode="table-cells"/>
					<xsl:apply-templates select="$table-properties/*"/>
					<!-- Apply direct table-cell properties defined on table level -->
					<!--<xsl:apply-templates select="$table-properties/w:tblBorders/w:insideH|$table-properties/w:tblBorders/w:insideV|$table-properties/w:tblCellMar"/>-->

					<!-- Set direct table-cell properties -->
					<xsl:apply-templates select="w:tcPr/*"/>

					<!-- The table cell content -->
					<xsl:apply-templates select="*[not(self::w:tcPr)]"/>
				</fo:table-cell>
			</xsl:when>
			<xsl:when test="w:tcPr/w:vMerge or w:tcPr/w:vMerge/@w:val='continue'">
				<fo:table-cell v-merge="continue">
				</fo:table-cell>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<!-- getTableCellStyleName-->
	<xsl:template name="getTableCellStyleName">
		<xsl:param name="styleId" select="ancestor::w:tbl[1]/w:tblPr/w:tblStyle/@w:val"/>
		<xsl:param name="contextCell" select="parent::w:tc"/>
		<xsl:param name="contextRow" select="ancestor::w:tr"/>

		<xsl:variable name="tblLook">
			<xsl:choose>
				<xsl:when test="ancestor::w:tbl[1]/w:tblPr/w:tblLook/@w:val">
					<xsl:value-of select="ancestor::w:tbl[1]/w:tblPr/w:tblLook/@w:val"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>0000</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="firstPart">
			<xsl:value-of select="substring($tblLook, 3, 2)"/>
		</xsl:variable>
		<xsl:variable name="secondPart">
			<xsl:value-of select="substring($tblLook, 1, 2)"/>
		</xsl:variable>

		<xsl:variable name="hasFirstCol" select="($firstPart = '80' or $firstPart = 'A0' or $firstPart = 'C0' or $firstPart = 'E0') and count(key('styles', $styleId)/w:tblStylePr[@w:type = 'firstCol']) != 0"/>
		<xsl:variable name="hasFirstRow" select="($firstPart = '20' or $firstPart = '60' or $firstPart = 'A0' or $firstPart = 'E0') and count(key('styles', $styleId)/w:tblStylePr[@w:type = 'firstRow']) != 0"/>
		<xsl:variable name="hasBand1Vert" select="not(($secondPart = '04' or $secondPart = '05' or $secondPart = '06' or $secondPart = '07') and count(key('styles', $styleId)/w:tblStylePr[@w:type = 'band1Vert']) != 0)"/>
		<xsl:variable name="hasBand1Horz" select="not(($secondPart = '02' or $secondPart = '03' or $secondPart = '06' or $secondPart = '07') and count(key('styles', $styleId)/w:tblStylePr[@w:type = 'band1Horz']) != 0)"/>
		<xsl:variable name="hasLastCol" select="($secondPart = '01' or $secondPart = '03' or $secondPart = '05' or $secondPart = '07') and count(key('styles', $styleId)/w:tblStylePr[@w:type = 'lastCol']) != 0"/>
		<xsl:variable name="hasLastRow" select="($firstPart = '40' or $firstPart = '60' or $firstPart = 'C0' or $firstPart = 'E0') and count(key('styles', $styleId)/w:tblStylePr[@w:type = 'lastRow']) != 0"/>

		<!--<xsl:value-of select="concat($hasFirstCol, '_', $hasFirstRow, '_', $hasBand1Vert, '_', $hasBand1Horz, '_', $hasLastCol, '_', $hasLastRow)"/>-->

		<xsl:choose>
			<xsl:when test="$contextRow/preceding-sibling::w:tr and $contextRow/following-sibling::w:tr">
				<xsl:choose>
					<!--Col From Body-->
					<xsl:when test="$contextCell/preceding-sibling::w:tc and $contextCell/following-sibling::w:tc">
						<xsl:if test="key('styles', $styleId) or key('default-styles', 'table')/w:tblPr/w:tblStyle/@w:val != $styleId">
							<xsl:choose>
								<!--Has FirstCol Style-->
								<xsl:when test="$hasFirstCol">
									<xsl:if test="$hasBand1Vert and count($contextCell/preceding-sibling::w:tc) mod 2 = 1">
										<xsl:text> band1Vert</xsl:text>
									</xsl:if>
									<!--Has FirstRow Style-->
									<xsl:if test="$hasFirstRow and $hasBand1Horz and count($contextRow/preceding-sibling::w:tr) mod 2 = 1">
										<xsl:text> band1Horz</xsl:text>
									</xsl:if>
									<!--Hasn't FirstRow Style -->
									<xsl:if test="not($hasFirstRow) and $hasBand1Horz and count($contextRow/preceding-sibling::w:tr) mod 2 = 0">
										<xsl:text> band1Horz</xsl:text>
									</xsl:if>
								</xsl:when>
								<!--Hasn't FirstCol Style-->
								<xsl:when test="not($hasFirstCol)">
									<xsl:if test="$hasBand1Vert and count($contextCell/preceding-sibling::w:tc) mod 2 = 0">
										<xsl:text> band1Vert</xsl:text>
									</xsl:if>
									<!--Hasn FirstRow Style-->
									<xsl:if test="$hasFirstRow and $hasBand1Horz and count($contextRow/preceding-sibling::w:tr) mod 2 = 1">
										<xsl:text> band1Horz</xsl:text>
									</xsl:if>
									<!--Hasn't FirstRow Style -->
									<xsl:if test="not($hasFirstRow) and $hasBand1Horz and count($contextRow/preceding-sibling::w:tr) mod 2 = 0">
										<xsl:text> band1Horz</xsl:text>
									</xsl:if>
								</xsl:when>
							</xsl:choose>
						</xsl:if>
					</xsl:when>
					<xsl:otherwise>
						<xsl:choose>
							<!--Is Last Col-->
							<xsl:when test="$contextCell/preceding-sibling::w:tc">
								<xsl:if test="key('styles', $styleId) or key('default-styles', 'table')/w:tblPr/w:tblStyle/@w:val != $styleId">
									<xsl:choose>
										<!--Has LastCol Style-->
										<xsl:when test="$hasLastCol">
											<xsl:text> lastCol</xsl:text>
											<!--Has FirstRow Style-->
											<xsl:if test="$hasFirstRow and $hasBand1Horz and count($contextRow/preceding-sibling::w:tr) mod 2 = 1">
												<xsl:text> band1Horz</xsl:text>
											</xsl:if>
											<!--Hasn't FirstRow Style -->
											<xsl:if test="not($hasFirstRow) and $hasBand1Horz and count($contextRow/preceding-sibling::w:tr) mod 2 = 0">
												<xsl:text> band1Horz</xsl:text>
											</xsl:if>
										</xsl:when>
										<!--Hasn't LastCol Style-->
										<xsl:when test="not($hasLastCol)">
											<xsl:if test="$hasFirstCol and $hasBand1Vert and count($contextCell/preceding-sibling::w:tc) mod 2 = 1">
												<xsl:text> band1Vert</xsl:text>
											</xsl:if>
											<xsl:if test="not($hasFirstCol) and $hasBand1Vert and count($contextCell/preceding-sibling::w:tc) mod 2 = 0">
												<xsl:text> band1Vert</xsl:text>
											</xsl:if>
											<!--Has FirstRow Style-->
											<xsl:if test="$hasFirstRow and $hasBand1Horz and count($contextRow/preceding-sibling::w:tr) mod 2 = 1">
												<xsl:text> band1Horz</xsl:text>
											</xsl:if>
											<!--Hasn't FirstRow Style -->
											<xsl:if test="not($hasFirstRow) and $hasBand1Horz and count($contextRow/preceding-sibling::w:tr) mod 2 = 0">
												<xsl:text> band1Horz</xsl:text>
											</xsl:if>
										</xsl:when>
									</xsl:choose>
								</xsl:if>
							</xsl:when>
							<!--Is First Col-->
							<xsl:otherwise>
								<xsl:if test="key('styles', $styleId) or key('default-styles', 'table')/w:tblPr/w:tblStyle/@w:val != $styleId">
									<xsl:choose>
										<!--Has FirstCol Style-->
										<xsl:when test="$hasFirstCol">
											<xsl:text> firstCol</xsl:text>
											<!--Has FirstRow Style-->
											<xsl:if test="$hasFirstRow and $hasBand1Horz and count($contextRow/preceding-sibling::w:tr) mod 2 = 1">
												<xsl:text> band1Horz</xsl:text>
											</xsl:if>
											<!--Hasn't FirstRow Style -->
											<xsl:if test="not($hasFirstRow) and $hasBand1Horz and count($contextRow/preceding-sibling::w:tr) mod 2 = 0">
												<xsl:text> band1Horz</xsl:text>
											</xsl:if>
										</xsl:when>
										<!--Hasn't FirstCol Style-->
										<xsl:when test="not($hasFirstCol)">
											<xsl:if test="$hasBand1Vert">
												<xsl:text> band1Vert</xsl:text>
											</xsl:if>
											<!--Has FirstRow Style-->
											<xsl:if test="$hasFirstRow and $hasBand1Horz and count($contextRow/preceding-sibling::w:tr) mod 2 = 1">
												<xsl:text> band1Horz</xsl:text>
											</xsl:if>
											<!--Hasn't FirstRow Style -->
											<xsl:if test="not($hasFirstRow) and $hasBand1Horz and count($contextRow/preceding-sibling::w:tr) mod 2 = 0">
												<xsl:text> band1Horz</xsl:text>
											</xsl:if>
										</xsl:when>
									</xsl:choose>
								</xsl:if>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:otherwise>
				<xsl:choose>
					<!--Is Last Row-->
					<xsl:when test="$contextRow/preceding-sibling::w:tr">
						<xsl:choose>
							<!--Col From Body -->
							<xsl:when test="$contextCell/preceding-sibling::w:tc and  $contextCell/following-sibling::w:tc">
								<xsl:if test="key('styles', $styleId) or key('default-styles', 'table')/w:tblPr/w:tblStyle/@w:val != $styleId">
									<xsl:choose>
										<!--Has lastRow Style-->
										<xsl:when test="$hasLastRow">
											<xsl:text> lastRow</xsl:text>
											<xsl:if test="$hasFirstCol and $hasBand1Vert and count($contextCell/preceding-sibling::w:tc) mod 2 = 1">
												<xsl:text> band1Vert</xsl:text>
											</xsl:if>
											<xsl:if test="not($hasFirstCol) and $hasBand1Vert and count($contextCell/preceding-sibling::w:tc) mod 2 = 0">
												<xsl:text> band1Vert</xsl:text>
											</xsl:if>
										</xsl:when>
										<!--Hasn't LastRow Style-->
										<xsl:otherwise>
											<xsl:if test="$hasFirstCol and $hasBand1Vert and count($contextCell/preceding-sibling::w:tc) mod 2 = 1">
												<xsl:text> band1Vert</xsl:text>
											</xsl:if>
											<xsl:if test="not($hasFirstCol) and $hasBand1Vert and count($contextCell/preceding-sibling::w:tc) mod 2 = 0">
												<xsl:text> band1Vert</xsl:text>
											</xsl:if>
											<!--Has FirstRow Style-->
											<xsl:if test="$hasFirstRow and $hasBand1Horz and count($contextRow/preceding-sibling::w:tr) mod 2 = 1">
												<xsl:text> band1Horz</xsl:text>
											</xsl:if>
											<!--Hasn't FirstRow Style -->
											<xsl:if test="not($hasFirstRow) and $hasBand1Horz and count($contextRow/preceding-sibling::w:tr) mod 2 = 0">
												<xsl:text> band1Horz</xsl:text>
											</xsl:if>
										</xsl:otherwise>
									</xsl:choose>
								</xsl:if>
							</xsl:when>
							<xsl:otherwise>
								<xsl:choose>
									<!-- Is Last Col-->
									<xsl:when test="$contextCell/preceding-sibling::w:tc">
										<xsl:if test="key('styles', $styleId) or key('default-styles', 'table')/w:tblPr/w:tblStyle/@w:val != $styleId">
											<xsl:choose>
												<!--Has LastRow Style-->
												<xsl:when test="$hasLastRow">
													<xsl:text> lastRow</xsl:text>
													<xsl:if test="$hasLastCol">
														<xsl:text> lastCol</xsl:text>
													</xsl:if>
													<xsl:if test="not($hasLastCol) and $hasFirstCol and $hasBand1Vert and count($contextCell/preceding-sibling::w:tc) mod 2 = 1">
														<xsl:text> band1Vert</xsl:text>
													</xsl:if>
													<xsl:if test="not($hasLastCol) and not($hasFirstCol) and $hasBand1Vert and count($contextCell/preceding-sibling::w:tc) mod 2 = 0">
														<xsl:text> band1Vert</xsl:text>
													</xsl:if>
												</xsl:when>
												<!--Hasn't LastRow Style-->
												<xsl:otherwise>
													<xsl:if test="$hasLastCol">
														<xsl:text> lastCol</xsl:text>
													</xsl:if>
													<xsl:if test="not($hasLastCol) and not($hasFirstCol) and $hasBand1Vert and count($contextCell/preceding-sibling::w:tc) mod 2 = 0">
														<xsl:text> band1Vert</xsl:text>
													</xsl:if>
													<!--Has FirstRow Style-->
													<xsl:if test="not($hasLastCol) and $hasFirstRow and $hasBand1Horz and count($contextRow/preceding-sibling::w:tr) mod 2 = 1">
														<xsl:text> band1Horz</xsl:text>
													</xsl:if>
													<!--Hasn't FirstRow Style -->
													<xsl:if test="not($hasFirstRow) and $hasBand1Horz and count($contextRow/preceding-sibling::w:tr) mod 2 = 0">
														<xsl:text> band1Horz</xsl:text>
													</xsl:if>
												</xsl:otherwise>
											</xsl:choose>
										</xsl:if>
									</xsl:when>
									<!-- Is First Col-->
									<xsl:otherwise>
										<xsl:if test="key('styles', $styleId) or key('default-styles', 'table')/w:tblPr/w:tblStyle/@w:val != $styleId">
											<xsl:choose>
												<!--Has LastRow Style-->
												<xsl:when test="$hasLastRow">
													<xsl:text> lastRow</xsl:text>
													<xsl:if test="$hasFirstCol">
														<xsl:text> firstCol</xsl:text>
													</xsl:if>
													<xsl:if test="not($hasLastCol) and not($hasFirstCol) and $hasBand1Vert">
														<xsl:text> band1Vert</xsl:text>
													</xsl:if>
												</xsl:when>
												<!--Hasn't LastRow Style-->
												<xsl:otherwise>
													<xsl:if test="$hasFirstCol">
														<xsl:text> firstCol</xsl:text>
													</xsl:if>
													<xsl:if test="not($hasFirstCol) and $hasBand1Vert">
														<xsl:text> band1Vert</xsl:text>
													</xsl:if>
													<!--Has FirstRow Style-->
													<xsl:if test="$hasFirstRow and $hasBand1Horz and count($contextRow/preceding-sibling::w:tr) mod 2 = 1">
														<xsl:text> band1Horz</xsl:text>
													</xsl:if>
													<!--Hasn't FirstRow Style -->
													<xsl:if test="not($hasFirstRow) and $hasBand1Horz and count($contextRow/preceding-sibling::w:tr) mod 2 = 0">
														<xsl:text> band1Horz</xsl:text>
													</xsl:if>
												</xsl:otherwise>
											</xsl:choose>
										</xsl:if>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:when>
					<!--Is First Row-->
					<xsl:otherwise>
						<xsl:choose>
							<!--Col From Body -->
							<xsl:when test="$contextCell/preceding-sibling::w:tc and  $contextCell/following-sibling::w:tc">
								<xsl:if test="key('styles', $styleId) or key('default-styles', 'table')/w:tblPr/w:tblStyle/@w:val != $styleId">
									<xsl:choose>
										<!--Has FirstRow Style-->
										<xsl:when test="$hasFirstRow">
											<xsl:text> firstRow</xsl:text>
											<xsl:if test="$hasFirstCol and $hasBand1Vert and count($contextCell/preceding-sibling::w:tc) mod 2 = 1">
												<xsl:text> band1Vert</xsl:text>
											</xsl:if>
											<xsl:if test="not($hasFirstCol) and $hasBand1Vert and count($contextCell/preceding-sibling::w:tc) mod 2 = 0">
												<xsl:text> band1Vert</xsl:text>
											</xsl:if>
										</xsl:when>
										<!--Hasn't FirstRow Style-->
										<xsl:otherwise>
											<xsl:if test="$hasFirstCol and $hasBand1Vert and count($contextCell/preceding-sibling::w:tc) mod 2 = 1">
												<xsl:text> band1Vert</xsl:text>
											</xsl:if>
											<xsl:if test="not($hasFirstCol) and $hasBand1Vert and count($contextCell/preceding-sibling::w:tc) mod 2 = 0">
												<xsl:text> band1Vert</xsl:text>
											</xsl:if>
											<xsl:if test="$hasBand1Horz">
												<xsl:text> band1Horz</xsl:text>
											</xsl:if>
										</xsl:otherwise>
									</xsl:choose>
								</xsl:if>
							</xsl:when>
							<xsl:otherwise>
								<xsl:choose>
									<!-- Is Last Col-->
									<xsl:when test="$contextCell/preceding-sibling::w:tc">
										<xsl:if test="key('styles', $styleId) or key('default-styles', 'table')/w:tblPr/w:tblStyle/@w:val != $styleId">
											<xsl:choose>
												<!--Has FirstRow Style-->
												<xsl:when test="$hasFirstRow">
													<xsl:text> firstRow</xsl:text>
													<xsl:if test="$hasLastCol">
														<xsl:text> lastCol</xsl:text>
													</xsl:if>
													<xsl:if test="not($hasLastCol) and not($hasFirstCol) and $hasBand1Vert and count($contextCell/preceding-sibling::w:tc) mod 2 = 0">
														<xsl:text> band1Vert</xsl:text>
													</xsl:if>
													<xsl:if test="not($hasLastCol) and $hasFirstCol and $hasBand1Vert and count($contextCell/preceding-sibling::w:tc) mod 2 = 1">
														<xsl:text> band1Vert</xsl:text>
													</xsl:if>
												</xsl:when>
												<!--Hasn't FirstRow Style-->
												<xsl:otherwise>
													<xsl:if test="$hasLastCol">
														<xsl:text> lastCol</xsl:text>
													</xsl:if>
													<xsl:if test="not($hasLastCol) and not($hasFirstCol) and $hasBand1Vert and count($contextCell/preceding-sibling::w:tc) mod 2 = 0">
														<xsl:text> band1Vert</xsl:text>
													</xsl:if>
													<xsl:if test="not($hasLastCol) and $hasFirstCol and $hasBand1Vert and count($contextCell/preceding-sibling::w:tc) mod 2 = 1">
														<xsl:text> band1Vert</xsl:text>
													</xsl:if>
													<!--Has FirstRow Style-->
													<xsl:if test="$hasFirstRow and $hasBand1Horz and count($contextRow/preceding-sibling::w:tr) mod 2 = 1">
														<xsl:text> band1Horz</xsl:text>
													</xsl:if>
													<!--Hasn't FirstRow Style -->
													<xsl:if test="not($hasFirstRow) and $hasBand1Horz and count($contextRow/preceding-sibling::w:tr) mod 2 = 0">
														<xsl:text> band1Horz</xsl:text>
													</xsl:if>
												</xsl:otherwise>
											</xsl:choose>
										</xsl:if>
									</xsl:when>
									<!-- Is First Col-->
									<xsl:otherwise>
										<xsl:if test="key('styles', $styleId) or key('default-styles', 'table')/w:tblPr/w:tblStyle/@w:val != $styleId">
											<xsl:choose>
												<!--Has FirstRow Style-->
												<xsl:when test="$hasFirstRow">
													<xsl:text> firstRow</xsl:text>
													<xsl:if test="$hasFirstCol">
														<xsl:text> firstCol</xsl:text>
													</xsl:if>
													<xsl:if test="not($hasFirstCol) and $hasBand1Vert">
														<xsl:text> band1Vert</xsl:text>
													</xsl:if>
												</xsl:when>
												<!--Hasn't FirstRow Style-->
												<xsl:otherwise>
													<xsl:if test="$hasFirstCol">
														<xsl:text> firstCol</xsl:text>
													</xsl:if>
													<xsl:if test="not($hasFirstCol) and $hasBand1Vert">
														<xsl:text> band1Vert</xsl:text>
													</xsl:if>
													<xsl:if test="$hasBand1Horz">
														<xsl:text> band1Horz</xsl:text>
													</xsl:if>
												</xsl:otherwise>
											</xsl:choose>
										</xsl:if>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="getCurName">
		<xsl:param name="str"/>

		<xsl:if test="$str != ''">
			<xsl:choose>
				<xsl:when test="starts-with($str, ' ')">
					<xsl:call-template name="getCurName">
						<xsl:with-param name="str" select="substring-after($str, ' ')"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="contains($str, ' ')">
					<xsl:value-of select="substring-before($str, ' ')"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$str"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:if>
	</xsl:template>

	<xsl:template name="InsertBorderSide">
		<xsl:param name="side"/>
		<xsl:param name="sideName"/>
		<xsl:param name="emulateOpenOfficeTableBorders" select="'false'" />
		<xsl:param name="isTableBorder" select="'false'"/>

		<xsl:choose>
			<xsl:when test="$side/@w:val != 'none' and $side/@w:val != 'nil'">

				<xsl:variable name="style">
					<xsl:choose>
						<xsl:when test="$side/@w:val">
							<xsl:call-template name="ConvertBorderStyle">
								<xsl:with-param name="borderStyle" select="$side/@w:val"/>
							</xsl:call-template>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>solid</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>
				<xsl:attribute name="{concat('border-', $sideName, '-style')}">
					<xsl:value-of select="$style"/>
				</xsl:attribute>
				<xsl:attribute name="{concat('border-', $sideName, '-color')}">
					<xsl:choose>
						<xsl:when test="$side/@w:color and not($side/@w:color='auto')">
							<xsl:call-template name="ConvertHexColor">
								<xsl:with-param name="hexColor" select="$side/@w:color"/>
							</xsl:call-template>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>000000</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>
				<xsl:attribute name="{concat('border-', $sideName, '-width')}">
					<xsl:choose>
						<xsl:when test="$side/@w:sz and not($side/@w:sz='auto')">
							<xsl:choose>
								<xsl:when test="$style = 'double'">
									<xsl:call-template name="ConvertBorderSize">
										<xsl:with-param name="borderSize" select="$side/@w:sz * 2"/>
									</xsl:call-template>
								</xsl:when>
								<xsl:otherwise>
									<xsl:call-template name="ConvertBorderSize">
										<xsl:with-param name="borderSize" select="$side/@w:sz"/>
									</xsl:call-template>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>0</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>
			</xsl:when>
			<xsl:otherwise>
				<xsl:attribute name="{concat('border-', $sideName, '-style')}">
					<xsl:text>none</xsl:text>
				</xsl:attribute>
				<xsl:attribute name="{concat('border-', $sideName, '-color')}">
					<xsl:text>000000</xsl:text>
				</xsl:attribute>
				<xsl:attribute name="{concat('border-', $sideName, '-width')}">
					<xsl:text>0</xsl:text>
				</xsl:attribute>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="InsertBGColors">
		<xsl:param name="cellStyleName"/>
		<xsl:param name="styleId" select="ancestor::w:tbl[1]/w:tblPr/w:tblStyle/@w:val"/>
		<xsl:param name="tcShd" select="w:shd"/>
		<xsl:param name="tblShd" select="ancestor::w:tbl[1]/w:tblPr/w:shd"/>

		<xsl:choose>
			<xsl:when test="$tcShd">
				<xsl:call-template name="BG-color">
					<xsl:with-param name="node" select="$tcShd"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$tblShd">
				<xsl:call-template name="BG-color">
					<xsl:with-param name="node" select="$tblShd"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$cellStyleName != ''">
				<xsl:variable name="curName">
					<xsl:call-template name="getCurName">
						<xsl:with-param name="str" select="$cellStyleName"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:call-template name="InsertBGColors">
					<xsl:with-param name="styleId" select="$styleId"/>
					<xsl:with-param name="tcShd" select="key('styles', $styleId)/w:tblStylePr[@w:type = $curName]/w:tcPr/w:shd"/>
					<xsl:with-param name="tblShd" select="key('styles', $styleId)/w:tblPr/w:shd"/>
					<xsl:with-param name="cellStyleName" select="substring-after($cellStyleName, $curName)"/>
				</xsl:call-template>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="BG-color">
		<xsl:param name="node"/>
		<xsl:for-each select="$node">
			<xsl:choose>
				<xsl:when test="@w:color != 'auto'">
					<xsl:call-template name="ConvertHexColor">
						<xsl:with-param name="hexColor" select="@w:color"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="@w:fill != 'auto'">
					<xsl:call-template name="ConvertHexColor">
						<xsl:with-param name="hexColor" select="@w:fill"/>
					</xsl:call-template>
				</xsl:when>
			</xsl:choose>
		</xsl:for-each>
	</xsl:template>

	<xsl:template name="InsertLeftBbr">
		<xsl:param name="cellStyleName"/>
		<xsl:param name="styleId" select="ancestor::w:tbl[1]/w:tblPr/w:tblStyle/@w:val"/>
		<xsl:param name="tcBorders" select="w:tcBorders"/>
		<xsl:param name="tblBorders" select="ancestor::w:tbl[1]/w:tblPr/w:tblBorders"/>
		<xsl:param name="contextCell" select="."/>
		<xsl:param name="curStyleName"/>

		<xsl:choose>
			<xsl:when test="$tcBorders/w:insideV and $contextCell/preceding-sibling::w:tc and ($curStyleName = 'firstRow' or $curStyleName = 'band1Horz' or $curStyleName = 'lastRow')">
				<xsl:call-template name="InsertBorderSide">
					<xsl:with-param name="isTableBorder" select="'true'"/>
					<xsl:with-param name="side" select="$tcBorders/w:insideV"/>
					<xsl:with-param name="sideName" select="'left'"/>
					<xsl:with-param name="emulateOpenOfficeTableBorders" select="'true'" />
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$tcBorders/w:left and not($contextCell/preceding-sibling::w:tc) and ($curStyleName = 'firstRow' or $curStyleName = 'band1Horz' or $curStyleName = 'lastRow')">
				<xsl:call-template name="InsertBorderSide">
					<xsl:with-param name="isTableBorder" select="'true'"/>
					<xsl:with-param name="side" select="$tcBorders/w:left"/>
					<xsl:with-param name="sideName" select="'left'"/>
					<xsl:with-param name="emulateOpenOfficeTableBorders" select="'true'" />
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$tcBorders/w:left and not($curStyleName = 'firstRow' or $curStyleName = 'band1Horz' or $curStyleName = 'lastRow')">
				<xsl:call-template name="InsertBorderSide">
					<xsl:with-param name="isTableBorder" select="'true'"/>
					<xsl:with-param name="side" select="$tcBorders/w:left"/>
					<xsl:with-param name="sideName" select="'left'"/>
					<xsl:with-param name="emulateOpenOfficeTableBorders" select="'true'" />
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$tblBorders/w:insideV and $contextCell/preceding-sibling::w:tc">
				<xsl:call-template name="InsertBorderSide">
					<xsl:with-param name="isTableBorder" select="'true'"/>
					<xsl:with-param name="side" select="$tblBorders/w:insideV"/>
					<xsl:with-param name="sideName" select="'left'"/>
					<xsl:with-param name="emulateOpenOfficeTableBorders" select="'true'" />
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$tblBorders/w:left and not($contextCell/preceding-sibling::w:tc)">
				<xsl:call-template name="InsertBorderSide">
					<xsl:with-param name="isTableBorder" select="'true'"/>
					<xsl:with-param name="side" select="$tblBorders/w:left"/>
					<xsl:with-param name="sideName" select="'left'"/>
					<xsl:with-param name="emulateOpenOfficeTableBorders" select="'true'" />
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$cellStyleName != ''">
				<xsl:variable name="curName">
					<xsl:call-template name="getCurName">
						<xsl:with-param name="str" select="$cellStyleName"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:call-template name="InsertLeftBbr">
					<xsl:with-param name="styleId" select="$styleId"/>
					<xsl:with-param name="tcBorders" select="key('styles', $styleId)/w:tblStylePr[@w:type = $curName]/w:tcPr/w:tcBorders"/>
					<xsl:with-param name="tblBorders" select="key('styles', $styleId)/w:tblPr/w:tblBorders"/>
					<xsl:with-param name="cellStyleName" select="substring-after($cellStyleName, $curName)"/>
					<xsl:with-param name="curStyleName" select="$curName"/>
				</xsl:call-template>
			</xsl:when>

			<xsl:when test="key('styles', $styleId)/w:tcPr/w:tcBorders or key('styles', $styleId)/w:tblPr/w:tblBorders">
				<xsl:call-template name="InsertLeftBbr">
					<xsl:with-param name="styleId" select="key('styles', $styleId)/w:tblPr/w:tblStyle/@w:val"/>
					<xsl:with-param name="tcBorders" select="key('styles', $styleId)/w:tcPr/w:tcBorders"/>
					<xsl:with-param name="tblBorders" select="key('styles', $styleId)/w:tblPr/w:tblBorders"/>
				</xsl:call-template>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="InsertRightBbr">
		<xsl:param name="cellStyleName"/>
		<xsl:param name="styleId" select="ancestor::w:tbl[1]/w:tblPr/w:tblStyle/@w:val"/>
		<xsl:param name="tcBorders" select="w:tcBorders"/>
		<xsl:param name="tblBorders" select="ancestor::w:tbl[1]/w:tblPr/w:tblBorders"/>
		<xsl:param name="contextCell" select="."/>
		<xsl:param name="curStyleName"/>

		<xsl:choose>
			<xsl:when test="$tcBorders/w:insideV and $contextCell/following-sibling::w:tc and ($curStyleName = 'firstRow' or $curStyleName = 'band1Horz' or $curStyleName = 'lastRow')">
				<xsl:call-template name="InsertBorderSide">
					<xsl:with-param name="isTableBorder" select="'true'"/>
					<xsl:with-param name="side" select="$tcBorders/w:insideV"/>
					<xsl:with-param name="sideName" select="'right'"/>
					<xsl:with-param name="emulateOpenOfficeTableBorders" select="'true'" />
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$tcBorders/w:right and not($contextCell/following-sibling::w:tc) and ($curStyleName = 'firstRow' or $curStyleName = 'band1Horz' or $curStyleName = 'lastRow')">
				<xsl:call-template name="InsertBorderSide">
					<xsl:with-param name="isTableBorder" select="'true'"/>
					<xsl:with-param name="side" select="$tcBorders/w:right"/>
					<xsl:with-param name="sideName" select="'right'"/>
					<xsl:with-param name="emulateOpenOfficeTableBorders" select="'true'" />
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$tcBorders/w:right and not($curStyleName = 'firstRow' or $curStyleName = 'band1Horz' or $curStyleName = 'lastRow')">
				<xsl:call-template name="InsertBorderSide">
					<xsl:with-param name="isTableBorder" select="'true'"/>
					<xsl:with-param name="side" select="$tcBorders/w:right"/>
					<xsl:with-param name="sideName" select="'right'"/>
					<xsl:with-param name="emulateOpenOfficeTableBorders" select="'true'" />
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$tblBorders/w:insideV and $contextCell/following-sibling::w:tc">
				<xsl:call-template name="InsertBorderSide">
					<xsl:with-param name="isTableBorder" select="'true'"/>
					<xsl:with-param name="side" select="$tblBorders/w:insideV"/>
					<xsl:with-param name="sideName" select="'right'"/>
					<xsl:with-param name="emulateOpenOfficeTableBorders" select="'true'" />
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$tblBorders/w:right and not($contextCell/following-sibling::w:tc)">
				<xsl:call-template name="InsertBorderSide">
					<xsl:with-param name="isTableBorder" select="'true'"/>
					<xsl:with-param name="side" select="$tblBorders/w:right"/>
					<xsl:with-param name="sideName" select="'right'"/>
					<xsl:with-param name="emulateOpenOfficeTableBorders" select="'true'" />
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$cellStyleName != ''">
				<xsl:variable name="curName">
					<xsl:call-template name="getCurName">
						<xsl:with-param name="str" select="$cellStyleName"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:call-template name="InsertRightBbr">
					<xsl:with-param name="styleId" select="$styleId"/>
					<xsl:with-param name="tcBorders" select="key('styles', $styleId)/w:tblStylePr[@w:type = $curName]/w:tcPr/w:tcBorders"/>
					<xsl:with-param name="tblBorders" select="key('styles', $styleId)/w:tblPr/w:tblBorders"/>
					<xsl:with-param name="cellStyleName" select="substring-after($cellStyleName, $curName)"/>
					<xsl:with-param name="curStyleName" select="$curName"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="key('styles', $styleId)/w:tcPr/w:tcBorders or key('styles', $styleId)/w:tblPr/w:tblBorders">
				<xsl:call-template name="InsertRightBbr">
					<xsl:with-param name="styleId" select="key('styles', $styleId)/w:tblPr/w:tblStyle/@w:val"/>
					<xsl:with-param name="tcBorders" select="key('styles', $styleId)/w:tcPr/w:tcBorders"/>
					<xsl:with-param name="tblBorders" select="key('styles', $styleId)/w:tblPr/w:tblBorders"/>
				</xsl:call-template>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="InsertTopBbr">
		<xsl:param name="cellStyleName"/>
		<xsl:param name="styleId" select="ancestor::w:tbl[1]/w:tblPr/w:tblStyle/@w:val"/>
		<xsl:param name="tcBorders" select="w:tcBorders"/>
		<xsl:param name="tblBorders" select="ancestor::w:tbl[1]/w:tblPr/w:tblBorders"/>
		<xsl:param name="contextRow" select="ancestor::w:tr"/>
		<xsl:param name="curStyleName"/>

		<xsl:choose>

			<xsl:when test="$tcBorders/w:insideH and $contextRow/preceding-sibling::w:tr and ($curStyleName = 'firstCol' or $curStyleName = 'band1Vert' or $curStyleName = 'lastCol')">
				<xsl:call-template name="InsertBorderSide">
					<xsl:with-param name="isTableBorder" select="'true'"/>
					<xsl:with-param name="side" select="$tcBorders/w:insideH"/>
					<xsl:with-param name="sideName" select="'top'"/>
					<xsl:with-param name="emulateOpenOfficeTableBorders" select="'true'" />
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$tcBorders/w:top and not($contextRow/preceding-sibling::w:tr) and ($curStyleName = 'firstCol' or $curStyleName = 'band1Vert' or $curStyleName = 'lastCol')">
				<xsl:call-template name="InsertBorderSide">
					<xsl:with-param name="isTableBorder" select="'true'"/>
					<xsl:with-param name="side" select="$tcBorders/w:top"/>
					<xsl:with-param name="sideName" select="'top'"/>
					<xsl:with-param name="emulateOpenOfficeTableBorders" select="'true'" />
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$tcBorders/w:top and not($curStyleName = 'firstCol' or $curStyleName = 'band1Vert' or $curStyleName = 'lastCol')">
				<xsl:call-template name="InsertBorderSide">
					<xsl:with-param name="isTableBorder" select="'true'"/>
					<xsl:with-param name="side" select="$tcBorders/w:top"/>
					<xsl:with-param name="sideName" select="'top'"/>
					<xsl:with-param name="emulateOpenOfficeTableBorders" select="'true'" />
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$tblBorders/w:insideH and $contextRow/preceding-sibling::w:tr">
				<xsl:call-template name="InsertBorderSide">
					<xsl:with-param name="isTableBorder" select="'true'"/>
					<xsl:with-param name="side" select="$tblBorders/w:insideH"/>
					<xsl:with-param name="sideName" select="'top'"/>
					<xsl:with-param name="emulateOpenOfficeTableBorders" select="'true'" />
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$tblBorders/w:top and not($contextRow/preceding-sibling::w:tr)">
				<xsl:call-template name="InsertBorderSide">
					<xsl:with-param name="isTableBorder" select="'true'"/>
					<xsl:with-param name="side" select="$tblBorders/w:top"/>
					<xsl:with-param name="sideName" select="'top'"/>
					<xsl:with-param name="emulateOpenOfficeTableBorders" select="'true'" />
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$cellStyleName != ''">
				<xsl:variable name="curName">
					<xsl:call-template name="getCurName">
						<xsl:with-param name="str" select="$cellStyleName"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:call-template name="InsertTopBbr">
					<xsl:with-param name="styleId" select="$styleId"/>
					<xsl:with-param name="tcBorders" select="key('styles', $styleId)/w:tblStylePr[@w:type = $curName]/w:tcPr/w:tcBorders"/>
					<xsl:with-param name="tblBorders" select="key('styles', $styleId)/w:tblPr/w:tblBorders"/>
					<xsl:with-param name="cellStyleName" select="substring-after($cellStyleName, $curName)"/>
					<xsl:with-param name="curStyleName" select="$curName"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="key('styles', $styleId)/w:tcPr/w:tcBorders or key('styles', $styleId)/w:tblPr/w:tblBorders">
				<xsl:call-template name="InsertTopBbr">
					<xsl:with-param name="styleId" select="key('styles', $styleId)/w:tblPr/w:tblStyle/@w:val"/>
					<xsl:with-param name="tcBorders" select="key('styles', $styleId)/w:tcPr/w:tcBorders"/>
					<xsl:with-param name="tblBorders" select="key('styles', $styleId)/w:tblPr/w:tblBorders"/>
				</xsl:call-template>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="InsertBottomBbr">
		<xsl:param name="cellStyleName"/>
		<xsl:param name="styleId" select="ancestor::w:tbl[1]/w:tblPr/w:tblStyle/@w:val"/>
		<xsl:param name="tcBorders" select="w:tcBorders"/>
		<xsl:param name="tblBorders" select="ancestor::w:tbl[1]/w:tblPr/w:tblBorders"/>
		<xsl:param name="contextRow" select="ancestor::w:tr"/>
		<xsl:param name="curStyleName"/>

		<xsl:choose>
			<xsl:when test="$tcBorders/w:insideH and $contextRow/following-sibling::w:tr and ($curStyleName = 'firstCol' or $curStyleName = 'band1Vert' or $curStyleName = 'lastCol')">
				<xsl:call-template name="InsertBorderSide">
					<xsl:with-param name="isTableBorder" select="'true'"/>
					<xsl:with-param name="side" select="$tcBorders/w:insideH"/>
					<xsl:with-param name="sideName" select="'bottom'"/>
					<xsl:with-param name="emulateOpenOfficeTableBorders" select="'true'" />
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$tcBorders/w:bottom and not($contextRow/following-sibling::w:tr) and ($curStyleName = 'firstCol' or $curStyleName = 'band1Vert' or $curStyleName = 'lastCol')">
				<xsl:call-template name="InsertBorderSide">
					<xsl:with-param name="isTableBorder" select="'true'"/>
					<xsl:with-param name="side" select="$tcBorders/w:bottom"/>
					<xsl:with-param name="sideName" select="'bottom'"/>
					<xsl:with-param name="emulateOpenOfficeTableBorders" select="'true'" />
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$tcBorders/w:bottom and not($curStyleName = 'firstCol' or $curStyleName = 'band1Vert' or $curStyleName = 'lastCol')">
				<xsl:call-template name="InsertBorderSide">
					<xsl:with-param name="isTableBorder" select="'true'"/>
					<xsl:with-param name="side" select="$tcBorders/w:bottom"/>
					<xsl:with-param name="sideName" select="'bottom'"/>
					<xsl:with-param name="emulateOpenOfficeTableBorders" select="'true'" />
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$tblBorders/w:insideH and $contextRow/following-sibling::w:tr">
				<xsl:call-template name="InsertBorderSide">
					<xsl:with-param name="isTableBorder" select="'true'"/>
					<xsl:with-param name="side" select="$tblBorders/w:insideH"/>
					<xsl:with-param name="sideName" select="'bottom'"/>
					<xsl:with-param name="emulateOpenOfficeTableBorders" select="'true'" />
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$tblBorders/w:bottom and not($contextRow/following-sibling::w:tr)">
				<xsl:call-template name="InsertBorderSide">
					<xsl:with-param name="isTableBorder" select="'true'"/>
					<xsl:with-param name="side" select="$tblBorders/w:bottom"/>
					<xsl:with-param name="sideName" select="'bottom'"/>
					<xsl:with-param name="emulateOpenOfficeTableBorders" select="'true'" />
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$cellStyleName != ''">
				<xsl:variable name="curName">
					<xsl:call-template name="getCurName">
						<xsl:with-param name="str" select="$cellStyleName"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:call-template name="InsertBottomBbr">
					<xsl:with-param name="styleId" select="$styleId"/>
					<xsl:with-param name="tcBorders" select="key('styles', $styleId)/w:tblStylePr[@w:type = $curName]/w:tcPr/w:tcBorders"/>
					<xsl:with-param name="tblBorders" select="key('styles', $styleId)/w:tblPr/w:tblBorders"/>
					<xsl:with-param name="cellStyleName" select="substring-after($cellStyleName, $curName)"/>
					<xsl:with-param name="curStyleName" select="$curName"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="key('styles', $styleId)/w:tcPr/w:tcBorders or key('styles', $styleId)/w:tblPr/w:tblBorders">
				<xsl:call-template name="InsertBottomBbr">
					<xsl:with-param name="styleId" select="key('styles', $styleId)/w:tblPr/w:tblStyle/@w:val"/>
					<xsl:with-param name="tcBorders" select="key('styles', $styleId)/w:tcPr/w:tcBorders"/>
					<xsl:with-param name="tblBorders" select="key('styles', $styleId)/w:tblPr/w:tblBorders"/>
				</xsl:call-template>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<!-- w:tcBorders            -->
	<!-- Parent element: tcPr.  -->
	<xsl:template match="w:tcBorders">
		<xsl:apply-templates select="w:top | w:bottom | w:left | w:right" mode="border"/>
	</xsl:template>


	<!-- ПАРАМЕТРЫ BORDER-ов -->
	<!-- TODO: Добавить значения по умолчанию -->

	<xsl:template match="w:right | w:left | w:top | w:bottom | w:start | w:end" mode="border">

		<xsl:variable name="borderPositionNorm">
			<xsl:choose>
				<xsl:when test="local-name() = 'start'">
					<xsl:value-of select="'left'"/>
				</xsl:when>
				<xsl:when test="local-name() = 'end'">
					<xsl:value-of select="'right'"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="local-name()"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="border-x-color">
			<xsl:value-of select="concat(concat('border-',$borderPositionNorm),'-color')"/>
		</xsl:variable>

		<xsl:variable name="border-x-style">
			<xsl:value-of select="concat(concat('border-',$borderPositionNorm),'-style')"/>
		</xsl:variable>

		<xsl:variable name="border-x-width">
			<xsl:value-of select="concat(concat('border-',$borderPositionNorm),'-width')"/>
		</xsl:variable>

		<xsl:if test="@w:color">
			<xsl:attribute name="{$border-x-color}">
				<xsl:call-template name="ConvertHexColor">
					<xsl:with-param name="hexColor" select="@w:color"/>
				</xsl:call-template>
			</xsl:attribute>
		</xsl:if>

		<xsl:if test="@w:val">
			<xsl:attribute name="{$border-x-style}">
				<xsl:call-template name="ConvertBorderStyle">
					<xsl:with-param name="borderStyle" select="@w:val"/>
				</xsl:call-template>
			</xsl:attribute>
		</xsl:if>

		<xsl:if test="@w:sz">
			<xsl:attribute name="{$border-x-width}">
				<xsl:call-template name="ConvertBorderSize">
					<xsl:with-param name="borderSize" select="@w:sz"/>
				</xsl:call-template>
			</xsl:attribute>
		</xsl:if>

	</xsl:template>


	<!-- ======================================== -->
	<!-- Convert tableWidthPropertyType (MS Word) -->
	<!-- to points or percents.                   -->
	<!-- ======================================== -->
	<xsl:template name="ConvertTableWidthPropertyType">
		<xsl:param name="type" select="auto"/>
		<xsl:param name="value" select="0"/>

		<xsl:choose>
			<xsl:when test="$type='nil' or $type='dxa'">
				<xsl:value-of select="$value div 20"/>
				<xsl:text>pt</xsl:text>
			</xsl:when>
			<xsl:when test="$type='pct'">
				<xsl:value-of select="$value div 50"/>
				<xsl:text>%</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>auto</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!-- ========================================================== -->
	<!-- Represents the width that the table should be indented by. -->
	<!-- parent elements: w:tblPr, w:tblPrEx                        -->
	<!-- ========================================================== -->
	<xsl:template match="w:tblInd">
		<xsl:attribute name="start-indent">
			<xsl:call-template name="ConvertTableWidthPropertyType">
				<xsl:with-param name="type" select="@w:type"/>
				<xsl:with-param name="value" select="@w:w"/>
			</xsl:call-template>
		</xsl:attribute>
	</xsl:template>

	<xsl:template match="w:tblW">
		<xsl:attribute name="width">
			<xsl:call-template name="ConvertTableWidthPropertyType">
				<xsl:with-param name="type" select="@w:type"/>
				<xsl:with-param name="value" select="@w:w"/>
			</xsl:call-template>
		</xsl:attribute>
	</xsl:template>

	<!-- ============================================== -->
	<!-- Maps to top, bottom, left, and right paddings. -->
	<!-- parent elements: w:tblCellMar, w:tcMar         -->
	<!-- ============================================== -->
	<xsl:template match="w:right | w:left | w:top | w:bottom | w:start | w:end" mode="paddings">

		<xsl:variable name="borderPositionNorm">
			<xsl:choose>
				<xsl:when test="local-name() = 'start'">
					<xsl:value-of select="'left'"/>
				</xsl:when>
				<xsl:when test="local-name() = 'end'">
					<xsl:value-of select="'right'"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="local-name()"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:attribute name="{concat('padding-',$borderPositionNorm)}">
			<xsl:call-template name="ConvertTableWidthPropertyType">
				<xsl:with-param name="type" select="@w:type"/>
				<xsl:with-param name="value" select="@w:w"/>
			</xsl:call-template>
		</xsl:attribute>
	</xsl:template>

	<!-- =========================================================== -->
	<!-- Represents the cell margin defaults for this table's cells. -->
	<!-- parent elements: w:tblPr                                    -->
	<!-- =========================================================== -->
	<xsl:template match="w:tblCellMar">
		<xsl:apply-templates select="*" mode="paddings"/>
	</xsl:template>

	<!-- ======================================================= -->
	<!-- Represents the number of grid units this cell consumes. -->
	<!-- parent elements: w:tcPr                                 -->
	<!-- ======================================================= -->
	<xsl:template match="w:gridSpan">
		<xsl:attribute name="number-columns-spanned">
			<xsl:value-of select="@w:val"/>
		</xsl:attribute>
	</xsl:template>

	<!-- ================================================= -->
	<!-- Specifies whether this cell is part of            -->
	<!-- (or the beginning of) a vertically merged region. -->
	<!-- parent elements: w:tcPr                           -->
	<!-- ================================================= -->
	<xsl:template match="w:vMerge">
		<xsl:if test="@w:val='restart'">
			<xsl:variable name="column-number" select="count(.|ancestor::w:tc[1]/preceding-sibling::w:tc[not(w:tcPr/w:gridSpan)]) + sum(ancestor::w:tc[1]/preceding-sibling::w:tc/w:tcPr/w:gridSpan/@w:val)"/>
			<xsl:call-template name="CountCellsSpanned">
				<xsl:with-param name="number-cells-spanned" select="1"/>
				<xsl:with-param name="following-cell" select="ancestor::w:tr[1]/following-sibling::w:tr[1]/w:tc[count(.|preceding-sibling::w:tc[not(w:tcPr/w:gridSpan)]) + sum(preceding-sibling::w:tc/w:tcPr/w:gridSpan/@w:val) = $column-number]"/>
				<xsl:with-param name="col-or-row" select="'row'"/>
				<xsl:with-param name="real-column-number" select="$column-number"/>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<!-- =================================================== -->
	<!-- Specifies whether this cell is part of              -->
	<!-- (or the beginning of) a horizontally merged region. -->
	<!-- parent elements: w:tcPr                             -->
	<!-- =================================================== -->
	<xsl:template match="w:hMerge">
		<xsl:if test="@w:val='restart'">
			<xsl:call-template name="CountCellsSpanned">
				<xsl:with-param name="number-cells-spanned" select="1"/>
				<xsl:with-param name="following-cell" select="ancestor::w:tc[1]/following-sibling::w:tc[1]"/>
				<xsl:with-param name="col-or-row" select="'col'"/>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<!-- ================================== -->
	<!-- Count the number of spanned cells. -->
	<!-- ================================== -->
	<xsl:template name="CountCellsSpanned">
		<xsl:param name="number-cells-spanned"/>
		<xsl:param name="following-cell"/>
		<xsl:param name="col-or-row"/>
		<xsl:param name="real-column-number"/>
		<xsl:choose>
			<xsl:when test="$col-or-row='col' and $following-cell/w:tcPr/w:hMerge[not(@w:val='restart')]">
				<xsl:call-template name="CountCellsSpanned">
					<xsl:with-param name="number-cells-spanned" select="$number-cells-spanned + 1"/>
					<xsl:with-param name="following-cell" select="$following-cell/following-sibling::w:tc[1]"/>
					<xsl:with-param name="col-or-row" select="'col'"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$col-or-row='row' and $following-cell/w:tcPr/w:vMerge[not(@w:val='restart')]">
				<xsl:call-template name="CountCellsSpanned">
					<xsl:with-param name="number-cells-spanned" select="$number-cells-spanned + 1"/>
					<xsl:with-param name="following-cell" select="$following-cell/ancestor::w:tr[1]/following-sibling::w:tr[1]/w:tc[count(.|preceding-sibling::w:tc[not(w:tcPr/w:gridSpan)]) + sum(preceding-sibling::w:tc/w:tcPr/w:gridSpan/@w:val) = $real-column-number]"/>
					<xsl:with-param name="col-or-row" select="'row'"/>
					<xsl:with-param name="real-column-number" select="$real-column-number"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:choose>
					<xsl:when test="$col-or-row='col'">
						<xsl:attribute name="number-columns-spanned">
							<xsl:value-of select="$number-cells-spanned"/>
						</xsl:attribute>
					</xsl:when>
					<xsl:otherwise>
						<xsl:attribute name="number-rows-spanned">
							<xsl:value-of select="$number-cells-spanned"/>
						</xsl:attribute>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<!-- ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ДЛЯ BORDER -->

	<xsl:template name="ConvertBorderSize">
		<xsl:param name="borderSize"/>


		<xsl:choose>
			<xsl:when test="contains($borderSize, 'pt')">
				<xsl:value-of select="$borderSize"/>
			</xsl:when>
			<xsl:otherwise>
				<!-- Не может быть меньше 2 или больше 96 -->
				<xsl:variable name="borderSizeNorm">
					<xsl:choose>
						<xsl:when test="$borderSize &lt; 2">
							<xsl:value-of select="2"/>
						</xsl:when>
						<xsl:when test="$borderSize &gt; 96">
							<xsl:value-of select="96"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="$borderSize"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>

				<!-- w:sz это 1/8 пункта -->
				<xsl:value-of select="$borderSizeNorm * 0.125"/>
				<xsl:text>pt</xsl:text>
			</xsl:otherwise>
		</xsl:choose>

	</xsl:template>

	<xsl:template name="ConvertBorderStyle">
		<xsl:param name="borderStyle"/>

		<xsl:choose>
			<xsl:when test="($borderStyle = 'single') or ($borderStyle = 'thick')">
				<xsl:text>solid</xsl:text>
			</xsl:when>
			<xsl:when test="($borderStyle = 'nil') or ($borderStyle = 'none')">
				<xsl:text>none</xsl:text>
			</xsl:when>

			<xsl:when test="($borderStyle = 'double')">
				<xsl:text>double</xsl:text>
			</xsl:when>

			<xsl:when test="($borderStyle = 'dotted')">
				<xsl:text>dotted</xsl:text>
			</xsl:when>

			<xsl:when test="($borderStyle = 'dashed')">
				<xsl:text>dashed</xsl:text>
			</xsl:when>

			<xsl:otherwise>
				<xsl:text>solid</xsl:text>
			</xsl:otherwise>

		</xsl:choose>

	</xsl:template>


	<!-- ********************************************** -->
	<!-- ********************************************** -->
	<!--             ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ            -->
	<!-- ********************************************** -->
	<!-- ********************************************** -->

	<!--
  Удалить указанные символы из строки
  
  @param  inputString   входная строка
  @param  simbolToRemove  символ, который требуется удалить   
 
  -->
	<xsl:template name="RemoveSimbols">
		<xsl:param name="inputString"/>
		<xsl:param name="simbolToRemove"/>
		<xsl:choose>
			<xsl:when test="contains($inputString, $simbolToRemove)">
				<xsl:value-of select="substring-before($inputString, $simbolToRemove)"/>
				<xsl:call-template name="RemoveSimbols">
					<xsl:with-param name="inputString" select="substring-after($inputString, $simbolToRemove)"/>
					<xsl:with-param name="simbolToRemove" select="$simbolToRemove"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$inputString"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!--
  Привести параметр language к формату XSL-FO
  
  @param  строка language
  -->
	<xsl:template name="NormalizeLanguage">
		<xsl:param name="inputString"/>
		<xsl:variable name="tmpString">
			<xsl:choose>
				<xsl:when test="contains($inputString, '-')">
					<xsl:value-of select="substring-before($inputString, '-')"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$inputString"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:call-template name="UpperCaseLatin">
			<xsl:with-param name="inputString" select="$tmpString"/>
		</xsl:call-template>
	</xsl:template>

	<!--
    Удалить пробельные символы из имени шрифта
    
    @param  font-family имя шрифта
  -->
	<xsl:template name="NormalizeFontFamilyName">
		<xsl:param name="font-family" />
		<xsl:call-template name="RemoveSimbols">
			<xsl:with-param name="inputString" select="$font-family"/>
			<xsl:with-param name="simbolToRemove" select="' '"/>
		</xsl:call-template>
	</xsl:template>

	<xsl:template name="LowerCaseLatin">
		<xsl:param name="inputString"/>
		<xsl:value-of select="translate($inputString, 'ABCDEFGHIJKLMNOPQRSTUVWXYZ', 'abcdefghijklmnopqrstuvwxyz')"/>
		<!-- LowerCaseLatin -->
	</xsl:template>

	<xsl:template name="UpperCaseLatin">
		<xsl:param name="inputString"/>
		<xsl:value-of select="translate($inputString, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ')"/>
		<!-- LowerCaseLatin -->
	</xsl:template>

	<xsl:template name="NormalizeColorName">
		<xsl:param name = "colorName"/>
		<xsl:choose>
			<xsl:when test="$colorName='black'">
				<xsl:text>#000000</xsl:text>
				<!--<xsl:text>black</xsl:text>-->
			</xsl:when>
			<xsl:when test="$colorName='blue'">
				<xsl:text>#0000FF</xsl:text>
				<!--<xsl:text>blue</xsl:text>-->
			</xsl:when>
			<xsl:when test="$colorName='cyan'">
				<xsl:text>#00FFFF</xsl:text>
				<!--<xsl:text>aqua</xsl:text>-->
			</xsl:when>
			<xsl:when test="$colorName='green'">
				<xsl:text>#00FF00</xsl:text>
				<!--<xsl:text>lime</xsl:text>-->
			</xsl:when>
			<xsl:when test="$colorName='magenta'">
				<xsl:text>#FF00FF</xsl:text>
				<!--<xsl:text>fuchsia</xsl:text>-->
			</xsl:when>
			<xsl:when test="$colorName='red'">
				<xsl:text>#FF0000</xsl:text>
				<!--<xsl:text>red</xsl:text>-->
			</xsl:when>
			<xsl:when test="$colorName='yellow'">
				<xsl:text>#FFFF00</xsl:text>
				<!--<xsl:text>yellow</xsl:text>-->
			</xsl:when>
			<xsl:when test="$colorName='white'">
				<xsl:text>#FFFFFF</xsl:text>
				<!--<xsl:text>white</xsl:text>-->
			</xsl:when>
			<xsl:when test="$colorName='darkblue'">
				<xsl:text>#00008B</xsl:text>
				<!--<xsl:text>navy</xsl:text>-->
			</xsl:when>
			<xsl:when test="$colorName='darkcyan'">
				<xsl:text>#008B8B</xsl:text>
				<!--<xsl:text>teal</xsl:text>-->
			</xsl:when>
			<xsl:when test="$colorName='darkgreen'">
				<xsl:text>#008B00</xsl:text>
				<!--<xsl:text>green</xsl:text>-->
			</xsl:when>
			<xsl:when test="$colorName='darkmagenta'">
				<xsl:text>#8B008B</xsl:text>
				<!--<xsl:text>purple</xsl:text>-->
			</xsl:when>
			<xsl:when test="$colorName='darkred'">
				<xsl:text>#8B0000</xsl:text>
				<!--<xsl:text>maroon</xsl:text>-->
			</xsl:when>
			<xsl:when test="$colorName='darkyellow'">
				<xsl:text>#8B8B00</xsl:text>
				<!--<xsl:text>olive</xsl:text>-->
			</xsl:when>
			<xsl:when test="$colorName='darkgray'">
				<xsl:text>#808080</xsl:text>
				<!--<xsl:text>gray</xsl:text>-->
			</xsl:when>
			<xsl:when test="$colorName='lightgray'">
				<xsl:text>#C0C0C0</xsl:text>
				<!--<xsl:text>silver</xsl:text>-->
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>#FFFFFF</xsl:text>
				<!--<xsl:value-of select="$colorName"/>-->
			</xsl:otherwise>
		</xsl:choose>

		<!-- NormalizeColorName -->
	</xsl:template>

	<!--
  Перевести код цвета, записанный в стиле OOX в стиль FO
  
  @param  hexColor  код цвета
  @param  style     стиль 
  -->
	<xsl:template name="ConvertHexColor">
		<xsl:param name="hexColor" select="'auto'"/>
		<xsl:param name="style" select="'none'"/>
		<xsl:choose>
			<xsl:when test="$hexColor='auto'">
				<xsl:choose>
					<xsl:when test="$style='three-d-emboss' or $style='ridge'">
						<xsl:text>#B0B0B0</xsl:text>
					</xsl:when>
					<xsl:otherwise>
						<!-- MS Word treats 'auto' as black, not as inherited. -->
						<!--<xsl:text>black</xsl:text>-->
						<xsl:text>#000000</xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:when test="contains($hexColor, '#') and string-length($hexColor) = 7">
				<xsl:value-of select="$hexColor"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:choose>
					<xsl:when test="string-length($hexColor) = 6">
						<xsl:value-of select="concat('#', $hexColor)"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:variable name="normalHexColor">
							<xsl:call-template name="NormalizeColorName">
								<xsl:with-param name="colorName" select="$hexColor"/>
							</xsl:call-template>
						</xsl:variable>
						<xsl:choose>
							<xsl:when test="contains($normalHexColor, '#') and string-length($normalHexColor) = 7">
								<xsl:call-template name="ConvertHexColor">
									<xsl:with-param name="hexColor" select="$normalHexColor"/>
								</xsl:call-template>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="$normalHexColor"/>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:otherwise>
		</xsl:choose>

		<!-- ConvertHexColor -->
	</xsl:template>


	<xsl:template name="Convert-pt2mm">
		<xsl:param name="inputValue"/>
		<xsl:value-of select="$inputValue * 0.3528"/>
	</xsl:template>

	<xsl:template name="Convert-in2pt">
		<xsl:param name="inputValue"/>
		<xsl:value-of select="$inputValue * 72"/>
	</xsl:template>

	<xsl:template name="Convert-mm2pt">
		<xsl:param name="inputValue"/>
		<xsl:value-of select="$inputValue div 0.3528"/>
	</xsl:template>

	<xsl:template name="Convert-cm2pt">
		<xsl:param name="inputValue"/>
		<xsl:value-of select="$inputValue div 0.03528"/>
	</xsl:template>

	<xsl:template name="Convert-in2cm">
		<xsl:param name="inputValue"/>
		<xsl:value-of select="$inputValue * 2.54"/>
	</xsl:template>

	<xsl:template name="Convert-em2pt">
		<xsl:param name="inputValue"/>
		<xsl:value-of select="$inputValue * 72.0 div  (25.4 * 36000)"/>
		<xsl:text>pt</xsl:text>
	</xsl:template>


	<!--
  Check for existing unsupported elements.
  -->
	<xsl:template name="CheckNotSupportedElements">
		<xsl:param name="elements"/>
		<xsl:for-each select="$elements">
			<xsl:if test="contains($not-supported-inlines, .)">
				<xsl:value-of select="'presents'"/>
			</xsl:if>
		</xsl:for-each>
	</xsl:template>

	<!-- ======================= -->
	<!-- Custom recording tables -->
	<!-- ======================= -->
	<!-- =========== -->
	<!-- Symbol font -->
	<!-- =========== -->
	<my:recoding-table font="Symbol">
		<my:char code="&#xB7;" value="B7" altvalue="F0B7" entity="&#x2022;"/>
	</my:recoding-table>
	<!-- ============== -->
	<!-- Wingdings font -->
	<!-- ============== -->
	<my:recoding-table font="Wingdings">
		<my:char code="&#x9F;" value="9F" altvalue="F09F" entity="&#x2022;"/>
		<my:char code="&#xFC;" value="FC" altvalue="F0FC" entity="&#x2713;"/>
		<my:char code="&#x6C;" value="6C" altvalue="F06C" entity="&#x25CF;"/>
	</my:recoding-table>


	<!-- IMAGES -->

	<!-- ================================= -->
	<!-- The template parses an CSS style. -->
	<!-- ================================= -->
	<xsl:template name="ParsingCSSStyle">
		<xsl:param name="style"/>
		<xsl:param name="object"/>

		<xsl:if test="contains($style, 'width:')">
			<xsl:variable name="width-with-trailing" select="substring-after($style, 'width:')"/>
			<xsl:variable name="width">
				<xsl:choose>
					<xsl:when test="contains($width-with-trailing, ';')">
						<xsl:value-of select="substring-before($width-with-trailing, ';')"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="$width-with-trailing"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<xsl:variable name="attribute-name">
				<xsl:choose>
					<xsl:when test="$object='image'">
						<xsl:text>content-width</xsl:text>
					</xsl:when>
					<xsl:otherwise>
						<xsl:text>width</xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>

			<xsl:attribute name="{$attribute-name}">
				<xsl:value-of select="$width"/>
			</xsl:attribute>
		</xsl:if>

		<xsl:if test="contains($style, 'height:')">
			<xsl:variable name="height-with-trailing" select="substring-after($style, 'height:')"/>
			<xsl:variable name="height">
				<xsl:choose>
					<xsl:when test="contains($height-with-trailing, ';')">
						<xsl:value-of select="substring-before($height-with-trailing, ';')"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="$height-with-trailing"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<xsl:variable name="attribute-name">
				<xsl:choose>
					<xsl:when test="$object='image'">
						<xsl:text>content-height</xsl:text>
					</xsl:when>
					<xsl:otherwise>
						<xsl:text>height</xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>

			<xsl:attribute name="{$attribute-name}">
				<xsl:value-of select="$height"/>
			</xsl:attribute>
		</xsl:if>

		<xsl:attribute name="scaling">
			<xsl:text>non-uniform</xsl:text>
		</xsl:attribute>
		<!-- <xsl:template name="ParsingCSSStyle"> -->
	</xsl:template>

	<!-- ================================= -->
	<!-- The template return media subtype -->
	<!-- from the name of the binary data. -->
	<!-- ================================= -->
	<xsl:template name="RetrieveMediaSubtype">
		<xsl:param name="name"/>
		<xsl:choose>
			<xsl:when test="contains($name, '.jpg') or contains($name, '.jpeg') or
                      contains($name, '.jpe') or contains($name, '.jfif')">
				<xsl:text>jpeg</xsl:text>
			</xsl:when>
			<xsl:when test="contains($name, '.gif')">
				<xsl:text>gif</xsl:text>
			</xsl:when>
			<xsl:when test="contains($name, '.tif') or contains($name, '.tiff')">
				<xsl:text>tiff</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>auto</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
		<!-- <xsl:template name="RetrieveMediaSubtype"> -->
	</xsl:template>


	<xsl:template match="w:pict" priority="-1">
		<xsl:variable name="binary-data" select="w:binData | w:movie | w:background | w:applet
                                                 | w:scriptAnchor | w:ocx | w:msAccessHTML"/>
		<xsl:apply-templates select="v:shape">
			<xsl:with-param name="binary-data" select="$binary-data"/>
		</xsl:apply-templates>
	</xsl:template>

	<!-- ======================================= -->
	<!-- Containes the reference to this object. -->
	<!-- ======================================= -->
	<xsl:template match="v:shape" priority="-1">
		<xsl:param name="binary-data"/>

		<xsl:if test="v:imagedata">
			<fo:external-graphic>
				<!-- content-width, content-height -->
				<xsl:call-template name="ParsingCSSStyle">
					<xsl:with-param name="style" select="@style"/>
					<xsl:with-param name="object" select="'image'"/>
				</xsl:call-template>

				<!-- scaling and etc. -->
				<!--<xsl:apply-templates select="o:lock"/>
        -->
				<!-- border styles and border widths -->
				<!--
        <xsl:apply-templates select="w10:bordertop|w10:borderbottom|w10:borderleft|w10:borderright"/>
        -->
				<!-- border colors -->
				<!--
        <xsl:apply-templates select="@o:bordertopcolor|@o:borderleftcolor|@o:borderbottomcolor|@o:borderrightcolor"/>-->

				<xsl:attribute name="src">

					<!-- <xsl:choose>
            <xsl:when test="$binary-data">
       
              <xsl:apply-templates select="$binary-data" />
            </xsl:when>
            <xsl:otherwise>

              <xsl:value-of select="concat('url(&quot;', v:imagedata/@src, '&quot;)')"/>
            </xsl:otherwise>
          </xsl:choose>
        -->

					<xsl:variable name="imagePath">
						<xsl:variable name="xml-name">
							<xsl:call-template name="ExtractFileName">
								<xsl:with-param name="fullName" select="ancestor::oox:part/@oox:name"/>
							</xsl:call-template>
						</xsl:variable>
						<xsl:variable name="xml-rels-name">
							<xsl:value-of select="concat('word/_rels/',$xml-name,'.rels')"/>
						</xsl:variable>
						<xsl:value-of select="/oox:package/oox:part[@oox:name=$xml-rels-name]/rels:Relationships/rels:Relationship[(@Type='http://schemas.openxmlformats.org/officeDocument/2006/relationships/image') and (@Id=current()/v:imagedata/@r:id)]/@Target"/>
					</xsl:variable>


					<xsl:variable name="imagePathFileName">
						<xsl:call-template name="ExtractFileName">
							<xsl:with-param name="fullName" select="$imagePath"/>
						</xsl:call-template>
					</xsl:variable>

					<!--          <xsl:value-of select="concat('avsooximagesource:',$imagePath)"/> -->
					<xsl:value-of select="concat('./', $avs-properties-fofilename, '.files/', $imagePathFileName)"/>


					<!--<xsl:message><xsl:text>Image:</xsl:text><xsl:value-of select="$imagePath"/></xsl:message>-->

				</xsl:attribute>

			</fo:external-graphic>
		</xsl:if>
	</xsl:template>



	<!--
  Получить значение свойства, прописанного в атрибуте @style
  Например, для узла 
  <v:shape style="position:absolute;margin-left:0;margin-top:14.15pt;width:199.9pt;height:199.9pt;z-index:251653120;mso-wrap-distance-left:0;mso-wrap-distance-right:0;mso-position-horizontal:center">
  
  вызов шаблона 
  <xsl:call-template name="GetShapeStyleProperty">
    <xsl:with-param name="propertyName" select="'margin-top'"/>
    <xsl:with-param name="shape" select="v:shape"/>
  </xsl:call-template>
  
  вернет значение
  '14.15pt'  
  
  -->
	<xsl:template name="GetShapeStyleProperty">
		<xsl:param name="shape"/>
		<xsl:param name="propertyName"/>
		<xsl:variable name="propertyValue">
			<xsl:choose>
				<!-- свойство записано не на последнем месте, за ним следует символ ';' -->
				<xsl:when test="contains(substring-after($shape/@style,concat($propertyName,':')),';')">
					<xsl:value-of select="substring-before(substring-after($shape/@style,concat($propertyName,':')),';')"/>
				</xsl:when>
				<!-- свойство записано на последнем месте -->
				<xsl:otherwise>
					<xsl:value-of select="substring-after($shape/@style,concat($propertyName,':'))"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:value-of select="$propertyValue"/>
	</xsl:template>


	<xsl:template name="ExtractFileName">
		<xsl:param name="fullName"/>

		<xsl:choose>
			<xsl:when test="contains($fullName,'/')">
				<xsl:call-template name="ExtractFileName">
					<xsl:with-param name="fullName" select="substring-after($fullName,'/')"/>
				</xsl:call-template>
			</xsl:when>

			<xsl:when test="contains($fullName,'\')">
				<xsl:call-template name="ExtractFileName">
					<xsl:with-param name="fullName" select="substring-after($fullName,'\')"/>
				</xsl:call-template>
			</xsl:when>

			<xsl:otherwise>
				<xsl:value-of select="$fullName"/>
			</xsl:otherwise>

		</xsl:choose>

	</xsl:template>


	<!-- ======= -->
	<!-- Дефис -->
	<!-- ======= -->
	<xsl:template match="w:softHyphen">
		<xsl:text>&#x00AD;</xsl:text>
	</xsl:template>

	<!-- без разрыва -->
	<xsl:template match="w:noBreakHyphen">
		<xsl:text>&#2011;</xsl:text>
	</xsl:template>

	<!-- template w:hyperlink -->
	<xsl:template match="w:hyperlink">
		<xsl:element name="fo:basic-link">

			<xsl:choose>
				<xsl:when test="@r:id">

					<xsl:variable name="relationship-hyperlink-targetmode">
						<xsl:call-template name="SolveRelation">
							<xsl:with-param name="id" select="@r:id"/>
							<xsl:with-param name="arg" select="'TargetMode'"/>
						</xsl:call-template>
					</xsl:variable>

					<xsl:attribute name="external-destination">
						<xsl:call-template name="SolveRelation">
							<xsl:with-param name="id" select="@r:id"/>
							<xsl:with-param name="arg" select="'Target'"/>
						</xsl:call-template>
					</xsl:attribute>

				</xsl:when>
				<xsl:when test="@w:anchor">

					<xsl:attribute name="internal-destination">
						<xsl:value-of select="@w:anchor"/>
					</xsl:attribute>

				</xsl:when>
			</xsl:choose>

			<xsl:apply-templates select="*"/>

		</xsl:element>
	</xsl:template>

	<xsl:template match="w:br">
		<!-- defines a page, column, or text-wrapping break -->
		<xsl:choose>
			<xsl:when test="@w:type = 'page'">
				<xsl:element name="fo:break">
					<xsl:attribute name="break-before">
						<xsl:value-of select="'page'"/>
					</xsl:attribute>
				</xsl:element>
			</xsl:when>
			<xsl:when test="@w:type = 'column'">
				<xsl:element name="fo:break">
					<xsl:attribute name="break-before">
						<xsl:value-of select="'column'"/>
					</xsl:attribute>
				</xsl:element>
			</xsl:when>
			<!-- if @w:type="text-wrapping" or omitted - it's a new-line break -->
			<xsl:otherwise>
				<fo:break/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!-- 
  <xsl:template match="w:footnoteReference">
    <fo:footnote>

      <fo:inline>
        <xsl:call-template name="ApplyRunProperties"/>

        <xsl:number level="any" count="/oox:package/oox:part/w:document//w:footnoteReference" format="1"/>

      </fo:inline>
      <fo:footnote-body>
      </fo:footnote-body>
    </fo:footnote>

  </xsl:template>
  -->

	<xsl:template match="*" priority="-1">
		<xsl:if test="$verbose='yes'">
			<xsl:message>
				<xsl:text>Warning! Unsupported element: </xsl:text>
				<xsl:value-of select="name()"/>
				<xsl:text>. Element's contents will be lost.</xsl:text>
			</xsl:message>
		</xsl:if>
	</xsl:template>

	<xsl:template match="@*" priority="-1">
		<xsl:if test="$verbose='yes'">
			<xsl:message>
				<xsl:text>Warning! Unsupported attribute: </xsl:text>
				<xsl:value-of select="name()"/>
				<xsl:text>. Element's contents will be lost.</xsl:text>
			</xsl:message>
		</xsl:if>
	</xsl:template>
	<xsl:variable name="shapesIdTable">
		<xsl:text>v:rect:1;</xsl:text>
		<xsl:text>v:roundrect:2;</xsl:text>
		<xsl:text>v:oval:3;</xsl:text>
		<xsl:text>v:line:20;</xsl:text>
	</xsl:variable>


	<!--<xsl:template match="w:pict | w:object">
    <xsl:apply-templates select=".//*[name()='v:rect' or 
                                      name()='v:oval' or                          
                                      name()='v:shape' or 
                                      name()='v:roundrect' or 
                                      name()='v:line']" mode="inPict"/>
  </xsl:template>-->

	<xsl:template match="v:oval | v:roundrect | v:rect | v:shape |v:line">
		<xsl:param name="styleGroupParam"/>
		<xsl:param name="styleInGroupParam"/>
		<xsl:call-template name="setShape"/>
	</xsl:template>

	<xsl:template name="shapeId">
		<xsl:param name="nameShape"/>
		<xsl:call-template name="getAttribute">
			<xsl:with-param name="str" select="$shapesIdTable"/>
			<xsl:with-param name="attrName" select="$nameShape"/>
		</xsl:call-template>
	</xsl:template>

	<xsl:template name="setShape">
		<xsl:variable name="id" select="substring-after(@type, '#')"/>
		<shape>
			<xsl:if test="@filled">
				<xsl:attribute name="filled">
					<xsl:value-of select="@filled"/>
				</xsl:attribute>
			</xsl:if>
			<xsl:if test="name() != 'v:shape'">
				<xsl:attribute name="type">
					<xsl:call-template name="shapeId">
						<xsl:with-param name="nameShape" select="name()"/>
					</xsl:call-template>
				</xsl:attribute>
			</xsl:if>
			<xsl:choose>
				<xsl:when test="$id != ''">
					<xsl:variable name="shapeType" select="preceding::v:shapetype[@id = $id][position() = 1]"/>
					<template>
						<xsl:if test="$shapeType">
							<xsl:if test="$shapeType/@coordsize">
								<xsl:attribute name="coordsize">
									<xsl:value-of select="$shapeType/@coordsize"/>
								</xsl:attribute>
							</xsl:if>
							<xsl:if test="$shapeType/@path">
								<xsl:attribute name="path">
									<xsl:value-of select="$shapeType/@path"/>
								</xsl:attribute>
							</xsl:if>
							<xsl:if test="$shapeType/@adj">
								<xsl:attribute name="adj">
									<xsl:value-of select="$shapeType/@adj"/>
								</xsl:attribute>
							</xsl:if>
							<xsl:if test="$shapeType/@o:spt">
								<xsl:attribute name="spt">
									<xsl:value-of select="$shapeType/@o:spt"/>
								</xsl:attribute>
							</xsl:if>
							<xsl:if test="$shapeType/@o:oned">
								<xsl:attribute name="oned">
									<xsl:value-of select="$shapeType/@o:oned"/>
								</xsl:attribute>
							</xsl:if>
							<xsl:if test="$shapeType/@filled">
								<xsl:attribute name="filled">
									<xsl:value-of select="$shapeType/@filled"/>
								</xsl:attribute>
							</xsl:if>
							<xsl:if test="$shapeType/@stroked or $shapeType/@strokecolor or $shapeType/@strokeweight">
								<xsl:element name="stroke">
									<xsl:if test="$shapeType/@stroked">
										<xsl:attribute name="stroked">
											<xsl:value-of select="$shapeType/@stroked"/>
										</xsl:attribute>
									</xsl:if>
									<xsl:if test="$shapeType/@strokecolor">
										<xsl:attribute name="strokecolor">
											<xsl:value-of select="$shapeType/@strokecolor"/>
										</xsl:attribute>
									</xsl:if>
									<xsl:if test="$shapeType/@strokeweight">
										<xsl:variable name="weight">
											<xsl:call-template name="convertToMM">
												<xsl:with-param name="value" select="$shapeType/@strokeweight"/>
											</xsl:call-template>
										</xsl:variable>
										<xsl:if test="$weight != 'NaN'">
											<xsl:attribute name="strokeweight">
												<xsl:value-of select="$weight"/>
											</xsl:attribute>
										</xsl:if>
									</xsl:if>
								</xsl:element>
							</xsl:if>
							<xsl:copy-of select="$shapeType/v:stroke"/>
							<xsl:copy-of select="$shapeType/v:formulas"/>
							<xsl:copy-of select="$shapeType/v:path"/>
							<xsl:copy-of select="$shapeType/v:handles"/>
							<xsl:copy-of select="$shapeType/o:complex "/>
						</xsl:if>
					</template>
				</xsl:when>
			</xsl:choose>
			<xsl:if test="@path">
				<xsl:element name="path">
					<xsl:attribute name="val">
						<xsl:value-of select="@path"/>
					</xsl:attribute>
				</xsl:element>
			</xsl:if>
			<xsl:if test="@coordsize">
				<xsl:element name="coordsize">
					<xsl:attribute name="val">
						<xsl:value-of select="@coordsize"/>
					</xsl:attribute>
				</xsl:element>
			</xsl:if>
			<xsl:copy-of select="v:path"/>
			<xsl:if test="@style or (@from and @to)">
				<xsl:call-template name="insertBounds">
					<xsl:with-param name="style" select="@style"/>
				</xsl:call-template>
			</xsl:if>
			<xsl:if test="@fillcolor">
				<xsl:element name="fillcolor">
					<xsl:attribute name="val">
						<xsl:value-of select="@fillcolor"/>
					</xsl:attribute>
				</xsl:element>
			</xsl:if>
			<xsl:if test="@arcsize">
				<xsl:element name="arcsize">
					<xsl:attribute name="val">
						<xsl:value-of select="@arcsize"/>
					</xsl:attribute>
				</xsl:element>
			</xsl:if>
			<xsl:if test="@alt">
				<xsl:element name="alt">
					<xsl:attribute name="val">
						<xsl:value-of select="@alt"/>
					</xsl:attribute>
				</xsl:element>
			</xsl:if>
			<xsl:if test="@o:connectortype">
				<xsl:element name="connectortype">
					<xsl:attribute name="val">
						<xsl:value-of select="@o:connectortype"/>
					</xsl:attribute>
				</xsl:element>
			</xsl:if>
			<xsl:if test="@stroked or @strokecolor or @strokeweight">
				<xsl:element name="stroke">
					<xsl:if test="@stroked">
						<xsl:attribute name="stroked">
							<xsl:value-of select="@stroked"/>
						</xsl:attribute>
					</xsl:if>
					<xsl:if test="@strokecolor">
						<xsl:attribute name="strokecolor">
							<xsl:value-of select="@strokecolor"/>
						</xsl:attribute>
					</xsl:if>
					<xsl:if test="@strokeweight">
						<xsl:variable name="weight">
							<xsl:call-template name="convertToMM">
								<xsl:with-param name="value" select="@strokeweight"/>
							</xsl:call-template>
						</xsl:variable>
						<xsl:if test="$weight != 'NaN'">
							<xsl:attribute name="strokeweight">
								<xsl:value-of select="$weight"/>
							</xsl:attribute>
						</xsl:if>
					</xsl:if>
				</xsl:element>
			</xsl:if>
			<xsl:call-template name="setFill"/>
			<xsl:call-template name="setImageData"/>
			<xsl:copy-of select="./v:stroke"/>
			<xsl:copy-of select="./o:lock"/>
			<xsl:copy-of select="./w10:wrap"/>
			<xsl:copy-of select="./w10:anchorlock "/>
			<xsl:copy-of select="./v:shadow"/>
			<xsl:copy-of select="./v:textbox"/>
			<xsl:copy-of select="./o:extrusion"/>
			<xsl:if test="v:textbox">
				<xsl:call-template name="insertTextBox"/>
			</xsl:if>
		</shape>
	</xsl:template>

	<xsl:template name="setImageData">
		<xsl:if test="./v:imagedata">
			<xsl:element name="imageData">
				<xsl:call-template name="insertFileName">
					<xsl:with-param name="id" select="./v:imagedata/@r:id"/>
				</xsl:call-template>
			</xsl:element>
		</xsl:if>
	</xsl:template>
	<xsl:template name="setFill">
		<xsl:if test="./v:fill">
			<xsl:element name="fill">
				<xsl:for-each select="./v:fill/@*">
					<xsl:if test="name() != 'r:id'">
						<xsl:copy-of select="."/>
					</xsl:if>
				</xsl:for-each>
				<xsl:if test="./v:fill/@r:id">
					<xsl:call-template name="insertFileName">
						<xsl:with-param name="id" select="./v:fill/@r:id"/>
					</xsl:call-template>
				</xsl:if>
			</xsl:element>
		</xsl:if>
	</xsl:template>

	<xsl:template name="insertFileName">
		<xsl:param name="id"/>

		<xsl:element name="source">
			<xsl:attribute name="dir">
				<xsl:value-of select="$avs-properties-temp-directory"/>
			</xsl:attribute>
			<xsl:call-template name="SolveRelation">
				<xsl:with-param name="id" select="$id"/>
				<xsl:with-param name="arg" select="'Target'"/>
			</xsl:call-template>
		</xsl:element>

		<!--<xsl:value-of select="$id"/>-->
	</xsl:template>

	<xsl:template name="convertLineCoord">
		<xsl:param name="begX"/>
		<xsl:param name="begY"/>
		<xsl:param name="endX"/>
		<xsl:param name="endY"/>

		<xsl:choose>
			<xsl:when test="$endX &gt;= $begX and $endY &gt;= $begY">
				<xsl:call-template name="setBounds">
					<xsl:with-param name="MarginLeft" select="$begX"/>
					<xsl:with-param name="MarginTop" select="$begY"/>
					<xsl:with-param name="Width" select="$endX - $begX"/>
					<xsl:with-param name="Height" select="$endY - $begY"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$endX &gt;= $begX and $endY &lt; $begY">
				<xsl:call-template name="setBounds">
					<xsl:with-param name="MarginLeft" select="$begX"/>
					<xsl:with-param name="MarginTop" select="$endY"/>
					<xsl:with-param name="Width" select="$endX - $begX"/>
					<xsl:with-param name="Height" select="$begY - $endY"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$endX &lt; $begX and $endY &gt;= $begY">
				<xsl:call-template name="setBounds">
					<xsl:with-param name="MarginLeft" select="$endX"/>
					<xsl:with-param name="MarginTop" select="$begY"/>
					<xsl:with-param name="Width" select="$begX - $endX"/>
					<xsl:with-param name="Height" select="$endY - $begY"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$endX &lt; $begX and $endY &lt; $begY">
				<xsl:call-template name="setBounds">
					<xsl:with-param name="MarginLeft" select="$endX"/>
					<xsl:with-param name="MarginTop" select="$endY"/>
					<xsl:with-param name="Width" select="$begX - $endX"/>
					<xsl:with-param name="Height" select="$begY - $endY"/>
				</xsl:call-template>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="insertBounds">
		<xsl:choose>
			<xsl:when test="@from and @to">
				<xsl:variable name="from" select="@from"/>
				<xsl:variable name="to" select="@to"/>

				<xsl:call-template name="convertLineCoord">
					<xsl:with-param name="begX" select="substring-before($from, ',')"/>
					<xsl:with-param name="begY" select="substring-after($from, ',')"/>
					<xsl:with-param name="endX" select="substring-before($to, ',')"/>
					<xsl:with-param name="endY" select="substring-after($to, ',')"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:variable name="style" select="@style"/>
				<xsl:variable name="MarginLeft">
					<xsl:call-template name="convertToDx">
						<xsl:with-param name="value">
							<xsl:call-template name="getAttribute">
								<xsl:with-param name="str" select="$style"/>
								<xsl:with-param name="attrName" select="'margin-left'"/>
							</xsl:call-template>
						</xsl:with-param>
					</xsl:call-template>
				</xsl:variable>
				<xsl:variable name="MarginTop">
					<xsl:call-template name="convertToDx">
						<xsl:with-param name="value">
							<xsl:call-template name="getAttribute">
								<xsl:with-param name="str" select="$style"/>
								<xsl:with-param name="attrName" select="'margin-top'"/>
							</xsl:call-template>
						</xsl:with-param>
					</xsl:call-template>
				</xsl:variable>
				<xsl:variable name="Width">
					<xsl:call-template name="convertToDx">
						<xsl:with-param name="value">
							<xsl:call-template name="getAttribute">
								<xsl:with-param name="str" select="$style"/>
								<xsl:with-param name="attrName" select="'width'"/>
							</xsl:call-template>
						</xsl:with-param>
					</xsl:call-template>
				</xsl:variable>
				<xsl:variable name="Height">
					<xsl:call-template name="convertToDx">
						<xsl:with-param name="value">
							<xsl:call-template name="getAttribute">
								<xsl:with-param name="str" select="$style"/>
								<xsl:with-param name="attrName" select="'height'"/>
							</xsl:call-template>
						</xsl:with-param>
					</xsl:call-template>
				</xsl:variable>
				<xsl:variable name="ZIndex">
					<xsl:call-template name="getAttribute">
						<xsl:with-param name="str" select="$style"/>
						<xsl:with-param name="attrName" select="'z-index'"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:variable name="Flip">
					<xsl:call-template name="getAttribute">
						<xsl:with-param name="str" select="$style"/>
						<xsl:with-param name="attrName" select="'flip'"/>
						<xsl:with-param name="withTrim" select="false"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:variable name="Rotate">
					<xsl:variable name="tmpRotate">
						<xsl:call-template name="getAttribute">
							<xsl:with-param name="str" select="$style"/>
							<xsl:with-param name="attrName" select="'rotation'"/>
						</xsl:call-template>
					</xsl:variable>
					<xsl:choose>
						<xsl:when test="contains($tmpRotate, 'fd')">
							<xsl:value-of select="substring-before($tmpRotate, 'fd') div 60000"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="$tmpRotate"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>
				<xsl:variable name="PosHorRel">
					<xsl:call-template name="getAttribute">
						<xsl:with-param name="str" select="$style"/>
						<xsl:with-param name="attrName" select="'mso-position-horizontal-relative'"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:variable name="PosHor">
					<xsl:call-template name="getAttribute">
						<xsl:with-param name="str" select="$style"/>
						<xsl:with-param name="attrName" select="'mso-position-horizontal'"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:variable name="PosVertRel">
					<xsl:call-template name="getAttribute">
						<xsl:with-param name="str" select="$style"/>
						<xsl:with-param name="attrName" select="'mso-position-vertical-relative'"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:variable name="PosVert">
					<xsl:call-template name="getAttribute">
						<xsl:with-param name="str" select="$style"/>
						<xsl:with-param name="attrName" select="'mso-position-vertical'"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:variable name="Pos">
					<xsl:call-template name="getAttribute">
						<xsl:with-param name="str" select="$style"/>
						<xsl:with-param name="attrName" select="'position'"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:variable name="Left">
					<xsl:call-template name="getAttribute">
						<xsl:with-param name="str" select="$style"/>
						<xsl:with-param name="attrName" select="'left'"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:variable name="Top">
					<xsl:call-template name="getAttribute">
						<xsl:with-param name="str" select="$style"/>
						<xsl:with-param name="attrName" select="'top'"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:variable name="WrapDistLeft">
					<xsl:call-template name="convertToDx">
						<xsl:with-param name="value">
							<xsl:call-template name="getAttribute">
								<xsl:with-param name="str" select="$style"/>
								<xsl:with-param name="attrName" select="'mso-wrap-distance-left'"/>
							</xsl:call-template>
						</xsl:with-param>
					</xsl:call-template>
				</xsl:variable>
				<xsl:variable name="WrapDistRight">
					<xsl:call-template name="convertToDx">
						<xsl:with-param name="value">
							<xsl:call-template name="getAttribute">
								<xsl:with-param name="str" select="$style"/>
								<xsl:with-param name="attrName" select="'mso-wrap-distance-right'"/>
							</xsl:call-template>
						</xsl:with-param>
					</xsl:call-template>
				</xsl:variable>
				<xsl:variable name="WrapDistTop">
					<xsl:call-template name="convertToDx">
						<xsl:with-param name="value">
							<xsl:call-template name="getAttribute">
								<xsl:with-param name="str" select="$style"/>
								<xsl:with-param name="attrName" select="'mso-wrap-distance-top'"/>
							</xsl:call-template>
						</xsl:with-param>
					</xsl:call-template>
				</xsl:variable>
				<xsl:variable name="WrapDistBottom">
					<xsl:call-template name="convertToDx">
						<xsl:with-param name="value">
							<xsl:call-template name="getAttribute">
								<xsl:with-param name="str" select="$style"/>
								<xsl:with-param name="attrName" select="'mso-wrap-distance-bottom'"/>
							</xsl:call-template>
						</xsl:with-param>
					</xsl:call-template>
				</xsl:variable>
				<xsl:call-template name="setBounds">
					<xsl:with-param name="MarginLeft" select="$MarginLeft"/>
					<xsl:with-param name="MarginTop" select="$MarginTop"/>
					<xsl:with-param name="Width" select="$Width"/>
					<xsl:with-param name="Height" select="$Height"/>
					<xsl:with-param name="PosHorRel" select="$PosHorRel"/>
					<xsl:with-param name="PosVertRel" select="$PosVertRel"/>
					<xsl:with-param name="PosHor" select="$PosHor"/>
					<xsl:with-param name="PosVert" select="$PosVert"/>
					<xsl:with-param name="Pos" select="$Pos"/>
					<xsl:with-param name="Left" select="$Left"/>
					<xsl:with-param name="Top" select="$Top"/>
					<xsl:with-param name="WrapDistLeft" select="$WrapDistLeft"/>
					<xsl:with-param name="WrapDistRight" select="$WrapDistRight"/>
					<xsl:with-param name="WrapDistTop" select="$WrapDistTop"/>
					<xsl:with-param name="WrapDistBottom" select="$WrapDistBottom"/>
					<xsl:with-param name="zIndex" select="$ZIndex"/>
					<xsl:with-param name="flip" select="$Flip"/>
					<xsl:with-param name="rotation" select="$Rotate"/>
				</xsl:call-template>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="setBounds">
		<xsl:param name="MarginLeft"/>
		<xsl:param name="MarginTop"/>
		<xsl:param name="Width"/>
		<xsl:param name="Height"/>
		<xsl:param name="PosHorRel"/>
		<xsl:param name="PosVertRel"/>
		<xsl:param name="PosHor"/>
		<xsl:param name="PosVert"/>
		<xsl:param name="Pos"/>
		<xsl:param name="Left"/>
		<xsl:param name="Top"/>
		<xsl:param name="WrapDistLeft"/>
		<xsl:param name="WrapDistRight"/>
		<xsl:param name="WrapDistTop"/>
		<xsl:param name="WrapDistBottom"/>
		<xsl:param name="zIndex"/>
		<xsl:param name="flip"/>
		<xsl:param name="rotation"/>
		<xsl:element name="bounds">
			<xsl:if test="$MarginLeft != ''">
				<xsl:element name="marginLeft">
					<xsl:attribute name="val">
						<xsl:value-of select="$MarginLeft"/>
					</xsl:attribute>
				</xsl:element>
			</xsl:if>
			<xsl:if test="$MarginTop != ''">
				<xsl:element name="marginTop">
					<xsl:attribute name="val">
						<xsl:value-of select="$MarginTop"/>
					</xsl:attribute>
				</xsl:element>
			</xsl:if>
			<xsl:if test="$Width != ''">
				<xsl:element name="width">
					<xsl:attribute name="val">
						<xsl:value-of select="$Width"/>
					</xsl:attribute>
				</xsl:element>
			</xsl:if>
			<xsl:if test="$Height != ''">
				<xsl:element name="height">
					<xsl:attribute name="val">
						<xsl:value-of select="$Height"/>
					</xsl:attribute>
				</xsl:element>
			</xsl:if>
			<xsl:if test="$PosHorRel != ''">
				<xsl:element name="PosHorRel">
					<xsl:attribute name="val">
						<xsl:value-of select="$PosHorRel"/>
					</xsl:attribute>
				</xsl:element>
			</xsl:if>
			<xsl:if test="$PosHor != ''">
				<xsl:element name="PosHor">
					<xsl:attribute name="val">
						<xsl:value-of select="$PosHor"/>
					</xsl:attribute>
				</xsl:element>
			</xsl:if>
			<xsl:if test="$PosVertRel != ''">
				<xsl:element name="PosVertRel">
					<xsl:attribute name="val">
						<xsl:value-of select="$PosVertRel"/>
					</xsl:attribute>
				</xsl:element>
			</xsl:if>
			<xsl:if test="$PosVert != ''">
				<xsl:element name="PosVert">
					<xsl:attribute name="val">
						<xsl:value-of select="$PosVert"/>
					</xsl:attribute>
				</xsl:element>
			</xsl:if>
			<xsl:if test="$Pos != ''">
				<xsl:element name="Pos">
					<xsl:attribute name="val">
						<xsl:value-of select="$Pos"/>
					</xsl:attribute>
				</xsl:element>
			</xsl:if>
			<xsl:if test="$Left != ''">
				<xsl:element name="left">
					<xsl:attribute name="val">
						<xsl:value-of select="$Left"/>
					</xsl:attribute>
				</xsl:element>
			</xsl:if>
			<xsl:if test="$Top != ''">
				<xsl:element name="top">
					<xsl:attribute name="val">
						<xsl:value-of select="$Top"/>
					</xsl:attribute>
				</xsl:element>
			</xsl:if>
			<xsl:if test="$WrapDistLeft != ''">
				<xsl:element name="wrapDistLeft">
					<xsl:attribute name="val">
						<xsl:value-of select="$WrapDistLeft"/>
					</xsl:attribute>
				</xsl:element>
			</xsl:if>
			<xsl:if test="$WrapDistRight != ''">
				<xsl:element name="wrapDistRight">
					<xsl:attribute name="val">
						<xsl:value-of select="$WrapDistRight"/>
					</xsl:attribute>
				</xsl:element>
			</xsl:if>
			<xsl:if test="$WrapDistTop != ''">
				<xsl:element name="wrapDistTop">
					<xsl:attribute name="val">
						<xsl:value-of select="$WrapDistTop"/>
					</xsl:attribute>
				</xsl:element>
			</xsl:if>
			<xsl:if test="$WrapDistBottom != ''">
				<xsl:element name="wrapDistBottom">
					<xsl:attribute name="val">
						<xsl:value-of select="$WrapDistBottom"/>
					</xsl:attribute>
				</xsl:element>
			</xsl:if>
			<xsl:if test="$zIndex != ''">
				<xsl:element name="z-index">
					<xsl:attribute name="val">
						<xsl:value-of select="$zIndex"/>
					</xsl:attribute>
				</xsl:element>
			</xsl:if>
			<xsl:if test="$flip != ''">
				<xsl:element name="flip">
					<xsl:attribute name="val">
						<xsl:value-of select="$flip"/>
					</xsl:attribute>
				</xsl:element>
			</xsl:if>
			<xsl:if test="$rotation != ''">
				<xsl:element name="rotation">
					<xsl:attribute name="val">
						<xsl:value-of select="$rotation"/>
					</xsl:attribute>
				</xsl:element>
			</xsl:if>
		</xsl:element>
	</xsl:template>

	<xsl:template name="insertTextBox">
		<xsl:variable name="style">
			<xsl:value-of select="./v:textbox/@style"/>
		</xsl:variable>
		<xsl:variable name="mustFit">
			<xsl:call-template name="getAttribute">
				<xsl:with-param name="str" select="$style"/>
				<xsl:with-param name="attrName" select="'mso-fit-shape-to-text'"/>
			</xsl:call-template>
		</xsl:variable>
		<textBox mustFit="{$mustFit}">
			<xsl:apply-templates select="./v:textbox/w:txbxContent/*"/>
		</textBox>
	</xsl:template>

	<xsl:template name="convertToDx">
		<xsl:param name="value"/>
		<xsl:choose>
			<xsl:when test="contains($value, 'cm')">
				<xsl:value-of select="round(number(substring-before($value, 'cm')) * 1440 div 2.54)"/>
			</xsl:when>
			<xsl:when test="contains($value, 'mm')">
				<xsl:value-of select="round(number(substring-before($value, 'mm')) * 1440 div 25.4)"/>
			</xsl:when>
			<xsl:when test="contains($value, 'in')">
				<xsl:value-of select="round(number(substring-before($value, 'in')) * 1440)"/>
			</xsl:when>
			<xsl:when test="contains($value, 'pt')">
				<xsl:value-of select="round(number(substring-before($value, 'pt')) * 20)"/>
			</xsl:when>
			<xsl:when test="contains($value, 'px')">
				<xsl:value-of select="round(number(substring-before($value, 'px')) * 1440 div 96.19)"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$value"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<xsl:template name="convertToMM">
		<xsl:param name="value"/>
		<xsl:choose>
			<xsl:when test="contains($value, 'cm')">
				<xsl:value-of select="round(number(substring-before($value, 'cm')) * 10)"/>
			</xsl:when>
			<xsl:when test="contains($value, 'mm')">
				<xsl:value-of select="round(number(substring-before($value, 'mm')))"/>
			</xsl:when>
			<xsl:when test="contains($value, 'in')">
				<xsl:value-of select="round(number(substring-before($value, 'in')) * 25.4)"/>
			</xsl:when>
			<xsl:when test="contains($value, 'pt')">
				<xsl:value-of select="round(number(substring-before($value, 'pt')) * 20 * 25.4 div 1440)"/>
			</xsl:when>
			<xsl:when test="contains($value, 'px')">
				<xsl:value-of select="round(number(substring-before($value, 'px')) * 25.4 div 96.19)"/>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="getAttribute">
		<xsl:param name="str"/>
		<xsl:param name="attrName"/>
		<xsl:param name="withTrim" select="true"/>
		<xsl:choose>
			<xsl:when test="(contains($attrName,'width') or contains($attrName,'height')
			or contains($attrName, 'top') or contains($attrName, 'bottom')) 
			and contains($str, concat('-', $attrName, ':'))">
				<xsl:call-template name="getAttribute">
					<xsl:with-param name="str" select="concat(substring-before($str, concat('-', $attrName)), substring-after($str, concat('-', $attrName)))"/>
					<xsl:with-param name="attrName" select="$attrName"/>
					<xsl:with-param name="withTrim" select="$withTrim"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:if test="contains($str, concat($attrName, ':'))">
					<xsl:variable name="str2">
						<!--<xsl:value-of select="substring-after($str, concat($attrName, ':'))"/>-->
						<xsl:call-template name="lastSubstringAfter">
							<xsl:with-param name="str" select="$str"/>
							<xsl:with-param name="item" select="concat($attrName, ':')"/>
						</xsl:call-template>						
					</xsl:variable>
					<xsl:variable name="value">
						<xsl:choose>
							<xsl:when test="contains($str2, ';')">
								<xsl:value-of select="normalize-space(substring-before($str2, ';'))"/>
								<!--<xsl:call-template name="trim">
									<xsl:with-param name="str" select="substring-before($str2, ';')"/>
								</xsl:call-template>-->
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="normalize-space($str2)"/>
								<!--<xsl:call-template name="trim">
									<xsl:with-param name="str" select="$str2"/>
								</xsl:call-template>-->
							</xsl:otherwise>
						</xsl:choose>
					</xsl:variable>
					<!--<xsl:message>
						<xsl:value-of select="$attrName"/>
						<xsl:text> - </xsl:text>
						<xsl:value-of select="$str2"/>
						<xsl:text> - </xsl:text>
						<xsl:value-of select="$value"/>
					</xsl:message>-->
					<xsl:choose>
						<xsl:when test="$withTrim = 'true'">
							<xsl:call-template name="trim">
								<xsl:with-param name="str" select="$value"/>
							</xsl:call-template>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="$value"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:if>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="lastSubstringAfter">
		<xsl:param name="str"/>
		<xsl:param name="item"/>
		<xsl:variable name="substr">
			<xsl:value-of select="substring-after($str, $item)"/>
		</xsl:variable>
		<xsl:variable name="notLastOccure">
			<xsl:value-of select="substring-after($substr, $item)"/>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="$notLastOccure != ''">
				<xsl:call-template name="lastSubstringAfter">
					<xsl:with-param name="str" select="$substr"/>
					<xsl:with-param name="item" select="$item"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$substr"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="trim">
		<xsl:param name="str"/>
		<xsl:variable name="trimLeft">
			<xsl:call-template name="leftTrim">
				<xsl:with-param name="str" select="$str"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:call-template name="trimRight">
			<xsl:with-param name="str" select="$trimLeft"/>
		</xsl:call-template>
	</xsl:template>


	<xsl:template name="leftTrim">
		<xsl:param name="str"/>
		<xsl:choose>
			<xsl:when test="starts-with($str, ' ')">
				<xsl:call-template name="leftTrim">
					<xsl:with-param name="str" select="substring-after($str, ' ')"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$str"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="trimRight">
		<xsl:param name="str"/>
		<xsl:choose>
			<xsl:when test="contains($str, ' ')">
				<xsl:value-of select="substring-before($str, ' ')"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$str"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>




	<!--
    <xsl:template match="@* | node()">
        <xsl:copy>
            <xsl:apply-templates select="@* | node()"/>
        </xsl:copy>
    </xsl:template>
  -->
</xsl:stylesheet>
