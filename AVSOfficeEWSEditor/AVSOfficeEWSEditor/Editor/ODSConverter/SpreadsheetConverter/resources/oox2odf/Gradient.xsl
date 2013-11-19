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
    xmlns:pzip="urn:cleverage:xmlns:post-processings:zip"
    xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
    xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
    xmlns:number="urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0"
    xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
    xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
    xmlns:chart="urn:oasis:names:tc:opendocument:xmlns:chart:1.0"
    xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
    xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
    xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
    xmlns:xdr="http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing"
    xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main"
    xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart"
    xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:dc="http://purl.org/dc/elements/1.1/"
    xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
    xmlns:e="http://schemas.openxmlformats.org/spreadsheetml/2006/main"
    xmlns:oox="urn:oox"
    exclude-result-prefixes="e r c a xdr oox">
    
    <!-- @Filename: chart.xsl -->
    <!-- @Description: This stylesheet is used for charts conversion -->
    <!-- @Created: 2007-05-24 -->
    
    <!--<xsl:import href="relationships.xsl"/>-->
    
      <xsl:template name="tmpGradientFillTiletoRect">
    <xsl:param name="SMName"/>
    <xsl:for-each select="a:gsLst/a:gs">
      <xsl:if test="position()=1">
        <xsl:choose>
          <xsl:when test="a:srgbClr/@val">
            <xsl:attribute name="draw:start-color">
              <!--<xsl:value-of select="concat('#',a:srgbClr/@val)" />-->
              <xsl:call-template name="tmpgetColorCode">
                <xsl:with-param name="color">
                  <xsl:value-of select="a:srgbClr/@val" />
                </xsl:with-param>
                <xsl:with-param name="lumMod">
                  <xsl:value-of select="a:srgbClr/a:lumMod/@val" />
                </xsl:with-param>
                <xsl:with-param name="lumOff">
                  <xsl:value-of select="a:srgbClr/a:lumOff/@val" />
                </xsl:with-param>
                <xsl:with-param name ="shade">
                  <xsl:for-each select="a:srgbClr/a:shade/@val">
                    <xsl:value-of select=". div 1000"/>
                  </xsl:for-each>
                </xsl:with-param>
                <xsl:with-param name ="tint">
                  <xsl:choose>
                    <xsl:when test="a:srgbClr/a:tint/@val">
                      <xsl:for-each select="a:srgbClr/a:tint/@val">
                        <xsl:value-of select=". div 100000"/>
                      </xsl:for-each>
                    </xsl:when>
                  </xsl:choose>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:attribute>
          </xsl:when>
          <xsl:when test="a:schemeClr/@val">
            <xsl:variable name="var_schemeClr" select="a:schemeClr/@val"/>
            <xsl:attribute name="draw:start-color">
              <xsl:call-template name="tmpgetColorCode">
                <xsl:with-param name="color">
                <xsl:value-of select="a:schemeClr/@val" />
                 </xsl:with-param>
                <xsl:with-param name="lumMod">
                  <xsl:value-of select="a:schemeClr/a:lumMod/@val" />
                </xsl:with-param>
                <xsl:with-param name="lumOff">
                  <xsl:value-of select="a:schemeClr/a:lumOff/@val" />
                </xsl:with-param>
                <xsl:with-param name ="shade">
                  <xsl:for-each select="a:schemeClr/a:shade/@val">
                    <xsl:value-of select=". div 1000"/>
                  </xsl:for-each>
                </xsl:with-param>
                <xsl:with-param name ="tint">
                  <xsl:choose>
                    <xsl:when test="a:schemeClr/a:tint/@val">
                      <xsl:for-each select="a:schemeClr/a:tint/@val">
                        <xsl:value-of select=". div 100000"/>
                      </xsl:for-each>
                    </xsl:when>
                  </xsl:choose>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:attribute>
          </xsl:when>
        </xsl:choose>
      </xsl:if>
      <xsl:if test="position()=last()">
        <xsl:choose>
          <xsl:when test="a:srgbClr/@val">
            <xsl:attribute name="draw:end-color">
              <!--<xsl:value-of select="concat('#',a:srgbClr/@val)" />-->
              <xsl:call-template name="tmpgetColorCode">
                <xsl:with-param name="color">
                  <xsl:value-of select="a:srgbClr/@val" />
                </xsl:with-param>
                <xsl:with-param name="lumMod">
                  <xsl:value-of select="a:srgbClr/a:lumMod/@val" />
                </xsl:with-param>
                <xsl:with-param name="lumOff">
                  <xsl:value-of select="a:srgbClr/a:lumOff/@val" />
                </xsl:with-param>
                <xsl:with-param name ="shade">
                  <xsl:for-each select="a:srgbClr/a:shade/@val">
                    <xsl:value-of select=". div 1000"/>
                  </xsl:for-each>
                </xsl:with-param>
                <xsl:with-param name ="tint">
                  <xsl:choose>
                    <xsl:when test="a:srgbClr/a:tint/@val">
                      <xsl:for-each select="a:srgbClr/a:tint/@val">
                        <xsl:value-of select=". div 100000"/>
                      </xsl:for-each>
                    </xsl:when>
                  </xsl:choose>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:attribute>
          </xsl:when>
          <xsl:when test="a:schemeClr/@val">
            <xsl:variable name="var_schemeClr" select="a:schemeClr/@val"/>
            <xsl:attribute name="draw:end-color">
              <xsl:call-template name="tmpgetColorCode">
                <xsl:with-param name="color">
                  <!--<xsl:value-of select="a:schemeClr/@val"/>-->
                  <!--xsl:choose>
                    <xsl:when test="$SMName!=''">
                      <xsl:for-each select="document(concat('ppt/slideMasters/',$SMName))//p:clrMap">
                        <xsl:call-template name="tmpThemeClr">
                          <xsl:with-param name="ClrMap" select="$var_schemeClr"/>
                        </xsl:call-template>
                      </xsl:for-each>
                    </xsl:when>
                    <xsl:otherwise-->
                      <xsl:value-of select="a:schemeClr/@val" />
                    <!--/xsl:otherwise>
                  </xsl:choose-->
                </xsl:with-param>
                <xsl:with-param name="lumMod">
                  <xsl:value-of select="a:schemeClr/a:lumMod/@val" />
                </xsl:with-param>
                <xsl:with-param name="lumOff">
                  <xsl:value-of select="a:schemeClr/a:lumOff/@val" />
                </xsl:with-param>
                <xsl:with-param name ="shade">
                  <xsl:for-each select="a:schemeClr/a:shade/@val">
                    <xsl:value-of select=". div 1000"/>
                  </xsl:for-each>
                </xsl:with-param>
                <xsl:with-param name ="tint">
                  <xsl:choose>
                    <xsl:when test="a:schemeClr/a:tint/@val">
                      <xsl:for-each select="a:schemeClr/a:tint/@val">
                        <xsl:value-of select=". div 100000"/>
                      </xsl:for-each>
                    </xsl:when>
                  </xsl:choose>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:attribute>
          </xsl:when>
        </xsl:choose>
      </xsl:if>
    </xsl:for-each>
        
    <xsl:for-each select="a:lin">
      <xsl:if test="@ang">
        <xsl:attribute name="draw:angle">
          <xsl:variable name="angleValue">
            <xsl:value-of select="round((((( -1 * @ang ) + 5400000 ) mod 21600000 ) div 60000) * 10)"/>
          </xsl:variable>
          <xsl:choose>
            <xsl:when test="$angleValue &lt; 0">
              <xsl:value-of select="-1 * $angleValue "/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$angleValue "/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
      </xsl:if>
      <xsl:attribute name="draw:style">
        <xsl:value-of select="'linear'"/>
      </xsl:attribute>
    </xsl:for-each>
        
    <xsl:for-each select="a:path">
      <xsl:choose>
        <xsl:when test="@path='circle'">
          <xsl:attribute name="draw:style">
            <xsl:value-of select="'radial'"/>
          </xsl:attribute>
        </xsl:when>
        <xsl:when test="@path='shape'">
          <xsl:attribute name="draw:style">
            <xsl:value-of select="'ellipsoid'"/>
          </xsl:attribute>
        </xsl:when>
        <xsl:when test="@path='rect'">
          <xsl:attribute name="draw:style">
            <xsl:value-of select="'rectangular'"/>
          </xsl:attribute>
        </xsl:when>
      </xsl:choose>
      <xsl:for-each select="a:fillToRect">
        <xsl:choose>
          <xsl:when test="@l and @r and @t and @b">
            <xsl:attribute name="draw:cx">
              <xsl:variable name="var_cx">
                <xsl:value-of select="number(format-number(( (@l+ @r) div 2 ) div 1000,'#')) "/>
              </xsl:variable>
              <xsl:value-of select="concat(100 - $var_cx,'%') "/>
            </xsl:attribute>
            <xsl:attribute name="draw:cy">
              <xsl:variable name="var_cy">
                <xsl:value-of select="number(format-number(( (@t+ @b) div 2 ) div 1000,'#')) "/>
              </xsl:variable>
              <xsl:value-of select="concat(100 - $var_cy,'%') "/>
            </xsl:attribute>
          </xsl:when>
          <xsl:when test="@l and @b">
            <xsl:attribute name="draw:cx">
              <xsl:variable name="var_cx">
                <xsl:value-of select="number(format-number( @l  div 1000,'#')) "/>
              </xsl:variable>
              <xsl:value-of select="concat($var_cx,'%') "/>
            </xsl:attribute>
            <xsl:attribute name="draw:cy">
              <xsl:value-of select="'0%' "/>
            </xsl:attribute>
          </xsl:when>
          <xsl:when test="@l and @t">
            <xsl:attribute name="draw:cx">
              <xsl:variable name="var_cx">
                <xsl:value-of select="number(format-number( @l  div 1000,'#')) "/>
              </xsl:variable>
              <xsl:value-of select="concat($var_cx,'%') "/>
            </xsl:attribute>
            <xsl:attribute name="draw:cy">
              <xsl:variable name="var_cy">
                <xsl:value-of select="number(format-number( @t  div 1000,'#')) "/>
              </xsl:variable>
              <xsl:value-of select="concat($var_cy,'%') "/>
            </xsl:attribute>
          </xsl:when>
          <xsl:when test="@r and @b">
            <xsl:attribute name="draw:cx">
              <xsl:value-of select="'0%' "/>
            </xsl:attribute>
            <xsl:attribute name="draw:cy">
              <xsl:value-of select="'0%' "/>
            </xsl:attribute>
          </xsl:when>
          <xsl:when test="@r and @t">
            <xsl:attribute name="draw:cx">
              <xsl:value-of select="'0%' "/>
            </xsl:attribute>
            <xsl:attribute name="draw:cy">
              <xsl:variable name="var_cy">
                <xsl:value-of select="number(format-number( @t  div 1000,'#')) "/>
              </xsl:variable>
              <xsl:value-of select="concat($var_cy,'%') "/>
            </xsl:attribute>
          </xsl:when>
          <xsl:otherwise>
            <xsl:attribute name="draw:cx">
              <xsl:value-of select="'100%' "/>
            </xsl:attribute>
            <xsl:attribute name="draw:cy">
              <xsl:value-of select="'100%' "/>
            </xsl:attribute>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:for-each>
    </xsl:for-each>
  </xsl:template>
  
  <xsl:template name="tmpgetColorCode">
    <xsl:param name="color"/>
    <xsl:param name ="lumMod"/>
    <xsl:param name ="lumOff"/>
    <xsl:param  name ="shade"/>
    <xsl:param name ="tint"/>
    <xsl:message terminate="no">progress:a:p</xsl:message>
    <xsl:message terminate="no">progress:p:cSld</xsl:message>
    <xsl:variable name ="ThemeColor">
      <xsl:for-each select ="document('xl/theme/theme1.xml')/a:theme/a:themeElements/a:clrScheme">
        <xsl:for-each select ="node()">
          <xsl:if test ="name() =concat('a:',$color)">
            <xsl:choose >
              <xsl:when test ="contains(node()/@val,'window') ">
                <xsl:value-of select ="node()/@lastClr"/>
              </xsl:when>
              <xsl:otherwise >
                <xsl:value-of select ="node()/@val"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:if>
        </xsl:for-each>
      </xsl:for-each>			
    </xsl:variable>
    <xsl:variable name ="BgTxColors">
      <xsl:if test ="$color ='bg2'">
        <xsl:value-of select ="document('xl/theme/theme1.xml') //a:lt2/a:srgbClr/@val"/>
      </xsl:if>
      <xsl:if test ="$color ='bg1'">
        <!--<xsl:value-of select ="document('xl/theme/theme1.xml') //a:lt1/node()/@lastClr" />-->
        <xsl:choose>
          <xsl:when test ="document('xl/theme/theme1.xml') //a:lt1/a:srgbClr/@val">
            <xsl:value-of select ="document('xl/theme/theme1.xml') //a:lt1/a:srgbClr/@val" />
          </xsl:when>
          <xsl:when test="document('xl/theme/theme1.xml') //a:lt1/node()/@lastClr">
            <xsl:value-of select ="document('xl/theme/theme1.xml') //a:lt1/node()/@lastClr" />
          </xsl:when>
        </xsl:choose>
      </xsl:if>
      <!--<xsl:if test ="$color ='tx1'">
        <xsl:value-of select ="document('xl/theme/theme1.xml') //a:dk1/node()/@lastClr"/>
        </xsl:if>-->
      <xsl:if test ="$color ='tx1'">
        <xsl:choose>
          <xsl:when test ="document('xl/theme/theme1.xml') //a:dk1/node()/@lastClr">
            <xsl:value-of select ="document('xl/theme/theme1.xml') //a:dk1/node()/@lastClr"/>
          </xsl:when>
          <xsl:when test ="document('xl/theme/theme1.xml') //a:dk1/node()/@val">
            <xsl:value-of select ="document('xl/theme/theme1.xml') //a:dk1/node()/@val"/>
          </xsl:when>
        </xsl:choose>
      </xsl:if>
      <xsl:if test ="$color ='tx2'">
        <xsl:value-of select ="document('xl/theme/theme1.xml') //a:dk2/a:srgbClr/@val"/>
      </xsl:if>			
    </xsl:variable>
    <xsl:variable name ="NewColor">
      <xsl:choose>
        <xsl:when test ="$ThemeColor != ''">
        <xsl:value-of select ="$ThemeColor"/>
        </xsl:when>
        <xsl:when test ="$BgTxColors !=''">
        <xsl:value-of select ="$BgTxColors"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select ="$color"/>
        </xsl:otherwise>
      </xsl:choose>

    </xsl:variable>
    <xsl:call-template name ="ConverThemeColor">
      <xsl:with-param name="color" select="$NewColor" />
      <xsl:with-param name ="lumMod" select ="$lumMod"/>
      <xsl:with-param name ="lumOff" select ="$lumOff"/>
      <xsl:with-param name ="shade" select ="$shade"/>
      <xsl:with-param name ="tint" select ="$tint"/>
    </xsl:call-template>
  </xsl:template>
  
  <xsl:template name="ConverThemeColor">
    <xsl:param name="color"/>
    <xsl:param name ="lumMod"/>
    <xsl:param name ="lumOff"/>
    <xsl:param name ="shade"/>
    <xsl:param name ="tint"/>
    <xsl:message terminate="no">progress:p:cSld</xsl:message>
    <xsl:variable name ="Red">
      <xsl:call-template name ="HexToDec">
        <xsl:with-param name ="number" select ="substring($color,1,2)"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name ="Green">
      <xsl:call-template name ="HexToDec">
        <xsl:with-param name ="number" select ="substring($color,3,2)"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name ="Blue">
      <xsl:call-template name ="HexToDec">
        <xsl:with-param name ="number" select ="substring($color,5,2)"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="$shade!='' and $tint=''">
      <xsl:call-template name ="calculateShade">
        <xsl:with-param name ="Red" select ="$Red" />
        <xsl:with-param name ="Green" select ="$Green"/>
        <xsl:with-param name ="Blue" select ="$Blue"/>
        <xsl:with-param name ="shade" select ="$shade"/>
      </xsl:call-template >
      </xsl:when>
      <xsl:when test="$tint!='' and $shade=''">
        <xsl:value-of select="'#'"/>
        <!--<xsl:value-of select="'#FFFF00'"/>-->
        <xsl:call-template name="CalculateTintedColor">
          <xsl:with-param name="color" select="$color"/>
          <xsl:with-param name="tint" select="$tint"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="$tint='' and $shade=''">
      <xsl:choose >
        <xsl:when test ="$lumOff = '' and $lumMod != '' ">
          <xsl:variable name ="NewRed">
            <xsl:value-of  select =" floor($Red * $lumMod div 100000) "/>
          </xsl:variable>
          <xsl:variable name ="NewGreen">
            <xsl:value-of  select =" floor($Green * $lumMod div 100000)"/>
          </xsl:variable>
          <xsl:variable name ="NewBlue">
            <xsl:value-of  select =" floor($Blue * $lumMod div 100000)"/>
          </xsl:variable>
          <xsl:call-template name ="CreateRGBColor">
            <xsl:with-param name ="Red" select ="$NewRed"/>
            <xsl:with-param name ="Green" select ="$NewGreen"/>
            <xsl:with-param name ="Blue" select ="$NewBlue"/>
          </xsl:call-template>
        </xsl:when>			
        <xsl:when test ="$lumMod = '' and $lumOff != ''">
          <!-- TBD Not sure whether this condition will occure-->
        </xsl:when>
        <xsl:when test ="$lumMod = '' and $lumOff =''">
          <xsl:value-of  select ="concat('#',$color)"/>
        </xsl:when>
        <xsl:when test ="$lumOff != '' and $lumMod!= '' ">
          <xsl:variable name ="NewRed">
            <xsl:value-of select ="floor(((255 - $Red) * (1 - ($lumMod  div 100000)))+ $Red )"/>
          </xsl:variable>
          <xsl:variable name ="NewGreen">
            <xsl:value-of select ="floor(((255 - $Green) * ($lumOff  div 100000)) + $Green )"/>
          </xsl:variable>
          <xsl:variable name ="NewBlue">
            <xsl:value-of select ="floor(((255 - $Blue) * ($lumOff div 100000)) + $Blue) "/>
          </xsl:variable>
          <xsl:call-template name ="CreateRGBColor">
            <xsl:with-param name ="Red" select ="$NewRed"/>
            <xsl:with-param name ="Green" select ="$NewGreen"/>
            <xsl:with-param name ="Blue" select ="$NewBlue"/>
          </xsl:call-template>
        </xsl:when>
      </xsl:choose>
      </xsl:when>
    </xsl:choose>
  </xsl:template>
  
  <xsl:template name ="calculateShade">
    <xsl:param name ="Red"/>
    <xsl:param name ="Green"/>
    <xsl:param name ="Blue"/>
    <xsl:param name ="shade"/>
    <xsl:value-of select ="concat('shade-tint:',$Red,':',$Green,':',$Blue ,':',$shade )"/>
  </xsl:template>
  <!-- Converts Hexa Decimal Value to Decimal-->
  <xsl:template name="HexToDec">
    <!-- @Description: This is a recurive algorithm converting a hex to decimal -->
    <!-- @Context: None -->
    <xsl:param name="number"/>
    <!-- (string|number) The hex number to convert -->
    <xsl:param name="step" select="0"/>
    <!-- (number) The exponent (only used during convertion)-->
    <xsl:param name="value" select="0"/>
    <!-- (number) The result from the previous digit's convertion (only used during convertion) -->
    <xsl:variable name="number1">
      <!-- translates all letters to lower case -->
      <xsl:value-of select="translate($number,'ABCDEF','abcdef')"/>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="string-length($number1) &gt; 0">
        <xsl:variable name="one">
          <!-- The last digit in the hex number -->
          <xsl:choose>
            <xsl:when test="substring($number1,string-length($number1) ) = 'a'">
              <xsl:text>10</xsl:text>
            </xsl:when>
            <xsl:when test="substring($number1,string-length($number1)) = 'b'">
              <xsl:text>11</xsl:text>
            </xsl:when>
            <xsl:when test="substring($number1,string-length($number1)) = 'c'">
              <xsl:text>12</xsl:text>
            </xsl:when>
            <xsl:when test="substring($number1,string-length($number1)) = 'd'">
              <xsl:text>13</xsl:text>
            </xsl:when>
            <xsl:when test="substring($number1,string-length($number1)) = 'e'">
              <xsl:text>14</xsl:text>
            </xsl:when>
            <xsl:when test="substring($number1,string-length($number1)) = 'f'">
              <xsl:text>15</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="substring($number1,string-length($number1))"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        <xsl:variable name="power">
          <!-- The result of the exponent calculation -->
          <xsl:call-template name="Power">
            <xsl:with-param name="base">16</xsl:with-param>
            <xsl:with-param name="exponent">
              <xsl:value-of select="number($step)"/>
            </xsl:with-param>
            <xsl:with-param name="value1">16</xsl:with-param>
          </xsl:call-template>
        </xsl:variable>
        <xsl:choose>
          <xsl:when test="string-length($number1) = 1">
            <xsl:value-of select="($one * $power )+ number($value)"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="HexToDec">
              <xsl:with-param name="number">
                <xsl:value-of select="substring($number1,1,string-length($number1) - 1)"/>
              </xsl:with-param>
              <xsl:with-param name="step">
                <xsl:value-of select="number($step) + 1"/>
              </xsl:with-param>
              <xsl:with-param name="value">
                <xsl:value-of select="($one * $power) + number($value)"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
    </xsl:choose>
  </xsl:template>
  
  <!-- Converts Decimal Value to Hexadecimal-->
  <xsl:template name="DecToHex">
    <xsl:param name="number"/>
    <xsl:variable name="high">
      <xsl:call-template name="HexMap">
        <xsl:with-param name="value">
          <xsl:value-of select="floor($number div 16)"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="low">
      <xsl:call-template name="HexMap">
        <xsl:with-param name="value">
          <xsl:value-of select="$number mod 16"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>
    <xsl:value-of select="concat($high,$low)"/>
  </xsl:template>
  
  <!-- calculates power function -->
  <xsl:template name="HexMap">
    <xsl:param name="value"/>
    <xsl:choose>
      <xsl:when test="$value = 10">
        <xsl:text>A</xsl:text>
      </xsl:when>
      <xsl:when test="$value = 11">
        <xsl:text>B</xsl:text>
      </xsl:when>
      <xsl:when test="$value = 12">
        <xsl:text>C</xsl:text>
      </xsl:when>
      <xsl:when test="$value = 13">
        <xsl:text>D</xsl:text>
      </xsl:when>
      <xsl:when test="$value = 14">
        <xsl:text>E</xsl:text>
      </xsl:when>
      <xsl:when test="$value = 15">
        <xsl:text>F</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$value"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  
  <xsl:template name ="CreateRGBColor">
    <xsl:param name ="Red"/>
    <xsl:param name ="Green"/>
    <xsl:param name ="Blue"/>
    <xsl:message terminate="no">progress:p:cSld</xsl:message>
    <xsl:variable name ="NewRed">
      <xsl:call-template name ="DecToHex">
        <xsl:with-param name ="number">
          <xsl:choose>
            <xsl:when test="$Red != 'NaN'">
              <xsl:value-of select="$Red"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>0</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:with-param>        
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name ="NewGreen">
      <xsl:call-template name ="DecToHex">
        <xsl:with-param name ="number">
          <xsl:choose>
            <xsl:when test="$Green != 'NaN'">
              <xsl:value-of select="$Green"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>0</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name ="NewBlue">
      <xsl:call-template name ="DecToHex">
        <xsl:with-param name ="number">
          <xsl:choose>
            <xsl:when test="$Blue != 'NaN'">
              <xsl:value-of select="$Blue"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>0</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="lcletters">abcdefghijklmnopqrstuvwxyz</xsl:variable>
    <xsl:variable name="ucletters">ABCDEFGHIJKLMNOPQRSTUVWXYZ</xsl:variable>
    <xsl:value-of  select ="translate(concat('#',$NewRed,$NewGreen,$NewBlue),ucletters,lcletters)"/>
  </xsl:template>
  
  <xsl:template name="Power">
    <xsl:param name="base"/>
    <xsl:param name="exponent"/>
    <xsl:param name="value1" select="$base"/>
    <xsl:choose>
      <xsl:when test="$exponent = 0">
        <xsl:text>1</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:choose>
          <xsl:when test="$exponent &gt; 1">
            <xsl:call-template name="Power">
              <xsl:with-param name="base">
                <xsl:value-of select="$base"/>
              </xsl:with-param>
              <xsl:with-param name="exponent">
                <xsl:value-of select="$exponent -1"/>
              </xsl:with-param>
              <xsl:with-param name="value1">
                <xsl:value-of select="$value1 * $base"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$value1"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

</xsl:stylesheet>
