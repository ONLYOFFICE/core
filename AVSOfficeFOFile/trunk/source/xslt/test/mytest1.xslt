<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet
  version="2.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:msxsl="urn:schemas-microsoft-com:xslt"
  xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"
  exclude-result-prefixes="msxsl"
>
  
  <xsl:output method="xml" indent="yes" encoding="utf-8" version="1.0"/>

  <xsl:template match="/">
    <body>
      <xsl:for-each select="//w:rPr/w:sz[number(@w:val) > 20]">
        <xsl:call-template name="PrintPar"/>
      </xsl:for-each>
    </body>
  </xsl:template>

  <xsl:template name="PrintPar">
    [
        <par>
          <xsl:value-of select="../../."/>
        </par>

      <!--<xsl:for-each select="child::w:pPr">
      <xsl:element name="style">
        <xsl:attribute name="val">
          <xsl:value-of select="w:pStyle/@w:val"/>          
        </xsl:attribute>
      </xsl:element>
    </xsl:for-each>-->
    ]
  </xsl:template>



  <!--   <xsl:template match="@* | node()">
        <xsl:copy>
            <xsl:apply-templates select="@* | node()"/>
        </xsl:copy>
    </xsl:template>
-->
  </xsl:stylesheet>
