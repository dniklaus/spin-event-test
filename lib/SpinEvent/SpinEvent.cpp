#include "SpinEvent.h"
#include <SpinTimer.h>

SpinEvent::SpinEvent()
: SpinTimerAction()
, m_timer(new SpinTimer(0, this))
{ }

void SpinEvent::send()
{
  m_timer->start();
}

void SpinEvent::timeExpired()
{
  this->received();
}
