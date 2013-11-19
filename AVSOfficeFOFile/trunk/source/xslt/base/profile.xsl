<?xml version="1.0" encoding="UTF-8"?>

<!-- ===================================================================

WordML and XSL-FO are very high-level representations, and the actual
layout of the text on the page depends on fine details of algorithms 
implemented in a specific formatting engine.

The profile is designed as a tool to adjust the text layout of
the resulting XSL-FO document.

======================================================================== -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">


<!-- ======== -->
<!-- DEFAULTS -->
<!-- ======== -->
  <xsl:variable name="default-width.list-label" select="9"/>
  <xsl:variable name="default-font-size" select="10"/>
  <xsl:variable name="default-font-size.list-label" select="12"/>
  <xsl:variable name="default-font-size.symbol" select="12"/>
  <xsl:variable name="default-widows" select="2"/>
  <xsl:variable name="default-orphans" select="2"/>
  <xsl:variable name="white-space-collapse" select="'false'"/>
  <xsl:variable name="default-header-extent" select="'11in'"/>
  <xsl:variable name="default-footer-extent" select="'11in'"/>
  <xsl:variable name="default-line-height" select="1.147"/>
  
  <!-- not used in the current version of the style sheet -->
  <xsl:variable name="not-supported-inlines" select="''"/>

</xsl:stylesheet>

