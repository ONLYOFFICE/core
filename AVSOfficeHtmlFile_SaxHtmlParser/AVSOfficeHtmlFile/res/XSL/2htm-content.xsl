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
  xmlns="http://www.w3.org/1999/xhtml"
  xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:number="urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0"
  xmlns:wp="http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:xlink="http://www.w3.org/1999/xlink"
  xmlns:dc="http://purl.org/dc/elements/1.1/"
  xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
  xmlns:pcut="urn:cleverage:xmlns:post-processings:pcut"
  xmlns:v="urn:schemas-microsoft-com:vml"
  xmlns:o="urn:schemas-microsoft-com:office:office"
  xmlns:cp="http://schemas.openxmlformats.org/package/2006/metadata/core-properties"
  xmlns:oox="urn:oox"
  exclude-result-prefixes="w r xlink number wp oox">

  <xsl:import href="2htm-tables.xsl"/>
  <xsl:import href="2htm-lists.xsl"/>
  <xsl:import href="2htm-fonts.xsl"/>
  <xsl:import href="2htm-fields.xsl"/>
  <xsl:import href="2htm-footnotes.xsl"/>
  <xsl:import href="2htm-indexes.xsl"/>
  <xsl:import href="2htm-track.xsl"/>
  <!--xsl:import href="2htm-ole.xsl"/-->
  <xsl:import href="2htm-frames.xsl"/>
  <xsl:import href="2htm-sections.xsl"/>
  <!--xsl:import href="2htm-comments.xsl"/-->
  <xsl:import href="2htm-styles.xsl"/>

  <xsl:strip-space elements="*"/>
  <!--<xsl:preserve-space elements="w:p"/>-->
  <!--<xsl:preserve-space elements="w:r"/>-->
  <xsl:preserve-space elements="w:t"/>

  <xsl:key name="InstrText" match="w:instrText" use="''"/>
  <xsl:key name="bookmarkStart" match="w:bookmarkStart" use="@w:id"/>
  <xsl:key name="bookmarksByName" match="w:bookmarkStart" use="@w:name"/>
  <xsl:key name="numPr" match="w:pPr/w:numPr" use="''"/>
  <!--xsl:key name="sectPr" match="w:sectPr" use="''"/-->

  <xsl:key name="p" match="w:p" use="@oox:id" />
  <xsl:key name="sectPr" match="w:sectPr" use="@oox:s" />

  <!--main document-->
  <xsl:template name="content">
    <head>
      <title>
        <!--<xsl:value-of select="key('Part', 'docProps/core.xml')/cp:coreProperties/dc:title"/>-->
        <xsl:choose>
          <xsl:when test="key('Part', 'docProps/core.xml')/cp:coreProperties/dc:title != ''">
            <xsl:value-of select="key('Part', 'docProps/core.xml')/cp:coreProperties/dc:title"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>Document</xsl:text>
          </xsl:otherwise>
        </xsl:choose>
      </title>
      <meta http-equiv="Content-Type" content="text/html;charset=utf-8"/>
      <!-- charset=utf-8"/>-->
      <!--TODO: may be a useful part of code-->
      <!--<office:font-face-decls>
                <xsl:apply-templates select="key('Part', 'word/fontTable.xml')/w:fonts"/>
            </office:font-face-decls>-->

      <!-- automatic styles for document body -->
      <style type="text/css">
        <xsl:text disable-output-escaping="yes">&#10;</xsl:text>
        <xsl:call-template name="InsertBodyStyles"/>
        <xsl:call-template name="InsertListStyles"/>
        <xsl:call-template name="InsertSectionsStyles"/>
        <xsl:call-template name="InsertFootnoteStyles"/>
        <xsl:call-template name="InsertEndnoteStyles"/>
        <xsl:call-template name="InsertFrameStyles"/>
        <!--
					    makz: paragraph style for additional paragraph that should not be visible
				    -->
        <xsl:text disable-output-escaping="yes"> p.hiddenParagraph { visibility:hidden } &#10;</xsl:text>
        <xsl:call-template name="styles"/>
      </style>        
      <script type="text/javascript">
        <xsl:text>
							var useragent = navigator.userAgent;
							var navigatorname;
							if (useragent.indexOf('MSIE')!= -1)
							{
							navigatorname="MSIE";
							}
							else if (useragent.indexOf('Gecko')!= -1)
							{
							if (useragent.indexOf('Chrome')!= -1)
							navigatorname="Google Chrome";
							else
							navigatorname="Mozilla";
							}
							else if (useragent.indexOf('Mozilla')!= -1)
							{
							navigatorname="old Netscape or Mozilla";
							}
							else if (useragent.indexOf('Opera')!= -1)
							{
							navigatorname="Opera";
							}
							
							function symbol(code1,code2)
							{
							if (navigatorname == 'MSIE')
								document.write(code1);
							else
								document.write(code2);
							}
							</xsl:text>
      </script>
    </head>
    <body>

      <!-- Специальная ветка для CKEditor. -->
      <xsl:if test="$HtmlMode != 'CKEditor'">
        <xsl:text disable-output-escaping="yes">&lt;div class="basic"&gt;</xsl:text>
      </xsl:if>
        
        <!--AVSMedia: skip user fields; still may become useful later-->
        <!--<xsl:call-template name="InsertUserFieldDecls" />-->
        <xsl:call-template name="TrackChanges"/>
        <xsl:call-template name="InsertDocumentBody"/>
        <xsl:call-template name="InsertDocumentFootnotes"/>
        <xsl:call-template name="InsertDocumentEndnotes"/>

      <!-- Специальная ветка для CKEditor. -->
      <xsl:if test="$HtmlMode != 'CKEditor'">
        <xsl:text disable-output-escaping="yes">&lt;/div&gt;</xsl:text>
      </xsl:if>

    </body>
  </xsl:template>

  <!--<xsl:template match="cp:coreProperties">
        <xsl:value-of select="dc:title"/>
    </xsl:template>-->

  <!--  generates automatic styles for frames -->
  <xsl:template name="InsertFrameStyles">
    <!-- when w:pict is child of paragraph-->
    <!--<xsl:if test="key('Part', 'word/document.xml')/w:document/w:body/w:p/w:r/w:pict">
			<xsl:apply-templates select="key('Part', 'word/document.xml')/w:document/w:body/w:p/w:r/w:pict"
			  mode="automaticpict"/>
		</xsl:if>-->

    <!-- when w:pict is child of a cell-->
    <!--<xsl:if test="key('Part', 'word/document.xml')/w:document/w:body/w:tbl/w:tr/w:tc/w:p/w:r/w:pict">
			<xsl:apply-templates select="key('Part', 'word/document.xml')/w:document/w:body/w:tbl/w:tr/w:tc/w:p/w:r/w:pict" mode="automaticpict"/>
		</xsl:if>-->
    <!-- 2008-10-31/divo: Generate a style for every w:pict, not only the ones in tables and paragraphs -->
    <xsl:apply-templates select="key('Part', 'word/document.xml')/w:document/w:body//w:pict" mode="automaticpict"/>
  </xsl:template>

  <!--
		Summary: Insert the declarations of all SET fields
		Author: makz (DIaLOGIKa)
		Date: 2.11.2007
	-->
  <xsl:template name="InsertUserFieldDecls">
    <!--<text:user-field-decls>
            <xsl:apply-templates select="key('Part', 'word/document.xml')/w:document/w:body//w:instrText" mode="UserFieldDecls" />
        </text:user-field-decls>-->
  </xsl:template>

  <!--  generates automatic styles for sections-->
  <xsl:template name="InsertSectionsStyles">
    <xsl:if test="key('Part', 'word/document.xml')/w:document/w:body/w:p/w:pPr/w:sectPr">
      <xsl:apply-templates
			  select="key('Part', 'word/document.xml')/w:document/w:body/w:p/w:pPr/w:sectPr"
			  mode="automaticstyles"/>
    </xsl:if>
  </xsl:template>

  <!--  generates automatic styles for paragraphs  how does it exactly work ?? -->
  <xsl:template name="InsertBodyStyles">
    <xsl:apply-templates select="key('Part', 'word/document.xml')/w:document/w:body" mode="automaticstyles"/>
  </xsl:template>

  <xsl:template name="InsertListStyles">
    <!-- document with lists-->
    <xsl:for-each select="key('Part', 'word/document.xml')">
      <xsl:if test="$List2List = 1">
        <xsl:choose>
          <xsl:when test="key('numPr', '')/w:numId">
            <!-- automatic list styles with empty num format for elements which has non-existent w:num attached -->
            <xsl:apply-templates
							select="key('numPr', '')/w:numId[not(key('Part', 'word/numbering.xml')/w:numbering/w:num/@w:numId = @w:val)][1]"
							mode="automaticstyles"/>
            <!-- automatic list styles-->
            <xsl:apply-templates select="key('Part', 'word/numbering.xml')/w:numbering/w:num"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:for-each select="key('Part', 'word/styles.xml')">
              <xsl:if test="key('numPr', '')/w:numId">
                <!-- automatic list styles-->
                <xsl:apply-templates select="key('Part', 'word/numbering.xml')/w:numbering/w:num"/>
              </xsl:if>
            </xsl:for-each>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:if>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="InsertFootnoteStyles">
    <xsl:if test="key('Part', 'word/footnotes.xml')/w:footnotes/w:footnote/w:p/w:r/w:rPr | 
				              key('Part', 'word/footnotes.xml')/w:footnotes/w:footnote/w:p/w:pPr">
      <xsl:apply-templates select="key('Part', 'word/footnotes.xml')/w:footnotes/w:footnote/w:p" mode="automaticstyles"/>
    </xsl:if>
  </xsl:template>

  <xsl:template name="InsertEndnoteStyles">
    <xsl:if test="key('Part', 'word/endnotes.xml')/w:endnotes/w:endnote/w:p/w:r/w:rPr | 
                      key('Part', 'word/endnotes.xml')/w:endnotes/w:endnote/w:p/w:pPr">
      <xsl:apply-templates select="key('Part', 'word/endnotes.xml')/w:endnotes/w:endnote/w:p" mode="automaticstyles"/>
    </xsl:if>
  </xsl:template>

  <!--  when paragraph has no parent style it should be set to Normal style which contains all default paragraph properties -->
  <xsl:template name="InsertParagraphParentStyle">
    <xsl:choose>
      <xsl:when test="w:pStyle">
        <xsl:attribute name="style:parent-style-name">

          <xsl:variable name="isDefaultTOCStyle">
            <xsl:call-template name ="CheckDefaultTOCStyle">
              <xsl:with-param name="name">
                <xsl:value-of select="w:pStyle/@w:val" />
              </xsl:with-param>
            </xsl:call-template>
          </xsl:variable>

          <xsl:choose>
            <xsl:when test="$isDefaultTOCStyle='true'">
              <xsl:value-of select="concat('Contents_20_',substring-after(w:pStyle/@w:val,'TOC'))"/>
            </xsl:when>
            <xsl:when test="w:pStyle/@w:val='FootnoteText'">
              <xsl:text>Footnote_20_Symbol</xsl:text>
            </xsl:when>
            <xsl:when test="w:pStyle/@w:val='EndnoteText'">
              <xsl:text>Endnote_20_Symbol</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="w:pStyle/@w:val"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
      </xsl:when>
      <xsl:otherwise>
        <xsl:attribute name="style:parent-style-name">
          <xsl:text>Normal</xsl:text>
        </xsl:attribute>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>


  <!--
	  Summary:  Converts the child elements of the body and handels the section conversion
	  Author:   CleverAge
	  Modified: makz (DIaLOGIKa)
	-->
  <xsl:template name="InsertDocumentBody">
    <!--<xsl:choose>
            <xsl:when test="key('Part', 'word/document.xml')/w:document/w:body/w:p/w:pPr/w:sectPr" >
                <!-@-
					There is more than the default sectPr, we need to write an own section for each sectPr.
				-@->
                <xsl:apply-templates select="key('Part', 'word/document.xml')/w:document/w:body//w:sectPr" mode="sections" />
            </xsl:when>
            <xsl:otherwise>
                <!-@-
				There is only the default section, so we convert the childs directly. 
				The properties of the default sectPr are applied tho the "Standard" master page
				-@->
                <xsl:apply-templates select="key('Part', 'word/document.xml')/w:document/w:body/child::node()" />
            </xsl:otherwise>
        </xsl:choose>-->
    <xsl:apply-templates select="key('Part', 'word/document.xml')/w:document/w:body/child::node()" />
  </xsl:template>

  <xsl:template name="InsertDocumentFootnotes">
    <xsl:apply-templates select="key('Part', 'word/footnotes.xml')/w:footnotes/child::node()" />
  </xsl:template>

  <xsl:template name="InsertDocumentEndnotes">
    <xsl:apply-templates select="key('Part', 'word/endnotes.xml')/w:endnotes/child::node()" />
  </xsl:template>


  <!-- create a style for each paragraph. Do not take w:sectPr/w:rPr into consideration. -->
  <xsl:template
	  match="w:pPr[parent::w:p]|w:r[parent::w:p[not(child::w:pPr)] and (child::w:br[@w:type='page' or @w:type='column'] or contains(child::w:pict/v:shape/@style,'mso-position-horizontal-relative:char'))]"
	  mode="automaticstyles">
    <!--TODO: This part of code generates styles for each paragraph element.
                  It seems unnecessary since we can define styles inline in p elements in HTML.
                  Still may become useful later for some purposes.-->
    <!--
        <xsl:message terminate="no">progress:w:pPr</xsl:message>
        <style type="text/css">
            <xsl:text>
p.</xsl:text>
            <xsl:value-of select="generate-id(parent::w:p)"/>
            <xsl:text>
{</xsl:text>
            <!-@-
            <xsl:call-template name="InsertParagraphParentStyle"/>
            <xsl:call-template name="MasterPageName"/>-@->

            <xsl:for-each select="parent::w:p">
                <!-@-context switch -@->
                <xsl:call-template name="InsertPageNumberOffset" />
            </xsl:for-each>

            <xsl:call-template name="InsertParagraphProperties"/>
            <!-@- add text-properties to empty paragraphs. -@->
            <!-@-clam, dialogika: bugfix 1752761-@->
            <xsl:if test="parent::w:p[count(child::node()) = 1]/w:pPr/w:rPr or w:rPr/w:vanish">
                <xsl:call-template name="InsertTextProperties"/>
            </xsl:if>
            <xsl:text>
}
            </xsl:text>
        </style>
        -->
  </xsl:template>

  <xsl:template
	  match="w:p[not(./w:pPr) and not(w:r/w:br[@w:type='page' or @w:type='column']) and not(descendant::w:pict)]"
	  mode="automaticstyles">
    <!--<xsl:if test="key('Part', 'word/styles.xml')/w:styles/w:docDefaults/w:pPrDefault">
            <style type="text/css">
                <xsl:text> p.</xsl:text>
                <xsl:value-of select="generate-id(.)"/>
                <xsl:text>{ </xsl:text>
                <!-@-<xsl:call-template name="MasterPageName"/>-@->
                <xsl:call-template name="InsertDefaultParagraphProperties"/>
                <xsl:text>} </xsl:text>
            </style>
        </xsl:if>
        <xsl:apply-templates mode="automaticstyles"/>-->
  </xsl:template>

  <!-- create a style for each run. Do not take w:pPr/w:rPr into consideration. Ignore runs with no properties. -->
  <xsl:template match="w:rPr[parent::w:r and not(count(child::node())=1 and child::w:noProof)]"
	  mode="automaticstyles">
    <!--TODO: This part of code generates styles for each text (span) element.
                  It seems unnecessary since we can define styles inline in span element in HTML.
                  Still may become useful later for some purposes.-->
    <!--<xsl:message terminate="no">progress:w:rPr</xsl:message>
        <style type="text/css">
            <xsl:text>
span.</xsl:text>
            <xsl:value-of select="generate-id(parent::w:r)"/>
            <xsl:text>
{
    font:inherit;</xsl:text>
            <xsl:if test="w:rStyle">
                <xsl:attribute name="style:parent-style-name">
                    <!-@-clam bugfix #1806204-@->
                    <xsl:choose>
                        <xsl:when test="ancestor::w:r[contains(w:instrText,'TOC')] and w:rStyle/@w:val='Hyperlink'">X3AS7TOCHyperlink</xsl:when>
                        <xsl:otherwise>
                            <xsl:value-of select="w:rStyle/@w:val"/>
                        </xsl:otherwise>
                    </xsl:choose>
                </xsl:attribute>
            </xsl:if>
            <xsl:call-template name="InsertTextProperties"/>
            <xsl:text>
}
            </xsl:text>
        </style>-->
  </xsl:template>

  <!-- ignore text in automatic styles mode. -->
  <!--<xsl:template match="w:t" mode="automaticstyles">-->
  <!-- do nothing. -->
  <!--</xsl:template>-->

  <!--  get outline level from styles hierarchy -->
  <xsl:template name="GetOutlineLevelByStyleId">
    <xsl:param name="styleId"/>

    <xsl:choose>
      <xsl:when test="key('StyleId', $styleId)[1]/w:pPr/w:outlineLvl/@w:val">
        <xsl:value-of select="key('StyleId', $styleId)[1]/w:pPr/w:outlineLvl/@w:val"/>
      </xsl:when>
      <!--  Search outlineLvl recursively in style hierarchy -->
      <xsl:when test="key('StyleId', $styleId)[1]/w:basedOn/@w:val">
        <xsl:call-template name="GetOutlineLevelByStyleId">
          <xsl:with-param name="styleId">
            <xsl:value-of select="key('StyleId', $styleId)[1]/w:basedOn/@w:val"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- Get outlineLvl if the paragraph is heading -->
  <!--<xsl:template name="GetOutlineLevel">
		<xsl:param name="node" select="."/>
		
		<xsl:for-each select="key('Part', 'word/styles.xml')">
			<xsl:choose>
				<xsl:when test="$node/w:pPr/w:pStyle/@w:val">
					<xsl:variable name="outline">
						<xsl:value-of select="$node/w:pPr/w:pStyle/@w:val"/>
					</xsl:variable>
					-->
  <!--Search outlineLvl in styles.xml  -->
  <!--
					<xsl:choose>
						-->
  <!--clam: "or w:basedOn/@w:val='Heading'" added because of bug #1785483-->
  <!--
						<xsl:when test="key('StyleId', $outline)[1][w:pPr/w:outlineLvl/@w:val and (w:basedOn/@w:val='Normal' or w:basedOn/@w:val='Heading')]">
							<xsl:value-of select="key('StyleId', $outline)[1]/w:pPr/w:outlineLvl/@w:val"/>
						</xsl:when>
						<xsl:otherwise/>
					</xsl:choose>
				</xsl:when>
				<xsl:otherwise>
					<xsl:variable name="outline">
						<xsl:value-of select="$node/w:r/w:rPr/w:rStyle/@w:val"/>
					</xsl:variable>
					<xsl:variable name="linkedStyleOutline">
						<xsl:value-of select="key('StyleId', $outline)[1]/w:link/@w:val"/>
					</xsl:variable>
					-->
  <!--if outlineLvl is not defined search in parent style by w:link-->
  <!--
					<xsl:choose>
						<xsl:when test="key('StyleId', $linkedStyleOutline)[1]/w:pPr/w:outlineLvl/@w:val">
							<xsl:value-of
							  select="key('StyleId', $linkedStyleOutline)[1]/w:pPr/w:outlineLvl/@w:val"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="GetOutlineLevelByStyleId">
								<xsl:with-param name="styleId">
									<xsl:value-of select="key('StyleId', $outline)[1]/w:link/@w:val"/>
								</xsl:with-param>
							</xsl:call-template>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:for-each>
	</xsl:template>-->

  <xsl:template name="GetOutlineLevel">
    <xsl:param name="node" select="."/>

    <xsl:variable name="outlineLevel">
      <xsl:choose>
        <xsl:when test="$node/w:pPr/w:pStyle/@w:val">
          <xsl:call-template name="GetOutlineLevelByStyleId">
            <xsl:with-param name="styleId">
              <xsl:value-of select="$node/w:pPr/w:pStyle/@w:val"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <!--if outlineLvl is not defined search in parent style by w:link-->
          <xsl:call-template name="GetOutlineLevelByStyleId">
            <xsl:with-param name="styleId">
              <xsl:value-of select="key('StyleId', $node/w:r/w:rPr/w:rStyle/@w:val)[1]/w:link/@w:val"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="isDefaultHeading">
      <xsl:call-template name="CheckDefaultHeading">
        <xsl:with-param name="Name">
          <xsl:value-of select="key('StyleId',$node/w:pPr/w:pStyle/@w:val)/w:name/@w:val" />
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <!-- a value of 9 means body text, i.e. it is equivalent to no outline level set -->
    <xsl:if test="not($outlineLevel = '' or $outlineLevel = '9') and $isDefaultHeading='true'">
      <xsl:value-of select="$outlineLevel"/>
    </xsl:if>

  </xsl:template>


  <!--math, dialogika: added for bugfix #1802258 BEGIN -->
  <!--Checks recursively whether given style is default heading (must start with Counter = 1)-->
  <xsl:template name="CheckDefaultHeading">
    <xsl:param name="Name" />
    <xsl:param name="Counter" select="1"/>

    <xsl:choose>
      <xsl:when test="$Counter &gt; 9" >false</xsl:when>
      <xsl:when test="concat('heading ',$Counter) = $Name">true</xsl:when>
      <!--math, dialogika: added for bugfix #1792424 BEGIN -->
      <xsl:when test="concat('Heading ',$Counter) = $Name">true</xsl:when>
      <!--math, dialogika: added for bugfix #1792424 END -->
      <xsl:otherwise>
        <xsl:call-template name="CheckDefaultHeading">
          <xsl:with-param name="Name" select="$Name" />
          <xsl:with-param name="Counter">
            <xsl:value-of select="$Counter + 1" />
          </xsl:with-param>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  <!--math, dialogika: added for bugfix #1802258 END -->

  <!--AVSMedia: Wraper for 'writeParagraph' template. Resolves TOC references.-->
  <xsl:template match="w:p">
    <xsl:param name="ftnID"/>
    <xsl:param name="ednID"/>

    <xsl:variable name="instrText" select="w:r/w:instrText"/>

    <xsl:choose>
      <xsl:when test="contains($instrText, 'PAGEREF')">
        <a>
          <xsl:attribute name="href">
            <xsl:value-of select="concat( '#', substring-before(substring-after($instrText,'PAGEREF '),' ') )"/>
          </xsl:attribute>
          <xsl:choose>
            <xsl:when test="count(preceding-sibling::w:p) = 0">
              <xsl:call-template name="writeParagraph">
                <xsl:with-param name="ftnID" select="$ftnID"/>
                <xsl:with-param name="ednID" select="$ednID"/>
              </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
              <xsl:call-template name="writeParagraph"/>
            </xsl:otherwise>
          </xsl:choose>
        </a>
      </xsl:when>
      <xsl:otherwise>
        <xsl:choose>
          <xsl:when test="count(preceding-sibling::w:p) = 0">
            <xsl:call-template name="writeParagraph">
              <xsl:with-param name="ftnID" select="$ftnID"/>
              <xsl:with-param name="ednID" select="$ednID"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="writeParagraph"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- This templates checks several properties of the current paragraphs and then decides whether to create
		a paragraphs, lists or headings by calling apply-templates in mode paragraph, heading or list -->
  <xsl:template name="writeParagraph">
    <xsl:param name="ftnID"/>
    <xsl:param name="ednID"/>

    <xsl:message terminate="no">progress:w:p</xsl:message>

    <xsl:variable name="outlineLevel">
      <xsl:call-template name="GetOutlineLevel">
        <xsl:with-param name="node" select="."/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="numId">
      <xsl:call-template name="GetListProperty">
        <xsl:with-param name="node" select="."/>
        <xsl:with-param name="property">w:numId</xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="ilvl">
      <xsl:call-template name="GetListProperty">
        <xsl:with-param name="node" select="."/>
        <xsl:with-param name="property">w:ilvl</xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="styleId" select="w:pPr/w:pStyle/@w:val"/>

    <!--math, dialogika: added for bugfix #1802258 BEGIN -->
    <xsl:variable name="IsDefaultHeading">
      <xsl:call-template name="CheckDefaultHeading">
        <xsl:with-param name="Name">
          <xsl:value-of select="key('StyleId',$styleId)/w:name/@w:val" />
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>
    <!--math, dialogika: added for bugfix #1802258 END -->


    <!--Only show message if list style numbering inherited from default heading is destroyed-->
    <xsl:if test="$outlineLevel != '' and $IsDefaultHeading != 'true'">

      <xsl:variable name="isParentDefaultHeading">
        <xsl:call-template name="CheckDefaultHeading">
          <xsl:with-param name="Name" select="key('StyleId',key('StyleId',$styleId)/w:basedOn/@w:val)/w:name/@w:val" />
        </xsl:call-template>
      </xsl:variable>

      <xsl:if test="$isParentDefaultHeading = 'true'">
        <xsl:message terminate="no">translation.oox2htm.lists.numbering</xsl:message>
      </xsl:if>
    </xsl:if>

    <xsl:choose>
      
      <!-- TO DO: Тут ранее была отключена работа с TOC (table-of content), в принципе, это правильно, но весь параграф выключать не надо-->
      <!--check if the paragraph starts a table-of content or Bibliography or Alphabetical Index -->
      <!--<xsl:when test="w:r[contains(w:instrText,'TOC') or contains(w:instrText,'BIBLIOGRAPHY') or contains(w:instrText, 'INDEX' )]">
        --><!--TODO: AVSMedia: Currently I see no reason to handle TOC in a some special way, but may be I am wrong.--><!--
        --><!--<xsl:apply-templates select="." mode="tocstart"/>--><!--
      </xsl:when>-->

      <!-- ignore paragraph if it's deleted in change tracking mode-->
      <xsl:when test="key('p', number(@oox:id)-1)/w:pPr/w:rPr/w:del" />

      <!--  check if the paragraph is a list element (it can be a heading but only if it's style is NOT linked to a list level 
				- for linked heading styles there's oultine list style created and they can't be in list (see bug  #1619448)) -->

      <!--math, dialogika: bugfix #1947995 BEGIN
				It seems that outline numbering is only created for heading styles which have a numId defined in styles.xml.
				These headings should not be put into a text:list element and used with the next case (xsl:apply-templates select="." mode="heading")-->
      <!--<xsl:when test="not($outlineLevel != '' and $IsDefaultHeading='true' and key('StyleId',$styleId)/w:pPr/w:numPr/w:numId) 
							and $numId != '' and $ilvl &lt; 10 and key('numId', $numId)/w:abstractNumId/@w:val != ''">-->

      <!--<xsl:when
			      test="not($outlineLevel != '' and not(w:pPr/w:numPr) and $ifNormal='true' and contains($styleId,'Heading')) and $numId != '' and $ilvl &lt; 10 and key('numId', $numId)/w:abstractNumId/@w:val != ''">-->

      <!--math, dialogika: bugfix #1947995 END-->

      <!--xsl:when
						test="$numId != '' and $ilvl &lt; 10 and key('Part', 'word/numbering.xml')/w:numbering/w:num[@w:numId=$numId]/w:abstractNumId/@w:val != '' 
						and not(key('Part', 'word/styles.xml')/w:styles/w:style[@w:styleId = $styleId and child::w:pPr/w:outlineLvl and child::w:pPr/w:numPr/w:numId])"-->

      <!-- divo/20081009: check that we are 
					- in a list
					- maximum at level 9 (OOo does not support more than 10 list levels)
					- and there is a numbering definition for the numId
			-->
      <!--<xsl:when test="$numId != '' and $ilvl &lt; 10 and key('numId', $numId)">-->
      <xsl:when test="$numId != '' and $ilvl &lt; 10">
      <!--<xsl:when test="$numId != '' and $ilvl &lt; 10 and w:pPr/w:numPr">-->
        <xsl:apply-templates select="." mode="list">
          <xsl:with-param name="numId" select="$numId"/>
          <xsl:with-param name="ilvl" select="$ilvl"/>
        </xsl:apply-templates>

      </xsl:when>

      <!--math, dialogika: changed for bugfix #1802258 -->
      <!--check if the paragraph is a heading, only "default headings" are converted to text:h, 
				otherwise a regular text:p will be generated 
			-->
      <xsl:when test="$outlineLevel != '' and $IsDefaultHeading='true'">
        <xsl:apply-templates select="." mode="heading">
          <xsl:with-param name="outlineLevel" select="$outlineLevel"/>
        </xsl:apply-templates>
      </xsl:when>

      <!--  default scenario - paragraph-->
      <xsl:otherwise>
        <xsl:apply-templates select="." mode="paragraph">
          <xsl:with-param name="ftnID" select="$ftnID"/>
          <xsl:with-param name="ednID" select="$ednID"/>
        </xsl:apply-templates>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!--paragraph with outline level is a heading-->
  <xsl:template match="w:p" mode="heading">
    <xsl:param name="outlineLevel"/>
    <xsl:variable name="numId">
      <xsl:call-template name="GetListProperty">
        <xsl:with-param name="node" select="."/>
        <xsl:with-param name="property">w:numId</xsl:with-param>
      </xsl:call-template>
    </xsl:variable>


    <xsl:choose>
      <xsl:when test="ancestor::w:txbxContent">
        <p>
          <!--style name-->          
          <xsl:if test="w:pPr/w:pStyle/@w:val or w:r/w:br[@w:type='page' or @w:type='column']">
            <xsl:if test="w:pPr/w:pStyle/@w:val">
              <xsl:attribute name="class">
                <xsl:call-template name="InsertStyleNameInTeg">
                  <xsl:with-param name="styleName" select="w:pPr/w:pStyle/@w:val"/>
                </xsl:call-template>
              </xsl:attribute>
            </xsl:if>
          </xsl:if>
          
          <!--header outline level -->
          <xsl:call-template name="InsertHeadingOutlineLvl">
            <xsl:with-param name="outlineLevel" select="$outlineLevel"/>
          </xsl:call-template>

          <!-- Специальная ветка для CKEditor. -->
          <xsl:if test="$HtmlMode = 'CKEditor'">
            <xsl:text disable-output-escaping="yes">&lt;span </xsl:text>
            <xsl:text disable-output-escaping="yes">style="</xsl:text>

            <xsl:choose>
              <xsl:when test="./w:pPr">
                <xsl:for-each select="w:pPr">
                  <xsl:call-template name="InsertSpanProperties"/>
                </xsl:for-each>
              </xsl:when>
              <xsl:otherwise>
                <xsl:call-template name="InsertDefaultSpanProperties" />
              </xsl:otherwise>
            </xsl:choose>

            <xsl:text>"</xsl:text>
            <xsl:text disable-output-escaping="yes">&gt;</xsl:text>
          </xsl:if>          
          
          <!--<xsl:text disable-output-escaping="yes">&lt;span style=&quot;visibility:hidden&quot;&gt;&#8204;&lt;/span&gt;</xsl:text>-->
          <!-- unnumbered heading is list header  -->
          <xsl:call-template name="InsertHeadingAsListHeader"/>
          <!--change track end-->
          <xsl:if test="key('p', number(@oox:id)-1)/w:pPr/w:rPr/w:ins and $numId!=''">
            <text:change-end>
              <xsl:attribute name="text:change-id">
                <xsl:value-of select="generate-id(key('p', number(@oox:id)-1))"/>
              </xsl:attribute>
            </text:change-end>
          </xsl:if>
          <xsl:apply-templates/>
          <xsl:if test="w:pPr/w:rPr/w:del">
            <!--      if this following paragraph is attached to this one in track changes mode-->
            <xsl:call-template name="InsertDeletedParagraph"/>
          </xsl:if>
          <xsl:if test="w:pPr/w:rPr/w:ins">
            <text:change-start>
              <xsl:attribute name="text:change-id">
                <xsl:value-of select="generate-id(self::node())"/>
              </xsl:attribute>
            </text:change-start>
          </xsl:if>

          <!-- Проверяем, возможно тегов с текстом вообще нет в параграфе, тогда добавляем неразрывный пробел-->
          <xsl:if test="count(descendant::w:t | descendant::w:instrText) = 0">
            <xsl:text>&#160;</xsl:text>
          </xsl:if>

          <!-- Специальная ветка для CKEditor. -->
          <xsl:if test="$HtmlMode = 'CKEditor'">
            <xsl:text disable-output-escaping="yes">&lt;/span&gt;</xsl:text>
          </xsl:if>
        </p>
      </xsl:when>
      <xsl:otherwise>
        <!--<h3>-->
        <!--<p class="Heading1">-->
        <p>
          <!--style name-->
          <xsl:if test="w:pPr or w:r/w:br[@w:type='page' or @w:type='column']">
            <xsl:if test="w:pPr/w:pStyle/@w:val">
              <xsl:attribute name="class">
                <xsl:call-template name="InsertStyleNameInTeg">
                  <xsl:with-param name="styleName" select="w:pPr/w:pStyle/@w:val"/>
                </xsl:call-template>
              </xsl:attribute>
            </xsl:if>
          </xsl:if>
          
          <!--clam, dialogika: bugfix #2088822-->
          <xsl:variable name="myNum" select="key('Part', 'word/numbering.xml')/w:numbering/w:num[@w:numId = $numId]"></xsl:variable>
          <xsl:if test="$myNum/w:lvlOverride[@w:ilvl=$outlineLevel]/w:startOverride">
            <xsl:attribute name="text:restart-numbering">true</xsl:attribute>
            <xsl:attribute name="text:start-value">
              <xsl:value-of select="$myNum/w:lvlOverride[@w:ilvl=$outlineLevel]/w:startOverride/@w:val"/>
            </xsl:attribute>
          </xsl:if>

          <!--header outline level -->
          <xsl:call-template name="InsertHeadingOutlineLvl">
            <xsl:with-param name="outlineLevel" select="$outlineLevel"/>
          </xsl:call-template>

          <!-- Специальная ветка для CKEditor. -->
          <xsl:if test="$HtmlMode = 'CKEditor'">
            <xsl:text disable-output-escaping="yes">&lt;span </xsl:text>
            <xsl:text disable-output-escaping="yes"> heading="</xsl:text>
            <xsl:choose>
              <xsl:when test="$outlineLevel &lt; 0" >
                <xsl:text>h1</xsl:text>               
              </xsl:when>
              <xsl:when test="$outlineLevel &gt; 5" >
                <xsl:text>h6</xsl:text>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="concat('h',format-number(($outlineLevel+1), 0))"/>
              </xsl:otherwise>
            </xsl:choose>
            <xsl:text disable-output-escaping="yes">"</xsl:text>
            <xsl:text disable-output-escaping="yes"> style="</xsl:text>

            <xsl:choose>
              <xsl:when test="./w:pPr">
                <xsl:for-each select="w:pPr">
                  <xsl:call-template name="InsertSpanProperties"/>
                </xsl:for-each>
              </xsl:when>
              <xsl:otherwise>
                <xsl:call-template name="InsertDefaultSpanProperties" />
              </xsl:otherwise>
            </xsl:choose>

            <xsl:text>"</xsl:text>
            <xsl:text disable-output-escaping="yes">&gt;</xsl:text>
          </xsl:if>          
          
          <!--<xsl:text disable-output-escaping="yes">&lt;span style=&quot;visibility:hidden&quot;&gt;&#8204;&lt;/span&gt;</xsl:text>-->
          <!-- unnumbered heading is list header  -->
          <xsl:call-template name="InsertHeadingAsListHeader"/>
          <!--change track end-->
          <xsl:if test="key('p', number(@oox:id)-1)/w:pPr/w:rPr/w:ins and $numId!=''">
            <text:change-end>
              <xsl:attribute name="text:change-id">
                <xsl:value-of select="generate-id(key('p', number(@oox:id)-1))"/>
              </xsl:attribute>
            </text:change-end>
          </xsl:if>
          <xsl:apply-templates/>
          <!--<xsl:apply-templates select="./w:*"/>-->
          <xsl:if test="w:pPr/w:rPr/w:del">
            <!--      if this following paragraph is attached to this one in track changes mode-->
            <xsl:call-template name="InsertDeletedParagraph"/>
          </xsl:if>
          <xsl:if test="w:pPr/w:rPr/w:ins">
            <text:change-start>
              <xsl:attribute name="text:change-id">
                <xsl:value-of select="generate-id(self::node())"/>
              </xsl:attribute>
            </text:change-start>
          </xsl:if>

          <!-- Проверяем, возможно тегов с текстом вообще нет в параграфе, тогда добавляем неразрывный пробел-->
          <xsl:if test="count(descendant::w:t | descendant::w:instrText) = 0">
            <xsl:text>&#160;</xsl:text>
          </xsl:if>

          <!-- Специальная ветка для CKEditor. -->
          <xsl:if test="$HtmlMode = 'CKEditor'">
            <xsl:text disable-output-escaping="yes">&lt;/span&gt;</xsl:text>
          </xsl:if>
        </p>
        <!--</h3>-->
      </xsl:otherwise>
    </xsl:choose>


    <xsl:if test="w:pPr/w:rPr/w:ins and $numId=''">
      <text:change-end>
        <xsl:attribute name="text:change-id">
          <xsl:value-of select="generate-id(self::node())"/>
        </xsl:attribute>
      </text:change-end>
    </xsl:if>
  </xsl:template>

  <xsl:template name="InsertDeletedParagraph">
    <text:change>
      <xsl:attribute name="text:change-id">
        <xsl:value-of select="generate-id(w:pPr/w:rPr)"/>
      </xsl:attribute>
    </text:change>
    <xsl:apply-templates select="key('p', @oox:id+1)/child::node()"/>
    <xsl:if test="key('p', @oox:id+1)/w:pPr/w:rPr/w:del">
      <xsl:for-each select="key('p', @oox:id+1)">
        <xsl:call-template name="InsertDeletedParagraph"/>
      </xsl:for-each>
    </xsl:if>
  </xsl:template>

  <!--  set heading as list header (needed when number was deleted manually)-->
  <xsl:template name="InsertHeadingAsListHeader">
    <xsl:variable name="numId">
      <xsl:call-template name="GetListProperty">
        <xsl:with-param name="node" select="."/>
        <xsl:with-param name="property">w:numId</xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <!--check if there's a any numId in document-->
    <xsl:for-each select="key('Part', 'word/document.xml')">
      <xsl:choose>
        <xsl:when test="key('numPr','')/w:ins"/>
        <xsl:when test="key('numPr', '')/w:numId">
          <xsl:call-template name="InsertListHeader">
            <xsl:with-param name="numId" select="$numId"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <!--check if there's a any numId in styles-->
          <xsl:for-each select="key('Part', 'word/styles.xml')">
            <xsl:if test="key('numPr', '')/w:numId">
              <xsl:call-template name="InsertListHeader">
                <xsl:with-param name="numId" select="$numId"/>
              </xsl:call-template>
            </xsl:if>
          </xsl:for-each>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:for-each>
  </xsl:template>

  <!--  set heading as list header (needed when number was deleted manually)-->
  <xsl:template name="InsertListHeader">
    <xsl:param name="numId"/>
    <xsl:for-each select="key('Part', 'word/numbering.xml')">
      <xsl:if test="not(key('numId', $numId))">
        <xsl:attribute name="text:is-list-header">
          <xsl:text>true</xsl:text>
        </xsl:attribute>
      </xsl:if>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="InsertHeadingOutlineLvl">
    <!--TODO: Paragraph outline is implemented through adding
            'style' attribute, since it is the only case in which
            such an attribute may be added to paragraph itself.
            Should be reworked if the other cases will appear.-->
    <xsl:param name="outlineLevel"/>
    <xsl:attribute name="style">

      <xsl:if test="./w:pPr and ./w:pPr/w:jc">
        <xsl:apply-templates select="./w:pPr/w:jc" mode="pPrChildren"/>
      </xsl:if>
      
      <xsl:variable name="headingLvl">
        <xsl:call-template name="GetListProperty">
          <xsl:with-param name="node" select="."/>
          <xsl:with-param name="property">w:ilvl</xsl:with-param>
        </xsl:call-template>
      </xsl:variable>
      <xsl:text>outline-width:</xsl:text>
      <xsl:choose>
        <xsl:when test="$headingLvl != ''">
          <xsl:value-of select="$headingLvl+1"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$outlineLevel+1"/>
        </xsl:otherwise>
      </xsl:choose>
      <xsl:text>; </xsl:text>
    </xsl:attribute>
  </xsl:template>

  <!--  paragraphs-->
  <xsl:template match="w:p" mode="paragraph">
    <xsl:param name="ftnID"/>
    <xsl:param name="ednID"/>

    <xsl:choose>
      
      <!-- Обрабатываем вложенные параграфы -->
      <xsl:when test="parent::w:p">
        <xsl:apply-templates select="child::node()"/>
      </xsl:when>
      
      <!-- default scenario -->
      <xsl:otherwise>

        <xsl:variable name="numId">
          <xsl:call-template name="GetListProperty">
            <xsl:with-param name="node" select="."/>
            <xsl:with-param name="property">w:numId</xsl:with-param>
          </xsl:call-template>
        </xsl:variable>

        <xsl:variable name="paragraphStyle">
          <xsl:for-each select="w:pPr">
            <xsl:call-template name="InsertParagraphProperties"/>
          </xsl:for-each>
        </xsl:variable>

        <xsl:variable name="divStyle">
          <xsl:for-each select="w:pPr">
            <xsl:call-template name="InsertDivProperties"/>
          </xsl:for-each>
        </xsl:variable>

        <!-- Специальная ветка для CKEditor. -->
        <xsl:if test="$HtmlMode = 'CKEditor'">

          <!-- Если в свойствах параграфа есть w:pageBreakBefore, тогда вставим здесь разрыв страницы -->
          <xsl:for-each select="w:pPr">
            <xsl:if test="w:pageBreakBefore and ( not(w:pageBreakBefore/@w:val) or w:pageBreakBefore/@w:val = 'true' or w:pageBreakBefore/@w:val = 'on' or w:pageBreakBefore/@w:val = 1 )">

              <img>

                <xsl:attribute name="align">
                  <xsl:value-of select="''"/>
                </xsl:attribute>

                <xsl:attribute name="aria-label">
                  <xsl:value-of select="'Page Break'"/>
                </xsl:attribute>

                <xsl:attribute name="class">
                  <xsl:value-of select="'cke_pagebreak'"/>
                </xsl:attribute>

                <xsl:attribute name="data-cke-realelement">
                  <xsl:value-of select="'%3Cdiv%20style%3D%22page-break-after%3A%20always%3B%22%3E%3Cspan%20style%3D%22display%3A%20none%3B%22%3E%26nbsp%3B%3C%2Fspan%3E%3C%2Fdiv%3E'" />
                </xsl:attribute>

                <!-- Когда CKEditor встретит этот Guid, он заменит его не нормальный путь -->
                <xsl:attribute name="src">
                  <xsl:value-of select="'{C03A67CC-3827-4363-84BB-D095B6B60120}'"/>
                </xsl:attribute>

              </img>

            </xsl:if>
          </xsl:for-each>
        </xsl:if>


          <div>
          <xsl:if test="$divStyle != ''">
            <xsl:attribute name="style">
              <xsl:value-of select="$divStyle"/>
            </xsl:attribute>
          </xsl:if>
          <p>

            <!-- Reference the style -->
            <xsl:if test="w:pPr or w:r/w:br[@w:type='page' or @w:type='column'] or key('Part', 'word/styles.xml')/w:styles/w:docDefaults/w:pPrDefault">
              <xsl:if test="w:pPr/w:pStyle/@w:val or ( ./w:r/w:ptab/@w:alignment = 'right' and ./w:pPr/w:pStyle/@w:val = 'Footer' )">
                <xsl:attribute name="class">
                  <xsl:choose>
                    <xsl:when test="./w:r/w:ptab/@w:alignment = 'right' and ./w:pPr/w:pStyle/@w:val = 'Footer'">
                      <xsl:text>X3AS7TABSTYLE</xsl:text>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:call-template name="InsertStyleNameInTeg">
                        <xsl:with-param name="styleName" select="w:pPr/w:pStyle/@w:val"/>
                      </xsl:call-template>
                    </xsl:otherwise>
                  </xsl:choose>
                </xsl:attribute>
              </xsl:if>
            </xsl:if>

            <xsl:if test="$paragraphStyle != ''">
              <xsl:attribute name="style">
                <xsl:value-of select="$paragraphStyle"/>
              </xsl:attribute>
            </xsl:if>

            <!-- Специальная ветка для CKEditor. -->
            <xsl:if test="$HtmlMode = 'CKEditor'">

              <xsl:text disable-output-escaping="yes">&lt;span </xsl:text>
              <xsl:text disable-output-escaping="yes">style="</xsl:text>

              <xsl:choose>
                <xsl:when test="./w:pPr">
                  <xsl:for-each select="w:pPr">
                    <xsl:call-template name="InsertSpanProperties"/>
                  </xsl:for-each>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:call-template name="InsertDefaultSpanProperties"/>
                </xsl:otherwise>
              </xsl:choose>
              
              <xsl:text>"</xsl:text>
              <xsl:text disable-output-escaping="yes">&gt;</xsl:text>
            </xsl:if>

            <xsl:choose>
              <xsl:when test="$ftnID != ''">
                <xsl:if test="$ftnID != ''">
                  <a name="{concat('ftn',../@w:id)}" href="{concat('#ftnref',../@w:id)}">
                    <xsl:apply-templates select="child::*[position() = 2]"/>
                  </a>
                  <xsl:apply-templates select="child::*[position() != 2]"/>

                </xsl:if>
              </xsl:when>
              <xsl:when test="$ednID != ''">
                <xsl:if test="$ednID != ''">
                  <a name="{concat('edn',../@w:id)}" href="{concat('#ednref',../@w:id)}">
                    <xsl:apply-templates select="child::*[position() = 2]"/>
                  </a>
                  <xsl:apply-templates select="child::*[position() != 2]"/>
                </xsl:if>
              </xsl:when>
              <xsl:otherwise>
                <!--<xsl:value-of select="name(.)"/>-->
                <xsl:apply-templates/>
              </xsl:otherwise>
            </xsl:choose>

            <xsl:if test="w:pPr/w:rPr/w:del">
              <xsl:call-template name="InsertDeletedParagraph"/>
            </xsl:if>

            <!--<xsl:if test="not(w:r)">
										<xsl:text disable-output-escaping="yes">&lt;br/&gt;</xsl:text>
									</xsl:if>-->

            <!-- Проверяем, возможно тегов с текстом вообще нет в параграфе, тогда добавляем неразрывный пробел-->
            <xsl:if test="count(descendant::w:t | descendant::w:instrText) = 0">
              <xsl:text>&#160;</xsl:text>
            </xsl:if>

            <!-- Специальная ветка для CKEditor. -->
            <xsl:if test="$HtmlMode = 'CKEditor'">
              <xsl:text disable-output-escaping="yes">&lt;/span&gt;</xsl:text>
            </xsl:if>

          </p>
        </div>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- Detect if there is text before a break page in a paragraph -->
  <!-- BUG 1583404 - Page breaks not correct converted - 17/07/2007-->
  <xsl:template match="w:r[w:br[@w:type='page']]">
    <xsl:call-template name="writeRun"/>
    <!--<xsl:apply-templates select="w:t"/>-->
  </xsl:template>

  <!--tabs-->
  <xsl:template match="w:tab[not(parent::w:tabs)]">

    <xsl:choose>
      <xsl:when test="ancestor::w:footnote or ancestor::w:endnote">
        <xsl:variable name="StyleId">
          <xsl:value-of select="ancestor::w:p/w:pPr/w:pStyle/@w:val"/>
        </xsl:variable>
        <xsl:choose>
          <xsl:when
            test="generate-id(.) = generate-id(ancestor::w:p/descendant::w:tab[1]) and (ancestor::w:p/w:pPr/w:ind/@w:hanging != '' or key('StyleId', $StyleId)/w:pPr/w:ind/@w:hanging != '')"/>
          <xsl:otherwise>
            <span style="mso-tab-count:1">&#160;&#160;</span>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:otherwise>
        <xsl:variable name="PrevTabsCount" select="count(../../preceding-sibling::w:r/w:tab)"/>
        <xsl:variable name="Value">
          
        </xsl:variable>
        <span style="mso-tab-count:1">&#160;&#160;</span>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <!--clam, dialogika: bugfix #1803097-->
  <!--<xsl:template match="w:ptab[@w:relativeTo='margin' and @w:alignment='right' and @w:leader='none']">-->
  <xsl:template match="w:ptab">
    <span style="mso-tab-count:1">&#160;&#160;</span>
  </xsl:template>

  <!-- run -->

  <xsl:template match="w:r">
    <xsl:param name="ignoreFieldFlag" />

    <xsl:call-template name="writeRun">
      <xsl:with-param name="ignoreFieldFlag" select="$ignoreFieldFlag"/>
    </xsl:call-template>

  </xsl:template>


  <xsl:template name="writeRun">
    <xsl:param name="ignoreFieldFlag" />
    <xsl:param name="toFirstItemInParagraph"/>

    <!-- Специальная ветка для CKEditor:
         Такой спан обязательно надо вставить для w:r, у которых вначале не идет w:rPr. -->
    <xsl:if test="$HtmlMode = 'CKEditor'">
      <xsl:text disable-output-escaping="yes">&lt;span </xsl:text>
      <xsl:text disable-output-escaping="yes">style="</xsl:text>
      <xsl:call-template name="InsertSpanProperties2"/>
      <xsl:text disable-output-escaping="yes">"&gt;</xsl:text>
    </xsl:if>

    <xsl:message terminate="no">progress:w:r</xsl:message>
    <xsl:choose>

      <!--fields-->
      <!--xsl:when test="preceding::w:fldChar[1][@w:fldCharType='begin' or @w:fldCharType='separate']"-->
      <xsl:when test="@oox:f and $ignoreFieldFlag!='true'">
        <xsl:call-template name="InsertField"/>
      </xsl:when>

      <!-- Comments -->
      <xsl:when test="w:commentReference/@w:id">
        <xsl:call-template name="InsertComment">
          <xsl:with-param name="Id">
            <xsl:value-of select="w:commentReference/@w:id"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>

      <!--  Track changes    -->
      <xsl:when test="parent::w:del">
        <xsl:call-template name="TrackChangesDeleteMade"/>
      </xsl:when>
      <xsl:when test="parent::w:ins">
        <xsl:call-template name="TrackChangesInsertMade"/>
      </xsl:when>
      <xsl:when test="descendant::w:rPrChange">
        <xsl:call-template name="TrackChangesChangesMade"/>
      </xsl:when>

      <!-- attach automatic style-->
      <xsl:when test="w:rPr[not(count(child::node())=1 and child::w:noProof)]">
        <xsl:text disable-output-escaping="yes">&lt;span</xsl:text>
        <xsl:if test="w:rPr/w:rStyle">
          <xsl:text> class="</xsl:text>
          <xsl:call-template name="InsertStyleNameInTeg">
            <xsl:with-param name="styleName" select="w:rPr/w:rStyle/@w:val"/>
          </xsl:call-template>
          <!--<xsl:value-of select="w:rPr/w:rStyle/@w:val"/>-->
          <xsl:text>"</xsl:text>
        </xsl:if>
        <xsl:variable name="textStyle">
          <xsl:call-template name="InsertTextProperties"/>
        </xsl:variable>
        <xsl:if test="$textStyle != ''">
          <!--<xsl:attribute name="style">-->
          <xsl:text disable-output-escaping="yes"> style=&quot;</xsl:text>
          <xsl:value-of select="$textStyle"/>
          <xsl:text disable-output-escaping="yes">&quot;</xsl:text>
          <!--<xsl:call-template name="InsertTextProperties"/>-->
          <!--</xsl:attribute>-->
        </xsl:if>

        <xsl:variable name="Bold">
          <xsl:choose>
            <xsl:when test="w:rPr/w:b and (not(w:rPr/w:b/@w:val) or (w:rPr/w:b/@w:val != 'false' and w:rPr/w:b/@w:val != '0') )">
              <xsl:text>true</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>false</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>

        <xsl:variable name="Italic">
          <xsl:choose>
            <xsl:when test="w:rPr/w:i and (not(w:rPr/w:i/@w:val) or (w:rPr/w:i/@w:val != 'false' and w:rPr/w:i/@w:val != '0') )">
              <xsl:text>true</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>false</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>

        <xsl:variable name="Underline">
          <xsl:choose>
            <xsl:when test="w:rPr/w:u and (not(w:rPr/w:u/@w:val) or (w:rPr/w:u/@w:val != 'false' and w:rPr/w:u/@w:val != '0' and w:rPr/w:u/@w:val != 'none') )">
              <xsl:text>true</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>false</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>

        <xsl:variable name="Subscript">
          <xsl:choose>
            <xsl:when test="w:rPr/w:vertAlign/@w:val = 'subscript'">
              <xsl:text>true</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>false</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>

        <xsl:variable name="Superscript">
          <xsl:choose>
            <xsl:when test="w:rPr/w:vertAlign/@w:val = 'superscript'">
              <xsl:text>true</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>false</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>

        <xsl:text disable-output-escaping="yes">&gt;</xsl:text>
        <xsl:choose>
          <xsl:when test="$HtmlMode = 'CKEditor'">

            <!-- Вариант, когда для стилей шрифта Bold, Italic, Underline, писались отдельные тэги <b> <i> <u> -->
            <!--<xsl:if test="$Bold = 'true'">
              <xsl:text disable-output-escaping="yes">&lt;b&gt;</xsl:text>
            </xsl:if>

            <xsl:if test="$Italic = 'true'">
              <xsl:text disable-output-escaping="yes">&lt;i&gt;</xsl:text>
            </xsl:if>

            <xsl:if test="$Underline = 'true'">
              <xsl:text disable-output-escaping="yes">&lt;u&gt;</xsl:text>
            </xsl:if>

            <xsl:call-template name="writeR"/>

            <xsl:if test="$Underline = 'true'">
              <xsl:text disable-output-escaping="yes">&lt;/u&gt;</xsl:text>
            </xsl:if>

            <xsl:if test="$Italic = 'true'">
              <xsl:text disable-output-escaping="yes">&lt;/i&gt;</xsl:text>
            </xsl:if>

            <xsl:if test="$Bold = 'true'">
              <xsl:text disable-output-escaping="yes">&lt;/b&gt;</xsl:text>
            </xsl:if>-->

            <xsl:text disable-output-escaping="yes">&lt;span </xsl:text>
            <xsl:text disable-output-escaping="yes"> style=&quot;</xsl:text>
            <xsl:if test="$Bold = 'true'">
              <xsl:text disable-output-escaping="yes">font-weight:bold;</xsl:text>
            </xsl:if>

            <xsl:if test="$Italic = 'true'">
              <xsl:text disable-output-escaping="yes">font-style:italic;</xsl:text>
            </xsl:if>

            <xsl:if test="$Underline = 'true'">
              <xsl:text disable-output-escaping="yes">text-decoration:underline;</xsl:text>
            </xsl:if>
            <xsl:text disable-output-escaping="yes">&quot;</xsl:text>
            <xsl:text disable-output-escaping="yes">&gt;</xsl:text>

            <xsl:if test="$Subscript = 'true'">
              <xsl:text disable-output-escaping="yes">&lt;sub&gt;</xsl:text>
            </xsl:if>

            <xsl:if test="$Superscript = 'true'">
              <xsl:text disable-output-escaping="yes">&lt;sup&gt;</xsl:text>
            </xsl:if>

            <xsl:call-template name="writeR"/>

            <xsl:if test="$Superscript = 'true'">
              <xsl:text disable-output-escaping="yes">&lt;/sup&gt;</xsl:text>
            </xsl:if>

            <xsl:if test="$Subscript = 'true'">
              <xsl:text disable-output-escaping="yes">&lt;/sub&gt;</xsl:text>
            </xsl:if>

            <xsl:text disable-output-escaping="yes">&lt;/span&gt;</xsl:text>

          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="writeR"/>
          </xsl:otherwise>
        </xsl:choose>
        <xsl:text disable-output-escaping="yes">&lt;/span&gt;</xsl:text>
      </xsl:when>

      <!--default scenario-->
      <xsl:otherwise>
        <xsl:call-template name="writeR"/>
      </xsl:otherwise>
    </xsl:choose>
    
    <!-- Специальная ветка для CKEditor. -->
    <xsl:if test="$HtmlMode = 'CKEditor'">
      <xsl:text disable-output-escaping="yes">&lt;/span&gt;</xsl:text>
    </xsl:if>

  </xsl:template>

  <xsl:template name="InsertStyleNameInTeg">
    <xsl:param name="styleName"/>

    <xsl:if test="$styleName != ''">
      <xsl:call-template name="InsertStyleNameInTeg">
        <xsl:with-param name="styleName">
          <xsl:value-of select="key('Part', 'word/styles.xml')/w:styles/w:style[@w:styleId = $styleName]/w:basedOn/@w:val"/>
        </xsl:with-param>
      </xsl:call-template>

      <xsl:value-of select="concat('style_', $styleName, ' ')"/>
      <xsl:text>style_</xsl:text>
      <xsl:call-template name="DelSpaces">
        <xsl:with-param name="str" select="$styleName"/>
      </xsl:call-template>
      <xsl:text> </xsl:text>

    </xsl:if>
  </xsl:template>


  <xsl:template name="writeR">
    <xsl:choose>
      <xsl:when test="w:footnoteReference">
        <a xmlns="http://www.w3.org/1999/xhtml" name="{concat('ftnref',w:footnoteReference/@w:id)}" href="{concat('#ftn',w:footnoteReference/@w:id)}">
          <xsl:apply-templates/>
        </a>
      </xsl:when>
      <xsl:when test="w:endnoteReference">
        <a xmlns="http://www.w3.org/1999/xhtml" name="{concat('ednref',w:endnoteReference/@w:id)}" href="{concat('#edn',w:endnoteReference/@w:id)}">
          <xsl:apply-templates/>
        </a>
      </xsl:when>
      <xsl:otherwise>
        <xsl:apply-templates/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- path for hyperlinks-->
  <xsl:template name="GetLinkPath">
    <xsl:param name="linkHref"/>
    <xsl:choose>
      <xsl:when test="contains($linkHref, 'file:///') or contains($linkHref, 'http://') or contains($linkHref, 'https://') or contains($linkHref, 'mailto:')">
        <xsl:value-of select="$linkHref"/>
      </xsl:when>
      <xsl:when test="contains($linkHref,'#')">
        <xsl:value-of select="$linkHref"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="concat('../',$linkHref)"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- hyperlinks -->
  <xsl:template match="w:hyperlink">
    <a xmlns="http://www.w3.org/1999/xhtml">
      <xsl:attribute name="href">

        <!--file or web page hyperlink with relationship id -->
        <xsl:if test="@r:id">
          <xsl:variable name="relationshipId">
            <xsl:value-of select="@r:id"/>
          </xsl:variable>

          <xsl:variable name="document">
            <xsl:variable name="part">
              <xsl:value-of select="ancestor::oox:part/@oox:name"/>
            </xsl:variable>
            <xsl:value-of select="substring-after($part, '/')"/>
          </xsl:variable>

          <xsl:for-each select="key('Part', concat('word/_rels/',$document,'.rels'))//node()[name() = 'Relationship']">
            <xsl:if test="./@Id=$relationshipId">
              <xsl:call-template name="GetLinkPath">
                <xsl:with-param name="linkHref" select="@Target"/>
              </xsl:call-template>
            </xsl:if>
          </xsl:for-each>
        </xsl:if>
        <!--document hyperlink -->
        <xsl:if test="@w:anchor">
          <xsl:value-of select="concat('#',@w:anchor)"/>
        </xsl:if>
      </xsl:attribute>

      <!--hyperlink text-->
      <xsl:apply-templates select="w:r">
        <xsl:with-param name="ignoreFieldFlag" select="'true'" />
      </xsl:apply-templates>
    </a>

  </xsl:template>

  <!-- bookmarks -->
  <xsl:template match="w:bookmarkStart">

    <xsl:choose>
      <xsl:when test="$HtmlMode = 'CKEditor'"/>
      <xsl:otherwise>
        <!--<xsl:variable name="isHrefToThis">
			<xsl:call-template name="isHrefToThisBookmark">
				<xsl:with-param name="name" select="@w:name"/>
			</xsl:call-template>
		</xsl:variable>-->
        <xsl:variable name="name">
          <xsl:value-of select="@w:name"/>
        </xsl:variable>
        <xsl:if test="count(preceding::w:hyperlink [@w:anchor = $name]) != 0 or  count(following/w:hyperlink [@w:anchor = $name]) != 0">


          <a xmlns="http://www.w3.org/1999/xhtml">
            <xsl:attribute name="name">
              <xsl:value-of select="$name" />
            </xsl:attribute>
            <xsl:text />
          </a>
        </xsl:if>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template match="w:BookmarkEnd">

  </xsl:template>

  <!-- simple text  -->
  <!--dialogika, clam: span added for hyperlinks (bug #1827518) -->

  <xsl:template name="DrawText">
    <xsl:param name="CurNode" />

    <xsl:message terminate="no">progress:w:t</xsl:message>
    <xsl:call-template name="InsertDropCapText"/>

    <xsl:choose>
      <!--check whether string contains whitespace sequence-->
      <xsl:when test="not( contains( $CurNode/text(), '  ' ) or
                                 substring( $CurNode/text(), 1, 1 ) = ' ' or
                                 substring( $CurNode/text(), string-length( $CurNode/text() ), 1 ) = ' ' )">
        <xsl:choose>
          <xsl:when test="$CurNode/../w:rPr/w:rStyle/@w:val = 'Hyperlink' and $CurNode/../w:rPr/w:color">
            <span class="{generate-id($CurNode/..)}">
              <xsl:value-of select="$CurNode/text()"/>
            </span>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$CurNode/text()"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:otherwise>
        <!--converts whitespaces sequence to text:s-->
        <xsl:choose>
          <xsl:when test="$CurNode/../w:rPr/w:rStyle/@w:val = 'Hyperlink' and $CurNode/../w:rPr/w:color">
            <span class="{generate-id($CurNode/..)}">
              <xsl:call-template name="InsertWhiteSpaces"/>
            </span>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="InsertWhiteSpaces"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>
  
  <xsl:template match="w:t">
    <xsl:message terminate="no">progress:w:t</xsl:message>

    <xsl:variable name="FldLvl">
      <xsl:value-of select="count(preceding::w:fldChar[@w:fldCharType = 'begin']) - count(preceding::w:fldChar[@w:fldCharType = 'end'])" />
    </xsl:variable>

    <!--<xsl:variable name="spacePreserve">
      <xsl:value-of select="@xml:space" />
    </xsl:variable>-->

    <xsl:choose>


      <xsl:when test="$FldLvl = 0">

        <xsl:call-template name="InsertDropCapText"/>
        <xsl:choose>
        <!--check whether string contains whitespace sequence-->
        <xsl:when test="not( contains( ., '  ' ) or
                                 substring( ., 1, 1 ) = ' ' or
                                 substring( ., string-length( . ), 1 ) = ' ' )">
          <xsl:choose>
            <xsl:when test="../w:rPr/w:rStyle/@w:val = 'Hyperlink' and ../w:rPr/w:color">
              <span class="{generate-id(..)}">
                <xsl:value-of select="."/>                
              </span>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="."/>
            </xsl:otherwise>   
          </xsl:choose>
        </xsl:when>
        <xsl:otherwise>
          <!--converts whitespaces sequence to text:s-->
          <xsl:choose>
            <xsl:when test="../w:rPr/w:rStyle/@w:val = 'Hyperlink' and ../w:rPr/w:color">
              <span class="{generate-id(..)}">
                <xsl:call-template name="InsertWhiteSpaces"/>
              </span>
            </xsl:when>
            <!--<xsl:when test="$spacePreserve = 'preserve'">
              <pre>
                <xsl:call-template name="InsertWhiteSpaces"/>
              </pre>
            </xsl:when>-->
            <xsl:otherwise>
              <xsl:call-template name="InsertWhiteSpaces"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:otherwise>
      </xsl:choose>
      </xsl:when>
      <xsl:otherwise>


        <xsl:variable name="List" select="preceding::w:fldChar[@w:fldCharType = 'begin'] | preceding::w:fldChar[@w:fldCharType = 'separate']" />

        <xsl:variable name="Num">

          <xsl:call-template name="FindFldBegin">
            <xsl:with-param name="List" select="$List"/>
            <xsl:with-param name="FldLvl" select="$FldLvl" />
            <xsl:with-param name="Num" select="count($List)" />
          </xsl:call-template>

        </xsl:variable>

        <xsl:if test="$Num != -1">

          <xsl:variable name="TextNode" select="." />
          <xsl:for-each select="$List">

            <xsl:if test="position() = $Num">
              <xsl:call-template name="ProcessField">
                <xsl:with-param name="CurNode" select="current()" />
                <xsl:with-param name="TextNode" select="$TextNode" />
              </xsl:call-template>
            </xsl:if>

          </xsl:for-each>

        </xsl:if>

      </xsl:otherwise>

    </xsl:choose>
  </xsl:template>

  <xsl:template match="w:instrText[ text() != '' ]">
    <xsl:message terminate="no">progress:w:instrText</xsl:message>

    <xsl:variable name="FldLvl">
      <xsl:value-of select="count(preceding::w:fldChar[@w:fldCharType = 'begin']) - count(preceding::w:fldChar[@w:fldCharType = 'end'])" />
    </xsl:variable>

    <xsl:choose>


      <xsl:when test="$FldLvl = 0">
        <xsl:call-template name="InsertDropCapText"/>
        <xsl:choose>
          <!--check whether string contains whitespace sequence-->
          <xsl:when test="not( contains( ., '  ' ) or
                                 substring( ., 1, 1 ) = ' ' or
                                 substring( ., string-length( . ), 1 ) = ' ' )">
            <xsl:choose>
              <xsl:when test="../w:rPr/w:rStyle/@w:val = 'Hyperlink' and ../w:rPr/w:color">
                <span class="{generate-id(..)}">
                  <xsl:value-of select="."/>
                </span>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="."/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:when>
          <xsl:otherwise>
            <!--converts whitespaces sequence to text:s-->
            <xsl:choose>
              <xsl:when test="../w:rPr/w:rStyle/@w:val = 'Hyperlink' and ../w:rPr/w:color">
                <span class="{generate-id(..)}">
                  <xsl:call-template name="InsertWhiteSpaces"/>
                </span>
              </xsl:when>
              <xsl:otherwise>
                <xsl:call-template name="InsertWhiteSpaces"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:otherwise>

        
        <xsl:variable name="List" select="preceding::w:fldChar[@w:fldCharType = 'begin'] | preceding::w:fldChar[@w:fldCharType = 'separate']" />

        <xsl:variable name="Num">

          <xsl:call-template name="FindFldBegin">
            <xsl:with-param name="List" select="$List"/>
            <xsl:with-param name="FldLvl" select="$FldLvl" />
            <xsl:with-param name="Num" select="count($List)" />
          </xsl:call-template>

        </xsl:variable>

        <xsl:if test="$Num != -1">

          <xsl:variable name="TextNode" select="." />
          <xsl:for-each select="$List">

            <xsl:if test="position() = $Num">
              <xsl:call-template name="ProcessField">
                <xsl:with-param name="CurNode" select="current()" />
                <xsl:with-param name="TextNode" select="$TextNode" />
              </xsl:call-template>
            </xsl:if>

          </xsl:for-each>
          
        </xsl:if>
                       
      </xsl:otherwise>

    </xsl:choose>
  </xsl:template>

  <!-- drop cap text  (only on first w:t of first w:r of w:p) -->
  <xsl:template name="InsertDropCapText">
    <xsl:if test="not(preceding-sibling::w:t[1]) and not(parent::w:r/preceding-sibling::w:r[1])">
      <xsl:variable name="prev-paragraph"
			  select="ancestor-or-self::w:p[1]/preceding-sibling::w:p[1]"/>
      <xsl:if test="$prev-paragraph/w:pPr/w:framePr[@w:dropCap]">
        <span class="{generate-id($prev-paragraph/w:r[1])}">
          <xsl:value-of select="$prev-paragraph/w:r[1]/w:t"/>
        </span>
      </xsl:if>
    </xsl:if>
  </xsl:template>

  <!--  convert multiple white spaces  -->
  <xsl:template name="InsertWhiteSpaces">
    <xsl:param name="string" select="."/>
    <xsl:param name="length" select="string-length(.)"/>
    <!-- string which doesn't contain whitespaces-->
    <xsl:choose>
      <xsl:when test="not(contains($string,' '))">
        <xsl:value-of select="$string"/>
      </xsl:when>

      <!-- convert white spaces  -->
      <xsl:otherwise>
        <xsl:variable name="before">
          <xsl:value-of select="substring-before($string,' ')"/>
        </xsl:variable>
        <xsl:variable name="after">
          <xsl:call-template name="CutStartSpaces">
            <xsl:with-param name="cuted">
              <xsl:value-of select="substring-after($string,' ')"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:variable>
        <xsl:variable name="numberOfSpaces" select="$length - string-length($before) - string-length($after)" />
        <xsl:value-of select="$before"/>
        <xsl:choose>
          <!-- По какой то неведомой мне причине, HTMLayout не показывает простые пробелы 
                 до и после текста в тэге, поэтому мы ставим конструкцию <span> </span>, но 
                 почему то так он видит только если до данной конструкции есть другие тэги(
                 т.е. для пробела после текста так работает, а для пробела до текста - нет),
                 поэтому в итоге делаем <span><span/> </span>
                 PS: Как только избавимся от дурацкого HTMLayout эту заглушку надо будет убрать -->
          <xsl:when test="$numberOfSpaces = 1 and $before = ''">
            <xsl:text disable-output-escaping="yes">&lt;span&gt;&lt;span&gt;&lt;/span&gt; &lt;/span&gt;</xsl:text>
          </xsl:when>
          <xsl:when test="$numberOfSpaces = 1 and $after = ''">
            <xsl:text disable-output-escaping="yes">&lt;span&gt; &lt;/span&gt;</xsl:text>
          </xsl:when>
          <xsl:when test="$numberOfSpaces = 1">
            <!-- single whitespace in the middle of the string -->
            <xsl:text> </xsl:text>
          </xsl:when>
          <xsl:otherwise>
            <!--STUB-->
            <xsl:text disable-output-escaping="yes"> </xsl:text>
            <!-- several consecutive whitespace characters -->
            <!--
                        <xsl:call-template name="InsertSpacesSequence">
                            <xsl:with-param name="string">
                                <xsl:value-of select="$string"/>
                            </xsl:with-param>
                        </xsl:call-template>
                        -->
          </xsl:otherwise>
        </xsl:choose>

        <!--repeat it for substring which has whitespaces-->
        <xsl:call-template name="InsertWhiteSpaces">
          <xsl:with-param name="string">
            <xsl:value-of select="$after"/>
          </xsl:with-param>
          <xsl:with-param name="length">
            <xsl:value-of select="string-length($after)"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="InsertSpacesSequence">
    <xsl:param name="string"/>

    <xsl:choose>
      <xsl:when test="starts-with($string,' ')">
        <span style="mso-tab-count:1">&#160;&#160;</span>
        <xsl:call-template name="InsertSpacesSequence">
          <xsl:with-param name="string">
            <xsl:value-of select="substring-after($string,' ')"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!--  cut start spaces -->
  <xsl:template name="CutStartSpaces">
    <xsl:param name="cuted"/>
    <xsl:choose>
      <xsl:when test="starts-with($cuted,' ')">
        <xsl:call-template name="CutStartSpaces">
          <xsl:with-param name="cuted">
            <xsl:value-of select="substring-after($cuted,' ')"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$cuted"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- line breaks  -->
  <xsl:template match="w:br[not(@w:type) or @w:type!='page' and @w:type!='column']">
    <xsl:text disable-output-escaping="yes">&lt;br/&gt;</xsl:text>
  </xsl:template>

  <xsl:template match="w:noBreakHyphen"></xsl:template>

  <!-- 
		*************************************************************************
		Templates for creating Automatic Styles
		*************************************************************************
	-->
  <!-- page or column break must have style defined in paragraph -->
  <xsl:template match="w:br[@w:type='page' or @w:type='column']" mode="automaticstyles">
    <!--AVSMedia: not needed in XHTML-->
    <!--
        <xsl:if test="not(ancestor::w:p/w:pPr)">
            <style:style style:name="{generate-id(ancestor::w:p)}" style:family="paragraph">
                <xsl:call-template name="MasterPageName"/>
                <style:paragraph-properties>
                    <xsl:call-template name="InsertParagraphProperties"/>
                </style:paragraph-properties>
            </style:style>
        </xsl:if>-->

  </xsl:template>

  <xsl:template match="w:br[@w:type='page' or @w:type='column']">

    <xsl:if test="$HtmlMode = 'CKEditor'">
      <img>

        <xsl:attribute name="align">
          <xsl:value-of select="''"/>
        </xsl:attribute>

        <xsl:attribute name="aria-label">
          <xsl:value-of select="'Page Break'"/>
        </xsl:attribute>

        <xsl:attribute name="class">
          <xsl:value-of select="'cke_pagebreak'"/>
        </xsl:attribute>

        <xsl:attribute name="data-cke-realelement">
          <xsl:value-of select="'%3Cdiv%20style%3D%22page-break-after%3A%20always%3B%22%3E%3Cspan%20style%3D%22display%3A%20none%3B%22%3E%26nbsp%3B%3C%2Fspan%3E%3C%2Fdiv%3E'" />
        </xsl:attribute>

        <!-- Когда CKEditor встретит этот Guid, он заменит его не нормальный путь -->
        <xsl:attribute name="src">
          <xsl:value-of select="'{C03A67CC-3827-4363-84BB-D095B6B60120}'"/>
        </xsl:attribute>

      </img>
    </xsl:if>
  </xsl:template>

  <!-- symbols : text style -->
  <xsl:template match="w:sym" mode="automaticstyles"/>

  <!-- symbols -->
  <xsl:template match="w:sym">
    <span>
      <xsl:variable name="numDec">
        <xsl:call-template name="HexToDec">
          <xsl:with-param name="number">
            <xsl:value-of select="substring(@w:char, 1, 4)"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:variable>
      <xsl:variable name="fontFamily">
        <xsl:if test="@w:font">
          <xsl:text>font-family:&#39;</xsl:text>
          <xsl:value-of select="@w:font"/>
          <xsl:text>&#39;;</xsl:text>
        </xsl:if>
      </xsl:variable>
      <xsl:if test="@w:font">
        <xsl:attribute name="style">
          <xsl:value-of select="$fontFamily"/>
        </xsl:attribute>
      </xsl:if>
      <!-- character post-processing -->
      <xsl:choose>
        <xsl:when test="$numDec = 61482">
          <xsl:text>*</xsl:text>
        </xsl:when>
        <xsl:when test="number($numDec) >= 65534">
          <!-- Плохие символы заменяем пробелами -->
          <xsl:text disable-output-escaping="yes">&amp;#32;</xsl:text>
        </xsl:when>
        <xsl:when test="number($numDec) > 61482"> 
          <xsl:text disable-output-escaping="yes">&lt;script type = "text/javascript"&gt;</xsl:text>
          <xsl:text>symbol("</xsl:text>
          <xsl:text disable-output-escaping="yes">&amp;#</xsl:text>
          <xsl:value-of select="number($numDec) - 61440"/>
          <xsl:text>;", "</xsl:text>
          <xsl:text disable-output-escaping="yes">&amp;#</xsl:text>
          <xsl:value-of select="number($numDec)"/>
          <xsl:text>;")</xsl:text>
          <xsl:text disable-output-escaping="yes">&lt;/script&gt;</xsl:text>

          <xsl:text disable-output-escaping="yes">&lt;noscript&gt;</xsl:text>
          <xsl:text disable-output-escaping="yes">&amp;#</xsl:text>
          <xsl:value-of select="number($numDec) - 61440"/>
          <xsl:text>;</xsl:text>
          <xsl:text disable-output-escaping="yes">&lt;/noscript&gt;</xsl:text>

        </xsl:when>
        <xsl:otherwise>
          <xsl:text disable-output-escaping="yes">&amp;#</xsl:text>
          <xsl:value-of select="$numDec"/>
          <xsl:text>;</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </span>
  </xsl:template>

  <!--ignore text in automatic styles mode-->
  <xsl:template match="text()" mode="automaticstyles"/>

  <!-- insert a master page name when required -->
  <xsl:template name="MasterPageName">
    <xsl:if test="ancestor::w:body">

      <!-- NB : precSectPr defines properties of preceding section,
			whereas followingSectPr defines properties of current section -->
      <!--xsl:param name="precSectPr" select="preceding::w:p[w:pPr/w:sectPr][1]/w:pPr/w:sectPr"/>
			<xsl:param name="followingSectPr" select="following::w:p[w:pPr/w:sectPr][1]/w:pPr/w:sectPr"/-->
      <!--<xsl:param name="mainSectPr" select="key('Part', 'word/document.xml')/w:document/w:body/w:sectPr"/>-->

      <xsl:variable name="mainSectPr" select="key('Part', 'word/document.xml')/w:document/w:body/w:sectPr"/>
      <xsl:variable name="precSectPr" select="key('sectPr', number(ancestor-or-self::node()/@oox:s) - 1)"/>
      <xsl:variable name="followingSectPr" select="(w:sectPr | key('sectPr', number(ancestor-or-self::node()/@oox:s))[ancestor::w:p/w:pPr/w:sectPr])[1]"/>
      <xsl:choose>
        <!-- first case : current section is continuous with preceding section (test if precSectPr exist to avoid bugs) -->
        <xsl:when test="$precSectPr and $followingSectPr/w:type/@w:val = 'continuous' ">
          <!-- no new master page. Warn loss of page header/footer change (should not occure in OOX, but Word 2007 handles it) -->
          <xsl:if
					  test="$followingSectPr/w:headerReference[@w:type='default']/@r:id != $precSectPr/w:headerReference[@w:type='default']/@r:id
							  or $followingSectPr/w:headerReference[@w:type='even']/@r:id != $precSectPr/w:headerReference[@w:type='even']/@r:id 
							  or $followingSectPr/w:headerReference[@w:type='first']/@r:id != $precSectPr/w:headerReference[@w:type='first']/@r:id 
							  or $followingSectPr/w:footerReference[@w:type='default']/@r:id != $precSectPr/w:footerReference[@w:type='default']/@r:id
							  or $followingSectPr/w:footerReference[@w:type='even']/@r:id != $precSectPr/w:footerReference[@w:type='even']/@r:id 
							  or $followingSectPr/w:footerReference[@w:type='first']/@r:id != $precSectPr/w:footerReference[@w:type='first']/@r:id">
            <xsl:message terminate="no">
              <xsl:text>feedback:Header/footer change after continuous section break.</xsl:text>
            </xsl:message>
          </xsl:if>
        </xsl:when>
        <!-- beginning of document -->
        <xsl:when test="not(preceding::w:p[ancestor::w:body])">
          <xsl:choose>
            <xsl:when test="$followingSectPr">
              <xsl:choose>
                <xsl:when test="$followingSectPr/w:titlePg">
                  <xsl:attribute name="style:master-page-name">
                    <xsl:value-of select="concat('First_H_',generate-id($followingSectPr))"/>
                  </xsl:attribute>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:attribute name="style:master-page-name">
                    <xsl:value-of select="concat('H_',generate-id($followingSectPr))"/>
                  </xsl:attribute>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:when>
            <xsl:otherwise>
              <xsl:choose>
                <xsl:when test="$mainSectPr/w:titlePg">
                  <xsl:attribute name="style:master-page-name">First_Page</xsl:attribute>
                </xsl:when>
                <!--<xsl:otherwise>
                  <xsl:attribute name="style:master-page-name">Standard</xsl:attribute>
                </xsl:otherwise>-->
              </xsl:choose>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:when>
        <xsl:otherwise>
          <!--clam: bugfix #1800794-->
          <!--<xsl:if test="preceding::w:p[parent::w:body|parent::w:tbl/tr/tv][1]/w:pPr/w:sectPr">-->

          <!-- 20080715/divo: performance improvement by using xsl:key -->
          <!--xsl:if test="preceding::w:p[parent::w:body|parent::w:tc][1]/w:pPr/w:sectPr"-->
          <xsl:if test="key('p', number(ancestor-or-self::node()/@oox:id) - 1)/w:pPr/w:sectPr">
            <xsl:choose>
              <xsl:when test="$followingSectPr and not($followingSectPr/w:headerReference) and not($followingSectPr/w:footerReference)">
                <xsl:attribute name="style:master-page-name">
                  <!-- jslaurent : hack to make it work in any situation. Does not make any sense though.
										 master page names should be reviewed and unified : many names not consistent, many styles never used -->
                  <xsl:value-of select="concat('H_',generate-id($followingSectPr))"/>
                </xsl:attribute>
              </xsl:when>
              <xsl:otherwise>
                <xsl:choose>
                  <xsl:when test="$followingSectPr">
                    <xsl:choose>
                      <xsl:when test="$followingSectPr/w:titlePg">
                        <xsl:attribute name="style:master-page-name">
                          <xsl:value-of select="concat('First_H_',generate-id($followingSectPr))"/>
                        </xsl:attribute>
                      </xsl:when>
                      <xsl:otherwise>
                        <xsl:if
												  test="$followingSectPr/w:headerReference or $followingSectPr/w:footerReference">
                          <xsl:attribute name="style:master-page-name">
                            <xsl:value-of select="concat('H_',generate-id($followingSectPr))"/>
                          </xsl:attribute>
                        </xsl:if>
                      </xsl:otherwise>
                    </xsl:choose>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:choose>
                      <xsl:when test="$mainSectPr/w:titlePg">
                        <xsl:attribute name="style:master-page-name">First_Page</xsl:attribute>
                      </xsl:when>
                      <xsl:otherwise>
                        <xsl:choose>
                          <xsl:when test="following-sibling::w:r/w:lastRenderedPageBreak and $precSectPr/w:type/@w:val = 'continuous' and generate-id(..) = generate-id($precSectPr/following::w:p[1])">
                            <xsl:attribute name="style:master-page-name">Standard</xsl:attribute>
                          </xsl:when>
                          <xsl:when test="$precSectPr and $mainSectPr/w:type/@w:val = 'continuous' ">
                            <!-- no new master page. Warn loss of page header/footer change (should not occure in OOX, but Word 2007 handles it) -->
                            <xsl:if
														  test="$mainSectPr/w:headerReference[@w:type='default']/@r:id != $precSectPr/w:headerReference[@w:type='default']/@r:id
																  or $mainSectPr/w:headerReference[@w:type='even']/@r:id != $precSectPr/w:headerReference[@w:type='even']/@r:id 
																  or $mainSectPr/w:headerReference[@w:type='first']/@r:id != $precSectPr/w:headerReference[@w:type='first']/@r:id 
																  or $mainSectPr/w:footerReference[@w:type='default']/@r:id != $precSectPr/w:footerReference[@w:type='default']/@r:id
																  or $mainSectPr/w:footerReference[@w:type='even']/@r:id != $precSectPr/w:footerReference[@w:type='even']/@r:id 
																  or $mainSectPr/w:footerReference[@w:type='first']/@r:id != $precSectPr/w:footerReference[@w:type='first']/@r:id">
                              <xsl:message terminate="no">
                                <xsl:text>feedback:Header/footer change after continuous section break.</xsl:text>
                              </xsl:message>
                            </xsl:if>
                          </xsl:when>
                          <xsl:otherwise>
                            <xsl:attribute name="style:master-page-name">Standard</xsl:attribute>
                          </xsl:otherwise>
                        </xsl:choose>
                      </xsl:otherwise>
                    </xsl:choose>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:if>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:if>
  </xsl:template>

  <!--
		Summary: Writes the offset for page numbers. 
            The calling context must be a w:p node
		Author:  makz (DIaLOGIKa)
	-->
  <xsl:template name="InsertPageNumberOffset">
    <!--Only change the page number offset for a paragraph in the body ...-->
    <xsl:if test="ancestor::w:body">
      <!--... and only if this is the first paragraph in the section ...-->
      <xsl:if test="not(preceding-sibling::w:p) or preceding-sibling::w:p[1]/w:pPr/w:sectPr">

        <xsl:variable name="currentSectPr" select="key('sectPr', number(ancestor-or-self::node()/@oox:s))" />

        <xsl:if test="$currentSectPr/w:pgNumType/@w:start">
          <!-- there is a sectPr for this paragraph -->
          <xsl:attribute name="style:page-number">
            <xsl:value-of select="$currentSectPr/w:pgNumType/@w:start"/>
          </xsl:attribute>
        </xsl:if>
      </xsl:if>
    </xsl:if>
  </xsl:template>
</xsl:stylesheet>
