/*
 * slots.c
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

void GetSlots(unsigned char *buffer, ov_slots *ovSlots) {
   uint8_t *bPoint;

   bPoint = buffer + 0xFB0; /* First index slot */
   uint16_t i = 0;
   i = ((uint16_t)((bPoint[1] & 0x03) << 10) | (uint16_t)((bPoint[2] & 0xFF) << 2) | (uint8_t)((bPoint[3] >> 6) & 0x03));
   bPoint = buffer + 0xFD0; /* Second index slot */
   uint16_t j = 0;
   j = ((uint16_t)((bPoint[1] & 0x03) << 10) | (uint16_t)((bPoint[2] & 0xFF) << 2) | (uint8_t)((bPoint[3] >> 6) & 0x03));

   ovSlots->transactionindex = (i > j ? 0xFB0 : 0xFD0);

   bPoint = buffer + ovSlots->transactionindex;

   uint8_t cardindex = ((bPoint[3] >> 5) & 0x01);
   ovSlots->informationindex = (cardindex == 0x01 ? 0x5C0 : 0x580);

   uint8_t indexes = ((bPoint[31] >> 5) & 0x07);

   ovSlots->subscriptionindex = ((indexes & 0x04) == 0x00 ? 0xF10 : 0xF30);
   ovSlots->travelhistoryindex = ((indexes & 0x02) == 0x00 ? 0xF50 : 0xF70);
   ovSlots->creditindex = ((indexes & 0x01) == 0x00 ? 0xF90 : 0xFA0);

   /* Not a slot, but not an index either... so just putting it here for now */
   uint8_t transaction = (bPoint[30] & 0x0F);
   ovSlots->recenttransaction = ((transaction & 0x0F) == 0x01 ? 0xE80 : (transaction & 0x0F) == 0x06 ? 0xEA0 : 0xEC0);
}
