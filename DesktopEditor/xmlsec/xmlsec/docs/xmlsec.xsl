<xsl:stylesheet version = '1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
    <xsl:output method="html" encoding="ISO-8859-1" />
    <xsl:param name="topfolder"></xsl:param>

    <xsl:template name="linkto">
	<xsl:param name="href"/>
	<xsl:param name="text"/>
	<xsl:element name="a"> 
	    <xsl:attribute name="href">
		<xsl:value-of select="$topfolder"/>
		<xsl:value-of select="$href"/>
	    </xsl:attribute>		
	    <xsl:value-of select="$text"/>
	</xsl:element>
    </xsl:template>

    <xsl:template name="myimg">
	<xsl:param name="src"/>
	<xsl:param name="alt"/>
	<xsl:element name="img"> 
	    <xsl:attribute name="src">
		<xsl:value-of select="$topfolder"/>
		<xsl:value-of select="$src"/>
	    </xsl:attribute>		
	    <xsl:attribute name="alt">
		<xsl:value-of select="$alt"/>
	    </xsl:attribute>			    
	    <xsl:attribute name="border">0</xsl:attribute>
	</xsl:element>
    </xsl:template>

    <xsl:template match="/">

<html>
<head>
<xsl:copy-of select="//head/*" />
<style type="text/css">
table.CALSTABLE > tbody > tr:nth-child(1) > td:nth-child(1) {
    width: 20em;
}
.synopsis, .classsynopsis {
    background: #eeeeee;
    border: solid 1px #aaaaaa;
}
.programlisting {
    background: #eeeeee;
    border: solid 1px #000000;
}
.navigation {
    background: #eeeeee;
    border: solid 1px #000000;
}
.navigation a {
    color: initial;
}
.navigation a:visited {
    color: initial;
}
</style>
</head>
<body>
<table width="100%" valign="top">
<tr valign="top">
    <!-- top navigation menu start -->     
    <td valign="top" align="left" width="210">
	<xsl:call-template name="myimg"> 
	    <xsl:with-param name="src">images/logo.gif</xsl:with-param> 
	    <xsl:with-param name="alt">XML Security Library</xsl:with-param> 
	</xsl:call-template>
        <p/>
        <ul>

	    <li>
		<xsl:call-template name="linkto"> 
		    <xsl:with-param name="href">index.html</xsl:with-param> 
		    <xsl:with-param name="text">Home</xsl:with-param> 
		</xsl:call-template>
	    </li>	    
	    <li>
		<xsl:call-template name="linkto"> 
		    <xsl:with-param name="href">download.html</xsl:with-param> 
		    <xsl:with-param name="text">Download</xsl:with-param> 
		</xsl:call-template>
	    </li>
	    <li>
		<xsl:call-template name="linkto"> 
		    <xsl:with-param name="href">news.html</xsl:with-param> 
		    <xsl:with-param name="text">News</xsl:with-param> 
		</xsl:call-template>
	    </li>	    
	    <li>
		<xsl:call-template name="linkto"> 
		    <xsl:with-param name="href">documentation.html</xsl:with-param> 
		    <xsl:with-param name="text">Documentation</xsl:with-param> 
		</xsl:call-template>
	    </li>	    
	    <ul>
	    <li>
		<xsl:call-template name="linkto"> 
		    <xsl:with-param name="href">faq.html</xsl:with-param> 
		    <xsl:with-param name="text">FAQ</xsl:with-param> 
		</xsl:call-template>
	    </li>	    
	    <li>
		<xsl:call-template name="linkto"> 
		    <xsl:with-param name="href">api/xmlsec-notes.html</xsl:with-param> 
		    <xsl:with-param name="text">Tutorial</xsl:with-param> 
		</xsl:call-template>
	    </li>	    
	    <li>
		<xsl:call-template name="linkto"> 
		    <xsl:with-param name="href">api/xmlsec-reference.html</xsl:with-param> 
		    <xsl:with-param name="text">API reference</xsl:with-param> 
		</xsl:call-template>
	    </li>	    
	    <li>
		<xsl:call-template name="linkto"> 
		    <xsl:with-param name="href">api/xmlsec-examples.html</xsl:with-param> 
		    <xsl:with-param name="text">Examples</xsl:with-param> 
		</xsl:call-template>
	    </li>	    
	    </ul>
	    <li>
		<xsl:call-template name="linkto"> 
		    <xsl:with-param name="href">xmldsig.html</xsl:with-param> 
		    <xsl:with-param name="text">XML Digital Signature</xsl:with-param> 
		</xsl:call-template>
	    </li>	    
	    <ul>
	    <li>
		<a href="http://www.aleksey.com/xmlsec/xmldsig-verifier.html">Online Verifier</a> 
	    </li>
	    </ul>
	    <li>
		<xsl:call-template name="linkto"> 
		    <xsl:with-param name="href">xmlenc.html</xsl:with-param> 
		    <xsl:with-param name="text">XML Encryption</xsl:with-param> 
		</xsl:call-template>
	    </li>
	    <li>
		<xsl:call-template name="linkto"> 
		    <xsl:with-param name="href">c14n.html</xsl:with-param> 
		    <xsl:with-param name="text">XML Canonicalization</xsl:with-param> 
		</xsl:call-template>
	    </li>
	    <li>
		<xsl:call-template name="linkto"> 
		    <xsl:with-param name="href">bugs.html</xsl:with-param> 
		    <xsl:with-param name="text">Reporting Bugs</xsl:with-param> 
		</xsl:call-template>
	    </li>
            <li><a href="http://www.aleksey.com/pipermail/xmlsec">Mailing list</a></li>
	    <li>
		<xsl:call-template name="linkto"> 
		    <xsl:with-param name="href">related.html</xsl:with-param> 
		    <xsl:with-param name="text">Related</xsl:with-param> 
		</xsl:call-template>
	    </li>	    
	    <li>
		<xsl:call-template name="linkto"> 
		    <xsl:with-param name="href">authors.html</xsl:with-param> 
		    <xsl:with-param name="text">Authors</xsl:with-param> 
		</xsl:call-template>
	    </li>
	</ul>
	<table width="100%"><tr><td width="15"></td><td>
      	<a href="http://xmlsoft.org/">
	    <xsl:call-template name="myimg"> 
		<xsl:with-param name="src">images/libxml2-logo.png</xsl:with-param> 
		<xsl:with-param name="alt">LibXML2</xsl:with-param> 
	    </xsl:call-template>
	</a>
	</td></tr><tr><td width="15"></td><td>
	<a href="http://xmlsoft.org/XSLT">
	    <xsl:call-template name="myimg"> 
		<xsl:with-param name="src">images/libxslt-logo.png</xsl:with-param> 
		<xsl:with-param name="alt">LibXSLT</xsl:with-param> 
	    </xsl:call-template>
	</a>
	</td></tr><tr><td width="15"></td><td>
	<a href="http://www.openssl.org/">
	    <xsl:call-template name="myimg"> 
		<xsl:with-param name="src">images/openssl-logo.png</xsl:with-param> 
		<xsl:with-param name="alt">OpenSSL</xsl:with-param> 
	    </xsl:call-template>
	</a>
	</td></tr>
	
	<!-- links block markers -->
	<xsl:comment>Links - start</xsl:comment>
	<xsl:comment>Links - end</xsl:comment>
	
	</table>
     </td>
     <!-- top navigation menu end -->

     <td valign="top" > 
        <table width="100%" valign="top">
	    <tr><td valign="top" align="left" id="xmlsecContent"> 
	    <xsl:choose> 
		<xsl:when test="//td[@id='xmlsecContent']">
		    <xsl:copy-of select="//td[@id='xmlsecContent']/*" />
		</xsl:when>
		<xsl:otherwise>
		    <xsl:copy-of select="//body/*" />
		</xsl:otherwise>
	    </xsl:choose>
	    </td></tr>
	</table>
    </td>

</tr></table>
</body>
</html>

    </xsl:template>
</xsl:stylesheet>
