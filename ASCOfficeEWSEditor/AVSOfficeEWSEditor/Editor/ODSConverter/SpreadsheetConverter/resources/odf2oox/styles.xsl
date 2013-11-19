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
RefNo-1 20-Dec-2007 Sandeep S     1805556   Changes done to consider all the hyperlink (hyperlink within rowgroup) in the sheet.
RefNo-2 08-Feb-2008 Sandeep S     1738259  Changes done to Bug:Hyperlink text color is not retained after conversion
RefNo-3 14-Oct-2008 Sandeep s     2149116  Changes done to retain Time&Date format in case of formula.
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
  xmlns:xlink="http://www.w3.org/1999/xlink"
  xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main"
  xmlns:pzip="urn:cleverage:xmlns:post-processings:zip"
  xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:number="urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0"
  xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
  xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
  exclude-result-prefixes="svg table r text style number fo draw">

  <!--<xsl:import href="measures.xsl"/>-->
	<xsl:key name="text" match="number:text-style" use="@style:name"/>
  <xsl:key name="number" match="number:number-style" use="@style:name"/>
  <xsl:key name="percentage" match="number:percentage-style" use="@style:name"/>
  <xsl:key name="currency" match="number:currency-style" use="@style:name"/>
  <xsl:key name="date" match="number:date-style" use="@style:name"/>
  <xsl:key name="time" match="number:time-style" use="@style:name"/>
  <xsl:key name="font" match="style:font-face" use="@style:name"/>
  <xsl:key name="hyperlink" match="text:a" use="@xlink:href"/>

  <xsl:template name="styles">
    <styleSheet>
		<xsl:for-each select ="document('content.xml')">
      <xsl:call-template name="InsertNumFormats"/>
      <xsl:call-template name="InsertFonts"/>
      <xsl:call-template name="InsertFills"/>
      <xsl:call-template name="InsertBorders"/>
      <xsl:call-template name="InsertCellFormats"/>
      <xsl:call-template name="InsertCellStyles"/>
      <xsl:call-template name="InsertConditionalFormat"/>
      <xsl:call-template name="InsertTableStyles"/>
		</xsl:for-each>
    </styleSheet>
  </xsl:template>

  <!-- insert all number formats -->

  <xsl:template name="InsertNumFormats">

    <!-- message about  not supported quarter and week date format-->
	  <xsl:for-each select ="office:document-content/office:automatic-styles">
		  <xsl:choose>
			  <xsl:when test="number:date-style/number:quarter">
				  <xsl:message terminate="no">translation.odf2oox.QuarterDateFormat</xsl:message>
			  </xsl:when>
			  <xsl:when test="number:date-style/number:week-of-year">
				  <xsl:message terminate="no">translation.odf2oox.WeekDateFormat</xsl:message>
			  </xsl:when>
		  </xsl:choose>
	  </xsl:for-each>
	  <xsl:for-each select ="document('styles.xml')/office:document-styles/office:styles">
		  <xsl:choose>
			  <xsl:when test="number:date-style/number:quarter">
				  <xsl:message terminate="no">translation.odf2oox.QuarterDateFormat</xsl:message>
			  </xsl:when>
			  <xsl:when test="number:date-style/number:week-of-year">
				  <xsl:message terminate="no">translation.odf2oox.WeekDateFormat</xsl:message>
			  </xsl:when>
		  </xsl:choose>
	  </xsl:for-each>


	  <numFmts>

      <!-- number of all number styles in content.xml -->
      <xsl:variable name="countNumber">
        <xsl:value-of
          select="count(office:document-content/office:automatic-styles/number:number-style)"
        />
      </xsl:variable>

      <!-- number of all number styles in styles.xml -->
      <xsl:variable name="countStyleNumber">
        <xsl:value-of
          select="count(document('styles.xml')/office:document-styles/office:styles/number:number-style)"
        />
      </xsl:variable>

      <!-- number of all percentage styles in content.xml -->
      <xsl:variable name="countPercentage">
        <xsl:value-of
          select="count(office:document-content/office:automatic-styles/number:percentage-style)"
        />
      </xsl:variable>

      <!-- number of all percentage styles in styles.xml -->
      <xsl:variable name="countStylePercentage">
        <xsl:value-of
          select="count(document('styles.xml')/office:document-styles/office:styles/number:percentage-style)"
        />
      </xsl:variable>

      <!-- number of all currency styles in content.xml -->
      <xsl:variable name="countCurrency">
        <xsl:value-of
          select="count(office:document-content/office:automatic-styles/number:currency-style)"
        />
      </xsl:variable>

      <!-- number of all currency styles in styles.xml -->
      <xsl:variable name="countStyleCurrency">
        <xsl:value-of
          select="count(document('styles.xml')/office:document-styles/office:styles/number:currency-style)"
        />
      </xsl:variable>

      <!-- number of all date styles in content.xml -->
      <xsl:variable name="countDate">
        <xsl:value-of
          select="count(office:document-content/office:automatic-styles/number:date-style)"
        />
      </xsl:variable>

      <!-- number of all date styles in styles.xml -->
      <xsl:variable name="countStyleDate">
        <xsl:value-of
          select="count(document('styles.xml')/office:document-styles/office:styles/number:date-style)"
        />
      </xsl:variable>

      <!-- number of all time styles in content.xml -->
      <xsl:variable name="countTime">
        <xsl:value-of
          select="count(office:document-content/office:automatic-styles/number:time-style)"
        />
      </xsl:variable>

      <!-- number of all time styles in styles.xml -->
      <xsl:variable name="countStyleTime">
        <xsl:value-of
          select="count(document('styles.xml')/office:document-styles/office:styles/number:time-style)"
        />
      </xsl:variable>

      <xsl:attribute name="count">
        <xsl:value-of
          select="$countNumber+$countStyleNumber+$countPercentage+$countStylePercentage+$countCurrency+$countStyleCurrency+$countDate+$countStyleDate+$countTime+$countStyleTime"
        />
      </xsl:attribute>

      <!-- apply number styles from content.xml -->
      <xsl:apply-templates
        select="office:document-content/office:automatic-styles/number:number-style[1]"
        mode="numFormat">
        <xsl:with-param name="numId">1</xsl:with-param>
        <xsl:with-param name="styleName"/>
      </xsl:apply-templates>
		<!--Defect Id: 1959472
                    Fixed By : Vijayeta Tilak
                    Date     : 4th Jul '08
                    Desc     : user defined format #####Bf handeled-->
		<xsl:apply-templates
        select="office:document-content/office:automatic-styles/number:text-style[1]"
        mode="numFormat">
			<xsl:with-param name="numId">1</xsl:with-param>
			<xsl:with-param name="styleName"/>
		</xsl:apply-templates>		

      <!-- apply number styles from styles.xml -->
      <xsl:apply-templates
        select="document('styles.xml')/office:document-styles/office:styles/number:number-style[1]"
        mode="numFormat">
        <xsl:with-param name="numId">
          <xsl:value-of select="$countNumber+1"/>
        </xsl:with-param>
        <xsl:with-param name="styleName"/>
      </xsl:apply-templates>

      <!-- apply percentage styles from content.xml -->
      <xsl:apply-templates
        select="office:document-content/office:automatic-styles/number:percentage-style[1]"
        mode="numFormat">
        <xsl:with-param name="numId">
          <xsl:value-of select="$countNumber+$countStyleNumber+1"/>
        </xsl:with-param>
        <xsl:with-param name="styleName"/>
      </xsl:apply-templates>

      <!-- apply percentage styles from styles.xml -->
      <xsl:apply-templates
        select="document('styles.xml')/office:document-styles/office:styles/number:percentage-style[1]"
        mode="numFormat">
        <xsl:with-param name="numId">
          <xsl:value-of select="$countNumber+$countStyleNumber+$countPercentage+1"/>
        </xsl:with-param>
        <xsl:with-param name="styleName"/>
      </xsl:apply-templates>

      <!-- apply currency styles from content.xml -->
      <xsl:apply-templates
        select="office:document-content/office:automatic-styles/number:currency-style[1]"
        mode="numFormat">
        <xsl:with-param name="numId">
          <xsl:value-of
            select="$countNumber+$countStyleNumber+$countPercentage+$countStylePercentage+1"/>
        </xsl:with-param>
        <xsl:with-param name="styleName"/>
      </xsl:apply-templates>

      <!-- apply currency styles from styles.xml -->
      <xsl:apply-templates
        select="document('styles.xml')/office:document-styles/office:styles/number:currency-style[1]"
        mode="numFormat">
        <xsl:with-param name="numId">
          <xsl:value-of
            select="$countNumber+$countStyleNumber+$countPercentage+$countStylePercentage+$countCurrency+1"
          />
        </xsl:with-param>
        <xsl:with-param name="styleName"/>
      </xsl:apply-templates>

      <!-- apply date styles from content.xml -->
      <xsl:apply-templates
        select="office:document-content/office:automatic-styles/number:date-style[1]"
        mode="numFormat">
        <xsl:with-param name="numId">
          <xsl:value-of
            select="$countNumber+$countStyleNumber+$countPercentage+$countStylePercentage+$countCurrency+$countStyleCurrency+1"
          />
        </xsl:with-param>
        <xsl:with-param name="styleName"/>
      </xsl:apply-templates>

      <!-- apply date styles from styles.xml -->
      <xsl:apply-templates
        select="document('styles.xml')/office:document-styles/office:styles/number:date-style[1]"
        mode="numFormat">
        <xsl:with-param name="numId">
          <xsl:value-of
            select="$countNumber+$countStyleNumber+$countPercentage+$countStylePercentage+$countCurrency+$countStyleCurrency+$countDate+1"
          />
        </xsl:with-param>
        <xsl:with-param name="styleName"/>
      </xsl:apply-templates>

      <!-- apply time styles from content.xml -->
      <xsl:apply-templates
        select="office:document-content/office:automatic-styles/number:time-style[1]"
        mode="numFormat">
        <xsl:with-param name="numId">
          <xsl:value-of
            select="$countNumber+$countStyleNumber+$countPercentage+$countStylePercentage+$countCurrency+$countStyleCurrency+$countDate+$countStyleDate+1"
          />
        </xsl:with-param>
        <xsl:with-param name="styleName"/>
      </xsl:apply-templates>

      <!-- apply time styles from styles.xml -->
      <xsl:apply-templates
        select="document('styles.xml')/office:document-styles/office:styles/number:time-style[1]"
        mode="numFormat">
        <xsl:with-param name="numId">
          <xsl:value-of
            select="$countNumber+$countStyleNumber+$countPercentage+$countStylePercentage+$countCurrency+$countStyleCurrency+$countDate+$countStyleDate+$countTime+1"
          />
        </xsl:with-param>
        <xsl:with-param name="styleName"/>
      </xsl:apply-templates>

    </numFmts>
  </xsl:template>

  <!-- insert all fonts -->

  <xsl:template name="InsertFonts">
    <fonts>
      <xsl:attribute name="count">
        <xsl:value-of
          select="count(office:document-content/office:automatic-styles/style:style[@style:family='table-cell']/style:text-properties) + 1 + count(document('content.xml')/descendant::text:a[not(ancestor::draw:custom-shape)])"
        />
      </xsl:attribute>

      <!-- default font-->
		<xsl:for-each
              select="document('styles.xml')">
      <xsl:choose>
        <xsl:when
				  test="office:document-styles/office:styles/style:style[@style:name='Default' and @style:family='table-cell']/style:text-properties">
          <font>
            <xsl:for-each
						  select="office:document-styles/office:styles/style:style[@style:name='Default' and @style:family='table-cell']/style:text-properties">
              <xsl:call-template name="InsertTextProperties">
                <xsl:with-param name="mode">default</xsl:with-param>
              </xsl:call-template>
            </xsl:for-each>
          </font>
        </xsl:when>

        <!-- application default-->
        <xsl:otherwise>
          <font>
            <sz val="10"/>
            <name val="Arial"/>
          </font>
        </xsl:otherwise>
      </xsl:choose>
		</xsl:for-each>

      <xsl:apply-templates
        select="office:document-content/office:automatic-styles"
        mode="fonts"/>
		<!--Code added  by vijayeta, Fix for the bug 1797056, date: 2nd Nov '07-->
		<!--<xsl:apply-templates
        select="office:document-content/office:automatic-styles"
        mode="notesFonts"/>-->
		<!--End of code added  by vijayeta, Fix for the bug 1797056, date: 2nd Nov '07-->
       <!--<xsl:apply-templates select="document('styles.xml')/office:document-styles/office:styles"
        mode="fonts"/>-->
		<!--style:text-properties[parent::node()[@style:family='table-cell']-->
		<xsl:for-each select ="office:document-content/office:automatic-styles/style:style[@style:family='text']">
			<xsl:call-template name="notesFontsspcl"/>
		</xsl:for-each>
		<xsl:for-each select ="document('styles.xml')/office:document-styles/office:styles/child::node()[@style:family='table-cell']/style:text-properties">
			<xsl:call-template name="fontsspcl"/>
		</xsl:for-each>
      <!--hyperlink font properties-->
      <xsl:choose>
        <xsl:when
          test="descendant::text:a[not(ancestor::draw:custom-shape)and not(ancestor::office:annotation)]">
          <xsl:call-template name="InsertHyperlinkTextStyle"/>

        </xsl:when>
      </xsl:choose>

    </fonts>
  </xsl:template>

	<xsl:template name="notesFontsspcl">
		<xsl:param name="hyperlink"/>
		<font>
			<xsl:call-template name="InsertTextProperties">
				<xsl:with-param name="mode">fonts</xsl:with-param>
				<xsl:with-param name="hyperlink">
					<xsl:value-of select="$hyperlink"/>
				</xsl:with-param>
			</xsl:call-template>
		</font>
	</xsl:template>

	<xsl:template name="fontsspcl">
		<xsl:param name="hyperlink"/>
		<font>
			<xsl:call-template name="InsertTextProperties">
				<xsl:with-param name="mode">fonts</xsl:with-param>
				<xsl:with-param name="hyperlink">
					<xsl:value-of select="$hyperlink"/>
				</xsl:with-param>
			</xsl:call-template>
		</font>
	</xsl:template>
	
  <xsl:template name="InsertFills">
    <fills>
      <fill>
        <patternFill patternType="none"/>
      </fill>
      <fill>
        <patternFill patternType="gray125"/>
      </fill>

      <xsl:for-each select="office:document-content/office:automatic-styles/child::node()/style:table-cell-properties">
        <xsl:call-template name="InsertFillForBackground"/>
      </xsl:for-each>
      <xsl:for-each select="document('styles.xml')/office:document-styles/office:styles/child::node()/style:table-cell-properties">
        <xsl:call-template name="InsertFillForBackground"/>
      </xsl:for-each>
      <!--<xsl:apply-templates
        select="office:document-content/office:automatic-styles"
        mode="background-color"/>

      <xsl:apply-templates select="document('styles.xml')/office:document-styles/office:styles"
        mode="background-color"/>-->
    </fills>
  </xsl:template>
  <xsl:template match="style:table-cell-properties" mode="background-color">
    <xsl:param name="Object"/>
		<xsl:call-template name ="InsertFillForBackground">
			<xsl:with-param name="Object" select ="$Object"/>
		</xsl:call-template>
  </xsl:template>
	<xsl:template name ="InsertFillForBackground">
		<xsl:param name="Object"/>
    <fill>
      <xsl:choose>
        <xsl:when test="@fo:background-color and @fo:background-color != 'transparent'">
          <xsl:call-template name="GetCellColor">
            <xsl:with-param name="color">
              <xsl:value-of select="substring-after(@fo:background-color, '#')"/>
            </xsl:with-param>
            <xsl:with-param name="Object">
              <xsl:value-of select="$Object"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <patternFill patternType="none"/>
        </xsl:otherwise>
      </xsl:choose>
    </fill>
  </xsl:template>

  <xsl:template name="GetCellColor">
    <xsl:param name="color"/>
    <xsl:param name="Object"/>
    <xsl:choose>
      <xsl:when test="$Object = 'conditional'">
        <xsl:choose>
          <xsl:when test="$color">
            <patternFill>
              <xsl:attribute name="patternType">
                <xsl:text>solid</xsl:text>
              </xsl:attribute>
              <bgColor>
                <xsl:attribute name="rgb">
                  <xsl:value-of select="concat('FF', $color)"/>
                </xsl:attribute>
              </bgColor>
            </patternFill>
          </xsl:when>
        </xsl:choose>
      </xsl:when>
      <xsl:otherwise>
        <xsl:choose>
          <xsl:when test="$color">
            <patternFill>
              <xsl:attribute name="patternType">
                <xsl:text>solid</xsl:text>
              </xsl:attribute>
              <fgColor>
                <xsl:attribute name="rgb">
                  <xsl:value-of select="concat('FF', $color)"/>
                </xsl:attribute>
              </fgColor>
              <bgColor>
                <xsl:attribute name="indexed">
                  <xsl:text>64</xsl:text>
                </xsl:attribute>
              </bgColor>
            </patternFill>
          </xsl:when>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>


  </xsl:template>

  <xsl:template name="InsertBorders">

    <borders>
      <!-- default border -->
      <border>
        <left/>
        <right/>
        <top/>
        <bottom/>
        <diagonal/>
      </border>
      <xsl:apply-templates
        select="office:document-content/office:automatic-styles"
        mode="border"/>

      <xsl:apply-templates select="document('styles.xml')/office:document-styles/office:styles"
        mode="border"/>
    </borders>
  </xsl:template>


	<xsl:template name="InsertCellFormats">

		<!-- number of multiline cells in document -->
		<xsl:variable name="multilines">
			<xsl:choose>
				<xsl:when
				  test="office:document-content/office:body/office:spreadsheet/descendant::table:table-cell[text:p[2]]">
					<xsl:for-each
					  select="office:document-content/office:body/office:spreadsheet/descendant::text:p[last()]">
						<xsl:number count="table:table-cell[text:p[2]]" level="any"/>
					</xsl:for-each>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>0</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:for-each select ="document('styles.xml')/office:document-styles">
			<xsl:variable name="numStyleCount">
				<xsl:value-of
				   select="count(document('content.xml')/office:document-content/office:automatic-styles/number:number-style)"/>
			</xsl:variable>

			<xsl:variable name="styleNumStyleCount">
				<xsl:value-of
				  select="count(office:styles/number:number-style)"/>
			</xsl:variable>

			<xsl:variable name="percentStyleCount">
				<xsl:value-of
				  select="count(document('content.xml')/office:document-content/office:automatic-styles/number:percentage-style)"/>
			</xsl:variable>

			<xsl:variable name="stylePercentStyleCount">
				<xsl:value-of
				  select="count(office:styles/number:percentage-style)"/>
			</xsl:variable>

			<xsl:variable name="currencyStyleCount">
				<xsl:value-of
				  select="count(document('content.xml')/office:document-content/office:automatic-styles/number:currency-style)"/>
			</xsl:variable>

			<xsl:variable name="styleCurrencyStyleCount">
				<xsl:value-of
				  select="count(office:styles/number:currency-style)"/>
			</xsl:variable>

			<xsl:variable name="dateStyleCount">
				<xsl:value-of
				  select="count(document('content.xml')/office:document-content/office:automatic-styles/number:date-style)"
      />
			</xsl:variable>

			<xsl:variable name="styleDateStyleCount">
				<xsl:value-of
				  select="count(office:styles/number:date-style)"
      />
			</xsl:variable>

			<xsl:variable name="timeStyleCount">
				<xsl:value-of
				  select="count(document('content.xml')/office:document-content/office:automatic-styles/number:time-style)"
      />
			</xsl:variable>

			<xsl:variable name="contentFontsCount">
				<xsl:value-of
				  select="count(document('content.xml')/office:document-content/office:automatic-styles/style:style[@style:family='table-cell' or @style:family='text']/style:text-properties)"
      />
			</xsl:variable>

			<xsl:variable name="styleFontsCount">
				<xsl:value-of
				  select="count(office:styles/style:style[(@style:family='table-cell' or @style:family='text') and not(@style:name='Default')]/style:text-properties)"
      />
			</xsl:variable>

			<xsl:variable name="contentFillCount">
				<xsl:value-of
				  select="count(document('content.xml')/office:document-content/office:automatic-styles/style:style[@style:family='table-cell']/style:table-cell-properties)"
      />
			</xsl:variable>

			<cellStyleXfs>
				<xsl:attribute name="count">
					<xsl:value-of
					  select="count(office:styles/style:style[@style:family='table-cell']) + 1"
        />
				</xsl:attribute>

				<!-- default style -->
				<xf numFmtId="0" fontId="0" fillId="0" borderId="0"/>
				<xsl:apply-templates
				  select="office:styles/style:style"
				  mode="cellFormats">
					<xsl:with-param name="numStyleCount" select="$numStyleCount"/>						
					<xsl:with-param name="styleNumStyleCount" select="$styleNumStyleCount"/>						
					<xsl:with-param name="percentStyleCount" select="$percentStyleCount"/>
					<xsl:with-param name="stylePercentStyleCount" select="$stylePercentStyleCount"/>						
					<xsl:with-param name="currencyStyleCount" select="$currencyStyleCount"/>
					<xsl:with-param name="styleCurrencyStyleCount" select="$styleCurrencyStyleCount"/>					
					<xsl:with-param name="dateStyleCount" select="$dateStyleCount"/>					
					<xsl:with-param name="styleDateStyleCount" select="$styleDateStyleCount"/>					
					<xsl:with-param name="timeStyleCount" select="$timeStyleCount"/>					
					<xsl:with-param name="FileName">
						<xsl:text>styles</xsl:text>
					</xsl:with-param>
					<xsl:with-param name="AtributeName">
						<xsl:text>cellStyleXfs</xsl:text>
					</xsl:with-param>
					<xsl:with-param name="contentFontsCount" select="$contentFontsCount"/>					
					<xsl:with-param name="styleFontsCount" select="$styleFontsCount"/>					
					<xsl:with-param name="contentFillCount" select="$contentFillCount"/>					
				</xsl:apply-templates>

				<!-- add cell formats for hyperlinks-->
				<xsl:if
				  test="not(office:styles/style:style[@style:name = 'Hyperlink' and @style:family = 'table-cell'])">
					<xsl:call-template name="InsertHyperlinksProperties"/>
				</xsl:if>

			</cellStyleXfs>

			<cellXfs>
                          <xsl:variable name ="ValOfCount">
				  <xsl:for-each select ="document('content.xml')">					  
					  <xsl:value-of
					  select="count(office:document-content/office:automatic-styles/style:style[@style:family='table-cell']) + 1 + $multilines  + count(descendant::text:a[@xlink:href and not(ancestor::draw:custom-shape) and not(ancestor::office:annotation)])"/>					  
					</xsl:for-each>
				</xsl:variable>
				<xsl:attribute name="count">
					<xsl:value-of
					  select="$ValOfCount"/>
				</xsl:attribute>

				<!-- default style -->
				<xf numFmtId="0" fontId="0" fillId="0" borderId="0" xfId="0"/>

				<!-- output cell formats from content.xml -->
				<xsl:apply-templates
				  select="document('content.xml')/office:document-content/office:automatic-styles/style:style"
				  mode="cellFormats">
					<xsl:with-param name="numStyleCount" select="$numStyleCount"/>					
					<xsl:with-param name="styleNumStyleCount" select="$styleNumStyleCount"/>					
					<xsl:with-param name="percentStyleCount" select="$percentStyleCount"/>					
					<xsl:with-param name="stylePercentStyleCount" select="$stylePercentStyleCount"/>					
					<xsl:with-param name="currencyStyleCount" select="$currencyStyleCount"/>					
					<xsl:with-param name="styleCurrencyStyleCount" select="$styleCurrencyStyleCount"/>					
					<xsl:with-param name="dateStyleCount" select="$dateStyleCount"/>
					<xsl:with-param name="styleDateStyleCount" select="$styleDateStyleCount"/>					
					<xsl:with-param name="timeStyleCount" select="$timeStyleCount"/>					
					<xsl:with-param name="contentFontsCount" select="$contentFontsCount"/>					
					<xsl:with-param name="styleFontsCount" select="$styleFontsCount"/>					
					<xsl:with-param name="contentFillCount" select="$contentFillCount"/>					
					<xsl:with-param name="postStyleName">
						<xsl:text>true</xsl:text>
					</xsl:with-param>
				</xsl:apply-templates>

				<!-- output cell formats from styles.xml -->
				<xsl:apply-templates
				  select="office:styles/style:style"
				  mode="cellFormats">
					<xsl:with-param name="numStyleCount" select="$numStyleCount"/>					
					<xsl:with-param name="styleNumStyleCount" select="$styleNumStyleCount"/>					
					<xsl:with-param name="percentStyleCount" select="$percentStyleCount"/>
					<xsl:with-param name="stylePercentStyleCount" select="$stylePercentStyleCount"/>
					<xsl:with-param name="currencyStyleCount" select="$currencyStyleCount"/>
					<xsl:with-param name="styleCurrencyStyleCount" select="$styleCurrencyStyleCount"/>
					<xsl:with-param name="dateStyleCount" select="$dateStyleCount"/>
					<xsl:with-param name="styleDateStyleCount" select="$styleDateStyleCount"/>
					<xsl:with-param name="timeStyleCount" select="$timeStyleCount"/>
					<xsl:with-param name="FileName">
						<xsl:text>styles</xsl:text>
					</xsl:with-param>
					<xsl:with-param name="contentFontsCount" select="$contentFontsCount"/>
					<xsl:with-param name="styleFontsCount" select="$styleFontsCount"/>
					<xsl:with-param name="contentFillCount" select="$contentFillCount"/>
					<xsl:with-param name="postStyleName">
						<xsl:text>true</xsl:text>
					</xsl:with-param>
				</xsl:apply-templates>

        <!-- add cell formats for multiline cells, which must have wrap property -->

				<xsl:call-template name="InsertMultilineCellFormats"/>

        <!-- add cell formats for hyperlinks-->
				<xsl:call-template name="InsertHyperlinksStyleProperties">
					<xsl:with-param name="numStyleCount" select="$numStyleCount"/>
					<xsl:with-param name="styleNumStyleCount" select="$styleNumStyleCount"/>
					<xsl:with-param name="percentStyleCount" select="$percentStyleCount"/>
					<xsl:with-param name="stylePercentStyleCount" select="$stylePercentStyleCount"/>
					<xsl:with-param name="currencyStyleCount" select="$currencyStyleCount"/>
					<xsl:with-param name="styleCurrencyStyleCount" select="$styleCurrencyStyleCount"/>
					<xsl:with-param name="dateStyleCount" select="$dateStyleCount"/>
					<xsl:with-param name="styleDateStyleCount" select="$styleDateStyleCount"/>
					<xsl:with-param name="timeStyleCount" select="$timeStyleCount"/>
					<xsl:with-param name="contentFontsCount" select="$contentFontsCount"/>
					<xsl:with-param name="styleFontsCount" select="$styleFontsCount"/>
					<xsl:with-param name="contentFillCount" select="$contentFillCount"/>
					<xsl:with-param name="postStyleName">
						<xsl:text>true</xsl:text>
					</xsl:with-param>
				</xsl:call-template>
        <!--Start of RefNo-3-->
        <xf numFmtId="14" fontId="0" fillId="0" borderId="0" xfId="0"/>
        <xf numFmtId="18" fontId="0" fillId="0" borderId="0" xfId="0"/>
        <xf numFmtId="22" fontId="0" fillId="0" borderId="0" xfId="0"/>
        <!--End of RefNo-3-->
			</cellXfs>

		</xsl:for-each>

	</xsl:template>

	<xsl:template name="InsertCellStyles">
		<cellStyles>
			<xsl:for-each select="document('styles.xml')/office:document-styles">
				<xsl:for-each select="office:styles">
					<xsl:apply-templates select="style:style[@style:family = 'table-cell']" mode="cellStyle"/>
				</xsl:for-each>
				<xsl:if
				 test="not(office:styles/style:style/@style:name = 'Hyperlink') and document('content.xml')/descendant::text:a[not(ancestor::draw:custom-shape) and not(ancestor::office:annotation)]">
					<cellStyle
					  xfId="{count(office:styles/style:style[@style:family = 'table-cell']) + 1}"
					  name="Hyperlink"/>
				</xsl:if>

				<xsl:if
				  test="not(office:styles/style:style[@style:family = 'table-cell'])">
					<cellStyle name="Normal" xfId="0" builtinId="0"/>
				</xsl:if>
			</xsl:for-each>
		</cellStyles>
  </xsl:template>

  <xsl:template match="style:style[@style:family = 'table-cell']" mode="cellStyle">
    <cellStyle xfId="{position()}">
      <xsl:attribute name="name">
        <xsl:value-of select="@style:name"/>
      </xsl:attribute>
    </cellStyle>
  </xsl:template>

  <xsl:template name="InsertTableStyles">
    <tableStyles count="0" defaultTableStyle="TableStyleMedium9"
      defaultPivotStyle="PivotStyleLight16"/>
  </xsl:template>

  <!-- Modified  by vijayeta, Fix for the bug 1797056, date: 2nd Nov '07-->
  <xsl:template
     match="style:text-properties[parent::node()[@style:family='table-cell']]"
     mode="fonts">
    <!--<xsl:template
    match="style:text-properties[parent::node()[@style:family='table-cell' or @style:family='text']]"
    mode="fonts">-->
    <xsl:param name="hyperlink"/>
    <font>
      <xsl:call-template name="InsertTextProperties">
        <xsl:with-param name="mode">fonts</xsl:with-param>
        <xsl:with-param name="hyperlink">
          <xsl:value-of select="$hyperlink"/>
        </xsl:with-param>
      </xsl:call-template>
    </font>
  </xsl:template>
  <!--End of Modification  by vijayeta, Fix for the bug 1797056, date: 2nd Nov '07-->
  <!--Code added  by vijayeta, Fix for the bug 1797056, date: 2nd Nov '07-->
  <xsl:template
  match="style:text-properties[parent::node()[@style:family='text']]"
  mode="notesFonts">
    <xsl:param name="hyperlink"/>
    <font>
      <xsl:call-template name="InsertTextProperties">
        <xsl:with-param name="mode">fonts</xsl:with-param>
        <xsl:with-param name="hyperlink">
          <xsl:value-of select="$hyperlink"/>
        </xsl:with-param>
      </xsl:call-template>
    </font>
  </xsl:template>
  <!--End of code added  by vijayeta, Fix for the bug 1797056, date: 2nd Nov '07-->

  <xsl:template name="InsertUnderline">
    <xsl:param name="underlineStyle"/>
    <xsl:param name="underlineType"/>
    <xsl:if test="$underlineStyle != 'none' ">
      <u>
        <xsl:attribute name="val">
          <xsl:choose>
            <xsl:when test="$underlineStyle = 'accounting' ">
              <xsl:choose>
                <xsl:when test="$underlineType = 'double' ">doubleAccounting</xsl:when>
                <xsl:otherwise>singleAccounting</xsl:otherwise>
              </xsl:choose>
            </xsl:when>
            <xsl:otherwise>
              <xsl:choose>
                <xsl:when test="$underlineType = 'double' ">double</xsl:when>
                <xsl:otherwise>single</xsl:otherwise>
              </xsl:choose>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
      </u>
    </xsl:if>
  </xsl:template>

  <xsl:template match="style:style[@style:family='table-cell']" mode="cellFormats">
    <xsl:param name="numStyleCount"/>
    <xsl:param name="styleNumStyleCount"/>
    <xsl:param name="percentStyleCount"/>
    <xsl:param name="stylePercentStyleCount"/>
    <xsl:param name="currencyStyleCount"/>
    <xsl:param name="styleCurrencyStyleCount"/>
    <xsl:param name="dateStyleCount"/>
    <xsl:param name="styleDateStyleCount"/>
    <xsl:param name="timeStyleCount"/>
    <xsl:param name="FileName"/>
    <xsl:param name="AtributeName"/>
    <xsl:param name="contentFontsCount"/>
    <xsl:param name="styleFontsCount"/>
    <xsl:param name="contentFillCount"/>
    <xsl:param name="postStyleName" select="false"/>
    <xsl:param name="hyperlink"/>
    <xsl:param name="hyperlinkId"/>
    <!--RefNo-2-->
    <xsl:param name="hypMultiLine" select="false"/>


    <!-- number format id -->
    <xsl:variable name="numFormat">
      <xsl:call-template name="NumFmtId">
        <xsl:with-param name="FileName" select="$FileName"/>
        <xsl:with-param name="numStyleCount" select="$numStyleCount"/>
        <xsl:with-param name="styleNumStyleCount" select="$styleNumStyleCount"/>
        <xsl:with-param name="percentStyleCount" select="$percentStyleCount"/>
        <xsl:with-param name="stylePercentStyleCount" select="$stylePercentStyleCount"/>
        <xsl:with-param name="currencyStyleCount" select="$currencyStyleCount"/>
        <xsl:with-param name="styleCurrencyStyleCount" select="$styleCurrencyStyleCount"/>
        <xsl:with-param name="dateStyleCount" select="$dateStyleCount"/>
        <xsl:with-param name="styleDateStyleCount" select="$styleDateStyleCount"/>
        <xsl:with-param name="timeStyleCount" select="$timeStyleCount"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:if test="not($AtributeName = 'cellStyleXfs' and $FileName = '')">
      <xf numFmtId="{$numFormat}" fillId="0" borderId="0">
        <xsl:if test="$postStyleName">
          <xsl:choose>
            <xsl:when test="$hyperlink = 'true'">
              <xsl:attribute name="table:post-style-name">
                <xsl:value-of select="concat(@style:name, 'h')"/>
              </xsl:attribute>
            </xsl:when>
            <xsl:otherwise>
              <xsl:attribute name="table:post-style-name">
                <xsl:value-of select="@style:name"/>
              </xsl:attribute>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:if>
        <xsl:if test="$AtributeName != 'cellStyleXfs'">
          <xsl:attribute name="xfId">
            <xsl:choose>
              <xsl:when test="$FileName = 'styles'">
                <xsl:value-of select="position()"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:call-template name="XfId"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:attribute>
        </xsl:if>
        <xsl:call-template name="SetFormatProperties">
          <!--RefNo-2-->
          <xsl:with-param name="multiline" select="$hypMultiLine"/>
          <xsl:with-param name="FileName" select="$FileName"/>
          <xsl:with-param name="contentFontsCount" select="$contentFontsCount"/>
          <xsl:with-param name="styleFontsCount">
            <xsl:value-of select="$styleFontsCount"/>
          </xsl:with-param>
          <xsl:with-param name="contentFillCount" select="$contentFillCount"/>
          <xsl:with-param name="hyperlink">
            <xsl:value-of select="$hyperlink"/>
          </xsl:with-param>
          <xsl:with-param name="hyperlinkId">
            <xsl:value-of select="$hyperlinkId"/>
          </xsl:with-param>
        </xsl:call-template>
      </xf>
    </xsl:if>
  </xsl:template>

  <xsl:template name="SetFormatProperties">
    <xsl:param name="multiline" select="'false'"/>
    <xsl:param name="FileName"/>
    <xsl:param name="contentFontsCount"/>
    <xsl:param name="styleFontsCount"/>
    <xsl:param name="contentFillCount"/>
    <xsl:param name="hyperlink"/>
    <xsl:param name="hyperlinkId" select="0"/>

    <!-- font -->
    <xsl:call-template name="FontId">
      <xsl:with-param name="FileName" select="$FileName"/>
      <xsl:with-param name="contentFontsCount" select="$contentFontsCount"/>
      <xsl:with-param name="styleFontsCount">
        <xsl:value-of select="$styleFontsCount"/>
      </xsl:with-param>
      <xsl:with-param name="hyperlink">
        <xsl:value-of select="$hyperlink"/>
      </xsl:with-param>
      <xsl:with-param name="hyperlinkId">
        <xsl:value-of select="$hyperlinkId"/>
      </xsl:with-param>
    </xsl:call-template>

    <!-- border -->
<!-- Changes By: Vijayeta
     Changes   : SP2, Additional Condition Added for Diagonal Borders 
     File      :Excel_SmokeTest_InputFile..xlsx->SP2->Excel_SmokeTest_InputFile..ods->Trans->Excel_SmokeTest_InputFile..xlsx
-->
    <xsl:if
      test="(style:table-cell-properties/@fo:border) or 
	  (style:table-cell-properties/@fo:border-bottom) or
	  (style:table-cell-properties/@fo:border-left) or
	  (style:table-cell-properties/@fo:border-right)or
	  (style:table-cell-properties/@fo:border-top) or 
	  (style:table-cell-properties/@style:diagonal-bl-tr)or
	  (style:table-cell-properties/@style:diagonal-tl-br) ">
      <xsl:attribute name="applyBorder">
        <xsl:text>1</xsl:text>
      </xsl:attribute>
      <xsl:attribute name="borderId">
        <!-- change referencing node to style:table-cell-properties and count-->
        <xsl:for-each select="style:table-cell-properties">
          <xsl:number count="style:table-cell-properties[parent::node()/@style:family='table-cell']"
            level="any"/>
        </xsl:for-each>
      </xsl:attribute>
    </xsl:if>

    <!--cell background color-->

    <xsl:call-template name="BackgroundId">
      <xsl:with-param name="FileName" select="$FileName"/>
      <xsl:with-param name="contentFillCount" select="$contentFillCount"/>
    </xsl:call-template>

    <!-- text -alignment -->
    <!-- 1st 'or' - horizontal alignment
            2nd 'or' - horizontal alignment 'fill'
             3rd 'or' - vertical alignment 
             4th 'or' - angle oriented text
             5th 'or' - vertically stacked text 
             6th 'or' - wraped text -->


    <xsl:if
      test="style:table-cell-properties/@style:cell-protect and style:table-cell-properties/@style:cell-protect != 'protected' ">
      <xsl:attribute name="applyProtection">
        <xsl:text>1</xsl:text>
      </xsl:attribute>
    </xsl:if>

    <xsl:variable name="StyleParentStyleName">
      <xsl:value-of select="@style:parent-style-name"/>
    </xsl:variable>

    <xsl:variable name="horizontalAlign">
      <xsl:call-template name="InheritHorizontalAlign"/>
    </xsl:variable>

    <xsl:variable name="verticalAlign">
      <xsl:call-template name="InheritVerticalAlign"/>
    </xsl:variable>

    <xsl:variable name="repeatContent">
      <xsl:call-template name="InheritRepeatContent"/>
    </xsl:variable>

    <xsl:variable name="rotation">
      <xsl:call-template name="InheritRotation"/>
    </xsl:variable>

    <xsl:variable name="verticalText">
      <xsl:call-template name="InheritVerticalText"/>
    </xsl:variable>

    <xsl:variable name="wordWrap">
      <xsl:call-template name="InheritWordWrap"/>
    </xsl:variable>

    <xsl:if
      test="($horizontalAlign != '' ) or ($repeatContent = 'true' ) or ($verticalAlign != '' ) or ($rotation != '' ) or ($verticalText = 'ttb' ) or ($wordWrap = 'wrap' ) or ($multiline = 'true' )">
      <xsl:attribute name="applyAlignment">
        <xsl:text>1</xsl:text>
      </xsl:attribute>

      <alignment>
        <!-- horizontal alignment -->
        <!-- 1st 'or' - horizontal alignment 
                2nd 'or' - horizontal alignment 'fill' 
        -->
        <xsl:if test="($horizontalAlign != '' ) or ($repeatContent = 'true' )">
          <xsl:attribute name="horizontal">
            <xsl:choose>
              <xsl:when test="$repeatContent = 'true' ">
                <xsl:text>fill</xsl:text>
              </xsl:when>
              <xsl:otherwise>
                <xsl:if test="$horizontalAlign">
                  <xsl:choose>
                    <xsl:when test="$horizontalAlign = 'start' ">
                      <xsl:text>left</xsl:text>
                    </xsl:when>
                    <xsl:when test="$horizontalAlign = 'end' ">
                      <xsl:text>right</xsl:text>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:value-of select="$horizontalAlign"/>
                    </xsl:otherwise>
                  </xsl:choose>
                </xsl:if>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:attribute>
        </xsl:if>

        <!-- change default horizontal alignment-->
        <xsl:if test="$horizontalAlign = '' ">
          <xsl:choose>
            <!-- change default horizontal alignment  of vertically stacked text to 'left' -->
            <xsl:when test="$verticalText = 'ttb' ">
              <xsl:attribute name="horizontal">
                <xsl:text>left</xsl:text>
              </xsl:attribute>
            </xsl:when>
            <!-- change default horizontal alignment of angle oriented text when angle equals 90 degrees -->
            <xsl:when test="$rotation = 90">
              <xsl:attribute name="horizontal">
                <xsl:text>left</xsl:text>
              </xsl:attribute>
            </xsl:when>
            <!-- change default horizontal alignment of angle oriented text when angle equals -90 degrees -->
            <xsl:when test="$rotation = 270">
              <xsl:attribute name="horizontal">
                <xsl:text>right</xsl:text>
              </xsl:attribute>
            </xsl:when>
            <!-- change default alignment of angle oriented text when angle equals (-90,0) degrees or (0,90) degrees -->
            <xsl:when
              test="(($rotation &lt; 90 and $rotation &gt; 0) or $rotation &gt; 270)">
              <xsl:attribute name="horizontal">
                <xsl:text>center</xsl:text>
              </xsl:attribute>
            </xsl:when>
          </xsl:choose>
        </xsl:if>

        <!-- vertical-alignment -->
        <xsl:if test="$verticalAlign != '' ">
          <xsl:attribute name="vertical">
            <xsl:choose>
              <xsl:when test="$verticalAlign = 'automatic' ">
                <xsl:text>bottom</xsl:text>
              </xsl:when>
              <xsl:when test="$verticalAlign = 'middle' ">
                <xsl:text>center</xsl:text>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$verticalAlign"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:attribute>
        </xsl:if>

        <!-- text rotation -->
        <xsl:if test="($rotation != '' and $rotation != '0') or $verticalText = 'ttb' ">
          <xsl:attribute name="textRotation">
            <xsl:choose>
              <!-- ascending text angle -->
              <xsl:when test="$rotation &lt; 91 and not($verticalText = 'ttb' )">
                <xsl:value-of select="$rotation"/>
              </xsl:when>
              <!-- descending text angle -->
              <xsl:when test="$rotation &gt; 269">
                <xsl:value-of select="450 - $rotation"/>
              </xsl:when>
              <xsl:when test="$verticalText = 'ttb' ">
                <xsl:text>255</xsl:text>
              </xsl:when>
              <xsl:otherwise>
                <xsl:text>0</xsl:text>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:attribute>
        </xsl:if>

        <!-- wraped text -->
        <xsl:if test="$wordWrap = 'wrap' or $multiline = 'true' ">
          <xsl:attribute name="wrapText">
            <xsl:text>1</xsl:text>
          </xsl:attribute>
        </xsl:if>

        <!-- Indent  (can not be negative)-->
        <xsl:if
          test="style:paragraph-properties/@fo:margin-left and not(starts-with(style:paragraph-properties/@fo:margin-left,'-'))">
          <xsl:attribute name="indent">
            <xsl:variable name="indentLeft">
              <xsl:value-of select="style:paragraph-properties/@fo:margin-left"/>
            </xsl:variable>
            <xsl:variable name="indent_value">
              <xsl:call-template name="point-measure">
                <xsl:with-param name="length">
                  <xsl:value-of select="$indentLeft"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:variable>
            <xsl:value-of select="round($indent_value div 10)"/>
          </xsl:attribute>
        </xsl:if>

      </alignment>

    </xsl:if>

    <xsl:if
      test="style:table-cell-properties/@style:cell-protect and style:table-cell-properties/@style:cell-protect != 'protected' ">

      <!-- cell protection -->
      <protection>
        <xsl:choose>
          <xsl:when test="style:table-cell-properties/@style:cell-protect='formula-hidden' ">
            <xsl:attribute name="locked">
              <xsl:text>0</xsl:text>
            </xsl:attribute>
            <xsl:attribute name="hidden">
              <xsl:text>1</xsl:text>
            </xsl:attribute>
          </xsl:when>
          <xsl:when
            test="style:table-cell-properties/@style:cell-protect='protected formula-hidden' or style:table-cell-properties/@style:cell-protect='hidden-and-protected' ">
            <xsl:attribute name="hidden">
              <xsl:text>1</xsl:text>
            </xsl:attribute>
          </xsl:when>
        </xsl:choose>
      </protection>
    </xsl:if>

  </xsl:template>

  <!-- Insert Font Style -->

  <xsl:template name="FontId">
    <xsl:param name="FileName"/>
    <xsl:param name="contentFontsCount"/>
    <xsl:param name="styleFontsCount"/>
    <xsl:param name="hyperlink"/>
    <xsl:param name="hyperlinkId"/>

    <xsl:choose>
      <xsl:when test="style:text-properties">
        <xsl:attribute name="applyFont">
          <xsl:text>1</xsl:text>
        </xsl:attribute>

        <xsl:if test="$contentFontsCount !='' ">
          <!--<xsl:attribute name="ZMIENNA_contentFontsCount">
            <xsl:value-of select="$contentFontsCount"/>
          </xsl:attribute>-->
        </xsl:if>

        <xsl:if test="$styleFontsCount !='' ">
          <!--<xsl:attribute name="ZMIENNA_styleFontsCount">
            <xsl:value-of select="$styleFontsCount"/>
          </xsl:attribute>-->
        </xsl:if>

        <xsl:if test="$hyperlinkId !='' ">
          <!--<xsl:attribute name="ZMIENNA_hyperlinkId">
            <xsl:value-of select="$hyperlinkId"/>
          </xsl:attribute>-->
        </xsl:if>

        <xsl:attribute name="fontId">
          <xsl:choose>
            <xsl:when test="$hyperlink = 'true'">
              <xsl:value-of select="$contentFontsCount + $styleFontsCount + $hyperlinkId + 1"/>
            </xsl:when>
            <xsl:otherwise>
              <!-- change referencing node to style:text-properties and count-->
              <xsl:for-each select="style:text-properties[1]">
                <xsl:variable name="fontNum">
                  <xsl:number
                    count="style:text-properties[parent::node()/@style:family='table-cell']"
                    level="any"/>
                </xsl:variable>
                <xsl:choose>
                  <xsl:when test="$FileName = 'styles' ">
                    <xsl:value-of select="$contentFontsCount + $fontNum"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="$fontNum"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:for-each>
            </xsl:otherwise>
          </xsl:choose>


        </xsl:attribute>

      </xsl:when>
      <xsl:when
        test="(@style:parent-style-name != '' and @style:parent-style-name != 'Default') or (not(@table:style-name) and $hyperlink = 'true')">
        <xsl:variable name="StyleParentStyleName">
          <xsl:value-of select="@style:parent-style-name"/>
        </xsl:variable>
        <xsl:choose>
          <xsl:when test="$hyperlink = 'true'">
            <xsl:attribute name="fontId">
              <xsl:value-of select="$contentFontsCount + $styleFontsCount + $hyperlinkId + 1"/>
            </xsl:attribute>
          </xsl:when>
          <xsl:when test="key('style',@style:parent-style-name)/style:text-properties">
            <xsl:for-each select="key('style',$StyleParentStyleName)">
              <xsl:call-template name="FontId">
                <xsl:with-param name="FileName" select="$FileName"/>
                <xsl:with-param name="contentFontsCount">
                  <xsl:value-of select="$contentFontsCount"/>
                </xsl:with-param>
                <xsl:with-param name="styleFontsCount">
                  <xsl:value-of select="$styleFontsCount"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
            <xsl:for-each
              select="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name = $StyleParentStyleName]">
              <xsl:call-template name="FontId">
                <xsl:with-param name="FileName">
                  <xsl:text>styles</xsl:text>
                </xsl:with-param>
                <xsl:with-param name="contentFontsCount">
                  <xsl:value-of select="$contentFontsCount"/>
                </xsl:with-param>
                <xsl:with-param name="styleFontsCount">
                  <xsl:value-of select="$styleFontsCount"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:for-each>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:otherwise>
        <xsl:attribute name="fontId">
          <xsl:text>0</xsl:text>
        </xsl:attribute>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="BackgroundId">
    <xsl:param name="FileName"/>
    <xsl:param name="contentFillCount"/>


    <xsl:choose>
      <xsl:when
        test="style:table-cell-properties/@fo:background-color and style:table-cell-properties/@fo:background-color != 'transparent'">
        <xsl:attribute name="applyFill">
          <xsl:text>1</xsl:text>
        </xsl:attribute>
        <xsl:attribute name="fillId">
          <!-- change referencing node to style:table-cell-properties and count-->
          <xsl:variable name="fill">
            <xsl:for-each select="style:table-cell-properties">
              <xsl:number
                count="style:table-cell-properties[parent::node()/@style:family='table-cell']"
                level="any"/>
            </xsl:for-each>
          </xsl:variable>
          <xsl:choose>
            <xsl:when test="$FileName = 'styles' ">
              <xsl:value-of select="$contentFillCount + $fill + 1"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$fill + 1"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
      </xsl:when>

      <xsl:when test="@style:parent-style-name != '' and @style:parent-style-name != 'Default'">
        <xsl:variable name="StyleParentStyleName">
          <xsl:value-of select="@style:parent-style-name"/>
        </xsl:variable>

        <xsl:choose>
          <xsl:when
            test="key('style',@style:parent-style-name)/style:table-cell-properties/@fo:background-color and key('style',@style:parent-style-name)/style:table-cell-properties/@fo:background-color != 'transparent'">
            <xsl:for-each select="key('style',$StyleParentStyleName)">
              <xsl:call-template name="BackgroundId">
                <xsl:with-param name="FileName" select="$FileName"/>
                <xsl:with-param name="contentFillCount">
                  <xsl:value-of select="$contentFillCount"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
            <xsl:for-each
              select="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name = $StyleParentStyleName]">
              <xsl:call-template name="BackgroundId">
                <xsl:with-param name="FileName">
                  <xsl:text>styles</xsl:text>
                </xsl:with-param>
                <xsl:with-param name="contentFillCount">
                  <xsl:value-of select="$contentFillCount"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:for-each>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:otherwise>
        <xsl:attribute name="fillId">
          <xsl:text>0</xsl:text>
        </xsl:attribute>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="XfId">
    <xsl:choose>
      <xsl:when test="@style:parent-style-name != ''">
        <xsl:variable name="StyleParentStyleName">
          <xsl:value-of select="@style:parent-style-name"/>
        </xsl:variable>
        <xsl:choose>
          <!-- search parent style in styles.xml -->
          <xsl:when
            test="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name = $StyleParentStyleName]">
            <xsl:for-each
              select="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name = $StyleParentStyleName]">
              <xsl:value-of
                select="count(preceding-sibling::style:style[@style:family='table-cell']) + 1"/>
            </xsl:for-each>
          </xsl:when>
          <!-- search parent style in content.xml -->
          <xsl:when test="key('style',$StyleParentStyleName)">
            <xsl:for-each select="key('style',$StyleParentStyleName)">
              <xsl:call-template name="XfId"/>
            </xsl:for-each>
          </xsl:when>
          <!-- when parent style is specified but it doesn't exist -->
          <xsl:otherwise>
            <xsl:text>0</xsl:text>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:otherwise>
        <xsl:text>0</xsl:text>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- insert run properties -->
  <xsl:template match="style:style" mode="textstyles">
    <xsl:param name="parentCellStyleName"/>
    <xsl:param name="defaultCellStyleName"/>
    <!--Start of RefNo-3-->
    <xsl:param name="hyperlinkExist" select="'false'"/>
    <!--End of RefNo-3-->
    <xsl:if test="style:text-properties">
      <xsl:variable name="FontSize">
        <xsl:value-of select="@style:family"/>
      </xsl:variable>

      <xsl:variable name="styleNameName">
        <xsl:value-of select="@style:name"/>
      </xsl:variable>

      <xsl:if test="not(preceding-sibling::style:style/@style:name = $styleNameName)">
        <rPr>
          <!-- if the style is for the text that is inside text:span tag -->
          <xsl:if
            test="style:text-properties/parent::node()/@style:name = //table:table-cell/child::*/text:span/@text:style-name and not(style:text-properties/parent::node()/@style:name = //table:table-cell/@table:style-name and not(style:text-properties/parent::node()/@style:name = //table:table-cell/child::*/text:span/@text:style-name))">
            <xsl:variable name="StyleName">
              <xsl:value-of select="@style:name"/>
            </xsl:variable>
            <xsl:variable name="ParentStyleName">
              <xsl:value-of
                select="//text:span[@text:style-name = $StyleName]/parent::node()/parent::node()/@table:style-name"
              />
            </xsl:variable>
            <xsl:variable name="ParentFontSize">
              <xsl:value-of
                select="//style:style[@style:name = $ParentStyleName]/style:text-properties/@fo:font-size"
              />
            </xsl:variable>
            <xsl:variable name="ParentFontWeight">
              <xsl:value-of
                select="//style:style[@style:name = $ParentStyleName]/style:text-properties/@fo:font-weight"
              />
            </xsl:variable>

            <!-- if text surrounded by text:span -->
            <xsl:if test="@style:family = 'text'">
              <!-- font size -->
              <sz>
                <xsl:attribute name="val">
                  <!-- if there is font size for the current span -->
                  <xsl:if test="@fo:font-size!=''">
                    <xsl:value-of select="@fo:font-size"/>
                  </xsl:if>
                  <!-- if not but there is a global font size, insert global font size -->
                  <xsl:if test="not(@fo:font-size) and $ParentFontSize!=''">
                    <xsl:value-of select="translate(translate($ParentFontSize, 'p', ''), 't', '')"/>
                  </xsl:if>
                  <!-- else insert some default font size-->
                  <xsl:if test="not(@fo:font-size) and $ParentFontSize=''">
                    <xsl:value-of select="10"/>
                  </xsl:if>
                </xsl:attribute>
              </sz>
              <xsl:if test="not(@fo:font-weight) and $ParentFontWeight = 'bold'">
                <b/>
              </xsl:if>
            </xsl:if>

          </xsl:if>
          <xsl:apply-templates select="style:text-properties" mode="textstyles">
            <xsl:with-param name="parentCellStyleName" select="$parentCellStyleName"/>
            <xsl:with-param name="defaultCellStyleName" select="$defaultCellStyleName"/>
            <!--Start of RefNo-3-->
            <xsl:with-param name="hyperlink" select="$hyperlinkExist"/>
            <!--End of RefNo-3-->
          </xsl:apply-templates>
        </rPr>
      </xsl:if>
    </xsl:if>
  </xsl:template>

  <!-- convert text properties -->
  <xsl:template match="style:text-properties" mode="textstyles">
    <xsl:param name="parentCellStyleName"/>
    <xsl:param name="defaultCellStyleName"/>
    <xsl:param name="hyperlink"/>
    <xsl:call-template name="InsertTextProperties">
      <xsl:with-param name="mode">textstyles</xsl:with-param>
      <xsl:with-param name="parentCellStyleName" select="$parentCellStyleName"/>
      <xsl:with-param name="defaultCellStyleName" select="$defaultCellStyleName"/>
      <xsl:with-param name="hyperlink">
        <xsl:value-of select="$hyperlink"/>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <!-- insert text properties -->
  <xsl:template name="InsertTextProperties">
    <xsl:param name="mode"/>
    <xsl:param name="parentCellStyleName"/>
    <xsl:param name="defaultCellStyleName"/>
    <xsl:param name="hyperlink"/>

    <!-- font weight -->
    <xsl:variable name="fontWeight">
      <xsl:choose>
        <xsl:when test="name() = 'style:text-properties' ">
          <xsl:for-each select="parent::node()[name(.) = 'style:style' ]">
            <xsl:call-template name="InheritFontWeight"/>
          </xsl:for-each>
        </xsl:when>
        <xsl:when test="name() = 'style:style' ">
          <xsl:call-template name="InheritFontWeight"/>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="fontWeightComplex">
      <xsl:choose>
        <xsl:when test="name() = 'style:text-properties' ">
          <xsl:for-each select="parent::node()[name(.) = 'style:style' ]">
            <xsl:call-template name="InheritFontWeightComplex"/>
          </xsl:for-each>
        </xsl:when>
        <xsl:when test="name() = 'style:style' ">
          <xsl:call-template name="InheritFontWeightComplex"/>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>

    <xsl:if test="$fontWeight = 'bold' or $fontWeightComplex = 'bold' ">
      <b/>
    </xsl:if>

    <!-- italic -->
    <xsl:variable name="fontStyle">
      <xsl:choose>
        <xsl:when test="name() = 'style:text-properties' ">
          <xsl:for-each select="parent::node()[name(.) = 'style:style' ]">
            <xsl:call-template name="InheritFontStyle"/>
          </xsl:for-each>
        </xsl:when>
        <xsl:when test="name() = 'style:style' ">
          <xsl:call-template name="InheritFontStyle"/>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>

    <xsl:if test="$fontWeight = 'italic' or $fontStyle = 'italic' ">
      <i/>
    </xsl:if>

    <!-- underline -->
    <xsl:variable name="fontUnderlineStyle">
      <xsl:choose>
        <xsl:when test="name() = 'style:text-properties' ">
          <xsl:for-each select="parent::node()[name(.) = 'style:style' ]">
            <xsl:call-template name="InheritFontUnderlineStyle"/>
          </xsl:for-each>
        </xsl:when>
        <xsl:when test="name() = 'style:style' ">
          <xsl:call-template name="InheritFontUnderlineStyle"/>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="fontUnderlineType">
      <xsl:choose>
        <xsl:when test="name() = 'style:text-properties' ">
          <xsl:for-each select="parent::node()[name(.) = 'style:style' ]">
            <xsl:call-template name="InheritFontUnderlineType"/>
          </xsl:for-each>
        </xsl:when>
        <xsl:when test="name() = 'style:style' ">
          <xsl:call-template name="InheritFontUnderlineType"/>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>

    <xsl:if
      test="$fontUnderlineStyle != '' or key('style',$defaultCellStyleName)/style:text-properties/@style:text-underline-style">
      <xsl:choose>
        <xsl:when test="$fontUnderlineStyle != '' ">
          <xsl:call-template name="InsertUnderline">
            <xsl:with-param name="underlineStyle">
              <xsl:value-of select="$fontUnderlineStyle"/>
            </xsl:with-param>
            <xsl:with-param name="underlineType">
              <xsl:value-of select="$fontUnderlineType"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:when>
        <xsl:when
          test="key('style',$defaultCellStyleName)/style:text-properties/@style:text-underline-style">
          <xsl:call-template name="InsertUnderline">
            <xsl:with-param name="underlineStyle">
              <xsl:value-of
                select="key('style',$defaultCellStyleName)/style:text-properties/@style:text-underline-style"
              />
            </xsl:with-param>
            <xsl:with-param name="underlineType">
              <xsl:value-of
                select="key('style',$defaultCellStyleName)/style:text-properties/@style:text-underline-type"
              />
            </xsl:with-param>
          </xsl:call-template>
        </xsl:when>
      </xsl:choose>
    </xsl:if>

    <!-- font size -->
    <xsl:variable name="fontSize">
      <xsl:choose>
        <xsl:when test="name() = 'style:text-properties' ">
          <xsl:for-each select="parent::node()[name(.) = 'style:style' ]">
            <xsl:call-template name="InheritFontSize"/>
          </xsl:for-each>
        </xsl:when>
        <xsl:when test="name() = 'style:style' ">
          <xsl:call-template name="InheritFontSize"/>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="fontSizeComplex">
      <xsl:choose>
        <xsl:when test="name() = 'style:text-properties' ">
          <xsl:for-each select="parent::node()[name(.) = 'style:style' ]">
            <xsl:call-template name="InheritFontSizeComplex"/>
          </xsl:for-each>
        </xsl:when>
        <xsl:when test="name() = 'style:style' ">
          <xsl:call-template name="InheritFontSizeComplex"/>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="fontSizeAsian">
      <xsl:choose>
        <xsl:when test="name() = 'style:text-properties' ">
          <xsl:for-each select="parent::node()[name(.) = 'style:style' ]">
            <xsl:call-template name="InheritFontSizeAsian"/>
          </xsl:for-each>
        </xsl:when>
        <xsl:when test="name() = 'style:style' ">
          <xsl:call-template name="InheritFontSizeAsian"/>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>

    <xsl:if
      test="$fontSize != '' or key('style',$defaultCellStyleName)/style:text-properties/@fo:font-size or $mode = 'default'  or $fontSizeComplex != '' or $fontSizeAsian != '' ">
      <xsl:variable name="size">
        <xsl:choose>
          <xsl:when test="$fontSize != '' ">
            <xsl:value-of select="$fontSize"/>
          </xsl:when>
          <xsl:when test="key('style',$defaultCellStyleName)/style:text-properties/@fo:font-size">
            <xsl:value-of
              select="key('style',$defaultCellStyleName)/style:text-properties/@fo:font-size"/>
          </xsl:when>
          <xsl:when test="$fontSizeComplex != '' ">
            <xsl:value-of select="$fontSizeComplex"/>
          </xsl:when>
          <xsl:when test="$fontSizeAsian != '' ">
            <xsl:value-of select="$fontSizeAsian"/>
          </xsl:when>
          <xsl:when test="$mode='default'">
            <xsl:text>10</xsl:text>
          </xsl:when>
        </xsl:choose>
      </xsl:variable>
      <sz>
        <xsl:attribute name="val">
          <xsl:call-template name="point-measure">
            <xsl:with-param name="length">
              <xsl:value-of select="$size"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:attribute>
      </sz>
    </xsl:if>

    <!-- strikethrough -->
    <xsl:variable name="fontStrikethrough">
      <xsl:choose>
        <xsl:when test="name() = 'style:text-properties' ">
          <xsl:for-each select="parent::node()[name(.) = 'style:style' ]">
            <xsl:call-template name="InheritFontStrikethrough"/>
          </xsl:for-each>
        </xsl:when>
        <xsl:when test="name() = 'style:style' ">
          <xsl:call-template name="InheritFontStrikethrough"/>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>

    <xsl:if
      test="@style:text-line-through-style and @style:text-line-through-style != 'none'  or key('style',$parentCellStyleName)/style:text-properties[@style:text-line-through-style and @style:text-line-through-style != 'none'] or key('style',$defaultCellStyleName)/style:text-properties[@style:text-line-through-style and @style:text-line-through-style != 'none']">
      <strike/>
    </xsl:if>

    <!-- font position can not be inherited -->
    <!-- superscript -->
    <!-- in ODS it can be "super 58%" or "38% 58%" -->
    <xsl:if
      test="contains(@style:text-position, 'super' ) or number(substring-before(@style:text-position, '%' ) &gt; 0)">
      <vertAlign val="superscript"/>
    </xsl:if>

    <!-- subscript -->
    <!-- in ODS it can be "sub 58%" or "-38% 58%" -->
    <xsl:if
      test="contains(@style:text-position, 'sub' ) or number(substring-before(@style:text-position, '%' ) &lt; 0)">
      <vertAlign val="subscript"/>
    </xsl:if>

    <!-- font color -->
    <xsl:variable name="fontColor">
      <xsl:choose>
        <xsl:when test="name() = 'style:text-properties' ">
          <xsl:for-each select="parent::node()[name(.) = 'style:style' ]">
            <xsl:call-template name="InheritFontColor"/>
          </xsl:for-each>
        </xsl:when>
        <xsl:when test="name() = 'style:style' ">
          <xsl:call-template name="InheritFontColor"/>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>

    <xsl:choose>
     
      <xsl:when
        test="$fontColor != '' or key('style',$defaultCellStyleName)/style:text-properties/@fo:color">
        <xsl:variable name="color">
          <xsl:choose>
            <xsl:when test="$fontColor != '' ">
              <xsl:value-of select="$fontColor"/>
            </xsl:when>
            <xsl:when test="key('style',$defaultCellStyleName)/style:text-properties/@fo:color">
              <xsl:value-of
                select="key('style',$defaultCellStyleName)/style:text-properties/@fo:color"/>
            </xsl:when>
          </xsl:choose>
        </xsl:variable>
        <color rgb="{concat('FF',substring-after($color,'#'))}"/>
      </xsl:when>
      <xsl:when test="$hyperlink = 'true'">
        <color theme="10"/>
      </xsl:when>
    </xsl:choose>


    <!-- font family -->
    <xsl:variable name="fontFamily">
      <xsl:choose>
        <xsl:when test="name() = 'style:text-properties' ">
          <xsl:for-each select="parent::node()[name(.) = 'style:style' ]">
            <xsl:call-template name="InheritFontFamily"/>
          </xsl:for-each>
        </xsl:when>
        <xsl:when test="name() = 'style:style' ">
          <xsl:call-template name="InheritFontFamily"/>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="$mode = 'textstyles'">
        <xsl:if
          test="@style:font-name or key('style',$parentCellStyleName)/style:text-properties/@style:font-name or key('style',$defaultCellStyleName)/style:text-properties/@style:font-name">
          <xsl:variable name="font">
            <xsl:choose>
              <xsl:when test="@style:font-name">
                <xsl:value-of
                  select="translate(key('font',@style:font-name)/@svg:font-family,&quot;&apos;&quot;,&quot;&quot;)"
                />
              </xsl:when>
              <xsl:when
                test="key('style',$parentCellStyleName)/style:text-properties/@style:font-name">
                <xsl:value-of
                  select="translate(key('font',key('style',$parentCellStyleName)/style:text-properties/@style:font-name)/@svg:font-family,&quot;&apos;&quot;,&quot;&quot;)"
                />
              </xsl:when>
              <xsl:when
                test="key('style',$defaultCellStyleName)/style:text-properties/@style:font-name">
                <xsl:value-of
                  select="translate(key('font',key('style',$defaultCellStyleName)/style:text-properties/@style:font-name)/@svg:font-family,&quot;&apos;&quot;,&quot;&quot;)"
                />
              </xsl:when>
            </xsl:choose>
          </xsl:variable>
          <rFont val="{$font}"/>
        </xsl:if>
      </xsl:when>
      <xsl:when test="$mode = 'fonts' or $mode='default' ">
        <xsl:choose>
		<!-- code added to fix bug ref=SP2Compatibility, files-'Testfeatures M1.ods''Text.ods'-->
          <xsl:when test="$fontFamily != '' ">
			  <xsl:variable name ="apos">
				  <xsl:text >&quot;</xsl:text>
			  </xsl:variable>
            <name>
              <xsl:attribute name="val">
                <xsl:choose>
                  <xsl:when
                    test="not(translate($fontFamily,&quot;&apos;&quot;,&quot;&quot;) = '' )">
                    <!--<xsl:value-of
                      select="translate($fontFamily,&quot;&apos;&quot;,&quot;&quot;)"
                    />-->
					  <xsl:value-of
						select="translate($fontFamily,$apos,'')"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="$fontFamily"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:attribute>
            </name>
          </xsl:when>
          <xsl:when test="$mode = 'default' ">
            <name>
              <xsl:attribute name="val">
                <xsl:text>Arial</xsl:text>
              </xsl:attribute>
            </name>
          </xsl:when>
        </xsl:choose>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="number:text" mode="fonts"/>
  <xsl:template match="text()" mode="fonts"/>
  <xsl:template match="number:text" mode="cellFormats"/>

  <xsl:template name="InsertMultilineCellFormats">

    <xsl:variable name="numStyleCount">
      <xsl:value-of
        select="count(document('content.xml')/office:document-content/office:automatic-styles/number:number-style)"
      />
    </xsl:variable>

    <xsl:variable name="styleNumStyleCount">
      <xsl:value-of
        select="count(office:styles/number:number-style)"
      />
    </xsl:variable>

    <xsl:variable name="percentStyleCount">
      <xsl:value-of
        select="count(document('content.xml')/office:document-content/office:automatic-styles/number:percentage-style)"
      />
    </xsl:variable>

    <xsl:variable name="stylePercentStyleCount">
      <xsl:value-of
        select="count(office:styles/number:percentage-style)"
      />
    </xsl:variable>

    <xsl:variable name="currencyStyleCount">
      <xsl:value-of
        select="count(document('content.xml')/office:document-content/office:automatic-styles/number:currency-style)"
      />
    </xsl:variable>

    <xsl:variable name="styleCurrencyStyleCount">
      <xsl:value-of
        select="count(office:styles/number:currency-style)"
      />
    </xsl:variable>

    <xsl:variable name="dateStyleCount">
      <xsl:value-of
        select="count(document('content.xml')/office:document-content/office:automatic-styles/number:date-style)"
      />
    </xsl:variable>

    <xsl:variable name="styleDateStyleCount">
      <xsl:value-of
        select="count(office:styles/number:date-style)"
      />
    </xsl:variable>

    <xsl:variable name="timeStyleCount">
      <xsl:value-of
        select="count(document('content.xml')/office:document-content/office:automatic-styles/number:time-style)"
      />
    </xsl:variable>

    <xsl:variable name="contentFontsCount">
      <xsl:value-of
        select="count(document('content.xml')/office:document-content/office:automatic-styles/style:style[@style:family='table-cell' or @style:family='text']/style:text-properties)"
      />
    </xsl:variable>

    <xsl:variable name="styleFontsCount">
      <xsl:value-of
        select="count(office:styles/style:style[@style:family='table-cell' or @style:family='text']/style:text-properties)"
      />
    </xsl:variable>

    <xsl:variable name="contentFillCount">
      <xsl:value-of
        select="count(document('content.xml')/office:document-content/office:automatic-styles/style:style[@style:family='table-cell']/style:table-cell-properties)"
      />
    </xsl:variable>

    <xsl:for-each
      select="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:table">

      <!-- string with listed columns and their styles -->
      <xsl:variable name="ColumnTable">
        <xsl:apply-templates select="descendant::table:table-column[1]" mode="tag">
          <xsl:with-param name="colNumber">1</xsl:with-param>
        </xsl:apply-templates>
      </xsl:variable>

      <xsl:variable name="style" select="key('style',@table:style-name)"/>

      <xsl:for-each select="descendant::table:table-cell[text:p[2]]">

        <xsl:variable name="formatNumber">
          <xsl:choose>
            <xsl:when test="key('style',@table:style-name)">
              <xsl:for-each select="key('style',@table:style-name)">
                <xsl:value-of
                  select="count(preceding-sibling::style:style[@style:family='table-cell']) + 1"/>
              </xsl:for-each>
            </xsl:when>
            <xsl:otherwise>
              <xsl:variable name="TableStyleName">
                <xsl:value-of select="@table:style-name"/>
              </xsl:variable>
              <xsl:for-each
                select="office:styles/style:style[@style:name=$TableStyleName]">
                <xsl:value-of
                  select="count(preceding-sibling::style:style[@style:family='table-cell']) + 1"/>
              </xsl:for-each>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>

        <!-- number format id -->
        <xsl:variable name="numFmtId">
          <xsl:call-template name="GetNumFmtId">
            <xsl:with-param name="numStyle">
              <xsl:value-of select="$style"/>
            </xsl:with-param>
            <xsl:with-param name="numStyleCount" select="$numStyleCount"/>
            <xsl:with-param name="styleNumStyleCount" select="$styleNumStyleCount"/>
            <xsl:with-param name="percentStyleCount" select="$percentStyleCount"/>
            <xsl:with-param name="stylePercentStyleCount" select="$stylePercentStyleCount"/>
            <xsl:with-param name="currencyStyleCount" select="$currencyStyleCount"/>
            <xsl:with-param name="styleCurrencyStyleCount" select="$styleCurrencyStyleCount"/>
            <xsl:with-param name="dateStyleCount" select="$dateStyleCount"/>
            <xsl:with-param name="styleDateStyleCount" select="$styleDateStyleCount"/>
            <xsl:with-param name="timeStyleCount" select="$timeStyleCount"/>
          </xsl:call-template>
        </xsl:variable>

        <xf numFmtId="{$numFmtId}" fontId="0" fillId="0" borderId="0" xfId="0">

          <xsl:choose>
            <!-- when style is set for cell -->
            <xsl:when test="$formatNumber != '' ">

              <xsl:attribute name="applyFont">
                <xsl:text>1</xsl:text>
              </xsl:attribute>

              <xsl:attribute name="applyAlignment">
                <xsl:text>1</xsl:text>
              </xsl:attribute>

              <xsl:attribute name="xfId">
                <xsl:choose>
                  <xsl:when test="key('style',@table:style-name)">
                    <xsl:for-each select="key('style',@table:style-name)">
                      <xsl:call-template name="XfId"/>
                    </xsl:for-each>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:variable name="cellStyleName">
                      <xsl:value-of select="@table:style-name"/>
                    </xsl:variable>
                    <xsl:for-each
                      select="office:styles/style:style[@style:name=$cellStyleName]">
                      <xsl:call-template name="XfId"/>
                    </xsl:for-each>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:attribute>

              <xsl:choose>
                <!-- when style is in content.xml -->
                <xsl:when test="key('style',@table:style-name)">
                  <xsl:for-each
                    select="document('content.xml')/office:document-content/office:automatic-styles/style:style[@style:family='table-cell'][position() = $formatNumber]">
                    <xsl:call-template name="SetFormatProperties">
                      <xsl:with-param name="multiline">
                        <xsl:text>true</xsl:text>
                      </xsl:with-param>
                      <xsl:with-param name="contentFontsCount">
                        <xsl:value-of select="$contentFontsCount"/>
                      </xsl:with-param>
                      <xsl:with-param name="styleFontsCount">
                        <xsl:value-of select="$styleFontsCount"/>
                      </xsl:with-param>
                      <xsl:with-param name="contentFillCount">
                        <xsl:value-of select="$contentFillCount"/>
                      </xsl:with-param>
                    </xsl:call-template>
                  </xsl:for-each>
                </xsl:when>
                <!-- when style is in styles.xml -->
                <xsl:otherwise>
                  <xsl:for-each
                    select="office:styles/style:style[@style:family='table-cell'][position() = $formatNumber]">
                    <xsl:call-template name="SetFormatProperties">
                      <xsl:with-param name="multiline">
                        <xsl:text>true</xsl:text>
                      </xsl:with-param>
                      <xsl:with-param name="FileName" select=" 'styles' "/>
                      <xsl:with-param name="contentFontsCount">
                        <xsl:value-of select="$contentFontsCount"/>
                      </xsl:with-param>
                      <xsl:with-param name="styleFontsCount">
                        <xsl:value-of select="$styleFontsCount"/>
                      </xsl:with-param>
                      <xsl:with-param name="contentFillCount">
                        <xsl:value-of select="$contentFillCount"/>
                      </xsl:with-param>
                    </xsl:call-template>
                  </xsl:for-each>
                </xsl:otherwise>
              </xsl:choose>

            </xsl:when>

            <!-- when style is set for column or there is none -->
            <xsl:otherwise>

              <!-- sequential number of this table:table-cell tag -->
              <xsl:variable name="position">
                <xsl:value-of select="count(preceding-sibling::table:table-cell) + 1"/>
              </xsl:variable>

              <!-- real column number -->
              <xsl:variable name="colNum">
                <xsl:for-each select="parent::node()/table:table-cell[1]">
                  <xsl:call-template name="GetColNumber">
                    <xsl:with-param name="position">
                      <xsl:value-of select="$position"/>
                    </xsl:with-param>
                  </xsl:call-template>
                </xsl:for-each>
              </xsl:variable>

              <!-- name of the style set for this column -->
              <xsl:variable name="columnCellStyle">
                <xsl:call-template name="GetColumnCellStyle">
                  <xsl:with-param name="colNum">
                    <xsl:value-of select="$colNum"/>
                  </xsl:with-param>
                  <xsl:with-param name="TableColumnTagNum">
                    <xsl:value-of select="$ColumnTable"/>
                  </xsl:with-param>
                </xsl:call-template>
              </xsl:variable>

              <xsl:attribute name="applyAlignment">
                <xsl:text>1</xsl:text>
              </xsl:attribute>

              <xsl:choose>
                <!-- when style was set for column -->
                <xsl:when test="$columnCellStyle != '' ">
                  <xsl:choose>
                    <!-- when style is in content.xml -->
                    <xsl:when test="key('style',$columnCellStyle)">
                      <xsl:for-each select="key('style',$columnCellStyle)">
                        <xsl:call-template name="SetFormatProperties">
                          <xsl:with-param name="multiline">
                            <xsl:text>true</xsl:text>
                          </xsl:with-param>
                        </xsl:call-template>
                      </xsl:for-each>
                    </xsl:when>
                    <!-- when style is in styles.xml -->
                    <xsl:otherwise>
                      <xsl:for-each
                        select="office:styles/style:style[@style:name=$columnCellStyle]">


                        <xsl:attribute name="xfId">
                          <xsl:value-of
                            select="count(preceding-sibling::style:style[@style:family='table-cell']) + 1"
                          />
                        </xsl:attribute>


                        <xsl:call-template name="SetFormatProperties">
                          <xsl:with-param name="multiline">
                            <xsl:text>true</xsl:text>
                          </xsl:with-param>
                        </xsl:call-template>


                      </xsl:for-each>
                    </xsl:otherwise>
                  </xsl:choose>

                </xsl:when>
                <!-- when there was no style and there is hyperlink in merged cell -->
                <xsl:when test="descendant::text:a">

                  <xsl:attribute name="xfId">
                    <xsl:value-of
                      select="count(office:styles/style:style[@style:family = 'table-cell']) + 1"
                    />
                  </xsl:attribute>

                  <xsl:variable name="contentFontCount">
                    <xsl:value-of
                      select="count(document('content.xml')/office:document-content/office:automatic-styles/style:style/style:text-properties[parent::node()[@style:family='table-cell' or @style:family='text']])"
                    />
                  </xsl:variable>

                  <xsl:variable name="styleFontCount">
                    <xsl:value-of
                      select="count(office:styles/style:style/style:text-properties[parent::node()[@style:family='table-cell' or @style:family='text']])"
                    />
                  </xsl:variable>

                  <xsl:attribute name="fontId">
                    <xsl:choose>
                      <xsl:when
                        test="office:styles/style:style[contains(@style:name, 'Hyperlink') and @style:family = 'table-cell']">
                        <xsl:value-of select="$contentFontCount + $styleFontCount + 2"/>
                      </xsl:when>
                      <xsl:otherwise>
                        <xsl:value-of select="$contentFontCount + $styleFontCount "/>
                      </xsl:otherwise>
                    </xsl:choose>
                  </xsl:attribute>

                  <alignment wrapText="1"/>
                </xsl:when>
                <xsl:otherwise>
                  <alignment wrapText="1"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:otherwise>
          </xsl:choose>
        </xf>
      </xsl:for-each>
    </xsl:for-each>
  </xsl:template>


  <!-- template to get horizontal align inherited from parent style-->

  <xsl:template name="InheritHorizontalAlign">
    <xsl:choose>
      <xsl:when test="style:paragraph-properties/@fo:text-align">
        <xsl:value-of select="style:paragraph-properties/@fo:text-align"/>
      </xsl:when>
      <xsl:when test="@style:parent-style-name != '' and @style:parent-style-name != 'Default'">
        <xsl:variable name="StyleParentStyleName">
          <xsl:value-of select="@style:parent-style-name"/>
        </xsl:variable>
        <xsl:choose>
          <xsl:when
            test="key('style',@style:parent-style-name)/style:paragraph-properties/@fo:text-align">
            <xsl:for-each select="key('style',$StyleParentStyleName)">
              <xsl:call-template name="InheritHorizontalAlign"/>
            </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
            <xsl:for-each
              select="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name = $StyleParentStyleName]">
              <xsl:call-template name="InheritHorizontalAlign"/>
            </xsl:for-each>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- template to get repeat content inherited from parent style -->

  <xsl:template name="InheritRepeatContent">
    <xsl:choose>
      <xsl:when test="style:table-cell-properties/@style:repeat-content">
        <xsl:value-of select="style:table-cell-properties/@style:repeat-content"/>
      </xsl:when>
      <xsl:when test="@style:parent-style-name != '' and @style:parent-style-name != 'Default'">
        <xsl:variable name="StyleParentStyleName">
          <xsl:value-of select="@style:parent-style-name"/>
        </xsl:variable>
        <xsl:choose>
          <xsl:when
            test="key('style',@style:parent-style-name)/style:table-cell-properties/@style:repeat-content">
            <xsl:for-each select="key('style',$StyleParentStyleName)">
              <xsl:call-template name="InheritRepeatContent"/>
            </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
            <xsl:for-each
              select="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name = $StyleParentStyleName]">
              <xsl:call-template name="InheritRepeatContent"/>
            </xsl:for-each>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- template to get vertical align inherited from parent style -->

  <xsl:template name="InheritVerticalAlign">
    <xsl:choose>
      <xsl:when test="style:table-cell-properties/@style:vertical-align">
        <xsl:value-of select="style:table-cell-properties/@style:vertical-align"/>
      </xsl:when>
      <xsl:when test="@style:parent-style-name != '' and @style:parent-style-name != 'Default'">
        <xsl:variable name="StyleParentStyleName">
          <xsl:value-of select="@style:parent-style-name"/>
        </xsl:variable>
        <xsl:choose>
          <xsl:when
            test="key('style',@style:parent-style-name)/style:table-cell-properties/@style:vertical-align">
            <xsl:for-each select="key('style',$StyleParentStyleName)">
              <xsl:call-template name="InheritVerticalAlign"/>
            </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
            <xsl:for-each
              select="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name = $StyleParentStyleName]">
              <xsl:call-template name="InheritVerticalAlign"/>
            </xsl:for-each>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- template to get rotation inherited from parent style -->

  <xsl:template name="InheritRotation">
    <xsl:choose>
      <xsl:when test="style:table-cell-properties/@style:rotation-angle">
        <xsl:value-of select="style:table-cell-properties/@style:rotation-angle"/>
      </xsl:when>
      <xsl:when test="@style:parent-style-name != '' and @style:parent-style-name != 'Default'">
        <xsl:variable name="StyleParentStyleName">
          <xsl:value-of select="@style:parent-style-name"/>
        </xsl:variable>
        <xsl:choose>
          <xsl:when
            test="key('style',@style:parent-style-name)/style:table-cell-properties/@style:rotation-angle">
            <xsl:for-each select="key('style',$StyleParentStyleName)">
              <xsl:call-template name="InheritRotation"/>
            </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
            <xsl:for-each
              select="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name = $StyleParentStyleName]">
              <xsl:call-template name="InheritRotation"/>
            </xsl:for-each>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- template to get vertical text inherited from parent style -->

  <xsl:template name="InheritVerticalText">
    <xsl:choose>
      <xsl:when test="style:table-cell-properties/@style:direction">
        <xsl:value-of select="style:table-cell-properties/@style:direction"/>
      </xsl:when>
      <xsl:when test="@style:parent-style-name != '' and @style:parent-style-name != 'Default'">
        <xsl:variable name="StyleParentStyleName">
          <xsl:value-of select="@style:parent-style-name"/>
        </xsl:variable>
        <xsl:choose>
          <xsl:when
            test="key('style',@style:parent-style-name)/style:table-cell-properties/@style:direction">
            <xsl:for-each select="key('style',$StyleParentStyleName)">
              <xsl:call-template name="InheritVerticalText"/>
            </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
            <xsl:for-each
              select="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name = $StyleParentStyleName]">
              <xsl:call-template name="InheritVerticalText"/>
            </xsl:for-each>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- template to get word wrap inherited from parent style -->

  <xsl:template name="InheritWordWrap">
    <xsl:choose>
      <xsl:when test="style:table-cell-properties/@fo:wrap-option">
        <xsl:value-of select="style:table-cell-properties/@fo:wrap-option"/>
      </xsl:when>
      <xsl:when test="@style:parent-style-name != '' and @style:parent-style-name != 'Default'">
        <xsl:variable name="StyleParentStyleName">
          <xsl:value-of select="@style:parent-style-name"/>
        </xsl:variable>
        <xsl:choose>
          <xsl:when
            test="key('style',@style:parent-style-name)/style:table-cell-properties/@fo:wrap-option">
            <xsl:for-each select="key('style',$StyleParentStyleName)">
              <xsl:call-template name="InheritWordWrap"/>
            </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
            <xsl:for-each
              select="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name = $StyleParentStyleName]">
              <xsl:call-template name="InheritWordWrap"/>
            </xsl:for-each>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- template to get font weight inherited from parent style -->

  <xsl:template name="InheritFontWeight">
    <xsl:choose>
      <xsl:when test="style:text-properties/@fo:font-weight">
        <xsl:value-of select="style:text-properties/@fo:font-weight"/>
      </xsl:when>
      <xsl:when test="@style:parent-style-name != '' and @style:parent-style-name != 'Default'">
        <xsl:variable name="StyleParentStyleName">
          <xsl:value-of select="@style:parent-style-name"/>
        </xsl:variable>
        <xsl:choose>
          <xsl:when test="key('style',parent::node()/@style:parent-style-name)">
            <xsl:for-each select="key('style',$StyleParentStyleName)">
              <xsl:call-template name="InheritFontWeight"/>
            </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
            <xsl:for-each
              select="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name = $StyleParentStyleName]">
              <xsl:call-template name="InheritFontWeight"/>
            </xsl:for-each>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- template to get font weight complex inherited from parent style -->

  <xsl:template name="InheritFontWeightComplex">
    <xsl:choose>
      <xsl:when test="style:text-properties/@style:font-weight-complex">
        <xsl:value-of select="style:text-properties/@style:font-weight-complex"/>
      </xsl:when>
      <xsl:when test="@style:parent-style-name != '' and @style:parent-style-name != 'Default'">
        <xsl:variable name="StyleParentStyleName">
          <xsl:value-of select="@style:parent-style-name"/>
        </xsl:variable>
        <xsl:choose>
          <xsl:when test="key('style',parent::node()/@style:parent-style-name)">
            <xsl:for-each select="key('style',$StyleParentStyleName)">
              <xsl:call-template name="InheritFontWeightComplex"/>
            </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
            <xsl:for-each
              select="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name = $StyleParentStyleName]">
              <xsl:call-template name="InheritFontWeightComplex"/>
            </xsl:for-each>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- template to get font size inherited from parent style -->

  <xsl:template name="InheritFontStyle">
    <xsl:choose>
      <xsl:when test="style:text-properties/@fo:font-style">
        <xsl:value-of select="style:text-properties/@fo:font-style"/>
      </xsl:when>
      <xsl:when test="@style:parent-style-name != '' and @style:parent-style-name != 'Default'">
        <xsl:variable name="StyleParentStyleName">
          <xsl:value-of select="@style:parent-style-name"/>
        </xsl:variable>
        <xsl:choose>
          <xsl:when test="key('style',parent::node()/@style:parent-style-name)">
            <xsl:for-each select="key('style',$StyleParentStyleName)">
              <xsl:call-template name="InheritFontStyle"/>
            </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
            <xsl:for-each
              select="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name = $StyleParentStyleName]">
              <xsl:call-template name="InheritFontStyle"/>
            </xsl:for-each>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- template to get underline style inherited from parent style -->

  <xsl:template name="InheritFontUnderlineStyle">
    <xsl:choose>
      <xsl:when test="style:text-properties/@style:text-underline-style">
        <xsl:value-of select="style:text-properties/@style:text-underline-style"/>
      </xsl:when>
      <xsl:when test="@style:parent-style-name != '' and @style:parent-style-name != 'Default'">
        <xsl:variable name="StyleParentStyleName">
          <xsl:value-of select="@style:parent-style-name"/>
        </xsl:variable>
        <xsl:choose>
          <xsl:when test="key('style',parent::node()/@style:parent-style-name)">
            <xsl:for-each select="key('style',$StyleParentStyleName)">
              <xsl:call-template name="InheritFontUnderlineStyle"/>
            </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
            <xsl:for-each
              select="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name = $StyleParentStyleName]">
              <xsl:call-template name="InheritFontUnderlineStyle"/>
            </xsl:for-each>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- template to get underline type inherited from parent style -->

  <xsl:template name="InheritFontUnderlineType">
    <xsl:choose>
      <xsl:when test="style:text-properties/@style:text-underline-type">
        <xsl:value-of select="style:text-properties/@style:text-underline-type"/>
      </xsl:when>
      <xsl:when test="@style:parent-style-name != '' and @style:parent-style-name != 'Default'">
        <xsl:variable name="StyleParentStyleName">
          <xsl:value-of select="@style:parent-style-name"/>
        </xsl:variable>
        <xsl:choose>
          <xsl:when test="key('style',parent::node()/@style:parent-style-name)">
            <xsl:for-each select="key('style',$StyleParentStyleName)">
              <xsl:call-template name="InheritFontUnderlineType"/>
            </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
            <xsl:for-each
              select="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name = $StyleParentStyleName]">
              <xsl:call-template name="InheritFontUnderlineType"/>
            </xsl:for-each>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- template to get font size inherited from parent style -->

  <xsl:template name="InheritFontSize">
    <xsl:choose>
      <xsl:when test="style:text-properties/@fo:font-size">
        <xsl:value-of select="style:text-properties/@fo:font-size"/>
      </xsl:when>
      <xsl:when test="@style:parent-style-name != '' and @style:parent-style-name != 'Default'">
        <xsl:variable name="StyleParentStyleName">
          <xsl:value-of select="@style:parent-style-name"/>
        </xsl:variable>
        <xsl:choose>
          <xsl:when test="key('style',parent::node()/@style:parent-style-name)">
            <xsl:for-each select="key('style',$StyleParentStyleName)">
              <xsl:call-template name="InheritFontSize"/>
            </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
            <xsl:for-each
              select="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name = $StyleParentStyleName]">
              <xsl:call-template name="InheritFontSize"/>
            </xsl:for-each>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- template to get complex font size inherited from parent style -->

  <xsl:template name="InheritFontSizeComplex">
    <xsl:choose>
      <xsl:when test="style:text-properties/@style:font-size-complex">
        <xsl:value-of select="style:text-properties/@style:font-size-complex"/>
      </xsl:when>
      <xsl:when test="@style:parent-style-name != '' and @style:parent-style-name != 'Default'">
        <xsl:variable name="StyleParentStyleName">
          <xsl:value-of select="@style:parent-style-name"/>
        </xsl:variable>
        <xsl:choose>
          <xsl:when test="key('style',parent::node()/@style:parent-style-name)">
            <xsl:for-each select="key('style',$StyleParentStyleName)">
              <xsl:call-template name="InheritFontSizeComplex"/>
            </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
            <xsl:for-each
              select="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name = $StyleParentStyleName]">
              <xsl:call-template name="InheritFontSizeComplex"/>
            </xsl:for-each>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- template to get asian font size inherited from parent style -->

  <xsl:template name="InheritFontSizeAsian">
    <xsl:choose>
      <xsl:when test="style:text-properties/@style:font-size-asian">
        <xsl:value-of select="style:text-properties/@style:font-size-asian"/>
      </xsl:when>
      <xsl:when test="@style:parent-style-name != '' and @style:parent-style-name != 'Default'">
        <xsl:variable name="StyleParentStyleName">
          <xsl:value-of select="@style:parent-style-name"/>
        </xsl:variable>
        <xsl:choose>
          <xsl:when test="key('style',parent::node()/@style:parent-style-name)">
            <xsl:for-each select="key('style',$StyleParentStyleName)">
              <xsl:call-template name="InheritFontSizeAsian"/>
            </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
            <xsl:for-each
              select="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name = $StyleParentStyleName]">
              <xsl:call-template name="InheritFontSizeAsian"/>
            </xsl:for-each>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- template to get font strikethrough inherited from parent style -->

  <xsl:template name="InheritFontStrikethrough">
    <xsl:choose>
      <xsl:when test="style:text-properties/@style:text-line-through-style">
        <xsl:value-of select="style:text-properties/@style:text-line-through-style"/>
      </xsl:when>
      <xsl:when test="@style:parent-style-name != '' and @style:parent-style-name != 'Default'">
        <xsl:variable name="StyleParentStyleName">
          <xsl:value-of select="@style:parent-style-name"/>
        </xsl:variable>
        <xsl:choose>
          <xsl:when test="key('style',parent::node()/@style:parent-style-name)">
            <xsl:for-each select="key('style',$StyleParentStyleName)">
              <xsl:call-template name="InheritFontStrikethrough"/>
            </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
            <xsl:for-each
              select="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name = $StyleParentStyleName]">
              <xsl:call-template name="InheritFontStrikethrough"/>
            </xsl:for-each>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- template to get font color inherited from parent style -->

  <xsl:template name="InheritFontColor">
    <xsl:choose>
      <xsl:when test="style:text-properties/@fo:color">
        <xsl:value-of select="style:text-properties/@fo:color"/>
      </xsl:when>
      <xsl:when test="@style:parent-style-name != '' and @style:parent-style-name != 'Default'">
        <xsl:variable name="StyleParentStyleName">
          <xsl:value-of select="@style:parent-style-name"/>
        </xsl:variable>
        <xsl:choose>
          <xsl:when test="key('style',parent::node()/@style:parent-style-name)">
            <xsl:for-each select="key('style',$StyleParentStyleName)">
              <xsl:call-template name="InheritFontColor"/>
            </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
            <xsl:for-each
              select="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name = $StyleParentStyleName]">
              <xsl:call-template name="InheritFontColor"/>
            </xsl:for-each>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- template to get font family inherited from parent style -->

  <xsl:template name="InheritFontFamily">
    <xsl:choose>
      <xsl:when test="key('font',style:text-properties/@style:font-name)/@svg:font-family">
        <xsl:value-of select="key('font',style:text-properties/@style:font-name)/@svg:font-family"/>
      </xsl:when>
      <xsl:when test="@style:parent-style-name != '' and @style:parent-style-name != 'Default'">
        <xsl:variable name="StyleParentStyleName">
          <xsl:value-of select="@style:parent-style-name"/>
        </xsl:variable>
        <xsl:choose>
          <xsl:when test="key('style',parent::node()/@style:parent-style-name)">
            <xsl:for-each select="key('style',$StyleParentStyleName)">
              <xsl:call-template name="InheritFontFamily"/>
            </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
            <xsl:for-each
              select="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name = $StyleParentStyleName]">
              <xsl:call-template name="InheritFontFamily"/>
            </xsl:for-each>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
    </xsl:choose>
  </xsl:template>


  <xsl:template name="InsertHyperlinksProperties">
    <xsl:if
      test="document('content.xml')/descendant::text:a[not(ancestor::draw:custom-shape) and not(ancestor::office:annotation)]">

      <xsl:variable name="xfId">
        <xsl:value-of
          select="count(document('styles.xml')/office:document-styles/office:styles/style:style[@style:family = 'table-cell']) + 1"
        />
      </xsl:variable>

      <xsl:variable name="contentFontCount">
        <xsl:value-of
          select="count(document('content.xml')/office:document-content/office:automatic-styles/style:style/style:text-properties[parent::node()[@style:family='table-cell' or @style:family='text']])"
        />
      </xsl:variable>

      <xsl:variable name="styleFontCount">
        <xsl:value-of
          select="count(document('styles.xml')/office:document-styles/office:styles/style:style/style:text-properties[parent::node()[@style:family='table-cell' or @style:family='text']])"
        />
      </xsl:variable>


      <xsl:variable name="fontId">
        <xsl:choose>
          <xsl:when
            test="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name = 'Hyperlink' and @style:family = 'table-cell']">
            <xsl:value-of select="$contentFontCount + $styleFontCount"/>

          </xsl:when>
          <xsl:when
            test="document('styles.xml')/office:document-styles/office:styles/style:style[contains(@style:name, 'Hyperlink') and @style:family = 'table-cell']">
            <xsl:value-of select="$contentFontCount + $styleFontCount + 2"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$contentFontCount + $styleFontCount"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>

      <xf numFmtId="0" fillId="0" borderId="0" xfId="{$xfId}" fontId="{$fontId}"/>

    </xsl:if>
  </xsl:template>

  <!-- template to get number format inherited from parent style -->

  <xsl:template name="NumFmtId">
    <xsl:param name="FileName"/>
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
      <xsl:when test="@style:data-style-name and @style:data-style-name != 'N0' ">
        <xsl:call-template name="GetNumFmtId">
          <xsl:with-param name="numStyle">
            <xsl:value-of select="@style:data-style-name"/>
          </xsl:with-param>
          <xsl:with-param name="numStyleCount" select="$numStyleCount"/>
          <xsl:with-param name="styleNumStyleCount" select="$styleNumStyleCount"/>
          <xsl:with-param name="percentStyleCount" select="$percentStyleCount"/>
          <xsl:with-param name="stylePercentStyleCount" select="$stylePercentStyleCount"/>
          <xsl:with-param name="currencyStyleCount" select="$currencyStyleCount"/>
          <xsl:with-param name="styleCurrencyStyleCount" select="$styleCurrencyStyleCount"/>
          <xsl:with-param name="dateStyleCount" select="$dateStyleCount"/>
          <xsl:with-param name="styleDateStyleCount" select="$styleDateStyleCount"/>
          <xsl:with-param name="timeStyleCount" select="$timeStyleCount"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="@style:parent-style-name != '' and @style:parent-style-name != 'Default'">
        <xsl:variable name="StyleParentStyleName">
          <xsl:value-of select="@style:parent-style-name"/>
        </xsl:variable>
        <xsl:choose>
          <xsl:when test="key('style',@style:parent-style-name)">
            <xsl:for-each select="key('style',$StyleParentStyleName)">
              <xsl:call-template name="NumFmtId">
                <xsl:with-param name="FileName" select="$FileName"/>
                <xsl:with-param name="numStyleCount" select="$numStyleCount"/>
                <xsl:with-param name="styleNumStyleCount" select="$styleNumStyleCount"/>
                <xsl:with-param name="percentStyleCount" select="$percentStyleCount"/>
                <xsl:with-param name="stylePercentStyleCount" select="$stylePercentStyleCount"/>
                <xsl:with-param name="currencyStyleCount" select="$currencyStyleCount"/>
                <xsl:with-param name="styleCurrencyStyleCount" select="$styleCurrencyStyleCount"/>
                <xsl:with-param name="dateStyleCount" select="$dateStyleCount"/>
                <xsl:with-param name="styleDateStyleCount" select="$styleDateStyleCount"/>
                <xsl:with-param name="timeStyleCount" select="$timeStyleCount"/>
              </xsl:call-template>
            </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
            <xsl:for-each
              select="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name = $StyleParentStyleName]">
              <xsl:call-template name="NumFmtId">
                <xsl:with-param name="FileName">
                  <xsl:text>styles</xsl:text>
                </xsl:with-param>
                <xsl:with-param name="numStyleCount" select="$numStyleCount"/>
                <xsl:with-param name="styleNumStyleCount" select="$styleNumStyleCount"/>
                <xsl:with-param name="percentStyleCount" select="$percentStyleCount"/>
                <xsl:with-param name="stylePercentStyleCount" select="$stylePercentStyleCount"/>
                <xsl:with-param name="currencyStyleCount" select="$currencyStyleCount"/>
                <xsl:with-param name="styleCurrencyStyleCount" select="$styleCurrencyStyleCount"/>
                <xsl:with-param name="dateStyleCount" select="$dateStyleCount"/>
                <xsl:with-param name="styleDateStyleCount" select="$styleDateStyleCount"/>
                <xsl:with-param name="timeStyleCount" select="$timeStyleCount"/>
              </xsl:call-template>
            </xsl:for-each>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:otherwise>
        <xsl:text>0</xsl:text>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="InsertHyperlinksStyleProperties">
    <xsl:param name="numStyleCount"/>
    <xsl:param name="styleNumStyleCount"/>
    <xsl:param name="percentStyleCount"/>
    <xsl:param name="stylePercentStyleCount"/>
    <xsl:param name="currencyStyleCount"/>
    <xsl:param name="styleCurrencyStyleCount"/>
    <xsl:param name="dateStyleCount"/>
    <xsl:param name="styleDateStyleCount"/>
    <xsl:param name="timeStyleCount"/>
    <xsl:param name="FileName"/>
    <xsl:param name="AtributeName"/>
    <xsl:param name="contentFontsCount"/>
    <xsl:param name="styleFontsCount"/>
    <xsl:param name="contentFillCount"/>
    <xsl:param name="postStyleName" select="false"/>

    <!--RefNo-1<xsl:for-each
      select="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:table/table:table-row/table:table-cell[descendant::text:a]">-->
    <xsl:for-each
      select="document('content.xml')">
		  <xsl:for-each
			select="office:document-content/office:body/office:spreadsheet/table:table/descendant::table:table-row/table:table-cell[descendant::text:a]">

      <!--RefNo-2:variable to chk multiline-->
      <xsl:variable name="hypMultiLine">
         <xsl:if test="text:p[2]">
           <xsl:text>true</xsl:text>
         </xsl:if>        
      </xsl:variable>
      <xsl:variable name="StyleName">
        <xsl:value-of select="@table:style-name"/>
      </xsl:variable>

      <xsl:choose>
        <xsl:when
					test="/office:document-content/office:automatic-styles/style:style[@style:name=$StyleName]">

          <xsl:apply-templates
						select="/office:document-content/office:automatic-styles/style:style[@style:name=$StyleName]"
            mode="cellFormats">
            <xsl:with-param name="numStyleCount">
              <xsl:value-of select="$numStyleCount"/>
            </xsl:with-param>
            <xsl:with-param name="styleNumStyleCount">
              <xsl:value-of select="$styleNumStyleCount"/>
            </xsl:with-param>
            <xsl:with-param name="percentStyleCount">
              <xsl:value-of select="$percentStyleCount"/>
            </xsl:with-param>
            <xsl:with-param name="stylePercentStyleCount">
              <xsl:value-of select="$stylePercentStyleCount"/>
            </xsl:with-param>
            <xsl:with-param name="currencyStyleCount">
              <xsl:value-of select="$currencyStyleCount"/>
            </xsl:with-param>
            <xsl:with-param name="styleCurrencyStyleCount">
              <xsl:value-of select="$styleCurrencyStyleCount"/>
            </xsl:with-param>
            <xsl:with-param name="dateStyleCount">
              <xsl:value-of select="$dateStyleCount"/>
            </xsl:with-param>
            <xsl:with-param name="styleDateStyleCount">
              <xsl:value-of select="$styleDateStyleCount"/>
            </xsl:with-param>
            <xsl:with-param name="timeStyleCount">
              <xsl:value-of select="$timeStyleCount"/>
            </xsl:with-param>
            <xsl:with-param name="contentFontsCount">
              <xsl:value-of select="$contentFontsCount"/>
            </xsl:with-param>
            <xsl:with-param name="styleFontsCount">
              <xsl:value-of select="$styleFontsCount"/>
            </xsl:with-param>
            <xsl:with-param name="contentFillCount">
              <xsl:value-of select="$contentFillCount"/>
            </xsl:with-param>
            <xsl:with-param name="postStyleName">
              <xsl:text>true</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="hyperlink">
              <xsl:text>true</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="hyperlinkId">
              <xsl:number count="table:table-cell[descendant::text:a]" level="any"/>
            </xsl:with-param>
            <!--refNo-2-->
            <xsl:with-param name="hypMultiLine">
              <xsl:value-of select="$hypMultiLine"/>
            </xsl:with-param>
          </xsl:apply-templates>

        </xsl:when>
        <xsl:when
          test="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name=$StyleName]">

          <xsl:apply-templates
            select="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name=$StyleName]"
            mode="cellFormats">
            <xsl:with-param name="numStyleCount">
              <xsl:value-of select="$numStyleCount"/>
            </xsl:with-param>
            <xsl:with-param name="styleNumStyleCount">
              <xsl:value-of select="$styleNumStyleCount"/>
            </xsl:with-param>
            <xsl:with-param name="percentStyleCount">
              <xsl:value-of select="$percentStyleCount"/>
            </xsl:with-param>
            <xsl:with-param name="stylePercentStyleCount">
              <xsl:value-of select="$stylePercentStyleCount"/>
            </xsl:with-param>
            <xsl:with-param name="currencyStyleCount">
              <xsl:value-of select="$currencyStyleCount"/>
            </xsl:with-param>
            <xsl:with-param name="styleCurrencyStyleCount">
              <xsl:value-of select="$styleCurrencyStyleCount"/>
            </xsl:with-param>
            <xsl:with-param name="dateStyleCount">
              <xsl:value-of select="$dateStyleCount"/>
            </xsl:with-param>
            <xsl:with-param name="styleDateStyleCount">
              <xsl:value-of select="$styleDateStyleCount"/>
            </xsl:with-param>
            <xsl:with-param name="timeStyleCount">
              <xsl:value-of select="$timeStyleCount"/>
            </xsl:with-param>
            <xsl:with-param name="contentFontsCount">
              <xsl:value-of select="$contentFontsCount"/>
            </xsl:with-param>
            <xsl:with-param name="styleFontsCount">
              <xsl:value-of select="$styleFontsCount"/>
            </xsl:with-param>
            <xsl:with-param name="contentFillCount">
              <xsl:value-of select="$contentFillCount"/>
            </xsl:with-param>
            <xsl:with-param name="postStyleName">
              <xsl:text>true</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="hyperlink">
              <xsl:text>true</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="hyperlinkId">
              <xsl:number count="table:table-cell[descendant::text:a]" level="any"/>
            </xsl:with-param>
            <!--refNo-2-->
            <xsl:with-param name="hypMultiLine">
              <xsl:value-of select="$hypMultiLine"/>
            </xsl:with-param>
          </xsl:apply-templates>

        </xsl:when>

        <xsl:otherwise>
          <xsl:apply-templates
            select="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name='Default']"
            mode="cellFormats">
            <xsl:with-param name="numStyleCount">
              <xsl:value-of select="$numStyleCount"/>
            </xsl:with-param>
            <xsl:with-param name="styleNumStyleCount">
              <xsl:value-of select="$styleNumStyleCount"/>
            </xsl:with-param>
            <xsl:with-param name="percentStyleCount">
              <xsl:value-of select="$percentStyleCount"/>
            </xsl:with-param>
            <xsl:with-param name="stylePercentStyleCount">
              <xsl:value-of select="$stylePercentStyleCount"/>
            </xsl:with-param>
            <xsl:with-param name="currencyStyleCount">
              <xsl:value-of select="$currencyStyleCount"/>
            </xsl:with-param>
            <xsl:with-param name="styleCurrencyStyleCount">
              <xsl:value-of select="$styleCurrencyStyleCount"/>
            </xsl:with-param>
            <xsl:with-param name="dateStyleCount">
              <xsl:value-of select="$dateStyleCount"/>
            </xsl:with-param>
            <xsl:with-param name="styleDateStyleCount">
              <xsl:value-of select="$styleDateStyleCount"/>
            </xsl:with-param>
            <xsl:with-param name="timeStyleCount">
              <xsl:value-of select="$timeStyleCount"/>
            </xsl:with-param>
            <xsl:with-param name="contentFontsCount">
              <xsl:value-of select="$contentFontsCount"/>
            </xsl:with-param>
            <xsl:with-param name="styleFontsCount">
              <xsl:value-of select="$styleFontsCount"/>
            </xsl:with-param>
            <xsl:with-param name="contentFillCount">
              <xsl:value-of select="$contentFillCount"/>
            </xsl:with-param>
            <xsl:with-param name="postStyleName">
              <xsl:text>true</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="hyperlink">
              <xsl:text>true</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="hyperlinkId">
              <xsl:number count="table:table-cell[descendant::text:a]" level="any"/>
            </xsl:with-param>
            <!--refNo-2-->
            <xsl:with-param name="hypMultiLine">
              <xsl:value-of select="$hypMultiLine"/>
            </xsl:with-param>
          </xsl:apply-templates>
        </xsl:otherwise>

      </xsl:choose>


    </xsl:for-each>
	  </xsl:for-each>
  </xsl:template>

  <xsl:template match="table:table-cell" mode="HyperlinkStyle">
    <xsl:param name="numStyleCount"/>
    <xsl:param name="styleNumStyleCount"/>
    <xsl:param name="percentStyleCount"/>
    <xsl:param name="stylePercentStyleCount"/>
    <xsl:param name="currencyStyleCount"/>
    <xsl:param name="styleCurrencyStyleCount"/>
    <xsl:param name="dateStyleCount"/>
    <xsl:param name="styleDateStyleCount"/>
    <xsl:param name="timeStyleCount"/>
    <xsl:param name="FileName"/>
    <xsl:param name="AtributeName"/>
    <xsl:param name="contentFontsCount"/>
    <xsl:param name="styleFontsCount"/>
    <xsl:param name="contentFillCount"/>
    <xsl:param name="postStyleName" select="false"/>
    <xsl:param name="hyperlinkId"/>


    <xsl:variable name="StyleName">
      <xsl:value-of select="@table:style-name"/>
    </xsl:variable>

    <xsl:apply-templates
      select="document('content.xml')/office:document-content/office:automatic-styles/style:style[@style:name=$StyleName]"
      mode="cellFormats">
      <xsl:with-param name="numStyleCount">
        <xsl:value-of select="$numStyleCount"/>
      </xsl:with-param>
      <xsl:with-param name="styleNumStyleCount">
        <xsl:value-of select="$styleNumStyleCount"/>
      </xsl:with-param>
      <xsl:with-param name="percentStyleCount">
        <xsl:value-of select="$percentStyleCount"/>
      </xsl:with-param>
      <xsl:with-param name="stylePercentStyleCount">
        <xsl:value-of select="$stylePercentStyleCount"/>
      </xsl:with-param>
      <xsl:with-param name="currencyStyleCount">
        <xsl:value-of select="$currencyStyleCount"/>
      </xsl:with-param>
      <xsl:with-param name="styleCurrencyStyleCount">
        <xsl:value-of select="$styleCurrencyStyleCount"/>
      </xsl:with-param>
      <xsl:with-param name="dateStyleCount">
        <xsl:value-of select="$dateStyleCount"/>
      </xsl:with-param>
      <xsl:with-param name="styleDateStyleCount">
        <xsl:value-of select="$styleDateStyleCount"/>
      </xsl:with-param>
      <xsl:with-param name="timeStyleCount">
        <xsl:value-of select="$timeStyleCount"/>
      </xsl:with-param>
      <xsl:with-param name="contentFontsCount">
        <xsl:value-of select="$contentFontsCount"/>
      </xsl:with-param>
      <xsl:with-param name="styleFontsCount">
        <xsl:value-of select="$styleFontsCount"/>
      </xsl:with-param>
      <xsl:with-param name="contentFillCount">
        <xsl:value-of select="$contentFillCount"/>
      </xsl:with-param>
      <xsl:with-param name="postStyleName">
        <xsl:text>true</xsl:text>
      </xsl:with-param>
      <xsl:with-param name="hyperlink">
        <xsl:text>true</xsl:text>
      </xsl:with-param>
    </xsl:apply-templates>

    <xsl:if test="following-sibling::table:table-cell[1]"> next <xsl:apply-templates
        select="following-sibling::table:table-cell[1]" mode="HyperlinkStyle">
        <xsl:with-param name="numStyleCount">
          <xsl:value-of select="$numStyleCount"/>
        </xsl:with-param>
        <xsl:with-param name="styleNumStyleCount">
          <xsl:value-of select="$styleNumStyleCount"/>
        </xsl:with-param>
        <xsl:with-param name="percentStyleCount">
          <xsl:value-of select="$percentStyleCount"/>
        </xsl:with-param>
        <xsl:with-param name="stylePercentStyleCount">
          <xsl:value-of select="$stylePercentStyleCount"/>
        </xsl:with-param>
        <xsl:with-param name="currencyStyleCount">
          <xsl:value-of select="$currencyStyleCount"/>
        </xsl:with-param>
        <xsl:with-param name="styleCurrencyStyleCount">
          <xsl:value-of select="$styleCurrencyStyleCount"/>
        </xsl:with-param>
        <xsl:with-param name="dateStyleCount">
          <xsl:value-of select="$dateStyleCount"/>
        </xsl:with-param>
        <xsl:with-param name="styleDateStyleCount">
          <xsl:value-of select="$styleDateStyleCount"/>
        </xsl:with-param>
        <xsl:with-param name="timeStyleCount">
          <xsl:value-of select="$timeStyleCount"/>
        </xsl:with-param>
        <xsl:with-param name="contentFontsCount">
          <xsl:value-of select="$contentFontsCount"/>
        </xsl:with-param>
        <xsl:with-param name="styleFontsCount">
          <xsl:value-of select="$styleFontsCount"/>
        </xsl:with-param>
        <xsl:with-param name="contentFillCount">
          <xsl:value-of select="$contentFillCount"/>
        </xsl:with-param>
        <xsl:with-param name="postStyleName">
          <xsl:text>true</xsl:text>
        </xsl:with-param>
        <xsl:with-param name="hyperlinkId">
          <xsl:choose>
            <xsl:when test="descendant::text:a">
              <xsl:value-of select="$hyperlinkId + 1"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$hyperlinkId"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:with-param>
      </xsl:apply-templates>
    </xsl:if>

  </xsl:template>

  <xsl:template name="InsertHyperlinkTextStyle">

    <!--RefNo-1<xsl:apply-templates
      select="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:table/table:table-row/table:table-cell[descendant::text:a]"
      mode="HyperlinkText"/>-->
    <xsl:apply-templates
      select="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:table//table:table-row/table:table-cell[descendant::text:a]"
      mode="HyperlinkText"/>

  </xsl:template>

  <xsl:template match="table:table-cell" mode="HyperlinkText">

    <xsl:variable name="StyleName">
      <xsl:value-of select="@table:style-name"/>
    </xsl:variable>

    <xsl:for-each select="descendant::text:a">
      <xsl:choose>
        <xsl:when
          test="document('content.xml')/office:document-content/office:automatic-styles/style:style[@style:name=$StyleName]/style:text-properties">
          <xsl:apply-templates
            select="document('content.xml')/office:document-content/office:automatic-styles/style:style[@style:name=$StyleName]/style:text-properties"
            mode="fonts">
            <xsl:with-param name="hyperlink">
              <xsl:text>true</xsl:text>
            </xsl:with-param>
          </xsl:apply-templates>
        </xsl:when>
        <xsl:when
          test="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name=$StyleName]">

          <xsl:choose>
            <xsl:when
              test="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name=$StyleName]/style:text-properties">
              <xsl:apply-templates
                select="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name=$StyleName]/style:text-properties"
                mode="fonts">
                <xsl:with-param name="hyperlink">
                  <xsl:text>true</xsl:text>
                </xsl:with-param>
              </xsl:apply-templates>
            </xsl:when>
            <xsl:otherwise>
              <xsl:choose>
                <xsl:when
                  test="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name=$StyleName]/@style:parent-style-name">
                  <xsl:variable name="ParentStyleName">
                    <xsl:value-of
                      select="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name=$StyleName]/@style:parent-style-name"
                    />
                  </xsl:variable>
                  <xsl:apply-templates
                    select="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name=$ParentStyleName]/style:text-properties"
                    mode="fonts">
                    <xsl:with-param name="hyperlink">
                      <xsl:text>true</xsl:text>
                    </xsl:with-param>
                  </xsl:apply-templates>
                </xsl:when>
                <xsl:otherwise>
                  <font>
                    <u val="single"/>
                    <sz val="11"/>
                    <color theme="10"/>
                    <name val="Calibri"/>
                    <family val="2"/>
                  </font>
                </xsl:otherwise>
              </xsl:choose>


            </xsl:otherwise>
          </xsl:choose>


        </xsl:when>
        <xsl:otherwise>
          <font>
            <u val="single"/>
            <sz val="11"/>
            <color theme="10"/>
            <name val="Calibri"/>
            <family val="2"/>
          </font>
        </xsl:otherwise>
      </xsl:choose>

    </xsl:for-each>


  </xsl:template>

</xsl:stylesheet>
