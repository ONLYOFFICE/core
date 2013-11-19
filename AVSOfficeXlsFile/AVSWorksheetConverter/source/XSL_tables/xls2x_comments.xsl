<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:xlsx="http://www.avs4you.ru/XLSXConverter/1.0/DirectoryStructure">

  <xsl:template match="WorksheetSubstream[Note]" mode="comments">
    <!--<xlsx:file>
      <xsl:attribute name="name">comments<xsl:value-of select="count(preceding-sibling::WorksheetSubstream[Note]) + 1"/>.xml</xsl:attribute>
      <comments xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <authors>
          <xsl:for-each select="Note/NoteSh">
            <author><xsl:value-of select="@stAuthor"/></author>
          </xsl:for-each>
        </authors>
        <commentList>
          <xsl:for-each select="Note/NoteSh">
            <comment ref="{@ref}" authorId="{position() - 1}">
              <text>111</text>
            </comment>
          </xsl:for-each>
        </commentList>
      </comments>
    </xlsx:file>-->
  </xsl:template>
  
</xsl:stylesheet>