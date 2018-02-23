#include <EEPROM.h>

/*

  PROTOCOL:
  
  START CHAR, ADDRESS BYTE, COMMAND, VARIABLE, CRC, END CHAR
  
  START CHAR   - 0x02
  
  ADDRESS BYTE - 0x00 - All units
               - 0x01 - 0xFF         255 devices
  
  COMMAND      - 0x00 = Blank        Go to zero
               - 0x01 = Identify     Set status LED
               - 0x02 = Reset        Home and clear identify mode
               - 0x03 = Set address
               - 0x04 = Go to flap   Looks up flap index and goes to it
               - 0x05 = Nudge        Nudges the unit one flap
               - 0x06 = Poke         Pokes the unit one stepper steps
               - 0x07 = Calibrate    Updates the position the stepper actually is at
               
  VARIABLE     - 
  
  CRC          -
  
  END CHAR     - 0x03
  
 */
 
// Uncomment to turn on debug mode (w/Verbose output to Serial)
#define debug true

#define STATUS_LED_PIN      13
#define STEPPER_STEP_PIN    9
#define STEPPER_MICROSTEPS  32
#define STEPPER_SPEED       2
#define NUM_FLAPS           50

// Definitions
#define STX                 0x02
#define ETX                 0x03

#define ALL_UNITS           0x00
#define BLANK_CMD           0x00
#define ID_CMD              0x01
#define RESET_CMD           0x02
#define SETADR_CMD          0x03
#define GOTO_CMD            0x04
#define NUDGE_CMD           0x05
#define POKE_CMD            0x06
#define CALI_CMD            0x07

int currentFlapIndex = 0;
int originalFlapIndex;
byte current_adr;

String flaps =          " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@#:/$£,.!-&?*";
String lowercaseFlaps = " abcdefghijklmnopqrstuvwxyz0123456789@#;/$£,.!_&?*";

boolean eeprom_write = false;
boolean identify_mode = false;

void setup() {
  pinMode( STATUS_LED_PIN, OUTPUT );
  pinMode( STEPPER_STEP_PIN, OUTPUT );
  Serial.begin( 9600 );
  Serial.flush();
  
  eeprom_init();
  
  #ifdef debug
    Serial.print( "NOTICE: Address configured on: 0x" );
    Serial.println( current_adr, HEX );
  #endif
}

void loop() {
  checkSerialData();
}
