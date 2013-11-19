<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:number="urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0"
  xmlns:wp="http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:dc="http://purl.org/dc/elements/1.1/"
  xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns="http://schemas.openxmlformats.org/package/2006/relationships"
  xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
  xmlns:oox="urn:oox"
  exclude-result-prefixes="w oox">

  <xsl:key name="footnotes" match="w:footnoteReference" use="''"/>
  <xsl:key name="endnotes" match="w:endnoteReference" use="''"/>

  <xsl:template match="w:footnoteReference">
    <xsl:variable name="footnoteId" select="@w:id"/>
    <xsl:choose>
      <xsl:when test="@w:customMarkFollows='1' or @w:customMarkFollows='true' or @w:customMarkFollows='on'">
      </xsl:when>
      <xsl:otherwise>
        <xsl:for-each select="key('Part', 'word/footnotes.xml')/w:footnotes/w:footnote[@w:id=$footnoteId]">

          <!-- Считаем количество предыдущих w:footnoteRef, это значение
               и есть номер ссылки -->
          <xsl:number value="count(preceding::w:footnoteRef) + 1" format="1"/>
          <xsl:value-of select="number"/>

        </xsl:for-each>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="w:footnoteRef">

    <!-- Считаем количество предыдущих w:footnoteRef, это значение
               и есть номер ссылки -->
    <xsl:number value="count(preceding::w:footnoteRef) + 1" format="1"/>
    <xsl:value-of select="concat(number, ' ')"/>

  </xsl:template>

  <xsl:template match="w:footnote">
    
    <!-- Для начала узнаем, есть ли вообще какие-нибудь ссылки -->
    <xsl:if test="(count(following::w:footnoteRef) + count(child::w:footnoteRef)) &gt; -1">

      <xsl:variable name="ftnID">
        <xsl:value-of select="@w:id"/>
      </xsl:variable>

      <xsl:if test="count( preceding-sibling::w:footnote ) = 0">
        <hr xmlns="http://www.w3.org/1999/xhtml"/>"
      </xsl:if>

      <xsl:apply-templates>
        <xsl:with-param name="ftnID" select="$ftnID"/>
      </xsl:apply-templates>

    </xsl:if>

  </xsl:template>

  <xsl:template match="w:endnoteReference">
    <xsl:variable name="endnoteId" select="@w:id"/>
    <xsl:choose>
      <xsl:when test="@w:customMarkFollows='1' or @w:customMarkFollows='true' or @w:customMarkFollows='on'">
      </xsl:when>
      <xsl:otherwise>
        <xsl:for-each select="key('Part', 'word/endnotes.xml')/w:endnotes/w:endnote[@w:id=$endnoteId]">

          <!-- Считаем количество предыдущих w:footnoteRef, это значение
               и есть номер ссылки -->
          <xsl:number value="count(preceding::w:endnoteRef) + 1" format="1"/>
          <xsl:value-of select="number"/>

        </xsl:for-each>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="w:endnoteRef">

    <!-- Считаем количество предыдущих w:footnoteRef, это значение
               и есть номер ссылки -->
    <xsl:number value="count(preceding::w:endnoteRef) + 1" format="1"/>
    <xsl:value-of select="number"/>

  </xsl:template>

  <xsl:template match="w:endnote">

    <!-- Для начала узнаем, есть ли вообще какие-нибудь ссылки -->
    <xsl:if test="(count(following::w:endnoteRef) + count(child::w:endnoteRef)) &gt; 0">

      <xsl:variable name="ednID">
        <xsl:value-of select="@w:id"/>
      </xsl:variable>

      <xsl:if test="count( preceding-sibling::w:endnote ) = 0">
        <hr xmlns="http://www.w3.org/1999/xhtml"/>"
      </xsl:if>

      <xsl:apply-templates>
        <xsl:with-param name="ednID" select="$ednID"/>
      </xsl:apply-templates>

    </xsl:if>

  </xsl:template>

</xsl:stylesheet>
