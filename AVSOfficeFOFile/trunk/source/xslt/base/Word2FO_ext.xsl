<?xml version="1.0" encoding="UTF-8"?>

<!-- =====================================================

This stylesheet contains some RenderX extensions to XSL-FO
for WordML conversion.

**********************************************************

IMPLEMENTATION NOTES

The extensions add the following functionality as compared
with the generic stylesheet:

1. Flow sections - you can specify continuous sections 
   with different number of columns.
2. Document properties - you can save meta information
   (document properties) in the output file.
3. PDF hierarchical bookmarks. 

======================================================= -->


<xsl:stylesheet xmlns:fo="http://www.w3.org/1999/XSL/Format"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:rx="http://www.renderx.com/XSL/Extensions"
                xmlns:w="http://schemas.microsoft.com/office/word/2003/wordml"
                xmlns:wx="http://schemas.microsoft.com/office/word/2003/auxHint"
                xmlns:o="urn:schemas-microsoft-com:office:office"
                version="1.0">

  <xsl:import href="Word2FO.xsl"/>

  <xsl:output method="xml"
              version="1.0"
              indent="no"
              encoding="utf-8" />

<!-- ========================== -->
<!-- REDEFINED GLOBAL CONSTANTS -->
<!-- ========================== -->
<xsl:variable name="section-block-name" select="'rx:flow-section'"/>
<xsl:variable name="section-block-namespace" select="'http://www.renderx.com/XSL/Extensions'"/>



<!-- =================== -->
<!-- MAIN ROOT TRANSFORM -->
<!-- =================== -->
  <xsl:template match="/w:wordDocument">
  
  <fo:root>
    <!-- Set default font-family attribute on fo:root -->
    <xsl:apply-templates select="w:fonts/w:defaultFonts"/>
    
    <!-- ==================== -->
    <!-- Set meta information -->
    <!-- ==================== -->
    <rx:meta-info>
     <xsl:apply-templates select="o:DocumentProperties/*" />
    </rx:meta-info>
    
    <!-- Create physical page layout -->
    <xsl:call-template name="CreatePageLayout"/>
    
    <!-- ========= -->
    <!-- Bookmarks -->
    <!-- ========= -->
    <rx:outline>
      <xsl:apply-templates select="w:body" mode="bookmarks" />
    </rx:outline>
    
    <!-- Generate page sequences -->
    <xsl:call-template name="GeneratePageSequences"/>
    
  </fo:root>
  </xsl:template>



<!-- ================ -->
<!-- Meta information -->
<!-- ================ -->
    <xsl:template match="o:DocumentProperties/o:*" priority="-1">
      <rx:meta-field>
        <!-- "name" attribute -->
        <xsl:attribute name="name">
          <xsl:call-template name="ConvertNameToLowercase">
            <xsl:with-param name="field-name" select="local-name()"/>
          </xsl:call-template>
        </xsl:attribute>
        <!-- "value" attribute -->
        <xsl:attribute name="value">
          <xsl:value-of select="."/>
        </xsl:attribute>
      </rx:meta-field>
    </xsl:template>

    <!-- ================================== -->
    <!-- Convert element names to lowercase -->
    <!-- ================================== -->
    <xsl:template name="ConvertNameToLowercase">
      <xsl:param name="field-name"/>

      <xsl:variable name="lc-letter" select="'abcdefghijklmnopqrstuvwxyz'" />
      <xsl:variable name="uc-letter" select="'ABCDEFGHIJKLMNOPQRSTUVWXYZ'" />

      <xsl:value-of select="translate($field-name, $uc-letter, $lc-letter)"/>
    </xsl:template>



<!-- =============== -->
<!--    Bookmarks    -->
<!-- =============== -->
    <xsl:template match="wx:sub-section" mode="bookmarks">
       <rx:bookmark internal-destination="{generate-id()}">
         <rx:bookmark-label>
           <xsl:value-of select="w:p[1]/w:r/w:t" />
         </rx:bookmark-label>

         <xsl:apply-templates select="*" mode="bookmarks" />
      </rx:bookmark>
    </xsl:template>

    <xsl:template match="*" mode="bookmarks" priority="-1">
      <xsl:apply-templates select="*" mode="bookmarks"/>
    </xsl:template>

</xsl:stylesheet>
