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
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:odf="urn:odf"
  xmlns:pzip="urn:cleverage:xmlns:post-processings:zip"
  xmlns:math="http://www.w3.org/1998/Math/MathML"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:xdr="http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:number="urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0"
  xmlns:meta="urn:oasis:names:tc:opendocument:xmlns:meta:1.0"
  xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main"
  xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
  xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
  xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:v="urn:schemas-microsoft-com:vml"
  xmlns:o="urn:schemas-microsoft-com:office:office"
  xmlns:pxsi="urn:cleverage:xmlns:post-processings:pivotTable"
  xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
  xmlns:x="urn:schemas-microsoft-com:office:excel" exclude-result-prefixes="odf style text number">

  <xsl:import href="workbook.xsl"/>
  <xsl:import href="sharedStrings.xsl"/>
  <xsl:import href="odf2oox-compute-size.xsl"/>
  <xsl:import href="contentTypes.xsl"/>
  <xsl:import href="package_relationships.xsl"/>
  <xsl:import href="common-meta.xsl"/>
  <xsl:import href="part_relationships.xsl"/>
  <xsl:import href="pivotTable.xsl"/>
  <xsl:import href="common.xsl"/>
  <xsl:import href="merge_cell.xsl"/>
  <xsl:import href="styles.xsl"/>
  <xsl:import href="number.xsl"/>
  <xsl:import href="comments.xsl"/>
  <xsl:import href="date.xsl"/>
  <xsl:import href="chart.xsl"/>
  <xsl:import href="drawing.xsl"/>
  <xsl:import href="connections.xsl"/>
  <xsl:import href="measures.xsl"/>
  <xsl:import href="gradient.xsl"/>
  <xsl:import href="cell.xsl"/>
	<xsl:import href="conditional.xsl"/>
	<xsl:import href="worksheets.xsl"/>	
	<xsl:import href="ole_objects.xsl"/>
	<xsl:import href="change_tracking.xsl"/>
	<xsl:import href="pixel-measure.xsl"/>
	<xsl:import href="page.xsl"/>
	<xsl:import href="border.xsl"/>	
	<xsl:import href="sortFilter.xsl"/>
	<xsl:import href="validation.xsl"/>
	<xsl:import href="data_consolidation.xsl"/>
	<xsl:import href="scenario.xsl"/>

  <xsl:strip-space elements="*"/>
  <xsl:preserve-space elements="text:p text:span number:text"/>

  <xsl:param name="outputFile"/>
  
  <!-- a string containing detailed information on environment and
       converter version to be added to the document's meta data -->
  <xsl:param name="generator"/>
  
  <xsl:output method="xml" encoding="UTF-8"/>

  <!-- App version number -->
  <!-- WARNING: it has to be of type xx.yy -->
  <!-- (otherwise Word cannot open the doc) -->
  <xsl:variable name="app-version">1.00</xsl:variable>

  <!-- existence of docProps/custom.xml file -->
  <xsl:variable name="docprops-custom-file">
    <xsl:choose>
      <xsl:when test="document('meta.xml')/office:document-meta/office:meta/meta:user-defined != ''">
        <xsl:text>1</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:text>0</xsl:text>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:variable>


  <xsl:key name="chart" match="office:chart" use="''"/>
  <xsl:key name="pivot" match="table:data-pilot-table" use="''"/>
	<xsl:key name="table" match="table:table" use="''"/>
  <xsl:template match="/odf:source">
    <xsl:processing-instruction name="mso-application">progid="Word.Document"</xsl:processing-instruction>


    <pzip:archive pzip:target="{$outputFile}">

      <pzip:entry pzip:target="[Content_Types].xml">
        <xsl:call-template name="ContentTypes"/>
      </pzip:entry>
      <!-- CHANGE -->
      <!-- styles -->
      <pzip:entry pzip:target="xl/styles.xml">
        <xsl:call-template name="styles"/>
      </pzip:entry>
      <!-- input: styles.xml -->

      <!-- main content -->
      <pzip:entry pzip:target="xl/workbook.xml">
        <xsl:call-template name="InsertWorkbook"/>
      </pzip:entry>

      <!-- main content -->
      <xsl:if
        test="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:table/table:table-row/table:table-cell/table:cell-range-source">
        <pzip:entry pzip:target="xl/connections.xml">
          <xsl:call-template name="InsertConnections"/>
        </pzip:entry>
      </xsl:if>

      <!-- shared strings -->
      <pzip:entry pzip:target="xl/sharedStrings.xml">
        <xsl:call-template name="InsertSharedStrings"/>
      </pzip:entry>
      <!-- input: content.xml -->
      <!-- output: xl/sharedStrings.xml -->


      <!-- insert sheets -->
      <xsl:call-template name="InsertSheets"/>
      <!-- input: content.xml -->
      <!-- output:  xl/worksheets/sheet_N_.xml -->
      <!--xsl:if
        test="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:table/draw:frame/draw:object">
        <pzip:entry pzip:target="xl/media/image1.emf">
          <empty/>
        </pzip:entry>
      </xsl:if-->

      <xsl:for-each
        select="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:table">

        <!-- if scenario contains more than 32 changing cells, it cannot be converted to Excel -->
        <xsl:for-each select="table:scenario">
          <!-- the first cell in the scenario -->
          <xsl:variable name="firstCell">
            <xsl:value-of
              select="substring-after(substring-before(@table:scenario-ranges, ':'),'.')"/>
          </xsl:variable>
          <!-- the last cell in the scenario -->
          <xsl:variable name="lastCell">
            <xsl:value-of
              select="substring-after(substring-after(@table:scenario-ranges, '.'), '.')"/>
          </xsl:variable>

          <xsl:variable name="absFirstCell">
            <xsl:call-template name="rel2Abs">
              <xsl:with-param name="relatCellAddress">
                <xsl:value-of select="$firstCell"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:variable>

          <xsl:variable name="absLastCell">
            <xsl:call-template name="rel2Abs">
              <xsl:with-param name="relatCellAddress">
                <xsl:value-of select="$lastCell"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:variable>

          <!-- one-based index number of the column, e.g. 'A' becomes 1 -->
          <xsl:variable name="firstColumnNumber">
            <!-- for columns from 'A' to 'Z'(one-letter column name) -->
            <xsl:choose>
              <xsl:when
                test="string-length(substring-before(substring-after($absFirstCell, '$'), '$')) = 1">
                <xsl:call-template name="CharacterToPosition">
                  <xsl:with-param name="character">
                    <xsl:value-of
                      select="substring-before(substring-after($absFirstCell, '$'), '$')"/>
                  </xsl:with-param>
                </xsl:call-template>
              </xsl:when>
              <!-- for columns which names contain more than one letter -->
              <xsl:otherwise>
                <xsl:call-template name="returnColumnNumber">
                  <xsl:with-param name="columnName">
                    <xsl:value-of select="substring-before(substring-after($absFirstCell, '$'), '$')"/>
                  </xsl:with-param>
                </xsl:call-template>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:variable>

          <xsl:variable name="lastColumnNumber">
            <!-- for columns from 'A' to 'Z'(one-letter column name) -->
            <xsl:choose>
              <xsl:when
                test="string-length(substring-before(substring-after($absLastCell, '$'), '$')) = 1">
                <xsl:call-template name="CharacterToPosition">
                  <xsl:with-param name="character">
                    <xsl:value-of
                      select="substring-before(substring-after($absLastCell, '$'), '$')"/>
                  </xsl:with-param>
                </xsl:call-template>
              </xsl:when>
              <!-- for columns which names contain more than one letter -->
              <xsl:otherwise>
                <xsl:call-template name="returnColumnNumber">
                  <xsl:with-param name="columnName">
                    <xsl:value-of select="substring-before(substring-after($absLastCell, '$'), '$')"/>
                  </xsl:with-param>
                </xsl:call-template>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:variable>

          <!-- scenario width measured in cells -->
          <xsl:variable name="scenarioWidth">
            <xsl:value-of select="number($lastColumnNumber) - number($firstColumnNumber) + 1"/>
          </xsl:variable>

          <xsl:variable name="firstRowNumber">
            <xsl:value-of select="substring-after(substring-after($absFirstCell, '$'), '$')"/>
          </xsl:variable> 
          
          <xsl:variable name="lastRowNumber">
            <xsl:value-of select="substring-after(substring-after($absLastCell, '$'), '$')"/>
          </xsl:variable>

          <!-- scenario height measured in cells -->
          <xsl:variable name="scenarioHeight">
            <xsl:value-of select="number($lastRowNumber) - number($firstRowNumber)"/>
          </xsl:variable> 
          
          <!-- when scenario bigger than supported -->
          <xsl:if test="number($scenarioHeight) * number($scenarioWidth) &gt; 32">
            <xsl:message terminate="no">translation.odf2oox.scenarioTooBig</xsl:message>
          </xsl:if>
          
        </xsl:for-each>
        
        <!-- added message, see bug: https://sourceforge.net/tracker/index.php?func=detail&aid=1760182&group_id=169337&atid=929855 -->
        <xsl:for-each select="draw:text-box/text:p/text:a">
          <xsl:message terminate="no">translation.odf2oox.hyperlinkInTextbox</xsl:message>
        </xsl:for-each>

        <xsl:variable name="comment">
          <xsl:choose>
            <xsl:when test="not(table:scenario) and descendant::office:annotation">
              <xsl:text>true</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>false</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>

        <xsl:variable name="picture">
          <xsl:choose>
            <xsl:when
              test="descendant::draw:frame/draw:image[not(starts-with(@xlink:href,'./ObjectReplacements')) and not(name(parent::node()/parent::node()) = 'draw:g' )]">
              <xsl:text>true</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>false</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>

        <xsl:variable name="hyperlink">
          <xsl:choose>
            <xsl:when
              test="descendant::text:a[not(ancestor::draw:custom-shape) and not(ancestor::office:annotation)] or (ancestor::draw:text-box)">
              <xsl:text>true</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>false</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>

        <xsl:variable name="textBox">
          <xsl:choose>
            <xsl:when
              test="descendant::draw:frame/draw:text-box[not(name(parent::node()/parent::node()) = 'draw:g' )]">
              <xsl:text>true</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>false</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>

        <xsl:variable name="chart">
          <xsl:for-each select="descendant::draw:frame/draw:object">
            <xsl:choose>
              <xsl:when test="not(document(concat(translate(@xlink:href,'./',''),'/settings.xml')))">
                <xsl:for-each
                  select="document(concat(translate(@xlink:href,'./',''),'/content.xml'))">
                  <xsl:choose>
                    <xsl:when test="office:document-content/office:body/office:chart">
                      <xsl:text>true</xsl:text>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:text>false</xsl:text>
                    </xsl:otherwise>
                  </xsl:choose>
                </xsl:for-each>
              </xsl:when>
              <xsl:otherwise>
                <xsl:text>false</xsl:text>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:for-each>
        </xsl:variable>

		  <!--Defect Id       :1784784
			* Code Changed by :Vijayeta Tilak
			* Date            :26th Dec '07
			* Description     :This part of code( an OR condition to 'when' loop) was added because when a file contains OLE object directly in a drive
			*                  then the value of attibute 'xlink:href' begins from a '/' and not '../'(which offcourse means within the folder.	
			-->
        <xsl:variable name="sheetOleObject">
          <xsl:choose>
            <xsl:when
              test="descendant::draw:frame/draw:object[starts-with(@xlink:href,'../') or starts-with(@xlink:href,'/') and not(name(parent::node()/parent::node()) = 'draw:g' )]">
              <xsl:text>true</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>false</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>

        <xsl:variable name="sheetNum">
          <xsl:value-of select="position()"/>
        </xsl:variable>

        <xsl:variable name="tableName">
          <xsl:value-of select="@table:name"/>
        </xsl:variable>

        <xsl:variable name="apos">
          <xsl:text>&apos;</xsl:text>
        </xsl:variable>
        
        <xsl:variable name="startRangeAddressRow">
          <xsl:choose>
            <xsl:when test="substring-before(substring-after(key('pivot','')/@table:target-range-address, '.'), ':') != ''">
              <xsl:call-template name="GetRowNum">            
                <xsl:with-param name="cell">              
                  <xsl:value-of select="substring-before(substring-after(key('pivot','')/@table:target-range-address, '.'), ':')"/>
                </xsl:with-param>
              </xsl:call-template>            
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>0</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        
        <xsl:variable name="endRangeAddressRow">
          <xsl:choose>
            <xsl:when test="substring-after(substring-after(key('pivot','')/@table:target-range-address, '.'), '.') != ''">
              <xsl:call-template name="GetRowNum">
                <xsl:with-param name="cell">
                  <xsl:value-of select="substring-after(substring-after(key('pivot','')/@table:target-range-address, '.'), '.')"/>
                </xsl:with-param>
              </xsl:call-template>     
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>0</xsl:text>
            </xsl:otherwise>
          </xsl:choose>          
        </xsl:variable>
        
        <xsl:variable name="startRangeAddressCol">          
          <xsl:choose>
            <xsl:when test="substring-before(substring-after(key('pivot','')/@table:target-range-address, '.'), ':') != ''">
              <xsl:call-template name="GetColNum">
                <xsl:with-param name="cell">
                  <xsl:value-of select="substring-before(substring-after(key('pivot','')/@table:target-range-address, '.'), ':')"/>
                </xsl:with-param>
              </xsl:call-template>      
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>0</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        
        <xsl:variable name="endRangeAddressCol">
          <xsl:choose>
            <xsl:when test="substring-after(substring-after(key('pivot','')/@table:target-range-address, '.'), '.') != ''">
              <xsl:call-template name="GetColNum">
                <xsl:with-param name="cell">
                  <xsl:value-of select="substring-after(substring-after(key('pivot','')/@table:target-range-address, '.'), '.')"/>
                </xsl:with-param>
              </xsl:call-template>       
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>0</xsl:text>
            </xsl:otherwise>
          </xsl:choose>          
        </xsl:variable>
        
	     <!--
	      Defect Id :1957957(roundtrip), Internal defect-5843
		  Date      :5th June '08
		  Changes by:Vijayeta		  
	      Desc      :Error occurs when the source table does not exist, but the name is mentioned in the attribute 'table:cell-range-address'(ods)
					 Hence, handled here by an additional condition to check if the attribute value is a table in the Worksheet
		  -->
        <xsl:variable name="pivot">
          <xsl:choose>
            <xsl:when
              test="key('pivot','')[translate(substring-before(@table:target-range-address,'.'),$apos,'') = $tableName and table:source-cell-range/@table:cell-range-address] and $startRangeAddressRow != $endRangeAddressRow and $startRangeAddressCol != $endRangeAddressCol">
				<xsl:variable name="sourceSheetName1">
					<xsl:value-of select="key('pivot','')/table:source-cell-range/@table:cell-range-address"/>
				</xsl:variable>
				<xsl:variable name ="sourceSheetName">
					<xsl:value-of select ="translate(substring-before($sourceSheetName1,'.'),$apos,'')"/>
				</xsl:variable>
				<xsl:choose >
					<xsl:when test ="key('table','')[@table:name=$sourceSheetName]">
              <xsl:text>true</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>false</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
              <!--<xsl:text>true</xsl:text>-->
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>false</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
				
        </xsl:variable>

        <!-- insert comments -->
        <xsl:if test="$comment = 'true' ">
          <xsl:call-template name="InsertComments">
            <xsl:with-param name="sheetId" select="position()"/>
          </xsl:call-template>
        </xsl:if>

        <!-- create VmlDrawing.xml file for comment`s or OLE object -->
        <xsl:if test="$comment = 'true' or $sheetOleObject = 'true' ">

          <xsl:call-template name="CreateVmlDrawing"/>

          <xsl:if test="$sheetOleObject = 'true'">

            <xsl:call-template name="CreateVmlDrawingRelationships">
              <xsl:with-param name="sheetNum" select="$sheetNum"/>
            </xsl:call-template>

            <!--  create emf dummy images -->
			  <!--Defect Id   :1784784
			* Code Changed by :Vijayeta Tilak
			* Date            :26th Dec '07
			* Description     :This part of code( an OR condition to 'for-each' loop) was added because when a file contains OLE object directly in a drive
			*                  then the value of attibute 'xlink:href' begins from a '/' and not '../'(which offcourse means within the folder.	
			-->
            <xsl:for-each
              select="descendant::draw:frame/draw:object[starts-with(@xlink:href,'../') or starts-with(@xlink:href,'/')  and not(name(parent::node()/parent::node()) = 'draw:g' )]">
              <pzip:entry pzip:target="{concat('xl/media/image',$sheetNum,'_',position(),'.emf')}">
                <empty/>
              </pzip:entry>
            </xsl:for-each>

          </xsl:if>
        </xsl:if>

        <xsl:if test="contains($chart,'true') or $picture='true' or $textBox = 'true' ">
          <xsl:call-template name="CreateDrawing"/>

          <xsl:if test="contains($chart,'true') or $picture='true' or $hyperlink = 'true'">
            <xsl:call-template name="CreateDrawingRelationships"/>
          </xsl:if>

          <xsl:if test="contains($chart,'true')">
            <xsl:call-template name="CreateChartFile">
              <xsl:with-param name="sheetNum" select="position()"/>
            </xsl:call-template>
          </xsl:if>
        </xsl:if>

        <xsl:if test="$pivot = 'true'">

          <xsl:for-each
            select="key('pivot','')[translate(substring-before(@table:target-range-address,'.'),$apos,'') = $tableName]">
            
            <!-- don't convert pivot tables with empty cells in first row since Excel doesn't support them, and don't convert hidden, one cell or one row pivot tables (Excel doesn't support it too)-->
            <xsl:variable name="sheetName">
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
              <xsl:for-each select="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:table[@table:name=$sheetName]">
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
              <!-- create pivot table array in postprocessor -->
              <pxsi:pivotTable>
                
                <xsl:for-each select="table:source-cell-range">
                  
                  <xsl:variable name="rowStart">
                    <xsl:call-template name="GetRowNum">
                      <xsl:with-param name="cell">
                        <xsl:value-of
                          select="substring-after(substring-before(@table:cell-range-address,':'),'.')"
                        />
                      </xsl:with-param>
                    </xsl:call-template>
                  </xsl:variable>
                  
                  <xsl:variable name="colStart">
                    <xsl:call-template name="GetColNum">
                      <xsl:with-param name="cell">
                        <xsl:value-of
                          select="substring-after(substring-before(@table:cell-range-address,':'),'.')"
                        />
                      </xsl:with-param>
                    </xsl:call-template>
                  </xsl:variable>
                  
                  <xsl:variable name="rowEnd">
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
								<!--<xsl:value-of select="substring-after(substring-after(@table:cell-range-address,':'),'.')"/>-->
							</xsl:with-param>
						</xsl:call-template>
					</xsl:variable>
                  
                  <xsl:variable name="colEnd">
                    <xsl:call-template name="GetColNum">
                      <xsl:with-param name="cell">
                        <xsl:value-of
                          select="substring-after(substring-after(@table:cell-range-address,':'),'.')"
                        />
                      </xsl:with-param>
                    </xsl:call-template>
                  </xsl:variable>
                  
                  <xsl:attribute name="pxsi:sheetNum">
                    <xsl:variable name="sourceSheet">
                      <xsl:value-of select="translate(substring-before(@table:cell-range-address,'.'),$apos,'')"/>
                    </xsl:variable>
                    
                    <xsl:for-each select="parent::node()/parent::node()/parent::node()/table:table[@table:name = $sourceSheet]">
                      <xsl:value-of select="count(preceding-sibling::table:table) + 1"/>
                    </xsl:for-each>
                  </xsl:attribute>
                  
                  <xsl:attribute name="pxsi:colStart">
                    <xsl:value-of select="$colStart"/>
                  </xsl:attribute>
                  
                  <xsl:attribute name="pxsi:colEnd">
                    <xsl:value-of select="$colEnd"/>
                  </xsl:attribute>
                  
                  <xsl:attribute name="pxsi:rowStart">
                    <!-- first row is always a header row -->
                    <xsl:value-of select="$rowStart + 1"/>
                  </xsl:attribute>
                  
                  <xsl:attribute name="pxsi:rowEnd">
                    <xsl:value-of select="$rowEnd"/>
                  </xsl:attribute>
                  
                </xsl:for-each>
              </pxsi:pivotTable>
              
              
              <xsl:call-template name="CreatePivotTable">
                <xsl:with-param name="sheetNum" select="$sheetNum"/>
              </xsl:call-template>
              
              <xsl:call-template name="CreatePivotTableRels">
                <xsl:with-param name="sheetNum" select="$sheetNum"/>
              </xsl:call-template>
              
              <xsl:variable name="pivotSource">
                <xsl:value-of select="table:source-cell-range/@table:cell-range-address"/>
              </xsl:variable>
              
              <!-- do not duplicate the same source range cache -->
              <!-- due to postprocessor requirements cache definition must be created after pivot table -->
              <xsl:if
                test="not(preceding-sibling::table:data-pilot-table[table:source-cell-range/@table:cell-range-address = $pivotSource])">
                <xsl:call-template name="CreateCacheDefinition"/>
                <xsl:call-template name="CreateCacheDefinitionRels"/>
                <xsl:call-template name="CreateCacheRecords"/>
              </xsl:if>
            </xsl:if>
          </xsl:for-each>
        </xsl:if>

        <!-- insert relationships -->
        <xsl:call-template name="CreateSheetRelationships">
          <xsl:with-param name="sheetNum" select="position()"/>
          <xsl:with-param name="comment" select="$comment"/>
          <xsl:with-param name="chart" select="$chart"/>
          <xsl:with-param name="picture" select="$picture"/>
          <xsl:with-param name="hyperlink" select="$hyperlink"/>
          <xsl:with-param name="textBox" select="$textBox"/>
          <xsl:with-param name="pivot" select="$pivot"/>
        </xsl:call-template>
      </xsl:for-each>

      <!-- insert pivotTables -->
      <!-- <xsl:call-template name="InsertPivotTables"/> -->
      <!-- input: content.xml -->
      <!-- output:  xl/pivotTable/pivotTable_N_.xml 
                                 xl/pivotTable/_rels/pivotTable_N_.xml.rels 
                                 xl/pivotCacheDefinition/pivotCacheDefinition_N_.xml
                                 xl/pivotCacheDefinition/pivotCacheRecords_N_.xml
                                 xl/pivotCacheDefinition/_rels/pivotCacheDefinition_N_.xml.rels                
            -->

      <!-- insert revisions -->
      <!-- <xsl:call-template name="InsertRevisions"/> -->
      <!-- input: content.xml -->
      <!-- output: xl/revisions/revisionHeaders.xml
                                xl/revisions/revisionLog_N_.xml
                                xl/revisions/userNames.xml
                                xl/revisions/_rels/revisionHeaders.xml.rels
            -->

      <!-- connections -->
      <!--<pzip:entry pzip:target="xl/connections.xml">
                <xsl:call-template name="InsertConnections"/>
            </pzip:entry>-->
      <!-- input: content.xml -->

      <!-- queryTables -->
      <!--<xsl:call-template name="InsertQueryTables"/>-->
      <!-- input: content.xml -->
      <!-- output:  xl/queryTables/queryTableN.xml -->

      <!-- settings  -->
      <!-- <pzip:entry pzip:target="xl/settings.xml">
                <xsl:call-template name="InsertSettings"/>
            </pzip:entry>-->
      <!-- input: content.xml -->

      <!-- insert drawings -->
      <!-- input: content.xml 
        Object_N_/content.xml
        Pictures/_imageFile_
      -->
      <!-- output:  xl/drawings/drawing_N_.xml
        xl/drawings/_rels/drawing_N_.xml.rels
        xl/charts/chart_N_.xml 
        xl/media/_imageFile_ -->

      <!-- /CHANGE -->

      <!-- part relationship item -->
      <pzip:entry pzip:target="xl/_rels/workbook.xml.rels">
        <xsl:call-template name="InsertPartRelationships"/>
      </pzip:entry>

      <!-- Document core properties -->
      <pzip:entry pzip:target="docProps/core.xml">
        <xsl:call-template name="docprops-core"/>
      </pzip:entry>

      <!-- Document app properties -->
      <pzip:entry pzip:target="docProps/app.xml">
        <xsl:call-template name="docprops-app"/>
      </pzip:entry>

      <!-- Document custom properties -->
      <xsl:if test="$docprops-custom-file > 0">
        <pzip:entry pzip:target="docProps/custom.xml">
          <xsl:call-template name="docprops-custom"/>
        </pzip:entry>
      </xsl:if>

      <!-- package relationship item -->
      <pzip:entry pzip:target="_rels/.rels">
        <xsl:call-template name="package-relationships"/>
      </pzip:entry>

      <xsl:variable name="oleObject">
        <xsl:choose>
			<!--Defect Id     :1784784
			* Code Changed by :Vijayeta Tilak
			* Date            :26th Dec '07
			* Description     :This part of code( an OR condition to 'when' loop) was added because when a file contains OLE object directly in a drive
			*                  then the value of attibute 'xlink:href' begins from a '/' and not '../'(which offcourse means within the folder.	
			-->
          <xsl:when
            test="document('content.xml')/descendant::draw:frame/draw:object[starts-with(@xlink:href,'../') or starts-with(@xlink:href,'/') and not(name(parent::node()/parent::node()) = 'draw:g' )]">
            <xsl:text>true</xsl:text>
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>false</xsl:text>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>

      <xsl:if test="$oleObject = 'true'">
        <xsl:call-template name="InsertOLEexternalLinks"/>
        <xsl:call-template name="OLEexternalLinks_rels"/>
      </xsl:if>

      <xsl:call-template name="InsertLinkExternalRels"/>

      <!-- Insert Change Tracking -->
      <xsl:if
        test="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:tracked-changes">
        <xsl:call-template name="CreateRevisionHeadersRels"/>
        <xsl:call-template name="CreateRevisionFiles"/>
        <xsl:call-template name="revisionHeaders"/>
        <xsl:call-template name="userName"/>
      </xsl:if>

    </pzip:archive>
  </xsl:template>

  <xsl:template name="docprops-app">
    <Properties xmlns="http://schemas.openxmlformats.org/officeDocument/2006/extended-properties"
      xmlns:vt="http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes"
      xmlns:dc="http://purl.org/dc/elements/1.1/">
      <xsl:call-template name="GetDocSecurityExtendedProperty"/>
      <xsl:call-template name="GetApplicationExtendedProperty">
        <xsl:with-param name="generator" select="$generator"/>
      </xsl:call-template>
      <xsl:for-each select="document('meta.xml')/office:document-meta/office:meta">
        <xsl:apply-templates select="meta:editing-duration"/>
      </xsl:for-each>
    </Properties>
  </xsl:template>

  <xsl:template name="InsertComments">
    <xsl:param name="sheetId"/>
    <pzip:entry
      pzip:target="{concat(&quot;xl/comments&quot;,$sheetId,&quot;.xml&quot;)}">
      <xsl:call-template name="comments">
        <xsl:with-param name="sheetNum" select="$sheetId"/>
      </xsl:call-template>
    </pzip:entry>
  </xsl:template>

  <xsl:template name="CreateSheetRelationships">
    <!-- @Context: table:table -->
    <xsl:param name="comment"/>
    <xsl:param name="picture"/>
    <xsl:param name="hyperlink"/>
    <xsl:param name="chart"/>
    <xsl:param name="textBox"/>
    <xsl:param name="pivot"/>

    <!--      <xsl:if
      test="$comment = 'true' or $picture != 'true' or $hyperlink = 'true' or contains($chart,'true')">-->

    <xsl:variable name="OLEObject">
      <xsl:choose>
		  <!--Defect Id       :1784784
			* Code Changed by :Vijayeta Tilak
			* Date            :26th Dec '07
			* Description     :This part of code( an OR condition to 'when') was added because when a file contains OLE object directly in a drive
			*                  then the value of attibute 'xlink:href' begins from a '/' and not '../'(which offcourse means within the folder.	
			-->
        <xsl:when
          test="descendant::draw:frame/draw:object[starts-with(@xlink:href,'../') or starts-with(@xlink:href,'/') and not(name(parent::node()/parent::node()) = 'draw:g' )]">
          <xsl:text>true</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>false</xsl:text>
        </xsl:otherwise>
      </xsl:choose>

    </xsl:variable>
    <xsl:if
      test="$comment = 'true' or $hyperlink='true' or contains($chart,'true') or $picture = 'true' or $textBox = 'true' or table:table-row/table:table-cell/table:cell-range-source or $OLEObject = 'true'
      or ($pivot = 'true' ) ">
      <!-- package relationship item -->
      <pzip:entry pzip:target="{concat('xl/worksheets/_rels/sheet',position(),'.xml.rels')}">
        <xsl:call-template name="InsertWorksheetsRels">
          <xsl:with-param name="sheetNum" select="position()"/>
          <xsl:with-param name="comment" select="$comment"/>
          <xsl:with-param name="picture" select="$picture"/>
          <xsl:with-param name="hyperlink" select="$hyperlink"/>
          <xsl:with-param name="chart" select="$chart"/>
          <xsl:with-param name="OLEObject">
            <xsl:value-of select="$OLEObject"/>
          </xsl:with-param>
          <xsl:with-param name="textBox" select="$textBox"/>
          <xsl:with-param name="pivot" select="$pivot"/>
        </xsl:call-template>
      </pzip:entry>

    </xsl:if>

  </xsl:template>

  <xsl:template name="CreateDrawingRelationships">
    <!-- package relationship item -->
    <pzip:entry pzip:target="{concat('xl/drawings/_rels/drawing',position(),'.xml.rels')}">
      <xsl:call-template name="InsertDrawingRels">
        <xsl:with-param name="sheetNum" select="position()"/>
      </xsl:call-template>
    </pzip:entry>
  </xsl:template>

  <xsl:template name="CreateVmlDrawingRelationships">
    <xsl:param name="sheetNum"/>

    <!-- package relationship item -->
    <pzip:entry pzip:target="{concat('xl/drawings/_rels/vmlDrawing',position(),'.vml.rels')}">
      <xsl:call-template name="InsertVmlDrawingRels">
        <xsl:with-param name="sheetNum" select="$sheetNum"/>
      </xsl:call-template>
    </pzip:entry>
  </xsl:template>

  <xsl:template name="CreateVmlDrawing">

    <xsl:variable name="sheetId" select="position()"/>
    <pzip:entry pzip:target="{concat('xl/drawings/vmlDrawing',position(),'.vml')}">

      <pxsi:dummyContainer xmlns:pxsi="urn:cleverage:xmlns:post-processings:comments">
        <o:shapelayout v:ext="edit">
          <o:idmap v:ext="edit" data="1"/>
        </o:shapelayout>
        <v:shapetype id="_x0000_t202" coordsize="21600,21600" o:spt="202"
          path="m,l,21600r21600,l21600,xe">
          <v:stroke joinstyle="miter"/>
          <v:path gradientshapeok="t" o:connecttype="rect"/>
        </v:shapetype>

        <!--For each comment-->
        <xsl:for-each
          select="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:table[$sheetId]/descendant::table:table-row/table:table-cell/office:annotation">
          <xsl:call-template name="InsertTextBox"/>
        </xsl:for-each>


        <!-- For each OLE object -->
		  <!--Defect Id       :1784784
			* Code Changed by :Vijayeta Tilak
			* Date            :26th Dec '07
			* Description     :This part of code( an additional '/') was added because when a notepad is inserted as an object, an addional elemnt
			*                  'table:shapes' is present which is absent when anyother object such as a word doc is inseretd
			-->
        <!--<xsl:for-each select="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:table[$sheetId]/table:shapes/draw:frame">-->
		  <xsl:for-each select="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:table[$sheetId]//draw:frame">

          <xsl:variable name="width">
            <xsl:call-template name="point-measure">
              <xsl:with-param name="length" select="@svg:width"/>
            </xsl:call-template>
          </xsl:variable>

          <xsl:variable name="height">
            <xsl:call-template name="point-measure">
              <xsl:with-param name="length" select="@svg:height"/>
            </xsl:call-template>
          </xsl:variable>

          <xsl:variable name="z-index">
            <xsl:value-of select="position()"/>
          </xsl:variable>

          <xsl:variable name="margin-left">
            <xsl:call-template name="point-measure">
              <xsl:with-param name="length" select="@svg:x"/>
            </xsl:call-template>
          </xsl:variable>

          <xsl:variable name="margin-top">
            <xsl:call-template name="point-measure">
              <xsl:with-param name="length" select="@svg:y"/>
            </xsl:call-template>
          </xsl:variable>
			  <xsl:variable name="anchorLeft">
				  <xsl:call-template name="InsertStartColumn"/>
			  </xsl:variable>
			  <xsl:variable name="anchorLeftOffset">
				  <!--<xsl:call-template name="InsertStartColumnOffset"/>-->
				  <xsl:value-of select ="round(substring-before(@svg:x,'in')*96.21212)"/>
			  </xsl:variable>
			  <xsl:variable name="anchorTop">
				  <xsl:call-template name="InsertStartRow"/>
			  </xsl:variable>
			  <xsl:variable name="anchorTopOffset">
				  <!--<xsl:call-template name="InsertStartRowOffset"/>-->
				  <xsl:value-of select ="round(substring-before(@svg:y,'in')*96.21212)"/>
			  </xsl:variable>
			  <xsl:variable name="anchorRight">
				  <xsl:call-template name="InsertEndColumn"/>
			  </xsl:variable>
			  <xsl:variable name="anchorRightOffset">
				  <!--<xsl:call-template name="InsertEndColumnOffset"/>-->
				  <xsl:value-of select ="round(substring-before(@table:end-x,'in')*96.21212)"/>
			  </xsl:variable>
			  <xsl:variable name="anchorBottom">
				  <xsl:call-template name="InsertEndRow"/>
			  </xsl:variable>
			  <xsl:variable name="anchorBottomOffset">
				  <xsl:value-of select ="round(substring-before(@table:end-y,'in')*96.21212)"/>
				  <!--<xsl:call-template name="InsertEndRowOffset"/>-->
			  </xsl:variable>
			  <!--Added by vijyeta,draw:notify-on-update-of-ranges appears when chart is present ,and xlink:href has path as object in it when the object is embedded, hence not handeled at present  -->
        <xsl:if test ="draw:object and not(./draw:object/@draw:notify-on-update-of-ranges)and not(contains(./draw:object/@xlink:href,'./Object'))">
          <v:shape type="#_x0000_t75"
            style="position:absolute;margin-left:{$margin-left}pt;margin-top:{$margin-top}pt;width:{$width}pt;height:{$height}pt;z-index:{$z-index}"
            filled="t" fillcolor="window [65]" stroked="t" strokecolor="windowText [64]"
            o:insetmode="auto">

            <xsl:attribute name="id">
              <xsl:value-of select="concat('_x0000_s', $sheetId * 1024 + position())"/>
            </xsl:attribute>
            <v:fill color2="window [65]"/>
            <v:imagedata o:relid="{concat('rId',position())}" o:title=""/>
            <x:ClientData ObjectType="Pict">
              <x:SizeWithCells/>
				<x:Anchor>
					<xsl:value-of select ="concat($anchorLeft,',',$anchorLeftOffset,',',$anchorTop,',',$anchorTopOffset,',',$anchorRight,',',$anchorRightOffset,',',$anchorBottom,',',$anchorBottomOffset)"/>
				</x:Anchor>
              <x:CF>Pict</x:CF>
              <x:AutoPict/>
              <x:DDE/>
            </x:ClientData>
          </v:shape>
          </xsl:if>
        </xsl:for-each>


      </pxsi:dummyContainer>

    </pzip:entry>
  </xsl:template>

  <xsl:template name="CreateDrawing">
    <pzip:entry pzip:target="{concat('xl/drawings/drawing',position(),'.xml')}">
      <xsl:call-template name="InsertDrawing"/>
    </pzip:entry>
  </xsl:template>

  <xsl:template name="CreateRevisionHeadersRels">
    <pzip:entry pzip:target="xl/revisions/_rels/revisionHeaders.xml.rels">
      <xsl:call-template name="InsertRevisionsRels"/>
    </pzip:entry>
  </xsl:template>

  <xsl:template name="CreatePivotTable">
    <!-- @Context: table:data-pilot-table -->
    <xsl:param name="sheetNum"/>

    <pzip:entry pzip:target="{concat('xl/pivotTables/pivotTable',$sheetNum,'_',position(),'.xml')}">
      <xsl:call-template name="InsertPivotTable"/>
    </pzip:entry>
  </xsl:template>

  <xsl:template name="CreatePivotTableRels">
    <!-- @Context: table:data-pilot-table -->
    <xsl:param name="sheetNum"/>

    <pzip:entry
      pzip:target="{concat('xl/pivotTables/_rels/pivotTable',$sheetNum,'_',position(),'.xml.rels')}">
      <xsl:call-template name="InsertPivotTableRels"/>
    </pzip:entry>
  </xsl:template>

  <xsl:template name="CreateCacheDefinition">
    <!-- @Context: table:data-pilot-table -->
    <pzip:entry pzip:target="{concat('xl/pivotCache/pivotCacheDefinition_',generate-id(.),'.xml')}">
      <xsl:call-template name="InsertCacheDefinition"/>
    </pzip:entry>
  </xsl:template>

  <xsl:template name="CreateCacheDefinitionRels">
    <!-- @Context: table:data-pilot-table -->
    <pzip:entry
      pzip:target="{concat('xl/pivotCache/_rels/pivotCacheDefinition_',generate-id(.),'.xml.rels')}">
      <xsl:call-template name="InsertCacheDefinitionRels"/>
    </pzip:entry>
  </xsl:template>

  <xsl:template name="CreateCacheRecords">
    <!-- @Context: table:data-pilot-table -->
    <pzip:entry pzip:target="{concat('xl/pivotCache/pivotCacheRecords_',generate-id(.),'.xml')}">
      <xsl:call-template name="InsertCacheRecords"/>
    </pzip:entry>
  </xsl:template>

</xsl:stylesheet>
