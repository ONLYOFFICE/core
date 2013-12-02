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
    xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
    xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
	xmlns:meta="urn:oasis:names:tc:opendocument:xmlns:meta:1.0"
    xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
    xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
    exclude-result-prefixes="table r">
    <!--<xsl:import href="conditional.xsl"/>-->

    <!-- search validation -->
    <xsl:template match="table:table-row" mode="validation">
        <xsl:param name="rowNumber"/>
        <xsl:param name="cellNumber"/>
        <xsl:param name="TableColumnTagNum"/>
        <xsl:param name="MergeCell"/>
        <xsl:param name="tableName"/>

        <xsl:apply-templates select="child::node()[name() = 'table:table-cell'][1]"
            mode="validation">
            <xsl:with-param name="colNumber">
                <xsl:text>0</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="rowNumber" select="$rowNumber"/>
            <xsl:with-param name="TableColumnTagNum" select="$TableColumnTagNum"/>
            <xsl:with-param name="MergeCell" select="$MergeCell"/>
            <xsl:with-param name="tableName" select="$tableName"/>
        </xsl:apply-templates>

        <!-- check next row -->
        <xsl:choose>
            <!-- next row is a sibling -->
            <xsl:when test="following-sibling::node()[1][name() = 'table:table-row' ]">
                <xsl:apply-templates select="following-sibling::table:table-row[1]"
                    mode="validation">
					<!--Vijayeta,SP2,@table:number-rows-repeated-->
                    <!--<xsl:with-param name="rowNumber">
                        <xsl:choose>
                            <xsl:when test="@table:number-rows-repeated">
                                <xsl:value-of select="$rowNumber+@table:number-rows-repeated"/>
                            </xsl:when>
                            <xsl:otherwise>
                                <xsl:value-of select="$rowNumber+1"/>
                            </xsl:otherwise>
                        </xsl:choose>
                    </xsl:with-param>-->
                    <xsl:with-param name="rowNumber">
                        <xsl:choose>
                            <xsl:when test="@table:number-rows-repeated">
								<xsl:choose >
									<xsl:when test ="@table:number-rows-repeated &gt; 65536">
										<xsl:value-of select ="$rowNumber+(65536 - (1048576 - @table:number-rows-repeated))"/>
									</xsl:when>
									<xsl:when test ="@table:number-rows-repeated &lt;= 65536">
                                <xsl:value-of select="$rowNumber+@table:number-rows-repeated"/>
                            </xsl:when>
								</xsl:choose>
							</xsl:when>
                            <xsl:otherwise>
                                <xsl:value-of select="$rowNumber+1"/>
                            </xsl:otherwise>
                        </xsl:choose>
                    </xsl:with-param>
					<!--Vijayeta,SP2,@table:number-rows-repeated,End-->
                    <xsl:with-param name="cellNumber">
                        <xsl:text>0</xsl:text>
                    </xsl:with-param>
                    <xsl:with-param name="TableColumnTagNum" select="$TableColumnTagNum"/>
                    <xsl:with-param name="MergeCell" select="$MergeCell"/>
                    <xsl:with-param name="tableName" select="$tableName"/>
                </xsl:apply-templates>
            </xsl:when>
        </xsl:choose>
    </xsl:template>

    <!-- insert validation -->
    <xsl:template match="table:table-cell|table:covered-table-cell" mode="validation">
        <xsl:param name="colNumber"/>
        <xsl:param name="rowNumber"/>
        <xsl:param name="TableColumnTagNum"/>
        <xsl:param name="MergeCell"/>
        <xsl:param name="tableName"/>

        <xsl:if test="./@table:content-validation-name != ''">

            <xsl:variable name="ValidationName">
                <xsl:value-of select="@table:content-validation-name"/>
            </xsl:variable>
			<!--Vijayeta,SP2,@table:number-columns-repeated/@table:number-rows-repeated-->
			<!--<xsl:variable name="RowsRepeated">
				<xsl:value-of select="parent::table:table-row/@table:number-rows-repeated"/>
			</xsl:variable>-->
			<!--<xsl:variable name="CollRepeated">
                <xsl:value-of select="@table:number-columns-repeated"/>
            </xsl:variable>-->
            <xsl:variable name="RowsRepeated">
				<xsl:choose >
					<xsl:when test ="parent::table:table-row/@table:number-rows-repeated &gt; 65536">
						<xsl:value-of select ="65536 - (1048576 - parent::table:table-row/@table:number-rows-repeated)"/>
					</xsl:when>
					<xsl:when test ="parent::table:table-row/@table:number-rows-repeated &lt;= 65536">
                <xsl:value-of select="parent::table:table-row/@table:number-rows-repeated"/>
					</xsl:when>
				</xsl:choose>
            </xsl:variable>
            <xsl:variable name="CollRepeated">
				<!--<xsl:choose >
					<xsl:when test ="@table:number-columns-repeated &gt; 256">
						<xsl:value-of select ="256 - (16384 - @table:number-columns-repeated)"/>
					</xsl:when>
					<xsl:when test ="@table:number-columns-repeated &lt;= 256">
                <xsl:value-of select="@table:number-columns-repeated"/>
					</xsl:when>
				</xsl:choose>-->
				<xsl:variable name ="tempOfficeVersion">
					<xsl:value-of select ="document('meta.xml')/office:document-meta/office:meta/meta:generator"/>
				</xsl:variable>
				<xsl:choose>
					<xsl:when test ="starts-with($tempOfficeVersion,'OpenOffice.org')">
						<xsl:value-of select="@table:number-columns-repeated"/>
					</xsl:when>
					<xsl:when test ="starts-with($tempOfficeVersion,'MicrosoftOffice')">
						<xsl:choose>
							<xsl:when test ="@table:number-columns-repeated &gt; 256">
								<xsl:value-of select ="256 - (16384 - @table:number-columns-repeated)"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="@table:number-columns-repeated"/>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="@table:number-columns-repeated"/>
					</xsl:otherwise>
				</xsl:choose>
            </xsl:variable>

			<!--Vijayeta,SP2,@table:number-columns-repeated/@table:number-rows-repeated,End-->
            <xsl:for-each
                select="ancestor::office:body/office:spreadsheet/table:content-validations/table:content-validation[@table:name = $ValidationName]">
                <xsl:call-template name="dataValid">
                    <xsl:with-param name="rowNumber">
                        <xsl:value-of select="$rowNumber"/>
                    </xsl:with-param>
                    <xsl:with-param name="colNumber">
                        <xsl:value-of select="$colNumber"/>
                    </xsl:with-param>
                    <xsl:with-param name="RowsRepeated">
                        <xsl:value-of select="$RowsRepeated"/>
                    </xsl:with-param>
                    <xsl:with-param name="CollRepeated">
                        <xsl:value-of select="$CollRepeated"/>
                    </xsl:with-param>
                </xsl:call-template>
            </xsl:for-each>
        </xsl:if>

        <xsl:if test="following-sibling::table:table-cell">
            <xsl:apply-templates
                select="following-sibling::node()[1][name() = 'table:table-cell' or name() = 'table:covered-table-cell']"
                mode="validation">
				<!--Vijayeta,SP2,@table:number-columns-repeated,End-->
                <!--<xsl:with-param name="colNumber">
                    <xsl:choose>
                        <xsl:when test="@table:number-columns-repeated != ''">
                            <xsl:value-of
                                select="number($colNumber) + number(@table:number-columns-repeated)"
                            />
                        </xsl:when>
                        <xsl:otherwise>
                            <xsl:value-of select="$colNumber + 1"/>
                        </xsl:otherwise>
                    </xsl:choose>
                </xsl:with-param>-->
				<xsl:with-param name="colNumber">
					<!--<xsl:choose>
						<xsl:when test="@table:number-columns-repeated != ''">
							<xsl:choose >
								<xsl:when test ="@table:number-columns-repeated &gt; 256">
									<xsl:value-of select ="number($colNumber) + number(256 - (16384 - number(@table:number-columns-repeated)))"/>
								</xsl:when>
								<xsl:when test ="@table:number-columns-repeated &lt;= 256">
									<xsl:value-of
								select="number($colNumber) + number(@table:number-columns-repeated)"/>
								</xsl:when>
							</xsl:choose>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="$colNumber + 1"/>
						</xsl:otherwise>
					</xsl:choose>-->
					<xsl:variable name ="tempOfficeVersion">
						<xsl:value-of select ="document('meta.xml')/office:document-meta/office:meta/meta:generator"/>
					</xsl:variable>
					<xsl:choose>
						<xsl:when test="@table:number-columns-repeated != ''">
							<xsl:choose>
								<xsl:when test ="starts-with($tempOfficeVersion,'OpenOffice.org')">
									<xsl:value-of
										select="number($colNumber) + number(@table:number-columns-repeated)"/>
								</xsl:when>
								<xsl:when test ="starts-with($tempOfficeVersion,'MicrosoftOffice')">
									<xsl:choose>
										<xsl:when test ="@table:number-columns-repeated &gt; 256">
											<xsl:value-of select ="number($colNumber) + number(256 - (16384 - number(@table:number-columns-repeated)))"/>
										</xsl:when>
										<xsl:otherwise>
											<xsl:value-of
										select="number($colNumber) + number(@table:number-columns-repeated)"/>
										</xsl:otherwise>
									</xsl:choose>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of
										select="number($colNumber) + number(@table:number-columns-repeated)"/>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="$colNumber + 1"/>
						</xsl:otherwise>
					</xsl:choose>
                </xsl:with-param>
				<!--Vijayeta,SP2,@table:number-columns-repeated,End-->
                <xsl:with-param name="rowNumber">
                    <xsl:value-of select="$rowNumber"/>
                </xsl:with-param>
                <xsl:with-param name="TableColumnTagNum" select="$TableColumnTagNum"/>
                <xsl:with-param name="MergeCell" select="$MergeCell"/>
                <xsl:with-param name="tableName" select="$tableName"/>
            </xsl:apply-templates>
        </xsl:if>

    </xsl:template>

    <!-- Data Validation Template -->
    <xsl:template name="dataValid">
        <xsl:param name="colNumber"/>
        <xsl:param name="rowNumber"/>
        <xsl:param name="RowsRepeated"/>
        <xsl:param name="CollRepeated"/>

        <dataValidation>
            <!-- Criteria Allow -->
            <xsl:attribute name="type">
                <xsl:choose>
                    <xsl:when test="contains(@table:condition, 'is-whole-number')">
                        <xsl:text>whole</xsl:text>
                    </xsl:when>
                    <xsl:when test="contains(@table:condition, 'is-decimal-number')">
                        <xsl:text>decimal</xsl:text>
                    </xsl:when>
                    <xsl:when test="contains(@table:condition, 'is-date')">
                        <xsl:text>date</xsl:text>
                    </xsl:when>
                    <xsl:when test="contains(@table:condition, 'is-time')">
                        <xsl:text>time</xsl:text>
                    </xsl:when>
					<!--Conditon(or condition) added by Sateesh, fix for the bug 1768233
					Bug Description:ODS Filters dropdown list worngly converted-office XP/2003
					File Name:Criminal.EN.ods
					Date:14th July '08-->
                    <xsl:when test="contains(@table:condition, 'is-in-list(&quot;') or contains(@table:condition, 'cell-content-is-in-list')">
                        <xsl:text>list</xsl:text>
                    </xsl:when>
                    <xsl:when test="contains(@table:condition, 'cell-content-text-length')">
                        <xsl:text>textLength</xsl:text>
                    </xsl:when>
                    <xsl:otherwise>
                        <xsl:text>whole</xsl:text>
                    </xsl:otherwise>
                </xsl:choose>
            </xsl:attribute>

            <!-- Error Allert Action-->
			<!--
				Changes By: Vijayeta
				Desc      : attribute 'errorStyle' is set to blank in SP2 converted file, which actually works in SP2 but in SP1
				File      : MS_Kalkulace_Campus_Agreement.xlsx->SP2->MS_Kalkulace_Campus_Agreement.ods->Translator->MS_Kalkulace_Campus_Agreement.xlsx
			-->
            <xsl:for-each select="table:error-message">
					<xsl:if test ="table:error-message/@table:message-type">
                <xsl:attribute name="errorStyle">
                    <xsl:choose>
                        <xsl:when test="contains(@table:message-type, 'information')">
                            <xsl:text>information</xsl:text>
                        </xsl:when>
                        <xsl:when test="contains(@table:message-type, 'warning')">
                            <xsl:text>warning</xsl:text>
                        </xsl:when>
                        <xsl:when test="contains(@table:message-type, 'stop')">
                            <xsl:text>stop</xsl:text>
                        </xsl:when>
                    </xsl:choose>
                </xsl:attribute>
					</xsl:if>                
            </xsl:for-each>            
            <xsl:if test="table:error-macro">
                <xsl:message terminate="no">translation.odf2oox.DataValidErrorMacro</xsl:message>
            </xsl:if>
            <xsl:if test="contains(@table:condition, 'is-in-list(') and not(contains(@table:condition, 'is-in-list(&quot;'))">
                <xsl:message terminate="no">translation.odf2oox.DataValidCellRange</xsl:message>
            </xsl:if>         
            <!-- Criteria Data -->
            <xsl:attribute name="operator">
                <xsl:choose>
                    <xsl:when test="contains(@table:condition, '&lt;=')">
                        <xsl:text>lessThanOrEqual</xsl:text>
                    </xsl:when>
                    <xsl:when test="contains(@table:condition, '&lt;')">
                        <xsl:text>lessThan</xsl:text>
                    </xsl:when>
                    <xsl:when test="contains(@table:condition, '&gt;=')">
                        <xsl:text>greaterThanOrEqual</xsl:text>
                    </xsl:when>
                    <xsl:when test="contains(@table:condition, '&gt;')">
                        <xsl:text>greaterThan</xsl:text>
                    </xsl:when>
                    <xsl:when test="contains(@table:condition, '!=')">
                        <xsl:text>notEqual</xsl:text>
                    </xsl:when>
                    <xsl:when test="contains(@table:condition, 'is-between')">
                        <xsl:text>between</xsl:text>
                    </xsl:when>
                    <xsl:when test="contains(@table:condition, 'is-not-between')">
                        <xsl:text>notBetween</xsl:text>
                    </xsl:when>
                    <xsl:otherwise>
                        <xsl:text>equal</xsl:text>
                    </xsl:otherwise>
                </xsl:choose>
            </xsl:attribute>

            <!-- Criteria Allow Blank Cells -->
            <xsl:choose>
                <xsl:when test="contains(@table:allow-empty-cell, 'true')">
                    <xsl:attribute name="allowBlank">
                        <xsl:text>1</xsl:text>
                    </xsl:attribute>
                </xsl:when>
            </xsl:choose>

            <!-- Input Help - Show input help when cell is selected -->
            <xsl:for-each select="table:help-message">
                <xsl:if test="contains(@table:display, 'true')">
                    <xsl:attribute name="showInputMessage">
                        <xsl:text>1</xsl:text>
                    </xsl:attribute>
                </xsl:if>

                <!-- Input Help Title -->
                <xsl:if test="@table:title !='' ">
                    <xsl:attribute name="promptTitle">
                        <xsl:value-of select="@table:title"/>
                    </xsl:attribute>
                </xsl:if>

                <!-- Input Help - input help -->
                <xsl:if test=".!=''">
                    <xsl:attribute name="prompt">
                        <xsl:value-of select="."/>
                    </xsl:attribute>
                </xsl:if>
            </xsl:for-each>

            <!-- Show error message wheninvalid values are entered -->
            <xsl:for-each select="table:error-message">
                <xsl:if test="contains(@table:display, 'true')">
                    <xsl:attribute name="showErrorMessage">
                        <xsl:text>1</xsl:text>
                    </xsl:attribute>
                </xsl:if>

                <!-- Error Allert Title -->
                <xsl:if test="@table:title !='' ">
                    <xsl:attribute name="errorTitle">
                        <xsl:value-of select="@table:title"/>
                    </xsl:attribute>
                </xsl:if>

                <!-- Error Allert message -->
                <xsl:if test=".!='' ">
                    <xsl:attribute name="error">
                        <xsl:value-of select="substring(., 1, 255)"/>
                    </xsl:attribute>
                </xsl:if>
            </xsl:for-each>

            <!-- Converts number of column on letter -->
            <xsl:variable name="Collnumber2Char">
                <xsl:call-template name="NumbersToChars">
                    <xsl:with-param name="num">
                        <xsl:value-of select="$colNumber"/>
                    </xsl:with-param>
                </xsl:call-template>
            </xsl:variable>

            <!-- Defines range of cells -->
            <xsl:attribute name="sqref">

                <xsl:choose>
                    <xsl:when test="$RowsRepeated !='' and $CollRepeated !='' ">
                        <xsl:variable name="RownumberEnd">
                            <xsl:value-of select="$rowNumber + $RowsRepeated"/>
                        </xsl:variable>
                        <xsl:variable name="CollnumberEnd2Char">
                            <xsl:call-template name="NumbersToChars">
                                <xsl:with-param name="num">
                                    <xsl:value-of select="$colNumber + $CollRepeated - 1"/>
                                </xsl:with-param>
                            </xsl:call-template>
                        </xsl:variable>
                        <xsl:value-of
                            select="concat((concat((concat($Collnumber2Char, $rowNumber)), ':')), concat($CollnumberEnd2Char, $RownumberEnd))"
                        />
                    </xsl:when>

                    <xsl:when test="$RowsRepeated !='' and $CollRepeated =''">
                        <xsl:variable name="RownumberEnd">
                            <xsl:value-of select="$rowNumber + $RowsRepeated - 1"/>
                        </xsl:variable>
                        <xsl:value-of
                            select="concat((concat((concat($Collnumber2Char, $rowNumber)), ':')), concat($Collnumber2Char, $RownumberEnd))"
                        />
                    </xsl:when>

                    <xsl:when test="$RowsRepeated ='' and $CollRepeated !=''">
                        <xsl:variable name="CollnumberEnd2Char">
                            <xsl:call-template name="NumbersToChars">
                                <xsl:with-param name="num">
                                    <xsl:value-of select="$colNumber + $CollRepeated - 1"/>
                                </xsl:with-param>
                            </xsl:call-template>
                        </xsl:variable>
                        <xsl:value-of
                            select="concat((concat((concat($Collnumber2Char, $rowNumber)), ':')), concat($CollnumberEnd2Char, $rowNumber))"
                        />
                    </xsl:when>

                    <xsl:otherwise>
                        <xsl:value-of select="concat($Collnumber2Char, $rowNumber)"/>
                    </xsl:otherwise>
                </xsl:choose>

            </xsl:attribute>

            <!-- inserts formula template -->
            <xsl:call-template name="InsertValidityFormula">
                <xsl:with-param name="tableName" select="@table:name"/>
            </xsl:call-template>

        </dataValidation>
    </xsl:template>

    <!-- Insert Criteria Formula of Data Validation -->
    <xsl:template name="InsertValidityFormula">
        <xsl:param name="tableName"/>

        <xsl:choose>
            <xsl:when test="contains(@table:condition, '.$#REF!$#REF!')">
                <formula1/>
            </xsl:when>

            <!-- Condition: Formula is -->
            <xsl:when test="contains(@table:condition, 'is-true-formula')">
                <formula1/>
            </xsl:when>

            <!-- Condition: Cell content is less than or equal to-->
            <xsl:when test="contains(@table:condition, '&lt;=')">
                <formula1>
                    <xsl:call-template name="TranslateFormula">
                        <xsl:with-param name="formula"
                            select="substring-after(@table:condition,'&lt;=')"/>
                        <xsl:with-param name="tableName" select="$tableName"/>
                    </xsl:call-template>
                </formula1>
            </xsl:when>

            <!-- Condition: Cell content is less than -->
            <xsl:when test="contains(@table:condition, '&lt;')">
                <formula1>
                    <xsl:call-template name="TranslateFormula">
                        <xsl:with-param name="formula"
                            select="substring-after(@table:condition,'&lt;')"/>
                        <xsl:with-param name="tableName" select="$tableName"/>
                    </xsl:call-template>
                </formula1>
            </xsl:when>

            <!-- Condition: Cell content is greater than or equal to-->
            <xsl:when test="contains(@table:condition, '&gt;=')">
                <formula1>
                    <xsl:call-template name="TranslateFormula">
                        <xsl:with-param name="formula"
                            select="substring-after(@table:condition,'&gt;=')"/>
                        <xsl:with-param name="tableName" select="$tableName"/>
                    </xsl:call-template>
                </formula1>
            </xsl:when>

            <!-- Condition: Cell content is greater than -->
            <xsl:when test="contains(@table:condition, '&gt;')">
                <formula1>
                    <xsl:call-template name="TranslateFormula">
                        <xsl:with-param name="formula"
                            select="substring-after(@table:condition,'&gt;')"/>
                        <xsl:with-param name="tableName" select="$tableName"/>
                    </xsl:call-template>
                </formula1>
            </xsl:when>

            <!-- Condition: Cell content is not equal to -->
            <xsl:when test="contains(@table:condition, '!=')">
                <formula1>
                    <xsl:call-template name="TranslateFormula">
                        <xsl:with-param name="formula"
                            select="substring-after(@table:condition,'!=')"/>
                        <xsl:with-param name="tableName" select="$tableName"/>
                    </xsl:call-template>
                </formula1>
            </xsl:when>

            <!-- Condition: Cell content is equal to -->
            <xsl:when test="contains(@table:condition, '=')">
                <formula1>
                    <xsl:call-template name="TranslateFormula">
                        <xsl:with-param name="formula"
                            select="substring-after(@table:condition,'=')"/>
                        <xsl:with-param name="tableName" select="$tableName"/>
                    </xsl:call-template>
                </formula1>
            </xsl:when>

            <!-- Condition: Cell content is between -->
            <!-- Condition: Cell content is not between -->
            <xsl:when test="contains(@table:condition, 'between')">
                <formula1>
                    <xsl:call-template name="TranslateFormula">
                        <xsl:with-param name="formula"
                            select="substring-before(substring-after(@table:condition,'between('),',')"/>
                        <xsl:with-param name="tableName" select="$tableName"/>
                    </xsl:call-template>
                </formula1>
                <formula2>
                    <xsl:call-template name="TranslateFormula">
                        <xsl:with-param name="formula"
                            select="substring-before(substring-after(@table:condition,','),')')"/>
                        <xsl:with-param name="tableName" select="$tableName"/>
                    </xsl:call-template>
                </formula2>
            </xsl:when>
			<!-- Condition: Cell content is in list -->
			<!-- Code added by Sateesh, fix for the bug 1768233
				 Bug Description:ODS Filters dropdown list worngly converted-office XP/2003
				 File Name:Criminal.EN.ods
				 Date:14th July '08-->
			<xsl:when test="contains(@table:condition, 'cell-content-is-in-list')">
				<xsl:variable name="listValue">
					<xsl:value-of select="substring-before(substring-after(@table:condition,'cell-content-is-in-list(['),'])')"/>
				</xsl:variable>
				<xsl:choose>
					<xsl:when test="contains($listValue,'.') and contains($listValue,':')">
						<formula1>
							<xsl:value-of select="concat(substring-after(substring-before($listValue,':'),'.'),':',substring-after(substring-after($listValue,':'),'.'))"/>
						</formula1>
						<formula2>
							<xsl:text>0</xsl:text>
						</formula2>
					</xsl:when>
					<xsl:when test="contains(@table:condition,';')">
						<formula1>
							<xsl:variable name="value">
								<xsl:value-of select="translate(translate(substring-before(substring-after(@table:condition,'cell-content-is-in-list('),')'),'&quot;',' '),';',',')"/>
							</xsl:variable>
							<xsl:value-of select="concat('&quot;',$value,'&quot;')"/>
						</formula1>
						<formula2>
							<xsl:text>0</xsl:text>
						</formula2>
					</xsl:when>
					<xsl:otherwise>
						<formula1>
							<xsl:variable name="value">
								<xsl:value-of select="translate(translate(substring-before(substring-after(@table:condition,'cell-content-is-in-list('),')'),'&quot;',' '),';',',')"/>
							</xsl:variable>
							<xsl:value-of select="concat('&quot;',$value,'&quot;')"/>
						</formula1>
						<formula2>
							<xsl:text>0</xsl:text>
						</formula2>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<!--End-->

        </xsl:choose>
    </xsl:template>

    <xsl:template name="TranslateReferences">
        <xsl:param name="string"/>
        <xsl:param name="tableName"/>

        <xsl:variable name="apos">
            <xsl:text>&apos;</xsl:text>
        </xsl:variable>

        <xsl:choose>
            <xsl:when test="contains($string,'[')">

                <xsl:variable name="reference">
                    <xsl:value-of select="substring-before(substring-after($string,'['),']')"/>
                </xsl:variable>

                <xsl:choose>
                    <!-- when reference is to a cell in the same sheet ($sheet_name.ref or .ref)-->
                    <xsl:when
                        test="substring-before($reference,'.') = concat('$',$tableName) or substring-before($reference,'.') = '' ">
                        <xsl:call-template name="TranslateReferences">
                            <xsl:with-param name="string">
                                <xsl:value-of select="substring-before($string,'[')"/>
                                <xsl:value-of select="substring-after($reference,'.')"/>
                                <xsl:value-of select="substring-after($string,']')"/>
                            </xsl:with-param>
                            <xsl:with-param name="tableName" select="$tableName"/>
                        </xsl:call-template>
                    </xsl:when>

                    <!-- when reference is to a cell in another sheet -->
                    <xsl:when test="substring-before($reference,'.') != concat('$',$tableName)">

                        <xsl:variable name="sheet">
                            <xsl:choose>
                                <xsl:when test="starts-with($reference,'$')">
                                    <xsl:value-of
                                        select="substring-after(substring-before($reference,'.'),'$')"
                                    />
                                </xsl:when>
                                <xsl:otherwise>
                                    <xsl:value-of select="substring-before($reference,'.')"/>
                                </xsl:otherwise>
                            </xsl:choose>
                        </xsl:variable>

                        <xsl:variable name="refSheetNumber">
                            <xsl:for-each
                                select="/office:document-content/office:body/office:spreadsheet/table:table[@table:name = translate($sheet,$apos,'')]">
                                <xsl:number count="table:table" level="single"/>
                            </xsl:for-each>
                        </xsl:variable>

                        <xsl:variable name="checkedName">
                            <xsl:for-each
                                select="/office:document-content/office:body/office:spreadsheet/table:table[@table:name = translate($sheet,$apos,'')]">
                                <xsl:call-template name="CheckSheetName">
                                    <xsl:with-param name="sheetNumber">
                                        <xsl:value-of select="$refSheetNumber"/>
                                    </xsl:with-param>
                                    <xsl:with-param name="name">
                                        <xsl:value-of
                                            select="substring(translate($sheet,&quot;*\/[]:&apos;?&quot;,&quot;&quot;),1,31)"
                                        />
                                    </xsl:with-param>
                                </xsl:call-template>
                            </xsl:for-each>
                        </xsl:variable>

                        <xsl:call-template name="TranslateReferences">
                            <xsl:with-param name="string">
                                <xsl:value-of select="substring-before($string,'[')"/>
                                <!-- if sheet name has space than write name in apostrophes -->
                                <xsl:if test="contains($checkedName,' ')">
                                    <xsl:text>&apos;</xsl:text>
                                </xsl:if>
                                <xsl:value-of select="$checkedName"/>
                                <xsl:if test="contains($checkedName,' ')">
                                    <xsl:text>&apos;</xsl:text>
                                </xsl:if>
                                <xsl:text>!</xsl:text>
                                <xsl:value-of select="substring-after($reference,'.')"/>
                                <xsl:value-of select="substring-after($string,']')"/>
                            </xsl:with-param>
                            <xsl:with-param name="tableName" select="$tableName"/>
                        </xsl:call-template>
                    </xsl:when>
                </xsl:choose>
            </xsl:when>

            <xsl:otherwise>
                <xsl:value-of select="$string"/>
            </xsl:otherwise>

        </xsl:choose>
    </xsl:template>
</xsl:stylesheet>
