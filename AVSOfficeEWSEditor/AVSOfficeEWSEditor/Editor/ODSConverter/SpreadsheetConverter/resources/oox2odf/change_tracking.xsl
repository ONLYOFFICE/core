<?xml version="1.0" encoding="UTF-8"?>
<!--
    * Copyright (c) 2006, Clever Age
    * All rights reserved.
    * 
    * Redistribution and use in source and binary forms, with or without
    * modification, are permitted provided that <office:spreadsheet the following conditions are met:
    *
    *     * Redistributions of source code must retain the above copyright
    *       notice, this list of conditions and the following disclaimer.
    *     * Redistributions in binary form must reproduce the above copyright
    *       notice, this list of conditions and the following disclaimer in the
    *       documentation and/or other materials provided with the distribution.
    *     * Neither the name of Clever Age nor the names of its contributors 
    *       may be used to endorse or promote products derived from this software
    *       without specific prior written permission.
    *
    * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
    * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
    * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
  xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns:number="urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0"
  xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
  xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:dc="http://purl.org/dc/elements/1.1/"
  xmlns:meta="urn:oasis:names:tc:opendocument:xmlns:meta:1.0"
  xmlns:chart="urn:oasis:names:tc:opendocument:xmlns:chart:1.0"
  xmlns:dr3d="urn:oasis:names:tc:opendocument:xmlns:dr3d:1.0"
  xmlns:math="http://www.w3.org/1998/Math/MathML"
  xmlns:form="urn:oasis:names:tc:opendocument:xmlns:form:1.0"
  xmlns:script="urn:oasis:names:tc:opendocument:xmlns:script:1.0"
  xmlns:ooo="http://openoffice.org/2004/office" xmlns:ooow="http://openoffice.org/2004/writer"
  xmlns:oooc="http://openoffice.org/2004/calc" xmlns:dom="http://www.w3.org/2001/xml-events"
  xmlns:xforms="http://www.w3.org/2002/xforms" xmlns:xsd="http://www.w3.org/2001/XMLSchema"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" office:version="1.0"
  xmlns:e="http://schemas.openxmlformats.org/spreadsheetml/2006/main" exclude-result-prefixes="e r">

  <xsl:template name="InsertChangeTracking">

    <!-- Search Revision Log Files -->
    <xsl:variable name="revisionLogFiles">
      <xsl:for-each select="key('Part', 'xl/_rels/workbook.xml.rels')//node()[name()='Relationship']">
        <xsl:if
          test="./@Type = 'http://schemas.openxmlformats.org/officeDocument/2006/relationships/revisionHeaders'">
          <xsl:value-of select="./@Target"/>
        </xsl:if>
      </xsl:for-each>
    </xsl:variable>

    <!-- Open Files with Change Tracking -->

    <xsl:for-each select="key('Part', concat('xl/', $revisionLogFiles))">
      <table:tracked-changes>
        <xsl:call-template name="SearchChangeTracking">
          <xsl:with-param name="revisionLogFiles">
            <xsl:value-of select="$revisionLogFiles"/>
          </xsl:with-param>
        </xsl:call-template>
      </table:tracked-changes>
    </xsl:for-each>

  </xsl:template>

  <!-- Insert Change Tracking -->

  <xsl:template name="SearchChangeTracking">
    <xsl:param name="revisionLogFiles"/>


    <!-- Search Previous RangeMoved -->

    <xsl:variable name="prevRangeMoved">
      <xsl:for-each select="e:headers">
        <xsl:apply-templates select="e:header[1]" mode="SearchRangeMoved">
          <xsl:with-param name="revisionLogFiles">
            <xsl:value-of select="$revisionLogFiles"/>
          </xsl:with-param>
        </xsl:apply-templates>
      </xsl:for-each>
    </xsl:variable>

    <xsl:for-each select="e:headers">
      <xsl:apply-templates select="e:header[1]">
        <xsl:with-param name="revisionLogFiles">
          <xsl:value-of select="$revisionLogFiles"/>
        </xsl:with-param>
        <xsl:with-param name="prevRangeMoved">
          <xsl:value-of select="$prevRangeMoved"/>
        </xsl:with-param>
      </xsl:apply-templates>
    </xsl:for-each>

  </xsl:template>



  <!-- Insert Change Tracking Properties -->

  <xsl:template match="e:header">
    <xsl:param name="revisionLogFiles"/>
    <xsl:param name="positionHeader" select="1"/>
    <xsl:param name="previousStyle"/>
    <xsl:param name="prevRangeMoved"/>
    <xsl:param name="previousDelColStyle"/>

    <xsl:variable name="userName">
      <xsl:value-of select="@userName"/>
    </xsl:variable>

    <xsl:variable name="dateTime">
      <xsl:value-of select="@dateTime"/>
    </xsl:variable>

    <xsl:variable name="id">
      <xsl:value-of select="@r:id"/>
    </xsl:variable>

    <xsl:variable name="Target">
      <xsl:for-each
        select="key('Part', concat('xl/', substring-before($revisionLogFiles, '/'), '/_rels/', substring-after($revisionLogFiles, '/'), '.rels'))//node()[name()='Relationship']">
        <xsl:if test="./@Id=$id">
          <xsl:value-of
            select="concat('xl/', substring-before($revisionLogFiles, '/'), '/', ./@Target)"/>
        </xsl:if>
      </xsl:for-each>
    </xsl:variable>

    <xsl:for-each select="key('Part', $Target)/e:revisions">
      <xsl:if test="e:rcc|e:rm|e:rrc">
        <xsl:apply-templates select="node()[name()='rcc' or name()='rm' or name() = 'rrc'][1]">
          <xsl:with-param name="dateTime">
            <xsl:value-of select="$dateTime"/>
          </xsl:with-param>
          <xsl:with-param name="userName">
            <xsl:value-of select="$userName"/>
          </xsl:with-param>
          <xsl:with-param name="positionHeader">
            <xsl:value-of select="$positionHeader"/>
          </xsl:with-param>
          <xsl:with-param name="previousStyle">
            <xsl:value-of select="$previousStyle"/>
          </xsl:with-param>
          <xsl:with-param name="prevRangeMoved">
            <xsl:value-of select="$prevRangeMoved"/>
          </xsl:with-param>
          <xsl:with-param name="previousDelColStyle">
            <xsl:value-of select="$previousDelColStyle"/>
          </xsl:with-param>
        </xsl:apply-templates>
      </xsl:if>
    </xsl:for-each>

    <!-- Search Previous Style or Value in Change Tracking -->
    <xsl:variable name="prevStyle">
      <xsl:for-each select="key('Part', $Target)/e:revisions">
        <xsl:if test="e:rcc|e:rm">
          <xsl:apply-templates select="node()[name()='rcc' or name()='rm'][1]"
            mode="SearchPreviousStyle">
            <xsl:with-param name="dateTime">
              <xsl:value-of select="$dateTime"/>
            </xsl:with-param>
            <xsl:with-param name="userName">
              <xsl:value-of select="$userName"/>
            </xsl:with-param>
            <xsl:with-param name="positionHeader">
              <xsl:value-of select="$positionHeader"/>
            </xsl:with-param>
            <xsl:with-param name="previousStyle">
              <xsl:value-of select="$previousStyle"/>
            </xsl:with-param>
          </xsl:apply-templates>
        </xsl:if>
      </xsl:for-each>
    </xsl:variable>

    <!-- Search Previous Delete Col in Change Tracking -->

    <xsl:variable name="prevDelColStyle">

      <xsl:for-each select="key('Part', $Target)/e:revisions">

        <xsl:if test="e:rrc">

          <xsl:apply-templates select="node()[name()='rrc'][1]" mode="prevDelColStyle">
            <xsl:with-param name="positionHeader">
              <xsl:value-of select="$positionHeader"/>
            </xsl:with-param>
            <xsl:with-param name="previousDelColStyle">
              <xsl:value-of select="$previousDelColStyle"/>
            </xsl:with-param>
          </xsl:apply-templates>
        </xsl:if>
      </xsl:for-each>
    </xsl:variable>

    <xsl:variable name="DeleteMovedRange">
      <xsl:for-each select="key('Part', $Target)/e:revisions">
        <xsl:choose>
          <xsl:when test="e:rcc|e:rm">
            <xsl:apply-templates select="node()[name()='rcc' or name()='rm'][1]"
              mode="prevRangeMoved">
              <xsl:with-param name="positionHeader">
                <xsl:value-of select="$positionHeader"/>
              </xsl:with-param>
              <xsl:with-param name="prevRangeMoved">
                <xsl:value-of select="$prevRangeMoved"/>
              </xsl:with-param>
            </xsl:apply-templates>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$prevRangeMoved"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:for-each>
    </xsl:variable>


    <xsl:if test="following-sibling::e:header">
      <xsl:apply-templates select="following-sibling::e:header[1]">
        <xsl:with-param name="revisionLogFiles">
          <xsl:value-of select="$revisionLogFiles"/>
        </xsl:with-param>
        <xsl:with-param name="positionHeader">
          <xsl:value-of select="$positionHeader + 1"/>
        </xsl:with-param>
        <xsl:with-param name="previousStyle">
          <xsl:value-of select="$prevStyle"/>
        </xsl:with-param>
        <xsl:with-param name="prevRangeMoved">
          <xsl:value-of select="$DeleteMovedRange"/>
        </xsl:with-param>
        <xsl:with-param name="previousDelColStyle">
          <xsl:value-of select="$prevDelColStyle"/>
        </xsl:with-param>
      </xsl:apply-templates>
    </xsl:if>

  </xsl:template>

  <xsl:template match="e:rcc|e:rm|e:rrc">
    <xsl:param name="dateTime"/>
    <xsl:param name="userName"/>
    <xsl:param name="positionHeader"/>
    <xsl:param name="previousStyle"/>
    <xsl:param name="prevRangeMoved"/>
    <xsl:param name="previousDelColStyle"/>

    <!-- Insert Text "Change Tracking" -->

    <xsl:variable name="Cell">
      <xsl:choose>
        <xsl:when test="e:oc/@r">
          <xsl:choose>
            <xsl:when
              test="substring-before(substring-after(concat(';', $prevRangeMoved), concat(';', e:oc/@r, '-')), ';')">
              <xsl:value-of
                select="substring-before(substring-after(concat(';', $prevRangeMoved), concat(';', e:oc/@r, '-')), ';')"
              />
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="e:oc/@r"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:when>
        <xsl:when test="e:nc/@r">
          <xsl:choose>
            <xsl:when
              test="substring-before(substring-after(concat(';', $prevRangeMoved), concat(';', e:nc/@r, '-')), ';')">
              <xsl:value-of
                select="substring-before(substring-after(concat(';', $prevRangeMoved), concat(';', e:nc/@r, '-')), ';')"
              />
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="e:nc/@r"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:when>
        <xsl:when test="@source">
          <xsl:choose>
            <xsl:when
              test="substring-before(substring-after(concat(';', $prevRangeMoved), concat(';', @source, '-')), ';')">
              <xsl:value-of
                select="substring-before(substring-after(concat(';', $prevRangeMoved), concat(';', @source, '-')), ';')"
              />
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="@source"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:when>
        <xsl:when test="e:rfmt/@sqref">
          <xsl:value-of select="substring-before(e:rfmt/@sqref, ':')"/>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>


    <xsl:variable name="tableId">
      <xsl:choose>
        <xsl:when test="./@rId">
          <xsl:value-of select="concat('ct', $positionHeader, ./@rId)"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="concat('ct', $positionHeader, position())"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="colNum">
      <xsl:call-template name="GetColNum">
        <xsl:with-param name="cell">
          <xsl:value-of select="$Cell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="rowNum">
      <xsl:call-template name="GetRowNum">
        <xsl:with-param name="cell">
          <xsl:value-of select="$Cell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="name() = 'rcc'">
        <table:cell-content-change>

          <xsl:attribute name="table:id">
            <xsl:value-of select="$tableId"/>
          </xsl:attribute>


          <table:cell-address>
            <xsl:attribute name="table:table">
              <xsl:value-of select="@sId - 1"/>
            </xsl:attribute>
            <xsl:attribute name="table:row">

              <xsl:value-of select="$rowNum - 1"/>

            </xsl:attribute>
            <xsl:attribute name="table:column">

              <xsl:value-of select="$colNum - 1"/>

            </xsl:attribute>
          </table:cell-address>


          <xsl:call-template name="InsertAuthorAndDateOfChange">
            <xsl:with-param name="userName">
              <xsl:value-of select="$userName"/>
            </xsl:with-param>
            <xsl:with-param name="dateTime">
              <xsl:value-of select="$dateTime"/>
            </xsl:with-param>
          </xsl:call-template>

          <table:previous>
            <xsl:if
              test="contains(concat(';', $previousStyle), concat(';', $rowNum, ':', $colNum, '-'))">
              <xsl:attribute name="table:id">
                <xsl:value-of
                  select="substring-before(substring-after(concat(';', $previousStyle), concat(';', $rowNum, ':', $colNum, '-')), ';')"
                />
              </xsl:attribute>
            </xsl:if>

            <table:change-track-table-cell office:value-type="string">
              <xsl:if test="e:oc/e:is/e:t">
                <text:p>
                  <xsl:value-of select="e:oc/e:is/e:t"/>
                </text:p>
              </xsl:if>
            </table:change-track-table-cell>
          </table:previous>

        </table:cell-content-change>


        <xsl:apply-templates select="following-sibling::node()[name()='rcc' or name()='rm'][1]">
          <xsl:with-param name="dateTime">
            <xsl:value-of select="$dateTime"/>
          </xsl:with-param>
          <xsl:with-param name="userName">
            <xsl:value-of select="$userName"/>
          </xsl:with-param>
          <xsl:with-param name="positionHeader">
            <xsl:value-of select="$positionHeader"/>
          </xsl:with-param>
          <xsl:with-param name="previousStyle">
            <xsl:value-of select="concat($rowNum, ':', $colNum, '-', $tableId, ';', $previousStyle)"
            />
          </xsl:with-param>
          <xsl:with-param name="prevRangeMoved">
            <xsl:value-of select="$prevRangeMoved"/>
          </xsl:with-param>
        </xsl:apply-templates>


      </xsl:when>


      <!-- Moved Cell -->

      <xsl:when test="name() = 'rm'">
        <table:movement>
          <xsl:attribute name="table:id">
            <xsl:value-of select="concat('ct', $positionHeader, position())"/>
          </xsl:attribute>

          <xsl:variable name="colNumSource">
            <xsl:call-template name="GetColNum">
              <xsl:with-param name="cell">
                <xsl:choose>
                  <xsl:when test="contains(@source, ':')">
                    <xsl:value-of select="substring-before(@source, ':')"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="@source"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:variable>

          <xsl:variable name="rowNumSource">
            <xsl:call-template name="GetRowNum">
              <xsl:with-param name="cell">
                <xsl:choose>
                  <xsl:when test="contains(@source, ':')">
                    <xsl:value-of select="substring-before(@source, ':')"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="@source"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:variable>

          <table:source-range-address>

            <xsl:choose>
              <xsl:when test="contains(@source, ':')">

                <xsl:variable name="colNumSourceDestination">
                  <xsl:call-template name="GetColNum">
                    <xsl:with-param name="cell">
                      <xsl:choose>
                        <xsl:when test="contains(@source, ':')">
                          <xsl:value-of select="substring-after(@source, ':')"/>
                        </xsl:when>
                        <xsl:otherwise>
                          <xsl:value-of select="@source"/>
                        </xsl:otherwise>
                      </xsl:choose>
                    </xsl:with-param>
                  </xsl:call-template>
                </xsl:variable>

                <xsl:variable name="rowNumSourceDestination">
                  <xsl:call-template name="GetRowNum">
                    <xsl:with-param name="cell">
                      <xsl:choose>
                        <xsl:when test="contains(@source, ':')">
                          <xsl:value-of select="substring-after(@source, ':')"/>
                        </xsl:when>
                        <xsl:otherwise>
                          <xsl:value-of select="@source"/>
                        </xsl:otherwise>
                      </xsl:choose>
                    </xsl:with-param>
                  </xsl:call-template>
                </xsl:variable>

                <xsl:attribute name="table:start-table">
                  <xsl:value-of select="@sheetId - 1"/>
                </xsl:attribute>

                <xsl:attribute name="table:start-column">
                  <xsl:value-of select="$colNumSource - 1"/>
                </xsl:attribute>

                <xsl:attribute name="table:start-row">
                  <xsl:value-of select="$rowNumSource - 1"/>
                </xsl:attribute>

                <xsl:attribute name="table:end-column">
                  <xsl:value-of select="$colNumSourceDestination - 1"/>
                </xsl:attribute>

                <xsl:attribute name="table:end-row">
                  <xsl:value-of select="$rowNumSourceDestination - 1"/>
                </xsl:attribute>

                <xsl:attribute name="table:end-table">
                  <xsl:value-of select="@sheetId - 1"/>
                </xsl:attribute>

              </xsl:when>
              <xsl:otherwise>
                <xsl:attribute name="table:column">
                  <xsl:value-of select="$colNumSource - 1"/>
                </xsl:attribute>
                <xsl:attribute name="table:row">
                  <xsl:value-of select="$rowNumSource - 1"/>
                </xsl:attribute>
                <xsl:attribute name="table:table">
                  <xsl:value-of select="@sheetId - 1"/>
                </xsl:attribute>
              </xsl:otherwise>
            </xsl:choose>

          </table:source-range-address>

          <xsl:variable name="cellDestination">
            <xsl:choose>
              <xsl:when
                test="substring-before(substring-after(concat(';', $prevRangeMoved), concat(';', @source, '-')), ';')">
                <xsl:value-of
                  select="substring-before(substring-after(concat(';', $prevRangeMoved), concat(';', @source, '-')), ';')"
                />
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="@destination"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:variable>

          <xsl:variable name="colNumDestination">
            <xsl:call-template name="GetColNum">
              <xsl:with-param name="cell">
                <xsl:choose>
                  <xsl:when test="contains($cellDestination, ':')">
                    <xsl:value-of select="substring-before($cellDestination, ':')"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="$cellDestination"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:variable>

          <xsl:variable name="rowNumDestination">
            <xsl:call-template name="GetRowNum">
              <xsl:with-param name="cell">
                <xsl:choose>
                  <xsl:when test="contains($cellDestination, ':')">
                    <xsl:value-of select="substring-before($cellDestination, ':')"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="$cellDestination"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:variable>

          <table:target-range-address>

            <xsl:choose>
              <xsl:when test="contains(@source, ':')">

                <xsl:variable name="colNumEndDestination">
                  <xsl:call-template name="GetColNum">
                    <xsl:with-param name="cell">
                      <xsl:choose>
                        <xsl:when test="contains($cellDestination, ':')">
                          <xsl:value-of select="substring-after($cellDestination, ':')"/>
                        </xsl:when>
                        <xsl:otherwise>
                          <xsl:value-of select="$cellDestination"/>
                        </xsl:otherwise>
                      </xsl:choose>
                    </xsl:with-param>
                  </xsl:call-template>
                </xsl:variable>

                <xsl:variable name="rowNumEndDestination">
                  <xsl:call-template name="GetRowNum">
                    <xsl:with-param name="cell">
                      <xsl:choose>
                        <xsl:when test="contains($cellDestination, ':')">
                          <xsl:value-of select="substring-after($cellDestination, ':')"/>
                        </xsl:when>
                        <xsl:otherwise>
                          <xsl:value-of select="$cellDestination"/>
                        </xsl:otherwise>
                      </xsl:choose>
                    </xsl:with-param>
                  </xsl:call-template>
                </xsl:variable>

                <xsl:attribute name="table:start-table">
                  <xsl:value-of select="@sourceSheetId - 1"/>
                </xsl:attribute>

                <xsl:attribute name="table:start-column">
                  <xsl:value-of select="$colNumDestination - 1"/>
                </xsl:attribute>

                <xsl:attribute name="table:start-row">
                  <xsl:value-of select="$rowNumDestination - 1"/>
                </xsl:attribute>

                <xsl:attribute name="table:end-column">
                  <xsl:value-of select="$colNumEndDestination - 1"/>
                </xsl:attribute>

                <xsl:attribute name="table:end-row">
                  <xsl:value-of select="$rowNumEndDestination - 1"/>
                </xsl:attribute>

                <xsl:attribute name="table:end-table">
                  <xsl:value-of select="@sourceSheetId - 1"/>
                </xsl:attribute>

              </xsl:when>
              <xsl:otherwise>
                <xsl:attribute name="table:column">
                  <xsl:value-of select="$colNumDestination - 1"/>
                </xsl:attribute>
                <xsl:attribute name="table:row">
                  <xsl:value-of select="$rowNumDestination - 1"/>
                </xsl:attribute>
                <xsl:attribute name="table:table">
                  <xsl:value-of select="@sourceSheetId - 1"/>
                </xsl:attribute>
              </xsl:otherwise>
            </xsl:choose>

          </table:target-range-address>

          <xsl:call-template name="InsertAuthorAndDateOfChange">
            <xsl:with-param name="userName">
              <xsl:value-of select="$userName"/>
            </xsl:with-param>
            <xsl:with-param name="dateTime">
              <xsl:value-of select="$dateTime"/>
            </xsl:with-param>
          </xsl:call-template>

        </table:movement>


        <xsl:apply-templates select="following-sibling::node()[name()='rcc' or name()='rm'][1]">
          <xsl:with-param name="dateTime">
            <xsl:value-of select="$dateTime"/>
          </xsl:with-param>
          <xsl:with-param name="userName">
            <xsl:value-of select="$userName"/>
          </xsl:with-param>
          <xsl:with-param name="positionHeader">
            <xsl:value-of select="$positionHeader"/>
          </xsl:with-param>
          <xsl:with-param name="previousStyle">
            <xsl:value-of select="concat($rowNum, ':', $colNum, '-', $tableId, ';', $previousStyle)"
            />
          </xsl:with-param>
          <xsl:with-param name="prevRangeMoved">
            <xsl:value-of
              select="concat(substring-after(substring-after(concat(';', $prevRangeMoved), concat(';', @source, '-')), ';'), substring-after(concat(';', $prevRangeMoved), concat(';', @source, '-')))"
            />
          </xsl:with-param>
        </xsl:apply-templates>

      </xsl:when>

      <xsl:when test="name() = 'rrc'">
        <xsl:choose>
          <xsl:when test="@action = 'deleteCol'">

            <table:deletion table:type="column">

              <xsl:variable name="sqref">
                <xsl:value-of select="e:rfmt/@sqref"/>
              </xsl:variable>

              <xsl:attribute name="table:position">
                <xsl:value-of select="$colNum - 1"/>
              </xsl:attribute>

              <xsl:attribute name="table:table">
                <xsl:value-of select="e:rfmt/@sheetId - 1"/>
              </xsl:attribute>

              <xsl:if
                test="not(substring-before(substring-after(concat(';', $previousStyle), concat(';', $rowNum, ':', $colNum, '-')), ';'))">
                <xsl:attribute name="table:multi-deletion-spanned">
                  <xsl:value-of select="count(following-sibling::e:rrc[e:rfmt/@sqref = $sqref]) + 1"
                  />
                </xsl:attribute>
              </xsl:if>

              <xsl:attribute name="table:id">
                <xsl:value-of select="$tableId"/>
              </xsl:attribute>

              <xsl:call-template name="InsertAuthorAndDateOfChange">
                <xsl:with-param name="userName">
                  <xsl:value-of select="$userName"/>
                </xsl:with-param>
                <xsl:with-param name="dateTime">
                  <xsl:value-of select="$dateTime"/>
                </xsl:with-param>
              </xsl:call-template>

              <xsl:if
                test="substring-before(substring-after(concat(';', $previousStyle), concat(';', $rowNum, ':', $colNum, '-')), ';')">
                <table:deletions>
                  <table:change-deletion table:id="ct1">
                    <xsl:attribute name="table:id">
                      <xsl:value-of
                        select="substring-before(substring-after(concat(';', $previousStyle), concat(';', $rowNum, ':', $colNum, '-')), ';')"
                      />
                    </xsl:attribute>
                  </table:change-deletion>
                </table:deletions>
              </xsl:if>

            </table:deletion>

            <xsl:apply-templates
              select="following-sibling::node()[name()='rcc' or name()='rm' or name()='rrc'][1]">
              <xsl:with-param name="dateTime">
                <xsl:value-of select="$dateTime"/>
              </xsl:with-param>
              <xsl:with-param name="userName">
                <xsl:value-of select="$userName"/>
              </xsl:with-param>
              <xsl:with-param name="positionHeader">
                <xsl:value-of select="$positionHeader"/>
              </xsl:with-param>
              <xsl:with-param name="previousStyle">
                <xsl:value-of
                  select="concat($rowNum, ':', $colNum, '-', $tableId, ';', $previousStyle)"/>
              </xsl:with-param>
              <xsl:with-param name="prevRangeMoved">
                <xsl:value-of select="$prevRangeMoved"/>
              </xsl:with-param>
            </xsl:apply-templates>

          </xsl:when>

          <xsl:when test="@action = 'deleteRow'">
            <table:deletion table:type="row">

              <xsl:variable name="sqref">
                <xsl:value-of select="e:rfmt/@sqref"/>
              </xsl:variable>

              <xsl:attribute name="table:position">
                <xsl:value-of select="$rowNum - 1"/>
              </xsl:attribute>

              <xsl:attribute name="table:table">
                <xsl:value-of select="e:rfmt/@sheetId - 1"/>
              </xsl:attribute>

              <xsl:if
                test="not(substring-before(substring-after(concat(';', $previousStyle), concat(';', $rowNum, ':', $colNum, '-')), ';'))">
                <xsl:attribute name="table:multi-deletion-spanned">
                  <xsl:value-of select="count(following-sibling::e:rrc[e:rfmt/@sqref = $sqref]) + 1"
                  />
                </xsl:attribute>
              </xsl:if>

              <xsl:attribute name="table:id">
                <xsl:value-of select="$tableId"/>
              </xsl:attribute>

              <xsl:call-template name="InsertAuthorAndDateOfChange">
                <xsl:with-param name="userName">
                  <xsl:value-of select="$userName"/>
                </xsl:with-param>
                <xsl:with-param name="dateTime">
                  <xsl:value-of select="$dateTime"/>
                </xsl:with-param>
              </xsl:call-template>

              <xsl:if
                test="substring-before(substring-after(concat(';', $previousStyle), concat(';', $rowNum, ':', $colNum, '-')), ';')">
                <table:deletions>
                  <table:change-deletion table:id="ct1">
                    <xsl:attribute name="table:id">
                      <xsl:value-of
                        select="substring-before(substring-after(concat(';', $previousStyle), concat(';', $rowNum, ':', $colNum, '-')), ';')"
                      />
                    </xsl:attribute>
                  </table:change-deletion>
                </table:deletions>
              </xsl:if>

            </table:deletion>

            <xsl:apply-templates
              select="following-sibling::node()[name()='rcc' or name()='rm' or name()='rrc'][1]">
              <xsl:with-param name="dateTime">
                <xsl:value-of select="$dateTime"/>
              </xsl:with-param>
              <xsl:with-param name="userName">
                <xsl:value-of select="$userName"/>
              </xsl:with-param>
              <xsl:with-param name="positionHeader">
                <xsl:value-of select="$positionHeader"/>
              </xsl:with-param>
              <xsl:with-param name="previousStyle">
                <xsl:value-of
                  select="concat($rowNum, ':', $colNum, '-', $tableId, ';', $previousStyle)"/>
              </xsl:with-param>
              <xsl:with-param name="prevRangeMoved">
                <xsl:value-of select="$prevRangeMoved"/>
              </xsl:with-param>
            </xsl:apply-templates>
          </xsl:when>

        </xsl:choose>

      </xsl:when>
      <xsl:otherwise/>
    </xsl:choose>
  </xsl:template>

  <!-- Insert Change Tracking Properties -->


  <xsl:template match="e:header" mode="SearchRangeMoved">
    <xsl:param name="revisionLogFiles"/>
    <xsl:param name="positionHeader" select="1"/>
    <xsl:param name="previousRangeMoved"/>

    <xsl:variable name="id">
      <xsl:value-of select="@r:id"/>
    </xsl:variable>

    <xsl:variable name="Target">
      <xsl:for-each
        select="key('Part', concat('xl/', substring-before($revisionLogFiles, '/'), '/_rels/', substring-after($revisionLogFiles, '/'), '.rels'))//node()[name()='Relationship']">
        <xsl:if test="./@Id=$id">
          <xsl:value-of
            select="concat('xl/', substring-before($revisionLogFiles, '/'), '/', ./@Target)"/>
        </xsl:if>
      </xsl:for-each>
    </xsl:variable>

    <xsl:variable name="prevRangeMoved">
      <xsl:for-each select="key('Part', $Target)/e:revisions">
        <xsl:if test="e:rcc|e:rm">
          <xsl:apply-templates select="node()[name()='rcc' or name()='rm'][1]"
            mode="SearchRangeMoved">
            <xsl:with-param name="positionHeader">
              <xsl:value-of select="$positionHeader"/>
            </xsl:with-param>
            <xsl:with-param name="previousRangeMoved">
              <xsl:value-of select="$previousRangeMoved"/>
            </xsl:with-param>
          </xsl:apply-templates>
        </xsl:if>
      </xsl:for-each>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="following-sibling::e:header">
        <xsl:apply-templates select="following-sibling::e:header[1]" mode="SearchRangeMoved">
          <xsl:with-param name="revisionLogFiles">
            <xsl:value-of select="$revisionLogFiles"/>
          </xsl:with-param>
          <xsl:with-param name="positionHeader">
            <xsl:value-of select="$positionHeader + 1"/>
          </xsl:with-param>
          <xsl:with-param name="previousRangeMoved">
            <xsl:choose>
              <xsl:when test="$prevRangeMoved != ''">
                <xsl:value-of select="$prevRangeMoved"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$previousRangeMoved"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>
        </xsl:apply-templates>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$previousRangeMoved"/>
      </xsl:otherwise>
    </xsl:choose>


  </xsl:template>

  <xsl:template match="e:rcc|e:rm" mode="SearchRangeMoved">
    <xsl:param name="positionHeader"/>
    <xsl:param name="previousRangeMoved"/>

    <xsl:variable name="tableId">
      <xsl:choose>
        <xsl:when test="./@rId">
          <xsl:value-of select="concat('ct', $positionHeader, ./@rId)"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="concat('ct', $positionHeader, position())"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:choose>

      <xsl:when test="name() = 'rcc'">

        <xsl:choose>
          <xsl:when test="following-sibling::node()[name()='rcc' or name()='rm'][1]">
            <xsl:apply-templates select="following-sibling::node()[name()='rcc' or name()='rm'][1]"
              mode="SearchRangeMoved">
              <xsl:with-param name="positionHeader">
                <xsl:value-of select="$positionHeader"/>
              </xsl:with-param>
              <xsl:with-param name="previousRangeMoved">
                <xsl:value-of select="$previousRangeMoved"/>
              </xsl:with-param>
            </xsl:apply-templates>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$previousRangeMoved"/>
          </xsl:otherwise>
        </xsl:choose>

      </xsl:when>

      <xsl:when test="name() = 'rm'">

        <xsl:choose>
          <xsl:when test="following-sibling::node()[name()='rcc' or name()='rm'][1]">
            <xsl:apply-templates select="following-sibling::node()[name()='rcc' or name()='rm'][1]"
              mode="SearchRangeMoved">
              <xsl:with-param name="positionHeader">
                <xsl:value-of select="$positionHeader"/>
              </xsl:with-param>
              <xsl:with-param name="previousRangeMoved">
                <xsl:call-template name="AddRangeMoved">
                  <xsl:with-param name="previousRangeMoved">
                    <xsl:value-of
                      select="concat($previousRangeMoved, @source, '-', @destination, ';')"/>
                  </xsl:with-param>
                  <xsl:with-param name="source">
                    <xsl:value-of select="@source"/>
                  </xsl:with-param>
                  <xsl:with-param name="destination">
                    <xsl:value-of select="@destination"/>
                  </xsl:with-param>
                </xsl:call-template>
                <!--xsl:value-of select="concat($previousRangeMoved, @source, '-', @destination, ';')"/-->
              </xsl:with-param>
            </xsl:apply-templates>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="AddRangeMoved">
              <xsl:with-param name="previousRangeMoved">
                <xsl:value-of select="concat($previousRangeMoved, @source, '-', @destination, ';')"
                />
              </xsl:with-param>
              <xsl:with-param name="source">
                <xsl:value-of select="@source"/>
              </xsl:with-param>
              <xsl:with-param name="destination">
                <xsl:value-of select="@destination"/>
              </xsl:with-param>
            </xsl:call-template>
            <!--xsl:value-of select="concat($previousRangeMoved, @source, '-', @destination,  ';')"/-->
          </xsl:otherwise>
        </xsl:choose>

      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$previousRangeMoved"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="AddRangeMoved">
    <xsl:param name="previousRangeMoved"/>
    <xsl:param name="Result"/>
    <xsl:param name="source"/>
    <xsl:param name="destination"/>

    <xsl:choose>
      <xsl:when test="$previousRangeMoved != ''">
        <xsl:variable name="FromCell">
          <xsl:value-of select="substring-before($previousRangeMoved, '-')"/>
        </xsl:variable>

        <xsl:variable name="ToCell">
          <xsl:value-of select="substring-before(substring-after($previousRangeMoved, '-'), ';')"/>
        </xsl:variable>

        <xsl:call-template name="AddRangeMoved">
          <xsl:with-param name="Result">
            <xsl:choose>
              <xsl:when test="$ToCell = $source">
                <xsl:value-of select="concat($Result, $FromCell, '-', $destination, ';')"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="concat($Result, $FromCell, '-', $ToCell, ';')"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>
          <xsl:with-param name="source">
            <xsl:value-of select="$source"/>
          </xsl:with-param>
          <xsl:with-param name="destination">
            <xsl:value-of select="$destination"/>
          </xsl:with-param>
          <xsl:with-param name="previousRangeMoved">
            <xsl:value-of select="substring-after($previousRangeMoved, ';')"/>
          </xsl:with-param>
        </xsl:call-template>

      </xsl:when>

      <xsl:otherwise>
        <xsl:value-of select="$Result"/>
      </xsl:otherwise>

    </xsl:choose>


  </xsl:template>

  <!-- Insert Change Tracking Properties -->
  <xsl:template match="e:rcc|e:rm" mode="SearchPreviousStyle">
    <xsl:param name="dateTime"/>
    <xsl:param name="userName"/>
    <xsl:param name="positionHeader"/>
    <xsl:param name="previousStyle"/>

    <xsl:variable name="tableId">
      <xsl:choose>
        <xsl:when test="./@rId">
          <xsl:value-of select="concat('ct', $positionHeader, ./@rId)"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="concat('ct', $positionHeader, position())"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="colNum">
      <xsl:call-template name="GetColNum">
        <xsl:with-param name="cell">
          <xsl:choose>
            <xsl:when test="e:oc/@r">
              <xsl:value-of select="e:oc/@r"/>
            </xsl:when>
            <xsl:when test="e:nc/@r">
              <xsl:value-of select="e:nc/@r"/>
            </xsl:when>
            <xsl:when test="@source">
              <xsl:value-of select="@source"/>
            </xsl:when>
          </xsl:choose>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="rowNum">
      <xsl:call-template name="GetRowNum">
        <xsl:with-param name="cell">
          <xsl:choose>
            <xsl:when test="e:oc/@r">
              <xsl:value-of select="e:oc/@r"/>
            </xsl:when>
            <xsl:when test="e:nc/@r">
              <xsl:value-of select="e:nc/@r"/>
            </xsl:when>
            <xsl:when test="@source">
              <xsl:value-of select="@source"/>
            </xsl:when>
          </xsl:choose>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="name() = 'rcc'">

        <xsl:choose>
          <xsl:when test="following-sibling::node()[name()='rcc' or name()='rm'][1]">
            <xsl:apply-templates select="following-sibling::node()[name()='rcc' or name()='rm'][1]"
              mode="SearchPreviousStyle">
              <xsl:with-param name="dateTime">
                <xsl:value-of select="$dateTime"/>
              </xsl:with-param>
              <xsl:with-param name="userName">
                <xsl:value-of select="$userName"/>
              </xsl:with-param>
              <xsl:with-param name="positionHeader">
                <xsl:value-of select="$positionHeader"/>
              </xsl:with-param>
              <xsl:with-param name="previousStyle">
                <xsl:value-of
                  select="concat($rowNum, ':', $colNum, '-', $tableId, ';', $previousStyle)"/>
              </xsl:with-param>
            </xsl:apply-templates>
          </xsl:when>
          <xsl:otherwise>
            <xsl:choose>
              <xsl:when
                test="contains(concat(';', $previousStyle), concat(';', $rowNum, ':', $colNum))">
                <xsl:value-of
                  select="concat($rowNum, ':', $colNum, '-', $tableId, ';', substring-before(concat(';', $previousStyle), concat(';', $rowNum, ':', $colNum)))"
                />
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of
                  select="concat($rowNum, ':', $colNum, '-', $tableId, ';', $previousStyle)"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:otherwise>
        </xsl:choose>

      </xsl:when>
      <xsl:when test="name() = 'rm'">

        <xsl:choose>
          <xsl:when test="following-sibling::node()[name()='rcc' or name()='rm'][1]">
            <xsl:apply-templates select="following-sibling::node()[name()='rcc' or name()='rm'][1]"
              mode="SearchPreviousStyle">
              <xsl:with-param name="dateTime">
                <xsl:value-of select="$dateTime"/>
              </xsl:with-param>
              <xsl:with-param name="userName">
                <xsl:value-of select="$userName"/>
              </xsl:with-param>
              <xsl:with-param name="positionHeader">
                <xsl:value-of select="$positionHeader"/>
              </xsl:with-param>
              <xsl:with-param name="previousStyle">
                <xsl:value-of
                  select="concat($rowNum, ':', $colNum, '-', $tableId, ';', $previousStyle)"/>
              </xsl:with-param>
            </xsl:apply-templates>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$previousStyle"/>
          </xsl:otherwise>
        </xsl:choose>

      </xsl:when>
      <xsl:otherwise/>
    </xsl:choose>
  </xsl:template>

  <!-- Insert Text "Change Tracking" -->



  <xsl:template match="e:rcc|e:rm" mode="prevRangeMoved">
    <xsl:param name="positionHeader"/>
    <xsl:param name="prevRangeMoved"/>


    <xsl:variable name="Cell">
      <xsl:choose>
        <xsl:when test="e:oc/@r">
          <xsl:value-of select="e:oc/@r"/>
        </xsl:when>
        <xsl:when test="e:nc/@r">
          <xsl:value-of select="e:nc/@r"/>
        </xsl:when>
        <xsl:when test="@source">
          <xsl:value-of select="@source"/>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="colNum">
      <xsl:call-template name="GetColNum">
        <xsl:with-param name="cell">
          <xsl:value-of select="$Cell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="rowNum">
      <xsl:call-template name="GetRowNum">
        <xsl:with-param name="cell">
          <xsl:value-of select="$Cell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="name() = 'rcc'">
        <xsl:choose>
          <xsl:when test="following-sibling::node()[name()='rcc' or name()='rm']">
            <xsl:apply-templates select="following-sibling::node()[name()='rcc' or name()='rm'][1]"
              mode="prevRangeMoved">
              <xsl:with-param name="positionHeader">
                <xsl:value-of select="$positionHeader"/>
              </xsl:with-param>
              <xsl:with-param name="prevRangeMoved">
                <xsl:value-of select="$prevRangeMoved"/>
              </xsl:with-param>
            </xsl:apply-templates>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$prevRangeMoved"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:when test="name() = 'rm'">

        <xsl:choose>
          <xsl:when test="following-sibling::node()[name()='rcc' or name()='rm']">
            <xsl:apply-templates select="following-sibling::node()[name()='rcc' or name()='rm'][1]"
              mode="prevRangeMoved">
              <xsl:with-param name="positionHeader">
                <xsl:value-of select="$positionHeader"/>
              </xsl:with-param>
              <xsl:with-param name="prevRangeMoved">
                <xsl:value-of
                  select="substring-after(substring-after(concat(';', $prevRangeMoved), concat(';', $Cell, '-')), ';')"
                />
              </xsl:with-param>
            </xsl:apply-templates>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of
              select="substring-after(substring-after(concat(';', $prevRangeMoved), concat(';', $Cell, '-')), ';')"
            />
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="InsertAuthorAndDateOfChange">
    <xsl:param name="userName"/>
    <xsl:param name="dateTime"/>

    <office:change-info>
      <dc:creator>
        <xsl:value-of select="$userName"/>
      </dc:creator>
      <dc:date>
        <xsl:value-of select="$dateTime"/>
      </dc:date>
    </office:change-info>

  </xsl:template>

  <!-- Insert Style Of Prev Delete Col -->
  <xsl:template match="e:rrc" mode="prevDelColStyle">
    <xsl:param name="positionHeader"/>
    <xsl:param name="previousDelColStyle"/>

    <xsl:variable name="tableId">
      <xsl:choose>
        <xsl:when test="./@rId">
          <xsl:value-of select="concat('ct', $positionHeader, ./@rId)"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="concat('ct', $positionHeader, position())"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="following-sibling::node()[name()='rrc'][1]">
        <xsl:apply-templates select="following-sibling::node()[name()='rrc'][1]"
          mode="prevDelColStyle">
          <xsl:with-param name="positionHeader">
            <xsl:value-of select="$positionHeader"/>
          </xsl:with-param>
          <xsl:with-param name="previousDelColStyle">
            <xsl:value-of
              select="concat(substring-before(e:rfmt/@sqref, ':'), '-', $tableId, ';', $previousDelColStyle)"
            />
          </xsl:with-param>
        </xsl:apply-templates>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$previousDelColStyle"/>
      </xsl:otherwise>
    </xsl:choose>


  </xsl:template>

</xsl:stylesheet>
