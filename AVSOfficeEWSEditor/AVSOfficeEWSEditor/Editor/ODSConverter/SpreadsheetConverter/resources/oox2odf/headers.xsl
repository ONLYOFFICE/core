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
  xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main"
  xmlns:e="http://schemas.openxmlformats.org/spreadsheetml/2006/main"
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0" exclude-result-prefixes="a e">

  <!--<xsl:import href="common.xsl"/>-->

  <xsl:template name="InsertHeaderFooterStyles">
    <xsl:param name="activeTab"/>

    <xsl:variable name="defaultFontFace">
      <xsl:value-of select="key('Font', '0')/e:name/@val"/>
    </xsl:variable>

    <xsl:variable name="defaultFontSize">
      <xsl:value-of select="key('Font', '0')/e:sz/@val"/>
    </xsl:variable>

    <xsl:for-each
      select="key('Part', concat('xl/worksheets/sheet', $activeTab + 1,'.xml'))/e:worksheet/e:headerFooter">
      <!-- odd header styles -->
      <xsl:for-each select="e:oddHeader">
        <xsl:call-template name="GetHeaderFooterStyles">
          <xsl:with-param name="fontFace" select="$defaultFontFace"/>
          <xsl:with-param name="fontSize" select="$defaultFontSize"/>
        </xsl:call-template>
      </xsl:for-each>

      <!-- even header styles -->
      <xsl:for-each select="e:evenHeader">
        <xsl:call-template name="GetHeaderFooterStyles">
          <xsl:with-param name="fontFace" select="$defaultFontFace"/>
          <xsl:with-param name="fontSize" select="$defaultFontSize"/>
        </xsl:call-template>
      </xsl:for-each>

      <!-- odd footer styles -->
      <xsl:for-each select="e:oddFooter">
        <xsl:call-template name="GetHeaderFooterStyles">
          <xsl:with-param name="fontFace" select="$defaultFontFace"/>
          <xsl:with-param name="fontSize" select="$defaultFontSize"/>
        </xsl:call-template>
      </xsl:for-each>

      <!-- even footer styles -->
      <xsl:for-each select="e:evenFooter">
        <xsl:call-template name="GetHeaderFooterStyles">
          <xsl:with-param name="fontFace" select="$defaultFontFace"/>
          <xsl:with-param name="fontSize" select="$defaultFontSize"/>
        </xsl:call-template>
      </xsl:for-each>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="GetHeaderFooterStyles">
    <xsl:param name="region" select=" 'C' "/>
    <xsl:param name="paragraph" select="1"/>
    <xsl:param name="tags" select="text()"/>
    <xsl:param name="styleNum" select="1"/>
    <xsl:param name="changedStyle" select="1"/>

    <xsl:param name="fontFace"/>
    <xsl:param name="fontSize"/>
    <xsl:param name="fontColor" select=" 'K000000' "/>
    <xsl:param name="underline" select=" 'none' "/>
    <xsl:param name="strikethrough" select="0"/>
    <!-- superscript/subscript -->
    <xsl:param name="position" select=" 'normal' "/>

    <xsl:choose>
      <xsl:when test="substring-before($tags,'&amp;') = '' ">
        <xsl:choose>
          <!-- skip region tags -->
          <xsl:when
            test="(contains($tags,'&amp;L' ) and substring-before($tags,'&amp;L' ) = '' ) or  (contains($tags,'&amp;C' ) and substring-before($tags,'&amp;C' ) = '' ) or
            (contains($tags,'&amp;R' ) and substring-before($tags,'&amp;R' ) = '' )">

            <xsl:call-template name="GetHeaderFooterStyles">
              <xsl:with-param name="region" select="substring($tags,2,1)"/>
              <xsl:with-param name="paragraph" select="1"/>
              <xsl:with-param name="tags" select="substring($tags,3)"/>
              <xsl:with-param name="styleNum" select="1"/>
              <xsl:with-param name="changedStyle" select="1"/>
              <!-- restore default properties -->
              <xsl:with-param name="fontFace">
                <xsl:value-of select="key('Font', '0')/e:name/@val"/>
              </xsl:with-param>
              <xsl:with-param name="fontSize">
                <xsl:value-of select="key('Font', '0')/e:sz/@val"/>
              </xsl:with-param>

              <xsl:with-param name="underline" select=" 'none' "/>
              <xsl:with-param name="strikethrough" select="0"/>
              <xsl:with-param name="position" select=" 'normal' "/>
            </xsl:call-template>
          </xsl:when>
          <!-- skip picture and file path tags -->
          <xsl:when
            test="(contains($tags,'&amp;G' ) and substring-before($tags,'&amp;G' ) = '') or  (contains($tags,'&amp;Z' ) and substring-before($tags,'&amp;Z' ) = '')">

            <xsl:call-template name="GetHeaderFooterStyles">
              <xsl:with-param name="tags" select="substring($tags,substring($tags,3))"/>

              <xsl:with-param name="region" select="$region"/>
              <xsl:with-param name="paragraph" select="$paragraph"/>
              <xsl:with-param name="styleNum" select="$styleNum"/>
              <xsl:with-param name="fontSize" select="$fontSize"/>
              <xsl:with-param name="fontFace" select="$fontFace"/>
              <xsl:with-param name="fontColor" select="$fontColor"/>
              <xsl:with-param name="underline" select="$underline"/>
              <xsl:with-param name="strikethrough" select="$strikethrough"/>
              <xsl:with-param name="position" select="$position"/>
            </xsl:call-template>
          </xsl:when>
          <!-- on other field tags output current style -->
          <xsl:when
            test="(contains($tags,'&amp;P' ) and substring-before($tags,'&amp;P' ) = '' ) or  (contains($tags,'&amp;N' ) and substring-before($tags,'&amp;N' ) = '' ) or
            (contains($tags,'&amp;D' ) and substring-before($tags,'&amp;D' ) = '' ) or (contains($tags,'&amp;T' ) and substring-before($tags,'&amp;T' ) = '' ) or
            (contains($tags,'&amp;F' ) and substring-before($tags,'&amp;F' ) = '' ) or (contains($tags,'&amp;A' ) and substring-before($tags,'&amp;A' ) = '' )">

            <xsl:if test="$changedStyle">
              <xsl:call-template name="InsertHeaderFooterStyle">
                <xsl:with-param name="region" select="$region"/>
                <xsl:with-param name="paragraph" select="$paragraph"/>
                <xsl:with-param name="tags" select="$tags"/>
                <xsl:with-param name="styleNum" select="$styleNum"/>
                <xsl:with-param name="fontFace" select="$fontFace"/>
                <xsl:with-param name="fontSize" select="$fontSize"/>
                <xsl:with-param name="fontColor" select="$fontColor"/>
                <xsl:with-param name="underline" select="$underline"/>
                <xsl:with-param name="strikethrough" select="$strikethrough"/>
                <xsl:with-param name="position" select="$position"/>
              </xsl:call-template>
            </xsl:if>

            <xsl:call-template name="GetHeaderFooterStyles">
              <xsl:with-param name="tags" select="substring($tags,3)"/>
              <xsl:with-param name="styleNum" select="$styleNum + $changedStyle"/>
              <xsl:with-param name="changedStyle" select="0"/>

              <xsl:with-param name="region" select="$region"/>
              <xsl:with-param name="paragraph" select="$paragraph"/>
              <xsl:with-param name="fontFace" select="$fontFace"/>
              <xsl:with-param name="fontSize" select="$fontSize"/>
              <xsl:with-param name="fontColor" select="$fontColor"/>
              <xsl:with-param name="underline" select="$underline"/>
              <xsl:with-param name="strikethrough" select="$strikethrough"/>
              <xsl:with-param name="position" select="$position"/>
            </xsl:call-template>
          </xsl:when>
          <!-- font style -->
          <xsl:when
            test="contains($tags,'&amp;&quot;' ) and substring-before($tags,'&amp;&quot;' ) = '' ">
            <xsl:variable name="fontTag">
              <xsl:value-of
                select="substring-before(substring-after($tags,'&quot;' ),'&quot;' )"/>
            </xsl:variable>

            <xsl:variable name="font">
              <xsl:choose>
                <xsl:when test="contains($fontTag,',' )">
                  <xsl:value-of select="substring-before($fontTag,',' )"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="$fontTag"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>

            <xsl:variable name="newFont">
              <xsl:choose>
                <xsl:when test="$font = '-' ">
                  <xsl:text>Calibri</xsl:text>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="$font"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>

            <xsl:call-template name="GetHeaderFooterStyles">
              <xsl:with-param name="tags"
                select="substring-after(substring-after($tags,'&amp;&quot;' ),'&quot;')"/>
              <xsl:with-param name="fontFace" select="$newFont"/>
              <xsl:with-param name="changedStyle" select="1"/>

              <xsl:with-param name="region" select="$region"/>
              <xsl:with-param name="paragraph" select="$paragraph"/>
              <xsl:with-param name="styleNum" select="$styleNum"/>
              <xsl:with-param name="fontSize" select="$fontSize"/>
              <xsl:with-param name="fontColor" select="$fontColor"/>
              <xsl:with-param name="underline" select="$underline"/>
              <xsl:with-param name="strikethrough" select="$strikethrough"/>
              <xsl:with-param name="position" select="$position"/>
            </xsl:call-template>
          </xsl:when>
          <!-- font size -->
          <xsl:when
            test="number(substring($tags,2,1)) and contains($tags,'&amp;' ) and substring-before($tags,'&amp;' ) = '' ">

            <xsl:variable name="size">
              <xsl:call-template name="ReadFontSizeTag">
                <xsl:with-param name="tags" select="substring($tags,2)"/>
              </xsl:call-template>
            </xsl:variable>

            <xsl:call-template name="GetHeaderFooterStyles">
              <!-- +2 = &amp; + next character -->
              <xsl:with-param name="tags" select="substring($tags,string-length($size) + 2)"/>
              <xsl:with-param name="fontSize" select="$size"/>
              <xsl:with-param name="changedStyle" select="1"/>

              <xsl:with-param name="region" select="$region"/>
              <xsl:with-param name="paragraph" select="$paragraph"/>
              <xsl:with-param name="styleNum" select="$styleNum"/>
              <xsl:with-param name="fontFace" select="$fontFace"/>
              <xsl:with-param name="fontColor" select="$fontColor"/>
              <xsl:with-param name="underline" select="$underline"/>
              <xsl:with-param name="strikethrough" select="$strikethrough"/>
              <xsl:with-param name="position" select="$position"/>
            </xsl:call-template>
          </xsl:when>
          <!-- font color -->
          <xsl:when
            test="contains($tags,'&amp;K' ) and substring-before($tags,'&amp;K' ) = '' ">

            <xsl:variable name="value">
              <xsl:value-of select="substring($tags,3,6)"/>
            </xsl:variable>

            <xsl:variable name="newFontColor">
              <xsl:choose>
                <!-- when color is specified with theme color ID and tint value -->
                <xsl:when test="contains($value,'-') or contains($value,'+')">
                  <xsl:variable name="colorId">
                    <xsl:value-of select="number(substring($value,1,2))"/>
                  </xsl:variable>

                  <xsl:variable name="theme">
                    <xsl:choose>
                      <xsl:when test="$colorId = 0">
                        <xsl:text>1</xsl:text>
                      </xsl:when>
                      <xsl:when test="$colorId = 1">
                        <xsl:text>0</xsl:text>
                      </xsl:when>
                      <xsl:when test="$colorId = 2">
                        <xsl:text>3</xsl:text>
                      </xsl:when>
                      <xsl:when test="$colorId = 3">
                        <xsl:text>2</xsl:text>
                      </xsl:when>
                      <xsl:otherwise>
                        <xsl:value-of select="$colorId"/>
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

                  <xsl:variable name="tint">
                    <xsl:choose>
                      <xsl:when test="substring($value,3,1) = '-' ">
                        <xsl:value-of
                          select="number(concat(substring($value,3,1),'.',substring($value,5)))"/>
                      </xsl:when>
                      <xsl:otherwise>
                        <xsl:value-of
                          select="number(concat(substring($value,4,1),'.',substring($value,5)))"/>
                      </xsl:otherwise>
                    </xsl:choose>

                  </xsl:variable>

                  <xsl:choose>
                    <xsl:when test="$tint != 0">
                      <xsl:text>K</xsl:text>
                      <xsl:call-template name="CalculateTintedColor">
                        <xsl:with-param name="color" select="$themeColor"/>
                        <xsl:with-param name="tint" select="$tint"/>
                      </xsl:call-template>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:value-of select="concat('K',$themeColor)"/>
                    </xsl:otherwise>
                  </xsl:choose>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="concat('K',$value)"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>

            <xsl:call-template name="GetHeaderFooterStyles">
              <xsl:with-param name="tags" select="substring($tags,9)"/>
              <xsl:with-param name="fontColor" select="$newFontColor"/>
              <xsl:with-param name="changedStyle" select="1"/>

              <xsl:with-param name="region" select="$region"/>
              <xsl:with-param name="paragraph" select="$paragraph"/>
              <xsl:with-param name="styleNum" select="$styleNum"/>
              <xsl:with-param name="fontSize" select="$fontSize"/>
              <xsl:with-param name="fontFace" select="$fontFace"/>
              <xsl:with-param name="underline" select="$underline"/>
              <xsl:with-param name="strikethrough" select="$strikethrough"/>
              <xsl:with-param name="position" select="$position"/>
            </xsl:call-template>
          </xsl:when>
          <!-- strikethrough -->
          <xsl:when
            test="contains($tags,'&amp;S' ) and substring-before($tags,'&amp;S' ) = '' ">

            <xsl:call-template name="GetHeaderFooterStyles">
              <xsl:with-param name="tags" select="substring($tags,3)"/>
              <xsl:with-param name="strikethrough" select="1 - $strikethrough"/>
              <xsl:with-param name="changedStyle" select="1"/>

              <xsl:with-param name="region" select="$region"/>
              <xsl:with-param name="paragraph" select="$paragraph"/>
              <xsl:with-param name="styleNum" select="$styleNum"/>
              <xsl:with-param name="fontSize" select="$fontSize"/>
              <xsl:with-param name="fontFace" select="$fontFace"/>
              <xsl:with-param name="fontColor" select="$fontColor"/>
              <xsl:with-param name="underline" select="$underline"/>
              <xsl:with-param name="position" select="$position"/>
            </xsl:call-template>
          </xsl:when>
          <!-- superscript -->
          <xsl:when
            test="contains($tags,'&amp;X' ) and substring-before($tags,'&amp;X' ) = '' ">

            <xsl:variable name="newPosition">
              <xsl:choose>
                <xsl:when test="$position = 'superscript' ">
                  <xsl:text>normal</xsl:text>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:text>superscript</xsl:text>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>

            <xsl:call-template name="GetHeaderFooterStyles">
              <xsl:with-param name="tags" select="substring($tags,3)"/>
              <xsl:with-param name="position" select="$newPosition"/>
              <xsl:with-param name="changedStyle" select="1"/>

              <xsl:with-param name="region" select="$region"/>
              <xsl:with-param name="paragraph" select="$paragraph"/>
              <xsl:with-param name="styleNum" select="$styleNum"/>
              <xsl:with-param name="fontSize" select="$fontSize"/>
              <xsl:with-param name="fontFace" select="$fontFace"/>
              <xsl:with-param name="fontColor" select="$fontColor"/>
              <xsl:with-param name="underline" select="$underline"/>
              <xsl:with-param name="strikethrough" select="$strikethrough"/>
            </xsl:call-template>
          </xsl:when>
          <!-- subscript -->
          <xsl:when
            test="contains($tags,'&amp;Y' ) and substring-before($tags,'&amp;Y' ) = '' ">

            <xsl:variable name="newPosition">
              <xsl:choose>
                <xsl:when test="$position = 'subscript' ">
                  <xsl:text>normal</xsl:text>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:text>subscript</xsl:text>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>

            <xsl:call-template name="GetHeaderFooterStyles">
              <xsl:with-param name="tags" select="substring($tags,3)"/>
              <xsl:with-param name="changedStyle" select="1"/>

              <xsl:with-param name="region" select="$region"/>
              <xsl:with-param name="paragraph" select="$paragraph"/>
              <xsl:with-param name="styleNum" select="$styleNum"/>
              <xsl:with-param name="fontSize" select="$fontSize"/>
              <xsl:with-param name="fontFace" select="$fontFace"/>
              <xsl:with-param name="fontColor" select="$fontColor"/>
              <xsl:with-param name="underline" select="$underline"/>
              <xsl:with-param name="strikethrough" select="$strikethrough"/>
              <xsl:with-param name="position" select="$position"/>
            </xsl:call-template>
          </xsl:when>
          <!-- single underline -->
          <xsl:when
            test="contains($tags,'&amp;U' ) and substring-before($tags,'&amp;U' ) = '' ">

            <xsl:variable name="newUnderline">
              <xsl:choose>
                <xsl:when test="$underline = 'single' ">
                  <xsl:text>normal</xsl:text>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:text>single</xsl:text>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>

            <xsl:call-template name="GetHeaderFooterStyles">
              <xsl:with-param name="tags" select="substring($tags,3)"/>
              <xsl:with-param name="underline" select="$newUnderline"/>
              <xsl:with-param name="changedStyle" select="1"/>

              <xsl:with-param name="region" select="$region"/>
              <xsl:with-param name="paragraph" select="$paragraph"/>
              <xsl:with-param name="styleNum" select="$styleNum"/>
              <xsl:with-param name="fontSize" select="$fontSize"/>
              <xsl:with-param name="fontFace" select="$fontFace"/>
              <xsl:with-param name="fontColor" select="$fontColor"/>
              <xsl:with-param name="strikethrough" select="$strikethrough"/>
              <xsl:with-param name="position" select="$position"/>
            </xsl:call-template>
          </xsl:when>
          <!-- double underline -->
          <xsl:when
            test="contains($tags,'&amp;E' ) and substring-before($tags,'&amp;E' ) = '' ">

            <xsl:variable name="newUnderline">
              <xsl:choose>
                <xsl:when test="$underline = 'double' ">
                  <xsl:text>normal</xsl:text>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:text>double</xsl:text>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>

            <xsl:call-template name="GetHeaderFooterStyles">
              <xsl:with-param name="tags" select="substring($tags,3)"/>
              <xsl:with-param name="underline" select="$newUnderline"/>
              <xsl:with-param name="changedStyle" select="1"/>

              <xsl:with-param name="region" select="$region"/>
              <xsl:with-param name="paragraph" select="$paragraph"/>
              <xsl:with-param name="styleNum" select="$styleNum"/>
              <xsl:with-param name="fontSize" select="$fontSize"/>
              <xsl:with-param name="fontFace" select="$fontFace"/>
              <xsl:with-param name="fontColor" select="$fontColor"/>
              <xsl:with-param name="strikethrough" select="$strikethrough"/>
              <xsl:with-param name="position" select="$position"/>
            </xsl:call-template>
          </xsl:when>
          <!-- skip other formating tags -->
          <xsl:when
            test="(contains($tags,'&amp;B' ) and substring-before($tags,'&amp;B' ) = '' ) or (contains($tags,'&amp;I' ) and substring-before($tags,'&amp;I' ) = '' ) or 
            (contains($tags,'&amp;O' ) and substring-before($tags,'&amp;O' ) = '' ) or (contains($tags,'&amp;H' ) and substring-before($tags,'&amp;H' ) = '' )">
            <xsl:call-template name="GetHeaderFooterStyles">
              <xsl:with-param name="tags" select="substring($tags,3)"/>
              <xsl:with-param name="changedStyle" select="1"/>

              <xsl:with-param name="region" select="$region"/>
              <xsl:with-param name="paragraph" select="$paragraph"/>
              <xsl:with-param name="styleNum" select="$styleNum"/>
              <xsl:with-param name="fontSize" select="$fontSize"/>
              <xsl:with-param name="fontFace" select="$fontFace"/>
              <xsl:with-param name="fontColor" select="$fontColor"/>
              <xsl:with-param name="underline" select="$underline"/>
              <xsl:with-param name="strikethrough" select="$strikethrough"/>
              <xsl:with-param name="position" select="$position"/>
            </xsl:call-template>
          </xsl:when>
          <!-- output current style on plain text at the end -->
          <xsl:otherwise>

            <xsl:if test="$tags != '' ">
              <xsl:if test="$changedStyle">
                <xsl:call-template name="InsertHeaderFooterStyle">
                  <xsl:with-param name="region" select="$region"/>
                  <xsl:with-param name="paragraph" select="$paragraph"/>
                  <xsl:with-param name="tags" select="$tags"/>
                  <xsl:with-param name="changedStyle" select="0"/>
                  <xsl:with-param name="styleNum" select="$styleNum"/>
                  <xsl:with-param name="fontFace" select="$fontFace"/>
                  <xsl:with-param name="fontSize" select="$fontSize"/>
                  <xsl:with-param name="fontColor" select="$fontColor"/>
                  <xsl:with-param name="underline" select="$underline"/>
                  <xsl:with-param name="strikethrough" select="$strikethrough"/>
                  <xsl:with-param name="position" select="$position"/>
                </xsl:call-template>
              </xsl:if>
            </xsl:if>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <!-- output current style on plain text before next &amp; -->
      <xsl:otherwise>
        
        <xsl:choose>
          <!-- new line-->
          <xsl:when test="substring($tags,1,1) = '&#xA;' ">
            <xsl:call-template name="GetHeaderFooterStyles">
              <xsl:with-param name="paragraph" select="$paragraph + 1"/>
              <xsl:with-param name="tags" select="substring($tags,2)"/>
              <xsl:with-param name="changedStyle" select="0"/>
              <xsl:with-param name="styleNum" select="1"/>

              <xsl:with-param name="region" select="$region"/>
              <xsl:with-param name="fontFace" select="$fontFace"/>
              <xsl:with-param name="fontSize" select="$fontSize"/>
              <xsl:with-param name="fontColor" select="$fontColor"/>
              <xsl:with-param name="underline" select="$underline"/>
              <xsl:with-param name="strikethrough" select="$strikethrough"/>
              <xsl:with-param name="position" select="$position"/>
            </xsl:call-template>
          </xsl:when>

          <xsl:otherwise>
            <xsl:variable name="stringLength">
              <xsl:value-of select="string-length(substring-before($tags,'&amp;'))"/>
            </xsl:variable>

            <xsl:if test="$changedStyle">
              <xsl:call-template name="InsertHeaderFooterStyle">
                <xsl:with-param name="region" select="$region"/>
                <xsl:with-param name="paragraph" select="$paragraph"/>
                <xsl:with-param name="tags" select="$tags"/>
                <xsl:with-param name="styleNum" select="$styleNum"/>
                <xsl:with-param name="fontFace" select="$fontFace"/>
                <xsl:with-param name="fontSize" select="$fontSize"/>
                <xsl:with-param name="fontColor" select="$fontColor"/>
                <xsl:with-param name="underline" select="$underline"/>
                <xsl:with-param name="strikethrough" select="$strikethrough"/>
                <xsl:with-param name="position" select="$position"/>
              </xsl:call-template>
            </xsl:if>
            
            <xsl:call-template name="GetHeaderFooterStyles">
              <xsl:with-param name="tags" select="substring($tags,$stringLength + 1)"/>
              <xsl:with-param name="changedStyle" select="0"/>
              <xsl:with-param name="styleNum" select="$styleNum + $changedStyle"/>

              <xsl:with-param name="region" select="$region"/>
              <xsl:with-param name="paragraph" select="$paragraph"/>
              <xsl:with-param name="fontFace" select="$fontFace"/>
              <xsl:with-param name="fontSize" select="$fontSize"/>
              <xsl:with-param name="fontColor" select="$fontColor"/>
              <xsl:with-param name="underline" select="$underline"/>
              <xsl:with-param name="strikethrough" select="$strikethrough"/>
              <xsl:with-param name="position" select="$position"/>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- processes tags defining fields (page number, sheet name etc.) and plain text -->
  <xsl:template name="GetHeaderFooterFields">
    <xsl:param name="region" select="'C' "/>
    <xsl:param name="paragraph"/>
    <xsl:param name="tags"/>
    <xsl:param name="changedStyle" select="1"/>
    <xsl:param name="styleNum" select="1"/>

    <xsl:choose>
      <xsl:when test="substring-before($tags,'&amp;') = '' or ( contains($tags,'file:///&amp;Z&amp;F' ) and substring-before($tags,'file:///&amp;Z&amp;F' ) = '' )">
        <xsl:choose>
          <!-- page number -->
          <xsl:when
            test="contains($tags,'&amp;P' ) and substring-before($tags,'&amp;P' ) = '' ">
            <text:span>
              <xsl:attribute name="text:style-name">
                <xsl:value-of
                  select="concat(generate-id(),'_',$region,'P',$paragraph,'-',$styleNum)"/>
              </xsl:attribute>
              <text:page-number>1</text:page-number>
            </text:span>

            <xsl:call-template name="GetHeaderFooterFields">
              <xsl:with-param name="tags" select="substring-after($tags,'&amp;P' )"/>
              <xsl:with-param name="styleNum" select="$styleNum + $changedStyle"/>
              <xsl:with-param name="changedStyle" select="0"/>

              <xsl:with-param name="region" select="$region"/>
              <xsl:with-param name="paragraph" select="$paragraph"/>
            </xsl:call-template>
          </xsl:when>

          <!-- total pages -->
          <xsl:when
            test="contains($tags,'&amp;N' ) and substring-before($tags,'&amp;N' ) = '' ">
            <text:span>
              <xsl:attribute name="text:style-name">
                <xsl:value-of
                  select="concat(generate-id(),'_',$region,'P',$paragraph,'-',$styleNum)"/>
              </xsl:attribute>
              <text:page-count>99</text:page-count>
            </text:span>

            <xsl:call-template name="GetHeaderFooterFields">
              <xsl:with-param name="tags" select="substring-after($tags,'&amp;N' )"/>
              <xsl:with-param name="styleNum" select="$styleNum + $changedStyle"/>
              <xsl:with-param name="changedStyle" select="0"/>

              <xsl:with-param name="region" select="$region"/>
              <xsl:with-param name="paragraph" select="$paragraph"/>
            </xsl:call-template>
          </xsl:when>

          <!-- date -->
          <xsl:when
            test="contains($tags,'&amp;D' ) and substring-before($tags,'&amp;D' ) = '' ">
            <text:span>
              <xsl:attribute name="text:style-name">
                <xsl:value-of
                  select="concat(generate-id(),'_',$region,'P',$paragraph,'-',$styleNum)"/>
              </xsl:attribute>
              <text:date style:data-style-name="N2" text:date-value="2007-04-07"
              >2007-04-01</text:date>
            </text:span>

            <xsl:call-template name="GetHeaderFooterFields">
              <xsl:with-param name="tags" select="substring-after($tags,'&amp;D' )"/>
              <xsl:with-param name="styleNum" select="$styleNum + $changedStyle"/>
              <xsl:with-param name="changedStyle" select="0"/>

              <xsl:with-param name="region" select="$region"/>
              <xsl:with-param name="paragraph" select="$paragraph"/>
            </xsl:call-template>
          </xsl:when>

          <!-- time -->
          <xsl:when
            test="contains($tags,'&amp;T' ) and substring-before($tags,'&amp;T' ) = '' ">
            <text:span>
              <xsl:attribute name="text:style-name">
                <xsl:value-of
                  select="concat(generate-id(),'_',$region,'P',$paragraph,'-',$styleNum)"/>
              </xsl:attribute>
              <text:time>12:21:24</text:time>
            </text:span>

            <xsl:call-template name="GetHeaderFooterFields">
              <xsl:with-param name="tags" select="substring-after($tags,'&amp;T' )"/>
              <xsl:with-param name="styleNum" select="$styleNum + $changedStyle"/>
              <xsl:with-param name="changedStyle" select="0"/>

              <xsl:with-param name="region" select="$region"/>
              <xsl:with-param name="paragraph" select="$paragraph"/>
            </xsl:call-template>
          </xsl:when>

          <!-- file path -->
          <xsl:when
            test="(contains($tags,'&amp;Z&amp;F' ) and substring-before($tags,'&amp;Z&amp;F' ) = '' ) or 
            (contains($tags,'file:///&amp;Z&amp;F' ) and substring-before($tags,'file:///&amp;Z&amp;F' ) = '' )">
            
            <text:span>
              <xsl:attribute name="text:style-name">
                <xsl:value-of
                  select="concat(generate-id(),'_',$region,'P',$paragraph,'-',$styleNum)"/>
              </xsl:attribute>
              <text:file-name text:display="full">???</text:file-name>
            </text:span>
            
            <xsl:call-template name="GetHeaderFooterFields">
              <xsl:with-param name="tags" select="substring-after($tags,'&amp;F' )"/>

              <xsl:with-param name="region" select="$region"/>
              <xsl:with-param name="paragraph" select="$paragraph"/>
              <xsl:with-param name="styleNum" select="$styleNum"/>
            </xsl:call-template>

          </xsl:when>

          <!-- file name -->
          <xsl:when
            test="contains($tags,'&amp;F' ) and substring-before($tags,'&amp;F' ) = '' ">
            <text:span>
              <xsl:attribute name="text:style-name">
                <xsl:value-of
                  select="concat(generate-id(),'_',$region,'P',$paragraph,'-',$styleNum)"/>
              </xsl:attribute>
              <text:title>???</text:title>
            </text:span>

            <xsl:call-template name="GetHeaderFooterFields">
              <xsl:with-param name="tags" select="substring-after($tags,'&amp;F' )"/>
              <xsl:with-param name="styleNum" select="$styleNum + $changedStyle"/>
              <xsl:with-param name="changedStyle" select="0"/>

              <xsl:with-param name="region" select="$region"/>
              <xsl:with-param name="paragraph" select="$paragraph"/>
            </xsl:call-template>
          </xsl:when>

          <!-- sheet name -->
          <xsl:when
            test="contains($tags,'&amp;A' ) and substring-before($tags,'&amp;A' ) = '' ">
            <text:span>
              <xsl:attribute name="text:style-name">
                <xsl:value-of
                  select="concat(generate-id(),'_',$region,'P',$paragraph,'-',$styleNum)"/>
              </xsl:attribute>
              <text:sheet-name>???</text:sheet-name>
            </text:span>

            <xsl:call-template name="GetHeaderFooterFields">
              <xsl:with-param name="tags" select="substring-after($tags,'&amp;A' )"/>
              <xsl:with-param name="styleNum" select="$styleNum + $changedStyle"/>
              <xsl:with-param name="changedStyle" select="0"/>

              <xsl:with-param name="region" select="$region"/>
              <xsl:with-param name="paragraph" select="$paragraph"/>
            </xsl:call-template>
          </xsl:when>

          <!-- skip picture -->
          <xsl:when
            test="contains($tags,'&amp;G' ) and substring-before($tags,'&amp;G' ) = '' ">
            <xsl:call-template name="GetHeaderFooterFields">
              <xsl:with-param name="tags" select="substring($tags,3)"/>

              <xsl:with-param name="region" select="$region"/>
              <xsl:with-param name="paragraph" select="$paragraph"/>
              <xsl:with-param name="styleNum" select="$styleNum"/>
            </xsl:call-template>
          </xsl:when>

          <!-- skip font style tag -->
          <xsl:when
            test="contains($tags,'&amp;&quot;' ) and substring-before($tags,'&amp;&quot;' ) = '' ">
            <xsl:call-template name="GetHeaderFooterFields">
              <xsl:with-param name="tags"
                select="substring-after(substring-after($tags,'&amp;&quot;' ),'&quot;')"/>
              <xsl:with-param name="changedStyle" select="1"/>

              <xsl:with-param name="region" select="$region"/>
              <xsl:with-param name="paragraph" select="$paragraph"/>
              <xsl:with-param name="styleNum" select="$styleNum"/>
            </xsl:call-template>
          </xsl:when>

          <!-- skip font size tag -->
          <xsl:when
            test="number(substring($tags,2,1)) and contains($tags,'&amp;' ) and substring-before($tags,'&amp;' ) = '' ">
            <xsl:call-template name="SkipFontSizeTag">
              <xsl:with-param name="tags" select="substring($tags,2)"/>
              <xsl:with-param name="changedStyle" select="1"/>

              <xsl:with-param name="region" select="$region"/>
              <xsl:with-param name="paragraph" select="$paragraph"/>
              <xsl:with-param name="styleNum" select="$styleNum"/>
            </xsl:call-template>
          </xsl:when>

          <!-- skip font color tag -->
          <xsl:when
            test="contains($tags,'&amp;K' ) and substring-before($tags,'&amp;K' ) = '' ">
            <xsl:call-template name="GetHeaderFooterFields">
              <xsl:with-param name="tags" select="substring($tags,9)"/>
              <xsl:with-param name="changedStyle" select="1"/>

              <xsl:with-param name="region" select="$region"/>
              <xsl:with-param name="paragraph" select="$paragraph"/>
              <xsl:with-param name="styleNum" select="$styleNum"/>
            </xsl:call-template>
          </xsl:when>

          <!-- skip other formating tags -->
          <xsl:when
            test="(contains($tags,'&amp;S' ) and substring-before($tags,'&amp;S' ) = '' ) or (contains($tags,'&amp;X' ) and substring-before($tags,'&amp;X' ) = '' ) or
            (contains($tags,'&amp;Y' ) and substring-before($tags,'&amp;Y' ) = '' ) or (contains($tags,'&amp;U' ) and substring-before($tags,'&amp;U' ) = '' ) or
            (contains($tags,'&amp;E' ) and substring-before($tags,'&amp;E' ) = '' ) or (contains($tags,'&amp;B' ) and substring-before($tags,'&amp;B' ) = '' ) or
            (contains($tags,'&amp;I' ) and substring-before($tags,'&amp;I' ) = '' ) or (contains($tags,'&amp;O' ) and substring-before($tags,'&amp;O' ) = '' ) or
            (contains($tags,'&amp;H' ) and substring-before($tags,'&amp;H' ) = '' ) or (contains($tags, '&amp;U'))">
            <xsl:call-template name="GetHeaderFooterFields">
              <xsl:with-param name="tags" select="substring($tags,3)"/>
              <xsl:with-param name="changedStyle" select="1"/>

              <xsl:with-param name="region" select="$region"/>
              <xsl:with-param name="paragraph" select="$paragraph"/>
              <xsl:with-param name="styleNum" select="$styleNum"/>
            </xsl:call-template>            
          </xsl:when>

          <!-- plain text at the end -->
          <xsl:otherwise>
            <xsl:if test="$tags != '' ">
              <text:span>
                <xsl:attribute name="text:style-name">
                  <xsl:value-of
                    select="concat(generate-id(),'_',$region,'P',$paragraph,'-',$styleNum)"/>
                </xsl:attribute>
                <xsl:value-of select="$tags"/>
              </text:span>
            </xsl:if>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>

      <!-- plain text before next &amp; -->
      <xsl:otherwise>
        <text:span>
          <xsl:attribute name="text:style-name">
            <xsl:value-of select="concat(generate-id(),'_',$region,'P',$paragraph,'-',$styleNum)"/>
          </xsl:attribute>
          <xsl:value-of select="substring-before($tags,'&amp;')"/>
        </text:span>

        <xsl:call-template name="GetHeaderFooterFields">
          <xsl:with-param name="tags"
            select="substring-after($tags,substring-before($tags,'&amp;'))"/>
          <xsl:with-param name="styleNum" select="$styleNum + $changedStyle"/>
          <xsl:with-param name="changedStyle" select="0"/>

          <xsl:with-param name="region" select="$region"/>
          <xsl:with-param name="paragraph" select="$paragraph"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="InsertHeaderFooter">
    <xsl:param name="activeTab"/>

    <xsl:for-each
      select="key('Part', concat('xl/worksheets/sheet', $activeTab + 1,'.xml'))/e:worksheet/e:headerFooter">

      <style:header>
        <xsl:for-each select="e:oddHeader">
          <xsl:call-template name="InsertRegions"/>
        </xsl:for-each>
      </style:header>

      <style:header-left>
        <xsl:choose>
          <xsl:when test="e:evenHeader and @differentOddEven = 1">
            <xsl:for-each select="e:evenHeader">
              <xsl:call-template name="InsertRegions"/>
            </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
            <xsl:attribute name="style:display">
              <xsl:text>false</xsl:text>
            </xsl:attribute>
          </xsl:otherwise>
        </xsl:choose>
      </style:header-left>

      <style:footer>
        <xsl:for-each select="e:oddFooter">
          <xsl:call-template name="InsertRegions"/>
        </xsl:for-each>
      </style:footer>

      <style:footer-left>
        <xsl:choose>
          <xsl:when test="e:evenFooter and @differentOddEven = 1">
            <xsl:for-each select="e:evenFooter">
              <xsl:call-template name="InsertRegions"/>
            </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
            <xsl:attribute name="style:display">
              <xsl:text>false</xsl:text>
            </xsl:attribute>
          </xsl:otherwise>
        </xsl:choose>
      </style:footer-left>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="InsertRegions">
    <!-- header/footer content string can start with out a region specification tag then it is treated as center region -->
    <xsl:variable name="start">
      <xsl:value-of select="substring(text(),1,2)"/>
    </xsl:variable>

    <xsl:if test="contains(text(),'&amp;L')">
      <xsl:variable name="leftContent">
        <xsl:choose>
          <xsl:when test="contains(text(),'&amp;C' )">
            <xsl:value-of
              select="substring-before(substring-after(text(),'&amp;L' ),'&amp;C' )"/>
          </xsl:when>
          <xsl:when test="contains(text(),'&amp;R' )">
            <xsl:value-of
              select="substring-before(substring-after(text(),'&amp;L' ),'&amp;R' )"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="substring-after(text(),'&amp;L' )"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>

      <style:region-left>
        <xsl:call-template name="InsertParagraphs">
          <xsl:with-param name="region" select=" 'L' "/>
          <xsl:with-param name="string" select="$leftContent"/>
        </xsl:call-template>
      </style:region-left>
    </xsl:if>

    <!-- header/footer content string can start with out a region specification tag then it is treated as center region -->
    <xsl:if
      test="contains(text(),'&amp;C') or ($start != '&amp;L' and $start != '&amp;C' and $start != '&amp;R')">
      <xsl:variable name="centerContent">
        <xsl:choose>

          <xsl:when test="contains(text(),'&amp;R' )">
            <xsl:choose>
              <!-- when header/footer content string starts with a region specification tag then center region tag is present for sure -->
              <xsl:when test="$start = '&amp;L' or $start = '&amp;C' ">
                <xsl:value-of
                  select="substring-before(substring-after(text(),'&amp;C' ),'&amp;R' )"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="substring-before(text(),'&amp;R' )"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:when>

          <xsl:otherwise>
            <xsl:choose>
              <!-- when header/footer content string starts with a region specification tag then center region tag is present for sure -->
              <xsl:when test="$start = '&amp;L' or $start = '&amp;C' ">
                <xsl:value-of select="substring-after(text(),'&amp;C' )"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="text()"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>

      <xsl:choose>
        <xsl:when test="contains(text(),'&amp;L') or contains(text(),'&amp;R')">
          <style:region-center>
            <xsl:call-template name="InsertParagraphs">
              <xsl:with-param name="region" select=" 'C' "/>
              <xsl:with-param name="string" select="$centerContent"/>
            </xsl:call-template>
          </style:region-center>
        </xsl:when>
        <xsl:otherwise>
          <xsl:call-template name="InsertParagraphs">
            <xsl:with-param name="region" select=" 'C' "/>
            <xsl:with-param name="string" select="$centerContent"/>
          </xsl:call-template>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:if>

    <xsl:if test="contains(text(),'&amp;R' )">
      <xsl:variable name="rightContent">
        <xsl:value-of select="substring-after(text(),'&amp;R' )"/>
      </xsl:variable>

      <style:region-right>
        <xsl:call-template name="InsertParagraphs">
          <xsl:with-param name="region" select=" 'R' "/>
          <xsl:with-param name="string" select="$rightContent"/>
        </xsl:call-template>
      </style:region-right>
    </xsl:if>
  </xsl:template>

  <xsl:template name="InsertParagraphs">
    <xsl:param name="region"/>
    <xsl:param name="paragraph" select="0"/>
    <xsl:param name="string" select="."/>
    <xsl:choose>
      <xsl:when test="contains($string, '&#xA;')">
        <text:p>
          <xsl:call-template name="GetHeaderFooterFields">
            <xsl:with-param name="paragraph" select="$paragraph + 1"/>
            <xsl:with-param name="tags">
              <xsl:value-of select="substring-before($string, '&#xA;')"/>
            </xsl:with-param>

            <xsl:with-param name="region" select="$region"/>
          </xsl:call-template>
        </text:p>
        <xsl:call-template name="InsertParagraphs">
          <xsl:with-param name="paragraph" select="$paragraph + 1"/>
          <xsl:with-param name="string" select="substring-after($string, '&#xA;')"/>

          <xsl:with-param name="region" select="$region"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <text:p>
          <xsl:call-template name="GetHeaderFooterFields">
            <xsl:with-param name="paragraph" select="$paragraph + 1"/>
            <xsl:with-param name="tags" select="$string"/>

            <xsl:with-param name="region" select="$region"/>
          </xsl:call-template>
        </text:p>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="InsertHeaderFooterStyle">
    <xsl:param name="region"/>
    <xsl:param name="paragraph"/>
    <xsl:param name="styleNum" select="1"/>
    <xsl:param name="tags" select="text()"/>

    <xsl:param name="fontFace" select=" 'Calibri' "/>
    <xsl:param name="fontSize" select="11"/>
    <xsl:param name="fontColor" select=" 'K000000' "/>
    <xsl:param name="underline" select=" 'none' "/>
    <xsl:param name="strikethrough" select="0"/>
    <!-- superscript/subscript -->
    <xsl:param name="position" select=" 'normal' "/>

    <style:style>
      <xsl:attribute name="style:name">
        <xsl:value-of select="concat(generate-id(),'_',$region,'P',$paragraph,'-',$styleNum)"/>
      </xsl:attribute>
      <xsl:attribute name="style:family">
        <xsl:text>text</xsl:text>
      </xsl:attribute>

      <style:text-properties>
        <xsl:attribute name="fo:color">
          <xsl:value-of select="concat('#',substring($fontColor,2))"/>
        </xsl:attribute>
        <xsl:attribute name="style:font-name">
          <xsl:value-of select="$fontFace"/>
        </xsl:attribute>
        <xsl:attribute name="fo:font-size">
          <xsl:value-of select="concat($fontSize,'pt')"/>
        </xsl:attribute>

        <xsl:choose>
          <xsl:when test="$underline = 'single' ">
            <xsl:attribute name="style:text-underline-type">single</xsl:attribute>
            <xsl:attribute name="style:text-underline-style">solid</xsl:attribute>
            <xsl:attribute name="style:text-underline-width">normal</xsl:attribute>
          </xsl:when>
          <xsl:when test="$underline = 'double' ">
            <xsl:attribute name="style:text-underline-type">double</xsl:attribute>
            <xsl:attribute name="style:text-underline-style">solid</xsl:attribute>
            <xsl:attribute name="style:text-underline-width">normal</xsl:attribute>
          </xsl:when>
        </xsl:choose>

        <xsl:if test="$strikethrough = 1">
          <xsl:attribute name="style:text-line-through-style">
            <xsl:text>solid</xsl:text>
          </xsl:attribute>
        </xsl:if>

        <xsl:choose>
          <xsl:when test="$position = 'superscript' ">
            <xsl:attribute name="style:text-position">
              <xsl:text>super 58%</xsl:text>
            </xsl:attribute>
          </xsl:when>
          <xsl:when test="$position = 'subscript' ">
            <xsl:attribute name="style:text-position">
              <xsl:text>sub 58%</xsl:text>
            </xsl:attribute>
          </xsl:when>
        </xsl:choose>
      </style:text-properties>
    </style:style>
  </xsl:template>

  <xsl:template name="SkipFontSizeTag">
    <xsl:param name="region"/>
    <xsl:param name="paragraph"/>
    <xsl:param name="tags"/>
    <xsl:param name="styleNum"/>

    <xsl:choose>
      <xsl:when test="number(substring($tags,1,1)) or substring($tags,1,1) = '0' ">
        <xsl:call-template name="SkipFontSizeTag">
          <xsl:with-param name="tags" select="substring($tags,2)"/>

          <xsl:with-param name="region" select="$region"/>
          <xsl:with-param name="paragraph" select="$paragraph"/>
          <xsl:with-param name="styleNum" select="$styleNum"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="GetHeaderFooterFields">
          <xsl:with-param name="changedStyle" select="1"/>
          <xsl:with-param name="region" select="$region"/>
          <xsl:with-param name="paragraph" select="$paragraph"/>
          <xsl:with-param name="tags" select="$tags"/>
          <xsl:with-param name="styleNum" select="$styleNum"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="ReadFontSizeTag">
    <xsl:param name="tags"/>
    <xsl:param name="value" select=" '' "/>

    <xsl:choose>
      <xsl:when test="number(substring($tags,1,1)) or substring($tags,1,1) = '0' ">
        <xsl:call-template name="ReadFontSizeTag">
          <xsl:with-param name="tags" select="substring($tags,2)"/>
          <xsl:with-param name="value" select="concat($value,substring($tags,1,1))"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$value"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="InsertHeaderFooterFonts">
    <xsl:param name="activeTab"/>

    <xsl:for-each
      select="key('Part', concat('xl/worksheets/sheet', $activeTab + 1,'.xml'))/e:worksheet/e:headerFooter">
      <!-- odd header styles -->
      <xsl:for-each select="e:oddHeader">
        <xsl:call-template name="GetHeaderFooterFonts">
          <xsl:with-param name="tags" select="text()"/>
        </xsl:call-template>
      </xsl:for-each>

      <!-- even header styles -->
      <xsl:for-each select="e:evenHeader">
        <xsl:call-template name="GetHeaderFooterFonts">
          <xsl:with-param name="tags" select="text()"/>
        </xsl:call-template>
      </xsl:for-each>

      <!-- odd footer styles -->
      <xsl:for-each select="e:oddFooter">
        <xsl:call-template name="GetHeaderFooterFonts">
          <xsl:with-param name="tags" select="text()"/>
        </xsl:call-template>
      </xsl:for-each>

      <!-- even footer styles -->
      <xsl:for-each select="e:evenFooter">
        <xsl:call-template name="GetHeaderFooterFonts">
          <xsl:with-param name="tags" select="text()"/>
        </xsl:call-template>
      </xsl:for-each>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="GetHeaderFooterFonts">
    <xsl:param name="tags"/>

    <xsl:choose>
      <xsl:when test="substring-before($tags,'&amp;') = '' ">
        <xsl:choose>
          <!-- skip region tags -->
          <xsl:when
            test="(contains($tags,'&amp;L' ) and substring-before($tags,'&amp;L' ) = '' ) or  (contains($tags,'&amp;C' ) and substring-before($tags,'&amp;C' ) = '' ) or
              (contains($tags,'&amp;R' ) and substring-before($tags,'&amp;R' ) = '' )">

            <xsl:call-template name="GetHeaderFooterFonts">
              <xsl:with-param name="tags" select="substring($tags,3)"/>
            </xsl:call-template>
          </xsl:when>
          <!-- skip field tags -->
          <xsl:when
            test="(contains($tags,'&amp;P' ) and substring-before($tags,'&amp;P' ) = '' ) or  (contains($tags,'&amp;N' ) and substring-before($tags,'&amp;N' ) = '' ) or
              (contains($tags,'&amp;D' ) and substring-before($tags,'&amp;D' ) = '' ) or (contains($tags,'&amp;T' ) and substring-before($tags,'&amp;T' ) = '' ) or
              (contains($tags,'&amp;Z' ) and substring-before($tags,'&amp;Z' ) = '' ) or (contains($tags,'&amp;F' ) and substring-before($tags,'&amp;F' ) = '' ) or
              (contains($tags,'&amp;A' ) and substring-before($tags,'&amp;A' ) = '' ) or (contains($tags,'&amp;G' ) and substring-before($tags,'&amp;G' ) = '' )">

            <xsl:call-template name="GetHeaderFooterFonts">
              <xsl:with-param name="tags" select="substring($tags,3)"/>
            </xsl:call-template>

          </xsl:when>
          <!-- font style -->
          <xsl:when
            test="contains($tags,'&amp;&quot;' ) and substring-before($tags,'&amp;&quot;' ) = '' ">
            <xsl:variable name="fontTag">
              <xsl:value-of
                select="substring-before(substring-after($tags,'&quot;' ),'&quot;' )"/>
            </xsl:variable>

            <xsl:variable name="font">
              <xsl:choose>
                <xsl:when test="contains($fontTag,',' )">
                  <xsl:value-of select="substring-before($fontTag,',' )"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="$fontTag"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>

            <xsl:variable name="newFont">
              <xsl:choose>
                <xsl:when test="$font = '-' ">
                  <xsl:text>Calibri</xsl:text>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="$font"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>

            <style:font-face>
              <xsl:attribute name="style:name">
                <xsl:value-of select="$newFont"/>
              </xsl:attribute>
              <xsl:attribute name="svg:font-family">
                <xsl:value-of select="$newFont"/>
              </xsl:attribute>
            </style:font-face>

            <xsl:call-template name="GetHeaderFooterFonts">
              <xsl:with-param name="tags"
                select="substring-after(substring-after($tags,'&amp;&quot;' ),'&quot;')"
              />
            </xsl:call-template>
          </xsl:when>
          <!-- font size -->
          <xsl:when
            test="number(substring($tags,2,1)) and contains($tags,'&amp;' ) and substring-before($tags,'&amp;' ) = '' ">

            <xsl:variable name="size">
              <xsl:call-template name="ReadFontSizeTag">
                <xsl:with-param name="tags" select="substring($tags,2)"/>
              </xsl:call-template>
            </xsl:variable>

            <xsl:call-template name="GetHeaderFooterFonts">
              <!-- +2 = &amp; + next character -->
              <xsl:with-param name="tags" select="substring($tags,string-length($size) + 2)"/>
            </xsl:call-template>
          </xsl:when>
          <!-- font color -->
          <xsl:when
            test="contains($tags,'&amp;K' ) and substring-before($tags,'&amp;K' ) = '' ">

            <xsl:call-template name="GetHeaderFooterFonts">
              <xsl:with-param name="tags" select="substring($tags,9)"/>
            </xsl:call-template>
          </xsl:when>
          <!-- skip other formating tags -->
          <xsl:when
            test="(contains($tags,'&amp;S' ) and substring-before($tags,'&amp;S' ) = '' ) or (contains($tags,'&amp;X' ) and substring-before($tags,'&amp;X' ) = '' ) or
        (contains($tags,'&amp;Y' ) and substring-before($tags,'&amp;Y' ) = '' ) or (contains($tags,'&amp;U' ) and substring-before($tags,'&amp;U' ) = '' ) or
        (contains($tags,'&amp;E' ) and substring-before($tags,'&amp;E' ) = '' ) or (contains($tags,'&amp;B' ) and substring-before($tags,'&amp;B' ) = '' ) or 
        (contains($tags,'&amp;I' ) and substring-before($tags,'&amp;I' ) = '' ) or (contains($tags,'&amp;O' ) and substring-before($tags,'&amp;O' ) = '' ) or 
        (contains($tags,'&amp;H' ) and substring-before($tags,'&amp;H' ) = '' )">

            <xsl:call-template name="GetHeaderFooterFonts">
              <xsl:with-param name="tags" select="substring($tags,3)"/>
            </xsl:call-template>
          </xsl:when>
        </xsl:choose>
      </xsl:when>
      <!-- plain text before next &amp; -->
      <xsl:otherwise>
        <xsl:variable name="stringLength">
          <xsl:value-of select="string-length(substring-before($tags,'&amp;'))"/>
        </xsl:variable>

        <xsl:call-template name="GetHeaderFooterFonts">
          <xsl:with-param name="tags" select="substring($tags,$stringLength + 1)"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

</xsl:stylesheet>
