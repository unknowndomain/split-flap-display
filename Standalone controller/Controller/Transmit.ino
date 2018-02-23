void transmit_message( int adr, int cmd, int var ) {
  Serial1.write( STX );
  Serial1.write( adr );
  Serial1.write( cmd );
  Serial1.write( var );
  Serial1.write( calculate_crc( adr, cmd, var ) );
  Serial1.write( ETX );
}

int calculate_crc( int adr, int cmd, int var ) {
  return STX ^ adr ^ cmd ^ var ^ ETX;
}
