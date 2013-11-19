<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:msxsl="urn:schemas-microsoft-com:xslt"
                xmlns:xlsx="http://www.avs4you.ru/XLSXConverter/1.0/DirectoryStructure">

  <xsl:template match="ChartSheetSubstream">
    <xlsx:file name="sheet{position()}.xml">
      <chartsheet xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships">

        <xsl:call-template name="sheetPr"/>

        <xsl:call-template name="sheetViews_charts"/>

        <xsl:call-template name="sheetProtection"/>

        <xsl:call-template name="customSheetViews"/>

        <xsl:for-each select="PAGESETUP[1]">
          <xsl:call-template name="pageMargins"/>
          <xsl:call-template name="pageSetup"/>
          <xsl:call-template name="headerFooter"/>
        </xsl:for-each>

        <drawing xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships">
          <xsl:attribute name="r:id">
            <xsl:text>rId</xsl:text>
            <xsl:call-template name="generate_worksheet_entity_id"/>
          </xsl:attribute>
        </drawing>

        <!-- TODO: Implement the rest -->

      </chartsheet>
    </xlsx:file>
  </xsl:template>


  <xsl:template name="sheetViews_charts">
    <sheetViews xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
      <xsl:for-each select="WINDOW">
        <sheetView>
          <xsl:call-template name="attrib_with_def">
            <xsl:with-param name="name">tabSelected</xsl:with-param>
            <xsl:with-param name="val" select="Window2/@fSelected"/>
            <xsl:with-param name="def_val">false</xsl:with-param>
          </xsl:call-template>
          <xsl:if test="Scl">
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">zoomScale</xsl:with-param>
              <xsl:with-param name="val" select="round(Scl/@nscl div Scl/@dscl * 100)"/>
              <!-- Back:  @dscl = 100; @nscl = zoomScale -->
              <xsl:with-param name="def_val">100</xsl:with-param>
            </xsl:call-template>
          </xsl:if>
          <xsl:attribute name="workbookViewId">
            <xsl:value-of select="position() - 1"/>
          </xsl:attribute>

          <xsl:if test="not(Scl)">
            <xsl:attribute name="zoomToFit">true</xsl:attribute>
          </xsl:if>
        </sheetView>
      </xsl:for-each>
    </sheetViews>
  </xsl:template>



</xsl:stylesheet>