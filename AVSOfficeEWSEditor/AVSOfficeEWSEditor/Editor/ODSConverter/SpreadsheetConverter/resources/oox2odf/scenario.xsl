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
    xmlns:dc="http://purl.org/dc/elements/1.1/"
    xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
    xmlns:e="http://schemas.openxmlformats.org/spreadsheetml/2006/main"
    xmlns:oox="urn:oox"
    exclude-result-prefixes="e oox r">

    <!-- Insert Scenario Styles -->

    <xsl:template name="InsertScenarioStyles">

        <style:style style:name="scenario" style:family="table" style:master-page-name="Default">
            <style:table-properties>
                <xsl:attribute name="table:display">
                    <xsl:text>false</xsl:text>
                </xsl:attribute>
                <xsl:attribute name="style:writing-mode">
                    <xsl:text>lr-tb</xsl:text>
                </xsl:attribute>
            </style:table-properties>
        </style:style>

    </xsl:template>

    <!--  Check cell when the Scenario is starting and ending -->
    <xsl:template name="ScenarioCell">
        <xsl:param name="sheet"/>
        <xsl:param name="document"/>

        <xsl:apply-templates select="e:worksheet/e:scenarios/e:scenario/e:inputCells[1]">
            <xsl:with-param name="document">
                <xsl:value-of select="$document"/>
            </xsl:with-param>
        </xsl:apply-templates>

    </xsl:template>

    <xsl:template name="Scenarios">


        <xsl:for-each select="e:scenario">


            <table:table>

                <xsl:attribute name="table:name">
                    <xsl:value-of select="@name"/>
                </xsl:attribute>

                <xsl:attribute name="table:style-name">
                    <xsl:text>scenario</xsl:text>
                </xsl:attribute>

                <xsl:call-template name="InsertScenario">
                    <xsl:with-param name="number" select="position()"/>
                </xsl:call-template>

            </table:table>

        </xsl:for-each>
    </xsl:template>

    <xsl:template name="InsertScenario">
        <xsl:param name="number"/>

        <table:scenario>

            <xsl:choose>
                <xsl:when test="contains(@locked, '1')">
                    <xsl:attribute name="table:protected">
                        <xsl:text>true</xsl:text>
                    </xsl:attribute>
                </xsl:when>
            </xsl:choose>


            <xsl:attribute name="table:border-color">
                <xsl:text>#c0c0c0</xsl:text>
            </xsl:attribute>

            
            <xsl:attribute name="table:is-active">
                <xsl:choose>
                    <xsl:when test="contains(@hidden, '1')">
                        <xsl:text>false</xsl:text>
                    </xsl:when>
                    <xsl:when test="parent::node()/@show + 1 =$number">
                        <xsl:text>true</xsl:text>
                    </xsl:when>
                    <xsl:otherwise>
                        <xsl:text>false</xsl:text>
                    </xsl:otherwise>
                </xsl:choose>
            </xsl:attribute>

            <xsl:attribute name="table:scenario-ranges">

                <xsl:variable name="scenarioName">
                    <xsl:choose>
                        <xsl:when test="parent::node()/e:scenario">
                            <xsl:value-of select="@name"/>
                        </xsl:when>
                    </xsl:choose>
                </xsl:variable>

                <xsl:variable name="firstInputCell">
                    <xsl:value-of select="e:inputCells/@r[1]"/>
                </xsl:variable>

                <xsl:variable name="lastInputCell">
                    <xsl:value-of select="e:inputCells[last()]/@r"/>
                </xsl:variable>

                <xsl:value-of
                    select="concat($scenarioName,'.',$firstInputCell,':',$scenarioName,'.',$lastInputCell)"/>

            </xsl:attribute>

            <xsl:attribute name="table:comment">
                <xsl:value-of select="@comment"/>
            </xsl:attribute>

        </table:scenario>


        <xsl:call-template name="InsertScenarioCells"/>
    </xsl:template>

    <xsl:template name="InsertScenarioCells">

        <xsl:variable name="ScenarioCells">

            <xsl:for-each select="e:inputCells">
                <xsl:variable name="numCol">
                    <xsl:call-template name="GetColNum">
                        <xsl:with-param name="cell">
                            <xsl:value-of select="@r"/>
                        </xsl:with-param>
                    </xsl:call-template>
                </xsl:variable>

                <xsl:variable name="numRow">
                    <xsl:call-template name="GetRowNum">
                        <xsl:with-param name="cell">
                            <xsl:value-of select="@r"/>
                        </xsl:with-param>
                    </xsl:call-template>
                </xsl:variable>

                <xsl:variable name="inputValues">
                    <xsl:value-of select="@val"/>
                </xsl:variable>

                <xsl:value-of select="concat($numRow,':',$numCol,'-',$inputValues,';')"/>
            </xsl:for-each>

        </xsl:variable>

        <xsl:variable name="firstRow">
            <xsl:value-of select="substring-before($ScenarioCells, ':')"/>
        </xsl:variable>

        <table:table-row>

            <xsl:attribute name="table:number-rows-repeated">
                <xsl:value-of select="$firstRow - 1"/>
            </xsl:attribute>

        </table:table-row>

        <xsl:call-template name="InsertRowScenario">
            <xsl:with-param name="ScenarioCells">
                <xsl:value-of select="$ScenarioCells"/>
            </xsl:with-param>
            <xsl:with-param name="numRow">
                <xsl:value-of select="$firstRow"/>
            </xsl:with-param>
        </xsl:call-template>
        
    </xsl:template>

    <xsl:template name="InsertRowScenario">
        <xsl:param name="numRow"/>
        <xsl:param name="ScenarioCells"/>

        <xsl:if test="contains(concat(';', $ScenarioCells), concat(';', $numRow, ':'))">
            
            <table:table-row>
                <xsl:call-template name="InsertCellScenario">
                    <xsl:with-param name="numRow">
                        <xsl:value-of select="$numRow"/>
                    </xsl:with-param>
                    <xsl:with-param name="ScenarioCells">
                        <xsl:value-of select="$ScenarioCells"/>
                    </xsl:with-param>
                </xsl:call-template>
            </table:table-row>

            <xsl:call-template name="InsertRowScenario">
                <xsl:with-param name="numRow">
                    <xsl:value-of select="$numRow + 1"/>
                </xsl:with-param>
                <xsl:with-param name="ScenarioCells">
                    <xsl:value-of select="$ScenarioCells"/>
                </xsl:with-param>
            </xsl:call-template>

        </xsl:if>

      <!--Defect Id       :1877274(FileNames:Verpackung.xlsx,Verpackung-E.xlsx,Optimierung.xlsx)
			  * Code Changed by :Sateesh Reddy
			  * Date            :05th Jan '08
				      -->
      <xsl:variable name="lastRow">
        <xsl:value-of select="substring-before(substring-after($ScenarioCells, ';'),':')"/>
      </xsl:variable>

      <xsl:if test="not(contains(concat(';', $ScenarioCells), concat(';', $numRow, ':')))">
        <xsl:if test="$numRow &lt; $lastRow">
          <table:table-row>
            <xsl:attribute name="table:number-rows-repeated">
              <xsl:value-of select="$lastRow - $numRow"/>
            </xsl:attribute>
          </table:table-row>
          <table:table-row>
            <xsl:call-template name="InsertCellScenario">
              <xsl:with-param name="numRow">
                <xsl:value-of select="$lastRow"/>
              </xsl:with-param>
              <xsl:with-param name="ScenarioCells">
                <xsl:value-of select="$ScenarioCells"/>
              </xsl:with-param>
            </xsl:call-template>
          </table:table-row>
        </xsl:if>
      </xsl:if>

     
    </xsl:template>

    <xsl:template name="InsertCellScenario">
        <xsl:param name="numRow"/>
        <xsl:param name="ScenarioCells"/>

        <xsl:variable name="FirstCol">
            <xsl:value-of
                select="substring-before(substring-after(concat(';', $ScenarioCells), concat(';', $numRow, ':')), '-')"
            />
        </xsl:variable>

        <xsl:if test="$FirstCol &gt; 1">
            <table:table-cell>
                <xsl:attribute name="table:number-columns-repeated">
                    <xsl:value-of select="$FirstCol - 1"/>
                </xsl:attribute>
            </table:table-cell>

            <xsl:call-template name="InsertValue">
                <xsl:with-param name="numRow">
                    <xsl:value-of select="$numRow"/>
                </xsl:with-param>
                <xsl:with-param name="ScenarioCells">
                    <xsl:value-of select="$ScenarioCells"/>
                </xsl:with-param>
            </xsl:call-template>

        </xsl:if>
    </xsl:template>

    <xsl:template name="InsertValue">
        <xsl:param name="numRow"/>
        <xsl:param name="ScenarioCells"/>

        <xsl:variable name="numCol">
            <xsl:value-of
                select="substring-before(substring-after(concat(';', $ScenarioCells), concat(';', $numRow, ':')), '-')"
            />
        </xsl:variable>

        <xsl:if
            test="contains(concat(';', $ScenarioCells), concat(';', $numRow, ':', $numCol, '-'))">

            <table:table-cell>
                <text:p>
                    <xsl:value-of
                        select="substring-before(substring-after(concat(';', $ScenarioCells), concat(';', $numRow, ':', $numCol, '-')), ';')"
                    />
                </text:p>
            </table:table-cell>

            <xsl:call-template name="InsertValue">
                <xsl:with-param name="numRow">
                    <xsl:value-of select="$numRow"/>
                </xsl:with-param>
                <xsl:with-param name="ScenarioCells">
                    <xsl:value-of
                        select="substring-after(substring-after(concat(';', $ScenarioCells), concat(';', $numRow, ':', $numCol, '-')), ';')"
                    />
                </xsl:with-param>
            </xsl:call-template>

        </xsl:if>

    </xsl:template>

</xsl:stylesheet>
