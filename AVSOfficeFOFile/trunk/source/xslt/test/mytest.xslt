<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:msxsl="urn:schemas-microsoft-com:xslt" exclude-result-prefixes="msxsl"
                
                xmlns:fo="http://www.w3.org/1999/XSL/Format"
                xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"
                
>

  <xsl:output method="xml" indent="yes" encoding="utf-8" version="1.0"/>

  <xsl:template match="/w:document">
    <fo:root xmlns:fo="http://www.w3.org/1999/XSL/Format"
         xmlns:fox="http://xml.apache.org/fop/extensions">

      <xsl:call-template name="CreatePageLayout"/>
      <xsl:call-template name="CreatePageSequence"/>

    </fo:root>
  </xsl:template>


  <xsl:template name="CreatePageLayout">
    <fo:layout-master-set>
      <fo:simple-page-master master-name="simple"
                    page-height="29.7cm"
                    page-width="21cm"
                    margin-top="1cm"
                    margin-bottom="2cm"
                    margin-left="2.5cm"
                    margin-right="2.5cm">
        <fo:region-body margin-top="3cm"/>
        <fo:region-before extent="3cm"/>
        <fo:region-after extent="1.5cm"/>
      </fo:simple-page-master>
    </fo:layout-master-set>
  </xsl:template>

  <!-- ***************************************************************************  -->
  <!-- ***************************************************************************  -->
  <!-- ***************************************************************************  -->

  <xsl:template name="CreatePageSequence">

    <fo:page-sequence master-reference="simple">
      <fo:flow flow-name="xsl-region-body">

        <xsl:for-each select="w:body/w:p">
          <fo:block>
            <xsl:for-each select="w:r">

                <xsl:apply-templates select="*"/>

            </xsl:for-each>
          </fo:block>
        </xsl:for-each>

      </fo:flow>
    </fo:page-sequence>

  </xsl:template>




  <!--  <xsl:template match="@* | node()">
        <xsl:copy>
            <xsl:apply-templates select="@* | node()"/>
        </xsl:copy>
    </xsl:template>
  -->
</xsl:stylesheet>
