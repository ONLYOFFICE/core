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
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
  xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main"
  xmlns:pzip="urn:cleverage:xmlns:post-processings:zip"
  xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:number="urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0"
  xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
  xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
  xmlns:v="urn:schemas-microsoft-com:vml" xmlns:o="urn:schemas-microsoft-com:office:office"
  xmlns:x="urn:schemas-microsoft-com:office:excel" xmlns:dc="http://purl.org/dc/elements/1.1/"
  exclude-result-prefixes="svg table r text style number fo">

  <!--<xsl:import href="sharedStrings.xsl"/>
  <xsl:import href="styles.xsl"/>-->

  <xsl:template name="comments">
    <xsl:param name="sheetNum"/>
    <comments xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
      <authors>
        <author>Author</author>
      </authors>
      <commentList>
        <xsl:variable name="noteId">
          <xsl:number value="0"/>
        </xsl:variable>
        <xsl:apply-templates
          select="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:table[$sheetNum]/descendant::table:table-row/table:table-cell/office:annotation">
          <xsl:with-param name="noteId" select="$noteId+1"/>
        </xsl:apply-templates>
      </commentList>
    </comments>
  </xsl:template>

  <xsl:template match="office:annotation">
    <xsl:param name="noteId"/>
    <comment authorId="0" noteId="{count(preceding::office:annotation)+1}">
      <text>
        <xsl:call-template name="InsertCommentText"/>
      </text>
    </comment>
  </xsl:template>

  <xsl:template name="InsertTextBox">
     <!--  Defect Id   : 1802491,1788390 and 1797047 
		   Code Changed by :Vijayeta Tilak
		   Date            :15th Oct '07
		   Description     :If attribute office:display is not present, then the visibility is set to true
     -->
    <!-- Defect Id :  1838531
      Code changed by: Tomasz Mueller
      Date:  30th Jan '08
      Description: if attribute office:display is not present then the visibility is set to false
    -->
    <xsl:variable name="VisibleOrHidden">
      <xsl:choose>
        <xsl:when test="@office:display and @office:display != ''">
          <xsl:if test ="@office:display='true'">
            <xsl:value-of select ="'visible'"/>
          </xsl:if>
          <xsl:if test ="@office:display='false'">
            <xsl:value-of select ="'hidden'"/>
          </xsl:if>
          <!--<xsl:text>visible</xsl:text>-->
        </xsl:when>
        <xsl:otherwise>
          <!--<xsl:text>hidden</xsl:text>-->
          <xsl:value-of select ="'hidden'"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <!--End of code by Vijayeta, fix for bugs, 1802491,1788390 and 1797047 date:15th Oct '07-->
    <xsl:variable name="id">
      <xsl:value-of select="1025 + count(preceding::office:annotation) + 1"/>
    </xsl:variable>

    <xsl:variable name="height">
      <xsl:call-template name="point-measure">
        <xsl:with-param name="length" select="@svg:height"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="width">
      <xsl:call-template name="point-measure">
        <xsl:with-param name="length" select="@svg:width"/>
      </xsl:call-template>
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

    <xsl:variable name="z-index">
      <xsl:value-of select="position()"/>
    </xsl:variable>

    <v:shape noteId="{count(preceding::office:annotation)+1}" id="{concat('_x0000_s',$id)}"
		  type="#_x0000_t202"
		  style="position:absolute;
        margin-left:{$margin-left}pt;margin-top:{$margin-top};width:{$width}pt;height:{$height}pt;z-index:{$z-index};
        visibility:{$VisibleOrHidden};mso-wrap-style:none;v-text-anchor:middle"
		  fillcolor="#ffffc0" o:insetmode="auto">

      <v:fill color2="#00003f"/>
      <v:stroke startarrow="block" joinstyle="round"/>
      <v:shadow on="t" color="black" obscured="t"/>
      <v:path arrowok="t" o:connecttype="none"/>
      <v:textbox>
        <div style="text-align:left"/>
      </v:textbox>
      <x:ClientData ObjectType="Note">

        <x:SizeWithCells/>

        <x:Locked>False</x:Locked>
        <x:PrintObject>False</x:PrintObject>
        <x:AutoFill>False</x:AutoFill>
        <x:AutoLine>False</x:AutoLine>
        <x:LockText>False</x:LockText>
        <!--  <x:Row>2</x:Row>
          <x:Column>2</x:Column>-->
		  <!--  Defect Id   : 1802491,1788390 and 1797047 
		   Code Changed by :Vijayeta Tilak
		   Date            :15th Oct '07
		   Description     :If attribute office:display is not present, then the visibility is set to true
		  -->
        <!-- Defect Id :  1838531
          Code changed by: Tomasz Mueller
          Date:  30th Jan '08
          Description: if attribute office:display is not present then the visibility is set to false
        -->
        <xsl:if test="@office:display='true'">
          <x:Visible/>
        </xsl:if>
        <xsl:if test="@office:display='false' or not(@office:display)">
          <x:Hidden/>
        </xsl:if>
        <!--<xsl:if test="@office:display">
            <x:Visible/>
          </xsl:if>-->
        <!--End of code by Vijayeta, fix for bugs, 1802491,1788390 and 1797047 date:15th Oct '07-->
      </x:ClientData>
    </v:shape>
  </xsl:template>

  <xsl:template match="dc:date"/>

  <xsl:template name="InsertCommentText">

    <xsl:choose>
      <xsl:when test="text:span|text:p/text:span|text:p/text:span/text:a">
        <xsl:apply-templates mode="run"/>
      </xsl:when>
      
      <xsl:otherwise>
        <t xml:space="preserve"><xsl:apply-templates mode="text"/></t>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

</xsl:stylesheet>
