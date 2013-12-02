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
RefNo-1 22-Jan-2008 Sandeep S     1833074   Changes for fixing Cell Content missing and 1832335 New line inserted in note content after roundtrip conversions                                              
RefNo-2 22-Sep-2008 Sandeep S New feature Changes for formula implementation.
RefNo-3 8-Jan-2009 Sandeep S     ODF1.1   Changes done for ODF1.1 conformance  
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:v="urn:schemas-microsoft-com:vml" xmlns:xlink="http://www.w3.org/1999/xlink"
  xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:pxsi="urn:cleverage:xmlns:post-processings:shared-strings"
  xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
  xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns:e="http://schemas.openxmlformats.org/spreadsheetml/2006/main"
  xmlns:oox="urn:oox"
  exclude-result-prefixes="e oox r pxsi v">

  <xsl:key name="SharedStrings" match="e:sst/e:si" use="@oox:id"/>

  <!--Start of RefNo-2:Formula implemetation-->
  <xsl:variable name="invalidChars">
    <xsl:text>&apos;!-$#():,.+^&amp;=&lt;&gt;%{};"</xsl:text>
  </xsl:variable>
  
  <xsl:variable name ="sheetNames">
    <xsl:for-each select ="key('Part','xl/workbook.xml')//e:sheets/e:sheet">
      <xsl:variable name="checkedName">
        <xsl:call-template name="CheckSheetName">
          <xsl:with-param name="sheetNumber">
            <xsl:value-of select="position()"/>
          </xsl:with-param>
          <xsl:with-param name="name">
            <xsl:value-of select="@name"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:variable>
      <xsl:value-of select ="concat(@name,':',$checkedName,'::')"/>
    </xsl:for-each>
  </xsl:variable>
  <!--End of RefNo-2-->
  <xsl:template name="InsertText">
    <xsl:param name="position"/>
    <xsl:param name="colNum"/>
    <xsl:param name="rowNum"/>
    <xsl:param name="sheetNr"/>
    <xsl:param name="rSheredStrings"/>
    <xsl:param name ="CheckIfNote"/>

    <xsl:variable name="partId" select="ancestor::e:worksheet/@oox:part" />
	  <xsl:variable name="extLink">
		  <xsl:text>&apos;[</xsl:text>
	  </xsl:variable>
    <!--Start of RefNo-2:Formula implemetation-->
    <xsl:if test="e:f and not(e:f='')">
      <!--Add code for not compatible formulas-->
      <xsl:choose>
        <!--chk for sheet sheets n style-->
        <!--(contains(.,'([') or contains(.,'{[') or contains(.,' [') or contains(.,',[') or starts-with(.,'[')) or -->
        <xsl:when test="not(
                           (contains(e:f,'([') or contains(e:f,'{[') or contains(e:f,' [') or contains(e:f,',[') or starts-with(e:f,'[') or contains(e:f, $extLink)) 
                        or (contains(e:f,'AVERAGEIF('))
                        or (contains(e:f,'AVERAGEIFS('))
                        or (contains(e:f,'COUNTIFS('))
                        or (contains(e:f,'CUBEKPIMEMBER('))
                        or (contains(e:f,'CUBERANKEDMEMBER('))
                        or (contains(e:f,'CUBESET('))
                        or (contains(e:f,'CUBESETCOUNT('))
                        or (contains(e:f,'CUBEVALUE('))                       
                        or (contains(e:f,'GETPIVOTDATA('))
                        or (contains(e:f,'IFERROR('))
                        or (contains(e:f,'RTD('))
                        or (contains(e:f,'SUMIFS(')))">
          <xsl:choose>
            <xsl:when test="contains(e:f,'!')">          
              <xsl:choose>
                <xsl:when test="string-length($sheetNames) = string-length(translate($sheetNames,$invalidChars,''))">
                  <xsl:attribute name="table:formula">
                    <xsl:value-of select="concat('sonataOoxFormula',e:f)"/>
                 </xsl:attribute>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:attribute name="table:formula">
                    <xsl:value-of select="concat('sonataOoxFormula',e:f,'##shtName##',$sheetNames)"/>
                  </xsl:attribute>
                </xsl:otherwise>
              </xsl:choose>       
            </xsl:when>
            <xsl:otherwise>
              <xsl:attribute name="table:formula">
                <xsl:value-of select="concat('sonataOoxFormula',e:f)"/>
              </xsl:attribute>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:when>        
      </xsl:choose>
    
      <!--TODO caluculate correct value-->
      <xsl:if test="e:f/@t='array'">
        <xsl:choose>
          <xsl:when test="not(contains(@r,':'))">
            <xsl:attribute name="table:number-matrix-columns-spanned">
              <xsl:value-of select="1"/>
            </xsl:attribute>
            <xsl:attribute name="table:number-matrix-rows-spanned">
              <xsl:value-of select="1"/>
            </xsl:attribute>
          </xsl:when>
          <xsl:otherwise>
            <xsl:variable name="refSpan" select="substring-after(@r,':')"/>
              <!--A2:C3 C3 -(3-colNum)+1= 3 || 3-rowNum+1 = 2-->
              <xsl:variable name="spanColNum">
                <xsl:call-template name="GetColNum">
                  <xsl:with-param name="cell">
                    <xsl:value-of select="$refSpan"/>
                  </xsl:with-param>
                </xsl:call-template>
              </xsl:variable>
              <xsl:variable name="spanRowNum">
                <xsl:call-template name="GetRowNum">
                  <xsl:with-param name="cell">
                    <xsl:value-of select="$refSpan"/>
                  </xsl:with-param>
                </xsl:call-template>
            </xsl:variable>
            <xsl:attribute name="table:number-matrix-columns-spanned">
              <xsl:value-of select="$spanColNum - $colNum + 1"/>
            </xsl:attribute>
            <xsl:attribute name="table:number-matrix-rows-spanned">
              <xsl:value-of select="$spanRowNum - $rowNum + 1"/>
            </xsl:attribute>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:if>
      
    </xsl:if>
    <!--End of RefNo-2-->
    <!--boolean to be handeled-->
    <xsl:choose>
      <!-- <xsl:when test="@t='d'">
        <xsl:attribute name="office:value-type">
          <xsl:text>date</xsl:text>
        </xsl:attribute>
      </xsl:when>-->
      <xsl:when test="@t='s' or key('ref',@r)[@oox:part = $partId]">
        <xsl:attribute name="office:value-type">
          <xsl:text>string</xsl:text>
        </xsl:attribute>
         <!--Redundant Code-->
        <!--<xsl:variable name="id">
          <xsl:value-of select="e:v"/> 
        </xsl:variable>-->
        <!--end-->
        <!--Start of RefNo-3:ODF1.1:Added office:annotation abv text node-->
        <xsl:if test="$CheckIfNote = 'true'">
          <xsl:call-template name="InsertNoteInThisCell">
            <xsl:with-param name="rowNum">
              <xsl:value-of select="$rowNum"/>
            </xsl:with-param>
            <xsl:with-param name="colNum">
              <xsl:value-of select="$colNum"/>
            </xsl:with-param>
            <xsl:with-param name="sheetNr">
              <xsl:value-of select="$sheetNr"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:if>
        <!--End of RefNo-3-->
        <text:p>
          <xsl:choose>
            <xsl:when test="key('ref',@r)[@oox:part = $partId]">
              <xsl:variable name="XlinkHref">
                <xsl:call-template name="XlinkHref">
                  <xsl:with-param name="sheetNr">
                    <xsl:value-of select="$sheetNr"/>
                  </xsl:with-param>
                </xsl:call-template>
              </xsl:variable>
              <!-- a postprocessor puts here strings from sharedstrings -->
							<!--Vijayeta,SP2,Text in Hyperlink Missing and regression defect 2632716-->
							<xsl:choose>
								<xsl:when test="translate(substring-after($XlinkHref,'#'),'.','')!='' or contains($XlinkHref,'http://') or contains($XlinkHref,'file:///') or contains($XlinkHref,'mailto:')">
              <xsl:choose>
                <xsl:when test="contains($rSheredStrings, e:v) ">
                  <xsl:for-each select="key('SharedStrings', e:v)">
                    <xsl:apply-templates select="e:r[1]" mode="hyperlink">
                      <xsl:with-param name="XlinkHref">
                        <xsl:value-of select="$XlinkHref"/>
                      </xsl:with-param>
                      <xsl:with-param name="position">
                        <xsl:text>1</xsl:text>
                      </xsl:with-param>
                    </xsl:apply-templates>
                  </xsl:for-each>
                </xsl:when>
                <xsl:otherwise>   
               <!--Changes As per XSLT Best Practices-->             
                  <!--<text:a>
                    <xsl:attribute name="xlink:href">
                      <xsl:value-of select="$XlinkHref"/>
                    </xsl:attribute>-->                                
                  <text:a xlink:href="{$XlinkHref}" >                 
                    <!--<xsl:choose>
                      <xsl:when test="@t='s'"/>
                      <xsl:otherwise>
                        <xsl:value-of select="e:v"/>
                      </xsl:otherwise>
                    </xsl:choose>-->                    
                    <xsl:if test="@t !=string('s')">
                      <xsl:value-of select="e:v"/>
                    </xsl:if>                  
                    <pxsi:v>
                      <xsl:value-of select="e:v"/>
                    </pxsi:v>
                  </text:a>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:when>
								<xsl:otherwise>
									<xsl:choose>
										<xsl:when test="contains($rSheredStrings, e:v) ">
											<xsl:for-each select="key('SharedStrings', e:v)">
												<xsl:apply-templates/>
												<!--RefNo-1: Changes for fixing 1833074 XLSX: Cell Content missing-->
												<!--<xsl:with-param name ="textp" select ="'T'"/>
                    </xsl:apply-templates >-->
											</xsl:for-each>
										</xsl:when>
										<xsl:otherwise>
											<pxsi:v>
												<xsl:value-of select="e:v"/>
											</pxsi:v>
										</xsl:otherwise>
									</xsl:choose>
								</xsl:otherwise>
							</xsl:choose>
							<!--Vijayeta,SP2,Text in Hyperlink Missing,End-->
						</xsl:when>
            <xsl:otherwise>
              <!-- a postprocessor puts here strings from sharedstrings -->
              <xsl:choose>
                <xsl:when test="contains($rSheredStrings, e:v) ">
                  <xsl:for-each select="key('SharedStrings', e:v)">
                    <xsl:apply-templates/>
                      <!--RefNo-1: Changes for fixing 1833074 XLSX: Cell Content missing-->
                      <!--<xsl:with-param name ="textp" select ="'T'"/>
                    </xsl:apply-templates >-->
                  </xsl:for-each>
                </xsl:when>
                <xsl:otherwise>
                  <pxsi:v>
                    <xsl:value-of select="e:v"/>
                  </pxsi:v>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:otherwise>
          </xsl:choose>
        </text:p>
      </xsl:when>
      <xsl:when test="@t = 'e' ">
        <xsl:attribute name="office:value-type">
          <xsl:text>string</xsl:text>
        </xsl:attribute>
        <!--Start of RefNo-3:ODF1.1:Added office:annotation abv text node-->
        <xsl:if test="$CheckIfNote = 'true'">
          <xsl:call-template name="InsertNoteInThisCell">
            <xsl:with-param name="rowNum">
              <xsl:value-of select="$rowNum"/>
            </xsl:with-param>
            <xsl:with-param name="colNum">
              <xsl:value-of select="$colNum"/>
            </xsl:with-param>
            <xsl:with-param name="sheetNr">
              <xsl:value-of select="$sheetNr"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:if>
        <!--End of RefNo-3-->
        <text:p>
          <xsl:value-of select="e:v"/>
        </text:p>
      </xsl:when>
      <xsl:when test="@t = 'str' ">
        <xsl:attribute name="office:value-type">
          <xsl:choose>
            <xsl:when test="number(e:v)">
              <xsl:text>float</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>string</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
        <!--RefNo-3:Added office:value attribute in case of folat.-->
        <xsl:if test="number(e:v)">
          <xsl:attribute name="office:value">
            <xsl:value-of select="number(e:v)"/>
          </xsl:attribute>
        </xsl:if>
        <!--Start of RefNo-3:ODF1.1:Added office:annotation abv text node-->
        <xsl:if test="$CheckIfNote = 'true'">
          <xsl:call-template name="InsertNoteInThisCell">
            <xsl:with-param name="rowNum">
              <xsl:value-of select="$rowNum"/>
            </xsl:with-param>
            <xsl:with-param name="colNum">
              <xsl:value-of select="$colNum"/>
            </xsl:with-param>
            <xsl:with-param name="sheetNr">
              <xsl:value-of select="$sheetNr"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:if>
        <!--End of RefNo-3-->
        <text:p>
          <xsl:choose>
            <xsl:when test="number(e:v)">
              <xsl:call-template name="FormatNumber">
                <xsl:with-param name="value">
                  <xsl:value-of select="e:v"/>
                </xsl:with-param>
                <xsl:with-param name="numStyle">
                  <xsl:value-of select="key('numFmtId',key('Xf', $position)/@numFmtId)/@formatCode" />
                </xsl:with-param>
              </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="e:v"/>
            </xsl:otherwise>
          </xsl:choose>
        </text:p>
      </xsl:when>
      <xsl:when test="@t = 'n'">
        <xsl:attribute name="office:value-type">
          <xsl:text>float</xsl:text>
        </xsl:attribute>
        <xsl:attribute name="office:value">
          <xsl:value-of select="e:v"/>
        </xsl:attribute>
        <!--Start of RefNo-3:ODF1.1:Added office:annotation abv text node-->
        <xsl:if test="$CheckIfNote = 'true'">
          <xsl:call-template name="InsertNoteInThisCell">
            <xsl:with-param name="rowNum">
              <xsl:value-of select="$rowNum"/>
            </xsl:with-param>
            <xsl:with-param name="colNum">
              <xsl:value-of select="$colNum"/>
            </xsl:with-param>
            <xsl:with-param name="sheetNr">
              <xsl:value-of select="$sheetNr"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:if>
        <!--End of RefNo-3-->
        <text:p>
          <xsl:call-template name="FormatNumber">
            <xsl:with-param name="value">
              <xsl:value-of select="e:v"/>
            </xsl:with-param>
            <xsl:with-param name="numStyle">
              <xsl:value-of select="key('numFmtId',key('Xf', $position)/@numFmtId)/@formatCode" />
            </xsl:with-param>
          </xsl:call-template>
        </text:p>
      </xsl:when>
      <!--Start of RefNo-2:Added boolean value type-->
      <xsl:when test="@t = 'b'">
        <xsl:attribute name="office:value-type">
          <xsl:text>boolean</xsl:text>
        </xsl:attribute>
        <xsl:attribute name="office:boolean-value">
          <xsl:choose>
            <xsl:when test="e:v = '0'">
              <xsl:value-of select="'false'"/>
            </xsl:when>
            <xsl:when test="e:v = '1'">
              <xsl:value-of select="'true'"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="e:v"/>
            </xsl:otherwise>            
          </xsl:choose>
        </xsl:attribute>
        <!--Start of RefNo-3:ODF1.1:Added office:annotation abv text node-->
        <xsl:if test="$CheckIfNote = 'true'">
          <xsl:call-template name="InsertNoteInThisCell">
            <xsl:with-param name="rowNum">
              <xsl:value-of select="$rowNum"/>
            </xsl:with-param>
            <xsl:with-param name="colNum">
              <xsl:value-of select="$colNum"/>
            </xsl:with-param>
            <xsl:with-param name="sheetNr">
              <xsl:value-of select="$sheetNr"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:if>
        <!--End of RefNo-3-->
        <text:p>
          <xsl:choose>
            <xsl:when test="e:v = '0'">
              <xsl:value-of select="'FALSE'"/>
            </xsl:when>
            <xsl:when test="e:v = '1'">
              <xsl:value-of select="'TRUE'"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="e:v"/>
            </xsl:otherwise>
          </xsl:choose>
        </text:p>
      </xsl:when>
      <!--End of RefNo-2-->
      <xsl:otherwise>
        <xsl:variable name="numStyle">
          <xsl:value-of select="key('numFmtId',key('Xf', $position)/@numFmtId)/@formatCode" />
        </xsl:variable>
        <xsl:variable name="numId">
          <xsl:value-of select="key('Xf', $position)/@numFmtId"/>
        </xsl:variable>

        <xsl:variable name="strippedFormat">
          <xsl:call-template name="StripText">
            <xsl:with-param name="formatCode" select="$numStyle"/>
            <xsl:with-param name="preserveCurrency" select="'true'"/>
          </xsl:call-template>
        </xsl:variable>

        <!--Start of RefNo-3:ODF1.1:used variable to store value type-->
        <xsl:variable name="valueType">
          <xsl:choose>
            <xsl:when
              test="contains($numStyle,'%') or ((not($numStyle) or $numStyle = '')  and ($numId = 9 or $numId = 10))">
              <xsl:text>percentage</xsl:text>
            </xsl:when>
            <xsl:when
              test="(contains($strippedFormat,'y') or (contains($strippedFormat,'m') and not(contains($strippedFormat,'h') or contains($strippedFormat,'s'))) or (contains($strippedFormat,'d') and not(contains($strippedFormat,'Red'))) or ($numId &gt; 13 and $numId &lt; 18) or $numId = 22)">
              <xsl:choose>
                <xsl:when
                  test="contains($numStyle, '[$$-409]') or contains($numStyle, '[$Sk-41B]') or contains($numStyle, '[$Din.-81A]') or contains($numStyle, '[$€-180C]') or contains($numStyle, '[$€-1007]')">
                  <xsl:text>currency</xsl:text>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:text>date</xsl:text>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:when>

            <!--'and' at the end is for Latvian currency -->
            <xsl:when
              test="contains($strippedFormat,'h') or contains($strippedFormat,'s') and not(contains($numStyle,'[$Ls-426]'))">
              <xsl:text>time</xsl:text>
            </xsl:when>

            <xsl:when
              test="contains($numStyle,'zł') or contains($numStyle,'$') or contains($numStyle,'£') or contains($numStyle,'€')">
              <xsl:text>currency</xsl:text>
            </xsl:when>
            <xsl:when test="$numId = 18 or $numId = 20 ">
              <xsl:text>time</xsl:text>
            </xsl:when>
            <xsl:when test="$numId = 49">
              <xsl:text>string</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:choose>
                <xsl:when test="@t='d'">
                  <xsl:attribute name="office:value-type">
                    <xsl:text>date</xsl:text>
                  </xsl:attribute>
                </xsl:when>
                <xsl:otherwise>
              <xsl:text>float</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
              
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>

        <xsl:attribute name="office:value-type">
          <xsl:value-of select="$valueType"/>
        </xsl:attribute>
        <!--End of RefNo-3-->
        <xsl:choose>
          <xsl:when
            test="((contains($strippedFormat,'y') or (contains($strippedFormat,'m') and not(contains($strippedFormat,'h') or contains($strippedFormat,'s'))) or (contains($strippedFormat,'d') and not(contains($strippedFormat,'Red'))) or ($numId &gt; 13 and $numId &lt; 18) or $numId = 22)) and not(contains($numStyle, '[$$-409]') or contains($numStyle, '[$Sk-41B]') or contains($numStyle, '[$Din.-81A]') or contains($numStyle, '[$€-180C]') or contains($numStyle, '[$€-1007]'))">
            <xsl:choose>
              <xsl:when test ="not(@t) or @t!='d'">
            <xsl:attribute name="office:date-value">
              <xsl:call-template name="NumberToDate">
                <xsl:with-param name="value">
                  <xsl:choose>
                    <xsl:when
                      test="key('Part', 'xl/workbook.xml')/e:workbook/e:workbookPr/@date1904 =1 ">
                      <xsl:value-of select="(e:v) + (1462)"/>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:value-of select="e:v"/>
                    </xsl:otherwise>
                  </xsl:choose>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:attribute>
                <!--<xsl:attribute name="office:date-value">
              
            </xsl:attribute>-->
              </xsl:when>
              <xsl:when test ="@t='d'">
                <xsl:attribute name="office:date-value">
					<xsl:variable name="DateWithTime">
						<xsl:call-template name="CheckTime">
							<xsl:with-param name="value">
                  <xsl:value-of select ="./e:v"/>
							</xsl:with-param>
						</xsl:call-template>
					</xsl:variable>
					<xsl:value-of select ="$DateWithTime"/>                  
                </xsl:attribute>
              </xsl:when>
          </xsl:choose>            
          </xsl:when>
          <!--'and' at the end is for Latvian currency -->
          <xsl:when
            test="not(contains($numStyle,'[$Ls-426]') or contains($numStyle,'[$kr-425]') or contains($numStyle,'[$€-813]')) and (contains($strippedFormat,'h') or contains($strippedFormat,'s') or $numId = 18 or $numId = 20)">
				<xsl:choose>
					
					<xsl:when test ="@t='d'">
						<xsl:attribute name="office:time-value">
								<xsl:call-template name="NumberToTime">
									<xsl:with-param name="value">
										<xsl:value-of select="e:v"/>
									</xsl:with-param>
									<xsl:with-param name ="isIsoDate" select ="'true'"/>
								</xsl:call-template>
							</xsl:attribute>
					</xsl:when>
					<xsl:otherwise>
            <xsl:attribute name="office:time-value">
              <xsl:call-template name="NumberToTime">
                <xsl:with-param name="value">
                  <xsl:value-of select="e:v"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:attribute>
					</xsl:otherwise>
				</xsl:choose>              
          </xsl:when>
          <xsl:otherwise>
            <!--RefNo-3:ODF1.1: Avoid office value if value type is string-->
           
             
             <xsl:if test="$valueType != 'string'">
            <xsl:attribute name="office:value">
              <xsl:value-of select="e:v"/>
            </xsl:attribute>
            </xsl:if>
            
            
          </xsl:otherwise>
        </xsl:choose>
        <!--Start of RefNo-3:ODF1.1:Added office:annotation abv text node-->
        <xsl:if test="$CheckIfNote = 'true'">
          <xsl:call-template name="InsertNoteInThisCell">
            <xsl:with-param name="rowNum">
              <xsl:value-of select="$rowNum"/>
            </xsl:with-param>
            <xsl:with-param name="colNum">
              <xsl:value-of select="$colNum"/>
            </xsl:with-param>
            <xsl:with-param name="sheetNr">
              <xsl:value-of select="$sheetNr"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:if>
        <!--End of RefNo-3-->
        <text:p>
          <xsl:choose>
            <xsl:when
              test="(contains($strippedFormat,'y') or (contains($strippedFormat,'m') and not(contains($strippedFormat,'h') or contains($strippedFormat,'s'))) or (contains($strippedFormat,'d') and not(contains($strippedFormat,'Red'))) or ($numId &gt; 13 and $numId &lt; 18) or $numId = 22)">             
              <xsl:variable name="formatCode">
                <xsl:choose>
                  <xsl:when test="$numStyle != '' ">
                    <xsl:value-of select="$numStyle"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:call-template name="GetBuiltInformatCode">
                      <xsl:with-param name="ID" select="$numId"/>
                    </xsl:call-template>
                  </xsl:otherwise>
                </xsl:choose>                
              </xsl:variable>              
              <xsl:call-template name="FormatDate">
                <xsl:with-param name="value">
                    <xsl:choose>                      
                      <xsl:when test ="@t='d'">
                        <xsl:variable name="DateWithTime">
                          <xsl:call-template name="CheckTime">
                            <xsl:with-param name="value">
                              <xsl:value-of select="e:v"/>
                            </xsl:with-param>
                          </xsl:call-template> 
                        </xsl:variable>
                        <xsl:value-of select ="$DateWithTime"/>
                      </xsl:when>                    
                      <xsl:otherwise>
                        <xsl:variable name ="temp">
                  <xsl:call-template name="NumberToDate">
                    <xsl:with-param name="value">
                      <xsl:value-of select="e:v"/>
                    </xsl:with-param>
                  </xsl:call-template>
                        </xsl:variable>
                        <xsl:value-of select ="$temp"/>
                      </xsl:otherwise>
                    </xsl:choose>                   
                </xsl:with-param>
                <xsl:with-param name="format">
                    <xsl:variable name ="temp2">
                  <xsl:choose>
                    <xsl:when test="contains($formatCode,']')">
                      <xsl:value-of select="substring-after($formatCode,']')"/>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:value-of select="$formatCode"/>
                    </xsl:otherwise>
                  </xsl:choose>
                    </xsl:variable>
                    <xsl:value-of select ="$temp2"/>
                </xsl:with-param>
                <xsl:with-param name="numId">
                  <xsl:value-of select="$numId"/>
                </xsl:with-param>
                <xsl:with-param name="processedFormat">
                    <xsl:variable name ="temp3">
                  <xsl:choose>
                    <xsl:when test="contains($formatCode,']')">
                      <xsl:value-of select="substring-after($formatCode,']')"/>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:value-of select="$formatCode"/>
                    </xsl:otherwise>
                  </xsl:choose>
                    </xsl:variable>
                    <xsl:value-of select ="$temp3"/>
                </xsl:with-param>
                <xsl:with-param name="numValue">
                  <xsl:value-of select="e:v"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:when>

            <!--'and' at the end is for Latvian currency -->
            <xsl:when
              test="contains($strippedFormat,'h') or contains($strippedFormat,'s') and not(contains($strippedFormat,'[$Ls-426]'))">              
              <xsl:variable name="formatCode">
                <xsl:choose>
                  <xsl:when test="$numStyle != '' ">
                    <xsl:value-of select="$numStyle"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:call-template name="GetBuiltInformatCode">
                      <xsl:with-param name="ID" select="$numId"/>
                    </xsl:call-template>
                  </xsl:otherwise>
                </xsl:choose>                
              </xsl:variable>              
              <xsl:call-template name="FormatTime">
                <xsl:with-param name="value">
					<xsl:choose>
						<xsl:when test ="@t='d'">
							<xsl:call-template name="NumberToTime">
								<xsl:with-param name="value">
									<xsl:value-of select="e:v"/>
								</xsl:with-param>
								<xsl:with-param name ="isIsoDate" select ="'true'"/>
							</xsl:call-template>							
						</xsl:when>
						<xsl:otherwise>							
                  <xsl:call-template name="NumberToTime">
                    <xsl:with-param name="value">
                      <xsl:value-of select="e:v"/>
                    </xsl:with-param>
									<xsl:with-param name ="isIsoDate" select ="'false'"/>
                  </xsl:call-template>
						</xsl:otherwise>
					</xsl:choose>
                </xsl:with-param>
                <xsl:with-param name="format">
                  <xsl:choose>
                    <xsl:when test="contains($formatCode,']') and not(contains($formatCode,'[h'))">
                      <xsl:value-of select="substring-after($formatCode,']')"/>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:value-of select="$formatCode"/>
                    </xsl:otherwise>
                  </xsl:choose>
                </xsl:with-param>
                <xsl:with-param name="numId">
                  <xsl:value-of select="$numId"/>
                </xsl:with-param>
                <xsl:with-param name="processedFormat">
                  <xsl:choose>
                    <xsl:when test="contains($formatCode,']') and not(contains($formatCode,'[h'))">
                      <xsl:value-of select="substring-after($formatCode,']')"/>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:value-of select="$formatCode"/>
                    </xsl:otherwise>
                  </xsl:choose>
                </xsl:with-param>
                <xsl:with-param name="numValue">
                  <xsl:value-of select="e:v"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:when>

            <xsl:otherwise>
              <xsl:call-template name="FormatNumber">
                <xsl:with-param name="value">
                  <xsl:value-of select="e:v"/>
                </xsl:with-param>
                <xsl:with-param name="numStyle">
                  <xsl:value-of select="$numStyle"/>
                </xsl:with-param>
                <xsl:with-param name="numId">
                  <xsl:value-of select="$numId"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:otherwise>
          </xsl:choose>
        </text:p>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="XlinkHref">
    <xsl:param name="sheetNr"/>

    <xsl:variable name="partId" select="ancestor::e:worksheet/@oox:part" />

    <xsl:variable name="target">
      <!-- path to sheet file from xl/ catalog (i.e. $sheet = worksheets/sheet1.xml) -->
      <xsl:for-each select="key('ref',@r)[@oox:part = $partId]">
        <xsl:call-template name="GetTarget">
          <xsl:with-param name="id">
            <xsl:value-of select="@r:id"/>
          </xsl:with-param>
          <xsl:with-param name="document">
            <xsl:value-of select="concat('xl/worksheets/sheet', $sheetNr, '.xml')"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:for-each>
    </xsl:variable>

    <xsl:choose>
      <!-- when hyperlink leads to a file in network -->
<!--Commenetd by vijayets to fix the defect 2682680-->
      <!--<xsl:when test="starts-with($target,'file:///\\')">
        <xsl:value-of select="translate(substring-after($target,'file:///'),'\','/')"/>
      </xsl:when>-->
<!--Commenetd by vijayets to fix the defect 2682680-->
      <!--when hyperlink leads to www or mailto -->
      <xsl:when test="contains($target,':')">
        <xsl:value-of select="$target"/>
      </xsl:when>
      <!-- when hyperlink leads to another place in workbook -->
      <xsl:when test="key('ref',@r)[@oox:part = $partId]/@location != '' ">
        <xsl:for-each select="key('ref',@r)[@oox:part = $partId]">

          <xsl:variable name="apos">
            <xsl:text>&apos;</xsl:text>
          </xsl:variable>

          <xsl:variable name="sheetName">
            <xsl:choose>
              <xsl:when test="starts-with(@location,$apos)">
                <xsl:value-of select="$apos"/>
                <xsl:value-of select="substring-before(substring-after(@location,$apos),$apos)"/>
                <xsl:value-of select="$apos"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="substring-before(@location,'!')"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:variable>

          <xsl:variable name="invalidChars">
            <xsl:text>&apos;!,.+$-()</xsl:text>
          </xsl:variable>

          <xsl:variable name="checkedName">
            <xsl:for-each
              select="key('Part', 'xl/workbook.xml')/e:workbook/e:sheets/e:sheet[@name = translate($sheetName,$apos,'')]">
              <xsl:call-template name="CheckSheetName">
                <xsl:with-param name="sheetNumber">
                  <xsl:for-each
                    select="key('Part', 'xl/workbook.xml')/e:workbook/e:sheets/e:sheet[@name = translate($sheetName,$apos,'')]">
                    <xsl:value-of select="count(preceding-sibling::e:sheet) + 1"/>
                  </xsl:for-each>
                </xsl:with-param>
                <xsl:with-param name="name">
                  <xsl:value-of select="translate($sheetName,$invalidChars,'')"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:for-each>
          </xsl:variable>

          <xsl:text>#</xsl:text>
          <xsl:value-of select="$checkedName"/>
          <xsl:text>.</xsl:text>
          <xsl:value-of select="substring-after(@location,concat($sheetName,'!'))"/>

        </xsl:for-each>
      </xsl:when>
      <!--when hyperlink leads to a document -->
      <xsl:otherwise>
        <xsl:call-template name="Change20PercentToSpace">
          <xsl:with-param name="string" select="$target"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <!-- change  '%20' to space  after conversion-->
  <xsl:template name="Change20PercentToSpace">
    <xsl:param name="string"/>

    <xsl:choose>
      <xsl:when test="contains($string,'%20')">
        <xsl:choose>
          <xsl:when test="substring-before($string,'%20') =''">
            <xsl:call-template name="Change20PercentToSpace">
              <xsl:with-param name="string" select="concat(' ',substring-after($string,'%20'))"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:when test="substring-before($string,'%20') !=''">
            <xsl:call-template name="Change20PercentToSpace">
              <xsl:with-param name="string"
                select="concat(substring-before($string,'%20'),' ',substring-after($string,'%20'))"
              />
            </xsl:call-template>
          </xsl:when>
        </xsl:choose>
      </xsl:when>

      <!--xsl:when test="contains($slash,'..\..\..\')">
          <xsl:choose>
          <xsl:when test="substring-before($slash,'..\..\..\') =''">
          <xsl:call-template name="Change20PercentToSpace">
          <xsl:with-param name="slash"
          select="concat('../../../',substring-after($slash,'..\..\..\'))"/>
          </xsl:call-template>
          </xsl:when>
          </xsl:choose>
        </xsl:when-->
      
      <xsl:when test="contains($string,'..\..\..\')">
        <xsl:choose>
          <xsl:when test="substring-before($string,'..\..\..\') =''">
            
            <xsl:call-template name="Change20PercentToSpace">
              <xsl:with-param name="string"
                select="concat('../../../',substring-after($string,'..\..\..\'))"/>
            </xsl:call-template>
          </xsl:when>
        </xsl:choose>
      </xsl:when>

      <xsl:when test="contains($string,'..\..\')">
        <xsl:choose>
          <xsl:when test="substring-before($string,'..\..\') =''">

            <xsl:call-template name="Change20PercentToSpace">
              <xsl:with-param name="string"
                select="concat('../../../',substring-after($string,'..\..\'))"/>
            </xsl:call-template>
          </xsl:when>
        </xsl:choose>
      </xsl:when>

      <xsl:when test="contains($string,'..\')">
        <xsl:choose>
          <xsl:when test="substring-before($string,'..\') =''">

            <xsl:call-template name="Change20PercentToSpace">
              <xsl:with-param name="string" select="concat('../../',substring-after($string,'..\'))"
              />
            </xsl:call-template>
          </xsl:when>
        </xsl:choose>
      </xsl:when>


      <xsl:when test="not(contains($string,'../')) and not(contains($string,'..\..\'))">

        <xsl:value-of select="concat('../',$string)"/>

      </xsl:when>

      <xsl:otherwise>
        <xsl:value-of select="$string"/>
        <!--xsl:value-of select="translate($string,'\','/')"/-->
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <!--  convert multiple white spaces  -->
  <xsl:template name="InsertWhiteSpaces">
    <xsl:param name="string" select="."/>
    <xsl:param name="length" select="string-length(.)"/>
    <!-- string which doesn't contain whitespaces-->
    <xsl:choose>
      <xsl:when test="not(contains($string,' '))">
        <xsl:value-of select="$string"/>
      </xsl:when>
      <!-- convert white spaces  -->
      <xsl:otherwise>
        <xsl:variable name="before">
          <xsl:value-of select="substring-before($string,' ')"/>
        </xsl:variable>
        <xsl:variable name="after">
          <xsl:call-template name="CutStartSpaces">
            <xsl:with-param name="cuted">
              <xsl:value-of select="substring-after($string,' ')"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:variable>
        <xsl:if test="$before != '' ">
          <xsl:value-of select="concat($before,' ')"/>
        </xsl:if>
        <!--add remaining whitespaces as text:s if there are any-->
        <xsl:if test="string-length(concat($before,' ', $after)) &lt; $length ">
          <xsl:choose>
            <xsl:when test="($length - string-length(concat($before, $after))) = 1">
              <text:s/>
            </xsl:when>
            <xsl:otherwise>
              <text:s>
                <xsl:attribute name="text:c">
                  <xsl:choose>
                    <xsl:when test="$before = ''">
                      <xsl:value-of select="$length - string-length($after)"/>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:value-of select="$length - string-length(concat($before,' ', $after))"/>
                    </xsl:otherwise>
                  </xsl:choose>
                </xsl:attribute>
              </text:s>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:if>
        <!--repeat it for substring which has whitespaces-->
        <xsl:if test="contains($string,' ') and $length &gt; 0">
          <xsl:call-template name="InsertWhiteSpaces">
            <xsl:with-param name="string">
              <xsl:value-of select="$after"/>
            </xsl:with-param>
            <xsl:with-param name="length">
              <xsl:value-of select="string-length($after)"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:if>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!--  cut start spaces -->
  <xsl:template name="CutStartSpaces">
    <xsl:param name="cuted"/>
    <xsl:choose>
      <xsl:when test="starts-with($cuted,' ')">
        <xsl:call-template name="CutStartSpaces">
          <xsl:with-param name="cuted">
            <xsl:value-of select="substring-after($cuted,' ')"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$cuted"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="CountContinuous">
    <xsl:param name="count" select="0"/>

    <xsl:variable name="carryOn">
      <xsl:choose>
        <xsl:when test="following-sibling::e:c[1]/@s and not(following-sibling::e:c[1]/e:v)">
          <xsl:variable name="position">
            <xsl:value-of select="following-sibling::e:c[1]/@s"/>
          </xsl:variable>
          <xsl:variable name="horizontal">
            <xsl:value-of select="key('Xf', $position)/e:alignment/@horizontal"/>
          </xsl:variable>

          <xsl:choose>
            <xsl:when test="$horizontal = 'centerContinuous' ">
              <xsl:text>true</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>false</xsl:text>
            </xsl:otherwise>
          </xsl:choose>

        </xsl:when>
        <xsl:otherwise>
          <xsl:text>false</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="$carryOn = 'true'">
        <xsl:for-each select="following-sibling::e:c[1]">
          <xsl:call-template name="CountContinuous">
            <xsl:with-param name="count" select="$count + 1"/>
          </xsl:call-template>
        </xsl:for-each>
      </xsl:when>
      <xsl:otherwise>
        <!-- number of following cells plus the starting one -->
        <xsl:value-of select="$count + 1"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>


  <xsl:template name="CheckTime">
    <xsl:param name="value"/>
    <xsl:choose>
      <xsl:when test="not(contains($value,'T'))">     
		  <xsl:choose>
			  <xsl:when test ="contains($value,':') or contains($value,'.')">
				  <xsl:variable name="YearAndDay">
					  <xsl:call-template name="CalculateYearAndDay">
						  <xsl:with-param name="days">							  
						     <xsl:value-of select="0"/>								 
						  </xsl:with-param>
						  <xsl:with-param name="currentYear">1900</xsl:with-param>
					  </xsl:call-template>
				  </xsl:variable>

				  <!-- year -->
				  <xsl:variable name="years">
					  <xsl:value-of select="substring-before($YearAndDay,':') - 1900"/>
				  </xsl:variable>

				  <xsl:variable name="dateInLastYear">
					  <xsl:value-of select="substring-after($YearAndDay,':')"/>
				  </xsl:variable>

				  <!-- month and day -->
				  <xsl:variable name="monthsAndDays">
					  <xsl:call-template name="DaysToMonthsAndDays">
						  <xsl:with-param name="days">
							  <xsl:value-of select="$dateInLastYear"/>
						  </xsl:with-param>
						  <xsl:with-param name="ExtraDay">
							  <xsl:choose>
								  <xsl:when
									test="((1900 + $years - 4 * floor((1900 + $years) div 4)) = 0 and (1900 + $years - 100 * floor((1900 + $years) div 100) != 0)) or (1900 + $years - 400 * floor((1900 + $years) div 400)) = 0"
                                            >1</xsl:when>
								  <xsl:otherwise>0</xsl:otherwise>
							  </xsl:choose>
						  </xsl:with-param>
					  </xsl:call-template>
				  </xsl:variable>
				  <xsl:value-of
					select="concat((1900+$years),'-',$monthsAndDays,'T',$value)"/>
			  </xsl:when>
			  <xsl:otherwise>
        <xsl:value-of select="concat($value,'T','00:00:00')"/>        
			  </xsl:otherwise>
		  </xsl:choose>             
      </xsl:when>
      <xsl:when test="(contains($value,'T'))">
        <xsl:value-of select="e:v"/>         
      </xsl:when>
    </xsl:choose>
   
    
  </xsl:template>


</xsl:stylesheet>
