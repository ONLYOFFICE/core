<?xml version="1.0" encoding="UTF-8"?>
<!--
  * Copyright (c) 2007, Clever Age
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
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main"
  xmlns:pzip="urn:cleverage:xmlns:post-processings:zip"
  xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:meta="urn:oasis:names:tc:opendocument:xmlns:meta:1.0"
  xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
  xmlns:pxsi="urn:cleverage:xmlns:post-processings:pivotTable"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  exclude-result-prefixes="text">

  <!--<xsl:import href="sharedStrings.xsl"/>-->

  <xsl:template name="InsertPivotTable">
    <!-- @Context: table:data-pilot-table -->

    <pivotTableDefinition applyNumberFormats="0" applyBorderFormats="0" applyFontFormats="0"
      applyPatternFormats="0" applyAlignmentFormats="0" applyWidthHeightFormats="1"
      dataCaption="Data" updatedVersion="3" minRefreshableVersion="3" showCalcMbrs="0"
      useAutoFormatting="1" itemPrintTitles="1" createdVersion="3" indent="0" outline="1"
      outlineData="1" multipleFieldFilters="0">

      <xsl:attribute name="name">
        <xsl:value-of select="@table:name"/>
      </xsl:attribute>

      <xsl:if test="@table:grand-total = 'none' or @table:grand-total = 'column' ">
        <xsl:attribute name="rowGrandTotals">
          <xsl:text>0</xsl:text>
        </xsl:attribute>
      </xsl:if>

      <xsl:if test="@table:grand-total = 'none' or @table:grand-total = 'row' ">
        <xsl:attribute name="colGrandTotals">
          <xsl:text>0</xsl:text>
        </xsl:attribute>
      </xsl:if>
<!--
	Changes By: Vijayeta
	Desc      : Crash Because of Pivot Table,Some times when a 'Value' field appears as row,
				instead of condition table:source-field-name = "", SP2 retains as table:source-field-name = "Values" or table:source-field-name = "Daten"
				In this case,code picks up all the fields from source into a container and all the fields of pivot table
				into another container, and then compares both the containers, which reults is false and node
				rowFields is not added.  So add additional check, if @table:is-data-layout-field is 'true', other cases, attr is absent.
	File      : TR_Bestellungen.DE.xlsx->SP2->TR_Bestellungen.DE.ods->Translator->TR_Bestellungen.DE.xlsx
-->
      <xsl:if
        test="table:data-pilot-field[@table:source-field-name = '' and @table:orientation = 'row' ]
		      or table:data-pilot-field[@table:source-field-name != '' and @table:orientation = 'row' and @table:is-data-layout-field='true']">
        <xsl:attribute name="dataOnRows">
          <xsl:text>1</xsl:text>
        </xsl:attribute>
      </xsl:if>

      <xsl:variable name="source">
        <xsl:value-of select="table:source-cell-range/@table:cell-range-address"/>
      </xsl:variable>

      <xsl:attribute name="cacheId">
        <!-- first pilot table on this source id -->
        <xsl:variable name="pivotSource">
          <xsl:value-of select="table:source-cell-range/@table:cell-range-address"/>
        </xsl:variable>

        <xsl:for-each select="key('pivot','')">
          <!-- do not duplicate the same source range cache -->
          <xsl:if
            test="table:source-cell-range/@table:cell-range-address = $pivotSource and 
            not(preceding-sibling::table:data-pilot-table[table:source-cell-range/@table:cell-range-address = $pivotSource])">
            <xsl:value-of select="count(preceding-sibling::table:data-pilot-table)"/>
          </xsl:if>
        </xsl:for-each>
      </xsl:attribute>

      <location firstHeaderRow="1" firstDataRow="2" firstDataCol="1">

        <xsl:variable name="pageFields">
          <xsl:value-of select="count(table:data-pilot-field[@table:orientation = 'page' ])"/>
        </xsl:variable>

        <xsl:if test="table:data-pilot-field[@table:orientation = 'page' ]">
          <xsl:attribute name="rowPageCount">
            <xsl:value-of select="$pageFields"/>
          </xsl:attribute>
          <xsl:attribute name="colPageCount">
            <xsl:text>1</xsl:text>
          </xsl:attribute>
        </xsl:if>

        <xsl:variable name="startRow">
          <xsl:call-template name="GetRowNum">
            <xsl:with-param name="cell">
              <xsl:value-of
                select="substring-after(substring-before(@table:target-range-address,':'),'.')"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:variable>

        <xsl:choose>
          <xsl:when test="@table:show-filter-button = 'false' ">
            <xsl:attribute name="ref">
              <xsl:value-of
                select="substring-before(substring-after(substring-before(@table:target-range-address,':'),'.'),$startRow)"/>
              <xsl:value-of select="$startRow + 1 + $pageFields"/>
              <xsl:text>:</xsl:text>
              <xsl:value-of
                select="substring-after(substring-after(@table:target-range-address,':'),'.')"/>
            </xsl:attribute>
          </xsl:when>
          <xsl:otherwise>

            <xsl:attribute name="ref">
              <!-- column-->
              <xsl:value-of
                select="substring-before(substring-after(substring-before(@table:target-range-address,':'),'.'),$startRow)"/>
              <xsl:value-of select="$startRow + 2 + $pageFields"/>
              <xsl:text>:</xsl:text>
              <xsl:value-of
                select="substring-after(substring-after(@table:target-range-address,':'),'.')"/>
            </xsl:attribute>
          </xsl:otherwise>
        </xsl:choose>

      </location>

      <pivotFields>

        <!--xsl:attribute name="count">

          <xsl:variable name="startCol">
            <xsl:call-template name="GetColNum">
              <xsl:with-param name="cell">
                <xsl:value-of
                  select="substring-after(substring-before(@table:target-range-address,':'),'.')"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:variable>

          <xsl:variable name="endCol">
            <xsl:call-template name="GetColNum">
              <xsl:with-param name="cell">
                <xsl:value-of
                  select="substring-after(substring-after(@table:target-range-address,':'),'.')"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:variable>

          <xsl:value-of select="$endCol - $startCol + 1"/>
        </xsl:attribute-->

        <xsl:variable name="names">
          <xsl:for-each select="table:data-pilot-field[@table:source-field-name != '' ]">
            <xsl:if test="position() &gt; 1 ">
              <xsl:text>~</xsl:text>
            </xsl:if>
            <xsl:value-of select="@table:source-field-name"/>
          </xsl:for-each>
        </xsl:variable>

        <xsl:variable name="axes">
          <xsl:for-each select="table:data-pilot-field[@table:source-field-name != '' ]">

            <xsl:variable name="name">
              <xsl:value-of select="@table:source-field-name"/>
            </xsl:variable>

            <xsl:if test="position() &gt; 1 ">
              <xsl:text>~</xsl:text>
            </xsl:if>

            <xsl:choose>
              <xsl:when test="@table:orientation = 'data' ">
                <xsl:text>data</xsl:text>

                <xsl:choose>
                  <xsl:when
                    test="following-sibling::table:data-pilot-field[@table:source-field-name = $name and @table:orientation = 'page']">
                    <xsl:text>,axisPage</xsl:text>
                  </xsl:when>
                  <xsl:when
                    test="following-sibling::table:data-pilot-field[@table:source-field-name = $name and @table:orientation = 'column']">
                    <xsl:text>,axisCol</xsl:text>
                  </xsl:when>
                  <xsl:when
                    test="following-sibling::table:data-pilot-field[@table:source-field-name = $name and @table:orientation = 'row']">
                    <xsl:text>,axisRow</xsl:text>
                  </xsl:when>
                </xsl:choose>

              </xsl:when>
              <xsl:otherwise>

                <xsl:choose>
                  <xsl:when test="@table:orientation = 'page' ">
                    <xsl:text>axisPage</xsl:text>
                  </xsl:when>
                  <xsl:when test="@table:orientation = 'column' ">
                    <xsl:text>axisCol</xsl:text>
                  </xsl:when>
                  <xsl:when test="@table:orientation = 'row' ">
                    <xsl:text>axisRow</xsl:text>
                  </xsl:when>
                </xsl:choose>

                <xsl:if
                  test="following-sibling::table:data-pilot-field[@table:source-field-name = $name and @table:orientation = 'data']">
                  <xsl:text>,data</xsl:text>
                </xsl:if>

              </xsl:otherwise>
            </xsl:choose>
          </xsl:for-each>
        </xsl:variable>

        <xsl:variable name="sort">
          <xsl:for-each select="table:data-pilot-field[@table:source-field-name != '' ]">
            <xsl:if test="position() &gt; 1 ">
              <xsl:text>~</xsl:text>
            </xsl:if>
            <xsl:choose>
              <xsl:when
                test="table:data-pilot-level/table:data-pilot-sort-info[@table:order = 'descending' and @table:sort-mode != 'manual' and @table:sort-mode != 'none']">
                <xsl:text>descending</xsl:text>
              </xsl:when>
              <xsl:when
                test="table:data-pilot-level/table:data-pilot-sort-info[@table:order = 'ascending' and @table:sort-mode != 'manual' and @table:sort-mode != 'none'] ">
                <xsl:text>ascending</xsl:text>
              </xsl:when>
            </xsl:choose>
          </xsl:for-each>
        </xsl:variable>

        <xsl:variable name="hide">
          <xsl:for-each select="table:data-pilot-field[@table:source-field-name != '' ]">
            <xsl:if test="position() &gt; 1 ">
              <xsl:text>~</xsl:text>
            </xsl:if>
            <xsl:if
              test="table:data-pilot-level/table:data-pilot-members/table:data-pilot-member[@table:display = 'false' ]">
              <xsl:text>;</xsl:text>
              <xsl:for-each
                select="table:data-pilot-level/table:data-pilot-members/table:data-pilot-member[@table:display = 'false' ]">
                <xsl:value-of select="@table:name"/>
                <xsl:text>;</xsl:text>
              </xsl:for-each>
            </xsl:if>
          </xsl:for-each>
        </xsl:variable>

        <xsl:variable name="subtotals">
          <xsl:for-each select="table:data-pilot-field[@table:source-field-name != '' ]">
            <xsl:if test="position() &gt; 1 ">
              <xsl:text>~</xsl:text>
            </xsl:if>

            <xsl:if
              test="table:data-pilot-level/table:data-pilot-subtotals/table:data-pilot-subtotal[@table:function]">

              <xsl:for-each
                select="table:data-pilot-level/table:data-pilot-subtotals/table:data-pilot-subtotal[@table:function]">

                <xsl:if test="position() &gt; 1 ">
                  <xsl:text>;</xsl:text>
                </xsl:if>

                <xsl:choose>
                  <xsl:when test="@table:function = 'sum' ">
                    <xsl:text>sum</xsl:text>
                  </xsl:when>
                  <xsl:when test="@table:function = 'count' ">
                    <xsl:text>countA</xsl:text>
                  </xsl:when>
                  <xsl:when test="@table:function = 'average' ">
                    <xsl:text>avg</xsl:text>
                  </xsl:when>
                  <xsl:when test="@table:function = 'max' ">
                    <xsl:text>max</xsl:text>
                  </xsl:when>
                  <xsl:when test="@table:function = 'min' ">
                    <xsl:text>min</xsl:text>
                  </xsl:when>
                  <xsl:when test="@table:function = 'product' ">
                    <xsl:text>product</xsl:text>
                  </xsl:when>
                  <xsl:when test="@table:function = 'countnums' ">
                    <xsl:text>count</xsl:text>
                  </xsl:when>
                  <xsl:when test="@table:function = 'stdev' ">
                    <xsl:text>stdDev</xsl:text>
                  </xsl:when>
                  <xsl:when test="@table:function = 'stdevp' ">
                    <xsl:text>stdDevP</xsl:text>
                  </xsl:when>
                  <xsl:when test="@table:function = 'var' ">
                    <xsl:text>var</xsl:text>
                  </xsl:when>
                  <xsl:when test="@table:function = 'varp' ">
                    <xsl:text>varP</xsl:text>
                  </xsl:when>
                  <xsl:when test="@table:function = 'auto' ">
                    <xsl:text>default</xsl:text>
                  </xsl:when>
                </xsl:choose>
              </xsl:for-each>
            </xsl:if>
          </xsl:for-each>
        </xsl:variable>

        <xsl:variable name="blanks">
          <xsl:for-each select="table:data-pilot-field[@table:source-field-name != '' ]">
            <xsl:if test="position() &gt; 1 ">
              <xsl:text>~</xsl:text>
            </xsl:if>
            <xsl:choose>
              <xsl:when test="table:data-pilot-level[@table:show-empty = 'false' ]">
                <xsl:text>false</xsl:text>
              </xsl:when>
              <xsl:otherwise>
                <xsl:text>true</xsl:text>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:for-each>
        </xsl:variable>

        <xsl:variable name="outlines">
          <xsl:for-each select="table:data-pilot-field[@table:source-field-name != '' ]">
            <xsl:if test="position() &gt; 1 ">
              <xsl:text>~</xsl:text>
            </xsl:if>
            <xsl:choose>
              <xsl:when
                test="table:data-pilot-level/table:data-pilot-layout-info/@table:layout-mode = 'tabular-layout' ">
                <xsl:text>0</xsl:text>
              </xsl:when>
              <xsl:when
                test="table:data-pilot-level/table:data-pilot-layout-info/@table:layout-mode = 'outline-subtotals-bottom' ">
                <xsl:text>bottom</xsl:text>
              </xsl:when>
              <xsl:when
                test="table:data-pilot-level/table:data-pilot-layout-info/@table:layout-mode = 'outline-subtotals-top' ">
                <xsl:text>top</xsl:text>
              </xsl:when>
            </xsl:choose>
          </xsl:for-each>
        </xsl:variable>

        <xsl:variable name="emptyLines">
          <xsl:for-each select="table:data-pilot-field[@table:source-field-name != '' ]">
            <xsl:if test="position() &gt; 1 ">
              <xsl:text>~</xsl:text>
            </xsl:if>
            <xsl:choose>
              <xsl:when
                test="table:data-pilot-level/table:data-pilot-layout-info/@table:add-empty-lines = 'true' ">
                <xsl:text>1</xsl:text>
              </xsl:when>
              <xsl:otherwise>
                <xsl:text>0</xsl:text>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:for-each>
        </xsl:variable>

        <pxsi:pivotFields>
          <xsl:attribute name="pxsi:names">
            <xsl:value-of select="$names"/>
          </xsl:attribute>

          <xsl:attribute name="pxsi:axes">
            <xsl:value-of select="$axes"/>
          </xsl:attribute>

          <xsl:attribute name="pxsi:sort">
            <xsl:value-of select="$sort"/>
          </xsl:attribute>

          <xsl:attribute name="pxsi:hide">
            <xsl:value-of select="$hide"/>
          </xsl:attribute>

          <xsl:attribute name="pxsi:subtotals">
            <xsl:value-of select="$subtotals"/>
          </xsl:attribute>

          <xsl:attribute name="pxsi:blanks">
            <xsl:value-of select="$blanks"/>
          </xsl:attribute>

          <xsl:attribute name="pxsi:outlines">
            <xsl:value-of select="$outlines"/>
          </xsl:attribute>

          <xsl:attribute name="pxsi:emptyLines">
            <xsl:value-of select="$emptyLines"/>
          </xsl:attribute>
        </pxsi:pivotFields>

        <xsl:variable name="sourceStartCol">
          <xsl:call-template name="GetColNum">
            <xsl:with-param name="cell">
              <xsl:value-of
                select="substring-after(substring-before(table:source-cell-range/@table:cell-range-address,':'),'.')"
              />
            </xsl:with-param>
          </xsl:call-template>
        </xsl:variable>

        <xsl:variable name="sourceEndCol">
          <xsl:call-template name="GetColNum">
            <xsl:with-param name="cell">
              <xsl:value-of
                select="substring-after(substring-after(table:source-cell-range/@table:cell-range-address,':'),'.')"
              />
            </xsl:with-param>
          </xsl:call-template>
        </xsl:variable>

      </pivotFields>

      <xsl:variable name="axedFields">
        <xsl:value-of
          select="count(table:data-pilot-field[@table:source-field-name != '' and @table:orientation != 'data' ])"
        />
      </xsl:variable>

      <xsl:variable name="dataFields">
        <xsl:value-of
          select="count(table:data-pilot-field[@table:source-field-name != '' and @table:orientation = 'data' ])"
        />
      </xsl:variable>

      <xsl:if
        test="table:data-pilot-field[@table:source-field-name != '' and @table:orientation = 'row' ] or $dataFields &gt;= 2">
        <rowFields count="{count(table:data-pilot-field[@table:orientation = 'row'])}">
          <xsl:for-each select="table:data-pilot-field[@table:orientation = 'row']">
            <xsl:choose>
              <!-- "Values" field -->
				<!--
				Changes By: Vijayeta
				Desc      : Crash Because of Pivot Table,Some times when a 'Value' field appears as row,
				instead of condition table:source-field-name = "", SP2 retains as table:source-field-name = "Values" or table:source-field-name = "Daten"
				In this case,code picks up all the fields from source into a container and all the fields of pivot table
				into another container, and then compares both the containers, which reults is false and node
				rowFields is not added.  So add additional check, if @table:is-data-layout-field is 'true', other cases, attr is absent.
				File      : TR_Bestellungen.DE.xlsx->SP2->TR_Bestellungen.DE.ods->Translator->TR_Bestellungen.DE.xlsx
				-->
              <xsl:when test="@table:source-field-name = '' and $dataFields &gt;= 2">
                <field x="-2"/>
              </xsl:when>
			  <xsl:when test="@table:source-field-name != '' and @table:is-data-layout-field='true' and $dataFields &gt;= 2">
				<field x="-2"/>
			  </xsl:when>
              <xsl:when test="@table:source-field-name != '' ">
                <field>
                  <pxsi:fieldPos pxsi:name="{@table:source-field-name}" pxsi:attribute="x"/>
                </field>
              </xsl:when>
            </xsl:choose>
          </xsl:for-each>

        </rowFields>
      </xsl:if>

      <!-- TO DO <rowItems> -->

      <xsl:if test="table:data-pilot-field[@table:orientation = 'column' ]">
        <colFields
          count="{count(table:data-pilot-field[@table:source-field-name != '' and @table:orientation = 'column'])}">

          <xsl:for-each select="table:data-pilot-field[@table:orientation = 'column']">

            <xsl:choose>
              <!-- "Values" field -->
				<!-- 
				  Added: By Vijayeta, 
                  Date:  1/9/2009
                  Desc: Crash Because of Pivot Table,Some times when a 'Value' field appears as column/row,
					    instead of condition table:source-field-name = "", SP2 retains as table:source-field-name = "Values"
						In this case,code picks up all the fields from source into a container and all the fields of pivot table
						into another container, and then compares both the containers, which reults is false and node
						colFields/rowfields is not added.  So add additional check, if @table:is-data-layout-field is 'true', other cases, attr is absent
                  File:  Define_Name_Pivot_Table.ods(SP2 output of Define_Name_Pivot_Table.xlsx)
				 -->
              <xsl:when test="@table:source-field-name = '' or @table:is-data-layout-field='true' or table:data-pilot-level[@table:show-empty='true']">
                <field x="-2"/>
              </xsl:when>
              <xsl:when test="@table:source-field-name != '' ">
                <!-- colfields need to be handled by post-processor in order to avoid crashes -->
                <field pxsi:table-source-field-name="{concat(@table:source-field-name,'pxsi')}">
                  <pxsi:fieldPos pxsi:name="{@table:source-field-name}" pxsi:attribute="x"/>
                </field>
              </xsl:when>
            </xsl:choose>
          </xsl:for-each>
        </colFields>
      </xsl:if>

      <!-- page fields -->
      <xsl:if test="table:data-pilot-field[@table:orientation = 'page' ]">
        <pageFields>
          <xsl:for-each select="table:data-pilot-field[@table:orientation = 'page' ]">
            <pageField hier="-1">

              <pxsi:fieldPos pxsi:name="{@table:source-field-name}" pxsi:attribute="fld"/>

              <xsl:if test="@table:selected-page">
                <pxsi:pageItem>
                  <xsl:attribute name="pxsi:pageField">
                    <xsl:value-of select="@table:source-field-name"/>
                  </xsl:attribute>
                  <xsl:attribute name="pxsi:pageItem">
                    <xsl:value-of select="@table:selected-page"/>
                  </xsl:attribute>
                </pxsi:pageItem>
              </xsl:if>

            </pageField>
          </xsl:for-each>
        </pageFields>
      </xsl:if>

      <!-- date fields -->
      <xsl:if
        test="table:data-pilot-field[@table:source-field-name != '' and @table:orientation = 'data' ]">
        <dataFields
          count="{count(table:data-pilot-field[@table:source-field-name != '' and @table:orientation = 'data'])}">
          <xsl:for-each
            select="table:data-pilot-field[@table:source-field-name != '' and @table:orientation = 'data']">
            <dataField>

              <xsl:attribute name="name">
                <!-- Function name with uppercase first letter -->
                <xsl:choose>
                  <xsl:when test="@table:function = 'countnums' ">
                    <xsl:text>Count</xsl:text>
                  </xsl:when>
                  <xsl:when test="@table:function = 'stdev' ">
                    <xsl:text>StdDev</xsl:text>
                  </xsl:when>
                  <xsl:when test="@table:function = 'stdevp' ">
                    <xsl:text>StdDevp</xsl:text>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of
                      select="translate(substring(@table:function,1,1),'scampv','SCAMPV')"/>
                    <xsl:value-of select="substring(@table:function,2)"/>
                  </xsl:otherwise>
                </xsl:choose>
                <xsl:text> of </xsl:text>
                <xsl:value-of select="@table:source-field-name"/>
              </xsl:attribute>

              <xsl:if test="@table:function != 'sum'  and @table:function != 'auto'">

                <xsl:attribute name="subtotal">
                  <xsl:choose>
                    <xsl:when test="@table:function = 'countnums' ">
                      <xsl:text>countNums</xsl:text>
                    </xsl:when>
                    <xsl:when test="@table:function = 'stdev' ">
                      <xsl:text>stdDev</xsl:text>
                    </xsl:when>
                    <xsl:when test="@table:function = 'stdevp' ">
                      <xsl:text>stdDevp</xsl:text>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:value-of select="@table:function"/>
                    </xsl:otherwise>
                  </xsl:choose>
                </xsl:attribute>

              </xsl:if>

              <!-- show data format as -->
              <xsl:for-each select="table:data-pilot-field-reference[@table:type]">

                <xsl:attribute name="showDataAs">

                  <xsl:choose>
                    <xsl:when test="@table:type= 'member-difference' ">
                      <xsl:text>difference</xsl:text>
                    </xsl:when>

                    <xsl:when test="@table:type= 'member-percentage' ">
                      <xsl:text>percent</xsl:text>
                    </xsl:when>

                    <xsl:when test="@table:type= 'member-percentage-difference' ">
                      <xsl:text>percentDiff</xsl:text>
                    </xsl:when>

                    <xsl:when test="@table:type= 'running-total' ">
                      <xsl:text>runTotal</xsl:text>
                    </xsl:when>

                    <xsl:when test="@table:type= 'row-percentage' ">
                      <xsl:text>percentOfRow</xsl:text>
                    </xsl:when>

                    <xsl:when test="@table:type= 'column-percentage' ">
                      <xsl:text>percentOfColumn</xsl:text>
                    </xsl:when>

                    <xsl:when test="@table:type= 'total-percentage' ">
                      <xsl:text>percentOfTotal</xsl:text>
                    </xsl:when>

                    <xsl:when test="@table:type= 'index' ">
                      <xsl:text>index</xsl:text>
                    </xsl:when>

                  </xsl:choose>
                </xsl:attribute>
              </xsl:for-each>

              <xsl:attribute name="baseField">
                <xsl:text>0</xsl:text>
              </xsl:attribute>

              <xsl:attribute name="baseItem">
                <xsl:text>0</xsl:text>
              </xsl:attribute>

              <pxsi:fieldPos pxsi:name="{@table:source-field-name}" pxsi:attribute="fld"/>

            </dataField>
          </xsl:for-each>
        </dataFields>
      </xsl:if>

      <pivotTableStyleInfo name="PivotStyleLight16" showRowHeaders="1" showColHeaders="1"
        showRowStripes="0" showColStripes="0" showLastColumn="1"/>

      <!-- insert filters -->
      <xsl:if test="parent::node()/table:data-pilot-table/table:source-cell-range/table:filter">
        <filters>
          <xsl:for-each
            select="table:source-cell-range/table:filter/child::node()[name() ='table:filter-or' or name() ='table:filter-and']/table:filter-condition[@table:field-number]">

            <xsl:variable name="pivotIdFilter">
              <xsl:value-of select="@table:field-number"/>
            </xsl:variable>

            <!--xsl:variable name="dataPilotFieldPosition">
              <xsl:for-each select="ancestor::table:data-pilot-table/table:data-pilot-field[@table:orientation = 'row' or @table:orientation = 'column' or @table:orientation = 'page']">
                <xsl:value-of select="position()"/>
              </xsl:for-each>
            </xsl:variable-->

            <xsl:if
              test="not(preceding-sibling::table:filter-condition[@table:field-number = $pivotIdFilter])">

              <filter evalOrder="-1" id="10">

                <xsl:variable name="fieldType">
                  <xsl:for-each
                    select="ancestor::table:data-pilot-table/table:data-pilot-field[@table:source-field-name != ''][position() = $pivotIdFilter + 1]">
                    <xsl:value-of select="@table:orientation"/>
                  </xsl:for-each>
                </xsl:variable>

                <!--xsl:for-each
                  select="ancestor::table:data-pilot-table/table:data-pilot-field[@table:source-field-name != ''][position() = $pivotIdFilter + 1][@table:orientation='row' or @table:orientation='column' or @table:orientation='page' ]">
                  <xsl:attribute name="stringValue1">
                    <xsl:value-of select="parent::node()/table:source-cell-range/table:filter/child::node()[name() ='table:filter-or' or name() ='table:filter-and']/table:filter-condition[ position() = $dataPilotFieldPosition - 1]/@table:value"/>
                  </xsl:attribute>
                </xsl:for-each-->

                <xsl:attribute name="fld">
                  <xsl:value-of select="@table:field-number"/>
                </xsl:attribute>

                <pxsi:filterType pxsi:filterField="{@table:field-number}"
                  pxsi:filterValue="{@table:value}">
                  <xsl:attribute name="pxsi:condition">

                    <xsl:choose>
                      <xsl:when test="@table:operator='=' ">
                        <xsl:text>Equal</xsl:text>
                      </xsl:when>

                      <xsl:when test="@table:operator='!=' ">
                        <xsl:text>NotEqual</xsl:text>
                      </xsl:when>

                      <xsl:when test="@table:operator='&lt;' ">
                        <xsl:text>LessThan</xsl:text>
                      </xsl:when>

                      <xsl:when test="@table:operator='&lt;=' ">
                        <xsl:text>LessThanOrEqual</xsl:text>
                      </xsl:when>

                      <xsl:when test="@table:operator='&gt;' ">
                        <xsl:text>GreaterThan</xsl:text>
                      </xsl:when>

                      <xsl:when test="@table:operator='&gt;=' ">
                        <xsl:text>GreaterThanOrEqual</xsl:text>
                      </xsl:when>
                    </xsl:choose>

                  </xsl:attribute>
                </pxsi:filterType>

                <autoFilter ref="A1">
                  <filterColumn colId="0">
                    <customFilters>
                      <xsl:call-template name="InsertFilters"/>
                    </customFilters>
                  </filterColumn>
                </autoFilter>
              </filter>
            </xsl:if>
          </xsl:for-each>

          <!--xsl:if test="table:source-cell-range/table:filter[@table:field-number]">
            <filter>
              
              <xsl:attribute name="fld">
                <xsl:value-of select="@table:field-number"/>
              </xsl:attribute>
             
              <autoFilter ref="A1">
                <filterColumn colId="0">
                <customFilters>
                <customFilter operator="lessThan" val="3"/>
                <customFilter operator="greaterThan" val="7"/>
                </customFilters>
                </filterColumn>
                </autoFilter>
            </filter>
          </xsl:if-->

        </filters>

      </xsl:if>

    </pivotTableDefinition>

  </xsl:template>

  <xsl:template name="InsertFilters">

    <xsl:variable name="pivotIdFilter">
      <xsl:value-of select="@table:field-number"/>
    </xsl:variable>

    <customFilter val="3">

      <xsl:if test="@table:operator != '=' ">

        <xsl:attribute name="operator">
          <xsl:choose>

            <xsl:when test="@table:operator='=' ">
              <xsl:text>equal</xsl:text>
            </xsl:when>

            <xsl:when test="@table:operator='!=' ">
              <xsl:text>notEqual</xsl:text>
            </xsl:when>

            <xsl:when test="@table:operator='&lt;' ">
              <xsl:text>lessThan</xsl:text>
            </xsl:when>

            <xsl:when test="@table:operator='&lt;=' ">
              <xsl:text>lessThanOrEqual</xsl:text>
            </xsl:when>

            <xsl:when test="@table:operator='&gt;' ">
              <xsl:text>greaterThan</xsl:text>
            </xsl:when>

            <xsl:when test="@table:operator='&gt;=' ">
              <xsl:text>greaterThanOrEqual</xsl:text>
            </xsl:when>

          </xsl:choose>
        </xsl:attribute>

      </xsl:if>

      <xsl:attribute name="val">
        <xsl:value-of select="@table:value"/>
      </xsl:attribute>

    </customFilter>

    <xsl:if test="following-sibling::table:filter-condition[@table:field-number = $pivotIdFilter]">
      <xsl:for-each
        select="following-sibling::table:filter-condition[@table:field-number = $pivotIdFilter][1]">
        <xsl:call-template name="InsertFilters"/>
      </xsl:for-each>
    </xsl:if>

  </xsl:template>
	<!--
	    * Defect Id       :1898488
		* Code Changed by :Vijayeta Tilak
		* Date            :20th May '08
		* Description     :On roundtrip, the change data source option in xlsx gives the file name and its path 
		                   along with the source data range. To prevent this, additional function 'translate' added to set the 
						   value of attribute 'sheet'.
    -->
  <xsl:template name="InsertCacheDefinition">
	  <xsl:variable name ="apos">
		  <xsl:text >&apos;</xsl:text>
	  </xsl:variable>
    <!-- @Context: table:data-pilot-table -->
    <pivotCacheDefinition r:id="{generate-id(.)}" refreshedBy="Author"
      refreshedDate="39328.480206134256" createdVersion="3" refreshedVersion="3"
      minRefreshableVersion="3" recordCount="32">

      <cacheSource type="worksheet">
        <worksheetSource ref="E1:G33" sheet="Sheet2">
          <xsl:attribute name="ref">
            <xsl:value-of
              select="substring-after(substring-before(table:source-cell-range/@table:cell-range-address,':'),'.')"/>
            <xsl:text>:</xsl:text>
            <xsl:value-of
              select="substring-after(substring-after(table:source-cell-range/@table:cell-range-address,':'),'.')"/>
          </xsl:attribute>

          <xsl:attribute name="sheet">
            <xsl:value-of
              select="translate(substring-before(table:source-cell-range/@table:cell-range-address,'.'),$apos,'')"/>
          </xsl:attribute>
        </worksheetSource>
      </cacheSource>

      <cacheFields>

        <pxsi:cacheFields/>

      </cacheFields>

    </pivotCacheDefinition>
  </xsl:template>

  <xsl:template name="InsertCacheRecords">
    <!-- @Context: table:data-pilot-table -->

    <pivotCacheRecords>

      <pxsi:cacheRecords/>

    </pivotCacheRecords>

  </xsl:template>

  <xsl:template name="GetCacheRows">
    <xsl:param name="rowStart"/>
    <xsl:param name="rowEnd"/>
    <xsl:param name="colStart"/>
    <xsl:param name="colEnd"/>
    <xsl:param name="rowNum" select="1"/>
    <xsl:param name="sheetName"/>
	  <!--Vijayeta,SP2,@table:number-rows-repeated-->
    <!--<xsl:variable name="rows">
      <xsl:choose>
        <xsl:when test="@table:number-rows-repeated">
          <xsl:value-of select="@table:number-rows-repeated"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>1</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>-->
    <xsl:variable name="rows">
      <xsl:choose>
        <xsl:when test="@table:number-rows-repeated">
				  <xsl:choose >
					  <xsl:when test ="@table:number-rows-repeated &gt; 65536">
						  <xsl:value-of select ="65536 - (1048576 - @table:number-rows-repeated)"/>
					  </xsl:when>
					  <xsl:when test ="@table:number-rows-repeated &lt;= 65536">
          <xsl:value-of select="@table:number-rows-repeated"/>
        </xsl:when>
				  </xsl:choose>
			  </xsl:when>
        <xsl:otherwise>
          <xsl:text>1</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
	  <!--Vijayeta,SP2,@table:number-rows-repeated,End-->
    <xsl:choose>
      <!-- skip rows before start of the source -->
      <xsl:when test="$rowNum + $rows &lt;= $rowStart">
        <xsl:for-each
          select="following::table:table-row[ancestor::table:table/@table:name = $sheetName][1]">
          <xsl:call-template name="GetCacheRows">
            <xsl:with-param name="rowStart" select="$rowStart"/>
            <xsl:with-param name="rowEnd" select="$rowEnd"/>
            <xsl:with-param name="colStart" select="$colStart"/>
            <xsl:with-param name="colEnd" select="$colEnd"/>
            <xsl:with-param name="rowNum" select="$rowNum + $rows"/>
          </xsl:call-template>
        </xsl:for-each>
      </xsl:when>
      <xsl:when test="$rowNum &lt;= $rowEnd">
		  <!--Vijayeta,SP2,@table:number-rows-repeated-->
		  <xsl:variable name ="tableNumberRowsRepeated">
        <xsl:choose>
			  <xsl:when test ="@table:number-rows-repeated &gt; 65536">
				  <xsl:value-of select ="65536 - (1048576 - @table:number-rows-repeated)"/>
			  </xsl:when>
			  <xsl:when test ="@table:number-rows-repeated &lt;= 65536">
				  <xsl:value-of select="@table:number-rows-repeated"/>
			  </xsl:when>
		  </xsl:choose>
		  </xsl:variable>
        <xsl:choose>
          <xsl:when test="$tableNumberRowsRepeated &gt; 1">
			  <!--Vijayeta,SP2,@table:number-rows-repeated,End-->
            <xsl:call-template name="InsertRepeatedCacheRow">
              <xsl:with-param name="colStart" select="$colStart"/>
              <xsl:with-param name="colEnd" select="$colEnd"/>
              <xsl:with-param name="repeat" select="$rows"/>
              <xsl:with-param name="rowNum" select="$rowNum"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="InsertCacheRow">
              <xsl:with-param name="colStart" select="$colStart"/>
              <xsl:with-param name="colEnd" select="$colEnd"/>
              <xsl:with-param name="rowNum" select="$rowNum"/>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>

        <xsl:for-each
          select="following::table:table-row[ancestor::table:table/@table:name = $sheetName][1]">
          <xsl:call-template name="GetCacheRows">
            <xsl:with-param name="rowStart" select="$rowStart"/>
            <xsl:with-param name="rowEnd" select="$rowEnd"/>
            <xsl:with-param name="colStart" select="$colStart"/>
            <xsl:with-param name="colEnd" select="$colEnd"/>
            <xsl:with-param name="sheetName" select="$sheetName"/>
            <xsl:with-param name="rowNum" select="$rowNum + $rows"/>
          </xsl:call-template>
        </xsl:for-each>
      </xsl:when>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="InsertRepeatedCacheRow">
    <xsl:param name="colStart"/>
    <xsl:param name="colEnd"/>
    <xsl:param name="repeat"/>
    <xsl:param name="count" select="1"/>
    <!-- temporary parameter -->
    <xsl:param name="rowNum"/>

    <xsl:if test="$count &lt;= $repeat">
      <xsl:call-template name="InsertCacheRow">
        <xsl:with-param name="colStart" select="$colStart"/>
        <xsl:with-param name="colEnd" select="$colEnd"/>
        <xsl:with-param name="rowNum" select="$rowNum"/>
      </xsl:call-template>

      <xsl:call-template name="InsertRepeatedCacheRow">
        <xsl:with-param name="colStart" select="$colStart"/>
        <xsl:with-param name="colEnd" select="$colEnd"/>
        <xsl:with-param name="repeat" select="$repeat"/>
        <xsl:with-param name="count" select="$count + 1"/>
        <xsl:with-param name="rowNum" select="$rowNum + 1"/>
      </xsl:call-template>
    </xsl:if>

  </xsl:template>

  <xsl:template name="InsertCacheRow">
    <xsl:param name="colStart"/>
    <xsl:param name="colEnd"/>
    <!-- temporary parameter -->
    <xsl:param name="rowNum"/>

    <r>
      <xsl:attribute name="c">
        <xsl:value-of select="$rowNum"/>
      </xsl:attribute>
      <xsl:call-template name="InsertCacheRowFields">
        <xsl:with-param name="colStart" select="$colStart"/>
        <xsl:with-param name="colEnd" select="$colEnd"/>
      </xsl:call-template>
    </r>

  </xsl:template>

  <xsl:template name="InsertCacheRowFields">
    <xsl:param name="colStart"/>
    <xsl:param name="colEnd"/>
    <xsl:param name="colNum" select="1"/>

	  <!--Vijayeta,SP2,@table:number-columns-repeated-->
    <!--<xsl:variable name="columns">
      <xsl:choose>
        <xsl:when test="@table:number-columns-repeated">
          <xsl:value-of select="@table:number-columns-repeated"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>1</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>-->

    <xsl:variable name="columns">
      <!--<xsl:choose>
        <xsl:when test="@table:number-columns-repeated">
				  <xsl:choose >
					  <xsl:when test ="@table:number-columns-repeated &gt; 256">
						  <xsl:value-of select ="256 - (16384 - @table:number-columns-repeated)"/>
					  </xsl:when>
					  <xsl:when test ="@table:number-columns-repeated &lt;= 256">
          <xsl:value-of select="@table:number-columns-repeated"/>
        </xsl:when>
				  </xsl:choose>
			  </xsl:when>
        <xsl:otherwise>
          <xsl:text>1</xsl:text>
        </xsl:otherwise>
      </xsl:choose>-->
		<xsl:variable name ="tempOfficeVersion">
			<xsl:value-of select ="document('meta.xml')/office:document-meta/office:meta/meta:generator"/>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="@table:number-columns-repeated">
				<xsl:choose>
					<xsl:when test ="starts-with($tempOfficeVersion,'OpenOffice.org')">
						<xsl:value-of select="@table:number-columns-repeated"/>
					</xsl:when>
					<xsl:when test ="starts-with($tempOfficeVersion,'MicrosoftOffice')">
						<xsl:choose>
							<xsl:when test ="@table:number-columns-repeated &gt; 256">
								<xsl:value-of select ="256 - (16384 - @table:number-columns-repeated)"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="@table:number-columns-repeated"/>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="@table:number-columns-repeated"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>1</xsl:text>
			</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

	  <!--Vijayeta,SP2,@table:number-columns-repeated,End-->
    <xsl:choose>
      <!-- skip cells before start of the source -->
      <xsl:when test="$colNum &lt; $colStart and $colNum + $columns - 1 &lt; $colStart">
        <xsl:for-each select="following-sibling::node()[1]">
          <xsl:call-template name="InsertCacheRowFields">
            <xsl:with-param name="colStart" select="$colStart"/>
            <xsl:with-param name="colEnd" select="$colEnd"/>
            <xsl:with-param name="colNum" select="$colNum + $columns"/>
          </xsl:call-template>
        </xsl:for-each>
      </xsl:when>

      <!-- when single column is inside source -->
      <xsl:when test="$columns = 1 and $colNum &lt;= $colEnd">

        <xsl:call-template name="InsertCacheRowField">
          <xsl:with-param name="colNum" select="$colNum"/>
        </xsl:call-template>

        <xsl:for-each select="following-sibling::node()[1]">
          <xsl:call-template name="InsertCacheRowFields">
            <xsl:with-param name="colStart" select="$colStart"/>
            <xsl:with-param name="colEnd" select="$colEnd"/>
            <xsl:with-param name="colNum" select="$colNum + $columns"/>
          </xsl:call-template>
        </xsl:for-each>
      </xsl:when>

      <!-- when whole repeated column is inside source -->
      <xsl:when
        test="$columns &gt; 1 and $colNum &gt;= $colStart and $colNum + $columns - 1 &lt;= $colEnd">

        <xsl:call-template name="InsertRepeatedCacheRowFields">
          <xsl:with-param name="repeat" select="$columns"/>
          <xsl:with-param name="colNum" select="$colNum"/>
        </xsl:call-template>

        <xsl:for-each select="following-sibling::node()[1]">
          <xsl:call-template name="InsertCacheRowFields">
            <xsl:with-param name="colStart" select="$colStart"/>
            <xsl:with-param name="colEnd" select="$colEnd"/>
            <xsl:with-param name="colNum" select="$colNum + $columns"/>
          </xsl:call-template>
        </xsl:for-each>
      </xsl:when>

      <!-- when whole source is inside repeated columns -->
      <xsl:when test="$colNum &lt;= $colStart and $colNum + $columns - 1 &gt;= $colEnd">

        <xsl:call-template name="InsertRepeatedCacheRowFields">
          <xsl:with-param name="repeat" select="$colEnd - $colStart + 1"/>
          <xsl:with-param name="colNum" select="$colNum"/>
        </xsl:call-template>

      </xsl:when>

      <!-- when source starts inside repeated columns 
        (if $colNum + $columns - 1 is also lower than $colEnd than it will be covered by the first when )-->
      <xsl:when test="$colNum &lt; $colStart and $colNum + $columns - 1 &lt;= $colEnd">

        <xsl:call-template name="InsertRepeatedCacheRowFields">
          <xsl:with-param name="repeat" select="$columns - ($colStart - $colNum)"/>
          <xsl:with-param name="colNum" select="$colNum"/>
        </xsl:call-template>

        <xsl:for-each select="following-sibling::node()[1]">
          <xsl:call-template name="InsertCacheRowFields">
            <xsl:with-param name="colStart" select="$colStart"/>
            <xsl:with-param name="colEnd" select="$colEnd"/>
            <xsl:with-param name="colNum" select="$colNum + $columns"/>
          </xsl:call-template>
        </xsl:for-each>
      </xsl:when>

      <!-- when source ends inside repeated columns -->
      <xsl:when test="$colNum &lt;= $colEnd and $colNum + $columns - 1 &gt; $colEnd">

        <xsl:call-template name="InsertRepeatedCacheRowFields">
          <xsl:with-param name="repeat" select="$colEnd - $colNum + 1"/>
          <xsl:with-param name="colNum" select="$colNum"/>
        </xsl:call-template>

      </xsl:when>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="InsertRepeatedCacheRowFields">
    <xsl:param name="repeat"/>
    <xsl:param name="count" select="1"/>
    <!-- temporary parameter -->
    <xsl:param name="colNum"/>

    <xsl:if test="$count &lt;= $repeat">
      <xsl:call-template name="InsertCacheRowField">
        <xsl:with-param name="colNum" select="$colNum"/>
      </xsl:call-template>

      <xsl:call-template name="InsertRepeatedCacheRowFields">
        <xsl:with-param name="repeat" select="$repeat"/>
        <xsl:with-param name="count" select="$count + 1"/>
        <xsl:with-param name="colNum" select="$colNum + 1"/>
      </xsl:call-template>
    </xsl:if>

  </xsl:template>

  <xsl:template name="InsertCacheRowField">
    <!-- temporary parameter -->
    <xsl:param name="colNum"/>

    <c>
      <xsl:value-of select="$colNum"/>
    </c>
  </xsl:template>

  <xsl:template name="WritePivotCells">

    <xsl:variable name="sheetName">
      <xsl:value-of select="@table:name"/>
    </xsl:variable>
    <xsl:text>;</xsl:text>

    <!-- do not process twice duplicated pivot source-->
    <xsl:for-each
      select="key('pivot','')[not(preceding-sibling::table:data-pilot-table/table:source-cell-range/@table:cell-range-address = table:source-cell-range/@table:cell-range-address)]">
      <xsl:variable name="apos">'</xsl:variable>
      <xsl:variable name="thisSheetName">
        <xsl:value-of select="translate(substring-before(table:source-cell-range/@table:cell-range-address,'.'),$apos,'')"/>
      </xsl:variable>
      <xsl:variable name="cellAddress">
        <xsl:value-of select="table:source-cell-range/@table:cell-range-address"/>
      </xsl:variable>
		<!--
		Defect: 1894250
		Fixed By: Vijayeta
		Desc : Performance related defect, where in the pivot range of the source table(table:cell-range-address) in input is row num 65536,
			   where as the source table spans only upto 1089 rows. Hence, the code iterates 65536 times, and resulting in long conversion time.
			   Here this part of code takes a count of rows in source table and compares it with the value of attribute table:cell-range-address.
		Date: 10th Sep '08	
		variables: rowCount,columnForRowCount,rowNumCellRange
		-->
		<xsl:variable name ="rowCount">
			<xsl:call-template name ="getTableRowCount">
				<xsl:with-param name="sheetName">
					<xsl:value-of select="$sheetName"/>
				</xsl:with-param>
				<xsl:with-param name="cellAddress">
					<xsl:value-of select="$cellAddress"/>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:variable>
		<xsl:variable name ="columnForRowCount">
			<xsl:if test ="$cellAddress != ''">
			<xsl:call-template name="NumbersToChars">
				<xsl:with-param name="num">
					<xsl:variable name ="number">
						<xsl:call-template name="GetColNum">
							<xsl:with-param name="cell" select="substring-after(substring-after($cellAddress,':'),'.')"/>
						</xsl:call-template>
					</xsl:variable>
					<xsl:value-of select ="$number - 1"/>
				</xsl:with-param>
			</xsl:call-template>
			</xsl:if>
		</xsl:variable>
		<xsl:variable name ="rowNumCellRange">
			<xsl:if test ="$cellAddress != ''">
			<xsl:call-template name="GetRowNum">
				<xsl:with-param name="cell" select="substring-after(substring-after($cellAddress,':'),'.')"/>
			</xsl:call-template>
			</xsl:if>
		</xsl:variable>
      <xsl:variable name="CreatePivotTable">
        <xsl:for-each select="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:table[@table:name=$thisSheetName]">
          <xsl:apply-templates select="table:table-row[1]" mode="checkPivotCells">
            <xsl:with-param name="rowNumber">1</xsl:with-param>
            <xsl:with-param name="cellStart">
              <xsl:value-of select="substring-before(substring-after($cellAddress,'.'),':')"/>
            </xsl:with-param>
            <xsl:with-param name="cellEnd">
				  <xsl:choose>
					  <xsl:when test ="$rowNumCellRange &gt; $rowCount ">
						  <xsl:value-of select ="concat($columnForRowCount,$rowCount)"/>
					  </xsl:when>
					  <xsl:otherwise>
              <xsl:value-of select="substring-after(substring-after($cellAddress,':'),'.')"/>
					  </xsl:otherwise>
				  </xsl:choose>
				  <!--<xsl:value-of select="substring-after(substring-after($cellAddress,':'),'.')"/>-->
            </xsl:with-param>
          </xsl:apply-templates>
        </xsl:for-each>
      </xsl:variable>
      <xsl:if test="$CreatePivotTable != 'false'">
        
        <!-- check only pivot sources on this sheet -->
        <xsl:if
          test="substring-before(translate(table:source-cell-range/@table:cell-range-address,$apos,''), '.') = $sheetName ">
          
          <xsl:variable name="rowStart">
            <xsl:for-each select="table:source-cell-range">
              <xsl:call-template name="GetRowNum">
                <xsl:with-param name="cell">
                  <xsl:value-of
                    select="substring-after(substring-before(@table:cell-range-address,':'),'.')"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:for-each>
          </xsl:variable>
          
          <xsl:variable name="colStart">
            <xsl:for-each select="table:source-cell-range">
              <xsl:call-template name="GetColNum">
                <xsl:with-param name="cell">
                  <xsl:value-of
                    select="substring-after(substring-before(@table:cell-range-address,':'),'.')"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:for-each>
          </xsl:variable>
          
          <xsl:variable name="rowEnd">
            <xsl:for-each select="table:source-cell-range">
				<!--
				Defect: 1894250
				Fixed By: Vijayeta
				Desc : Performance related defect, where in the pivot range of the source table(table:cell-range-address) in input is row num 65536,
					   where as the source table spans only upto 1089 rows. Hence, the code iterates 65536 times, and resulting in long conversion time.
					   Here this part of code takes a count of rows in source table and compares it with the value of attribute table:cell-range-address.
				Date: 10th Sep '08	
				variables: rowCount,columnForRowCount,rowNumCellRange
				-->
				<xsl:call-template name="GetRowNum">
					<xsl:with-param name="cell">
						<xsl:choose>
							<xsl:when test ="$rowNumCellRange &gt; $rowCount">
								<xsl:value-of select ="concat($columnForRowCount,$rowCount)"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="substring-after(substring-after(@table:cell-range-address,':'),'.')"/>
							</xsl:otherwise>
						</xsl:choose>
						<!--<xsl:value-of
                    select="substring-after(substring-after(@table:cell-range-address,':'),'.')"/>-->
					</xsl:with-param>
				</xsl:call-template>
            </xsl:for-each>
          </xsl:variable>
          
          <xsl:variable name="colEnd">
            <xsl:for-each select="table:source-cell-range">
              <xsl:call-template name="GetColNum">
                <xsl:with-param name="cell">
                  <xsl:value-of
                    select="substring-after(substring-after(@table:cell-range-address,':'),'.')"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:for-each>
          </xsl:variable>
          
          <xsl:call-template name="ListOutCellsInRange">
            <xsl:with-param name="rowStart" select="$rowStart"/>
            <xsl:with-param name="rowEnd" select="$rowEnd"/>
            <xsl:with-param name="colStart" select="$colStart"/>
            <xsl:with-param name="colEnd" select="$colEnd"/>
          </xsl:call-template>
        </xsl:if>
      </xsl:if>
    </xsl:for-each>

  </xsl:template>

  <xsl:template name="ListOutCellsInRange">
    <xsl:param name="rowStart"/>
    <xsl:param name="rowEnd"/>
    <xsl:param name="colStart"/>
    <xsl:param name="colEnd"/>
    <xsl:param name="row" select="$rowStart"/>

    <xsl:call-template name="ListOutCellsInRow">
      <xsl:with-param name="row" select="$row"/>
      <xsl:with-param name="colStart" select="$colStart"/>
      <xsl:with-param name="colEnd" select="$colEnd"/>
    </xsl:call-template>

    <xsl:if test="$row != $rowEnd">
      <xsl:call-template name="ListOutCellsInRange">
        <xsl:with-param name="rowStart" select="$rowStart"/>
        <xsl:with-param name="rowEnd" select="$rowEnd"/>
        <xsl:with-param name="colStart" select="$colStart"/>
        <xsl:with-param name="colEnd" select="$colEnd"/>
        <xsl:with-param name="row" select="$row + 1"/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

  <xsl:template name="ListOutCellsInRow">
    <xsl:param name="row"/>
    <xsl:param name="colStart"/>
    <xsl:param name="colEnd"/>
    <xsl:param name="column" select="$colStart"/>

    <xsl:value-of select="$column"/>
    <xsl:text>:</xsl:text>
    <xsl:value-of select="$row"/>
    <xsl:text>;</xsl:text>

    <xsl:if test="$column != $colEnd">
      <xsl:call-template name="ListOutCellsInRow">
        <xsl:with-param name="colStart" select="$colStart"/>
        <xsl:with-param name="colEnd" select="$colEnd"/>
        <xsl:with-param name="row" select="$row"/>
        <xsl:with-param name="column" select="$column + 1"/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

  <xsl:template match="text:p" mode="pivot">
    <xsl:if test="preceding-sibling::text:p[1]">
      <xsl:text> </xsl:text>
    </xsl:if>
    <xsl:apply-templates mode="pivot" xml:space="preserve"/>
  </xsl:template>

  <xsl:template match="text:span" mode="pivot">
    <xsl:apply-templates mode="pivot" xml:space="preserve"/>
  </xsl:template>

  <xsl:template match="text:a" mode="pivot">
    <xsl:apply-templates mode="pivot" xml:space="preserve"/>
  </xsl:template>

  <xsl:template match="text:s" mode="pivot">

    <xsl:call-template name="InsertRepeatSpaces">
      <xsl:with-param name="nr">
        <xsl:text>1</xsl:text>
      </xsl:with-param>
      <xsl:with-param name="repeat">
        <xsl:value-of select="@text:c"/>
      </xsl:with-param>
    </xsl:call-template>

  </xsl:template>

  <xsl:template match="text()" mode="pivot">
    <xsl:value-of select="."/>
  </xsl:template>
  
<!-- do not process theese elements -->
  <xsl:template match="draw:frame|office:annotation" mode="pivot"/>

  <!-- template to process rows in order to find empty pivot cells -->
  
  <xsl:template match="table:table-row" mode="checkPivotCells">
    <xsl:param name="rowNumber"/>
    <xsl:param name="cellStart"/>
    <xsl:param name="cellEnd"/>
    <xsl:variable name="CreatePivotTable">
    <xsl:apply-templates select="table:table-cell[1]" mode="checkPivotCells">
      <xsl:with-param name="rowNumber" select="$rowNumber"/>
      <xsl:with-param name="colNumber">0</xsl:with-param>
      <xsl:with-param name="cellStart" select="$cellStart"/>
      <xsl:with-param name="cellEnd" select="$cellEnd"/>
      <xsl:with-param name="isinPivotTable">false</xsl:with-param>
    </xsl:apply-templates>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="$CreatePivotTable='false'">false</xsl:when>
      <xsl:when test="$CreatePivotTable='true'">true</xsl:when>
      <xsl:otherwise>
        <xsl:apply-templates select="following-sibling::table:table-row[1]" mode="checkPivotCells">
          <xsl:with-param name="rowNumber">
			  <!--Vijayeta,SP2,@table:number-rows-repeated-->
			  <xsl:variable name ="tableNumberRowsRepeated">
            <xsl:choose>
					  <xsl:when test ="@table:number-rows-repeated &gt; 65536">
						  <xsl:value-of select ="65536 - (1048576 - @table:number-rows-repeated)"/>
					  </xsl:when>
					  <xsl:when test ="@table:number-rows-repeated &lt;= 65536">
						  <xsl:value-of select="@table:number-rows-repeated"/>
					  </xsl:when>
				  </xsl:choose>
			  </xsl:variable>
            <xsl:choose>
              <!--<xsl:when test="@table:number-rows-repeated">
                <xsl:value-of select="$rowNumber+@table:number-rows-repeated+1"/>
              </xsl:when>-->
              <xsl:when test="@table:number-rows-repeated">
					<xsl:value-of select="$rowNumber+$tableNumberRowsRepeated+1"/>
              </xsl:when>
				<!--Vijayeta,SP2,@table:number-rows-repeated,End-->
              <xsl:otherwise>
                <xsl:value-of select="$rowNumber+1"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>
          <xsl:with-param name="cellStart" select="$cellStart"/>
          <xsl:with-param name="cellEnd" select="$cellEnd"/>
        </xsl:apply-templates>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  
  <!-- template to process cells in order to find empty pivot cells -->
  
  <xsl:template match="table:table-cell" mode="checkPivotCells">
    <xsl:param name="rowNumber"/>
    <xsl:param name="colNumber"/>
    <xsl:param name="cellStart"/>
    <xsl:param name="cellEnd"/>
    <xsl:param name="isinPivotTable"/>
    <xsl:variable name="colChar">
      <xsl:call-template name="NumbersToChars">
        <xsl:with-param name="num" select="$colNumber"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="cellNum">
      <xsl:value-of select="concat($colChar, $rowNumber)"/>
    </xsl:variable>
    <xsl:variable name="numberOfEndCell">
      <xsl:call-template name="GetNumberOfEndCell">
        <xsl:with-param name="endCell">
          <xsl:value-of select="$cellEnd"/>
        </xsl:with-param>
        <xsl:with-param name="number">2</xsl:with-param>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="lastCellInRow">
      <xsl:value-of select="concat($numberOfEndCell,$rowNumber)"/>
    </xsl:variable>
     <xsl:choose>
      
      <!-- check if cells processed are in a pivot table -->
      <xsl:when test="$isinPivotTable='true'">
        <xsl:choose>
          
          <!-- if there is an attribute @table:number-columns-repeated, then  the next cell must be empty -->
          <xsl:when test="@table:number-columns-repeated">false</xsl:when>
          
          <!-- if the last column of pivot table is reached, then there are no empty cells in first row -->
          <xsl:when test="$cellNum = $lastCellInRow">true</xsl:when>
          
          <!-- otherwise process following cells--> 
          <xsl:otherwise>
            <xsl:apply-templates select="following-sibling::table:table-cell[1]" mode="checkPivotCells">
              <xsl:with-param name="rowNumber" select="$rowNumber"/>
				<!--Vijayeta,SP2,@table:number-columns-repeated-->
              <!--<xsl:with-param name="colNumber">
                <xsl:choose>
                  <xsl:when test="@table:number-columns-repeated">
                    <xsl:value-of select="$colNumber+@table:number-columns-repeated+1"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="$colNumber+1"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>-->
              <xsl:with-param name="colNumber">
                <!--<xsl:choose>
                  <xsl:when test="@table:number-columns-repeated">
							<xsl:choose >
								<xsl:when test ="@table:number-columns-repeated &gt; 256">
									<xsl:value-of select ="$colNumber +1 + (256 - (16384 - @table:number-columns-repeated))"/>
								</xsl:when>
								<xsl:when test ="@table:number-columns-repeated &lt;= 256">
                    <xsl:value-of select="$colNumber+@table:number-columns-repeated+1"/>
                  </xsl:when>
							</xsl:choose>
						</xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="$colNumber+1"/>
                  </xsl:otherwise>
                </xsl:choose>-->
				  <xsl:choose>
					  <xsl:when test="@table:number-columns-repeated">
						  <xsl:variable name ="tempOfficeVersion">
							  <xsl:value-of select ="document('meta.xml')/office:document-meta/office:meta/meta:generator"/>
						  </xsl:variable>
						  <xsl:choose>
							  <xsl:when test ="starts-with($tempOfficeVersion,'OpenOffice.org')">
								  <xsl:value-of select="$colNumber+@table:number-columns-repeated+1"/>
							  </xsl:when>
							  <xsl:when test ="starts-with($tempOfficeVersion,'MicrosoftOffice')">
								  <xsl:choose>
									  <xsl:when test ="@table:number-columns-repeated &gt; 256">
										  <xsl:value-of select ="$colNumber +1 + (256 - (16384 - @table:number-columns-repeated))"/>
									  </xsl:when>
									  <xsl:otherwise>
										  <xsl:value-of select="$colNumber+@table:number-columns-repeated+1"/>
									  </xsl:otherwise>
								  </xsl:choose>								 
							  </xsl:when>
							  <xsl:otherwise>
								  <xsl:value-of select="$colNumber+@table:number-columns-repeated+1"/>
							  </xsl:otherwise>
						  </xsl:choose>
					  </xsl:when>
					  <xsl:otherwise>
						  <xsl:value-of select="$colNumber+1"/>
					  </xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>
				<!--Vijayeta,SP2,@table:number-columns-repeated,End-->
              <xsl:with-param name="isinPivotTable">
                <xsl:choose>
                  <xsl:when test="$isinPivotTable='true'">true</xsl:when>
                  <xsl:otherwise>
                    <xsl:choose>
                      <xsl:when test="$cellNum = $cellStart">true</xsl:when>
                      <xsl:otherwise>false</xsl:otherwise>
                    </xsl:choose>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>
              <xsl:with-param name="cellStart" select="$cellStart"/>
              <xsl:with-param name="cellEnd" select="$cellEnd"/>
            </xsl:apply-templates>
          </xsl:otherwise>
          
        </xsl:choose>
      </xsl:when>
      
      <!-- otherwise process following cells -->
      <xsl:otherwise>
        <xsl:apply-templates select="following-sibling::table:table-cell[1]" mode="checkPivotCells">
          <xsl:with-param name="rowNumber" select="$rowNumber"/>
			<!--Vijayeta,SP2,@table:number-columns-repeated-->
          <!--<xsl:with-param name="colNumber">
            <xsl:choose>
              <xsl:when test="@table:number-columns-repeated">
                <xsl:value-of select="$colNumber+@table:number-columns-repeated+1"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$colNumber+1"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>-->
          <xsl:with-param name="colNumber">
            <!--<xsl:choose>
              <xsl:when test="@table:number-columns-repeated">
						<xsl:choose >
							<xsl:when test ="@table:number-columns-repeated &gt; 256">
								<xsl:value-of select ="$colNumber +1 + (256 - (16384 - @table:number-columns-repeated))"/>
							</xsl:when>
							<xsl:when test ="@table:number-columns-repeated &lt;= 256">
                <xsl:value-of select="$colNumber+@table:number-columns-repeated+1"/>
              </xsl:when>
						</xsl:choose>
					</xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$colNumber+1"/>
              </xsl:otherwise>
            </xsl:choose>-->
			  <xsl:variable name ="tempOfficeVersion">
				  <xsl:value-of select ="document('meta.xml')/office:document-meta/office:meta/meta:generator"/>
			  </xsl:variable>
			  <xsl:choose>
				  <xsl:when test="@table:number-columns-repeated">
					  <xsl:choose>
						  <xsl:when test ="starts-with($tempOfficeVersion,'OpenOffice.org')">
							  <xsl:value-of select="$colNumber+@table:number-columns-repeated+1"/>
						  </xsl:when>
						  <xsl:when test ="starts-with($tempOfficeVersion,'MicrosoftOffice')">
							  <xsl:choose>
								  <xsl:when test ="@table:number-columns-repeated &gt; 256">
									  <xsl:value-of select ="$colNumber +1 + (256 - (16384 - @table:number-columns-repeated))"/>
								  </xsl:when>
								  <xsl:otherwise>
									  <xsl:value-of select="$colNumber+@table:number-columns-repeated+1"/>
								  </xsl:otherwise>
							  </xsl:choose>
						  </xsl:when>
						  <xsl:otherwise>
							  <xsl:value-of select="$colNumber+@table:number-columns-repeated+1"/>
						  </xsl:otherwise>
					  </xsl:choose>
				  </xsl:when>
				  <xsl:otherwise>
					  <xsl:value-of select="$colNumber+1"/>
				  </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>
			<!--Vijayeta,SP2,@table:number-columns-repeated,End-->
          <xsl:with-param name="isinPivotTable">
            <xsl:choose>
              <xsl:when test="$isinPivotTable='true'">true</xsl:when>
              <xsl:otherwise>
                <xsl:choose>
                  <xsl:when test="$cellNum = $cellStart">true</xsl:when>
                  <xsl:otherwise>false</xsl:otherwise>
                </xsl:choose>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>
          <xsl:with-param name="cellStart" select="$cellStart"/>
          <xsl:with-param name="cellEnd" select="$cellEnd"/>
        </xsl:apply-templates>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  
  <xsl:template match="text()" mode="checkPivotCells"/>
  
  <!-- template to get column number from cell with given cell address -->
  
  <xsl:template name="GetNumberOfEndCell">
    <xsl:param name="endCell"/>
    <xsl:param name="number"/>
    <xsl:choose>
      <xsl:when test="starts-with(substring($endCell,$number),'0') or starts-with(substring($endCell,$number),'1') or starts-with(substring($endCell,$number),'2') or starts-with(substring($endCell,$number),'3') or starts-with(substring($endCell,$number),'4') or starts-with(substring($endCell,$number),'5') or starts-with(substring($endCell,$number),'6') or starts-with(substring($endCell,$number),'7') or starts-with(substring($endCell,$number),'8') or starts-with(substring($endCell,$number),'9')">
        <xsl:value-of select="substring($endCell,0,$number)"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="GetNumberOfEndCell">
          <xsl:with-param name="endCell" select="$endCell"/>
          <xsl:with-param name="number">
            <xsl:value-of select="$number+1"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  
</xsl:stylesheet>
