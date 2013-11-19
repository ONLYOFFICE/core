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
  xmlns:b="http://schemas.openxmlformats.org/officeDocument/2006/bibliography"
  xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
  xmlns:oox="urn:oox"
  exclude-result-prefixes="w number wp xlink r b oox">

	<!-- 
  *************************************************************************
  MATCHING TEMPLATES
  *************************************************************************
  -->

	<!--
  Summary: Inserts a declaration for every SET field
  Author: makz (DIaLOGIKa)
  Date: 2.11.2007
  -->
	<xsl:template match="w:instrText" mode="UserFieldDecls">
		<!-- rebuild the field code using a series of instrText, in current run or followings -->
		<xsl:variable name="fieldCode">
			<xsl:call-template name="BuildFieldCode"/>
		</xsl:variable>
		<!-- first field instruction. Should contains field type. If not, switch to next instruction -->
		<xsl:variable name="fieldType">
			<xsl:call-template name="GetFieldTypeFromCode">
				<xsl:with-param name="fieldCode" select="$fieldCode"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:if test="$fieldType = 'SET' or $fieldType = 'set' ">
			<xsl:call-template name="InsertUserVariableDecl">
				<xsl:with-param name="fieldCode" select="$fieldCode"/>
			</xsl:call-template>
		</xsl:if>

	</xsl:template>

	<xsl:template name="left-trim">
		<xsl:param name="s" />
		<xsl:choose>
			<xsl:when test="substring($s, 1, 1) = ''">
				<xsl:value-of select="$s"/>
			</xsl:when>
			<xsl:when test="normalize-space(substring($s, 1, 1)) = ''">
				<xsl:call-template name="left-trim">
					<xsl:with-param name="s" select="substring($s, 2)" />
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$s" />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="right-trim">
		<xsl:param name="s" />
		<xsl:choose>
			<xsl:when test="substring($s, 1, 1) = ''">
				<xsl:value-of select="$s"/>
			</xsl:when>
			<xsl:when test="normalize-space(substring($s, string-length($s))) = ''">
				<xsl:call-template name="right-trim">
					<xsl:with-param name="s" select="substring($s, 1, string-length($s) - 1)" />
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$s" />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="trim">
		<xsl:param name="s" />
		<xsl:call-template name="right-trim">
			<xsl:with-param name="s">
				<xsl:call-template name="left-trim">
					<xsl:with-param name="s" select="$s" />
				</xsl:call-template>
			</xsl:with-param>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match="w:fldChar[@w:fldCharType != 'begin']" />

  <!-- <xsl:template match="ProcessFieldBegin">
       Отлавливаем все начала полей
  -->
  <xsl:template match="w:fldChar[@w:fldCharType = 'begin']">

    <!--<xsl:call-template name="ProcessFieldBegin">
      <xsl:with-param name="FieldLevel" select="0"/>
      <xsl:with-param name="CurNode" select="."/>
    </xsl:call-template>-->

  </xsl:template>

  <!-- <xsl:template name="ProcessFieldBegin">
       Данный типлэйт используется для обработки w:fldChar с уровнем вложнности FieldLevel.
       Вложенность fldChar имеется ввиду, относительно других fldChar
  -->
	<xsl:template name="ProcessFieldBegin">
		<xsl:param name="FieldLevel"/>
    <xsl:param name="CurNode"/>
		<xsl:param name="parentRunNode"/>

		<xsl:variable name="bLevelFlag">
			<xsl:choose>
				<xsl:when test="count($CurNode/preceding::w:fldChar[@w:fldCharType = 'begin']) = (count($CurNode/preceding::w:fldChar[@w:fldCharType = 'end']) + $FieldLevel)">
					<xsl:text>true</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>false</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

    <!-- Для начала проверим, есть ли у данного поля часть 'separate', и поле instrText до 'separate' -->
		<xsl:variable name="bSeparate">
      <xsl:choose>
        <xsl:when test="$bLevelFlag = 'true'">
          <xsl:call-template name="CheckFldSeparate">
            <xsl:with-param name="CurNode" select="$CurNode" />
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>false</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
		</xsl:variable>

		<xsl:if test="($bSeparate = 'true') and ($bLevelFlag = 'true')">
      
			<!-- rebuild the field code using a series of instrText, in current run or followings -->
			<xsl:variable name="fieldCode">
				<xsl:call-template name="BuildFieldCode">
					<xsl:with-param name="instrText" select="$CurNode/following::w:instrText[1]"/>
				</xsl:call-template>
			</xsl:variable>

			<!-- first field instruction. Should contains field type. If not, switch to next instruction -->
			<xsl:variable name="fieldType">
				<xsl:variable name="field">
					<xsl:call-template name="GetFieldTypeFromCode">
						<xsl:with-param name="fieldCode" select="$fieldCode"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:value-of select="translate($field, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ')"/>
			</xsl:variable>

      <!-- TO DO: 
                  1. Работу всех полей нужно сделать через тимплэйт ProcessFieldSeparate, 
                     по примеру TOC или HYPERLINK 
                  2. Здесь мы предполагаем, что между командами 'begin' и 'separate' нет
                     вложенных полей и, соотственно, команд 'begin', 'separate', 'end'. Но
                     не смотря на то, что таких файлов еще не встречалось, неплохо бы сделать
                     "заглушку" и на такой случай (нужно просто найти ноду с необходимым 
                     'separate' и послать ее в тимплэйт ProcessFieldSeparate вместо команды
                     "$CurNode/following::w:fldChar[@w:fldCharType = 'separate'][1]").
      -->
      
			<xsl:choose>
        <!-- TOC, HYPERLINK, PAGEREF -->
        <xsl:when test="$fieldType='TOC' or $fieldType='HYPERLINK' or $fieldType='PAGEREF'">

          <xsl:call-template name="ProcessFieldSeparate">
            <xsl:with-param name="FieldType" select="$fieldType"/>
            <xsl:with-param name="FieldCode" select="$fieldCode"/>
            <xsl:with-param name="CurNode" select="$CurNode/following::w:fldChar[@w:fldCharType = 'separate'][1]" />
            <xsl:with-param name="FieldLevel" select="$FieldLevel" />
          </xsl:call-template>

        </xsl:when>
				<!-- XE -->
				<xsl:when test="$fieldType='XE'">
					<xsl:call-template name="InsertIndexMark">
						<xsl:with-param name="instrText" select="$fieldCode"/>
					</xsl:call-template>
				</xsl:when>
				<!-- TITLE -->
				<xsl:when test="$fieldType='TITLE'">
					<xsl:call-template name="InsertTitle"/>
				</xsl:when>
				<!-- REF -->
				<xsl:when test="$fieldType='REF'">
					<!--xsl:call-template name="InsertUserVariable">
            <xsl:with-param name="fieldCode" select="$fieldCode"/>
          </xsl:call-template-->
					<!-- divo: quick fix, code is duplicated from template match="w:fldSimple[contains(@w:instr,'REF')]" below -->
					<xsl:variable name="fieldName">
						<xsl:call-template name="ExtractFieldName">
							<xsl:with-param name="fieldCode" select="." />
							<xsl:with-param name="fieldType" select="'REF'" />
						</xsl:call-template>
					</xsl:variable>
					<xsl:variable name="referencedItem" select="key('bookmarksByName', $fieldName)" />

					<xsl:choose>
						<!-- Is the referenced field a bookmark? -->
						<xsl:when test="$referencedItem">
							<xsl:call-template name="InsertCrossReference">
								<xsl:with-param name="fieldCode" select="."/>
							</xsl:call-template>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="InsertUserVariable">
								<xsl:with-param name="fieldCode" select="."/>
							</xsl:call-template>
						</xsl:otherwise>
					</xsl:choose>

				</xsl:when>
				<!-- USERINITIALS -->
				<xsl:when test="$fieldType='USERINITIALS'">
					<xsl:call-template name="InsertUserInitials"/>
				</xsl:when>
				<!-- USERNAME -->
				<xsl:when test="$fieldType='USERNAME'">
					<xsl:call-template name="InsertUserName"/>
				</xsl:when>
				<!-- USERADDRESS -->
				<xsl:when test="$fieldType='USERADDRESS'">
					<xsl:call-template name="InsertUserAddress"/>
				</xsl:when>
				<!-- USERADDRESS -->
				<xsl:when test="$fieldType='SET'">
					<!-- do nothing -->
				</xsl:when>
				<!-- CITATION -->
				<xsl:when test="$fieldType='CITATION'">
					<xsl:call-template name="InsertTextBibliographyMark">
						<xsl:with-param name="TextIdentifier">
							<xsl:value-of select="substring-before(substring-after(self::node(), 'CITATION '), ' \')"/>
						</xsl:with-param>
					</xsl:call-template>
				</xsl:when>
				<!-- handling text after AUTOTEXT field -->
				<xsl:when test="contains($fieldCode,'STYLEREF') and preceding::w:instrText[1][contains(.,'AUTOTEXT')]">
					<xsl:value-of select="following::w:r[w:t][1]"/>
				</xsl:when>
				<!--  possible date types: DATE, PRINTDATE, SAVEDATE, CREATEDATE, INFO CreateDate, INFO PrintDate, INFO Savedate-->
				<xsl:when test="contains($fieldCode, 'DATE' ) or contains($fieldCode,  'date') 
          or contains($fieldCode,'LastSavedTime') or contains($fieldCode,'CreateDate')
          or contains($fieldCode, 'PrintDate') or contains($fieldCode,'SaveDate') or contains($fieldCode,'CreateTime')">
					<xsl:call-template name="InsertDateType">
						<xsl:with-param name="fieldCode" select="$fieldCode"/>
						<xsl:with-param name="fieldType" select="$fieldType"/>
						<xsl:with-param name="fieldText" select="string(../following-sibling::w:r/w:t)" />
					</xsl:call-template>
				</xsl:when>
				<!--page-count NUMPAGE, DOCPROPERTY Pages-->
				<xsl:when test="($fieldType='NUMPAGE' or  contains(.,'NUMPAGES') or $fieldType='numpage' or contains($fieldCode,'Pages')) and not(contains(.,'PAGE/NUMPAGES'))  and not(contains(.,'PAGE/NumPages'))">
					<xsl:call-template name="InsertPageCount"/>
				</xsl:when>
				<xsl:when test="$fieldType='PAGE' and not(contains(.,'PAGE/NUMPAGES')) and not(contains(.,'PAGE/NumPages'))">
					<xsl:call-template name="InsertPageNumber"/>
				</xsl:when>
				<!-- possible time types: TIME, EDITTIME, DOCPROPERTY CreateTime, DOCPROPERTY TotalEditingTime,  INFO EditTime-->
				<xsl:when test="$fieldType='TIME' or contains($fieldCode,'TotalEditingTime') or contains($fieldCode, 'EditTime')">
					<xsl:call-template name="InsertTimeType">
						<xsl:with-param name="fieldCode" select="$fieldCode"/>
						<xsl:with-param name="fieldType" select="$fieldType"/>
					</xsl:call-template>
				</xsl:when>
				<!--initiial creator name AUTHOR and DOCPROPERTY Author-->
				<xsl:when test="$fieldType='AUTHOR'">
					<xsl:call-template name="InsertAuthor"/>
				</xsl:when>
				<!--caption field  from which Index of Figures is created -->
				<xsl:when test="$fieldType='SEQ'">
					<xsl:call-template name="InsertSequence">
						<xsl:with-param name="fieldCode" select="$fieldCode"/>
						<xsl:with-param name="sequenceContext" select="following::w:r[w:t][1]"/>
					</xsl:call-template>
				</xsl:when>
				<!--creator name LASTSAVEDBY, DOCPROPERTY LastSavedBy-->
				<xsl:when test="$fieldType='LASTSAVEDBY' or  contains($fieldCode,'LastSavedBy')">
					<xsl:call-template name="InsertCreator"/>
				</xsl:when>
				<!--editing cycles number REVNUM, DOCPROPERTY RevisionNumber, INFO RevNum-->
				<xsl:when test="$fieldType='REVNUM' or contains($fieldCode,'RevisionNumber') or contains($fieldCode, 'RevNum')">
					<xsl:call-template name="InsertEditingCycles"/>
				</xsl:when>
				<!--FILENAME, INFO FileName-->
				<xsl:when test="$fieldType='FILENAME' or contains($fieldCode,'FileName')">
					<xsl:call-template name="InsertFileName"/>
				</xsl:when>
				<!-- KEYWORDS, DOCPROPERTY Keywords -->
				<xsl:when test="contains($fieldCode,'KEYWORDS') or contains($fieldCode,'keywords') or contains($fieldCode,'Keywords')">
					<xsl:call-template name="InsertKeywords"/>
				</xsl:when>
				<!-- DOCPROPERTY Company, INFO Company-->
				<xsl:when test="contains($fieldCode,'Company') or contains($fieldCode, 'company')">
					<xsl:call-template name="InsertCompany"/>
				</xsl:when>
				<!--TEMPLATE, DOCPROPERTY Template-->
				<xsl:when test="$fieldType='TEMPLATE' or contains($fieldCode,'Template')">
					<xsl:call-template name="InsertTemplate"/>
				</xsl:when>
				<!--NUMWORDS, DOCPROPERTY Words-->
				<xsl:when test="$fieldType='NUMWORDS' or contains($fieldCode,'Words')">
					<xsl:call-template name="InsertWordCount"/>
				</xsl:when>
				<!--NUMCHARS and DOCPROPERTY Characters,  INFO NumChars-->
				<xsl:when test="$fieldType='NUMCHARS' or contains($fieldCode,'Characters') or contains($fieldCode, 'NumChars')">
					<xsl:call-template name="InsertCharacterCount"/>
				</xsl:when>
				<!-- DOCPROPERTY Paragraphs, INFO Paragraphs-->
				<xsl:when test="contains($fieldCode,'Paragraphs') or contains($fieldCode,'paragraphs') ">
					<xsl:call-template name="InsertCompany"/>
				</xsl:when>
				<!-- COMMENTS, DOCPROPERTY Comments-->
				<xsl:when test="contains($fieldCode,'COMMENTS') or contains($fieldCode,'Comments') ">
					<xsl:call-template name="InsertComments"/>
				</xsl:when>
				<!--document subject SUBJECT, DOCPROPERTY Subject-->
				<xsl:when test="contains($fieldCode,'SUBJECT') or contains($fieldCode,'Subject') or contains($fieldCode,'subject')">
					<xsl:call-template name="InsertSubject"/>
				</xsl:when>
				<xsl:when test="contains($fieldCode, 'DOCPROPERTY')">
					<!--bugfix: 1759893-->
					<xsl:if test="../following-sibling::w:r/w:fldChar[@w:fldCharType = 'separate']">
						<xsl:choose>
							<xsl:when test="../following-sibling::w:r/w:t">
								<xsl:value-of select="../following-sibling::w:r/w:t"></xsl:value-of>
							</xsl:when>
						</xsl:choose>
					</xsl:if>
				</xsl:when>
				<xsl:when test="$fieldType = 'MACROBUTTON'">
					<xsl:variable name="WithoutMACROBUTTON">
						<xsl:call-template name="left-trim">
							<xsl:with-param name="s" select="substring-after($fieldCode,'MACROBUTTON ')" />
						</xsl:call-template>
						<!--<xsl:value-of select="substring-after($fieldCode,'MACROBUTTON ')"/>-->
					</xsl:variable>
					<xsl:variable name="WithoutFirstArgument">
						<xsl:value-of select="substring-after($WithoutMACROBUTTON,' ')"/>
					</xsl:variable>
					<xsl:value-of select="$WithoutFirstArgument"/>
				</xsl:when>
			</xsl:choose>
		</xsl:if>
	</xsl:template>

  <xsl:template name="FindFldBegin">
    <xsl:param name="FldLvl" />
    <xsl:param name="List"/>
    <xsl:param name="Num" />

    <xsl:choose>
      <xsl:when test="count($List) = 0 or $Num = 0">
        <xsl:value-of select="-1" />
      </xsl:when>
      <xsl:when test="count($List[$Num]/preceding::w:fldChar[@w:fldCharType = 'begin']) = (count($List[$Num]/preceding::w:fldChar[@w:fldCharType = 'end']) + $FldLvl - 1 )">

        <xsl:choose>

          <!-- Проверяем это текст или инструкция для данного поля -->
          <xsl:when test="$Num != count($List) and $List[$Num + 1]/@w:fldCharType = 'separate'">
            <xsl:value-of select="$Num" />
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="-1" />
          </xsl:otherwise>

        </xsl:choose>
      </xsl:when>
      <xsl:otherwise>

        <xsl:call-template name="FindFldBegin" >
          <xsl:with-param name="FldLvl" select="$FldLvl" />
          <xsl:with-param name="List" select="$List" />
          <xsl:with-param name="Num" select="$Num - 1" />
        </xsl:call-template>

      </xsl:otherwise>
    </xsl:choose>
    
  </xsl:template>
  
  <xsl:template name="ProcessField">
    <xsl:param name="FieldLevel" />
    <xsl:param name="CurNode"/>
    <xsl:param name="TextNode" />


      <!-- rebuild the field code using a series of instrText, in current run or followings -->
      <xsl:variable name="fieldCode">
        <xsl:call-template name="BuildFieldCode">
          <xsl:with-param name="instrText" select="$CurNode/following::w:instrText[1]"/>
        </xsl:call-template>
      </xsl:variable>

      <!-- first field instruction. Should contains field type. If not, switch to next instruction -->
      <xsl:variable name="fieldType">
        <xsl:variable name="field">
          <xsl:call-template name="GetFieldTypeFromCode">
            <xsl:with-param name="fieldCode" select="$fieldCode"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:value-of select="translate($field, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ')"/>
      </xsl:variable>

      <xsl:choose>

        <!-- TOC -->
        <xsl:when test="$fieldType = 'TOC'">

          <xsl:call-template name="DrawText">
            <xsl:with-param name="CurNode" select="$TextNode" />
          </xsl:call-template>

        </xsl:when>

        <!-- HYPERLINK -->
        <xsl:when test="$fieldType = 'HYPERLINK'">

          <xsl:call-template name="InsertHyperlinkField">
            <xsl:with-param name="strCommand" select="$fieldCode"/>
            <xsl:with-param name="CurNode" select="$CurNode" />
            <xsl:with-param name="Text" select="$TextNode/text()"/>
          </xsl:call-template>

        </xsl:when>

        <!-- PAGEREF -->
        <xsl:when test="$fieldType = 'PAGEREF'">

          <xsl:call-template name="InsertPageRefField">
            <xsl:with-param name="Text" select="$TextNode/text()"/>
            <xsl:with-param name="strCommand" select="$fieldCode"/>
            <xsl:with-param name="CurNode" select="$CurNode" />
          </xsl:call-template>

        </xsl:when>

        <!-- XE -->
        <xsl:when test="$fieldType='XE'">
          <xsl:call-template name="InsertIndexMark">
            <xsl:with-param name="instrText" select="$fieldCode"/>
          </xsl:call-template>
        </xsl:when>
        <!-- TITLE -->
        <xsl:when test="$fieldType='TITLE'">
          <xsl:call-template name="DrawText">
            <xsl:with-param name="CurNode" select="$TextNode" />
          </xsl:call-template>
          <!--<xsl:call-template name="InsertTitle"/>-->
        </xsl:when>
        <!-- REF -->
        <xsl:when test="$fieldType='REF'">
          <!--xsl:call-template name="InsertUserVariable">
            <xsl:with-param name="fieldCode" select="$fieldCode"/>
          </xsl:call-template-->
          <!-- divo: quick fix, code is duplicated from template match="w:fldSimple[contains(@w:instr,'REF')]" below -->
          <xsl:variable name="fieldName">
            <xsl:call-template name="ExtractFieldName">
              <xsl:with-param name="fieldCode" select="." />
              <xsl:with-param name="fieldType" select="'REF'" />
            </xsl:call-template>
          </xsl:variable>
          <xsl:variable name="referencedItem" select="key('bookmarksByName', $fieldName)" />

          <xsl:choose>
            <!-- Is the referenced field a bookmark? -->
            <xsl:when test="$referencedItem">
              <xsl:call-template name="InsertCrossReference">
                <xsl:with-param name="fieldCode" select="."/>
              </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
              <xsl:call-template name="InsertUserVariable">
                <xsl:with-param name="fieldCode" select="."/>
              </xsl:call-template>
            </xsl:otherwise>
          </xsl:choose>

        </xsl:when>
        <!-- USERINITIALS -->
        <xsl:when test="$fieldType='USERINITIALS'">
          <xsl:call-template name="InsertUserInitials"/>
        </xsl:when>
        <!-- USERNAME -->
        <xsl:when test="$fieldType='USERNAME'">
          <xsl:call-template name="InsertUserName"/>
        </xsl:when>
        <!-- USERADDRESS -->
        <xsl:when test="$fieldType='USERADDRESS'">
          <xsl:call-template name="InsertUserAddress"/>
        </xsl:when>
        <!-- USERADDRESS -->
        <xsl:when test="$fieldType='SET'">
          <!-- do nothing -->
        </xsl:when>
        <!-- CITATION -->
        <xsl:when test="$fieldType='CITATION'">
          <xsl:call-template name="InsertTextBibliographyMark">
            <xsl:with-param name="TextIdentifier">
              <xsl:value-of select="substring-before(substring-after(self::node(), 'CITATION '), ' \')"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:when>
        <!-- handling text after AUTOTEXT field -->
        <xsl:when test="contains($fieldCode,'STYLEREF') and preceding::w:instrText[1][contains(.,'AUTOTEXT')]">
          <xsl:value-of select="following::w:r[w:t][1]"/>
        </xsl:when>
        <!--  possible date types: DATE, PRINTDATE, SAVEDATE, CREATEDATE, INFO CreateDate, INFO PrintDate, INFO Savedate-->
        <xsl:when test="contains($fieldCode, 'DATE' ) or contains($fieldCode,  'date') 
          or contains($fieldCode,'LastSavedTime') or contains($fieldCode,'CreateDate')
          or contains($fieldCode, 'PrintDate') or contains($fieldCode,'SaveDate') or contains($fieldCode,'CreateTime')">
          <xsl:call-template name="InsertDateType">
            <xsl:with-param name="fieldCode" select="$fieldCode"/>
            <xsl:with-param name="fieldType" select="$fieldType"/>
            <xsl:with-param name="fieldText" select="string(../following-sibling::w:r/w:t)" />
          </xsl:call-template>
        </xsl:when>
        <!--page-count NUMPAGE, DOCPROPERTY Pages-->
        <xsl:when test="($fieldType='NUMPAGE' or  contains(.,'NUMPAGES') or $fieldType='numpage' or contains($fieldCode,'Pages')) and not(contains(.,'PAGE/NUMPAGES'))  and not(contains(.,'PAGE/NumPages'))">
          <xsl:call-template name="InsertPageCount"/>
        </xsl:when>
        <xsl:when test="$fieldType='PAGE' and not(contains(.,'PAGE/NUMPAGES')) and not(contains(.,'PAGE/NumPages'))">
          <xsl:call-template name="InsertPageNumber"/>
        </xsl:when>
        <!-- possible time types: TIME, EDITTIME, DOCPROPERTY CreateTime, DOCPROPERTY TotalEditingTime,  INFO EditTime-->
        <xsl:when test="$fieldType='TIME' or contains($fieldCode,'TotalEditingTime') or contains($fieldCode, 'EditTime')">
          <xsl:call-template name="InsertTimeType">
            <xsl:with-param name="fieldCode" select="$fieldCode"/>
            <xsl:with-param name="fieldType" select="$fieldType"/>
          </xsl:call-template>
        </xsl:when>
        <!--initiial creator name AUTHOR and DOCPROPERTY Author-->
        <xsl:when test="$fieldType='AUTHOR'">
          <xsl:call-template name="InsertAuthor"/>
        </xsl:when>
        <!--caption field  from which Index of Figures is created -->
        <xsl:when test="$fieldType='SEQ'">
          <xsl:call-template name="InsertSequence">
            <xsl:with-param name="fieldCode" select="$fieldCode"/>
            <xsl:with-param name="sequenceContext" select="following::w:r[w:t][1]"/>
          </xsl:call-template>
        </xsl:when>
        <!--creator name LASTSAVEDBY, DOCPROPERTY LastSavedBy-->
        <xsl:when test="$fieldType='LASTSAVEDBY' or  contains($fieldCode,'LastSavedBy')">
          <xsl:call-template name="InsertCreator"/>
        </xsl:when>
        <!--editing cycles number REVNUM, DOCPROPERTY RevisionNumber, INFO RevNum-->
        <xsl:when test="$fieldType='REVNUM' or contains($fieldCode,'RevisionNumber') or contains($fieldCode, 'RevNum')">
          <xsl:call-template name="InsertEditingCycles"/>
        </xsl:when>
        <!--FILENAME, INFO FileName-->
        <xsl:when test="$fieldType='FILENAME' or contains($fieldCode,'FileName')">
          <xsl:call-template name="InsertFileName"/>
        </xsl:when>
        <!-- KEYWORDS, DOCPROPERTY Keywords -->
        <xsl:when test="contains($fieldCode,'KEYWORDS') or contains($fieldCode,'keywords') or contains($fieldCode,'Keywords')">
          <xsl:call-template name="InsertKeywords"/>
        </xsl:when>
        <!-- DOCPROPERTY Company, INFO Company-->
        <xsl:when test="contains($fieldCode,'Company') or contains($fieldCode, 'company')">
          <xsl:call-template name="InsertCompany"/>
        </xsl:when>
        <!--TEMPLATE, DOCPROPERTY Template-->
        <xsl:when test="$fieldType='TEMPLATE' or contains($fieldCode,'Template')">
          <xsl:call-template name="InsertTemplate"/>
        </xsl:when>
        <!--NUMWORDS, DOCPROPERTY Words-->
        <xsl:when test="$fieldType='NUMWORDS' or contains($fieldCode,'Words')">
          <xsl:call-template name="InsertWordCount"/>
        </xsl:when>
        <!--NUMCHARS and DOCPROPERTY Characters,  INFO NumChars-->
        <xsl:when test="$fieldType='NUMCHARS' or contains($fieldCode,'Characters') or contains($fieldCode, 'NumChars')">
          <xsl:call-template name="InsertCharacterCount"/>
        </xsl:when>
        <!-- DOCPROPERTY Paragraphs, INFO Paragraphs-->
        <xsl:when test="contains($fieldCode,'Paragraphs') or contains($fieldCode,'paragraphs') ">
          <xsl:call-template name="InsertCompany"/>
        </xsl:when>
        <!-- COMMENTS, DOCPROPERTY Comments-->
        <xsl:when test="contains($fieldCode,'COMMENTS') or contains($fieldCode,'Comments') ">
          <xsl:call-template name="InsertComments"/>
        </xsl:when>
        <!--document subject SUBJECT, DOCPROPERTY Subject-->
        <xsl:when test="contains($fieldCode,'SUBJECT') or contains($fieldCode,'Subject') or contains($fieldCode,'subject')">
          <xsl:call-template name="InsertSubject"/>
        </xsl:when>
        <xsl:when test="contains($fieldCode, 'DOCPROPERTY')">
          <!--bugfix: 1759893-->
          <xsl:if test="../following-sibling::w:r/w:fldChar[@w:fldCharType = 'separate']">
            <xsl:choose>
              <xsl:when test="../following-sibling::w:r/w:t">
                <xsl:value-of select="../following-sibling::w:r/w:t"></xsl:value-of>
              </xsl:when>
            </xsl:choose>
          </xsl:if>
        </xsl:when>
        <xsl:when test="$fieldType = 'MACROBUTTON'">
          <xsl:variable name="WithoutMACROBUTTON">
            <xsl:call-template name="left-trim">
              <xsl:with-param name="s" select="substring-after($fieldCode,'MACROBUTTON ')" />
            </xsl:call-template>
            <!--<xsl:value-of select="substring-after($fieldCode,'MACROBUTTON ')"/>-->
          </xsl:variable>
          <xsl:variable name="WithoutFirstArgument">
            <xsl:value-of select="substring-after($WithoutMACROBUTTON,' ')"/>
          </xsl:variable>
          <xsl:value-of select="$WithoutFirstArgument"/>
        </xsl:when>
      </xsl:choose>
  </xsl:template>

  <xsl:template match="w:instrText" mode="automaticstyles">
		<!-- rebuild the field code using a series of instrText, in current run or followings -->
		<xsl:variable name="fieldCode">
			<xsl:call-template name="BuildFieldCode"/>
		</xsl:variable>
		<!-- first field instruction. Should contains field type. If not, switch to next instruction -->
		<xsl:variable name="fieldType">
			<xsl:call-template name="GetFieldTypeFromCode">
				<xsl:with-param name="fieldCode" select="$fieldCode"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:choose>
			<!--  possible date types: DATE, PRINTDATE, SAVEDATE, CREATEDATE-->
			<xsl:when test="contains($fieldType, 'DATE')">
				<xsl:call-template name="InsertDateStyle">
					<xsl:with-param name="fieldCode" select="$fieldCode" />
					<xsl:with-param name="dateText" select="string(../following-sibling::w:r/w:t)"/>
				</xsl:call-template>
			</xsl:when>
			<!-- possible time types: TIME, EDITTIME-->
			<xsl:when test="contains($fieldType, 'TIME')">
				<xsl:call-template name="InsertTimeStyle">
					<xsl:with-param name="timeText" select="$fieldCode"/>
				</xsl:call-template>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<!-- template with mode automaticpict should behave as with mode automaticstyles -->
	<xsl:template match="w:instrText" mode="automaticpict">
		<xsl:apply-templates select="." mode="automaticstyles"/>
	</xsl:template>

	<!--
  Summary: Converts references to fields or bookmarks
  Author: Clever Age
  Modified: makz (DIaLOGIka)
  Date: 2.11.2007
  -->
	<xsl:template match="w:fldSimple[contains(@w:instr,'REF')]" mode="fields">
		<!-- TODO code is duplicated in template match="w:instrText" above, create a new template and call it here -->
		<xsl:variable name="fieldName">
			<xsl:call-template name="ExtractFieldName">
				<xsl:with-param name="fieldCode" select="@w:instr" />
				<xsl:with-param name="fieldType" select="'REF'" />
			</xsl:call-template>
		</xsl:variable>
		<xsl:variable name="referencedItem" select="key('bookmarksByName', $fieldName)" />

		<xsl:choose>
			<!-- Is the referenced field a bookmark? -->
			<xsl:when test="$referencedItem">
				<xsl:call-template name="InsertCrossReference">
					<xsl:with-param name="fieldCode" select="@w:instr"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:call-template name="InsertUserVariable">
					<xsl:with-param name="fieldCode" select="@w:instr"/>
				</xsl:call-template>
			</xsl:otherwise>
		</xsl:choose>

	</xsl:template>

	<!--document title TITLE, DOCPROPERTY Title-->
	<xsl:template match="w:fldSimple[contains(@w:instr,'TITLE') or contains(@w:instr, 'Title')]" mode="fields">
		<xsl:call-template name="InsertTitle"/>
	</xsl:template>

	<xsl:template match="w:fldSimple[contains(@w:instr,'USERNAME')]" mode="fields">
		<xsl:call-template name="InsertUserName"/>
	</xsl:template>

	<!--AUTHOR and DOCPROPERTY Author-->
	<xsl:template match="w:fldSimple[contains(@w:instr,'AUTHOR') or contains(@w:instr,' Author')]" mode="fields">
		<xsl:call-template name="InsertAuthor"/>
	</xsl:template>

	<!--creator-name LASTSAVEDBY, DOCPROPERTY LastSavedBy-->
	<xsl:template match="w:fldSimple[contains(@w:instr,'LASTSAVEDBY') or contains(@w:instr,'LastSavedBy')]" mode="fields">
		<xsl:call-template name="InsertCreator"/>
	</xsl:template>

	<!--user initials-->
	<xsl:template match="w:fldSimple[contains(@w:instr,'USERINITIALS')]" mode="fields">
		<xsl:call-template name="InsertUserInitials"/>
	</xsl:template>

	<!--chapter name or chapter number-->
	<xsl:template match="w:fldSimple[contains(@w:instr,'STYLEREF')]" mode="fields">
		<!--text:chapter>
      <xsl:choose>
        <xsl:when test="self::node()[contains(@w:instr,'\n')]">
          <xsl:attribute name="text:display">number</xsl:attribute>
        </xsl:when>
        <xsl:when test="self::node()[contains(@w:instr,'\*')]">
          <xsl:attribute name="text:display">name</xsl:attribute>
        </xsl:when>
        <xsl:when test="self::node()[contains(@w:instr,'\s')]">
          <xsl:attribute name="text:display">plain-number</xsl:attribute>
        </xsl:when>
      </xsl:choose>
      <xsl:if test="self::node()[contains(@w:instr,'Heading')]">
        <xsl:attribute name="text:outline-level">
          <xsl:value-of
            select="substring(substring-before(@w:instr,' \'),string-length(substring-before(@w:instr,' \')))"/>
        </xsl:attribute>
      </xsl:if>
      <xsl:apply-templates select="w:r/child::node()"/>
    </text:chapter-->
		<xsl:message terminate="no">translation.oox2htm.fields.styleref</xsl:message>
		<xsl:apply-templates select="w:r/child::node()"/>
	</xsl:template>

	<!--document subject SUBJECT, DOCPROPERTY Subject-->
	<xsl:template match="w:fldSimple[contains(@w:instr,'SUBJECT') or contains(@w:instr, 'Subject')]" mode="fields">
		<xsl:call-template name="InsertSubject"/>
	</xsl:template>

	<xsl:template match="w:fldSimple">
		<!--
        <text:span text:style-name="{generate-id(w:r)}">
            <xsl:choose>
                <!-@- handling text after AUTOTEXT field -@->
                <xsl:when test="contains(@w:instr,'STYLEREF') and preceding::w:instrText[1][contains(.,'AUTOTEXT')]">
                    <xsl:value-of  select="descendant::w:t"/>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:apply-templates select="." mode="fields"/>
                </xsl:otherwise>
            </xsl:choose>
        </text:span>-->
	</xsl:template>

	<!--  possible date types: DATE, PRINTDATE, SAVEDATE, CREATEDATE, 
    DOPCPROPERTY LastSavedTime, DOCPROPERTY CreateTime, INFO CreateDate, INFO PrintDate, INFO Savedate-->
	<xsl:template match="w:fldSimple[contains(@w:instr,'DATE') or contains(@w:instr,'LastSavedTime') or contains(@w:instr, 'CreateTime') 
                or contains(@w:instr,'CreateDate') or contains(@w:instr, 'PrintDate') or contains(@w:instr, 'SaveDate')] "
                  mode="fields">
		<xsl:variable name="fieldType">
			<xsl:call-template name="GetFieldTypeFromCode">
				<xsl:with-param name="fieldCode" select="@w:instr"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:call-template name="InsertDateType">
			<xsl:with-param name="fieldCode" select="@w:instr"/>
			<xsl:with-param name="fieldType" select="$fieldType"/>
			<xsl:with-param name="fieldText" select="string(w:r/w:t)" />
		</xsl:call-template>
	</xsl:template>

	<!-- possible time types: TIME, EDITTIME, DOCPROPERTY TotalEditingTime, INFO EditTime-->
	<xsl:template match="w:fldSimple[contains(@w:instr,'TIME') or contains(@w:instr, 'TotalEditingTime') or 
                contains(@w:instr, 'EditTime')]" mode="fields">
		<xsl:variable name="fieldType">
			<xsl:call-template name="GetFieldTypeFromCode">
				<xsl:with-param name="fieldCode" select="@w:instr"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:call-template name="InsertTimeType">
			<xsl:with-param name="fieldCode" select="@w:instr"/>
			<xsl:with-param name="fieldType" select="$fieldType"/>
		</xsl:call-template>
	</xsl:template>

	<!--page number-->
	<xsl:template match="w:fldSimple[(contains(@w:instr,'PAGE') or contains(@w:instr,'page')) 
                and not(contains(@w:instr,'NUMPAGE')) and not(contains(@w:instr,'PAGEREF'))]" mode="fields">
		<xsl:call-template name="InsertPageNumber">
			<xsl:with-param name="fieldCode" select="@w:instr"/>
		</xsl:call-template>
	</xsl:template>

	<!--page-count NUMPAGE, DOCPROPERTY Pages-->
	<xsl:template match="w:fldSimple[contains(@w:instr,'NUMPAGE') or contains(@w:instr,'Pages')]" mode="fields">
		<xsl:call-template name="InsertPageCount">
			<xsl:with-param name="fieldCode" select="@w:instr"/>
		</xsl:call-template>
	</xsl:template>

	<!--editing cycles number REVNUM, DOCPROPERTY RevisionNumber, INFO RevNum-->
	<xsl:template match="w:fldSimple[contains(@w:instr,'REVNUM') or contains(@w:instr, 'RevisionNumber') or contains(@w:instr, 'RevNum')]" mode="fields">
		<xsl:call-template name="InsertEditingCycles"/>
	</xsl:template>

	<!--FILENAME, INFO FileName-->
	<xsl:template match="w:fldSimple[contains(@w:instr,'FILENAME') or contains(@w:instr, 'FileName')]" mode="fields">
		<xsl:call-template name="InsertFileName"/>
	</xsl:template>

	<xsl:template match="w:fldSimple[contains(@w:instr,'DATE') or contains(@w:instr,'LastSavedTime') or contains(@w:instr, 'CreateTime')
    or contains(@w:instr,'CreateDate') or contains(@w:instr, 'PrintDate') or contains(@w:instr, 'SaveDate')]" mode="automaticstyles">
		<xsl:call-template name="InsertDateStyle">
			<xsl:with-param name="fieldCode" select="@w:instr"/>
			<xsl:with-param name="dateText" select="string(w:r/w:t)"/>
		</xsl:call-template>
	</xsl:template>

	<!--caption field  from which Index of Figures is created -->
	<xsl:template match="w:fldSimple[contains(@w:instr,'SEQ')]" mode="fields">
		<xsl:call-template name="InsertSequence">
			<xsl:with-param name="fieldCode" select="@w:instr"/>
			<xsl:with-param name="sequenceContext" select="w:r"/>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match="w:fldSimple[contains(@w:instr,'TIME') or contains(@w:instr, 'TotalEditingTime') or contains(@w:instr, 'EditTime')]" mode="automaticstyles">
		<xsl:call-template name="InsertTimeStyle">
			<xsl:with-param name="timeText" select="@w:instr"/>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match="w:fldSimple" mode="automaticstyles">
		<xsl:apply-templates select="w:r/w:rPr" mode="automaticstyles"/>
	</xsl:template>

	<xsl:template match="w:sdt/w:sdtContent/w:fldSimple">
		<xsl:apply-templates/>
	</xsl:template>

	<!-- DOCPROPERTY Paragraphs, INFO Paragraphs-->
	<xsl:template match="w:fldSimple[contains(@w:instr,'Company')]" mode="fields">
		<xsl:call-template name="InsertCompany"/>
	</xsl:template>

	<xsl:template match="w:fldSimple[contains(@w:instr,'USERADDRESS')]" mode="fields">
		<xsl:call-template name="InsertUserAddress"/>
	</xsl:template>

	<!--TEMPLATE, DOCPROPERTY Template-->
	<xsl:template match="w:fldSimple[contains(@w:instr,'TEMPLATE')or contains(@w:instr, 'Template')]" mode="fields">
		<xsl:call-template name="InsertTemplate"/>
	</xsl:template>

	<!--KEYWORDS and DOCPROPERTY Keywords-->
	<xsl:template match="w:fldSimple[contains(@w:instr,'KEYWORDS') or contains(@w:instr,'Keywords')]" mode="fields">
		<xsl:call-template name="InsertKeywords"/>
	</xsl:template>

	<!--NUMCHARS and DOCPROPERTY Characters, INFO NumChars-->
	<xsl:template match="w:fldSimple[contains(@w:instr,'NUMCHARS') or contains(@w:instr, 'Characters') or contains(@w:instr, 'NumChars')]" mode="fields">
		<xsl:call-template name="InsertCharacterCount"/>
	</xsl:template>

	<!-- DOCPROPERTY Paragraphs, INFO Paragraphs-->
	<xsl:template match="w:fldSimple[contains(@w:instr,'Paragraphs')]" mode="fields">
		<xsl:call-template name="InsertParagraphCount"/>
	</xsl:template>

	<!-- COMMENTS, DOCPROPERTY Comments-->
	<xsl:template match="w:fldSimple[contains(@w:instr,'COMMENTS') or contains(@w:instr, 'Comments')]" mode="fields">
		<xsl:call-template name="InsertComments"/>
	</xsl:template>

	<!--NUMWORDS, DOCPROPERTY Words-->
	<xsl:template match="w:fldSimple[contains(@w:instr,'NUMWORDS') or contains(@w:instr, 'Words')]" mode="fields">
		<xsl:call-template name="InsertWordCount"/>
	</xsl:template>

	<!-- 
  *************************************************************************
  CALLED TEMPLATES
  *************************************************************************
  -->

	<!-- Date and Time Fields -->
	<xsl:template name="InsertDate">
		<xsl:param name="dateText"/>
		<!--
        <text:date>
            <xsl:call-template name="InsertDateContent">
                <xsl:with-param name="dateText" select="$dateText"/>
            </xsl:call-template>
        </text:date>-->
	</xsl:template>

	<xsl:template name="InsertDateType">
		<xsl:param name="fieldCode"/>
		<xsl:param name="fieldType"/>
		<xsl:param name="fieldText"/>

		<xsl:choose>
			<xsl:when test="$fieldType = 'DATE'">
				<xsl:call-template name="InsertDate">
					<xsl:with-param name="dateText" select="$fieldText"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:when
              test="$fieldType = 'CREATEDATE' or contains($fieldCode, 'CreateDate') or contains($fieldCode, 'CreateTime')">
				<xsl:call-template name="InsertCreationDate">
					<xsl:with-param name="dateText" select="$fieldText"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$fieldType = 'PRINTDATE' or contains($fieldCode, 'PrintDate')">
				<xsl:call-template name="InsertPrintDate">
					<xsl:with-param name="dateText" select="$fieldText"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:when
              test="$fieldType = 'SAVEDATE' or contains($fieldCode,'LastSavedTime') or contains($fieldCode, 'SaveDate')">
				<xsl:call-template name="InsertModificationDate">
					<xsl:with-param name="dateText" select="$fieldText"/>
				</xsl:call-template>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="InsertCreationDate">
		<xsl:param name="dateText"/>
		<!--<text:creation-date>
            <xsl:call-template name="InsertDateContent">
                <xsl:with-param name="dateText" select="$dateText"/>
            </xsl:call-template>
        </text:creation-date>-->
	</xsl:template>

	<xsl:template name="InsertPrintDate">
		<xsl:param name="dateText"/>
		<!--<text:print-date>
            <xsl:call-template name="InsertDateContent">
                <xsl:with-param name="dateText" select="$dateText"/>
            </xsl:call-template>
        </text:print-date>-->
	</xsl:template>

	<xsl:template name="InsertModificationDate">
		<xsl:param name="dateText"/>
		<!--<text:modification-date>
            <xsl:call-template name="InsertDateContent">
                <xsl:with-param name="dateText" select="$dateText"/>
            </xsl:call-template>
        </text:modification-date>-->
	</xsl:template>

	<xsl:template name="InsertDateContent">
		<xsl:param name="dateText"/>

		<xsl:attribute name="style:data-style-name">
			<xsl:value-of select="generate-id(.)"/>
		</xsl:attribute>
		<xsl:attribute name="text:date-value">
			<xsl:value-of select="$dateText"/>
		</xsl:attribute>
		<xsl:value-of select="$dateText"/>
	</xsl:template>

	<xsl:template name="InsertTimeType">
		<xsl:param name="fieldCode"/>
		<xsl:param name="fieldType"/>

		<xsl:choose>
			<xsl:when test="$fieldType = 'TIME' ">
				<xsl:call-template name="InsertTime">
					<xsl:with-param name="timeText" select="$fieldCode"/>
				</xsl:call-template>
			</xsl:when>
			<!-- EDITTIME, DOCPROPERTY TotalEditingTime-->
			<xsl:when
              test="$fieldType = 'EDITTIME' or contains($fieldCode,'TotalEditingTime') or contains($fieldCode, 'EditTime')">
				<xsl:call-template name="InsertEditTime">
					<xsl:with-param name="timeText" select="$fieldCode"/>
				</xsl:call-template>
			</xsl:when>
			<!-- DOCPROPERTY CreateTime-->
			<xsl:when test="contains($fieldCode,'CreateTime') ">
				<xsl:call-template name="InsertCreationTime">
					<xsl:with-param name="timeText" select="$fieldCode"/>
				</xsl:call-template>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="InsertCreationTime">
		<xsl:param name="timeText"/>
		<!--<text:creation-time>
            <xsl:attribute name="style:data-style-name">
                <xsl:value-of select="generate-id(.)"/>
            </xsl:attribute>
            <xsl:attribute name="text:time-value">
                <xsl:value-of select="$timeText"/>
            </xsl:attribute>
        </text:creation-time>-->
	</xsl:template>

	<xsl:template name="InsertTime">
		<xsl:param name="timeText"/>
		<!--<text:time>
            <xsl:attribute name="style:data-style-name">
                <xsl:value-of select="generate-id(.)"/>
            </xsl:attribute>
            <xsl:attribute name="text:time-value">
                <xsl:value-of select="$timeText"/>
            </xsl:attribute>
        </text:time>-->
	</xsl:template>

	<xsl:template name="InsertEditTime">
		<xsl:param name="timeText"/>
		<!--<text:editing-duration>
            <xsl:attribute name="style:data-style-name">
                <xsl:value-of select="generate-id(.)"/>
            </xsl:attribute>
        </text:editing-duration>-->
	</xsl:template>

	<!-- build a field code using current instrText and recursively forward (go to next instrText) -->
	<xsl:template name="BuildFieldCode">
		<xsl:param name="instrText" select="."/>
		<xsl:param name="fieldCode" />

		<!-- context must be w:instrText -->
		<xsl:choose>
			<!-- if next sibling instrText -->
			<xsl:when test="$instrText/following-sibling::w:instrText">
				<xsl:call-template name="BuildFieldCode">
					<xsl:with-param name="instrText" select="$instrText/following-sibling::w:instrText[1]"/>
					<xsl:with-param name="fieldCode" select="concat($fieldCode, $instrText/text())"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$instrText/following-sibling::w:fldChar[@w:fldCharType = 'end']">
				<xsl:value-of select="concat($fieldCode, $instrText/text())"/>
			</xsl:when>
			<!-- if next run with instrText, before end of field -->
			<xsl:when test="$instrText/parent::w:r/following-sibling::w:r[1]/*[self::w:instrText or self::w:fldChar[@w:fldCharType = 'end']][1][self::w:instrText]">
				<xsl:call-template name="BuildFieldCode">
					<!-- we know now that first run having instrText is before end of field -->
					<xsl:with-param name="instrText"
                        select="$instrText/parent::w:r/following-sibling::w:r[1]/w:instrText[1]"/>
					<xsl:with-param name="fieldCode" select="concat($fieldCode, $instrText/text())"/>
				</xsl:call-template>
			</xsl:when>
			<!-- if next paragraph with instrText, before end of field :
        Find first paragraph having instrText before end of field, and then first run having instrText before end of field -->
			<xsl:when test="$instrText/ancestor::w:p/following-sibling::w:p/w:r[1][w:instrText or w:fldChar[@w:fldCharType = 'end']][1]/*[self::w:instrText or self::w:fldChar[@w:fldCharType = 'end']][1][self::w:instrText]">
				<!-- check first is field does not end in context paragraph -->
				<xsl:choose>
					<xsl:when
                      test="not($instrText/parent::w:r/following-sibling::w:r[1]/w:fldChar[@w:fldCharType = 'end'])">
						<xsl:call-template name="BuildFieldCode">
							<!-- we know now that first run having instrText is before end of field -->
							<xsl:with-param name="instrText"
                              select="$instrText/ancestor::w:p/following-sibling::w:p[w:r/w:instrText][1]/w:r[w:instrText][1]/w:instrText[1]"/>
							<xsl:with-param name="fieldCode" select="concat($fieldCode, $instrText/text())"/>
						</xsl:call-template>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="concat($fieldCode, $instrText/text())"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
      <xsl:when test="$instrText/ancestor::w:p/following-sibling::w:p/w:r[1][w:instrText or w:fldChar[@w:fldCharType = 'separate']][1]/*[self::w:instrText or self::w:fldChar[@w:fldCharType = 'separate']][1][self::w:instrText]">
        <!-- check first is field does not end in context paragraph -->
        <xsl:choose>
          <xsl:when
                      test="not($instrText/parent::w:r/following-sibling::w:r[1]/w:fldChar[@w:fldCharType = 'end'])">
            <xsl:call-template name="BuildFieldCode">
              <!-- we know now that first run having instrText is before end of field -->
              <xsl:with-param name="instrText"
                              select="$instrText/ancestor::w:p/following-sibling::w:p[w:r/w:instrText][1]/w:r[w:instrText][1]/w:instrText[1]"/>
              <xsl:with-param name="fieldCode" select="concat($fieldCode, $instrText/text())"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="concat($fieldCode, $instrText/text())"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:otherwise>
				<xsl:value-of select="concat($fieldCode, $instrText/text())"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

  <!-- return a field type if it is contained in param string -->
	<xsl:template name="GetFieldTypeFromCode">
		<xsl:param name="fieldCode"/>
		<!-- field can start with space, but first none-space text is field code -->
		<xsl:variable name="newFieldCode">
			<xsl:call-template name="suppressFieldCodeFirstSpaceChar">
				<xsl:with-param name="string" select="$fieldCode"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="contains($newFieldCode, ' ')">
				<xsl:value-of select="substring-before($newFieldCode, ' ')"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$newFieldCode"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!-- suppress first space char in a given string (used for field codes -->
	<xsl:template name="suppressFieldCodeFirstSpaceChar">
		<xsl:param name="string"/>
		<xsl:choose>
			<xsl:when test="substring($string, 1, 1) = ' ' ">
				<xsl:call-template name="suppressFieldCodeFirstSpaceChar">
					<xsl:with-param name="string" select="substring($string, 2, string-length($string) - 1)"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$string"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!--
  Summary: writes the declaration of a user field
  Author: makz (DIaLOGIKa)
  Date: 2.11.2007
  -->
	<xsl:template name="InsertUserVariableDecl">
		<xsl:param name="fieldCode"/>

		<xsl:variable name="fieldName">
			<xsl:call-template name="ExtractFieldName">
				<xsl:with-param name="fieldCode" select="$fieldCode" />
				<xsl:with-param name="fieldType" select="'SET'" />
			</xsl:call-template>
		</xsl:variable>
		<xsl:variable name="fieldValue">
			<xsl:call-template name="ExtractFieldValue">
				<xsl:with-param name="fieldCode" select="$fieldCode" />
				<xsl:with-param name="fieldName" select="$fieldName" />
				<xsl:with-param name="fieldType" select="'SET'" />
			</xsl:call-template>
		</xsl:variable>
		<xsl:variable name="valueType">
			<xsl:choose>
				<xsl:when test="number($fieldValue)">float</xsl:when>
				<xsl:otherwise>string</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<!--
        <text:user-field-decl>
            <xsl:attribute name="text:name">
                <xsl:value-of select="$fieldName"/>
            </xsl:attribute>
            <xsl:attribute name="office:value-type">
                <xsl:value-of select="$valueType"/>
            </xsl:attribute>
            <xsl:choose>
                <xsl:when test="$valueType = 'float' or $valueType = 'percentage' ">
                    <xsl:attribute name="office:value">
                        <xsl:value-of select="$fieldValue"/>
                    </xsl:attribute>
                </xsl:when>
                <xsl:when test="$valueType = 'currency' ">
                    <xsl:attribute name="office:value">
                        <xsl:value-of select="$fieldValue"/>
                    </xsl:attribute>
                    <xsl:attribute name="office:currency"/>
                </xsl:when>
                <xsl:when test="$valueType = 'date' ">
                    <xsl:attribute name="office:date-value">
                        <xsl:value-of select="$fieldValue"/>
                    </xsl:attribute>
                </xsl:when>
                <xsl:when test="$valueType = 'time' ">
                    <xsl:attribute name="office:time-value">
                        <xsl:value-of select="$fieldValue"/>
                    </xsl:attribute>
                </xsl:when>
                <xsl:when test="$valueType = 'boolean' ">
                    <xsl:attribute name="office:boolean-value">
                        <xsl:value-of select="$fieldValue"/>
                    </xsl:attribute>
                </xsl:when>
                <xsl:when test="$valueType = 'string' ">
                    <xsl:attribute name="office:string-value">
                        <xsl:value-of select="$fieldValue"/>
                    </xsl:attribute>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:attribute name="office:value">
                        <xsl:value-of select="$fieldValue"/>
                    </xsl:attribute>
                </xsl:otherwise>
            </xsl:choose>
        </text:user-field-decl>-->
	</xsl:template>

	<!--
  Summary: writes the user field
  Author: Clever Age
  Modified: makz (DIaLOGIKa)
  Date: 2.11.2007
  -->
	<xsl:template name="InsertUserVariable">
		<xsl:param name="fieldCode"/>

		<!-- troncate field to find arguments -->
		<xsl:variable name="fieldName">
			<xsl:call-template name="ExtractFieldName">
				<xsl:with-param name="fieldCode" select="$fieldCode" />
				<xsl:with-param name="fieldType" select="'REF'" />
			</xsl:call-template>
		</xsl:variable>

		<!--
        <text:user-field-get>
            <xsl:attribute name="text:name">
                <xsl:value-of select="$fieldName"/>
            </xsl:attribute>
            <xsl:value-of select="w:r/w:t"/>
        </text:user-field-get>
        -->

		<!--
    <xsl:variable name="fieldValue">
      <xsl:call-template name="ExtractFieldValue">
        <xsl:with-param name="fieldCode" select="$fieldCode" />
        <xsl:with-param name="fieldName" select="$fieldName" />
      </xsl:call-template>
    </xsl:variable>
    -->

		<!-- COMMENT : variable fields should be declared before set, but application should support direct set before declaration,
      and it is too complex to find wether the field has already been declared. So no declaration is performed. 
    <text:variable-set>
      <xsl:attribute name="text:name">
        <xsl:value-of select="$fieldName"/>
      </xsl:attribute>
      <xsl:variable name="valueType">
        <xsl:choose>
          <xsl:when test="number($fieldValue)">float</xsl:when>
          <xsl:otherwise>string</xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <xsl:attribute name="office:value-type">
        <xsl:value-of select="$valueType"/>
      </xsl:attribute>
      
      <xsl:choose>
        <xsl:when test="$valueType = 'float' or $valueType = 'percentage' ">
          <xsl:attribute name="office:value">
            <xsl:value-of select="$fieldValue"/>
          </xsl:attribute>
        </xsl:when>
        <xsl:when test="$valueType = 'currency' ">
          <xsl:attribute name="office:value">
            <xsl:value-of select="$fieldValue"/>
          </xsl:attribute>
          <xsl:attribute name="office:currency"/>
        </xsl:when>
        <xsl:when test="$valueType = 'date' ">
          <xsl:attribute name="office:date-value">
            <xsl:value-of select="$fieldValue"/>
          </xsl:attribute>
        </xsl:when>
        <xsl:when test="$valueType = 'time' ">
          <xsl:attribute name="office:time-value">
            <xsl:value-of select="$fieldValue"/>
          </xsl:attribute>
        </xsl:when>
        <xsl:when test="$valueType = 'boolean' ">
          <xsl:attribute name="office:boolean-value">
            <xsl:value-of select="$fieldValue"/>
          </xsl:attribute>
        </xsl:when>
        <xsl:when test="$valueType = 'string' ">
          <xsl:attribute name="office:string-value">
            <xsl:value-of select="$fieldValue"/>
          </xsl:attribute>
        </xsl:when>
        <xsl:otherwise>
          <xsl:attribute name="office:value">
            <xsl:value-of select="$fieldValue"/>
          </xsl:attribute>
        </xsl:otherwise>
      </xsl:choose>
      <xsl:attribute name="text:display">none</xsl:attribute>
    </text:variable-set>
    -->
	</xsl:template>

	<!--
  Summary: extracts the field name out of a field code
  Author: makz (DIaLOGIKa)
  Date: 2.11.2007
  -->
	<xsl:template name="ExtractFieldName">
		<xsl:param name="fieldCode" />
		<xsl:param name="fieldType" />

		<xsl:variable name="newFieldCode">
			<xsl:call-template name="suppressFieldCodeFirstSpaceChar">
				<xsl:with-param name="string" select="substring-after($fieldCode, $fieldType)"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:variable name="fieldName">
			<xsl:choose>
				<xsl:when test="contains($newFieldCode, ' ')">
					<xsl:value-of select="substring-before($newFieldCode, ' ')"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$newFieldCode"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="contains($fieldName, '&quot;')">
				<xsl:value-of select="substring-before(substring-after($fieldName, '&quot;'), '&quot;')"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$fieldName"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!--
  Summary: extracts the field value out of a field code
  Author: makz (DIaLOGIKa)
  Date: 2.11.2007
  -->
	<xsl:template name="ExtractFieldValue">
		<xsl:param name="fieldCode" />
		<xsl:param name="fieldName" />
		<xsl:param name="fieldType" />

		<xsl:variable name="newFieldCode">
			<xsl:call-template name="suppressFieldCodeFirstSpaceChar">
				<xsl:with-param name="string" select="substring-after(substring-after($fieldCode, $fieldType), $fieldName)" />
			</xsl:call-template>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="substring($newFieldCode, 1,1) = &apos;&quot;&apos;">
				<xsl:value-of select="substring-before(substring-after($newFieldCode, &apos;&quot;&apos;), &apos;&quot;&apos;)" />
			</xsl:when>
			<xsl:when test="contains($newFieldCode, ' ')">
				<xsl:value-of select="substring-before($newFieldCode, ' ')"/>
			</xsl:when>
			<!--xsl:when test="$newFieldCode = '' ">
          < at least a blank space >
          <xsl:text xml:space="preserve"> </xsl:text>
          </xsl:when-->
			<xsl:otherwise>
				<xsl:value-of select="$newFieldCode"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!-- alphabetical index mark -->
	<xsl:template name="InsertIndexMark">
		<xsl:param name="instrText"/>
		<xsl:variable name="Value">
			<xsl:value-of select="substring-before(substring-after($instrText,'&quot;'),'&quot;')" />
		</xsl:variable>

		<!--
        <text:alphabetical-index-mark>
            <xsl:choose>
                <xsl:when test="not(contains($Value, ':'))">
                    <xsl:attribute name="text:string-value">
                        <xsl:value-of select="$Value"/>
                    </xsl:attribute>
                </xsl:when>
                <xsl:when test="contains($Value, ':')">
                    <xsl:variable name="TextKey1">
                        <xsl:value-of select="substring-before($Value, ':')"/>
                    </xsl:variable>
                    <xsl:variable name="TextKey2">
                        <xsl:value-of select="substring-after($Value, ':')"/>
                    </xsl:variable>
                    <xsl:choose>
                        <xsl:when test="contains($TextKey2, ':')">
                            <xsl:attribute name="text:string-value">
                                <xsl:value-of select="substring-after($TextKey2, ':')"/>
                            </xsl:attribute>
                            <xsl:attribute name="text:key2">
                                <xsl:value-of select="substring-before($TextKey2, ':')"/>
                            </xsl:attribute>
                        </xsl:when>
                        <xsl:otherwise>
                            <xsl:attribute name="text:string-value">
                                <xsl:value-of select="$TextKey2"/>
                            </xsl:attribute>
                        </xsl:otherwise>
                    </xsl:choose>
                    <xsl:attribute name="text:key1">
                        <xsl:value-of select="$TextKey1"/>
                    </xsl:attribute>
                </xsl:when>
            </xsl:choose>
        </text:alphabetical-index-mark>
        -->
	</xsl:template>

	<!--
  Summary: inserts a reference to bookmark
  Author: Clever Age
  Modified: makz (DIaLOGIKa)
  Date: 2.11.2007
  -->
	<xsl:template name="InsertCrossReference">
		<xsl:param name="fieldCode"/>
		<xsl:param name="displayValue" />

		<!--clam, dialogika: bugfix 2000762-->
		<xsl:variable name="ReferenceFormat">
			<xsl:choose>
				<xsl:when test="contains($fieldCode, ' \w')">chapter</xsl:when>
				<xsl:otherwise>text</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<!--
        <text:bookmark-ref>
            <xsl:attribute name="text:reference-format">
                <xsl:value-of select="$ReferenceFormat"/>
            </xsl:attribute>
            <xsl:choose>
                <xsl:when test="contains($fieldCode, 'PAGEREF')">
                    <xsl:attribute name="text:ref-name">
                        <xsl:choose>
                            <xsl:when test="contains($fieldCode, ' \')">
                                <xsl:value-of select="substring-before(concat('_',substring-after($fieldCode,'_')), ' \')"/>
                            </xsl:when>
                            <xsl:otherwise>
                                <xsl:value-of select="concat('_',substring-after($fieldCode,'_'))"/>
                            </xsl:otherwise>
                        </xsl:choose>
                    </xsl:attribute>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:attribute name="text:ref-name">
                        <!-@-clam, dialogika: trim inserted because of bug 1839938-@->
                        <xsl:choose>
                            <xsl:when test="contains($fieldCode, ' \')">
                                <xsl:call-template name="trim">
                                    <xsl:with-param name="s" select="substring-before(substring-after($fieldCode, 'REF '), ' \')" />
                                </xsl:call-template>
                            </xsl:when>
                            <xsl:otherwise>
                                <xsl:call-template name="trim">
                                    <xsl:with-param name="s" select="substring-after($fieldCode, 'REF ')" />
                                </xsl:call-template>
                            </xsl:otherwise>
                        </xsl:choose>

                    </xsl:attribute>
                </xsl:otherwise>
            </xsl:choose>

            <xsl:choose>
                <xsl:when test="name()='w:instrText'">
                    <!-@-clam, dialogika: bugfix 2000762-@->
                    <!-@-<xsl:apply-templates select="../following-sibling::w:r/w:t"/>-@->
                    <xsl:call-template name="InsertInstrTextFieldText">
                        <xsl:with-param name="currentRun" select="../following-sibling::w:r[1]"></xsl:with-param>
                    </xsl:call-template>
                </xsl:when>
                <!-@-clam, dialogika: bugfix 2000762-@->
                <xsl:when test="name() = 'w:fldSimple'">
                    <xsl:apply-templates select="w:r/w:t"/>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:apply-templates select="following-sibling::w:r/w:t"/>
                </xsl:otherwise>
            </xsl:choose>
        </text:bookmark-ref>
        -->
	</xsl:template>

	<!--clam, dialogika: bugfix 2000762-->
	<xsl:template name="InsertInstrTextFieldText">
		<xsl:param name="currentRun"/>
		<xsl:choose>
			<xsl:when test="$currentRun/w:fldChar[@w:fldCharType = 'end']">
				<!--stop it-->
			</xsl:when>
			<xsl:when test="$currentRun/w:t">
				<xsl:apply-templates select="$currentRun/w:t"/>
				<xsl:if test="$currentRun/following-sibling::w:r">
					<xsl:call-template name="InsertInstrTextFieldText">
						<xsl:with-param name="currentRun" select="$currentRun/following-sibling::w:r[1]"></xsl:with-param>
					</xsl:call-template>
				</xsl:if>
			</xsl:when>
			<xsl:otherwise>
				<xsl:if test="$currentRun/following-sibling::w:r">
					<xsl:call-template name="InsertInstrTextFieldText">
						<xsl:with-param name="currentRun" select="$currentRun/following-sibling::w:r[1]"></xsl:with-param>
					</xsl:call-template>
				</xsl:if>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="InsertFieldCharFieldContent">
		<xsl:variable name="fieldCharId" select="generate-id(.)"/>
		<!-- element that starts field-->
		<xsl:variable name="fieldBegin"
          select="preceding::w:fldChar[@w:fldCharType='begin' and not(following::w:fldChar
      [@w:fldCharType='end' and generate-id(following::w:instrText[contains(self::node(),'REF')][1]) = $fieldCharId ]) ][1] "/>
		<!-- element that ends field-->
		<xsl:variable name="fieldEnd"
          select="following::w:fldChar[@w:fldCharType='end' and not(preceding::w:fldChar
      [@w:fldCharType='begin' and generate-id(preceding::w:instrText[contains(self::node(),'REF')][1]) = fieldCharId ]) ][1] "/>
		<!--paragraph that contains field-->
		<xsl:variable name="fieldParagraph" select="generate-id(ancestor::w:p)"/>

		<!--  inserts field  text contents from beginning to ending  element -->
		<xsl:for-each
          select="$fieldBegin/following::w:t[generate-id(ancestor::w:p) = $fieldParagraph
      and generate-id(preceding::w:fldChar[1]) != generate-id($fieldEnd) ]  ">
			<xsl:apply-templates/>
		</xsl:for-each>
	</xsl:template>

	<xsl:template name="InsertUserName">
		<!--
        <text:author-name text:fixed="false">
            <xsl:apply-templates select="w:r/child::node()"/>
        </text:author-name>
        -->
	</xsl:template>

	<xsl:template name="InsertAuthor">
		<!--
        <text:initial-creator>
            <xsl:apply-templates select="w:r/child::node()"/>
        </text:initial-creator>
        -->
	</xsl:template>

	<xsl:template name="InsertCreator">
		<!--
        <text:creator>
            <xsl:apply-templates select="w:r/child::node()"/>
        </text:creator>
        -->
	</xsl:template>

	<!--user initials-->
	<xsl:template name="InsertUserInitials">
		<!--
        <text:author-initials>
            <xsl:apply-templates select="w:r/child::node()"/>
        </text:author-initials>
        -->
	</xsl:template>

	<xsl:template name="InsertSubject">
		<!--
        <text:subject>
            <!-@-
      makz: Changed for bugfix 2088835.
      
      <xsl:apply-templates select="w:r/child::node()"/>
      -@->
            <xsl:choose>
                <xsl:when test="name() = 'w:fldSimple'">
                    <xsl:apply-templates select="w:r" />
                </xsl:when>
                <xsl:when test="name() = 'w:instrText'">
                    <!-@-get this field id -@->
                    <xsl:variable name="fieldId" select="../@oox:fid" />
                    <!-@- convert all runs of this field -@->
                    <xsl:apply-templates select="../following-sibling::w:r[@oox:fid=$fieldId and w:t]">
                        <xsl:with-param name="ignoreFieldFlag" select="'true'" />
                    </xsl:apply-templates>
                </xsl:when>
            </xsl:choose>
        </text:subject>
        -->
	</xsl:template>

	<xsl:template name="InsertTitle">
		<!--
        <text:title>
            <!-@-
      makz: Changed for bugfix 2088835.

      <xsl:apply-templates select="w:r/child::node()"/>
      -@->
            <xsl:choose>
                <xsl:when test="name() = 'w:fldSimple'">
                    <xsl:apply-templates select="w:r" />
                </xsl:when>
                <xsl:when test="name() = 'w:instrText'">
                    <!-@-get this field id -@->
                    <xsl:variable name="fieldId" select="../@oox:fid" />
                    <!-@- convert all runs of this field -@->
                    <xsl:apply-templates select="../following-sibling::w:r[@oox:fid=$fieldId and w:t]">
                        <xsl:with-param name="ignoreFieldFlag" select="'true'" />
                    </xsl:apply-templates>
                </xsl:when>
            </xsl:choose>
        </text:title>
        -->
	</xsl:template>

	<xsl:template name="InsertPageCount">
		<xsl:choose>
			<xsl:when test="./w:r/w:t">
				<!--
                <text:page-count>
                    <xsl:value-of select="./w:r/w:t"/>
                </text:page-count>
                -->
			</xsl:when>
			<xsl:otherwise>
				<!--
                <text:page-count/>
                -->
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<xsl:template name="InsertSequence">
		<xsl:param name="fieldCode"/>
		<xsl:param name="sequenceContext"/>
		<xsl:variable name="refType">
			<xsl:value-of select="substring-before(substring-after($fieldCode,'SEQ '),' ')"/>
		</xsl:variable>

		<!--
        <text:sequence text:ref-name="{concat('ref',concat($refType,$sequenceContext/w:t))}"
          text:name="{$refType}" text:formula="{concat(concat('ooow:',$refType),'+1')}">
            <xsl:apply-templates select="$sequenceContext/child::node()"/>
        </text:sequence>
        -->
	</xsl:template>

	<xsl:template name="InsertDateStyle">
		<xsl:param name="fieldCode"/>
		<xsl:param name="dateText"/>

		<xsl:variable name="FormatDate">
			<xsl:value-of select="substring-before(substring-after($dateText, '&quot;'), '&quot;')"/>
		</xsl:variable>

		<!-- some of the DOCPROPERTY date field types have constant date format, 
      which is not saved in fieldCode so it need to be given directly in these cases-->
		<xsl:choose>

			<xsl:when test="contains($fieldCode, 'CREATETIME') or contains($fieldCode,'LASTESAVEDTIME')">
				<xsl:call-template name="InsertDocprTimeStyle"/>
			</xsl:when>

			<xsl:when test="contains($fieldCode, 'CREATEDATE') or contains($fieldCode, 'SAVEDATE') or contains($fieldCode, 'PRINTDATE')">
				<xsl:call-template name="InsertCreationDateFormat"/>
			</xsl:when>

			<!--default scenario-->
			<xsl:otherwise>
				<xsl:call-template name="InsertDateFormat">
					<xsl:with-param name="FormatDate">
						<xsl:value-of select="$FormatDate"/>
					</xsl:with-param>
					<xsl:with-param name="ParamField">
						<xsl:text>DATE</xsl:text>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="InsertCreationDateFormat">
		<!--
        <number:date-style style:name="{generate-id()}">
            <number:day/>
            <number:text xml:space="preserve"> </number:text>
            <number:month number:style="long" number:textual="true"/>
            <number:text xml:space="preserve"> </number:text>
            <number:year number:style="long"/>
        </number:date-style>
        -->
	</xsl:template>

	<!--DOCPROPERTY CreateDate, SaveDate, PrintDate default format-->
	<xsl:template name="InsertDocprLongDateStyle">
		<!--
        <number:date-style style:name="{generate-id()}">
            <number:month number:style="long"/>
            <number:text>-</number:text>
            <number:day number:style="long"/>
            <number:text>-</number:text>
            <number:year number:style="long"/>
            <number:text>
                <xsl:text> </xsl:text>
            </number:text>
            <number:hours number:style="long"/>
            <number:text>:</number:text>
            <number:minutes number:style="long"/>
            <number:text>:</number:text>
            <number:seconds number:style="long"/>
            <number:text>
                <xsl:text> </xsl:text>
            </number:text>
            <number:am-pm/>
        </number:date-style>
        -->
	</xsl:template>

	<!--DOCPROPERTY CreateTime, LastSavedTime default format-->
	<xsl:template name="InsertDocprTimeStyle">
		<!--
        <number:date-style style:name="{generate-id()}">
            <number:year number:style="long"/>
            <number:text>-</number:text>
            <number:month number:style="long"/>
            <number:text>-</number:text>
            <number:day number:style="long"/>
            <number:text>
                <xsl:text> </xsl:text>
            </number:text>
            <number:hours number:style="long"/>
            <number:text>:</number:text>
            <number:minutes number:style="long"/>
        </number:date-style>
        -->
	</xsl:template>

	<xsl:template name="InsertTimeStyle">
		<xsl:param name="timeText"/>
		<xsl:variable name="FormatDate">
			<xsl:value-of
              select="substring-before(substring-after($timeText, '&quot;'), '&quot;')"/>
		</xsl:variable>
		<xsl:call-template name="InsertDateFormat">
			<xsl:with-param name="FormatDate">
				<xsl:value-of select="$FormatDate"/>
			</xsl:with-param>
			<xsl:with-param name="ParamField">
				<xsl:text>TIME</xsl:text>
			</xsl:with-param>
		</xsl:call-template>
	</xsl:template>

	<xsl:template name="InsertFieldProperties">
		<xsl:param name="fieldCodeContainer" select="ancestor::w:fldSimple | ancestor::w:r/w:instrText"/>

		<xsl:variable name="fieldCode">
			<xsl:choose>
				<xsl:when test="$fieldCodeContainer/@w:instr">
					<xsl:value-of select="$fieldCodeContainer/@w:instr"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name="BuildFieldCode">
						<xsl:with-param name="instrText" select="$fieldCodeContainer"/>
					</xsl:call-template>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:choose>
			<xsl:when test="contains($fieldCode,'Upper')">
				<xsl:attribute name="fo:text-transform">
					<xsl:text>uppercase</xsl:text>
				</xsl:attribute>
			</xsl:when>
			<xsl:when test="contains($fieldCode,'Lower')">
				<xsl:attribute name="fo:text-transform">
					<xsl:text>lowercase</xsl:text>
				</xsl:attribute>
			</xsl:when>
			<xsl:when test="contains($fieldCode,'FirstCap') or contains($fieldCode,'Caps')">
				<xsl:attribute name="fo:text-transform">
					<xsl:text>capitalize</xsl:text>
				</xsl:attribute>
			</xsl:when>
			<xsl:when test="contains($fieldCode,'SBCHAR')">
				<xsl:attribute name="fo:letter-spacing">
					<xsl:text>-0.018cm</xsl:text>
				</xsl:attribute>
			</xsl:when>
			<xsl:when test="contains($fieldCode,'DBCHAR')">
				<xsl:attribute name="fo:letter-spacing">
					<xsl:text>0.176cm</xsl:text>
				</xsl:attribute>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="InsertDateFormat">
		<xsl:param name="FormatDate"/>
		<xsl:param name="ParamField"/>
		<!--
        <number:date-style>
            <xsl:attribute name="style:name">
                <xsl:value-of select="generate-id()"/>
            </xsl:attribute>
            <xsl:call-template name="InsertFormatDateStyle">
                <xsl:with-param name="FormatDate">
                    <xsl:choose>
                        <xsl:when test="$FormatDate!=''">
                            <xsl:value-of select="$FormatDate"/>
                        </xsl:when>
                        <xsl:otherwise>
                            <xsl:choose>
                                <xsl:when test="$ParamField='TIME'">
                                    <xsl:text>HH:mm</xsl:text>
                                </xsl:when>
                                <xsl:when test="$ParamField='DATE'">
                                    <xsl:text>yyyy-MM-dd</xsl:text>
                                </xsl:when>
                            </xsl:choose>
                        </xsl:otherwise>
                    </xsl:choose>
                </xsl:with-param>
            </xsl:call-template>
        </number:date-style>
        -->
	</xsl:template>

	<xsl:template name="InsertFormatDateStyle">
		<xsl:param name="FormatDate"/>
		<xsl:param name="DateText"/>
		<xsl:choose>

			<xsl:when test="starts-with($FormatDate, 'yyyy')">
				<xsl:if test="$DateText">
					<!--
                    <number:text>
                        <xsl:value-of select="$DateText"/>
                    </number:text>
                    -->
				</xsl:if>
				<!--
                <number:year number:style="long"/>
                -->
				<xsl:call-template name="InsertFormatDateStyle">
					<xsl:with-param name="FormatDate">
						<xsl:value-of select="substring-after($FormatDate, 'yyyy')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>

			<xsl:when test="starts-with($FormatDate, 'yy')">
				<xsl:if test="$DateText">
					<!--
                    <number:text>
                        <xsl:value-of select="$DateText"/>
                    </number:text>
                    -->
				</xsl:if>
				<!--
                <number:year/>
                -->
				<xsl:call-template name="InsertFormatDateStyle">
					<xsl:with-param name="FormatDate">
						<xsl:value-of select="substring-after($FormatDate, 'yy')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>

			<xsl:when test="starts-with($FormatDate, 'y')">
				<xsl:if test="$DateText">
					<!--
                    <number:text>
                        <xsl:value-of select="$DateText"/>
                    </number:text>
                    -->
				</xsl:if>
				<!--
                <number:year/>
                -->
				<xsl:call-template name="InsertFormatDateStyle">
					<xsl:with-param name="FormatDate">
						<xsl:value-of select="substring-after($FormatDate, 'y')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>

			<xsl:when test="starts-with($FormatDate, 'YYYY')">
				<xsl:if test="$DateText">
					<!--
                    <number:text>
                        <xsl:value-of select="$DateText"/>
                    </number:text>
                    -->
				</xsl:if>
				<!--
                <number:year number:style="long"/>
                -->
				<xsl:call-template name="InsertFormatDateStyle">
					<xsl:with-param name="FormatDate">
						<xsl:value-of select="substring-after($FormatDate, 'YYYY')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>

			<xsl:when test="starts-with($FormatDate, 'YY')">
				<xsl:if test="$DateText">
					<!--
                    <number:text>
                        <xsl:value-of select="$DateText"/>
                    </number:text>
                    -->
				</xsl:if>
				<!--
                <number:year/>
                -->
				<xsl:call-template name="InsertFormatDateStyle">
					<xsl:with-param name="FormatDate">
						<xsl:value-of select="substring-after($FormatDate, 'YY')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>

			<xsl:when test="starts-with($FormatDate, 'Y')">
				<xsl:if test="$DateText">
					<!--
                    <number:text>
                        <xsl:value-of select="$DateText"/>
                    </number:text>
                    -->
				</xsl:if>
				<!--
                <number:year/>
                -->
				<xsl:call-template name="InsertFormatDateStyle">
					<xsl:with-param name="FormatDate">
						<xsl:value-of select="substring-after($FormatDate, 'Y')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>

			<xsl:when test="starts-with($FormatDate, 'MMMM')">
				<xsl:if test="$DateText">
					<!--
                    <number:text>
                        <xsl:value-of select="$DateText"/>
                    </number:text>
                    -->
				</xsl:if>
				<!--
                <number:month number:style="long" number:textual="true"/>
                -->
				<xsl:call-template name="InsertFormatDateStyle">
					<xsl:with-param name="FormatDate">
						<xsl:value-of select="substring-after($FormatDate, 'MMMM')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>

			<xsl:when test="starts-with($FormatDate, 'MMM')">
				<xsl:if test="$DateText">
					<!--
                    <number:text>
                        <xsl:value-of select="$DateText"/>
                    </number:text>
                    -->
				</xsl:if>
				<!--
                <number:month number:textual="true"/>
                -->
				<xsl:call-template name="InsertFormatDateStyle">
					<xsl:with-param name="FormatDate">
						<xsl:value-of select="substring-after($FormatDate, 'MMM')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>

			<xsl:when test="starts-with($FormatDate, 'MM')">
				<xsl:if test="$DateText">
					<!--
                    <number:text>
                        <xsl:value-of select="$DateText"/>
                    </number:text>
                    -->
				</xsl:if>
				<!--
                <number:month number:style="long"/>
                -->
				<xsl:call-template name="InsertFormatDateStyle">
					<xsl:with-param name="FormatDate">
						<xsl:value-of select="substring-after($FormatDate, 'MM')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>

			<xsl:when test="starts-with($FormatDate, 'M')">
				<xsl:if test="$DateText">
					<!--
                    <number:text>
                        <xsl:value-of select="$DateText"/>
                    </number:text>
                    -->
				</xsl:if>
				<!--
                <number:month/>
                -->
				<xsl:call-template name="InsertFormatDateStyle">
					<xsl:with-param name="FormatDate">
						<xsl:value-of select="substring-after($FormatDate, 'M')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>

			<xsl:when test="starts-with($FormatDate, 'dddd')">
				<xsl:if test="$DateText">
					<!--
                    <number:text>
                        <xsl:value-of select="$DateText"/>
                    </number:text>
                    -->
				</xsl:if>
				<!--
                <number:day-of-week number:style="long"/>
                -->
				<xsl:call-template name="InsertFormatDateStyle">
					<xsl:with-param name="FormatDate">
						<xsl:value-of select="substring-after($FormatDate, 'dddd')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>

			<xsl:when test="starts-with($FormatDate, 'ddd')">
				<xsl:if test="$DateText">
					<!--
                    <number:text>
                        <xsl:value-of select="$DateText"/>
                    </number:text>
                    -->
				</xsl:if>
				<!--
                <number:day-of-week/>
                -->
				<xsl:call-template name="InsertFormatDateStyle">
					<xsl:with-param name="FormatDate">
						<xsl:value-of select="substring-after($FormatDate, 'ddd')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>

			<xsl:when test="starts-with($FormatDate, 'dd')">
				<xsl:if test="$DateText">
					<!--
                    <number:text>
                        <xsl:value-of select="$DateText"/>
                    </number:text>
                    -->
				</xsl:if>
				<!--
                <number:day number:style="long"/>
                -->
				<xsl:call-template name="InsertFormatDateStyle">
					<xsl:with-param name="FormatDate">
						<xsl:value-of select="substring-after($FormatDate, 'dd')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>

			<xsl:when test="starts-with($FormatDate, 'd')">
				<xsl:if test="$DateText">
					<!--
                    <number:text>
                        <xsl:value-of select="$DateText"/>
                    </number:text>
                    -->
				</xsl:if>
				<!--
                <number:day number:style="long"/>
                -->
				<xsl:call-template name="InsertFormatDateStyle">
					<xsl:with-param name="FormatDate">
						<xsl:value-of select="substring-after($FormatDate, 'd')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>

			<xsl:when test="starts-with($FormatDate, 'DDDD')">
				<xsl:if test="$DateText">
					<!--
                    <number:text>
                        <xsl:value-of select="$DateText"/>
                    </number:text>
                    -->
				</xsl:if>
				<!--
                <number:day-of-week number:style="long"/>
                -->
				<xsl:call-template name="InsertFormatDateStyle">
					<xsl:with-param name="FormatDate">
						<xsl:value-of select="substring-after($FormatDate, 'DDDD')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>

			<xsl:when test="starts-with($FormatDate, 'DDD')">
				<xsl:if test="$DateText">
					<!--
                    <number:text>
                        <xsl:value-of select="$DateText"/>
                    </number:text>
                    -->
				</xsl:if>
				<!--
                <number:day-of-week/>
                -->
				<xsl:call-template name="InsertFormatDateStyle">
					<xsl:with-param name="FormatDate">
						<xsl:value-of select="substring-after($FormatDate, 'DDD')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>

			<xsl:when test="starts-with($FormatDate, 'DD')">
				<xsl:if test="$DateText">
					<!--
                    <number:text>
                        <xsl:value-of select="$DateText"/>
                    </number:text>
                    -->
				</xsl:if>
				<!--
                <number:day number:style="long"/>
                -->
				<xsl:call-template name="InsertFormatDateStyle">
					<xsl:with-param name="FormatDate">
						<xsl:value-of select="substring-after($FormatDate, 'DD')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>

			<xsl:when test="starts-with($FormatDate, 'D')">
				<xsl:if test="$DateText">
					<!--
                    <number:text>
                        <xsl:value-of select="$DateText"/>
                    </number:text>
                    -->
				</xsl:if>
				<!--
                <number:day number:style="long"/>
                -->
				<xsl:call-template name="InsertFormatDateStyle">
					<xsl:with-param name="FormatDate">
						<xsl:value-of select="substring-after($FormatDate, 'D')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>


			<xsl:when test="starts-with($FormatDate, 'hh')">
				<xsl:if test="$DateText">
					<!--
                    <number:text>
                        <xsl:value-of select="$DateText"/>
                    </number:text>
                    -->
				</xsl:if>
				<!--
                <number:hours number:style="long"/>
                -->
				<xsl:call-template name="InsertFormatDateStyle">
					<xsl:with-param name="FormatDate">
						<xsl:value-of select="substring-after($FormatDate, 'hh')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>

			<xsl:when test="starts-with($FormatDate, 'h')">
				<xsl:if test="$DateText">
					<!--
                    <number:text>
                        <xsl:value-of select="$DateText"/>
                    </number:text>
                    -->
				</xsl:if>
				<!--
                <number:hours/>
                -->
				<xsl:call-template name="InsertFormatDateStyle">
					<xsl:with-param name="FormatDate">
						<xsl:value-of select="substring-after($FormatDate, 'h')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>

			<xsl:when test="starts-with($FormatDate, 'HH')">
				<xsl:if test="$DateText">
					<!--
                    <number:text>
                        <xsl:value-of select="$DateText"/>
                    </number:text>
                    -->
				</xsl:if>
				<!--
                <number:hours number:style="long"/>
                -->
				<xsl:call-template name="InsertFormatDateStyle">
					<xsl:with-param name="FormatDate">
						<xsl:value-of select="substring-after($FormatDate, 'HH')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>

			<xsl:when test="starts-with($FormatDate, 'H')">
				<xsl:if test="$DateText">
					<!--
                    <number:text>
                        <xsl:value-of select="$DateText"/>
                    </number:text>
                    -->
				</xsl:if>
				<!--
                <number:hours/>
                -->
				<xsl:call-template name="InsertFormatDateStyle">
					<xsl:with-param name="FormatDate">
						<xsl:value-of select="substring-after($FormatDate, 'H')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>

			<xsl:when test="starts-with($FormatDate, 'mm')">
				<xsl:if test="$DateText">
					<!--
                    <number:text>
                        <xsl:value-of select="$DateText"/>
                    </number:text>
                    -->
				</xsl:if>
				<!--
                <number:minutes number:style="long"/>
                -->
				<xsl:call-template name="InsertFormatDateStyle">
					<xsl:with-param name="FormatDate">
						<xsl:value-of select="substring-after($FormatDate, 'mm')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>

			<xsl:when test="starts-with($FormatDate, 'm')">
				<xsl:if test="$DateText">
					<!--
                    <number:text>
                        <xsl:value-of select="$DateText"/>
                    </number:text>
                    -->
				</xsl:if>
				<!--
                <number:minutes/>
                -->
				<xsl:call-template name="InsertFormatDateStyle">
					<xsl:with-param name="FormatDate">
						<xsl:value-of select="substring-after($FormatDate, 'm')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>

			<xsl:when test="starts-with($FormatDate, 'ss')">
				<xsl:if test="$DateText">
					<!--
                    <number:text>
                        <xsl:value-of select="$DateText"/>
                    </number:text>
                    -->
				</xsl:if>
				<!--
                <number:seconds number:style="long"/>
                -->
				<xsl:call-template name="InsertFormatDateStyle">
					<xsl:with-param name="FormatDate">
						<xsl:value-of select="substring-after($FormatDate, 'ss')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>

			<xsl:when test="starts-with($FormatDate, 's')">
				<xsl:if test="$DateText">
					<!--
                    <number:text>
                        <xsl:value-of select="$DateText"/>
                    </number:text>
                    -->
				</xsl:if>
				<!--
                <number:seconds/>
                -->
				<xsl:call-template name="InsertFormatDateStyle">
					<xsl:with-param name="FormatDate">
						<xsl:value-of select="substring-after($FormatDate, 's')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>

			<xsl:when test="starts-with($FormatDate, 'SS')">
				<xsl:if test="$DateText">
					<!--
                    <number:text>
                        <xsl:value-of select="$DateText"/>
                    </number:text>
                    -->
				</xsl:if>
				<!--
                <number:seconds number:style="long"/>
                -->
				<xsl:call-template name="InsertFormatDateStyle">
					<xsl:with-param name="FormatDate">
						<xsl:value-of select="substring-after($FormatDate, 'SS')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>

			<xsl:when test="starts-with($FormatDate, 'S')">
				<xsl:if test="$DateText">
					<!--
                    <number:text>
                        <xsl:value-of select="$DateText"/>
                    </number:text>
                    -->
				</xsl:if>
				<!--
                <number:seconds/>
                -->
				<xsl:call-template name="InsertFormatDateStyle">
					<xsl:with-param name="FormatDate">
						<xsl:value-of select="substring-after($FormatDate, 'S')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>

			<xsl:when test="starts-with($FormatDate, 'am/pm')">
				<xsl:if test="$DateText">
					<!--
                    <number:text>
                        <xsl:value-of select="$DateText"/>
                    </number:text>
                    -->
				</xsl:if>
				<!--
                <number:am-pm/>
                -->
				<xsl:call-template name="InsertFormatDateStyle">
					<xsl:with-param name="FormatDate">
						<xsl:value-of select="substring-after($FormatDate, 'am/pm')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>

			<xsl:otherwise>
				<xsl:variable name="Apostrof">
					<xsl:text>&apos;</xsl:text>
				</xsl:variable>

				<xsl:choose>
					<xsl:when test="contains(substring($FormatDate, 1, 1), $Apostrof)">
						<xsl:call-template name="InsertFormatDateStyle">
							<xsl:with-param name="FormatDate">
								<xsl:value-of
                                  select="substring-after(substring-after($FormatDate, $Apostrof), $Apostrof)"/>
							</xsl:with-param>
							<xsl:with-param name="DateText">
								<xsl:value-of
                                  select="concat($DateText, substring-before(substring-after($FormatDate, $Apostrof), $Apostrof))"
                />
							</xsl:with-param>
						</xsl:call-template>
					</xsl:when>

					<xsl:when test="substring-after($FormatDate, substring($FormatDate, 1, 1))">
						<xsl:call-template name="InsertFormatDateStyle">
							<xsl:with-param name="FormatDate">
								<xsl:value-of select="substring-after($FormatDate, substring($FormatDate, 1, 1))"/>
							</xsl:with-param>
							<xsl:with-param name="DateText">
								<xsl:value-of select="concat($DateText, substring($FormatDate, 1, 1))"/>
							</xsl:with-param>
						</xsl:call-template>
					</xsl:when>

					<xsl:otherwise>
						<xsl:if test="$DateText">
							<!--
                            <number:text>
                                <xsl:value-of select="$DateText"/>
                            </number:text>
                            -->
						</xsl:if>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!-- Page Number Field -->
	<xsl:template name="InsertPageNumber">
		<xsl:variable name="docName">
			<xsl:call-template name="GetDocumentName" />
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="$docName = 'document.xml'">
				<xsl:if test="following::w:sectPr[1]/w:pgNumType/@w:chapStyle">
					<!--
                    <text:chapter>
                        <xsl:attribute name="text:display">
                            <xsl:text>number</xsl:text>
                        </xsl:attribute>
                        <xsl:attribute name="text:outline-level">
                            <xsl:value-of select="following::w:sectPr[1]/w:pgNumType/@w:chapStyle"/>
                        </xsl:attribute>
                    </text:chapter>
                    -->
					<xsl:choose>
						<xsl:when test="following::w:sectPr[1]/w:pgNumType/@w:chapSep = 'period'">
							<xsl:text>.</xsl:text>
						</xsl:when>
						<xsl:when test="following::w:sectPr[1]/w:pgNumType/@w:chapSep = 'colon'">
							<xsl:text>:</xsl:text>
						</xsl:when>
						<xsl:otherwise>-</xsl:otherwise>
					</xsl:choose>
				</xsl:if>
			</xsl:when>
			<xsl:otherwise>

				<xsl:variable name="rId">
					<xsl:value-of select="key('Part', 'word/_rels/document.xml.rels')/Relationships/Relationship[@Target = $docName]/@Id" />
				</xsl:variable>
				<xsl:for-each select="key('Part', 'word/document.xml')">
					<xsl:for-each
                      select="key('sectPr', '')[w:headerReference/@r:id = $rId or w:footerReference/@r:id = $rId]">
						<xsl:if test="w:pgNumType/@w:chapStyle">
							<!--
                            <text:chapter>
                                <xsl:attribute name="text:display">
                                    <xsl:text>number</xsl:text>
                                </xsl:attribute>
                                <xsl:attribute name="text:outline-level">
                                    <xsl:value-of select="w:pgNumType/@w:chapStyle"/>
                                </xsl:attribute>
                            </text:chapter>
                            -->
							<xsl:choose>
								<xsl:when test="w:pgNumType/@w:chapSep = 'period'">
									<xsl:text>.</xsl:text>
								</xsl:when>
								<xsl:when test="following::w:sectPr[1]/w:pgNumType/@w:chapSep = 'colon'">
									<xsl:text>:</xsl:text>
								</xsl:when>
								<xsl:otherwise>-</xsl:otherwise>
							</xsl:choose>
						</xsl:if>
					</xsl:for-each>
				</xsl:for-each>
			</xsl:otherwise>
		</xsl:choose>

		<xsl:variable name="WInstr">
			<xsl:value-of select="./@w:instr"/>
		</xsl:variable>
		<!--<text:page-number>

            <xsl:variable name="rId" select="concat('',key('Part', 'word/_rels/document.xml.rels')/descendant::node()[@Target=$docName]/@Id)"/>
            <xsl:variable name="nodesectPr" select="key('sectPr', '')[w:headerReference/@r:id = $rId or w:footerReference/@r:id = $rId]"/>

            <!-@-dialogika, clam: bugfix (special workaround) for #1826728-@->
            <xsl:choose>
                <xsl:when test="$nodesectPr[1]/w:pgNumType/@w:start='0'">
                    <xsl:attribute name="text:select-page">
                        <xsl:text>previous</xsl:text>
                    </xsl:attribute>
                </xsl:when>
                <xsl:when test="$nodesectPr[1]/preceding::w:sectPr[w:pgNumType/@w:start][1]/w:pgNumType/@w:start='0'">
                    <xsl:attribute name="text:select-page">
                        <xsl:text>previous</xsl:text>
                    </xsl:attribute>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:attribute name="text:select-page">
                        <xsl:text>current</xsl:text>
                    </xsl:attribute>
                </xsl:otherwise>
            </xsl:choose>

            <xsl:variable name="standardNumType">
                <xsl:choose>
                    <xsl:when test="$nodesectPr/w:pgNumType/@w:fmt">
                        <xsl:choose>
                            <xsl:when test="$nodesectPr/w:pgNumType/@w:fmt = 'lowerRoman'">i</xsl:when>
                            <xsl:when test="$nodesectPr/w:pgNumType/@w:fmt = 'upperRoman'">I</xsl:when>
                            <xsl:when test="$nodesectPr/w:pgNumType/@w:fmt = 'lowerLetter'">a</xsl:when>
                            <xsl:when test="$nodesectPr/w:pgNumType/@w:fmt = 'upperLetter'">A</xsl:when>
                            <xsl:otherwise>1</xsl:otherwise>
                        </xsl:choose>
                    </xsl:when>
                    <xsl:otherwise>1</xsl:otherwise>
                </xsl:choose>
            </xsl:variable>
            <xsl:attribute name="style:num-format">
                <xsl:choose>
                    <xsl:when test="contains($WInstr, 'Arabic')">1</xsl:when>
                    <xsl:when test="contains($WInstr, 'alphabetic')">a</xsl:when>
                    <xsl:when test="contains($WInstr, 'ALPHABETIC')">A</xsl:when>
                    <xsl:when test="contains($WInstr, 'roman')">i</xsl:when>
                    <xsl:when test="contains($WInstr, 'ROMAN')">I</xsl:when>
                    <xsl:when test="$standardNumType">
                        <xsl:value-of select="$standardNumType"/>
                    </xsl:when>
                    <xsl:otherwise>1</xsl:otherwise>
                </xsl:choose>
            </xsl:attribute>
            <xsl:value-of select="./w:r/w:t"/>
        </text:page-number>
        -->
	</xsl:template>

	<!-- Insert Citations -->
	<xsl:template name="InsertTextBibliographyMark">
		<xsl:param name="TextIdentifier"/>

		<xsl:variable name="Path"
          select="key('Part', 'customXml/item1.xml')/b:Sources/b:Source[b:Tag = $TextIdentifier]"/>

		<xsl:variable name="BibliographyType" select="$Path/b:SourceType"/>

		<xsl:variable name="LastName">
			<xsl:choose>
				<xsl:when test="$Path/b:Author/b:Author/b:NameList/b:Person/b:Last">
					<xsl:value-of select="$Path/b:Author/b:Author/b:NameList/b:Person/b:Last"/>
				</xsl:when>
				<xsl:when test="$Path/b:Author/b:Author/b:Corporate">
					<xsl:value-of select="$Path/b:Author/b:Author/b:Corporate"/>
				</xsl:when>
				<xsl:otherwise/>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="FirstName">
			<xsl:choose>
				<xsl:when test="$Path/b:Author/b:Author/b:NameList/b:Person/b:First">
					<xsl:value-of select="$Path/b:Author/b:Author/b:NameList/b:Person/b:First"/>
				</xsl:when>
				<xsl:otherwise/>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="Middle">
			<xsl:choose>
				<xsl:when test="$Path/b:Author/b:Author/b:NameList/b:Person/b:Middle">
					<xsl:value-of select="$Path/b:Author/b:Author/b:NameList/b:Person/b:Middle"/>
				</xsl:when>
				<xsl:otherwise/>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="Author">
			<xsl:choose>
				<xsl:when test="$LastName != '' and $FirstName != '' and $Middle != ''">
					<xsl:value-of select="concat($LastName, ' ', $FirstName,' ', $Middle)"/>
				</xsl:when>
				<xsl:when test="$LastName != '' and $FirstName != ''">
					<xsl:value-of select="concat($LastName, ' ', $FirstName)"/>
				</xsl:when>
				<xsl:when test="$LastName != '' and $Middle != ''">
					<xsl:value-of select="concat($LastName, ' ', $Middle)"/>
				</xsl:when>
				<xsl:when test="$FirstName != '' and $Middle != ''">
					<xsl:value-of select="concat($FirstName,' ', $Middle)"/>
				</xsl:when>
				<xsl:when test="$LastName != ''">
					<xsl:value-of select="$LastName"/>
				</xsl:when>
				<xsl:when test="$FirstName != ''">
					<xsl:value-of select="$FirstName"/>
				</xsl:when>
				<xsl:when test="$Middle != ''">
					<xsl:value-of select="$Middle"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="City">
			<xsl:value-of select="$Path/b:City"/>
		</xsl:variable>

		<xsl:variable name="StateProvince">
			<xsl:value-of select="$Path/b:StateProvince"/>
		</xsl:variable>

		<xsl:variable name="CountryRegion">
			<xsl:value-of select="$Path/b:CountryRegion"/>
		</xsl:variable>

		<xsl:variable name="Address">
			<xsl:choose>
				<xsl:when test="$City != '' and $StateProvince != '' and $CountryRegion != ''">
					<xsl:value-of select="concat($City,' ',$StateProvince,' ',$CountryRegion)"/>
				</xsl:when>
				<xsl:when test="$City != '' and $StateProvince != ''">
					<xsl:value-of select="concat($City,' ',$StateProvince)"/>
				</xsl:when>
				<xsl:when test="$City != '' and $CountryRegion != ''">
					<xsl:value-of select="concat($City,' ',$CountryRegion)"/>
				</xsl:when>
				<xsl:when test="$StateProvince != '' and $CountryRegion != ''">
					<xsl:value-of select="concat($StateProvince,' ',$CountryRegion)"/>
				</xsl:when>
				<xsl:when test="$City != ''">
					<xsl:value-of select="$City"/>
				</xsl:when>
				<xsl:when test="$StateProvince != ''">
					<xsl:value-of select="$StateProvince"/>
				</xsl:when>
				<xsl:when test="$CountryRegion != ''">
					<xsl:value-of select="$CountryRegion"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="TextIdent">
			<xsl:value-of select="$LastName"/>
			<xsl:if test="$Path/b:Year">
				<xsl:text>, </xsl:text>
				<xsl:value-of select="$Path/b:Year"/>
			</xsl:if>
		</xsl:variable>

		<!--
        <text:bibliography-mark>
            <xsl:attribute name="text:identifier">
                <xsl:value-of select="$TextIdent"/>
            </xsl:attribute>
            <xsl:attribute name="text:bibliography-type">
                <xsl:choose>
                    <xsl:when test="$BibliographyType = 'Book'">
                        <xsl:text>book</xsl:text>
                    </xsl:when>
                    <xsl:when test="$BibliographyType = 'JournalArticle'">
                        <xsl:text>article</xsl:text>
                    </xsl:when>
                    <xsl:otherwise>
                        <xsl:text>book</xsl:text>
                    </xsl:otherwise>
                </xsl:choose>
            </xsl:attribute>
            <xsl:if test="$Author">
                <xsl:attribute name="text:author">
                    <xsl:value-of select="$Author"/>
                </xsl:attribute>
            </xsl:if>
            <xsl:if test="$Path/b:Title">
                <xsl:attribute name="text:title">
                    <xsl:value-of select="$Path/b:Title"/>
                </xsl:attribute>
            </xsl:if>
            <xsl:if test="$Path/b:Year">
                <xsl:attribute name="text:year">
                    <xsl:value-of select="$Path/b:Year"/>
                </xsl:attribute>
            </xsl:if>
            <xsl:if test="$Path/b:Publisher">
                <xsl:attribute name="text:publisher">
                    <xsl:value-of select="$Path/b:Publisher"/>
                </xsl:attribute>
            </xsl:if>
            <xsl:if test="$Address">
                <xsl:attribute name="text:address">
                    <xsl:value-of select="$Address"/>
                </xsl:attribute>
            </xsl:if>
            <xsl:if test="$Path/b:Volume">
                <xsl:attribute name="text:volume">
                    <xsl:value-of select="$Path/b:Volume"/>
                </xsl:attribute>
            </xsl:if>
            <xsl:if test="$Path/b:StandardNumber">
                <xsl:attribute name="text:number">
                    <xsl:value-of select="$Path/b:StandardNumber"/>
                </xsl:attribute>
            </xsl:if>
            <xsl:if test="$Path/b:Pages">
                <xsl:attribute name="text:pages">
                    <xsl:value-of select="$Path/b:Pages"/>
                </xsl:attribute>
            </xsl:if>
            <xsl:if test="$Path/b:Edition">
                <xsl:attribute name="text:edition">
                    <xsl:value-of select="$Path/b:Edition"/>
                </xsl:attribute>
            </xsl:if>
            <xsl:value-of select="$LastName"/>
        </text:bibliography-mark>
        -->
	</xsl:template>

  <!-- <xsl:template name="GetLinkText"> 
       Данный типлэйт используется для поиска текста между 'separate' и 'end' -->
	<xsl:template name="GetLinkText">
		<xsl:param name="CurNode"/>

    <!-- Забираем текст после 'separate' -->
    <!-- Смотрим все следущие поля -->
    <xsl:variable name="FldChars" select="$CurNode/following::w:fldChar[@w:fldCharType = 'end'][1] | $CurNode/following::w:t[1] | $CurNode/following::w:instrText[1]"/>

    <xsl:choose>

      <xsl:when test="count($FldChars) = 0 or count($FldChars[@w:fldCharType = 'end']) = 0">
        <!-- Пустой текст -->
        <xsl:text></xsl:text>
      </xsl:when>
      <xsl:when test="$FldChars[1]/@w:fldCharType = 'end'">
        <!-- Пустой текст -->
        <xsl:text></xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="left-trim">
          <xsl:with-param name="s" select="$FldChars[1]"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
    
    
  </xsl:template>

	<!-- <xsl:template name="InsertHyperlinkField">
       Данный типлэйт работает с fldChar типа HYPERLINK
  -->
	<xsl:template name="InsertHyperlinkField">
    <xsl:param name="strCommand"/>
    <xsl:param name="Text"/>
    <xsl:param name="CurNode" />

		<xsl:variable name="Link">
			<xsl:variable name="instrLen" select="string-length($strCommand)" />
			<xsl:choose>
				<xsl:when test="$instrLen &gt; 12">       
					<!-- Согласно спецификации HYPERLINK [switches] "link text"
               Пример: HYPERLINK \l "_Toc223257436" -->
					<xsl:value-of select="substring-before(substring-after($strCommand, '&quot;'), '&quot;')" />
          
				</xsl:when>
				<xsl:when test="$CurNode/../following-sibling::w:r[w:instrText]">
					<!-- TO DO: сделать по нормальному. Пробуем найти ссылку в следующей инструкции -->
					<xsl:value-of select="$CurNode/../following-sibling::w:r[w:instrText]/w:instrText" />
				</xsl:when>
				<xsl:otherwise>
					<!-- Ищем ссылку в следующей инструкции, которая возможно лежить в другом параграфе -->
					<xsl:value-of select="$CurNode/following::w:r[w:instrText][1]/w:instrText" />
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

    <!-- Ищем текст к инструкции --><!--
    
		<xsl:variable name="Text">
			<xsl:call-template name="GetLinkText">
				<xsl:with-param name="CurNode" select="$CurNode/following::w:fldChar[@w:fldCharType = 'separate'][1]"/>
			</xsl:call-template>
		</xsl:variable>-->

		<a xmlns="http://www.w3.org/1999/xhtml">
			<xsl:variable name="href">
				<xsl:choose>
					<xsl:when test="contains($Link, 'HYPERLINK')">
						<xsl:value-of select="concat( '#', substring-before(substring-after($Link, '&quot;'), '&quot;') )" />
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="concat( '#', $Link )"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<xsl:attribute name="href">
				<xsl:value-of select="$href" />
			</xsl:attribute>

			<xsl:value-of select="$Text"/>
		</a>

	</xsl:template>


  <!-- <xsl:template name="InsertPageRefField">
       Данный типлэйт работает с fldChar типа PAGEREF
  -->
  <xsl:template name="InsertPageRefField">
    <xsl:param name="strCommand"/>
    <xsl:param name="CurNode"/>
    <xsl:param name="Text"/>

    <xsl:variable name="Link">
      <xsl:variable name="instrLen" select="string-length($strCommand)" />
      <xsl:choose>
        <xsl:when test="$instrLen &gt; 12">
          <!-- Согласно спецификации PAGEREF link_text [switches] 
               Пример: PAGEREF \l _Toc223257436 -->
          <xsl:call-template name="FldInstrDeleteSwitches">
            <xsl:with-param name="strInstr" select="normalize-space(substring-after($strCommand, 'PAGEREF'))" />
          </xsl:call-template>

        </xsl:when>
        <xsl:when test="$CurNode/../following-sibling::w:r[w:instrText]">
          <!-- TO DO: сделать по нормальному. Пробуем найти ссылку в следующей инструкции -->
          <xsl:value-of select="$CurNode/../following-sibling::w:r[w:instrText]/w:instrText" />
        </xsl:when>
        <xsl:otherwise>
          <!-- Ищем ссылку в следующей инструкции, которая возможно лежить в другом параграфе -->
          <xsl:value-of select="$CurNode/following::w:r[w:instrText][1]/w:instrText" />
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <!-- Ищем текст к инструкции --><!--

    <xsl:variable name="Text">
      <xsl:call-template name="GetLinkText">
        <xsl:with-param name="CurNode" select="$CurNode/following::w:fldChar[@w:fldCharType = 'separate'][1]"/>
      </xsl:call-template>
    </xsl:variable>-->

    <a xmlns="http://www.w3.org/1999/xhtml">
      <xsl:variable name="href">
        <xsl:value-of select="concat( '#', $Link )"/>
      </xsl:variable>
      <xsl:attribute name="href">
        <xsl:value-of select="$href" />
      </xsl:attribute>

      <xsl:value-of select="$Text"/>
    </a>

  </xsl:template>


  <!-- <xsl:template name="ProcessFieldSeparate">
       Данный типлэйт обрабатывает поле(field) после команды Separate
  -->
  <xsl:template name="ProcessFieldSeparate">
    <xsl:param name="FieldLevel"/>
    <xsl:param name="CurNode"/>
    <xsl:param name="FieldType"/>
    <xsl:param name="FieldCode"/>

    <xsl:variable name="ListFieldAndText" select="($CurNode/following::w:fldChar[1] | $CurNode/following::w:instrText[1] | $CurNode/following::w:t[1] | $CurNode/following::w:tab[not(parent::w:tabs)][1] | $CurNode/following::w:p[1] | $CurNode/descendant::w:fldChar[1] | $CurNode/descendant::w:instrText[1] | $CurNode/descendant::w:t[1] | $CurNode/descendant::w:tab[not(parent::w:tabs)][1] | $CurNode/descendant::w:p[1])[1]" />
    <xsl:variable name="ListEndField" select="$CurNode/following::w:fldChar[@w:fldCharType = 'end'][1]" />

    <xsl:choose>
      <xsl:when test="count($ListFieldAndText) = 0 or count($ListEndField) = 0">
        <!-- Ничего не делаем, просто выходим из цикла таким образом -->
      </xsl:when>
      <xsl:when test="name($ListFieldAndText[1]) = 'w:p'">

        <!-- Обрабатываем начало нового парагафа только в самом верхнем поле -->
        <xsl:if test="$FieldLevel = 0">
          <!-- Закрываем текущий параграф и начинаем новый -->

          <!-- </p> -->
          <xsl:text disable-output-escaping="yes">&lt;/p&gt;</xsl:text>

          <!-- </div> -->
          <xsl:text disable-output-escaping="yes">&lt;/div&gt;</xsl:text>

          <!-- Ищем стили для текущего <div> -->
          <xsl:variable name="DivStyle">
            <!--<xsl:for-each select="$ListFieldAndText[1]/w:pPr">
              <xsl:call-template name="InsertDivProperties">
                <xsl:with-param name="CurNode" select="$ListFieldAndText[1]" />
              </xsl:call-template>
            </xsl:for-each>-->
          </xsl:variable>

          <!-- Ищем стили для текущего <p> -->
          <xsl:variable name="ParagraphStyle">
            <!--<xsl:for-each select="$ListFieldAndText[1]/w:pPr">
              <xsl:call-template name="InsertParagraphProperties">
                <xsl:with-param name="CurNode" select="$ListFieldAndText[1]" />
              </xsl:call-template>
            </xsl:for-each>-->
          </xsl:variable>

          <!-- <div> -->
          <xsl:text disable-output-escaping="yes">&lt;div</xsl:text>

          <xsl:if test="$DivStyle != ''">
            <xsl:text> style="</xsl:text>
            <xsl:value-of select="$DivStyle"/>
            <xsl:text>"</xsl:text>
          </xsl:if>

          <xsl:text disable-output-escaping="yes">&gt;</xsl:text>
                   
          <!-- <p> -->
          <xsl:text disable-output-escaping="yes">&lt;p</xsl:text>

          <xsl:if test="$ListFieldAndText[1]/w:pPr or $ListFieldAndText[1]/w:r/w:br[@w:type='page' or @w:type='column'] or key('Part', 'word/styles.xml')/w:styles/w:docDefaults/w:pPrDefault">
            <xsl:if test="$ListFieldAndText[1]/w:pPr/w:pStyle/@w:val or ( $ListFieldAndText[1]/w:r/w:ptab/@w:alignment = 'right' and $ListFieldAndText[1]/w:pPr/w:pStyle/@w:val = 'Footer' )">
              <xsl:text> class="</xsl:text>
              <xsl:choose>
                <xsl:when test="$ListFieldAndText[1]/w:r/w:ptab/@w:alignment = 'right' and $ListFieldAndText[1]/w:pPr/w:pStyle/@w:val = 'Footer'">
                  <xsl:text>X3AS7TABSTYLE</xsl:text>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:call-template name="InsertStyleNameInTeg">
                    <xsl:with-param name="styleName" select="$ListFieldAndText[1]/w:pPr/w:pStyle/@w:val"/>
                  </xsl:call-template>
                </xsl:otherwise>
              </xsl:choose>
              <xsl:text>"</xsl:text>
            </xsl:if>
          </xsl:if>

          <xsl:if test="$ParagraphStyle != ''">
            <xsl:text> style="</xsl:text>
            <xsl:value-of select="$ParagraphStyle"/>
            <xsl:text>"</xsl:text>
          </xsl:if>

          <xsl:text disable-output-escaping="yes">&gt;</xsl:text>
          
        </xsl:if>

        <!-- Переходим к следующему элементу -->
        <xsl:call-template name="ProcessFieldSeparate">
          <xsl:with-param name="FieldCode" select="$FieldCode"/>
          <xsl:with-param name="FieldType" select="$FieldType"/>
          <xsl:with-param name="FieldLevel" select="$FieldLevel" />
          <xsl:with-param name="CurNode" select="$ListFieldAndText[1]"/>
        </xsl:call-template>

      </xsl:when>
      <xsl:when test="name($ListFieldAndText[1]) = 'w:tab'">
        
        <!-- Табы обрабатываем только внутри своих уровней -->
        <!-- Сначала проверим относится ли данный текст к текущему полю -->
        <xsl:variable name="bCheckLvl">
          <xsl:choose>
            <xsl:when test="count($ListFieldAndText[1]/preceding::w:fldChar[@w:fldCharType = 'begin']) = ( count($ListFieldAndText[1]/preceding::w:fldChar[@w:fldCharType = 'end']) + $FieldLevel + 1 )">
              <xsl:text>true</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>false</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>

        <xsl:if test="$bCheckLvl = 'true'">
          <span style="mso-tab-count:1">&#160;&#160;</span>
        </xsl:if>
        
        <!-- Переходим к следующему элементу -->
        <xsl:call-template name="ProcessFieldSeparate">
          <xsl:with-param name="FieldCode" select="$FieldCode"/>
          <xsl:with-param name="FieldType" select="$FieldType"/>
          <xsl:with-param name="FieldLevel" select="$FieldLevel" />
          <xsl:with-param name="CurNode" select="$ListFieldAndText[1]"/>
        </xsl:call-template>
        
      </xsl:when>
      <xsl:when test="name($ListFieldAndText[1]) = 'w:fldChar' and $ListFieldAndText[1]/@w:fldCharType = 'begin'">

        <!-- Обрабатываем вложенное поле -->
        <xsl:call-template name="ProcessFieldBegin">
          <xsl:with-param name="FieldLevel" select="$FieldLevel + 1" />
          <xsl:with-param name="CurNode" select="$ListFieldAndText[1]" />
        </xsl:call-template>


        <!-- Переходим к следующему элементу -->
        <xsl:call-template name="ProcessFieldSeparate">
          <xsl:with-param name="FieldCode" select="$FieldCode"/>
          <xsl:with-param name="FieldType" select="$FieldType"/>
          <xsl:with-param name="FieldLevel" select="$FieldLevel" />
          <xsl:with-param name="CurNode" select="$ListFieldAndText[1]"/>
        </xsl:call-template>


      </xsl:when>
      <xsl:when test="(name($ListFieldAndText[1]) = 'w:instrText') or (name($ListFieldAndText[1]) = 'w:t')">

        <!-- Сначала проверим относится ли данный текст к текущему полю -->
        <xsl:variable name="bCheckLvl">
          <xsl:choose>
            <xsl:when test="count($ListFieldAndText[1]/preceding::w:fldChar[@w:fldCharType = 'begin']) = ( count($ListFieldAndText[1]/preceding::w:fldChar[@w:fldCharType = 'end']) + $FieldLevel + 1 )">
              <xsl:text>true</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>false</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>

        <xsl:choose>
          <xsl:when test="$bCheckLvl = 'true'">

            <xsl:variable name="Text" select="$ListFieldAndText[1]/text()"/>

            <!-- Ищем родительскую ноду w:r -->

            <xsl:variable name="ParentR" select="$CurNode/ancestor::w:r[last()]"/>

            <xsl:variable name="bStyle">
              <xsl:choose>
                <xsl:when test="$ParentR/w:rPr[not(count($ParentR/child::node())=1 and $ParentR/child::w:noProof)]">
                  <xsl:text>true</xsl:text>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:text>false</xsl:text>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>

            <xsl:if test="$bStyle = 'true'">

              <xsl:text disable-output-escaping="yes">&lt;span</xsl:text>
              <xsl:if test="$ParentR/w:rPr/w:rStyle">
                <xsl:text> class="</xsl:text>
                <xsl:call-template name="InsertStyleNameInTeg">
                  <xsl:with-param name="styleName" select="$ParentR/w:rPr/w:rStyle/@w:val"/>
                </xsl:call-template>
                <xsl:text>"</xsl:text>
              </xsl:if>
              <xsl:variable name="textStyle">
                <xsl:call-template name="InsertTextProperties">
                  <xsl:with-param name="CurNode" select="$ParentR" />
                </xsl:call-template>
              </xsl:variable>
              <xsl:if test="$textStyle != ''">
                <xsl:text disable-output-escaping="yes"> style=&quot;</xsl:text>
                <xsl:value-of select="$textStyle"/>
                <xsl:text disable-output-escaping="yes">&quot;</xsl:text>
              </xsl:if>
              <xsl:text disable-output-escaping="yes">&gt;</xsl:text>
            </xsl:if>

            <xsl:choose>

              <!-- TOC -->
              <xsl:when test="$FieldType = 'TOC'">

                <xsl:call-template name="DrawText">
                  <xsl:with-param name="Level" select="$FieldLevel + 1" />
                  <xsl:with-param name="CurNode" select="$ListFieldAndText[1]" />
                </xsl:call-template>

              </xsl:when>

              <!-- HYPERLINK -->
              <xsl:when test="$FieldType = 'HYPERLINK'">

                <xsl:call-template name="InsertHyperlinkField">
                  <xsl:with-param name="strCommand" select="$FieldCode"/>
                  <xsl:with-param name="CurNode" select="$CurNode" />
                  <xsl:with-param name="Text" select="$Text"/>
                </xsl:call-template>

              </xsl:when>

              <!-- PAGEREF -->
              <xsl:when test="$FieldType = 'PAGEREF'">

                <xsl:call-template name="InsertPageRefField">
                  <xsl:with-param name="Text" select="$Text"/>
                  <xsl:with-param name="strCommand" select="$FieldCode"/>
                  <xsl:with-param name="CurNode" select="$CurNode" />
                </xsl:call-template>

              </xsl:when>

            </xsl:choose>

            <xsl:if test="$bStyle = 'true'">
              <xsl:text disable-output-escaping="yes">&lt;/span&gt;</xsl:text>
            </xsl:if>

            <!-- Переходим к следующему элементу -->
            <xsl:call-template name="ProcessFieldSeparate">
              <xsl:with-param name="FieldCode" select="$FieldCode"/>
              <xsl:with-param name="FieldType" select="$FieldType"/>
              <xsl:with-param name="FieldLevel" select="$FieldLevel" />
              <xsl:with-param name="CurNode" select="$ListFieldAndText[1]"/>
            </xsl:call-template>

          </xsl:when>
          <xsl:otherwise>

            <!-- Переходим к следующему элементу -->
            <xsl:call-template name="ProcessFieldSeparate">
              <xsl:with-param name="FieldCode" select="$FieldCode"/>
              <xsl:with-param name="FieldType" select="$FieldType"/>
              <xsl:with-param name="FieldLevel" select="$FieldLevel" />
              <xsl:with-param name="CurNode" select="$ListFieldAndText[1]"/>
            </xsl:call-template>

          </xsl:otherwise>
        </xsl:choose>

      </xsl:when>
      <xsl:when test="($ListFieldAndText[1]/@w:fldCharType = 'end') and ($FieldLevel = (count($ListFieldAndText[1]/preceding::w:fldChar[@w:fldCharType = 'begin']) - count($ListFieldAndText[1]/preceding::w:fldChar[@w:fldCharType = 'end']) - 1 ) )" >
        <!-- Проверяем заканчивает ли даный 'end' поле текущего уровня. Если да, то здесь мы заканчиваем обработку текщуего поля -->
      </xsl:when>
      <xsl:otherwise>

        <!-- Переходим к следующему элементу -->
        <xsl:call-template name="ProcessFieldSeparate">
          <xsl:with-param name="FieldCode" select="$FieldCode"/>
          <xsl:with-param name="FieldType" select="$FieldType"/>
          <xsl:with-param name="FieldLevel" select="$FieldLevel" />
          <xsl:with-param name="CurNode" select="$ListFieldAndText[1]"/>
        </xsl:call-template>

      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>


  <xsl:template name="InsertField">
		<!--xsl:variable name="Gudule" select="preceding::*[1][self::w:fldChar[@w:fldCharType='begin']] "/-->

		<xsl:choose>
			<!-- workaround for autonum fields - inserting numbera as plain text -->
			<xsl:when test="child::w:instrText[contains(.,'AUTONUM')]">
				<!--
                <text:span text:style-name="{generate-id(self::node())}">
                    <xsl:value-of select="concat(count(preceding::w:instrText[contains(.,'AUTONUM')])+1,'.')"/>
                </text:span>
                -->
			</xsl:when>
			<!-- default scenario - catch beginning of field instruction. Other runs ignored (handled by first w:instrText processing). -->
			<xsl:when test="w:instrText/@oox:firstInstrText and not(contains(w:instrText[1],'AUTOTEXT') or contains(w:instrText[1],'AUTONUM'))">
				<xsl:apply-templates select="w:instrText[1]"/>
			</xsl:when>
			<!-- autotext fields should be processed like normal text, because there is no autotext field in OO -->
			<xsl:when test="w:instrText/@oox:firstInstrText and (contains(w:instrText[1],'AUTOTEXT') or contains(w:instrText[1],'AUTONUM'))">
				<xsl:choose>
					<xsl:when test="w:rPr[not(count(child::node())=1 and child::w:noProof)]">
						<!--
                        <text:span text:style-name="{generate-id(self::node())}">
                            <xsl:apply-templates/>
                        </text:span>
                        -->
					</xsl:when>
					<xsl:otherwise>
						<xsl:apply-templates/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:otherwise>
				<!--  the same hyperlink can be in more then one paragraph so print as seperate text:a for each run which is in hyperlink field (between w:fldChar begin - end)-->
				<xsl:choose>
					<!-- autotext fields should be processed like normal text, because there is no autotext field in OO -->
					<xsl:when test="preceding::w:instrText[1][contains(.,'AUTOTEXT') or contains(.,'AUTONUM')]">
						<xsl:choose>
							<xsl:when test="w:rPr[not(count(child::node())=1 and child::w:noProof)]">
								<!--
                                <text:span text:style-name="{generate-id(self::node())}">
                                    <xsl:apply-templates/>
                                </text:span>
                                -->
							</xsl:when>
							<xsl:otherwise>
								<xsl:apply-templates/>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:when>
					<!--<xsl:otherwise>
            <xsl:apply-templates select="preceding::w:instrText[1][contains(.,'HYPERLINK')]">
              <xsl:with-param name="parentRunNode" select="."/>
            </xsl:apply-templates>
          </xsl:otherwise>-->
				</xsl:choose>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="InsertEditingCycles">
		<!--
        <text:editing-cycles>
            <xsl:apply-templates select="w:r/child::node()"/>
        </text:editing-cycles>
        -->
	</xsl:template>

	<xsl:template name="InsertFileName">
		<!--
        <text:file-name text:display="name">
            <xsl:apply-templates select="w:r/child::node()"/>
        </text:file-name>
        -->
	</xsl:template>

	<xsl:template name="InsertKeywords">
		<!--
        <text:keywords>
            <xsl:apply-templates select="w:r/child::node()"/>
        </text:keywords>
        -->
	</xsl:template>

	<xsl:template name="InsertCompany">
		<!--
        <text:sender-company>
            <xsl:apply-templates select="w:r/child::node()"/>
        </text:sender-company>
        -->
	</xsl:template>

	<xsl:template name="InsertUserAddress">
		<!--
        <text:sender-street>
            <xsl:apply-templates select="w:r/child::node()"/>
        </text:sender-street>
        -->
	</xsl:template>

	<xsl:template name="InsertTemplate">
		<!--
        <text:template-name text:display="name">
            <xsl:apply-templates select="w:r/child::node()"/>
        </text:template-name>
        -->
	</xsl:template>

	<xsl:template name="InsertWordCount">
		<!--
        <text:word-count>
            <xsl:apply-templates select="w:r/child::node()"/>
        </text:word-count>
        -->
	</xsl:template>

	<xsl:template name="InsertCharacterCount">
		<!--
        <text:character-count>
            <xsl:apply-templates select="w:r/child::node()"/>
        </text:character-count>>
        -->
	</xsl:template>

	<xsl:template name="InsertParagraphCount">
		<!--
        <text:paragraph-count>
            <xsl:apply-templates select="w:r/child::node()"/>
        </text:paragraph-count>
        -->
	</xsl:template>

	<xsl:template name="InsertComments">
		<!--
        <text:description>
            <xsl:apply-templates select="w:r/child::node()"/>
        </text:description>
        -->
	</xsl:template>

  <!-- <xsl:template name="CheckFldSeparate"> 
       Данный типлэйт используется для проверки fldChar на наличие 'separate' и команды
       w:instrText, находящейся между 'begin' и 'separate' -->
  <xsl:template name="CheckFldSeparate">
    <xsl:param name="CurNode" select="."/>
    
    <!-- Смотрим все следущие поля -->
    <xsl:variable name="FldChars" select="$CurNode/following::w:fldChar"/>

    <xsl:choose>
      
      <xsl:when test="count($FldChars) = 0 or count($FldChars[@w:fldCharType = 'end']) = 0">
        <!-- Такого не должно быть -->
        <xsl:text>false</xsl:text>
      </xsl:when>
      <xsl:when test="$FldChars[1]/@w:fldCharType = 'separate'">
        <!-- Следующий fldChar имеет тип 'separate', проверим, есть ли до него какие-нибуль инструкции -->
        <xsl:variable name="FldAndInstr" select="following::w:fldChar[1] | following::w:instrText[1]"/>
        <xsl:choose>
          <xsl:when test="$FldAndInstr[1]/@w:fldCharType = 'separate'">
            <xsl:text>false</xsl:text>
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>true</xsl:text>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:when test="$FldChars[1]/@w:fldCharType = 'end'">
        <xsl:text>false</xsl:text>
      </xsl:when>
      <xsl:when test="$FldChars[1]/@w:fldCharType = 'begin'">
        <xsl:call-template name="CheckFldSeparate">
          <xsl:with-param name="CurNode" select="following::w:fldChar[@w:fldCharType = 'end'][1]"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <!-- Такого не должно быть -->
        <xsl:text>false</xsl:text>
      </xsl:otherwise>
    </xsl:choose>
    
    
  </xsl:template>

  <!-- <xsl:template name="FldInstrDeleteSwitches"> 
       Данный типлэйт используется для поиска в строке с командой подстроки без переключателей
       swithes.
  -->
  <xsl:template name="FldInstrDeleteSwitches">
    <xsl:param name="strInstr" />

    <xsl:choose>
      <xsl:when test="not(contains(normalize-space($strInstr), '\'))">
        <xsl:value-of select="normalize-space($strInstr)" />
      </xsl:when>

      <xsl:when test="contains(normalize-space($strInstr), ' ')">
        <xsl:choose>
          <xsl:when test="not(contains(substring-before($strInstr, ' '), '/'))">
            <xsl:value-of select="normalize-space(substring-before($strInstr, ' '))" />
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="FldInstrDeleteSwitches">
              <xsl:with-param name="strInstr" select="substring-after($strInstr, ' ')" />
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      
      <xsl:otherwise>
        <xsl:value-of select="normalize-space($strInstr)" />
      </xsl:otherwise>
    </xsl:choose>
    
  </xsl:template>



</xsl:stylesheet>
