void checkSerialData() {
  // Only start if there is any data available
  if ( Serial.available() > 0 ) {

    // Look to see if the first byte is a start character, otherwise nudge it off
    if ( Serial.peek() == STX ) {

      // If there is a full packet waiting
      if ( Serial.available() >= 6 ) {

        // Read in a whole packet of data
        byte start_char = Serial.read();
        byte adr = Serial.read();
        byte cmd = Serial.read();
        byte var = Serial.read();
        byte crc = Serial.read();
        byte end_char = Serial.read();
      
        // Check that the first and last chars are valid
        if ( start_char == STX && end_char == ETX ) {
          
          // Stop if address doesn't match
          if ( adr != ALL_UNITS && adr != current_adr ) {
            #ifdef debug
              Serial.println( "WARNING: Address doesn't match" );
            #endif
            return;
          }
          
          // Stop if CRC error
          if ( crc_check( adr, cmd, var, crc ) == false ) {
            #ifdef debug
              Serial.println( "ERROR: CRC invalid" );
            #endif
            return;
          }
          
          // Stop if invalid command
          if ( cmd > 0x07 ) {
            #ifdef debug
              Serial.println( "ERROR: Invalid command" );
            #endif
            return;
          }
                    
          // Perform command
          switch ( cmd ) {
            case BLANK_CMD:
              goToFlap( ' ' );
            break;
            
            case ID_CMD:
              if ( var == 0x01 ) {
                digitalWrite( STATUS_LED_PIN, HIGH );
                #ifdef debug
                  Serial.println( "NOTICE: Status LED On" );
                #endif
              } else if ( var == 0x00 ) {
                digitalWrite( STATUS_LED_PIN, LOW );
                #ifdef debug
                  Serial.println( "NOTICE: Status LED Off" );
                #endif
              } else {
                #ifdef debug
                  Serial.println( "WARNING: Invalid variable" );
                #endif
              }
            break;
            
            case RESET_CMD:           
              eeprom_write = false;
            break;
            
            case SETADR_CMD:
              if ( var != ALL_UNITS ) {
                store_address( var );
              } else {
                #ifdef debug
                  Serial.println( "ERROR: 0x00 is a protected address" );
                #endif
              }
            break;
            
            case GOTO_CMD:
              goToFlap( char( var ) );
            break;
            
            case NUDGE_CMD:
              for ( int i = 0; i < STEPPER_MICROSTEPS; i++ ) {
                digitalWrite( STEPPER_STEP_PIN, HIGH );
                delay( STEPPER_SPEED );
                digitalWrite( STEPPER_STEP_PIN, LOW );
                delay( STEPPER_SPEED );
              }
              #ifdef debug
                Serial.println( "NOTICE: Nudged" );
              #endif
            break;
            
            case POKE_CMD:
              digitalWrite( STEPPER_STEP_PIN, HIGH );
              delay( STEPPER_SPEED );
              digitalWrite( STEPPER_STEP_PIN, LOW );
              delay( STEPPER_SPEED );
               
              #ifdef debug
                Serial.println( "NOTICE: Poked" );
              #endif
            break;
            
            case CALI_CMD:
              originalFlapIndex = currentFlapIndex;
              currentFlapIndex = flaps.indexOf( var );
  
              if ( currentFlapIndex == -1 ) {
                currentFlapIndex = lowercaseFlaps.indexOf( var );
              }
              
              goToFlapIndex( originalFlapIndex );
               
              #ifdef debug
                Serial.println( "NOTICE: Calibrated" );
              #endif
            break;
            
            default:
              #ifdef debug
                Serial.println( "ERROR: Invalid command" );
                Serial.println( "CRITICAL: Invalid command slipped past error detection" );
              #endif
            break;
          }
        } else {
          #ifdef debug
            Serial.println( "ERROR: Command improperly formatted" );
          #endif
          return;
        }
      }
    } else {
      Serial.read();
    }
  }
}
