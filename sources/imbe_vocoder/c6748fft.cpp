#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "c6748fft.h"

extern "C"
#include "DSP_fft16x16t.h"

#ifndef PI
# ifdef M_PI
#  define PI M_PI
# else
#  define PI 3.14159265358979323846
# endif
#endif

/* ======================================================================== */
/*  D2S -- Truncate a 'double' to a 'short', with clamping.                 */
/* ======================================================================== */
short C6748Fft::d2s (double d) {
	if (d >= 32767.0)
		return 32767;
	if (d <= -32768.0)
		return -32768;
	return (short) d;
}

/* ======================================================================== */
/*  GEN_TWIDDLE -- Generate twiddle factors for TI's custom FFTs.           */
/*                                                                          */
/*  USAGE                                                                   */
/*      This routine is called as follows:                                  */
/*                                                                          */
/*          int gen_twiddle(short *w, int n, double scale)                  */
/*                                                                          */
/*          short  *w     Pointer to twiddle-factor array                   */
/*          int    n      Size of FFT                                       */
/*          double scale  Scale factor to apply to values.                  */
/*                                                                          */
/*      The routine will generate the twiddle-factors directly into the     */
/*      array you specify.  The array needs to be approximately 2*N         */
/*      elements long.  (The actual size, which is slightly smaller, is     */
/*      returned by the function.)                                          */
/* ======================================================================== */
int C6748Fft::gen_twiddle (short *w, int n, double scale) {

	int i, j, k;
	for (j = 1, k = 0; j < n >> 2; j = j << 2) {
		for (i = 0; i < n >> 2; i += j << 1) {
			w[k + 11] = d2s(scale * cos(6.0 * PI * (i + j) / n));
			w[k + 10] = d2s(scale * sin(6.0 * PI * (i + j) / n));
			w[k + 9] = d2s(scale * cos(6.0 * PI * (i) / n));
			w[k + 8] = d2s(scale * sin(6.0 * PI * (i) / n));

			w[k + 7] = d2s(scale * cos(4.0 * PI * (i + j) / n));
			w[k + 6] = d2s(scale * sin(4.0 * PI * (i + j) / n));
			w[k + 5] = d2s(scale * cos(4.0 * PI * (i) / n));
			w[k + 4] = d2s(scale * sin(4.0 * PI * (i) / n));

			w[k + 3] = d2s(scale * cos(2.0 * PI * (i + j) / n));
			w[k + 2] = d2s(scale * sin(2.0 * PI * (i + j) / n));
			w[k + 1] = d2s(scale * cos(2.0 * PI * (i) / n));
			w[k + 0] = d2s(scale * sin(2.0 * PI * (i) / n));

			k += 12;
		}
	}

	return k;
}

C6748Fft::C6748Fft () {

	m_inFftBuffer = (Cmplx16 *) memalign(8, FFT_LENGTH * sizeof(Cmplx16));
	m_outFftBuffer = (Cmplx16 *) memalign(8, FFT_LENGTH * sizeof(Cmplx16));
	m_twiddleFactors = (Cmplx16 *) memalign(8, FFT_LENGTH * sizeof(Cmplx16));

	/* -------------------------------------------------------------------- */
	/*  Generate the twiddle-factor array.                                  */
	/* -------------------------------------------------------------------- */
	double scale = 32767.5;
	int size = gen_twiddle((Word16*) m_twiddleFactors, FFT_LENGTH, scale);
}

C6748Fft::~C6748Fft () {

	free(m_twiddleFactors);
	free(m_outFftBuffer);
	free(m_inFftBuffer);
}

void C6748Fft::directTransform (Cmplx16* data) const {

	for (int i = 0; i < FFT_LENGTH; i++) {
		m_inFftBuffer[i].re = data[i].re / FFT_LENGTH;
		m_inFftBuffer[i].im = data[i].im / FFT_LENGTH;
	}

	DSP_fft16x16t((Word16*) m_twiddleFactors, FFT_LENGTH, (Word16*) m_inFftBuffer, (Word16*) m_outFftBuffer);
	memcpy(data, m_outFftBuffer, FFT_LENGTH * sizeof(Cmplx16));
}

void C6748Fft::inverseTransform (Cmplx16* data) const {
}
