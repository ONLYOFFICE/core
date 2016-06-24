#-------------------------------------------------
#
# Project created by QtCreator 2015-06-03T10:54:04
#
#-------------------------------------------------

QT       -= core gui

TARGET = OdfFileReaderLib
TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

win32 {
    QMAKE_CXXFLAGS_RELEASE -= -Zc:strictStrings
    QMAKE_CXXFLAGS += /bigobj
} else {
    QMAKE_CXXFLAGS += -std=c++11 -Wall -Wno-ignored-qualifiers
}
############### destination path ###############
DESTINATION_SDK_PATH = $$PWD/../../build/lib

# WINDOWS
win32:contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTDIR = $$DESTINATION_SDK_PATH/win_64/DEBUG
} else {
    DESTDIR = $$DESTINATION_SDK_PATH/win_64
}
}
win32:!contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTDIR = $$DESTINATION_SDK_PATH/win_32/DEBUG
} else {
    DESTDIR = $$DESTINATION_SDK_PATH/win_32
}
}

linux-g++ | linux-g++-64 | linux-g++-32:contains(QMAKE_HOST.arch, x86_64):{
    DESTDIR = $$DESTINATION_SDK_PATH/linux_64
}
linux-g++ | linux-g++-64 | linux-g++-32:!contains(QMAKE_HOST.arch, x86_64):{
    DESTDIR = $$DESTINATION_SDK_PATH/linux_32
}

mac {
    DESTDIR = $$DESTINATION_SDK_PATH/mac_64
}
############### destination path ###############

DEFINES +=  UNICODE \
            _UNICODE \
            _USE_LIBXML2_READER_ \
            _USE_XMLLITE_READER_ \
            USE_LITE_READER

INCLUDEPATH += ../include
INCLUDEPATH += ../../DesktopEditor/freetype-2.5.2/include

CONFIG(debug, debug|release){
DEFINES +=  _DEBUG
}
#################### WINDOWS #####################
win32 {
    DEFINES += \
        LIBXML_READER_ENABLED

INCLUDEPATH += ../../Common/DocxFormat/Source/XML/libxml2/XML/include
INCLUDEPATH += C:/boost_1_58_0
}
#################### WINDOWS #####################

#################### LINUX ########################
linux-g++ | linux-g++-64 | linux-g++-32 {
    DEFINES += \
        LINUX \
        _LINUX \
        _LINUX_QT

INCLUDEPATH += /usr/include/libxml2
}

mac {
    DEFINES += \
        LINUX \
        _LINUX \
        _LINUX_QT \
        _MAC \
        MAC \
        LIBXML_READER_ENABLED

INCLUDEPATH += ../../Common/DocxFormat/Source/XML/libxml2/XML/include
INCLUDEPATH += ../../Common/boost_1_58_0
}
#################### LINUX ########################

build_fast {
SOURCES += \
    odffilereaderlib_odf.cpp \
    odffilereaderlib_docx.cpp \
    odffilereaderlib_pptx.cpp \
    odffilereaderlib_xlsx.cpp \
    odffilereaderlib_converter.cpp
} else {
SOURCES += \    
    ../src/conversionelement.cpp \
    ../src/xml/attributes.cpp \
    ../src/xml/sax.cpp \
    ../src/xml/sax_xmllite.cpp \
    ../src/xml/utils.cpp \
    ../src/xml/xmlchar.cpp \
    ../src/odf/abstract_xml.cpp \
    ../src/odf/anim_elements.cpp \
    ../src/odf/calcs_styles.cpp \
    ../src/odf/chart_build_oox.cpp \
    ../src/odf/createandread.cpp \
    ../src/odf/documentcontext.cpp \
    ../src/odf/draw_common.cpp \
    ../src/odf/draw_frame.cpp \
    ../src/odf/draw_frame_docx.cpp \
    ../src/odf/draw_frame_pptx.cpp \
    ../src/odf/draw_frame_xlsx.cpp \
    ../src/odf/draw_page.cpp \
    ../src/odf/draw_shapes.cpp \
    ../src/odf/draw_shapes_docx.cpp \
    ../src/odf/draw_shapes_pptx.cpp \
    ../src/odf/draw_shapes_xlsx.cpp \
    ../src/odf/font_face.cpp \
    ../src/odf/header_footer.cpp \
    ../src/odf/list.cpp \
    ../src/odf/logging.cpp \
    ../src/odf/note.cpp \
    ../src/odf/number_style.cpp \
    ../src/odf/odf_content_xml.cpp \
    ../src/odf/odfcontext.cpp \
    ../src/odf/odf_document.cpp \
    ../src/odf/odf_document_impl.cpp \
    ../src/odf/office_annotation.cpp \
    ../src/odf/office_binary_data.cpp \
    ../src/odf/office_body.cpp \
    ../src/odf/office_chart.cpp \
    ../src/odf/office_document.cpp \
    ../src/odf/office_elements_create.cpp \
    ../src/odf/office_event_listeners.cpp \
    ../src/odf/office_presentation.cpp \
    ../src/odf/office_scripts.cpp \
    ../src/odf/office_settings.cpp \
    ../src/odf/office_spreadsheet.cpp \
    ../src/odf/office_text.cpp \
    ../src/odf/paragraph_elements.cpp \
    ../src/odf/ruby.cpp \
    ../src/odf/search_table_cell.cpp \
    ../src/odf/skipelement.cpp \
    ../src/odf/style_chart_properties.cpp \
    ../src/odf/style_graphic_properties.cpp \
    ../src/odf/style_map.cpp \
    ../src/odf/style_paragraph_properties.cpp \
    ../src/odf/style_paragraph_properties_docx.cpp \
    ../src/odf/style_paragraph_properties_pptx.cpp \
    ../src/odf/style_presentation.cpp \
    ../src/odf/style_regions.cpp \
    ../src/odf/styles.cpp \
    ../src/odf/styles_list.cpp \
    ../src/odf/styles_lite_container.cpp \
    ../src/odf/style_table_properties.cpp \
    ../src/odf/style_text_properties.cpp \
    ../src/odf/svg_parser.cpp \
    ../src/odf/table.cpp \
    ../src/odf/table_calculation_settings.cpp \
    ../src/odf/table_docx.cpp \
    ../src/odf/table_named_expressions.cpp \
    ../src/odf/table_pptx.cpp \
    ../src/odf/table_xlsx.cpp \
    ../src/odf/templates.cpp \
    ../src/odf/text_content.cpp \
    ../src/odf/text_elements.cpp \
    ../src/odf/datatypes/anchortype.cpp \
    ../src/odf/datatypes/backgroundcolor.cpp \
    ../src/odf/datatypes/bool.cpp \
    ../src/odf/datatypes/bordermodel.cpp \
    ../src/odf/datatypes/borderstyle.cpp \
    ../src/odf/datatypes/borderwidths.cpp \
    ../src/odf/datatypes/calcext_type.cpp \
    ../src/odf/datatypes/chartdatalabelnumber.cpp \
    ../src/odf/datatypes/charterrorcategory.cpp \
    ../src/odf/datatypes/chartinterpolation.cpp \
    ../src/odf/datatypes/chartlabelarrangement.cpp \
    ../src/odf/datatypes/chartregressiontype.cpp \
    ../src/odf/datatypes/chartseriessource.cpp \
    ../src/odf/datatypes/chartsolidtype.cpp \
    ../src/odf/datatypes/chartsymbol.cpp \
    ../src/odf/datatypes/clockvalue.cpp \
    ../src/odf/datatypes/color.cpp \
    ../src/odf/datatypes/common_attlists.cpp \
    ../src/odf/datatypes/direction.cpp \
    ../src/odf/datatypes/drawfill.cpp \
    ../src/odf/datatypes/dropcaplength.cpp \
    ../src/odf/datatypes/fillimagerefpoint.cpp \
    ../src/odf/datatypes/fobreak.cpp \
    ../src/odf/datatypes/fontfamilygeneric.cpp \
    ../src/odf/datatypes/fontpitch.cpp \
    ../src/odf/datatypes/fontrelief.cpp \
    ../src/odf/datatypes/fontsize.cpp \
    ../src/odf/datatypes/fontstretch.cpp \
    ../src/odf/datatypes/fontstyle.cpp \
    ../src/odf/datatypes/fontvariant.cpp \
    ../src/odf/datatypes/fontweight.cpp \
    ../src/odf/datatypes/gradientstyle.cpp \
    ../src/odf/datatypes/hatchstyle.cpp \
    ../src/odf/datatypes/hyphenationkeep.cpp \
    ../src/odf/datatypes/hyphenationladdercount.cpp \
    ../src/odf/datatypes/iconset_type.cpp \
    ../src/odf/datatypes/keeptogether.cpp \
    ../src/odf/datatypes/layoutgridmode.cpp \
    ../src/odf/datatypes/length.cpp \
    ../src/odf/datatypes/lengthorpercent.cpp \
    ../src/odf/datatypes/letterspacing.cpp \
    ../src/odf/datatypes/linebreak.cpp \
    ../src/odf/datatypes/linemode.cpp \
    ../src/odf/datatypes/linestyle.cpp \
    ../src/odf/datatypes/linetype.cpp \
    ../src/odf/datatypes/linewidth.cpp \
    ../src/odf/datatypes/markerstyle.cpp \
    ../src/odf/datatypes/noteclass.cpp \
    ../src/odf/datatypes/officevaluetype.cpp \
    ../src/odf/datatypes/pageusage.cpp \
    ../src/odf/datatypes/percent.cpp \
    ../src/odf/datatypes/percentorscale.cpp \
    ../src/odf/datatypes/presentationclass.cpp \
    ../src/odf/datatypes/punctuationwrap.cpp \
    ../src/odf/datatypes/rotationalign.cpp \
    ../src/odf/datatypes/runthrough.cpp \
    ../src/odf/datatypes/scripttype.cpp \
    ../src/odf/datatypes/shadowtype.cpp \
    ../src/odf/datatypes/smil_transitiontype.cpp \
    ../src/odf/datatypes/stylefamily.cpp \
    ../src/odf/datatypes/stylehorizontalpos.cpp \
    ../src/odf/datatypes/stylehorizontalrel.cpp \
    ../src/odf/datatypes/styleleadercolor.cpp \
    ../src/odf/datatypes/styleposition.cpp \
    ../src/odf/datatypes/style_ref.cpp \
    ../src/odf/datatypes/stylerepeat.cpp \
    ../src/odf/datatypes/styletype.cpp \
    ../src/odf/datatypes/styleverticalpos.cpp \
    ../src/odf/datatypes/styleverticalrel.cpp \
    ../src/odf/datatypes/stylewrap.cpp \
    ../src/odf/datatypes/stylewrapcontourmode.cpp \
    ../src/odf/datatypes/tablealign.cpp \
    ../src/odf/datatypes/tablecentering.cpp \
    ../src/odf/datatypes/tablemode.cpp \
    ../src/odf/datatypes/tablevisibility.cpp \
    ../src/odf/datatypes/targetframename.cpp \
    ../src/odf/datatypes/textalign.cpp \
    ../src/odf/datatypes/textalignsource.cpp \
    ../src/odf/datatypes/textautospace.cpp \
    ../src/odf/datatypes/textcombine.cpp \
    ../src/odf/datatypes/textdisplay.cpp \
    ../src/odf/datatypes/textemphasize.cpp \
    ../src/odf/datatypes/textposition.cpp \
    ../src/odf/datatypes/textrotationscale.cpp \
    ../src/odf/datatypes/texttransform.cpp \
    ../src/odf/datatypes/underlinecolor.cpp \
    ../src/odf/datatypes/verticalalign.cpp \
    ../src/odf/datatypes/wrapoption.cpp \
    ../src/odf/datatypes/writingmode.cpp \
    ../src/odf/datatypes/xlink.cpp \
    ../src/docx/docx_content_type.cpp \
    ../src/docx/docx_conversion_context.cpp \
    ../src/docx/docx_drawing.cpp \
    ../src/docx/docx_package.cpp \
    ../src/docx/docx_rels.cpp \
    ../src/docx/docx_table_context.cpp \
    ../src/docx/headers_footers.cpp \
    ../src/docx/hyperlinks.cpp \
    ../src/docx/measuredigits.cpp \
    ../src/docx/mediaitems.cpp \
    ../src/docx/mediaitems_utils.cpp \
    ../src/docx/namespaces.cpp \
    ../src/docx/oox_chart_axis.cpp \
    ../src/docx/oox_chart_context.cpp \
    ../src/docx/oox_chart_legend.cpp \
    ../src/docx/oox_chart_series.cpp \
    ../src/docx/oox_chart_shape.cpp \
    ../src/docx/oox_conversion_context.cpp \
    ../src/docx/oox_data_labels.cpp \
    ../src/docx/oox_drawing.cpp \
    ../src/docx/oox_drawing_fills.cpp \
    ../src/docx/oox_layout.cpp \
    ../src/docx/oox_package.cpp \
    ../src/docx/oox_plot_area.cpp \
    ../src/docx/oox_title.cpp \
    ../src/docx/oox_types_chart.cpp \
    ../src/docx/pptx_comments.cpp \
    ../src/docx/pptx_comments_context.cpp \
    ../src/docx/pptx_conversion_context.cpp \
    ../src/docx/pptx_drawing.cpp \
    ../src/docx/pptx_drawings.cpp \
    ../src/docx/pptx_output_xml.cpp \
    ../src/docx/pptx_package.cpp \
    ../src/docx/pptx_slide_context.cpp \
    ../src/docx/pptx_table_context.cpp \
    ../src/docx/pptx_text_context.cpp \
    ../src/docx/xlsx_alignment.cpp \
    ../src/docx/xlsx_border.cpp \
    ../src/docx/xlsx_borders.cpp \
    ../src/docx/xlsx_cell_format.cpp \
    ../src/docx/xlsx_cell_style.cpp \
    ../src/docx/xlsx_cell_styles.cpp \
    ../src/docx/xlsx_color.cpp \
    ../src/docx/xlsx_comments.cpp \
    ../src/docx/xlsx_comments_context.cpp \
    ../src/docx/xlsx_complex_number_format.cpp \
    ../src/docx/xlsxconversioncontext.cpp \
    ../src/docx/xlsx_defined_names.cpp \
    ../src/docx/xlsx_drawing.cpp \
    ../src/docx/xlsx_drawing_context.cpp \
    ../src/docx/xlsx_drawings.cpp \
    ../src/docx/xlsx_fill.cpp \
    ../src/docx/xlsx_fills.cpp \
    ../src/docx/xlsx_font.cpp \
    ../src/docx/xlsx_fonts.cpp \
    ../src/docx/xlsx_hyperlinks.cpp \
    ../src/docx/xlsx_merge_cells.cpp \
    ../src/docx/xlsx_numFmts.cpp \
    ../src/docx/xlsx_num_format_context.cpp \
    ../src/docx/xlsx_output_xml.cpp \
    ../src/docx/xlsx_package.cpp \
    ../src/docx/xlsx_protection.cpp \
    ../src/docx/xlsx_sharedstrings.cpp \
    ../src/docx/xlsx_styles.cpp \
    ../src/docx/xlsx_tablecontext.cpp \
    ../src/docx/xlsx_table_metrics.cpp \
    ../src/docx/xlsx_table_state.cpp \
    ../src/docx/xlsx_textcontext.cpp \
    ../src/docx/xlsx_utils.cpp \
    ../src/docx/xlsx_xf.cpp \
    ../src/common/CPColorUtils.cpp \
    ../src/common/CPString.cpp \
    ../src/common/readdocelement.cpp \
    ../src/ConvertOO2OOX.cpp \
    ../src/odf/math_elements.cpp
}

SOURCES += \
    ../formulasconvert/formulasconvert_oox.cpp \
    ../src/odf/math_elementaries.cpp \
    ../src/odf/math_layout_elements.cpp \
    ../src/odf/math_limit_elements.cpp \
    ../src/odf/math_table_elements.cpp \
    ../src/odf/math_token_elements.cpp \
    ../src/odf/datatypes/mathvariant.cpp \
    ../formulasconvert/formulasconvert_odf.cpp \
    ../src/odf/calcext_elements.cpp \
    ../src/odf/table_database_ranges.cpp \
    ../src/docx/xlsx_conditionalFormatting.cpp \
    ../src/docx/xlsx_dxfs.cpp

HEADERS += \
    ../formulasconvert/formulasconvert.h \
    ../src/conversionelement.h \
    ../src/progressCallback.h \
    ../src/xml/sax_xmllite.h \
    ../src/odf/abstract_xml.h \
    ../src/odf/all_elements.h \
    ../src/odf/anim_elements.h \
    ../src/odf/calcs_styles.h \
    ../src/odf/chart_build_oox.h \
    ../src/odf/chart_objects.h \
    ../src/odf/documentcontext.h \
    ../src/odf/draw_common.h \
    ../src/odf/draw_frame.h \
    ../src/odf/draw_page.h \
    ../src/odf/draw_shapes.h \
    ../src/odf/font_face.h \
    ../src/odf/header_footer.h \
    ../src/odf/list.h \
    ../src/odf/note.h \
    ../src/odf/number_style.h \
    ../src/odf/odf_content_xml.h \
    ../src/odf/odfcontext.h \
    ../src/odf/odf_document_impl.h \
    ../src/odf/office_annotation.h \
    ../src/odf/office_binary_data.h \
    ../src/odf/office_body.h \
    ../src/odf/office_chart.h \
    ../src/odf/office_document.h \
    ../src/odf/office_elements.h \
    ../src/odf/office_elements_create.h \
    ../src/odf/office_elements_type.h \
    ../src/odf/office_event_listeners.h \
    ../src/odf/office_presentation.h \
    ../src/odf/office_scripts.h \
    ../src/odf/office_settings.h \
    ../src/odf/office_spreadsheet.h \
    ../src/odf/office_text.h \
    ../src/odf/paragraph_content.h \
    ../src/odf/paragraph_elements.h \
    ../src/odf/ruby.h \
    ../src/odf/search_table_cell.h \
    ../src/odf/serialize_elements.h \
    ../src/odf/skipelement.h \
    ../src/odf/style_chart_properties.h \
    ../src/odf/style_graphic_properties.h \
    ../src/odf/style_map.h \
    ../src/odf/style_paragraph_properties.h \
    ../src/odf/style_presentation.h \
    ../src/odf/style_regions.h \
    ../src/odf/styles.h \
    ../src/odf/styles_list.h \
    ../src/odf/styles_lite_container.h \
    ../src/odf/style_table_properties.h \
    ../src/odf/style_text_properties.h \
    ../src/odf/svg_parser.h \
    ../src/odf/table.h \
    ../src/odf/table_calculation_settings.h \
    ../src/odf/table_named_expressions.h \
    ../src/odf/templates.h \
    ../src/odf/text_content.h \
    ../src/odf/text_elements.h \
    ../src/odf/visitor.h \
    ../src/odf/datatypes/anchortype.h \
    ../src/odf/datatypes/backgroundcolor.h \
    ../src/odf/datatypes/bool.h \
    ../src/odf/datatypes/bordermodel.h \
    ../src/odf/datatypes/borderstyle.h \
    ../src/odf/datatypes/borderwidths.h \
    ../src/odf/datatypes/calcext_type.h \
    ../src/odf/datatypes/chartdatalabelnumber.h \
    ../src/odf/datatypes/charterrorcategory.h \
    ../src/odf/datatypes/chartinterpolation.h \
    ../src/odf/datatypes/chartlabelarrangement.h \
    ../src/odf/datatypes/chartregressiontype.h \
    ../src/odf/datatypes/chartseriessource.h \
    ../src/odf/datatypes/chartsolidtype.h \
    ../src/odf/datatypes/chartsymbol.h \
    ../src/odf/datatypes/clockvalue.h \
    ../src/odf/datatypes/color.h \
    ../src/odf/datatypes/common_attlists.h \
    ../src/odf/datatypes/custom_shape_types_convert.h \
    ../src/odf/datatypes/direction.h \
    ../src/odf/datatypes/drawfill.h \
    ../src/odf/datatypes/dropcaplength.h \
    ../src/odf/datatypes/errors.h \
    ../src/odf/datatypes/fillimagerefpoint.h \
    ../src/odf/datatypes/fobreak.h \
    ../src/odf/datatypes/fontfamilygeneric.h \
    ../src/odf/datatypes/fontpitch.h \
    ../src/odf/datatypes/fontrelief.h \
    ../src/odf/datatypes/fontsize.h \
    ../src/odf/datatypes/fontstretch.h \
    ../src/odf/datatypes/fontstyle.h \
    ../src/odf/datatypes/fontvariant.h \
    ../src/odf/datatypes/fontweight.h \
    ../src/odf/datatypes/gradientstyle.h \
    ../src/odf/datatypes/hatchstyle.h \
    ../src/odf/datatypes/hyphenationkeep.h \
    ../src/odf/datatypes/hyphenationladdercount.h \
    ../src/odf/datatypes/iconset_type.h \
    ../src/odf/datatypes/keeptogether.h \
    ../src/odf/datatypes/layoutgridmode.h \
    ../src/odf/datatypes/length.h \
    ../src/odf/datatypes/lengthorpercent.h \
    ../src/odf/datatypes/letterspacing.h \
    ../src/odf/datatypes/linebreak.h \
    ../src/odf/datatypes/linemode.h \
    ../src/odf/datatypes/linestyle.h \
    ../src/odf/datatypes/linetype.h \
    ../src/odf/datatypes/linewidth.h \
    ../src/odf/datatypes/markerstyle.h \
    ../src/odf/datatypes/noteclass.h \
    ../src/odf/datatypes/odfattributes.h \
    ../src/odf/datatypes/officevaluetype.h \
    ../src/odf/datatypes/pageusage.h \
    ../src/odf/datatypes/percent.h \
    ../src/odf/datatypes/percentorscale.h \
    ../src/odf/datatypes/presentationclass.h \
    ../src/odf/datatypes/punctuationwrap.h \
    ../src/odf/datatypes/rotationalign.h \
    ../src/odf/datatypes/runthrough.h \
    ../src/odf/datatypes/scripttype.h \
    ../src/odf/datatypes/shadowtype.h \
    ../src/odf/datatypes/smil_transitiontype.h \
    ../src/odf/datatypes/stylefamily.h \
    ../src/odf/datatypes/stylehorizontalpos.h \
    ../src/odf/datatypes/stylehorizontalrel.h \
    ../src/odf/datatypes/styleleadercolor.h \
    ../src/odf/datatypes/styleposition.h \
    ../src/odf/datatypes/style_ref.h \
    ../src/odf/datatypes/stylerepeat.h \
    ../src/odf/datatypes/styletype.h \
    ../src/odf/datatypes/styleverticalpos.h \
    ../src/odf/datatypes/styleverticalrel.h \
    ../src/odf/datatypes/stylewrap.h \
    ../src/odf/datatypes/stylewrapcontourmode.h \
    ../src/odf/datatypes/tablealign.h \
    ../src/odf/datatypes/tablecentering.h \
    ../src/odf/datatypes/tablemode.h \
    ../src/odf/datatypes/tablevisibility.h \
    ../src/odf/datatypes/targetframename.h \
    ../src/odf/datatypes/textalign.h \
    ../src/odf/datatypes/textalignsource.h \
    ../src/odf/datatypes/textautospace.h \
    ../src/odf/datatypes/textcombine.h \
    ../src/odf/datatypes/textdisplay.h \
    ../src/odf/datatypes/textemphasize.h \
    ../src/odf/datatypes/textposition.h \
    ../src/odf/datatypes/textrotationscale.h \
    ../src/odf/datatypes/texttransform.h \
    ../src/odf/datatypes/underlinecolor.h \
    ../src/odf/datatypes/verticalalign.h \
    ../src/odf/datatypes/wrapoption.h \
    ../src/odf/datatypes/writingmode.h \
    ../src/odf/datatypes/xlink.h \
    ../src/docx/docx_content_type.h \
    ../src/docx/docx_conversion_context.h \
    ../src/docx/docx_conversion_state.h \
    ../src/docx/docx_drawing.h \
    ../src/docx/docx_package.h \
    ../src/docx/docx_rels.h \
    ../src/docx/docx_table_context.h \
    ../src/docx/drawing_object_description.h \
    ../src/docx/headers_footers.h \
    ../src/docx/hyperlinks.h \
    ../src/docx/measuredigits.h \
    ../src/docx/mediaitems.h \
    ../src/docx/mediaitems_utils.h \
    ../src/docx/namespaces.h \
    ../src/docx/oox_chart_axis.h \
    ../src/docx/oox_chart_context.h \
    ../src/docx/oox_chart_legend.h \
    ../src/docx/oox_chart_series.h \
    ../src/docx/oox_chart_shape.h \
    ../src/docx/oox_chart_values.h \
    ../src/docx/oox_conversion_context.h \
    ../src/docx/oox_data_labels.h \
    ../src/docx/oox_drawing.h \
    ../src/docx/oox_drawing_fills.h \
    ../src/docx/oox_layout.h \
    ../src/docx/oox_package.h \
    ../src/docx/oox_plot_area.h \
    ../src/docx/ooxtablerowspanned.h \
    ../src/docx/oox_title.h \
    ../src/docx/oox_types_chart.h \
    ../src/docx/pptx_comments.h \
    ../src/docx/pptx_comments_context.h \
    ../src/docx/pptx_conversion_context.h \
    ../src/docx/pptx_default_serializes.h \
    ../src/docx/pptx_drawing.h \
    ../src/docx/pptx_drawings.h \
    ../src/docx/pptx_output_xml.h \
    ../src/docx/pptx_package.h \
    ../src/docx/pptx_slide_context.h \
    ../src/docx/pptx_table_context.h \
    ../src/docx/pptx_text_context.h \
    ../src/docx/xlsx_alignment.h \
    ../src/docx/xlsx_border.h \
    ../src/docx/xlsx_borders.h \
    ../src/docx/xlsx_cell_format.h \
    ../src/docx/xlsx_cell_style.h \
    ../src/docx/xlsx_cell_styles.h \
    ../src/docx/xlsx_color.h \
    ../src/docx/xlsx_comments.h \
    ../src/docx/xlsx_comments_context.h \
    ../src/docx/xlsx_complex_number_format.h \
    ../src/docx/xlsxconversioncontext.h \
    ../src/docx/xlsx_defined_names.h \
    ../src/docx/xlsx_drawing.h \
    ../src/docx/xlsx_drawing_context.h \
    ../src/docx/xlsx_drawings.h \
    ../src/docx/xlsx_fill.h \
    ../src/docx/xlsx_fills.h \
    ../src/docx/xlsx_font.h \
    ../src/docx/xlsx_fonts.h \
    ../src/docx/xlsx_formula.h \
    ../src/docx/xlsx_hyperlinks.h \
    ../src/docx/xlsx_merge_cells.h \
    ../src/docx/xlsx_numFmts.h \
    ../src/docx/xlsx_num_format_context.h \
    ../src/docx/xlsx_output_xml.h \
    ../src/docx/xlsx_package.h \
    ../src/docx/xlsx_protection.h \
    ../src/docx/xlsx_row_spanned.h \
    ../src/docx/xlsx_sharedstrings.h \
    ../src/docx/xlsx_styles.h \
    ../src/docx/xlsx_tablecontext.h \
    ../src/docx/xlsx_table_metrics.h \
    ../src/docx/xlsx_table_position.h \
    ../src/docx/xlsx_table_state.h \
    ../src/docx/xlsx_textcontext.h \
    ../src/docx/xlsx_utils.h \
    ../src/docx/xlsx_xf.h \
    ../include/logging.h \
    ../include/cpdoccore/CPColorUtils.h \
    ../include/cpdoccore/CPHash.h \
    ../include/cpdoccore/CPNoncopyable.h \
    ../include/cpdoccore/CPOptional.h \
    ../include/cpdoccore/CPScopedPtr.h \
    ../include/cpdoccore/CPSharedPtr.h \
    ../include/cpdoccore/CPString.h \
    ../include/cpdoccore/CPWeakPtr.h \
    ../include/cpdoccore/common/readdocelement.h \
    ../include/cpdoccore/common/readstring.h \
    ../include/cpdoccore/common/writedocelement.h \
    ../include/cpdoccore/odf/odf_document.h \
    ../include/cpdoccore/odf/odfformat.h \
    ../include/cpdoccore/utf8cpp/utf8/checked.h \
    ../include/cpdoccore/utf8cpp/utf8/core.h \
    ../include/cpdoccore/utf8cpp/utf8/unchecked.h \
    ../include/cpdoccore/utf8cpp/utf8.h \
    ../include/cpdoccore/xml/attributes.h \
    ../include/cpdoccore/xml/nodetype.h \
    ../include/cpdoccore/xml/sax.h \
    ../include/cpdoccore/xml/simple_xml_writer.h \
    ../include/cpdoccore/xml/utils.h \
    ../include/cpdoccore/xml/xmlchar.h \
    ../include/cpdoccore/xml/xmlelement.h \
    ../src/ConvertOO2OOX.h \
    ../src/odf/math_elements.h \
    ../src/odf/math_elementaries.h \
    ../src/odf/math_layout_elements.h \
    ../src/odf/math_limit_elements.h \
    ../src/odf/math_table_elements.h \
    ../src/odf/math_token_elements.h \
    ../src/odf/datatypes/mathvariant.h \
    ../src/odf/calcext_elements.h \
    ../src/odf/table_database_ranges.h \
    ../src/docx/xlsx_conditionalFormatting.h \
    ../src/docx/xlsx_dxfs.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
