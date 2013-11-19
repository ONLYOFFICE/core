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
    RefNo-1 19-Dec-2007 Sandeep S     1832369   Changes done to avoid crash, while converting column charts "combination Chart".  
    RefNo-2 02-Jan-2008 Sandeep S     1797015   Changes done to fix the secondary y-axis and stock chart 3rd type problem.
    '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
    xmlns:pzip="urn:cleverage:xmlns:post-processings:zip"
    xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
    xmlns:chart="urn:oasis:names:tc:opendocument:xmlns:chart:1.0"
    xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
    xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
    xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart"
    xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main"
    xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
    xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
    xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
    xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:v="urn:schemas-microsoft-com:vml"
    xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0">
    
    
    
    <xsl:template name="tmpGradientFill">
        <xsl:param name="gradStyleName"/>
        <xsl:param name="opacity"/>
        <xsl:param name="ChartDirectory"/>
        
        <a:gradFill flip="none" rotWithShape="1">
            
            <xsl:for-each select="document(concat($ChartDirectory, '/', 'styles.xml'))//draw:gradient[@draw:name= $gradStyleName]">
                <a:gsLst>
                    <a:gs pos="0">
                        <a:srgbClr>
                            <xsl:attribute name="val">
                                <xsl:if test="@draw:start-color">
                                    <xsl:value-of select="substring-after(@draw:start-color,'#')" />
                                </xsl:if>
                                <xsl:if test="not(@draw:start-color)">
                                    <xsl:value-of select="'ffffff'" />
                                </xsl:if>
                            </xsl:attribute>
                            <xsl:call-template name="tmpshapeTransperancy">
                                <xsl:with-param name="tranparency" select="$opacity"/>
                            </xsl:call-template>
                        </a:srgbClr >
                    </a:gs>
                    <a:gs pos="100000">
                        <a:srgbClr>
                            <xsl:attribute name="val">
                                <xsl:if test="@draw:end-color">
                                    <xsl:value-of select="substring-after(@draw:end-color,'#')" />
                                </xsl:if>
                                <xsl:if test="not(@draw:end-color)">
                                    <xsl:value-of select="'ffffff'" />
                                </xsl:if>
                            </xsl:attribute>
                            <xsl:call-template name="tmpshapeTransperancy">
                                <xsl:with-param name="tranparency" select="$opacity"/>
                            </xsl:call-template>
                        </a:srgbClr>
                    </a:gs>
                </a:gsLst>
                <xsl:choose>
                    <xsl:when test="@draw:style='radial'">
                        <a:path path="circle">
                            <xsl:call-template name="tmpFillToRect"/>
                        </a:path>
                        <xsl:call-template name="tmpTileToRect"/>
                    </xsl:when>
                    <xsl:when test="@draw:style='ellipsoid'">
                        <a:path path="shape">
                            <xsl:call-template name="tmpFillToRect"/>
                        </a:path>
                        <xsl:call-template name="tmpTileToRect"/>
                    </xsl:when>
                    <xsl:when test="@draw:style='linear'">
                        <a:lin  scaled="1">
                            <xsl:if test="@draw:angle!=''">
                                <xsl:attribute name="ang">
                                    <xsl:choose>
                                        <xsl:when test="@draw:angle">
                                            <xsl:variable name="angleValue">
                                                <xsl:value-of select="round(((( ( -1 * @draw:angle) + 900 ) mod 3600)  div 10) * 60000)"/>
                                            </xsl:variable>
                                            <xsl:choose>
                                                <xsl:when test="$angleValue &lt; 0">
                                                    <xsl:value-of select="-1 * $angleValue "/>
                                                </xsl:when>
                                                <xsl:otherwise>
                                                    <xsl:value-of select="$angleValue "/>
                                                </xsl:otherwise>
                                            </xsl:choose>
                                            
                                        </xsl:when>
                                        <xsl:otherwise>
                                            <xsl:value-of select="'0'"/>
                                        </xsl:otherwise>
                                    </xsl:choose>
                                </xsl:attribute>
                            </xsl:if>
                        </a:lin>
                        <a:tileRect/>
                    </xsl:when>
                    <xsl:when test="@draw:style='rectangular' or @draw:style='square'">
                        <a:path path="rect">
                            <xsl:call-template name="tmpFillToRect"/>
                        </a:path>
                        <xsl:call-template name="tmpTileToRect"/>
                    </xsl:when>
                    <xsl:otherwise>
                        <a:lin ang="0" scaled="1"/>
                        <a:tileRect/>
                    </xsl:otherwise>
                </xsl:choose>
            </xsl:for-each>
        </a:gradFill>
    </xsl:template>
    
    <xsl:template name="tmpTileToRect">
        <xsl:choose>
            <xsl:when test="@draw:cx and @draw:cy">
                <xsl:choose>
                    <xsl:when test="substring-before(@draw:cx,'%') =100 and substring-before(@draw:cy,'%') = 100">
                        <a:tileRect/>
                    </xsl:when>
                    <xsl:when test="substring-before(@draw:cx,'%') =0 and substring-before(@draw:cy,'%') = 0">
                        <a:tileRect l="-100000" t="-100000"/>
                    </xsl:when>
                    <xsl:when test="substring-before(@draw:cx,'%') =100 and substring-before(@draw:cy,'%') = 0">
                        <a:tileRect r="-100000" t="-100000"/>
                    </xsl:when>
                    <xsl:when test="substring-before(@draw:cx,'%') =0 and substring-before(@draw:cy,'%') = 100">
                        <a:tileRect l="-100000" b="-100000"/>
                    </xsl:when>
                    <xsl:when test="substring-before(@draw:cx,'%') > 0 and substring-before(@draw:cy,'%') > 0">
                        <a:tileRect/>
                    </xsl:when>
                </xsl:choose>
            </xsl:when>
            <xsl:otherwise>
                <a:tileRect/>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>
    
    <xsl:template name="tmpFillToRect">
        <xsl:choose>
            <xsl:when test="@draw:cx and @draw:cy">
                <xsl:choose>
                    <xsl:when test="substring-before(@draw:cx,'%') =100 and substring-before(@draw:cy,'%') = 100">
                        <a:fillToRect l="50000" t="50000" r="50000" b="50000"/>
                    </xsl:when>
                    <xsl:when test="substring-before(@draw:cx,'%') =0 and substring-before(@draw:cy,'%') = 0">
                        <a:fillToRect r="100000" b="100000"/>
                    </xsl:when>
                    <xsl:when test="substring-before(@draw:cx,'%') =100 and substring-before(@draw:cy,'%') = 0">
                        <a:fillToRect l="100000" b="100000"/>
                    </xsl:when>
                    <xsl:when test="substring-before(@draw:cx,'%') =0 and substring-before(@draw:cy,'%') = 100">
                        <a:fillToRect r="100000" t="100000"/>
                    </xsl:when>
                    <xsl:when test="substring-before(@draw:cx,'%') > 0 and substring-before(@draw:cy,'%') > 0">
                        <a:fillToRect l="100000" t="100000" r="100000" b="100000"/>
                    </xsl:when>
                </xsl:choose>
            </xsl:when>
            <xsl:otherwise>
                <a:fillToRect/>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>
    
    <xsl:template name="tmpshapeTransperancy" >
        <xsl:param name="tranparency"/>
        <xsl:choose>
            <xsl:when test ="$tranparency ='0'">
                <a:alpha val="0"/>
            </xsl:when>
            <xsl:when test="$tranparency !=''">
                <a:alpha>
                    <xsl:attribute name="val">
                        <xsl:value-of select="format-number($tranparency * 1000,'#')" />
                    </xsl:attribute>
                </a:alpha>
            </xsl:when>
        </xsl:choose>
    </xsl:template>
    
</xsl:stylesheet>
