#include <string.h>
#include <Arduino.h>
#include <bluefruit.h>

#define READ_BUFSIZE                    (499)
uint8_t packetbuffer[READ_BUFSIZE+1];

uint8_t readPacket(BLEUart *ble_uart, uint16_t timeout) 
{
  uint16_t origtimeout = timeout, replyidx = 0;
  memset(packetbuffer, 0, READ_BUFSIZE);

  while (timeout--) {
    if (replyidx >= 499) break;

    while (ble_uart->available()) {
      char c =  ble_uart->read();
      if (c == '!') {
        replyidx = 0;
      }
      packetbuffer[replyidx] = c;
      replyidx++;
      timeout = origtimeout;
    }
    
    if (timeout == 0) break;
    delay(1);
  }

  packetbuffer[replyidx] = 0;  // null term

  if (!replyidx)  // no data or timeout 
    return 0;
  if (packetbuffer[0] != '!')  // doesn't start with '!' packet beginning
    return 0;
  
  // // check checksum!
  // uint8_t xsum = 0;
  // uint8_t checksum = packetbuffer[replyidx-1];
  
  // for (uint8_t i=0; i<replyidx-1; i++) {
  //   xsum += packetbuffer[i];
  // }
  // xsum = ~xsum;

  // // Throw an error message if the checksum's don't match
  // if (xsum != checksum)
  // {
  //   Serial.print("Checksum mismatch in packet : ");
  //   printHex(packetbuffer, replyidx+1);
  //   return 0;
  // }

  return replyidx;
}

