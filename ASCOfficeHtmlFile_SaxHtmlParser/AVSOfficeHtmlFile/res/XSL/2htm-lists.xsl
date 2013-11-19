<?xml version="1.0" encoding="UTF-8"?>
<!--
 * Copyright (c) 2006, Clever Age
 * Copyright (c) 2008, DIaLOGIKa
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
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
				xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
				xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
				xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
				xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
				xmlns:dc="http://purl.org/dc/elements/1.1/"
				xmlns:v="urn:schemas-microsoft-com:vml"
				xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
				xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"
				xmlns:oox="urn:oox"
                xmlns="http://www.w3.org/1999/xhtml"
				exclude-result-prefixes="w v r oox">

	<!-- a key on all numberings defined in numbering.xml -->
	<xsl:key name="numId" match="w:num" use="@w:numId"/>
	<xsl:key name="abstractNumId" match="w:abstractNum" use="@w:abstractNumId"/>
	<xsl:key name="numPicBullet" match="w:numPicBullet " use="@w:numPicBulletId"/>
	<xsl:key name="paragraphsByStyleId" match="w:document/w:body/w:p" use="w:pPr/w:pStyle/@w:val"/>

	<!--insert num template for each text-list style -->
	<xsl:template match="w:num">

		<!-- apply abstractNum template with the same id -->
		<xsl:apply-templates select="key('abstractNumId',w:abstractNumId/@w:val)">
			<xsl:with-param name="id" select="@w:numId"/>
		</xsl:apply-templates>

	</xsl:template>

	<!-- insert abstractNum template -->
	<xsl:template match="w:abstractNum">
		<xsl:param name="id"/>
		<xsl:variable name="listStyleName">
			<xsl:choose>

				<!-- if there is w:lvlOverride, numbering properties can be taken from w:num, and list style must be referred to numId -->
				<xsl:when test="key('numId',$id)/w:lvlOverride">
					<xsl:value-of select="concat('LO',$id)"/>
				</xsl:when>

				<!-- math, dialogika: bugfix #1771273 list style is referred to NumId to avoid duplicate list styles BEGIN-->
				<xsl:otherwise>
					<!--<xsl:value-of select="concat('L',@w:abstractNumId)"/>-->
					<xsl:value-of select="concat('L',$id)"/>
				</xsl:otherwise>
				<!-- math, dialogika: bugfix #1771273 list style is referred to NumId to avoid duplicate list styles END-->

			</xsl:choose>
		</xsl:variable>
		<xsl:for-each select="w:lvl">
			<xsl:variable name="level" select="@w:ilvl"/>
			<xsl:choose>

				<!-- when numbering style is overriden, num template is used -->
				<xsl:when test="key('numId',$id)/w:lvlOverride[@w:ilvl = $level]/w:lvl">

					<!--math, dialogika: changed for correct indentation calculation BEGIN -->
					<!--added parameter <xsl:with-param name="numId">-->
					<xsl:apply-templates select="key('numId',$id)/w:lvlOverride[@w:ilvl = $level]/w:lvl[@w:ilvl = $level]">
						<xsl:with-param name="numId" select="$id"/>
						<xsl:with-param name="listStyleName" select="$listStyleName"/>
					</xsl:apply-templates>
					<!--math, dialogika: changed for correct indentation calculation END -->

				</xsl:when>

				<xsl:otherwise>
					<xsl:apply-templates select=".">
						<xsl:with-param name="numId" select="$id"/>
						<xsl:with-param name="listStyleName" select="$listStyleName"/>
					</xsl:apply-templates>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:for-each>
	</xsl:template>

	<xsl:template match="w:lvl">
		<!--math, dialogika: changed for correct indentation calculation BEGIN -->
		<!--added parameter <xsl:param name="numId">-->
		<xsl:param name="numId"/>
		<xsl:param name="listStyleName"/>
		<!--math, dialogika: changed for correct indentation calculation END -->

		<xsl:variable name="lvl" select="number(@w:ilvl)+1"/>

		<xsl:text> li.</xsl:text>
		<!--<xsl:text> ul.</xsl:text>-->
		<xsl:value-of select="concat($listStyleName, '_', $lvl)"/>
		<xsl:text> { </xsl:text>

		<xsl:choose>
			<!-- if numFmt is none, nothing to do -->
			<xsl:when test="w:numFmt/@w:val = 'none' ">
				<xsl:text>list-style-type:none; </xsl:text>
			</xsl:when>

			<!--check if it's numbering, bullet or picture bullet -->
			<!--AVSMedia: TODO: uncomment and rework later-->
			<!--
                <xsl:when test="w:numFmt[@w:val = 'bullet'] and w:lvlPicBulletId/@w:val != ''">

                    <xsl:variable name="document">
                        <xsl:call-template name="GetDocumentName"/>
                    </xsl:variable>

                    <xsl:variable name="lvlPicBulletId">
                        <xsl:value-of select="w:lvlPicBulletId/@w:val"/>
                    </xsl:variable>

                    <xsl:variable name="rId">
                        <xsl:value-of
                          select="key('numPicBullet', $lvlPicBulletId)/w:pict/v:shape/v:imagedata/@r:id"/>
                    </xsl:variable>

                    <xsl:variable name="XlinkHref">
                        <xsl:variable name="pzipsource">
                            <xsl:value-of select="key('Part', concat('word/_rels/',$document,'.rels'))//node()[name() = 'Relationship'][@Id=$rId]/@Target" />
                        </xsl:variable>
                        <xsl:value-of select="concat('Pictures/', substring-after($pzipsource,'/'))"/>
                    </xsl:variable>

                    <xsl:call-template name="CopyPictures">
                        <xsl:with-param name="document" select="$document"/>
                        <xsl:with-param name="rId" select="$rId"/>
                    </xsl:call-template>

                    <text:list-level-style-image>
                        <xsl:attribute name="text:level">
                            <xsl:value-of select="$lvl"/>
                        </xsl:attribute>
                        <xsl:attribute name="href">
                            <xsl:value-of select="$XlinkHref"/>
                        </xsl:attribute>
                        <style:list-level-properties>

                            <!-@-math, dialogika: changed for correct indentation calculation BEGIN -@->
                            <!-@-added parameter <xsl:with-param name="numId">-@->
                            <xsl:call-template name="InsertListLevelProperties">
                                <xsl:with-param name="numId" select="$numId"/>
                            </xsl:call-template>
                            <!-@-math, dialogika: changed for correct indentation calculation END -@->

                            <style:text-properties fo:font-size="96"/>
                        </style:list-level-properties>
                    </text:list-level-style-image>
                </xsl:when>
                -->

			<xsl:when test="w:numFmt[@w:val = 'bullet']">
				<xsl:text>list-style-type:</xsl:text>

				<xsl:call-template name="BulletFormat">
					<xsl:with-param name="symbol" select="w:lvlText/@w:val"/>
				</xsl:call-template>

				<xsl:text>;</xsl:text>
				<!--<xsl:attribute name="text:level">
                            <xsl:value-of select="$lvl"/>
                        </xsl:attribute>-->
				<!--<xsl:choose>
                            <!-@-clam: special style for bullets with symbol font (bug #1806059)-@->
                            <xsl:when test="w:rPr/w:rFonts/@w:ascii='Symbol'">
                                <xsl:attribute name="text:style-name">BulletSymbol</xsl:attribute>
                            </xsl:when>
                            <xsl:otherwise />
                        </xsl:choose>-->
				<!--<xsl:attribute name="text:bullet-char">
                            <xsl:call-template name="TextChar"/>
                        </xsl:attribute>-->

				<!--math, dialogika: changed for correct indentation calculation BEGIN -->
				<xsl:call-template name="InsertListLevelProperties">
					<xsl:with-param name="numId" select="$numId"/>
				</xsl:call-template>
				<!--math, dialogika: changed for correct indentation calculation END -->

				<!--<xsl:for-each select="w:rPr">
                        <xsl:call-template name="InsertTextProperties"/>
                    </xsl:for-each>-->
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>list-style-type:</xsl:text>
				<!--<xsl:attribute name="text:level">
                            <xsl:value-of select="$lvl"/>
                        </xsl:attribute>
                        <xsl:if test="w:rPr/w:rStyle">
                            <xsl:attribute name="text:style-name">
                                <xsl:value-of select="w:rPr/w:rStyle"/>
                            </xsl:attribute>
                        </xsl:if>
                        <xsl:if test="not(number(substring(w:lvlText/@w:val,string-length(w:lvlText/@w:val))))">
                            <xsl:attribute name="style:num-suffix">
                                <xsl:value-of select="substring(w:lvlText/@w:val,string-length(w:lvlText/@w:val))"/>
                            </xsl:attribute>
                        </xsl:if>-->
				<xsl:call-template name="NumFormat">
					<xsl:with-param name="format" select="w:numFmt/@w:val"/>
					<xsl:with-param name="BeforeAfterNum" select="w:lvlText/@w:val"/>
				</xsl:call-template>
				<xsl:text>; </xsl:text>
				<!--<xsl:choose>
                        <!-@-
						  dialogika, clam: bugfix #1831298
						  dialogika, makz: bugfix #1827565; changed that it works dynamically for more than 2 levels
						  -@->
                        <xsl:when test="starts-with(w:lvlText/@w:val, concat('%',$lvl))">
                            <xsl:attribute name="style:num-letter-sync">true</xsl:attribute>
                            <xsl:choose>
                                <xsl:when test="w:start and w:start/@w:val > 1">
                                    <xsl:attribute name="text:start-value">
                                        <xsl:value-of select="w:start/@w:val"/>
                                    </xsl:attribute>
                                </xsl:when>
                                <xsl:otherwise>
                                    <xsl:variable name="cnt">
                                        <xsl:call-template name="substring-count">
                                            <xsl:with-param name="string" select="w:lvlText/@w:val" />
                                            <xsl:with-param name="occurrence" select="concat('%',$lvl)" />
                                        </xsl:call-template>
                                    </xsl:variable>

                                    <xsl:attribute name="text:start-value">
                                        <xsl:value-of select="1 + (($cnt - 1)*26)"/>
                                    </xsl:attribute>
                                </xsl:otherwise>
                            </xsl:choose>
                        </xsl:when>
                        <xsl:otherwise>
                            <xsl:if test="w:start and w:start/@w:val > 1">
                                <xsl:attribute name="text:start-value">
                                    <xsl:value-of select="w:start/@w:val"/>
                                </xsl:attribute>
                            </xsl:if>
                            <xsl:variable name="display">
                                <xsl:call-template name="CountDisplayListLevels">
                                    <xsl:with-param name="string" select="./w:lvlText/@w:val"/>
                                    <xsl:with-param name="count">0</xsl:with-param>
                                </xsl:call-template>
                            </xsl:variable>
                            <xsl:if test="$display &gt; 1">
                                <xsl:attribute name="text:display-levels">
                                    <xsl:value-of select="$display"/>
                                </xsl:attribute>
                            </xsl:if>
                        </xsl:otherwise>
                    </xsl:choose>
                    -->


				<!--math, dialogika: changed for correct indentation calculation BEGIN -->
				<!--added parameter <xsl:with-param name="numId">-->
				<xsl:call-template name="InsertListLevelProperties">
					<xsl:with-param name="numId" select="$numId"/>
				</xsl:call-template>
				<!--math, dialogika: changed for correct indentation calculation END -->
			</xsl:otherwise>
		</xsl:choose>

		<xsl:text disable-output-escaping="yes">} &#10;</xsl:text>
	</xsl:template>

	<!-- buller symbol -->
	<xsl:template name="BulletFormat">
		<xsl:param name="symbol"/>

		<xsl:choose>
			<xsl:when  test="$symbol = ''">
				<xsl:text>circle</xsl:text>
			</xsl:when>
			<xsl:when  test="$symbol = 'o'">
				<xsl:text>circle</xsl:text>
			</xsl:when>
			<xsl:when  test="$symbol = '•'">
				<xsl:text>disc</xsl:text>
			</xsl:when>
			<xsl:when  test="$symbol = ''">
				<xsl:text>square</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>disc</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<!-- numbering format -->
	<xsl:template name="NumFormat">
		<xsl:param name="format"/>
		<xsl:param name="BeforeAfterNum"/>

		<xsl:variable name="result">
			<!--<xsl:if test="$BeforeAfterNum != ''">-->
			<xsl:choose>
				<xsl:when test="$format= 'decimal' ">decimal</xsl:when>
				<xsl:when test="$format= 'lowerRoman' ">lower-roman</xsl:when>
				<xsl:when test="$format= 'upperRoman' ">upper-roman</xsl:when>
				<xsl:when test="$format= 'lowerLetter' ">lower-alpha</xsl:when>
				<xsl:when test="$format= 'upperLetter' ">upper-alpha</xsl:when>
				<xsl:otherwise>decimal</xsl:otherwise>
			</xsl:choose>
			<!--</xsl:if>-->
		</xsl:variable>

		<xsl:value-of select="$result"/>

		<!--<xsl:if test="$BeforeAfterNum != ''">
            <xsl:variable name="NumPrefix">
                <xsl:value-of select="substring-before($BeforeAfterNum, '%')"/>
            </xsl:variable>
            <xsl:if test="$NumPrefix != ''">
                <xsl:attribute name="style:num-prefix">
                    <xsl:value-of select="$NumPrefix"/>
                </xsl:attribute>
            </xsl:if>
            <xsl:variable name="NumSuffix">
                <xsl:call-template name="AfterTextNumber">
                    <!-@-math, dialogika: this is wrong:-@->
                    <!-@-!!! Parameter name used here is different as parameter name in template -> NumSuffix = NULL always !!!-@->
                    <xsl:with-param name="string" select="$BeforeAfterNum"/>
                </xsl:call-template>
            </xsl:variable>
            <xsl:if test="$NumSuffix != ''">
                <xsl:attribute name="style:num-suffix">
                    <xsl:value-of select="$NumSuffix"/>
                </xsl:attribute>
            </xsl:if>
        </xsl:if>-->
	</xsl:template>

	<!-- text after numbering in list -->
	<xsl:template name="AfterTextNumber">
		<xsl:param name="BeforeAfterNum"/>
		<xsl:choose>
			<xsl:when test="contains(substring-after($BeforeAfterNum,'%'), '%')">
				<xsl:call-template name="AfterTextNumber">
					<xsl:with-param name="BeforeAfterNum" select="substring-after($BeforeAfterNum,'%')"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:choose>
					<xsl:when test="substring(substring-after($BeforeAfterNum, '%'), 2) != ''">
						<xsl:value-of select="substring(substring-after($BeforeAfterNum, '%'), 2)"/>
					</xsl:when>
					<xsl:otherwise/>
				</xsl:choose>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!--math, dialogika: changed for correct indentation calculation BEGIN -->

	<!-- properties for each list level -->
	<xsl:template name="InsertListLevelProperties">
		<xsl:param name="numId"/>

		<xsl:variable name="Ind" select="w:pPr/w:ind"/>

		<xsl:variable name="ListStyleInd" select="w:pPr/w:ind"/>

		<xsl:variable name="tab" select="w:pPr/w:tabs/w:tab/@w:pos"/>

		<xsl:variable name="abstractNumId" select="parent::w:abstractNum/@w:abstractNumId"/>

		<xsl:variable name="ilvl" select="@w:ilvl"/>

		<xsl:variable name="StyleId" select="w:pStyle/@w:val"/>

		<xsl:variable name="paragraph_ref_this_list_level"
		  select="key('Part', 'word/document.xml')/w:document/w:body/w:p[w:pPr/w:numPr/w:numId/@w:val=$numId and w:pPr/w:numPr/w:ilvl/@w:val=$ilvl][1]"/>

		<xsl:variable name="paragraph_ref_this_styleid" select="key('paragraphsByStyleId', $StyleId)[1]"/>

		<xsl:variable name="style" select="key('StyleId', $StyleId)[1]"/>

		<xsl:variable name="Hanging">
			<xsl:choose>
				<xsl:when test="$paragraph_ref_this_list_level">
					<!--Paragraph directly referencing this numID exists-->
					<xsl:choose>
						<xsl:when test="$paragraph_ref_this_list_level/w:pPr/w:ind/@w:hanging">
							<!--Paragraph has direct formatting -> take this value-->
							<xsl:value-of select="$paragraph_ref_this_list_level/w:pPr/w:ind/@w:hanging"/>
						</xsl:when>

						<!--FIRSTLINE: Paragraph has direct formatting firstLine -> firstLine counts-->
						<xsl:when test="$paragraph_ref_this_list_level/w:pPr/w:ind/@w:firstLine">0</xsl:when>

						<xsl:otherwise>
							<!--Paragraph has *NO* direct formatting-->
							<xsl:choose>
								<xsl:when test="$ListStyleInd/@w:hanging">
									<!--take list style value-->
									<xsl:value-of select="$ListStyleInd/@w:hanging"/>
								</xsl:when>
								<xsl:otherwise>0</xsl:otherwise>
							</xsl:choose>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>

				<!--Paragraph directly referencing this numID does *NOT* exist-->

				<xsl:when test="$paragraph_ref_this_styleid/w:pPr/w:ind/@w:hanging">
					<!-- Use direct formatting of paragraph referencing this paragraph style-->
					<xsl:value-of select="$paragraph_ref_this_styleid/w:pPr/w:ind/@w:hanging"/>
				</xsl:when>

				<!--FIRSTLINE: direct formatting of paragraph referencing this paragraph style has firstLine -> firstLine counts-->
				<xsl:when test="$paragraph_ref_this_styleid/w:pPr/w:ind/@w:firstLine">0</xsl:when>

				<xsl:when test="$style/w:pPr/w:ind/@w:hanging">
					<!-- Use paragraph style value-->
					<xsl:value-of select="$style/w:pPr/w:ind/@w:hanging"/>
				</xsl:when>

				<!--FIRSTLINE: Paragraph style has firstLine -> firstLine counts-->
				<xsl:when test="$style/w:pPr/w:ind/@w:firstLine">0</xsl:when>

				<xsl:when test="w:pPr/w:ind/@w:hanging">
					<!-- Use list style value-->
					<xsl:value-of select="w:pPr/w:ind/@w:hanging"/>
				</xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>


		<xsl:variable name="Left">
			<xsl:choose>
				<xsl:when test="$paragraph_ref_this_list_level">
					<!--Paragraph directly referencing this numID exists-->
					<xsl:choose>
						<xsl:when test="$paragraph_ref_this_list_level/w:pPr/w:ind/@w:left">
							<!--Paragraph has direct formatting -> take this value-->
							<xsl:value-of select="$paragraph_ref_this_list_level/w:pPr/w:ind/@w:left"/>
						</xsl:when>
						<xsl:otherwise>
							<!--Paragraph has *NO* direct formatting-->
							<xsl:choose>
								<xsl:when test="$ListStyleInd/@w:left">
									<!--take list style value-->
									<xsl:value-of select="$ListStyleInd/@w:left"/>
								</xsl:when>
								<xsl:otherwise>0</xsl:otherwise>
							</xsl:choose>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>

				<!--Paragraph directly referencing this numID does *NOT* exist-->

				<xsl:when test="$paragraph_ref_this_styleid/w:pPr/w:ind/@w:left">
					<!-- Use direct formatting of paragraph referencing this paragraph style-->
					<xsl:value-of select="$paragraph_ref_this_styleid/w:pPr/w:ind/@w:left"/>
				</xsl:when>

				<xsl:when test="$style/w:pPr/w:ind/@w:left">
					<!-- Use paragraph style value-->
					<xsl:value-of select="$style/w:pPr/w:ind/@w:left"/>
				</xsl:when>
				<xsl:when test="w:pPr/w:ind/@w:left">
					<!-- Use list style value-->
					<xsl:value-of select="w:pPr/w:ind/@w:left"/>
				</xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>


		<xsl:variable name="FirstLine">
			<xsl:choose>
				<xsl:when test="$paragraph_ref_this_list_level">
					<!--Paragraph directly referencing this numID exists-->
					<xsl:choose>
						<xsl:when test="$paragraph_ref_this_list_level/w:pPr/w:ind/@w:firstLine">
							<!--Paragraph has direct formatting -> take this value-->
							<xsl:value-of select="$paragraph_ref_this_list_level/w:pPr/w:ind/@w:firstLine"/>
						</xsl:when>
						<xsl:otherwise>
							<!--Paragraph has *NO* direct formatting-->
							<xsl:choose>
								<xsl:when test="$ListStyleInd/@w:firstLine">
									<!--take list style value-->
									<xsl:value-of select="$ListStyleInd/@w:firstLine"/>
								</xsl:when>
								<xsl:otherwise>0</xsl:otherwise>
							</xsl:choose>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>

				<!--Paragraph directly referencing this numID does *NOT* exist-->
				<xsl:when test="$paragraph_ref_this_styleid/w:pPr/w:ind/@w:firstLine">
					<!-- Use direct formatting of paragraph referencing this paragraph style-->
					<xsl:value-of select="$paragraph_ref_this_styleid/w:pPr/w:ind/@w:firstLine"/>
				</xsl:when>

				<xsl:when test="$style/w:pPr/w:ind/@w:firstLine">
					<!-- Use paragraph style value-->
					<xsl:value-of select="$style/w:pPr/w:ind/@w:firstLine"/>
				</xsl:when>
				<xsl:when test="w:pPr/w:ind/@w:firstLine">
					<!-- Use list style value-->
					<xsl:value-of select="w:pPr/w:ind/@w:firstLine"/>
				</xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="tabs"
		  select="$paragraph_ref_this_list_level/w:pPr/w:tabs | $paragraph_ref_this_styleid/w:pPr/w:tabs | $style/w:pPr/w:tabs | w:pPr/w:tabs" />

		<xsl:variable name="SpaceToNextTab">

			<xsl:variable name="MinTabOffset">
				<xsl:value-of select="350"/>
			</xsl:variable>

			<xsl:choose>
				<xsl:when test="w:suff/@w:val='nothing'">0</xsl:when>
				<xsl:when test="w:suff/@w:val='space'">350</xsl:when>
				<xsl:otherwise>

					<xsl:variable name="MinRelevantCustomTab">
						<xsl:choose>
							<xsl:when test="$Hanging != '0'">
								<!--hanging -> get min custom tab that is between start of first line and start of paragrah text-->
								<xsl:call-template name="GetMinVal">
									<xsl:with-param name="values" select="$tabs/w:tab[@w:pos &gt; ( $Left - $Hanging + $MinTabOffset) and @w:pos &lt; $Left ]/@w:pos" />
								</xsl:call-template>
							</xsl:when>
							<xsl:otherwise>

								<!--no hanging -> get min custom tab that is bigger than the start of the first line-->
								<xsl:call-template name="GetMinVal">
									<xsl:with-param name="values" select="$tabs/w:tab[@w:pos &gt; ( $Left + $FirstLine + $MinTabOffset)]/@w:pos" />
								</xsl:call-template>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:variable>

					<xsl:choose>
						<xsl:when test="$Hanging != '0'">

							<!--hanging-->

							<!--<xsl:value-of select="$Hanging" />-->

							<xsl:choose>
								<xsl:when test="$MinRelevantCustomTab != 'NaN'">
									<!--take min relevant custom tab -->
									<xsl:value-of select="$Hanging - ($Left - $MinRelevantCustomTab)" />
								</xsl:when>
								<xsl:otherwise>
									<!--take hanging value-->
									<xsl:value-of select="$Hanging" />
								</xsl:otherwise>
							</xsl:choose>
						</xsl:when>

						<xsl:otherwise>
							<!--no hanging-->
							<xsl:variable name="DefaultTab">
								<xsl:value-of select="key('Part', 'word/settings.xml')/w:settings/w:defaultTabStop/@w:val"/>
							</xsl:variable>
							<xsl:variable name="NextDefaultTabPos">
								<xsl:value-of select="(floor(($Left + $FirstLine + $MinTabOffset) div $DefaultTab) + 1) * $DefaultTab"/>
							</xsl:variable>

							<xsl:choose>
								<xsl:when test="$MinRelevantCustomTab != 'NaN'">
									<!--take min relevant custom tab-->
									<xsl:value-of select="$MinRelevantCustomTab - ($Left + $FirstLine)"/>
								</xsl:when>
								<xsl:otherwise>
									<!--take next default tab-->
									<xsl:value-of select="$NextDefaultTabPos - ($Left + $FirstLine)"/>
								</xsl:otherwise>
							</xsl:choose>

						</xsl:otherwise>
					</xsl:choose>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<!--text:space-before-->
		<xsl:variable name="marginLeft">
			<xsl:call-template name="ConvertTwips">
				<xsl:with-param name="length" select="$Left - $SpaceToNextTab"/>
				<xsl:with-param name="unit">cm</xsl:with-param>
			</xsl:call-template>
		</xsl:variable>
		<xsl:if test="$marginLeft != '0cm'">
			<xsl:text>margin-left:</xsl:text>
			<xsl:choose>
				<xsl:when test="contains($marginLeft, '-')">
					<xsl:text>0cm</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$marginLeft"/>
				</xsl:otherwise>
			</xsl:choose>
			<xsl:text>; </xsl:text>
		</xsl:if>

		<!--text:min-label-width-->
		<!--<xsl:attribute name="text:min-label-width">
            <xsl:call-template name="ConvertTwips">
                <xsl:with-param name="length" select="$SpaceToNextTab"/>
                <xsl:with-param name="unit">cm</xsl:with-param>
            </xsl:call-template>
        </xsl:attribute>-->

		<!--math, dialogika: changed for correct indentation calculation END -->

		<!-- Picture Bullet Size -->
		<!--AVSMedia: maybe should be uncommented later-->
		<!--
        <xsl:if test="w:lvlPicBulletId/@w:val != ''">
            <xsl:attribute name="fo:width">
                <xsl:call-template name="ConvertPoints">
                    <xsl:with-param name="length">
                        <xsl:choose>
                            <xsl:when test="w:rPr/w:sz/@w:val">
                                <xsl:value-of select="w:rPr/w:sz/@w:val div 2"/>
                            </xsl:when>
                            <xsl:otherwise>12</xsl:otherwise>
                        </xsl:choose>
                    </xsl:with-param>
                    <xsl:with-param name="unit">cm</xsl:with-param>
                </xsl:call-template>
            </xsl:attribute>
            <xsl:attribute name="fo:height">
                <xsl:call-template name="ConvertPoints">
                    <xsl:with-param name="length">
                        <xsl:choose>
                            <xsl:when test="w:rPr/w:sz/@w:val">
                                <xsl:value-of select="w:rPr/w:sz/@w:val div 2"/>
                            </xsl:when>
                            <xsl:otherwise>12</xsl:otherwise>
                        </xsl:choose>
                    </xsl:with-param>
                    <xsl:with-param name="unit">cm</xsl:with-param>
                </xsl:call-template>
            </xsl:attribute>
        </xsl:if>
        -->

		<!-- Bullet Align -->
		<!--
        <xsl:text>text-align:</xsl:text>
            <xsl:choose>
                <xsl:when test="w:lvlJc/@w:val='center'">
                    <xsl:text>center</xsl:text>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:text>left</xsl:text>
                </xsl:otherwise>
            </xsl:choose>
        <xsl:text>; </xsl:text>
        -->
	</xsl:template>

	<!--math, dialogika: Added template for getting min value of a node-set BEGIN -->
	<xsl:template name="GetMinVal">
		<xsl:param name="values"/>
		<xsl:choose>
			<xsl:when test="$values">
				<xsl:for-each select="$values">
					<xsl:sort data-type="number" order="ascending"/>
					<xsl:if test="position()=1">
						<xsl:value-of select="."/>
					</xsl:if>
				</xsl:for-each>
			</xsl:when>
			<xsl:otherwise>NaN</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<!--math, dialogika: Added template for getting min value of a node-set END -->

	<!-- bullet types -->
	<xsl:template name="TextChar">
		<xsl:param name="isNumPrefix"/>
	</xsl:template>

	<!-- checks for list numPr properties (numid or level) in styles hierarchy -->
	<xsl:template name="GetListStyleProperty">
		<xsl:param name="style"/>
		<xsl:param name="property"/>

		<xsl:choose>
			<xsl:when test="$style/w:pPr/w:numPr and $property = 'w:ilvl' ">
				<xsl:choose>
					<xsl:when test="$style/w:pPr/w:numPr/w:ilvl">
						<xsl:value-of select="$style/w:pPr/w:numPr/w:ilvl/@w:val"/>
					</xsl:when>
					<xsl:otherwise>0</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:when test="$style/w:pPr/w:numPr/*[name() = $property]/@w:val">
				<xsl:value-of select="$style/w:pPr/w:numPr/*[name() = $property]/@w:val"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:if test="$style/w:basedOn">
					<xsl:variable name="parentStyle" select="$style/w:basedOn/@w:val"/>
					<xsl:call-template name="GetListStyleProperty">
						<xsl:with-param name="style" select="key('StyleId', $parentStyle)"/>
						<xsl:with-param name="property" select="$property"/>
					</xsl:call-template>
				</xsl:if>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!-- checks for list numPr properties (numid or level) for given element  -->
	<xsl:template name="GetListProperty">
		<xsl:param name="node"/>
		<xsl:param name="property"/>

		<!-- second condition in xsl:when checks if there isn't another w:p node before decendant -->
		<xsl:choose>
			<xsl:when
			  test="$node/descendant::w:numPr[not(ancestor::w:pPrChange)] and $node/descendant::w:numPr/child::node()[name() = $property] and
						generate-id($node)=generate-id($node/descendant::w:numPr[not(ancestor::w:pPrChange)]/ancestor::w:p[1])">
				<xsl:value-of select="$node/descendant::w:numPr/child::node()[name() = $property]/@w:val"/>
			</xsl:when>

			<xsl:when
			  test="$node/descendant::w:pStyle[not(ancestor::w:pPrChange)] and 
						generate-id($node)=generate-id($node/descendant::w:pStyle[not(ancestor::w:pPrChange)]/ancestor::w:p[1])">
				<xsl:variable name="styleId" select="$node/descendant::w:pStyle/@w:val"/>

				<xsl:variable name="pStyle" select="key('StyleId', $styleId)"/>
				<xsl:variable name="propertyValue">
					<xsl:call-template name="GetListStyleProperty">
						<xsl:with-param name="style" select="$pStyle"/>
						<xsl:with-param name="property" select="$property"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:value-of select="$propertyValue"/>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<!-- heading list display levels  -->
	<xsl:template name="CountDisplayListLevels">
		<xsl:param name="string"/>
		<xsl:param name="count"/>
		<xsl:choose>
			<xsl:when test="string-length(substring-after($string,'%')) &gt; 0">
				<xsl:call-template name="CountDisplayListLevels">
					<xsl:with-param name="string" select="substring-after($string,'%')"/>
					<xsl:with-param name="count" select="$count +1"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$count"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


  <xsl:template name="FindPrecendingParagraphWithNum">
    <xsl:param name="CurNode" select="."/>

    <xsl:variable name="PrecedingNode" select="$CurNode/preceding-sibling::w:p[1]"/>
    <xsl:variable name="Count" select="count($CurNode/preceding-sibling::w:p)"/>

    <xsl:variable name="ilvl">
      <xsl:call-template name="GetListProperty">
        <xsl:with-param name="node" select="$PrecedingNode"/>
        <xsl:with-param name="property">w:ilvl</xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="$ilvl != ''">
        <xsl:value-of select="$Count"/>
      </xsl:when>
      <xsl:when test="$PrecedingNode">
        <xsl:call-template name="FindPrecendingParagraphWithNum">
          <xsl:with-param name="CurNode" select="$PrecedingNode" />
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="-1"/>
      </xsl:otherwise>
    </xsl:choose>
    
  </xsl:template>
  
	<!--AVSMedia: this template counts the value (index number) of a list item-->
	<xsl:template name="CountListItemValue">

		<xsl:param name="node" select="."/>

		<xsl:param name="numId">
			<xsl:call-template name="GetListProperty">
				<xsl:with-param name="node" select="$node"/>
				<xsl:with-param name="property">w:numId</xsl:with-param>
			</xsl:call-template>
		</xsl:param>

		<xsl:param name="ilvl">
			<xsl:call-template name="GetListProperty">
				<xsl:with-param name="node" select="$node"/>
				<xsl:with-param name="property">w:ilvl</xsl:with-param>
			</xsl:call-template>
		</xsl:param>

    <xsl:variable name="abstractNumId">
      <xsl:choose>
        <xsl:when
				  test="key('numId', $numId)/w:abstractNumId/@w:val">
          <xsl:value-of select="key('numId', $numId)/w:abstractNumId/@w:val" />
        </xsl:when>
      </xsl:choose>
    </xsl:variable>

    <!--<xsl:value-of select="count($node/preceding-sibling::w:p[w:pPr/w:numPr/w:ilvl/@w:val = $ilvl and w:pPr/w:numPr/w:numId/@w:val = $numId]) + 1"/>		-->

    <xsl:variable name="precedingNodeNum">
      <xsl:call-template name="FindPrecendingParagraphWithNum">
        <xsl:with-param name="CurNode" select="$node"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="precedingNode" select="$node/preceding-sibling::w:p[position() = count($node/preceding-sibling::w:p) - $precedingNodeNum + 1]"/>

    <!--<xsl:variable name="precedingNode" select="$node/preceding-sibling::w:p[w:pPr/w:numPr/w:ilvl/@w:val != ''][1]"/>-->

		<xsl:variable name="precedingNumId">
			<xsl:call-template name="GetListProperty">
				<xsl:with-param name="node" select="$precedingNode"/>
				<xsl:with-param name="property">w:numId</xsl:with-param>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="precedingIlvl">
			<xsl:call-template name="GetListProperty">
				<xsl:with-param name="node" select="$precedingNode"/>
				<xsl:with-param name="property">w:ilvl</xsl:with-param>
			</xsl:call-template>
		</xsl:variable>

    <xsl:variable name="precedingAbstractNumId">
      <xsl:choose>
        <xsl:when
				  test="key('numId', $precedingNumId)/w:abstractNumId/@w:val">
          <xsl:value-of select="key('numId', $precedingNumId)/w:abstractNumId/@w:val" />
        </xsl:when>
      </xsl:choose>
    </xsl:variable>


    <xsl:choose>
			<!--<xsl:when test="($precedingNumId = $numId) and ($precedingIlvl = $ilvl)">-->
      <xsl:when test="($precedingAbstractNumId = $abstractNumId) and ($precedingIlvl = $ilvl)">
          <xsl:variable name="result">
					<xsl:call-template name="CountListItemValue">
						<xsl:with-param name="node" select="$precedingNode"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:value-of select="$result + 1"/>
			</xsl:when>
      <!--<xsl:when test="($precedingNumId = $numId) and ($precedingIlvl &gt; $ilvl)">-->
      <xsl:when test="($precedingAbstractNumId = $abstractNumId) and ($precedingIlvl &gt; $ilvl)">
        <xsl:variable name="result">
          <xsl:call-template name="CountListItemValue">
            <xsl:with-param name="node" select="$precedingNode"/>
            <xsl:with-param name="ilvl" select="$ilvl"/>
            <xsl:with-param name="numId" select="$numId"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:value-of select="$result"/>
      </xsl:when>
      <!--<xsl:when test="($precedingNumId = $numId) and ($precedingIlvl &lt; $ilvl)">-->
      <xsl:when test="($precedingAbstractNumId = $abstractNumId) and ($precedingIlvl &lt; $ilvl)">
        <!-- TO DO: Прочитать нужно ли делать рестарт на данном уровне -->
        <xsl:variable name="numbering" select="key('Part', 'word/numbering.xml')/w:numbering"/>
        <xsl:variable name="absNumId" select="$numbering/w:num[@w:numId = $numId]/w:abstractNumId/@w:val"/>
        <xsl:variable name="absNum" select="$numbering/w:abstractNum[@w:abstractNumId = $absNumId]"/>
        <xsl:variable name="lvl" select="$absNum/w:lvl[@w:ilvl = $ilvl]"/>
        <xsl:choose>
          <xsl:when test="$lvl/w:start/@w:val != ''">
            <xsl:value-of select="$lvl/w:start/@w:val"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="number(1)"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:when test="$precedingNode">
				<xsl:call-template name="CountListItemValue">
					<xsl:with-param name="node" select="$precedingNode"/>
					<xsl:with-param name="ilvl" select="$ilvl"/>
					<xsl:with-param name="numId" select="$numId"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:variable name="numbering" select="key('Part', 'word/numbering.xml')/w:numbering"/>
				<xsl:variable name="absNumId" select="$numbering/w:num[@w:numId = $numId]/w:abstractNumId/@w:val"/>
				<xsl:variable name="absNum" select="$numbering/w:abstractNum[@w:abstractNumId = $absNumId]"/>
				<xsl:variable name="lvl" select="$absNum/w:lvl[@w:ilvl = $ilvl]"/>
				<xsl:choose>
					<xsl:when test="$lvl/w:start/@w:val != ''">
						<xsl:value-of select="$lvl/w:start/@w:val"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="number(1)"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:otherwise>
		</xsl:choose>

	</xsl:template>

	<!--This template converts (multi-level) list from the flat structure in OpenXML to a hierarchical/nested list structure in ODT.
		
		The list conversion is triggered on the first paragraph in the list, then this template and InsertListLevel are called recursively to 
		create the nested structure of the entire list. This means that this template must check whether it is called on the first paragraph 
		of a list or on a following one. In the latter case this template will exit immediately. -->
	<xsl:template match="w:p" mode="list">
		<xsl:param name="numId"/>
		<xsl:param name="ilvl"/>
		<xsl:param name="listLevel" select="number(-1)"/>

		<xsl:choose>
			<xsl:when test="$List2List = 1">
				<xsl:call-template name="List2List">
					<xsl:with-param name="numId" select="$numId"/>
					<xsl:with-param name="ilvl"	select="$ilvl"/>
					<xsl:with-param name="listLevel" select="$listLevel"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:call-template name="List2Text">
					<xsl:with-param name="numId" select="$numId"/>
					<xsl:with-param name="ilvl"	select="$ilvl"/>
				</xsl:call-template>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!--Converted list to text-->
	<xsl:template name="List2Text">
		<xsl:param name="numId"/>
		<xsl:param name="ilvl"/>

		<xsl:variable name="numbering" select="key('Part', 'word/numbering.xml')/w:numbering"/>
		<xsl:variable name="absNumId" select="$numbering/w:num[@w:numId = $numId]/w:abstractNumId/@w:val"/>
		<xsl:variable name="absNum" select="$numbering/w:abstractNum[@w:abstractNumId = $absNumId]"/>
		<xsl:variable name="lvl" select="$absNum/w:lvl[@w:ilvl = $ilvl]"/>

		<xsl:variable name="NumFmt" select="$lvl/w:numFmt/@w:val"/>

    <xsl:variable name="prevPar" select="count( preceding::w:p)"/>
    <xsl:variable name="prevTable" select="count( preceding::w:tbl)"/>

    <!-- Специальная ветка для CKEditor. -->
    <xsl:if test="$HtmlMode = 'CKEditor'">

      <!-- Если в свойствах параграфа есть w:pageBreakBefore, тогда вставим здесь разрыв страницы -->
      <xsl:for-each select="w:pPr">
        <xsl:if test="(w:pageBreakBefore and ( not(w:pageBreakBefore/@w:val) or w:pageBreakBefore/@w:val = 'true' or w:pageBreakBefore/@w:val = 'on' or w:pageBreakBefore/@w:val = 1 )) and ($prevPar != 0 or $prevTable != 0)">

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


    <!-- Здесь делается замена 0xF0XY -> 0xXY, для всех кроме 0xF027, потому что 0x27 это апостроф, закрывающий строку, и с ним Xslt код не работает -->
    <xsl:variable name="LvlText" select="translate( $lvl/w:lvlText/@w:val, '&#xf020;&#xf021;&#xf022;&#xf023;&#xf024;&#xf025;&#xf026;&#xf028;&#xf029;&#xf02a;&#xf02b;&#xf02c;&#xf02d;&#xf02e;&#xf02f;&#xf030;&#xf031;&#xf032;&#xf033;&#xf034;&#xf035;&#xf036;&#xf037;&#xf038;&#xf039;&#xf03a;&#xf03b;&#xf03c;&#xf03d;&#xf03e;&#xf03f;&#xf040;&#xf041;&#xf042;&#xf043;&#xf044;&#xf045;&#xf046;&#xf047;&#xf048;&#xf049;&#xf04a;&#xf04b;&#xf04c;&#xf04d;&#xf04e;&#xf04f;&#xf050;&#xf051;&#xf052;&#xf053;&#xf054;&#xf055;&#xf056;&#xf057;&#xf058;&#xf059;&#xf05a;&#xf05b;&#xf05c;&#xf05d;&#xf05e;&#xf05f;&#xf060;&#xf061;&#xf062;&#xf063;&#xf064;&#xf065;&#xf066;&#xf067;&#xf068;&#xf069;&#xf06a;&#xf06b;&#xf06c;&#xf06d;&#xf06e;&#xf06f;&#xf070;&#xf071;&#xf072;&#xf073;&#xf074;&#xf075;&#xf076;&#xf077;&#xf078;&#xf079;&#xf07a;&#xf07b;&#xf07c;&#xf07d;&#xf07e;&#xf07f;&#xf080;&#xf081;&#xf082;&#xf083;&#xf084;&#xf085;&#xf086;&#xf087;&#xf088;&#xf089;&#xf08a;&#xf08b;&#xf08c;&#xf08d;&#xf08e;&#xf08f;&#xf090;&#xf091;&#xf092;&#xf093;&#xf094;&#xf095;&#xf096;&#xf097;&#xf098;&#xf099;&#xf09a;&#xf09b;&#xf09c;&#xf09d;&#xf09e;&#xf09f;&#xf0a0;&#xf0a1;&#xf0a2;&#xf0a3;&#xf0a4;&#xf0a5;&#xf0a6;&#xf0a7;&#xf0a8;&#xf0a9;&#xf0aa;&#xf0ab;&#xf0ac;&#xf0ad;&#xf0ae;&#xf0af;&#xf0b0;&#xf0b1;&#xf0b2;&#xf0b3;&#xf0b4;&#xf0b5;&#xf0b6;&#xf0b7;&#xf0b8;&#xf0b9;&#xf0ba;&#xf0bb;&#xf0bc;&#xf0bd;&#xf0be;&#xf0bf;&#xf0c0;&#xf0c1;&#xf0c2;&#xf0c3;&#xf0c4;&#xf0c5;&#xf0c6;&#xf0c7;&#xf0c8;&#xf0c9;&#xf0ca;&#xf0cb;&#xf0cc;&#xf0cd;&#xf0ce;&#xf0cf;&#xf0d0;&#xf0d1;&#xf0d2;&#xf0d3;&#xf0d4;&#xf0d5;&#xf0d6;&#xf0d7;&#xf0d8;&#xf0d9;&#xf0da;&#xf0db;&#xf0dc;&#xf0dd;&#xf0de;&#xf0df;&#xf0e0;&#xf0e1;&#xf0e2;&#xf0e3;&#xf0e4;&#xf0e5;&#xf0e6;&#xf0e7;&#xf0e8;&#xf0e9;&#xf0ea;&#xf0eb;&#xf0ec;&#xf0ed;&#xf0ee;&#xf0ef;&#xf0f0;&#xf0f1;&#xf0f2;&#xf0f3;&#xf0f4;&#xf0f5;&#xf0f6;&#xf0f7;&#xf0f8;&#xf0f9;&#xf0fa;&#xf0fb;&#xf0fc;&#xf0fd;&#xf0fe;&#xf0ff;','&#x20;&#x21;&#x22;&#x23;&#x24;&#x25;&#x26;&#x28;&#x29;&#x2a;&#x2b;&#x2c;&#x2d;&#x2e;&#x2f;&#x30;&#x31;&#x32;&#x33;&#x34;&#x35;&#x36;&#x37;&#x38;&#x39;&#x3a;&#x3b;&#x3c;&#x3d;&#x3e;&#x3f;&#x40;&#x41;&#x42;&#x43;&#x44;&#x45;&#x46;&#x47;&#x48;&#x49;&#x4a;&#x4b;&#x4c;&#x4d;&#x4e;&#x4f;&#x50;&#x51;&#x52;&#x53;&#x54;&#x55;&#x56;&#x57;&#x58;&#x59;&#x5a;&#x5b;&#x5c;&#x5d;&#x5e;&#x5f;&#x60;&#x61;&#x62;&#x63;&#x64;&#x65;&#x66;&#x67;&#x68;&#x69;&#x6a;&#x6b;&#x6c;&#x6d;&#x6e;&#x6f;&#x70;&#x71;&#x72;&#x73;&#x74;&#x75;&#x76;&#x77;&#x78;&#x79;&#x7a;&#x7b;&#x7c;&#x7d;&#x7e;&#x7f;&#x80;&#x81;&#x82;&#x83;&#x84;&#x85;&#x86;&#x87;&#x88;&#x89;&#x8a;&#x8b;&#x8c;&#x8d;&#x8e;&#x8f;&#x90;&#x91;&#x92;&#x93;&#x94;&#x95;&#x96;&#x97;&#x98;&#x99;&#x9a;&#x9b;&#x9c;&#x9d;&#x9e;&#x9f;&#xa0;&#xa1;&#xa2;&#xa3;&#xa4;&#xa5;&#xa6;&#xa7;&#xa8;&#xa9;&#xaa;&#xab;&#xac;&#xad;&#xae;&#xaf;&#xb0;&#xb1;&#xb2;&#xb3;&#xb4;&#xb5;&#xb6;&#xb7;&#xb8;&#xb9;&#xba;&#xbb;&#xbc;&#xbd;&#xbe;&#xbf;&#xc0;&#xc1;&#xc2;&#xc3;&#xc4;&#xc5;&#xc6;&#xc7;&#xc8;&#xc9;&#xca;&#xcb;&#xcc;&#xcd;&#xce;&#xcf;&#xd0;&#xd1;&#xd2;&#xd3;&#xd4;&#xd5;&#xd6;&#xd7;&#xd8;&#xd9;&#xda;&#xdb;&#xdc;&#xdd;&#xde;&#xdf;&#xe0;&#xe1;&#xe2;&#xe3;&#xe4;&#xe5;&#xe6;&#xe7;&#xe8;&#xe9;&#xea;&#xeb;&#xec;&#xed;&#xee;&#xef;&#xf0;&#xf1;&#xf2;&#xf3;&#xf4;&#xf5;&#xf6;&#xf7;&#xf8;&#xf9;&#xfa;&#xfb;&#xfc;&#xfd;&#xfe;&#xff;' )"/>
    
		<p>
			<xsl:if test="w:pPr/w:pStyle/@w:val">
				<xsl:attribute name="class">
					<xsl:call-template name="InsertStyleNameInTeg">
						<xsl:with-param name="styleName" select="w:pPr/w:pStyle/@w:val"/>
					</xsl:call-template>
				</xsl:attribute>
			</xsl:if>

			<xsl:variable name="textPStyle">
				<xsl:for-each select="./w:pPr">
					<xsl:call-template name="InsertParagraphProperties"/>
				</xsl:for-each>
				<xsl:apply-templates select="./w:pPr/." mode="pPrChildren"/>
			</xsl:variable>
			<xsl:variable name="levelPStyle">
				<xsl:for-each select="$lvl/w:pPr">
					<xsl:call-template name="InsertParagraphProperties"/>
				</xsl:for-each>
				<xsl:apply-templates select="$lvl/w:pPr/." mode="pPrChildren"/>
			</xsl:variable>
			<xsl:if test="$textPStyle != '' or $levelPStyle != ''">
				<xsl:attribute name="style">
					<xsl:value-of select="concat($levelPStyle,$textPStyle)"/>
				</xsl:attribute>
			</xsl:if>

      <!--<xsl:choose>-->
        <!-- Специальная заглушка для таблиц в CkEditor -->
        <!--<xsl:when test="count(descendant::w:t | descendant::w:instrText) = 0 and ancestor::w:tc">
          <xsl:text disable-output-escaping="yes">&lt;br&gt;</xsl:text>
        </xsl:when>
        <xsl:otherwise>-->

          <span>

            <xsl:variable name="textSpanStyle">
              <!-- Специальная ветка для CKEditor. -->
              <xsl:if test="$HtmlMode = 'CKEditor'">

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
              </xsl:if>
              
            </xsl:variable>

            <xsl:variable name="textRStyle">
              <xsl:call-template name="InsertTextPropertiesInNumList"/>
            </xsl:variable>
            <xsl:variable name="levelRStyle">
              <xsl:apply-templates select="$lvl/w:rPr/." mode="rPrChildren"/>
            </xsl:variable>

            <xsl:choose>
              <xsl:when test="$textRStyle != '' or $levelRStyle != '' or $textSpanStyle != ''">
                <xsl:attribute name="style">
                  <xsl:value-of select="concat( $textSpanStyle, $textRStyle, $levelRStyle, 'padding-right:20px;')"/>
                </xsl:attribute>
              </xsl:when>
              <xsl:otherwise>
                <xsl:attribute name="style">
                  <xsl:text>padding-right:20px;</xsl:text>
                </xsl:attribute>
              </xsl:otherwise>
            </xsl:choose>

            <xsl:call-template name="SetNumberList">
              <xsl:with-param name="NumFmt" select="$NumFmt"/>
              <xsl:with-param name="node" select="."/>
              <xsl:with-param name="Text"	select="$LvlText"/>
              <xsl:with-param name="NumLevel" select="number($ilvl) + 1"/>
            </xsl:call-template>

          </span>
          
          <!--<span style="visibility:hidden;font-size:12pt;font-family:Arial;">&#160;&#160;&#160;&#160;</span>-->

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

          <xsl:apply-templates select="child::*"/>

          <!-- Специальная ветка для CKEditor. -->
          <xsl:if test="$HtmlMode = 'CKEditor'">
            <xsl:text disable-output-escaping="yes">&lt;/span&gt;</xsl:text>
          </xsl:if>

        <!--</xsl:otherwise>
      </xsl:choose>-->
        
    </p>
  </xsl:template>


  <xsl:template name="InsertTextPropertiesInNumList">
		<xsl:if test="not(./w:pPr/w:framePr[@w:dropCap])">
			<xsl:apply-templates select="./w:pPr/w:rPr/w:*" mode="rPrChildren-dropcap-forbidden"/>
			<xsl:apply-templates select="./w:pPr/w:rPr/w:*" mode="rPrChildren"/>
		</xsl:if>
		<xsl:for-each select="./w:pPr/w:rPr/*">
			<xsl:call-template name="InsertTextStrikeLine"/>
		</xsl:for-each>
	</xsl:template>


	<xsl:template name="SetNumberList">
		<xsl:param name="NumFmt"/>
		<xsl:param name="node"/>
		<xsl:param name="Text"/>
		<xsl:param name="NumLevel"/>
    <xsl:param name="valIlvl" select="./w:pPr/w:numPr/w:ilvl/@w:val"/>
    <xsl:param name="valNumId" select="./w:pPr/w:numPr/w:numId/@w:val"/>
    <xsl:param name="prevSib" select="count(./preceding-sibling::w:p[w:pPr/w:numPr/w:ilvl/@w:val = $valIlvl and w:pPr/w:numPr/w:numId/@w:val = $valNumId])"/>

    <xsl:variable name="Count1">
          <xsl:value-of select="count(preceding::w:p[w:pPr/w:numPr/w:ilvl/@w:val = $valIlvl and w:pPr/w:numPr/w:numId/@w:val = $valNumId])+1"/>
          <!--<xsl:variable name="Count1" select="count(preceding::w:p[w:pPr/w:numPr/w:ilvl/@w:val = $valIlvl and w:pPr/w:numPr/w:numId/@w:val = $valNumId])+1"/>-->
    </xsl:variable>

		<xsl:choose>
			<xsl:when test="contains($Text, '%')">
				<xsl:value-of select="substring-before($Text, '%')"/>

				<xsl:variable name="newText">
					<xsl:value-of select="substring-after($Text, '%')"/>
				</xsl:variable>

				<xsl:variable name="level">
					<xsl:value-of select="number(substring($newText, 1, 1))"/>
				</xsl:variable>

				<xsl:if test="$level != 'NaN' and number($NumLevel) >= number($level)">
					<xsl:variable name="NumInThisLevel">
						<xsl:variable name="s">
							<xsl:call-template name="CountListItemValue">
								<xsl:with-param name="node" select="$node"/>
								<xsl:with-param name="ilvl" select="($level - 1)"/>
							</xsl:call-template>
						</xsl:variable>
						<xsl:choose>
              <xsl:when test="$prevSib = 0">
                <xsl:value-of select="$Count1"/>
              </xsl:when>
							<xsl:when test="$NumLevel = $level">
								<xsl:value-of select="$s"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="$s - 1"/>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:variable>

					<xsl:choose>
						<xsl:when test="$NumFmt = 'decimal'">
							<xsl:number value="$NumInThisLevel" format="1"/>
						</xsl:when>
						<xsl:when test="$NumFmt = 'lowerRoman'">
							<xsl:number value="$NumInThisLevel" format="i"/>
						</xsl:when>
						<xsl:when test="$NumFmt = 'upperRoman'">
							<xsl:number value="$NumInThisLevel" format="I"/>
						</xsl:when>
						<xsl:when test="$NumFmt = 'lowerLetter'">
							<xsl:number value="$NumInThisLevel" format="a"/>
						</xsl:when>
						<xsl:when test="$NumFmt = 'upperLetter'">
							<xsl:number value="$NumInThisLevel" format="A"/>
						</xsl:when>
						<xsl:when test="$NumFmt = 'russianLower'">
							<xsl:number value="$NumInThisLevel" format="а"/>
						</xsl:when>
						<xsl:when test="$NumFmt = 'russianUpper'">
							<xsl:number value="$NumInThisLevel" format="А"/>
						</xsl:when>
						<xsl:when test="$NumFmt = 'decimalZero'">
							<xsl:number value="$NumInThisLevel" format="01"/>
						</xsl:when>
						<xsl:when test="$NumFmt = 'bullet'">
							<xsl:value-of select="$Text"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:number value="$NumInThisLevel" format="1"/>
						</xsl:otherwise>
					</xsl:choose>


					<xsl:call-template name="SetNumberList">
						<xsl:with-param name="NumFmt" select="$NumFmt"/>
						<xsl:with-param name="node" select="$node"/>
						<xsl:with-param name="Text"	select="substring-after($newText, $level)"/>
						<xsl:with-param name="NumLevel" select="$NumLevel"/>
					</xsl:call-template>
				</xsl:if>
			</xsl:when>
			<xsl:otherwise>        
        <xsl:value-of select="$Text"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<!--Converted list to list-->
	<xsl:template name="List2List">
		<xsl:param name="numId"/>
		<xsl:param name="ilvl"/>
		<xsl:param name="listLevel"/>

		<xsl:variable name="isStartOfList">
			<xsl:call-template name="IsStartOfList">
				<xsl:with-param name="node" select="."/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="isStartOfLevel">
			<xsl:call-template name="IsStartOfLevel">
				<xsl:with-param name="node" select="."/>
			</xsl:call-template>
		</xsl:variable>

		<!--Proceed only for the first paragraph in a list. This template will recursively convert the following paragraphs that 
			still belong to the same list. If later on the main paragraph template (match="w:p" in 2odt-content.xsl) is called on these following
			paragraphs, don't do anything here -->
		<xsl:if test="$isStartOfList='true' or $listLevel &gt;= 0">

			<xsl:variable name="listNumStyle">
				<xsl:choose>
					<!-- if there is w:lvlOverride, numbering properties can be taken from w:num, and list style must be referred to numId -->
					<xsl:when test="key('numId', $numId)/w:lvlOverride">
						<xsl:value-of select="concat('LO',$numId)"/>
					</xsl:when>
					<xsl:when test="key('abstractNumId', key('numId', $numId)/w:abstractNumId/@w:val)/w:numStyleLink">
						<xsl:variable name="linkedStyle" select="key('abstractNumId', key('numId', $numId)/w:abstractNumId/@w:val)/w:numStyleLink/@w:val" />
						<xsl:variable name="linkedNumId" select="key('StyleId', $linkedStyle)/w:pPr/w:numPr/w:numId/@w:val" />

						<!-- math, dialogika: bugfix #1771273 list style is referred to NumId to avoid duplicate list styles BEGIN-->
						<!--<xsl:value-of select="concat('L',key('numId', $linkedNumId)/w:abstractNumId/@w:val)" />-->
						<xsl:value-of select="concat('L',$linkedNumId)" />
					</xsl:when>
					<xsl:otherwise>
						<!--<xsl:value-of select="concat('L',key('numId', $numId)/w:abstractNumId/@w:val)" />-->
						<xsl:value-of select="concat('L',$numId)" />
					</xsl:otherwise>
					<!-- math, dialogika: bugfix #1771273 list style is referred to NumId to avoid duplicate list styles END-->
				</xsl:choose>
			</xsl:variable>

			<xsl:choose>
				<xsl:when test="($isStartOfList='true' and $listLevel = -1) or $isStartOfLevel">
					<!-- the first time we create a list with a style reference -->

					<xsl:choose>
						<xsl:when test="$isStartOfLevel = 'true' and $listLevel &gt;= 0">
							<li style="list-style-type:none;">
								<ul>
									<!--<ul class="{concat($listNumStyle, '_', $ilvl + 1)}">-->

									<!--  TODO - continue numbering-->
									<!--<xsl:attribute name="text:continue-numbering">true</xsl:attribute>-->
									<xsl:choose>
										<xsl:when test="$listLevel + 1 &lt; $ilvl">
											<!-- a list level has been omitted, e.g. the list start with level 2 -->
											<xsl:apply-templates select="." mode="list">
												<xsl:with-param name="numId" select="$numId"/>
												<xsl:with-param name="ilvl" select="$ilvl"/>
												<xsl:with-param name="listLevel" select="$listLevel + 1"/>
											</xsl:apply-templates>
										</xsl:when>
										<xsl:otherwise>
											<xsl:call-template name="InsertListLevel">
												<xsl:with-param name="ilvl" select="$ilvl"/>
												<xsl:with-param name="numId" select="$numId"/>
												<xsl:with-param name="listLevel" select="$listLevel + 1"/>
												<xsl:with-param name="listNumStyle" select="$listNumStyle"/>
											</xsl:call-template>
										</xsl:otherwise>
									</xsl:choose>
								</ul>
							</li>
						</xsl:when>
						<xsl:otherwise>
							<ul>
								<!--<ul class="{concat($listNumStyle, '_', $ilvl + 1)}">-->

								<!--  TODO - continue numbering-->
								<!--<xsl:attribute name="text:continue-numbering">true</xsl:attribute>-->
								<xsl:choose>
									<xsl:when test="$listLevel + 1 &lt; $ilvl">
										<!-- a list level has been omitted, e.g. the list start with level 2 -->
										<xsl:apply-templates select="." mode="list">
											<xsl:with-param name="numId" select="$numId"/>
											<xsl:with-param name="ilvl" select="$ilvl"/>
											<xsl:with-param name="listLevel" select="$listLevel + 1"/>
										</xsl:apply-templates>
									</xsl:when>
									<xsl:otherwise>
										<xsl:call-template name="InsertListLevel">
											<xsl:with-param name="ilvl" select="$ilvl"/>
											<xsl:with-param name="numId" select="$numId"/>
											<xsl:with-param name="listLevel" select="$listLevel + 1"/>
											<xsl:with-param name="listNumStyle" select="$listNumStyle"/>
										</xsl:call-template>
									</xsl:otherwise>
								</xsl:choose>
							</ul>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
				<xsl:when test="$listLevel &lt; $ilvl">
					<!-- creating a nested list within a text:list-item node -->
					<!--<xsl:attribute name="text:continue-numbering">true</xsl:attribute>-->
					<xsl:variable name="listStyle">
						<xsl:call-template name="InsertListStyle"/>
					</xsl:variable>

					<xsl:if test="$listStyle != ''">
						<xsl:attribute name="style">
							<xsl:value-of select="$listStyle"/>
						</xsl:attribute>
					</xsl:if>

					<xsl:choose>
						<xsl:when test="$listLevel + 1 &lt; $ilvl">
							<!-- a list level has been omitted, e.g. the list jumps from level 1 to level 3-->
							<xsl:apply-templates select="." mode="list">
								<xsl:with-param name="numId" select="$numId"/>
								<xsl:with-param name="ilvl" select="$ilvl"/>
								<xsl:with-param name="listLevel" select="$listLevel + 1"/>
							</xsl:apply-templates>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="InsertListLevel">
								<xsl:with-param name="ilvl" select="$ilvl"/>
								<xsl:with-param name="numId" select="$numId"/>
								<xsl:with-param name="listLevel" select="$listLevel + 1"/>
								<xsl:with-param name="listNumStyle" select="$listNumStyle"/>
							</xsl:call-template>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
			</xsl:choose>

			<xsl:variable name="followingBlockEltId">
				<xsl:call-template name="GetNextItemOnIlvl">
					<xsl:with-param name="node" select="."/>
					<xsl:with-param name="ilvl" select="$listLevel"/>
					<xsl:with-param name="numId" select="$numId"/>
				</xsl:call-template>
			</xsl:variable>

			<xsl:if test="$followingBlockEltId != ''">
				<xsl:variable name="followingBlockEltOnLevel" select="following-sibling::node()[generate-id() = $followingBlockEltId]"/>

				<xsl:call-template name="InsertListLevel">
					<xsl:with-param name="node" select="$followingBlockEltOnLevel"/>
					<xsl:with-param name="ilvl" select="$listLevel"/>
					<xsl:with-param name="numId" select="$numId"/>
					<xsl:with-param name="listLevel" select="$listLevel"/>
					<xsl:with-param name="listNumStyle" select="$listNumStyle"/>
				</xsl:call-template>
			</xsl:if>
		</xsl:if>
	</xsl:template>

	<xsl:template name="InsertListStyle">
		<xsl:variable name="numId" select="w:numPr/w:numId/@w:val"/>
		<xsl:variable name="ilvl" select="w:numPr/w:ilvl/@w:val"/>

		<xsl:for-each select="//w:numbering/w:num[@w:numId = $numId]">
			<xsl:variable name="abstractNumId" select="w:abstractNumId"/>
			<xsl:for-each select="//w:numbering/w:abstractNum[@w:abstractNumId = $abstractNumId]">
				<xsl:for-each select="w:lvl[@w:ilvl = $ilvl]">
					<xsl:choose>
						<xsl:when test="w:numFmt/@w:val = 'decimal' ">
							<xsl:text>list-style-type:decimal; </xsl:text>
						</xsl:when>
						<xsl:when test="w:numFmt/@w:val = 'lowerRoman' ">
							<xsl:text>list-style-type:lower-roman; </xsl:text>
						</xsl:when>
						<xsl:when test="w:numFmt/@w:val = 'upperRoman' ">
							<xsl:text>list-style-type:upper-roman; </xsl:text>
						</xsl:when>
						<xsl:when test="w:numFmt/@w:val = 'lowerLetter' ">
							<xsl:text>list-style-type:lower-alpha; </xsl:text>
						</xsl:when>
						<xsl:when test="w:numFmt/@w:val = 'upperLetter' ">
							<xsl:text>list-style-type:upper-alpha; </xsl:text>
						</xsl:when>
						<xsl:when test="w:numFmt/@w:val = 'bullet' ">
							<xsl:text>list-style-type:disc; </xsl:text>
						</xsl:when>
					</xsl:choose>
				</xsl:for-each>
			</xsl:for-each>
		</xsl:for-each>
	</xsl:template>

	<!-- converts element as list item and insert nested level if there is any -->
	<xsl:template name="InsertListLevel">
		<xsl:param name="node" select="."/>
		<xsl:param name="ilvl"/>
		<xsl:param name="numId"/>
		<xsl:param name="listLevel"/>
		<xsl:param name="listNumStyle"/>

		<xsl:choose>
			<!-- if this paragraph is attached to preceding in track changes mode-->
			<xsl:when test="key('p', number(@oox:id)-1)/w:pPr/w:rPr/w:del"/>

			<xsl:otherwise>

				<xsl:variable name="followingBlockElt" select="$node/following-sibling::node()[1]"/>

				<xsl:choose>
					<xsl:when test="$listLevel = $ilvl">
						<!--<xsl:if test="$List2List = 1">												-->
						<xsl:text disable-output-escaping="yes">&lt;li class="</xsl:text>
						<xsl:value-of select="concat($listNumStyle, '_', $ilvl + 1)"/>
						<xsl:text>" value="</xsl:text>
						<xsl:call-template name="CountListItemValue">
							<xsl:with-param name="node" select="$node"/>
						</xsl:call-template>

						<xsl:variable name="style">
							<xsl:call-template name="setMarginInListLevel">
								<xsl:with-param name="node" select="$node"/>
							</xsl:call-template>
						</xsl:variable>

						<xsl:if test="$style != ''">
							<xsl:text>" style="</xsl:text>
							<xsl:value-of select="concat('margin-left:', $style, ';')"/>
						</xsl:if>
						<xsl:text disable-output-escaping="yes">"&gt;</xsl:text>
						<!--</xsl:if>-->

						<!-- insert list item content -->
						<xsl:variable name="outlineLevel">
							<xsl:call-template name="GetOutlineLevel">
								<xsl:with-param name="node" select="$node"/>
							</xsl:call-template>
						</xsl:variable>

						<xsl:choose>
							<xsl:when test="$outlineLevel != ''">
								<xsl:apply-templates select="$node" mode="heading"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:apply-templates select="$node" mode="paragraph"/>
							</xsl:otherwise>
						</xsl:choose>

						<!--<xsl:if test="$List2List = 1">-->
						<xsl:text disable-output-escaping="yes">&lt;/li&gt;</xsl:text>
						<!--</xsl:if>-->
					</xsl:when>
				</xsl:choose>

				<xsl:variable name="followingNumId">
					<xsl:call-template name="GetListProperty">
						<xsl:with-param name="node" select="$followingBlockElt"/>
						<xsl:with-param name="property">w:numId</xsl:with-param>
					</xsl:call-template>
				</xsl:variable>

				<xsl:variable name="followingIlvl">
					<xsl:call-template name="GetListProperty">
						<xsl:with-param name="node" select="$followingBlockElt"/>
						<xsl:with-param name="property">w:ilvl</xsl:with-param>
					</xsl:call-template>
				</xsl:variable>

				<xsl:for-each select="$followingBlockElt">
					<xsl:choose>
						<xsl:when test="$followingIlvl &gt; $listLevel and $numId = $followingNumId">
							<xsl:apply-templates select="$followingBlockElt" mode="list">
								<xsl:with-param name="numId" select="$followingNumId"/>
								<xsl:with-param name="ilvl" select="$followingIlvl"/>
								<xsl:with-param name="listLevel" select="$listLevel"/>
							</xsl:apply-templates>
						</xsl:when>
						<xsl:when test="$followingIlvl = $listLevel and $numId = $followingNumId">
							<xsl:call-template name="InsertListLevel">
								<xsl:with-param name="node" select="$followingBlockElt"/>
								<xsl:with-param name="ilvl" select="$followingIlvl"/>
								<xsl:with-param name="numId" select="$followingNumId"/>
								<xsl:with-param name="listLevel" select="$listLevel"/>
								<xsl:with-param name="listNumStyle" select="$listNumStyle"/>
							</xsl:call-template>
						</xsl:when>
					</xsl:choose>
				</xsl:for-each>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<xsl:template name="setMarginInListLevel">
		<xsl:param name="node"/>

		<xsl:variable name="DirectNumId">
			<xsl:value-of select="$node/w:pPr/w:numPr/w:numId/@w:val"/>
		</xsl:variable>

		<xsl:variable name="DirectIlvl">
			<xsl:value-of select="$node/w:pPr/w:numPr/w:ilvl/@w:val"/>
		</xsl:variable>

		<xsl:variable name="NumberingDefinitions" select="key('Part', 'word/numbering.xml')/w:numbering"/>

		<xsl:variable name="DirectAbstractNumId" select="key('numId', $DirectNumId)/w:abstractNumId/@w:val"/>

		<xsl:variable name="DirectListLevelDefinition" select="key('abstractNumId', $DirectAbstractNumId)/w:lvl[@w:ilvl=$DirectIlvl][1]"/>

		<xsl:variable name="StyleId" select="$node/w:pPr/w:pStyle/@w:val"/>

		<xsl:variable name="ParagraphStyleDefinition" select="key('StyleId', $StyleId)[1]"/>

		<xsl:variable name="IndirectNumId">
			<xsl:call-template name="GetIndirektListLevelParameter">
				<xsl:with-param name="StyleId" select="$StyleId"/>
				<xsl:with-param name="Parameter">NumId</xsl:with-param>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="IndirectAbstractNumId">
			<xsl:call-template name="GetIndirektListLevelParameter">
				<xsl:with-param name="StyleId" select="$StyleId"/>
				<xsl:with-param name="Parameter">AbstractNumId</xsl:with-param>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="IndirectIlvl">
			<xsl:call-template name="GetIndirektListLevelParameter">
				<xsl:with-param name="StyleId" select="$StyleId"/>
				<xsl:with-param name="Parameter">Ilvl</xsl:with-param>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="IndirectListLevelDefinition" select="key('abstractNumId', $IndirectAbstractNumId)/w:lvl[@w:ilvl=$IndirectIlvl][1]"/>


		<xsl:variable name="SummuryMarginLeft">
			<xsl:variable name="inParagraph">
				<xsl:choose>
					<xsl:when test="$node/w:pPr/w:ind/@w:left">
						<xsl:value-of select="$node/w:pPr/w:ind/@w:left"/>
					</xsl:when>
					<xsl:otherwise>0</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<xsl:variable name="inListLevel">
				<xsl:choose>
					<xsl:when test="$DirectListLevelDefinition/w:pPr/w:ind/@w:left">
						<xsl:value-of select="$DirectListLevelDefinition/w:pPr/w:ind/@w:left"/>
					</xsl:when>
					<xsl:otherwise>0</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<xsl:value-of select="$inParagraph"/>
		</xsl:variable>

		<xsl:if test="$SummuryMarginLeft != 0">
			<xsl:call-template name="ConvertTwips">
				<xsl:with-param name="length">
					<xsl:value-of select="$SummuryMarginLeft - 576"/>
				</xsl:with-param>
				<xsl:with-param name="unit">cm</xsl:with-param>
			</xsl:call-template>
		</xsl:if>

	</xsl:template>

	<!--  checks whether a paragraph node is the first one in a list -->
	<xsl:template name="IsStartOfList">
		<xsl:param name="node" select="."/>

		<xsl:variable name="numId">
			<xsl:call-template name="GetListProperty">
				<xsl:with-param name="node" select="$node"/>
				<xsl:with-param name="property">w:numId</xsl:with-param>
			</xsl:call-template>
		</xsl:variable>
		<xsl:variable name="precedingNumId">
			<xsl:call-template name="GetListProperty">
				<xsl:with-param name="node" select="$node/preceding-sibling::node()[1]"/>
				<xsl:with-param name="property">w:numId</xsl:with-param>
			</xsl:call-template>
		</xsl:variable>

		<xsl:choose>
			<!-- item is the first one on a specific level if numId or ilvl have changed -->
			<xsl:when test="$numId != $precedingNumId">true</xsl:when>
			<xsl:otherwise>false</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<xsl:template name="IsStartOfLevel">
		<xsl:param name="node" select="."/>

		<xsl:variable name="numId">
			<xsl:call-template name="GetListProperty">
				<xsl:with-param name="node" select="$node"/>
				<xsl:with-param name="property">w:numId</xsl:with-param>
			</xsl:call-template>
		</xsl:variable>
		<xsl:variable name="level">
			<xsl:call-template name="GetListProperty">
				<xsl:with-param name="node" select="$node"/>
				<xsl:with-param name="property">w:ilvl</xsl:with-param>
			</xsl:call-template>
		</xsl:variable>
		<xsl:variable name="precedingNumId">
			<xsl:call-template name="GetListProperty">
				<xsl:with-param name="node" select="$node/preceding-sibling::node()[1]"/>
				<xsl:with-param name="property">w:numId</xsl:with-param>
			</xsl:call-template>
		</xsl:variable>
		<xsl:variable name="precedingLevel">
			<xsl:call-template name="GetListProperty">
				<xsl:with-param name="node" select="$node/preceding-sibling::node()[1]"/>
				<xsl:with-param name="property">w:ilvl</xsl:with-param>
			</xsl:call-template>
		</xsl:variable>

		<xsl:choose>
			<xsl:when test="$numId != $precedingNumId">true</xsl:when>
			<xsl:when test="$level != $precedingLevel">true</xsl:when>
			<xsl:otherwise>false</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!--
		Return the id of the next node with list level ilvl in the same list or an empty nodeset 
		if there is no such node following. 
		
		The id is generated using generate-id() -->
	<xsl:template name="GetNextItemOnIlvl">
		<xsl:param name="node"/>
		<xsl:param name="ilvl"/>
		<xsl:param name="numId"/>

		<xsl:variable name="followingBlockElt" select="$node/following-sibling::node()[1]"/>

		<xsl:variable name="followingNumId">
			<xsl:call-template name="GetListProperty">
				<xsl:with-param name="node" select="$followingBlockElt"/>
				<xsl:with-param name="property">w:numId</xsl:with-param>
			</xsl:call-template>
		</xsl:variable>

		<xsl:if test="$followingNumId = $numId">
			<!-- the list is continued -->
			<xsl:variable name="followingLevel">
				<xsl:call-template name="GetListProperty">
					<xsl:with-param name="node" select="$followingBlockElt"/>
					<xsl:with-param name="property">w:ilvl</xsl:with-param>
				</xsl:call-template>
			</xsl:variable>

			<xsl:choose>
				<xsl:when test="$followingLevel = $ilvl">
					<xsl:value-of select="generate-id($followingBlockElt)"/>
				</xsl:when>

				<xsl:when test="$followingLevel &gt; $ilvl">
					<xsl:call-template name="GetNextItemOnIlvl">
						<xsl:with-param name="node" select="$followingBlockElt"/>
						<xsl:with-param name="ilvl" select="$ilvl"/>
						<xsl:with-param name="numId" select="$numId"/>
					</xsl:call-template>
				</xsl:when>
				<!--we return an empty string otherwise (namely if the list is not continued (i.e. $followingLevel='')
					or if $followingLevel &lt; $ilvl-->
			</xsl:choose>
		</xsl:if>
	</xsl:template>

	<!--+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		Templates for automatic styles and styles
		+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++-->

	<!-- inserts automatic list styles with empty num format for elements which has non-existent w:num attached -->
	<xsl:template name="InsertDefaultListStyle">
		<xsl:param name="numId"/>

		<!-- list style with empty num format must be referred to numId, because there is no abstractNumId -->
		<xsl:variable name="listName" select="concat('LO',$numId)"/>
		<text:list-style style:name="{$listName}">
			<xsl:call-template name="InsertDefaultLevelProperties">
				<xsl:with-param name="levelNum" select="'1'"/>
			</xsl:call-template>
		</text:list-style>
	</xsl:template>

	<!-- inserts level with empty num format for elements which has non-existent w:num attached -->
	<xsl:template name="InsertDefaultLevelProperties">
		<xsl:param name="levelNum"/>
		<text:list-level-style-number text:level="{$levelNum}" style:num-format=""/>
		<xsl:if test="$levelNum &lt; 9">
			<xsl:call-template name="InsertDefaultLevelProperties">
				<xsl:with-param name="levelNum" select="$levelNum+1"/>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<!-- inserts automatic list styles with empty num format for elements which has non-existent w:num attached -->
	<xsl:template match="w:numId" mode="automaticstyles">
		<xsl:variable name="numId" select="@w:val"/>
		<xsl:for-each select="key('Part', 'word/numbering.xml')">
			<xsl:if test="key('numId',@w:val) = ''">
				<xsl:call-template name="InsertDefaultListStyle">
					<xsl:with-param name="numId" select="$numId"/>
				</xsl:call-template>
			</xsl:if>
		</xsl:for-each>
	</xsl:template>

	<!--  heading numbering style -->
	<xsl:template name="InsertOutlineListStyle">
		<xsl:param name="numid"/>
		<text:outline-style>
			<!--create outline level style only for styles which have outlineLvl and numId what means that list level is linked to a style -->
			<xsl:for-each
			  select="key('Part', 'word/styles.xml')/w:styles/w:style[child::w:pPr/w:outlineLvl and child::w:pPr/w:numPr and starts-with(@w:styleId,'Heading')]">
				<xsl:variable name="styleId" select="@w:styleId"/>

				<xsl:variable name="numId">
					<xsl:choose>
						<xsl:when test="w:pPr/w:numPr/w:numId/@w:val">
							<xsl:value-of select="w:pPr/w:numPr/w:numId/@w:val"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="GetListStyleProperty">
								<xsl:with-param name="style" select="."/>
								<xsl:with-param name="property">w:numId</xsl:with-param>
							</xsl:call-template>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>
				<xsl:variable name="levelId" select="w:pPr/w:numPr/w:ilvl/@w:val | w:pPr/w:outlineLvl/@w:val"/>
				<text:outline-level-style>
					<xsl:attribute name="text:level">
						<xsl:value-of select="./w:pPr/w:outlineLvl/@w:val+1"/>
					</xsl:attribute>
					<xsl:for-each select="key('Part', 'word/numbering.xml')">
						<xsl:variable name="abstractNum" select="key('abstractNumId',key('numId',$numId)/w:abstractNumId/@w:val)"/>
						<!--w:lvl shows which level defintion should be taken from abstract num-->
						<xsl:for-each select="$abstractNum/w:lvl[@w:ilvl = $levelId][1]">
							<xsl:if
							  test="not(number(substring(./w:lvlText/@w:val,string-length(./w:lvlText/@w:val)))) and ./w:lvlText/@w:val != 'nothing'">
								<xsl:attribute name="style:num-suffix">
									<xsl:variable name="suffix">
										<xsl:value-of select="substring(./w:lvlText/@w:val,string-length(./w:lvlText/@w:val))"/>
									</xsl:variable>
									<xsl:choose>
										<xsl:when test="w:suff/@w:val='space'">
											<xsl:value-of select="concat($suffix,' ')"/>
										</xsl:when>
										<xsl:otherwise>
											<!--math, dialogika: test to improve appearance of TOC in ODT ???-->
											<!--TODO: add a check whether space is already at the end of the suffix-->
											<!--to avoid that roundtrip DOCX->ODT->DOCX->ODT adds another additional space-->
											<!--<xsl:value-of select="concat($suffix,' ')"/>-->
											<xsl:value-of select="$suffix"/>
										</xsl:otherwise>
									</xsl:choose>
								</xsl:attribute>
							</xsl:if>
							<!--math, dialogika: Bugfix #1948002 Insert style:num-format="" if numFmt = 'none' -->
							<xsl:call-template name="NumFormat">
								<xsl:with-param name="format" select="./w:numFmt/@w:val"/>
								<xsl:with-param name="BeforeAfterNum">
									<xsl:if test="./w:numFmt/@w:val != 'none'">
										<xsl:value-of select="./w:lvlText/@w:val" />
									</xsl:if>
								</xsl:with-param>
							</xsl:call-template>
							<xsl:variable name="display">
								<xsl:call-template name="CountDisplayListLevels">
									<xsl:with-param name="string" select="./w:lvlText/@w:val"/>
									<xsl:with-param name="count">0</xsl:with-param>
								</xsl:call-template>
							</xsl:variable>
							<xsl:if test="$display &gt; 1">
								<xsl:attribute name="text:display-levels">
									<xsl:value-of select="$display"/>
								</xsl:attribute>
							</xsl:if>
							<style:list-level-properties>
								<!--math, dialogika: changed for correct indentation calculation BEGIN -->
								<!--math, dialogika: Bugfix #1948002 InsertListLevelProperties only if numFmt is != none -->
								<xsl:if test="./w:numFmt/@w:val != 'none'">
									<xsl:call-template name="InsertListLevelProperties">
										<xsl:with-param name="numId" select="$numid"/>
									</xsl:call-template>
								</xsl:if>
								<!--math, dialogika: changed for correct indentation calculation END -->
							</style:list-level-properties>
						</xsl:for-each>
					</xsl:for-each>
				</text:outline-level-style>
			</xsl:for-each>
		</text:outline-style>
	</xsl:template>
</xsl:stylesheet>
