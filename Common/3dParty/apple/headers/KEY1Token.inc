/* C++ code produced by gperf version 3.0.1 */
/* Command-line: gperf --compare-strncmp --enum --null-strings --readonly-tables --language C++ --output-file KEY1Token.inc KEY1Token.gperf  */
/* Computed positions: -k'1,3,6,9,14,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif

#line 10 "KEY1Token.gperf"

#if defined __GNUC__
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

using namespace KEY1Token;
#line 18 "KEY1Token.gperf"
struct Token
{
  const char *name;
  int id;
};
#include <string.h>
/* maximum key range = 602, duplicates = 0 */

class Perfect_Hash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const struct Token *in_word_set (const char *str, unsigned int len);
};

inline unsigned int
Perfect_Hash::hash (register const char *str, register unsigned int len)
{
  static const unsigned short asso_values[] =
    {
      612, 612, 612, 612, 612, 612, 612, 612, 612, 612,
      612, 612, 612, 612, 612, 612, 612, 612, 612, 612,
      612, 612, 612, 612, 612, 612, 612, 612, 612, 612,
      612, 612, 612, 612, 612, 612, 612, 612, 612, 612,
      612, 612, 612, 612, 612, 220, 612,   0, 612, 612,
      612, 612, 612, 612, 612, 612, 612, 612, 612, 612,
      612, 612, 612, 612, 612, 612,   0,   0,   0,   0,
        0,   0,  10, 612, 612, 612,   0, 612,  30,  15,
       55, 612,   5,  60,   5, 612,  10, 612, 612, 612,
      612, 612, 612, 612, 612,   0, 612,  20, 165, 115,
       65,   0, 105, 135, 175,  60,   0,   0,  30, 145,
       10,   5, 155,  10,   5,  30,   5, 200,  15,  20,
        0, 190,   0, 612, 612, 612, 612, 612, 612, 612,
      612, 612, 612, 612, 612, 612, 612, 612, 612, 612,
      612, 612, 612, 612, 612, 612, 612, 612, 612, 612,
      612, 612, 612, 612, 612, 612, 612, 612, 612, 612,
      612, 612, 612, 612, 612, 612, 612, 612, 612, 612,
      612, 612, 612, 612, 612, 612, 612, 612, 612, 612,
      612, 612, 612, 612, 612, 612, 612, 612, 612, 612,
      612, 612, 612, 612, 612, 612, 612, 612, 612, 612,
      612, 612, 612, 612, 612, 612, 612, 612, 612, 612,
      612, 612, 612, 612, 612, 612, 612, 612, 612, 612,
      612, 612, 612, 612, 612, 612, 612, 612, 612, 612,
      612, 612, 612, 612, 612, 612, 612, 612, 612, 612,
      612, 612, 612, 612, 612, 612, 612, 612, 612, 612,
      612, 612, 612, 612, 612, 612
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[13]];
      /*FALLTHROUGH*/
      case 13:
      case 12:
      case 11:
      case 10:
      case 9:
        hval += asso_values[(unsigned char)str[8]];
      /*FALLTHROUGH*/
      case 8:
      case 7:
      case 6:
        hval += asso_values[(unsigned char)str[5]];
      /*FALLTHROUGH*/
      case 5:
      case 4:
      case 3:
        hval += asso_values[(unsigned char)str[2]];
      /*FALLTHROUGH*/
      case 2:
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval + asso_values[(unsigned char)str[len - 1]];
}

const struct Token *
Perfect_Hash::in_word_set (register const char *str, register unsigned int len)
{
  enum
    {
      TOTAL_KEYWORDS = 203,
      MIN_WORD_LENGTH = 1,
      MAX_WORD_LENGTH = 39,
      MIN_HASH_VALUE = 10,
      MAX_HASH_VALUE = 611
    };

  static const struct Token wordlist[] =
    {
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 199 "KEY1Token.gperf"
      {"theme",theme},
      {(char*)0},
#line 211 "KEY1Token.gperf"
      {"tr",tr},
      {(char*)0},
#line 196 "KEY1Token.gperf"
      {"text",text},
#line 207 "KEY1Token.gperf"
      {"title",title},
      {(char*)0},
#line 198 "KEY1Token.gperf"
      {"textbox",textbox},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 65 "KEY1Token.gperf"
      {"element",element},
      {(char*)0},
#line 132 "KEY1Token.gperf"
      {"none",none},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 175 "KEY1Token.gperf"
      {"size",size},
      {(char*)0}, {(char*)0},
#line 209 "KEY1Token.gperf"
      {"tl",tl},
      {(char*)0},
#line 206 "KEY1Token.gperf"
      {"tile",tile},
#line 168 "KEY1Token.gperf"
      {"serie",serie},
      {(char*)0},
#line 221 "KEY1Token.gperf"
      {"version",version},
      {(char*)0},
#line 112 "KEY1Token.gperf"
      {"line",line},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 174 "KEY1Token.gperf"
      {"showZero",showZero},
      {(char*)0},
#line 133 "KEY1Token.gperf"
      {"notes",notes},
      {(char*)0},
#line 188 "KEY1Token.gperf"
      {"stroke-style",stroke_style},
      {(char*)0},
#line 32 "KEY1Token.gperf"
      {"axes",axes},
#line 172 "KEY1Token.gperf"
      {"shape",shape},
      {(char*)0},
#line 187 "KEY1Token.gperf"
      {"stroke-color",stroke_color},
#line 166 "KEY1Token.gperf"
      {"sequence",sequence},
      {(char*)0},
#line 183 "KEY1Token.gperf"
      {"start",start},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 182 "KEY1Token.gperf"
      {"span",span},
#line 185 "KEY1Token.gperf"
      {"steps",steps},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 93 "KEY1Token.gperf"
      {"ident",ident},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 134 "KEY1Token.gperf"
      {"null",null},
#line 197 "KEY1Token.gperf"
      {"text-attributes",text_attributes},
      {(char*)0},
#line 130 "KEY1Token.gperf"
      {"natural-size",natural_size},
      {(char*)0},
#line 131 "KEY1Token.gperf"
      {"node",node},
#line 111 "KEY1Token.gperf"
      {"level",level},
      {(char*)0},
#line 225 "KEY1Token.gperf"
      {"visible",visible},
      {(char*)0}, {(char*)0},
#line 95 "KEY1Token.gperf"
      {"image",image},
      {(char*)0},
#line 171 "KEY1Token.gperf"
      {"shadow-style",shadow_style},
      {(char*)0},
#line 67 "KEY1Token.gperf"
      {"end-color",end_color},
      {(char*)0}, {(char*)0},
#line 208 "KEY1Token.gperf"
      {"titleVisible",titleVisible},
#line 212 "KEY1Token.gperf"
      {"tracks-master",tracks_master},
#line 53 "KEY1Token.gperf"
      {"data",data},
#line 177 "KEY1Token.gperf"
      {"slide",slide},
      {(char*)0}, {(char*)0},
#line 62 "KEY1Token.gperf"
      {"div",div},
#line 195 "KEY1Token.gperf"
      {"tail",tail},
#line 170 "KEY1Token.gperf"
      {"seriesDirection",seriesDirection},
#line 169 "KEY1Token.gperf"
      {"series",series},
      {(char*)0},
#line 162 "KEY1Token.gperf"
      {"relative",relative},
#line 60 "KEY1Token.gperf"
      {"direction",direction},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 27 "KEY1Token.gperf"
      {"altLineVisible",altLineVisible},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 173 "KEY1Token.gperf"
      {"showGrid",showGrid},
#line 33 "KEY1Token.gperf"
      {"axis",axis},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 161 "KEY1Token.gperf"
      {"reference",reference},
#line 114 "KEY1Token.gperf"
      {"line-tail-style",line_tail_style},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 73 "KEY1Token.gperf"
      {"font",font},
      {(char*)0},
#line 137 "KEY1Token.gperf"
      {"offset",offset},
#line 92 "KEY1Token.gperf"
      {"id",id},
      {(char*)0},
#line 160 "KEY1Token.gperf"
      {"rect",rect},
#line 180 "KEY1Token.gperf"
      {"solid",solid},
      {(char*)0}, {(char*)0},
#line 66 "KEY1Token.gperf"
      {"end",end},
#line 77 "KEY1Token.gperf"
      {"font-name",font_name},
      {(char*)0},
#line 159 "KEY1Token.gperf"
      {"radius",radius},
#line 61 "KEY1Token.gperf"
      {"display-name",display_name},
      {(char*)0},
#line 68 "KEY1Token.gperf"
      {"file",file},
      {(char*)0},
#line 45 "KEY1Token.gperf"
      {"center",center},
      {(char*)0}, {(char*)0},
#line 109 "KEY1Token.gperf"
      {"left",left},
      {(char*)0}, {(char*)0},
#line 51 "KEY1Token.gperf"
      {"content",content},
#line 64 "KEY1Token.gperf"
      {"duration",duration},
#line 71 "KEY1Token.gperf"
      {"fill-type",fill_type},
#line 163 "KEY1Token.gperf"
      {"right",right},
#line 139 "KEY1Token.gperf"
      {"orientation",orientation},
      {(char*)0}, {(char*)0},
#line 78 "KEY1Token.gperf"
      {"font-size",font_size},
#line 50 "KEY1Token.gperf"
      {"color",color},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 129 "KEY1Token.gperf"
      {"name",name},
#line 28 "KEY1Token.gperf"
      {"angle",angle},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 215 "KEY1Token.gperf"
      {"type",type},
#line 52 "KEY1Token.gperf"
      {"dash-style",dash_style},
      {(char*)0}, {(char*)0},
#line 82 "KEY1Token.gperf"
      {"gradient",gradient},
#line 56 "KEY1Token.gperf"
      {"dataFormatterPrefix",dataFormatterPrefix},
#line 54 "KEY1Token.gperf"
      {"dataFormatterHasThousandsSeparators",dataFormatterHasThousandsSeparators},
#line 135 "KEY1Token.gperf"
      {"number",number},
#line 38 "KEY1Token.gperf"
      {"br",br},
#line 222 "KEY1Token.gperf"
      {"vertical",vertical},
#line 57 "KEY1Token.gperf"
      {"dataFormatterSuffix",dataFormatterSuffix},
#line 193 "KEY1Token.gperf"
      {"table",table},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 24 "KEY1Token.gperf"
      {"DefaultLegendRelativePosition",DefaultLegendRelativePosition},
      {(char*)0}, {(char*)0},
#line 146 "KEY1Token.gperf"
      {"pattern",pattern},
      {(char*)0},
#line 55 "KEY1Token.gperf"
      {"dataFormatterNumberOfDecimals",dataFormatterNumberOfDecimals},
      {(char*)0}, {(char*)0},
#line 165 "KEY1Token.gperf"
      {"segment",segment},
      {(char*)0},
#line 59 "KEY1Token.gperf"
      {"dict",dict},
      {(char*)0}, {(char*)0},
#line 153 "KEY1Token.gperf"
      {"presentation",presentation},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 35 "KEY1Token.gperf"
      {"bl",bl},
#line 126 "KEY1Token.gperf"
      {"metadata",metadata},
      {(char*)0},
#line 86 "KEY1Token.gperf"
      {"guide",guide},
      {(char*)0},
#line 181 "KEY1Token.gperf"
      {"spacing",spacing},
#line 120 "KEY1Token.gperf"
      {"majorTickPositions",majorTickPositions},
      {(char*)0},
#line 70 "KEY1Token.gperf"
      {"fill-style",fill_style},
      {(char*)0},
#line 118 "KEY1Token.gperf"
      {"lock-aspect-ratio",lock_aspect_ratio},
      {(char*)0},
#line 44 "KEY1Token.gperf"
      {"byte-size",byte_size},
      {(char*)0},
#line 40 "KEY1Token.gperf"
      {"bullet",bullet},
#line 25 "KEY1Token.gperf"
      {"DefaultLegendSize",DefaultLegendSize},
#line 128 "KEY1Token.gperf"
      {"minorTickPositions",minorTickPositions},
      {(char*)0},
#line 202 "KEY1Token.gperf"
      {"tickLabelsAngle",tickLabelsAngle},
#line 127 "KEY1Token.gperf"
      {"middle",middle},
      {(char*)0},
#line 152 "KEY1Token.gperf"
      {"pos",pos},
#line 155 "KEY1Token.gperf"
      {"prototype-data",prototype_data},
#line 31 "KEY1Token.gperf"
      {"array",array},
      {(char*)0},
#line 122 "KEY1Token.gperf"
      {"master-slide",master_slide},
#line 117 "KEY1Token.gperf"
      {"location",location},
#line 176 "KEY1Token.gperf"
      {"size-technique",size_technique},
      {(char*)0},
#line 79 "KEY1Token.gperf"
      {"font-superscript",font_superscript},
#line 138 "KEY1Token.gperf"
      {"opacity",opacity},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 100 "KEY1Token.gperf"
      {"interBarGap",interBarGap},
      {(char*)0}, {(char*)0},
#line 156 "KEY1Token.gperf"
      {"prototype-drawables",prototype_drawables},
      {(char*)0},
#line 58 "KEY1Token.gperf"
      {"description",description},
#line 43 "KEY1Token.gperf"
      {"bullets",bullets},
      {(char*)0},
#line 76 "KEY1Token.gperf"
      {"font-ligatures",font_ligatures},
      {(char*)0},
#line 191 "KEY1Token.gperf"
      {"symbol",symbol},
#line 154 "KEY1Token.gperf"
      {"prototype-bullets",prototype_bullets},
      {(char*)0},
#line 194 "KEY1Token.gperf"
      {"tab-stops",tab_stops},
#line 90 "KEY1Token.gperf"
      {"horizontal",horizontal},
      {(char*)0},
#line 204 "KEY1Token.gperf"
      {"tickLabelsVisible",tickLabelsVisible},
      {(char*)0},
#line 192 "KEY1Token.gperf"
      {"symbolFillMode",symbolFillMode},
#line 74 "KEY1Token.gperf"
      {"font-color",font_color},
      {(char*)0}, {(char*)0},
#line 124 "KEY1Token.gperf"
      {"master-slides",master_slides},
#line 147 "KEY1Token.gperf"
      {"pieSliceOffset",pieSliceOffset},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 42 "KEY1Token.gperf"
      {"bullet-indentation",bullet_indentation},
      {(char*)0}, {(char*)0},
#line 87 "KEY1Token.gperf"
      {"guides",guides},
      {(char*)0}, {(char*)0},
#line 88 "KEY1Token.gperf"
      {"head",head},
#line 226 "KEY1Token.gperf"
      {"width",width},
#line 89 "KEY1Token.gperf"
      {"hidden",hidden},
      {(char*)0}, {(char*)0},
#line 46 "KEY1Token.gperf"
      {"character",character},
#line 69 "KEY1Token.gperf"
      {"fill-color",fill_color},
#line 81 "KEY1Token.gperf"
      {"g",g},
#line 75 "KEY1Token.gperf"
      {"font-kerning",font_kerning},
      {(char*)0},
#line 103 "KEY1Token.gperf"
      {"justified",justified},
      {(char*)0},
#line 116 "KEY1Token.gperf"
      {"lineVisible",lineVisible},
#line 107 "KEY1Token.gperf"
      {"labelVisible",labelVisible},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 119 "KEY1Token.gperf"
      {"locked",locked},
#line 189 "KEY1Token.gperf"
      {"stroke-width",stroke_width},
      {(char*)0},
#line 200 "KEY1Token.gperf"
      {"thumbnail",thumbnail},
#line 201 "KEY1Token.gperf"
      {"thumbnails",thumbnails},
#line 190 "KEY1Token.gperf"
      {"styles",styles},
      {(char*)0}, {(char*)0},
#line 136 "KEY1Token.gperf"
      {"numberOfPoints",numberOfPoints},
#line 49 "KEY1Token.gperf"
      {"chartFrame",chartFrame},
#line 167 "KEY1Token.gperf"
      {"sequence-bullet-style",sequence_bullet_style},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 214 "KEY1Token.gperf"
      {"transition-style",transition_style},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 110 "KEY1Token.gperf"
      {"legend",legend},
#line 148 "KEY1Token.gperf"
      {"pieSlicePercentVisible",pieSlicePercentVisible},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 47 "KEY1Token.gperf"
      {"character-bullet-style",character_bullet_style},
      {(char*)0},
#line 213 "KEY1Token.gperf"
      {"transformation",transformation},
#line 224 "KEY1Token.gperf"
      {"visibility",visibility},
#line 186 "KEY1Token.gperf"
      {"string",string},
      {(char*)0},
#line 39 "KEY1Token.gperf"
      {"buildChunkingStyle",buildChunkingStyle},
      {(char*)0}, {(char*)0},
#line 184 "KEY1Token.gperf"
      {"start-color",start_color},
      {(char*)0},
#line 210 "KEY1Token.gperf"
      {"top",top},
#line 63 "KEY1Token.gperf"
      {"drawables",drawables},
#line 179 "KEY1Token.gperf"
      {"slide-size",slide_size},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 113 "KEY1Token.gperf"
      {"line-head-style",line_head_style},
#line 157 "KEY1Token.gperf"
      {"prototype-plugin",prototype_plugin},
      {(char*)0}, {(char*)0},
#line 80 "KEY1Token.gperf"
      {"font-underline",font_underline},
      {(char*)0},
#line 97 "KEY1Token.gperf"
      {"image-scale",image_scale},
      {(char*)0},
#line 106 "KEY1Token.gperf"
      {"labelPosition",labelPosition},
      {(char*)0},
#line 96 "KEY1Token.gperf"
      {"image-data",image_data},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 145 "KEY1Token.gperf"
      {"path",path},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 34 "KEY1Token.gperf"
      {"background-fill-style",background_fill_style},
#line 158 "KEY1Token.gperf"
      {"prototype-plugins",prototype_plugins},
      {(char*)0}, {(char*)0},
#line 123 "KEY1Token.gperf"
      {"master-slide-id",master_slide_id},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 178 "KEY1Token.gperf"
      {"slide-list",slide_list},
#line 121 "KEY1Token.gperf"
      {"marker-type",marker_type},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 91 "KEY1Token.gperf"
      {"http://developer.apple.com/schemas/APXL",NS_URI_KEY},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 98 "KEY1Token.gperf"
      {"image-bullet-style",image_bullet_style},
#line 30 "KEY1Token.gperf"
      {"application-version",application_version},
      {(char*)0},
#line 149 "KEY1Token.gperf"
      {"plugin",plugin},
#line 151 "KEY1Token.gperf"
      {"point_at_top",point_at_top},
#line 104 "KEY1Token.gperf"
      {"key",key},
      {(char*)0}, {(char*)0},
#line 29 "KEY1Token.gperf"
      {"application-name",application_name},
      {(char*)0},
#line 223 "KEY1Token.gperf"
      {"vertical-alignment",vertical_alignment},
#line 83 "KEY1Token.gperf"
      {"gradient-angle",gradient_angle},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 144 "KEY1Token.gperf"
      {"paragraph-tail-indent",paragraph_tail_indent},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 142 "KEY1Token.gperf"
      {"paragraph-first-line-indent",paragraph_first_line_indent},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 72 "KEY1Token.gperf"
      {"floating-content",floating_content},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 150 "KEY1Token.gperf"
      {"plugin-data",plugin_data},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 36 "KEY1Token.gperf"
      {"body",body},
      {(char*)0}, {(char*)0},
#line 41 "KEY1Token.gperf"
      {"bullet-characters",bullet_characters},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 143 "KEY1Token.gperf"
      {"paragraph-head-indent",paragraph_head_indent},
      {(char*)0}, {(char*)0},
#line 99 "KEY1Token.gperf"
      {"inherited",inherited},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 125 "KEY1Token.gperf"
      {"match-point",match_point},
      {(char*)0},
#line 216 "KEY1Token.gperf"
      {"ui-state",ui_state},
#line 102 "KEY1Token.gperf"
      {"is-filled",is_filled},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 115 "KEY1Token.gperf"
      {"lineOpacity",lineOpacity},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 37 "KEY1Token.gperf"
      {"bottom",bottom},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 140 "KEY1Token.gperf"
      {"page-number",page_number},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 205 "KEY1Token.gperf"
      {"time-stamp",time_stamp},
      {(char*)0},
#line 203 "KEY1Token.gperf"
      {"tickLabelsOpacity",tickLabelsOpacity},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 94 "KEY1Token.gperf"
      {"id-ref",id_ref},
      {(char*)0}, {(char*)0},
#line 141 "KEY1Token.gperf"
      {"paragraph-alignment",paragraph_alignment},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 164 "KEY1Token.gperf"
      {"scale-to-fit",scale_to_fit},
      {(char*)0},
#line 101 "KEY1Token.gperf"
      {"interSeriesGap",interSeriesGap},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 219 "KEY1Token.gperf"
      {"userMaximum",userMaximum},
      {(char*)0}, {(char*)0},
#line 217 "KEY1Token.gperf"
      {"useUserMaximum",useUserMaximum},
#line 108 "KEY1Token.gperf"
      {"layerElementsForShadowing",layerElementsForShadowing},
      {(char*)0},
#line 105 "KEY1Token.gperf"
      {"labelOpacity",labelOpacity},
      {(char*)0}, {(char*)0},
#line 48 "KEY1Token.gperf"
      {"chart-prototype",chart_prototype},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 220 "KEY1Token.gperf"
      {"userMinimum",userMinimum},
      {(char*)0}, {(char*)0},
#line 218 "KEY1Token.gperf"
      {"useUserMinimum",useUserMinimum},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 26 "KEY1Token.gperf"
      {"altLineOpacity",altLineOpacity},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 85 "KEY1Token.gperf"
      {"grow-horizontally",grow_horizontally},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 84 "KEY1Token.gperf"
      {"gridOpacity",gridOpacity}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register const char *s = wordlist[key].name;

          if (s && *str == *s && !strncmp (str + 1, s + 1, len - 1) && s[len] == '\0')
            return &wordlist[key];
        }
    }
  return 0;
}
#line 227 "KEY1Token.gperf"

