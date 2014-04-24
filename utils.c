/*
 * utils.c
 *
 * libovc, a library to extract all OV-chipkaart data.
 *
 * Copyright (C) 2013  Wilbert Duijvenvoorde
 *
 * Authors:
 * Wilbert Duijvenvoorde
 * 'Huuf' (http://www.huuf.info/OV/)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "libovc.h"

uint32_t GetBitsFromBuffer(uint8_t *buffer, uint8_t iStartBit, uint8_t iLength) {
   int iEndBit = iStartBit + iLength - 1;
   int iSByte = iStartBit / 8;
   int iSBit = iStartBit % 8;
   int iEByte = iEndBit / 8;
   int iEBit = iEndBit % 8;

   if (iSByte == iEByte) {
      return (uint32_t)((buffer[iEByte] >> (7 - iEBit)) & (0xFF >> (8 - iLength)));
   } else {
      uint32_t uRet = ((buffer[iSByte] & (0xFF >> iSBit)) << (((iEByte - iSByte - 1) * 8) + (iEBit + 1)));

      int i;
      for (i = iSByte + 1; i < iEByte; i++) {
         uRet |= ((buffer[i] & 0xFF) << (((iEByte - i - 1) * 8) + (iEBit + 1)));
      }
      uRet |= (buffer[iEByte] >> (7 - iEBit));

      return uRet;
   }
}

uint32_t GetBitsFromInteger(int buffer, int iStartBit, int iLength) {
   return (buffer >> (iStartBit)) & (0xFF >> (8 - iLength));
}

uint32_t ConvertBCDtoInteger(unsigned char buffer) {
   return (((buffer & 0xF0) >> 4) * 10) + ((buffer & 0x0F));
}
