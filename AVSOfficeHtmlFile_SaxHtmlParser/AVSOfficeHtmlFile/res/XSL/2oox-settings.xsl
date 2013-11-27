<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:output method="xml" encoding="UTF-8"/>

	<xsl:strip-space elements="*"/>

	<xsl:template match="/html">
		<xsl:message>Progress: html</xsl:message>
		<w:settings xmlns:o="urn:schemas-microsoft-com:office:office" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships" xmlns:m="http://schemas.openxmlformats.org/officeDocument/2006/math" xmlns:v="urn:schemas-microsoft-com:vml" xmlns:w10="urn:schemas-microsoft-com:office:word" xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main" xmlns:sl="http://schemas.openxmlformats.org/schemaLibrary/2006/main">
			<!--<w:view w:val="web"/>-->
			<w:zoom w:percent="110"/>
			<w:displayBackgroundShape/>
			<w:defaultTabStop w:val="708"/>
			<w:characterSpacingControl w:val="doNotCompress"/>
			<w:compat>
				<w:useFELayout/>
			</w:compat>
			<w:rsids>
				<w:rsidRoot w:val="00FD7FED"/>
				<w:rsid w:val="00301D39"/>
				<w:rsid w:val="004D28DB"/>
				<w:rsid w:val="004D5F21"/>
				<w:rsid w:val="00753386"/>
				<w:rsid w:val="007C43CF"/>
				<w:rsid w:val="00820E0B"/>
				<w:rsid w:val="00FD7FED"/>
			</w:rsids>
			<m:mathPr>
				<m:mathFont m:val="Cambria Math"/>
				<m:brkBin m:val="before"/>
				<m:brkBinSub m:val="--"/>
				<m:smallFrac m:val="off"/>
				<m:dispDef/>
				<m:lMargin m:val="0"/>
				<m:rMargin m:val="0"/>
				<m:defJc m:val="centerGroup"/>
				<m:wrapIndent m:val="1440"/>
				<m:intLim m:val="subSup"/>
				<m:naryLim m:val="undOvr"/>
			</m:mathPr>
			<w:themeFontLang w:val="en-En" w:eastAsia="zh-TW"/>
			<w:clrSchemeMapping w:bg1="light1" w:t1="dark1" w:bg2="light2" w:t2="dark2" w:accent1="accent1" w:accent2="accent2" w:accent3="accent3" w:accent4="accent4" w:accent5="accent5" w:accent6="accent6" w:hyperlink="hyperlink" w:followedHyperlink="followedHyperlink"/>
			<w:shapeDefaults>
				<o:shapedefaults v:ext="edit" spidmax="2050"/>
				<o:shapelayout v:ext="edit">
					<o:idmap v:ext="edit" data="1"/>
				</o:shapelayout>
			</w:shapeDefaults>
			<w:decimalSymbol w:val="."/>
			<w:listSeparator w:val=";"/>
		</w:settings>
	</xsl:template>

	<xsl:template match="text()"/>

</xsl:stylesheet>
