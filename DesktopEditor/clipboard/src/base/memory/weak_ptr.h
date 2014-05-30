// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Weak pointers are pointers to an object that do not affect its lifetime,
// and which may be invalidated (i.e. reset to NULL) by the object, or its
// owner, at any time, most commonly when the object is about to be deleted.

// Weak pointers are useful when an object needs to be accessed safely by one
// or more objects other than its owner, and those callers can cope with the
// object vanishing and e.g. tasks posted to it being silently dropped.
// Reference-counting such an object would complicate the ownership graph and
// make it harder to reason about the object's lifetime.

// EXAMPLE:
//
//  class Controller {
//   public:
//    void SpawnWorker() { Worker::StartNew(weak_factory_.GetWeakPtr()); }
//    void WorkComplete(const Result& result) { ... }
//   private:
//    WeakPtrFactory<Controller> weak_factory_;
//  };
//
//  class Worker {
//   public:
//    static void StartNew(const WeakPtr<Controller>& controller) {
//      Worker* worker = new Worker(controller);
//      // Kick off asynchronous processing...
//    }
//   private:
//    Worker(const WeakPtr<Controller>& controller)
//        : controller_(controller) {}
//    void DidCompleteAsynchronousProcessing(const Result& result) {
//      if (controller_)
//        controller_->WorkComplete(result);
//    }
//    WeakPtr<Controller> controller_;
//  };
//
// With this implementation a caller may use SpawnWorker() to dispatch multiple
// Workers and subsequently delete the Controller, without waiting for all
// Workers to have completed.

// ------------------------- IMPORTANT: Thread-safety -------------------------

// Weak pointers must always be dereferenced and invalidated on the same thread
// otherwise checking the pointer would be racey.  WeakPtrFactory enforces this
// by binding itself to the current thread when a WeakPtr is first created
// and un-binding only when those pointers are invalidated.  WeakPtrs may still
// be handed off to other threads, however, so long as they are only actually
// dereferenced on the originating thread. This includes posting tasks to the
// thread using base::Bind() to invoke a method on the object via the WeakPtr.

// Calling SupportsWeakPtr::DetachFromThread() can work around the limitations
// above and cancel the thread binding of the object and all WeakPtrs pointing
// to it, but it's not recommended and unsafe.  See crbug.com/232143.

#ifndef BASE_MEMORY_WEAK_PTR_H_
#define BASE_MEMORY_WEAK_PTR_H_

#include "base/basictypes.h"
#include "base/base_export.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/template_util.h"
#include "base/threading/thread_checker.h"

namespace base {

template <typename T> class SupportsWeakPtr;
template <typename T> class WeakPtr;

namespace internal {
// These classes are part of the WeakPtr implementation.
// DO NOT USE THESE CLASSES DIRECTLY YOURSELF.

class BASE_EXPORT WeakReference {
 public:
  // While Flag is bound to a specific thread, it may be deleted from another
  // via base::WeakPtr::~WeakPtr().
  class Flag : public RefCountedThreadSafe<Flag> {
   public:
    Flag();

    void Invalidate();
    bool IsValid() const;

    void DetachFromThread() { thread_checker_.DetachFromThread(); }

   private:
    friend class base::RefCountedThreadSafe<Flag>;

    ~Flag();

    ThreadChecker thread_checker_;
    bool is_valid_;
  };

  WeakReference();
  explicit WeakReference(const Flag* flag);
  ~WeakReference();

  bool is_valid() const;

 private:
  scoped_refptr<const Flag> flag_;
};

class BASE_EXPORT WeakReferenceOwner {
 public:
  WeakReferenceOwner();
  ~WeakReferenceOwner();

  WeakReference GetRef() const;

  bool HasRefs() const {
    return flag_.get() && !flag_->HasOneRef();
  }

  void Invalidate();

  // Indicates that this object will be used on another thread from now on.
  // Do not use this in new code. See crbug.com/232143.
  void DetachFromThread() {
    if (flag_) flag_->DetachFromThread();
  }

 private:
  mutable scoped_refptr<WeakReference::Flag> flag_;
};

// This class simplifies the implementation of WeakPtr's type conversion
// constructor by avoiding the need for a public accessor for ref_.  A
// WeakPtr<T> cannot access the private members of WeakPtr<U>, so this
// base class gives us a way to access ref_ in a protected fashion.
class BASE_EXPORT WeakPtrBase {
 public:
  WeakPtrBase();
  ~WeakPtrBase();

 protected:
  explicit WeakPtrBase(const WeakReference& ref);

  WeakReference ref_;
};

// This class provides a common implementation of common functions that would
// otherwise get instantiated separately for each distinct instantiation of
// SupportsWeakPtr<>.
class SupportsWeakPtrBase {
 public:
  // A safe static downcast of a WeakPtr<Base> to WeakPtr<Derived>. This
  // conversion will only compile if there is exists a Base which inherits
  // from SupportsWeakPtr<Base>. See base::AsWeakPtr() below for a helper
  // function that makes calling this easier.
  template<typename Derived>
  static WeakPtr<Derived> StaticAsWeakPtr(Derived* t) {
    typedef
        is_convertible<Derived, internal::SupportsWeakPtrBase&> convertible;
    COMPILE_ASSERT(convertible::value,
                   AsWeakPtr_argument_inherits_from_SupportsWeakPtr);
    return AsWeakPtrImpl<Derived>(t, *t);
  }

 private:
  // This template function uses type inference to find a Base of Derived
  // which is an instance of SupportsWeakPtr<Base>. We can then safely
  // static_cast the Base* to a Derived*.
  template <typename Derived, typename Base>
  static WeakPtr<Derived> AsWeakPtrImpl(
      Derived* t, const SupportsWeakPtr<Base>&) {
    WeakPtr<Base> ptr = t->Base::AsWeakPtr();
    return WeakPtr<Derived>(ptr.ref_, static_cast<Derived*>(ptr.ptr_));
  }
};

}  // namespace internal

template <typename T> class WeakPtrFactory;

// The WeakPtr class holds a weak reference to |T*|.
//
// This class is designed to be used like a normal pointer.  You should always
// null-test an object of this class before using it or invoking a method that
// may result in the underlying object being destroyed.
//
// EXAMPLE:
//
//   class Foo { ... };
//   WeakPtr<Foo> foo;
//   if (foo)
//     foo->method();
//
template <typename T>
class WeakPtr : public internal::WeakPtrBase {
 public:
  WeakPtr() : ptr_(NULL) {
  }

  // Allow conversion from U to T provided U "is a" T. Note that this
  // is separate from the (implicit) copy constructor.
  template <typename U>
  WeakPtr(const WeakPtr<U>& other) : WeakPtrBase(other), ptr_(other.ptr_) {
  }

  T* get() const { return ref_.is_valid() ? ptr_ : NULL; }
  operator T*() const { return get(); }

  T& operator*() const {
    DCHECK(get() != NULL);
    return *get();
  }
  T* operator->() const {
    DCHECK(get() != NULL);
    return get();
  }

  void reset() {
    ref_ = internal::WeakReference();
    ptr_ = NULL;
  }

 private:
  friend class internal::SupportsWeakPtrBase;
  template <typename U> friend class WeakPtr;
  friend class SupportsWeakPtr<T>;
  friend class WeakPtrFactory<T>;

  WeakPtr(const internal::WeakReference& ref, T* ptr)
      : WeakPtrBase(ref),
        ptr_(ptr) {
  }

  // This pointer is only valid when ref_.is_valid() is true.  Otherwise, its
  // value is undefined (as opposed to NULL).
  T* ptr_;
};

// A class may be composed of a WeakPtrFactory and thereby
// control how it exposes weak pointers to itself.  This is helpful if you only
// need weak pointers within the implementation of a class.  This class is also
// useful when working with primitive types.  For example, you could have a
// WeakPtrFactory<bool> that is used to pass around a weak reference to a bool.
template <class T>
class WeakPtrFactory {
 public:
  explicit WeakPtrFactory(T* ptr) : ptr_(ptr) {
  }

  ~WeakPtrFactory() {
    ptr_ = NULL;
  }

  WeakPtr<T> GetWeakPtr() {
    DCHECK(ptr_);
    return WeakPtr<T>(weak_reference_owner_.GetRef(), ptr_);
  }

  // Call this method to invalidate all existing weak pointers.
  void InvalidateWeakPtrs() {
    DCHECK(ptr_);
    weak_reference_owner_.Invalidate();
  }

  // Call this method to determine if any weak pointers exist.
  bool HasWeakPtrs() const {
    DCHECK(ptr_);
    return weak_reference_owner_.HasRefs();
  }

  // Indicates that this object will be used on another thread from now on.
  // Do not use this in new code. See crbug.com/232143.
  void DetachFromThread() {
    DCHECK(ptr_);
    weak_reference_owner_.DetachFromThread();
  }

 private:
  internal::WeakReferenceOwner weak_reference_owner_;
  T* ptr_;
  DISALLOW_IMPLICIT_CONSTRUCTORS(WeakPtrFactory);
};

// A class may extend from SupportsWeakPtr to let others take weak pointers to
// it. This avoids the class itself implementing boilerplate to dispense weak
// pointers.  However, since SupportsWeakPtr's destructor won't invalidate
// weak pointers to the class until after the derived class' members have been
// destroyed, its use can lead to subtle use-after-destroy issues.
template <class T>
class SupportsWeakPtr : public internal::SupportsWeakPtrBase {
 public:
  SupportsWeakPtr() {}

  WeakPtr<T> AsWeakPtr() {
    return WeakPtr<T>(weak_reference_owner_.GetRef(), static_cast<T*>(this));
  }

  // Indicates that this object will be used on another thread from now on.
  // Do not use this in new code. See crbug.com/232143.
  void DetachFromThread() {
    weak_reference_owner_.DetachFromThread();
  }

 protected:
  ~SupportsWeakPtr() {}

 private:
  internal::WeakReferenceOwner weak_reference_owner_;
  DISALLOW_COPY_AND_ASSIGN(SupportsWeakPtr);
};

// Helper function that uses type deduction to safely return a WeakPtr<Derived>
// when Derived doesn't directly extend SupportsWeakPtr<Derived>, instead it
// extends a Base that extends SupportsWeakPtr<Base>.
//
// EXAMPLE:
//   class Base : public base::SupportsWeakPtr<Producer> {};
//   class Derived : public Base {};
//
//   Derived derived;
//   base::WeakPtr<Derived> ptr = base::AsWeakPtr(&derived);
//
// Note that the following doesn't work (invalid type conversion) since
// Derived::AsWeakPtr() is WeakPtr<Base> SupportsWeakPtr<Base>::AsWeakPtr(),
// and there's no way to safely cast WeakPtr<Base> to WeakPtr<Derived> at
// the caller.
//
//   base::WeakPtr<Derived> ptr = derived.AsWeakPtr();  // Fails.

template <typename Derived>
WeakPtr<Derived> AsWeakPtr(Derived* t) {
  return internal::SupportsWeakPtrBase::StaticAsWeakPtr<Derived>(t);
}

}  // namespace base

#endif  // BASE_MEMORY_WEAK_PTR_H_
