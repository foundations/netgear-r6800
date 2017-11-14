/*

    File: referral.h
    
    Copyright (C) 2010 sErCoMm

    This source is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2, or (at your option)
    any later version.

    This source is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#ifndef _DNRD_REFERRAL_H_
#define _DNRD_REFERRAL_H_

#define PICKUP_OK       0
#define PICKUP_FAIL     -1


#define MAX_RECURSION_COUNT 2

int pickup_referral(void *msg, int msg_len, char *server, int len);

#endif /* _DNRD_CHECK_H_ */
