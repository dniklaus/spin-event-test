#pragma once

#include <SpinTimer.h>

class SpinEvent : public SpinTimerAction
{
public:
  SpinEvent();
  virtual ~SpinEvent() { }

  void send();
  virtual void received() { }

protected:
  void timeExpired();

private:
  SpinTimer* m_timer;
};
