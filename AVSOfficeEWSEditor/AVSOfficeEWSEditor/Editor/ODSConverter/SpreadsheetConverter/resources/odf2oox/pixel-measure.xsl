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
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  
  <!--      U n i t s
    1 pt = 20 twip
    1 in = 72 pt = 1440 twip
    1 cm = 1440 / 2.54 twip
    1 pica = 12 pt
    1 dpt (didot point) = 1/72 in (almost the same as 1 pt)
    1 px = 0.0264cm at 96dpi (Windows default)
    1 milimeter(mm) = 0.1cm 
  -->
  
  <!-- 
    Convert various length units to pixels
  -->
<!--Change By : Vijayeta
    Changes   : Additional condition 'check' added, cos in case of col width,
                when the calculation is rounded, exact calclation is lost and 
                col wid is found to be lesser than expected
    File       : Excel_SmokeTest_InputFile..xlsx->sp2->Excel_SmokeTest_InputFile..ods->Translator->Excel_SmokeTest_InputFile..xlsx 
-->
  <xsl:template name="pixel-measure">
    <xsl:param name="length"/>
	<xsl:param name="check" select ="0"/>
	  <xsl:choose >
		  <xsl:when test ="$check=0">
    <xsl:choose>
      <xsl:when test="contains($length, 'cm')">
        <xsl:value-of select="round(number(substring-before($length, 'cm')) div 0.0264)"/>
      </xsl:when>
      <xsl:when test="contains($length, 'mm')">
        <xsl:value-of select="round(number(substring-before($length, 'mm')) div 0.264)"/>
      </xsl:when>
      <xsl:when test="contains($length, 'in')">
        <xsl:value-of select="round(number(substring-before($length, 'in')) * 2.54 div 0.0264)"/>
      </xsl:when>
      <xsl:when test="contains($length, 'pt')">
        <xsl:value-of select="round(number(substring-before($length, 'pt')) * 96 div 72)"/>
      </xsl:when>
      <xsl:when test="contains($length, 'pica')">
        <xsl:value-of select="round(number(substring-before($length, 'pica')) * 2.54 div 0.1584)"/>
      </xsl:when>
      <xsl:when test="contains($length, 'dpt')">
        <xsl:value-of select="round(number(substring-before($length, 'dpt')) * 2.54 div 1.9008)"/>
      </xsl:when>
      <xsl:when test="contains($length, 'px')">
        <xsl:value-of select="$length"/>
      </xsl:when>
      <xsl:when test="not($length) or $length='' ">0</xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$length"/>
      </xsl:otherwise>
    </xsl:choose>
		  </xsl:when>
		  <xsl:when test ="$check=1">
			  <xsl:choose>
				  <xsl:when test="contains($length, 'cm')">
					  <xsl:value-of select="number(substring-before($length, 'cm')) div 0.0264"/>
				  </xsl:when>
				  <xsl:when test="contains($length, 'mm')">
					  <xsl:value-of select="number(substring-before($length, 'mm')) div 0.264"/>
				  </xsl:when>
				  <xsl:when test="contains($length, 'in')">
					  <xsl:value-of select="number(substring-before($length, 'in')) * 2.54 div 0.0264"/>
				  </xsl:when>
				  <xsl:when test="contains($length, 'pt')">
					  <xsl:value-of select="number(substring-before($length, 'pt')) * 96 div 72"/>
				  </xsl:when>
				  <xsl:when test="contains($length, 'pica')">
					  <xsl:value-of select="number(substring-before($length, 'pica')) * 2.54 div 0.1584"/>
				  </xsl:when>
				  <xsl:when test="contains($length, 'dpt')">
					  <xsl:value-of select="number(substring-before($length, 'dpt')) * 2.54 div 1.9008"/>
				  </xsl:when>
				  <xsl:when test="contains($length, 'px')">
					  <xsl:value-of select="$length"/>
				  </xsl:when>
				  <xsl:when test="not($length) or $length='' ">0</xsl:when>
				  <xsl:otherwise>
					  <xsl:value-of select="$length"/>
				  </xsl:otherwise>
			  </xsl:choose>
		  </xsl:when>
	  </xsl:choose>    
  </xsl:template>  
</xsl:stylesheet>
