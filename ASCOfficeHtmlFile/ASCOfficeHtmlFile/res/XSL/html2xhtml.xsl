<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns="http://schemas.openxmlformats.org/officeDocument/2006/extended-properties">

	<xsl:output method="xml" encoding="UTF-8"/>

	<xsl:strip-space elements="*"/>

	<xsl:template match="/">
		<xsl:copy-of select="."/>
	</xsl:template>


</xsl:stylesheet>