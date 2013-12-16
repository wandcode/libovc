/*
 * transaction.c
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

void OvcClassicTransaction(unsigned char *buffer, int offset, ov_transaction *ovTrans) {
   unsigned char *bPoint = buffer + offset;
   memset(ovTrans, 0, sizeof(ov_transaction));
   int iBitOffset = 0;

   ovTrans->location = offset;
   ovTrans->fieldbits = (bitmask_transaction)(GetBitsFromBuffer(bPoint, 0, 28));
   iBitOffset += 28;

   ovTrans->action = -3; /* no-data */
   ovTrans->date = GetBitsFromBuffer(bPoint, iBitOffset, 14);
   iBitOffset += 14;
   ovTrans->time = GetBitsFromBuffer(bPoint, iBitOffset, 11);
   iBitOffset += 11;

   if (ovTrans->fieldbits & bit_trs_unknown1) {
      ovTrans->unknown1 = GetBitsFromBuffer(bPoint, iBitOffset, 24);
      iBitOffset += 24;
   }

   if (ovTrans->fieldbits & bit_trs_action) {
      ovTrans->action = GetBitsFromBuffer(bPoint, iBitOffset, 7);
      iBitOffset += 7;
   }

   if (ovTrans->fieldbits & bit_trs_company) {
      ovTrans->company = GetBitsFromBuffer(bPoint, iBitOffset, 16);
      iBitOffset += 16;
   }

   if (ovTrans->fieldbits & bit_trs_transactionid) {
      ovTrans->transactionid = GetBitsFromBuffer(bPoint, iBitOffset, 24);
      iBitOffset += 24;
   }

   if (ovTrans->fieldbits & bit_trs_stationid) {
      ovTrans->stationid = GetBitsFromBuffer(bPoint, iBitOffset, 16);
      iBitOffset += 16;
   }

   if (ovTrans->fieldbits & bit_trs_machineid) {
      ovTrans->machineid = GetBitsFromBuffer(bPoint, iBitOffset, 24);
      iBitOffset += 24;
   }

   if (ovTrans->fieldbits & bit_trs_vehicleid) {
      ovTrans->vehicleid = GetBitsFromBuffer(bPoint, iBitOffset, 16);
      iBitOffset += 16;
   }

   if (ovTrans->fieldbits & bit_trs_productid) {
      ovTrans->productid = GetBitsFromBuffer(bPoint, iBitOffset, 5);
      iBitOffset += 5;
   }

   if (ovTrans->fieldbits & bit_trs_unknown2) {
      ovTrans->unknown2 = GetBitsFromBuffer(bPoint, iBitOffset, 16);
      iBitOffset += 16;
   }

   if (ovTrans->fieldbits & bit_trs_amount) {
      ovTrans->amount = GetBitsFromBuffer(bPoint, iBitOffset, 16);
      iBitOffset += 16;
   }

   if (ovTrans->fieldbits & bit_trs_subscriptionid) {
      ovTrans->subscriptionid = GetBitsFromBuffer(bPoint, iBitOffset, 13);
      iBitOffset += 13;
   }
}

/*
 * Get all transactions
 *
 * This includes:
 * - Transactionhistory (11 slots -- 0xB00 - 0xC60)
 * - Travelhistory (13 slots -- 0xC80 - 0xE40)
 * - Charges (3 slots -- 0xE80 - 0xEC0)
 * - Unused slot 0xE60
 */
ov_transaction* GetAllTransactions(unsigned char *buffer) {
   uint8_t count = 28; /* Number of slots including the unused slot */
   int offset = 0xB00;

   ov_transaction *transactions = (ov_transaction*)malloc(count * sizeof(ov_transaction));
   if (transactions == NULL) {
      return;
   }

   uint8_t i;
   for(i = 0; i < count; i++) {
      offset = (i < 7 ? (0xB00 + i * 0x20)
              : i > 24 ? (0xE80 + (i - 25) * 0x20)
              : i > 20 ? (0xE00 + (i - 21) * 0x20)
              : i > 13 ? (0xD00 + (i - 14) * 0x20)
              : (0xC00 + (i - 7) * 0x20));

      OvcClassicTransaction(buffer, offset, &transactions[i]);
   }

   return transactions;
}

/* Get the transactionhistory according to the index, ranges from 0xB00 - 0xC60*/
ov_transaction* GetTransactionhistory(unsigned char *buffer, ov_indexes *indexes) {
   uint8_t count = 10;

   ov_transaction *transactions = (ov_transaction*)malloc(count * sizeof(ov_transaction));
   if (transactions == NULL) {
      return NULL;
   }

   uint8_t i;
   for(i = 0; i < count; i++) {
      OvcClassicTransaction(buffer, indexes->transactionhistoryindexes[i], &transactions[i]);
   }

   return transactions;
}

/* Get the travelhistory according to the index, ranges from 0xC80 - 0xE40 */
ov_transaction* GetTravelhistory(unsigned char *buffer, ov_indexes *indexes) {
   uint8_t count = 12;

   ov_transaction *transactions = (ov_transaction*)malloc(count * sizeof(ov_transaction));
   if (transactions == NULL) {
      return NULL;
   }

   uint8_t i;
   for(i = 0; i < count; i++) {
      OvcClassicTransaction(buffer, indexes->travelhistoryindexes[i], &transactions[i]);
   }

   return transactions;
}
