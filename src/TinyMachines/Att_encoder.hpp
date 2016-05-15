#pragma once

#include <Automaton.h>

#define TINYMACHINE
#undef FACTORY
#undef STATE_TYPE
#undef MACHINE

#ifdef TINYMACHINE
#define MACHINE TinyMachine
#define FACTORY 0
#define STATE_TYPE tiny_state_t
#else
#define MACHINE Machine
#define FACTORY factory
#define STATE_TYPE state_t
#endif

class Att_encoder: public MACHINE {

  public:
    Att_encoder( void ) : MACHINE() {
#ifndef TINYMACHINE        
      class_label = "ENC"; 
#endif
    };
    short _pin1, _pin2;     
    const static char _enc_states[];
    uint8_t _enc_bits;
    uint8_t _enc_counter; 
    int8_t _enc_direction;
    int _divider;
    int _value, _min, _max;
    bool _wrap;
    atm_connector _onup;
    atm_connector _ondown;

    enum { IDLE, UP, DOWN }; // STATES
    enum { EVT_UP, EVT_DOWN, ELSE }; // EVENTS
    enum { ACT_SAMPLE, ACT_UP, ACT_DOWN }; // ACTIONS
	
    Att_encoder & begin( int pin1, int pin2, int divider = 1 );
    Att_encoder & trace( Stream & stream );
    Att_encoder& onUp( Machine& machine, int event = 0 );
    Att_encoder& onUp( TinyMachine& machine, int event = 0 );
    Att_encoder& onUp( atm_cb_t callback, int idx = 0 );
    Att_encoder& onUp( const char * label, int event = 0 );
    Att_encoder& onDown( Machine& machine, int event = 0 );
    Att_encoder& onDown( TinyMachine& machine, int event = 0 );
    Att_encoder& onDown( atm_cb_t callback, int idx = 0 );
    Att_encoder& onDown( const char * label, int event = 0 );
    int state( void );
    Att_encoder& range( int min, int max, bool wrap = false );
    Att_encoder& set( int value );
    

  private:
    bool count( int direction );
    int event( int id ); 
    void action( int id ); 
};

