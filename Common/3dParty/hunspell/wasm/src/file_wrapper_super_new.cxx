#include <stdlib.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include "filemgr.hxx"
#include "csutil.hxx"
#include "engine.h"

// TODO change make/
// TODO refactor
// TODO optimise memory
// TODO debug
int FileMgr::fail(const char* err, const char* par) {
  fprintf(stderr, err, par);
  return -1;
}

FileMgr::FileMgr(const char* file, const char* key) : hin(NULL), linenum(0) {
  in[0] = '\0';

    CFileMemory* file_memory = CSpellchecker::Get(file);
    if (!file_memory)
    {
        fail(MSG_OPEN, file);
    }
    else
    {
        std::string workaround_string;
        for (size_t i = 0; i < file_memory->len; i++) {
            workaround_string.push_back(file_memory->data[i]);
        }
        workaround_string.shrink_to_fit();
        workaround = std::istringstream(workaround_string, std::ios_base::in);
    }
}

FileMgr::~FileMgr() {
  if(hin)delete hin;
}

bool FileMgr::getline(std::string& dest) {
  bool ret = false;
  ++linenum;
  char buf[20001];
  if (!workaround.eof()) {
    ret = static_cast<bool>(workaround.getline(buf, 20000));
  }
  if (!ret) {
    --linenum;
  }
  dest = buf;
  return ret;
}

int FileMgr::getlinenum() {
  return linenum;
}