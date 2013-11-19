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
  xmlns:pzip="urn:cleverage:xmlns:post-processings:zip"
  xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0" exclude-result-prefixes="svg table r text style fo">
  
  <!-- Insert Borders -->
  
  <xsl:template match="style:table-cell-properties" mode="border">
    
    <border>
      
      <xsl:if test="@style:diagonal-bl-tr != '' and @style:diagonal-bl-tr != 'none'">
        <xsl:attribute name="diagonalUp">
          <xsl:text>1</xsl:text>
        </xsl:attribute>
      </xsl:if>
      
      <xsl:if test="@style:diagonal-tl-br != '' and @style:diagonal-tl-br != 'none'">
        <xsl:attribute name="diagonalDown">
          <xsl:text>1</xsl:text>
        </xsl:attribute>
      </xsl:if>
      
      <xsl:if test="(@fo:border-left != '' and @fo:border-left != 'none') or (@fo:border != '' and @fo:border != 'none' )">
        
        <left>
            <xsl:attribute name="style">
              <xsl:call-template name="GetBorderStyle">
                <xsl:with-param name="style">
                  <xsl:choose>
                    <xsl:when test="@fo:border-left">
                      <xsl:value-of select="substring-before(@fo:border-left, '#')"/>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:value-of select="substring-before(@fo:border, '#')"/>
                    </xsl:otherwise>
                  </xsl:choose>                  
                </xsl:with-param>
              </xsl:call-template>
            </xsl:attribute>
                <xsl:call-template name="GetBorderColor">
                  <xsl:with-param name="color">
                    <xsl:choose>
                      <xsl:when test="@fo:border-left">
                        <xsl:value-of select="substring-after(@fo:border-left, '#')"/>
                      </xsl:when>
                      <xsl:otherwise>
                        <xsl:value-of select="substring-after(@fo:border, '#')"/>
                      </xsl:otherwise>
                    </xsl:choose>                              
                  </xsl:with-param>
                </xsl:call-template>
      </left>
        
      </xsl:if> 
      
      <xsl:if test="(@fo:border-right != '' and @fo:border-right != 'none') or (@fo:border != '' and @fo:border != 'none' )">
        
      <right>
          <xsl:attribute name="style">
            <xsl:call-template name="GetBorderStyle">
              <xsl:with-param name="style">
                <xsl:choose>
                  <xsl:when test="@fo:border-right">
                    <xsl:value-of select="substring-before(@fo:border-right, '#')"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="substring-before(@fo:border, '#')"/>
                  </xsl:otherwise>
                </xsl:choose>                  
              </xsl:with-param>
            </xsl:call-template>
          </xsl:attribute>
          <xsl:call-template name="GetBorderColor">
            <xsl:with-param name="color">
              <xsl:choose>
                <xsl:when test="@fo:border-right">
                  <xsl:value-of select="substring-after(@fo:border-right, '#')"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="substring-after(@fo:border, '#')"/>
                </xsl:otherwise>
              </xsl:choose>                    
            </xsl:with-param>
          </xsl:call-template>
      </right>
        
      </xsl:if>


      <xsl:if test="(@fo:border-top != '' and @fo:border-top != 'none') or (@fo:border != '' and @fo:border != 'none' )" >
        
      <top>
        <xsl:attribute name="style">
          <xsl:call-template name="GetBorderStyle">
            <xsl:with-param name="style">
              <xsl:choose>
                <xsl:when test="@fo:border-top">
                  <xsl:value-of select="substring-before(@fo:border-top, '#')"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="substring-before(@fo:border, '#')"/>
                </xsl:otherwise>
              </xsl:choose>                  
            </xsl:with-param>
          </xsl:call-template>
        </xsl:attribute>
          <xsl:call-template name="GetBorderColor">
            <xsl:with-param name="color">
              <xsl:choose>
                <xsl:when test="@fo:border-top">
                  <xsl:value-of select="substring-after(@fo:border-top, '#')"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="substring-after(@fo:border, '#')"/>
                </xsl:otherwise>
              </xsl:choose>                         
            </xsl:with-param>
          </xsl:call-template>
      </top>
        
      </xsl:if>
      
      
      <xsl:if test="(@fo:border-bottom != '' and @fo:border-bottom != 'none') or (@fo:border != '' and @fo:border != 'none' )">
        
      <bottom>
          <xsl:attribute name="style">
            <xsl:call-template name="GetBorderStyle">
              <xsl:with-param name="style">
                <xsl:choose>
                  <xsl:when test="@fo:border-bottom">
                    <xsl:value-of select="substring-before(@fo:border-bottom, '#')"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="substring-before(@fo:border, '#')"/>
                  </xsl:otherwise>
                </xsl:choose>                  
              </xsl:with-param>
            </xsl:call-template>
          </xsl:attribute>
          <xsl:call-template name="GetBorderColor">
            <xsl:with-param name="color">
              <xsl:choose>
                <xsl:when test="@fo:border-bottom">
                  <xsl:value-of select="substring-after(@fo:border-bottom, '#')"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="substring-after(@fo:border, '#')"/>
                </xsl:otherwise>
              </xsl:choose>                                  
            </xsl:with-param>
          </xsl:call-template>
      </bottom>
        
      </xsl:if>

     
        <xsl:choose>
          <xsl:when test="@style:diagonal-bl-tr != '' and @style:diagonal-bl-tr != 'none'">
            <diagonal>
            <xsl:attribute name="style">
              <xsl:call-template name="GetBorderStyle">
                <xsl:with-param name="style">
                  <xsl:choose>
                    <xsl:when test="@style:diagonal-bl-tr">
                      <xsl:value-of select="substring-before(@style:diagonal-bl-tr, '#')"/>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:value-of select="substring-before(@style:diagonal-bl-tr, '#')"/>
                    </xsl:otherwise>
                  </xsl:choose>                  
                </xsl:with-param>
              </xsl:call-template>
            </xsl:attribute>
            <xsl:call-template name="GetBorderColor">
              <xsl:with-param name="color">
                <xsl:choose>
                  <xsl:when test="@style:diagonal-bl-tr">
                    <xsl:value-of select="substring-after(@style:diagonal-bl-tr, '#')"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="substring-after(@fo:border, '#')"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>
            </xsl:call-template>    
              </diagonal>
          </xsl:when>
          <xsl:when test="@style:diagonal-tl-br != '' and @style:diagonal-tl-br != 'none'">
            <diagonal>
            <xsl:attribute name="style">
              <xsl:call-template name="GetBorderStyle">
                <xsl:with-param name="style">
                  <xsl:choose>
                    <xsl:when test="@style:diagonal-tl-br">
                      <xsl:value-of select="substring-before(@style:diagonal-tl-br, '#')"/>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:value-of select="substring-before(@style:diagonal-tl-br, '#')"/>
                    </xsl:otherwise>
                  </xsl:choose>                  
                </xsl:with-param>
              </xsl:call-template>
            </xsl:attribute>
            <xsl:call-template name="GetBorderColor">
              <xsl:with-param name="color">
                <xsl:choose>
                  <xsl:when test="@style:diagonal-tl-br">
                    <xsl:value-of select="substring-after(@style:diagonal-tl-br, '#')"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="substring-after(@fo:border, '#')"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>
            </xsl:call-template>
            </diagonal>
          </xsl:when>
        </xsl:choose>
     
      
      </border>
    
  </xsl:template>
  
  <xsl:template match="text()" mode="border"/>
  
	<!--Changes By : Vijayeta				
				Desc       : Borders in SP2 Conversion
							 The conventional manner of representing a border in the input as well as Translator converetd ODS 
							 is '0.002cm solid #000000', but, ON SP2 Reverse Conversion, it's retained as 'thin solid #000000'.
							 Hence, additional conditions added(thin solid,thick solid and 2pt solid)
				File       : Mobelhersteller.DE.xlsx,Gasgesetz.DE.xlsx, FM_Shapes.FR.CONFIDENTIAL.xlsx,FM_Landscape.FR.CONFIDENTIAL.xlsx and _fedict - Xls 001.CONFIDENTIAL.xlsx
	-->
 <xsl:template name="GetBorderStyle">
    <xsl:param name="style"/>
   <xsl:variable name="Width">
     <xsl:value-of select="substring-before($style,'cm')"/>
   </xsl:variable>
       <xsl:choose>
         <xsl:when test="contains($style, 'double')">
           <xsl:text>double</xsl:text>
         </xsl:when>
         <xsl:when test="$Width = 0.002">
           <xsl:text>thin</xsl:text>
         </xsl:when>
         <xsl:when test="$Width = 0.088">
           <xsl:text>medium</xsl:text>
         </xsl:when>
         <xsl:when test="$Width &gt; 0.088">
           <xsl:text>thick</xsl:text>
         </xsl:when>
			<!--SP2-->
			<xsl:when test="contains($style, 'thin solid')">
				<xsl:text>thin</xsl:text>
			</xsl:when>
			<xsl:when test="contains($style, 'thick solid')">
				<xsl:text>thick</xsl:text>
			</xsl:when>			
			<xsl:when test="contains($style, '2pt solid')">
				<xsl:text>medium</xsl:text>
			</xsl:when>
			<!--End SP2-->
         <xsl:otherwise>
           <xsl:text>thin</xsl:text>
         </xsl:otherwise>
       </xsl:choose>
 </xsl:template>
  
  <xsl:template name="GetBorderColor">
    <xsl:param name="color"/>
    <xsl:choose>
      <xsl:when test="$color != '' and $color != '000000'">
        <color>
          <xsl:attribute name="rgb">
            <xsl:value-of select="concat('FF', $color)"/>
          </xsl:attribute>
        </color>
      </xsl:when>
      <xsl:otherwise>
        <color indexed="64"/>    
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>
  
</xsl:stylesheet>
