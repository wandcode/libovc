/*
 * ovdata.c
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

/*
 *
 * TODO: Rewrite (OR... should this be included at all?)
 *
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
   bit_agency_TLS        = 0x00,
   bit_agency_connexxion = 0x01,
   bit_agency_gvb        = 0x02,
   bit_agency_htm        = 0x03,
   bit_agency_ns         = 0x04,
   bit_agency_ret        = 0x05,
   bit_agency_veolia     = 0x07,
   bit_agency_arriva     = 0x08,
   bit_agency_syntus     = 0x09,
   bit_agency_qbuzz      = 0x0A,
   bit_agency_duo        = 0x0C, // Could also be 2C though... ( http://www.ov-chipkaart.de/forum/viewtopic.php?f=10&t=299 )
   bit_agency_store      = 0x19,
   bit_agency_duo_alt    = 0x2C
} bitmask_agency;

char* GetClasstype(uint16_t classtype) {
   switch (classtype) {
      case bit_class_first:  return "1e klas";
      case bit_class_second: return "2e klas";
      default:               return "Unknown";
   }
}

char* GetCompanyName(uint8_t company) {
   switch (company) {
      case bit_agency_TLS:        return "TLS";
      case bit_agency_connexxion: return "Connexxion"; /* or Breng, Hermes, GVU */
      case bit_agency_gvb:        return "GVB";
      case bit_agency_htm:        return "HTM";
      case bit_agency_ns:         return "NS";
      case bit_agency_ret:        return "RET";
      case bit_agency_veolia:     return "Veolia";
      case bit_agency_arriva:     return "Arriva"; /* or Aquabus */
      case bit_agency_syntus:     return "Syntus";
      case bit_agency_qbuzz:      return "Qbuzz";
      case bit_agency_duo:        return "DUO";
      case bit_agency_store:      return "Reseller"; /* used by Albert Heijn, Primera and Hermes busses and maybe even more */
      case bit_agency_duo_alt:    return "DUO";
      default:                    return "Unknown";
   }
}


/* It seems that all the IDs are unique, so why bother with the companies? */
char* GetSubscriptionName(uint16_t subsciption_id) {
   switch (subsciption_id) {
      /* NS */
      case 0x0001: return "NS-Jaarabonnement";
      case 0x0003: return "NS-Jaarabonnement gezinslid";
      case 0x0005: return "OV-jaarkaart";
      case 0x0007: return "OV-Bijkaart";
      case 0x0009: return "NS-Vervoer-legitimatiebewijs";
      case 0x000B: return "NS-Vervoerbewijs";
      case 0x000E: return "NS-Vervoerbewijs Ex.";
      case 0x0011: return "NS Businesscard";
      case 0x0014: return "Jaartrajectabonnement";
      case 0x0016: return "Stad Streek-Jaarabonnement";
      case 0x0019: return "Voordeelurenabonnement (twee jaar)";
      case 0x001A: return "Voordeelurenabonnement";
      case 0x001B: return "Voordeelurenabonnement 60+ doorlopend";
      case 0x001C: return "Voordeelurenabonnement 60+";
      case 0x001D: return "Voordeelurenabonnement";
      case 0x001E: return "Kids Vrij";
      case 0x0028: return "OV-chip Plus";
      case 0x0032: return "Maandabon. Altijd Vrij";
      case 0x0033: return "Maandabon. Altijd Voordeel";
      case 0x0034: return "Maandabon. Altijd Voordeel";
      case 0x0054: return "Railrunner";
      case 0x0055: return "Dagkaart";
      case 0x0057: return "VanAnaarBeter enkele reis";
      case 0x005A: return "Keuzedag";
      case 0x005B: return "Keuzedag VDU";
      case 0x005E: return "Toeslag 2-1 Weekenddag";
      case 0x005F: return "Toeslag 2-1 Dag";
      case 0x0060: return "Toeslag 2-1 Daluren";
      case 0x0061: return "Toeslag 2-1 Keuzedag";
      case 0x0078: return "Dagkaart fiets";
      case 0x0079: return "Dagkaart hond";
      case 0x007A: return "ICE Toeslag enkele reis";
      case 0x00A2: return "Stationsmedewerker pas lokaal";
      case 0x00A3: return "Stationsmedewerker pas landelijk";
      case 0x00AF: return "Studenten OV-chipkaart week (2009)";
      case 0x00B0: return "Studenten OV-chipkaart weekend (2009)";
      case 0x00B1: return "Studentenkaart korting week (2009)";
      case 0x00B2: return "Studentenkaart korting weekend (2009)";
      case 0x00B4: return "NS-Privilegepas/legitimatie";
      case 0x00B5: return "NS-Privilegepas";
      case 0x00B6: return "NS-Privilegepas Junior";
      case 0x00B7: return "NS-Legitimatiebewijs";
      case 0x00B9: return "NS-reisrecht RET personeel";
      case 0x00BA: return "NS-reisrecht GVB personeel";
      case 0x00C9: return "Reizen op saldo bij NS";
      case 0x00CA: return "Reizen op saldo bij NS";
      case 0x00CD: return "Voordeelurenabonnement reizen op saldo";
      case 0x00CE: return "Voordeelurenabonnement reizen op saldo";
      case 0x00CF: return "NS-Business Card Check in/out";
      case 0x00D0: return "NS-Business Card Check in/out";
      case 0x00D1: return "Dal Voordeel";
      case 0x00D2: return "Dal Voordeel";
      case 0x00D3: return "Weekend vrij";
      case 0x00D4: return "Weekend vrij";
      case 0x00D5: return "Dal Vrij";
      case 0x00D6: return "Dal Vrij";
      case 0x00D7: return "Altijd Voordeel";
      case 0x00D8: return "Altijd Voordeel";
      case 0x00DB: return "Altijd Vrij";
      case 0x00DC: return "Altijd Vrij";
      case 0x00E5: return "Reizen op saldo (tijdelijk eerste klas)";
      case 0x00E6: return "Reizen op saldo (tijdelijk tweede klas)";
      case 0x00E7: return "Reizen op saldo (tijdelijk eerste klas korting)";
      case 0x00E8: return "Tijdelijke klassewissel korting";
      case 0x00E9: return "BC vandaag 1e klasse";
      case 0x00EA: return "BC vandaag 2e klasse";
      case 0x00F1: return "rosmobility (Radiuz)";
      case 0x00F2: return "rosmobility (Radiuz)";
      case 0x00F3: return "rosmobility (Mobility Mixx)";
      case 0x00F4: return "rosmobility (Mobility Mixx)";
      case 0x00F5: return "Radiuz vandaag";
      case 0x00F7: return "Mobility Mixx vandaag";
      case 0x0103: return "Klassewissel";
      case 0x0104: return "Klassewissel";
      case 0x0105: return "Klassewissel";
      case 0x0106: return "Klassewissel";
      case 0x0107: return "Klassewissel";
      case 0x0108: return "Klassewissel";
      case 0x010F: return "Samenreiskorting";
      case 0x012B: return "Activeren Kaartservicecode";
      case 0x012D: return "Fyra toeslag enkele reis";
      case 0x012E: return "Fyra Toeslag retour";
      case 0x012F: return "Fyra Maandtoeslag";
      case 0x0130: return "Fyra Altijd Toeslagvrij";
      case 0x0131: return "Fyra Altijd Toeslagvrij";
      case 0x0137: return "Fyra Toeslag enkele reis";
      /* Arriva */
      case 0x059A: return "Dalkorting";
      /* Veolia */
      case 0x0626: return "DALU Dalkorting";
      /* Connexxion */
      case 0x0692: return "Daluren Oost-Nederland";
      case 0x069C: return "Daluren Oost-Nederland";
      /* DUO */
      case 0x09C6: return "Student weekend-vrij";
      case 0x09C7: return "Student week-korting";
      case 0x09C9: return "Student week-vrij";
      case 0x09CA: return "Student weekend-korting";
      /* GVB */
      case 0x0BBD: return "Fietssupplement";
   }
};
