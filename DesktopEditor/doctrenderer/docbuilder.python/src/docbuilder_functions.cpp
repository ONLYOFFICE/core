#include "docbuilder_functions.h"

#include <cstring>
#include <wchar.h>

// ===== CDocBuilderValue =====
CDocBuilderValue* CDocBuilderValue_Create()
{
	return new CDocBuilderValue();
}

CDocBuilderValue* CDocBuilderValue_Copy(CDocBuilderValue* other)
{
	return new CDocBuilderValue(*other);
}

void CDocBuilderValue_Destroy(CDocBuilderValue* self)
{
	delete self;
}

bool CDocBuilderValue_IsEmpty(CDocBuilderValue* self)
{
	return self->IsEmpty();
}

void CDocBuilderValue_Clear(CDocBuilderValue* self)
{
	self->Clear();
}

bool CDocBuilderValue_IsNull(CDocBuilderValue* self)
{
	return self->IsNull();
}

bool CDocBuilderValue_IsUndefined(CDocBuilderValue* self)
{
	return self->IsUndefined();
}

bool CDocBuilderValue_IsInt(CDocBuilderValue* self)
{
	return self->IsInt();
}

bool CDocBuilderValue_IsDouble(CDocBuilderValue* self)
{
	return self->IsDouble();
}

bool CDocBuilderValue_IsString(CDocBuilderValue* self)
{
	return self->IsString();
}

bool CDocBuilderValue_IsFunction(CDocBuilderValue* self)
{
	return self->IsFunction();
}

bool CDocBuilderValue_IsObject(CDocBuilderValue* self)
{
	return self->IsObject();
}

bool CDocBuilderValue_IsArray(CDocBuilderValue* self)
{
	return self->IsArray();
}

unsigned int CDocBuilderValue_GetLength(CDocBuilderValue* self)
{
	return self->GetLength();
}

bool CDocBuilderValue_ToBool(CDocBuilderValue* self)
{
	return self->ToBool();
}

int CDocBuilderValue_ToInt(CDocBuilderValue* self)
{
	return self->ToInt();
}

double CDocBuilderValue_ToDouble(CDocBuilderValue* self)
{
	return self->ToDouble();
}

const wchar_t* CDocBuilderValue_ToString(CDocBuilderValue* self)
{
	// NOTE: User is responsible for calling DeleteWCharP() on returned pointer
	CString strValue = self->ToString();
	size_t len = wcslen(strValue.c_str());
	wchar_t* strRes = new wchar_t[len + 1];
	memcpy(strRes, strValue.c_str(), (len + 1) * sizeof(wchar_t));
	return strRes;
}

CDocBuilderValue* CDocBuilderValue_GetProperty(CDocBuilderValue* self, const wchar_t* name)
{
	return new CDocBuilderValue(self->GetProperty(name));
}

CDocBuilderValue* CDocBuilderValue_GetByIndex(CDocBuilderValue* self, int index)
{
	return new CDocBuilderValue(self->Get(index));
}

void CDocBuilderValue_SetProperty(CDocBuilderValue* self, const wchar_t* name, CDocBuilderValue* value)
{
	self->Set(name, *value);
}

void CDocBuilderValue_SetByIndex(CDocBuilderValue* self, int index, CDocBuilderValue* value)
{
	self->Set(index, *value);
}

CDocBuilderValue* CDocBuilderValue_CreateWithBool(bool value)
{
	return new CDocBuilderValue(value);
}

CDocBuilderValue* CDocBuilderValue_CreateWithInt(int value)
{
	return new CDocBuilderValue(value);
}

CDocBuilderValue* CDocBuilderValue_CreateWithUInt(unsigned int value)
{
	return new CDocBuilderValue(value);
}

CDocBuilderValue* CDocBuilderValue_CreateWithDouble(double value)
{
	return new CDocBuilderValue(value);
}

CDocBuilderValue* CDocBuilderValue_CreateWithString(const wchar_t* value)
{
	return new CDocBuilderValue(value);
}

CDocBuilderValue* CDocBuilderValue_CreateUndefined()
{
	return new CDocBuilderValue(CDocBuilderValue::CreateUndefined());
}

CDocBuilderValue* CDocBuilderValue_CreateNull()
{
	return new CDocBuilderValue(CDocBuilderValue::CreateNull());
}

CDocBuilderValue* CDocBuilderValue_CreateArray(int length)
{
	return new CDocBuilderValue(CDocBuilderValue::CreateArray(length));
}

CDocBuilderValue* CDocBuilderValue_Call0(CDocBuilderValue* self, const wchar_t* name)
{
	return new CDocBuilderValue(self->Call(name));
}

CDocBuilderValue* CDocBuilderValue_Call1(CDocBuilderValue* self, const wchar_t* name, CDocBuilderValue* p1)
{
	return new CDocBuilderValue(self->Call(name, *p1));
}

CDocBuilderValue* CDocBuilderValue_Call2(CDocBuilderValue* self, const wchar_t* name, CDocBuilderValue* p1, CDocBuilderValue* p2)
{
	return new CDocBuilderValue(self->Call(name, *p1, *p2));
}

CDocBuilderValue* CDocBuilderValue_Call3(CDocBuilderValue* self, const wchar_t* name, CDocBuilderValue* p1, CDocBuilderValue* p2, CDocBuilderValue* p3)
{
	return new CDocBuilderValue(self->Call(name, *p1, *p2, *p3));
}

CDocBuilderValue* CDocBuilderValue_Call4(CDocBuilderValue* self, const wchar_t* name, CDocBuilderValue* p1, CDocBuilderValue* p2, CDocBuilderValue* p3, CDocBuilderValue* p4)
{
	return new CDocBuilderValue(self->Call(name, *p1, *p2, *p3, *p4));
}

CDocBuilderValue* CDocBuilderValue_Call5(CDocBuilderValue* self, const wchar_t* name, CDocBuilderValue* p1, CDocBuilderValue* p2, CDocBuilderValue* p3, CDocBuilderValue* p4, CDocBuilderValue* p5)
{
	return new CDocBuilderValue(self->Call(name, *p1, *p2, *p3, *p4, *p5));
}

CDocBuilderValue* CDocBuilderValue_Call6(CDocBuilderValue* self, const wchar_t* name, CDocBuilderValue* p1, CDocBuilderValue* p2, CDocBuilderValue* p3, CDocBuilderValue* p4, CDocBuilderValue* p5, CDocBuilderValue* p6)
{
	return new CDocBuilderValue(self->Call(name, *p1, *p2, *p3, *p4, *p5, *p6));
}

// ===== CDocBuilder =====
CDocBuilder* CDocBuilder_Create()
{
	return new CDocBuilder();
}

void CDocBuilder_Destroy(CDocBuilder* self)
{
	delete self;
}

int CDocBuilder_OpenFile(CDocBuilder* self, const wchar_t* path, const wchar_t* params)
{
	return self->OpenFile(path, params);
}

bool CDocBuilder_CreateFileByType(CDocBuilder* self, int type)
{
	return self->CreateFile(type);
}

bool CDocBuilder_CreateFileByExtension(CDocBuilder* self, const wchar_t* extension)
{
	return self->CreateFile(extension);
}

void CDocBuilder_SetTmpFolder(CDocBuilder* self, const wchar_t* folder)
{
	self->SetTmpFolder(folder);
}

int CDocBuilder_SaveFileByType(CDocBuilder* self, int type, const wchar_t* path)
{
	return self->SaveFile(type, path);
}

int CDocBuilder_SaveFileByTypeWithParams(CDocBuilder* self, int type, const wchar_t* path, const wchar_t* params)
{
	return self->SaveFile(type, path, params);
}

int CDocBuilder_SaveFileByExtension(CDocBuilder* self, const wchar_t* extension, const wchar_t* path)
{
	return self->SaveFile(extension, path);
}

int CDocBuilder_SaveFileByExtensionWithParams(CDocBuilder* self, const wchar_t* extension, const wchar_t* path, const wchar_t* params)
{
	return self->SaveFile(extension, path, params);
}

void CDocBuilder_CloseFile(CDocBuilder* self)
{
	self->CloseFile();
}

bool CDocBuilder_ExecuteCommand(CDocBuilder* self, const wchar_t* command)
{
	return self->ExecuteCommand(command);
}

bool CDocBuilder_ExecuteCommandWithRetValue(CDocBuilder* self, const wchar_t* command, CDocBuilderValue* retValue)
{
	return self->ExecuteCommand(command, retValue);
}

bool CDocBuilder_Run(CDocBuilder* self, const wchar_t* path)
{
	return self->Run(path);
}

bool CDocBuilder_RunText(CDocBuilder* self, const wchar_t* commands)
{
	return self->RunTextW(commands);
}

void CDocBuilder_SetProperty(CDocBuilder* self, const wchar_t* param, const wchar_t* value)
{
	self->SetPropertyW(param, value);
}

void CDocBuilder_WriteData(CDocBuilder* self, const wchar_t* path, const wchar_t* value, bool append)
{
	self->WriteData(path, value, append);
}

bool CDocBuilder_IsSaveWithDoctrendererMode(CDocBuilder* self)
{
	return self->IsSaveWithDoctrendererMode();
}

char* CDocBuilder_GetVersion(CDocBuilder* self)
{
	// NOTE: User is responsible for calling DeleteCharP() on returned pointer
	return self->GetVersion();
}

CDocBuilderContext* CDocBuilder_GetContext(CDocBuilder* self)
{
	return new CDocBuilderContext(self->GetContext());
}

void CDocBuilder_Initialize()
{
	CDocBuilder::Initialize();
}

void CDocBuilder_InitializeWithDirectory(const wchar_t* directory)
{
	CDocBuilder::Initialize(directory);
}

void CDocBuilder_Dispose()
{
	CDocBuilder::Dispose();
}

// ===== CDocBuilderContextScope =====
CDocBuilderContextScope* CDocBuilderContextScope_Create()
{
	return new CDocBuilderContextScope();
}

CDocBuilderContextScope* CDocBuilderContextScope_Copy(CDocBuilderContextScope* other)
{
	return new CDocBuilderContextScope(*other);
}

void CDocBuilderContextScope_Destroy(CDocBuilderContextScope* self)
{
	delete self;
}

void CDocBuilderContextScope_Close(CDocBuilderContextScope* self)
{
	self->Close();
}

// ===== CDocBuilderContext =====
CDocBuilderContext* CDocBuilderContext_Create()
{
	return new CDocBuilderContext();
}

CDocBuilderContext* CDocBuilderContext_Copy(CDocBuilderContext* other)
{
	return new CDocBuilderContext(*other);
}

void CDocBuilderContext_Destroy(CDocBuilderContext* self)
{
	delete self;
}

CDocBuilderValue* CDocBuilderContext_CreateUndefined(CDocBuilderContext* self)
{
	return new CDocBuilderValue(self->CreateUndefined());
}

CDocBuilderValue* CDocBuilderContext_CreateNull(CDocBuilderContext* self)
{
	return new CDocBuilderValue(self->CreateNull());
}

CDocBuilderValue* CDocBuilderContext_CreateObject(CDocBuilderContext* self)
{
	return new CDocBuilderValue(self->CreateObject());
}

CDocBuilderValue* CDocBuilderContext_CreateArray(CDocBuilderContext* self, int length)
{
	return new CDocBuilderValue(self->CreateArray(length));
}

CDocBuilderValue* CDocBuilderContext_GetGlobal(CDocBuilderContext* self)
{
	return new CDocBuilderValue(self->GetGlobal());
}

CDocBuilderContextScope* CDocBuilderContext_CreateScope(CDocBuilderContext* self)
{
	return new CDocBuilderContextScope(self->CreateScope());
}

bool CDocBuilderContext_IsError(CDocBuilderContext* self)
{
	return self->IsError();
}

// ===== Utility =====
void DeleteWCharP(wchar_t* str)
{
	delete[] str;
}

void DeleteCharP(char* str)
{
	delete[] str;
}
