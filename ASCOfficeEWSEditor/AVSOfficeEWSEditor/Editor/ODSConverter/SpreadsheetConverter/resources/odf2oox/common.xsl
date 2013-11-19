<?xml version="1.0" encoding="UTF-8"?>
<!--
  * Copyright (c) 2006, Clever Age
  * All rights reserved.
  * 
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions are met:
  *
  *     * Redistributions of source code must retain the above copyright
  *       notice, this list of conditions and the following disclaimer.
  *     * Redistributions in binary form must reproduce the above copyright
  *       notice, this list of conditions and the following disclaimer in the
  *       documentation and/or other materials provided with the distribution.
  *     * Neither the name of Clever Age nor the names of its contributors 
  *       may be used to endorse or promote products derived from this software
  *       without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
  * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
  * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
  xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main"
  xmlns:pzip="urn:cleverage:xmlns:post-processings:zip"
  xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0" exclude-result-prefixes="table">

  <!-- template to convert cell type -->
  <xsl:template name="ConvertTypes">
    <xsl:param name="type"/>
    <!-- TO DO percentage -->
    <xsl:choose>
      <xsl:when test="$type='float'">n</xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- converting number of column to char name of column -->
  <xsl:template name="NumbersToChars">
    <xsl:param name="num"/>
	  <xsl:variable name ="number">
		  <xsl:value-of select ="translate($num,'$','')"/>
	  </xsl:variable>
    <xsl:choose>
      <xsl:when test="$number = 0">A</xsl:when>
      <xsl:when test="$number = 1">B</xsl:when>
      <xsl:when test="$number = 2">C</xsl:when>
      <xsl:when test="$number = 3">D</xsl:when>
      <xsl:when test="$number = 4">E</xsl:when>
      <xsl:when test="$number = 5">F</xsl:when>
      <xsl:when test="$number = 6">G</xsl:when>
      <xsl:when test="$number = 7">H</xsl:when>
      <xsl:when test="$number = 8">I</xsl:when>
      <xsl:when test="$number = 9">J</xsl:when>
      <xsl:when test="$number = 10">K</xsl:when>
      <xsl:when test="$number = 11">L</xsl:when>
      <xsl:when test="$number = 12">M</xsl:when>
      <xsl:when test="$number = 13">N</xsl:when>
      <xsl:when test="$number = 14">O</xsl:when>
      <xsl:when test="$number = 15">P</xsl:when>
      <xsl:when test="$number = 16">Q</xsl:when>
      <xsl:when test="$number = 17">R</xsl:when>
      <xsl:when test="$number = 18">S</xsl:when>
      <xsl:when test="$number = 19">T</xsl:when>
      <xsl:when test="$number = 20">U</xsl:when>
      <xsl:when test="$number = 21">V</xsl:when>
      <xsl:when test="$number = 22">W</xsl:when>
      <xsl:when test="$number = 23">X</xsl:when>
      <xsl:when test="$number = 24">Y</xsl:when>
      <xsl:when test="$number = 25">Z</xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="NumbersToChars">
          <xsl:with-param name="num">
			  <!--<xsl:value-of select="floor($num div 26)-1"/>-->
            <xsl:value-of select="floor($number div 26)-1"/>
          </xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="NumbersToChars">
          <xsl:with-param name="num">
			  <!--<xsl:value-of select="$num - 26*floor($num div 26)"/>-->
            <xsl:value-of select="$num - 26*floor($number div 26)"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- calculates power function -->
  <xsl:template name="Power">
    <xsl:param name="base"/>
    <xsl:param name="exponent"/>
    <xsl:param name="value1" select="$base"/>

    <xsl:choose>
      <xsl:when test="$exponent = 0">
        <xsl:text>1</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:choose>
          <xsl:when test="$exponent &gt; 1">
            <xsl:call-template name="Power">
              <xsl:with-param name="base">
                <xsl:value-of select="$base"/>
              </xsl:with-param>
              <xsl:with-param name="exponent">
                <xsl:value-of select="$exponent -1"/>
              </xsl:with-param>
              <xsl:with-param name="value1">
                <xsl:value-of select="$value1 * $base"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$value1"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- translates literal index to number -->
  <xsl:template name="GetAlphabeticPosition">
    <xsl:param name="literal"/>
    <xsl:param name="number" select="0"/>
    <xsl:param name="level" select="0"/>

    <xsl:variable name="lastCharacter">
      <xsl:value-of select="substring($literal,string-length($literal),1)"/>
    </xsl:variable>

    <xsl:variable name="lastCharacterPosition">
      <xsl:call-template name="CharacterToPosition">
        <xsl:with-param name="character" select="$lastCharacter"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="power">
      <xsl:call-template name="Power">
        <xsl:with-param name="base" select="26"/>
        <xsl:with-param name="exponent" select="$level"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="string-length($literal)>1">
        <xsl:call-template name="GetAlphabeticPosition">
          <xsl:with-param name="literal" select="substring($literal,0,string-length($literal))"/>
          <xsl:with-param name="level" select="$level+1"/>
          <xsl:with-param name="number">
            <xsl:value-of select="$lastCharacterPosition*$power + $number"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$lastCharacterPosition*$power + $number"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- returns position in alphabet of a single character-->
  <xsl:template name="CharacterToPosition">
    <xsl:param name="character"/>

    <xsl:choose>
      <xsl:when test="$character='A'">1</xsl:when>
      <xsl:when test="$character='B'">2</xsl:when>
      <xsl:when test="$character='C'">3</xsl:when>
      <xsl:when test="$character='D'">4</xsl:when>
      <xsl:when test="$character='E'">5</xsl:when>
      <xsl:when test="$character='F'">6</xsl:when>
      <xsl:when test="$character='G'">7</xsl:when>
      <xsl:when test="$character='H'">8</xsl:when>
      <xsl:when test="$character='I'">9</xsl:when>
      <xsl:when test="$character='J'">10</xsl:when>
      <xsl:when test="$character='K'">11</xsl:when>
      <xsl:when test="$character='L'">12</xsl:when>
      <xsl:when test="$character='M'">13</xsl:when>
      <xsl:when test="$character='N'">14</xsl:when>
      <xsl:when test="$character='O'">15</xsl:when>
      <xsl:when test="$character='P'">16</xsl:when>
      <xsl:when test="$character='Q'">17</xsl:when>
      <xsl:when test="$character='R'">18</xsl:when>
      <xsl:when test="$character='S'">19</xsl:when>
      <xsl:when test="$character='T'">20</xsl:when>
      <xsl:when test="$character='U'">21</xsl:when>
      <xsl:when test="$character='V'">22</xsl:when>
      <xsl:when test="$character='W'">23</xsl:when>
      <xsl:when test="$character='X'">24</xsl:when>
      <xsl:when test="$character='Y'">25</xsl:when>
      <xsl:when test="$character='Z'">26</xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- template to convert column width -->
<!-- Change By : Vijayeta
     Changes   : Additional condition 'check' added, cos in case of col width,
                when the calculation is rounded, exact calclation is lost and 
                col wid is found to be lesser than expected
     File       : Excel_SmokeTest_InputFile..xlsx->sp2->Excel_SmokeTest_InputFile..ods->Translator->Excel_SmokeTest_InputFile..xlsx 

  --> 
  <xsl:template name="ConvertToCharacters">
    <xsl:param name="width"/>
    <xsl:param name="defaultFontSize"/>
    <xsl:variable name="pixelWidth">
      <xsl:call-template name="pixel-measure">
        <xsl:with-param name="length">
          <xsl:value-of select="$width"/>
        </xsl:with-param>
		<xsl:with-param name="check" select ="1"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="fontSize">
      <xsl:call-template name="pixel-measure">
        <xsl:with-param name="length">
          <xsl:value-of select="concat($defaultFontSize,'pt')"/>
        </xsl:with-param>
		<xsl:with-param name="check" select ="1"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="realFontSize">
      <xsl:value-of select="72 * $fontSize div 96"/>
    </xsl:variable>
    <xsl:value-of select="($pixelWidth) div (2 div 3 * $realFontSize)"/>
  </xsl:template>

  <!-- Template to check if value is hexadecimal -->
  <xsl:template name="CheckIfHexadecimal">
    <xsl:param name="value"/>
    <xsl:param name="result"/>

    <xsl:variable name="char">
      <xsl:value-of select="substring($value, 1, 1)"/>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="$value != ''">
        <xsl:choose>
          <xsl:when
            test="$char = '0' or number($char) or $char = 'A' or $char = 'a' or $char = 'B'  or $char = 'b' or $char = 'c' or $char = 'C' or $char = 'd' or $char = 'D' or $char = 'e' or $char = 'E' or $char = 'f' or $char = 'F'">
            <xsl:call-template name="CheckIfHexadecimal">
              <xsl:with-param name="value">
                <xsl:value-of select="substring($value, 2)"/>
              </xsl:with-param>
              <xsl:with-param name="result">
                <xsl:text>true</xsl:text>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <xsl:choose>
              <xsl:when test="$result != ''">
                <xsl:value-of select="$result"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:text>false</xsl:text>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:otherwise>
        <xsl:choose>
          <xsl:when test="$result = 'true'">
            <xsl:text>true</xsl:text>
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>false</xsl:text>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="GetRowNum">
    <xsl:param name="cell"/>
    <xsl:choose>
      <xsl:when test="number($cell)">
        <xsl:value-of select="$cell"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="GetRowNum">
          <xsl:with-param name="cell" select="substring-after($cell,substring($cell,1,1))"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>


  <!-- gets a column number from cell coordinates -->
  <xsl:template name="GetColNum">
    <xsl:param name="cell"/>
    <xsl:param name="columnId"/>
	  <xsl:variable name ="tempCell">
		  <xsl:value-of select ="translate($cell,'$','')"/>
	  </xsl:variable>
    <xsl:choose>
      <!-- when whole literal column id has been extracted than convert alphabetic index to number -->
      <xsl:when test="number($tempCell)">
        <xsl:call-template name="GetAlphabeticPosition">
          <xsl:with-param name="literal" select="$columnId"/>
        </xsl:call-template>
      </xsl:when>
      <!--  recursively extract literal column id (i.e if $cell='GH15' it will return 'GH') -->
      <xsl:otherwise>
        <xsl:call-template name="GetColNum">
          <xsl:with-param name="cell" select="substring-after($cell,substring($tempCell,1,1))"/>
          <xsl:with-param name="columnId" select="concat($columnId,substring($tempCell,1,1))"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- template which converts date to number -->
  <!-- date format can be yyyy-mm-dd or yyyy-mm-ddThh:mm:ss.uu -->
  <xsl:template name="DateToNumber">
    <xsl:param name="value"/>
    <xsl:variable name="year">
      <xsl:value-of select="substring-before($value,'-')"/>
    </xsl:variable>
    <xsl:variable name="month">
      <xsl:value-of select="substring-before(substring-after($value,'-'),'-')"/>
    </xsl:variable>
    <xsl:variable name="day">
      <xsl:choose>
        <xsl:when test="contains($value,'T')">
          <xsl:value-of
            select="substring-before(substring-after(substring-after($value,'-'),'-'),'T')"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="substring-after(substring-after($value,'-'),'-')"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="hour">
      <xsl:value-of select="substring-before(substring-after($value,'T'),':')"/>
    </xsl:variable>
    <xsl:variable name="minutes">
      <xsl:value-of select="substring-before(substring-after($value,':'),':')"/>
    </xsl:variable>
    <xsl:variable name="seconds">
      <xsl:value-of select="substring-after(substring-after($value,':'),':')"/>
    </xsl:variable>

    <!-- number of leap years between given year and 1900 -->
    <xsl:variable name="leapYears">
      <xsl:value-of
        select="floor(($year -1901) div 4) - floor(($year - 1901) div 100) + floor(($year - 1601) div 400)"
      />
    </xsl:variable>

    <!-- get number of days before given month -->
    <xsl:variable name="daysOfMonths">
      <xsl:call-template name="MonthsToDays">
        <xsl:with-param name="month">
          <xsl:value-of select="number($month)"/>
        </xsl:with-param>
        <xsl:with-param name="isYearLeap">
          <xsl:choose>
            <xsl:when
              test="(($year - 4 * floor($year div 4)) = 0 and ($year - 100 * floor($year div 100)) != 0) or ($year - 400 * floor($year div 400)) = 0"
              >true</xsl:when>
            <xsl:otherwise>false</xsl:otherwise>
          </xsl:choose>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="realValue">
      <xsl:choose>
        <xsl:when test="contains($value,'T')">
          <xsl:value-of
            select="number($day) + $daysOfMonths + 365 * ($year - 1900) + $leapYears + 1 + $hour div 24 + $minutes div 1440 + $seconds div 86400"
          />
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of
            select="number($day) + $daysOfMonths + 365 * ($year - 1900) + $leapYears + 1"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <!-- modification due to Excel date bug(leap year 1900) -->
    <xsl:choose>
      <xsl:when test="$realValue &lt; 61 and $realValue &gt;= 2">
        <xsl:value-of select="$realValue -1"/>
      </xsl:when>
	  <!--
	      Defect Id :1873854
		  Date      :5th June '08
		  Changes by:Shabeer		  
	      Desc      : Condition included to check dates less than 1/1/1900 and converting it to the base date (1/1/1900)-->
      <xsl:when test="$realValue &lt; 1">
		  <xsl:message terminate="no">translation.odf2oox.baseDate</xsl:message>
        <xsl:value-of select="'1'"/>       
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$realValue"/>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <!-- template which converts time to number -->

  <xsl:template name="TimeToNumber">
    <xsl:param name="value"/>
    <xsl:variable name="hour">
      <xsl:value-of select="substring-after(substring-before($value,'H'),'T')"/>
    </xsl:variable>
    <xsl:variable name="minutes">
      <xsl:value-of select="substring-before(substring-after($value,'H'),'M')"/>
    </xsl:variable>
    <xsl:variable name="seconds">
      <xsl:value-of select="substring-before(substring-after($value,'M'),'S')"/>
    </xsl:variable>
    <xsl:value-of select="$hour div 24 + $minutes div 1440 + $seconds div 86400"/>
  </xsl:template>

  <xsl:template name="MonthsToDays">
    <xsl:param name="month"/>
    <xsl:param name="isYearLeap"/>
    <xsl:variable name="extraDay">
      <xsl:choose>

        <!-- if there's a leap year than add extra day after 2nd month -->
        <xsl:when test="$isYearLeap = 'true' and $month &gt; 2">1</xsl:when>
        <xsl:otherwise>0</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="numberOfDays">
      <xsl:choose>
        <xsl:when test="$month = 1">0</xsl:when>
        <xsl:when test="$month = 2">31</xsl:when>
        <xsl:when test="$month = 3">59</xsl:when>
        <xsl:when test="$month = 4">90</xsl:when>
        <xsl:when test="$month = 5">120</xsl:when>
        <xsl:when test="$month = 6">151</xsl:when>
        <xsl:when test="$month = 7">181</xsl:when>
        <xsl:when test="$month = 8">212</xsl:when>
        <xsl:when test="$month = 9">243</xsl:when>
        <xsl:when test="$month = 10">273</xsl:when>
        <xsl:when test="$month = 11">304</xsl:when>
        <xsl:when test="$month = 12">334</xsl:when>
      </xsl:choose>
    </xsl:variable>
    <xsl:value-of select="$numberOfDays + $extraDay"/>
  </xsl:template>
  
  
  <!-- template to convert relative cell address into absolute address -->
  <xsl:template name="rel2Abs">
    <xsl:param name="relatCellAddress"/>
    <xsl:text>$</xsl:text>
    <xsl:call-template name="auxiliary">
      <xsl:with-param name="cellAddressRelative">
        <xsl:value-of select="$relatCellAddress"/>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>  
  
  <!-- auxiliary template for absoluteCellAddress template -->
  <xsl:template name="auxiliary">
    <xsl:param name="cellAddressRelative"/>
    <xsl:choose>
      <xsl:when test="substring($cellAddressRelative, 1, 1) != '0' and substring($cellAddressRelative, 1, 1) != '1' and substring($cellAddressRelative, 1, 1) != '2' and substring($cellAddressRelative, 1, 1) != '3' and substring($cellAddressRelative, 1, 1) != '4' and substring($cellAddressRelative, 1, 1) != '5' and substring($cellAddressRelative, 1, 1) != '6' and substring($cellAddressRelative, 1, 1) != '7' and substring($cellAddressRelative, 1, 1) != '8' and substring($cellAddressRelative, 1, 1) != '9'">
        <xsl:value-of select="substring($cellAddressRelative, 1, 1)"/>
        <xsl:call-template name="auxiliary">
          <xsl:with-param name="cellAddressRelative">
            <xsl:value-of select="substring($cellAddressRelative, 2)"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:text>$</xsl:text>
        <xsl:value-of select="$cellAddressRelative"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  
  <!-- template which takes column name and returns columns number, e.g. column EJ is 140th column -->
<xsl:template name="returnColumnNumber">
  <xsl:param name="columnName"/>
  <xsl:param name="currentValue">
    <xsl:value-of select="number(0)"/>
  </xsl:param>
  <xsl:choose>
    <xsl:when test="string-length($columnName) &gt; 0">
      <xsl:variable name="charValue">
        <xsl:call-template name="CharacterToPosition">
          <xsl:with-param name="character">
            <xsl:value-of select="substring($columnName, 1, 1)"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:variable>
      <xsl:variable name="poweredValue">
        <xsl:call-template name="Power">
          <xsl:with-param name="base">
            <xsl:value-of select="number(26)"/>
          </xsl:with-param>
          <xsl:with-param name="exponent">
            <xsl:value-of select="string-length($columnName) -1 "/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:variable>
      <xsl:variable name="temporary">
        <xsl:value-of select="$poweredValue * $charValue + $currentValue"/>
      </xsl:variable>
      <xsl:call-template name="returnColumnNumber">
        <xsl:with-param name="columnName">
          <xsl:value-of select="substring($columnName, 2)"/>
        </xsl:with-param>
        <xsl:with-param name="currentValue">
          <xsl:value-of select="$temporary"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:when>
    <xsl:otherwise>
      <xsl:value-of select="number($currentValue)"/>
    </xsl:otherwise>
  </xsl:choose>
  
</xsl:template>
	<!--
	Defect: 1894250
	Fixed By: Vijayeta
	Desc : Performance related defect, where in the pivot range of the source table(table:cell-range-address) in input is row num 65536,
	       where as the source table spans only upto 1089 rows. Hence, the code iterates 65536 times, and resulting in long conversion time.
		   Here this part of code takes a count of rows in source table and compares it with the value of attribute table:cell-range-address.
    Date: 10th Sep '08	
	-->
	<xsl:template name ="getTableRowCount">
		<xsl:param name="sheetName"/>
		<xsl:param name="cellAddress"/>
		<xsl:variable name ="rowCount">
			<xsl:for-each select ="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:table[@table:name=$sheetName]">
				<xsl:value-of select ="count(table:table-row)"/>
			</xsl:for-each>
		</xsl:variable>
		<xsl:variable name ="countRowsRepeated">
			<xsl:for-each select ="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:table[@table:name=$sheetName]">
				<xsl:value-of select ="count(table:table-row[@table:number-rows-repeated])"/>
			</xsl:for-each>
		</xsl:variable>
		<xsl:variable name ="sumRowsRepeated">
			<xsl:for-each select ="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:table[@table:name=$sheetName]">
				<xsl:value-of select ="sum(table:table-row/@table:number-rows-repeated)"/>
			</xsl:for-each>
		</xsl:variable>
		<xsl:value-of select ="$rowCount + $sumRowsRepeated - $countRowsRepeated"/>
	</xsl:template>	
</xsl:stylesheet>
