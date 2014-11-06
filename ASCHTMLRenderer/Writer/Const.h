#pragma once

// viewer
static CString g_bstr_viewer_page								= L"<div class=\"blockpage\">\n";
static CString g_string_viewer_page								= _T("<div id=\"page%d\" class=\"blockpage\" style=\"top: %dpx; width: %dpx; height: %dpx;\">\n");
static CString g_bstr_viewer_page_between						= L"<div class=\"blockpagebetween\"></div>\n";
static CString g_bstr_viewer_end_div							= L"</div>\n";
static CString g_bstr_viewer_end_div2							= L"\n</div>\n";

// script constans
static CString g_bstr_script_open								= L"{\n";
static CString g_bstr_script_close								= L"}\n";
static CString g_bstr_script_var								= L"var ";

// canvas (vector graphics)
static CString g_string_canvas_ByID								= _T("_canvas = document.getElementById('shape_%d');\n");
static CString g_bstr_canvas_Correct							= L"if(_canvas && _canvas.getContext)\n";
static CString g_bstr_canvas_Context							= L"c = _canvas.getContext('2d');\n";

static CString g_bstr_canvas_StartPath							= L"b(c);\n";
static CString g_bstr_canvas_ClosePath							= L"x(c);\n";

static CString g_string_canvas_MoveTo							= _T("m(c,%.2lf,%.2lf);\n");
static CString g_string_canvas_LineTo							= _T("l(c,%.2lf,%.2lf);\n");
static CString g_string_canvas_CurveTo							= _T("c(c,%.2lf,%.2lf,%.2lf,%.2lf,%.2lf,%.2lf);\n");

static CString g_string_canvas_MoveToD							= _T("m(c,%d,%d);\n");
static CString g_string_canvas_LineToD							= _T("l(c,%d,%d);\n");
static CString g_string_canvas_CurveToD							= _T("cu(c,%d,%d,%d,%d,%d,%d);\n");

static CString g_bstr_canvas_Save								= L"c.save();\n";
static CString g_bstr_canvas_Restore							= L"c.restore();\n";
static CString g_bstr_canvas_Clip								= L"c.clip();\n";

static CString g_bstr_canvas_Stroke								= L"s(c);\n";
static CString g_bstr_canvas_Fill								= L"f(c);\n";

static CString g_string_css_color								= _T("rgb(%d,%d,%d)");

static CString g_string_canvas_FillColor						= _T("c.fillStyle = \"#%06x\";\n");
static CString g_string_canvas_FillColorA						= _T("c.fillStyle = \"rgba(%d,%d,%d,%.2lf)\";\n");
static CString g_string_canvas_StrokeColor						= _T("c.strokeStyle = \"#%06x\";\n");
static CString g_string_canvas_StrokeColorA						= _T("c.strokeStyle = \"rgba(%d,%d,%d,%.2lf)\";\n");
static CString g_string_canvas_drawImage						= _T("img%d = new Image();img%d.src = \"media\\\\image%d.jpg\";img%d.onload = function(){c.drawImage(img%d,%d,%d,%d,%d);drawpage%d_%d(c);};\n}\nfunction drawpage%d_%d(c)\n{\n");

// vector markup language
static CString g_bstr_vml_InitializeStyle						= L"v\\:*{behavior:url(#default#VML)}\n";

static CString g_bstr_vml_StartPath								= L"";
static CString g_bstr_vml_ClosePath								= L"x";

static CString g_string_vml_MoveTo								= _T("m%d,%d ");
static CString g_string_vml_LineTo								= _T("l%d,%d ");
static CString g_string_vml_CurveTo								= _T("c%d,%d,%d,%d,%d,%d ");

// text (потом заменить на "умное" расположение текста)
static CString g_string_text									= _T("<p style=\"position:absolute; left:%d; top:%d; font-family: %ls; font-size: %dpx; color:#%x;\">%ls</p>\n");

// image
static CString g_string_image									= _T("<img style=\"position:absolute; left:%dpx; top: %dpx; width: %dpx; height:%dpx;\" src=\"media\\image%d.jpg\"");

//shape
static CString g_bstr_shape_div									= L"<div style=\"position: absolute; width: 100%; height: 100%;\">\n";
static CString g_string_shape									= _T("<canvas style=\"position:absolute left:0; top:0; width:%dpx; height:%dpx;\" id=\"shape_%d\" width=\"%dpx\" height=\"%dpx\">\n");
static CString g_string_shape_vml								= _T("<v:shape style=\"position:absolute; width:1; height:1\" coordsize=\"1 1\" stroked=\"%ls\" strokecolor=\"#%x\" filled=\"%ls\" fillcolor=\"#%x\" path=\"%ls\" />\n");
static CString g_string_shape_vmlTexture						= _T("<v:shape style=\"position:absolute; width:1; height:1\" coordsize=\"1 1\" stroked=\"%ls\" strokecolor=\"#%x\" filled=\"%ls\" path=\"%ls\"><v:fill src=\"media\\image%d.jpg\" type=\"frame\" opacity=\"%.2lf\"/></v:shape>\n");
static CString g_string_shape_vmlAlpha							= _T("<v:shape style=\"position:absolute; width:1; height:1\" coordsize=\"1 1\" stroked=\"%ls\" strokecolor=\"#%x\" filled=\"%ls\" fillcolor=\"#%x\" path=\"%ls\"><v:fill opacity=\"%.2lf\"/></v:shape>\n");
static CString g_bstr_shape_closecanvas							= L"</canvas>\n";
static CString g_string_rect									= _T("<div class=\"rt\" style=\"border-width: %dpx; background-color: %ls; border-color: %ls; opacity:%.2lf; left: %dpx; top: %dpx; width: %dpx; height: %dpx;\"></div>\n");

// new
static CString g_string_graphic_page							= _T("<canvas id=\"page%d_2D\" width=\"%dpx\" height=\"%dpx\">\n");
static CString g_string_canvas_page_script						= _T("<script type=\"text/javascript\">drawpage%d();</script>\n");
static CString g_string_page_script								= _T("<script type=\"text/javascript\">_tx%d();</script>\n");
static CString g_string_page_method								= _T("function _tx%d(){\n");

static CString g_string_canvas_page_script2						= _T("function drawpage%d()\n{\nvar drawingCanvas = document.getElementById('page%d_2D');\nif(drawingCanvas && drawingCanvas.getContext)\n{\nvar c = drawingCanvas.getContext('2d');\n");
static CString g_bstr_paragraph1								= _T("<p class=\"bp\">");
static CString g_string_paragraph2								= _T("<p class=\"bp\" style=\"font-family: %ls; font-size: %dpx; color:#%x;\">");
static CString g_string_paragraph3								= _T("<p class=\"bp\" style=\"left: %dpx; top: %dpx; font-family: %ls; font-size: %dpx; color:#%x; white-space: nowrap;\">");
static CString g_string_paragraph4								= _T("<p class=\"bp\" style=\"left: %dpx; top: %dpx; width: %dpx; height: %dpx; text-align: %ls; line-height: %d; text-indent: %dpx\">");
static CString g_string_paragraph5								= _T("<p id=\"l%d\" class=\"bp\" style=\"left: %dpx; top: %dpx; font-family: %ls; font-size: %dpx; color:#%x;\" len=\"%d\">");
static CString g_string_paragraph6								= _T("<p id=\"l%d\" class=\"bp\" style=\"left: %dpx; top: %dpx; width: %dpx; font-family: %ls; font-size: %dpx; color:#%x;\">");
static CString g_string_paragraph_style							= _T("<p class=\"bp s%d\" style=\"left: %dpx; top: %dpx; width: %dpx;\">");
static CString g_string_paragraph_style_underline				= _T("<p class=\"bp s%d\" style=\"left: %dpx; top: %dpx; width: %dpx; text-decoration:underline;\">");
static CString g_string_paragraph_style_strike					= _T("<p class=\"bp s%d\" style=\"left: %dpx; top: %dpx; width: %dpx; text-decoration:line-through;\">");
static CString g_string_paragraph_style_underline_strike		= _T("<p class=\"bp s%d\" style=\"left: %dpx; top: %dpx; width: %dpx; text-decoration:underline line-through;\">");

static CString g_string_text_css								= _T("font-family: %ls; font-size: %dpx; color:#%06x; font-style:normal; font-weight:normal;");
static CString g_string_text_css_ib								= _T("font-family: %ls; font-size: %dpx; color:#%06x; font-style:italic; font-weight:bold;");
static CString g_string_text_css_i								= _T("font-family: %ls; font-size: %dpx; color:#%06x; font-style:italic; font-weight:normal;");
static CString g_string_text_css_b								= _T("font-family: %ls; font-size: %dpx; color:#%06x; font-weight:bold; font-style:normal;");
static CString g_string_style_name								= _T(".s%d {");

static CString g_bstr_paragraph_end								= _T("</p>\n");
static CString g_bstr_nobr_start								= _T("<nobr>");
static CString g_bstr_nobr_end									= _T("</nobr>");


static CString g_string_lineFunc								= _T("mt(\"l%d\",%d);");
//static CString g_bstr_lineMeasure								= L"function mt(id,size){var el = document.getElementById(id);if (el.offsetWidth < size){el.style.whiteSpace = \"normal\";el.style.width = size;el.style.height = el.offsetHeight;el.style.textAlign = \"justify\";el.innerHTML += \"<span class=\\\"jf\\\"></span>\";}}";
static CString g_bstr_lineMeasure								= L"function mt(id,size){var el = document.getElementById(id);if (el.offsetWidth < size){el.className=\"pa\";el.style.width=size;}}\n";

static CString g_string_span1									= _T("<span class=\"bt\" style=\"left:%d; top:%d;\">");
static CString g_string_span2									= _T("<span class=\"bt\" style=\"left:%d; top:%d; font-family: %ls; font-size: %dpx; color:#%x;\">");
static CString g_string_span3									= _T("<span class=\"bs\" style=\"font-family: %ls; font-size: %dpx; color:#%x;\">");
static CString g_string_span_style								= _T("<span class=\"s%d\">");
static CString g_string_span_style_underline					= _T("<span class=\"s%d\" style=\"text-decoration:underline;\">");
static CString g_string_span_style_strike						= _T("<span class=\"s%d\" style=\"text-decoration:line-through;\">");
static CString g_string_span_style_underline_strike				= _T("<span class=\"s%d\" style=\"text-decoration:underline line-through;\">");

static CString g_string_span_style_underline2					= _T("<span style=\"text-decoration:underline;\">");
static CString g_string_span_style_strike2						= _T("<span style=\"text-decoration:line-through;\">");
static CString g_string_span_style_underline_strike2			= _T("<span style=\"text-decoration:underline line-through;\">");

static CString g_bstr_span_end									= _T("</span>");

static CString g_bstr_space										= L"&nbsp;";
static CString g_bstr_space2									= L" ";

static CString g_bstr_basicscript								= L"function b(c) { c.beginPath(); }\nfunction m(c,x,y) { c.moveTo(x,y); }\nfunction l(c,x,y) { c.lineTo(x,y); }\nfunction cu(c,x1,y1,x2,y2,x3,y3) { c.bezierCurveTo(x1,y1,x2,y2,x3,y3); }\nfunction s(c) { c.stroke(); }\nfunction f(c) { c.fill(); }\nfunction x(c) { c.closePath(); }\n\n";

