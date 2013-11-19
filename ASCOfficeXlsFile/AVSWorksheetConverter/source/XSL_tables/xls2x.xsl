<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:xlsx="http://www.avs4you.ru/XLSXConverter/1.0/DirectoryStructure"
                xmlns:msxsl="urn:schemas-microsoft-com:xslt"
                extension-element-prefixes="msxsl">

  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>

  <xsl:template match="/">
     <xsl:apply-templates select="/root"/>
  </xsl:template>
  
  <xsl:template match="root">
    <xlsx:root id="{@id}">
      <xsl:call-template name="content_types"/>
      <xsl:call-template name="root_rels"/>
      <xsl:apply-templates select="WorkbookStreamObject"/>
    </xlsx:root>
  </xsl:template>



</xsl:stylesheet>