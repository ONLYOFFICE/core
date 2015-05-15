#ifndef _PDF_READER_ERROR_CONSTANTS_H
#define _PDF_READER_ERROR_CONSTANTS_H

namespace PdfReader
{
	typedef enum
	{
		errorNone          =  0, // Нет ошибок
		errorOpenFile      =  1, // Ошибка при открытии PDF файла
		errorBadCatalog    =  2, // couldn't read the page catalog
		errorDamaged       =  3, // PDF файл был поврежден и его невозможно восстановить
		errorEncrypted     =  4, // Файл зашифрован, авторизация не пройдена
		errorHighlightFile =  5, // nonexistent or invalid highlight file
		errorBadPrinter    =  6, // плохой принтер
		errorPrinting      =  7, // ошибка во время печати
		errorPermission    =  8, // Ошибка связанная с ограничениями наложенными на файл
		errorBadPageNum    =  9, // Неверное количество страниц
		errorFileIO        = 10  // Ошибка при чтении/записи
	} EError;
}

#endif // _PDF_READER_ERROR_CONSTANTS_H
