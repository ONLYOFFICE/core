import ctypes
import os
import platform
import sys

DOCBUILDER_HANDLE = ctypes.c_void_p
STRING_HANDLE = ctypes.c_void_p

_lib = None

def loadLibrary(path):
    if any(platform.win32_ver()):
        os.environ['PATH'] += ';' + path
    else:
        os.environ['PATH'] += ':' + path

    global _lib
    _lib = ctypes.CDLL('docbuilder_functions.dll')

    # init all function signatures
    _lib.CDocBuilder_InitializeWithDirectory.argtypes = [ctypes.c_wchar_p]
    _lib.CDocBuilder_InitializeWithDirectory.restype = None

    _lib.CDocBuilder_Create.argtypes = []
    _lib.CDocBuilder_Create.restype = DOCBUILDER_HANDLE

    _lib.CDocBuilder_GetVersion.argtypes = [DOCBUILDER_HANDLE]
    _lib.CDocBuilder_GetVersion.restype = STRING_HANDLE

    _lib.CDocBuilder_Dispose.argtypes = []
    _lib.CDocBuilder_Dispose.restype = None

    _lib.CDocBuilder_Destroy.argtypes = [DOCBUILDER_HANDLE]
    _lib.CDocBuilder_Destroy.restype = None

    _lib.DeleteCharP.argtypes = [ctypes.c_char_p]
    _lib.DeleteCharP.restype = None


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('Specify the folder with document builder and all dll-s')
        exit(0)

    path = sys.argv[1]

    loadLibrary(path)

    # ---------
    _lib.CDocBuilder_InitializeWithDirectory(path)
    builder = _lib.CDocBuilder_Create()

    sVersion = _lib.CDocBuilder_GetVersion(builder)
    version = ctypes.cast(sVersion, ctypes.c_char_p).value
    print(version)
    _lib.DeleteCharP(ctypes.cast(sVersion, ctypes.c_char_p))

    _lib.CDocBuilder_Dispose()
    _lib.CDocBuilder_Destroy(builder)
