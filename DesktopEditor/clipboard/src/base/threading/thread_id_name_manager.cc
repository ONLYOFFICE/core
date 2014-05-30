// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/threading/thread_id_name_manager.h"

#include <stdlib.h>
#include <string.h>

#include "base/logging.h"
#include "base/memory/singleton.h"
#include "base/string_util.h"

namespace base {
namespace {

static const char kDefaultName[] = "";
static std::string* g_default_name;

typedef std::map<PlatformThreadId, std::string*>::iterator
    ThreadIdToInternedNameIterator;
typedef std::map<std::string, std::string*>::iterator
    NameToInternedNameIterator;
}

ThreadIdNameManager::ThreadIdNameManager() {
  g_default_name = new std::string(kDefaultName);

  AutoLock locked(lock_);
  name_to_interned_name_[kDefaultName] = g_default_name;
}

ThreadIdNameManager::~ThreadIdNameManager() {
}

ThreadIdNameManager* ThreadIdNameManager::GetInstance() {
  return Singleton<ThreadIdNameManager,
      LeakySingletonTraits<ThreadIdNameManager> >::get();
}

const char* ThreadIdNameManager::GetDefaultInternedString() {
  return g_default_name->c_str();
}

void ThreadIdNameManager::SetName(PlatformThreadId id, const char* name) {
  std::string str_name(name);

  AutoLock locked(lock_);
  NameToInternedNameIterator iter = name_to_interned_name_.find(str_name);
  std::string* leaked_str = NULL;
  if (iter != name_to_interned_name_.end()) {
    leaked_str = iter->second;
  } else {
    leaked_str = new std::string(str_name);
    name_to_interned_name_[str_name] = leaked_str;
  }
  thread_id_to_interned_name_[id] = leaked_str;
}

const char* ThreadIdNameManager::GetName(PlatformThreadId id) {
  AutoLock locked(lock_);
  ThreadIdToInternedNameIterator iter = thread_id_to_interned_name_.find(id);
  // A platform thread may not have a name set, so return blank.
  if (iter == thread_id_to_interned_name_.end())
    return name_to_interned_name_[kDefaultName]->c_str();
  return iter->second->c_str();
}

void ThreadIdNameManager::RemoveName(PlatformThreadId id) {
  if (id == kInvalidThreadId)
    return;

  AutoLock locked(lock_);
  ThreadIdToInternedNameIterator iter = thread_id_to_interned_name_.find(id);
  DCHECK((iter != thread_id_to_interned_name_.end()));
  thread_id_to_interned_name_.erase(iter);
}

}  // namespace base
