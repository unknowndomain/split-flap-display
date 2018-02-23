#define EEPROM_START_POS  0
#define EEPROM_ADR_POS    1
#define EEPROM_END_POS    2

void eeprom_init() {
  if ( EEPROM.read( EEPROM_START_POS ) == STX && EEPROM.read( EEPROM_END_POS ) == ETX ) {
    eeprom_read_config();
  } else {
    eeprom_write_defaults();
  }
}

void store_address( byte adr ) {
  if ( eeprom_write == false ) {
    EEPROM.write( EEPROM_ADR_POS, adr );
    eeprom_write = true;
    #ifdef debug
      Serial.print( "NOTICE: New address stored in EEPROM: 0x" );
      Serial.println( adr, HEX );
    #endif
    digitalWrite( STATUS_LED_PIN, HIGH );
    delay ( 250 );
    digitalWrite( STATUS_LED_PIN, LOW );
  } else {
    #ifdef debug
      Serial.println( "WARNING: Cannot write to EEPROM more than once per power cycle without a soft reset" );
    #endif
  }
}

void eeprom_write_defaults() {
  if ( eeprom_write == false ) {
    EEPROM.write( EEPROM_START_POS, STX );
    store_address( 0x01 );
    EEPROM.write( EEPROM_END_POS, ETX );  
    #ifdef debug
      Serial.println( "NOTICE: Default values written to EEPROM" );
    #endif
  } else {
    #ifdef debug
      Serial.println( "WARNING: Cannot write to EEPROM more than once per power cycle without a soft reset" );
    #endif
  }
}

void eeprom_read_config() {
  current_adr = EEPROM.read( EEPROM_ADR_POS );
}
