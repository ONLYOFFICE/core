#ifndef DOCBUILDER_FUNCTIONS_H
#define DOCBUILDER_FUNCTIONS_H

#include "docbuilder.h"

#include "../common/base_export.h"
#ifdef DOCBUILDER_FUNCTIONS_EXPORT
#define DOCBUILDER_FUNC_DECL Q_DECL_EXPORT
#else
#define DOCBUILDER_FUNC_DECL Q_DECL_IMPORT
#endif

using namespace NSDoctRenderer;

extern "C"
{
// ===== CDocBuilderValue =====
DOCBUILDER_FUNC_DECL CDocBuilderValue* CDocBuilderValue_Create();
DOCBUILDER_FUNC_DECL CDocBuilderValue* CDocBuilderValue_Copy(CDocBuilderValue* other);
DOCBUILDER_FUNC_DECL void CDocBuilderValue_Destroy(CDocBuilderValue* self);

DOCBUILDER_FUNC_DECL bool CDocBuilderValue_IsEmpty(CDocBuilderValue* self);
DOCBUILDER_FUNC_DECL void CDocBuilderValue_Clear(CDocBuilderValue* self);

DOCBUILDER_FUNC_DECL bool CDocBuilderValue_IsNull(CDocBuilderValue* self);
DOCBUILDER_FUNC_DECL bool CDocBuilderValue_IsUndefined(CDocBuilderValue* self);
DOCBUILDER_FUNC_DECL bool CDocBuilderValue_IsInt(CDocBuilderValue* self);
DOCBUILDER_FUNC_DECL bool CDocBuilderValue_IsDouble(CDocBuilderValue* self);
DOCBUILDER_FUNC_DECL bool CDocBuilderValue_IsString(CDocBuilderValue* self);
DOCBUILDER_FUNC_DECL bool CDocBuilderValue_IsFunction(CDocBuilderValue* self);
DOCBUILDER_FUNC_DECL bool CDocBuilderValue_IsObject(CDocBuilderValue* self);
DOCBUILDER_FUNC_DECL bool CDocBuilderValue_IsArray(CDocBuilderValue* self);
// TODO:
// DOCBUILDER_FUNC_DECL bool CDocBuilderValue_IsTypedArray(CDocBuilderValue* self);

DOCBUILDER_FUNC_DECL unsigned int CDocBuilderValue_GetLength(CDocBuilderValue* self);

DOCBUILDER_FUNC_DECL bool CDocBuilderValue_ToBool(CDocBuilderValue* self);
DOCBUILDER_FUNC_DECL int CDocBuilderValue_ToInt(CDocBuilderValue* self);
DOCBUILDER_FUNC_DECL double CDocBuilderValue_ToDouble(CDocBuilderValue* self);
DOCBUILDER_FUNC_DECL const wchar_t* CDocBuilderValue_ToString(CDocBuilderValue* self);

DOCBUILDER_FUNC_DECL CDocBuilderValue* CDocBuilderValue_GetProperty(CDocBuilderValue* self, const wchar_t* name);
DOCBUILDER_FUNC_DECL CDocBuilderValue* CDocBuilderValue_GetByIndex(CDocBuilderValue* self, int index);

DOCBUILDER_FUNC_DECL void CDocBuilderValue_SetProperty(CDocBuilderValue* self, const wchar_t* name, CDocBuilderValue* value);
DOCBUILDER_FUNC_DECL void CDocBuilderValue_SetByIndex(CDocBuilderValue* self, int index, CDocBuilderValue* value);

DOCBUILDER_FUNC_DECL CDocBuilderValue* CDocBuilderValue_CreateWithBool(bool value);
DOCBUILDER_FUNC_DECL CDocBuilderValue* CDocBuilderValue_CreateWithInt(int value);
DOCBUILDER_FUNC_DECL CDocBuilderValue* CDocBuilderValue_CreateWithUInt(unsigned int value);
DOCBUILDER_FUNC_DECL CDocBuilderValue* CDocBuilderValue_CreateWithDouble(double value);
DOCBUILDER_FUNC_DECL CDocBuilderValue* CDocBuilderValue_CreateWithString(const wchar_t* value);

DOCBUILDER_FUNC_DECL CDocBuilderValue* CDocBuilderValue_CreateUndefined();
DOCBUILDER_FUNC_DECL CDocBuilderValue* CDocBuilderValue_CreateNull();
DOCBUILDER_FUNC_DECL CDocBuilderValue* CDocBuilderValue_CreateArray(int length);

DOCBUILDER_FUNC_DECL CDocBuilderValue* CDocBuilderValue_Call0(CDocBuilderValue* self, const wchar_t* name);
DOCBUILDER_FUNC_DECL CDocBuilderValue* CDocBuilderValue_Call1(CDocBuilderValue* self, const wchar_t* name, CDocBuilderValue* p1);
DOCBUILDER_FUNC_DECL CDocBuilderValue* CDocBuilderValue_Call2(CDocBuilderValue* self, const wchar_t* name, CDocBuilderValue* p1, CDocBuilderValue* p2);
DOCBUILDER_FUNC_DECL CDocBuilderValue* CDocBuilderValue_Call3(CDocBuilderValue* self, const wchar_t* name, CDocBuilderValue* p1, CDocBuilderValue* p2, CDocBuilderValue* p3);
DOCBUILDER_FUNC_DECL CDocBuilderValue* CDocBuilderValue_Call4(CDocBuilderValue* self, const wchar_t* name, CDocBuilderValue* p1, CDocBuilderValue* p2, CDocBuilderValue* p3, CDocBuilderValue* p4);
DOCBUILDER_FUNC_DECL CDocBuilderValue* CDocBuilderValue_Call5(CDocBuilderValue* self, const wchar_t* name, CDocBuilderValue* p1, CDocBuilderValue* p2, CDocBuilderValue* p3, CDocBuilderValue* p4, CDocBuilderValue* p5);
DOCBUILDER_FUNC_DECL CDocBuilderValue* CDocBuilderValue_Call6(CDocBuilderValue* self, const wchar_t* name, CDocBuilderValue* p1, CDocBuilderValue* p2, CDocBuilderValue* p3, CDocBuilderValue* p4, CDocBuilderValue* p5, CDocBuilderValue* p6);

// ===== CDocBuilder =====
DOCBUILDER_FUNC_DECL CDocBuilder* CDocBuilder_Create();
DOCBUILDER_FUNC_DECL void CDocBuilder_Destroy(CDocBuilder* self);

DOCBUILDER_FUNC_DECL int CDocBuilder_OpenFile(CDocBuilder* self, const wchar_t* path, const wchar_t* params);
DOCBUILDER_FUNC_DECL bool CDocBuilder_CreateFileByType(CDocBuilder* self, int type);
DOCBUILDER_FUNC_DECL bool CDocBuilder_CreateFileByExtension(CDocBuilder* self, const wchar_t* extension);

DOCBUILDER_FUNC_DECL void CDocBuilder_SetTmpFolder(CDocBuilder* self, const wchar_t* folder);

DOCBUILDER_FUNC_DECL int CDocBuilder_SaveFileByType(CDocBuilder* self, int type, const wchar_t* path);
DOCBUILDER_FUNC_DECL int CDocBuilder_SaveFileByTypeWithParams(CDocBuilder* self, int type, const wchar_t* path, const wchar_t* params);
DOCBUILDER_FUNC_DECL int CDocBuilder_SaveFileByExtension(CDocBuilder* self, const wchar_t* extension, const wchar_t* path);
DOCBUILDER_FUNC_DECL int CDocBuilder_SaveFileByExtensionWithParams(CDocBuilder* self, const wchar_t* extension, const wchar_t* path, const wchar_t* params);

DOCBUILDER_FUNC_DECL void CDocBuilder_CloseFile(CDocBuilder* self);

DOCBUILDER_FUNC_DECL bool CDocBuilder_ExecuteCommand(CDocBuilder* self, const wchar_t* command);
DOCBUILDER_FUNC_DECL bool CDocBuilder_ExecuteCommandWithRetValue(CDocBuilder* self, const wchar_t* command, CDocBuilderValue* retValue);

DOCBUILDER_FUNC_DECL bool CDocBuilder_Run(CDocBuilder* self, const wchar_t* path);
DOCBUILDER_FUNC_DECL bool CDocBuilder_RunText(CDocBuilder* self, const wchar_t* commands);

DOCBUILDER_FUNC_DECL void CDocBuilder_SetProperty(CDocBuilder* self, const wchar_t* param, const wchar_t* value);

DOCBUILDER_FUNC_DECL void CDocBuilder_WriteData(CDocBuilder* self, const wchar_t* path, const wchar_t* value, bool append);

DOCBUILDER_FUNC_DECL bool CDocBuilder_IsSaveWithDoctrendererMode(CDocBuilder* self);

DOCBUILDER_FUNC_DECL char* CDocBuilder_GetVersion(CDocBuilder* self);

DOCBUILDER_FUNC_DECL CDocBuilderContext* CDocBuilder_GetContext(CDocBuilder* self);

DOCBUILDER_FUNC_DECL void CDocBuilder_Initialize();
DOCBUILDER_FUNC_DECL void CDocBuilder_InitializeWithDirectory(const wchar_t* directory);
DOCBUILDER_FUNC_DECL void CDocBuilder_Dispose();

// ===== CDocBuilderContextScope =====
DOCBUILDER_FUNC_DECL CDocBuilderContextScope* CDocBuilderContextScope_Create();
DOCBUILDER_FUNC_DECL CDocBuilderContextScope* CDocBuilderContextScope_Copy(CDocBuilderContextScope* other);
DOCBUILDER_FUNC_DECL void CDocBuilderContextScope_Destroy(CDocBuilderContextScope* self);

DOCBUILDER_FUNC_DECL void CDocBuilderContextScope_Close(CDocBuilderContextScope* self);

// ===== CDocBuilderContext =====
DOCBUILDER_FUNC_DECL CDocBuilderContext* CDocBuilderContext_Create();
DOCBUILDER_FUNC_DECL CDocBuilderContext* CDocBuilderContext_Copy(CDocBuilderContext* other);
DOCBUILDER_FUNC_DECL void CDocBuilderContext_Destroy(CDocBuilderContext* self);

DOCBUILDER_FUNC_DECL CDocBuilderValue* CDocBuilderContext_CreateUndefined(CDocBuilderContext* self);
DOCBUILDER_FUNC_DECL CDocBuilderValue* CDocBuilderContext_CreateNull(CDocBuilderContext* self);
DOCBUILDER_FUNC_DECL CDocBuilderValue* CDocBuilderContext_CreateObject(CDocBuilderContext* self);
DOCBUILDER_FUNC_DECL CDocBuilderValue* CDocBuilderContext_CreateArray(CDocBuilderContext* self, int length);
// TODO:
// CDocBuilderContext_CreateTypedArray(CDocBuilderContext* self, unsigned char* buffer);

DOCBUILDER_FUNC_DECL CDocBuilderValue* CDocBuilderContext_GetGlobal(CDocBuilderContext* self);

DOCBUILDER_FUNC_DECL CDocBuilderContextScope* CDocBuilderContext_CreateScope(CDocBuilderContext* self);

DOCBUILDER_FUNC_DECL bool CDocBuilderContext_IsError(CDocBuilderContext* self);

// ===== Utility =====
DOCBUILDER_FUNC_DECL void DeleteWCharP(wchar_t* str);
DOCBUILDER_FUNC_DECL void DeleteCharP(char* str);
}

#endif // DOCBUILDER_FUNCTIONS_H
