#ifndef _ERROR_H
#define _ERROR_H

#include "Types.h"
#include "Utils.h"
#include "Consts.h"

#include "../Common/OfficeFileErrorDescription.h"


// Все ошибки в файле OfficeFileErrorDescription.h

//#define AVS_OFFICEPDFWRITER_ERROR_ARRAY_COUNT_ERR                      0x1001
//#define AVS_OFFICEPDFWRITER_ERROR_ARRAY_ITEM_NOT_FOUND                 0x1002
//#define AVS_OFFICEPDFWRITER_ERROR_ARRAY_ITEM_UNEXPECTED_TYPE           0x1003
//#define AVS_OFFICEPDFWRITER_ERROR_BINARY_LENGTH_ERR                    0x1004
//#define AVS_OFFICEPDFWRITER_ERROR_CANNOT_GET_PALLET                    0x1005
//#define AVS_OFFICEPDFWRITER_ERROR_DICT_COUNT_ERR                       0x1007
//#define AVS_OFFICEPDFWRITER_ERROR_DICT_ITEM_NOT_FOUND                  0x1008
//#define AVS_OFFICEPDFWRITER_ERROR_DICT_ITEM_UNEXPECTED_TYPE            0x1009
//#define AVS_OFFICEPDFWRITER_ERROR_DICT_STREAM_LENGTH_NOT_FOUND         0x100A
//#define AVS_OFFICEPDFWRITER_ERROR_DOC_ENCRYPTDICT_NOT_FOUND            0x100B
//#define AVS_OFFICEPDFWRITER_ERROR_DOC_INVALID_OBJECT                   0x100C
////                                                                     0x100D
//#define AVS_OFFICEPDFWRITER_ERROR_DUPLICATE_REGISTRATION               0x100E
//#define AVS_OFFICEPDFWRITER_ERROR_EXCEED_JWW_CODE_NUM_LIMIT            0x100F
////                                                                     0x1010
//#define AVS_OFFICEPDFWRITER_ERROR_ENCRYPT_INVALID_PASSWORD             0x1011
////                                                                     0x1012
//#define AVS_OFFICEPDFWRITER_ERROR_ERR_UNKNOWN_CLASS                    0x1013
//#define AVS_OFFICEPDFWRITER_ERROR_EXCEED_GSTATE_LIMIT                  0x1014
//#define AVS_OFFICEPDFWRITER_ERROR_FAILD_TO_ALLOC_MEM                   0x1015
//#define AVS_OFFICEPDFWRITER_ERROR_FILE_IO_ERROR                        0x1016
//#define AVS_OFFICEPDFWRITER_ERROR_FILE_OPEN_ERROR                      0x1017
//#define AVS_OFFICEPDFWRITER_ERROR_FILE_WRITE_ERROR                     0x1018
//#define AVS_OFFICEPDFWRITER_ERROR_FONT_EXISTS                          0x1019
//#define AVS_OFFICEPDFWRITER_ERROR_FONT_INVALID_WIDTHS_TABLE            0x101A
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_AFM_HEADER                   0x101B
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_ANNOTATION                   0x101C
////                                                                     0x101D
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_BIT_PER_COMPONENT            0x101E
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_CHAR_METRICS_DATA            0x101F
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_COLOR_SPACE                  0x1020
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_COMPRESSION_MODE             0x1021
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_DATE_TIME                    0x1022
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_DESTINATION                  0x1023
////                                                                     0x1024
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_DOCUMENT                     0x1025
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_DOCUMENT_STATE               0x1026
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_ENCODER                      0x1027
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_ENCODER_TYPE                 0x1028
////                                                                     0x1029
////                                                                     0x102A
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_ENCODING_NAME                0x102B
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_ENCRYPT_KEY_LEN              0x102C
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_FONTDEF_DATA                 0x102D
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_FONTDEF_TYPE                 0x102E
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_FONT_NAME                    0x102F
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_IMAGE                        0x1030
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_JPEG_DATA                    0x1031
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_N_DATA                       0x1032
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT                       0x1033
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJ_ID                       0x1034
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_OPERATION                    0x1035
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_OUTLINE                      0x1036
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_PAGE                         0x1037
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_PAGES                        0x1038
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_PARAMETER                    0x1039
////                                                                     0x103A
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_PNG_IMAGE                    0x103B
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_STREAM                       0x103C
//#define AVS_OFFICEPDFWRITER_ERROR_MISSING_FILE_NAME_ENTRY              0x103D
////                                                                     0x103E
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_TTC_FILE                     0x103F
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_TTC_INDEX                    0x1040
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_WX_DATA                      0x1041
//#define AVS_OFFICEPDFWRITER_ERROR_ITEM_NOT_FOUND                       0x1042
//#define AVS_OFFICEPDFWRITER_ERROR_LIBPNG_ERROR                         0x1043
//#define AVS_OFFICEPDFWRITER_ERROR_NAME_INVALID_VALUE                   0x1044
//#define AVS_OFFICEPDFWRITER_ERROR_NAME_OUT_OF_RANGE                    0x1045
////                                                                     0x1046
////                                                                     0x1047
//#define AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_PARAM_COUNT             0x1048
//#define AVS_OFFICEPDFWRITER_ERROR_PAGES_MISSING_KIDS_ENTRY             0x1049
//#define AVS_OFFICEPDFWRITER_ERROR_PAGE_CANNOT_FIND_OBJECT              0x104A
//#define AVS_OFFICEPDFWRITER_ERROR_PAGE_CANNOT_GET_ROOT_PAGES           0x104B
//#define AVS_OFFICEPDFWRITER_ERROR_PAGE_CANNOT_RESTORE_GSTATE           0x104C
//#define AVS_OFFICEPDFWRITER_ERROR_PAGE_CANNOT_SET_PARENT               0x104D
//#define AVS_OFFICEPDFWRITER_ERROR_PAGE_FONT_NOT_FOUND                  0x104E
//#define AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_FONT                    0x104F
//#define AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_FONT_SIZE               0x1050
//#define AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_GMODE                   0x1051
//#define AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_INDEX                   0x1052
//#define AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_ROTATE_VALUE            0x1053
//#define AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_SIZE                    0x1054
//#define AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_XOBJECT                 0x1055
//#define AVS_OFFICEPDFWRITER_ERROR_PAGE_OUT_OF_RANGE                    0x1056
//#define AVS_OFFICEPDFWRITER_ERROR_REAL_OUT_OF_RANGE                    0x1057
//#define AVS_OFFICEPDFWRITER_ERROR_STREAM_EOF                           0x1058
//#define AVS_OFFICEPDFWRITER_ERROR_STREAM_READLN_CONTINUE               0x1059
////                                                                     0x105A
//#define AVS_OFFICEPDFWRITER_ERROR_STRING_OUT_OF_RANGE                  0x105B
//#define AVS_OFFICEPDFWRITER_ERROR_THIS_FUNC_WAS_SKIPPED                0x105C
//#define AVS_OFFICEPDFWRITER_ERROR_TTF_CANNOT_EMBEDDING_FONT            0x105D
//#define AVS_OFFICEPDFWRITER_ERROR_TTF_INVALID_CMAP                     0x105E
//#define AVS_OFFICEPDFWRITER_ERROR_TTF_INVALID_FORMAT                   0x105F
//#define AVS_OFFICEPDFWRITER_ERROR_TTF_MISSING_TABLE                    0x1060
//#define AVS_OFFICEPDFWRITER_ERROR_UNSUPPORTED_FONT_TYPE                0x1061
//#define AVS_OFFICEPDFWRITER_ERROR_UNSUPPORTED_FUNC                     0x1062
//#define AVS_OFFICEPDFWRITER_ERROR_UNSUPPORTED_JPEG_FORMAT              0x1063
//#define AVS_OFFICEPDFWRITER_ERROR_UNSUPPORTED_TYPE1_FONT               0x1064
//#define AVS_OFFICEPDFWRITER_ERROR_XREF_COUNT_ERR                       0x1065
//#define AVS_OFFICEPDFWRITER_ERROR_ZLIB_ERROR                           0x1066
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_PAGE_INDEX                   0x1067
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_URL                          0x1068
//#define AVS_OFFICEPDFWRITER_ERROR_PAGE_LAYOUT_OUT_OF_RANGE             0x1069
//#define AVS_OFFICEPDFWRITER_ERROR_PAGE_MODE_OUT_OF_RANGE               0x1070
//#define AVS_OFFICEPDFWRITER_ERROR_PAGE_NUM_STYLE_OUT_OF_RANGE          0x1071
//#define AVS_OFFICEPDFWRITER_ERROR_ANNOT_INVALID_ICON                   0x1072
//#define AVS_OFFICEPDFWRITER_ERROR_ANNOT_INVALID_BORDER_STYLE           0x1073
//#define AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_DIRECTION               0x1074
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_FONT                         0x1075
//#define AVS_OFFICEPDFWRITER_ERROR_PAGE_INSUFFICIENT_SPACE              0x1076
//#define AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_DISPLAY_TIME            0x1077
//#define AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_TRANSITION_TIME         0x1078
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_PAGE_SLIDESHOW_TYPE          0x1079
//#define AVS_OFFICEPDFWRITER_ERROR_EXT_GSTATE_OUT_OF_RANGE              0x1080
//#define AVS_OFFICEPDFWRITER_ERROR_INVALID_EXT_GSTATE                   0x1081
//#define AVS_OFFICEPDFWRITER_ERROR_EXT_GSTATE_READ_ONLY                 0x1082
//#define AVS_OFFICEPDFWRITER_ERROR_CURRENT_DESTINATION_NOT_FOUND        0x1083
//#define AVS_OFFICEPDFWRITER_ERROR_CURRENT_EXTGSTATE_NOT_FOUND          0x1084        
//#define AVS_OFFICEPDFWRITER_ERROR_CURRENT_FONT_NOT_FOUND               0x1085             
//#define AVS_OFFICEPDFWRITER_ERROR_CURRENT_ANNOTATION_NOT_FOUND         0x1086       
//#define AVS_OFFICEPDFWRITER_ERROR_CURRENT_OUTLINE_NOT_FOUND            0x1087          
//#define AVS_OFFICEPDFWRITER_ERROR_CURRENT_IMAGE_NOT_FOUND              0x1088            


typedef struct TErrorPair 
{
    unsigned long nErrorNo;
    const char   *sErrorName;
} ErrorPair;

static const ErrorPair c_aoErrorNameMap[] = 
{
	{ NOERROR,                                                      "No error"},
	{ AVS_OFFICEPDFWRITER_ERROR_ARRAY_COUNT_ERR,                    "Array: Count error"},
	{ AVS_OFFICEPDFWRITER_ERROR_ARRAY_ITEM_NOT_FOUND,               "Arrar: Item not found"},
	{ AVS_OFFICEPDFWRITER_ERROR_ARRAY_ITEM_UNEXPECTED_TYPE,         "Array: Item unexpected type"},
	{ AVS_OFFICEPDFWRITER_ERROR_BINARY_LENGTH_ERR,                  "Binary length error"},
	{ AVS_OFFICEPDFWRITER_ERROR_CANNOT_GET_PALLET,                  "Cannot get pallet"},
	{ AVS_OFFICEPDFWRITER_ERROR_DICT_COUNT_ERR,                     "Dictionary: Count error"},
	{ AVS_OFFICEPDFWRITER_ERROR_DICT_ITEM_NOT_FOUND,                "Dictionary: Item not found"}, 
	{ AVS_OFFICEPDFWRITER_ERROR_DICT_ITEM_UNEXPECTED_TYPE,          "Dictionary: Item unexpected type"},
	{ AVS_OFFICEPDFWRITER_ERROR_DICT_STREAM_LENGTH_NOT_FOUND,       "Dictionary Stream: Length not found"},
	{ AVS_OFFICEPDFWRITER_ERROR_DOC_ENCRYPTDICT_NOT_FOUND,          "Document: Encrypt Dictionary not found"},
	{ AVS_OFFICEPDFWRITER_ERROR_DOC_INVALID_OBJECT,                 "Document: Invalid object"},

	{ AVS_OFFICEPDFWRITER_ERROR_DUPLICATE_REGISTRATION,             "Duplicate registration"},
	{ AVS_OFFICEPDFWRITER_ERROR_EXCEED_JWW_CODE_NUM_LIMIT,          "Exceed JWW code number limit"},

	{ AVS_OFFICEPDFWRITER_ERROR_ENCRYPT_INVALID_PASSWORD,           "Encrypt: Invalid password"},

	{ AVS_OFFICEPDFWRITER_ERROR_ERR_UNKNOWN_CLASS,                  "Unknown object class"},
	{ AVS_OFFICEPDFWRITER_ERROR_EXCEED_GSTATE_LIMIT,                "Exceed GState limit"},
	{ AVS_OFFICEPDFWRITER_ERROR_FAILD_TO_ALLOC_MEM,                 "Failed to allocate memory"},
	{ AVS_OFFICEPDFWRITER_ERROR_FILE_IO_ERROR,                      "File IO error"},
	{ AVS_OFFICEPDFWRITER_ERROR_FILE_OPEN_ERROR,                    "File open error"},
	{ AVS_OFFICEPDFWRITER_ERROR_FILE_WRITE_ERROR,                   "File write error"},
	{ AVS_OFFICEPDFWRITER_ERROR_FONT_EXISTS,                        "Font already exist"},
	{ AVS_OFFICEPDFWRITER_ERROR_FONT_INVALID_WIDTHS_TABLE,          "Invalid fonts Widths Table"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_AFM_HEADER,                 "Invalid AFM header"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_ANNOTATION,                 "Invalid annotation"},

	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_BIT_PER_COMPONENT,          "Invalid bits per component"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_CHAR_METRICS_DATA,          "Invalid char metrics data"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_COLOR_SPACE,                "Invalid color space"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_COMPRESSION_MODE,           "Invalid compression mode"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_DATE_TIME,                  "Invalid date time"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_DESTINATION,                "Invalid destination"},

	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_DOCUMENT,                   "Invalid document"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_DOCUMENT_STATE,             "Invalid document state"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_ENCODER,                    "Invalid encoder"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_ENCODER_TYPE,               "Invalid encoder type"},


	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_ENCODING_NAME,              "Invalid encoder name"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_ENCRYPT_KEY_LEN,            "Invalid encrypt key len"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_FONTDEF_DATA,               "Invalid fontdef data"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_FONTDEF_TYPE,               "Invalid fontdef type"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_FONT_NAME,                  "Invalid font name"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_IMAGE,                      "Invalid image"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_JPEG_DATA,                  "Invalid jpeg data"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_N_DATA,                     "Invalid PostScripts name"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT,                     "Invalid object"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJ_ID,                     "Invalid object ID"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_OPERATION,                  "Invalid operation"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_OUTLINE,                    "Invalid outline"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_PAGE,                       "Invalid page"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_PAGES,                      "Invalid pages"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_PARAMETER,                  "Invalid parameter"},

	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_PNG_IMAGE,                  "Invalid png image"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_STREAM,                     "Invalid stream"},
	{ AVS_OFFICEPDFWRITER_ERROR_MISSING_FILE_NAME_ENTRY,            "Missing file name entry"},

	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_TTC_FILE,                   "Invalid True Type Collection file"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_TTC_INDEX,                  "Invalid True Type Collection index"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_WX_DATA,                    "Invalid widths data in AFM file"},
	{ AVS_OFFICEPDFWRITER_ERROR_ITEM_NOT_FOUND,                     "Item not found"},
	{ AVS_OFFICEPDFWRITER_ERROR_LIBPNG_ERROR,                       "Libpng error"},
	{ AVS_OFFICEPDFWRITER_ERROR_NAME_INVALID_VALUE,                 "Name: Invalid value"},
	{ AVS_OFFICEPDFWRITER_ERROR_NAME_OUT_OF_RANGE,                  "Name: Out of range"},

	{ AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_PARAM_COUNT,           "Page: Invalid parameters count"},
	{ AVS_OFFICEPDFWRITER_ERROR_PAGES_MISSING_KIDS_ENTRY,           "Pages: Missing kids entry"},
	{ AVS_OFFICEPDFWRITER_ERROR_PAGE_CANNOT_FIND_OBJECT,            "Page: Can't find object"},
	{ AVS_OFFICEPDFWRITER_ERROR_PAGE_CANNOT_GET_ROOT_PAGES,         "Page: Can't get root Pages"},
	{ AVS_OFFICEPDFWRITER_ERROR_PAGE_CANNOT_RESTORE_GSTATE,         "Page: Can't restore GState"},
	{ AVS_OFFICEPDFWRITER_ERROR_PAGE_CANNOT_SET_PARENT,             "Page: Can't set parent"},
	{ AVS_OFFICEPDFWRITER_ERROR_PAGE_FONT_NOT_FOUND,                "Page: Font not found"},
	{ AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_FONT,                  "Page: Invalid font"},
	{ AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_FONT_SIZE,             "Page: Invalid font size"},
	{ AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_GMODE,                 "Page: Invalid graphics mode"},
	{ AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_INDEX,                 "Page: Invalid index"},
	{ AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_ROTATE_VALUE,          "Page: Invalid rotate value"},
	{ AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_SIZE,                  "Page: Invalid size"},
	{ AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_XOBJECT,               "Page: Invalid XObject"},
	{ AVS_OFFICEPDFWRITER_ERROR_PAGE_OUT_OF_RANGE,                  "Page: Out of Range"},
	{ AVS_OFFICEPDFWRITER_ERROR_REAL_OUT_OF_RANGE,                  "Real: Out of Range"},
	{ AVS_OFFICEPDFWRITER_ERROR_STREAM_EOF,                         "Stream: EOF"},
	{ AVS_OFFICEPDFWRITER_ERROR_STREAM_READLN_CONTINUE,             "Stream: Read line continiue"},

	{ AVS_OFFICEPDFWRITER_ERROR_STRING_OUT_OF_RANGE,                "String: Out of Range"},
	{ AVS_OFFICEPDFWRITER_ERROR_THIS_FUNC_WAS_SKIPPED,              "This function was skipped"},
	{ AVS_OFFICEPDFWRITER_ERROR_TTF_CANNOT_EMBEDDING_FONT,          "True Type Font: Can't embedding font"},
	{ AVS_OFFICEPDFWRITER_ERROR_TTF_INVALID_CMAP,                   "True Type Font: Invalid CMap"},
	{ AVS_OFFICEPDFWRITER_ERROR_TTF_INVALID_FORMAT,                 "True Type Font: Invalid format"},
	{ AVS_OFFICEPDFWRITER_ERROR_TTF_MISSING_TABLE,                  "True Type Font: Missing table"},
	{ AVS_OFFICEPDFWRITER_ERROR_UNSUPPORTED_FONT_TYPE,              "Unsupported font type"},
	{ AVS_OFFICEPDFWRITER_ERROR_UNSUPPORTED_FUNC,                   "Unsupported function"},
	{ AVS_OFFICEPDFWRITER_ERROR_UNSUPPORTED_JPEG_FORMAT,            "Unsupported jpeg format"},
	{ AVS_OFFICEPDFWRITER_ERROR_UNSUPPORTED_TYPE1_FONT,             "Unsupported Type1 font"},
	{ AVS_OFFICEPDFWRITER_ERROR_XREF_COUNT_ERR,                     "XRef: Count error"},
	{ AVS_OFFICEPDFWRITER_ERROR_ZLIB_ERROR,                         "ZLIB: Error"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_PAGE_INDEX,                 "Invalid page index"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_URL,                        "Invalid URL"},
	{ AVS_OFFICEPDFWRITER_ERROR_PAGE_LAYOUT_OUT_OF_RANGE,           "Page Layout out of range"},
	{ AVS_OFFICEPDFWRITER_ERROR_PAGE_MODE_OUT_OF_RANGE,             "Page Mode out of range"},
	{ AVS_OFFICEPDFWRITER_ERROR_PAGE_NUM_STYLE_OUT_OF_RANGE,        "Page Num style out of range"},
	{ AVS_OFFICEPDFWRITER_ERROR_ANNOT_INVALID_ICON,                 "Annotation: Invalid icon"},
	{ AVS_OFFICEPDFWRITER_ERROR_ANNOT_INVALID_BORDER_STYLE,         "Annotation: Invalid border style"},
	{ AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_DIRECTION,             "Page: Invalid direction"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_FONT,                       "Invalid font"},
	{ AVS_OFFICEPDFWRITER_ERROR_PAGE_INSUFFICIENT_SPACE,            "Page: Insufficient space"},
	{ AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_DISPLAY_TIME,          "Page: Invalid display time"},
	{ AVS_OFFICEPDFWRITER_ERROR_PAGE_INVALID_TRANSITION_TIME,       "Page: Invalid transition time"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_PAGE_SLIDESHOW_TYPE,        "Invalid slideshow type"},
	{ AVS_OFFICEPDFWRITER_ERROR_EXT_GSTATE_OUT_OF_RANGE,            "ExtGState out of range"},
	{ AVS_OFFICEPDFWRITER_ERROR_INVALID_EXT_GSTATE,                 "Invalid ExtGState"},
	{ AVS_OFFICEPDFWRITER_ERROR_EXT_GSTATE_READ_ONLY,               "ExtGState type is read only"},
	{ AVS_OFFICEPDFWRITER_ERROR_CURRENT_DESTINATION_NOT_FOUND,      "Current Destination not found"},
	{ AVS_OFFICEPDFWRITER_ERROR_CURRENT_EXTGSTATE_NOT_FOUND,        "Current ExtGState not found"},
	{ AVS_OFFICEPDFWRITER_ERROR_CURRENT_FONT_NOT_FOUND,             "Current Font not found"},
	{ AVS_OFFICEPDFWRITER_ERROR_CURRENT_ANNOTATION_NOT_FOUND,       "Current Annotation not found"},
	{ AVS_OFFICEPDFWRITER_ERROR_CURRENT_OUTLINE_NOT_FOUND,          "Current Outline not found"},
	{ AVS_OFFICEPDFWRITER_ERROR_CURRENT_IMAGE_NOT_FOUND,            "Current Image not found"},
	{ AVS_OFFICEPDFWRITER_ERROR_FONT_NOT_FOUND,                     "Font not found"},
	{ AVS_OFFICEPDFWRITER_ERROR_DRAW_PATH,                          "Wrong Draw Path type"},
	{ AVS_OFFICEPDFWRITER_ERROR_BAD_SAFEARRAY,                      "Bad Safe Array"},   
	{ AVS_OFFICEPDFWRITER_ERROR_AVS_REGISTRATION,                   "AVSRegistration turn off"},
	{ 0xFFFF,                                                       NULL}

};

//
//  Error 
//

typedef struct  TErrorRec  *Error;

typedef struct  TErrorRec 
{
    unsigned long           nErrorNo;
    unsigned long           nDetailNo;
    ErrorHandler            ErrorFn;
    void                   *pUserData;
} ErrorRec;


/*  ErrorInit
 *
 *  Если ErrorFn = NULL, тогда по умолчанию устанавливается error-handler.
 *  pUserData используется для определения объекта, в котором обнаружилась ошибка.
 *
 */
void ErrorInit                  (Error oError, void *pUserData)
{
	UtilsMemSet( oError, 0, sizeof(ErrorRec) );

	oError->pUserData = pUserData;
}

void ErrorReset                 (Error oError)
{
	oError->nErrorNo  = NOERROR;
	oError->nDetailNo = NOERROR;
}
unsigned long ErrorGetCode      (Error oError)
{
	return oError->nErrorNo;
}

unsigned long ErrorGetDetailCode(Error oError)
{
	return oError->nDetailNo;
}
const char *  ErrorGetString    (Error oError);
unsigned long SetError          (Error oError, unsigned long nErrorNo, unsigned long nDetailNo)
{
	oError->nErrorNo  = nErrorNo;
	oError->nDetailNo = nDetailNo;
	const char *sError = ErrorGetString( oError );

    return nErrorNo;
}

unsigned long CheckError        (Error oError)
{
	if ( oError->nErrorNo != OK && oError->ErrorFn )
		oError->ErrorFn( oError->nErrorNo, oError->nDetailNo, oError->pUserData);

	return oError->nErrorNo;
}

unsigned long RaiseError        (Error oError, unsigned long nErrorNo, unsigned long nDetailNo)
{
    SetError( oError, nErrorNo, nDetailNo);

    return CheckError(oError);
}

void          CopyError         (Error oDst, Error oSrc)
{
	oDst->nErrorNo  = oSrc->nErrorNo;
	oDst->nDetailNo = oSrc->nDetailNo;
	oDst->ErrorFn   = oSrc->ErrorFn;
	oDst->pUserData = oSrc->pUserData;
}

const char *  ErrorGetString    (Error oError)
{
    const ErrorPair* pMap = c_aoErrorNameMap;

	while ( pMap->nErrorNo <= oError->nErrorNo ) 
	{
		if ( pMap->nErrorNo == oError->nErrorNo )
			return pMap->sErrorName;
        pMap++;
    }

	return c_aoErrorNameMap[0].sErrorName;

}
#endif /* _ERROR_H */

