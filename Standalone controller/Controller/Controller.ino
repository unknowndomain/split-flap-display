#include <LiquidCrystal.h>
#include <Metro.h>

#define UPDATE_FREQUENCY 5000

#define NO_BTN     0
#define SELECT_BTN 1
#define UP_BTN     2
#define DOWN_BTN   3
#define LEFT_BTN   4
#define RIGHT_BTN  5

#define RST_PIN  8
#define EN_PIN   9
#define D4_PIN   4
#define D5_PIN   5
#define D6_PIN   6
#define D7_PIN   7
#define BACKLIGHT_PIN 10
#define BTN_PIN  A0

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

LiquidCrystal lcd( RST_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN );

Metro updateMessage = Metro( UPDATE_FREQUENCY );

boolean button_state = false;

String flaps = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@#:/$£,.!-&?*";

char* messages[] = {
  "?????      ",
  "????? HELLO",
  "????? THERE",
  "?????      ",
  "????? SOUTH",
  "?????  LDN ",
  "????? MAKER",
  "????? SPACE",
  "?????      ",
  "?????  GLA ",
  "?????      "
};

String edited_message;
int num_messages = sizeof( messages ) / sizeof( char* );
int message_length = 11;
int current_message = 1;
int cursor_position = 0;

boolean edit_mode = false;

byte left[8] = {
  B00000,
  B00100,
  B01100,
  B11111,
  B01100,
  B00100,
  B00000,
  B00000
};

byte right[8] = {
  B00000,
  B00100,
  B00110,
  B11111,
  B00110,
  B00100,
  B00000,
  B00000
};

byte up[8] = {
  B00100,
  B01110,
  B11111,
  B00100,
  B00100,
  B00100,
  B00100,
  B00000
};

byte down[8] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B11111,
  B01110,
  B00100,
  B00000
};

void setup() {
  // Start Serial
  Serial.begin( 9600 );
  Serial1.begin( 9600 );
  while ( ! Serial && ! Serial1 );
  
  // Start LCD
  lcd.begin( 16, 2 );
  pinMode( BACKLIGHT_PIN, OUTPUT );
  digitalWrite( BACKLIGHT_PIN, HIGH );
  lcd.createChar( 0, left );
  lcd.createChar( 1, right );
  lcd.createChar( 2, up );
  lcd.createChar( 3, down );

  // Redraw display
  new_message();
  draw();
}

void loop() {
  if ( ! edit_mode && updateMessage.check() )
    new_message();
    
  if ( edit_mode && updateMessage.check() ) {
    exitEditMode();
  }
  
  int btn = get_button_status();
  if ( btn != NO_BTN ) {
    // Enable edit mode
    enterEditMode();
    updateMessage.reset();
    
    
    // Prevent button repeating
    if ( ! button_state ) {
      button_state = true;
    
      // Handle edit mode buttons
      if ( edit_mode ) {
        
        // Up button
        if ( btn == UP_BTN ) {
            transmit_message( cursor_position + 1, POKE_CMD, 0x00 );
        }
        
        // Down button
        if ( btn == DOWN_BTN ) {
            transmit_message( cursor_position + 1, NUDGE_CMD, 0x00 );
            Serial.print( "NUDGE: " );
            Serial.println( cursor_position+1 );
        }
        
        // Select button
        if ( btn == SELECT_BTN ) {
          exitEditMode();
          new_message();
        }
        
      }
      
      // Cursor LEFT / RIGHT
      if ( btn == LEFT_BTN ) {
        enterEditMode();
        cursor_position--;
      } else if ( btn == RIGHT_BTN ) {
        edit_mode = true;
        cursor_position++;
      }
      
      // Limit cursor postion
      if ( cursor_position < 0 ) cursor_position = 0;
      if ( cursor_position > message_length-1 ) cursor_position = message_length -1;
      
      // Skip center space
      if ( cursor_position == 5 && btn == LEFT_BTN ) cursor_position = 4;
      if ( cursor_position == 5 && btn == RIGHT_BTN ) cursor_position = 6;
      
      // Redraw screen
      draw();
    }
  } else { 
    button_state = false;
  }
}

void edit_message() {
  for ( int i = 0; i < message_length; i++ )
    transmit_message( i+1, CALI_CMD, edited_message[ i ] );
  draw();
}

void new_message() {
  current_message++;
  if ( current_message > num_messages - 1 ) current_message = 0;
  draw();
  for ( int i = 0; i < message_length; i++ ) {
    transmit_message( i + 1, GOTO_CMD, messages[ current_message ][ i ] );
//    delay( 100 );
  }
}

void enterEditMode() {
  if ( ! edit_mode ) {
    edit_mode = true;
    edited_message = messages[ current_message ];
  }
}

void exitEditMode() {
  if ( edit_mode ) {
    edit_mode = false;
    edited_message = "";
    cursor_position = 0;
    draw();
  }
}

void draw() {
  lcd.clear();
  if ( edit_mode ) {
    lcd.write( byte( 2 ) );
    lcd.print( "     Edit     " );
    lcd.write( byte( 3 ) );
    lcd.blink();
    lcd.setCursor( 2, 1 );
    lcd.print( edited_message );
    lcd.setCursor( cursor_position + 2, 1 );
  } else {
    lcd.noBlink();
    lcd.write( byte( 0 ) );
    lcd.print( "    Select    " );
    lcd.write( byte( 1 ) );
    lcd.setCursor( 2, 1 );
    lcd.print( messages[ current_message ] );
  }
}
