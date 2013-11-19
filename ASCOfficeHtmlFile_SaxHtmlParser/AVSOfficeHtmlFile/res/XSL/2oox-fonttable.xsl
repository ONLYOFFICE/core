<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:output method="xml" encoding="UTF-8"/>

	<xsl:strip-space elements="*"/>

	<xsl:template match="/html">
		<xsl:message>Progress: html</xsl:message>
		<w:fonts xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships" xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main">
			<w:font w:name="Calibri">
				<w:panose1 w:val="020F0502020204030204"/>
				<w:charset w:val="CC"/>
				<w:family w:val="swiss"/>
				<w:pitch w:val="variable"/>
				<w:sig w:usb0="A00002EF" w:usb1="4000207B" w:usb2="00000000" w:usb3="00000000" w:csb0="0000009F" w:csb1="00000000"/>
			</w:font>
			<w:font w:name="PMingLiU">
				<w:altName w:val="新細明體"/>
				<w:panose1 w:val="02020300000000000000"/>
				<w:charset w:val="88"/>
				<w:family w:val="roman"/>
				<w:pitch w:val="variable"/>
				<w:sig w:usb0="00000003" w:usb1="080E0000" w:usb2="00000016" w:usb3="00000000" w:csb0="00100001" w:csb1="00000000"/>
			</w:font>
			<w:font w:name="Times New Roman">
				<w:panose1 w:val="02020603050405020304"/>
				<w:charset w:val="CC"/>
				<w:family w:val="roman"/>
				<w:pitch w:val="variable"/>
				<w:sig w:usb0="20002A87" w:usb1="80000000" w:usb2="00000008" w:usb3="00000000" w:csb0="000001FF" w:csb1="00000000"/>
			</w:font>
			<w:font w:name="Arial">
				<w:panose1 w:val="020B0604020202020204"/>
				<w:charset w:val="CC"/>
				<w:family w:val="swiss"/>
				<w:pitch w:val="variable"/>
				<w:sig w:usb0="20002A87" w:usb1="80000000" w:usb2="00000008" w:usb3="00000000" w:csb0="000001FF" w:csb1="00000000"/>
			</w:font>
			<w:font w:name="Cambria">
				<w:panose1 w:val="02040503050406030204"/>
				<w:charset w:val="CC"/>
				<w:family w:val="roman"/>
				<w:pitch w:val="variable"/>
				<w:sig w:usb0="A00002EF" w:usb1="4000004B" w:usb2="00000000" w:usb3="00000000" w:csb0="0000009F" w:csb1="00000000"/>
			</w:font>
		</w:fonts>
	</xsl:template>

	<xsl:template match="text()"/>

</xsl:stylesheet>
