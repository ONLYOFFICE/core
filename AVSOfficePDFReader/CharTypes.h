#ifndef _CHARTYPES_H
#define _CHARTYPES_H

// Unicode-символы.
typedef unsigned int Unicode;

// Для элементов CID.
typedef unsigned int CID;

// Общий тип, рассчитан наодин из следюущих вариантов:
// - 8-битные символы
// - 16-битный CID
// - Unicode
typedef unsigned int CharCode;

#endif /* _CHARTYPES_H */
