/*
 * subscription.c
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

void OvcSubscription(unsigned char *buffer, int offset, ov_subscription *ovSubscription) {
   unsigned char *bPoint = buffer + offset;
   memset(ovSubscription, 0, sizeof(ov_subscription));
   int iBitOffset = 0;

   ovSubscription->location = offset;
   ovSubscription->fieldbits = (bitmask_subscription)(GetBitsFromBuffer(bPoint, 0, 28));
   iBitOffset += 28;

   if (ovSubscription->fieldbits & bit_sub_companyid) {
      ovSubscription->companyid = GetBitsFromBuffer(bPoint, iBitOffset, 8);
      iBitOffset += 8;
   }

   if (ovSubscription->fieldbits & bit_sub_subscriptiontype) {
      ovSubscription->subscriptiontype = GetBitsFromBuffer(bPoint, iBitOffset, 16);
      iBitOffset += 24;   /* Skipping the first 8 bits, as they are not used OR don't belong to subscriptiontype at all */
   }

   if (ovSubscription->fieldbits & bit_sub_subscriptionid) {
      ovSubscription->subscriptionid = GetBitsFromBuffer(bPoint, iBitOffset, 24);
      iBitOffset += 24;
   }

   if (ovSubscription->fieldbits & bit_sub_classtype) {
      ovSubscription->classtype = GetBitsFromBuffer(bPoint, iBitOffset, 10);
      iBitOffset += 10;
   }

   if (ovSubscription->fieldbits & bit_sub_subidentifier) {
      ovSubscription->subfieldbits1 = (bitmask_subscription)(GetBitsFromBuffer(bPoint, iBitOffset, 9));
      iBitOffset += 9;
   }

   if (ovSubscription->subfieldbits1 & bit_sub_validfromdate) {
      if (ovSubscription->subfieldbits1 & bit_sub_validfromdate) {
         ovSubscription->validfromdate = GetBitsFromBuffer(bPoint, iBitOffset, 14);
         iBitOffset += 14;
      }

      if (ovSubscription->subfieldbits1 & bit_sub_validfromtime) {
         ovSubscription->validfromtime = GetBitsFromBuffer(bPoint, iBitOffset, 11);
         iBitOffset += 11;
      }

      if (ovSubscription->subfieldbits1 & bit_sub_validtodate) {
         ovSubscription->validtodate = GetBitsFromBuffer(bPoint, iBitOffset, 14);
         iBitOffset += 14;
      }

      if (ovSubscription->subfieldbits1 & bit_sub_validtotime) {
         ovSubscription->validtotime = GetBitsFromBuffer(bPoint, iBitOffset, 11);
         iBitOffset += 11;
      }

      if (ovSubscription->subfieldbits1 & bit_sub_trip) {
         if (ovSubscription->fieldbits & bit_sub_tripidentifier) {
            /* Don't know what every bit represents, so it's not used and thus _might_ be incomplete/wrong */
            ovSubscription->subfieldbits2 = (bitmask_subscription)(GetBitsFromBuffer(bPoint, iBitOffset, 16));
            iBitOffset += 16;

            ovSubscription->tripfrom = GetBitsFromBuffer(bPoint, iBitOffset, 16);
            iBitOffset += 16;

            ovSubscription->tripto = GetBitsFromBuffer(bPoint, iBitOffset, 16);
            iBitOffset += 16;

            /* Don't know what every bit represents, so it's not used and thus _might_ be incomplete/wrong */
            ovSubscription->subfieldbits3 = (bitmask_subscription)(GetBitsFromBuffer(bPoint, iBitOffset, 4));
            iBitOffset += 4;

            ovSubscription->tripdate = GetBitsFromBuffer(bPoint, iBitOffset, 14);
            iBitOffset += 14;

            ovSubscription->triptime = GetBitsFromBuffer(bPoint, iBitOffset, 11);
            iBitOffset += 11;

            ovSubscription->tripunknown = GetBitsFromBuffer(bPoint, iBitOffset, 8);
            iBitOffset += 8;

            ovSubscription->tripcompanyid = GetBitsFromBuffer(bPoint, iBitOffset, 8);
            iBitOffset += 8;
         } else {
            /* Don't know what every bit represents, so it's not used and thus _might_ be incomplete/wrong */
            ovSubscription->subfieldbits2 = (bitmask_subscription)(GetBitsFromBuffer(bPoint, iBitOffset, 12));
            iBitOffset += 12;

            ovSubscription->somedate = GetBitsFromBuffer(bPoint, iBitOffset, 14);
            iBitOffset += 14;

            ovSubscription->sometime = GetBitsFromBuffer(bPoint, iBitOffset, 11);
            iBitOffset += 11;

            if (ovSubscription->subfieldbits2 & bit_sub_trip) { /* 0x0000010, has unknown values... */
               iBitOffset += 16; /* ...so let's skip the bits for now */
            } else if (ovSubscription->subfieldbits2 & bit_sub_unknown6) { /* 0x0000100, has unknown values and additional data... */
               iBitOffset += 44; /* ...so let's skip the bits for now */
            } else {
               ovSubscription->unknown1 = GetBitsFromBuffer(bPoint, iBitOffset, 8);
               iBitOffset += 8;

               ovSubscription->company = GetBitsFromBuffer(bPoint, iBitOffset, 8);
               iBitOffset += 8;
            }
         }
      }
   }

   if (ovSubscription->fieldbits & bit_sub_machineid) {
      ovSubscription->machineid = GetBitsFromBuffer(bPoint, iBitOffset, 24);
      iBitOffset += 24;
   }
}

/*
 * Get all subscriptions
 *
 * - 15 slots -- 0x800 - 0xAC0
 * - Skips the extra info, because not all of the slots have it (could use a rewrite to include it anyway)
 */
ov_subscription* GetAllSubscriptions(unsigned char *buffer) {
   uint8_t count = 15;
   int offset = 0x800;

   ov_subscription *subscriptions = (ov_subscription*)malloc(count * sizeof(ov_subscription));
   if (subscriptions == NULL) {
      return;
   }

   uint8_t i;
   for(i = 0; i < count; i++) {
      offset = (i < 5 ? (0x800 + i * 0x30)
              : i > 9 ? (0xA00 + (i - 10) * 0x30)
              : (0x900 + (i - 5) * 0x30));

      OvcSubscription(buffer, offset, &subscriptions[i]);
   }

   return subscriptions;
}

/* Get the subscriptions according to the index, includes the extra info */
ov_subscription* GetSubscriptions(unsigned char *buffer, int offset, ov_indexes *indexes) {
   uint8_t count = 0;

   ov_subscriptionindices *indices = GetSubscriptionIndices(buffer, offset, &count, indexes);
   if (indices == NULL) {
	   return NULL;
   }

   ov_subscription *subscriptions = (ov_subscription*)malloc(count * sizeof(ov_subscription));
   if (subscriptions == NULL) {
      return NULL;
   }

   uint8_t i;
   for(i = 0; i < count; i++) {
      OvcSubscription(buffer, indices[i].index, &subscriptions[i]);
   }

   return subscriptions;
}
