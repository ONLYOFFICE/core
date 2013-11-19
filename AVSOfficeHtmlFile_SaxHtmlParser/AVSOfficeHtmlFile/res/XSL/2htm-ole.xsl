<?xml version="1.0" encoding="UTF-8" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"
                xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
                xmlns:o="urn:schemas-microsoft-com:office:office"
                xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
                xmlns:rels="http://schemas.openxmlformats.org/package/2006/relationships"
                xmlns:xlink="http://www.w3.org/1999/xlink"
                xmlns:v="urn:schemas-microsoft-com:vml"
                xmlns:pzip="urn:cleverage:xmlns:post-processings:zip"
                xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
                xmlns:oox="urn:oox"
                xmlns="http://www.w3.org/1999/xhtml"
                exclude-result-prefixes="xlink draw rels oox">

	<!-- 
  *************************************************************************
  MATCHING TEMPLATES
  *************************************************************************
  -->

	<!--
  Summary: Converts OLE objects
  Author: makz (DIaLOGIKa)
  Date: 14.11.2007
  -->
	<xsl:template match="o:OLEObject">
		<span>
			<img>

				<xsl:attribute name="style">
					<xsl:call-template name="InsertCommonShapeProperties">
						<xsl:with-param name="shape" select="../v:shape" />
					</xsl:call-template>
					<xsl:call-template name="InsertShapeStyleProperties">
						<xsl:with-param name="shape" select="../v:shape" />
					</xsl:call-template>
					<xsl:call-template name="InsertShapeZindex"/>
				</xsl:attribute>

				<xsl:variable name="imageId" select="../v:shape/v:imagedata/@r:id" />

				<!-- get the hosting document (headerX.xml, document.xml) -->
				<xsl:variable name="hostingDoc" select="ancestor::oox:part/@oox:name" />
				<xsl:variable name="relFile" select="concat('word/_rels/', concat(substring-after($hostingDoc, '/'), '.rels'))" />


				<xsl:variable name="imagePath" select="key('Part', $relFile)/rels:Relationships/rels:Relationship[@Id=$imageId]/@Target" />

				<xsl:variable name="filename" select="substring-after($imagePath, '/')" />
				<xsl:attribute name="src">
					<xsl:value-of  select="$filename"/>
					<!--<xsl:value-of  select="concat('$MediaFolder$/', $filename)"/>-->
				</xsl:attribute>
				<xsl:attribute name="alt">
					<xsl:text>OLE - object!</xsl:text>
				</xsl:attribute>
			</img>

			<!--<xsl:call-template name="InsertObject">
        <xsl:with-param name="oldFilePath" select="$objectPath" />
        <xsl:with-param name="oldFileName" select="$objectName" />
        <xsl:with-param name="newFileName" select="$newName" />
      </xsl:call-template>-->
			<!--<xsl:call-template name="InsertObjectPreview">
                <xsl:with-param name="imageFilePath" select="$imagePath" />
                <xsl:with-param name="newFileName" select="$newName" />
            </xsl:call-template>-->
		</span>
	</xsl:template>

	<!--
  Summary: Converts the style for OLE objects
  Author: makz (DIaLOGIKa)
  Date: 14.11.2007
  -->
	<!--
    <xsl:template match="o:OLEObject" mode="automaticstyles">
        <style:style style:family="graphic" style:parent-style-name="Frame">
            <xsl:attribute name="style:name">
                <xsl:value-of select="generate-id(../v:shape)"/>
            </xsl:attribute>

            <style:graphic-properties>
                <xsl:call-template name="InsertShapeStyleProperties">
                    <xsl:with-param name="shape" select="../v:shape" />
                </xsl:call-template>
            </style:graphic-properties>
        </style:style>
    </xsl:template>
    -->

	<!-- 
  *************************************************************************
  CALLED TEMPLATES
  *************************************************************************
  -->

	<xsl:template name="InsertOlePreviewName">
		<xsl:param name="thisId" />
		<xsl:param name="relFile" />

		<!-- get the hosting document (headerX.xml, document.xml) -->
		<xsl:variable name="hostingDoc">
			<xsl:call-template name="substring-after-last">
				<xsl:with-param name ="string" select="substring-before($relFile, '.rels')" />
				<xsl:with-param name="occurrence" select="'/'" />
			</xsl:call-template>
		</xsl:variable>
		<xsl:variable name="olePreview" select="key('Part', concat('word/',$hostingDoc))//v:shape/v:imagedata[@r:id=$thisId]" />
		<xsl:variable name="oleId" select="$olePreview/../../o:OLEObject/@r:id" />
		<xsl:variable name="allRels" select="key('Part', $relFile)/rels:Relationships/rels:Relationship" />
		<xsl:variable name="previewRelTarget" select="$allRels[@Id=$oleId]/@Target" />

		<xsl:value-of select="substring-before(substring-after($previewRelTarget, '/'), '.')" />
	</xsl:template>


	<!--
  Summary: inserts the object itself
  Author: makz (DIaLOGIKa)
  Date: 14.11.2007
  -->
	<xsl:template name="InsertObject">
		<xsl:param name="oldFilePath" />
		<xsl:param name="newFileName" />

		<xsl:choose>
			<!-- it's an embedded object -->
			<xsl:when test="@Type='Embed'">
				<!-- copy the embedded object -->
				<pzip:copy pzip:source="{concat('word/', $oldFilePath)}"
                           pzip:target="{$newFileName}"/>

				<draw:object-ole>
					<xsl:call-template name="InsertObjectHref">
						<xsl:with-param name="link" select="concat('./', $newFileName)" />
					</xsl:call-template>
					<xsl:call-template name="InsertObjectShow" />
					<xsl:call-template name="InsertObjectType" />
					<xsl:call-template name="InsertObjectActuate" />
				</draw:object-ole>
			</xsl:when>
			<!-- it's an external object -->
			<xsl:otherwise>
				<draw:object>
					<xsl:call-template name="InsertObjectHref">
						<xsl:with-param name="link" select="concat('http://www.dialogika.de/odf-converter/makeOdfPath#', substring-after($oldFilePath, 'file:///'))" />
					</xsl:call-template>
					<xsl:call-template name="InsertObjectShow" />
					<xsl:call-template name="InsertObjectType" />
					<xsl:call-template name="InsertObjectActuate" />
				</draw:object>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!--
  Summary: inserts the preview picture of the object
  Author: makz (DIaLOGIKa)
  Date: 14.11.2007
  -->
	<xsl:template name="InsertObjectPreview">
		<xsl:param name="imageFilePath" />
		<xsl:param name="newFileName" />

		<!--<xsl:variable name="suffix" select="substring-after($imageFilePath, '.')" />

        <img>
            <xsl:attribute name="src">
                <xsl:value-of  select="concat('word/', $imageFilePath)"/>
            </xsl:attribute>-->
		<xsl:variable name="filename" select="substring-after($imageFilePath, '/')" />

		<img>
			<xsl:attribute name="src">
				<xsl:value-of  select="$filename"/>
			</xsl:attribute>
			<xsl:attribute name="alt">
				<xsl:text>OLE - object!</xsl:text>
			</xsl:attribute>
			<!--
      <xsl:call-template name="InsertObjectShow" />
      <xsl:call-template name="InsertObjectType" />
      <xsl:call-template name="InsertObjectActuate" />
      
      <xsl:call-template name="InsertObjectHref">
        <xsl:with-param name="link" select="concat('./ObjectReplacements/', $newFileName)" />
      </xsl:call-template>
      
      <!-@- copy placeholder for unsupported images -@->
      <xsl:choose>
        <!-@-
        <xsl:when test="$suffix='wmf' or $suffix='emf'">
        -@->
        <xsl:when test="$suffix='xxx'">
          <pzip:copy pzip:source="#CER#WordprocessingConverter.dll#OdfConverter.Wordprocessing.resources.OLEplaceholder.png#"
                     pzip:target="{concat('ObjectReplacements/', $newFileName)}" />
        </xsl:when>

        <xsl:otherwise>
          <pzip:copy pzip:source="{concat('word/', $imageFilePath)}"
                     pzip:target="{concat('ObjectReplacements/', $newFileName)}" />
        </xsl:otherwise>
      </xsl:choose>
        -->

		</img>
	</xsl:template>


	<!--
  Summary: Inserts the href attribute to the object and copies the file if it is internal
  Author: makz (DIaLOGIKa)
  Date: 14.11.2007
  -->
	<xsl:template name="InsertObjectHref">
		<xsl:param name="link" select="''" />

		<xsl:attribute name="href" >
			<xsl:value-of select="$link"/>
		</xsl:attribute>
	</xsl:template>

	<!--
  Summary: Inserts the show attribute to the object
  Author: makz (DIaLOGIKa)
  Date: 14.11.2007
  -->
	<xsl:template name="InsertObjectShow">
		<xsl:attribute name="xlink:show">
			<xsl:text>embed</xsl:text>
		</xsl:attribute>
	</xsl:template>

	<!--
  Summary: Inserts the type attribute to the object
  Author: makz (DIaLOGIKa)
  Date: 14.11.2007
  -->
	<xsl:template name="InsertObjectType">
		<xsl:attribute name="xlink:type">
			<xsl:text>simple</xsl:text>
		</xsl:attribute>
	</xsl:template>

	<!--
  Summary: Inserts the actuate attribute to the object
  Author: makz (DIaLOGIKa)
  Date: 14.11.2007
  -->
	<xsl:template name="InsertObjectActuate">
		<xsl:attribute name="xlink:actuate">
			<xsl:text>onLoad</xsl:text>
		</xsl:attribute>
	</xsl:template>

</xsl:stylesheet>