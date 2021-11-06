#ifndef APPUTILS_HPP
#define APPUTILS_HPP

/**
 * 
 * Utilite functions 
 * 
 *
 */

#include <Arduino.h>
#include <list>

/**
 * @brief split string by delimeter to list of string
 * 
 * @param a_path input string like /rootdir/file
 * @param cd delimter char like /
 * @return std::list<String> "rootdir", file
 */
std::list<String> splitPath(const String &a_path, const char *cd);

/*
  Name  : CRC-16 CCITT
  Poly  : 0x1021    x^16 + x^12 + x^5 + 1
  Init  : 0xFFFF
  Revert: false
  XorOut: 0x0000
  Check : 0x29B1 ("123456789")
  MaxLen: 4095 байт (32767 бит) - обнаружение
    одинарных, двойных, тройных и всех нечетных ошибок
*/
unsigned short crc16_CCITT(unsigned char *pcBlock, unsigned short len);

/*
  Name  : CRC-16 CCITT
  Poly  : 0x1021    x^16 + x^12 + x^5 + 1
  Init  : 0xFFFF
  Revert: false
  XorOut: 0x0000
  Check : 0x29B1 ("123456789")
  MaxLen: 4095 байт (32767 бит) - обнаружение
    одинарных, двойных, тройных и всех нечетных ошибок
*/
unsigned short crc16_CCITT(String a_pcBlock);

#endif