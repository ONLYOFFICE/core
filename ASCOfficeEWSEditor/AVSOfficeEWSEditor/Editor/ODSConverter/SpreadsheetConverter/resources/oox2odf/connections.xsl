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
    xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
    xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
    xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
    xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
    xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
    xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
    xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
    xmlns:number="urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0"
    xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:dc="http://purl.org/dc/elements/1.1/"
    xmlns:e="http://schemas.openxmlformats.org/spreadsheetml/2006/main" exclude-result-prefixes="e r">
    
    <!-- Get cell when the connectionl is starting and ending -->
    <xsl:template name="ConnectionsCell">
        <xsl:param name="number"/>
        <xsl:for-each select="ancestor::e:workbook/e:definedNames">
            <xsl:apply-templates select="e:definedName[1]" mode="connections">
                <xsl:with-param name="number">
                    <xsl:value-of select="$number"/>
                </xsl:with-param>
            </xsl:apply-templates>
        </xsl:for-each>
    </xsl:template>
    
    <xsl:template match="e:definedName" mode="Connections">
        <xsl:param name="number"/>
        <xsl:param name="ConnectionsCell"/>

        <xsl:choose>
            <xsl:when test="not(contains(@name, '_xlnm.Print_Titles')) and not(contains(@name, '_xlnm.Print_Area')) and @localSheetId = $number">
                
                <xsl:variable name="Value">
                    <xsl:value-of select="."/>
                </xsl:variable>
                
                <xsl:variable name="RowStart">
                    <xsl:value-of select="substring-before(substring-after(substring-after($Value, '$'), '$'), ':')"/>
                </xsl:variable>
                
                <xsl:variable name="ColStart">
                    <xsl:call-template name="GetColNum">
                        <xsl:with-param name="cell">
                            <xsl:value-of select="concat(substring-before(substring-after($Value, '$'), '$'), $RowStart)"/>
                        </xsl:with-param>
                    </xsl:call-template>
                </xsl:variable>
                
                <xsl:variable name="RowEnd">
                    <xsl:value-of select="substring-after(substring-after(substring-after($Value, ':'), '$'), '$')"/>
                </xsl:variable>
                
                <xsl:variable name="ColEnd">
                    <xsl:call-template name="GetColNum">
                        <xsl:with-param name="cell">
                            <xsl:value-of select="concat(substring-before(substring-after(substring-after($Value, ':'), '$'), '$'), $RowEnd)"/>
                        </xsl:with-param>
                    </xsl:call-template>
                </xsl:variable>
                
                <xsl:variable name="name">
                    <xsl:value-of select="@name"/>
                </xsl:variable>
                
                <xsl:variable name="ConnectionsCellResult">
                    <xsl:value-of select="concat($RowStart, ':', $ColStart, '-', $RowEnd, ':', $ColEnd, '=', $name, ';')"/>
                </xsl:variable>

                <xsl:choose>
                    <xsl:when test="following-sibling::e:definedName">
                        <xsl:apply-templates select="following-sibling::e:definedName[1]" mode="connections">
                            <xsl:with-param name="number">
                                <xsl:value-of select="$number"/>                            
                            </xsl:with-param>
                            <xsl:with-param name="ConnectionsCell">                            
                                <xsl:value-of select="$ConnectionsCellResult"/>
                            </xsl:with-param>
                        </xsl:apply-templates>
                    </xsl:when>
                    <xsl:otherwise>
                        <xsl:value-of select="concat($ConnectionsCell, $ConnectionsCellResult)"/>
                    </xsl:otherwise>
                </xsl:choose>
                
            </xsl:when>
            <xsl:otherwise>
                
                <xsl:choose>
                    <xsl:when test="following-sibling::e:definedName">
                        <xsl:apply-templates select="following-sibling::e:definedName[1]" mode="connections">
                            <xsl:with-param name="number">
                                <xsl:value-of select="$number"/>                            
                            </xsl:with-param>
                            <xsl:with-param name="ConnectionsCell">                            
                                <xsl:value-of select="$ConnectionsCell"/>
                            </xsl:with-param>
                        </xsl:apply-templates>        
                    </xsl:when>
                    <xsl:otherwise>
                        <xsl:value-of select="$ConnectionsCell"/>
                    </xsl:otherwise>
                </xsl:choose>
                
            </xsl:otherwise>
        </xsl:choose>
        
    </xsl:template>
    
    <!-- Insert Connection -->
    <xsl:template name="InsertConnections">
        <xsl:param name="rowNum"/>
        <xsl:param name="colNum"/>
        <xsl:param name="ConnectionsCell"/>
        <xsl:param name="sheetNr"/>
        
        <xsl:variable name="EndRow">
            <xsl:value-of select="substring-before(substring-after(concat(';', $ConnectionsCell), concat(';', $rowNum, ':', $colNum, '-')), ':')"/>
        </xsl:variable>
        
        <xsl:variable name="EndCol">
            <xsl:value-of select="substring-before(substring-after(substring-after(concat(';', $ConnectionsCell), concat(';', $rowNum, ':', $colNum, '-')), ':'), '=')"/>
        </xsl:variable>
        
        <xsl:variable name="ConnectionName">
            <xsl:value-of select="substring-before(substring-after(substring-after(concat(';', $ConnectionsCell), concat(';', $rowNum, ':', $colNum, '-')), '='), ';')"/>
        </xsl:variable>
      
     
        <table:cell-range-source table:filter-name="calc_HTML_WebQuery">
            
              <xsl:attribute name="table:last-row-spanned">
                   <xsl:value-of select="$EndRow - $rowNum + 1"/>         
              </xsl:attribute>
            
            <xsl:attribute name="table:last-column-spanned">
                <xsl:value-of select="$EndCol - $colNum + 1"/>
            </xsl:attribute>
  
            <xsl:call-template name="ConnectionProperties">
                <xsl:with-param name="sheetNr">
                    <xsl:value-of select="$sheetNr"/>
                </xsl:with-param>
                <xsl:with-param name="ConnectionName">
                    <xsl:value-of select="$ConnectionName"/>
                </xsl:with-param>
            </xsl:call-template>
            
        </table:cell-range-source>

    </xsl:template>
    
    <!-- Insert connection properties -->
    <xsl:template name="ConnectionProperties">
        <xsl:param name="sheetNr"/>
        <xsl:param name="ConnectionName"/>

        
        <xsl:variable name="RelsName">            
            <xsl:value-of select="concat('sheet', $sheetNr, '.xml.rels')"/>
        </xsl:variable>
        <xsl:variable name="PathRels">
            <xsl:value-of select="concat('xl/worksheets/_rels/', $RelsName)"/>
        </xsl:variable>
      
        <xsl:for-each select="key('Part', $PathRels)//node()[name() = 'Relationship' and contains(@Type,'queryTable')]"> 
            <xsl:for-each select="key('Part', concat('xl/', substring-after(@Target, '/')))//node()[name() = 'queryTable']">
                <xsl:if test="@name = $ConnectionName">
                    
                    <xsl:variable name="ConnectionId">
                        <xsl:value-of select="@connectionId"/>        
                    </xsl:variable>
                   
                    <xsl:for-each select="key('Part', 'xl/connections.xml')//node()[name() = 'connection']">
                        <xsl:if test="@id = $ConnectionId">
                            <xsl:attribute name="xlink:href">
                                <xsl:value-of select="e:webPr/@url"/>
                            </xsl:attribute>
                            <xsl:choose>
                                <xsl:when test="e:webPr/e:tables/e:x">
                                    <xsl:attribute name="table:name">
                                        <xsl:apply-templates select="e:webPr/e:tables/e:x[1]"/>
                                   </xsl:attribute>
                                </xsl:when>
                                <xsl:when test="not(e:webPr/e:tables/e:x)">
                                    <xsl:attribute name="table:name">
                                        <xsl:apply-templates select="HTML_tables"/>
                                    </xsl:attribute>
                                </xsl:when>
                                <xsl:otherwise>
                                    <xsl:attribute name="table:name">
                                        <xsl:text>HTML_1</xsl:text>
                                    </xsl:attribute>
                                </xsl:otherwise>
                            </xsl:choose>
                        </xsl:if>
                    </xsl:for-each>
                    
                </xsl:if>
            </xsl:for-each>
        </xsl:for-each>
    </xsl:template>
    
    <!-- Insert numbers of tables  -->
    <xsl:template match="e:x">
        <xsl:param name="TableName"/>
        
        <xsl:choose>
            <xsl:when test="following-sibling::e:x">
                <xsl:apply-templates select="following-sibling::e:x[1]">
                    <xsl:with-param name="TableName">
                        <xsl:choose>
                            <xsl:when test="$TableName != ''">
                                <xsl:value-of select="concat($TableName, ';HTML_', @v)"/>
                            </xsl:when>
                            <xsl:otherwise>
                                <xsl:value-of select="concat('HTML_', @v)"/>
                            </xsl:otherwise>
                        </xsl:choose>
                    </xsl:with-param>
                </xsl:apply-templates>
            </xsl:when>
            <xsl:otherwise>
                <xsl:choose>
                    <xsl:when test="$TableName != ''">
                        <xsl:value-of select="concat($TableName, ';HTML_', @v)"/>
                    </xsl:when>
                    <xsl:otherwise>
                        <xsl:value-of select="concat('HTML_', @v)"/>
                    </xsl:otherwise>
                </xsl:choose>
            </xsl:otherwise>
        </xsl:choose>
        
        
    </xsl:template>
    
</xsl:stylesheet>