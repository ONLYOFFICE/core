//#include "license.hunspell"
//#include "license.myspell"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "./filemgr.hxx"
 #include "csutil.hxx"

 #include "./engine.h"

int FileMgr::fail(const char * err, const char * par) {
    fprintf(stderr, err, par);
    return -1;
}

FileMgr::FileMgr(const char * file, const char * key)
    : hin(NULL), linenum(0)
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
    if (hin) delete hin;
}

bool FileMgr::getline(std::string& res) {
    bool ret = false;
    if(index >= size)
        return false;
    int i = 0, l = BUFSIZE-1;
    size_t cur_index = index;
    while (memory[index] != '\n' && memory[index] != EOF && i <= l)
    {index++;i++;}
    res = std::string(memory + cur_index, index - cur_index);
    index++;    
    if(index < size)
    {
        linenum++;
        return true;
    }
    return false;
}

int FileMgr::getlinenum() {
    return linenum;
}
