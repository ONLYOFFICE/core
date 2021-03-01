/* file manager class - read lines of files [filename] OR [filename.hz] */
#ifndef _FILEMGR_HXX_
#define _FILEMGR_HXX_

#include "hunvisapi.h"

#include "hunzip.hxx"
#include <stdio.h>

class LIBHUNSPELL_DLL_EXPORTED FileMgr
{
private:
    FileMgr(const FileMgr&);
    FileMgr& operator = (const FileMgr&);
protected:
    FILE * fin;
    Hunzip * hin;
    char in[BUFSIZE + 50]; // input buffer
    int fail(const char * err, const char * par);
    int linenum;

#ifdef HUNSPELL_WASM_MODULE
    char* memory;
    size_t index;
    size_t size;
#endif

public:
    FileMgr(const char * filename, const char * key = NULL);
    ~FileMgr();
    char * getline();
    int getlinenum();
};
#endif
