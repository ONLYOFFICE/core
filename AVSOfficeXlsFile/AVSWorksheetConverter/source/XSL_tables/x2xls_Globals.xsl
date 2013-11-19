<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:xlsx="http://www.avs4you.ru/XLSXConverter/1.0/DirectoryStructure"
                xmlns:r="http://schemas.openxmlformats.org/package/2006/relationships"
                xmlns:mainr="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
                xmlns:main="http://schemas.openxmlformats.org/spreadsheetml/2006/main"
                xmlns:msxsl="urn:schemas-microsoft-com:xslt"
                xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main"
                extension-element-prefixes="msxsl">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>


  <xsl:template name="GlobalsSubstream">
    <GlobalsSubstream>
      <BOF vers="1536" dt="5" rupBuild="5612" rupYear="1997" fWin="true" fRisc="false" fBeta="false"
                      fWinAny="false" fMacAny="false" fBetaAny="false" fRiscAny="false" fOOM="false" fGlJmp="false"
                      fFontLimit="false" verXLHigh="3" verLowestBiff="6" verLastXLSaved="3"/>
      <xsl:call-template name="INTERFACE"/>
      <WriteAccess userName="  "/>
      <xsl:call-template name="FileSharing"/>
      <CodePage cv="1200"/> <!-- TODO: find the source -->
      <DSF/>
      <xsl:call-template name="RRTabId"/>
      <xsl:call-template name="PROTECTION"/>
      <xsl:call-template name="Window1"/>
      <Backup fBackup="{boolean($workbook_file/main:workbook/main:workbookPr/@backupFile = 'true' or $workbook_file/main:workbook/main:workbookPr/@backupFile = '1')}"/>
      <xsl:call-template name="HideObj"/>
      <xsl:call-template name="Date1904"/>
      <CalcPrecision fFullPrec="{not(boolean($workbook_file/main:workbook/main:calcPr/@fullPrecision = 'false' or $workbook_file/main:workbook/main:calcPr/@fullPrecision = '0'))}" />
      <RefreshAll refreshAll="{boolean($workbook_file/main:workbook/main:workbookPr/@refreshAllConnections = 'true' or $workbook_file/main:workbook/main:workbookPr/@refreshAllConnections = '1')}" />
      <xsl:call-template name="BookBool"/>
      <xsl:call-template name="FORMATTING"/>
      <xsl:call-template name="UserBView"/>
      <UsesELFs useselfs="false"/> <!-- This feature is not supported in Excel 2007, therefore it is always 'false' -->
      <xsl:call-template name="BUNDLESHEET"/>
      <Country iCountryDef="1" iCountryWinIni="1" />
      <xsl:call-template name="SUPBOOK"/>
      <xsl:call-template name="LBL"/>
      <xsl:if test="$workbook_file/main:workbook/main:calcPr/@calcId">
        <RecalcId dwBuild="{$workbook_file/main:workbook/main:calcPr/@calcId}"/>
      </xsl:if>
      <xsl:call-template name="SHAREDSTRINGS"/> <!-- ExtSST inside -->
      <xsl:call-template name="BookExt"/>
      <EOF/>
    </GlobalsSubstream>
  </xsl:template>

 
  <xsl:template name="INTERFACE">
    <INTERFACE>
      <InterfaceHdr codePage="1200"/>
      <Mms/>
      <InterfaceEnd/>
    </INTERFACE>
  </xsl:template>
  
  <xsl:template name="FileSharing">
    <xsl:if test="$workbook_file/main:workbook/main:fileSharing">
      <FileSharing>
        <xsl:attribute name="readOnlyRecommended">
          <xsl:choose>
            <xsl:when test="$workbook_file/main:workbook/main:fileSharing/@readOnlyRecommended = 'true' or $workbook_file/main:workbook/main:fileSharing/@readOnlyRecommended = '1'">true</xsl:when>
            <xsl:otherwise>false</xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
        <xsl:attribute name="password">
          <xsl:choose>
            <xsl:when test="$workbook_file/main:workbook/main:fileSharing/@password != ''">
              <xsl:value-of select="$workbook_file/main:workbook/main:fileSharing/@password"/>
            </xsl:when>
            <xsl:otherwise>0000</xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
        <xsl:attribute name="userName">
          <xsl:if test="$workbook_file/main:workbook/main:fileSharing/@stUNUsername != ''">
            <xsl:value-of select="$workbook_file/main:workbook/main:fileSharing/@stUNUsername"/>
          </xsl:if>
        </xsl:attribute>
      </FileSharing>
    </xsl:if>
  </xsl:template>
  
  <xsl:template name="RRTabId">
    <RRTabId>
      <xsl:for-each select="$workbook_file/main:workbook/main:sheets/main:sheet">
        <SheetId id="{@sheetId}"/>
      </xsl:for-each>
    </RRTabId>
    
  </xsl:template>
  
  <xsl:template name="PROTECTION">   <!-- TODO: implement main:protection parsing -->
    <PROTECTION>
      <WinProtect fLockWn="false"/>
      <Protect fLock="false"/>
      <Password wPassword="0000"/>
      <Prot4Rev fRevLock="false"/>
      <Prot4RevPass protPwdRev="0000"/>
    </PROTECTION>
  </xsl:template>
  
  <xsl:template name="Window1">
    <xsl:for-each select="$workbook_file/main:workbook/main:bookViews/main:workbookView">
      <Window1>
        <xsl:attribute name="xWn">
          <xsl:choose>
            <xsl:when test="@xWindow"><xsl:value-of select="@xWindow"/></xsl:when>
            <xsl:otherwise>0</xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
        <xsl:attribute name="yWn">
          <xsl:choose>
            <xsl:when test="@yWindow"><xsl:value-of select="@yWindow"/></xsl:when>
            <xsl:otherwise>30</xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
        <xsl:attribute name="dxWn">
          <xsl:choose>
            <xsl:when test="@windowWidth"><xsl:value-of select="@windowWidth"/></xsl:when>
            <xsl:otherwise>8000</xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
        <xsl:attribute name="dyWn">
          <xsl:choose>
            <xsl:when test="@windowHeight"><xsl:value-of select="@windowHeight"/></xsl:when>
            <xsl:otherwise>16000</xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
        <xsl:choose>
          <xsl:when test="@visibility = 'hidden'">
            <xsl:attribute name="fHidden">true</xsl:attribute>
            <xsl:attribute name="fVeryHidden">false</xsl:attribute>
          </xsl:when>
          <xsl:when test="@visibility = 'veryHidden'">
            <xsl:attribute name="fHidden">true</xsl:attribute>
            <xsl:attribute name="fVeryHidden">true</xsl:attribute>
          </xsl:when>
          <xsl:otherwise> <!-- no tag or equal to 'visible' -->
            <xsl:attribute name="fHidden">false</xsl:attribute>
            <xsl:attribute name="fVeryHidden">false</xsl:attribute>
          </xsl:otherwise>
        </xsl:choose>
        <xsl:attribute name="fIconic">
          <xsl:choose>
            <xsl:when test="@minimized = 'true' or @minimized = '1'">true</xsl:when>
            <xsl:otherwise>false</xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
        <xsl:attribute name="fDspHScroll">
          <xsl:choose>
            <xsl:when test="@showHorizontalScroll = 'false' or @showHorizontalScroll = '0'">false</xsl:when>
            <xsl:otherwise>true</xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
        <xsl:attribute name="fDspVScroll">
          <xsl:choose>
            <xsl:when test="@showVerticalScroll = 'false' or @showVerticalScroll = '0'">false</xsl:when>
            <xsl:otherwise>true</xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
        <xsl:attribute name="fBotAdornment">
          <xsl:choose>
            <xsl:when test="@showSheetTabs = 'false' or @showSheetTabs = '0'">false</xsl:when>
            <xsl:otherwise>true</xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
        <xsl:attribute name="fNoAFDateGroup">
          <xsl:choose>
            <xsl:when test="@autoFilterDateGrouping = 'false' or @autoFilterDateGrouping = '0'">true</xsl:when>
            <xsl:otherwise>false</xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
        <xsl:attribute name="itabCur">
          <xsl:choose>
            <xsl:when test="@activeTab"><xsl:value-of select="@activeTab"/></xsl:when>
            <xsl:otherwise>0</xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
        <xsl:attribute name="itabFirst">
          <xsl:choose>
            <xsl:when test="@firstSheet"><xsl:value-of select="@firstSheet"/></xsl:when>
            <xsl:otherwise>0</xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
        <xsl:attribute name="ctabSel">1</xsl:attribute>
        <xsl:attribute name="wTabRatio">
          <xsl:choose>
            <xsl:when test="@tabRatio"><xsl:value-of select="@tabRatio"/></xsl:when>
            <xsl:otherwise>600</xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
      </Window1>
    </xsl:for-each>
  </xsl:template>
  
  <xsl:template name="HideObj">
    <HideObj>
      <xsl:attribute name="hideObj">
        <xsl:choose>
          <xsl:when test="$workbook_file/main:workbook/main:workbookPr/@showObjects = 'none'">none</xsl:when>
          <xsl:when test="$workbook_file/main:workbook/main:workbookPr/@showObjects = 'placeholders'">placeholders</xsl:when>
          <xsl:otherwise>all</xsl:otherwise> <!-- empty or 'all' -->
        </xsl:choose>
      </xsl:attribute>
    </HideObj>
  </xsl:template>
  
  <xsl:template name="Date1904">
    <Date1904>
      <xsl:attribute name="f1904DateSystem">
        <xsl:choose>
          <xsl:when test="$workbook_file/main:workbook/main:workbookPr/@date1904 = 'true' or $workbook_file/main:workbook/main:workbookPr/@date1904 = '1'">true</xsl:when>
          <xsl:otherwise>false</xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
    </Date1904>
  </xsl:template>
  
  <xsl:template name="BookBool">
    <BookBool>
      <xsl:call-template name="attrib_bool_req_inv">
        <xsl:with-param name="name">fNoSaveSup</xsl:with-param>
        <xsl:with-param name="val" select="$workbook_file/main:workbook/main:workbookPr/@saveExternalLinkValues"/>
        <xsl:with-param name="def_val">true</xsl:with-param>
      </xsl:call-template>
      <xsl:attribute name="fHasEnvelope">false</xsl:attribute>
      <xsl:attribute name="fEnvelopeVisible">false</xsl:attribute>
      <xsl:attribute name="fEnvelopeInitDone">false</xsl:attribute>
      <xsl:attribute name="grUpdateLinks">
        <xsl:choose>
          <xsl:when test="$workbook_file/main:workbook/main:workbookPr/@updateLinks = 'never'">never</xsl:when>
          <xsl:when test="$workbook_file/main:workbook/main:workbookPr/@updateLinks = 'always'">always</xsl:when>
          <xsl:otherwise>userSet</xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
      <xsl:call-template name="attrib_bool_req_inv">
        <xsl:with-param name="name">fHideBorderUnselLists</xsl:with-param>
        <xsl:with-param name="val" select="$workbook_file/main:workbook/main:workbookPr/@showBorderUnselectedTables"/>
        <xsl:with-param name="def_val">true</xsl:with-param>
      </xsl:call-template>
    </BookBool>
  </xsl:template>

  <xsl:template name="attrib_bool_req">
    <xsl:param name="name"/>
    <xsl:param name="val"/>
    <xsl:param name="def_val"/>  <!-- If not specified that means 'false -->
    <xsl:attribute name="{$name}">
      <xsl:choose>
        <xsl:when test="$def_val = 'true'">
          <xsl:value-of select="not(boolean($val = 'false' or $val = '0'))"/>
        </xsl:when>
        <xsl:otherwise> <!-- default is 'false' -->
          <xsl:value-of select="boolean($val = 'true' or $val = '1')"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:attribute>
  </xsl:template>

  <xsl:template name="attrib_bool_req_inv">
    <xsl:param name="name"/>
    <xsl:param name="val"/>
    <xsl:param name="def_val"/> <!-- If not specified that means 'false --> <!-- This is the default value as it appears in the result xlsx -->
    <xsl:attribute name="{$name}">
      <xsl:choose>
        <xsl:when test="$def_val = 'true'">
          <xsl:value-of select="boolean($val = 'false' or $val = '0')"/>
        </xsl:when>
        <xsl:otherwise> <!-- default is 'false' -->
          <xsl:value-of select="not(boolean($val = 'true' or $val = '1'))"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:attribute>
  </xsl:template>

  <xsl:template name="attrib_req_with_def">
    <xsl:param name="name"/>
    <xsl:param name="val"/>
    <xsl:param name="def_val"/>
    <xsl:attribute name="{$name}">
      <xsl:choose>
        <xsl:when test="$val != '' and string($val) != 'NaN'"><xsl:value-of select="$val"/></xsl:when>
        <xsl:otherwise><xsl:value-of select="$def_val"/></xsl:otherwise>
      </xsl:choose>
    </xsl:attribute>
  </xsl:template>

  <xsl:key name="numFmtId" match="main:numFmt" use="@numFmtId"/>
  
  <xsl:template name="FORMATTING">
    <FORMATTING>
      <xsl:for-each select="$stylesheet_file/main:styleSheet/main:fonts/main:font[position() &lt; 511]">
        <xsl:call-template name="Font"/>
      </xsl:for-each>
      <xsl:for-each select="$shared_strings_file/main:sst/main:si/main:r/main:rPr[position() &lt; 511 - count($stylesheet_file/main:styleSheet/main:fonts/main:font)]">
        <xsl:call-template name="Font"/>
      </xsl:for-each>
      <xsl:for-each select="$stylesheet_file/main:styleSheet/main:numFmts/main:numFmt[position() &lt; (218 + 1)]">
        <Format ifmt="{@numFmtId}" stFormat="{@formatCode}"/>
      </xsl:for-each>
      <xsl:variable name="numFmt_count" select="count($stylesheet_file/main:styleSheet/main:numFmts/main:numFmt)"/>
      <xsl:for-each select="$stylesheet_file/main:styleSheet/main:dxfs/main:dxf/main:numFmt[generate-id(.) = generate-id(key('numFmtId', @numFmtId))]">
        <xsl:if test="position() &lt; (218 - $numFmt_count + 1)">
          <xsl:if test="key('numFmt_is_user_defined', @numFmtId)">
            <Format ifmt="{@numFmtId}" stFormat="{@formatCode}"/>
          </xsl:if>
        </xsl:if>
      </xsl:for-each>
      <XFS>
        <xsl:for-each select="$stylesheet_file/main:styleSheet/main:cellStyleXfs/main:xf">
          <xsl:call-template name="XF"/>
        </xsl:for-each>
        <xsl:if test="count($stylesheet_file/main:styleSheet/main:cellStyleXfs/main:xf) &lt; 15">
          <xsl:call-template name="copyXF">
            <xsl:with-param name="from" select="$stylesheet_file/main:styleSheet/main:cellStyleXfs/main:xf[position() = 1]"/>
            <xsl:with-param name="count" select="15 - count($stylesheet_file/main:styleSheet/main:cellStyleXfs/main:xf)"/>
          </xsl:call-template>
        </xsl:if>
        <xsl:for-each select="$stylesheet_file/main:styleSheet/main:cellXfs/main:xf">
          <xsl:call-template name="XF"/>
        </xsl:for-each>
      </XFS>
      <STYLES>
        <xsl:for-each select="$stylesheet_file/main:styleSheet/main:cellStyles/main:cellStyle">
          <xsl:call-template name="Style"/>
        </xsl:for-each>
      </STYLES>
      <xsl:call-template name="Palette"/>
      <xsl:call-template name="ClrtClient"/>
    </FORMATTING>
  </xsl:template>

  <xsl:template name="Font">
    <Font>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">dyHeight</xsl:with-param>
        <xsl:with-param name="val" select="main:sz/@val * 20"/>
        <xsl:with-param name="def_val">200</xsl:with-param>
      </xsl:call-template>
      <xsl:attribute name="fItalic">
        <xsl:choose>
          <xsl:when test="main:i and not(boolean(main:i/@val)) or main:i/@val = 'true' or main:i/@val = '1'">true</xsl:when>
          <xsl:otherwise>false</xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
      <xsl:attribute name="fStrikeOut">
        <xsl:choose>
          <xsl:when test="main:strike and not(boolean(main:strike/@val)) or main:strike/@val = 'true' or main:strike/@val = '1'">true</xsl:when>
          <xsl:otherwise>false</xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
      <xsl:attribute name="fOutline">
        <xsl:choose>
          <xsl:when test="main:outline and not(boolean(main:outline/@val)) or main:outline/@val = 'true' or main:outline/@val = '1'">true</xsl:when>
          <xsl:otherwise>false</xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
      <xsl:attribute name="fShadow">
        <xsl:choose>
          <xsl:when test="main:shadow and not(boolean(main:shadow/@val)) or main:shadow/@val = 'true' or main:shadow/@val = '1'">true</xsl:when>
          <xsl:otherwise>false</xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
      <xsl:attribute name="fCondense">
        <xsl:choose>
          <xsl:when test="main:condense and not(boolean(main:condense/@val)) or main:condense/@val = 'true' or main:condense/@val = '1'">true</xsl:when>
          <xsl:otherwise>false</xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
      <xsl:attribute name="fExtend">
        <xsl:choose>
          <xsl:when test="main:extend and not(boolean(main:extend/@val)) or main:extend/@val = 'true' or main:extend/@val = '1'">true</xsl:when>
          <xsl:otherwise>false</xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">icv</xsl:with-param>
        <xsl:with-param name="val">
          <xsl:call-template name="color2Icv">
            <xsl:with-param name="color" select="main:color"/>
          </xsl:call-template>
        </xsl:with-param>
        <xsl:with-param name="def_val">32767</xsl:with-param>
      </xsl:call-template>
      <xsl:attribute name="bls">
        <xsl:choose>
          <xsl:when test="main:b and not(boolean(main:b/@val)) or main:b/@val = 'true' or main:b/@val = '1'">700</xsl:when>
          <xsl:otherwise>400</xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
      <xsl:attribute name="sss">
        <xsl:choose>
          <xsl:when test="main:vertAlign/@val = 'superscript'">1</xsl:when>
          <xsl:when test="main:vertAlign/@val = 'subscript'">2</xsl:when>
          <xsl:otherwise>0</xsl:otherwise> <!-- absent or 'baseline' -->
        </xsl:choose>
      </xsl:attribute>
      <xsl:attribute name="uls">
        <xsl:choose>
          <xsl:when test="main:u and not(boolean(main:u/@val)) or main:u/@val = 'single'">1</xsl:when>
          <xsl:when test="main:u/@val = 'double'">2</xsl:when>
           <xsl:when test="main:u/@val = 'singleAccounting'">33</xsl:when>
           <xsl:when test="main:u/@val = 'doubleAccounting'">34</xsl:when>
         <xsl:otherwise>0</xsl:otherwise> <!-- absent or 'none' -->
        </xsl:choose>
      </xsl:attribute>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">bFamily</xsl:with-param>
        <xsl:with-param name="val" select="main:family/@val"/>
        <xsl:with-param name="def_val">0</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">bCharSet</xsl:with-param>
        <xsl:with-param name="val" select="main:charset/@val"/>
        <xsl:with-param name="def_val">0</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">fontName</xsl:with-param>
        <xsl:with-param name="val" select="main:name/@val | main:rFont/@val"/>
        <xsl:with-param name="def_val"></xsl:with-param>
      </xsl:call-template>
    </Font>
  </xsl:template>

  <xsl:template name="XF">
    <xsl:variable name="cellOrStyle" select="boolean(../self::main:cellXfs)"/>
    <XF>
      <xsl:attribute name="ifnt"><xsl:value-of select="@fontId"/></xsl:attribute>
      <xsl:attribute name="ifmt"><xsl:value-of select="@numFmtId"/></xsl:attribute>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fLocked</xsl:with-param>
        <xsl:with-param name="val" select="main:protection/@locked"/>
        <xsl:with-param name="def_val">true</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fHidden</xsl:with-param>
        <xsl:with-param name="val" select="main:protection/@hidden"/>
      </xsl:call-template>
      <xsl:attribute name="fStyle"><xsl:value-of select="not($cellOrStyle)"/></xsl:attribute>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">f123Prefix</xsl:with-param>
        <xsl:with-param name="val" select="@quotePrefix"/>
      </xsl:call-template>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">ixfParent</xsl:with-param>
        <xsl:with-param name="val" select="@xfId"/> <!-- Copied as is because we write all StyleXF-s strict before CellXF-s -->
        <xsl:with-param name="def_val">4095</xsl:with-param>
      </xsl:call-template>
      <xsl:variable name="propTagName">
        <xsl:choose>
          <xsl:when test="$cellOrStyle">CellXF</xsl:when>
          <xsl:otherwise>StyleXF</xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <xsl:element name="{$propTagName}">
        <xsl:attribute name="alc">
          <xsl:call-template name="HorizontalAlignment_StringToID">
            <xsl:with-param name="string" select="main:alignment/@horizontal"/>
          </xsl:call-template>
        </xsl:attribute>
        <xsl:call-template name="attrib_bool_req">
          <xsl:with-param name="name">fWrap</xsl:with-param>
          <xsl:with-param name="val" select="main:alignment/@wrapText"/>
        </xsl:call-template>
        <xsl:attribute name="alcV">
          <xsl:call-template name="VerticalAlignment_StringToID">
            <xsl:with-param name="string" select="main:alignment/@vertical"/>
          </xsl:call-template>
        </xsl:attribute>
        <xsl:call-template name="attrib_bool_req">
          <xsl:with-param name="name">fJustLast</xsl:with-param>
          <xsl:with-param name="val" select="main:alignment/@justifyLastLine"/>
        </xsl:call-template>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">trot</xsl:with-param>
          <xsl:with-param name="val" select="main:alignment/@textRotation"/>
          <xsl:with-param name="def_val">0</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">cIndent</xsl:with-param>
          <xsl:with-param name="val" select="main:alignment/@indent"/>
          <xsl:with-param name="def_val">0</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_bool_req">
          <xsl:with-param name="name">fShrinkToFit</xsl:with-param>
          <xsl:with-param name="val" select="main:alignment/@shrinkToFit"/>
        </xsl:call-template>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">iReadOrder</xsl:with-param>
          <xsl:with-param name="val" select="main:alignment/@readingOrder"/>
          <xsl:with-param name="def_val">0</xsl:with-param>
        </xsl:call-template>
        <xsl:if test="$cellOrStyle">
          <xsl:call-template name="attrib_bool_req">
            <xsl:with-param name="name">fAtrNum</xsl:with-param>
            <xsl:with-param name="val" select="@applyNumberFormat"/>
          </xsl:call-template>
          <xsl:call-template name="attrib_bool_req">
            <xsl:with-param name="name">fAtrFnt</xsl:with-param>
            <xsl:with-param name="val" select="@applyFont"/>
          </xsl:call-template>
          <xsl:call-template name="attrib_bool_req">
            <xsl:with-param name="name">fAtrAlc</xsl:with-param>
            <xsl:with-param name="val" select="@applyAlignment"/>
          </xsl:call-template>
          <xsl:call-template name="attrib_bool_req">
            <xsl:with-param name="name">fAtrBdr</xsl:with-param>
            <xsl:with-param name="val" select="@applyBorder"/>
          </xsl:call-template>
          <xsl:call-template name="attrib_bool_req">
            <xsl:with-param name="name">fAtrPat</xsl:with-param>
            <xsl:with-param name="val" select="@applyFill"/>
          </xsl:call-template>
          <xsl:call-template name="attrib_bool_req">
            <xsl:with-param name="name">fAtrProt</xsl:with-param>
            <xsl:with-param name="val" select="@applyProtection"/>
          </xsl:call-template>
        </xsl:if>
        <xsl:variable name="borderId">
          <xsl:choose>
            <xsl:when test="@borderId != ''"><xsl:value-of select="@borderId"/></xsl:when>
            <xsl:otherwise>0</xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        <xsl:variable name="border" select="../../main:borders/main:border[position() = $borderId + 1]"/>
        <xsl:attribute name="dgLeft">
          <xsl:call-template name="BorderStyle_StringToID">
            <xsl:with-param name="string" select="$border/main:left/@style"/>
          </xsl:call-template>
        </xsl:attribute>
        <xsl:attribute name="dgRight">
          <xsl:call-template name="BorderStyle_StringToID">
            <xsl:with-param name="string" select="$border/main:right/@style"/>
          </xsl:call-template>
        </xsl:attribute>
        <xsl:attribute name="dgTop">
          <xsl:call-template name="BorderStyle_StringToID">
            <xsl:with-param name="string" select="$border/main:top/@style"/>
          </xsl:call-template>
        </xsl:attribute>
        <xsl:attribute name="dgBottom">
          <xsl:call-template name="BorderStyle_StringToID">
            <xsl:with-param name="string" select="$border/main:bottom/@style"/>
          </xsl:call-template>
        </xsl:attribute>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">icvLeft</xsl:with-param>
          <xsl:with-param name="val">
            <xsl:call-template name="color2Icv">
              <xsl:with-param name="color" select="$border/main:left/main:color"/>
            </xsl:call-template>
          </xsl:with-param>
          <xsl:with-param name="def_val">0</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">icvRight</xsl:with-param>
          <xsl:with-param name="val">
            <xsl:call-template name="color2Icv">
              <xsl:with-param name="color" select="$border/main:right/main:color"/>
            </xsl:call-template>
          </xsl:with-param>
          <xsl:with-param name="def_val">0</xsl:with-param>
        </xsl:call-template>
        <xsl:attribute name="grbitDiag">
          <xsl:choose>
            <xsl:when test="not($border/@diagonalDown = 'true' or $border/@diagonalDown = '1') and not($border/@diagonalUp = 'true' or $border/@diagonalUp = '1')">0</xsl:when>
            <xsl:when test="($border/@diagonalDown = 'true' or $border/@diagonalDown = '1') and not($border/@diagonalUp = 'true' or $border/@diagonalUp = '1')">1</xsl:when>
            <xsl:when test="not($border/@diagonalDown = 'true' or $border/@diagonalDown = '1') and ($border/@diagonalUp = 'true' or $border/@diagonalUp = '1')">2</xsl:when>
            <xsl:when test="($border/@diagonalDown = 'true' or $border/@diagonalDown = '1') and ($border/@diagonalUp = 'true' or $border/@diagonalUp = '1')">3</xsl:when>
          </xsl:choose>
        </xsl:attribute>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">icvTop</xsl:with-param>
          <xsl:with-param name="val">
            <xsl:call-template name="color2Icv">
              <xsl:with-param name="color" select="$border/main:top/main:color"/>
            </xsl:call-template>
          </xsl:with-param>
          <xsl:with-param name="def_val">0</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">icvBottom</xsl:with-param>
          <xsl:with-param name="val">
            <xsl:call-template name="color2Icv">
              <xsl:with-param name="color" select="$border/main:bottom/main:color"/>
            </xsl:call-template>
          </xsl:with-param>
          <xsl:with-param name="def_val">0</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">icvDiag</xsl:with-param>
          <xsl:with-param name="val">
            <xsl:call-template name="color2Icv">
              <xsl:with-param name="color" select="$border/main:diagonal/main:color"/>
            </xsl:call-template>
          </xsl:with-param>
          <xsl:with-param name="def_val">0</xsl:with-param>
        </xsl:call-template>
        <xsl:attribute name="dgDiag">
          <xsl:call-template name="BorderStyle_StringToID">
            <xsl:with-param name="string" select="$border/main:diagonal/@style"/>
          </xsl:call-template>
        </xsl:attribute>
        <!-- TODO: fHasXFExt -->
        <xsl:if test="$cellOrStyle">
          <xsl:attribute name="fHasXFExt">false</xsl:attribute>
        </xsl:if>
        <xsl:variable name="fillId">
          <xsl:choose>
            <xsl:when test="@fillId != ''">
              <xsl:value-of select="@fillId"/>
            </xsl:when>
            <xsl:otherwise>0</xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        <xsl:variable name="fill" select="../../main:fills/main:fill[position() = $fillId + 1]"/>
        <xsl:attribute name="fls">
          <xsl:call-template name="PatternType_StringToID">
            <xsl:with-param name="string" select="$fill/main:patternFill/@patternType"/>
          </xsl:call-template>
        </xsl:attribute>
        <xsl:variable name="fgColor_icv">
          <xsl:call-template name="color2Icv">
            <xsl:with-param name="color" select="$fill/main:patternFill/main:fgColor"/>
          </xsl:call-template>
        </xsl:variable>

        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">icvFore</xsl:with-param>
          <xsl:with-param name="val" select="$fgColor_icv"/>
          <xsl:with-param name="def_val">64</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">icvBack</xsl:with-param>
          <xsl:with-param name="val">
            <xsl:choose>
              <xsl:when test="$fill/main:patternFill/@patternType = 'solid'">
                <xsl:value-of select="$fgColor_icv"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:call-template name="color2Icv">
                  <xsl:with-param name="color" select="$fill/main:patternFill/main:bgColor"/>
                </xsl:call-template>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>
          <xsl:with-param name="def_val">65</xsl:with-param>
        </xsl:call-template>
        <xsl:if test="$cellOrStyle">
          <xsl:attribute name="fsxButton">false</xsl:attribute>
        </xsl:if>
      </xsl:element>
    </XF>
  </xsl:template>

  <xsl:template name="copyXF">
    <xsl:param name="from"/>
    <xsl:param name="count"/>
    <xsl:if test="$count > 0">
      <xsl:for-each select="$from">
          <xsl:call-template name="XF"/>
      </xsl:for-each>
      <xsl:call-template name="copyXF">
        <xsl:with-param name="from" select="$from"/>
        <xsl:with-param name="count" select="$count - 1"/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

  <xsl:template name="Style">
    <Style>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">ixfe</xsl:with-param>
        <xsl:with-param name="val" select="@xfId"/> <!-- Copied as is because we write all StyleXF-s strict before CellXF-s -->
        <xsl:with-param name="def_val">0</xsl:with-param>
      </xsl:call-template>
      <xsl:choose>
        <xsl:when test="@builtinId != '0'">
          <xsl:attribute name="fBuiltIn">true</xsl:attribute>
          <BuiltInStyle istyBuiltIn="{@builtinId}">
            <xsl:call-template name="attrib_req_with_def">
              <xsl:with-param name="name">iLevel</xsl:with-param>
              <xsl:with-param name="val" select="@iLevel"/> <!-- Copied as is because we write all StyleXF-s strict before CellXF-s -->
              <xsl:with-param name="def_val">255</xsl:with-param>
            </xsl:call-template>
          </BuiltInStyle>
        </xsl:when>
        <xsl:otherwise>
          <xsl:attribute name="fBuiltIn">false</xsl:attribute>
          <xsl:attribute name="user"><xsl:value-of select="@name"/></xsl:attribute> <!-- Default is '' -->
        </xsl:otherwise>
      </xsl:choose>
    </Style>
  </xsl:template>
    
  <xsl:template name="BUNDLESHEET">
    <xsl:for-each select="$workbook_file/main:workbook/main:sheets/main:sheet">
      <BUNDLESHEET>
        <BoundSheet8>
          <!-- lbPlyPos will be set in XLS serializer -->
          <!-- Since we set order of sheets here, the order of pointers would be accending -->
          <xsl:call-template name="attrib_req_with_def">
            <xsl:with-param name="name">hsState</xsl:with-param>
            <xsl:with-param name="val" select="@state"/>
            <xsl:with-param name="def_val">visible</xsl:with-param>
          </xsl:call-template>
          <xsl:variable name="rId" select="@mainr:id"/>
          <xsl:variable name="sheetTypeURI">
            <xsl:value-of select="$workbook_file/r:Relationships/r:Relationship[@Id = $rId]/@Type"/>
          </xsl:variable>
          <xsl:attribute name="dt">
            <xsl:call-template name="SheetTypeURI_StringToID">
              <xsl:with-param name="string" select="$sheetTypeURI"/>
            </xsl:call-template>
          </xsl:attribute>
          <xsl:attribute name="stName"><xsl:value-of select="@name"/></xsl:attribute>
        </BoundSheet8>
      </BUNDLESHEET>
    </xsl:for-each>
  </xsl:template>
  
  <xsl:template name="SHAREDSTRINGS">
    <xsl:variable name="set_size_dep">
      <xsl:choose>
        <xsl:when test="$shared_strings_file/main:sst">
          <xsl:value-of select="floor($shared_strings_file/main:sst/@uniqueCount div 128) + 1"/>
        </xsl:when>
        <xsl:otherwise>0</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="set_size">
      <xsl:choose>
        <xsl:when test="$set_size_dep > 8">
          <xsl:value-of select="$set_size_dep"/>
        </xsl:when>
        <xsl:otherwise>8</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <SHAREDSTRINGS>
      <SST>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">cstTotal</xsl:with-param>
          <xsl:with-param name="val" select="$shared_strings_file/main:sst/@count"/>
          <xsl:with-param name="def_val">0</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">cstUnique</xsl:with-param>
          <xsl:with-param name="val" select="$shared_strings_file/main:sst/@uniqueCount"/>
          <xsl:with-param name="def_val">0</xsl:with-param>
        </xsl:call-template>
        <xsl:if test="$shared_strings_file/main:sst">
          <xsl:for-each select="$shared_strings_file/main:sst/main:si">
            <XLUnicodeRichExtendedString>
              <xsl:attribute name="text">
                <!-- This concatenates all text nodes (there are only 't' text nodes here) -->
                <xsl:for-each select="main:t | main:r/main:t">
                  <xsl:value-of select="."/>
                </xsl:for-each>
              </xsl:attribute>
              <xsl:attribute name="mark_set_start">
                <xsl:choose>
                  <xsl:when test="(position() - 1) mod $set_size = 0">true</xsl:when>
                  <xsl:otherwise>false</xsl:otherwise>
                </xsl:choose>
              </xsl:attribute>
              <xsl:for-each select="main:r[position() > 1]">
                <FormatRun>
                  <xsl:variable name="runPos" select="position()"/>
                  <xsl:variable name="rSet">
                    <xsl:copy-of select="preceding-sibling::main:r/main:t"/>
                  </xsl:variable>
                  <xsl:attribute name="ich">
                    <xsl:value-of select="string-length($rSet)"/>
                  </xsl:attribute>
                  <xsl:attribute name="ifnt">
                    <xsl:variable name="numBaseFonts" select="count($stylesheet_file/main:styleSheet/main:fonts/main:font)"/>
                    <xsl:variable name="ifntUnchecked" select="$numBaseFonts + count(preceding::main:r/main:rPr)"/>
                    <xsl:choose>
                      <xsl:when test="main:rPr and $ifntUnchecked &lt; 511">
                        <xsl:value-of select="$ifntUnchecked"/>
                      </xsl:when>
                      <xsl:otherwise>0</xsl:otherwise>
                    </xsl:choose>
                  </xsl:attribute>
                </FormatRun>
              </xsl:for-each>
            </XLUnicodeRichExtendedString>
          </xsl:for-each>
        </xsl:if>
      </SST>
    </SHAREDSTRINGS>
    <ExtSST>
      <xsl:attribute name="dsst"><xsl:value-of select="$set_size"/></xsl:attribute>
      <xsl:attribute name="num_sets">
        <xsl:choose>
          <xsl:when test="$shared_strings_file/main:sst/@uniqueCount">
            <xsl:value-of select="floor(($shared_strings_file/main:sst/@uniqueCount + $set_size - 1) div $set_size)"/>
          </xsl:when>
          <xsl:otherwise>0</xsl:otherwise>
        </xsl:choose>       
      </xsl:attribute>
    </ExtSST>
  </xsl:template>
  
  <xsl:template name="BookExt">
    <BookExt> <!-- Let it always exists -->
      <xsl:call-template name="attrib_bool_req_inv">
        <xsl:with-param name="name">fDontAutoRecover</xsl:with-param>
        <xsl:with-param name="val" select="$workbook_file/main:workbook/main:fileRecoveryPr/@autoRecover"/>
        <xsl:with-param name="def_val">true</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fHidePivotList</xsl:with-param>
        <xsl:with-param name="val" select="$workbook_file/main:workbook/main:workbookPr/@hidePivotFieldList"/>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fFilterPrivacy</xsl:with-param>
        <xsl:with-param name="val" select="$workbook_file/main:workbook/main:workbookPr/@filterPrivacy"/>
      </xsl:call-template>
      <xsl:attribute name="fFilterPrivacy">false</xsl:attribute> <!-- TODO: implement when smart tags are implemented -->
      <xsl:attribute name="fEmbedFactoids">false</xsl:attribute> <!-- TODO: implement when smart tags are implemented -->
      <xsl:attribute name="mdFactoidDisplay">0</xsl:attribute> <!-- TODO: implement when smart tags are implemented -->
      <xsl:attribute name="fSavedDuringRecovery">false</xsl:attribute> <!-- TODO: unknown source for the tag -->
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fCreatedViaMinimalSave</xsl:with-param>
        <xsl:with-param name="val" select="$workbook_file/main:workbook/main:fileRecoveryPr/@crashSave"/>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fOpenedViaDataRecovery</xsl:with-param>
        <xsl:with-param name="val" select="$workbook_file/main:workbook/main:fileRecoveryPr/@dataExtractLoad"/>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fOpenedViaSafeLoad</xsl:with-param>
        <xsl:with-param name="val" select="$workbook_file/main:workbook/main:fileRecoveryPr/@repairLoad"/>
      </xsl:call-template>
      <!--<xsl:if test="$workbook/main:workbookPr/@promptedSolutions = 'true' or $workbook/main:workbookPr/@promptedSolutions = '1' or
                    $workbook/main:workbookPr/@showInkAnnotation = 'false' or $workbook/main:workbookPr/@showInkAnnotation = '0' or
                    $workbook/main:workbookPr/@publishItems = 'true' or $workbook/main:workbookPr/@publishItems = '1' or
                    $workbook/main:workbookPr/@showPivotChartFilter = 'true' or $workbook/main:workbookPr/@showPivotChartFilter = '1'">-->
        <BookExt_Conditional11>
          <xsl:call-template name="attrib_bool_req">
            <xsl:with-param name="name">fBuggedUserAboutSolution</xsl:with-param>
            <xsl:with-param name="val" select="$workbook_file/main:workbook/main:workbookPr/@promptedSolutions"/>
          </xsl:call-template>
          <xsl:call-template name="attrib_bool_req">
            <xsl:with-param name="name">fShowInkAnnotation</xsl:with-param>
            <xsl:with-param name="val" select="$workbook_file/main:workbook/main:workbookPr/@showInkAnnotation"/>
            <xsl:with-param name="def_val">true</xsl:with-param>
          </xsl:call-template>
        </BookExt_Conditional11>
      <!--</xsl:if>-->
      <!--<xsl:if test="$workbook/main:workbookPr/@publishItems = 'true' or $workbook/main:workbookPr/@publishItems = '1' or
                    $workbook/main:workbookPr/@showPivotChartFilter = 'true' or $workbook/main:workbookPr/@showPivotChartFilter = '1'">-->
        <BookExt_Conditional12>
          <xsl:call-template name="attrib_bool_req">
            <xsl:with-param name="name">fPublishedBookItems</xsl:with-param>
            <xsl:with-param name="val" select="$workbook_file/main:workbook/main:workbookPr/@publishItems"/>
          </xsl:call-template>
          <xsl:call-template name="attrib_bool_req">
            <xsl:with-param name="name">fShowPivotChartFilter</xsl:with-param>
            <xsl:with-param name="val" select="$workbook_file/main:workbook/main:workbookPr/@showPivotChartFilter"/>
          </xsl:call-template>
        </BookExt_Conditional12>
        <!--</xsl:if>-->
      </BookExt>
  </xsl:template>


  <xsl:template name="color2Icv"> <!-- Transforms <color> tag to the nearest indexed Icv value -->
    <xsl:param name="color"/>
    <xsl:choose>
      <xsl:when test="$color/@indexed"><xsl:value-of select="$color/@indexed"/></xsl:when>
      <xsl:when test="$color/@rgb">
        <xsl:call-template name="rgb2index">
          <xsl:with-param name="rgb" select="$color/@rgb"/>
          <xsl:with-param name="tint" select="$color/@tint"/>
        </xsl:call-template>
      </xsl:when>
      <!-- Auto color is skipped and shall be set in the context of an invocation due to different auto value
      <xsl:when test="$color/@auto = 'true' or $color/@auto = '1'">
        <xsl:value-of select="'64'"/>
      </xsl:when>
      -->
      <xsl:when test="$color/@theme">
        <xsl:variable name="color_pos">
          <xsl:choose>
            <xsl:when test="$color/@theme = 0">1</xsl:when>
            <xsl:when test="$color/@theme = 1">0</xsl:when>
            <xsl:when test="$color/@theme = 2">3</xsl:when>
            <xsl:when test="$color/@theme = 3">2</xsl:when>
            <xsl:otherwise><xsl:value-of select="$color/@theme"/></xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        <xsl:variable name="color_item" select="$theme_file/a:theme/a:themeElements/a:clrScheme/*[position() = $color_pos + 1]"/>
        <xsl:if test="$color_item">
          <xsl:variable name="rgb" select="$color_item/a:srgbClr/@val"/>
          <xsl:variable name="sys_color" select="$color_item/a:sysClr"/>
          <xsl:choose>
            <xsl:when test="$rgb != ''">
              <xsl:call-template name="rgb2index">
                <xsl:with-param name="rgb" select="$rgb"/>
                <xsl:with-param name="tint" select="$color/@tint"/>
              </xsl:call-template>
            </xsl:when>
            <xsl:when test="$sys_color and $sys_color/@lastClr">
              <xsl:call-template name="rgb2index">
                <xsl:with-param name="rgb" select="$sys_color/@lastClr"/>
                <xsl:with-param name="tint" select="$color/@tint"/>
              </xsl:call-template>
            </xsl:when>
            <xsl:when test="$sys_color and not($sys_color/@lastClr)">
              <!-- No idea -->
            </xsl:when>
          </xsl:choose>

        </xsl:if>

      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:variable name="hex_digits">0123456789ABCDEF</xsl:variable>
  
  <xsl:template name="hex2dec">
    <xsl:param name="hex"/>
    <xsl:param name="dec">0</xsl:param>
    <xsl:choose>
      <xsl:when test="$hex != ''">
        <xsl:call-template name="hex2dec">
          <xsl:with-param name="hex" select="substring($hex, 2, string-length($hex) - 1)"/>
          <xsl:with-param name="dec" select="(number($dec) * 16) + string-length(substring-before($hex_digits, substring($hex, 1, 1)))"/>
        </xsl:call-template>        
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$dec"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
    
  <xsl:template name="getClosestIcvIndex">
    <xsl:param name="palette"/>
    <xsl:param name="r"/>
    <xsl:param name="g"/>
    <xsl:param name="b"/>
    <!--
    <xsl:message>getClosestIcvIndex(<xsl:value-of select="$r"/>, <xsl:value-of select="$g"/>,<xsl:value-of select="$b"/>)</xsl:message>
    -->
    <xsl:variable name="distances_map">
      <xsl:call-template name="make_distances_map">
        <xsl:with-param name="r" select="$r"/>
        <xsl:with-param name="g" select="$g"/>
        <xsl:with-param name="b" select="$b"/>
        <xsl:with-param name="palette" select="$palette"/>
      </xsl:call-template>
    </xsl:variable>


    <xsl:variable name="distances_map_real" select="msxsl:node-set($distances_map)"/>
    <xsl:variable name="min_distance" select="$distances_map_real/min_distance/@value"/>
    <xsl:variable name="index" select="$distances_map_real/icv[@distance = $min_distance]/@index"/>
    <xsl:value-of select="$index"/>
    <!--
    <xsl:message>chosen index=<xsl:value-of select="$index"/>
    </xsl:message>
    -->
  </xsl:template>

  <xsl:template name="make_distances_map">
    <xsl:param name="palette"/>
    <xsl:param name="r"/>
    <xsl:param name="g"/>
    <xsl:param name="b"/>
    <xsl:param name="start_pos">9</xsl:param>
    <xsl:param name="min_distance">6502500</xsl:param>  <!-- 30*255*255 + 59*255*255 + 11*255*255 -->
    <xsl:variable name="argb" select="$palette/main:indexedColors/main:rgbColor[position() = $start_pos]/@rgb"/>
    <xsl:choose>
      <xsl:when test="$argb">
        <xsl:variable name="ri">
          <xsl:call-template name="hex2dec">
            <xsl:with-param name="hex" select="substring($argb, 3, 2)"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:variable name="gi">
          <xsl:call-template name="hex2dec">
            <xsl:with-param name="hex" select="substring($argb, 5, 2)"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:variable name="bi">
          <xsl:call-template name="hex2dec">
            <xsl:with-param name="hex" select="substring($argb, 7, 2)"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:variable name="distance" select="30 * ($ri - $r) * ($ri - $r) + 59 * ($gi - $g) * ($gi - $g) + 11 * ($bi - $b) * ($bi - $b)"/>
        <icv index="{$start_pos - 1}" distance="{$distance}"/>
        <!--
        <xsl:message>index=<xsl:value-of select="$start_pos - 1"/> distance=<xsl:value-of select="$distance"/></xsl:message>
        -->
        <xsl:call-template name="make_distances_map">
          <xsl:with-param name="r" select="$r"/>
          <xsl:with-param name="g" select="$g"/>
          <xsl:with-param name="b" select="$b"/>
          <xsl:with-param name="palette" select="$palette"/>
          <xsl:with-param name="start_pos" select="$start_pos + 1"/>
          <xsl:with-param name="min_distance">
            <xsl:choose>
              <xsl:when test="$distance &lt; $min_distance">
                <xsl:value-of select="$distance"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$min_distance"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <min_distance value="{$min_distance}"/>
        <!--
        <xsl:message>min_distance=<xsl:value-of select="$min_distance"/></xsl:message>
        -->
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="rgb2index">
    <xsl:param name="rgb"/> <!-- Pass only 8 or 6 hexadecimal digits here -->
    <xsl:param name="tint"/>
    <xsl:variable name="alpha_skip">
      <xsl:choose>
        <xsl:when test="string-length($rgb) = 8">2</xsl:when>
        <xsl:otherwise>0</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="r">
      <xsl:call-template name="hex2dec">
        <xsl:with-param name="hex" select="substring($rgb, 1 + $alpha_skip, 2)"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="g">
      <xsl:call-template name="hex2dec">
        <xsl:with-param name="hex" select="substring($rgb, 3 + $alpha_skip, 2)"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="b">
      <xsl:call-template name="hex2dec">
        <xsl:with-param name="hex" select="substring($rgb, 5 + $alpha_skip, 2)"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="rgb_corrected">
      <xsl:call-template name="applyTintToRGB">
        <xsl:with-param name="r" select="$r"/>
        <xsl:with-param name="g" select="$g"/>
        <xsl:with-param name="b" select="$b"/>
        <xsl:with-param name="tint" select="$tint"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="palette">
      <xsl:choose>
        <xsl:when test="$stylesheet_file/main:styleSheet/main:colors/main:indexedColors">
          <xsl:copy-of select="$stylesheet_file/main:styleSheet/main:colors/main:indexedColors"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:copy-of select="$indexedColors"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <!--
    <xsl:message>rgb: <xsl:value-of select="$r"/> :<xsl:value-of select="$g"/> :<xsl:value-of select="$b"/> (<xsl:value-of select="$rgb"/>)</xsl:message>
    -->
    <xsl:choose>
      <xsl:when test="msxsl:node-set($rgb_corrected)/rgb_corrected">
         <xsl:call-template name="getClosestIcvIndex">
          <xsl:with-param name="r" select="msxsl:node-set($rgb_corrected)/rgb_corrected/@r"/>
          <xsl:with-param name="g" select="msxsl:node-set($rgb_corrected)/rgb_corrected/@g"/>
          <xsl:with-param name="b" select="msxsl:node-set($rgb_corrected)/rgb_corrected/@b"/>
          <xsl:with-param name="palette" select="msxsl:node-set($palette)"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="getClosestIcvIndex">
          <xsl:with-param name="r" select="$r"/>
          <xsl:with-param name="g" select="$g"/>
          <xsl:with-param name="b" select="$b"/>
          <xsl:with-param name="palette" select="msxsl:node-set($palette)"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
    
  </xsl:template>

  <xsl:template name="Palette">
    <xsl:variable name="indexedColors" select="$stylesheet_file/main:styleSheet/main:colors/main:indexedColors"/>
    <xsl:if test="$indexedColors">
      <Palette>
        <xsl:for-each select="$indexedColors/main:rgbColor[position() > 8]">
          <LongRGB argb="{@rgb}"/>
        </xsl:for-each>
      </Palette>
    </xsl:if>
  </xsl:template>

  <xsl:template name="ClrtClient"> <!-- Left static untill Charts are implemented -->
    <ClrtClient>
      <LongRGB argb="00000000"/>
      <LongRGB argb="00FFFFFF"/>
      <LongRGB argb="00000000"/>
    </ClrtClient>
  </xsl:template>

  <xsl:template name="applyTintToRGB">
    <xsl:param name="r"/>
    <xsl:param name="g"/>
    <xsl:param name="b"/>
    <xsl:param name="tint">0</xsl:param>
    <xsl:if test="$tint and $tint != 0">
      <xsl:variable name="max">
        <xsl:choose>
          <xsl:when test="$r > $g and $r > $b"><xsl:value-of select="$r"/></xsl:when>
          <xsl:when test="$g >= $r and $g > $b"><xsl:value-of select="$g"/></xsl:when>
          <xsl:otherwise><xsl:value-of select="$b"/></xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <xsl:variable name="min">
        <xsl:choose>
          <xsl:when test="$r &lt; $g and $r &lt; $b"><xsl:value-of select="$r"/></xsl:when>
          <xsl:when test="$g &lt;= $r and $g &lt; $b"><xsl:value-of select="$g"/></xsl:when>
          <xsl:otherwise><xsl:value-of select="$b"/></xsl:otherwise>
        </xsl:choose>
      </xsl:variable>

      <xsl:variable name="h">
        <xsl:choose>
          <xsl:when test="$max = $min">0</xsl:when> <!-- Undefined -->
          <xsl:when test="$max = $r and $g >= $b"><xsl:value-of select="($g - $b) div (6 * ($max - $min))"/></xsl:when>
          <xsl:when test="$max = $r and $g &lt; $b"><xsl:value-of select="($g - $b) div (6 * ($max - $min)) + 1"/></xsl:when>
          <xsl:when test="$max = $g"><xsl:value-of select="($b - $r) div (6 * ($max - $min)) + 1 div 3"/></xsl:when>
          <xsl:when test="$max = $b"><xsl:value-of select="($r - $g) div (6 * ($max - $min)) + 2 div 3"/></xsl:when>
        </xsl:choose>
      </xsl:variable>
      
      <xsl:variable name="l" select="($max + $min) div 2 div 255"/>
      
      <xsl:variable name="s">
        <xsl:choose>
          <xsl:when test="$l = 0">0</xsl:when>
          <xsl:when test="$l > 0 and $l &lt;= 0.5"><xsl:value-of select="($max - $min) div ($max + $min)"/></xsl:when>
          <xsl:when test="$l > 0.5 and $l &lt; 1"><xsl:value-of select="($max - $min) div (2 * 255 - ($max + $min))"/></xsl:when>
          <xsl:when test="$l = 1">0</xsl:when>
        </xsl:choose>
      </xsl:variable>
      <!--
      <xsl:message>RGB(<xsl:value-of select="$r"/>, <xsl:value-of select="$g"/>, <xsl:value-of select="$b"/>) = HSL(<xsl:value-of select="round($h * 240)"/>, <xsl:value-of select="round($s * 240)"/>, <xsl:value-of select="round($l * 240)"/>) tint=<xsl:value-of select="$tint"/></xsl:message>
      -->
      <xsl:variable name="l_corrected">
        <xsl:choose>
          <xsl:when test="$tint &lt; 0"><xsl:value-of select="$l * (1 + $tint)"/></xsl:when>
          <xsl:when test="$tint > 0"><xsl:value-of select="$l * (1 - $tint) + $tint"/></xsl:when>
          <xsl:otherwise><xsl:value-of select="$l"/></xsl:otherwise>
        </xsl:choose>
      </xsl:variable>


      <xsl:variable name="q">
        <xsl:choose>
          <xsl:when test="$l_corrected &lt; 0.5"><xsl:value-of select="$l_corrected * (1 + $s)"/></xsl:when>
          <xsl:when test="$l_corrected >= 0.5"><xsl:value-of select="$l_corrected + $s - ($l_corrected * $s)"/></xsl:when>
        </xsl:choose>
      </xsl:variable>

      <xsl:variable name="p" select="2 * $l_corrected - $q"/>
      <xsl:variable name="Tr" select="$h + 1 div 3"/>
      <xsl:variable name="Tg" select="$h"/>
      <xsl:variable name="Tb" select="$h - 1 div 3"/>
      
      <xsl:variable name="Tr01">
        <xsl:choose>
          <xsl:when test="$Tr &lt; 0"><xsl:value-of select="$Tr + 1"/></xsl:when>
          <xsl:when test="$Tr > 1"><xsl:value-of select="$Tr - 1"/></xsl:when>
          <xsl:otherwise><xsl:value-of select="$Tr"/></xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <xsl:variable name="Tg01">
        <xsl:choose>
          <xsl:when test="$Tg &lt; 0"><xsl:value-of select="$Tg + 1"/></xsl:when>
          <xsl:when test="$Tg > 1"><xsl:value-of select="$Tg - 1"/></xsl:when>
          <xsl:otherwise><xsl:value-of select="$Tg"/></xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <xsl:variable name="Tb01">
        <xsl:choose>
          <xsl:when test="$Tb &lt; 0"><xsl:value-of select="$Tb + 1"/></xsl:when>
          <xsl:when test="$Tb > 1"><xsl:value-of select="$Tb - 1"/></xsl:when>
          <xsl:otherwise><xsl:value-of select="$Tb"/></xsl:otherwise>
        </xsl:choose>
      </xsl:variable>

      <xsl:variable name="r_out">
        <xsl:choose>
          <xsl:when test="$Tr01 &lt; 1 div 6"><xsl:value-of select="($p + ($q - $p) * 6 * $Tr01) * 255"/></xsl:when>
          <xsl:when test="$Tr01 >= 1 div 6 and $Tr01 &lt; 1 div 2"><xsl:value-of select="$q * 255"/></xsl:when>
          <xsl:when test="$Tr01 >= 1 div 2 and $Tr01 &lt; 2 div 3"><xsl:value-of select="($p + ($q - $p) * 6 * (2 div 3 - $Tr01)) * 255"/></xsl:when>
          <xsl:otherwise><xsl:value-of select="$p * 255"/></xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <xsl:variable name="g_out">
        <xsl:choose>
          <xsl:when test="$Tg01 &lt; 1 div 6"><xsl:value-of select="($p + ($q - $p) * 6 * $Tg01) * 255"/></xsl:when>
          <xsl:when test="$Tg01 >= 1 div 6 and $Tg01 &lt; 1 div 2"><xsl:value-of select="$q * 255"/></xsl:when>
          <xsl:when test="$Tg01 >= 1 div 2 and $Tg01 &lt; 2 div 3"><xsl:value-of select="($p + ($q - $p) * 6 * (2 div 3 - $Tg01)) * 255"/></xsl:when>
          <xsl:otherwise><xsl:value-of select="$p * 255"/></xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <xsl:variable name="b_out">
        <xsl:choose>
          <xsl:when test="$Tb01 &lt; 1 div 6"><xsl:value-of select="($p + ($q - $p) * 6 * $Tb01) * 255"/></xsl:when>
          <xsl:when test="$Tb01 >= 1 div 6 and $Tb01 &lt; 1 div 2"><xsl:value-of select="$q * 255"/></xsl:when>
          <xsl:when test="$Tb01 >= 1 div 2 and $Tb01 &lt; 2 div 3"><xsl:value-of select="($p + ($q - $p) * 6 * (2 div 3 - $Tb01)) * 255"/></xsl:when>
          <xsl:otherwise><xsl:value-of select="$p * 255"/></xsl:otherwise>
        </xsl:choose>
      </xsl:variable>

      <rgb_corrected r="{round($r_out)}" g="{round($g_out)}" b="{round($b_out)}"/>
      <!--
      <xsl:message>HSL(<xsl:value-of select="round($h * 240)"/>, <xsl:value-of select="round($s * 240)"/>, <xsl:value-of select="round($l_corrected * 240)"/>) = RGB(<xsl:value-of select="round($r_out)"/>, <xsl:value-of select="round($g_out)"/>, <xsl:value-of select="round($b_out)"/>)</xsl:message>
      -->
    </xsl:if>

  </xsl:template>
  
  <xsl:template name="UserBView">
    <xsl:for-each select="$workbook_file/main:workbook/main:customWorkbookViews/main:customWorkbookView">
      <UserBView tabId="{@activeSheetId}" guid="{@guid}" dx="{@windowWidth}" dy="{@windowHeight}">
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">x</xsl:with-param>
          <xsl:with-param name="val" select="@xWindow"/>
          <xsl:with-param name="def_val">0</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">y</xsl:with-param>
          <xsl:with-param name="val" select="@yWindow"/>
          <xsl:with-param name="def_val">0</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">wTabRatio</xsl:with-param>
          <xsl:with-param name="val" select="@tabRatio"/>
          <xsl:with-param name="def_val">600</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_bool_req">
          <xsl:with-param name="name">fDspFmlaBar</xsl:with-param>
          <xsl:with-param name="val" select="@showFormulaBar"/>
          <xsl:with-param name="def_val">true</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_bool_req">
          <xsl:with-param name="name">fDspStatus</xsl:with-param>
          <xsl:with-param name="val" select="@showStatusbar"/>
          <xsl:with-param name="def_val">true</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">mdNoteDisp</xsl:with-param>
          <xsl:with-param name="val" select="@showComments"/>
          <xsl:with-param name="def_val">commIndicator</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_bool_req">
          <xsl:with-param name="name">fDspHScroll</xsl:with-param>
          <xsl:with-param name="val" select="@showHorizontalScroll"/>
          <xsl:with-param name="def_val">true</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_bool_req">
          <xsl:with-param name="name">fDspVScroll</xsl:with-param>
          <xsl:with-param name="val" select="@showVerticalScroll"/>
          <xsl:with-param name="def_val">true</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_bool_req">
          <xsl:with-param name="name">fBotAdornment</xsl:with-param>
          <xsl:with-param name="val" select="@showSheetTabs"/>
          <xsl:with-param name="def_val">true</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_bool_req">
          <xsl:with-param name="name">fZoom</xsl:with-param>
          <xsl:with-param name="val" select="@maximized"/>
          <xsl:with-param name="def_val">false</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">fHideObj</xsl:with-param>
          <xsl:with-param name="val" select="@showObjects"/>
          <xsl:with-param name="def_val">all</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_bool_req">
          <xsl:with-param name="name">fPrintIncl</xsl:with-param>
          <xsl:with-param name="val" select="@includePrintSettings"/>
          <xsl:with-param name="def_val">true</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_bool_req">
          <xsl:with-param name="name">fRowColIncl</xsl:with-param>
          <xsl:with-param name="val" select="@includeHiddenRowCol"/>
          <xsl:with-param name="def_val">true</xsl:with-param>
        </xsl:call-template>
        <xsl:attribute name="fInvalidTabId">false</xsl:attribute>
        <xsl:call-template name="attrib_bool_req">
          <xsl:with-param name="name">fTimedUpdate</xsl:with-param>
          <xsl:with-param name="val" select="@autoUpdate"/>
          <xsl:with-param name="def_val">false</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_bool_req">
          <xsl:with-param name="name">fAllMemChanges</xsl:with-param>
          <xsl:with-param name="val" select="@changesSavedWin"/>
          <xsl:with-param name="def_val">false</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_bool_req">
          <xsl:with-param name="name">fOnlySync</xsl:with-param>
          <xsl:with-param name="val" select="@onlySync"/>
          <xsl:with-param name="def_val">false</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_bool_req">
          <xsl:with-param name="name">fPersonalView</xsl:with-param>
          <xsl:with-param name="val" select="@personalView"/>
          <xsl:with-param name="def_val">false</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_bool_req">
          <xsl:with-param name="name">fIconic</xsl:with-param>
          <xsl:with-param name="val" select="@minimized"/>
          <xsl:with-param name="def_val">false</xsl:with-param>
        </xsl:call-template>
        <xsl:if test="(@personalView = 'true' or @personalView = '1') and (@autoUpdate = 'true' or @autoUpdate = '1')">
          <xsl:call-template name="attrib_req_with_def">
            <xsl:with-param name="name">wMergeInterval</xsl:with-param>
            <xsl:with-param name="val" select="@mergeInterval"/>
            <xsl:with-param name="def_val">5</xsl:with-param>
          </xsl:call-template>
        </xsl:if>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">st</xsl:with-param>
          <xsl:with-param name="val" select="@name"/>
        </xsl:call-template>
      </UserBView>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="LBL">
    <xsl:for-each select="$workbook_file/main:workbook/main:definedNames/main:definedName">
      <LBL>
        <Lbl>
          <xsl:call-template name="attrib_bool_req">
            <xsl:with-param name="name">fHidden</xsl:with-param>
            <xsl:with-param name="val" select="@hidden"/>
          </xsl:call-template>
          <xsl:call-template name="attrib_bool_req">
            <xsl:with-param name="name">fFunc</xsl:with-param>
            <xsl:with-param name="val" select="@xlm"/>
          </xsl:call-template>
          <xsl:call-template name="attrib_bool_req">
            <xsl:with-param name="name">fOB</xsl:with-param>
            <xsl:with-param name="val" select="@vbProcedure"/>
          </xsl:call-template>
          <xsl:attribute name="fProc">
            <xsl:value-of select="boolean(@xlm = 'true' or @xlm = '1' or @vbProcedure = 'true' or @vbProcedure = '1')"/>
          </xsl:attribute>
          <xsl:attribute name="fCalcExp">
            <xsl:value-of select="boolean(starts-with(., '{') and substring(., string-length(.), 1) = '}')"/>
          </xsl:attribute>
          <xsl:attribute name="fBuiltin">
            <xsl:value-of select="starts-with(@name, '_xlnm.')"/>
          </xsl:attribute>
          <xsl:call-template name="attrib_req_with_def">
            <xsl:with-param name="name">fGrp</xsl:with-param>
            <xsl:with-param name="val" select="@functionGroupId"/>
            <xsl:with-param name="def_val">0</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_bool_req">
            <xsl:with-param name="name">fPublished</xsl:with-param>
            <xsl:with-param name="val" select="@publishToServer"/>
          </xsl:call-template>
          <xsl:call-template name="attrib_bool_req">
            <xsl:with-param name="name">fWorkbookParam</xsl:with-param>
            <xsl:with-param name="val" select="@workbookParameter"/>
          </xsl:call-template>
          <xsl:call-template name="attrib_req_with_def">
            <xsl:with-param name="name">chKey</xsl:with-param>
            <xsl:with-param name="val" select="@shortcutKey"/>
            <xsl:with-param name="def_val">0</xsl:with-param>
          </xsl:call-template>
          <xsl:attribute name="itab">
            <xsl:choose>
              <xsl:when test="@localSheetId"><xsl:value-of select="@localSheetId + 1"/></xsl:when>
              <xsl:otherwise>0</xsl:otherwise>
            </xsl:choose>
          </xsl:attribute>
          <xsl:call-template name="attrib_req_with_def">
            <xsl:with-param name="name">Name</xsl:with-param>
            <xsl:with-param name="val" select="@name"/>
          </xsl:call-template>
          <NameParsedFormula assembled_formula="{.}"/>
        </Lbl>
        <xsl:if test="@comment">
          <NameCmt comment="@comment"/>
        </xsl:if>
      </LBL>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="SUPBOOK">
    <xsl:variable name="f_set" select="$worksheets_files/main:worksheet/main:sheetData/main:row/main:c/main:f"/>
    <xsl:variable name="dn_set" select="$workbook_file/main:workbook/main:definedNames/main:definedName"/>
    <xsl:variable name="cf_set" select="$worksheets_files/main:worksheet/main:conditionalFormatting/main:cfRule/main:formula"/>
    <SUPBOOK>
      <SupBook ctab="{count($worksheets_files/main:worksheet)}" cch="1025" virtPath="" rgst=""/>
    </SUPBOOK>
    <SUPBOOK>
      <SupBook ctab="1" cch="14849" virtPath="" rgst=""/>
        <xsl:for-each select="$f_set[contains(., '(')]">
          <xsl:call-template name="find_UDF_and_gen_ExternName">
            <xsl:with-param name="str" select="."/>
          </xsl:call-template>
        </xsl:for-each>
        <xsl:for-each select="$dn_set[contains(., '(')]">
          <xsl:call-template name="find_UDF_and_gen_ExternName">
            <xsl:with-param name="str" select="."/>
          </xsl:call-template>
        </xsl:for-each>
        <xsl:for-each select="$cf_set[contains(., '(')]">
          <xsl:call-template name="find_UDF_and_gen_ExternName">
            <xsl:with-param name="str" select="."/>
          </xsl:call-template>
        </xsl:for-each>
      
      <ExternSheet>
        <xsl:for-each select="$f_set[contains(., '!')]">
          <xsl:call-template name="find_3D_and_gen_XTI">
            <xsl:with-param name="str" select="."/>
          </xsl:call-template>
        </xsl:for-each>
        <xsl:for-each select="$dn_set[contains(., '!')]">
          <xsl:call-template name="find_3D_and_gen_XTI">
            <xsl:with-param name="str" select="."/>
          </xsl:call-template>
        </xsl:for-each>
        <xsl:for-each select="$cf_set[contains(., '!')]">
          <xsl:call-template name="find_3D_and_gen_XTI">
            <xsl:with-param name="str" select="."/>
          </xsl:call-template>
        </xsl:for-each>
        <XTI iSupBook="1" itabFirst="-2" itabLast="-2"/>
      </ExternSheet>
    </SUPBOOK>
  </xsl:template>

  <xsl:variable name="apos">&apos;</xsl:variable>
  <xsl:variable name="apos2">&apos;&apos;</xsl:variable>
  <xsl:template name="find_3D_and_gen_XTI">
    <xsl:param name="str"/>
    <!--<xsl:message>find_3D_and_gen_XTI("<xsl:value-of select="$str"/>")</xsl:message>-->
    <xsl:if test="contains($str, '!')">
      <xsl:variable name="apos_excl">&apos;!</xsl:variable>
      <xsl:variable name="simplier_str">
        <xsl:call-template name="substr_inline_strings_with_000">
          <xsl:with-param name="str" select="$str"/>
        </xsl:call-template>
      </xsl:variable>
      <xsl:variable name="before" select="substring-before($simplier_str, '!')"/>
      <xsl:choose>
        <xsl:when test="substring($before, string-length($before), 1) = $apos">
          <xsl:call-template name="gen_XTI">
            <xsl:with-param name="str" select="substring-after(substring($before, 1, string-length($before) - 1), $apos)"/>
          </xsl:call-template>
          
        </xsl:when>
        <xsl:otherwise>
          <xsl:variable name="translated_before" select="translate($before, '-+*/^&amp;%&lt;=> ;(,', '+++++++++++++++')"/>
          <xsl:choose>
            <xsl:when test="contains($translated_before, '+')">
              <xsl:call-template name="find_3D_and_gen_XTI">
                <xsl:with-param name="str" select="concat(substring-after($translated_before, '+'), '!')"/>
              </xsl:call-template>
            </xsl:when>
            <xsl:when test="contains(substring-after($translated_before, ':'), ':')">
              <xsl:call-template name="find_3D_and_gen_XTI">
                <xsl:with-param name="str" select="concat(substring-after($translated_before, ':'), '!')"/>
              </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
              <xsl:if test="not($translated_before = '#REF' or $translated_before = '#NULL' or $translated_before = '#DIV/0' or
                                $translated_before = '#VALUE' or $translated_before = '#NUM')">
                <xsl:call-template name="gen_XTI">
                  <xsl:with-param name="str" select="$translated_before"/>
                </xsl:call-template>
              </xsl:if>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:otherwise>
      </xsl:choose>

      <xsl:call-template name="find_3D_and_gen_XTI">
        <xsl:with-param name="str" select="substring-after($str, '!')"/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

  <xsl:template name="gen_XTI">
    <xsl:param name="str"/>
    <!--<xsl:message>gen_XTI("<xsl:value-of select="$str"/>")</xsl:message>-->
    <xsl:variable name="escaped_str">
      <xsl:call-template name="remove_double_apos">
        <xsl:with-param name="str" select="$str"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="contains($escaped_str, ':')">
        <xsl:variable name="sheet_name1" select="substring-before($escaped_str, ':')"/>
        <xsl:variable name="sheet_name2" select="substring-after($escaped_str, ':')"/>
        <xsl:if test="$sheet_name1 != '' and $sheet_name2 != ''">
          <XTI iSupBook="0" itabFirst="{$workbook_file/main:workbook/main:sheets/main:sheet[@name = $sheet_name1]/@sheetId - 1}"
                            itabLast="{$workbook_file/main:workbook/main:sheets/main:sheet[@name = $sheet_name2]/@sheetId - 1}"/>
        </xsl:if>
      </xsl:when>
      <xsl:otherwise>
        <xsl:variable name="sheet_id" select="$workbook_file/main:workbook/main:sheets/main:sheet[@name = $escaped_str]/@sheetId"/>
        <xsl:if test="$sheet_id">
          <XTI iSupBook="0" itabFirst="{$sheet_id - 1}" itabLast="{$sheet_id - 1}"/>
        </xsl:if>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="remove_double_apos">
    <xsl:param name="str"/>
    <xsl:param name="left_str"/>
    <!--<xsl:message>remove_double_apos("<xsl:value-of select="$str"/>", "<xsl:value-of select="$left_str"/>")</xsl:message>-->
    <xsl:choose>
      <xsl:when test="$str = '' or not(contains($str, $apos2))">
        <xsl:value-of select="$left_str"/>
        <xsl:value-of select="$str"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:variable name="apos2_pos" select="string-length(substring-before($str, $apos2)) + 1"/>
        <xsl:call-template name="remove_double_apos">
          <xsl:with-param name="str" select="substring($str, $apos2_pos + 2)"/>
          <xsl:with-param name="left_str" select="concat($left_str, substring($str, 1, $apos2_pos))"/>
        </xsl:call-template>
        </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  
  
  <xsl:template name="find_UDF_and_gen_ExternName">
    <xsl:param name="str"/>
    <xsl:if test="contains($str, '(')">
    <!--<xsl:message>find_UDF_and_gen_ExternName("<xsl:value-of select="$str"/>")</xsl:message>-->
      <xsl:variable name="simplier_str">
        <xsl:call-template name="substr_inline_strings_with_000">
          <xsl:with-param name="str" select="$str"/>
        </xsl:call-template>
      </xsl:variable>
      <xsl:variable name="before" select="substring-before($simplier_str, '(')"/>
      <xsl:if test="$before != ''">
        <xsl:variable name="translated_before" select="translate($before, '-+*/^&amp;%&lt;=> ;:,', '+++++++++++++++')"/>
        <xsl:choose>
          <xsl:when test="contains($translated_before, '+')">
            <xsl:call-template name="find_UDF_and_gen_ExternName">
              <xsl:with-param name="str" select="concat(substring-after($translated_before, '+'), '(')"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <xsl:if test="not(msxsl:node-set($BuiltInFunctions)/main:BuiltInFunctions/main:func[@name = $translated_before])">
              <!--<xsl:message>Generate ExternName("<xsl:value-of select="$translated_before"/>")</xsl:message>-->
              <ExternName fBuiltIn="false" fWantAdvise="false" fWantPict="false" fOle="false" fOleLink="false" cf="0" fIcon="false">
                <AddinUdf udfName="{$translated_before}"/>
              </ExternName>
            </xsl:if>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:if>

      <xsl:call-template name="find_UDF_and_gen_ExternName">
        <xsl:with-param name="str" select="substring-after($str, '(')"/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

  <xsl:variable name="quot">&quot;</xsl:variable>
  <xsl:variable name="quot2">&quot;&quot;</xsl:variable>

  <xsl:template name="substr_inline_strings_with_000">
    <xsl:param name="str"/>
    <!--<xsl:message>remove_double_apos("<xsl:value-of select="$str"/>", "<xsl:value-of select="$left_str"/>")</xsl:message>-->
    <xsl:choose>
      <xsl:when test="contains($str, $quot2)">
        <xsl:call-template name="substr_inline_strings_with_000">
          <xsl:with-param name="str" select="concat(substring-before($str, $quot2), '000', substring-after($str, $quot2))"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="contains($str, $quot)">
        <xsl:call-template name="substr_inline_strings_with_000">
          <xsl:with-param name="str" select="concat(substring-before($str, $quot), '000', substring-after(substring-after($str, $apos), $quot))"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$str"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>


</xsl:stylesheet>