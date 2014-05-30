// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/surface/transport_dib.h"

// Desktop GTK Linux builds use the old-style SYSV SHM based DIBs.
// Linux Aura and Chrome OS do too. This will change very soon.
#if defined(TOOLKIT_GTK) || (defined(OS_LINUX) && defined(USE_AURA))

#include <errno.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "skia/ext/platform_canvas.h"
#include "ui/base/x/x11_util.h"
#include "ui/gfx/size.h"

// The shmat system call uses this as it's invalid return address
static void *const kInvalidAddress = (void*) -1;

TransportDIB::TransportDIB()
    : address_(kInvalidAddress),
      x_shm_(0),
      display_(NULL),
      inflight_counter_(0),
      detached_(false),
      size_(0) {
}

TransportDIB::~TransportDIB() {
  if (address_ != kInvalidAddress) {
    shmdt(address_);
    address_ = kInvalidAddress;
  }

  if (x_shm_) {
    DCHECK(display_);
    ui::DetachSharedMemory(display_, x_shm_);
  }
}

// static
TransportDIB* TransportDIB::Create(size_t size, uint32 sequence_num) {
  const int shmkey = shmget(IPC_PRIVATE, size, 0600);
  if (shmkey == -1) {
    DLOG(ERROR) << "Failed to create SysV shared memory region"
                << " errno:" << errno;
    return NULL;
  } else {
    VLOG(1) << "Created SysV shared memory region " << shmkey;
  }

  void* address = shmat(shmkey, NULL /* desired address */, 0 /* flags */);
  // Here we mark the shared memory for deletion. Since we attached it in the
  // line above, it doesn't actually get deleted but, if we crash, this means
  // that the kernel will automatically clean it up for us.
  shmctl(shmkey, IPC_RMID, 0);
  if (address == kInvalidAddress)
    return NULL;

  TransportDIB* dib = new TransportDIB;

  dib->key_.shmkey = shmkey;
  dib->address_ = address;
  dib->size_ = size;
  return dib;
}

// static
TransportDIB* TransportDIB::Map(Handle handle) {
  scoped_ptr<TransportDIB> dib(CreateWithHandle(handle));
  if (!dib->Map())
    return NULL;
  return dib.release();
}

// static
TransportDIB* TransportDIB::CreateWithHandle(Handle shmkey) {
  TransportDIB* dib = new TransportDIB;
  dib->key_.shmkey = shmkey;
  return dib;
}

// static
bool TransportDIB::is_valid_handle(Handle dib) {
  return dib >= 0;
}

// static
bool TransportDIB::is_valid_id(Id id) {
  return id.shmkey != -1;
}

skia::PlatformCanvas* TransportDIB::GetPlatformCanvas(int w, int h) {
  if ((address_ == kInvalidAddress && !Map()) || !VerifyCanvasSize(w, h))
    return NULL;
  return skia::CreatePlatformCanvas(w, h, true,
                                    reinterpret_cast<uint8_t*>(memory()),
                                    skia::RETURN_NULL_ON_FAILURE);
}

bool TransportDIB::Map() {
  if (!is_valid_id(key_))
    return false;
  if (address_ != kInvalidAddress)
    return true;

  struct shmid_ds shmst;
  if (shmctl(key_.shmkey, IPC_STAT, &shmst) == -1)
    return false;

  void* address = shmat(key_.shmkey, NULL /* desired address */, 0 /* flags */);
  if (address == kInvalidAddress)
    return false;

  address_ = address;
  size_ = shmst.shm_segsz;
  return true;
}

void* TransportDIB::memory() const {
  DCHECK_NE(address_, kInvalidAddress);
  return address_;
}

TransportDIB::Id TransportDIB::id() const {
  return key_;
}

TransportDIB::Handle TransportDIB::handle() const {
  return key_.shmkey;
}

XID TransportDIB::MapToX(Display* display) {
  if (!x_shm_) {
    x_shm_ = ui::AttachSharedMemory(display, key_.shmkey);
    display_ = display;
  }

  return x_shm_;
}

void TransportDIB::DecreaseInFlightCounter() {
  CHECK(inflight_counter_);
  inflight_counter_--;
  if (!inflight_counter_ && detached_)
    delete this;
}

void TransportDIB::Detach() {
  CHECK(!detached_);
  detached_ = true;
  if (!inflight_counter_)
    delete this;
}

#endif

