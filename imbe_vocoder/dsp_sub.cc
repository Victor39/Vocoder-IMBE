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

//#define ENABLE_OPTIMIZATION

#include "typedef.h"
#include "basic_op.h"
#include "imbe.h"
#include "tbls.h"
#include "dsp_sub.h"
#include "math_sub.h"
#include "encode.h"
#include "imbe_vocoder.h"


//-----------------------------------------------------------------------------
//	PURPOSE:
//				Perform inverse DCT
//
//
//  INPUT:
//              in     -  pointer to input data
//              m_lim  -  input data's size
//              i_lim  -  result's size
//              out    -  pointer to save result 
//
//	OUTPUT:
//		None
//
//	RETURN:
//		        Saved in out result of conversion
//
//-----------------------------------------------------------------------------
void imbe_vocoder::idct(Word16 *in, Word16 m_lim, Word16 i_lim, Word16 *out)
{
	UWord16 angl_step, angl_intl, angl_intl_2;
	UWord16 angl_acc;
	Word32  sum;
	Word16  i, m;

	if(m_lim == 1)
	{
		angl_intl   = CNST_0_5_Q1_15;
		angl_intl_2 = CNST_1_0_Q1_15;
	}
	else
	{
		angl_intl   = div_s ((Word16) CNST_0_5_Q5_11, m_lim << 11); // calculate 0.5/m_lim
		angl_intl_2 = shl(angl_intl, 1);
	}

	angl_step = angl_intl;
	for(i = 0; i < i_lim; i++)
	{
		sum = 0;
		angl_acc = angl_step;
		for(m = 1; m < m_lim; m++)
		{
			sum = L_add(sum, L_shr( L_mult(in[m], cos_fxp(angl_acc)), 7));
			angl_acc += angl_step;			
		}
		sum = L_add(sum, L_shr( L_deposit_h(in[0]), 8));
		out[i] = extract_l(L_shr_r (sum, 8)); 
		angl_step += angl_intl_2; 
	}
}

//-----------------------------------------------------------------------------
//	PURPOSE:
//				Perform DCT
//
//
//  INPUT:
//              in     -  pointer to input data
//              m_lim  -  input data's size
//              i_lim  -  result's size
//              out    -  pointer to save result 
//
//	OUTPUT:
//		None
//
//	RETURN:
//		        Saved in out result of conversion
//
//-----------------------------------------------------------------------------
void imbe_vocoder::dct(Word16 *in, Word16 m_lim, Word16 i_lim, Word16 *out)
{
	UWord16 angl_step, angl_intl, angl_intl_2, angl_begin;
	UWord16 angl_acc;
	Word32  sum;
	Word16  i, m;

	if(m_lim == 1)
	{
		angl_intl   = CNST_0_5_Q1_15;
		angl_intl_2 = CNST_1_0_Q1_15;
	}
	else
	{
		angl_intl   = div_s ((Word16) CNST_0_5_Q5_11, m_lim << 11); // calculate 0.5/m_lim
		angl_intl_2 = shl(angl_intl, 1);
	}

	// Calculate first coefficient
	sum = 0;
	for(m = 0; m < m_lim; m++)
		sum = L_add(sum, L_deposit_l(in[m]));
	out[0] = extract_l(L_mpy_ls(sum, angl_intl_2));

	// Calculate the others coefficients
	angl_begin = angl_intl;
	angl_step  = angl_intl_2;
	for(i = 1; i < i_lim; i++)
	{
		sum = 0;
		angl_acc = angl_begin;
		for(m = 0; m < m_lim; m++)
		{
			sum = L_add(sum, L_deposit_l(mult(in[m], cos_fxp(angl_acc))));
			angl_acc += angl_step;			
		}
		out[i] = extract_l(L_mpy_ls(sum, angl_intl_2));

		angl_step  += angl_intl_2;  
		angl_begin += angl_intl;
	}
}

