#pragma once

#include <Automaton.h>

class Atm_bit : public Machine {
 public:
  Atm_bit( void ) : Machine(){};

  state_t _last_state;
  atm_connector _connector[4];
  int8_t _indicator;
  bool _indicatorActiveLow;

  enum { OFF, ON };                                       // STATES
  enum { EVT_ON, EVT_OFF, EVT_TOGGLE, EVT_INPUT, ELSE };  // EVENTS
  enum { ACT_ON, ACT_OFF, ACT_INPUT };                    // ACTIONS

  Atm_bit& begin( bool initialState = false );
  Atm_bit& onChange( atm_cb_push_t callback, int idx = 0 );
  Atm_bit& onChange( Machine& machine, int event = 0 );
  Atm_bit& onChange( bool status, atm_cb_push_t callback, int idx = 0 );
  Atm_bit& onChange( bool status, Machine& machine, int event = 0 );
  Atm_bit& onInput( bool status, atm_cb_push_t callback, int idx = 0 );
  Atm_bit& onInput( bool status, Machine& machine, int event = 0 );
  Atm_bit& led( int led, bool activeLow = false );
  Atm_bit& trace( Stream& stream );
  int event( int id );
  void action( int id );
};