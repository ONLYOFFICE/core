<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:msxsl="urn:schemas-microsoft-com:xslt"
                extension-element-prefixes="msxsl">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>

  <xsl:template name="BorderStyle_IDToString" >
    <xsl:param name="id"/>
    <xsl:choose>
      <xsl:when test="$id = 1">thin</xsl:when>
      <xsl:when test="$id = 2">medium</xsl:when>
      <xsl:when test="$id = 3">dashed</xsl:when>
      <xsl:when test="$id = 4">dotted</xsl:when>
      <xsl:when test="$id = 5">thick</xsl:when>
      <xsl:when test="$id = 6">double</xsl:when>
      <xsl:when test="$id = 7">hair</xsl:when>
      <xsl:when test="$id = 8">mediumDashed</xsl:when>
      <xsl:when test="$id = 9">dashDot</xsl:when>
      <xsl:when test="$id = 10">mediumDashDot</xsl:when>
      <xsl:when test="$id = 11">dashDotDot</xsl:when>
      <xsl:when test="$id = 12">mediumDashDotDot</xsl:when>
      <xsl:when test="$id = 13">slantDashDot</xsl:when>
      <xsl:when test="$id = 0">none</xsl:when>
      <xsl:otherwise>none</xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  
  <xsl:template name="BorderStyle_StringToID" >
    <xsl:param name="string"/>
    <xsl:choose>
      <xsl:when test="$string = 'thin'">1</xsl:when>
      <xsl:when test="$string = 'medium'">2</xsl:when>
      <xsl:when test="$string = 'dashed'">3</xsl:when>
      <xsl:when test="$string = 'dotted'">4</xsl:when>
      <xsl:when test="$string = 'thick'">5</xsl:when>
      <xsl:when test="$string = 'double'">6</xsl:when>
      <xsl:when test="$string = 'hair'">7</xsl:when>
      <xsl:when test="$string = 'mediumDashed'">8</xsl:when>
      <xsl:when test="$string = 'dashDot'">9</xsl:when>
      <xsl:when test="$string = 'mediumDashDot'">10</xsl:when>
      <xsl:when test="$string = 'dashDotDot'">11</xsl:when>
      <xsl:when test="$string = 'mediumDashDotDot'">12</xsl:when>
      <xsl:when test="$string = 'slantDashDot'">13</xsl:when>
      <xsl:when test="$string = 'none'"></xsl:when>
      <xsl:otherwise>0</xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  
  <xsl:template name="PatternType_IDToString" >
    <xsl:param name="id"/>
    <xsl:choose>
      <xsl:when test="$id = 1">solid</xsl:when>
      <xsl:when test="$id = 2">mediumGray</xsl:when>
      <xsl:when test="$id = 3">darkGray</xsl:when>
      <xsl:when test="$id = 4">lightGray</xsl:when>
      <xsl:when test="$id = 5">darkHorizontal</xsl:when>
      <xsl:when test="$id = 6">darkVertical</xsl:when>
      <xsl:when test="$id = 7">darkDown</xsl:when>
      <xsl:when test="$id = 8">darkUp</xsl:when>
      <xsl:when test="$id = 9">darkGrid</xsl:when>
      <xsl:when test="$id = 10">darkTrellis</xsl:when>
      <xsl:when test="$id = 11">lightHorizontal</xsl:when>
      <xsl:when test="$id = 12">lightVertical</xsl:when>
      <xsl:when test="$id = 13">lightDown</xsl:when>
      <xsl:when test="$id = 14">lightUp</xsl:when>
      <xsl:when test="$id = 15">lightGrid</xsl:when>
      <xsl:when test="$id = 16">lightTrellis</xsl:when>
      <xsl:when test="$id = 17">gray125</xsl:when>
      <xsl:when test="$id = 18">gray0625</xsl:when>
      <xsl:when test="$id = 0">none</xsl:when>
      <xsl:otherwise>none</xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  
  <xsl:template name="PatternType_StringToID" >
    <xsl:param name="string"/>
    <xsl:choose>
      <xsl:when test="$string = 'solid'">1</xsl:when>
      <xsl:when test="$string = 'mediumGray'">2</xsl:when>
      <xsl:when test="$string = 'darkGray'">3</xsl:when>
      <xsl:when test="$string = 'lightGray'">4</xsl:when>
      <xsl:when test="$string = 'darkHorizontal'">5</xsl:when>
      <xsl:when test="$string = 'darkVertical'">6</xsl:when>
      <xsl:when test="$string = 'darkDown'">7</xsl:when>
      <xsl:when test="$string = 'darkUp'">8</xsl:when>
      <xsl:when test="$string = 'darkGrid'">9</xsl:when>
      <xsl:when test="$string = 'darkTrellis'">10</xsl:when>
      <xsl:when test="$string = 'lightHorizontal'">11</xsl:when>
      <xsl:when test="$string = 'lightVertical'">12</xsl:when>
      <xsl:when test="$string = 'lightDown'">13</xsl:when>
      <xsl:when test="$string = 'lightUp'">14</xsl:when>
      <xsl:when test="$string = 'lightGrid'">15</xsl:when>
      <xsl:when test="$string = 'lightTrellis'">16</xsl:when>
      <xsl:when test="$string = 'gray125'">17</xsl:when>
      <xsl:when test="$string = 'gray0625'">18</xsl:when>
      <xsl:when test="$string = 'none'">0</xsl:when>
      <xsl:otherwise>0</xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  
  <xsl:template name="HorizontalAlignment_IDToString" >
    <xsl:param name="id"/>
    <xsl:choose>
      <xsl:when test="$id = 1">left</xsl:when>
      <xsl:when test="$id = 2">center</xsl:when>
      <xsl:when test="$id = 3">right</xsl:when>
      <xsl:when test="$id = 4">fill</xsl:when>
      <xsl:when test="$id = 5">justify</xsl:when>
      <xsl:when test="$id = 6">centerContinuous</xsl:when>
      <xsl:when test="$id = 7">distributed</xsl:when>
      <xsl:when test="$id = 0">general</xsl:when>
      <xsl:otherwise>general</xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  
  <xsl:template name="HorizontalAlignment_StringToID" >
    <xsl:param name="string"/>
    <xsl:choose>
      <xsl:when test="$string = 'left'">1</xsl:when>
      <xsl:when test="$string = 'center'">2</xsl:when>
      <xsl:when test="$string = 'right'">3</xsl:when>
      <xsl:when test="$string = 'fill'">4</xsl:when>
      <xsl:when test="$string = 'justify'">5</xsl:when>
      <xsl:when test="$string = 'centerContinuous'">6</xsl:when>
      <xsl:when test="$string = 'distributed'">7</xsl:when>
      <xsl:when test="$string = 'general'">0</xsl:when>
      <xsl:otherwise>0</xsl:otherwise>
    </xsl:choose>
  </xsl:template>  
  
  <xsl:template name="VerticalAlignment_IDToString" >
    <xsl:param name="id"/>
    <xsl:choose>
      <xsl:when test="$id = 0">top</xsl:when>
      <xsl:when test="$id = 1">center</xsl:when>
      <xsl:when test="$id = 2">bottom</xsl:when>
      <xsl:when test="$id = 3">justify</xsl:when>
      <xsl:when test="$id = 4">distributed</xsl:when>
      <xsl:otherwise>bottom</xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="VerticalAlignment_StringToID" >
    <xsl:param name="string"/>
    <xsl:choose>
      <xsl:when test="$string = 'top'">0</xsl:when>
      <xsl:when test="$string = 'center'">1</xsl:when>
      <xsl:when test="$string = 'bottom'">2</xsl:when>
      <xsl:when test="$string = 'justify'">3</xsl:when>
      <xsl:when test="$string = 'distributed'">4</xsl:when>
      <xsl:otherwise>2</xsl:otherwise>
    </xsl:choose>
  </xsl:template>  
  
  <xsl:template name="SheetTypeURI_StringToID" >
    <xsl:param name="string"/>
    <xsl:choose>
      <xsl:when test="$string = 'http://schemas.openxmlformats.org/officeDocument/2006/relationships/worksheet'">0</xsl:when>
      <xsl:when test="$string = 'http://schemas.openxmlformats.org/officeDocument/2006/relationships/dialogsheet'">0</xsl:when>
      <xsl:when test="$string = 'http://schemas.microsoft.com/office/2006/relationships/xlMacrosheet'">1</xsl:when>
      <xsl:when test="$string = 'http://schemas.microsoft.com/office/2006/relationships/xlIntlMacrosheet'">1</xsl:when>
      <xsl:when test="$string = 'http://schemas.openxmlformats.org/officeDocument/2006/relationships/chartsheet'">2</xsl:when>
      <xsl:when test="$string = 'http://schemas.microsoft.com/office/2006/relationships/vbaProject'">6</xsl:when>
    </xsl:choose>
  </xsl:template>  
  
  
  
  <xsl:variable name="indexedColors">
    <indexedColors xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
      <rgbColor rgb="00000000"/>
      <rgbColor rgb="00FFFFFF"/>
      <rgbColor rgb="00FF0000"/>
      <rgbColor rgb="0000FF00"/>
      <rgbColor rgb="000000FF"/>
      <rgbColor rgb="00FFFF00"/>
      <rgbColor rgb="00FF00FF"/>
      <rgbColor rgb="0000FFFF"/>
      <rgbColor rgb="00000000"/>
      <rgbColor rgb="00FFFFFF"/>
      <rgbColor rgb="00FF0000"/>
      <rgbColor rgb="0000FF00"/>
      <rgbColor rgb="000000FF"/>
      <rgbColor rgb="00FFFF00"/>
      <rgbColor rgb="00FF00FF"/>
      <rgbColor rgb="0000FFFF"/>
      <rgbColor rgb="00800000"/>
      <rgbColor rgb="00008000"/>
      <rgbColor rgb="00000080"/>
      <rgbColor rgb="00808000"/>
      <rgbColor rgb="00800080"/>
      <rgbColor rgb="00008080"/>
      <rgbColor rgb="00C0C0C0"/>
      <rgbColor rgb="00808080"/>
      <rgbColor rgb="009999FF"/>
      <rgbColor rgb="00993366"/>
      <rgbColor rgb="00FFFFCC"/>
      <rgbColor rgb="00CCFFFF"/>
      <rgbColor rgb="00660066"/>
      <rgbColor rgb="00FF8080"/>
      <rgbColor rgb="000066CC"/>
      <rgbColor rgb="00CCCCFF"/>
      <rgbColor rgb="00000080"/>
      <rgbColor rgb="00FF00FF"/>
      <rgbColor rgb="00FFFF00"/>
      <rgbColor rgb="0000FFFF"/>
      <rgbColor rgb="00800080"/>
      <rgbColor rgb="00800000"/>
      <rgbColor rgb="00008080"/>
      <rgbColor rgb="000000FF"/>
      <rgbColor rgb="0000CCFF"/>
      <rgbColor rgb="00CCFFFF"/>
      <rgbColor rgb="00CCFFCC"/>
      <rgbColor rgb="00FFFF99"/>
      <rgbColor rgb="0099CCFF"/>
      <rgbColor rgb="00FF99CC"/>
      <rgbColor rgb="00CC99FF"/>
      <rgbColor rgb="00FFCC99"/>
      <rgbColor rgb="003366FF"/>
      <rgbColor rgb="0033CCCC"/>
      <rgbColor rgb="0099CC00"/>
      <rgbColor rgb="00FFCC00"/>
      <rgbColor rgb="00FF9900"/>
      <rgbColor rgb="00FF6600"/>
      <rgbColor rgb="00666699"/>
      <rgbColor rgb="00969696"/>
      <rgbColor rgb="00003366"/>
      <rgbColor rgb="00339966"/>
      <rgbColor rgb="00003300"/>
      <rgbColor rgb="00333300"/>
      <rgbColor rgb="00993300"/>
      <rgbColor rgb="00993366"/>
      <rgbColor rgb="00333399"/>
      <rgbColor rgb="00333333"/>
    </indexedColors>
  </xsl:variable>
  
  <xsl:variable name="BuiltInFunctions">
    <BuiltInFunctions xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
      <func name="COUNT"/>
      <func name="IF"/>
      <func name="ISNA"/>
      <func name="ISERROR"/>
      <func name="SUM"/>
      <func name="AVERAGE"/>
      <func name="MIN"/>
      <func name="MAX"/>
      <func name="ROW"/>
      <func name="COLUMN"/>
      <func name="NA"/>
      <func name="NPV"/>
      <func name="STDEV"/>
      <func name="DOLLAR"/>
      <func name="FIXED"/>
      <func name="SIN"/>
      <func name="COS"/>
      <func name="TAN"/>
      <func name="ATAN"/>
      <func name="PI"/>
      <func name="SQRT"/>
      <func name="EXP"/>
      <func name="LN"/>
      <func name="LOG10"/>
      <func name="ABS"/>
      <func name="INT"/>
      <func name="SIGN"/>
      <func name="ROUND"/>
      <func name="LOOKUP"/>
      <func name="INDEX"/>
      <func name="REPT"/>
      <func name="MID"/>
      <func name="LEN"/>
      <func name="VALUE"/>
      <func name="TRUE"/>
      <func name="FALSE"/>
      <func name="AND"/>
      <func name="OR"/>
      <func name="NOT"/>
      <func name="MOD"/>
      <func name="DCOUNT"/>
      <func name="DSUM"/>
      <func name="DAVERAGE"/>
      <func name="DMIN"/>
      <func name="DMAX"/>
      <func name="DSTDEV"/>
      <func name="VAR"/>
      <func name="DVAR"/>
      <func name="TEXT"/>
      <func name="LINEST"/>
      <func name="TREND"/>
      <func name="LOGEST"/>
      <func name="GROWTH"/>
      <func name="GOTO"/>
      <func name="HALT"/>
      <func name="RETURN"/>
      <func name="PV"/>
      <func name="FV"/>
      <func name="NPER"/>
      <func name="PMT"/>
      <func name="RATE"/>
      <func name="MIRR"/>
      <func name="IRR"/>
      <func name="RAND"/>
      <func name="MATCH"/>
      <func name="DATE"/>
      <func name="TIME"/>
      <func name="DAY"/>
      <func name="MONTH"/>
      <func name="YEAR"/>
      <func name="WEEKDAY"/>
      <func name="HOUR"/>
      <func name="MINUTE"/>
      <func name="SECOND"/>
      <func name="NOW"/>
      <func name="AREAS"/>
      <func name="ROWS"/>
      <func name="COLUMNS"/>
      <func name="OFFSET"/>
      <func name="ABSREF"/>
      <func name="RELREF"/>
      <func name="ARGUMENT"/>
      <func name="SEARCH"/>
      <func name="TRANSPOSE"/>
      <func name="ERROR"/>
      <func name="STEP"/>
      <func name="TYPE"/>
      <func name="ECHO"/>
      <func name="SET.NAME"/>
      <func name="CALLER"/>
      <func name="DEREF"/>
      <func name="WINDOWS"/>
      <func name="SERIES"/>
      <func name="DOCUMENTS"/>
      <func name="ACTIVE.CELL"/>
      <func name="SELECTION"/>
      <func name="RESULT"/>
      <func name="ATAN2"/>
      <func name="ASIN"/>
      <func name="ACOS"/>
      <func name="CHOOSE"/>
      <func name="HLOOKUP"/>
      <func name="VLOOKUP"/>
      <func name="LINKS"/>
      <func name="INPUT"/>
      <func name="ISREF"/>
      <func name="GET.FORMULA"/>
      <func name="GET.NAME"/>
      <func name="SET.VALUE"/>
      <func name="LOG"/>
      <func name="EXEC"/>
      <func name="CHAR"/>
      <func name="LOWER"/>
      <func name="UPPER"/>
      <func name="PROPER"/>
      <func name="LEFT"/>
      <func name="RIGHT"/>
      <func name="EXACT"/>
      <func name="TRIM"/>
      <func name="REPLACE"/>
      <func name="SUBSTITUTE"/>
      <func name="CODE"/>
      <func name="NAMES"/>
      <func name="DIRECTORY"/>
      <func name="FIND"/>
      <func name="CELL"/>
      <func name="ISERR"/>
      <func name="ISTEXT"/>
      <func name="ISNUMBER"/>
      <func name="ISBLANK"/>
      <func name="T"/>
      <func name="N"/>
      <func name="FOPEN"/>
      <func name="FCLOSE"/>
      <func name="FSIZE"/>
      <func name="FREADLN"/>
      <func name="FREAD"/>
      <func name="FWRITELN"/>
      <func name="FWRITE"/>
      <func name="FPOS"/>
      <func name="DATEVALUE"/>
      <func name="TIMEVALUE"/>
      <func name="SLN"/>
      <func name="SYD"/>
      <func name="DDB"/>
      <func name="GET.DEF"/>
      <func name="REFTEXT"/>
      <func name="TEXTREF"/>
      <func name="INDIRECT"/>
      <func name="REGISTER"/>
      <func name="CALL"/>
      <func name="ADD.BAR"/>
      <func name="ADD.MENU"/>
      <func name="ADD.COMMAND"/>
      <func name="ENABLE.COMMAND"/>
      <func name="CHECK.COMMAND"/>
      <func name="RENAME.COMMAND"/>
      <func name="SHOW.BAR"/>
      <func name="DELETE.MENU"/>
      <func name="DELETE.COMMAND"/>
      <func name="GET.CHART.ITEM"/>
      <func name="DIALOG.BOX"/>
      <func name="CLEAN"/>
      <func name="MDETERM"/>
      <func name="MINVERSE"/>
      <func name="MMULT"/>
      <func name="FILES"/>
      <func name="IPMT"/>
      <func name="PPMT"/>
      <func name="COUNTA"/>
      <func name="CANCEL.KEY"/>
      <func name="FOR"/>
      <func name="WHILE"/>
      <func name="BREAK"/>
      <func name="NEXT"/>
      <func name="INITIATE"/>
      <func name="REQUEST"/>
      <func name="POKE"/>
      <func name="EXECUTE"/>
      <func name="TERMINATE"/>
      <func name="RESTART"/>
      <func name="HELP"/>
      <func name="GET.BAR"/>
      <func name="PRODUCT"/>
      <func name="FACT"/>
      <func name="GET.CELL"/>
      <func name="GET.WORKSPACE"/>
      <func name="GET.WINDOW"/>
      <func name="GET.DOCUMENT"/>
      <func name="DPRODUCT"/>
      <func name="ISNONTEXT"/>
      <func name="GET.NOTE"/>
      <func name="NOTE"/>
      <func name="STDEVP"/>
      <func name="VARP"/>
      <func name="DSTDEVP"/>
      <func name="DVARP"/>
      <func name="TRUNC"/>
      <func name="ISLOGICAL"/>
      <func name="DCOUNTA"/>
      <func name="DELETE.BAR"/>
      <func name="UNREGISTER"/>
      <func name="USDOLLAR"/>
      <func name="FINDB"/>
      <func name="SEARCHB"/>
      <func name="REPLACEB"/>
      <func name="LEFTB"/>
      <func name="RIGHTB"/>
      <func name="MIDB"/>
      <func name="LENB"/>
      <func name="ROUNDUP"/>
      <func name="ROUNDDOWN"/>
      <func name="ASC"/>
      <func name="DBCS"/>
      <func name="RANK"/>
      <func name="ADDRESS"/>
      <func name="DAYS360"/>
      <func name="TODAY"/>
      <func name="VDB"/>
      <func name="ELSE"/>
      <func name="ELSE.IF"/>
      <func name="END.IF"/>
      <func name="FOR.CELL"/>
      <func name="MEDIAN"/>
      <func name="SUMPRODUCT"/>
      <func name="SINH"/>
      <func name="COSH"/>
      <func name="TANH"/>
      <func name="ASINH"/>
      <func name="ACOSH"/>
      <func name="ATANH"/>
      <func name="DGET"/>
      <func name="CREATE.OBJECT"/>
      <func name="VOLATILE"/>
      <func name="LAST.ERROR"/>
      <func name="CUSTOM.UNDO"/>
      <func name="CUSTOM.REPEAT"/>
      <func name="FORMULA.CONVERT"/>
      <func name="GET.LINK.INFO"/>
      <func name="TEXT.BOX"/>
      <func name="INFO"/>
      <func name="GROUP"/>
      <func name="GET.OBJECT"/>
      <func name="DB"/>
      <func name="PAUSE"/>
      <func name="RESUME"/>
      <func name="FREQUENCY"/>
      <func name="ADD.TOOLBAR"/>
      <func name="DELETE.TOOLBAR"/>
      <func name="RESET.TOOLBAR"/>
      <func name="EVALUATE"/>
      <func name="GET.TOOLBAR"/>
      <func name="GET.TOOL"/>
      <func name="SPELLING.CHECK"/>
      <func name="ERROR.TYPE"/>
      <func name="APP.TITLE"/>
      <func name="WINDOW.TITLE"/>
      <func name="SAVE.TOOLBAR"/>
      <func name="ENABLE.TOOL"/>
      <func name="PRESS.TOOL"/>
      <func name="REGISTER.ID"/>
      <func name="GET.WORKBOOK"/>
      <func name="AVEDEV"/>
      <func name="BETADIST"/>
      <func name="GAMMALN"/>
      <func name="BETAINV"/>
      <func name="BINOMDIST"/>
      <func name="CHIDIST"/>
      <func name="CHIINV"/>
      <func name="COMBIN"/>
      <func name="CONFIDENCE"/>
      <func name="CRITBINOM"/>
      <func name="EVEN"/>
      <func name="EXPONDIST"/>
      <func name="FDIST"/>
      <func name="FINV"/>
      <func name="FISHER"/>
      <func name="FISHERINV"/>
      <func name="FLOOR"/>
      <func name="GAMMADIST"/>
      <func name="GAMMAINV"/>
      <func name="CEILING"/>
      <func name="HYPGEOMDIST"/>
      <func name="LOGNORMDIST"/>
      <func name="LOGINV"/>
      <func name="NEGBINOMDIST"/>
      <func name="NORMDIST"/>
      <func name="NORMSDIST"/>
      <func name="NORMINV"/>
      <func name="NORMSINV"/>
      <func name="STANDARDIZE"/>
      <func name="ODD"/>
      <func name="PERMUT"/>
      <func name="POISSON"/>
      <func name="TDIST"/>
      <func name="WEIBULL"/>
      <func name="SUMXMY2"/>
      <func name="SUMX2MY2"/>
      <func name="SUMX2PY2"/>
      <func name="CHITEST"/>
      <func name="CORREL"/>
      <func name="COVAR"/>
      <func name="FORECAST"/>
      <func name="FTEST"/>
      <func name="INTERCEPT"/>
      <func name="PEARSON"/>
      <func name="RSQ"/>
      <func name="STEYX"/>
      <func name="SLOPE"/>
      <func name="TTEST"/>
      <func name="PROB"/>
      <func name="DEVSQ"/>
      <func name="GEOMEAN"/>
      <func name="HARMEAN"/>
      <func name="SUMSQ"/>
      <func name="KURT"/>
      <func name="SKEW"/>
      <func name="ZTEST"/>
      <func name="LARGE"/>
      <func name="SMALL"/>
      <func name="QUARTILE"/>
      <func name="PERCENTILE"/>
      <func name="PERCENTRANK"/>
      <func name="MODE"/>
      <func name="TRIMMEAN"/>
      <func name="TINV"/>
      <func name="MOVIE.COMMAND"/>
      <func name="GET.MOVIE"/>
      <func name="CONCATENATE"/>
      <func name="POWER"/>
      <func name="PIVOT.ADD.DATA"/>
      <func name="GET.PIVOT.TABLE"/>
      <func name="GET.PIVOT.FIELD"/>
      <func name="GET.PIVOT.ITEM"/>
      <func name="RADIANS"/>
      <func name="DEGREES"/>
      <func name="SUBTOTAL"/>
      <func name="SUMIF"/>
      <func name="COUNTIF"/>
      <func name="COUNTBLANK"/>
      <func name="SCENARIO.GET"/>
      <func name="OPTIONS.LISTS.GET"/>
      <func name="ISPMT"/>
      <func name="DATEDIF"/>
      <func name="DATESTRING"/>
      <func name="NUMBERSTRING"/>
      <func name="ROMAN"/>
      <func name="OPEN.DIALOG"/>
      <func name="SAVE.DIALOG"/>
      <func name="VIEW.GET"/>
      <func name="GETPIVOTDATA"/>
      <func name="HYPERLINK"/>
      <func name="PHONETIC"/>
      <func name="AVERAGEA"/>
      <func name="MAXA"/>
      <func name="MINA"/>
      <func name="STDEVPA"/>
      <func name="VARPA"/>
      <func name="STDEVA"/>
      <func name="VARA"/>
      <func name="BAHTTEXT"/>
      <func name="THAIDAYOFWEEK"/>
      <func name="THAIDIGIT"/>
      <func name="THAIMONTHOFYEAR"/>
      <func name="THAINUMSOUND"/>
      <func name="THAINUMSTRING"/>
      <func name="THAISTRINGLENGTH"/>
      <func name="ISTHAIDIGIT"/>
      <func name="ROUNDBAHTDOWN"/>
      <func name="ROUNDBAHTUP"/>
      <func name="THAIYEAR"/>
      <func name="RTD"/>
      <func name="BEEP"/>
      <func name="OPEN"/>
      <func name="OPEN.LINKS"/>
      <func name="CLOSE.ALL"/>
      <func name="SAVE"/>
      <func name="SAVE.AS"/>
      <func name="FILE.DELETE"/>
      <func name="PAGE.SETUP"/>
      <func name="PRINT"/>
      <func name="PRINTER.SETUP"/>
      <func name="QUIT"/>
      <func name="NEW.WINDOW"/>
      <func name="WINDOW.SIZE"/>
      <func name="WINDOW.MOVE"/>
      <func name="FULL"/>
      <func name="CLOSE"/>
      <func name="RUN"/>
      <func name="SET.PRINT.AREA"/>
      <func name="SET.PRINT.TITLES"/>
      <func name="SET.PAGE.BREAK"/>
      <func name="REMOVE.PAGE.BREAK"/>
      <func name="FONT"/>
      <func name="DISPLAY"/>
      <func name="PROTECT.DOCUMENT"/>
      <func name="PRECISION"/>
      <func name="A1.R1C1"/>
      <func name="CALCULATE.NOW"/>
      <func name="CALCULATION"/>
      <func name="DATA.FIND"/>
      <func name="EXTRACT"/>
      <func name="DATA.DELETE"/>
      <func name="SET.DATABASE"/>
      <func name="SET.CRITERIA"/>
      <func name="SORT"/>
      <func name="DATA.SERIES"/>
      <func name="TABLE"/>
      <func name="FORMAT.NUMBER"/>
      <func name="ALIGNMENT"/>
      <func name="STYLE"/>
      <func name="BORDER"/>
      <func name="CELL.PROTECTION"/>
      <func name="COLUMN.WIDTH"/>
      <func name="UNDO"/>
      <func name="CUT"/>
      <func name="COPY"/>
      <func name="PASTE"/>
      <func name="CLEAR"/>
      <func name="PASTE.SPECIAL"/>
      <func name="EDIT.DELETE"/>
      <func name="INSERT"/>
      <func name="FILL.RIGHT"/>
      <func name="FILL.DOWN"/>
      <func name="DEFINE.NAME"/>
      <func name="CREATE.NAMES"/>
      <func name="FORMULA.GOTO"/>
      <func name="FORMULA.FIND"/>
      <func name="SELECT.LAST.CELL"/>
      <func name="SHOW.ACTIVE.CELL"/>
      <func name="COMBINATION"/>
      <func name="PREFERRED"/>
      <func name="ADD.OVERLAY"/>
      <func name="GRIDLINES"/>
      <func name="SET.PREFERRED"/>
      <func name="AXES"/>
      <func name="LEGEND"/>
      <func name="ATTACH.TEXT"/>
      <func name="ADD.ARROW"/>
      <func name="SELECT.CHART"/>
      <func name="SELECT.PLOT.AREA"/>
      <func name="PATTERNS"/>
      <func name="MAIN.CHART"/>
      <func name="OVERLAY"/>
      <func name="SCALE"/>
      <func name="FORMAT.LEGEND"/>
      <func name="FORMAT.TEXT"/>
      <func name="EDIT.REPEAT"/>
      <func name="PARSE"/>
      <func name="JUSTIFY"/>
      <func name="HIDE"/>
      <func name="UNHIDE"/>
      <func name="WORKSPACE"/>
      <func name="FORMULA"/>
      <func name="FORMULA.FILL"/>
      <func name="FORMULA.ARRAY"/>
      <func name="DATA.FIND.NEXT"/>
      <func name="DATA.FIND.PREV"/>
      <func name="FORMULA.FIND.NEXT"/>
      <func name="FORMULA.FIND.PREV"/>
      <func name="ACTIVATE"/>
      <func name="ACTIVATE.NEXT"/>
      <func name="ACTIVATE.PREV"/>
      <func name="UNLOCKED.NEXT"/>
      <func name="UNLOCKED.PREV"/>
      <func name="COPY.PICTURE"/>
      <func name="SELECT"/>
      <func name="DELETE.NAME"/>
      <func name="DELETE.FORMAT"/>
      <func name="VLINE"/>
      <func name="HLINE"/>
      <func name="VPAGE"/>
      <func name="HPAGE"/>
      <func name="VSCROLL"/>
      <func name="HSCROLL"/>
      <func name="ALERT"/>
      <func name="NEW"/>
      <func name="CANCEL.COPY"/>
      <func name="SHOW.CLIPBOARD"/>
      <func name="MESSAGE"/>
      <func name="PASTE.LINK"/>
      <func name="APP.ACTIVATE"/>
      <func name="DELETE.ARROW"/>
      <func name="ROW.HEIGHT"/>
      <func name="FORMAT.MOVE"/>
      <func name="FORMAT.SIZE"/>
      <func name="FORMULA.REPLACE"/>
      <func name="SEND.KEYS"/>
      <func name="SELECT.SPECIAL"/>
      <func name="APPLY.NAMES"/>
      <func name="REPLACE.FONT"/>
      <func name="FREEZE.PANES"/>
      <func name="SHOW.INFO"/>
      <func name="SPLIT"/>
      <func name="ON.WINDOW"/>
      <func name="ON.DATA"/>
      <func name="DISABLE.INPUT"/>
      <func name="OUTLINE"/>
      <func name="LIST.NAMES"/>
      <func name="FILE.CLOSE"/>
      <func name="SAVE.WORKBOOK"/>
      <func name="DATA.FORM"/>
      <func name="COPY.CHART"/>
      <func name="ON.TIME"/>
      <func name="WAIT"/>
      <func name="FORMAT.FONT"/>
      <func name="FILL.UP"/>
      <func name="FILL.LEFT"/>
      <func name="DELETE.OVERLAY"/>
      <func name="SHORT.MENUS"/>
      <func name="SET.UPDATE.STATUS"/>
      <func name="COLOR.PALETTE"/>
      <func name="DELETE.STYLE"/>
      <func name="WINDOW.RESTORE"/>
      <func name="WINDOW.MAXIMIZE"/>
      <func name="CHANGE.LINK"/>
      <func name="CALCULATE.DOCUMENT"/>
      <func name="ON.KEY"/>
      <func name="APP.RESTORE"/>
      <func name="APP.MOVE"/>
      <func name="APP.SIZE"/>
      <func name="APP.MINIMIZE"/>
      <func name="APP.MAXIMIZE"/>
      <func name="BRING.TO.FRONT"/>
      <func name="SEND.TO.BACK"/>
      <func name="MAIN.CHART.TYPE"/>
      <func name="OVERLAY.CHART.TYPE"/>
      <func name="SELECT.END"/>
      <func name="OPEN.MAIL"/>
      <func name="SEND.MAIL"/>
      <func name="STANDARD.FONT"/>
      <func name="CONSOLIDATE"/>
      <func name="SORT.SPECIAL"/>
      <func name="VIEW.3D"/>
      <func name="GOAL.SEEK"/>
      <func name="WORKGROUP"/>
      <func name="FILL.GROUP"/>
      <func name="UPDATE.LINK"/>
      <func name="PROMOTE"/>
      <func name="DEMOTE"/>
      <func name="SHOW.DETAIL"/>
      <func name="UNGROUP"/>
      <func name="OBJECT.PROPERTIES"/>
      <func name="SAVE.NEW.OBJECT"/>
      <func name="SHARE"/>
      <func name="SHARE.NAME"/>
      <func name="DUPLICATE"/>
      <func name="APPLY.STYLE"/>
      <func name="ASSIGN.TO.OBJECT"/>
      <func name="OBJECT.PROTECTION"/>
      <func name="HIDE.OBJECT"/>
      <func name="SET.EXTRACT"/>
      <func name="CREATE.PUBLISHER"/>
      <func name="SUBSCRIBE.TO"/>
      <func name="ATTRIBUTES"/>
      <func name="SHOW.TOOLBAR"/>
      <func name="PRINT.PREVIEW"/>
      <func name="EDIT.COLOR"/>
      <func name="SHOW.LEVELS"/>
      <func name="FORMAT.MAIN"/>
      <func name="FORMAT.OVERLAY"/>
      <func name="ON.RECALC"/>
      <func name="EDIT.SERIES"/>
      <func name="DEFINE.STYLE"/>
      <func name="LINE.PRINT"/>
      <func name="ENTER.DATA"/>
      <func name="MERGE.STYLES"/>
      <func name="EDITION.OPTIONS"/>
      <func name="PASTE.PICTURE"/>
      <func name="PASTE.PICTURE.LINK"/>
      <func name="SPELLING"/>
      <func name="ZOOM"/>
      <func name="INSERT.OBJECT"/>
      <func name="WINDOW.MINIMIZE"/>
      <func name="SOUND.NOTE"/>
      <func name="SOUND.PLAY"/>
      <func name="FORMAT.SHAPE"/>
      <func name="EXTEND.POLYGON"/>
      <func name="FORMAT.AUTO"/>
      <func name="FILL.AUTO"/>
      <func name="CUSTOMIZE.TOOLBAR"/>
      <func name="ADD.TOOL"/>
      <func name="EDIT.OBJECT"/>
      <func name="ON.DOUBLECLICK"/>
      <func name="ON.ENTRY"/>
      <func name="WORKBOOK.ADD"/>
      <func name="WORKBOOK.MOVE"/>
      <func name="WORKBOOK.COPY"/>
      <func name="WORKBOOK.OPTIONS"/>
      <func name="SAVE.WORKSPACE"/>
      <func name="CHART.WIZARD"/>
      <func name="DELETE.TOOL"/>
      <func name="MOVE.TOOL"/>
      <func name="WORKBOOK.SELECT"/>
      <func name="WORKBOOK.ACTIVATE"/>
      <func name="ASSIGN.TO.TOOL"/>
      <func name="COPY.TOOL"/>
      <func name="RESET.TOOL"/>
      <func name="CONSTRAIN.NUMERIC"/>
      <func name="PASTE.TOOL"/>
      <func name="WORKBOOK.NEW"/>
      <func name="SCENARIO.CELLS"/>
      <func name="SCENARIO.DELETE"/>
      <func name="SCENARIO.ADD"/>
      <func name="SCENARIO.EDIT"/>
      <func name="SCENARIO.SHOW"/>
      <func name="SCENARIO.SHOW.NEXT"/>
      <func name="SCENARIO.SUMMARY"/>
      <func name="PIVOT.TABLE.WIZARD"/>
      <func name="PIVOT.FIELD.PROPERTIES"/>
      <func name="PIVOT.FIELD"/>
      <func name="PIVOT.ITEM"/>
      <func name="PIVOT.ADD.FIELDS"/>
      <func name="OPTIONS.CALCULATION"/>
      <func name="OPTIONS.EDIT"/>
      <func name="OPTIONS.VIEW"/>
      <func name="ADDIN.MANAGER"/>
      <func name="MENU.EDITOR"/>
      <func name="ATTACH.TOOLBARS"/>
      <func name="ATTACH.TOOLBARS"/>
      <func name="OPTIONS.CHART"/>
      <func name="VBA.INSERT.FILE"/>
      <func name="VBA.PROCEDURE.DEFINITION"/>
      <func name="ROUTING.SLIP"/>
      <func name="ROUTE.DOCUMENT"/>
      <func name="MAIL.LOGON"/>
      <func name="INSERT.PICTURE"/>
      <func name="EDIT.TOOL"/>
      <func name="CHART.TREND"/>
      <func name="PIVOT.ITEM.PROPERTIES"/>
      <func name="WORKBOOK.INSERT"/>
      <func name="OPTIONS.TRANSITION"/>
      <func name="OPTIONS.GENERAL"/>
      <func name="FILTER.ADVANCED"/>
      <func name="MAIL.ADD.MAILER"/>
      <func name="MAIL.DELETE.MAILER"/>
      <func name="MAIL.REPLY"/>
      <func name="MAIL.REPLY.ALL"/>
      <func name="MAIL.FORWARD"/>
      <func name="MAIL.NEXT.LETTER"/>
      <func name="DATA.LABEL"/>
      <func name="INSERT.TITLE"/>
      <func name="FONT.PROPERTIES"/>
      <func name="MACRO.OPTIONS"/>
      <func name="WORKBOOK.HIDE"/>
      <func name="WORKBOOK.UNHIDE"/>
      <func name="WORKBOOK.DELETE"/>
      <func name="WORKBOOK.NAME"/>
      <func name="ADD.CHART.AUTOFORMAT"/>
      <func name="DELETE.CHART.AUTOFORMAT"/>
      <func name="CHART.ADD.DATA"/>
      <func name="AUTO.OUTLINE"/>
      <func name="TAB.ORDER"/>
      <func name="SHOW.DIALOG"/>
      <func name="SELECT.ALL"/>
      <func name="UNGROUP.SHEETS"/>
      <func name="SUBTOTAL.CREATE"/>
      <func name="SUBTOTAL.REMOVE"/>
      <func name="RENAME.OBJECT"/>
      <func name="WORKBOOK.SCROLL"/>
      <func name="WORKBOOK.NEXT"/>
      <func name="WORKBOOK.PREV"/>
      <func name="WORKBOOK.TAB.SPLIT"/>
      <func name="FULL.SCREEN"/>
      <func name="WORKBOOK.PROTECT"/>
      <func name="SCROLLBAR.PROPERTIES"/>
      <func name="PIVOT.SHOW.PAGES"/>
      <func name="TEXT.TO.COLUMNS"/>
      <func name="FORMAT.CHARTTYPE"/>
      <func name="LINK.FORMAT"/>
      <func name="TRACER.DISPLAY"/>
      <func name="TRACER.NAVIGATE"/>
      <func name="TRACER.CLEAR"/>
      <func name="TRACER.ERROR"/>
      <func name="PIVOT.FIELD.GROUP"/>
      <func name="PIVOT.FIELD.UNGROUP"/>
      <func name="CHECKBOX.PROPERTIES"/>
      <func name="LABEL.PROPERTIES"/>
      <func name="LISTBOX.PROPERTIES"/>
      <func name="EDITBOX.PROPERTIES"/>
      <func name="PIVOT.REFRESH"/>
      <func name="LINK.COMBO"/>
      <func name="OPEN.TEXT"/>
      <func name="HIDE.DIALOG"/>
      <func name="SET.DIALOG.FOCUS"/>
      <func name="ENABLE.OBJECT"/>
      <func name="PUSHBUTTON.PROPERTIES"/>
      <func name="SET.DIALOG.DEFAULT"/>
      <func name="FILTER"/>
      <func name="FILTER.SHOW.ALL"/>
      <func name="CLEAR.OUTLINE"/>
      <func name="FUNCTION.WIZARD"/>
      <func name="ADD.LIST.ITEM"/>
      <func name="SET.LIST.ITEM"/>
      <func name="REMOVE.LIST.ITEM"/>
      <func name="SELECT.LIST.ITEM"/>
      <func name="SET.CONTROL.VALUE"/>
      <func name="SAVE.COPY.AS"/>
      <func name="OPTIONS.LISTS.ADD"/>
      <func name="OPTIONS.LISTS.DELETE"/>
      <func name="SERIES.AXES"/>
      <func name="SERIES.X"/>
      <func name="SERIES.Y"/>
      <func name="ERRORBAR.X"/>
      <func name="ERRORBAR.Y"/>
      <func name="FORMAT.CHART"/>
      <func name="SERIES.ORDER"/>
      <func name="MAIL.LOGOFF"/>
      <func name="CLEAR.ROUTING.SLIP"/>
      <func name="APP.ACTIVATE.MICROSOFT"/>
      <func name="MAIL.EDIT.MAILER"/>
      <func name="ON.SHEET"/>
      <func name="STANDARD.WIDTH"/>
      <func name="SCENARIO.MERGE"/>
      <func name="SUMMARY.INFO"/>
      <func name="FIND.FILE"/>
      <func name="ACTIVE.CELL.FONT"/>
      <func name="ENABLE.TIPWIZARD"/>
      <func name="VBA.MAKE.ADDIN"/>
      <func name="INSERTDATATABLE"/>
      <func name="WORKGROUP.OPTIONS"/>
      <func name="MAIL.SEND.MAILER"/>
      <func name="AUTOCORRECT"/>
      <func name="POST.DOCUMENT"/>
      <func name="PICKLIST"/>
      <func name="VIEW.SHOW"/>
      <func name="VIEW.DEFINE"/>
      <func name="VIEW.DELETE"/>
      <func name="SHEET.BACKGROUND"/>
      <func name="INSERT.MAP.OBJECT"/>
      <func name="OPTIONS.MENONO"/>
      <func name="MSOCHECKS"/>
      <func name="NORMAL"/>
      <func name="LAYOUT"/>
      <func name="RM.PRINT.AREA"/>
      <func name="CLEAR.PRINT.AREA"/>
      <func name="ADD.PRINT.AREA"/>
      <func name="MOVE.BRK"/>
      <func name="HIDECURR.NOTE"/>
      <func name="HIDEALL.NOTES"/>
      <func name="DELETE.NOTE"/>
      <func name="TRAVERSE.NOTES"/>
      <func name="ACTIVATE.NOTES"/>
      <func name="PROTECT.REVISIONS"/>
      <func name="UNPROTECT.REVISIONS"/>
      <func name="OPTIONS.ME"/>
      <func name="WEB.PUBLISH"/>
      <func name="NEWWEBQUERY"/>
      <func name="PIVOT.TABLE.CHART"/>
      <func name="OPTIONS.SAVE"/>
      <func name="OPTIONS.SPELL"/>
      <func name="HIDEALL.INKANNOTS"/>
    </BuiltInFunctions>
  </xsl:variable>


</xsl:stylesheet>