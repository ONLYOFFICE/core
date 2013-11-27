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
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:dc="http://purl.org/dc/elements/1.1/"
  xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"
  xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
  xmlns:oox="urn:oox"
                xmlns="http://www.w3.org/1999/xhtml"
  exclude-result-prefixes="w dc xlink oox">

	<xsl:template match="w:font">
		<style:font-face xmlns="http://www.w3.org/1999/xhtml">
			<xsl:attribute name="style:name">
				<xsl:value-of select="@w:name"/>
			</xsl:attribute>

			<xsl:if test="w:charset/@w:val='02'">
				<xsl:attribute name="style:font-charset">
					<xsl:text>x-symbol</xsl:text>
				</xsl:attribute>
			</xsl:if>

			<xsl:attribute name="svg:font-family">
				<xsl:value-of select="@w:name"/>
			</xsl:attribute>

			<xsl:if test="w:family/@w:val != 'auto' ">
				<xsl:attribute name="style:font-family-generic">
					<xsl:value-of select="w:family/@w:val"/>
				</xsl:attribute>
			</xsl:if>

			<xsl:if test="w:family/@w:val = 'auto' ">
				<xsl:attribute name="style:font-family-generic">system</xsl:attribute>
			</xsl:if>

			<xsl:if test="w:pitch/@w:val != 'default' ">
				<xsl:attribute name="style:font-pitch">
					<xsl:value-of select="w:pitch/@w:val"/>
				</xsl:attribute>
			</xsl:if>

		</style:font-face>
	</xsl:template>
</xsl:stylesheet>
