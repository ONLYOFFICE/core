<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:xlsx="http://www.avs4you.ru/XLSXConverter/1.0/DirectoryStructure">

  <xsl:template match="WorkbookStreamObject">
    <xlsx:dir name="xl">
      <xsl:call-template name="xl_rels"/>
      <xsl:apply-templates select="GlobalsSubstream/FORMATTING"/>
      <xsl:apply-templates select="GlobalsSubstream/SHAREDSTRINGS/SST[@cstTotal != 0]"/>
      <!--<xsl:apply-templates select="WorksheetSubstream" mode="comments"/>-->

      <xsl:if test="WorksheetSubstream[OBJECTS] | ChartSheetSubstream">
        <xlsx:dir name="drawings">
          <xsl:call-template name="drawings_rels"/>
          <xsl:apply-templates select="WorksheetSubstream[OBJECTS] | ChartSheetSubstream" mode="drawing"/>
        </xlsx:dir>
        <xlsx:dir name="charts">
          <xsl:apply-templates select="WorksheetSubstream/OBJECTS/CHART | ChartSheetSubstream" mode="charts"/>
        </xlsx:dir>
      </xsl:if>

      <xsl:if test="WorksheetSubstream[FEAT11]">
        <xlsx:dir name="tables">
          <xsl:for-each select="WorksheetSubstream">

            <xsl:for-each select="FEAT11/Feature11">
              <xsl:variable name="table_id" select="TableFeatureType/@idList"/>

              <xlsx:file name="table{$table_id}.xml">
                <table xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
                  <xsl:attribute name="id">
                    <xsl:value-of select="$table_id"/>
                  </xsl:attribute>
                  <xsl:attribute name="name">
                    <xsl:value-of select="TableFeatureType/@rgbName"/>
                  </xsl:attribute>
                  <xsl:attribute name="displayName">
                    <xsl:value-of select="TableFeatureType/@rgbName"/>
                  </xsl:attribute>
                  <xsl:attribute name="ref">
                    <xsl:value-of select="Ref8U/@name"/>
                  </xsl:attribute>
                  <xsl:attribute name="totalsRowShown">
                    <xsl:value-of select="TableFeatureType/@fShownTotalRow"/>
                  </xsl:attribute>

                  <autoFilter xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
                    <xsl:attribute name="ref">
                      <xsl:value-of select="@sqref"/>
                    </xsl:attribute>

                    <!-- Feat11FieldDataItem items -->
                    <xsl:for-each select="TableFeatureType/Feat11FieldDataItem/Feat11FdaAutoFilter[@autoFilter_str1 or @autoFilter_str2]">
                      <filterColumn>
                        <xsl:attribute name="colId">
                          <xsl:value-of select="../@idField - 1"/>
                        </xsl:attribute>

                        <xsl:if test="@autoFilter_str1">
                          <filter>
                            <xsl:attribute name="val">
                              <xsl:value-of select="@autoFilter_str1"/>
                            </xsl:attribute>
                          </filter>
                        </xsl:if>
                        <xsl:if test="@autoFilter_str2">
                          <filter>
                            <xsl:attribute name="val">
                              <xsl:value-of select="@autoFilter_str2"/>
                            </xsl:attribute>
                          </filter>
                        </xsl:if>

                      </filterColumn>
                    </xsl:for-each>

                    <!-- AutoFilter12 -->
                    <xsl:if test="../AutoFilter12[@idList = $table_id]">

                      <xsl:for-each select="../AutoFilter12[@idList = $table_id]">
                        <filterColumn>
                          <xsl:attribute name="colId">
                            <xsl:value-of select="@iEntry"/>
                          </xsl:attribute>

                          <xsl:for-each select="AF12Criteria">
                            <filter>
                              <xsl:attribute name="val">
                                <xsl:value-of select="@str"/>
                              </xsl:attribute>
                            </filter>
                          </xsl:for-each>

                        </filterColumn>
                      </xsl:for-each>

                    </xsl:if>                    

                  </autoFilter>

                  <xsl:if test="TableFeatureType/@cFieldData > 0">
                    <tableColumns>
                      <xsl:attribute name="count">
                        <xsl:value-of select="TableFeatureType/@cFieldData"/>
                      </xsl:attribute>

                      <xsl:for-each select="TableFeatureType/Feat11FieldDataItem">
                        <tableColumn>
                          <xsl:attribute name="id">
                            <xsl:value-of select="@idField"/>
                          </xsl:attribute>
                          <xsl:attribute name="name">
                            <xsl:value-of select="@strCaption"/>
                          </xsl:attribute>
                        </tableColumn>
                      </xsl:for-each>

                    </tableColumns>
                  </xsl:if>

                  <xsl:if test="../List12[@idList = $table_id]/List12TableStyleClientInfo">
                    <tableStyleInfo>
                      <xsl:attribute name="name">
                        <xsl:value-of select="../List12[@idList = $table_id]/List12TableStyleClientInfo/@stListStyleName"/>
                      </xsl:attribute>
                      <xsl:attribute name="showFirstColumn">
                        <xsl:value-of select="../List12[@idList = $table_id]/List12TableStyleClientInfo/@fFirstColumn"/>
                      </xsl:attribute>
                      <xsl:attribute name="showLastColumn">
                        <xsl:value-of select="../List12[@idList = $table_id]/List12TableStyleClientInfo/@fLastColumn"/>
                      </xsl:attribute>
                      <xsl:attribute name="showRowStripes">
                        <xsl:value-of select="../List12[@idList = $table_id]/List12TableStyleClientInfo/@fRowStripes"/>
                      </xsl:attribute>
                      <xsl:attribute name="showColumnStripes">
                        <xsl:value-of select="../List12[@idList = $table_id]/List12TableStyleClientInfo/@fColumnStripes"/>
                      </xsl:attribute>
                    </tableStyleInfo>
                  </xsl:if>

                </table>
              </xlsx:file>

            </xsl:for-each>

          </xsl:for-each>
        </xlsx:dir>
      </xsl:if>

      <xlsx:file name="workbook.xml">
        <workbook xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships">
          <xsl:if test="GlobalsSubstream/BOF/@verLastXLSaved != 0">
            <!-- Protection against OpenOffice -->
            <fileVersion appName="xl" lastEdited="{GlobalsSubstream/BOF/@verXLHigh}"
                         lowestEdited="{GlobalsSubstream/BOF/@verLastXLSaved}" rupBuild="{GlobalsSubstream/BOF/@rupBuild}"/>
          </xsl:if>
          <xsl:call-template name="fileSharing"/>
          <workbookPr>
            <xsl:call-template name="attrib_with_def">
              <!--Not sure in this rule-->
              <xsl:with-param name="name">allowRefreshQuery</xsl:with-param>
              <xsl:with-param name="val" select="WorksheetSubstream/QUERYTABLE/Qsi/@fAutoRefresh"/>
              <xsl:with-param name="def_val">false</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">autoCompressPictures</xsl:with-param>
              <xsl:with-param name="val" select="GlobalsSubstream/CompressPictures/@fAutoCompressPictures"/>
              <xsl:with-param name="def_val">true</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">backupFile</xsl:with-param>
              <xsl:with-param name="val" select="GlobalsSubstream/Backup/@fBackup"/>
              <xsl:with-param name="def_val">false</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_inv_with_def">
              <!--Inversed-->
              <xsl:with-param name="name">checkCompatibility</xsl:with-param>
              <xsl:with-param name="val" select="GlobalsSubstream/Compat12/@fNoCompatChk"/>
              <xsl:with-param name="def_val">false</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib">
              <xsl:with-param name="name">codeName</xsl:with-param>
              <xsl:with-param name="val" select="GlobalsSubstream/CodeName/@codeName"/>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">date1904</xsl:with-param>
              <xsl:with-param name="val" select="GlobalsSubstream/Date1904/@f1904DateSystem"/>
              <xsl:with-param name="def_val">false</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib">
              <xsl:with-param name="name">defaultThemeVersion</xsl:with-param>
              <xsl:with-param name="val" select="GlobalsSubstream/THEME/Theme/@dwThemeVersion"/>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">filterPrivacy</xsl:with-param>
              <xsl:with-param name="val" select="GlobalsSubstream/BookExt/@fFilterPrivacy"/>
              <xsl:with-param name="def_val">false</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">hidePivotFieldList</xsl:with-param>
              <xsl:with-param name="val" select="GlobalsSubstream/BookExt/@fHidePivotList"/>
              <xsl:with-param name="def_val">false</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">promptedSolutions</xsl:with-param>
              <xsl:with-param name="val" select="GlobalsSubstream/BookExt/BookExt_Conditional11/@fBuggedUserAboutSolution"/>
              <xsl:with-param name="def_val">false</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">publishItems</xsl:with-param>
              <xsl:with-param name="val" select="GlobalsSubstream/BookExt/BookExt_Conditional12/@fPublishedBookItems"/>
              <xsl:with-param name="def_val">false</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">refreshAllConnections</xsl:with-param>
              <xsl:with-param name="val" select="GlobalsSubstream/RefreshAll/@refreshAll"/>
              <xsl:with-param name="def_val">false</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_inv_with_def">
              <!--Inversed-->
              <xsl:with-param name="name">saveExternalLinkValues</xsl:with-param>
              <xsl:with-param name="val" select="GlobalsSubstream/BookBool/@fNoSaveSup"/>
              <xsl:with-param name="def_val">true</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_inv_with_def">
              <!--Inversed-->
              <xsl:with-param name="name">showBorderUnselectedTables</xsl:with-param>
              <xsl:with-param name="val" select="GlobalsSubstream/BookBool/@fHideBorderUnselLists"/>
              <xsl:with-param name="def_val">true</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">showInkAnnotation</xsl:with-param>
              <xsl:with-param name="val" select="GlobalsSubstream/BookExt/BookExt_Conditional11/@fShowInkAnnotation"/>
              <xsl:with-param name="def_val">true</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">showObjects</xsl:with-param>
              <xsl:with-param name="val" select="GlobalsSubstream/HideObj/@hideObj"/>
              <xsl:with-param name="def_val">all</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">showPivotChartFilter</xsl:with-param>
              <xsl:with-param name="val" select="GlobalsSubstream/BookExt/BookExt_Conditional12/@fShowPivotChartFilter"/>
              <xsl:with-param name="def_val">false</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">updateLinks</xsl:with-param>
              <xsl:with-param name="val" select="GlobalsSubstream/BookBool/@grUpdateLinks"/>
              <xsl:with-param name="def_val">userSet</xsl:with-param>
            </xsl:call-template>
          </workbookPr>
          <bookViews>
            <xsl:for-each select="GlobalsSubstream/Window1">
              <workbookView xWindow="{@xWn}" yWindow="{@yWn}" windowWidth="{@dxWn}" windowHeight="{@dyWn}">
                <xsl:call-template name="attrib_with_def">
                  <xsl:with-param name="name">activeTab</xsl:with-param>
                  <xsl:with-param name="val" select="@itabCur"/>
                  <xsl:with-param name="def_val">0</xsl:with-param>
                </xsl:call-template>
                <xsl:call-template name="attrib_inv_with_def">
                  <!--Inversed-->
                  <xsl:with-param name="name">autoFilterDateGrouping</xsl:with-param>
                  <xsl:with-param name="val" select="@fNoAFDateGroup"/>
                  <xsl:with-param name="def_val">true</xsl:with-param>
                </xsl:call-template>
                <xsl:call-template name="attrib_with_def">
                  <xsl:with-param name="name">firstSheet</xsl:with-param>
                  <xsl:with-param name="val" select="@itabFirst"/>
                  <xsl:with-param name="def_val">0</xsl:with-param>
                </xsl:call-template>
                <xsl:call-template name="attrib_with_def">
                  <xsl:with-param name="name">minimized</xsl:with-param>
                  <xsl:with-param name="val" select="@fIconic"/>
                  <xsl:with-param name="def_val">false</xsl:with-param>
                </xsl:call-template>
                <xsl:call-template name="attrib_with_def">
                  <xsl:with-param name="name">showHorizontalScroll</xsl:with-param>
                  <xsl:with-param name="val" select="@fDspHScroll"/>
                  <xsl:with-param name="def_val">true</xsl:with-param>
                </xsl:call-template>
                <xsl:call-template name="attrib_with_def">
                  <xsl:with-param name="name">showVerticalScroll</xsl:with-param>
                  <xsl:with-param name="val" select="@fDspVScroll"/>
                  <xsl:with-param name="def_val">true</xsl:with-param>
                </xsl:call-template>
                <xsl:call-template name="attrib_with_def">
                  <xsl:with-param name="name">showSheetTabs</xsl:with-param>
                  <xsl:with-param name="val" select="@fBotAdornment"/>
                  <xsl:with-param name="def_val">true</xsl:with-param>
                </xsl:call-template>
                <xsl:call-template name="attrib_with_def">
                  <xsl:with-param name="name">tabRatio</xsl:with-param>
                  <xsl:with-param name="val" select="@wTabRatio"/>
                  <xsl:with-param name="def_val">600</xsl:with-param>
                </xsl:call-template>
                <xsl:choose>
                  <xsl:when test="@fHidden = 'true' and @fVeryHidden = 'false'">
                    <xsl:attribute name="visibility">hidden</xsl:attribute>
                  </xsl:when>
                  <xsl:when test="@fHidden = 'true' and @fVeryHidden = 'true'">
                    <xsl:attribute name="visibility">veryHidden</xsl:attribute>
                  </xsl:when>
                  <!--Default is "visible"-->
                </xsl:choose>
              </workbookView>
            </xsl:for-each>
          </bookViews>
          <sheets>
            <xsl:variable name="ws_num" select="count(WorksheetSubstream)"/>
            <xsl:for-each select="WorksheetSubstream | ChartSheetSubstream">
              <sheet>
                <xsl:variable name="stream_ptr" select="./BOF/@stream_ptr"/>
                <xsl:attribute name="name">
                  <xsl:value-of select="../GlobalsSubstream/BUNDLESHEET/BoundSheet8[@lbPlyPos=$stream_ptr]/@stName"></xsl:value-of>
                </xsl:attribute>
                <xsl:attribute name="sheetId">
                  <xsl:variable name="sheet_num" select="position()"/>
                  <xsl:value-of select="../GlobalsSubstream/RRTabId/SheetId[position() = $sheet_num]/@id"></xsl:value-of>
                </xsl:attribute>
                <xsl:call-template name="attrib_with_def">
                  <xsl:with-param name="name">state</xsl:with-param>
                  <xsl:with-param name="val">
                    <xsl:value-of select="../GlobalsSubstream/BUNDLESHEET/BoundSheet8[@lbPlyPos=$stream_ptr]/@hsState"></xsl:value-of>
                  </xsl:with-param>
                  <xsl:with-param name="def_val">visible</xsl:with-param>
                </xsl:call-template>
                <xsl:attribute name="r:id">
                  <xsl:text>rId</xsl:text>
                  <xsl:value-of select="position()"></xsl:value-of>
                </xsl:attribute>
              </sheet>
            </xsl:for-each>
          </sheets>
          <xsl:call-template name="functionGroups"/>
          <xsl:if test="GlobalsSubstream/LBL/Lbl">
            <definedNames>
              <xsl:for-each select="GlobalsSubstream/LBL/Lbl">
                <xsl:if test="NameParsedFormula/@assembled_formula != ''">
                  <definedName name="{@Name}">
                    <!--<xsl:value-of select="NameParsedFormula/@assembled_formula"/>-->
                    <xsl:call-template name="attrib">
                      <xsl:with-param name="name">comment</xsl:with-param>
                      <xsl:with-param name="val" select="following-sibling::NameCmt/@comment"/>
                    </xsl:call-template>
                    <!--
                    May be must be got from 'revisions/RRDDefName'
                    <xsl:call-template name="attrib">
                      <xsl:with-param name="name">customMenu</xsl:with-param>
                      <xsl:with-param name="val" select=""/>
                    </xsl:call-template>
                    <xsl:call-template name="attrib">
                      <xsl:with-param name="name">description</xsl:with-param>
                      <xsl:with-param name="val" select=""/>
                    </xsl:call-template>
                    <xsl:call-template name="attrib_with_def">   !!! Not sure
                      <xsl:with-param name="name">function</xsl:with-param>
                      <xsl:with-param name="val"><xsl:value-of select="@fFunc"/></xsl:with-param>
                      <xsl:with-param name="def_val">false</xsl:with-param>
                    </xsl:call-template>
                    -->
                    <xsl:call-template name="attrib_with_def">
                      <xsl:with-param name="name">functionGroupId</xsl:with-param>
                      <xsl:with-param name="val" select="@fGrp"/>
                      <xsl:with-param name="def_val">0</xsl:with-param>
                    </xsl:call-template>
                    <!--
                    May be must be got from 'revisions/RRDDefName'
                    <xsl:call-template name="attrib">
                      <xsl:with-param name="name">help</xsl:with-param>
                      <xsl:with-param name="val" select=""/>
                    </xsl:call-template>
                    -->
                    <xsl:call-template name="attrib_with_def">
                      <xsl:with-param name="name">hidden</xsl:with-param>
                      <xsl:with-param name="val" select="@fHidden"/>
                      <xsl:with-param name="def_val">false</xsl:with-param>
                    </xsl:call-template>
                    <xsl:call-template name="attrib_with_def">
                      <xsl:with-param name="name">localSheetId</xsl:with-param>
                      <xsl:with-param name="val" select="@itab - 1"/>
                      <xsl:with-param name="def_val">-1</xsl:with-param>
                    </xsl:call-template>
                    <xsl:call-template name="attrib_with_def">
                      <xsl:with-param name="name">publishToServer</xsl:with-param>
                      <xsl:with-param name="val" select="@fPublished"/>
                      <xsl:with-param name="def_val">false</xsl:with-param>
                    </xsl:call-template>
                    <xsl:call-template name="attrib_with_def">
                      <xsl:with-param name="name">shortcutKey</xsl:with-param>
                      <xsl:with-param name="val" select="@chKey"/>
                      <xsl:with-param name="def_val">0</xsl:with-param>
                    </xsl:call-template>
                    <!--
                    May be must be got from 'revisions/RRDDefName'
                    <xsl:call-template name="attrib">
                      <xsl:with-param name="name">statusBar</xsl:with-param>
                      <xsl:with-param name="val" select=""/>
                    </xsl:call-template>
                    -->
                    <xsl:call-template name="attrib_with_def">
                      <!-- !!! Not sure -->
                      <xsl:with-param name="name">vbProcedure</xsl:with-param>
                      <xsl:with-param name="val" select="@fOB"/>
                      <xsl:with-param name="def_val">false</xsl:with-param>
                    </xsl:call-template>
                    <xsl:call-template name="attrib_with_def">
                      <xsl:with-param name="name">workbookParameter</xsl:with-param>
                      <xsl:with-param name="val" select="@fWorkbookParam"/>
                      <xsl:with-param name="def_val">false</xsl:with-param>
                    </xsl:call-template>
                    <xsl:call-template name="attrib_with_def">
                      <!-- !!! Not sure -->
                      <xsl:with-param name="name">xlm</xsl:with-param>
                      <xsl:with-param name="val" select="@fFunc"/>
                      <xsl:with-param name="def_val">false</xsl:with-param>
                    </xsl:call-template>
                    <!--Content:-->
                    <xsl:call-template name="fillUndefinedNames">
                      <xsl:with-param name="str" select="NameParsedFormula/@assembled_formula"/>
                    </xsl:call-template>
                  </definedName>
                </xsl:if>
              </xsl:for-each>
            </definedNames>
          </xsl:if>
          <calcPr>
            <xsl:call-template name="attrib">
              <xsl:with-param name="name">calcId</xsl:with-param>
              <xsl:with-param name="val" select="GlobalsSubstream/RecalcId/@dwBuild"/>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">calcMode</xsl:with-param>
              <xsl:with-param name="val" select="WorksheetSubstream/GLOBALS/CalcMode/@fAutoRecalc"/>
              <xsl:with-param name="def_val">auto</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">fullCalcOnLoad</xsl:with-param>
              <xsl:with-param name="val" select="boolean(GlobalsSubstream/RecalcId/@dwBuild = 1)"/>
              <xsl:with-param name="def_val">false</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">refMode</xsl:with-param>
              <xsl:with-param name="val" select="WorksheetSubstream/GLOBALS/CalcRefMode/@fRefA1"/>
              <xsl:with-param name="def_val">A1</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">iterate</xsl:with-param>
              <xsl:with-param name="val" select="WorksheetSubstream/GLOBALS/CalcIter/@vfIter"/>
              <xsl:with-param name="def_val">false</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">iterateCount</xsl:with-param>
              <xsl:with-param name="val" select="WorksheetSubstream/GLOBALS/CalcCount/@cIter"/>
              <xsl:with-param name="def_val">100</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">iterateDelta</xsl:with-param>
              <xsl:with-param name="val" select="WorksheetSubstream/GLOBALS/CalcDelta/@numDelta"/>
              <xsl:with-param name="def_val">0.001</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">fullPrecision</xsl:with-param>
              <xsl:with-param name="val" select="GlobalsSubstream/CalcPrecision/@fFullPrec"/>
              <xsl:with-param name="def_val">true</xsl:with-param>
            </xsl:call-template>
            <xsl:if test="WorksheetSubstream/Uncalced">
              <xsl:attribute name="calcCompleted">false</xsl:attribute>
            </xsl:if>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">calcOnSave</xsl:with-param>
              <xsl:with-param name="val" select="WorksheetSubstream/GLOBALS/CalcSaveRecalc/@fSaveRecalc"/>
              <xsl:with-param name="def_val">true</xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">concurrentCalc</xsl:with-param>
              <xsl:with-param name="val" select="GlobalsSubstream/MTRSettings/@fMTREnabled"/>
              <xsl:with-param name="def_val">true</xsl:with-param>
            </xsl:call-template>
            <xsl:if test="GlobalsSubstream/MTRSettings/@fUserSetThreadCount != 'false' and GlobalsSubstream/MTRSettings/@cUserThreadCount != 0">
              <xsl:attribute name="concurrentManualCount">
                <xsl:value-of select="GlobalsSubstream/MTRSettings/@cUserThreadCount"/>
              </xsl:attribute>
            </xsl:if>
            <xsl:call-template name="attrib_with_def">
              <xsl:with-param name="name">forceFullCalc</xsl:with-param>
              <xsl:with-param name="val" select="GlobalsSubstream/ForceFullCalculation/@fNoDeps"/>
              <xsl:with-param name="def_val">false</xsl:with-param>
            </xsl:call-template>


          </calcPr>
          <xsl:if test="GlobalsSubstream/UserBView">
            <customWorkbookViews>
              <xsl:for-each select="GlobalsSubstream/UserBView">
                <customWorkbookView name="{@st}" guid="{@guid}" windowWidth="{@dx}" windowHeight="{@dy}" activeSheetId="{@tabId}">
                  <xsl:call-template name="attrib_with_def">
                    <xsl:with-param name="name">autoUpdate</xsl:with-param>
                    <xsl:with-param name="val" select="@fTimedUpdate"/>
                    <xsl:with-param name="def_val">false</xsl:with-param>
                  </xsl:call-template>
                  <xsl:call-template name="attrib_with_def">
                    <xsl:with-param name="name">changesSavedWin</xsl:with-param>
                    <xsl:with-param name="val" select="@fAllMemChanges"/>
                    <xsl:with-param name="def_val">false</xsl:with-param>
                  </xsl:call-template>
                  <xsl:call-template name="attrib_with_def">
                    <xsl:with-param name="name">includeHiddenRowCol</xsl:with-param>
                    <xsl:with-param name="val" select="@fRowColIncl"/>
                    <xsl:with-param name="def_val">true</xsl:with-param>
                  </xsl:call-template>
                  <xsl:call-template name="attrib_with_def">
                    <xsl:with-param name="name">includePrintSettings</xsl:with-param>
                    <xsl:with-param name="val" select="@fPrintIncl"/>
                    <xsl:with-param name="def_val">true</xsl:with-param>
                  </xsl:call-template>
                  <xsl:call-template name="attrib_with_def">
                    <xsl:with-param name="name">maximized</xsl:with-param>
                    <xsl:with-param name="val" select="@fZoom"/>
                    <xsl:with-param name="def_val">false</xsl:with-param>
                  </xsl:call-template>
                  <xsl:call-template name="attrib_with_def">
                    <xsl:with-param name="name">minimized</xsl:with-param>
                    <xsl:with-param name="val" select="@fIconic"/>
                    <xsl:with-param name="def_val">false</xsl:with-param>
                  </xsl:call-template>
                  <xsl:call-template name="attrib">
                    <xsl:with-param name="name">mergeInterval</xsl:with-param>
                    <xsl:with-param name="val" select="@wMergeInterval"/>
                  </xsl:call-template>
                  <xsl:call-template name="attrib_with_def">
                    <xsl:with-param name="name">onlySync</xsl:with-param>
                    <xsl:with-param name="val" select="@fOnlySync"/>
                    <xsl:with-param name="def_val">false</xsl:with-param>
                  </xsl:call-template>
                  <xsl:call-template name="attrib_with_def">
                    <xsl:with-param name="name">personalView</xsl:with-param>
                    <xsl:with-param name="val" select="@fPersonalView"/>
                    <xsl:with-param name="def_val">false</xsl:with-param>
                  </xsl:call-template>
                  <xsl:call-template name="attrib_with_def">
                    <xsl:with-param name="name">showComments</xsl:with-param>
                    <xsl:with-param name="val" select="@mdNoteDisp"/>
                    <xsl:with-param name="def_val">commIndicator</xsl:with-param>
                  </xsl:call-template>
                  <xsl:call-template name="attrib_with_def">
                    <xsl:with-param name="name">showFormulaBar</xsl:with-param>
                    <xsl:with-param name="val" select="@fDspFmlaBar"/>
                    <xsl:with-param name="def_val">true</xsl:with-param>
                  </xsl:call-template>
                  <xsl:call-template name="attrib_with_def">
                    <xsl:with-param name="name">showHorizontalScroll</xsl:with-param>
                    <xsl:with-param name="val" select="@fDspHScroll"/>
                    <xsl:with-param name="def_val">true</xsl:with-param>
                  </xsl:call-template>
                  <xsl:call-template name="attrib_with_def">
                    <xsl:with-param name="name">showVerticalScroll</xsl:with-param>
                    <xsl:with-param name="val" select="@fDspVScroll"/>
                    <xsl:with-param name="def_val">true</xsl:with-param>
                  </xsl:call-template>
                  <xsl:call-template name="attrib_with_def">
                    <xsl:with-param name="name">showObjects</xsl:with-param>
                    <xsl:with-param name="val" select="@fHideObj"/>
                    <xsl:with-param name="def_val">all</xsl:with-param>
                  </xsl:call-template>
                  <xsl:call-template name="attrib_with_def">
                    <xsl:with-param name="name">showSheetTabs</xsl:with-param>
                    <xsl:with-param name="val" select="@fBotAdornment"/>
                    <xsl:with-param name="def_val">true</xsl:with-param>
                  </xsl:call-template>
                  <xsl:call-template name="attrib_with_def">
                    <xsl:with-param name="name">showStatusbar</xsl:with-param>
                    <xsl:with-param name="val" select="@fDspStatus"/>
                    <xsl:with-param name="def_val">true</xsl:with-param>
                  </xsl:call-template>
                  <xsl:call-template name="attrib_with_def">
                    <xsl:with-param name="name">tabRatio</xsl:with-param>
                    <xsl:with-param name="val" select="@wTabRatio"/>
                    <xsl:with-param name="def_val">600</xsl:with-param>
                  </xsl:call-template>
                  <xsl:call-template name="attrib_with_def">
                    <xsl:with-param name="name">xWindow</xsl:with-param>
                    <xsl:with-param name="val" select="@x"/>
                    <xsl:with-param name="def_val">0</xsl:with-param>
                  </xsl:call-template>
                  <xsl:call-template name="attrib_with_def">
                    <xsl:with-param name="name">yWindow</xsl:with-param>
                    <xsl:with-param name="val" select="@y"/>
                    <xsl:with-param name="def_val">0</xsl:with-param>
                  </xsl:call-template>
                </customWorkbookView>
              </xsl:for-each>
            </customWorkbookViews>
          </xsl:if>
          <xsl:call-template name="fileRecoveryPr"/>
        </workbook>
      </xlsx:file>
      <xsl:if test="WorksheetSubstream">
        <xlsx:dir name="worksheets">
          <xsl:call-template name="worksheets_rels"/>
          <xsl:apply-templates select="WorksheetSubstream"/>
        </xlsx:dir>
      </xsl:if>
      <xsl:if test="ChartSheetSubstream">
        <xlsx:dir name="chartsheets">
          <xsl:call-template name="chartsheets_rels"/>
          <xsl:apply-templates select="ChartSheetSubstream"/>
        </xlsx:dir>
      </xsl:if>
      <xsl:call-template name="printerSettings"/>
      <xsl:call-template name="images"/>
    </xlsx:dir>
  </xsl:template>

  <xsl:template name ="attrib_with_def">
    <xsl:param name="name"/>
    <xsl:param name="val"/>
    <xsl:param name="def_val"/>
    <!--<xsl:message>'attrib_with_def' called with name='<xsl:value-of select="$name"/>'</xsl:message>-->
    <xsl:if test="$val != '' and $val != $def_val">
      <xsl:attribute name="{$name}">
        <xsl:value-of select="$val"/>
      </xsl:attribute>
    </xsl:if>
  </xsl:template>

  <xsl:template name ="attrib_inv_with_def">
    <!-- Only for boolean values -->
    <xsl:param name="name"/>
    <xsl:param name="val"/>
    <xsl:param name="def_val"/>
    <xsl:if test="$val != '' and not($val = 'true') != boolean($def_val = 'true')">
      <xsl:attribute name="{$name}">
        <xsl:value-of select="not($val='true')"/>
      </xsl:attribute>
    </xsl:if>
  </xsl:template>

  <xsl:template name ="attrib">
    <xsl:param name="name"/>
    <xsl:param name="val"/>
    <xsl:if test="$val != ''">
      <xsl:attribute name="{$name}">
        <xsl:value-of select="$val"/>
      </xsl:attribute>
    </xsl:if>
  </xsl:template>

  <xsl:template name ="fillUndefinedNames">
    <xsl:param name="str"/>
    <xsl:choose>
      <xsl:when test="contains($str, '#UNDEFINED_NAME')">
        <xsl:variable name="part1" select="substring-before($str,'#UNDEFINED_NAME(')"/>
        <xsl:variable name="the_rest" select="substring-after($str,'#UNDEFINED_NAME(')"/>
        <xsl:variable name="num" select="substring-before($the_rest,')!')"/>
        <xsl:variable name="part2" select="substring-after($the_rest,')!')"/>
        <xsl:variable name="val">
          <xsl:value-of select="$part1"/>
          <xsl:value-of select="ancestor::GlobalsSubstream/LBL[position()=$num]/Lbl/@Name"/>
          <xsl:value-of select="$part2"/>
        </xsl:variable>
        <xsl:call-template name="fillUndefinedNames">
          <xsl:with-param name="str" select="$val"/>
        </xsl:call-template>
      </xsl:when>

      <xsl:otherwise>
        <xsl:value-of select="$str"/>
      </xsl:otherwise>

    </xsl:choose>
  </xsl:template>

  <xsl:template name="printerSettings">
    <xsl:if test="WorksheetSubstream/CUSTOMVIEW/Pls or WorksheetSubstream/PAGESETUP/Pls or ChartSheetSubstream/CUSTOMVIEW/Pls or ChartSheetSubstream/PAGESETUP/Pls">
      <xlsx:dir name="printerSettings">
        <xsl:for-each select="WorksheetSubstream | ChartSheetSubstream">
          <xsl:for-each select="CUSTOMVIEW">
            <xsl:if test="Pls">
              <xlsx:file format="bin">
                <xsl:attribute name="name">
                  <xsl:text>printerSettings</xsl:text>
                  <xsl:call-template name="generate_printer_settings_id"/>
                  <xsl:text>.bin</xsl:text>
                </xsl:attribute>
                <xsl:attribute name="bin_data_id">
                  <xsl:value-of select="Pls/@bin_data_id"/>
                </xsl:attribute>
              </xlsx:file>
            </xsl:if>
          </xsl:for-each>
          <xsl:for-each select="PAGESETUP">
            <xsl:if test="Pls">
              <xlsx:file format="bin">
                <xsl:attribute name="name">
                  <xsl:text>printerSettings</xsl:text>
                  <xsl:call-template name="generate_printer_settings_id"/>
                  <xsl:text>.bin</xsl:text>
                </xsl:attribute>
                <xsl:attribute name="bin_data_id">
                  <xsl:value-of select="Pls/@bin_data_id"/>
                </xsl:attribute>
              </xlsx:file>
            </xsl:if>
          </xsl:for-each>
        </xsl:for-each>
      </xlsx:dir>
    </xsl:if>
  </xsl:template>

  <xsl:template name="images">
    <xsl:if test="GlobalsSubstream/MSODRAWINGGROUP/MsoDrawingGroup/OfficeArtDggContainer/OfficeArtBStoreContainer">
      <xlsx:dir name="media">
        <xsl:for-each select="GlobalsSubstream/MSODRAWINGGROUP/MsoDrawingGroup/OfficeArtDggContainer/OfficeArtBStoreContainer/OfficeArtBStoreContainerFileBlock">
          <!--<xsl:if test="OfficeArtBStoreContainerFileBlock">-->
          <xlsx:file format="bin">
            <xsl:attribute name="name">
              <xsl:text>image</xsl:text>
              <xsl:value-of select="position()"/>
              <xsl:text>.</xsl:text>
              <xsl:value-of select="@type"/>
            </xsl:attribute>
            <xsl:attribute name="bin_data_id">
              <xsl:value-of select="@bin_data_id"/>
            </xsl:attribute>
          </xlsx:file>
          <!--</xsl:if>-->
        </xsl:for-each>
      </xlsx:dir>
    </xsl:if>
  </xsl:template>

  <xsl:template name="fileRecoveryPr">
    <xsl:variable name="bookext" select="GlobalsSubstream/BookExt"/>
    <xsl:if test="$bookext/@fDontAutoRecover = 'true' or $bookext/@fCreatedViaMinimalSave = 'true' or
                  $bookext/@fOpenedViaDataRecovery = 'true' or $bookext/@fOpenedViaSafeLoad = 'true'">
      <fileRecoveryPr xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:if test="$bookext/@fDontAutoRecover = 'true'">
          <xsl:attribute name="autoRecover">false</xsl:attribute>
          <!-- Inversed -->
        </xsl:if>
        <xsl:if test="$bookext/@fCreatedViaMinimalSave = 'true'">
          <xsl:attribute name="crashSave">true</xsl:attribute>
        </xsl:if>
        <xsl:if test="$bookext/@fOpenedViaDataRecovery = 'true'">
          <xsl:attribute name="dataExtractLoad">true</xsl:attribute>
        </xsl:if>
        <xsl:if test="$bookext/@fOpenedViaSafeLoad = 'true'">
          <xsl:attribute name="repairLoad">true</xsl:attribute>
        </xsl:if>
      </fileRecoveryPr>
    </xsl:if>
  </xsl:template>

  <xsl:template name="fileSharing">
    <xsl:variable name="sharing" select="GlobalsSubstream/FileSharing"/>
    <xsl:if test="$sharing/@fReadOnlyRec = 'true' or $sharing/@stUNUsername != '' or $sharing/@wResPass">
      <fileSharing xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:if test="$sharing/@fReadOnlyRec = 'true'">
          <xsl:attribute name="readOnlyRecommended">true</xsl:attribute>
        </xsl:if>
        <xsl:if test="$sharing/@stUNUsername != ''">
          <xsl:attribute name="userName">
            <xsl:value-of select="$sharing/@stUNUsername"/>
          </xsl:attribute>
        </xsl:if>
        <xsl:if test="$sharing/@wResPass">
          <xsl:attribute name="password">
            <xsl:value-of select="$sharing/@wResPass"/>
          </xsl:attribute>
        </xsl:if>
      </fileSharing>
    </xsl:if>
  </xsl:template>


  <xsl:template name="functionGroups">
    <xsl:if test="GlobalsSubstream/FNGROUPS">
      <functionGroups xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:if test="GlobalsSubstream/FNGROUPS/BuiltInFnGroupCount/@count != 16">
          <xsl:attribute name="builtInGroupCount">
            <xsl:value-of select="GlobalsSubstream/FNGROUPS/BuiltInFnGroupCount/@count"/>
          </xsl:attribute>
        </xsl:if>
        <xsl:for-each select="GlobalsSubstream/FNGROUPS/FnGroupName">
          <functionGroup name="{@rgch}"/>
        </xsl:for-each>
        <xsl:for-each select="GlobalsSubstream/FNGROUPS/FnGrp12">
          <functionGroup name="{@astFnGrp}"/>
        </xsl:for-each>
      </functionGroups>
    </xsl:if>
  </xsl:template>


</xsl:stylesheet>