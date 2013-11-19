<?xml version="1.0" encoding="UTF-8"?>
<!-- 
  * Copyright (c) 2007-2009 DIaLOGIKa
  *
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
  *     * Neither the names of copyright holders, nor the names of its contributors 
  *       may be used to endorse or promote products derived from this software 
  *       without specific prior written permission.
  * 
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
  * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
  * IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
  * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
  * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
  * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
  * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
  -->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

  <xsl:template name="substring-after-last">
    <xsl:param name="string" />
    <xsl:param name="occurrence" />

    <xsl:choose>
      <xsl:when test="$occurrence = '' or not(contains($string, $occurrence))">
        <xsl:value-of select="$string" />
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="substring-after-last">
          <xsl:with-param name="string" select="substring-after($string, $occurrence)" />
          <xsl:with-param name="occurrence" select="$occurrence" />
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="substring-count">
    <xsl:param name="string" />
    <xsl:param name="occurrence" />
    <xsl:param name="count" select="0" />

    <xsl:choose>
      <xsl:when test="$occurrence = '' or not(contains($string, $occurrence))">
        <xsl:value-of select="$count" />
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="substring-count">
          <xsl:with-param name="string" select="substring-after($string, $occurrence)" />
          <xsl:with-param name="occurrence" select="$occurrence" />
          <xsl:with-param name="count" select="$count +1" />
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

</xsl:stylesheet>