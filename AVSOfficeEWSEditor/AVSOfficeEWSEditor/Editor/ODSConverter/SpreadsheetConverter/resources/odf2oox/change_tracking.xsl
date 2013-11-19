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
    xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"
    xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
    xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:pzip="urn:cleverage:xmlns:post-processings:zip"
    xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
    xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
    xmlns:meta="urn:oasis:names:tc:opendocument:xmlns:meta:1.0"
    xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
    xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
    xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
    exclude-result-prefixes="#default w r office style table">

    <xsl:key name="OldValue" match="table:cell-content-change" use="@table:id"/>

    <xsl:template name="revisionHeaders">
        <xsl:for-each
            select="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:tracked-changes">
            <pzip:entry pzip:target="xl/revisions/revisionHeaders.xml">
                <xsl:call-template name="revisionHeaderProperties"/>
            </pzip:entry>
        </xsl:for-each>
    </xsl:template>

    <!-- Create Revisions Files -->
    <xsl:template name="CreateRevisionFiles">
        <xsl:for-each
            select="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:tracked-changes">
            <xsl:apply-templates
                select="node()[1][name()='table:cell-content-change' or name()='table:deletion' or name()='table:movement' or name()='table:insertion']"
                mode="revisionFiles"/>
        </xsl:for-each>
    </xsl:template>

    <xsl:template
        match="table:cell-content-change|table:deletion|table:movement|table:insertion|table:insertion"
        mode="revisionFiles">
        <xsl:param name="rId" select="1"/>

        <pzip:entry pzip:target="{concat('xl/revisions/revisionLog', generate-id(), '.xml')}">
            <xsl:call-template name="InsertChangeTrackingProperties">
                <xsl:with-param name="rId">
                    <xsl:value-of select="$rId"/>
                </xsl:with-param>
            </xsl:call-template>
        </pzip:entry>

        <xsl:if
            test="following-sibling::node()[1][name()='table:cell-content-change' or name()='table:deletion' or name()='table:movement' or name()='table:insertion']">
            <xsl:apply-templates
                select="following-sibling::node()[1][name()='table:cell-content-change' or name()='table:deletion' or name()='table:movement' or name()='table:insertion']"
                mode="revisionFiles">
                <xsl:with-param name="rId">
                    <xsl:value-of select="$rId+1"/>
                </xsl:with-param>
            </xsl:apply-templates>
        </xsl:if>

    </xsl:template>

    <xsl:template name="userName">
        <pzip:entry pzip:target="xl/revisions/userNames.xml">
            <users xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main"
                xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
                count="0"/>
        </pzip:entry>
    </xsl:template>

    <!-- Insert Change Tracking Properties -->
    <xsl:template name="InsertChangeTrackingProperties">
        <xsl:param name="rId"/>

        <revisions xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main"
            xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships">

            <xsl:if test="name() = 'table:cell-content-change'">

                <xsl:variable name="colNum">
                    <xsl:value-of select="table:cell-address/@table:column"/>
                </xsl:variable>

                <xsl:variable name="rowNum">
                    <xsl:value-of select="table:cell-address/@table:row"/>
                </xsl:variable>

                <xsl:variable name="colNumChar">
                    <xsl:call-template name="NumbersToChars">
                        <xsl:with-param name="num">
                            <xsl:value-of select="table:cell-address/@table:column"/>
                        </xsl:with-param>
                    </xsl:call-template>
                </xsl:variable>

                <xsl:variable name="sheetId">
                    <xsl:value-of select="table:cell-address/@table:table"/>
                </xsl:variable>


                <rcc>

                    <xsl:attribute name="rId">
                        <xsl:value-of select="$rId"/>
                    </xsl:attribute>

                    <xsl:attribute name="sId">
                        <xsl:value-of select="$sheetId + 1"/>
                    </xsl:attribute>


                    <xsl:variable name="OldValue">
                        <xsl:choose>
                            <xsl:when test="table:previous/@table:id">
                                <xsl:choose>
                                    <xsl:when
                                        test="key('OldValue', @table:id)/table:previous/table:change-track-table-cell/text:p">
                                        <xsl:value-of
                                            select="key('OldValue', @table:id)/table:previous/table:change-track-table-cell/text:p"
                                        />
                                    </xsl:when>
                                    <xsl:when
                                        test="key('OldValue', @table:id)/table:previous/table:change-track-table-cell/@office:value">
                                        <xsl:value-of
                                            select="key('OldValue', @table:id)/table:previous/table:change-track-table-cell/@office:value"
                                        />
                                    </xsl:when>
                                </xsl:choose>
                            </xsl:when>
                            <xsl:when test="table:previous/table:change-track-table-cell/text:p">
                                <xsl:value-of
                                    select="table:previous/table:change-track-table-cell/text:p"/>
                            </xsl:when>
                            <xsl:when
                                test="table:previous/table:change-track-table-cell/@office:value">
                                <xsl:value-of
                                    select="table:previous/table:change-track-table-cell/@office:value"
                                />
                            </xsl:when>
                        </xsl:choose>
                    </xsl:variable>

                    <oc t="inlineStr">
                        <xsl:attribute name="r">
                            <xsl:value-of select="concat($colNumChar, $rowNum + 1)"/>
                        </xsl:attribute>
                        <xsl:if test="$OldValue != ''">
                            <is>
                                <t>
                                    <xsl:value-of select="$OldValue"/>
                                </t>
                            </is>
                        </xsl:if>
                    </oc>


                    <xsl:variable name="NewValue">
                        <xsl:choose>
                            <xsl:when
                                test="following-sibling::table:cell-content-change[table:cell-address/@table:column = $colNum and table:cell-address/@table:row = $rowNum and table:cell-address/@table:table = $sheetId]">
                                <xsl:for-each
                                    select="following-sibling::table:cell-content-change[table:cell-address/@table:column = $colNum and table:cell-address/@table:row = $rowNum and table:cell-address/@table:table = $sheetId][1]">
                                    <xsl:choose>
                                        <xsl:when
                                            test="table:previous/table:change-track-table-cell/text:p">
                                            <xsl:value-of
                                                select="table:previous/table:change-track-table-cell/text:p"
                                            />
                                        </xsl:when>
                                        <xsl:otherwise>
                                            <xsl:value-of
                                                select="table:previous/table:change-track-table-cell/@office:value"
                                            />
                                        </xsl:otherwise>
                                    </xsl:choose>
                                </xsl:for-each>
                            </xsl:when>
                            <xsl:otherwise>
                                <xsl:call-template name="SearchPresentValue">
                                    <xsl:with-param name="sheetId">
                                        <xsl:value-of select="$sheetId + 1"/>
                                    </xsl:with-param>
                                    <xsl:with-param name="SearchColNum">
                                        <xsl:value-of select="$colNum"/>
                                    </xsl:with-param>
                                    <xsl:with-param name="SearchRowNum">
                                        <xsl:value-of select="$rowNum"/>
                                    </xsl:with-param>
                                </xsl:call-template>
                            </xsl:otherwise>
                        </xsl:choose>
                    </xsl:variable>

                    <nc t="inlineStr">
                        <xsl:attribute name="r">
                            <xsl:value-of select="concat($colNumChar, $rowNum + 1)"/>
                        </xsl:attribute>
                        <xsl:if test="$NewValue != ''">
                            <is>
                                <t>
                                    <xsl:value-of select="$NewValue"/>
                                </t>
                            </is>
                        </xsl:if>
                    </nc>

                </rcc>

            </xsl:if>

            <xsl:if test="name()='table:deletion'">

                <xsl:variable name="rowNum">
                    <xsl:value-of select="./@table:position"/>
                </xsl:variable>

                <xsl:variable name="colNumChar">
                    <xsl:call-template name="NumbersToChars">
                        <xsl:with-param name="num">
                            <xsl:value-of select="./@table:position"/>
                        </xsl:with-param>
                    </xsl:call-template>
                </xsl:variable>

                <xsl:variable name="sheetId">
                    <xsl:value-of select="./@table:table + 1"/>
                </xsl:variable>

                <xsl:choose>
                    <xsl:when test="@table:type='column'">
                        <rrc rId="{$rId}" sId="{$sheetId}"
                            ref="{concat($colNumChar, '1', ':', $colNumChar, '1048576')}"
                            action="deleteCol">
                            <rfmt sheetId="{$sheetId}" xfDxf="1"
                                sqref="{concat($colNumChar, '1', ':', $colNumChar, '1048576')}"
                                start="0" length="0"/>
                        </rrc>
                    </xsl:when>
                    <xsl:when test="@table:type='row'">
                        <rrc rId="{$rId}" sId="{$sheetId}"
                            ref="{concat('A', $rowNum, ':', 'XFD',$rowNum)}" action="deleteRow">
                            <rfmt sheetId="{$sheetId}" xfDxf="1"
                                sqref="{concat('A', $rowNum, ':', 'XFD',$rowNum)}" start="0"
                                length="0"/>
                        </rrc>
                    </xsl:when>
                </xsl:choose>
            </xsl:if>

            <xsl:if test="name()='table:movement'">

                <xsl:choose>
                    <!-- When source range has a start column and target range hasn`t --> 
                    <xsl:when
                        test="table:source-range-address/@table:start-column and table:source-range-address/@table:start-row and not(table:target-range-address/@table:start-column) and not(table:target-range-address/@table:end-column)"/> 

                    <xsl:when
                        test="table:source-range-address/@table:start-column and table:source-range-address/@table:start-row">

                        <xsl:variable name="SourceRowNumStart">
                            <xsl:value-of select="table:source-range-address/@table:start-row + 1"/>
                        </xsl:variable>

                        <xsl:variable name="SourceColNumStart">
                            <xsl:call-template name="NumbersToChars">
                                <xsl:with-param name="num">
                                    <xsl:value-of
                                        select="table:source-range-address/@table:start-column"/>
                                </xsl:with-param>
                            </xsl:call-template>
                        </xsl:variable>

                        <xsl:variable name="SourceRowNumEnd">
                            <xsl:value-of select="table:source-range-address/@table:end-row + 1"/>
                        </xsl:variable>

                        <xsl:variable name="SourceColNumEnd">
                            <xsl:call-template name="NumbersToChars">
                                <xsl:with-param name="num">
                                    <xsl:value-of
                                        select="table:source-range-address/@table:end-column"/>
                                </xsl:with-param>
                            </xsl:call-template>
                        </xsl:variable>

                        <xsl:variable name="TargetRowNumStart">
                            <xsl:value-of select="table:target-range-address/@table:start-row + 1"/>
                        </xsl:variable>

                        <xsl:variable name="TargetColNumStart">
                            <xsl:call-template name="NumbersToChars">
                                <xsl:with-param name="num">
                                    <xsl:value-of
                                        select="table:target-range-address/@table:start-column"/>
                                </xsl:with-param>
                            </xsl:call-template>
                        </xsl:variable>

                        <xsl:variable name="TargetRowNumEnd">
                            <xsl:value-of select="table:target-range-address/@table:end-row + 1"/>
                        </xsl:variable>

                        <xsl:variable name="TargetColNumEnd">
                            <xsl:call-template name="NumbersToChars">
                                <xsl:with-param name="num">
                                    <xsl:value-of
                                        select="table:target-range-address/@table:end-column"/>
                                </xsl:with-param>
                            </xsl:call-template>
                        </xsl:variable>

                        <!--xxx>
                                    <xsl:value-of
                                select="concat($SourceRowNumStart, $SourceColNumStart, $SourceRowNumEnd, $SourceColNumEnd)"
                                />
                                    <xsl:value-of select="$TargetColNumEnd"/>
                                </xxx>

                                <zzzz>
                            SourceRowNumStart <xsl:value-of select="$SourceRowNumStart"/>
                            SourceColNumStart<xsl:value-of select="$SourceColNumStart"/>
                            SourceRowNumEnd <xsl:value-of select="$SourceRowNumEnd"/>
                            SourceColNumEnd<xsl:value-of select="$SourceColNumEnd"/>
                            TargetRowNumStart<xsl:value-of select="$TargetRowNumStart"/>
                            TargetColNumStart<xsl:value-of select="___$TargetColNumStart"/>
                            TargetRowNumEnd<xsl:value-of select="$TargetRowNumEnd"/>
                            TargetColNumEnd<xsl:value-of select="___$TargetColNumEnd"/>
                        </zzzz-->

                        <rm rId="{$rId}"
                            sheetId="{table:source-range-address/@table:start-table + 1}"
                            source="{concat($SourceColNumStart, $SourceRowNumStart, ':', $SourceColNumEnd, $SourceRowNumEnd)}"
                            destination="{concat($TargetColNumStart, $TargetRowNumStart, ':', $TargetColNumEnd, $TargetRowNumEnd)}"
                            sourceSheetId="{table:target-range-address/@table:start-table + 1}"/>

                    </xsl:when>
                    
                    <!-- When target range has a start column and source range hasn`t  --> 
                    <xsl:when
                        test="table:target-range-address/@table:start-column and table:target-range-address/@table:end-column  [not(table:source-range-address/@table:start-column) and not(table:source-range-address/@table:start-row)]"/>
                        
                        <xsl:when
                            test="table:target-range-address/@table:start-column and table:target-range-address/@table:end-column">
                        
                        <xsl:variable name="SourceRowNum">
                            <xsl:value-of select="table:source-range-address/@table:row + 1"/>
                        </xsl:variable>
                        
                        <xsl:variable name="SourceColNum">
                            <xsl:call-template name="NumbersToChars">
                                <xsl:with-param name="num">
                                    <xsl:value-of select="table:source-range-address/@table:column"/>
                                </xsl:with-param>
                            </xsl:call-template>
                        </xsl:variable>
                        
                        <xsl:variable name="TargetRowNum">
                            <xsl:value-of select="table:target-range-address/@table:row + 1"/>
                        </xsl:variable>
                        
                        <xsl:variable name="TargetColNum">
                            <xsl:call-template name="NumbersToChars">
                                <xsl:with-param name="num">
                                    <xsl:value-of select="table:target-range-address/@table:column"/>
                                </xsl:with-param>
                            </xsl:call-template>
                        </xsl:variable>
                        
                        <rm rId="{$rId}" sheetId="{table:source-range-address/@table:table + 1}" source="{concat($SourceColNum, $SourceRowNum)}" destination="{concat($TargetColNum, $TargetRowNum)}" sourceSheetId="{table:target-range-address/@table:table + 1}"/>
   
                    </xsl:when>
                </xsl:choose>

            </xsl:if>

            <xsl:if test="name()='table:insertion'">



                <xsl:choose>

                    <xsl:when test="@table:type='table'">

                        <xsl:variable name="sheetId">
                            <xsl:value-of select="@table:position+1"/>
                        </xsl:variable>


                        <ris rId="{$rId}" sheetId="{$sheetId}"
                            name="{concat('[]', document('content.xml')/office:document-content/office:body/office:spreadsheet/table:table[position() = $sheetId]/@table:name)}"
                            sheetPosition="{$sheetId}"/>

                    </xsl:when>

                    <xsl:when test="@table:type='row'">

                        <xsl:variable name="sheetId">
                            <xsl:value-of select="@table:table+1"/>
                        </xsl:variable>

                        <xsl:variable name="rowInsert">
                            <xsl:value-of select="@table:position + 1"/>
                        </xsl:variable>

                        <rrc rId="{$rId}" sId="{$sheetId}"
                            ref="{concat('A', $rowInsert, ':XFD', $rowInsert)}" action="insertRow"/>

                    </xsl:when>

                    <xsl:when test="@table:type='column'">

                        <xsl:variable name="sheetId">
                            <xsl:value-of select="@table:table+1"/>
                        </xsl:variable>

                        <xsl:variable name="colInsert">
                            <xsl:call-template name="NumbersToChars">
                                <xsl:with-param name="num">
                                    <xsl:value-of select="@table:position"/>
                                </xsl:with-param>
                            </xsl:call-template>
                        </xsl:variable>

                        <rrc rId="{$rId}" sId="{$sheetId}"
                            ref="{concat($colInsert, '1:', $colInsert, '1048576')}"
                            action="insertCol"/>

                    </xsl:when>

                </xsl:choose>




            </xsl:if>

        </revisions>

    </xsl:template>

    <xsl:template name="SearchPresentValue">
        <xsl:param name="sheetId"/>
        <xsl:param name="SearchColNum"/>
        <xsl:param name="SearchRowNum"/>

        <xsl:for-each
            select="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:table[position() = $sheetId]">
            <xsl:apply-templates select="table:table-row[1]" mode="changeTracking">
                <xsl:with-param name="rowNumber">
                    <xsl:text>0</xsl:text>
                </xsl:with-param>
                <xsl:with-param name="colNumber">
                    <xsl:text>0</xsl:text>
                </xsl:with-param>
                <xsl:with-param name="SearchColNum">
                    <xsl:value-of select="$SearchColNum"/>
                </xsl:with-param>
                <xsl:with-param name="SearchRowNum">
                    <xsl:value-of select="$SearchRowNum"/>
                </xsl:with-param>
            </xsl:apply-templates>
        </xsl:for-each>

    </xsl:template>

    <!-- Search Value -->
    <xsl:template match="table:table-row" mode="changeTracking">
        <xsl:param name="rowNumber"/>
        <xsl:param name="colNumber"/>
        <xsl:param name="SearchColNum"/>
        <xsl:param name="SearchRowNum"/>

        <xsl:apply-templates select="child::node()[name() = 'table:table-cell'][1]"
            mode="changeTracking">
            <xsl:with-param name="colNumber">
                <xsl:text>0</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="rowNumber" select="$rowNumber"/>
            <xsl:with-param name="SearchColNum">
                <xsl:value-of select="$SearchColNum"/>
            </xsl:with-param>
            <xsl:with-param name="SearchRowNum">
                <xsl:value-of select="$SearchRowNum"/>
            </xsl:with-param>
        </xsl:apply-templates>

        <xsl:if test="$SearchRowNum &gt;= $rowNumber">
            <!-- check next row -->
            <xsl:choose>
                <!-- next row is a sibling -->
                <xsl:when test="following-sibling::node()[1][name() = 'table:table-row' ]">
                    <xsl:apply-templates select="following-sibling::table:table-row[1]"
                        mode="changeTracking">
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
												<xsl:value-of select = "$rowNumber+(65536 - (1048576 - @table:number-rows-repeated))"/>
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
                        <xsl:with-param name="colNumber">
                            <xsl:text>0</xsl:text>
                        </xsl:with-param>
                        <xsl:with-param name="SearchColNum">
                            <xsl:value-of select="$SearchColNum"/>
                        </xsl:with-param>
                        <xsl:with-param name="SearchRowNum">
                            <xsl:value-of select="$SearchRowNum"/>
                        </xsl:with-param>
                    </xsl:apply-templates>
                </xsl:when>
            </xsl:choose>
        </xsl:if>
    </xsl:template>

    <!-- Search Value -->
    <xsl:template match="table:table-cell|table:covered-table-cell" mode="changeTracking">
        <xsl:param name="colNumber"/>
        <xsl:param name="rowNumber"/>
        <xsl:param name="SearchColNum"/>
        <xsl:param name="SearchRowNum"/>

        <xsl:if test="$rowNumber = $SearchRowNum and $colNumber = $SearchColNum">
            <xsl:value-of select="text:p"/>
        </xsl:if>

        <xsl:if test="$SearchRowNum &gt;= $rowNumber and $SearchColNum &gt; $colNumber">

            <xsl:choose>
                <xsl:when test="following-sibling::table:table-cell">

                    <xsl:apply-templates
                        select="following-sibling::node()[name() = 'table:table-cell' or name() = 'table:covered-table-cell'][1]"
                        mode="changeTracking">
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
						<!--Vijayeta,SP2,@table:number-columns-repeated-->
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
						<!--Vijayeta,SP2,@table:number-columns-repeated-->
                        <xsl:with-param name="rowNumber">
                            <xsl:value-of select="$rowNumber"/>
                        </xsl:with-param>
                        <xsl:with-param name="SearchColNum">
                            <xsl:value-of select="$SearchColNum"/>
                        </xsl:with-param>
                        <xsl:with-param name="SearchRowNum">
                            <xsl:value-of select="$SearchRowNum"/>
                        </xsl:with-param>
                    </xsl:apply-templates>

                </xsl:when>
            </xsl:choose>

        </xsl:if>

    </xsl:template>


</xsl:stylesheet>
