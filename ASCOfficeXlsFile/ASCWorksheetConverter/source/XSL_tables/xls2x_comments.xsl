<?xml version="1.0"?>
<xsl:stylesheet xmlns:xlsx="http://www.avs4you.ru/XLSXConverter/1.0/DirectoryStructure"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <!--<xsl:key name="distinctState" match="Note/NoteSh" use="."></xsl:key>-->
  
  <xsl:template match="WorksheetSubstream[Note]" mode="comments">
    <xlsx:file>
      
      <xsl:attribute name="name">comments<xsl:value-of select="count(preceding-sibling::WorksheetSubstream[Note]) + 1"/>.xml</xsl:attribute>
      <comments xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <authors>

          <!--<xsl:for-each select="Note/NoteSh[generate-id() = generate-id(key('distinctState', .))]">
            <author>
              <xsl:value-of select="@stAuthor"></xsl:value-of>
            </author>
          </xsl:for-each>-->
          
          <!--<xsl:for-each select="Note/NoteSh">
            <author>
              <xsl:value-of select="@stAuthor"></xsl:value-of>
            </author>
          </xsl:for-each>-->

          <!-- TODO Anylize authors -->
          <author>Author</author>
            
        </authors>
        <commentList>
          <xsl:for-each select="Note/NoteSh">
            <xsl:variable name="noteId" select="@idObj"></xsl:variable>
            <xsl:variable name="cellRef" select="@ref"></xsl:variable>
            <xsl:variable name="authorPos" select="position() - 1"></xsl:variable>
            <xsl:variable name="authorName" select="@stAuthor"></xsl:variable>

            <xsl:for-each select="../../OBJECTS/OBJ/Obj/FtCmo">
              <xsl:if test="@id = $noteId">
                <xsl:variable name="textObjectId" select="position()"></xsl:variable>

                <comment ref="{$cellRef}" authorId="0">
                <!--<comment ref="{$cellRef}" authorId="{$authorPos}">-->
                  <text>
                    <r>
                      <rPr>
                        <b />
                        <sz val="9" />
                        <color indexed="81" />
                        <rFont val="Tahoma" />
                        <charset val="1" />
                      </rPr>
                      <t>
                        <xsl:value-of select="concat($authorName, ':')"/>
                      </t>
                    </r>
                    <r>
                      <rPr>
                        <sz val="9"/>
                        <color indexed="81"/>
                        <rFont val="Tahoma"/>
                        <charset val="1"/>
                      </rPr>
                      
                      <t xml:space="preserve"><xsl:call-template name="replace2">
                                                <xsl:with-param name="input" select="../../../TEXTOBJECT[ $textObjectId ]/TxO/@commentText"/>
                                                <xsl:with-param name="from" select="concat($authorName, ':')"/>
                                                <xsl:with-param name="to" select="''"/>
                                              </xsl:call-template></t>
                      
                    </r>
                  </text>
                </comment>

              </xsl:if>
            </xsl:for-each>

          </xsl:for-each>
        </commentList>
      </comments>
    </xlsx:file>
  </xsl:template>

  <xsl:template name="replace2">
    <xsl:param name="input"/>
    <xsl:param name="from"/>
    <xsl:param name="to"/>

    <xsl:choose>
      <xsl:when test="contains($input, $from)">
        <!--   вывод подстроки предшествующей образцу  + вывод строки замены -->
        <xsl:value-of select="substring-before($input, $from)"/>
        <xsl:value-of select="$to"/>


        <!--   вход в итерацию -->
        <xsl:call-template name="replace2">
          <!--  в качестве входного параметра задается подстрока после образца замены  -->
          <xsl:with-param name="input" select="substring-after($input, $from)"/>
          <xsl:with-param name="from" select="$from"/>
          <xsl:with-param name="to" select="$to"/>
        </xsl:call-template>

      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$input"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  
</xsl:stylesheet>