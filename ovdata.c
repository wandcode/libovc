/*
 * ovdata.c
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

typedef enum bitmask_class {
   bit_class_first  = 0x140,
   bit_class_second = 0x180
} bitmask_class;

typedef enum bitmask_process {
   bit_process_puchase  = 0x00,
   bit_process_checkin  = 0x01,
   bit_process_checkout = 0x02,
   bit_process_transfer = 0x06,
   bit_process_banned   = 0x07,
   bit_process_credit   = -0x02,
   bit_process_nodata   = -0x03
} bitmask_process;

typedef enum bitmask_agency {
   bit_agency_tls        = 0x00,
   bit_agency_connexxion = 0x01,
   bit_agency_gvb        = 0x02,
   bit_agency_htm        = 0x03,
   bit_agency_ns         = 0x04,
   bit_agency_ret        = 0x05,
   bit_agency_veolia     = 0x07,
   bit_agency_arriva     = 0x08,
   bit_agency_syntus     = 0x09,
   bit_agency_qbuzz      = 0x0A,
   bit_agency_duo        = 0x0C, /* or 0x2C */
   bit_agency_store      = 0x19,
   bit_agency_duo_alt    = 0x2C
} bitmask_agency;

/* Get the class type */
void GetClasstype(uint16_t classtype, char *out) {
   switch (classtype) {
      case bit_class_first:  sprintf(out, "1e klas"); return;
      case bit_class_second: sprintf(out, "2e klas"); return;
      default:               sprintf(out, "Unknown %i", classtype); return;
   }
}

/* Get the company name */
void GetCompanyName(uint8_t company, char *out) {
   switch (company) {
      case bit_agency_tls:        sprintf(out, "TLS"); return;
      case bit_agency_connexxion: sprintf(out, "Connexxion"); return; /* or Breng, Hermes, GVU */
      case bit_agency_gvb:        sprintf(out, "GVB"); return;
      case bit_agency_htm:        sprintf(out, "HTM"); return;
      case bit_agency_ns:         sprintf(out, "NS"); return;
      case bit_agency_ret:        sprintf(out, "RET"); return;
      case bit_agency_veolia:     sprintf(out, "Veolia"); return;
      case bit_agency_arriva:     sprintf(out, "Arriva"); return; /* or Aquabus */
      case bit_agency_syntus:     sprintf(out, "Syntus"); return;
      case bit_agency_qbuzz:      sprintf(out, "Qbuzz"); return;
      case bit_agency_duo:        sprintf(out, "DUO"); return;
      case bit_agency_store:      sprintf(out, "Reseller"); return; /* used by Albert Heijn, Primera and Hermes busses and maybe even more */
      case bit_agency_duo_alt:    sprintf(out, "DUO"); return;
      default:                    sprintf(out, "Unknown %i", company); return;
   }
}

/* Number of days in a month */
uint16_t DaysInMonth[12] = {
   31, /* januari */
   28, /* februari */
   31, /* march */
   30, /* april */
   31, /* may */
   30, /* june */
   31, /* july */
   31, /* august */
   30, /* september */
   31, /* october */
   30, /* november */
   31, /* december */
};

/* Is the given year a leap year? */
uint16_t is_leapyear(uint16_t year) {
   return (year % 4) ? 0 : (!(year % 400)) ? 1 : (!(year % 100)) ? 0 : 1;
}

/* Add days since 1997-1-1 */
void GetDateSince1997(uint16_t days, char *out) {
   uint16_t iYear = 1997;
   uint16_t iLeap = 0;
   uint16_t iMonth = 0;

   while (days >= 365 + iLeap) {
      days -= 365 + iLeap;
      iYear++;
      iLeap = is_leapyear(iYear);
   }

   while (days >= DaysInMonth[iMonth] + ((iMonth == 2 && iLeap)?1:0)) {
      days -= DaysInMonth[iMonth] + ((iMonth == 2 && iLeap)?1:0);
      iMonth++;
   }

   sprintf(out, "%04u-%02u-%02u", iYear, iMonth + 1, days + 1);
}

void GetTime(uint16_t time, char *out) {
   sprintf(out, "%02u:%02u", time / 60, time % 60);
}
