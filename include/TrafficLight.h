#ifndef TrafficLight_h
#define TrafficLight_h

#include <inttypes.h>

/*!
  @brief Implementation of three color traffic light state machine. TrafficLight class contains binding to the HW
*/
struct TrafficLightStateMachine
{
  TrafficLightStateMachine();

/*!
  @brief Updates the state machine, and pins. Non-blocking.
  */
  void update();

/*!
  @brief How often is the state machine updated. Defaults to 800ms.
  */
  void setInterval(uint16_t interval_millis);

/*!
  @brief Changes state to green light
  */
  void green();

/*!
  @brief Changes state to red light
  */
  void red();

/*!
  @brief Changes state to blinking yellow
  */
  void disable();

protected:
/*!
  @brief Writes state to pins
*/
    virtual void updatePins(bool red, bool yellow, bool green) const = 0;
/*!
  @brief Gets board specific timing
  */
    virtual unsigned long getTime() const = 0;

private:
    uint8_t state_;
    uint16_t interval_;

    unsigned long lastchange_;
};

/*!
  @brief Binding of TrafficLightStateMachine to arduino pins
*/
struct TrafficLight : TrafficLightStateMachine
{
  TrafficLight();
/*!
  @brief Attach to red/yellow/green pins
*/
  void attach(int rpin, int ypin, int gpin);

protected:
  virtual void updatePins(bool red, bool yellow, bool green) const override;
  virtual unsigned long getTime() const override;

private:
  uint8_t red_, yellow_, green_;
};

#endif
