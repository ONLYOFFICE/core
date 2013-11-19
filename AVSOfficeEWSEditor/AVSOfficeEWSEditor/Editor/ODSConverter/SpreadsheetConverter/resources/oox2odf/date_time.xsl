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
  <xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
      xmlns:v="urn:schemas-microsoft-com:vml" xmlns:xlink="http://www.w3.org/1999/xlink"
      xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
      xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
      xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
      xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
      xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
      xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
      xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
      xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
      xmlns:e="http://schemas.openxmlformats.org/spreadsheetml/2006/main" exclude-result-prefixes="e r v">
      
    <xsl:template name="FormatDate">

    <!-- @Descripition: inserts date to cell in a correct format -->
    <!-- @Context: None -->

    <xsl:param name="value"/>
    <!-- (dateTime) input date value -->
    <xsl:param name="format"/>
    <!-- (string) format code -->
    <xsl:param name="numId"/>
    <!-- (int) format ID -->
    <xsl:param name="processedFormat"/>
    <!-- (string) part of format code which is being processed -->
    <xsl:param name="outputValue"/>
    <!-- (dateTime) output date value -->
    <xsl:param name="numValue"/>
    <!-- (float) date value as a number  -->
    <xsl:choose>

      <!-- year -->
      <xsl:when test="starts-with($processedFormat,'y')">
        <xsl:choose>
          <xsl:when test="starts-with(substring-after($processedFormat,'y'),'yyy')">
            <xsl:call-template name="FormatDate">
              <xsl:with-param name="value" select="$value"/>
              <xsl:with-param name="format" select="$format"/>
              <xsl:with-param name="numId" select="$numId"/>
              <xsl:with-param name="processedFormat">
                <xsl:value-of select="substring-after($processedFormat,'yyyy')"/>
              </xsl:with-param>
              <xsl:with-param name="outputValue">
                <xsl:value-of select="concat($outputValue,substring-before($value,'-'))"/>
              </xsl:with-param>
              <xsl:with-param name="numValue" select="$numValue"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="FormatDate">
              <xsl:with-param name="value" select="$value"/>
              <xsl:with-param name="format" select="$format"/>
              <xsl:with-param name="numId" select="$numId"/>
              <xsl:with-param name="processedFormat">
                <xsl:value-of select="substring-after($processedFormat,'yy')"/>
              </xsl:with-param>
              <xsl:with-param name="outputValue">
                <xsl:value-of
                  select="concat($outputValue,substring(substring-before($value,'-'),3))"/>
              </xsl:with-param>
              <xsl:with-param name="numValue" select="$numValue"/>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>

      <xsl:when test="starts-with($processedFormat,'m')">
        <xsl:choose>

          <!-- minutes -->
          <xsl:when test="contains(substring-before($format,'m'),'h:')">
            <xsl:choose>
              <xsl:when test="starts-with(substring-after($processedFormat,'m'),'m')">
                <xsl:call-template name="FormatDate">
                  <xsl:with-param name="value" select="$value"/>
                  <xsl:with-param name="format" select="$format"/>
                  <xsl:with-param name="numId" select="$numId"/>
                  <xsl:with-param name="processedFormat">
                    <xsl:value-of select="substring-after($processedFormat,'mm')"/>
                  </xsl:with-param>
                  <xsl:with-param name="outputValue">
                    <xsl:value-of
                      select="concat($outputValue,substring-before(substring-after(substring-after($value,'T'),':'),':'))"
                    />
                  </xsl:with-param>
                  <xsl:with-param name="numValue" select="$numValue"/>
                </xsl:call-template>
              </xsl:when>
              <xsl:otherwise>
                <xsl:call-template name="FormatDate">
                  <xsl:with-param name="value" select="$value"/>
                  <xsl:with-param name="format" select="$format"/>
                  <xsl:with-param name="numId" select="$numId"/>
                  <xsl:with-param name="processedFormat">
                    <xsl:value-of select="substring-after($processedFormat,'m')"/>
                  </xsl:with-param>
                  <xsl:with-param name="outputValue">
                    <xsl:value-of
                      select="concat($outputValue,number(substring-before(substring-after(substring-after($value,'T'),':'),':')))"
                    />
                  </xsl:with-param>
                  <xsl:with-param name="numValue" select="$numValue"/>
                </xsl:call-template>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:when>

          <!-- month -->
          <xsl:otherwise>
            <xsl:choose>
              <xsl:when test="starts-with(substring-after($processedFormat,'m'),'mmm')">
                <xsl:call-template name="FormatDate">
                  <xsl:with-param name="value" select="$value"/>
                  <xsl:with-param name="format" select="$format"/>
                  <xsl:with-param name="numId" select="$numId"/>
                  <xsl:with-param name="processedFormat">
                    <xsl:value-of select="substring-after($processedFormat,'mmmm')"/>
                  </xsl:with-param>
                  <xsl:with-param name="outputValue">
                    <xsl:variable name="monthName">
                      <xsl:call-template name="ConvertMonthToName">
                        <xsl:with-param name="month">
                          <xsl:value-of select="substring-before(substring-after($value,'-'),'-')"/>
                        </xsl:with-param>
                      </xsl:call-template>
                    </xsl:variable>
                    <xsl:value-of select="concat($outputValue,$monthName)"/>
                  </xsl:with-param>
                  <xsl:with-param name="numValue" select="$numValue"/>
                </xsl:call-template>
              </xsl:when>
              <xsl:when test="starts-with(substring-after($processedFormat,'m'),'mm')">
                <xsl:call-template name="FormatDate">
                  <xsl:with-param name="value" select="$value"/>
                  <xsl:with-param name="format" select="$format"/>
                  <xsl:with-param name="numId" select="$numId"/>
                  <xsl:with-param name="processedFormat">
                    <xsl:value-of select="substring-after($processedFormat,'mmm')"/>
                  </xsl:with-param>
                  <xsl:with-param name="outputValue">
                    <xsl:variable name="monthName">
                      <xsl:call-template name="ConvertMonthToShortName">
                        <xsl:with-param name="month">
                          <xsl:value-of select="substring-before(substring-after($value,'-'),'-')"/>
                        </xsl:with-param>
                      </xsl:call-template>
                    </xsl:variable>
                    <xsl:value-of select="concat($outputValue,$monthName)"/>
                  </xsl:with-param>
                  <xsl:with-param name="numValue" select="$numValue"/>
                </xsl:call-template>
              </xsl:when>
              <xsl:when test="starts-with(substring-after($processedFormat,'m'),'m')">
                <xsl:call-template name="FormatDate">
                  <xsl:with-param name="value" select="$value"/>
                  <xsl:with-param name="format" select="$format"/>
                  <xsl:with-param name="numId" select="$numId"/>
                  <xsl:with-param name="processedFormat">
                    <xsl:value-of select="substring-after($processedFormat,'mm')"/>
                  </xsl:with-param>
                  <xsl:with-param name="outputValue">
                    <xsl:value-of
                      select="concat($outputValue,substring-before(substring-after($value,'-'),'-'))"
                    />
                  </xsl:with-param>
                  <xsl:with-param name="numValue" select="$numValue"/>
                </xsl:call-template>
              </xsl:when>
              <xsl:otherwise>
                <xsl:call-template name="FormatDate">
                  <xsl:with-param name="value" select="$value"/>
                  <xsl:with-param name="format" select="$format"/>
                  <xsl:with-param name="numId" select="$numId"/>
                  <xsl:with-param name="processedFormat">
                    <xsl:value-of select="substring-after($processedFormat,'m')"/>
                  </xsl:with-param>
                  <xsl:with-param name="outputValue">
                    <xsl:value-of
                      select="concat($outputValue,number(substring-before(substring-after($value,'-'),'-')))"
                    />
                  </xsl:with-param>
                  <xsl:with-param name="numValue" select="$numValue"/>
                </xsl:call-template>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>

      <!-- day -->
      <xsl:when test="starts-with($processedFormat,'d')">
        <xsl:choose>
          <xsl:when test="starts-with(substring-after($processedFormat,'d'),'ddd')">
            <xsl:call-template name="FormatDate">
              <xsl:with-param name="value" select="$value"/>
              <xsl:with-param name="format" select="$format"/>
              <xsl:with-param name="numId" select="$numId"/>
              <xsl:with-param name="processedFormat">
                <xsl:value-of select="substring-after($processedFormat,'dddd')"/>
              </xsl:with-param>
              <xsl:with-param name="outputValue">
                <xsl:variable name="dayOfWeek">
                  <xsl:call-template name="ConvertDayToName">
                    <xsl:with-param name="day">
                      <xsl:value-of select="$numValue"/>
                    </xsl:with-param>
                  </xsl:call-template>
                </xsl:variable>
                <xsl:value-of select="concat($outputValue,$dayOfWeek)"/>
              </xsl:with-param>
              <xsl:with-param name="numValue" select="$numValue"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:when test="starts-with(substring-after($processedFormat,'d'),'dd')">
            <xsl:call-template name="FormatDate">
              <xsl:with-param name="value" select="$value"/>
              <xsl:with-param name="format" select="$format"/>
              <xsl:with-param name="numId" select="$numId"/>
              <xsl:with-param name="processedFormat">
                <xsl:value-of select="substring-after($processedFormat,'dddd')"/>
              </xsl:with-param>
              <xsl:with-param name="outputValue">
                <xsl:variable name="dayOfWeek">
                  <xsl:call-template name="ConvertDayToShortName">
                    <xsl:with-param name="day">
                      <xsl:value-of select="$numValue"/>
                    </xsl:with-param>
                  </xsl:call-template>
                </xsl:variable>
                <xsl:value-of select="concat($outputValue,$dayOfWeek)"/>
              </xsl:with-param>
              <xsl:with-param name="numValue" select="$numValue"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:when test="starts-with(substring-after($processedFormat,'d'),'d')">
            <xsl:call-template name="FormatDate">
              <xsl:with-param name="value" select="$value"/>
              <xsl:with-param name="format" select="$format"/>
              <xsl:with-param name="numId" select="$numId"/>
              <xsl:with-param name="processedFormat">
                <xsl:value-of select="substring-after($processedFormat,'dd')"/>
              </xsl:with-param>
              <xsl:with-param name="outputValue">
                <xsl:value-of
                  select="concat($outputValue,substring-before(substring-after(substring-after($value,'-'),'-'),'T'))"
                />
              </xsl:with-param>
              <xsl:with-param name="numValue" select="$numValue"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="FormatDate">
              <xsl:with-param name="value" select="$value"/>
              <xsl:with-param name="format" select="$format"/>
              <xsl:with-param name="numId" select="$numId"/>
              <xsl:with-param name="processedFormat">
                <xsl:value-of select="substring-after($processedFormat,'d')"/>
              </xsl:with-param>
              <xsl:with-param name="outputValue">
                <xsl:value-of
                  select="concat($outputValue,number(substring-before(substring-after(substring-after($value,'-'),'-'),'T')))"
                />
              </xsl:with-param>
              <xsl:with-param name="numValue" select="$numValue"/>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>

      <!-- hours -->
      <xsl:when test="starts-with($processedFormat,'h')">
        <xsl:choose>
          <xsl:when test="starts-with(substring-after($processedFormat,'h'),'h')">
            <xsl:call-template name="FormatDate">
              <xsl:with-param name="value" select="$value"/>
              <xsl:with-param name="format" select="$format"/>
              <xsl:with-param name="numId" select="$numId"/>
              <xsl:with-param name="processedFormat">
                <xsl:value-of select="substring-after($processedFormat,'hh')"/>
              </xsl:with-param>
              <xsl:with-param name="outputValue">
                <xsl:value-of
                  select="concat($outputValue,substring-before(substring-after($value,'T'),':'))"/>
              </xsl:with-param>
              <xsl:with-param name="numValue" select="$numValue"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="FormatDate">
              <xsl:with-param name="value" select="$value"/>
              <xsl:with-param name="format" select="$format"/>
              <xsl:with-param name="numId" select="$numId"/>
              <xsl:with-param name="processedFormat">
                <xsl:value-of select="substring-after($processedFormat,'h')"/>
              </xsl:with-param>
              <xsl:with-param name="outputValue">
                <xsl:value-of
                  select="concat($outputValue,number(substring-before(substring-after($value,'T'),':')))"
                />
              </xsl:with-param>
              <xsl:with-param name="numValue" select="$numValue"/>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>

      <!-- seconds -->
      <xsl:when test="starts-with($processedFormat,'s')">
        <xsl:choose>
          <xsl:when test="starts-with(substring-after($processedFormat,'s'),'s')">
            <xsl:call-template name="FormatDate">
              <xsl:with-param name="value" select="$value"/>
              <xsl:with-param name="format" select="$format"/>
              <xsl:with-param name="numId" select="$numId"/>
              <xsl:with-param name="processedFormat">
                <xsl:value-of select="substring-after($processedFormat,'ss')"/>
              </xsl:with-param>
              <xsl:with-param name="outputValue">
                <xsl:value-of
                  select="concat($outputValue,substring-after(substring-after(substring-after($value,'T'),':'),':'))"
                />
              </xsl:with-param>
              <xsl:with-param name="numValue" select="$numValue"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="FormatDate">
              <xsl:with-param name="value" select="$value"/>
              <xsl:with-param name="format" select="$format"/>
              <xsl:with-param name="numId" select="$numId"/>
              <xsl:with-param name="processedFormat">
                <xsl:value-of select="substring-after($processedFormat,'s')"/>
              </xsl:with-param>
              <xsl:with-param name="outputValue">
                <xsl:value-of
                  select="concat($outputValue,number(substring-after(substring-after(substring-after($value,'T'),':'),':')))"
                />
              </xsl:with-param>
              <xsl:with-param name="numValue" select="$numValue"/>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>

      <xsl:when
        test="starts-with($processedFormat,'\') or starts-with($processedFormat,'@') or starts-with($processedFormat,';')">
        <xsl:call-template name="FormatDate">
          <xsl:with-param name="value" select="$value"/>
          <xsl:with-param name="format" select="$format"/>
          <xsl:with-param name="numId" select="$numId"/>
          <xsl:with-param name="processedFormat">
            <xsl:value-of select="substring($processedFormat,2)"/>
          </xsl:with-param>
          <xsl:with-param name="outputValue" select="$outputValue"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="string-length($processedFormat) = 0">
        <xsl:value-of select="$outputValue"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="FormatDate">
          <xsl:with-param name="value" select="$value"/>
          <xsl:with-param name="format" select="$format"/>
          <xsl:with-param name="numId" select="$numId"/>
          <xsl:with-param name="processedFormat">
            <xsl:value-of select="substring($processedFormat,2)"/>
          </xsl:with-param>
          <xsl:with-param name="outputValue">
            <xsl:value-of select="concat($outputValue,substring($processedFormat,0,2))"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="FormatTime">

    <!-- @Descripition: inserts time to cell in a correct format -->
    <!-- @Context: None -->

    <xsl:param name="value"/>
    <!-- (time) input time value -->
    <xsl:param name="format"/>
    <!-- (string) format code -->
    <xsl:param name="numId"/>
    <!-- (int) format ID -->
    <xsl:param name="processedFormat"/>
    <!-- (string) part of format code which is being processed -->
    <xsl:param name="outputValue"/>
    <!-- (time) output time value -->
    <xsl:choose>
      <xsl:when test="starts-with($processedFormat,'h')">
        <xsl:choose>
          <xsl:when test="starts-with(substring-after($processedFormat,'h'),'h')">
            <xsl:call-template name="FormatTime">
              <xsl:with-param name="value" select="$value"/>
              <xsl:with-param name="format" select="$format"/>
              <xsl:with-param name="numId" select="$numId"/>
              <xsl:with-param name="processedFormat">
                <xsl:value-of select="substring-after($processedFormat,'hh')"/>
              </xsl:with-param>
              <xsl:with-param name="outputValue">
                <xsl:value-of
                  select="concat($outputValue,substring-before(substring-after($value,'PT'),'H'))"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="FormatTime">
              <xsl:with-param name="value" select="$value"/>
              <xsl:with-param name="format" select="$format"/>
              <xsl:with-param name="numId" select="$numId"/>
              <xsl:with-param name="processedFormat">
                <xsl:value-of select="substring-after($processedFormat,'h')"/>
              </xsl:with-param>
              <xsl:with-param name="outputValue">
                <xsl:value-of
                  select="concat($outputValue,number(substring-before(substring-after($value,'PT'),'H')))"
                />
              </xsl:with-param>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:when test="starts-with($processedFormat,'m')">
        <xsl:choose>
          <xsl:when test="starts-with(substring-after($processedFormat,'m'),'m')">
            <xsl:call-template name="FormatTime">
              <xsl:with-param name="value" select="$value"/>
              <xsl:with-param name="format" select="$format"/>
              <xsl:with-param name="numId" select="$numId"/>
              <xsl:with-param name="processedFormat">
                <xsl:value-of select="substring-after($processedFormat,'mm')"/>
              </xsl:with-param>
              <xsl:with-param name="outputValue">
                <xsl:value-of
                  select="concat($outputValue,substring-before(substring-after($value,'H'),'M'))"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="FormatTime">
              <xsl:with-param name="value" select="$value"/>
              <xsl:with-param name="format" select="$format"/>
              <xsl:with-param name="numId" select="$numId"/>
              <xsl:with-param name="processedFormat">
                <xsl:value-of select="substring-after($processedFormat,'m')"/>
              </xsl:with-param>
              <xsl:with-param name="outputValue">
                <xsl:value-of
                  select="concat($outputValue,number(substring-before(substring-after($value,'H'),'M')))"
                />
              </xsl:with-param>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:when test="starts-with($processedFormat,'s')">
        <xsl:choose>
          <xsl:when test="starts-with(substring-after($processedFormat,'s'),'s')">
            <xsl:call-template name="FormatTime">
              <xsl:with-param name="value" select="$value"/>
              <xsl:with-param name="format" select="$format"/>
              <xsl:with-param name="numId" select="$numId"/>
              <xsl:with-param name="processedFormat">
                <xsl:value-of select="substring-after($processedFormat,'ss')"/>
              </xsl:with-param>
              <xsl:with-param name="outputValue">
                <xsl:value-of
                  select="concat($outputValue,substring-before(substring-after($value,'M'),'S'))"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="FormatTime">
              <xsl:with-param name="value" select="$value"/>
              <xsl:with-param name="format" select="$format"/>
              <xsl:with-param name="numId" select="$numId"/>
              <xsl:with-param name="processedFormat">
                <xsl:value-of select="substring-after($processedFormat,'s')"/>
              </xsl:with-param>
              <xsl:with-param name="outputValue">
                <xsl:value-of
                  select="concat($outputValue,number(substring-before(substring-after($value,'M'),'S')))"
                />
              </xsl:with-param>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:when
        test="starts-with($processedFormat,'\') or starts-with($processedFormat,'@') or starts-with($processedFormat,';') or starts-with($processedFormat,'[') or starts-with($processedFormat,']')">
        <xsl:call-template name="FormatTime">
          <xsl:with-param name="value" select="$value"/>
          <xsl:with-param name="format" select="$format"/>
          <xsl:with-param name="numId" select="$numId"/>
          <xsl:with-param name="processedFormat">
            <xsl:value-of select="substring($processedFormat,2)"/>
          </xsl:with-param>
          <xsl:with-param name="outputValue" select="$outputValue"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="string-length($processedFormat) = 0">
        <xsl:value-of select="$outputValue"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="FormatTime">
          <xsl:with-param name="value" select="$value"/>
          <xsl:with-param name="format" select="$format"/>
          <xsl:with-param name="numId" select="$numId"/>
          <xsl:with-param name="processedFormat">
            <xsl:value-of select="substring($processedFormat,2)"/>
          </xsl:with-param>
          <xsl:with-param name="outputValue">
            <xsl:value-of select="concat($outputValue,substring($processedFormat,0,2))"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="ConvertMonthToName">

    <!-- @Descripition: gets month name from its number -->
    <!-- @Context: None -->

    <xsl:param name="month"/>
    <!-- (int) month number -->
    <xsl:choose>
      <xsl:when test="number($month) = 1">January</xsl:when>
      <xsl:when test="number($month) = 2">February</xsl:when>
      <xsl:when test="number($month) = 3">March</xsl:when>
      <xsl:when test="number($month) = 4">April</xsl:when>
      <xsl:when test="number($month) = 5">May</xsl:when>
      <xsl:when test="number($month) = 6">June</xsl:when>
      <xsl:when test="number($month) = 7">July</xsl:when>
      <xsl:when test="number($month) = 8">August</xsl:when>
      <xsl:when test="number($month) = 9">September</xsl:when>
      <xsl:when test="number($month) = 10">October</xsl:when>
      <xsl:when test="number($month) = 11">November</xsl:when>
      <xsl:when test="number($month) = 12">December</xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="ConvertMonthToShortName">

    <!-- @Descripition: gets shortcut of month name from its number -->
    <!-- @Context: None -->

    <xsl:param name="month"/>
    <!-- (int) month number -->
    <xsl:choose>
      <xsl:when test="number($month) = 1">Jan</xsl:when>
      <xsl:when test="number($month) = 2">Feb</xsl:when>
      <xsl:when test="number($month) = 3">Mar</xsl:when>
      <xsl:when test="number($month) = 4">Apr</xsl:when>
      <xsl:when test="number($month) = 5">May</xsl:when>
      <xsl:when test="number($month) = 6">Jun</xsl:when>
      <xsl:when test="number($month) = 7">Jul</xsl:when>
      <xsl:when test="number($month) = 8">Aug</xsl:when>
      <xsl:when test="number($month) = 9">Sep</xsl:when>
      <xsl:when test="number($month) = 10">Oct</xsl:when>
      <xsl:when test="number($month) = 11">Nov</xsl:when>
      <xsl:when test="number($month) = 12">Dec</xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="ConvertDayToName">

    <!-- @Descripition: gets day of week name from number of days after 1.01.1900 -->
    <!-- @Context: None -->

    <xsl:param name="day"/>
    <!-- (int) number of days -->
    <xsl:variable name="dayOfWeek">
      <xsl:value-of select="$day - 7 * floor($day div 7)"/>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="$dayOfWeek = 0">Monday</xsl:when>
      <xsl:when test="$dayOfWeek = 1">Tuesday</xsl:when>
      <xsl:when test="$dayOfWeek = 2">Wednesday</xsl:when>
      <xsl:when test="$dayOfWeek = 3">Thursday</xsl:when>
      <xsl:when test="$dayOfWeek = 4">Friday</xsl:when>
      <xsl:when test="$dayOfWeek = 5">Saturday</xsl:when>
      <xsl:when test="$dayOfWeek = 6">Sunday</xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="ConvertDayToShortName">

    <!-- @Descripition: gets shortcut of day of week name from number of days after 1.01.1900 -->
    <!-- @Context: None -->

    <xsl:param name="day"/>
    <!-- (int) number of days -->
    <xsl:variable name="dayOfWeek">
      <xsl:value-of select="$day - 7 * floor($day div 7)"/>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="$dayOfWeek = 0">Mon</xsl:when>
      <xsl:when test="$dayOfWeek = 1">Tue</xsl:when>
      <xsl:when test="$dayOfWeek = 2">Wed</xsl:when>
      <xsl:when test="$dayOfWeek = 3">Thu</xsl:when>
      <xsl:when test="$dayOfWeek = 4">Fri</xsl:when>
      <xsl:when test="$dayOfWeek = 5">Sat</xsl:when>
      <xsl:when test="$dayOfWeek = 6">Sun</xsl:when>
    </xsl:choose>
  </xsl:template>
  
  </xsl:stylesheet>