#pragma once

class Watchdog {
public:
  const int WATCHDOG_TIMEOUT_MS = 500;

  static void feed_watchdog();
private:
  static Watchdog m_instance;

  void do_feed_watchdog();

  bool did_setup { false };
};
