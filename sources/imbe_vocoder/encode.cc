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


#include "typedef.h"
#include "globals.h"
#include "imbe.h"
#include "basic_op.h"
#include "dsp_sub.h"
#include "aux_sub.h"
#include "tbls.h"
#include "encode.h"
#include "dc_rmv.h"
#include "pe_lpf.h"
#include "pitch_est.h"
#include "pitch_ref.h"
#include "v_uv_det.h"
#include "sa_encode.h"
#include "ch_encode.h"
#include "imbe_vocoder.h"



void imbe_vocoder::encode_init(void)
{
	v_zap(pitch_est_buf, PITCH_EST_BUF_SIZE);
	v_zap(pitch_ref_buf, PITCH_EST_BUF_SIZE);
	v_zap(pe_lpf_mem, PE_LPF_ORD);
	pitch_est_init();
	dc_rmv_mem = 0;
	sa_encode_init();
	pitch_ref_init();
}

#include <ti/sysbios/hal/Seconds.h>
#include <ti/sysbios/interfaces/ISeconds.h>
ISeconds_Time ts1, ts2;
ISeconds_Time resultTime1, resultTime2, resultTime3, resultTime4, resultTime5;
void imbe_vocoder::encode(IMBE_PARAM *imbe_param, Word16 *frame_vector, Word16 *snd)
{
	Word16 i;
	Word16 *wr_ptr, *sig_ptr;
	
	Seconds_getTime(&ts1);

	for(i = 0; i < PITCH_EST_BUF_SIZE - FRAME; i++)
	{
		pitch_est_buf[i] = pitch_est_buf[i + FRAME];
		pitch_ref_buf[i] = pitch_ref_buf[i + FRAME];
	}
		
	dc_rmv(snd, &pitch_ref_buf[PITCH_EST_BUF_SIZE - FRAME], &dc_rmv_mem, FRAME);
	pe_lpf(&pitch_ref_buf[PITCH_EST_BUF_SIZE - FRAME], &pitch_est_buf[PITCH_EST_BUF_SIZE - FRAME], pe_lpf_mem, FRAME);

	Seconds_getTime(&ts2);
	resultTime1.secs = (ts2.secs - ts1.secs);
	resultTime1.nsecs = (ts2.nsecs - ts1.nsecs);

	Seconds_getTime(&ts1);

	pitch_est(imbe_param, pitch_est_buf);

	Seconds_getTime(&ts2);
	resultTime2.secs = (ts2.secs - ts1.secs);
	resultTime2.nsecs = (ts2.nsecs - ts1.nsecs);

	Seconds_getTime(&ts1);
    //
	// Speech windowing and FFT calculation
	//
	wr_ptr  = (Word16 *)wr;
	sig_ptr = &pitch_ref_buf[40];
	for(i = 146; i < 256; i++) 
	{
		fft_buf[i].re = mult(*sig_ptr++, *wr_ptr++); 
		fft_buf[i].im = 0;
	}
	fft_buf[0].re = *sig_ptr++;
	fft_buf[0].im = 0;
	wr_ptr--;
	for(i = 1; i < 111; i++) 
	{
		fft_buf[i].re = mult(*sig_ptr++, *wr_ptr--); 
		fft_buf[i].im = 0;
	}
	for(i = 111; i < 146; i++) 
		fft_buf[i].re = fft_buf[i].im = 0;

	Seconds_getTime(&ts2);
	resultTime3.secs = (ts2.secs - ts1.secs);
	resultTime3.nsecs = (ts2.nsecs - ts1.nsecs);

	Seconds_getTime(&ts1);

	fft->directTransform(fft_buf);

	Seconds_getTime(&ts2);
	resultTime4.secs = (ts2.secs - ts1.secs);
	resultTime4.nsecs = (ts2.nsecs - ts1.nsecs);

	Seconds_getTime(&ts1);

	pitch_ref(imbe_param, fft_buf);
	v_uv_det(imbe_param, fft_buf);
	sa_encode(imbe_param);
	encode_frame_vector(imbe_param, frame_vector);

	Seconds_getTime(&ts2);
	resultTime5.secs = (ts2.secs - ts1.secs);
	resultTime5.nsecs = (ts2.nsecs - ts1.nsecs);

}
