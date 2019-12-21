#ifndef PLATFORM
#define PLATFORM


class Platform{
  public:
    void setup();
    void moveTo(double xNew, double yNew);
    void homeBoth();
    static Platform* getInstance();
    static Platform* instance;
  private:
    
    double x;
    double y;
    Platform();
  
};




#endif
