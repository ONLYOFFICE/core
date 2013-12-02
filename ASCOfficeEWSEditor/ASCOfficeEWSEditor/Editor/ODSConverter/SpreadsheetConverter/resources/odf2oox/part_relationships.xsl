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
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main"
  xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:pzip="urn:cleverage:xmlns:post-processings:zip"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:dc="http://purl.org/dc/elements/1.1/"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"
  xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
  exclude-result-prefixes="w r xlink office draw text style dc">

  <xsl:template name="InsertPartRelationships">
    <Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships">

      <!-- Sheet relationship -->
      <xsl:for-each
        select="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:table">
        <xsl:if test="not(table:scenario)">
          <Relationship Id="{generate-id(.)}"
            Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/worksheet">
            <xsl:variable name="NumberSheet">
              <xsl:value-of select="position()"/>
            </xsl:variable>
            <xsl:attribute name="Target">
              <xsl:value-of select="concat(concat('worksheets/sheet', $NumberSheet), '.xml')"/>
            </xsl:attribute>
          </Relationship>
        </xsl:if>
      </xsl:for-each>

      <xsl:if
        test="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:tracked-changes">
        <Relationship Id="{generate-id(.)}"
          Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/revisionHeaders"
          Target="revisions/revisionHeaders.xml"/>
        <Relationship Id="{concat(generate-id(.), 1)}"
          Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/usernames"
          Target="revisions/userNames.xml"/>
      </xsl:if>

      <!--  Static relationships -->
      <Relationship Id="rId1"
        Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles"
        Target="styles.xml"/>
      <Relationship Id="rId2"
        Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/sharedStrings"
        Target="sharedStrings.xml"/>
      <Relationship Id="rId3"
        Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/connections"
        Target="connections.xml"/>

      <xsl:for-each select="document('content.xml')">

        <!--Insert OLE relationships -->
		  <!--Defect Id     :1784784
			* Code Changed by :Vijayeta Tilak
			* Date            :26th Dec '07
			* Description     :This part of code( an OR condition to 'for-each' loop) was added because when a file contains OLE object directly in a drive
			*                  then the value of attibute 'xlink:href' begins from a '/' and not '../'(which offcourse means within the folder.	
			-->
        <xsl:for-each
          select="descendant::draw:frame/draw:object[starts-with(@xlink:href,'../') or starts-with(@xlink:href,'/') and not(name(parent::node()/parent::node()) = 'draw:g' )]">
          <Relationship xmlns="http://schemas.openxmlformats.org/package/2006/relationships"
            Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/externalLink">
            <xsl:attribute name="Id">
              <xsl:value-of select="generate-id()"/>
            </xsl:attribute>

            <xsl:attribute name="Target">
              <!--xsl:if test="draw:object|draw:object-ole"-->
              <xsl:value-of
                select="concat(concat('externalLinks/externalLink', position()), '.xml')"/>
              <!--/xsl:if-->
            </xsl:attribute>
          </Relationship>
        </xsl:for-each>

        <xsl:for-each select="key('pivot','')">
          <xsl:variable name="pivotSource">
            <xsl:value-of select="table:source-cell-range/@table:cell-range-address"/>
          </xsl:variable>

          <!-- do not duplicate the same source range cache -->
          <xsl:if
            test="not(preceding-sibling::table:data-pilot-table[table:source-cell-range/@table:cell-range-address = $pivotSource])">
            
            <!-- don't convert pivot tables with empty cells in first row since Excel doesn't support them -->
            <xsl:variable name="apos">'</xsl:variable>
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
            <Relationship Id="{generate-id()}"
              Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/pivotCacheDefinition"
              Target="{concat('pivotCache/pivotCacheDefinition_',generate-id(),'.xml')}"/>
            </xsl:if>
          </xsl:if>
        </xsl:for-each>
      </xsl:for-each>

    </Relationships>

  </xsl:template>


  <xsl:template name="InsertRevisionsRels">
    <Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships">
      <xsl:for-each
        select="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:tracked-changes/node()[name()='table:cell-content-change' or name()='table:deletion' or name()='table:movement' or name()='table:insertion']">
        <Relationship Id="{generate-id()}"
          Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/revisionLog"
          Target="{concat('revisionLog', generate-id(), '.xml')}"/>
      </xsl:for-each>
    </Relationships>
  </xsl:template>

  <xsl:template match="table:cell-content-change|table:deletion|table:deletion|table:movement"
    mode="revisionRels">

    <Relationship Id="{generate-id()}"
      Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/revisionLog"
      Target="{concat('revisionLog', generate-id(), '.xml')}"/>

    <xsl:if
      test="following-sibling::node()[1][name()='table:cell-content-change' or name()='table:deletion']">
      <xsl:apply-templates
        select="following-sibling::node()[1][name()='table:cell-content-change' or name()='table:deletion' or name()='table:movement']"
        mode="revisionRels"/>
    </xsl:if>

  </xsl:template>

  <xsl:template name="TranslateIllegalChars">
    <xsl:param name="string"/>

    <xsl:choose>

      <!-- remove space-->
      <xsl:when test="contains($string,' ')">
        <xsl:choose>
          <xsl:when test="substring-before($string,' ') =''">
            <xsl:call-template name="TranslateIllegalChars">
              <xsl:with-param name="string" select="substring-after($string,' ')"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:when test="substring-before($string,' ') !=''">
            <xsl:call-template name="TranslateIllegalChars">
              <xsl:with-param name="string"
                select="concat(substring-before($string,' '),substring-after($string,' '))"/>
            </xsl:call-template>
          </xsl:when>
        </xsl:choose>
      </xsl:when>

      <!-- change  '&lt;' to '%3C'  after conversion-->
      <xsl:when test="contains($string,'&lt;')">
        <xsl:choose>
          <xsl:when test="substring-before($string,'&lt;') =''">
            <xsl:call-template name="TranslateIllegalChars">
              <xsl:with-param name="string"
                select="concat('%3C',substring-after($string,'&lt;'))"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:when test="substring-before($string,'&lt;') !=''">
            <xsl:call-template name="TranslateIllegalChars">
              <xsl:with-param name="string"
                select="concat(substring-before($string,'&lt;'),'%3C',substring-after($string,'&lt;'))"
              />
            </xsl:call-template>
          </xsl:when>
        </xsl:choose>
      </xsl:when>

      <!-- change  '&gt;' to '%3E'  after conversion-->
      <xsl:when test="contains($string,'&gt;')">
        <xsl:choose>
          <xsl:when test="substring-before($string,'&gt;') =''">
            <xsl:call-template name="TranslateIllegalChars">
              <xsl:with-param name="string"
                select="concat('%3E',substring-after($string,'&gt;'))"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:when test="substring-before($string,'&gt;') !=''">
            <xsl:call-template name="TranslateIllegalChars">
              <xsl:with-param name="string"
                select="concat(substring-before($string,'&gt;'),'%3E',substring-after($string,'&gt;'))"
              />
            </xsl:call-template>
          </xsl:when>
        </xsl:choose>
      </xsl:when>

      <xsl:otherwise>
        <xsl:value-of select="$string"/>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>


  <xsl:template name="InsertWorksheetsRels">
    <!-- @Context: table:table -->
    <xsl:param name="sheetNum"/>
    <xsl:param name="comment"/>
    <xsl:param name="picture"/>
    <xsl:param name="hyperlink"/>
    <xsl:param name="chart"/>
    <xsl:param name="textBox"/>
    <xsl:param name="OLEObject"/>
    <xsl:param name="pivot"/>

    <xsl:variable name="tableName">
      <xsl:value-of select="@table:name"/>
    </xsl:variable>
    
    <xsl:variable name="apos">
      <xsl:text>&apos;</xsl:text>
    </xsl:variable>

    <Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships">
      <!-- comments.xml file -->
      <xsl:if test="$comment = 'true' ">
        <Relationship Id="{concat('rId',$sheetNum+1)}"
          Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/comments"
          Target="{concat('../comments',$sheetNum,'.xml')}"/>
      </xsl:if>

      <!-- vmlDrawing.vml file -->
      <!--xsl:if test="$comment = 'true' ">
        <Relationship Id="{concat('v_rId',$sheetNum)}"
          Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/vmlDrawing"
          Target="{concat('../drawings/vmlDrawing',$sheetNum,'.vml')}"/>
      </xsl:if-->

      <!--hyperlink-->
      <xsl:if test="$hyperlink = 'true' ">
        <xsl:for-each
          select="descendant::text:a[not(ancestor::draw:custom-shape) and not(ancestor::office:annotation)]">

          <Relationship Id="{generate-id(.)}"
            Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink"
            TargetMode="External">

            <xsl:attribute name="Target">
              <xsl:if test="@xlink:href">
                <xsl:choose>
                  <!-- when hyperlink to a site or mailto-->
                  <xsl:when test="contains(@xlink:href,':') and not(starts-with(@xlink:href,'/'))">
                    <xsl:call-template name="TranslateIllegalChars">
                      <xsl:with-param name="string" select="@xlink:href"/>
                    </xsl:call-template>
                  </xsl:when>
                  <!-- when hyperlink to an document-->
                  <xsl:otherwise>

                    <xsl:variable name="translatedTarget">
                      <xsl:call-template name="SpaceTo20Percent">
                        <xsl:with-param name="string" select="@xlink:href"/>
                      </xsl:call-template>
                    </xsl:variable>

                    <xsl:choose>
                      <!-- when starts with up folder sign -->
                      <xsl:when test="starts-with($translatedTarget,'../' )">
                        <xsl:value-of
                          select="translate(substring-after($translatedTarget,'../'),'/','\')"/>
                      </xsl:when>
                      <!-- when file is in another disk -->
                      <xsl:when test="starts-with($translatedTarget,'/')">
                        <xsl:value-of
                          select="concat('file:///',translate(substring-after($translatedTarget,'/'),'/','\'))"
                        />
                      </xsl:when>
                      <xsl:otherwise>
                        <xsl:value-of select="translate($translatedTarget,'/','\')"/>
                      </xsl:otherwise>
                    </xsl:choose>

                  </xsl:otherwise>
                </xsl:choose>
              </xsl:if>
            </xsl:attribute>
          </Relationship>
        </xsl:for-each>
      </xsl:if>

      <!-- Extrnal Link -->
      <xsl:for-each
        select="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:table/table:table-row/table:table-cell/table:cell-range-source">

        <Relationship Id="{generate-id()}"
          Target="{concat('../queryTables/queryTable', position(), '.xml')}"
          Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/queryTable"/>

      </xsl:for-each>

      <!-- drawing.xml file -->
      <xsl:if test="contains($chart,'true') or $picture = 'true' or $textBox = 'true' ">
        <Relationship Id="{concat('d_rId',$sheetNum)}"
          Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/drawing"
          Target="{concat('../drawings/drawing',$sheetNum,'.xml')}"/>
      </xsl:if>

      <xsl:if test="$OLEObject = 'true' or $comment = 'true'">
        <Relationship Id="v_rId1"
          Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/vmlDrawing"
          Target="{concat('../drawings/vmlDrawing', $sheetNum,'.vml')}"/>
      </xsl:if>

      <xsl:if test="$pivot = 'true' ">
        <xsl:for-each
          select="key('pivot','')[translate(substring-before(@table:target-range-address,'.'),$apos,'') = $tableName]">
          
          <!-- don't convert pivot tables with empty cells in first row since Excel doesn't support them -->
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
          <Relationship Id="{generate-id(.)}"
            Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/pivotTable"
            Target="{concat('../pivotTables/pivotTable',$sheetNum,'_',position(),'.xml')}"/>
            </xsl:if>
        </xsl:for-each>
      </xsl:if>
    </Relationships>
  </xsl:template>


  <xsl:template name="InsertChartRels">
    <xsl:param name="chartDirectory"/>

    <Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships">
      <!-- search for bitmap fills -->
      <xsl:for-each
        select="/office:document-content/office:automatic-styles/style:style/style:graphic-properties[@draw:fill = 'bitmap' ]">

        <xsl:variable name="fillImage">
          <xsl:value-of select="@draw:fill-image-name"/>
        </xsl:variable>

        <!-- id based on a style in content.xml -->
        <xsl:variable name="fillId">
          <xsl:value-of select="generate-id()"/>
        </xsl:variable>

        <!-- go to bitmap-fill style -->
        <xsl:for-each
          select="document(concat($chartDirectory,'/styles.xml'))/office:document-styles/office:styles/draw:fill-image[@draw:name = $fillImage]">
          <!--Vijayeta,SP2-->
          <!--<xsl:variable name="imageName" select="substring-after(@xlink:href, 'Pictures/')"/>-->
			<xsl:variable name="imageName" select="substring-after(@xlink:href, '/')"/>

          <pzip:copy pzip:source="{concat($chartDirectory,'/',@xlink:href)}"
            pzip:target="xl/media/{concat($fillId,$imageName)}"/>

          <Relationship
            Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/image">
            <xsl:attribute name="Id">
              <xsl:value-of select="$fillId"/>
            </xsl:attribute>

            <xsl:attribute name="Target">
              <xsl:text>../media/</xsl:text>
              <!-- the same bitmap fill in two diferent charts has the same name so to distinct them it is preceded with generate-id() -->
              <xsl:value-of select="concat($fillId,$imageName)"/>
            </xsl:attribute>
          </Relationship>
        </xsl:for-each>

      </xsl:for-each>
    </Relationships>
  </xsl:template>

  <xsl:template name="InsertDrawingRels">
    <xsl:param name="sheetNum"/>

    <Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships">
      <!-- chart rels -->
      <xsl:variable name="chart">
        <xsl:for-each select="descendant::draw:frame/draw:object">
          <xsl:choose>
            <xsl:when test="not(document(concat(translate(@xlink:href,'./',''),'/settings.xml')))">
              <xsl:for-each select="document(concat(translate(@xlink:href,'./',''),'/content.xml'))">
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

      <xsl:if test="contains($chart, 'true')">
        <xsl:for-each
          select="descendant::draw:frame/draw:object[document(concat(translate(@xlink:href,'./',''),'/content.xml'))/office:document-content/office:body/office:chart]">
          <Relationship Id="{generate-id(parent::node())}"
            Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/chart"
            Target="{concat('../charts/chart',$sheetNum,'_',position(),'.xml')}"/>
        </xsl:for-each>
      </xsl:if>

      <!-- pictures -->
      <xsl:for-each
        select="descendant::draw:frame/draw:image[not(name(parent::node()/parent::node()) = 'draw:g' )  and not(parent::node()/draw:object)]">

        <xsl:choose>
          <!-- embeded pictures -->
			<!--Vijayeta,SP2,Pictures-->
           <!--<xsl:when test="starts-with(@xlink:href, 'Pictures/')">
		       <xsl:variable name="imageName" select="substring-after(@xlink:href, 'Pictures/')"/> -->			
			<xsl:when test="starts-with(@xlink:href, 'Pictures/') or starts-with(@xlink:href, 'media/')">
            <xsl:variable name="imageName" select="substring-after(@xlink:href, '/')"/>
			  <!-- Code added by vijayeta, fix for the bug 1806778
				   check image is of type .svm, as OOX does not support image of type .svm
				   Date:23rd Oct '07-->
			  <xsl:if test ="not(contains($imageName,'.svm'))">
            <pzip:copy pzip:source="{@xlink:href}" pzip:target="xl/media/{$imageName}"/>
            <Relationship xmlns="http://schemas.openxmlformats.org/package/2006/relationships"
              Id="{generate-id(parent::node())}"
              Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/image"
              Target="../media/{$imageName}"/>
			  </xsl:if>
			  <!--End of Code added by vijayeta, for the bug 1806778 -->
          </xsl:when>
          <!-- linked pictures -->
          <xsl:otherwise>
            <!-- change spaces to %20 -->
            <xsl:variable name="translatedTarget">
              <xsl:call-template name="TranslateIllegalChars">
                <xsl:with-param name="string" select="@xlink:href"/>
              </xsl:call-template>
            </xsl:variable>
            <xsl:choose>
              <!-- picture is located in the same disk -->
              <xsl:when test="starts-with($translatedTarget,'../')">
                <!--<pxsi:physicalPath xmlns:pxsi="urn:cleverage:xmlns:post-processings:path">-->
                  <Relationship xmlns="http://schemas.openxmlformats.org/package/2006/relationships"
                    Id="{generate-id(parent::node())}"
                    Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/image"
                    TargetMode="External">
                    <xsl:message terminate="no">translation.odf2oox.draggedImagePath</xsl:message>
                    <xsl:attribute name="Target">
                          <!--Absolute path -->
				<xsl:value-of select ="concat('Image-Path:',@xlink:href)"/>
                      <!--<xsl:value-of
                        select="translate(substring-after($translatedTarget,'../'),'/','\')"/>-->
                    </xsl:attribute>
                  </Relationship>
                <!--</pxsi:physicalPath>-->
              </xsl:when>
              <!-- when file is on another disk -->
              <xsl:otherwise>
                <xsl:variable name="target">
                  <xsl:choose>
                    <!-- when file is on local disk -->
                    <xsl:when test="starts-with($translatedTarget,'/')">
                      <xsl:value-of
                        select="concat('file:///',translate(substring-after($translatedTarget,'/'),'/','\'))"
                      />
                    </xsl:when>
                    <!-- when file is on network disk -->
                    <xsl:otherwise>
                      <xsl:value-of select="translate($translatedTarget,'/','\')"/>
                    </xsl:otherwise>
                  </xsl:choose>
                </xsl:variable>

                <Relationship xmlns="http://schemas.openxmlformats.org/package/2006/relationships"
                  Id="{generate-id(parent::node())}"
                  Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/image"
                  Target="{$target}" TargetMode="External"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:for-each>

		<!--Defect Id     :1784784
			* Code Changed by :Vijayeta Tilak
			* Date            :26th Dec '07
			* Description     :This part of code( an OR condition to 'for-each' loop) was added because when a file contains OLE object directly in a drive
			*                  then the value of attibute 'xlink:href' begins from a '/' and not '../'(which offcourse means within the folder.	
			-->
      <xsl:for-each
        select="descendant::draw:frame/draw:object[starts-with(@xlink:href,'../') or starts-with(@xlink:href,'/') and not(name(parent::node()/parent::node()) = 'draw:g' )]">

        <Relationship
          Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/image">

          <xsl:attribute name="Id">
            <xsl:value-of select="concat('rId',position())"/>
          </xsl:attribute>

          <xsl:attribute name="Target">
            <xsl:value-of select="concat('../media/image',position(),'.emf')"/>
          </xsl:attribute>
        </Relationship>

      </xsl:for-each>
		<!-- Code added by vijayeta, Fix for the bug 1760182, date:23rd Oct '07
		     If the text box has Hyperlink-->
		<xsl:if test ="descendant::draw:frame/draw:text-box/text:p/text:a">
			<xsl:for-each select="descendant::draw:frame/draw:text-box/text:p/text:a">
				<xsl:variable name ="hlinkPos">
					<xsl:value-of select ="generate-id(.)"/>
				</xsl:variable>
				<Relationship>
					<xsl:choose >
						<xsl:when test="@xlink:href[contains(.,'http://') or contains(.,'mailto:')]">
							<xsl:attribute name ="Id">
								<xsl:value-of select ="concat('mailLinkTextBox',$hlinkPos)"/>
							</xsl:attribute>
							<xsl:attribute name="Type">
								<xsl:value-of select="'http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink'"/>
							</xsl:attribute>
							<xsl:attribute name="Target">
								<xsl:value-of select="@xlink:href"/>
							</xsl:attribute>
							<xsl:attribute name="TargetMode">
								<xsl:value-of select="'External'"/>
							</xsl:attribute>
						</xsl:when>
						<xsl:otherwise>
							<xsl:if test="@xlink:href[ contains (.,':') ]">
								<xsl:attribute name="Id">
									<xsl:value-of select="concat('fileTextBox',$hlinkPos)"/>
								</xsl:attribute>
								<xsl:attribute name="Type">
									<xsl:value-of select="'http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink'"/>
								</xsl:attribute>
								<xsl:attribute name="Target">
									<xsl:value-of select="concat('file:///',translate(substring-after(@xlink:href,'/'),'/','\'))"/>
								</xsl:attribute>
								<xsl:attribute name="TargetMode">
									<xsl:value-of select="'External'"/>
								</xsl:attribute>
							</xsl:if>
							<xsl:if test="not(@xlink:href[ contains (.,':') ])">
								<xsl:attribute name="Id">
									<xsl:value-of select="concat('fileTextBox',$hlinkPos)"/>
								</xsl:attribute>
								<xsl:attribute name="Type">
									<xsl:value-of select="'http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink'"/>
								</xsl:attribute>
								<xsl:attribute name="Target">
									<!--links Absolute Path-->
									<xsl:variable name ="xlinkPath" >
										<xsl:value-of select ="@xlink:href"/>
									</xsl:variable>
									<xsl:value-of select ="concat('hyperlink-path:',$xlinkPath)"/>
								</xsl:attribute>
								<xsl:attribute name="TargetMode">
									<xsl:value-of select="'External'"/>
								</xsl:attribute>
							</xsl:if>
						</xsl:otherwise>
					</xsl:choose>
				</Relationship>
			</xsl:for-each>
		</xsl:if>
		<!--End of Code added by vijayeta, Fix for the bug 1760182, date:23rd Oct '07-->
    </Relationships>
  </xsl:template>

	      <!--Defect Id     :1784784
			* Code Changed by :Vijayeta Tilak
			* Date            :26th Dec '07
			* Description     :This part of code( an OR condition to 'for-each' loop) was added because when a file contains OLE object directly in a drive
			*                  then the value of attibute 'xlink:href' begins from a '/' and not '../'(which offcourse means within the folder.	
			-->
	<xsl:template name="InsertVmlDrawingRels">	  
    <xsl:param name="sheetNum"/>
    <Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships">
      <xsl:for-each
        select="descendant::draw:frame/draw:object[starts-with(@xlink:href,'../') or starts-with(@xlink:href,'/')  and not(name(parent::node()/parent::node()) = 'draw:g' )]">
        <Relationship
          Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/image">
          <xsl:attribute name="Id">
            <xsl:value-of select="concat('rId',position())"/>
          </xsl:attribute>
          <xsl:variable name="ObjectNumber">
            <xsl:value-of select="substring-after(@xlink:href, './ObjectReplacements/Object ')"/>
          </xsl:variable>
          <xsl:attribute name="Target">
            <xsl:value-of select="concat('../media/image',$sheetNum,'_',position(),'.emf')"/>
          </xsl:attribute>
        </Relationship>
      </xsl:for-each>
    </Relationships>
  </xsl:template>

  <xsl:template name="SpaceTo20Percent">
    <xsl:param name="string"/>

    <xsl:choose>
      <!-- change space to  '%20' after conversion-->
      <xsl:when test="contains($string,' ')">
        <xsl:choose>
          <xsl:when test="substring-before($string,' ') =''">
            <xsl:call-template name="TranslateIllegalChars">
              <xsl:with-param name="string" select="concat('%20',substring-after($string,' '))"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:when test="substring-before($string,' ') !=''">
            <xsl:call-template name="SpaceTo20Percent">
              <xsl:with-param name="string"
                select="concat(substring-before($string,' '),'%20',substring-after($string,' '))"/>
            </xsl:call-template>
          </xsl:when>
        </xsl:choose>
      </xsl:when>

      <xsl:otherwise>
        <xsl:value-of select="$string"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="InsertLinkExternalRels">

    <xsl:for-each
      select="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:table/table:table-row/table:table-cell/table:cell-range-source">

      <xsl:call-template name="InsertQueryTable"/>

    </xsl:for-each>
  </xsl:template>


  <!-- Insert Revision Headers -->
  <xsl:template name="revisionHeaderProperties">
    <!--<headers xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main"
      xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
      guid="{concat('{', '100', '}')}" diskRevisions="1" revisionId="2" version="3">-->
    <headers xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main"
      xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
	 diskRevisions="1" revisionId="2" version="3">
		  <!--OpenXML Validator-->
		  <xsl:attribute name="guid">
			  <xsl:text>{</xsl:text>
			  <xsl:value-of select="'sonataRevisionHeaderGuid'"/>
			  <xsl:text>}</xsl:text>
		  </xsl:attribute>
      <xsl:for-each
        select="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:tracked-changes">
        <xsl:apply-templates
          select="node()[1][name()='table:cell-content-change' or name()='table:deletion' or name()='table:movement' or name()='table:insertion']"
          mode="revisionHeaders">
          <xsl:with-param name="num">
            <xsl:text>1</xsl:text>
          </xsl:with-param>
        </xsl:apply-templates>
      </xsl:for-each>
    </headers>
  </xsl:template>

  <xsl:template match="table:cell-content-change|table:deletion|table:movement|table:insertion"
    mode="revisionHeaders">
    <xsl:param name="num"/>

    <header maxSheetId="4" r:id="{generate-id()}" >
		<!--guid="{concat('{', $num, '}')}"-->
      <xsl:attribute name="userName">
        <xsl:value-of select="office:change-info/dc:creator"/>
      </xsl:attribute>
      <xsl:attribute name="dateTime">
        <xsl:value-of select="office:change-info/dc:date"/>
      </xsl:attribute>
		<xsl:attribute name="guid">
			<xsl:text>{</xsl:text>
			<xsl:value-of select="'sonataRevisionHeaderGuid'"/>
			<xsl:text>}</xsl:text>
		</xsl:attribute>
      <sheetIdMap>
        <xsl:for-each
          select="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:table">
          <sheetId>
            <xsl:attribute name="val">
              <xsl:value-of select="position()"/>
            </xsl:attribute>
          </sheetId>
        </xsl:for-each>
      </sheetIdMap>
    </header>

    <xsl:if
      test="following-sibling::node()[1][name()='table:cell-content-change' or name()='table:deletion' or name()='table:movement' or name()='table:insertion']">
      <xsl:apply-templates
        select="following-sibling::node()[1][name()='table:cell-content-change' or name()='table:deletion' or name()='table:movement' or name()='table:insertion']"
        mode="revisionHeaders">
        <xsl:with-param name="num">
          <xsl:value-of select="$num + 1"/>
        </xsl:with-param>
      </xsl:apply-templates>
    </xsl:if>

  </xsl:template>

  <xsl:template name="InsertPivotTableRels">
    <!-- @Context: table:data-pilot-table -->
    <Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships">
      <Relationship Id="{generate-id(.)}"
        Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/pivotCacheDefinition">

        <xsl:variable name="pivotSource">
          <xsl:value-of select="table:source-cell-range/@table:cell-range-address"/>
        </xsl:variable>

        <!-- go to the first pivotTable on that source range -->
        <xsl:for-each
          select="key('pivot','')[table:source-cell-range/@table:cell-range-address = $pivotSource][1]">
          <xsl:attribute name="Target">
            <xsl:value-of
              select="concat('../pivotCache/pivotCacheDefinition_',generate-id(.),'.xml')"/>
          </xsl:attribute>
        </xsl:for-each>
      </Relationship>
    </Relationships>
  </xsl:template>

  <xsl:template name="InsertCacheDefinitionRels">
    <!-- @Context: table:data-pilot-table -->
    <Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships">
      <Relationship Id="{generate-id(.)}"
        Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/pivotCacheRecords"
        Target="{concat('pivotCacheRecords_',generate-id(.),'.xml')}"/>
    </Relationships>
  </xsl:template>

</xsl:stylesheet>
