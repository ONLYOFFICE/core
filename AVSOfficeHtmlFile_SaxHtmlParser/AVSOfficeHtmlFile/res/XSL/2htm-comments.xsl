<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
  xmlns:dc="http://purl.org/dc/elements/1.1/"
  xmlns:meta="urn:oasis:names:tc:opendocument:xmlns:meta:1.0"
  xmlns:pzip="urn:cleverage:xmlns:post-processings:zip"
  xmlns:xlink="http://www.w3.org/1999/xlink"
  xmlns:oox="urn:oox"
                xmlns="http://www.w3.org/1999/xhtml"
  exclude-result-prefixes="office text  fo style dc meta pzip xlink w oox">


	<xsl:template name="InsertComment">
		<xsl:param name="Id"/>
		<!--<office:annotation>
      <xsl:if test="key('Part', 'word/comments.xml')/w:comments/w:comment[@w:id = $Id]/@w:author">
        <dc:creator>
          <xsl:value-of select="key('Part', 'word/comments.xml')/w:comments/w:comment[@w:id = $Id]/@w:author"/>
        </dc:creator>
      </xsl:if>
      <xsl:if test="key('Part', 'word/comments.xml')/w:comments/w:comment[@w:id = $Id]/@w:date">
        <dc:date>
          <xsl:value-of select="key('Part', 'word/comments.xml')/w:comments/w:comment[@w:id = $Id]/@w:date"/>
        </dc:date>
      </xsl:if>
      <xsl:apply-templates select="key('Part', 'word/comments.xml')/w:comments/w:comment[@w:id = $Id]/w:p"/>
    </office:annotation>-->
	</xsl:template>
</xsl:stylesheet>
