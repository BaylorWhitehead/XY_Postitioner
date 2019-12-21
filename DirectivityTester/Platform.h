#ifndef PLATFORM
#define PLATFORM

class Platform{
  public:
    void moveTo(double xNew, double yNew);
    void homeBoth();
    static Platform* getInstance();

  private:
    static Platform* instance;
    double x;
    double y;
    
    Platform();
    void setup();
    void moveMotor(double distance, char axis);
};

#endif
