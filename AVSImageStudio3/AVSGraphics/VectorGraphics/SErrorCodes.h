#ifndef _SERROR_CODES_H
#define _SERROR_CODES_H

//-------------------------------------------------------------------------------------------------------------------------------

#define SNoError                 0  // Нет ошибок

#define SErrorNoCurrentPoint     1  // У Path нет текущей точки

#define SErrorEmptyPath          2  // В Path нет точек

#define SErrorWrongPath          3  // В Subpath только одна точка

#define SErrorNoSaveState        4  // Стэк состояний SState пустой

#define SErrorOpenFile           5  // Ошибка при открытии файла

#define SErrorNoGlyph            6  // Невозможно получить данный символ

#define SErrorColorModeMismatch  7  // Несостыковка цветовых моделей

#define SErrorSingularMatrix     8  // Матрица вырожденна

#define SErrorTooSmallBMP        9  // Изображение слишком маленькое для сохранение его в BMP

#define SErrorMemory            10  // Недостаточно памяти

#define SErrorTemp              11  // Невозможно создать временный файл

#define SErrorAVSImageStudio    12  // Ошибка при работе с AVSImageStudio

#endif /* _SERROR_CODES_H */
