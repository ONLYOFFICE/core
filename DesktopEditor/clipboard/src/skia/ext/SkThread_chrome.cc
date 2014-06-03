// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/skia/include/core/SkThread.h"

#include <new>

#include "base/atomicops.h"
#include "base/basictypes.h"
#include "base/logging.h"
#include "base/synchronization/lock.h"

/** Adds one to the int specified by the address (in a thread-safe manner), and
    returns the previous value.
    No additional memory barrier is required.
    This must act as a compiler barrier.
*/
int32_t sk_atomic_inc(int32_t* addr) {
  // sk_atomic_inc is expected to return the old value,
  // Barrier_AtomicIncrement returns the new value.
  return base::subtle::NoBarrier_AtomicIncrement(addr, 1) - 1;
}

/*  Subtracts one from the int specified by the address (in a thread-safe
    manner), and returns the previous value.
    Expected to act as a release (SL/S) memory barrier and a compiler barrier.
*/
int32_t sk_atomic_dec(int32_t* addr) {
  // sk_atomic_dec is expected to return the old value,
  // Barrier_AtomicIncrement returns the new value.
  return base::subtle::Barrier_AtomicIncrement(addr, -1) + 1;
}
/** If sk_atomic_dec does not act as an aquire (L/SL) barrier, this is expected
    to act as an aquire (L/SL) memory barrier and as a compiler barrier.
*/
void sk_membar_aquire__after_atomic_dec() { }

/** Adds one to the int specified by the address iff the int specified by the
    address is not zero (in a thread-safe manner), and returns the previous
    value.
    No additional memory barrier is required.
    This must act as a compiler barrier.
*/
int32_t sk_atomic_conditional_inc(int32_t* addr) {
    int32_t value = *addr;

    while (true) {
        if (value == 0) {
            return 0;
        }

        int32_t before;
        before = base::subtle::Acquire_CompareAndSwap(addr, value, value + 1);

        if (before == value) {
            return value;
        } else {
            value = before;
        }
    }
}
/** If sk_atomic_conditional_inc does not act as an aquire (L/SL) barrier, this
    is expected to act as an aquire (L/SL) memory barrier and as a compiler
    barrier.
*/
void sk_membar_aquire__after_atomic_conditional_inc() { }

SkMutex::SkMutex() {
  COMPILE_ASSERT(sizeof(base::Lock) <= sizeof(fStorage), Lock_is_too_big_for_SkMutex);
  base::Lock* lock = reinterpret_cast<base::Lock*>(fStorage);
  new(lock) base::Lock();
}

SkMutex::~SkMutex() {
  base::Lock* lock = reinterpret_cast<base::Lock*>(fStorage);
  lock->~Lock();
}

void SkMutex::acquire() {
  base::Lock* lock = reinterpret_cast<base::Lock*>(fStorage);
  lock->Acquire();
}

void SkMutex::release() {
  base::Lock* lock = reinterpret_cast<base::Lock*>(fStorage);
  lock->Release();
}
