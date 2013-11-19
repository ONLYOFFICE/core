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
  xmlns:pzip="urn:cleverage:xmlns:post-processings:zip"
  xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main"
  xmlns:pic="http://schemas.openxmlformats.org/drawingml/2006/picture"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"
  xmlns:xdr="http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing"
  xmlns:e="http://schemas.openxmlformats.org/spreadsheetml/2006/main"
  xmlns:rels="http://schemas.openxmlformats.org/package/2006/relationships"
  exclude-result-prefixes="e a pic r w rels">

  <xsl:template name="GetTarget">
    <xsl:param name="document"/>
    <xsl:param name="file" select="$document"/>
    <xsl:param name="id"/>

    <!--xsl:variable name="file" select="substring-after($document, '/')" /-->
    
    
    <xsl:choose>
      <xsl:when test="contains($file,'/')">
        <xsl:call-template name="GetTarget">
          <xsl:with-param name="file" select="substring-after($file,'/')"/>
          <xsl:with-param name="id" select="$id"/>
          <xsl:with-param name="document" select="$document"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="key('Part', concat(substring-before($document,$file),'_rels/',$file,'.rels'))/rels:Relationships/rels:Relationship[@Id=$id]/@Target"/>

        <!--xsl:if test="key('Part', concat(substring-before($document,$file),'_rels/',$file,'.rels'))">
          <xsl:for-each
            select="key('Part', concat(substring-before($document,$file),'_rels/',$file,'.rels'))//node()[name()='Relationship']">
            <xsl:if test="./@Id=$id">
              <xsl:value-of select="./@Target"/>
              </xsl:if>
          </xsl:for-each>
        </xsl:if-->
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  
  <xsl:template name="CopyPictures">
    <xsl:param name="document"/>
    <xsl:param name="rId"/>
    <xsl:param name="targetName"/>
    <xsl:param name="destFolder" select="'Pictures'"/>
    
    <!--  Copy Pictures Files to the picture catalog -->
    <xsl:variable name="id">
      <xsl:choose>
        <xsl:when test="$rId != ''">  
          <xsl:value-of select="$rId"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="xdr:pic/xdr:blipFill/a:blip/@r:embed"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:if test="key('Part', concat('xl/drawings/_rels/', $document))//node()[name() = 'Relationship' and contains(@Type,'image')]">
      <xsl:for-each
        select="key('Part', concat('xl/drawings/_rels/', $document))//node()[name() = 'Relationship' and contains(@Type,'image')]">
          <xsl:variable name="targetmode">
            <xsl:value-of select="./@TargetMode"/>
          </xsl:variable>
          <xsl:variable name="pzipsource">
            <xsl:value-of select="./@Target"/>
          </xsl:variable>
          <xsl:variable name="pziptarget">
            <xsl:choose>
              <xsl:when test="$targetName != ''">
                <xsl:value-of select="$targetName"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="substring-after($pzipsource,'/')"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:variable>        
          <xsl:choose>
            <xsl:when test="$targetmode='External'"/>
            <xsl:when test="$destFolder = '.'">
              <pzip:copy pzip:source="{concat('xl/',$pzipsource)}" pzip:target="{$pziptarget}"/>
            </xsl:when>            
            <xsl:otherwise>
              <pzip:copy pzip:source="{concat('xl/',substring-after($pzipsource, '/'))}" pzip:target="{concat($destFolder,'/',substring-after(substring-after($pzipsource, '/'), '/'))}"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:for-each>
        </xsl:if>

  </xsl:template>
  
  <!--  gets document name where image is placed (header, footer, document)-->
  <!--xsl:template name="GetDocumentName">
    <xsl:param name="rootId"/>
    
    <xsl:choose>
      <xsl:when test="ancestor::w:document ">
        <xsl:text>document.xml</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:for-each
          select="key('Part', 'word/_rels/document.xml.rels')//node()[name() = 'Relationship'][substring-after(@Target,'.') = 'xml']">
          <xsl:variable name="target">
            <xsl:value-of select="./@Target"/>
          </xsl:variable>
          <xsl:if test="generate-id(key('Part', concat('word/',$target))//node()) = $rootId">
            <xsl:value-of select="$target"/>
          </xsl:if>
        </xsl:for-each>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template-->
  
  
  
</xsl:stylesheet>
