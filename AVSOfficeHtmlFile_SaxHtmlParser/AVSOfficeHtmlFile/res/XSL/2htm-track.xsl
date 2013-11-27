<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:number="urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0"
  xmlns:wp="http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:dc="http://purl.org/dc/elements/1.1/"
  xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns="http://schemas.openxmlformats.org/package/2006/relationships"
  xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
  xmlns:oox="urn:oox"
  exclude-result-prefixes="w r xlink number wp oox">

	<xsl:key name="track-changes" match="w:ins|w:del|w:pPrChange|w:rPrChange" use="''"/>

	<xsl:template match="w:r" mode="trackchanges">
		<xsl:choose>
			<xsl:when test="parent::w:del">
				<xsl:choose>
					<xsl:when
					  test="generate-id(.) = generate-id(ancestor::w:p/descendant::w:r[last()]) and ancestor::w:p/w:pPr/w:rPr/w:del"/>
					<xsl:when
					  test="generate-id(.) = generate-id(ancestor::w:p/descendant::w:r[1]) and key('p', number(@oox:id)-1)/w:pPr/w:rPr/w:del"/>
					<xsl:otherwise>
						<text:changed-region>
							<xsl:attribute name="text:id">
								<xsl:value-of select="generate-id(.)"/>
							</xsl:attribute>
							<text:deletion>
								<office:change-info>
									<dc:creator>
										<xsl:value-of select="parent::w:del/@w:author"/>
									</dc:creator>
									<dc:date>
										<xsl:choose>
											<xsl:when test="contains(parent::w:del/@w:date,'Z')">
												<xsl:value-of select="substring-before(parent::w:del/@w:date,'Z')"/>
											</xsl:when>
											<xsl:otherwise>
												<xsl:value-of select="parent::w:del/@w:date"/>
											</xsl:otherwise>
										</xsl:choose>
									</dc:date>
								</office:change-info>
								<text:p>
									<xsl:attribute name="text:style-name">
										<xsl:value-of select="generate-id(ancestor::w:p)"/>
									</xsl:attribute>
									<xsl:choose>
										<!--check whether string contains  whitespace sequence-->
										<xsl:when test="not(contains(w:delText,'  '))">
											<xsl:value-of select="w:delText"/>
											<xsl:if test="w:delInstrText">
												<xsl:value-of select="w:delInstrText"/>
											</xsl:if>
										</xsl:when>
										<xsl:otherwise>
											<!--converts whitespaces sequence to text:s-->
											<xsl:call-template name="InsertWhiteSpaces">
												<xsl:with-param name="string">
													<xsl:value-of select="w:delText"/>
												</xsl:with-param>
											</xsl:call-template>
										</xsl:otherwise>
									</xsl:choose>
								</text:p>
							</text:deletion>
						</text:changed-region>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:when test="parent::w:ins">
				<text:changed-region>
					<xsl:attribute name="text:id">
						<xsl:value-of select="generate-id(.)"/>
					</xsl:attribute>
					<text:insertion>
						<office:change-info>
							<dc:creator>
								<xsl:value-of select="parent::w:ins/@w:author"/>
							</dc:creator>
							<dc:date>
								<xsl:choose>
									<xsl:when test="contains(parent::w:ins/@w:date,'Z')">
										<xsl:value-of select="substring-before(parent::w:ins/@w:date,'Z')"/>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select="parent::w:ins/@w:date"/>
									</xsl:otherwise>
								</xsl:choose>
							</dc:date>
						</office:change-info>
					</text:insertion>
				</text:changed-region>
			</xsl:when>
			<xsl:when test="descendant::w:rPrChange">
				<text:changed-region>
					<xsl:attribute name="text:id">
						<xsl:value-of select="generate-id(.)"/>
					</xsl:attribute>
					<text:format-change>
						<office:change-info>
							<dc:creator>
								<xsl:value-of select="descendant::w:rPrChange/@w:author"/>
							</dc:creator>
							<dc:date>
								<xsl:choose>
									<xsl:when test="contains(descendant::w:rPrChange/@w:date,'Z')">
										<xsl:value-of select="substring-before(descendant::w:rPrChange/@w:date,'Z')"/>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select="descendant::w:rPrChange/@w:date"/>
									</xsl:otherwise>
								</xsl:choose>
							</dc:date>
						</office:change-info>
					</text:format-change>
				</text:changed-region>
			</xsl:when>
		</xsl:choose>
		<xsl:apply-templates mode="trackchanges"/>
	</xsl:template>

	<xsl:template match="w:rPr[parent::w:pPr]" mode="trackchanges">
		<xsl:choose>
			<xsl:when test="w:del">
				<text:changed-region>
					<xsl:attribute name="text:id">
						<xsl:value-of select="generate-id(.)"/>
					</xsl:attribute>
					<text:deletion>
						<office:change-info>
							<dc:creator>
								<xsl:value-of select="w:del/@w:author"/>
							</dc:creator>
							<dc:date>
								<xsl:choose>
									<xsl:when test="contains(w:del/@w:date,'Z')">
										<xsl:value-of select="substring-before(w:del/@w:date,'Z')"/>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select="w:del/@w:date"/>
									</xsl:otherwise>
								</xsl:choose>
							</dc:date>
						</office:change-info>
						<text:p>
							<xsl:attribute name="text:style-name">
								<xsl:value-of select="generate-id(ancestor::w:p)"/>
							</xsl:attribute>
							<xsl:if test="ancestor::w:p/descendant::w:r[last()]/parent::w:del">
								<xsl:value-of select="ancestor::w:p/descendant::w:r[last()]"/>
							</xsl:if>
						</text:p>
						<text:p>
							<xsl:attribute name="text:style-name">
								<xsl:choose>
									<xsl:when test="key('p', @oox:id+1)/w:pPr/w:pPrChange/w:pPr/w:pStyle">
										<xsl:value-of select="key('p', @oox:id+1)/w:pPr/w:pPrChange/w:pPr/w:pStyle/@w:val"
                    />
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select="generate-id(key('p', @oox:id+1))"/>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:attribute>
							<xsl:if test="key('p', @oox:id+1)/descendant::w:r[1]/parent::w:del">
								<xsl:value-of select="key('p', @oox:id+1)/descendant::w:r[1]"/>
							</xsl:if>
						</text:p>
					</text:deletion>
				</text:changed-region>
			</xsl:when>
			<xsl:when test="w:ins">
				<text:changed-region>
					<xsl:attribute name="text:id">
						<xsl:value-of select="generate-id(ancestor::w:p)"/>
					</xsl:attribute>
					<text:insertion>
						<office:change-info>
							<dc:creator>
								<xsl:value-of select="w:ins/@w:author"/>
							</dc:creator>
							<dc:date>
								<xsl:choose>
									<xsl:when test="contains(w:ins/@w:date,'Z')">
										<xsl:value-of select="substring-before(w:ins/@w:date,'Z')"/>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select="w:ins/@w:date"/>
									</xsl:otherwise>
								</xsl:choose>
							</dc:date>
						</office:change-info>
					</text:insertion>
				</text:changed-region>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="TrackChanges">
		<xsl:variable name="hasTrackChanges">
			<xsl:for-each select="key('Part', 'word/document.xml')/w:document/w:body">
				<xsl:if test="key('track-changes', '')">true</xsl:if>
			</xsl:for-each>
			<xsl:for-each
			  select="key('Part', 'word/_rels/document.xml.rels')/Relationships/Relationship/@Target[contains(.,'footer') or contains(.,'header') or contains(.,'footnotes') or contains(.,'endnotes')]">
				<xsl:variable name="file">
					<xsl:value-of select="."/>
				</xsl:variable>
				<xsl:for-each select="key('Part', concat('word/',$file))">
					<xsl:if test="key('track-changes', '')">true</xsl:if>
				</xsl:for-each>
			</xsl:for-each>
		</xsl:variable>
		<xsl:if test="$hasTrackChanges = 'true' ">
			<text:tracked-changes>
				<xsl:for-each select="key('Part', 'word/document.xml')/w:document/w:body">
					<xsl:if test="key('track-changes', '')">
						<xsl:apply-templates select="key('Part', 'word/document.xml')/w:document/w:body"
						  mode="trackchanges"/>
					</xsl:if>
				</xsl:for-each>
				<xsl:for-each
				  select="key('Part', 'word/_rels/document.xml.rels')/Relationships/Relationship/@Target[contains(.,'footer') or contains(.,'header') or contains(.,'footnotes') or contains(.,'endnotes')]">
					<xsl:variable name="file">
						<xsl:value-of select="."/>
					</xsl:variable>
					<xsl:for-each select="key('Part', concat('word/',$file))">
						<xsl:if test="key('track-changes', '')">
							<xsl:apply-templates select="key('Part', concat('word/',$file))" mode="trackchanges"/>
						</xsl:if>
					</xsl:for-each>
				</xsl:for-each>
			</text:tracked-changes>
		</xsl:if>
	</xsl:template>

	<xsl:template name="TrackChangesInsertMade">
		<text:change-start>
			<xsl:attribute name="text:change-id">
				<xsl:value-of select="generate-id(.)"/>
			</xsl:attribute>
		</text:change-start>
		<text:span>
			<xsl:attribute name="text:style-name">
				<xsl:value-of select="generate-id(.)"/>
			</xsl:attribute>
			<xsl:apply-templates/>
		</text:span>
		<text:change-end>
			<xsl:attribute name="text:change-id">
				<xsl:value-of select="generate-id(.)"/>
			</xsl:attribute>
		</text:change-end>
	</xsl:template>

	<xsl:template name="TrackChangesDeleteMade">
		<xsl:choose>
			<xsl:when
			  test="generate-id(.) = generate-id(ancestor::w:p/descendant::w:r[last()]) and ancestor::w:p/w:pPr/w:rPr/w:del"/>
			<xsl:when
			  test="generate-id(.) = generate-id(ancestor::w:p/descendant::w:r[1]) and key('p', number(@oox:id)-1)/w:pPr/w:rPr/w:del"/>
			<xsl:otherwise>
				<text:change>
					<xsl:attribute name="text:change-id">
						<xsl:value-of select="generate-id(.)"/>
					</xsl:attribute>
				</text:change>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="TrackChangesChangesMade">
		<text:change-start>
			<xsl:attribute name="text:change-id">
				<xsl:value-of select="generate-id(.)"/>
			</xsl:attribute>
		</text:change-start>
		<text:span>
			<xsl:attribute name="text:style-name">
				<xsl:value-of select="generate-id(.)"/>
			</xsl:attribute>
			<xsl:value-of select="descendant::text()"/>
		</text:span>
		<text:change-end>
			<xsl:attribute name="text:change-id">
				<xsl:value-of select="generate-id(.)"/>
			</xsl:attribute>
		</text:change-end>
	</xsl:template>

	<xsl:template match="w:t" mode="trackchanges"/>

	<xsl:template match="w:delText" mode="trackchanges"/>

	<xsl:template match="w:delInstrText" mode="trackchanges"/>

	<xsl:template match="w:instrText" mode="trackchanges"/>

	<xsl:template match="wp:posOffset" mode="trackchanges"/>

</xsl:stylesheet>
