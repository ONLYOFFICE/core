// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_THREADING_THREAD_ID_NAME_MANAGER_H_
#define BASE_THREADING_THREAD_ID_NAME_MANAGER_H_

#include <map>
#include <string>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/synchronization/lock.h"
#include "base/threading/platform_thread.h"

template <typename T> struct DefaultSingletonTraits;

namespace base {

class BASE_EXPORT ThreadIdNameManager {
 public:
  static ThreadIdNameManager* GetInstance();

  static const char* GetDefaultInternedString();

  // Set the name for the given id.
  void SetName(PlatformThreadId id, const char* name);

  // Get the name for the given id.
  const char* GetName(PlatformThreadId id);

  // Remove the name for the given id.
  void RemoveName(PlatformThreadId id);

 private:
  friend struct DefaultSingletonTraits<ThreadIdNameManager>;

  ThreadIdNameManager();
  ~ThreadIdNameManager();

  // lock_ protects both the thread_id_to_name_ and name_to_interned_name_ maps.
  Lock lock_;

  std::map<PlatformThreadId, std::string*> thread_id_to_interned_name_;
  std::map<std::string, std::string*> name_to_interned_name_;

  DISALLOW_COPY_AND_ASSIGN(ThreadIdNameManager);
};

}  // namespace base

#endif  // BASE_THREADING_THREAD_ID_NAME_MANAGER_H_
