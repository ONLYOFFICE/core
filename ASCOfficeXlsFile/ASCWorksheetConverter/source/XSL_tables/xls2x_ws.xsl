<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:msxsl="urn:schemas-microsoft-com:xslt"
                xmlns:xlsx="http://www.avs4you.ru/XLSXConverter/1.0/DirectoryStructure">

  <xsl:template match="WorksheetSubstream">
    <xlsx:file name="sheet{position()}.xml">
      <worksheet xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships">

        <xsl:call-template name="sheetPr"/>

        <xsl:if test="Dimensions/@ref != ''">
          <dimension ref="{Dimensions/@ref}"/>
        </xsl:if>

        <xsl:call-template name="sheetViews"/>
        <xsl:call-template name="sheetFormatPr"/>
        <xsl:call-template name="cols"/>

        <xsl:call-template name="sheetData"/>

        <!-- sheetCalcPr/@fullCalcOnLoad left default since no corresponding tag found in XLS -->
        <!-- The possible solution is:
        <xsl:if test="Uncalced">
          <sheetCalcPr fullCalcOnLoad="true"/>
        </xsl:if>
        -->

        <xsl:call-template name="sheetProtection"/>
        <xsl:call-template name="protectedRanges"/>

        <xsl:call-template name="scenarios"/>
        <!-- TODO: check and finish -->
        <xsl:call-template name="autoFilter"/>
        <!-- TODO: check and finish -->
        <xsl:call-template name="sortState"/>
        <xsl:call-template name="dataConsolidate"/>
        <!-- TODO: check and finish -->

        <xsl:call-template name="customSheetViews"/>
        <xsl:call-template name="mergeCells"/>

        <xsl:call-template name="phoneticPr"/>
        <!-- TODO: check and finish -->
        <xsl:call-template name="conditionalFormatting"/>
        <xsl:call-template name="dataValidations"/>
        <!-- TODO: check and finish -->
        <xsl:call-template name="hyperlinks"/>

        <xsl:for-each select="PAGESETUP[1]">
          <!-- [1] - is to be sure we don't generate more than one page setups -->
          <xsl:call-template name="printOptions"/>
          <xsl:call-template name="pageMargins"/>
          <xsl:call-template name="pageSetup"/>
          <xsl:call-template name="headerFooter"/>
        </xsl:for-each>

        <xsl:for-each select="GLOBALS[1]">
          <!-- [1] - is to be sure we don't generate more than one page setups -->
          <xsl:apply-templates select="HorizontalPageBreaks" mode="view"/>
          <xsl:apply-templates select="VerticalPageBreaks" mode="view"/>
        </xsl:for-each>

        <xsl:call-template name="cellWatches"/>

        <xsl:call-template name="drawing"/>
        <xsl:call-template name="legacyDrawing"/>
        <xsl:call-template name="tableParts"/>

        <!-- TODO: Implement the rest -->

      </worksheet>
    </xlsx:file>
  </xsl:template>


  <xsl:template match="Selection" mode="view">
    <xsl:variable name="irefAct">
      <xsl:value-of select="@irefAct"/>
    </xsl:variable>
    <xsl:if test="@pnn != 'topLeft' or @activeCell != 'A1' or $irefAct != 0 or @sqref != 'A1'">
      <selection xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:if test="@pnn != 'topLeft'">
          <xsl:attribute name="pane">
            <xsl:value-of select="@pnn"/>
          </xsl:attribute>
        </xsl:if>
        <xsl:call-template name="attrib_with_def">
          <xsl:with-param name="name">activeCell</xsl:with-param>
          <xsl:with-param name="val" select="@activeCell"/>
          <xsl:with-param name="def_val">A1</xsl:with-param>
        </xsl:call-template>
        <xsl:if test="$irefAct != 0">
          <xsl:attribute name="activeCellId">
            <xsl:value-of select="$irefAct"/>
          </xsl:attribute>
        </xsl:if>
        <xsl:if test="@sqref != 'A1'">
          <xsl:attribute name="sqref">
            <xsl:value-of select="@sqref"/>
          </xsl:attribute>
        </xsl:if>
      </selection>
    </xsl:if>
  </xsl:template>

  <xsl:template match="HorizontalPageBreaks" mode="view">
    <rowBreaks xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
      <xsl:call-template name="attrib_with_def">
        <xsl:with-param name="name">count</xsl:with-param>
        <xsl:with-param name="val" select="@cbrk"/>
        <xsl:with-param name="def_val">0</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_with_def">
        <xsl:with-param name="name">manualBreakCount</xsl:with-param>
        <xsl:with-param name="val" select="@cbrk"/>
        <xsl:with-param name="def_val">0</xsl:with-param>
      </xsl:call-template>
      <xsl:for-each select="HorzBrk">
        <brk>
          <xsl:call-template name="attrib_with_def">
            <xsl:with-param name="name">id</xsl:with-param>
            <xsl:with-param name="val" select="@row"/>
            <xsl:with-param name="def_val">0</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_with_def">
            <xsl:with-param name="name">min</xsl:with-param>
            <xsl:with-param name="val">
              <xsl:choose>
                <xsl:when test="@colStart='255'">16383</xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="@colStart"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:with-param>
            <xsl:with-param name="def_val">0</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_with_def">
            <xsl:with-param name="name">max</xsl:with-param>
            <xsl:with-param name="val">
              <xsl:choose>
                <xsl:when test="@colEnd='255'">16383</xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="@colEnd"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:with-param>
            <xsl:with-param name="def_val">0</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_with_def">
            <xsl:with-param name="name">man</xsl:with-param>
            <xsl:with-param name="val">true</xsl:with-param>
            <!-- TODO: check the other variants -->
            <xsl:with-param name="def_val">false</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_with_def">
            <xsl:with-param name="name">pt</xsl:with-param>
            <!-- TODO: Process Pivot tables -->
            <xsl:with-param name="val" select="'false'"/>
            <xsl:with-param name="def_val">false</xsl:with-param>
          </xsl:call-template>
        </brk>
      </xsl:for-each>
    </rowBreaks>
  </xsl:template>

  <xsl:template match="VerticalPageBreaks" mode="view">
    <colBreaks xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
      <xsl:call-template name="attrib_with_def">
        <xsl:with-param name="name">count</xsl:with-param>
        <xsl:with-param name="val" select="@cbrk"/>
        <xsl:with-param name="def_val">0</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_with_def">
        <xsl:with-param name="name">manualBreakCount</xsl:with-param>
        <xsl:with-param name="val" select="@cbrk"/>
        <xsl:with-param name="def_val">0</xsl:with-param>
      </xsl:call-template>
      <xsl:for-each select="VertBrk">
        <brk>
          <xsl:call-template name="attrib_with_def">
            <xsl:with-param name="name">id</xsl:with-param>
            <xsl:with-param name="val" select="@col"/>
            <xsl:with-param name="def_val">0</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_with_def">
            <xsl:with-param name="name">min</xsl:with-param>
            <xsl:with-param name="val">
              <xsl:choose>
                <xsl:when test="@rowStart='65535'">1048575</xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="@rowStart"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:with-param>
            <xsl:with-param name="def_val">0</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_with_def">
            <xsl:with-param name="name">max</xsl:with-param>
            <xsl:with-param name="val">
              <xsl:choose>
                <xsl:when test="@rowEnd='65535'">1048575</xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="@rowEnd"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:with-param>
            <xsl:with-param name="def_val">0</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_with_def">
            <xsl:with-param name="name">man</xsl:with-param>
            <xsl:with-param name="val">true</xsl:with-param>
            <!-- TODO: check the other variants -->
            <xsl:with-param name="def_val">false</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_with_def">
            <xsl:with-param name="name">pt</xsl:with-param>
            <!-- TODO: Process Pivot tables -->
            <xsl:with-param name="val" select="false"/>
            <xsl:with-param name="def_val">false</xsl:with-param>
          </xsl:call-template>
        </brk>
      </xsl:for-each>
    </colBreaks>
  </xsl:template>

  <xsl:template name="pageMargins">
    <xsl:if test="LeftMargin">
      <pageMargins left="{LeftMargin/@num}" right="{RightMargin/@num}" top="{TopMargin/@num}"
                   bottom="{BottomMargin/@num}" header="{Setup/@numHdr}" footer="{Setup/@numFtr}" xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main"/>
    </xsl:if>
  </xsl:template>

  <xsl:template name="printOptions">
    <xsl:if test="UserSViewBegin/@fHorizontal = 'true' or UserSViewBegin/@fVertical = 'true' or
            self::CUSTOMVIEW and (UserSViewBegin/@fPrintRwCol = 'true' or UserSViewBegin/@fPrintGrid = 'true') or
            self::PAGESETUP and (../GLOBALS/PrintRowCol/@printRwCol = 'true' or
            ../GLOBALS/PrintGrid/@fPrintGrid = 'true')">
      <printOptions xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:if test="UserSViewBegin/@fHorizontal = 'true'">
          <xsl:attribute name="horizontalCentered">true</xsl:attribute>
        </xsl:if>
        <xsl:if test="UserSViewBegin/@fVertical = 'true'">
          <xsl:attribute name="verticalCentered">true</xsl:attribute>
        </xsl:if>
        <xsl:choose>
          <xsl:when test="self::CUSTOMVIEW">
            <xsl:if test="UserSViewBegin/@fPrintRwCol = 'true'">
              <xsl:attribute name="headings">true</xsl:attribute>
            </xsl:if>
            <xsl:if test="UserSViewBegin/@fPrintGrid = 'true'">
              <xsl:attribute name="gridLines">true</xsl:attribute>
            </xsl:if>
          </xsl:when>
          <xsl:when test="self::PAGESETUP">
            <xsl:if test="../GLOBALS/PrintRowCol/@printRwCol = 'true'">
              <xsl:attribute name="headings">true</xsl:attribute>
            </xsl:if>
            <xsl:if test="../GLOBALS/PrintGrid/@fPrintGrid = 'true'">
              <xsl:attribute name="gridLines">true</xsl:attribute>
            </xsl:if>
          </xsl:when>
        </xsl:choose>
        <!-- gridLinesSet ommited to be always default-->
      </printOptions>
    </xsl:if>
  </xsl:template>

  <xsl:template name="pageSetup">
    <xsl:if test="Setup/@iPaperSize != '1' or Setup/@dmPaperLength or Setup/@dmPaperWidth or Setup/@iScale != '100' or
            Setup/@iPageStart != '1' or Setup/@iFitWidth != '1' or Setup/@iFitHeight != '1' or Setup/@fLeftToRight='true' or
            Setup/@fPortrait or Setup/@fNoColor = 'true' or Setup/@fDraft = 'true' or 
            (Setup/@fNotes='true' and Setup/@fEndNotes='true') or (Setup/@fNotes='true' and Setup/@fEndNotes='false') or
            Setup/@fUsePage = 'true' or (Setup/@iErrors and Setup/@iErrors != 0) or Setup/@iRes != '600' or
            Setup/@iVRes != '600' or Setup/@iCopies != '1' or Pls">
      <pageSetup xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships">
        <xsl:if test="Setup/@iPaperSize != '1'">
          <xsl:attribute name="paperSize">
            <xsl:value-of select="Setup/@iPaperSize"/>
          </xsl:attribute>
        </xsl:if>
        <xsl:if test="Setup/@dmPaperLength">
          <xsl:attribute name="paperHeight">
            <xsl:value-of select="Setup/@dmPaperLength"/>mm
          </xsl:attribute>
        </xsl:if>
        <xsl:if test="Setup/@dmPaperWidth">
          <xsl:attribute name="paperWidth">
            <xsl:value-of select="Setup/@dmPaperWidth"/>mm
          </xsl:attribute>
        </xsl:if>
        <xsl:if test="Setup/@iScale != '100'">
          <xsl:attribute name="scale">
            <xsl:value-of select="Setup/@iScale"/>
          </xsl:attribute>
        </xsl:if>
        <xsl:if test="Setup/@iPageStart != '1'">
          <xsl:attribute name="firstPageNumber">
            <xsl:value-of select="Setup/@iPageStart"/>
          </xsl:attribute>
        </xsl:if>
        <xsl:if test="Setup/@iFitWidth != '1'">
          <xsl:attribute name="fitToWidth">
            <xsl:value-of select="Setup/@iFitWidth"/>
          </xsl:attribute>
        </xsl:if>
        <xsl:if test="Setup/@iFitHeight != '1'">
          <xsl:attribute name="fitToHeight">
            <xsl:value-of select="Setup/@iFitHeight"/>
          </xsl:attribute>
        </xsl:if>
        <xsl:if test="Setup/@fLeftToRight='true'">
          <xsl:attribute name="pageOrder">overThenDown</xsl:attribute>
          <!-- The default is 'downThenOver' -->
        </xsl:if>
        <xsl:choose>
          <xsl:when test="Setup/@fPortrait='true'">
            <xsl:attribute name="orientation">portrait</xsl:attribute>
          </xsl:when>
          <xsl:when test="Setup/@fPortrait='false'">
            <xsl:attribute name="orientation">landscape</xsl:attribute>
          </xsl:when>
          <!-- The default is 'default' and ommited (Occurs when @fPortrait is absent). -->
        </xsl:choose>

        <!-- usePrinterDefaults left default since no corresponding tag in the XLS -->

        <xsl:if test="Setup/@fNoColor = 'true'">
          <xsl:attribute name="blackAndWhite">true</xsl:attribute>
        </xsl:if>
        <xsl:if test="Setup/@fDraft = 'true'">
          <xsl:attribute name="draft">true</xsl:attribute>
        </xsl:if>
        <xsl:choose>
          <xsl:when test="Setup/@fNotes='true' and Setup/@fEndNotes='true'">
            <xsl:attribute name="cellComments">atEnd</xsl:attribute>
          </xsl:when>
          <xsl:when test="Setup/@fNotes='true' and Setup/@fEndNotes='false'">
            <xsl:attribute name="cellComments">asDisplayed</xsl:attribute>
          </xsl:when>
          <!-- The default is 'none' and ommited. -->
        </xsl:choose>
        <xsl:if test="Setup/@fUsePage = 'true'">
          <xsl:attribute name="useFirstPageNumber">true</xsl:attribute>
        </xsl:if>
        <xsl:choose>
          <!-- Default value 'displayed' ommited -->
          <xsl:when test="Setup/@iErrors = 1">
            <xsl:attribute name="errors">blank</xsl:attribute>
          </xsl:when>
          <xsl:when test="Setup/@iErrors = 2">
            <xsl:attribute name="errors">dash</xsl:attribute>
          </xsl:when>
          <xsl:when test="Setup/@iErrors = 3">
            <xsl:attribute name="errors">NA</xsl:attribute>
          </xsl:when>
        </xsl:choose>
        <xsl:if test="Setup/@iRes != '600'">
          <xsl:attribute name="horizontalDpi">
            <xsl:value-of select="Setup/@iRes"/>
          </xsl:attribute>
        </xsl:if>
        <xsl:if test="Setup/@iVRes != '600'">
          <xsl:attribute name="verticalDpi">
            <xsl:value-of select="Setup/@iVRes"/>
          </xsl:attribute>
        </xsl:if>
        <xsl:if test="Setup/@iCopies != '1'">
          <xsl:attribute name="copies">
            <xsl:value-of select="Setup/@iCopies"/>
          </xsl:attribute>
        </xsl:if>
        <xsl:if test="Pls">
          <!-- This 'if' ensures that we have binary data -->
          <xsl:attribute name="r:id">
            <xsl:text>rId</xsl:text>
            <xsl:call-template name="generate_worksheet_entity_id"/>
          </xsl:attribute>
        </xsl:if>
      </pageSetup>
    </xsl:if>
  </xsl:template>

  <!-- Local Id of printer settings for .rels and local reference -->
  <xsl:template name="generate_worksheet_entity_id">
    <xsl:variable name="Note_present">
      <xsl:choose>
        <xsl:when test="../Note">1</xsl:when>
        <xsl:otherwise>0</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="self::HLINK">
        <xsl:value-of select="count(preceding-sibling::HLINK/HLink/HyperlinkObject[@hlstmfHasMoniker = 'true']) + 1"/>
      </xsl:when>
      <xsl:when test="self::CUSTOMVIEW">
        <xsl:value-of select="count(preceding-sibling::CUSTOMVIEW/Pls) + 1 + count(../HLINK/HLink/HyperlinkObject[@hlstmfHasMoniker = 'true'])"/>
      </xsl:when>
      <xsl:when test="self::PAGESETUP">
        <xsl:value-of select="count(preceding-sibling::PAGESETUP/Pls) + 1 + count(../CUSTOMVIEW/Pls) + count(../HLINK/HLink/HyperlinkObject[@hlstmfHasMoniker = 'true'])"/>
      </xsl:when>
      <!-- Note should occur only once-->
      <xsl:when test="self::Note">
        <xsl:value-of select="1 + 0 + count(../PAGESETUP/Pls) + count(../CUSTOMVIEW/Pls) + count(../HLINK/HLink/HyperlinkObject[@hlstmfHasMoniker = 'true'])"/>
      </xsl:when>
      <!-- OBJECTS should occur only once -->
      <xsl:when test="self::OBJECTS">
        <xsl:value-of select="1 + $Note_present + count(../PAGESETUP/Pls) + count(../CUSTOMVIEW/Pls) + count(../HLINK/HLink/HyperlinkObject[@hlstmfHasMoniker = 'true'])"/>
      </xsl:when>
      <xsl:when test="self::ChartSheetSubstream">
        <xsl:value-of select="1 + count(PAGESETUP/Pls) + count(CUSTOMVIEW/Pls)"/>
      </xsl:when>
      <xsl:otherwise>0</xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- Global ID of printer settings for .rels and settings files numbering -->
  <xsl:template name="generate_printer_settings_id">
    <!-- Current node must be PAGESETUP or CUSTOMVIEW -->
    <xsl:variable name="localId">
      <xsl:value-of select="count(preceding-sibling::PAGESETUP/Pls) + count(preceding-sibling::CUSTOMVIEW/Pls) + 1"/>
    </xsl:variable>
    <xsl:variable name="prev_ws" select="../preceding-sibling::WorksheetSubstream"/>
    <xsl:variable name="prev_cs" select="../preceding-sibling::ChartSheetSubstream"/>

    <xsl:variable name="base_pls_number">
      <xsl:value-of select="count($prev_ws/PAGESETUP/Pls) + count($prev_ws/CUSTOMVIEW/Pls) + count($prev_cs/PAGESETUP/Pls) + count($prev_cs/CUSTOMVIEW/Pls)"/>
    </xsl:variable>
    <xsl:value-of select="$localId + $base_pls_number"/>
  </xsl:template>

  <xsl:template name="headerFooter">
    <xsl:if test="HeaderFooter and (HeaderFooter/@fHFDiffOddEven = 'true' or HeaderFooter/@fHFDiffFirst = 'true' or
            HeaderFooter/@fHFScaleWithDoc = 'false' or HeaderFooter/@fHFAlignMargins = 'false' or
            Header/@ast != '' or Footer/@ast != '' or HeaderFooter/@fHFDiffOddEven = 'true' and
            (HeaderFooter/@strHeaderEven != '' or HeaderFooter/@strFooterEven != '') or
            HeaderFooter/@fHFDiffFirst = 'true' and (HeaderFooter/@strHeaderFirst != '' or
            HeaderFooter/@strFooterFirst != ''))">
      <headerFooter xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:if test="HeaderFooter/@fHFDiffOddEven = 'true'">
          <xsl:attribute name="differentOddEven">true</xsl:attribute>
        </xsl:if>
        <xsl:if test="HeaderFooter/@fHFDiffFirst = 'true'">
          <xsl:attribute name="differentFirst">true</xsl:attribute>
        </xsl:if>
        <xsl:if test="HeaderFooter/@fHFScaleWithDoc = 'false'">
          <xsl:attribute name="scaleWithDoc">false</xsl:attribute>
        </xsl:if>
        <xsl:if test="HeaderFooter/@fHFAlignMargins = 'false'">
          <xsl:attribute name="alignWithMargins">false</xsl:attribute>
        </xsl:if>

        <xsl:if test="Header/@ast != ''">
          <oddHeader>
            <xsl:value-of select="Header/@ast"/>
          </oddHeader>
        </xsl:if>
        <xsl:if test="Footer/@ast != ''">
          <oddFooter>
            <xsl:value-of select="Footer/@ast"/>
          </oddFooter>
        </xsl:if>

        <xsl:if test="HeaderFooter/@fHFDiffOddEven = 'true'">
          <xsl:if test="HeaderFooter/@strHeaderEven != ''">
            <evenHeader>
              <xsl:value-of select="HeaderFooter/@strHeaderEven"/>
            </evenHeader>
          </xsl:if>
          <xsl:if test="HeaderFooter/@strFooterEven != ''">
            <evenFooter>
              <xsl:value-of select="HeaderFooter/@strFooterEven"/>
            </evenFooter>
          </xsl:if>
        </xsl:if>

        <xsl:if test="HeaderFooter/@fHFDiffFirst = 'true'">
          <xsl:if test="HeaderFooter/@strHeaderFirst != ''">
            <firstHeader>
              <xsl:value-of select="HeaderFooter/@strHeaderFirst"/>
            </firstHeader>
          </xsl:if>
          <xsl:if test="HeaderFooter/@strFooterFirst != ''">
            <firstFooter>
              <xsl:value-of select="HeaderFooter/@strFooterFirst"/>
            </firstFooter>
          </xsl:if>
        </xsl:if>
      </headerFooter>
    </xsl:if>
  </xsl:template>

  <xsl:template name="sheetPr">
    <xsl:if test="CodeName/@codeName != '' or SheetExt/SheetExtOptional/@fCondFmtCalc = 'false' or
            SORTANDFILTER/FilterMode or SheetExt/SheetExtOptional/@fNotPublished = 'false' or
            GLOBALS/WsBool/@fSyncHoriz = 'true' or GLOBALS/WsBool/@fSyncVert = 'true' or
            GLOBALS/Sync/@ref != '' or GLOBALS/WsBool/@fAltFormulaEntry = 'true' or
            GLOBALS/WsBool/@fAltExprEval = 'true' or SheetExt/SheetExtOptional/CFColor or
            GLOBALS/WsBool/@fApplyStyles = 'true' or GLOBALS/WsBool/@fRowSumsBelow = 'false' or
            CUSTOMVIEW/UserSViewBegin/@fDspGutsSv = 'false' or GLOBALS/WsBool/@fShowAutoBreaks = 'false' or 
            GLOBALS/WsBool/@fFitToPage = 'true'">
      <sheetPr xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:if test="CodeName/@codeName != ''">
          <xsl:attribute name="codeName">
            <xsl:value-of select="CodeName/@codeName"/>
          </xsl:attribute>
        </xsl:if>
        <xsl:if test="SheetExt/SheetExtOptional/@fCondFmtCalc = 'false'">
          <xsl:attribute name="enableFormatConditionsCalculation">false</xsl:attribute>
        </xsl:if>
        <xsl:if test="SORTANDFILTER/FilterMode">
          <!-- If at least one exists -->
          <xsl:attribute name="filterMode">true</xsl:attribute>
        </xsl:if>
        <xsl:if test="SheetExt/SheetExtOptional/@fNotPublished = 'false'">
          <xsl:attribute name="published">false</xsl:attribute>
        </xsl:if>
        <xsl:if test="GLOBALS/WsBool/@fSyncHoriz = 'true'">
          <xsl:attribute name="syncHorizontal">true</xsl:attribute>
        </xsl:if>
        <xsl:if test="GLOBALS/WsBool/@fSyncVert = 'true'">
          <xsl:attribute name="syncVertical">true</xsl:attribute>
        </xsl:if>
        <xsl:if test="GLOBALS/Sync/@ref != ''">
          <xsl:attribute name="syncRef">
            <xsl:value-of select="GLOBALS/Sync/@ref"/>
          </xsl:attribute>
        </xsl:if>
        <xsl:if test="GLOBALS/WsBool/@fAltFormulaEntry = 'true'">
          <xsl:attribute name="transitionEntry">true</xsl:attribute>
        </xsl:if>
        <xsl:if test="GLOBALS/WsBool/@fAltExprEval = 'true'">
          <xsl:attribute name="transitionEvaluation">true</xsl:attribute>
        </xsl:if>

        <xsl:if test="SheetExt/SheetExtOptional/CFColor">
          <xsl:call-template name="color">
            <xsl:with-param name="tag_name">tabColor</xsl:with-param>
            <xsl:with-param name="propColor" select="SheetExt/SheetExtOptional/CFColor"/>
          </xsl:call-template>
        </xsl:if>
        <xsl:if test="GLOBALS/WsBool/@fApplyStyles = 'true' or GLOBALS/WsBool/@fDspGuts = 'false' or 
                      GLOBALS/WsBool/@fRowSumsBelow = 'false' or GLOBALS/WsBool/@fColSumsRight = 'false'">
          <outlinePr>
            <xsl:if test="GLOBALS/WsBool/@fApplyStyles = 'true'">
              <xsl:attribute name="applyStyles">true</xsl:attribute>
            </xsl:if>
            <xsl:if test="GLOBALS/WsBool/@fDspGuts = 'false'">
              <xsl:attribute name="showOutlineSymbols">false</xsl:attribute>
            </xsl:if>
            <xsl:if test="GLOBALS/WsBool/@fRowSumsBelow = 'false'">
              <xsl:attribute name="summaryBelow">false</xsl:attribute>
            </xsl:if>
            <xsl:if test="GLOBALS/WsBool/@fColSumsRight = 'false'">
              <xsl:attribute name="summaryRight">false</xsl:attribute>
            </xsl:if>
          </outlinePr>
        </xsl:if>
        <xsl:if test="GLOBALS/WsBool/@fShowAutoBreaks = 'false' or GLOBALS/WsBool/@fFitToPage = 'true'">
          <pageSetUpPr>
            <xsl:if test="GLOBALS/WsBool/@fShowAutoBreaks = 'false'">
              <xsl:attribute name="autoPageBreaks">false</xsl:attribute>
            </xsl:if>
            <xsl:if test="GLOBALS/WsBool/@fFitToPage = 'true'">
              <xsl:attribute name="fitToPage">true</xsl:attribute>
            </xsl:if>
          </pageSetUpPr>
        </xsl:if>
      </sheetPr>
    </xsl:if>
  </xsl:template>

  <xsl:template name="sheetViews">
    <sheetViews xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
      <xsl:for-each select="WINDOW">
        <sheetView>
          <xsl:call-template name="attrib_with_def">
            <xsl:with-param name="name">windowProtection</xsl:with-param>
            <xsl:with-param name="val" select="/root/WorkbookStreamObject/GlobalsSubstream/PROTECTION/WinProtect/@fLockWn"/>
            <xsl:with-param name="def_val">false</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_with_def">
            <xsl:with-param name="name">showFormulas</xsl:with-param>
            <xsl:with-param name="val" select="Window2/@fDspFmlaRt"/>
            <xsl:with-param name="def_val">false</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_with_def">
            <xsl:with-param name="name">showGridLines</xsl:with-param>
            <xsl:with-param name="val" select="Window2/@fDspGridRt"/>
            <xsl:with-param name="def_val">true</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_with_def">
            <xsl:with-param name="name">showRowColHeaders</xsl:with-param>
            <xsl:with-param name="val" select="Window2/@fDspRwColRt"/>
            <xsl:with-param name="def_val">true</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_with_def">
            <xsl:with-param name="name">showZeros</xsl:with-param>
            <xsl:with-param name="val" select="Window2/@fDspZerosRt"/>
            <xsl:with-param name="def_val">true</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_with_def">
            <xsl:with-param name="name">rightToLeft</xsl:with-param>
            <xsl:with-param name="val" select="Window2/@fRightToLeft"/>
            <xsl:with-param name="def_val">false</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_with_def">
            <xsl:with-param name="name">tabSelected</xsl:with-param>
            <xsl:with-param name="val" select="Window2/@fSelected"/>
            <xsl:with-param name="def_val">false</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_with_def">
            <xsl:with-param name="name">showRuler</xsl:with-param>
            <xsl:with-param name="val" select="PLV/@fRulerVisible"/>
            <xsl:with-param name="def_val">true</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_with_def">
            <xsl:with-param name="name">showOutlineSymbols</xsl:with-param>
            <xsl:with-param name="val" select="Window2/@fDspGuts"/>
            <xsl:with-param name="def_val">true</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_with_def">
            <xsl:with-param name="name">defaultGridColor</xsl:with-param>
            <xsl:with-param name="val" select="Window2/@fDefaultHdr"/>
            <xsl:with-param name="def_val">true</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_inv_with_def">
            <!-- Inversed -->
            <xsl:with-param name="name">showWhiteSpace</xsl:with-param>
            <xsl:with-param name="val" select="PLV/@fWhitespaceHidden"/>
            <xsl:with-param name="def_val">true</xsl:with-param>
          </xsl:call-template>
          <xsl:choose>
            <xsl:when test="Window2/@fSLV = 'true'">
              <xsl:attribute name="view">pageBreakPreview</xsl:attribute>
            </xsl:when>
            <xsl:when test="PLV/@fPageLayoutView = 'true'">
              <xsl:attribute name="view">pageLayout</xsl:attribute>
            </xsl:when>
            <!-- The default is 'normal' and ommited -->
          </xsl:choose>
          <xsl:call-template name="attrib_with_def">
            <xsl:with-param name="name">topLeftCell</xsl:with-param>
            <xsl:with-param name="val" select="Window2/@topLeftCell"/>
            <xsl:with-param name="def_val">A1</xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="attrib_with_def">
            <xsl:with-param name="name">colorId</xsl:with-param>
            <xsl:with-param name="val" select="Window2/@icvHdr"/>
            <xsl:with-param name="def_val">64</xsl:with-param>
          </xsl:call-template>
          <xsl:if test="Scl">
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">zoomScale</xsl:with-param>
              <xsl:with-param name="val" select="round(Scl/@nscl div Scl/@dscl * 100)"/>
              <!-- Back:  @dscl = 100; @nscl = zoomScale -->
              <xsl:with-param name="def_val">100</xsl:with-param>
            </xsl:call-template>
          </xsl:if>
          <xsl:if test="Window2/@fSLV = 'true' or PLV/fPageLayoutView = 'true' or not(Scl)">
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">zoomScaleNormal</xsl:with-param>
              <xsl:with-param name="val" select="Window2/@wScaleNormal"/>
              <xsl:with-param name="def_val">0</xsl:with-param>
            </xsl:call-template>
          </xsl:if>
          <xsl:if test="Window2/@fSLV = 'false' or not(Scl)">
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">zoomScaleSheetLayoutView</xsl:with-param>
              <xsl:with-param name="val" select="Window2/@wScaleSLV"/>
              <xsl:with-param name="def_val">0</xsl:with-param>
            </xsl:call-template>
          </xsl:if>
          <xsl:call-template name="attrib_with_def">
            <xsl:with-param name="name">zoomScalePageLayoutView</xsl:with-param>
            <xsl:with-param name="val" select="PLV/@wScalePLV"/>
            <xsl:with-param name="def_val">0</xsl:with-param>
          </xsl:call-template>
          <xsl:attribute name="workbookViewId">
            <xsl:value-of select="position() - 1"/>
          </xsl:attribute>

          <xsl:if test="Pane">
            <pane>
              <xsl:call-template name="attrib_with_def">
                <xsl:with-param name="name">xSplit</xsl:with-param>
                <xsl:with-param name="val" select="Pane/@x"/>
                <xsl:with-param name="def_val">0</xsl:with-param>
              </xsl:call-template>
              <xsl:call-template name="attrib_with_def">
                <xsl:with-param name="name">ySplit</xsl:with-param>
                <xsl:with-param name="val" select="Pane/@y"/>
                <xsl:with-param name="def_val">0</xsl:with-param>
              </xsl:call-template>
              <xsl:call-template name="attrib_with_def">
                <xsl:with-param name="name">topLeftCell</xsl:with-param>
                <xsl:with-param name="val" select="Pane/@topLeftCell"/>
                <xsl:with-param name="def_val">A1</xsl:with-param>
              </xsl:call-template>
              <xsl:call-template name="attrib_with_def">
                <xsl:with-param name="name">activePane</xsl:with-param>
                <xsl:with-param name="val" select="Pane/@pnnAcct"/>
                <xsl:with-param name="def_val">topLeft</xsl:with-param>
              </xsl:call-template>
              <xsl:choose>
                <xsl:when test="Window2/@fFrozenRt = 'true' and Window2/@fFrozenNoSplit = 'true'">
                  <xsl:attribute name="state">frozen</xsl:attribute>
                </xsl:when>
                <xsl:when test="Window2/@fFrozenRt = 'true' and Window2/@fFrozenNoSplit = 'false'">
                  <xsl:attribute name="state">frozenSplit</xsl:attribute>
                </xsl:when>
                <!-- The default is 'split' and ommited -->
              </xsl:choose>
            </pane>
          </xsl:if>

          <xsl:apply-templates select="Selection" mode="view"/>
        </sheetView>
      </xsl:for-each>
    </sheetViews>
  </xsl:template>

  <xsl:template name="sheetFormatPr">
    <sheetFormatPr xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
      <xsl:call-template name="attrib_with_def">
        <xsl:with-param name="name">baseColWidth</xsl:with-param>
        <xsl:with-param name="val" select="COLUMNS/DefColWidth/@cchdefColWidth"/>
        <xsl:with-param name="def_val">8</xsl:with-param>
      </xsl:call-template>
      <xsl:if test="DxGCol">
        <xsl:call-template name="attrib">
          <xsl:with-param name="name">defaultColWidth</xsl:with-param>
          <xsl:with-param name="val" select="DxGCol/@dxgCol div 256"/>
        </xsl:call-template>
      </xsl:if>
      <xsl:call-template name="attrib">
        <xsl:with-param name="name">defaultRowHeight</xsl:with-param>
        <xsl:with-param name="val">
          <xsl:choose>
            <xsl:when test="GLOBALS/DefaultRowHeight/@miyRw != ''">
              <xsl:value-of select="GLOBALS/DefaultRowHeight/@miyRw div 20"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="GLOBALS/DefaultRowHeight/@miyRwHidden div 20"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_with_def">
        <xsl:with-param name="name">customHeigh</xsl:with-param>
        <xsl:with-param name="val" select="GLOBALS/DefaultRowHeight/@fUnsynced"/>
        <xsl:with-param name="def_val">false</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_with_def">
        <xsl:with-param name="name">zeroHeight</xsl:with-param>
        <xsl:with-param name="val" select="GLOBALS/DefaultRowHeight/@fDyZero"/>
        <xsl:with-param name="def_val">false</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_with_def">
        <xsl:with-param name="name">thickTop</xsl:with-param>
        <xsl:with-param name="val" select="GLOBALS/DefaultRowHeight/@fExAsc"/>
        <xsl:with-param name="def_val">false</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_with_def">
        <xsl:with-param name="name">thickBottom</xsl:with-param>
        <xsl:with-param name="val" select="GLOBALS/DefaultRowHeight/@fExDsc"/>
        <xsl:with-param name="def_val">false</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_with_def">
        <xsl:with-param name="name">outlineLevelRow</xsl:with-param>
        <xsl:with-param name="val" select="GLOBALS/Guts/@iLevelRwMac"/>
        <xsl:with-param name="def_val">0</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="attrib_with_def">
        <xsl:with-param name="name">outlineLevelCol</xsl:with-param>
        <xsl:with-param name="val" select="GLOBALS/Guts/@iLevelColMac"/>
        <xsl:with-param name="def_val">0</xsl:with-param>
      </xsl:call-template>
    </sheetFormatPr>
  </xsl:template>

  <xsl:template name="cols">
    <xsl:if test="COLUMNS/ColInfo">
      <cols xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:for-each select="COLUMNS/ColInfo">
          <xsl:variable name="colLast">
            <xsl:choose>
              <xsl:when test="@colLast = 256">16384</xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="@colLast + 1"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:variable>
          <col min="{@colFirst + 1}" max="{$colLast}">
            <xsl:call-template name="attrib">
              <xsl:with-param name="name">width</xsl:with-param>
              <xsl:with-param name="val" select="@coldx div 256"/>
            </xsl:call-template>
            <xsl:variable name="ixfe" select="@ixfe"/>
            <xsl:if test="$ixfe != $ixfe0">
              <xsl:attribute name="style">
                <xsl:value-of select="key('get_cell_xf', $ixfe)/@cell_xf_current_id"/>
              </xsl:attribute>
            </xsl:if>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">hidden</xsl:with-param>
              <xsl:with-param name="val" select="@fHidden"/>
              <xsl:with-param name="def_val">false</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">bestFit</xsl:with-param>
              <xsl:with-param name="val" select="@fBestFit"/>
              <xsl:with-param name="def_val">false</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">customWidth</xsl:with-param>
              <xsl:with-param name="val" select="@fUserSet"/>
              <xsl:with-param name="def_val">false</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">phonetic</xsl:with-param>
              <xsl:with-param name="val" select="@fPhonetic"/>
              <xsl:with-param name="def_val">false</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">outlineLevel</xsl:with-param>
              <xsl:with-param name="val" select="@iOutLevel"/>
              <xsl:with-param name="def_val">0</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">collapsed</xsl:with-param>
              <xsl:with-param name="val" select="@fCollapsed"/>
              <xsl:with-param name="def_val">false</xsl:with-param>
            </xsl:call-template>
          </col>
        </xsl:for-each>
      </cols>
    </xsl:if>
  </xsl:template>

  <xsl:template name="sheetProtection">
    <xsl:if test="PROTECTION">
      <sheetProtection xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <!-- algorithmName, hashValue, saltValue, spinCount have no corresponding tags in XLS -->
        <!-- password attribute has been deprecated as far as I understood, but there is no other 
             way to translate WorksheetSubstream/PROTECTION/Password into XLSX format -->
        <xsl:call-template name="attrib">
          <xsl:with-param name="name">password</xsl:with-param>
          <xsl:with-param name="val" select="PROTECTION/Password/@wPassword"/>
        </xsl:call-template>
        <xsl:call-template name="attrib_with_def">
          <xsl:with-param name="name">sheet</xsl:with-param>
          <xsl:with-param name="val" select="PROTECTION/Protect/@fLock"/>
          <xsl:with-param name="def_val">false</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_with_def">
          <xsl:with-param name="name">objects</xsl:with-param>
          <xsl:with-param name="val" select="PROTECTION/ObjProtect/@fLockObj"/>
          <xsl:with-param name="def_val">false</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_with_def">
          <xsl:with-param name="name">scenarios</xsl:with-param>
          <xsl:with-param name="val" select="PROTECTION/ScenarioProtect/@fScenProtect"/>
          <xsl:with-param name="def_val">false</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_inv_with_def">
          <!-- Inversed -->
          <xsl:with-param name="name">formatCells</xsl:with-param>
          <xsl:with-param name="val" select="FEAT/FeatHdr/EnhancedProtection/@iprotFormatCells"/>
          <xsl:with-param name="def_val">true</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_inv_with_def">
          <!-- Inversed -->
          <xsl:with-param name="name">formatColumns</xsl:with-param>
          <xsl:with-param name="val" select="FEAT/FeatHdr/EnhancedProtection/@iprotFormatColumns"/>
          <xsl:with-param name="def_val">true</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_inv_with_def">
          <!-- Inversed -->
          <xsl:with-param name="name">formatRows</xsl:with-param>
          <xsl:with-param name="val" select="FEAT/FeatHdr/EnhancedProtection/@iprotFormatRows"/>
          <xsl:with-param name="def_val">true</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_inv_with_def">
          <!-- Inversed -->
          <xsl:with-param name="name">insertColumns</xsl:with-param>
          <xsl:with-param name="val" select="FEAT/FeatHdr/EnhancedProtection/@iprotInsertColumns"/>
          <xsl:with-param name="def_val">true</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_inv_with_def">
          <!-- Inversed -->
          <xsl:with-param name="name">insertRows</xsl:with-param>
          <xsl:with-param name="val" select="FEAT/FeatHdr/EnhancedProtection/@iprotInsertRows"/>
          <xsl:with-param name="def_val">true</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_inv_with_def">
          <!-- Inversed -->
          <xsl:with-param name="name">insertHyperlinks</xsl:with-param>
          <xsl:with-param name="val" select="FEAT/FeatHdr/EnhancedProtection/@iprotInsertHyperlinks"/>
          <xsl:with-param name="def_val">true</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_inv_with_def">
          <!-- Inversed -->
          <xsl:with-param name="name">deleteColumns</xsl:with-param>
          <xsl:with-param name="val" select="FEAT/FeatHdr/EnhancedProtection/@iprotDeleteColumns"/>
          <xsl:with-param name="def_val">true</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_inv_with_def">
          <!-- Inversed -->
          <xsl:with-param name="name">deleteRows</xsl:with-param>
          <xsl:with-param name="val" select="FEAT/FeatHdr/EnhancedProtection/@iprotDeleteRows"/>
          <xsl:with-param name="def_val">true</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_inv_with_def">
          <!-- Inversed -->
          <xsl:with-param name="name">selectLockedCells</xsl:with-param>
          <xsl:with-param name="val" select="FEAT/FeatHdr/EnhancedProtection/@iprotSelLockedCells"/>
          <xsl:with-param name="def_val">false</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_inv_with_def">
          <!-- Inversed -->
          <xsl:with-param name="name">sort</xsl:with-param>
          <xsl:with-param name="val" select="FEAT/FeatHdr/EnhancedProtection/@iprotSort"/>
          <xsl:with-param name="def_val">true</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_inv_with_def">
          <!-- Inversed -->
          <xsl:with-param name="name">autoFilter</xsl:with-param>
          <xsl:with-param name="val" select="FEAT/FeatHdr/EnhancedProtection/@iprotAutoFilter"/>
          <xsl:with-param name="def_val">true</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_inv_with_def">
          <!-- Inversed -->
          <xsl:with-param name="name">pivotTables</xsl:with-param>
          <xsl:with-param name="val" select="FEAT/FeatHdr/EnhancedProtection/@iprotPivotTables"/>
          <xsl:with-param name="def_val">true</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="attrib_inv_with_def">
          <!-- Inversed -->
          <xsl:with-param name="name">selectUnlockedCells</xsl:with-param>
          <xsl:with-param name="val" select="FEAT/FeatHdr/EnhancedProtection/@iprotSelUnlockedCells"/>
          <xsl:with-param name="def_val">false</xsl:with-param>
        </xsl:call-template>
      </sheetProtection>
    </xsl:if>
  </xsl:template>

  <xsl:template name="protectedRanges">
    <xsl:if test="FEAT/Feat/FeatProtection">
      <protectedRanges xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:for-each select="FEAT/Feat[@isf = 2]">
          <!-- isf = 2(ISFPROTECTION) means that Feat contains FeatProtection -->
          <protectedRange>
            <xsl:call-template name="attrib">
              <xsl:with-param name="name">password</xsl:with-param>
              <xsl:with-param name="val" select="FeatProtection/@wPassword"/>
            </xsl:call-template>
            <xsl:call-template name="attrib">
              <xsl:with-param name="name">sqref</xsl:with-param>
              <xsl:with-param name="val" select="@sqref"/>
            </xsl:call-template>
            <xsl:call-template name="attrib">
              <xsl:with-param name="name">name</xsl:with-param>
              <xsl:with-param name="val" select="FeatProtection/@stTitle"/>
            </xsl:call-template>
            <!-- algorithmName, hashValue, saltValue, spinCount have no corresponding tags in XLS -->
            <!-- TODO: securityDescriptor implementation -->
          </protectedRange>
        </xsl:for-each>
      </protectedRanges>
    </xsl:if>
  </xsl:template>

  <xsl:template name="scenarios">
    <xsl:if test="SCENARIOS">
      <!-- TODO: check and finish -->
      <scenarios xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:call-template name="attrib">
          <xsl:with-param name="name">current</xsl:with-param>
          <xsl:with-param name="val" select="SCENARIOS/ScenMan/@isctCur"/>
        </xsl:call-template>
        <xsl:call-template name="attrib">
          <xsl:with-param name="name">show</xsl:with-param>
          <xsl:with-param name="val" select="SCENARIOS/ScenMan/@isctShown"/>
        </xsl:call-template>
        <xsl:call-template name="attrib">
          <xsl:with-param name="name">sqref</xsl:with-param>
          <xsl:with-param name="val" select="SCENARIOS/ScenMan/@rgref"/>
        </xsl:call-template>
      </scenarios>
    </xsl:if>
  </xsl:template>

  <xsl:template name="replace">
    <xsl:param name="input"/>
    <xsl:param name="from"/>
    <xsl:param name="to"/>

    <xsl:choose>
      <xsl:when test="contains($input, $from)">
        <!--   вывод подстроки предшествующей образцу  + вывод строки замены -->
        <xsl:value-of select="substring-before($input, $from)"/>
        <xsl:value-of select="$to"/>


        <!--   вход в итерацию -->
        <xsl:call-template name="replace">
          <!--  в качестве входного параметра задается подстрока после образца замены  -->
          <xsl:with-param name="input" select="substring-after($input, $from)"/>
          <xsl:with-param name="from" select="$from"/>
          <xsl:with-param name="to" select="$to"/>
        </xsl:call-template>

      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$input"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="autoFilter">
    <!--<xsl:if test="SORTANDFILTER | AUTOFILTER">
       TODO: check and finish 
      <autoFilter xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
      </autoFilter>
    </xsl:if>-->

    <xsl:if test="SORTANDFILTER/AUTOFILTER">

      <xsl:variable name="worksheetIndex" select="@index"/>
      <xsl:variable name="autoFilterRef" select="substring-after(/root/WorkbookStreamObject/GlobalsSubstream/LBL[position() = $worksheetIndex + 1]/Lbl[@Name = '_xlnm._FilterDatabase']/NameParsedFormula/@assembled_formula, '!')"/>

      <autoFilter xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:attribute name="ref">
          <xsl:call-template name="replace">
            <xsl:with-param name="input" select="$autoFilterRef"/>
            <xsl:with-param name="from" select="'$'"/>
            <xsl:with-param name="to" select="''"/>
          </xsl:call-template>
        </xsl:attribute>

        <xsl:for-each select="SORTANDFILTER/AUTOFILTER/AutoFilter">
          <xsl:variable name="iEntryAutoFilter" select="@iEntry"/>
          <xsl:variable name="fSimple1" select="@fSimple1"/>
          <xsl:variable name="fSimple2" select="@fSimple2"/>

          <filterColumn colId="{$iEntryAutoFilter}">
            <filters>
              <xsl:choose>

                <xsl:when test="$fSimple1 = 1">
                  <filter>
                    <xsl:attribute name="val">
                      <xsl:value-of select="@str1"/>
                    </xsl:attribute>
                  </filter>
                  <xsl:if test="$fSimple2 = 1">
                    <filter>
                      <xsl:attribute name="val">
                        <xsl:value-of select="@str2"/>
                      </xsl:attribute>
                    </filter>
                  </xsl:if>
                </xsl:when>

                <xsl:otherwise>
                  <xsl:for-each select="../AutoFilter12[@iEntry = $iEntryAutoFilter]/AF12Criteria">
                    <filter>
                      <xsl:attribute name="val">
                        <xsl:value-of select="@str"/>
                      </xsl:attribute>
                    </filter>
                  </xsl:for-each>
                </xsl:otherwise>

              </xsl:choose>

            </filters>
          </filterColumn>

        </xsl:for-each>
      </autoFilter>
    </xsl:if>

  </xsl:template>

  <xsl:template name="sortState">
    <!-- Only data introduced in Excel2007 will be converted -->
    <xsl:variable name="SortData12" select="SORTANDFILTER/SORTDATA12/SortData"/>
    <xsl:if test="$SortData12">
      <sortState xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:if test="$SortData12/@fCol = 'true'">
          <xsl:attribute name="columnSort">true</xsl:attribute>
        </xsl:if>
        <xsl:if test="$SortData12/@fCaseSensitive = 'true'">
          <xsl:attribute name="caseSensitive">true</xsl:attribute>
        </xsl:if>
        <xsl:if test="$SortData12/@fAltMethod = 'true'">
          <xsl:attribute name="sortMethod">stroke</xsl:attribute>
          <!-- TODO: find out how to distinguish 'stroke order' and 'Mandarin phonetic symbols'-->
        </xsl:if>
        <xsl:attribute name="ref">
          <xsl:value-of select="$SortData12/@rfx"/>
        </xsl:attribute>
        <xsl:for-each select="$SortData12/SortCond12">
          <sortCondition>
            <xsl:if test="@fSortDes = 'true'">
              <xsl:attribute name="descending">true</xsl:attribute>
            </xsl:if>
            <xsl:if test="@sortOn != 0">
              <xsl:choose>
                <xsl:when test="@sortOn = 0">
                  <xsl:attribute name="sortBy">value</xsl:attribute>
                </xsl:when>
                <xsl:when test="@sortOn = 1">
                  <xsl:attribute name="sortBy">cellColor</xsl:attribute>
                </xsl:when>
                <xsl:when test="@sortOn = 2">
                  <xsl:attribute name="sortBy">fontColor</xsl:attribute>
                </xsl:when>
                <xsl:when test="@sortOn = 3">
                  <xsl:attribute name="sortBy">icon</xsl:attribute>
                </xsl:when>
              </xsl:choose>
            </xsl:if>
            <xsl:attribute name="ref">
              <xsl:value-of select="@rfx"/>
            </xsl:attribute>
            <xsl:if test="@stSslist != ''">
              <xsl:attribute name="customList">
                <xsl:value-of select="@stSslist"/>
              </xsl:attribute>
            </xsl:if>
            <xsl:if test="@dxfId">
              <xsl:attribute name="dxfId">
                <xsl:value-of select="@dxfId"/>
              </xsl:attribute>
            </xsl:if>
            <xsl:if test="@iIconSet and @iIconSet != 0 and @iIconSet != -1">
              <xsl:attribute name="iconSet">
                <xsl:choose>
                  <xsl:when test="@iIconSet = 0">3Arrows</xsl:when>
                  <xsl:when test="@iIconSet = 1">3ArrowsGray</xsl:when>
                  <xsl:when test="@iIconSet = 2">3Flags</xsl:when>
                  <xsl:when test="@iIconSet = 3">3TrafficLights1</xsl:when>
                  <xsl:when test="@iIconSet = 4">3TrafficLights2</xsl:when>
                  <xsl:when test="@iIconSet = 5">3Signs</xsl:when>
                  <xsl:when test="@iIconSet = 6">3Symbols</xsl:when>
                  <xsl:when test="@iIconSet = 7">3Symbols2</xsl:when>
                  <xsl:when test="@iIconSet = 8">4Arrows</xsl:when>
                  <xsl:when test="@iIconSet = 9">4ArrowsGray</xsl:when>
                  <xsl:when test="@iIconSet = 10">4RedToBlack</xsl:when>
                  <xsl:when test="@iIconSet = 11">4Rating</xsl:when>
                  <xsl:when test="@iIconSet = 12">4TrafficLights</xsl:when>
                  <xsl:when test="@iIconSet = 13">5Arrows</xsl:when>
                  <xsl:when test="@iIconSet = 14">5ArrowsGray</xsl:when>
                  <xsl:when test="@iIconSet = 15">5Rating</xsl:when>
                  <xsl:when test="@iIconSet = 16">5Quarters</xsl:when>
                </xsl:choose>
              </xsl:attribute>
            </xsl:if>
            <xsl:if test="@iIcon">
              <xsl:attribute name="iconId">
                <xsl:value-of select="@iIcon"/>
              </xsl:attribute>
            </xsl:if>
          </sortCondition>
        </xsl:for-each>
      </sortState>
    </xsl:if>
  </xsl:template>

  <xsl:template name="dataConsolidate">
    <!--<xsl:if test="DCON">
      TODO: check and finish 
      <dataConsolidate xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
      </dataConsolidate>
    </xsl:if>-->
  </xsl:template>

  <xsl:template name="customSheetViews">
    <xsl:if test="CUSTOMVIEW">
      <customSheetViews xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:for-each select="CUSTOMVIEW">
          <customSheetView guid="{UserSViewBegin/@guid}">
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">scale</xsl:with-param>
              <xsl:with-param name="val" select="UserSViewBegin/@wScale"/>
              <xsl:with-param name="def_val">100</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">colorId</xsl:with-param>
              <xsl:with-param name="val" select="UserSViewBegin/@icvHdr"/>
              <xsl:with-param name="def_val">64</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">showPageBreaks</xsl:with-param>
              <xsl:with-param name="val" select="UserSViewBegin/@fShowBrks"/>
              <xsl:with-param name="def_val">false</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">showFormulas</xsl:with-param>
              <xsl:with-param name="val" select="UserSViewBegin/@fDspFmlaSv"/>
              <xsl:with-param name="def_val">false</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">showGridLines</xsl:with-param>
              <xsl:with-param name="val" select="UserSViewBegin/@fDspGridSv"/>
              <xsl:with-param name="def_val">true</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">showRowCol</xsl:with-param>
              <xsl:with-param name="val" select="UserSViewBegin/@fDspRwColSv"/>
              <xsl:with-param name="def_val">true</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">outlineSymbols</xsl:with-param>
              <xsl:with-param name="val" select="UserSViewBegin/@fDspGutsSv"/>
              <xsl:with-param name="def_val">true</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">zeroValues</xsl:with-param>
              <xsl:with-param name="val" select="UserSViewBegin/@fDspZerosSv"/>
              <xsl:with-param name="def_val">true</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">fitToPage</xsl:with-param>
              <xsl:with-param name="val" select="UserSViewBegin/@fFitToPage"/>
              <xsl:with-param name="def_val">false</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">printArea</xsl:with-param>
              <xsl:with-param name="val" select="UserSViewBegin/@fPrintArea"/>
              <xsl:with-param name="def_val">false</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">filter</xsl:with-param>
              <xsl:with-param name="val" select="UserSViewBegin/@fFilterMode"/>
              <xsl:with-param name="def_val">false</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">showAutoFilter</xsl:with-param>
              <xsl:with-param name="val" select="UserSViewBegin/@fEzFilter"/>
              <xsl:with-param name="def_val">false</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">hiddenRows</xsl:with-param>
              <xsl:with-param name="val" select="UserSViewBegin/@fHiddenRw"/>
              <xsl:with-param name="def_val">false</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">hiddenColumns</xsl:with-param>
              <xsl:with-param name="val" select="UserSViewBegin/@fHiddenCol"/>
              <xsl:with-param name="def_val">false</xsl:with-param>
            </xsl:call-template>
            <!-- The following left default for a while. TODO: find out
                <xsl:call-template name="attrib_with_def">
                  <xsl:with-param name="name">state</xsl:with-param>
                  <xsl:with-param name="val" select="UserSViewBegin/@??????"/>
                  <xsl:with-param name="def_val">visible</xsl:with-param>
                </xsl:call-template>
                -->
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">filterUnique</xsl:with-param>
              <xsl:with-param name="val" select="UserSViewBegin/@fFilterUnique"/>
              <xsl:with-param name="def_val">false</xsl:with-param>
            </xsl:call-template>
            <xsl:choose>
              <xsl:when test="UserSViewBegin/@fSheetLayoutView = 'true'">
                <xsl:attribute name="view">pageBreakPreview</xsl:attribute>
              </xsl:when>
              <xsl:when test="UserSViewBegin/@fPageLayoutView = 'true'">
                <xsl:attribute name="view">pageLayout</xsl:attribute>
              </xsl:when>
              <!-- The default is 'normal' and ommited -->
            </xsl:choose>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">showRuler</xsl:with-param>
              <xsl:with-param name="val" select="UserSViewBegin/@fRuler"/>
              <xsl:with-param name="def_val">true</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">topLeftCell</xsl:with-param>
              <xsl:with-param name="val" select="UserSViewBegin/@ref8TopLeft"/>
              <xsl:with-param name="def_val">A1</xsl:with-param>
            </xsl:call-template>

            <pane>
              <xsl:call-template name="attrib_with_def">
                <xsl:with-param name="name">xSplit</xsl:with-param>
                <xsl:with-param name="val" select="UserSViewBegin/@operNumX"/>
                <xsl:with-param name="def_val">0</xsl:with-param>
              </xsl:call-template>
              <xsl:call-template name="attrib_with_def">
                <xsl:with-param name="name">ySplit</xsl:with-param>
                <xsl:with-param name="val" select="UserSViewBegin/@operNumY"/>
                <xsl:with-param name="def_val">0</xsl:with-param>
              </xsl:call-template>
              <xsl:call-template name="attrib_with_def">
                <xsl:with-param name="name">topLeftCell</xsl:with-param>
                <xsl:with-param name="val" select="UserSViewBegin/@pane_top_left_cell"/>
                <xsl:with-param name="def_val">A1</xsl:with-param>
              </xsl:call-template>
              <xsl:call-template name="attrib_with_def">
                <xsl:with-param name="name">activePane</xsl:with-param>
                <xsl:with-param name="val" select="UserSViewBegin/@pnnSel"/>
                <xsl:with-param name="def_val">topLeft</xsl:with-param>
              </xsl:call-template>
              <xsl:choose>
                <xsl:when test="UserSViewBegin/@fFrozen = 'true' and UserSViewBegin/@fFrozenNoSplit = 'true'">
                  <xsl:attribute name="state">frozen</xsl:attribute>
                </xsl:when>
                <xsl:when test="UserSViewBegin/@fFrozen = 'true' and UserSViewBegin/@fFrozenNoSplit = 'false'">
                  <xsl:attribute name="state">frozenSplit</xsl:attribute>
                </xsl:when>
                <!-- The default is 'split' and ommited -->
              </xsl:choose>
            </pane>

            <xsl:apply-templates select="Selection" mode="view"/>
            <xsl:apply-templates select="HorizontalPageBreaks" mode="view"/>
            <xsl:apply-templates select="VerticalPageBreaks" mode="view"/>

            <xsl:call-template name="pageMargins"/>
            <xsl:call-template name="printOptions"/>
            <xsl:call-template name="pageSetup"/>
            <xsl:call-template name="headerFooter"/>

            <xsl:if test="AUTOFILTER">
              <xsl:call-template name="autoFilter"/>
            </xsl:if>

          </customSheetView>
        </xsl:for-each>
      </customSheetViews>
    </xsl:if>
  </xsl:template>

  <xsl:template name="mergeCells">
    <xsl:if test="MergeCells and (MergeCells/@cmcs &gt; 0)">
      <mergeCells count="{MergeCells/@cmcs}" xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:for-each select="MergeCells/Ref8">
          <mergeCell ref="{@name}"/>
        </xsl:for-each>
      </mergeCells>
    </xsl:if>
  </xsl:template>

  <xsl:template name="phoneticPr">
    <xsl:if test="PHONETICINFO">
      <!-- TODO: check and finish -->
      <phoneticPr fontId="{PHONETICINFO/PhoneticInfo/Phs/@ifnt}" xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:variable name="type" select="PHONETICINFO/PhoneticInfo/Phs/@phType"/>
        <xsl:variable name="type_str">
          <xsl:choose>
            <xsl:when test="$type = 0">halfwidthKatakana</xsl:when>
            <xsl:when test="$type = 1">fullwidthKatakana</xsl:when>
            <xsl:when test="$type = 2">Hiragana</xsl:when>
            <xsl:when test="$type = 3">noConversion</xsl:when>
          </xsl:choose>
        </xsl:variable>
        <xsl:call-template name="attrib_with_def">
          <xsl:with-param name="name">type</xsl:with-param>
          <xsl:with-param name="val" select="$type_str"/>
          <xsl:with-param name="def_val">fullwidthKatakana</xsl:with-param>
        </xsl:call-template>
        <xsl:variable name="align" select="PHONETICINFO/PhoneticInfo/Phs/@alcH"/>
        <xsl:variable name="align_str">
          <xsl:choose>
            <xsl:when test="$align = 0">noControl</xsl:when>
            <xsl:when test="$align = 1">left</xsl:when>
            <xsl:when test="$align = 2">center</xsl:when>
            <xsl:when test="$align = 3">distributed</xsl:when>
          </xsl:choose>
        </xsl:variable>
        <xsl:call-template name="attrib_with_def">
          <xsl:with-param name="name">alignment</xsl:with-param>
          <xsl:with-param name="val" select="$align_str"/>
          <xsl:with-param name="def_val">left</xsl:with-param>
        </xsl:call-template>
      </phoneticPr>
    </xsl:if>
  </xsl:template>

  <xsl:template name="conditionalFormatting">
    <xsl:for-each select="CONDFMTS/CONDFMT | CONDFMTS/CONDFMT12">
      <conditionalFormatting xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <!-- TODO: 'pivot' attribute skipped for a while -->
        <xsl:call-template name="attrib">
          <xsl:with-param name="name">sqref</xsl:with-param>
          <xsl:with-param name="val" select="CondFmt/SqRefU/@sqref | CondFmt12/CondFmtStructure/SqRefU/@sqref"/>
        </xsl:call-template>
        <xsl:variable name="condition_pos" select="position()"/>
        <xsl:variable name="condition_id" select="CondFmt/@nID | CondFmt12/CondFmtStructure/@nID"/>
        <xsl:for-each select="CF | CF12">
          <xsl:variable name="cf_pos" select="position()"/>
          <xsl:variable name="cf_ex" select="../../CFEx[@nID = $condition_id]/CFExNonCF12[@icf + 1 = $cf_pos]"/>
          <cfRule>
            <xsl:attribute name="type">
              <xsl:choose>
                <xsl:when test="@ct = 1">cellIs</xsl:when>
                <xsl:when test="@ct = 2">expression</xsl:when>
                <xsl:when test="@ct = 6">iconSet</xsl:when>
              </xsl:choose>
            </xsl:attribute>
            <xsl:attribute name="dxfId">
              <!-- TODO: It seems that we need to generate DXFs -->
              <xsl:value-of select="count(../../../../GlobalsSubstream/FORMATTING/DXF) + 
                            count(preceding::CF) + count(preceding::CF12)"/>
            </xsl:attribute>
            <xsl:call-template name="attrib">
              <xsl:with-param name="name">priority</xsl:with-param>
              <xsl:with-param name="val">
                <xsl:value-of select="count(../../CONDFMT[position() &lt; $condition_pos]/CF) +
                                      count(../../CONDFMT12[position() &lt; $condition_pos]/CF12) + position()"/>
              </xsl:with-param>
            </xsl:call-template>
            <xsl:choose>
              <xsl:when test="$cf_ex and $cf_ex/@fStopIfTrue = 'false' or @fStopIfTrue = 'false'"/>
              <xsl:otherwise>
                <xsl:attribute name="stopIfTrue">true</xsl:attribute>
              </xsl:otherwise>
            </xsl:choose>
            <xsl:if test="@ct = 1">
              <xsl:attribute name="operator">
                <xsl:choose>
                  <xsl:when test="@cp = 1">between</xsl:when>
                  <xsl:when test="@cp = 2">notBetween</xsl:when>
                  <xsl:when test="@cp = 3">equal</xsl:when>
                  <xsl:when test="@cp = 4">notEqual</xsl:when>
                  <xsl:when test="@cp = 5">greaterThan</xsl:when>
                  <xsl:when test="@cp = 6">lessThan</xsl:when>
                  <xsl:when test="@cp = 7">greaterThanOrEqual</xsl:when>
                  <xsl:when test="@cp = 8">lessThanOrEqual</xsl:when>
                </xsl:choose>
              </xsl:attribute>
            </xsl:if>
            <xsl:for-each select="CFParsedFormulaNoCCE[@assembled_formula != '']">
              <formula>
                <xsl:value-of select="@assembled_formula"/>
              </formula>
            </xsl:for-each>
          </cfRule>
        </xsl:for-each>
      </conditionalFormatting>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="dataValidations">
    <!--<xsl:if test="DVAL">
       TODO: check and finish 
      <dataValidations xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
      </dataValidations>
    </xsl:if>-->
  </xsl:template>

  <xsl:template name="hyperlinks">
    <xsl:if test="HLINK">
      <hyperlinks xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:for-each select="HLINK">
          <hyperlink ref="{HLink/Ref8U/@name}" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships">
            <xsl:if test="HLink/HyperlinkObject/@hlstmfHasMoniker = 'true'">
              <xsl:attribute name="r:id">
                <xsl:text>rId</xsl:text>
                <xsl:call-template name="generate_worksheet_entity_id"/>
              </xsl:attribute>
            </xsl:if>
            <xsl:if test="HLink/HyperlinkObject/@hlstmfHasLocationStr = 'true'">
              <xsl:attribute name="location">
                <xsl:value-of select="HLink/HyperlinkObject/@location"/>
              </xsl:attribute>
            </xsl:if>
            <xsl:if test="HLinkTooltip">
              <xsl:attribute name="tooltip">
                <xsl:value-of select="HLinkTooltip/@wzTooltip"/>
              </xsl:attribute>
            </xsl:if>
            <xsl:if test="HLink/HyperlinkObject/@hlstmfHasDisplayName = 'true'">
              <xsl:attribute name="display">
                <xsl:value-of select="HLink/HyperlinkObject/@displayName"/>
              </xsl:attribute>
            </xsl:if>
          </hyperlink>
        </xsl:for-each>
      </hyperlinks>
    </xsl:if>
  </xsl:template>

  <xsl:key name="get_shfmla_by_ref" use="concat(../../@index, '=', @ref)" match="/root/WorkbookStreamObject/WorksheetSubstream/SHFMLA_SET/shfmla"/>

  <xsl:template name="get_shfmla_si">
    <xsl:param name="ref"/>
    <xsl:param name="formula"/>
    <xsl:variable name="ws_index" select="$formula/../../../../@index"/>
    <xsl:value-of select="key('get_shfmla_by_ref', concat($ws_index, '=', $ref))/@si"/>
  </xsl:template>

  <xsl:template name="f">
    <xsl:param name="formula"/>
    <!-- Must point to FORMULA tag -->
    <xsl:if test="$formula/Formula/@fShrFmla = 'true' or $formula/Array or 
            $formula/Table or $formula/Formula/@fAlwaysCalc = 'true' or 
            $formula/Formula/CellParsedFormula/@assembled_formula != ''">
      <!-- Avoid empty <f></f> -->
      <f xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <!-- 'bx' attribute is skipped since no corresponding setting found in XSL TODO: find this out-->
        <xsl:choose>
          <xsl:when test="$formula/Formula/@fShrFmla = 'true' and $formula/Formula/CellParsedFormula/Rgce/PtgExp">
            <!-- Shared formulas -->
            <xsl:attribute name="t">shared</xsl:attribute>
            <xsl:choose>
              <xsl:when test="$formula/ShrFmla">
                <!--Formula itself-->
                <xsl:attribute name="ref">
                  <xsl:value-of select="$formula/ShrFmla/RefU/@name"/>
                </xsl:attribute>
                <xsl:if test="$formula/Formula/@fAlwaysCalc = 'true'">
                  <xsl:attribute name="ca">true</xsl:attribute>
                </xsl:if>
                <xsl:attribute name="si">
                  <!-- 0-based -->
                  <xsl:call-template name="get_shfmla_si">
                    <xsl:with-param name="formula" select="$formula"/>
                    <xsl:with-param name="ref" select="$formula/Formula/Cell/@ref"/>
                  </xsl:call-template>
                </xsl:attribute>
              </xsl:when>
              <xsl:otherwise>
                <!-- Formula reference-->
                <xsl:if test="$formula/Formula/@fAlwaysCalc = 'true'">
                  <xsl:attribute name="ca">true</xsl:attribute>
                </xsl:if>
                <xsl:attribute name="si">
                  <xsl:call-template name="get_shfmla_si">
                    <xsl:with-param name="formula" select="$formula"/>
                    <xsl:with-param name="ref" select="$formula/Formula/CellParsedFormula/Rgce/PtgExp/@ref"/>
                  </xsl:call-template>
                </xsl:attribute>
              </xsl:otherwise>
            </xsl:choose>
            <xsl:if test="$formula/ShrFmla/SharedParsedFormula/@assembled_formula != ''">
              <xsl:value-of select="$formula/ShrFmla/SharedParsedFormula/@assembled_formula"/>
            </xsl:if>
          </xsl:when>

          <xsl:when test="$formula/Array">
            <!-- Array formulas -->
            <xsl:attribute name="t">array</xsl:attribute>
            <xsl:if test="$formula/Array/@fAlwaysCalc = 'true'">
              <xsl:attribute name="aca">true</xsl:attribute>
            </xsl:if>
            <xsl:attribute name="ref">
              <xsl:value-of select="$formula/Array/Ref/@name"/>
            </xsl:attribute>
            <xsl:if test="$formula/Formula/@fAlwaysCalc = 'true'">
              <xsl:attribute name="ca">true</xsl:attribute>
            </xsl:if>
            <xsl:if test="$formula/Array/ArrayParsedFormula/@assembled_formula != ''">
              <xsl:value-of select="$formula/Array/ArrayParsedFormula/@assembled_formula"/>
            </xsl:if>
          </xsl:when>

          <xsl:when test="$formula/Table">
            <!-- TableData formulas -->
            <xsl:attribute name="t">dataTable</xsl:attribute>
            <xsl:attribute name="ref">
              <xsl:value-of select="$formula/Table/Ref/@name"/>
            </xsl:attribute>
            <xsl:if test="$formula/Table/@fTbl2 = 'true'">
              <xsl:attribute name="dt2D">true</xsl:attribute>
            </xsl:if>
            <xsl:if test="$formula/Table/@fRw = 'true'">
              <xsl:attribute name="dtr">true</xsl:attribute>
            </xsl:if>
            <xsl:if test="$formula/Table/@fDeleted1 = 'true'">
              <xsl:attribute name="del1">true</xsl:attribute>
            </xsl:if>
            <xsl:if test="$formula/Table/@fDeleted2 = 'true'">
              <xsl:attribute name="del2">true</xsl:attribute>
            </xsl:if>
            <xsl:if test="$formula/Table/@r1">
              <xsl:attribute name="r1">
                <xsl:value-of select="$formula/Table/@r1"/>
              </xsl:attribute>
            </xsl:if>
            <xsl:if test="$formula/Table/@r2 and $formula/Table/@fTbl2 = 'true'">
              <xsl:attribute name="r2">
                <xsl:value-of select="$formula/Table/@r2"/>
              </xsl:attribute>
            </xsl:if>

            <xsl:if test="$formula/Table/@fAlwaysCalc = 'true'">
              <xsl:attribute name="ca">true</xsl:attribute>
            </xsl:if>
          </xsl:when>

          <xsl:otherwise>
            <!-- Type = 'normal' -->
            <xsl:if test="$formula/Formula/@fAlwaysCalc = 'true'">
              <xsl:attribute name="ca">true</xsl:attribute>
            </xsl:if>
            <xsl:if test="$formula/Formula/CellParsedFormula/@assembled_formula != ''">
              <xsl:value-of select="$formula/Formula/CellParsedFormula/@assembled_formula"/>
            </xsl:if>
          </xsl:otherwise>
        </xsl:choose>
      </f>
    </xsl:if>
  </xsl:template>

  <xsl:template name="cellWatches">
    <xsl:if test="CellWatch">
      <cellWatches xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:for-each select="CellWatch">
          <cellWatch r="{FrtRefHeaderU/@ref8}"/>
        </xsl:for-each>
      </cellWatches>
    </xsl:if>
  </xsl:template>

  <xsl:key name="get_cell_or_style_xf" use="@xf_current_id" match="/root/WorkbookStreamObject/GlobalsSubstream/FORMATTING/XFS/XF/*"/>
  <xsl:key name="get_cell_xf" use="@xf_current_id" match="/root/WorkbookStreamObject/GlobalsSubstream/FORMATTING/XFS/XF/CellXF"/>
  <xsl:key name="get_style_xf" use="@xf_current_id" match="/root/WorkbookStreamObject/GlobalsSubstream/FORMATTING/XFS/XF/StyleXF"/>

  <xsl:template name="sheetData">
    <sheetData xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
      <xsl:for-each select="CELLTABLE/CELL_GROUP/Row">
        <xsl:variable name="rw" select="@rw"/>
        <row r="{$rw + 1}">
          <xsl:if test="@fGhostDirty = 'true'">
            <xsl:variable name="ixfe_val" select="@ixfe_val"/>
            <xsl:if test="$ixfe_val != $ixfe0">
              <xsl:attribute name="s">
                <xsl:value-of select="key('get_cell_xf', $ixfe_val)/@cell_xf_current_id"/>
              </xsl:attribute>
            </xsl:if>
            <xsl:attribute name="customFormat">true</xsl:attribute>
          </xsl:if>
          <xsl:if test="@miyRw != ../../../GLOBALS/DefaultRowHeight/@miyRw">
            <xsl:attribute name="ht">
              <xsl:value-of select="@miyRw div 20"/>
            </xsl:attribute>
          </xsl:if>
          <xsl:if test="@fDyZero = 'true'">
            <xsl:attribute name="hidden">true</xsl:attribute>
          </xsl:if>
          <xsl:if test="@fUnsynced = 'true'">
            <xsl:attribute name="customHeight">true</xsl:attribute>
          </xsl:if>
          <xsl:if test="@iOutLevel != 0">
            <xsl:attribute name="outlineLevel">
              <xsl:value-of select="@iOutLevel"/>
            </xsl:attribute>
          </xsl:if>
          <xsl:if test="@fCollapsed = 'true'">
            <xsl:attribute name="collapsed">true</xsl:attribute>
          </xsl:if>
          <xsl:if test="@fExAsc = 'true'">
            <xsl:attribute name="thickTop">true</xsl:attribute>
          </xsl:if>
          <xsl:if test="@fExDes = 'true'">
            <xsl:attribute name="thickBot">true</xsl:attribute>
          </xsl:if>
          <xsl:if test="@fPhonetic = 'true'">
            <xsl:attribute name="ph">true</xsl:attribute>
          </xsl:if>

          <xsl:apply-templates select="../CELL[@rw = $rw]/*" mode="cell_table"/>
        </row>
      </xsl:for-each>
    </sheetData>
  </xsl:template>


  <xsl:template match="LabelSst" mode="cell_table">
    <xsl:call-template name="Cell">
      <xsl:with-param name="Cell_tag" select="Cell"/>
      <xsl:with-param name="t">s</xsl:with-param>
      <xsl:with-param name="v" select="@isst"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="Number" mode="cell_table">
    <xsl:call-template name="Cell">
      <xsl:with-param name="Cell_tag" select="Cell"/>
      <xsl:with-param name="v" select="@num"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="BoolErr" mode="cell_table">
    <xsl:call-template name="Cell">
      <xsl:with-param name="Cell_tag" select="Cell"/>
      <xsl:with-param name="t">
        <xsl:choose>
          <xsl:when test="Bes/@fError = 'true' or Bes/@bBoolErr > 1">e</xsl:when>
          <xsl:when test="Bes/@fError = 'false' and Bes/@bBoolErr &lt; 2">b</xsl:when>
        </xsl:choose>
      </xsl:with-param>
      <xsl:with-param name="v" select="Bes/@value"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="RK" mode="cell_table">
    <xsl:call-template name="Cell">
      <xsl:with-param name="Cell_tag" select="Cell"/>
      <xsl:with-param name="v" select="RkRec/@RK"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="MulRk" mode="cell_table">
    <xsl:for-each select="Cell">
      <xsl:variable name="cell_pos" select="position()"/>
      <xsl:call-template name="Cell">
        <xsl:with-param name="Cell_tag" select="."/>
        <xsl:with-param name="v" select="../RkRec[position() = $cell_pos]/@RK"/>
      </xsl:call-template>
    </xsl:for-each>
  </xsl:template>

  <xsl:template match="FORMULA" mode="cell_table">
    <xsl:call-template name="Cell">
      <xsl:with-param name="Cell_tag" select="Formula/Cell"/>
      <xsl:with-param name="t">
        <xsl:choose>
          <xsl:when test="Formula/@val_type = '2'">e</xsl:when>
          <xsl:when test="Formula/@val_type = '1'">b</xsl:when>
          <xsl:when test="Formula/@val_type = '0'">str</xsl:when>
        </xsl:choose>
      </xsl:with-param>
      <xsl:with-param name="f" select="."/>
      <xsl:with-param name="v">
        <xsl:choose>
          <xsl:when test="Formula/@val_type = '0'">
            <xsl:value-of select="String/@string"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="Formula/@val"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="Blank" mode="cell_table">
    <xsl:call-template name="Cell">
      <xsl:with-param name="Cell_tag" select="Cell"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="MulBlank" mode="cell_table">

    <xsl:choose>
      <xsl:when test="@common_ixfe != ''">
        <xsl:call-template name="MulBlank_gen_equal">
          <xsl:with-param name="colFirst" select="@colFirst"/>
          <!-- pass 0-based -->
          <xsl:with-param name="colLast" select="@colLast"/>
          <!-- pass 0-based -->
          <xsl:with-param name="rw" select="../@rw + 1"/>
          <!-- pass 1-based -->
          <xsl:with-param name="ixfe" select="@common_ixfe"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="IXFCell">

        <xsl:variable name="colFirst" select="@colFirst"/>
        <xsl:variable name="rwref" select="../@rw + 1"/>
        <xsl:for-each select="IXFCell">

          <xsl:call-template name="Cell_no_tag">
            <xsl:with-param name="ref" select="concat(key('col2ref', $colFirst + position() - 1)/@ref, $rwref)"/>
            <xsl:with-param name="ixfe" select="."/>
          </xsl:call-template>

        </xsl:for-each>

      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="MulBlank_gen_equal">
    <xsl:param name="colFirst"/>
    <!-- $col is 0-based-->
    <xsl:param name="colLast"/>
    <!-- $colLast is 0-based-->
    <xsl:param name="rw"/>
    <!-- $rw is 1-based-->
    <xsl:param name="ixfe"/>

    <xsl:for-each select="/root/Helpers/col_names/col_name[@index >= $colFirst and @index &lt;= $colLast]">
      <xsl:call-template name="Cell_no_tag">
        <xsl:with-param name="ref" select="concat(@ref, $rw)"/>
        <xsl:with-param name="ixfe" select="$ixfe"/>
      </xsl:call-template>
    </xsl:for-each>

  </xsl:template>

  <xsl:template name="Cell">
    <xsl:param name="Cell_tag"/>
    <xsl:param name="t"/>
    <xsl:param name="f"/>
    <!-- points to FORMULA tag-->
    <xsl:param name="v"/>

    <xsl:call-template name="Cell_no_tag">
      <xsl:with-param name="ref" select="$Cell_tag/@ref"/>
      <xsl:with-param name="ixfe" select="$Cell_tag/@ixfe"/>
      <xsl:with-param name="t" select="$t"/>
      <xsl:with-param name="f" select="$f"/>
      <xsl:with-param name="v" select="$v"/>
    </xsl:call-template>

  </xsl:template>

  <xsl:variable name="ixfe0" select="/root/WorkbookStreamObject/GlobalsSubstream/FORMATTING/XFS/XF/CellXF[@cell_xf_current_id = 0]/@xf_current_id"/>

  <xsl:template name="Cell_no_tag">
    <xsl:param name="ref"/>
    <xsl:param name="ixfe"/>
    <xsl:param name="t"/>
    <xsl:param name="f"/>
    <!-- points to FORMULA tag-->
    <xsl:param name="v"/>

    <c r="{$ref}" xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
      <xsl:if test="$ixfe != $ixfe0 and $ixfe != 0">
        <xsl:attribute name="s">
          <xsl:value-of select="key('get_cell_xf', $ixfe)/@cell_xf_current_id"/>
        </xsl:attribute>
      </xsl:if>
      <xsl:if test="$t != ''">
        <xsl:attribute name="t">
          <xsl:value-of select="$t"/>
        </xsl:attribute>
      </xsl:if>
      <!-- 'cm' and 'vm' attributes are skipped. TODO: implement metadata -->
      <!-- 'ph' attribute is skipped since no corresponding setting in XSL -->
      <!-- 'is' tag is skipped for a while TODO: find out -->
      <xsl:if test="$f">
        <xsl:call-template name="f">
          <xsl:with-param name="formula" select="$f"/>
          <!-- points to FORMULA tag-->
        </xsl:call-template>
      </xsl:if>
      <xsl:if test="$v != ''">
        <v xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
          <xsl:value-of select="$v"/>
        </v>
      </xsl:if>
    </c>

  </xsl:template>

  <!-- converting number of column to char name of column -->
  <xsl:key name="col2ref" use="@index" match="/root/Helpers/col_names/col_name"/>

  <xsl:template name="drawing">
    <!-- Only one <drawing> tag is necessary per sheet -->
    <xsl:for-each select="OBJECTS[1]">
      <drawing xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships">
        <xsl:attribute name="r:id">
          <xsl:text>rId</xsl:text>
          <xsl:call-template name="generate_worksheet_entity_id"/>
        </xsl:attribute>
      </drawing>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="legacyDrawing">
    <!-- Only one <legacyDrawing> tag is necessary per sheet -->
    
    <xsl:variable name="commentObjects" select="OBJECTS/OBJ/Obj/FtCmo[@ot = 25]"></xsl:variable>
    <xsl:if test="$commentObjects">      
      <legacyDrawing xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships">
        <xsl:attribute name="r:id">
          <xsl:text>rId</xsl:text>
          <xsl:value-of select="1 + position() + count(PAGESETUP/Pls) + count(CUSTOMVIEW/Pls) + count(OBJECTS/OBJ/Obj/FtCmo[@ot = 25]) + count(preceding-sibling::HLINK/HLink/HyperlinkObject[@hlstmfHasMoniker = 'true'])"/>
        </xsl:attribute>
      </legacyDrawing>
    </xsl:if>    
    
  </xsl:template>

  <xsl:template name="tableParts">
    
      <xsl:variable name="countTables" select="count(FEAT11/Feature11)" />      

      <xsl:if test="$countTables &gt; 0">
        <tableParts xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
          <xsl:attribute name="count">
            <xsl:value-of select="$countTables"/>
          </xsl:attribute>          

          <xsl:for-each select="FEAT11/Feature11">
            <tablePart xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships">
              <xsl:attribute name="r:id">
                <xsl:text>rId</xsl:text>
                <xsl:value-of select="1 + position() + count(../PAGESETUP/Pls) + count(../CUSTOMVIEW/Pls) + count(../OBJECTS/OBJ) + count(../preceding-sibling::HLINK/HLink/HyperlinkObject[@hlstmfHasMoniker = 'true'])"/>
              </xsl:attribute>
            </tablePart>
          </xsl:for-each>

        </tableParts>
      </xsl:if>
    

  </xsl:template>

</xsl:stylesheet>