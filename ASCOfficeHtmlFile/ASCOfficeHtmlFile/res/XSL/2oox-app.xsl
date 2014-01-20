<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns="http://schemas.openxmlformats.org/officeDocument/2006/extended-properties"
                xmlns:vt="http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes">

	<xsl:output method="xml" encoding="UTF-8"/>

	<xsl:strip-space elements="*"/>

	<xsl:template match="/html">
		<xsl:message>Progress: html</xsl:message>
		<Properties>
			<Template>Normal.dotm</Template>
			<TotalTime>1</TotalTime>
			<Pages>1</Pages>
			<Words>10</Words>
			<Characters>62</Characters>
			<Application>ASC Office Studio</Application>
			<DocSecurity>0</DocSecurity>
			<Lines>1</Lines>
			<Paragraphs>1</Paragraphs>
			<ScaleCrop>false</ScaleCrop>
			<Company>ASC Media</Company>
			<LinksUpToDate>false</LinksUpToDate>
			<CharactersWithSpaces>71</CharactersWithSpaces>
			<SharedDoc>false</SharedDoc>
			<HyperlinksChanged>false</HyperlinksChanged>
			<AppVersion>12.0000</AppVersion>
		</Properties>
	</xsl:template>

	<xsl:template match="text()"/>

</xsl:stylesheet>
