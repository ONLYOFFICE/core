<?xml version="1.0" encoding="UTF-8"?>
<?xml-stylesheet href="xsl2html.xsl" type="text/xsl" media="screen"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
    xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
    xmlns:dc="http://purl.org/dc/elements/1.1/"
    xmlns:meta="urn:oasis:names:tc:opendocument:xmlns:meta:1.0"
    exclude-result-prefixes="office meta dc">

  <!-- @Filename: common-meta.xsl -->
  <!-- @Description: This stylesheet is used to perform direct conversion on document metadata -->
  <!-- @Created: 2006-12-12 -->


  <xsl:template name="docprops-core">
    <!-- @Description:  Produce core document properties -->
    <!-- @Context: Any -->
    <cp:coreProperties
        xmlns:cp="http://schemas.openxmlformats.org/package/2006/metadata/core-properties"
        xmlns:dcmitype="http://purl.org/dc/dcmitype/">
      <xsl:apply-templates select="document('meta.xml')/office:document-meta/office:meta" mode="core" />
    </cp:coreProperties>
  </xsl:template>

  <xsl:template name="docprops-custom">
    <!-- @Description: Custom document properties -->
    <!-- @Context: Any -->
    <xsl:apply-templates select="document('meta.xml')/office:document-meta/office:meta" mode="custom" />
  </xsl:template>

  <xsl:template match="/office:document-meta/office:meta" mode="core">
    <!-- @Private -->
    <!-- @Description: Core meta data -->
    <!-- @Context: Any -->
    <!-- report lost properties -->
    <xsl:apply-templates select="meta:auto-reload" mode="core" />
    <xsl:apply-templates select="meta:hyperlink-behaviour" mode="core" />
    <!-- creation date -->
    <xsl:apply-templates select="meta:creation-date" mode="core" />
    <!-- creator -->
    <xsl:apply-templates select="meta:initial-creator" mode="core" />
    <!-- description -->
    <xsl:apply-templates select="dc:description" mode="core" />
    <!-- identifier -->
    <xsl:apply-templates select="dc:identifier" mode="core" />
    <!-- keywords -->
    <xsl:call-template name="MetaKeywords" />
    <!-- language -->
    <xsl:apply-templates select="dc:language" mode="core" />
    <!-- last modification author -->
    <xsl:apply-templates select="dc:creator" mode="core" />
    <!-- last printing -->
    <xsl:apply-templates select="meta:printed-date" mode="core" />
    <!-- last modification date -->
    <xsl:apply-templates select="dc:date" mode="core" />
    <!-- number of times it was saved -->
    <xsl:apply-templates select="meta:editing-cycles" mode="core" />
    <!-- topic -->
    <xsl:apply-templates select="dc:subject" mode="core" />
    <!-- title -->
    <xsl:apply-templates select="dc:title" mode="core" />
    <!-- category -->
    <xsl:apply-templates select="meta:user-defined[@meta:name = 'Category']" mode="core" />
    <!-- content status -->
    <xsl:apply-templates select="meta:user-defined[@meta:name = 'Content Status']" mode="core" />
  </xsl:template>

  <xsl:template match="/office:document-meta/office:meta" mode="custom">
    <!-- @Description: Produce custom properties -->
    <Properties xmlns="http://schemas.openxmlformats.org/officeDocument/2006/custom-properties"
        xmlns:vt="http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes">
      <xsl:apply-templates select="meta:user-defined" />
    </Properties>
  </xsl:template>

  <!-- creation date -->
  <xsl:template match="meta:creation-date" mode="core">
    <!-- @Description: creation date -->
    <xsl:variable name="dateIsValid">
      <xsl:call-template name="validateDate">
        <xsl:with-param name="date" select="." />
      </xsl:call-template>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="$dateIsValid != 'false' ">
        <dcterms:created xmlns:dcterms="http://purl.org/dc/terms/" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:type="dcterms:W3CDTF">
			<!--<xsl:value-of select="$dateIsValid" />-->
			<!-- date is parsed again in post-processor -->
			<xsl:value-of select="." />
        </dcterms:created>
      </xsl:when>
      <xsl:otherwise>
        <xsl:message terminate="no">translation.odf2oox.documentCreationDate</xsl:message>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- initial creator -->
  <xsl:template match="meta:initial-creator" mode="core">
    <!-- @Description: Initial creator -->
    <dc:creator xmlns:dc="http://purl.org/dc/elements/1.1/">
      <xsl:value-of select="." />
    </dc:creator>
  </xsl:template>

  <!-- description -->
  <xsl:template match="dc:description" mode="core">
    <!-- @Description: Description/Comment -->
    <dc:description xmlns:dc="http://purl.org/dc/elements/1.1/">
      <xsl:value-of select="." />
    </dc:description>
  </xsl:template>

  <!-- identifier -->
  <xsl:template match="dc:identifier" mode="core">
    <!-- @Description: Identifier -->
    <dc:identifier xmlns:dc="http://purl.org/dc/elements/1.1/">
      <xsl:value-of select="." />
    </dc:identifier>
  </xsl:template>

  <!-- keywords -->
  <xsl:template name="MetaKeywords">
    <!-- @Description: Keywords meta -->
    <!-- @Context: /office:document-meta/office:meta -->
    <xsl:if test="/office:document-meta/office:meta/meta:keyword">
      <cp:keywords xmlns:cp="http://schemas.openxmlformats.org/package/2006/metadata/core-properties">
        <xsl:for-each select="/office:document-meta/office:meta/meta:keyword">
          <xsl:if test="not(position() = 1)">
            <xsl:text> </xsl:text>
          </xsl:if>
          <xsl:value-of select="." />
        </xsl:for-each>
      </cp:keywords>
    </xsl:if>
  </xsl:template>

  <!-- language -->
  <xsl:template match="dc:language" mode="core">
    <!-- @Description: Language -->
    <dc:language xmlns:dc="http://purl.org/dc/elements/1.1/">
      <xsl:value-of select="." />
    </dc:language>
  </xsl:template>

  <!-- last modification author -->
  <xsl:template match="dc:creator" mode="core">
    <!-- @Description: Author/Creator -->
    <cp:lastModifiedBy xmlns:cp="http://schemas.openxmlformats.org/package/2006/metadata/core-properties">
      <xsl:value-of select="." />
    </cp:lastModifiedBy>
  </xsl:template>

  <!-- last printing -->
  <xsl:template match="meta:printed-date" mode="core">
    <!-- @Description: Print date -->
    <cp:lastPrinted xmlns:cp="http://schemas.openxmlformats.org/package/2006/metadata/core-properties">
      <xsl:value-of select="." />
    </cp:lastPrinted>
  </xsl:template>

  <!-- last modification date -->
  <xsl:template match="dc:date" mode="core">
    <!-- @Description: Last modification date -->
    <xsl:variable name="dateIsValid">
      <xsl:call-template name="validateDate">
        <xsl:with-param name="date" select="." />
      </xsl:call-template>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="$dateIsValid != 'false' ">
        <dcterms:modified xmlns:dcterms="http://purl.org/dc/terms/" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:type="dcterms:W3CDTF">
			<!--<xsl:value-of select="$dateIsValid" />-->
			<!-- date is parsed again in post-processor -->
			<xsl:value-of select="." />
        </dcterms:modified>
      </xsl:when>
      <xsl:otherwise>
        <xsl:message terminate="no">translation.odf2oox.documentModificationDate</xsl:message>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- number of times it was saved -->
  <xsl:template match="meta:editing-cycles" mode="core">
    <!-- @Description: Number of times the document was saved (Editing cycles) -->
    <cp:revision xmlns:cp="http://schemas.openxmlformats.org/package/2006/metadata/core-properties">
      <xsl:value-of select="." />
    </cp:revision>
  </xsl:template>

  <!-- topic -->
  <xsl:template match="dc:subject" mode="core">
    <!-- @Description: Topic/Subject -->
    <dc:subject xmlns:dc="http://purl.org/dc/elements/1.1/">
      <xsl:value-of select="." />
    </dc:subject>
  </xsl:template>

  <!-- title -->
  <xsl:template match="dc:title" mode="core">
    <!-- @Description: Title -->
    <dc:title xmlns:dc="http://purl.org/dc/elements/1.1/">
      <xsl:value-of select="." />
    </dc:title>
  </xsl:template>

  <xsl:template match="meta:user-defined[@meta:name = 'Category']" mode="core">
    <!-- @Description: Category -->
    <cp:category xmlns:cp="http://schemas.openxmlformats.org/package/2006/metadata/core-properties">
      <xsl:value-of select="." />
    </cp:category>
  </xsl:template>

  <xsl:template match="meta:user-defined[@meta:name = 'Content Status']" mode="core">
    <!-- @Description: Content Status -->
    <cp:contentStatus xmlns:cp="http://schemas.openxmlformats.org/package/2006/metadata/core-properties">
      <xsl:value-of select="." />
    </cp:contentStatus>
  </xsl:template>

  <!-- report lost properties -->
  <xsl:template match="meta:auto-reload" mode="core">
    <!-- @Description: Auto reload property -->
    <xsl:message terminate="no">translation.odf2oox.internetProperties</xsl:message>
  </xsl:template>

  <xsl:template match="meta:hyperlink-behaviour" mode="core">
    <!-- @Description: Hyperlink behaviour -->
    <xsl:message terminate="no">translation.odf2oox.internetProperties</xsl:message>
  </xsl:template>

  <!-- user defined properties -->
  <xsl:template match="/office:document-meta/office:meta/meta:user-defined[@meta:name != '' 
							and @meta:name != 'Company' and @meta:name != 'Manager' and @meta:name != 'HyperlinkBase' 
							and @meta:name != 'Category' and @meta:name != 'Content Status']">
    <!-- @Description: User defined properties -->
    <!-- NOTE: the properties Company, Manager and HyperlinkBase are not translated into custom.xml but into app.xml
				and the properties category and contentStatus are translated into core.xml -->

    <!-- check for unique names 
         WARNING: DOCX files with non-unique names of document properties do not open in Word,
         therefore any duplicates are discarded (a future version could create unique names, but that is hard in XSLT)
    -->
    <xsl:variable name="current" select="." />
    <xsl:variable name="propName" select="@meta:name" />
    
    <xsl:if test="generate-id(.) = generate-id(/office:document-meta/office:meta/meta:user-defined[@meta:name = $propName][1])">
      <property xmlns="http://schemas.openxmlformats.org/officeDocument/2006/custom-properties"
          xmlns:vt="http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes">
        <xsl:attribute name="fmtid">{D5CDD505-2E9C-101B-9397-08002B2CF9AE}</xsl:attribute>
        <xsl:attribute name="pid">
          <xsl:value-of select="position() + 1" />
        </xsl:attribute>
        <xsl:attribute name="name">
          <xsl:value-of select="@meta:name" />
        </xsl:attribute>
        <xsl:choose>
          <xsl:when test="@meta:value-type='boolean'">
            <vt:bool>
              <xsl:value-of select="text()" />
            </vt:bool>
          </xsl:when>
          <xsl:when test="@meta:value-type='date' 
						  and number(substring(text(), 1, 4)) != 'NaN'
						  and substring(text(), 5, 1) = '-'
						  and number(substring(text(), 6, 2)) != 'NaN'
						  and substring(text(), 8, 1) = '-'
						  and number(substring(text(), 9, 2)) != 'NaN'
						  and substring(text(), 11, 1) = 'T'
						  and number(substring(text(), 12, 2)) != 'NaN'
						  and substring(text(), 14, 1) = ':'
						  and number(substring(text(), 15, 2)) != 'NaN'
						  and substring(text(), 17, 1) = ':'
						  and number(substring(text(), 18, 2)) != 'NaN'">
            <!-- filetime values must be in the format yyyy-mm-ddThh:mm:ss -->
            <vt:filetime>
              <xsl:value-of select="text()" />
            </vt:filetime>
          </xsl:when>
          <xsl:when test="@meta:value-type='float'">
            <!-- 8byte real values are created for any number -->
            <vt:r8>
              <xsl:value-of select="text()" />
            </vt:r8>
          </xsl:when>
          <xsl:otherwise>
            <vt:lpwstr>
              <xsl:value-of select="text()" />
            </vt:lpwstr>
          </xsl:otherwise>
        </xsl:choose>
      </property>
    </xsl:if>
  </xsl:template>

  <!-- page count statistics extended property -->
  <xsl:template match="@meta:page-count">
    <!-- @Description: Page statistics -->
    <Pages xmlns="http://schemas.openxmlformats.org/officeDocument/2006/extended-properties">
      <xsl:value-of select="." />
    </Pages>
  </xsl:template>

  <!-- word count statistics extended property -->
  <xsl:template match="@meta:word-count">
    <!-- @Description: Word statistics -->
    <Words xmlns="http://schemas.openxmlformats.org/officeDocument/2006/extended-properties">
      <xsl:value-of select="." />
    </Words>
  </xsl:template>

  <!-- application extended property -->
  <xsl:template name="GetApplicationExtendedProperty">
    <!-- @Description: Application extended properties -->
    <!-- @Context: Any -->
    <xsl:param name="generator" />
    <Application
        xmlns="http://schemas.openxmlformats.org/officeDocument/2006/extended-properties">
      <xsl:value-of select="$generator" />
    </Application>
  </xsl:template>

  <!-- document security extended property -->
  <xsl:template name="GetDocSecurityExtendedProperty">
    <!-- @Description: Document security extended property -->
    <!-- @Context: Any -->
    <DocSecurity
        xmlns="http://schemas.openxmlformats.org/officeDocument/2006/extended-properties"
        >0</DocSecurity>
  </xsl:template>

  <!-- paragraphs statistics extended property -->
  <xsl:template match="@meta:paragraph-count">
    <!-- @Description: Paragraph statistics -->
    <Paragraphs
        xmlns="http://schemas.openxmlformats.org/officeDocument/2006/extended-properties">
      <xsl:value-of select="." />
    </Paragraphs>
  </xsl:template>


  <!--  editing duration -->
  <xsl:template match="meta:editing-duration">
    <!-- @Description: Editing duration -->
    <xsl:variable name="hours">
      <xsl:choose>
        <xsl:when test="contains(., 'H')">
          <xsl:value-of select="substring-before(substring-after(., 'T'), 'H')" />
        </xsl:when>
        <xsl:otherwise>0</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="minutes">
      <xsl:choose>
        <xsl:when test="contains(., 'H')">
          <xsl:value-of select="substring-before(substring-after(., 'H'), 'M')" />
        </xsl:when>
        <xsl:when test="contains(., 'M')">
          <xsl:value-of select="substring-before(substring-after(., 'T'), 'M')" />
        </xsl:when>
        <xsl:otherwise>0</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="seconds">
      <xsl:choose>
        <xsl:when test="contains(., 'M')">
          <xsl:value-of select="substring-before(substring-after(., 'M'), 'S')" />
        </xsl:when>
        <xsl:when test="contains(., 'S')">
          <xsl:value-of select="substring-before(substring-after(., 'T'), 'S')" />
        </xsl:when>
        <xsl:otherwise>0</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <TotalTime xmlns="http://schemas.openxmlformats.org/officeDocument/2006/extended-properties">
      <xsl:choose>
        <xsl:when test="number($seconds) &gt; 30">
          <xsl:value-of select="60 * number($hours) + number($minutes) + 1" />
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="60 * number($hours) + number($minutes)" />
        </xsl:otherwise>
      </xsl:choose>
    </TotalTime>
  </xsl:template>


  <!-- characters with spaces statistics -->
  <xsl:template match="@meta:character-count">
    <!-- @Description: Characters statistics -->
    <CharactersWithSpaces xmlns="http://schemas.openxmlformats.org/officeDocument/2006/extended-properties">
      <xsl:value-of select="." />
    </CharactersWithSpaces>
  </xsl:template>

  <!-- non whitespace character count statistics -->
  <xsl:template match="@meta:non-whitespace-character-count">
    <!-- @Description: Non whitespace characters statistics -->
    <Characters xmlns="http://schemas.openxmlformats.org/officeDocument/2006/extended-properties">
      <xsl:value-of select="." />
    </Characters>
  </xsl:template>


  <!-- check if a date is valid regarding W3C format -->
  <xsl:template name="validateDate">
    <!-- @Description: W3C date format validation-->
    <!-- @Context: Any date (meta:creation-date, dc:date) -->
    <xsl:param name="date" />
    <!-- date parameter -->
    <!-- year -->
    <xsl:variable name="Y">
      <xsl:choose>
        <xsl:when test="contains($date, '-')">
          <xsl:value-of select="substring-before($date, '-')" />
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$date" />
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <!-- month -->
    <xsl:variable name="M">
      <xsl:variable name="date_Y" select="substring-after($date, concat($Y, '-'))" />
      <xsl:choose>
        <xsl:when test="contains($date_Y, '-')">
          <xsl:value-of select="substring-before($date_Y, '-')" />
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$date_Y" />
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <!-- day -->
    <xsl:variable name="D">
      <xsl:variable name="date_Y_m" select="substring-after($date, concat($Y, '-', $M, '-'))" />
      <xsl:choose>
        <xsl:when test="contains($date_Y_m, 'T')">
          <xsl:value-of select="substring-before($date_Y_m, 'T')" />
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$date_Y_m" />
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <!-- hour -->
    <xsl:variable name="h">
      <xsl:if test="contains($date, 'T')">
        <xsl:value-of select="substring-before(substring-after($date, 'T'), ':')" />
      </xsl:if>
    </xsl:variable>
    <!-- minutes -->
    <xsl:variable name="m">
      <xsl:if test="contains($date, 'T')">
        <xsl:variable name="time_h" select="substring-after(substring-after($date, 'T'), ':')" />
        <xsl:choose>
          <xsl:when test="contains($time_h, ':')">
            <xsl:value-of select="substring-before($time_h, ':')" />
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$time_h" />
          </xsl:otherwise>
        </xsl:choose>
      </xsl:if>
    </xsl:variable>
    <!-- seconds (with our without decimal) -->
    <xsl:variable name="s">
      <xsl:if test="contains($date, 'T')">
        <xsl:variable name="time_h_m"
            select="substring-after(substring-after($date, 'T'), concat($h, ':', $m, ':'))" />
        <xsl:choose>
          <xsl:when test="contains($time_h_m, 'Z')">
            <xsl:value-of select="substring-before($time_h_m, 'Z')" />
          </xsl:when>
          <xsl:when test="contains($time_h_m, '+')">
            <xsl:value-of select="substring-before($time_h_m, '+')" />
          </xsl:when>
		  <xsl:when test="contains($time_h_m, '-')">
			<xsl:value-of select="substring-before($time_h_m, '-')" />
		  </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$time_h_m" />
          </xsl:otherwise>
        </xsl:choose>
      </xsl:if>
    </xsl:variable>

    <!-- validate format -->
    <xsl:choose>
      <xsl:when test="string-length($Y) != 4">false</xsl:when>
      <xsl:when test="$M != '' and string-length($M) != 2">false</xsl:when>
      <xsl:when test="$D != '' and string-length($D) != 2">false</xsl:when>
      <xsl:when test="$h != '' and string-length($h) != 2">false</xsl:when>
      <xsl:when test="$m != '' and string-length($m) != 2">false</xsl:when>
      <xsl:when test="$s != '' and string-length($s) &gt; 4">false</xsl:when>
      <xsl:when test="string-length(substring-after($date, 'Z')) &gt; 0">false</xsl:when>
      <xsl:when test="$M != '' and number($M) &gt; 12">false</xsl:when>
      <xsl:when test="$M != '' and number($M) &lt; 1">false</xsl:when>
      <xsl:when test="$D != '' and number($D) &gt; 31">false</xsl:when>
      <xsl:when test="$D != '' and number($D) &lt; 1">false</xsl:when>
      <xsl:when test="$h != '' and number($h) &gt; 24">false</xsl:when>
      <xsl:when test="$h != '' and number($h) &lt; 0">false</xsl:when>
      <xsl:when test="$m != '' and number($m) &gt; 60">false</xsl:when>
      <xsl:when test="$m != '' and number($m) &lt; 0">false</xsl:when>
      <xsl:when test="$s != '' and number($s) &gt; 60">false</xsl:when>
      <xsl:when test="$s != '' and number($s) &lt; 0">false</xsl:when>
      <xsl:when test="contains($date, 'T') and not(contains($date, 'Z') or contains($date, '+'))">
        <xsl:value-of select="concat($date, 'Z')" />
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$date" />
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

</xsl:stylesheet>
