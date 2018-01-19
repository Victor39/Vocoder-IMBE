#ifndef IMBE_VOCODER_CROSSPLATFPRMFFT_H_
#define IMBE_VOCODER_CROSSPLATFPRMFFT_H_

#include "fft.h"

class CrossplatfprmFft : public Fft {
public:
	CrossplatfprmFft ();
	virtual ~CrossplatfprmFft ();

	virtual void directTransform (Cmplx16 * data) const;
	virtual void inverseTransform (Cmplx16 * data) const;

private:
	Word16 m_wrArray[FFT_LENGTH / 2 + 1];
	Word16 m_wiArray[FFT_LENGTH / 2 + 1];

	void transform(Word16 *data, Word16 isign) const;
};

#endif /* IMBE_VOCODER_CROSSPLATFPRMFFT_H_ */
