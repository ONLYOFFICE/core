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
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0" exclude-result-prefixes="svg">

  <xsl:template name="OddHeaderFooter">
    <xsl:param name="masterPage"/>
    
    <xsl:for-each
      select="office:master-styles/style:master-page[@style:name = $masterPage]/style:header">
      <xsl:if test="not(@style:display = 'false' )">
		  <!--xml:space="preserve"-->
		  <oddHeader ><xsl:call-template name="InsertHeaderFooterContent"/></oddHeader>
      </xsl:if>
    </xsl:for-each>

    <xsl:for-each
      select="office:master-styles/style:master-page[@style:name = $masterPage]/style:footer">
      <xsl:if test="not(@style:display = 'false' )">
		  <!--xml:space="preserve"-->
		  <oddFooter><xsl:call-template name="InsertHeaderFooterContent"/></oddFooter>
      </xsl:if>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="EvenHeaderFooter">
    <xsl:param name="masterPage"/>
    
    <!-- if there is even header or footer -->
    <xsl:if
      test="office:master-styles/style:master-page[@style:name = $masterPage]/style:header-left[not(@style:display = 'false' )] or office:master-styles/style:master-page[@style:name = $masterPage]/style:footer-left[not(@style:display = 'false' )]">

      <!-- print even header -->
      <xsl:choose>
        <!-- if even header has to be printed then print it -->
        <xsl:when
          test="office:master-styles/style:master-page[@style:name = $masterPage]/style:header-left[not(@style:display = 'false' )]">
          <xsl:for-each
            select="office:master-styles/style:master-page[@style:name = $masterPage]/style:header-left">
			  <!--xml:space="preserve"-->
			  <evenHeader><xsl:call-template name="InsertHeaderFooterContent"/></evenHeader>
          </xsl:for-each>
        </xsl:when>
        <!-- if not then if odd header has to be printed - print it also for even pages -->
        <xsl:when
          test="office:master-styles/style:master-page[@style:name = $masterPage]/style:header[not(@style:display = 'false' )] and office:master-styles/style:master-page[@style:name = $masterPage]/style:footer-left[not(@style:display = 'false' )]">
          <xsl:for-each
            select="office:master-styles/style:master-page[@style:name = $masterPage]/style:header">
			  <!--xml:space="preserve"-->
			  <evenHeader><xsl:call-template name="InsertHeaderFooterContent"/></evenHeader>
          </xsl:for-each>
        </xsl:when>
      </xsl:choose>

      <!-- print even footer -->
      <xsl:choose>
        <!-- if even footer has to be printed then print it -->
        <xsl:when
          test="office:master-styles/style:master-page[@style:name = $masterPage]/style:footer-left[not(@style:display = 'false' )]">
          <xsl:for-each
            select="office:master-styles/style:master-page[@style:name = $masterPage]/style:footer-left">
			  <!--xml:space="preserve"-->
			  <evenFooter><xsl:call-template name="InsertHeaderFooterContent"/></evenFooter>
          </xsl:for-each>
        </xsl:when>
        <!-- if not then if odd footer has to be printed - print it also for even pages -->
        <xsl:when
          test="office:master-styles/style:master-page[@style:name = $masterPage]/style:footer[not(@style:display = 'false' )] and office:master-styles/style:master-page[@style:name = $masterPage]/style:header-left[not(@style:display = 'false' )]">
          <xsl:for-each
            select="office:master-styles/style:master-page[@style:name = $masterPage]/style:footer">
			  <!--xml:space="preserve"-->
			  <evenFooter><xsl:call-template name="InsertHeaderFooterContent"/></evenFooter>
          </xsl:for-each>
        </xsl:when>
      </xsl:choose>
    </xsl:if>
  </xsl:template>

  <!-- TO DO pictures in header and footer -->
  <xsl:template name="InsertHeaderFooterContent">
    <xsl:if test="style:region-left">
      <xsl:text>&amp;L</xsl:text>
      <xsl:for-each select="style:region-left/text:p">
        <xsl:if test="position() &gt; 1">
          <xsl:value-of select="'&#xD;&#xA;'"/>
        </xsl:if>
        <xsl:apply-templates mode="pageTag"/>
      </xsl:for-each>
    </xsl:if>

    <!-- when there is only center header than content is right after style:header -->
    <xsl:if test="style:region-center or text:p">
      <xsl:text>&amp;C</xsl:text>
      <xsl:choose>
        <xsl:when test="style:region-center ">
          <xsl:for-each select="style:region-center/text:p">
            <xsl:if test="position() &gt; 1">
              <xsl:value-of select="'&#xD;&#xA;'"/>
            </xsl:if>
            <xsl:apply-templates mode="pageTag"/>
          </xsl:for-each>
        </xsl:when>
        <xsl:when test="text:p">
          <xsl:for-each select="text:p">
            <xsl:if test="position() &gt; 1">
              <xsl:value-of select="'&#xD;&#xA;'"/>
            </xsl:if>
            <xsl:apply-templates mode="pageTag"/>
          </xsl:for-each>
        </xsl:when>
      </xsl:choose>
    </xsl:if>

    <xsl:if test="style:region-right">
      <xsl:text>&amp;R</xsl:text>
      <xsl:for-each select="style:region-right/text:p">
        <xsl:if test="position() &gt; 1">
          <xsl:value-of select="'&#xD;&#xA;'"/>
        </xsl:if>
        <xsl:apply-templates mode="pageTag"/>
      </xsl:for-each>
    </xsl:if>
  </xsl:template>

  <xsl:template match="text()[parent::text:p]" mode="pageTag">
    <xsl:text>&amp;"Arial"&amp;10</xsl:text>
    <xsl:value-of select="."/>
  </xsl:template>

  <xsl:template match="text:span" mode="pageTag">

    <xsl:variable name="style">
      <xsl:value-of select="@text:style-name"/>
    </xsl:variable>

    <xsl:choose>
      <!-- if this span has the same style as previous then don't duplicate tags -->
      <xsl:when
        test="preceding-sibling::node()[1][name() = 'text:span' and @text:style-name = $style]"/>
      <xsl:otherwise>
        <xsl:for-each select="key('style',$style)/style:text-properties">
          <xsl:if test="@style:font-name or @fo:font-style or @fo:font-weight">
            <xsl:text>&amp;"</xsl:text>

            <xsl:choose>
              <xsl:when test="@style:font-name">
                <xsl:value-of
                  select="translate(key('font',@style:font-name)/@svg:font-family,&quot;&apos;&quot;,&quot;&quot;)"
                />
              </xsl:when>
              <xsl:otherwise>
                <xsl:text>Arial</xsl:text>
              </xsl:otherwise>
            </xsl:choose>

            <xsl:if test="@fo:font-style='italic' or @fo:font-weight='bold' ">
              <xsl:text>,</xsl:text>
            </xsl:if>

            <xsl:if test="@fo:font-weight = 'bold' ">
              <xsl:text>Bold</xsl:text>
              <xsl:if test="@fo:font-style">
                <xsl:text> </xsl:text>
              </xsl:if>
            </xsl:if>

            <xsl:if test="@fo:font-style = 'italic' ">
              <xsl:text>Italic</xsl:text>
            </xsl:if>

            <xsl:text>"</xsl:text>
          </xsl:if>

          <xsl:if test="@fo:font-size">
            <xsl:text>&amp;</xsl:text>
            <xsl:value-of select="substring-before(@fo:font-size,'pt')"/>
          </xsl:if>

          <xsl:choose>
            <xsl:when test="@fo:color">
              <xsl:text>&amp;</xsl:text>
              <xsl:value-of select="translate(@fo:color,'#abcdef','KABCDEF')"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>&amp;K000000</xsl:text>
            </xsl:otherwise>
          </xsl:choose>


          <xsl:if test="@style:text-underline-style and @style:text-underline-style != 'none' ">
            <xsl:choose>
              <xsl:when test="@style:text-underline-type = 'double' ">
                <xsl:text>&amp;E</xsl:text>
              </xsl:when>
              <xsl:otherwise>
                <xsl:text>&amp;U</xsl:text>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:if>

          <xsl:if
            test="@style:text-line-through-style and @style:text-line-through-style != 'none' ">
            <xsl:text>&amp;S</xsl:text>
          </xsl:if>

          <xsl:if test="@style:text-position">
            <xsl:choose>
              <xsl:when test="contains(@style:text-position,'super')">
                <xsl:text>&amp;X</xsl:text>
              </xsl:when>
              <xsl:when test="contains(@style:text-position,'sub')">
                <xsl:text>&amp;Y</xsl:text>
              </xsl:when>
            </xsl:choose>
          </xsl:if>
        </xsl:for-each>
      </xsl:otherwise>
    </xsl:choose>

    <xsl:apply-templates mode="pageTag"/>

    <!-- if next text:span has different style switch-off formatings which require switching-off (underline, subscript, superscript)-->
    <xsl:if
      test="not(following-sibling::node()[1][name() = 'text:span' and @text:style-name = $style])">
      <xsl:for-each select="key('style',$style)/style:text-properties">
        <xsl:if test="@style:text-underline-style and @style:text-underline-style != 'none' ">
          <xsl:choose>
            <xsl:when test="@style:text-underline-type = 'double' ">
              <xsl:text>&amp;E</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>&amp;U</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:if>

        <xsl:if test="@style:text-line-through-style and @style:text-line-through-style != 'none' ">
          <xsl:text>&amp;S</xsl:text>
        </xsl:if>

        <xsl:if test="@style:text-position">
          <xsl:choose>
            <xsl:when test="contains(@style:text-position,'super')">
              <xsl:text>&amp;X</xsl:text>
            </xsl:when>
            <xsl:when test="contains(@style:text-position,'sub')">
              <xsl:text>&amp;Y</xsl:text>
            </xsl:when>
          </xsl:choose>
        </xsl:if>

      </xsl:for-each>
    </xsl:if>
  </xsl:template>

  <xsl:template match="text:title" mode="pageTag">
    <xsl:text>&amp;F</xsl:text>
  </xsl:template>

  <xsl:template match="text:sheet-name" mode="pageTag">
    <xsl:text>&amp;A</xsl:text>
  </xsl:template>

  <xsl:template match="text:page-number" mode="pageTag">
    <xsl:text>&amp;P</xsl:text>
  </xsl:template>

  <xsl:template match="text:page-count" mode="pageTag">
    <xsl:text>&amp;N</xsl:text>
  </xsl:template>

  <xsl:template match="text:date" mode="pageTag">
    <xsl:text>&amp;D</xsl:text>
  </xsl:template>

  <xsl:template match="text:time" mode="pageTag">
    <xsl:text>&amp;T</xsl:text>
  </xsl:template>
    
  <xsl:template match="text:file-name" mode="pageTag">
    <xsl:text>file:///&amp;Z&amp;F</xsl:text>
  </xsl:template>

</xsl:stylesheet>
