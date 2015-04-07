#ifndef _LIST_H
#define _LIST_H

#pragma once 

#include "MemoryManager.h"
#include "Utils.h"
#include "Errors.h"

typedef struct TListRec  *List;

typedef struct TListRec 
{
      MMgr         oMMgr;
      Error        oError;
      unsigned int nBlockSize;
      unsigned int nItemsPerBlock;
      unsigned int nCount;
      void       **ppObj;
} ListRec;


/*
 *  Resize
 *
 *  pList  : Указатель на объект List.
 *  nCount : Разера массива указателей.
 *
 *  return:  success, returns OK.
 *           otherwise, returns error-code, который записан в MMgr объекте.
 *
 */

unsigned long Resize(List pList, unsigned int nCount);
/*
 *  ListNew
 *
 *  oMMgr          :  описатель объекта MMgr.
 *  nItemsPerBlock :  количество указателей.
 *
 *  return:  success, возвращает handle на новый List объект
 *           otherwise, возвращает NULL.
 *
 */

List ListNew(MMgr oMMgr, unsigned int nItemsPerBlock);
/*
 *  ListAdd
 *
 *  pList :  Указатель на объект List.
 *  pItem :  Указатель на объект, который нужно добавить.
 *
 *  return:  success, returns OK.
 *           возвращает AVS_OFFICEPDFWRITER_ERROR_FAILD_TO_ALLOC_MEM, когда расширение объекта List невозможно
 *
 */

unsigned long ListAdd(List pList, void *pItem);
/*
 *  ListFind
 *
 *  pList  : Указатель на объект List.
 *  nCount : Разера массива указателей.
 *
 *  return:  success, возвращет номер объекта.
 *           otherwise, возвращает отрицательное значение.
 *
 */

int ListFind  (List pList, void *pItem);
/*
 *  ListInsert
 *
 *  pList   :  Указатель на объект List.
 *  pTarget :  Указатель на объект, который двигаем.
 *  pItem   :  Указатель на объект, который вставляем в список.
 *
 *  Добавляем новый объект pItem перед pTarget в списке.
 *
 *  return:  success, return OK.
 *           возвращает AVS_OFFICEPDFWRITER_ERROR_FAILD_TO_ALLOC_MEM, когда расширение объекта List невозможно
 *           возвращает AVS_OFFICEPDFWRITER_ERROR_ITEM_NOT_FOUND, когда объект который двигаем не найдем
 *
 */

unsigned long ListInsert(List pList, void *pTarget, void *pItem);
/*
 *  ListRemoveByIndex
 *
 *  Удаляем объект из списка, определенный номером в списке.
 *
 *  pList  :  Указатель на объект List.
 *  nIndex :  Номер объекта, который нужно удалить.
 *
 *  return:  success, возвращает указатель на удаленный из списка объект.
 *           возвращает NULL, если объект не найден в списке.
 *
 */

void* ListRemoveByIndex( List pList, unsigned int nIndex );
/*
 *  ListRemove
 *
 *  Удаляем объект из списка, определенный параметром pItem.
 *  Область памяти, занимаемая объектом, не освобождается.
 *
 *  pList :  Указатель на объект List.
 *  pItem :  Указатель на объект, который удаляем.
 *
 *  return:  success, return OK.
 *           возвращает AVS_OFFICEPDFWRITER_ERROR_ITEM_NOT_FOUND, если объект не найден в списке.
 *
 */

unsigned long ListRemove( List pList, void *pItem );
/*
 *  ListItemAt
 *
 *  pList  :  Указатель на объект List.
 *  pIndex :  Номер объекта.
 *
 *  return:  success, возвращает указатель на объект.
 *           otherwise, возвращает NULL.
 *
 */

void* ListItemAt( List pList, unsigned int nIndex);
/*
 *  ListClear
 *
 *  pList :  Указатель на объект List.
 *
 */

void ListClear( List pList );
/*
 * Listfree
 * 
 * Удаляем сам список и все объекты ему принадлежащие.
 *
 * pList :  Указатель на объект List.
 *
 */

void ListFree( List pList );
/*
 * Listfree2
 * 
 * Удаляем сам список, но все объекты ему принадлежащие
 * оставляем в памяти.
 *
 * pList :  Указатель на объект List.
 *
 */

void ListFree2( List pList );
#endif /* _LIST_H */

