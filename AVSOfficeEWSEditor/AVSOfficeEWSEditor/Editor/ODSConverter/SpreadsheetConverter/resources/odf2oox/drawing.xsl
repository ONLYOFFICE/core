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
Log		Date 	Modified   By       BugNo.		MOdification description
RefNo-1	28-Feb-2008 Sandeep s           1877279 XLSX:Roundtrip failure on open (V1.1):For the file Match13RIX2-AMUS.ods    
-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:xdr="http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing"
  xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main"
  xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
  xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
  xmlns:xlink="http://www.w3.org/1999/xlink"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns:chart="urn:oasis:names:tc:opendocument:xmlns:chart:1.0"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0" exclude-result-prefixes="a r">

  <!--<xsl:import href="cell.xsl"/>
  <xsl:import href="common.xsl"/>-->

  <!-- sonata:line style constants-->
  <xsl:variable name ="dot">
    <xsl:value-of select ="'0.07'"/>
  </xsl:variable>
  <xsl:variable name ="dash">
    <xsl:value-of select ="'0.282'"/>
  </xsl:variable>
  <xsl:variable name ="longDash">
    <xsl:value-of select ="'0.564'"/>
  </xsl:variable>
  <xsl:variable name ="distance">
    <xsl:value-of select ="'0.211'"/>
  </xsl:variable>
  <xsl:variable name="sm-sm">
    <xsl:value-of select ="'0.15'"/>
  </xsl:variable>
  <xsl:variable name="sm-med">
    <xsl:value-of select ="'0.18'"/>
  </xsl:variable>
  <xsl:variable name="sm-lg">
    <xsl:value-of select ="'0.2'"/>
  </xsl:variable>
  <xsl:variable name="med-sm">
    <xsl:value-of select ="'0.21'" />
  </xsl:variable>
  <xsl:variable name="med-med">
    <xsl:value-of select ="'0.25'"/>
  </xsl:variable>
  <xsl:variable name="med-lg">
    <xsl:value-of select ="'0.3'" />
  </xsl:variable>
  <xsl:variable name="lg-sm">
    <xsl:value-of select ="'0.31'" />
  </xsl:variable>
  <xsl:variable name="lg-med">
    <xsl:value-of select ="'0.35'" />
  </xsl:variable>
  <xsl:variable name="lg-lg">
    <xsl:value-of select ="'0.4'" />
  </xsl:variable>
  <!-- Insert Drawing (picture, chart)  -->
  <xsl:template name="InsertDrawing">
    <xdr:wsDr>
      <!--Insert Chart -->
      <xsl:for-each select="descendant::draw:frame">
        <xsl:variable name="chart">
          <xsl:for-each select="descendant::draw:object">
            <xsl:choose>
							<!--<xsl:when test="not(document(concat(translate(@xlink:href,'./',''),'/settings.xml'))) and ((./parent::node()/@table:end-cell-address) or not((./parent::node()/@style:rel-width) and ./parent::node()/@style:rel-height))">-->
							<xsl:when test="not(document(concat(translate(@xlink:href,'./',''),'/settings.xml'))) and (./parent::node()/@table:end-cell-address)">
								<xsl:for-each
								  select="document(concat(translate(@xlink:href,'./',''),'/content.xml'))">
									<xsl:choose>
										<xsl:when test="office:document-content/office:body/office:chart">
											<xsl:text>true</xsl:text>
										</xsl:when>
										<xsl:otherwise>
											<xsl:text>false</xsl:text>
										</xsl:otherwise>
									</xsl:choose>
								</xsl:for-each>
							</xsl:when>
							<!--Vijayeta,SP2-->
							<xsl:when test="not(document(concat(translate(@xlink:href,'./',''),'/settings.xml'))) and not(./parent::node()/@table:end-cell-address) and not(ancestor::table:shapes)
						  and not(./parent::node()/@table:end-x) and not(./parent::node()/@table:end-y) and (./parent::node()/@style:rel-width) and (./parent::node()/@style:rel-height)">
								<xsl:for-each
								  select="document(concat(translate(@xlink:href,'./',''),'/content.xml'))">
									<xsl:choose>
										<xsl:when test="office:document-content/office:body/office:chart">
											<xsl:text>true|sp2</xsl:text>
										</xsl:when>
										<xsl:otherwise>
											<xsl:text>false</xsl:text>
										</xsl:otherwise>
									</xsl:choose>
								</xsl:for-each>
							</xsl:when>
							<xsl:when test="not(document(concat(translate(@xlink:href,'./',''),'/settings.xml'))) and not (./parent::node()/@table:end-cell-address) and (ancestor::table:shapes)">
                <xsl:for-each
                  select="document(concat(translate(@xlink:href,'./',''),'/content.xml'))">
                  <xsl:choose>
                    <xsl:when test="office:document-content/office:body/office:chart">
                      <xsl:text>true</xsl:text>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:text>false</xsl:text>
                    </xsl:otherwise>
                  </xsl:choose>
                </xsl:for-each>
              </xsl:when>
              <xsl:otherwise>
                <xsl:text>false</xsl:text>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:for-each>
        </xsl:variable>
        <xsl:choose>
          <!-- insert chart -->
          <xsl:when test="contains($chart, 'true')">           
           <xsl:variable name="ChartStock">
             <xsl:choose>
               <xsl:when test="document(concat(substring-after(draw:object/@xlink:href,'./'),'/content.xml'))/office:document-content/office:body/office:chart/chart:chart/@chart:class = 'chart:stock'">
                 <xsl:text>true</xsl:text>
               </xsl:when>
               <xsl:otherwise>
                 <xsl:text>false</xsl:text>
               </xsl:otherwise>
             </xsl:choose>
           </xsl:variable>
           
           <!-- max 4 series in StockChart are supported in Excel -->
            <!--<xsl:if test="not($ChartStock = 'true')">-->
						<!--Vijayeta,SP2
						    When condition for files from translator and for files from sp2-->
						<xsl:choose>
							<xsl:when test ="$chart='true|sp2'">
								<xdr:oneCellAnchor>
									<xsl:call-template name="SetPositionSP2"/>
									<xdr:graphicFrame macro="">
										<xdr:nvGraphicFramePr>
											<xdr:cNvPr id="{position()}" name="{concat('Chart ',position())}"/>
											<xdr:cNvGraphicFramePr>
												<a:graphicFrameLocks/>
											</xdr:cNvGraphicFramePr>
										</xdr:nvGraphicFramePr>
										<xdr:xfrm>
											<a:off x="0" y="0"/>
											<a:ext cx="0" cy="0"/>
										</xdr:xfrm>
										<a:graphic>
											<a:graphicData uri="http://schemas.openxmlformats.org/drawingml/2006/chart">
												<c:chart xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart"
												  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
												  r:id="{generate-id(.)}"/>
											</a:graphicData>
										</a:graphic>
									</xdr:graphicFrame>
									<xdr:clientData/>
								</xdr:oneCellAnchor>
							</xsl:when>
							<xsl:when test ="$chart='true'">
								<!--Fix for the defect  2229985 , chart Height and width-->
								<xsl:choose>
									<xsl:when
									  test="parent::node()[name() = 'table:table-cell' or name() = 'table:covered-table-cell' ] 
			                                or parent::node()[name() = 'draw:g']">
           <xdr:twoCellAnchor>
              <xsl:call-template name="SetPosition"/>
              <xdr:graphicFrame macro="">
                <xdr:nvGraphicFramePr>
                  <xdr:cNvPr id="{position()}" name="{concat('Chart ',position())}"/>
                  <xdr:cNvGraphicFramePr>
                    <a:graphicFrameLocks/>
                  </xdr:cNvGraphicFramePr>
                </xdr:nvGraphicFramePr>
                <xdr:xfrm>
                  <a:off x="0" y="0"/>
                  <a:ext cx="0" cy="0"/>
                </xdr:xfrm>
                <a:graphic>
                  <a:graphicData uri="http://schemas.openxmlformats.org/drawingml/2006/chart">
                    <c:chart xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart"
                      xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
                      r:id="{generate-id(.)}"/>
                  </a:graphicData>
                </a:graphic>
              </xdr:graphicFrame>
              <xdr:clientData/>
            </xdr:twoCellAnchor>
							</xsl:when>
									<xsl:when test ="not(parent::node()[name() = 'draw:g']) and ancestor::table:shapes">
										<xdr:absoluteAnchor>
											<xdr:pos x="1" y="0" />
											<xdr:ext>
												<xsl:attribute name="cx">
													<xsl:call-template name="emu-measure">
														<xsl:with-param name="length" select="@svg:width" />
													</xsl:call-template>
												</xsl:attribute>
												<xsl:attribute name="cy">
													<xsl:call-template name="emu-measure">
														<xsl:with-param name="length" select="@svg:height" />
													</xsl:call-template>
												</xsl:attribute>
											</xdr:ext>
											<xdr:graphicFrame macro="">
												<xdr:nvGraphicFramePr>
													<xdr:cNvPr id="{position()}" name="{concat('Chart ',position())}"/>
													<xdr:cNvGraphicFramePr>
														<a:graphicFrameLocks/>
													</xdr:cNvGraphicFramePr>
												</xdr:nvGraphicFramePr>
												<xdr:xfrm>
													<a:off x="0" y="0"/>
													<a:ext cx="0" cy="0"/>
												</xdr:xfrm>
												<a:graphic>
													<a:graphicData uri="http://schemas.openxmlformats.org/drawingml/2006/chart">
														<c:chart xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart"
														  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
														  r:id="{generate-id(.)}"/>
													</a:graphicData>
												</a:graphic>
											</xdr:graphicFrame>
											<xdr:clientData/>
										</xdr:absoluteAnchor>
									</xsl:when>
								</xsl:choose>
								<!--Fix for the defect  2229985 -->
							</xsl:when>
						</xsl:choose>
            <!--</xsl:if>-->
          </xsl:when>
          <!-- insert picture -->
          <xsl:when
            test="draw:image[not(name(parent::node()/parent::node()) = 'draw:g' ) and not(parent::node()/draw:object)]">
            <!-- Code added by vijayeta, fix for the bug 1806778
				  check image is of type .svm, as OOX does not support image of type .svm
				  Date:23rd Oct '07-->
						<!--Vijayeta,SP2,Pictures-->
						<!--<xsl:variable name ="imageExt">
              <xsl:value-of select ="substring-after(draw:image/@xlink:href,'Pictures/')"/>
								</xsl:variable>-->
						<xsl:variable name ="imageExt">
							<xsl:value-of select ="substring-after(draw:image/@xlink:href,'/')"/>
            </xsl:variable>
						<!--Vijayeta,SP2,Pictures,End-->
            <xsl:if test ="not(contains($imageExt,'.svm'))">
							<!--Vijayeta,SP2,Pictures-->
							<xsl:choose>
								<xsl:when test="(@table:end-cell-address) and (@table:end-x) and (@table:end-y) ">
              <xdr:twoCellAnchor>
                <xsl:call-template name="SetPosition"/>
                <xdr:pic>
                  <xdr:nvPicPr>
                    <xdr:cNvPr>
                      <xsl:attribute name="id">
                        <xsl:value-of select="position()"/>
                      </xsl:attribute>
                      <xsl:attribute name="name">
                        <xsl:value-of select="@draw:name"/>
                      </xsl:attribute>
                      <xsl:attribute name="descr">
                        <xsl:value-of select="@draw:name"/>
                      </xsl:attribute>
                    </xdr:cNvPr>
                    <xdr:cNvPicPr>
                      <a:picLocks noChangeAspect="0"/>
                    </xdr:cNvPicPr>
                  </xdr:nvPicPr>
                  <xdr:blipFill>
                    <a:blip
                      xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships">
                      <xsl:choose>
                        <!-- embeded picture -->
														<!--Code Changes by: Vijayeta,SP2,Pictures
														    Desc           : Ods By Sp2 has @xlink:href=media/image1.jpeg and not @xlink:href=Pictures/image1.jpeg -->
														<!--<xsl:when test="starts-with(draw:image/@xlink:href, 'Pictures/')">-->
														<xsl:when test="starts-with(draw:image/@xlink:href, 'Pictures/') or starts-with(draw:image/@xlink:href, 'media/')">
                          <xsl:attribute name="r:embed">
                            <xsl:value-of select="generate-id()"/>
                          </xsl:attribute>
                        </xsl:when>
                        <!-- linked picture -->
                        <xsl:otherwise>
                          <xsl:attribute name="r:link">
                            <xsl:value-of select="generate-id()"/>
                          </xsl:attribute>
                        </xsl:otherwise>
                      </xsl:choose>
                    </a:blip>
                <!--Image Cropping-->
                <xsl:variable name="graphicStyleName">
                  <xsl:value-of select="@draw:style-name"/>
                </xsl:variable>
                <xsl:variable name="imagePath">
                  <xsl:value-of select="draw:image/@xlink:href"/>
                </xsl:variable>
                <xsl:for-each select="document('content.xml')/office:document-content/office:automatic-styles/style:style[@style:name=$graphicStyleName]">
                  <xsl:if test="style:graphic-properties/@fo:clip">
                    <xsl:variable name="cropValue">
                      <xsl:value-of select="style:graphic-properties/@fo:clip"/>
                    </xsl:variable>
                    <xsl:variable name="var_Top">
                      <xsl:value-of select="substring-before(substring-after($cropValue,'rect('),' ')"/>
                    </xsl:variable>
                    <xsl:variable name="var_Right">
                      <xsl:value-of select="substring-before(substring-after(substring-after($cropValue,'rect('),' '),' ')"/>
                    </xsl:variable>
                    <xsl:variable name="var_Bottom">
                      <xsl:value-of select="substring-before(substring-after(substring-after(substring-after($cropValue,'rect('),' '),' '),' ')"/>
                    </xsl:variable>
                    <xsl:variable name="var_Left">
                      <xsl:value-of select="substring-before(substring-after(substring-after(substring-after(substring-after($cropValue,'rect('),' '),' '),' '),')')"/>
                    </xsl:variable>
                    <xsl:choose>
                      <xsl:when test="contains($var_Top,'in')">
                    <xsl:if test="not($var_Top='0in' and $var_Right='0in' and $var_Bottom='0in' and $var_Left='0in')">
                      <a:srcRect>
                            <xsl:value-of select ="concat('image-properties:',$imagePath,':',substring-before($var_Top,'in'),':',substring-before($var_Right,'in'),':',substring-before($var_Bottom,'in'),':',substring-before($var_Left,'in'),':','in')"/>
                      </a:srcRect>
                    </xsl:if>
                      </xsl:when>
                      <xsl:when test="contains($var_Top,'cm')">
                        <xsl:if test="not($var_Top='0cm' and $var_Right='0cm' and $var_Bottom='0cm' and $var_Left='0cm')">
                          <a:srcRect>
                            <xsl:value-of select ="concat('image-properties:',$imagePath,':',substring-before($var_Top,'cm'),':',substring-before($var_Right,'cm'),':',substring-before($var_Bottom,'cm'),':',substring-before($var_Left,'cm'),':','cm')"/>
                          </a:srcRect>
                        </xsl:if>
                      </xsl:when>
                    </xsl:choose>
                  </xsl:if>
                </xsl:for-each>
                <!--End-->
                    <a:stretch>
                      <a:fillRect/>
                    </a:stretch>
                  </xdr:blipFill>
                  <xsl:call-template name="InsertFrameProperties"/>
                </xdr:pic>
                <xdr:clientData/>
              </xdr:twoCellAnchor>
								</xsl:when>
								<xsl:when test="not(@table:end-cell-address) and not(@table:end-x) and not(@table:end-y) and (@style:rel-width) and (@style:rel-height)">
									<xdr:oneCellAnchor>
										<xsl:call-template name="SetPositionSP2"/>
										<xdr:pic>
											<xdr:nvPicPr>
												<xdr:cNvPr>
													<xsl:attribute name="id">
														<xsl:value-of select="position()"/>
													</xsl:attribute>
													<xsl:attribute name="name">
														<xsl:value-of select="@draw:name"/>
													</xsl:attribute>
													<xsl:attribute name="descr">
														<xsl:value-of select="@draw:name"/>
													</xsl:attribute>
												</xdr:cNvPr>
												<xdr:cNvPicPr>
													<a:picLocks noChangeAspect="0"/>
												</xdr:cNvPicPr>
											</xdr:nvPicPr>
											<xdr:blipFill>
												<a:blip
												  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships">
													<xsl:choose>
														<!-- embeded picture -->
														<!--Code Changes by: Vijayeta,SP2,Pictures
														    Desc           : Ods By Sp2 has @xlink:href=media/image1.jpeg and not @xlink:href=Pictures/image1.jpeg -->
														<!--<xsl:when test="starts-with(draw:image/@xlink:href, 'Pictures/')">-->
														<xsl:when test="starts-with(draw:image/@xlink:href, 'Pictures/') or starts-with(draw:image/@xlink:href, 'media/')">
															<xsl:attribute name="r:embed">
																<xsl:value-of select="generate-id()"/>
															</xsl:attribute>
														</xsl:when>
														<!-- linked picture -->
														<xsl:otherwise>
															<xsl:attribute name="r:link">
																<xsl:value-of select="generate-id()"/>
															</xsl:attribute>
														</xsl:otherwise>
													</xsl:choose>
												</a:blip>
												<!--Image Cropping-->
												<xsl:variable name="graphicStyleName">
													<xsl:value-of select="@draw:style-name"/>
												</xsl:variable>
												<xsl:variable name="imagePath">
													<xsl:value-of select="draw:image/@xlink:href"/>
												</xsl:variable>
												<xsl:for-each select="document('content.xml')/office:document-content/office:automatic-styles/style:style[@style:name=$graphicStyleName]">
													<xsl:if test="style:graphic-properties/@fo:clip">
														<xsl:variable name="cropValue">
															<xsl:value-of select="style:graphic-properties/@fo:clip"/>
														</xsl:variable>
														<xsl:variable name="var_Top">
															<xsl:value-of select="substring-before(substring-after($cropValue,'rect('),' ')"/>
														</xsl:variable>
														<xsl:variable name="var_Right">
															<xsl:value-of select="substring-before(substring-after(substring-after($cropValue,'rect('),' '),' ')"/>
														</xsl:variable>
														<xsl:variable name="var_Bottom">
															<xsl:value-of select="substring-before(substring-after(substring-after(substring-after($cropValue,'rect('),' '),' '),' ')"/>
														</xsl:variable>
														<xsl:variable name="var_Left">
															<xsl:value-of select="substring-before(substring-after(substring-after(substring-after(substring-after($cropValue,'rect('),' '),' '),' '),')')"/>
														</xsl:variable>
														<xsl:choose>
															<xsl:when test="contains($var_Top,'in')">
																<xsl:if test="not($var_Top='0in' and $var_Right='0in' and $var_Bottom='0in' and $var_Left='0in')">
																	<a:srcRect>
																		<xsl:value-of select ="concat('image-properties:',$imagePath,':',substring-before($var_Top,'in'),':',substring-before($var_Right,'in'),':',substring-before($var_Bottom,'in'),':',substring-before($var_Left,'in'),':','in')"/>
																	</a:srcRect>
																</xsl:if>
															</xsl:when>
															<xsl:when test="contains($var_Top,'cm')">
																<xsl:if test="not($var_Top='0cm' and $var_Right='0cm' and $var_Bottom='0cm' and $var_Left='0cm')">
																	<a:srcRect>
																		<xsl:value-of select ="concat('image-properties:',$imagePath,':',substring-before($var_Top,'cm'),':',substring-before($var_Right,'cm'),':',substring-before($var_Bottom,'cm'),':',substring-before($var_Left,'cm'),':','cm')"/>
																	</a:srcRect>
																</xsl:if>
															</xsl:when>
														</xsl:choose>
													</xsl:if>
												</xsl:for-each>
												<!--End-->
												<a:stretch>
													<a:fillRect/>
												</a:stretch>
											</xdr:blipFill>
											<xsl:call-template name="InsertFrameProperties"/>
										</xdr:pic>
										<xdr:clientData/>
									</xdr:oneCellAnchor>
								</xsl:when>
							</xsl:choose>
							<!--Vijayeta,SP2,Pictures-->
            </xsl:if>
			  <xsl:if test ="contains($imageExt,'.svm')">
				  <!-- warn Data or Images copied from other applications will be lost and not converted. -->
				  <xsl:message terminate="no">translation.odf2oox.imageFromOtherApplication</xsl:message>
			  </xsl:if>
            <!--End of Code added by vijayeta, for the bug 1806778 -->
          </xsl:when>
          <!-- insert text-box -->
          <xsl:when test="draw:text-box">
						<xsl:choose>
							<xsl:when test="(@table:end-cell-address) and (@table:end-x) and (@table:end-y) ">
            <xdr:twoCellAnchor>
              <xsl:call-template name="SetPosition"/>
              <xdr:sp macro="" textlink="">
                <xdr:nvSpPr>
                  <xdr:cNvPr id="{position()}" name="{concat('TextBox ',position())}">
                    <!-- Code added by vijayeta, Fix for the bug 1760182, date:23rd Oct '07
		                     If the text box has Hyperlink-->
                    <xsl:if test ="draw:text-box/text:p/text:a">
		      <!-- warn Hyperlink applied to whole text box when converted to XLSX.-->
		      <xsl:message terminate="no">translation.odf2oox.hyperlinkTextbox</xsl:message>
                      <xsl:for-each select ="draw:text-box/text:p/text:a">
                        <xsl:variable name ="hlinkPos">
                          <xsl:value-of select ="generate-id(.)"/>
                        </xsl:variable>
                        <a:hlinkClick>
                          <xsl:if test="@xlink:href[ contains(.,'http://') or contains(.,'mailto:')]">
                            <xsl:attribute name="r:id">
                              <xsl:value-of select ="concat('mailLinkTextBox',$hlinkPos)"/>
                            </xsl:attribute>
                          </xsl:if>
                          <xsl:if test="not(@xlink:href[ contains(.,'http://') or contains(.,'mailto:')]) and @xlink:href[ contains(.,':') or contains(.,'../')]">
                            <xsl:attribute name="r:id">
                              <xsl:value-of select="concat('fileTextBox',$hlinkPos)"/>
                            </xsl:attribute>
                          </xsl:if>
                        </a:hlinkClick>
                      </xsl:for-each>
                    </xsl:if>
                    <!--End of Code added by vijayeta, Fix for the bug 1760182, date:23rd Oct '07-->
                  </xdr:cNvPr>
                  <xdr:cNvSpPr txBox="1"/>
                </xdr:nvSpPr>
                <xsl:call-template name="InsertFrameProperties"/>
                <!--RefNo-1:Added an if condition to chk for the presence of text-->
                <xsl:if test="draw:text-box//text:p">
                <xdr:txBody>
                  <xsl:call-template name="InsertTextBoxProperties"/>
                  <a:lstStyle/>
                  <!-- insert text -->
                  <xsl:apply-templates mode="text-box"/>
                </xdr:txBody>
               </xsl:if> 
              </xdr:sp>
              <xdr:clientData/>
            </xdr:twoCellAnchor>
          </xsl:when>
							<xsl:when test="not(@table:end-cell-address) and not(@table:end-x) and not(@table:end-y)">
								<xdr:oneCellAnchor>
									<xsl:call-template name="SetPositionSP2"/>
									<xdr:sp macro="" textlink="">
										<xdr:nvSpPr>
											<xdr:cNvPr id="{position()}" name="{concat('TextBox ',position())}">
												<!-- Code added by vijayeta, Fix for the bug 1760182, date:23rd Oct '07
													 If the text box has Hyperlink-->
												<xsl:if test ="draw:text-box/text:p/text:a">
													<!-- warn Hyperlink applied to whole text box when converted to XLSX.-->
													<xsl:message terminate="no">translation.odf2oox.hyperlinkTextbox</xsl:message>
													<xsl:for-each select ="draw:text-box/text:p/text:a">
														<xsl:variable name ="hlinkPos">
															<xsl:value-of select ="generate-id(.)"/>
														</xsl:variable>
														<a:hlinkClick>
															<xsl:if test="@xlink:href[ contains(.,'http://') or contains(.,'mailto:')]">
																<xsl:attribute name="r:id">
																	<xsl:value-of select ="concat('mailLinkTextBox',$hlinkPos)"/>
																</xsl:attribute>
															</xsl:if>
															<xsl:if test="not(@xlink:href[ contains(.,'http://') or contains(.,'mailto:')]) and @xlink:href[ contains(.,':') or contains(.,'../')]">
																<xsl:attribute name="r:id">
																	<xsl:value-of select="concat('fileTextBox',$hlinkPos)"/>
																</xsl:attribute>
															</xsl:if>
														</a:hlinkClick>
													</xsl:for-each>
												</xsl:if>
												<!--End of Code added by vijayeta, Fix for the bug 1760182, date:23rd Oct '07-->
											</xdr:cNvPr>
											<xdr:cNvSpPr txBox="1"/>
										</xdr:nvSpPr>
										<xsl:call-template name="InsertFrameProperties"/>
										<!--RefNo-1:Added an if condition to chk for the presence of text-->
										<xsl:if test="draw:text-box//text:p">
											<xdr:txBody>
												<xsl:call-template name="InsertTextBoxProperties"/>
												<a:lstStyle/>
												<!-- insert text -->
												<xsl:apply-templates mode="text-box"/>
											</xdr:txBody>
										</xsl:if>
									</xdr:sp>
									<xdr:clientData/>
								</xdr:oneCellAnchor>
							</xsl:when>
						</xsl:choose>
					</xsl:when>
        </xsl:choose>
      </xsl:for-each>
    </xdr:wsDr>

  </xsl:template>

  <!-- Insert Position of Drawing -->
  <xsl:template name="SetPosition">
    <xsl:variable name="InsertStartColumn">
      <xsl:call-template name="InsertStartColumn"/>
    </xsl:variable>
    <xsl:variable name="InsertStartColumnOffset">
      <xsl:call-template name="InsertStartColumnOffset"/>
    </xsl:variable>
    <xsl:variable name="InsertStartRow">
      <xsl:call-template name="InsertStartRow"/>
    </xsl:variable>
    <xsl:variable name="InsertStartRowOffset">
      <xsl:call-template name="InsertStartRowOffset"/>
    </xsl:variable>
    <xsl:variable name="InsertEndColumn">
      <xsl:call-template name="InsertEndColumn"/>
    </xsl:variable>
    <xsl:variable name="InsertEndColumnOffset">
      <xsl:call-template name="InsertEndColumnOffset"/>
    </xsl:variable>
    <xsl:variable name="InsertEndRow">
      <xsl:call-template name="InsertEndRow"/>
    </xsl:variable>
    <xsl:variable name="InsertEndRowOffset">
      <xsl:call-template name="InsertEndRowOffset"/>
    </xsl:variable>

    <xdr:from>
      <xdr:col>
        <xsl:choose>
          <xsl:when
            test="key('style', @draw:style-name)/style:graphic-properties/@draw:transform!= '' or @draw:transform!= ''">
            <xsl:value-of select="$InsertStartColumn - ($InsertEndColumn - $InsertStartColumn) "/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$InsertStartColumn"/>
          </xsl:otherwise>
        </xsl:choose>
      </xdr:col>
      <xdr:colOff>
        <xsl:value-of select="$InsertStartColumnOffset"/>
      </xdr:colOff>
      <xdr:row>
        <xsl:choose>
          <xsl:when
            test="key('style', @draw:style-name)/style:graphic-properties/@draw:transform!= '' or @draw:transform!= ''">
            <xsl:value-of select="$InsertStartRow - ($InsertEndRow - $InsertStartRow)"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$InsertStartRow"/>
          </xsl:otherwise>
        </xsl:choose>
      </xdr:row>
      <xdr:rowOff>
        <xsl:value-of select="$InsertStartRowOffset"/>
      </xdr:rowOff>
    </xdr:from>
    <xdr:to>
      <xdr:col>

        <!-- To do  (check roundtrip Diagrammtypen.xlsx)-->
        <xsl:choose>
          <xsl:when
            test="key('style', @draw:style-name)/style:graphic-properties/@draw:transform!= '' or @draw:transform!= '' and $InsertStartColumn != '' and $InsertStartColumn != 'NaN'">
            <xsl:value-of select="$InsertStartColumn"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:choose>
              <xsl:when test="$InsertEndColumn != '' and $InsertEndColumn != 'NaN'">
                <xsl:value-of select="$InsertEndColumn"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$InsertStartColumn"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:otherwise>
        </xsl:choose>

      </xdr:col>
      <xdr:colOff>
        <xsl:value-of select="$InsertEndColumnOffset"/>
      </xdr:colOff>
      <xdr:row>
        <xsl:choose>
          <xsl:when
            test="key('style', @draw:style-name)/style:graphic-properties/@draw:transform!= '' or @draw:transform!= ''">
            <xsl:value-of select="$InsertStartRow"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$InsertEndRow"/>
          </xsl:otherwise>
        </xsl:choose>
      </xdr:row>
      <xdr:rowOff>
        <xsl:value-of select="$InsertEndRowOffset"/>
      </xdr:rowOff>
    </xdr:to>
  </xsl:template>

  <!-- Insert top left corner col number -->
  <xsl:template name="InsertStartColumn">
    <xsl:choose>
      <!-- when anchor is to cell -->
      <xsl:when
        test="parent::node()[name() = 'table:table-cell' or name() = 'table:covered-table-cell' ]">
        <xsl:for-each select="parent::node()">
          <xsl:variable name="position">
            <xsl:value-of
              select="count(preceding-sibling::table:table-cell) + count(preceding-sibling::table:covered-table-cell) + 1"
            />
          </xsl:variable>
          <xsl:variable name="number">
            <xsl:for-each
              select="parent::node()/child::node()[name() = 'table:table-cell' or name() = 'table:covered-table-cell' ][1]">
              <xsl:call-template name="GetColNumber">
                <xsl:with-param name="position" select="$position"/>
              </xsl:call-template>
            </xsl:for-each>
          </xsl:variable>
          <xsl:value-of select="$number - 1"/>
        </xsl:for-each>
      </xsl:when>
      <!-- when anchor is to page -->
			<!--When Within a Chart-->
			<xsl:when test ="parent::node()[name() = 'draw:g']">
				<xsl:choose >
					<xsl:when
			       test="./parent::node()/parent::node()[name() = 'table:table-cell' or name() = 'table:covered-table-cell' ]">
						<xsl:for-each select="./parent::node()/parent::node()">
							<xsl:variable name="position">
								<xsl:value-of
								  select="count(preceding-sibling::table:table-cell) + count(preceding-sibling::table:covered-table-cell) + 1"
            />
							</xsl:variable>
							<xsl:variable name="number">
								<xsl:for-each
								  select="parent::node()/child::node()[name() = 'table:table-cell' or name() = 'table:covered-table-cell' ][1]">
									<xsl:call-template name="GetColNumber">
										<xsl:with-param name="position" select="$position"/>
									</xsl:call-template>
								</xsl:for-each>
							</xsl:variable>
							<xsl:value-of select="$number - 1"/>
						</xsl:for-each>
					</xsl:when>
					<xsl:when
			       test="not(./parent::node()/parent::node()[name() = 'table:table-cell' or name() = 'table:covered-table-cell' ]) and ancestor::table:shapes">
						<xsl:text>1</xsl:text>
					</xsl:when>
				</xsl:choose>				
			</xsl:when>
			<!--When Within a Chart,End-->
			<!--Vijayeta,SP2-->
			<!--<xsl:when test="parent::node()[name() = 'table:shapes']">-->
			<xsl:when test ="not(parent::node()[name() = 'draw:g']) and ancestor::table:shapes">
        <xsl:text>1</xsl:text>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- Insert top left corner row number -->
  <xsl:template name="InsertStartRow">
    <xsl:choose>
      <!-- when anchor is to cell -->
      <xsl:when
        test="parent::node()[name() = 'table:table-cell' or name() = 'table:covered-table-cell' ]">
        <!-- get parent table:table-row id -->
        <xsl:variable name="rowId">
          <xsl:value-of select="generate-id(ancestor::table:table-row)"/>
        </xsl:variable>
        <!-- go to first table:table-row-->
        <xsl:for-each select="ancestor::table:table/descendant::table:table-row[1]">
          <xsl:variable name="number">
            <xsl:call-template name="GetRowNumber">
              <xsl:with-param name="rowId" select="$rowId"/>
              <xsl:with-param name="tableId" select="generate-id(ancestor::table:table)"/>
            </xsl:call-template>
          </xsl:variable>
          <xsl:value-of select="$number - 1"/>
        </xsl:for-each>
      </xsl:when>
			<!--When Text Box Within a Chart-->
			<xsl:when test ="parent::node()[name() = 'draw:g']">
				<xsl:choose>
					<xsl:when
			       test="./parent::node()/parent::node()[name() = 'table:table-cell' or name() = 'table:covered-table-cell' ]">
						<xsl:variable name="rowId">
							<xsl:value-of select="generate-id(ancestor::table:table-row)"/>
						</xsl:variable>
						<!-- go to first table:table-row-->
						<xsl:for-each select="ancestor::table:table/descendant::table:table-row[1]">
							<xsl:variable name="number">
								<xsl:call-template name="GetRowNumber">
									<xsl:with-param name="rowId" select="$rowId"/>
									<xsl:with-param name="tableId" select="generate-id(ancestor::table:table)"/>
								</xsl:call-template>
							</xsl:variable>
							<xsl:value-of select="$number - 1"/>
						</xsl:for-each>
					</xsl:when>
					<xsl:when
			       test="not(./parent::node()/parent::node()[name() = 'table:table-cell' or name() = 'table:covered-table-cell' ]) and ancestor::table:shapes">
						<xsl:text>31</xsl:text>
					</xsl:when>

					</xsl:choose>
			</xsl:when>
			<!--When Text Box Within a Chart,End-->
			<!--Vijayeta,SP2-->
      <!-- when anchor is to page -->
			<!--<xsl:when test="parent::node()[name() = 'table:shapes']">-->
			<xsl:when test ="not(parent::node()[name() = 'draw:g']) and ancestor::table:shapes">
        <xsl:text>31</xsl:text>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- Insert bottom right corner col number -->
  <xsl:template name="InsertEndColumn">
    <xsl:choose>
      <!-- when anchor is to cell -->
      <xsl:when
        test="parent::node()[name() = 'table:table-cell' or name() = 'table:covered-table-cell' ]">
        <xsl:variable name="number">
          <xsl:call-template name="GetColNum">
            <xsl:with-param name="cell" select="substring-after(@table:end-cell-address,'.')"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:value-of select="$number - 1"/>
      </xsl:when>
      <!-- when anchor is to page -->
			<!--Vijayeta,SP2-->
			<!--<xsl:when test="parent::node()[name() = 'table:shapes']">-->
			<xsl:when test ="ancestor::table:shapes">
        <xsl:text>5</xsl:text>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- Insert bottom corner row number -->
  <xsl:template name="InsertEndRow">
    <xsl:choose>
      <!-- when anchor is to cell -->
      <xsl:when
        test="parent::node()[name() = 'table:table-cell' or name() = 'table:covered-table-cell' ]">
        <xsl:variable name="number">
          <xsl:call-template name="GetRowNum">
            <xsl:with-param name="cell" select="substring-after(@table:end-cell-address,'.')"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:value-of select="$number - 1"/>
      </xsl:when>
      <!-- when anchor is to page -->
			<!--Vijayeta,SP2-->
			<!--<xsl:when test="parent::node()[name() = 'table:shapes']">-->
			<xsl:when test ="ancestor::table:shapes">
        <xsl:text>46</xsl:text>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- Horizontal offset of top left corner -->
  <xsl:template name="InsertStartColumnOffset">
    <xsl:choose>
      <!-- when anchor is to cell -->
      <xsl:when
        test="parent::node()[name() = 'table:table-cell' or name() = 'table:covered-table-cell' ]">
        <xsl:call-template name="emu-measure">
          <xsl:with-param name="length" select="@svg:x"/>
        </xsl:call-template>
      </xsl:when>
			<!--When Text Box Within a Chart-->
			<xsl:when test ="parent::node()[name() = 'draw:g']">
				<xsl:choose>
					<xsl:when
			       test="./parent::node()/parent::node()[name() = 'table:table-cell' or name() = 'table:covered-table-cell' ]">
						<xsl:call-template name="emu-measure">
							<xsl:with-param name="length" select="@svg:x"/>
						</xsl:call-template>
					</xsl:when>
					<xsl:when
			       test="not(./parent::node()/parent::node()[name() = 'table:table-cell' or name() = 'table:covered-table-cell' ]) and ancestor::table:shapes">
						<xsl:text>714375</xsl:text>
					</xsl:when>
				</xsl:choose>
			</xsl:when>
			<!--When Text Box Within a Chart,End-->
      <!-- when anchor is to page -->
			<!--<xsl:when test="parent::node()[name() = 'table:shapes']">-->
			<xsl:when test ="not(parent::node()[name() = 'draw:g']) and ancestor::table:shapes">
        <xsl:text>714375</xsl:text>
      </xsl:when>
			<!--Vijayeta,SP2-->
    </xsl:choose>
  </xsl:template>

  <!-- Vertical offset of top left corner -->
  <xsl:template name="InsertStartRowOffset">
    <xsl:choose>
      <!-- when anchor is to cell -->
      <xsl:when
        test="parent::node()[name() = 'table:table-cell' or name() = 'table:covered-table-cell' ]">
        <xsl:call-template name="emu-measure">
          <xsl:with-param name="length" select="@svg:y"/>
        </xsl:call-template>
      </xsl:when>
			<!--When Text Box Within a Chart-->
			<xsl:when test ="parent::node()[name() = 'draw:g']">
				<xsl:choose>
					<xsl:when
			       test="./parent::node()/parent::node()[name() = 'table:table-cell' or name() = 'table:covered-table-cell' ]">
					<xsl:call-template name="emu-measure">
						<xsl:with-param name="length" select="@svg:y"/>
					</xsl:call-template>
					</xsl:when>
					<xsl:when
			       test="not(./parent::node()/parent::node()[name() = 'table:table-cell' or name() = 'table:covered-table-cell' ]) and ancestor::table:shapes">
						<xsl:text>104775</xsl:text>
					</xsl:when>
					</xsl:choose>
			</xsl:when>
      <!-- when anchor is to page -->
			<!--Vijayeta,SP2-->
			<!--<xsl:when test="parent::node()[name() = 'table:shapes']">-->
			<xsl:when test ="not(parent::node()[name() = 'draw:g']) and ancestor::table:shapes">
        <xsl:text>104775</xsl:text>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- Horizontal offset of  bottom right corner -->
  <xsl:template name="InsertEndColumnOffset">
    <xsl:choose>
      <!-- when anchor is to cell -->
      <xsl:when
        test="parent::node()[name() = 'table:table-cell' or name() = 'table:covered-table-cell' ]">
        <xsl:call-template name="emu-measure">
          <xsl:with-param name="length" select="@ table:end-x"/>
        </xsl:call-template>
      </xsl:when>
      <!-- when anchor is to page -->
			<!--Vijayeta,SP2-->
			<!--<xsl:when test="parent::node()[name() = 'table:shapes']">-->
			<xsl:when test ="ancestor::table:shapes">
        <xsl:text>447675</xsl:text>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- Vertical offset of  bottom right corner -->
  <xsl:template name="InsertEndRowOffset">
    <xsl:choose>
      <!-- when anchor is to cell -->
      <xsl:when
        test="parent::node()[name() = 'table:table-cell' or name() = 'table:covered-table-cell' ]">
        <xsl:call-template name="emu-measure">
          <xsl:with-param name="length" select="@ table:end-y"/>
        </xsl:call-template>
      </xsl:when>
      <!-- when anchor is to page -->
			<!--Vijayeta,SP2-->
			<!--<xsl:when test="parent::node()[name() = 'table:shapes']">-->
			<xsl:when test ="ancestor::table:shapes">
        <xsl:text>104775</xsl:text>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!--image border width and line style-->
  <xsl:template name="InsertFrameProperties">

    <xdr:spPr>

      <a:xfrm>
        <xsl:if
          test="key('style', @draw:style-name)/style:graphic-properties/@draw:transform!= '' or @draw:transform!= ''">
          <xsl:attribute name="flipV">
            <xsl:text>1</xsl:text>
          </xsl:attribute>
        </xsl:if>
        <xsl:if
          test="key('style', @draw:style-name)/style:graphic-properties/@style:mirror='horizontal'">
          <xsl:attribute name="flipH">
            <xsl:text>1</xsl:text>
          </xsl:attribute>
        </xsl:if>
         <!--Code added by Sateesh, fix for the bug 1840212
             (Excel crashes when using the context menu of the picture (e.g. Ungroup or Edit Picture)
             Date:29th Jan '08-->
        <a:off>
          <xsl:attribute name="x">
            <xsl:call-template name="emu-measure">
              <xsl:with-param name="length" select="@svg:x" />
            </xsl:call-template>
          </xsl:attribute>
          <xsl:attribute name="y">
            <xsl:call-template name="emu-measure">
              <xsl:with-param name="length" select="@svg:y" />
            </xsl:call-template>
          </xsl:attribute>
        </a:off>
        <a:ext>
          <xsl:attribute name="cx">
            <xsl:call-template name="emu-measure">
              <xsl:with-param name="length" select="@svg:width" />
            </xsl:call-template>
          </xsl:attribute>
          <xsl:attribute name="cy">
            <xsl:call-template name="emu-measure">
              <xsl:with-param name="length" select="@svg:height" />
            </xsl:call-template>
          </xsl:attribute>
        </a:ext>
        <!--End-->
      </a:xfrm>

      <a:prstGeom prst="rect">
        <a:avLst/>
      </a:prstGeom>

      <xsl:for-each select="key('style', @draw:style-name)/style:graphic-properties">
        <xsl:call-template name="InsertDrawingFill"/>
        <xsl:call-template name="InsertDrawingBorder"/>
      </xsl:for-each>

    </xdr:spPr>

  </xsl:template>

  <xsl:template match="text:p" mode="text-box">
    <xsl:variable name="tekst">
      <xsl:value-of select="."/>
    </xsl:variable>

    <a:p>
      <xsl:call-template name="InsertParagraphProperties"/>

      <xsl:choose>
        <!-- empty line -->
        <xsl:when test="$tekst = '' and not(text:s) and not(text:span)">
          <xsl:call-template name="InsertEndPara"/>
        </xsl:when>

        <!-- text:p without text:span -->
        <xsl:when test="not(text:span)">
          <a:r>
            <a:rPr lang="en-EN" sz="1200">
              <xsl:for-each select="key('style',@text:style-name)/style:text-properties">
				  <xsl:call-template name="InsertRunProperties"/>
              </xsl:for-each>

              <!-- default font face -->
              <xsl:if test="not(key('style',@text:style-name)/style:text-properties)">
                <a:latin typeface="Times New Roman" charset="0"/>
              </xsl:if>

            </a:rPr>
            <a:t>
              <xsl:apply-templates mode="text-box"/>
            </a:t>
          </a:r>
        </xsl:when>

        <xsl:otherwise>
          <xsl:apply-templates mode="text-box"/>
        </xsl:otherwise>
      </xsl:choose>
    </a:p>
  </xsl:template>

  <xsl:template match="text:span" mode="text-box">

    <xsl:variable name="tekst">
      <xsl:value-of select="."/>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="$tekst != '' or text:s">
        <a:r>
          <a:rPr lang="en-EN" sz="1200">
            <xsl:for-each select="key('style',@text:style-name)/style:text-properties">
				<xsl:call-template name="InsertRunProperties"/>
            </xsl:for-each>
          </a:rPr>
          <a:t>
            <xsl:apply-templates mode="text-box"/>
          </a:t>
        </a:r>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="InsertEndPara"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="text()[parent::text:p and parent::node()/text:span]" mode="text-box">
    <a:r>
      <a:rPr lang="en-EN" sz="1200">
        <xsl:for-each select="key('style',@text:style-name)/style:text-properties">
			<xsl:call-template name="InsertRunProperties"/>
        </xsl:for-each>

        <!-- default font face -->
        <xsl:if test="not(key('style',@text:style-name)/style:text-properties)">
          <a:latin typeface="Times New Roman" charset="0"/>
        </xsl:if>

      </a:rPr>
      <a:t>
        <xsl:value-of select="."/>
      </a:t>
    </a:r>
  </xsl:template>

  <xsl:template match="text()" mode="text-box">
        <xsl:value-of select="."/>
   </xsl:template>

  <xsl:template match="text:s" mode="text-box">
    <xsl:call-template name="InsertRepeatSpaces">
      <xsl:with-param name="nr">
        <xsl:text>1</xsl:text>
      </xsl:with-param>
      <xsl:with-param name="repeat">
        <xsl:value-of select="@text:c"/>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="InsertEndPara">
    <a:endParaRPr lang="en-EN" sz="1400"/>
  </xsl:template>

	<!--Vijayeta,SP2-->
  <xsl:template name="InsertRunProperties">
   <!-- font-size -->
    <xsl:if test="@fo:font-size">
	<xsl:attribute name="sz">					
				<xsl:variable name ="size">
					<xsl:call-template name="point-measure">
						<xsl:with-param name="length" select="@fo:font-size"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:value-of select="number(round($size)* 100)"/>
				</xsl:attribute>				
 </xsl:if>

    <!-- bold -->
    <xsl:if test="@fo:font-weight = 'bold' ">
      <xsl:attribute name="b">
        <xsl:text>1</xsl:text>
      </xsl:attribute>
    </xsl:if>

    <!-- italic -->
    <xsl:if test="@fo:font-style = 'italic' ">
      <xsl:attribute name="i">
        <xsl:text>1</xsl:text>
      </xsl:attribute>
    </xsl:if>

    <!-- strike-through -->
    <xsl:choose>
      <xsl:when test="@style:text-line-through-type = 'solid' ">
        <xsl:attribute name="strike">
          <xsl:text>sngStrike</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <xsl:when test="contains(@style:text-line-through-type,'double' )">
        <xsl:attribute name="strike">
          <xsl:text>dblStrike</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <!-- style:text-line-through-style-->
      <xsl:when test="@style:text-line-through-style = 'solid' ">
        <xsl:attribute name="strike">
          <xsl:text>sngStrike</xsl:text>
        </xsl:attribute>
      </xsl:when>
    </xsl:choose>

    <!-- font underline-->
    <xsl:choose>
      <!-- double-->
      <xsl:when
        test="@style:text-underline-style = 'solid' and contains(@style:text-underline-type, 'double' )">
        <xsl:attribute name="u">
          <xsl:text>dbl</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <!-- bold -->
      <xsl:when
        test="@style:text-underline-style  = 'solid' and contains(@style:text-underline-width, 'bold' )">
        <xsl:attribute name="u">
          <xsl:text>heavy</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <!-- single-->
      <xsl:when
        test="@style:text-underline-style = 'solid' and contains(@style:text-underline-width, 'auto' )">
        <xsl:attribute name="u">
          <xsl:text>sng</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <!-- dotted -->
      <xsl:when
        test="@style:text-underline-style = 'dotted' and contains(@style:text-underline-width, 'auto' )">
        <xsl:attribute name="u">
          <xsl:text>dotted</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <!-- bold dotted -->
      <xsl:when
        test="@style:text-underline-style = 'dotted' and contains(@style:text-underline-width, 'bold' )">
        <xsl:attribute name="u">
          <xsl:text>dottedHeavy</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <!-- dash -->
      <xsl:when
        test="@style:text-underline-style = 'dash' and contains(@style:text-underline-width, 'auto' )">
        <xsl:attribute name="u">
          <xsl:text>dash</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <!-- bold dash-->
      <xsl:when
        test="@style:text-underline-style = 'dash' and contains(@style:text-underline-width, 'bold' )">
        <xsl:attribute name="u">
          <xsl:text>dashHeavy</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <!-- dash long -->
      <xsl:when
        test="@style:text-underline-style = 'long-dash' and contains(@style:text-underline-width, 'auto' )">
        <xsl:attribute name="u">
          <xsl:text>dashLong</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <!-- bold dash long -->
      <xsl:when
        test="@style:text-underline-style = 'long-dash' and contains(@style:text-underline-width, 'bold' )">
        <xsl:attribute name="u">
          <xsl:text>dashLongHeavy</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <!-- dot dash -->
      <xsl:when
        test="@style:text-underline-style = 'dot-dash' and contains(@style:text-underline-width, 'auto' )">
        <xsl:attribute name="u">
          <xsl:text>dotDash</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <!-- bold dot dash -->
      <xsl:when
        test="@style:text-underline-style = 'dot-dash' and contains(@style:text-underline-width, 'bold' )">
        <xsl:attribute name="u">
          <xsl:text>dotDashHeavy</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <!-- dot-dot-dash -->
      <xsl:when
        test="@style:text-underline-style= 'dot-dot-dash' and contains(@style:text-underline-width, 'auto' )">
        <xsl:attribute name="u">
          <xsl:text>dotDotDash</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <!-- bold dot-dot-dash -->
      <xsl:when
        test="@style:text-underline-style= 'dot-dot-dash' and contains(@style:text-underline-width, 'bold' )">
        <xsl:attribute name="u">
          <xsl:text>dotDotDashHeavy</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <!-- double wavy -->
      <xsl:when
        test="contains(@style:text-underline-style, 'wave' ) and contains(@style:text-underline-type, 'double' )">
        <xsl:attribute name="u">
          <xsl:text>wavyDbl</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <!-- wavy -->
      <xsl:when
        test="contains(@style:text-underline-style, 'wave' ) and contains(@style:text-underline-width, 'auto' )">
        <xsl:attribute name="u">
          <xsl:text>wavy</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <!-- bold wavy -->
      <xsl:when
        test="contains(@style:text-underline-style, 'wave' ) and contains(@style:text-underline-width, 'bold' )">
        <xsl:attribute name="u">
          <xsl:text>wavyHeavy</xsl:text>
        </xsl:attribute>
      </xsl:when>
    </xsl:choose>

    <!-- text position -->
    <xsl:if test="@style:text-position">
      <xsl:attribute name="baseline">
        <xsl:choose>
          <!-- superscript -->
          <xsl:when test="substring-before(@style:text-position,' ') = 'super' ">
            <xsl:value-of
              select="number(substring-before(substring-after(@style:text-position,' '), '%' )) * 1000"
            />
          </xsl:when>
          <!-- subscript -->
          <xsl:when test="substring-before(@style:text-position,' ') = 'sub' ">
            <xsl:text>-</xsl:text>
            <xsl:value-of
              select="number(substring-before(substring-after(@style:text-position,' '), '%' )) * 1000"
            />
          </xsl:when>
          <!-- when position is specified like 30% 75% -->
          <xsl:when test="contains(@style:text-position, ' ')">
            <xsl:value-of select="number(substring-before(@style:text-position,'%' )) * 1000"/>
          </xsl:when>
<!--Fixed by : Vijayeta
    Desc     : Crash in _1.1LinearGraph, as attribute baseline='', hence 'otherwise' condition added-->
			<xsl:otherwise>
				<xsl:value-of select="number(translate(@style:text-position,'%','' )) * 1000"/>
			</xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
    </xsl:if>

    <!-- letter kerning-->
    <xsl:if test="style:text-properties/@style:letter-kerning = 'true' ">
      <xsl:attribute name="kern">
        <xsl:value-of select="100"/>
      </xsl:attribute>
    </xsl:if>

    <!--character spacing -->
    <xsl:if test="@fo:letter-spacing">
      <xsl:attribute name="spc">
        <!-- condensed -->
				<!--Vijayeta,SP2-->
				<xsl:variable name ="size">
					<xsl:call-template name="point-measure">
						<xsl:with-param name="length" select="@fo:letter-spacing"/>
					</xsl:call-template>
				</xsl:variable>
				<!--<xsl:if test="substring-before(@fo:letter-spacing, 'cm' )&lt; 0 ">
          <xsl:value-of
            select="format-number(substring-before(@fo:letter-spacing,'cm') * 7200 div 2.54 ,'#')"/>
				</xsl:if>-->
				<xsl:if test="$size &lt; 0 or $size &gt; 0 ">
					<xsl:value-of
					  select="format-number($size * 100 ,'#')"/>
        </xsl:if>
        <!-- expanded -->
				<!--<xsl:if test="substring-before(@fo:letter-spacing, 'cm' ) &gt;= 0">
          <xsl:value-of
					  select="format-number((substring-before(@fo:letter-spacing, 'cm' ) * 72 div 2.54) *100 ,'#')" />
				</xsl:if>-->
				<xsl:if test="$size = 0">
					<xsl:value-of select="$size" />
        </xsl:if>
      </xsl:attribute>
    </xsl:if>

    <!-- font color -->
    <xsl:if test="@fo:color">
      <a:solidFill>
        <a:srgbClr>
          <xsl:attribute name="val">
            <xsl:value-of select="substring-after(@fo:color,'#')"/>
          </xsl:attribute>
        </a:srgbClr>
      </a:solidFill>
    </xsl:if>

    <!-- text shadow -->
    <xsl:if test="@fo:text-shadow != 'none'">
      <a:effectLst>
        <a:outerShdw blurRad="38100" dist="38100" dir="2700000">
          <a:srgbClr val="000000">
            <a:alpha val="43137"/>
          </a:srgbClr>
        </a:outerShdw>
      </a:effectLst>
    </xsl:if>

    <!-- underline color -->
    <!--<xsl:if test="@style:text-underline-color">-->
	  <!--a:srgbclr val=''-->
	  <xsl:if test ="substring-after(@style:text-underline-color,'#')!=''">
      <a:uFill>
        <a:solidFill>
          <a:srgbClr>
            <xsl:attribute name="val">
              <xsl:value-of select="substring-after(@style:text-underline-color,'#')"/>
            </xsl:attribute>
          </a:srgbClr>
        </a:solidFill>
      </a:uFill>
    </xsl:if>

    <!-- font family  (Times New Roman default)-->
    <a:latin typeface="Times New Roman" charset="0">
      <xsl:if test="@fo:font-family">
        <xsl:attribute name="typeface">
          <xsl:value-of select="translate(@fo:font-family, &quot;'&quot;,'')"/>
        </xsl:attribute>
      </xsl:if>
      <!-- Code added by Vijayeta, fix for the bug 1806752, date: 12th Oct '07-->
      <xsl:if test="@style:font-name">
        <xsl:attribute name="typeface">
          <xsl:value-of select="translate(@style:font-name, &quot;'&quot;,'')"/>
        </xsl:attribute>
      </xsl:if>
      <!--End of Code added by Vijayeta, fix for the bug 1806752-->
    </a:latin>

  </xsl:template>

  <xsl:template name="InsertParagraphProperties">
    <xsl:for-each select="key('style',@text:style-name)/style:paragraph-properties">
      <a:pPr>

        <!-- first line indent -->
        <xsl:if test="@fo:text-indent and substring-before(@fo:text-indent,'cm') != 0">
          <xsl:attribute name="indent">
            <xsl:call-template name="emu-measure">
              <xsl:with-param name="length" select="@fo:text-indent"/>
            </xsl:call-template>
          </xsl:attribute>
        </xsl:if>

        <!-- left margin -->
        <xsl:if test="@fo:margin-left and substring-before(@fo:margin-left,'cm') &gt; 0">
          <xsl:attribute name="marL">
            <!--fo:margin-left-->
            <xsl:call-template name="emu-measure">
              <xsl:with-param name="length" select="@fo:margin-left"/>
            </xsl:call-template>
          </xsl:attribute>
        </xsl:if>

        <!-- horizontal align -->
        <xsl:if test="@fo:text-align">
          <xsl:attribute name="algn">
            <xsl:choose>
              <xsl:when test="@fo:text-align = 'center' ">
                <xsl:text>ctr</xsl:text>
              </xsl:when>
              <xsl:when test="@fo:text-align = 'end' ">
                <xsl:text>r</xsl:text>
              </xsl:when>
              <xsl:when test="@fo:text-align = 'justify' ">
                <xsl:text>just</xsl:text>
              </xsl:when>
              <xsl:otherwise>
                <xsl:text>l</xsl:text>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:attribute>
        </xsl:if>

        <!-- line spacing -->
        <!-- if the line spacing is in terms of Percentage, multiply the value with 1000 -->
        <xsl:if
          test="@fo:line-height and substring-before(@fo:line-height,'%') &gt; 0 and not(substring-before(@fo:line-height,'%') = 100)">
          <a:lnSpc>
            <a:spcPct>
              <xsl:attribute name="val">
                <xsl:value-of
                  select="format-number(substring-before(@fo:line-height,'%')* 1000,'#.##') "/>
              </xsl:attribute>
            </a:spcPct>
          </a:lnSpc>
        </xsl:if>
        <!--If the line spacing is in terms of Points,multiply the value with 2835-->
        <xsl:if test="@style:line-spacing and substring-before(@style:line-spacing,'cm') &gt; 0">
          <a:lnSpc>
            <a:spcPts>
              <xsl:attribute name="val">
                <xsl:value-of select="round(substring-before(@style:line-spacing,'cm')* 2835) "/>
              </xsl:attribute>
            </a:spcPts>
          </a:lnSpc>
        </xsl:if>
        <xsl:if
          test="@style:line-height-at-least and substring-before(@style:line-height-at-least,'cm') &gt; 0 ">
          <a:lnSpc>
            <a:spcPts>
              <xsl:attribute name="val">
                <xsl:value-of
                  select="round(substring-before(@style:line-height-at-least,'cm')* 2835) "/>
              </xsl:attribute>
            </a:spcPts>
          </a:lnSpc>
        </xsl:if>

        <!--spacing above -->
        <xsl:if test="@fo:margin-top and substring-before(@fo:margin-top,'cm') &gt; 0 ">
          <a:spcBef>
            <a:spcPts>
              <xsl:attribute name="val">
                <xsl:value-of select="round(substring-before(@fo:margin-top,'cm')* 2835) "/>
              </xsl:attribute>
            </a:spcPts>
          </a:spcBef>
        </xsl:if>

        <!-- spacing below -->
        <xsl:if test="@fo:margin-bottom and substring-before(@fo:margin-bottom,'cm') &gt; 0 ">
          <a:spcAft>
            <a:spcPts>
              <xsl:attribute name="val">
                <xsl:value-of select="round(substring-before(@fo:margin-bottom,'cm')* 2835) "/>
              </xsl:attribute>
            </a:spcPts>
          </a:spcAft>
        </xsl:if>

      </a:pPr>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="InsertTextBoxProperties">
    <a:bodyPr wrap="square" rtlCol="0" anchor="t">
      <xsl:for-each select="key('style',@draw:style-name)/style:graphic-properties">

        <xsl:attribute name="lIns">
          <xsl:choose>
            <xsl:when test="@fo:padding-left">
              <xsl:call-template name="emu-measure">
                <xsl:with-param name="unit" select="'cm'"/>
                <xsl:with-param name="length" select="@fo:padding-left"/>
              </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>0</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>

        <xsl:attribute name="tIns">
          <xsl:choose>
            <xsl:when test="@fo:padding-top">
              <xsl:call-template name="emu-measure">
                <xsl:with-param name="unit" select="'cm'"/>
                <xsl:with-param name="length" select="@fo:padding-top"/>
              </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>0</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>

        <xsl:attribute name="rIns">
          <xsl:choose>
            <xsl:when test="@fo:padding-right">
              <xsl:call-template name="emu-measure">
                <xsl:with-param name="unit" select="'cm'"/>
                <xsl:with-param name="length" select="@fo:padding-right"/>
              </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>0</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>

        <xsl:attribute name="bIns">
          <xsl:choose>
            <xsl:when test="@fo:padding-bottom">
              <xsl:call-template name="emu-measure">
                <xsl:with-param name="unit" select="'cm'"/>
                <xsl:with-param name="length" select="@fo:padding-bottom"/>
              </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>0</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>

      </xsl:for-each>
    </a:bodyPr>

  </xsl:template>

  <xsl:template name="InsertDrawingFill">
    <xsl:param name="chartDirectory"/>
    <xsl:param name="default"/>

    <xsl:choose>
      <!-- background image -->
      <xsl:when test="@draw:fill = 'bitmap' ">
        <xsl:call-template name="InsertBitmapFill">
          <xsl:with-param name="chartDirectory" select="$chartDirectory"/>
        </xsl:call-template>
      </xsl:when>
      <!-- background color-->
      <xsl:when test="not(@draw:fill = 'none' ) and @draw:fill-color">
        <a:solidFill>
          <a:srgbClr>
            <xsl:attribute name="val">
              <xsl:value-of select="substring-after(@draw:fill-color, '#')"/>
            </xsl:attribute>

            <!-- transparency -->
            <xsl:if test="@draw:opacity">
              <a:alpha>
                <xsl:attribute name="val">
                  <xsl:value-of select="substring-before(@draw:opacity,'%' ) * 1000"/>
                </xsl:attribute>
              </a:alpha>
            </xsl:if>

          </a:srgbClr>
        </a:solidFill>
      </xsl:when>
      <xsl:when test="@draw:fill = 'none' ">
        <a:noFill/>
      </xsl:when>
      <xsl:when test="$default = 'solid' "/>
      <xsl:otherwise>
        <a:noFill/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
<!-- Sonata: line style for charts component -->
  <xsl:template name="InsertDrawingBorder">
    <xsl:param name ="parentStyle" />
    <xsl:param name="chartDirectory"/>
    <xsl:variable name="chartStyleDirectory">
      <xsl:choose>
        <xsl:when test="$chartDirectory!=''">
      <xsl:value-of select="concat($chartDirectory,'/styles.xml')"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="'styles.xml'"/>
        </xsl:otherwise>
      </xsl:choose>

    </xsl:variable>
    <a:ln>
      <!-- Border width -->
      <xsl:choose>
        <xsl:when test ="@svg:stroke-width">
          <xsl:attribute name ="w">
            <xsl:call-template name="emu-measure">
              <xsl:with-param name="length" select="@svg:stroke-width"/>
              <xsl:with-param name="unit">emu</xsl:with-param>
            </xsl:call-template>
          </xsl:attribute>
        </xsl:when>
      
      </xsl:choose>
      <!-- Cap type-->
      <xsl:if test ="@draw:stroke-dash">
        <xsl:variable name ="dash" select ="@draw:stroke-dash" />
        <xsl:if test ="document($chartStyleDirectory)/office:document-styles/office:styles/draw:stroke-dash[@draw:name=$dash]">
          <xsl:if test ="document($chartStyleDirectory)/office:document-styles/office:styles/draw:stroke-dash[@draw:name=$dash]/@draw:style='round'">
            <xsl:attribute name ="cap">
              <xsl:value-of select ="'rnd'"/>
            </xsl:attribute>
          </xsl:if>
        </xsl:if>
      </xsl:if>
      <xsl:if test="not(@draw:stroke = 'none' )">
        <a:solidFill>
          <a:srgbClr>
            <xsl:attribute name="val">
              <xsl:choose>
                <xsl:when test="@svg:stroke-color != '' ">
                  <xsl:value-of select="substring-after(@svg:stroke-color, '#')"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:text>000000</xsl:text>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:attribute>
            <xsl:if test="@svg:stroke-opacity != ''">
              <xsl:variable name="transparency">
                <xsl:value-of select="substring-before(@svg:stroke-opacity, '%')"/>
              </xsl:variable>
              <a:alpha>
                <xsl:attribute name="val">
                  <xsl:value-of select="$transparency * 1000"/>
                </xsl:attribute>
              </a:alpha>
            </xsl:if>
          </a:srgbClr>
        </a:solidFill>
      </xsl:if>
      <!-- Dash type-->
      <xsl:choose>
        <xsl:when test ="@draw:stroke='dash'">
          <a:prstDash>
            <xsl:attribute name ="val">
              <xsl:call-template name ="getDashType">
                <xsl:with-param name ="stroke-dash" select ="@draw:stroke-dash" />
                <xsl:with-param name ="chartStyleDirectory" select ="$chartStyleDirectory" />
              </xsl:call-template>
            </xsl:attribute>
          </a:prstDash>
        </xsl:when>
        <xsl:when test ="not(@draw:stroke) and $parentStyle!=''">
          <xsl:for-each select ="document($chartStyleDirectory)/office:document-styles/office:styles/style:style[@style:name = $parentStyle]/style:graphic-properties">
            <xsl:if test="@draw:stroke='dash'">
              <a:prstDash>
                <xsl:attribute name ="val">
                  <xsl:call-template name ="getDashType">
                    <xsl:with-param name ="stroke-dash" select ="@draw:stroke-dash" />
                    <xsl:with-param name ="chartStyleDirectory" select ="$chartStyleDirectory" />
                  </xsl:call-template>
                </xsl:attribute>
              </a:prstDash>
            </xsl:if>
          </xsl:for-each>
        </xsl:when>
      </xsl:choose>
    

    </a:ln>
  </xsl:template>
  <xsl:template name ="getDashType">
    <xsl:param name ="stroke-dash" />
    <xsl:param name="chartStyleDirectory"/>
    <xsl:if test ="document($chartStyleDirectory)/office:document-styles/office:styles/draw:stroke-dash[@draw:name=$stroke-dash]">
      <xsl:for-each select="document($chartStyleDirectory)/office:document-styles/office:styles/draw:stroke-dash[@draw:name=$stroke-dash][1]">
          <xsl:variable name="Unit1">
            <xsl:call-template name="getConvertUnit">
              <xsl:with-param name="length" select="@draw:dots1-length"/>
            </xsl:call-template>
          </xsl:variable>
          <xsl:variable name="Unit2">
            <xsl:call-template name="getConvertUnit">
              <xsl:with-param name="length" select="@draw:dots2-length"/>
            </xsl:call-template>
          </xsl:variable>
          <xsl:variable name="Unit3">
            <xsl:call-template name="getConvertUnit">
              <xsl:with-param name="length" select="@draw:distance"/>
            </xsl:call-template>
          </xsl:variable>
          <xsl:variable name ="dots1" select="@draw:dots1"/>
          <xsl:variable name ="dots1-length" select ="substring-before(@draw:dots1-length, $Unit1)" />
          <xsl:variable name ="dots2" select="@draw:dots2"/>
          <xsl:variable name ="dots2-length" select ="substring-before(@draw:dots2-length, $Unit2)"/>
          <xsl:variable name ="distance" select ="substring-before(@draw:distance, $Unit3)" />



          <xsl:choose>
            <xsl:when test ="($dots1=1) and ($dots2=1)">
              <xsl:choose>
                <xsl:when test ="($dots1-length &lt;= $dot) and ($dots2-length &lt;= $dot)">
                  <xsl:value-of select ="'sysDot'" />
                </xsl:when>
                <xsl:when test ="(($dots1-length &lt;= $dot) and ($dots2-length &lt;= $dash)) or
										 (($dots1-length &lt;= $dash) and ($dots2-length &lt;= $dot)) ">
                  <xsl:value-of select ="'dashDot'" />
                </xsl:when>
                <xsl:when test ="($dots1-length &lt;= $dash) and ($dots2-length &lt;= $dash)">
                  <xsl:value-of select ="'dash'" />
                </xsl:when>
                <xsl:when test ="(($dots1-length &lt;= $dot) and (($dots2-length &gt;= $dash) and ($dots2-length &lt;= $longDash))) or
										 (($dots2-length &lt;= $dot) and (($dots1-length &gt;= $dash) and ($dots1-length &lt;= $longDash))) ">
                  <xsl:value-of select ="'lgDashDot'" />
                </xsl:when>
                <xsl:when test ="(($dots1-length &gt;= $dash) and ($dots1-length &lt;= $longDash)) or 
										 (($dots2-length &gt;= $dash) and ($dots2-length &lt;= $longDash))">
                  <xsl:value-of select ="'lgDash'" />
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select ="'sysDash'" />
                </xsl:otherwise>
              </xsl:choose>
            </xsl:when>
            <xsl:when test ="(($dots1=2) and ($dots2=1)) or (($dots1=1) and ($dots2=2))">
              <!--<xsl:if test ="(($dots1-length &lt;= $dot) and ($dots2-length &gt;= $dash) and ($dots2-length &lt;= $longDash))">-->
              <xsl:value-of select ="'lgDashDotDot'" />
              <!--</xsl:if>-->
            </xsl:when>
            <xsl:when test ="($dots1=1) and ($dots2=2)">
					
            <xsl:if test ="(($dots2-length &lt;= $dot) and ($dots1-length &gt;= $dash) and ($dots1-length &lt;= $longDash))">
            
						<xsl:value-of select ="'lgDashDotDot'" />
					
            </xsl:if>
            
				</xsl:when>
            <xsl:when test ="(($dots1 &gt;= 1) and not($dots2))">
              <xsl:choose>
                <xsl:when test ="($dots1-length &lt;= $dot)">
                  <xsl:value-of select ="'sysDash'" />
                </xsl:when>
                <xsl:when test ="($dots1-length &gt;= $dot) and ($dots1-length &lt;= $dash)">
                  <xsl:value-of select ="'dash'" />
                </xsl:when>
                <xsl:when test ="($dots1-length &gt;= $dash) and ($dots1-length &lt;= $longDash)">
                  <xsl:value-of select ="'lgDash'" />
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select ="'sysDash'" />
                </xsl:otherwise>
              </xsl:choose>
            </xsl:when>
            <xsl:when test ="(($dots2 &gt;= 1) and not($dots1))">
              <xsl:choose>
                <xsl:when test ="($dots2-length &lt;= $dot)">
                  <xsl:value-of select ="'sysDash'" />
                </xsl:when>
                <xsl:when test ="($dots2-length &gt;= $dot) and ($dots2-length &lt;= $dash)">
                  <xsl:value-of select ="'dash'" />
                </xsl:when>
                <xsl:when test ="($dots2-length &gt;= $dash) and ($dots2-length &lt;= $longDash)">
                  <xsl:value-of select ="'lgDash'" />
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select ="'sysDash'" />
                </xsl:otherwise>
              </xsl:choose>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select ="'sysDash'" />
            </xsl:otherwise>
          </xsl:choose>
     
      </xsl:for-each>
    </xsl:if >
    <xsl:if test ="not(document($chartStyleDirectory)/office:document-styles/office:styles/draw:stroke-dash[@draw:name=$stroke-dash])">
      <xsl:value-of select ="'sysDash'" />
    </xsl:if>
  </xsl:template>
  <xsl:template name ="getConvertUnit">
    <xsl:param name ="length"/>
    <xsl:choose>
      <xsl:when test="contains($length,'cm')">
        <xsl:value-of select="'cm'"/>
      </xsl:when>
      <xsl:when test="contains($length,'%')">
        <xsl:value-of select="'%'"/>
      </xsl:when>
      <xsl:when test="contains($length,'pt')">
        <xsl:value-of select="'pt'"/>
      </xsl:when>
      <xsl:when test="contains($length,'in')">
        <xsl:value-of select="'in'"/>
      </xsl:when>
      <!--mm to cm -->
      <xsl:when test="contains($length,'mm')">
        <xsl:value-of select="'mm'"/>
      </xsl:when>
      <!-- m to cm -->
      <xsl:when test="contains($length,'m')">
        <xsl:value-of select="'m'"/>
      </xsl:when>
      <!-- km to cm -->
      <xsl:when test="contains($length,'km')">
        <xsl:value-of select="'km'"/>
      </xsl:when>
      <!-- mi to cm -->
      <xsl:when test="contains($length,'mi')">
        <xsl:value-of select="'mi'"/>
      </xsl:when>
      <!-- ft to cm -->
      <xsl:when test="contains($length,'ft')">
        <xsl:value-of select="'ft'"/>
      </xsl:when>
      <!-- em to cm -->
      <xsl:when test="contains($length,'em')">
        <xsl:value-of select="'em'"/>
      </xsl:when>
      <!-- px to cm -->
      <xsl:when test="contains($length,'px')">
        <xsl:value-of select="'px'"/>
      </xsl:when>
      <!-- pc to cm -->
      <xsl:when test="contains($length,'pc')">
        <xsl:value-of select="'pc'"/>
      </xsl:when>
      <!-- ex to cm 1 ex to 6 px-->
      <xsl:when test="contains($length,'ex')">
        <xsl:value-of select="'ex'"/>
      </xsl:when>
    </xsl:choose>
  </xsl:template >
  <xsl:template name="InsertDrawingBorder1">
    <!-- line style -->
    <a:ln>
      <xsl:if test="@svg:stroke-width">
        <xsl:attribute name="w">
          <xsl:call-template name="emu-measure">
            <xsl:with-param name="length" select="@svg:stroke-width"/>
            <xsl:with-param name="unit">emu</xsl:with-param>
          </xsl:call-template>
        </xsl:attribute>
      </xsl:if>
      <xsl:choose>
        <xsl:when test="not(@draw:stroke = 'none' )">
          <a:solidFill>
              <a:srgbClr>
                <xsl:attribute name="val">
                  <xsl:choose>
                    <xsl:when test="@svg:stroke-color != '' ">
                      <xsl:value-of select="substring-after(@svg:stroke-color, '#')"/>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:text>000000</xsl:text>
                    </xsl:otherwise>
                  </xsl:choose>
                </xsl:attribute>
                <xsl:if test="@svg:stroke-opacity != ''">
                  <xsl:variable name="transparency">
                    <xsl:value-of select="substring-before(@svg:stroke-opacity, '%')"/>
                  </xsl:variable>
                  <a:alpha>
                    <xsl:attribute name="val">
                      <xsl:value-of select="$transparency * 1000"/>
                    </xsl:attribute>
                  </a:alpha>
                </xsl:if>
              </a:srgbClr>
          </a:solidFill>
          <xsl:if test="not(@draw:stroke-dash = 'none')">
          <a:prstDash val="dash"/>
        </xsl:if>
        </xsl:when>
        <xsl:otherwise>
          <a:noFill/>
        </xsl:otherwise>
      </xsl:choose>
    </a:ln>
  </xsl:template>
  <xsl:template name="InsertBitmapFill">
    <xsl:param name="chartDirectory"/>
    <xsl:variable name="fillImage">
      <xsl:value-of select="@draw:fill-image-name"/>
    </xsl:variable>

    <xsl:variable name="fillId">
      <xsl:for-each
        select="document(concat($chartDirectory,'/styles.xml'))/office:document-styles/office:styles/draw:fill-image[@draw:name = $fillImage]">
        <xsl:value-of select="generate-id()"/>
      </xsl:for-each>
    </xsl:variable>

    <xsl:if test="$fillId != '' ">
      <a:blipFill dpi="0" rotWithShape="1">
        <!-- id based on a style in content.xml -->
        <a:blip xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
          r:embed="{generate-id()}">

          <xsl:if test="@draw:opacity">
            <a:alphaModFix>
              <xsl:attribute name="amt">
                <xsl:value-of select="substring-before(@draw:opacity,'%' ) * 1000"/>
              </xsl:attribute>
            </a:alphaModFix>
          </xsl:if>

        </a:blip>

        <a:srcRect/>

        <xsl:choose>
          <xsl:when test="not(@style:repeat = 'stretch' or @style:repeat = 'no-repeat')">
            <a:tile tx="0" ty="0" sx="100000" sy="100000" flip="none" algn="tl">

              <!-- bitmap position -->
              <xsl:if test="@draw:fill-image-ref-point">
                <xsl:attribute name="algn">
                  <xsl:choose>
                    <xsl:when test="@draw:fill-image-ref-point = 'top-left' ">
                      <xsl:text>tl</xsl:text>
                    </xsl:when>
                    <xsl:when test="@draw:fill-image-ref-point = 'top' ">
                      <xsl:text>t</xsl:text>
                    </xsl:when>
                    <xsl:when test="@draw:fill-image-ref-point = 'top-right' ">
                      <xsl:text>tr</xsl:text>
                    </xsl:when>
                    <xsl:when test="@draw:fill-image-ref-point = 'left' ">
                      <xsl:text>l</xsl:text>
                    </xsl:when>
                    <xsl:when test="@draw:fill-image-ref-point = 'center' ">
                      <xsl:text>ctr</xsl:text>
                    </xsl:when>
                    <xsl:when test="@draw:fill-image-ref-point = 'right' ">
                      <xsl:text>r</xsl:text>
                    </xsl:when>
                    <xsl:when test="@draw:fill-image-ref-point = 'bottom-left' ">
                      <xsl:text>bl</xsl:text>
                    </xsl:when>
                    <xsl:when test="@draw:fill-image-ref-point = 'bottom' ">
                      <xsl:text>b</xsl:text>
                    </xsl:when>
                    <xsl:when test="@draw:fill-image-ref-point = 'bottom-right' ">
                      <xsl:text>br</xsl:text>
                    </xsl:when>
                  </xsl:choose>
                </xsl:attribute>
              </xsl:if>
            </a:tile>
          </xsl:when>
          <xsl:otherwise>
            <a:stretch>
              <a:fillRect/>
            </a:stretch>
          </xsl:otherwise>
        </xsl:choose>

      </a:blipFill>
    </xsl:if>

  </xsl:template>

  <xsl:template name="InsertTextRotation">
    <!-- rotation -->
    <xsl:if test="@style:rotation-angle">
      <xsl:attribute name="rot">
        <xsl:choose>
          <!-- 0 deg -->
          <xsl:when test="@style:rotation-angle = 0">
            <xsl:text>0</xsl:text>
          </xsl:when>
          <!-- (0 ; 180> deg -->
          <xsl:when
            test="@style:rotation-angle &lt; 90 and @style:rotation-angle &lt;= 180">
            <xsl:text>-</xsl:text>
            <xsl:value-of select="@style:rotation-angle * 60000"/>
          </xsl:when>
          <!-- (180 ; 360) deg -->
          <xsl:otherwise>
            <xsl:value-of select="(360 - @style:rotation-angle) * 60000"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
    </xsl:if>

    <!-- vertically stacked -->
    <xsl:if test="@style:direction = 'ttb' ">
      <xsl:attribute name="vert">
        <xsl:text>wordArtVert</xsl:text>
      </xsl:attribute>
    </xsl:if>
	</xsl:template>

	<!--Vijayeta,SP2-->
	<xsl:template name="SetPositionSP2">
		<xsl:variable name="InsertStartColumn">
			<xsl:call-template name="InsertStartColumn"/>
		</xsl:variable>
		<xsl:variable name="InsertStartColumnOffset">
			<xsl:call-template name="InsertStartColumnOffset"/>
		</xsl:variable>
		<xsl:variable name="InsertStartRow">
			<xsl:call-template name="InsertStartRow"/>
		</xsl:variable>
		<xsl:variable name="InsertStartRowOffset">
			<xsl:call-template name="InsertStartRowOffset"/>
		</xsl:variable>
		<xdr:from>
			<xdr:col>
				<xsl:value-of select="$InsertStartColumn"/>
			</xdr:col>
			<xdr:colOff>
				<xsl:value-of select="$InsertStartColumnOffset"/>
			</xdr:colOff>
			<xdr:row>
				<xsl:value-of select="$InsertStartRow"/>
			</xdr:row>
			<xdr:rowOff>
				<xsl:value-of select="$InsertStartRowOffset"/>
			</xdr:rowOff>
		</xdr:from>
		<xdr:ext>
			<xsl:attribute name="cx">
				<xsl:call-template name="emu-measure">
					<xsl:with-param name="length" select="@svg:width" />
				</xsl:call-template>
			</xsl:attribute>
			<xsl:attribute name="cy">
				<xsl:call-template name="emu-measure">
					<xsl:with-param name="length" select="@svg:height" />
				</xsl:call-template>
			</xsl:attribute>
		</xdr:ext>
  </xsl:template>

</xsl:stylesheet>
