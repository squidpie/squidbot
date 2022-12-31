#pragma once

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

class RunActionContextBase {
public:
  RunActionContextBase() {}
  virtual ~RunActionContextBase() {}
};

class RunActionBase {
public:
  RunActionBase() {}
  virtual ~RunActionBase() {}
  virtual void run_action() = 0;
};

template <class R, class Rc> class RunnerBase {
public:
  virtual ~RunnerBase() {}
  virtual void start() = 0;
  virtual void stop() = 0;
};

template <class R, class Rc> class Runner : virtual public RunnerBase<R, Rc> {
public:
  Runner(std::shared_ptr<Rc> context) { run_action = std::make_shared<R>(context); }
  ~Runner() {}

  void start() {
    set_running(true);
    thread_handle = new std::thread(&Runner::thread_loop, this);
  }

  void stop() {
    set_running(false);
    thread_handle->join();
  }

  void inject(std::shared_ptr<R> _run_action) { run_action = _run_action; }

private:
  void thread_loop() {
    while (is_running()) {
      run_action->run_action();
    }
  }

  void set_running(bool __is_running) {
    std::lock_guard<std::mutex> lock(is_running_lock);
    _is_running = __is_running;
  }

  bool is_running() {
    std::lock_guard<std::mutex> lock(is_running_lock);
    return _is_running;
  }

  std::thread *thread_handle;
  std::shared_ptr<R> run_action;
  bool _is_running;
  std::mutex is_running_lock;
};