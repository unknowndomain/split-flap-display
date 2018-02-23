int get_button_status() {
  int button_value = analogRead( BTN_PIN );
  if ( button_value > 1000 ) {
    return NO_BTN;
  } else {
    if ( button_value < 50 ) {
      return RIGHT_BTN;
    } else if ( button_value < 195 ) {
      return UP_BTN;
    } else if ( button_value < 380 ) {
      return DOWN_BTN;
    } else if ( button_value < 555 ) {
      return LEFT_BTN;
    } else if ( button_value < 790 ) {
      return SELECT_BTN;
    }
  }
}
