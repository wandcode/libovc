/*
 * autocharge.c
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

void GetAutoCharge(unsigned char *buffer, unsigned short offset, ov_autocharge *ovAutocharge) {
   unsigned char *bPoint = buffer + offset;

   if ((bPoint[13] & 0x02) == 0x02) { /* Has date of birth, so it's a personal card (no autocharge on anonymous cards) */
      ovAutocharge->active = (bPoint[22] >> 5) & 0x07;
      ovAutocharge->limit = ((bPoint[22] & 0x1F) << 11) | ((bPoint[23] & 0xFF) << 3) | ((bPoint[24] >> 5) & 0x07);
      ovAutocharge->charge = ((bPoint[24] & 0x1F) << 11) | ((bPoint[25] & 0xFF) << 3) | ((bPoint[26] >> 5) & 0x07);
      ovAutocharge->unknown = ((bPoint[26] & 0x1F) << 11) | ((bPoint[27] & 0xFF) << 3) | ((bPoint[28] >> 5) & 0x07);
   } else {
      ovAutocharge->active = 0;
      ovAutocharge->limit = 0;
      ovAutocharge->charge = 0;
      ovAutocharge->unknown = 0;
   }
}
