/* C++ code produced by gperf version 3.0.1 */
/* Command-line: gperf --compare-strncmp --enum --null-strings --readonly-tables --language C++ --output-file IWORKToken.inc IWORKToken.gperf  */
/* Computed positions: -k'1-2,4-8,11,13,22,24,$' */

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

#line 10 "IWORKToken.gperf"

#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

using namespace IWORKToken;
#line 18 "IWORKToken.gperf"
struct Token
{
  const char *name;
  int id;
};
#include <string.h>
/* maximum key range = 4146, duplicates = 0 */

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
      4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147,
      4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147,
      4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147,
      4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147,
      4147, 4147, 4147, 4147, 4147,    5,   25,    5,   55,  115,
       125,   10,   15,   25,   40,   20,    5,   30,    5, 4147,
      4147, 4147, 4147, 4147, 4147,   40,    5,   15,    0,    5,
         0,   30,   55,  180, 4147, 4147,   40,   15,    0,    0,
         5,    0,    5,    5,    0,   35,   25,    0,   30,   85,
      4147, 4147, 4147, 4147, 4147,    0, 4147,   75,  595,   45,
       495,    0,    0,  475,  410,    5,  605,   50,  235,   30,
        40,   35,  650,   10,  130,   15,    5,  100,  780,   90,
       180,  920,   10, 4147, 4147, 4147, 4147, 4147, 4147, 4147,
      4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147,
      4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147,
      4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147,
      4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147,
      4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147,
      4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147,
      4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147,
      4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147,
      4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147,
      4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147,
      4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147,
      4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147, 4147,
      4147, 4147, 4147, 4147, 4147, 4147, 4147
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[23]];
      /*FALLTHROUGH*/
      case 23:
      case 22:
        hval += asso_values[(unsigned char)str[21]];
      /*FALLTHROUGH*/
      case 21:
      case 20:
      case 19:
      case 18:
      case 17:
      case 16:
      case 15:
      case 14:
      case 13:
        hval += asso_values[(unsigned char)str[12]];
      /*FALLTHROUGH*/
      case 12:
      case 11:
        hval += asso_values[(unsigned char)str[10]];
      /*FALLTHROUGH*/
      case 10:
      case 9:
      case 8:
        hval += asso_values[(unsigned char)str[7]];
      /*FALLTHROUGH*/
      case 7:
        hval += asso_values[(unsigned char)str[6]];
      /*FALLTHROUGH*/
      case 6:
        hval += asso_values[(unsigned char)str[5]];
      /*FALLTHROUGH*/
      case 5:
        hval += asso_values[(unsigned char)str[4]];
      /*FALLTHROUGH*/
      case 4:
        hval += asso_values[(unsigned char)str[3]+1];
      /*FALLTHROUGH*/
      case 3:
      case 2:
        hval += asso_values[(unsigned char)str[1]];
      /*FALLTHROUGH*/
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
      TOTAL_KEYWORDS = 655,
      MIN_WORD_LENGTH = 1,
      MAX_WORD_LENGTH = 44,
      MIN_HASH_VALUE = 1,
      MAX_HASH_VALUE = 4146
    };

  static const struct Token wordlist[] =
    {
      {(char*)0},
#line 229 "IWORKToken.gperf"
      {"f",f},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 566 "IWORKToken.gperf"
      {"t",t},
      {(char*)0},
#line 247 "IWORKToken.gperf"
      {"fit",fit},
#line 635 "IWORKToken.gperf"
      {"tile",tile},
      {(char*)0}, {(char*)0},
#line 528 "IWORKToken.gperf"
      {"sf",sf},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 534 "IWORKToken.gperf"
      {"size",size},
#line 244 "IWORKToken.gperf"
      {"first",first},
      {(char*)0},
#line 548 "IWORKToken.gperf"
      {"st",st},
      {(char*)0}, {(char*)0},
#line 634 "IWORKToken.gperf"
      {"tight",tight},
#line 517 "IWORKToken.gperf"
      {"s",s},
#line 290 "IWORKToken.gperf"
      {"fs",fs},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 449 "IWORKToken.gperf"
      {"of",of},
#line 254 "IWORKToken.gperf"
      {"fmt",fmt},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 636 "IWORKToken.gperf"
      {"title",title},
      {(char*)0},
#line 154 "IWORKToken.gperf"
      {"cf",cf},
      {(char*)0}, {(char*)0},
#line 550 "IWORKToken.gperf"
      {"start",start},
#line 425 "IWORKToken.gperf"
      {"mf-ref",mf_ref},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 560 "IWORKToken.gperf"
      {"style",style},
#line 450 "IWORKToken.gperf"
      {"offset",offset},
#line 196 "IWORKToken.gperf"
      {"ct",ct},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 409 "IWORKToken.gperf"
      {"m",m},
#line 253 "IWORKToken.gperf"
      {"fm",fm},
      {(char*)0}, {(char*)0},
#line 554 "IWORKToken.gperf"
      {"stops",stops},
#line 155 "IWORKToken.gperf"
      {"cf-ref",cf_ref},
      {(char*)0}, {(char*)0},
#line 423 "IWORKToken.gperf"
      {"mode",mode},
      {(char*)0},
#line 447 "IWORKToken.gperf"
      {"o",o},
#line 255 "IWORKToken.gperf"
      {"fo",fo},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 436 "IWORKToken.gperf"
      {"none",none},
      {(char*)0},
#line 428 "IWORKToken.gperf"
      {"n",n},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 231 "IWORKToken.gperf"
      {"false",false_},
#line 562 "IWORKToken.gperf"
      {"styles",styles},
#line 540 "IWORKToken.gperf"
      {"so",so},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 133 "IWORKToken.gperf"
      {"c",c},
      {(char*)0},
#line 529 "IWORKToken.gperf"
      {"sfa",sfa},
      {(char*)0},
#line 520 "IWORKToken.gperf"
      {"scale",scale},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 355 "IWORKToken.gperf"
      {"k",k},
      {(char*)0},
#line 435 "IWORKToken.gperf"
      {"nsc",nsc},
#line 604 "IWORKToken.gperf"
      {"tabs",tabs},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 24 "IWORKToken.gperf"
      {"0",_0},
      {(char*)0},
#line 98 "IWORKToken.gperf"
      {"amt",amt},
#line 623 "IWORKToken.gperf"
      {"text",text},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 429 "IWORKToken.gperf"
      {"name",name},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 191 "IWORKToken.gperf"
      {"count",count},
      {(char*)0},
#line 432 "IWORKToken.gperf"
      {"nc",nc},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 620 "IWORKToken.gperf"
      {"tailSize",tailSize},
#line 647 "IWORKToken.gperf"
      {"true",true_},
      {(char*)0},
#line 353 "IWORKToken.gperf"
      {"italic",italic},
#line 516 "IWORKToken.gperf"
      {"rt",rt},
      {(char*)0},
#line 341 "IWORKToken.gperf"
      {"increment",increment},
      {(char*)0},
#line 573 "IWORKToken.gperf"
      {"table-style",table_style},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 99 "IWORKToken.gperf"
      {"angle",angle},
#line 87 "IWORKToken.gperf"
      {"a",a},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 508 "IWORKToken.gperf"
      {"right",right},
      {(char*)0}, {(char*)0},
#line 232 "IWORKToken.gperf"
      {"filename",filename},
#line 542 "IWORKToken.gperf"
      {"sort",sort},
#line 245 "IWORKToken.gperf"
      {"firstLineIndent",firstLineIndent},
      {(char*)0}, {(char*)0},
#line 260 "IWORKToken.gperf"
      {"fontSize",fontSize},
      {(char*)0}, {(char*)0},
#line 267 "IWORKToken.gperf"
      {"format",format},
      {(char*)0}, {(char*)0},
#line 549 "IWORKToken.gperf"
      {"star",star},
      {(char*)0}, {(char*)0},
#line 530 "IWORKToken.gperf"
      {"sfclass",sfclass},
      {(char*)0},
#line 622 "IWORKToken.gperf"
      {"technique",technique},
#line 285 "IWORKToken.gperf"
      {"frame",frame},
#line 278 "IWORKToken.gperf"
      {"format-type",format_type},
#line 176 "IWORKToken.gperf"
      {"comment",comment},
#line 234 "IWORKToken.gperf"
      {"fill-ref",fill_ref},
#line 569 "IWORKToken.gperf"
      {"table-cell-ref",table_cell_ref},
#line 343 "IWORKToken.gperf"
      {"inflection",inflection},
#line 669 "IWORKToken.gperf"
      {"w",w},
#line 31 "IWORKToken.gperf"
      {"ID",ID},
      {(char*)0}, {(char*)0},
#line 32 "IWORKToken.gperf"
      {"IDREF",IDREF},
#line 570 "IWORKToken.gperf"
      {"table-cell-style",table_cell_style},
      {(char*)0}, {(char*)0},
#line 515 "IWORKToken.gperf"
      {"rows",rows},
#line 571 "IWORKToken.gperf"
      {"table-cell-style-ref",table_cell_style_ref},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 177 "IWORKToken.gperf"
      {"connection-line",connection_line},
#line 179 "IWORKToken.gperf"
      {"connection-style",connection_style},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 180 "IWORKToken.gperf"
      {"connection-style-ref",connection_style_ref},
      {(char*)0}, {(char*)0},
#line 677 "IWORKToken.gperf"
      {"xsi",xsi},
      {(char*)0},
#line 572 "IWORKToken.gperf"
      {"table-info",table_info},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 340 "IWORKToken.gperf"
      {"implicit-format-type",implicit_format_type},
#line 274 "IWORKToken.gperf"
      {"format-name",format_name},
#line 510 "IWORKToken.gperf"
      {"rn",rn},
      {(char*)0}, {(char*)0},
#line 521 "IWORKToken.gperf"
      {"scale-with-text",scale_with_text},
      {(char*)0},
#line 454 "IWORKToken.gperf"
      {"outline",outline},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 263 "IWORKToken.gperf"
      {"footnote",footnote},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 605 "IWORKToken.gperf"
      {"tabs-ref",tabs_ref},
      {(char*)0}, {(char*)0},
#line 25 "IWORKToken.gperf"
      {"1",_1},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 225 "IWORKToken.gperf"
      {"extent",extent},
#line 183 "IWORKToken.gperf"
      {"content",content},
      {(char*)0},
#line 266 "IWORKToken.gperf"
      {"footnotes",footnotes},
      {(char*)0}, {(char*)0},
#line 522 "IWORKToken.gperf"
      {"section",section},
      {(char*)0},
#line 385 "IWORKToken.gperf"
      {"line",line},
      {(char*)0},
#line 646 "IWORKToken.gperf"
      {"transition-attributes",transition_attributes},
      {(char*)0},
#line 259 "IWORKToken.gperf"
      {"fontName",fontName},
      {(char*)0},
#line 645 "IWORKToken.gperf"
      {"transition",transition},
      {(char*)0},
#line 184 "IWORKToken.gperf"
      {"content-size",content_size},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 512 "IWORKToken.gperf"
      {"row",row},
      {(char*)0}, {(char*)0},
#line 496 "IWORKToken.gperf"
      {"r",r},
      {(char*)0}, {(char*)0},
#line 242 "IWORKToken.gperf"
      {"filterset",filterset},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 241 "IWORKToken.gperf"
      {"filters",filters},
      {(char*)0},
#line 233 "IWORKToken.gperf"
      {"fill",fill},
#line 29 "IWORKToken.gperf"
      {"1347307366",_1347307366},
      {(char*)0}, {(char*)0},
#line 618 "IWORKToken.gperf"
      {"tailPositionX",tailPositionX},
#line 152 "IWORKToken.gperf"
      {"cell-text",cell_text},
#line 574 "IWORKToken.gperf"
      {"table-style-ref",table_style_ref},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 537 "IWORKToken.gperf"
      {"slide-style",slide_style},
#line 187 "IWORKToken.gperf"
      {"core-image-filter-descriptor-ref",core_image_filter_descriptor_ref},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 188 "IWORKToken.gperf"
      {"core-image-filter-info",core_image_filter_info},
      {(char*)0}, {(char*)0},
#line 422 "IWORKToken.gperf"
      {"miter",miter},
#line 275 "IWORKToken.gperf"
      {"format-negative-style",format_negative_style},
#line 262 "IWORKToken.gperf"
      {"footers",footers},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 146 "IWORKToken.gperf"
      {"cell-coordinates",cell_coordinates},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 438 "IWORKToken.gperf"
      {"num-footer-rows",num_footer_rows},
      {(char*)0},
#line 616 "IWORKToken.gperf"
      {"tailAtCenter",tailAtCenter},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 446 "IWORKToken.gperf"
      {"numrows", numrows},
#line 169 "IWORKToken.gperf"
      {"col",col},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 192 "IWORKToken.gperf"
      {"crbr",crbr},
      {(char*)0}, {(char*)0},
#line 356 "IWORKToken.gperf"
      {"keepLinesTogether",keepLinesTogether},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 218 "IWORKToken.gperf"
      {"element",element},
      {(char*)0},
#line 561 "IWORKToken.gperf"
      {"style-run",style_run},
#line 603 "IWORKToken.gperf"
      {"tableVectorStyle-ref",tableVectorStyle_ref},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 144 "IWORKToken.gperf"
      {"cell-comment-drawable-info",cell_comment_drawable_info},
      {(char*)0}, {(char*)0},
#line 382 "IWORKToken.gperf"
      {"left",left},
      {(char*)0},
#line 194 "IWORKToken.gperf"
      {"custom",custom},
#line 279 "IWORKToken.gperf"
      {"format-use-accounting-style",format_use_accounting_style},
      {(char*)0},
#line 615 "IWORKToken.gperf"
      {"tail",tail},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 265 "IWORKToken.gperf"
      {"footnotebr",footnotebr},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 675 "IWORKToken.gperf"
      {"x",x},
#line 287 "IWORKToken.gperf"
      {"frame-w",frame_w},
      {(char*)0}, {(char*)0},
#line 256 "IWORKToken.gperf"
      {"followingLayoutStyle",followingLayoutStyle},
#line 601 "IWORKToken.gperf"
      {"tableVectorBegin",tableVectorBegin},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 444 "IWORKToken.gperf"
      {"number-format-ref",number_format_ref},
#line 443 "IWORKToken.gperf"
      {"number-format",number_format},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 26 "IWORKToken.gperf"
      {"1246774599",_1246774599},
      {(char*)0},
#line 115 "IWORKToken.gperf"
      {"authors",authors},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 619 "IWORKToken.gperf"
      {"tailPositionY",tailPositionY},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 415 "IWORKToken.gperf"
      {"mark",mark},
#line 271 "IWORKToken.gperf"
      {"format-currency-code",format_currency_code},
#line 189 "IWORKToken.gperf"
      {"core-image-filter-info-ref",core_image_filter_info_ref},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 261 "IWORKToken.gperf"
      {"footer",footer},
      {(char*)0},
#line 264 "IWORKToken.gperf"
      {"footnote-mark",footnote_mark},
      {(char*)0},
#line 568 "IWORKToken.gperf"
      {"table-cell",table_cell},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 437 "IWORKToken.gperf"
      {"null",null},
#line 342 "IWORKToken.gperf"
      {"index",index},
#line 153 "IWORKToken.gperf"
      {"center",center},
      {(char*)0},
#line 186 "IWORKToken.gperf"
      {"core-image-filter-descriptor",core_image_filter_descriptor},
      {(char*)0}, {(char*)0},
#line 582 "IWORKToken.gperf"
      {"tableCellMinXSide-ref",tableCellMinXSide_ref},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 580 "IWORKToken.gperf"
      {"tableCellContent",tableCellContent},
#line 330 "IWORKToken.gperf"
      {"ht",ht},
      {(char*)0},
#line 404 "IWORKToken.gperf"
      {"lnbr",lnbr},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 28 "IWORKToken.gperf"
      {"1346651680",_1346651680},
#line 518 "IWORKToken.gperf"
      {"scalar",scalar},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 276 "IWORKToken.gperf"
      {"format-show-thousands-separator",format_show_thousands_separator},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 531 "IWORKToken.gperf"
      {"shape",shape},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 337 "IWORKToken.gperf"
      {"image",image},
#line 441 "IWORKToken.gperf"
      {"number",number},
#line 190 "IWORKToken.gperf"
      {"cornerRadius",cornerRadius},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 565 "IWORKToken.gperf"
      {"superscript",superscript},
#line 523 "IWORKToken.gperf"
      {"sectionstyle",sectionstyle},
#line 91 "IWORKToken.gperf"
      {"animationAuto",animationAuto},
      {(char*)0}, {(char*)0},
#line 551 "IWORKToken.gperf"
      {"start-index",start_index},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 524 "IWORKToken.gperf"
      {"sectionstyle-ref",sectionstyle_ref},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 584 "IWORKToken.gperf"
      {"tableCellMinYSide-ref",tableCellMinYSide_ref},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 374 "IWORKToken.gperf"
      {"layers",layers},
#line 502 "IWORKToken.gperf"
      {"restart-list",restart_list},
      {(char*)0}, {(char*)0},
#line 30 "IWORKToken.gperf"
      {"1414088262",_1414088262},
      {(char*)0},
#line 329 "IWORKToken.gperf"
      {"ho",ho},
#line 538 "IWORKToken.gperf"
      {"slider-orientation",slider_orientation},
      {(char*)0},
#line 27 "IWORKToken.gperf"
      {"1299148630",_1299148630},
#line 583 "IWORKToken.gperf"
      {"tableCellMaxXSide-ref",tableCellMaxXSide_ref},
#line 536 "IWORKToken.gperf"
      {"sl",sl},
      {(char*)0}, {(char*)0},
#line 563 "IWORKToken.gperf"
      {"stylesheet",stylesheet},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 564 "IWORKToken.gperf"
      {"stylesheet-ref",stylesheet_ref},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 328 "IWORKToken.gperf"
      {"hc",hc},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 621 "IWORKToken.gperf"
      {"target",target},
#line 94 "IWORKToken.gperf"
      {"animationDuration",animationDuration},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 165 "IWORKToken.gperf"
      {"chart-style",chart_style},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 581 "IWORKToken.gperf"
      {"tableCellFormula",tableCellFormula},
      {(char*)0},
#line 163 "IWORKToken.gperf"
      {"chart-series-style",chart_series_style},
      {(char*)0},
#line 600 "IWORKToken.gperf"
      {"tableVectorAxis",tableVectorAxis},
      {(char*)0},
#line 164 "IWORKToken.gperf"
      {"chart-series-style-ref",chart_series_style_ref},
      {(char*)0},
#line 599 "IWORKToken.gperf"
      {"tableVectorArrayColumnVectors",tableVectorArrayColumnVectors},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 195 "IWORKToken.gperf"
      {"custom-space-color",custom_space_color},
#line 391 "IWORKToken.gperf"
      {"link",link},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 220 "IWORKToken.gperf"
      {"end",end},
      {(char*)0}, {(char*)0},
#line 585 "IWORKToken.gperf"
      {"tableCellMaxYSide-ref",tableCellMaxYSide_ref},
#line 288 "IWORKToken.gperf"
      {"frame-x",frame_x},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 588 "IWORKToken.gperf"
      {"tableCellValue",tableCellValue},
#line 335 "IWORKToken.gperf"
      {"ident",ident},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 360 "IWORKToken.gperf"
      {"kind",kind},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 651 "IWORKToken.gperf"
      {"underline",underline},
      {(char*)0},
#line 598 "IWORKToken.gperf"
      {"tableVectorArrayRowVectors",tableVectorArrayRowVectors},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 280 "IWORKToken.gperf"
      {"formula",formula},
      {(char*)0}, {(char*)0},
#line 159 "IWORKToken.gperf"
      {"chart-info",chart_info},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 372 "IWORKToken.gperf"
      {"layer",layer},
      {(char*)0},
#line 317 "IWORKToken.gperf"
      {"headers",headers},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 387 "IWORKToken.gperf"
      {"linear",linear},
      {(char*)0}, {(char*)0},
#line 373 "IWORKToken.gperf"
      {"layer-ref",layer_ref},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 339 "IWORKToken.gperf"
      {"image-ref",image_ref},
#line 236 "IWORKToken.gperf"
      {"filterClassName",filterClassName},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 258 "IWORKToken.gperf"
      {"fontColor",fontColor},
      {(char*)0},
#line 631 "IWORKToken.gperf"
      {"texture-fill-ref",texture_fill_ref},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 161 "IWORKToken.gperf"
      {"chart-name",chart_name},
#line 414 "IWORKToken.gperf"
      {"margin",margin},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 587 "IWORKToken.gperf"
      {"tableCellStyle-ref",tableCellStyle_ref},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 148 "IWORKToken.gperf"
      {"cell-storage",cell_storage},
#line 158 "IWORKToken.gperf"
      {"chart-column_names",chart_column_names},
#line 204 "IWORKToken.gperf"
      {"date-time",date_time},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 433 "IWORKToken.gperf"
      {"ncoc",ncoc},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 625 "IWORKToken.gperf"
      {"text-cell",text_cell},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 208 "IWORKToken.gperf"
      {"direction",direction},
#line 526 "IWORKToken.gperf"
      {"self-contained-movie",self_contained_movie},
#line 338 "IWORKToken.gperf"
      {"image-media",image_media},
      {(char*)0},
#line 257 "IWORKToken.gperf"
      {"followingParagraphStyle",followingParagraphStyle},
      {(char*)0},
#line 166 "IWORKToken.gperf"
      {"chart-style-ref",chart_style_ref},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 628 "IWORKToken.gperf"
      {"text-storage",text_storage},
#line 36 "IWORKToken.gperf"
      {"Series_3",Series_3},
#line 589 "IWORKToken.gperf"
      {"tableInfoTable",tableInfoTable},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 408 "IWORKToken.gperf"
      {"lower-roman",lower_roman},
      {(char*)0},
#line 37 "IWORKToken.gperf"
      {"Series_4",Series_4},
#line 147 "IWORKToken.gperf"
      {"cell-date",cell_date},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 40 "IWORKToken.gperf"
      {"Series_7",Series_7},
      {(char*)0},
#line 178 "IWORKToken.gperf"
      {"connection-path",connection_path},
#line 316 "IWORKToken.gperf"
      {"header",header},
#line 357 "IWORKToken.gperf"
      {"keepWithNext",keepWithNext},
#line 567 "IWORKToken.gperf"
      {"tab",tab},
      {(char*)0},
#line 250 "IWORKToken.gperf"
      {"flags",flags},
      {(char*)0}, {(char*)0},
#line 38 "IWORKToken.gperf"
      {"Series_5",Series_5},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 228 "IWORKToken.gperf"
      {"external-text-wrap-ref",external_text_wrap_ref},
      {(char*)0}, {(char*)0},
#line 336 "IWORKToken.gperf"
      {"identifier",identifier},
      {(char*)0}, {(char*)0},
#line 641 "IWORKToken.gperf"
      {"top",top},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 214 "IWORKToken.gperf"
      {"du",du},
#line 118 "IWORKToken.gperf"
      {"baseSize",baseSize},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 487 "IWORKToken.gperf"
      {"pos",pos},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 484 "IWORKToken.gperf"
      {"pm",pm},
#line 39 "IWORKToken.gperf"
      {"Series_6",Series_6},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 354 "IWORKToken.gperf"
      {"join",join},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 558 "IWORKToken.gperf"
      {"stroke",stroke},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 145 "IWORKToken.gperf"
      {"cell-comment-mapping",cell_comment_mapping},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 485 "IWORKToken.gperf"
      {"point",point},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 131 "IWORKToken.gperf"
      {"bullet",bullet},
      {(char*)0},
#line 33 "IWORKToken.gperf"
      {"Series_0",Series_0},
#line 546 "IWORKToken.gperf"
      {"span",span},
#line 162 "IWORKToken.gperf"
      {"chart-row_names",chart_row_names},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 333 "IWORKToken.gperf"
      {"http://www.w3.org/2001/XMLSchema-instance",NS_URI_XSI},
#line 371 "IWORKToken.gperf"
      {"largest",largest},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 596 "IWORKToken.gperf"
      {"tableModelVectors",tableModelVectors},
#line 284 "IWORKToken.gperf"
      {"fraction",fraction},
      {(char*)0}, {(char*)0},
#line 201 "IWORKToken.gperf"
      {"date-format",date_format},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 553 "IWORKToken.gperf"
      {"stop-index",stop_index},
#line 268 "IWORKToken.gperf"
      {"format-base",format_base},
      {(char*)0}, {(char*)0},
#line 181 "IWORKToken.gperf"
      {"container-hint",container_hint},
      {(char*)0},
#line 272 "IWORKToken.gperf"
      {"format-decimal-places",format_decimal_places},
      {(char*)0},
#line 139 "IWORKToken.gperf"
      {"cap",cap},
#line 579 "IWORKToken.gperf"
      {"tableCellArrayCellsByRow",tableCellArrayCellsByRow},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 453 "IWORKToken.gperf"
      {"other-datas",other_datas},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 418 "IWORKToken.gperf"
      {"media",media},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 246 "IWORKToken.gperf"
      {"firstPageMaster",firstPageMaster},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 109 "IWORKToken.gperf"
      {"aspectRatioLocked",aspectRatioLocked},
      {(char*)0},
#line 132 "IWORKToken.gperf"
      {"butt",butt},
#line 88 "IWORKToken.gperf"
      {"align",align},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 221 "IWORKToken.gperf"
      {"end-point",end_point},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 211 "IWORKToken.gperf"
      {"double",double_},
#line 434 "IWORKToken.gperf"
      {"neither",neither},
#line 101 "IWORKToken.gperf"
      {"angle-gradient-ref",angle_gradient_ref},
#line 100 "IWORKToken.gperf"
      {"angle-gradient",angle_gradient},
      {(char*)0},
#line 314 "IWORKToken.gperf"
      {"h",h},
#line 630 "IWORKToken.gperf"
      {"texture-fill",texture_fill},
      {(char*)0},
#line 282 "IWORKToken.gperf"
      {"formula-chart-model",formula_chart_model},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 126 "IWORKToken.gperf"
      {"bottom",bottom},
      {(char*)0}, {(char*)0},
#line 53 "IWORKToken.gperf"
      {"SFTCellStylePropertyDateTimeFormat",SFTCellStylePropertyDateTimeFormat},
      {(char*)0}, {(char*)0},
#line 474 "IWORKToken.gperf"
      {"parent-ident",parent_ident},
#line 160 "IWORKToken.gperf"
      {"chart-model-object",chart_model_object},
      {(char*)0},
#line 461 "IWORKToken.gperf"
      {"page-start",page_start},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 90 "IWORKToken.gperf"
      {"alignment",alignment},
      {(char*)0},
#line 442 "IWORKToken.gperf"
      {"number-cell",number_cell},
      {(char*)0}, {(char*)0},
#line 156 "IWORKToken.gperf"
      {"characterstyle",characterstyle},
      {(char*)0}, {(char*)0},
#line 668 "IWORKToken.gperf"
      {"vo",vo},
#line 157 "IWORKToken.gperf"
      {"characterstyle-ref",characterstyle_ref},
      {(char*)0}, {(char*)0},
#line 648 "IWORKToken.gperf"
      {"tscale",tscale},
#line 128 "IWORKToken.gperf"
      {"br",br},
      {(char*)0},
#line 602 "IWORKToken.gperf"
      {"tableVectorEnd",tableVectorEnd},
      {(char*)0},
#line 56 "IWORKToken.gperf"
      {"SFTCellStylePropertyLayoutStyle",SFTCellStylePropertyLayoutStyle},
      {(char*)0},
#line 34 "IWORKToken.gperf"
      {"Series_1",Series_1},
#line 629 "IWORKToken.gperf"
      {"textBackground",textBackground},
#line 559 "IWORKToken.gperf"
      {"stroke-ref",stroke_ref},
#line 424 "IWORKToken.gperf"
      {"movie-media",movie_media},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 462 "IWORKToken.gperf"
      {"pair",pair},
      {(char*)0},
#line 407 "IWORKToken.gperf"
      {"lower-alpha",lower_alpha},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 509 "IWORKToken.gperf"
      {"rightIndent",rightIndent},
#line 578 "IWORKToken.gperf"
      {"tableCellArrayCellsByColumn",tableCellArrayCellsByColumn},
#line 35 "IWORKToken.gperf"
      {"Series_2",Series_2},
#line 673 "IWORKToken.gperf"
      {"wrap",wrap},
#line 105 "IWORKToken.gperf"
      {"annotation-field-ref",annotation_field_ref},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 202 "IWORKToken.gperf"
      {"date-format-ref",date_format_ref},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 351 "IWORKToken.gperf"
      {"insertion-point",insertion_point},
#line 106 "IWORKToken.gperf"
      {"annotations",annotations},
#line 555 "IWORKToken.gperf"
      {"stretch",stretch},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 347 "IWORKToken.gperf"
      {"inputDistance",inputDistance},
#line 57 "IWORKToken.gperf"
      {"SFTCellStylePropertyParagraphStyle",SFTCellStylePropertyParagraphStyle},
#line 103 "IWORKToken.gperf"
      {"annotation",annotation},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 315 "IWORKToken.gperf"
      {"head",head},
      {(char*)0}, {(char*)0},
#line 663 "IWORKToken.gperf"
      {"version",version},
      {(char*)0}, {(char*)0},
#line 475 "IWORKToken.gperf"
      {"parent-ref",parent_ref},
      {(char*)0}, {(char*)0},
#line 492 "IWORKToken.gperf"
      {"projects",projects},
      {(char*)0},
#line 294 "IWORKToken.gperf"
      {"ghost-text",ghost_text},
      {(char*)0}, {(char*)0},
#line 644 "IWORKToken.gperf"
      {"transform-gradient",transform_gradient},
#line 295 "IWORKToken.gperf"
      {"ghost-text-ref",ghost_text_ref},
#line 458 "IWORKToken.gperf"
      {"page-count",page_count},
      {(char*)0}, {(char*)0},
#line 359 "IWORKToken.gperf"
      {"keywords",keywords},
#line 649 "IWORKToken.gperf"
      {"type",type},
      {(char*)0},
#line 270 "IWORKToken.gperf"
      {"format-base-use-minus-sign",format_base_use_minus_sign},
      {(char*)0},
#line 170 "IWORKToken.gperf"
      {"col-span",col_span},
#line 54 "IWORKToken.gperf"
      {"SFTCellStylePropertyDurationFormat",SFTCellStylePropertyDurationFormat},
#line 590 "IWORKToken.gperf"
      {"tableModelCells",tableModelCells},
#line 168 "IWORKToken.gperf"
      {"chisel",chisel},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 445 "IWORKToken.gperf"
      {"numcols", numcols},
      {(char*)0},
#line 318 "IWORKToken.gperf"
      {"headline-style",headline_style},
      {(char*)0},
#line 113 "IWORKToken.gperf"
      {"audio-only-image",audio_only_image},
      {(char*)0},
#line 452 "IWORKToken.gperf"
      {"original-size",original_size},
      {(char*)0},
#line 114 "IWORKToken.gperf"
      {"audio-only-image-ref",audio_only_image_ref},
#line 291 "IWORKToken.gperf"
      {"g",g},
      {(char*)0},
#line 576 "IWORKToken.gperf"
      {"table-vector-style",table_vector_style},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 577 "IWORKToken.gperf"
      {"table-vector-style-ref",table_vector_style_ref},
      {(char*)0}, {(char*)0},
#line 593 "IWORKToken.gperf"
      {"tableModelPartitionSource",tableModelPartitionSource},
#line 175 "IWORKToken.gperf"
      {"columns-ref",columns_ref},
      {(char*)0},
#line 302 "IWORKToken.gperf"
      {"grid-row",grid_row},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 174 "IWORKToken.gperf"
      {"columns",columns},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 281 "IWORKToken.gperf"
      {"formula-cell",formula_cell},
#line 358 "IWORKToken.gperf"
      {"key",key},
      {(char*)0}, {(char*)0},
#line 172 "IWORKToken.gperf"
      {"column",column},
      {(char*)0},
#line 117 "IWORKToken.gperf"
      {"baselineShift",baselineShift},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 466 "IWORKToken.gperf"
      {"pageBreakBefore",pageBreakBefore},
#line 637 "IWORKToken.gperf"
      {"title-placeholder-ref",title_placeholder_ref},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 467 "IWORKToken.gperf"
      {"pagemaster",pagemaster},
#line 197 "IWORKToken.gperf"
      {"d",d},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 171 "IWORKToken.gperf"
      {"color",color},
      {(char*)0},
#line 334 "IWORKToken.gperf"
      {"id",id},
#line 222 "IWORKToken.gperf"
      {"equal-columns",equal_columns},
#line 586 "IWORKToken.gperf"
      {"tableCellPreferredHeight",tableCellPreferredHeight},
#line 410 "IWORKToken.gperf"
      {"main-movie",main_movie},
#line 592 "IWORKToken.gperf"
      {"tableModelIsHeaderRow",tableModelIsHeaderRow},
#line 286 "IWORKToken.gperf"
      {"frame-h",frame_h},
      {(char*)0}, {(char*)0},
#line 95 "IWORKToken.gperf"
      {"alpha-mask-path",alpha_mask_path},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 411 "IWORKToken.gperf"
      {"main-movie-ref",main_movie_ref},
      {(char*)0},
#line 479 "IWORKToken.gperf"
      {"pattern-ref",pattern_ref},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 301 "IWORKToken.gperf"
      {"grid-column",grid_column},
      {(char*)0},
#line 514 "IWORKToken.gperf"
      {"row-span",row_span},
#line 327 "IWORKToken.gperf"
      {"href",href},
#line 671 "IWORKToken.gperf"
      {"width",width},
#line 84 "IWORKToken.gperf"
      {"SFTTableNameStylePropertyLayoutStyle",SFTTableNameStylePropertyLayoutStyle},
#line 322 "IWORKToken.gperf"
      {"host-cell-ID",host_cell_ID},
      {(char*)0},
#line 85 "IWORKToken.gperf"
      {"SFTTableNameStylePropertyParagraphStyle",SFTTableNameStylePropertyParagraphStyle},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 149 "IWORKToken.gperf"
      {"cell-style",cell_style},
#line 235 "IWORKToken.gperf"
      {"filled",filled},
#line 459 "IWORKToken.gperf"
      {"page-margins",page_margins},
      {(char*)0},
#line 627 "IWORKToken.gperf"
      {"text-label-ref",text_label_ref},
      {(char*)0},
#line 597 "IWORKToken.gperf"
      {"tableVectorAlong",tableVectorAlong},
      {(char*)0}, {(char*)0},
#line 151 "IWORKToken.gperf"
      {"cell-style-ref",cell_style_ref},
      {(char*)0},
#line 182 "IWORKToken.gperf"
      {"contbr",contbr},
#line 478 "IWORKToken.gperf"
      {"pattern",pattern},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 127 "IWORKToken.gperf"
      {"both",both},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 130 "IWORKToken.gperf"
      {"bubble-offset",bubble_offset},
      {(char*)0},
#line 331 "IWORKToken.gperf"
      {"http://developer.apple.com/namespaces/sf",NS_URI_SF},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 48 "IWORKToken.gperf"
      {"SFIUDropShadow",SFIUDropShadow},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 248 "IWORKToken.gperf"
      {"fitting-height",fitting_height},
#line 383 "IWORKToken.gperf"
      {"leftIndent",leftIndent},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 480 "IWORKToken.gperf"
      {"phase",phase},
#line 209 "IWORKToken.gperf"
      {"directional",directional},
#line 575 "IWORKToken.gperf"
      {"table-vector",table_vector},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 375 "IWORKToken.gperf"
      {"layout",layout},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 381 "IWORKToken.gperf"
      {"layoutStyle",layoutStyle},
#line 633 "IWORKToken.gperf"
      {"textured-fill-ref",textured_fill_ref},
      {(char*)0}, {(char*)0},
#line 401 "IWORKToken.gperf"
      {"listTextIndents",listTextIndents},
      {(char*)0}, {(char*)0},
#line 657 "IWORKToken.gperf"
      {"val",val},
#line 591 "IWORKToken.gperf"
      {"tableModelIsHeaderColumn",tableModelIsHeaderColumn},
      {(char*)0},
#line 379 "IWORKToken.gperf"
      {"layoutstyle",layoutstyle},
      {(char*)0}, {(char*)0},
#line 199 "IWORKToken.gperf"
      {"date-cell",date_cell},
#line 539 "IWORKToken.gperf"
      {"slider-position",slider_position},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 300 "IWORKToken.gperf"
      {"grid",grid},
#line 150 "IWORKToken.gperf"
      {"cell-style-default-line-height",cell_style_default_line_height},
      {(char*)0}, {(char*)0},
#line 525 "IWORKToken.gperf"
      {"selection-end",selection_end},
      {(char*)0},
#line 416 "IWORKToken.gperf"
      {"masking-shape-path-source",masking_shape_path_source},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 397 "IWORKToken.gperf"
      {"listLabelGeometries",listLabelGeometries},
#line 368 "IWORKToken.gperf"
      {"labelCharacterStyle8",labelCharacterStyle8},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 303 "IWORKToken.gperf"
      {"gridline-index",gridline_index,},
#line 363 "IWORKToken.gperf"
      {"labelCharacterStyle3",labelCharacterStyle3},
#line 398 "IWORKToken.gperf"
      {"listLabelIndents",listLabelIndents},
#line 499 "IWORKToken.gperf"
      {"rd",rd},
#line 405 "IWORKToken.gperf"
      {"localize",localize},
#line 185 "IWORKToken.gperf"
      {"copyright",copyright},
#line 364 "IWORKToken.gperf"
      {"labelCharacterStyle4",labelCharacterStyle4},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 125 "IWORKToken.gperf"
      {"bold",bold},
#line 367 "IWORKToken.gperf"
      {"labelCharacterStyle7",labelCharacterStyle7},
#line 332 "IWORKToken.gperf"
      {"http://developer.apple.com/namespaces/sfa",NS_URI_SFA},
      {(char*)0}, {(char*)0},
#line 653 "IWORKToken.gperf"
      {"unfiltered-ref",unfiltered_ref},
#line 365 "IWORKToken.gperf"
      {"labelCharacterStyle5",labelCharacterStyle5},
#line 102 "IWORKToken.gperf"
      {"anon-styles",anon_styles},
      {(char*)0}, {(char*)0},
#line 140 "IWORKToken.gperf"
      {"capitalization",capitalization},
#line 369 "IWORKToken.gperf"
      {"labelCharacterStyle9",labelCharacterStyle9},
#line 655 "IWORKToken.gperf"
      {"upper-roman",upper_roman},
#line 292 "IWORKToken.gperf"
      {"generic-cell",generic_cell},
#line 277 "IWORKToken.gperf"
      {"format-string",format_string},
#line 476 "IWORKToken.gperf"
      {"path",path},
      {(char*)0},
#line 135 "IWORKToken.gperf"
      {"calc-engine",calc_engine},
      {(char*)0}, {(char*)0},
#line 283 "IWORKToken.gperf"
      {"formula-string",formula_string},
#line 366 "IWORKToken.gperf"
      {"labelCharacterStyle6",labelCharacterStyle6},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 535 "IWORKToken.gperf"
      {"sizesLocked",sizesLocked},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 348 "IWORKToken.gperf"
      {"inputGlossiness",inputGlossiness},
      {(char*)0},
#line 325 "IWORKToken.gperf"
      {"horizontal-offset",horizontal_offset},
      {(char*)0}, {(char*)0},
#line 136 "IWORKToken.gperf"
      {"calc-engine-entities",calc_engine_entities},
#line 460 "IWORKToken.gperf"
      {"page-number",page_number},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 110 "IWORKToken.gperf"
      {"attachment",attachment},
#line 544 "IWORKToken.gperf"
      {"spaceBefore",spaceBefore},
      {(char*)0}, {(char*)0},
#line 111 "IWORKToken.gperf"
      {"attachment-ref",attachment_ref},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 61 "IWORKToken.gperf"
      {"SFTDefaultFooterBodyVectorStyleProperty",SFTDefaultFooterBodyVectorStyleProperty},
#line 674 "IWORKToken.gperf"
      {"wrap-style",wrap_style},
      {(char*)0},
#line 143 "IWORKToken.gperf"
      {"cell-address",cell_address},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 116 "IWORKToken.gperf"
      {"b",b},
      {(char*)0}, {(char*)0},
#line 650 "IWORKToken.gperf"
      {"unaligned",unaligned},
#line 511 "IWORKToken.gperf"
      {"round",round},
#line 617 "IWORKToken.gperf"
      {"tailLineEnd",tailLineEnd,},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 556 "IWORKToken.gperf"
      {"strikethru",strikethru},
#line 112 "IWORKToken.gperf"
      {"attachments",attachments},
      {(char*)0},
#line 376 "IWORKToken.gperf"
      {"layoutMargins",layoutMargins},
      {(char*)0},
#line 44 "IWORKToken.gperf"
      {"SFC2DPieFillProperty",SFC2DPieFillProperty},
      {(char*)0},
#line 670 "IWORKToken.gperf"
      {"widowControl",widowControl},
#line 640 "IWORKToken.gperf"
      {"tocStyle",tocStyle},
      {(char*)0},
#line 47 "IWORKToken.gperf"
      {"SFC3DPieFillProperty",SFC3DPieFillProperty},
#line 173 "IWORKToken.gperf"
      {"column-label-formulas",column_label_formulas},
      {(char*)0},
#line 638 "IWORKToken.gperf"
      {"tocstyle",tocstyle},
      {(char*)0},
#line 440 "IWORKToken.gperf"
      {"num-header-rows",num_header_rows},
      {(char*)0},
#line 639 "IWORKToken.gperf"
      {"tocstyle-ref",tocstyle_ref},
#line 439 "IWORKToken.gperf"
      {"num-header-columns",num_header_columns},
#line 73 "IWORKToken.gperf"
      {"SFTDefaultGroupingRow3CellStyleProperty",SFTDefaultGroupingRowCell3StyleProperty},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 67 "IWORKToken.gperf"
      {"SFTDefaultGroupingLevel3VectorStyleProperty",SFTDefaultGroupingLevel3VectorStyleProperty},
#line 74 "IWORKToken.gperf"
      {"SFTDefaultGroupingRow4CellStyleProperty",SFTDefaultGroupingRowCell4StyleProperty},
#line 361 "IWORKToken.gperf"
      {"labelCharacterStyle1",labelCharacterStyle1},
#line 527 "IWORKToken.gperf"
      {"seriesIndex",seriesIndex},
      {(char*)0},
#line 68 "IWORKToken.gperf"
      {"SFTDefaultGroupingLevel4VectorStyleProperty",SFTDefaultGroupingLevel4VectorStyleProperty},
#line 547 "IWORKToken.gperf"
      {"span-cell",span_cell},
#line 380 "IWORKToken.gperf"
      {"layoutstyle-ref",layoutstyle_ref},
      {(char*)0}, {(char*)0},
#line 237 "IWORKToken.gperf"
      {"filtered",filtered},
#line 75 "IWORKToken.gperf"
      {"SFTDefaultGroupingRow5CellStyleProperty",SFTDefaultGroupingRowCell5StyleProperty},
#line 362 "IWORKToken.gperf"
      {"labelCharacterStyle2",labelCharacterStyle2},
      {(char*)0},
#line 142 "IWORKToken.gperf"
      {"cb",cb},
#line 69 "IWORKToken.gperf"
      {"SFTDefaultGroupingLevel5VectorStyleProperty",SFTDefaultGroupingLevel5VectorStyleProperty},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 323 "IWORKToken.gperf"
      {"host-table-ID",host_table_ID},
#line 198 "IWORKToken.gperf"
      {"data",data},
      {(char*)0},
#line 320 "IWORKToken.gperf"
      {"height",height},
      {(char*)0}, {(char*)0},
#line 238 "IWORKToken.gperf"
      {"filtered-image",filtered_image},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 239 "IWORKToken.gperf"
      {"filtered-image-ref",filtered_image_ref},
      {(char*)0},
#line 346 "IWORKToken.gperf"
      {"inputColor",inputColor},
      {(char*)0}, {(char*)0},
#line 513 "IWORKToken.gperf"
      {"row-label-formulas",row_label_formulas},
#line 421 "IWORKToken.gperf"
      {"min-value",min_value},
#line 626 "IWORKToken.gperf"
      {"text-label",text_label},
#line 465 "IWORKToken.gperf"
      {"padding-ref",padding_ref},
      {(char*)0}, {(char*)0},
#line 70 "IWORKToken.gperf"
      {"SFTDefaultGroupingRow0CellStyleProperty",SFTDefaultGroupingRowCell0StyleProperty},
      {(char*)0}, {(char*)0},
#line 595 "IWORKToken.gperf"
      {"tableModelTableID",tableModelTableID},
#line 64 "IWORKToken.gperf"
      {"SFTDefaultGroupingLevel0VectorStyleProperty",SFTDefaultGroupingLevel0VectorStyleProperty},
      {(char*)0}, {(char*)0},
#line 532 "IWORKToken.gperf"
      {"shearXAngle",shearXAngle},
      {(char*)0}, {(char*)0},
#line 463 "IWORKToken.gperf"
      {"pgbr",pgbr},
#line 666 "IWORKToken.gperf"
      {"vertical-offset",vertical_offset},
      {(char*)0}, {(char*)0},
#line 393 "IWORKToken.gperf"
      {"list-label-geometry-ref",list_label_geometry_ref},
#line 76 "IWORKToken.gperf"
      {"SFTDefaultHeaderBodyVectorStyleProperty",SFTDefaultHeaderBodyVectorStyleProperty},
      {(char*)0},
#line 134 "IWORKToken.gperf"
      {"cached-data",cached_data},
      {(char*)0},
#line 390 "IWORKToken.gperf"
      {"line-end",line_end},
#line 93 "IWORKToken.gperf"
      {"animationDelay",animationDelay},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 42 "IWORKToken.gperf"
      {"SFC2DColumnFillProperty",SFC2DColumnFillProperty},
#line 63 "IWORKToken.gperf"
      {"SFTDefaultFooterSeparatorVectorStyleProperty",SFTDefaultFooterSeparatorVectorStyleProperty},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 46 "IWORKToken.gperf"
      {"SFC3DColumnFillProperty",SFC3DColumnFillProperty},
      {(char*)0}, {(char*)0},
#line 457 "IWORKToken.gperf"
      {"p",p},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 664 "IWORKToken.gperf"
      {"verticalAlignment",verticalAlignment},
#line 203 "IWORKToken.gperf"
      {"data-ref",data_ref},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 661 "IWORKToken.gperf"
      {"vector-style",vector_style},
      {(char*)0},
#line 77 "IWORKToken.gperf"
      {"SFTDefaultHeaderColumnCellStyleProperty",SFTDefaultHeaderColumnCellStyleProperty},
#line 97 "IWORKToken.gperf"
      {"attachment-wrap-type",attachment_wrap_type},
      {(char*)0}, {(char*)0},
#line 632 "IWORKToken.gperf"
      {"textured-fill",textured_fill},
      {(char*)0}, {(char*)0},
#line 662 "IWORKToken.gperf"
      {"vector-style-ref",vector_style_ref},
#line 498 "IWORKToken.gperf"
      {"rb",rb},
#line 138 "IWORKToken.gperf"
      {"callout2-path",callout2_path},
#line 71 "IWORKToken.gperf"
      {"SFTDefaultGroupingRow1CellStyleProperty",SFTDefaultGroupingRowCell1StyleProperty},
      {(char*)0},
#line 533 "IWORKToken.gperf"
      {"shearYAngle",shearYAngle},
#line 297 "IWORKToken.gperf"
      {"gradient-stop-ref",gradient_stop_ref},
#line 65 "IWORKToken.gperf"
      {"SFTDefaultGroupingLevel1VectorStyleProperty",SFTDefaultGroupingLevel1VectorStyleProperty},
#line 417 "IWORKToken.gperf"
      {"max-value",max_value},
      {(char*)0}, {(char*)0},
#line 419 "IWORKToken.gperf"
      {"menu-choices",menu_choices},
#line 227 "IWORKToken.gperf"
      {"external-text-wrap",external_text_wrap},
#line 72 "IWORKToken.gperf"
      {"SFTDefaultGroupingRow2CellStyleProperty",SFTDefaultGroupingRowCell2StyleProperty},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 66 "IWORKToken.gperf"
      {"SFTDefaultGroupingLevel2VectorStyleProperty",SFTDefaultGroupingLevel2VectorStyleProperty},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 543 "IWORKToken.gperf"
      {"spaceAfter",spaceAfter},
      {(char*)0},
#line 92 "IWORKToken.gperf"
      {"animationAutoPlay",animationAutoPlay},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 431 "IWORKToken.gperf"
      {"naturalSize",naturalSize},
#line 610 "IWORKToken.gperf"
      {"tabular-info",tabular_info},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 654 "IWORKToken.gperf"
      {"upper-alpha",upper_alpha},
      {(char*)0},
#line 420 "IWORKToken.gperf"
      {"metadata",metadata},
#line 216 "IWORKToken.gperf"
      {"duration-format-ref",duration_format_ref},
#line 215 "IWORKToken.gperf"
      {"duration-format",duration_format},
#line 497 "IWORKToken.gperf"
      {"radial",radial},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 41 "IWORKToken.gperf"
      {"SFC2DAreaFillProperty",SFC2DAreaFillProperty},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 96 "IWORKToken.gperf"
      {"alpha-threshold",alpha_threshold},
#line 45 "IWORKToken.gperf"
      {"SFC3DAreaFillProperty",SFC3DAreaFillProperty},
      {(char*)0},
#line 448 "IWORKToken.gperf"
      {"oddPageMaster",oddPageMaster},
      {(char*)0}, {(char*)0},
#line 104 "IWORKToken.gperf"
      {"annotation-field",annotation_field},
      {(char*)0},
#line 370 "IWORKToken.gperf"
      {"language",language},
      {(char*)0},
#line 473 "IWORKToken.gperf"
      {"parameters",parameters},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 79 "IWORKToken.gperf"
      {"SFTDefaultHeaderSeparatorVectorStyleProperty",SFTDefaultHeaderSeparatorVectorStyleProperty},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 345 "IWORKToken.gperf"
      {"inputAngle",inputAngle},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 60 "IWORKToken.gperf"
      {"SFTDefaultBorderVectorStyleProperty",SFTDefaultBorderVectorStyleProperty},
      {(char*)0},
#line 489 "IWORKToken.gperf"
      {"poster-image",poster_image},
      {(char*)0}, {(char*)0},
#line 167 "IWORKToken.gperf"
      {"chart-type",chart_type},
#line 62 "IWORKToken.gperf"
      {"SFTDefaultFooterRowCellStyleProperty",SFTDefaultFooterRowCellStyleProperty},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 488 "IWORKToken.gperf"
      {"position",position},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 477 "IWORKToken.gperf"
      {"path-join",path_join},
      {(char*)0}, {(char*)0},
#line 606 "IWORKToken.gperf"
      {"tabstop",tabstop},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 269 "IWORKToken.gperf"
      {"format-base-places",format_base_places},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 350 "IWORKToken.gperf"
      {"inputRadius",inputRadius},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 55 "IWORKToken.gperf"
      {"SFTCellStylePropertyNumberFormat",SFTCellStylePropertyNumberFormat},
      {(char*)0}, {(char*)0},
#line 205 "IWORKToken.gperf"
      {"datasource",datasource},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 507 "IWORKToken.gperf"
      {"result-text-cell",result_text_cell},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 119 "IWORKToken.gperf"
      {"bezier",bezier},
#line 384 "IWORKToken.gperf"
      {"leveled",leveled},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 121 "IWORKToken.gperf"
      {"bezier-ref",bezier_ref},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 455 "IWORKToken.gperf"
      {"overrides",overrides},
      {(char*)0}, {(char*)0},
#line 500 "IWORKToken.gperf"
      {"regular",regular},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 319 "IWORKToken.gperf"
      {"headLineEnd",headLineEnd},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 406 "IWORKToken.gperf"
      {"locked",locked},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 226 "IWORKToken.gperf"
      {"externalTextWrap",externalTextWrap},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 78 "IWORKToken.gperf"
      {"SFTDefaultHeaderRowCellStyleProperty",SFTDefaultHeaderRowCellStyleProperty},
      {(char*)0}, {(char*)0},
#line 400 "IWORKToken.gperf"
      {"listStyle",listStyle},
      {(char*)0}, {(char*)0},
#line 676 "IWORKToken.gperf"
      {"x-coordinate",x_coordinate},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 501 "IWORKToken.gperf"
      {"relative",relative},
#line 402 "IWORKToken.gperf"
      {"liststyle",liststyle},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 224 "IWORKToken.gperf"
      {"evenPageMaster",evenPageMaster},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 552 "IWORKToken.gperf"
      {"sticky-note",sticky_note},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 324 "IWORKToken.gperf"
      {"horizontal-gridline-styles",horizontal_gridline_styles},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 656 "IWORKToken.gperf"
      {"v",v},
#line 352 "IWORKToken.gperf"
      {"intratopicbr",intratopicbr},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 430 "IWORKToken.gperf"
      {"natural",natural},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 594 "IWORKToken.gperf"
      {"tableModelStyle-ref",tableModelStyle_ref},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 456 "IWORKToken.gperf"
      {"overrides-ref",overrides_ref},
      {(char*)0},
#line 396 "IWORKToken.gperf"
      {"list-level",list_level},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 200 "IWORKToken.gperf"
      {"data-formulas",data_formulas},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 207 "IWORKToken.gperf"
      {"decimal-number",number},
#line 652 "IWORKToken.gperf"
      {"unfiltered",unfiltered},
#line 557 "IWORKToken.gperf"
      {"string",string},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 608 "IWORKToken.gperf"
      {"tabuar-style-name-internal",tabular_style_name_internal},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 658 "IWORKToken.gperf"
      {"value",value},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 389 "IWORKToken.gperf"
      {"linespacing-ref",linespacing_ref},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 541 "IWORKToken.gperf"
      {"solid",solid},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 403 "IWORKToken.gperf"
      {"liststyle-ref",liststyle_ref},
      {(char*)0}, {(char*)0},
#line 660 "IWORKToken.gperf"
      {"value-title",value_title},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 504 "IWORKToken.gperf"
      {"result-cell",result_cell},
#line 206 "IWORKToken.gperf"
      {"decimal",decimal},
#line 643 "IWORKToken.gperf"
      {"tracking",tracking},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 240 "IWORKToken.gperf"
      {"filter-properties",filter_properties},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 377 "IWORKToken.gperf"
      {"layoutParagraphStyle",layoutParagraphStyle},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 464 "IWORKToken.gperf"
      {"padding",padding},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 213 "IWORKToken.gperf"
      {"drawables",drawables},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 89 "IWORKToken.gperf"
      {"aligned",aligned},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 81 "IWORKToken.gperf"
      {"SFTHeaderRowRepeatsProperty",SFTHeaderRowRepeatsProperty},
      {(char*)0},
#line 659 "IWORKToken.gperf"
      {"value-ref",value_ref},
      {(char*)0},
#line 519 "IWORKToken.gperf"
      {"scalar-path",scalar_path},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 137 "IWORKToken.gperf"
      {"calligraphy-stroke",calligraphy_stroke},
      {(char*)0}, {(char*)0},
#line 210 "IWORKToken.gperf"
      {"displayname",displayname},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 326 "IWORKToken.gperf"
      {"horizontalFlip",horizontalFlip},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 678 "IWORKToken.gperf"
      {"y",y},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 378 "IWORKToken.gperf"
      {"layoutParagraphStyle-ref",layoutParagraphStyle_ref},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 108 "IWORKToken.gperf"
      {"array-ref",array_ref},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 486 "IWORKToken.gperf"
      {"point-path",point_path},
#line 58 "IWORKToken.gperf"
      {"SFTDefaultBodyCellStyleProperty",SFTDefaultBodyCellStyleProperty},
#line 230 "IWORKToken.gperf"
      {"facing-pages",facing_pages},
#line 59 "IWORKToken.gperf"
      {"SFTDefaultBodyVectorStyleProperty",SFTDefaultBodyVectorStyleProperty},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 667 "IWORKToken.gperf"
      {"verticalFlip",verticalFlip},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 141 "IWORKToken.gperf"
      {"category-title",category_title},
#line 217 "IWORKToken.gperf"
      {"editable-bezier-path",editable_bezier_path},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 50 "IWORKToken.gperf"
      {"SFTableStylePropertyCellStyle",SFTableStylePropertyCellStyle},
      {(char*)0}, {(char*)0},
#line 82 "IWORKToken.gperf"
      {"SFTStrokeProperty", SFTStrokeProperty},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 505 "IWORKToken.gperf"
      {"result-date-cell",result_date_cell},
      {(char*)0}, {(char*)0},
#line 344 "IWORKToken.gperf"
      {"inline-wrap-enabled",inline_wrap_enabled},
      {(char*)0},
#line 490 "IWORKToken.gperf"
      {"preferred-height",preferred_height},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 249 "IWORKToken.gperf"
      {"fitting-width",fitting_width},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 243 "IWORKToken.gperf"
      {"filter_visibility",filter_visibility},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 296 "IWORKToken.gperf"
      {"gradient-stop",gradient_stop},
      {(char*)0},
#line 219 "IWORKToken.gperf"
      {"empty",empty},
#line 386 "IWORKToken.gperf"
      {"lineSpacing",lineSpacing},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 80 "IWORKToken.gperf"
      {"SFTHeaderColumnRepeatsProperty",SFTHeaderColumnRepeatsProperty},
#line 388 "IWORKToken.gperf"
      {"linespacing",linespacing},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 399 "IWORKToken.gperf"
      {"listLabelTypes",listLabelTypes},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 298 "IWORKToken.gperf"
      {"graphic-style",graphic_style},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 299 "IWORKToken.gperf"
      {"graphic-style-ref",graphic_style_ref},
#line 321 "IWORKToken.gperf"
      {"hfs-type",hfs_type},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 289 "IWORKToken.gperf"
      {"frame-y",frame_y},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 223 "IWORKToken.gperf"
      {"error_warning_mapping",error_warning_mapping},
#line 612 "IWORKToken.gperf"
      {"tabular-model-ref",tabular_model_ref},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 86 "IWORKToken.gperf"
      {"__multilingual",__multilingual},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 672 "IWORKToken.gperf"
      {"workspace-style",workspace_style},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 273 "IWORKToken.gperf"
      {"format-fraction-accuracy",format_fraction_accuracy},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 252 "IWORKToken.gperf"
      {"floating-wrap-type",floating_wrap_type},
#line 665 "IWORKToken.gperf"
      {"vertical-gridline-styles",vertical_gridline_styles,},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 545 "IWORKToken.gperf"
      {"spacing",spacing},
      {(char*)0},
#line 624 "IWORKToken.gperf"
      {"text-body",text_body},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 506 "IWORKToken.gperf"
      {"result-number-cell",result_number_cell},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 43 "IWORKToken.gperf"
      {"SFC2DMixedColumnFillProperty",SFC2DMixedColumnFillProperty},
#line 392 "IWORKToken.gperf"
      {"list-label-geometry",list_label_geometry},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 395 "IWORKToken.gperf"
      {"list-label-typeinfo-ref",list_label_typeinfo_ref},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 52 "IWORKToken.gperf"
      {"SFTableStylePropertyHeaderRowCellStyle",SFTableStylePropertyHeaderRowCellStyle},
      {(char*)0}, {(char*)0},
#line 51 "IWORKToken.gperf"
      {"SFTableStylePropertyHeaderColumnCellStyle",SFTableStylePropertyHeaderColumnCellStyle},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 481 "IWORKToken.gperf"
      {"placeholder-size",placeholder_size},
#line 482 "IWORKToken.gperf"
      {"placeholder-style",placeholder_style},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 483 "IWORKToken.gperf"
      {"placeholder-style-ref",placeholder_style_ref},
      {(char*)0}, {(char*)0},
#line 394 "IWORKToken.gperf"
      {"list-label-typeinfo",list_label_typeinfo},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 613 "IWORKToken.gperf"
      {"tabular-style",tabular_style},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 614 "IWORKToken.gperf"
      {"tabular-style-ref",tabular_style_ref},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 679 "IWORKToken.gperf"
      {"y-coordinate",y_coordinate},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 129 "IWORKToken.gperf"
      {"bubble-cellid",bubble_cellid},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 306 "IWORKToken.gperf"
      {"group-formula-value",group_formula_value},
#line 124 "IWORKToken.gperf"
      {"body-placeholder-ref",body_placeholder_ref},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 503 "IWORKToken.gperf"
      {"result-bool-cell",result_bool_cell},
      {(char*)0},
#line 611 "IWORKToken.gperf"
      {"tabular-model",tabular_model},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 412 "IWORKToken.gperf"
      {"manipulated-stroke",manipulated_stroke},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 123 "IWORKToken.gperf"
      {"binary-ref",binary_ref},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 494 "IWORKToken.gperf"
      {"proxied-cell-ref",proxied_cell_ref},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 349 "IWORKToken.gperf"
      {"inputOpacity",inputOpacity},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 293 "IWORKToken.gperf"
      {"geometry",geometry},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 491 "IWORKToken.gperf"
      {"preferred-width",preferred_width},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 193 "IWORKToken.gperf"
      {"crop-geometry",crop_geometry},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 313 "IWORKToken.gperf"
      {"groupings-element",groupings_element},
      {(char*)0},
#line 312 "IWORKToken.gperf"
      {"grouping-state",grouping_state},
      {(char*)0}, {(char*)0},
#line 493 "IWORKToken.gperf"
      {"property-map",property_map},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 642 "IWORKToken.gperf"
      {"traced-path",traced_path},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 609 "IWORKToken.gperf"
      {"tabular-style-name-internal",tabular_style_name_internal},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 427 "IWORKToken.gperf"
      {"mutable-array-ref",mutable_array_ref},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 107 "IWORKToken.gperf"
      {"array",array},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 251 "IWORKToken.gperf"
      {"floating-wrap-enabled",floating_wrap_enabled},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 304 "IWORKToken.gperf"
      {"group",group},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 470 "IWORKToken.gperf"
      {"paragraphStroke",paragraphStroke},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 83 "IWORKToken.gperf"
      {"SFTTableBandedRowsProperty",SFTTableBandedRowsProperty},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 305 "IWORKToken.gperf"
      {"group-formula-string",group_formula_string},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 212 "IWORKToken.gperf"
      {"drawable-shape",drawable_shape},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 120 "IWORKToken.gperf"
      {"bezier-path",bezier_path},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 311 "IWORKToken.gperf"
      {"grouping-order",grouping_order},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 307 "IWORKToken.gperf"
      {"group-formula-value-valid",group_formula_value_valid},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 413 "IWORKToken.gperf"
      {"manually-sized",manually_sized},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 495 "IWORKToken.gperf"
      {"proxy-master-layer",proxy_master_layer},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 471 "IWORKToken.gperf"
      {"paragraphstyle",paragraphstyle},
      {(char*)0},
#line 308 "IWORKToken.gperf"
      {"group-value",group_value},
      {(char*)0},
#line 472 "IWORKToken.gperf"
      {"paragraphstyle-ref",paragraphstyle_ref},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0},
#line 49 "IWORKToken.gperf"
      {"SFTableCellStylePropertyFill",SFTableCellStylePropertyFill},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 451 "IWORKToken.gperf"
      {"opacity",opacity},
#line 309 "IWORKToken.gperf"
      {"grouping",grouping},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 469 "IWORKToken.gperf"
      {"paragraphFill",paragraphFill},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 122 "IWORKToken.gperf"
      {"binary",binary},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 468 "IWORKToken.gperf"
      {"paragraphBorderType",paragraphBorderType},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 426 "IWORKToken.gperf"
      {"mutable-array",mutable_array},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 310 "IWORKToken.gperf"
      {"grouping-display",grouping_display}
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
#line 680 "IWORKToken.gperf"

