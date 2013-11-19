<?xml version="1.0" encoding="UTF-8" ?>
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
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:oox="urn:oox"
  xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"
  xmlns:ct="http://schemas.openxmlformats.org/package/2006/content-types">

	<xsl:template match="/oox:package">
		<xsl:call-template name="VisitPart">
			<xsl:with-param name="content-type" select="'main+xml'"/>
		</xsl:call-template>
		<xsl:call-template name="VisitPart">
			<xsl:with-param name="content-type" select="'styles+xml'"/>
		</xsl:call-template>
		<xsl:call-template name="VisitPart">
			<xsl:with-param name="content-type" select="'header+xml'"/>
		</xsl:call-template>
		<xsl:call-template name="VisitPart">
			<xsl:with-param name="content-type" select="'footer+xml'"/>
		</xsl:call-template>
		<xsl:call-template name="VisitPart">
			<xsl:with-param name="content-type" select="'footnotes+xml'"/>
		</xsl:call-template>
		<xsl:call-template name="VisitPart">
			<xsl:with-param name="content-type" select="'endnotes+xml'"/>
		</xsl:call-template>
	</xsl:template>

	<xsl:template name="VisitPart">
		<xsl:param name="content-type"/>

		<xsl:for-each
		  select="/oox:package/oox:part[@oox:name='[Content_Types].xml']/ct:Types/ct:Override[contains(@ContentType, $content-type)]">

			<xsl:variable name="path" select="substring-after(@PartName, '/')"/>
			<xsl:for-each select="/oox:package/oox:part[@oox:name=$path]">
				<xsl:apply-templates/>
			</xsl:for-each>
		</xsl:for-each>
	</xsl:template>

	<xsl:template match="w:body">
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="w:p">
		<xsl:message terminate="no">progress:w:p</xsl:message>
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="w:r">
		<xsl:message terminate="no">progress:w:r</xsl:message>
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="w:pPr">
		<xsl:message terminate="no">progress:w:pPr</xsl:message>
	</xsl:template>

	<xsl:template match="w:rPr">
		<xsl:message terminate="no">progress:w:rPr</xsl:message>
	</xsl:template>

	<xsl:template match="w:t">
		<xsl:message terminate="no">progress:w:t</xsl:message>
	</xsl:template>

	<xsl:template match="w:style">
		<xsl:message terminate="no">progress:w:style</xsl:message>
	</xsl:template>

</xsl:stylesheet>
