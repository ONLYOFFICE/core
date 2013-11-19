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
    xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
    xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
    xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"
    xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
    exclude-result-prefixes="#default xlink office draw text style style r"> 
    
    <!-- Insert querytable.xml.rels file -->
    <xsl:template name="QueryTable">
        <xsl:apply-templates select="table:table-row/table:table-cell/table:cell-range-source" mode="queryTable"/>
    </xsl:template>
    
    <xsl:template match="table:cell-range-source" mode="queryTable">
        <xsl:call-template name="InsertQueryTable"></xsl:call-template>
    </xsl:template>
    
    <xsl:template name="InsertQueryTable">
        <xsl:variable name="queryTableTarget">
            <xsl:value-of select="concat('xl/queryTables/queryTable', position(), '.xml')"/>
        </xsl:variable>
        <pzip:entry pzip:target="{$queryTableTarget}">
            <queryTable xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main"
                autoFormatId="16" applyNumberFormats="0" applyBorderFormats="0"
                applyFontFormats="1" applyPatternFormats="1" applyAlignmentFormats="0"
                applyWidthHeightFormats="0">
                <xsl:attribute name="name">
                    <xsl:value-of select="generate-id()"/>
                </xsl:attribute>
                <xsl:attribute name="connectionId">
                    <xsl:value-of select="position()"/>
                </xsl:attribute>
            </queryTable>
        </pzip:entry>        
    </xsl:template>
    
    <!-- Insert conections to web site -->
    <xsl:template name="InsertConnections">        
        <connections xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
            <xsl:for-each select="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:table/table:table-row/table:table-cell/table:cell-range-source">
                <connection type="4" refreshedVersion="3" background="1" saveData="1">
                    <xsl:attribute name="id">
                        <xsl:value-of select="position()"/>
                    </xsl:attribute>
                    <xsl:attribute name="name">
                        <xsl:value-of select="concat('Connection', position())"/>
                    </xsl:attribute>
                    <webPr sourceData="1" parsePre="1" consecutive="1" xl2000="1">
                        <xsl:attribute name="url">
                            <xsl:value-of select="@xlink:href"/>
                        </xsl:attribute>
                        
                        <xsl:if test="number(substring-after(@table:name,'HTML_' ))">
                            <xsl:attribute name="htmlTables">
                                <xsl:text>1</xsl:text>
                            </xsl:attribute>
                            <tables>
                                <xsl:call-template name="InsertTablesConnection">
                                    <xsl:with-param name="TableName">
                                        <xsl:value-of select="concat(@table:name, ';')"/>
                                    </xsl:with-param>
                                </xsl:call-template>
                            </tables>
                        </xsl:if>
                        
                    </webPr>
                </connection>
            </xsl:for-each>
          </connections>
    </xsl:template>
    
    <!-- Insert connections to tables -->
    <xsl:template name="InsertTablesConnection">
       <xsl:param name="TableName"/>
            <x>
                <xsl:attribute name="v">
                    <xsl:value-of select="substring-before(substring-after($TableName, '_'), ';')"/>        
                </xsl:attribute>
            </x>
        <xsl:if test="substring-after($TableName, ';') != ''">
            <xsl:call-template name="InsertTablesConnection">
                <xsl:with-param name="TableName">
                    <xsl:value-of select="substring-after($TableName, ';')"/>
                </xsl:with-param>
            </xsl:call-template>
        </xsl:if>
    </xsl:template>
    
    
</xsl:stylesheet>
