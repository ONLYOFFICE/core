<?xml version="1.0" encoding="UTF-8"?>

<!-- =============================================================

This stylesheet defines presentation and structure of
contents of the resulting document. It contains templates
to convert Word structures into their equivalents in XSL-FO.

******************************************************************

IMPLEMENTATION SPECIFICS

To preserve initial whitespace characters, fo:leader with zero
length is inserted before each text element, preventing the
leading whitespace from collapsing by the effect of 
'white-space-treatment' XSL-FO property.

============================================================== -->


<xsl:stylesheet xmlns:fo="http://www.w3.org/1999/XSL/Format"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:w="http://schemas.microsoft.com/office/word/2003/wordml"
                xmlns:o="urn:schemas-microsoft-com:office:office"
                xmlns:v="urn:schemas-microsoft-com:vml"
                xmlns:wx="http://schemas.microsoft.com/office/word/2003/auxHint"
                xmlns:aml="http://schemas.microsoft.com/aml/2001/core"
                xmlns:w10="urn:schemas-microsoft-com:office:word"
                xmlns:dt="uuid:C2F41010-65B3-11d1-A29F-00AA00C14882"
                xmlns:st1="urn:schemas-microsoft-com:office:smarttags"
                version="1.0">

  <xsl:output method="xml"
              version="1.0"
              indent="no"
              encoding="utf-8" />


<!-- ================== -->
<!-- DOCUMENT'S CONTENT -->
<!-- ================== -->
  <xsl:template match="w:body">
    <xsl:apply-templates select="*"/>
  </xsl:template>

  <!-- =========================== -->
  <!-- Headers and Footers content -->
  <!-- =========================== -->
  <xsl:template match="w:hdr">
    <xsl:apply-templates select="*"/>
  </xsl:template>

  <xsl:template match="w:ftr">
    <xsl:apply-templates select="*"/>
  </xsl:template>


  <!-- ============================================================ -->
  <!-- Elements that can occur anywhere where we start a new block, -->
  <!-- such inside the body, comments, headers, textboxes, etc.     -->
  <!-- ============================================================ -->
    <!-- ============================================= -->
    <!-- Represents a container for document sections. -->
    <!-- ============================================= -->
    <xsl:template match="wx:sect">
      <xsl:element name="{$section-block-name}" namespace="{$section-block-namespace}">
        <!-- Set default properties. -->
        <xsl:call-template name="SetDefaultAttributes"/>
        <!-- Apply section properties. -->
        <!--<xsl:apply-templates select=".//w:sectPr/*[not(self::w:pgSz)]"/>-->
        <xsl:if test="$section-block-name='rx:flow-section'">
          <xsl:apply-templates select=".//w:sectPr/w:cols"/>
        </xsl:if>
        
        <fo:marker marker-class-name="first-page-header">
          <xsl:call-template name="DisplayStaticContent">
            <xsl:with-param name="section" select="."/>
            <xsl:with-param name="hdr-or-ftr" select="'header'"/>
            <xsl:with-param name="type" select="'first'"/>
          </xsl:call-template>
        </fo:marker>
        <fo:marker marker-class-name="first-page-footer">
          <xsl:call-template name="DisplayStaticContent">
            <xsl:with-param name="section" select="."/>
            <xsl:with-param name="hdr-or-ftr" select="'footer'"/>
            <xsl:with-param name="type" select="'first'"/>
          </xsl:call-template>
        </fo:marker>
        <fo:marker marker-class-name="odd-page-header">
          <xsl:call-template name="DisplayStaticContent">
            <xsl:with-param name="section" select="."/>
            <xsl:with-param name="hdr-or-ftr" select="'header'"/>
            <xsl:with-param name="type" select="'odd'"/>
          </xsl:call-template>
        </fo:marker>
        <fo:marker marker-class-name="odd-page-footer">
          <xsl:call-template name="DisplayStaticContent">
            <xsl:with-param name="section" select="."/>
            <xsl:with-param name="hdr-or-ftr" select="'footer'"/>
            <xsl:with-param name="type" select="'odd'"/>
          </xsl:call-template>
        </fo:marker>
        <fo:marker marker-class-name="even-page-header">
          <xsl:call-template name="DisplayStaticContent">
            <xsl:with-param name="section" select="."/>
            <xsl:with-param name="hdr-or-ftr" select="'header'"/>
            <xsl:with-param name="type" select="'even'"/>
          </xsl:call-template>
        </fo:marker>
        <fo:marker marker-class-name="even-page-footer">
          <xsl:call-template name="DisplayStaticContent">
            <xsl:with-param name="section" select="."/>
            <xsl:with-param name="hdr-or-ftr" select="'footer'"/>
            <xsl:with-param name="type" select="'even'"/>
          </xsl:call-template>
        </fo:marker>
        
        <xsl:variable name="body-content">
          <xsl:apply-templates select="*"/> <!-- select="*[not(self::w:sectPr)]" -->
        </xsl:variable>
        <xsl:choose>
          <xsl:when test="string-length($body-content) != 0">
              <xsl:copy-of select="$body-content"/>
          </xsl:when>
          <xsl:otherwise>
            <fo:block><fo:leader/></fo:block>
          </xsl:otherwise>
        </xsl:choose>

      </xsl:element>
    </xsl:template>

    <!-- ================================= -->
    <!-- Represents a document subsection. -->
    <!-- Parent elements: wx:sect.         -->
    <!-- ================================= -->
    <xsl:template match="wx:sub-section">
      <fo:block id="{generate-id()}">
        <xsl:apply-templates select="*"/>
      </fo:block>
    </xsl:template>

    <!-- ======================================================== -->
    <!-- wx:pBdrGroup element holds borders on a group of blocks. -->
    <!-- ======================================================== -->
    <xsl:template match="wx:pBdrGroup">
      <fo:block>
        <!-- borders on the group of blocks -->
        <xsl:apply-templates select="wx:borders/*"/>
        <!-- background-color -->
        <xsl:apply-templates select="wx:shd"/>
        <!-- left and right paddings -->
        <xsl:apply-templates select="wx:margin-left | wx:margin-right"/>

        <xsl:variable name="first-paragraph" select="w:p[position()=1]"/>
        <xsl:variable name="last-paragraph" select="w:p[position()=last()]"/>
        <!-- set space-before from the first child paragrapg -->
          <!-- default paragraph style -->
          <xsl:apply-templates select="$default-paragraph-style">
            <xsl:with-param name="specific-attribute" select="'w:spacing'"/>
            <xsl:with-param name="mode" select="'before'"/>
          </xsl:apply-templates>
          <!-- style-level (pStyle)-->
          <xsl:apply-templates select="key('styles', $first-paragraph/w:pPr/w:pStyle/@w:val)[@w:type='paragraph']">
            <xsl:with-param name="specific-attribute" select="'w:spacing'"/>
            <xsl:with-param name="mode" select="'before'"/>
          </xsl:apply-templates>
           <!-- p-level (direct; override style-level) -->
          <xsl:apply-templates select="$first-paragraph/w:pPr/w:spacing">
            <xsl:with-param name="mode" select="'before'"/>
          </xsl:apply-templates>
        <!-- set space-after from the last child paragrapg -->
          <!-- default paragraph style -->
          <xsl:apply-templates select="$default-paragraph-style">
            <xsl:with-param name="specific-attribute" select="'w:spacing'"/>
            <xsl:with-param name="mode" select="'after'"/>
          </xsl:apply-templates>
          <!-- style-level (pStyle)-->
          <xsl:apply-templates select="key('styles', $last-paragraph/w:pPr/w:pStyle/@w:val)[@w:type='paragraph']">
            <xsl:with-param name="specific-attribute" select="'w:spacing'"/>
            <xsl:with-param name="mode" select="'after'"/>
          </xsl:apply-templates>
           <!-- p-level (direct; override style-level) -->
          <xsl:apply-templates select="$last-paragraph/w:pPr/w:spacing">
            <xsl:with-param name="mode" select="'after'"/>
          </xsl:apply-templates>

        <!-- the group content -->
        <xsl:apply-templates select="*[not(self::wx:borders) and not(self::wx:apo) and not(self::wx:shd)
                                       and not(self::wx:margin-left) and not(self::wx:margin-right)]"/>
      </fo:block>
    </xsl:template>


    <!-- =============================== -->
    <!-- Represents a paragraph element. -->
    <!-- =============================== -->
    <xsl:template match="w:p">
      <xsl:variable name="isList" select="w:pPr/w:listPr"/>
      <xsl:variable name="isFootnote" select="parent::w:footnote"/>
      <xsl:choose>
        <xsl:when test="$isList">
          <fo:list-block>
            <fo:list-item>
              <xsl:call-template name="ApplyParaProperties"/>
              <!-- Display a list-item-label element. -->
              <xsl:apply-templates select="w:pPr/w:listPr"/>
              <!-- Display a list-item-body element. -->
              <fo:list-item-body>
                <!-- Set explicit end-indent on list-item-body to avoid a warning. -->
                <xsl:attribute name="end-indent">inherit</xsl:attribute>
                <xsl:attribute name="start-indent">
                  <xsl:choose>
                     <xsl:when test="w:pPr/w:tabs/w:tab[@w:val='left']">
                       <xsl:value-of select="w:pPr/w:tabs/w:tab[@w:val='left']/@w:pos div 20"/>
                       <xsl:text>pt</xsl:text>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:variable name="list" select="key('lists', w:pPr/w:listPr/w:ilfo/@w:val)"/>
                      <xsl:variable name="tab" select="key('list-properties', $list/w:ilst/@w:val)/w:lvl[@w:ilvl=current()/w:pPr/w:listPr/w:ilvl/@w:val]/w:pPr/w:tabs/w:tab[@w:val='list']"/>
                      <xsl:choose>
                        <xsl:when test="w:pPr/w:tabs/w:tab[@w:val='clear']">
                          <xsl:value-of select="(w:pPr/w:listPr/wx:t/@wx:wTabAfter + w:pPr/w:listPr/wx:t/@wx:wTabBefore) div 20 + $default-width.list-label"/>
                          <xsl:text>pt</xsl:text>
                        </xsl:when>
                        <xsl:when test="$tab">
                          <xsl:value-of select="$tab/@w:pos div 20"/>
                          <xsl:text>pt</xsl:text>
                        </xsl:when>
                        <xsl:when test="w:pPr/w:listPr/wx:t">
                          <xsl:value-of select="(w:pPr/w:listPr/wx:t/@wx:wTabAfter + w:pPr/w:listPr/wx:t/@wx:wTabBefore) div 20 + $default-width.list-label"/>
                          <xsl:text>pt</xsl:text>
                        </xsl:when>
                        <xsl:otherwise><xsl:text>0pt</xsl:text></xsl:otherwise>
                      </xsl:choose>
                    </xsl:otherwise>
                  </xsl:choose>
                </xsl:attribute>
                <xsl:attribute name="text-indent"><xsl:text>0pt</xsl:text></xsl:attribute>
                <fo:block>
                  <xsl:apply-templates select="*[not(self::w:pPr)]"/>
                  <xsl:if test="not(w:r)"><fo:leader/></xsl:if>
                </fo:block>
              </fo:list-item-body>
            </fo:list-item>
          </fo:list-block>
        </xsl:when>
        <xsl:when test="$isFootnote">
          <fo:footnote>
            <xsl:apply-templates select="w:r[w:footnoteRef]"/>
            
            <fo:footnote-body>
              <fo:block start-indent="0" text-indent="0" font-size="10pt" padding-before="3pt">
                <!-- Set properties -->
                <xsl:call-template name="ApplyParaProperties"/>
                
                <fo:inline keep-together.within-line="always" font-size="66.7%" baseline-shift="super">
                  <xsl:number level="any" count="w:footnote" from="/w:wordDocument/w:body" format="1"/>
                </fo:inline>
                <!--<xsl:text> </xsl:text>-->
                <xsl:apply-templates select="*[not(w:footnoteRef) and not(self::w:pPr)]"/>
              </fo:block>
            </fo:footnote-body>
          </fo:footnote>
        </xsl:when>
        <xsl:otherwise>
          <xsl:variable name="needDisplayBlock">
            <xsl:call-template name="CheckNotSupportedElements">
              <xsl:with-param name="elements" select="w:r/*[not(self::w:rPr)]"/>
            </xsl:call-template>
          </xsl:variable>
          <xsl:if test="true()"> <!-- $needDisplayBlock -->
          <fo:block>
            <xsl:call-template name="ApplyParaProperties"/>
            <xsl:choose>
              <!-- If the paragraph doesn't contains runs - -->
              <!-- it acts as a pargraph break element.     -->
              <xsl:when test="count(*[not(self::w:pPr)])=0 and not(wx:allowEmptyCollapse)">
                <!-- Set inline-level properties -->
                <xsl:apply-templates select="w:pPr/w:rPr/*[not(self::w:rStyle)]"/>
                <fo:leader/>
              </xsl:when>
              <xsl:otherwise>
                <!-- Display run-level elements. -->
                <xsl:apply-templates select="*[not(self::w:pPr)]"/>
              </xsl:otherwise>
            </xsl:choose>
          </fo:block>
          </xsl:if>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:template>


    <!-- ========================= -->
    <!-- Represents a run element. -->
    <!-- Parent elements: w:p.     -->
    <!-- ========================= -->
    <xsl:template match="w:r">
      <!-- Don't generate inline area, if the run contains the following: -->
        <!-- 1. footnote -->
        <!-- 2. endnote -->
        <!-- 3. field-delimiting character -->
        <!-- 4. field instruction -->
      <xsl:if test="not(w:footnote) and not(w:endnote) and not(w:fldChar) and not(w:instrText)">
        <fo:inline>
          <!-- Set properties -->
          <xsl:call-template name="ApplyRunProperties"/>
          
          <!-- Display text elements. -->
          <xsl:apply-templates select="*[not(self::w:rPr)]"/>
        </fo:inline>
      </xsl:if>
      
      <!-- Display a footnote -->
      <xsl:apply-templates select="w:footnote"/>
      <!-- Field instructions are enclosed in field-delimiting characters. -->
      <!-- Apply a field-delimiting character to process field instruction content. -->
      <xsl:apply-templates select="w:fldChar"/>
    </xsl:template>


    <!-- ======================== -->
    <!-- Represents text content. -->
    <!-- Parent elements: w:r.    -->
    <!-- ======================== -->
    <xsl:template match="w:t">
      <!-- Preserve initial whitespace characters. -->
      <!-- fo:leader with zero lenght affects on white-space-treatment:   -->
      <!-- initial white-space characters are not surround line-feed now. -->
      <fo:leader leader-length="0pt" />
      <!-- text content -->
      <xsl:choose>
        <xsl:when test="ancestor::w:fldSimple">
          <xsl:call-template name="ApplyFieldInstruction">
            <xsl:with-param name="instruction" select="normalize-space(ancestor::w:fldSimple/@w:instr)"/>
            <xsl:with-param name="default-text" select="."/>
          </xsl:call-template>
        </xsl:when>
        <xsl:when test="../preceding-sibling::w:r[1][w:fldChar/@w:fldCharType='separate']">
          <xsl:call-template name="ApplyFieldInstruction">
            <xsl:with-param name="instruction">
              <xsl:call-template name="ConcatInstruction">
                <xsl:with-param name="current-run-element" select="parent::w:r"/>
              </xsl:call-template>
            </xsl:with-param>
            <xsl:with-param name="default-text" select="."/>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="."/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:template>


    <!-- ============================== -->
    <!-- Represents a symbol character. -->
    <!-- Parent elements: w:r.          -->
    <!-- ============================== -->
    <xsl:template match="w:sym">
      <xsl:call-template name="ConvertSymbol">
        <xsl:with-param name="symbol" select="@w:char"/>
        <xsl:with-param name="font-family" select="@w:font"/>
      </xsl:call-template>
    </xsl:template>


  <!-- ========================== -->
  <!-- Lists and their properties -->
  <!-- ========================== -->
    <xsl:template match="w:listPr">
      <fo:list-item-label>
        <xsl:attribute name="font-size">
          <xsl:value-of select="$default-font-size.list-label"/>
          <xsl:text>pt</xsl:text>
        </xsl:attribute>
        <!-- list-item-label properties at list definition level-->
        <xsl:apply-templates select="key('lists', w:ilfo/@w:val)">
          <xsl:with-param name="list-level" select="w:ilvl/@w:val"/>
        </xsl:apply-templates>
        <!-- direct list-item-label properties -->
        <xsl:apply-templates select="*[not(self::ilvl) and not(self::w:ilfo) and not(self::wx:t)]"/>
        <!-- redefine start-indent -->
        <xsl:if test="wx:t/@wx:wTabBefore">
          <xsl:attribute name="start-indent"><xsl:value-of select="wx:t/@wx:wTabBefore div 20"/><xsl:text>pt</xsl:text></xsl:attribute>
        </xsl:if>
        <xsl:attribute name="text-indent"><xsl:text>0pt</xsl:text></xsl:attribute>
        <!-- list-item-label-content -->
        <xsl:apply-templates select="wx:t"/>
      </fo:list-item-label>
    </xsl:template>

    <xsl:template match="w:list">
      <xsl:param name="list-level"/>
      <!-- list properties defined inside an appropriate w:listDef element -->
      <xsl:apply-templates select="key('list-properties', w:ilst/@w:val)/w:lvl[@w:ilvl=$list-level]"/>
      <!-- overriden list properties -->
      <xsl:apply-templates select="lvlOverride">
        <xsl:with-param name="list-level" select="$list-level"/>
      </xsl:apply-templates>
    </xsl:template>

    <xsl:template match="w:lvl">
      <xsl:apply-templates select="*"/>
    </xsl:template>

    <xsl:template match="w:lvlOverride">
      <xsl:param name="list-level"/>
      <xsl:apply-templates select="w:lvl[@w:ilvl=$list-level]"/>
    </xsl:template>

    <!-- =================================== -->
    <!-- Represents list-item-label content. -->
    <!-- Parent elements: w:listPr.          -->
    <!-- =================================== -->
    <xsl:template match="wx:t">
      <fo:block>
        <xsl:call-template name="ConvertChars">
          <xsl:with-param name="char-string" select="@wx:val"/>
          <xsl:with-param name="font-family" select="../wx:font/@wx:val"/>
        </xsl:call-template>
      </fo:block>
    </xsl:template>



  <!-- ================ -->
  <!-- Tabular elements -->
  <!-- ================ -->
      <!-- ============================= -->
      <!-- Represents the table element. -->
      <!-- ============================= -->
      <xsl:template match="w:tbl">
        <fo:table>
          <!-- Set default-style table properties -->
          <xsl:apply-templates select="$default-table-style"/>
          <!-- Set style-level table properties -->
          <xsl:apply-templates select="key('styles', w:tblPr/w:tblStyle/@w:val)[@w:type='table']"/>
          <!-- Set direct table properties -->
          <xsl:apply-templates select="w:tblPr/*[not(self::w:tblStyle) and not(self::w:tblCellMar)]"/>
          <!-- Set table columns -->
          <xsl:apply-templates select="w:tblGrid"/>
          <!-- Internal links -->
          <xsl:apply-templates select="aml:annotation"/>
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

      <!-- ====================== -->
      <!-- Parent element: w:tbl. -->
      <!-- ====================== -->
      <xsl:template match="w:tblGrid">
        <xsl:apply-templates select="*"/>
      </xsl:template>

      <!-- ========================== -->
      <!-- Represents a table column. -->
      <!-- Parent element: w:tblGrid. -->
      <!-- ========================== -->
      <xsl:template match="w:gridCol">
        <fo:table-column>
          <xsl:attribute name="column-number"><xsl:number count="w:gridCol"/></xsl:attribute>
          <xsl:attribute name="column-width"><xsl:value-of select="@w:w div 20"/><xsl:text>pt</xsl:text></xsl:attribute>
        </fo:table-column>
      </xsl:template>

      <!-- ======================= -->
      <!-- Represents a table row. -->
      <!-- Parent element: w:tbl.  -->
      <!-- ======================= -->
      <xsl:template match="w:tr">
        <fo:table-row>
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
          <xsl:apply-templates select="w:tc"/>
        </fo:table-row>
      </xsl:template>

      <!-- ======================== -->
      <!-- Represents a table cell. -->
      <!-- Parent element: w:tr.    -->
      <!-- ======================== -->
      <xsl:template match="w:tc">
        <!-- should not generate fo:table-cell when the w:tc is spanned cell -->
        <xsl:if test="(not(w:tcPr/w:hmerge) or w:tcPr/w:hmerge/@w:val='restart') and (not(w:tcPr/w:vmerge) or w:tcPr/w:vmerge/@w:val='restart')">
          <fo:table-cell>
            <xsl:variable name="table-properties" select="ancestor::w:tbl/w:tblPr"/>
            <!-- Set default-style table-cell properties -->
            <xsl:apply-templates select="$default-table-style" mode="table-cells"/>
            <!-- Set style-level table-cell properties -->
            <xsl:apply-templates select="key('styles', $table-properties/w:tblStyle/@w:val)[@w:type='table']" mode="table-cells"/>
            <!-- Apply direct table-cell properties defined on table level -->
            <xsl:apply-templates select="$table-properties/w:tblBorders/w:insideH|$table-properties/w:tblBorders/w:insideV|$table-properties/w:tblCellMar"/>
            <!-- Set direct table-cell properties -->
            <xsl:apply-templates select="w:tcPr/*"/>
            
            <!-- The table cell content -->
            <xsl:apply-templates select="*[not(self::w:tcPr)]"/>
          </fo:table-cell>
        </xsl:if>
      </xsl:template>



  <!-- ========== -->
  <!-- Tabulation -->
  <!-- ========== -->
      <!-- ========================= -->
      <!-- Represents tab stop list. -->
      <!-- Parent elements: w:pPr    -->
      <!-- ========================= -->
      <xsl:template match="w:tabs">
        <xsl:apply-templates select="*"/>
      </xsl:template>
      
      <!-- ======================= -->
      <!-- Represents a tab stop.  -->
      <!-- Parent elements: w:tabs -->
      <!-- ======================= -->
      <xsl:template match="w:tabs/w:tab">
      </xsl:template>
      
      <!-- =========================== -->
      <!-- Represents a tab character. -->
      <!-- Parent elements: w:r        -->
      <!-- =========================== -->
      <xsl:template match="w:tab">
        <!-- Draw leader only if the paragraph contains visible text content. -->
        <xsl:if test="ancestor::w:p[w:r/w:t or w:hlink or w:fldSimple or w:subDoc]">
          <fo:leader leader-pattern="space">
            <xsl:attribute name="leader-length">
              <xsl:choose>
                <xsl:when test="@wx:wTab">
                  <xsl:value-of select="@wx:wTab div 20"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="../preceding-sibling::w:r[w:tab/@wx:wTab][1]/w:tab/@wx:wTab div 20"/>
                </xsl:otherwise>
              </xsl:choose>
              <xsl:text>pt</xsl:text>
            </xsl:attribute>
          </fo:leader>
        </xsl:if>
      </xsl:template>



  <!-- ======== -->
  <!-- Graphics -->
  <!-- ======== -->
      <!-- =========================================== -->
      <!-- Represents a picture or other binary object -->
      <!-- that appears at this point in the document. -->
      <!-- Parent elements: w:r                        -->
      <!-- =========================================== -->
      <xsl:template match="w:pict">
        <xsl:variable name="binary-data" select="w:binData | w:movie | w:background | w:applet
                                                 | w:scriptAnchor | w:ocx | w:msAccessHTML"/>
        <xsl:apply-templates select="v:shape">
          <xsl:with-param name="binary-data" select="$binary-data"/>
        </xsl:apply-templates>
      </xsl:template>

      <!-- ======================================= -->
      <!-- Containes the reference to this object. -->
      <!-- ======================================= -->
      <xsl:template match="v:shape">
        <xsl:param name="binary-data"/>
        
        <xsl:if test="v:imagedata">
          <fo:external-graphic>
            <!-- content-width, content-height -->
            <xsl:call-template name="ParsingCSSStyle">
              <xsl:with-param name="style" select="@style"/>
              <xsl:with-param name="object" select="'image'"/>
            </xsl:call-template>
            <!-- scaling and etc. -->
            <xsl:apply-templates select="o:lock"/>
            <!-- border styles and border widths -->
            <xsl:apply-templates select="w10:bordertop|w10:borderbottom|w10:borderleft|w10:borderright"/>
            <!-- border colors -->
            <xsl:apply-templates select="@o:bordertopcolor|@o:borderleftcolor|@o:borderbottomcolor|@o:borderrightcolor"/>
            
            <xsl:attribute name="src">
              <xsl:choose>
                <xsl:when test="$binary-data">
                  <!-- Embedded object -->
                  <xsl:apply-templates select="$binary-data" />
                </xsl:when>
                <xsl:otherwise>
                  <!-- Linked object -->
                  <xsl:value-of select="concat('url(&quot;', v:imagedata/@src, '&quot;)')"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:attribute>
        </fo:external-graphic>
        </xsl:if>
      </xsl:template>

      <!-- ================================================== -->
      <!-- Contains the binary data representing this object. -->
      <!-- Parent elements: w:pict, w:bgPict                  -->
      <!-- ================================================== -->
      <xsl:template match="w:binData">
        <xsl:variable name="media-type">
          <xsl:text>image/</xsl:text>
          <xsl:call-template name="RetrieveMediaSubtype">
            <xsl:with-param name="name" select="@w:name"/>
          </xsl:call-template>
        </xsl:variable>
        
        <xsl:value-of select="concat('url(&quot;data:', $media-type, ';base64,', ., '&quot;)')"/>
      </xsl:template>

      <!-- =========================================== -->
      <!-- Auxiliary information for image processing. -->
      <!-- =========================================== -->
      <xsl:template match="o:lock">
        <xsl:if test="@aspectratio">
          <xsl:attribute name="scaling">
            <xsl:choose>
              <xsl:when test="@aspectratio='f'">
                <xsl:text>non-uniform</xsl:text>
              </xsl:when>
              <xsl:otherwise>
                <xsl:text>uniform</xsl:text>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:attribute>
        </xsl:if>
      </xsl:template>



  <!-- ========== -->
  <!-- Hyperlinks -->
  <!-- ========== -->
      <!-- =============================== -->
      <!-- Represents a hyperlink element. -->
      <!-- Parent elements: w:p, w:hlink.  -->
      <!-- =============================== -->
      <xsl:template match="w:hlink">
        <fo:basic-link>
          <xsl:choose>
            <xsl:when test="@w:dest">
              <xsl:attribute name="external-destination"><xsl:value-of select="concat('url(&quot;', @w:dest, '&quot;)')"/></xsl:attribute>
              <xsl:apply-templates select="*"/>
            </xsl:when>
            <xsl:when test="@w:bookmark">
              <xsl:attribute name="internal-destination"><xsl:value-of select="@w:bookmark"/></xsl:attribute>
              <xsl:apply-templates select="*"/>
            </xsl:when>
            <xsl:otherwise/>
          </xsl:choose>
        </fo:basic-link>
      </xsl:template>

      <!-- ============================================================ -->
      <!-- Represents a tracked insertion, deletion, formatting change, -->
      <!-- comment, or bookmark (internal link) in a document.          -->
      <!-- ============================================================ -->
      <!-- block-level context -->
      <xsl:template match="aml:annotation">
        <xsl:choose>
          <xsl:when test="@w:type='Word.Bookmark.Start'"><fo:block id="{@w:name}"/></xsl:when>
          <xsl:otherwise/>
        </xsl:choose>
      </xsl:template>
      
      <!-- inline-level context -->
      <xsl:template match="aml:annotation[ancestor::w:p]" priority="1">
        <xsl:choose>
          <xsl:when test="@w:type='Word.Bookmark.Start'"><fo:inline id="{@w:name}"/></xsl:when>
          <xsl:otherwise/>
        </xsl:choose>
      </xsl:template>
      
      <!-- block-level context -->
      <xsl:template match="aml:annotation[ancestor::w:footnote]" priority="2">
        <xsl:choose>
          <xsl:when test="@w:type='Word.Bookmark.Start'"><fo:block id="{@w:name}"/></xsl:when>
          <xsl:otherwise/>
        </xsl:choose>
      </xsl:template>



  <!-- ========= -->
  <!-- Footnotes -->
  <!-- ========= -->
      <!-- ===================================== -->
      <!-- Represents a footnote that appears at -->
      <!-- this point in the document.           -->
      <!-- Parent elements: w:r                  -->
      <!-- ===================================== -->
      <xsl:template match="w:footnote">
        <xsl:apply-templates select="w:p"/>
      </xsl:template>
      
      <!-- ===================================== -->
      <!-- Represents a footnote reference mark. -->
      <!-- Parent elements: w:r                  -->
      <!-- ===================================== -->
      <xsl:template match="w:footnoteRef">
        <!-- Set keep-with-previous.within-line -->
        <!-- It's default behaviour of MS Word. -->
        <xsl:attribute name="keep-with-previous.within-line">
          <xsl:text>always</xsl:text>
        </xsl:attribute>
        
        <xsl:number level="any" count="/w:wordDocument/w:body//w:footnote" format="1"/>
      </xsl:template>



  <!-- ============================= -->
  <!-- Page, column, and line breaks -->
  <!-- ============================= -->
      <!-- ================================= -->
      <!-- Represents a text-wrapping break. -->
      <!-- @w:type specifies the break type. -->
      <!-- Parent elements: w:r              -->
      <!-- ================================= -->
      <xsl:template match="w:br">
        <!-- defines a page, column, or text-wrapping break -->
        <xsl:choose>
          <xsl:when test="@w:type='page'">
          </xsl:when>
          <xsl:when test="@w:type='column'">
          </xsl:when>
          <!-- if @w:type="text-wrapping" or omitted - it's a new-line break -->
          <xsl:otherwise><fo:block/></xsl:otherwise>
        </xsl:choose>
      </xsl:template>



  <!-- ======= -->
  <!-- Hyphens -->
  <!-- ======= -->
      <!-- =========================================== -->
      <!-- Represents a soft-hyphen (optional hyphen). -->
      <!-- =========================================== -->
      <xsl:template match="w:softHyphen">
        <xsl:text>&#x00AD;</xsl:text>
      </xsl:template>
      <!-- ================================= -->
      <!-- Represents a non-breaking hyphen. -->
      <!-- ================================= -->
      <xsl:template match="w:noBreakHyphen">
        <xsl:text>&#2011;</xsl:text>
      </xsl:template>



  <!-- ====== -->
  <!-- Fields -->
  <!-- ====== -->
      <!-- ============================================================== -->
      <!-- Represents a simple Word field (with plain text instructions). -->
      <!-- Simple fields are run-time calculated entities in Word         -->
      <!-- (for example, page numbers).                                   -->
      <!-- Parent elements: w:p, w:hlink, w:fldSimple.                    -->
      <!-- ============================================================== -->
      <xsl:template match="w:fldSimple">
        <xsl:apply-templates select="*"/>
      </xsl:template>
      
      <!-- ================================= -->
      <!-- Apply an instruction for a field. -->
      <!-- ================================= -->
      <xsl:template name="ApplyFieldInstruction">
        <xsl:param name="instruction"/>
        <xsl:param name="default-text"/>
        
        <xsl:choose>
          <xsl:when test="starts-with($instruction, 'PAGE')">
            <fo:page-number/>
          </xsl:when>
          <xsl:when test="starts-with($instruction, 'NUMPAGES')">
            <fo:page-number-citation ref-id="{generate-id(/)}"/>
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



  <!-- ========== -->
  <!-- Smart tags -->
  <!-- ========== -->
      <xsl:template match="st1:*">
        <xsl:apply-templates select="*"/>
      </xsl:template>

</xsl:stylesheet>
