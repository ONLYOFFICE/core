/* C++ code produced by gperf version 3.0.1 */
/* Command-line: gperf --compare-strncmp --enum --null-strings --readonly-tables --language C++ --output-file KEY2Token.inc KEY2Token.gperf  */
/* Computed positions: -k'1,4,$' */

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

#line 10 "KEY2Token.gperf"

#if defined __GNUC__
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

using namespace KEY2Token;
#line 18 "KEY2Token.gperf"
struct Token
{
  const char *name;
  int id;
};
#include <string.h>
/* maximum key range = 140, duplicates = 0 */

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
  static const unsigned char asso_values[] =
    {
      141, 141, 141, 141, 141, 141, 141, 141, 141, 141,
      141, 141, 141, 141, 141, 141, 141, 141, 141, 141,
      141, 141, 141, 141, 141, 141, 141, 141, 141, 141,
      141, 141, 141, 141, 141, 141, 141, 141, 141, 141,
      141, 141, 141, 141, 141, 141, 141, 141,   5,  65,
        0, 141,  35,   0, 141,   5, 141,   0, 141, 141,
      141, 141, 141, 141, 141, 141,   0, 141, 141, 141,
      141, 141, 141, 141, 141, 141, 141, 141, 141, 141,
      141, 141, 141, 141, 141, 141, 141, 141, 141, 141,
      141, 141, 141, 141, 141, 141, 141,   0,  25,   0,
       15,   0,  55,  10,  10,   5, 141,  15,  20,   0,
       10,  25,  40, 141,  25,  25,   5,   0,  30,   5,
      141,  40, 141, 141, 141, 141, 141, 141, 141, 141,
      141, 141, 141, 141, 141, 141, 141, 141, 141, 141,
      141, 141, 141, 141, 141, 141, 141, 141, 141, 141,
      141, 141, 141, 141, 141, 141, 141, 141, 141, 141,
      141, 141, 141, 141, 141, 141, 141, 141, 141, 141,
      141, 141, 141, 141, 141, 141, 141, 141, 141, 141,
      141, 141, 141, 141, 141, 141, 141, 141, 141, 141,
      141, 141, 141, 141, 141, 141, 141, 141, 141, 141,
      141, 141, 141, 141, 141, 141, 141, 141, 141, 141,
      141, 141, 141, 141, 141, 141, 141, 141, 141, 141,
      141, 141, 141, 141, 141, 141, 141, 141, 141, 141,
      141, 141, 141, 141, 141, 141, 141, 141, 141, 141,
      141, 141, 141, 141, 141, 141, 141, 141, 141, 141,
      141, 141, 141, 141, 141, 141
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[3]];
      /*FALLTHROUGH*/
      case 3:
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
      TOTAL_KEYWORDS = 67,
      MIN_WORD_LENGTH = 1,
      MAX_WORD_LENGTH = 46,
      MIN_HASH_VALUE = 1,
      MAX_HASH_VALUE = 140
    };

  static const struct Token wordlist[] =
    {
      {(char*)0},
#line 49 "KEY2Token.gperf"
      {"c",c},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0},
#line 65 "KEY2Token.gperf"
      {"metadata",metadata},
#line 89 "KEY2Token.gperf"
      {"type",type},
#line 84 "KEY2Token.gperf"
      {"theme",theme},
#line 58 "KEY2Token.gperf"
      {"i",i},
#line 50 "KEY2Token.gperf"
      {"comment",comment},
#line 41 "KEY2Token.gperf"
      {"animationType",animationType},
#line 66 "KEY2Token.gperf"
      {"name",name},
#line 26 "KEY2Token.gperf"
      {"2005112100",VERSION_STR_3},
      {(char*)0},
#line 62 "KEY2Token.gperf"
      {"master-slide",master_slide},
      {(char*)0},
#line 83 "KEY2Token.gperf"
      {"text",text},
#line 85 "KEY2Token.gperf"
      {"theme-list",theme_list},
#line 28 "KEY2Token.gperf"
      {"92008102400",VERSION_STR_5},
      {(char*)0},
#line 36 "KEY2Token.gperf"
      {"animationEndOffset",animationEndOffset},
      {(char*)0},
#line 39 "KEY2Token.gperf"
      {"animationStartOffset",animationStartOffset},
#line 27 "KEY2Token.gperf"
      {"72007061400",VERSION_STR_4},
#line 35 "KEY2Token.gperf"
      {"animationDuration",animationDuration},
#line 40 "KEY2Token.gperf"
      {"animationTimingReferent",animationTimingReferent},
#line 73 "KEY2Token.gperf"
      {"size",size},
#line 86 "KEY2Token.gperf"
      {"title",title},
      {(char*)0}, {(char*)0},
#line 55 "KEY2Token.gperf"
      {"headline",headline},
#line 53 "KEY2Token.gperf"
      {"direction",direction},
#line 52 "KEY2Token.gperf"
      {"depth",depth},
#line 78 "KEY2Token.gperf"
      {"sticky-note",sticky_note},
#line 34 "KEY2Token.gperf"
      {"animationDelayAutomaticWith",animationDelayAutomaticWith},
#line 30 "KEY2Token.gperf"
      {"animationAuto",animationAuto},
      {(char*)0},
#line 67 "KEY2Token.gperf"
      {"notes",notes},
#line 54 "KEY2Token.gperf"
      {"events",events},
#line 42 "KEY2Token.gperf"
      {"authors",authors},
#line 63 "KEY2Token.gperf"
      {"master-slides",master_slides},
#line 70 "KEY2Token.gperf"
      {"page",page},
#line 74 "KEY2Token.gperf"
      {"slide",slide},
#line 80 "KEY2Token.gperf"
      {"string",string},
#line 56 "KEY2Token.gperf"
      {"headlineParagraphStyle",headlineParagraphStyle},
#line 37 "KEY2Token.gperf"
      {"animationInterchunkAuto",animationInterchunkAuto},
      {(char*)0},
#line 24 "KEY2Token.gperf"
      {"2004102100",VERSION_STR_2},
#line 77 "KEY2Token.gperf"
      {"slide-style",slide_style},
#line 33 "KEY2Token.gperf"
      {"animationDelayAutmaticAfter",animationDelayAutomaticAfter},
#line 61 "KEY2Token.gperf"
      {"keywords",keywords},
#line 32 "KEY2Token.gperf"
      {"animationDelay",animationDelay},
#line 75 "KEY2Token.gperf"
      {"slide-list",slide_list},
      {(char*)0},
#line 31 "KEY2Token.gperf"
      {"animationAutoPlay",animationAutoPlay},
#line 60 "KEY2Token.gperf"
      {"key",key},
#line 51 "KEY2Token.gperf"
      {"decimal-number",number},
#line 82 "KEY2Token.gperf"
      {"stylesheet",stylesheet},
      {(char*)0},
#line 79 "KEY2Token.gperf"
      {"sticky-notes",sticky_notes},
#line 29 "KEY2Token.gperf"
      {"BGBuildDurationProperty",BGBuildDurationProperty},
#line 38 "KEY2Token.gperf"
      {"animationInterchunkDelay",animationInterchunkDelay},
#line 45 "KEY2Token.gperf"
      {"build",build},
#line 68 "KEY2Token.gperf"
      {"number",number},
#line 87 "KEY2Token.gperf"
      {"title-placeholder",title_placeholder},
#line 69 "KEY2Token.gperf"
      {"object-placeholder",object_placeholder},
      {(char*)0},
#line 64 "KEY2Token.gperf"
      {"master-ref",master_ref},
#line 46 "KEY2Token.gperf"
      {"build-chunk",build_chunk},
#line 90 "KEY2Token.gperf"
      {"version",version},
      {(char*)0}, {(char*)0},
#line 25 "KEY2Token.gperf"
      {"2005092101",COMPATIBLE_VERSION_STR_3,},
      {(char*)0},
#line 48 "KEY2Token.gperf"
      {"bullets",bullets},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 47 "KEY2Token.gperf"
      {"build-chunks",build_chunks},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 72 "KEY2Token.gperf"
      {"presentation",presentation},
      {(char*)0},
#line 76 "KEY2Token.gperf"
      {"slide-number-placeholder",slide_number_placeholder},
      {(char*)0}, {(char*)0}, {(char*)0},
#line 59 "KEY2Token.gperf"
      {"info-ref",info_ref},
      {(char*)0}, {(char*)0},
#line 57 "KEY2Token.gperf"
      {"http://developer.apple.com/namespaces/keynote2",NS_URI_KEY},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 88 "KEY2Token.gperf"
      {"title-placeholder-ref",title_placeholder_ref},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 43 "KEY2Token.gperf"
      {"body-placeholder",body_placeholder},
      {(char*)0}, {(char*)0},
#line 81 "KEY2Token.gperf"
      {"style-ref",style_ref},
      {(char*)0},
#line 71 "KEY2Token.gperf"
      {"parent-build-ref",parent_build_ref},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
      {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 44 "KEY2Token.gperf"
      {"body-placeholder-ref",body_placeholder_ref}
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
#line 91 "KEY2Token.gperf"

