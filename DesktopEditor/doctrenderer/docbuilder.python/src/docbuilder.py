import ctypes
import os
import platform
import sys

OBJECT_HANDLE = ctypes.c_void_p
STRING_HANDLE = ctypes.c_void_p

_lib = None

def loadLibrary(path):
    if any(platform.win32_ver()):
        os.environ['PATH'] += ';' + path
    else:
        os.environ['PATH'] += ':' + path

    global _lib
    _lib = ctypes.CDLL(path + '/docbuilder_functions.dll')

    # init all function signatures
    # ===== CDocBuilderValue =====
    _lib.CDocBuilderValue_Create.argtypes = []
    _lib.CDocBuilderValue_Create.restype = OBJECT_HANDLE

    _lib.CDocBuilderValue_Copy.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilderValue_Copy.restype = OBJECT_HANDLE

    _lib.CDocBuilderValue_Destroy.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilderValue_Destroy.restype = None

    _lib.CDocBuilderValue_IsEmpty.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilderValue_IsEmpty.restype = ctypes.c_bool

    _lib.CDocBuilderValue_Clear.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilderValue_Clear.restype = None

    _lib.CDocBuilderValue_IsNull.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilderValue_IsNull.restype = ctypes.c_bool

    _lib.CDocBuilderValue_IsUndefined.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilderValue_IsUndefined.restype = ctypes.c_bool

    _lib.CDocBuilderValue_IsInt.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilderValue_IsInt.restype = ctypes.c_bool

    _lib.CDocBuilderValue_IsDouble.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilderValue_IsDouble.restype = ctypes.c_bool

    _lib.CDocBuilderValue_IsString.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilderValue_IsString.restype = ctypes.c_bool

    _lib.CDocBuilderValue_IsFunction.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilderValue_IsFunction.restype = ctypes.c_bool

    _lib.CDocBuilderValue_IsObject.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilderValue_IsObject.restype = ctypes.c_bool

    _lib.CDocBuilderValue_IsArray.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilderValue_IsArray.restype = ctypes.c_bool

    _lib.CDocBuilderValue_GetLength.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilderValue_GetLength.restype = ctypes.c_uint

    _lib.CDocBuilderValue_ToBool.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilderValue_ToBool.restype = ctypes.c_bool

    _lib.CDocBuilderValue_ToInt.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilderValue_ToInt.restype = ctypes.c_int

    _lib.CDocBuilderValue_ToDouble.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilderValue_ToDouble.restype = ctypes.c_double

    _lib.CDocBuilderValue_ToString.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilderValue_ToString.restype = STRING_HANDLE

    _lib.CDocBuilderValue_GetProperty.argtypes = [OBJECT_HANDLE, ctypes.c_wchar_p]
    _lib.CDocBuilderValue_GetProperty.restype = OBJECT_HANDLE

    _lib.CDocBuilderValue_GetByIndex.argtypes = [OBJECT_HANDLE, ctypes.c_int]
    _lib.CDocBuilderValue_GetByIndex.restype = OBJECT_HANDLE

    _lib.CDocBuilderValue_SetProperty.argtypes = [OBJECT_HANDLE, ctypes.c_wchar_p, OBJECT_HANDLE]
    _lib.CDocBuilderValue_SetProperty.restype = None

    _lib.CDocBuilderValue_SetByIndex.argtypes = [OBJECT_HANDLE, ctypes.c_int, OBJECT_HANDLE]
    _lib.CDocBuilderValue_SetByIndex.restype = None

    _lib.CDocBuilderValue_CreateWithBool.argtypes = [ctypes.c_bool]
    _lib.CDocBuilderValue_CreateWithBool.restype = OBJECT_HANDLE

    _lib.CDocBuilderValue_CreateWithInt.argtypes = [ctypes.c_int]
    _lib.CDocBuilderValue_CreateWithInt.restype = OBJECT_HANDLE

    _lib.CDocBuilderValue_CreateWithUInt.argtypes = [ctypes.c_uint]
    _lib.CDocBuilderValue_CreateWithUInt.restype = OBJECT_HANDLE

    _lib.CDocBuilderValue_CreateWithDouble.argtypes = [ctypes.c_double]
    _lib.CDocBuilderValue_CreateWithDouble.restype = OBJECT_HANDLE

    _lib.CDocBuilderValue_CreateWithString.argtypes = [ctypes.c_wchar_p]
    _lib.CDocBuilderValue_CreateWithString.restype = OBJECT_HANDLE

    _lib.CDocBuilderValue_CreateUndefined.argtypes = []
    _lib.CDocBuilderValue_CreateUndefined.restype = OBJECT_HANDLE

    _lib.CDocBuilderValue_CreateNull.argtypes = []
    _lib.CDocBuilderValue_CreateNull.restype = OBJECT_HANDLE

    _lib.CDocBuilderValue_CreateArray.argtypes = [ctypes.c_int]
    _lib.CDocBuilderValue_CreateArray.restype = OBJECT_HANDLE

    _lib.CDocBuilderValue_Call0.argtypes = [OBJECT_HANDLE, ctypes.c_wchar_p]
    _lib.CDocBuilderValue_Call0.restype = OBJECT_HANDLE

    _lib.CDocBuilderValue_Call1.argtypes = [OBJECT_HANDLE, ctypes.c_wchar_p, OBJECT_HANDLE]
    _lib.CDocBuilderValue_Call1.restype = OBJECT_HANDLE

    _lib.CDocBuilderValue_Call2.argtypes = [OBJECT_HANDLE, ctypes.c_wchar_p, OBJECT_HANDLE, OBJECT_HANDLE]
    _lib.CDocBuilderValue_Call2.restype = OBJECT_HANDLE

    _lib.CDocBuilderValue_Call3.argtypes = [OBJECT_HANDLE, ctypes.c_wchar_p, OBJECT_HANDLE, OBJECT_HANDLE, OBJECT_HANDLE]
    _lib.CDocBuilderValue_Call3.restype = OBJECT_HANDLE

    _lib.CDocBuilderValue_Call4.argtypes = [OBJECT_HANDLE, ctypes.c_wchar_p, OBJECT_HANDLE, OBJECT_HANDLE, OBJECT_HANDLE, OBJECT_HANDLE]
    _lib.CDocBuilderValue_Call4.restype = OBJECT_HANDLE

    _lib.CDocBuilderValue_Call5.argtypes = [OBJECT_HANDLE, ctypes.c_wchar_p, OBJECT_HANDLE, OBJECT_HANDLE, OBJECT_HANDLE, OBJECT_HANDLE, OBJECT_HANDLE]
    _lib.CDocBuilderValue_Call5.restype = OBJECT_HANDLE

    _lib.CDocBuilderValue_Call6.argtypes = [OBJECT_HANDLE, ctypes.c_wchar_p, OBJECT_HANDLE, OBJECT_HANDLE, OBJECT_HANDLE, OBJECT_HANDLE, OBJECT_HANDLE, OBJECT_HANDLE]
    _lib.CDocBuilderValue_Call6.restype = OBJECT_HANDLE

    # ===== CDocBuilder =====
    _lib.CDocBuilder_Create.argtypes = []
    _lib.CDocBuilder_Create.restype = OBJECT_HANDLE

    _lib.CDocBuilder_Destroy.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilder_Destroy.restype = None

    _lib.CDocBuilder_OpenFile.argtypes = [OBJECT_HANDLE, ctypes.c_wchar_p, ctypes.c_wchar_p]
    _lib.CDocBuilder_OpenFile.restype = ctypes.c_int

    _lib.CDocBuilder_CreateFileByType.argtypes = [OBJECT_HANDLE, ctypes.c_int]
    _lib.CDocBuilder_CreateFileByType.restype = ctypes.c_bool

    _lib.CDocBuilder_CreateFileByExtension.argtypes = [OBJECT_HANDLE, ctypes.c_wchar_p]
    _lib.CDocBuilder_CreateFileByExtension.restype = ctypes.c_bool

    _lib.CDocBuilder_SetTmpFolder.argtypes = [OBJECT_HANDLE, ctypes.c_wchar_p]
    _lib.CDocBuilder_SetTmpFolder.restype = None

    _lib.CDocBuilder_SaveFileByType.argtypes = [OBJECT_HANDLE, ctypes.c_int, ctypes.c_wchar_p]
    _lib.CDocBuilder_SaveFileByType.restype = ctypes.c_int

    _lib.CDocBuilder_SaveFileByTypeWithParams.argtypes = [OBJECT_HANDLE, ctypes.c_int, ctypes.c_wchar_p, ctypes.c_wchar_p]
    _lib.CDocBuilder_SaveFileByTypeWithParams.restype = ctypes.c_int

    _lib.CDocBuilder_SaveFileByExtension.argtypes = [OBJECT_HANDLE, ctypes.c_wchar_p, ctypes.c_wchar_p]
    _lib.CDocBuilder_SaveFileByExtension.restype = ctypes.c_int

    _lib.CDocBuilder_SaveFileByExtensionWithParams.argtypes = [OBJECT_HANDLE, ctypes.c_wchar_p, ctypes.c_wchar_p, ctypes.c_wchar_p]
    _lib.CDocBuilder_SaveFileByExtensionWithParams.restype = ctypes.c_int

    _lib.CDocBuilder_CloseFile.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilder_CloseFile.restype = None

    _lib.CDocBuilder_ExecuteCommand.argtypes = [OBJECT_HANDLE, ctypes.c_wchar_p]
    _lib.CDocBuilder_ExecuteCommand.restype = ctypes.c_bool

    _lib.CDocBuilder_ExecuteCommandWithRetValue.argtypes = [OBJECT_HANDLE, ctypes.c_wchar_p, OBJECT_HANDLE]
    _lib.CDocBuilder_ExecuteCommandWithRetValue.restype = ctypes.c_bool

    _lib.CDocBuilder_Run.argtypes = [OBJECT_HANDLE, ctypes.c_wchar_p]
    _lib.CDocBuilder_Run.restype = ctypes.c_bool

    _lib.CDocBuilder_RunText.argtypes = [OBJECT_HANDLE, ctypes.c_wchar_p]
    _lib.CDocBuilder_RunText.restype = ctypes.c_bool

    _lib.CDocBuilder_SetProperty.argtypes = [OBJECT_HANDLE, ctypes.c_wchar_p, ctypes.c_wchar_p]
    _lib.CDocBuilder_SetProperty.restype = None

    _lib.CDocBuilder_WriteData.argtypes = [OBJECT_HANDLE, ctypes.c_wchar_p, ctypes.c_wchar_p, ctypes.c_bool]
    _lib.CDocBuilder_WriteData.restype = None

    _lib.CDocBuilder_IsSaveWithDoctrendererMode.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilder_IsSaveWithDoctrendererMode.restype = ctypes.c_bool

    _lib.CDocBuilder_GetVersion.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilder_GetVersion.restype = STRING_HANDLE

    _lib.CDocBuilder_GetContext.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilder_GetContext.restype = OBJECT_HANDLE

    _lib.CDocBuilder_Initialize.argtypes = []
    _lib.CDocBuilder_Initialize.restype = None

    _lib.CDocBuilder_InitializeWithDirectory.argtypes = [ctypes.c_wchar_p]
    _lib.CDocBuilder_InitializeWithDirectory.restype = None

    _lib.CDocBuilder_Dispose.argtypes = []
    _lib.CDocBuilder_Dispose.restype = None

    # ===== CDocBuilderContextScope =====
    _lib.CDocBuilderContextScope_Create.argtypes = []
    _lib.CDocBuilderContextScope_Create.restype = OBJECT_HANDLE

    _lib.CDocBuilderContextScope_Copy.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilderContextScope_Copy.restype = OBJECT_HANDLE

    _lib.CDocBuilderContextScope_Destroy.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilderContextScope_Destroy.restype = None

    _lib.CDocBuilderContextScope_Close.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilderContextScope_Close.restype = None

    # ===== CDocBuilderContext =====
    _lib.CDocBuilderContext_Create.argtypes = []
    _lib.CDocBuilderContext_Create.restype = OBJECT_HANDLE

    _lib.CDocBuilderContext_Copy.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilderContext_Copy.restype = OBJECT_HANDLE

    _lib.CDocBuilderContext_Destroy.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilderContext_Destroy.restype = None

    _lib.CDocBuilderContext_CreateUndefined.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilderContext_CreateUndefined.restype = OBJECT_HANDLE

    _lib.CDocBuilderContext_CreateNull.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilderContext_CreateNull.restype = OBJECT_HANDLE

    _lib.CDocBuilderContext_CreateObject.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilderContext_CreateObject.restype = OBJECT_HANDLE

    _lib.CDocBuilderContext_CreateArray.argtypes = [OBJECT_HANDLE, ctypes.c_int]
    _lib.CDocBuilderContext_CreateArray.restype = OBJECT_HANDLE

    _lib.CDocBuilderContext_GetGlobal.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilderContext_GetGlobal.restype = OBJECT_HANDLE

    _lib.CDocBuilderContext_CreateScope.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilderContext_CreateScope.restype = OBJECT_HANDLE

    _lib.CDocBuilderContext_IsError.argtypes = [OBJECT_HANDLE]
    _lib.CDocBuilderContext_IsError.restype = ctypes.c_bool

    # ===== Utility =====
    _lib.DeleteWCharP.argtypes = [ctypes.c_wchar_p]
    _lib.DeleteWCharP.restype = None

    _lib.DeleteCharP.argtypes = [ctypes.c_char_p]
    _lib.DeleteCharP.restype = None

class CDocBuilderValue:
    def __init__(self, value=None):
        if value is None:
            self._internal = _lib.CDocBuilderValue_Create()
        elif isinstance(value, bool):
            self._internal = _lib.CDocBuilderValue_CreateWithBool(ctypes.c_bool(value))
        elif isinstance(value, int):
            self._internal = _lib.CDocBuilderValue_CreateWithInt(ctypes.c_int(value))
        elif isinstance(value, float):
            self._internal = _lib.CDocBuilderValue_CreateWithDouble(ctypes.c_double(value))
        elif isinstance(value, str):
            self._internal = _lib.CDocBuilderValue_CreateWithString(ctypes.c_wchar_p(value))
        elif isinstance(value, CDocBuilderValue):
            self._internal = _lib.CDocBuilderValue_Copy(value._internal)
        elif isinstance(value, OBJECT_HANDLE):
            self._internal = value
        else:
            raise TypeError("Unsupported type for CDocBuilderValue constructor")
        self._lib = _lib

    def __del__(self):
        # using self._lib instead of global _lib because it might be already garbage collected during this function call
        self._lib.CDocBuilderValue_Destroy(self._internal)

    def IsEmpty(self):
        return _lib.CDocBuilderValue_IsEmpty(self._internal)

    def Clear(self):
        _lib.CDocBuilderValue_Clear(self._internal)

    def IsNull(self):
        return _lib.CDocBuilderValue_IsNull(self._internal)

    def IsUndefined(self):
        return _lib.CDocBuilderValue_IsUndefined(self._internal)

    def IsInt(self):
        return _lib.CDocBuilderValue_IsInt(self._internal)

    def IsDouble(self):
        return _lib.CDocBuilderValue_IsDouble(self._internal)

    def IsString(self):
        return _lib.CDocBuilderValue_IsString(self._internal)

    def IsFunction(self):
        return _lib.CDocBuilderValue_IsFunction(self._internal)

    def IsObject(self):
        return _lib.CDocBuilderValue_IsObject(self._internal)

    def IsArray(self):
        return _lib.CDocBuilderValue_IsArray(self._internal)

    def GetLength(self):
        return _lib.CDocBuilderValue_GetLength(self._internal)

    def ToBool(self):
        return _lib.CDocBuilderValue_ToBool(self._internal)

    def ToInt(self):
        return _lib.CDocBuilderValue_ToInt(self._internal)

    def ToDouble(self):
        return _lib.CDocBuilderValue_ToDouble(self._internal)

    def ToString(self):
        strRes = _lib.CDocBuilderValue_ToString(self._internal)
        res = ctypes.cast(strRes, ctypes.c_wchar_p).value
        _lib.DeleteWCharP(ctypes.cast(strRes, ctypes.c_wchar_p))
        return res

    def GetProperty(self, name):
        return CDocBuilderValue(OBJECT_HANDLE(_lib.CDocBuilderValue_GetProperty(self._internal, ctypes.c_wchar_p(name))))

    def Get(self, key):
        if isinstance(key, int):
            return CDocBuilderValue(OBJECT_HANDLE(_lib.CDocBuilderValue_GetByIndex(self._internal, ctypes.c_int(key))))
        elif isinstance(key, str):
            return CDocBuilderValue(OBJECT_HANDLE(_lib.CDocBuilderValue_GetProperty(self._internal, ctypes.c_wchar_p(key))))
        else:
            return None

    def SetProperty(self, name, value):
        _lib.CDocBuilderValue_SetProperty(self._internal, ctypes.c_wchar_p(name), value._internal)

    def Set(self, key, value):
        if isinstance(key, int):
            _lib.CDocBuilderValue_SetByIndex(self._internal, ctypes.c_int(key), value._internal)
        elif isinstance(key, str):
            _lib.CDocBuilderValue_SetProperty(self._internal, ctypes.c_wchar_p(key), value._internal)

    def __getitem__(self, key):
        return self.Get(key)

    def __setitem__(self, key, value):
        self.Set(key, value)

    @staticmethod
    def CreateUndefined():
        return CDocBuilderValue(OBJECT_HANDLE(_lib.CDocBuilderValue_CreateUndefined()))

    @staticmethod
    def CreateNull():
        return CDocBuilderValue(OBJECT_HANDLE(_lib.CDocBuilderValue_CreateNull()))

    @staticmethod
    def CreateArray(length):
        return CDocBuilderValue(OBJECT_HANDLE(_lib.CDocBuilderValue_CreateArray(length)))

    def Call(self, name, *args):
        if len(args) == 0:
            return CDocBuilderValue(OBJECT_HANDLE(_lib.CDocBuilderValue_Call0(self._internal, ctypes.c_wchar_p(name))))
        elif len(args) < 7:
            values = []
            for i in range(len(args)):
                p = args[i]
                if not isinstance(p, CDocBuilderValue):
                    p = CDocBuilderValue(p)
                values.append(p)
            if len(args) == 1:
                return CDocBuilderValue(OBJECT_HANDLE(_lib.CDocBuilderValue_Call1(self._internal, ctypes.c_wchar_p(name), values[0]._internal)))
            elif len(args) == 2:
                return CDocBuilderValue(OBJECT_HANDLE(_lib.CDocBuilderValue_Call2(self._internal, ctypes.c_wchar_p(name), values[0]._internal, values[1]._internal)))
            elif len(args) == 3:
                return CDocBuilderValue(OBJECT_HANDLE(_lib.CDocBuilderValue_Call3(self._internal, ctypes.c_wchar_p(name), values[0]._internal, values[1]._internal, values[2]._internal)))
            elif len(args) == 4:
                return CDocBuilderValue(OBJECT_HANDLE(_lib.CDocBuilderValue_Call4(self._internal, ctypes.c_wchar_p(name), values[0]._internal, values[1]._internal, values[2]._internal, values[3]._internal)))
            elif len(args) == 5:
                return CDocBuilderValue(OBJECT_HANDLE(_lib.CDocBuilderValue_Call5(self._internal, ctypes.c_wchar_p(name), values[0]._internal, values[1]._internal, values[2]._internal, values[3]._internal, values[4]._internal)))
            elif len(args) == 6:
                return CDocBuilderValue(OBJECT_HANDLE(_lib.CDocBuilderValue_Call6(self._internal, ctypes.c_wchar_p(name), values[0]._internal, values[1]._internal, values[2]._internal, values[3]._internal, values[4]._internal, values[5]._internal)))
        else:
            raise TypeError("Call() expects at most 6 arguments")

class CDocBuilder:
    def __init__(self):
        self._internal = _lib.CDocBuilder_Create()
        self._lib = _lib

    def __del__(self):
        # using self._lib instead of global _lib because it might be already garbage collected during this function call
        self._lib.CDocBuilder_Destroy(self._internal)

    def OpenFile(self, path, params):
        return _lib.CDocBuilder_OpenFile(self._internal, ctypes.c_wchar_p(path), ctypes.c_wchar_p(params))

    def CreateFile(self, type):
        if isinstance(type, int):
            return _lib.CDocBuilder_CreateFileByType(self._internal, ctypes.c_int(type))
        elif isinstance(type, str):
            return _lib.CDocBuilder_CreateFileByExtension(self._internal, ctypes.c_wchar_p(type))
        else:
            return False

    def SetTmpFolder(self, folder):
        _lib.CDocBuilder_SetTmpFolder(self._internal, ctypes.c_wchar_p(folder))

    def SaveFile(self, type, path, params=None):
        if isinstance(type, int):
            if params is None:
                return _lib.CDocBuilder_SaveFileByType(self._internal, ctypes.c_int(type), ctypes.c_wchar_p(path))
            else:
                return _lib.CDocBuilder_SaveFileByTypeWithParams(self._internal, ctypes.c_int(type), ctypes.c_wchar_p(path), ctypes.c_wchar_p(params))
        elif isinstance(type, str):
            if params is None:
                return _lib.CDocBuilder_SaveFileByExtension(self._internal, ctypes.c_wchar_p(type), ctypes.c_wchar_p(path))
            else:
                return _lib.CDocBuilder_SaveFileByExtensionWithParams(self._internal, ctypes.c_wchar_p(type), ctypes.c_wchar_p(path), ctypes.c_wchar_p(params))
        else:
            return -1

    def CloseFile(self):
        _lib.CDocBuilder_CloseFile(self._internal)

    def ExecuteCommand(self, command, retValue=None):
        if retValue is None:
            return _lib.CDocBuilder_ExecuteCommand(self._internal, ctypes.c_wchar_p(command))
        else:
            return _lib.CDocBuilder_ExecuteCommandWithRetValue(self._internal, ctypes.c_wchar_p(command), retValue._internal)

    def Run(self, path):
        return _lib.CDocBuilder_Run(self._internal, ctypes.c_wchar_p(path))

    def RunText(self, commands):
        return _lib.CDocBuilder_RunText(self._internal, ctypes.c_wchar_p(commands))

    def SetProperty(self, param, value):
        _lib.CDocBuilder_SetProperty(self._internal, ctypes.c_wchar_p(param), ctypes.c_wchar_p(value))

    def WriteData(self, path, value, append):
        _lib.CDocBuilder_WriteData(self._internal, ctypes.c_wchar_p(path), ctypes.c_wchar_p(value), ctypes.c_bool(append))

    def IsSaveWithDoctrendererMode(self):
        return _lib.CDocBuilder_IsSaveWithDoctrendererMode(self._internal)

    def GetVersion(self):
        strVersion = _lib.CDocBuilder_GetVersion(self._internal)
        version = ctypes.cast(strVersion, ctypes.c_char_p).value
        _lib.DeleteCharP(ctypes.cast(strVersion, ctypes.c_char_p))
        return version

    def GetContext(self):
        return CDocBuilderContext(OBJECT_HANDLE(_lib.CDocBuilder_GetContext(self._internal)))

    @staticmethod
    def Initialize(directory=None):
        if directory is None:
            _lib.CDocBuilder_Initialize()
        else:
            _lib.CDocBuilder_InitializeWithDirectory(ctypes.c_wchar_p(directory))

    @staticmethod
    def Dispose():
        _lib.CDocBuilder_Dispose()

class CDocBuilderContextScope:
    def __init__(self, value=None):
        self._lib = _lib
        if value is None:
            self._internal = _lib.CDocBuilderContextScope_Create()
        elif isinstance(value, CDocBuilderContextScope):
            self._internal = _lib.CDocBuilderContextScope_Copy(value._internal)
        elif isinstance(value, OBJECT_HANDLE):
            self._internal = value
        else:
            raise TypeError("Unsupported type for CDocBuilderContextScope constructor")
        self._lib = _lib

    def __del__(self):
        # using self._lib instead of global _lib because it might be already garbage collected during this function call
        self._lib.CDocBuilderContextScope_Destroy(self._internal)

    def Close(self):
        _lib.CDocBuilderContextScope_Close(self._internal)

class CDocBuilderContext:
    def __init__(self, value=None):
        if value is None:
            self._internal = _lib.CDocBuilderContext_Create()
        elif isinstance(value, CDocBuilderContext):
            self._internal = _lib.CDocBuilderContext_Copy(value._internal)
        elif isinstance(value, OBJECT_HANDLE):
            self._internal = value
        else:
            raise TypeError("Unsupported type for CDocBuilderContext constructor")
        self._lib = _lib

    def __del__(self):
        # using self._lib instead of global _lib because it might be already garbage collected during this function call
        self._lib.CDocBuilderContext_Destroy(self._internal)

    def CreateUndefined(self):
        return CDocBuilderValue(OBJECT_HANDLE(_lib.CDocBuilderContext_CreateUndefined(self._internal)))

    def CreateNull(self):
        return CDocBuilderValue(OBJECT_HANDLE(_lib.CDocBuilderContext_CreateNull(self._internal)))

    def CreateObject(self):
        return CDocBuilderValue(OBJECT_HANDLE(_lib.CDocBuilderContext_CreateObject(self._internal)))

    def CreateArray(self, length):
        return CDocBuilderValue(OBJECT_HANDLE(_lib.CDocBuilderContext_CreateArray(self._internal, ctypes.c_int(length))))

    def GetGlobal(self):
        return CDocBuilderValue(OBJECT_HANDLE(_lib.CDocBuilderContext_GetGlobal(self._internal)))

    def CreateScope(self):
        return CDocBuilderContextScope(OBJECT_HANDLE(_lib.CDocBuilderContext_CreateScope(self._internal)))

    def IsError(self):
        return _lib.CDocBuilderContext_IsError(self._internal)

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('Specify the folder with document builder and all dll-s')
        exit(0)

    path = sys.argv[1]

    loadLibrary(path)

    # ---------
    '''
    _lib.CDocBuilder_InitializeWithDirectory(path)
    builder = _lib.CDocBuilder_Create()

    sVersion = _lib.CDocBuilder_GetVersion(builder)
    version = ctypes.cast(sVersion, ctypes.c_char_p).value
    print(version)
    _lib.DeleteCharP(ctypes.cast(sVersion, ctypes.c_char_p))

    _lib.CDocBuilder_Dispose()
    _lib.CDocBuilder_Destroy(builder)
    '''
    # ---------
    '''
    CDocBuilder.Initialize(path)
    builder = CDocBuilder()

    version = builder.GetVersion()
    print(version)

    CDocBuilder.Dispose()
    '''
    # ---------
    CDocBuilder.Initialize(path)
    builder = CDocBuilder()

    builder.CreateFile('docx')

    context = builder.GetContext()
    scope = context.CreateScope()

    globalObj = context.GetGlobal()

    api = globalObj['Api']
    document = api.Call('GetDocument')
    paragraph = api.Call('CreateParagraph')
    paragraph.Call('SetSpacingAfter', 1000, False)
    paragraph.Call('AddText', 'Hello, World!')
    content = context.CreateArray(1)
    content[0] = paragraph
    document.Call('InsertContent', content)

    dstPath = os.getcwd() + '/result.docx'
    builder.SaveFile('docx', dstPath)
    builder.CloseFile()

    CDocBuilder.Dispose()
