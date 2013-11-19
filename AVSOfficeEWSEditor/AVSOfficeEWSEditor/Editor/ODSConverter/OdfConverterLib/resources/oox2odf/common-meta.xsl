<?xml version="1.0" encoding="UTF-8"?>
<?xml-stylesheet href="xsl2html.xsl" type="text/xsl" media="screen"?>

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
    xmlns:dc="http://purl.org/dc/elements/1.1/" xmlns:dcterms="http://purl.org/dc/terms/"
    xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
    xmlns:meta="urn:oasis:names:tc:opendocument:xmlns:meta:1.0"
    xmlns="http://schemas.openxmlformats.org/officeDocument/2006/extended-properties"
    xmlns:ex="http://schemas.openxmlformats.org/officeDocument/2006/extended-properties"
    xmlns:vt="http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes"
    xmlns:cp="http://schemas.openxmlformats.org/package/2006/metadata/core-properties"
    xmlns:cust-p="http://schemas.openxmlformats.org/officeDocument/2006/custom-properties"
    exclude-result-prefixes="vt cust-p cp ex">

  <!-- @Filename: common-meta.xsl -->
  <!-- @Description: This stylesheet is used to perform reverse conversion on document metadata -->
  <!-- @Created: 2006-12-12 -->

  <xsl:template name="meta">
    <!-- @Description: Generates the openOffice element containing the document metadata. -->
    <!-- @Context: Any -->

    <xsl:param name="generator"/>
    <xsl:param name="app-version"/>
    <!-- The application version -->

    <office:document-meta office:version="1.1">
      <office:meta>
        <!-- generator -->
        <meta:generator>
          <xsl:value-of select="$generator"/>
        </meta:generator>
        <!-- title -->
        <xsl:apply-templates
            select="document('docProps/core.xml')/cp:coreProperties/dc:title" mode="meta"/>
        <!-- description -->
        <xsl:apply-templates
            select="document('docProps/core.xml')/cp:coreProperties/dc:description"
            mode="meta"/>
        <!-- creator -->
        <xsl:apply-templates
            select="document('docProps/core.xml')/cp:coreProperties/dc:creator" mode="meta"/>
        <!-- creation date -->
        <xsl:apply-templates
            select="document('docProps/core.xml')/cp:coreProperties/dcterms:created"
            mode="meta"/>
        <!-- last modifier -->
        <xsl:apply-templates
            select="document('docProps/core.xml')/cp:coreProperties/cp:lastModifiedBy"
            mode="meta"/>
        <!-- last modification date -->
        <xsl:apply-templates
            select="document('docProps/core.xml')/cp:coreProperties/dcterms:modified"
            mode="meta"/>
        <!-- last print date -->
        <xsl:apply-templates
            select="document('docProps/core.xml')/cp:coreProperties/cp:lastPrinted"
            mode="meta"/>
        <!-- subject -->
        <xsl:apply-templates
            select="document('docProps/core.xml')/cp:coreProperties/dc:subject" mode="meta"/>
        <!-- editing time -->
        <xsl:apply-templates select="document('docProps/core.xml')/Properties/TotalTime"
            mode="meta"/>
        <!-- revision number -->
        <xsl:apply-templates
            select="document('docProps/core.xml')/cp:coreProperties/cp:revision" mode="meta"/>
        <!-- custom properties-->
        <xsl:apply-templates
            select="document('docProps/custom.xml')/cust-p:Properties/cust-p:property"
            mode="meta"/>
        <!-- keywords -->
        <xsl:apply-templates
            select="document('docProps/core.xml')/cp:coreProperties/cp:keywords" mode="meta"/>
        <!-- language -->
        <xsl:apply-templates
            select="document('docProps/core.xml')/cp:coreProperties/dc:language" mode="meta"/>
        <!-- category -->
        <xsl:apply-templates
            select="document('docProps/core.xml')/cp:coreProperties/cp:category" mode="meta"/>
        <!-- content status -->
        <xsl:apply-templates
                    select="document('docProps/core.xml')/cp:coreProperties/cp:contentStatus" mode="meta"/>
        <!-- document statistics -->
        <xsl:call-template name="GetDocumentStatistics"/>
        <!-- Total editing time and other app properties -->
        <xsl:apply-templates
					select="document('docProps/app.xml')/ex:Properties/*" mode="meta"/>
      </office:meta>
    </office:document-meta>
  </xsl:template>


  <xsl:template match="dc:title" mode="meta">
    <!-- @Description: Generates the &lt;dc:title&gt; element (last modification date) -->
    <!-- @Context: Any &lt;dc:title&gt; element -->
    <!-- @Private -->

    <dc:title>
      <xsl:value-of select="substring(., 1, 65533)"/>
    </dc:title>
  </xsl:template>


  <xsl:template match="dc:description" mode="meta">
    <!-- @Description: Generates the &lt;dc:description&gt; element (last modification date) -->
    <!-- @Context: Any &lt;dc:description&gt; element -->
    <!-- @Private -->

    <dc:description>
      <xsl:value-of select="substring(., 1, 65533)"/>
    </dc:description>
  </xsl:template>


  <xsl:template match="dc:creator" mode="meta">
    <!-- @Description: Generates the &lt;meta:initial-creator&gt; element (last modification date) -->
    <!-- @Context: Any &lt;dc:creator&gt; element -->
    <!-- @Private -->

    <meta:initial-creator>
      <xsl:value-of select="."/>
    </meta:initial-creator>
  </xsl:template>


  <xsl:template match="dcterms:created" mode="meta">
    <!-- @Description: Generates the &lt;meta:creation-date&gt; element (last modification date) -->
    <!-- @Context: Any &lt;dcterms:created&gt; element -->
    <!-- @Private -->

	  <meta:creation-date>
		  <xsl:value-of select="."/>
	  </meta:creation-date>
  </xsl:template>


  <xsl:template match="cp:lastModifiedBy" mode="meta">
    <!-- @Description: Generates the &lt;dc:creator&gt; element (last modification date) -->
    <!-- @Context: Any &lt;cp:lastModifiedBy&gt; element -->
    <!-- @Private -->
	
    <dc:creator>
      <xsl:value-of select="."/>
    </dc:creator>
  </xsl:template>


  <xsl:template match="dcterms:modified" mode="meta">
    <!-- @Description: Generates the &lt;dc:date&gt; element (last modification date) -->
    <!-- @Context: Any &lt;dcterms:modified&gt; element -->
    <!-- @Private -->

    <dc:date>
      <xsl:value-of select="."/>
    </dc:date>
  </xsl:template>

  <xsl:template match="cp:lastPrinted" mode="meta">
    <!-- @Description: Generates the &lt;meta:print-date&gt; element -->
    <!-- @Context: Any &lt;cp:lastPrinted&gt; element -->
    <!-- @Private -->

    <meta:print-date>
      <xsl:value-of select="."/>
    </meta:print-date>
  </xsl:template>

  <xsl:template match="dc:subject" mode="meta">
    <!-- @Description: Generates the &lt;dc:subject&gt; element -->
    <!-- @Context: Any  &lt;dc:subject&gt; element -->
    <!-- @Private -->

    <dc:subject>
      <xsl:value-of select="substring(., 1, 65533)"/>
    </dc:subject>
  </xsl:template>

  <xsl:template match="TotalTime|ex:TotalTime" mode="meta">
    <!-- @Description: Generates the &lt;meta:editing-duration&gt; element -->
    <!-- @Context: Any &lt;TotalTime&gt; element-->
    <!-- @Private -->

    <meta:editing-duration>
      <xsl:choose>
        <xsl:when test=". &gt; 60">
          <xsl:text>PT</xsl:text>
          <xsl:variable name="hours" select="round(.) div 60"/>
          <xsl:value-of select="$hours"/>
          <xsl:text>H</xsl:text>
          <xsl:value-of select="number(.) - (number($hours) * 60)"/>
          <xsl:text>M</xsl:text>
          <xsl:text>0S</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>PT</xsl:text>
          <xsl:text>0H</xsl:text>
          <xsl:value-of select="."/>
          <xsl:text>M</xsl:text>
          <xsl:text>0S</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </meta:editing-duration>
  </xsl:template>

  <xsl:template match="ex:Manager" mode="meta">
    <!-- @Description: preserve property Manager as user-defined property -->
    <!-- @Context: Any &lt;Manager&gt; element-->

    <meta:user-defined meta:name="Manager" meta:value-type="string">
      <xsl:value-of select="text()"/>
    </meta:user-defined>
  </xsl:template>

  <xsl:template match="ex:Company" mode="meta">
    <!-- @Description: preserve property Manager as user-defined property -->
    <!-- @Context: Any &lt;Manager&gt; element-->

    <meta:user-defined meta:name="Company" meta:value-type="string">
      <xsl:value-of select="text()"/>
    </meta:user-defined>
  </xsl:template>

  <xsl:template match="ex:HyperlinkBase" mode="meta">
    <!-- @Description: preserve property Manager as user-defined property -->
    <!-- @Context: Any &lt;Manager&gt; element-->

    <meta:user-defined meta:name="HyperlinkBase" meta:value-type="string">
      <xsl:value-of select="text()"/>
    </meta:user-defined>
  </xsl:template>

  <xsl:template match="ex:*" mode="meta">
    <!-- by default do not retain any element -->
  </xsl:template>

  <xsl:template match="cp:revision" mode="meta">
    <!-- @Description: Generates the &lt;meta:editing-cycles&gt; element -->
    <!-- @Context: Any &lt;cp:revision&gt; element -->
    <!-- @Private -->

    <meta:editing-cycles>
      <xsl:value-of select="."/>
    </meta:editing-cycles>
  </xsl:template>


  <xsl:template match="cust-p:property" mode="meta">
    <!-- @Description: Generates the &lt;meta:user-defined&gt; element(s) -->
    <!-- @Context: Any &lt;cust-p:property&gt; -->
    <!-- @Private -->

    <xsl:for-each select=".">
      <meta:user-defined meta:name="{@name}">
        <xsl:attribute name="meta:value-type">
          <xsl:choose>
            <xsl:when test="vt:bool">
              <xsl:text>boolean</xsl:text>
            </xsl:when>
            <xsl:when test="vt:filetime | vt:date">
              <xsl:text>date</xsl:text>
            </xsl:when>
            <xsl:when test="vt:i1 | vt:i2 | vt:i4 | vt:i8 | vt:int | vt:ui1 | vt:ui2 | vt:ui4 | vt:ui8 | vt:uint | vt:decimal">
              <!--- translate any number to a float value -->
              <xsl:text>float</xsl:text>
            </xsl:when>
            <!-- we do not write out string as type since this is the default type used by OOo.
                 OOo will always add it's default doc properties (Info1, Info2, Info3 etc) on saving
                 and we might end up with duplicate doc properties, e.g. Info1 without a type and Info1 as string
                 from multiple roundtrips. 
                 WARNING: DOCX files with non-unique names of document properties do not open in Word -->
            <xsl:when test="substring(@name, 1, 5) = 'Info ' and string-length(@name) &lt; 7">
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>string</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
        <xsl:choose>
          <xsl:when test="vt:filetime | vt:date">
            <!-- workaround for problems with date time in OOo: cut trailing 'Z' -->
            <xsl:value-of select="substring(child::node()/text(), 1, 19)"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="child::node()/text()"/>
          </xsl:otherwise>
        </xsl:choose>
      </meta:user-defined>
    </xsl:for-each>
  </xsl:template>


  <xsl:template match="cp:keywords" mode="meta">
    <!-- @Description: Generates the &lt;meta:keyword&gt; element(s) -->
    <!-- @Context: Any &lt;cp:keywords&gt; element-->
    <!-- @Private -->

    <meta:keyword>
      <xsl:value-of select="substring(., 1, 65533)"/>
    </meta:keyword>
  </xsl:template>

  <xsl:template match="cp:category" mode="meta">
    <!-- @Description: Preserve the OOXML category property as user-defined property) -->
    <!-- @Context: Any &lt;cp:category&gt; element-->

    <meta:user-defined meta:name="Category" meta:value-type="string">
      <xsl:value-of select="text()"/>
    </meta:user-defined>
  </xsl:template>

  <xsl:template match="cp:contentStatus" mode="meta">
    <!-- @Description: Preserve the OOXML contentStatus property as user-defined property) -->
    <!-- @Context: Any &lt;cp:category&gt; element-->

    <meta:user-defined meta:name="Content Status" meta:value-type="string">
      <xsl:value-of select="text()"/>
    </meta:user-defined>
  </xsl:template>

  <xsl:template match="dc:language" mode="meta">
    <!-- @Description: Generates the &lt;dc:language&gt; element -->
    <!-- @Context: Any &lt;dc:language&gt; element-->
    <!-- @Private -->

    <dc:language>
      <xsl:value-of select="."/>
    </dc:language>
  </xsl:template>

  <xsl:template name="GetDocumentStatistics">
    <!-- @Description: Generates the &lt;meta:document-statistic&gt; element -->
    <!-- @Context: Any -->
    <!-- @Private -->

    <meta:document-statistic>
      <xsl:for-each select="document('docProps/app.xml')/Properties">
        <!-- word count -->
        <xsl:if test="Word">
          <xsl:attribute name="meta:word-count">
            <xsl:value-of select="Word"/>
          </xsl:attribute>
        </xsl:if>
        <!-- page count -->
        <xsl:if test="Pages">
          <xsl:attribute name="meta:page-count">
            <xsl:value-of select="Pages"/>
          </xsl:attribute>
        </xsl:if>
        <!-- paragraph count -->
        <xsl:if test="Paragraphs">
          <xsl:attribute name="meta:paragraph-count">
            <xsl:value-of select="Paragraphs"/>
          </xsl:attribute>
        </xsl:if>
        <!-- character count -->
        <xsl:if test="Characters">
          <xsl:attribute name="meta:non-whitespace-character-count">
            <xsl:value-of select="Characters"/>
          </xsl:attribute>
        </xsl:if>
        <!-- character count -->
        <xsl:if test="CharactersWithSpaces">
          <xsl:attribute name="meta:character-count">
            <xsl:value-of select="CharactersWithSpaces"/>
          </xsl:attribute>
        </xsl:if>
      </xsl:for-each>
    </meta:document-statistic>
  </xsl:template>



  <!-- other properties to fit :
        core : category, contentStatus, contentType
        app : Application, AppVersion, Company, DigSig, DocSecurity, HeadingPairs, HiddenSlides, HLinks, HyperlinkBase, HyperlinksChanged,
        Lines, LinksUpToDate, Manager, MMClips, Notes, PresentationFormat, Properties, ScaleCrop, SharedDoc, Slides, Template, TitlesOfParts.
    -->


</xsl:stylesheet>
