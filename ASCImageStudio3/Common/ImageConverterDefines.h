#pragma once
#include <windows.h>

#ifndef _IMAGECONVERTERDEFINES_H_ // For Borland compiler
#define _IMAGECONVERTERDEFINES_H_

#define AVS_ERROR_FIRST                   MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0300)

#define IMAGECONVERTER_UNEXPECTED_ERROR         (AVS_ERROR_FIRST + 0x0000) //Неизвестаня ошибка
#define IMAGECONVERTER_ERROR_XML_FORMAT         (AVS_ERROR_FIRST + 0x0001) //Входная XML имеет неправильный формат
#define IMAGECONVERTER_ERROR_CREATE_HANDLE      (AVS_ERROR_FIRST + 0x0002) //Ошибка при создании Mutex or Event or OtherHandle
#define IMAGECONVERTER_ERROR_INPUT_DATA         (AVS_ERROR_FIRST + 0x0003) //Входные данные
#define IMAGECONVERTER_ERROR_FILE_NOT_EXIST     (AVS_ERROR_FIRST + 0x0004) //Исходный файл не существует
//#define IMAGECONVERTER_ERROR_FILECACHE        (AVS_ERROR_FIRST + 0x0005) //Ошибка FileCache
#define IMAGECONVERTER_ERROR_IMAGESTUDIO        (AVS_ERROR_FIRST + 0x0006) //Ошибка ImageStudio
#define IMAGECONVERTER_ERROR_CONVERT            (AVS_ERROR_FIRST + 0x0007) //Ошибка при конвертации
#define IMAGECONVERTER_ERROR_IMAGEFILE          (AVS_ERROR_FIRST + 0x0008) //Ошибка при открытии файла ImageFile3
#define IMAGECONVERTER_ERROR_REMOVE_TIFF        (AVS_ERROR_FIRST + 0x000a) //ошибка удаления tiff или gif файла при Stop
#define IMAGECONVERTER_ERROR_REMOVE_TEMP_DIR    (AVS_ERROR_FIRST + 0x000b) //ошибка удаления темповой директории при сохранениии в PDF
#define IMAGECONVERTER_ERROR_PDFFILE_CONTROL    (AVS_ERROR_FIRST + 0x000c) //Не удалось создать контрол PdfFile
#define IMAGECONVERTER_ERROR_TIFF_SAVE          (AVS_ERROR_FIRST + 0x000d) //ошибка сохранения в TIFF
#define IMAGECONVERTER_ERROR_GIF_SAVE           (AVS_ERROR_FIRST + 0x000e) //ошибка сохранения в GIF
#define IMAGECONVERTER_ERROR_FILE_ACCESS_READ   (AVS_ERROR_FIRST + 0x000f) //ошибка доступа для чтения
#define IMAGECONVERTER_ERROR_FILE_ACCESS_WRITE  (AVS_ERROR_FIRST + 0x0010) //ошибка доступа для записи
#define IMAGECONVERTER_ERROR_FILE_SAVE			(AVS_ERROR_FIRST + 0x0011) //ошибка доступа для записи

#define IMAGECONVERTER_ERROR_SAVE_PDF_ADDITIONAL    (0x0100) //Прибавка к ошибкам PdfFile




#define IMAGECONVERTER_STATE_IDLE           0x0000
#define IMAGECONVERTER_STATE_RUN            0x0001
#define IMAGECONVERTER_STATE_SUSPENDED      0x0002

#define IMAGECONVERTER_LOGOPARAM_VERT_TOP       1<<0  //1
#define IMAGECONVERTER_LOGOPARAM_VERT_CENTER    1<<1  //2
#define IMAGECONVERTER_LOGOPARAM_VERT_BOTTOM    1<<2  //4

#define IMAGECONVERTER_LOGOPARAM_HOR_LEFT       1<<3  //8
#define IMAGECONVERTER_LOGOPARAM_HOR_CENTER     1<<4  //16
#define IMAGECONVERTER_LOGOPARAM_HOR_RIGHT      1<<5  //32

#endif //#ifndef _IMAGECONVERTERDEFINES_H_  // For Borland compiler