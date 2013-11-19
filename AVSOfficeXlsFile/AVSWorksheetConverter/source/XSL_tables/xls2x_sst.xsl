<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:xlsx="http://www.avs4you.ru/XLSXConverter/1.0/DirectoryStructure">

  <xsl:template match="SST">
    <xsl:if test="@cstTotal != 0">
      <xlsx:file name="sharedStrings.xml">
        <sst xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main" count="{@cstTotal}" uniqueCount="{@cstUnique}">
          <xsl:for-each select="XLUnicodeRichExtendedString">
            <si>
              <xsl:choose>
                <xsl:when test="FormatRun">
                  <r>
                    <xsl:variable name="run_text"><xsl:value-of select="substring(@text, 1, FormatRun[1]/@ich)"/></xsl:variable>
                    <t>
                      <xsl:if test="starts-with($run_text, ' ') or substring($run_text, string-length($run_text), 1) = ' ' ">
                        <xsl:attribute name="xml:space">preserve</xsl:attribute>
                      </xsl:if>
                      <xsl:value-of select="$run_text"/>
                    </t>
                  </r>
                  <xsl:for-each select="FormatRun">
                    <r>
                      <xsl:if test="@ich != string-length(@text)">
                        <xsl:variable name="ifnt" select="@ifnt"/>
                        <xsl:variable name="actual_font" select="/root/WorkbookStreamObject/GlobalsSubstream/FORMATTING/Font[position() = $ifnt + 1]"/>
                        <xsl:call-template name="font">
                          <xsl:with-param name="tag_name">rPr</xsl:with-param>
                          <xsl:with-param name="rFont" select="$actual_font/@fontName"/>
                          <xsl:with-param name="charset" select="$actual_font/@bCharSet"/>
                          <xsl:with-param name="family" select="$actual_font/@bFamily"/>
                          <xsl:with-param name="b" select="$actual_font/@bls"/>
                          <xsl:with-param name="i" select="$actual_font/@fItalic"/>
                          <xsl:with-param name="strike" select="$actual_font/@fStrikeOut"/>
                          <xsl:with-param name="outline" select="$actual_font/@fOutline"/>
                          <xsl:with-param name="shadow" select="$actual_font/@fShadow"/>
                          <xsl:with-param name="condense" select="$actual_font/@fCondense"/>
                          <xsl:with-param name="extend" select="$actual_font/@fExtend"/>
                          <xsl:with-param name="propColor" select="$actual_font/@icv"/>   <!-- Icv -->
                          <xsl:with-param name="sz" select="$actual_font/@dyHeight"/>
                          <xsl:with-param name="u" select="$actual_font/@uls"/>
                          <xsl:with-param name="vertAlign" select="$actual_font/@sss"/>
                          <xsl:with-param name="scheme"/>
                        </xsl:call-template>
                      </xsl:if>
                      <xsl:variable name="next_pos">
                        <xsl:choose>
                          <xsl:when test="following-sibling::FormatRun"><xsl:value-of select="following-sibling::FormatRun[1]/@ich + 1"/></xsl:when>
                          <xsl:otherwise><xsl:value-of select="string-length(../@text) + 1"/></xsl:otherwise>
                        </xsl:choose>
                      </xsl:variable>
                      <xsl:variable name="run_text"><xsl:value-of select="substring(../@text, @ich + 1, $next_pos - @ich - 1)"/></xsl:variable>
                      <t>
                        <xsl:if test="starts-with($run_text, ' ') or substring($run_text, string-length($run_text), 1) = ' ' ">
                          <xsl:attribute name="xml:space">preserve</xsl:attribute>
                        </xsl:if>
                        <xsl:value-of select="$run_text"/>
                      </t>
                    </r>
                  </xsl:for-each>
                </xsl:when>
                <xsl:otherwise>
                  <t>
                    <xsl:value-of select="@text"/>
                  </t>
                </xsl:otherwise>
              </xsl:choose>
              <!-- 'rPh' and 'phoneticPr' tags are omitted here due to the beta restrictions. TODO: implement -->
            </si>
          </xsl:for-each>        
        </sst>
      </xlsx:file>
    </xsl:if>
  </xsl:template>
  
</xsl:stylesheet>