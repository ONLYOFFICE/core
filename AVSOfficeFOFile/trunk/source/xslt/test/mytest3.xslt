<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet
  version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:msxsl="urn:schemas-microsoft-com:xslt"
  xmlns:fo="http://www.w3.org/1999/XSL/Format"
  xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"
  
  xmlns:oox="urn:oox"
  exclude-result-prefixes="w oox msxsl">

  <xsl:output method="xml"
              version="1.0"
              indent="yes"
              encoding="utf-8"/>

  <!-- ======== -->
  <!-- DEFAULTS -->
  <!-- ======== -->
  <xsl:variable name="default-width.list-label" select="9"/>
  <xsl:variable name="default-font-size" select="10"/>
  <xsl:variable name="default-font-size.list-label" select="12"/>
  <xsl:variable name="default-font-size.symbol" select="12"/>
  <xsl:variable name="default-widows" select="2"/>
  <xsl:variable name="default-orphans" select="2"/>
  <xsl:variable name="white-space-collapse" select="'false'"/>
  <xsl:variable name="default-header-extent" select="'11in'"/>
  <xsl:variable name="default-footer-extent" select="'11in'"/>
  <xsl:variable name="default-line-height" select="1.147"/>


  <xsl:variable name="master-name">simple</xsl:variable>

  <xsl:key name="Part" match="/oox:package/oox:part" use="@oox:name"/>

  <xsl:key name="styles" match="/oox:package/oox:part/w:styles/w:style" use="@w:styleId"/>

  <xsl:variable name="default-paragraph-style" select="key('Part', 'word/styles.xml')/w:styles/w:style[@w:default='1' and @w:type='paragraph']"/>
  <xsl:variable name="default-character-style" select="key('Part', 'word/styles.xml')/w:styles/w:style[@w:default='1' and @w:type='character']"/>
  <xsl:variable name="default-table-style" select="key('Part', 'word/styles.xml')/w:styles/w:style[@w:default='on' and @w:type='table']"/>
  <xsl:variable name="default-rPr-style" select="key('Part', 'word/styles.xml')/w:styles/w:docDefaults/w:rPrDefault/w:rPr"/>
  
  
  <xsl:template match="/oox:package">
    <fo:root>

      <xsl:comment>
        <xsl:text>debug output:
default-paragraph-style: </xsl:text>
        <xsl:value-of select="$default-paragraph-style/@w:styleId"/>
        <xsl:text>debug output:
default-character-style: </xsl:text>
        <xsl:value-of select="$default-character-style/@w:styleId"/>
      </xsl:comment>
      
      
      
      <!--
      Children:layout-master-set,declarations?,page-sequence+
      -->

      <!-- single <fo:layout-master-set> object -->
      <xsl:call-template name="Include-layout-master-set-object"/>

      <!-- an optional <fo:declarations> object -->
      <xsl:call-template name="Include-declarations-object"/>

      <!-- one or more <fo:page-sequences> objects -->
      <xsl:call-template name="Include-page-sequencese">
        <xsl:with-param name="master-reference-name">
          <xsl:value-of select="$master-name"/>
        </xsl:with-param>
      </xsl:call-template>
    </fo:root>
  </xsl:template>


  <!-- IMPLEMENTATION -->
  <!-- single <fo:layout-master-set> object -->
  <xsl:template name="Include-layout-master-set-object">

    <!--
    <xsl:variable name="page-width">21cm</xsl:variable>
    <xsl:variable name="page-height">29.7cm</xsl:variable>
    -->

    <xsl:element name="fo:layout-master-set">

      <xsl:element name="fo:simple-page-master">

        <xsl:attribute name="master-name">
          <xsl:value-of select="$master-name"/>
        </xsl:attribute>

        <xsl:apply-templates select="//oox:part/w:document/w:body/w:sectPr"/>

        <xsl:element name="fo:region-body"/>

        <!-- fo:simple-page-master -->
      </xsl:element>

      <!-- fo:layout-master-set -->
    </xsl:element>

    <!-- Include-layout-master-set-object -->
  </xsl:template>


  <xsl:template match="//oox:part/w:document/w:body/w:sectPr">
    <xsl:apply-templates select="w:pgSz"/>
    <xsl:apply-templates select="w:pgMar">
      <xsl:with-param name="area" select="'page-master'"/>
    </xsl:apply-templates>
  </xsl:template>

  <!-- PAGE SIZE -->
  <xsl:template match="w:pgSz">
    <xsl:attribute name="page-width">
      <xsl:call-template name="Convert-in2cm">
        <xsl:with-param name="inputValue" select="@w:w div 1440"/>
      </xsl:call-template>
      <xsl:text>cm</xsl:text>
    </xsl:attribute>
    <xsl:attribute name="page-height">
      <xsl:call-template name="Convert-in2cm">
        <xsl:with-param name="inputValue" select="@w:h div 1440"/>
      </xsl:call-template>
      <xsl:text>cm</xsl:text>
    </xsl:attribute>
  </xsl:template>
  <!-- END PAGE SIZE -->

  <!-- PAGE MARGIN -->
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
            <xsl:value-of select="@w:header div 20"/>
            <xsl:text>pt</xsl:text>
          </xsl:attribute>
        </xsl:if>
        <xsl:if test="@w:footer">
          <xsl:attribute name="margin-bottom">
            <xsl:value-of select="@w:footer div 20"/>
            <xsl:text>pt</xsl:text>
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
                <xsl:value-of select="@w:right div 20"/>
                <xsl:text>pt</xsl:text>
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
                <xsl:value-of select="@w:left div 20"/>
                <xsl:text>pt</xsl:text>
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
                    <xsl:value-of select="@w:top div 20"/>
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
                    <xsl:value-of select="@w:bottom div 20"/>
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
  <!-- END PAGE MARGIN -->


  <!-- IMPLEMENTATION -->
  <!-- an optional <fo:declarations> object -->
  <xsl:template name="Include-declarations-object">
    <xsl:comment>an optional &lt;fo:declarations&gt; object </xsl:comment>
    <!--
    <fo:declarations>
      Contents:(color-profile)+
    </fo:declarations>
    -->
    <!-- Include-declarations-object -->
  </xsl:template>

  <!-- IMPLEMENTATION -->
  <!-- one or more <fo:page-sequence> objects -->
  <xsl:template name="Include-page-sequencese">

    <xsl:param name="master-reference-name"/>

    <xsl:element name="fo:page-sequence">
      <xsl:attribute name="master-reference">
        <xsl:value-of select="$master-reference-name"/>
      </xsl:attribute>

      <xsl:comment>an optional &lt;fo:title&gt; object</xsl:comment>
      <xsl:comment>an optional &lt;fo:static-content&gt; object</xsl:comment>


      <xsl:element name="fo:flow">
        <xsl:attribute name="flow-name">
          <xsl:value-of select="'xsl-region-body'"/>
        </xsl:attribute>
        <xsl:call-template name="SimpleProcessDocument"/>
      </xsl:element>

    </xsl:element>
  </xsl:template>


  <xsl:template name="SimpleProcessDocument" match="oox:part">
    <xsl:apply-templates select="key('Part', 'word/document.xml')/w:document/w:body"/>
    <!--    <xsl:apply-templates select="oox:package/oox:part[@oox:name='word/document.xml']/w:document/w:body"/>-->
  </xsl:template>

  <!-- PROCESS DOCUMENT -->
  <!--<xsl:template name="SimpleProcessDocument" match="key('Part', 'word/document.xml')">-->
  <xsl:template name="SimpleProcessDocumentImpl" match="w:body">
    
    <xsl:for-each select="w:p">

      <xsl:comment>
        <xsl:text>&lt;</xsl:text>
        <xsl:value-of select="name(.)"/>
        <xsl:text>&gt;</xsl:text>
      </xsl:comment>

      <xsl:element name="fo:block">

        <xsl:call-template name="ImplementProperties">
          <xsl:with-param name="propertiesTagName" select="w:pPr"/>
        </xsl:call-template>

        <xsl:for-each select="descendant::w:r">

          <xsl:element name="fo:inline">

            <xsl:call-template name="ImplementProperties">
              <xsl:with-param name="propertiesTagName" select="w:rPr"/>
            </xsl:call-template>

            <xsl:value-of select="w:t/."/>

            <!-- fo:inline -->
          </xsl:element>

        </xsl:for-each>

        <!-- fo:block -->
      </xsl:element>
      
      <xsl:comment>
        <xsl:text>&lt;/</xsl:text>
        <xsl:value-of select="name(.)"/>
        <xsl:text>&gt;</xsl:text>
      </xsl:comment>
      
    </xsl:for-each>
  </xsl:template>


  <!-- ADD PROPERTIES ATTRIBUTES -->
  <xsl:template name="ImplementProperties">
    <xsl:param name="propertiesTagName"/>

<!--    <xsl:apply-templates select="$default-rPr-style"  mode="properties-to-attrib"/>
    <xsl:apply-templates select="$default-paragraph-style"  mode="properties-to-attrib"/>-->
    <!--    <xsl:comment>
      <xsl:text>Style: </xsl:text>
      <xsl:value-of select="w:pPr/w:pStyle/@w:val"/>
    </xsl:comment> -->

    <xsl:apply-templates select="key('styles', w:pPr/w:pStyle/@w:val)[@w:type='paragraph']/w:rPr" mode="properties-to-attrib"/>
    <xsl:apply-templates select="key('styles', w:pPr/w:pStyle/@w:val)[@w:type='paragraph']/w:pPr" mode="properties-to-attrib"/>
    
    <!-- <xsl:apply-templates select="key('styles', w:pPr/w:pStyle/@w:val)[@w:type='paragraph']"/> -->
    <xsl:apply-templates select="$propertiesTagName" mode="properties-to-attrib"/>
    
  </xsl:template>

  <!-- w:pageBreakBefore -> break-before -->
  <xsl:template match="w:pageBreakBefore" mode="properties-to-attrib">
    <xsl:attribute name="break-before">
      <xsl:choose>
        <xsl:when test="@w:val='off'">
          <xsl:text>auto</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>page</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:attribute>
  </xsl:template>
  
  <!-- w:jc -> text-align -->
  <xsl:template match="w:jc" mode="properties-to-attrib">
    <xsl:attribute name="text-align">
      <xsl:choose>
        <xsl:when test="@w:val='left'">
          <xsl:text>left</xsl:text>
        </xsl:when>
        <xsl:when test="@w:val='right'">
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
  <xsl:template match="w:sz" mode="properties-to-attrib">
    <xsl:attribute name="font-size">
      <xsl:value-of select="@w:val div 2"/>
      <xsl:text>pt</xsl:text>
    </xsl:attribute>
  </xsl:template>

  <!-- w:b -> font-weight bold -->
  <xsl:template match="w:b" mode="properties-to-attrib">
    <xsl:attribute name="font-weight">
      <xsl:choose>
        <xsl:when test="@w:val = 'off'">
          <xsl:text>normal</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>bold</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:attribute>
  </xsl:template>

  <!-- w:i -> font-style italic -->
  <xsl:template match="w:i" mode="properties-to-attrib">
    <xsl:attribute name="font-style">
      <xsl:choose>
        <xsl:when test="@w:val = 'off'">
          <xsl:text>normal</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>italic</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:attribute>
  </xsl:template>

  <!--
  <xsl:template match="w:shadow" mode="properties-to-attrib">
    <xsl:attribute name="text-shadow">
      <xsl:choose>
        <xsl:when test="@w:val = 'off'">
          <xsl:text>none</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>black</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:attribute>
  </xsl:template>
  -->

  <!-- w:caps ->  text-transform uppercase -->
  <xsl:template match="w:caps" mode="properties-to-attrib">
    <xsl:attribute name="text-transform">
      <xsl:choose>
        <xsl:when test="@w:val = 'off'">
          <xsl:text>none</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>uppercase</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:attribute>
  </xsl:template>

  <!-- w:strike -> text-decoration line-through -->
  <xsl:template match="w:strike" mode="properties-to-attrib">
    <xsl:attribute name="text-decoration">
      <xsl:choose>
        <xsl:when test="@w:val = 'off'">
          <xsl:text>none</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>line-through</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:attribute>
  </xsl:template>

  <!-- w:dstrike -> text-decoration line-through -->
  <xsl:template match="w:dstrike" mode="properties-to-attrib">
    <xsl:attribute name="text-decoration">
      <xsl:choose>
        <xsl:when test="@w:val = 'off'">
          <xsl:text>none</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>line-through</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:attribute>
  </xsl:template>

  <!-- w:u -> text-decoration underline -->
  <xsl:template match="w:u" mode="properties-to-attrib">
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
  </xsl:template>

  <xsl:template match="w:rFonts" mode="properties-to-attrib">
    <xsl:choose>
      <xsl:when test="@w:ascii">
        <xsl:attribute name="font-family">

          <!--
          <xsl:call-template name="NormalizeFontFamilyName">
            <xsl:with-param name="font-family" select="@w:ascii"/>
          </xsl:call-template>
          -->
         
          <xsl:value-of select="@w:ascii"/>

        </xsl:attribute>
      </xsl:when>
      <xsl:otherwise/>
    </xsl:choose>
  </xsl:template>


  <!-- w:vertAlign -> baseline-shift and font-size -->
  <xsl:template match="w:vertAlign" mode="properties-to-attrib">

    <xsl:if test="name(../../.)!='w:pPr'">
      <!-- непонятно почему это потребовалось, у свойств параграфа стояло w:wertAlign='superscript', хотя вроде бы не должно быть -->
      <!-- TODO: разобраться почему так -->

      <xsl:choose>
        <xsl:when test="@w:val='superscript'">
          <xsl:attribute name="baseline-shift">
            <xsl:text>super</xsl:text>
          </xsl:attribute>
          <xsl:attribute name="font-size">
            <xsl:text>66.7%</xsl:text>
          </xsl:attribute>
        </xsl:when>
        <xsl:when test="@w:val='subscript'">
          <xsl:attribute name="baseline-shift">
            <xsl:text>sub</xsl:text>
          </xsl:attribute>
          <xsl:attribute name="font-size">
            <xsl:text>66.7%</xsl:text>
          </xsl:attribute>
        </xsl:when>
        <xsl:otherwise/>
      </xsl:choose>
      
    </xsl:if>
    
  </xsl:template>

  <!-- w:position -> baseline-shift -->
  <xsl:template match="w:position" mode="properties-to-attrib">
    <xsl:attribute name="baseline-shift">
      <xsl:value-of select="@w:val div 2"/>
      <xsl:text>pt</xsl:text>
    </xsl:attribute>
  </xsl:template>

  <xsl:template match="w:lang">
    <xsl:attribute name="language">
      <xsl:value-of select="@w:val"/>
    </xsl:attribute>
  </xsl:template>


  <!-- COLOR -->

  <!-- w:color -> color -->
  <xsl:template match="w:color" mode="properties-to-attrib">
    <xsl:attribute name="color">
      <xsl:call-template name="ConvertHexColor">
        <xsl:with-param name="hexColor" select="@w:val"/>
      </xsl:call-template>
    </xsl:attribute>
  </xsl:template>


  <!-- w:highlight -> background-color -->
  <xsl:template match="w:highlight" mode="properties-to-attrib">
    
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
    <xsl:param name="mode"/>
    <!-- block-level properties:                 -->
    <!-- space-before/space-after or line-height -->
    <!-- Parent element: w:pPr.                  -->
    <xsl:if test="@w:before and not($mode='after')">
      <xsl:attribute name="space-before">
        <xsl:value-of select="@w:before div 20"/>
        <xsl:text>pt</xsl:text>
      </xsl:attribute>
      <xsl:if test="not(@w:before-autospacing) or @w:before-autospacing='off'">
        <xsl:attribute name="space-before.conditionality">
          <xsl:text>retain</xsl:text>
        </xsl:attribute>
      </xsl:if>
    </xsl:if>
    <xsl:if test="@w:before-lines and not($mode='after')">
      <xsl:attribute name="space-before">
        <xsl:value-of select="@w:before-lines *.12"/>
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
        <xsl:value-of select="@w:after div 20"/>
        <xsl:text>pt</xsl:text>
      </xsl:attribute>
      <xsl:if test="not(@w:after-autospacing) or @w:before-autospacing='off'">
        <xsl:attribute name="space-after.conditionality">
          <xsl:text>retain</xsl:text>
        </xsl:attribute>
      </xsl:if>
    </xsl:if>
    <xsl:if test="@w:after-lines and not($mode='before')">
      <xsl:attribute name="space-after">
        <xsl:value-of select="@w:after-lines *.12"/>
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
        <xsl:value-of select="@w:val div 20"/>
        <xsl:text>pt</xsl:text>
      </xsl:attribute>
    </xsl:if>
  </xsl:template>

  <!-- ********************************************** -->
  <!-- ********************************************** -->  
  <!--             ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ            -->
  <!-- ********************************************** -->
  <!-- ********************************************** -->

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

  <!-- удаляем пробельные символы из имени шрифта -->
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

  <xsl:template name="NormalizeColorName">
   
    <xsl:param name = "colorName"/>
    
    <xsl:choose>
      <xsl:when test="$colorName='black'">
        <xsl:text>black</xsl:text>
      </xsl:when>
      <xsl:when test="$colorName='blue'">
        <xsl:text>blue</xsl:text>
      </xsl:when>
      <xsl:when test="$colorName='cyan'">
        <xsl:text>aqua</xsl:text>
      </xsl:when>
      <xsl:when test="$colorName='green'">
        <xsl:text>lime</xsl:text>
      </xsl:when>
      <xsl:when test="$colorName='magenta'">
        <xsl:text>fuchsia</xsl:text>
      </xsl:when>
      <xsl:when test="$colorName='red'">
        <xsl:text>red</xsl:text>
      </xsl:when>
      <xsl:when test="$colorName='yellow'">
        <xsl:text>yellow</xsl:text>
      </xsl:when>
      <xsl:when test="$colorName='white'">
        <xsl:text>white</xsl:text>
      </xsl:when>
      <xsl:when test="$colorName='darkblue'">
        <xsl:text>navy</xsl:text>
      </xsl:when>
      <xsl:when test="$colorName='darkcyan'">
        <xsl:text>teal</xsl:text>
      </xsl:when>
      <xsl:when test="$colorName='darkgreen'">
        <xsl:text>green</xsl:text>
      </xsl:when>
      <xsl:when test="$colorName='darkmagenta'">
        <xsl:text>purple</xsl:text>
      </xsl:when>
      <xsl:when test="$colorName='darkred'">
        <xsl:text>maroon</xsl:text>
      </xsl:when>
      <xsl:when test="$colorName='darkyellow'">
        <xsl:text>olive</xsl:text>
      </xsl:when>
      <xsl:when test="$colorName='darkgrey'">
        <xsl:text>gray</xsl:text>
      </xsl:when>
      <xsl:when test="$colorName='lightgrey'">
        <xsl:text>silver</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:text>transparent</xsl:text>
        <!--<xsl:value-of select="$colorName"/>-->
      </xsl:otherwise>
    </xsl:choose>
    
    <!-- NormalizeColorName -->
  </xsl:template>

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
      <xsl:otherwise>
        <xsl:value-of select="concat('#', $hexColor)"/>
      </xsl:otherwise>
    </xsl:choose>
    
    <!-- ConvertHexColor -->
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



  <!-- PROCESS DOCUMENT -->
  <!-- <xsl:template name="ProcessDocument">
    <xsl:value-of select = "."/>
  </xsl:template>
  -->


  <!--
  <xsl:output method="xml" indent="yes"/>
  <xsl:template match="@* | node()">
    <xsl:copy>
    
      <xsl:apply-templates select="@* | node()"/>
    </xsl:copy>
  </xsl:template>
  -->

</xsl:stylesheet>
