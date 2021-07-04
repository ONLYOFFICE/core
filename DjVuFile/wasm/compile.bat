SET SCRIPTPATH=%~dp0
CD /D %~dp0

call ../emsdk/emsdk_env.bat

call em++ -o djvu.js^
 -O3^
 -fno-rtti^
 -s WASM=1^
 -s ALLOW_MEMORY_GROWTH=1^
 -s FILESYSTEM=0^
 -s ENVIRONMENT='web'^
 -s ERROR_ON_UNDEFINED_SYMBOLS=0^
 -s EXPORTED_FUNCTIONS="['_malloc', '_free', '_DJVU_Load', '_DJVU_Close', '_DJVU_GetInfo', '_DJVU_GetPixmap', '_DJVU_Delete']"^
 "libdjvu/Arrays.cpp"^
 "libdjvu/BSByteStream.cpp"^
 "libdjvu/BSEncodeByteStream.cpp"^
 "libdjvu/ByteStream.cpp"^
 "libdjvu/DataPool.cpp"^
 "libdjvu/debug.cpp"^
 "libdjvu/DjVmDir.cpp"^
 "libdjvu/DjVmDir0.cpp"^
 "libdjvu/DjVmDoc.cpp"^
 "libdjvu/DjVmNav.cpp"^
 "libdjvu/DjVuAnno.cpp"^
 "libdjvu/DjVuDocEditor.cpp"^
 "libdjvu/DjVuDocument.cpp"^
 "libdjvu/DjVuDumpHelper.cpp"^
 "libdjvu/DjVuErrorList.cpp"^
 "libdjvu/DjVuFile.cpp"^
 "libdjvu/DjVuFileCache.cpp"^
 "libdjvu/DjVuGlobal.cpp"^
 "libdjvu/DjVuGlobalMemory.cpp"^
 "libdjvu/DjVuImage.cpp"^
 "libdjvu/DjVuInfo.cpp"^
 "libdjvu/DjVuMessageLite.cpp"^
 "libdjvu/DjVuNavDir.cpp"^
 "libdjvu/DjVuPalette.cpp"^
 "libdjvu/DjVuPort.cpp"^
 "libdjvu/DjVuText.cpp"^
 "libdjvu/DjVuToPS.cpp"^
 "libdjvu/GBitmap.cpp"^
 "libdjvu/GContainer.cpp"^
 "libdjvu/GException.cpp"^
 "libdjvu/GIFFManager.cpp"^
 "libdjvu/GMapAreas.cpp"^
 "libdjvu/GPixmap.cpp"^
 "libdjvu/GRect.cpp"^
 "libdjvu/GScaler.cpp"^
 "libdjvu/GSmartPointer.cpp"^
 "libdjvu/GString.cpp"^
 "libdjvu/GThreads.cpp"^
 "libdjvu/GUnicode.cpp"^
 "libdjvu/IFFByteStream.cpp"^
 "libdjvu/IW44EncodeCodec.cpp"^
 "libdjvu/IW44Image.cpp"^
 "libdjvu/JB2EncodeCodec.cpp"^
 "libdjvu/JB2Image.cpp"^
 "libdjvu/JPEGDecoder.cpp"^
 "libdjvu/MMRDecoder.cpp"^
 "libdjvu/MMX.cpp"^
 "libdjvu/UnicodeByteStream.cpp"^
 "libdjvu/XMLParser.cpp"^
 "libdjvu/XMLTags.cpp"^
 "libdjvu/ZPCodec.cpp"^
 "libdjvu/wasm/atomic.cpp"^
 "libdjvu/wasm/DjVuMessage.cpp"^
 "libdjvu/wasm/GOS.cpp"^
 "libdjvu/wasm/GURL.cpp"^
 "djvu_common.cpp"^
 -DWIN32^
 -DNDEBUG^
 -D_LIB^
 -D_CRT_SECURE_NO_WARNINGS^
 -DWASM_MODE^
 -Derrno=0^
 "-DUINT=unsigned int"^
 -DTHREADMODEL=0^
 -DDEBUGLVL=0

echo "finalize..."

call powershell -Command "(Get-Content ./djvu.js) -replace 'function getBinaryPromise\(\){', 'function getBinaryPromise2(){' | Set-Content ./djvu.js"

if not exist "./deploy/djvu" mkdir "./../deploy/djvu"

call powershell -Command "(Get-Content ./djvu_base.js) -replace '//module', (Get-Content ./djvu.js) | Set-Content ./../deploy/djvu/djvu.js"
call echo f | xcopy /b/v/y/f "djvu.wasm" "./../deploy/djvu/djvu.wasm"
