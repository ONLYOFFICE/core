<?xml version="1.0" encoding="UTF-8"?>

<!-- ===================================================================

This stylesheet controls translation of properties. It also provides
mechanisms for property overrides at different levels, for style
derivation, and for property conflict resolution.

************************************************************************

IMPLEMENTATION SPECIFICS

1. When displaying its own contents, MS Word always applies a default
style on every document component even though there is a specific style
defined on the component. So, the default style is always applied on
components in the stylesheet.

2. 'MS Office 2003 XML Reference Schemas' gives the following definition
of 'w:space' attribute: 'gets or sets the border space in eighths of
a point.' But MS Word sets the attribute in points when creating/saving
XML document. So, a value of the attribute is considered as set
in points in the stylesheet.

3. If line spacing is specified as a multiplier in the input document,
MS Word saves the explicit value that doesn't depend on font size of 
the text. The stylesheet implements an algorithm to resolve the issue.

==================================================================== -->


<xsl:stylesheet xmlns:fo="http://www.w3.org/1999/XSL/Format"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
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


  <!-- =================== -->
  <!-- Document properties -->
  <!-- =================== -->
      <!-- ================================================ -->
      <!-- Specifies the size and orientation of this page. -->
      <!-- Parent elements: w:sectPr                        -->
      <!-- ================================================ -->
      <xsl:template match="w:pgSz">
        <xsl:attribute name="page-width">
          <xsl:value-of select="@w:w div 1440"/><xsl:text>in</xsl:text>
        </xsl:attribute>
        <xsl:attribute name="page-height">
          <xsl:value-of select="@w:h div 1440"/><xsl:text>in</xsl:text>
        </xsl:attribute>
      </xsl:template>



  <!-- ================== -->
  <!-- Section properties -->
  <!-- ================== -->
      <!-- =========================== -->
      <!-- Specifies the page margins. -->
      <!-- Parent elements: w:sectPr   -->
      <!-- =========================== -->
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
            <xsl:if test="@w:header">
              <xsl:attribute name="margin-top">
                <xsl:value-of select="@w:header div 20"/><xsl:text>pt</xsl:text>
              </xsl:attribute>
            </xsl:if>
            <xsl:if test="@w:footer">
              <xsl:attribute name="margin-bottom">
                <xsl:value-of select="@w:footer div 20"/><xsl:text>pt</xsl:text>
              </xsl:attribute>
            </xsl:if>
            <xsl:if test="@w:right">
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
                        <xsl:value-of select="(@w:right div 20) - ($border-right/@w:sz div 8) - ($border-right/@w:space)"/>
                        <xsl:text>pt</xsl:text>
                      </xsl:otherwise>
                    </xsl:choose>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="@w:right div 20"/><xsl:text>pt</xsl:text>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:attribute>
            </xsl:if>
            <xsl:if test="@w:left">
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
                        <xsl:value-of select="(@w:left div 20) - ($border-left/@w:sz div 8) - ($border-left/@w:space)"/>
                        <xsl:text>pt</xsl:text>
                      </xsl:otherwise>
                    </xsl:choose>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="@w:left div 20"/><xsl:text>pt</xsl:text>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:attribute>
            </xsl:if>
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
                        <xsl:value-of select="$border-left/@w:space"/><xsl:text>pt</xsl:text>
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
                        <xsl:value-of select="$border-right/@w:space"/><xsl:text>pt</xsl:text>
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
                        <xsl:value-of select="(@w:top - @w:header) div 20"/><xsl:text>pt</xsl:text>
                      </xsl:when>
                      <xsl:otherwise>
                        <xsl:value-of select="@w:top div 20"/><xsl:text>pt</xsl:text>
                      </xsl:otherwise>
                    </xsl:choose>
                  </xsl:attribute>
                </xsl:if>
                <xsl:if test="@w:bottom">
                  <xsl:attribute name="margin-bottom">
                    <xsl:choose>
                      <xsl:when test="@w:footer">
                        <xsl:value-of select="(@w:bottom - @w:footer) div 20"/><xsl:text>pt</xsl:text>
                      </xsl:when>
                      <xsl:otherwise>
                        <xsl:value-of select="@w:bottom div 20"/><xsl:text>pt</xsl:text>
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
                      <xsl:value-of select="$border-left/@w:space"/><xsl:text>pt</xsl:text>
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
                      <xsl:value-of select="$border-right/@w:space"/><xsl:text>pt</xsl:text>
                    </xsl:otherwise>
                  </xsl:choose>
                </xsl:attribute>
              </xsl:if>
            </xsl:if>
          </xsl:when>
          <xsl:otherwise/>
        </xsl:choose>
      </xsl:template>
      
      <!-- =========================== -->
      <!-- Specifies the page borders. -->
      <!-- Parent elements: w:sectPr   -->
      <!-- =========================== -->
      <xsl:template match="w:pgBorders">
        <xsl:param name="page" select="not-first"/>
        
        <xsl:choose>
          <xsl:when test="@w:display='first-page'">
            <xsl:if test="$page='first'">
              <xsl:apply-templates select="*"/>
            </xsl:if>
          </xsl:when>
          <xsl:when test="@w:display='not-first-page'">
            <xsl:if test="not($page='first')">
              <xsl:apply-templates select="*"/>
            </xsl:if>
          </xsl:when>
          <xsl:otherwise>
            <xsl:apply-templates select="*"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:template>
      
      <!-- ================================= -->
      <!-- Sets vertical alignment for text. -->
      <!-- Parent elements: w:sectPr, w:tcPr -->
      <!-- ================================= -->
      <xsl:template match="w:vAlign">
        <xsl:attribute name="display-align">
          <xsl:choose>
            <xsl:when test="@w:val='top'"><xsl:text>before</xsl:text></xsl:when>
            <xsl:when test="@w:val='center'"><xsl:text>center</xsl:text></xsl:when>
            <xsl:when test="@w:val='bottom'"><xsl:text>after</xsl:text></xsl:when>
            <xsl:otherwise><xsl:text>auto</xsl:text></xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
      </xsl:template>
      
      <!-- ================================================= -->
      <!-- Specifies the column properties for this section. -->
      <!-- Parent elements: w:sectPr                         -->
      <!-- Note: XSL-FO can support the columns of the same  -->
      <!--       width only.                                 -->
      <!-- ================================================= -->
      <xsl:template match="w:cols">
        <xsl:if test="@w:num">
          <xsl:attribute name="column-count">
            <xsl:value-of select="@w:num"/>
          </xsl:attribute>
          <xsl:attribute name="column-gap">
            <xsl:value-of select="@w:space div 20"/><xsl:text>pt</xsl:text>
          </xsl:attribute>
        </xsl:if>
      </xsl:template>



  <!-- ====================== -->
  <!-- Block-level properties -->
  <!-- ====================== -->
      <!-- ============================= -->
      <!-- Apply block-level properties. -->
      <!-- ============================= -->
      <xsl:template name="ApplyParaProperties">
        <!-- Set p properties for the paragraph.               -->
        <!-- (i.e. set block-level attributes on the fo:block) -->
          <!-- default paragraph style -->
          <xsl:apply-templates select="$default-paragraph-style"/>
          <!-- style-level (pStyle)-->
          <xsl:apply-templates select="key('styles', w:pPr/w:pStyle/@w:val)[@w:type='paragraph']"/>
           <!-- p-level (direct; override style-level) -->
          <xsl:apply-templates select="w:pPr/*[not(self::w:rPr) and not(self::w:pStyle) and not(self::w:listPr) and not(self::w:sectPr)]"/>
          
          <xsl:if test="parent::wx:pBdrGroup">
            <!-- set space-before=0 for the first paragraph within pBdrGroup -->
            <xsl:if test="not(preceding-sibling::w:p)">
              <xsl:attribute name="space-before">
                <xsl:text>0pt</xsl:text>
              </xsl:attribute>
            </xsl:if>
            <!-- set space-after=0 for the last paragraph within pBdrGroup -->
            <xsl:if test="not(following-sibling::w:p)">
              <xsl:attribute name="space-after">
                <xsl:text>0pt</xsl:text>
              </xsl:attribute>
            </xsl:if>
          </xsl:if>

        <!-- Set r properties for the paragraph mark. -->
        <!-- (i.e. set inline attributes on the fo:block)   -->
          <!-- Currently, MS Word 2003 ignores style-level  -->
          <!-- r properties applied at the paragraph level. -->
            <!-- style-level (rStyle) -->
            <!--<xsl:apply-templates select="key('styles', w:pPr/w:rPr/w:rStyle/@w:val)"/>-->
          
          <!-- MS Word collates inline-level properties of all run   -->
          <!-- within the paragraph when saving the document as XML. -->
          <!-- Apply direct run properties at paragraph level! -->
          <xsl:apply-templates select="w:pPr/w:rPr/*[not(self::w:rStyle)]"/>
      </xsl:template>

      <!-- ================================================== -->
      <!-- Represents text alignment.                         -->
      <!-- Parent elements: w:pPr, w:tblPr, w:tblprEx, w:trPr -->
      <!-- ================================================== -->
      <xsl:template match="w:jc">
        <xsl:attribute name="text-align">
          <xsl:choose>
            <xsl:when test="@w:val='left'"><xsl:text>left</xsl:text></xsl:when>
            <xsl:when test="@w:val='right'"><xsl:text>right</xsl:text></xsl:when>
            <xsl:when test="@w:val='center'"><xsl:text>center</xsl:text></xsl:when>
            <xsl:when test="@w:val='both' or @w:val='distribute'">
              <xsl:text>justify</xsl:text>
            </xsl:when>
            <xsl:otherwise><xsl:text>inherit</xsl:text></xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
        <xsl:if test="@w:val='distribute'">
          <xsl:attribute name="text-align-last"><xsl:text>justify</xsl:text></xsl:attribute>
        </xsl:if>
      </xsl:template>
      
      <!-- ========================================== -->
      <!-- Represents Page Break Before option:       -->
      <!-- forces a page break before this paragraph. -->
      <!-- Parent elements: w:pPr                     -->
      <!-- ========================================== -->
      <xsl:template match="w:pageBreakBefore">
        <xsl:attribute name="break-before">
          <xsl:choose>
            <xsl:when test="@w:val='off'"><xsl:text>auto</xsl:text></xsl:when>
            <xsl:otherwise><xsl:text>page</xsl:text></xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
      </xsl:template>
      
      <!-- ======================================== -->
      <!-- Represents Keep Lines Together option:   -->
      <!-- prevents a page break in this paragraph. -->
      <!-- Parent elements: w:pPr                   -->
      <!-- ======================================== -->
      <xsl:template match="w:keepLines">
        <xsl:attribute name="keep-together.within-page">
          <xsl:choose>
            <xsl:when test="@w:val='off'"><xsl:text>auto</xsl:text></xsl:when>
            <xsl:otherwise><xsl:text>always</xsl:text></xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
      </xsl:template>
      
      <!-- ============================================ -->
      <!-- Represents Keep with Next Paragraph option:  -->
      <!-- prevents a page break between this paragraph -->
      <!-- and the next.                                -->
      <!-- Parent elements: w:pPr                       -->
      <!-- ============================================ -->
      <xsl:template match="w:keepNext">
        <xsl:attribute name="keep-with-next.within-page">
          <xsl:choose>
            <xsl:when test="@w:val='off'"><xsl:text>auto</xsl:text></xsl:when>
            <xsl:otherwise><xsl:text>always</xsl:text></xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
      </xsl:template>
      
      <!-- ============================================ -->
      <!-- Represents Widow/Orphan Control option:      -->
      <!-- prevents Word from printing the last line of -->
      <!-- a paragraph by itself at the top of the page -->
      <!-- (widow) or the first line of a paragraph at  -->
      <!-- the bottom of a page (orphan).               -->
      <!-- Parent elements: w:pPr                       -->
      <!-- ============================================ -->
      <xsl:template match="w:widowControl">
        <xsl:choose>
          <xsl:when test="@w:val='off'">
            <xsl:attribute name="widows"><xsl:text>1</xsl:text></xsl:attribute>
            <xsl:attribute name="orphans"><xsl:text>1</xsl:text></xsl:attribute>
          </xsl:when>
          <xsl:otherwise>
            <xsl:attribute name="widows"><xsl:text>2</xsl:text></xsl:attribute>
            <xsl:attribute name="orphans"><xsl:text>2</xsl:text></xsl:attribute>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:template>
      
      <!-- ======================================================= -->
      <!-- The element represents two different set of properties: -->
      <!-- 1) spacing between lines and paragraphs.                -->
      <!-- Parent elements: w:pPr                                  -->
      <!-- 2) the amount by which the spacing between              -->
      <!--    characters is expanded or condensed.                 -->
      <!-- Parent elements: w:rPr                                  -->
      <!-- ======================================================= -->
      <xsl:template match="w:spacing">
        <xsl:param name="mode"/>
      <!-- block-level properties:                 -->
      <!-- space-before/space-after or line-height -->
      <!-- Parent element: w:pPr.                  -->
        <xsl:if test="@w:before and not($mode='after')">
          <xsl:attribute name="space-before"><xsl:value-of select="@w:before div 20"/><xsl:text>pt</xsl:text></xsl:attribute>
          <xsl:if test="not(@w:before-autospacing) or @w:before-autospacing='off'">
            <xsl:attribute name="space-before.conditionality"><xsl:text>retain</xsl:text></xsl:attribute>
          </xsl:if>
        </xsl:if>
        <xsl:if test="@w:before-lines and not($mode='after')">
          <xsl:attribute name="space-before"><xsl:value-of select="@w:before-lines *.12"/><xsl:text>pt</xsl:text></xsl:attribute>
          <xsl:if test="not(@w:before-autospacing) or @w:before-autospacing='off'">
            <xsl:attribute name="space-before.conditionality"><xsl:text>retain</xsl:text></xsl:attribute>
          </xsl:if>
        </xsl:if>
        <xsl:if test="@w:after and not($mode='before')">
          <xsl:attribute name="space-after"><xsl:value-of select="@w:after div 20"/><xsl:text>pt</xsl:text></xsl:attribute>
          <xsl:if test="not(@w:after-autospacing) or @w:before-autospacing='off'">
            <xsl:attribute name="space-after.conditionality"><xsl:text>retain</xsl:text></xsl:attribute>
          </xsl:if>
        </xsl:if>
        <xsl:if test="@w:after-lines and not($mode='before')">
          <xsl:attribute name="space-after"><xsl:value-of select="@w:after-lines *.12"/><xsl:text>pt</xsl:text></xsl:attribute>
          <xsl:if test="not(@w:after-autospacing) or @w:before-autospacing='off'">
            <xsl:attribute name="space-after.conditionality"><xsl:text>retain</xsl:text></xsl:attribute>
          </xsl:if>
        </xsl:if>
        <!-- line-height -->
        <xsl:if test="@w:line and not($mode='before') and not($mode='after')">
          <xsl:choose>
            <xsl:when test="@w:line-rule='at-least'">
              <xsl:attribute name="line-height.minimum">
                <xsl:value-of select="@w:line div 20"/>
                <xsl:text>pt</xsl:text>
              </xsl:attribute>
            </xsl:when>
            <xsl:when test="@w:line-rule='exact'">
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
                <xsl:value-of select="((@w:line div 20) div 12) * $default-line-height"/>
                <!--<xsl:text>pt</xsl:text>-->
              </xsl:attribute>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:if>
      <!-- inline-level property: letter-spacing -->
      <!-- Parent element: w:rPr.                -->
        <xsl:if test="@w:val">
          <xsl:attribute name="letter-spacing">
            <xsl:value-of select="@w:val div 20"/><xsl:text>pt</xsl:text>
          </xsl:attribute>
        </xsl:if>
      </xsl:template>
      
      
      <!-- ================================= -->
      <!-- Represents paragraph indentation. -->
      <!-- Parent elements: w:pPr            -->
      <!-- ================================= -->
      <xsl:template match="w:ind">
        <!-- start-indent/end-indent -->
        <xsl:if test="@w:left">
          <xsl:attribute name="start-indent"><xsl:value-of select="@w:left div 20"/><xsl:text>pt</xsl:text></xsl:attribute>
        </xsl:if>
        <xsl:if test="@w:left-chars">
          <xsl:attribute name="start-indent"><xsl:value-of select="@w:left-chars *.12"/><xsl:text>pt</xsl:text></xsl:attribute>
        </xsl:if>
        <xsl:if test="@w:right">
          <xsl:attribute name="end-indent"><xsl:value-of select="@w:right div 20"/><xsl:text>pt</xsl:text></xsl:attribute>
        </xsl:if>
        <xsl:if test="@w:right-chars">
          <xsl:attribute name="end-indent"><xsl:value-of select="@w:right-chars *.12"/><xsl:text>pt</xsl:text></xsl:attribute>
        </xsl:if>
        <!-- text-indent -->
        <xsl:if test="@w:first-line">
          <xsl:attribute name="text-indent"><xsl:value-of select="@w:first-line div 20"/><xsl:text>pt</xsl:text></xsl:attribute>
        </xsl:if>
        <xsl:if test="@w:first-line-chars">
          <xsl:attribute name="text-indent"><xsl:value-of select="@w:first-line-chars *.12"/><xsl:text>pt</xsl:text></xsl:attribute>
        </xsl:if>
        <xsl:if test="@w:hanging">
          <xsl:attribute name="text-indent"><xsl:value-of select="- @w:hanging div 20"/><xsl:text>pt</xsl:text></xsl:attribute>
        </xsl:if>
        <xsl:if test="@w:hanging-chars">
          <xsl:attribute name="text-indent"><xsl:value-of select="- @w:hanging-chars *.12"/><xsl:text>pt</xsl:text></xsl:attribute>
        </xsl:if>
      </xsl:template>



  <!-- ================= -->
  <!-- Inline properties -->
  <!-- ================= -->
      <!-- ============================= -->
      <!-- Apply inline-level properties -->
      <!-- ============================= -->
      <xsl:template name="ApplyRunProperties">
        <xsl:variable name="runStyle" select="key('styles', w:rPr/w:rStyle/@w:val)[@w:type='character']/w:rPr/*"/>
        <xsl:variable name="paraStyleR" select="key('styles', ancestor::p/w:pPr/w:pStyle/@w:val)[@w:type='paragraph']/w:rPr/*"/>
        <!-- default style -->
        <xsl:apply-templates select="$default-character-style"/>
        <!-- style-level -->
        <xsl:for-each select="$runStyle">
          <xsl:if test="not($paraStyleR[name()=name(current()) and current()/@w:val='off'])">
            <xsl:apply-templates select="."/>
          </xsl:if>
        </xsl:for-each>
        <!-- direct properties (override style-level) -->
        <xsl:apply-templates select="w:rPr/*[not(self::w:rStyle)]"/>
      </xsl:template>
      
      <!-- ================================== -->
      <!-- Represents the fonts for this run. -->
      <!-- Parent elements: w:rPr             -->
      <!-- ================================== -->
      <xsl:template match="w:rFonts">
        <xsl:choose>
          <xsl:when test="@w:ascii">
            <xsl:attribute name="font-family">
              <xsl:variable name="alt-font-name">
                <xsl:call-template name="AltFontName">
                  <xsl:with-param name="font-name" select="@w:ascii"/>
                </xsl:call-template>
              </xsl:variable>
              <xsl:call-template name="NormalizeFontFamilyName">
                <xsl:with-param name="font-family" select="$alt-font-name"/>
              </xsl:call-template>
            </xsl:attribute>
          </xsl:when>
          <xsl:otherwise/>
        </xsl:choose>
      </xsl:template>
      
      <!-- ================================ -->
      <!-- Represents the font-family name. -->
      <!-- ================================ -->
      <xsl:template match="wx:font">
        <xsl:attribute name="font-family">
          <xsl:variable name="alt-font-name">
            <xsl:call-template name="AltFontName">
              <xsl:with-param name="font-name" select="@wx:val"/>
            </xsl:call-template>
          </xsl:variable>
          <xsl:call-template name="NormalizeFontFamilyName">
            <xsl:with-param name="font-family" select="$alt-font-name"/>
          </xsl:call-template>
        </xsl:attribute>
      </xsl:template>
      
      <!-- ========================================= -->
      <!-- Formats lowercase text as capital letters -->
      <!-- and reduces their size                    -->
      <!-- Parent elements: w:rPr                    -->
      <!-- ========================================= -->
      <xsl:template match="w:smallCaps">
        <xsl:attribute name="font-variant">
          <xsl:choose>
            <xsl:when test="@w:val='off'"><xsl:text>normal</xsl:text></xsl:when>
            <xsl:otherwise><xsl:text>small-caps</xsl:text></xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
      </xsl:template>
      
      <!-- ====================================== -->
      <!-- Represents the font size for the Asian -->
      <!-- and Latin fonts in this run.           -->
      <!-- Parent elements: w:rPr                 -->
      <!-- ====================================== -->
      <xsl:template match="w:sz">
        <xsl:attribute name="font-size"><xsl:value-of select="@w:val div 2"/><xsl:text>pt</xsl:text></xsl:attribute>
      </xsl:template>
      
      <!-- ======================================== -->
      <!-- Sets Latin and Asian characters to bold. -->
      <!-- Parent elements: w:rPr                   -->
      <!-- ======================================== -->
      <xsl:template match="w:b">
        <xsl:attribute name="font-weight">
          <xsl:choose>
            <xsl:when test="@w:val='off'"><xsl:text>normal</xsl:text></xsl:when>
            <xsl:otherwise><xsl:text>bold</xsl:text></xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
      </xsl:template>
      
      <!-- ========================================== -->
      <!-- Sets Latin and Asian characters to italic. -->
      <!-- Parent elements: w:rPr                     -->
      <!-- ========================================== -->
      <xsl:template match="w:i">
        <xsl:attribute name="font-style">
          <xsl:choose>
            <xsl:when test="@w:val='off'"><xsl:text>normal</xsl:text></xsl:when>
            <xsl:otherwise><xsl:text>italic</xsl:text></xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
      </xsl:template>
      
      <!-- ========================================== -->
      <!-- Formats lowercase text as capital letters. -->
      <!-- Parent elements: w:rPr                     -->
      <!-- ========================================== -->
      <xsl:template match="w:caps">
        <xsl:attribute name="text-transform">
          <xsl:choose>
            <xsl:when test="@w:val='off'"><xsl:text>none</xsl:text></xsl:when>
            <xsl:otherwise><xsl:text>uppercase</xsl:text></xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
      </xsl:template>
      
      <!--  ============================= -->
      <!-- Draws a line through the text. -->
      <!-- Parent elements: w:rPr         -->
      <!-- ============================== -->
      <xsl:template match="w:strike">
        <xsl:attribute name="text-decoration">
          <xsl:choose>
            <xsl:when test="@w:val='off'"><xsl:text>none</xsl:text></xsl:when>
            <xsl:otherwise><xsl:text>line-through</xsl:text></xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
      </xsl:template>
      
      <!--  ==================================== -->
      <!-- Draws a double line through the text. -->
      <!-- Parent elements: w:rPr                -->
      <!-- ===================================== -->
      <xsl:template match="w:dstrike">
        <xsl:attribute name="text-decoration">
          <xsl:choose>
            <xsl:when test="@w:val='off'"><xsl:text>none</xsl:text></xsl:when>
            <xsl:otherwise><xsl:text>line-through</xsl:text></xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
      </xsl:template>
      
      <!--  ================================================ -->
      <!-- Represents the underline formatting for this run. -->
      <!-- Parent elements: w:rPr                            -->
      <!-- ================================================= -->
      <xsl:template match="w:u">
        <xsl:attribute name="text-decoration">
          <xsl:choose>
            <xsl:when test="@w:val='none'"><xsl:text>none</xsl:text></xsl:when>
            <xsl:otherwise><xsl:text>underline</xsl:text></xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
      </xsl:template>
      
      <!-- ===================================================== -->
      <!-- Adjusts the vertical position of the text relative to -->
      <!-- the baseline and changes the font size if possible.   -->
      <!-- To raise or lower the text without reducing           -->
      <!-- the font size, use the position element.              -->
      <!-- Parent elements: w:rPr                                -->
      <!-- ===================================================== -->
      <xsl:template match="w:vertAlign">
        <xsl:choose>
          <xsl:when test="@w:val='superscript'">
            <xsl:attribute name="baseline-shift"><xsl:text>super</xsl:text></xsl:attribute>
            <xsl:attribute name="font-size"><xsl:text>66.7%</xsl:text></xsl:attribute>
          </xsl:when>
          <xsl:when test="@w:val='subscript'">
            <xsl:attribute name="baseline-shift"><xsl:text>sub</xsl:text></xsl:attribute>
            <xsl:attribute name="font-size"><xsl:text>66.7%</xsl:text></xsl:attribute>
          </xsl:when>
          <xsl:otherwise/>
        </xsl:choose>
      </xsl:template>
      
      <!-- ============================================== -->
      <!-- Represents the amount by which text should be  -->
      <!-- raised or lowered in relation to the baseline. -->
      <!-- Parent elements: w:rPr                         -->
      <!-- ============================================== -->
      <xsl:template match="w:position">
        <xsl:attribute name="baseline-shift">
          <xsl:value-of select="@w:val div 2"/><xsl:text>pt</xsl:text>
        </xsl:attribute>
      </xsl:template>
      
      <!-- ====================================== -->
      <!-- Specifies either an automatic color or -->
      <!-- a hexadecimal color code for this run. -->
      <!-- Parent elements: w:rPr                 -->
      <!-- ====================================== -->
      <xsl:template match="w:color">
        <xsl:attribute name="color">
          <xsl:call-template name="ConvertHexColor">
            <xsl:with-param name="hexColor" select="@w:val"/>
          </xsl:call-template>
        </xsl:attribute>
      </xsl:template>
      
      <!-- ====================================== -->
      <!-- Represents the languages for this run. -->
      <!-- Parent elements: w:rPr                 -->
      <!-- ====================================== -->
      <xsl:template match="w:lang">
        <xsl:attribute name="language"><xsl:value-of select="@w:val"/></xsl:attribute>
      </xsl:template>


  <!-- ====================== -->
  <!-- Style-level properties -->
  <!-- ====================== -->
      <xsl:template match="w:style">
        <xsl:param name="specific-attribute"/>
        <xsl:param name="mode"/>
        
        <!-- Apply "parent" style properties -->
        <xsl:apply-templates select="key('styles', w:basedOn/@w:val)">
          <xsl:with-param name="specific-attribute" select="$specific-attribute"/>
          <xsl:with-param name="mode" select="$mode"/>
        </xsl:apply-templates>
        <!-- Apply paragraph, character, and table properties. -->
        <xsl:apply-templates select="w:pPr|w:rPr|w:tblPr">
          <xsl:with-param name="specific-attribute" select="$specific-attribute"/>
          <xsl:with-param name="mode" select="$mode"/>
        </xsl:apply-templates>
        <!-- table-style conditional-override properties -->
        <xsl:apply-templates select="w:tblStylePr/*[not(self::w:trPr) and not(self::w:tcPr)]"/>
      </xsl:template>
          <!-- ===================================== -->
          <!-- Style-level properties fot table-rows -->
          <!-- ===================================== -->
          <xsl:template match="w:style" mode="table-rows">
            <!-- Apply "parent" style properties -->
            <xsl:apply-templates select="key('styles', w:basedOn/@w:val)" mode="table-rows"/>
            <!-- Apply table-row properties. -->
            <xsl:apply-templates select="w:trPr"/>
            <!-- table-style conditional-override properties -->
            <xsl:apply-templates select="w:tblStylePr/w:trPr"/>
          </xsl:template>
          <!-- ====================================== -->
          <!-- Style-level properties fot table-cells -->
          <!-- ====================================== -->
          <xsl:template match="w:style" mode="table-cells">
            <!-- Apply "parent" style properties -->
            <xsl:apply-templates select="key('styles', w:basedOn/@w:val)" mode="table-cells"/>
            <!-- Apply table-cell properties. -->
              <xsl:apply-templates select="w:tblPr/w:tblCellMar | w:tblPr/w:tblBorders/w:insideH | w:tblPr/w:tblBorders/w:insideV"/>
              <xsl:apply-templates select="w:tcPr"/>
            <!-- table-style conditional-override properties -->
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
            <xsl:apply-templates select="*[not(self::w:pStyle) and not(self::w:listPr)]"/>
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

      <!-- ===================== -->
      <!-- Properties for tables -->
      <!-- ===================== -->
      <xsl:template match="w:tblPr">
        <xsl:param name="specific-attribute"/>
        <xsl:param name="mode"/>
        
        <xsl:choose>
          <xsl:when test="$specific-attribute">
            <xsl:apply-templates select="*[name()=$specific-attribute]">
              <xsl:with-param name="mode" select="$mode"/>
            </xsl:apply-templates>
          </xsl:when>
          <xsl:otherwise>
            <xsl:apply-templates select="*[not(self::w:tblStyle) and not(self::w:tblCellMar)]"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:template>

      <!-- ==================== -->
      <!-- Table-row properties -->
      <!-- ==================== -->
      <xsl:template match="w:trPr">
        <xsl:apply-templates select="*"/>
      </xsl:template>

      <!-- ===================== -->
      <!-- Table-cell properties -->
      <!-- ===================== -->
      <xsl:template match="w:tcPr">
        <xsl:apply-templates select="*"/>
      </xsl:template>


  <!-- ============================== -->
  <!-- Properties of tabular elements -->
  <!-- ============================== -->
      <!-- ========================================================== -->
      <!-- Represents the width that the table should be indented by. -->
      <!-- Parent elements: w:tblPr, w:tblPrEx                        -->
      <!-- ========================================================== -->
      <xsl:template match="w:tblInd">
        <xsl:attribute name="start-indent">
          <xsl:call-template name="ConvertTableWidthPropertyType">
            <xsl:with-param name="type" select="@w:type"/>
            <xsl:with-param name="value" select="@w:w"/>
          </xsl:call-template>
        </xsl:attribute>
      </xsl:template>

      <!-- ================================================ -->
      <!-- Represents the border definitions for the table. -->
      <!-- Parent elements: w:tblPr, w:tblPrEx              -->
      <!-- ================================================ -->
      <xsl:template match="w:tblBorders">
        <xsl:apply-templates select="w:top|w:bottom|w:left|w:right"/>
      </xsl:template>
      
      <!-- =========================================================== -->
      <!-- Represents the cell margin defaults for this table's cells. -->
      <!-- Parent elements: w:tblPr                                    -->
      <!-- =========================================================== -->
      <xsl:template match="w:tblCellMar">
        <xsl:apply-templates select="*" mode="paddings"/>
      </xsl:template>
      
      <!-- ===================================================== -->
      <!-- Defines the borders for this cell - these definitions -->
      <!-- override the definitions given by the table borders.  -->
      <!-- Parent elements: w:tcPr                               -->
      <!-- ===================================================== -->
      <xsl:template match="w:tcBorders">
        <xsl:apply-templates select="*"/>
      </xsl:template>
      
      <!-- =================================================== -->
      <!-- Represents the horizontal border between two cells. -->
      <!-- Parent elements: w:tblBorders, w:tcBorders          -->
      <!-- =================================================== -->
      <xsl:template match="w:insideH">
        <xsl:call-template name="SetBorderStyle">
          <xsl:with-param name="attribute-name" select="'border-left-style'"/>
          <xsl:with-param name="style" select="@w:val"/>
        </xsl:call-template>
        <xsl:call-template name="SetBorderStyle">
          <xsl:with-param name="attribute-name" select="'border-right-style'"/>
          <xsl:with-param name="style" select="@w:val"/>
        </xsl:call-template>
        <xsl:if test="@w:color">
          <xsl:attribute name="border-left-color">
            <xsl:call-template name="ConvertHexColor">
              <xsl:with-param name="hexColor" select="@w:color"/>
              <xsl:with-param name="style" select="@w:val"/>
            </xsl:call-template>
          </xsl:attribute>
          <xsl:attribute name="border-right-color">
            <xsl:call-template name="ConvertHexColor">
              <xsl:with-param name="hexColor" select="@w:color"/>
              <xsl:with-param name="style" select="@w:val"/>
            </xsl:call-template>
          </xsl:attribute>
        </xsl:if>
        <xsl:if test="@w:sz">
          <!-- set half border-width on the cell -->
          <xsl:attribute name="border-left-width">
            <xsl:value-of select="@w:sz div 16"/><xsl:text>pt</xsl:text>
          </xsl:attribute>
          <!-- set half border-width on the cell -->
          <xsl:attribute name="border-right-width">
            <xsl:value-of select="@w:sz div 16"/><xsl:text>pt</xsl:text>
          </xsl:attribute>
        </xsl:if>
      </xsl:template>

      <!-- ================================================= -->
      <!-- Represents the vertical border between two cells. -->
      <!-- Parent elements: w:tblBorders, w:tcBorders        -->
      <!-- ================================================= -->
      <xsl:template match="w:insideV">
        <xsl:call-template name="SetBorderStyle">
          <xsl:with-param name="attribute-name" select="'border-top-style'"/>
          <xsl:with-param name="style" select="@w:val"/>
        </xsl:call-template>
        <xsl:call-template name="SetBorderStyle">
          <xsl:with-param name="attribute-name" select="'border-bottom-style'"/>
          <xsl:with-param name="style" select="@w:val"/>
        </xsl:call-template>
        <xsl:if test="@w:color">
          <xsl:attribute name="border-top-color">
            <xsl:call-template name="ConvertHexColor">
              <xsl:with-param name="hexColor" select="@w:color"/>
              <xsl:with-param name="style" select="@w:val"/>
            </xsl:call-template>
          </xsl:attribute>
          <xsl:attribute name="border-bottom-color">
            <xsl:call-template name="ConvertHexColor">
              <xsl:with-param name="hexColor" select="@w:color"/>
              <xsl:with-param name="style" select="@w:val"/>
            </xsl:call-template>
          </xsl:attribute>
        </xsl:if>
        <xsl:if test="@w:sz">
          <!-- set half border-width on the cell -->
          <xsl:attribute name="border-top-width">
            <xsl:value-of select="@w:sz div 16"/><xsl:text>pt</xsl:text>
          </xsl:attribute>
          <!-- set half border-width on the cell -->
          <xsl:attribute name="border-bottom-width">
            <xsl:value-of select="@w:sz div 16"/><xsl:text>pt</xsl:text>
          </xsl:attribute>
        </xsl:if>
      </xsl:template>
      
      <!-- ======================================================= -->
      <!-- Represents the number of grid units this cell consumes. -->
      <!-- Parent elements: w:tcPr                                 -->
      <!-- ======================================================= -->
      <xsl:template match="w:gridSpan">
        <xsl:attribute name="number-columns-spanned">
          <xsl:value-of select="@w:val"/>
        </xsl:attribute>
      </xsl:template>
      
      <!-- =================================================== -->
      <!-- Specifies whether this cell is part of              -->
      <!-- (or the beginning of) a horizontally merged region. -->
      <!-- Parent elements: w:tcPr                             -->
      <!-- =================================================== -->
      <xsl:template match="w:hmerge">
        <xsl:if test="@w:val='restart'">
          <xsl:call-template name="CountCellsSpanned">
            <xsl:with-param name="number-cells-spanned" select="1"/>
            <xsl:with-param name="following-cell" select="ancestor::w:tc[1]/following-sibling::w:tc[1]"/>
            <xsl:with-param name="col-or-row" select="'col'"/>
          </xsl:call-template>
        </xsl:if>
      </xsl:template>
      
      <!-- ================================================= -->
      <!-- Specifies whether this cell is part of            -->
      <!-- (or the beginning of) a vertically merged region. -->
      <!-- Parent elements: w:tcPr                           -->
      <!-- ================================================= -->
      <xsl:template match="w:vmerge">
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


  <!-- ========================================== -->
  <!-- Border, Padding, and Background properties -->
  <!-- ========================================== -->
        <!-- ===================================== -->
        <!-- Represents borders for the paragraph. -->
        <!-- Parent elements: w:pPr                -->
        <!-- ===================================== -->
          <!-- Borders are drawn at the wx:pBdrGroup element -->
          <xsl:template match="w:pBdr"/>
        
        <!-- ================================================ -->
        <!-- Represents top, bottom, left, and right borders. -->
        <!-- Parent elements: w:divBdr, w:pgBorders, w:pBdr,  -->
        <!--                  w:tblBorders, w:tcBorders       -->
        <!-- ================================================ -->
        <xsl:template match="w:top|w:bottom|w:left|w:right">
          <xsl:variable name="side">
            <xsl:choose>
              <xsl:when test="local-name()='bottom'">
                <xsl:text>after</xsl:text>
              </xsl:when>
              <xsl:when test="local-name()='top'">
                <xsl:text>before</xsl:text>
              </xsl:when>
              <xsl:otherwise><xsl:value-of select="local-name()"/></xsl:otherwise>
            </xsl:choose>
          </xsl:variable>
          <xsl:variable name="style-attribute-name" select="concat('border-',local-name(),'-style')"/>
          <xsl:call-template name="SetBorderStyle">
            <xsl:with-param name="attribute-name" select="$style-attribute-name"/>
            <xsl:with-param name="style" select="@w:val"/>
          </xsl:call-template>
          <xsl:if test="@w:color">
            <xsl:attribute name="{concat('border-',local-name(),'-color')}">
              <xsl:call-template name="ConvertHexColor">
                <xsl:with-param name="hexColor" select="@w:color"/>
                <xsl:with-param name="style" select="@w:val"/>
              </xsl:call-template>
            </xsl:attribute>
          </xsl:if>
          <xsl:if test="@w:sz">
            <xsl:attribute name="{concat('border-',local-name(),'-width')}">
              <xsl:choose>
                <xsl:when test="parent::w:tcBorders">
                  <!-- set half border-width on the cell -->
                  <xsl:value-of select="@w:sz div 16"/><xsl:text>pt</xsl:text>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="@w:sz div 8"/><xsl:text>pt</xsl:text>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:attribute>
          </xsl:if>
          <!-- Table cell's paddings are setted by w:tblCellMar elements -->
          <!--<xsl:if test="@w:space">
            <xsl:choose>
              <xsl:when test="parent::w:pBdr">
                <xsl:attribute name="{concat('padding-',$side)}">
                  <xsl:value-of select="@w:space"/><xsl:text>pt</xsl:text>
                </xsl:attribute>
                <xsl:if test="$side='after' or $side='before'">
                  <xsl:attribute name="{concat('padding-',$side,'.conditionality')}">
                    <xsl:text>retain</xsl:text>
                  </xsl:attribute>
                </xsl:if>
              </xsl:when>
              <xsl:otherwise>
                <xsl:attribute name="{concat('padding-',local-name())}">
                  <xsl:value-of select="@w:space"/><xsl:text>pt</xsl:text>
                </xsl:attribute>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:if>-->
      </xsl:template>
      
      <!-- ================================================== -->
      <!-- Represents the properties for borders of an image. -->
      <!-- Parent elements: v:shape                           -->
      <!-- ================================================== -->
      <xsl:template match="w10:bordertop|w10:borderleft|w10:borderbottom|w10:borderright">
        <xsl:variable name="style-attribute-name" select="concat('border-', substring-after(local-name(), 'border'), '-style')"/>
        <xsl:call-template name="SetBorderStyle">
          <xsl:with-param name="attribute-name" select="$style-attribute-name"/>
          <xsl:with-param name="style" select="@type"/>
        </xsl:call-template>
        <xsl:if test="@width">
          <xsl:attribute name="{concat('border-',substring-after(local-name(), 'border'),'-width')}">
            <xsl:value-of select="@width div 8"/><xsl:text>pt</xsl:text>
          </xsl:attribute>
        </xsl:if>
      </xsl:template>
      
      <!-- ============================================= -->
      <!-- Represents the color for borders of an image. -->
      <!-- ============================================= -->
      <xsl:template match="@o:bordertopcolor|@o:borderleftcolor|@o:borderbottomcolor|@o:borderrightcolor">
        <xsl:attribute name="{concat('border-', substring-before(substring-after(local-name(), 'border'), 'color'), '-color')}">
          <xsl:choose>
            <xsl:when test=".='this'"><xsl:text>inherit</xsl:text></xsl:when>
            <xsl:otherwise><xsl:value-of select="."/></xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
      </xsl:template>
      
      <!-- ============================================== -->
      <!-- Maps to top, bottom, left, and right paddings. -->
      <!-- Parent elements: w:tblCellMar, w:tcMar         -->
      <!-- ============================================== -->
      <xsl:template match="w:top|w:left|w:bottom|w:right" mode="paddings">
        <xsl:attribute name="{concat('padding-',local-name())}">
          <xsl:call-template name="ConvertTableWidthPropertyType">
            <xsl:with-param name="type" select="@w:type"/>
            <xsl:with-param name="value" select="@w:w"/>
          </xsl:call-template>
        </xsl:attribute>
      </xsl:template>

      <!-- ================================================ -->
      <!-- Represents top, bottom, left, and right borders. -->
      <!-- Parent elements: wx:pBdrGroup/wx:borders         -->
      <!-- ================================================ -->
      <xsl:template match="wx:top|wx:bottom|wx:left|wx:right">
        <xsl:variable name="side">
          <xsl:choose>
            <xsl:when test="local-name()='bottom'">
              <xsl:text>after</xsl:text>
            </xsl:when>
            <xsl:when test="local-name()='top'">
              <xsl:text>before</xsl:text>
            </xsl:when>
            <xsl:otherwise><xsl:value-of select="local-name()"/></xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        <xsl:attribute name="{concat('border-',$side,'-style')}">
          <xsl:value-of select="@wx:val"/>
        </xsl:attribute>
        <xsl:if test="@wx:color">
          <xsl:attribute name="{concat('border-',$side,'-color')}">
            <xsl:call-template name="ConvertHexColor">
              <xsl:with-param name="hexColor" select="@wx:color"/>
              <xsl:with-param name="style" select="@wx:val"/>
            </xsl:call-template>
          </xsl:attribute>
        </xsl:if>
        <xsl:if test="@wx:bdrwidth">
          <xsl:attribute name="{concat('border-',$side,'-width')}">
            <xsl:value-of select="@wx:bdrwidth div 20"/><xsl:text>pt</xsl:text>
          </xsl:attribute>
          <xsl:if test="$side='after' or $side='before'">
            <xsl:attribute name="{concat('border-',$side,'-width.conditionality')}">
              <xsl:text>retain</xsl:text>
            </xsl:attribute>
          </xsl:if>
        </xsl:if>
        <xsl:if test="@wx:space">
          <xsl:attribute name="{concat('padding-',$side)}">
            <xsl:value-of select="@wx:space"/><xsl:text>pt</xsl:text>
          </xsl:attribute>
          <xsl:if test="$side='after' or $side='before'">
            <xsl:attribute name="{concat('padding-',$side,'.conditionality')}">
              <xsl:text>retain</xsl:text>
            </xsl:attribute>
          </xsl:if>
        </xsl:if>
      </xsl:template>

      <!-- ========================================== -->
      <!-- Represents the left margin for the border. -->
      <!-- Parent elements: wx:pBdrGroup              -->
      <!-- ========================================== -->
      <xsl:template match="wx:margin-left">
        <xsl:attribute name="padding-left">
          <xsl:value-of select="- @wx:val div 20"/>
          <xsl:text>pt</xsl:text>
        </xsl:attribute>
      </xsl:template>

      <!-- =========================================== -->
      <!-- Represents the right margin for the border. -->
      <!-- Parent elements: wx:pBdrGroup               -->
      <!-- =========================================== -->
      <xsl:template match="wx:margin-right">
        <xsl:attribute name="padding-right">
          <xsl:value-of select="- @wx:val div 20"/>
          <xsl:text>pt</xsl:text>
        </xsl:attribute>
      </xsl:template>

      <!-- ===================================== -->
      <!-- Represents the shading for the group. -->
      <!-- Parent elements: wx:pBdrGroup         -->
      <!-- ===================================== -->
      <xsl:template match="wx:shd">
        <xsl:attribute name="background-color">
          <xsl:call-template name="ConvertHexColor">
            <xsl:with-param name="hexColor" select="@wx:bgcolor"/>
          </xsl:call-template>
        </xsl:attribute>
      </xsl:template>

      <!-- ======================================== -->
      <!-- Represents shading and background-color. -->
      <!-- Parent elements: w:pPr, w:rPr, w:tblPr,  -->
      <!-- w:tblPrEx, w:tcpr.                       -->
      <!-- ======================================== -->
      <xsl:template match="w:shd">
        <xsl:if test="@w:fill">
          <xsl:attribute name="background-color">
            <xsl:call-template name="ConvertHexColor">
              <xsl:with-param name="hexColor" select="@w:fill"/>
            </xsl:call-template>
          </xsl:attribute>
        </xsl:if>
      </xsl:template>

      <!-- ========================================== -->
      <!-- Marks text as highlighted so it stands out -->
      <!-- from the surrounding text.                 -->
      <!-- Parent elements: w:rPr                     -->
      <!-- ========================================== -->
      <xsl:template match="w:highlight">
        <xsl:attribute name="background-color">
          <xsl:choose>
            <xsl:when test="@w:val='black'"><xsl:text>black</xsl:text></xsl:when>
            <xsl:when test="@w:val='blue'"><xsl:text>blue</xsl:text></xsl:when>
            <xsl:when test="@w:val='cyan'"><xsl:text>aqua</xsl:text></xsl:when>
            <xsl:when test="@w:val='green'"><xsl:text>lime</xsl:text></xsl:when>
            <xsl:when test="@w:val='magenta'"><xsl:text>fuchsia</xsl:text></xsl:when>
            <xsl:when test="@w:val='red'"><xsl:text>red</xsl:text></xsl:when>
            <xsl:when test="@w:val='yellow'"><xsl:text>yellow</xsl:text></xsl:when>
            <xsl:when test="@w:val='white'"><xsl:text>white</xsl:text></xsl:when>
            <xsl:when test="@w:val='dark-blue'"><xsl:text>navy</xsl:text></xsl:when>
            <xsl:when test="@w:val='dark-cyan'"><xsl:text>teal</xsl:text></xsl:when>
            <xsl:when test="@w:val='dark-green'"><xsl:text>green</xsl:text></xsl:when>
            <xsl:when test="@w:val='dark-magenta'"><xsl:text>purple</xsl:text></xsl:when>
            <xsl:when test="@w:val='dark-red'"><xsl:text>maroon</xsl:text></xsl:when>
            <xsl:when test="@w:val='dark-yellow'"><xsl:text>olive</xsl:text></xsl:when>
            <xsl:when test="@w:val='dark-grey'"><xsl:text>gray</xsl:text></xsl:when>
            <xsl:when test="@w:val='light-grey'"><xsl:text>silver</xsl:text></xsl:when>
            <xsl:otherwise><xsl:text>transparent</xsl:text></xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
      </xsl:template>


  <!-- ================== -->
  <!-- Default attributes -->
  <!-- ================== -->
      <!-- =============================================== -->
      <!-- Represents the default fonts for this document. -->
      <!-- Parent elements: w:fonts                        -->
      <!-- =============================================== -->
      <xsl:template match="w:defaultFonts">
        <xsl:choose>
          <xsl:when test="@w:ascii">
            <xsl:attribute name="font-family">
              <!-- If font definition has alt name we should use it. -->
              <xsl:variable name="alt-font-name">
                <xsl:call-template name="AltFontName">
                  <xsl:with-param name="font-name" select="@w:ascii"/>
                </xsl:call-template>
              </xsl:variable>
              <xsl:call-template name="NormalizeFontFamilyName">
                <xsl:with-param name="font-family" select="$alt-font-name"/>
              </xsl:call-template>
            </xsl:attribute>
          </xsl:when>
          <xsl:otherwise/>
        </xsl:choose>
      </xsl:template>

      <!-- ======================================= -->
      <!-- These attributes have different default -->
      <!-- values in XSL-FO and MS Word 2003.      -->
      <!-- ======================================= -->
      <xsl:template name="SetDefaultAttributes">
        <xsl:attribute name="widows"><xsl:value-of select="$default-widows"/></xsl:attribute>
        <xsl:attribute name="orphans"><xsl:value-of select="$default-orphans"/></xsl:attribute>
        <xsl:attribute name="font-size">
          <xsl:value-of select="$default-font-size"/>
          <xsl:text>pt</xsl:text>
        </xsl:attribute>
        <xsl:attribute name="line-height"><xsl:value-of select="$default-line-height"/></xsl:attribute>
        <xsl:attribute name="white-space-collapse"><xsl:value-of select="$white-space-collapse"/></xsl:attribute>
      </xsl:template>

</xsl:stylesheet>
