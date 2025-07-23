#ifndef LED_H
#define LED_H

class LED {
public:
  LED(int pin);
  void on();
  void off();
private:
  int _pin;
};

#endif
