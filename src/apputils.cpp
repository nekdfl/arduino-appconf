#include "apputils.hpp"

std::list<String> splitPath(const String &a_path, const char *cd)
{
    String tmp;
    std::list<String> reslist;
    for (auto ch : a_path)
    {
        if (ch == *cd)
        {
            if (!tmp.isEmpty())
                reslist.push_back(tmp);
            tmp.clear();
        }
        else
        {
            tmp += ch;
        }
    }
    if (!tmp.isEmpty())
        reslist.push_back(tmp);

    return reslist;
};

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

unsigned short crc16_CCITT(unsigned char *pcBlock, unsigned short len)
{
    unsigned short crc = 0xFFFF;
    unsigned char i;

    while (len--)
    {
        crc ^= *pcBlock++ << 8;

        for (i = 0; i < 8; i++)
            crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
    }
    return crc;
}

unsigned short crc16_CCITT(String a_bcBlock)
{
    unsigned char *pcBlock = (unsigned char *)a_bcBlock.c_str();
    unsigned short len = a_bcBlock.length();
    unsigned short crc = 0xFFFF;
    unsigned char i;

    while (len--)
    {
        crc ^= *pcBlock++ << 8;

        for (i = 0; i < 8; i++)
            crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
    }
    return crc;
}
