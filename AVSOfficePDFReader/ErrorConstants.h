#ifndef _ERROR_CONSTANTS_H
#define _ERROR_CONSTANTS_H

#define ErrorNone             0 // Нет ошибок

#define ErrorOpenFile         1 // Ошибка при открытии PDF файла

#define ErrorBadCatalog       2	// couldn't read the page catalog

#define ErrorDamaged          3 // PDF файл был поврежден и его невозможно восстановить

#define ErrorEncrypted        4 // Файл зашифрован, авторизация не пройдена

#define ErrorHighlightFile    5	// nonexistent or invalid highlight file

#define ErrorBadPrinter       6 // плохой принтер

#define ErrorPrinting         7 // ошибка во время печати

#define ErrorPermission       8	// Ошибка связанная с ограничениями наложенными на файл

#define ErrorBadPageNum       9	// Неверное количество страниц

#define ErrorFileIO          10 // Ошибка при чтении/записи

#endif /* _ERROR_CONSTANTS_H */
