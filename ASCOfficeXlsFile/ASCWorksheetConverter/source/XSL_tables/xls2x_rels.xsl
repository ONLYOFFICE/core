<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:xlsx="http://www.avs4you.ru/XLSXConverter/1.0/DirectoryStructure">

  <xsl:template name="root_rels">
    <!-- This section is mandatory, so no condition applyed to the existance -->
    <xlsx:dir name="_rels">
      <xlsx:file name=".rels">
        <Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships">
          <xsl:for-each select="WorkbookStreamObject">
            <Relationship Id="rId{position()}" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument" Target="xl/workbook.xml" />
          </xsl:for-each>
        </Relationships>
      </xlsx:file>
    </xlsx:dir>
  </xsl:template>

  <xsl:template name="content_types">
    <!-- This section is mandatory, so no condition applyed to the existance -->
    <xlsx:file name="[Content_Types].xml">
      <Types xmlns="http://schemas.openxmlformats.org/package/2006/content-types">
        <Override PartName="/xl/styles.xml" ContentType="application/vnd.openxmlformats-officedocument.spreadsheetml.styles+xml"/>
        <Default Extension="rels" ContentType="application/vnd.openxmlformats-package.relationships+xml"/>
        <Default Extension="xml" ContentType="application/xml"/>
        <Default Extension="vml" ContentType="application/vnd.openxmlformats-officedocument.vmlDrawing" />
        <xsl:for-each select="WorkbookStreamObject">
          <xsl:if test="WorksheetSubstream/CUSTOMVIEW/Pls or WorksheetSubstream/PAGESETUP/Pls or ChartSheetSubstream/CUSTOMVIEW/Pls or ChartSheetSubstream/PAGESETUP/Pls">
            <Default Extension="bin" ContentType="application/vnd.openxmlformats-officedocument.spreadsheetml.printerSettings"/>
          </xsl:if>
          <Override PartName="/xl/workbook.xml" ContentType="application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml" />
          <xsl:for-each select="WorksheetSubstream">
            <Override PartName="/xl/worksheets/sheet{position()}.xml" ContentType="application/vnd.openxmlformats-officedocument.spreadsheetml.worksheet+xml" />
          </xsl:for-each>
          <xsl:for-each select="ChartSheetSubstream">
            <Override PartName="/xl/chartsheets/sheet{position()}.xml" ContentType="application/vnd.openxmlformats-officedocument.spreadsheetml.chartsheet+xml" />
          </xsl:for-each>
          <xsl:for-each select="WorksheetSubstream[Note]">
            <Override PartName="/xl/comments{position()}.xml" ContentType="application/vnd.openxmlformats-officedocument.spreadsheetml.comments+xml"/>
          </xsl:for-each>
          <xsl:if test="GlobalsSubstream/SHAREDSTRINGS/SST/@cstTotal != 0">
            <Override PartName="/xl/sharedStrings.xml" ContentType="application/vnd.openxmlformats-officedocument.spreadsheetml.sharedStrings+xml"/>
          </xsl:if>
          <xsl:for-each select="WorksheetSubstream[OBJECTS] | ChartSheetSubstream">
            <Override PartName="/xl/drawings/drawing{position()}.xml" ContentType="application/vnd.openxmlformats-officedocument.drawing+xml"/>
          </xsl:for-each>
          <xsl:for-each select="WorksheetSubstream/OBJECTS/CHART | ChartSheetSubstream">
            <Override PartName="/xl/charts/chart{position()}.xml" ContentType="application/vnd.openxmlformats-officedocument.drawingml.chart+xml"/>
          </xsl:for-each>
          <xsl:for-each select="GlobalsSubstream/MSODRAWINGGROUP/MsoDrawingGroup/OfficeArtDggContainer/OfficeArtBStoreContainer/OfficeArtBStoreContainerFileBlock">
            <Override PartName="/xl/media/image{position()}.{@type}" ContentType="image/{@type}"/>
          </xsl:for-each>
          <xsl:for-each select="WorksheetSubstream/FEAT11/Feature11">
            <Override PartName="/xl/tables/table{position()}.xml" ContentType="application/vnd.openxmlformats-officedocument.spreadsheetml.table+xml"/>
          </xsl:for-each>
        </xsl:for-each>
      </Types>
    </xlsx:file>
  </xsl:template>

  <xsl:template name="xl_rels">
    <!-- This section is mandatory, so no condition applyed to the existance -->
    <xlsx:dir name="_rels">
      <xlsx:file name="workbook.xml.rels">
        <Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships">
          <xsl:variable name="ws_num" select="count(WorksheetSubstream | ChartSheetSubstream)"/>
          <xsl:for-each select="WorksheetSubstream | ChartSheetSubstream">
            <xsl:choose>
              <xsl:when test="self::WorksheetSubstream">
                <xsl:variable name="local_pos" select="count(preceding-sibling::WorksheetSubstream) + 1"/>
                <Relationship Id="rId{position()}" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/worksheet" Target="worksheets/sheet{$local_pos}.xml" />
              </xsl:when>
              <xsl:when test="self::ChartSheetSubstream">
                <xsl:variable name="local_pos" select="count(preceding-sibling::ChartSheetSubstream) + 1"/>
                <Relationship Id="rId{position()}" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/chartsheet" Target="chartsheets/sheet{$local_pos}.xml" />
              </xsl:when>
            </xsl:choose>
          </xsl:for-each>
          <Relationship Id="rId{$ws_num + 1}" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles" Target="styles.xml"/>
          <Relationship Id="rId{$ws_num + 2}" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/sharedStrings" Target="sharedStrings.xml"/>
        </Relationships>
      </xlsx:file>
    </xlsx:dir>
  </xsl:template>

  <xsl:template name="worksheets_rels">
    <!-- This section is optional -->
    <xlsx:dir name="_rels">
      <xsl:for-each select="WorksheetSubstream">
        <xsl:variable name="ws_index" select="position()"/>
        <xsl:if test="CUSTOMVIEW/Pls or PAGESETUP/Pls or HLINK or Note or OBJECTS or FEAT11">
          <xlsx:file name="sheet{position()}.xml.rels">
            <Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships">
              <xsl:for-each select="OBJECTS[1]">  <!-- Must not run more than once -->
                <Relationship>
                  <xsl:attribute name="Id">
                    <xsl:text>rId</xsl:text>
                    <xsl:call-template name="generate_worksheet_entity_id"/>
                  </xsl:attribute>
                  <xsl:attribute name="Type">http://schemas.openxmlformats.org/officeDocument/2006/relationships/drawing</xsl:attribute>
                  <xsl:attribute name="Target">../drawings/drawing<xsl:value-of select="count(../preceding-sibling::WorksheetSubstream[OBJECTS]) + count(../preceding-sibling::ChartSheetSubstream) + 1"/>.xml</xsl:attribute>
                </Relationship>
              </xsl:for-each>
              
              <xsl:for-each select="Note[1]">   <!-- Must not run more than once -->
                <Relationship>
                  <xsl:attribute name="Id">
                    <xsl:text>rId</xsl:text>
                    <xsl:call-template name="generate_worksheet_entity_id"/>
                  </xsl:attribute>
                  <xsl:attribute name="Type">http://schemas.openxmlformats.org/officeDocument/2006/relationships/comments</xsl:attribute>
                  <xsl:attribute name="Target">../comments<xsl:value-of select="count(../preceding-sibling::WorksheetSubstream[Note]) + 1"/>.xml</xsl:attribute>
                </Relationship>

                <Relationship>
                  <xsl:attribute name="Id">
                    <xsl:text>rId</xsl:text>
                    <xsl:value-of select="1 + $ws_index + count(../PAGESETUP/Pls) + count(../CUSTOMVIEW/Pls) + count(../OBJECTS/OBJ/Obj/FtCmo[@ot = 25]) + count(../preceding-sibling::HLINK/HLink/HyperlinkObject[@hlstmfHasMoniker = 'true'])"/>
                  </xsl:attribute>
                  <xsl:attribute name="Type">http://schemas.openxmlformats.org/officeDocument/2006/relationships/vmlDrawing</xsl:attribute>
                  <xsl:attribute name="Target">../drawings/vmlDrawing<xsl:value-of select="$ws_index"/>.vml</xsl:attribute>
                </Relationship>
              </xsl:for-each>
              
              <xsl:for-each select="HLINK[HLink/HyperlinkObject/@hlstmfHasMoniker = 'true']">
                <Relationship>
                  <xsl:attribute name="Id">
                    <xsl:text>rId</xsl:text>
                    <xsl:call-template name="generate_worksheet_entity_id"/>
                  </xsl:attribute>
                  <xsl:attribute name="Type">http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink</xsl:attribute>
                  <xsl:attribute name="Target">
                    <xsl:choose>
                      <xsl:when test="HLink/HyperlinkObject/FileMoniker">
                        <xsl:choose>
                          <xsl:when test="HLink/HyperlinkObject/FileMoniker/@unicodePath != ''">
                            <xsl:value-of select="HLink/HyperlinkObject/FileMoniker/@unicodePath"/>
                          </xsl:when>
                          <xsl:otherwise>
                            <xsl:value-of select="HLink/HyperlinkObject/FileMoniker/@ansiPath"/>
                          </xsl:otherwise>
                        </xsl:choose>
                      </xsl:when>
                      <xsl:when test="HLink/HyperlinkObject/URLMoniker">
                        <xsl:value-of select="HLink/HyperlinkObject/URLMoniker/@url"/>
                      </xsl:when>
                      <xsl:when test="HLink/HyperlinkObject/@hlstmfMonikerSavedAsStr = 'true'">
                        <xsl:value-of select="HLink/HyperlinkObject/@moniker"/>
                      </xsl:when>
                    </xsl:choose>
                  </xsl:attribute>
                  <xsl:attribute name="TargetMode">External</xsl:attribute>
                </Relationship>
              </xsl:for-each>
              <xsl:for-each select="CUSTOMVIEW">
                <xsl:if test="Pls">
                  <Relationship>
                    <xsl:attribute name="Id">
                      <xsl:text>rId</xsl:text>
                      <xsl:call-template name="generate_worksheet_entity_id"/>
                    </xsl:attribute>
                    <xsl:attribute name="Type">http://schemas.openxmlformats.org/officeDocument/2006/relationships/printerSettings</xsl:attribute>
                    <xsl:attribute name="Target">
                      <xsl:text>../printerSettings/printerSettings</xsl:text>
                      <xsl:call-template name="generate_printer_settings_id"/>
                      <xsl:text>.bin</xsl:text>
                    </xsl:attribute>
                  </Relationship>
                </xsl:if>
              </xsl:for-each>
              <xsl:for-each select="PAGESETUP">
                <xsl:if test="Pls">
                  <Relationship>
                    <xsl:attribute name="Id">
                      <xsl:text>rId</xsl:text>
                      <xsl:call-template name="generate_worksheet_entity_id"/>
                    </xsl:attribute>
                    <xsl:attribute name="Type">http://schemas.openxmlformats.org/officeDocument/2006/relationships/printerSettings</xsl:attribute>
                    <xsl:attribute name="Target">
                      <xsl:text>../printerSettings/printerSettings</xsl:text>
                      <xsl:call-template name="generate_printer_settings_id"/>
                      <xsl:text>.bin</xsl:text>
                    </xsl:attribute>
                  </Relationship>
                </xsl:if>
              </xsl:for-each>

              <xsl:for-each select="FEAT11/Feature11">
                <!-- Must not run more than once -->
                <Relationship>
                  <xsl:attribute name="Id">
                    <xsl:text>rId</xsl:text>
                    <xsl:value-of select="1 + position() + count(../PAGESETUP/Pls) + count(../CUSTOMVIEW/Pls) + count(../OBJECTS/OBJ) + count(../preceding-sibling::HLINK/HLink/HyperlinkObject[@hlstmfHasMoniker = 'true'])"/>
                  </xsl:attribute>
                  <xsl:attribute name="Type">http://schemas.openxmlformats.org/officeDocument/2006/relationships/table</xsl:attribute>
                  <xsl:attribute name="Target">../tables/table<xsl:value-of select="position() + count(../../preceding-sibling::WorksheetSubstream/FEAT11/Feature11)"/>.xml</xsl:attribute>
                </Relationship>
              </xsl:for-each>           
              
            </Relationships>
          </xlsx:file>
        </xsl:if>
      </xsl:for-each>
    </xlsx:dir>
  </xsl:template>

  <xsl:template name="chartsheets_rels">
    <!-- This section is optional -->
    <xlsx:dir name="_rels">
      <xsl:for-each select="ChartSheetSubstream">
        <xlsx:file name="sheet{position()}.xml.rels">
          <Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships">
            <Relationship>
              <xsl:attribute name="Id">
                <xsl:text>rId</xsl:text>
                <xsl:call-template name="generate_worksheet_entity_id"/>
              </xsl:attribute>
              <xsl:attribute name="Type">http://schemas.openxmlformats.org/officeDocument/2006/relationships/drawing</xsl:attribute>
              <xsl:attribute name="Target">../drawings/drawing<xsl:value-of select="count(preceding-sibling::WorksheetSubstream[OBJECTS]) + count(preceding-sibling::ChartSheetSubstream) + 1"/>.xml</xsl:attribute>
            </Relationship>
            
            <xsl:for-each select="CUSTOMVIEW">
              <xsl:if test="Pls">
                <Relationship>
                  <xsl:attribute name="Id">
                    <xsl:text>rId</xsl:text>
                    <xsl:call-template name="generate_worksheet_entity_id"/>
                  </xsl:attribute>
                  <xsl:attribute name="Type">http://schemas.openxmlformats.org/officeDocument/2006/relationships/printerSettings</xsl:attribute>
                  <xsl:attribute name="Target">
                    <xsl:text>../printerSettings/printerSettings</xsl:text>
                    <xsl:call-template name="generate_printer_settings_id"/>
                    <xsl:text>.bin</xsl:text>
                  </xsl:attribute>
                </Relationship>
              </xsl:if>
            </xsl:for-each>
            <xsl:for-each select="PAGESETUP">
              <xsl:if test="Pls">
                <Relationship>
                  <xsl:attribute name="Id">
                    <xsl:text>rId</xsl:text>
                    <xsl:call-template name="generate_worksheet_entity_id"/>
                  </xsl:attribute>
                  <xsl:attribute name="Type">http://schemas.openxmlformats.org/officeDocument/2006/relationships/printerSettings</xsl:attribute>
                  <xsl:attribute name="Target">
                    <xsl:text>../printerSettings/printerSettings</xsl:text>
                    <xsl:call-template name="generate_printer_settings_id"/>
                    <xsl:text>.bin</xsl:text>
                  </xsl:attribute>
                </Relationship>
              </xsl:if>
            </xsl:for-each>
          </Relationships>
        </xlsx:file>
      </xsl:for-each>
    </xlsx:dir>
  </xsl:template>


</xsl:stylesheet>