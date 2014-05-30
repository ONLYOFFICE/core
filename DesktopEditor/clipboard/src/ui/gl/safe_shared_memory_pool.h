// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_SAFE_SHARED_MEMORY_POOL_H_
#define UI_GL_SAFE_SHARED_MEMORY_POOL_H_

#include <map>

#include "base/basictypes.h"
#include "base/memory/shared_memory.h"
#include "base/synchronization/lock.h"
#include "build/build_config.h"

namespace base {
class SharedMemory;
}

namespace gfx {
class SafeSharedMemoryPool;
class ScopedSafeSharedMemory;
}

namespace gfx {

// These classes exist to help protect against deletion of shared
// memory that is being used on a worker thread. It's mainly a
// security measure to prevent use-after-free in the browser, due
// to a misbehaving client. That said, this should be removed
// in favor of higher-level reference counting of an appropriate
// opaque 'memory blob' data-structure.

class ScopedSafeSharedMemory {
 public:
  base::SharedMemory* shared_memory();
  ScopedSafeSharedMemory(SafeSharedMemoryPool* pool,
                         base::SharedMemory* memory,
                         size_t shm_size);
  ~ScopedSafeSharedMemory();
 private:
  base::SharedMemory* safe_shared_memory_;
  base::SharedMemoryHandle original_handle_;
  SafeSharedMemoryPool* pool_;

  DISALLOW_COPY_AND_ASSIGN(ScopedSafeSharedMemory);
};

class SafeSharedMemoryPool {
 public:
  SafeSharedMemoryPool();
  virtual ~SafeSharedMemoryPool();

 private:
  friend class ScopedSafeSharedMemory;

  // Acquires and release shared memory. The acquired shared memory
  // is guaranteed to live until it is released.
  base::SharedMemory* AcquireSafeSharedMemory(base::SharedMemory*, size_t size);
  void ReleaseSafeSharedMemory(const base::SharedMemoryHandle&);

  // Utility function to duplicate shared memory.
  base::SharedMemory* DuplicateSharedMemory(base::SharedMemory*, size_t size);

  // Track all SharedMemory's that we have already duplicated.
  struct TrackedMemory {
    base::SharedMemory* safe_shared_memory;
    size_t shm_size;
    int reference_count;
  };

  typedef std::map<base::SharedMemoryHandle, TrackedMemory> MemoryMap;
  MemoryMap memory_;

  // Track usage to diagnose crashes.
  int handles_acquired_;
  int handles_consumed_;
  size_t address_space_consumed_;
  int max_handles_acquired_;
  int max_handles_consumed_;
  size_t max_address_space_consumed_;

  base::Lock lock_;

  DISALLOW_COPY_AND_ASSIGN(SafeSharedMemoryPool);
};

}  // namespace gfx

#endif  // UI_GL_ASYNC_TASK_DELEGATE_H_

