#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_


class Joystick_T
{
  public:
    void useManualSend(bool useManualSend);

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