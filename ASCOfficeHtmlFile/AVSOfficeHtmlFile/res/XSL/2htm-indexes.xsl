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
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
  xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:b="http://schemas.openxmlformats.org/officeDocument/2006/bibliography"
  xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:dc="http://purl.org/dc/elements/1.1/"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns:oox="urn:oox"
                xmlns="http://www.w3.org/1999/xhtml"
  exclude-result-prefixes="w text style oox">

	<xsl:key name="tocBookmark" match="w:bookmarkStart" use="@w:name"/>
	<xsl:key name="OutlineLevel" match="w:outlineLvl" use="''"/>
	<xsl:key name="StyleId" match="w:style" use="@w:styleId"/>

	<!-- paragraph which starts table of content -->
	<xsl:template match="w:p" mode="tocstart">
		<xsl:variable name="instrTextContent">
			<xsl:apply-templates select="w:r/w:instrText[1]" mode="textContent">
				<xsl:with-param name="textContent"/>
			</xsl:apply-templates>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="w:r[contains(w:instrText,'TOC')]">
				<xsl:choose>
					<xsl:when test="not(contains($instrTextContent,'\c'))">
						<text:table-of-content text:style-name="Sect1">
							<xsl:message terminate="no">translation.oox2htm.toc.protection.manualChange</xsl:message>
							<xsl:attribute name="text:protected">false</xsl:attribute>
							<xsl:attribute name="text:name">Table of Contents1</xsl:attribute>
							<xsl:call-template name="InsertIndexProperties">
								<xsl:with-param name="type">TOC</xsl:with-param>
								<xsl:with-param name="instrTextContent" select="$instrTextContent"/>
							</xsl:call-template>
							<text:index-body>
								<xsl:apply-templates select="." mode="index"/>
							</text:index-body>
						</text:table-of-content>
					</xsl:when>
					<xsl:otherwise>
						<text:table-index text:style-name="Sect1">
							<xsl:message terminate="no">translation.oox2htm.toc.protection.manualChange</xsl:message>
							<xsl:attribute name="text:protected">false</xsl:attribute>
							<xsl:attribute name="text:name">Index of Tables1</xsl:attribute>
							<xsl:call-template name="InsertIndexProperties">
								<xsl:with-param name="type">INDEX</xsl:with-param>
								<xsl:with-param name="instrTextContent" select="$instrTextContent"/>
							</xsl:call-template>
							<text:index-body>
								<xsl:apply-templates select="." mode="index"/>
							</text:index-body>
						</text:table-index>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:when test="w:r[contains(w:instrText,'INDEX')]">
				<text:alphabetical-index>
					<!-- TODO : localization -->
					<xsl:message terminate="no">feedback:many alphabetical index properties</xsl:message>
					<xsl:attribute name="text:style-name">
						<!--<xsl:value-of select="generate-id(key('p', @oox:id+1)/descendant::w:sectPr)"/> clam bugfix #1771286 -->
						<xsl:value-of select="generate-id(following::w:sectPr[1])"/>
					</xsl:attribute>
					<xsl:attribute name="text:protected">false</xsl:attribute>
					<xsl:attribute name="text:name">Alphabetical Index1</xsl:attribute>
					<xsl:call-template name="InsertIndexProperties">
						<xsl:with-param name="type">INDEXA</xsl:with-param>
						<xsl:with-param name="instrTextContent" select="$instrTextContent"/>
					</xsl:call-template>
					<text:index-body>
						<xsl:apply-templates select="." mode="indexa"/>
					</text:index-body>
				</text:alphabetical-index>
			</xsl:when>
			<xsl:when test="w:r[contains(w:instrText,'BIBLIOGRAPHY')]">
				<text:bibliography>
					<xsl:attribute name="text:name">
						<xsl:value-of select="generate-id(w:r[contains(w:instrText,'BIBLIOGRAPHY')])"/>
					</xsl:attribute>
					<text:bibliography-source>
						<text:index-title-template/>
						<xsl:call-template name="InsertBibliographyProperties"/>
					</text:bibliography-source>
					<text:index-body>
						<xsl:apply-templates select="." mode="index"/>
					</text:index-body>
				</text:bibliography>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<!-- paragraph in index-->
	<xsl:template match="w:p" mode="index">

		<text:p text:style-name="{generate-id()}">
			<xsl:apply-templates>
				<xsl:with-param name="ignoreFieldFlag" select="'true'" />
			</xsl:apply-templates>
		</text:p>

		<xsl:if test="following-sibling::w:p[1][count(preceding::w:fldChar[@w:fldCharType='begin']) &gt; count(preceding::w:fldChar[@w:fldCharType='end']) and descendant::text() and (not(w:r/w:fldChar) or w:r[w:fldChar]/preceding-sibling::w:r[w:t])]">
			<xsl:apply-templates select="following-sibling::w:p[1]" mode="index"/>
		</xsl:if>

	</xsl:template>

	<!-- template to create string containing styles include in TOC with appropriate levels -->

	<xsl:template match="w:p" mode="stylesOfTOC">
		<xsl:param name="stylesWithLevel"/>

		<!-- index source style -->
		<xsl:variable name="thisStyle">
			<xsl:choose>
				<xsl:when test="w:hyperlink">
					<xsl:for-each select="key('tocBookmark',w:hyperlink/@w:anchor)">
						<xsl:value-of select="parent::w:p/w:pPr/w:pStyle/@w:val"/>
					</xsl:for-each>
				</xsl:when>
				<xsl:otherwise>
					<xsl:variable name="instrTextContent">
						<xsl:apply-templates select="w:r/w:instrText[1]" mode="textContent">
							<xsl:with-param name="textContent"/>
						</xsl:apply-templates>
					</xsl:variable>
					<xsl:for-each select="key('tocBookmark',substring-before(substring-after($instrTextContent,'PAGEREF '),' '))">
						<xsl:value-of select="parent::w:p/w:pPr/w:pStyle/@w:val"/>
					</xsl:for-each>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<!-- outline level -->
		<xsl:variable name="thisLevel">
			<xsl:value-of select="substring(w:pPr/w:pStyle/@w:val,string-length(w:pPr/w:pStyle/@w:val))"/>
		</xsl:variable>

		<!--math, dialogika: added for bugfix #1802258 BEGIN -->
		<xsl:variable name="IsDefaultHeading">
			<xsl:call-template name="CheckDefaultHeading">
				<xsl:with-param name="Name">
					<xsl:value-of select="key('StyleId',$thisStyle)/w:name/@w:val" />
				</xsl:with-param>
			</xsl:call-template>
		</xsl:variable>
		<!--math, dialogika: added for bugfix #1802258 END -->

		<!--math, dialogika: added to avoid regression on solution for directly applied outline levels BEGIN -->
		<xsl:variable name="IsDirectOutline">
			<xsl:choose>
				<xsl:when test="w:hyperlink">
					<xsl:for-each select="key('tocBookmark',w:hyperlink/@w:anchor)">
						<xsl:choose>
							<xsl:when test="parent::w:p/w:pPr/w:outlineLvl/@w:val &lt; 9">true</xsl:when>
							<xsl:otherwise>false</xsl:otherwise>
						</xsl:choose>
					</xsl:for-each>
				</xsl:when>
				<xsl:otherwise>
					<xsl:variable name="instrTextContent">
						<xsl:apply-templates select="w:r/w:instrText[1]" mode="textContent">
							<xsl:with-param name="textContent"/>
						</xsl:apply-templates>
					</xsl:variable>
					<xsl:for-each select="key('tocBookmark',substring-before(substring-after($instrTextContent,'PAGEREF '),' '))">
						<xsl:choose>
							<xsl:when test="parent::w:p/w:pPr/w:outlineLvl/@w:val &lt; 9">true</xsl:when>
							<xsl:otherwise>false</xsl:otherwise>
						</xsl:choose>
					</xsl:for-each>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<!--math, dialogika: added to avoid regression on solution for directly applied outline levels END -->

		<xsl:choose>
			<xsl:when test="following-sibling::w:p[1][count(preceding::w:fldChar[@w:fldCharType='begin']) &gt; count(preceding::w:fldChar[@w:fldCharType='end']) and descendant::text()]">
				<xsl:apply-templates select="following-sibling::w:p[1]" mode="stylesOfTOC">
					<xsl:with-param name="stylesWithLevel">
						<xsl:choose>

							<!--math, dialogika: changed for bugfix #1802258 BEGIN -->
							<!--<xsl:when test="contains($thisStyle,'Heading') and not(contains($thisStyle,'Annex'))">-->
							<xsl:when test="$IsDefaultHeading='true'">
								<!--math, dialogika: changed for bugfix #1802258 END -->

								<xsl:value-of select="$stylesWithLevel"/>
							</xsl:when>
							<xsl:otherwise>

								<!--math, dialogika: Changed to avoid that all occurences of a style which is used in a paragraph
                    where an outline level is directly applied (not via style) are added to TOC. BEGIN -->

								<!--<xsl:if test="$thisStyle and $thisStyle!=''">
                  <xsl:value-of select="concat($stylesWithLevel,$thisStyle,':',$thisLevel,'.')"/>
                </xsl:if>-->
								<xsl:choose>
									<!--math, dialogika: Changed to avoid regression on solution for directly applied outline levels -->
									<xsl:when test="$thisStyle and $thisStyle!=''
                            and ($IsDirectOutline = 'false'
                            or key('StyleId',$thisStyle)/w:pPr/w:outlineLvl/@w:val &lt; 9)">
										<xsl:value-of select="concat($stylesWithLevel,$thisStyle,':',$thisLevel,'.')"/>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select="$stylesWithLevel"/>
									</xsl:otherwise>
								</xsl:choose>

								<!--math, dialogika: Changed to avoid that all occurences of a style which is used in a paragraph
                    where an outline level is directly applied (not via style) are added to TOC. END -->

							</xsl:otherwise>
						</xsl:choose>
					</xsl:with-param>
				</xsl:apply-templates>
			</xsl:when>
			<xsl:otherwise>
				<xsl:choose>

					<!--math, dialogika: changed for bugfix #1802258 BEGIN -->
					<!--<xsl:when test="contains($thisStyle,'Heading') and not(contains($thisStyle,'Annex'))">-->
					<xsl:when test="$IsDefaultHeading='true'">
						<!--math, dialogika: changed for bugfix #1802258 END -->

						<xsl:value-of select="$stylesWithLevel"/>
					</xsl:when>
					<xsl:otherwise>

						<!--math, dialogika: Changed to avoid that all occurences of a style which is used in a paragraph
                where an outline level is directly applied (not via style) are added to TOC. BEGIN -->

						<!--<xsl:value-of select="concat($stylesWithLevel,$thisStyle,':',$thisLevel,'.')"/>-->
						<xsl:choose>
							<!--math, dialogika: Changed to avoid regression on solution for directly applied outline levels -->
							<xsl:when test="$thisStyle and $thisStyle!=''
                            and ($IsDirectOutline = 'false'
                            or key('StyleId',$thisStyle)/w:pPr/w:outlineLvl/@w:val &lt; 9)">
								<xsl:value-of select="concat($stylesWithLevel,$thisStyle,':',$thisLevel,'.')"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="$stylesWithLevel"/>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:otherwise>

					<!--math, dialogika: Changed to avoid that all occurences of a style which is used in a paragraph
              where an outline level is directly applied (not via style) are added to TOC. END -->

				</xsl:choose>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!-- paragraph in alphabetical index-->
	<xsl:template match="w:p" mode="indexa">
		<xsl:choose>
			<xsl:when test="descendant::w:r[contains(w:instrText,'INDEX')]">
				<xsl:if test="following-sibling::w:p[1][count(preceding::w:fldChar[@w:fldCharType='begin']) &gt; count(preceding::w:fldChar[@w:fldCharType='end']) and descendant::text()]">
					<xsl:apply-templates select="following-sibling::w:p[1]" mode="indexa"/>
				</xsl:if>
			</xsl:when>
			<xsl:otherwise>
				<text:p>
					<!--math, dialogika: bugfix #1771286 BEGIN-->
					<!--<xsl:attribute name="text:style-name">Normal</xsl:attribute>-->
					<xsl:attribute name="text:style-name">
						<xsl:choose>
							<xsl:when test="w:pPr/w:pStyle/@w:val">
								<xsl:value-of select="w:pPr/w:pStyle/@w:val" />
							</xsl:when>
							<xsl:otherwise>Normal</xsl:otherwise>
						</xsl:choose>
					</xsl:attribute>
					<!--math, dialogika: bugfix #1771286 END-->
					<xsl:apply-templates mode="indexa"/>
				</text:p>
				<xsl:if test="following-sibling::w:p[1][count(preceding::w:fldChar[@w:fldCharType='begin']) &gt; count(preceding::w:fldChar[@w:fldCharType='end']) and descendant::text()]">
					<xsl:apply-templates select="following-sibling::w:p[1]" mode="indexa"/>
				</xsl:if>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!--take content from multiple w:instrText elements -->
	<xsl:template match="w:instrText" mode="textContent">
		<xsl:param name="textContent"/>
		<xsl:variable name="text">
			<xsl:value-of select="."/>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="following-sibling::w:instrText">
				<xsl:apply-templates select="following-sibling::w:instrText[1]" mode="textContent">
					<xsl:with-param name="textContent">
						<xsl:value-of select="concat($textContent,$text)"/>
					</xsl:with-param>
				</xsl:apply-templates>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="concat($textContent,$text)"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="GetMaxLevelFromStyles">
		<xsl:param name="stylesWithLevels"/>
		<xsl:param name="defaultLevel"/>
		<xsl:choose>
			<xsl:when test="$stylesWithLevels!=''">
				<xsl:variable name="firstNum">
					<xsl:choose>
						<xsl:when test="contains(substring-after($stylesWithLevels,';'),';')">
							<xsl:value-of select="number(substring-before(substring-after($stylesWithLevels,';'),';'))"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="number(substring-after($stylesWithLevels,';'))"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>
				<xsl:variable name="otherNum">
					<xsl:call-template name="GetMaxLevelFromStyles">
						<xsl:with-param name="stylesWithLevels">
							<xsl:value-of select="substring-after(substring-after($stylesWithLevels,';'),';')"/>
						</xsl:with-param>
						<xsl:with-param name="defaultLevel" select="$defaultLevel"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:variable name="num">
					<xsl:choose>
						<xsl:when test="number($firstNum) &gt; number($otherNum)">
							<xsl:value-of select ="$firstNum"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="$otherNum"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>
				<xsl:choose>
					<xsl:when test="number($num) &gt; number($defaultLevel)">
						<xsl:value-of select="$num"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="$defaultLevel"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<!--math, dialogika: bugfix #1785483 BEGIN-->
			<!--<xsl:otherwise>0</xsl:otherwise>-->
			<xsl:otherwise>
				<xsl:value-of select="$defaultLevel" />
			</xsl:otherwise>
			<!--math, dialogika: bugfix #1785483 END-->
		</xsl:choose>
	</xsl:template>

	<xsl:template name="GetMinLevelFromStyles">
		<xsl:param name="stylesWithLevels"/>
		<xsl:param name="defaultLevel"/>
		<xsl:choose>
			<xsl:when test="$stylesWithLevels!=''">
				<xsl:variable name="firstNum">
					<xsl:choose>
						<xsl:when test="contains(substring-after($stylesWithLevels,';'),';')">
							<xsl:value-of select="number(substring-before(substring-after($stylesWithLevels,';'),';'))"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="number(substring-after($stylesWithLevels,';'))"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>
				<xsl:variable name="otherNum">
					<xsl:call-template name="GetMinLevelFromStyles">
						<xsl:with-param name="stylesWithLevels">
							<xsl:value-of select="substring-after(substring-after($stylesWithLevels,';'),';')"/>
						</xsl:with-param>
						<xsl:with-param name="defaultLevel" select="$defaultLevel"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:variable name="num">
					<xsl:choose>
						<xsl:when test="number($firstNum) &lt; number($otherNum)">
							<xsl:value-of select ="$firstNum"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="$otherNum"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>
				<xsl:choose>
					<xsl:when test="number($num) &lt; number($defaultLevel)">
						<xsl:value-of select="$num"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="$defaultLevel"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:otherwise>10</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="GetStyleForLevel">
		<xsl:param name="stylesWithLevels"/>
		<xsl:param name="level"/>
		<xsl:choose>
			<xsl:when test="contains(substring-after($stylesWithLevels,';'),';')">
				<xsl:choose>
					<xsl:when test="number(substring-before(substring-after($stylesWithLevels,';'),';'))=number($level)">
						<xsl:value-of select="number(substring(substring-before($stylesWithLevels,';'),string-length(substring-before($stylesWithLevels,';'))))"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:call-template name="GetStyleForLevel">
							<xsl:with-param name="stylesWithLevels">
								<xsl:value-of select="substring-after(substring-after($stylesWithLevels,';'),';')"/>
							</xsl:with-param>
							<xsl:with-param name="level" select="$level"/>
						</xsl:call-template>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:otherwise>
				<!--math, dialogika: Bugfix 1828347 BEGIN-->
				<!--<xsl:choose>
          <xsl:when test="number(substring-after($stylesWithLevels,';'))=number($level)">
            <xsl:value-of select="number(substring(substring-before($stylesWithLevels,';'),string-length(substring-before($stylesWithLevels,';'))))"/>
          </xsl:when>
          <xsl:otherwise>-->
				<xsl:value-of select="$level"/>
				<!--</xsl:otherwise>
        </xsl:choose>-->
				<!--math, dialogika: Bugfix 1828347 END-->
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="GetOutlineLevelMax">
		<xsl:param name="value"/>
		<xsl:param name="count"/>
		<xsl:variable name="getValue">
			<xsl:value-of select="key('OutlineLevel', '')[number($count)]/@w:val"/>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="$count &gt; 0">
				<xsl:call-template name="GetOutlineLevelMax">
					<xsl:with-param name="value">
						<xsl:choose>
							<xsl:when test="number($getValue) &gt; number($value)">
								<xsl:value-of select="number($getValue)"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="number($value)"/>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:with-param>
					<xsl:with-param name="count">
						<xsl:value-of select="$count - 1"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$value + 1"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!-- insert table-of content properties -->
	<xsl:template name="InsertIndexProperties">
		<xsl:param name="type"/>
		<xsl:param name="instrTextContent"/>
		<xsl:variable name="maxLevel">
			<xsl:choose>
				<xsl:when test="contains($instrTextContent,'\t')">
					<xsl:call-template name="GetMaxLevelFromStyles">
						<xsl:with-param name="stylesWithLevels">
							<xsl:value-of select="substring-before(substring-after(substring-after($instrTextContent,'\t'),'&quot;'),'&quot;')"/>
						</xsl:with-param>
						<xsl:with-param name="defaultLevel">
							<xsl:choose>
								<xsl:when test="contains($instrTextContent,'-')">
									<xsl:value-of select="substring-before(substring-after($instrTextContent,'-'),'&quot;')"/>
								</xsl:when>
								<xsl:otherwise>0</xsl:otherwise>
							</xsl:choose>
						</xsl:with-param>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="contains($instrTextContent,'-')">
					<xsl:value-of select="substring-before(substring-after($instrTextContent,'-'),'&quot;')"/>
				</xsl:when>

				<xsl:when test="contains($instrTextContent,'\o')">

					<xsl:variable name="CountOutlineLevel">
						<xsl:for-each select="key('Part', 'word/document.xml')">
							<xsl:value-of select="count(key('OutlineLevel', '')/@w:val)"/>
						</xsl:for-each>
					</xsl:variable>
					<xsl:variable name="CountStyleOutlineLevel">
						<xsl:for-each select="key('Part', 'word/styles.xml')">
							<xsl:value-of select="count(key('OutlineLevel', '')/@w:val)"/>
						</xsl:for-each>
					</xsl:variable>
					<xsl:choose>
						<xsl:when test="$CountOutlineLevel > 0">
							<xsl:for-each select="key('Part', 'word/document.xml')">
								<xsl:call-template name="GetOutlineLevelMax">
									<xsl:with-param name="value">0</xsl:with-param>
									<xsl:with-param name="count">
										<xsl:value-of select="$CountOutlineLevel"/>
									</xsl:with-param>
								</xsl:call-template>
							</xsl:for-each>
						</xsl:when>
						<xsl:when test="$CountStyleOutlineLevel > 0">
							<xsl:for-each select="key('Part', 'word/styles.xml')">
								<xsl:call-template name="GetOutlineLevelMax">
									<xsl:with-param name="value">0</xsl:with-param>
									<xsl:with-param name="count">
										<xsl:value-of select="$CountStyleOutlineLevel"/>
									</xsl:with-param>
								</xsl:call-template>
							</xsl:for-each>
						</xsl:when>
						<xsl:otherwise>0</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
				<!-- when number of levels shown is not declared, all levels to show must be found according to their styles -->
				<xsl:when test="$type='TOC'">
					<xsl:apply-templates select="." mode="toclevel">
						<xsl:with-param name="level">0</xsl:with-param>
					</xsl:apply-templates>
				</xsl:when>
				<xsl:when test="$type='INDEXA'">3</xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="$type='TOC'">
				<text:table-of-content-source>
					<xsl:attribute name="text:outline-level">
						<xsl:choose>
							<xsl:when test="$maxLevel=0">1</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="$maxLevel"/>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:attribute>

					<!--math, dialogika: Bugfix #1841783: Use outline level only if \o is present-->
					<xsl:if test="substring-before(substring-after(substring-after($instrTextContent,'\o'),'&quot;'),'&quot;') = '' and contains($instrTextContent,'\t')">
						<xsl:attribute name="text:use-outline-level">false</xsl:attribute>
					</xsl:if>

					<xsl:attribute name="text:use-index-source-styles">true</xsl:attribute>
					<xsl:call-template name="InsertContentOfIndexProperties">
						<xsl:with-param name="styleName">Contents_20_Heading</xsl:with-param>
						<xsl:with-param name="maxLevel" select="$maxLevel"/>
						<xsl:with-param name="instrTextContent" select="$instrTextContent"/>
						<xsl:with-param name="type" select="$type"/>
					</xsl:call-template>
				</text:table-of-content-source>
			</xsl:when>
			<xsl:when test="$type='INDEXA'">
				<text:alphabetical-index-source text:sort-algorithm="alphanumeric">
					<xsl:if test="contains($instrTextContent,'\h')">
						<xsl:attribute name="text:alphabetical-separators">true</xsl:attribute>
					</xsl:if>
					<xsl:call-template name="InsertContentOfIndexProperties">
						<xsl:with-param name="styleName">Index_20_Heading</xsl:with-param>
						<xsl:with-param name="maxLevel" select="$maxLevel"/>
						<xsl:with-param name="instrTextContent" select="$instrTextContent"/>
						<xsl:with-param name="type" select="$type"/>
					</xsl:call-template>
				</text:alphabetical-index-source>
			</xsl:when>
			<xsl:otherwise>
				<text:table-index-source text:caption-sequence-name="{substring-before(substring-after($instrTextContent,'&quot;'),'&quot;')}">
					<xsl:attribute name="text:caption-sequence-format">
						<xsl:choose>
							<xsl:when test="contains($instrTextContent,'\a')">caption</xsl:when>
							<xsl:otherwise>text</xsl:otherwise>
						</xsl:choose>
					</xsl:attribute>
					<xsl:call-template name="InsertContentOfIndexProperties">
						<xsl:with-param name="styleName">Table_20_index_20_heading</xsl:with-param>
						<xsl:with-param name="maxLevel" select="$maxLevel"/>
						<xsl:with-param name="instrTextContent" select="$instrTextContent"/>
					</xsl:call-template>
				</text:table-index-source>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="InsertContentOfIndexProperties">
		<xsl:param name="styleName"/>
		<xsl:param name="maxLevel"/>
		<xsl:param name="instrTextContent"/>
		<xsl:param name="type"/>
		<xsl:if test="$maxLevel = 0"/>
		<text:index-title-template text:style-name="{$styleName}"/>
		<xsl:variable name="level">
			<xsl:choose>
				<xsl:when test="$type='INDEXA'">0</xsl:when>
				<xsl:when test="contains($instrTextContent,'\t')">
					<xsl:call-template name="GetMinLevelFromStyles">
						<xsl:with-param name="stylesWithLevels">
							<xsl:value-of select="substring-before(substring-after(substring-after($instrTextContent,'\t'),'&quot;'),'&quot;')"/>
						</xsl:with-param>
						<xsl:with-param name="defaultLevel">
							<xsl:choose>
								<xsl:when test="contains($instrTextContent,'-')">
									<xsl:value-of select="substring-after(substring-before($instrTextContent,'-'),'&quot;')"/>
								</xsl:when>
								<xsl:otherwise>10</xsl:otherwise>
							</xsl:choose>
						</xsl:with-param>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="contains($instrTextContent,'-')">
					<xsl:value-of select="substring-after(substring-before($instrTextContent,'-'),'&quot;')"/>
				</xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:call-template name="InsertIndexEntryProperties">
			<xsl:with-param name="level" select="$level"/>
			<xsl:with-param name="maxLevel" select="$maxLevel"/>
			<xsl:with-param name="instrTextContent" select="$instrTextContent"/>
			<xsl:with-param name="type" select="$type"/>
		</xsl:call-template>

		<xsl:if test="$type='TOC'">

			<!-- create string of index source styles included in TOC -->
			<!--math, dialogika: Added for bugfix #1841783-->
			<xsl:variable name="stylesWithLevelIncludedViaOptionT">
				<xsl:call-template name ="FormatStyleList">
					<xsl:with-param name ="string">
						<xsl:call-template name ="ExtractOptionTFromTOCDefinition">
							<xsl:with-param name ="string">
								<xsl:value-of select="$instrTextContent" />
							</xsl:with-param>
						</xsl:call-template>
						<!--<xsl:value-of select="substring-before(substring-after(substring-after($instrTextContent,'\t'),'&quot;'),'&quot;')"/>-->
					</xsl:with-param>
				</xsl:call-template>
			</xsl:variable>

			<!--math, dialogika: Changed for bugfix #1841783-->
			<xsl:variable name="stylesOfTOC">
				<xsl:apply-templates select="." mode="stylesOfTOC">
					<xsl:with-param name="stylesWithLevel">
						<!--<xsl:call-template name="StringReplace">
              <xsl:with-param name ="string">-->
						<xsl:value-of select="$stylesWithLevelIncludedViaOptionT"/>
						<!--</xsl:with-param>
              <xsl:with-param name="substring1">
                <xsl:text> </xsl:text>
              </xsl:with-param>
              <xsl:with-param name="substring2">
                <xsl:text>_20_</xsl:text>                
              </xsl:with-param>
            </xsl:call-template>-->
					</xsl:with-param>
				</xsl:apply-templates>
			</xsl:variable>

			<!-- post-processor creates text:index-source-styles elements from string -->
			<pxsi:index-source-styles xmlns:pxsi="urn:cleverage:xmlns:post-processings:source-styles">
				<xsl:value-of select="$stylesOfTOC"/>
			</pxsi:index-source-styles>

		</xsl:if>
	</xsl:template>


	<!--math, dialogika: Added for bugfix #1841783-->
	<!--Append several \t in TOC definition into one stringAppend several \t in TOC definition into one string-->
	<xsl:template name="ExtractOptionTFromTOCDefinition">
		<xsl:param name ="string" />
		<xsl:param name ="result" />
		<xsl:choose>
			<xsl:when test="contains($string,'\t')">
				<xsl:call-template name="ExtractOptionTFromTOCDefinition">
					<xsl:with-param name="string">
						<xsl:value-of select="substring-after($string,'\t')" />
					</xsl:with-param>
					<xsl:with-param name="result">
						<xsl:value-of select="concat($result,substring-before(substring-after(substring-after($string,'\t'),'&quot;'),'&quot;'))" />
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$result" />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<!--math, dialogika: Replace occurences of substring1 in string with substring2-->
	<xsl:template name="StringReplace">
		<xsl:param name ="string" />
		<xsl:param name ="substring1" />
		<xsl:param name ="substring2" />
		<xsl:choose>
			<xsl:when test ="contains($string,$substring1)">
				<xsl:call-template name="StringReplace">
					<xsl:with-param name="string">
						<xsl:value-of select="concat(substring-before($string,$substring1),$substring2,substring-after($string,$substring1))" />
					</xsl:with-param>
					<xsl:with-param name="substring1">
						<xsl:value-of select="$substring1" />
					</xsl:with-param>
					<xsl:with-param name="substring2">
						<xsl:value-of select="$substring2" />
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$string" />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!--math, dialogika: Added for bugfix #1841783-->
	<!--Format string list for the post processor (StyleId:Level.StyleId:Level. ...)-->
	<xsl:template name="FormatStyleList">
		<xsl:param name ="string" />
		<xsl:param name ="result" />

		<xsl:choose>
			<xsl:when test="$string != ''">
				<xsl:call-template name="FormatStyleList">
					<xsl:with-param name="string">
						<xsl:value-of select="substring-after(substring-after($string,';'),';')" />
					</xsl:with-param>
					<xsl:with-param name="result">
						<xsl:variable name="styleId">
							<xsl:variable name="styleName">
								<xsl:value-of select="substring-before($string,';')" />
							</xsl:variable>
							<xsl:choose>
								<xsl:when test="key('StyleId', $styleName)" >
									<xsl:value-of select ="$styleName"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="key('Part', 'word/styles.xml')/w:styles/w:style[w:name/@w:val=$styleName]/@w:styleId" />
								</xsl:otherwise>
							</xsl:choose>
						</xsl:variable>
						<xsl:value-of select="concat($result,$styleId,':',substring(substring-after($string,';'),1,1),'.')" />
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$result" />
			</xsl:otherwise>
		</xsl:choose>

	</xsl:template>


	<!-- insert entry properties -->
	<xsl:template name="InsertIndexEntryProperties">
		<xsl:param name="level"/>
		<xsl:param name="maxLevel"/>
		<xsl:param name="instrTextContent"/>
		<xsl:param name="type"/>
		<xsl:variable name="node" select="self::node()"/>
		<xsl:variable name="levelForStyle">
			<xsl:call-template name="GetStyleForLevel">
				<xsl:with-param name="stylesWithLevels">
					<xsl:value-of select="substring-before(substring-after(substring-after($instrTextContent,'\t'),'&quot;'),'&quot;')"/>
				</xsl:with-param>
				<xsl:with-param name="level" select="$level"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="not(number($level) &gt; number($maxLevel)) and $levelForStyle!='NaN'">
				<xsl:choose>
					<xsl:when test="$type='TOC'">
						<!-- text:outline-level can't be 0, so when $level = 0, 1is displayed -->
						<text:table-of-content-entry-template text:outline-level="{$level}">
							<xsl:if test="$level = 0">
								<xsl:attribute name="text:outline-level">1</xsl:attribute>
							</xsl:if>
							<xsl:call-template name="InsertContentOfIndexEntryProperties">
								<xsl:with-param name="level" select="$level"/>
								<xsl:with-param name="instrTextContent" select="$instrTextContent"/>
								<xsl:with-param name="levelForStyle" select="$levelForStyle"/>
								<xsl:with-param name="node" select="$node"/>
								<xsl:with-param name="type" select="$type"/>
							</xsl:call-template>
						</text:table-of-content-entry-template>
					</xsl:when>
					<xsl:when test="$type='INDEXA'">
						<text:alphabetical-index-entry-template>
							<xsl:choose>
								<xsl:when test="$level = '0'">
									<xsl:attribute name="text:outline-level">separator</xsl:attribute>
								</xsl:when>
								<xsl:otherwise>
									<xsl:attribute name="text:outline-level">
										<xsl:value-of select="$level"/>
									</xsl:attribute>
								</xsl:otherwise>
							</xsl:choose>
							<xsl:call-template name="InsertContentOfIndexEntryProperties">
								<xsl:with-param name="level" select="$level"/>
								<xsl:with-param name="instrTextContent" select="$instrTextContent"/>
								<xsl:with-param name="levelForStyle" select="$levelForStyle"/>
								<xsl:with-param name="node" select="$node"/>
								<xsl:with-param name="type" select="$type"/>
							</xsl:call-template>
						</text:alphabetical-index-entry-template>
					</xsl:when>
					<xsl:otherwise>
						<text:table-index-entry-template>
							<xsl:call-template name="InsertContentOfIndexEntryProperties">
								<xsl:with-param name="level" select="$level"/>
								<xsl:with-param name="instrTextContent" select="$instrTextContent"/>
								<xsl:with-param name="levelForStyle" select="$levelForStyle"/>
								<xsl:with-param name="node" select="$node"/>
								<xsl:with-param name="type" select="$type"/>
							</xsl:call-template>
						</text:table-index-entry-template>
					</xsl:otherwise>
				</xsl:choose>
				<xsl:call-template name="InsertIndexEntryProperties">
					<xsl:with-param name="level" select="number($level)+1"/>
					<xsl:with-param name="maxLevel" select="$maxLevel"/>
					<xsl:with-param name="instrTextContent" select="$instrTextContent"/>
					<xsl:with-param name="type" select="$type"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise></xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="InsertContentOfIndexEntryProperties">
		<xsl:param name="level"/>
		<xsl:param name="instrTextContent"/>
		<xsl:param name="levelForStyle"/>
		<xsl:param name="node"/>
		<xsl:param name="type"/>
		<xsl:if test="$level = 0"/>
		<xsl:attribute name="text:style-name">
			<xsl:choose>
				<!--math, dialogika: bugfix #1771286 BEGIN-->
				<xsl:when test="$type='INDEXA' and key('Part', 'word/styles.xml')/w:styles/w:style/w:name[@w:val=(concat('index ',$level))]">
					<xsl:value-of select="concat('index ',$level)" />
				</xsl:when>
				<!--math, dialogika: bugfix #1771286 END-->
				<xsl:when test="$type='INDEXA'">Normal</xsl:when>
				<xsl:when test="$level=0">
					<xsl:call-template name="TocToContent">
						<xsl:with-param name="styleValue">
							<xsl:value-of select="w:pPr/w:pStyle/@w:val"/>
						</xsl:with-param>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="contains($instrTextContent,'\t')">
					<xsl:value-of select="concat('Contents_20_',$levelForStyle)"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="concat('Contents_20_',$level)"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:attribute>
		<xsl:call-template name="EntryIterator">
			<xsl:with-param name="fieldCharCount">0</xsl:with-param>
			<xsl:with-param name="level" select="$level"/>
			<xsl:with-param name="node" select="$node"/>
			<xsl:with-param name="instrTextContent" select="$instrTextContent"/>
			<xsl:with-param name="type" select="$type"/>
		</xsl:call-template>
	</xsl:template>


	<!--math: Added for bugfix #1934315 START-->
	<!--xsl:template name ="CheckDefaultTOCStyle">
    <xsl:param name="name" />
    <xsl:param name="Counter" select="1"/>
    <xsl:choose>
      <xsl:when test="$Counter &gt; 9" >false</xsl:when>
      <xsl:when test="concat('TOC',$Counter) = $Name">true</xsl:when>
      <xsl:when test="$Name = 'TOCHeading'">true</xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="CheckDefaultTOCStyle">
          <xsl:with-param name="name" select="$Name" />
          <xsl:with-param name="Counter">
            <xsl:value-of select="$Counter + 1" />
          </xsl:with-param>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template-->
	<xsl:template name ="CheckDefaultTOCStyle">
		<xsl:param name="name" />
		<xsl:choose>
			<xsl:when test="$name = 'TOCHeading' 
                or $name = 'TOC1'
                or $name = 'TOC2'
                or $name = 'TOC3'
                or $name = 'TOC4'
                or $name = 'TOC5'
                or $name = 'TOC6'
                or $name = 'TOC7'
                or $name = 'TOC8'
                or $name = 'TOC9'">true</xsl:when>
			<xsl:otherwise>false</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<!--math: Added for bugfix #1934315 END-->


	<!--template which changes style 'TOC' to style 'Contents_20'-->
	<xsl:template name="TocToContent">
		<xsl:param name="styleValue"/>

		<!--math: Added for bugfix #1934315 START-->
		<xsl:variable name="isDefaultTOCStyle">
			<xsl:call-template name ="CheckDefaultTOCStyle">
				<xsl:with-param name="name">
					<xsl:value-of select="$styleValue" />
				</xsl:with-param>
			</xsl:call-template>
		</xsl:variable>
		<!--math: Added for bugfix #1934315 END-->

		<xsl:choose>
			<!--<xsl:when test="contains($styleValue,'TOC')">-->
			<xsl:when test="$isDefaultTOCStyle = 'true'">
				<xsl:value-of select="concat('Contents_20_',substring-after($styleValue,'TOC'))"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$styleValue"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>



	<!-- searches entry properties through all toc -->
	<xsl:template name="EntryIterator">
		<xsl:param name="fieldCharCount"/>
		<xsl:param name="level"/>
		<xsl:param name="node"/>
		<xsl:param name="instrTextContent"/>
		<xsl:param name="type"/>
		<xsl:variable name="styleLevel">
			<xsl:choose>
				<xsl:when test="contains($instrTextContent,'\t')">
					<xsl:variable name="levelForStyle">
						<xsl:call-template name="GetStyleForLevel">
							<xsl:with-param name="stylesWithLevels">
								<xsl:value-of select="substring-before(substring-after(substring-after($instrTextContent,'\t'),'&quot;'),'&quot;')"/>
							</xsl:with-param>
							<xsl:with-param name="level" select="$level"/>
						</xsl:call-template>
					</xsl:variable>
					<xsl:choose>
						<xsl:when test="$levelForStyle!='NaN'">
							<xsl:value-of select="$levelForStyle"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="$level"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$level"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="count">
			<xsl:choose>
				<xsl:when test="descendant::w:fldChar/@w:fldCharType='begin'">
					<xsl:value-of select="number($fieldCharCount)+1"/>
				</xsl:when>
				<xsl:when test="descendant::w:fldChar/@w:fldCharType='end'">
					<xsl:value-of select="number($fieldCharCount)-1"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="number($fieldCharCount)"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="instrText">
			<xsl:choose>
				<xsl:when test="contains($instrTextContent, 'PAGEREF')">
					<xsl:value-of select="substring-before($instrTextContent, 'PAGEREF')"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$instrTextContent"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:if test="$count &gt; 0">

			<xsl:choose>
				<xsl:when test="$type='INDEXA'">
					<xsl:if test="contains($instrText, '\h') and not(contains($instrText, '\c'))">
						<xsl:call-template name="InsertHyperlinkStartToTOC"/>
					</xsl:if>

					<xsl:choose>
						<xsl:when test="$level = 0">
							<text:index-entry-text/>
						</xsl:when>
						<xsl:otherwise>
							<text:index-entry-text/>

							<!--clam bugfix #1771286-->
							<xsl:choose>
								<xsl:when test="contains($instrText, 'INDEX \e &quot;&quot;')">
									<text:index-entry-tab-stop style:type="right" style:leader-char="."/>
									<text:index-entry-span> </text:index-entry-span>
								</xsl:when>
								<xsl:otherwise>
									<text:index-entry-span>, </text:index-entry-span>
								</xsl:otherwise>
							</xsl:choose>

							<text:index-entry-page-number/>
							<xsl:if test="contains($instrText,'\h') and not(contains($instrText, '\c'))">
								<text:index-entry-link-end/>
							</xsl:if>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
				<xsl:when test="(contains(w:pPr/w:pStyle/@w:val,$styleLevel) and not(contains(preceding-sibling::w:p[(preceding-sibling::node()=$node or self::node()=$node)]/w:pPr/w:pStyle/@w:val,$styleLevel))) or $styleLevel = 0">
					<xsl:if test="contains($instrText, '\h') and not(contains($instrText, '\c'))">
						<xsl:call-template name="InsertHyperlinkStartToTOC"/>
					</xsl:if>

					<xsl:variable name="NumId">
						<xsl:value-of select="string(key('Part', 'word/styles.xml')/w:styles/w:style[@w:styleId=concat('Heading', string($level))]/w:pPr/w:numPr/w:numId/@w:val)"/>
					</xsl:variable>
					<xsl:variable name="AbstractNumId" select="string(key('Part', 'word/numbering.xml')/w:numbering/w:num[@w:numId=$NumId]/w:abstractNumId/@w:val)" />
					<xsl:variable name="Tabs" select="key('Part', 'word/numbering.xml')/w:numbering/w:abstractNum[@w:abstractNumId=$AbstractNumId]/w:lvl[@w:ilvl=number($level)-1]/w:pPr/w:tabs" />


					<xsl:choose>
						<xsl:when test="$Tabs/w:tab">
							<xsl:call-template name="continueEntryIterator">
								<xsl:with-param name="ParagraphPropertiesTabs" select="$Tabs/w:tab | key('StyleId',w:pPr/w:pStyle/@w:val)/w:pPr/w:tabs/w:tab"/>
								<xsl:with-param name="type" select="$type" />
								<xsl:with-param name="instrText" select="$instrText" />
								<xsl:with-param name="level" select="$level" />
							</xsl:call-template>
						</xsl:when>
						<xsl:when test="./w:r/w:instrText">
							<xsl:call-template name="continueEntryIterator">
								<xsl:with-param name="ParagraphPropertiesTabs" select="w:pPr/w:tabs/w:tab | key('StyleId',w:pPr/w:pStyle/@w:val)/w:pPr/w:tabs/w:tab"/>
								<xsl:with-param name="type" select="$type" />
								<xsl:with-param name="instrText" select="$instrText" />
								<xsl:with-param name="level" select="$level" />
							</xsl:call-template>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="continueEntryIterator">
								<xsl:with-param name="ParagraphPropertiesTabs" select="preceding-sibling::node()[w:r/w:instrText]/w:pPr/w:tabs/w:tab | key('StyleId',w:pPr/w:pStyle/@w:val)/w:pPr/w:tabs/w:tab"/>
								<xsl:with-param name="type" select="$type" />
								<xsl:with-param name="instrText" select="$instrText" />
								<xsl:with-param name="level" select="$level" />
							</xsl:call-template>
						</xsl:otherwise>
					</xsl:choose>

				</xsl:when>
				<xsl:otherwise>
					<xsl:for-each select="following-sibling::w:p[1]">
						<xsl:call-template name="EntryIterator">
							<xsl:with-param name="fieldCharCount" select="$count"/>
							<xsl:with-param name="level" select="$level"/>
							<xsl:with-param name="node" select="$node"/>
							<xsl:with-param name="instrTextContent" select="$instrTextContent"/>
							<xsl:with-param name="type" select="$type"/>
						</xsl:call-template>
					</xsl:for-each>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:if>
	</xsl:template>

	<xsl:template name="continueEntryIterator">
		<xsl:param name="ParagraphPropertiesTabs" />
		<xsl:param name="type" />
		<xsl:param name="instrText" />
		<xsl:param name="level" />

		<xsl:variable name="NumId">
			<xsl:value-of select="key('Part', 'word/styles.xml')/w:styles/w:style[@w:styleId=concat('Heading', string($level))]/w:pPr/w:numPr/w:numId/@w:val"/>
		</xsl:variable>
		<xsl:variable name="AbstractNumId" select="key('Part', 'word/numbering.xml')/w:numbering/w:num[@w:numId=$NumId]/w:abstractNumId/@w:val" />
		<xsl:variable name="suff" select="key('Part', 'word/numbering.xml')/w:numbering/w:abstractNum[@w:abstractNumId=$AbstractNumId]/w:lvl[@w:ilvl=number($level)-1]/w:suff" />

		<xsl:choose>
			<xsl:when test="$type='TOC' and not($suff) and count($ParagraphPropertiesTabs) > 1">
				<text:index-entry-chapter/>
				<xsl:apply-templates select="$ParagraphPropertiesTabs[@w:val='left' or @w:val='num'][1]" mode="TOCentry">
					<xsl:with-param name="TOCMODE">true</xsl:with-param>
				</xsl:apply-templates>
				<text:index-entry-text/>
				<xsl:apply-templates select="$ParagraphPropertiesTabs[@w:val='right'][number(last())]" mode="TOCentry">
				</xsl:apply-templates>
				<text:index-entry-page-number />
				<xsl:if test="contains($instrText,'\h') and not(contains($instrText, '\c'))">
					<text:index-entry-link-end/>
				</xsl:if>
			</xsl:when>
			<xsl:otherwise>
				<xsl:if test="$type='TOC'">
					<text:index-entry-chapter/>
					<!--math, dialogika: Bugfix #1947993 and #1828338 BEGIN-->
					<!--Only put space here (between number and text) if either no style with current level = outlineLvl is found or style with outlineLvl has a numbering-->
					<xsl:variable name="StylesWithCurrentOutlineLevel" select="document('word/styles.xml')/w:styles/w:style[w:pPr/w:outlineLvl/@w:val = $level]" />
					<!--<xsl:if test="not($StylesWithCurrentOutlineLevel[1]) or $StylesWithCurrentOutlineLevel/w:pPr/w:numPr/w:numId">
                  <text:index-entry-span>
                    <xsl:text> </xsl:text>
                  </text:index-entry-span>
                </xsl:if>-->
					<!--math, dialogika: Bugfix #1947993 and #1828338 END-->
				</xsl:if>

				<xsl:if test="string-length($NumId) > 0 and not($suff)">
					<text:index-entry-tab-stop>
						<xsl:attribute name="style:type">left</xsl:attribute>
						<xsl:attribute name="style:position">1cm</xsl:attribute>
					</text:index-entry-tab-stop>
				</xsl:if>

				<text:index-entry-text/>
				<xsl:choose>
					<xsl:when test="$type='TOC' and count($ParagraphPropertiesTabs[@w:val='right']) > 0">
						<xsl:apply-templates select="$ParagraphPropertiesTabs[@w:val='right'][number(last())]" mode="TOCentry"/>
					</xsl:when>
					<xsl:when test="$type='TOC'">
						<text:index-entry-tab-stop>
							<xsl:attribute name="style:type">right</xsl:attribute>
							<xsl:call-template name="InsertStyleLeaderChar">
								<xsl:with-param name="leaderChar">dot</xsl:with-param>
							</xsl:call-template>
						</text:index-entry-tab-stop>
					</xsl:when>
					<xsl:otherwise>
						<xsl:apply-templates select="(descendant::w:r/w:tab)[number(last())]" mode="entry"/>
					</xsl:otherwise>
				</xsl:choose>
				<xsl:apply-templates select="(descendant::w:r/w:t)[number(last())]" mode="entry"/>
				<xsl:if test="contains($instrText,'\h') and not(contains($instrText, '\c'))">
					<text:index-entry-link-end/>
				</xsl:if>
			</xsl:otherwise>
		</xsl:choose>

	</xsl:template>


	<!-- template which inserts hyperlink start tag in TOC formatting-->
	<xsl:template name="InsertHyperlinkStartToTOC">
		<text:index-entry-link-start>
			<xsl:attribute name="text:style-name">
				<!--clam: bugfix 1806204-->
				<xsl:choose>
					<xsl:when test="w:hyperlink/w:r/w:rPr/w:rStyle/@w:val = 'Hyperlink'">X3AS7TOCHyperlink</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="w:hyperlink/w:r/w:rPr/w:rStyle/@w:val"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>
		</text:index-entry-link-start>
	</xsl:template>

	<!--insert Bibliography properties -->

	<xsl:template name="InsertBibliographyProperties">

		<xsl:variable name="Style">
			<xsl:value-of select="key('Part', 'customXml/item1.xml')/b:Sources/@StyleName"/>
		</xsl:variable>

		<text:bibliography-entry-template text:bibliography-type="book">
			<xsl:attribute name="text:style-name">
				<xsl:value-of select="generate-id()"/>
			</xsl:attribute>
			<xsl:choose>
				<xsl:when test="$Style = 'APA'">
					<text:index-entry-bibliography text:bibliography-data-field="author"/>
					<text:index-entry-span>.(</text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="year"/>
					<text:index-entry-span>). </text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="title"/>
					<text:index-entry-span>. </text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="address"/>
					<text:index-entry-span>: </text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="publisher"/>
				</xsl:when>
				<xsl:when test="$Style = 'Chicago' or $Style = 'ISO 690 - Numerical Reference' or $Style = 'MLA' or $Style = 'Turabian'">
					<text:index-entry-bibliography text:bibliography-data-field="author"/>
					<text:index-entry-span>. </text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="title"/>
					<text:index-entry-span>. </text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="address"/>
					<text:index-entry-span> : </text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="publisher"/>
					<text:index-entry-span>, </text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="year"/>
					<text:index-entry-span>. </text:index-entry-span>
				</xsl:when>
				<xsl:when test="$Style = 'GB7714'">
					<text:index-entry-bibliography text:bibliography-data-field="author"/>
					<text:index-entry-span>.</text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="year"/>
					<text:index-entry-span>.</text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="title"/>
					<text:index-entry-span>. </text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="address"/>
					<text:index-entry-span>: </text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="publisher"/>
					<text:index-entry-span>, </text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="year"/>
					<text:index-entry-span>. </text:index-entry-span>
				</xsl:when>
				<xsl:when test="$Style = 'GOST - Name Sort'">
					<text:index-entry-bibliography text:bibliography-data-field="author"/>
					<text:index-entry-span> </text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="title"/>
					<text:index-entry-span>[</text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="bibliography-type"/>
					<text:index-entry-span>].-</text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="address"/>
					<text:index-entry-span> : </text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="publisher"/>
					<text:index-entry-span>, </text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="year"/>
					<text:index-entry-span>. </text:index-entry-span>
				</xsl:when>
				<xsl:when test="$Style = 'GOST - Title Sort'">
					<text:index-entry-bibliography text:bibliography-data-field="title"/>
					<text:index-entry-span> </text:index-entry-span>
					<text:index-entry-span>[</text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="bibliography-type"/>
					<text:index-entry-span>]/ AUT. </text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="author"/>
					<text:index-entry-span>. - </text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="address"/>
					<text:index-entry-span> : </text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="publisher"/>
					<text:index-entry-span>, </text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="year"/>
					<text:index-entry-span>. </text:index-entry-span>
				</xsl:when>
				<xsl:when test="$Style = 'ISO 690 - First Element and Date'">

					<text:index-entry-bibliography text:bibliography-data-field="author"/>
					<text:index-entry-span>. </text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="year"/>
					<text:index-entry-span>. </text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="title"/>
					<text:index-entry-span>. </text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="address"/>
					<text:index-entry-span> : </text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="publisher"/>
					<text:index-entry-span>, </text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="year"/>
					<text:index-entry-span>. </text:index-entry-span>
				</xsl:when>
				<xsl:when test="$Style = 'SIST02'">
					<text:index-entry-bibliography text:bibliography-data-field="author"/>
					<text:index-entry-span> </text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="title"/>
					<text:index-entry-span>. </text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="address"/>
					<text:index-entry-span>, </text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="publisher"/>
					<text:index-entry-span>, </text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="year"/>
					<text:index-entry-span>. </text:index-entry-span>
				</xsl:when>
				<xsl:otherwise>
					<text:index-entry-bibliography text:bibliography-data-field="author"/>
					<text:index-entry-span>. </text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="title"/>
					<text:index-entry-span>. </text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="address"/>
					<text:index-entry-span> : </text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="publisher"/>
					<text:index-entry-span>, </text:index-entry-span>
					<text:index-entry-bibliography text:bibliography-data-field="year"/>
					<text:index-entry-span>. </text:index-entry-span>
				</xsl:otherwise>
			</xsl:choose>
		</text:bibliography-entry-template>
	</xsl:template>

	<!-- insert page number -->
	<xsl:template match="w:t" mode="entry">
		<xsl:variable name="text">
			<xsl:value-of select="child::text()"/>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="number($text)">
				<text:index-entry-page-number/>
			</xsl:when>
			<xsl:otherwise/>
		</xsl:choose>
	</xsl:template>

	<!-- clam, DIaLOGIKa: this template is only used inside the TOC -->
	<!-- insert entry properties for tabs -->
	<xsl:template match="w:tab" mode="TOCentry">
		<xsl:param name="TOCMODE"></xsl:param>
		<xsl:variable name="tabCount">
			<xsl:value-of select="count(parent::w:r/preceding-sibling::w:r/w:tab)+1"/>
		</xsl:variable>

		<xsl:variable name="leaderChar">
			<xsl:value-of select="@w:leader"/>
		</xsl:variable>

		<!-- clam, DIaLOGIKa: in ODT, only 'left' tabs can have a position in the TOC-->
		<xsl:variable name="styleType">
			<xsl:choose>
				<xsl:when test="@w:val='num'">left</xsl:when>
				<!-- clam, DIaLOGIKa: the 8800 should be replaced by a computed value. It is used to distinguish 'real' right tabs (pointing to the line end) from other ones -->
				<xsl:when test="@w:val='right' and @w:pos and number(@w:pos) &lt; 8800">left</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="@w:val"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:if test="$styleType != '' and $styleType != 'clear'">

			<!--math, dialogika: bugfix #1804154 BEGIN-->

			<xsl:variable name="InstrText">
				<xsl:value-of select="key('Part', 'word/document.xml')/w:document/w:body//w:instrText" />
			</xsl:variable>


			<text:index-entry-tab-stop style:type="{$styleType}">
				<!--if style type is left, there must be style:position attribute -->

				<xsl:if test="$styleType = 'left'">
					<xsl:attribute name="style:position">
						<xsl:variable name="position">
							<xsl:value-of select="@w:pos"/>
						</xsl:variable>
						<xsl:call-template name="ConvertTwips">
							<xsl:with-param name="length" select="$position"/>
							<xsl:with-param name="unit">cm</xsl:with-param>
						</xsl:call-template>
					</xsl:attribute>
				</xsl:if>
				<xsl:if test="$leaderChar and $leaderChar!='' and $leaderChar!='heavy' and $leaderChar!='middleDot' and $leaderChar!='none'">
					<xsl:call-template name="InsertStyleLeaderChar">
						<xsl:with-param name="leaderChar" select="$leaderChar"/>
					</xsl:call-template>
				</xsl:if>

			</text:index-entry-tab-stop>

		</xsl:if>
	</xsl:template>

	<!-- insert entry properties for tabs -->
	<xsl:template match="w:tab[not(parent::w:tabs)]" mode="entry">
		<xsl:param name="TOCMODE"></xsl:param>
		<xsl:variable name="tabCount">
			<xsl:value-of select="count(parent::w:r/preceding-sibling::w:r/w:tab)+1"/>
		</xsl:variable>
		<xsl:variable name="styleType">
			<xsl:call-template name="GetTabParams">
				<xsl:with-param name="param">w:val</xsl:with-param>
				<xsl:with-param name="tabCount" select="$tabCount"/>
				<xsl:with-param name="maxtabCount" select="$tabCount"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:variable name="leaderChar">
			<xsl:call-template name="GetTabParams">
				<xsl:with-param name="param">w:leader</xsl:with-param>
				<xsl:with-param name="tabCount" select="$tabCount"/>
				<xsl:with-param name="maxtabCount" select="$tabCount"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:if test="$styleType != '' and $styleType != 'clear'">

			<!--math, dialogika: bugfix #1804154 BEGIN-->

			<xsl:variable name="InstrText">
				<xsl:value-of select="key('Part', 'word/document.xml')/w:document/w:body//w:instrText" />
			</xsl:variable>
			<xsl:choose>

				<!--if \p is not contained, word always uses right align. Propably, use of '\p' can not be correctly converted to ODT
            The existing code in this bugfix has not been changed for this case-->
				<xsl:when test="not($TOCMODE) and not(contains($InstrText,'\p'))">
					<text:index-entry-tab-stop style:type="right" >
						<xsl:variable name="StyleId">
							<xsl:value-of select="../../w:pPr/w:pStyle/@w:val" />
						</xsl:variable>

						<xsl:variable name="StyleLeaderChar">
							<xsl:value-of select="key('StyleId',$StyleId)/w:pPr/w:tabs/w:tab/@w:leader" />
						</xsl:variable>

						<!--default: 'dot'-->
						<xsl:if test="$StyleLeaderChar=''">
							<xsl:call-template name="InsertStyleLeaderChar">
								<xsl:with-param name="leaderChar">dot</xsl:with-param>
							</xsl:call-template>
						</xsl:if>

						<xsl:if test="$StyleLeaderChar and $StyleLeaderChar!='' and $StyleLeaderChar!='heavy' and $StyleLeaderChar!='middleDot' and $StyleLeaderChar!='none'">
							<xsl:call-template name="InsertStyleLeaderChar">
								<xsl:with-param name="leaderChar" select="$StyleLeaderChar"/>
							</xsl:call-template>
						</xsl:if>

					</text:index-entry-tab-stop>
				</xsl:when>

				<!--math, dialogika: bugfix #1804154 END-->


				<xsl:otherwise>

					<text:index-entry-tab-stop style:type="{$styleType}">
						<!--if style type is left, there must be style:position attribute -->

						<xsl:if test="$styleType = 'left'">
							<xsl:attribute name="style:position">
								<xsl:variable name="position">
									<xsl:call-template name="GetTabParams">
										<xsl:with-param name="param">w:pos</xsl:with-param>
										<xsl:with-param name="tabCount" select="$tabCount"/>
										<xsl:with-param name="maxtabCount" select="$tabCount"/>
									</xsl:call-template>
								</xsl:variable>
								<xsl:call-template name="ConvertTwips">
									<xsl:with-param name="length" select="$position"/>
									<xsl:with-param name="unit">cm</xsl:with-param>
								</xsl:call-template>
							</xsl:attribute>
						</xsl:if>
						<xsl:if test="$leaderChar and $leaderChar!='' and $leaderChar!='heavy' and $leaderChar!='middleDot' and $leaderChar!='none'">
							<xsl:call-template name="InsertStyleLeaderChar">
								<xsl:with-param name="leaderChar" select="$leaderChar"/>
							</xsl:call-template>
						</xsl:if>

					</text:index-entry-tab-stop>

				</xsl:otherwise>

			</xsl:choose>

		</xsl:if>
	</xsl:template>

	<!-- insert tab-leader char -->
	<xsl:template name="InsertStyleLeaderChar">
		<xsl:param name="leaderChar"/>
		<xsl:attribute name="style:leader-char">
			<xsl:choose>
				<xsl:when test="$leaderChar='dot'">.</xsl:when>
				<xsl:when test="$leaderChar='hyphen'">-</xsl:when>
				<xsl:when test="$leaderChar='underscore'">_</xsl:when>
				<xsl:otherwise>none</xsl:otherwise>
			</xsl:choose>
		</xsl:attribute>
	</xsl:template>

	<!-- get properties of tabs -->
	<xsl:template name="GetTabParams">
		<xsl:param name="param"/>
		<xsl:param name="tabCount"/>
		<xsl:param name="maxtabCount"/>
		<xsl:choose>
			<xsl:when test="$tabCount > 0">
				<xsl:variable name="partName" select="ancestor::oox:part/@oox:name" />
				<xsl:variable name="precTab" select="preceding::w:tabs[ancestor::oox:part/@oox:name = $partName]" />
				<xsl:choose>
					<xsl:when test="$precTab/w:tab[number($tabCount)][not($param='w:val' and @w:val='clear')]">
						<xsl:choose>
							<xsl:when test="$precTab/w:tab[number($tabCount)]/attribute::node()[name()=$param]">
								<xsl:value-of select="$precTab/w:tab[number($tabCount)]/attribute::node()[name()=$param]"/>
							</xsl:when>
							<xsl:otherwise/>
						</xsl:choose>
					</xsl:when>
					<xsl:otherwise>
						<xsl:call-template name="GetTabParams">
							<xsl:with-param name="param" select="$param"/>
							<xsl:with-param name="tabCount" select="number($tabCount)-1"/>
							<xsl:with-param name="maxtabCount" select="$maxtabCount"/>
						</xsl:call-template>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:otherwise>
				<xsl:call-template name="GetTabParamsFromStyles">
					<xsl:with-param name="tabStyle">
						<xsl:value-of select="ancestor::w:p/w:pPr/w:pStyle/@w:val"/>
					</xsl:with-param>
					<xsl:with-param name="attribute" select="$param"/>
					<xsl:with-param name="tabCount" select="$maxtabCount"/>
					<xsl:with-param name="maxtabCount" select="$maxtabCount"/>
				</xsl:call-template>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!--get properties of tabs from styles.xml -->
	<xsl:template name="GetTabParamsFromStyles">
		<xsl:param name="tabStyle"/>
		<xsl:param name="attribute"/>
		<xsl:param name="tabCount"/>
		<xsl:param name="maxtabCount"/>

		<xsl:variable name="ancestor-style" select="ancestor::w:p/w:pPr/w:pStyle/@w:val"/>

		<xsl:for-each select="key('Part', 'word/styles.xml')">
			<xsl:choose>
				<xsl:when test="$tabCount > 0">
					<xsl:choose>
						<xsl:when test="key('StyleId', $tabStyle)[1]/w:pPr/w:tabs/w:tab[number($tabCount)][not($attribute='w:val' and @w:val='clear')]">
							<xsl:choose>
								<xsl:when test="key('StyleId', $tabStyle)[1]/w:pPr/w:tabs/w:tab[number($tabCount)]/attribute::node()[name()=$attribute]">
									<xsl:value-of select="key('StyleId', $tabStyle)[1]/w:pPr/w:tabs/w:tab[number($tabCount)]/attribute::node()[name()=$attribute]"/>
								</xsl:when>
								<xsl:otherwise/>
							</xsl:choose>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="GetTabParamsFromStyles">
								<xsl:with-param name="tabStyle">
									<xsl:value-of select="$ancestor-style"/>
								</xsl:with-param>
								<xsl:with-param name="attribute" select="$attribute"/>
								<xsl:with-param name="tabCount" select="number($tabCount)-1"/>
								<xsl:with-param name="maxtabCount" select="$maxtabCount"/>
							</xsl:call-template>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
				<xsl:when test="key('StyleId', $tabStyle)[1]/w:basedOn/@w:val">
					<xsl:call-template name="GetTabParamsFromStyles">
						<xsl:with-param name="tabStyle">
							<xsl:value-of select="key('StyleId', $tabStyle)[1]/w:basedOn/@w:val"/>
						</xsl:with-param>
						<xsl:with-param name="attribute" select="$attribute"/>
						<xsl:with-param name="tabCount" select="$maxtabCount"/>
						<xsl:with-param name="maxtabCount" select="$maxtabCount"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:otherwise/>
			</xsl:choose>
		</xsl:for-each>
	</xsl:template>

	<!-- handle text in table-of content -->
	<xsl:template match="text()" mode="entry"/>



	<!--
  makz: There is no need for a special handling for runs in TOCs
  
  <xsl:template match="w:r" mode="index">
    <xsl:choose>
      <xsl:when test="parent::w:hyperlink">
        <xsl:call-template name="InsertTOCHyperlink"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="InsertTOCText"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  <xsl:template match="w:r" mode="indexa">
    <xsl:choose>
      <xsl:when test="parent::w:hyperlink">
        <xsl:call-template name="InsertTOCHyperlink"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="InsertTOCText"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  -->
	<!-- Insert TOC text -->
	<!--
  <xsl:template name="InsertTOCText">
    <xsl:choose>
      -->
	<!--  ignore text when we are in field-->
	<!--
      <xsl:when test="w:fldChar or w:instrText"/>
      -->
	<!--attach style-->
	<!--
      <xsl:when test="w:rPr">
        <text:span text:style-name="{generate-id(self::node())}">
          <xsl:apply-templates/>
        </text:span>
      </xsl:when>
      -->
	<!--default scenario-->
	<!--
      <xsl:otherwise>
        <xsl:apply-templates />
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

 -->
	<!-- Insert TOC Hyperlinks -->
	<!--
  <xsl:template name="InsertTOCHyperlink">    
    <text:a xlink:type="simple">
      <xsl:attribute name="xlink:href">
        <xsl:value-of select="concat('#',parent::w:hyperlink/@w:anchor)"/>
      </xsl:attribute>
      -->
	<!--TOC hyperlink text-->
	<!--
      <xsl:call-template name="InsertTOCText"/>
    </text:a>
  </xsl:template>
  -->

	<!-- template to search TOC level in paragraph parent style -->
	<xsl:template match="w:p" mode="toclevel">
		<xsl:param name="level"/>
		<xsl:variable name="thisLevel">
			<xsl:value-of select="number(substring(w:pPr/w:pStyle/@w:val,string-length(w:pPr/w:pStyle/@w:val)))"/>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="following-sibling::w:p[1][count(preceding::w:fldChar[@w:fldCharType='begin']) &gt; count(preceding::w:fldChar[@w:fldCharType='end']) and descendant::text()]">
				<xsl:apply-templates select="following-sibling::w:p[1]" mode="toclevel">
					<xsl:with-param name="level">
						<xsl:choose>
							<xsl:when test="$thisLevel &gt; $level">
								<xsl:value-of select="$thisLevel"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="$level"/>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:with-param>
				</xsl:apply-templates>
			</xsl:when>
			<xsl:when test="$thisLevel &gt; $level">
				<xsl:value-of select="$thisLevel"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$level"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
</xsl:stylesheet>
