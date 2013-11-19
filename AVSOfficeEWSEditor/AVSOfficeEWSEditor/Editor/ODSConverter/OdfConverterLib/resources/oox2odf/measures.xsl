<?xml version="1.0" encoding="UTF-8"?>
<?xml-stylesheet href="xsl2html.xsl" type="text/xsl" media="screen"?>
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
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <!-- @Filename: measure.xsl -->
  <!-- @Description: This stylesheet is used as a toolkit library to perform 
    reverse conversion between the different measure units. Units supported are:
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
  <!-- @TODO:
    <ul>
    <li>Named template 'ConvertToMeasure' currently only support 'cm' unit.</li>
    </ul>
  -->
  
 
  <xsl:template name="ConvertTwips">
    <!-- @Description: Convert a measure in twips to a 'unit' measure -->
    <!-- @Context: None -->
    <!-- @Private -->
    
    <xsl:param name="length"/> <!-- (number) The length (in twips) to convert -->
    <xsl:param name="unit"/>   <!-- (string) The unit to convert to -->

    <xsl:variable name="result">
      <xsl:choose>
        <xsl:when test="$length='0' or $length=''">
          <xsl:value-of select="concat(0, $unit)"/>
        </xsl:when>
        <xsl:when test="$unit = 'cm'">
          <xsl:value-of select="concat(format-number($length * 2.54 div 1440,'#.###'),'cm')"/>
        </xsl:when>
        <xsl:when test="$unit = 'mm'">
          <xsl:value-of select="concat(format-number($length * 25.4 div 1440,'#.###'),'mm')"/>
        </xsl:when>
        <xsl:when test="$unit= 'in'">
          <xsl:value-of select="concat(format-number($length div 1440,'#.###'),'in')"/>
        </xsl:when>
        <xsl:when test="$unit = 'pt'">
          <xsl:value-of select="concat(format-number($length div 20,'#.###'),'pt')"/>
        </xsl:when>
        <xsl:when test="$unit = 'twip'">
          <xsl:value-of select="concat($length,'twip')"/>
        </xsl:when>
        <xsl:when test="$unit = 'pica'">
          <xsl:value-of select="concat(format-number($length div 240,'#.###'),'pica')"/>
        </xsl:when>
        <xsl:when test="$unit = 'dpt'">
          <xsl:value-of select="concat(format-number($length div 20,'#.###'),'dpt')"/>
        </xsl:when>
        <xsl:when test="$unit = 'px'">
          <xsl:value-of select="concat(format-number($length * 96.19 div 1440,'#.###'),'px')"/>
        </xsl:when>
        <xsl:when test="$unit='pct'">
          <xsl:value-of select="concat(format-number($length div 50, '#.###'), '%')"/>
        </xsl:when>
        <xsl:when test="not($length)">
          <xsl:value-of select="concat(0,'cm')"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$length"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="contains($result,'NaN')">
        <xsl:value-of select="concat('0', $unit)"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$result"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="ConvertPoints">
    <!-- @Description: Convert a measure in points to a specified unit -->
    <!-- @Context: None -->
    <!-- @Private -->
    
    <xsl:param name="length"/> <!-- (number|string) The length to convert (including or not the unit) -->
    <xsl:param name="unit"/>   <!-- (string) The unit to converted to -->
    
    <xsl:variable name="lengthVal"> <!-- The actual length (with unit removed if needed) -->
      <xsl:choose>
        <xsl:when test="contains($length,'pt')">
          <xsl:value-of select="substring-before($length,'pt')"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$length"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    
    <xsl:choose>
      <xsl:when test="$lengthVal='0' or $lengthVal=''">
        <xsl:value-of select="concat(0, $unit)"/>
      </xsl:when>
      <xsl:when test="$unit = 'cm'">
        <xsl:value-of select="concat(format-number($lengthVal * 2.54 div 72,'#.###'),'cm')"/>
      </xsl:when>
      <xsl:when test="$unit = 'mm'">
        <xsl:value-of select="concat(format-number($lengthVal * 25.4 div 72,'#.###'),'mm')"/>
      </xsl:when>
      <xsl:when test="$unit = 'in'">
        <xsl:value-of select="concat(format-number($lengthVal div 72,'#.###'),'in')"/>
      </xsl:when>
      <xsl:when test="$unit = 'pt'">
        <xsl:value-of select="concat($lengthVal,'pt')"/>
      </xsl:when>
      <xsl:when test="$unit = 'pica'">
        <xsl:value-of select="concat(format-number($lengthVal div 12,'#.###'),'pica')"/>
      </xsl:when>
      <xsl:when test="$unit = 'dpt'">
        <xsl:value-of select="concat($lengthVal,'dpt')"/>
      </xsl:when>
      <xsl:when test="$unit = 'px'">
        <xsl:value-of select="concat(format-number($lengthVal * 96.19 div 72,'#.###'),'px')"/>
      </xsl:when>
      <xsl:when test="not($lengthVal)">
        <xsl:value-of select="concat(0,'cm')"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$lengthVal"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="ConvertHalfPoints">
    <!-- @Description: Convert a measure in half points to a specified unit -->
    <!-- @Context: None -->
    <!-- @Private -->
    
    <xsl:param name="length"/> <!-- (number) The length to convert -->
    <xsl:param name="unit"/>   <!-- (string) The unit to convert to -->
    
    <xsl:choose>
      <xsl:when test="$length='0' or $length=''">
        <xsl:value-of select="concat(0, $unit)"/>
      </xsl:when>
      <xsl:when test="$unit = 'cm'">
        <xsl:value-of select="concat(format-number($length * 2.54 div 144,'#.###'),'cm')"/>
      </xsl:when>
      <xsl:when test="$unit = 'mm'">
        <xsl:value-of select="concat(format-number($length * 25.4 div 144,'#.###'),'mm')"/>
      </xsl:when>
      <xsl:when test="$unit = 'in'">
        <xsl:value-of select="concat(format-number($length div 144,'#.###'),'in')"/>
      </xsl:when>
      <xsl:when test="$unit = 'pt'">
        <xsl:value-of select="concat($length div 2,'pt')"/>
      </xsl:when>
      <xsl:when test="$unit = 'pica'">
        <xsl:value-of select="concat(format-number($length div 144,'#.###'),'pica')"/>
      </xsl:when>
      <xsl:when test="$unit = 'dpt'">
        <xsl:value-of select="concat($length div 2,'dpt')"/>
      </xsl:when>
      <xsl:when test="$unit = 'px'">
        <xsl:value-of select="concat(format-number($length * 96.19 div 144,'#.###'),'px')"/>
      </xsl:when>
      <xsl:when test="not($length)">
        <xsl:value-of select="concat(0,'cm')"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$length"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="ConvertEighthsPoints">
    <!-- @Description: Convert a measure in eigths of a point to a specified unit -->
    <!-- @Context: None -->
    <!-- @Private -->
    
    <xsl:param name="length"/> <!-- (number) The length (in eigths of a point) to convert -->
    <xsl:param name="unit"/> <!-- (string) The unit to converted to -->
    
    <xsl:choose>
      <xsl:when test="$length='0' or $length=''">
        <xsl:value-of select="concat(0, $unit)"/>
      </xsl:when>
      <xsl:when test="$unit = 'cm'">
        <xsl:value-of select="concat(format-number($length * 2.54 div 576,'#.###'),'cm')"/>
      </xsl:when>
      <xsl:when test="$unit = 'mm'">
        <xsl:value-of select="concat(format-number($length * 25.4 div 576,'#.###'),'mm')"/>
      </xsl:when>
      <xsl:when test="$unit = 'in'">
        <xsl:value-of select="concat(format-number($length div 576,'#.###'),'in')"/>
      </xsl:when>
      <xsl:when test="$unit = 'pt'">
        <xsl:value-of select="concat(format-number($length div 8,'#.###'),'pt')"/>
      </xsl:when>
      <xsl:when test="$unit = 'pica'">
        <xsl:value-of select="concat(format-number($length div 96,'#.###'),'pica')"/>
      </xsl:when>
      <xsl:when test="$unit = 'dpt'">
        <xsl:value-of select="concat(format-number($length div 8,'#.###'),'dpt')"/>
      </xsl:when>
      <xsl:when test="$unit = 'px'">
        <xsl:value-of select="concat(format-number($length * 96.19 div 576,'#.###'),'px')"/>
      </xsl:when>
      <xsl:when test="not($length)">
        <xsl:value-of select="concat(0,'cm')"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$length"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="ConvertEmu">
    <!--  @Description: Convert emu to a specified unit (2 decimals) -->
    <!-- @Context: None -->
    <!-- @Private -->
    
    <xsl:param name="length"/> <!-- (number) The length (in emu) to convert -->
    <xsl:param name="unit"/>   <!-- (string) The unit to converted to -->
    
    <xsl:choose>
      <xsl:when
        test="$length = '' or not($length) or $length = 0 or format-number($length div 360000, '#.##') = ''">
        <xsl:value-of select="concat(0,'cm')"/>
      </xsl:when>
      <xsl:when test="$unit = 'cm'">
        <xsl:value-of select="concat(format-number($length div 360000, '0.##'), 'cm')"/>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="ConvertEmu3">
    <!-- @Description: Convert emu to a specified unit (3 decimals) -->
    <!-- @Context: None -->
    <!-- @Private -->
    
    <xsl:param name="length"/> <!-- (number) The length (in emu) to convert -->
    <xsl:param name="unit"/>   <!-- (string) The unit to converted to -->
    <xsl:choose>
      <xsl:when
        test="$length = '' or not($length) or $length = 0 or format-number($length div 360000, '#.###') = ''">
        <xsl:value-of select="concat(0,'cm')"/>
      </xsl:when>
      <xsl:when test="$unit = 'cm'">
        <xsl:value-of select="concat(format-number($length div 360000, '0.###'), 'cm')"/>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="GetValue">
    <!-- @Description: Extract scalar from length -->
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
    <!-- @Description: Convert length from specified unit to a speficied target unit.
      The conversion table is as follows:
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
    
    <xsl:param name="length"/>                 <!-- (number|string) The length to convert including or not the unit -->
    <xsl:param name="sourceUnit"/>             <!-- (string) The unit to convert from. If omitted, the unit should be specified in the length parameter -->
    <xsl:param name="destUnit"/>               <!-- (string) The unit to convert to -->
    <xsl:param name="addUnit">true</xsl:param> <!-- (string) If set to 'true', will append the convertion unit to the result -->
    
    <xsl:choose>
      <xsl:when test="$length='' or $length='0' or $length='0cm' or $length='0mm' or $length='0in' or $length='0pt' or $length='0twip' or $length='0pika' or $length='0dpt' or $length='0px'">
        <xsl:choose>
          <xsl:when test="$addUnit = 'true'">
            <xsl:value-of select="concat('0', $destUnit)"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="'0'"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <!-- used when unit type is given in length string -->
      <xsl:when test="$sourceUnit = ''">
        <xsl:call-template name="ConvertToMeasure">
          <xsl:with-param name="length" select="$length"/>
          <xsl:with-param name="destUnit" select="$destUnit"/>
          <xsl:with-param name="addUnit" select="$addUnit"/>
        </xsl:call-template>
      </xsl:when>
      <!-- used when unit type is not given in length string -->
      <xsl:otherwise>
        <xsl:call-template name="ConvertFromMeasure">
          <xsl:with-param name="length" select="$length"/>
          <xsl:with-param name="sourceUnit" select="$sourceUnit"/>
          <xsl:with-param name="destUnit" select="$destUnit"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="ConvertFromMeasure">
    <!--  @Description: Convert from given measure - for usage when unit type is not given in string-->
    <!-- @Private -->
    <!-- @Context: None -->
    
    <xsl:param name="length"/>     <!-- (number) The length to convert -->
    <xsl:param name="sourceUnit"/> <!-- (string) The unit to convert from -->
    <xsl:param name="destUnit"/>   <!-- (string) The unit to convert to -->
    
    <xsl:choose>
      <xsl:when test="$sourceUnit = 'eighths-points' ">
        <xsl:call-template name="ConvertEighthsPoints">
          <xsl:with-param name="length" select="$length"/>
          <xsl:with-param name="unit" select="$destUnit"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="$sourceUnit = 'half-points' ">
        <xsl:call-template name="ConvertHalfPoints">
          <xsl:with-param name="length" select="$length"/>
          <xsl:with-param name="unit" select="$destUnit"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="$sourceUnit = 'pt' ">
        <xsl:call-template name="ConvertPoints">
          <xsl:with-param name="length" select="$length"/>
          <xsl:with-param name="unit" select="$destUnit"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="$sourceUnit = 'twip' ">
        <xsl:call-template name="ConvertTwips">
          <xsl:with-param name="length" select="$length"/>
          <xsl:with-param name="unit" select="$destUnit"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="$sourceUnit = 'emu' ">
        <xsl:call-template name="ConvertEmu">
          <xsl:with-param name="length" select="$length"/>
          <xsl:with-param name="unit" select="$destUnit"/>
        </xsl:call-template>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="ConvertToMeasure">
    <!--  @Description: Convert to given measure - for usage when unit type is given in string-->
    <!-- @Context: None -->
    <!-- @Private -->
    <!-- @TODO: other units (at the moment only cm is supported)-->
    
    <xsl:param name="length"/>                 <!-- (string) The length to convert (including the unit) -->
    <xsl:param name="destUnit"/>               <!-- (string) The unit to convert to -->
    <xsl:param name="addUnit">true</xsl:param> <!-- (string) If set to true, wil append 'cm' to the result -->
    
    <xsl:choose>
      <xsl:when test="contains($destUnit, 'cm')">
        <xsl:call-template name="ConvertToCentimeters">
          <xsl:with-param name="length" select="$length"/>
          <xsl:with-param name="addUnit" select="$addUnit"/>
        </xsl:call-template>
      </xsl:when>
      
    </xsl:choose>
  </xsl:template>

  <xsl:template name="ConvertToCentimeters">
    <!-- @Description: Convert given unit to cm -->
    <!-- @Context: None -->
    <!-- @Private -->
    
    <xsl:param name="length"/>                <!-- (string) The length to convert (including the unit) -->
    <!--<xsl:param name="round">false</xsl:param>--> <!-- (string) If set to true, the result will be rounded to the nearest integer, otherwise the result will be a 3 decimal digit -->
    <xsl:param name="addUnit">true</xsl:param><!-- (string) If set to true, wil append 'cm' to the result -->
    
    <xsl:variable name="newlength"> <!-- The convertion's result -->
      <xsl:choose>
        <xsl:when test="contains($length, 'cm')">
          <xsl:value-of select="$length"/>
        </xsl:when>
        <xsl:when test="contains($length, 'mm')">
          <xsl:value-of select="format-number(substring-before($length, 'mm') div 10,'#.###')"/>
        </xsl:when>
        <xsl:when test="contains($length, 'in')">
          <xsl:value-of select="format-number(substring-before($length, 'in') * 2.54,'#.###')"/>
        </xsl:when>
        <xsl:when test="contains($length, 'pt')">
          <xsl:value-of
            select="format-number(substring-before($length, 'pt') * 2.54 div 72,'#.###')"/>
        </xsl:when>
        <xsl:when test="contains($length, 'twip')">
          <xsl:value-of
            select="format-number(substring-before($length, 'twip') * 2.54 div 1440,'#.###')"/>
        </xsl:when>
        <xsl:when test="contains($length, 'pica')">
          <xsl:value-of
            select="format-number(substring-before($length, 'pica') * 2.54 div 6,'#.###')"/>
        </xsl:when>
        <xsl:when test="contains($length, 'dpt')">
          <xsl:value-of
            select="format-number(substring-before($length, 'pt') * 2.54 div 72,'#.###')"/>
        </xsl:when>
        <xsl:when test="contains($length, 'px')">
          <xsl:value-of select="format-number(substring-before($length, 'px') * 0.0264,'#.###')"/>
        </xsl:when>
        <xsl:when test="not($length) or $length='' or number($length) = 0">0</xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="format-number($length * 2.54 div 1440,'#.###')"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    
    <xsl:variable name="roundLength"> <!-- The result of the rounding -->
      <!--<xsl:choose>
        <xsl:when test="$round='true'">
          <xsl:value-of select="round($newlength)"/>
        </xsl:when>
        <xsl:otherwise>-->
          <xsl:value-of select="(round($newlength * 1000)) div 1000"/>
        <!--</xsl:otherwise>
      </xsl:choose>-->
    </xsl:variable>
    
    <xsl:choose>
      <xsl:when test="$addUnit = 'true' "> 
        <xsl:value-of select="concat($roundLength, 'cm')"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$roundLength"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  
  <xsl:template name="HexToDec">
    <!-- @Description: This is a recurive algorithm converting a hex to decimal -->
    <!-- @Context: None -->
    
    <xsl:param name="number"/>            <!-- (string|number) The hex number to convert -->
    <xsl:param name="step" select="0"/>   <!-- (number) The exponent (only used during convertion)-->
    <xsl:param name="value" select="0"/>  <!-- (number) The result from the previous digit's convertion (only used during convertion) -->
    
    <xsl:variable name="number1"> <!-- translates all letters to lower case -->
      <xsl:value-of select="translate($number,'ABCDEF','abcdef')"/>
    </xsl:variable>
    
    <xsl:choose>
      <xsl:when test="string-length($number1) &gt; 0">
        
        <xsl:variable name="one"> <!-- The last digit in the hex number -->
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
        
        <xsl:variable name="power">   <!-- The result of the exponent calculation -->
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

 
  <xsl:template name="DecToHex">
    <!-- @Description: Converts an decimal to hexadecimal -->
    <!-- @Context: None -->
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
          <xsl:value-of select="floor($number mod 16)"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>
    
    <xsl:value-of select="concat($high,$low)"/>
  </xsl:template>
  
  <xsl:template name="HexMap">
    <!-- @Description : Maps [10-15] decimal values to hexadecimal digit -->
    <!-- @Context: None -->
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
  
  
  <xsl:template name="Power">
    <!-- @Description: Calculates the power of a given number (i.e. x<sup>y</sup>) -->
    <!-- @Context: None -->
    
    <xsl:param name="base"/>     <!-- (number) The exponent base -->
    <xsl:param name="exponent"/> <!-- (number) The exponent (e.g. y)-->
    <xsl:param name="value1"  select="$base"/>   <!-- (number) The number on which the exponent should be applied (e.g. x)  -->
    
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
