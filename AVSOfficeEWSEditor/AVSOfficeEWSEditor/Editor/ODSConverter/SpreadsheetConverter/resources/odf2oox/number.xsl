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
<!--
Modification Log
LogNo. |Date       |ModifiedBy   |BugNo.   |Modification                                                      |
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
RefNo-1 28-Oct-2008 Sandeep S     1796999   Changes done to fix Negative value is shown positive(avoiding duplicate format code).
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
  xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
  xmlns:number="urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0"
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
  exclude-result-prefixes="number style fo">
	
	<xsl:import href="chart.xsl"/>
	
	<xsl:key name="numtextStyle" match="style:style" use="@style:data-style-name"/>
	<xsl:key name="table-cell" match="table:table-cell" use="@table:style-name"/>
	<xsl:key name="number" match="number:number-style" use="@style:name"/>
  <xsl:template match="number:number-style" mode="numFormat">

    <!-- @Description: inserts number format -->
    <!-- @Context: none -->

    <xsl:param name="numId"/>
    <!--(int) number format id -->
    <xsl:param name="format"/>
    <!--(string) format code -->
    <xsl:param name="styleName"/>
    <!--(string) style name -->

    <xsl:choose>

      <!-- separate format or last part of partly format -->
      <xsl:when
        test="not((substring(@style:name,string-length(@style:name)-1) = 'P0') or (substring(@style:name,string-length(@style:name)-1) = 'P1' )or (substring(@style:name,string-length(@style:name)-1) = 'P2'))">
        <numFmt numFmtId="{$numId}">
          <xsl:attribute name="formatCode">
            <xsl:variable name="thisFormat">
              <xsl:call-template name="GetFormatCode"/>
            </xsl:variable>
            <xsl:choose>
              <xsl:when test="@style:name = $styleName">
                <xsl:value-of select="concat($format,$thisFormat)"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$thisFormat"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:attribute>
        </numFmt>
        <xsl:choose>
          <xsl:when test="following-sibling::number:number-style">
            <xsl:apply-templates select="following-sibling::number:number-style[1]" mode="numFormat">
              <xsl:with-param name="numId">
                <xsl:value-of select="$numId + 1"/>
              </xsl:with-param>
              <xsl:with-param name="format">
                <xsl:choose>
                  <xsl:when test="@style:name = $styleName"/>
                  <xsl:otherwise>
                    <xsl:value-of select="$format"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>
              <xsl:with-param name="styleName"/>
            </xsl:apply-templates>
          </xsl:when>
        </xsl:choose>
      </xsl:when>

      <!-- partly format -->
      <xsl:otherwise>
        <xsl:variable name="thisFormat">
          <xsl:call-template name="GetFormatCode"/>
        </xsl:variable>
        <xsl:choose>
          <xsl:when test="following-sibling::number:number-style">
            <xsl:apply-templates select="following-sibling::number:number-style[1]" mode="numFormat">
              <xsl:with-param name="numId">
                <xsl:value-of select="$numId + 1"/>
              </xsl:with-param>
              <xsl:with-param name="format">
                <xsl:choose>
                  <xsl:when test="substring(@style:name,1,string-length(@style:name)-2)=$styleName">
                    <xsl:value-of select="concat($format,$thisFormat,';')"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="concat($thisFormat,';')"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>
              <xsl:with-param name="styleName">
                <xsl:value-of select="substring(@style:name,1,string-length(@style:name)-2)"/>
              </xsl:with-param>
            </xsl:apply-templates>

          </xsl:when>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>
	<!-- 
		Defect: 1959472
		Code By :Vijayeta
		Date    :4th Jul '08
		Desc    :User Defined format ##### BF not handeled 
	-->
	<xsl:template match="number:text-style" mode="numFormat">
		<!-- @Description: inserts number format -->
		<!-- @Context: none -->
		<xsl:param name="numId"/>
		<!--(int) number format id -->
		<!--<xsl:param name="format"/>-->
		<!--(string) format code -->
		<xsl:param name="styleName"/>
		<!--(string) style name -->
		<xsl:variable name ="numTextStyleName">
			<xsl:value-of select ="@style:name"/>
		</xsl:variable>
		<xsl:if test ="key('numtextStyle',$numTextStyleName)">
			<xsl:for-each select ="key('numtextStyle',$numTextStyleName)">
				<xsl:variable name ="mainStyle">
					<xsl:value-of select ="@style:name"/>
				</xsl:variable>
				<xsl:variable name ="Data">
					<xsl:value-of select ="key('table-cell',$mainStyle)/@office:value"/>
				</xsl:variable>
				<xsl:choose>
					<xsl:when test ="$Data=''">

					</xsl:when>
					<xsl:otherwise>
						<xsl:variable name ="styleToBeUsed">
							<xsl:choose>
								<xsl:when test ="$Data &gt; 0">
									<xsl:value-of select ="key('text',$numTextStyleName)/style:map[@style:condition='value()&gt;0']/@style:apply-style-name"/>
								</xsl:when>
								<xsl:when test ="$Data &lt; 0">
									<xsl:value-of select ="key('text',$numTextStyleName)/style:map[@style:condition='value()&lt;0']/@style:apply-style-name"/>
								</xsl:when>
								<xsl:when test ="$Data = 0">
									<xsl:value-of select ="key('text',$numTextStyleName)/style:map[@style:condition='value()=0']/@style:apply-style-name"/>
								</xsl:when>
							</xsl:choose>
						</xsl:variable>
						<xsl:for-each select ="key('number',$styleToBeUsed)">
							<numFmt numFmtId="{$numId}">
								<xsl:attribute name="formatCode">
									<xsl:variable name="thisFormat">
										<xsl:call-template name="GetFormatCodeForText">
											<xsl:with-param name ="styleToBeUsed">
												<xsl:value-of select ="$styleToBeUsed"/>
											</xsl:with-param>
										</xsl:call-template>
									</xsl:variable>
									<xsl:value-of select="$thisFormat"/>
								</xsl:attribute>
							</numFmt>
						</xsl:for-each>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:for-each>
			<xsl:apply-templates select="following-sibling::number:text-style[1]" mode="numFormat">
				<xsl:with-param name="numId">
					<xsl:value-of select="$numId + 1"/>
				</xsl:with-param>
				<xsl:with-param name="styleName">
					<xsl:value-of select="substring(@style:name,1,string-length(@style:name)-2)"/>
				</xsl:with-param>
			</xsl:apply-templates>
		</xsl:if>
		<xsl:if test ="not(key('numtextStyle',$numTextStyleName))">
			<xsl:apply-templates select="following-sibling::number:text-style" mode="numFormat">
				<xsl:with-param name="numId">
					<xsl:value-of select="$numId + 1"/>
				</xsl:with-param>
				<xsl:with-param name="styleName"/>
			</xsl:apply-templates>
		</xsl:if>
	</xsl:template>
	<!--Eo defect 1959472-->
  <xsl:template match="number:percentage-style" mode="numFormat">

    <!-- @Description: inserts percentage formats -->
    <!-- @Context: none -->

    <xsl:param name="numId"/>
    <!--(int) number format id -->
    <xsl:param name="format"/>
    <!--(string) format code -->
    <xsl:param name="styleName"/>
    <!--(string) style name -->

    <xsl:choose>

      <!-- separate format or last part of partly format -->
      <xsl:when
        test="not((substring(@style:name,string-length(@style:name)-1) = 'P0') or (substring(@style:name,string-length(@style:name)-1) = 'P1') or (substring(@style:name,string-length(@style:name)-1) = 'P2'))">
        <numFmt numFmtId="{$numId}">
          <xsl:attribute name="formatCode">
            <xsl:variable name="thisFormat">
              <xsl:call-template name="GetFormatCode"/>

            </xsl:variable>
            <xsl:choose>
              <xsl:when test="@style:name = $styleName">
                <xsl:value-of select="concat($format,$thisFormat,'%')"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="concat($thisFormat,'%')"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:attribute>
        </numFmt>
        <xsl:choose>
          <xsl:when test="following-sibling::number:percentage-style">
            <xsl:apply-templates select="following-sibling::number:percentage-style[1]"
              mode="numFormat">
              <xsl:with-param name="numId">
                <xsl:value-of select="$numId + 1"/>
              </xsl:with-param>
              <xsl:with-param name="format">
                <xsl:choose>
                  <xsl:when test="@style:name = $styleName"/>
                  <xsl:otherwise>
                    <xsl:value-of select="$format"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>
              <xsl:with-param name="styleName"/>
            </xsl:apply-templates>
          </xsl:when>
        </xsl:choose>
      </xsl:when>

      <!-- partly format -->
      <xsl:otherwise>
        <xsl:variable name="thisFormat">
          <xsl:call-template name="GetFormatCode"/>
        </xsl:variable>
        <xsl:choose>
          <xsl:when test="following-sibling::number:percentage-style">
            <xsl:apply-templates select="following-sibling::number:percentage-style[1]"
              mode="numFormat">
              <xsl:with-param name="numId">
                <xsl:value-of select="$numId + 1"/>
              </xsl:with-param>
              <xsl:with-param name="format">
                <xsl:choose>
                  <xsl:when test="@style:name=substring($styleName,1,string-length($styleName)-2)">
                    <xsl:value-of select="concat($format,$thisFormat,'%;')"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="concat($thisFormat,'%;')"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>
              <xsl:with-param name="styleName">
                <xsl:value-of select="substring(@style:name,1,string-length(@style:name)-2)"/>
              </xsl:with-param>
            </xsl:apply-templates>
          </xsl:when>
        </xsl:choose>
      </xsl:otherwise>

    </xsl:choose>

  </xsl:template>

  <xsl:template match="number:currency-style" mode="numFormat">

    <!-- @Description: inserts currency formats -->
    <!--  @Context: none -->

    <xsl:param name="numId"/>
    <!--(int) number format id-->
    <xsl:param name="format"/>
    <!-- (string) format code -->
    <xsl:param name="styleName"/>
    <!-- (string) style name -->

    <xsl:variable name="currencySymbol">
      <xsl:call-template name="ConvertValueSymbol">
        <xsl:with-param name="symbol">
          <xsl:value-of select="number:currency-symbol"/>
        </xsl:with-param>
        <xsl:with-param name="language" select="number:currency-symbol/@number:language"/>
        <xsl:with-param name="country" select="number:currency-symbol/@number:country"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>

      <!-- separate format or last part of partly format -->
      <xsl:when
        test="not((substring(@style:name,string-length(@style:name)-1) = 'P0') or (substring(@style:name,string-length(@style:name)-1) = 'P1') or (substring(@style:name,string-length(@style:name)-1) = 'P2'))">
        <numFmt numFmtId="{$numId}">
          <xsl:attribute name="formatCode">
            <xsl:variable name="thisFormat">

              <xsl:call-template name="GetFormatCode">
                <xsl:with-param name="currencySymbol" select="$currencySymbol"/>
              </xsl:call-template>
            </xsl:variable>
            <xsl:choose>
              <xsl:when test="@style:name = $styleName">
                <!--Start of RefNo-1:avoiding duplicate format code-->
                <xsl:choose>
                  <xsl:when test="substring-before($format,';') = $thisFormat">
                    <xsl:value-of select="$thisFormat"/>                    
                  </xsl:when>
                  <xsl:otherwise>
                <xsl:value-of select="concat($format,$thisFormat)"/>
                  </xsl:otherwise>
                </xsl:choose>
                <!--End of RefNo-1-->
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$thisFormat"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:attribute>
        </numFmt>
        <xsl:choose>
          <xsl:when test="following-sibling::number:currency-style">
            <xsl:apply-templates select="following-sibling::number:currency-style[1]"
              mode="numFormat">
              <xsl:with-param name="numId">
                <xsl:value-of select="$numId + 1"/>
              </xsl:with-param>
              <xsl:with-param name="format">
                <xsl:choose>
                  <xsl:when test="@style:name = $styleName"/>
                  <xsl:otherwise>
                    <xsl:value-of select="$format"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>
              <xsl:with-param name="styleName"/>
            </xsl:apply-templates>
          </xsl:when>
        </xsl:choose>
      </xsl:when>

      <!-- partly format -->
      <xsl:otherwise>
        <xsl:variable name="thisFormat">
          <xsl:call-template name="GetFormatCode">
            <xsl:with-param name="currencySymbol" select="$currencySymbol"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:choose>
          <xsl:when test="following-sibling::number:currency-style">
            <xsl:apply-templates select="following-sibling::number:currency-style[1]"
              mode="numFormat">
              <xsl:with-param name="numId">
                <xsl:value-of select="$numId + 1"/>
              </xsl:with-param>
              <xsl:with-param name="format">
                <xsl:choose>
                  <xsl:when test="@style:name=substring($styleName,1,string-length($styleName)-2)">
                    <!--Start of RefNo-1:avoiding duplicate format code-->
                    <xsl:choose>
                      <xsl:when test="substring-before($format,';') = $thisFormat">
                        <xsl:value-of select="concat($thisFormat,';')"/>
                      </xsl:when>
                      <xsl:otherwise>
                    <xsl:value-of select="concat($format,$thisFormat,';')"/>
                      </xsl:otherwise>
                    </xsl:choose>
                    <!--End of RefNo-1-->
                    <!--<xsl:value-of select="concat($format,$thisFormat,';')"/>-->
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="concat($thisFormat,';')"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>
              <xsl:with-param name="styleName">
                <xsl:value-of select="substring(@style:name,1,string-length(@style:name)-2)"/>
              </xsl:with-param>
            </xsl:apply-templates>
          </xsl:when>
        </xsl:choose>
      </xsl:otherwise>

    </xsl:choose>

  </xsl:template>

  <xsl:template name="GetFormatCode">

    <!-- @Description: gets number code format -->
    <!-- @Context: none -->

    <xsl:param name="currencySymbol"/>
    <!--(string) currency symbol if it's currency format -->

    <xsl:variable name="value">
      <xsl:choose>

        <!-- add leading zeros if min-integer-digits > 0 -->
        <xsl:when
          test="number:number/@number:min-integer-digits and number:number/@number:min-integer-digits &gt; 0">
          <xsl:call-template name="AddLeadingZeros">
            <xsl:with-param name="num">
              <xsl:value-of select="number:number/@number:min-integer-digits"/>
            </xsl:with-param>
            <xsl:with-param name="val"/>
          </xsl:call-template>
        </xsl:when>

        <xsl:when
          test="number:scientific-number/@number:min-integer-digits and number:scientific-number/@number:min-integer-digits &gt; 0">
          <xsl:call-template name="AddLeadingZeros">
            <xsl:with-param name="num">
              <xsl:value-of select="number:scientific-number/@number:min-integer-digits"/>
            </xsl:with-param>
            <xsl:with-param name="val"/>
          </xsl:call-template>
        </xsl:when>

        <xsl:when
          test="number:fraction/@number:min-integer-digits and number:fraction/@number:min-integer-digits &gt; 0">
          <xsl:call-template name="AddLeadingZeros">
            <xsl:with-param name="num">
              <xsl:value-of select="number:fraction/@number:min-integer-digits"/>
            </xsl:with-param>
            <xsl:with-param name="val"/>
          </xsl:call-template>
        </xsl:when>

        <xsl:when test="number:fraction/@number:min-integer-digits = 0">#</xsl:when>

        <!-- plain fraction format -->
        <xsl:when test="number:fraction"/>

        <xsl:otherwise>#</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="endValue">
      <xsl:choose>

        <!-- add decimal places -->
        <xsl:when
          test="number:number/@number:decimal-places &gt; 0 or (number:number and not(number:number/@number:decimal-places) and not(number:fraction) and document('styles.xml')/office:document-styles/office:styles/style:default-style/style:table-cell-properties/@style:decimal-places &gt; 0)">
          <xsl:call-template name="AddDecimalPlaces">
            <xsl:with-param name="value">
              <xsl:choose>

                <!-- add grouping -->
                <xsl:when
                  test="number:number/@number:grouping and number:number/@number:grouping = 'true'">
                  <xsl:call-template name="AddGrouping">
                    <xsl:with-param name="value">
                      <xsl:value-of select="concat($value,'.')"/>
                    </xsl:with-param>
                    <xsl:with-param name="numDigits">
                      <xsl:choose>
                        <xsl:when test="number:number/@number:min-integer-digits">
                          <xsl:value-of select="3 - number:number/@number:min-integer-digits"/>
                        </xsl:when>
                        <xsl:otherwise>2</xsl:otherwise>
                      </xsl:choose>
                    </xsl:with-param>
                  </xsl:call-template>
                </xsl:when>

                <xsl:otherwise>
                  <xsl:value-of select="concat($value,'.')"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:with-param>
            <xsl:with-param name="num">
              <xsl:choose>
                <xsl:when test="number:number/@number:decimal-places &gt; 0">
                  <xsl:value-of select="number:number/@number:decimal-places"/>
                </xsl:when>
                <xsl:when test="not(number:number/@number:decimal-places)">
                  <xsl:value-of
                    select="document('styles.xml')/office:document-styles/office:styles/style:default-style/style:table-cell-properties/@style:decimal-places"
                  />
                </xsl:when>
              </xsl:choose>
            </xsl:with-param>
            <xsl:with-param name="decimalReplacement">
              <xsl:choose>
                <xsl:when test="number:number/@number:decimal-replacement=''">#</xsl:when>
                <xsl:when
                  test="not(number:number/@number:decimal-places) and document('styles.xml')/office:document-styles/office:styles/style:default-style/style:table-cell-properties/@style:decimal-places"
                  >#</xsl:when>
                <xsl:otherwise>0</xsl:otherwise>
              </xsl:choose>
            </xsl:with-param>
          </xsl:call-template>

          <xsl:if
            test="number:number/@number:display-factor and number:number/@number:display-factor != '' ">
            <xsl:call-template name="UseThousandDisplayFactor">
              <xsl:with-param name="displayFactor">
                <xsl:value-of select="number:number/@number:display-factor"/>
              </xsl:with-param>
              <xsl:with-param name="value">
                <xsl:value-of select="''"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:if>

        </xsl:when>

        <!-- add decimal places to scientific format -->
        <xsl:when
          test="number:scientific-number/@number:decimal-places &gt; 0 or (number:scientific-number and not(number:scientific-number/@number:decimal-places) and not(number:fraction) and document('styles.xml')/office:document-styles/office:styles/style:default-style/style:table-cell-properties/@style:decimal-places &gt; 0)">
          <xsl:variable name="scientificFormat">
            <xsl:call-template name="AddDecimalPlaces">
              <xsl:with-param name="value">
                <xsl:choose>

                  <!-- add grouping -->
                  <xsl:when
                    test="number:scientific-number/@number:grouping and number:scientific-number/@number:grouping = 'true'">
                    <xsl:call-template name="AddGrouping">
                      <xsl:with-param name="value">
                        <xsl:value-of select="concat($value,'.')"/>
                      </xsl:with-param>
                      <xsl:with-param name="numDigits">
                        <xsl:choose>
                          <xsl:when test="number:scientific-number/@number:min-integer-digits">
                            <xsl:value-of
                              select="3 - number:scientific-number/@number:min-integer-digits"/>
                          </xsl:when>
                          <xsl:otherwise>2</xsl:otherwise>
                        </xsl:choose>
                      </xsl:with-param>
                    </xsl:call-template>
                  </xsl:when>

                  <xsl:otherwise>
                    <xsl:value-of select="concat($value,'.')"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>
              <xsl:with-param name="num">
                <xsl:choose>
                  <xsl:when test="number:scientific-number/@number:decimal-places &gt; 0">
                    <xsl:value-of select="number:scientific-number/@number:decimal-places"/>
                  </xsl:when>
                  <xsl:when test="not(number:scientific-number/@number:decimal-places)">
                    <xsl:value-of
                      select="document('styles.xml')/office:document-styles/office:styles/style:default-style/style:table-cell-properties/@style:decimal-places"
                    />
                  </xsl:when>
                </xsl:choose>
              </xsl:with-param>
              <xsl:with-param name="decimalReplacement">
                <xsl:choose>
                  <xsl:when test="number:scientific-number/@number:decimal-replacement=''">#</xsl:when>
                  <xsl:otherwise>0</xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>
            </xsl:call-template>
            <xsl:if
              test="number:scientific-number/@number:display-factor and number:scientific-number/@number:display-factor != '' ">
              <xsl:call-template name="UseThousandDisplayFactor">
                <xsl:with-param name="displayFactor">
                  <xsl:value-of select="number:number/@number:display-factor"/>
                </xsl:with-param>
                <xsl:with-param name="value">
                  <xsl:value-of select="''"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:if>
          </xsl:variable>
          <xsl:call-template name="AddMinExponentDigits">
            <xsl:with-param name="number">
              <xsl:value-of select="number:scientific-number/@number:min-exponent-digits"/>
            </xsl:with-param>
            <xsl:with-param name="scientificFormat" select="concat($scientificFormat,'E+')"/>
          </xsl:call-template>
        </xsl:when>

        <!-- add fraction -->
        <xsl:when test="number:fraction">
          <xsl:call-template name="AddFraction">
            <xsl:with-param name="value">
              <xsl:choose>

                <!-- add grouping -->
                <xsl:when
                  test="number:fraction/@number:grouping and number:fraction/@number:grouping = 'true'">
                  <xsl:call-template name="AddGrouping">
                    <xsl:with-param name="value">
                      <xsl:value-of select="concat($value,'.')"/>
                    </xsl:with-param>
                    <xsl:with-param name="numDigits">
                      <xsl:choose>
                        <xsl:when test="number:fraction/@number:min-integer-digits">
                          <xsl:value-of select="3 - number:fraction/@number:min-integer-digits"/>
                        </xsl:when>
                        <xsl:otherwise>2</xsl:otherwise>
                      </xsl:choose>
                    </xsl:with-param>
                  </xsl:call-template>
                </xsl:when>

                <xsl:otherwise>
                  <xsl:value-of select="concat($value,'.')"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:with-param>
            <xsl:with-param name="numerator">
              <xsl:value-of select="number:fraction/@number:min-numerator-digits"/>
            </xsl:with-param>
            <!--<xsl:with-param name="denominator">
              <xsl:value-of select="number:fraction/@number:min-denominator-digits"/>				
            </xsl:with-param>-->
			  <!--Vijayeta,SP2,Numberformat lost in case when denomnator is a value,Testfeatures M3-->
            <xsl:with-param name="denominator">
				  <xsl:choose>
					  <xsl:when test ="number:fraction/@number:min-denominator-digits">
						  <xsl:value-of select="concat('digits','|',number:fraction/@number:min-denominator-digits)"/>
					  </xsl:when>
					  <xsl:when test ="number:fraction/@number:denominator-value">
						  <xsl:value-of select="concat('value','|',number:fraction/@number:denominator-value)"/>
					  </xsl:when>
				  </xsl:choose>
            </xsl:with-param>
			  <!--<xsl:value-of select="number:fraction/@number:min-denominator-digits"/>-->
			  <!--Vijayeta,SP2,Numberformat lost in case when denomnator is a value,Testfeatures M3,End-->

          </xsl:call-template>
        </xsl:when>

        <!-- add scientific number format -->
        <xsl:when test="number:scientific-number">
          <xsl:variable name="scientificFormat">
            <xsl:choose>

              <!-- add grouping -->
              <xsl:when
                test="number:scientific-number/@number:grouping and number:scientific-number/@number:grouping = 'true'">
                <xsl:call-template name="AddGrouping">
                  <xsl:with-param name="value">
                    <xsl:value-of select="$value"/>
                  </xsl:with-param>
                  <xsl:with-param name="numDigits">
                    <xsl:choose>
                      <xsl:when test="number:scientific-number/@number:min-integer-digits">
                        <xsl:value-of
                          select="3 - number:scientific-number/@number:min-integer-digits"/>
                      </xsl:when>
                      <xsl:otherwise>2</xsl:otherwise>
                    </xsl:choose>
                  </xsl:with-param>
                </xsl:call-template>
              </xsl:when>

              <xsl:otherwise>
                <xsl:choose>
                  <xsl:when
                    test="number:scientific-number/@number:display-factor and number:scientific-number/@number:display-factor!=''">
                    <xsl:call-template name="UseThousandDisplayFactor">
                      <xsl:with-param name="displayFactor">
                        <xsl:value-of select="number:scientific-number/@number:display-factor"/>
                      </xsl:with-param>
                      <xsl:with-param name="value">
                        <xsl:value-of select="$value"/>
                      </xsl:with-param>
                    </xsl:call-template>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="$value"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:variable>
          <xsl:call-template name="AddMinExponentDigits">
            <xsl:with-param name="number">
              <xsl:value-of select="number:scientific-number/@number:min-exponent-digits"/>
            </xsl:with-param>
            <xsl:with-param name="scientificFormat" select="concat($scientificFormat,'.E+')"/>
          </xsl:call-template>
        </xsl:when>

        <xsl:otherwise>
          <xsl:choose>

            <!-- add grouping -->
            <xsl:when
              test="number:number/@number:grouping and number:number/@number:grouping = 'true'">
              <xsl:call-template name="AddGrouping">
                <xsl:with-param name="value">
                  <xsl:value-of select="$value"/>
                </xsl:with-param>
                <xsl:with-param name="numDigits">
                  <xsl:choose>
                    <xsl:when test="number:number/@number:min-integer-digits">
                      <xsl:value-of select="3 - number:number/@number:min-integer-digits"/>
                    </xsl:when>
                    <xsl:otherwise>2</xsl:otherwise>
                  </xsl:choose>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:when>

            <xsl:otherwise>
              <xsl:choose>
                <xsl:when
                  test="number:number/@number:display-factor and number:number/@number:display-factor!=''">
                  <xsl:call-template name="UseThousandDisplayFactor">
                    <xsl:with-param name="displayFactor">
                      <xsl:value-of select="number:number/@number:display-factor"/>
                    </xsl:with-param>
                    <xsl:with-param name="value">
                      <xsl:value-of select="$value"/>
                    </xsl:with-param>
                  </xsl:call-template>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="$value"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <!-- add color to negative number formatting when necessary -->
    <xsl:variable name="finalValue">
      <xsl:choose>
        <xsl:when test="style:text-properties/@fo:color='#ff0000'">
          <xsl:choose>
            <xsl:when test="style:text-properties/following-sibling::number:text ='-'">
              <xsl:value-of select="concat('[Red]-',$endValue)"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="concat('[Red]',$endValue)"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:when>
        <xsl:when test="style:text-properties/@fo:color='#00ffff'">
          <xsl:choose>
            <xsl:when test="style:text-properties/following-sibling::number:text ='-'">
              <xsl:value-of select="concat('[Cyan]-',$endValue)"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="concat('[Cyan]',$endValue)"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:when>
        <xsl:when test="style:text-properties/@fo:color='#0000ff'">
          <xsl:choose>
            <xsl:when test="style:text-properties/following-sibling::number:text ='-'">
              <xsl:value-of select="concat('[Blue]-',$endValue)"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="concat('[Blue]',$endValue)"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:when>
        <xsl:when test="style:text-properties/@fo:color='#00ff00'">
          <xsl:choose>
            <xsl:when test="style:text-properties/following-sibling::number:text ='-'">
              <xsl:value-of select="concat('[Green]-',$endValue)"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="concat('[Green]',$endValue)"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:when>
        <xsl:when test="style:text-properties/@fo:color='#ffff00'">
          <xsl:choose>
            <xsl:when test="style:text-properties/following-sibling::number:text ='-'">
              <xsl:value-of select="concat('[Yellow]-',$endValue)"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="concat('[Yellow]',$endValue)"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:when>
        <xsl:when test="style:text-properties/@fo:color='#ff00ff'">
          <xsl:choose>
            <xsl:when test="style:text-properties/following-sibling::number:text ='-'">
              <xsl:value-of select="concat('[Magenta]-',$endValue)"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="concat('[Magenta]',$endValue)"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:when>
        <xsl:when test="style:text-properties/@fo:color='#ffffff'">
          <xsl:choose>
            <xsl:when test="style:text-properties/following-sibling::number:text ='-'">
              <xsl:value-of select="concat('[White]-',$endValue)"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="concat('[White]',$endValue)"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:when>

        <xsl:otherwise>
          <xsl:value-of select="$endValue"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <!-- add currency symbol -->
    <xsl:variable name="valueWithCurrency">
      <xsl:choose>
        <xsl:when
          test="$currencySymbol and $currencySymbol!='' and number:number/following-sibling::number:currency-symbol">

          <!-- add space before currency symbol -->
          <xsl:variable name="currencyFormat">
            <xsl:choose>
              <xsl:when test="number:number/following-sibling::number:text = ' '">
                <xsl:value-of select="concat('\ ',$currencySymbol)"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$currencySymbol"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:variable>

          <xsl:value-of select="concat($finalValue,$currencyFormat)"/>
        </xsl:when>
        <xsl:when
          test="$currencySymbol and $currencySymbol!='' and number:number/preceding-sibling::number:currency-symbol">

          <!-- add space after currency symbol -->
          <xsl:variable name="currencyFormat2">
            <xsl:choose>
              <xsl:when test="number:number/preceding-sibling::number:text = ' '">
                <xsl:value-of select="concat($currencySymbol,'\ ')"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$currencySymbol"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:variable>
          <xsl:value-of select="concat($currencyFormat2,$finalValue)"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$finalValue"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="startText">
      <xsl:choose>
        <xsl:when
          test="number:text[not(preceding-sibling::node())] != ' ' and number:text[not(preceding-sibling::node())] != '.'">
          <xsl:value-of select="number:text[not(preceding-sibling::node())]"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="''"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="endText">
      <xsl:choose>
        <xsl:when
          test="number:text[not(following-sibling::node())] != ' ' and number:text[not(following-sibling::node())] != '-' and number:text[not(preceding-sibling::node())] != '.'">
          <xsl:value-of select="number:text[not(following-sibling::node())]"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="''"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <!-- handle text in number format -->
    <xsl:choose>
      <xsl:when test="$startText != '' and $endText != '' ">
        <xsl:value-of
          select="concat('&quot;',$startText,'&quot;',$valueWithCurrency,'&quot;',$endText,'&quot;')"
        />
      </xsl:when>
      <xsl:when test="$startText != '' ">
        <xsl:value-of select="concat('&quot;',$startText,'&quot;',$valueWithCurrency)"/>
      </xsl:when>
      <xsl:when test="$endText != '' ">
        <xsl:value-of select="concat($valueWithCurrency,'&quot;',$endText,'&quot;')"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$valueWithCurrency"/>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="AddLeadingZeros">

    <!-- @Description: adds leading zeros -->
    <!-- @Context: none -->

    <xsl:param name="num"/>
    <!--(int) number of leading zeros-->
    <xsl:param name="val"/>
    <!--(string) output format value -->
    <xsl:choose>
      <xsl:when test="$num &gt; 0">
        <xsl:call-template name="AddLeadingZeros">
          <xsl:with-param name="num">
            <xsl:value-of select="$num - 1"/>
          </xsl:with-param>
          <xsl:with-param name="val">
            <xsl:value-of select="concat('0',$val)"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$val"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="AddMinExponentDigits">

    <!-- @Description: adds minimum exponent-digits -->
    <!-- @Context: none -->

    <xsl:param name="number"/>
    <!-- (int) number of minimum exponent digits -->
    <xsl:param name="scientificFormat"/>
    <!-- (string) whole scientific format -->
    <xsl:choose>
      <xsl:when test="$number &gt; 0">
        <xsl:call-template name="AddMinExponentDigits">
          <xsl:with-param name="number">
            <xsl:value-of select="$number -1"/>
          </xsl:with-param>
          <xsl:with-param name="scientificFormat">
            <xsl:value-of select="concat($scientificFormat,'0')"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$scientificFormat"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="AddGrouping">

    <!-- @Description: adds grouping -->
    <!-- @Context: none -->

    <xsl:param name="numDigits"/>
    <!--(int) number of digits -->
    <xsl:param name="value"/>
    <!-- (string) output format value -->
    <xsl:choose>
      <xsl:when test="$numDigits &gt; 0">
        <xsl:call-template name="AddGrouping">
          <xsl:with-param name="numDigits">
            <xsl:value-of select="$numDigits -1"/>
          </xsl:with-param>
          <xsl:with-param name="value">
            <xsl:value-of select="concat('#',$value)"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="concat('#,',$value)"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="AddDecimalPlaces">

    <!-- @Description: adds decimal places -->
    <!-- @Context: none -->

    <xsl:param name="value"/>
    <!--(string) format value -->
    <xsl:param name="num"/>
    <!-- (int) decimal places -->
    <xsl:param name="decimalReplacement"/>
    <!--(string) decimal replacement -->
    <xsl:choose>
      <xsl:when test="$num &gt; 0">
        <xsl:call-template name="AddDecimalPlaces">
          <xsl:with-param name="value">
            <xsl:value-of select="concat($value,$decimalReplacement)"/>
          </xsl:with-param>
          <xsl:with-param name="num">
            <xsl:value-of select="$num - 1"/>
          </xsl:with-param>
          <xsl:with-param name="decimalReplacement" select="$decimalReplacement"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$value"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="AddFraction">

    <!-- @Description: adds fraction format -->
    <!-- @Context: none -->

    <xsl:param name="value"/>
    <!-- (string) output fraction format -->
    <xsl:param name="numerator"/>
    <!-- (int) number of digits in numerator -->
    <xsl:param name="denominator"/>
    <!-- (int) number of digits in denominator -->
    <xsl:variable name="fraction">
      <!--<xsl:call-template name="ConvertNumeratorDenominator">
        <xsl:with-param name="numerator" select="$numerator"/>
        <xsl:with-param name="denominator" select="$denominator"/>
        <xsl:with-param name="value">/</xsl:with-param>
      </xsl:call-template>-->
		<!--Vijayeta,SP2,Testfeatures M3-->
		<xsl:choose>
			<xsl:when test ="starts-with($denominator,'digits|')">
      <xsl:call-template name="ConvertNumeratorDenominator">
        <xsl:with-param name="numerator" select="$numerator"/>
					<xsl:with-param name="denominator" select="substring-after($denominator,'digits|')"/>
					<xsl:with-param name="value">/</xsl:with-param>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test ="starts-with($denominator,'value|')">
				<xsl:call-template name="ConvertNumeratorDenominatorWithValue">
					<xsl:with-param name="numerator" select="$numerator"/>
					<xsl:with-param name="denominator" select="substring-after($denominator,'value|')"/>
        <xsl:with-param name="value">/</xsl:with-param>
      </xsl:call-template>
			</xsl:when>
		</xsl:choose>
    </xsl:variable>
    <xsl:value-of select="concat(translate($value,'.',''),'&quot; &quot;',$fraction)"/>
  </xsl:template>

  <xsl:template name="ConvertNumeratorDenominator">

    <!-- @Description: converts number of digits in numerator and denominator into plain fraction format -->
    <!-- @Context: none -->

    <xsl:param name="numerator"/>
    <!-- (int) number of digits in numerator -->
    <xsl:param name="denominator"/>
    <!-- (int) number of digits in denominator -->
    <xsl:param name="value"/>
    <!-- (string) output fraction format -->
    <xsl:choose>
      <xsl:when test="$numerator &gt; 0">
        <xsl:call-template name="ConvertNumeratorDenominator">
          <xsl:with-param name="numerator">
            <xsl:value-of select="$numerator - 1"/>
          </xsl:with-param>
          <xsl:with-param name="denominator" select="$denominator"/>
          <xsl:with-param name="value">
            <xsl:value-of select="concat('?',$value)"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="$denominator &gt; 0">
        <xsl:call-template name="ConvertNumeratorDenominator">
          <xsl:with-param name="numerator" select="$numerator"/>
          <xsl:with-param name="denominator">
            <xsl:value-of select="$denominator - 1"/>
          </xsl:with-param>
          <xsl:with-param name="value">
            <xsl:value-of select="concat($value,'?')"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$value"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="UseThousandDisplayFactor">

    <!-- @Description: inserts display factor -->
    <!-- @Context: none -->

    <xsl:param name="displayFactor"/>
    <!--(int) display factor -->
    <xsl:param name="value"/>
    <!--(string) output format value -->
    <xsl:choose>
      <xsl:when test="string-length($displayFactor) &gt; 1">
        <xsl:call-template name="UseThousandDisplayFactor">
          <xsl:with-param name="displayFactor">
            <xsl:value-of select="number($displayFactor) div 1000"/>
          </xsl:with-param>
          <xsl:with-param name="value">
            <xsl:value-of select="concat($value,',')"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$value"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- template to get number format id -->

  <xsl:template name="GetNumFmtId">
    <xsl:param name="numStyle"/>
    <xsl:param name="numStyleCount"/>
    <xsl:param name="styleNumStyleCount"/>
    <xsl:param name="percentStyleCount"/>
    <xsl:param name="stylePercentStyleCount"/>
    <xsl:param name="currencyStyleCount"/>
    <xsl:param name="styleCurrencyStyleCount"/>
    <xsl:param name="dateStyleCount"/>
    <xsl:param name="styleDateStyleCount"/>
    <xsl:param name="timeStyleCount"/>
    <xsl:choose>
      <xsl:when test="key('number',$numStyle)">
        <xsl:for-each select="key('number',$numStyle)">
          <xsl:value-of select="count(preceding-sibling::number:number-style)+1"/>
        </xsl:for-each>
      </xsl:when>
	<!-- 
		Defect: 1959472
		Code By :Vijayeta
		Date    :4th Jul '08
		Desc    :User Defined format ##### BF not handeled 
	-->
		<xsl:when test="key('text',$numStyle)">
			<xsl:for-each select="key('text',$numStyle)">
				<xsl:value-of select="count(preceding-sibling::number:text-style)+1"/>
			</xsl:for-each>
		</xsl:when>
	<!-- End Of code change for 1959472-->
      <xsl:when
        test="document('styles.xml')/office:document-styles/office:styles/number:number-style[@style:name=$numStyle]">
        <xsl:for-each
          select="document('styles.xml')/office:document-styles/office:styles/number:number-style[@style:name=$numStyle]">
          <xsl:value-of select="count(preceding-sibling::number:number-style)+1+$numStyleCount"/>
        </xsl:for-each>
      </xsl:when>
      <xsl:when test="key('percentage',$numStyle)">
        <xsl:for-each select="key('percentage',$numStyle)">
          <xsl:value-of
            select="count(preceding-sibling::number:percentage-style)+1+$numStyleCount+$styleNumStyleCount"
          />
        </xsl:for-each>
      </xsl:when>
      <xsl:when
        test="document('styles.xml')/office:document-styles/office:styles/number:percentage-style[@style:name=$numStyle]">
        <xsl:for-each
          select="document('styles.xml')/office:document-styles/office:styles/number:percentage-style[@style:name=$numStyle]">
          <xsl:value-of
            select="count(preceding-sibling::number:percentage-style)+1+$numStyleCount+$styleNumStyleCount+$percentStyleCount"
          />
        </xsl:for-each>
      </xsl:when>
      <xsl:when test="key('currency',$numStyle)">
        <xsl:for-each select="key('currency',$numStyle)">
          <xsl:value-of
            select="count(preceding-sibling::number:currency-style)+1+$numStyleCount+$styleNumStyleCount+$percentStyleCount+$stylePercentStyleCount"
          />
        </xsl:for-each>
      </xsl:when>
      <xsl:when
        test="document('styles.xml')/office:document-styles/office:styles/number:currency-style[@style:name=$numStyle]">
        <xsl:for-each
          select="document('styles.xml')/office:document-styles/office:styles/number:currency-style[@style:name=$numStyle]">
          <xsl:value-of
            select="count(preceding-sibling::number:currency-style)+1+$numStyleCount+$styleNumStyleCount+$percentStyleCount+$stylePercentStyleCount+$currencyStyleCount"
          />
        </xsl:for-each>
      </xsl:when>
      <xsl:when test="key('date',$numStyle)">
        <xsl:for-each select="key('date',$numStyle)">
          <xsl:value-of
            select="count(preceding-sibling::number:date-style)+1+$numStyleCount+$styleNumStyleCount+$percentStyleCount+$stylePercentStyleCount+$currencyStyleCount+$styleCurrencyStyleCount"
          />
        </xsl:for-each>
      </xsl:when>
      <xsl:when
        test="document('styles.xml')/office:document-styles/office:styles/number:date-style[@style:name=$numStyle]">
        <xsl:for-each
          select="document('styles.xml')/office:document-styles/office:styles/number:date-style[@style:name=$numStyle]">
          <xsl:value-of
            select="count(preceding-sibling::number:date-style)+1+$numStyleCount+$styleNumStyleCount+$percentStyleCount+$stylePercentStyleCount+$currencyStyleCount+$styleCurrencyStyleCount+$dateStyleCount"
          />
        </xsl:for-each>
      </xsl:when>
      <xsl:when test="key('time',$numStyle)">
        <xsl:for-each select="key('time',$numStyle)">
          <xsl:value-of
            select="count(preceding-sibling::number:time-style)+1+$numStyleCount+$styleNumStyleCount+$percentStyleCount+$stylePercentStyleCount+$currencyStyleCount+$styleCurrencyStyleCount+$dateStyleCount+$styleDateStyleCount"
          />
        </xsl:for-each>
      </xsl:when>
      <xsl:when
        test="document('styles.xml')/office:document-styles/office:styles/number:time-style[@style:name=$numStyle]">
        <xsl:for-each
          select="document('styles.xml')/office:document-styles/office:styles/number:time-style[@style:name=$numStyle]">
          <xsl:value-of
            select="count(preceding-sibling::number:time-style)+1+$numStyleCount+$styleNumStyleCount+$percentStyleCount+$stylePercentStyleCount+$currencyStyleCount+$styleCurrencyStyleCount+$dateStyleCount+$styleDateStyleCount+$timeStyleCount"
          />
        </xsl:for-each>
      </xsl:when>
      <xsl:otherwise>0</xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="ConvertValueSymbol">
    <xsl:param name="symbol"/>
    <xsl:param name="country"/>
    <xsl:param name="language"/>
    <xsl:choose>
      <xsl:when test="$symbol = '$' and $country = 'AU' and $language = 'en' ">[$$-C09]</xsl:when>
      <xsl:when test="$symbol = '$' and $country = 'CA' and $language = 'en' ">[$$-1009]</xsl:when>
      <xsl:when test="$symbol = '$' and $country = 'NZ' and $language = 'en' ">[$$-1409]</xsl:when>
      <xsl:when test="$symbol = '$' ">[$$-409]</xsl:when>
      <xsl:when test="$symbol = 'SFr.' and $language = 'it' and $country = 'CH' ">[$SFr.-810]</xsl:when>
      <xsl:when test="$symbol = 'SFr.' ">[$SFr.-807]</xsl:when>
      <xsl:when test="$symbol = 'SIT' and $language = 'sl' and $country = 'SI' ">[$SIT-424]</xsl:when>
      <xsl:when test="$symbol = 'kr' and $language = 'da' and $country = 'DK' ">[$kr-406]</xsl:when>
      <xsl:when test="$symbol = 'kr' and $language = 'nn' and $country = 'NO' ">[$kr-814]</xsl:when>
      <xsl:when test="$symbol = 'kr' and $language = 'et' and $country = 'EE' ">[$kr-425]</xsl:when>
      <xsl:when test="$symbol = 'kr' and $country = 'NO' ">[$kr-414]</xsl:when>
      <xsl:when test="$symbol = 'kr' ">[$kr-41D]</xsl:when>
      <xsl:when test="$symbol = 'kr.' ">[$kr.-40F]</xsl:when>
      <xsl:when test="$symbol = 'Kč' ">[$Kč-405]</xsl:when>
      <xsl:when test="$symbol = 'Sk' ">[$Sk-41B]</xsl:when>
      <xsl:when test="$symbol = 'Lt' ">[$Lt-427]</xsl:when>
      <xsl:when test="$symbol = 'Ls' ">[$Ls-426]</xsl:when>
      <xsl:when test="$symbol = 'lei' ">[$lei-418]</xsl:when>
      <xsl:when test="$symbol = 'Din' ">[$Din.-81A]</xsl:when>
      <xsl:when test="$symbol = 'KM' ">[$KM-141A]</xsl:when>
      <xsl:when test="$symbol = 'HK$' ">[$HK$-C04]</xsl:when>
      <xsl:when test="$symbol = 'NT$' ">[$NT$-404]</xsl:when>
      <xsl:when test="$symbol = 'Ft' ">[$Ft-40E]</xsl:when>
      <xsl:when test="$symbol = '￥' and $country = 'CN' and $language = 'zh' ">[$￥-804]</xsl:when>
      <xsl:when test="$symbol = '￥' ">[$¥-411]</xsl:when>
      <xsl:when test="$symbol = '￦' ">[$₩-412]</xsl:when>
      <xsl:when test="$symbol = 'USD' ">[$USD]</xsl:when>
      <xsl:when test="$symbol = '£' and $country = 'LU' and $language = 'lb' ">[$£-452]</xsl:when>
      <xsl:when test="$symbol = '£' ">[$£-809]</xsl:when>
      <xsl:when test="$symbol = 'GBP' ">[$GBP]</xsl:when>
      <xsl:when test="$symbol = 'zł' ">&quot;zł&quot;</xsl:when>
      <xsl:when test="$symbol = 'PLN' ">[$PLN]</xsl:when>
      <xsl:when test="$symbol = 'ДИН' ">[$Дин.-C1A]</xsl:when>
      <xsl:when test="$symbol = 'лв' or $symbol = 'лв.' ">[$лв-402]</xsl:when>
      <xsl:when test="$symbol = '€' and $country = 'IE' and $language = 'ga' ">[$€-1809]</xsl:when>
      <xsl:when test="$symbol = '€' and $country = 'PT' and $language = 'pt' ">[$€-816]</xsl:when>
      <xsl:when test="$symbol = '€' and $country = 'NL' and $language = 'nl' ">[$€-413]</xsl:when>
      <xsl:when test="$symbol = '€' and $country = 'BE' and $language = 'nl' ">[$€-813]</xsl:when>
      <xsl:when test="$symbol = '€' and $country = 'FI' and $language = 'sv' ">[$€-81D]</xsl:when>
      <xsl:when test="$symbol = '€' and $country = 'MC' and $language = 'fr' ">[$€-180C]</xsl:when>
      <xsl:when test="$symbol = '€' and $country = 'LU' and $language = 'fr' ">[$€-140C]</xsl:when>
      <xsl:when test="$symbol = '€' and $country = 'BE' and $language = 'fr' ">[$€-80C]</xsl:when>
      <xsl:when test="$symbol = '€' and $country = 'FR' and $language = 'fr' ">[$€-40C]</xsl:when>
      <xsl:when test="$symbol = '€' and $country = 'LU' and $language = 'de' ">[$€-1007]</xsl:when>
      <xsl:when test="$symbol = '€' and $country = 'AT' and $language = 'de' ">[$€-C07]</xsl:when>
      <xsl:when test="$symbol = '€' and $country = 'DE' and $language = 'de' ">[$€-407]</xsl:when>
      <xsl:when test="$symbol = '€' and $country = 'FI' ">[$€-40B]</xsl:when>
      <xsl:when test="$symbol = '€' and $country = 'GR' ">[$€-408]</xsl:when>
      <xsl:when test="$symbol = '€' and $country = 'IT' ">[$€-410]</xsl:when>
      <xsl:when test="$symbol = '€' ">
        <xsl:choose>
          <xsl:when test="following-sibling::number:number">[$€-2]</xsl:when>
          <xsl:otherwise>[$€-1]</xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:when test="$symbol = 'EUR' ">[$EUR]</xsl:when>
    </xsl:choose>
  </xsl:template>
	<!-- 
		Defect: 1959472
		Code By :Vijayeta
		Date    :4th Jul '08
		Desc    :User Defined format ##### BF not handeled , Additional template added
	-->
  <xsl:template name="GetFormatCodeForText">
		<xsl:param name ="styleToBeUsed"/>
		<!-- @Description: gets number code format -->
		<!-- @Context: none -->

		<xsl:param name="currencySymbol"/>
		<!--(string) currency symbol if it's currency format -->

		<xsl:variable name="value">
			<xsl:choose>
				<!-- add leading zeros if min-integer-digits > 0 -->
				<xsl:when
				  test="number:number/@number:min-integer-digits and number:number/@number:min-integer-digits &gt; 0">
					<xsl:call-template name="AddLeadingZeros">
						<xsl:with-param name="num">
							<xsl:value-of select="number:number/@number:min-integer-digits"/>
						</xsl:with-param>
						<xsl:with-param name="val"/>
					</xsl:call-template>
				</xsl:when>

				<xsl:when
				  test="number:scientific-number/@number:min-integer-digits and number:scientific-number/@number:min-integer-digits &gt; 0">
					<xsl:call-template name="AddLeadingZeros">
						<xsl:with-param name="num">
							<xsl:value-of select="number:scientific-number/@number:min-integer-digits"/>
						</xsl:with-param>
						<xsl:with-param name="val"/>
					</xsl:call-template>
				</xsl:when>

				<xsl:when
				  test="number:fraction/@number:min-integer-digits and number:fraction/@number:min-integer-digits &gt; 0">
					<xsl:call-template name="AddLeadingZeros">
						<xsl:with-param name="num">
							<xsl:value-of select="number:fraction/@number:min-integer-digits"/>
						</xsl:with-param>
						<xsl:with-param name="val"/>
					</xsl:call-template>
				</xsl:when>

				<xsl:when test="number:fraction/@number:min-integer-digits = 0">#</xsl:when>

				<!-- plain fraction format -->
				<xsl:when test="number:fraction"/>

				<xsl:otherwise>#</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="endValue">
			<xsl:choose>

				<!-- add decimal places -->
				<xsl:when
				  test="number:number/@number:decimal-places &gt; 0 or (number:number and not(number:number/@number:decimal-places) and not(number:fraction) and document('styles.xml')/office:document-styles/office:styles/style:default-style/style:table-cell-properties/@style:decimal-places &gt; 0)">
					<xsl:call-template name="AddDecimalPlaces">
						<xsl:with-param name="value">
							<xsl:choose>

								<!-- add grouping -->
								<xsl:when
								  test="number:number/@number:grouping and number:number/@number:grouping = 'true'">
									<xsl:call-template name="AddGrouping">
										<xsl:with-param name="value">
											<xsl:value-of select="concat($value,'.')"/>
										</xsl:with-param>
										<xsl:with-param name="numDigits">
											<xsl:choose>
												<xsl:when test="number:number/@number:min-integer-digits">
													<xsl:value-of select="3 - number:number/@number:min-integer-digits"/>
												</xsl:when>
												<xsl:otherwise>2</xsl:otherwise>
											</xsl:choose>
										</xsl:with-param>
									</xsl:call-template>
								</xsl:when>

								<xsl:otherwise>
									<xsl:value-of select="concat($value,'.')"/>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:with-param>
						<xsl:with-param name="num">
							<xsl:choose>
								<xsl:when test="number:number/@number:decimal-places &gt; 0">
									<xsl:value-of select="number:number/@number:decimal-places"/>
								</xsl:when>
								<xsl:when test="not(number:number/@number:decimal-places)">
									<xsl:value-of
									  select="document('styles.xml')/office:document-styles/office:styles/style:default-style/style:table-cell-properties/@style:decimal-places"
                  />
								</xsl:when>
							</xsl:choose>
						</xsl:with-param>
						<xsl:with-param name="decimalReplacement">
							<xsl:choose>
								<xsl:when test="number:number/@number:decimal-replacement=''">#</xsl:when>
								<xsl:when
								  test="not(number:number/@number:decimal-places) and document('styles.xml')/office:document-styles/office:styles/style:default-style/style:table-cell-properties/@style:decimal-places"
                  >#</xsl:when>
								<xsl:otherwise>0</xsl:otherwise>
							</xsl:choose>
						</xsl:with-param>
					</xsl:call-template>

					<xsl:if
					  test="number:number/@number:display-factor and number:number/@number:display-factor != '' ">
						<xsl:call-template name="UseThousandDisplayFactor">
							<xsl:with-param name="displayFactor">
								<xsl:value-of select="number:number/@number:display-factor"/>
							</xsl:with-param>
							<xsl:with-param name="value">
								<xsl:value-of select="''"/>
							</xsl:with-param>
						</xsl:call-template>
					</xsl:if>

				</xsl:when>

				<!-- add decimal places to scientific format -->
				<xsl:when
				  test="number:scientific-number/@number:decimal-places &gt; 0 or (number:scientific-number and not(number:scientific-number/@number:decimal-places) and not(number:fraction) and document('styles.xml')/office:document-styles/office:styles/style:default-style/style:table-cell-properties/@style:decimal-places &gt; 0)">
					<xsl:variable name="scientificFormat">
						<xsl:call-template name="AddDecimalPlaces">
							<xsl:with-param name="value">
								<xsl:choose>

									<!-- add grouping -->
									<xsl:when
									  test="number:scientific-number/@number:grouping and number:scientific-number/@number:grouping = 'true'">
										<xsl:call-template name="AddGrouping">
											<xsl:with-param name="value">
												<xsl:value-of select="concat($value,'.')"/>
											</xsl:with-param>
											<xsl:with-param name="numDigits">
												<xsl:choose>
													<xsl:when test="number:scientific-number/@number:min-integer-digits">
														<xsl:value-of
														  select="3 - number:scientific-number/@number:min-integer-digits"/>
													</xsl:when>
													<xsl:otherwise>2</xsl:otherwise>
												</xsl:choose>
											</xsl:with-param>
										</xsl:call-template>
									</xsl:when>

									<xsl:otherwise>
										<xsl:value-of select="concat($value,'.')"/>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:with-param>
							<xsl:with-param name="num">
								<xsl:choose>
									<xsl:when test="number:scientific-number/@number:decimal-places &gt; 0">
										<xsl:value-of select="number:scientific-number/@number:decimal-places"/>
									</xsl:when>
									<xsl:when test="not(number:scientific-number/@number:decimal-places)">
										<xsl:value-of
										  select="document('styles.xml')/office:document-styles/office:styles/style:default-style/style:table-cell-properties/@style:decimal-places"
                    />
									</xsl:when>
								</xsl:choose>
							</xsl:with-param>
							<xsl:with-param name="decimalReplacement">
								<xsl:choose>
									<xsl:when test="number:scientific-number/@number:decimal-replacement=''">#</xsl:when>
									<xsl:otherwise>0</xsl:otherwise>
								</xsl:choose>
							</xsl:with-param>
						</xsl:call-template>
						<xsl:if
						  test="number:scientific-number/@number:display-factor and number:scientific-number/@number:display-factor != '' ">
							<xsl:call-template name="UseThousandDisplayFactor">
								<xsl:with-param name="displayFactor">
									<xsl:value-of select="number:number/@number:display-factor"/>
								</xsl:with-param>
								<xsl:with-param name="value">
									<xsl:value-of select="''"/>
								</xsl:with-param>
							</xsl:call-template>
						</xsl:if>
					</xsl:variable>
					<xsl:call-template name="AddMinExponentDigits">
						<xsl:with-param name="number">
							<xsl:value-of select="number:scientific-number/@number:min-exponent-digits"/>
						</xsl:with-param>
						<xsl:with-param name="scientificFormat" select="concat($scientificFormat,'E+')"/>
					</xsl:call-template>
				</xsl:when>

				<!-- add fraction -->
				<xsl:when test="number:fraction">
					<xsl:call-template name="AddFraction">
						<xsl:with-param name="value">
							<xsl:choose>

								<!-- add grouping -->
								<xsl:when
								  test="number:fraction/@number:grouping and number:fraction/@number:grouping = 'true'">
									<xsl:call-template name="AddGrouping">
										<xsl:with-param name="value">
											<xsl:value-of select="concat($value,'.')"/>
										</xsl:with-param>
										<xsl:with-param name="numDigits">
											<xsl:choose>
												<xsl:when test="number:fraction/@number:min-integer-digits">
													<xsl:value-of select="3 - number:fraction/@number:min-integer-digits"/>
												</xsl:when>
												<xsl:otherwise>2</xsl:otherwise>
											</xsl:choose>
										</xsl:with-param>
									</xsl:call-template>
								</xsl:when>

								<xsl:otherwise>
									<xsl:value-of select="concat($value,'.')"/>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:with-param>
						<xsl:with-param name="numerator">
							<xsl:value-of select="number:fraction/@number:min-numerator-digits"/>
						</xsl:with-param>
						<!--<xsl:with-param name="denominator">
							<xsl:value-of select="number:fraction/@number:min-denominator-digits"/>
						</xsl:with-param>-->
						<!--Vijayeta,SP2,Numberformat lost in case when denomnator is a value,Testfeatures M3-->
						<xsl:with-param name="denominator">
							<xsl:choose>
								<xsl:when test ="number:fraction/@number:min-denominator-digits">
									<xsl:value-of select="concat('digits','|',number:fraction/@number:min-denominator-digits)"/>
								</xsl:when>
								<xsl:when test ="number:fraction/@number:denominator-value">
									<xsl:value-of select="concat('value','|',number:fraction/@number:denominator-value)"/>
								</xsl:when>
							</xsl:choose>
						</xsl:with-param>
						<!--<xsl:value-of select="number:fraction/@number:min-denominator-digits"/>-->
						<!--Vijayeta,SP2,Numberformat lost in case when denomnator is a value,Testfeatures M3,End-->
					</xsl:call-template>
				</xsl:when>

				<!-- add scientific number format -->
				<xsl:when test="number:scientific-number">
					<xsl:variable name="scientificFormat">
						<xsl:choose>

							<!-- add grouping -->
							<xsl:when
							  test="number:scientific-number/@number:grouping and number:scientific-number/@number:grouping = 'true'">
								<xsl:call-template name="AddGrouping">
									<xsl:with-param name="value">
										<xsl:value-of select="$value"/>
									</xsl:with-param>
									<xsl:with-param name="numDigits">
										<xsl:choose>
											<xsl:when test="number:scientific-number/@number:min-integer-digits">
												<xsl:value-of
												  select="3 - number:scientific-number/@number:min-integer-digits"/>
											</xsl:when>
											<xsl:otherwise>2</xsl:otherwise>
										</xsl:choose>
									</xsl:with-param>
								</xsl:call-template>
							</xsl:when>

							<xsl:otherwise>
								<xsl:choose>
									<xsl:when
									  test="number:scientific-number/@number:display-factor and number:scientific-number/@number:display-factor!=''">
										<xsl:call-template name="UseThousandDisplayFactor">
											<xsl:with-param name="displayFactor">
												<xsl:value-of select="number:scientific-number/@number:display-factor"/>
											</xsl:with-param>
											<xsl:with-param name="value">
												<xsl:value-of select="$value"/>
											</xsl:with-param>
										</xsl:call-template>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select="$value"/>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:variable>
					<xsl:call-template name="AddMinExponentDigits">
						<xsl:with-param name="number">
							<xsl:value-of select="number:scientific-number/@number:min-exponent-digits"/>
						</xsl:with-param>
						<xsl:with-param name="scientificFormat" select="concat($scientificFormat,'.E+')"/>
					</xsl:call-template>
				</xsl:when>

				<xsl:otherwise>
					<xsl:choose>

						<!-- add grouping -->
						<xsl:when
						  test="number:number/@number:grouping and number:number/@number:grouping = 'true'">
							<xsl:call-template name="AddGrouping">
								<xsl:with-param name="value">
									<xsl:value-of select="$value"/>
								</xsl:with-param>
								<xsl:with-param name="numDigits">
									<xsl:choose>
										<xsl:when test="number:number/@number:min-integer-digits">
											<xsl:value-of select="3 - number:number/@number:min-integer-digits"/>
										</xsl:when>
										<xsl:otherwise>2</xsl:otherwise>
									</xsl:choose>
								</xsl:with-param>
							</xsl:call-template>
						</xsl:when>

						<xsl:otherwise>
							<xsl:choose>
								<xsl:when
								  test="number:number/@number:display-factor and number:number/@number:display-factor!=''">
									<xsl:call-template name="UseThousandDisplayFactor">
										<xsl:with-param name="displayFactor">
											<xsl:value-of select="number:number/@number:display-factor"/>
										</xsl:with-param>
										<xsl:with-param name="value">
											<xsl:value-of select="$value"/>
										</xsl:with-param>
									</xsl:call-template>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="$value"/>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<!-- add color to negative number formatting when necessary -->
		<xsl:variable name="finalValue">
			<xsl:choose>
				<xsl:when test="style:text-properties/@fo:color='#ff0000'">
					<xsl:choose>
						<xsl:when test="style:text-properties/following-sibling::number:text ='-'">
							<xsl:value-of select="concat('[Red]-',$endValue)"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="concat('[Red]',$endValue)"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
				<xsl:when test="style:text-properties/@fo:color='#00ffff'">
					<xsl:choose>
						<xsl:when test="style:text-properties/following-sibling::number:text ='-'">
							<xsl:value-of select="concat('[Cyan]-',$endValue)"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="concat('[Cyan]',$endValue)"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
				<xsl:when test="style:text-properties/@fo:color='#0000ff'">
					<xsl:choose>
						<xsl:when test="style:text-properties/following-sibling::number:text ='-'">
							<xsl:value-of select="concat('[Blue]-',$endValue)"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="concat('[Blue]',$endValue)"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
				<xsl:when test="style:text-properties/@fo:color='#00ff00'">
					<xsl:choose>
						<xsl:when test="style:text-properties/following-sibling::number:text ='-'">
							<xsl:value-of select="concat('[Green]-',$endValue)"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="concat('[Green]',$endValue)"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
				<xsl:when test="style:text-properties/@fo:color='#ffff00'">
					<xsl:choose>
						<xsl:when test="style:text-properties/following-sibling::number:text ='-'">
							<xsl:value-of select="concat('[Yellow]-',$endValue)"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="concat('[Yellow]',$endValue)"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
				<xsl:when test="style:text-properties/@fo:color='#ff00ff'">
					<xsl:choose>
						<xsl:when test="style:text-properties/following-sibling::number:text ='-'">
							<xsl:value-of select="concat('[Magenta]-',$endValue)"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="concat('[Magenta]',$endValue)"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
				<xsl:when test="style:text-properties/@fo:color='#ffffff'">
					<xsl:choose>
						<xsl:when test="style:text-properties/following-sibling::number:text ='-'">
							<xsl:value-of select="concat('[White]-',$endValue)"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="concat('[White]',$endValue)"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>

				<xsl:otherwise>
					<xsl:value-of select="$endValue"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<!-- add currency symbol -->
		<xsl:variable name="valueWithCurrency">
			<xsl:choose>
				<xsl:when
				  test="$currencySymbol and $currencySymbol!='' and number:number/following-sibling::number:currency-symbol">

					<!-- add space before currency symbol -->
					<xsl:variable name="currencyFormat">
						<xsl:choose>
							<xsl:when test="number:number/following-sibling::number:text = ' '">
								<xsl:value-of select="concat('\ ',$currencySymbol)"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="$currencySymbol"/>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:variable>

					<xsl:value-of select="concat($finalValue,$currencyFormat)"/>
				</xsl:when>
				<xsl:when
				  test="$currencySymbol and $currencySymbol!='' and number:number/preceding-sibling::number:currency-symbol">

					<!-- add space after currency symbol -->
					<xsl:variable name="currencyFormat2">
						<xsl:choose>
							<xsl:when test="number:number/preceding-sibling::number:text = ' '">
								<xsl:value-of select="concat($currencySymbol,'\ ')"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="$currencySymbol"/>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:variable>
					<xsl:value-of select="concat($currencyFormat2,$finalValue)"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$finalValue"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="startText">
			<xsl:choose>
				<xsl:when
				  test="number:text[not(preceding-sibling::node())] != ' ' and number:text[not(preceding-sibling::node())] != '.'">
					<xsl:value-of select="number:text[not(preceding-sibling::node())]"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="''"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="endText">
			<xsl:for-each select ="key('number',$styleToBeUsed)//number:text">
				<xsl:value-of select ="."/>
			</xsl:for-each>
		</xsl:variable>

		<!-- handle text in number format -->
		<xsl:choose>
			<xsl:when test="$startText != '' and $endText != '' ">
				<xsl:value-of
				  select="concat('&quot;',$startText,'&quot;',$valueWithCurrency,'&quot;',$endText,'&quot;')"
        />
			</xsl:when>
			<xsl:when test="$startText != '' ">
				<xsl:value-of select="concat('&quot;',$startText,'&quot;',$valueWithCurrency)"/>
			</xsl:when>
			<xsl:when test="$endText != '' ">
				<xsl:value-of select="concat($valueWithCurrency,'&quot;',$endText,'&quot;')"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$valueWithCurrency"/>
			</xsl:otherwise>
		</xsl:choose>

	</xsl:template>
</xsl:stylesheet>
