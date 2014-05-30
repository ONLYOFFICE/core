// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gl/safe_shared_memory_pool.h"

#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/process_util.h"
#include "build/build_config.h"

using base::SharedMemory;
using base::SharedMemoryHandle;

namespace gfx {

ScopedSafeSharedMemory::ScopedSafeSharedMemory(SafeSharedMemoryPool* pool,
                                               base::SharedMemory* memory,
                                               size_t shm_size) {
  DCHECK(pool);
  DCHECK(memory);
  DCHECK(memory->memory());
  pool_ = pool;
  original_handle_ = memory->handle();
  safe_shared_memory_ = pool->AcquireSafeSharedMemory(memory, shm_size);
  CHECK(safe_shared_memory_);
}

ScopedSafeSharedMemory::~ScopedSafeSharedMemory() {
  // Release the handle. The pool will delete the SharedMemory
  // object when it is no longer referenced.
  pool_->ReleaseSafeSharedMemory(original_handle_);
}

base::SharedMemory* ScopedSafeSharedMemory::shared_memory() {
  return safe_shared_memory_;
}


SafeSharedMemoryPool::SafeSharedMemoryPool()
    : handles_acquired_(0),
      handles_consumed_(0),
      address_space_consumed_(0),
      max_handles_acquired_(0),
      max_handles_consumed_(0),
      max_address_space_consumed_(0) {
}

SafeSharedMemoryPool::~SafeSharedMemoryPool() {
}

base::SharedMemory* SafeSharedMemoryPool::
    AcquireSafeSharedMemory(base::SharedMemory* shared_memory,
                            size_t shm_size) {
  DCHECK(shared_memory);
  DCHECK(shared_memory->memory());
  base::AutoLock scoped_lock(lock_);

  // Adjust stats.
  handles_acquired_++;
  max_handles_acquired_ = std::max(max_handles_acquired_,
                                   handles_acquired_);

  MemoryMap::iterator it = memory_.find(shared_memory->handle());
  // If we don't already have it, duplicated it.
  if (it == memory_.end()) {
    // Duplicate a new shared memory and track it.
    TrackedMemory tracker;
    tracker.safe_shared_memory = DuplicateSharedMemory(shared_memory, shm_size);
    tracker.reference_count = 1;
    tracker.shm_size = shm_size;
    memory_[shared_memory->handle()] = tracker;

    // Adjust stats.
    handles_consumed_++;
    address_space_consumed_ += shm_size;
    max_handles_consumed_ = std::max(max_handles_consumed_,
                                         handles_consumed_);
    max_address_space_consumed_ = std::max(max_address_space_consumed_,
                                               address_space_consumed_);
    return tracker.safe_shared_memory;
  }

  // Otherwise, add a reference and return the existing one.
  DCHECK(it->second.reference_count);
  DCHECK(it->second.safe_shared_memory);
  DCHECK(it->second.safe_shared_memory->memory());
  it->second.reference_count++;
  return it->second.safe_shared_memory;
}

void SafeSharedMemoryPool::
    ReleaseSafeSharedMemory(const base::SharedMemoryHandle& handle) {
  base::AutoLock scoped_lock(lock_);

  // Adjust stats.
  handles_acquired_--;
  DCHECK(handles_acquired_ >= 0);

  MemoryMap::iterator it = memory_.find(handle);
  CHECK(it != memory_.end());
  CHECK(it->second.reference_count);
  CHECK(it->second.safe_shared_memory);
  if (--it->second.reference_count == 0) {
    // Adjust stats.
    handles_consumed_--;
    address_space_consumed_ -= it->second.shm_size;
    DCHECK(handles_consumed_ >= 0);
    DCHECK(address_space_consumed_ >= 0);
    // Delete the safe memory and remove it.
    delete it->second.safe_shared_memory;
    memory_.erase(it);
  }
}

SharedMemory* SafeSharedMemoryPool::DuplicateSharedMemory(
    SharedMemory* shared_memory, size_t size) {
  // Duplicate the handle.
  SharedMemoryHandle duped_shared_memory_handle;
  if (!shared_memory->ShareToProcess(
      base::GetCurrentProcessHandle(),
      &duped_shared_memory_handle)) {
    LOG(ERROR) << "Failed SharedMemory::ShareToProcess";
    LOG(ERROR) << "Total handles acquired " << handles_acquired_;
    LOG(ERROR) << "Total handles open " << handles_consumed_;
    LOG(ERROR) << "Total address space " << address_space_consumed_;
    LOG(ERROR) << "Max handles acquired " << max_handles_acquired_;
    LOG(ERROR) << "Max handles open " << max_handles_consumed_;
    LOG(ERROR) << "Max address space " << max_address_space_consumed_;
    CHECK(false); // Diagnosing a crash.
    return NULL;
  }
  scoped_ptr<SharedMemory> duped_shared_memory(
      new SharedMemory(duped_shared_memory_handle, false));
  // Map the shared memory into this process. This validates the size.
  if (!duped_shared_memory->Map(size)) {
    LOG(ERROR) << "Failed SharedMemory::Map";
    LOG(ERROR) << "Total handles acquired " << handles_acquired_;
    LOG(ERROR) << "Total handles open " << handles_consumed_;
    LOG(ERROR) << "Total address space " << address_space_consumed_;
    LOG(ERROR) << "Max handles acquired " << max_handles_acquired_;
    LOG(ERROR) << "Max handles open " << max_handles_consumed_;
    LOG(ERROR) << "Max address space " << max_address_space_consumed_;
    CHECK(false); // Diagnosing a crash.
    return NULL;
  }
  return duped_shared_memory.release();
}

}  // namespace gfx
