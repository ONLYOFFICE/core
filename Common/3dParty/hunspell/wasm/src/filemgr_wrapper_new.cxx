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
        memin.from_buffer((char *)file_memory->data, file_memory->len);
    }
}

FileMgr::~FileMgr()
{
    if (hin)
        delete hin;
}

bool FileMgr::getline(std::string &dest)
{
	bool ret = memin.get_line(dest);
	if (ret)
		++linenum;	
    return ret;
}

int FileMgr::getlinenum()
{
    return linenum;
}
