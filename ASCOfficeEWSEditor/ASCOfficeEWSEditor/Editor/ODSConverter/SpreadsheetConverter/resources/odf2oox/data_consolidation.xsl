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
    xmlns:config="urn:oasis:names:tc:opendocument:xmlns:config:1.0"
    xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
    xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:v="urn:schemas-microsoft-com:vml"
    xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
    xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
    xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
    xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0" exclude-result-prefixes="table">

    <xsl:template name="InsertDataConsolidate">
        <xsl:for-each
            select="/office:document-content/office:body/office:spreadsheet/table:consolidation">

            <xsl:if
                test="/office:document-content/office:body/office:spreadsheet/table:consolidation/@table:function">

                <xsl:variable name="ViewFunction">
                    <xsl:choose>
                        <xsl:when test="@table:function='countnums'">count</xsl:when>
                        <xsl:when test="@table:function='count'">countNums</xsl:when>
                        <xsl:when test="@table:function='stdev'">stdDev</xsl:when>
                        <xsl:when test="@table:function='stdevp'">stdDevp</xsl:when>
                        <xsl:otherwise>
                            <xsl:value-of select="@table:function"/>
                        </xsl:otherwise>
                    </xsl:choose>
                </xsl:variable>

                <dataConsolidate function="{$ViewFunction}">

                    <xsl:for-each
                        select="/office:document-content/office:body/office:spreadsheet/table:consolidation">

                        <dataRefs>
                            <xsl:if test="@table:source-cell-range-addresses and @table:target-cell-address">

                                <xsl:variable name="ViewConsolidation">
                                    <xsl:value-of select="@table:source-cell-range-addresses"/>
                                </xsl:variable>

                                <xsl:variable name="ViewTargetCellAddress">
                                    <xsl:value-of select="@table:target-cell-address"/>
                                </xsl:variable>
                                
                                <xsl:call-template name="InsertDataRef">
                                    <xsl:with-param name="ViewConsolidation">
                                        <xsl:value-of select="$ViewConsolidation"/>
                                    </xsl:with-param>
                                    <xsl:with-param name="ViewTargetCellAddress">
                                        <xsl:value-of select="$ViewTargetCellAddress"/>
                                    </xsl:with-param>
                                </xsl:call-template>

                            </xsl:if>
                        </dataRefs>
                    </xsl:for-each>

                </dataConsolidate>
            </xsl:if>
        </xsl:for-each>
    </xsl:template>

    <xsl:template name="InsertDataRef">
        <xsl:param name="ViewConsolidation"/>
        <xsl:param name="ViewTargetCellAddress"/>

        <xsl:variable name="FirstCell">
            <xsl:value-of select="substring-after(substring-before($ViewConsolidation,':'),'.')"/>
        </xsl:variable>

        <xsl:variable name="LastCell">
            <xsl:choose>
                <xsl:when
                    test="substring-before(substring-after(substring-after($ViewConsolidation,':'),'.'),' ') != ''">
                    <xsl:value-of
                        select="substring-before(substring-after(substring-after($ViewConsolidation,':'),'.'),' ')"
                    />
                </xsl:when>

                <xsl:otherwise>
                    <xsl:value-of
                        select="substring-after(substring-after($ViewConsolidation,':'),'.')"/>
                </xsl:otherwise>

            </xsl:choose>
        </xsl:variable>

        <xsl:variable name="ViewConsolidationCell">
            <xsl:choose>
                <xsl:when test="$FirstCell = $LastCell">
                    <xsl:value-of select="$FirstCell"/>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:value-of select="concat($FirstCell,':',$LastCell)"/>
                </xsl:otherwise>
            </xsl:choose>
        </xsl:variable>

        <xsl:if test="$ViewConsolidationCell !=''">

            <xsl:variable name="sheet">
                <xsl:value-of select="substring-before($ViewConsolidation,'.')"/>
            </xsl:variable>

            <xsl:variable name="targetSheet">
                <xsl:value-of select="substring-before($ViewTargetCellAddress,'.')"/>
            </xsl:variable>
            
            <xsl:variable name="ViewTargetSheet">
                <xsl:choose>
                    <xsl:when test="$sheet = $targetSheet">
                        <xsl:value-of select="$sheet"/>
                    </xsl:when>
                    <xsl:otherwise>
                        <xsl:value-of select="$targetSheet"/>
                    </xsl:otherwise>
                </xsl:choose>
            </xsl:variable>
            
            <!--xsl:variable name="checkName">
                <xsl:for-each
                select="/office:document-content/office:body/office:spreadsheet/table:table[@table:name = translate($sheet,'','')]">
                <xsl:call-template name="CheckSheetName">
                <xsl:with-param name="sheetNumber">
                <xsl:number count="table:table" level="single"/>
                </xsl:with-param>
                <xsl:with-param name="name">
                <xsl:value-of
                select="substring(translate($sheet,&quot;*\/[]:&apos;?&quot;,&quot;&quot;),1,31)"/>
                
                </xsl:with-param>
                </xsl:call-template>
                </xsl:for-each>
                </xsl:variable-->

            <dataRef ref="{$ViewConsolidationCell}" sheet="{$sheet}"/>

            <xsl:call-template name="InsertDataRef">
                <xsl:with-param name="ViewConsolidation">
                    <xsl:value-of select="substring-after($ViewConsolidation,' ')"/>
                </xsl:with-param>
                <xsl:with-param name="ViewTargetSheet">
                    <xsl:value-of select="$ViewTargetSheet"/>
                </xsl:with-param>
            </xsl:call-template>

        </xsl:if>
    </xsl:template>

</xsl:stylesheet>
