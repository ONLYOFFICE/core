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
Log		  Date 	     Modified   By       BugNo.		MOdification description
RefNo-1	1-Feb-2008 Sandeep s           1835598   Changes done to fix bug:XLSX: Textbox with hyperlink not properly converted 
-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
  xmlns:form="urn:oasis:names:tc:opendocument:xmlns:form:1.0"
  xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns:xdr="http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing"
  xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main"
  xmlns:number="urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0"
  xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
  xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:dc="http://purl.org/dc/elements/1.1/"
  xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart"
  xmlns:e="http://schemas.openxmlformats.org/spreadsheetml/2006/main"
  xmlns:rels="http://schemas.openxmlformats.org/package/2006/relationships"
  xmlns:oox="urn:oox"
  exclude-result-prefixes="e oox r c xdr draw rels xlink">

  <!--<xsl:import href="relationships.xsl"/>
  <xsl:import href="chart.xsl"/>
  <xsl:import href="insert_text.xsl"/>-->
  <!-- Added by vijayeta, fix fo the bug 1760182-->
 

  <xsl:key name="drawing" match="e:drawing" use="@oox:part"/>

  <!-- Get cell with picture -->
  <xsl:template name="PictureCell">
    <xsl:param name="sheet"/>
    <xsl:param name="mode"/>

    <xsl:choose>
      <!-- when this is a chartsheet (chartsheets/sheet1.xml)-->
      <xsl:when test="$mode = 'chartsheets' ">
        <xsl:apply-templates select="e:chartsheet/e:drawing" mode="chartsheet">
          <xsl:with-param name="sheet">
            <xsl:value-of select="$sheet"/>
          </xsl:with-param>
        </xsl:apply-templates>
      </xsl:when>
      <xsl:otherwise>
        <xsl:apply-templates select="e:worksheet/e:drawing">
          <xsl:with-param name="sheet">
            <xsl:value-of select="$sheet"/>
          </xsl:with-param>
        </xsl:apply-templates>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- Get Row with Picture -->
  <xsl:template name="PictureRow">
    <xsl:param name="PictureCell"/>
    <xsl:param name="Result"/>
    <xsl:choose>
      <xsl:when test="$PictureCell != ''">
        <xsl:call-template name="PictureRow">
          <xsl:with-param name="PictureCell">
            <xsl:value-of select="substring-after($PictureCell, ';')"/>
          </xsl:with-param>
          <xsl:with-param name="Result">
            <xsl:value-of
              select="concat($Result,  concat(substring-before($PictureCell, ':'), ';'))"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$Result"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="e:drawing">
    <xsl:param name="sheet"/>

    <xsl:variable name="Target">
      <xsl:call-template name="GetTargetPicture">
        <xsl:with-param name="sheet">
          <xsl:value-of select="$sheet"/>
        </xsl:with-param>
        <xsl:with-param name="id">
          <xsl:value-of select="@r:id"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:for-each select="key('Part', concat('xl/', substring-after($Target, '/')))">
			<!--SP2,Scenario:ods_SP2_xlsx_2.5_ods,ChartLost because SP2 created xlsx has oneCellAnchor in Place of twoCellAnchor-->
			<!--<xsl:if test="xdr:wsDr/xdr:twoCellAnchor">
        <xsl:apply-templates select="xdr:wsDr/xdr:twoCellAnchor[1]"/>
			    </xsl:if>-->
			<xsl:choose>
				<xsl:when test="xdr:wsDr/xdr:twoCellAnchor">
					<xsl:apply-templates select="xdr:wsDr/xdr:twoCellAnchor[1]"/>
				</xsl:when>
				<xsl:when test="xdr:wsDr/xdr:oneCellAnchor">
					<xsl:apply-templates select="xdr:wsDr/xdr:oneCellAnchor[1]"/>
				</xsl:when>
			</xsl:choose>
			<!--SP2,Scenario:ods_SP2_xlsx_2.5_ods,End-->
    </xsl:for-each>
  </xsl:template>

  <xsl:template match="e:drawing" mode="chartsheet">
    <xsl:param name="sheet"/>

    <xsl:variable name="id">
      <xsl:value-of select="@r:id"/>
    </xsl:variable>

    <xsl:variable name="Target">
      <xsl:if
        test="key('Part', concat(concat('xl/chartsheets/_rels/', $sheet), '.rels'))//node()[name()='Relationship']">
        <xsl:for-each
          select="key('Part', concat(concat('xl/chartsheets/_rels/', $sheet), '.rels'))//node()[name()='Relationship']">
          <xsl:if test="./@Id=$id">
            <xsl:value-of select="@Target"/>
          </xsl:if>
        </xsl:for-each>
      </xsl:if>
    </xsl:variable>

    <xsl:for-each select="key('Part', concat('xl/', substring-after($Target, '/')))">
      <xsl:if test="xdr:wsDr/xdr:absoluteAnchor">
        <xsl:text>1:1;</xsl:text>
      </xsl:if>
    </xsl:for-each>
  </xsl:template>

  <!-- We check drawing's file -->
  <xsl:template name="GetTargetPicture">
    <xsl:param name="id"/>
    <xsl:param name="sheet"/>
    <xsl:if
      test="key('Part', concat(concat('xl/worksheets/_rels/', $sheet), '.rels'))//node()[name()='Relationship']">
      <xsl:for-each
        select="key('Part', concat(concat('xl/worksheets/_rels/', $sheet), '.rels'))//node()[name()='Relationship']">
        <xsl:if test="./@Id=$id">
          <xsl:value-of select="@Target"/>
        </xsl:if>
      </xsl:for-each>
    </xsl:if>
  </xsl:template>


  <!-- We check cell when the picture is starting and ending -->

  <xsl:template match="xdr:twoCellAnchor">
    <xsl:param name="PictureCell"/>

    <xsl:variable name="PictureColStart">
      <xsl:choose>
        <xsl:when test="xdr:pic/xdr:spPr/a:xfrm/@flipV = 1">
          <xsl:value-of select="xdr:to/xdr:col + 1"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="xdr:from/xdr:col + 1"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="PictureRowStart">
      <xsl:choose>
        <xsl:when test="xdr:pic/xdr:spPr/a:xfrm/@flipV = 1">
          <xsl:value-of select="xdr:to/xdr:row + 1"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="xdr:from/xdr:row + 1"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="following-sibling::xdr:twoCellAnchor[1]">
        <xsl:apply-templates select="following-sibling::xdr:twoCellAnchor[1]">
          <xsl:with-param name="PictureCell">
            <xsl:value-of
              select="concat(concat(concat(concat($PictureCell, $PictureRowStart), ':'), $PictureColStart), ';')"
            />
          </xsl:with-param>
        </xsl:apply-templates>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of
          select="concat(concat(concat(concat($PictureCell, $PictureRowStart), ':'), $PictureColStart), ';')"
        />
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>
	<!--SP2,Scenario:ods_SP2_xlsx_2.5_ods,ChartLost because SP2 created xlsx has oneCellAnchor in Place of twoCellAnchor-->
	<xsl:template match="xdr:oneCellAnchor">
		<xsl:param name="PictureCell"/>
		<xsl:variable name="PictureColStart">
			<xsl:choose>
				<xsl:when test="xdr:pic/xdr:spPr/a:xfrm/@flipV = 1">
					<xsl:value-of select="xdr:to/xdr:col + 1"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="xdr:from/xdr:col + 1"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="PictureRowStart">
			<xsl:choose>
				<xsl:when test="xdr:pic/xdr:spPr/a:xfrm/@flipV = 1">
					<xsl:value-of select="xdr:to/xdr:row + 1"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="xdr:from/xdr:row + 1"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="following-sibling::xdr:oneCellAnchor[1]">
				<xsl:apply-templates select="following-sibling::xdr:oneCellAnchor[1]">
					<xsl:with-param name="PictureCell">
						<xsl:value-of
						  select="concat(concat(concat(concat($PictureCell, $PictureRowStart), ':'), $PictureColStart), ';')"/>
					</xsl:with-param>
				</xsl:apply-templates>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of
				  select="concat(concat(concat(concat($PictureCell, $PictureRowStart), ':'), $PictureColStart), ';')"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<!--SP2,Scenario:ods_SP2_xlsx_2.5_ods,End-->

  <!-- Insert picture -->

  <xsl:template name="InsertPicture">
    <xsl:param name="sheet"/>
    <xsl:param name="NameSheet"/>
    <xsl:param name="Drawing"/>

    <xsl:choose>
      <!-- insert picture -->
      <xsl:when test="xdr:pic">
        <draw:frame draw:z-index="0" draw:name="Graphics 1" draw:text-style-name="P1">
          <!--style name-->
          <xsl:attribute name="draw:style-name">
            <xsl:value-of select="generate-id(.)"/>
          </xsl:attribute>

          <!-- position -->
          <xsl:call-template name="SetPosition">
            <xsl:with-param name="sheet">
              <xsl:value-of select="$sheet"/>
            </xsl:with-param>
            <xsl:with-param name="NameSheet">
              <xsl:value-of select="$NameSheet"/>
            </xsl:with-param>
          </xsl:call-template>

          <xsl:for-each select="xdr:pic/xdr:spPr">
            <xsl:call-template name="InsertImageFlip">
              <xsl:with-param name="atribute">
                <xsl:text>draw</xsl:text>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:for-each>

          <draw:image xlink:type="simple" xlink:show="embed" xlink:actuate="onLoad">
            <xsl:call-template name="InsertImageHref">
              <xsl:with-param name="document">
                <xsl:value-of select="concat($Drawing, '.rels')"/>
              </xsl:with-param>
            </xsl:call-template>

            <text:p/>
          </draw:image>

        </draw:frame>
      </xsl:when>

      <!-- insert chart -->
      <xsl:when test="xdr:graphicFrame/a:graphic/a:graphicData/c:chart">
        <draw:frame draw:z-index="0">
          <!-- position -->
          <xsl:call-template name="SetPosition">
            <xsl:with-param name="sheet">
              <xsl:value-of select="$sheet"/>
            </xsl:with-param>
            <xsl:with-param name="NameSheet">
              <xsl:value-of select="$NameSheet"/>
            </xsl:with-param>
          </xsl:call-template>
          <draw:object xlink:type="simple" xlink:show="embed" xlink:actuate="onLoad">
            <xsl:attribute name="xlink:href">
              <xsl:value-of
                select="concat('./Object ',generate-id(xdr:graphicFrame/a:graphic/a:graphicData/c:chart))"
              />
            </xsl:attribute>
          </draw:object>
        </draw:frame>
      </xsl:when>

      <!-- insert text-box -->
      <xsl:when test="xdr:sp/xdr:nvSpPr/xdr:cNvSpPr/@txBox = 1">
        <draw:frame draw:z-index="0">
          <!-- Code added by vijayeta, Fix for the bug 1760182, date:23rd Oct '07
		         If the text box has Hyperlink, variables defined-->
          <xsl:variable name ="hlinkId">
            <xsl:if test ="xdr:sp/xdr:nvSpPr/xdr:cNvPr/a:hlinkClick">
              <xsl:value-of select ="xdr:sp/xdr:nvSpPr/xdr:cNvPr/a:hlinkClick/@r:id"/>
            </xsl:if>
          </xsl:variable>
          <xsl:variable name ="drawingRels">
            <xsl:value-of select ="concat('xl/drawings/_rels/',$Drawing,'.rels')"/>
          </xsl:variable>
          <xsl:variable name ="target">
            <xsl:value-of select ="key('Part', $drawingRels)//node()/node()[@Id=$hlinkId]/@Target"/>
          </xsl:variable>
          <xsl:variable name ="newTarget">
            <xsl:choose>
              <!-- when hyperlink leads to a file in network -->
              <xsl:when test="starts-with($target,'file:///\\')">
                <xsl:value-of select="translate(substring-after($target,'file:///'),'\','/')"/>
              </xsl:when>
              <!--when hyperlink leads to www or mailto -->
              <xsl:when test="contains($target,':')">
                <xsl:value-of select="$target"/>
              </xsl:when>
              <!--when hyperlink leads to a document -->
              <xsl:otherwise>
                <xsl:call-template name="Change20PercentToSpace">
                  <xsl:with-param name="string" select="$target"/>
                </xsl:call-template>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:variable>
          <!--End of Code added by vijayeta, Fix for the bug 1760182, date:23rd Oct '07, variable definition -->
          <xsl:attribute name="draw:style-name">
            <xsl:value-of select="generate-id(.)"/>
          </xsl:attribute>

          <!-- position -->
          <xsl:call-template name="SetPosition">
            <xsl:with-param name="sheet">
              <xsl:value-of select="$sheet"/>
            </xsl:with-param>
            <xsl:with-param name="NameSheet">
              <xsl:value-of select="$NameSheet"/>
            </xsl:with-param>
          </xsl:call-template>
          <!-- Code added by vijayeta, Fix for the bug 1760182, date:23rd Oct '07
		         If the text box has Hyperlink-->
          <xsl:choose >
            <xsl:when test ="$hlinkId!='' and $newTarget!=''">
              <draw:text-box>
                <!--<text:p>
                  <xsl:if test="a:pPr">
                    <xsl:attribute name="text:style-name">
                      <xsl:value-of select="generate-id(.)"/>
                    </xsl:attribute>
                  </xsl:if>
                  <text:a>
                    <xsl:attribute name ="xlink:href">
                      <xsl:value-of select =" $newTarget"/>
                    </xsl:attribute>
                    --><!--RefNo-1--><!--
                    --><!--<xsl:apply-templates select="xdr:sp/xdr:txBody/a:p/a:r/a:t"/>--><!--
                    --><!--<xsl:text>click here for the link</xsl:text>--><!--
                  </text:a>
                </text:p>-->
                <xsl:apply-templates select="xdr:sp/xdr:txBody"/>
              </draw:text-box>
            </xsl:when>
            <xsl:otherwise >
              <draw:text-box>
                <xsl:apply-templates select="xdr:sp/xdr:txBody"/>
              </draw:text-box>
            </xsl:otherwise>
          </xsl:choose>
          <!--End of Code added by vijayeta, Fix for the bug 1760182, date:23rd Oct '07-->
          <!--<draw:text-box>
            <xsl:apply-templates select="xdr:sp/xdr:txBody"/>
          </draw:text-box>-->

        </draw:frame>
      </xsl:when>

    </xsl:choose>
  </xsl:template>


  <!-- Get min. row number with picture -->
  <xsl:template name="GetMinRowWithPicture">
    <xsl:param name="min"/>
    <xsl:param name="PictureRow"/>
    <xsl:param name="AfterRow"/>
    <!--Redundant Code-->
    <!--<xsl:variable name="numRow">
      <xsl:value-of select="substring-before($PictureRow, ';')"/>
    </xsl:variable>-->    
    <xsl:choose>
      <xsl:when test="$AfterRow != ''">

        <xsl:choose>
          <xsl:when test="$PictureRow = ''">
            <xsl:value-of select="$min"/>
          </xsl:when>
          <xsl:when test="$min = ''">
            <xsl:call-template name="GetMinRowWithPicture">
              <xsl:with-param name="min">
                <xsl:if test="$AfterRow &lt; substring-before($PictureRow, ';')">
                  <xsl:value-of select="substring-before($PictureRow, ';')"/>
                </xsl:if>
              </xsl:with-param>
              <xsl:with-param name="PictureRow">
                <xsl:value-of select="substring-after($PictureRow, ';')"/>
              </xsl:with-param>
              <xsl:with-param name="AfterRow">
                <xsl:value-of select="$AfterRow"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:when>
          <xsl:when
            test="$min &gt; substring-before($PictureRow, ';') and $AfterRow &lt; substring-before($PictureRow, ';')">
            <xsl:call-template name="GetMinRowWithPicture">
              <xsl:with-param name="min">
                <xsl:value-of select="substring-before($PictureRow, ';')"/>
              </xsl:with-param>
              <xsl:with-param name="PictureRow">
                <xsl:value-of select="substring-after($PictureRow, ';')"/>
              </xsl:with-param>
              <xsl:with-param name="AfterRow">
                <xsl:value-of select="$AfterRow"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="GetMinRowWithPicture">
              <xsl:with-param name="min">
                <xsl:value-of select="$min"/>
              </xsl:with-param>
              <xsl:with-param name="PictureRow">
                <xsl:value-of select="substring-after($PictureRow, ';')"/>
              </xsl:with-param>
              <xsl:with-param name="AfterRow">
                <xsl:value-of select="$AfterRow"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>

      </xsl:when>
      <xsl:otherwise>

        <xsl:choose>
          <xsl:when test="$PictureRow = ''">
            <xsl:value-of select="$min"/>
          </xsl:when>
          <xsl:when test="$min = ''">
            <xsl:call-template name="GetMinRowWithPicture">
              <xsl:with-param name="min">
                <xsl:value-of select="substring-before($PictureRow, ';')"/>
              </xsl:with-param>
              <xsl:with-param name="PictureRow">
                <xsl:value-of select="substring-after($PictureRow, ';')"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:when>
          <xsl:when test="$min &gt; substring-before($PictureRow, ';')">
            <xsl:call-template name="GetMinRowWithPicture">
              <xsl:with-param name="min">
                <xsl:value-of select="substring-before($PictureRow, ';')"/>
              </xsl:with-param>
              <xsl:with-param name="PictureRow">
                <xsl:value-of select="substring-after($PictureRow, ';')"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="GetMinRowWithPicture">
              <xsl:with-param name="min">
                <xsl:value-of select="$min"/>
              </xsl:with-param>
              <xsl:with-param name="PictureRow">
                <xsl:value-of select="substring-after($PictureRow, ';')"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>

      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>





  <!-- delete cell with picture which are inserted -->

  <xsl:template name="DeleteRow">
    <xsl:param name="PictureRow"/>
    <xsl:param name="GetMinRowWithPicture"/>
    <xsl:choose>
      <xsl:when
        test="contains($PictureRow, concat(';', concat($GetMinRowWithPicture,';'))) and $PictureRow != ''">
        <xsl:call-template name="DeleteRow">
          <xsl:with-param name="GetMinRowWithPicture">
            <xsl:value-of select="$GetMinRowWithPicture"/>
          </xsl:with-param>
          <xsl:with-param name="PictureRow">
            <xsl:value-of
              select="concat(substring-after($PictureRow, concat($GetMinRowWithPicture,';')), substring-before($PictureRow, concat($GetMinRowWithPicture,';')))"
            />
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$PictureRow"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>


  <!-- inserts image href from relationships -->
  <xsl:template name="InsertImageHref">
    <xsl:param name="document"/>
    <xsl:param name="rId"/>
    <xsl:param name="targetName"/>
    <xsl:param name="srcFolder" select="'Pictures'"/>

    <xsl:variable name="id">
      <xsl:choose>
        <xsl:when test="$rId != ''">
          <xsl:value-of select="$rId"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:if test="xdr:pic/xdr:blipFill/a:blip/@r:embed">
            <xsl:value-of select="xdr:pic/xdr:blipFill/a:blip/@r:embed"/>
          </xsl:if>
          <xsl:if test="xdr:pic/xdr:blipFill/a:blip/@r:link">
            <xsl:value-of select="xdr:pic/xdr:blipFill/a:blip/@r:link"/>
          </xsl:if>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:for-each select="key('Part', concat('xl/drawings/_rels/',$document))/rels:Relationships/rels:Relationship[@Id=$id]">

      <xsl:variable name="targetmode">
        <xsl:value-of select="@TargetMode"/>
      </xsl:variable>
      <xsl:variable name="pzipsource">
        <xsl:value-of select="@Target"/>
      </xsl:variable>
      <xsl:variable name="pziptarget">
        <xsl:choose>
          <xsl:when test="$targetName != ''">
            <xsl:value-of select="$targetName"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="substring-after(translate(substring-after($pzipsource,'/'),'\' ,'/'),'/')"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <xsl:attribute name="xlink:href">
        <xsl:choose>
          <xsl:when test="$targetmode='External'">
            <xsl:value-of select="$pziptarget"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="concat($srcFolder,'/', $pziptarget)"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
    </xsl:for-each>
  </xsl:template>

  <!-- Set Position -->
  <xsl:template name="SetPosition">
    <xsl:param name="sheet"/>
    <xsl:param name="NameSheet"/>
		<!--SP2,Scenario:ods_SP2_xlsx_2.5_ods,ChartLost because SP2 created xlsx has oneCellAnchor in Place of twoCellAnchor
			If condition to test if'xdr:to' prsent(twoCellAnchor), which is absent in oneCellAnchor,also take care of xdr:from and xdr:to along with svg:height and svg:width-->
      <xsl:variable name="ColEnd">
			<xsl:choose>
				<xsl:when test="xdr:pic/xdr:spPr/a:xfrm/@flipV = 1 or xdr:to/xdr:col">
        <xsl:call-template name="NumbersToChars">
          <xsl:with-param name="num">
            <xsl:choose>
              <xsl:when test="xdr:pic/xdr:spPr/a:xfrm/@flipV = 1">
                <xsl:value-of select="xdr:to/xdr:col + (xdr:to/xdr:col - xdr:from/xdr:col) - 1"/>
              </xsl:when>
								<xsl:when test="xdr:to/xdr:col">
                <xsl:value-of select="xdr:to/xdr:col"/>
								</xsl:when>
            </xsl:choose>
          </xsl:with-param>
        </xsl:call-template>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="''"/>
				</xsl:otherwise>
			</xsl:choose>
      </xsl:variable>
      <xsl:variable name="RowEnd">
        <xsl:choose>
				<xsl:when test="xdr:pic/xdr:spPr/a:xfrm/@flipV = 1 or xdr:to/xdr:col">
					<xsl:choose>
          <xsl:when test="xdr:pic/xdr:spPr/a:xfrm/@flipV = 1">
            <xsl:value-of select="xdr:to/xdr:row + (xdr:to/xdr:row - xdr:from/xdr:row) + 1"/>
          </xsl:when>
						<xsl:when test="xdr:to/xdr:col">
            <xsl:value-of select="xdr:to/xdr:row + 1"/>
						</xsl:when>
					</xsl:choose>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="''"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <xsl:variable name="apos">
        <xsl:text>&apos;</xsl:text>
      </xsl:variable>
      <xsl:variable name="invalidChars">
        <xsl:text>&apos;!,.+$-()</xsl:text>
      </xsl:variable>
      <xsl:variable name="checkedName">
        <xsl:for-each
          select="key('Part', 'xl/workbook.xml')/e:workbook/e:sheets/e:sheet[@name = translate($NameSheet,$apos,'')]">
          <xsl:call-template name="CheckSheetName">
            <xsl:with-param name="sheetNumber">
              <xsl:for-each
                select="key('Part', 'xl/workbook.xml')/e:workbook/e:sheets/e:sheet[@name = translate($NameSheet,$apos,'')]">
                <xsl:value-of select="count(preceding-sibling::e:sheet) + 1"/>
              </xsl:for-each>
            </xsl:with-param>
            <xsl:with-param name="name">
              <xsl:value-of select="translate($NameSheet,$invalidChars,'')"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:for-each>
      </xsl:variable>
		<xsl:if test="$RowEnd!='' and $ColEnd!=''">
			<xsl:attribute name="table:end-cell-address">
      <xsl:value-of select="concat($apos,$checkedName,$apos, '.', $ColEnd, $RowEnd)"/>
    </xsl:attribute>
		</xsl:if>
    <xsl:attribute name="svg:x">
      <xsl:choose>
        <xsl:when test="xdr:pic/xdr:spPr/a:xfrm/@flipV = 1">
          <xsl:call-template name="ConvertEmu">
            <xsl:with-param name="length" select="xdr:to/xdr:colOff"/>
            <xsl:with-param name="unit">cm</xsl:with-param>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:call-template name="ConvertEmu">
            <xsl:with-param name="length" select="xdr:from/xdr:colOff"/>
            <xsl:with-param name="unit">cm</xsl:with-param>
          </xsl:call-template>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:attribute>
    <xsl:attribute name="svg:y">
      <xsl:call-template name="ConvertEmu">
        <xsl:with-param name="length" select="xdr:from/xdr:rowOff"/>
        <xsl:with-param name="unit">cm</xsl:with-param>
      </xsl:call-template>
    </xsl:attribute>
		<xsl:if test="$RowEnd!='' and $ColEnd!=''">
    <xsl:attribute name="table:end-x">
      <xsl:choose>
        <xsl:when test="xdr:pic/xdr:spPr/a:xfrm/@flipV = 1">
          <xsl:call-template name="ConvertEmu">
            <xsl:with-param name="length" select="xdr:from/xdr:colOff"/>
            <xsl:with-param name="unit">cm</xsl:with-param>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:call-template name="ConvertEmu">
            <xsl:with-param name="length" select="xdr:to/xdr:colOff"/>
            <xsl:with-param name="unit">cm</xsl:with-param>
          </xsl:call-template>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:attribute>
		</xsl:if>
		<xsl:if test="$RowEnd!='' and $ColEnd!=''">
    <xsl:attribute name="table:end-y">
      <xsl:call-template name="ConvertEmu">
        <xsl:with-param name="length" select="xdr:to/xdr:rowOff"/>
        <xsl:with-param name="unit">cm</xsl:with-param>
      </xsl:call-template>
    </xsl:attribute>
		</xsl:if>
    <!-- Code added by Sateesh, fix for the bug 1840212
         Date:29th Jan '08-->
		<xsl:choose>
			<xsl:when test="xdr:pic">
    <xsl:attribute name="svg:width">
      <xsl:call-template name="ConvertEmu">
        <xsl:with-param name="length" select="xdr:pic/xdr:spPr/a:xfrm/a:ext/@cx"/>
        <xsl:with-param name="unit">cm</xsl:with-param>
      </xsl:call-template>
    </xsl:attribute>
    <xsl:attribute name="svg:height">
      <xsl:call-template name="ConvertEmu">
        <xsl:with-param name="length" select="xdr:pic/xdr:spPr/a:xfrm/a:ext/@cy"/>
        <xsl:with-param name="unit">cm</xsl:with-param>
      </xsl:call-template>
    </xsl:attribute>
			</xsl:when>
			<xsl:otherwise>
				<xsl:choose>
					<xsl:when test="self::node()[name()='xdr:twoCellAnchor']">
						<xsl:variable name="startrowno" select="xdr:from/xdr:row">
						</xsl:variable>
						<xsl:variable name="startrowoffset">
							<xsl:value-of select="xdr:from/xdr:rowOff"/>
						</xsl:variable>
						<xsl:variable name="endrowno" select="xdr:to/xdr:row">
						</xsl:variable>
						<xsl:variable name="endrowoffset">
							<xsl:value-of select="xdr:to/xdr:rowOff"/>
						</xsl:variable>
						<xsl:variable name="startrowoffsetincm">
							<xsl:call-template name="ConvertEmu">
								<xsl:with-param name="length" select="$startrowoffset"/>
								<xsl:with-param name="unit" select="'cm'"/>
							</xsl:call-template>
						</xsl:variable>
						<xsl:variable name="endrowoffsetincm">
							<xsl:call-template name="ConvertEmu">
								<xsl:with-param name="length" select="$endrowoffset"/>
								<xsl:with-param name="unit" select="'cm'"/>
							</xsl:call-template>
						</xsl:variable>
						<xsl:variable name="startcolno" select="xdr:from/xdr:col">
						</xsl:variable>
						<xsl:variable name="startcoloffset">
							<xsl:call-template name="ConvertEmu">
								<xsl:with-param name="length" select="xdr:from/xdr:colOff"/>
								<xsl:with-param name="unit" select="'cm'"/>
							</xsl:call-template>
						</xsl:variable>
						<xsl:variable name="endcolno" select="xdr:to/xdr:col">
						</xsl:variable>
						<xsl:variable name="endcoloffset">
							<xsl:call-template name="ConvertEmu">
								<xsl:with-param name="length" select="xdr:to/xdr:colOff"/>
								<xsl:with-param name="unit" select="'cm'"/>
							</xsl:call-template>
						</xsl:variable>
						<xsl:for-each select="key('Part', concat('xl/',$sheet))/e:worksheet/e:drawing">
							<xsl:variable name="defaultrowheight">
								<xsl:value-of select="parent::node()/child::node()[name()='sheetFormatPr']/@defaultRowHeight"/>
							</xsl:variable>
							<xsl:variable name="chartrowheight">
								<xsl:call-template name="CalculateRowHeight">
									<xsl:with-param name="sheet" select="$sheet"/>
									<xsl:with-param name="startrowno" select="$startrowno"/>
									<xsl:with-param name="endrowno" select="$endrowno +1"/>
									<xsl:with-param name="total" select="0"/>
									<xsl:with-param name="defaultrowheight" select="$defaultrowheight"/>
									<xsl:with-param name="countchartrow" select="$startrowno + 1"/>
								</xsl:call-template>
							</xsl:variable>
							<xsl:variable name="chartrowheightincm">
								<xsl:call-template name="ConvertToCentimeters">
									<xsl:with-param name="length">
										<xsl:value-of
										  select="concat($chartrowheight,'pt')"/>
									</xsl:with-param>
								</xsl:call-template>
							</xsl:variable>
							<xsl:variable name="finalchartrowheight">
								<xsl:value-of select="concat((substring-before($chartrowheightincm,'cm') - substring-before($startrowoffsetincm,'cm') + substring-before($endrowoffsetincm,'cm')),'cm')"/>
							</xsl:variable>
							<xsl:attribute name="svg:height">
								<xsl:value-of select="$finalchartrowheight"/>
							</xsl:attribute>
							<!--calculating coloumnwidth-->
							<xsl:variable name="defaultFontSize">
								<xsl:for-each select="key('Part', 'xl/styles.xml')">
									<xsl:choose>
										<xsl:when test="e:styleSheet/e:fonts/e:font">
											<xsl:value-of select="e:styleSheet/e:fonts/e:font[1]/e:sz/@val"/>
										</xsl:when>
										<xsl:otherwise>
											<xsl:text>11</xsl:text>
										</xsl:otherwise>
									</xsl:choose>
								</xsl:for-each>
							</xsl:variable>
							<xsl:variable name="defaultFontStyle">
								<xsl:for-each select="key('Part', 'xl/styles.xml')">
									<xsl:choose>
										<xsl:when test="e:styleSheet/e:fonts/e:font">
											<xsl:value-of select="e:styleSheet/e:fonts/e:font[1]/e:name/@val"/>
										</xsl:when>
										<xsl:otherwise>
											<xsl:text>11</xsl:text>
										</xsl:otherwise>
									</xsl:choose>
								</xsl:for-each>
							</xsl:variable>
							<xsl:variable name="defaultcolWidth">
								<xsl:choose>
									<xsl:when test="parent::node()/child::node()[name()='sheetFormatPr']/@defaultColWidth">
										<xsl:value-of
										  select="parent::node()/child::node()[name()='sheetFormatPr']/@defaultColWidth"/>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select="'null'"/>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:variable>
							<xsl:variable name="colWidPlusdefaultColCount">
								<!--<xsl:if test="parent::node()/child::node()[name()='cols']">-->
								<xsl:call-template name="CalculateColWidth">
									<xsl:with-param name="sheet" select="$sheet"/>
									<xsl:with-param name="startcolno" select="$startcolno"/>
									<xsl:with-param name="endcolno" select="$endcolno +1"/>
									<xsl:with-param name="total" select="0"/>
									<xsl:with-param name="defaultcolWidth" select="$defaultcolWidth"/>
									<xsl:with-param name="countchartcol" select="$startcolno + 1"/>
									<xsl:with-param name="totalDefaultColCount"/>
									<xsl:with-param name="startcoloffset" select="$startcoloffset"/>
									<xsl:with-param name="endcoloffset" select="$endcoloffset"/>
								</xsl:call-template>
								<!--</xsl:if>-->
							</xsl:variable>
							<xsl:variable name="customColWid">
								<xsl:variable name="customColWidPt">
									<xsl:value-of select="substring-before($colWidPlusdefaultColCount,'|')"/>
								</xsl:variable>
								<xsl:choose>
									<xsl:when test ="$customColWidPt = 0">
										<xsl:value-of select="concat($customColWidPt,'cm')"/>
									</xsl:when>
									<xsl:when test ="$customColWidPt = ''">
										<xsl:value-of select="concat(0,'cm')"/>
									</xsl:when>
									<xsl:when test ="$customColWidPt != 0">
										<xsl:call-template name="ConvertToCentimeters">
											<xsl:with-param name="length">
												<xsl:value-of
												  select="concat(substring-before($colWidPlusdefaultColCount,'|'),'pt')"/>
											</xsl:with-param>
										</xsl:call-template>
									</xsl:when>
								</xsl:choose>
							</xsl:variable>
							<xsl:variable name="defaultColCount">
								<xsl:value-of select="substring-after($colWidPlusdefaultColCount,'|')"/>
							</xsl:variable>
							<xsl:attribute name="svg:width">
								<xsl:choose>
									<xsl:when test="$defaultcolWidth != 'null' and translate($customColWid,'cm','')=0 ">
										<xsl:variable name="totalDefWid">
											<xsl:value-of select="number($defaultColCount) * number($defaultcolWidth) "/>
										</xsl:variable>
										<xsl:value-of select="concat('sonataChartWidth:','False|',$defaultFontStyle,'|',$defaultFontSize,'|','0','|',$totalDefWid,'|',substring-before($startcoloffset,'cm'),'|',substring-before($endcoloffset,'cm'))"/>
										<!--<xsl:value-of select="concat(($defaultColCount * substring-before($defaultcolWidth,'cm')),'cm') "/>-->
									</xsl:when>
									<xsl:when test="$defaultcolWidth != 'null' and translate($customColWid,'cm','')!=0">
										<xsl:variable name="totalDefWid">
											<xsl:value-of select="number($defaultColCount) * number($defaultcolWidth)+ substring-before($colWidPlusdefaultColCount,'|')"/>
										</xsl:variable>
										<xsl:value-of select="concat('sonataChartWidth:','False|',$defaultFontStyle,'|',$defaultFontSize,'|','0','|',$totalDefWid,'|',substring-before($startcoloffset,'cm'),'|',substring-before($endcoloffset,'cm'))"/>
									</xsl:when>
									<xsl:otherwise>
										<xsl:if test ="substring-before($colWidPlusdefaultColCount,'|')!=''">
											<xsl:value-of select="concat('sonataChartWidth:','False|',$defaultFontStyle,'|',$defaultFontSize,'|',$defaultColCount,'|',substring-before($colWidPlusdefaultColCount,'|'),'|',substring-before($startcoloffset,'cm'),'|',substring-before($endcoloffset,'cm'))"/>
										</xsl:if>
										<xsl:if test ="substring-before($colWidPlusdefaultColCount,'|')=''">
											<xsl:value-of select="concat('sonataChartWidth:','False|',$defaultFontStyle,'|',$defaultFontSize,'|',$defaultColCount,'|','0','|',substring-before($startcoloffset,'cm'),'|',substring-before($endcoloffset,'cm'))"/>
										</xsl:if>
										<!--<xsl:value-of select="concat('sonataChartWidth:','False|',$defaultFontStyle,'|',$defaultFontSize,'|',$defaultColCount,'|',substring-before($colWidPlusdefaultColCount,'|'))"/>-->
									</xsl:otherwise>
								</xsl:choose>
								<!-- Call Post Processor Here-->
							</xsl:attribute>
						</xsl:for-each>
					</xsl:when>
					<xsl:when test="self::node()[name()='xdr:oneCellAnchor']">
						<xsl:choose>
							<xsl:when test="number(xdr:ext/@cx)=0 and number(xdr:ext/@cy)=0">
								<xsl:attribute name="svg:width">
									<xsl:value-of select="'12.7cm'"/>
								</xsl:attribute>
								<xsl:attribute name="svg:height">
									<xsl:value-of select="'7.62cm'"/>
								</xsl:attribute>
							</xsl:when>
							<xsl:when test="number(xdr:ext/@cx)!=0 and number(xdr:ext/@cy)!=0">
								<xsl:call-template name="InsertAbsoluteSize"/>
							</xsl:when>
						</xsl:choose>
					</xsl:when>
					<xsl:when test="self::node()[name()='xdr:absoluteAnchor']">
						<xsl:call-template name="InsertAbsolutePosition"/>
						<xsl:call-template name="InsertAbsoluteSize"/>
						<xsl:attribute name="xlink:href">
							<xsl:value-of
							  select="concat('./Object ',generate-id(xdr:graphicFrame/a:graphic/a:graphicData/c:chart))"/>
						</xsl:attribute>
					</xsl:when>
				</xsl:choose>
			</xsl:otherwise>
		</xsl:choose>
    <!--End-->
  </xsl:template>
	<!--SP2,Scenario:ods_SP2_xlsx_2.5_ods,End-->
  <!-- Insert Picture Style -->

  <xsl:template name="InsertGraphicProperties">
    <xsl:call-template name="InsertImageFlip">
      <xsl:with-param name="atribute">
        <xsl:text>style</xsl:text>
      </xsl:with-param>
    </xsl:call-template>

    <!--<xsl:call-template name="InsertGraphicBorder"/>-->

    <xsl:call-template name="InsertFill"/>
    <xsl:call-template name="InsertLineColor"/>
    <xsl:call-template name="InsertLineStyle"/>

    <xsl:for-each select="parent::node()/xdr:txBody">
      <xsl:call-template name="InsertTextLayout"/>
    </xsl:for-each>

    <xsl:attribute name="fo:min-height">
      <xsl:variable name="border">
        <xsl:choose>
          <xsl:when test="a:ln/@w">

            <xsl:variable name="width">
              <xsl:call-template name="ConvertEmu3">
                <xsl:with-param name="length">
                  <xsl:value-of select="a:ln/@w"/>
                </xsl:with-param>
                <xsl:with-param name="unit">cm</xsl:with-param>
              </xsl:call-template>
            </xsl:variable>

            <xsl:choose>
              <xsl:when test="substring($width,1,1) = '.' ">
                <xsl:value-of select="concat('0',substring-before($width,'cm'))"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="substring-before($width,'cm')"/>
              </xsl:otherwise>
            </xsl:choose>

          </xsl:when>
          <xsl:otherwise>
            <xsl:text>0</xsl:text>
          </xsl:otherwise>
        </xsl:choose>

      </xsl:variable>
      <xsl:value-of select="concat(a:xfrm/a:ext/@cy div 360000 - $border,'cm')"/>
    </xsl:attribute>

  </xsl:template>

  <xsl:template match="e:sheet" mode="PictureStyle">
    <xsl:param name="number"/>

    <xsl:variable name="Id">
      <xsl:call-template name="GetTarget">
        <xsl:with-param name="id">
          <xsl:value-of select="@r:id"/>
        </xsl:with-param>
        <xsl:with-param name="document">xl/workbook.xml</xsl:with-param>
      </xsl:call-template>
    </xsl:variable>
<!--Redundant-->
    <!--<xsl:variable name="sheetName">
      <xsl:value-of select="@name"/>
    </xsl:variable>-->
    <xsl:for-each select="key('Part', concat('xl/',$Id))">
      <xsl:apply-templates select="e:worksheet/e:drawing" mode="PictureStyle">
        <xsl:with-param name="sheet">
          <xsl:value-of select="$Id"/>
        </xsl:with-param>
      </xsl:apply-templates>
    </xsl:for-each>

    <!-- Insert next Table -->

    <xsl:apply-templates select="following-sibling::e:sheet[1]" mode="PictureStyle">
      <xsl:with-param name="number">
        <xsl:value-of select="$number + 1"/>
      </xsl:with-param>
    </xsl:apply-templates>

  </xsl:template>

  <xsl:template match="e:drawing" mode="PictureStyle">
    <xsl:param name="sheet"/>
    <xsl:variable name="Target">
      <xsl:call-template name="GetTargetPicture">
        <xsl:with-param name="sheet">
          <xsl:value-of select="substring-after($sheet, '/')"/>
        </xsl:with-param>
        <xsl:with-param name="id">
          <xsl:value-of select="@r:id"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:for-each select="key('Part', concat('xl/', substring-after($Target, '/')))">
			<!--SP2,Scenario:ods_SP2_xlsx_2.5_ods,ChartLost because SP2 created xlsx has oneCellAnchor in Place of twoCellAnchor-->
			<!--<xsl:if test="xdr:wsDr/xdr:twoCellAnchor">
        <xsl:apply-templates select="xdr:wsDr/xdr:twoCellAnchor[1]" mode="PictureStyle">
          <xsl:with-param name="sheet" select="$sheet"/>
          <xsl:with-param name="Target" select="$Target"/>
        </xsl:apply-templates>
			</xsl:if>-->
			<xsl:choose>
				<xsl:when test="xdr:wsDr/xdr:twoCellAnchor">
					<xsl:apply-templates select="xdr:wsDr/xdr:twoCellAnchor[1]" mode="PictureStyle">
						<xsl:with-param name="sheet" select="$sheet"/>
						<xsl:with-param name="Target" select="$Target"/>
					</xsl:apply-templates>
				</xsl:when>
				<xsl:when test="xdr:wsDr/xdr:oneCellAnchor">
					<xsl:apply-templates select="xdr:wsDr/xdr:oneCellAnchor[1]" mode="PictureStyle">
						<xsl:with-param name="sheet" select="$sheet"/>
						<xsl:with-param name="Target" select="$Target"/>
					</xsl:apply-templates>
				</xsl:when>
			</xsl:choose>
			<!--SP2,Scenario:ods_SP2_xlsx_2.5_ods,End-->
    </xsl:for-each>

  </xsl:template>

  <xsl:template match="xdr:twoCellAnchor" mode="PictureStyle">
    <xsl:param name="PictureCell"/>
    <xsl:param name="sheet"/>
    <xsl:param name="Target"/>

    <xsl:variable name="PictureColStart">
      <xsl:value-of select="xdr:from/xdr:col"/>
    </xsl:variable>

    <xsl:variable name="PictureRowStart">
      <xsl:value-of select="xdr:from/xdr:row"/>
    </xsl:variable>

    <style:style style:name="{generate-id(.)}" style:family="graphic">
      <style:graphic-properties>

        <!-- insert graphic properties -->
        <xsl:choose>
          <xsl:when test="xdr:pic">
            <xsl:for-each select="xdr:pic/xdr:spPr">
              <xsl:call-template name="InsertGraphicProperties"/>
            </xsl:for-each>
          </xsl:when>
          <xsl:when test="xdr:sp/xdr:spPr">
            <xsl:for-each select="xdr:sp/xdr:spPr">
              <xsl:call-template name="InsertGraphicProperties"/>
            </xsl:for-each>
          </xsl:when>
        </xsl:choose>

        <!--Cropping-->
        <xsl:variable name="relationId">
          <xsl:value-of select="xdr:pic/xdr:blipFill/a:blip/@r:embed"/>
        </xsl:variable>

        <xsl:variable name="Drawing">
          <xsl:value-of select="substring-after(substring-after($Target, '/'), '/')"/>
        </xsl:variable>
        <xsl:variable name="document">
          <xsl:value-of select="concat($Drawing, '.rels')"/>
        </xsl:variable>

        <xsl:variable name="pzipsource">
          <xsl:for-each select="key('Part', concat('xl/drawings/_rels/',$document))//node()[name() = 'Relationship']">
            <xsl:if test="./@Id=$relationId">
              <xsl:value-of select="./@Target"/>
            </xsl:if>
          </xsl:for-each>
        </xsl:variable>

        <xsl:if test="xdr:pic/xdr:blipFill/a:srcRect/@l or xdr:pic/xdr:blipFill/a:srcRect/@r or xdr:pic/xdr:blipFill/a:srcRect/@t or xdr:pic/xdr:blipFill/a:srcRect/@b">
          <xsl:variable name="left">
            <xsl:if test="xdr:pic/xdr:blipFill/a:srcRect/@l">
              <xsl:value-of select="xdr:pic/xdr:blipFill/a:srcRect/@l"/>
            </xsl:if>
            <xsl:if test="not(xdr:pic/xdr:blipFill/a:srcRect/@l)">
              <xsl:value-of select="0"/>
            </xsl:if>
          </xsl:variable>
          <xsl:variable name="right">
            <xsl:if test="xdr:pic/xdr:blipFill/a:srcRect/@r">
              <xsl:value-of select="xdr:pic/xdr:blipFill/a:srcRect/@r"/>
            </xsl:if>
            <xsl:if test="not(xdr:pic/xdr:blipFill/a:srcRect/@r)">
              <xsl:value-of select="0"/>
            </xsl:if>
          </xsl:variable>
          <xsl:variable name="top">
            <xsl:if test="xdr:pic/xdr:blipFill/a:srcRect/@t">
              <xsl:value-of select="xdr:pic/xdr:blipFill/a:srcRect/@t"/>
            </xsl:if>
            <xsl:if test="not(xdr:pic/xdr:blipFill/a:srcRect/@t)">
              <xsl:value-of select="0"/>
            </xsl:if>
          </xsl:variable>
          <xsl:variable name="bottom">
            <xsl:if test="xdr:pic/xdr:blipFill/a:srcRect/@b">
              <xsl:value-of select="xdr:pic/xdr:blipFill/a:srcRect/@b"/>
            </xsl:if>
            <xsl:if test="not(xdr:pic/xdr:blipFill/a:srcRect/@b)">
              <xsl:value-of select="0"/>
            </xsl:if>
          </xsl:variable>
          <xsl:attribute name ="fo:clip">
            <xsl:variable name="temp">
              <xsl:value-of select="concat('image-props:',concat('xl/',substring-after($pzipsource, '/')),':',$left,':',$right,':',$top,':',$bottom)"/>
            </xsl:variable>
            <xsl:value-of select="$temp"/>
          </xsl:attribute>
        </xsl:if>
        <!--End-->
      </style:graphic-properties>
    </style:style>

    <!--xsl:call-template name="InsertGraphicProperties"/-->

    <xsl:apply-templates select="following-sibling::xdr:twoCellAnchor[1]" mode="PictureStyle">
      <xsl:with-param name="PictureCell">
        <xsl:value-of
          select="concat(concat(concat(concat($PictureCell, $PictureRowStart), ':'), $PictureColStart), ';')"
        />
      </xsl:with-param>
      <xsl:with-param name="sheet" select="$sheet"/>
      <xsl:with-param name="Target" select="$Target"/>
    </xsl:apply-templates>

  </xsl:template>
	<!--SP2,Scenario:ods_SP2_xlsx_2.5_ods,ChartLost because SP2 created xlsx has oneCellAnchor in Place of twoCellAnchor-->
	<xsl:template match="xdr:oneCellAnchor" mode="PictureStyle">
		<xsl:param name="PictureCell"/>
		<xsl:param name="sheet"/>
		<xsl:param name="Target"/>
		<xsl:variable name="PictureColStart">
			<xsl:value-of select="xdr:from/xdr:col"/>
		</xsl:variable>
		<xsl:variable name="PictureRowStart">
			<xsl:value-of select="xdr:from/xdr:row"/>
		</xsl:variable>
		<style:style style:name="{generate-id(.)}" style:family="graphic">
			<style:graphic-properties>
				<!-- insert graphic properties -->
				<xsl:choose>
					<xsl:when test="xdr:pic">
						<xsl:for-each select="xdr:pic/xdr:spPr">
							<xsl:call-template name="InsertGraphicProperties"/>
						</xsl:for-each>
					</xsl:when>
					<xsl:when test="xdr:sp/xdr:spPr">
						<xsl:for-each select="xdr:sp/xdr:spPr">
							<xsl:call-template name="InsertGraphicProperties"/>
						</xsl:for-each>
					</xsl:when>
				</xsl:choose>
				<!--Cropping-->
				<xsl:variable name="relationId">
					<xsl:value-of select="xdr:pic/xdr:blipFill/a:blip/@r:embed"/>
				</xsl:variable>
				<xsl:variable name="Drawing">
					<xsl:value-of select="substring-after(substring-after($Target, '/'), '/')"/>
				</xsl:variable>
				<xsl:variable name="document">
					<xsl:value-of select="concat($Drawing, '.rels')"/>
				</xsl:variable>
				<xsl:variable name="pzipsource">
					<xsl:for-each select="key('Part', concat('xl/drawings/_rels/',$document))//node()[name() = 'Relationship']">
						<xsl:if test="./@Id=$relationId">
							<xsl:value-of select="./@Target"/>
						</xsl:if>
					</xsl:for-each>
				</xsl:variable>
				<xsl:if test="xdr:pic/xdr:blipFill/a:srcRect/@l or xdr:pic/xdr:blipFill/a:srcRect/@r or xdr:pic/xdr:blipFill/a:srcRect/@t or xdr:pic/xdr:blipFill/a:srcRect/@b">
					<xsl:variable name="left">
						<xsl:if test="xdr:pic/xdr:blipFill/a:srcRect/@l">
							<xsl:value-of select="xdr:pic/xdr:blipFill/a:srcRect/@l"/>
						</xsl:if>
						<xsl:if test="not(xdr:pic/xdr:blipFill/a:srcRect/@l)">
							<xsl:value-of select="0"/>
						</xsl:if>
					</xsl:variable>
					<xsl:variable name="right">
						<xsl:if test="xdr:pic/xdr:blipFill/a:srcRect/@r">
							<xsl:value-of select="xdr:pic/xdr:blipFill/a:srcRect/@r"/>
						</xsl:if>
						<xsl:if test="not(xdr:pic/xdr:blipFill/a:srcRect/@r)">
							<xsl:value-of select="0"/>
						</xsl:if>
					</xsl:variable>
					<xsl:variable name="top">
						<xsl:if test="xdr:pic/xdr:blipFill/a:srcRect/@t">
							<xsl:value-of select="xdr:pic/xdr:blipFill/a:srcRect/@t"/>
						</xsl:if>
						<xsl:if test="not(xdr:pic/xdr:blipFill/a:srcRect/@t)">
							<xsl:value-of select="0"/>
						</xsl:if>
					</xsl:variable>
					<xsl:variable name="bottom">
						<xsl:if test="xdr:pic/xdr:blipFill/a:srcRect/@b">
							<xsl:value-of select="xdr:pic/xdr:blipFill/a:srcRect/@b"/>
						</xsl:if>
						<xsl:if test="not(xdr:pic/xdr:blipFill/a:srcRect/@b)">
							<xsl:value-of select="0"/>
						</xsl:if>
					</xsl:variable>
					<xsl:attribute name ="fo:clip">
						<xsl:variable name="temp">
							<xsl:value-of select="concat('image-props:',concat('xl/',substring-after($pzipsource, '/')),':',$left,':',$right,':',$top,':',$bottom)"/>
						</xsl:variable>
						<xsl:value-of select="$temp"/>
					</xsl:attribute>
				</xsl:if>
				<!--End-->
			</style:graphic-properties>
		</style:style>
		<!--xsl:call-template name="InsertGraphicProperties"/-->
		<xsl:apply-templates select="following-sibling::xdr:oneCellAnchor[1]" mode="PictureStyle">
			<xsl:with-param name="PictureCell">
				<xsl:value-of
				  select="concat(concat(concat(concat($PictureCell, $PictureRowStart), ':'), $PictureColStart), ';')"/>
			</xsl:with-param>
			<xsl:with-param name="sheet" select="$sheet"/>
			<xsl:with-param name="Target" select="$Target"/>
		</xsl:apply-templates>
	</xsl:template>
	<!--SP2,Scenario:ods_SP2_xlsx_2.5_ods,End-->

  <!-- To do insert border -->
  <xsl:template name="InsertGraphicBorder">
    <xsl:if test="a:ln[not(a:noFill)]">
      <xsl:variable name="width">
        <xsl:call-template name="ConvertEmu3">
          <xsl:with-param name="length">
            <xsl:value-of select="a:ln/@w"/>
          </xsl:with-param>
          <xsl:with-param name="unit">cm</xsl:with-param>
        </xsl:call-template>
      </xsl:variable>

      <xsl:variable name="type">
        <xsl:choose>
          <xsl:when test="a:ln/a:prstDash/@val = 'solid'">
            <xsl:text>solid</xsl:text>
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>solid</xsl:text>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>

      <xsl:variable name="color">
        <xsl:choose>
          <xsl:when test="a:ln/a:solidFill/a:srgbClr/@val != ''">
            <xsl:value-of select="a:ln/a:solidFill/a:srgbClr/@val"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>000000</xsl:text>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>

      <xsl:attribute name="draw:stroke">
        <xsl:value-of select="$type"/>
      </xsl:attribute>

      <xsl:attribute name="draw:stroke-dash">
        <xsl:text>Line_20_with_20_Fine_20_Dots</xsl:text>
      </xsl:attribute>

      <xsl:attribute name="svg:stroke-width">
        <xsl:value-of select="$width"/>
      </xsl:attribute>

      <xsl:attribute name="svg:stroke-color">
        <xsl:value-of select="concat('#', $color)"/>
      </xsl:attribute>

    </xsl:if>
  </xsl:template>

  <!-- Insert Flip properties -->
  <xsl:template name="InsertImageFlip">
    <xsl:param name="atribute"/>
    <!--  picture flip (vertical, horizontal)-->
    <xsl:if test="a:xfrm/attribute::node()">
      <xsl:choose>
        <!-- TO DO Vertical  -->
        <xsl:when test="a:xfrm/@flipV = '1' and $atribute != 'style'">
          <xsl:attribute name="draw:transform">
            <xsl:text>rotate (3.1415926535892) translate (2.064cm 0.425cm)</xsl:text>
          </xsl:attribute>
        </xsl:when>
        <!-- horizontal -->
        <xsl:when test="a:xfrm/@flipH = '1'  and $atribute = 'style'">
          <xsl:attribute name="style:mirror">
            <xsl:text>horizontal</xsl:text>
          </xsl:attribute>
        </xsl:when>
      </xsl:choose>
    </xsl:if>
  </xsl:template>

  <!-- Insert cell with picture -->
  <xsl:template name="InsertPictureInThisCell">
    <xsl:param name="rowNum"/>
    <xsl:param name="collNum"/>
    <xsl:param name="sheet"/>
    <xsl:param name="NameSheet"/>
    <xsl:param name="Target"/>

    <xsl:for-each select="key('Part', concat('xl/', substring-after($Target, '/')))">
			<!--SP2,Scenario:ods_SP2_xlsx_2.5_ods,ChartLost because SP2 created xlsx has oneCellAnchor in Place of twoCellAnchor-->
			<!--<xsl:if test="xdr:wsDr/xdr:twoCellAnchor">
        <xsl:for-each
          select="key('Part', concat(concat('xl/worksheets/_rels/', substring-after($sheet, '/')), '.rels'))//node()[name()='Relationship']">
          <xsl:call-template name="CopyPictures">
            <xsl:with-param name="document">
              <xsl:value-of
                select="concat(substring-after(substring-after($Target, '/'), '/'), '.rels')"/>
            </xsl:with-param>
            <xsl:with-param name="targetName">
              <xsl:text>Pictures</xsl:text>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:for-each>
        <xsl:for-each select="xdr:wsDr/xdr:twoCellAnchor">
          <xsl:choose>
            <xsl:when test="xdr:pic/xdr:spPr/a:xfrm/@flipV = 1">
              <xsl:if test="(xdr:to/xdr:col + 1) = $collNum and (xdr:to/xdr:row + 1) = $rowNum">

                <xsl:call-template name="InsertPicture">
                  <xsl:with-param name="NameSheet">
                    <xsl:value-of select="$NameSheet"/>
                  </xsl:with-param>
                  <xsl:with-param name="sheet">
                    <xsl:value-of select="$sheet"/>
                  </xsl:with-param>
                  <xsl:with-param name="Drawing">
                    <xsl:value-of select="substring-after(substring-after($Target, '/'), '/')"/>
                  </xsl:with-param>
                </xsl:call-template>
              </xsl:if>
            </xsl:when>
            <xsl:otherwise>
              <xsl:if test="(xdr:from/xdr:col + 1) = $collNum and (xdr:from/xdr:row + 1) = $rowNum">
                <xsl:call-template name="InsertPicture">
                  <xsl:with-param name="NameSheet">
                    <xsl:value-of select="$NameSheet"/>
                  </xsl:with-param>
                  <xsl:with-param name="sheet">
                    <xsl:value-of select="$sheet"/>
                  </xsl:with-param>
                  <xsl:with-param name="Drawing">
                    <xsl:value-of select="substring-after(substring-after($Target, '/'), '/')"/>
                  </xsl:with-param>
                </xsl:call-template>
              </xsl:if>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:for-each>
			</xsl:if>-->
			<xsl:choose>
				<xsl:when test="xdr:wsDr/xdr:twoCellAnchor">
					<xsl:for-each
					  select="key('Part', concat(concat('xl/worksheets/_rels/', substring-after($sheet, '/')), '.rels'))//node()[name()='Relationship']">
						<xsl:call-template name="CopyPictures">
							<xsl:with-param name="document">
								<xsl:value-of
								  select="concat(substring-after(substring-after($Target, '/'), '/'), '.rels')"/>
							</xsl:with-param>
							<xsl:with-param name="targetName">
								<xsl:text>Pictures</xsl:text>
							</xsl:with-param>
						</xsl:call-template>
					</xsl:for-each>
					<xsl:for-each select="xdr:wsDr/xdr:twoCellAnchor">
						<xsl:choose>
							<xsl:when test="xdr:pic/xdr:spPr/a:xfrm/@flipV = 1">
								<xsl:if test="(xdr:to/xdr:col + 1) = $collNum and (xdr:to/xdr:row + 1) = $rowNum">
									<xsl:call-template name="InsertPicture">
										<xsl:with-param name="NameSheet">
											<xsl:value-of select="$NameSheet"/>
										</xsl:with-param>
										<xsl:with-param name="sheet">
											<xsl:value-of select="$sheet"/>
										</xsl:with-param>
										<xsl:with-param name="Drawing">
											<xsl:value-of select="substring-after(substring-after($Target, '/'), '/')"/>
										</xsl:with-param>
									</xsl:call-template>
								</xsl:if>
							</xsl:when>
							<xsl:otherwise>
								<xsl:if test="(xdr:from/xdr:col + 1) = $collNum and (xdr:from/xdr:row + 1) = $rowNum">
									<xsl:call-template name="InsertPicture">
										<xsl:with-param name="NameSheet">
											<xsl:value-of select="$NameSheet"/>
										</xsl:with-param>
										<xsl:with-param name="sheet">
											<xsl:value-of select="$sheet"/>
										</xsl:with-param>
										<xsl:with-param name="Drawing">
											<xsl:value-of select="substring-after(substring-after($Target, '/'), '/')"/>
										</xsl:with-param>
									</xsl:call-template>
      </xsl:if>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:for-each>
				</xsl:when>
				<xsl:when test="xdr:wsDr/xdr:oneCellAnchor">
					<xsl:for-each
					  select="key('Part', concat(concat('xl/worksheets/_rels/', substring-after($sheet, '/')), '.rels'))//node()[name()='Relationship']">
						<xsl:call-template name="CopyPictures">
							<xsl:with-param name="document">
								<xsl:value-of
								  select="concat(substring-after(substring-after($Target, '/'), '/'), '.rels')"/>
							</xsl:with-param>
							<xsl:with-param name="targetName">
								<xsl:text>Pictures</xsl:text>
							</xsl:with-param>
						</xsl:call-template>
    </xsl:for-each>
					<xsl:for-each select="xdr:wsDr/xdr:oneCellAnchor">
						<xsl:choose>
							<xsl:when test="xdr:pic/xdr:spPr/a:xfrm/@flipV = 1">
								<xsl:if test="(xdr:from/xdr:row + 1) = $rowNum">
									<xsl:call-template name="InsertPicture">
										<xsl:with-param name="NameSheet">
											<xsl:value-of select="$NameSheet"/>
										</xsl:with-param>
										<xsl:with-param name="sheet">
											<xsl:value-of select="$sheet"/>
										</xsl:with-param>
										<xsl:with-param name="Drawing">
											<xsl:value-of select="substring-after(substring-after($Target, '/'), '/')"/>
										</xsl:with-param>
									</xsl:call-template>
								</xsl:if>
							</xsl:when>
							<xsl:otherwise>
								<xsl:if test="(xdr:from/xdr:col + 1) = $collNum and (xdr:from/xdr:row + 1) = $rowNum">
									<xsl:call-template name="InsertPicture">
										<xsl:with-param name="NameSheet">
											<xsl:value-of select="$NameSheet"/>
										</xsl:with-param>
										<xsl:with-param name="sheet">
											<xsl:value-of select="$sheet"/>
										</xsl:with-param>
										<xsl:with-param name="Drawing">
											<xsl:value-of select="substring-after(substring-after($Target, '/'), '/')"/>
										</xsl:with-param>
									</xsl:call-template>
								</xsl:if>
							</xsl:otherwise>
						</xsl:choose>
    </xsl:for-each>
				</xsl:when>
				<xsl:when test="xdr:wsDr/xdr:absoluteAnchor">

				</xsl:when>
			</xsl:choose>
			<!--SP2,Scenario:ods_SP2_xlsx_2.5_ods,End-->
		</xsl:for-each>
  </xsl:template>

  <xsl:template name="InsertChartsheet">
    <xsl:param name="number"/>
    <xsl:param name="sheet"/>

    <table:table>

      <!-- Insert Table (Sheet) Name -->

      <xsl:variable name="checkedName">
        <xsl:call-template name="CheckSheetName">
          <xsl:with-param name="sheetNumber">
            <xsl:value-of select="$number"/>
          </xsl:with-param>
          <xsl:with-param name="name">
            <xsl:value-of select="translate(@name,'!-$#:(),.+','')"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:variable>

      <xsl:attribute name="table:name">
        <!--        <xsl:value-of select="@name"/>-->
        <xsl:value-of select="$checkedName"/>
      </xsl:attribute>

      <!-- Insert Table Style Name (style:table-properties) -->

      <xsl:attribute name="table:style-name">
        <xsl:value-of select="generate-id()"/>
      </xsl:attribute>

      <!-- drawing.xml file path -->
      <xsl:variable name="Target">
        <xsl:for-each select="key('Part', concat('xl/',$sheet))/e:chartsheet/e:drawing">
          <xsl:variable name="id">
            <xsl:value-of select="@r:id"/>
          </xsl:variable>
          <xsl:if
            test="key('Part', concat(concat('xl/chartsheets/_rels/', substring-after($sheet,'/')), '.rels'))//node()[name()='Relationship']">
            <xsl:for-each
              select="key('Part', concat(concat('xl/chartsheets/_rels/', substring-after($sheet,'/')), '.rels'))//node()[name()='Relationship']">
              <xsl:if test="./@Id=$id">
                <xsl:value-of select="@Target"/>
              </xsl:if>
            </xsl:for-each>
          </xsl:if>
        </xsl:for-each>
      </xsl:variable>

      <office:forms form:automatic-focus="false" form:apply-design-mode="false"/>
      <table:shapes>

        <xsl:for-each select="key('Part', concat('xl/', substring-after($Target, '/')))">
          <xsl:for-each select="xdr:wsDr/xdr:absoluteAnchor">

            <draw:frame draw:z-index="0" svg:width="7.999cm" svg:height="6.999cm" svg:x="0cm"
              svg:y="0cm">

              <xsl:call-template name="InsertAbsolutePosition"/>
              <xsl:call-template name="InsertAbsoluteSize"/>

              <draw:object xlink:type="simple" xlink:show="embed" xlink:actuate="onLoad">
                <xsl:attribute name="xlink:href">
                  <xsl:value-of
                    select="concat('./Object ',generate-id(xdr:graphicFrame/a:graphic/a:graphicData/c:chart))"
                  />
                </xsl:attribute>
              </draw:object>

            </draw:frame>
          </xsl:for-each>
        </xsl:for-each>

      </table:shapes>
      <table:table-column
        table:style-name="{concat('co', key('Part', 'xl/worksheets/sheet1.xml')/e:worksheet/@oox:part)}"/>
      <table:table-row
        table:style-name="{concat('ro', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)}">
        <table:table-cell/>
      </table:table-row>
    </table:table>
  </xsl:template>

  <xsl:template name="InsertAbsolutePosition">

    <xsl:if test="xdr:pos">
      <xsl:attribute name="svg:x">
        <xsl:call-template name="ConvertEmu">
          <xsl:with-param name="length" select="xdr:pos/@x"/>
          <xsl:with-param name="unit">cm</xsl:with-param>
        </xsl:call-template>
      </xsl:attribute>
      <xsl:attribute name="svg:y">
        <xsl:call-template name="ConvertEmu">
          <xsl:with-param name="length" select="xdr:pos/@y"/>
          <xsl:with-param name="unit">cm</xsl:with-param>
        </xsl:call-template>
      </xsl:attribute>
    </xsl:if>
  </xsl:template>

  <xsl:template name="InsertAbsoluteSize">

    <xsl:if test="xdr:ext">
      <xsl:attribute name="svg:width">
        <xsl:call-template name="ConvertEmu">
          <xsl:with-param name="length" select="xdr:ext/@cx"/>
          <xsl:with-param name="unit">cm</xsl:with-param>
        </xsl:call-template>
      </xsl:attribute>
      <xsl:attribute name="svg:height">
        <xsl:call-template name="ConvertEmu">
          <xsl:with-param name="length" select="xdr:ext/@cy"/>
          <xsl:with-param name="unit">cm</xsl:with-param>
        </xsl:call-template>
      </xsl:attribute>
    </xsl:if>
  </xsl:template>

  <xsl:template match="a:p">
    <text:p>
      <xsl:if test="a:pPr">
        <xsl:attribute name="text:style-name">
          <xsl:value-of select="generate-id(.)"/>
        </xsl:attribute>
      </xsl:if>
      <xsl:apply-templates/>
    </text:p>
  </xsl:template>

  <xsl:template match="a:r">
    <text:span>
      <xsl:if test="a:rPr">
        <xsl:attribute name="text:style-name">
          <xsl:value-of select="generate-id(.)"/>
        </xsl:attribute>
      </xsl:if>

      <xsl:apply-templates/>
    </text:span>
  </xsl:template>

  <xsl:template match="a:t">
    <xsl:choose>
      <!--check whether string contains  whitespace sequence-->
      <xsl:when test="not(contains(., '  '))">
        <xsl:choose>
          <!-- single space before case -->
          <xsl:when test="substring(text(),1,1) = ' ' ">
            <text:s/>
            <xsl:value-of select="substring-after(text(),' ')"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="."/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:otherwise>
        <!--converts whitespaces sequence to text:s -->
        <!-- inside "if" when text starts with a single space -->
        <xsl:if test="substring(text(),1,1) = ' ' and substring(text(),2,1) != ' ' ">
          <text:s/>
        </xsl:if>
        <xsl:call-template name="InsertWhiteSpaces"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="InsertFill">

    <xsl:choose>
      <!-- No fill -->
      <xsl:when test="a:noFill">

        <xsl:attribute name="draw:fill">
          <xsl:value-of select="'none'"/>
        </xsl:attribute>

      </xsl:when>

      <!-- Solid fill-->
      <xsl:when test="a:solidFill">
        <xsl:attribute name="draw:fill">
          <xsl:value-of select="'solid'"/>
        </xsl:attribute>

        <!-- Standard color-->
        <xsl:if test="a:solidFill/a:srgbClr/@val">
          <xsl:attribute name="draw:fill-color">
            <xsl:value-of select="concat('#',a:solidFill/a:srgbClr/@val)"/>
          </xsl:attribute>

          <!-- Transparency percentage-->
          <xsl:if test="a:solidFill/a:srgbClr/a:alpha/@val">
            <xsl:variable name="alpha">
              <xsl:value-of select="a:solidFill/a:srgbClr/a:alpha/@val"/>
            </xsl:variable>

            <xsl:if test="($alpha != '') or ($alpha != 0)">
              <xsl:attribute name="draw:opacity">
                <xsl:value-of select="concat(($alpha div 1000), '%')"/>
              </xsl:attribute>
            </xsl:if>

          </xsl:if>
        </xsl:if>
        <!--changed by sonata for bug no:2163376-->
        <xsl:if test="a:solidFill/a:sysClr/@lastClr">
          <xsl:attribute name="draw:fill-color">
            <!--<xsl:value-of select="concat('#',a:solidFill/a:sysClr/@lastClr)"/>-->
            <xsl:call-template name="tmpgetColorCode">
              <xsl:with-param name="color">
                <xsl:value-of select="a:solidFill/a:sysClr/@lastClr"/>
              </xsl:with-param>
              <xsl:with-param name="lumMod">
                <xsl:value-of select="a:solidFill/a:sysClr/a:lumMod/@val"/>
              </xsl:with-param>
              <xsl:with-param name="lumOff">
                <xsl:value-of select="a:solidFill/a:sysClr/a:lumOff/@val"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:attribute>
        </xsl:if>
        <!--end-->
        <!--Theme color-->
        <xsl:if test="a:solidFill/a:schemeClr/@val">

          <xsl:attribute name="draw:fill-color">
            <xsl:call-template name="getColorCode">
              <xsl:with-param name="color">
                <xsl:value-of select="a:solidFill/a:schemeClr/@val"/>
              </xsl:with-param>
              <xsl:with-param name="lumMod">
                <xsl:value-of select="a:solidFill/a:schemeClr/a:lumMod/@val"/>
              </xsl:with-param>
              <xsl:with-param name="lumOff">
                <xsl:value-of select="a:solidFill/a:schemeClr/a:lumOff/@val"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:attribute>

          <!-- Transparency percentage-->
          <xsl:if test="a:solidFill/a:schemeClr/a:alpha/@val">
            <xsl:variable name="alpha">
              <xsl:value-of select="a:solidFill/a:schemeClr/a:alpha/@val"/>
            </xsl:variable>

            <xsl:if test="($alpha != '') or ($alpha != 0)">
              <xsl:attribute name="draw:opacity">
                <xsl:value-of select="concat(($alpha div 1000), '%')"/>
              </xsl:attribute>
            </xsl:if>

          </xsl:if>
        </xsl:if>
      </xsl:when>
      <!--added by Sonata to support Pattfill in Calc using work Around-->
      <xsl:when test="a:pattFill">
        <xsl:attribute name="draw:fill">
          <xsl:value-of select="'solid'"/>
        </xsl:attribute>

        <!-- Standard color-->
        <xsl:if test="a:pattFill/a:fgClr/a:srgbClr/@val">
          <xsl:attribute name="draw:fill-color">
            <xsl:value-of select="concat('#',a:pattFill/a:fgClr/a:srgbClr/@val)"/>
          </xsl:attribute>

          <!-- Transparency percentage-->
          <xsl:if test="a:pattFill/a:fgClr/a:srgbClr/a:alpha/@val">
            <xsl:variable name="alpha">
              <xsl:value-of select="a:pattFill/a:fgClr/a:srgbClr/a:alpha/@val"/>
            </xsl:variable>

            <xsl:if test="($alpha != '') or ($alpha != 0)">
              <xsl:attribute name="draw:opacity">
                <xsl:value-of select="concat(($alpha div 1000), '%')"/>
              </xsl:attribute>
            </xsl:if>

          </xsl:if>
        </xsl:if>

        <!--Theme color-->
        <xsl:if test="a:pattFill/a:fgClr/a:schemeClr/@val">

          <xsl:attribute name="draw:fill-color">
            <xsl:call-template name="getColorCode">
              <xsl:with-param name="color">
                <xsl:value-of select="a:pattFill/a:fgClr/a:schemeClr/@val"/>
              </xsl:with-param>
              <xsl:with-param name="lumMod">
                <xsl:value-of select="a:pattFill/a:fgClr/a:schemeClr/a:lumMod/@val"/>
              </xsl:with-param>
              <xsl:with-param name="lumOff">
                <xsl:value-of select="a:pattFill/a:fgClr/a:schemeClr/a:lumOff/@val"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:attribute>

          <!-- Transparency percentage-->
          <xsl:if test="a:pattFill/a:fgClr/a:schemeClr/a:alpha/@val">
            <xsl:variable name="alpha">
              <xsl:value-of select="a:pattFill/a:fgClr/a:schemeClr/a:alpha/@val"/>
            </xsl:variable>

            <xsl:if test="($alpha != '') or ($alpha != 0)">
              <xsl:attribute name="draw:opacity">
                <xsl:value-of select="concat(($alpha div 1000), '%')"/>
              </xsl:attribute>
            </xsl:if>

          </xsl:if>
        </xsl:if>
      </xsl:when>

      <!-- fill from style -->
      <xsl:when test="parent::node()/xdr:style/a:fillRef">
        <xsl:attribute name="draw:fill">
          <xsl:value-of select="'solid'"/>
        </xsl:attribute>

        <!-- Standard color-->
        <xsl:if test="parent::node()/xdr:style/a:fillRef/a:srgbClr/@val">
          <xsl:attribute name="draw:fill-color">
            <xsl:value-of select="concat('#',parent::node()/xdr:style/a:fillRef/a:srgbClr/@val)"/>
          </xsl:attribute>

          <!-- Shade percentage-->
          <!--<xsl:if test="xdr:style/a:fillRef/a:srgbClr/a:shade/@val">
              <xsl:variable name ="shade">
              <xsl:value-of select ="a:solidFill/a:srgbClr/a:shade/@val"/>
              </xsl:variable>
              <xsl:if test="($shade != '') or ($shade != 0)">
              <xsl:attribute name ="draw:shadow-opacity">
              <xsl:value-of select="concat(($shade div 1000), '%')"/>
              </xsl:attribute>
              </xsl:if>
              </xsl:if>-->
        </xsl:if>

        <!--Theme color-->
        <xsl:if test="parent::node()/xdr:style/a:fillRef//a:schemeClr/@val">
          <xsl:attribute name="draw:fill-color">
            <xsl:call-template name="getColorCode">
              <xsl:with-param name="color">
                <xsl:value-of select="parent::node()/xdr:style/a:fillRef/a:schemeClr/@val"/>
              </xsl:with-param>
              <xsl:with-param name="lumMod">
                <xsl:value-of select="parent::node()/xdr:style/a:fillRef/a:schemeClr/a:lumMod/@val"
                />
              </xsl:with-param>
              <xsl:with-param name="lumOff">
                <xsl:value-of select="parent::node()/xdr:style/a:fillRef/a:schemeClr/a:lumOff/@val"
                />
              </xsl:with-param>
            </xsl:call-template>
          </xsl:attribute>

          <!-- Shade percentage-->
          <!--<xsl:if test="a:solidFill/a:schemeClr/a:shade/@val">
              <xsl:variable name ="shade">
              <xsl:value-of select ="a:solidFill/a:schemeClr/a:shade/@val"/>
              </xsl:variable>
              <xsl:if test="($shade != '') or ($shade != 0)">
              <xsl:attribute name ="draw:shadow-opacity">
              <xsl:value-of select="concat(($shade div 1000), '%')"/>
              </xsl:attribute>
              </xsl:if>
              </xsl:if>-->
        </xsl:if>

      </xsl:when>
      <!-- default fill for textbox -->
      <xsl:otherwise>
        <xsl:if test="parent::node()/xdr:nvSpPr/xdr:cNvSpPr/@txBox = 1">
          <xsl:attribute name="draw:fill">
            <xsl:text>none</xsl:text>
          </xsl:attribute>
        </xsl:if>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="InsertLineColor">

    <xsl:choose>
      <!-- No line-->
      <xsl:when test="a:ln/a:noFill">
        <xsl:attribute name="draw:stroke">
          <xsl:value-of select="'none'"/>
        </xsl:attribute>
      </xsl:when>

		<!-- Solid line color-->
		<xsl:when test="a:ln/a:solidFill">
			<xsl:attribute name="draw:stroke">
				<xsl:value-of select="'solid'"/>
			</xsl:attribute>

			<!-- Standard color for border-->
			<xsl:if test="a:ln/a:solidFill/a:srgbClr/@val">
				<xsl:attribute name="svg:stroke-color">
					<xsl:value-of select="concat('#',a:ln/a:solidFill/a:srgbClr/@val)"/>
				</xsl:attribute>

				<!-- if color is specified with luminance factors -->
				<xsl:if test="a:ln/a:solidFill/a:srgbClr/a:lumMod">
					<xsl:attribute name="svg:stroke-color">
						<xsl:call-template name="getColorCode">
							<xsl:with-param name="color">
								<xsl:value-of select="a:ln/a:solidFill/a:srgbClr/@val"/>
							</xsl:with-param>
							<xsl:with-param name="lumMod">
								<xsl:value-of select="a:ln/a:solidFill/a:srgbClr/a:lumMod/@val"/>
							</xsl:with-param>
							<xsl:with-param name="lumOff">
								<xsl:choose>
									<xsl:when test="a:ln/a:solidFill/a:srgbClr/a:lumOff/@val">
										<xsl:value-of select="a:ln/a:solidFill/a:srgbClr/a:lumOff/@val"/>
									</xsl:when>
									<xsl:otherwise>
										<xsl:text>0</xsl:text>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:with-param>
							<xsl:with-param name="noTheme">
								<xsl:text>true</xsl:text>
							</xsl:with-param>
						</xsl:call-template>
					</xsl:attribute>
				</xsl:if>

				<!-- Transparency percentage-->
				<xsl:if test="a:ln/a:solidFill/a:srgbClr/a:alpha/@val">
					<xsl:variable name="alpha">
						<xsl:value-of select="a:ln/a:solidFill/a:srgbClr/a:alpha/@val"/>
					</xsl:variable>

					<xsl:if test="($alpha != '') or ($alpha != 0)">
						<xsl:attribute name="svg:stroke-opacity">
							<xsl:value-of select="concat(($alpha div 1000), '%')"/>
						</xsl:attribute>
					</xsl:if>

				</xsl:if>
			</xsl:if>

			<!-- Theme color for border -->
			<xsl:if test="a:ln/a:solidFill/a:schemeClr/@val">
				<xsl:attribute name="svg:stroke-color">
					<xsl:call-template name="getColorCode">
						<xsl:with-param name="color">
							<xsl:value-of select="a:ln/a:solidFill/a:schemeClr/@val"/>
						</xsl:with-param>
						<xsl:with-param name="lumMod">
							<xsl:value-of select="a:ln/a:solidFill/a:schemeClr/a:lumMod/@val"/>
						</xsl:with-param>
						<xsl:with-param name="lumOff">
							<xsl:value-of select="a:ln/a:solidFill/a:schemeClr/a:lumOff/@val"/>
						</xsl:with-param>
					</xsl:call-template>
				</xsl:attribute>

				<!-- Transparency percentage-->
				<xsl:if test="a:ln/a:solidFill/a:schemeClr/a:alpha/@val">
					<xsl:variable name="alpha">
						<xsl:value-of select="a:ln/a:solidFill/a:schemeClr/a:alpha/@val"/>
					</xsl:variable>

					<xsl:if test="($alpha != '') or ($alpha != 0)">
						<xsl:attribute name="svg:stroke-opacity">
							<xsl:value-of select="concat(($alpha div 1000), '%')"/>
						</xsl:attribute>
					</xsl:if>

				</xsl:if>
				<!-- Transparency percentage-->
				<xsl:if test="a:ln/a:solidFill/a:srgbClr/a:alpha/@val">
					<xsl:variable name="alpha">
						<xsl:value-of select="a:ln/a:solidFill/a:srgbClr/a:alpha/@val"/>
					</xsl:variable>

					<xsl:if test="($alpha != '') or ($alpha != 0)">
						<xsl:attribute name="svg:stroke-opacity">
							<xsl:value-of select="concat(($alpha div 1000), '%')"/>
						</xsl:attribute>
					</xsl:if>

				</xsl:if>
			</xsl:if>
		</xsl:when>
		<!-- gradient line color-->
		<xsl:when test="a:ln/a:gradFill">
			<xsl:attribute name="draw:stroke">
				<xsl:value-of select="'solid'"/>
			</xsl:attribute>
			<xsl:for-each select="a:ln/a:gradFill/a:gsLst/a:gs[1]">
				<!-- gradient color for border-->
				<xsl:if test="a:srgbClr/@val">
					<xsl:attribute name="svg:stroke-color">
						<xsl:value-of select="concat('#',a:srgbClr/@val)"/>
					</xsl:attribute>

					<!-- if color is specified with luminance factors -->
					<xsl:if test="a:srgbClr/a:lumMod">
						<xsl:attribute name="svg:stroke-color">
							<xsl:call-template name="getColorCode">
								<xsl:with-param name="color">
									<xsl:value-of select="a:srgbClr/@val"/>
								</xsl:with-param>
								<xsl:with-param name="lumMod">
									<xsl:value-of select="a:srgbClr/a:lumMod/@val"/>
								</xsl:with-param>
								<xsl:with-param name="lumOff">
									<xsl:choose>
										<xsl:when test="a:srgbClr/a:lumOff/@val">
											<xsl:value-of select="a:srgbClr/a:lumOff/@val"/>
										</xsl:when>
										<xsl:otherwise>
											<xsl:text>0</xsl:text>
										</xsl:otherwise>
									</xsl:choose>
								</xsl:with-param>
								<xsl:with-param name="noTheme">
									<xsl:text>true</xsl:text>
								</xsl:with-param>
							</xsl:call-template>
						</xsl:attribute>
					</xsl:if>

					<!-- Transparency percentage-->
					<xsl:if test="a:ln/a:solidFill/a:srgbClr/a:alpha/@val">
						<xsl:variable name="alpha">
							<xsl:value-of select="a:ln/a:solidFill/a:srgbClr/a:alpha/@val"/>
						</xsl:variable>

						<xsl:if test="($alpha != '') or ($alpha != 0)">
							<xsl:attribute name="svg:stroke-opacity">
								<xsl:value-of select="concat(($alpha div 1000), '%')"/>
							</xsl:attribute>
						</xsl:if>

					</xsl:if>
				</xsl:if>

				<!-- Theme color for border -->
				<xsl:if test="a:schemeClr/@val">
					<xsl:attribute name="svg:stroke-color">
						<xsl:call-template name="getColorCode">
							<xsl:with-param name="color">
								<xsl:value-of select="a:schemeClr/@val"/>
							</xsl:with-param>
							<xsl:with-param name="lumMod">
								<xsl:value-of select="a:schemeClr/a:lumMod/@val"/>
							</xsl:with-param>
							<xsl:with-param name="lumOff">
								<xsl:value-of select="a:schemeClr/a:lumOff/@val"/>
							</xsl:with-param>
						</xsl:call-template>
					</xsl:attribute>

					<!-- Transparency percentage-->
					<xsl:if test="a:schemeClr/a:alpha/@val">
						<xsl:variable name="alpha">
							<xsl:value-of select="a:schemeClr/a:alpha/@val"/>
						</xsl:variable>

						<xsl:if test="($alpha != '') or ($alpha != 0)">
							<xsl:attribute name="svg:stroke-opacity">
								<xsl:value-of select="concat(($alpha div 1000), '%')"/>
							</xsl:attribute>
						</xsl:if>

					</xsl:if>
					<!-- Transparency percentage-->
					<xsl:if test="a:srgbClr/a:alpha/@val">
						<xsl:variable name="alpha">
							<xsl:value-of select="a:srgbClr/a:alpha/@val"/>
						</xsl:variable>

						<xsl:if test="($alpha != '') or ($alpha != 0)">
							<xsl:attribute name="svg:stroke-opacity">
								<xsl:value-of select="concat(($alpha div 1000), '%')"/>
							</xsl:attribute>
						</xsl:if>

					</xsl:if>
				</xsl:if>
			</xsl:for-each>

		</xsl:when>
		<!--added by Sonata to support Pattfill in Calc using work Around-->
		<xsl:when test="a:ln/a:pattFill">
			<xsl:attribute name="draw:stroke">
				<xsl:value-of select="'solid'"/>
			</xsl:attribute>

			<!-- Standard color-->
			<xsl:if test="a:ln/a:pattFill/a:fgClr/a:srgbClr/@val">
				<xsl:attribute name="svg:stroke-color">
					<xsl:value-of select="concat('#',a:ln/a:pattFill/a:fgClr/a:srgbClr/@val)"/>
				</xsl:attribute>

				<!-- Transparency percentage-->
				<xsl:if test="a:ln/a:pattFill/a:fgClr/a:srgbClr/a:alpha/@val">
					<xsl:variable name="alpha">
						<xsl:value-of select="a:ln/a:pattFill/a:fgClr/a:srgbClr/a:alpha/@val"/>
					</xsl:variable>

					<xsl:if test="($alpha != '') or ($alpha != 0)">
						<xsl:attribute name="svg:stroke-opacity">
							<xsl:value-of select="concat(($alpha div 1000), '%')"/>
						</xsl:attribute>
					</xsl:if>

				</xsl:if>
			</xsl:if>

			<!--Theme color-->
			<xsl:if test="a:ln/a:pattFill/a:fgClr/a:schemeClr/@val">

				<xsl:attribute name="svg:stroke-color">
					<xsl:call-template name="getColorCode">
						<xsl:with-param name="color">
							<xsl:value-of select="a:ln/a:pattFill/a:fgClr/a:schemeClr/@val"/>
						</xsl:with-param>
						<xsl:with-param name="lumMod">
							<xsl:value-of select="a:ln/a:pattFill/a:fgClr/a:schemeClr/a:lumMod/@val"/>
						</xsl:with-param>
						<xsl:with-param name="lumOff">
							<xsl:value-of select="a:ln/a:pattFill/a:fgClr/a:schemeClr/a:lumOff/@val"/>
						</xsl:with-param>
					</xsl:call-template>
				</xsl:attribute>

				<!-- Transparency percentage-->
				<xsl:if test="a:ln/a:pattFill/a:fgClr/a:schemeClr/a:alpha/@val">
					<xsl:variable name="alpha">
						<xsl:value-of select="a:ln/a:pattFill/a:fgClr/a:schemeClr/a:alpha/@val"/>
					</xsl:variable>

					<xsl:if test="($alpha != '') or ($alpha != 0)">
						<xsl:attribute name="svg:stroke-opacity">
							<xsl:value-of select="concat(($alpha div 1000), '%')"/>
						</xsl:attribute>
					</xsl:if>

				</xsl:if>
			</xsl:if>
		</xsl:when>

		<xsl:otherwise>
        <!--Line reference-->
        <xsl:if
          test="not( (a:prstGeom/@prst='flowChartInternalStorage') or
          (a:prstGeom/@prst='flowChartPredefinedProcess') or
          (a:prstGeom/@prst='flowChartSummingJunction') or
          (a:prstGeom/@prst='flowChartOr') )">
          <xsl:if test="parent::node()/xdr:style/a:lnRef">
            <xsl:attribute name="draw:stroke">
              <xsl:value-of select="'solid'"/>
            </xsl:attribute>

            <!--Standard color for border-->
            <xsl:if test="parent::node()/xdr:style/a:lnRef/a:srgbClr/@val">
              <xsl:attribute name="svg:stroke-color">
                <xsl:value-of select="concat('#',parent::node()/xdr:style/a:lnRef/a:srgbClr/@val)"/>
              </xsl:attribute>

              <!--Shade percentage-->
              <!--
                <xsl:if test="xdr:style/a:lnRef/a:srgbClr/a:shade/@val">
                <xsl:variable name ="shade">
                <xsl:value-of select ="xdr:style/a:lnRef/a:srgbClr/a:shade/@val"/>
                </xsl:variable>
              -->
              <!--<xsl:if test="($shade != '') or ($shade != 0)">
                <xsl:attribute name ="svg:stroke-opacity">
                <xsl:value-of select="concat(($shade div 1000), '%')"/>
                </xsl:attribute>
                </xsl:if>-->
              <!--
                </xsl:if>-->
            </xsl:if>

            <!--Theme color for border-->
            <xsl:if test="parent::node()/xdr:style/a:lnRef/a:schemeClr/@val">

              <xsl:attribute name="svg:stroke-color">
                <xsl:call-template name="getColorCode">
                  <xsl:with-param name="color">
                    <xsl:value-of select="parent::node()/xdr:style/a:lnRef/a:schemeClr/@val"/>
                  </xsl:with-param>
                  <xsl:with-param name="lumMod">
                    <xsl:value-of
                      select="parent::node()/xdr:style/a:lnRef/a:schemeClr/a:lumMod/@val"/>
                  </xsl:with-param>
                  <xsl:with-param name="lumOff">
                    <xsl:value-of
                      select="parent::node()/xdr:style/a:lnRef/a:schemeClr/a:lumOff/@val"/>
                  </xsl:with-param>
                </xsl:call-template>
              </xsl:attribute>

              <!--Shade percentage -->
              <!--<xsl:if test="xdr:style/a:lnRef/a:schemeClr/a:shade/@val">
                <xsl:variable name ="shade">
                <xsl:value-of select ="xdr:style/a:lnRef/a:schemeClr/a:shade/@val"/>
                </xsl:variable>
              -->
              <!--<xsl:if test="($shade != '') or ($shade != 0)">
                <xsl:attribute name ="svg:stroke-opacity">
                <xsl:value-of select="concat(($shade div 1000), '%')"/>
                </xsl:attribute>
                </xsl:if>-->
              <!--
                </xsl:if>-->
            </xsl:if>

          </xsl:if>
        </xsl:if>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="InsertLineStyle">

    <!-- Line width-->
    <xsl:choose>
      <xsl:when test="a:ln/@w">
        <xsl:attribute name="svg:stroke-width">
          <!--Sonata: defect#2151484-->
          <xsl:choose>
            <xsl:when test="format-number(a:ln/@w div 360000,'#.###')=''">
              <xsl:value-of select="'0cm'"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="concat(format-number(a:ln/@w div 360000, '#.###'), 'cm')"/>
            </xsl:otherwise>
          </xsl:choose>
        
        </xsl:attribute>
      </xsl:when>

      <!-- line width from theme -->
      <xsl:when test="parent::node()/xdr:style/a:lnRef/@idx">
        <xsl:variable name="index">
          <xsl:value-of select="parent::node()/xdr:style/a:lnRef/@idx + 1"/>
        </xsl:variable>
        <xsl:attribute name="svg:stroke-width">
          <xsl:call-template name="ConvertEmu">
            <xsl:with-param name="length"
              select="key('Part', 'xl/theme/theme1.xml')/a:theme/a:themeElements/a:fmtScheme/a:lnStyleLst/a:ln[position() = $index]/@w"/>
            <xsl:with-param name="unit">cm</xsl:with-param>
          </xsl:call-template>
        </xsl:attribute>
      </xsl:when>

      <!-- TextBox default border -->
      <xsl:when test="not(a:ln/@w) and (parent::node()/xdr:nvSpPr/xdr:cNvSpPr/@txBox = 1)">
        <xsl:attribute name="draw:stroke">
          <xsl:value-of select="'none'"/>
        </xsl:attribute>
      </xsl:when>
    </xsl:choose>


    <!-- Line Dash property-->
    <xsl:choose>
      <xsl:when test="not(a:ln/a:noFill) and a:ln/a:prstDash">
        <xsl:for-each select="a:ln">
          <xsl:call-template name="InsertLineDash"/>
        </xsl:for-each>
      </xsl:when>

      <!-- line dash from theme -->
      <xsl:when test="parent::node()/xdr:style/a:lnRef/@idx">
        <xsl:variable name="index">
          <xsl:value-of select="parent::node()/xdr:style/a:lnRef/@idx + 1"/>
        </xsl:variable>

        <xsl:for-each
          select="key('Part', 'xl/theme/theme1.xml')/a:theme/a:themeElements/a:fmtScheme/a:lnStyleLst/a:ln[position() = $index]">
          <xsl:call-template name="InsertLineDash"/>
        </xsl:for-each>
      </xsl:when>
    </xsl:choose>

    <!-- Line join property -->
    <xsl:choose>
      <xsl:when test="a:ln/a:miter">
        <xsl:attribute name="draw:stroke-linejoin">
          <xsl:text>miter</xsl:text>
        </xsl:attribute>
      </xsl:when>

      <xsl:when test="a:ln/a:bevel">
        <xsl:attribute name="draw:stroke-linejoin">
          <xsl:text>bevel</xsl:text>
        </xsl:attribute>
      </xsl:when>

      <xsl:when test="a:ln/a:round">
        <xsl:attribute name="draw:stroke-linejoin">
          <xsl:text>round</xsl:text>
        </xsl:attribute>
      </xsl:when>
    </xsl:choose>

    <!-- Line Arrow -->
    <!-- Head End-->
    <xsl:for-each select="a:ln/a:headEnd">
      <xsl:if test="@type">
        <xsl:attribute name="draw:marker-start">
          <xsl:value-of select="@type"/>
        </xsl:attribute>

        <xsl:attribute name="draw:marker-start-width">
          <xsl:call-template name="getArrowSize">
            <xsl:with-param name="w" select="@w"/>
            <xsl:with-param name="len" select="@len"/>
          </xsl:call-template>
        </xsl:attribute>
      </xsl:if>
    </xsl:for-each>

    <!-- Tail End-->
    <xsl:for-each select="a:ln/a:tailEnd">
      <xsl:if test="@type">
        <xsl:attribute name="draw:marker-end">
          <xsl:value-of select="@type"/>
        </xsl:attribute>

        <xsl:attribute name="draw:marker-end-width">
          <xsl:call-template name="getArrowSize">
            <xsl:with-param name="w" select="@w"/>
            <xsl:with-param name="len" select="@len"/>
          </xsl:call-template>
        </xsl:attribute>
      </xsl:if>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="getArrowSize">
    <xsl:param name="w"/>
    <xsl:param name="len"/>

    <xsl:choose>
      <xsl:when test="($w = 'sm' ) and ($len = 'sm' )">
        <xsl:value-of select="concat(0.15,'cm' )"/>
      </xsl:when>

      <xsl:when test="($w = 'sm' ) and ($len = 'med' )">
        <xsl:value-of select="concat(0.18,'cm' )"/>
      </xsl:when>

      <xsl:when test="($w = 'sm' ) and ($len = 'lg' )">
        <xsl:value-of select="concat(0.2,'cm' )"/>
      </xsl:when>

      <xsl:when test="($w = 'med' ) and ($len = 'sm' )">
        <xsl:value-of select="concat(0.21,'cm' )"/>
      </xsl:when>

      <xsl:when test="($w = 'med' ) and ($len = 'lg' )">
        <xsl:value-of select="concat(0.3,'cm' )"/>
      </xsl:when>

      <xsl:when test="($w = 'lg' ) and ($len = 'sm' )">
        <xsl:value-of select="concat(0.31,'cm' )"/>
      </xsl:when>

      <xsl:when test="($w = 'lg' ) and ($len = 'med' )">
        <xsl:value-of select="concat(0.35,'cm' )"/>
      </xsl:when>

      <xsl:when test="($w = 'lg' ) and ($len = 'lg' )">
        <xsl:value-of select="concat(0.4,'cm' )"/>
      </xsl:when>

      <xsl:otherwise>
        <xsl:value-of select="concat(0.25,'cm' )"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="InsertLineDash">

    <!--Code added by Sonata-->
    <xsl:if test="a:noFill">
      <xsl:attribute name="draw:stroke">
        <xsl:value-of select="'none'"/>
      </xsl:attribute>
    </xsl:if>
    <!--End of 1953423-->
    <xsl:if test ="not(a:noFill)">
      <xsl:choose>

        <xsl:when test ="a:prstDash/@val='solid'">
        <xsl:attribute name="draw:stroke">
            <xsl:value-of select ="'solid'"/>
        </xsl:attribute>
      </xsl:when>

      <xsl:otherwise>
        <xsl:attribute name="draw:stroke">
            <xsl:value-of select ="'dash'"/>
        </xsl:attribute>

        <xsl:attribute name="draw:stroke-dash">

          <xsl:choose>
            <xsl:when test="(a:prstDash/@val = 'sysDot' ) and (@cap = 'rnd' )">
                <xsl:value-of select ="'Fine_20_Dotted'"/>
            </xsl:when>

            <xsl:when test="a:prstDash/@val = 'sysDot' ">
                <xsl:value-of select ="'Fine_20_Dotted'"/>
            </xsl:when>

            <xsl:when test="(a:prstDash/@val = 'sysDash' ) and (@cap = 'rnd' )">
                <xsl:value-of select ="'Dash'"/>
            </xsl:when>

            <xsl:when test="a:prstDash/@val = 'sysDash' ">
                <xsl:value-of select ="'Dash'"/>
            </xsl:when>

            <!--changed by sonata for bug no:2229924-->
            <!--<xsl:when test="(a:prstDash/@val = 'dash' ) and (@cap = 'rnd' )">
                <xsl:value-of select ="'dashRound'"/>
            </xsl:when>-->
            <xsl:when test="(a:prstDash/@val = 'dash' ) and (@cap = 'rnd' )">
              <xsl:value-of select ="'Ultrafine_20_Dashed'"/>
            </xsl:when>
            <!--end of change-->
            

            <xsl:when test="a:prstDash/@val = 'dash' ">
                <xsl:value-of select ="'Ultrafine_20_Dashed'"/>
            </xsl:when>

            <xsl:when test="(a:prstDash/@val = 'dashDot' ) and (@cap = 'rnd' )">
                <xsl:value-of select ="'dashDotRound'"/>
            </xsl:when>

            <xsl:when test="a:prstDash/@val = 'dashDot' ">
                <xsl:value-of select ="'dashDot'"/>
            </xsl:when>

            <xsl:when test="(a:prstDash/@val = 'lgDash' ) and (@cap = 'rnd' )">
                <xsl:value-of select ="'Fine_20_Dashed'"/>
            </xsl:when>

            <xsl:when test="a:prstDash/@val = 'lgDash' ">
                <xsl:value-of select ="'Fine_20_Dashed'"/>
            </xsl:when>

            <xsl:when test="(a:prstDash/@val = 'lgDashDot' ) and (@cap = 'rnd' )">
                <xsl:value-of select ="'_1_Dot_2_Dash'"/>
            </xsl:when>

            <xsl:when test="a:prstDash/@val = 'lgDashDot' ">
                <xsl:value-of select ="'_1_Dot_2_Dash'"/>
            </xsl:when>

            <xsl:when test="(a:prstDash/@val = 'lgDashDotDot' ) and (@cap = 'rnd' )">
                <xsl:value-of select ="'_32__20_Dots_20_1_20_Dash'"/>
            </xsl:when>

            <xsl:when test="a:prstDash/@val = 'lgDashDotDot' ">
                <xsl:value-of select ="'_32__20_Dots_20_1_20_Dash'"/>
            </xsl:when>
          </xsl:choose>
        </xsl:attribute>

      </xsl:otherwise>
      </xsl:choose>
    </xsl:if>
    
  
  </xsl:template>

  <xsl:template name="InsertTextLayout">

    <xsl:if test="a:bodyPr/@lIns">
      <xsl:attribute name="fo:padding-left">
        <xsl:call-template name="ConvertEmu">
          <xsl:with-param name="length" select="a:bodyPr/@lIns"/>
          <xsl:with-param name="unit">cm</xsl:with-param>
        </xsl:call-template>
      </xsl:attribute>
    </xsl:if>
    <xsl:if test="not(a:bodyPr/@lIns)">
      <xsl:attribute name="fo:padding-left">
        <xsl:call-template name="ConvertEmu">
          <xsl:with-param name="length" select="'91440'"/>
          <xsl:with-param name="unit">cm</xsl:with-param>
        </xsl:call-template>
      </xsl:attribute>
    </xsl:if>

    <xsl:if test="a:bodyPr/@tIns">
      <xsl:attribute name="fo:padding-top">
        <xsl:call-template name="ConvertEmu">
          <xsl:with-param name="length" select="a:bodyPr/@tIns"/>
          <xsl:with-param name="unit">cm</xsl:with-param>
        </xsl:call-template>
      </xsl:attribute>
    </xsl:if>
    <xsl:if test="not(a:bodyPr/@tIns)">
      <xsl:attribute name="fo:padding-top">
        <xsl:call-template name="ConvertEmu">
          <xsl:with-param name="length" select="'45720'"/>
          <xsl:with-param name="unit">cm</xsl:with-param>
        </xsl:call-template>
      </xsl:attribute>
    </xsl:if>

    <xsl:if test="a:bodyPr/@rIns">
      <xsl:attribute name="fo:padding-right">
        <xsl:call-template name="ConvertEmu">
          <xsl:with-param name="length" select="a:bodyPr/@rIns"/>
          <xsl:with-param name="unit">cm</xsl:with-param>
        </xsl:call-template>
      </xsl:attribute>
    </xsl:if>
    <xsl:if test="not(a:bodyPr/@rIns)">
      <xsl:attribute name="fo:padding-right">
        <xsl:call-template name="ConvertEmu">
          <xsl:with-param name="length" select="'91440'"/>
          <xsl:with-param name="unit">cm</xsl:with-param>
        </xsl:call-template>
      </xsl:attribute>
    </xsl:if>

    <xsl:if test="a:bodyPr/@bIns">
      <xsl:attribute name="fo:padding-bottom">
        <xsl:call-template name="ConvertEmu">
          <xsl:with-param name="length" select="a:bodyPr/@bIns"/>
          <xsl:with-param name="unit">cm</xsl:with-param>
        </xsl:call-template>
      </xsl:attribute>
    </xsl:if>
    <xsl:if test="not(a:bodyPr/@bIns)">
      <xsl:attribute name="fo:padding-bottom">
        <xsl:call-template name="ConvertEmu">
          <xsl:with-param name="length" select="'45720'"/>
          <xsl:with-param name="unit">cm</xsl:with-param>
        </xsl:call-template>
      </xsl:attribute>
    </xsl:if>

    <xsl:if test="( (a:bodyPr/a:spAutoFit) or (a:bodyPr/@wrap='square') )">
      <xsl:attribute name="draw:auto-grow-height">
        <xsl:value-of select="'true'"/>
      </xsl:attribute>
    </xsl:if>
    <xsl:if test="not(a:bodyPr/a:spAutoFit)">
      <xsl:attribute name="draw:auto-grow-height">
        <xsl:value-of select="'false'"/>
      </xsl:attribute>
      <xsl:attribute name="draw:auto-grow-width">
        <xsl:value-of select="'false'"/>
      </xsl:attribute>
    </xsl:if>
  </xsl:template>

  <xsl:template name="InsertTextBoxTextStyles">
    <!-- get all sheet Id's -->
		  <!-- Perofomance-->
		<xsl:for-each select="key('Part', 'xl/workbook.xml')/e:workbook">
			<xsl:for-each select="e:sheets/e:sheet">
      <xsl:variable name="sheet">
        <xsl:call-template name="GetTarget">
          <xsl:with-param name="id">
            <xsl:value-of select="@r:id"/>
          </xsl:with-param>
          <xsl:with-param name="document">xl/workbook.xml</xsl:with-param>
        </xsl:call-template>
      </xsl:variable>
      <!--i.e. $sheet = worksheets/sheet1.xml -->

      <!-- go to worksheet file and search for drawing -->
				  <!-- Perofomance-->
				<xsl:for-each select="key('Part', concat('xl/',$sheet))/e:worksheet">
					<xsl:for-each select="e:drawing">
        <xsl:variable name="drawing">
          <xsl:call-template name="GetTarget">
            <xsl:with-param name="id">
              <xsl:value-of select="@r:id"/>
            </xsl:with-param>
            <xsl:with-param name="document">
              <xsl:value-of select="concat('xl/',$sheet)"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:variable>
        <!-- i.e. $drawing = ../drawings/drawing2.xml -->

        <!-- finally insert entry for each chart -->
						<!--SP2,Scenario:ods_SP2_xlsx_2.5_ods,ChartLost because SP2 created xlsx has oneCellAnchor in Place of twoCellAnchor-->
						<!--<xsl:for-each select="key('Part', concat('xl/',substring-after($drawing,'/')))/xdr:wsDr/xdr:twoCellAnchor/xdr:sp/xdr:txBody">-->
        <xsl:for-each
						  select="key('Part', concat('xl/',substring-after($drawing,'/')))/xdr:wsDr//xdr:sp/xdr:txBody">
							<!--SP2,Scenario:ods_SP2_xlsx_2.5_ods,End-->
          <xsl:for-each select="a:p/a:pPr">
            <xsl:call-template name="InsertTextBoxParagraphStyle"/>
          </xsl:for-each>
          <xsl:for-each select="a:p/a:r/a:rPr">
            <style:style style:name="{generate-id(parent::node())}" style:family="text">
              <style:text-properties fo:font-family="Calibri" fo:font-size="11pt" fo:font-weight="normal"
                fo:color="#000000">
                <xsl:call-template name="TextBoxRunProperties">
                  <xsl:with-param name="rPr" select="."/>
	   </xsl:call-template>
              </style:text-properties>
            </style:style>
          </xsl:for-each>
   </xsl:for-each>
      </xsl:for-each>
    </xsl:for-each>
				</xsl:for-each>
		</xsl:for-each>
  </xsl:template>

  <xsl:template name="InsertTextBoxParagraphStyle">
    <style:style style:name="{generate-id(parent::node())}" style:family="paragraph">
      <style:paragraph-properties>

        <!-- horizontal alignment -->
        <xsl:if test="@algn ='ctr' or @algn ='r' or @algn ='l' or @algn ='just' ">
          <xsl:attribute name="fo:text-align">
            <xsl:choose>
              <!-- Center Alignment-->
              <xsl:when test="@algn ='ctr' ">
                <xsl:text>center</xsl:text>
              </xsl:when>
              <!-- Right Alignment-->
              <xsl:when test="@algn ='r' ">
                <xsl:text>end</xsl:text>
              </xsl:when>
              <!-- Left Alignment-->
              <xsl:when test="@algn ='l' ">
                <xsl:text>start</xsl:text>
              </xsl:when>
              <xsl:when test="@algn ='just' ">
                <xsl:text>justify</xsl:text>
              </xsl:when>
            </xsl:choose>
          </xsl:attribute>
        </xsl:if>

        <!-- left margin -->
        <xsl:if test="@marL">
          <xsl:attribute name="fo:margin-left">
            <xsl:value-of select="concat(format-number(@marL div 360000, '0.##'), 'cm')"/>
          </xsl:attribute>
        </xsl:if>

        <!-- first line indent -->
        <xsl:if test="@indent">
          <xsl:attribute name="fo:text-indent">
            <xsl:choose>
              <xsl:when test="@indent &gt; 0">
                <xsl:value-of select="concat(format-number(@indent div 360000, '0.##'), 'cm')"/>
              </xsl:when>
              <xsl:when test="@indent = 0">
                <xsl:text>0</xsl:text>
              </xsl:when>
              <xsl:when
                test="(@indent &lt; 0) and number(substring-after(@indent,'-')) &lt;= @marL">
                <xsl:value-of select="concat(format-number(@indent div 360000, '0.##'), 'cm')"/>
              </xsl:when>
              <xsl:when
                test="(@indent &lt; 0) and number(substring-after(@indent,'-')) &gt; @marL">
                <xsl:value-of select="concat(format-number(@marL div 360000, '0.##'), 'cm')"/>
              </xsl:when>
            </xsl:choose>
          </xsl:attribute>
        </xsl:if>

        <!-- spacing before -->
        <xsl:if test="a:spcBef/a:spcPts/@val">
          <xsl:attribute name="fo:margin-top">
            <xsl:value-of
              select="concat(format-number(a:spcBef/a:spcPts/@val div 2835, '0.##'), 'cm')"/>
          </xsl:attribute>
        </xsl:if>

        <!-- spacig after-->
        <xsl:if test="a:spcAft/a:spcPts/@val">
          <xsl:attribute name="fo:margin-bottom">
            <xsl:value-of
              select="concat(format-number(a:spcAft/a:spcPts/@val div 2835, '0.##'), 'cm')"/>
          </xsl:attribute>
        </xsl:if>

        <!-- If the line space is in Percentage-->
        <xsl:if test="a:lnSpc/a:spcPct/@val">
          <xsl:attribute name="fo:line-height">
            <xsl:value-of select="concat(format-number(a:lnSpc/a:spcPct/@val div 1000,'###'), '%')"
            />
          </xsl:attribute>
        </xsl:if>

        <!-- If the line space is in Points-->
        <xsl:if test="a:lnSpc/a:spcPts">
          <xsl:attribute name="style:line-height-at-least">
            <xsl:value-of
              select="concat(format-number(a:lnSpc/a:spcPts/@val div 2835, '0.##'), 'cm')"/>
          </xsl:attribute>
        </xsl:if>
      </style:paragraph-properties>
    </style:style>
  </xsl:template>

  <xsl:template name="InsertBitmapFill">

    <xsl:for-each select="a:blipFill">
      <xsl:attribute name="draw:fill">
        <xsl:text>bitmap</xsl:text>
      </xsl:attribute>

      <xsl:attribute name="draw:fill-image-name">
        <xsl:value-of select="generate-id()"/>
      </xsl:attribute>

      <xsl:choose>
        <!-- tile bitmap -->
        <xsl:when test="a:tile">
          <xsl:for-each select="a:tile">
            <xsl:attribute name="style:repeat">
              <xsl:text>repeat</xsl:text>
            </xsl:attribute>

            <!-- bitmap position -->
            <xsl:if test="@algn">
              <xsl:attribute name="draw:fill-image-ref-point">
                <xsl:choose>
                  <xsl:when test="@algn = 'tl' ">
                    <xsl:text>top-left</xsl:text>
                  </xsl:when>
                  <xsl:when test="@algn = 't' ">
                    <xsl:text>top</xsl:text>
                  </xsl:when>
                  <xsl:when test="@algn = 'tr' ">
                    <xsl:text>top-right</xsl:text>
                  </xsl:when>
                  <xsl:when test="@algn = 'l' ">
                    <xsl:text>left</xsl:text>
                  </xsl:when>
                  <xsl:when test="@algn = 'ctr' ">
                    <xsl:text>center</xsl:text>
                  </xsl:when>
                  <xsl:when test="@algn = 'r' ">
                    <xsl:text>right</xsl:text>
                  </xsl:when>
                  <xsl:when test="@algn = 'bl' ">
                    <xsl:text>bottom-left</xsl:text>
                  </xsl:when>
                  <xsl:when test="@algn = 'b' ">
                    <xsl:text>bottom</xsl:text>
                  </xsl:when>
                  <xsl:when test="@algn = 'br' ">
                    <xsl:text>bottom-right</xsl:text>
                  </xsl:when>
                </xsl:choose>
              </xsl:attribute>
            </xsl:if>
          </xsl:for-each>
        </xsl:when>
        <xsl:otherwise>
          <xsl:attribute name="style:repeat">
            <xsl:text>stretch</xsl:text>
          </xsl:attribute>
        </xsl:otherwise>
      </xsl:choose>

      <!-- transparency -->
      <xsl:for-each select="a:blip/a:alphaModFix">
        <xsl:attribute name="draw:opacity">
          <xsl:value-of select="concat(@amt div 1000,'%' )"/>
        </xsl:attribute>
      </xsl:for-each>
    </xsl:for-each>

  </xsl:template>

  <xsl:template name="TextBoxRunProperties">
    <xsl:param name="defRPr" select="."/>
    <xsl:param name="rPr" select="."/>
    <xsl:param name="deftxPr" select="."/>
    <!-- font name -->
    <xsl:choose>
      <xsl:when test ="$rPr/a:latin/@typeface or
                       $rPr/a:latin/@typeface or
                       $rPr/a:sym/@typeface or $rPr/a:cs/@typeface">

        <xsl:call-template name="tmpChartFontName">
          <xsl:with-param name="node" select="$rPr"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test ="$defRPr/a:latin/@typeface or
                       $defRPr/a:latin/@typeface or
                       $defRPr/a:sym/@typeface or $defRPr/a:cs/@typeface">
        <xsl:call-template name="tmpChartFontName">
          <xsl:with-param name="node" select="$defRPr"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test ="$deftxPr/a:latin/@typeface or
                       $deftxPr/a:latin/@typeface or
                       $deftxPr/a:sym/@typeface or $deftxPr/a:cs/@typeface">
        <xsl:call-template name="tmpChartFontName">
          <xsl:with-param name="node" select="$deftxPr"/>
        </xsl:call-template>
      </xsl:when>
    </xsl:choose>

    <!-- font size -->
    <xsl:choose>
      <xsl:when test="$rPr/@sz">
      <xsl:attribute name="fo:font-size">
          <xsl:value-of select="concat(format-number($rPr/@sz div 100,'0.##'), 'pt')"/>
      </xsl:attribute>
            <xsl:attribute name="style:font-size-asian">
              <xsl:value-of select="concat(format-number($rPr/@sz div 100,'0.##'), 'pt')"/>
            </xsl:attribute>
            <xsl:attribute name="style:font-size-complex">
              <xsl:value-of select="concat(format-number($rPr/@sz div 100,'0.##'), 'pt')"/>
            </xsl:attribute>
      </xsl:when>
      <xsl:when test="$defRPr/@sz">
         <xsl:attribute name="fo:font-size">
              <xsl:value-of select="concat(format-number($defRPr/@sz div 100,'0.##'), 'pt')"/>
            </xsl:attribute>
            <xsl:attribute name="style:font-size-asian">
              <xsl:value-of select="concat(format-number($defRPr/@sz div 100,'0.##'), 'pt')"/>
            </xsl:attribute>
            <xsl:attribute name="style:font-size-complex">
          <xsl:value-of select="concat(format-number($defRPr/@sz div 100,'0.##'), 'pt')"/>
        </xsl:attribute>
      </xsl:when>
      <xsl:when test="$deftxPr/@sz">
         <xsl:attribute name="fo:font-size">
              <xsl:value-of select="concat(format-number($deftxPr/@sz div 100,'0.##'), 'pt')"/>
            </xsl:attribute>
            <xsl:attribute name="style:font-size-asian">
              <xsl:value-of select="concat(format-number($deftxPr/@sz div 100,'0.##'), 'pt')"/>
            </xsl:attribute>
            <xsl:attribute name="style:font-size-complex">
              <xsl:value-of select="concat(format-number($deftxPr/@sz div 100,'0.##'), 'pt')"/>
            </xsl:attribute>
     </xsl:when>
    </xsl:choose>
    <xsl:choose>
      <xsl:when test="$rPr/@b = 1">
      <xsl:attribute name="fo:font-weight">
        <xsl:value-of select="'bold'"/>
      </xsl:attribute>
      </xsl:when>
      <xsl:when test="$defRPr/@b = 1">
      <xsl:attribute name="fo:font-weight">
        <xsl:value-of select="'bold'"/>
      </xsl:attribute>
      </xsl:when>
      <xsl:when test="$rPr/@b = 0">
        <xsl:attribute name="fo:font-weight">
          <xsl:value-of select="'normal'"/>
        </xsl:attribute>
      </xsl:when>
      <xsl:when test="$defRPr/@b = 0">
        <xsl:attribute name="fo:font-weight">
          <xsl:value-of select="'normal'"/>
        </xsl:attribute>
      </xsl:when>
      <xsl:when test="$deftxPr/@b = 1">
        <xsl:attribute name="fo:font-weight">
          <xsl:value-of select="'bold'"/>
        </xsl:attribute>
      </xsl:when>
      <xsl:when test="$deftxPr/@b = 1">
        <xsl:attribute name="fo:font-weight">
          <xsl:value-of select="'bold'"/>
        </xsl:attribute>
      </xsl:when>
    </xsl:choose>
 <!-- Italic-->
    <xsl:choose>
      <xsl:when test="$rPr/@i = 1">
      <xsl:attribute name="fo:font-style">
        <xsl:value-of select="'italic'"/>
      </xsl:attribute>
      </xsl:when>
      <xsl:when test="$defRPr/@i = 1">
        <xsl:attribute name="fo:font-style">
          <xsl:value-of select="'italic'"/>
        </xsl:attribute>
      </xsl:when>
      <xsl:when test="$rPr/@i = 0">
        <xsl:attribute name="fo:font-style">
          <xsl:value-of select="'none'"/>
        </xsl:attribute>
      </xsl:when>
      <xsl:when test="$defRPr/@i = 0">
        <xsl:attribute name="fo:font-style">
          <xsl:value-of select="'none'"/>
        </xsl:attribute>
      </xsl:when>
      <xsl:when test="$deftxPr/@i = 1">
        <xsl:attribute name="fo:font-style">
          <xsl:value-of select="'italic'"/>
        </xsl:attribute>
      </xsl:when>
      <xsl:when test="$deftxPr/@i = 0">
        <xsl:attribute name="fo:font-style">
          <xsl:value-of select="'none'"/>
        </xsl:attribute>
      </xsl:when>
    </xsl:choose>
    <!-- font color -->
    <xsl:choose>
   
        <xsl:when test="$rPr/a:solidFill or $defRPr/a:solidFill or $deftxPr/a:solidFill ">
          <xsl:choose>
            <xsl:when test="$rPr/a:solidFill">
              <xsl:choose>
                <xsl:when test="$rPr/a:solidFill/a:srgbClr/@val">
        <xsl:attribute name="fo:color">
                    <xsl:value-of select="concat('#',$rPr/a:solidFill/a:srgbClr/@val)"/>
        </xsl:attribute>
      </xsl:when>
 <!-- scheme color -->
                <xsl:when test="$rPr/a:solidFill/a:schemeClr/@val">
        <xsl:attribute name="fo:color">
          <xsl:call-template name="getColorCode">
            <xsl:with-param name="color">
                        <xsl:value-of select="$rPr/a:solidFill/a:schemeClr/@val"/>
            </xsl:with-param>
            <xsl:with-param name="lumMod">
                        <xsl:value-of select="$rPr/a:solidFill/a:schemeClr/a:lumMod/@val"/>
            </xsl:with-param>
            <xsl:with-param name="lumOff">
                        <xsl:value-of select="$rPr/a:solidFill/a:schemeClr/a:lumOff/@val"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:attribute>
      </xsl:when>
              </xsl:choose>
            </xsl:when>
            <xsl:when test="$defRPr/a:solidFill">
              <xsl:choose>
                <xsl:when test="$defRPr/a:solidFill/a:srgbClr/@val">
                  <xsl:attribute name="fo:color">
                    <xsl:value-of select="concat('#',$defRPr/a:solidFill/a:srgbClr/@val)"/>
                  </xsl:attribute>
                </xsl:when>

                <!-- scheme color -->
                <xsl:when test="$defRPr/a:solidFill/a:schemeClr/@val">
                  <xsl:attribute name="fo:color">
                    <xsl:call-template name="getColorCode">
                      <xsl:with-param name="color">
                        <xsl:value-of select="$defRPr/a:solidFill/a:schemeClr/@val"/>
                      </xsl:with-param>
                      <xsl:with-param name="lumMod">
                        <xsl:value-of select="$defRPr/a:solidFill/a:schemeClr/a:lumMod/@val"/>
                      </xsl:with-param>
                      <xsl:with-param name="lumOff">
                        <xsl:value-of select="$defRPr/a:solidFill/a:schemeClr/a:lumOff/@val"/>
                      </xsl:with-param>
                    </xsl:call-template>
                  </xsl:attribute>
                </xsl:when>
              </xsl:choose>
            </xsl:when>
            <xsl:when test="$deftxPr/a:solidFill">
              <xsl:choose>
                <xsl:when test="$deftxPr/a:solidFill/a:srgbClr/@val">
                  <xsl:attribute name="fo:color">
                    <xsl:value-of select="concat('#',$deftxPr/a:solidFill/a:srgbClr/@val)"/>
                  </xsl:attribute>
                </xsl:when>

                <!-- scheme color -->
                <xsl:when test="$defRPr/a:solidFill/a:schemeClr/@val">
                  <xsl:attribute name="fo:color">
                    <xsl:call-template name="getColorCode">
                      <xsl:with-param name="color">
                        <xsl:value-of select="$deftxPr/a:solidFill/a:schemeClr/@val"/>
                      </xsl:with-param>
                      <xsl:with-param name="lumMod">
                        <xsl:value-of select="$deftxPr/a:solidFill/a:schemeClr/a:lumMod/@val"/>
                      </xsl:with-param>
                      <xsl:with-param name="lumOff">
                        <xsl:value-of select="$deftxPr/a:solidFill/a:schemeClr/a:lumOff/@val"/>
                      </xsl:with-param>
                    </xsl:call-template>
                  </xsl:attribute>
                </xsl:when>
              </xsl:choose>
            </xsl:when>
          </xsl:choose>
        </xsl:when>
      <!-- style color -->
      <xsl:when
        test="parent::node()/parent::node()/parent::node()/parent::node()/xdr:style/a:fontRef/a:srgbClr">
        <xsl:attribute name="fo:color">
          <xsl:value-of
            select="concat('#',parent::node()/parent::node()/parent::node()/parent::node()/xdr:style/a:fontRef/a:srgbClr/@val)"
          />
        </xsl:attribute>
      </xsl:when>

      <!-- style scheme color -->
      <xsl:when
        test="parent::node()/parent::node()/parent::node()/parent::node()/xdr:style/a:fontRef/a:schemeClr">
        <xsl:for-each
          select="parent::node()/parent::node()/parent::node()/parent::node()/xdr:style/a:fontRef/a:schemeClr">
          <xsl:attribute name="fo:color">
            <xsl:call-template name="getColorCode">
              <xsl:with-param name="color">
                <xsl:value-of select="@val"/>
              </xsl:with-param>
              <xsl:with-param name="lumMod">
                <xsl:value-of select="a:lumMod/@val"/>
              </xsl:with-param>
              <xsl:with-param name="lumOff">
                <xsl:value-of select="a:lumOff/@val"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:attribute>
        </xsl:for-each>
      </xsl:when>
    </xsl:choose>
  <!-- style:text-underline-style -->
    <xsl:choose>
      <xsl:when test="$rPr[@u != 'none'] ">
        <xsl:call-template name="tmpUnderlineStyle">
          <xsl:with-param name="node" select="$rPr"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="$defRPr[@u != 'none'] ">
        <xsl:call-template name="tmpUnderlineStyle">
          <xsl:with-param name="node" select="$defRPr"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="$deftxPr[@u != 'none'] ">
        <xsl:call-template name="tmpUnderlineStyle">
          <xsl:with-param name="node" select="$deftxPr"/>
        </xsl:call-template>
      </xsl:when>
    </xsl:choose>
    <!-- strike-through -->
    <xsl:choose>
      <!--Sonata: Defect:2183805 -->
      <xsl:when test="$rPr[@strike = 'noStrike'] "/>
      <xsl:when test="$rPr[@strike != 'noStrike'] ">
        <xsl:attribute name="style:text-line-through-style">
          <xsl:text>solid</xsl:text>
        </xsl:attribute>
        <xsl:choose>
          <xsl:when test="$rPr[@strike = 'sngStrike'] ">
            <xsl:attribute name="style:text-line-through-type">
              <xsl:value-of select="'single'"/>
            </xsl:attribute>
          </xsl:when>
          <xsl:when test="$rPr[@strike = 'dblStrike'] ">
            <xsl:attribute name="style:text-line-through-type">
              <xsl:text>double</xsl:text>
            </xsl:attribute>
          </xsl:when>
        </xsl:choose>
        </xsl:when>
      <xsl:when test="$defRPr[@strike = 'noStrike'] "/>
      <xsl:when test="$defRPr[@strike != 'noStrike'] ">
          <xsl:attribute name="style:text-line-through-style">
            <xsl:text>solid</xsl:text>
          </xsl:attribute>
          <xsl:choose>
            <xsl:when test="$defRPr[@strike = 'sngStrike'] ">
              <xsl:attribute name="style:text-line-through-type">
                <xsl:value-of select="'single'"/>
              </xsl:attribute>
            </xsl:when>
            <xsl:when test="$defRPr[@strike = 'dblStrike'] ">
              <xsl:attribute name="style:text-line-through-type">
                <xsl:text>double</xsl:text>
              </xsl:attribute>
            </xsl:when>
          </xsl:choose>
        </xsl:when>
      <xsl:when test="$deftxPr[@strike = 'noStrike'] "/>
      <xsl:when test="$deftxPr[@strike != 'noStrike'] ">
        <xsl:attribute name="style:text-line-through-style">
          <xsl:text>solid</xsl:text>
        </xsl:attribute>
        <xsl:choose>
          <xsl:when test="$deftxPr[@strike = 'sngStrike'] ">
            <xsl:attribute name="style:text-line-through-type">
              <xsl:value-of select="'single'"/>
            </xsl:attribute>
          </xsl:when>
          <xsl:when test="$deftxPr[@strike = 'dblStrike'] ">
            <xsl:attribute name="style:text-line-through-type">
              <xsl:text>double</xsl:text>
            </xsl:attribute>
          </xsl:when>
        </xsl:choose>
      </xsl:when>
    </xsl:choose>
    <!-- text position -->
    <xsl:choose>
      <xsl:when test="$rPr[@baseline != 0]">
        <xsl:attribute name="style:text-position">
          <xsl:value-of select="concat($rPr/@baseline div 1000,'%' )"/>
          <xsl:text> 75%</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <xsl:when test="$defRPr[@baseline != 0]">
        <xsl:attribute name="style:text-position">
          <xsl:value-of select="concat($defRPr/@baseline div 1000,'%' )"/>
          <xsl:text> 75%</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <xsl:when test="$deftxPr[@baseline != 0]">
        <xsl:attribute name="style:text-position">
          <xsl:value-of select="concat($deftxPr/@baseline div 1000,'%' )"/>
          <xsl:text> 75%</xsl:text>
        </xsl:attribute>
      </xsl:when>
    </xsl:choose>
    <!-- character spacing -->
    <xsl:choose>
      <xsl:when test="$rPr/@spc">
        <xsl:attribute name="fo:letter-spacing">
          <xsl:variable name="length" select="$rPr/@spc"/>
          <xsl:value-of select="concat(format-number($length * 2.54 div 7200,'0.###'),'cm')"/>
        </xsl:attribute>
      </xsl:when>
      <xsl:when test="$defRPr/@spc">
        <xsl:attribute name="fo:letter-spacing">
          <xsl:variable name="length" select="$defRPr/@spc"/>
          <xsl:value-of select="concat(format-number($length * 2.54 div 7200,'0.###'),'cm')"/>
        </xsl:attribute>
      </xsl:when>
      <xsl:when test="$deftxPr/@spc">
        <xsl:attribute name="fo:letter-spacing">
          <xsl:variable name="length" select="$deftxPr/@spc"/>
          <xsl:value-of select="concat(format-number($length * 2.54 div 7200,'0.###'),'cm')"/>
        </xsl:attribute>
      </xsl:when>
    </xsl:choose>
    <!--Kerning true or false -->
    <xsl:choose>
      <xsl:when test="$rPr[@kern != 0]">
        <xsl:attribute name="style:letter-kerning">
          <xsl:text>true</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <xsl:when test="$defRPr[@kern != 0]">
        <xsl:attribute name="style:letter-kerning">
          <xsl:text>true</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <xsl:when test="$deftxPr[@kern != 0]">
        <xsl:attribute name="style:letter-kerning">
          <xsl:text>true</xsl:text>
        </xsl:attribute>
      </xsl:when>
    </xsl:choose>
    <!--Shadow fo:text-shadow-->
    <xsl:choose>
      <xsl:when test="$rPr/a:effectLst/a:outerShdw">
        <xsl:attribute name="fo:text-shadow">
          <xsl:text>1pt 1pt</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <xsl:when test="$defRPr/a:effectLst/a:outerShdw">
        <xsl:attribute name="fo:text-shadow">
          <xsl:text>1pt 1pt</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <xsl:when test="$deftxPr/a:effectLst/a:outerShdw">
        <xsl:attribute name="fo:text-shadow">
          <xsl:text>1pt 1pt</xsl:text>
        </xsl:attribute>
      </xsl:when>
    </xsl:choose>
   
  </xsl:template>
  <xsl:template name="tmpChartFontName">
    <xsl:param name="node"/>
    <xsl:for-each select="$node">
      <xsl:choose>
        <xsl:when test="a:latin/@typeface">
          <xsl:attribute name ="fo:font-family">
            <xsl:variable name ="typeFaceVal" select ="a:latin/@typeface"/>
            <xsl:for-each select ="a:latin/@typeface">
              <xsl:choose>
                <xsl:when test="$typeFaceVal='+mn-lt' or $typeFaceVal='+mj-lt' ">
                  <xsl:value-of  select ="'Calibri'"/>
                </xsl:when>
                <xsl:when test="not($typeFaceVal='+mn-lt' or $typeFaceVal='+mj-lt')">
                  <xsl:value-of select ="."/>
                </xsl:when>
              </xsl:choose>
            </xsl:for-each>
          </xsl:attribute>
        </xsl:when>
        <xsl:when test ="a:ea/@typeface">
          <xsl:attribute name ="fo:font-family">
            <xsl:variable name ="typeFaceVal" select ="a:ea/@typeface"/>
            <xsl:for-each select ="a:ea/@typeface">
              <xsl:choose>
                <xsl:when test="$typeFaceVal='+mn-lt' or $typeFaceVal='+mj-lt' ">
                  <xsl:value-of  select ="'Calibri'"/>
                </xsl:when>
                <xsl:when test="not($typeFaceVal='+mn-lt' or $typeFaceVal='+mj-lt')">
                  <xsl:value-of select ="."/>
                </xsl:when>
              </xsl:choose>
            </xsl:for-each>
          </xsl:attribute>
        </xsl:when>
        <xsl:when test ="a:sym/@typeface">
          <xsl:attribute name ="fo:font-family">
            <xsl:variable name ="typeFaceVal" select ="a:sym/@typeface"/>
            <xsl:for-each select ="a:sym/@typeface">
              <xsl:choose>
                <xsl:when test="$typeFaceVal='+mn-lt' or $typeFaceVal='+mj-lt' ">
                  <xsl:value-of  select ="'Calibri'"/>
                </xsl:when>
                <xsl:when test="not($typeFaceVal='+mn-lt' or $typeFaceVal='+mj-lt')">
                  <xsl:value-of select ="."/>
                </xsl:when>
              </xsl:choose>
            </xsl:for-each>
          </xsl:attribute>
        </xsl:when>
        <xsl:when test ="a:cs/@typeface">
          <xsl:attribute name ="fo:font-family">
            <xsl:variable name ="typeFaceVal" select ="a:cs/@typeface"/>
            <xsl:for-each select ="a:cs/@typeface">
              <xsl:choose>
                <xsl:when test="$typeFaceVal='+mn-lt' or $typeFaceVal='+mj-lt' ">
                  <xsl:value-of  select ="'Calibri'"/>
                </xsl:when>
                <xsl:when test="not($typeFaceVal='+mn-lt' or $typeFaceVal='+mj-lt')">
                  <xsl:value-of select ="."/>
                </xsl:when>
              </xsl:choose>
            </xsl:for-each>
          </xsl:attribute>
        </xsl:when>
      </xsl:choose>
    </xsl:for-each>
  </xsl:template>
  <xsl:template name="tmpUnderlineStyle">
    <xsl:param name="node"/>
    <xsl:for-each select="$node">
      <!-- underline style -->
      <xsl:choose>
        <xsl:when test="@u = 'sng' ">
          <xsl:attribute name="style:text-underline-style">
            <xsl:text>solid</xsl:text>
          </xsl:attribute>
          <xsl:attribute name="style:text-underline-type">
            <xsl:text>single</xsl:text>
          </xsl:attribute>
          <xsl:attribute name="style:text-underline-width">
            <xsl:text>auto</xsl:text>
          </xsl:attribute>
        </xsl:when>

        <!-- double -->
        <xsl:when test="@u = 'dbl' ">
          <xsl:attribute name="style:text-underline-style">
            <xsl:text>solid</xsl:text>
          </xsl:attribute>
          <xsl:attribute name="style:text-underline-type">
            <xsl:text>double</xsl:text>
          </xsl:attribute>
          <xsl:attribute name="style:text-underline-width">
            <xsl:text>auto</xsl:text>
          </xsl:attribute>
        </xsl:when>

        <!-- bold -->
        <xsl:when test="@u= 'heavy' ">
          <xsl:attribute name="style:text-underline-style">
            <xsl:text>solid</xsl:text>
          </xsl:attribute>
          <xsl:attribute name="style:text-underline-width">
            <xsl:text>bold</xsl:text>
          </xsl:attribute>
        </xsl:when>

        <!-- dotted-->
        <xsl:when test="@u= 'dotted' ">
          <xsl:attribute name="style:text-underline-style">
            <xsl:text>dotted</xsl:text>
          </xsl:attribute>
          <xsl:attribute name="style:text-underline-width">
            <xsl:text>auto</xsl:text>
          </xsl:attribute>
        </xsl:when>

        <!-- bold dotted -->
        <xsl:when test="@u = 'dottedHeavy' ">
          <xsl:attribute name="style:text-underline-style">
            <xsl:text>dotted</xsl:text>
          </xsl:attribute>
          <xsl:attribute name="style:text-underline-width">
            <xsl:text>bold</xsl:text>
          </xsl:attribute>
        </xsl:when>

        <!-- dash -->
        <xsl:when test="@u = 'dash' ">
          <xsl:attribute name="style:text-underline-style">
            <xsl:text>dash</xsl:text>
          </xsl:attribute>
          <xsl:attribute name="style:text-underline-width">
            <xsl:text>auto</xsl:text>
          </xsl:attribute>
        </xsl:when>

        <!-- bold dash -->
        <xsl:when test="@u = 'dashHeavy' ">
          <xsl:attribute name="style:text-underline-style">
            <xsl:text>dash</xsl:text>
          </xsl:attribute>
          <xsl:attribute name="style:text-underline-width">
            <xsl:text>bold</xsl:text>
          </xsl:attribute>
        </xsl:when>

        <!-- long dash -->
        <xsl:when test="@u = 'dashLong' ">
          <xsl:attribute name="style:text-underline-style">
            <xsl:text>long-dash</xsl:text>
          </xsl:attribute>
          <xsl:attribute name="style:text-underline-width">
            <xsl:text>auto</xsl:text>
          </xsl:attribute>
        </xsl:when>

        <!-- bold long dash -->
        <xsl:when test="@u = 'dashLongHeavy' ">
          <xsl:attribute name="style:text-underline-style">
            <xsl:text>long-dash</xsl:text>
          </xsl:attribute>
          <xsl:attribute name="style:text-underline-width">
            <xsl:text>bold</xsl:text>
          </xsl:attribute>
        </xsl:when>

        <!-- dot-dash -->
        <xsl:when test="@u = 'dotDash' ">
          <xsl:attribute name="style:text-underline-style">
            <xsl:text>dot-dash</xsl:text>
          </xsl:attribute>
          <xsl:attribute name="style:text-underline-width">
            <xsl:text>auto</xsl:text>
          </xsl:attribute>
        </xsl:when>

        <!-- bold dot-dash -->
        <xsl:when test="@u = 'dotDashHeavy' ">
          <xsl:attribute name="style:text-underline-style">
            <xsl:text>dot-dash</xsl:text>
          </xsl:attribute>
          <xsl:attribute name="style:text-underline-width">
            <xsl:text>bold</xsl:text>
          </xsl:attribute>
        </xsl:when>

        <!-- dot-dot-dash -->
        <xsl:when test="@u = 'dotDotDash' ">
          <xsl:attribute name="style:text-underline-style">
            <xsl:text>dot-dot-dash</xsl:text>
          </xsl:attribute>
          <xsl:attribute name="style:text-underline-width">
            <xsl:text>auto</xsl:text>
          </xsl:attribute>
        </xsl:when>

        <!-- bold dot-dot-dash -->
        <xsl:when test="@u = 'dotDotDashHeavy' ">
          <xsl:attribute name="style:text-underline-style">
            <xsl:text>dot-dot-dash</xsl:text>
          </xsl:attribute>
          <xsl:attribute name="style:text-underline-width">
            <xsl:text>bold</xsl:text>
          </xsl:attribute>
        </xsl:when>

        <!-- wavy -->
        <xsl:when test="@u = 'wavy' ">
          <xsl:attribute name="style:text-underline-style">
            <xsl:text>wave</xsl:text>
          </xsl:attribute>
          <xsl:attribute name="style:text-underline-width">
            <xsl:text>auto</xsl:text>
          </xsl:attribute>
        </xsl:when>

        <!-- bold wave -->
        <xsl:when test="@u = 'wavyHeavy' ">
          <xsl:attribute name="style:text-underline-style">
            <xsl:text>wave</xsl:text>
          </xsl:attribute>
          <xsl:attribute name="style:text-underline-width">
            <xsl:text>bold</xsl:text>
          </xsl:attribute>
        </xsl:when>

        <!-- double wave -->
        <xsl:when test="@u = 'wavyDbl' ">
          <xsl:attribute name="style:text-underline-style">
            <xsl:text>wave</xsl:text>
          </xsl:attribute>
          <xsl:attribute name="style:text-underline-type">
            <xsl:text>double</xsl:text>
          </xsl:attribute>
          <xsl:attribute name="style:text-underline-width">
            <xsl:text>auto</xsl:text>
          </xsl:attribute>
        </xsl:when>

        <xsl:otherwise>
          <xsl:attribute name="style:text-underline-type">
            <xsl:text>single</xsl:text>
          </xsl:attribute>
          <xsl:attribute name="style:text-underline-width">
            <xsl:text>auto</xsl:text>
          </xsl:attribute>
        </xsl:otherwise>
      </xsl:choose>

      <!-- underline color -->
      <xsl:if test="a:uFill/a:solidFill/a:srgbClr/@val">
        <xsl:attribute name="style:text-underline-color">
          <xsl:value-of select="concat('#',a:uFill/a:solidFill/a:srgbClr/@val)"/>
        </xsl:attribute>
      </xsl:if>
      <xsl:if test="a:uFill/a:solidFill/a:schemeClr/@val">
        <xsl:attribute name="style:text-underline-color">
          <xsl:call-template name="getColorCode">
            <xsl:with-param name="color">
              <xsl:value-of select="a:uFill/a:solidFill/a:schemeClr/@val"/>
            </xsl:with-param>
            <xsl:with-param name="lumMod">
              <xsl:value-of select="a:uFill/a:solidFill/a:schemeClr/a:lumMod/@val"/>
            </xsl:with-param>
            <xsl:with-param name="lumOff">
              <xsl:value-of select="a:uFill/a:solidFill/a:schemeClr/a:lumOff/@val"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:attribute>
      </xsl:if>
    </xsl:for-each>
	</xsl:template>

	<!--SP2,Scenario:ods_SP2_xlsx_2.5_ods,ChartLost because SP2 created xlsx has oneCellAnchor in Place of twoCellAnchor-->
	<xsl:template name="CalculateRowHeight">
		<xsl:param name="sheet"/>
		<xsl:param name="startrowno"/>
		<xsl:param name="endrowno"/>
		<xsl:param name="total"/>
		<xsl:param name="defaultrowheight"/>
		<xsl:param name="countchartrow"/>
		<xsl:variable name="totalrowheight">
			<xsl:choose>
				<xsl:when test="($countchartrow &gt;= $startrowno+1) and ($countchartrow &lt;= $endrowno)">
					<xsl:choose>
						<xsl:when test="parent::node()/child::node()[name()='sheetData']/child::node()/@r=$countchartrow">
							<xsl:choose>
								<xsl:when test="parent::node()/child::node()[name()='sheetData']/child::node()[@r=$countchartrow]/@ht">
									<!--<xsl:when test="parent::node()/child::node()[name()='sheetData']/child::node()/@ht">-->
									<!--<xsl:variable name="customrowheight" select="parent::node()/child::node()[name()='sheetData']/child::node()/@ht"/>-->
									<xsl:variable name="customrowheight" select="parent::node()/child::node()[name()='sheetData']/child::node()[@r=$countchartrow]/@ht"/>
									<xsl:value-of select="$total + $customrowheight"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="$total+$defaultrowheight"/>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="$total+$defaultrowheight"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$total"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="($countchartrow &gt;= $startrowno+1) and ($countchartrow &lt; $endrowno -1)">
				<xsl:call-template name="CalculateRowHeight">
					<xsl:with-param name="sheet" select="$sheet"/>
					<xsl:with-param name="startrowno" select="$startrowno"/>
					<xsl:with-param name="endrowno" select="$endrowno"/>
					<xsl:with-param name="total" select="$totalrowheight"/>
					<xsl:with-param name="defaultrowheight" select="$defaultrowheight"/>
					<xsl:with-param name="countchartrow" select="$countchartrow + 1"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$totalrowheight"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="CalculateColWidth">
		<xsl:param name="sheet"/>
		<xsl:param name="startcolno"/>
		<xsl:param name="endcolno"/>
		<xsl:param name="total"/>
		<xsl:param name="defaultcolWidth"/>
		<xsl:param name="countchartcol"/>
		<xsl:param name="totalDefaultColCount"/>
		<xsl:param name="startcoloffset"/>
		<xsl:param name="endcoloffset"/>
		<xsl:variable name="totalCustomColWidth">
			<xsl:choose>
				<xsl:when test="($countchartcol &gt;= $startcolno + 1) and ($countchartcol &lt;= $endcolno)">
					<xsl:choose>
						<xsl:when test="parent::node()/child::node()[name()='cols']">
							<xsl:variable name="customColWidth">
								<xsl:for-each select="parent::node()/child::node()[name()='cols']/child::node()[($countchartcol &gt; @min or $countchartcol=@min) and ($countchartcol &lt; @max or $countchartcol =@max)]">
									<xsl:choose>
										<xsl:when test="($countchartcol &gt; @min or $countchartcol=@min) and ($countchartcol &lt; @max or $countchartcol =@max)">
											<xsl:choose>
												<xsl:when test="@width">
													<xsl:value-of select="@width"/>
												</xsl:when>
											</xsl:choose>
										</xsl:when>
										<xsl:otherwise>
											<xsl:value-of select="$total"/>
										</xsl:otherwise>
									</xsl:choose>
								</xsl:for-each>
							</xsl:variable>
							<xsl:if test ="$customColWidth!=''">
								<xsl:value-of select="$total + $customColWidth"/>
							</xsl:if>
							<xsl:if test ="$customColWidth = ''">
								<xsl:value-of select="$total"/>
							</xsl:if>
						</xsl:when>
					</xsl:choose>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="countDefaultColWidth">
			<xsl:variable name ="flag">
				<xsl:choose>
					<xsl:when test="($countchartcol &gt;= $startcolno + 1) and ($countchartcol &lt;= $endcolno)">
						<xsl:choose>
							<xsl:when test="parent::node()/child::node()[name()='cols']">
								<xsl:for-each select="parent::node()/child::node()[name()='cols']/child::node()">
									<!-- NewCode-->
									<xsl:choose >
										<xsl:when test="($countchartcol &gt;= @min) and ($countchartcol &lt;= @max)">
											<xsl:value-of select ="'true'"/>
										</xsl:when>
										<xsl:when test="($countchartcol &lt;= @min)">
											<xsl:value-of select ="'false'"/>
										</xsl:when>
										<xsl:otherwise>
											<xsl:value-of select ="'false'"/>
										</xsl:otherwise>
									</xsl:choose>

									<!-- NewCode-->
								</xsl:for-each>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="$endcolno -($startcolno)"/>
							</xsl:otherwise>
						</xsl:choose>

					</xsl:when>
				</xsl:choose>
			</xsl:variable>
			<xsl:choose>
				<xsl:when test ="contains($flag,'true')">
					<xsl:value-of select ="concat($totalDefaultColCount,'false|')"/>
				</xsl:when>
				<xsl:when test ="not(contains($flag,'true'))">
					<xsl:value-of select ="concat($totalDefaultColCount,'true|')"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>
		<xsl:choose>

			<xsl:when test="($countchartcol &gt;= $startcolno + 1) and ($countchartcol &lt; $endcolno -1)">
				<xsl:call-template name="CalculateColWidth">
					<xsl:with-param name="sheet" select="$sheet"/>
					<xsl:with-param name="startcolno" select="$startcolno"/>
					<xsl:with-param name="endcolno" select="$endcolno"/>
					<xsl:with-param name="total">
						<xsl:if test="$totalCustomColWidth != 0 ">
							<xsl:value-of select="$totalCustomColWidth"/>
						</xsl:if>
						<xsl:if test="$totalCustomColWidth = 0 ">
							<xsl:value-of select="0"/>
						</xsl:if>
					</xsl:with-param>
					<xsl:with-param name="totalDefaultColCount" select="$countDefaultColWidth"/>
					<xsl:with-param name="defaultcolWidth" select="$defaultcolWidth"/>
					<xsl:with-param name="countchartcol" select="$countchartcol + 1"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:variable name="finalCount">
					<xsl:call-template name="increaseCount">
						<xsl:with-param name ="totalCount" select ="0"/>
						<xsl:with-param name="countDefaultColWidth" select ="$countDefaultColWidth"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:value-of select="concat($totalCustomColWidth,'|',$finalCount)"/>
			</xsl:otherwise>
		</xsl:choose>
  </xsl:template>

	<xsl:template name="increaseCount">
		<xsl:param name ="totalCount"/>
		<xsl:param name="countDefaultColWidth"/>
		<xsl:variable name ="totalNo">
			<xsl:value-of select ="$totalCount"/>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test ="$countDefaultColWidth!='' and substring-before($countDefaultColWidth,'|')='true'">
				<xsl:call-template name ="increaseCount">
					<xsl:with-param name ="totalCount">
						<xsl:value-of select ="$totalNo+1"/>
					</xsl:with-param>
					<xsl:with-param name ="countDefaultColWidth" select ="substring-after($countDefaultColWidth,'|')"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test ="$countDefaultColWidth!='' and substring-before($countDefaultColWidth,'|')='false'">
				<xsl:call-template name ="increaseCount">
					<xsl:with-param name ="totalCount">
						<xsl:value-of select ="$totalNo"/>
					</xsl:with-param>
					<xsl:with-param name ="countDefaultColWidth" select ="substring-after($countDefaultColWidth,'|')"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$totalNo"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<!--SP2,Scenario:ods_SP2_xlsx_2.5_ods,End-->
</xsl:stylesheet>
