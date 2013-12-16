/*
 * credit.c
 *
 * libovc, a library to extract all OV-chipkaart data.
 *
 * Copyright (C) 2013  Wilbert Duijvenvoorde
 *
 * Authors:
 * Wilbert Duijvenvoorde
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

void GetCredits(unsigned char *buffer, unsigned short offset, ov_credit *ovCredit) {
   unsigned char *bPoint = buffer + offset;

   ovCredit->banbits = ((bPoint[0] & 0xFF) << 1) | ((bPoint[1] >> 7) & 0x01);
   ovCredit->id = ((bPoint[1] & 0x7F) << 5) | ((bPoint[2] >> 3) & 0x1F);
   ovCredit->creditid = ((bPoint[6] & 0x0F) << 12) | ((bPoint[7] & 0xFF) << 4) | ((bPoint[8] >> 4) & 0x0F);
   int credit = ((bPoint[9] & 0x03) << 13) | ((bPoint[10] & 0xFF) << 5) | ((bPoint[11] >> 3) & 0x1F);

   if ((bPoint[9] & 0x04) != 4) {
      credit ^= (char)0x7FFF;
      credit *= -1;
   }

   ovCredit->credit = credit;
}
