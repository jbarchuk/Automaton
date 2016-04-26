#include "Atm_step.h"

Atm_step & Atm_step::begin( void )
{
  const static state_t state_table[] PROGMEM = {
    /*            ON_ENTER    ON_LOOP  ON_EXIT  EVT_STEP  EVT_SWEEP  EVT_BURST  EVT_LINEAR   ELSE */
    /* IDLE    */       -1,        -1,      -1,   LINEAR,     SWEEP,     BURST,     LINEAR,    -1,
    /* LINEAR  */       -1,        -1,      -1,       S0,     SWEEP,     BURST,     LINEAR,    -1,
    /* S0      */   ACT_S0,        -1,      -1,       S1,     SWEEP,     BURST,     LINEAR,    -1, // Linear mode
    /* S1      */   ACT_S1,        -1,      -1,       S2,     SWEEP,     BURST,     LINEAR,    -1,
    /* S2      */   ACT_S2,        -1,      -1,       S3,     SWEEP,     BURST,     LINEAR,    -1,
    /* S3      */   ACT_S3,        -1,      -1,       S4,     SWEEP,     BURST,     LINEAR,    -1,
    /* S4      */   ACT_S4,        -1,      -1,       S5,     SWEEP,     BURST,     LINEAR,    -1,
    /* S5      */   ACT_S5,        -1,      -1,       S6,     SWEEP,     BURST,     LINEAR,    -1,
    /* S6      */   ACT_S6,        -1,      -1,       S7,     SWEEP,     BURST,     LINEAR,    -1,
    /* S7      */   ACT_S7,        -1,      -1,       S0,     SWEEP,     BURST,     LINEAR,    -1,
    /* SWEEP   */       -1,        -1,      -1,       X0,     SWEEP,     BURST,     LINEAR,    -1,
    /* X0      */   ACT_S0,        -1,      -1,       X1,     SWEEP,     BURST,     LINEAR,    -1, // Sweep mode
    /* X1      */   ACT_S1,        -1,      -1,       X2,     SWEEP,     BURST,     LINEAR,    -1,
    /* X2      */   ACT_S2,        -1,      -1,       X3,     SWEEP,     BURST,     LINEAR,    -1,
    /* X3      */   ACT_S3,        -1,      -1,       X4,     SWEEP,     BURST,     LINEAR,    -1,
    /* X4      */   ACT_S4,        -1,      -1,       X5,     SWEEP,     BURST,     LINEAR,    -1,
    /* X5      */   ACT_S5,        -1,      -1,       X6,     SWEEP,     BURST,     LINEAR,    -1,
    /* X6      */   ACT_S6,        -1,      -1,       X7,     SWEEP,     BURST,     LINEAR,    -1,
    /* X7      */   ACT_S7,        -1,      -1,       X8,     SWEEP,     BURST,     LINEAR,    -1,
    /* X8      */   ACT_S6,        -1,      -1,       X9,     SWEEP,     BURST,     LINEAR,    -1,
    /* X9      */   ACT_S5,        -1,      -1,       XA,     SWEEP,     BURST,     LINEAR,    -1,
    /* XA      */   ACT_S4,        -1,      -1,       XB,     SWEEP,     BURST,     LINEAR,    -1,
    /* XB      */   ACT_S3,        -1,      -1,       XC,     SWEEP,     BURST,     LINEAR,    -1,
    /* XC      */   ACT_S2,        -1,      -1,       XD,     SWEEP,     BURST,     LINEAR,    -1,
    /* XD      */   ACT_S1,        -1,      -1,       X0,     SWEEP,     BURST,     LINEAR,    -1,
    /* BURST   */       -1,        -1,      -1,       U0,     SWEEP,     BURST,     LINEAR,    -1, // Burst mode	
    /* U0      */   ACT_S0,        -1,      -1,       -1,     SWEEP,     BURST,     LINEAR,    U1, 
    /* U1      */   ACT_S1,        -1,      -1,       -1,     SWEEP,     BURST,     LINEAR,    U2,
    /* U2      */   ACT_S2,        -1,      -1,       -1,     SWEEP,     BURST,     LINEAR,    U3,
    /* U3      */   ACT_S3,        -1,      -1,       -1,     SWEEP,     BURST,     LINEAR,    U4,
    /* U4      */   ACT_S4,        -1,      -1,       -1,     SWEEP,     BURST,     LINEAR,    U5,
    /* U5      */   ACT_S5,        -1,      -1,       -1,     SWEEP,     BURST,     LINEAR,    U6,
    /* U6      */   ACT_S6,        -1,      -1,       -1,     SWEEP,     BURST,     LINEAR,    U7,
    /* U7      */   ACT_S7,        -1,      -1,       -1,     SWEEP,     BURST,     LINEAR, BURST,
  };
  Machine::begin( state_table, ELSE );
  return *this;
}

Atm_step & Atm_step::onStep( uint8_t idx, stepcb_t callback )
{
  _step[idx]._mode = MODE_CALLBACK;
  _step[idx]._callback = callback;
  return *this;
}

Atm_step & Atm_step::onStep( uint8_t idx, Machine * machine, state_t event /* = 0 */ )
{
  _step[idx]._mode = MODE_MACHINE;
  _step[idx]._client_machine = machine;
  _step[idx]._client_machine_event = event;
  return *this;
}

Atm_step & Atm_step::onStep( uint8_t idx, const char * label, state_t event /* = 0 */ )
{
  _step[idx]._mode = MODE_FACTORY;
  _step[idx]._client_label = label;
  _step[idx]._client_label_event = event;
  return *this;
}

int Atm_step::event( int id )
{
  switch (id ) {
    case EVT_STEP: // TODO CHANGE TO TINY_READ_STATE for Tiny Machine!!!
      state_t on_enter = read_state( state_table + ( current * state_width ) + ATM_ON_ENTER );
      return ( on_enter > -1 ) && ( _step[on_enter]._mode == 0 );
  }
  return 0;
}

void Atm_step::action( int id )
{
  if ( id > -1 ) {
    switch ( _step[id]._mode ) {
      case MODE_CALLBACK:
        flags |= ATM_CALLBACK_FLAG;
        (*_step[id]._callback)( id );
        flags &= ~ATM_CALLBACK_FLAG;
        return;
      case MODE_MACHINE:
        _step[id]._client_machine->trigger( _step[id]._client_machine_event );
        return;
      case MODE_FACTORY:
        factory->trigger( _step[id]._client_label, _step[id]._client_label_event );
        return;
    }
  }
}

Atm_step & Atm_step::trace( Stream * stream ) {

  setTrace( stream, atm_serial_debug::trace,
            "EVT_STEP\0EVT_SWEEP\0EVT_BURST\0EVT_LINEAR\0ELSE\0"
			"IDLE\0LINEAR\0S0\0S1\0S2\0S3\0S4\0S5\0S6\0S7\0"
			"SWEEP\0X0\0X1\0X2\0X3\0X4\0X5\0X6\0X7\0X8\0X9\0XA\0XB\0XC\0XD\0"
			"BURST\0U0\0U1\0U2\0U3\0U4\0U5\0U6\0U7" );
			return *this;
}

