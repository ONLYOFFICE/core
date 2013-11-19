<?xml version="1.0" encoding="UTF-8"?>
<?xml-stylesheet href="xsl2html.xsl" type="text/xsl" media="screen"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

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
  
  <!-- @Filename: measures.xsl -->
  <!-- @Description: This stylesheet is used as a toolkit library to perform 
    direct conversion between the different measure units. Units supported are:
    <ul>
    <li>pt (points)</li>
    <li>in (inches)</li>
    <li>cm (centimeter)</li>
    <li>mm (milimeter)</li>
    <li>px (pixels)</li>
    <li>pica (1 PostScript pica)</li>
    <li>dpt (Didot point)</li>
    </ul>
  -->
  <!-- @Created: 2006-12-12 -->    
 
  <xsl:template name="GetUnit">
    <!-- @Description: Gets the unit of a length. -->  
    <!-- @Context: None -->
   
    <xsl:param name="length"/> <!-- (string) The length including the unit to be returned 
                                      (see units supported above) -->
    
    <xsl:choose>
      <xsl:when test="contains($length, 'cm')">cm</xsl:when>
      <xsl:when test="contains($length, 'mm')">mm</xsl:when>
      <xsl:when test="contains($length, 'in')">in</xsl:when>
      <xsl:when test="contains($length, 'pt')">pt</xsl:when>
      <xsl:when test="contains($length, 'twip')">twip</xsl:when>
      <xsl:when test="contains($length, 'pica')">pica </xsl:when>
      <xsl:when test="contains($length, 'dpt')">dpt</xsl:when>
      <xsl:when test="contains($length, 'px')">px</xsl:when>
    </xsl:choose>
  </xsl:template>



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


  <xsl:template name="GetValue">
    <!-- @Description: Gets the unit-less value of a measured length -->  
    <!-- @Context: None -->
    
    <xsl:param name="length"/> <!-- (string) The length including the unit -->
    <xsl:choose>
      <xsl:when test="contains($length, 'cm')">
        <xsl:value-of select="substring-before($length,'cm')"/>
      </xsl:when>
      <xsl:when test="contains($length, 'mm')">
        <xsl:value-of select="substring-before($length,'mm')"/>
      </xsl:when>
      <xsl:when test="contains($length, 'in')">
        <xsl:value-of select="substring-before($length,'in')"/>
      </xsl:when>
      <xsl:when test="contains($length, 'pt')">
        <xsl:value-of select="substring-before($length,'pt')"/>
      </xsl:when>
      <xsl:when test="contains($length, 'twip')">
        <xsl:value-of select="substring-before($length,'twip')"/>
      </xsl:when>
      <xsl:when test="contains($length, 'pica')">
        <xsl:value-of select="substring-before($length,'pica')"/>
      </xsl:when>
      <xsl:when test="contains($length, 'dpt')">
        <xsl:value-of select="substring-before($length,'dpt')"/>
      </xsl:when>
      <xsl:when test="contains($length, 'px')">
        <xsl:value-of select="substring-before($length,'px')"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$length"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>


  <xsl:template name="ConvertMeasure">
    <!-- @Description: Converts a measure with a given unit. The conversion table is as follows:
      <ul>
      <li>1 pt   = 20 twip                             </li>
      <li>1 in   = 72 pt = 1440 twip                   </li>
      <li>1 cm   = 1440 / 2.54 twip                    </li>
      <li>1 pica = 12 pt                               </li>
      <li>1 dpt  = 1/72 in (almost the same as 1 pt)   </li>
      <li>1 px   = 0.0264cm at 96dpi (Windows default) </li>
      <li>1 mm   = 0.1cm                               </li>
      </ul>
    -->
    <!-- @Context: None -->

    <xsl:param name="length"/> <!-- (string) The length including the unit -->
    <xsl:param name="unit"/>   <!-- (string) The unit to be converted to -->
    
    <xsl:choose>
      <xsl:when test="$unit = 'twips' ">
        <xsl:call-template name="twips-measure">
          <xsl:with-param name="length" select="$length"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="$unit = 'milimeter' ">
        <xsl:call-template name="milimeter-measure">
          <xsl:with-param name="length" select="$length"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="$unit = 'eightspoint' ">
        <xsl:call-template name="eightspoint-measure">
          <xsl:with-param name="length" select="$length"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="$unit = 'emu' ">
        <xsl:call-template name="emu-measure">
          <xsl:with-param name="length" select="$length"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="$unit = 'point' ">
        <xsl:call-template name="point-measure">
          <xsl:with-param name="length" select="$length"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>0</xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  
  
  <xsl:template name="DegreesAngle">
    <!-- @Description: Converts a radian angle to degrees. Returns 0 if no value is specified. -->  
    <!-- @Context: None -->
    
    <xsl:param name="angle"/><!-- (number) The angle to be converted (in radiant) -->
    <xsl:param name="select" select="'false'"/><!-- (string) If set to 'true', only converts the values pi, pi/2, -pi, -pi/2 -->
    <xsl:param name="revert" select="'false'"/><!-- (string) If set to 'true', returns a negated result -->
    
    <xsl:variable name="val"><!-- Contains the convertion's result  -->     
      <xsl:choose>
        <xsl:when test="not($angle)">0</xsl:when>
        <xsl:when test="not($select = 'true')">
          <xsl:value-of select="round(number($angle) * 90 div 1.5707963267946)"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:choose>
            <xsl:when test="$angle = 1.5707963267946">90</xsl:when>
            <xsl:when test="$angle = -1.5707963267946">-90</xsl:when>
            <xsl:when test="$angle = 3.1415926535892">180</xsl:when>
            <xsl:when test="$angle = -3.1415926535892">180</xsl:when>
            <xsl:otherwise>0</xsl:otherwise>
          </xsl:choose>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    
    <xsl:choose>
      <xsl:when test="$revert = 'true' ">
        <xsl:value-of select=" - number($val)"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$val"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="twips-measure">
    <!-- @Private -->
    <!-- @Description: Convert various length units to twips (twentieths of a point)	-->
    <!-- @Context: None -->
    
    <xsl:param name="length"/> <!-- (string) The length including the unit -->
    
    <xsl:choose>
      <xsl:when test="contains($length, 'cm')">
        <xsl:value-of select="round(number(substring-before($length, 'cm')) * 1440 div 2.54)"/>
      </xsl:when>
      <xsl:when test="contains($length, 'mm')">
        <xsl:value-of select="round(number(substring-before($length, 'mm')) * 1440 div 25.4)"/>
      </xsl:when>
      <xsl:when test="contains($length, 'in')">
        <xsl:value-of select="round(number(substring-before($length, 'in')) * 1440)"/>
      </xsl:when>
      <xsl:when test="contains($length, 'pt')">
        <xsl:value-of select="round(number(substring-before($length, 'pt')) * 20)"/>
      </xsl:when>
      <xsl:when test="contains($length, 'twip')">
        <xsl:value-of select="substring-before($length, 'twip')"/>
      </xsl:when>
      <xsl:when test="contains($length, 'pica')">
        <xsl:value-of select="round(number(substring-before($length, 'pica')) * 240)"/>
      </xsl:when>
      <xsl:when test="contains($length, 'dpt')">
        <xsl:value-of select="round(number(substring-before($length, 'dpt')) * 20)"/>
      </xsl:when>
      <xsl:when test="contains($length, 'px')">
        <xsl:value-of select="round(number(substring-before($length, 'px')) * 1440 div 96.19)"/>
      </xsl:when>
      <xsl:when test="not($length) or $length='' ">0</xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$length"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="milimeter-measure">
    <!-- @Private -->
    <!-- @Description:  Convert various length units to milimeters -->
    <!-- @Context: None -->
    
    <xsl:param name="length"/> <!-- (string) The length including the unit -->
    
    <xsl:param name="round">true</xsl:param>
    <xsl:variable name="newlength">
      <xsl:choose>
        <xsl:when test="contains($length, 'cm')">
          <xsl:value-of select="number(substring-before($length, 'cm')) * 10"/>
        </xsl:when>
        <xsl:when test="contains($length, 'mm')">
          <xsl:value-of select="number(substring-before($length, 'mm'))"/>
        </xsl:when>
        <xsl:when test="contains($length, 'in')">
          <xsl:value-of select="number(substring-before($length, 'in')) * 25.4"/>
        </xsl:when>
        <xsl:when test="contains($length, 'pt')">
          <xsl:value-of select="number(substring-before($length, 'pt')) * 25.4 div 72"/>
        </xsl:when>
        <xsl:when test="contains($length, 'twip')">
          <xsl:value-of select="number(substring-before($length, 'twip')) * 25.4 div 1440"/>
        </xsl:when>
        <xsl:when test="contains($length, 'pica')">
          <xsl:value-of select="number(substring-before($length, 'pica')) * 25.4 div 6"/>
        </xsl:when>
        <xsl:when test="contains($length, 'dpt')">
          <xsl:value-of select="number(substring-before($length, 'pt')) * 25.4 div 72"/>
        </xsl:when>
        <xsl:when test="contains($length, 'px')">
          <xsl:value-of select="number(substring-before($length, 'px')) * 0.264"/>
        </xsl:when>
        <xsl:when test="not($length) or $length='' ">0</xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$length"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="$round='true'">
        <xsl:value-of select="round($newlength)"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="(round($newlength * 100)) div 100"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="eightspoint-measure">
    <!-- @Private -->
    <!-- @Description: Convert  length units to eights of a point	-->
    <!-- @Context: None -->
    
    <xsl:param name="length"/> <!-- (string) The length including the unit -->
    
    <xsl:choose>
      <xsl:when test="contains($length, 'cm')">
        <xsl:value-of select="round(number(substring-before($length, 'cm')) * 576 div 2.54)"/>
      </xsl:when>
      <xsl:when test="contains($length, 'mm')">
        <xsl:value-of select="round(number(substring-before($length, 'mm')) * 576 div 25.4)"/>
      </xsl:when>
      <xsl:when test="contains($length, 'in')">
        <xsl:value-of select="round(number(substring-before($length, 'in')) * 576)"/>
      </xsl:when>
      <xsl:when test="contains($length, 'pt')">
        <xsl:value-of select="round(number(substring-before($length, 'pt')) * 8)"/>
      </xsl:when>
      <xsl:when test="contains($length, 'pica')">
        <xsl:value-of select="round(number(substring-before($length, 'pica')) * 96)"/>
      </xsl:when>
      <xsl:when test="contains($length, 'dpt')">
        <xsl:value-of select="round(number(substring-before($length, 'dpt')) * 8)"/>
      </xsl:when>
      <xsl:when test="contains($length, 'px')">
        <xsl:value-of select="round(number(substring-before($length, 'px')) * 576 div 96.19)"/>
      </xsl:when>
      <xsl:when test="not($length) or $length='' ">0</xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$length"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="point-measure">
    <!-- @Private -->
    <!-- @Description: Convert  length units to point -->
    <!-- @Context: None -->
    
    <xsl:param name="length"/> <!-- (string) The length including the unit -->
    
    <xsl:param name="round">true</xsl:param>
    <xsl:variable name="newlength">
      <xsl:choose>
        <xsl:when test="contains($length, 'cm')">
          <xsl:value-of select="number(substring-before($length, 'cm')) * 72 div 2.54"/>
        </xsl:when>
        <xsl:when test="contains($length, 'mm')">
          <xsl:value-of select="number(substring-before($length, 'mm')) * 72 div 25.4"/>
        </xsl:when>
        <xsl:when test="contains($length, 'in')">
          <xsl:value-of select="number(substring-before($length, 'in')) * 72"/>
        </xsl:when>
        <xsl:when test="contains($length, 'pt')">
          <xsl:value-of select="number(substring-before($length, 'pt'))"/>
        </xsl:when>
        <xsl:when test="contains($length, 'pica')">
          <xsl:value-of select="number(substring-before($length, 'pica')) * 12"/>
        </xsl:when>
        <xsl:when test="contains($length, 'dpt')">
          <xsl:value-of select="number(substring-before($length, 'dpt'))"/>
        </xsl:when>
        <xsl:when test="contains($length, 'px')">
          <xsl:value-of select="number(substring-before($length, 'px')) * 72 div 96.19"/>
        </xsl:when>
        <xsl:when test="not($length) or $length='' ">0</xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$length"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="$round='true'">
        <xsl:value-of select="round($newlength)"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="(round($newlength * 100)) div 100"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="emu-measure">
    <!-- @Private -->
    <!-- @Description: Convert to emu (1cm = 360000 emu) -->
    <!-- @Context: None -->

    <!-- (string) The length including the unit -->
    <xsl:param name="length"/>

    <xsl:choose>
      <xsl:when test="contains($length, 'cm')">
        <xsl:value-of select="round(number(substring-before($length, 'cm')) * 360000)"/>
      </xsl:when>
      <xsl:when test="contains($length, 'mm')">
        <xsl:value-of select="round(number(substring-before($length, 'mm')) * 36000)"/>
      </xsl:when>
      <xsl:when test="contains($length, 'in')">
        <xsl:value-of select="round(number(substring-before($length, 'in')) * 360000 * 2.54)"/>
      </xsl:when>
      <xsl:when test="contains($length, 'pt')">
        <xsl:value-of select="round(number(substring-before($length, 'pt')) * 360000 * 2.54 div 72)"
        />
      </xsl:when>
      <xsl:when test="contains($length, 'px')">
        <xsl:value-of select="round(number(substring-before($length, 'px')) * 360000 div 37.87)"/>
      </xsl:when>
      <xsl:when test="not($length) or $length='' ">0</xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$length"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="Power">
    <!-- @Description: Calculates the power of a given number (i.e. x<sup>y</sup>) -->
    <!-- @Context: None -->

    <xsl:param name="base"/>
    <!-- (number) The exponent base -->
    <xsl:param name="exponent"/>
    <!-- (number) The exponent (e.g. y)-->
    <xsl:param name="value1"  select="$base"/>
    <!-- (number) The number on which the exponent should be applied (e.g. x)  -->

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