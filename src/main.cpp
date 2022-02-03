/*
 * wiring-skeleton.cpp
 *
 *  Created on: 15.03.2017
 *      Author: niklausd
 */

#include <Arduino.h>

// PlatformIO libraries
#include <SerialCommand.h>  // pio lib install 173, lib details see https://github.com/kroimon/Arduino-SerialCommand
#include <SpinTimer.h>      // pio lib install 11599, lib details see https://github.com/dniklaus/spin-timer

#include <DbgTracePort.h>
#include <DbgTraceLevel.h>

// private libraries
#include <ProductDebug.h>
#include <Indicator.h>
#include <Button.h>
#include <DetectorStrategy.h>
#include <ButtonEdgeDetector.h>

// local components (lib folder)
#include <MyBuiltinLedIndicatorAdapter.h>
#include <MyButtonAdapter.h>
#include <ArduinoDigitalInPinSupervisor.h>
#include <SpinEvent.h>

SerialCommand* sCmd = 0;

// indicator implementation for built in LED
Indicator* led  = 0;

class MySpinEvent : public SpinEvent
{
private:
  DbgTrace_Port* m_trPort;
public:
  MySpinEvent()
  : SpinEvent()
  , m_trPort(new DbgTrace_Port("evt", DbgTrace_Level::debug))
  { }

  void received()
  {
    TR_PRINTF(m_trPort, DbgTrace_Level::debug, "MySpinEvent::received()!");
  }
};

MySpinEvent* evt = 0;
DbgTrace_Port* sendEvtTrPort = 0;

void sendEvt()
{
  if (0 != evt)
  {
    if (0 != sendEvtTrPort)
    {
      TR_PRINTF(sendEvtTrPort, DbgTrace_Level::debug, "MySpinEvent::send()!");
    }
    evt->send();
  }
}

void setup()
{
  // setup basic debug environment (heap usage printer, trace ports & dbg cli)
  setupProdDebugEnv();

  // indicator LED
  led = new Indicator("led", "Built in LED.");
  led->assignAdapter(new MyBuiltinLedIndicatorAdapter());

  sendEvtTrPort = new DbgTrace_Port("snd", DbgTrace_Level::debug);
  evt = new MySpinEvent();
  sCmd->addCommand("evt", sendEvt);

#ifdef USER_BTN
  new Button(new ArduinoDigitalInPinSupervisor(USER_BTN), new ButtonEdgeDetector(), new MyButtonAdapter(led));
#endif
}

void loop()
{
  // file deepcode ignore CppSameEvalBinaryExpressionfalse: sCmd gets instantiated by setupProdDebugEnv()
  if (0 != sCmd)
  {
    sCmd->readSerial();     // process serial commands
  }
  scheduleTimers();         // process Timers
}
