<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:xlsx="http://www.avs4you.ru/XLSXConverter/1.0/DirectoryStructure"
                xmlns:r="http://schemas.openxmlformats.org/package/2006/relationships"
                xmlns:mainr="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
                xmlns:main="http://schemas.openxmlformats.org/spreadsheetml/2006/main"
                xmlns:msxsl="urn:schemas-microsoft-com:xslt"
                extension-element-prefixes="msxsl">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>


  <xsl:template name="WorksheetSubstream">
    <xsl:for-each select="$worksheets_files/main:worksheet">
      <xsl:variable name="ws_position" select="position()"/>
      <WorksheetSubstream>
        <BOF vers="1536" dt="16" rupBuild="5612" rupYear="1997" fWin="true" fRisc="false" fBeta="false" 
                        fWinAny="false" fMacAny="false" fBetaAny="false" fRiscAny="false" fOOM="false" fGlJmp="false" 
                        fFontLimit="false" verXLHigh="3" verLowestBiff="6" verLastXLSaved="3"/>
        <xsl:call-template name="Index"/>
        <xsl:call-template name="GLOBALS"/>
        <xsl:call-template name="PAGESETUP">
          <xsl:with-param name="ws_position" select="$ws_position"/>
        </xsl:call-template>
        <xsl:call-template name="COLUMNS"/>
        <Dimensions ref="{main:dimension/@ref}"/>
        <xsl:call-template name="CELLTABLE"/>
        <xsl:call-template name="WINDOW"/>
        <xsl:call-template name="CUSTOMVIEW">
          <xsl:with-param name="ws_position" select="$ws_position"/>
        </xsl:call-template>
        <xsl:call-template name="DxGCol"/>
        <xsl:call-template name="MergeCells"/>
        <xsl:call-template name="CONDFMTS"/>
        <xsl:call-template name="HLINK">
          <xsl:with-param name="ws_position" select="$ws_position"/>
        </xsl:call-template>
        
        <EOF/>
      </WorksheetSubstream>
      
    </xsl:for-each>
  </xsl:template>


  <xsl:template name="Index">
    <Index>
      <xsl:choose>
        <xsl:when test="main:sheetData/main:row">
          <xsl:attribute name="rwMic"><xsl:value-of select="main:sheetData/main:row[position() = 1]/@r - 1"/></xsl:attribute>
          <xsl:attribute name="rwMac"><xsl:value-of select="main:sheetData/main:row[position() = last()]/@r - 1"/></xsl:attribute>
        </xsl:when>
        <xsl:otherwise>
          <xsl:attribute name="rwMic">0</xsl:attribute>
          <xsl:attribute name="rwMac">0</xsl:attribute>
        </xsl:otherwise>
      </xsl:choose>
      <!-- The following value substitute 'FilePointer's generation and is handled in C++ -->
      <xsl:attribute name="num_pointers"><xsl:value-of select="floor((count(main:sheetData/main:row) + 31) div 32)"/></xsl:attribute>
    </Index>
  </xsl:template>

  <xsl:template name="GLOBALS">
    <GLOBALS>
      <CalcMode>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">fAutoRecalc</xsl:with-param>
          <xsl:with-param name="val" select="$workbook_file/main:workbook/main:calcPr/@calcMode"/>
          <xsl:with-param name="def_val">auto</xsl:with-param>
        </xsl:call-template>
      </CalcMode>
      <CalcCount>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">cIter</xsl:with-param>
          <xsl:with-param name="val" select="$workbook_file/main:workbook/main:calcPr/@iterateCount"/>
          <xsl:with-param name="def_val">100</xsl:with-param>
        </xsl:call-template>
      </CalcCount>
      <CalcRefMode>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">fRefA1</xsl:with-param>
          <xsl:with-param name="val" select="$workbook_file/main:workbook/main:calcPr/@refMode"/>
          <xsl:with-param name="def_val">A1</xsl:with-param>
        </xsl:call-template>
      </CalcRefMode>
      <CalcIter>
        <xsl:call-template name="attrib_bool_req">
          <xsl:with-param name="name">vfIter</xsl:with-param>
          <xsl:with-param name="val" select="$workbook_file/main:workbook/main:calcPr/@iterate"/>
        </xsl:call-template>
      </CalcIter>
      <CalcDelta>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">numDelta</xsl:with-param>
          <xsl:with-param name="val" select="$workbook_file/main:workbook/main:calcPr/@iterateDelta"/>
          <xsl:with-param name="def_val">0.001</xsl:with-param>
        </xsl:call-template>
      </CalcDelta>
      <CalcSaveRecalc>
        <xsl:call-template name="attrib_bool_req">
          <xsl:with-param name="name">fSaveRecalc</xsl:with-param>
          <xsl:with-param name="val" select="$workbook_file/main:workbook/main:calcPr/@calcOnSave"/>
          <xsl:with-param name="def_val">true</xsl:with-param>
        </xsl:call-template>
      </CalcSaveRecalc>
      <PrintRowCol>
        <xsl:call-template name="attrib_bool_req">
          <xsl:with-param name="name">printRwCol</xsl:with-param>
          <xsl:with-param name="val" select="main:printOptions/@headings"/>
        </xsl:call-template>
      </PrintRowCol>
      <PrintGrid>
        <xsl:call-template name="attrib_bool_req">
          <xsl:with-param name="name">fPrintGrid</xsl:with-param>
          <xsl:with-param name="val" select="main:printOptions/@gridLines"/>
        </xsl:call-template>
      </PrintGrid>
      <GridSet/>
      <Guts>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">iLevelRwMac</xsl:with-param>
          <xsl:with-param name="val" select="main:sheetFormatPr/@outlineLevelRow"/>
          <xsl:with-param name="def_val">0</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">iLevelColMac</xsl:with-param>
          <xsl:with-param name="val" select="main:sheetFormatPr/@outlineLevelCol"/>
          <xsl:with-param name="def_val">0</xsl:with-param>
        </xsl:call-template>
      </Guts>
      <xsl:call-template name="DefaultRowHeight"/>
      <xsl:call-template name="WsBool"/>
      <xsl:call-template name="Sync"/>
      <xsl:call-template name="HorizontalPageBreaks"/>
      <xsl:call-template name="VerticalPageBreaks"/>
      
   </GLOBALS>
  </xsl:template>

  <xsl:template name="DefaultRowHeight">
    <DefaultRowHeight>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fUnsynced</xsl:with-param>
        <xsl:with-param name="val" select="main:sheetFormatPr/@customHeigh"/>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fDyZero</xsl:with-param>
        <xsl:with-param name="val" select="main:sheetFormatPr/@zeroHeight"/>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fExAsc</xsl:with-param>
        <xsl:with-param name="val" select="main:sheetFormatPr/@thickTop"/>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fExDsc</xsl:with-param>
        <xsl:with-param name="val" select="main:sheetFormatPr/@thickBottom"/>
      </xsl:call-template>
      <xsl:choose>
        <xsl:when test="not(main:sheetFormatPr/@defaultRowHeight)">
          <xsl:attribute name="miyRw"><xsl:value-of select="200"/></xsl:attribute> <!--workaround-->
        </xsl:when>
        <xsl:when test="not(main:sheetFormatPr/@zeroHeight = 'true' or main:sheetFormatPr/@zeroHeight = '1')">
          <xsl:attribute name="miyRw"><xsl:value-of select="main:sheetFormatPr/@defaultRowHeight * 20"/></xsl:attribute>
        </xsl:when>
        <xsl:otherwise>
          <xsl:attribute name="miyRwHidden"><xsl:value-of select="main:sheetFormatPr/@defaultRowHeight * 20"/></xsl:attribute>
        </xsl:otherwise>
        
      </xsl:choose>
    </DefaultRowHeight>
  </xsl:template>
  
  <xsl:template name="WsBool">
    <WsBool>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fShowAutoBreaks</xsl:with-param>
        <xsl:with-param name="val" select="main:sheetPr/main:pageSetUpPr/@autoPageBreaks"/>
        <xsl:with-param name="def_val">true</xsl:with-param>
      </xsl:call-template>
      <xsl:attribute name="fDialog"><xsl:value-of select="boolean(self::main:dialogsheet)"/></xsl:attribute>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fApplyStyles</xsl:with-param>
        <xsl:with-param name="val" select="main:sheetPr/main:outlinePr/@applyStyles"/>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fRowSumsBelow</xsl:with-param>
        <xsl:with-param name="val" select="main:sheetPr/main:outlinePr/@summaryBelow"/>
        <xsl:with-param name="def_val">true</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fColSumsRight</xsl:with-param>
        <xsl:with-param name="val" select="main:sheetPr/main:outlinePr/@summaryRight"/>
        <xsl:with-param name="def_val">true</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fFitToPage</xsl:with-param>
        <xsl:with-param name="val" select="main:sheetPr/main:pageSetUpPr/@fitToPage"/>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fDspGuts</xsl:with-param>
        <xsl:with-param name="val" select="main:sheetPr/main:outlinePr/@showOutlineSymbols"/>
        <xsl:with-param name="def_val">true</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fSyncHoriz</xsl:with-param>
        <xsl:with-param name="val" select="main:sheetPr/@syncHorizontal"/>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fSyncVert</xsl:with-param>
        <xsl:with-param name="val" select="main:sheetPr/@syncVertical"/>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fAltExprEval</xsl:with-param>
        <xsl:with-param name="val" select="main:sheetPr/@transitionEvaluation"/>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fAltFormulaEntry</xsl:with-param>
        <xsl:with-param name="val" select="main:sheetPr/@transitionEntry"/>
      </xsl:call-template>
    </WsBool>
  </xsl:template>
  
  <xsl:template name="PAGESETUP">
    <xsl:param name="ws_position"/>
    <PAGESETUP>
      <Header ast="{main:headerFooter/main:oddHeader}"/>
      <Footer ast="{main:headerFooter/main:oddFooter}"/>
      <HCenter hcenter="{boolean(main:printOptions/@horizontalCentered = 'true' or main:printOptions/@horizontalCentered = '1')}"/>
      <VCenter vcenter="{boolean(main:printOptions/@verticalCentered = 'true' or main:printOptions/@verticalCentered = '1')}"/>
      <xsl:if test="main:pageMargins">
        <LeftMargin num="{main:pageMargins/@left}"/>
        <RightMargin num="{main:pageMargins/@right}"/>
        <TopMargin num="{main:pageMargins/@top}"/>
        <BottomMargin num="{main:pageMargins/@bottom}"/>
      </xsl:if>
      <xsl:call-template name="Pls">
        <xsl:with-param name="ws_position" select="$ws_position"/>
      </xsl:call-template>
      <xsl:call-template name="Setup"/>
      <xsl:call-template name="HeaderFooter"/>
    </PAGESETUP>
  </xsl:template>

  <xsl:template name="Setup">
    <Setup>
      <xsl:choose>
        <xsl:when test="main:pageSetup/@mainr:id"> <!-- Binary data present -->
          <xsl:call-template name="attrib_req_with_def">
            <xsl:with-param name="name">iPaperSize</xsl:with-param>
            <xsl:with-param name="val" select="main:pageSetup/@paperSize"/>
            <xsl:with-param name="def_val">1</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_req_with_def">
            <xsl:with-param name="name">iScale</xsl:with-param>
            <xsl:with-param name="val" select="main:pageSetup/@scale"/>
            <xsl:with-param name="def_val">100</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_req_with_def">
            <xsl:with-param name="name">iRes</xsl:with-param>
            <xsl:with-param name="val" select="main:pageSetup/@horizontalDpi"/>
            <xsl:with-param name="def_val">600</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_req_with_def">
            <xsl:with-param name="name">iVRes</xsl:with-param>
            <xsl:with-param name="val" select="main:pageSetup/@verticalDpi"/>
            <xsl:with-param name="def_val">600</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_req_with_def">
            <xsl:with-param name="name">iCopies</xsl:with-param>
            <xsl:with-param name="val" select="main:pageSetup/@copies"/>
            <xsl:with-param name="def_val">1</xsl:with-param>
          </xsl:call-template>
          <xsl:attribute name="fPortrait">
            <xsl:value-of select="boolean(main:pageSetup/@orientation = 'portrait')"/>
          </xsl:attribute>
          <xsl:attribute name="fNoOrient">
            <xsl:value-of select="boolean(main:pageSetup/@orientation = 'default' or not(main:pageSetup/@orientation))"/>
          </xsl:attribute>
          <xsl:attribute name="fNoPls">false</xsl:attribute>
        </xsl:when>
        <xsl:otherwise> <!-- No binary data -->
          <xsl:attribute name="fNoPls">true</xsl:attribute>
        </xsl:otherwise>
      </xsl:choose>
      <xsl:choose>
        <xsl:when test="main:pageSetup/@useFirstPageNumber = 'true' or main:pageSetup/@useFirstPageNumber = '1'">
          <xsl:call-template name="attrib_req_with_def">
            <xsl:with-param name="name">iPageStart</xsl:with-param>
            <xsl:with-param name="val" select="main:pageSetup/@firstPageNumber"/>
            <xsl:with-param name="def_val">1</xsl:with-param>
          </xsl:call-template>
          <xsl:attribute name="fUsePage">true</xsl:attribute>
        </xsl:when>
        <xsl:otherwise>
          <xsl:attribute name="fUsePage">false</xsl:attribute>
        </xsl:otherwise>
      </xsl:choose>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">iFitWidth</xsl:with-param>
        <xsl:with-param name="val" select="main:pageSetup/@fitToWidth"/>
        <xsl:with-param name="def_val">1</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">iFitHeight</xsl:with-param>
        <xsl:with-param name="val" select="main:pageSetup/@fitToHeight"/>
        <xsl:with-param name="def_val">1</xsl:with-param>
      </xsl:call-template>
      <xsl:attribute name="fLeftToRight">
        <xsl:value-of select="boolean(main:pageSetup/@fitToHeight = 'overThenDown')"/>
      </xsl:attribute>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fNoColor</xsl:with-param>
        <xsl:with-param name="val" select="main:pageSetup/@blackAndWhite"/>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fDraft</xsl:with-param>
        <xsl:with-param name="val" select="main:pageSetup/@draft"/>
      </xsl:call-template>
      <xsl:choose>
        <xsl:when test="main:pageSetup/@blackAndWhite = 'atEnd'">
          <xsl:attribute name="fNotes">true</xsl:attribute>
          <xsl:attribute name="fEndNotes">true</xsl:attribute>
        </xsl:when>
        <xsl:when test="main:pageSetup/@blackAndWhite = 'asDisplayed'">
          <xsl:attribute name="fNotes">true</xsl:attribute>
          <xsl:attribute name="fEndNotes">false</xsl:attribute>
        </xsl:when>
        <xsl:otherwise>  <!-- 'none' or ommited. -->
          <xsl:attribute name="fNotes">false</xsl:attribute>
          <xsl:attribute name="fEndNotes">false</xsl:attribute>
        </xsl:otherwise>
      </xsl:choose>
      <xsl:attribute name="iErrors">
        <xsl:choose>
          <xsl:when test="main:pageSetup/@errors = 'blank'">1</xsl:when>
          <xsl:when test="main:pageSetup/@errors = 'dash'">2</xsl:when>
          <xsl:when test="main:pageSetup/@errors = 'NA'">3</xsl:when>
          <xsl:otherwise>0</xsl:otherwise>  <!-- 'displayed' or ommited -->
        </xsl:choose>
      </xsl:attribute>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">numHdr</xsl:with-param>
        <xsl:with-param name="val" select="main:pageMargins/@header"/>
        <xsl:with-param name="def_val">0</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">numFtr</xsl:with-param>
        <xsl:with-param name="val" select="main:pageMargins/@footer"/>
        <xsl:with-param name="def_val">0</xsl:with-param>
      </xsl:call-template>
    </Setup>
  </xsl:template>
  
  <xsl:template name="COLUMNS">
    <COLUMNS>
      <DefColWidth>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">cchdefColWidth</xsl:with-param>
          <xsl:with-param name="val" select="main:sheetFormatPr/@baseColWidth"/>
          <xsl:with-param name="def_val">8</xsl:with-param>
        </xsl:call-template>
      </DefColWidth>
      <xsl:for-each select="main:cols/main:col[position() &lt; 256]">
        <ColInfo>
          <xsl:call-template name="attrib_req_with_def">
            <xsl:with-param name="name">colFirst</xsl:with-param>
            <xsl:with-param name="val" select="@min - 1"/>
            <xsl:with-param name="def_val">0</xsl:with-param>
          </xsl:call-template>
          <xsl:variable name="colLast">
            <xsl:choose>
              <xsl:when test="@max = 16384">256</xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="@max - 1"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:variable>
          <xsl:call-template name="attrib_req_with_def">
            <xsl:with-param name="name">colLast</xsl:with-param>
            <xsl:with-param name="val" select="$colLast"/>
            <xsl:with-param name="def_val">0</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_req_with_def">
            <xsl:with-param name="name">coldx</xsl:with-param>
            <xsl:with-param name="val" select="@width * 256"/>
            <xsl:with-param name="def_val">0</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_req_with_def">
            <xsl:with-param name="name">ixfe</xsl:with-param>
            <xsl:with-param name="val" select="@style + $cellStyleXfs_count"/>
            <xsl:with-param name="def_val">0</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_bool_req">
            <xsl:with-param name="name">fHidden</xsl:with-param>
            <xsl:with-param name="val" select="@hidden"/>
          </xsl:call-template>
          <xsl:call-template name="attrib_bool_req">
            <xsl:with-param name="name">fUserSet</xsl:with-param>
            <xsl:with-param name="val" select="@customWidth"/>
          </xsl:call-template>
          <xsl:call-template name="attrib_bool_req">
            <xsl:with-param name="name">fBestFit</xsl:with-param>
            <xsl:with-param name="val" select="@bestFit"/>
          </xsl:call-template>
          <xsl:call-template name="attrib_bool_req">
            <xsl:with-param name="name">fPhonetic</xsl:with-param>
            <xsl:with-param name="val" select="@phonetic"/>
          </xsl:call-template>
          <xsl:call-template name="attrib_req_with_def">
            <xsl:with-param name="name">iOutLevel</xsl:with-param>
            <xsl:with-param name="val" select="@outlineLevel"/>
            <xsl:with-param name="def_val">0</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_bool_req">
            <xsl:with-param name="name">fCollapsed</xsl:with-param>
            <xsl:with-param name="val" select="@collapsed"/>
          </xsl:call-template>
        </ColInfo>
      </xsl:for-each>
    </COLUMNS>
  </xsl:template>

  <xsl:template name="WINDOW">
    <xsl:for-each select="main:sheetViews/main:sheetView">
      <WINDOW>
        <xsl:call-template name="Window2"/>
        <xsl:call-template name="PLV"/>
        <xsl:call-template name="Scl"/>
        <xsl:call-template name="Pane"/>
        <xsl:call-template name="Selection"/>
      </WINDOW>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="Window2">
    <Window2>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fSelected</xsl:with-param>
        <xsl:with-param name="val" select="@tabSelected"/>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fDspFmlaRt</xsl:with-param>
        <xsl:with-param name="val" select="@showFormulas"/>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fDspGridRt</xsl:with-param>
        <xsl:with-param name="val" select="@showGridLines"/>
        <xsl:with-param name="def_val">true</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fDspRwColRt</xsl:with-param>
        <xsl:with-param name="val" select="@showRowColHeaders"/>
        <xsl:with-param name="def_val">true</xsl:with-param>
      </xsl:call-template>
      <xsl:choose>
        <xsl:when test="main:pane/@state = 'frozen'">
          <xsl:attribute name="fFrozenRt">true</xsl:attribute>
          <xsl:attribute name="fFrozenNoSplit">true</xsl:attribute>
        </xsl:when>
        <xsl:when test="main:pane/@state = 'frozenSplit'">
          <xsl:attribute name="fFrozenRt">true</xsl:attribute>
          <xsl:attribute name="fFrozenNoSplit">false</xsl:attribute>
        </xsl:when>
        <xsl:otherwise>   <!-- 'split' or ommited -->
          <xsl:attribute name="fFrozenRt">false</xsl:attribute>
          <xsl:attribute name="fFrozenNoSplit">false</xsl:attribute>
        </xsl:otherwise>
      </xsl:choose>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fDspZerosRt</xsl:with-param>
        <xsl:with-param name="val" select="@showZeros"/>
        <xsl:with-param name="def_val">true</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fDefaultHdr</xsl:with-param>
        <xsl:with-param name="val" select="@defaultGridColor"/>
        <xsl:with-param name="def_val">true</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fRightToLeft</xsl:with-param>
        <xsl:with-param name="val" select="@rightToLeft"/>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fDspGuts</xsl:with-param>
        <xsl:with-param name="val" select="@showOutlineSymbols"/>
        <xsl:with-param name="def_val">true</xsl:with-param>
      </xsl:call-template>
      <xsl:attribute name="fPaged">
        <xsl:variable name="w2pos" select="position()"/>
        <xsl:variable name="activeTab">
          <xsl:choose>
            <xsl:when test="$workbook_file/main:workbook/main:bookViews/main:workbookView[position() = $w2pos]/@activeTab">
              <xsl:value-of select="$workbook_file/main:workbook/main:bookViews/main:workbookView[position() = $w2pos]/@activeTab"/>
            </xsl:when>
            <xsl:otherwise>0</xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        <xsl:choose>
          <xsl:when test="$activeTab = count(../../preceding-sibling::main:worksheet)">true</xsl:when>
          <xsl:otherwise>false</xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
      <xsl:attribute name="fSLV"><xsl:value-of select="boolean(@view = 'pageBreakPreview')"/></xsl:attribute>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">topLeftCell</xsl:with-param>
        <xsl:with-param name="val" select="@topLeftCell"/>
        <xsl:with-param name="def_val">A1</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">icvHdr</xsl:with-param>
        <xsl:with-param name="val" select="@colorId"/>
        <xsl:with-param name="def_val">64</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">wScaleSLV</xsl:with-param>
        <xsl:with-param name="val" select="@zoomScaleSheetLayoutView"/>
        <xsl:with-param name="def_val">0</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">wScaleNormal</xsl:with-param>
        <xsl:with-param name="val" select="@zoomScaleNormal"/>
        <xsl:with-param name="def_val">0</xsl:with-param>
      </xsl:call-template>
    </Window2>
  </xsl:template>

  <xsl:template name="Selection">
    <xsl:for-each select="main:selection">
      <Selection>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">pnn</xsl:with-param>
          <xsl:with-param name="val" select="@pane"/>
          <xsl:with-param name="def_val">topLeft</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">activeCell</xsl:with-param>
          <xsl:with-param name="val" select="@activeCell"/>
          <xsl:with-param name="def_val">A1</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">irefAct</xsl:with-param>
          <xsl:with-param name="val" select="@activeCellId"/>
          <xsl:with-param name="def_val">0</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">sqref</xsl:with-param>
          <xsl:with-param name="val" select="@sqref"/>
          <xsl:with-param name="def_val">A1</xsl:with-param>
        </xsl:call-template>
      </Selection>
    </xsl:for-each>
    <xsl:if test="not(main:selection)">
      <Selection pnn="topLeft" activeCell="A1" irefAct="0" sqref="A1"/>
    </xsl:if>
  </xsl:template>
  
  <xsl:template name="CELLTABLE">
    <xsl:if test="main:sheetData/main:row">
      <CELLTABLE>
        <xsl:call-template name="CELL_32_splitter">
          <xsl:with-param name="array_formulas" select="main:sheetData/main:row/main:c/main:f[@t = 'array']"/>
          <xsl:with-param name="table_formulas" select="main:sheetData/main:row/main:c/main:f[@t = 'dataTable']"/>
        </xsl:call-template>
      </CELLTABLE>
    </xsl:if>
  </xsl:template>

  <xsl:variable name="NUM_ROWS_IN_ROW_BLOCK">28</xsl:variable>
  <xsl:template name="CELL_32_splitter">
    <xsl:param name="array_formulas"/>
    <xsl:param name="table_formulas"/>
    <xsl:param name="from_inc">1</xsl:param>
    <xsl:if test="main:sheetData/main:row[position() >= $from_inc and position() &lt; $from_inc + $NUM_ROWS_IN_ROW_BLOCK]">
      <xsl:variable name="defaultRowHeight20" select="main:sheetFormatPr/@defaultRowHeight * 20"/>
      <CELL_GROUP>
        <xsl:for-each select="main:sheetData/main:row[position() >= $from_inc and position() &lt; $from_inc + $NUM_ROWS_IN_ROW_BLOCK]">
          <Row rw="{@r - 1}">
            <xsl:call-template name="attrib_req_with_def">
              <xsl:with-param name="name">colMic</xsl:with-param>
              <xsl:with-param name="val">
                <xsl:call-template name="ref2col">
                  <xsl:with-param name="ref" select="main:c[position() = 1]/@r"/>
                </xsl:call-template>
              </xsl:with-param>
              <xsl:with-param name="def_val">0</xsl:with-param>
            </xsl:call-template>
            <xsl:variable name="last_cell_col">
              <xsl:call-template name="ref2col">
                <xsl:with-param name="ref" select="main:c[position() = last()]/@r"/>
              </xsl:call-template>
            </xsl:variable>
            <xsl:call-template name="attrib_req_with_def">
              <xsl:with-param name="name">colMac</xsl:with-param>
              <xsl:with-param name="val" select="$last_cell_col + 1"/> <!-- +1 because it is 1-based -->
              <xsl:with-param name="def_val">0</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_req_with_def">
              <xsl:with-param name="name">miyRw</xsl:with-param>
              <xsl:with-param name="val" select="@ht * 20"/>
              <xsl:with-param name="def_val"><xsl:value-of select="$defaultRowHeight20"/></xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_req_with_def">
              <xsl:with-param name="name">iOutLevel</xsl:with-param>
              <xsl:with-param name="val" select="@outlineLevel"/>
              <xsl:with-param name="def_val">0</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_bool_req">
              <xsl:with-param name="name">fCollapsed</xsl:with-param>
              <xsl:with-param name="val" select="@collapsed"/>
            </xsl:call-template>
            <xsl:call-template name="attrib_bool_req">
              <xsl:with-param name="name">fDyZero</xsl:with-param>
              <xsl:with-param name="val" select="@hidden"/>
            </xsl:call-template>
            <xsl:call-template name="attrib_bool_req">
              <xsl:with-param name="name">fUnsynced</xsl:with-param>
              <xsl:with-param name="val" select="boolean(@customHeight) or @ht"/>
            </xsl:call-template>
            <xsl:call-template name="attrib_bool_req">
              <xsl:with-param name="name">fGhostDirty</xsl:with-param>
              <xsl:with-param name="val" select="@customFormat"/>
            </xsl:call-template>
            <xsl:choose>
              <xsl:when test="@customFormat = 'true' or @customFormat = '1'">
                <xsl:attribute name="fGhostDirty">true</xsl:attribute>
                <xsl:variable name="s">
                  <xsl:choose>
                    <xsl:when test="@s"><xsl:value-of select="@s"/></xsl:when>
                    <xsl:otherwise>0</xsl:otherwise>
                  </xsl:choose>
                </xsl:variable>
                <xsl:attribute name="ixfe_val">
                  <xsl:value-of select="$s + $cellStyleXfs_count"/>
                </xsl:attribute>
              </xsl:when>
              <xsl:otherwise>
                <xsl:attribute name="fGhostDirty">false</xsl:attribute>
                <xsl:attribute name="ixfe_val">0</xsl:attribute>              
              </xsl:otherwise>
            </xsl:choose>
            <xsl:call-template name="attrib_bool_req">
              <xsl:with-param name="name">fExAsc</xsl:with-param>
              <xsl:with-param name="val" select="@thickTop"/>
            </xsl:call-template>
            <xsl:call-template name="attrib_bool_req">
              <xsl:with-param name="name">fExDes</xsl:with-param>
              <xsl:with-param name="val" select="@thickBot"/>
            </xsl:call-template>
            <xsl:call-template name="attrib_bool_req">
              <xsl:with-param name="name">fPhonetic</xsl:with-param>
              <xsl:with-param name="val" select="@ph"/>
            </xsl:call-template>
            <!-- The following value substitute 'FilePointer's generation and is handled in C++ -->
            <xsl:attribute name="in_block_position"><xsl:value-of select="position()"/></xsl:attribute>
          </Row>
        </xsl:for-each>
        <xsl:for-each select="main:sheetData/main:row[position() >= $from_inc and position() &lt; $from_inc + $NUM_ROWS_IN_ROW_BLOCK]/main:c">
          <xsl:variable name="ref" select="@r"/>
          <xsl:variable name="row">
            <xsl:call-template name="ref2row">
              <xsl:with-param name="ref" select="$ref"/>
            </xsl:call-template>
          </xsl:variable>
          <xsl:variable name="col">
            <xsl:call-template name="ref2col">
              <xsl:with-param name="ref" select="$ref"/>
            </xsl:call-template>
          </xsl:variable>
          <CELL>
            <xsl:variable name="master_c_array_ref">
              <xsl:call-template name="find_master_c_ref">
                <xsl:with-param name="array" select="$array_formulas"/>
                <xsl:with-param name="row" select="$row"/>
                <xsl:with-param name="col" select="$col"/>
                <xsl:with-param name="ref" select="$ref"/>
              </xsl:call-template>
            </xsl:variable>
            <xsl:variable name="master_c_table_ref">
              <xsl:call-template name="find_master_c_ref">
                <xsl:with-param name="array" select="$table_formulas"/>
                <xsl:with-param name="row" select="$row"/>
                <xsl:with-param name="col" select="$col"/>
                <xsl:with-param name="ref" select="$ref"/>
              </xsl:call-template>
            </xsl:variable>
            <xsl:choose>
              <xsl:when test="main:f or $master_c_array_ref != '' or $master_c_table_ref != ''">
                <xsl:call-template name="FORMULA">
                  <xsl:with-param name="master_c_array_ref" select="$master_c_array_ref"/>
                  <xsl:with-param name="master_c_table_ref" select="$master_c_table_ref"/>
                </xsl:call-template>
              </xsl:when>
              <!-- The below cases don't include main:f -->
              <xsl:when test="@t = 's'">
                <xsl:call-template name="LabelSst"/>
              </xsl:when>
              <xsl:when test="not(@t) and main:v">
                <xsl:call-template name="Number"/>
              </xsl:when>
              <xsl:when test="(@t = 'e' or @t = 'b')">
                <xsl:call-template name="BoolErr"/>
              </xsl:when>
              <xsl:when test="@t = 'str'">
                <xsl:call-template name="FORMULA"/> <!-- Empty Formula with String value -->
              </xsl:when>
              <xsl:otherwise>
                <xsl:call-template name="Blank"/>
              </xsl:otherwise>
              <!-- 'RK' is not created ever. 'Number' is used instead. -->
              <!-- 'MulRK' is not created ever. Set of 'Number' records is used instead. -->
              <!-- 'MulBlank' is not created ever. Set of separated 'Blank' records is used instead. -->
            </xsl:choose>
          </CELL>
        </xsl:for-each>
        <DBCell num_pointers="{count(main:sheetData/main:row[position() >= $from_inc and position() &lt; $from_inc + $NUM_ROWS_IN_ROW_BLOCK and main:c])}"/>
      </CELL_GROUP>
      <xsl:call-template name="CELL_32_splitter">
        <xsl:with-param name="from_inc"><xsl:value-of select="$from_inc + $NUM_ROWS_IN_ROW_BLOCK"/></xsl:with-param>
        <xsl:with-param name="array_formulas" select="$array_formulas"/>
        <xsl:with-param name="table_formulas" select="$table_formulas"/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

  <xsl:template name="ref2col"> <!-- 0-based -->
    <xsl:param name="ref"/>
    <xsl:param name="col">-1</xsl:param>
    <xsl:variable name="alphabet">ABCDEFGHIJKLMNOPQRSTUVWXYZ</xsl:variable>
    <xsl:variable name="aaaaaaaa">AAAAAAAAAAAAAAAAAAAAAAAAAA</xsl:variable>
    <xsl:if test="$ref">
      <xsl:variable name="sym">
        <xsl:value-of select="substring($ref, 1, 1)" />
      </xsl:variable>
      <xsl:choose>
        <xsl:when test="contains($alphabet, $sym)">
          <xsl:variable name="sym_code">
            <xsl:value-of select="string-length(substring-before($alphabet, $sym))" />
          </xsl:variable>
          <xsl:call-template name="ref2col">
            <xsl:with-param name="ref" select="substring($ref, 2, string-length($ref) - 1)"/>
            <xsl:with-param name="col" select="($col + 1) * 26 + $sym_code"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$col"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:if>
  </xsl:template>

  <xsl:template name="ref2row"> <!-- 0-based -->
    <xsl:param name="ref"/>
    <xsl:variable name="numbers">0123456789</xsl:variable>
    <xsl:variable name="zzzeros">0000000000</xsl:variable>
    <xsl:if test="$ref">
      <xsl:variable name="pos">
        <xsl:value-of select="string-length(substring-before(translate($ref, $numbers, $zzzeros), '0'))"/>
      </xsl:variable>
      <xsl:value-of select="number(substring($ref, $pos + 1, string-length($ref) - $pos)) - 1"/>
    </xsl:if>
  </xsl:template>

  <xsl:template name="find_master_c_ref">
    <xsl:param name="array"/>
    <xsl:param name="row"/>
    <xsl:param name="col"/>
    <xsl:param name="ref"/> <!-- the same as (row,col) -->
    <xsl:for-each select="$array">  <!-- main:f selected-->
      <xsl:variable name="aref" select="@ref"/>
      <xsl:choose>
        <xsl:when test="contains($aref, ':')">
          <xsl:variable name="aref1" select="substring-before($aref, ':')"/>
          <xsl:variable name="aref2" select="substring-after($aref, ':')"/>
          <xsl:variable name="first_row">
            <xsl:call-template name="ref2row">
              <xsl:with-param name="ref" select="$aref1"/>
            </xsl:call-template>
          </xsl:variable>
          <xsl:variable name="last_row">
            <xsl:call-template name="ref2row">
              <xsl:with-param name="ref" select="$aref2"/>
            </xsl:call-template>
          </xsl:variable>
          <xsl:variable name="first_col">
            <xsl:call-template name="ref2col">
              <xsl:with-param name="ref" select="$aref1"/>
            </xsl:call-template>
          </xsl:variable>
          <xsl:variable name="last_col">
            <xsl:call-template name="ref2col">
              <xsl:with-param name="ref" select="$aref2"/>
            </xsl:call-template>
          </xsl:variable>
          <xsl:if test="$row >= $first_row and $row &lt;= $last_row and $col >= $first_col and $col &lt;= $last_col">
            <xsl:value-of select="../@r"/>
          </xsl:if>
        </xsl:when>
        <xsl:when test="$ref = $aref">
          <xsl:value-of select="../@r"/>
        </xsl:when>
      </xsl:choose>
    </xsl:for-each>

  </xsl:template>


  <xsl:template name="Cell">
    <Cell rw="{../@r - 1}">
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">col</xsl:with-param>
        <xsl:with-param name="val">
          <xsl:call-template name="ref2col">
            <xsl:with-param name="ref" select="@r"/>
          </xsl:call-template>
        </xsl:with-param>
        <xsl:with-param name="def_val">0</xsl:with-param>
      </xsl:call-template>
      <xsl:variable name="s">
        <xsl:choose>
          <xsl:when test="@s"><xsl:value-of select="@s"/></xsl:when>
          <xsl:otherwise>0</xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">ixfe</xsl:with-param>
        <xsl:with-param name="val" select="$s + $cellStyleXfs_count"/>
        <xsl:with-param name="def_val">0</xsl:with-param>
      </xsl:call-template>
    </Cell>
    <CellOffsetResender>
      <xsl:attribute name="row_starts">
        <xsl:choose>
          <xsl:when test="not(preceding-sibling::main:c)">true</xsl:when>
          <xsl:otherwise>false</xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
      <xsl:attribute name="last_in_block">
        <xsl:choose>
          <xsl:when test="not(preceding-sibling::main:c) and following-sibling::main:c[position() = last()]">true</xsl:when>
          <xsl:otherwise>false</xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
    </CellOffsetResender>
  </xsl:template>

  <xsl:template name="LabelSst">
    <LabelSst isst="{main:v}">
      <xsl:call-template name="Cell"/>
    </LabelSst>
  </xsl:template>

  <xsl:template name="Blank">
    <Blank>
      <xsl:call-template name="Cell"/>
    </Blank>
  </xsl:template>

  <xsl:template name="Number">
    <Number num="{main:v}">
      <xsl:call-template name="Cell"/>
    </Number>
  </xsl:template>

  <xsl:template name="BoolErr">
    <BoolErr>
      <xsl:call-template name="Cell"/>
      <Bes  value="{main:v}"/>
    </BoolErr>
  </xsl:template>

  <xsl:template name="FORMULA">
    <xsl:param name="master_c_array_ref"/>
    <xsl:param name="master_c_table_ref"/>
    <FORMULA>
      <xsl:call-template name="Formula">
        <xsl:with-param name="master_c_array_ref" select="$master_c_array_ref"/>
        <xsl:with-param name="master_c_table_ref" select="$master_c_table_ref"/>
      </xsl:call-template>
      <xsl:choose>
        <xsl:when test="main:f/@t = 'shared'">
          <xsl:call-template name="ShrFmla"/>
        </xsl:when>
        <xsl:when test="main:f/@t = 'array'">
          <xsl:call-template name="Array"/>
        </xsl:when>
        <xsl:when test="main:f/@t = 'dataTable'">
          <xsl:call-template name="Table"/>
        </xsl:when>
      </xsl:choose>
      <xsl:if test="@t = 'str'">
        <String string="{main:v}"/>
      </xsl:if>
    </FORMULA>
  </xsl:template>

  <xsl:template name="ShrFmla">
    <xsl:if test="main:f != '' and main:f/@ref">
      <xsl:variable name="si" select="main:f/@si"/>
      <ShrFmla cUse="{count(../../main:row/main:c/main:f[@si = $si])}">
        <RefU name="{main:f/@ref}"/>
        <SharedParsedFormula assembled_formula="{main:f}"/>
      </ShrFmla>
    </xsl:if>
  </xsl:template>

  <xsl:template name="Array">
    <Array>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fAlwaysCalc</xsl:with-param>
        <xsl:with-param name="val" select="main:f/@aca"/>
      </xsl:call-template>
      <Ref name="{main:f/@ref}"/>
      <ArrayParsedFormula assembled_formula="{main:f}"/>
    </Array>
  </xsl:template>

  <xsl:template name="Table">
    <Table>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fAlwaysCalc</xsl:with-param>
        <xsl:with-param name="val" select="main:f/@ca"/>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fRw</xsl:with-param>
        <xsl:with-param name="val" select="main:f/@dtr"/>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fTbl2</xsl:with-param>
        <xsl:with-param name="val" select="main:f/@dt2D"/>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fDeleted1</xsl:with-param>
        <xsl:with-param name="val" select="main:f/@del1"/>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fDeleted2</xsl:with-param>
        <xsl:with-param name="val" select="main:f/@del2"/>
      </xsl:call-template>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">r1</xsl:with-param>
        <xsl:with-param name="val" select="main:f/@r1"/>
      </xsl:call-template>
      <xsl:if test="main:f/@dt2D = '1' or main:f/@dt2D = 'true'">
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">r2</xsl:with-param>
          <xsl:with-param name="val" select="main:f/@r2"/>
        </xsl:call-template>
      </xsl:if>
      <Ref name="{main:f/@ref}"/>
    </Table>
  </xsl:template>

  <xsl:template name="Formula">
    <xsl:param name="master_c_array_ref"/>
    <xsl:param name="master_c_table_ref"/>
    <Formula val="{main:v}">
      <xsl:attribute name="val">
        <xsl:if test="not(@t) or @t != 'str'"><xsl:value-of select="main:v"/></xsl:if>
      </xsl:attribute>
      <xsl:attribute name="val_type">
        <xsl:choose>
          <xsl:when test="@t = 'e'">2</xsl:when>  <!-- Error -->
          <xsl:when test="@t = 'b'">1</xsl:when>  <!-- Boolean -->
          <xsl:when test="@t = 'str'">0</xsl:when>  <!-- The result is a String -->
          <xsl:when test="not(main:v != '')">3</xsl:when>  <!-- Blank -->
          <xsl:otherwise>4</xsl:otherwise>  <!-- Numeric -->
        </xsl:choose>
      </xsl:attribute>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fAlwaysCalc</xsl:with-param>
        <xsl:with-param name="val" select="main:f/@ca"/>
      </xsl:call-template>
      <xsl:attribute name="fFill">false</xsl:attribute> <!-- TODO: check -->
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fShrFmla</xsl:with-param>
        <xsl:with-param name="val" select="main:f/@t = 'shared'"/>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fClearErrors</xsl:with-param> <!-- TODO: obtain this value from main:worksheet/main:ignoredErrors/main:ignoredError -->
        <xsl:with-param name="val" select="'false'"/>
      </xsl:call-template>
      <xsl:call-template name="Cell"/>
      <xsl:choose>
        <xsl:when test="main:f/@t = 'shared'">
          <xsl:variable name="si" select="main:f/@si"/>
          <xsl:call-template name="PtgExp_gen">
            <xsl:with-param name="master_c" select="../../main:row/main:c[main:f/@si = $si and main:f != '']"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:when test="$master_c_array_ref != ''">
          <xsl:call-template name="PtgExp_gen">
            <xsl:with-param name="master_c" select="../../main:row/main:c[@r = $master_c_array_ref]"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:when test="$master_c_table_ref != ''">
          <xsl:call-template name="PtgExp_gen">
            <xsl:with-param name="master_c" select="../../main:row/main:c[@r = $master_c_table_ref]"/>
            <xsl:with-param name="tag_name">PtgTbl</xsl:with-param>
          </xsl:call-template>
        </xsl:when>
        <xsl:when test="not(main:f != '') and @t = 'str'">
          <CellParsedFormula assembled_formula="&quot;{main:v}&quot;"/>
        </xsl:when>
        <xsl:otherwise>
          <CellParsedFormula assembled_formula="{main:f}"/>
        </xsl:otherwise>
      </xsl:choose>
    </Formula>
  </xsl:template>

  <xsl:template name="PtgExp_gen">
    <xsl:param name="master_c"/>
    <xsl:param name="tag_name">PtgExp</xsl:param>
    <CellParsedFormula assembled_formula="">
      <Rgce>
        <xsl:element name="{$tag_name}">
          <xsl:attribute name="ref">
            <xsl:value-of select="$master_c/@r"/>
          </xsl:attribute>
        </xsl:element>
      </Rgce>
    </CellParsedFormula>
  </xsl:template>

  <xsl:template name="DxGCol">
    <xsl:if test="main:sheetFormatPr/@defaultColWidth">
      <DxGCol dxgCol="{main:sheetFormatPr/@defaultColWidth * 256}"/>
    </xsl:if>
  </xsl:template>

  <xsl:template name="MergeCells">
    <xsl:if test="main:mergeCells/main:mergeCell">
      <MergeCells cmcs="{count(main:mergeCells/main:mergeCell)}">
        <xsl:for-each select="main:mergeCells/main:mergeCell">
          <Ref8 name="{@ref}"/>
        </xsl:for-each>
      </MergeCells>
    </xsl:if>
  </xsl:template>

  <xsl:template name="Sync">
    <xsl:if test="main:sheetPr/@syncRef">
      <Sync cmcs="{count(main:mergeCells/main:mergeCell)}">
        <xsl:for-each select="main:mergeCells/main:mergeCell">
          <Ref8 name="{@ref}"/>
        </xsl:for-each>
      </Sync>
    </xsl:if>
  </xsl:template>

  <xsl:template name="HorizontalPageBreaks">
    <xsl:if test="main:rowBreaks/main:brk">
      <HorizontalPageBreaks cbrk="{count(main:rowBreaks/main:brk)}">
        <xsl:for-each select="main:rowBreaks/main:brk">
          <xsl:sort select="@id" data-type="number"/>
          <xsl:sort select="@min" data-type="number"/>
          <HorzBrk>
            <xsl:call-template name="attrib_req_with_def">
              <xsl:with-param name="name">row</xsl:with-param>
              <xsl:with-param name="val" select="@id"/>
              <xsl:with-param name="def_val">0</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_req_with_def">
              <xsl:with-param name="name">colStart</xsl:with-param>
              <xsl:with-param name="val">
                <xsl:choose>
                  <xsl:when test="@min > 255">255</xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="@min"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>
              <xsl:with-param name="def_val">0</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_req_with_def">
              <xsl:with-param name="name">colEnd</xsl:with-param>
              <xsl:with-param name="val">
                <xsl:choose>
                  <xsl:when test="@max > 255">255</xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="@max"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>
              <xsl:with-param name="def_val">0</xsl:with-param>
            </xsl:call-template>
          </HorzBrk>
        </xsl:for-each>
      </HorizontalPageBreaks>
    </xsl:if>
  </xsl:template>
  
  <xsl:template name="VerticalPageBreaks">
    <xsl:if test="main:colBreaks/main:brk">
      <VerticalPageBreaks cbrk="{count(main:colBreaks/main:brk)}">
        <xsl:for-each select="main:colBreaks/main:brk">
          <xsl:sort select="@id" data-type="number"/>
          <xsl:sort select="@min" data-type="number"/>
          <VertBrk>
            <xsl:call-template name="attrib_req_with_def">
              <xsl:with-param name="name">col</xsl:with-param>
              <xsl:with-param name="val" select="@id"/>
              <xsl:with-param name="def_val">0</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_req_with_def">
              <xsl:with-param name="name">rowStart</xsl:with-param>
              <xsl:with-param name="val">
                <xsl:choose>
                  <xsl:when test="@min > 65535">65535</xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="@min"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>
              <xsl:with-param name="def_val">0</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_req_with_def">
              <xsl:with-param name="name">rowEnd</xsl:with-param>
              <xsl:with-param name="val">
                <xsl:choose>
                  <xsl:when test="@max > 65535">65535</xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="@max"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>
              <xsl:with-param name="def_val">0</xsl:with-param>
            </xsl:call-template>
          </VertBrk>
        </xsl:for-each>
      </VerticalPageBreaks>
    </xsl:if>
  </xsl:template>
  
  <xsl:template name="Pls">
    <xsl:param name="ws_position"/>
    <xsl:if test="main:pageSetup/@mainr:id">
      <Pls>
        <xsl:variable name="rId" select="main:pageSetup/@mainr:id"/>
        <xsl:variable name="ws_path" select="msxsl:node-set($sheets_paths)/sheet_path[position() = $ws_position]"/>
        <xsl:variable name="ws_path_no_filename">
          <xsl:call-template name="filepath2dirpath">
            <xsl:with-param name="filepath" select="$ws_path"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:variable name="printer_file_name" select="ancestor-or-self::main:worksheet/following-sibling::*[position() = 1]/self::r:Relationships/r:Relationship[@Id = $rId]/@Target"/>
        <xsl:variable name="bin_data_id_container">
          <xsl:call-template name="getReferenceByPath">
            <xsl:with-param name="path" select="concat('/', $ws_path_no_filename, $printer_file_name)"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:attribute name="bin_data_id">
          <xsl:value-of select="msxsl:node-set($bin_data_id_container)/bin_data_id/@id"/>
        </xsl:attribute>
        <!--
        <xsl:message>id = "<xsl:value-of select="msxsl:node-set($bin_data_id_container)/bin_data_id/@id"/>"</xsl:message>
        -->
      </Pls>
    </xsl:if>
  </xsl:template>
  
  <xsl:template name="PLV">
    <PLV>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">wScalePLV</xsl:with-param>
        <xsl:with-param name="val" select="@zoomScalePageLayoutView"/>
        <xsl:with-param name="def_val">0</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fPageLayoutView</xsl:with-param>
        <xsl:with-param name="val" select="boolean(@view = 'pageLayout')"/>
        <xsl:with-param name="def_val">false</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fRulerVisible</xsl:with-param>
        <xsl:with-param name="val" select="@showRuler"/>
        <xsl:with-param name="def_val">true</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req_inv">
        <xsl:with-param name="name">fWhitespaceHidden</xsl:with-param>
        <xsl:with-param name="val" select="@showWhiteSpace"/>
        <xsl:with-param name="def_val">true</xsl:with-param>
      </xsl:call-template>
    </PLV>
  </xsl:template>

  <xsl:template name="Scl">
    <xsl:if test="@zoomScale != 100">
      <Scl nscl="{@zoomScale}" dscl="100"/>
    </xsl:if>
  </xsl:template>

  <xsl:template name="Pane">
    <xsl:for-each select="main:pane[position() = 1]">
      <Pane>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">x</xsl:with-param>
          <xsl:with-param name="val" select="@xSplit"/>
          <xsl:with-param name="def_val">0</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">y</xsl:with-param>
          <xsl:with-param name="val" select="@ySplit"/>
          <xsl:with-param name="def_val">0</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">topLeftCell</xsl:with-param>
          <xsl:with-param name="val" select="@topLeftCell"/>
          <xsl:with-param name="def_val">A1</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">pnnAcct</xsl:with-param>
          <xsl:with-param name="val" select="@activePane"/>
          <xsl:with-param name="def_val">topLeft</xsl:with-param>
        </xsl:call-template>
      </Pane>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="HLINK">
    <xsl:param name="ws_position"/>
    <xsl:for-each select="main:hyperlinks/main:hyperlink">
      <HLINK>
        <HLink> <!-- hlinkClsid is determined in C++ processor -->
          <Ref8U name="{@ref}"/>
          <HyperlinkObject>
            <xsl:variable name="rId" select="@mainr:id"/>
            <xsl:variable name="moniker" select="../../following-sibling::*[position() = 1]/self::r:Relationships/r:Relationship[@Id = $rId]/@Target"/>

            <xsl:call-template name="attrib_bool_req">
              <xsl:with-param name="name">hlstmfHasMoniker</xsl:with-param>
              <xsl:with-param name="val" select="boolean($moniker != '')"/>
            </xsl:call-template>
            <xsl:attribute name="hlstmfIsAbsolute">
              <xsl:choose>
                <xsl:when test="$moniker != '' and string-length(substring-before($moniker, ':')) != 0 and
                                (contains(substring-after($moniker, ':'), '/') or contains(substring-after($moniker, ':'), '\')) or
                                starts-with($moniker, 'mailto:')">true</xsl:when>
                <xsl:otherwise>false</xsl:otherwise>
              </xsl:choose>
            </xsl:attribute>
            <xsl:call-template name="attrib_bool_req">
              <xsl:with-param name="name">hlstmfSiteGaveDisplayName</xsl:with-param>
              <xsl:with-param name="val" select="boolean(@display != '')"/>
            </xsl:call-template>
            <xsl:call-template name="attrib_bool_req">
              <xsl:with-param name="name">hlstmfHasLocationStr</xsl:with-param>
              <xsl:with-param name="val" select="boolean(@location != '')"/>
            </xsl:call-template>
            <xsl:call-template name="attrib_bool_req">
              <xsl:with-param name="name">hlstmfHasDisplayName</xsl:with-param>
              <xsl:with-param name="val" select="boolean(@display != '')"/>
            </xsl:call-template>
            <xsl:attribute name="hlstmfHasGUID">false</xsl:attribute>
            <xsl:attribute name="hlstmfHasCreationTime">false</xsl:attribute>
            <xsl:attribute name="hlstmfHasFrameName">false</xsl:attribute>
            <xsl:variable name="hlstmfMonikerSavedAsStr">
              <xsl:choose>
                <xsl:when test="starts-with($moniker, 'file:///\\')">true</xsl:when>
                <xsl:otherwise>false</xsl:otherwise>
              </xsl:choose>
            </xsl:variable>
            <xsl:attribute name="hlstmfMonikerSavedAsStr"><xsl:value-of select="$hlstmfMonikerSavedAsStr"/></xsl:attribute>
            <xsl:attribute name="hlstmfAbsFromGetdataRel">false</xsl:attribute>

            <xsl:if test="@display != ''">
              <xsl:attribute name="displayName"><xsl:value-of select="@display"/></xsl:attribute>
            </xsl:if>
            <!-- targetFrameName is never saved -->
            <xsl:if test="$hlstmfMonikerSavedAsStr = 'true'">
              <xsl:attribute name="moniker"><xsl:value-of select="$moniker"/></xsl:attribute>
            </xsl:if>
            <xsl:if test="@location != ''">
              <xsl:attribute name="location"><xsl:value-of select="@location"/></xsl:attribute>
            </xsl:if>
            <xsl:if test="$moniker != '' and $hlstmfMonikerSavedAsStr = 'false'">
              <!-- oleMoniker is never saved -->                                    <!-- TODO -->
              <!--
              <xsl:choose>
                <xsl:when test="ds">
                  <URLMoniker url="{$moniker}"/>
                </xsl:when>
                <xsl:otherwise>
                  <FileMoniker>
                    
                  </FileMoniker>
                </xsl:otherwise>
              </xsl:choose>
              -->
              <URLMoniker url="{$moniker}"/>
            </xsl:if>
            <!-- guid is never saved -->
            <!-- fileTime is never saved -->
          </HyperlinkObject>          
        </HLink>
        <xsl:if test="@tooltip">
          <HLinkTooltip wzTooltip="{@tooltip}" ref="{@ref}"/>
        </xsl:if>        
      </HLINK>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="CUSTOMVIEW">
    <xsl:param name="ws_position"/>
    <xsl:for-each select="main:customSheetViews/main:customSheetView">
      <CUSTOMVIEW>
        <xsl:call-template name="UserSViewBegin">
          <xsl:with-param name="ws_position" select="$ws_position"/>
        </xsl:call-template>
        <xsl:call-template name="Selection"/>
        <xsl:call-template name="HorizontalPageBreaks"/>
        <xsl:call-template name="VerticalPageBreaks"/>
        <xsl:if test="main:headerFooter">
          <Header ast="{main:headerFooter/main:oddHeader}"/>
          <Footer ast="{main:headerFooter/main:oddFooter}"/>
        </xsl:if>
        <xsl:if test="main:printOptions">
          <HCenter hcenter="{boolean(main:printOptions/@horizontalCentered = 'true' or main:printOptions/@horizontalCentered = '1')}"/>
          <VCenter vcenter="{boolean(main:printOptions/@verticalCentered = 'true' or main:printOptions/@verticalCentered = '1')}"/>
        </xsl:if>
        <xsl:if test="main:pageMargins">
          <LeftMargin num="{main:pageMargins/@left}"/>
          <RightMargin num="{main:pageMargins/@right}"/>
          <TopMargin num="{main:pageMargins/@top}"/>
          <BottomMargin num="{main:pageMargins/@bottom}"/>
        </xsl:if>
        <xsl:call-template name="Pls">
          <xsl:with-param name="ws_position" select="$ws_position"/>
        </xsl:call-template>
        <xsl:if test="main:pageSetup">
          <xsl:call-template name="Setup"/>
        </xsl:if>
        <xsl:call-template name="HeaderFooter"/>
        <!-- TODO: implement <PrintSize> for charts -->
        <UserSViewEnd/>
      </CUSTOMVIEW>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="UserSViewBegin">
    <xsl:param name="ws_position"/>
    <UserSViewBegin guid="{@guid}">
      <xsl:attribute name="iTabid">
        <xsl:value-of select="$workbook_file/main:workbook/main:sheets/main:sheet[position() = $ws_position]/@sheetId"/>
      </xsl:attribute>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">wScale</xsl:with-param>
        <xsl:with-param name="val" select="@scale"/>
        <xsl:with-param name="def_val">100</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">icvHdr</xsl:with-param>
        <xsl:with-param name="val" select="@colorId"/>
        <xsl:with-param name="def_val">64</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">pnnSel</xsl:with-param>
        <xsl:with-param name="val" select="main:pane/@activePane"/>
        <xsl:with-param name="def_val">topLeft</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fShowBrks</xsl:with-param>
        <xsl:with-param name="val" select="@showPageBreaks"/>
        <xsl:with-param name="def_val">false</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fDspFmlaSv</xsl:with-param>
        <xsl:with-param name="val" select="@showFormulas"/>
        <xsl:with-param name="def_val">false</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fDspGridSv</xsl:with-param>
        <xsl:with-param name="val" select="@showGridLines"/>
        <xsl:with-param name="def_val">true</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fDspRwColSv</xsl:with-param>
        <xsl:with-param name="val" select="@showRowCol"/>
        <xsl:with-param name="def_val">true</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fDspGutsSv</xsl:with-param>
        <xsl:with-param name="val" select="@outlineSymbols"/>
        <xsl:with-param name="def_val">true</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fDspZerosSv</xsl:with-param>
        <xsl:with-param name="val" select="@zeroValues"/>
        <xsl:with-param name="def_val">true</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fHorizontal</xsl:with-param>
        <xsl:with-param name="val" select="main:printOptions/@horizontalCentered"/>
        <xsl:with-param name="def_val">false</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fVertical</xsl:with-param>
        <xsl:with-param name="val" select="main:printOptions/@verticalCentered"/>
        <xsl:with-param name="def_val">false</xsl:with-param>
      </xsl:call-template>
      <xsl:variable name="guid" select="@guid"/>
      <xsl:variable name="customWorkbookView" select="$workbook_file/main:workbook/main:customWorkbookViews/main:customWorkbookView[@guid = $guid]"/>
      <xsl:variable name="includePrintSettings" select="$customWorkbookView/@includePrintSettings"/>
      <xsl:choose>
        <xsl:when test="$includePrintSettings = 'false' or $includePrintSettings = '0' or
                        boolean(../../self::main:dialogsheet)"> <!-- default value of 'includePrintSettings' is true and may be ommitted -->
          <xsl:attribute name="fPrintRwCol">false</xsl:attribute>
          <xsl:attribute name="fPrintGrid">false</xsl:attribute>
          <xsl:attribute name="fFitToPage">false</xsl:attribute>
          <xsl:attribute name="fPrintArea">false</xsl:attribute>
          <xsl:attribute name="fOnePrintArea">false</xsl:attribute>
        </xsl:when>
        <xsl:otherwise>
          <xsl:call-template name="attrib_bool_req">
            <xsl:with-param name="name">fPrintRwCol</xsl:with-param>
            <xsl:with-param name="val" select="main:printOptions/@headings"/>
            <xsl:with-param name="def_val">false</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_bool_req">
            <xsl:with-param name="name">fPrintGrid</xsl:with-param>
            <xsl:with-param name="val" select="main:printOptions/@gridLines"/>
            <xsl:with-param name="def_val">false</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_bool_req">
            <xsl:with-param name="name">fFitToPage</xsl:with-param>
            <xsl:with-param name="val" select="@fitToPage"/>
            <xsl:with-param name="def_val">false</xsl:with-param>
          </xsl:call-template>
          <xsl:choose>
            <xsl:when test="@printArea = 'true' or @printArea = '1'">
              <xsl:attribute name="fPrintArea">true</xsl:attribute>
              <xsl:attribute name="fOnePrintArea">
                <xsl:value-of select="boolean(count(../main:customSheetView[@printArea = 'true' or @printArea = '1']) = 1)"/>
              </xsl:attribute>
            </xsl:when>
            <xsl:otherwise>
              <xsl:attribute name="fPrintArea">false</xsl:attribute>
              <xsl:attribute name="fOnePrintArea">false</xsl:attribute>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:otherwise>
      </xsl:choose>
      <xsl:variable name="includeHiddenRowCol" select="$customWorkbookView/@includeHiddenRowCol"/>
      <xsl:choose>
        <xsl:when test="$includeHiddenRowCol = 'false' or $includeHiddenRowCol = '0' or
                        boolean(../../self::main:dialogsheet)">  <!-- default value of 'includeHiddenRowCol' is true and may be ommitted -->
          <xsl:attribute name="fFilterMode">false</xsl:attribute>
          <xsl:attribute name="fEzFilter">false</xsl:attribute>
          <xsl:attribute name="fHiddenRw">false</xsl:attribute>
          <xsl:attribute name="fHiddenCol">false</xsl:attribute>
          <xsl:attribute name="fFilterUnique">false</xsl:attribute>
        </xsl:when>
        <xsl:otherwise>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fFilterMode</xsl:with-param>
        <xsl:with-param name="val" select="@filter"/>
        <xsl:with-param name="def_val">false</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fEzFilter</xsl:with-param>
        <xsl:with-param name="val" select="@showAutoFilter"/>
        <xsl:with-param name="def_val">false</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fHiddenRw</xsl:with-param>
        <xsl:with-param name="val" select="@hiddenRows"/>
        <xsl:with-param name="def_val">false</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fHiddenCol</xsl:with-param>
        <xsl:with-param name="val" select="@hiddenColumns"/>
        <xsl:with-param name="def_val">false</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fFilterUnique</xsl:with-param>
        <xsl:with-param name="val" select="@filterUnique"/>
        <xsl:with-param name="def_val">false</xsl:with-param>
      </xsl:call-template>
        </xsl:otherwise>
      </xsl:choose>

      <xsl:choose>
        <xsl:when test="main:pane/@state = 'frozen'">
          <xsl:attribute name="fFrozen">true</xsl:attribute>
          <xsl:attribute name="fFrozenNoSplit">true</xsl:attribute>
        </xsl:when>
        <xsl:when test="main:pane/@state = 'frozenSplit'">
          <xsl:attribute name="fFrozen">true</xsl:attribute>
          <xsl:attribute name="fFrozenNoSplit">false</xsl:attribute>
        </xsl:when>
        <xsl:otherwise>  <!-- The default is 'split' or absent -->
          <xsl:attribute name="fFrozen">false</xsl:attribute>
          <xsl:attribute name="fFrozenNoSplit">false</xsl:attribute>
        </xsl:otherwise>
      </xsl:choose>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fSplitV</xsl:with-param>
        <xsl:with-param name="val" select="boolean(main:pane/@ySplit != 0)"/>
        <xsl:with-param name="def_val">false</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fSplitH</xsl:with-param>
        <xsl:with-param name="val" select="boolean(main:pane/@xSplit != 0)"/>
        <xsl:with-param name="def_val">false</xsl:with-param>
      </xsl:call-template>
      <xsl:choose>
        <xsl:when test="@view = 'pageBreakPreview'">
          <xsl:attribute name="fSheetLayoutView">true</xsl:attribute>
          <xsl:attribute name="fPageLayoutView">false</xsl:attribute>
        </xsl:when>
        <xsl:when test="@view = 'pageLayout'">
          <xsl:attribute name="fSheetLayoutView">false</xsl:attribute>
          <xsl:attribute name="fPageLayoutView">true</xsl:attribute>
        </xsl:when>
        <xsl:otherwise>  <!-- The default is 'normal' or absent -->
          <xsl:attribute name="fSheetLayoutView">false</xsl:attribute>
          <xsl:attribute name="fPageLayoutView">false</xsl:attribute>
        </xsl:otherwise>
      </xsl:choose>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fRuler</xsl:with-param>
        <xsl:with-param name="val" select="@showRuler"/>
        <xsl:with-param name="def_val">true</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">ref8TopLeft</xsl:with-param>
        <xsl:with-param name="val" select="@topLeftCell"/>
        <xsl:with-param name="def_val">A1</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">operNumX</xsl:with-param>
        <xsl:with-param name="val" select="main:pane/@xSplit"/>
        <xsl:with-param name="def_val">0</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">operNumY</xsl:with-param>
        <xsl:with-param name="val" select="main:pane/@ySplit"/>
        <xsl:with-param name="def_val">0</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">pane_top_left_cell</xsl:with-param>
        <xsl:with-param name="val" select="main:pane/@topLeftCell"/>
        <xsl:with-param name="def_val">A1</xsl:with-param>
      </xsl:call-template>
    </UserSViewBegin>
  </xsl:template>

  <xsl:template name="HeaderFooter">
    <xsl:if test="main:headerFooter/@differentOddEven or main:headerFooter/@differentFirst or main:headerFooter/@scaleWithDoc or
                  main:headerFooter/@alignWithMargins or main:headerFooter/main:evenHeader or main:headerFooter/main:evenFooter or 
                  main:headerFooter/main:firstHeader or main:headerFooter/main:firstFooter">
      <HeaderFooter>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">guidSView</xsl:with-param>
          <xsl:with-param name="val" select="@guid"/>
          <xsl:with-param name="def_val">{00000000-0000-0000-0000-000000000000}</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_bool_req">
          <xsl:with-param name="name">fHFDiffOddEven</xsl:with-param>
          <xsl:with-param name="val" select="main:headerFooter/@differentOddEven"/>
        </xsl:call-template>
        <xsl:call-template name="attrib_bool_req">
          <xsl:with-param name="name">fHFDiffFirst</xsl:with-param>
          <xsl:with-param name="val" select="main:headerFooter/@differentFirst"/>
        </xsl:call-template>
        <xsl:call-template name="attrib_bool_req">
          <xsl:with-param name="name">fHFScaleWithDoc</xsl:with-param>
          <xsl:with-param name="val" select="main:headerFooter/@scaleWithDoc"/>
          <xsl:with-param name="def_val">true</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_bool_req">
          <xsl:with-param name="name">fHFAlignMargins</xsl:with-param>
          <xsl:with-param name="val" select="main:headerFooter/@alignWithMargins"/>
          <xsl:with-param name="def_val">true</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">strHeaderEven</xsl:with-param>
          <xsl:with-param name="val" select="main:headerFooter/main:evenHeader"/>
        </xsl:call-template>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">strFooterEven</xsl:with-param>
          <xsl:with-param name="val" select="main:headerFooter/main:evenFooter"/>
        </xsl:call-template>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">strHeaderFirst</xsl:with-param>
          <xsl:with-param name="val" select="main:headerFooter/main:firstHeader"/>
        </xsl:call-template>
        <xsl:call-template name="attrib_req_with_def">
          <xsl:with-param name="name">strFooterFirst</xsl:with-param>
          <xsl:with-param name="val" select="main:headerFooter/main:firstFooter"/>
        </xsl:call-template>
      </HeaderFooter>
    </xsl:if>
  </xsl:template>


  <xsl:variable name="CF_rule_types_formula_raw">
    <type name="expression"/>
    <type name="containsErrors"/>
    <type name="notContainsErrors"/>
    <type name="containsBlanks"/>
    <type name="notContainsBlanks"/>
    <type name="timePeriod"/>
    <type name="containsText"/>
    <type name="notContainsText"/>
    <type name="beginsWith"/>
    <type name="endsWith"/>
  </xsl:variable>
  <xsl:variable name="CF_rule_types_formula" select="msxsl:node-set($CF_rule_types_formula_raw)"/>
  
  <xsl:template name="CONDFMTS">
    <xsl:if test="main:conditionalFormatting">
      <CONDFMTS>
        <xsl:for-each select="main:conditionalFormatting">
          <xsl:variable name="is_12">
            <xsl:for-each select="main:cfRule">
              <xsl:variable name="type" select="@type"/>
              <xsl:if test="$type != 'cellIs' and not($CF_rule_types_formula/type[@name = $type])">true</xsl:if>
            </xsl:for-each>
          </xsl:variable>
          <xsl:choose>
            <xsl:when test="$is_12 != ''"><!--@type != 'cellIs' and not($CF_rule_types_formula/type[@name = $type])-->
              <!--<CONDFMT12>
                <CondFmt12>
                  
                </CondFmt12>
              </CONDFMT12>-->
            </xsl:when>
            <xsl:otherwise> <!-- Office 97 compatible -->
              <CONDFMT>
                <CondFmt ccf="{count(main:cfRule[position() &lt;= 3])}" fToughRecalc="false" nID="{position()}" refBound="{@sqref}">
                  <SqRefU sqref="{@sqref}"/>
                </CondFmt>
                <xsl:for-each select="main:cfRule[position() &lt;= 3]">
                <xsl:variable name="type" select="@type"/>
                  <CF>
                    <xsl:attribute name="ct">
                      <xsl:choose>
                        <xsl:when test="@type = 'cellIs'">1</xsl:when>
                        <xsl:when test="$CF_rule_types_formula/type[@name = $type]">2</xsl:when>
                      </xsl:choose>
                    </xsl:attribute>
                    <xsl:attribute name="cp">
                      <xsl:choose>
                        <xsl:when test="@type != 'cellIs'">0</xsl:when>
                        <xsl:when test="@operator = 'between'">1</xsl:when>
                        <xsl:when test="@operator = 'notBetween'">2</xsl:when>
                        <xsl:when test="@operator = 'equal'">3</xsl:when>
                        <xsl:when test="@operator = 'notEqual'">4</xsl:when>
                        <xsl:when test="@operator = 'greaterThan'">5</xsl:when>
                        <xsl:when test="@operator = 'lessThan'">6</xsl:when>
                        <xsl:when test="@operator = 'greaterThanOrEqual'">7</xsl:when>
                        <xsl:when test="@operator = 'lessThanOrEqual'">8</xsl:when>
                      </xsl:choose>
                    </xsl:attribute>
                    <xsl:call-template name="DXFN">
                      <xsl:with-param name="dxfId" select="@dxfId"/>
                    </xsl:call-template>
                    <CFParsedFormulaNoCCE assembled_formula="{main:formula[1]}"/>
                    <CFParsedFormulaNoCCE assembled_formula="{main:formula[2]}"/>
                  </CF>
                </xsl:for-each>
              </CONDFMT>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:for-each>
      </CONDFMTS>
    </xsl:if>
  </xsl:template>

  <xsl:template name="DXFN">
    <xsl:param name="dxfId"/>
    <DXFN>
      <xsl:variable name="dxf" select="$stylesheet_file/main:styleSheet/main:dxfs/main:dxf[position() = $dxfId + 1]"/>
      <xsl:variable name="is_not_alignment" select="not($dxf/main:alignment)"/>
      <xsl:attribute name="alchNinch"><xsl:value-of select="$is_not_alignment"/></xsl:attribute>
      <xsl:attribute name="alcvNinch"><xsl:value-of select="$is_not_alignment"/></xsl:attribute>
      <xsl:attribute name="wrapNinch"><xsl:value-of select="$is_not_alignment"/></xsl:attribute>
      <xsl:attribute name="trotNinch"><xsl:value-of select="$is_not_alignment"/></xsl:attribute>
      <xsl:attribute name="kintoNinch"><xsl:value-of select="$is_not_alignment"/></xsl:attribute>
      <xsl:attribute name="cIndentNinch"><xsl:value-of select="$is_not_alignment"/></xsl:attribute>
      <xsl:attribute name="fShrinkNinch"><xsl:value-of select="$is_not_alignment"/></xsl:attribute>
      <xsl:attribute name="fMergeCellNinch">true</xsl:attribute>
      <xsl:attribute name="lockedNinch"><xsl:value-of select="not($dxf/main:protection)"/></xsl:attribute>
      <xsl:attribute name="hiddenNinch"><xsl:value-of select="not($dxf/main:protection)"/></xsl:attribute>
      <xsl:attribute name="glLeftNinch"><xsl:value-of select="not($dxf/main:border/main:left)"/></xsl:attribute> <!-- TESTED -->
      <xsl:attribute name="glRightNinch"><xsl:value-of select="not($dxf/main:border/main:right)"/></xsl:attribute> <!-- TESTED -->
      <xsl:attribute name="glTopNinch"><xsl:value-of select="not($dxf/main:border/main:top)"/></xsl:attribute> <!-- TESTED -->
      <xsl:attribute name="glBottomNinch"><xsl:value-of select="not($dxf/main:border/main:bottom)"/></xsl:attribute> <!-- TESTED -->
      <xsl:attribute name="glDiagDownNinch"><xsl:value-of select="not($dxf/main:border/main:diagonal)"/></xsl:attribute> <!-- TESTED -->
      <xsl:attribute name="glDiagUpNinch"><xsl:value-of select="not($dxf/main:border/main:diagonal)"/></xsl:attribute> <!-- TESTED -->
      <xsl:attribute name="flsNinch"> <!-- TESTED -->
        <xsl:value-of select="boolean(not($dxf/main:fill/main:gradientFill) and not($dxf/main:fill/main:patternFill/@patternType))"/>
      </xsl:attribute>
      <xsl:attribute name="icvFNinch"><xsl:value-of select="boolean(not($dxf/main:fill/main:gradientFill) and 
                                                            not($dxf/main:fill/main:patternFill/main:fgColor))"/></xsl:attribute> <!-- TESTED -->
      <xsl:attribute name="icvBNinch"><xsl:value-of select="boolean(not($dxf/main:fill/main:gradientFill) and 
                                                            not($dxf/main:fill/main:patternFill/main:bgColor))"/></xsl:attribute> <!-- TESTED -->
      <xsl:attribute name="ifmtNinch"><xsl:value-of select="not($dxf/main:numFmt)"/></xsl:attribute>
      <xsl:attribute name="fIfntNinch">true</xsl:attribute> <!-- TESTED -->
      <xsl:attribute name="ibitAtrNum"><xsl:value-of select="boolean($dxf/main:numFmt)"/></xsl:attribute> <!-- TESTED -->
      <xsl:attribute name="ibitAtrFnt"><xsl:value-of select="boolean($dxf/main:font)"/></xsl:attribute> <!-- TESTED -->
      <xsl:attribute name="ibitAtrAlc"><xsl:value-of select="boolean($dxf/main:alignment)"/></xsl:attribute>
      <xsl:attribute name="ibitAtrBdr"><xsl:value-of select="boolean($dxf/main:border)"/></xsl:attribute> <!-- TESTED -->
      <xsl:attribute name="ibitAtrPat"><xsl:value-of select="boolean($dxf/main:fill)"/></xsl:attribute> <!-- TESTED -->
      <xsl:attribute name="ibitAtrProt"><xsl:value-of select="boolean($dxf/main:protection)"/></xsl:attribute>
      <xsl:attribute name="iReadingOrderNinch"><xsl:value-of select="not($dxf/main:alignment/main:readingOrder)"/></xsl:attribute>
      <xsl:choose>
        <xsl:when test="$dxf/main:numFmt">
          <xsl:for-each select="$dxf/main:numFmt[1]">
            <xsl:attribute name="fIfmtUser"><xsl:value-of select="boolean(key('numFmt_is_user_defined', $dxf/main:numFmt[1]/@numFmtId))"/></xsl:attribute> <!-- TESTED -->
          </xsl:for-each>
        </xsl:when>
        <xsl:otherwise>
            <xsl:attribute name="fIfmtUser">false</xsl:attribute> <!-- TESTED -->
        </xsl:otherwise>
      </xsl:choose>
      <xsl:attribute name="fNewBorder"><xsl:value-of select="'false'"/></xsl:attribute>
      <xsl:attribute name="fZeroInited"><xsl:value-of select="'true'"/></xsl:attribute>
      <xsl:for-each select="$dxf/main:numFmt[1]">
        <xsl:call-template name="DXFNum"/>
      </xsl:for-each>
      <xsl:for-each select="$dxf/main:font[1]">
        <xsl:call-template name="DXFFntD"/>
      </xsl:for-each>
      <xsl:for-each select="$dxf/main:alignment[1]">
        <xsl:call-template name="DXFALC"/>
      </xsl:for-each>
      <xsl:for-each select="$dxf/main:border[1]">
        <xsl:call-template name="DXFBdr"/>
      </xsl:for-each>
      <xsl:for-each select="$dxf/main:fill[1]">
        <xsl:call-template name="DXFPat"/>
      </xsl:for-each>
      <xsl:for-each select="$dxf/main:protection[1]">
        <xsl:call-template name="DXFProt"/>
      </xsl:for-each>
      
     
    </DXFN>
  </xsl:template>

  <xsl:key name="numFmt_is_user_defined" match="main:dxf/main:numFmt[@numFmtId > 4 and @numFmtId &lt; 9 or 
           @numFmtId > 22 and @numFmtId &lt; 37 or @numFmtId > 40 and @numFmtId &lt; 45 or @numFmtId > 49]" use="@numFmtId"/>
  
  <xsl:template name="DXFNum">
    <DXFNum ifmt="{@numFmtId}" fmt="{@formatCode}">
<!--
      <xsl:message>key: <xsl:copy-of select="key('numFmt_is_user_defined', @numFmtId)"/></xsl:message>
      <xsl:choose>
        <xsl:when test="key('numFmt_is_user_defined', @numFmtId)">
          <Format stFormat="{@formatCode}"/>
        </xsl:when>
        <xsl:otherwise>
          <Format ifmt="{@numFmtId}"/>
        </xsl:otherwise>
      </xsl:choose>
      -->
    </DXFNum>
  </xsl:template>
  
  <xsl:template name="DXFFntD">
    <DXFFntD>
      <xsl:attribute name="stFontName"><xsl:value-of select="main:name/@val"/></xsl:attribute>
      <xsl:choose>
        <xsl:when test="main:color">
          
          <xsl:call-template name="attrib_req_with_def">
            <xsl:with-param name="name">icvFore</xsl:with-param>
            <xsl:with-param name="val">
              <xsl:call-template name="color2Icv">
                <xsl:with-param name="color" select="main:color"/>
              </xsl:call-template>
            </xsl:with-param>
            <xsl:with-param name="def_val">32767</xsl:with-param>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise><xsl:attribute name="icvFore">-1</xsl:attribute></xsl:otherwise>
      </xsl:choose>
      <xsl:attribute name="ftsItalic"><xsl:value-of select="not(boolean(main:i))"/></xsl:attribute>
      <xsl:attribute name="ftsStrikeout"><xsl:value-of select="not(boolean(main:strike))"/></xsl:attribute>
      <xsl:attribute name="fSssNinch"><xsl:value-of select="not(boolean(main:vertAlign))"/></xsl:attribute>
      <xsl:attribute name="fUlsNinch"><xsl:value-of select="not(boolean(main:u))"/></xsl:attribute>
      <xsl:attribute name="fBlsNinch"><xsl:value-of select="not(boolean(main:b))"/></xsl:attribute>
      <xsl:attribute name="ich">0</xsl:attribute>
      <xsl:attribute name="cch">0</xsl:attribute>
      <xsl:attribute name="iFnt">1</xsl:attribute>
      <xsl:attribute name="stFontName"><xsl:value-of select="main:name/@val"/></xsl:attribute>
      <Stxp>
        <xsl:attribute name="twpHeight">
          <xsl:choose>
            <xsl:when test="main:sz"><xsl:value-of select="main:sz/@val"/></xsl:when>
            <xsl:otherwise>-1</xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
        <xsl:attribute name="ftsItalic">
          <xsl:choose>
            <xsl:when test="main:i and not(boolean(main:i/@val)) or main:i/@val = 'true' or main:i/@val = '1'">true</xsl:when>
            <xsl:otherwise>false</xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
        <xsl:attribute name="ftsStrikeout">
          <xsl:choose>
            <xsl:when test="main:strike and not(boolean(main:strike/@val)) or main:strike/@val = 'true' or main:strike/@val = '1'">true</xsl:when>
            <xsl:otherwise>false</xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
        <xsl:attribute name="bls">
          <xsl:choose>
            <xsl:when test="main:b">
              <xsl:choose>
                <xsl:when test="not(boolean(main:b/@val)) or main:b/@val = 'true' or main:b/@val = '1'">700</xsl:when>
                <xsl:otherwise>400</xsl:otherwise>
              </xsl:choose>
            </xsl:when>
            <xsl:otherwise>-1</xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
        <xsl:attribute name="sss">
          <xsl:choose>
            <xsl:when test="main:vertAlign">
              <xsl:choose>
                <xsl:when test="main:vertAlign/@val = 'baseline'">0</xsl:when>
                <xsl:when test="main:vertAlign/@val = 'superscript'">1</xsl:when>
                <xsl:when test="main:vertAlign/@val = 'subscript'">2</xsl:when>
              </xsl:choose>
            </xsl:when>
            <xsl:otherwise>-1</xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
        <xsl:attribute name="uls">
          <xsl:choose>
            <xsl:when test="main:u">
              <xsl:choose>
                <xsl:when test="main:u/@val = 'none'">0</xsl:when>
                <xsl:when test="main:u/@val = 'single'">1</xsl:when>
                <xsl:when test="main:u/@val = 'double'">2</xsl:when>
                <xsl:when test="main:u/@val = 'singleAccounting'">33</xsl:when>
                <xsl:when test="main:u/@val = 'doubleAccounting'">34</xsl:when>
              </xsl:choose>
            </xsl:when>
            <xsl:otherwise>255</xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
        <xsl:attribute name="bFamily">
          <xsl:choose>
            <xsl:when test="main:family">
              <xsl:value-of select="main:family/@val"/>
            </xsl:when>
            <xsl:otherwise>0</xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
        <xsl:attribute name="bCharSet">
          <xsl:choose>
            <xsl:when test="main:charset">
              <xsl:value-of select="main:charset/@val"/>
            </xsl:when>
            <xsl:otherwise>0</xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
      </Stxp>
    </DXFFntD>
  </xsl:template>

  <xsl:template name="DXFALC">
    <DXFALC>
      <xsl:attribute name="alc">
        <xsl:call-template name="HorizontalAlignment_StringToID">
          <xsl:with-param name="string" select="@horizontal"/>
        </xsl:call-template>
      </xsl:attribute>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fWrap</xsl:with-param>
        <xsl:with-param name="val" select="@wrapText"/>
      </xsl:call-template>
      <xsl:attribute name="alcV">
        <xsl:call-template name="VerticalAlignment_StringToID">
          <xsl:with-param name="string" select="@vertical"/>
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
        <xsl:with-param name="name">fJustLast</xsl:with-param>
        <xsl:with-param name="val" select="@justifyLastLine"/>
      </xsl:call-template>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">trot</xsl:with-param>
        <xsl:with-param name="val" select="@textRotation"/>
        <xsl:with-param name="def_val">0</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">cIndent</xsl:with-param>
        <xsl:with-param name="val" select="@indent"/>
        <xsl:with-param name="def_val">0</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fShrinkToFit</xsl:with-param>
        <xsl:with-param name="val" select="@shrinkToFit"/>
      </xsl:call-template>
      <xsl:attribute name="fMergeCell">0</xsl:attribute>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">iReadOrder</xsl:with-param>
        <xsl:with-param name="val" select="@readingOrder"/>
        <xsl:with-param name="def_val">0</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">iIndent</xsl:with-param>
        <xsl:with-param name="val" select="@relativeIndent"/>
        <xsl:with-param name="def_val">255</xsl:with-param>
      </xsl:call-template>
    </DXFALC>
  </xsl:template>

  <xsl:template name="DXFBdr">
    <DXFBdr>
      <xsl:attribute name="dgLeft">
        <xsl:call-template name="BorderStyle_StringToID">
          <xsl:with-param name="string" select="main:left/@style"/>
        </xsl:call-template>
      </xsl:attribute>
      <xsl:attribute name="dgRight">
        <xsl:call-template name="BorderStyle_StringToID">
          <xsl:with-param name="string" select="main:right/@style"/>
        </xsl:call-template>
      </xsl:attribute>
      <xsl:attribute name="dgTop">
        <xsl:call-template name="BorderStyle_StringToID">
          <xsl:with-param name="string" select="main:top/@style"/>
        </xsl:call-template>
      </xsl:attribute>
      <xsl:attribute name="dgBottom">
        <xsl:call-template name="BorderStyle_StringToID">
          <xsl:with-param name="string" select="main:bottom/@style"/>
        </xsl:call-template>
      </xsl:attribute>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">icvLeft</xsl:with-param>
        <xsl:with-param name="val">
          <xsl:call-template name="color2Icv">
            <xsl:with-param name="color" select="main:left/main:color"/>
          </xsl:call-template>
        </xsl:with-param>
        <xsl:with-param name="def_val">0</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">icvRight</xsl:with-param>
        <xsl:with-param name="val">
          <xsl:call-template name="color2Icv">
            <xsl:with-param name="color" select="main:right/main:color"/>
          </xsl:call-template>
        </xsl:with-param>
        <xsl:with-param name="def_val">0</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">bitDiagDown</xsl:with-param>
        <xsl:with-param name="val" select="@diagonalDown"/>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">bitDiagUp</xsl:with-param>
        <xsl:with-param name="val" select="@diagonalUp"/>
      </xsl:call-template>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">icvTop</xsl:with-param>
        <xsl:with-param name="val">
          <xsl:call-template name="color2Icv">
            <xsl:with-param name="color" select="main:top/main:color"/>
          </xsl:call-template>
        </xsl:with-param>
        <xsl:with-param name="def_val">0</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">icvBottom</xsl:with-param>
        <xsl:with-param name="val">
          <xsl:call-template name="color2Icv">
            <xsl:with-param name="color" select="main:bottom/main:color"/>
          </xsl:call-template>
        </xsl:with-param>
        <xsl:with-param name="def_val">0</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_req_with_def">
        <xsl:with-param name="name">icvDiag</xsl:with-param>
        <xsl:with-param name="val">
          <xsl:call-template name="color2Icv">
            <xsl:with-param name="color" select="main:diagonal/main:color"/>
          </xsl:call-template>
        </xsl:with-param>
        <xsl:with-param name="def_val">0</xsl:with-param>
      </xsl:call-template>
      <xsl:attribute name="dgDiag">
        <xsl:call-template name="BorderStyle_StringToID">
          <xsl:with-param name="string" select="main:diagonal/@style"/>
        </xsl:call-template>
      </xsl:attribute>


    </DXFBdr>
  </xsl:template>
  
  <xsl:template name="DXFPat">
    <DXFPat>
      <xsl:choose>
        <xsl:when test="main:patternFill">
          <xsl:attribute name="fls">
            <xsl:call-template name="PatternType_StringToID">
              <xsl:with-param name="string" select="main:patternFill/@patternType"/>
            </xsl:call-template>
          </xsl:attribute>
          <xsl:call-template name="attrib_req_with_def">
            <xsl:with-param name="name">icvForeground</xsl:with-param>
            <xsl:with-param name="val">
              <xsl:call-template name="color2Icv">
                <xsl:with-param name="color" select="main:patternFill/main:fgColor"/>
              </xsl:call-template>
            </xsl:with-param>
            <xsl:with-param name="def_val">64</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_req_with_def">
            <xsl:with-param name="name">icvBackground</xsl:with-param>
            <xsl:with-param name="val">
              <xsl:call-template name="color2Icv">
                <xsl:with-param name="color" select="main:patternFill/main:bgColor"/>
              </xsl:call-template>
            </xsl:with-param>
            <xsl:with-param name="def_val">65</xsl:with-param>
          </xsl:call-template>
        </xsl:when>
        <xsl:when test="main:gradientFill">
          <xsl:attribute name="fls">16</xsl:attribute>
          <xsl:call-template name="attrib_req_with_def">
            <xsl:with-param name="name">icvForeground</xsl:with-param>
            <xsl:with-param name="val">
              <xsl:call-template name="color2Icv">
                <xsl:with-param name="color" select="main:gradientFill/main:stop[1]/main:color"/>
              </xsl:call-template>
            </xsl:with-param>
            <xsl:with-param name="def_val">64</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_req_with_def">
            <xsl:with-param name="name">icvBackground</xsl:with-param>
            <xsl:with-param name="val">
              <xsl:call-template name="color2Icv">
                <xsl:with-param name="color" select="main:gradientFill/main:stop[2]/main:color"/>
              </xsl:call-template>
            </xsl:with-param>
            <xsl:with-param name="def_val">65</xsl:with-param>
          </xsl:call-template>

        </xsl:when>
      </xsl:choose>
    </DXFPat>
  </xsl:template>

  <xsl:template name="DXFProt">
    <DXFProt>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fLocked</xsl:with-param>
        <xsl:with-param name="val" select="@locked"/>
        <xsl:with-param name="def_val">true</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_bool_req">
        <xsl:with-param name="name">fHidden</xsl:with-param>
        <xsl:with-param name="val" select="@hidden"/>
      </xsl:call-template>
    </DXFProt>
  </xsl:template>
  
</xsl:stylesheet>