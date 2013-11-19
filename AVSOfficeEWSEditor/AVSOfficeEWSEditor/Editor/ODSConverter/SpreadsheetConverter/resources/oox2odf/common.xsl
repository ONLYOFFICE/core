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
Date        |ModifiedBy  |BugNo.   |Modification                                                      |
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
18-July-2008 Sateesh      1803299   Modification done to GetBuildInColor Template 
									Updated index values according to ECMA-376.pdf Standard 
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
  xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main"
  xmlns:e="http://schemas.openxmlformats.org/spreadsheetml/2006/main" exclude-result-prefixes="e r">

  <!-- gets a column number from cell coordinates -->
  <xsl:template name="GetColNum">
    <xsl:param name="cell"/>
    <xsl:param name="columnId"/>

    <!--xsl:value-of select="substring-before($cell, '|')"/-->
    <xsl:choose>
    <!-- when whole literal column id has been extracted than convert alphabetic index to number -->
    <xsl:when test="number(substring($cell,1,1))">
        <xsl:call-template name="GetAlphabeticPosition">
          <xsl:with-param name="literal" select="$columnId"/>
        </xsl:call-template>
      </xsl:when>
    <!--  recursively extract literal column id (i.e if $cell='GH15' it will return 'GH') -->
    <xsl:otherwise>
      <xsl:choose>
        <xsl:when test="$cell != ''">
          <xsl:call-template name="GetColNum">
            <xsl:with-param name="cell" select="substring-after($cell,substring($cell,1,1))"/>
            <xsl:with-param name="columnId" select="concat($columnId,substring($cell,1,1))"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="-1"/>
        </xsl:otherwise>
      </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="GetColNum2">
    <xsl:param name="cell"/>

    <xsl:value-of select="substring-before($cell, '|')"/>
    
  </xsl:template>

  <xsl:template name="GetRowNum2">
    <xsl:param name="cell"/>

    <xsl:value-of select="substring-after($cell, '|')"/>
    
  </xsl:template>

  <!-- gets a row number from cell coordinates -->
  <xsl:template name="GetRowNum">
    <xsl:param name="cell"/>

    <!--xsl:value-of select="substring-after($cell, '|')"/-->
    <xsl:choose>
      <xsl:when test="number($cell)">
        <xsl:value-of select="$cell"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:choose>
          <xsl:when test="$cell != ''">
            <xsl:call-template name="GetRowNum">
              <xsl:with-param name="cell" select="substring-after($cell,substring($cell,1,1))"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="-1"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- translates literal index to number -->
  <xsl:template name="GetAlphabeticPosition">
    <xsl:param name="literal"/>
    <xsl:param name="number" select="0"/>
    <xsl:param name="level" select="0"/>

    <xsl:variable name="lastCharacter">
      <xsl:value-of select="substring($literal,string-length($literal),1)"/>
    </xsl:variable>

    <xsl:variable name="lastCharacterPosition">
      <xsl:call-template name="CharacterToPosition">
        <xsl:with-param name="character" select="$lastCharacter"/>
      </xsl:call-template>
    </xsl:variable>
    
    <xsl:variable name="power">
      <xsl:call-template name="Power">
        <xsl:with-param name="base" select="26"/>
        <xsl:with-param name="exponent" select="$level"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="string-length($literal) &gt; 1">
        <xsl:call-template name="GetAlphabeticPosition">
          <xsl:with-param name="literal" select="substring($literal,1,string-length($literal) - 1)"/>
          <xsl:with-param name="level" select="$level+1"/>
          <xsl:with-param name="number">
            <xsl:value-of select="$lastCharacterPosition*$power + $number"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$lastCharacterPosition*$power + $number"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- returns position in alphabet of a single character-->
  <xsl:template name="CharacterToPosition">
    <xsl:param name="character"/>

    <xsl:choose>
      <xsl:when test="$character='A'">1</xsl:when>
      <xsl:when test="$character='B'">2</xsl:when>
      <xsl:when test="$character='C'">3</xsl:when>
      <xsl:when test="$character='D'">4</xsl:when>
      <xsl:when test="$character='E'">5</xsl:when>
      <xsl:when test="$character='F'">6</xsl:when>
      <xsl:when test="$character='G'">7</xsl:when>
      <xsl:when test="$character='H'">8</xsl:when>
      <xsl:when test="$character='I'">9</xsl:when>
      <xsl:when test="$character='J'">10</xsl:when>
      <xsl:when test="$character='K'">11</xsl:when>
      <xsl:when test="$character='L'">12</xsl:when>
      <xsl:when test="$character='M'">13</xsl:when>
      <xsl:when test="$character='N'">14</xsl:when>
      <xsl:when test="$character='O'">15</xsl:when>
      <xsl:when test="$character='P'">16</xsl:when>
      <xsl:when test="$character='Q'">17</xsl:when>
      <xsl:when test="$character='R'">18</xsl:when>
      <xsl:when test="$character='S'">19</xsl:when>
      <xsl:when test="$character='T'">20</xsl:when>
      <xsl:when test="$character='U'">21</xsl:when>
      <xsl:when test="$character='V'">22</xsl:when>
      <xsl:when test="$character='W'">23</xsl:when>
      <xsl:when test="$character='X'">24</xsl:when>
      <xsl:when test="$character='Y'">25</xsl:when>
      <xsl:when test="$character='Z'">26</xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- converting number of column to char name of column -->
  <xsl:template name="NumbersToChars">
    <xsl:param name="num"/>
    <xsl:choose>
      <xsl:when test="$num = 0">A</xsl:when>
      <xsl:when test="$num = 1">B</xsl:when>
      <xsl:when test="$num = 2">C</xsl:when>
      <xsl:when test="$num = 3">D</xsl:when>
      <xsl:when test="$num = 4">E</xsl:when>
      <xsl:when test="$num = 5">F</xsl:when>
      <xsl:when test="$num = 6">G</xsl:when>
      <xsl:when test="$num = 7">H</xsl:when>
      <xsl:when test="$num = 8">I</xsl:when>
      <xsl:when test="$num = 9">J</xsl:when>
      <xsl:when test="$num = 10">K</xsl:when>
      <xsl:when test="$num = 11">L</xsl:when>
      <xsl:when test="$num = 12">M</xsl:when>
      <xsl:when test="$num = 13">N</xsl:when>
      <xsl:when test="$num = 14">O</xsl:when>
      <xsl:when test="$num = 15">P</xsl:when>
      <xsl:when test="$num = 16">Q</xsl:when>
      <xsl:when test="$num = 17">R</xsl:when>
      <xsl:when test="$num = 18">S</xsl:when>
      <xsl:when test="$num = 19">T</xsl:when>
      <xsl:when test="$num = 20">U</xsl:when>
      <xsl:when test="$num = 21">V</xsl:when>
      <xsl:when test="$num = 22">W</xsl:when>
      <xsl:when test="$num = 23">X</xsl:when>
      <xsl:when test="$num = 24">Y</xsl:when>
      <xsl:when test="$num = 25">Z</xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="NumbersToChars">
          <xsl:with-param name="num">
            <xsl:value-of select="floor($num div 26)-1"/>
          </xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="NumbersToChars">
          <xsl:with-param name="num">
            <xsl:value-of select="$num - 26*floor($num div 26)"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- conversion based on http://support.microsoft.com/kb/29240 -->
  <xsl:template name="RGBtoHLS">
    <xsl:param name="rgb"/>

    <xsl:variable name="HLSMAX" select="255"/>
    <xsl:variable name="RGBMAX" select="255"/>
    <xsl:variable name="UNDEFINED">
      <xsl:value-of select="$HLSMAX * 2 div 3"/>
    </xsl:variable>

    <xsl:variable name="r">
      <xsl:call-template name="HexToDec">
        <xsl:with-param name="number">
          <xsl:value-of select="substring($rgb,1,2)"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="g">
      <xsl:call-template name="HexToDec">
        <xsl:with-param name="number">
          <xsl:value-of select="substring($rgb,3,2)"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="b">
      <xsl:call-template name="HexToDec">
        <xsl:with-param name="number">
          <xsl:value-of select="substring($rgb,5,2)"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="max">
      <xsl:choose>
        <xsl:when test="$r &gt; $g">
          <xsl:choose>
            <xsl:when test="$r &gt; $b">
              <xsl:value-of select="$r"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$b"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:when>
        <xsl:otherwise>
          <xsl:choose>
            <xsl:when test="$g &gt; $b">
              <xsl:value-of select="$g"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$b"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="min">
      <xsl:choose>
        <xsl:when test="$r &lt; $g">
          <xsl:choose>
            <xsl:when test="$r &lt; $b">
              <xsl:value-of select="$r"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$b"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:when>
        <xsl:otherwise>
          <xsl:choose>
            <xsl:when test="$g &lt; $b">
              <xsl:value-of select="$g"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$b"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="l">
      <xsl:value-of select="floor(( (($max + $min) * $HLSMAX) + $RGBMAX ) div (2 * $RGBMAX))"/>
    </xsl:variable>

    <xsl:variable name="s">
      <xsl:choose>
        <xsl:when test="$max = $min">
          <xsl:text>0</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:choose>
            <xsl:when test="($l &lt; ($HLSMAX div 2)) or ($l = ($HLSMAX div 2))">
              <xsl:value-of
                select="floor(( (($max - $min) * $HLSMAX) + (($max + $min) div 2) ) div ($max + $min))"
              />
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of
                select="floor(( (($max - $min) * $HLSMAX) + ((2 * $RGBMAX - $max - $min) div 2) ) div (2 * $RGBMAX - $max - $min))"
              />
            </xsl:otherwise>
          </xsl:choose>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="Rdelta">
      <xsl:value-of
        select="( (($max - $r) * ($HLSMAX div 6)) + (($max - $min) div 2) ) div ($max - $min)"/>
    </xsl:variable>
    <xsl:variable name="Gdelta">
      <xsl:value-of
        select="( (($max - $g) * ($HLSMAX div 6)) + (($max - $min) div 2) ) div ($max - $min)"/>
    </xsl:variable>
    <xsl:variable name="Bdelta">
      <xsl:value-of
        select="( (($max - $b) * ($HLSMAX div 6)) + (($max - $min) div 2) ) div ($max - $min)"/>
    </xsl:variable>

    <xsl:variable name="h_part">
      <xsl:choose>
        <xsl:when test="$max = $min">
          <xsl:value-of select="$UNDEFINED"/>
        </xsl:when>
        <xsl:when test="$r = $max">
          <xsl:value-of select="$Bdelta - $Gdelta"/>
        </xsl:when>
        <xsl:when test="$g = $max">
          <xsl:value-of select="($HLSMAX div 3) + $Rdelta - $Bdelta"/>
        </xsl:when>
        <xsl:when test="$b = $max">
          <xsl:value-of select="((2 * $HLSMAX) div 3) + $Gdelta - $Rdelta"/>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="h">
      <xsl:choose>
        <xsl:when test="$h_part &lt; 0">
          <xsl:value-of select="floor($h_part + $HLSMAX)"/>
        </xsl:when>
        <xsl:when test="$h_part &gt; $HLSMAX">
          <xsl:value-of select="floor($h_part - $HLSMAX)"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="floor($h_part)"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:value-of select="concat($h,',',$l,',',$s)"/>
  </xsl:template>

  <!-- conversion based on http://support.microsoft.com/kb/29240 -->
  <xsl:template name="HueToRGB">
    <xsl:param name="n1"/>
    <xsl:param name="n2"/>
    <xsl:param name="hue"/>

    <xsl:variable name="HLSMAX" select="255"/>

    <xsl:variable name="h">
      <xsl:choose>
        <xsl:when test="$hue &lt; 0">
          <xsl:value-of select="$hue + $HLSMAX"/>
        </xsl:when>
        <xsl:when test="$hue &gt; $HLSMAX">
          <xsl:value-of select="$hue - $HLSMAX"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$hue"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="$h &lt; ($HLSMAX div 6)">
        <xsl:value-of select="$n1 + ((($n2 - $n1) * $h + ($HLSMAX div 12)) div ($HLSMAX div 6))"/>
      </xsl:when>
      <xsl:when test="$h &lt; ($HLSMAX div 2)">
        <xsl:value-of select="$n2"/>
      </xsl:when>
      <xsl:when test="$h &lt; (($HLSMAX * 2) div 3)">
        <xsl:value-of
          select="$n1 + ((($n2 - $n1) * ((($HLSMAX * 2) div 3) - $h) + ($HLSMAX div 12)) div ($HLSMAX div 6))"
        />
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$n1"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- conversion based on http://support.microsoft.com/kb/29240 -->
  <xsl:template name="HLStoRGB">
    <xsl:param name="hls"/>

    <xsl:variable name="HLSMAX" select="255"/>
    <xsl:variable name="RGBMAX" select="255"/>

    <xsl:variable name="hue">
      <xsl:value-of select="substring-before($hls,',')"/>
    </xsl:variable>
    <xsl:variable name="lum">
      <xsl:value-of select="substring-before(substring-after($hls,','),',')"/>
    </xsl:variable>
    <xsl:variable name="sat">
      <xsl:value-of select="substring-after(substring-after($hls,','),',')"/>
    </xsl:variable>

    <xsl:variable name="magic2">
      <xsl:choose>
        <xsl:when test="$lum &gt; $HLSMAX div 2">
          <xsl:value-of select="$lum + $sat - (($lum * $sat) + ($HLSMAX div 2)) div $HLSMAX"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="($lum * ($HLSMAX + $sat) + ($HLSMAX div 2)) div $HLSMAX"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="magic1">
      <xsl:value-of select="2 * $lum - $magic2"/>
    </xsl:variable>

    <xsl:variable name="red">
      <xsl:choose>
        <xsl:when test="$sat = 0">
          <xsl:value-of select="($lum * $RGBMAX) div $HLSMAX"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:variable name="hueR">
            <xsl:call-template name="HueToRGB">
              <xsl:with-param name="n1">
                <xsl:value-of select="$magic1"/>
              </xsl:with-param>
              <xsl:with-param name="n2">
                <xsl:value-of select="$magic2"/>
              </xsl:with-param>
              <xsl:with-param name="hue">
                <xsl:value-of select="$hue + ($HLSMAX div 3)"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:variable>
          <xsl:value-of select="($hueR * $RGBMAX + ($HLSMAX div 2)) div $HLSMAX"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="green">
      <xsl:choose>
        <xsl:when test="$sat = 0">
          <xsl:value-of select="($lum * $RGBMAX) div $HLSMAX"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:variable name="hueG">
            <xsl:call-template name="HueToRGB">
              <xsl:with-param name="n1">
                <xsl:value-of select="$magic1"/>
              </xsl:with-param>
              <xsl:with-param name="n2">
                <xsl:value-of select="$magic2"/>
              </xsl:with-param>
              <xsl:with-param name="hue">
                <xsl:value-of select="$hue"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:variable>
          <xsl:value-of select="($hueG * $RGBMAX + ($HLSMAX div 2)) div $HLSMAX"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="blue">
      <xsl:choose>
        <xsl:when test="$sat = 0">
          <xsl:value-of select="($lum * $RGBMAX) div $HLSMAX"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:variable name="hueB">
            <xsl:call-template name="HueToRGB">
              <xsl:with-param name="n1">
                <xsl:value-of select="$magic1"/>
              </xsl:with-param>
              <xsl:with-param name="n2">
                <xsl:value-of select="$magic2"/>
              </xsl:with-param>
              <xsl:with-param name="hue">
                <xsl:value-of select="$hue - ($HLSMAX div 3)"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:variable>
          <xsl:value-of select="($hueB * $RGBMAX + ($HLSMAX div 2)) div $HLSMAX"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:value-of select="concat(floor($red),',',floor($green),',',floor($blue))"/>
  </xsl:template>

  <xsl:template name="CalculateTintedColor">
    <xsl:param name="color"/>
    <xsl:param name="tint"/>

    <xsl:variable name="HLSMAX" select="255"/>

    <xsl:variable name="hls">
      <xsl:call-template name="RGBtoHLS">
        <xsl:with-param name="rgb" select="$color"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="hue">
      <xsl:value-of select="substring-before($hls,',')"/>
    </xsl:variable>
    <xsl:variable name="lum">
      <xsl:value-of select="substring-before(substring-after($hls,','),',')"/>
    </xsl:variable>
    <xsl:variable name="sat">
      <xsl:value-of select="substring-after(substring-after($hls,','),',')"/>
    </xsl:variable>

    <xsl:variable name="newLum">
      <xsl:choose>
        <xsl:when test="$tint &lt; 0">
          <xsl:value-of select="floor($lum * (1 + $tint))"/>
        </xsl:when>
        <xsl:when test="$tint > 0">
          <xsl:value-of select="floor($lum * (1 - $tint) + ($HLSMAX - $HLSMAX * (1 - $tint)))"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$lum"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="rgb">
      <xsl:call-template name="HLStoRGB">
        <xsl:with-param name="hls">
          <xsl:value-of select="concat($hue,',',$newLum,',',$sat)"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="red">
      <xsl:call-template name="DecToHex">
        <xsl:with-param name="number">
          <xsl:value-of select="substring-before($rgb,',')"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="green">
      <xsl:call-template name="DecToHex">
        <xsl:with-param name="number">
          <xsl:value-of select="substring-before(substring-after($rgb,','),',')"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="blue">
      <xsl:call-template name="DecToHex">
        <xsl:with-param name="number">
          <xsl:value-of select="substring-after(substring-after($rgb,','),',')"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:value-of select="concat($red,$green,$blue)"/>

  </xsl:template>

  <xsl:template name="ConvertScientific">
    <xsl:param name="value"/>

    <xsl:variable name="base">
      <xsl:value-of select="substring-before($value,'E')"/>
    </xsl:variable>
    <!-- after E- or E+ -->
    <xsl:variable name="exponent">
      <xsl:value-of select="substring(substring-after($value,'E'),2)"/>
    </xsl:variable>
    <xsl:variable name="sign">
      <xsl:value-of select="substring(substring-after($value,'E'),1,1)"/>
    </xsl:variable>

    <xsl:variable name="factor">
      <xsl:call-template name="Power">
        <xsl:with-param name="base" select="10"/>
        <xsl:with-param name="exponent" select="$exponent"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="$sign = '+'">
        <xsl:value-of select="$base * $factor"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$base div $factor"/>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="GetBuildInColor">
    <xsl:param name="index"/>
    <xsl:choose>
      <xsl:when test="$index=0">
        <xsl:text>000000</xsl:text>
      </xsl:when>
      <xsl:when test="$index=1">
        <xsl:text>FFFFFF</xsl:text>
      </xsl:when>
      <xsl:when test="$index=2">
        <xsl:text>FF0000</xsl:text>
      </xsl:when>
      <xsl:when test="$index=3">
        <xsl:text>00FF00</xsl:text>
      </xsl:when>
      <xsl:when test="$index=4">
        <xsl:text>0000FF</xsl:text>
      </xsl:when>
      <xsl:when test="$index=5">
        <xsl:text>FFFF00</xsl:text>
      </xsl:when>
      <xsl:when test="$index=6">
        <xsl:text>FF00FF</xsl:text>
      </xsl:when>
      <xsl:when test="$index=7">
        <xsl:text>00FFFF</xsl:text>
      </xsl:when>
      <xsl:when test="$index=8">
        <xsl:text>000000</xsl:text>
      </xsl:when>
      <xsl:when test="$index=9">
        <xsl:text>FFFFFF</xsl:text>
      </xsl:when>
      <xsl:when test="$index=10">
        <xsl:text>FF0000</xsl:text>
      </xsl:when>
      <xsl:when test="$index=11">
        <xsl:text>00FF00</xsl:text>
      </xsl:when>
      <xsl:when test="$index=12">
        <xsl:text>0000FF</xsl:text>
      </xsl:when>
      <xsl:when test="$index=13">
        <xsl:text>FFFF00</xsl:text>
      </xsl:when>
      <xsl:when test="$index=14">
        <xsl:text>FF00FF</xsl:text>
      </xsl:when>
      <xsl:when test="$index=15">
        <xsl:text>00FFFF</xsl:text>
      </xsl:when>
      <xsl:when test="$index=16">
        <xsl:text>800000</xsl:text>
      </xsl:when>
      <xsl:when test="$index=17">
        <xsl:text>008000</xsl:text>
      </xsl:when>
      <xsl:when test="$index=18">
        <xsl:text>000080</xsl:text>
      </xsl:when>
      <xsl:when test="$index=19">
        <xsl:text>808000</xsl:text>
      </xsl:when>
      <xsl:when test="$index=20">
        <xsl:text>800080</xsl:text>
      </xsl:when>
      <xsl:when test="$index=21">
        <xsl:text>008080</xsl:text>
      </xsl:when>
      <xsl:when test="$index=22">
        <xsl:text>C0C0C0</xsl:text>
      </xsl:when>
      <xsl:when test="$index=23">
        <xsl:text>808080</xsl:text>
      </xsl:when>
      <xsl:when test="$index=24">
        <!--<xsl:text>8080FF</xsl:text>-->
		<xsl:text>9999FF</xsl:text>
      </xsl:when>
      <xsl:when test="$index=25">
		<!--<xsl:text>802060</xsl:text>-->
		<xsl:text>993366</xsl:text>
      </xsl:when>
      <xsl:when test="$index=26">
		<!--<xsl:text>FFFFC0</xsl:text>-->
		<xsl:text>FFFFCC</xsl:text>
      </xsl:when>
      <xsl:when test="$index=27">
		<!--<xsl:text>A0E0E0</xsl:text>-->
		<xsl:text>CCFFFF</xsl:text>
      </xsl:when>
      <xsl:when test="$index=28">
        <!--<xsl:text>600080</xsl:text>-->
		<xsl:text>660066</xsl:text>
      </xsl:when>
      <xsl:when test="$index=29">
        <xsl:text>FF8080</xsl:text>
      </xsl:when>
      <xsl:when test="$index=30">
        <!--<xsl:text>0080C0</xsl:text>-->
		<xsl:text>0066CC</xsl:text>
      </xsl:when>
      <xsl:when test="$index=31">
		<!--<xsl:text>C0C0FF</xsl:text>-->
		<xsl:text>CCCCFF</xsl:text>
      </xsl:when>
      <xsl:when test="$index=32">
        <xsl:text>000080</xsl:text>
      </xsl:when>
      <xsl:when test="$index=33">
        <xsl:text>FF00FF</xsl:text>
      </xsl:when>
      <xsl:when test="$index=34">
        <xsl:text>FFFF00</xsl:text>
      </xsl:when>
      <xsl:when test="$index=35">
        <xsl:text>00FFFF</xsl:text>
      </xsl:when>
      <xsl:when test="$index=36">
        <xsl:text>800080</xsl:text>
      </xsl:when>
      <xsl:when test="$index=37">
        <xsl:text>800000</xsl:text>
      </xsl:when>
      <xsl:when test="$index=38">
        <xsl:text>008080</xsl:text>
      </xsl:when>
      <xsl:when test="$index=39">
        <xsl:text>0000FF</xsl:text>
      </xsl:when>
      <xsl:when test="$index=40">
        <xsl:text>00CCFF</xsl:text>
      </xsl:when>
      <xsl:when test="$index=41">
        <xsl:text>CCFFFF</xsl:text>
      </xsl:when>
      <xsl:when test="$index=42">
        <xsl:text>CCFFCC</xsl:text>
      </xsl:when>
      <xsl:when test="$index=43">
        <xsl:text>FFFF99</xsl:text>
      </xsl:when>
      <xsl:when test="$index=44">
		<!--<xsl:text>A6CAF0</xsl:text>-->
		<xsl:text>99CCFF</xsl:text>
      </xsl:when>
      <xsl:when test="$index=45">
        <!--<xsl:text>CC9CCC</xsl:text>-->
		<xsl:text>FF99CC</xsl:text>
      </xsl:when>
      <xsl:when test="$index=46">
        <xsl:text>CC99FF</xsl:text>
      </xsl:when>
      <xsl:when test="$index=47">
        <xsl:text>FFCC99</xsl:text>
      </xsl:when>
      <xsl:when test="$index=48">
        <xsl:text>3366FF</xsl:text>
      </xsl:when>
      <xsl:when test="$index=49">
        <xsl:text>33CCCC</xsl:text>
      </xsl:when>
      <xsl:when test="$index=50">
		<!--<xsl:text>339933</xsl:text>-->
		<xsl:text>99CC00</xsl:text>
      </xsl:when>
      <xsl:when test="$index=51">
		<!--<xsl:text>FFD700</xsl:text>-->
		<xsl:text>FFCC00</xsl:text>
      </xsl:when>
      <xsl:when test="$index=52">
		<!--<xsl:text>FFA500</xsl:text>-->
		<xsl:text>FF9900</xsl:text>
      </xsl:when>
      <xsl:when test="$index=53">
		<!--<xsl:text>996666</xsl:text>-->
		<xsl:text>FF6600</xsl:text>
      </xsl:when>
      <xsl:when test="$index=54">
        <xsl:text>666699</xsl:text>
      </xsl:when>
      <xsl:when test="$index=55">
        <xsl:text>969696</xsl:text>
      </xsl:when>
      <xsl:when test="$index=56">
        <!--<xsl:text>3333CC</xsl:text>-->
		<xsl:text>003366</xsl:text>
      </xsl:when>
      <xsl:when test="$index=57">
        <!--<xsl:text>336666</xsl:text>-->
		<xsl:text>339966</xsl:text>
      </xsl:when>
      <xsl:when test="$index=58">
        <xsl:text>003300</xsl:text>
      </xsl:when>
      <xsl:when test="$index=59">
        <xsl:text>333300</xsl:text>
      </xsl:when>
      <xsl:when test="$index=60">
		<xsl:text>993300</xsl:text>
		<!--<xsl:text>663300</xsl:text>-->
      </xsl:when>
      <xsl:when test="$index=61">
        <xsl:text>993366</xsl:text>
      </xsl:when>
      <xsl:when test="$index=62">
        <xsl:text>333399</xsl:text>
      </xsl:when>
      <xsl:when test="$index=63">
        <!--<xsl:text>424242</xsl:text>-->
		<xsl:text>333333</xsl:text>
      </xsl:when>
      <xsl:when test="$index=64">
        <xsl:text>000000</xsl:text>
      </xsl:when>
      <xsl:when test="$index=65">
        <xsl:text>FFFFFF</xsl:text>
      </xsl:when>
      <xsl:otherwise>000000</xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="ConvertFromCharacters">
    <xsl:param name="value"/>

    <!-- strange but true: the best result is when you WON'T convert average digit width from pt to px-->
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

    <xsl:variable name="fontSize">
      <xsl:call-template name="ConvertPoints">
        <xsl:with-param name="length">
          <xsl:value-of select="$defaultFontSize"/>
        </xsl:with-param>
        <xsl:with-param name="unit">px</xsl:with-param>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="realFontSize">
      <xsl:value-of select="(round(number(substring-before($fontSize,'px'))) div 96) * 72"/>
    </xsl:variable>
    <!-- for proportional fonts average digit width is 2/3 of font size-->
    <xsl:variable name="avgDigitWidth">
      <xsl:value-of select="$realFontSize * 2 div 3"/>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="$avgDigitWidth * $value = 0">
        <xsl:text>0cm</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="ConvertToCentimeters">
          <xsl:with-param name="length">
            <xsl:value-of select="concat(($avgDigitWidth * $value),'px')"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="InsertColor">
    <xsl:variable name="this" select="."/>
    <xsl:variable name="colorDefinition" select="./@fillcolor"/>

    <xsl:choose>
      <xsl:when test="@rgb">
        <xsl:value-of select="concat('#',substring(@rgb,3,9))"/>
      </xsl:when>
      <xsl:when test="@indexed">
        <xsl:choose>
          <xsl:when
            test="key('Part', 'xl/styles.xml')/e:styleSheet/e:colors/e:indexedColors/e:rgbColor[$this/@indexed + 1]">
            <xsl:variable name="color">
              <xsl:value-of
                select="key('Part', 'xl/styles.xml')/e:styleSheet/e:colors/e:indexedColors/e:rgbColor[$this/@indexed + 1]/@rgb"
              />
            </xsl:variable>
            <xsl:value-of select="concat('#',substring($color,3,9))"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:variable name="color">
              <xsl:call-template name="GetBuildInColor">
                <xsl:with-param name="index">
                  <xsl:value-of select="@indexed"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:variable>
            <xsl:value-of select="concat('#',$color)"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:when test="@theme">
        <xsl:variable name="theme">
          <xsl:choose>
            <xsl:when test="@theme = 0">
              <xsl:text>1</xsl:text>
            </xsl:when>
            <xsl:when test="@theme = 1">
              <xsl:text>0</xsl:text>
            </xsl:when>
            <xsl:when test="@theme = 2">
              <xsl:text>3</xsl:text>
            </xsl:when>
            <xsl:when test="@theme = 3">
              <xsl:text>2</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="@theme"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>

        <!-- color from theme -->
        <xsl:variable name="themeColor">
          <xsl:choose>
            <!-- if color is in 'sysClr' node -->
            <xsl:when
              test="key('Part', 'xl/theme/theme1.xml')/a:theme/a:themeElements/a:clrScheme/child::node()[position() = $theme + 1]/child::node()/@lastClr">
              <xsl:value-of
                select="key('Part', 'xl/theme/theme1.xml')/a:theme/a:themeElements/a:clrScheme/child::node()[position() = $theme + 1]/child::node()/@lastClr"
              />
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of
                select="key('Part', 'xl/theme/theme1.xml')/a:theme/a:themeElements/a:clrScheme/child::node()[position() = $theme + 1]/child::node()/@val"
              />
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>

        <xsl:variable name="color">
          <xsl:choose>
            <!-- if tinted color -->
            <xsl:when test="@tint">
              <xsl:call-template name="CalculateTintedColor">
                <xsl:with-param name="color" select="$themeColor"/>
                <xsl:with-param name="tint">
                  <xsl:choose>
                    <xsl:when test="contains(@tint,'E')">
                      <xsl:call-template name="ConvertScientific">
                        <xsl:with-param name="value">
                          <xsl:value-of select="@tint"/>
                        </xsl:with-param>
                      </xsl:call-template>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:value-of select="@tint"/>
                    </xsl:otherwise>
                  </xsl:choose>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$themeColor"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>

        <xsl:value-of select="concat('#',$color)"/>
      </xsl:when>
      <xsl:when test="@fillcolor">

        <xsl:choose>
          <!-- named color, not hexadecimal value -->
          <!-- must be translated to hexadecimal value -->
          <xsl:when test="not(starts-with($colorDefinition, '#'))">
            <xsl:choose>
              <xsl:when test="starts-with($colorDefinition, 'black')">
                <xsl:value-of select="'#000000'"/>
              </xsl:when>
              <xsl:when test="starts-with($colorDefinition, 'silver')">
                <xsl:value-of select="'#C0C0C0'"/>
              </xsl:when>
              <xsl:when test="starts-with($colorDefinition, 'gray')">
                <xsl:value-of select="'#808080'"/>
              </xsl:when>
              <xsl:when test="starts-with($colorDefinition, 'white')">
                <xsl:value-of select="'#FFFFFF'"/>
              </xsl:when>
              <xsl:when test="starts-with($colorDefinition, 'maroon')">
                <xsl:value-of select="'#800000'"/>
              </xsl:when>
              <xsl:when test="starts-with($colorDefinition, 'red')">
                <xsl:value-of select="'#FF0000'"/>
              </xsl:when>
              <xsl:when test="starts-with($colorDefinition, 'purple')">
                <xsl:value-of select="'#800080'"/>
              </xsl:when>
              <xsl:when test="starts-with($colorDefinition, 'fuchsia')">
                <xsl:value-of select="'#FF00FF'"/>
              </xsl:when>
              <xsl:when test="starts-with($colorDefinition, 'green')">
                <xsl:value-of select="'#008000'"/>
              </xsl:when>
              <xsl:when test="starts-with($colorDefinition, 'lime')">
                <xsl:value-of select="'#00FF00'"/>
              </xsl:when>
              <xsl:when test="starts-with($colorDefinition, 'olive')">
                <xsl:value-of select="'#808000'"/>
              </xsl:when>
              <xsl:when test="starts-with($colorDefinition, 'yellow')">
                <xsl:value-of select="'#FFFF00'"/>
              </xsl:when>
              <xsl:when test="starts-with($colorDefinition, 'navy')">
                <xsl:value-of select="'#000080'"/>
              </xsl:when>
              <xsl:when test="starts-with($colorDefinition, 'blue')">
                <xsl:value-of select="'#0000FF'"/>
              </xsl:when>
              <xsl:when test="starts-with($colorDefinition, 'teal')">
                <xsl:value-of select="'#008080'"/>
              </xsl:when>
              <xsl:when test="starts-with($colorDefinition, 'aqua')">
                <xsl:value-of select="'#00FFFF'"/>
              </xsl:when>
            </xsl:choose>
          </xsl:when>
          <!-- color given in a shortened hexadecimal value, e.g. #9cf instead of #99ccff -->
          <xsl:when test="string-length(substring-before($colorDefinition, ' ')) = 4">
            <xsl:variable name="r" select="substring($colorDefinition, 2, 1)"/>
            <xsl:variable name="g" select="substring($colorDefinition, 3, 1)"/>
            <xsl:variable name="b" select="substring($colorDefinition, 4, 1)"/>
            <xsl:text>#</xsl:text>
            <xsl:value-of select="concat($r, $r, $g, $g, $b, $b)"/>
          </xsl:when>
          <!-- color given in a proper hexadecimal value -->
          <xsl:otherwise>
            <xsl:value-of select="substring($colorDefinition, 1, 7)"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>

    </xsl:choose>

  </xsl:template>
  
  <xsl:template name="InsertStrokeColor">
    <xsl:param name="insideVStrokeNode"/>
    <xsl:variable name="colorDefinition">
  
    <xsl:choose>
      <xsl:when test="$insideVStrokeNode = 1">
        <xsl:value-of select="@color"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="@strokecolor"/>
      </xsl:otherwise>
    </xsl:choose>
    </xsl:variable>   
    
    <xsl:choose>
      <!-- named color, not hexadecimal value -->
      <!-- must be translated to hexadecimal value -->
      <xsl:when test="not(starts-with($colorDefinition, '#'))">
        <xsl:choose>
          <xsl:when test="starts-with($colorDefinition, 'black')">
            <xsl:value-of select="'#000000'"/>
          </xsl:when>
          <xsl:when test="starts-with($colorDefinition, 'silver')">
            <xsl:value-of select="'#C0C0C0'"/>
          </xsl:when>
          <xsl:when test="starts-with($colorDefinition, 'gray')">
            <xsl:value-of select="'#808080'"/>
          </xsl:when>
          <xsl:when test="starts-with($colorDefinition, 'white')">
            <xsl:value-of select="'#FFFFFF'"/>
          </xsl:when>
          <xsl:when test="starts-with($colorDefinition, 'maroon')">
            <xsl:value-of select="'#800000'"/>
          </xsl:when>
          <xsl:when test="starts-with($colorDefinition, 'red')">
            <xsl:value-of select="'#FF0000'"/>
          </xsl:when>
          <xsl:when test="starts-with($colorDefinition, 'purple')">
            <xsl:value-of select="'#800080'"/>
          </xsl:when>
          <xsl:when test="starts-with($colorDefinition, 'fuchsia')">
            <xsl:value-of select="'#FF00FF'"/>
          </xsl:when>
          <xsl:when test="starts-with($colorDefinition, 'green')">
            <xsl:value-of select="'#008000'"/>
          </xsl:when>
          <xsl:when test="starts-with($colorDefinition, 'lime')">
            <xsl:value-of select="'#00FF00'"/>
          </xsl:when>
          <xsl:when test="starts-with($colorDefinition, 'olive')">
            <xsl:value-of select="'#808000'"/>
          </xsl:when>
          <xsl:when test="starts-with($colorDefinition, 'yellow')">
            <xsl:value-of select="'#FFFF00'"/>
          </xsl:when>
          <xsl:when test="starts-with($colorDefinition, 'navy')">
            <xsl:value-of select="'#000080'"/>
          </xsl:when>
          <xsl:when test="starts-with($colorDefinition, 'blue')">
            <xsl:value-of select="'#0000FF'"/>
          </xsl:when>
          <xsl:when test="starts-with($colorDefinition, 'teal')">
            <xsl:value-of select="'#008080'"/>
          </xsl:when>
          <xsl:when test="starts-with($colorDefinition, 'aqua')">
            <xsl:value-of select="'#00FFFF'"/>
          </xsl:when>
        </xsl:choose>
      </xsl:when>
      <!-- color given in a shortened hexadecimal value, e.g. #9cf instead of #99ccff -->
      <xsl:when test="string-length(substring-before($colorDefinition, ' ')) = 4">
        <xsl:variable name="r" select="substring($colorDefinition, 2, 1)"/>
        <xsl:variable name="g" select="substring($colorDefinition, 3, 1)"/>
        <xsl:variable name="b" select="substring($colorDefinition, 4, 1)"/>
        <xsl:text>#</xsl:text>
        <xsl:value-of select="concat($r, $r, $g, $g, $b, $b)"/>
      </xsl:when>
      <!-- color given in a proper hexadecimal value -->
      <xsl:otherwise>
        <xsl:value-of select="substring($colorDefinition, 1, 7)"/>
      </xsl:otherwise>
    </xsl:choose>
    
  </xsl:template>

  <xsl:template name="InsertPaperSize">
    <xsl:param name="paperSize"/>
    <xsl:param name="orientation"/>

    <xsl:choose>
      <xsl:when test="$orientation='landscape' ">
        <xsl:choose>
          <!-- A3 -->
          <xsl:when test="$paperSize=8">
            <xsl:attribute name="fo:page-width">
              <xsl:text>42cm</xsl:text>
            </xsl:attribute>
            <xsl:attribute name="fo:page-height">
              <xsl:text>29.699cm</xsl:text>
            </xsl:attribute>
          </xsl:when>
          <!-- A4 -->
          <xsl:when test="$paperSize=9">
            <xsl:attribute name="fo:page-width">
              <xsl:text>29.699cm</xsl:text>
            </xsl:attribute>
            <xsl:attribute name="fo:page-height">
              <xsl:text>20.999cm</xsl:text>
            </xsl:attribute>
          </xsl:when>
          <!-- A5 -->
          <xsl:when test="$paperSize=11">
            <xsl:attribute name="fo:page-width">
              <xsl:text>20.999cm</xsl:text>
            </xsl:attribute>
            <xsl:attribute name="fo:page-height">
              <xsl:text>14.799cm</xsl:text>
            </xsl:attribute>
          </xsl:when>
          <!-- B4 (JIS) -->
          <xsl:when test="$paperSize=12">
            <xsl:attribute name="fo:page-width">
              <xsl:text>36.4cm</xsl:text>
            </xsl:attribute>
            <xsl:attribute name="fo:page-height">
              <xsl:text>25.7cm</xsl:text>
            </xsl:attribute>
          </xsl:when>
          <!-- B5 (JIS) -->
          <xsl:when test="$paperSize=13">
            <xsl:attribute name="fo:page-width">
              <xsl:text>25.7cm</xsl:text>
            </xsl:attribute>
            <xsl:attribute name="fo:page-height">
              <xsl:text>18.2cm</xsl:text>
            </xsl:attribute>
          </xsl:when>
          <!-- Letter -->
          <xsl:when test="$paperSize=1">
            <xsl:attribute name="fo:page-width">
              <xsl:text>27.94cm</xsl:text>
            </xsl:attribute>
            <xsl:attribute name="fo:page-height">
              <xsl:text>21.59cm</xsl:text>
            </xsl:attribute>
          </xsl:when>
          <!-- Tabloid -->
          <xsl:when test="$paperSize=3">
            <xsl:attribute name="fo:page-width">
              <xsl:text>43.127cm</xsl:text>
            </xsl:attribute>
            <xsl:attribute name="fo:page-height">
              <xsl:text>27.958cm</xsl:text>
            </xsl:attribute>
          </xsl:when>
          <!-- Legal -->
          <xsl:when test="$paperSize=5">
            <xsl:attribute name="fo:page-width">
              <xsl:text>35.565cm</xsl:text>
            </xsl:attribute>
            <xsl:attribute name="fo:page-height">
              <xsl:text>21.59cm</xsl:text>
            </xsl:attribute>
          </xsl:when>
          <xsl:when test="$paperSize=43">
            <xsl:attribute name="fo:page-width">
              <xsl:text>14.8cm</xsl:text>
            </xsl:attribute>
            <xsl:attribute name="fo:page-height">
              <xsl:text>10cm</xsl:text>
            </xsl:attribute>
          </xsl:when>
          <!-- A4 as default -->
          <xsl:otherwise>
            <xsl:attribute name="fo:page-width">
              <xsl:text>29.699cm</xsl:text>
            </xsl:attribute>
            <xsl:attribute name="fo:page-height">
              <xsl:text>20.999cm</xsl:text>
            </xsl:attribute>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <!-- when orientation is 'portrait' -->
      <xsl:otherwise>
        <xsl:choose>
          <!-- A3 -->
          <xsl:when test="$paperSize=8">
            <xsl:attribute name="fo:page-width">
              <xsl:text>29.699cm</xsl:text>
            </xsl:attribute>
            <xsl:attribute name="fo:page-height">
              <xsl:text>42cm</xsl:text>
            </xsl:attribute>
          </xsl:when>
          <!-- A4 -->
          <xsl:when test="$paperSize=9">
            <xsl:attribute name="fo:page-width">
              <xsl:text>20.999cm</xsl:text>
            </xsl:attribute>
            <xsl:attribute name="fo:page-height">
              <xsl:text>29.699cm</xsl:text>
            </xsl:attribute>
          </xsl:when>
          <!-- A5 -->
          <xsl:when test="$paperSize=11">
            <xsl:attribute name="fo:page-width">
              <xsl:text>14.799cm</xsl:text>
            </xsl:attribute>
            <xsl:attribute name="fo:page-height">
              <xsl:text>20.999cm</xsl:text>
            </xsl:attribute>
          </xsl:when>
          <!-- B4 (JIS) -->
          <xsl:when test="$paperSize=12">
            <xsl:attribute name="fo:page-width">
              <xsl:text>25.7cm</xsl:text>
            </xsl:attribute>
            <xsl:attribute name="fo:page-height">
              <xsl:text>36.4cm</xsl:text>
            </xsl:attribute>
          </xsl:when>
          <!-- B5 (JIS) -->
          <xsl:when test="$paperSize=13">
            <xsl:attribute name="fo:page-width">
              <xsl:text>18.2cm</xsl:text>
            </xsl:attribute>
            <xsl:attribute name="fo:page-height">
              <xsl:text>25.7cm</xsl:text>
            </xsl:attribute>
          </xsl:when>
          <!-- Letter -->
          <xsl:when test="$paperSize=1">
            <xsl:attribute name="fo:page-width">
              <xsl:text>21.59cm</xsl:text>
            </xsl:attribute>
            <xsl:attribute name="fo:page-height">
              <xsl:text>27.94cm</xsl:text>
            </xsl:attribute>
          </xsl:when>
          <!-- Tabloid -->
          <xsl:when test="$paperSize=3">
            <xsl:attribute name="fo:page-width">
              <xsl:text>27.958cm</xsl:text>
            </xsl:attribute>
            <xsl:attribute name="fo:page-height">
              <xsl:text>43.127cm</xsl:text>
            </xsl:attribute>
          </xsl:when>
          <!-- Legal -->
          <xsl:when test="$paperSize=5">
            <xsl:attribute name="fo:page-width">
              <xsl:text>21.59cm</xsl:text>
            </xsl:attribute>
            <xsl:attribute name="fo:page-height">
              <xsl:text>35.565cm</xsl:text>
            </xsl:attribute>
          </xsl:when>
          <xsl:when test="$paperSize=43">
            <xsl:attribute name="fo:page-width">
              <xsl:text>10cm</xsl:text>
            </xsl:attribute>
            <xsl:attribute name="fo:page-height">
              <xsl:text>14.8cm</xsl:text>
            </xsl:attribute>
          </xsl:when>
          <!-- A4 as default -->
          <xsl:otherwise>
            <xsl:attribute name="fo:page-width">
              <xsl:text>20.999cm</xsl:text>
            </xsl:attribute>
            <xsl:attribute name="fo:page-height">
              <xsl:text>29.699cm</xsl:text>
            </xsl:attribute>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="NumberToDate">

    <!-- @Descripition: converts number value to date value -->
    <!-- @Context: None -->

    <xsl:param name="value"/>
    <!-- (float) number value -->
    <xsl:variable name="time">
      <xsl:value-of select="concat('0.',substring-after($value,'.'))"/>
    </xsl:variable>
    <xsl:variable name="timeInHours">
      <xsl:value-of select="24 * $time"/>
    </xsl:variable>
    <xsl:variable name="hours">
      <xsl:value-of select="floor($timeInHours)"/>
    </xsl:variable>
    <xsl:variable name="timeInMins">
      <xsl:value-of select="($timeInHours - $hours) * 60"/>
    </xsl:variable>
    <xsl:variable name="mins">
      <xsl:value-of select="floor($timeInMins)"/>
    </xsl:variable>

    <!-- seconds -->
    <xsl:variable name="seconds">
      <xsl:value-of select="($timeInMins - $mins) *  60"/>
    </xsl:variable>
    <xsl:variable name="secondsBeforeComma">
      <xsl:choose>
        <xsl:when test="contains($seconds,'.')">
          <xsl:value-of select="format-number(substring-before($seconds,'.'),'00')"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="format-number($seconds,'00')"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="secondsAfterComma">
      <xsl:choose>
        <xsl:when test="contains($seconds,'.')">
          <xsl:value-of select="concat('.',substring-after($seconds,'.'))"/>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="date">
      <xsl:choose>
        <xsl:when test="contains($value,'.')">
          <xsl:value-of select="number(substring-before($value,'.'))"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$value"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="YearAndDay">
      <xsl:call-template name="CalculateYearAndDay">
        <xsl:with-param name="days">
          <xsl:choose>

            <!-- modification due to Excel Date Bug(leap year 1900) -->
            <xsl:when test="$date &gt; 59">
              <xsl:value-of select="$date - 1"/>
            </xsl:when>

            <xsl:otherwise>
              <xsl:value-of select="$date"/>
            </xsl:otherwise>
          </xsl:choose>
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
      select="concat(concat(concat(concat(concat(concat(concat(concat((1900+$years),'-'),$monthsAndDays),'T'),format-number($hours,'00')),':'),format-number($mins,'00')),':'),concat($secondsBeforeComma,$secondsAfterComma))"
    />
  </xsl:template>

  <xsl:template name="NumberToTime">

    <!-- @Descripition: converts number value to time value -->
    <!-- @Context: None -->
    <xsl:param name="value"/>
	<xsl:param name="isIsoDate" select ="'false'"/>

    <xsl:choose>
      <xsl:when test="contains($value, 'E')">
        <xsl:variable name="Scient2time">
          <xsl:call-template name="ConvertScientific">
            <xsl:with-param name="value">
              <xsl:value-of select="."/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:variable>
        <xsl:variable name="hours">
			
          <xsl:choose>
            <xsl:when test="number($Scient2time)">
              <xsl:value-of select="floor($Scient2time * 24)"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>0</xsl:text>
            </xsl:otherwise>
					
          </xsl:choose>
        </xsl:variable>
        <!--added for ODF 1.1-->
          <xsl:variable name="newHours">
			  <xsl:if test ="$isIsoDate='false'">
            <xsl:if test="contains($hours,'-')">
            <xsl:value-of select="substring-after($hours,'-')"/>
            </xsl:if>
			  </xsl:if>
          </xsl:variable>
        <!--end-->
        <xsl:variable name="minutes">
			
          <xsl:choose>
            <xsl:when test="number($Scient2time)">
              <xsl:value-of select="floor(($Scient2time * 24 - $hours) * 60)"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>0</xsl:text>
            </xsl:otherwise>
          </xsl:choose>    

		</xsl:variable>
        <xsl:variable name="seconds">
			
          <xsl:choose>
            <xsl:when test="number($Scient2time)">
              <xsl:value-of select="$Scient2time * 86400 - $minutes * 60 - $hours * 3600"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>0</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
				       
        </xsl:variable>
        <xsl:variable name="newSeconds">
			<xsl:if test ="$isIsoDate='false'">
          <xsl:if test="contains($seconds,'-')">
            <xsl:value-of select="substring-after($seconds,'-')"/>
          </xsl:if>       
        </xsl:if>
        </xsl:variable>        
        <xsl:choose>
          <xsl:when test="not(contains($hours,'-'))">
            <xsl:choose>
              <xsl:when test="not(contains($seconds,'-'))">
        <xsl:value-of select="concat('PT',$hours,'H',$minutes,'M',$seconds,'S')"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="concat('-','PT',$hours,'H',$minutes,'M',$newSeconds,'S')"/>
              </xsl:otherwise>
            </xsl:choose>
            </xsl:when>
          <xsl:otherwise>
            <xsl:choose>
            <xsl:when test="not(contains($seconds,'-'))">
              <xsl:value-of select="concat('-','PT',$newHours,'H',$minutes,'M',$seconds,'S')"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="concat('-','PT',$newHours,'H',$minutes,'M',$newSeconds,'S')"/>
            </xsl:otherwise>
            </xsl:choose>
          </xsl:otherwise>
        </xsl:choose>        
        <!--end-->
      </xsl:when>
      <xsl:otherwise>
        <xsl:variable name="hours">
          <xsl:choose>
				<xsl:when test ="$isIsoDate='true' and contains($value,':') ">
					<xsl:choose >
						<xsl:when test ="contains($value,'T')
								  and not(contains($value,'P')) 
								  and not(contains($value,'H'))
								  and not(contains($value,'M'))
								  and not(contains($value,'S'))">
							<xsl:variable name ="time">
								<xsl:value-of select ="substring-after($value,'T')"/>
							</xsl:variable>							
							<xsl:value-of select ="substring-before($time,':')"/>
						</xsl:when>
						<xsl:when test ="not(contains($value,'T'))">
					<xsl:value-of select ="substring-before($value,':')"/>
				</xsl:when>
					</xsl:choose>					
				</xsl:when>
				<xsl:when test ="$isIsoDate='true' and not(contains($value,':')) 
						  and contains($value,'P') 
						  and contains($value,'H') 
						  and contains($value,'M')
						  and contains($value,'S')">
					<xsl:value-of select ="substring-before(substring-after($value,'PT'),'H')"/>
				</xsl:when>
				<xsl:otherwise>
					  <xsl:choose>
            <xsl:when test="number($value)">
              <xsl:value-of select="floor($value * 24)"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>0</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
				</xsl:otherwise>
				</xsl:choose>		
        </xsl:variable>
        <xsl:variable name="minutes">
          <xsl:choose>
				<xsl:when test ="$isIsoDate='true' and contains($value,':') ">
					<xsl:choose >
						<xsl:when test ="contains($value,'T')
								  and not(contains($value,'P')) 
								  and not(contains($value,'H'))
								  and not(contains($value,'M'))
								  and not(contains($value,'S'))">
							<xsl:variable name ="time">
								<xsl:value-of select ="substring-after($value,'T')"/>
							</xsl:variable>
							<xsl:value-of select ="substring-before(substring-after($time,':'),':')"/>
						</xsl:when>
						<xsl:when test ="not(contains($value,'T'))">
					<xsl:value-of select ="substring-before(substring-after($value,':'),':')"/>
				</xsl:when>
					</xsl:choose>					
				</xsl:when>
				<xsl:when test ="$isIsoDate='true' and not(contains($value,':')) 
						  and contains($value,'P') 
						  and contains($value,'H') 
						  and contains($value,'M')
						  and contains($value,'S')">
					<xsl:value-of select ="substring-before(substring-after($value,'H'),'M')"/>
				</xsl:when>
				<xsl:otherwise>
			
          <xsl:choose>
            <xsl:when test="number($value)">
              <xsl:value-of select="floor(($value * 24 - $hours) * 60)"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>0</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
				</xsl:otherwise>
			</xsl:choose>
        </xsl:variable>

        <xsl:variable name="seconds">
          <xsl:choose>
				<xsl:when test ="$isIsoDate='true' and contains($value,':') ">
					<xsl:choose >
						<xsl:when test ="contains($value,'T')
								  and not(contains($value,'P')) 
								  and not(contains($value,'H'))
								  and not(contains($value,'M'))
								  and not(contains($value,'S'))">
							<xsl:variable name ="time">
								<xsl:value-of select ="substring-after($value,'T')"/>
							</xsl:variable>
							<xsl:value-of select ="substring-after(substring-after($time,':'),':')"/>
						</xsl:when>
						<xsl:when test ="not(contains($value,'T'))">
					<xsl:value-of select ="substring-after(substring-after($value,':'),':')"/>
				</xsl:when>
					</xsl:choose>				
				</xsl:when>
				<xsl:when test ="$isIsoDate='true' and not(contains($value,':')) 
						  and contains($value,'P') 
						  and contains($value,'H') 
						  and contains($value,'M')
						  and contains($value,'S')">
					<xsl:value-of select ="substring-before(substring-after($value,'M'),'S')"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:choose>
            <xsl:when test="number($value)">
              <xsl:value-of select="$value * 86400 - $minutes * 60 - $hours * 3600"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>0</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
				</xsl:otherwise>
			</xsl:choose>
        </xsl:variable>
     <!--added by sonata for ODF 1.1-->
        <xsl:variable name="newSeconds">
          <xsl:if test="contains($seconds,'-')">
            <xsl:value-of select="substring-after($seconds,'-')"/>
          </xsl:if>
        </xsl:variable>
        <!--end-->

        <!--if condition added for ODF 1.1-->
        <xsl:if test="not(contains($seconds,'-'))">
        <xsl:value-of select="concat('PT',$hours,'H',$minutes,'M',$seconds,'S')"/>
        </xsl:if>

        <xsl:if test="contains($seconds,'-')">
          <xsl:value-of select="concat('-','PT',$hours,'H',$minutes,'M',$newSeconds,'S')"/>
        </xsl:if>
        <!--commented for ODF 1.1-->
        <!--<xsl:value-of select="concat('PT',$hours,'H',$minutes,'M',$seconds,'S')"/>-->
        <!--end-->
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="DaysToMonthsAndDays">

    <!-- @Descripition: converts day of a year to month and day of the year -->
    <!-- @Context: None -->

    <xsl:param name="days"/>
    <!-- (int) day of year -->
    <xsl:param name="ExtraDay"/>
    <!-- (int) extra day if it's a leap year -->
    <xsl:choose>
      <xsl:when test="$days &lt; 32">
        <xsl:value-of select="concat('01-',format-number($days,'00'))"/>
      </xsl:when>
      <xsl:when test="$days - $ExtraDay &lt; 60">
        <xsl:value-of select="concat('02-',format-number($days - 31,'00'))"/>
      </xsl:when>
      <xsl:when test="$days - $ExtraDay &lt; 91">
        <xsl:value-of select="concat('03-',format-number($days - $ExtraDay - 59,'00'))"/>
      </xsl:when>
      <xsl:when test="$days - $ExtraDay &lt; 121">
        <xsl:value-of select="concat('04-',format-number($days - $ExtraDay - 90,'00'))"/>
      </xsl:when>
      <xsl:when test="$days - $ExtraDay &lt; 152">
        <xsl:value-of select="concat('05-',format-number($days - $ExtraDay - 120,'00'))"/>
      </xsl:when>
      <xsl:when test="$days - $ExtraDay &lt; 182">
        <xsl:value-of select="concat('06-',format-number($days - $ExtraDay - 151,'00'))"/>
      </xsl:when>
      <xsl:when test="$days - $ExtraDay &lt; 213">
        <xsl:value-of select="concat('07-',format-number($days - $ExtraDay - 181,'00'))"/>
      </xsl:when>
      <xsl:when test="$days - $ExtraDay &lt; 244">
        <xsl:value-of select="concat('08-',format-number($days - $ExtraDay - 212,'00'))"/>
      </xsl:when>
      <xsl:when test="$days - $ExtraDay &lt; 274">
        <xsl:value-of select="concat('09-',format-number($days - $ExtraDay - 243,'00'))"/>
      </xsl:when>
      <xsl:when test="$days - $ExtraDay &lt; 305">
        <xsl:value-of select="concat('10-',format-number($days - $ExtraDay - 273,'00'))"/>
      </xsl:when>
      <xsl:when test="$days - $ExtraDay &lt; 335">
        <xsl:value-of select="concat('11-',format-number($days - $ExtraDay - 304,'00'))"/>
      </xsl:when>
      <xsl:when test="$days - $ExtraDay &lt; 366">
        <xsl:value-of select="concat('12-',format-number($days - $ExtraDay - 334,'00'))"/>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- calcualte year and number of days left from number of days -->

  <xsl:template name="CalculateYearAndDay">

    <!-- @Descripition: calcualte year and number of days left from number of days after 1.01.1900 -->
    <!-- @Context: None -->

    <xsl:param name="days"/>
    <!-- (int) number of days -->
    <xsl:param name="currentYear"/>
    <!-- (int) current year -->
    <xsl:choose>
      <xsl:when
        test="(($currentYear - 4 * floor(($currentYear) div 4)) = 0 and ($currentYear - 100 * floor(($currentYear) div 100) != 0)) or ($currentYear - 400 * floor(($currentYear) div 400)) = 0">
        <xsl:choose>
          <xsl:when test="$days &gt; 366">
            <xsl:call-template name="CalculateYearAndDay">
              <xsl:with-param name="days">
                <xsl:value-of select="$days -366"/>
              </xsl:with-param>
              <xsl:with-param name="currentYear">
                <xsl:value-of select="$currentYear+1"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="concat($currentYear,':',$days)"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:otherwise>
        <xsl:choose>
          <xsl:when test="$days &gt; 365">
            <xsl:call-template name="CalculateYearAndDay">
              <xsl:with-param name="days">
                <xsl:value-of select="$days -365"/>
              </xsl:with-param>
              <xsl:with-param name="currentYear">
                <xsl:value-of select="$currentYear+1"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="concat($currentYear,':',$days)"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="getColorCode">
    <xsl:param name="color"/>
    <xsl:param name="lumMod"/>
    <xsl:param name="lumOff"/>
    <xsl:param name="noTheme"/>

    <xsl:variable name="baseColor">
      <xsl:choose>
        <xsl:when test="$noTheme != '' ">
          <xsl:value-of select="$color"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:for-each select="key('Part', 'xl/theme/theme1.xml')/a:theme/a:themeElements/a:clrScheme">
            <xsl:for-each select="node()[name() = concat('a:',$color)]">

              <xsl:choose>
                <xsl:when test="contains(node()/@val,'window' ) ">
                  <xsl:value-of select="node()/@lastClr"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="node()/@val"/>
                </xsl:otherwise>
              </xsl:choose>

            </xsl:for-each>
          </xsl:for-each>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="BgTxColors">
      <xsl:if test="$color = 'bg2' ">
        <xsl:value-of select="key('Part', 'xl/theme/theme1.xml')//a:lt2/a:srgbClr/@val"/>
      </xsl:if>

      <xsl:if test="$color = 'bg1' ">
        <xsl:choose>
          <xsl:when test="key('Part', 'xl/theme/theme1.xml')//a:lt1/a:srgbClr/@val">
            <xsl:value-of select="key('Part', 'xl/theme/theme1.xml')//a:lt1/a:srgbClr/@val"/>
          </xsl:when>
          <xsl:when test="key('Part', 'xl/theme/theme1.xml')//a:lt1/node()/@lastClr">
            <xsl:value-of select="key('Part', 'xl/theme/theme1.xml')//a:lt1/node()/@lastClr"/>
          </xsl:when>
        </xsl:choose>
      </xsl:if>

      <xsl:if test="$color = 'tx1' ">
        <xsl:choose>
          <xsl:when test="key('Part', 'xl/theme/theme1.xml')//a:dk1/node()/@lastClr">
            <xsl:value-of select="key('Part', 'xl/theme/theme1.xml')//a:dk1/node()/@lastClr"/>
          </xsl:when>
          <xsl:when test="key('Part', 'xl/theme/theme1.xml')//a:dk1/node()/@val">
            <xsl:value-of select="key('Part', 'xl/theme/theme1.xml')//a:dk1/node()/@val"/>
          </xsl:when>
        </xsl:choose>
      </xsl:if>

      <xsl:if test="$color = 'tx2' ">
        <xsl:value-of select="key('Part', 'xl/theme/theme1.xml')//a:dk2/a:srgbClr/@val"/>
      </xsl:if>
    </xsl:variable>

    <xsl:variable name="NewColor">
      <xsl:if test="$baseColor != '' ">
        <xsl:value-of select="$baseColor"/>
      </xsl:if>
      <xsl:if test="$BgTxColors != '' ">
        <xsl:value-of select="$BgTxColors"/>
      </xsl:if>
    </xsl:variable>

    <xsl:call-template name="ConvertThemeColor">
      <xsl:with-param name="color" select="$NewColor"/>
      <xsl:with-param name="lumMod" select="$lumMod"/>
      <xsl:with-param name="lumOff" select="$lumOff"/>
    </xsl:call-template>

  </xsl:template>

  <xsl:template name="ConvertThemeColor">
    <xsl:param name="color"/>
    <xsl:param name="lumMod"/>
    <xsl:param name="lumOff"/>

    <xsl:variable name="Red">
      <xsl:call-template name="HexToDec">
        <xsl:with-param name="number" select="substring($color,1,2)"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="Green">
      <xsl:call-template name="HexToDec">
        <xsl:with-param name="number" select="substring($color,3,2)"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="Blue">
      <xsl:call-template name="HexToDec">
        <xsl:with-param name="number" select="substring($color,5,2)"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="$lumOff = '' and $lumMod != '' ">
        <xsl:variable name="NewRed">
          <xsl:value-of select=" floor($Red * $lumMod div 100000) "/>
        </xsl:variable>

        <xsl:variable name="NewGreen">
          <xsl:value-of select=" floor($Green * $lumMod div 100000)"/>
        </xsl:variable>

        <xsl:variable name="NewBlue">
          <xsl:value-of select=" floor($Blue * $lumMod div 100000)"/>
        </xsl:variable>

        <xsl:text>#</xsl:text>
        <xsl:call-template name="DecToHex">
          <xsl:with-param name="number" select="$NewRed"/>
        </xsl:call-template>
        <xsl:call-template name="DecToHex">
          <xsl:with-param name="number" select="$NewGreen"/>
        </xsl:call-template>
        <xsl:call-template name="DecToHex">
          <xsl:with-param name="number" select="$NewBlue"/>
        </xsl:call-template>
      </xsl:when>

      <xsl:when test="$lumMod = '' and $lumOff != '' ">
        <!-- TBD Not sure whether this condition will occur-->
      </xsl:when>

      <xsl:when test="$lumMod = '' and $lumOff = '' ">
        <xsl:value-of select="concat('#',$color)"/>
      </xsl:when>

      <xsl:when test="$lumOff != '' and $lumMod!= '' ">
        <xsl:variable name="NewRed">
          <xsl:value-of select="floor(((255 - $Red) * (1 - ($lumMod  div 100000)))+ $Red )"/>
        </xsl:variable>

        <xsl:variable name="NewGreen">
          <xsl:value-of select="floor(((255 - $Green) * ($lumOff  div 100000)) + $Green )"/>
        </xsl:variable>

        <xsl:variable name="NewBlue">
          <xsl:value-of select="floor(((255 - $Blue) * ($lumOff div 100000)) + $Blue) "/>
        </xsl:variable>

        <xsl:text>#</xsl:text>
        <xsl:call-template name="DecToHex">
          <xsl:with-param name="number" select="$NewRed"/>
        </xsl:call-template>
        <xsl:call-template name="DecToHex">
          <xsl:with-param name="number" select="$NewGreen"/>
        </xsl:call-template>
        <xsl:call-template name="DecToHex">
          <xsl:with-param name="number" select="$NewBlue"/>
        </xsl:call-template>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

	<xsl:template name ="getTableRangeSheetName">
		<xsl:param name ="tblName"/>
		<!-- Step 1: from worksheet get path for sheets-->
		<xsl:variable name ="tblSheetName">
			<xsl:for-each select="key('Part', 'xl/workbook.xml')/e:workbook/e:sheets">
				<xsl:for-each select ="e:sheet">
					<xsl:variable name="sheet">
						<xsl:call-template name="GetTarget">
							<xsl:with-param name="id">
								<xsl:value-of select="@r:id"/>
							</xsl:with-param>
							<xsl:with-param name="document">xl/workbook.xml</xsl:with-param>
						</xsl:call-template>
					</xsl:variable>
					<xsl:variable name ="sheetName">
						<xsl:value-of select ="@name"/>
					</xsl:variable>
					<xsl:for-each select ="key('Part', concat('xl/',$sheet))/e:worksheet/e:tableParts/e:tablePart">
						<xsl:variable name ="tablePart">
							<xsl:call-template name="GetTarget">
								<xsl:with-param name="id">
									<xsl:value-of select="@r:id"/>
								</xsl:with-param>
								<xsl:with-param name="document">
									<xsl:value-of select="concat('xl/',$sheet)"/>
								</xsl:with-param>
							</xsl:call-template>
						</xsl:variable>
						<xsl:for-each select ="document(concat('xl/',substring-after($tablePart,'../')))//node()[name()='table' and @name=$tblName]">
							<xsl:value-of select ="concat(@ref,'|',$sheetName)"/>
						</xsl:for-each>
					</xsl:for-each>
				</xsl:for-each>
			</xsl:for-each>
		</xsl:variable>
		<xsl:value-of select ="$tblSheetName"/>
	</xsl:template>

</xsl:stylesheet>
