#ifndef _ERROR_H
#define _ERROR_H


#define AVS_OFFICEFONTMANAGER_ERROR_NOT_ENOUGH_MEMORY             0x0001  // Не хватает памяти

#define AVS_OFFICEFONTMANAGER_ERROR_FONT_OPEN_ERROR               0x0002  // Библиотека FreeType не смогла открыть данный шрифт

#define AVS_OFFICEFONTMANAGER_ERROR_FONT_WASNT_LOAD               0x0003  // Нет текущего шрифта

#define AVS_OFFICEFONTMANAGER_ERROR_WIN_FONT_LIST_NOT_FOUND       0x0004  // Список шрифтов, установленных в Windows, не найден
#define AVS_OFFICEFONTMANAGER_ERROR_WIN_FONT_NOT_FOUND            0x0005  // Шрифт с данным именем не найден
#define AVS_OFFICEFONTMANAGER_ERROR_DEFAULT_FONT_WAS_LOAD         0x0006  // Нужного шрифта не нашли, был загружен стандартный шрифт



#endif /* _ERROR_H */