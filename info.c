/*
 * info.c
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

void GetCardInfo(unsigned char *buffer, unsigned short offset, ov_cardinfo *ovCardinfo) {
   unsigned char *bPoint = buffer + offset;

   ovCardinfo->company = (bPoint[6] >> 3) & 0x1F; /* Could be 4 bits though */
   ovCardinfo->expirationdate = ((bPoint[6] & 0x07) << 11) | ((bPoint[7] & 0xFF) << 3) | ((bPoint[8] >> 5) & 0x07);

   if ((bPoint[13] & 0x02) == 0x02) { /* Has date of birth */
      ovCardinfo->birthyear = ((bPoint[14] & 0xFF) << 8) | ((bPoint[15] & 0xFF));
      ovCardinfo->birthmonth = ((bPoint[16] & 0xFF));
      ovCardinfo->birthday = ((bPoint[17] & 0xFF));
   } else {
      ovCardinfo->birthyear = 0;
      ovCardinfo->birthmonth = 0;
      ovCardinfo->birthday = 0;
   }
}
