#include "pthread_setcanceltype.h"

#include <pthread.h>

static void call_exit(void) {
  pthread_exit(0);
}

int __pthread_do_cancel(struct pthread_internal_t* p) {
  if (p == (struct pthread_internal_t *)pthread_self()) {
      call_exit();
  } else if (p->attr.flags & PTHREAD_ATTR_FLAG_CANCEL_HANDLER) {
      pthread_kill((pthread_t)p, SIGRTMIN);
  } else {
      pthread_kill((pthread_t)p, SIGTERM);
  }
  return 0;
}

void pthread_cancel_handler(int signum) {
    pthread_exit(0);
}

void pthread_init(void) {
    struct sigaction sa;
    struct pthread_internal_t * p = (struct pthread_internal_t *)pthread_self();

    if (p->attr.flags & PTHREAD_ATTR_FLAG_CANCEL_HANDLER) {
        return;
    }

    p->attr.flags |= PTHREAD_ATTR_FLAG_CANCEL_HANDLER | PTHREAD_ATTR_FLAG_CANCEL_ENABLE;
    sa.sa_handler = pthread_cancel_handler;
    sigemptyset(&(sa.sa_mask));
    sa.sa_flags = 0;

    sigaction(SIGRTMIN, &sa, NULL);
}

int pthread_setcanceltype(int type, int *oldtype) {
//  struct pthread_internal_t *p = (struct pthread_internal_t*)pthread_self();
//  int newflags;
//
//  pthread_init();
//
//  switch (type) {
//    default:
//      return EINVAL;
//    case PTHREAD_CANCEL_DEFERRED:
//    case PTHREAD_CANCEL_ASYNCHRONOUS:
//      break;
//  }
//
//  pthread_mutex_lock(&p->cancel_lock);
//  if (oldtype) {
//      *oldtype = p->attr.flags & PTHREAD_ATTR_FLAG_CANCEL_ASYNCRONOUS;
//  }
//
//  if (type == PTHREAD_CANCEL_ASYNCHRONOUS) {
//      p->attr.flags |= PTHREAD_ATTR_FLAG_CANCEL_ASYNCRONOUS;
//  } else {
//      p->attr.flags &= ~PTHREAD_ATTR_FLAG_CANCEL_ASYNCRONOUS;
//  }
//
//  newflags=p->attr.flags;
//  pthread_mutex_unlock (&p->cancel_lock);
//
//  if ((newflags & PTHREAD_ATTR_FLAG_CANCEL_PENDING) && (newflags & PTHREAD_ATTR_FLAG_CANCEL_ENABLE) && (newflags & PTHREAD_ATTR_FLAG_CANCEL_ASYNCRONOUS)) {
//      __pthread_do_cancel(p);
//  }

    return 0;
}