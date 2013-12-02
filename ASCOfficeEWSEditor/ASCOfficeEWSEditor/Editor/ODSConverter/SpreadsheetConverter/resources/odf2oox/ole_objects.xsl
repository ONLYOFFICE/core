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
  xmlns:config="urn:oasis:names:tc:opendocument:xmlns:config:1.0"
  xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
  xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:v="urn:schemas-microsoft-com:vml"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
  xmlns:o="urn:schemas-microsoft-com:office:office" xmlns:x="urn:schemas-microsoft-com:office:excel"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  exclude-result-prefixes="table r config text style fo">

  <xsl:template name="InsertOLE_Object">
    <xsl:param name="sheetId"/>
	  <!--Defect Id       :1784784
			* Code Changed by :Vijayeta Tilak
			* Date            :26th Dec '07
			* Description     :This part of code( an OR condition to 'if') was added because when a file contains OLE object directly in a drive
			*                  then the value of attibute 'xlink:href' begins from a '/' and not '../'(which offcourse means within the folder.
			*                 This part of code( an additional '/') was added because when a notepad is inserted as an object, an addional elemnt
			*                  'table:shapes' is present which is absent when anyother object such as a word doc is inseretd
	   -->
    <xsl:if test="descendant::draw:frame/draw:object[starts-with(@xlink:href,'../') or starts-with(@xlink:href,'/')  and not(name(parent::node()/parent::node()) = 'draw:g' )]">
      <oleObjects>
        <xsl:apply-templates select=".//draw:frame[1]" mode="OLEobject">
          <xsl:with-param name="sheetId" select="$sheetId"/>
        </xsl:apply-templates>
      </oleObjects>
    </xsl:if>
  </xsl:template>
	<xsl:template match="draw:frame" mode="OLEobject">
		<xsl:param name="sheetId"/>
		<!--fix for the problem, where more than one ole objects linked do not work-->
		<xsl:variable name ="LinkId">
			<xsl:choose >
				<xsl:when test ="parent::node()[name()='table:table-cell']">
					<xsl:value-of select ="position()"/>
				</xsl:when>
				<xsl:otherwise >
					<xsl:value-of select ="count(preceding-sibling::draw:frame)+1"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="apos">
			<xsl:text>&apos;&apos;&apos;&apos;</xsl:text>
		</xsl:variable>
		<oleObject progId="opendocument.WriterDocument.1" oleUpdate="OLEUPDATE_ALWAYS">
			<xsl:attribute name="link">
				<xsl:value-of select="concat('[', $LinkId, ']!', $apos)"/>
			</xsl:attribute>
			<!--fix for the problem, where more than one ole objects linked do not work-->
			<xsl:attribute name="shapeId">
				<xsl:value-of select="$sheetId * 1024 + $LinkId"/>
			</xsl:attribute>
			<xsl:attribute name="progId">
				<xsl:value-of select="generate-id(draw:object)"/>
			</xsl:attribute>
		</oleObject>
		<xsl:if test="following-sibling::draw:frame">
			<xsl:apply-templates select="following-sibling::draw:frame[1]" mode="OLEobject">
				<xsl:with-param name="sheetId" select="$sheetId"/>
				<xsl:with-param name="LinkId">
					<xsl:value-of select="$LinkId + 1"/>
				</xsl:with-param>
			</xsl:apply-templates>
		</xsl:if>
	</xsl:template>
	<xsl:template name="InsertOLEexternalLinks">
	  <!--Defect Id       :1784784
		* Code Changed by :Vijayeta Tilak
		* Date            :26th Dec '07
		* Description     :This part of code( an OR condition to 'for-each' loop) was added because when a file contains OLE object directly in a drive
		*                  then the value of attibute 'xlink:href' begins from a '/' and not '../'(which offcourse means within the folder.	
	  -->
    <xsl:for-each
      select="document('content.xml')/office:document-content/office:body/office:spreadsheet/descendant::draw:frame/draw:object[starts-with(@xlink:href,'../') or starts-with(@xlink:href,'/')  and not(name(parent::node()/parent::node()) = 'draw:g' )]">
      <pzip:entry pzip:target="{concat('xl/externalLinks/externalLink',position(),'.xml')}">
        <externalLink xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
          <oleLink xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
            r:id="rId1">

            <xsl:attribute name="progId">
              <xsl:value-of select="generate-id()"/>
            </xsl:attribute>

            <oleItems>
              <oleItem name="'" advise="1" preferPic="1"/>
            </oleItems>
          </oleLink>
        </externalLink>
      </pzip:entry>
    </xsl:for-each>
  </xsl:template>


  <xsl:template name="OLEexternalLinks_rels">
	  <!--Defect Id     :1784784
		* Code Changed by :Vijayeta Tilak
		* Date            :26th Dec '07
		* Description     :This part of code( an OR condition to 'for-each' loop) was added because when a file contains OLE object directly in a drive
		*                  then the value of attibute 'xlink:href' begins from a '/' and not '../'(which offcourse means within the folder.	
		*                  Also an additional 'when' is added that takes care of the above mentioned condition.
		-->
    <xsl:for-each
      select="document('content.xml')/office:document-content/office:body/office:spreadsheet/descendant::draw:frame/draw:object[starts-with(@xlink:href,'../') or starts-with(@xlink:href,'/') and not(name(parent::node()/parent::node()) = 'draw:g' )]">
      <pzip:entry
        pzip:target="{concat('xl/externalLinks/_rels/externalLink',position(),'.xml.rels')}">
        <Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships">
                
              <xsl:choose>
                <xsl:when test="@xlink:href">

                  <!-- change spaces to %20 -->
                  <xsl:variable name="translatedTarget">
                    <xsl:call-template name="TranslateIllegalChars">
                      <xsl:with-param name="string" select="@xlink:href"/>
                    </xsl:call-template>
                  </xsl:variable>

                  <xsl:choose>
                    <!-- picture is located in the same disk -->
                    <xsl:when test="starts-with($translatedTarget,'../')">
                      <pxsi:physicalPath xmlns:pxsi="urn:cleverage:xmlns:post-processings:path">
                        <Relationship
                          xmlns="http://schemas.openxmlformats.org/package/2006/relationships"
                          Id="rId1"
                          Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/oleObject"
                          TargetMode="External">
                          <xsl:attribute name="Target">
                            <xsl:value-of
                              select="translate(substring-after($translatedTarget,'../'),'/','\')"/>
                          </xsl:attribute>
                          
                        </Relationship>
                      </pxsi:physicalPath>
                    </xsl:when>
					  <!-- fix for 1784784-->
					  <xsl:when test="starts-with($translatedTarget,'/')">
						  <pxsi:physicalPath xmlns:pxsi="urn:cleverage:xmlns:post-processings:path">
							  <Relationship
								xmlns="http://schemas.openxmlformats.org/package/2006/relationships"
								Id="rId1"
								Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/oleObject"
								TargetMode="External">
								  <xsl:attribute name="Target">
									  <xsl:value-of
										select="translate(substring-after($translatedTarget,'/'),'/','\')"/>
								  </xsl:attribute>
							  </Relationship>
						  </pxsi:physicalPath>
					  </xsl:when>
                    <xsl:otherwise>
                      <xsl:choose>
                        <!-- when file is on local disk -->
                        <xsl:when test="starts-with($translatedTarget,'/')">
                          <xsl:value-of
                            select="concat('file:///',translate(substring-after($translatedTarget,'/'),'/','\'))"
                          />
                        </xsl:when>
                        <!-- when file is on network disk -->
                        <xsl:otherwise>
                          <xsl:value-of select="translate($translatedTarget,'/','\')"/>
                        </xsl:otherwise>
                      </xsl:choose>
                    </xsl:otherwise>

                  </xsl:choose>

                </xsl:when>
              </xsl:choose>

        </Relationships>
      </pzip:entry>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="ExternalReference">
	  <!--Defect Id     :1784784
		* Code Changed by :Vijayeta Tilak
		* Date            :26th Dec '07
		* Description     :This part of code( an OR condition to 'for-each' loop) was added because when a file contains OLE object directly in a drive
		*                  then the value of attibute 'xlink:href' begins from a '/' and not '../'(which offcourse means within the folder.	
		-->
    <xsl:if
      test="descendant::draw:frame/draw:object[starts-with(@xlink:href,'../') or starts-with(@xlink:href,'/') and not(name(parent::node()/parent::node()) = 'draw:g' )]">
      <externalReferences>
        <xsl:for-each
          select="descendant::draw:frame/draw:object[starts-with(@xlink:href,'../') or starts-with(@xlink:href,'/') and not(name(parent::node()/parent::node()) = 'draw:g' )]">
          <externalReference>
            <xsl:attribute name="r:id">
              <xsl:value-of select="generate-id()"/>
            </xsl:attribute>
          </externalReference>
        </xsl:for-each>
      </externalReferences>
    </xsl:if>
  </xsl:template>

</xsl:stylesheet>
