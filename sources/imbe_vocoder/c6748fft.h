/*
 * c6748fft.h
 *
 *  Created on: 19 џэт. 2018 у.
 *      Author: Alsheuski
 */

#ifndef IMBE_VOCODER_C6748FFT_H_
#define IMBE_VOCODER_C6748FFT_H_

#include "fft.h"

class C6748Fft : public Fft {
public:
	C6748Fft ();
	virtual ~C6748Fft ();

	virtual void directTransform (Cmplx16 * data) const;
	virtual void inverseTransform (Cmplx16 * data) const;

private:
	Cmplx16 * m_inFftBuffer;
	Cmplx16 * m_outFftBuffer;
	Cmplx16 * m_twiddleFactors;
	short d2s(double d);
	int gen_twiddle(short *w, int n, double scale);
};

#endif /* IMBE_VOCODER_C6748FFT_H_ */
