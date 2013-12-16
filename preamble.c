/*
 * preamble.c
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

void GetPreamble(unsigned char *buffer, ov_preamble *ovPreamble) {
   ovPreamble->id = ((buffer[0] & 0xFF) << 24) | ((buffer[1] & 0xFF) << 16) | ((buffer[2] & 0xFF) << 8) | ((buffer[3] & 0xFF));
   ovPreamble->checkbit = (buffer[4] & 0xFF);
   ovPreamble->manufacturer = ((uint64_t)(buffer[5] & 0xFF) << 32) | ((buffer[6] & 0xFF) << 24) | ((buffer[7] & 0xFF) << 16) | ((buffer[8] & 0xFF) << 8) | ((buffer[9] & 0xFF));
   ovPreamble->publisher = ((uint64_t)(buffer[10] & 0xFF) << 40) | ((uint64_t)(buffer[11] & 0xFF) << 32) | ((buffer[12] & 0xFF) << 24) | ((buffer[13] & 0xFF) << 16) |  ((buffer[14] & 0xFF) << 8) | ((buffer[15] & 0xFF));
   ovPreamble->unknownConstant1 = 0; /* 88 bits -- too big to store and unknown, so null it for now */
   ovPreamble->expdate = ((buffer[27] & 0xFF) << 12) | ((buffer[28] & 0xFF) << 4) | ((buffer[29] & 0xF0) >> 4);
   ovPreamble->unknownConstant2 = ((uint64_t)(buffer[29] & 0x0F) << 32) | ((buffer[30] & 0xFF) << 24) | ((buffer[31] & 0xFF) << 16) | ((buffer[32] & 0xFF) << 8) |  (buffer[33] & 0xFF);
   ovPreamble->issuer = ((buffer[34] & 0xF0) >> 4);
   ovPreamble->type = (buffer[34] & 0x0F);
}
