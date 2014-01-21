<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:xlsx="http://www.avs4you.ru/XLSXConverter/1.0/DirectoryStructure"
                xmlns:xdr="http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing"
                xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">

  <xsl:template match="WorksheetSubstream" mode="legacyDrawing">
    <xsl:variable name="position" select="position()"/>

    <xsl:if test="count(OBJECTS/MsoDrawing/OfficeArtDgContainer//OfficeArtSpContainer/OfficeArtFSP[@shape_id = 202]) &gt; 0">
      <xlsx:file name="vmlDrawing{$position}.vml">

        <xml xmlns:v="urn:schemas-microsoft-com:vml" xmlns:o="urn:schemas-microsoft-com:office:office" xmlns:x="urn:schemas-microsoft-com:office:excel">
          <o:shapelayout v:ext="edit">
            <o:idmap v:ext="edit" data="1" />
          </o:shapelayout>
          <v:shapetype id="_x0000_t202" coordsize="21600,21600" o:spt="202" path="m,l,21600r21600,l21600,xe">
            <v:stroke joinstyle="miter" />
            <v:path gradientshapeok="t" o:connecttype="rect" />
          </v:shapetype>
        

        <xsl:for-each select="OBJECTS/MsoDrawing/OfficeArtDgContainer//OfficeArtSpContainer/OfficeArtFSP[@shape_id = 202]">

          <xsl:variable name="note_index" select="position()"></xsl:variable>
          
          <v:shape id="{concat('_x0000_s', @spid)}" type="#_x0000_t202" style="position:absolute; z-index:1; visibility:hidden" fillcolor="{concat('#', ../OfficeArtFOPT/OfficeArtRGFOPTE/fillColor/OfficeArtCOLORREF/@rgb)}" o:insetmode="auto">
            <v:fill color2="{concat('#', ../OfficeArtFOPT/OfficeArtRGFOPTE/fillBackColor/OfficeArtCOLORREF/@rgb)}" />
            <v:shadow on="t" color="black" obscured="t" />
            <v:path o:connecttype="none" />
            <v:textbox style="mso-direction-alt:auto">
              <div style="text-align:left" />
            </v:textbox>
             <x:ClientData ObjectType="Note">
              <x:MoveWithCells />
              <x:SizeWithCells />
              <!--<x:Anchor>1, 15, 0, 2, 3, 31, 4, 1</x:Anchor>-->
               
              <x:Anchor>
                <xsl:value-of select="../OfficeArtClientAnchorSheet/@colL"></xsl:value-of>
                <xsl:text>,0,</xsl:text>
                <xsl:value-of select="../OfficeArtClientAnchorSheet/@rwT"></xsl:value-of>
                <xsl:text>,0,</xsl:text>
                <xsl:value-of select="../OfficeArtClientAnchorSheet/@colR"></xsl:value-of>
                <xsl:text>,0,</xsl:text>
                <xsl:value-of select="../OfficeArtClientAnchorSheet/@rwB"></xsl:value-of>
                <xsl:text>,0</xsl:text>
            </x:Anchor>
               
              <x:AutoFill>False</x:AutoFill>
              <x:Row>
                <xsl:value-of select="../../../../../../Note[$note_index]/NoteSh/@row"></xsl:value-of>
              </x:Row>
              <x:Column>
                <xsl:value-of select="../../../../../../Note[$note_index]/NoteSh/@col"></xsl:value-of>
              </x:Column>
            </x:ClientData>
          </v:shape>
        </xsl:for-each>

        </xml>

      </xlsx:file>
    </xsl:if>
  </xsl:template>

  <xsl:template match="WorksheetSubstream" mode="drawing">
    <xsl:variable name="position" select="position()"/>
    <!--<xsl:variable name="position" select="count(preceding-sibling::WorksheetSubstream[OBJECTS]) + count(preceding-sibling::ChartSheetSubstream) + 1"/>-->
    <xlsx:file name="drawing{$position}.xml">
      <xdr:wsDr xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">

        <!--<xsl:apply-templates select="OBJECTS/MsoDrawing" mode="drawing"/>-->
        <xsl:apply-templates select="OBJECTS/OBJ" mode="extract_objects"/>

      </xdr:wsDr>
    </xlsx:file>
  </xsl:template>

  <xsl:template match="ChartSheetSubstream" mode="drawing">
    <xsl:variable name="position" select="position()"/>
    <!--<xsl:variable name="position" select="count(preceding-sibling::WorksheetSubstream[OBJECTS]) + count(preceding-sibling::ChartSheetSubstream) + 1"/>-->
    <xlsx:file name="drawing{$position}.xml">
      <xdr:wsDr xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">
        <xdr:absoluteAnchor>
          <xdr:pos x="{floor(CHARTFOMATS/Chart/@x * 12700)}" y="{floor(CHARTFOMATS/Chart/@y * 12700)}"/>
          <xdr:ext cx="{floor(CHARTFOMATS/Chart/@dx * 12700)}" cy="{floor(CHARTFOMATS/Chart/@dy * 12700)}"/>
          <xdr:graphicFrame macro="">
            <xdr:nvGraphicFramePr>
              <xsl:variable name="stream_ptr" select="./BOF/@stream_ptr"/>
              <xdr:cNvPr id="2" name="{../GlobalsSubstream/BUNDLESHEET/BoundSheet8[@lbPlyPos=$stream_ptr]/@stName}"/>
              <xdr:cNvGraphicFramePr>
                <a:graphicFrameLocks noGrp="1"/>
              </xdr:cNvGraphicFramePr>
            </xdr:nvGraphicFramePr>
            <xdr:xfrm>
              <a:off x="0" y="0"/>
              <a:ext cx="0" cy="0"/>
            </xdr:xfrm>
            <a:graphic>
              <a:graphicData uri="http://schemas.openxmlformats.org/drawingml/2006/chart">
                <c:chart xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships" r:id="rId1"/>
              </a:graphicData>
            </a:graphic>
          </xdr:graphicFrame>
          <xdr:clientData/>
        </xdr:absoluteAnchor>
      </xdr:wsDr>
    </xlsx:file>
  </xsl:template>

  <xsl:template match="OBJ" mode="extract_objects">
    <xsl:variable name="sp_position" select="position()"/>
    <xsl:variable name="object" select="."/>
    <xsl:variable name="ftcmo_ot" select="Obj/FtCmo/@ot"/>
    <xsl:for-each select="../MsoDrawing/OfficeArtDgContainer//OfficeArtSpContainer[OfficeArtClientAnchorSheet][position() = $sp_position]">
      <xsl:call-template name="OfficeArtSpContainer">
        <xsl:with-param name="object" select="$object"/>
        <xsl:with-param name="sp_position" select="$sp_position"/>
        <xsl:with-param name="ftcmo_ot" select="$ftcmo_ot"/>
      </xsl:call-template>
    </xsl:for-each>
    <xsl:apply-templates select="OfficeArtDgContainer//OfficeArtSpContainer[OfficeArtClientAnchorSheet]" mode="drawing"/>

  </xsl:template>

  <xsl:template name="OfficeArtSpContainer">
    <xsl:param name="sp_position"/>
    <xsl:param name="object"/>
    <xsl:param name="ftcmo_ot"/>

    <xsl:if test="$ftcmo_ot = 5 or ($ftcmo_ot = 8 and OfficeArtFOPT/OfficeArtRGFOPTE/pib)">
      <xdr:twoCellAnchor>
        <xdr:from>

          <xdr:col>
            <xsl:value-of select="OfficeArtClientAnchorSheet/@colL"/>
          </xdr:col>

          <xdr:colOff>
            <xsl:value-of select="OfficeArtClientAnchorSheet/@dxL"/>
            <!--<xsl:value-of select="0"/>-->
          </xdr:colOff>

          <xdr:row>
            <xsl:value-of select="OfficeArtClientAnchorSheet/@rwT"/>
          </xdr:row>

          <xdr:rowOff>
            <xsl:value-of select="OfficeArtClientAnchorSheet/@dyT"/>
            <!--<xsl:value-of select="0"/>-->
          </xdr:rowOff>

        </xdr:from>
        <xdr:to>

          <xdr:col>
            <xsl:value-of select="OfficeArtClientAnchorSheet/@colR"/>
          </xdr:col>

          <xdr:colOff>
            <xsl:value-of select="OfficeArtClientAnchorSheet/@dxR"/>
            <!--<xsl:value-of select="0"/>-->
          </xdr:colOff>

          <xdr:row>
            <xsl:value-of select="OfficeArtClientAnchorSheet/@rwB"/>
          </xdr:row>

          <xdr:rowOff>
            <xsl:value-of select="OfficeArtClientAnchorSheet/@dyB"/>
            <!--<xsl:value-of select="0"/>-->
          </xdr:rowOff>

        </xdr:to>

        <xsl:choose>
          <!-- picture -->
          <!--<xsl:when test="(OfficeArtFOPT/OfficeArtRGFOPTE/fillType/@type = 'msofillPicture') or OfficeArtFOPT/OfficeArtRGFOPTE/pib">-->
          <xsl:when test="$ftcmo_ot = 8">
            <xsl:variable name="object_local_id" select="count($object/preceding-sibling::OBJ/Obj/FtCmo[@ot = 8 or @ot = 5]) + 1"/>
            <xdr:pic>
              <xdr:nvPicPr>
                <xdr:cNvPr descr="" id="{OfficeArtFSP/@spid}" name="{concat('Picture ', $object_local_id)}" />
                <xdr:cNvPicPr />
              </xdr:nvPicPr>
              <xdr:blipFill>
                <a:blip r:embed="rId{$object_local_id}" xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"/>
                <a:stretch>
                  <a:fillRect />
                </a:stretch>
              </xdr:blipFill>
              <xdr:spPr>
                <a:xfrm>
                  <a:off x="0" y="0" />
                  <a:ext cx="0" cy="0" />
                </a:xfrm>
                <a:prstGeom prst="rect">
                  <a:avLst />
                </a:prstGeom>
              </xdr:spPr>
            </xdr:pic>
            <xdr:clientData/>
          </xsl:when>
          <xsl:when test="$ftcmo_ot = 5">
            <!-- chart -->
            <xdr:graphicFrame macro="">
              <xdr:nvGraphicFramePr>
                <xdr:cNvPr id="{OfficeArtFSP/@spid}" name="{concat('Chart ', $sp_position)}"/>
                <xdr:cNvGraphicFramePr>
                  <a:graphicFrameLocks/>
                </xdr:cNvGraphicFramePr>
              </xdr:nvGraphicFramePr>
              <xdr:xfrm>
                <xsl:if test="OfficeArtFSP/fFlipH = 'true'">
                  <xsl:attribute name="flipH">true</xsl:attribute>
                </xsl:if>
                <xsl:if test="OfficeArtFSP/fFlipV = 'true'">
                  <xsl:attribute name="flipV">true</xsl:attribute>
                </xsl:if>
                <a:off x="0" y="0"/>
                <a:ext cx="0" cy="0"/>
              </xdr:xfrm>
              <a:graphic>
                <xsl:variable name="uri">
                  <xsl:choose>
                    <xsl:when test="$object/self::OBJ">http://schemas.openxmlformats.org/drawingml/2006/chart</xsl:when>
                    <xsl:when test="$object/self::TEXTOBJECT">insert text object uri here</xsl:when>
                  </xsl:choose>
                </xsl:variable>
                <a:graphicData uri="{$uri}">
                  <xsl:choose>
                    <xsl:when test="$object/self::OBJ">
                      <xsl:variable name="object_local_id" select="count($object/preceding-sibling::OBJ/Obj/FtCmo[@ot = 5 or @ot = 8]) + 1"/>
                      <c:chart r:id="rId{$object_local_id}" xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"/>
                    </xsl:when>
                    <xsl:when test="$object/self::TEXTOBJECT">
                      <t:text xmlns:t="insert text object uri here" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships" r:id="rIdXXXXX"/>
                    </xsl:when>
                  </xsl:choose>
                </a:graphicData>
              </a:graphic>
            </xdr:graphicFrame>
            <xdr:clientData/>
          </xsl:when>
        </xsl:choose>
      </xdr:twoCellAnchor>
    </xsl:if>
    
  </xsl:template>

  <xsl:template name="drawings_rels">
    <xsl:if test="WorksheetSubstream/OBJECTS | ChartSheetSubstream">
      <xlsx:dir name="_rels">
        <xsl:for-each select="WorksheetSubstream/OBJECTS | ChartSheetSubstream">
          <xlsx:file name="drawing{position()}.xml.rels">
            <Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships">

              <xsl:if test="self::ChartSheetSubstream">
                <xsl:variable name="chart_id" select="count(preceding-sibling::WorksheetSubstream/OBJECTS/OBJ/Obj/FtCmo[@ot = 5]) + count(preceding-sibling::ChartSheetSubstream) + 1"/>
                <Relationship Id="rId1" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/chart"
                              Target="../charts/chart{$chart_id}.xml"/>
              </xsl:if>

              <xsl:for-each select="OBJ">

                <xsl:variable name="object_id" select="count(preceding-sibling::OBJ) + 1"/>
                <xsl:variable name="object_local_id" select="count(preceding-sibling::OBJ/Obj/FtCmo[@ot = 5 or @ot = 8]) + 1"/>
                <xsl:variable name="object_id_chart" select="count(preceding-sibling::OBJ/Obj/FtCmo[@ot = 5]) + count(../../preceding-sibling::WorksheetSubstream/OBJECTS/OBJ/Obj/FtCmo[@ot = 5]) + count(../../preceding-sibling::ChartSheetSubstream) + 1"/>
                <xsl:variable name="object_id_image" select="count(preceding-sibling::OBJ/Obj/FtCmo[@ot = 8]) + 1"/>
                <xsl:variable name="pib_op" select="../MsoDrawing/OfficeArtDgContainer//OfficeArtSpContainer[OfficeArtClientAnchorSheet][position() = $object_id]/OfficeArtFOPT//OfficeArtRGFOPTE/pib/@op"/>

                <xsl:if test="Obj/FtCmo[@ot = 5 or @ot = 8]">
                  <Relationship Id="rId{$object_local_id}">

                    <xsl:if test="Obj/FtCmo/@ot = 5">
                      <xsl:attribute name="Type">
                        <xsl:value-of select="'http://schemas.openxmlformats.org/officeDocument/2006/relationships/chart'" />
                      </xsl:attribute>
                      <xsl:attribute name="Target">
                        <xsl:text>../charts/chart</xsl:text>
                        <xsl:value-of select="$object_id_chart"/>
                        <xsl:text>.xml</xsl:text>
                      </xsl:attribute>
                    </xsl:if>

                    <xsl:variable name="image_ext" select="../../../GlobalsSubstream/MSODRAWINGGROUP/MsoDrawingGroup/OfficeArtDggContainer/OfficeArtBStoreContainer/OfficeArtBStoreContainerFileBlock[(@bin_data_id + 1) = $pib_op]/@type"/>
                    <xsl:if test="Obj/FtCmo/@ot = 8 and $image_ext">
                      <xsl:attribute name="Type">
                        <xsl:value-of select="'http://schemas.openxmlformats.org/officeDocument/2006/relationships/image'" />
                      </xsl:attribute>
                      <xsl:attribute name="Target">
                        <xsl:text>../media/image</xsl:text>
                        <!-- <xsl:value-of select="$object_id_image"/> -->
                        <xsl:value-of select="$pib_op"/>
                        <xsl:text>.</xsl:text>
                        <xsl:value-of select="$image_ext"/>
                      </xsl:attribute>
                    </xsl:if>

                  </Relationship>
                </xsl:if>

              </xsl:for-each>

            </Relationships>
          </xlsx:file>
        </xsl:for-each>
      </xlsx:dir>
    </xsl:if>
  </xsl:template>



</xsl:stylesheet>