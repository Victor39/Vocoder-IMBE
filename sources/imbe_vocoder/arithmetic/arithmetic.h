#ifndef ARITHMETIC_H_
#define ARITHMETIC_H_
/*
 * Project 25 IMBE Encoder/Decoder Fixed-Point implementation
 * Developed by Pavel Yazev E-mail: pyazev@gmail.com
 * Version 1.0 (c) Copyright 2009
 * 
 * This is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * The software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this; see the file COPYING.  If not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Boston, MA
 * 02110-1301, USA.
 */
/*___________________________________________________________________________
 |                                                                           |
 |   Constants and Globals                                                   |
 |                                                                           |
 | $Id $
 |___________________________________________________________________________|
*/
extern Flag Overflow;
extern Flag Carry;

#define MAX_32 (Word32)0x7fffffffL
#define MIN_32 (Word32)0x80000000L

#define MAX_16 (Word16)0x7fff
#define MIN_16 (Word16)0x8000


/*___________________________________________________________________________
 |                                                                           |
 |   Prototypes for basic arithmetic operators                               |
 |___________________________________________________________________________|
*/

/* Short add, 1 */
Word16 add (const Word16 var1, const Word16 var2);

/* Short sub, 1 */
Word16 sub (const Word16 var1, const Word16 var2);

/* Short abs, 1 */
Word16 abs_s (const Word16 var1);

/* Short shift left, 1 */
Word16 shl (const Word16 var1, const Word16 var2);

/* Short shift right, 1 */
Word16 shr (const Word16 var1, const Word16 var2);

/* Short mult, 1 */
Word16 mult (const Word16 var1, const Word16 var2);

/* Long mult, 1 */
Word32 L_mult (const Word16 var1, const Word16 var2);

/* Short negate, 1 */
Word16 negate (const Word16 var1);

/* Extract high, 1 */
Word16 extract_h (const Word32 L_var1);

/* Extract low, 1 */
Word16 extract_l (const Word32 L_var1);

/* Round, 1 */
Word16 round (const Word32 L_var1);

/* Mac, 1 */
Word32 L_mac (const Word32 L_var3, const Word16 var1, const Word16 var2);

/* Msu, 1 */
Word32 L_msu (const Word32 L_var3, const Word16 var1, const Word16 var2);

/* Mac without sat, 1 */
Word32 L_macNs (const Word32 L_var3, const Word16 var1, const Word16 var2);

/* Msu without sat, 1 */
Word32 L_msuNs (const Word32 L_var3, const Word16 var1, const Word16 var2);

/* Long add, 2 */
Word32 L_add (const Word32 L_var1, const Word32 L_var2);

/* Long sub, 2 */
Word32 L_sub (const Word32 L_var1, const Word32 L_var2);

/* Long add with c, 2 */
Word32 L_add_c (const Word32 L_var1, const Word32 L_var2);

/* Long sub with c, 2 */
Word32 L_sub_c (const Word32 L_var1, const Word32 L_var2);

/* Long negate, 2 */
Word32 L_negate (const Word32 L_var1);

/* Mult with round, 2 */
Word16 mult_r (const Word16 var1, const Word16 var2);

/* Long shift left, 2 */
Word32 L_shl (const Word32 L_var1, const Word16 var2);

/* Long shift right, 2 */
Word32 L_shr (const Word32 L_var1, const Word16 var2);

/* Shift right with round, 2 */
Word16 shr_r (const Word16 var1, const Word16 var2);

/* Mac with rounding, 2 */
Word16 mac_r (const Word32 L_var3, const Word16 var1, const Word16 var2);

/* Msu with rounding, 2 */
Word16 msu_r (const Word32 L_var3, const Word16 var1, const Word16 var2);

/* 16 bit var1 -> MSB, 2 */
Word32 L_deposit_h (const Word16 var1);

/* 16 bit var1 -> LSB, 2 */
Word32 L_deposit_l (const Word16 var1);

/* Long shift right with round, 3 */
Word32 L_shr_r (const Word32 L_var1, const Word16 var2);

/* Long abs, 3 */
Word32 L_abs (const Word32 L_var1);

/* Long saturation, 4 */
Word32 L_sat (const Word32 L_var1);

/* Short norm, 15 */
Word16 norm_s (const Word16 var1);

/* Short division, 18 */
Word16 div_s (const Word16 var1, const Word16 var2);

/* Long norm, 30 */
Word16 norm_l (const Word32 L_var1);

#endif /* ARITHMETIC_H_ */
