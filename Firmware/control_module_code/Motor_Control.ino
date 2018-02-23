void goToFlap( char character ) {
  #ifdef debug
    Serial.print( "CHAR: " );
    Serial.println( character );
  #endif
  
  int flapIndex = flaps.indexOf( character );
  
  if ( flapIndex == -1 )
    flapIndex = lowercaseFlaps.indexOf( character );
  
  goToFlapIndex( flapIndex == -1 ? 0 : flapIndex );
}

void goToFlapIndex( int flapIndex ) {
  int flapOffset = abs( flapIndex - currentFlapIndex );
  
  if ( flapIndex - currentFlapIndex < 0 )
    flapOffset = NUM_FLAPS - flapOffset;
  
  currentFlapIndex = flapIndex;
  
  for ( int i = 0; i < ( flapOffset * STEPPER_MICROSTEPS ); i++ ) {
    digitalWrite( STEPPER_STEP_PIN, HIGH );
    delay( STEPPER_SPEED );
    digitalWrite( STEPPER_STEP_PIN, LOW );
    delay( STEPPER_SPEED );
  }
}
