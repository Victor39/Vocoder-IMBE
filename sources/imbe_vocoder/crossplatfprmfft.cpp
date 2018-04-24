#include <string.h>
#include "crossplatfprmfft.h"
#include "math_sub.h"
#include <arithmetic.h>


CrossplatfprmFft::CrossplatfprmFft () {

	Word16 i, fft_len2, shift, step, theta;

	memset(m_wrArray, 0, sizeof(m_wrArray));
	memset(m_wiArray, 0, sizeof(m_wiArray));

	fft_len2 = shr(FFT_LENGTH, 1);
	shift = norm_s(fft_len2);
	step = shl(2, shift);
	theta = 0;

	for (i = 0; i <= fft_len2; i++) {
		m_wrArray[i] = cos_fxp(theta);
		m_wiArray[i] = sin_fxp(theta);
		if (i >= (fft_len2 - 1))
			theta = ONE_Q15;
		else
			theta = add(theta, step);
	}
}

CrossplatfprmFft::~CrossplatfprmFft () {
}

void CrossplatfprmFft::directTransform (Cmplx16* data) const {

	transform((Word16 *) data, 1);
}

void CrossplatfprmFft::inverseTransform (Cmplx16* data) const {

	transform((Word16 *) data, -1);
}


//  	Subroutine FFT: Fast Fourier Transform
// ***************************************************************
// * Replaces data by its DFT, if isign is 1, or replaces data   *
// * by inverse DFT times nn if isign is -1.  data is a complex  *
// * array of length nn, input as a real array of length 2*nn.   *
// * nn MUST be an integer power of two.  This is not checked    *
// * The real part of the number should be in the zeroeth        *
// * of data , and the imaginary part should be in the next      *
// * element.  Hence all the real parts should have even indeces *
// * and the imaginary parts, odd indeces.			             *
// *                                                             *
// * Data is passed in an array starting in position 0, but the  *
// * code is copied from Fortran so uses an internal pointer     *
// * which accesses position 0 as position 1, etc.		         *
// *                                                             *
// * This code uses e+jwt sign convention, so isign should be    *
// * reversed for e-jwt.                                         *
// ***************************************************************
//
// Q values:
// datam1 - Q14
// isign  - Q15

#define	SWAP(a,b) temp1 = (a);(a) = (b); (b) = temp1

void CrossplatfprmFft::transform (Word16 * data, Word16 isign) const{

	Word16 n, mmax, m, j, istep, i;
	Word16 wr, wi, temp1;
	Word32 L_tempr, L_tempi;
	Word16 * pdata;
	Word32 L_temp1, L_temp2;
	Word16 index, index_step;

	//  Use pointer indexed from 1 instead of 0
	pdata = &data[-1];

	n = shl(FFT_LENGTH, 1);
	j = 1;
	for (i = 1; i < n; i += 2) {
		if (j > i) {
			SWAP(pdata[j], pdata[i]);
			SWAP(pdata[j + 1], pdata[i + 1]);
		}
		m = FFT_LENGTH;
		while (m >= 2 && j > m) {
			j = sub(j, m);
			m = shr(m, 1);
		}
		j = add(j, m);
	}
	mmax = 2;

	// initialize index step
	index_step = FFT_LENGTH;

	while (n > mmax) {
		istep = shl(mmax, 1);  // istep = 2 * mmax

		index = 0;
		index_step = shr(index_step, 1);

		wr = ONE_Q15;
		wi = 0;
		for (m = 1; m < mmax; m += 2) {
			for (i = m; i <= n; i += istep) {
				j = i + mmax;

				// tempr = wr * data[j] - wi * data[j+1]
				L_temp1 = L_shr(L_mult(wr, pdata[j]), 1);
				L_temp2 = L_shr(L_mult(wi, pdata[j + 1]), 1);
				L_tempr = L_sub(L_temp1, L_temp2);

				// tempi = wr * data[j+1] + wi * data[j]
				L_temp1 = L_shr(L_mult(wr, pdata[j + 1]), 1);
				L_temp2 = L_shr(L_mult(wi, pdata[j]), 1);
				L_tempi = L_add(L_temp1, L_temp2);

				// data[j] = data[i] - tempr
				L_temp1 = L_shr(L_deposit_h(pdata[i]), 1);
				pdata[j] = round(L_sub(L_temp1, L_tempr));

				// data[i] += tempr
				pdata[i] = round(L_add(L_temp1, L_tempr));

				// data[j+1] = data[i+1] - tempi
				L_temp1 = L_shr(L_deposit_h(pdata[i + 1]), 1);
				pdata[j + 1] = round(L_sub(L_temp1, L_tempi));

				// data[i+1] += tempi
				pdata[i + 1] = round(L_add(L_temp1, L_tempi));
			}
			index = add(index, index_step);
			wr = m_wrArray[index];
			if (isign < 0)
				wi = negate(m_wiArray[index]);
			else
				wi = m_wiArray[index];
		}
		mmax = istep;
	}

}
