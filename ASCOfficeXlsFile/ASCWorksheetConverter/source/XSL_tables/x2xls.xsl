<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:xlsx="http://www.avs4you.ru/XLSXConverter/1.0/DirectoryStructure"
                xmlns:r="http://schemas.openxmlformats.org/package/2006/relationships"
                xmlns:mainr="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
                xmlns:types="http://schemas.openxmlformats.org/package/2006/content-types"
                xmlns:main="http://schemas.openxmlformats.org/spreadsheetml/2006/main"
                xmlns:msxsl="urn:schemas-microsoft-com:xslt"
                xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main"
                extension-element-prefixes="msxsl">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>


  <xsl:template match="/">
    <xsl:apply-templates select="root"/>
  </xsl:template>

  <!-- First of all we need to get references to all parts of XLSX we will use -->
  <xsl:variable name="theme_file_result_tree">
    <xsl:call-template name="FindTheme"/>
  </xsl:variable>
  <xsl:variable name="theme_file" select="msxsl:node-set($theme_file_result_tree)"/>
  
  <xsl:variable name="stylesheet_file_result_tree">
    <xsl:call-template name="FindStyles"/>
  </xsl:variable>
  <xsl:variable name="stylesheet_file" select="msxsl:node-set($stylesheet_file_result_tree)"/>

  <xsl:variable name="cellStyleXfs_count">
    <xsl:choose>
      <xsl:when test="count($stylesheet_file/main:styleSheet/main:cellStyleXfs/main:xf) > 15">
        <xsl:value-of select="count($stylesheet_file/main:styleSheet/main:cellStyleXfs/main:xf)"/>
      </xsl:when>
      <xsl:otherwise>15</xsl:otherwise>
    </xsl:choose>
  </xsl:variable>

  <xsl:variable name="shared_strings_file_result_tree">
    <xsl:call-template name="FindSharedStrings"/>
  </xsl:variable>
  <xsl:variable name="shared_strings_file" select="msxsl:node-set($shared_strings_file_result_tree)"/>
 
  <xsl:variable name="workbook_file_result_tree">
    <xsl:call-template name="FindWorkBook"/>
  </xsl:variable>
  <xsl:variable name="workbook_file" select="msxsl:node-set($workbook_file_result_tree)"/>
 
  <xsl:variable name="worksheets_files_result_tree">
    <xsl:call-template name="FindWorksheets"/>
  </xsl:variable>
  <xsl:variable name="worksheets_files" select="msxsl:node-set($worksheets_files_result_tree)"/>

  <xsl:variable name="root" select="/root"/>
  
  <xsl:template match="root">
    <root id="{@id}">
      <!-- Now let's construct the result -->
      <WorkbookStreamObject>
        <xsl:call-template name="GlobalsSubstream"/>
        <xsl:call-template name="WorksheetSubstream"/>
      </WorkbookStreamObject>
    </root>
  </xsl:template>
  
  <xsl:template name="FindWorkBook">
    <xsl:call-template name="getReferenceByPath">
      <xsl:with-param name="path" select="$wbpath"/> <!-- Defined as a global variable -->
    </xsl:call-template>
  </xsl:template>


  <xsl:variable name="wbpath" select="/root/file[@name = '[Content_Types].xml']/types:Types/types:Override[@ContentType = 'application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml']/@PartName"/>
  <xsl:variable name="wbpath_no_filename">
    <xsl:call-template name="filepath2dirpath">
      <xsl:with-param name="filepath" select="$wbpath"/>
    </xsl:call-template>
  </xsl:variable>
  <xsl:variable name="sheets_paths">
    <xsl:for-each select="$workbook_file/main:workbook/main:sheets/main:sheet">
      <sheet_path>
        <xsl:variable name="rId"><xsl:value-of select="@mainr:id"/></xsl:variable>
        <xsl:variable name="sheet_local_path">
          <xsl:value-of select="$workbook_file/r:Relationships/r:Relationship[@Id = $rId]/@Target"/>
        </xsl:variable>
        <xsl:value-of select="concat($wbpath_no_filename, $sheet_local_path)"/>
      </sheet_path>
    </xsl:for-each>
  </xsl:variable>
  
  
  <xsl:template name="FindWorksheets">
    <xsl:variable name="overrides" select="/root/file[@name = '[Content_Types].xml']/types:Types/types:Override[@ContentType = 'application/vnd.openxmlformats-officedocument.spreadsheetml.worksheet+xml']"/>
    <xsl:for-each select="$overrides">
      <xsl:variable name="position"><xsl:value-of select="position()"/></xsl:variable>
      <xsl:call-template name="getReferenceByPath">
        <xsl:with-param name="path" select="msxsl:node-set($sheets_paths)/sheet_path[position() = $position]"/>
      </xsl:call-template>
    </xsl:for-each>

  </xsl:template>
  
  
  <xsl:template name="FindStyles">
    <xsl:variable name="stpath" select="/root/file[@name = '[Content_Types].xml']/types:Types/types:Override[@ContentType = 'application/vnd.openxmlformats-officedocument.spreadsheetml.styles+xml']/@PartName"/>
    <xsl:call-template name="getReferenceByPath">
      <xsl:with-param name="path" select="$stpath"/>
    </xsl:call-template>
  </xsl:template>

  
  <xsl:template name="FindSharedStrings">
    <xsl:variable name="sstpath" select="/root/file[@name = '[Content_Types].xml']/types:Types/types:Override[@ContentType = 'application/vnd.openxmlformats-officedocument.spreadsheetml.sharedStrings+xml']/@PartName"/>
    <xsl:call-template name="getReferenceByPath">
      <xsl:with-param name="path" select="$sstpath"/>
    </xsl:call-template>
  </xsl:template>

  
  <xsl:template name="FindTheme">
    <xsl:variable name="theme_path" select="/root/file[@name = '[Content_Types].xml']/types:Types/types:Override[@ContentType = 'application/vnd.openxmlformats-officedocument.theme+xml']/@PartName"/>
    <xsl:call-template name="getReferenceByPath">
      <xsl:with-param name="path" select="$theme_path"/>
    </xsl:call-template>
  </xsl:template>


  <xsl:template name="getReferenceByPath">
    <xsl:param name="path"/>
    <!--
    <xsl:message>path=<xsl:value-of select="$path"/></xsl:message>
    -->
    <xsl:choose>
      <xsl:when test="starts-with($path, '/')"> <!-- Explicit start from root -->
        <xsl:for-each select="$root">
          <xsl:call-template name="getReferenceByPathHelper">
            <xsl:with-param name="path" select="substring-after($path, '/')"/>
          </xsl:call-template>
        </xsl:for-each>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="getReferenceByPathHelper">
          <xsl:with-param name="path" select="$path"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
    
  <xsl:template name="getReferenceByPathHelper">
    <xsl:param name="path"/>
    <xsl:variable name="dir_to_jump" select="substring-before($path, '/')"/>
    <!--
    <xsl:message>path=<xsl:value-of select="$path"/></xsl:message>
    -->
    <xsl:choose>
      <xsl:when test="starts-with($path, '/') or $dir_to_jump = '.'"> <!-- Need no jump -->
        <xsl:call-template name="getReferenceByPathHelper">
          <xsl:with-param name="path" select="substring-after($path, '/')"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="$dir_to_jump = '..'"> <!-- Need to jump up -->
        <xsl:for-each select="..">
          <xsl:call-template name="getReferenceByPathHelper">
            <xsl:with-param name="path" select="substring-after($path, '/')"/>
          </xsl:call-template>
        </xsl:for-each>
      </xsl:when>
      <xsl:when test="$dir_to_jump != ''"> <!-- Need to jump -->
        <xsl:for-each select="dir[@name = $dir_to_jump]"> <!-- We assume here that there is no duplicate names in one directory -->
          <xsl:call-template name="getReferenceByPathHelper">
            <xsl:with-param name="path" select="substring-after($path, '/')"/>
          </xsl:call-template>
        </xsl:for-each>
      </xsl:when>
      <xsl:otherwise>
        <xsl:for-each select="file[@name = $path]">
          <!-- Get reference to the content of the file -->
          <xsl:copy-of select="*"/>
          <!-- Get reference to the content of the Relationships file of the found file -->
          <xsl:copy-of select="../dir[@name = '_rels']/file[@name = concat($path, '.rels')]/r:Relationships"/>
        </xsl:for-each>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>


  <xsl:template name="filepath2dirpath">
    <xsl:param name="filepath"/>
    <xsl:value-of select="substring-before($filepath, '/')"></xsl:value-of>
    <xsl:text>/</xsl:text>
    <xsl:if test="contains(substring-after($filepath, '/'), '/')">
      <xsl:call-template name="filepath2dirpath">
        <xsl:with-param name="filepath" select="substring-after($filepath, '/')"/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>
  
</xsl:stylesheet>