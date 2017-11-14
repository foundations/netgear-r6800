/* ANSI-C code produced by gperf version 3.0.4 */
/* Command-line: gperf -m 10 ../../lib/unictype/pr_byname.gperf  */
/* Computed positions: -k'1,8,10' */

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

#line 1 "../../lib/unictype/pr_byname.gperf"
struct named_property { const char *name; uc_property_t property; };

#define TOTAL_KEYWORDS 79
#define MIN_WORD_LENGTH 4
#define MAX_WORD_LENGTH 34
#define MIN_HASH_VALUE 7
#define MAX_HASH_VALUE 120
/* maximum key range = 114, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
properties_hash (register const char *str, register unsigned int len)
{
  static const unsigned char asso_values[] =
    {
      121, 121, 121, 121, 121, 121, 121, 121, 121, 121,
      121, 121, 121, 121, 121, 121, 121, 121, 121, 121,
      121, 121, 121, 121, 121, 121, 121, 121, 121, 121,
      121, 121, 121, 121, 121, 121, 121, 121, 121, 121,
      121, 121, 121, 121, 121, 121, 121, 121, 121, 121,
      121, 121, 121, 121, 121, 121, 121, 121, 121, 121,
      121, 121, 121, 121, 121, 121, 121, 121, 121, 121,
      121, 121, 121, 121, 121, 121, 121, 121, 121, 121,
      121, 121, 121, 121, 121, 121, 121, 121, 121, 121,
      121, 121, 121, 121, 121,  12, 121,  30,   0,  51,
       37,   1,  42,  43,  36,   0,   4,  50,  14,  47,
        1,   9,   3,  43,   0,  13,   7,  37,  30,   0,
       44,  27,   0, 121, 121, 121, 121, 121
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[9]];
      /*FALLTHROUGH*/
      case 9:
      case 8:
        hval += asso_values[(unsigned char)str[7]];
      /*FALLTHROUGH*/
      case 7:
      case 6:
      case 5:
      case 4:
      case 3:
      case 2:
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

static const struct named_property properties[] =
  {
    {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 62 "../../lib/unictype/pr_byname.gperf"
    {"radical", { &uc_is_property_radical }},
#line 86 "../../lib/unictype/pr_byname.gperf"
    {"numeric", { &uc_is_property_numeric }},
#line 88 "../../lib/unictype/pr_byname.gperf"
    {"extender", { &uc_is_property_extender }},
    {""}, {""}, {""},
#line 41 "../../lib/unictype/pr_byname.gperf"
    {"bidi_control", { &uc_is_property_bidi_control }},
#line 60 "../../lib/unictype/pr_byname.gperf"
    {"ideographic", { &uc_is_property_ideographic }},
#line 28 "../../lib/unictype/pr_byname.gperf"
    {"id_start", { &uc_is_property_id_start }},
#line 52 "../../lib/unictype/pr_byname.gperf"
    {"bidi_whitespace", { &uc_is_property_bidi_whitespace }},
#line 36 "../../lib/unictype/pr_byname.gperf"
    {"join_control", { &uc_is_property_join_control }},
#line 66 "../../lib/unictype/pr_byname.gperf"
    {"space", { &uc_is_property_space }},
    {""}, {""},
#line 64 "../../lib/unictype/pr_byname.gperf"
    {"ids_trinary_operator", { &uc_is_property_ids_trinary_operator }},
#line 45 "../../lib/unictype/pr_byname.gperf"
    {"bidi_european_digit", { &uc_is_property_bidi_european_digit }},
#line 46 "../../lib/unictype/pr_byname.gperf"
    {"bidi_eur_num_separator", { &uc_is_property_bidi_eur_num_separator }},
#line 47 "../../lib/unictype/pr_byname.gperf"
    {"bidi_eur_num_terminator", { &uc_is_property_bidi_eur_num_terminator }},
#line 81 "../../lib/unictype/pr_byname.gperf"
    {"paired_punctuation", { &uc_is_property_paired_punctuation }},
#line 43 "../../lib/unictype/pr_byname.gperf"
    {"bidi_hebrew_right_to_left", { &uc_is_property_bidi_hebrew_right_to_left }},
#line 68 "../../lib/unictype/pr_byname.gperf"
    {"iso_control", { &uc_is_property_iso_control }},
#line 23 "../../lib/unictype/pr_byname.gperf"
    {"other_uppercase", { &uc_is_property_other_uppercase }},
#line 26 "../../lib/unictype/pr_byname.gperf"
    {"titlecase", { &uc_is_property_titlecase }},
#line 72 "../../lib/unictype/pr_byname.gperf"
    {"punctuation", { &uc_is_property_punctuation }},
#line 73 "../../lib/unictype/pr_byname.gperf"
    {"line_separator", { &uc_is_property_line_separator }},
#line 27 "../../lib/unictype/pr_byname.gperf"
    {"soft_dotted", { &uc_is_property_soft_dotted }},
#line 39 "../../lib/unictype/pr_byname.gperf"
    {"other_grapheme_extend", { &uc_is_property_other_grapheme_extend }},
#line 25 "../../lib/unictype/pr_byname.gperf"
    {"other_lowercase", { &uc_is_property_other_lowercase }},
#line 53 "../../lib/unictype/pr_byname.gperf"
    {"bidi_non_spacing_mark", { &uc_is_property_bidi_non_spacing_mark }},
#line 24 "../../lib/unictype/pr_byname.gperf"
    {"lowercase", { &uc_is_property_lowercase }},
#line 74 "../../lib/unictype/pr_byname.gperf"
    {"paragraph_separator", { &uc_is_property_paragraph_separator }},
#line 76 "../../lib/unictype/pr_byname.gperf"
    {"sentence_terminal", { &uc_is_property_sentence_terminal }},
#line 20 "../../lib/unictype/pr_byname.gperf"
    {"private_use", { &uc_is_property_private_use }},
#line 67 "../../lib/unictype/pr_byname.gperf"
    {"non_break", { &uc_is_property_non_break }},
#line 70 "../../lib/unictype/pr_byname.gperf"
    {"dash", { &uc_is_property_dash }},
#line 71 "../../lib/unictype/pr_byname.gperf"
    {"hyphen", { &uc_is_property_hyphen }},
#line 89 "../../lib/unictype/pr_byname.gperf"
    {"ignorable_control", { &uc_is_property_ignorable_control }},
#line 77 "../../lib/unictype/pr_byname.gperf"
    {"terminal_punctuation", { &uc_is_property_terminal_punctuation }},
#line 58 "../../lib/unictype/pr_byname.gperf"
    {"hex_digit", { &uc_is_property_hex_digit }},
#line 87 "../../lib/unictype/pr_byname.gperf"
    {"diacritic", { &uc_is_property_diacritic }},
#line 48 "../../lib/unictype/pr_byname.gperf"
    {"bidi_arabic_digit", { &uc_is_property_bidi_arabic_digit }},
#line 30 "../../lib/unictype/pr_byname.gperf"
    {"id_continue", { &uc_is_property_id_continue }},
#line 18 "../../lib/unictype/pr_byname.gperf"
    {"logical_order_exception", { &uc_is_property_logical_order_exception }},
#line 55 "../../lib/unictype/pr_byname.gperf"
    {"bidi_pdf", { &uc_is_property_bidi_pdf }},
#line 79 "../../lib/unictype/pr_byname.gperf"
    {"math", { &uc_is_property_math }},
#line 14 "../../lib/unictype/pr_byname.gperf"
    {"not_a_character", { &uc_is_property_not_a_character }},
#line 32 "../../lib/unictype/pr_byname.gperf"
    {"xid_start", { &uc_is_property_xid_start }},
#line 57 "../../lib/unictype/pr_byname.gperf"
    {"bidi_other_neutral", { &uc_is_property_bidi_other_neutral }},
#line 44 "../../lib/unictype/pr_byname.gperf"
    {"bidi_arabic_right_to_left", { &uc_is_property_bidi_arabic_right_to_left }},
#line 35 "../../lib/unictype/pr_byname.gperf"
    {"pattern_syntax", { &uc_is_property_pattern_syntax }},
#line 37 "../../lib/unictype/pr_byname.gperf"
    {"grapheme_base", { &uc_is_property_grapheme_base }},
#line 59 "../../lib/unictype/pr_byname.gperf"
    {"ascii_hex_digit", { &uc_is_property_ascii_hex_digit }},
#line 22 "../../lib/unictype/pr_byname.gperf"
    {"uppercase", { &uc_is_property_uppercase }},
#line 38 "../../lib/unictype/pr_byname.gperf"
    {"grapheme_extend", { &uc_is_property_grapheme_extend }},
#line 83 "../../lib/unictype/pr_byname.gperf"
    {"combining", { &uc_is_property_combining }},
#line 85 "../../lib/unictype/pr_byname.gperf"
    {"decimal_digit", { &uc_is_property_decimal_digit }},
#line 56 "../../lib/unictype/pr_byname.gperf"
    {"bidi_embedding_or_override", { &uc_is_property_bidi_embedding_or_override }},
#line 33 "../../lib/unictype/pr_byname.gperf"
    {"xid_continue", { &uc_is_property_xid_continue }},
#line 11 "../../lib/unictype/pr_byname.gperf"
    {"white_space", { &uc_is_property_white_space }},
#line 51 "../../lib/unictype/pr_byname.gperf"
    {"bidi_segment_separator", { &uc_is_property_bidi_segment_separator }},
#line 84 "../../lib/unictype/pr_byname.gperf"
    {"composite", { &uc_is_property_composite }},
#line 82 "../../lib/unictype/pr_byname.gperf"
    {"left_of_pair", { &uc_is_property_left_of_pair }},
#line 19 "../../lib/unictype/pr_byname.gperf"
    {"variation_selector", { &uc_is_property_variation_selector }},
#line 34 "../../lib/unictype/pr_byname.gperf"
    {"pattern_white_space", { &uc_is_property_pattern_white_space }},
#line 40 "../../lib/unictype/pr_byname.gperf"
    {"grapheme_link", { &uc_is_property_grapheme_link }},
#line 42 "../../lib/unictype/pr_byname.gperf"
    {"bidi_left_to_right", { &uc_is_property_bidi_left_to_right }},
#line 29 "../../lib/unictype/pr_byname.gperf"
    {"other_id_start", { &uc_is_property_other_id_start }},
#line 16 "../../lib/unictype/pr_byname.gperf"
    {"other_default_ignorable_code_point", { &uc_is_property_other_default_ignorable_code_point }},
#line 13 "../../lib/unictype/pr_byname.gperf"
    {"other_alphabetic", { &uc_is_property_other_alphabetic }},
#line 63 "../../lib/unictype/pr_byname.gperf"
    {"ids_binary_operator", { &uc_is_property_ids_binary_operator }},
#line 49 "../../lib/unictype/pr_byname.gperf"
    {"bidi_common_separator", { &uc_is_property_bidi_common_separator }},
#line 75 "../../lib/unictype/pr_byname.gperf"
    {"quotation_mark", { &uc_is_property_quotation_mark }},
#line 50 "../../lib/unictype/pr_byname.gperf"
    {"bidi_block_separator", { &uc_is_property_bidi_block_separator }},
    {""}, {""}, {""},
#line 65 "../../lib/unictype/pr_byname.gperf"
    {"zero_width", { &uc_is_property_zero_width }},
    {""},
#line 80 "../../lib/unictype/pr_byname.gperf"
    {"other_math", { &uc_is_property_other_math }},
    {""}, {""}, {""}, {""}, {""},
#line 17 "../../lib/unictype/pr_byname.gperf"
    {"deprecated", { &uc_is_property_deprecated }},
    {""}, {""}, {""},
#line 54 "../../lib/unictype/pr_byname.gperf"
    {"bidi_boundary_neutral", { &uc_is_property_bidi_boundary_neutral }},
#line 21 "../../lib/unictype/pr_byname.gperf"
    {"unassigned_code_value", { &uc_is_property_unassigned_code_value }},
    {""},
#line 12 "../../lib/unictype/pr_byname.gperf"
    {"alphabetic", { &uc_is_property_alphabetic }},
    {""}, {""}, {""}, {""},
#line 61 "../../lib/unictype/pr_byname.gperf"
    {"unified_ideograph", { &uc_is_property_unified_ideograph }},
    {""}, {""},
#line 78 "../../lib/unictype/pr_byname.gperf"
    {"currency_symbol", { &uc_is_property_currency_symbol }},
    {""},
#line 69 "../../lib/unictype/pr_byname.gperf"
    {"format_control", { &uc_is_property_format_control }},
    {""}, {""}, {""}, {""}, {""},
#line 31 "../../lib/unictype/pr_byname.gperf"
    {"other_id_continue", { &uc_is_property_other_id_continue }},
    {""}, {""}, {""}, {""}, {""},
#line 15 "../../lib/unictype/pr_byname.gperf"
    {"default_ignorable_code_point", { &uc_is_property_default_ignorable_code_point }}
  };

#ifdef __GNUC__
__inline
#if defined __GNUC_STDC_INLINE__ || defined __GNUC_GNU_INLINE__
__attribute__ ((__gnu_inline__))
#endif
#endif
const struct named_property *
uc_property_lookup (register const char *str, register unsigned int len)
{
  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = properties_hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register const char *s = properties[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &properties[key];
        }
    }
  return 0;
}
