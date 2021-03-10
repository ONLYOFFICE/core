#include "license.hunspell"
#include "license.myspell"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "filemgr.hxx"
#include "csutil.hxx"

#include "./engine.h"

int FileMgr::fail(const char * err, const char * par) {
    fprintf(stderr, err, par);
    return -1;
}

FileMgr::FileMgr(const char * file, const char * key)
    : hin(NULL), fin(NULL), linenum(0)
{
    in[0] = '\0';

    CFileMemory* file_memory = CSpellchecker::Get(file);
    if (!file_memory)
    {
        fail(MSG_OPEN, file);
    }
    else
    {
        index = 0;
        size = file_memory->len;
        memory = (char*)file_memory->data;
    }
}

FileMgr::~FileMgr()
{
    if (fin) fclose(fin);
    if (hin) delete hin;
}

char * FileMgr::getline() {
    if(index >= size)
        return NULL;
    int i = 0,
        l = BUFSIZE -1;
    for(;index < size && memory[index] != '\n' && memory[index] != EOF && i <= l; index++, i++)
        in[i] = memory[index];
    in[i] = '\0';
    index++;
    if(index < size)
        linenum++;
    return strdup(in);
}

int FileMgr::getlinenum() {
    return linenum;
}
