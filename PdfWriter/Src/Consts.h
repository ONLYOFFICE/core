#ifndef _PDF_WRITER_SRC_CONSTS_H
#define _PDF_WRITER_SRC_CONSTS_H

#define  OK                    0
#define  NOERROR               0

// Стандартный размер буфера в memory-stream-object
#define STREAM_BUF_SIZ         4096

#define MAX_OPENED_FT_FACES    30

// Необходимый размер буффера для конвертирования с символьную строку.
#define SHORT_BUFFER_SIZE      32
#define REAL_LEN               31//11
#define INT_LEN                11
#define TEXT_DEFAULT_LEN       256
#define DATE_TIME_STR_LEN      23

// Соответсвующие длины в таблицу cross-reference-table
#define BYTE_OFFSET_LEN        10
#define OBJ_ID_LEN             7
#define GEN_NO_LEN             5

// Стандартный размер страницы (А4)
#define DEF_PAGE_WIDTH         595.276
#define DEF_PAGE_HEIGHT        841.89

// Типы компрессии

#define  COMP_NONE            0x00
#define  COMP_TEXT            0x01
#define  COMP_IMAGE           0x02
#define  COMP_METADATA        0x04
#define  COMP_ALL             0x0F
// #define  COMP_BEST_COMPRESS   0x10
// #define  COMP_BEST_SPEED      0x20
#define  COMP_MASK            0xFF


// Permission flags (only Revision 2 is supported)

#define ENABLE_READ         0
#define ENABLE_PRINT        4
#define ENABLE_EDIT_ALL     8
#define ENABLE_COPY         16
#define ENABLE_EDIT         32

// Варианты просмотра документа

#define HIDE_TOOLBAR    1
#define HIDE_MENUBAR    2
#define HIDE_WINDOW_UI  4
#define FIT_WINDOW      8
#define CENTER_WINDOW   16


// Ограничения, в соответствии со спецификацией
#define LIMIT_MAX_INT             2147483647
#define LIMIT_MIN_INT             -2147483647

#define LIMIT_MAX_REAL            3.402823466e+38F
#define LIMIT_MIN_REAL            -3.402823466e+38F

#define LIMIT_MAX_STRING_LEN      65535
#define LIMIT_MAX_NAME_LEN        127

#define LIMIT_MAX_ARRAY           8191
#define LIMIT_MAX_DICT_ELEMENT    4095
#define LIMIT_MAX_XREF_ELEMENT    8388607
#define MAX_GENERATION_NUM        65535

#endif // _PDF_WRITER_SRC_CONSTS_H
