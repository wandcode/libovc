/*
 * index.c
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

/*
 *
 * TODO: rename variables to be consistent all across the source
 *
 */

void GetUnknownIndexes(unsigned char *buffer, uint16_t bPoint, ov_indexes *ovIndexes) {
   uint8_t offset = 32;

   uint8_t i;
   for (i = 0; i < 7; i++) {
      ovIndexes->unknownindexes[i] = GetBitsFromBuffer(buffer + bPoint, offset + (i * 4), 4);
   }
}

void GetSubscriptionStatusIndexes(unsigned char *buffer, uint16_t bPoint, ov_indexes *ovIndexes) {
   uint8_t offset = 60;

   uint8_t i;
   for (i = 0; i < 12; i++) {
      uint8_t bits = GetBitsFromBuffer(buffer + bPoint, offset + (i * 4), 4);
      ovIndexes->subscriptionstatusindexes[i] =
         (bits < 3 ? (0x6C0 + bits * 0x10)
        : bits > 11 ? (0x7C0 + (bits - 12) * 0x10)
        : bits > 8 ? (0x780 + (bits - 9) * 0x10)
        : bits > 5 ? (0x740 + (bits - 6) * 0x10)
        : (0x700 + (bits - 3) * 0x10));
   }
}

void GetSubscriptionIndexes(unsigned char *buffer, uint16_t bPoint, ov_indexes *ovIndexes) {
   uint8_t offset = 108;

   uint8_t i;
   for (i = 0; i < 12; i++) {
      uint8_t bits = GetBitsFromBuffer(buffer + bPoint, offset + (i * 4), 4);
      ovIndexes->subscriptionindexes[i] =
         (bits < 5 ? (0x800 + bits * 0x30)
        : bits > 9 ? (0xA00 + (bits - 10) * 0x30)
        : (0x900 + (bits - 5) * 0x30));
   }
}

void GetTransactionHistoryIndexes(unsigned char *buffer, uint16_t bPoint, ov_indexes *ovIndexes) {
   uint8_t offset = 156;

   uint8_t i;
   for (i = 0; i < 10; i++) {
      uint8_t bits = GetBitsFromBuffer(buffer + bPoint, offset + (i * 4), 4);
      ovIndexes->transactionhistoryindexes[i] = (bits < 7 ? (0xB00 + bits * 0x20) : (0xC00 + (bits - 7) * 0x20));
   }
}

void GetTravelHistoryIndexes(unsigned char *buffer, uint16_t bPoint, ov_indexes *ovIndexes) {
   uint8_t offset = 196;

   uint8_t i;
   for (i = 0; i < 12; i++) {
      uint8_t bits = GetBitsFromBuffer(buffer + bPoint, offset + (i * 4), 4);
      ovIndexes->travelhistoryindexes[i] =
         (bits < 3 ? (0xC80 + bits * 0x20)
        : bits > 9 ? (0xE00 + (bits - 10) * 0x20)
        : (0xD00 + (bits - 3) * 0x20));
   }
}

void GetAllIndexes(unsigned char *buffer, uint16_t bPoint, ov_indexes *ovIndexes) {
   GetUnknownIndexes(buffer, bPoint, ovIndexes);
   GetSubscriptionStatusIndexes(buffer, bPoint, ovIndexes);
   GetSubscriptionIndexes(buffer, bPoint, ovIndexes);
   GetTransactionHistoryIndexes(buffer, bPoint, ovIndexes);
   GetTravelHistoryIndexes(buffer, bPoint, ovIndexes);
}

ov_subscriptionindices* GetSubscriptionIndices(unsigned char *buffer, uint16_t bPoint, uint8_t *count, ov_indexes *ovIndexes) { /* input 0xf10 or 0xf30 */
   *count = GetBitsFromBuffer(buffer + bPoint, 0, 4);
   ov_subscriptionindices *indice;

   if ((indice = calloc(*count, sizeof(ov_subscriptionindices))) == NULL) {
      return (NULL);
   }

   uint8_t offset = 4;

   uint8_t i;
   for (i = 0; i < *count; i++) {
      uint32_t bits = GetBitsFromBuffer(buffer + bPoint, offset + (i * 21), 21);

      indice[i].type1 = GetBitsFromInteger(bits, 13, 8);
      indice[i].type2 = GetBitsFromInteger(bits, 7, 6);
      indice[i].used = GetBitsFromInteger(bits, 6, 1);
      indice[i].rest = GetBitsFromInteger(bits, 4, 2);
      indice[i].indexid = GetBitsFromInteger(bits, 0, 4);
      indice[i].index = ovIndexes->subscriptionindexes[indice[i].indexid - 1];
   }

   return indice;
}

ov_transactionindices* GetTravelIndices(unsigned char *buffer, unsigned short bPoint, uint8_t *count, ov_indexes *ovIndexes) { /* input 0xf50 or 0xf70 */
   *count = GetBitsFromBuffer(buffer + bPoint, 0, 4);
   ov_transactionindices *indice;

   if ((indice = calloc(*count, sizeof(ov_transactionindices))) == NULL) {
      return (NULL);
   }

   uint8_t offset = 4;

   uint8_t i;
   for (i = 0; i < *count; i++) {
      indice[i].identifier = GetBitsFromBuffer(buffer + bPoint, offset, 6);
      offset += 6;

      indice[i].company = GetBitsFromBuffer(buffer + bPoint, offset, 8);
      offset += 8;

      if ((indice[i].identifier & 0x10) != 0x00) {
         indice[i].status = GetBitsFromBuffer(buffer + bPoint, offset, 2);
         offset += 2;
      } else {
         indice[i].status = -1; /* Set it to negative (not available) */
      }

      indice[i].indexid = GetBitsFromBuffer(buffer + bPoint, offset, 4);
      indice[i].index = ovIndexes->travelhistoryindexes[indice[i].indexid - 1];
      offset += 4;

      if ((indice[i].identifier & 0x20) != 0x00) {
         indice[i].unknown = GetBitsFromBuffer(buffer + bPoint, offset, 4);
         offset += 4;
      } else {
         indice[i].unknown = -1; /* Set it to negative (not available) */
      }
   }

   return indice;
}
