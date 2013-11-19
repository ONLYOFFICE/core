<?xml version="1.0" encoding="UTF-8"?>
<!--
  * Copyright (c) 2006, Clever Age
  * All rights reserved.
  * 
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions are met:
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
<!--
Modification Log
LogNo. |Date       |ModifiedBy   |BugNo.   |Modification                                                      |
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
RefNo-1 7-Nov-2007 Sandeep S     1802631   Modification done to fix columns shifted bug.
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
  xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns:e="http://schemas.openxmlformats.org/spreadsheetml/2006/main" 
  xmlns:oox="urn:oox"
  exclude-result-prefixes="e oox r">


  <xsl:template name="CheckIfMerge">
    <xsl:param name="colNum"/>
    <xsl:param name="rowNum"/>
    <xsl:param name="MergeCell"/>

  

      <!-- Checks if Merge Cells exist  in the document-->
      <xsl:choose>
        <xsl:when test="$MergeCell != ''">
          <xsl:call-template name="SearchmergeCell">
            <xsl:with-param name="colNum">
              <xsl:value-of select="$colNum"/>
            </xsl:with-param>
            <xsl:with-param name="rowNum">
              <xsl:value-of select="$rowNum"/>
            </xsl:with-param>
            <xsl:with-param name="MergeCell">
              <xsl:value-of select="$MergeCell"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>false</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
  
  </xsl:template>
  
  <xsl:template name="SearchmergeCell">
    <xsl:param name="colNum"/>
    <xsl:param name="rowNum"/>
    <xsl:param name="MergeCell"/>
    
    <xsl:variable name="ThisCell">
      <xsl:value-of select="substring-before($MergeCell, ';')"/>
    </xsl:variable>
    
    <xsl:variable name="StartMergeCell">
      <xsl:value-of select="substring-before($ThisCell, ':')"/>
    </xsl:variable>
    
    <xsl:variable name="EndMergeCell">
      <xsl:value-of select="substring-after($ThisCell, ':')"/>
    </xsl:variable>
    
    <xsl:variable name="StartColNum">
      <xsl:call-template name="GetColNum">
        <xsl:with-param name="cell">
          <xsl:value-of select="$StartMergeCell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>
    
    <xsl:variable name="StartRowNum">
      <xsl:call-template name="GetRowNum">
        <xsl:with-param name="cell">
          <xsl:value-of select="$StartMergeCell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="EndColNumTmp">
      <xsl:call-template name="GetColNum">
        <xsl:with-param name="cell">
          <xsl:value-of select="$EndMergeCell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>
    
    <xsl:variable name="EndColNum">
      <xsl:choose>
        <xsl:when test="$EndColNumTmp != -1">
          <xsl:value-of select="$EndColNumTmp"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$StartColNum"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    
    <xsl:variable name="EndRowNumTmp">
      <xsl:call-template name="GetRowNum">
        <xsl:with-param name="cell">
          <xsl:value-of select="$EndMergeCell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="EndRowNum">
      <xsl:choose>
        <xsl:when test="$EndRowNumTmp != -1">
          <xsl:value-of select="$EndRowNumTmp"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$StartRowNum"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    
    <xsl:choose>
      
      <!-- Checks if "Merge Cell" is starting in this cell -->
      <xsl:when test="$colNum = $StartColNum and $rowNum = $StartRowNum">
        <xsl:value-of
          select="concat(number($EndRowNum - $StartRowNum + 1), concat(':',number($EndColNum - $StartColNum+1)))"
        />
      </xsl:when>
      
      <!-- Checks if this cell is in "Merge Cell" -->
      <xsl:when
        test="$colNum = $StartColNum and $EndColNum &gt;= $colNum and $rowNum &gt;= $StartRowNum and $EndRowNum &gt;= $rowNum">
        <xsl:value-of select="concat('true:', number($EndColNum - $StartColNum +1 ))"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:choose>
          <xsl:when test="substring-after($MergeCell, ';') != ''">
            <xsl:call-template name="SearchmergeCell">
            <xsl:with-param name="colNum">
              <xsl:value-of select="$colNum"/>
            </xsl:with-param>
            <xsl:with-param name="rowNum">
              <xsl:value-of select="$rowNum"/>
            </xsl:with-param>
              <xsl:with-param name="MergeCell">
                <xsl:value-of select="substring-after($MergeCell, ';')"/>
              </xsl:with-param>
            </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>false</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>
    
  </xsl:template>

  <xsl:template name="CheckIfBigMerge">
    <xsl:param name="colNum"/>
    <xsl:param name="BigMergeCell"/>
    <xsl:param name="Result"/>


    <xsl:choose>
      <xsl:when test="$BigMergeCell != ''">

        <!-- Checks if Merge Cells exist  in the document-->
        <xsl:variable name="StartBigMerge">
          <xsl:value-of select="substring-before($BigMergeCell, ':')"/>
        </xsl:variable>
        <xsl:variable name="EndBigMerge">
          <xsl:value-of select="substring-after(substring-before($BigMergeCell, ';'), ':')"/>
        </xsl:variable>

        <xsl:choose>
          <xsl:when test="$colNum = $StartBigMerge ">
            <xsl:value-of select="concat(concat($StartBigMerge, ':'), $EndBigMerge)"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="CheckIfBigMerge">
              <xsl:with-param name="colNum">
                <xsl:value-of select="$colNum"/>
              </xsl:with-param>
              <xsl:with-param name="BigMergeCell">
                <xsl:value-of select="substring-after($BigMergeCell, ';')"/>
              </xsl:with-param>
              <xsl:with-param name="Result">
                <xsl:choose>
                  <xsl:when test="$Result = '' and $StartBigMerge &gt; $colNum">
                    <xsl:value-of select="concat(concat($StartBigMerge, ':'), $EndBigMerge)"/>
                  </xsl:when>
                  <xsl:when
                    test="number(substring-before($Result, ':')) &gt; $StartBigMerge and $StartBigMerge &gt; $colNum">
                    <xsl:value-of select="concat(concat($StartBigMerge, ':'), $EndBigMerge)"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="$Result"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$Result"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="CheckIfBigMergeRow">
    <xsl:param name="RowNum"/>
    <xsl:param name="BigMergeCellRow"/>
    <xsl:param name="Result"/>


    <xsl:choose>
      <xsl:when test="$BigMergeCellRow != ''">

        <!-- Checks if Merge Cells exist  in the document-->
        <xsl:variable name="StartBigMerge">
          <xsl:value-of select="substring-before($BigMergeCellRow, ':')"/>
        </xsl:variable>
        <xsl:variable name="EndBigMerge">
          <xsl:value-of select="substring-after(substring-before($BigMergeCellRow, ';'), ':')"/>
        </xsl:variable>

        <xsl:choose>
          <xsl:when test="$RowNum = $StartBigMerge ">
            <xsl:value-of select="concat(concat($StartBigMerge, ':'), $EndBigMerge)"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="CheckIfBigMergeRow">
              <xsl:with-param name="RowNum">
                <xsl:value-of select="$RowNum"/>
              </xsl:with-param>
              <xsl:with-param name="BigMergeCellRow">
                <xsl:value-of select="substring-after($BigMergeCellRow, ';')"/>
              </xsl:with-param>
              <xsl:with-param name="Result">
                <xsl:choose>
                  <xsl:when test="$EndBigMerge &gt; $RowNum and $RowNum &gt; $StartBigMerge">
                    <xsl:text>true</xsl:text>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="$Result"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$Result"/>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <!-- Check If there are Big Merge Coll before this cell -->

  <xsl:template name="CheckIfBigMergeBefore">
    <xsl:param name="prevCellCol"/>
    <xsl:param name="colNum"/>
    <xsl:param name="rowNum"/>
    <xsl:param name="BigMergeCell"/>

    <xsl:variable name="CheckIfMerge">
      <xsl:value-of select="concat(substring-before($BigMergeCell, ';'), ';')"/>
    </xsl:variable>

    <xsl:choose>

      <!-- when there is big merge coll before this cell -->
      <xsl:when
        test="$prevCellCol != '' and $colNum &gt; substring-before($BigMergeCell, ':') and substring-before($CheckIfMerge, ':') &gt; $prevCellCol">
        <xsl:text>true</xsl:text>
      </xsl:when>
      <xsl:when test="$prevCellCol = '' and $colNum &gt; substring-before($BigMergeCell, ':')">
        <xsl:text>true</xsl:text>
      </xsl:when>
      <!-- when in this cell starting big merge coll -->
      <xsl:when test="$rowNum = '1' and $colNum = substring-before($BigMergeCell, ':')">
        <xsl:text>start</xsl:text>
      </xsl:when>

      <xsl:when test="substring-after($BigMergeCell, ';') != ''">
        <xsl:call-template name="CheckIfBigMergeBefore">
          <xsl:with-param name="prevCellCol">
            <xsl:value-of select="$prevCellCol"/>
          </xsl:with-param>
          <xsl:with-param name="colNum">
            <xsl:value-of select="$colNum"/>
          </xsl:with-param>
          <xsl:with-param name="rowNum">
            <xsl:value-of select="$rowNum"/>
          </xsl:with-param>
          <xsl:with-param name="BigMergeCell">
            <xsl:value-of select="substring-after($BigMergeCell, ';')"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>

      <xsl:otherwise>
        <xsl:text>false</xsl:text>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <!-- Check If there are Big Merge Coll after this cell -->

  <xsl:template name="CheckIfBigMergeAfter">
    <xsl:param name="colNum"/>
    <xsl:param name="rowNum"/>
    <xsl:param name="BigMergeCell"/>

    <xsl:variable name="CheckIfMerge">
      <xsl:value-of select="concat(substring-before($BigMergeCell, ';'), ';')"/>
    </xsl:variable>

    <xsl:choose>

      <!-- when there is big merge coll before this cell -->
      <xsl:when test="substring-before($CheckIfMerge, ':') &gt; $colNum">
        <xsl:text>true</xsl:text>
      </xsl:when>
      <xsl:when test="substring-after($BigMergeCell, ';') != ''">
        <xsl:call-template name="CheckIfBigMergeAfter">
          <xsl:with-param name="colNum">
            <xsl:value-of select="$colNum"/>
          </xsl:with-param>
          <xsl:with-param name="rowNum">
            <xsl:value-of select="$rowNum"/>
          </xsl:with-param>
          <xsl:with-param name="BigMergeCell">
            <xsl:value-of select="substring-after($BigMergeCell, ';')"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:text>false</xsl:text>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <!-- Insert If there are Big Merge Coll before this cell -->
  <xsl:template name="InsertBigMergeBefore">
    <xsl:param name="prevCellCol"/>
    <xsl:param name="colNum"/>
    <xsl:param name="rowNum"/>
    <xsl:param name="BigMergeCell"/>

    <xsl:variable name="CheckIfMerge">
      <xsl:value-of select="concat(substring-before($BigMergeCell, ';'), ';')"/>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="$prevCellCol = '' and $colNum &gt; substring-before($BigMergeCell, ':')">
        <xsl:call-template name="InsertColumnsBigMergeColl">
          <xsl:with-param name="CollNumber">
            <xsl:choose>
              <xsl:when test="$prevCellCol != ''">
                <xsl:value-of select="$prevCellCol"/>
              </xsl:when>
              <xsl:otherwise>1</xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>
          <xsl:with-param name="BigMergeCell">
            <xsl:value-of select="$CheckIfMerge"/>
          </xsl:with-param>
          <xsl:with-param name="RowNumber">
            <xsl:value-of select="$rowNum"/>
          </xsl:with-param>
          <xsl:with-param name="EndCell">
            <xsl:value-of select="$colNum"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>

      <xsl:when
        test="$prevCellCol != '' and $colNum &gt; substring-before($BigMergeCell, ':') and substring-before($BigMergeCell, ':') &gt; $prevCellCol">
        <xsl:call-template name="InsertColumnsBigMergeColl">
          <xsl:with-param name="CollNumber">
            <xsl:choose>
              <xsl:when test="$prevCellCol != ''">
                <xsl:value-of select="$prevCellCol"/>
              </xsl:when>
              <xsl:otherwise>1</xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>
          <xsl:with-param name="BigMergeCell">
            <xsl:value-of select="$CheckIfMerge"/>
          </xsl:with-param>
          <xsl:with-param name="RowNumber">
            <xsl:value-of select="$rowNum"/>
          </xsl:with-param>
          <xsl:with-param name="EndCell">
            <xsl:value-of select="$colNum"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
    </xsl:choose>

    <xsl:if test="substring-after($BigMergeCell, ';') != ''">
      <xsl:call-template name="InsertBigMergeBefore">
        <xsl:with-param name="prevCellCol">
          <xsl:value-of select="$prevCellCol"/>
        </xsl:with-param>
        <xsl:with-param name="colNum">
          <xsl:value-of select="$colNum"/>
        </xsl:with-param>
        <xsl:with-param name="rowNum">
          <xsl:value-of select="$rowNum"/>
        </xsl:with-param>
        <xsl:with-param name="BigMergeCell">
          <xsl:value-of select="substring-after($BigMergeCell, ';')"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:if>

  </xsl:template>

  <xsl:template match="e:mergeCell" mode="merge">    
    <xsl:param name="result"/>

    <xsl:choose>
      <xsl:when test="following-sibling::e:mergeCell[1]">
        <xsl:apply-templates select="following-sibling::e:mergeCell[1]" mode="merge">        
          <xsl:with-param name="result">
            <xsl:value-of select="concat($result, @ref, ';')"/>
          </xsl:with-param>
        </xsl:apply-templates>    
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="concat($result, @ref, ';')"/>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>



  <xsl:template match="e:mergeCell" mode="BigMergeColl">
    <xsl:param name="sheet"/>
    <xsl:param name="RefValue"/>

    <xsl:variable name="StartMergeCell">
      <xsl:value-of select="substring-before(@ref, ':')"/>
    </xsl:variable>

    <xsl:variable name="EndMergeCell">
      <xsl:value-of select="substring-after(@ref, ':')"/>
    </xsl:variable>

    <xsl:variable name="StartColNum">
      <xsl:call-template name="GetColNum">
        <xsl:with-param name="cell">
          <xsl:value-of select="$StartMergeCell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="EndColNum">
      <xsl:call-template name="GetColNum">
        <xsl:with-param name="cell">
          <xsl:value-of select="$EndMergeCell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="StartRowNum">
      <xsl:call-template name="GetRowNum">
        <xsl:with-param name="cell">
          <xsl:value-of select="$StartMergeCell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="EndRowNum">
      <xsl:call-template name="GetRowNum">
        <xsl:with-param name="cell">
          <xsl:value-of select="$EndMergeCell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="following-sibling::e:mergeCell[1]">
        <xsl:apply-templates select="following-sibling::e:mergeCell[1]" mode="BigMergeColl">
          <xsl:with-param name="sheet">
            <xsl:value-of select="$sheet"/>
          </xsl:with-param>
          <xsl:with-param name="RefValue">
            <xsl:choose>
              <xsl:when test="$StartRowNum = '1' and $EndRowNum = '1048576'">
                <xsl:value-of
                  select="concat($RefValue, concat($StartColNum, concat(':', concat($EndColNum, ';'))))"
                />
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$RefValue"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>
        </xsl:apply-templates>
      </xsl:when>
      <xsl:otherwise>
        <xsl:choose>
          <xsl:when test="$StartRowNum = '1' and $EndRowNum = '1048576'">
            <xsl:value-of
              select="concat($RefValue, concat($StartColNum, concat(':', concat($EndColNum, ';'))))"
            />
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$RefValue"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>


  <xsl:template match="e:mergeCell" mode="BigMergeRow">
    <xsl:param name="sheet"/>
    <xsl:param name="RefValue"/>


    <xsl:variable name="StartMergeCell">
      <xsl:value-of select="substring-before(@ref, ':')"/>
    </xsl:variable>

    <xsl:variable name="EndMergeCell">
      <xsl:value-of select="substring-after(@ref, ':')"/>
    </xsl:variable>

    <xsl:variable name="StartColNum">
      <xsl:call-template name="GetColNum">
        <xsl:with-param name="cell">
          <xsl:value-of select="$StartMergeCell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="EndColNum">
      <xsl:call-template name="GetColNum">
        <xsl:with-param name="cell">
          <xsl:value-of select="$EndMergeCell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="StartRowNum">
      <xsl:call-template name="GetRowNum">
        <xsl:with-param name="cell">
          <xsl:value-of select="$StartMergeCell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="EndRowNum">
      <xsl:call-template name="GetRowNum">
        <xsl:with-param name="cell">
          <xsl:value-of select="$EndMergeCell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="following-sibling::e:mergeCell[1]">
        <xsl:apply-templates select="following-sibling::e:mergeCell[1]" mode="BigMergeRow">
          <xsl:with-param name="sheet">
            <xsl:value-of select="$sheet"/>
          </xsl:with-param>
          <xsl:with-param name="RefValue">
            <xsl:choose>
              <xsl:when test="$StartColNum = 1 and $EndColNum = 16384">
                <xsl:value-of
                  select="concat($RefValue, concat($StartRowNum, concat(':', concat($EndRowNum, ';'))))"
                />
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$RefValue"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>
        </xsl:apply-templates>
      </xsl:when>
      <xsl:otherwise>
        <xsl:choose>
          <xsl:when test="$StartColNum = 1 and $EndColNum = 16384">
            <xsl:value-of
              select="concat($RefValue, concat($StartRowNum, concat(':', concat($EndRowNum, ';'))))"
            />
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$RefValue"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- Insert Empty Rows with Big Merge Coll -->
  <xsl:template name="InsertColumnsBigMergeRow">
    <xsl:param name="RowNumber"/>
    <xsl:param name="StartNumber"/>
    <xsl:param name="Repeat"/>
    <xsl:param name="BigMergeCell"/>

    <table:table-row table:style-name="{concat('ro', ancestor::e:worksheet/@oox:part)}">
      <xsl:call-template name="InsertColumnsBigMergeColl">
        <xsl:with-param name="BigMergeCell">
          <xsl:value-of select="$BigMergeCell"/>
        </xsl:with-param>
        <xsl:with-param name="RowNumber">
          <xsl:value-of select="$RowNumber"/>
        </xsl:with-param>
      </xsl:call-template>
    </table:table-row>

    <xsl:if test="$Repeat &gt; $RowNumber">
      <xsl:call-template name="InsertColumnsBigMergeRow">
        <xsl:with-param name="RowNumber">
          <xsl:value-of select="$RowNumber + 1"/>
        </xsl:with-param>
        <xsl:with-param name="Repeat">
          <xsl:value-of select="$Repeat"/>
        </xsl:with-param>
        <xsl:with-param name="BigMergeCell">
          <xsl:value-of select="$BigMergeCell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

  <!-- Insert Big Merge Coll when sheet is empty -->

  <xsl:template name="InsertColumnsBigMergeColl">
    <xsl:param name="CollNumber" select="1"/>
    <xsl:param name="BigMergeCell"/>
    <xsl:param name="RowNumber"/>
    <xsl:param name="EndCell"/>

    <xsl:if
      test="($EndCell != '' and $EndCell &gt; $CollNumber) or ($EndCell = '' and $CollNumber &lt; 257)">

      <xsl:choose>
        <xsl:when test="$BigMergeCell != '' and $RowNumber = 1">

          <xsl:variable name="MergeCell">
            <xsl:call-template name="CheckIfBigMerge">
              <xsl:with-param name="colNum">
                <xsl:value-of select="$CollNumber"/>
              </xsl:with-param>
              <xsl:with-param name="BigMergeCell">
                <xsl:value-of select="$BigMergeCell"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:variable>

          <xsl:choose>
            <!-- Insert empty cell  Before Big Merge Column-->
            <xsl:when test="number(substring-before($MergeCell, ':')) &gt; $CollNumber">
              <table:table-cell>
                <xsl:attribute name="table:number-columns-repeated">
                  <xsl:choose>
                    <xsl:when
                      test="$EndCell != '' and number(substring-before($MergeCell, ':')) &gt; $EndCell and $RowNumber = 1">
                      <xsl:value-of select="number($EndCell) - number($CollNumber) - 1"/>
                    </xsl:when>
                    <xsl:when
                      test="$EndCell != '' and number(substring-before($MergeCell, ':')) &gt; $EndCell and $RowNumber != 1">
                      <xsl:value-of select="number($EndCell) - number($CollNumber)"/>
                    </xsl:when>
                    <xsl:when test="number(substring-before($MergeCell, ':')) &gt; 256">
                      <xsl:value-of select="256 - number($CollNumber)"/>
                    </xsl:when>
                    <xsl:otherwise>
                      <!-- When Sheet is Empty -->
                      <xsl:choose>
                        <xsl:when test="$CollNumber = 1">
                          <xsl:value-of
                            select="number(substring-before($MergeCell, ':')) - number($CollNumber) "
                          />
                        </xsl:when>
                        <xsl:otherwise>
                          <xsl:value-of
                            select="number(substring-before($MergeCell, ':')) - number($CollNumber) - 1"
                          />
                        </xsl:otherwise>
                      </xsl:choose>
                    </xsl:otherwise>
                  </xsl:choose>
                </xsl:attribute>
              </table:table-cell>
              <xsl:call-template name="InsertColumnsBigMergeColl">
                <xsl:with-param name="CollNumber">
                  <xsl:value-of select="number(substring-before($MergeCell, ':'))"/>
                </xsl:with-param>
                <xsl:with-param name="BigMergeCell">
                  <xsl:value-of select="$BigMergeCell"/>
                </xsl:with-param>
                <xsl:with-param name="RowNumber">
                  <xsl:value-of select="$RowNumber"/>
                </xsl:with-param>
                <xsl:with-param name="EndCell">
                  <xsl:value-of select="$EndCell"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:when>

            <xsl:otherwise>
              <xsl:choose>
                <!-- Insert Big Merge Coll  in first row-->
                <xsl:when test="$RowNumber = '1'">
                  <xsl:variable name="ColumnsSpanned">
                    <xsl:value-of
                      select="number(substring-after($MergeCell, ':')) - number(substring-before($MergeCell, ':')) + 1"
                    />
                  </xsl:variable>
                  <!-- Insert First Cell where big merge coll is starting -->
                  <xsl:if test="$ColumnsSpanned != 'NaN' ">

                    <table:table-cell>
                      <xsl:attribute name="table:number-rows-spanned">
                        <xsl:value-of select="65536"/>
                      </xsl:attribute>
                      <xsl:attribute name="table:number-columns-spanned">
                        <xsl:choose>
                          <xsl:when
                            test="$EndCell != '' and number(substring-after($MergeCell, ':')) &gt; $EndCell">
                            <xsl:value-of
                              select="number($EndCell) - number(substring-before($MergeCell, ':')) + 1"
                            />
                          </xsl:when>
                          <xsl:when test="number(substring-after($MergeCell, ':')) &gt; 256">
                            <xsl:value-of
                              select="256 - number(substring-before($MergeCell, ':')) + 1"/>
                          </xsl:when>
                          <xsl:otherwise>
                            <xsl:value-of
                              select="number(substring-after($MergeCell, ':')) - number(substring-before($MergeCell, ':')) + 1"
                            />
                          </xsl:otherwise>
                        </xsl:choose>
                      </xsl:attribute>
                    </table:table-cell>

                  </xsl:if>
                  <!-- Insert covered-table-cell -->
                  <xsl:choose>
                    <xsl:when
                      test="number(substring-after($MergeCell, ':')) - number(substring-before($MergeCell, ':')) = 1">
                      <table:covered-table-cell/>
                      <xsl:call-template name="InsertColumnsBigMergeColl">
                        <xsl:with-param name="CollNumber">
                          <xsl:value-of select="$CollNumber + 2"/>
                        </xsl:with-param>
                        <xsl:with-param name="BigMergeCell">
                          <xsl:value-of
                            select="concat(substring-before($BigMergeCell, $MergeCell), substring-after($BigMergeCell, concat($MergeCell, ';')))"
                          />
                        </xsl:with-param>
                        <xsl:with-param name="RowNumber">
                          <xsl:value-of select="$RowNumber"/>
                        </xsl:with-param>
                        <xsl:with-param name="EndCell">
                          <xsl:value-of select="$EndCell"/>
                        </xsl:with-param>
                      </xsl:call-template>
                    </xsl:when>

                    <xsl:when
                      test="number(substring-after($MergeCell, ':')) - number(substring-before($MergeCell, ':')) &gt; 1">
                      <table:covered-table-cell>
                        <xsl:choose>
                          <xsl:when
                            test="number(substring-after($MergeCell, ':')) &gt; number(substring-before($MergeCell, ':'))">
                            <xsl:attribute name="table:number-columns-repeated">
                              <xsl:choose>
                                <xsl:when
                                  test="$EndCell != '' and number(substring-after($MergeCell, ':')) &gt; $EndCell">
                                  <xsl:value-of
                                    select="number($EndCell) - number(substring-before($MergeCell, ':'))"
                                  />
                                </xsl:when>
                                <xsl:when
                                  test="number(substring-after($MergeCell, ':')) &gt; 256">
                                  <xsl:value-of
                                    select="256 - number(substring-before($MergeCell, ':'))"/>
                                </xsl:when>
                                <xsl:otherwise>
                                  <xsl:value-of
                                    select="number(substring-after($MergeCell, ':')) - number(substring-before($MergeCell, ':'))"
                                  />
                                </xsl:otherwise>
                              </xsl:choose>
                            </xsl:attribute>
                          </xsl:when>
                          <xsl:otherwise>
                            <xsl:call-template name="InsertColumnsBigMergeColl">
                              <xsl:with-param name="CollNumber">
                                <xsl:value-of select="$CollNumber + 1"/>
                              </xsl:with-param>
                              <xsl:with-param name="BigMergeCell">
                                <xsl:value-of
                                  select="concat(substring-before($BigMergeCell, $MergeCell), substring-after($BigMergeCell, concat($MergeCell, ';')))"
                                />
                              </xsl:with-param>
                              <xsl:with-param name="RowNumber">
                                <xsl:value-of select="$RowNumber"/>
                              </xsl:with-param>
                              <xsl:with-param name="EndCell">
                                <xsl:value-of select="$EndCell"/>
                              </xsl:with-param>
                            </xsl:call-template>
                          </xsl:otherwise>
                        </xsl:choose>
                      </table:covered-table-cell>

                      <xsl:call-template name="InsertColumnsBigMergeColl">
                        <xsl:with-param name="CollNumber">
                          <xsl:value-of
                            select="$CollNumber + number(substring-after($MergeCell, ':')) - number(substring-before($MergeCell, ':')) + 1"
                          />
                        </xsl:with-param>
                        <xsl:with-param name="BigMergeCell">
                          <xsl:value-of
                            select="concat(substring-before($BigMergeCell, $MergeCell), substring-after($BigMergeCell, concat($MergeCell, ';')))"
                          />
                        </xsl:with-param>
                        <xsl:with-param name="RowNumber">
                          <xsl:value-of select="$RowNumber"/>
                        </xsl:with-param>
                        <xsl:with-param name="EndCell">
                          <xsl:value-of select="$EndCell"/>
                        </xsl:with-param>
                      </xsl:call-template>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:call-template name="InsertColumnsBigMergeColl">
                        <xsl:with-param name="CollNumber">
                          <xsl:variable name="NextCell">
                            <xsl:value-of
                              select="$CollNumber + number(substring-after($MergeCell, ':')) - number(substring-before($MergeCell, ':')) + 1"
                            />
                          </xsl:variable>
                          <xsl:choose>
                            <xsl:when test="contains($BigMergeCell, concat($NextCell, ':'))">
                              <xsl:value-of select="$NextCell"/>
                            </xsl:when>
                            <xsl:otherwise>
                              <xsl:value-of
                                select="$CollNumber + number(substring-after($MergeCell, ':')) - number(substring-before($MergeCell, ':')) + 1"
                              />
                            </xsl:otherwise>
                          </xsl:choose>
                        </xsl:with-param>
                        <xsl:with-param name="BigMergeCell">
                          <xsl:value-of
                            select="concat(substring-before($BigMergeCell, $MergeCell), substring-after($BigMergeCell, concat($MergeCell, ';')))"
                          />
                        </xsl:with-param>
                        <xsl:with-param name="RowNumber">
                          <xsl:value-of select="$RowNumber"/>
                        </xsl:with-param>
                        <xsl:with-param name="EndCell">
                          <xsl:value-of select="$EndCell"/>
                        </xsl:with-param>
                      </xsl:call-template>
                    </xsl:otherwise>
                  </xsl:choose>
                </xsl:when>


                <xsl:otherwise>
                  <xsl:if test="substring-after($MergeCell, ':') &gt; $CollNumber">
                    <table:covered-table-cell>
                      <xsl:choose>
                        <xsl:when
                          test="number(substring-after($MergeCell, ':')) &gt; number(substring-before($MergeCell, ':'))">
                          <xsl:attribute name="table:number-columns-repeated">
                            <xsl:choose>
                              <xsl:when
                                test="$EndCell != '' and number(substring-after($MergeCell, ':')) &gt; $EndCell">
                                <xsl:value-of
                                  select="number($EndCell) - number(substring-before($MergeCell, ':')) + 1"
                                />
                              </xsl:when>
                              <xsl:when test="number(substring-after($MergeCell, ':')) &gt; 256">
                                <xsl:value-of
                                  select="256 - number(substring-before($MergeCell, ':')) + 1"/>
                              </xsl:when>
                              <xsl:otherwise>
                                <xsl:value-of
                                  select="number(substring-after($MergeCell, ':')) - number(substring-before($MergeCell, ':')) + 1"
                                />
                              </xsl:otherwise>
                            </xsl:choose>
                          </xsl:attribute>
                        </xsl:when>
                        <xsl:otherwise/>
                      </xsl:choose>
                    </table:covered-table-cell>
                  </xsl:if>
                </xsl:otherwise>
              </xsl:choose>

            </xsl:otherwise>

          </xsl:choose>
        </xsl:when>
        <xsl:otherwise>

          <xsl:choose>
            <xsl:when test="$EndCell != '' and $CollNumber &lt; $EndCell">

              <table:table-cell>
                <xsl:choose>
                  <xsl:when test="$EndCell - $CollNumber &gt; 1">
                    <xsl:attribute name="table:number-columns-repeated">
                      <xsl:value-of select="$EndCell - $CollNumber"/>
                    </xsl:attribute>
                  </xsl:when>
                  <xsl:otherwise/>
                </xsl:choose>
              </table:table-cell>
            </xsl:when>
            <xsl:when test="$EndCell = '' and $CollNumber &lt; 256">
              <table:table-cell>
                <xsl:choose>
                  <xsl:when test="256 - $CollNumber &gt; 1">
                    <xsl:attribute name="table:number-columns-repeated">
                      <xsl:value-of select="256 - $CollNumber + 1"/>
                    </xsl:attribute>
                  </xsl:when>
                  <xsl:otherwise/>
                </xsl:choose>
              </table:table-cell>
            </xsl:when>
          </xsl:choose>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:if>
  </xsl:template>

  <!-- Input smallest Big Merge Coll -->
  <xsl:template name="InsertSmallestBigMergeColl">
    <xsl:param name="BigMergeColl"/>
    <xsl:param name="result"/>

    <xsl:variable name="FirstMerge">
      <xsl:value-of select="substring-before($BigMergeColl, ';')"/>
    </xsl:variable>

    <xsl:choose>
      <xsl:when
        test="$result != '' and $FirstMerge != '' and substring-before($result, ':') &gt; substring-before($FirstMerge, ':')">
        <xsl:call-template name="InsertSmallestBigMergeColl">
          <xsl:with-param name="BigMergeColl">
            <xsl:value-of select="substring-after($BigMergeColl, ';')"/>
          </xsl:with-param>
          <xsl:with-param name="result">
            <xsl:value-of select="$FirstMerge"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>

      <xsl:when
        test="$result != '' and $FirstMerge != '' and substring-before($FirstMerge, ':') &gt; substring-before($result, ':')">
        <xsl:call-template name="InsertSmallestBigMergeColl">
          <xsl:with-param name="BigMergeColl">
            <xsl:value-of select="substring-after($BigMergeColl, ';')"/>
          </xsl:with-param>
          <xsl:with-param name="result">
            <xsl:value-of select="$result"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>

      <xsl:when test="$result = '' and $FirstMerge != ''">
        <xsl:call-template name="InsertSmallestBigMergeColl">
          <xsl:with-param name="BigMergeColl">
            <xsl:value-of select="substring-after($BigMergeColl, ';')"/>
          </xsl:with-param>
          <xsl:with-param name="result">
            <xsl:value-of select="$FirstMerge"/>
          </xsl:with-param>
        </xsl:call-template>

      </xsl:when>

      <xsl:otherwise>
        <xsl:value-of select="$result"/>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <!-- Insert Big Merge Coll in empty first row when sheet is not empty -->

  <xsl:template name="InsertEmptyColumnsBigMergeCollSheetNotEmptyInFirstRow">
    <xsl:param name="CollNumber" select="1"/>
    <xsl:param name="BigMergeCell"/>
    <xsl:param name="RowNumber"/>


    <xsl:variable name="CheckMerge">
      <xsl:call-template name="InsertSmallestBigMergeColl">
        <xsl:with-param name="BigMergeColl">
          <xsl:value-of select="$BigMergeCell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="$CollNumber &lt; substring-before($CheckMerge, ':')">
        <table:table-cell>
          <xsl:attribute name="table:number-columns-repeated">
            <xsl:value-of select="substring-before($CheckMerge, ':') - $CollNumber"/>
          </xsl:attribute>
        </table:table-cell>

        <xsl:call-template name="InsertEmptyColumnsBigMergeCollSheetNotEmptyInFirstRow">
          <xsl:with-param name="CollNumber">
            <xsl:value-of select="substring-before($CheckMerge, ':')"/>
          </xsl:with-param>
          <xsl:with-param name="BigMergeCell">
            <xsl:value-of select="$BigMergeCell"/>
          </xsl:with-param>
          <xsl:with-param name="RowNumber">
            <xsl:value-of select="$RowNumber"/>
          </xsl:with-param>
        </xsl:call-template>

      </xsl:when>

      <!-- when in this cell big merge coll is starting -->
      <xsl:when test="$CollNumber = substring-before($CheckMerge, ':')">

        <table:table-cell>
          <xsl:attribute name="table:number-rows-spanned">
            <xsl:value-of select="65536"/>
          </xsl:attribute>
          <xsl:attribute name="table:number-columns-spanned">
            <xsl:value-of
              select="substring-after($CheckMerge, ':') - substring-before($CheckMerge, ':') + 1"/>
          </xsl:attribute>
        </table:table-cell>

        <xsl:if
          test="substring-after($CheckMerge, ':') - substring-before($CheckMerge, ':') &gt;= 1">
          <table:covered-table-cell>
            <xsl:attribute name="table:number-columns-repeated">
              <xsl:value-of
                select="substring-after($CheckMerge, ':') - substring-before($CheckMerge, ':')"/>
            </xsl:attribute>
          </table:covered-table-cell>
        </xsl:if>
        <xsl:call-template name="InsertEmptyColumnsBigMergeCollSheetNotEmptyInFirstRow">
          <xsl:with-param name="CollNumber">
            <xsl:value-of select="number(substring-after($CheckMerge, ':')) + 1"/>
          </xsl:with-param>
          <xsl:with-param name="BigMergeCell">
            <xsl:value-of
              select="concat(substring-before($BigMergeCell, concat($CheckMerge, ';')), substring-after($BigMergeCell, concat($CheckMerge, ';')))"
            />
          </xsl:with-param>
          <xsl:with-param name="RowNumber">
            <xsl:value-of select="$RowNumber"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <table:table-cell>
          <xsl:choose>
            <xsl:when test="256 - $CollNumber &gt; 1">
              <xsl:attribute name="table:number-columns-repeated">
                <xsl:value-of select="256 - $CollNumber + 1"/>
              </xsl:attribute>
            </xsl:when>
            <xsl:otherwise/>
          </xsl:choose>
        </table:table-cell>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <!-- When there are big merge coll and sheet isn't empty -->
  <xsl:template name="InsertBigMergeFirstRowEmpty">
    <xsl:param name="BigMergeCell"/>
    <xsl:param name="RowNumber"/>

    <xsl:choose>
      <xsl:when test="$RowNumber = 1">
        <table:table-row>
          <xsl:call-template name="InsertEmptyColumnsBigMergeCollSheetNotEmptyInFirstRow">
            <xsl:with-param name="CollNumber">1</xsl:with-param>
            <xsl:with-param name="BigMergeCell">
              <xsl:value-of select="$BigMergeCell"/>
            </xsl:with-param>
            <xsl:with-param name="RowNumber">1</xsl:with-param>
          </xsl:call-template>
        </table:table-row>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- Insert Big Merge Coll in empty row (no first row) when sheet is not empty -->

  <xsl:template name="InsertEmptyColumnsBigMergeCollSheetNotEmpty">
    <xsl:param name="CollNumber" select="1"/>
    <xsl:param name="BigMergeCell"/>
    <xsl:variable name="CheckMerge">
      <xsl:call-template name="InsertSmallestBigMergeColl">
        <xsl:with-param name="BigMergeColl">
          <xsl:value-of select="$BigMergeCell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>

      <xsl:when test="$CollNumber &lt; substring-before($CheckMerge, ':')">
        <table:table-cell>
          <xsl:attribute name="table:number-columns-repeated">
            <xsl:value-of select="substring-before($CheckMerge, ':') - $CollNumber"/>
          </xsl:attribute>
        </table:table-cell>

        <xsl:call-template name="InsertEmptyColumnsBigMergeCollSheetNotEmpty">
          <xsl:with-param name="CollNumber">
            <xsl:value-of select="substring-before($CheckMerge, ':')"/>
          </xsl:with-param>
          <xsl:with-param name="BigMergeCell">
            <xsl:value-of select="$BigMergeCell"/>
          </xsl:with-param>
        </xsl:call-template>

      </xsl:when>


      <!-- when in this cell big merge coll is starting -->
      <xsl:when test="$CollNumber = substring-before($CheckMerge, ':')">
        <table:covered-table-cell>
          <xsl:attribute name="table:number-columns-repeated">
            <xsl:value-of
              select="substring-after($CheckMerge, ':') - substring-before($CheckMerge, ':') + 1"/>
          </xsl:attribute>
        </table:covered-table-cell>
        <xsl:call-template name="InsertEmptyColumnsBigMergeCollSheetNotEmpty">
          <xsl:with-param name="CollNumber">
            <xsl:value-of select="number(substring-after($CheckMerge, ':')) + 1"/>
          </xsl:with-param>
          <xsl:with-param name="BigMergeCell">
            <xsl:value-of
              select="concat(substring-before($BigMergeCell, concat($CheckMerge, ';')), substring-after($BigMergeCell, concat($CheckMerge, ';')))"
            />
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <table:table-cell>
          <xsl:choose>
            <xsl:when test="256 - $CollNumber &gt; 1">
              <xsl:attribute name="table:number-columns-repeated">
                <xsl:value-of select="256 - $CollNumber + 1"/>
              </xsl:attribute>
            </xsl:when>
            <xsl:otherwise/>
          </xsl:choose>
        </table:table-cell>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- When there are big merge coll and sheet isn't empty -->
  <xsl:template name="InsertBigMergeRowEmpty">
    <xsl:param name="BigMergeCell"/>
    <xsl:param name="RowNumber"/>
    <xsl:param name="Repeat"/>

    <xsl:if test="$Repeat != '0'">
      <table:table-row>
        <xsl:call-template name="InsertEmptyColumnsBigMergeCollSheetNotEmpty">
          <xsl:with-param name="CollNumber">1</xsl:with-param>
          <xsl:with-param name="BigMergeCell">
            <xsl:value-of select="$BigMergeCell"/>
          </xsl:with-param>
        </xsl:call-template>
      </table:table-row>

      <xsl:call-template name="InsertBigMergeRowEmpty">
        <xsl:with-param name="BigMergeCell">
          <xsl:value-of select="$BigMergeCell"/>
        </xsl:with-param>
        <xsl:with-param name="RowNumber">
          <xsl:value-of select="$RowNumber + 1"/>
        </xsl:with-param>
        <xsl:with-param name="Repeat">
          <xsl:value-of select="$Repeat - 1"/>
        </xsl:with-param>
      </xsl:call-template>

    </xsl:if>
  </xsl:template>


  <!-- Insert Big Merge Coll in between two columns -->

  <xsl:template name="InsertEmptyColumnsSheetNotEmpty">
    <xsl:param name="BigMergeCell"/>
    <xsl:param name="prevCellCol"/>
    <xsl:param name="colNum"/>
    <xsl:param name="rowNum"/>


    <xsl:variable name="CheckMerge">
      <xsl:call-template name="InsertSmallestBigMergeColl">
        <xsl:with-param name="BigMergeColl">
          <xsl:value-of select="$BigMergeCell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>


    <xsl:choose>

      <xsl:when test="$CheckMerge != ''">
        <xsl:if
          test="$prevCellCol != '' and (substring-before($CheckMerge, ':') &gt; $prevCellCol + 1) and ($colNum &gt; substring-before($CheckMerge, ':'))">
          <table:table-cell>
            <xsl:attribute name="table:number-columns-repeated">
              <xsl:value-of select="substring-before($CheckMerge, ':') - $prevCellCol - 1"/>
            </xsl:attribute>
          </table:table-cell>
        </xsl:if>

        <xsl:if
          test="$prevCellCol = '' and (substring-before($CheckMerge, ':') &gt; 1) and ($colNum &gt; substring-before($CheckMerge, ':'))">

          <table:table-cell>
            <xsl:attribute name="table:number-columns-repeated">
              <xsl:value-of select="substring-before($CheckMerge, ':')  - 1"/>
            </xsl:attribute>
          </table:table-cell>
        </xsl:if>

        <!--  Insert Big Merge Cell Before this cell -->

        <xsl:choose>
          <xsl:when
            test="$prevCellCol != '' and ($colNum &gt; substring-before($CheckMerge, ':')) and (substring-before($CheckMerge, ':') &gt; $prevCellCol + 1)">
            <xsl:choose>

              <xsl:when
                test="substring-after($CheckMerge, ':') - substring-before($CheckMerge, ':') = 0">
                <xsl:choose>
                  <xsl:when test="$rowNum = '1'">
                    <table:table-cell table:number-columns-spanned="1"
                      table:number-rows-spanned="65536"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <table:covered-table-cell/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:when>

              <xsl:otherwise>
                <xsl:choose>
                  <xsl:when test="$rowNum = '1'">
                    <table:table-cell table:number-rows-spanned="65536">
                      <xsl:attribute name="table:number-columns-spanned">
                        <xsl:value-of
                          select="substring-after($CheckMerge, ':') - substring-before($CheckMerge, ':') + 1"
                        />
                      </xsl:attribute>
                    </table:table-cell>
                    <table:covered-table-cell>
                      <xsl:attribute name="table:number-columns-repeated">
                        <xsl:value-of
                          select="substring-after($CheckMerge, ':') - substring-before($CheckMerge, ':')"
                        />
                      </xsl:attribute>
                    </table:covered-table-cell>
                  </xsl:when>
                  <xsl:otherwise>
                    <table:covered-table-cell>
                      <xsl:attribute name="table:number-columns-repeated">
                        <xsl:value-of
                          select="substring-after($CheckMerge, ':') - substring-before($CheckMerge, ':') + 1"
                        />
                      </xsl:attribute>
                    </table:covered-table-cell>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:otherwise>
            </xsl:choose>

            <xsl:call-template name="InsertEmptyColumnsSheetNotEmpty">
              <xsl:with-param name="BigMergeCell">
                <xsl:value-of
                  select="concat(substring-before($BigMergeCell, concat($CheckMerge, ';')), substring-after($BigMergeCell, concat($CheckMerge, ';')))"
                />
              </xsl:with-param>
              <xsl:with-param name="colNum">
                <xsl:value-of select="$colNum"/>
              </xsl:with-param>
              <xsl:with-param name="prevCellCol">
                <xsl:value-of select="substring-after($CheckMerge, ':')"/>
              </xsl:with-param>
              <xsl:with-param name="rowNum">
                <xsl:value-of select="$rowNum"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:when>

          <xsl:when
            test="$prevCellCol = '' and ($colNum &gt; substring-before($CheckMerge, ':')) and (substring-before($CheckMerge, ':') &gt; 1)">
            <xsl:choose>
              <xsl:when
                test="substring-after($CheckMerge, ':') - substring-before($CheckMerge, ':') = 0">
                <xsl:choose>
                  <xsl:when test="$rowNum = '1'">
                    <table:table-cell table:number-columns-spanned="1"
                      table:number-rows-spanned="65536"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <table:covered-table-cell/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:when>
              <xsl:otherwise>
                <xsl:choose>
                  <xsl:when test="$rowNum = '1'">
                    <table:table-cell table:number-rows-spanned="65536">
                      <xsl:attribute name="table:number-columns-spanned">
                        <xsl:value-of
                          select="substring-after($CheckMerge, ':') - substring-before($CheckMerge, ':') + 1"
                        />
                      </xsl:attribute>
                    </table:table-cell>
                    <table:covered-table-cell>
                      <xsl:attribute name="table:number-columns-repeated">
                        <xsl:value-of
                          select="substring-after($CheckMerge, ':') - substring-before($CheckMerge, ':')"
                        />
                      </xsl:attribute>
                    </table:covered-table-cell>
                  </xsl:when>
                  <xsl:otherwise>
                    <table:covered-table-cell>
                      <xsl:attribute name="table:number-columns-repeated">
                        <xsl:value-of
                          select="substring-after($CheckMerge, ':') - substring-before($CheckMerge, ':') + 1"
                        />
                      </xsl:attribute>
                    </table:covered-table-cell>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:otherwise>
            </xsl:choose>

            <xsl:call-template name="InsertEmptyColumnsSheetNotEmpty">
              <xsl:with-param name="BigMergeCell">
                <xsl:value-of
                  select="concat(substring-before($BigMergeCell, concat($CheckMerge, ';')), substring-after($BigMergeCell, concat($CheckMerge, ';')))"
                />
              </xsl:with-param>
              <xsl:with-param name="colNum">
                <xsl:value-of select="$colNum"/>
              </xsl:with-param>
              <xsl:with-param name="prevCellCol">
                <xsl:value-of select="substring-after($CheckMerge, ':')"/>
              </xsl:with-param>
              <xsl:with-param name="rowNum">
                <xsl:value-of select="$rowNum"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:when>

          <xsl:otherwise>

            <xsl:call-template name="InsertEmptyColumnsSheetNotEmpty">
              <xsl:with-param name="BigMergeCell">
                <xsl:value-of
                  select="concat(substring-before($BigMergeCell, concat($CheckMerge, ';')), substring-after($BigMergeCell, concat($CheckMerge, ';')))"
                />
              </xsl:with-param>
              <xsl:with-param name="colNum">
                <xsl:value-of select="$colNum"/>
              </xsl:with-param>
              <xsl:with-param name="prevCellCol">
                <xsl:value-of select="$prevCellCol"/>
              </xsl:with-param>
              <xsl:with-param name="rowNum">
                <xsl:value-of select="$rowNum"/>
              </xsl:with-param>
            </xsl:call-template>

          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>


      <xsl:otherwise>
        <xsl:if test="$colNum - $prevCellCol - 1 &gt; 0">
          <table:table-cell>
            <xsl:attribute name="table:number-columns-repeated">
              <xsl:value-of select="$colNum - $prevCellCol - 1"/>
            </xsl:attribute>
          </table:table-cell>
        </xsl:if>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- Insert table:covered-table-cell -->

  <xsl:template name="InsertCoveredTableCell">
    <xsl:param name="BeforeMerge"/>
    <xsl:param name="prevCellCol"/>
    <xsl:param name="BigMergeCell"/>
    <xsl:param name="this"/>
    <xsl:param name="colNum"/>
    <xsl:param name="rowNum"/>
    <xsl:param name="CheckIfMerge"/>
    <xsl:param name="PictureCell"/>
    <xsl:param name="PictureRow"/>
    <xsl:param name="PictureColl"/>
    <xsl:param name="NoteCell"/>
    <xsl:param name="NoteRow"/>
    <xsl:param name="NoteColl"/>
    <xsl:param name="sheet"/>
    <xsl:param name="NameSheet"/>
    <xsl:param name="sheetNr"/>
    <xsl:param name="ConditionalCell"/>
    <xsl:param name="ConditionalCellStyle"/>
    <xsl:param name="ConditionalCellCol"/>
    <xsl:param name="ConditionalCellAll"/>
    <xsl:param name="ConditionalCellSingle"/>
    <xsl:param name="ConditionalCellMultiple"/>
    <xsl:param name="ConvertRepeat" select="0"/>

    <xsl:variable name="GetMinColWithElement">
      <xsl:call-template name="GetMinRowWithPicture">
        <xsl:with-param name="PictureRow">
          <xsl:value-of select="concat($NoteColl, $PictureColl)"/>
        </xsl:with-param>
        <xsl:with-param name="AfterRow">
          <xsl:value-of select="$colNum"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>
      <!--RefNo-1-->
      <xsl:when
        test="number(substring-after($CheckIfMerge, ':')) &gt; 1 and $GetMinColWithElement != '' and $GetMinColWithElement &lt;= ($colNum - 2 + number(substring-after($CheckIfMerge, ':')))">

        <xsl:call-template name="InsertPictureInMergeCell">
          <xsl:with-param name="BeforeMerge">
            <xsl:value-of select="$BeforeMerge"/>
          </xsl:with-param>
          <xsl:with-param name="prevCellCol">
            <xsl:value-of select="$prevCellCol"/>
          </xsl:with-param>
          <xsl:with-param name="BigMergeCell">
            <xsl:value-of select="$BigMergeCell"/>
          </xsl:with-param>
          <xsl:with-param name="this">
            <xsl:value-of select="$this"/>
          </xsl:with-param>
          <xsl:with-param name="colNum">
            <xsl:value-of select="$colNum"/>
          </xsl:with-param>
          <xsl:with-param name="rowNum">
            <xsl:value-of select="$rowNum"/>
          </xsl:with-param>
          <xsl:with-param name="CheckIfMerge">
            <xsl:value-of select="$CheckIfMerge"/>
          </xsl:with-param>
          <xsl:with-param name="PictureCell">
            <xsl:value-of select="$PictureCell"/>
          </xsl:with-param>
          <xsl:with-param name="PictureRow">
            <xsl:value-of select="$PictureRow"/>
          </xsl:with-param>
          <xsl:with-param name="PictureColl">
            <xsl:value-of select="$PictureColl"/>
          </xsl:with-param>
          <xsl:with-param name="NoteCell">
            <xsl:value-of select="$NoteCell"/>
          </xsl:with-param>
          <xsl:with-param name="NoteRow">
            <xsl:value-of select="$NoteRow"/>
          </xsl:with-param>
          <xsl:with-param name="NoteColl">
            <xsl:value-of select="$NoteColl"/>
          </xsl:with-param>
          <xsl:with-param name="sheet">
            <xsl:value-of select="$sheet"/>
          </xsl:with-param>
          <xsl:with-param name="NameSheet">
            <xsl:value-of select="$NameSheet"/>
          </xsl:with-param>
          <xsl:with-param name="sheetNr">
            <xsl:value-of select="$sheetNr"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalCell">
            <xsl:value-of select="$ConditionalCell"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalCellStyle">
            <xsl:value-of select="$ConditionalCellStyle"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalCellCol">
            <xsl:value-of select="$ConditionalCellCol"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalCellAll">
            <xsl:value-of select="$ConditionalCellAll"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalCellSingle">
            <xsl:value-of select="$ConditionalCellSingle"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalCellMultiple">
            <xsl:value-of select="$ConditionalCellMultiple"/>
          </xsl:with-param>
          <xsl:with-param name="EndColl">
            <xsl:value-of select="number(substring-after($CheckIfMerge, ':'))"/>
          </xsl:with-param>
        </xsl:call-template>

        <!--xsl:if test="$GetMinColWithElement - $colNum &gt; 0">
          <table:covered-table-cell>
            <xsl:attribute name="table:number-columns-repeated">
              <xsl:value-of select="$GetMinColWithElement - $colNum"/>
            </xsl:attribute>
          </table:covered-table-cell>
        </xsl:if>
        
      <table:covered-table-cell>
      
        <xsl:variable name="Target">
          <xsl:for-each select="key('Part', concat('xl/', $sheet))/e:worksheet/e:drawing">
            <xsl:call-template name="GetTargetPicture">
              <xsl:with-param name="sheet">
                <xsl:value-of select="substring-after($sheet, '/')"/>
              </xsl:with-param>
              <xsl:with-param name="id">
                <xsl:value-of select="@r:id"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:for-each>
        </xsl:variable>
        
        <xsl:if test="contains(concat(';', $PictureCell), concat(';', $rowNum, ':', $GetMinColWithElement, ';'))">
          
        <xsl:call-template name="InsertPictureInThisCell">
          <xsl:with-param name="sheet">
            <xsl:value-of select="$sheet"/>
          </xsl:with-param>
          <xsl:with-param name="NameSheet">
            <xsl:value-of select="$NameSheet"/>
          </xsl:with-param>
          <xsl:with-param name="collNum">
            <xsl:value-of select="$GetMinColWithElement"/>
          </xsl:with-param>
          <xsl:with-param name="rowNum">
            <xsl:value-of select="$rowNum"/>
          </xsl:with-param>
          <xsl:with-param name="Target">
            <xsl:value-of select="$Target"/>
          </xsl:with-param>
        </xsl:call-template>
        
        </xsl:if>
        
      </table:covered-table-cell>
      
        <xsl:call-template name="InsertCoveredTableCell">
          <xsl:with-param name="BeforeMerge">
            <xsl:value-of select="$BeforeMerge"/>
          </xsl:with-param>
          <xsl:with-param name="prevCellCol">
            <xsl:value-of select="$prevCellCol"/>
          </xsl:with-param>
          <xsl:with-param name="BigMergeCell">
            <xsl:value-of select="$BigMergeCell"/>
          </xsl:with-param>
          <xsl:with-param name="this">
            <xsl:value-of select="$this"/>
          </xsl:with-param>
          <xsl:with-param name="colNum">
            <xsl:value-of select="$GetMinColWithElement + 1"/>
          </xsl:with-param>
          <xsl:with-param name="rowNum">
            <xsl:value-of select="$rowNum"/>
          </xsl:with-param>
          <xsl:with-param name="CheckIfMerge">
            <xsl:value-of select="$CheckIfMerge"/>
          </xsl:with-param>
          <xsl:with-param name="PictureCell">
            <xsl:value-of select="$PictureCell"/>
          </xsl:with-param>
          <xsl:with-param name="PictureRow">
            <xsl:value-of select="$PictureRow"/>
          </xsl:with-param>
          <xsl:with-param name="PictureColl">
            <xsl:value-of select="$PictureColl"/>
          </xsl:with-param>
          <xsl:with-param name="NoteCell">
            <xsl:value-of select="$NoteCell"/>
          </xsl:with-param>
          <xsl:with-param name="NoteRow">
            <xsl:value-of select="$NoteRow"/>
          </xsl:with-param>
          <xsl:with-param name="NoteColl">
            <xsl:value-of select="$NoteColl"/>
          </xsl:with-param>
          <xsl:with-param name="sheet">
            <xsl:value-of select="$sheet"/>
          </xsl:with-param>
          <xsl:with-param name="NameSheet">
            <xsl:value-of select="$NameSheet"/>
          </xsl:with-param>
          <xsl:with-param name="sheetNr">
            <xsl:value-of select="$sheetNr"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalCell">
            <xsl:value-of select="$ConditionalCell"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalCellStyle">
            <xsl:value-of select="$ConditionalCellStyle"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalCellCol">
          <xsl:value-of select="$ConditionalCellCol"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalCellAll">
          <xsl:value-of select="$ConditionalCellAll"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalCellSingle">
          <xsl:value-of select="$ConditionalCellSingle"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalCellMultiple">
          <xsl:value-of select="$ConditionalCellMultiple"/>
          </xsl:with-param>
          <xsl:with-param name="ConvertRepeat">
            <xsl:value-of select="$GetMinColWithElement + 1"/>
          </xsl:with-param>
        </xsl:call-template-->

      </xsl:when>
      <xsl:when test="number(substring-after($CheckIfMerge, ':')) &gt; 1">
        <table:covered-table-cell>

          <xsl:attribute name="table:number-columns-repeated">
            <xsl:choose>
              <xsl:when test="$colNum + number(substring-after($CheckIfMerge, ':')) &gt; 256">
                <xsl:value-of select="256 - $colNum"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="number(substring-after($CheckIfMerge, ':')) - 1"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:attribute>

        </table:covered-table-cell>
      </xsl:when>
      <xsl:otherwise>
        <table:covered-table-cell/>
      </xsl:otherwise>
    </xsl:choose>

    <!--xsl:choose>
      <xsl:when test="number(substring-after($CheckIfMerge, ':')) &gt; 1">
      <table:covered-table-cell>
      <xsl:attribute name="table:number-columns-repeated">
      <xsl:choose>
      <xsl:when
      test="$colNum + number(substring-after($CheckIfMerge, ':')) &gt; 256">
      <xsl:value-of select="256 - $colNum"/>
      </xsl:when>
      <xsl:otherwise>
      <xsl:value-of select="number(substring-after($CheckIfMerge, ':')) - 1"/>
      </xsl:otherwise>
      </xsl:choose>
      </xsl:attribute>
      </table:covered-table-cell>
      </xsl:when>
      <xsl:otherwise>
      <table:covered-table-cell/>
      </xsl:otherwise>
      </xsl:choose-->

  </xsl:template>

  <xsl:template name="InsertPictureInMergeCell">
    <xsl:param name="BeforeMerge"/>
    <xsl:param name="prevCellCol"/>
    <xsl:param name="BigMergeCell"/>
    <xsl:param name="this"/>
    <xsl:param name="colNum"/>
    <xsl:param name="rowNum"/>
    <xsl:param name="CheckIfMerge"/>
    <xsl:param name="PictureCell"/>
    <xsl:param name="PictureRow"/>
    <xsl:param name="PictureColl"/>
    <xsl:param name="NoteCell"/>
    <xsl:param name="NoteRow"/>
    <xsl:param name="NoteColl"/>
    <xsl:param name="sheet"/>
    <xsl:param name="NameSheet"/>
    <xsl:param name="sheetNr"/>
    <xsl:param name="ConditionalCell"/>
    <xsl:param name="ConditionalCellStyle"/>
    <xsl:param name="ConditionalCellCol"/>
    <xsl:param name="ConditionalCellAll"/>
    <xsl:param name="ConditionalCellSingle"/>
    <xsl:param name="ConditionalCellMultiple"/>
    <xsl:param name="EndColl"/>



    <xsl:variable name="GetMinColWithElement">
      <xsl:call-template name="GetMinRowWithPicture">
        <xsl:with-param name="PictureRow">
          <xsl:value-of select="concat($NoteColl, $PictureColl)"/>
        </xsl:with-param>
        <xsl:with-param name="AfterRow">
          <xsl:value-of select="$colNum"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>
      <!--RefNo-1
      <xsl:when
        test="$EndColl &gt; 1 and $GetMinColWithElement != '' and $GetMinColWithElement &lt;= ($colNum - 1 + $EndColl)">
        -->
      <xsl:when
        test="$EndColl &gt; 1 and $GetMinColWithElement != ''">
        <!--Start of RefNo-1-->
        <xsl:choose>
          <xsl:when test="$GetMinColWithElement &lt;= ($colNum - 2 + $EndColl)">
            <!--End of RefNo-1-->
        <xsl:if test="$GetMinColWithElement - $colNum &gt; 0">
         
          <!--<table:covered-table-cell>
            <xsl:attribute name="table:number-columns-repeated">
              <xsl:value-of select="$GetMinColWithElement - $colNum"/>
            </xsl:attribute>
          </table:covered-table-cell>-->          
        <!--XSLT Best Practices-->
          <table:covered-table-cell table:number-columns-repeated="{$GetMinColWithElement - $colNum}"/>         
        </xsl:if>


        <table:covered-table-cell>

          <xsl:variable name="Target">
            <xsl:for-each select="key('Part', concat('xl/', $sheet))/e:worksheet/e:drawing">
              <xsl:call-template name="GetTargetPicture">
                <xsl:with-param name="sheet">
                  <xsl:value-of select="substring-after($sheet, '/')"/>
                </xsl:with-param>
                <xsl:with-param name="id">
                  <xsl:value-of select="@r:id"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:for-each>
          </xsl:variable>



          <xsl:if
            test="contains(concat(';', $PictureCell), concat(';', $rowNum, ':', $GetMinColWithElement, ';'))">

            <xsl:call-template name="InsertPictureInThisCell">
              <xsl:with-param name="sheet">
                <xsl:value-of select="$sheet"/>
              </xsl:with-param>
              <xsl:with-param name="NameSheet">
                <xsl:value-of select="$NameSheet"/>
              </xsl:with-param>
              <xsl:with-param name="collNum">
                <xsl:value-of select="$GetMinColWithElement"/>
              </xsl:with-param>
              <xsl:with-param name="rowNum">
                <xsl:value-of select="$rowNum"/>
              </xsl:with-param>
              <xsl:with-param name="Target">
                <xsl:value-of select="$Target"/>
              </xsl:with-param>
            </xsl:call-template>

          </xsl:if>

        </table:covered-table-cell>

        <xsl:call-template name="InsertPictureInMergeCell">
          <xsl:with-param name="BeforeMerge">
            <xsl:value-of select="$BeforeMerge"/>
          </xsl:with-param>
          <xsl:with-param name="prevCellCol">
            <xsl:value-of select="$GetMinColWithElement"/>
          </xsl:with-param>
          <xsl:with-param name="BigMergeCell">
            <xsl:value-of select="$BigMergeCell"/>
          </xsl:with-param>
          <xsl:with-param name="this">
            <xsl:value-of select="$this"/>
          </xsl:with-param>
          <xsl:with-param name="colNum">
            <xsl:value-of select="$GetMinColWithElement + 1"/>
          </xsl:with-param>
          <xsl:with-param name="rowNum">
            <xsl:value-of select="$rowNum"/>
          </xsl:with-param>
          <xsl:with-param name="CheckIfMerge">
            <xsl:value-of select="$CheckIfMerge"/>
          </xsl:with-param>
          <xsl:with-param name="PictureCell">
            <xsl:value-of select="$PictureCell"/>
          </xsl:with-param>
          <xsl:with-param name="PictureRow">
            <xsl:value-of select="$PictureRow"/>
          </xsl:with-param>
          <xsl:with-param name="PictureColl">
            <xsl:value-of select="$PictureColl"/>
          </xsl:with-param>
          <xsl:with-param name="NoteCell">
            <xsl:value-of select="$NoteCell"/>
          </xsl:with-param>
          <xsl:with-param name="NoteRow">
            <xsl:value-of select="$NoteRow"/>
          </xsl:with-param>
          <xsl:with-param name="NoteColl">
            <xsl:value-of select="$NoteColl"/>
          </xsl:with-param>
          <xsl:with-param name="sheet">
            <xsl:value-of select="$sheet"/>
          </xsl:with-param>
          <xsl:with-param name="NameSheet">
            <xsl:value-of select="$NameSheet"/>
          </xsl:with-param>
          <xsl:with-param name="sheetNr">
            <xsl:value-of select="$sheetNr"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalCell">
            <xsl:value-of select="$ConditionalCell"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalCellStyle">
            <xsl:value-of select="$ConditionalCellStyle"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalCellCol">
            <xsl:value-of select="$ConditionalCellCol"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalCellAll">
            <xsl:value-of select="$ConditionalCellAll"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalCellSingle">
            <xsl:value-of select="$ConditionalCellSingle"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalCellMultiple">
            <xsl:value-of select="$ConditionalCellMultiple"/>
          </xsl:with-param>
          <xsl:with-param name="EndColl">
            <!--Start of RefNo-1-->
            <xsl:choose>
              <xsl:when test="$GetMinColWithElement - $colNum &gt; 0">
                <xsl:value-of select="$EndColl - ($GetMinColWithElement - $colNum + 1)"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$EndColl - 1"/>
              </xsl:otherwise>
            </xsl:choose>
            <!--End of RefNo-1-->
          </xsl:with-param>
        </xsl:call-template>
            <!--Start of RefNo-1-->
          </xsl:when>
          <xsl:otherwise>
            <xsl:if test="($EndColl - 1) &gt; 0">              
              <!--<table:covered-table-cell>
                <xsl:attribute name="table:number-columns-repeated">
                  <xsl:value-of select="$EndColl - 1"/>
                </xsl:attribute>
              </table:covered-table-cell>-->   
              <!--Best Pactice, XSLT-->           
              <table:covered-table-cell table:number-columns-repeated="{$EndColl - 1}"/>
              <!--end-->
              
            </xsl:if>
          </xsl:otherwise>
        </xsl:choose>
        <!--End of RefNo-1-->
      </xsl:when>
      <!--RefNo-1-->
      <xsl:when test="$EndColl &gt; 1 and $GetMinColWithElement = '' and ($EndColl - 1) &gt; 0">
        <!--<table:covered-table-cell>
          <xsl:attribute name="table:number-columns-repeated">
           RefNo-1
            <xsl:value-of select="$EndColl - $prevCellCol"/>
            
            <xsl:value-of select="$EndColl - 1"/>
          </xsl:attribute>
        </table:covered-table-cell>-->
        <!--XSLT Best Practice-->
        <table:covered-table-cell table:number-columns-repeated="{$EndColl - 1}"/>
        <!--end-->
      </xsl:when>
    </xsl:choose>

  </xsl:template>



</xsl:stylesheet>
