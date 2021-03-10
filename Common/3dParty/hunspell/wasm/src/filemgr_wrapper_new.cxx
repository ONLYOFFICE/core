//#include "license.hunspell"
//#include "license.myspell"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "./filemgr.hxx"
#include "csutil.hxx"

#include "./engine.h"

int FileMgr::fail(const char *err, const char *par)
{
    fprintf(stderr, err, par);
    return -1;
}

FileMgr::FileMgr(const char *file, const char *key)
    : hin(NULL), linenum(0), memin(NULL)
{
    in[0] = '\0';

    CFileMemory *file_memory = CSpellchecker::Get(file);
    if (!file_memory)
    {
        fail(MSG_OPEN, file);
    }
    else
    {
        memin = new std::istrstream((char *)file_memory->data, file_memory->len);
    }
}

FileMgr::~FileMgr()
{
    if (hin)
        delete hin;
    if (memin)
        delete memin;
}

bool FileMgr::getline(std::string &dest)
{
    ++linenum;
    bool ret = static_cast<bool>(std::getline(*memin, dest));
    if (!ret)
    {
        --linenum;
    }
    return ret;
}

int FileMgr::getlinenum()
{
    return linenum;
}
