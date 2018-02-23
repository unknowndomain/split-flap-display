byte crc_calc( byte adr_char, byte cmd_char, byte var_char ) {
  return STX ^ adr_char ^ cmd_char ^ var_char ^ ETX;
}

boolean crc_check( byte adr_char, byte cmd_char, byte var_char, byte original_crc ) {
  byte new_crc = byte( STX ^ adr_char ^ cmd_char ^ var_char ^ ETX );
  return original_crc == new_crc ? true : false;
}
