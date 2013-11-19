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
<!--
Modification Log
LogNo. |Date       |ModifiedBy   |BugNo.   |Modification                                                      |
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
RefNo-1 15-Jul-2008 Sandeep S     1874669  Changes done to fix External Links are not retaining.                                               
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
  xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
  xmlns:o="urn:schemas-microsoft-com:office:office"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns:number="urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0"
  xmlns:v="urn:schemas-microsoft-com:vml" xmlns:xlink="http://www.w3.org/1999/xlink"
  xmlns:dc="http://purl.org/dc/elements/1.1/"
  xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
  xmlns:mc="http://schemas.openxmlformats.org/markup-compatibility/2006"
  xmlns:e="http://schemas.openxmlformats.org/spreadsheetml/2006/main" exclude-result-prefixes="e r v">

  <!--<xsl:import href="measures.xsl"/>-->

  <xsl:template name="InsertOLEObjects">
    <xsl:choose>
      <xsl:when
        test="e:oleObject[not(@r:id)] and not (key('Part', 'xl/drawings/vmlDrawing1.vml')//node()[name() = 'v:group' ])">
        <table:shapes>
          <xsl:for-each select="e:oleObject ">
            <xsl:call-template name="InsertOLEObjectsLinks"/>
          </xsl:for-each>
        </table:shapes>
      </xsl:when>
		<!-- Defect Desc: 2948283- Link Object Missing with Office 2010 Compatibility
			 By:          Vijayeta
			 Desc:        Additional when condition for additional nodes in Excel 2010 for extensibility
		-->
		<xsl:when
	   test="mc:AlternateContent/mc:Fallback/e:oleObject[not(@r:id)]">
			<table:shapes>
				<xsl:for-each select="mc:AlternateContent/mc:Fallback/e:oleObject ">
					<xsl:call-template name="InsertOLEObjectsLinks"/>
				</xsl:for-each>
			</table:shapes>
		</xsl:when>
		<!-- End of 2948283-->
      <xsl:when test="e:oleObject or mc:AlternateContent/mc:Fallback/e:oleObject" >
        <xsl:message terminate="no">translation.oox2odf.OLEObject</xsl:message>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="InsertOLEObjectsLinks">

    <xsl:variable name="ProgId">
      <xsl:value-of select="@progId"/>
    </xsl:variable>

    <xsl:variable name="NumberExternalLink">
      <xsl:value-of select="@link"/>
    </xsl:variable>

    <xsl:variable name="ExternalLink">
      <xsl:value-of
        select="concat('xl/externalLinks/externalLink', substring-after(substring-before($NumberExternalLink, ']'), '['), '.xml')"
      />
    </xsl:variable>

    <xsl:variable name="ExternalLinkRels">
      <xsl:value-of
        select="concat('xl/externalLinks/_rels/externalLink', substring-after(substring-before($NumberExternalLink, ']'), '['), '.xml', '.rels')"
      />
    </xsl:variable>

    <xsl:variable name="ExternalLinkId">
<!--'key' replaced by document, as ole objects do not get converted with 'key'-->
      <xsl:for-each select ="document($ExternalLink)">
        <!--<xsl:for-each select="key('Part', $ExternalLink)">-->
        <xsl:value-of select="e:externalLink/e:oleLink[@progId = $ProgId]/@r:id"/>
      </xsl:for-each>
    </xsl:variable>

    <xsl:variable name="XlinkOLEObject">
<!--'key' replaced by document, as ole objects do not get converted with 'key'-->

      <xsl:for-each select ="document($ExternalLinkRels)//node()[name()='Relationship']">
        <!--<xsl:for-each select="key('Part', $ExternalLinkRels)//node()[name()='Relationship']">-->
        <xsl:if test="./@Id = $ExternalLinkId">
          <xsl:choose>
            <!--added by chhavi for network path -->
            <xsl:when test="contains(./@Target, 'file:///\\')">
              <xsl:value-of select="translate(substring-after(./@Target, 'file:///'), '\', '/')"/>
            </xsl:when>
            <!-- ending here-->
            <xsl:when test="contains(./@Target, 'file://')">
              <xsl:value-of select="translate(substring-after(./@Target, 'file://'), '\', '/')"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="translate(concat('../', ./@Target), '\', '/')"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:if>
      </xsl:for-each>
    </xsl:variable>

    <xsl:variable name="ShapeId">
      <xsl:value-of select="concat('_x0000_s', @shapeId)"/>
    </xsl:variable>

    <xsl:if test="$XlinkOLEObject != ''">
      <draw:frame>
        <!--RefNo-1-->
        <!--select="key('Part', 'xl/drawings/vmlDrawing1.vml')//node()[name() = 'v:shape' and contains(@id,$ShapeId)] "/>-->
        <xsl:apply-templates
          select ="document('xl/drawings/vmlDrawing1.vml')//node()[name() = 'v:shape' and contains(@id,$ShapeId)]"/> 
        <draw:object xlink:type="simple" xlink:show="embed" xlink:actuate="onLoad">
          <xsl:attribute name="xlink:href">
            <xsl:value-of select="$XlinkOLEObject"/>
          </xsl:attribute>
        </draw:object>
      </draw:frame>

    </xsl:if>

  </xsl:template>


  <xsl:template match="v:shape">
    <xsl:call-template name="InsertshapeAbsolutePos"/>
    <xsl:call-template name="InsertShapeWidth"/>
    <xsl:call-template name="InsertShapeHeight"/>
  </xsl:template>

  <xsl:template name="InsertshapeAbsolutePos">
    <xsl:param name="shape" select="."/>

    <xsl:variable name="position">
      <xsl:call-template name="GetShapeProperties">
        <xsl:with-param name="shape" select="$shape"/>
        <xsl:with-param name="propertyName" select="'position'"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:if test="$position = 'absolute' or $shape[name()='e:framePr']">
      <xsl:variable name="svgx">
        <xsl:choose>
          <xsl:when test="$shape[name()='e:framePr']">
            <xsl:value-of select="$shape/@e:x"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="GetShapeProperties">
              <xsl:with-param name="shape" select="$shape"/>
              <xsl:with-param name="propertyName" select="'margin-left'"/>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>

      <xsl:attribute name="svg:x">
        <xsl:call-template name="ConvertMeasure">
          <xsl:with-param name="length" select="$svgx"/>
          <xsl:with-param name="destUnit" select="'cm'"/>
        </xsl:call-template>
      </xsl:attribute>

      <xsl:variable name="svgy">
        <xsl:choose>
          <xsl:when test="$shape[name()='e:framePr']">
            <xsl:value-of select="$shape/@e:y"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="GetShapeProperties">
              <xsl:with-param name="shape" select="$shape"/>
              <xsl:with-param name="propertyName" select="'margin-top'"/>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>

      <xsl:attribute name="svg:y">
        <xsl:call-template name="ConvertMeasure">
          <xsl:with-param name="length" select="$svgy"/>
          <xsl:with-param name="destUnit" select="'cm'"/>
        </xsl:call-template>
      </xsl:attribute>
    </xsl:if>
  </xsl:template>

  <xsl:template name="GetShapeProperties">
    <xsl:param name="shape"/>
    <xsl:param name="propertyName"/>
    <xsl:variable name="propertyValue">
      <xsl:choose>
        <xsl:when test="contains(substring-after($shape/@style,concat($propertyName,':')),';')">
          <xsl:value-of
            select="substring-before(substring-after($shape/@style,concat($propertyName,':')),';')"
          />
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="substring-after($shape/@style,concat($propertyName,':'))"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:value-of select="$propertyValue"/>
  </xsl:template>


  <xsl:template name="InsertShapeWidth">
    <xsl:param name="shape" select="."/>
    <xsl:variable name="wrapStyle">
      <xsl:choose>
        <xsl:when test="$shape[name()='e:framePr']">
          <xsl:value-of select="$shape/@wrap"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:call-template name="GetShapeProperties">
            <xsl:with-param name="shape" select="$shape"/>
            <xsl:with-param name="propertyName" select="'mso-wrap-style'"/>
          </xsl:call-template>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="relativeWidth">
      <xsl:call-template name="GetShapeProperties">
        <xsl:with-param name="shape" select="$shape"/>
        <xsl:with-param name="propertyName" select="'mso-width-percent'"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="$relativeWidth != ''">
        <xsl:call-template name="InsertShapeRelativeWidth">
          <xsl:with-param name="shape" select="$shape"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="$wrapStyle = '' or $wrapStyle != 'none' ">
        <xsl:variable name="width">
          <xsl:choose>
            <xsl:when test="$shape[name()='e:framePr']">
              <xsl:value-of select="$shape/@e:w"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:call-template name="GetShapeProperties">
                <xsl:with-param name="shape" select="$shape"/>
                <xsl:with-param name="propertyName" select="'width'"/>
              </xsl:call-template>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        <xsl:attribute name="svg:width">
          <xsl:choose>
            <xsl:when test="$width = 0 and $shape//@o:hr='t'">
              <xsl:call-template name="ConvertTwips">
                <xsl:with-param name="length"
                  select="following::e:pgSz[1]/@w - following::e:pgMar/@right[1] - following::e:pgMar/@left[1]"/>
                <xsl:with-param name="unit" select="'cm'"/>
              </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
              <xsl:call-template name="ConvertMeasure">
                <xsl:with-param name="length" select="$width"/>
                <xsl:with-param name="destUnit" select="'cm'"/>
              </xsl:call-template>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="InsertShapeRelativeWidth">
    <xsl:param name="shape" select="."/>

    <xsl:variable name="relativeWidth">
      <xsl:call-template name="GetShapeProperties">
        <xsl:with-param name="shape" select="$shape"/>
        <xsl:with-param name="propertyName" select="'mso-width-percent'"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:attribute name="style:rel-width">
      <xsl:value-of select="$relativeWidth div 10"/>
    </xsl:attribute>

    <xsl:variable name="relativeTo">
      <xsl:call-template name="GetShapeProperties">
        <xsl:with-param name="shape" select="$shape"/>
        <xsl:with-param name="propertyName" select="'mso-width-relative'"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:if test="$relativeTo != ''">
      <xsl:message terminate="no">translation.oox2odf.frame.relativeSize</xsl:message>
    </xsl:if>
  </xsl:template>


  <xsl:template name="InsertShapeHeight">
    <xsl:param name="shape" select="."/>

    <xsl:variable name="height">
      <xsl:choose>
        <xsl:when test="$shape[name()='e:framePr']">
          <xsl:value-of select="$shape/@h"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:call-template name="GetShapeProperties">
            <xsl:with-param name="shape" select="$shape"/>
            <xsl:with-param name="propertyName" select="'height'"/>
          </xsl:call-template>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="relativeHeight">
      <xsl:choose>
        <xsl:when test="$shape[name()='e:framePr']"/>
        <xsl:otherwise>
          <xsl:call-template name="GetShapeProperties">
            <xsl:with-param name="shape" select="$shape"/>
            <xsl:with-param name="propertyName" select="'mso-height-percent'"/>
          </xsl:call-template>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="$relativeHeight != ''">
        <xsl:call-template name="InsertShapeRelativeHeight">
          <xsl:with-param name="shape" select="$shape"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:if
          test="not(contains($shape/v:textbox/@style, 'mso-fit-shape-to-text:t'))  or $shape/@h">
          <xsl:attribute name="svg:height">
            <xsl:call-template name="ConvertMeasure">
              <xsl:with-param name="length" select="$height"/>
              <xsl:with-param name="destUnit" select="'cm'"/>
            </xsl:call-template>
          </xsl:attribute>
        </xsl:if>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="InsertShapeRelativeHeight">
    <xsl:param name="shape" select="."/>

    <xsl:variable name="relativeHeight">
      <xsl:call-template name="GetShapeProperties">
        <xsl:with-param name="shape" select="$shape"/>
        <xsl:with-param name="propertyName" select="'mso-height-percent'"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:attribute name="style:rel-height">
      <xsl:value-of select="$relativeHeight div 10"/>
    </xsl:attribute>

    <xsl:variable name="relativeTo">
      <xsl:call-template name="GetShapeProperties">
        <xsl:with-param name="shape" select="$shape"/>
        <xsl:with-param name="propertyName" select="'mso-height-relative'"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:if test="$relativeTo != ''">
      <xsl:message terminate="no">translation.oox2odf.frame.relativeSize</xsl:message>
    </xsl:if>
  </xsl:template>


</xsl:stylesheet>
