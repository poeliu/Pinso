// File: core/sync.h - Define synchronizations.

#ifndef CORE_SYNC_H_
#define CORE_SYNC_H_

#include <semaphore.h>

#include "core/basictypes.h"

// Define mutex interface.
class Mutex {
 public:
  Mutex() {}
  virtual ~Mutex() {}

  virtual void Lock() {}
  virtual void Unlock() {}
  virtual Mutex *Clone() { return new Mutex; }

 private:
  DISALLOW_COPY_CONSTRUCTORS(Mutex);
};

// Define scoped lock.
class ScopedLock {
 public:
  explicit ScopedLock(Mutex *mutex)
      : mutex_(mutex), locked_(false) {
    mutex_->Lock();
    locked_ = true;
  }

  ScopedLock(Mutex *mutex, bool initially_locked)
      : mutex_(mutex), locked_(false) {
    if (initially_locked) {
      mutex_->Lock();
      locked_ = true;
    }
  }

  ~ScopedLock() {
    if (locked_)
      mutex_->Unlock();
  }

 private:
  Mutex *mutex_;
  bool locked_;

  DISALLOW_COPY_CONSTRUCTORS(ScopedLock);
};

// Define read-write mutex interface.
class RWMutex {
 public:
  RWMutex() {}
  virtual ~RWMutex() {}

  virtual void LockRead() {}
  virtual void UnlockRead() {}
  virtual void LockWrite() {}
  virtual void UnlockWrite() {}
  virtual RWMutex *Clone() { return new RWMutex; }

 private:
  DISALLOW_COPY_CONSTRUCTORS(RWMutex);
};

// Define semaphore.
class Semaphore {
 public:
  explicit Semaphore(unsigned int value) {
    int ret = sem_init(&sem_, 0, value);
    assert(!ret);
  }

  ~Semaphore() {
    int ret = sem_destroy(&sem_);
    assert(!ret);
  }

  void Wait() {
    int ret = sem_wait(&sem_);
    assert(!ret);
  }

  void Post() {
    int ret = sem_post(&sem_);
    assert(!ret);
  }

 private:
  sem_t sem_;

  DISALLOW_COPY_CONSTRUCTORS(Semaphore);
};

// Define semaphore based mutex.
class SemMutex : public Mutex {
 public:
  SemMutex() { sem_ = new Semaphore(1); }
  ~SemMutex() { delete sem_; }

  void Lock() { sem_->Wait(); }
  void Unlock() { sem_->Post(); }
  Mutex *Clone() { return new SemMutex; }

 private:
  Semaphore *sem_;

  DISALLOW_COPY_CONSTRUCTORS(SemMutex);
};

// Define semaphore based read-write mutex.
class SemRWMutex : public RWMutex {
 public:
  SemRWMutex(int num_readers, Mutex *mutex)
      : num_readers_(num_readers), mutex_(mutex) {
    sem_ = new Semaphore(num_readers_);
  }

  ~SemRWMutex() {
    delete mutex_;
    delete sem_;
  }

  void LockRead() { sem_->Wait(); }
  void UnlockRead() { sem_->Post(); }

  void LockWrite() {
    mutex_->Lock();
    for (int i = 0; i < num_readers_; i++)
      sem_->Wait();
    mutex_->Unlock();
  }

  void UnlockWrite() {
    for (int i = 0; i < num_readers_; i++)
      sem_->Post();
  }

  RWMutex *Clone() {
    return new SemRWMutex(num_readers_, mutex_->Clone());
  }

 private:
  int num_readers_;
  Mutex *mutex_;
  Semaphore *sem_;

  DISALLOW_COPY_CONSTRUCTORS(SemRWMutex);
};

#endif

