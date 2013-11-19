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
RefNo-1 6-Jan-2009 Sandeep S     ODF1.1   Changes done for ODF1.1 conformance                                              
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
  xmlns:xdr="http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing"
  xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main"
  xmlns:number="urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0"
  xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
  xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:dc="http://purl.org/dc/elements/1.1/"
  xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart"
  xmlns:e="http://schemas.openxmlformats.org/spreadsheetml/2006/main"
  xmlns:oox="urn:oox"
  exclude-result-prefixes="e oox r c xdr xlink">

  <!-- Insert Empty Sheet with picture -->

  <xsl:template name="InsertEmptySheetWithElements">
    <xsl:param name="sheet"/>
    <xsl:param name="NameSheet"/>
    <xsl:param name="PictureCell"/>
    <xsl:param name="PictureRow"/>
    <xsl:param name="NoteCell"/>
    <xsl:param name="NoteRow"/>
    <xsl:param name="ConditionalCell"/>
    <xsl:param name="ConditionalCellStyle"/>
    <xsl:param name="ConditionalRow"/>
    <xsl:param name="ConditionalCellCol"/>
    <xsl:param name="ConditionalCellAll"/>
    <xsl:param name="ConditionalCellSingle"/>
    <xsl:param name="ConditionalCellMultiple"/>
    <xsl:param name="prevRow" select="0"/>
    <xsl:param name="sheetNr"/>
    <xsl:param name="ValidationCell"/>
    <xsl:param name="ValidationRow"/>
    <xsl:param name="ValidationCellStyle"/>
    <xsl:param name="AllRowBreakes"/>

    <xsl:variable name="id">
      <xsl:value-of select="key('drawing', ancestor::e:worksheet/@oox:part)/@r:id"/>
    </xsl:variable>

    <xsl:variable name="AllElementsCell">
      <xsl:value-of select="concat($PictureCell, $NoteCell, $ConditionalCell, $ValidationCell)"/>
    </xsl:variable>

    <xsl:variable name="AllElementsRow">
      <xsl:value-of
        select="concat($PictureRow, $NoteRow, $ConditionalRow, $ValidationRow, $AllRowBreakes)"/>
    </xsl:variable>

    <!--xsl:variable name="GetMinRowWithConditional">
      <xsl:call-template name="GetMinRowWithConditional">
        <xsl:with-param name="ConditionalCellSingle">
          <xsl:value-of select="$ConditionalCellSingle"/>
        </xsl:with-param>
        <xsl:with-param name="ConditionalCellMultiple">
          <xsl:value-of select="$ConditionalCellSingle"/>
        </xsl:with-param>
        <xsl:with-param name="AfterRow">
          <xsl:value-of select="$prevRow + 1"/>
        </xsl:with-param>
       </xsl:call-template>
    </xsl:variable-->
    
   

    <xsl:variable name="GetMinRowWithElement">
      <xsl:call-template name="GetMinRowWithPicture">
        <xsl:with-param name="PictureRow">
          <xsl:value-of select="$AllElementsRow"/>
        </xsl:with-param>
        <xsl:with-param name="AfterRow">
          <!--Start of RefNo-1:ODF1.1:rmoved increment to set the value to 0-->                   
          <xsl:value-of select="$prevRow"/>
          <!--<xsl:value-of select="$prevRow + 1"/>-->
          <!--End of RefNo-1-->
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:if test="number($GetMinRowWithElement) - number($prevRow) - 1 &gt; 0">
      <xsl:call-template name="InsertEmptyRows">
        <xsl:with-param name="repeat">
          <xsl:value-of select="number($GetMinRowWithElement) - number($prevRow) - 1"/>
        </xsl:with-param>
        <xsl:with-param name="sheet">
          <xsl:value-of select="$sheet"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:if>

    <xsl:variable name="CollsWithElements">
      <xsl:call-template name="GetCollsWithElement">
        <xsl:with-param name="rowNumber">
          <xsl:value-of select="$GetMinRowWithElement"/>
        </xsl:with-param>
        <xsl:with-param name="ElementCell">
          <xsl:value-of select="concat(';', $AllElementsCell)"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:if test="$GetMinRowWithElement != ''">

      <xsl:for-each select="key('Part', concat('xl/',$sheet))">
        <table:table-row>

          <xsl:choose>
            <xsl:when
              test="contains(concat(';', $AllRowBreakes), concat(';', $GetMinRowWithElement))">
              <xsl:attribute name="table:style-name">
                <xsl:value-of
                  select="generate-id(key('Part', concat('xl/',$sheet))/e:worksheet/e:rowBreaks)"/>
              </xsl:attribute>
            </xsl:when>
            <xsl:otherwise>
              <xsl:attribute name="table:style-name">
                <xsl:value-of select="concat('ro', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)"/>
              </xsl:attribute>
            </xsl:otherwise>
          </xsl:choose>

          <xsl:call-template name="InsertElementsInThisRow">
            <xsl:with-param name="sheet">
              <xsl:value-of select="$sheet"/>
            </xsl:with-param>
            <xsl:with-param name="NameSheet">
              <xsl:value-of select="$NameSheet"/>
            </xsl:with-param>
            <xsl:with-param name="PictureCell">
              <xsl:value-of select="$PictureCell"/>
            </xsl:with-param>
            <xsl:with-param name="PictureRow">
              <xsl:value-of select="$PictureRow"/>
            </xsl:with-param>
            <xsl:with-param name="NoteCell">
              <xsl:value-of select="$NoteCell"/>
            </xsl:with-param>
            <xsl:with-param name="NoteRow">
              <xsl:value-of select="$NoteRow"/>
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
            <xsl:with-param name="ElementsColl">
              <xsl:value-of select="$CollsWithElements"/>
            </xsl:with-param>
            <xsl:with-param name="rowNum">
              <xsl:value-of select="$GetMinRowWithElement"/>
            </xsl:with-param>
            <xsl:with-param name="sheetNr" select="$sheetNr"/>
            <xsl:with-param name="ValidationCell">
              <xsl:value-of select="$ValidationCell"/>
            </xsl:with-param>
            <xsl:with-param name="ValidationRow">
              <xsl:value-of select="$ValidationRow"/>
            </xsl:with-param>
            <xsl:with-param name="ValidationCellStyle">
              <xsl:value-of select="$ValidationCellStyle"/>
            </xsl:with-param>
            <xsl:with-param name="AllRowBreakes">
              <xsl:value-of select="$AllRowBreakes"/>
            </xsl:with-param>
          </xsl:call-template>
        </table:table-row>
      </xsl:for-each>

      <xsl:call-template name="InsertEmptySheetWithElements">
        <xsl:with-param name="sheet">
          <xsl:value-of select="$sheet"/>
        </xsl:with-param>
        <xsl:with-param name="NameSheet">
          <xsl:value-of select="$NameSheet"/>
        </xsl:with-param>
        <xsl:with-param name="PictureCell">
          <xsl:value-of select="$PictureCell"/>
        </xsl:with-param>
        <xsl:with-param name="PictureRow">
          <xsl:value-of select="$PictureRow"/>
        </xsl:with-param>
        <xsl:with-param name="NoteCell">
          <xsl:value-of select="$NoteCell"/>
        </xsl:with-param>
        <xsl:with-param name="NoteRow">
          <xsl:value-of select="$NoteRow"/>
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
        <xsl:with-param name="prevRow">
          <xsl:value-of select="$GetMinRowWithElement"/>
        </xsl:with-param>
        <xsl:with-param name="ConditionalRow">
          <xsl:value-of select="$ConditionalRow"/>
        </xsl:with-param>
        <xsl:with-param name="sheetNr" select="$sheetNr"/>
        <xsl:with-param name="ValidationCell">
          <xsl:value-of select="$ValidationCell"/>
        </xsl:with-param>
        <xsl:with-param name="ValidationRow">
          <xsl:value-of select="$ValidationRow"/>
        </xsl:with-param>
        <xsl:with-param name="ValidationCellStyle">
          <xsl:value-of select="$ValidationCellStyle"/>
        </xsl:with-param>
        <xsl:with-param name="AllRowBreakes">
          <xsl:value-of select="$AllRowBreakes"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:if>

  </xsl:template>

  <xsl:template name="InsertElementsInThisRow">
    <xsl:param name="sheet"/>
    <xsl:param name="NameSheet"/>
    <xsl:param name="PictureCell"/>
    <xsl:param name="PictureRow"/>
    <xsl:param name="NoteCell"/>
    <xsl:param name="NoteRow"/>
    <xsl:param name="ConditionalCell"/>
    <xsl:param name="ConditionalCellStyle"/>
    <xsl:param name="ConditionalCellCol"/>
    <xsl:param name="ConditionalCellAll"/>
    <xsl:param name="ConditionalCellSingle"/>
    <xsl:param name="ConditionalCellMultiple"/>
    <xsl:param name="ElementsColl"/>
    <xsl:param name="prevColl" select="0"/>
    <xsl:param name="rowNum"/>
    <xsl:param name="sheetNr"/>
    <xsl:param name="ValidationCell"/>
    <xsl:param name="ValidationRow"/>
    <xsl:param name="ValidationCellStyle"/>
    <xsl:param name="AllRowBreakes"/>

    <xsl:variable name="GetMinColWithElement">
      <xsl:call-template name="GetMinRowWithPicture">
        <xsl:with-param name="PictureRow">
          <xsl:value-of select="$ElementsColl"/>
        </xsl:with-param>
        <xsl:with-param name="AfterRow">
          <xsl:value-of select="$prevColl"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:call-template name="InsertEmptyColls">
      <xsl:with-param name="repeat">
        <xsl:value-of select="$GetMinColWithElement - $prevColl - 1"/>
      </xsl:with-param>
    </xsl:call-template>



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
    <!--Start of RefNo-1:ODF1.1:Added otherwise condition to include empty cell:To reslove uncompleted content model-->
    <xsl:choose>
      <xsl:when test="$GetMinColWithElement != ''">
        <!--<xsl:if test="$GetMinColWithElement != ''">-->

      <table:table-cell>

        <xsl:if
          test="contains(concat(';', $ValidationCell), concat(';', $rowNum, ':', $GetMinColWithElement, ';'))">

          <xsl:variable name="ValidationStyle">
            <xsl:value-of
              select="substring-before(substring-after(concat(';', $ValidationCellStyle), concat(';', $rowNum, ':', $GetMinColWithElement, ';-')), ';')"
            />
          </xsl:variable>
          <xsl:attribute name="table:content-validation-name">
            <xsl:value-of
              select="concat('val', substring-before((substring-after($sheet, 'worksheets/sheet')), '.'), $ValidationStyle + 1)"
            />
          </xsl:attribute>

        </xsl:if>

        <xsl:if
          test="contains(concat(';', $ConditionalCell), concat(';', $rowNum, ':', $GetMinColWithElement, ';'))">
          <xsl:variable name="ConditionalStyleId">
            <xsl:value-of
              select="generate-id(key('ConditionalFormatting', ancestor::e:worksheet/@oox:part)[@oox:id = substring-before(substring-after(concat(';', $ConditionalCellStyle), concat(';', $rowNum, ':', $GetMinColWithElement, ';-')), ';')])"
            />
			  		  
          </xsl:variable>
          <xsl:attribute name="table:style-name">
            <xsl:value-of select="$ConditionalStyleId"/>
          </xsl:attribute>
        </xsl:if>

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

        <xsl:if
          test="contains(concat(';', $NoteCell), concat(';', $rowNum, ':', $GetMinColWithElement, ';'))">

          <xsl:call-template name="InsertNoteInThisCell">
            <xsl:with-param name="sheetNr">
              <xsl:value-of select="$sheetNr"/>
            </xsl:with-param>
            <xsl:with-param name="colNum">
              <xsl:value-of select="$GetMinColWithElement"/>
            </xsl:with-param>
            <xsl:with-param name="rowNum">
              <xsl:value-of select="$rowNum"/>
            </xsl:with-param>
          </xsl:call-template>

        </xsl:if>

      </table:table-cell>

      <xsl:call-template name="InsertElementsInThisRow">
        <xsl:with-param name="sheet">
          <xsl:value-of select="$sheet"/>
        </xsl:with-param>
        <xsl:with-param name="NameSheet">
          <xsl:value-of select="$NameSheet"/>
        </xsl:with-param>
        <xsl:with-param name="PictureCell">
          <xsl:value-of select="$PictureCell"/>
        </xsl:with-param>
        <xsl:with-param name="PictureRow">
          <xsl:value-of select="$PictureRow"/>
        </xsl:with-param>
        <xsl:with-param name="NoteCell">
          <xsl:value-of select="$NoteCell"/>
        </xsl:with-param>
        <xsl:with-param name="NoteRow">
          <xsl:value-of select="$NoteRow"/>
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
        <xsl:with-param name="ElementsColl">
          <xsl:value-of select="$ElementsColl"/>
        </xsl:with-param>
        <xsl:with-param name="rowNum">
          <xsl:value-of select="$rowNum"/>
        </xsl:with-param>
        <xsl:with-param name="prevColl">
          <xsl:value-of select="$GetMinColWithElement"/>
        </xsl:with-param>
        <xsl:with-param name="sheetNr" select="$sheetNr"/>
        <xsl:with-param name="ValidationCell">
          <xsl:value-of select="$ValidationCell"/>
        </xsl:with-param>
        <xsl:with-param name="ValidationRow">
          <xsl:value-of select="$ValidationRow"/>
        </xsl:with-param>
        <xsl:with-param name="ValidationCellStyle">
          <xsl:value-of select="$ValidationCellStyle"/>
        </xsl:with-param>
      </xsl:call-template>
        <!--</xsl:if>-->
      </xsl:when>
      <xsl:otherwise>
        <table:table-cell/>
      </xsl:otherwise>
    </xsl:choose>
    <!--End of RefNo-1-->

  </xsl:template>


  <!-- Insert Empty Rows before element -->
  <xsl:template name="InsertEmptyRows">
    <xsl:param name="repeat"/>
    <xsl:param name="sheet"/>
    <xsl:if test="$repeat &gt; 0">
      <table:table-row table:style-name="{concat('ro', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)}"
        table:number-rows-repeated="{$repeat}">
        <table:table-cell table:number-columns-repeated="256"/>
      </table:table-row>
    </xsl:if>
  </xsl:template>

  <!-- Insert Empty Cols before element -->
  <xsl:template name="InsertEmptyColls">
    <xsl:param name="repeat"/>
    <xsl:if test="$repeat &gt; 0">
      <table:table-cell table:number-columns-repeated="{$repeat}"/>
    </xsl:if>
  </xsl:template>

  <!-- Get colls with picture from this row  -->

  <xsl:template name="GetCollsWithElement">
    <xsl:param name="rowNumber"/>
    <xsl:param name="ElementColl"/>
    <xsl:param name="ElementCell"/>

    <xsl:choose>
      <xsl:when test="contains($ElementCell, concat(concat(';',$rowNumber),':'))">
        <xsl:call-template name="GetCollsWithElement">
          <xsl:with-param name="rowNumber">
            <xsl:value-of select="$rowNumber"/>
          </xsl:with-param>
          <xsl:with-param name="ElementColl">
            <xsl:value-of
              select="concat($ElementColl, concat(substring-before(substring-after($ElementCell, concat(';', concat($rowNumber, ':'))),';'), ';'))"
            />
          </xsl:with-param>
          <xsl:with-param name="ElementCell">
            <xsl:value-of
              select="concat(';', substring-after(substring-after($ElementCell, concat(';', concat($rowNumber, ':'))),';'))"
            />
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$ElementColl"/>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="InsertElementsBetwenTwoRows">
    <xsl:param name="sheet"/>
    <xsl:param name="NameSheet"/>
    <xsl:param name="PictureCell"/>
    <xsl:param name="PictureRow"/>
    <xsl:param name="NoteCell"/>
    <xsl:param name="NoteRow"/>
    <xsl:param name="ConditionalCell"/>
    <xsl:param name="ConditionalCellStyle"/>
    <xsl:param name="ConditionalRow"/>
    <xsl:param name="ConditionalCellCol"/>
    <xsl:param name="ConditionalCellAll"/>
    <xsl:param name="ConditionalCellSingle"/>
    <xsl:param name="ConditionalCellMultiple"/>
    <xsl:param name="prevRow" select="0"/>
    <xsl:param name="sheetNr"/>
    <xsl:param name="EndRow"/>
    <xsl:param name="ValidationCell"/>
    <xsl:param name="ValidationRow"/>
    <xsl:param name="ValidationCellStyle"/>
    <xsl:param name="AllRowBreakes"/>


    <xsl:variable name="id">
      <xsl:value-of select="key('drawing', ancestor::e:worksheet/@oox:part)/@r:id"/>
    </xsl:variable>

    <xsl:variable name="AllElementsCell">
      <xsl:value-of select="concat($PictureCell, $NoteCell, $ConditionalCell, $ValidationCell)"/>
    </xsl:variable>

    <xsl:variable name="AllElementsRow">
      <xsl:value-of
        select="concat($PictureRow, $NoteRow, $ConditionalRow, $ValidationRow, $AllRowBreakes)"/>
    </xsl:variable>
    
    <!--xsl:variable name="GetMinRowWithConditional">
      <xsl:call-template name="GetMinRowWithConditional">
        <xsl:with-param name="ConditionalCellSingle">
          <xsl:value-of select="$ConditionalCellSingle"/>
        </xsl:with-param>
        <xsl:with-param name="ConditionalCellMultiple">
          <xsl:value-of select="$ConditionalCellSingle"/>
        </xsl:with-param>
        <xsl:with-param name="AfterRow">
          <xsl:value-of select="$prevRow + 1"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable-->

    <xsl:variable name="GetMinRowWithElement">
      <xsl:call-template name="GetMinRowWithPicture">
        <xsl:with-param name="PictureRow">
          <xsl:value-of select="$AllElementsRow"/>
        </xsl:with-param>
        <xsl:with-param name="AfterRow">
          <xsl:value-of select="$prevRow"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>
      <xsl:when
        test="number($GetMinRowWithElement) - number($prevRow) - 1 &gt; 0 and $GetMinRowWithElement &lt;= $EndRow">
        <xsl:call-template name="InsertEmptyRows">
          <xsl:with-param name="repeat">
            <xsl:value-of select="number($GetMinRowWithElement) - number($prevRow) - 1"/>
          </xsl:with-param>
          <xsl:with-param name="sheet">
            <xsl:value-of select="$sheet"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      <xsl:when
        test=" ($GetMinRowWithElement &gt; $EndRow or $GetMinRowWithElement = '') and $EndRow - $prevRow &gt; 0">
        <table:table-row table:style-name="{concat('ro', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)}"
          table:number-rows-repeated="{$EndRow - $prevRow}">
          <table:table-cell table:number-columns-repeated="256"/>
        </table:table-row>
      </xsl:when>
      <xsl:otherwise/>
    </xsl:choose>



    <xsl:variable name="CollsWithElements">
      <xsl:call-template name="GetCollsWithElement">
        <xsl:with-param name="rowNumber">
          <xsl:value-of select="$GetMinRowWithElement"/>
        </xsl:with-param>
        <xsl:with-param name="ElementCell">
          <xsl:value-of select="concat(';', $AllElementsCell)"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:if test="$GetMinRowWithElement != '' and $GetMinRowWithElement &lt;= $EndRow">

      <xsl:for-each select="key('Part', concat('xl/',$sheet))">
        <table:table-row>
          <xsl:choose>
            <xsl:when
              test="contains(concat(';', $AllRowBreakes), concat(';', $GetMinRowWithElement))">
              <xsl:attribute name="table:style-name">
                <xsl:value-of
                  select="generate-id(key('Part', concat('xl/',$sheet))/e:worksheet/e:rowBreaks)"/>
              </xsl:attribute>
            </xsl:when>
            <xsl:otherwise>
              <xsl:attribute name="table:style-name">
                <xsl:value-of select="concat('ro', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)"/>
              </xsl:attribute>
            </xsl:otherwise>
          </xsl:choose>

          <xsl:call-template name="InsertElementsInThisRow">
            <xsl:with-param name="sheet">
              <xsl:value-of select="$sheet"/>
            </xsl:with-param>
            <xsl:with-param name="NameSheet">
              <xsl:value-of select="$NameSheet"/>
            </xsl:with-param>
            <xsl:with-param name="PictureCell">
              <xsl:value-of select="$PictureCell"/>
            </xsl:with-param>
            <xsl:with-param name="PictureRow">
              <xsl:value-of select="$PictureRow"/>
            </xsl:with-param>
            <xsl:with-param name="NoteCell">
              <xsl:value-of select="$NoteCell"/>
            </xsl:with-param>
            <xsl:with-param name="NoteRow">
              <xsl:value-of select="$NoteRow"/>
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
            <xsl:with-param name="ElementsColl">
              <xsl:value-of select="$CollsWithElements"/>
            </xsl:with-param>
            <xsl:with-param name="rowNum">
              <xsl:value-of select="$GetMinRowWithElement"/>
            </xsl:with-param>
            <xsl:with-param name="sheetNr" select="$sheetNr"/>
            <xsl:with-param name="ValidationCell">
              <xsl:value-of select="$ValidationCell"/>
            </xsl:with-param>
            <xsl:with-param name="ValidationRow">
              <xsl:value-of select="$ValidationRow"/>
            </xsl:with-param>
            <xsl:with-param name="ValidationCellStyle">
              <xsl:value-of select="$ValidationCellStyle"/>
            </xsl:with-param>
          </xsl:call-template>
        </table:table-row>
      </xsl:for-each>

      <xsl:call-template name="InsertElementsBetwenTwoRows">
        <xsl:with-param name="sheet">
          <xsl:value-of select="$sheet"/>
        </xsl:with-param>
        <xsl:with-param name="NameSheet">
          <xsl:value-of select="$NameSheet"/>
        </xsl:with-param>
        <xsl:with-param name="PictureCell">
          <xsl:value-of select="$PictureCell"/>
        </xsl:with-param>
        <xsl:with-param name="PictureRow">
          <xsl:value-of select="$PictureRow"/>
        </xsl:with-param>
        <xsl:with-param name="NoteCell">
          <xsl:value-of select="$NoteCell"/>
        </xsl:with-param>
        <xsl:with-param name="NoteRow">
          <xsl:value-of select="$NoteRow"/>
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
        <xsl:with-param name="prevRow">
          <xsl:value-of select="$GetMinRowWithElement"/>
        </xsl:with-param>
        <xsl:with-param name="ConditionalRow">
          <xsl:value-of select="$ConditionalRow"/>
        </xsl:with-param>
        <xsl:with-param name="sheetNr" select="$sheetNr"/>
        <xsl:with-param name="EndRow">
          <xsl:value-of select="$EndRow"/>
        </xsl:with-param>
        <xsl:with-param name="ValidationCell">
          <xsl:value-of select="$ValidationCell"/>
        </xsl:with-param>
        <xsl:with-param name="ValidationRow">
          <xsl:value-of select="$ValidationRow"/>
        </xsl:with-param>
        <xsl:with-param name="ValidationCellStyle">
          <xsl:value-of select="$ValidationCellStyle"/>
        </xsl:with-param>
        <xsl:with-param name="AllRowBreakes">
          <xsl:value-of select="$AllRowBreakes"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:if>

  </xsl:template>

  <xsl:template name="InsertElementsBetweenTwoColl">
    <xsl:param name="sheet"/>
    <xsl:param name="NameSheet"/>
    <xsl:param name="PictureCell"/>
    <xsl:param name="PictureRow"/>
    <xsl:param name="NoteCell"/>
    <xsl:param name="NoteRow"/>
    <xsl:param name="ConditionalCell"/>
    <xsl:param name="ConditionalCellStyle"/>
    <xsl:param name="ConditionalCellCol"/>
    <xsl:param name="ConditionalCellAll"/>
    <xsl:param name="ConditionalCellSingle"/>
    <xsl:param name="ConditionalCellMultiple"/>
    <xsl:param name="ElementsColl"/>
    <xsl:param name="prevColl" select="0"/>
    <xsl:param name="rowNum"/>
    <xsl:param name="sheetNr"/>
    <xsl:param name="EndColl"/>
    <xsl:param name="ValidationCell"/>
    <xsl:param name="ValidationRow"/>
    <xsl:param name="ValidationCellStyle"/>

    <xsl:variable name="GetMinColWithElement">
      <xsl:call-template name="GetMinRowWithPicture">
        <xsl:with-param name="PictureRow">
          <xsl:value-of select="$ElementsColl"/>
        </xsl:with-param>
        <xsl:with-param name="AfterRow">
          <xsl:value-of select="$prevColl"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="$GetMinColWithElement &lt; $EndColl">

        <xsl:call-template name="InsertEmptyColls">
          <xsl:with-param name="repeat">
            <xsl:value-of select="$GetMinColWithElement - $prevColl - 1"/>
          </xsl:with-param>
        </xsl:call-template>

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

        <xsl:if test="$GetMinColWithElement != ''">

          <table:table-cell>

            <!-- check if conditional -->
            <xsl:if
              test="@s or contains(concat(';', $ConditionalCell), concat(';', $rowNum, ':', $GetMinColWithElement, ';'))">
                  <xsl:attribute name="table:style-name">
                    <xsl:value-of
                      select="generate-id(key('ConditionalFormatting', ancestor::e:worksheet/@oox:part)[@oox:id = substring-before(substring-after(concat(';', $ConditionalCellStyle), concat(';', $rowNum, ':', $GetMinColWithElement, ';-')), ';')])"
                    />
                  </xsl:attribute>
            </xsl:if>
            
            <xsl:if
              test="contains(concat(';', $ValidationCell), concat(';', $rowNum, ':', $GetMinColWithElement, ';'))">
              <xsl:variable name="ValidationStyle">
                <xsl:value-of
                  select="substring-before(substring-after(concat(';', $ValidationCellStyle), concat(';', $rowNum, ':', $GetMinColWithElement, ';-')), ';')"
                />
              </xsl:variable>
              <xsl:attribute name="table:content-validation-name">
                <xsl:value-of select="concat('val', $sheetNr, $ValidationStyle + 1)"/>
              </xsl:attribute>
            </xsl:if>

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

            <xsl:if
              test="contains(concat(';', $NoteCell), concat(';', $rowNum, ':', $GetMinColWithElement, ';'))">

              <xsl:call-template name="InsertNoteInThisCell">
                <xsl:with-param name="sheetNr">
                  <xsl:value-of select="$sheetNr"/>
                </xsl:with-param>
                <xsl:with-param name="colNum">
                  <xsl:value-of select="$GetMinColWithElement"/>
                </xsl:with-param>
                <xsl:with-param name="rowNum">
                  <xsl:value-of select="$rowNum"/>
                </xsl:with-param>
              </xsl:call-template>

            </xsl:if>



          </table:table-cell>

          <xsl:call-template name="InsertElementsBetweenTwoColl">
            <xsl:with-param name="sheet">
              <xsl:value-of select="$sheet"/>
            </xsl:with-param>
            <xsl:with-param name="NameSheet">
              <xsl:value-of select="$NameSheet"/>
            </xsl:with-param>
            <xsl:with-param name="PictureCell">
              <xsl:value-of select="$PictureCell"/>
            </xsl:with-param>
            <xsl:with-param name="PictureRow">
              <xsl:value-of select="$PictureRow"/>
            </xsl:with-param>
            <xsl:with-param name="NoteCell">
              <xsl:value-of select="$NoteCell"/>
            </xsl:with-param>
            <xsl:with-param name="NoteRow">
              <xsl:value-of select="$NoteRow"/>
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
            <xsl:with-param name="ElementsColl">
              <xsl:value-of select="$ElementsColl"/>
            </xsl:with-param>
            <xsl:with-param name="rowNum">
              <xsl:value-of select="$rowNum"/>
            </xsl:with-param>
            <xsl:with-param name="prevColl">
              <xsl:value-of select="$GetMinColWithElement"/>
            </xsl:with-param>
            <xsl:with-param name="sheetNr" select="$sheetNr"/>
            <xsl:with-param name="EndColl">
              <xsl:value-of select="$EndColl"/>
            </xsl:with-param>
            <xsl:with-param name="ValidationCell">
              <xsl:value-of select="$ValidationCell"/>
            </xsl:with-param>
            <xsl:with-param name="ValidationRow">
              <xsl:value-of select="$ValidationRow"/>
            </xsl:with-param>
            <xsl:with-param name="ValidationCellStyle">
              <xsl:value-of select="$ValidationCellStyle"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:if>

      </xsl:when>

      <xsl:otherwise>

        <xsl:call-template name="InsertEmptyColls">
          <xsl:with-param name="repeat">
            <xsl:value-of select="$EndColl - $prevColl - 1"/>
          </xsl:with-param>
        </xsl:call-template>

      </xsl:otherwise>

    </xsl:choose>
  </xsl:template>

</xsl:stylesheet>
