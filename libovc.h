/*
 * libovc.h
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

#ifndef LIBOVCDATA_H
#define LIBOVCDATA_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef enum bitmask_transaction {
   bit_trs_unknown1       = 0x0000002,
   bit_trs_action         = 0x0000004,
   bit_trs_company        = 0x0000010,
   bit_trs_transactionid  = 0x0000040,
   bit_trs_stationid      = 0x0000100,
   bit_trs_machineid      = 0x0000400,
   bit_trs_vehicleid      = 0x0004000,
   bit_trs_productid      = 0x0010000,
   bit_trs_unknown2       = 0x0100000,
   bit_trs_amount         = 0x0800000,
   bit_trs_subscriptionid = 0x2000000
} bitmask_transaction;

typedef struct {
   uint16_t location;           /* Address of the transaction (0x??? - 0x???) */        /* --- TODO --- */
   bitmask_transaction fieldbits;
   uint16_t date;               /* Days since 1997-01-01 */
   uint16_t time;               /* Minutes since midnight */
   uint32_t unknown1;           /* Unknown */
   int8_t action;               /* Action (Purchase: 0x00, Checkin: 0x01, Checkout: 0x02, Transfer: 0x06, No data: -0x03) */
   uint16_t company;            /* Company id */
   uint32_t transactionid;      /* Transaction id (unique) */
   uint16_t stationid;          /* Station id */
   uint32_t machineid;          /* Machine id */
   uint16_t vehicleid;          /* Vehicle id */
   uint8_t productid;           /* Product id */
   uint16_t unknown2;           /* Unknown */
   uint16_t amount;             /* Amount in Eurocents */
   uint16_t subscriptionid;     /* Subscription id (position in the subscriptionindex) */
} ov_transaction;

typedef enum bitmask_subscription {
   /* main identifiers */
   bit_sub_companyid        = 0x0000200,
   bit_sub_subscriptiontype = 0x0000400,
   bit_sub_subscriptionid   = 0x0000800,
   bit_sub_classtype        = 0x0002000,
   bit_sub_subidentifier    = 0x0200000,
   bit_sub_tripidentifier   = 0x0400000,
   bit_sub_machineid        = 0x0800000,
   /* sub identifiers */
   bit_sub_validfromdate    = 0x0000001,
   bit_sub_validfromtime    = 0x0000002,
   bit_sub_validtodate      = 0x0000004,
   bit_sub_validtotime      = 0x0000008,
   bit_sub_trip             = 0x0000010,
   /* sub trip identifiers */
   bit_sub_somedate         = 0x0000001,
   bit_sub_sometime         = 0x0000002,
   bit_sub_unknown1         = 0x0000004,
   bit_sub_company          = 0x0000008,
   bit_sub_unknown2         = 0x0000010,
   bit_sub_unknown3         = 0x0000020,
   bit_sub_unknown4         = 0x0000040,
   bit_sub_unknown5         = 0x0000080,
   bit_sub_unknown6         = 0x0000100,
   /* sub trip first identifiers */
   bit_sub_tripfrom         = 0x0000001,
   bit_sub_tripto           = 0x0000002,
   /* sub trip second identifiers */
   bit_sub_tripdate         = 0x0000001,
   bit_sub_triptime         = 0x0000002,
   bit_sub_tripunknown      = 0x0000004,
   bit_sub_tripcompanyid    = 0x0000008
} bitmask_subscription;

typedef struct {
   uint8_t type1;       /* Activation state (Activated: 0x01, Deactivated: 0x00) */
   uint8_t type2;       //
   uint8_t used;        /* Use state (Used: 0x01, Unused: 0x00) */
   uint8_t rest;        //
   uint8_t indexid;     /* Pointer to travel history index (ie. 1 -> [0], 2 -> [1], 3 -> [2], etc.) */
   uint16_t index;
} ov_subscriptionindices;

typedef struct {
   uint16_t location;                   /* Address of the subscription (0x800 - 0xAC0) */
   bitmask_subscription fieldbits;
   uint8_t companyid;                   /* Company id */
   uint16_t subscriptiontype;           /* Subscription type */
   uint32_t subscriptionid;             /* Subscription id */
   uint16_t classtype;                  /* Classtype (1st: 0x140, 2nd: 0x180, Unknown: everything else) */
   bitmask_subscription subfieldbits1;
   bitmask_subscription subfieldbits2;
   bitmask_subscription subfieldbits3;
   uint32_t machineid;                  /* Machine id */
   uint16_t validfromdate;              /* Valid from date (days since 1997-01-01) */
   uint16_t validfromtime;              /* Valid from time (minutes since midnight) */
   uint16_t validtodate;                /* Valid to date (days since 1997-01-01) */
   uint16_t validtotime;                /* Valid to time (minutes since midnight) */
   uint16_t somedate;                   /* Some date (days since 1997-01-01) */
   uint16_t sometime;                   /* Some time (minutes since midnight) */
   uint8_t unknown1;                    /* Unknown */
   uint8_t company;                     /* Company id */
   uint16_t tripfrom;                   /* Trip from (station id)*/
   uint16_t tripto;                     /* Trip to (station id)*/
   uint16_t tripdate;                   /* Trip date (days since 1997-01-01) */
   uint16_t triptime;                   /* Trip time (minutes since midnight) */
   uint8_t tripunknown;                 /* Unknown */
   uint8_t tripcompanyid;               /* Trip company id */
   ov_subscriptionindices extras;
} ov_subscription;

typedef struct {
   uint16_t informationindex;    /* 0x5C0 or 0x580 */
   uint16_t subscriptionindex;   /* 0xF10 or 0xF30 */
   uint16_t travelhistoryindex;  /* 0xF50 or 0xF70 */
   uint16_t creditindex;         /* 0xF90 or 0xFA0 */
   uint16_t transactionindex;    /* 0xFB0 or 0xFD0 */
   uint16_t recenttransaction;   /* 0xE80, 0xEA0 or 0xEC0 */
} ov_slots;

typedef struct {
   uint8_t active;      /* Activation state (Activated: 0x05) */
   uint16_t limit;      /* Amount limit in Eurocents before automatic charging */
   uint16_t charge;     /* Charge amount in Eurocents */
   uint16_t unknown;    /* Unknown */
} ov_autocharge;

typedef struct {
   uint8_t company;         /* Company id */
   uint16_t expirationdate; /* Expiration date (days since 1997-01-01) */
   uint16_t birthyear;      /* Birth year in hex/BCD format */
   uint8_t birthmonth;      /* Birth month in hex/BCD format */
   uint8_t birthday;        /* Birth day in hex/BCD format */
} ov_cardinfo;

typedef struct {
   uint16_t id;         /* Credit id */
   uint16_t creditid;   /* Credit id (transaction id) */
   uint16_t credit;     /* Credit amount in Eurocents */
   uint16_t banbits;    /* Banbits (Banned: 0xC0) */
} ov_credit;

typedef struct {
   uint8_t unknownindexes[7];               /* Unknown indexes */
   uint16_t subscriptionstatusindexes[12];  /* Subscriptionstatus indexes */
   uint16_t subscriptionindexes[12];        /* Subscription indexes */
   uint16_t transactionhistoryindexes[10];  /* Transactionhistory indexes */
   uint16_t travelhistoryindexes[12];       /* Travelhistory indexes */
} ov_indexes;

typedef struct {
   uint32_t id;                 /* Card id (serial number) */
   uint8_t checkbit;            /* Checkbit */
   uint64_t manufacturer;       /* Manufacturer */
   uint64_t publisher;          /* Publisher */
   uint8_t unknownConstant1;    /* Unknown -- 88 bits, too big to store, so we null it for now */
   uint32_t expdate;            /* Expiration date (days since 1997-01-01) */
   uint64_t unknownConstant2;   /* Unknown */
   uint8_t issuer;              /* Issuer (company id) */
   uint8_t type;                /* Type (Anonymous: 0x00, Personal: 0x02) */
} ov_preamble;

typedef struct {
   uint8_t identifier;
   uint8_t company;      /* Company id */
   int8_t status;        /* Status (Check-out: 0x00, Check-in: 0x01) */
   uint8_t indexid;      /* Pointer to travelhistory index (ie. 1 -> [0], 2 -> [1], 3 -> [2], etc.) */
   uint16_t index;
   int8_t unknown;       /* Unknown */
} ov_transactionindices;

#ifdef _WIN32   /* || _WIN64 */
   #define DllImport __declspec( dllimport )
#else
   #define DllImport
#endif

/* Functions */
DllImport void OvcClassicTransaction(unsigned char *buffer, int offset, ov_transaction *ovTrans);
DllImport void OvcSubscription(unsigned char *buffer, int offset, ov_subscription *ovSubscription);
DllImport void GetSlots(unsigned char *buffer, ov_slots *ovSlots);
DllImport void GetCredits(unsigned char*, unsigned short, ov_credit*);
DllImport void GetAllIndexes(unsigned char *buffer, uint16_t bPoint, ov_indexes *ovIndexes);
DllImport void GetPreamble(unsigned char *buffer, ov_preamble *ovPreamble);
DllImport void GetAutoCharge(unsigned char *buffer, unsigned short offset, ov_autocharge *ovAutocharge);
DllImport void GetCardInfo(unsigned char *buffer, unsigned short offset, ov_cardinfo *ovCardinfo);

DllImport ov_subscriptionindices* GetSubscriptionIndices(unsigned char *buffer, uint16_t bPoint, uint8_t *count, ov_indexes *ovIndexes);
DllImport ov_transactionindices* GetTravelIndices(unsigned char *buffer, unsigned short bPoint, uint8_t *count, ov_indexes *ovIndexes);
DllImport ov_transaction* GetTransactionhistory(unsigned char *buffer, ov_indexes *indexes);
DllImport ov_transaction* GetTravelhistory(unsigned char *buffer, ov_indexes *indexes);
DllImport ov_transaction* GetAllTransactions(unsigned char *buffer);
DllImport ov_subscription* GetSubscriptions(unsigned char *buffer, int offset, ov_indexes *indexes);

DllImport void GetClasstype(uint16_t classtype, char *out);
DllImport void GetCompanyName(uint8_t company, char *out);
DllImport void GetDateSince1997(uint16_t days, char *out);
DllImport void GetTime(uint16_t time, char *out);

/* Utils */
uint32_t GetBitsFromBuffer(uint8_t *buffer, uint8_t iStartBit, uint8_t iLength);
uint32_t GetBitsFromInteger(int buffer, int iStartBit, int iLength);
uint32_t ConvertBCDtoInteger(unsigned char buffer);

#endif
