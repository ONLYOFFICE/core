<?xml version="1.0" encoding="UTF-8"?>

<!-- ===============================================

This stylesheet defines named templates for common tasks:

1. convertsion of hexadecimal color values from Word to XSL-FO;
2. substitution of Unicode for character codes in symbol fonts;
3. normalization of font family names;
4. parsing of 'CSS style'-like structures;
5. page number format setting.

==================================================== -->


<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:w="http://schemas.microsoft.com/office/word/2003/wordml"
                xmlns:my="dummy.namespace"
                version="1.0">

  <xsl:output method="xml"
              version="1.0"
              indent="no"
              encoding="utf-8" />


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


<!-- ================= -->
<!-- GENERAL TEMPLATES -->
<!-- ================= -->
  
  <!-- ================================================= -->
  <!-- Convert hexadecimal color from MS Word to XSL-FO. -->
  <!-- ================================================= -->
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
            <xsl:text>black</xsl:text>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:otherwise><xsl:value-of select="concat('#', $hexColor)"/></xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- ======================================== -->
  <!-- Check for existing unsupported elements. -->
  <!-- ======================================== -->
  <xsl:template name="CheckNotSupportedElements">
    <xsl:param name="elements"/>
    <xsl:for-each select="$elements">
      <xsl:if test="contains($not-supported-inlines, .)">
        <xsl:value-of select="'presents'"/>
      </xsl:if>
    </xsl:for-each>
  </xsl:template>

  <!-- ================================ -->
  <!-- Convert "Symbol" and "Wingdings" -->
  <!-- symbol codes to Unicode.         -->
  <!-- ================================ -->
  <xsl:template name="ConvertSymbol">
    <xsl:param name="font-family"/>
    <xsl:param name="symbol"/>
    
    <xsl:apply-templates select="document('')//my:recoding-table[@font=$font-family]/my:char[@value=$symbol or @altvalue=$symbol]"/>
    
  </xsl:template>

  <!-- ============================================= -->
  <!-- Convert symbol codes (expressed as a unicode) -->
  <!-- to Unicode character.                         -->
  <!-- ============================================= -->
  <xsl:template name="ConvertChars">
    <xsl:param name="font-family"/>
    <xsl:param name="char-string"/>
    
    <xsl:variable name="recorded-char" select="document('')//my:recoding-table[@font=$font-family]/my:char[@code=$char-string]"/>
    
    <xsl:choose>
      <xsl:when test="$recorded-char">
        <xsl:attribute name="font-size">
          <xsl:value-of select="$default-font-size.symbol"/><xsl:text>pt</xsl:text>
        </xsl:attribute>
        <xsl:attribute name="font-family"><xsl:text>ZapfDingbats, Arial</xsl:text></xsl:attribute>
        <xsl:apply-templates select="$recorded-char"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$char-string"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- ===================================== -->
  <!-- Apply an entry from a recording table -->
  <!-- ===================================== -->
  <xsl:template match="my:recoding-table/my:char">
    <!--<xsl:message>Replacing <xsl:value-of select="@code"/> by <xsl:value-of select="@entity"/></xsl:message>-->
    <xsl:value-of select="@entity"/>
  </xsl:template>


  <!-- ================= -->
  <!-- Set border-style. -->
  <!-- ================= -->
  <xsl:template name="SetBorderStyle">
    <xsl:param name="attribute-name"/>
    <xsl:param name="style"/>
    
    <xsl:attribute name="{$attribute-name}">
      <xsl:choose>
        <xsl:when test="$style='single' or $style='solid'"><xsl:text>solid</xsl:text></xsl:when>
        <xsl:when test="$style='double'"><xsl:text>double</xsl:text></xsl:when>
        <xsl:when test="$style='dotted'"><xsl:text>dotted</xsl:text></xsl:when>
        <xsl:when test="$style='dashed'"><xsl:text>dashed</xsl:text></xsl:when>
        <xsl:when test="$style='three-d-emboss'"><xsl:text>ridge</xsl:text></xsl:when>
        <xsl:when test="$style='inset'"><xsl:text>inset</xsl:text></xsl:when>
        <xsl:when test="$style='outset'"><xsl:text>outset</xsl:text></xsl:when>
        <xsl:otherwise><xsl:text>none</xsl:text></xsl:otherwise>
      </xsl:choose>
    </xsl:attribute>
  </xsl:template>

  <!-- ======================================== -->
  <!-- Convert tableWidthPropertyType (MS Word) -->
  <!-- to points or percents.                   -->
  <!-- ======================================== -->
  <xsl:template name="ConvertTableWidthPropertyType">
    <xsl:param name="type" select="auto"/>
    <xsl:param name="value" select="0"/>
    
    <xsl:choose>
      <xsl:when test="$type='nil' or $type='dxa'"><xsl:value-of select="$value div 20"/><xsl:text>pt</xsl:text></xsl:when>
      <xsl:when test="$type='pct'"><xsl:value-of select="$value div 50"/><xsl:text>%</xsl:text></xsl:when>
      <xsl:otherwise><xsl:text>auto</xsl:text></xsl:otherwise>
    </xsl:choose>
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
      <xsl:otherwise><xsl:text>auto</xsl:text></xsl:otherwise>
    </xsl:choose>
  </xsl:template>

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
  </xsl:template>

  <!-- ====================== -->
  <!-- Normalize font family. -->
  <!-- ====================== -->
  <xsl:template name="NormalizeFontFamilyName">
    <xsl:param name="font-family" />
    <xsl:choose>
      <xsl:when test="contains($font-family, ' ')">
        <xsl:value-of select="substring-before($font-family, ' ')"/>
        <xsl:call-template name="NormalizeFontFamilyName">
          <xsl:with-param name="font-family" select="substring-after($font-family, ' ')"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise><xsl:value-of select="$font-family"/></xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- ======================= -->
  <!-- Set page number format. -->
  <!-- ======================= -->
  <xsl:template name="PageNumberFormat">
    <xsl:param name="format"/>
    
    <xsl:choose>
      <xsl:when test="$format='Arabic'"><xsl:text>1</xsl:text></xsl:when>
      <xsl:when test="$format='ArabicDash'"><xsl:text>- 1 -</xsl:text></xsl:when>
      <xsl:when test="$format='alphabetic'"><xsl:text>a</xsl:text></xsl:when>
      <xsl:when test="$format='ALPHABETIC'"><xsl:text>A</xsl:text></xsl:when>
      <xsl:when test="$format='roman'"><xsl:text>i</xsl:text></xsl:when>
      <xsl:when test="$format='ROMAN'"><xsl:text>I</xsl:text></xsl:when>
      <xsl:otherwise><xsl:text>1</xsl:text></xsl:otherwise>
    </xsl:choose>
  </xsl:template>


<!-- ================= -->
<!-- SPECIAL TEMPLATES -->
<!-- ================= -->

    <!-- ================================== -->
    <!-- Count the number of spanned cells. -->
    <!-- ================================== -->
    <xsl:template name="CountCellsSpanned">
      <xsl:param name="number-cells-spanned"/>
      <xsl:param name="following-cell"/>
      <xsl:param name="col-or-row"/>
      <xsl:param name="real-column-number"/>
      <xsl:choose>
        <xsl:when test="$col-or-row='col' and $following-cell/w:tcPr/w:hmerge[not(@w:val='restart')]">
          <xsl:call-template name="CountCellsSpanned">
            <xsl:with-param name="number-cells-spanned" select="$number-cells-spanned + 1"/>
            <xsl:with-param name="following-cell" select="$following-cell/following-sibling::w:tc[1]"/>
            <xsl:with-param name="col-or-row" select="'col'"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:when test="$col-or-row='row' and $following-cell/w:tcPr/w:vmerge[not(@w:val='restart')]">
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

    <!-- ===================== -->
    <!-- Select alt font name. -->
    <!-- ===================== -->
    <xsl:template name="AltFontName">
      <xsl:param name="font-name"/>
      <xsl:variable name="alt-font-name" select="key('fonts', $font-name)/w:altName"/>
      <xsl:choose>
        <xsl:when test="$alt-font-name">
          <xsl:value-of select="$alt-font-name/@w:val"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$font-name"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:template>
    
    <!-- ========================== -->
    <!-- Concatanate an instruction -->
    <!-- ========================== -->
    <xsl:template name="ConcatInstruction">
      <xsl:param name="current-run-element" select="."/>
      <xsl:param name="num-nested-loops" select="1"/>
      
      <xsl:value-of select="normalize-space($current-run-element/preceding-sibling::w:r[w:instrText][1]/w:instrText)"/>
      
      <!-- This code is designed for parsing nested loops of field instructions. -->
      <!-- Commented because complex fields are unsupported -->
      <!--<xsl:choose>
        <xsl:when test="$current-run-element[w:fldChar/@fldCharType='begin'] and $num-nested-loops=1">
          <xsl:call-template name="InstrText"/>
        </xsl:when>
        <xsl:when test="$current-run-element[w:fldChar/@fldCharType='begin'] and $num-nested-loops!=1">
          <xsl:call-template name="ConcatInstruction">
            <xsl:with-param name="current-run-element" select="$current-run-element/preceding-sibling::w:r"/>
            <xsl:with-param name="num-nested-loops" select="$num-nested-loops - 1"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:when test="$current-run-element[w:fldChar/@fldCharType='end']">
          <xsl:call-template name="ConcatInstruction">
            <xsl:with-param name="current-run-element" select="$current-run-element/preceding-sibling::w:r"/>
            <xsl:with-param name="num-nested-loops" select="$num-nested-loops + 1"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:call-template name="ConcatInstruction">
            <xsl:with-param name="current-run-element" select="$current-run-element/preceding-sibling::w:r"/>
            <xsl:with-param name="num-nested-loops" select="$num-nested-loops"/>
          </xsl:call-template>
        </xsl:otherwise>
      </xsl:choose>-->
    </xsl:template>

</xsl:stylesheet>
