<?xml version="1.0" encoding="UTF-8"?>

<!-- ========================================================

This stylesheet defines physical page layouts and creates
page sequences.                  

*************************************************************

IMPLEMENTATION SPECIFICS

1. Continuous sections don't genarate its own page sequences.
2. Contents of static regions are retrieved via fo:marker
elements.

========================================================== -->


<xsl:stylesheet xmlns:fo="http://www.w3.org/1999/XSL/Format"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:rx="http://www.renderx.com/XSL/Extensions"
                xmlns:w="http://schemas.microsoft.com/office/word/2003/wordml"
                xmlns:o="urn:schemas-microsoft-com:office:office"
                xmlns:v="urn:schemas-microsoft-com:vml"
                xmlns:wx="http://schemas.microsoft.com/office/word/2003/auxHint"
                xmlns:aml="http://schemas.microsoft.com/aml/2001/core"
                xmlns:w10="urn:schemas-microsoft-com:office:word"
                xmlns:dt="uuid:C2F41010-65B3-11d1-A29F-00AA00C14882"
                version="1.0">

  <xsl:output method="xml"
              version="1.0"
              indent="no"
              encoding="utf-8" />


  <!-- ============================ -->
  <!-- Create physical page layout. -->
  <!-- ============================ -->
  <xsl:template name="CreatePageLayout">
    <fo:layout-master-set>
  
      <!-- ========================================================= -->
      <!-- Generate page masters and page sequences for each section -->
      <!-- ========================================================= -->
      <xsl:for-each select="w:body/wx:sect">
        <xsl:variable name="first-page-name" select="concat('section',position(),'-first-page')"/>
        <xsl:variable name="odd-page-name" select="concat('section',position(),'-odd-page')"/>
        <xsl:variable name="even-page-name" select="concat('section',position(),'-even-page')"/>
        <xsl:variable name="page-sequence-master-name" select="concat('section',position(),'-page-sequence-master')"/>
        <!-- ====================================== -->
        <!-- Masters for first, odd, and even pages -->
        <!-- ====================================== -->
        <fo:simple-page-master master-name="{$first-page-name}">
          <xsl:apply-templates select=".//w:sectPr/w:pgSz"/>
          <xsl:apply-templates select=".//w:sectPr/w:pgMar">
            <xsl:with-param name="area" select="'page-master'"/>
          </xsl:apply-templates>
          
          <fo:region-body>
            <xsl:apply-templates select=".//w:sectPr/w:pgMar">
              <xsl:with-param name="area" select="'region-body'"/>
            </xsl:apply-templates>
            <xsl:apply-templates select=".//w:sectPr/w:pgBorders">
              <xsl:with-param name="page" select="first"/>
            </xsl:apply-templates>
            <xsl:apply-templates select=".//w:sectPr/w:cols"/>
            <xsl:apply-templates select=".//w:sectPr/w:vAlign"/>
          </fo:region-body>
          <fo:region-before region-name="first-page-header" extent="{$default-header-extent}">
            <xsl:apply-templates select=".//w:sectPr/w:pgMar">
              <xsl:with-param name="area" select="'static-content'"/>
            </xsl:apply-templates>
          </fo:region-before>
           <fo:region-after region-name="first-page-footer" extent="{$default-footer-extent}" display-align="after">
            <xsl:apply-templates select=".//w:sectPr/w:pgMar">
              <xsl:with-param name="area" select="'static-content'"/>
            </xsl:apply-templates>
          </fo:region-after>
        </fo:simple-page-master>
        
        <fo:simple-page-master master-name="{$odd-page-name}">
          <xsl:apply-templates select=".//w:sectPr/w:pgSz"/>
          <xsl:apply-templates select=".//w:sectPr/w:pgMar">
            <xsl:with-param name="area" select="'page-master'"/>
          </xsl:apply-templates>
          
          <fo:region-body>
            <xsl:apply-templates select=".//w:sectPr/w:pgMar">
              <xsl:with-param name="area" select="'region-body'"/>
            </xsl:apply-templates>
            <xsl:apply-templates select=".//w:sectPr/w:pgBorders"/>
            <xsl:apply-templates select=".//w:sectPr/w:cols"/>
            <xsl:apply-templates select=".//w:sectPr/w:vAlign"/>
          </fo:region-body>
          <fo:region-before region-name="odd-page-header" extent="{$default-header-extent}">
            <xsl:apply-templates select=".//w:sectPr/w:pgMar">
              <xsl:with-param name="area" select="'static-content'"/>
            </xsl:apply-templates>
          </fo:region-before>
          <fo:region-after region-name="odd-page-footer" extent="{$default-footer-extent}" display-align="after">
            <xsl:apply-templates select=".//w:sectPr/w:pgMar">
              <xsl:with-param name="area" select="'static-content'"/>
            </xsl:apply-templates>
          </fo:region-after>
        </fo:simple-page-master>
        
        <fo:simple-page-master master-name="{$even-page-name}">
          <xsl:apply-templates select=".//w:sectPr/w:pgSz"/>
          <xsl:apply-templates select=".//w:sectPr/w:pgMar">
            <xsl:with-param name="area" select="'page-master'"/>
          </xsl:apply-templates>
          
          <fo:region-body>
            <xsl:apply-templates select=".//w:sectPr/w:pgMar">
              <xsl:with-param name="area" select="'region-body'"/>
            </xsl:apply-templates>
            <xsl:apply-templates select=".//w:sectPr/w:pgBorders"/>
            <xsl:apply-templates select=".//w:sectPr/w:cols"/>
            <xsl:apply-templates select=".//w:sectPr/w:vAlign"/>
          </fo:region-body>
          <fo:region-before region-name="even-page-header" extent="{$default-header-extent}">
            <xsl:apply-templates select=".//w:sectPr/w:pgMar">
              <xsl:with-param name="area" select="'static-content'"/>
            </xsl:apply-templates>
          </fo:region-before>
          <fo:region-after region-name="even-page-footer" extent="{$default-footer-extent}" display-align="after">
            <xsl:apply-templates select=".//w:sectPr/w:pgMar">
              <xsl:with-param name="area" select="'static-content'"/>
            </xsl:apply-templates>
          </fo:region-after>
        </fo:simple-page-master>
        
        <!-- create a page-sequence master -->
        <fo:page-sequence-master master-name="{$page-sequence-master-name}">
          <xsl:if test=".//w:sectPr/w:titlePg[not(@w:val='off')]">
            <fo:single-page-master-reference master-reference="{$first-page-name}"/>
          </xsl:if>
          <fo:repeatable-page-master-alternatives>
            <fo:conditional-page-master-reference odd-or-even="odd" master-reference="{$odd-page-name}"/>
            <fo:conditional-page-master-reference odd-or-even="even" master-reference="{$even-page-name}"/>
          </fo:repeatable-page-master-alternatives>
        </fo:page-sequence-master>
        
      </xsl:for-each>
    </fo:layout-master-set>
  </xsl:template>
  
  
  <!-- ======================== -->
  <!-- Generate page sequences. -->
  <!-- ======================== -->
  <xsl:template name="GeneratePageSequences">
    <xsl:variable name="main-sections" select="w:body/wx:sect[not(preceding-sibling::wx:sect) or not(.//w:sectPr/w:type/@w:val='continuous')]"/>
    <!-- ==================================================== -->
    <!-- Apply page sequences for all non-continuous sections -->
    <!-- ==================================================== -->
    <xsl:for-each select="$main-sections">
      <xsl:variable name="next-main-sect-pos" select="position() + 1"/>
      <xsl:variable name="next-main-section" select="$main-sections[position()=$next-main-sect-pos]"/>
      <xsl:variable name="number-of-continuous-sections">
        <xsl:choose>
          <xsl:when test="$next-main-section">
            <xsl:value-of select="count($next-main-section/preceding-sibling::wx:sect) - count(preceding-sibling::wx:sect)"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="count(../wx:sect)"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      
      <xsl:variable name="current-position" select="count(preceding-sibling::wx:sect) + 1"/>
      <xsl:variable name="page-sequence-master-name" select="concat('section',$current-position,'-page-sequence-master')"/>
      <fo:page-sequence master-reference="{$page-sequence-master-name}">
        <!-- Set format attribute -->
        <xsl:variable name="instruction" select="normalize-space(.//w:fldSimple[1]/@w:instr[contains(., 'PAGE')])"/>
        <xsl:attribute name="format">
          <xsl:call-template name="PageNumberFormat">
            <xsl:with-param name="format" select="substring-before(substring-after($instruction, '\* '), ' ')"/>
          </xsl:call-template>
        </xsl:attribute>
        <!-- =================== -->
        <!-- Headers and Footers -->
        <!-- =================== -->
        <fo:static-content flow-name="first-page-header">
          <fo:retrieve-marker retrieve-class-name="first-page-header" retrieve-position="first-including-carryover" retrieve-boundary="page"/>
        </fo:static-content>
        <fo:static-content flow-name="first-page-footer">
          <fo:retrieve-marker retrieve-class-name="first-page-footer" retrieve-position="first-including-carryover" retrieve-boundary="page"/>
        </fo:static-content>
        <fo:static-content flow-name="odd-page-header">
          <fo:retrieve-marker retrieve-class-name="odd-page-header" retrieve-position="first-including-carryover" retrieve-boundary="page"/>
        </fo:static-content>
        <fo:static-content flow-name="odd-page-footer">
          <fo:retrieve-marker retrieve-class-name="odd-page-footer" retrieve-position="first-including-carryover" retrieve-boundary="page"/>
        </fo:static-content>
        <fo:static-content flow-name="even-page-header">
          <xsl:choose>
            <xsl:when test="ancestor::w:wordDocument/w:docPr/w:evenAndOddHeaders[not(@w:val='off')]">
              <fo:retrieve-marker retrieve-class-name="even-page-header" retrieve-position="first-including-carryover" retrieve-boundary="page"/>
            </xsl:when>
            <xsl:otherwise>
              <fo:retrieve-marker retrieve-class-name="odd-page-header" retrieve-position="first-including-carryover" retrieve-boundary="page"/>
            </xsl:otherwise>
          </xsl:choose>
        </fo:static-content>
        <fo:static-content flow-name="even-page-footer">
          <xsl:choose>
            <xsl:when test="ancestor::w:wordDocument/w:docPr/w:evenAndOddHeaders[not(@w:val='off')]">
              <fo:retrieve-marker retrieve-class-name="even-page-footer" retrieve-position="first-including-carryover" retrieve-boundary="page"/>
            </xsl:when>
            <xsl:otherwise>
              <fo:retrieve-marker retrieve-class-name="odd-page-footer" retrieve-position="first-including-carryover" retrieve-boundary="page"/>
            </xsl:otherwise>
          </xsl:choose>
        </fo:static-content>
        <!-- ========= -->
        <!-- Footnotes -->
        <!-- ========= -->
        <fo:static-content flow-name="xsl-footnote-separator">
          <fo:block>
             <fo:leader leader-pattern="rule"
                        leader-length="144pt"
                        rule-thickness="0.5pt"
                        rule-style="solid"
                        color="gray"/>
           </fo:block>
        </fo:static-content>
        <!-- =================================================================== -->
        <!-- Text flow is produced by applying templates to the wx:sect element: -->
        <!-- see templates under "DOCUMENT'S CONTENT"                            -->
        <!-- =================================================================== -->
        <fo:flow flow-name="xsl-region-body">
          <!-- Display the document's content -->
          <xsl:apply-templates select=".|following-sibling::wx:sect[position() &lt; $number-of-continuous-sections]" />
          
          <xsl:if test="position()=last()">
            <!-- Generate an empty block at the end of the FO document. -->
            <fo:block id="{generate-id(/)}"/>
          </xsl:if>
        </fo:flow>
        
      </fo:page-sequence>
    </xsl:for-each>
  </xsl:template>


  <!-- ====================== -->
  <!-- Display static content -->
  <!-- ====================== -->
  <xsl:template name="DisplayStaticContent">
    <xsl:param name="section"/>
    <xsl:param name="hdr-or-ftr"/>
    <xsl:param name="type"/>
    
    <xsl:choose>
      <xsl:when test="$hdr-or-ftr='header' and $section//w:sectPr/w:hdr[@w:type=$type]">
        <xsl:apply-templates select="$section//w:sectPr/w:hdr[@w:type=$type]"/>
      </xsl:when>
      <xsl:when test="$hdr-or-ftr='footer' and $section//w:sectPr/w:ftr[@w:type=$type]">
        <xsl:apply-templates select="$section//w:sectPr/w:ftr[@w:type=$type]"/>
      </xsl:when>
      <xsl:when test="$section">
        <xsl:call-template name="DisplayStaticContent">
          <xsl:with-param name="section" select="$section/preceding-sibling::wx:sect[1]"/>
          <xsl:with-param name="hdr-or-ftr" select="$hdr-or-ftr"/>
          <xsl:with-param name="type" select="$type"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise/>
    </xsl:choose>
  </xsl:template>

</xsl:stylesheet>
