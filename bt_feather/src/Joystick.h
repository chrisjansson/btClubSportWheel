#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_


class Joystick_T
{
  public:
    void useManualSend(bool useManualSend);

    int recv(void *buffer, uint16_t timeout);

    void send_now();

    void button(uint8_t button, bool val);

    void X(unsigned int val);

    void Y(unsigned int val);

    void clutch1(unsigned int val);

    void clutch2(unsigned int val);

    void hat(int val);

    
    void setWheel(unsigned int val);


    // void interval(uint16_t interval_millis);
    // uint8_t get(uint8_t value);

  protected:
    // unsigned long previous_millis;
    // uint16_t interval_millis;
    // uint8_t value;

  private:
    // inline void setValue(uint8_t value) {this->value = value;}
};

extern Joystick_T Joystick;

#endif