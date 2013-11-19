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
<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"
                xmlns="http://www.w3.org/1999/xhtml"
  exclude-result-prefixes="w">

	<!--
  Author: Clever Age
  Modified: makz (DIaLOGIKa)
  Date: 15.10.2007
  
  Template inserts legal 6-digit hex color
  -->

	<xsl:template name="IsHexNumber">
		<xsl:param name="str"/>
		<xsl:choose>
			<xsl:when test="string-length($str) = 0">
				<xsl:text>true</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:variable name="isNumericChar" select="translate( substring( $str, 1, 1 ), '0123456789abcdefABCDEF', '1111111111111111111111')"/>
				<xsl:choose>
					<xsl:when test="$isNumericChar = '1'">
						<xsl:call-template name="IsHexNumber">
							<xsl:with-param name="str" select="substring( $str, 2, string-length($str) - 1 )"/>
						</xsl:call-template>
					</xsl:when>
					<xsl:otherwise>
						<xsl:text>false</xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="RepairColor">
		<xsl:param name="color"/>
		<xsl:variable name="isHex">
			<xsl:call-template name="IsHexNumber">
				<xsl:with-param name="str" select="$color"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="(string-length($color) = 1) and ($isHex = 'true')">
				<xsl:value-of select="concat('#00', $color)"/>
			</xsl:when>
			<xsl:when test="(string-length($color) = 2) and ($isHex = 'true')">
				<xsl:value-of select="concat('#0', $color)"/>
			</xsl:when>
			<xsl:when test="(string-length($color) = 3) and ($isHex = 'true')">
				<xsl:value-of select="concat('#', $color)"/>
			</xsl:when>
			<xsl:when test="(string-length($color) = 6) and ($isHex = 'true')">
				<xsl:value-of select="concat('#', $color)"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$color"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="InsertColor">
		<xsl:param name="color"/>
		<xsl:variable name="repaired">
			<xsl:call-template name="RepairColor">
				<xsl:with-param name="color" select="$color"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:choose>
			<!-- Color contains more then the hex value -->
			<xsl:when test="contains($repaired, ' ')">
				<xsl:call-template name="InsertColor">
					<xsl:with-param name="repaired" select="substring-before($repaired,' ')"/>
				</xsl:call-template>
			</xsl:when>
			<!-- Color is a 6-digit hex color (#003564) -->
			<xsl:when test="contains($repaired,'#') and string-length($repaired) > 4">
				<xsl:value-of select="$repaired"/>
			</xsl:when>
			<!-- Color is a 3-digit hex color (#036) -->
			<xsl:when test="contains($repaired,'#') and string-length($repaired) = 4">
				<xsl:variable name="d1" select="substring($repaired, 2, 1)"/>
				<xsl:variable name="d2" select="substring($repaired, 3, 1)"/>
				<xsl:variable name="d3" select="substring($repaired, 4, 1)"/>
				<xsl:value-of  select="concat('#', $d1, $d1, $d2, $d2, $d3, $d3)"/>
			</xsl:when>
			<!-- Color is a standard color -->
			<xsl:otherwise>
				<!--TODO standard colors mapping (there are 10 standard colors in Word)-->
				<xsl:choose>
					<xsl:when test="$repaired = 'aqua' or contains($repaired,'aqua')">
						<xsl:text>#00ffff</xsl:text>
					</xsl:when>
					<xsl:when test="$repaired = 'cyan' or contains($repaired,'cyan')">
						<xsl:text>#00ffff</xsl:text>
					</xsl:when>
					<xsl:when test="$repaired = 'black' or contains($repaired,'black')">
						<xsl:text>#000000</xsl:text>
					</xsl:when>
					<xsl:when test="$repaired = 'blue' or contains($repaired,'blue')">
						<xsl:text>#0000ff</xsl:text>
					</xsl:when>
					<xsl:when test="$repaired = 'fuchsia' or contains($repaired,'fuchsia')">
						<xsl:text>#ff00ff</xsl:text>
					</xsl:when>
					<xsl:when test="$repaired = 'gray' or contains($repaired,'gray')">
						<xsl:text>#808080</xsl:text>
					</xsl:when>
					<xsl:when test="$repaired = 'green' or contains($repaired,'green')">
						<xsl:text>#008000</xsl:text>
					</xsl:when>
					<xsl:when test="$repaired = 'lime' or contains($repaired,'lime')">
						<xsl:text>#00ff00</xsl:text>
					</xsl:when>
					<xsl:when test="$repaired = 'maroon' or contains($repaired,'maroon')">
						<xsl:text>#800000</xsl:text>
					</xsl:when>
					<xsl:when test="$repaired = 'navy' or contains($repaired,'navy')">
						<xsl:text>#000080</xsl:text>
					</xsl:when>
					<xsl:when test="$repaired = 'olive' or contains($repaired,'olive')">
						<xsl:text>#808000</xsl:text>
					</xsl:when>
					<xsl:when test="$repaired = 'purple' or contains($repaired,'purple')">
						<xsl:text>#800080</xsl:text>
					</xsl:when>
					<xsl:when test="$repaired = 'red' or contains($repaired,'red')">
						<xsl:text>#ff0000</xsl:text>
					</xsl:when>
					<xsl:when test="$repaired = 'silver' or contains($repaired,'silver')">
						<xsl:text>#c0c0c0</xsl:text>
					</xsl:when>
					<xsl:when test="$repaired = 'teal' or contains($repaired,'teal')">
						<xsl:text>#008080</xsl:text>
					</xsl:when>
					<xsl:when test="$repaired = 'white' or contains($repaired,'white')">
						<xsl:text>#ffffff</xsl:text>
					</xsl:when>
					<xsl:when test="$repaired = 'yellow' or contains($repaired,'yellow')">
						<xsl:text>#ffff00</xsl:text>
					</xsl:when>
					<xsl:otherwise>
						<xsl:text>#000000</xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!-- 
  Summary:  Compute shading : call this template int the context of w:shd 
  Author:   CleverAge
  Modified: makz (DIaLOGIKa)
  -->
  <xsl:template name="ComputeShading">
    <xsl:param name="shd" select="."/>
    <xsl:variable name="_fg">
      <xsl:call-template name="InsertColor">
        <xsl:with-param name="color" select="$shd/@w:color"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="_bg">
      <xsl:call-template name="InsertColor">
        <xsl:with-param name="color" select="$shd/@w:fill"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="fg">
      <xsl:choose>
        <xsl:when test="substring($_fg, 1, 1)='#'">
          <xsl:value-of select="substring-after($_fg, '#' )"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$_fg"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="bg">
      <xsl:choose>
        <xsl:when test="substring($_bg, 1, 1)='#'">
          <xsl:value-of select="substring-after($_bg, '#' )"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$_bg"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:choose>
      <!-- clear pattern -->
      <xsl:when test="$shd/@w:val = 'clear' ">
        <xsl:choose>
          <xsl:when test="$shd/@w:fill != 'auto' ">
            <xsl:value-of select="concat('#', $bg)"/>
          </xsl:when>
          <xsl:otherwise><!--#ffffff--></xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <!-- solid pattern -->
      <xsl:when test="$shd/@w:val = 'solid'">
        <xsl:choose>
          <xsl:when test="$shd/@w:color = 'auto' and $shd/@w:fill = 'auto'">
            <xsl:text>#000000</xsl:text>
          </xsl:when>
          <xsl:when test="$shd/@w:color != 'auto'">
            <xsl:value-of select="$_fg"/>
            <!--<xsl:value-of select="concat('#', $shd/@w:color)"/>-->
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>#ffffff</xsl:text>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <!-- percentage pattern -->
      <xsl:when test="starts-with($shd/@w:val, 'pct')">
        <xsl:variable name="pct" select="number(substring-after($shd/@w:val, 'pct'))"/>
        <xsl:choose>
          <xsl:when test="$shd/@w:color= 'auto' and $shd/@w:fill = 'auto'">
            <xsl:call-template name="AlphaBlend">
              <xsl:with-param name="pct" select="$pct"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:when test="$shd/@w:color != 'auto' and $shd/@w:fill = 'auto'">
            <xsl:call-template name="AlphaBlend">
              <xsl:with-param name="fg" select="$fg"/>
              <xsl:with-param name="pct" select="$pct"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:when test="$shd/@w:color != 'auto' and $shd/@w:fill != 'auto'">
            <xsl:call-template name="AlphaBlend">
              <xsl:with-param name="fg" select="$fg"/>
              <xsl:with-param name="bg" select="$bg"/>
              <xsl:with-param name="pct" select="$pct"/>
            </xsl:call-template>
          </xsl:when>
        </xsl:choose>
      </xsl:when>
      <!-- unmanaged patterns : 0% blending -->
      <xsl:otherwise>
        <xsl:choose>
          <xsl:when test="$shd/@w:color= 'auto' and $shd/@w:fill = 'auto'">
            <xsl:call-template name="AlphaBlend"/>
          </xsl:when>
          <xsl:when test="$shd/@w:color = 'auto' and $shd/@w:fill != 'auto'">
            <xsl:call-template name="AlphaBlend">
              <xsl:with-param name="bg" select="$bg"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:when test="$shd/@w:color != 'auto' and $shd/@w:fill = 'auto'">
            <xsl:call-template name="AlphaBlend">
              <xsl:with-param name="fg" select="$fg"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:when test="$shd/@w:color != 'auto' and $shd/@w:fill != 'auto'">
            <xsl:call-template name="AlphaBlend">
              <xsl:with-param name="fg" select="$fg"/>
              <xsl:with-param name="bg" select="$bg"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>#ffffff</xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

	<!-- Alpha blend fg and bg hexadecimal colors -->
	<xsl:template name="AlphaBlend">
		<xsl:param name="fg" select="'000000'"/>
		<xsl:param name="bg" select="'FFFFFF'"/>
		<xsl:param name="pct" select="0"/>

		<xsl:variable name="alpha" select="floor($pct * 255 div 100)"/>

		<xsl:variable name="RfgDec">
			<xsl:call-template name="HexToDec">
				<xsl:with-param name="number" select="substring($fg, 1, 2)"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="GfgDec">
			<xsl:call-template name="HexToDec">
				<xsl:with-param name="number" select="substring($fg, 3, 2)"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="BfgDec">
			<xsl:call-template name="HexToDec">
				<xsl:with-param name="number" select="substring($fg, 5, 2)"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="RbgDec">
			<xsl:call-template name="HexToDec">
				<xsl:with-param name="number" select="substring($bg, 1, 2)"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="GbgDec">
			<xsl:call-template name="HexToDec">
				<xsl:with-param name="number" select="substring($bg, 3, 2)"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="BbgDec">
			<xsl:call-template name="HexToDec">
				<xsl:with-param name="number" select="substring($bg, 5, 2)"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="RHex">
			<xsl:call-template name="DecToHex">
				<xsl:with-param name="number">
					<xsl:call-template name="AlphaBlendChannel">
						<xsl:with-param name="fg" select="$RfgDec"/>
						<xsl:with-param name="bg" select="$RbgDec"/>
						<xsl:with-param name="alpha" select="$alpha"/>
					</xsl:call-template>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="GHex">
			<xsl:call-template name="DecToHex">
				<xsl:with-param name="number">
					<xsl:call-template name="AlphaBlendChannel">
						<xsl:with-param name="fg" select="$GfgDec"/>
						<xsl:with-param name="bg" select="$GbgDec"/>
						<xsl:with-param name="alpha" select="$alpha"/>
					</xsl:call-template>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="BHex">
			<xsl:call-template name="DecToHex">
				<xsl:with-param name="number">
					<xsl:call-template name="AlphaBlendChannel">
						<xsl:with-param name="fg" select="$BfgDec"/>
						<xsl:with-param name="bg" select="$BbgDec"/>
						<xsl:with-param name="alpha" select="$alpha"/>
					</xsl:call-template>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:variable>

		<xsl:value-of select="concat('#', concat($RHex, concat($GHex, $BHex)))"/>

	</xsl:template>

	<xsl:template name="AlphaBlendChannel">
		<xsl:param name="fg" />
		<xsl:param name="bg"/>
		<xsl:param name="alpha"/>
		<xsl:value-of select="(number($fg * $alpha) + number($bg * (255 - $alpha) )) div 255"/>
	</xsl:template>


</xsl:stylesheet>
