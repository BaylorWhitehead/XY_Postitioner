#ifndef DIGITAL_TRIGGER
#define DIGITAL_TRIGGER

class DigitalTrigger{
  public:
    void pulse(int time);
    static DigitalTrigger* getInstance();
  private:
    static DigitalTrigger* instance;
    void setup();
    DigitalTrigger();
};

#endif
